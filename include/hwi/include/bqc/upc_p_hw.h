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


#ifndef _UPC_P_HW_H_
#define _UPC_P_HW_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC_P Hardware Macros
 *
 *  Manually generated to format similar to that produced by DCR auto generation process.
 *  (though not exactly, some extra things).
 *
 *
 */
//@}


#include <hwi/include/common/bgq_bitnumbers.h>

__BEGIN_DECLS


#define _PACK64_  __attribute__((__packed__)) __attribute__((aligned (8)))



//! Old "SETF64" defines have been replaced with BGQ_INSERT with different input parms.
//! Use _UPC_INSERT instead to translate between parms
#define _UPC_INSERT(b1,b2,x,v) _BGQ_INSERT((b2-b1+1),b2,x,v)



//! UPC_P register mappings.   See UPC doc Section 1.14.1
//! This header expands the UPC_P Register definitions from specific bit definitions
//! to the more general MMIO Structure of the register layouts

#define UPC_P_NUM_COUNTERS      24

//! The counter addrs documented are doubleword addresses, so to get byte addresses, add 3 more lsb bits.
//! UPC_P addrs are offset from the start of L1P MMIO space.
//!  0x 600
//!  0b 110 0000 0000 000
//!  0b 11 0000 0000 0000
//!  0x 3000
#define PRIV_ADDR_MASK                  0x00200000000ULL                    //!< Set bit for privledged region
#define UPC_P_L1P_OFFSET                0x3000                              //!< Offset into L1P mmio space to UPC_P memroy

//! The upc_p local mmio addresses have been duplicated by 4 to create a separate address space for each hardware thread.
//! The local mmio addresses for upc_p are 0xFFFF.  The actual addresses for the upc_p fit within 0x3FFF.
//! So, upc_p accesses ignore the upper two bits, which will create a separate address for each hw thread.
//! The separate addresses prevent the A2 from flushing and stalling threads when each thread is attempting to
//! access the same register, or registers in the same 64 byte cache line (even though mmio is cache inhibited)
#define UPC_P_MMIO_THREAD_MASK(thread)  (thread<<14)                        //!< address mask of hw thread value

#if !(defined __FUSEDOS__) || defined __SPCM__                                                                          // FUSEDOS
#define UPC_P_MMIO_ADDR (PHYMAP_MINADDR_L1P + UPC_P_L1P_OFFSET)     //!< 0x3FDE8003000
#else                                                                                                                 // FUSEDOS
extern void* mmio_nonpriv_base;                                                                                       // FUSEDOS
#define UPC_P_MMIO_ADDR ((uint64_t)mmio_nonpriv_base + (PHYMAP_MINADDR_L1P + UPC_P_L1P_OFFSET - PHYMAP_MINADDR_MMIO)) // FUSEDOS
#endif                                                                                                                // FUSEDOS


#define DCR_CORE0_ADDR          ( PHYMAP_MINADDR_DCR )           //!< DCR Core0 starting addr (used to access non-local UPC units
#define DCR_NEXT_CORE_OFFSET    (0x20000)                        //!< Offset to next core within DCR range
#define DCR_L1P_ADDR_OFFSET     (0x10000)                        //!< Offset within DCR range to L1P starting address
#define DCR_UPC_P_ADDR(CORE_INDEX) ( DCR_CORE0_ADDR + ( DCR_NEXT_CORE_OFFSET * CORE_INDEX ) + DCR_L1P_ADDR_OFFSET + UPC_P_L1P_OFFSET)



//! \brief:     upc_p_counter
//! Type:       Read/Write External
//! Permission: Read(All); Write(Privileged Only)
//! Note: set BIG_OVF_INT_ENABLE if interrupt is desired on a big overflow.
//! vhdl:  mmio_read_count_data <= (0 to 47 => '0') & mmio_read_ovf & mmio_read_armed & mmio_read_count;
#define UPC_P__COUNTER_offset                 (0x600)                   //!< unit offset
#define UPC_P__COUNTER_range                  (24)                    //!< Number of counters in unit
//! Bit Definitions:
#define UPC_P__COUNTER__BIG_OVF_get(x)       _BGQ_GET(1,48,x)
#define UPC_P__COUNTER__BIG_OVF_set(v)       _BGQ_SET(1,48,v)              //!< Big overflow - if counter wraps in single UPC_C scan cycle
#define UPC_P__COUNTER__BIG_OVF_insert(x,v)  _UPC_INSERT(48,48,x,v)

#define UPC_P__COUNTER__ARMED_get(x)         _BGQ_GET(1,49,x)
#define UPC_P__COUNTER__ARMED_set(v)         _BGQ_SET(1,49,v)              //!< Counter armed to interrupt on overflow (set by UPC_C scan when 13 bit theshold reached)
#define UPC_P__COUNTER__ARMED_insert(x,v)    _UPC_INSERT(49,49,x,v)

#define UPC_P__COUNTER__COUNT_get(x)         _BGQ_GET(14,63,x)
#define UPC_P__COUNTER__COUNT_set(v)         _BGQ_SET(14,63,v)             //!< Counter value. 1st bit (msb) is reset each UPC_C scan cycle.
#define UPC_P__COUNTER__COUNT_insert(x,v)    _UPC_INSERT(50,63,x,v)

#define UPC_P__COUNTER__STABLE_COUNT_set(v)  _BGQ_SET(13,63,v)             //!< Counter bits which are stable (not reset each ring cycle)          //!< Counter value just used for threshold
#define UPC_P__COUNTER__THRESHOLD_COUNT_get(x) _BGQ_GET(13,63,x)           //!< Counter value just used for threshold
//! set threshold to store already armed if 14th msb is already a 1 (ls 13 bits don't care)
#define UPC_P__COUNTER__THRESHOLD_COUNT_set(v) \
    ( ((uint64_t)(v) < 0xFFFFFFFFFFFFE000UL) ? _BGQ_SET(13,63,(v)) : UPC_P__COUNTER__ARMED_set(1) | _BGQ_SET(13,63,(v)) )




//! \brief: upc_p_counter_cfg
//!
#define UPC_P__COUNTER_CFG_offset                       (0x620)       //!< Config register for each counter
#define UPC_P__COUNTER_CFG_range                        (24)        //!< Number of counters in unit
//! Bit Definitions:
#define UPC_P__COUNTER_CFG__SYNC_OVERRIDE_get(x)        _BGQ_GET(1,1,x)
#define UPC_P__COUNTER_CFG__SYNC_OVERRIDE_set(v)        _BGQ_SET(1,1,v)             //!< 0=wait for UPC_C start/stop; 1=start immed. (Control Run Bits are ignored if disabled)
#define UPC_P__COUNTER_CFG__SYNC_OVERRIDE_insert(x,v)   _UPC_INSERT(1,1,x,v)

#define UPC_P__COUNTER_CFG__OPS_NOT_FLOPS_get(x)        _BGQ_GET(1,2,x)
#define UPC_P__COUNTER_CFG__OPS_NOT_FLOPS_set(v)        _BGQ_SET(1,2,v)             //!< 0=don't weight op counts; 1=weight counts based on config reg weights
#define UPC_P__COUNTER_CFG__OPS_NOT_FLOPS_insert(x,v)   _UPC_INSERT(2,2,x,v)

#define UPC_P__COUNTER_CFG__OP_THREAD_MASK_get(x)       _BGQ_GET(4,6,x)
#define UPC_P__COUNTER_CFG__OP_THREAD_MASK_set(v)       _BGQ_SET(4,6,v)             //!< enable threads to include in op code counting.
#define UPC_P__COUNTER_CFG__OP_THREAD_MASK_insert(x,v)  _UPC_INSERT(3,6,x,v)
#define UPC_P__COUNTER_CFG__OP_THREAD_MASK_bit(b)       (1UL<<(63-(3+(b))))

#define UPC_P__COUNTER_CFG__XU_OPGROUP_SEL_get(x)       _BGQ_GET(28,35,x)
#define UPC_P__COUNTER_CFG__XU_OPGROUP_SEL_set(v)       _BGQ_SET(28,35,v)           //!< mask of XU opcode groups to count
#define UPC_P__COUNTER_CFG__XU_OPGROUP_SEL_insert(x,v)  _UPC_INSERT(8,35,x,v)

