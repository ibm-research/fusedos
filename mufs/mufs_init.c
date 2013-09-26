/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */
/* The pseudo-FS boilerplate code is derived from LFS               */
/* (http://lwn.net/Articles/57371/)                                 */
/* written by Jonathan Corbet.                                      */
/*                                                                  */
/* ================================================================ */

/*
 * MUFS
 */

#include "mufs.h"

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/kdev_t.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/pagemap.h>
#include <linux/blkdev.h>

#include <linux/device.h>

#include <asm/uaccess.h>

#include "hwi/include/bqc/PhysicalMap.h"
#include "spi/include/kernel/MU.h"
#include "spi/include/mu/Reset.h"
#include "hwi/include/bqc/mu_dcr.h"

#include "get_personality_of.h"

MODULE_AUTHOR("Marius Hillenbrand <mhillen@us.ibm.com>");
MODULE_LICENSE("GPL");

#define MUFS_MAGIC 0x20120731

static mufs_data_t * mufs_data = NULL;

void * mufs_priv_base   = NULL;
void * mufs_unpriv_base = NULL;

#define USE_MMIO_AREA
#ifdef USE_MMIO_AREA
void* mmio_nonpriv_base = NULL;
void* mmio_priv_base    = NULL;
#endif

// module parameter to reset MU during initialization
static int reset_mu = 0;
module_param(reset_mu, bool, S_IRUGO);


// query file operation functions
extern ssize_t inj_fifo_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset);
extern ssize_t rec_fifo_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset);
extern ssize_t bat_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset);

extern ssize_t coll_classroute_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset);
extern ssize_t gi_classroute_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset);

// allocation and deallocation file operation functions
extern int     inj_fifo_open (struct inode *i, struct file *filp);
extern ssize_t inj_fifo_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset);

extern int     rec_fifo_open (struct inode *i, struct file *filp);

extern int inj_fifo_release (struct inode * i, struct file *filp);
extern int rec_fifo_release (struct inode * i, struct file *filp);

extern int bat_entry_open   (struct inode *i, struct file *filp);
extern int bat_entry_release(struct inode *i, struct file *filp);

extern int coll_classroute_open (struct inode *i, struct file *filp);
extern int coll_classroute_release (struct inode * i, struct file *filp);

extern int gi_classroute_open (struct inode *i, struct file *filp);
extern int gi_classroute_release (struct inode * i, struct file *filp);

// reception FIFO enabling/disabling function
extern ssize_t rec_fifo_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset);

// class route setup and access functions
extern ssize_t coll_classroute_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset);
extern ssize_t gi_classroute_write  (struct file * filp, const char __user * buf, size_t len, loff_t * offset);
extern ssize_t gi_classroute_read   (struct file *filp, char __user * buf, size_t len, loff_t * offset);

// FIFO interrupt threshold setting functions
extern ssize_t inj_fifo_threshold_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset);
extern ssize_t rec_fifo_threshold_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset);

// Primordial class route query functions
extern ssize_t primordial_cr_read(struct file *filp, char __user * buf, size_t len, loff_t * offset);


// file_operations for the files in MUFS
static struct file_operations mufs_inj_fifo_query_fops = {
    .read = inj_fifo_query_read,
};

static struct file_operations mufs_rec_fifo_query_fops = {
    .read = rec_fifo_query_read,
};

static struct file_operations mufs_inj_fifo_fops = {
    .open    = inj_fifo_open,
    .write   = inj_fifo_write,
    .release = inj_fifo_release,
};

static struct file_operations mufs_rec_fifo_fops = {
    .open    = rec_fifo_open,
    .write   = rec_fifo_write,
    .release = rec_fifo_release,
};

static struct file_operations mufs_bat_query_fops = {
    .read = bat_query_read,
};

static struct file_operations mufs_bat_entry_fops = {
    .open = bat_entry_open,
    .release = bat_entry_release,
};

static struct file_operations mufs_coll_classroutes_query_fops = {
    .read = coll_classroute_query_read,
};

static struct file_operations mufs_gi_classroutes_query_fops = {
    .read = gi_classroute_query_read,
};

