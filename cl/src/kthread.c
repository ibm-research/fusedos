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
#include "tool.h"

// Modify the policy and priority of a KThread. 

int KThread_ChangePolicy(KThread_t*pKThread, int newPolicy, int newPriority)
{
    int rc = 0;
    // Modify the policy of the target kthread
    // If we are not yet running on the hardware thread of the target, move there.
    if (GetMyHWThreadState() != pKThread->pHWThread)
    {
        // Chase kthread to its current hardware thread
        IPI_changepolicy(pKThread, newPolicy, newPriority);
    }
    else
    {
        // If this is a transition to a comm thread, cache away the kthread pointer in the hardware thread state object
        if (newPolicy == SCHED_COMM)
            GetMyHWThreadState()->pCommThread = pKThread;
        pKThread->Policy = newPolicy;
        // Change priority
        rc = KThread_ChangePriority(pKThread,newPriority);
    }
    return rc;
}

// Modify the priority of a KThread, including the adjustment of the SchedOrder structure within the HwThreadState object 
// associated with the KThread.
int KThread_ChangePriority(KThread_t*pKThread, int newPriority)
{
    int rc = 0;
    if (pKThread->Priority != newPriority)
    {

        Kernel_WriteFlightLog(FLIGHTLOG, FL_SCHEDPRIO,  GetTID(pKThread),  // TID  
                                            (uint64_t)pKThread->Priority,  // original priority
                                            (uint64_t)newPriority,         // new priority
                                            mfspr(SPRG_SPIinfo)            // spi info spr
                                            );          

        // Get the HardwareThreadState object for the target kthread
        HWThreadState_t *pHwthreadState = pKThread->pHWThread;

        // Change the priority field in the target kthread. Assume caller has checked for validity of the new priority value
        pKThread->Priority = newPriority;

        pHwthreadState->priorityRefreshPending = 1;

        // Determine if we need to call the scheduler on our hardware thread or 
        // send an IPI to aother hardware thread. If we need to do an IPI, we will do it here, otherwise we
        // will return a flag to our caller that the scheduler should be entered on this hardware thread.
        // Set a flag if we need to potentially pre-empt the software thread that is currently running on the 
        // hardware thread that is hosting our target software thread
        int target_hwtindex = pKThread->ProcessorID;
        int my_hwtindex = ProcessorID();
        if (target_hwtindex == my_hwtindex)
        {
            // Set the pending scheduler flag in this kthread so that when we return from processing with
            // a system call or interrupt flow, we will call into the scheduler. 
            KThread_t *myKthread = GetMyKThread();
            // Only need to set the pending flag if we are not running in the kernel thread since the
            // only way we would be in the kernel thread is if we are in an interrupt handler that fired
            // while running in the scheduler.
            // Kernel thread will end up in the scheduler
            if (myKthread->SlotIndex != CONFIG_SCHED_KERNEL_SLOT_INDEX) 
            {

                // Set the pending scheduler flag in this kthread so that when we return from processing with
                // a system call or interrupt flow, we will call into the scheduler. 
                myKthread->Pending |= KTHR_PENDING_YIELD;
            }
        }
        else
        {

            // send IPI to the target hardware thread to re-run the scheduler
            IPI_run_scheduler(target_hwtindex,0,0);
        }
    }
    return rc;
}

void KThread_refreshPriorityData(HWThreadState_t *pHwthreadState)
{
    uint64_t priorityList[CONFIG_AFFIN_SLOTS_PER_HWTHREAD];
    uint64_t cur_val, prev_val, temp_val;
    int index, index2;
    for (index=0; index<CONFIG_AFFIN_SLOTS_PER_HWTHREAD; index++)
    {
        // Put the priority value in the upper 4 bytes and the slot index in the lower 4 bytes
        priorityList[index] = (((uint64_t)(pHwthreadState->SchedSlot[index]->Priority))<<32) + index;
    }
    // Do an insertion sort of the array to determine the new ordering for the slot indexes
    prev_val = priorityList[0];

    for (index = 1; index < CONFIG_AFFIN_SLOTS_PER_HWTHREAD; ++index)
    {
        cur_val = priorityList[index];
        if (prev_val < cur_val)
        {
            /* out of order: array[index-1] > array[index] */
            priorityList[index] = prev_val; /* move up the larger item first */

            /* find the insertion point for the smaller item */
            for (index2 = index - 1; index2 > 0;)
            {
                temp_val = priorityList[index2 - 1];
                if (temp_val < cur_val)
                {
                    priorityList[index2--] = temp_val;
                    /* still out of order, move up 1 slot to make room */
                }
                else
                    break;
            }
            priorityList[index2] = cur_val; /* insert the smaller item right here */
        }
        else
        {
            /* in order, advance to next element */
            prev_val = cur_val;
        }
    }
    // We now have the entries sorted, highest (most favored) to lowest (least favored) with the lower 4 bytes representing the required slot ordering. 
    // Equal priority entries will have the upper 4 bytes equal 
    // Create the SchedOrder doubleword.
    union
    {
        uint64_t SchedOrder;
        uint8_t  SchedOrderEntry[8];
    } loc;
    loc.SchedOrder = 0; // initialize

    uint32_t prevPriority = priorityList[0] >> 32;
    uint32_t relativePriority = 0;
    for (index=0; index<CONFIG_AFFIN_SLOTS_PER_HWTHREAD; index++)
    {
        uint32_t curPriority = priorityList[index] >> 32;
        if (prevPriority != curPriority)
            relativePriority += 0x10; // bump the upper nibble of the SchedOrderEntry to indicate the relative priority has changed
        // combine the slot index number with the relative priority number to create the complete entry 
        loc.SchedOrderEntry[index] = (priorityList[index] & 0x0F) + relativePriority;
        prevPriority = curPriority;
    }
    // Write our modified SchedOrder doubleword into the HwThreadState object if it differs from the current SchedOrder. 
    if (pHwthreadState->SchedOrder != loc.SchedOrder)
        pHwthreadState->SchedOrder = loc.SchedOrder;
}

