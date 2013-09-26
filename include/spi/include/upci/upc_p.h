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

#ifndef _UPC_P_H_
#define _UPC_P_H_

//! \addtogroup upci_spi
//@{
//! \file
//! UPC_P Low Level API Operation Definitions
//@}


#include <hwi/include/bqc/PhysicalMap.h>
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <hwi/include/bqc/A2_inlines.h>
#endif                                                                                                                 // FUSEDOS
//#include <hwi/include/bqc/MU_Addressing.h>
#include <hwi/include/bqc/l1p_mmio.h>


#include "hwi/include/bqc/upc_p_hw.h"
#include "spi/include/kernel/upc_hw.h"
#include "upc_atomic.h"



//! \brief: UPC_Ctr_Mode
//! Indicates how the counters should be controlled and integrate depending also on the
//! the UPC mode.
typedef enum UPC_Ctr_Mode {
    UPC_CM_NO_INIT,    //!< Don't initialize any subunits - calling code will do that explicitly
    UPC_CM_DISCON,     //!< Run UPC_P units disconnected from UPC_C ring, thus all counter controlled independently
    UPC_CM_INDEP,      //!< Init units for independent counter start
    UPC_CM_SYNC,       //!< Init units and set L2 & IO counters to sync their start/stop on UPC_C_Start_Sync_Counting() call
} UPC_Ctr_Mode_t;




__BEGIN_DECLS


//! \brief: Create a 64 bit ThreadBitMask from hardware thread ID number.  Repeats when thread id > 63)
#define UPC_APP_HWTID_MASK(_hwthd_) (0x8000000000000000ULL >> (_hwthd_))
#define UPC_AGENT_HWTID_MASK(_hwthd_) ((0x8000000000000000ULL >> ((_hwthd_)%64)) * ((_hwthd_)/64))


//! \brief: UPC_P Counter Event Groups
//! List of counter sources supported by the counter_cfg
#define UPC_P_CEG_A2        0ULL     //!< Configure to count A2 Events
#define UPC_P_CEG_OPCODES   1ULL     //!< Configure to count OpCodes
#define UPC_P_CEG_L1PWU1    2ULL     //!< Configure to count lower L1p/wake events (groups 0-3)
#define UPC_P_CEG_L1PWU2    3ULL     //!< Configure to count higher L1p events (groups 4-6)



//! \brief: UPC_P Counter Sub Group Events
//! List of SUB group supported by the counter_cfg
#define UPC_P_SUBG_MMU        0ULL     //!< Configure to count MMU
#define UPC_P_SUBG_AXU        1ULL     //!< Configure to count AXU (QFPU)
#define UPC_P_SUBG_IU         2ULL     //!< Configure to count IU
#define UPC_P_SUBG_LSU        3ULL     //!< Configure to count LSU

#define UPC_P_NUM_A2_UNITS     5      //!< Number of Event Modules in A2.  XU unit is
                                      //!< Handled on PC Bus and signals can't be remapped.
#define UPC_P_NUM_A2_UNIT_SIGS 8      //!< Number of signals per a2 unit



//! \brief: XU Op Groups
//! List of XU operation groups supported by the counter_cfg
// note: This list of defines copied from XU sheet of "upc_opcode_grps.xls"
#define UPC_P_XU_OGRP_FLD   0x8000000ULL //!< Floating Point Load
#define UPC_P_XU_OGRP_FST   0x4000000ULL //!< Floating Point Store
#define UPC_P_XU_OGRP_QLD   0x2000000ULL //!< Quad Floating Point Load
#define UPC_P_XU_OGRP_QST   0x1000000ULL //!< Quad Floating Point Store
#define UPC_P_XU_OGRP_BITS  0x0800000ULL //!< Bit manipulation
#define UPC_P_XU_OGRP_BRC   0x0400000ULL //!< Branch Conditional
#define UPC_P_XU_OGRP_BRU   0x0200000ULL //!< Branch Unconditional
#define UPC_P_XU_OGRP_CINV  0x0100000ULL //!< Cache Invalidate
#define UPC_P_XU_OGRP_CSTO  0x0080000ULL //!< Cache Store
#define UPC_P_XU_OGRP_CTCH  0x0040000ULL //!< Cache Touch
#define UPC_P_XU_OGRP_IADD  0x0020000ULL //!< Integer Arithmetic
#define UPC_P_XU_OGRP_ICMP  0x0010000ULL //!< Compare
#define UPC_P_XU_OGRP_ICSW  0x0008000ULL //!< Coprocessor
#define UPC_P_XU_OGRP_IDIV  0x0004000ULL //!< integer divide
#define UPC_P_XU_OGRP_ILOG  0x0002000ULL //!< Logical
#define UPC_P_XU_OGRP_IMOV  0x0001000ULL //!< Quick Register move
#define UPC_P_XU_OGRP_IMUL  0x0000800ULL //!< integer multiply
#define UPC_P_XU_OGRP_INT   0x0000400ULL //!< Interrupt/system call
#define UPC_P_XU_OGRP_LD    0x0000200ULL //!< Load
#define UPC_P_XU_OGRP_LSCX  0x0000100ULL //!< Load & Store reserve conditional
#define UPC_P_XU_OGRP_ST    0x0000080ULL //!< Store
#define UPC_P_XU_OGRP_SYNC  0x0000040ULL //!< Context Sync
#define UPC_P_XU_OGRP_TLB   0x0000020ULL //!< TLB other
#define UPC_P_XU_OGRP_TLBI  0x0000010ULL //!< TLB Invalidate
#define UPC_P_XU_OGRP_WCH   0x0000008ULL //!< Watch
#define UPC_P_XU_OGRP_CYCLES    0x0000004ULL //!< x1 cycles
#define UPC_P_XU_OGRP_MATCH 0x0000002ULL //!< Match XU OpCode
#define UPC_P_XU_OGRP_MASK  0x0000000ULL //!< Initial Generic Mask (starts out with none)
#define UPC_P_XU_OGRP_ALL   0xFFFFFF8ULL //!< All XU Op Codes