static struct file_operations mufs_coll_classroute_fops = {
    .open    = coll_classroute_open,
    .write   = coll_classroute_write,
    .release = coll_classroute_release,
};

static struct file_operations mufs_gi_classroute_fops = {
    .open    = gi_classroute_open,
    .read    = gi_classroute_read,
    .release = gi_classroute_release,
    .write   = gi_classroute_write,
};

static struct file_operations inj_fifo_threshold_fops = {
    .write = inj_fifo_threshold_write,
};

static struct file_operations rec_fifo_threshold_fops = {
    .write = rec_fifo_threshold_write,
};

static struct file_operations primordial_cr_fops = {
    .read = primordial_cr_read,
};

static struct inode *mufs_make_inode(struct super_block *sb, int mode) {

    struct inode *ret = new_inode(sb);

    if(ret) {
        ret->i_ino = get_next_ino();
        ret->i_mode = mode;
        ret->i_uid = ret->i_gid = 0;
        ret->i_blkbits = blksize_bits(PAGE_CACHE_SIZE);
        ret->i_blocks = 0;
        ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
        if( (mode & S_IFMT) == S_IFDIR)
            inc_nlink(ret);
    }
    return ret;
}

static struct dentry * mufs_create_file(struct super_block *sb, struct dentry *dir,
                                        const char *name, struct file_operations *fops, void *data) {
    struct dentry *dentry;
    struct inode *inode;

    dentry = d_alloc_name(dir, name);

    inode = mufs_make_inode(sb, S_IFREG | 0644);
    if(!inode)
        goto out_dput;

    inode->i_fop = fops;
    inode->i_private = data;

    d_add(dentry, inode);

    return dentry;

out_dput:
    dput(dentry);
    return 0;
}

/*
 * Create a directory which can be used to hold files.  This code is
 * almost identical to the "create file" logic, except that we create
 * the inode with a different mode, and use the libfs "simple" operations.
 */
static struct dentry *mufs_create_dir (struct super_block *sb,
                                       struct dentry *parent, const char *name) {
    struct dentry *dentry;
    struct inode *inode;

    dentry = d_alloc_name(parent, name);
    if (! dentry)
        goto out;

    inode = mufs_make_inode(sb, S_IFDIR | 0644);
    if (! inode)
        goto out_dput;
    inode->i_op = &simple_dir_inode_operations;
    inode->i_fop = &simple_dir_operations;

    d_add(dentry, inode);

    return dentry;

out_dput:
    dput(dentry);
out:
    return 0;
}

static void mufs_create_classroute_files( struct super_block *sb, struct dentry *dir,
                                        mufs_data_t *data) {
    int i;
    char name[8];

    mufs_create_file(sb, dir, "coll_query",    &mufs_coll_classroutes_query_fops, mufs_data);
    mufs_create_file(sb, dir, "gi_query",      &mufs_gi_classroutes_query_fops,   mufs_data);
    mufs_create_file(sb, dir, "primordial_cr", &primordial_cr_fops,               mufs_data);

    for(i=0; i < BGQ_COLL_CLASS_MAX_CLASSROUTES; i++) {
        snprintf(name, sizeof(name), "coll_%02d", i);
        mufs_create_file(sb, dir, name, &mufs_coll_classroute_fops, &(data->coll_classroutes[i]));
    }

    for(i=0; i < BGQ_GI_CLASS_MAX_CLASSROUTES; i++) {
        snprintf(name, sizeof(name), "gi_%02d", i);
        mufs_create_file(sb, dir, name, &mufs_gi_classroute_fops, &(data->gi_classroutes[i]));
    }
}