#define UPC_P__COUNTER_CFG__AXU_OPGROUP_SEL_get(x)      _BGQ_GET(22,57,x)
#define UPC_P__COUNTER_CFG__AXU_OPGROUP_SEL_set(v)      _BGQ_SET(22,57,v)           //!< mask of AXU opcode groups to count
#define UPC_P__COUNTER_CFG__AXU_OPGROUP_SEL_insert(x,v) _UPC_INSERT(36,57,x,v)

#define UPC_P__COUNTER_CFG__EVENT_GROUP_SEL_get(x)      _BGQ_GET(2,59,x)
#define UPC_P__COUNTER_CFG__EVENT_GROUP_SEL_set(v)      _BGQ_SET(2,59,v)            //!< event source: 00:A2, 01:XU/AXU OpCodes, 10:L1P select 0-3, 11:L1P select 4-7)
#define UPC_P__COUNTER_CFG__EVENT_GROUP_SEL_insert(x,v) _UPC_INSERT(58,59,x,v)

#define UPC_P__COUNTER_CFG__EVENT_SUB_SEL_get(x)        _BGQ_GET(2,61,x)            //!<  See L1P_UPC_Events_??.xls spreadsheet.
#define UPC_P__COUNTER_CFG__EVENT_SUB_SEL_set(v)        _BGQ_SET(2,61,v)            //!< A2/L1P Sub group events: A2(0)=MMU, (1)=QFPU, (2)=IU, (3)=LSU, L1P selects from 4 subgroups of events.
#define UPC_P__COUNTER_CFG__EVENT_SUB_SEL_insert(x,v)   _UPC_INSERT(60,61,x,v)

#define UPC_P__COUNTER_CFG__INT_ENABLE_get(x)           _BGQ_GET(1,62,x)
#define UPC_P__COUNTER_CFG__INT_ENABLE_set(v)           _BGQ_SET(1,62,v)            //!< enable interrupt on normal overflow (14th msb set when armed).
#define UPC_P__COUNTER_CFG__INT_ENABLE_insert(x,v)      _UPC_INSERT(62,62,x,v)

#define UPC_P__COUNTER_CFG__RESET                       UPC_P__COUNTER_CFG__SYNC_OVERRIDE_set(1UL)  //!< Value to reset config to when counter is inactive


//! \brief: upc_p_config_invert
//! Invert A2 Event signal inversion configuration mask.
// vhdl:  mmio_read_a2_event_config_i_data <= a2_event_config_i_q(0 to 39) & X"000000";
//        ac_ta_event_bus_i <= ac_ta_event_bus xor invert(0 to 7); -- XU
//        ac_at_fu_bypass_events_i <= ac_at_fu_bypass_events xor invert(8 to 15);
//        ac_at_lsu_bypass_events_i <= ac_at_lsu_bypass_events xor invert(16 to 23);
//        ac_at_iu_bypass_events_i <= ac_at_iu_bypass_events xor invert(24 to 31);
//        ac_at_mm_bypass_events_i <= ac_at_mm_bypass_events xor invert(32 to 39);
#define UPC_P__CONFIG_INVERT_offset                    (0x640)      //!< A2 Event Signal Inversions
//! Bit Definitions:
#define UPC_P__CONFIG_INVERT__XU_INVERT_get(x)         _BGQ_GET(8,7,x)
#define UPC_P__CONFIG_INVERT__XU_INVERT_set(v)         _BGQ_SET(8,7,v)               //!< Invert the XU event inputs.
#define UPC_P__CONFIG_INVERT__XU_INVERT_insert(x,v)    _UPC_INSERT(0,7,x,v)
#define UPC_P__CONFIG_INVERT__XU_INVERT_bit(b)          (1UL<<(63-(0+(b))))

#define UPC_P__CONFIG_INVERT__AXU_INVERT_get(x)        _BGQ_GET(8,15,x)
#define UPC_P__CONFIG_INVERT__AXU_INVERT_set(v)        _BGQ_SET(8,15,v)              //!< Invert the AXU event inputs.
#define UPC_P__CONFIG_INVERT__AXU_INVERT_insert(x,v)   _UPC_INSERT(8,15,x,v)
#define UPC_P__CONFIG_INVERT__AXU_INVERT_bit(b)         (1UL<<(63-(8+(b))))

#define UPC_P__CONFIG_INVERT__LSU_INVERT_get(x)        _BGQ_GET(8,23,x)
#define UPC_P__CONFIG_INVERT__LSU_INVERT_set(v)        _BGQ_SET(8,23,v)              //!< Invert the LSU event inputs.
#define UPC_P__CONFIG_INVERT__LSU_INVERT_insert(x,v)   _UPC_INSERT(16,23,x,v)
#define UPC_P__CONFIG_INVERT__LSU_INVERT_bit(b)         (1UL<<(63-(16+(b))))

#define UPC_P__CONFIG_INVERT__IU_INVERT_get(x)         _BGQ_GET(8,31,x)
#define UPC_P__CONFIG_INVERT__IU_INVERT_set(v)         _BGQ_SET(8,31,v)              //!< Invert the IU event inputs.
#define UPC_P__CONFIG_INVERT__IU_INVERT_insert(x,v)    _UPC_INSERT(24,31,x,v)
#define UPC_P__CONFIG_INVERT__IU_INVERT_bit(b)          (1UL<<(63-(24+(b))))

#define UPC_P__CONFIG_INVERT__MMU_INVERT_get(x)        _BGQ_GET(8,39,x)
#define UPC_P__CONFIG_INVERT__MMU_INVERT_set(v)        _BGQ_SET(8,39,v)              //!< Invert the MMU event inputs.
#define UPC_P__CONFIG_INVERT__MMU_INVERT_insert(x,v)   _UPC_INSERT(32,39,x,v)
#define UPC_P__CONFIG_INVERT__MMU_INVERT_bit(b)         (1UL<<(63-(32+(b))))




//! \brief: upc_p_config_edge
//! Invert A2/L1P Level/Edge signal configuration mask.
//! All L1P / wakeup events not listed are always level detected.
// vhdl:  mmio_read_a2_event_config_e_data <= a2_event_config_e_q(0 to 48) & "000" & X"000";
//        ac_ta_event_bus_i <= ac_ta_event_bus xor invert(0 to 7); -- XU
//        ac_at_fu_bypass_events_i <= ac_at_fu_bypass_events xor invert(8 to 15);
//        ac_at_lsu_bypass_events_i <= ac_at_lsu_bypass_events xor invert(16 to 23);
//        ac_at_iu_bypass_events_i <= ac_at_iu_bypass_events xor invert(24 to 31);
//        ac_at_mm_bypass_events_i <= ac_at_mm_bypass_events xor invert(32 to 39);
#define UPC_P__CONFIG_EDGE_offset                      (0x641)          //!< Relative Offset for edge selection reg
//! Bit Definitions:
#define UPC_P__CONFIG_EDGE__XU_EDGE_get(x)             _BGQ_GET(8,7,x)
#define UPC_P__CONFIG_EDGE__XU_EDGE_set(v)             _BGQ_SET(8,7,v)               //!< XU signals 0=level; 1=edge
#define UPC_P__CONFIG_EDGE__XU_EDGE_insert(x,v)        _UPC_INSERT(0,7,x,v)
#define UPC_P__CONFIG_EDGE__XU_EDGE_bit(b)             (1UL<<(63-(0+(b))))

#define UPC_P__CONFIG_EDGE__AXU_EDGE_get(x)            _BGQ_GET(8,15,x)
#define UPC_P__CONFIG_EDGE__AXU_EDGE_set(v)            _BGQ_SET(8,15,v)              //!< AXU signals 0=level; 1=edge
#define UPC_P__CONFIG_EDGE__AXU_EDGE_insert(x,v)       _UPC_INSERT(8,15,x,v)
#define UPC_P__CONFIG_EDGE__AXU_EDGE_bit(b)             (1UL<<(63-(8+(b))))

