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
 * \brief UPC compile time cnk application definitions
 *
 */
//@}

#include <unistd.h>
#include <sys/syscall.h>
#define upci_gettid() syscall(207, 0) // not defined by glibc


#include "spi/include/kernel/location.h"
#include "spi/include/upci/upc_common.h"
#include "cnk/include/SPI_syscalls.h"



__BEGIN_DECLS



__INLINE__ int Kernel_Upci_Raw_Syscall( uint64_t op, uint64_t target, void *pData, size_t dSize)
{
    return (int) CNK_SPI_SYSCALL_4(UPCI_OP, op, target, pData, dSize);
}



__INLINE__ int Kernel_Upci_Raw_Syscall_FixRC( uint64_t op, uint64_t target, void *pData, size_t dSize)
{
    int rc = 0;

    // Let's use the unused target value to track in flightlog entries how many times this call has repeated.
    uint64_t incrTarget = 0;
    if ((int64_t)target < 0) {
        incrTarget = 1;
        target = 0;
    }

    do {
        rc = Kernel_Upci_Raw_Syscall( op, target, pData, dSize);
        //fprintf(stderr, "upc syscall thd=%d: op:%ld, rc:%d\n", Kernel_ProcessorID(), op, rc);
        if (rc == EAGAIN) {
            Upci_Delay(1000);  // avoid a tight loop
            if (incrTarget) { target++; }
            #ifdef UPCI_INCL_DEBUG_DATA
            UPCI_INCR_DEBUG_COUNT(syscallRetries);
            #endif
        }
    } while (rc == EAGAIN);
    if (rc > 0) { rc *= -1; }
    return rc;
}



__INLINE__ int Kernel_Upci_ResetInts(uint64_t intStatusMask)
{
    uint64_t dummy = 0;
    Kernel_Upci_Raw_Syscall(UPCI_KOP_RESETINTS, intStatusMask, (void*)&dummy, sizeof(dummy));
    return 0;
}



__INLINE__ int Kernel_Upci_GetNodeParms ( Upci_Node_Parms_t *pNodeParms )
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UCPI_KOP_GETPARMS, -1, (void*)pNodeParms, sizeof(Upci_Node_Parms_t));
    if (!rc) {
        pNodeParms->cycleRatio = 1.0 * pNodeParms->cpuSpeed / pNodeParms->nwSpeed;
    }
    return rc;
}



__INLINE__ int Kernel_Upci_GetKDebug(Upci_KDebug_t *pKDebug)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UCPI_KOP_GETKDEBUG, -1, (void*)pKDebug, sizeof(Upci_KDebug_t));
    return rc;
}


__INLINE__ int Kernel_Upci_Mode_Init ( unsigned upcMode, UPC_Ctr_Mode_t ctrMode, unsigned unit )

{
    Upci_ModeInitData_t data;
    data.upcMode = upcMode;
    data.ctrMode = ctrMode;
    data.unit = unit;

    int rc = Kernel_Upci_Raw_Syscall_FixRC(UCPI_KOP_MODEINIT, -1, (void*)&data, sizeof(Upci_ModeInitData_t));
    UPCI_INCR_DEBUG_COUNT(syscallInits);
    //fprintf(stderr, _AT_ " Kernel_Upci_Mode_Init returned rc = %d\n", rc);
    return rc;
}



__INLINE__ int Kernel_Upci_Mode_Free ( )
{
    uint64_t dummy = 0;
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UCPI_KOP_MODEFREE, -1, (void*)&dummy, sizeof(dummy));
    UPCI_INCR_DEBUG_COUNT(syscallFrees);
    return rc;
}



__INLINE__ int Kernel_Upci_A2PC_ApplyRegs( Upci_A2PC_t *pA2pc)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UPCI_KOP_A2PC_APPLY, -1, (void*)pA2pc, sizeof(Upci_A2PC_t));

    return rc;
}



__INLINE__ int Kernel_Upci_A2PC_GetRegs( Upci_A2PC_Val_t *pA2pcVal)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UPCI_KOP_A2PC_GET, -1, (void*)pA2pcVal, sizeof(Upci_A2PC_Val_t));

    return rc;
}



__INLINE__ int Kernel_Upci_Punit_Cfg_Apply( Upci_Punit_Cfg_t *pCfg, unsigned unitId)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UPCI_KOP_PUNIT_CFG_APPLY, unitId, (void*)pCfg, sizeof(Upci_Punit_Cfg_t));
    #ifdef UPCI_INCL_DEBUG_DATA
    UPCI_INCR_DEBUG_COUNT(syscallApplies);
    if (unitId != Kernel_ProcessorCoreID()) {
        UPCI_INCR_DEBUG_COUNT(syscallXCoreApplies);
    }
    #endif

    return rc;
}



__INLINE__ int Kernel_Upci_Punit_Cfg_Attach( Upci_Punit_Cfg_t *pCfg, unsigned unitId)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UPCI_KOP_PUNIT_CFG_ATTACH, unitId, (void*)pCfg, sizeof(Upci_Punit_Cfg_t));
    #ifdef UPCI_INCL_DEBUG_DATA
    UPCI_INCR_DEBUG_COUNT(syscallApplies);
    if (unitId != Kernel_ProcessorCoreID()) {
        UPCI_INCR_DEBUG_COUNT(syscallXCoreApplies);
    }
    #endif

    return rc;
}



__INLINE__ int Kernel_Upci_SetBgpmThread()
{
    uint64_t dummy = 0;
    int rc = Kernel_Upci_Raw_Syscall(UPCI_KOP_SETBGPMTHREAD, -1, (void*)&dummy, sizeof(dummy));
    return (rc);
}

__INLINE__ int Kernel_Upci_ClearBgpmThread()
{
    uint64_t dummy = 0;
    int rc = Kernel_Upci_Raw_Syscall(UPCI_KOP_CLEARBGPMTHREAD, -1, (void*)&dummy, sizeof(dummy));
    return (rc);
}





__INLINE__ int Kernel_Upci_Wait4MailboxEmpty()
{
    uint64_t dummy = 0;
    int rc = Kernel_Upci_Raw_Syscall(UPCI_KOP_WAIT4MAILBOX, -1, (void*)&dummy, sizeof(dummy));
    return (rc);
}



__INLINE__ int Kernel_Upci_SetPmSig(int sig)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UCPI_KOP_SETPMSIG, -1, (void*)&sig, sizeof(int));
    return (rc);
}



__INLINE__ int Kernel_Upci_GetCNKCounts( Upci_CNKCtrType_t ctrType, PerfCountItem_t *ctrBuff, int buffLen)
{
    int rc = Kernel_Upci_Raw_Syscall_FixRC(UPCI_KOP_GETCNKCTRS, (int)ctrType, (void*)ctrBuff, buffLen*sizeof(PerfCountItem_t));
    return rc;
}



//! Upci_LockID is a unique identifier indicating which thread owns a Upci sofware lock.
//! It's used to debug detect when multiple attempts have been made by the same thread
//! allocate the same lock, and tell which thread actually has the lock.
//! The value is relative to the hardware thread, and under CNK also includes the
//! tid() value make completely unique when hw threads are overcommitted.
#ifdef UPC_ATOMIC_C
__thread uint64_t upciLockID = 0;
#else
extern __thread uint64_t upciLockID;
#endif


__INLINE__ uint64_t Upci_GetLockID()
{
    if (upciLockID == 0) {
        upciLockID = upci_gettid();
        upciLockID = upciLockID << 8;
        upciLockID |= (Kernel_ProcessorID() + 1);
    }
    return upciLockID;
}



__END_DECLS


#endif
