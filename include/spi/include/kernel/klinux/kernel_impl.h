/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2008, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
/*								    */
/* Created by:                                                      */
/*      Jay S. Bryant (jsbryant@us.ibm.com)                         */
/*      January, 2010                                               */
/*								    */ 

#ifndef  _KERNEL_KLINUX_KERNEL_IMPL_H_
#define  _KERNEL_KLINUX_KERNEL_IMPL_H_

/**
 * \file kernel_impl.h
 *
 * \brief C Header File containing Linux specific definitions for the Messaging Unit SPIs
 *
 * 	Note: This header is for use by the Linux kernel, not user space.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#if !(defined __FUSEDOS__) && !(defined __ROQ__)                                                                       // FUSEDOS
#include <asm/bluegene.h>
#endif                                                                                                                 // FUSEDOS

#ifndef __INLINE__
#define __INLINE__ extern inline
#endif

/**
 * \brief MU SPI Assertion
 *
 */
#ifndef MUSPI_assert
#define MUSPI_assert(x) BUG_ON( !(x) ) 
#endif

/**
 * \brief popcnt64
 *
 * This function comes from hwi/include/bqc/A2_inlines.h which doesn't
 * include well into Linux. The function is included here for
 * use by spi/include/mu/Desciptor_inlines.h
 *
 */
__INLINE__ unsigned int popcnt64( uint64_t v )
{
    int rval;

    asm volatile ("popcntd %0,%1" : "=r" (rval) : "r" (v) );

    return( rval );
}

// rotate left and insert with mask
// Must be a macro rather than an inline because of the constant operands. 
// Note: modifies initial argument (ra). 
#define RLIMI64(ra,rs,sh,mb) \
    asm("rldimi %0,%2,%3,%4" \
        : "=r" (ra) : "0" (ra), "r" (rs), "i" (sh), "i" (mb))


/**
 * \brief ppc_msync
 *
 * This function comes from hwi/include/bqc/A2_inlines.h which doesn't
 * include well into Linux. The function is included here for
 * use by spi/include/mu/RecFifo.h
 *
 */
__INLINE__ void ppc_msync(void) { do { asm volatile ("msync" : : : "memory"); } while(0); }


#endif /* _KERNEL_FIRMWARE_KERNEL_IMPL_H_ */