#define UPC_P__CONFIG_EDGE__LSU_EDGE_get(x)            _BGQ_GET(8,23,x)
#define UPC_P__CONFIG_EDGE__LSU_EDGE_set(v)            _BGQ_SET(8,23,v)              //!< LSU signals 0=level; 1=edge
#define UPC_P__CONFIG_EDGE__LSU_EDGE_insert(x,v)       _UPC_INSERT(16,23,x,v)
#define UPC_P__CONFIG_EDGE__LSU_EDGE_bit(b)             (1UL<<(63-(16+(b))))

#define UPC_P__CONFIG_EDGE__IU_EDGE_get(x)             _BGQ_GET(8,31,x)
#define UPC_P__CONFIG_EDGE__IU_EDGE_set(v)             _BGQ_SET(8,31,v)              //!< IU signals 0=level; 1=edge
#define UPC_P__CONFIG_EDGE__IU_EDGE_insert(x,v)        _UPC_INSERT(24,31,x,v)
#define UPC_P__CONFIG_EDGE__IU_EDGE_bit(b)             (1UL<<(63-(24+(b))))

#define UPC_P__CONFIG_EDGE__MMU_EDGE_get(x)            _BGQ_GET(8,39,x)
#define UPC_P__CONFIG_EDGE__MMU_EDGE_set(v)            _BGQ_SET(8,39,v)              //!< MMU signals 0=level; 1=edge
#define UPC_P__CONFIG_EDGE__MMU_EDGE_insert(x,v)       _UPC_INSERT(32,39,x,v)
#define UPC_P__CONFIG_EDGE__MMU_EDGE_bit(b)            (1UL<<(63-(32+(b))))

#define UPC_P__CONFIG_EDGE__L1P_DRAIN_get(x)           _BGQ_GET(4,43,x)
#define UPC_P__CONFIG_EDGE__L1P_DRAIN_set(v)           _BGQ_SET(4,43,v)              //!< L1P LU Drain event for each thread 0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_DRAIN_insert(x,v)      _UPC_INSERT(40,43,x,v)
#define UPC_P__CONFIG_EDGE__L1P_DRAIN_bit(b)           (1UL<<(63-(40+(b))))

#define UPC_P__CONFIG_EDGE__L1P_STALL_SRT_get(x)       _BGQ_GET(1,44,x)
#define UPC_P__CONFIG_EDGE__L1P_STALL_SRT_set(v)       _BGQ_SET(1,44,v)              //!< L1P LU Stall-SRT core wide event   0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_STALL_SRT_insert(x,v)  _UPC_INSERT(44,44,x,v)

#define UPC_P__CONFIG_EDGE__L1P_MMIO_DCR_get(x)        _BGQ_GET(1,45,x)
#define UPC_P__CONFIG_EDGE__L1P_MMIO_DCR_set(v)        _BGQ_SET(1,45,v)              //!< L1P LU Stall-MMIO/DCR core wide event        0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_MMIO_DCR_insert(x,v)   _UPC_INSERT(45,45,x,v)

#define UPC_P__CONFIG_EDGE__L1P_STREAM_DET_get(x)      _BGQ_GET(1,46,x)
#define UPC_P__CONFIG_EDGE__L1P_STREAM_DET_set(v)      _BGQ_SET(1,46,v)              //!< L1P LU Stall-stream detect core wide event   0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_STREAM_DET_insert(x,v) _UPC_INSERT(46,46,x,v)

#define UPC_P__CONFIG_EDGE__L1P_LIST_READ_get(x)       _BGQ_GET(1,47,x)
#define UPC_P__CONFIG_EDGE__L1P_LIST_READ_set(v)       _BGQ_SET(1,47,v)              //!< L1P LU Stall-list read core wide event       0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_LIST_READ_insert(x,v)  _UPC_INSERT(47,47,x,v)

#define UPC_P__CONFIG_EDGE__L1P_LIST_WRITE_get(x)      _BGQ_GET(1,48,x)
#define UPC_P__CONFIG_EDGE__L1P_LIST_WRITE_set(v)      _BGQ_SET(1,48,v)              //!< L1P LU Stall-list write core wide event      0=level; 1=edge.
#define UPC_P__CONFIG_EDGE__L1P_LIST_WRITE_insert(x,v) _UPC_INSERT(48,48,x,v)




//! \brief: upc_p_int_status
//! interrupt status registers indicate which counter has raised an interrupt on overflow.
//! The W1S W1C addresses when read give which of all counters have raised the interrupt.
//! The T0, T1, T2 and T2 addresses give only the associated counters with that thread affinity.
//! Big interrupts (status bits 0 thru 23) can't be set via W1S - must be a "real" big overflow.
//! The "small" interrupt status bits are only set when the counter_cfg.int_enable is also set.
#define UPC_P__INT_STATUS_offset                           (0x642)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_W1S_offset                       (0x642)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_W1C_offset                       (0x643)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_T0_offset                        (0x644)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_T1_offset                        (0x645)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_T2_offset                        (0x646)         //!< relative offset to int status reg
#define UPC_P__INT_STATUS_T3_offset                        (0x647)         //!< relative offset to int status reg
//! Bit Definitions:
#define UPC_P__INT_STATUS__COUNTER_BIGINT_get(x)       _BGQ_GET(24,23,x)
#define UPC_P__INT_STATUS__COUNTER_BIGINT_set(v)       _BGQ_SET(24,23,v)          //!< bit mask of 24 counters having with active interrupts
#define UPC_P__INT_STATUS__COUNTER_BIGINT_insert(x,v)  _UPC_INSERT(0,23,x,v)
#define UPC_P__INT_STATUS__COUNTER_BIGINT_bit(b)       (1UL<<(63-(0+(b))))

#define UPC_P__INT_STATUS__COUNTER_INT_get(x)          _BGQ_GET(24,55,x)
#define UPC_P__INT_STATUS__COUNTER_INT_set(v)          _BGQ_SET(24,55,v)          //!< bit mask of 24 counters having with active interrupts
#define UPC_P__INT_STATUS__COUNTER_INT_insert(x,v)     _UPC_INSERT(32,55,x,v)
#define UPC_P__INT_STATUS__COUNTER_INT_bit(b)          (1UL<<(63-(32+(b))))

#define UPC_P__INT_STATUS__UPC_C_THREAD_INT_get(x)      _BGQ_GET(4,59,x)
#define UPC_P__INT_STATUS__UPC_C_THREAD_INT_set(v)      _BGQ_SET(4,59,v)          //!< bit mask for 4 threads - sticky bits to indicate broadcasted interrupt delivered.
#define UPC_P__INT_STATUS__UPC_C_THREAD_INT_insert(x,v) _UPC_INSERT(56,59,x,v)
#define UPC_P__INT_STATUS__UPC_C_THREAD_INT_bit(b)      (1UL<<(63-(56+(b))))

#define UPC_P__INT_STATUS__ANY_OVF_THREAD_INT_get(x)      _BGQ_GET(4,63,x)
#define UPC_P__INT_STATUS__ANY_OVF_THREAD_INT_set(v)      _BGQ_SET(4,63,v)        //!< bit mask for 4 threads - sticky bits to indicate broadcasted interrupt delivered.
#define UPC_P__INT_STATUS__ANY_OVF_THREAD_INT_insert(x,v) _UPC_INSERT(60,63,x,v)
#define UPC_P__INT_STATUS__ANY_OVF_THREAD_INT_bit(b)      (1UL<<(63-(60+(b))))


//! Masks:
#define UPC_P__INT_STATUS__ALL_STATUS                      ( 0xFFFFFF00FFFFFFFFULL )              //!< Mask of all interrupt bits
#define UPC_P__INT_STATUS__COUNTER_STATUS                  ( 0xFFFFFF00FFFFFF00ULL )              //!< Mask of only counter interrupt bits
#define UPC_P__INT_STATUS__BIGINT_STATUS                   ( 0xFFFFFF0000000000ULL )              //!< Mask of only Big Interrupt bits
#define UPC_P__INT_STATUS__LTLINT_STATUS                   ( 0x00000000FFFFFF00ULL )              //!< Mask of only Ltl Interrupt bits
#define UPC_P__INT_STATUS__THREAD_STATUS(threadid)         ( 0x0000000000000088ULL >> threadid )  //!< Mask of thread bits
#define UPC_P__INT_STATUS__COUNTER_THREAD_STATUS(threadid) ( 0x8888880088888800ULL >> threadid )  //!< Mask of bits with thread affinity




