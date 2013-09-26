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
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "flih.h"
#include "JobControl.h"
#include "ToolControl.h"
#include "fs/filesetup.h"
#include "CoreDump.h"
#include "rank.h"
#include <hwi/include/bqc/wu_mmio.h>
#include <spi/include/mu/Reset.h>
#include <spi/include/mu/TermCheck.h>
#include <spi/include/mu/GIBarrier.h>
#include <hwi/include/bqc/MU_Addressing.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <hwi/include/bqc/testint_inlines.h>

Lock_Atomic_t mudm_atomic_lock;


Lock_Atomic_t pacing_lock;

// MUDM interface for performing broadcast of the count of abnormally exited processes 
struct mudm_rdma_bcast rdma_exit_bcast;
#endif                                                                                                                 // FUSEDOS

#include "fusedos_config.h"                                                                                                // FUSEDOS

	      
#if defined __FUSEDOS__                                                                                               // FUSEDOS
void App_ThreadExit( int status )                                                                                      // FUSEDOS
#else                                                                                                                  // FUSEDOS
void  __NORETURN App_ThreadExit( int status, KThread_t *kthread )
#endif                                                                                                                 // FUSEDOS
{
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    App_ClearMemory();                                                                                                 // FUSEDOS
#else                                                                                                                  // FUSEDOS

    // Make sure the Upci Swap Config is inactive
    kthread->pUpci_Cfg = NULL;

    // Cleanup watchpoints that may have been activated
    toolControl.cleanupThreadWatchpoints(kthread);

    // Is this kthread running on a hardware thread which is owned by some other process?
    AppProcess_t *hwtProc = GetProcessByProcessorID(ProcessorID());
    if (hwtProc != kthread->pAppProc )
    {
        // Set this kthread to be owned by the process associated with its current hardware thread
        kthread->pAppProc = hwtProc;
        // Restore the Physical PID so that the process exit flow uses the correct pid
        kthread->physical_pid = hwtProc->PhysicalPID;
        mtspr(SPRN_PID, hwtProc->PhysicalPID);
    }

    ppc_msync(); // Make sure all previous memory operations are complete before we set the state to free.

    // clean up and free the thread.
    // Reset all blocking indicators in the current kthread state
    Sched_Unblock(kthread, (kthread->State & ~SCHED_STATE_SUSPEND));
    // Set the RESET blocking indicator, needed to get thread counts right
    Sched_Block(kthread, SCHED_STATE_RESET);
    // Now free the kthread, preserving only the SUSPEND bit
    // We can free it with a simple assignment, but after that we can't touch it.
    kthread->State = (kthread->State & SCHED_STATE_SUSPEND) | SCHED_STATE_FREE;

    // Put the hwthread on the recycle list, making it a candidate to be searched
    // We put this on the list of the process that owns this hardware thread, 
    // which is not necessarily the process that was associated with this thread.
    Process_MakeHwThreadAvail(hwtProc, ProcessorID());

    // Enter the scheduler
    Scheduler(); 
    // This call does not return
#endif                                                                                                                 // FUSEDOS
}

#if defined __FUSEDOS__                                                                                               // FUSEDOS
extern void indicate_exit_to_threads();                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
void  App_ProcessExit( uint32_t status)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Run special processing for an app agent.
    if (IsAppAgent())
    {
        AppProcess_t *proc = GetMyProcess();
        proc->ExitStatus = status;
        App_AgentExit(AppExit_Phase1); // This call does not return
    }

    // Send IPI requests to all hardware threads in the Process associated with the exiting kthread
    int my_processorID = ProcessorID();
    AppProcess_t *pAppProcess = GetMyProcess();
    uint64_t hwthread_map = pAppProcess->HwthreadMask;
    int i;

    // Atomically attempt to reset the status of this Process. The thread that 
    // successfully resets the status will be the thread that is responsible for
    // signaling the other threads within the process
    uint32_t procStateLocal = pAppProcess->State;
    uint32_t *pState = (uint32_t*)(&pAppProcess->State);
    int signalThreads = 0;
    while (procStateLocal != ProcessState_ExitPending)
    {
        if (Compare_and_Swap32( pState, &procStateLocal, ProcessState_ExitPending ))
        {
            // We were sucessful in changing the state to ExitPending. We are now responsible for 
            // signaling the other hwthreads in this process
            signalThreads = 1;
            // Since we were the first thread to request an exit of this process, 
            // our exit status will represent the process
            pAppProcess->ExitStatus = status;
            // Exit the while loop
            break; 
        }
        else
        {
            // We were not sucessful in changing the status. If the status is still not
            // set to ExitPending, we need to try again. If someone else has changed the 
            // state to ExitPending, we are done.
            procStateLocal = pAppProcess->State; // re-read the state 
        }
    }
    // If we are responsible to notify the other threads, send the IPIs
    if (signalThreads)
    {
        for (i=0; i<64; i++)
        {
            if ((hwthread_map & _BN(i)) && (my_processorID != i))
            {
                // We found an index that we must send an IPI.
                IPI_process_exit(i, AppExit_Phase1);
            }
        }
        // We must force an exit of any pthread that may be running on remote hardware threads.
        if (pAppProcess->ThreadModel == CONFIG_THREAD_MODEL_ETA)
        {
            uint64_t thdmask = pAppProcess->HwthreadMask_Remote;
            for (i=0; i<64; i++)
            {
                if (thdmask & _BN(i))
                {
                    // We found an index that may contain a remote pthread. We must send an IPI.
                    IPI_remote_thread_exit(i, pAppProcess);
                }
            }
        }
    }
    // If this is a remote thread initiating the process termination, we just want to exit 
    // the thread. The App_Exit termination flow can only be entered on the hardware threads
    // that are owned by the process that is terminating.
    if (GetProcessByProcessorID(ProcessorID()) != GetMyProcess())
    {
        App_ThreadExit(0, GetMyKThread());
    }
    else
    {
        // Enter App_Exit. If we also have an IPI sent to us from another exit occurring on another
        // thread, this pending IPI condition will be cleaned up in the App_Exit processing.
        App_Exit(AppExit_Phase1, 1);
    }
#else                                                                                                                  // FUSEDOS
    indicate_exit_to_threads();                                                                                        // FUSEDOS
    App_Exit(AppExit_Phase1, 1);                                                                                       // FUSEDOS
#endif                                                                                                                 // FUSEDOS

}

