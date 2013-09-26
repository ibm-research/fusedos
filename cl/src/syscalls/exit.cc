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

#include <hwi/include/common/compiler_support.h>

#include "Kernel.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#if defined(__FUSEDOS__)                                                                                               // FUSEDOS
#include <linux/futex.h>                                                                                                // FUSEDOS
#include <sys/syscall.h>                                                                                                // FUSEDOS
#include <unistd.h>                                                                                                     // FUSEDOS
//#include <sys/time.h>                                                                                                 // FUSEDOS
#endif                                                                                                                  // FUSEDOS

uint64_t sc_exit( SYSCALL_FCN_ARGS )
{
    KThread_t *pKThr = GetMyKThread();
    int status = (int)r3;
    Futex_t *clr_tid = (Futex_t *)pKThr->pChild_TID;

    TRACESYSCALL(("(I) %s[%2d:%d]: status = %d. pParent_TID=0x%p, pChild_TID=0x%p.\n",
                  __func__, ProcessorCoreID(), ProcessorThreadID(), status, pKThr->pParent_TID, pKThr->pChild_TID ));

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    KThread_t *kthread = GetMyKThread();

    // Reset the guard page info. This must be done prior to the futex wake, otherwise the library may use the old stack/guard area for a
    // new pthread on a different hardware thread in the same core. This would result in the clone system call encountering a WAC
    // exception since this hardware thread's WAC will still be set. (All hwthreads on the core get interrupted whenever any of the 4
    // guard WAC registers are matched).
    kthread->GuardBaseAddress = -1;
    kthread->GuardEnableMask  = -1;
    Guard_Reset();
#endif                                                                                                                 // FUSEDOS


    if ( VMM_IsAppAddress( (const void*)clr_tid, sizeof(uint32_t) ) )
    {
        *clr_tid = (Futex_t)0;
        ppc_msync();
#if defined(__FUSEDOS__)                                                                                              // FUSEDOS
		syscall( SYS_futex, pKThr->pChild_TID, FUTEX_WAKE, INT_MAX, NULL, NULL, 0 );                                   // FUSEDOS
#else                                                                                                                  // FUSEDOS
        Futex_Wake( 1 /*FUTEX_WAKE */, (Futex_t *)pKThr->pChild_TID, INT_MAX, 0, 0, 0 );
#endif                                                                                                                 // FUSEDOS
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    App_ThreadExit( status, kthread ); // see AppCleanup.c
#else                                                                                                                  // FUSEDOS
    return 0;                                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

uint64_t sc_exit_group( SYSCALL_FCN_ARGS )
{
    // KThread_t *pKThr = GetMyKThread();
    int status = (int)r3;

    TRACESYSCALL(("(I) %s[%2d:%d]: status = %d.\n",
                  __func__, ProcessorCoreID(), ProcessorThreadID(), status ));
    
    App_ProcessExit( ((status & 0xFF) << 8) ); // exit value stored in byte 2 of status.
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    return(status);                                                                                                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
    return CNK_RC_SUCCESS(0); // Will not reach here.
#endif                                                                                                                 // FUSEDOS
}