// Move a Kthread from its current hardware thread to a new hardware thread. Return results of the operation.
void KThread_MoveToHwThread(KThread_t  *targetKThread)
{

    // Target KThread info
    KThread_t *pKThr = targetKThread;
    int processorID = pKThr->ProcessorID;
    // Target hardware thread info
    int new_hwthread = targetKThread->MigrationData.b.targetCPU;
    int new_slot = targetKThread->MigrationData.b.slot;

    HWThreadState_t *pTargetHwThdState = GetHWThreadStateByProcessorID(new_hwthread);
    // Current hardware thread info
    int myProcessorID = ProcessorID();
    HWThreadState_t *myHwThdState = GetMyHWThreadState();
    uint64_t migrateStatus = KThreadMigrateStatus_Success;

    // Determine if the KThread is already running on the targetted hardware thread. If so, no work to do.
    //   This test was already performed from the initiator of the set_affinity operation. We are 
    //   just verifying now that we are running on the kthread that is the target of the move. 
    if (new_hwthread == myProcessorID)
    {
        targetKThread->MigrationData.dword = 0; // We are done with the migration data.
        Kernel_WriteFlightLog(FLIGHTLOG, FL_MIGRATETH, (uint64_t)pKThr, (uint64_t)myProcessorID, (uint64_t)new_hwthread,(uint64_t)migrateStatus);
        return;
    }
    // Verify the KThread is really a kthread in the currently running hardware thread.
    //   This test was already performed from the initiator of the set_affinity operation. We are 
    //   just verifying now that we are running on the kthread that is the target of the move. 
    if (processorID != myProcessorID)
    {
        migrateStatus = KThreadMigrationStatus_HwthreadError;
    }
    // Inspect that the new hwthread and slot info passed in represents a kthread in the proper state. 
    // This should always be true if the initiating set_affinity code did its job, but just want to double check.
    KThread_t *pNewKThread = pTargetHwThdState->SchedSlot[ new_slot ];
    if (!(pNewKThread->State & SCHED_STATE_RESERVED))
    {
        migrateStatus = KThreadMigrationStatus_StateError;
    }
    if (migrateStatus != KThreadMigrateStatus_Success)
    {
        // Flight recorder entry that migration IPI handler ended with an error
        Kernel_WriteFlightLog(FLIGHTLOG, FL_MIGRATETH, (uint64_t)GetTID(pKThr), (uint64_t)myProcessorID, (uint64_t)new_hwthread,(uint64_t)migrateStatus);
        assert(migrateStatus == KThreadMigrateStatus_Success);
        // will not reach here
    }

    // Reset the Guarding registers associated with this hardware thread if the current thread is the thread that 
    // we are migrating or if the current thread is the Kernel thread. We must reset the WACs for the kernel
    // thread case since this the kthread we are migrating may have entered a futex or sleep. This would
    // have left the WAC guard registers set while waiting in the scheduler.
    if ((pKThr == myHwThdState->pCurrentThread) || 
        (myHwThdState->pCurrentThread == myHwThdState->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]))
    {
        Guard_Reset();
    }

    // Manage any watchpoints that may have been set for the migrating kthread
    Tool_MigrateWatchpoints(pKThr, new_hwthread, 0);

    // Lock our hardware thread for migration
    Kernel_Lock(&myHwThdState->migrationLock);

    // Set the RESET indicator into the state. We do not want to disturb any of the other bits in the kthread
    // state since this kthread may be in some other type of wait condition. We just want to put the brakes
    // on dispatching this kthread so that we can send the IPI_run_scheduler request and get control  on the target 
    // before the application is dispatched.
    Sched_Block(pKThr, SCHED_STATE_RESET);

    // Clear the UPC kthread active field. This is not supported for migrating threads.
    myHwThdState->pUpci_ActiveKThread = NULL;

    // Save fields that must be swapped between the kthreads
    uint16_t         tmpProcessorID = pKThr->ProcessorID;   
    uint16_t         tmpSlotIndex = pKThr->SlotIndex;   
    HWThreadState_t *tmppHwtThread = pKThr->pHWThread;  
    CoreState_t *    tmppCoreState = pKThr->pCoreState;

    // Set fields within the target kthread 
    pKThr->ProcessorID  = pNewKThread->ProcessorID;  
    pKThr->SlotIndex    = pNewKThread->SlotIndex;  
    pKThr->pHWThread    = pNewKThread->pHWThread;  
    pKThr->pCoreState   = pNewKThread->pCoreState;
    //note: we are purposely NOT setting the pAppProc field in this kthread since the process is staying the same.
    pKThr->Reg_State.sprg[SPRG_Index_SPIinfo] = (pKThr->Reg_State.sprg[SPRG_Index_SPIinfo] & ~0xfful) | new_hwthread;
    
    // Set fields within the kthread that we are replacing from the target hwthread
    pNewKThread->ProcessorID  = tmpProcessorID;
    pNewKThread->SlotIndex    = tmpSlotIndex;
    pNewKThread->pHWThread    = tmppHwtThread;
    pNewKThread->pCoreState   = tmppCoreState;
    pNewKThread->Reg_State.sprg[SPRG_Index_SPIinfo] = (pNewKThread->Reg_State.sprg[SPRG_Index_SPIinfo] & ~0xfful) | myProcessorID;
    // We are taking over this kthread to now be part of the process associated the hardware thread we are running on. It will be in our slot list
    pNewKThread->pAppProc     = GetProcessByProcessorID(ProcessorID()); 


    // If we are moving a COMM thread to a new hardware thread, clear the cached pointer in our hardware thread object
    if (pKThr->Policy == SCHED_COMM)
    {
        pNewKThread->pHWThread->pCommThread = NULL; // NULL the comm thread pointer in the orginal hardware thread state object
    }

    // Modify the slot array in our hardware thread to contain this kthread, replacing the kthread that we moved 
    myHwThdState->SchedSlot[pNewKThread->SlotIndex] = pNewKThread;

    // Note that pKThr is now orphaned from all slot tables.

    Kernel_Unlock(&myHwThdState->migrationLock);

    // Regenerate the priority data in our hardware thread
    KThread_refreshPriorityData(myHwThdState);

    // If we do not need to redrive (to save the non-volatiles), call the scheduler on the target hardware thread 
    if (pKThr == myHwThdState->pCurrentThread)
    {
        // Some comments regarding the non-standard state we are in. 
        // The current hardware thread has this kthread as its active kthread, however, this kthread structure has been
        // modified to contain the information regarding its new hardware thread. Also the new hardware thread 
        // is pointing to this kthread in its slot list. What allows this to work is that the STATE of this kthread
        // has the RESET indicator set, which will prevent this kthread from being dispatched on the new hardware thread,
        // eliminating the problem of having two hardware threads running the same kthread. The IPI_run_scheduler() call
        // that occurs within KThread_MigrateSelf will turn off this RESET indicator and allow the kthread to be 
        // dispatched on the new hardware thread.

        // The KThread we are migrating does not have its non-volatile state saved yet.
	// Therefore  we can't allow it to run on the target HWThread, hence the state bit of RESET is set.  
	// We will mark this kthread as a pending "MIGRATE".  We'll complete the migration
	// in KThread_MigrateSelf() below, which will be called before the thread is resumed.
	pKThr->Pending |= KTHR_PENDING_MIGRATE;
    }
    else
    {
	// Cause a reschedule on the target hwthread, because the new thread may be the highest priority thread.
	// This will also awaken the target thread if it happens to be sleeping and also bump the the thread count
        // in the SPR that contains the kernel SPI information.

        IPI_complete_migration(pKThr);
    }

    Kernel_WriteFlightLog(FLIGHTLOG, FL_MIGRATETH, (uint64_t)GetTID(pKThr), (uint64_t)myProcessorID, (uint64_t)new_hwthread,(uint64_t)migrateStatus);
}

