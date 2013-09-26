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
#include <asm/uaccess.h>

#include "spi/include/kernel/MU.h"
#include "spi/include/kernel/collective.h"
#include "spi/include/kernel/gi.h"

// open injection FIFO file -> first stage of allocation
int inj_fifo_open (struct inode *i, struct file *filp)
{
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)i->i_private;
    //struct subgroup * my_subgroup = my_fifo->subgroup;

    filp->private_data = my_fifo;

    // lock semaphore to achieve exclusive open()
    if(down_trylock(&(my_fifo->open_sem)))
        return EBUSY;

    return 0;
}


// 1) write FIFO attributes to injection FIFO file -> second stage of allocation
// 2) write 5 bytes, containing Kernel_InjFifoInterrupts_t -> ConfigureInjFifoInterrupts
ssize_t inj_fifo_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{

    int rc;
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)filp->private_data;
    struct subgroup * my_subgroup = my_fifo->subgroup;
    uint32_t fifo_id = my_fifo->fifo_idx_in_subgroup;

    Kernel_InjFifoAttributes_t attributes;

    // case 1) second stage of allocation
    if(len == sizeof(Kernel_InjFifoAttributes_t)) {

        // copy in attributes, abort if this fails
        if(copy_from_user( &attributes, buf, sizeof(Kernel_InjFifoAttributes_t) ))
            return -EINVAL;

        // lock subgroup
        if(down_interruptible( &(my_subgroup->sem) ))
            return -EINTR;

        MUFS_DEBUG("calling Kernel_AllocateInjFifos subgrp %u, sg_ptr %p, fifoids at %p, fifoid %u, attributes at %p",
                   my_subgroup->subgroup_id, &(my_subgroup->tmp_inj_sg), &fifo_id, fifo_id, &attributes);
        // call actual SPI allocation function
        rc = Kernel_AllocateInjFifos( my_subgroup->subgroup_id, &(my_subgroup->tmp_inj_sg), 1,
                                      &fifo_id, &attributes);

        // unlock
        up( &(my_subgroup->sem) );

        if(!rc) {
            // everything went fine, merge in data from
            // MUSPI_InjFifoSubGroup_t

            memcpy( &(my_subgroup->injFifoSubgroup._injfifos[my_fifo->fifo_idx_in_subgroup]),
                    &(my_subgroup->tmp_inj_sg._injfifos[my_fifo->fifo_idx_in_subgroup]),
                    sizeof(MUSPI_InjFifo_t) );

            return len; // indicate success
        } else {
            return -rc;
        }
    } else if(len==5) { // case 2) ConfigureInjFifoInterrupts
        Kernel_InjFifoInterrupts_t fifoInterrupt;

        // copy in FIFO interrupt attributes, abort if that fails
        if(copy_from_user( &fifoInterrupt, buf, sizeof(Kernel_InjFifoInterrupts_t) ))
            return -EINVAL;

        // lock subgroup
        if(down_interruptible( &(my_subgroup->sem) ))
            return -EINTR;

        rc = Kernel_ConfigureInjFifoInterrupts( &(my_subgroup->injFifoSubgroup), 1, &fifo_id, &fifoInterrupt );

        // unlock
        up( &(my_subgroup->sem) );

        if(!rc) {
            return len; // everything went fine
        } else {
            return -rc;
        }
    } else { // invalid / unknown buffer size 
        return - EINVAL;
    }
}


// open reception FIFO file -> allocation
int rec_fifo_open (struct inode *i, struct file *filp)
{
    int rc;
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)i->i_private;
    struct subgroup * my_subgroup = my_fifo->subgroup;
    uint32_t fifo_id = my_fifo->fifo_idx_in_subgroup;

    Kernel_RecFifoAttributes_t attributes;

    filp->private_data = my_fifo;

    if(down_trylock(&(my_fifo->open_sem)))
        return EBUSY;

    // setup dummy Fifo attributes
    memset( &attributes, 0, sizeof(Kernel_RecFifoAttributes_t));
    attributes.System = 0;

    // lock subgroup
    if(down_interruptible( &(my_subgroup->sem) ))
        return EINTR;

    MUFS_DEBUG("calling Kernel_AllocateRecFifos subgrp %d, tmp_rec_sg %p, fifo_id %d\n",
	    my_subgroup->subgroup_id, &(my_subgroup->tmp_rec_sg), fifo_id);

    // call actual allocation SPI function
    rc = Kernel_AllocateRecFifos (my_subgroup->subgroup_id, &(my_subgroup->tmp_rec_sg), 1,
                                  &fifo_id, &attributes);

    // unlock
    up( &(my_subgroup->sem) );

    if(!rc) {
        // everything went fine, merge in data from
        // MUSPI_RecFifoSubGroup_t

        memcpy( &(my_subgroup->recFifoSubgroup._recfifos[my_fifo->fifo_idx_in_subgroup]),
                &(my_subgroup->tmp_rec_sg._recfifos[my_fifo->fifo_idx_in_subgroup]), sizeof(MUSPI_RecFifo_t));
    }

    return rc; // return status from allocation function
}