// Called by App_ProcessExit and by the target of the Process Exit IPI operation initiated by App_Exit
void App_Exit(int phase, int noReturn)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int processorID = ProcessorID();
    AppState_t *pAppState = GetMyAppState();
    AppProcess_t *pProc = GetProcessByProcessorID(processorID);
    HWThreadState_t *hwt = GetMyHWThreadState();
    int isAppLeader = IsAppLeader(); // obtain this info now before we start resetting the data structures
    int isProcessLeader = IsProcessLeader(); // obtain this info now before we start resetting the data structures
    // Save info we need from our AppState. This is needed at the end of this function after it is reset.
    int hwThreadsInApp = pAppState->Active_Cores*CONFIG_HWTHREADS_PER_CORE;
    int nodesInJob = pAppState->jobLeaderData.NodesInJob;
    Lock_Atomic_t *pLock = &(NodeState.appBarrier[pAppState->AppStateIndex]); // node scope barrier

    uint32_t coredumponexit = 0;
    uint32_t coredumpdisabled = 0;
    uint32_t coredumponerror = 0;
    uint32_t coredumpmaxnodes = CONFIG_COREDUMPMAXNODES_DEFAULT;

    
    // Test to see what phase of exit we are in
    if (phase == AppExit_Phase1)
    {
        // ---------------------------
        // Phase 1 of Application Exit
        // ---------------------------

        // We may have re-enterred due to a latent AppExit IPI interrupt occurring after we already  
        // entered the scheduler in phase 1 of application exit. This can happen if more than one
        // thread is requesting that the process be terminated at approximately the same time.
        // If so, throw this request away.
        if (hwt->appExitPhase == AppExit_Phase1) 
        {
            Scheduler(); // Re-enter the scheduler
        }
        // Wait for all hardware threads in this process to begin termination
        Kernel_Barrier(Barrier_HwthreadsInProcess);

        if (isProcessLeader)
        {
            // Report the exiting of the process
            App_ReportProcessExit(pProc);

            // Flush the RAS buffers:
            NodeState.FW_Interface.flushRasBuffers();
        }
        // Wait for all hardware threads in this process to continue termination
        Kernel_Barrier(Barrier_HwthreadsInProcess);

        // See if we are configured to take a core dump on exit. If so do that here
        if (pProc->State !=  ProcessState_RankInactive)
        {
            App_GetEnvValue("BG_COREDUMPONEXIT", &coredumponexit);
            App_GetEnvValue("BG_COREDUMPONERROR", &coredumponerror);
            App_GetEnvValue("BG_COREDUMPDISABLED", &coredumpdisabled);
            App_GetEnvValue("BG_COREDUMPMAXNODES", &coredumpmaxnodes);

            if (((pProc->coreDumpOnExit) && (coredumpdisabled == 0) && ((pAppState->AbnormalTerminationSequenceNum <= coredumpmaxnodes) || (pProc->binaryCoredump == 1))) || 
                ((coredumponerror != 0) && ((EXITSTATUS_RCODE(pProc->ExitStatus)) != 0) && ((pAppState->AbnormalTerminationSequenceNum <= coredumpmaxnodes) || (pProc->binaryCoredump == 1))) || 
                (coredumponexit != 0) ||
                (pProc->coreDumpRank != 0)
               )
            {
                // If we have a failing kthread, we will call DumpCore from its hardware thread
                // so that we can capture the desired hardware state (TLBs). If no failing 
                // kthread is identified, the process leader hardware thread will be used.
                if ((!pProc->coredump_kthread && isProcessLeader) ||
                    (pProc->coredump_kthread && (pProc->coredump_kthread->pAppProc != GetProcessByProcessorID(processorID)) && isProcessLeader) || // remote thread failed?
                    (pProc->coredump_kthread && (pProc->coredump_kthread->ProcessorID == processorID)))
                {
                    // Obtain lock to reduce pounding the mudm job leader with the pacing requests from all procesess on this node
                    Kernel_Lock(&pacing_lock);
                    int pacingResult = App_DumpCorePacingBegin();
                    if (pacingResult >= 0)
                    {
                        DumpCore();
                        if (pacingResult > 0)
                        {
                            App_DumpCorePacingEnd();
                        }
                    }
                    Kernel_Unlock(&pacing_lock);

                }
            }
        }
        // Wait for all hardware threads in this process to continue termination
        Kernel_Barrier(Barrier_HwthreadsInProcess);
        if (isProcessLeader)
        {
            // Close any open file descriptors in this process.

            KThread_t *currentThread_original = hwt->pCurrentThread; 

            // Temporarily set current thread to the kernel thread. This will ensure that the File_ProcessCleanup() actions
            // are performed against the correct process. If we have interrupted a remote thread for another process, we 
            // do not want to be closing the file descriptors for that process.
            hwt->pCurrentThread = hwt->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]; 
            // Run the virtual close methods on open file descriptors.
            File_ProcessCleanup();
            // Restore the original current thread so as to not confuse the scheduler.
            hwt->pCurrentThread = currentThread_original;

            // Cleanup the private futex table for this process
            int i;
            for (i=0; i< NUM_FUTEX; i++)
            {
                pProc->Futex_Table_Private[i].futex_vaddr = NULL;
                pProc->Futex_Table_Private[i].pKThr_Waiter_Next = NULL;
            }

            // Turn off system call trace.
            if ((uint32_t)pAppState->RankForStrace == pProc->Rank)
            {
                pAppState->RankForStrace = bgcios::jobctl::DisableStrace;
                pProc->straceEnabled = false;
            }
        }
        // Atomically increment the count of threads reaching the end of AppExit phase 1.
        uint64_t prevCount = Fetch_and_Add(&pAppState->numThreadsAppExitPhase1, 1);

        // Test to see if we are the last hardware thread within this application to reach this point. 
        if ((prevCount+1) < pAppState->Active_Cores * CONFIG_HWTHREADS_PER_CORE)
        {
            // We are not the last hardware thread to arrive. Go into the scheduler so that we can 
            // accept IPI interrupts while we wait for all the processes to exit.

            // Set flag in hwthread state object to indicate that we have entered phase 1 of application
            // exit in this hardware thread. This is needed before we enable interrupts in the call to the
            // scheduler because we may have a latent AppExit(Phase1) IPI interrupt pending if this thread 
            // initiated a process exit around the same time that another thread initiated a process exit.
            hwt->appExitPhase = AppExit_Phase1;

            // First we must ensure that all kthreads belonging to the process that owns this hardware thread 
            // are blocked from running. We do not block any remote kthreads that may be running in another process.
            Sched_BlockForExit();

            // If the caller is allowing a return from this function to be done and if we
            // arrived here from within an application thread, force a save of the non-volatile state of the interrupted 
            // kthread before we enter the scheduler. This is needed for interrupted remote threads that may have been
            // running at the time of the IPI interrupt. 
            if (!noReturn && (GetMyKThread()->SlotIndex <  CONFIG_SCHED_KERNEL_SLOT_INDEX))
            {
                GetMyKThread()->Pending |= KTHR_PENDING_YIELD;
                return; // Note: caller expects that this AppExit() call may return in order to be redriven into the scheduler.
            }
            // Enter the scheduler
            Scheduler();
            // Will not reach here 
        }
        else
        {
            // We are the last hardware thread within this application to reach this point.
            // Send an IPI to all other hardware threads causing them to enter phase 2 of AppExit
            uint64_t hwthread_map = pAppState->AppThreadsMask;
            for (int i=0; i<64; i++)
            {
                if ((hwthread_map & _BN(i)) && (processorID != i))
                {
                    // We found an index that we must send an IPI.
                    IPI_process_exit(i, AppExit_Phase2);
                }
            }
            // Now lets start the termination of application agents
            if (NodeState.AppAgents)
            {
                // Signal any app agents that were started to make them exit.
                for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
                {
                    if (NodeState.AppAgents & _BN(i))
                    {
                        // Any App agent that has already exited will have entered phase 1 and
                        // be in the scheduler. If an app agent is still actively running,
                        // the following call will force it directly into phase 2 of the agent exit flow.
                        IPI_agent_exit(NodeState.FirstAppAgentProcessorID+i, AppExit_Phase2);
                    }
                }
                // Barrier with AppAgents
                Kernel_Barrier(Barrier_AppLeaderAndAgents);
            }
            // Proceed into phase 2 of application exit
        }
    }
    // ---------------------------
    // PHASE 2 of Application Exit
    // ---------------------------
    
    // Set flag indicating that we are entering the uninterruptable portion of application exit for this hardware thread.
    // This indicator is polled in the deliver IPI path since it is possible that an IPI is being driven from the 
    // tool/job control thread to a hardware thread which is no longer interruptable.
    GetMyHWThreadState()->appExitPhase = AppExit_Phase2;

    Kernel_Barrier(Barrier_HwthreadsInApp);
    Speculation_CleanupJob();                // Cleanup speculation state on each hardware thread
    Kernel_Barrier(Barrier_HwthreadsInApp);
    if (isAppLeader)
    {
        // Reset L2 speculation state
        Speculation_Init();
        
        PowerManagement_Shutdown();
        dumpFlightRecorder();
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);
    
    // Reinitialize the L1p unit on each core
    l1p_init();

    // Disable the IPI message checker
    IPI_disableMessageChecker();
    
    // Reinitialize the wakeup unit (including disabling the stack guard)
    wakeup_init();

    // Reinitialize the puea
    puea_init();

    // Cleanup the IPI communication area that this thread monitors. Each thread is responsible for its own area.
    // Also reset pending ipi interrupts from other application threads and application agents.
    ipi_cleanup();

    // Clear the process memory if the app was not preloaded. Don't bother wasting the cycles for a preloaded app.
    if (!Personality_ApplicationPreLoaded())
    {
        // Barrier to ensure all hardware threads in the App have reached this point continuing
        Kernel_Barrier(Barrier_HwthreadsInApp);
#endif                                                                                                                 // FUSEDOS
        App_ClearMemory();
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    }
    // Barrier to ensure all hardware threads in the App have reached this point continuing
    Kernel_Barrier(Barrier_HwthreadsInApp);

    if (ProcessorThreadID() == 0)
    {
        // Invalidate the icache. This must be done since the text segment was previously written with zeros. If we do 
        // not do this here, then the next job may execute the wrong instructions with this thread is launched.
        ici();
        isync();
        // Only uninstall TLBs if we installed them, otherwise we may attempt to use stale MMU data from the previous job if the static mapper was not yet run.
        if (pAppState->LoadState >= AppState_LoadComplete)
        {
            // Uninstall the TLBs in each core within the App
            vmm_uninstallStaticTLBMap(pProc->Tcoord);
        }
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);

    if(isAppLeader)
    {        
        // Save persistent data for the next job
        App_SavePersistent();

        // Cleanup file systems.
        File_JobCleanup();
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);
    if(isProcessLeader)
    {
        File_GetFSPtrFromType(FD_LOCAL)->cleanupJob(FD_LOCAL);
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);

    if(isAppLeader)
    {
        // Clear MMap file cache
        MMap_fileCacheClear();

        // Cleanup tools
        toolControl.detachAll(pAppState);
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);
    
    uint64_t interruptmap = BIC_ReadInterruptMap(ProcessorThreadID());
    BIC_WriteInterruptMap(ProcessorThreadID(), interruptmap & ~( _BN(37) | _BN(38) | _BN(39) | _BN(40) | _BN(44) ));
    Kernel_Barrier(Barrier_HwthreadsInApp);
    
    if(isAppLeader)
    {
        // Wait for all other nodes to reach this point before sending abnormal exit count.
	if ( ( NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_MU) != 0 ) 
	{
	    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
	}
        
        if (App_IsJobLeaderNode())
        {
            // Broadcast the abnormal exit info. Each node needs to know about any abnormal process termination.
            if (pAppState->jobLeaderData.NodesInJob > 1)
            {
                // Setup to perform the RDMA broadcast
                rdma_exit_bcast.status_mem = (uint64_t)(&pAppState->jobLeaderData.AbnormalExitCountBroadcastStatus);
                rdma_exit_bcast.status_val = 1;
                // Replicate the this object's data on all nodes so that they can continue processing. 
                rdma_exit_bcast.source_payload_paddr = (void*)&pAppState->jobLeaderData.AbnormalProcessExitCount;
                rdma_exit_bcast.payload_length =  sizeof(pAppState->jobLeaderData.AbnormalProcessExitCount); 
                rdma_exit_bcast.class_route = 14; // job-wide system class route.
                rdma_exit_bcast.dest_payload_paddr = (void*)&pAppState->jobLeaderData.AbnormalProcessExitCount;
                rdma_exit_bcast.num_in_class_route = pAppState->jobLeaderData.NodesInJob;
                rdma_exit_bcast.requestID = NULL;
                //Kernel_WriteFlightLog();
                int internalrc;
                do
                {
                    internalrc = mudm_bcast_RDMA_write(NodeState.MUDM,  &rdma_exit_bcast);
                } while(internalrc == -EBUSY);
                if (internalrc != -EINPROGRESS)
                {
                    printf("(E) mudm_bcast_RDMA_write in App_ReportJobExit() returned status %d\n", internalrc);
                }
                assert(internalrc == -EINPROGRESS);

                //printf("Broadcast abnormal process exit count %ld to all nodes\n",appState->jobLeaderData.AbnormalProcessExitCount );
            }
            else
            {
                // No broadcast is being done since this is the only node in the job.
                pAppState->jobLeaderData.AbnormalExitCountBroadcastStatus = 1; // Turn this on in our node since our count is valid.
            }
        }
        // Spin waiting for the mudm broadcast to complete. The status indicator will be set in all nodes when complete. 
        while (pAppState->jobLeaderData.AbnormalExitCountBroadcastStatus == 0)
        {
            ppc_msync();
        }

        uint32_t doCrcExchange = true;
        App_GetEnvValue("BG_CRC_EXCHANGE", &doCrcExchange);

        if((pAppState->jobLeaderData.AbnormalProcessExitCount) ||
           (pAppState->LoadState != AppState_StartIssued) ||
           pAppState->subBlockActive ||
           Personality_IsFPGA() || ((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_ND) == 0))
        {
            doCrcExchange = false;
        }

        if(doCrcExchange != 0)
        {
            int rc = 0;

            if ( ( rc = crcx_exchange_crcs(&systemJobGIBarrier) ) != 0 ) {
                //printf("(E) CRC Exchange failed. rc=%d\n",rc);
                pProc->ExitStatus = SIGJOBINTEGRITY;
            }
        }
        uint32_t doTermCheck = true;
        App_GetEnvValue("BG_TERMCHECK", &doTermCheck);

        if((pAppState->jobLeaderData.AbnormalProcessExitCount) ||
           (pAppState->LoadState != AppState_StartIssued) ||
           pAppState->subBlockActive ||
           Personality_IsFPGA() || ((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_ND) == 0))
        {
            doTermCheck = false;
        }

        if(doTermCheck != 0)
        {
            int rc = MUSPI_ND_TermCheck( &systemJobGIBarrier, 0 ); 

            if ( rc != 0 ) {
                //printf("(E) termination checks failed. rc=%d\n",rc);
                pProc->ExitStatus = SIGJOBINTEGRITY;
            }
        }
        uint32_t doMUReset = true;
        App_GetEnvValue("BG_MURESET", &doMUReset);

        if(IsSubNodeJob() ||
           Personality_IsFPGA() || 
           ((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_MU) == 0))
            doMUReset = false;

        if(doMUReset != 0)
        {
            int rc = 0;
            MUSPI_RESET_t mureset;
            MUSPI_InitReset(&mureset);
            do
            {            
                rc = MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                assert(rc == 0);
                rc = MUSPI_Reset(&mureset);
            }
            while(rc == EAGAIN);
            assert(rc == 0);
        }

        // If we did not pass the CRC exchange or the termination checks, we must send a process exit message
        if (pProc->ExitStatus == (uint32_t)SIGJOBINTEGRITY)
        {
            uint64_t prev_val = App_RegisterAbnormalProcessExit();
            if (prev_val == 0) // if zero, then we were the first process to encounter an abnormal termination
            {
                // Are we running in stand-alone mode
                if (!Personality_ApplicationPreLoaded())
                {
                    // We are the first to report this category of error
                    jobControl.exitProcess(pProc->app->JobID, pProc->Rank, pProc->ExitStatus, 0);
                }
            }
        }
        // Indicate that this node is ready for its final cleanup. Also determine if this node must send an ExitJob message. 
        int pendingExitJobMessage = App_ReportJobExit();
        int pendingMessageJobID = pAppState->JobID;
        int pendingMessageExitStatus = pAppState->jobLeaderData.JobExitStatus;

        // Initialize AppState and NodeState data areas controlled by this AppState.
        App_Reset();

        // -------------------------------------------------------------------------
        // WARNING:  Cannot access any AppState object structures after this point
        // -------------------------------------------------------------------------

        // Wait for all pending RAS issued by this node to be handled by the control system before proceeding
        while ( NodeState.FW_Interface.pollOutbox() );

        // Wait for all AppState objects across all nodes to be reset and ready for the next job.
        if((nodesInJob > 1) && !Personality_IsFPGA())
        {
            int rc = MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
            assert(rc== 0);
        }
        if (pendingExitJobMessage)
        {
            jobControl.exitJob(pendingMessageJobID, pendingMessageExitStatus);
        }
    }

    // The AppState structure is likely reset by the time a thread reaches here. We need to use
    // the internal version of our locking interface since we cannot access data in the AppState object. 
    Kernel_Barrier_Internal(pLock, hwThreadsInApp);
    BIC_WriteInterruptMap(ProcessorThreadID(), interruptmap );
    Kernel_Barrier_Internal(pLock, hwThreadsInApp);

    // Initialize the Scheduler and enter the scheduler
    Sched_Reset();
    Scheduler();
    // Control does not reach here