static void mufs_create_subgroup_files( struct super_block *sb, struct dentry *dir,
                                        mufs_subgroup_t *subgroup_data)
{
    char name[12];
    int i;

    // create query files within subgroup directory
    mufs_create_file(sb, dir, "injFifo_query", &mufs_inj_fifo_query_fops, subgroup_data);
    mufs_create_file(sb, dir, "recFifo_query", &mufs_rec_fifo_query_fops, subgroup_data);
    mufs_create_file(sb, dir, "bat_query", &mufs_bat_query_fops, subgroup_data);

    // create inj fifo files
    for(i=0; i < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP; i++) {
        snprintf(name, 11, "injFifo_%02d", i);
        mufs_create_file(sb, dir, name, &mufs_inj_fifo_fops, &(subgroup_data->inj_fifos[i]));
    }

    // create rec fifo files
    for(i=0; i < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; i++) {
        snprintf(name, 11, "recFifo_%02d", i);
        mufs_create_file(sb, dir, name, &mufs_rec_fifo_fops, &(subgroup_data->rec_fifos[i]));
    }

    // create base address files
    for(i=0; i < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP; i++) {
        snprintf(name, 11, "bat_%02d", i);
        mufs_create_file(sb, dir, name, &mufs_bat_entry_fops, &(subgroup_data->base_addresses[i]));
    }
}

static void mufs_create_root_files( struct super_block *sb, struct dentry *root)
{

    int i;
    char name[11];
    struct dentry *subgroupdir;
    struct dentry *classroutedir;

    // create subgroup directories 0..67
    for(i=0; i < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE; i++) {
        snprintf(name, 10, "subgrp_%02d", i);
        subgroupdir = mufs_create_dir(sb, root, name);

        mufs_create_subgroup_files(sb, subgroupdir, &(mufs_data->subgroups[i]));
    }

    classroutedir = mufs_create_dir(sb, root, "classroute");
    mufs_create_classroute_files(sb, classroutedir, mufs_data);

    // create FIFO threshold files here
    mufs_create_file(sb, root, "inj_fifo_threshold", &inj_fifo_threshold_fops, mufs_data);
    mufs_create_file(sb, root, "rec_fifo_threshold", &rec_fifo_threshold_fops, mufs_data);
}


static struct super_operations mufs_s_ops = {
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};

static int mufs_fill_super(struct super_block * sb, void *data, int silent)
{

    // derived from LWN.net LFS
    struct inode *root;
    struct dentry *root_dentry;

    sb->s_blocksize      = PAGE_CACHE_SIZE;
    sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
    sb->s_magic          = MUFS_MAGIC;
    sb->s_op             = &mufs_s_ops;
    /*
     * We need to conjure up an inode to represent the root directory
     * of this filesystem.  Its operations all come from libfs, so we
     * don't have to mess with actually *doing* things inside this
     * directory.
     */
    root = mufs_make_inode (sb, S_IFDIR | 0755);
    if (! root)
        goto out;
    root->i_op = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;

    set_nlink(root, 2);

    /*
     * Get a dentry to represent the directory in core.
     */
    root_dentry = d_make_root(root);
    if (! root_dentry)
        goto out_iput;
    sb->s_root = root_dentry;

    dget_dlock(root_dentry); // increase counter to avoid it reaching -1 on unmount

    mufs_create_root_files (sb, root_dentry);
    return 0;

out_iput:
    iput(root);
out:
    return -ENOMEM;
}

static int mufs_set_super(struct super_block *s, void *data)
{
    s->s_fs_info = data;
    return set_anon_super(s, data);
}

static struct dentry *mufs_mount(struct file_system_type *fst,
                                 int flags, const char * devname,void * data) {
    int err;
    struct super_block *sb = sget(fst, NULL, mufs_set_super, data);
    err = mufs_fill_super(sb, data, 0);
    if(err<0)
        return ERR_PTR(err);

    return sb->s_root;
}

static struct file_system_type mufs_type = {
    .owner = THIS_MODULE,
    .name  = "mufs",
    .mount = mufs_mount,
    .kill_sb = kill_litter_super,
};

// debuggin cruft
#if 0
#define PRINT_SIZEOF(TYPE) do{ printk( KERN_ERR "sizeof "#TYPE": %lu\n", sizeof(TYPE)); } while(0);