// Complete migration of the currently running thread.  Called from LC_Migrate in start.S, as a consequence of
// KTHR_PENDING_MIGRATE being set above in KThread_MoveToHwThread().  All register state has been saved at this
// point.
void KThread_MigrateSelf(KThread_t *kthr)
{
    // Switch to scheduler thread before we deliver the interrupt. We do not want to be running the same kthread
    // on two hardware threads at the same time, even though the window is small. 
    HWThreadState_t *hwt = GetMyHWThreadState();
    KThread_t *kernel_kthr  = hwt->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX];
    hwt->pCurrentThread = kernel_kthr; // set current thread pointer

    // The thread will have been put in RESET state above.  Signal its hardware thread and make it runnable.
    IPI_complete_migration(kthr);

    // Find something else to do ...
    Scheduler();
    // NORETURN
}
#endif                                                                                                                 // FUSEDOS


#if defined __FUSEDOS__                                                                                                // FUSEDOS
#include "fusedos_config.h"                                                                                            // FUSEDOS
#include "fusedos.h"                                                                                                   // FUSEDOS
#include "cl_debug.h"                                                                                                  // FUSEDOS
KThread_t * cl_GetAvailableKThread();                                                                                  // FUSEDOS
int cl_SaveFPUState();
#endif

// Launch a thread within a process
int KThread_LaunchAppSibling( KThread_t *pKThr_Parent,   // thread calling pthread_create
                               void *cl_child_stack, // the child thread's stack pointer
                               void *cl_parent_tid,  // write new TID here
                               void *cl_tls,         // into GPR[13] for PPC64 ABI, else GPR[2] for PPC32 ABI.
                               void *cl_child_tid,   // clear this on thread exit
                               int  cl_flags)        // remember the creation flags
                                                      