#endif                                                                                                                 // FUSEDOS
}

void App_ClearMemory()
{
    AppProcess_t *pProc = GetProcessByProcessorID(ProcessorID());
    // Each process leader is responsible to clear its application storage. The App leader will also clear the text
    // segment and the shared memory segment
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    if (1)                                                                                                             // FUSEDOS
#else                                                                                                                  // FUSEDOS
    if (IsProcessLeader())
#endif                                                                                                                 // FUSEDOS
    {
//      printf("data:%016lx-%016lx heap:%016lx-%016lx text:%016lx-%016lx shrd:%016lx-%016lx\n",
//             pProc->Data_VStart, pProc->Data_VEnd, pProc->Heap_VStart, pProc->Heap_VEnd,
//             pProc->Text_VStart, pProc->Text_VEnd, pProc->Shared_VStart, pProc->Shared_VEnd);

        // Clear the data segment
        if (pProc->Data_VStart)
        {
            TRACE(TRACE_VMM, ("App_ClearMemory: data %016lx-%016lx\n", pProc->Data_VStart, pProc->Data_VEnd));         // FUSEDOS
            memset((void*)pProc->Data_VStart, 0, (pProc->Data_VEnd + 1 - pProc->Data_VStart));
        }
        if (pProc->Heap_VStart)
        {
            TRACE(TRACE_VMM, ("App_ClearMemory: heap %016lx-%016lx\n", pProc->Heap_VStart, pProc->Heap_VEnd));         // FUSEDOS
            memset((void*)pProc->Heap_VStart, 0, (pProc->Heap_VEnd + 1 - pProc->Heap_VStart));
        }
        if (pProc->DYN_VStart)
        {
            TRACE(TRACE_VMM, ("App_ClearMemory: dyn %016lx-%016lx\n", pProc->DYN_VStart, pProc->DYN_VEnd));            // FUSEDOS
            memset((void*)pProc->DYN_VStart, 0, (pProc->DYN_VEnd + 1 - pProc->DYN_VStart));
        }
        // Clear the heap segment

#if defined __FUSEDOS__                                                                                               // FUSEDOS
        if (0)                                                                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
        if (IsAppLeader())
#endif                                                                                                                 // FUSEDOS
        {
            if (pProc->Text_VStart)
            {
                TRACE(TRACE_VMM, ("App_ClearMemory: text %016lx-%016lx\n", pProc->Text_VStart, pProc->Text_VEnd));     // FUSEDOS
                memset((void*)pProc->Text_VStart, 0, (pProc->Text_VEnd + 1 - pProc->Text_VStart));
            }
            if (NodeState.SharedMemory.VStart)
            {
                TRACE(TRACE_VMM, ("App_ClearMemory: text %016lx-%016lx\n", pProc->Shared_VStart, pProc->Shared_VEnd)); // FUSEDOS
                memset((void*)(NodeState.SharedMemory.VStart), 0, NodeState.SharedMemory.Size);
            }
        }
        //printf("Clear complete.\n");
    }
}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
void App_SavePersistent()
{
    if (NodeState.PersistentMemory.Size == 0)
    {
        return;
    }
    // If persistent memory was configured, move it to the high end of the physical address space, out
    // of the way of the temporary space (located in the lower GBs of physical space for apps and agents)
    uint64_t fromPage = (uint64_t)NodeState.PersistentMemory.PStart/VMM_PAGE_SIZE_1M;
    uint32_t numPages  = NodeState.PersistentMemory.Size/VMM_PAGE_SIZE_1M;
    uint64_t toPage  = (uint64_t)((NodeState.DomainDesc.ddrEnd + 1)/(1024*1024) - NodeState.PersistentMemory.Size/VMM_PAGE_SIZE_1M);
    uint64_t tempVFrom = 0x0000000400000000;
    uint64_t tempVTo   = 0x0000000500000000;
    // Move 1MB at a time from the current physical location to the end of physical memory. Start at the end
    // so that we do not have any destructive overlap during the copy
    if (toPage > fromPage) // Do we need to move the persistent data? Maybe we are already positioned at the end of physical memmory.
    {
        // Yes we need to move data. 

        for(int i=numPages-1; i >= 0; i--)
        {
             vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)((fromPage+i)*VMM_PAGE_SIZE_1M),
                          (void*)tempVFrom,
                          VMM_PAGE_SIZE_1M,0,0);
             vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)((toPage+i)*VMM_PAGE_SIZE_1M),
                          (void*)tempVTo,
                          VMM_PAGE_SIZE_1M,0,0);
             memcpy((void*)tempVTo,(void*)tempVFrom,VMM_PAGE_SIZE_1M);
             memset((void*)tempVFrom, 0, VMM_PAGE_SIZE_1M);
             
             vmm_UnmapUserSpace((void*)tempVFrom, VMM_PAGE_SIZE_1M, 0);
             vmm_UnmapUserSpace((void*)tempVTo,   VMM_PAGE_SIZE_1M, 0);
        }
    }
    // Modify the physical address value now that we have moved the persistent memory. We need to keep the Virtual 
    // address field set to the same value so that when the area is remapped for the next job, the same virtual 
    // address is used.
    NodeState.PersistentMemory.PStart = toPage * VMM_PAGE_SIZE_1M;
}