// close injection FIFO file -> deallocation
int inj_fifo_release (struct inode * i, struct file *filp)
{
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)filp->private_data;
    struct subgroup * my_subgroup = my_fifo->subgroup;
    uint32_t fifo_id = my_fifo -> fifo_idx_in_subgroup;
    int rc = 0;

    // lock subgroup
    if(down_interruptible( &(my_subgroup->sem) ))
        return EINTR;

    // deactivate FIFO before release, in case
    // user process has forgotten to to so (best effort)
    Kernel_InjFifoActivate( &(my_subgroup->injFifoSubgroup), 1,
	    &fifo_id, KERNEL_INJ_FIFO_DEACTIVATE);

    // actual FIFO deallocation
    rc = Kernel_DeallocateInjFifos( &(my_subgroup->injFifoSubgroup), 1, &fifo_id);

    // unlock
    up( &(my_subgroup->sem) );

    // release semaphore on file to allow others to open() it
    up(&(my_fifo->open_sem));
    return rc;
}


// close reception FIFO file -> deallocation
int rec_fifo_release (struct inode * i, struct file *filp)
{
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)filp->private_data;
    struct subgroup * my_subgroup = my_fifo->subgroup;
    uint32_t fifo_id = my_fifo -> fifo_idx_in_subgroup;
    uint64_t disableBits;
    int rc = 0;

    // lock subgroup
    if(down_interruptible( &(my_subgroup->sem) ))
        return EINTR;

    // deactivate FIFO before release, in case
    // user process has forgotten to to so (best effort)
    disableBits = (0x1L << 16) >> fifo_id;
    Kernel_RecFifoDisable(
	    my_subgroup->subgroup_id / BGQ_MU_NUM_FIFO_SUBGROUPS,
	    disableBits );

    // actual FIFO deallocation
    rc = Kernel_DeallocateRecFifos( &(my_subgroup->recFifoSubgroup), 1, &fifo_id);

    // unlock
    up( &(my_subgroup->sem) );

    // release semaphore on file to allow others to open() it
    up(&(my_fifo->open_sem));
    return rc;
}

// 1) write 1-byte 1 or 0 to reception FIFO file -> enable or disable the fifo 
// 2) write 5 bytes containing
ssize_t rec_fifo_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{

    int rc;
    mufs_fifo_t * my_fifo = (mufs_fifo_t *)filp->private_data;
    struct subgroup * my_subgroup = my_fifo->subgroup;

    uint8_t  flag;
    uint64_t enableBits;
    uint32_t group_id;
    uint32_t fifo_id_in_grp;

    if(len == 1) { // case 1) enable/disable FIFO
        // copy in flag, abort if this fails
        if(copy_from_user( &flag, buf, sizeof(flag) ))
            return -EINVAL;

        // prepare group_id and enableBits
        group_id = my_subgroup->subgroup_id / BGQ_MU_NUM_FIFO_SUBGROUPS;
        fifo_id_in_grp = my_fifo->fifo_idx_in_subgroup + (my_subgroup->subgroup_id % BGQ_MU_NUM_FIFO_SUBGROUPS) * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP;
        enableBits = 0x8000 >> fifo_id_in_grp;

        MUFS_DEBUG("enabling/disabling bit rec FIFO %u in subgroup %u -> FIFO %u in group %u -> enableBits %llx\n",
                my_fifo->fifo_idx_in_subgroup, my_subgroup->subgroup_id,
                fifo_id_in_grp, group_id,
                enableBits);

        // lock subgroup
        if(down_interruptible( &(my_subgroup->sem) ))
            return -EINTR;

        if(flag)
            rc = Kernel_RecFifoEnable(group_id, enableBits);
        else
            rc = Kernel_RecFifoDisable(group_id, enableBits);

        // unlock
        up( &(my_subgroup->sem) );

    } else if(len==5) { // case 2) ConfigureInjFifoInterrupts
        uint32_t fifo_id = my_fifo->fifo_idx_in_subgroup;
        Kernel_RecFifoInterrupts_t fifoInterrupt;

        // copy in FIFO interrupt attributes, abort if that fails
        if(copy_from_user( &fifoInterrupt, buf, sizeof(Kernel_RecFifoInterrupts_t) ))
            return -EINVAL;

        // lock subgroup
        if(down_interruptible( &(my_subgroup->sem) ))
            return -EINTR;

        rc = Kernel_ConfigureRecFifoInterrupts( &(my_subgroup->recFifoSubgroup), 1, &fifo_id, &fifoInterrupt );

        // unlock
        up( &(my_subgroup->sem) );

        if(!rc) {
            return len; // everything went fine
        } else {
            return -rc;
        }
    } else { // invalid / unknown buffer size 
        return - EINVAL;
    }

    if(rc) {
        return -rc; // error occurred, signal that
    } else {
        return len; // no error, return len to indicate success
    }
}


