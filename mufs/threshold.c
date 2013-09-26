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

ssize_t inj_fifo_threshold_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{
    //mufs_data_t * mufs_data = (mufs_data_t *)filp->f_dentry->d_inode->i_private;
    int rc;

    uint64_t thresholds[2];

    // verify buffer size -> must be a 1-byte flag
    if(len != sizeof(thresholds))
        return -EINVAL;

    // copy in flag, abort if this fails
    if(copy_from_user( &thresholds, buf, sizeof(thresholds) ))
        return -EINVAL;

    rc = Kernel_ConfigureInjFifoThresholds( &(thresholds[0]), &(thresholds[1]) );

    if(rc) {
        return -rc;
    } else {
        return sizeof(thresholds);
    }
}

ssize_t rec_fifo_threshold_write(struct file * filp, const char __user * buf, size_t len, loff_t * offset)
{
    //mufs_data_t * mufs_data = (mufs_data_t *)filp->f_dentry->d_inode->i_private;
    int rc;

    uint64_t threshold;

    // verify buffer size -> must be a 1-byte flag
    if(len != sizeof(threshold))
        return -EINVAL;

    // copy in flag, abort if this fails
    if(copy_from_user( &threshold, buf, sizeof(threshold) ))
        return -EINVAL;

    rc = Kernel_ConfigureRecFifoThreshold( threshold );

    if(rc) {
        return -rc;
    } else {
        return sizeof(threshold);
    }
}

