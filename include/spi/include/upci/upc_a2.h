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

#ifndef UPC_A2_H
#define UPC_A2_H

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief A2 Event SPR Bit field routines and common assembly inlines
 *
 * Control routines for the A2 Event SPRs.  They operate
 * on the registers according to event fields.
 *
 * For more information about the Event Select Registers,
 * See A2 user guide sections:
 * - 11.4 "Unit performance event tables"
 * - 11.5 "Unit event select registers"
 *
 *
 * \note: while used in testcases, the eventual UPCI ode
 * my not update on a field basis, but rather a register with a mask.
 *
 * \note: The update of these register requires a read-modify-write,
 * and these operations are not thread safe.  Any locking must be
 * provided outside of these routine calls.
 *
 *
 *
 */
//@}


#include <hwi/include/bqc/A2_inlines.h>

__BEGIN_DECLS


#define MASK32_bit(b) (1ULL<<(31-(b))) //!< only set given bit


__INLINE__ int upc_cntlz8( uint8_t v ) {
    int rval;
    uint32_t v32 = (v << 24);
    asm volatile ("cntlzw %0,%1" : "=r" (rval) : "r" (v32) );
    return rval;
}

__INLINE__ int upc_cntlz16( uint16_t v ) {
    int rval;
    uint32_t v32 = (v << 16);
    asm volatile ("cntlzw %0,%1" : "=r" (rval) : "r" (v32) );
    return rval;
}

__INLINE__ int upc_cntlz32( uint32_t v ) {
    int rval;
    asm volatile ("cntlzw %0,%1" : "=r" (rval) : "r" (v) );
    return rval;
}

__INLINE__ int upc_cntlz64( uint64_t v ) {
    int rval;
    asm volatile ("cntlzd %0,%1" : "=r" (rval) : "r" (v) );
    return rval;
}

#ifdef __HWSTUBS__
__INLINE__ int upc_popcnt32( uint32_t v ) {
    int rval;
    int i;
    for (i=0; i<32; ++i) {
        if (v & 0x1) rval++;
        v = v >> 1;
    }
    return rval;
}
__INLINE__ int upc_popcnt64( uint64_t v ) {
    int rval;
    int i;
    for (i=0; i<64; ++i) {
        if (v & 0x1) rval++;
        v = v >> 1;
    }
    return rval;
}
#else
__INLINE__ int upc_popcnt32( uint32_t v ) {
    int rval;
    asm volatile ("popcntw %0,%1" : "=r" (rval) : "r" (v) );
    return( rval );
}

__INLINE__ int upc_popcnt64( uint64_t v ) {
    int rval;
    asm volatile ("popcntd %0,%1" : "=r" (rval) : "r" (v) );
    return( rval );
}
#endif


// rotate left and insert with mask
// Must be a macro rather than an inline because of the constant operands.
// Note: modifies initial argument (ra).
#define RLIMI32(ra,rs,sh,mb,me) \
    asm("rlwimi %0,%2,%3,%4,%5" \
    : "=r" (ra) : "0" (ra), "r" (rs), "i" (sh), "i" (mb), "i" (me))


#define UNSIGNED32(x) x##U
#define _BGQ_SET32(w,b,v) (((v) & ((UNSIGNED32(2)<<((w)-1))-1)) << (31-(b)))
#define _BGQ_GET32(w,b,x) (((x) >> (31-(b))) & ((UNSIGNED32(2)<<((w)-1))-1))

#if !defined(__ASSEMBLY__)
#define _BGQ_INSERT32(w,b,x,v) RLIMI32(x, (v), (31-(b)), (1+(b)-(w)), (b))
#endif

#define _BwN(b)    _BGQ_SET32(1,b,1)

