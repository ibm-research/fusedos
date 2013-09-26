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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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

#ifndef _UPCI_PUNIT_CONFIG_H_  // Prevent multiple inclusion
#define _UPCI_PUNIT_CONFIG_H_

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <assert.h>

#include "err.h"
#include "upc_evttbl.h"
#endif                                                                                                                 // FUSEDOS
#include "upc_a2_pc.h"
#include "mode.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "punit_eventrecs.h"
#else                                                                                                                  // FUSEDOS
#include "hwi/include/bqc/upc_p_hw.h"                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS



//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief UPC Punit Hardware Configuration objects.
 *
 * Objects of this type track the configuration registers for a Punit necessary
 * to apply and control a set of punit events.  The structure is
 * shared with the kernel via syscalls for the purpose of
 * applying or attaching the configuration.
 *
 * The object use is wrapped by the Punit object and methods.  However, the
 * usage is as follows:
 * - A pre-reserved counter is configured via a method on the Punit_Cfg_t object.
 *   also indicating whether it's a shared or overflow counter.
 * - Attributes to counters are changeable in a similar fashion.
 * - The apply method will apply the active config to hardware.
 */
//@}


__BEGIN_DECLS






//////////////////////////////////////////////////////////////////
/*!
 *
 * Punit_Cfg_t tracks the UPC HW register settings
 * for a threads upc_p counters.
 * Contained are multiple muxable configs, along with
 * core-wide settings, shared, and overflow counter settings.
 *
 * This object will be referenced internally by the kernel, or be
 * contained in shared memory - so it may not contain pointers.
 */
typedef struct sPunit_Cfg
{
    uint32_t    cfgVersion;     //!< version of this structure and using code -
                                //!< for checking within kernel to insure
                                //!< consistent interface user state and kernel state.

    Upci_Mode_t mode;           //!< copy of global mode values to help with decisions
    unsigned    cCtrGrp;        //!< upc_c counter group to use for these 24 counters.  Needed because the spare core on DD1 or in SWModes
                                //!<   can shift the location of the associated upc_c off of the normally static unit to counter group mapping.

    int         l1pHwMode;      //!< l1p hw mode (L1P_CFG_UPC)
                                //!<   if <0 means cfg only enable bit for base/wu events
                                //!<   for switch/stream or list include enable bit

    UpciBool_t  syncStart;      //!< hardware synchronize the start of counters across threads and cores.
    UpciBool_t  enableInts;     //!< enable pm interrupts for the threads in this config.

    uint64_t    intStatusSav;   //!< ctx sw save/restore for upc_p int status (stale location not used - upcHwSwapCfgs in upci_syscall used instead)
    uint64_t    intMaskSav;     //!< ctx sw save/restore for upc_p config register for int masks only (not used)

    uint64_t    intEnableMask;  //!< which upc_p interrupt mask bits should be initialized (only used during
                                //!< apply - not by ctx sw

    //uint32_t    sharedCtrMask;  //!< Mask of configured counters which are shared (Cfg in muxCfgs[0])
    uint32_t    ovfCtrMask;       //!< Mask of counters which have overflow active (Cfg in muxCfg[0])
    uint32_t    thdCtrMask;       //!< Which threaded counters are configured (bits 0-24)

    uint64_t    ctrCfg[UPC_P_NUM_COUNTERS];         //!< upc_p counter config values

    unsigned    ctrBkRefIdx[UPC_P_NUM_COUNTERS];    //!< associated bgpm event index used by counter for quicker return of overflows
                                                    //!< punit only tracks value with counter for return by Upci_Punit_GetOvfBackRefs()
                                                    //!< - doesn't otherwise use.
    uint64_t    ctrOvfThres[UPC_P_NUM_COUNTERS];    //!< configured thresholds for ctrs configured to overflow
    uint64_t    ctrOvfAccum[UPC_P_NUM_COUNTERS];    //!< accumulated value for overflow counters
    uint64_t    ctrSS[UPC_P_NUM_COUNTERS];          //!< snapshot of counter values - restored on apply and updated on write.

    uint64_t    opCodeMatch;    //!< If nonzero - desired Opcode Match Value - see \ref UPC_P__OPCODE_CFG_offset
    uint64_t    fpWeights;      //!< Floating Point Weight bits for upc_p config

     //! A2 Signal Inversion & Mask -
     uint64_t   a2Invert;      //!< A2 Signal Inversion Bits - See \ref UPC_P__CONFIG_INVERT_offset
     uint64_t   a2Edge;        //!< A2 & L1p Signal Edge Bits - See \ref UPC_P__CONFIG_EDGE_offset
     uint64_t   a2EdgeMask;    //!< A2 & L1p Invert & Edge significant bits (edge has l1p bits, but ignored with Invert)

     //! A2 Event SPRs and Masks
     Upci_A2PC_t a2sprs;

} Upci_Punit_Cfg_t;


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
//  Notes about ctrOvfThres, ctrOvfAccum, and ctrSS arrays
//     On config init - all of these are cleared.
//     On set overflow - ctrOvfThres and ctrSS entries are set to threshold value (along with ovfCtrMask indicating ovf counter)
//     On Apply, upcp & upcc counters are initialized to ctrSS values (rather than merely reset)
//     On SnapShot (used by multiplexing operations), current counter values are read and stored in ctrSS.
//     On overflow (AccumResetThreshold), the overflowing counters have their value accumulated to ctrOvfAccum and counter reset to threshold.
//        ctrSS is not changed and remains a stale value (waiting for SS operation to update)
//    ?? subsequent apply issue??
//     On Reset, ctrSS is cleared, ctrOvfAccum is cleared, counters are cleared, and ctrSS for overflow ctrs is reset to theshold along with ctrs.
//     On Write, counter is written, but ctrSS remains stale.  It is not supported to write to an overflow counter.
//     Reads just read counter, except for ovfs which read ctrOvfAccum + an adjusted current ctr value.


