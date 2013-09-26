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

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

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


ssize_t coll_classroute_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{
    int rc;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)filp->private_data;
    ClassRoute_t cr;

    // verify buffer size -> must contain a class route
    if(len != sizeof(ClassRoute_t))
        return -EINVAL;

    // copy in class route, abort if this fails
    if(copy_from_user( &cr, buf, sizeof(cr) ))
        return -EINVAL;

    //MUFS_DEBUG("cr inp %x out %c\n", cr.input, cr.output);

    // lock classroute semaphore
    if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
        return EINTR;

    rc = Kernel_SetCollectiveClassRoute( my_classroute->id, &cr );

    // unlock
    up( &(my_classroute->mufs_data->classroute_sem) );

    if(rc==0) {
        return len;
    } else {
        return -rc;
    }
}

ssize_t gi_classroute_write (struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{
    int rc = 0;
    uint8_t control_reg_value;
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)filp->private_data;
    ClassRoute_t cr;
    volatile uint64_t * controlRegPtr;

    // verify buffer size -> must contain a class route or a 1-byte
    // value for the gi/barrier control register
    if(len != sizeof(ClassRoute_t) && len != sizeof(control_reg_value))
        return -EINVAL;

    // copy in class route / control register value, abort if this fails
    if(copy_from_user( &cr, buf, len ))
        return -EINVAL;

    // lock classroute semaphore
    if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
        return EINTR;

    // case 1: set ClassRoute
    if(len == sizeof(ClassRoute_t)) {
        //MUFS_DEBUG("cr set inp %x out %c\n", cr.input, cr.output);
        rc = Kernel_SetGlobalInterruptClassRoute( my_classroute->id, &cr );

    // case 2: set gi/barrier control register
    } else {
        control_reg_value = (*((uint8_t *)(&cr))) & 0x07; // use first byte 
        controlRegPtr = (uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,my_classroute->id));

        // store value into control register
        *controlRegPtr = (uint64_t) control_reg_value;
    }

    // unlock
    up( &(my_classroute->mufs_data->classroute_sem) );

    if(rc==0) {
        return len;
    } else {
        return -rc;
    }
}

ssize_t gi_classroute_read (struct file *filp, char __user * buf, size_t len, loff_t * offset)
{
    mufs_classroute_t * my_classroute = (mufs_classroute_t *)filp->private_data;
    volatile uint64_t * statusRegPtr  = (uint64_t*)(BGQ_MU_GI_STATUS_OFFSET(0,my_classroute->id));
    volatile uint64_t * controlRegPtr = (uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,my_classroute->id));

    uint8_t reg_values[2];
    long    copied;

    // check buffer size, just getting the status reg is ok
    if( len < sizeof(uint8_t) )
        return -EINVAL;

    // lock classroute semaphore
    if(down_interruptible( &(my_classroute->mufs_data->classroute_sem) ))
        return EINTR;

    reg_values[0] = (uint8_t)( *statusRegPtr  & 0x07 );
    reg_values[1] = (uint8_t)( *controlRegPtr & 0x07 );

    // unlock
    up( &(my_classroute->mufs_data->classroute_sem) );

    // we copy the unallocated FIFO ids to user-space
    // the size of the transfer (read()'s return value) indicates
    // the number of unallocated FIFOs
    len = min(len, sizeof(reg_values) );

    copied = copy_to_user( buf, &reg_values, len);
    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    return len; // return the number of bytes "read"
}