static void print_mufs_sizeofs(void)
{

    printk(KERN_ERR "MUFS struct sizes:\n");
    PRINT_SIZEOF(mufs_fifo_t);
    PRINT_SIZEOF(mufs_base_address_t);
    PRINT_SIZEOF(mufs_subgroup_t);
    PRINT_SIZEOF(mufs_data_t);

    PRINT_SIZEOF(Kernel_InjFifoAttributes_t);
    PRINT_SIZEOF(MUSPI_InjFifoSubGroup_t);

    PRINT_SIZEOF(MUSPI_RecFifoSubGroup_t);
    PRINT_SIZEOF(Kernel_RecFifoAttributes_t);
}
#endif


// do a MU reset (if requested by parameter)
//     (comes in handy when applications crash and do not clean up)
// initializes the address range registers
//    (very coarse at the moment ... TODO: dedicate one range to each
//    application, export the ranges via MUFS, and set them in CL
//    during process initialization)
static int init_and_reset_mu(void)
{
    int rc;
    MUSPI_RESET_t reset;

    if(reset_mu) {
        MUSPI_InitReset(&reset);
        reset.clear_system = 1;

        do {
            rc = MUSPI_Reset(&reset);
        } while(rc == EAGAIN);

        if(rc) {
            MUFS_DEBUG("error %d in sequence %llu while initializing MU ... :-( giving up\n", rc, reset.sequence);
            return -1;
        }
    }

#if 0
    int i;
    printk(KERN_ERR"dumping MU address ranges before init by MUFS\n");
    for(i = 0; i < 10; i++)
        MUFS_DEBUG("sys range %d: %p - %p\n", i, (void *)DCRReadPriv(MU_DCR(MIN_SYS_ADDR_RANGE) + i), (void *)DCRReadPriv(MU_DCR(MAX_SYS_ADDR_RANGE) + i));
    for(i = 0; i < 20; i++)
        MUFS_DEBUG("usr range %d: %p - %p\n", i, (void *)DCRReadPriv(MU_DCR(MIN_USR_ADDR_RANGE) + i), (void *)DCRReadPriv(MU_DCR(MAX_USR_ADDR_RANGE) + i));
#endif


#define ADDR_16G 16L*1024*1024*1024
    // use 2nd address range (index 1) to avoid interference with
    // bgvrnic and ROQ (both use the first address range)
    // system address ranges -- not used, but set them just to be sure
    DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE) + 1, 0);
    DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE) + 1, ADDR_16G);

    // set (one of the) user address ranges
    // TODO: make this more specific
    DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE) + 1, 0);
    DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE) + 1, ADDR_16G);

#undef ADDR_16G

#if 0
    printk(KERN_ERR"dumping MU address ranges after init by MUFS\n");
    for(i = 0; i < 10; i++)
        MUFS_DEBUG("sys range %d: %p - %p\n", i, (void *)DCRReadPriv(MU_DCR(MIN_SYS_ADDR_RANGE) + i), (void *)DCRReadPriv(MU_DCR(MAX_SYS_ADDR_RANGE) + i));
    for(i = 0; i < 20; i++)
        MUFS_DEBUG("usr range %d: %p - %p\n", i, (void *)DCRReadPriv(MU_DCR(MIN_USR_ADDR_RANGE) + i), (void *)DCRReadPriv(MU_DCR(MAX_USR_ADDR_RANGE) + i));
#endif
    return 0;
}

/*
 * Module housekeeping.
 */
