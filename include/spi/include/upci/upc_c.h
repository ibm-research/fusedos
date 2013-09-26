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

#ifndef _UPC_C_H_
#define _UPC_C_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC_C Low level functions
 */
//@}


//! UPC_C register mappings.
//! This header expands the UPC_C Register definitions from specific bit definitions
//! to the more general MMIO Structure of the register layouts

#include <hwi/include/bqc/PhysicalMap.h>
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <hwi/include/bqc/A2_inlines.h>
#endif                                                                                                                 // FUSEDOS
#include <hwi/include/bqc/dcr_support.h>
//#include <hwi/include/bqc/MU_Addressing.h>

#include "spi/include/kernel/upc_hw.h"
#include "spi/include/upci/upc_p.h"
#include "upc_c_dcr_wrapper.h"
#include "upc_common.h"
#include "upc_atomic.h"



__BEGIN_DECLS



//! \todo reorder the upc_c.h routines into a more logical order in this header file
//! \todo Need routines or instructions about how to query and parse the "info" registers after a bad interrupt





// UPC_C Control Functions

//! \brief: UPC_C_Enable_Mode1_Counter
//! Enable Mode 1 UPC_C counter - hides the multiple register interface in one function.
//! @param[in]  counter  Counter number 0 - 91
__INLINE__ void UPC_C_Enable_Mode1_Counter( int counter );



//! \brief: UPC_C_Reset_Mode1_Counter
//! Reset Mode 1 UPC_C counter - hides the multiple register interface in one function.
//! @param[in]  counter  Counter number 0 - 91
__INLINE__ void UPC_C_Reset_Mode1_Counter( int counter );



#ifdef __KERNEL__
//! \brief: UPC_C_Full_Clear
//! Clear out UPC_C, SRAM and all error interrupt settings.
//! Initialized state: clear out all of UPC_C including counters
void UPC_C_Full_Clear( );

#endif


//! \brief: UPC_C_Clear
//! Clear out UPC_C but leave the error interrupt settings alone.
//! Initialized state: mode1 counters reset, other counters left alone.
void UPC_C_Clear( );


//! \brief: UPC_C_Clear_SRAM
//! Clear  SRAM, leave all other settings alone.
void UPC_C_Clear_SRAM( );



//! \brief: UPC_C_Init_Mode
//! Initialize UPC_C Unit and UPC_P Units with the given counting mode base configuration
//! Notes:
//! 1) UPC_C should always be configured before UPC_P.  This routine will do that.
//!    IO Counters are also configured by this routine.
//!    UPC_P counters still require a subsequent config.
//! 2) In distributed mode (0), all of the UPC_P and L2 unit rings will be enabled.
//!    In other modes, all but the calling unit will be disabled
//!    IO and L2 counts will be put the passed in ctr_unit_mode, but UPC_P counters require
//!    separate configuration steps.
//! 3) L2 counters are always set to combine in the UPC_C SRAM (counter16 block 32).
//!
//! @param[in]  upcMode     The UPC mode to initialize on UPC_C
//! @param[in]  ctrMode     Indicate how to initialize other units and whether to also initialize upc_p and L2 unit mode and rings - normally yes
//! @param[in]  unit        Which UPC_P unit to attach to UPC_C for UPC modes 1 & 2
//!                         (-1) \ref UPC_LOCAL_UNIT indicates to use the UPC_P associated with the
//!                              the current hw thread.
void UPC_C_Init_Mode( int upcMode, UPC_Ctr_Mode_t ctrMode, short unit);



//! \brief: UPC_C_Disable
//! Put UPC_C to sleep, and optionally all other units.
//! @param[in]  ctrMode  Indicate whether to also initialize upc_p and L2 unit mode and rings - normally yes
void UPC_C_Disable( UPC_Ctr_Mode_t ctrMode);


#ifdef __KERNEL__

//! Interrupt type encoding
#define UPC_NO_INTERRUPT             0x0
#define UPC_NONCRITICAL_INTERRUPT    0x1
#define UPC_CRITICAL_INTERRUPT       0x2
#define UPC_MACHINE_CHECK            0x3

#define UPC_C_DEF_STATE_INTR  \
    ( UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_set(UPC_NONCRITICAL_INTERRUPT)     \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_set(UPC_CRITICAL_INTERRUPT)  \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_set(UPC_CRITICAL_INTERRUPT) \
    )

#define UPC_C_DEF_INTERNAL_ERROR_INTR  \
    ( UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(UPC_CRITICAL_INTERRUPT)     \
    | UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(UPC_CRITICAL_INTERRUPT)  \
    | UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(UPC_CRITICAL_INTERRUPT) \
    | UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(UPC_CRITICAL_INTERRUPT) \
    )



