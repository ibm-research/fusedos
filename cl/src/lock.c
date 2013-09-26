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

#include "Kernel.h"
//#include <sys/signal.h>

Lock_Atomic_t processleader_barrier;

// disable Kernel_Lock to "trap" its use at compile-time                                                               // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Kernel locking mechanism. 
// Note: currently implemented with ldarx/stdcx against a user supplied word in ddr. Could be reimplemented to not use ddr in the future
void Kernel_Lock(Lock_Atomic_t *pLock) 
{
    uint64_t my_unique_index = ProcessorID() + 1; 
    uint32_t freeValue = 0;
    // try to obtain the lock
    while (1)
    {
        if (compare_and_swap(pLock,             // lockword
                             &freeValue,        // value is zero when lock is not held
                             my_unique_index )) // store our thread_index+1 to indicate we own the lock
        {
            break; // break out of the the while loop. We now own the lock
        }
        else
        {
            // Spin at a low priority until lock is release and then try again
            ThreadPriority_Low();
            while (pLock->atom) { }
            ThreadPriority_High(); // Use high priority while we are holding the kernel lock
            freeValue = 0;
        }
    }
    isync();
}


int Kernel_Lock_WithTimeout(Lock_Atomic_t *pLock, uint64_t microseconds) 
{
    uint64_t my_unique_index = ProcessorID() + 1;
    uint64_t cyclesPerMicro = GetPersonality()->Kernel_Config.FreqMHz;
    uint64_t timeout_cycles = microseconds * cyclesPerMicro;
    uint64_t check_frequency = 1000;
    uint32_t freeValue = 0;
    uint64_t startTime = GetTimeBase();
    // try to obtain the lock
    while (1)
    {
        if (compare_and_swap(pLock,             // lockword
                             &freeValue,        // value is zero when lock is not held
                             my_unique_index )) // store our thread_index+1 to indicate we own the lock
        {
            break; // break out of the the while loop. We now own the lock
        }
        else
        {
            // Spin at a low priority until lock is released or timeout occurs
            ThreadPriority_Low();
            while (pLock->atom) {
                if (!check_frequency--)
                {
                    // Did we exceed the timeout value?
                    if ((GetTimeBase() - startTime) > timeout_cycles)
                    {
                        ThreadPriority_High(); // Use high priority while we are holding the kernel lock
                        return -1;
                    }
                    check_frequency = 1000; // Reload the check frequency
                }
            }
            ThreadPriority_High(); // Use high priority while we are holding the kernel lock
            freeValue = 0;
        }
    }
    isync();
    return 0;
}

void Kernel_Unlock(Lock_Atomic_t *pLock) 
{
    ppc_msync();
    // Consider checking that we own this lock before resetting it. This test may not be necessary if we can trust ourself!
    // reset the lock
    pLock->atom = 0;
    // Move our priority back down to normal. We were running with a high priority while holding the kernel lock
    ThreadPriority_Medium(); // Use high priority while we are holding the kernel lock

}

// Barrier implementation using a word in DDR
//   Parms:
//      
//      parm1: word to serve as the lock. Must be initialized to zero before
//             first use. Subsequent re-use  does not require initialization
//      parm2: number of hwthreads to rendezvous
// 
//      lockword layout: 
//          bits 0-15: - generation number; wraps harmlessly at 2^16
//          bits 16-31 - number of hwthreads that have arrived
void Kernel_Barrier_Internal(Lock_Atomic_t* pLock, uint32_t rendezvous_num)
{
    uint32_t tmp_val, generation;

    ppc_msync();

    // atomically increment the thread count
    do  /*loop doesn't iterate*/ 
    {
	tmp_val = LoadReserved32(&(ATOMIC_READ(pLock)));
	tmp_val = tmp_val + 1;
	generation = (tmp_val >> 16); // remember the current generation
	if ((tmp_val & 0xffff) == rendezvous_num)
	{
	    // The required number of threads have arrived.  Increment the
	    // generation to release all the threads (including this one),
	    // and clear the thread count for the next generation.
	    tmp_val = ((generation + 1) << 16);
	}
    } while (!StoreConditional32(&(pLock->atom), tmp_val));
    // Wait for the generation to change.
    while ((ATOMIC_READ(pLock) >> 16) == generation);
} 