//! \brief: AXU Op Groups
//! List of AXU operation groups supported by the counter_cfg
// note: This list of defines copied from AXU sheet of "upc_opcode_grps.xls"
#define UPC_P_AXU_OGRP_FOTH 0x200000ULL  //!< Floating Point Other
#define UPC_P_AXU_OGRP_FRES 0x100000ULL  //!< Floating Point Reciprocal Estimate
#define UPC_P_AXU_OGRP_FADD 0x080000ULL  //!< Floating Point Add
#define UPC_P_AXU_OGRP_FCVT 0x040000ULL  //!< Floating Point Convert
#define UPC_P_AXU_OGRP_FCMP 0x020000ULL  //!< Floating Point Compare
#define UPC_P_AXU_OGRP_FDIV 0x010000ULL  //!< Floating Point Divide
#define UPC_P_AXU_OGRP_FMOV 0x008000ULL  //!< Floating Point Move
#define UPC_P_AXU_OGRP_FMA  0x004000ULL  //!< Floating Point Multiply-Add
#define UPC_P_AXU_OGRP_FMUL 0x002000ULL  //!< Floating Point Multiply
#define UPC_P_AXU_OGRP_FRND 0x001000ULL  //!< Floating Point Round
#define UPC_P_AXU_OGRP_FSQE 0x000800ULL  //!< Floating Point Square Root Estimate
#define UPC_P_AXU_OGRP_FSQ  0x000400ULL  //!< Floating Point Square Root
#define UPC_P_AXU_OGRP_QMOV 0x000200ULL  //!< Quad Floating Point Move
#define UPC_P_AXU_OGRP_QOTH 0x000100ULL  //!< Quad Floating Point Other
#define UPC_P_AXU_OGRP_QADD 0x000080ULL  //!< Quad Floating Point Add
#define UPC_P_AXU_OGRP_QMUL 0x000040ULL  //!< Quad Floating Point Multiply
#define UPC_P_AXU_OGRP_QRES 0x000020ULL  //!< Quad Floating Point Reciprocal Estimate
#define UPC_P_AXU_OGRP_QMA  0x000010ULL  //!< Quad Floating Point Multiply-Add
#define UPC_P_AXU_OGRP_QRND 0x000008ULL  //!< Quad Floating Point Round
#define UPC_P_AXU_OGRP_QCVT 0x000004ULL  //!< Quad Floating Point Convert
#define UPC_P_AXU_OGRP_QCMP 0x000002ULL  //!< Quad Floating Point Compare
#define UPC_P_AXU_OGRP_UCODE 0x010400UL  //<! uCoded AXU Operations
#define UPC_P_AXU_OGRP_MATCH 0x000001ULL  //!< AXU Opcode Match
#define UPC_P_AXU_OGRP_MASK 0x000000ULL  //!< AXU Opcode Generic Mask (starts out with none selected)
#define UPC_P_AXU_OGRP_ALL  0x3FFFFEULL  //!< ALL AXU Instructions
#define UPC_P_AXU_OGRP_FPGRP1        0x3F7DFEULL  //!< Set #1 Floating Point Groups
#define UPC_P_AXU_OGRP_FPGRP1_SINGLE 0x3F7C00ULL  //!< Set #1 Single Floating Point Groups
#define UPC_P_AXU_OGRP_FPGRP1_QUAD   0x0001FEULL  //!< Set #1 Quad Floating Point Groups
#define UPC_P_AXU_OGRP_FPGRP2        0x196CF0ULL  //!< Set #2 Floating Point Groups
#define UPC_P_AXU_OGRP_FPGRP2_SINGLE 0x196C00ULL  //!< Set #2 Single Floating Point Groups
#define UPC_P_AXU_OGRP_FPGRP2_QUAD   0x0000F0ULL  //!< Set #2 Quad Floating Point Groups