/*!
 * Upci_Punit_Cfg_SetContext
 * @param[in]   pCfg       object to initialize
 * @param[in]   ctx        CESR_COUNT_... mask
 */
__INLINE__ void Upci_Punit_Cfg_SetContext(Upci_Punit_Cfg_t *pCfg, uint32_t ctx)
{
    Upci_A2PC_SetCesrMode(&(pCfg->a2sprs), ctx);
}




/**
 * \brief Upci_Punit_Cfg_GetCtlMask
 * return counter control mask
 * @param[in]   pCfg     object
 */
__INLINE__ uint32_t Upci_Punit_Cfg_GetCtlMask(Upci_Punit_Cfg_t * pCfg)
{
    assert(pCfg);
    return (pCfg->thdCtrMask | pCfg->ovfCtrMask);
}



/*!
 * initialize a Cfg object
 * @param[in]   pCfg    reference to object to initialize
 * @param[in]   pMode   mode object
 */
__INLINE__ void Upci_Punit_Cfg_Init(Upci_Punit_Cfg_t * pCfg, Upci_Mode_t *pMode)
{
    assert(pCfg);
    memset(pCfg, 0, sizeof(Upci_Punit_Cfg_t));
    memcpy(&(pCfg->mode), pMode, sizeof(Upci_Mode_t));
    pCfg->l1pHwMode = -1;
    pCfg->cfgVersion = UPCI_CFG_VERSION;

    pCfg->syncStart = (pMode->ctrMode == UPC_CM_SYNC);

    Upci_A2PC_Init(&(pCfg->a2sprs));

    // set default floating point weights
    pCfg->fpWeights = UPC_P__CONFIG__DIV_WEIGHT_set(3)  // 8 flops/op
                      | UPC_P__CONFIG__SQRT_WEIGHT_set(3)  // 8 flops/op
                      | UPC_P__CONFIG__AXU_MATCH_WEIGHT_set(0);  // 1 flop/op

    // Set default context
    Upci_Punit_Cfg_SetContext(pCfg, (uint32_t)(CESR_COUNT_HYPER_EVENTS
                                               | CESR_COUNT_SUPER_EVENTS
                                               | CESR_COUNT_USR_EVENTS));
}



/*! Upci_Punit_Cfg_Dump
 * Dump object for debug purposes.
 */