{
    KThread_t    *pKThr_Child  = NULL;
#ifndef __FUSEDOS__                                                                                                   // FUSEDOS
    AppState_t   *pAppState = GetMyAppState();
#endif                                                                                                                 // FUSEDOS
    AppProcess_t *pAppProc  =  pKThr_Parent->pAppProc;
#ifndef __FUSEDOS__                                                                                                   // FUSEDOS
    Regs_t       *pRegs_Child  = NULL;
#endif                                                                                                                 // FUSEDOS

    // Get an available kthread
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    // save FPU state in current thread                                                                                // FUSEDOS
	if(cl_SaveFPUState()) return -1;                                                                                   // FUSEDOS
    pKThr_Child = cl_GetAvailableKThread();                                                                            // FUSEDOS
	CL_DEBUG("allocating KThread %d for child\n", (pKThr_Child == NULL) ? -1 : pKThr_Child->ProcessorID);              // FUSEDOS
#else                                                                                                                  // FUSEDOS
    pKThr_Child = Process_GetAvailableKThread(pAppProc);
#endif                                                                                                                 // FUSEDOS

    if (pKThr_Child == NULL)
    {
        return -1; // no kthread structures are available.
    }
    int child_thread_processorid = pKThr_Child->ProcessorID;
    // Set the priority information for the new thread into the hardware thread state.
    // We default to the least favored priority. Syscalls allow this to be altered by the 
    // user/library code.
    pKThr_Child->Priority = CONFIG_SCHED_MIN_FIFO_PRIORITY;
    HWThreadState_t*   pHWT_Child = GetHWThreadStateByProcessorID(pKThr_Child->ProcessorID);

    pHWT_Child->priorityRefreshPending = 1;

    // Set the stack guard area. First test to see if guarding is enabled.  
    if (pAppProc->Guard_Enable)
    {
        // First lock the Mprotect manager table
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Lock(&pAppProc->MprotectLock);
#endif                                                                                                                 // FUSEDOS
        // Search the table of protected addresses and find the nearest protected page
        int i;
        uint64_t protect_start = 0;
        uint64_t protect_size = 0;
        for (i=0; i< MPROTECT_MGR_ENTRIES; i++)
        {
            uint64_t addr = pAppProc->MprotectMgr.address[i];
            uint64_t len  = pAppProc->MprotectMgr.length[i];
            if (addr && len &&                               // Do we have data in this table entry
                ((addr+len) < (uint64_t)cl_child_stack) &&                // Is the highest protected address less than our stack start address
                ((addr+len) > (protect_start+protect_size)) ) // Is this protected range closer to our stack than the previous range
            {
                protect_start = addr; // Save a new guard area start address
                protect_size = len;   // Save a new guard area length
            }
        }
        // Release the lock
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&pAppProc->MprotectLock);
#endif                                                                                                                 // FUSEDOS

        // Set up the guard page for the stack based on mprotect data stored in the process 
        if (protect_start && protect_size)
        {
            // Prepare the guard area by setting a pattern in the area and removing the area from the L1 cache
            //printf("LaunchSibling prepare: %016lx - %016lx  stack:%016lx\n", (uint64_t)protect_start, (uint64_t)protect_start+protect_size-1, (uint64_t)cl_child_stack);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
            Guard_Prepare(NULL, (void*)protect_start, protect_size);
#endif                                                                                                                 // FUSEDOS

            pKThr_Child->GuardBaseAddress = pAppProc->Heap_PStart + (protect_start - pAppProc->Heap_VStart);
            pKThr_Child->GuardEnableMask = ~(protect_size-1);

            TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Setting Guard. stack: %016lx Guard: %016lx, Guard Mask: %016lx\n", __func__, ProcessorID(),
                                              (uint64_t)cl_child_stack, protect_start, pKThr_Child->GuardEnableMask ));
            //printf("(I) %s[%d]: Setting Guard. stack: %016lx Guard: %016lx, Mask: %016lx\n", __func__, ProcessorID(),
            //                                  (uint64_t)cl_child_stack, protect_start, pKThr_Child->GuardEnableMask );

        }
        else
        {
            // Set to values that will not match (physical address larger than actual physical storage)
            pKThr_Child->GuardBaseAddress = -1; 
            pKThr_Child->GuardEnableMask =  -1;

            TRACE( TRACE_MemAlloc, ("(W) %s[%d]: Not setting guard! No address found in MMapMgr\n %016lx%016lx%016lx%016lx\n", __func__, ProcessorID(),
                                              pAppProc->MprotectMgr.address[0],pAppProc->MprotectMgr.address[1],
                                              pAppProc->MprotectMgr.address[2],pAppProc->MprotectMgr.address[3]));
        }
    }

    // Set the default policy. Inherit from the caller  
    pKThr_Child->Policy = pKThr_Parent->Policy;
    pKThr_Child->Priority = pKThr_Parent->Priority;
