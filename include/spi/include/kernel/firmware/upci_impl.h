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

#ifndef _KERNEL_UPCI_IMPL_H_
#define _KERNEL_UPCI_IMPL_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC compile time firmware definitions
 *
 */
//@}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "spi/include/kernel/location.h"
#endif                                                                                                                 // FUSEDOS
#include "spi/include/upci/upc_common.h"
#include "spi/include/upci/upc_scom.h"


__BEGIN_DECLS




//! Is_DD1_Enabled()
//! test if running on DD1 so workaround can be enabled
//! Workarounds should be  enabled if
//! - the JTAG_ID register indicates that this is DD1 hardware
//! - if the DD1Workarounds personality bit is set
//#define IS_PERS_ENABLED(mask)  ( ( fwext_getPersonality()->Kernel_Config.NodeConfig & (mask) ) != 0 )
//#define FW_DD1_WORKAROUNDS_ENABLED() ( TI_isDD1() || IS_PERS_ENABLED( PERS_ENABLE_DD1_Workarounds ) )
__INLINE__ int Is_DD1_Enabled()
{
    if (TI_isDD1()) return 1;

    Personality_t personality;
    Kernel_GetPersonality(&personality, sizeof(Personality_t));
    //FillPersonalityWA(&personality);
    return (personality.Kernel_Config.NodeConfig & PERS_ENABLE_DD1_Workarounds);
}



// does nothing in firmware
__INLINE__ int Kernel_Upci_ResetInts(uint64_t intStatusMask) { return 0; }




__INLINE__ int Kernel_Upci_GetNodeParms ( Upci_Node_Parms_t *pNodeParms )
{
    Personality_t personality;
    Kernel_GetPersonality(&personality, sizeof(Personality_t));

    pNodeParms->cpuSpeed = personality.Kernel_Config.FreqMHz;
    pNodeParms->nwSpeed = TI_NetworkFullSpeed() ? 500 : 250;

    // calc cycleRatio only in user state return logic because
    // floating point operations are not available within kernel.
    //pNodeParms->cycleRatio = pNodeParms->cpuSpeed / pNodeParms->nwSpeed;

    pNodeParms->spareCore = (DCRReadPriv(TESTINT_DCR(REDUND_CTRL)) >> (63-30)) & 0x1f;

    pNodeParms->dd1Enabled = Is_DD1_Enabled();
    pNodeParms->nodeConfig = personality.Kernel_Config.NodeConfig;

    return 0;
}



__INLINE__ int Kernel_Upci_GetKDebug(Upci_KDebug_t *pKDebug)
{
    assert(0);  // Don't call within firmware - call Kernel_Upci_GetKDebug_Copy() instead.
    return 0;
}



__INLINE__ int Kernel_Upci_GetKDebug_Copy(Upci_KDebug_t *pDest, Upci_KDebug_t *pSrc)
{
    memcpy((void*)pDest, (void*)pSrc, sizeof(Upci_KDebug_t));

    pDest->xucr1 = mfspr(SPRN_XUCR1);
    pDest->iullcr = mfspr(SPRN_IULLCR);

    // jobId, coresInited - filled at UPC_Init() time.

    return 0;
}



__INLINE__ int Kernel_Upci_Mode_Init ( unsigned upcMode, UPC_Ctr_Mode_t ctrMode, unsigned unit )
{
    if (Is_DD1_Enabled()) {
        //PRINTF( "Skip A2 Debug Reg config for DD1\n");
    }
    else if (Is_Pers_Enabled(PERS_ENABLE_FPGA)) {
        //PRINTF( "Skip A2 Debug Reg config for FPGA\n");
    }
    else {
        uint64_t saveto = UPC_Init_Scom();
        // Init Opcode Counting for each of the A2s
        unsigned core;
        for (core=0; core<CONFIG_MAX_CORES; core++) {
            //UPC_Enable_Core_Scom(core);
            UPC_Scom_Write( core, A2_SCOM_ABDSR, 0x0000000044600000ULL);
            //UPC_Scom_Error();
            UPC_Scom_Write( core, A2_SCOM_XDSR1, 0x0000000000002180ULL);
            //UPC_Scom_Error();
            //UPC_A2_Dump_State();
        }
        UPC_Restore_Timeout(saveto);
    }

    UPC_C_Clear();
    UPC_C_Clear_SRAM();
#ifdef __FWEXT__
    UPC_C_Init_Mode(upcMode, ctrMode, unit);
#else
    // Caller must initialize the rest of the units (UPC_P, I/O, L2)
    UPC_C_Init_Mode(upcMode, UPC_CM_NO_INIT, unit);
#endif

    return 0;
}



__INLINE__ int Kernel_Upci_Mode_Free ( )
{
    // only has effect in a real syscall
    return 0;
}




__INLINE__ int Kernel_Upci_A2PC_ApplyRegs( Upci_A2PC_t *pA2pc)
{
    Upci_A2PC_ApplyRegs(pA2pc);
    return 0;
}


__INLINE__ int Kernel_Upci_A2PC_GetRegs( Upci_A2PC_Val_t *pA2pcVal)
{
    Upci_A2PC_GetRegs(pA2pcVal);
    return 0;
}



__INLINE__ int Kernel_Upci_Punit_Cfg_Apply( Upci_Punit_Cfg_t *pCfg, unsigned unitId)
{
    //! From firmware, this SPI can only apply to current unit (core)
    //! From CNK user state - the syscall will to core-core interrupt to apply to different core.
    if (unitId != Kernel_ProcessorCoreID()) return UPCI_ENOTIMPL;

    Upci_Punit_Cfg_Apply(pCfg);
    return 0;
}
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS


__INLINE__ int Kernel_Upci_Punit_Cfg_Attach( Upci_Punit_Cfg_t *pCfg, unsigned unitId)
{
    //! From firmware, this SPI can only apply to current unit (core)
    //! From CNK user state - the syscall will to core-core interrupt to apply to different core.
    if (unitId != Kernel_ProcessorCoreID()) return UPCI_ENOTIMPL;

    Upci_Punit_Cfg_Apply(pCfg);
    return 0;
}


__INLINE__ int Kernel_Upci_SetBgpmThread() { return 0; }
__INLINE__ int Kernel_Upci_ClearBgpmThread() { return 0; }


__INLINE__ int Kernel_Upci_Wait4MailboxEmpty()
{
    uint64_t reg0, reg1;
    int loops = 0;
    do {
        loops++;
        reg0 = DCRReadPriv(TESTINT_DCR(MAILBOX_REG0));
        reg1 = DCRReadPriv(TESTINT_DCR(MAILBOX_REG1));
    } while (reg0 != reg1);

    // Upci_Delay(1600);  // get things stable again ?? - it's not helping.
    return loops;
}



__INLINE__ int Kernel_Upci_SetPmSig(int sig)
{
    // do nothing
    return 0;
}


__INLINE__ int Kernel_Upci_GetCNKCounts( Upci_CNKCtrType_t ctrType, PerfCountItem_t *ctrBuff, int buffLen)
{
    // do nothing
    return 0;
}



__INLINE__ uint64_t Upci_GetLockID()
{
    return (Kernel_ProcessorID() + 1);
}


#endif                                                                                                                 // FUSEDOS
__END_DECLS


#endif
