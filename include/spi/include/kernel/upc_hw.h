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

#ifndef _KERNEL_UPC_HW_H_
#define _KERNEL_UPC_HW_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC compile time definitions which vary
 * between firmware or user application compiles
 *
 */
//@}


#include <hwi/include/bqc/PhysicalMap.h>
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/testint_inlines.h>
#endif                                                                                                                 // FUSEDOS
#include <hwi/include/bqc/l1p_mmio.h>


#include "spi/include/upci/upc_c_dcr_wrapper.h"
#include "spi/include/upci/upc_common.h"


#define L1P_CFG_UPC_UADDR (L1P_CFG_UPC & ~PRIV_ADDR_MASK)
#define DCR_L1P_CFG_UPC_ADDR(CORE_INDEX) ( DCR_CORE0_ADDR + ( DCR_NEXT_CORE_OFFSET * CORE_INDEX ) + DCR_L1P_ADDR_OFFSET \
                                         + (L1P_CFG_UPC_UADDR - PHYMAP_MINADDR_L1P))
#include "upc_hw_impl.h"

__BEGIN_DECLS

//#define BGQ_GetNumThreads() 68


#ifndef LIKELY
#define LIKELY(x)       __builtin_expect(!!(x), 1)
#define UNLIKELY(x)     __builtin_expect(!!(x), 0)
#endif



//! Define gymnastics to create a compile time AT string.
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define _AT_ __FILE__ ":" TOSTRING(__LINE__)



//! \brief: UPC_FATAL_ERR
//! Macro to handle a fatal error and message given the kernel or user implementation.d
#define UPC_FATAL_ERR(...) UPC_FATAL_ERRIMPL(_AT_ " ", __VA_ARGS__)



//! \brief: UPC_CRITICAL_WARNING
//! Macro to send a critical but non-fatal warning message
#define UPC_CRITICAL_WARNING(...) UPC_CRITICAL_WARNIMPL(_AT_ " ", __VA_ARGS__)



//! \brief: UPC_P_Addr
//! Return Local or Remote address of UPC_P Unit
//! @param[in]  core_index  The core index of the UPC_P unit. A value < 0 means local unit.
//! @return     upc_p structure address
//! example: upc_p_mmio_t *upc_p = UPC_P_Addr( UPC_LOCAL_UNIT );
//! WARNING:  Due to a hardware defect (DCR Timeout), when running under cnk, the core_index must always be for the
//!           current core.  Otherwise this routine will assert.
//!           The exception is when running with -D__FWEXT__ test, such that the test has full control
//!           and is unlikely to try reading a local mmio address while another thread is doing a cross
//!           core DCR access.
//!           The CNK implementation will use core2core interrupts to always access UPC_P on the local core.
#define UPC_LOCAL_UNIT (-1)   //!< integer value indicating to operate on local unit versus a remote unit (via DCR).
upc_p_mmio_t *      UPC_P_Addr(int unit);


volatile uint64_t * UPC_L1p_Addr(int unit);


__END_DECLS

#endif