//! \brief: upc_p_opcode_cfg
#define UPC_P__OPCODE_CFG_offset                              (0x648)         //!< relative offset
//! Bit Definitions:
#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MATCH_get(x)       _BGQ_GET(6,5,x)
#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MATCH_set(v)       _BGQ_SET(6,5,v)               //!< compare with instruction bits 0:5
#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MATCH_insert(x,v)  _UPC_INSERT(0,5,x,v)

#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MATCH_get(x)       _BGQ_GET(10,15,x)
#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MATCH_set(v)       _BGQ_SET(10,15,v)             //!< compare with instruction bits 21:30
#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MATCH_insert(x,v)  _UPC_INSERT(6,15,x,v)

#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MASK_get(x)        _BGQ_GET(6,21,x)
#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MASK_set(v)        _BGQ_SET(6,21,v)              //!< which bits in opcode to compare
#define UPC_P__OPCODE_CFG__XU_OPCODE_MAJOR_MASK_insert(x,v)   _UPC_INSERT(16,21,x,v)

#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MASK_get(x)        _BGQ_GET(10,31,x)
#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MASK_set(v)        _BGQ_SET(10,31,v)             //!< which bits in opcode to compare
#define UPC_P__OPCODE_CFG__XU_OPCODE_MINOR_MASK_insert(x,v)   _UPC_INSERT(22,31,x,v)

#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MATCH_get(x)      _BGQ_GET(6,37,x)
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MATCH_set(v)      _BGQ_SET(6,37,v)              //!< compare with instruction bits 0:5
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MATCH_insert(x,v) _UPC_INSERT(32,37,x,v)

#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MATCH_get(x)      _BGQ_GET(10,47,x)
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MATCH_set(v)      _BGQ_SET(10,47,v)             //!< compare with instruction bits 21:30
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MATCH_insert(x,v) _UPC_INSERT(38,47,x,v)

#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MASK_get(x)       _BGQ_GET(6,53,x)
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MASK_set(v)       _BGQ_SET(6,53,v)              //!< which bits in opcode to compare
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MAJOR_MASK_insert(x,v)  _UPC_INSERT(48,53,x,v)

#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MASK_get(x)       _BGQ_GET(10,63,x)
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MASK_set(v)       _BGQ_SET(10,63,v)             //!< which bits in opcode to compare
#define UPC_P__OPCODE_CFG__AXU_OPCODE_MINOR_MASK_insert(x,v)  _UPC_INSERT(54,63,x,v)




//! \brief: upc_p_m2_trigger
#define UPC_P__M2_TRIGGER_offset                              (0x649)                       //!< relative offset
#define UPC_P__M2_TRIGGER_set(v)                              _BGQ_SET(1,63,v)              //!< Software trigger trace (write only)
#define UPC_P__M2_TRIGGER                                     UPC_P__M2_TRIGGER_set(1UL)    //!< trigger value to store.