// open BAT entry file -> allocation
int bat_entry_open (struct inode *i, struct file *filp)
{
    int rc;
    mufs_base_address_t * my_bat_entry = (mufs_base_address_t *)i->i_private;
    struct subgroup *     my_subgroup = my_bat_entry->subgroup;
    uint32_t              bat_entry_id = my_bat_entry->base_address_idx_in_subgroup;

    filp->private_data = my_bat_entry;

    if(down_trylock(&(my_bat_entry->open_sem)))
        return EBUSY;

    // lock subgroup
    if(down_interruptible( &(my_subgroup->sem) ))
        return EINTR;

    MUFS_DEBUG("calling Kernel_AllocateBaseAddressTable, subgrp %d, ptr %p, bat_entry_id %d\n",
	    my_subgroup->subgroup_id, &(my_subgroup->batSubgroup) , bat_entry_id);

    // call actual allocation SPI function
    rc = Kernel_AllocateBaseAddressTable (my_subgroup->subgroup_id, &(my_subgroup->batSubgroup), 1,
                                  &bat_entry_id, 0);
    // unlock
    up( &(my_subgroup->sem) );

    return rc; // return status from allocation function
}


// close BAT entry file -> deallocation
int bat_entry_release (struct inode * i, struct file *filp)
{
    mufs_base_address_t * my_bat_entry = (mufs_base_address_t *)filp->private_data;
    struct subgroup *     my_subgroup = my_bat_entry->subgroup;
    uint32_t              bat_entry_id = my_bat_entry->base_address_idx_in_subgroup;

    int rc = 0;

    // lock subgroup
    if(down_interruptible( &(my_subgroup->sem) ))
        return EINTR;

    // actual BAT entry deallocation
    rc = Kernel_DeallocateBaseAddressTable( &(my_subgroup->batSubgroup), 1, &bat_entry_id);

    // unlock
    up( &(my_subgroup->sem) );

    // release semaphore on file to allow others to open() it
    up(&(my_bat_entry->open_sem));
    return rc;
}


int coll_classroute_open (struct inode *i, struct file *filp)
{
    int rc;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)i->i_private;
    
    filp->private_data = my_classroute;

    if(down_trylock(&(my_classroute->open_sem)))
        return EBUSY;

    // lock classroute semaphore
    if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
        return EINTR;

    MUFS_DEBUG("calling Kernel_AllocateCollectiveClassRoute %d\n",
	    my_classroute->id);

    // call actual allocation SPI function
    rc = Kernel_AllocateCollectiveClassRoute(my_classroute->id);

    // unlock
    up( &(my_classroute->mufs_data->classroute_sem) );

    return rc;
}

int coll_classroute_release (struct inode * i, struct file *filp)
{
    int rc = 0;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)filp->private_data;

    // lock classroute semaphore
    if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
        return EINTR;

    rc = Kernel_DeallocateCollectiveClassRoute(my_classroute->id);

    // unlock
    up( &(my_classroute->mufs_data->classroute_sem) );

    // release semaphore on file to allow others to open() it
    up( &(my_classroute->open_sem) );

    return rc;
}


int gi_classroute_open (struct inode *i, struct file *filp)
{
    int rc = 0;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)i->i_private;
    Kernel_GlobalInterruptClassRouteAttributes_t attr;
    
    filp->private_data = my_classroute;

    memset(&attr, 0, sizeof(attr));
    attr.System = 0;

    if(down_trylock(&(my_classroute->open_sem)))
        return EBUSY;

    // system class route 15 can be opened, but it is already allocated
    // by the Firmware and must not be overwritten;
    // similarly, for subblock support we must keep job class route 14
    // alive over multiple alloc/release cycles.

    if( (my_classroute->id != 15) && (my_classroute->id != 14) ) {
        // lock classroute semaphore
        if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
            return EINTR;

	MUFS_DEBUG("calling Kernel_AllocateGlobalInterruptClassRoute, %d\n",
		my_classroute->id);

        // call actual allocation SPI function
        rc = Kernel_AllocateGlobalInterruptClassRoute(my_classroute->id, &attr);

        // unlock
        up( &(my_classroute->mufs_data->classroute_sem) );
    }

    return rc;
}

int gi_classroute_release (struct inode * i, struct file *filp)
{
    int rc = 0;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)filp->private_data;

    if( (my_classroute->id != 15) && (my_classroute->id != 14) ) {
        // lock classroute semaphore
        if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
            return EINTR;

        rc = Kernel_DeallocateGlobalInterruptClassRoute(my_classroute->id);

        // unlock
        up( &(my_classroute->mufs_data->classroute_sem) );
    }

    // release semaphore on file to allow others to open() it
    up( &(my_classroute->open_sem) );

    return rc;
}