// Reset the AppState and AppProcess structures after a Job ends
void  App_Reset()
{
    AppState_t *appState = GetMyAppState();
    int i;
    uint64_t procLeaderMask = appState->ProcessLeaderMask;

    if (IsAppAgent())
    {
        AppProcess_t *proc = GetMyProcess();
        memset(proc, 0, sizeof(AppProcess_t));
    }
    else
    {
        // Loop through all AppProcess objects  that were controlled by this AppState and initialize them.
        for (i=0; procLeaderMask; i++)
        {
            uint64_t select_mask = _BN(i);
            if (select_mask & procLeaderMask)
            {
                AppProcess_t *proc = GetProcessByProcessorID(i);
                memset(proc, 0, sizeof(AppProcess_t));
                procLeaderMask &= ~select_mask;
            }
        }
    }
    // Get a fresh copy of the mask since it was modified by the above for loop.
    procLeaderMask = appState->ProcessLeaderMask;

    // Completely zero out the AppState object and rebuild the necessary fields
    memset(appState, 0, sizeof(AppState_t));

    // Initialize the indexes to the process objects
    for (i = 0; i < CONFIG_MAX_PROCESSES; i++)
    {
        appState->ProcStateIndex[i] = -1;
    }

    // Clean out the shared futex data areas within NodeState
    memset(NodeState.Futex_Table_Shared, 0, sizeof(NodeState.Futex_Table_Shared));

    // ensure all of the above initializations are seen by other threads before we reset the masks in NodeState
    ppc_msync();
    if (IsAppAgent()) // Test to see if we are resetting the AppState object of an application agent
    {
        // Clean out the NodeState flag corresponding to this agent
        NodeState.AppAgents &= ~(_BN(NodeState.FirstAppAgentProcessorID - ProcessorID()));
    }
    else
    {
        // Clean out the mask fields in the NodeState object. 
        NodeState.ProcessLeaders &= ~procLeaderMask;
        NodeState.AppLeaders     &= ~(_BN( cntlz64(procLeaderMask) ));
    }
}