// Kernel scoped barrier interface 
void Kernel_Barrier(BarrierScope_t scope)
{
    Lock_Atomic_t*pLock = NULL;
    uint32_t num = 0;
    uint64_t index;
    AppProcess_t *pProc;
    AppState_t *appState;
    CoreState_t *pCS;

    switch (scope)
    {
       case Barrier_HwthreadsInNode:
          pLock = &NodeState.nodeBarrier; // node scope barrier
          num = CONFIG_HWTHREADS_PER_CORE * NodeState.NumCoresEnabled; // default to all hwthreads
          break;
       case Barrier_HwthreadsInScheduler:
          pLock = &NodeState.nodeBarrier; // node scope barrier
          num = CONFIG_HWTHREADS_PER_CORE * NodeState.NumCoresEnabled; // default to all hwthreads
          num = MIN(CONFIG_HWTHREADS_PER_CORE * CONFIG_MAX_CORES - CONFIG_SCHED_NUM_NOSCHED_HWTHREADS, num);
          break;
       case Barrier_AppHwthreadsInNode: // restrict to the hardware threads that are used in applications
          pLock = &NodeState.nodeBarrier; // node scope barrier
          num = (CONFIG_MAX_APP_CORES < NodeState.NumCoresEnabled) ? CONFIG_MAX_APP_CORES : NodeState.NumCoresEnabled;
          num *= CONFIG_HWTHREADS_PER_CORE;
          break;
       case Barrier_HwthreadsInProcess:
          // select dword and number to rendezvous
          pProc = GetProcessByProcessorID(ProcessorID());
          pLock = &pProc->processBarrier; 
          num = pProc->HWThread_Count;
          break;
    case Barrier_HwthreadsInCore:
        // select dword and number to rendezvous
        pCS = GetCoreStateByCore( ProcessorCoreID() );
        pLock = &pCS->coreBarrier; 
        num = CONFIG_HWTHREADS_PER_CORE;
        break;
    case Barrier_ProcessLeadersInNode:
        // select dword and number to rendezvous
        pLock = &NodeState.nodeBarrier; // node scope barrier
        num = popcnt64(NodeState.ProcessLeaders);
        break;
    case Barrier_ProcessLeadersInApp:
        // select dword and number to rendezvous
        appState = GetMyAppState();
        index = appState->AppStateIndex;
        pLock = &(NodeState.appBarrier[index]);
        num = appState->Active_Processes;
        break;
    case Barrier_AppLeadersInNode:
        // select dword and number to rendezvous
        pLock = &NodeState.nodeBarrier; // node scope barrier
        num = popcnt64(NodeState.AppLeaders);
        break;
    case Barrier_HwthreadsInApp:
        // select dword and number to rendezvous
        appState = GetMyAppState();
        index = appState->AppStateIndex;
        pLock = &(NodeState.appBarrier[index]); // node scope barrier
        num = appState->Active_Cores*CONFIG_HWTHREADS_PER_CORE;
        break;
    case Barrier_AppLeaderAndAgents:
        // select dword and number to rendezvous
        pLock = &NodeState.agentAndAppLeaderBarrier;
        num = popcnt64(NodeState.AppAgents) + 1; // Agents plus the AppLeader
        break;
    case Barrier_AppAgents:
        // select dword and number to rendezvous
        pLock = &NodeState.agentBarrier;
        num = popcnt64(NodeState.AppAgents);     // Agents-only
        break;
    };
    // Call the barrier 
    assert(pLock && num);
    Kernel_Barrier_Internal(pLock, num);
}

// First barrier usesd in the kernel. Collects core enable info from personality. 
void Kernel_Barrier_First(int numCoresToBarrier)
{
    // Wait until all enabled cores arrive, then leave the barrier
    Kernel_Barrier_Internal(&NodeState.nodeBarrier,numCoresToBarrier);
}
#endif                                                                                                                 // FUSEDOS





