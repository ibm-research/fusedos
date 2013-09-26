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
/* (C) Copyright IBM Corp.  2010, 2012                              */
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

#ifndef _KERNEL_UPC_HW_IMPL_H_
#define _KERNEL_UPC_HW_IMPL_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC compile time firmware definitions
 *
 */
//@}

#include <assert.h>
#include <hwi/include/bqc/PhysicalMap.h>

#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/MU_Addressing.h>
#include <hwi/include/bqc/l1p_mmio.h>
#include <hwi/include/bqc/dc_arbiter_dcr.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/testint_tdr.h>

#include "spi/include/kernel/location.h"
#include "hwi/include/bqc/upc_p_hw.h"


__BEGIN_DECLS



#define UPC_C_MMIO_ADDR (PHYMAP_MINADDR_UPC | PRIV_ADDR_MASK )      // 0x3FFFE400000
#define UPC_USER_THRLOCAL                                           //!< make vars the default thread global in kernel state
#define DCRWriteCommon DCRWritePriv  



//! \todo How could  I "gracefully" terminate from the same call within user state, fwext and cnk?
//!       what do I want to do with fatal errors within cnk?  Probably create a RAS msg
//!       and then is a long jump to the syscall return possible and appropriate?
//!       Terminate(-1); don't know how to terminate within CNK
//!       so I'll force a segfault to get a stack dump.
#define UPC_FATAL_ERRIMPL(loc, ...) \
{ \
    printf("INTERNAL ERROR (" loc ") " __VA_ARGS__);  \
    volatile int foo = *((volatile int*)NULL);  \
    assert(0==1); \
    foo = 1; \
}



#define UPC_CRITICAL_WARNIMPL(loc, ...) \
{ \
    printf("CRITICAL WARNING (" loc ") " __VA_ARGS__);  \
}



#ifdef UPC_C_C
upc_c_mmio_t *const  upc_c = (upc_c_mmio_t *const) UPC_C_MMIO_ADDR;
#else
extern upc_c_mmio_t *const  upc_c;
#endif

#ifdef UPC_P_C
upc_p_mmio_t *const upc_p_local = (upc_p_mmio_t*const)(UPC_P_MMIO_ADDR | PRIV_ADDR_MASK);
#else
extern upc_p_mmio_t *const upc_p_local;
#endif


__INLINE__ upc_p_mmio_t * UPC_P_Addr(int unit)
{
    upc_p_mmio_t * upc_p;

#ifdef __FWEXT__  // Allow full implementation only in a fwext test.
    // return local mmio if target is on current core
    if ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()))
    {                                                              //!< \todo double-check perf with split address
        upc_p = (upc_p_mmio_t*)(UPC_P_MMIO_ADDR | PRIV_ADDR_MASK); // UPC_P_MMIO_THREAD_MASK(PhysicalThreadID()));
    }
    // return DCR mmio if target is remote core
    else {
        upc_p = (upc_p_mmio_t *)( DCR_UPC_P_ADDR(unit) | PRIV_ADDR_MASK ) ;
    }
#else
    assert ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()));
    #if 0
    if (!((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()))) {
        // create exception so can get stack dump in CNK
        volatile char *foo = NULL;
        volatile char fooey = *foo;
    }
    #endif
    upc_p = (upc_p_mmio_t*)(UPC_P_MMIO_ADDR | PRIV_ADDR_MASK); // UPC_P_MMIO_THREAD_MASK(PhysicalThreadID()));
#endif
    return upc_p;
}



__INLINE__ volatile uint64_t * UPC_L1p_Addr(int unit)
{
    volatile uint64_t * pl1p;

#ifdef __FWEXT__  // Allow full implementation only in a fwext test.
    // return local mmio if target is on current core
    if ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID())) {
        pl1p = (volatile uint64_t *)(L1P_CFG_UPC_UADDR | PRIV_ADDR_MASK);
    }
    // return DCR mmio if target is remote core
    else {
        pl1p = (volatile uint64_t *)(DCR_L1P_CFG_UPC_ADDR(unit) | PRIV_ADDR_MASK);
    }
#else
    assert ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()));
    pl1p = (volatile uint64_t *)(L1P_CFG_UPC_UADDR | PRIV_ADDR_MASK);
#endif
    return pl1p;
}






__END_DECLS


#endif