static int __init mufs_init(void)
{
    unsigned int i, j, k;
    unsigned int subgroup_id, fifo_id, bat_entry_id;
    int rc = 0;

    struct bgq_personality * bgpersonality = NULL;

#if 0
    print_mufs_sizeofs();
#endif

    // allocate and initialize metadata
    mufs_data = kmalloc(sizeof(mufs_data_t), GFP_KERNEL);
    if(!mufs_data)
        return ENOMEM;

    memset(mufs_data, 0, sizeof(mufs_data_t));

    // init FIFO-related meta data
    for(i=0; i < BGQ_MU_NUM_FIFO_GROUPS; i++) {
        for(j=0; j < BGQ_MU_NUM_FIFO_SUBGROUPS; j++) {
            subgroup_id = i * BGQ_MU_NUM_FIFO_SUBGROUPS + j;

            mufs_data->subgroups[ subgroup_id ].subgroup_id = subgroup_id;

            sema_init( &(mufs_data->subgroups[ subgroup_id ].sem), 1 );

            // initialize MUSPI_InjFifoSubGroup_t injFifoSubgroup
            //  * subgroupid
            mufs_data->subgroups[ subgroup_id ].injFifoSubgroup.subgroupid = subgroup_id;

            // initialize MUSPI_RecFifoSubGroup_t recFifoSubgroup
            //  * subgroupid
            mufs_data->subgroups[ subgroup_id ].recFifoSubgroup.subgroupid = subgroup_id;

            // initialize inj fifo structs
            for(k=0; k < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP; k++) {
                fifo_id = subgroup_id * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP + k;
                mufs_data->subgroups[ subgroup_id ].inj_fifos[ k ]. fifo_id = fifo_id;
                mufs_data->subgroups[ subgroup_id ].inj_fifos[ k ]. fifo_idx_in_subgroup = k;
                mufs_data->subgroups[ subgroup_id ].inj_fifos[ k ]. subgroup =
                    &(mufs_data->subgroups[ subgroup_id ]);
                sema_init( &(mufs_data->subgroups[ subgroup_id ].inj_fifos[ k ].open_sem), 1 );
            }

            // initialize rec fifo structs
            for(k=0; k < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; k++) {
                fifo_id = subgroup_id * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + k;
                mufs_data->subgroups[ subgroup_id ].rec_fifos[ k ]. fifo_id = fifo_id;
                mufs_data->subgroups[ subgroup_id ].rec_fifos[ k ]. fifo_idx_in_subgroup = k;
                mufs_data->subgroups[ subgroup_id ].rec_fifos[ k ]. subgroup =
                    &(mufs_data->subgroups[ subgroup_id ]);
                sema_init( &(mufs_data->subgroups[ subgroup_id ].rec_fifos[ k ].open_sem), 1 );
            }

            // initialize base address table entries
            for(k=0; k < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP; k++) {
                bat_entry_id = subgroup_id * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP + k;
                mufs_data->subgroups[ subgroup_id ].base_addresses[ k ].base_address_id = bat_entry_id;
                mufs_data->subgroups[ subgroup_id ].base_addresses[ k ].base_address_idx_in_subgroup = k;
                mufs_data->subgroups[ subgroup_id ].base_addresses[ k ].subgroup =
                    &(mufs_data->subgroups[ subgroup_id ]);
                sema_init( &(mufs_data->subgroups[ subgroup_id ].base_addresses[ k ].open_sem), 1);
            }
        }
    }

    // init class route-related metadata
    sema_init( &(mufs_data->classroute_sem), 1 );

    for(i=0; i < BGQ_COLL_CLASS_MAX_CLASSROUTES; i++) {
        mufs_data->coll_classroutes[i].id = i;
        mufs_data->coll_classroutes[i].mufs_data = mufs_data;
        sema_init( &(mufs_data->coll_classroutes[i].open_sem), 1);
    }

    for(i=0; i < BGQ_GI_CLASS_MAX_CLASSROUTES; i++) {
        mufs_data->gi_classroutes[i].id = i;
        mufs_data->gi_classroutes[i].mufs_data = mufs_data;
        sema_init( &(mufs_data->gi_classroutes[i].open_sem), 1);
    }

    // get primordial class route from firmware Personality and store it
    // in mufs_data:

    bgpersonality = get_bgpersonality();
    if(bgpersonality == NULL) {
        rc = -ENXIO;
        goto out_data;
    }

    // copy primordial class route
    memcpy(&(mufs_data->primordial_cr), &(bgpersonality->network_config.primordial_class_route), sizeof(struct bgq_personality));

    // register and map I/O memory of MU
    mufs_data->mu_resource = request_mem_region(PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET,
                             PHYMAP_SIZE_MU, "mupriv");
    if( mufs_data->mu_resource == NULL) {
        rc = -ENXIO;
        goto out_data;
    }

    mufs_data->mu_unpriv_resource = request_mem_region(PHYMAP_MINADDR_MU, PHYMAP_SIZE_MU, "mu");
    if( mufs_data->mu_unpriv_resource == NULL ) {
        rc = -ENXIO;
        goto out_iomem;
    }

    // remap MU I/O memorry. ioremap does _PAGE_NO_CACHE and
    // _PAGE_GUARDED by default on PPC64

    mufs_data->mu_unpriv_base = ioremap_nocache(PHYMAP_MINADDR_MU, PHYMAP_SIZE_MU);
    if( mufs_data->mu_unpriv_base == NULL ) {
        rc = -ENXIO;
        goto out_iomem;
    }

    //mufs_data->mu_base = ioremap_nocache(PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET, PHYMAP_SIZE_MU);
    // _PAGE_GUARDED _PAGE_NO_CACHE
    mufs_data->mu_base = __ioremap_at(PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET,
                                      mufs_data->mu_unpriv_base + PHYMAP_PRIVILEGEDOFFSET, PHYMAP_SIZE_MU,
                                      PAGE_KERNEL_NCG | _PAGE_PRESENT);
    if( mufs_data->mu_base == NULL ) {
        rc = -ENXIO;
        goto out_iomem;
    }

    // check that offset PHYMAP_PRIVILEGEDOFFSET matches arithmetically
    if( mufs_data->mu_base != mufs_data->mu_unpriv_base + PHYMAP_PRIVILEGEDOFFSET ) {
        printk(KERN_ERR "%s: failed to map privileged MU memory right PHYMAP_PRIVILEGEDOFFSET above unprivileged part.\n", __func__);
        rc = -ENOMEM;
        goto out_iomem;
    }

    // check that offset trick is working. All MU SPI headers use
    // that (unfortunate) "trick".
    if( (void *)( (unsigned long int)mufs_data->mu_base & ~(unsigned long int)PHYMAP_PRIVILEGEDOFFSET)
        != mufs_data->mu_unpriv_base ) {
        printk(KERN_ERR "%s: failed to map MU memory so that offset \"trick\" would work, sorry.\n", __func__);
        rc = -ENOMEM;
        goto out_iomem;
    }

    mufs_priv_base   = mufs_data->mu_base;
    mufs_unpriv_base = mufs_data->mu_unpriv_base;

    printk("mufs_init: remapped MU I/O memory to %p and %p\n", mufs_data->mu_unpriv_base, mufs_data->mu_base);

    // initialize group status pointers, now that we know where MU is // mapped
    for(i=0; i < BGQ_MU_NUM_FIFO_GROUPS; i++) {
        for(j=0; j < BGQ_MU_NUM_FIFO_SUBGROUPS; j++) {
            subgroup_id = i * BGQ_MU_NUM_FIFO_SUBGROUPS + j;
            //  * groupStatus pointer (see AllocateInjfifos)
            mufs_data->subgroups[ subgroup_id ].injFifoSubgroup.groupStatus =
                (MUHWI_InjFifoStatus_t *) ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(i,j) );

            //  * groupStatus pointer (see AllocateRecFifos)
            mufs_data->subgroups[ subgroup_id ].recFifoSubgroup.groupStatus =
                (MUHWI_RecFifoStatus_t *) ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(i, j) );
        }

    }

