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

#ifndef _LOCK_H_ // Prevent multiple inclusion.
#define _LOCK_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__)
#if !defined(__ASSEMBLY__)


#include <hwi/include/common/bgq_alignment.h>
#include "atomicops.h"

extern NodeState_t NodeState; 

typedef enum
{
    Barrier_HwthreadsInNode = 1, // Barrier for all hardware threads in the node
    Barrier_AppHwthreadsInNode,  // Barrier for the hardware threads which are contained in processes
    Barrier_HwthreadsInCore,     // Barrier for the hardware threads which are contained in the current core
    Barrier_HwthreadsInProcess,  // Barrier for the hardware threads within the current process
    Barrier_ProcessLeadersInNode,// Barrier for the Process Leaders within the node 
    Barrier_ProcessLeadersInApp, // Barrier for the Process Leaders within the current App
    Barrier_AppLeadersInNode,    // Barrier for the Application leaders within the node
    Barrier_HwthreadsInScheduler,// Barrier for the hardware threads that use the scheduler
    Barrier_HwthreadsInApp,      // Barrier for the hardware threads within the current App
    Barrier_AppLeaderAndAgents,  // Barrier for the AppLeader and all agents associated with the AppLeader  
    Barrier_AppAgents            // Barrier for the application agents
} BarrierScope_t;
// Barrier to be used within the kernel
void Kernel_Barrier(BarrierScope_t);

// disable Kernel_Lock to "trap" its use at compile-time                                                               // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Exclusive lock. When obtained by a hwthread, other hwthreads will spin until Kernel_Unlock is called
// by the hwthread that obtained the lock.
//  parm1: doubleword initialized to zero on first use.
void Kernel_Lock(Lock_Atomic_t *pLock);

// Unlock. Should only be issued after a successful Kernel_Lock operation
//  parm1: doubleword used in a previous lock operation
void Kernel_Unlock(Lock_Atomic_t *pLock);

// This barrier function is meant to be used by the kernel after being launched 
// by the firmware. The caller will pass the number of hwthreads that are 
// participating in the barrier.
void Kernel_Barrier_First(int numThreadsToBarrier);

// Should only be used in special situations when the above enumerated lock types are not appropriate.
void Kernel_Barrier_Internal(Lock_Atomic_t*, uint32_t);

// Exclusive lock. When obtained by a hwthread, other hwthreads will spin until Kernel_Unlock is called
// by the hwthread that obtained the lock.
//  parm1: doubleword initialized to zero on first use.
//  parm2: time in microseconds until the lock request times out.
//  returns: 0:lock has been obtained. (-1):timeout has occurrred and lock has not been obtained 
int Kernel_Lock_WithTimeout(Lock_Atomic_t *pLock, uint64_t microseconds);

#endif                                                                                                                 // FUSEDOS

#endif // not __ASSEMBLY__
#endif // __KERNEL__ 

#endif // Add nothing below this line.

