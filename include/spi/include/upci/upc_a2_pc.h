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

#ifndef UPC_A2_PC_H
#define UPC_A2_PC_H

//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief A2 PC Event SPR Structure and methods
 *
 * Methods to populate a structure for and A2 event registers and
 * track signficant bits for the current thread.
 * Structure used with syscalls to RMW the appropriate registers.
 *

 */
//@}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <stdio.h>
#endif                                                                                                                 // FUSEDOS
#include "upc_common.h"
#include "upc_a2.h"

__BEGIN_DECLS


//! \brief Upci_A2PC_t
//! Structure to manage A2 PC Event Reg to set.
typedef struct sUpci_A2PC
{
    uint32_t cesr;
    uint32_t cesrMask;
    uint32_t aesr;
    uint32_t aesrMask;
    uint32_t iesr1;
    uint32_t iesr1Mask;
    uint32_t iesr2;
    uint32_t iesr2Mask;
    uint32_t mesr1;
    uint32_t mesr1Mask;
    uint32_t mesr2;
    uint32_t mesr2Mask;
    uint32_t xesr1;
    uint32_t xesr1Mask;
    uint32_t xesr2;
    uint32_t xesr2Mask;
    uint32_t xesr3;
    uint32_t xesr3Mask;
    uint32_t xesr4;
    uint32_t xesr4Mask;

} Upci_A2PC_t;



//! \brief Upci_A2PC_Val_t
//! Structure to query value of A2 PC Event Regs
typedef struct sUpci_A2PC_Val
{
    uint32_t cesr;
    uint32_t aesr;
    uint32_t iesr1;
    uint32_t iesr2;
    uint32_t mesr1;
    uint32_t mesr2;
    uint32_t xesr1;
    uint32_t xesr2;
    uint32_t xesr3;
    uint32_t xesr4;

} Upci_A2PC_Val_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS


//! Upci_A2PC_Init
//! \param[in] pA2pc   A2pc structure to init
__INLINE__ void Upci_A2PC_Init(Upci_A2PC_t *pA2pc)
{
    assert(pA2pc);
    memset(pA2pc, 0, sizeof(Upci_A2PC_t));
    pA2pc->cesr     = SPRN_CESR32__ENABLE_PERF_set(1) | SPRN_CESR32__ENABLE_TRACE_BUS_set(1);
    pA2pc->cesrMask = SPRN_CESR32__ENABLE_PERF_set(1) | SPRN_CESR32__ENABLE_TRACE_BUS_set(1);
    //printf("cesr: 0x%08x\n", pA2pc->cesr);
    //printf("cesrMask: 0x%08x\n", pA2pc->cesrMask);

}



//! Upci_A2PC_SetCesrMode
//!
//! set cesr counting mode (counting context)
//! \param[in] pA2pc  A2pc structure
//! \param[in] cmode  A2 Counting Mode
//!
__INLINE__ void Upci_A2PC_SetCesrMode(Upci_A2PC_t *pA2pc, uint32_t cmode)
{
    assert(pA2pc);
    SPRN_CESR32__COUNT_MODES_insert(pA2pc->cesr, cmode);
    SPRN_CESR32__COUNT_MODES_insert(pA2pc->cesrMask, 0xFFFF);
    //pA2pc->cesr = (pA2pc->cesr & ~SPRN_CESR32__COUNT_MODES_set(0xFFFF)) | SPRN_CESR32__COUNT_MODES_set(cmode);
    //pA2pc->cesrMask = pA2pc->cesrMask | SPRN_CESR32__COUNT_MODES_set(0xFFFF);
    //printf("cmode: 0x%08x\n", cmode);
    //printf("cesr: 0x%08x\n", pA2pc->cesr);
    //printf("cesrMask: 0x%08x\n", pA2pc->cesrMask);

}