//! upc_p_config
#define UPC_P__CONFIG_offset                       (0x660)      //!< UPC_P Config Register Offset
//! Bit Definitions:
//-------------------------------------------------------------------------------
//-- Config Register Bit Mappings    see ddx/upc/vhdl/upc_p.vhdl
//-------------------------------------------------------------------------------
//  enable                  <= upc_p_config(0);  	-- clock gate for all upc_p
//                                              	-- common function
//  trace_select_upper      <= upc_p_config(1);  	-- picks upper/lower 44 bits from
//                                              	-- trace bus
//  mode                    <= upc_p_config(2 to 3);  --
//  div_weight              <= upc_p_config(4 to 5);  -- weight for divide
//  sqrt_weight             <= upc_p_config(6 to 7);  -- weight for sqrt
//  op0_weight              <= upc_p_config(8 to 9);  -- weight for op0
//  mode2_trigger_sel       <= upc_p_config(10 to 11);
//
//  mode1_event_group0_sel  <= upc_p_config(12);
//  mode1_event_group1_sel  <= upc_p_config(13);
//  mode1_event_group2_sel  <= upc_p_config(14);
//  mode1_event_group3_sel  <= upc_p_config(15);
//  mode1_event_group4_sel  <= upc_p_config(16);
//  mode1_event_group5_sel  <= upc_p_config(17);
//  mode1_event_group6_sel  <= upc_p_config(18);
//  mode1_event_group7_sel  <= upc_p_config(19);
//  mode1_event_group8_sel  <= upc_p_config(20);
//  mode1_event_group9_sel  <= upc_p_config(21);
//  mode1_event_group10_sel <= upc_p_config(22);
//  mode1_event_group11_sel <= upc_p_config(23);
//  trace_select_opcode     <= upc_p_config(28);  	-- brings opcodes into widener
//  join_ring               <= upc_p_config(29);  	-- enables this upc to send/rcv
//                                               	-- ring transactions
//  upcc_int_en             <= upc_p_config(30 to 33);  -- allows upc_c interrupt
//                                                     	-- to interrupt each thread
//  interrupt_on_any        <= upc_p_config(34 to 37);  --
//
//  ovf_int_enable          <= upc_p_config(38 to 41);  --
//!
#define UPC_P__CONFIG__ENABLE_get(x)                   _BGQ_GET(1,0,x)
#define UPC_P__CONFIG__ENABLE_set(v)                   _BGQ_SET(1,0,v)
#define UPC_P__CONFIG__ENABLE_insert(x,v)              _UPC_INSERT(0,0,x,v)
//!< clock gate for all upc_p functions
//!<   No counting when 0. Starts/stops counting simultaneously.
//!<   Disable this bit along with interrupt and "join_ring" when
//!<   UPC_P is inactive to conserve power.
#define UPC_P__CONFIG__TRACE_SELECT_UPPER_get(x)       _BGQ_GET(1,1,x)
#define UPC_P__CONFIG__TRACE_SELECT_UPPER_set(v)       _BGQ_SET(1,1,v)
#define UPC_P__CONFIG__TRACE_SELECT_UPPER_insert(x,v)  _UPC_INSERT(1,1,x,v)
//!< pick upper or lower 44 bits from trace bus, 0=bits(0:43), 1=bits(44:87)
#define UPC_P__CONFIG__COUNTING_MODE_get(x)            _BGQ_GET(2,3,x)
#define UPC_P__CONFIG__COUNTING_MODE_set(v)            _BGQ_SET(2,3,v)
#define UPC_P__CONFIG__COUNTING_MODE_insert(x,v)       _UPC_INSERT(2,3,x,v)
//!< Counting Mode: 0=distributed; 1=detailed; 2=trace
#define UPC_P__CONFIG__DIV_WEIGHT_get(x)               _BGQ_GET(2,5,x)
#define UPC_P__CONFIG__DIV_WEIGHT_set(v)               _BGQ_SET(2,5,v)
#define UPC_P__CONFIG__DIV_WEIGHT_insert(x,v)          _UPC_INSERT(4,5,x,v)
//!< Op counting Flops per divide
//!<   0 = 1 flop  per scalar divide;  4 flops per vector divide
//!<   1 = 2 flops per scalar divide;  8 flops per vector divide
//!<   2 = 4 flops per scalar divide; 16 flops per vector divide
//!<   3 = 8 flops per scalar divide; 32 flops per vector divide
#define UPC_P__CONFIG__SQRT_WEIGHT_get(x)              _BGQ_GET(2,7,x)
#define UPC_P__CONFIG__SQRT_WEIGHT_set(v)              _BGQ_SET(2,7,v)
#define UPC_P__CONFIG__SQRT_WEIGHT_insert(x,v)         _UPC_INSERT(6,7,x,v)
//!< Op counting Flops per square root
//!<   0 = 1 flop  per square root;  4 flops per vector square root
//!<   1 = 2 flops per square root;  8 flops per vector square root
//!<   2 = 4 flops per square root; 16 flops per vector square root
//!<   3 = 8 flops per square root; 32 flops per vector square root
#define UPC_P__CONFIG__AXU_MATCH_WEIGHT_get(x)         _BGQ_GET(2,9,x)
#define UPC_P__CONFIG__AXU_MATCH_WEIGHT_set(v)         _BGQ_SET(2,9,v)
#define UPC_P__CONFIG__AXU_MATCH_WEIGHT_insert(x,v)    _UPC_INSERT(8,9,x,v)
//!< Op counting AXU opcode match Flops per match
//!<   0 = 1 flop  per match
//!<   1 = 2 flops per match
//!<   2 = 4 flops per match
//!<   3 = 8 flops per match
#define UPC_P__CONFIG__MODE2_TRIGGER_SEL_get(x)        _BGQ_GET(2,11,x)
#define UPC_P__CONFIG__MODE2_TRIGGER_SEL_set(v)        _BGQ_SET(2,11,v)
#define UPC_P__CONFIG__MODE2_TRIGGER_SEL_insert(x,v)   _UPC_INSERT(10,11,x,v)
//!< Which trigger bits events to send to UPC_C in Mode 2 (trace mode)
//!<   0 = bits 0:3
//!<   1 = bits 4:7
//!<   2 = bits 8:11
//!<   3 = bits 12:15
//!
//! Mode 1 Event Groups (See L1P_UPC_Events_v??.xls)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP0_SEL_get(x)      _BGQ_GET(1,12,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP0_SEL_set(v)      _BGQ_SET(1,12,v)               //!< 0=A2 bit 4 T2/3;  1=L1P T0
#define UPC_P__CONFIG__MODE1_EVENT_GROUP0_SEL_insert(x,v) _UPC_INSERT(12,12,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP1_SEL_get(x)      _BGQ_GET(1,13,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP1_SEL_set(v)      _BGQ_SET(1,13,v)               //!< 0=A2 bit 5 T2/3;  1=L1P T0
#define UPC_P__CONFIG__MODE1_EVENT_GROUP1_SEL_insert(x,v) _UPC_INSERT(13,13,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP2_SEL_get(x)      _BGQ_GET(1,14,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP2_SEL_set(v)      _BGQ_SET(1,14,v)            //!< 0=A2 bit 6 T2/3;  1=L1P T1
#define UPC_P__CONFIG__MODE1_EVENT_GROUP2_SEL_insert(x,v) _UPC_INSERT(14,14,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP3_SEL_get(x)      _BGQ_GET(1,15,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP3_SEL_set(v)      _BGQ_SET(1,15,v)            //!< 0=A2 bit 7 T2/3;  1=L1P T1
#define UPC_P__CONFIG__MODE1_EVENT_GROUP3_SEL_insert(x,v) _UPC_INSERT(15,15,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP4_SEL_get(x)      _BGQ_GET(1,16,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP4_SEL_set(v)      _BGQ_SET(1,16,v)            //!< 0=A2 bit 0 T0/1;  1=L1P T2
#define UPC_P__CONFIG__MODE1_EVENT_GROUP4_SEL_insert(x,v) _UPC_INSERT(16,16,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP5_SEL_get(x)      _BGQ_GET(1,17,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP5_SEL_set(v)      _BGQ_SET(1,17,v)            //!< 0=A2 bit 1 T0/1;  1=L1P T2
#define UPC_P__CONFIG__MODE1_EVENT_GROUP5_SEL_insert(x,v) _UPC_INSERT(17,17,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP6_SEL_get(x)      _BGQ_GET(1,18,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP6_SEL_set(v)      _BGQ_SET(1,18,v)            //!< 0=A2 bit 2 T0/1;  1=L1P T3
#define UPC_P__CONFIG__MODE1_EVENT_GROUP6_SEL_insert(x,v) _UPC_INSERT(18,18,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP7_SEL_get(x)      _BGQ_GET(1,19,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP7_SEL_set(v)      _BGQ_SET(1,19,v)            //!< 0=A2 bit 3 T0/1;  1=L1P T3
#define UPC_P__CONFIG__MODE1_EVENT_GROUP7_SEL_insert(x,v) _UPC_INSERT(19,19,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP8_SEL_get(x)      _BGQ_GET(1,20,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP8_SEL_set(v)      _BGQ_SET(1,20,v)            //!< 0=XU bit 45 T2/3;  1=L1P/wake T0
#define UPC_P__CONFIG__MODE1_EVENT_GROUP8_SEL_insert(x,v) _UPC_INSERT(20,20,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP9_SEL_get(x)      _BGQ_GET(1,21,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP9_SEL_set(v)      _BGQ_SET(1,21,v)            //!< 0=XU bit 45 T2/3;  1=L1P/wake T1
#define UPC_P__CONFIG__MODE1_EVENT_GROUP9_SEL_insert(x,v) _UPC_INSERT(21,21,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP10_SEL_get(x)     _BGQ_GET(1,22,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP10_SEL_set(v)     _BGQ_SET(1,22,v)               //!< 0=XU bit 01 T0/1;  1=L1P/wake T2
#define UPC_P__CONFIG__MODE1_EVENT_GROUP10_SEL_insert(x,v) _UPC_INSERT(22,22,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP11_SEL_get(x)      _BGQ_GET(1,23,x)
#define UPC_P__CONFIG__MODE1_EVENT_GROUP11_SEL_set(v)      _BGQ_SET(1,23,v)               //!< 0=XU bit 01 T0/1;  1=L1P/wake T3
#define UPC_P__CONFIG__MODE1_EVENT_GROUP11_SEL_insert(x,v) _UPC_INSERT(23,23,x,v)

#define UPC_P__CONFIG__MODE1_EVENT_GROUP_SEL_bit(b)        (1UL<<(63-(12+(b))))
#define UPC_P__CONFIG__MODE1_EVENT_GROUP_SEL_insert(x,grp,v) (v ? (x |= UPC_P__CONFIG__MODE1_EVENT_GROUP_SEL_bit(grp)) : (x &= ~(UPC_P__CONFIG__MODE1_EVENT_GROUP_SEL_bit(grp))))

//!
#define UPC_P__CONFIG__TRACE_SELECT_OPCODE_get(x)          _BGQ_GET(1,28,x)
#define UPC_P__CONFIG__TRACE_SELECT_OPCODE_set(v)          _BGQ_SET(1,28,v)
#define UPC_P__CONFIG__TRACE_SELECT_OPCODE_insert(x,v)     _UPC_INSERT(28,28,x,v)
//!< Bring opcodes into widener - enable in Mode 0/1, disable in Mode 2.

#define UPC_P__CONFIG__JOIN_RING_get(x)                    _BGQ_GET(1,29,x)
#define UPC_P__CONFIG__JOIN_RING_set(v)                    _BGQ_SET(1,29,v)
#define UPC_P__CONFIG__JOIN_RING_insert(x,v)               _UPC_INSERT(29,29,x,v)
//!< enable this upc_p into scan ring.  Disable this along with
//!<   interupts and "enable" bit when UPC_P should be inactive.

#define UPC_P__CONFIG__UPC_C_INT_ENABLE_get(x)             _BGQ_GET(4,33,x)
#define UPC_P__CONFIG__UPC_C_INT_ENABLE_set(v)             _BGQ_SET(4,33,v)
#define UPC_P__CONFIG__UPC_C_INT_ENABLE_insert(x,v)        _UPC_INSERT(30,33,x,v)
#define UPC_P__CONFIG__UPC_C_INT_ENABLE_bit(b)             (1UL<<(63-(30+(b))))
 //!< UPC_C Interrupt to A2 enable mask - which threads a UPC_C PM int will interrupt.

#define UPC_P__CONFIG__INTERRUPT_ON_ANY_get(x)             _BGQ_GET(4,37,x)
#define UPC_P__CONFIG__INTERRUPT_ON_ANY_set(v)             _BGQ_SET(4,37,v)
#define UPC_P__CONFIG__INTERRUPT_ON_ANY_insert(x,v)        _UPC_INSERT(34,37,x,v)
#define UPC_P__CONFIG__INTERRUPT_ON_ANY_bit(b)             (1UL<<(63-(34+(b))))
//!< Any Overflow enable mask to A2 - interrupt threads when any
//!<   "int on overflow enabled" counter overflows -
//!<   all counters freeze together.

