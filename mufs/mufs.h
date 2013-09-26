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

// mufs.h
//
// define data structures used by MUFS 

#ifndef __MUFS_H
#define __MUFS_H

//#define ENABLE_DEBUG

// override MU_Macros.h
#include "MU_Macros.h"

#ifdef __KERNEL__
#include <linux/ioport.h>

#ifdef ENABLE_DEBUG
#define MUFS_DEBUG(FMT, ...) do { printk(KERN_ERR "%s: "FMT, __func__, __VA_ARGS__); } while(0);
#else
#define MUFS_DEBUG(FMT, ...) ;
#endif
#endif

// provide a more friendly version of MUSPI_assert
#define MUSPI_assert(x) WARN_ON( !(x) )

#include <linux/semaphore.h>
#include "hwi/include/bqc/MU_Macros.h"
#include "hwi/include/bqc/classroute.h"
#include "spi/include/mu/RecFifo.h"
#include "spi/include/mu/InjFifo.h"
#include "spi/include/mu/Addressing.h"

#include <platforms/bgq/personality.h>
#include <asm/uaccess.h>

typedef struct fifo {
    struct subgroup * subgroup;
    uint32_t          fifo_id;
    uint8_t           fifo_idx_in_subgroup;
    struct semaphore  open_sem; // used to make open()ing the file exclusive -> first step to exclusive allocation
} mufs_fifo_t;

typedef struct base_address {
    struct subgroup * subgroup;
    unsigned int      base_address_id;
    uint8_t           base_address_idx_in_subgroup;
   struct semaphore  open_sem; // used to make open()ing the file exclusive -> first step to exclusive allocation
} mufs_base_address_t;

typedef struct classroute {
   uint8_t id;
   struct semaphore open_sem; // used to make open()ing the file exclusive -> first step to exclusive allocation
   struct mufs_data * mufs_data;
} mufs_classroute_t;


typedef struct subgroup {

    uint8_t subgroup_id;

    mufs_fifo_t inj_fifos[ BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP ];
    mufs_fifo_t rec_fifos[ BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP ];


    mufs_base_address_t base_addresses[ BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP ];

    struct semaphore sem;

    MUSPI_InjFifoSubGroup_t          injFifoSubgroup;
    MUSPI_RecFifoSubGroup_t          recFifoSubgroup;
   MUSPI_BaseAddressTableSubGroup_t batSubgroup;

    // temporary structs for SPI calls -- they are too large for the
    // kernel stack, having them here avoids kmalloc(), and the subgroup
    // lock ensures mutual exclusive use
    MUSPI_InjFifoSubGroup_t tmp_inj_sg;
    MUSPI_RecFifoSubGroup_t tmp_rec_sg;
} mufs_subgroup_t;

typedef struct mufs_data {

    struct resource * mu_resource;
    struct resource * mu_unpriv_resource;
    void *            mu_base;
    void *            mu_unpriv_base;
    mufs_subgroup_t   subgroups[ BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE ];

   struct semaphore classroute_sem;
   mufs_classroute_t coll_classroutes[BGQ_COLL_CLASS_MAX_CLASSROUTES];
   mufs_classroute_t gi_classroutes[BGQ_GI_CLASS_MAX_CLASSROUTES];

   struct bgq_primordial_class_route primordial_cr;

} mufs_data_t;

#endif // __MUFS_H