// Called by App_ProcessExit and by the target of the Process Exit IPI operation initiated by App_Exit
void __NORETURN   App_AgentExit(int phase)
{
    AppProcess_t *pProc = GetProcessByProcessorID(ProcessorID());

    if (phase == AppExit_Phase1)
    {
        // NOTE: if an appagent does not explicitly exit prior to the application exiting, then this Phase 1 block will
        //       never be executed. Care must be taken to not include any resets in this section that must be executed
        //       regardless of whether the Agent has explicitly exited.
        
        // If this agent is exiting with an non-zero status we want to terminate the job and potentially take a core dump. 
        if (pProc->ExitStatus)
        {
            // Report the App Agent failure
            App_ReportProcessExit(pProc);

            uint32_t coredumponexit = 0;
            uint32_t coredumpdisabled = 0;
            uint32_t coredumponerror = 0;
            uint32_t coredumpmaxnodes = CONFIG_COREDUMPMAXNODES_DEFAULT;
            uint32_t coredumpagentenabled = 0;
            // See if we are configured to take a core dump on exit. If so do that here
            App_GetEnvValue("BG_COREDUMPONEXIT", &coredumponexit);
            App_GetEnvValue("BG_COREDUMPONERROR", &coredumponerror);
            App_GetEnvValue("BG_COREDUMPDISABLED", &coredumpdisabled);
            App_GetEnvValue("BG_COREDUMPMAXNODES", &coredumpmaxnodes);
            App_GetEnvValue("BG_COREDUMPAGENTENABLED", &coredumpagentenabled);
            if (((pProc->coreDumpOnExit) && (coredumpdisabled == 0) && 
                 (GetParentAppState()->AbnormalTerminationSequenceNum <= coredumpmaxnodes)) || 
                 (coredumponexit != 0) || 
                 ((coredumponerror != 0) && ((EXITSTATUS_SIGNAL(pProc->ExitStatus)) != 0) &&
                  (GetParentAppState()->AbnormalTerminationSequenceNum <= coredumpmaxnodes)))
            {
                // Note that we do not call the pacing algorithm for app agents since these are not written to the file system
                if (coredumpagentenabled)
                {
                    DumpCore();
                }
            }
        }
        // First we must ensure that all kthreads in this hardware thread are blocked from running
        Sched_BlockForExit();
        // Enter the scheduler
        Scheduler();
        // Will not reach here 
    }
    // --------------------
    // phase 2 Agent Exit
    // --------------------

    // Set flag indicating that IPI interrupts are now blocked
    GetMyHWThreadState()->appExitPhase = AppExit_Phase2;

    // Close any open file descriptors in this process.
    File_ProcessCleanup();

    // Reinitialize the puea.
    puea_init();

    // Initialize the wakeup unit for this agent thread.
    wakeup_cleanup_agent();

    // Clear process memory associated with this agent.
    App_ClearMemory();

    // Barrier with other App Agents before removing the TLBs on the core.
    Kernel_Barrier(Barrier_AppAgents);

    // Uninstall the TLBs if they were installed. We can use the Load state of the AppAgent's AppState object to determine this
    if (GetMyAppState()->LoadState >= AppState_LoadComplete)
    {
        // If we are one of two app agents, only uninstall the tlbs if we are on thread id 0. Otherwise call uninstall.
        if ((ProcessorThreadID() == 0) ||
            (popcnt64(NodeState.AppAgents) == 1))
        {
            // Invalidate the icache. This must be done since the text segment was previously cleared. 
            ici();
            isync();
            
            // Clear background scrub TLB location
            vmm_clearScrubSlot();
            
            // uninstall the TLBs for the agent
            vmm_uninstallStaticTLBMap(pProc->Tcoord);
        }
    }
    // Barrier to ensure that the AppAgents and AppLeader are synchronized before reset. 
    Kernel_Barrier(Barrier_AppLeaderAndAgents);

    // Reset AppState and AppProcess structures.
    App_Reset();

    // All cleaned up and ready to accept the next job request
    // Initialize the Scheduler and enter the scheduler
    Sched_Reset();
    Scheduler();
    // Control does not reach here
}