void Upci_Punit_Cfg_Dump(unsigned indent, Upci_Punit_Cfg_t * o);



/*!
 * Upci_Punit_Cfg_AddEvent
 * Add a prereserved event to the punit configuration.
 * You indicate  whether the event is shared (or overflow)
 * This method does not protect caller from configuring events which conflict.  Rather, it assumes
 * reservations have been done to determine if conflicts occur
 * @param[in]   pCfg     object to update
 * @param[in]   pEvtRec  Pre-reserved event resource properties
 * @param[in]   bkRefIdx optionally caller's provides an index value into it's own array for the event.
 *                         this will be used to for the given event.  Caller gives value into it's own array,
 *                         to use as a back reference for a given event. Used to point back to overflowing
 *                         events.
*/
void Upci_Punit_Cfg_AddEvent(Upci_Punit_Cfg_t *pCfg, Upci_PunitEventRec_t *pEvtRec, int bkRefIdx);




#endif                                                                                                                 // FUSEDOS
/**
 * \brief Upci_Punit_Cfg_Apply
 * Kernel only routine to apply the UPC config to the hardware.
 * WARNING: the apply is to the current core or thread, so it assumes the
 * contained event configuration is consistent.
 * Also assumes effected counters are already stopped.
 * @param[in]   pCfg  object to apply
 */
void Upci_Punit_Cfg_Apply(Upci_Punit_Cfg_t *pCfg);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS


//! \brief: Set Edge Bit Value on Signals
//! Only valid for a2 and l1p base events.
//! @param[in]  pCfg        object
//! @param[in]  sig         a2 signal number or target hwthread for l1p.
//! @param[in]  pEvent      src event
//! @param[in]  edge        1 to set edge, 0 to set cycle
//! @return     UpciFalse when setting edges is not valid for the event
UpciBool_t Upci_Punit_Cfg_Edge_Masks(Upci_Punit_Cfg_t *pCfg, ushort sig, UPC_PunitEvtTableRec_t *pEvent, ushort edge);



//! \brief: Get Edge value for signal
//! Only valid for a2 and l1p base events.
//! @param[in]  pCfg        object
//! @param[in]  sig         a2 signal number or target hwthread if event is l1p.
//! @param[in]  pEvent      src event
//! @return     1 when edge is set, 0 if no valid or level is set.
ushort Upci_Punit_Cfg_GetEdgeValue(Upci_Punit_Cfg_t *pCfg, ushort sig, UPC_PunitEvtTableRec_t *pEvent);



//! \brief: Set the Inversion bits on signals
//! Only valid for A2 events
//! @param[in]  pCfg        object
//! @param[in]  sig         a2 signal number or target hwthread for l1p.
//! @param[in]  pEvent      src event
//! @param[in]  invert      1 to invert, 0 to notinvert
//! @return     UpciFalse when setting inversion is not valid for the event.
UpciBool_t Upci_Punit_Cfg_Invert_Masks(Upci_Punit_Cfg_t *pCfg, ushort sig, UPC_PunitEvtTableRec_t *pEvent, ushort invert);



//! \brief: Get Invert value for signal
//! Only valid for A2 events.
//! @param[in]  pCfg        object
//! @param[in]  sig         a2 signal number or target hwthread if event is l1p.
//! @param[in]  pEvent      src event
//! @return     1 when inversion is set, 0 if not valid for event or if is not set.
ushort Upci_Punit_Cfg_GetInvertValue(Upci_Punit_Cfg_t *pCfg, ushort sig, UPC_PunitEvtTableRec_t *pEvent);



/**
 * \brief Upci_Punit_Cfg_StopN_Snapshot_Counts
 * Stop counting and Snapshot the current counter values.
 * This exists to allow capturing the values prior to applying a different
 * punit to the same counters.  Then when reapplying the counters, the
 * snapshotted values would be restored.
 * The hardware counters are not reset by this method.
 *
 * @param[in]   pCfg     object
 *
 */
void Upci_Punit_Cfg_StopN_Snapshot_Counts(Upci_Punit_Cfg_t *pCfg);
#endif                                                                                                                 // FUSEDOS



__END_DECLS


#endif
