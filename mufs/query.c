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
/* ================================================================ */

#include "mufs.h"

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <linux/kdev_t.h>
#include <asm/page.h>
#include <asm/uaccess.h>

#include <linux/device.h>

#include "hwi/include/bqc/PhysicalMap.h"
#include "spi/include/kernel/MU.h"
#include "spi/include/kernel/collective.h"
#include "spi/include/kernel/gi.h"


ssize_t inj_fifo_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset)
{

    mufs_subgroup_t * my_subgroup = (mufs_subgroup_t *)filp->f_dentry->d_inode->i_private;

    int      rc;
    uint32_t num_fifos;
    uint32_t fifo_ids[BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP]; // 32 B on kernel stack should be fine ;-)
    long     copied;

    if(down_interruptible( &(my_subgroup->sem) ))
        return -EINTR;

    rc = Kernel_QueryInjFifos( my_subgroup->subgroup_id, &num_fifos, fifo_ids);

    up( &(my_subgroup->sem) );

    if(rc) // error in SPI function
        return -rc;

    // we copy the unallocated FIFO ids to user-space
    // the size of the transfer (read()'s return value) indicates
    // the number of unallocated FIFOs
    len = min(len, num_fifos * sizeof(uint32_t) );

    copied = copy_to_user( buf, fifo_ids, len);
    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

ssize_t rec_fifo_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset)
{

    mufs_subgroup_t * my_subgroup = (mufs_subgroup_t *)filp->f_dentry->d_inode->i_private;
    int      rc;
    uint32_t num_fifos;
    uint32_t fifo_ids[BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP]; // 16 B on kernel stack should be fine ;-)
    long     copied;

    if(down_interruptible( &(my_subgroup->sem) ))
        return -EINTR;

    rc = Kernel_QueryRecFifos( my_subgroup->subgroup_id, &num_fifos, fifo_ids);

    up( &(my_subgroup->sem) );

    if(rc) // error in SPI function
        return -rc;

    // we copy the unallocated FIFO ids to user-space
    // the size of the transfer (read()'s return value) indicates
    // the number of unallocated FIFOs
    len = min(len, num_fifos * sizeof(uint32_t) );

    copied = copy_to_user( buf, fifo_ids, len);
    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

ssize_t bat_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset)
{

    mufs_subgroup_t * my_subgroup = (mufs_subgroup_t *)filp->f_dentry->d_inode->i_private;
    int      rc;
    uint32_t num_bats;
    uint32_t bat_ids[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP]; // 32 B on kernel stack should be fine ;-)
    long     copied;

    if(down_interruptible( &(my_subgroup->sem) ))
        return -EINTR;

    rc = Kernel_QueryBaseAddressTable( my_subgroup->subgroup_id, &num_bats, bat_ids);

    up( &(my_subgroup->sem) );

    if(rc) // error in SPI function
        return -rc;

    // we copy the unallocated FIFO ids to user-space
    // the size of the transfer (read()'s return value) indicates
    // the number of unallocated FIFOs
    len = min(len, num_bats * sizeof(uint32_t) );

    copied = copy_to_user( buf, bat_ids, len);
    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

ssize_t coll_classroute_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset) {

    mufs_data_t * mufs_data = (mufs_data_t *)filp->f_dentry->d_inode->i_private;
    int rc;
    uint32_t num_classroutes;
    uint32_t classroute_ids[BGQ_COLL_CLASS_MAX_CLASSROUTES];
    long copied;

    if(down_interruptible( &(mufs_data->classroute_sem) ))
        return -EINTR;

    rc = Kernel_QueryCollectiveClassRoutes(&num_classroutes, classroute_ids, sizeof(classroute_ids));

    up( &(mufs_data->classroute_sem) );

    if(rc) // error in SPI function
        return -rc;

    len = min(len, num_classroutes * sizeof(uint32_t) );

    copied = copy_to_user( buf, classroute_ids, len );

    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

ssize_t gi_classroute_query_read (struct file *filp, char __user * buf, size_t len, loff_t * offset) {

    mufs_data_t * mufs_data = (mufs_data_t *)filp->f_dentry->d_inode->i_private;
    int rc;
    uint32_t num_classroutes;
    uint32_t classroute_ids[BGQ_GI_CLASS_MAX_CLASSROUTES];
    long copied;

    if(down_interruptible( &(mufs_data->classroute_sem) ))
        return -EINTR;

    rc = Kernel_QueryGlobalInterruptClassRoutes(&num_classroutes, classroute_ids, sizeof(classroute_ids));

    up( &(mufs_data->classroute_sem) );

    if(rc) // error in SPI function
        return -rc;

    len = min(len, num_classroutes * sizeof(uint32_t) );

    copied = copy_to_user( buf, classroute_ids, len );

    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