//! Upci_A2PC_SetCesrEvent
//!
//! Set PC Bus Event Signal source for the CESR reg
//!
//! \param[in] pA2pc  A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] unit_type      Which unit to set
//!
__INLINE__ void Upci_A2PC_SetCesrEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, cesr_a2_unit_t unit_type)
{
    assert(pA2pc);
    short offset  = 39-32+3 + (unit_sig_num*3);   // before left most bit + field length
    uint32_t mask = _BS32( offset, 0x7, 3 );
    uint32_t val  = _BS32( offset, unit_type, 3 );
    pA2pc->cesr   = ((~mask & pA2pc->cesr) | val);
    pA2pc->cesrMask |= mask;
}



//! Upci_A2PC_SetAxuEvent
//!
//! Set Quad Floating Point (QFPU=AXU) events - all 8 signals mapped to one SPR
//!
//! \param[in] pA2pc          A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
__INLINE__ void Upci_A2PC_SetAxuEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, short thread, short a2_event)
{
    assert(pA2pc);
    short offset    = 31+4-32+(unit_sig_num*4);   /* 1 bit for 2 threads, 3 bits for 8 QFPU events */
    uint32_t mask = _BS32( offset, 0xF, 4 );
    uint32_t val  = _BS32( offset, ((0x1 & thread) << 3) | (0x7 & a2_event), 4 );
    pA2pc->aesr   = ((~mask & pA2pc->aesr) | val);
    pA2pc->aesrMask |= mask;
}



//! Upci_A2PC_SetIuEvent
//!
//! Set Instruction Unit events - select between regs to map all 8 signals.
//!
//! \param[in] pA2pc          A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
__INLINE__ void Upci_A2PC_SetIuEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, short thread, short a2_event)
{
    short use_2nd_spr  = (0x4 & unit_sig_num);             // spr1 for signals 0-3, spr2 for signals 4-7.
    short offset       = 31+6-32+( (0x3 & unit_sig_num)*6);   /* 1 bits for 2 threads, 5 bits for 32 a2_events */
    uint32_t mask    = _BS32( offset, 0x3F, 6 );
    uint32_t val     = _BS32( offset, ((0x1 & thread) << 5) | (0x1F & a2_event), 6 );
    uint32_t spr_val = (use_2nd_spr) ? pA2pc->iesr2 : pA2pc->iesr1;
    spr_val          = (~mask & spr_val) | val;
    if (use_2nd_spr) {
        pA2pc->iesr2 = spr_val;
        pA2pc->iesr2Mask |= mask;
    }
    else {
        pA2pc->iesr1 = spr_val;
        pA2pc->iesr1Mask |= mask;
    }
}



//! Upci_A2PC_SetXuEvent
//!
//! Set Execution Unit events - select between regs to map all 8 signals.
//!
//! \param[in] pA2pc          A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
__INLINE__ void Upci_A2PC_SetXuEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, short thread, short a2_event)
{
    short use_2nd_spr  = (0x4 & unit_sig_num);             // spr1 for signals 0-3, spr2 for signals 4-7.
    short offset       = 31+6-32+( (0x3 & unit_sig_num)*6);   /* 1 bits for 2 threads, 5 bits for 32 a2_events */
    uint32_t mask    = _BS32( offset, 0x3F, 6 );
    uint32_t val     = _BS32( offset, ((0x1 & thread) << 5) | (0x1F & a2_event), 6 );
    uint32_t spr_val = (use_2nd_spr) ? pA2pc->xesr2 : pA2pc->xesr1;
    spr_val          = (~mask & spr_val) | val;
    if (use_2nd_spr) {
        pA2pc->xesr2 = spr_val;
        pA2pc->xesr2Mask |= mask;
    }
    else {
        pA2pc->xesr1 = spr_val;
        pA2pc->xesr1Mask |= mask;
    }
}



