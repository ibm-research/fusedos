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

#ifndef _UPC_L2_H_
#define _UPC_L2_H_

//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * UPC Low Level API Operation Definitions for L2 unit.
 */
//@}



#include "spi/include/kernel/location.h"
#include "spi/include/kernel/upc_hw.h"

#include <hwi/include/bqc/l2_dcr.h>

#include "upc_c.h"

#include "upc_p.h"


__BEGIN_DECLS

//! \brief: UPC_L2_EnableUPC
//! Enable units to send events to UPC_C
//! \note 04/08/11 - Contract changed again.  indepCtl will be ignored. Rather, we will
//!                  assume that punit and I/O type counters all have sync override set.
//!                  Thus, we can use the UPC_C Sync Start to only control the L2 counters
//!                  and achieve a much better start/stop time for L2 counters.
//! \note 11/16/10 - Contract changed: requires separate start/stop
//!                  operation.
//! @param[in]  indepCtl   synchronous or independent control
//! @param[in]  combine   Combine L2 slice counts into single set of L2 counts.
//! @note Requires UPC mode to be Mode 0.
void UPC_L2_EnableUPC(UpciBool_t indepCtl, UpciBool_t combine);



//! \brief: UPC_L2_Start
//! Start Enabled L2 Counters
//! (when set for independent control - use no effect otherwise)
//! @note Requires UPC mode to be Mode 0.
__INLINE__ void UPC_L2_Start() { UPC_C_Start_Sync_Counting_Delay(); }



//! \brief: UPC_L2_Stop
//! Stop Enabled L2 Counters
//! (when set for independent control - no effect otherwise)
//! @note Requires UPC mode to be Mode 0.
__INLINE__ void UPC_L2_Stop() { UPC_C_Stop_Sync_Counting_Delay(); }



//! \brief: UPC_L2_Reset
//! Reset L2 Counters
//! @note Requires UPC mode to be Mode 0.
void UPC_L2_Reset();



//! \brief L2 Counters
//! The events in the L2 counters are static.
//! There are only 6 counts (though UPC hardware can support up to 16 per slice)
//! Use these values with UPC_L2_ReadCtr
#define UPC_L2_CTR_HITS                   0
#define UPC_L2_CTR_MISSES                 1
#define UPC_L2_PREFETCH                   2   //!< fetching ahead of L1P prefetch
#define UPC_L2_FETCH_LINE                 3   //!< 128 byte line from main memory
#define UPC_L2_STORE_LINE                 4   //!< 128 byte line to main memory
#define UPC_L2_STORE_PARTIAL              5   //!< Store Partial line to main memory
#define UPC_L2_Req_Reject                 6   //!< L2 Req Reject
#define UPC_L2_Req_Retire                 7   //!< L2 Req Retire
#define UPC_L2_Req_Miss_and_Evict         8   //!< L2 Req Miss and Evict
#define UPC_L2_Req_Miss_and_Evict_Spec    9   //!< L2 Req Miss and Evict Spec
#define UPC_L2_Miss_and_Evict             10  //!< L2 Miss and Evict
#define UPC_L2_Miss_and_Evict_Spec        11  //!< L2 Miss and Evict Spec
#define UPC_L2_Req_Spec_Addr_Cleanup      12  //!< L2 Req Spec Addr Cleanup
#define UPC_L2_Spec_Addr_Cleanup          13  //!< L2 Spec Addr Cleanup
#define UPC_L2_Spec_Set_Cleanup           14  //!< L2 Spec Set Cleanup
#define UPC_L2_Spec_Retry                 15  //!< L2 Spec Retry

#define UPC_L2_NUM_COUNTERS  16  //!< Number of counts actually implemented



//! \brief: UPC_L2_ReadCtr
//! Read a numbered L2 Counter from UPC_C Sram.
//! @param[in]  slice        L2 slice (must be 0 when L2 counter slices are combined)
//! @param[in]  ctr          L2 counter number
//! @note Assumes UPC mode 0.
//! @note When counters are combined, only slice 0 is valid.  This function does not
//!       verify if l2 counters are combined.
__INLINE__ uint64_t UPC_L2_ReadCtr(unsigned slice, unsigned ctr)
{
    assert(ctr <= 16);
    assert(slice < L2_DCR_num);
    return (upc_c->data16.grp[slice + UPC_C_SRAM_BASE_L2_GROUP].counter[ctr]);
}



//! \brief UPC_L2_WriteCtr
//! Write a numbered L2 Counter
//! @param[in]  slice        L2 slice  (should be 0 when L2 counter slices are combined)
//! @param[in]  ctr          L2 counter number
//! @param[in]  value
//! @note
//! - Assumes UPC mode 0.
//! - When counters are combined, only slice 0 is valid.  This function does not
//!   verify if l2 counters are combined.
//! - Does not assume counters are stopped before update - so 800 pending event from L2 may
//!   also be added to value.
//! - There is no locking - multiple threads attempting this operation may collide
__INLINE__ void UPC_L2_WriteCtr(unsigned slice, unsigned ctr, uint64_t value)
{
    assert(ctr < 16);
    assert(slice < L2_DCR_num);
    upc_c->data16.grp[slice + UPC_C_SRAM_BASE_L2_GROUP].counter[ctr] = value;
}




//! \brief: UPC_L2_DisableUPC
//! Disable UPC and Ring in L2 Units
void UPC_L2_DisableUPC();







__END_DECLS

#endif