#ifndef __FUSEDOS__														                                           // FUSEDOS
    pRegs_Child = &(pKThr_Child->Reg_State);
#endif                                                                                                                 // FUSEDOS

    TRACE( TRACE_Process, ("(I) %s[%2d:%d]: Creating Core.HWThr.AppThr=[%d.%d] AppProc=%016lx.\n",
                                 __func__, ProcessorCoreID(), ProcessorThreadID(),
                                 child_thread_processorid>>2, child_thread_processorid & 0x3, (uint64_t)pAppProc ));


    // fill in the child's KThread and Launch Context
    pKThr_Child->pAppProc       = pAppProc;
    pKThr_Child->Clone_Flags    = cl_flags;
    pKThr_Child->pTLS_Area      = cl_tls;
    pKThr_Child->pParent_TID    = cl_parent_tid;
    pKThr_Child->pChild_TID     = cl_child_tid;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    pKThr_Child->FutexQueueNext = (KThread_t *)0;
    pKThr_Child->FutexVAddr     = NULL;
    pKThr_Child->FutexValue     = 0;
    pKThr_Child->FutexTimeout   = 0;
    pKThr_Child->FutexIsShared  = 0;
#endif                                                                                                                 // FUSEDOS
    pKThr_Child->TID_Address    = 0;
    pKThr_Child->SyscallReturn  = 0;
    pKThr_Child->Pending        = 0;
    pKThr_Child->PendingKThread = NULL;
    pKThr_Child->MaskedSignals  = pKThr_Parent->MaskedSignals; // inherit sigmask
    pKThr_Child->PendingSignals = 0;
    pKThr_Child->pUserStack_Bot = cl_child_stack;
    pKThr_Child->AlignmentExceptionCount = 0;
    pKThr_Child->physical_pid = pAppProc->PhysicalPID; // clone always selects a "home" hwthread first so we should use the hardware pid from the process.

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    pRegs_Child->gpr[ 1]        = (size_t)cl_child_stack;
    pRegs_Child->gpr[2]         = pKThr_Parent->Reg_State.gpr[2];
    pRegs_Child->gpr[13]        = (size_t)cl_tls; // PowerPC64 ABI
    pRegs_Child->gpr[ 3]        = 0;
    pRegs_Child->ip             = pKThr_Parent->Reg_State.ip;
    pRegs_Child->msr            = pKThr_Parent->Reg_State.msr;
    pRegs_Child->cr             = pKThr_Parent->Reg_State.cr;
    pRegs_Child->lr             = pKThr_Parent->Reg_State.lr;
    pRegs_Child->xer            = pKThr_Parent->Reg_State.xer;
    pRegs_Child->ctr            = pKThr_Parent->Reg_State.ctr;
    pRegs_Child->pid            = pKThr_Child->physical_pid; 
    pRegs_Child->sprg[SPRG_Index_SPIinfo] = (uint64_t)((pAppState->Active_Processes << 8) | // byte7: number of processes  
                                                       (child_thread_processorid));         // byte8: hw processor id

    if (pAppProc->propogateWatchpoints)
    {
        if (pKThr_Parent->watch1Type || pKThr_Parent->watch2Type)
        {
            pKThr_Child->watch1Type = pKThr_Parent->watch1Type;
            pKThr_Child->watch2Type = pKThr_Parent->watch2Type;
            pRegs_Child->dbcr0 = pKThr_Parent->Reg_State.dbcr0 & (DBCR0_IDM | DBCR0_DAC1(-1) | DBCR0_DAC2(-1) | DBCR0_DAC3(-1) | DBCR0_DAC4(-1));
            pRegs_Child->dbcr2 = pKThr_Parent->Reg_State.dbcr2 & (DBCR2_DAC1US(-1) | DBCR2_DAC1ER(-1) | DBCR2_DAC2US(-1) | DBCR2_DAC2ER(-1) | DBCR2_DAC12M);
            pRegs_Child->dbcr3 = pKThr_Parent->Reg_State.dbcr3 & (DBCR3_DAC3US(-1) | DBCR3_DAC3ER(-1) | DBCR3_DAC4US(-1) | DBCR3_DAC4ER(-1) | DBCR3_DAC34M);
            pRegs_Child->dac1 = pKThr_Parent->Reg_State.dac1;
            pRegs_Child->dac2 = pKThr_Parent->Reg_State.dac2;
            pRegs_Child->dac3 = pKThr_Parent->Reg_State.dac3;
            pRegs_Child->dac4 = pKThr_Parent->Reg_State.dac4;
            Tool_MigrateWatchpoints(pKThr_Child, child_thread_processorid, 1 );
        }
    }
#endif                                                                                                                 // FUSEDOS
    Process_InitializeThreadLocalStorage( pAppProc, (void *)cl_tls );

    ppc_msync();

    // The thread's non-volatile state has not been saved yet.  Set a Pending
    // bit, which will result in a full state save to the new thread and a call
    // to KThread_CompleteAppSibling() to enable it to run.  The state-saving
    // code and CompleteAppSibling need the new kthread pointer, so we pass it
    // through a KThread field reserved for this purpose.  Ugly but effective.
    // The thread will be launched as if returning from a syscall, so the
    // volatile state that isn't preserved across a syscall will be cleared.