//! \brief: UPC_C_Init_Error_Interrupts
//! Privileged only
//! Configure bad error interrupts to cause critical, non-critical errors.
//! No UPC error should end in a MachineCheck, but rather indicate
//! Critical or non-critical errors.  The default masks need to be
//! passed to the routine.
__INLINE__ void UPC_C_Init_Error_Interrupts( uint64_t stateCtl, uint64_t internalErrCtl)
{
    DCRWritePriv( UPC_C_DCR(UPC_C_INTERRUPT_STATE_CONTROL_LOW), stateCtl);
    DCRWritePriv( UPC_C_DCR(INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH), internalErrCtl);
}
#endif




//! \brief: UPC_C_Start_Counting
//! Sends counter enable to UPC_Ps and other units to start counting (only use when counter config "sync_override" is false.
//! When "sync" start, counters in Punits,L2, and I/O units are synchronized.  However, it takes 800 cycles
//! for all units to start counting.
__INLINE__ void UPC_C_Start_Sync_Counting( )
{
    upc_c->counter_start = UPC_C_DCR__COUNTER_START__ENABLE_set(1ULL);
    mbar();
}



//! \brief: UPC_C_Start_Counting_Delay()
//! Start Sync and Delay the required 800 cycles to insure.
#define UPC_SYNC_START_DELAY  (800)  //!< # of cycles it takes for all counters to start in when counters are synced.
__INLINE__ void UPC_C_Start_Sync_Counting_Delay( )
{
    UPC_C_Start_Sync_Counting();
    Upci_Delay(UPC_SYNC_START_DELAY);
}



//! \brief: UPC_C_Stop_Counting
//! Sends counter disable to UPC_Ps and other units to start counting (only use when counter config "sync_override" is false.
__INLINE__ UpciBool_t UPC_C_Stop_Sync_Counting( )
{
    UpciBool_t curState = (upc_c->counter_start != 0);
    upc_c->counter_start = UPC_C_DCR__COUNTER_START__ENABLE_set(0ULL);
    mbar();
    return curState;
}



//! \brief: UPC_C_Stop_Counting_Delay
//! Stop Sync and Delay the required 800 cycles to insure all counts are updated before further operations.
__INLINE__ void UPC_C_Stop_Sync_Counting_Delay( )
{
    UPC_C_Stop_Sync_Counting();
    Upci_Delay(UPC_SYNC_START_DELAY*2);
}





// Tracing Function Hints:
//    See "A2 Support for instruction sampling"
//        "A2 Support for core instruction trace"


// Defines for bits within a parsed trace record (each record is 64 bits)
#define UPC_M2TRACEREC__DATA_get(x)            _BGQ_GET(44,63,x)
#define UPC_M2TRACEREC__DATA_set(v)            _BGQ_SET(44,63,v)
#define UPC_M2TRACEREC__TRIGGER_get(x)         _BGQ_GET(4,19,x)
#define UPC_M2TRACEREC__TRIGGER_set(v)         _BGQ_SET(4,19,v)



#define UPC_MAX_TRACE_RECS 1536
typedef struct {
    uint64_t rec[UPC_MAX_TRACE_RECS];
    short    trigger_rec;
    short    triggered;
} upc_trace_buff_t;



//! \brief: UPC_C_Extract_M2_Trace_Records
//! the Mode 2 trace records stored in the UPC_C SRAM are spread into multiple pieces.
//! This function will extract those pieces and form them up into a coherent buffer of records
//! in memory, with the earlier record at the start, and with the trigger offset set
//! to the record where the trigger occurred.
//! The trace always captures 6 records at a time, so the trigger actually occurred from
//! one of the 6 records starting at the trigger offset.
//!
//! @param[in]  tbuff  pointer to trace buffer to fill out.
//!
//! Requirements:  Assumes UPC_C trace is stopped.
//!                Assumes UPC_C is still configured for the current trace - # of recs after trigger, etc.
//! Note: See UPC_C SRAM format comments in upc_c_dcr_wrapper.h
void UPC_C_Extract_M2_Trace_Records(upc_trace_buff_t *tbuff);



//! \brief: UPC_C_Start_M2_Capture
//! Assuming the UPC_C mode is configured for trace (mode 2),
//! Start UPC_C capturing data.
//!
//! @param[in]  final_x1_cycles - indicates number of x1 to keep after the trigger occurs
//!                                 (will be rounded up to a factor of 6)
//! @param[in]  int_when_done   - set UpciTrue if ?? interrupt is desired when trace is done.
void UPC_C_Start_M2_Capture(int final_x1_cycles, UpciBool_t int_when_done);


__END_DECLS

#endif


