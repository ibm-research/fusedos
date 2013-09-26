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
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/mm.h>
#include <asm/uaccess.h>

#include <asm/uaccess.h>

ssize_t primordial_cr_read(struct file *filp, char __user * buf, size_t len, loff_t * offset)
{
    mufs_data_t * mufs_data = (mufs_data_t *)filp->f_dentry->d_inode->i_private;
    long     copied;

    if(len != sizeof(struct bgq_primordial_class_route))
        return -EINVAL;

    copied = copy_to_user( buf, &(mufs_data->primordial_cr), len);
    if(copied) // failure in copy_to_user, some data left to be transferred
        return -EINVAL;

    // indicate success
    return len;
}
