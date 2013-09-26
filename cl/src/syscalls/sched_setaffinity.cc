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

// Includes
#include "Kernel.h"
#include <sched.h>

#if defined __FUSEDOS__                                                                          // FUSEDOS
#include "../cl_debug.h"                                                                          // FUSEDOS
#else                                                                                             // FUSEDOS
Lock_Atomic_t runtimeMmuTableUpdate; 
#endif                                                                                            // FUSEDOS

uint64_t sc_sched_setaffinity(SYSCALL_FCN_ARGS)
{
    int  tid   = (int)r3;
    size_t cpusetsize = (size_t)r4;
    cpu_set_t *cpu_mask = (cpu_set_t*)r5;
    
    // Get the Kthread associated with this tid. Function will return a NULL if this is an invalid tid
    // printf("set_affinity tid from app: %d\n",pid_or_tid);
#if !defined(__FUSEDOS__)                                                                        // FUSEDOS
    KThread_t* targetKThread = GetKThreadFromTid(tid);
    AppProcess_t *proc = GetMyProcess();

    // Did we find a valid KThread
    if (!targetKThread)
    {
        return CNK_RC_FAILURE(ESRCH); // no corresponding tid in this process was found
    }
    // is this kthread a process leader?
    if  (targetKThread->isProcessLeader)
    {
        //printf("setaffinity EINVAL due to target set to the process leader thread id\n");
        return CNK_RC_FAILURE(EINVAL); // A process leader thread is not a valid target
    }
#endif                                                                                            // FUSEDOS    
    
    // Determine the requested target hardware thread
    uint32_t hwthread_index;
    for (hwthread_index=0; hwthread_index < CONFIG_MAX_APP_THREADS; hwthread_index++)
    {
        if (CPU_ISSET_S(hwthread_index, cpusetsize, cpu_mask)) break;
    }
#if !defined(__FUSEDOS__)                                                                        // FUSEDOS
    // Did we find a valid bit on in the cpu mask?
    if (hwthread_index >= CONFIG_MAX_APP_THREADS)
    {
        //printf("setaffinity EINVAL due to request to set hwthread %d greater than mask size.\n",hwthread_index);
        return CNK_RC_FAILURE(EINVAL); // No cpu bit was set in the mask
    }
    // Determine if the target hardware thread index is a resource owned by this process. 
    bool remoteMigrate = 0;
    if (!((_BN(hwthread_index)) & (GetMyProcess()->HwthreadMask)))
    {
        // The targeted hardware thread is not a thread belonging to this process.
        // If the extended affinity model is activated, allow this
        if (proc->ThreadModel != CONFIG_THREAD_MODEL_ETA)
        {
            //printf("setaffinity EINVAL due to request to set hwthread %d not belonging to this process\n",hwthread_index);
            return CNK_RC_FAILURE(EINVAL); // No cpu bit was set in the mask
        }
        else
        {
            remoteMigrate = 1;
        }
    }
    // Is the target kthread already assigned to the target hardware thread?
    if ((uint32_t)(targetKThread->ProcessorID) == hwthread_index)
    {
        // Unfortunately we need to do some un-natural things here to deal with comm threads.
        if (proc->HWThread_Count == 2)
        {
            // Reinitialize the layout counter at its starting position so that a user pthread can be placed
            // on top of a comm thread before it is placed on the main thread.
            proc->HwtRecycleListMgr.threadLayout_count = 1;
        }
       // Nothing more to do.
        return CNK_RC_SUCCESS(0);
    }
    // See if we can reserve a kthread on the target hardware thread. If we can, there should be nothing stopping us from proceeding with the migration
    int reserved_slot = KThread_ReserveForMigration(hwthread_index);
    // if no slot was available on the target hardware thread, fail the request.
    if (reserved_slot < 0)
    {
        return CNK_RC_FAILURE(EINVAL); 
    }
    // Set the pending migration information in the kthread (target hwt and slot) (compare swap, if fails assume some other thread is
    // moving this kthread.
    if (KThread_SetMigrationTarget(targetKThread, hwthread_index, reserved_slot) < 0)
    {
        KThread_ReleaseForMigration(hwthread_index,reserved_slot);
        return CNK_RC_FAILURE(EINVAL); 
    }
    // If this is an Extended Affinity model move to a hardware thread outside of this process's ownership, see if we can generate 
    // generate the proper TLBs for this process space on the target core.
    uint32_t migration_pid = proc->PhysicalPID;
    if (remoteMigrate)
    {
        // If we are migrating within the hardware threads of a core and the configuration is 32 or 64 processes per node,
        // then our TLBs are already setup in this core and no additional work is needed to allow the migration to proceed.
        if ((proc->HWThread_Count > 2) ||
            (targetKThread->ProcessorID >> 2) != ((int32_t)hwthread_index >> 2))
        {
            // Call mapper function to prepare the TLBs for application. If the tlbs cannot be mapped for any
            // reason, an error will be returned. Also this function call will reserve the target core's mapping
            // so that no other request comes in before we eventually write the TLBs in the target thread.
            uint32_t destpid;  // new hardware pid to be used in the target hardware thread to represent this thread's process.
            int source_process = proc->Tcoord;
            int dest_process = (GetProcessByProcessorID(hwthread_index))->Tcoord;

            Kernel_Lock(&runtimeMmuTableUpdate);
            int copyProcessResult = vmm_copyProcessToOtherMMU(source_process, dest_process, &destpid);
            Kernel_Unlock(&runtimeMmuTableUpdate);
            if(copyProcessResult != 0)
            {
                //printf("vmm_copyProcesstoOtherMMU failed!\n");
                KThread_ReleaseForMigration(hwthread_index,reserved_slot);
                return CNK_RC_FAILURE(EINVAL);
            }
            //printf("vmm_copyProcesstoOtherMMU: source T:%d dest T:%d new dest Physical PID:%d. My Heap start/end=%016lx/%016lx\n", source_process, dest_process, destpid, proc->Heap_VStart, proc->Heap_VEnd );
            // Modify the default migration pid value 
            migration_pid = destpid; 
        }
        //printf("(I) Remote thread migration requested by %d from %d to %d slot %d\n", ProcessorID(), targetKThread->ProcessorID, hwthread_index, reserved_slot);
    }
    // There is no turning back now. Request that the thread be moved.

    // Set the physical pid to be used in the migrated thread.  This cannot be changed until we are ready to be dispatched on the new hwt
    //printf("sched_setaffinity: Setting migration pid to %d for hwt:%d tid:%d\n", migration_pid, hwthread_index, GetTID(targetKThread));
    targetKThread->MigrationData.b.physical_pid = migration_pid;

    //    Send the IPI to the target kthread to initiate the move
    int targetProcessorID = targetKThread->ProcessorID;
    if (ProcessorID() == targetProcessorID)
    {
        KThread_MoveToHwThread(targetKThread);
    }
    else
    {
        IPI_setaffinity(targetKThread);
    }
#else                                                                                             // FUSEDOS
	(void)cpusetsize;                                                                             // FUSEDOS
	CL_DEBUG("ignoring sched_setaffinity for tid %d, hw thread idx %d\n", tid, hwthread_index);   // FUSEDOS
#endif                                                                                            // FUSEDOS

    return CNK_RC_SUCCESS(0);
}

