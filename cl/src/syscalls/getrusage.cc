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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

// Includes

#include "Kernel.h"
#include <sys/resource.h>    // struct rusage

#define MILLION 1000000 

uint64_t sc_getrusage(SYSCALL_FCN_ARGS)
{
    int who = r3;
    struct rusage *buf = (struct rusage *)r4;

    TRACESYSCALL(("(I) %s[%d]: who=%d, buf=0x%016lx\n", __func__, ProcessorID(), who, (unsigned long)buf));

    // Check for error conditions.
    if (who != RUSAGE_SELF)
    {
        return CNK_RC_FAILURE(EINVAL);
    }
    if (!VMM_IsAppAddress(buf, sizeof(*buf)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }
    AppProcess_t *pAppProc = GetMyProcess();
    uint64_t text_size = pAppProc->Data_VEnd - pAppProc->Data_VStart;
    uint64_t heap_vaddr = pAppProc->Heap_VStart;
    uint64_t heap_size = pAppProc->Heap_End - pAppProc->Heap_Start;
    uint64_t maxAlloc_vaddr = MAX(pAppProc->Heap_Break, pAppProc->MmapMgr.high_mark);
    uint64_t processLeader_r1 = pAppProc->ProcessLeader_KThread->Reg_State.gpr[1];
    uint64_t now = GetCurrentTimeInMicroseconds() - GetMyAppState()->JobStartTime;
    /* Multi-core can't be completely precise when a non-process-leader thread
     * does a getrusage() and needs to get the *current* stack depth of the
     * process leader.  Probably should interrupt the process leader in this
     * case, but for now - we'll just snoop the last recorded gpr1 value.
     * If we happen to be the process leader or are on the same hwthread as
     * the leader, the r1 value we're about to pick up will be precise.
     */

    // All time is attributed to the user application.
    buf->ru_utime.tv_sec  = now / MILLION;
    buf->ru_utime.tv_usec = now % MILLION;
    buf->ru_stime.tv_sec  = 0;
    buf->ru_stime.tv_usec = 0;
    buf->ru_maxrss        = 0;
    buf->ru_maxrss       += (text_size+1023)/1024;
    buf->ru_maxrss       += (maxAlloc_vaddr - heap_vaddr + 1023)/1024;
    buf->ru_maxrss       += ((heap_vaddr+heap_size-1) - processLeader_r1 + 1023)/1024;

    buf->ru_ixrss         = 0;
    buf->ru_idrss         = 0;
    buf->ru_isrss         = 0;
    buf->ru_minflt        = 0;
    buf->ru_majflt        = 0;
    buf->ru_nswap         = 0;
    buf->ru_inblock       = 0;
    buf->ru_oublock       = 0;
    buf->ru_msgsnd        = 0;
    buf->ru_msgrcv        = 0;
    buf->ru_nsignals      = pAppProc->signalCount;
    buf->ru_nvcsw         = 0;
    buf->ru_nivcsw        = 0;

    return CNK_RC_SUCCESS(0);
}