#define UPC_P_OGRP_ALL 0ULL              //!< All instructions - value is meaningless - code will use combination of XU and AXU ALL masks to select all instructions



#define UPC_DISTRIB_MODE     (0)
#define UPC_DETAIL_MODE      (1)
#define UPC_TRACE_MODE       (2)


// Define gymnastics to create a compile time AT string.
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define _AT_ __FILE__ ":" TOSTRING(__LINE__)




// UPC_P Control Functions
//  For both local and remote UPC Units.

//! \brief: UPC_P_Disable_Unit
//! Disable (quiesce) the local or remote UPC_P units operations.
//! @param[in]  unit  The core index of the UPC_P unit to disable. A value < 0 means disable local unit.
//! example: UPC_P_Disable_Unit( UPC_LOCAL_UNIT );
//! \note Not thread safe - caller must sync per unit
void UPC_P_Disable_Unit( int unit );



//! \brief: UPC_P_Clear_Unit
//! Disable (quiesce) and clear all registers in the UPC_P unit
//! @param[in]  unit  The core index of the UPC_P unit to disable. A value < 0 means disable local unit.
//! example: UPC_P_Clear_Unit( UPC_LOCAL_UNIT );
//! \note Not thread safe - caller must sync per unit
void UPC_P_Clear_Unit( int unit );



//! \brief: UPC_P_Stop_Counters
//! Stop all counting on this unit.
//! @param[in]  unit  The core index of the UPC_P unit
//! example: UPC_P_Stop_Counters( UPC_LOCAL_UNIT );
//! \note Not thread safe - caller must sync per unit
__INLINE__ void UPC_P_Stop_Counters( int unit )
{
    upc_p_mmio_t *const upc_p = UPC_P_Addr(unit);
    upc_p->control_w1c = UPC_P__CONTROL__RUN_MASK;
    mbar();
}



//! \brief: UPC_P_Init_Unit_Mode
//! Initialize UPC_P Unit with the given counting mode base configuration but without any active counters
//! Initialized state: All interrupts, regs and counters disabled, cleared or reset except for
//!   - join ring enabled (unless "disconnected" is UpciTrue)
//!   - clock gate enabled.
//!   - Mode set.
//!   - thread combining masks set to b'1000'
//! @param[in]  unit          The core index of the UPC_P unit to enable. A value < 0 means enable current hw threads unit.
//! @param[in]  mode          The UPC mode to initialize on this UPC_P
//! @param[in]  ctrMode       Indicate how the counters will be used, whether to attach to ring to set DD1 mode workaround.
//! example: UPC_P_Init_Unit_Mode( UPC_LOCAL_UNIT, 0, UpciFalse);
//! \note Not thread safe - caller must sync per unit
void UPC_P_Init_Unit_Mode( int unit, int mode, UPC_Ctr_Mode_t ctrMode);



//! \brief: UPC_P_Enable_PM_Ints
//! Initialize UPC_P to raise A2 interrupts on overflow
//!   Enables interrupts from all 3 sources - little overflow, big overflow, and upc_c overflow to all 4 threads.
//!   Will not change existing state of config (only sets given interrupt for threads)
//!   Also does not change "shared interrupts".
//! @param[in]  unit        The core index of the UPC_P unit to set A value < 0 means local unit.
//! @param[in]  thread      The thread number to enable (or UPC_PM_ENABLE_ALLINT)
//! \note Not thread safe - caller must sync per unit
#define UPC_PM_ENABLE_ALLINT (-1)
void UPC_P_Enable_PM_Ints( int unit, short thread);



//! \brief: UPC_P_M2_Trigger
//! Send software Mode 2 Trigger to stop the mode 2 trace.
//! Use with UPC Mode 2 and with UPC_C_Extract_M2_Trace_Records, and UPC_C_Start_M2_Capture operations.
//! Must init SCOM accessable A2 debug registers to configure what is received.
//!
//! @param[in]  upc_p pointer  - ptr to upc_p unit - use UPC_P_Addr to get prior to starting trace.
//! \note Not thread safe - caller must sync per unit
__INLINE__ void UPC_P_M2_Trigger(upc_p_mmio_t * upc_p)
{
    upc_p->m2_trigger = UPC_P__M2_TRIGGER_set(1);
}


__END_DECLS

#endif