#ifndef __FUSEDOS__
    pKThr_Parent->PendingKThread = pKThr_Child;
    pKThr_Parent->Pending |= KTHR_PENDING_CLONE;
#endif

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    spc_context_t* pc_parent = get_spc_context(this_spc());                                                            // FUSEDOS
    spc_context_t* pc_child = get_spc_context(pKThr_Child->ProcessorID);                                               // FUSEDOS
    memset(&(pc_child->regs), 0, sizeof(regs_t));                                                                      // FUSEDOS
    pc_child->regs.gpr[1]  = (size_t)cl_child_stack; // stack pointer                                                  // FUSEDOS
    pc_child->regs.gpr[2]  = pc_parent->regs.gpr[2];                                                                   // FUSEDOS
    pc_child->regs.gpr[13] = (size_t)cl_tls;                                                                           // FUSEDOS
    pc_child->regs.gpr[3]  = 0;                                                                                        // FUSEDOS
    pc_child->regs.ip      = pc_parent->regs.ip;                                                                       // FUSEDOS
    pc_child->regs.msr     = pc_parent->regs.msr;                                                                      // FUSEDOS
    pc_child->regs.cr      = pc_parent->regs.cr;                                                                       // FUSEDOS
    pc_child->regs.lr      = pc_parent->regs.lr;                                                                       // FUSEDOS
    pc_child->regs.xer     = pc_parent->regs.xer;                                                                      // FUSEDOS
    pc_child->regs.ctr     = pc_parent->regs.ctr;                                                                      // FUSEDOS
    pc_child->regs.sprg[SPRG_Index_SPIinfo] = (pc_parent->regs.sprg[SPRG_Index_SPIinfo] & ~0xfful )                    // FUSEDOS
									| (child_thread_processorid);                                                      // FUSEDOS
    pc_child->regs.pid     = pc_parent->regs.pid;                                                                      // FUSEDOS

    int i;                                                                                                             // FUSEDOS
    for (i = 14; i <= 31; i++) {                                                                                       // FUSEDOS
      pc_child->regs.gpr[i] = pc_parent->regs.gpr[i];                                                                  // FUSEDOS
    }                                                                                                                  // FUSEDOS
    for (i = 0; i < NUM_QVRS; i++) {                                                                                   // FUSEDOS
      pc_child->regs.gvr[i] = pc_parent->regs.gvr[i];                                                                  // FUSEDOS
    }                                                                                                                  // FUSEDOS
	
	//CL_DEBUG("child context:\n");
	//if(debug) dump_context( pc_child );

    TRACE(TRACE_Process, ("(I) %s: tlb_entry_count %lu\n", __func__, pc_parent->tlb_entry_count));                     // FUSEDOS
    /*for (i = 0; i < pc_parent->tlb_entry_count; i++) {                                                                 // FUSEDOS
      pc_child->tlb_entry[i].slot = pc_parent->tlb_entry[i].slot;                                                      // FUSEDOS
      pc_child->tlb_entry[i].mas1 = pc_parent->tlb_entry[i].mas1;                                                      // FUSEDOS
      pc_child->tlb_entry[i].mas2 = pc_parent->tlb_entry[i].mas2;                                                      // FUSEDOS
      pc_child->tlb_entry[i].mas7_3 = pc_parent->tlb_entry[i].mas7_3;                                                  // FUSEDOS
      pc_child->tlb_entry[i].mas8 = pc_parent->tlb_entry[i].mas8;                                                      // FUSEDOS
      pc_child->tlb_entry[i].mmucr3 = pc_parent->tlb_entry[i].mmucr3;                                                  // FUSEDOS
      pc_child->tlb_entry_count++;                                                                                     // FUSEDOS
    }*/                                                                                                                  // FUSEDOS

	// start the cl thread for the child
	pKThr_Child->cl_thread_state = USE_SPC;
	ppc_msync();
	pthread_cond_broadcast( &NodeState.cl_thread_cond );
#endif                                                                                                                 // FUSEDOS

    return( GetTID(pKThr_Child) ); // return the TID for use by the pthread libraries
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Complete the launch of a thread within a process
void KThread_CompleteAppSibling( KThread_t *pKThr )
{
    // signal the target hwthread and remove the RESET status from the target kthread
    Sched_Unblock(pKThr, SCHED_STATE_RESET);
}

