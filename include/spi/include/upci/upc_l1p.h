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

#ifndef _UPC_L1P_H_
#define _UPC_L1P_H_

//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * UPC Low Level API Operation Definitions for "other" misc units.
 */
//@}





#include <hwi/include/bqc/PhysicalMap.h>
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <hwi/include/bqc/A2_inlines.h>
#endif                                                                                                                 // FUSEDOS
#include <hwi/include/bqc/l1p_mmio.h>

#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include "spi/include/kernel/location.h"
#include "spi/include/kernel/upc_hw.h"
#endif                                                                                                                 // FUSEDOS

#include "upc_p.h"
#include "upc_c.h"


__BEGIN_DECLS




//! \brief: UPC_L1p_SetMode
//! Select L1p Event Mode for given unit and enable events to UPC_P
//! @param[in]  unit        The core index of the UPC_P unit. A value < 0 means local unit.
//! @param[in]  l1p_mode    L1P_CFG_UPC_LIST, L1P_CFG_UPC_SWITCH, or L1P_CFG_UPC_STREAM
__INLINE__ void UPC_L1p_SetMode(short unit, uint64_t l1p_mode)
{
    volatile uint64_t * l1p_p = UPC_L1p_Addr(unit);
    *l1p_p = L1P_CFG_UPC_ENABLE | l1p_mode;
    mbar();
}



//! \brief: UPC_L1p_EnableUPC
//! Enable unit to send events to UPC_P
//! @param[in]  unit  The core index of the UPC_P unit. A value < 0 means local unit.
__INLINE__ void UPC_L1p_EnableUPC(short unit)
{
    volatile uint64_t * l1p_p = UPC_L1p_Addr(unit);
    *l1p_p |= L1P_CFG_UPC_ENABLE;
    mbar();
}



//! \brief: UPC_L1p_DisableUPC
//! Enable unit to send events to UPC_P
//! @param[in]  unit  The core index of the UPC_P unit. A value < 0 means local unit.
__INLINE__ void UPC_L1p_DisableUPC(short unit)
{
    volatile uint64_t * l1p_p = UPC_L1p_Addr(unit);
    *l1p_p &= ~L1P_CFG_UPC_ENABLE;
    mbar();
}




__END_DECLS

#endif