void App_ReportProcessExit(AppProcess_t *proc)
{
    if (proc->ExitStatus)
    {
        uint32_t exit_rc = EXITSTATUS_RCODE(proc->ExitStatus);
        uint32_t exit_signal = EXITSTATUS_SIGNAL(proc->ExitStatus);
        // If exiting due to signal, bump the abnormal process exit counter in the job leader node
        if (exit_signal || (exit_rc == 1))
        {

            AppState_t *app = GetParentAppState();
            // If at least one process has already reported an abnormal process exit from this node, we do not need to do it again.
            volatile uint32_t *lockword = &app->processExitReported;
            uint32_t zeroValue = 0;
            if(Compare_and_Swap32(lockword, &zeroValue, 1))
            {
                uint64_t prev_val = App_RegisterAbnormalProcessExit();
                // Store the sequence number into the AppState structure so all processes can see it.
                app->AbnormalTerminationSequenceNum =  prev_val + 1;
                //printf("Rank: %d termination sequence num: %d\n", GetMyProcess()->Rank,app->AbnormalTerminationSequenceNum);
                 
                if (prev_val == 0) // if zero, then we were the first process to encounter an abnormal termination
                {
                    // Are we running in stand-alone mode
                    if (!Personality_ApplicationPreLoaded())
                    {
                        // We are the first to report this category of error
                        // Is this an application agent?
                        uint32_t agentid = AppAgentIdentifier(proc);
                        uint32_t rank;
                        if (agentid)
                        {
                            // Find the process object for the application leader on this node.
                            AppProcess_t *appLeaderProcess= GetFirstProcess(((AppState_t*)proc->app->parentAppState));
                            rank = appLeaderProcess->Rank;
                        }
                        else
                        {
                            rank = proc->Rank;
                        }
                        jobControl.exitProcess(proc->app->JobID, proc->Rank, proc->ExitStatus, agentid);
                    }
                }

            }
            else
            {
                // We were not the first. We need to wait until the first abnormal terminator gets a sequence number
                while (!app->AbnormalTerminationSequenceNum)
                {
                    Delay(100);
                }
            }
        }
        // The following code will maintain an application/node scoped ExitStatus. An   
        // exit status of zero indicates that all processes in the job on this node
        // have exited normally.
        uint64_t localAppStatus;
        do
        {
            localAppStatus = LoadReserved32(&proc->app->ExitStatus);
            if (proc->ExitStatus > localAppStatus)
            {
                // Set this exit code as the new combined exit code.
                localAppStatus = proc->ExitStatus;

            }
            else
                break;
        } while (!StoreConditional32( &proc->app->ExitStatus, localAppStatus));
    }
    // Notify attached tools if required. Tools do not know about app agents, so bypass sending the message if this is an AppAgent
    if (!AppAgentIdentifier(proc))
    {
        toolControl.exitProcess(proc);
    }
}