#define _Bw1(b,v)  _BGQ_SET32(1,b,v)
#define _Bw2(b,v)  _BGQ_SET32(2,b,v)
#define _Bw3(b,v)  _BGQ_SET32(3,b,v)
#define _Bw4(b,v)  _BGQ_SET32(4,b,v)
#define _Bw5(b,v)  _BGQ_SET32(5,b,v)
#define _Bw6(b,v)  _BGQ_SET32(6,b,v)
#define _Bw7(b,v)  _BGQ_SET32(7,b,v)
#define _Bw8(b,v)  _BGQ_SET32(8,b,v)
#define _Bw9(b,v)  _BGQ_SET32(9,b,v)
#define _Bw10(b,v) _BGQ_SET32(10,b,v)
#define _Bw11(b,v) _BGQ_SET32(11,b,v)
#define _Bw12(b,v) _BGQ_SET32(12,b,v)
#define _Bw13(b,v) _BGQ_SET32(13,b,v)
#define _Bw14(b,v) _BGQ_SET32(14,b,v)
#define _Bw15(b,v) _BGQ_SET32(15,b,v)
#define _Bw16(b,v) _BGQ_SET32(16,b,v)
#define _Bw17(b,v) _BGQ_SET32(17,b,v)
#define _Bw18(b,v) _BGQ_SET32(18,b,v)
#define _Bw19(b,v) _BGQ_SET32(19,b,v)
#define _Bw20(b,v) _BGQ_SET32(20,b,v)
#define _Bw21(b,v) _BGQ_SET32(21,b,v)
#define _Bw22(b,v) _BGQ_SET32(22,b,v)
#define _Bw23(b,v) _BGQ_SET32(23,b,v)
#define _Bw24(b,v) _BGQ_SET32(24,b,v)
#define _Bw25(b,v) _BGQ_SET32(25,b,v)
#define _Bw26(b,v) _BGQ_SET32(26,b,v)
#define _Bw27(b,v) _BGQ_SET32(27,b,v)
#define _Bw28(b,v) _BGQ_SET32(28,b,v)
#define _Bw29(b,v) _BGQ_SET32(29,b,v)
#define _Bw30(b,v) _BGQ_SET32(30,b,v)
#define _Bw31(b,v) _BGQ_SET32(31,b,v)



//
// The A2_core.h file contains defines for SPRs (Special Purpose Regs)
// It may eventually contain defines for these.
// SPRN_AESR, SPRN_CESR,
// SPRN_IESR1, SPRN_IESR2,
// SPRN_MESR1, SPRN_MESR2,
// SPRN_XESR1, SPRN_XESR2

#ifndef SPRN_AESR
#define SPRN_CESR	(0x390)	      // 912
#define SPRN_AESR   (0x391)       // 913
#define SPRN_IESR1	(0x392)       // 914
#define SPRN_IESR2	(0x393)       // 915
#define SPRN_MESR1  (0x394)       // 916
#define SPRN_MESR2  (0x395)       // 917
#define SPRN_XESR1  (0x396)       // 918    XU event Select
#define SPRN_XESR2  (0x397)       // 919
#define SPRN_XESR3  (0x398)       // 920    LSU event Select
#define SPRN_XESR4  (0x399)       // 921
#endif


