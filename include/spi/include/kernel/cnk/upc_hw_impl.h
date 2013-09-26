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
 * \brief UPC compile time cnk application definitions
 *
 */
//@}

#include "stdio.h"
#include "stdlib.h"
#include <hwi/include/bqc/PhysicalMap.h>
#include "spi/include/kernel/location.h"
#include "hwi/include/bqc/upc_p_hw.h"



__BEGIN_DECLS



#define UPC_C_MMIO_ADDR (PHYMAP_MINADDR_UPC )     // 0x3FDFE400000
#define UPC_USER_THRLOCAL __thread                //!< make vars thread local in user state
#define DCRWriteCommon DCRWriteUser  


#define UPC_FATAL_ERRIMPL(loc, ...) \
{ \
    fprintf(stderr,"UPCI INTERNAL ERROR (" loc ") " __VA_ARGS__);  \
    UPC_Assert(0);            \
    exit(-1);                 \
}



#define UPC_CRITICAL_WARNIMPL(loc, ...) \
{ \
    fprintf(stderr,"UPCI CRITICAL WARNING (" loc ") " __VA_ARGS__);  \
}


#ifdef UPC_C_C
upc_c_mmio_t *const  upc_c = (upc_c_mmio_t *const) UPC_C_MMIO_ADDR;
#else
extern upc_c_mmio_t *const  upc_c;
#endif

#ifdef UPC_P_C
upc_p_mmio_t *const upc_p_local = (upc_p_mmio_t*const)(UPC_P_MMIO_ADDR);
#else
extern upc_p_mmio_t *const upc_p_local;
#endif



__INLINE__ upc_p_mmio_t * UPC_P_Addr(int unit)
{
    upc_p_mmio_t * upc_p;

#if 0   // old flexible implementation - in case hardware is fixed.
    // return local mmio if target is on current core
    if ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()))
    {                                                        //!< \todo double-check perf with split address
        upc_p = (upc_p_mmio_t*)(UPC_P_MMIO_ADDR);            // UPC_P_MMIO_THREAD_MASK(PhysicalThreadID()));
    }
    // return DCR mmio if target is remote core
    else {
        upc_p = (upc_p_mmio_t *)( DCR_UPC_P_ADDR(unit)) ;
    }
#else
    assert ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()));
    upc_p = (upc_p_mmio_t*)(UPC_P_MMIO_ADDR);            // UPC_P_MMIO_THREAD_MASK(PhysicalThreadID()));
#endif
    return upc_p;
}



__INLINE__ volatile uint64_t * UPC_L1p_Addr(int unit)
{
    volatile uint64_t * pl1p;

#if 0   // old flexible implementation - in case hardware is fixed.
    // return local mmio if target is on current core
    if ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID())) {
        pl1p = (volatile uint64_t *)L1P_CFG_UPC_UADDR;
    }
    // return DCR mmio if target is remote core
    else {
        pl1p = (volatile uint64_t *)DCR_L1P_CFG_UPC_ADDR(unit);
    }
#else
    assert ((unit < 0) || (unit == (int)Kernel_PhysicalProcessorID()));
    pl1p = (volatile uint64_t *)L1P_CFG_UPC_UADDR;
#endif
    return pl1p;
}






__END_DECLS


#endif