int App_ReportJobExit()
{
    AppState_t *appState = GetMyAppState();
    int rc = 0;

    // through the process Job leader node similar to the Exit Job message
    if (appState->LoadState == AppState_LoadFailed)
    {
        if (Personality_ApplicationPreLoaded())
        {
            printf("(E) Application load failure. Job %ld Load error code %d\n", appState->JobID, appState->LoadStateErrorCode);
            
            RASBEGIN(3);
            RASPUSH(appState->JobID);
            RASPUSH(appState->LoadStateErrorCode);
            RASPUSH(appState->ExitStatus);
            RASFINAL(RAS_KERNELPRELOADFAIL);
            
            Kernel_Crash(RAS_KERNELPRELOADFAIL);
        }
        else
        {
            // We didn't get through the Application Load within CNK. The Application was never launched.
            // Send message indicating that an Application Load Error has occurred
            jobControl.loadJobAck(appState->JobID, appState->LoadStateReturnCode, appState->LoadStateErrorCode);
            return 0;
        }
    }
    // Store the exit status in the job leader node if this is larger than the exit status already stored. Also
    // decrements the counter in the job leader so that the job leader knows when all nodes have reported.
    App_RegisterNodeAvailable(); 

    // If we are the job leader node, poll waiting for all nodes in the job to become available.
    // Once all nodes are available, we will signal the control system (or terminate the kernel if we 
    // are running in stand-alone mode).
    if (App_IsJobLeaderNode())
    {
        while (appState->jobLeaderData.RdmaCounterForExit != 0)
        {
            // spin waiting for all nodes in the job to check in.
            ppc_msync();
        }

        // Are we running in stand-alone mode
        if (Personality_ApplicationPreLoaded())
        {
            // Stand-alone mode. Kernel needs to terminate.
            if (!(EXITSTATUS_SIGNAL(appState->ExitStatus)))
            {
                printf("(I) Application exiting normally. Job %ld Exit status %d\n", appState->JobID, EXITSTATUS_RCODE(appState->ExitStatus));
                Kernel_Exit(EXITSTATUS_RCODE(appState->ExitStatus)); 
            }
            else
            {
                printf("(E) Application exiting due to signal. Job %ld signal %d\n", appState->JobID, EXITSTATUS_SIGNAL(appState->ExitStatus));
                Kernel_Exit(EXITSTATUS_SIGNAL(appState->ExitStatus)); 
            }
        }
        else
        {
            if (appState->LoadState == AppState_StartIssued)
            {
                rc = 1; // Return indication that indicates that an exitJob message is to be sent to the control system by this node.
            }
            else
            {
                // Since a start was not issued, we are not at the state where the control system expects to see an exitjob message. Since
                // we did not report a load error, it was likely cios that reported a load error to the control system, causing the 
                // cleanupJob message to get us here.
            }
        }
    }
    return rc;
}

int App_IsJobLeaderNode()
{
    AppState_t *app = GetParentAppState();
    Personality_t *pers = GetPersonality();
    if ((app->jobLeaderData.NodesInJob < 2) || // If 0 or 1 nodes are in the job, then we are the Job leader node.
        ((pers->Network_Config.Acoord == app->jobLeaderCoord.aCoord) &&
         (pers->Network_Config.Bcoord == app->jobLeaderCoord.bCoord) &&
         (pers->Network_Config.Ccoord == app->jobLeaderCoord.cCoord) &&
         (pers->Network_Config.Dcoord == app->jobLeaderCoord.dCoord) &&
         (pers->Network_Config.Ecoord == app->jobLeaderCoord.eCoord)))
    {
        return 1;
    }
    return 0;  
}

int App_IsLoadLeaderNode()
{
    AppState_t *app = GetParentAppState();
    return app->IsLoadLeader;
}

uint64_t App_RegisterAbnormalProcessExit()
{
    AppState_t *app = GetParentAppState();
    uint64_t *pcounter = (uint64_t*)&app->jobLeaderData.AbnormalProcessExitCount;;
    uint64_t previous_value = 0;

    if (app->jobLeaderData.NodesInJob > 1)
    {
        Kernel_Lock(&mudm_atomic_lock);

        // use MUDM to update the counter on the job leader node
        NodeState.remoteget.atomic_op = MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT;
        NodeState.remoteget.paddr_here = (uint64_t)(&NodeState.remoteget); // address of this structure
        NodeState.remoteget.remote_paddr = (uint64_t)(&app->jobLeaderData.AbnormalProcessExitCount); // address of the counter to be bumped
        NodeState.remoteget.returned_val = 0; // initialize the return value
        NodeState.remoteget.local_counter = 8; // this is an 8 byte operation
        mudm_remoteget_load_atomic(NodeState.MUDM,  &NodeState.remoteget);        
        while (NodeState.remoteget.local_counter)
        {
            ppc_msync();
        }
        previous_value = NodeState.remoteget.returned_val;
        
        Kernel_Unlock(&mudm_atomic_lock);
    }
    else // Size if job is 0 nodes (i.e subnode job) or 1 node. No need to use MUDM
    {
        previous_value = Fetch_and_Add(pcounter, 1);
    }
    return previous_value;
}