#define UPC_P__CONFIG__BIG_OVF_INT_ENABLE_get(x)           _BGQ_GET(4,41,x)
#define UPC_P__CONFIG__BIG_OVF_INT_ENABLE_set(v)           _BGQ_SET(4,41,v)
#define UPC_P__CONFIG__BIG_OVF_INT_ENABLE_insert(x,v)      _UPC_INSERT(38,41,x,v)
#define UPC_P__CONFIG__BIG_OVF_INT_ENABLE_bit(b)           (1UL<<(63-(38+(b))))
//!< Extra (big) overflow to A2 interrupt enable mask.
//!<   big overflow occurs when a 14 bit counter wraps in a scan cycle
//!<   (big overflow bit set)

#define UPC_P__CONFIG__UPC_P_INT_ENABLE_get(x)             _BGQ_GET(4,45,x)
#define UPC_P__CONFIG__UPC_P_INT_ENABLE_set(v)             _BGQ_SET(4,45,v)
#define UPC_P__CONFIG__UPC_P_INT_ENABLE_insert(x,v)        _UPC_INSERT(42,45,x,v)
#define UPC_P__CONFIG__UPC_P_INT_ENABLE_bit(b)             (1UL<<(63-(42+(b))))
//!< little overflow interrupt enable mask.
//!<   counter interrupts enabled, counter armed, and 14th bit set (little overflow).
//!<   It takes one extra x2 cycle after the interrupt is latched for the counters to freeze (counting cycles will have residual value).

#define UPC_P__CONFIG__M2_TRIGGER_SEL_get(x)              _BGQ_GET(4,49,x)
#define UPC_P__CONFIG__M2_TRIGGER_SEL_set(v)              _BGQ_SET(4,49,v)
#define UPC_P__CONFIG__M2_TRIGGER_SEL_insert(x,v)         _UPC_INSERT(46,49,x,v)
//!< Select which of 16 A2 Trigger signals should trigger the trace.

#define UPC_P__CONFIG__M2_INTERRUPT_TRIGGER_get(x)         _BGQ_GET(4,53,x)
#define UPC_P__CONFIG__M2_INTERRUPT_TRIGGER_set(v)         _BGQ_SET(4,53,v)
#define UPC_P__CONFIG__M2_INTERRUPT_TRIGGER_insert(x,v)    _UPC_INSERT(50,53,x,v)
#define UPC_P__CONFIG__M2_INTERRUPT_TRIGGER_bit(b)         (1UL<<(63-(50+(b))))
//!< Mode 2 trigger interrupt enable (i.e - trigger m2 trace on overflow_

#define UPC_P__CONFIG__M2_TRIGGER_ENABLE_get(x)            _BGQ_GET(1,54,x)
#define UPC_P__CONFIG__M2_TRIGGER_ENABLE_set(v)            _BGQ_SET(1,54,v)
#define UPC_P__CONFIG__M2_TRIGGER_ENABLE_insert(x,v)       _UPC_INSERT(54,54,x,v)
//!< Mode 2 trigger enable from A2 signal

#define UPC_P__CONFIG__SHARED_MODE_get(x)                  _BGQ_GET(1,57,x)
#define UPC_P__CONFIG__SHARED_MODE_set(v)                  _BGQ_SET(1,57,v)
#define UPC_P__CONFIG__SHARED_MODE_insert(x,v)             _UPC_INSERT(57,57,x,v)
//!< Set counter overflow shared mode - i.e. any overflow triggers int for all threads.

#define UPC_P__CONFIG__DD1MODE_get(x)                      _BGQ_GET(1,63,x)
#define UPC_P__CONFIG__DD1MODE_set(v)                      _BGQ_SET(1,63,v)
#define UPC_P__CONFIG__DD1MODE_insert(x,v)                 _UPC_INSERT(63,63,x,v)
//!< DD1/DD2 Chicken switch to return to DD1 behaivor  0=DD2, 1=DD1
//!<   In DD1, the physical to logical core mapping in UPC Ring was broken - only physical core mapping was implemented.
//!<      The effect was that:
//!<          UPC_P Counters would send counter values to UPC_C a shifted UPC_C group after the spare core.
//!<          that is, if physical spare core is 2, than UPC_P 2 would send counters to UPC_C group 3, rather than 2 as usual.
//!<          The last UPC_P Counters for core 16 would be sent into the bit bucket.
//!<      Also, in DD1, there is a Mode 1 sync problem, where events counted for the 24 upc_p counters in UPC_C group 0, are
//!<          slightly out of sync with the other mode 1 events spread in multiple upc_c groups.
//!<
//!<   In DD2, we've found that only the UPC_P counter store (logial mapping) was correctly fixed.  The "arm" bit used
//!<      used for fast overflow is still broken (using the physical mapping). With the result that the arm bit
//!<      may not work correctly with non-standard spare core.
//!<      Thus, the software should probably use the DD1 operation and do the logical to physical mapping itself.

#define UPC_P__CONFIG__UPC_P_INT_ENABLE_allbits(b)         (0x0000000222200000ULL >> (b))




//! \brief: upc_p_tc_cfg0
//! L1P thread combining masks (by event groups of 4).
#define UPC_P__TC_CFG0_offset                      (0x661)     //!< Thread combining reg offset
//! Bit Definitions:
#define UPC_P__TC_CFG0__L1P_E0_TCMT_get(x)         _BGQ_GET(4,3,x)
#define UPC_P__TC_CFG0__L1P_E0_TCMT_set(v)         _BGQ_SET(4,3,v)               //!< L1P Event signals (0 to 3);
#define UPC_P__TC_CFG0__L1P_E0_TCMT_insert(x,v)    _UPC_INSERT(0,3,x,v)

#define UPC_P__TC_CFG0__L1P_E1_TCMT_get(x)         _BGQ_GET(4,7,x)
#define UPC_P__TC_CFG0__L1P_E1_TCMT_set(v)         _BGQ_SET(4,7,v)               //!< L1P Event signals (4 to 7);
#define UPC_P__TC_CFG0__L1P_E1_TCMT_insert(x,v)    _UPC_INSERT(4,7,x,v)

#define UPC_P__TC_CFG0__L1P_E2_TCMT_get(x)         _BGQ_GET(4,11,x)
#define UPC_P__TC_CFG0__L1P_E2_TCMT_set(v)         _BGQ_SET(4,11,v)              //!< L1P Event signals (8 to 11);
#define UPC_P__TC_CFG0__L1P_E2_TCMT_insert(x,v)    _UPC_INSERT(8,11,x,v)

#define UPC_P__TC_CFG0__L1P_E3_TCMT_get(x)         _BGQ_GET(4,15,x)
#define UPC_P__TC_CFG0__L1P_E3_TCMT_set(v)         _BGQ_SET(4,15,v)              //!< L1P Event signals (12 to 15);
#define UPC_P__TC_CFG0__L1P_E3_TCMT_insert(x,v)    _UPC_INSERT(12,15,x,v)

#define UPC_P__TC_CFG0__L1P_E4_TCMT_get(x)         _BGQ_GET(4,19,x)
#define UPC_P__TC_CFG0__L1P_E4_TCMT_set(v)         _BGQ_SET(4,19,v)              //!< L1P Event signals (16 to 19);
#define UPC_P__TC_CFG0__L1P_E4_TCMT_insert(x,v)    _UPC_INSERT(16,19,x,v)

#define UPC_P__TC_CFG0__L1P_E5_TCMT_get(x)         _BGQ_GET(4,23,x)
#define UPC_P__TC_CFG0__L1P_E5_TCMT_set(v)         _BGQ_SET(4,23,v)              //!< L1P Event signals (20 to 23);
#define UPC_P__TC_CFG0__L1P_E5_TCMT_insert(x,v)    _UPC_INSERT(20,23,x,v)

#define UPC_P__TC_CFG0__L1P_E6_TCMT_get(x)         _BGQ_GET(4,27,x)
#define UPC_P__TC_CFG0__L1P_E6_TCMT_set(v)         _BGQ_SET(4,27,v)              //!< L1P Event signals (24 to 27);
#define UPC_P__TC_CFG0__L1P_E6_TCMT_insert(x,v)    _UPC_INSERT(24,27,x,v)