#ifdef USE_MMIO_AREA
    // mmap MMIO area for DCR access (copied from upcfs)
    mmio_nonpriv_base = ioremap_nocache(PHYMAP_MINADDR_MMIO, PHYMAP_SIZE_MMIO);

    if (mmio_nonpriv_base == NULL) {
        printk("mufs_init: ioremap mmio_nonpriv_base failed\n");
        rc = -ENXIO;
        goto out_iomem;
    }

    printk("mufs_init: mmio_nonpriv_base %p\n", mmio_nonpriv_base);
    mmio_priv_base = __ioremap_at(PHYMAP_MINADDR_MMIO + PHYMAP_PRIVILEGEDOFFSET, mmio_nonpriv_base + PHYMAP_PRIVILEGEDOFFSET,
                                  PHYMAP_SIZE_MMIO, PAGE_KERNEL_NCG | _PAGE_PRESENT);
    if (mmio_priv_base == NULL) {
        printk("mufs_init: ioremap mmio_priv_base failed\n");
        rc = -ENXIO;
        goto out_iomem;
    }

    printk("mufs_init: mmio_priv_base %p\n", mmio_priv_base);

    if (mmio_priv_base != mmio_nonpriv_base + PHYMAP_PRIVILEGEDOFFSET) {
        printk("mufs_init: Failed to map mmio_priv_base after mmio_nonpriv_base\n");
        rc = -ENOMEM;
        goto out_iomem;
    }

    if ((void*)((uint64_t)mmio_priv_base & ~(uint64_t)PHYMAP_PRIVILEGEDOFFSET) != mmio_nonpriv_base) {
        printk("mufs_init: Failed to map mmio memory to accommodate offset trick, PHYMAP_PRIVILEGEDOFFSET %p\n",
               (void*)PHYMAP_PRIVILEGEDOFFSET);
        rc = -ENOMEM;
        goto out_iomem;
    }