void KThread_InitHwThread()
{
    int core = ProcessorCoreID();
    int hwtid = ProcessorThreadID();
    int processorid = ProcessorID();
    CoreState_t *pCS = &(NodeState.CoreState[core]);
    HWThreadState_t *pHWT = &(pCS->HWThreads[hwtid]);

    uint32_t slot;
    for ( slot = 0 ; slot < CONFIG_SCHED_SLOTS_PER_HWTHREAD ; slot++)
    {
        KThread_t   *kthr = &(NodeState.KThreads[ slot + processorid * CONFIG_SCHED_SLOTS_PER_HWTHREAD]);
        pHWT->SchedSlot[slot] = kthr;
        memset(kthr,0x00,sizeof(KThread_t));
        kthr->State = SCHED_STATE_FREE;
        kthr->ProcessorID = processorid;
        kthr->SlotIndex = slot;
        kthr->pHWThread = pHWT;
        kthr->pCoreState = pCS;
        kthr->GuardBaseAddress = -1;
        kthr->GuardEnableMask = -1;
    }
    pHWT->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]->State = SCHED_STATE_RUN; // init Kernel thread to RUN state
    pHWT->itimer.signal = 0;
    pHWT->itimer.owner = NULL;
    pHWT->itimer.expiration = 0;
    pHWT->itimer.interval = 0;
    pHWT->udecr_expiration = 0;
    pHWT->SchedSlot[CONFIG_SCHED_SLOT_FIRST]->Priority = CONFIG_SCHED_MIN_FIFO_PRIORITY;
    pHWT->pUpci_ActiveKThread = NULL;
    pHWT->launchContextFlags.dword = 0;
    pHWT->appExitPhase = AppExit_Inactive;
    pHWT->pCommThread = NULL;
    pHWT->SlotsAllocated = 0;
    pHWT->nanoTimerEnabled = 0;
    pHWT->IpiExitInterruptPending = 0;
    pHWT->suspended = 0;
    pHWT->pendingSpecRestartContext = NULL;
    pHWT->ipiMessageCheckerExpiration = 0;
    // Initialize the priority information in the hardware thread
    KThread_refreshPriorityData(pHWT);
    pHWT->priorityRefreshPending = 0;
}

int KThread_SetMigrationTarget(KThread_t *kthread, uint32_t hwthread_index, int reserved_slot)
{
    volatile uint32_t *lockword = &kthread->MigrationData.dword;
    uint32_t oldVal = 0;
    uint32_t newValue = 0x80000000;
    if (Compare_and_Swap32( lockword, &oldVal, newValue ))
    {
        // We now own the migration data field
        kthread->MigrationData.b.slot = reserved_slot;
        kthread->MigrationData.b.targetCPU = hwthread_index;
        return 0;
    }
    return -1;
}

int KThread_ReserveForMigration(uint32_t hwthread_index)
{
    // Walk the slots in the target hardware threads scheduler and find a FREE kthread
    // and attempt to modify it to the RESET state.
    HWThreadState_t *hwt = GetHWThreadStateByProcessorID(hwthread_index);
    int affinity_slots = CONFIG_AFFIN_SLOTS_PER_HWTHREAD;
    int reserved_slot = -1;
    // loop through the scheduler affinity slots and find/reserve a slot for us.
    AppProcess_t *proc = GetMyProcess();
    if (proc->ThreadModel == CONFIG_THREAD_MODEL_BGP)
    {
        // If we have a comm thread, allow an app thread to co-exist with it.
        affinity_slots = (hwt->pCommThread ? 2 : 1);
    }

    int sched_slot;
    if (hwt->SlotsAllocated < affinity_slots)
    {
        Kernel_Lock(&hwt->migrationLock);
        // Refresh affinity_slots now that we have the lock
        if (proc->ThreadModel == CONFIG_THREAD_MODEL_BGP)
        {
            affinity_slots = (hwt->pCommThread ? 2 : 1); // refresh now that we have the lock
        }
	for (sched_slot=0; sched_slot < affinity_slots; sched_slot++)
	{
	    uint32_t *pState = &(hwt->SchedSlot[ sched_slot ]->State);
	    uint32_t oldState, newState;
	    // if state is FREE and not RESERVED, add the RESERVED bit
	    // atomically, preserving any other bits (e.g. SUSPEND).
	    do
	    {
		oldState = LoadReserved32(pState);
		if (!(oldState & SCHED_STATE_FREE) ||
		    (oldState & SCHED_STATE_RESERVED)) goto trynext;
		newState = oldState | SCHED_STATE_RESERVED;
	    } while (!StoreConditional32(pState, newState));
	    // We managed to reserve the kthread currently in sched_slot.
	    ppc_msync(); // Ensure that the state change is seen before the counter in updated
	    reserved_slot = sched_slot;
	    incrementSlotAllocatedCount(hwt);
	    break;  // exit the loop, we are done
	trynext:
	    continue;
	}
        Kernel_Unlock(&hwt->migrationLock);
    }
    return reserved_slot;
}

int KThread_ReleaseForMigration(uint32_t hwthread_index, int reserved_slot) 
{
    // Look at the provided slot and if the target kthread is in the RESET state, 
    // return it to the FREE state.
    HWThreadState_t *hwt = GetHWThreadStateByProcessorID(hwthread_index);
    Kernel_Lock(&hwt->migrationLock);
    uint32_t *pState = &(hwt->SchedSlot[ reserved_slot ]->State);
    uint32_t oldState, newState;
    int rc = -1;
    // state should be FREE and RESERVED.  If it is, remove the RESERVED bit
    // atomically, preserving any other bits (e.g. SUSPEND).
    do
    {
	oldState = LoadReserved32(pState);
	if (!(oldState & SCHED_STATE_RESERVED)) goto failure;
	newState = oldState & ~SCHED_STATE_RESERVED;
    } while (!StoreConditional32(pState, newState));
    // The kthread in reserved_slot was successfully released.
    decrementSlotAllocatedCount(hwt);
    rc = 0;
failure:
    Kernel_Unlock(&hwt->migrationLock);
    return rc;
}