#define UPC_P__TC_CFG0__L1P_E7_TCMT_get(x)         _BGQ_GET(4,31,x)
#define UPC_P__TC_CFG0__L1P_E7_TCMT_set(v)         _BGQ_SET(4,31,v)              //!< L1P Event signals (28 to 31);
#define UPC_P__TC_CFG0__L1P_E7_TCMT_insert(x,v)    _UPC_INSERT(28,31,x,v)

#define UPC_P__TC_CFG0__L1P_E8_TCMT_get(x)         _BGQ_GET(4,35,x)
#define UPC_P__TC_CFG0__L1P_E8_TCMT_set(v)         _BGQ_SET(4,35,v)              //!< L1P Event signals (32 to 35);
#define UPC_P__TC_CFG0__L1P_E8_TCMT_insert(x,v)    _UPC_INSERT(32,35,x,v)

#define UPC_P__TC_CFG0__L1P_E9_TCMT_get(x)         _BGQ_GET(4,39,x)
#define UPC_P__TC_CFG0__L1P_E9_TCMT_set(v)         _BGQ_SET(4,39,v)              //!< L1P Event signals (36 to 39);
#define UPC_P__TC_CFG0__L1P_E9_TCMT_insert(x,v)    _UPC_INSERT(36,39,x,v)

#define UPC_P__TC_CFG0__L1P_E10_TCMT_get(x)        _BGQ_GET(4,43,x)
#define UPC_P__TC_CFG0__L1P_E10_TCMT_set(v)        _BGQ_SET(4,43,v)             //!< L1P Event signals (40 to 43);
#define UPC_P__TC_CFG0__L1P_E10_TCMT_insert(x,v)   _UPC_INSERT(40,43,x,v)

#define UPC_P__TC_CFG0__L1P_E11_TCMT_get(x)        _BGQ_GET(4,47,x)
#define UPC_P__TC_CFG0__L1P_E11_TCMT_set(v)        _BGQ_SET(4,47,v)             //!< L1P Event signals (44 to 47);
#define UPC_P__TC_CFG0__L1P_E11_TCMT_insert(x,v)   _UPC_INSERT(44,47,x,v)

#define UPC_P__TC_CFG0__L1P_E12_TCMT_get(x)        _BGQ_GET(4,51,x)
#define UPC_P__TC_CFG0__L1P_E12_TCMT_set(v)        _BGQ_SET(4,51,v)             //!< L1P Event signals (48 to 51);
#define UPC_P__TC_CFG0__L1P_E12_TCMT_insert(x,v)   _UPC_INSERT(48,51,x,v)

#define UPC_P__TC_CFG0__L1P_E13_TCMT_get(x)        _BGQ_GET(4,55,x)
#define UPC_P__TC_CFG0__L1P_E13_TCMT_set(v)        _BGQ_SET(4,55,v)             //!< L1P Event signals (52 to 55);
#define UPC_P__TC_CFG0__L1P_E13_TCMT_insert(x,v)   _UPC_INSERT(52,55,x,v)

#define UPC_P__TC_CFG0__L1P_E14_TCMT_get(x)        _BGQ_GET(4,59,x)
#define UPC_P__TC_CFG0__L1P_E14_TCMT_set(v)        _BGQ_SET(4,59,v)             //!< L1P Event signals (56 to 59);
#define UPC_P__TC_CFG0__L1P_E14_TCMT_insert(x,v)   _UPC_INSERT(56,59,x,v)

#define UPC_P__TC_CFG0__L1P_E15_TCMT_get(x)        _BGQ_GET(4,63,x)
#define UPC_P__TC_CFG0__L1P_E15_TCMT_set(v)        _BGQ_SET(4,63,v)             //!< L1P Event signals (60 to 63);
#define UPC_P__TC_CFG0__L1P_E15_TCMT_insert(x,v)   _UPC_INSERT(60,63,x,v)




//! \brief: upc_p_tc_cfg1
//! L1P thread combining masks for each counter.
#define UPC_P__TC_CFG1_offset                      (0x662)     //!< Thread Combining Offset
//! Bit Definitions:
#define UPC_P__TC_CFG1__L1P_E16_TCMT_get(x)        _BGQ_GET(4,35,x)
#define UPC_P__TC_CFG1__L1P_E16_TCMT_set(v)        _BGQ_SET(4,35,v)             //!< L1P Event signals (64 to 67);
#define UPC_P__TC_CFG1__L1P_E16_TCMT_insert(x,v)   _UPC_INSERT(32,35,x,v)

#define UPC_P__TC_CFG1__L1P_E17_TCMT_get(x)        _BGQ_GET(4,39,x)
#define UPC_P__TC_CFG1__L1P_E17_TCMT_set(v)        _BGQ_SET(4,39,v)             //!< L1P Event signals (68 to 71);
#define UPC_P__TC_CFG1__L1P_E17_TCMT_insert(x,v)   _UPC_INSERT(36,39,x,v)

#define UPC_P__TC_CFG1__L1P_E18_TCMT_get(x)        _BGQ_GET(4,43,x)
#define UPC_P__TC_CFG1__L1P_E18_TCMT_set(v)        _BGQ_SET(4,43,v)             //!< L1P Event signals (72 to 75);
#define UPC_P__TC_CFG1__L1P_E18_TCMT_insert(x,v)   _UPC_INSERT(40,43,x,v)

#define UPC_P__TC_CFG1__L1P_E19_TCMT_get(x)        _BGQ_GET(4,47,x)
#define UPC_P__TC_CFG1__L1P_E19_TCMT_set(v)        _BGQ_SET(4,47,v)             //!< L1P Event signals (76 to 79);
#define UPC_P__TC_CFG1__L1P_E19_TCMT_insert(x,v)   _UPC_INSERT(44,47,x,v)

#define UPC_P__TC_CFG1__L1P_E20_TCMT_get(x)        _BGQ_GET(4,51,x)
#define UPC_P__TC_CFG1__L1P_E20_TCMT_set(v)        _BGQ_SET(4,51,v)             //!< L1P Event signals (80 to 83);
#define UPC_P__TC_CFG1__L1P_E20_TCMT_insert(x,v)   _UPC_INSERT(48,51,x,v)

#define UPC_P__TC_CFG1__L1P_E21_TCMT_get(x)        _BGQ_GET(4,55,x)
#define UPC_P__TC_CFG1__L1P_E21_TCMT_set(v)        _BGQ_SET(4,55,v)             //!< L1P Event signals (84 to 87);
#define UPC_P__TC_CFG1__L1P_E21_TCMT_insert(x,v)   _UPC_INSERT(52,55,x,v)




//! \brief: upc_p_tc_cfg2
//! A2 Unit thread combining masks (by each unit).
#define UPC_P__TC_CFG2_offset                      (0x663)    //!< A2 Thread Combining Offset
//! Bit Definitions:
#define UPC_P__TC_CFG2__A2_FU0_TCMT_get(x)         _BGQ_GET(4,27,x)
#define UPC_P__TC_CFG2__A2_FU0_TCMT_set(v)         _BGQ_SET(4,27,v)              //!< AXU bits 0,2,4,6 combined.
#define UPC_P__TC_CFG2__A2_FU0_TCMT_insert(x,v)    _UPC_INSERT(24,27,x,v)

#define UPC_P__TC_CFG2__A2_FU1_TCMT_get(x)         _BGQ_GET(4,31,x)
#define UPC_P__TC_CFG2__A2_FU1_TCMT_set(v)         _BGQ_SET(4,31,v)              //!< AXU bits 1,3,5,7 combined.
#define UPC_P__TC_CFG2__A2_FU1_TCMT_insert(x,v)    _UPC_INSERT(28,31,x,v)

#define UPC_P__TC_CFG2__A2_LSU0_TCMT_get(x)        _BGQ_GET(4,35,x)
#define UPC_P__TC_CFG2__A2_LSU0_TCMT_set(v)        _BGQ_SET(4,35,v)             //!< LSU bits 0,2,4,6 combined.
#define UPC_P__TC_CFG2__A2_LSU0_TCMT_insert(x,v)   _UPC_INSERT(32,35,x,v)