#endif

    rc = init_and_reset_mu();
    if(rc)
        goto out_iomem;

    rc = register_filesystem(&mufs_type);
    if(rc)
        goto out_iomem;
    return rc;

out_iomem: // release I/O memory
    printk(KERN_ERR "releasing mu_resource in out_iomem\n");
    if( mufs_data -> mu_resource )
        release_mem_region(PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET, PHYMAP_SIZE_MU);
    printk(KERN_ERR "releasing mu_unpriv_resource in out_iomem\n");
    if( mufs_data -> mu_unpriv_resource )
        release_mem_region(PHYMAP_MINADDR_MU, PHYMAP_SIZE_MU);
    printk(KERN_ERR "releasing mu_base in out_iomem\n");
    if( mufs_data->mu_base )
        __iounmap_at(mufs_data->mu_base, PHYMAP_SIZE_MU);
    printk(KERN_ERR "releasing mu_unpriv_base in out_iomem\n");
    if( mufs_data->mu_unpriv_base )
        __iounmap_at(mufs_data->mu_unpriv_base, PHYMAP_SIZE_MU);
#ifdef USE_MMIO_AREA
    printk(KERN_ERR "releasing mmio_nonpriv_base in out_iomem\n");
    if( mmio_nonpriv_base )
        iounmap(mmio_nonpriv_base);
    printk(KERN_ERR "releasing mmio_priv_base in out_iomem\n");
    if( mmio_priv_base )
        __iounmap_at(mmio_priv_base, PHYMAP_SIZE_MMIO);
#endif
out_data: // free MUFS datastructures
    if(mufs_data != NULL)
        kfree(mufs_data);
    if(bgpersonality != NULL)
        kfree(bgpersonality);
    return rc;
}


static void __exit mufs_cleanup(void)
{
    //
    // TODO deallocate all FIFOs that have been allocated via the FS
    // interface

#ifdef USE_MMIO_AREA
    // unmap mmio memory
    printk(KERN_ERR "releasing mmio_nonpriv_base in cleanup\n");
    __iounmap_at(mmio_nonpriv_base, PHYMAP_SIZE_MMIO);
    printk(KERN_ERR "releasing mmio_priv_base in cleanup\n");
    __iounmap_at(mmio_priv_base,    PHYMAP_SIZE_MMIO);
#endif

    // unmap I/O memory
    __iounmap_at(mufs_data->mu_base, PHYMAP_SIZE_MU);
    __iounmap_at(mufs_data->mu_unpriv_base, PHYMAP_SIZE_MU);

    // release I/O memory resource
    release_mem_region(PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET, PHYMAP_SIZE_MU);
    release_mem_region(PHYMAP_MINADDR_MU, PHYMAP_SIZE_MU);

    kfree(mufs_data);

    unregister_filesystem( &mufs_type );
}


module_init(mufs_init);
module_exit(mufs_cleanup);