void KThread_CompleteMigration(KThread_t *newKThread)
{
    //printf("complete migration started on hwt %d. Current tid:%d\n", ProcessorID(), GetTID(GetMyKThread()));
    HWThreadState_t *hwt = GetMyHWThreadState();
    Kernel_Lock(&hwt->migrationLock);
    int remoteThreadMigration = (newKThread->pAppProc == GetProcessByProcessorID(ProcessorID())) ? 0 : 1;

    // Save the previous kthread in the slot pointed to by the passed in kthread.
    KThread_t *originalKThread = hwt->SchedSlot[newKThread->SlotIndex];

    // Store the new Kthread into this slot.
    hwt->SchedSlot[newKThread->SlotIndex] = newKThread;

    // If we are a COMM thread, setup the cached kthread pointer
    if (newKThread->Policy == SCHED_COMM)
    {
        hwt->pCommThread = newKThread; // Set the comm thread pointer in the hardware thread state object
    }
    // Remember original hwthread id.  Once we've released originalKThread, we
    // can no longer trust its ProcessorID field.
    int originalProcessorID = originalKThread->ProcessorID;

    // Free the original kthread. This kthread is currently in the slot array of the hardware thread that was the source of the migration
    uint64_t appExitMask = remoteThreadMigration ? SCHED_STATE_APPEXIT : 0; // if this is a remote migration, we may have picked up a kthread with the exit status set.
    uint32_t *pState = &(originalKThread->State);
    uint32_t oldState, newState;
    // state should be FREE and RESERVED.  Remove the RESERVED bit
    // atomically, preserving any other bits (e.g. SUSPEND).
    do
    {
	oldState = LoadReserved32(pState);
	assert(oldState & SCHED_STATE_RESERVED);
	newState = oldState & ~(SCHED_STATE_RESERVED | appExitMask);
    } while (!StoreConditional32(pState, newState));

    ppc_msync();

    Kernel_Unlock(&hwt->migrationLock);

    // Put an entry on the HWThread Recycle list indicating that there is an additional slot available
    Process_MakeHwThreadAvail(originalKThread->pAppProc, originalProcessorID);

    // If this is the extended thread affinity migration, make sure the APPEXIT state is turned off.  We need to let these threads run.
    if (remoteThreadMigration)
    {
        Sched_Unblock(newKThread, SCHED_STATE_APPEXIT);
    }
    // Put this thread into a runnable state
    Sched_Unblock( newKThread, SCHED_STATE_RESET );

    // Regenerate the priority data in this hwthread since the new kthread likely has a different priority than 
    // the kthread that it replaced.
    hwt->priorityRefreshPending = 1;

    // If this new kthread has a higher priority than the currently running kthread, force us through the
    // scheduler to re-evaluate who should be running. If there are no other wait conditions on the new
    // kthread, then it should pre-empt this kthread.
    KThread_t *myKThread = GetMyKThread();
    if ((myKThread->SlotIndex != CONFIG_SCHED_KERNEL_SLOT_INDEX) &&
        (newKThread->Priority > myKThread->Priority))
    {
        myKThread->Pending |= KTHR_PENDING_YIELD;
    }

    // If this was a migration of a remote thread, call the static mapper to add the process TLBs to this core and modify the physical pid.
    if (remoteThreadMigration)
    {
        //printf("In complete migration. new pid %d existing pid in kthread: %d\n", newKThread->MigrationData.b.physical_pid, newKThread->physical_pid);
        newKThread->physical_pid = newKThread->MigrationData.b.physical_pid; // update the pid
        // Apply the new TLBs required for the process
        Kernel_Lock(&NodeState.CoreState[ProcessorCoreID()].coreLock);
        //printf("Calling vmm_installStaticTLBMapPartial. Installing tlbs for pid:%d current pid:%ld current tid:%d\n", newKThread->physical_pid, mfspr(SPRN_PID), GetTID(GetMyKThread()));
        vmm_installStaticTLBMapPartial(GetProcessByProcessorID(ProcessorID())->Tcoord, (int)newKThread->physical_pid);
        Kernel_Unlock(&NodeState.CoreState[ProcessorCoreID()].coreLock);
        //printf("Returned from calling vmm_installStaticTLBMapPartial\n");
        // Update the mask of remote thread targets. This is used in the process exit flow.
        uint64_t old_val, tmp_val;
        ppc_msync();
        do
        {
            old_val = LoadReserved(&(newKThread->pAppProc->HwthreadMask_Remote));
            tmp_val = old_val | _BN(newKThread->ProcessorID);
        }
        while ( !StoreConditional( &(newKThread->pAppProc->HwthreadMask_Remote), tmp_val ) );
    }
    else
    {
        // We may  be moving from a remote thread to a local thread. Refresh the physical_pid to match the process physical_pid
        newKThread->physical_pid = newKThread->pAppProc->PhysicalPID;
    }
    // We have consumed the migration data. Reset the data.
    newKThread->MigrationData.dword = 0;
}

#endif                                                                                                                 // FUSEDOS