#define UPC_P__TC_CFG2__A2_LSU1_TCMT_get(x)        _BGQ_GET(4,39,x)
#define UPC_P__TC_CFG2__A2_LSU1_TCMT_set(v)        _BGQ_SET(4,39,v)             //!< LSU bits 1,3,5,7 combined.
#define UPC_P__TC_CFG2__A2_LSU1_TCMT_insert(x,v)   _UPC_INSERT(36,39,x,v)

#define UPC_P__TC_CFG2__A2_MMU0_TCMT_get(x)        _BGQ_GET(4,43,x)
#define UPC_P__TC_CFG2__A2_MMU0_TCMT_set(v)        _BGQ_SET(4,43,v)             //!< MMU bits 0,2,4,6 combined.
#define UPC_P__TC_CFG2__A2_MMU0_TCMT_insert(x,v)   _UPC_INSERT(40,43,x,v)

#define UPC_P__TC_CFG2__A2_MMU1_TCMT_get(x)        _BGQ_GET(4,47,x)
#define UPC_P__TC_CFG2__A2_MMU1_TCMT_set(v)        _BGQ_SET(4,47,v)             //!< MMU bits 1,3,5,7 combined.
#define UPC_P__TC_CFG2__A2_MMU1_TCMT_insert(x,v)   _UPC_INSERT(44,47,x,v)

#define UPC_P__TC_CFG2__A2_IU0_TCMT_get(x)         _BGQ_GET(4,51,x)
#define UPC_P__TC_CFG2__A2_IU0_TCMT_set(v)         _BGQ_SET(4,51,v)              //!< IU bits 0,2,4,6 combined.
#define UPC_P__TC_CFG2__A2_IU0_TCMT_insert(x,v)    _UPC_INSERT(48,51,x,v)

#define UPC_P__TC_CFG2__A2_IU1_TCMT_get(x)         _BGQ_GET(4,55,x)
#define UPC_P__TC_CFG2__A2_IU1_TCMT_set(v)         _BGQ_SET(4,55,v)              //!< IU bits 1,3,5,7 combined.
#define UPC_P__TC_CFG2__A2_IU1_TCMT_insert(x,v)    _UPC_INSERT(52,55,x,v)

#define UPC_P__TC_CFG2__A2_XU0_TCMT_get(x)         _BGQ_GET(4,59,x)
#define UPC_P__TC_CFG2__A2_XU0_TCMT_set(v)         _BGQ_SET(4,59,v)              //!< XU bits 0,2,4,6 combined.
#define UPC_P__TC_CFG2__A2_XU0_TCMT_insert(x,v)    _UPC_INSERT(56,59,x,v)

#define UPC_P__TC_CFG2__A2_XU1_TCMT_get(x)         _BGQ_GET(4,63,x)
#define UPC_P__TC_CFG2__A2_XU1_TCMT_set(v)         _BGQ_SET(4,63,v)              //!< XU bits 1,3,5,7 combined.
#define UPC_P__TC_CFG2__A2_XU1_TCMT_insert(x,v)    _UPC_INSERT(60,63,x,v)





//! \brief: upc_p_control
//! control the run and reset of counters
#define UPC_P__CONTROL_W1C_offset                  (0x664)    //!< Counter Control reg offset
#define UPC_P__CONTROL_W1S_offset                  (0x665)    //!< Counter Control reg offset
#define UPC_P__CONTROL_WR_offset                   (0x666)    //!< Counter Control reg offset
//! Reset counter
#define UPC_P__CONTROL__RESET_get(x)               _BGQ_GET(24,23,x)
#define UPC_P__CONTROL__RESET_set(v)               _BGQ_SET(24,23,v)
#define UPC_P__CONTROL__RESET_insert(x,v)          _UPC_INSERT(0,23,x,v)
#define UPC_P__CONTROL__RESET_bit(b)               (1UL<<(63-(0+(b))))
//! Enable counter (starts counting based on "sync_override"
//! If "sync override" is disabled (0), then run bits are ignored.
#define UPC_P__CONTROL__RUN_get(x)                 _BGQ_GET(24,55,x)
#define UPC_P__CONTROL__RUN_set(v)                 _BGQ_SET(24,55,v)
#define UPC_P__CONTROL__RUN_insert(x,v)            _UPC_INSERT(32,55,x,v)
#define UPC_P__CONTROL__RUN_bit(b)                 (1UL<<(63-(32+(b))))
//! Masks
#define UPC_P__CONTROL__RESET_COUNTER_set(x,cnum) ((x8000000000000000ULL)>>cnum)
#define UPC_P__CONTROL__RUN_COUNTER_set(x,cnum)   ((x0000000080000000ULL)>>cnum)
#define UPC_P__CONTROL__RESET_MASK                (0xFFFFFF0000000000ULL)   //!< mask of just reset bits
#define UPC_P__CONTROL__RUN_MASK                  (0x00000000FFFFFF00ULL)   //!< mask of just run bits

#define UPC_P__CONTROL32_bit(b)                   (1UL<<(31-(b)))   //!< equivalent counter bit, but for within 32 bit word


//! \brief: upc_p_mmio_t
//! UPC_P MMIO Structure
//! set pointer addr to UPC_P_MMIO_ADDR
struct upc_p_mmio_struct {

    uint64_t  counter[UPC_P_NUM_COUNTERS];        //!< 14 bit counter array + extra bits. MMIO@=0x3ffe8003000
    uint64_t  reserved1[32-UPC_P_NUM_COUNTERS];   // pad out to 32 counters of space
    uint64_t  counter_cfg[UPC_P_NUM_COUNTERS];    //!< counter configuration array        MMIO@=0x3ffe8003100
    uint64_t  reserved2[32-UPC_P_NUM_COUNTERS];   // pad out to 32 counters of space

    uint64_t  cfg_invert;                         //!< event signal inversion selection   MMIO@=0x3ffe8003200
    uint64_t  cfg_edge;                           //!< event signal edge/level selection  MMIO@=0x3ffe8003208

    uint64_t  int_status_w1s;                     //!< counter interrupt status write 1 to set.       MMIO@=0x3ffe8003210
    uint64_t  int_status_w1c;                     //!< counter interrupt status write 1 to clear      MMIO@=0x3ffe8003218
    uint64_t  int_status[4];                      //!< thread x counter interrupt status (read only)  MMIO@=0x3ffe8003220-38
    //uint64_t  int_status_t0;                      //!< thread 0 counter interrupt status (read only)  MMIO@=0x3ffe8003220
    //uint64_t  int_status_t1;                      //!< thread 1 counter interrupt status (read only)  MMIO@=0x3ffe8003228
    //uint64_t  int_status_t2;                      //!< thread 2 counter interrupt status (read only)  MMIO@=0x3ffe8003230
    //uint64_t  int_status_t3;                      //!< thread 3 counter interrupt status (read only)  MMIO@=0x3ffe8003238

    uint64_t  opcode_cfg;                         //!< opcode counting config                         MMIO@=0x3ffe8003240
    uint64_t  m2_trigger;                         //!< mode 2 software trigger                        MMIO@=0x3ffe8003248
    uint64_t  reserved3[22];                      // pad

    uint64_t  p_config;                           //!< upc_p config reg                   MMIO@=0x3ffe8003300

    uint64_t  lp1_tc_config0;                     //!< upc_p lp1 thread combine config    MMIO@=0x3ffe8003308
    uint64_t  lp1_tc_config1;                     //!< upc_p lp1 thread combine config    MMIO@=0x3ffe8003310
    uint64_t  a2_tc_config;                       //!< upc_p a2 thread combine config     MMIO@=0x3ffe8003318

    uint64_t  control_w1c;                        //!< counter control write 1 to clear   MMIO@=0x3ffe8003320
    uint64_t  control_w1s;                        //!< counter control write 1 to set     MMIO@=0x3ffe8003328
    uint64_t  control;                            //!< counter control read/write reg     MMIO@=0x3ffe8003330

} _PACK64_ ;

typedef volatile struct upc_p_mmio_struct upc_p_mmio_t;



__END_DECLS

#endif