// Stores the exit status if we are the max value and decrements the counter. When all nodes in the job have checked in,
// the counter will reach zero.
void App_RegisterNodeAvailable()
{
    AppState_t *app = GetMyAppState();

    if (app->jobLeaderData.NodesInJob > 1)
    {
        // use MUDM RDMA to set the max exit status and also decrement the remote NodesActive counter
        NodeState.directput.source_val = (uint64_t)app->ExitStatus;      // local source value for atomic operation
        NodeState.directput.remote_counter_physAddr = (uint64_t)&app->jobLeaderData.RdmaCounterForExit;  // physical address of the remote counter
        NodeState.directput.remote_paddr = (uint64_t)&app->jobLeaderData.JobExitStatus;    // remote physical address on remote 
        NodeState.directput.paddr_here = (uint64_t)&NodeState.directput;      //starting physical address of this struct
        NodeState.directput.atomic_op = MUHWI_ATOMIC_OPCODE_STORE_MAX_UNSIGNED;  //atomic operation values from MU_Addressing.h in hwi/include/bqc/
        mudm_directput_store_atomic(NodeState.MUDM, &NodeState.directput); 
    }
    else // This is a 1 node job or a sub-node job
    {
        // Set the ActiveCount to zero since this is the only node in the job (this is already zero for a sub-node job)
        app->jobLeaderData.RdmaCounterForExit = 0;
        // Set the app's exit status into the job leader structure (one appState object for the entire job)
        app->jobLeaderData.JobExitStatus = app->ExitStatus;
    }
}

int App_CleanupJob(bgcios::jobctl::CleanupJobMessage *inMsg)
{
    AppState_t *appState = App_GetAppFromJobid(inMsg->header.jobId);
    if (appState->LoadState == AppState_LoadComplete)
    {
        // An error has been detected by CIOS during the load operation. We completed the load, however we are getting a cleanup
        // message prior to getting the Start message. Force the threads through the exit processing so that we can prepare for the next job.
        // We are not reporting an error here since the occurrence of the cleanupJob message implies that an error has already been 
        // indicated to the control system by CIOS.
        uint64_t hwthreads = appState->AppThreadsMask;
        for (int i=0; i<64; i++)
        {
            if (hwthreads & _BN(i))
            {
                IPI_process_exit(i, AppExit_Phase1);
            }
        }
    }
    return 0;
}

int App_DumpCorePacingBegin()
{
    AppState_t *app = GetParentAppState();
    uint64_t previous_value = 0x8000000000000000;
    uint64_t cyclesPerMicro = GetPersonality()->Kernel_Config.FreqMHz;
    uint64_t maxCoredumpTime = cyclesPerMicro * CONFIG_COREDUMP_WINDOW * 1000000ULL;
    uint64_t numNodesInJob = app->jobLeaderData.NodesInJob;
    uint64_t delaytime = cyclesPerMicro * (1000 + numNodesInJob/128);  // scaled milliseconds in processor cycles
    //uint64_t delaytime = cyclesPerMicro * 1000;  // 1 milliseconds in processor cycles

    // Is pacing enabled
    if ((app->jobLeaderData.corepacesem[1] == 0) || (app->jobLeaderData.NodesInJob <= 1))
    {
        return 0; // Indicate that no pacing is active and to proceed with the core file generation.
    }
    do
    {
        uint64_t basetime = app->jobControlSIGKILLstart;
        if (basetime && ((GetTimeBase() - basetime) > maxCoredumpTime))
        {
            //printf("Timeout after %d seconds on rank %d\n", CONFIG_COREDUMP_WINDOW, GetMyProcess()->Rank);
            return -1; // Indicate that no pacing is active and to NOT proceed with the core file generation.
        }
        Kernel_Lock(&mudm_atomic_lock);
        // use MUDM to update the counter on the job leader node
        NodeState.remoteget.atomic_op = MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED;
        NodeState.remoteget.paddr_here = (uint64_t)(&NodeState.remoteget); // address of this structure
        NodeState.remoteget.remote_paddr = (uint64_t)(&app->jobLeaderData.corepacesem[1]); // address of the counter
        NodeState.remoteget.returned_val = 0; // initialize the return value
        NodeState.remoteget.local_counter = 8; // this is an 8 byte operation
        uint64_t timeit = GetTimeBase();
        mudm_remoteget_load_atomic(NodeState.MUDM,  &NodeState.remoteget);        
        while (NodeState.remoteget.local_counter)
        {
            ppc_msync();
        }
        if ((GetTimeBase() - timeit) > delaytime)
        {
            // Response time for our mu atomic operation was very slow. Do not pound the interface as hard.
            delaytime = delaytime * 2;
        }
        previous_value = NodeState.remoteget.returned_val;
        Kernel_Unlock(&mudm_atomic_lock);
        if (previous_value == 0x8000000000000000)
        {
            Delay(delaytime); 
        }

    } while (previous_value == 0x8000000000000000);
    return 1; // Indicate that pacing is active which requires a call to App_DumpCorePacingEnd() after the core file is generation is completed.
}

int App_DumpCorePacingEnd()
{
    AppState_t *app = GetParentAppState();
    Kernel_Lock(&mudm_atomic_lock);
    // use MUDM to update the counter on the job leader node
    NodeState.remoteget.atomic_op = MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT;
    NodeState.remoteget.paddr_here = (uint64_t)(&NodeState.remoteget); // address of this structure
    NodeState.remoteget.remote_paddr = (uint64_t)(&app->jobLeaderData.corepacesem[1]); // address of the counter
    NodeState.remoteget.returned_val = 0; // initialize the return value
    NodeState.remoteget.local_counter = 8; // this is an 8 byte operation
    mudm_remoteget_load_atomic(NodeState.MUDM,  &NodeState.remoteget);        
    while (NodeState.remoteget.local_counter)
    {
        ppc_msync();
    }
    Kernel_Unlock(&mudm_atomic_lock);
    return 0;
}

// Called when the process is exiting and the Extended Thread Affinity facility is enabled. 
void App_RemoteThreadExit(AppProcess_t *proc)
{
    // Loop through all of the kthreads on this hardware thread and if a matching process is found,
    // force this kthread into exit.

    // This should only be called when the passed in process is not the home process for this hardware thread
    assert(proc != GetProcessByProcessorID(ProcessorID()));
    // Suspend the threads on our hardware thread
    HWThreadState_t *hwt = GetMyHWThreadState();
   // Exit threads on our hardware thread that match the passed in proc.
    for (int i = 0; i< CONFIG_AFFIN_SLOTS_PER_HWTHREAD; i++)
    {
        KThread_t *kthread = hwt->SchedSlot[i];
        if (kthread->pAppProc == proc)
        {
            // Force exit the remote thread.
            //printf("Forcing exit of remote tid:%d. Current kthread state:%d\n", GetTID(kthread), kthread->State);
            App_ThreadExit(0, kthread);
        }
    }
}
	      

#endif                                                                                                                 // FUSEDOS