//! CESR Fields
#define SPRN_CESR__ENABLE_PERF_set(x)           _B1(32,x)           //!< Enable performance events and bypass bus - normally set.
#define SPRN_CESR__ENABLE_PERF_get(x)           _BGQ_GET(1,32,x)
#define SPRN_CESR__ENABLE_PERF_insert(x,v)      _UPC_INSERT(32,32,x,v)  // does read/mod/write
#define SPRN_CESR__COUNT_MODES_set(x)           _B3(35,x)           //!< Performance Event Count Modes 0=user, 1=supervisor, 2=hypervisor
#define SPRN_CESR__COUNT_MODES_get(x)           _BGQ_GET(3,35,x)
#define SPRN_CESR__COUNT_MODES_insert(x,v)      _UPC_INSERT(33,35,x,v)
#define SPRN_CESR__ENABLE_TRACE_BUS_set(x)      _B1(36,x)           //!< Enables trace bus
#define SPRN_CESR__ENABLE_TRACE_BUS_get(x)      _BGQ_GET(1,36,x)
#define SPRN_CESR__ENABLE_TRACE_BUS_insert(x,v) _UPC_INSERT(36,36,x,v)
#define SPRN_CESR__INSTR_TRACE_set(x)           _B1(37,x)           //!< Enables core instruction tracing
#define SPRN_CESR__INSTR_TRACE_get(x)           _BGQ_GET(1,37,x)
#define SPRN_CESR__INSTR_TRACE_insert(x,v)      _UPC_INSERT(37,37,x,v)
#define SPRN_CESR__INSTR_TRACE_TID_set(x)       _B2(39,x)           //!< instr trace thread id (0-3)
#define SPRN_CESR__INSTR_TRACE_TID_get(x)       _BGQ_GET(2,39,x)
#define SPRN_CESR__INSTR_TRACE_TID_insert(x,v)  _UPC_INSERT(38,39,x,v)
#define SPRN_CESR__EVENTS_MASK                  _B24(63,0xFFFFFFUL) //!< mask of bits representing event signal assignments

//! CESR Fields for 32 bit register
#define _UPC_INSERT32(b1,b2,x,v) _BGQ_INSERT32((b2-b1+1),b2,x,v)
#define TOB32(_b_) (_b_-32)
#define SPRN_CESR32__ENABLE_PERF_set(x)           _Bw1(TOB32(32),x)           //!< Enable performance events and bypass bus - normally set.
#define SPRN_CESR32__ENABLE_PERF_get(x)           _BGQ_GET32(1,TOB32(32),x)
#define SPRN_CESR32__ENABLE_PERF_insert(x,v)      _UPC_INSERT32(TOB32(32),TOB32(32),x,v)  // does read/mod/write
#define SPRN_CESR32__COUNT_MODES_set(x)           _Bw3(TOB32(35),x)           //!< Performance Event Count Modes 0=user, 1=supervisor, 2=hypervisor
#define SPRN_CESR32__COUNT_MODES_get(x)           _BGQ_GET32(3,TOB32(35),x)
#define SPRN_CESR32__COUNT_MODES_insert(x,v)      _UPC_INSERT32(TOB32(33),TOB32(35),x,v)
#define SPRN_CESR32__ENABLE_TRACE_BUS_set(x)      _Bw1(TOB32(36),x)           //!< Enables trace bus
#define SPRN_CESR32__ENABLE_TRACE_BUS_get(x)      _BGQ_GET32(1,TOB32(36),x)
#define SPRN_CESR32__ENABLE_TRACE_BUS_insert(x,v) _UPC_INSERT32(TOB32(36),TOB32(36),x,v)
#define SPRN_CESR32__INSTR_TRACE_set(x)           _Bw1(TOB32(37),x)           //!< Enables core instruction tracing
#define SPRN_CESR32__INSTR_TRACE_get(x)           _BGQ_GET32(1,TOB32(37),x)
#define SPRN_CESR32__INSTR_TRACE_insert(x,v)      _UPC_INSERT32(TOB32(37),TOB32(37),x,v)
#define SPRN_CESR32__INSTR_TRACE_TID_set(x)       _Bw2(TOB32(39),x)           //!< instr trace thread id (0-3)
#define SPRN_CESR32__INSTR_TRACE_TID_get(x)       _BGQ_GET32(2,TOB32(39),x)
#define SPRN_CESR32__INSTR_TRACE_TID_insert(x,v)  _UPC_INSERT32(TOB32(38),TOB32(39),x,v)
#define SPRN_CESR32__EVENTS_MASK                  _Bw24(63,0xFFFFFFUL) //!< mask of bits representing event signal assignments