//! Upci_A2PC_SetLsuEvent
//!
//! Set Load Store Unit events - select between regs to map all 8 signals.
//!
//! \param[in] pA2pc          A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
__INLINE__ void Upci_A2PC_SetLsuEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, short thread, short a2_event)
{
    short use_2nd_spr  = (0x4 & unit_sig_num);             // spr1 for signals 0-3, spr2 for signals 4-7.
    short offset       = 31+6-32+( (0x3 & unit_sig_num)*6);   /* 1 bits for 2 threads, 5 bits for 32 a2_events */
    uint32_t mask    = _BS32( offset, 0x3F, 6 );
    uint32_t val     = _BS32( offset, ((0x1 & thread) << 5) | (0x1F & a2_event), 6 );
    uint32_t spr_val = (use_2nd_spr) ? pA2pc->xesr4 : pA2pc->xesr3;
    spr_val          = (~mask & spr_val) | val;
    if (use_2nd_spr) {
        pA2pc->xesr4 = spr_val;
        pA2pc->xesr4Mask |= mask;
    }
    else {
        pA2pc->xesr3 = spr_val;
        pA2pc->xesr3Mask |= mask;
    }
}



//! Upci_A2PC_SetMmuEvent
//!
//! Set Memory Management Unit events - select between regs to map all 8 signals.
//!
//! \param[in] pA2pc          A2pc structure
//! \param[in] unit_sig_num   A2 Event signal number(0-7)
//! \param[in] thread    A2 hardware thread number (0-3)
//! \param[in] a2_event  a2 unit event number
//!
__INLINE__ void Upci_A2PC_SetMmuEvent(Upci_A2PC_t *pA2pc, short unit_sig_num, short thread, short a2_event)
{
    short use_2nd_spr  = (0x4 & unit_sig_num);             // spr1 for signals 0-3, spr2 for signals 4-7.
    short offset       = 31+5-32+( (0x3 & unit_sig_num)*5);   /* 1 bits for 2 threads, 4 bits for 16 a2_events */
    uint32_t mask    = _BS32( offset, 0x1F, 5 );
    uint32_t val     = _BS32( offset, ((0x1 & thread) << 4) | (0xF & a2_event), 5 );
    uint32_t spr_val = (use_2nd_spr) ? pA2pc->mesr2 : pA2pc->mesr1;
    spr_val          = (~mask & spr_val) | val;
    if (use_2nd_spr) {
        pA2pc->mesr2 = spr_val;
        pA2pc->mesr2Mask |= mask;
    }
    else {
        pA2pc->mesr1 = spr_val;
        pA2pc->mesr1Mask |= mask;
    }
}



#endif                                                                                                                 // FUSEDOS
//! Upci_A2PC_ApplyRegs
//!
//! Firmware only routine to apply the A2PC Event Config to the current core
//!
//! \param[in] pA2pc     A2pc structure
//!
void Upci_A2PC_ApplyRegs(const Upci_A2PC_t *pA2pc);



//! Upci_A2PC_Dump
//! dump object for debug purposes
void Upci_A2PC_Dump(unsigned indent, Upci_A2PC_t *pA2pc);




//! Upci_A2PC_GetRegs
//!
//! Fimeware only routine to get values for A2PC Event SPRs
//!
//! \param[in] pA2pcVal     A2pc_Val structure
//!
void Upci_A2PC_GetRegs(Upci_A2PC_Val_t *pA2pcVal);



//! Upci_A2PC_DumpRegs
//!
//! Print retrieved contents of A2PC Event SPRs
//!
//! \param[in] pA2pcVal     A2pc_Val structure
//! \param[in] indent       indent level for printing
//!
void Upci_A2PC_DumpRegs_Indent(Upci_A2PC_Val_t *pA2pcVal, int indent);
__INLINE__ void Upci_A2PC_DumpRegs(Upci_A2PC_Val_t *pA2pcVal) { Upci_A2PC_DumpRegs_Indent(pA2pcVal, 0); }




__END_DECLS

#endif