//! list of CESR Event Selections for PC Bus event signals 32-39.
//! Also defines the list of available units
enum cesr_a2_unit {
   CESR_XU_EVENT=0,
   CESR_IU_EVENT=1,
   CESR_AXU_EVENT=2,
   CESR_MMU_EVENT=3,
   CESR_LSU_EVENT=4
};
typedef enum cesr_a2_unit cesr_a2_unit_t;



//! A2 Event Count Modes
enum cesr_modes {
    CESR_COUNT_HYPER_EVENTS = 0x1,
    CESR_COUNT_SUPER_EVENTS = 0x2,
    CESR_COUNT_USR_EVENTS   = 0x4,
};
typedef enum cesr_modes cesr_count_modes_t;





//! Init_A2_Counting
//!
//! configure a2 for event counting
//!  Resets the trace and instruction bits,
//!  and leaves the event config bits alone.
//!
//! \param[in] mode   A2 Counting Mode
//!
void Init_A2_Counting(cesr_count_modes_t mode);



//! Clear_A2_Events
//!
//! clear out event config for A2
//!
void Clear_A2_Events();



//! set_cesr_event
//!
//! Set PC Bus Event Signal source in CESR reg
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] unit_type      Which unit to set
//!
void set_cesr_event(short unit_sig_num, cesr_a2_unit_t unit_type);



//! get_cesr_event
//!
//! Get PC Bus Event Signal source in CESR reg
//!
//! \param[in]  unit_sig_num   A2 Event signal number(0-7)
//! \param[out] unit_type      return unit
//!
void get_cesr_event(short unit_sig_num, short *unit_type);



//! set_axu_event
//!
//! Set Quad Floating Point (QFPU=AXU) events - all 8 signals mapped to one SPR
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
void set_axu_event(short unit_sig_num, short thread, short a2_event);



//! get_axu_event
//!
//! Get Quad Floating Point (QFPU=AXU) events
//!
void get_axu_event(short unit_sig_num, short *thread, short *a2_event);



//! set_iu_event
//!
//! Set Instruction Unit events - select between regs to map all 8 signals.
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
void set_iu_event(short unit_sig_num, short thread, short a2_event);



//! get_iu_event
//!
//! Get Instruction Unit events
//!
//! \param[in]  unit_sig_num   A2 Event signal number(0-7)
//! \param[out] thread    A2 hardware thread number (0-3)
//! \param[out] a2_event  a2 unit event number
//!
void get_iu_event(short unit_sig_num, short *thread, short *a2_event);



//! set_xu_event
//!
//! Set Execution Unit events - select between regs to map all 8 signals.
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
void set_xu_event(short unit_sig_num, short thread, short a2_event);



//! get_xu_event
//!
//! Get Execution Unit events
//!
//! \param[in]  unit_sig_num   A2 Event signal number(0-7)
//! \param[out] thread    A2 hardware thread number (0-3)
//! \param[out] a2_event  a2 unit event number
//!
void get_xu_event(short unit_sig_num, short *thread, short *a2_event);



//! set_lsu_event
//!
//! Set Load Store Unit events - select between regs to map all 8 signals.
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
void set_lsu_event(short unit_sig_num, short thread, short a2_event);



//! get_lsu_event
//!
//! Get Load Store Unit events
//!
//! \param[in]  unit_sig_num   A2 Event signal number(0-7)
//! \param[out] thread    A2 hardware thread number (0-3)
//! \param[out] a2_event  a2 unit event number
//!
void get_lsu_event(short unit_sig_num, short *thread, short *a2_event);



//! set_mmu_event
//!
//! Set Memory Management Unit events - select between regs to map all 8 signals.
//!
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
void set_mmu_event(short unit_sig_num, short thread, short a2_event);



//! get_mmu_event
//!
//! Get Memory Management Unit events
//!
//! \param[in]  unit_sig_num   A2 Event signal number(0-7)
//! \param[out] thread    A2 hardware thread number (0-3)
//! \param[out] a2_event  a2 unit event number
//!
void get_mmu_event(short unit_sig_num, short *thread, short *a2_event);


__END_DECLS

#endif
