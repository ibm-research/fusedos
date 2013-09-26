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

#ifndef _KTHREAD_H_ // Prevent multiple inclusion.
#define _KTHREAD_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__)

// Bit values for Pending field
#define KTHR_PENDING_SNAPSHOT  0x0001
#define KTHR_PENDING_MIGRATE   0x0002
#define KTHR_PENDING_YIELD     0x0004
#define KTHR_PENDING_SIGNAL    0x0008
#define KTHR_PENDING_CLONE     0x0010
#define KTHR_PENDING_SIGRETURN 0x0020
#define KTHR_PENDING_NVGPR     0x0040

#if !defined(__ASSEMBLY__)

#include <cnk/include/Config.h>

#include <hwi/include/common/bgq_alignment.h>

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <cnk/include/klibtypes.h>
#include <cnk/include/AppState.h>
#include "CoreState.h"
#endif                                                                                                                 // FUSEDOS

#ifdef __FUSEDOS__                                                                                                    // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS

enum cl_thread_state_t { IDLE, USE_SPC, EXIT };                                                                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS

typedef struct KThread_t
{
    // This register context must be located in the beginning of the KThread structure
    Regs_t Reg_State;

    uint64_t SyscallReturn;  // Flag for LC_ContextLaunch.
    			     // If non-zero, Reg_State holds the minimum state
    			     // necessary for returning from a syscall.
			     // Otherwise it holds a complete register state.
    uint64_t Pending;        // Bit vector of exceptional conditions to be
			     // handled when the thread is next dispatched
    void*    PendingKThread; // Helper field, used with PENDING_CLONE
    uint64_t PendingNVGPR;   // Helper field, used with PENDING_NVGPR

    uint64_t SchedulerDivert;  // Bit vector of diversions to be taken when
                               // this thread next reaches Scheduler()
    uint64_t ExceptionCode;    // DEBUG_CODE for current exception
    uint64_t ExceptionTime;    // Timebase for current exception
    int SigInfoSigno;          // siginfo for current signal or exception
    int SigInfoCode;
    void *SigInfoAddr;

    uint64_t MaskedSignals;
    uint64_t PendingSignals;
                                           
    uint32_t State;     // Free/Futex, Blocking reasons, etc. see Sched.h
    int32_t  ProcessorID; // processor id (0..67)
    int32_t  Priority;  // user supplied, unmodified priority 0 = least preferred app thread.  99: most preferred app thread
    uint32_t Policy;    // scheduling policy 
    uint16_t SlotIndex;    // 0..(CONFIG_SCHED_SLOTS_PER_HWTHREAD-1): This s/w thread's index
    uint8_t  isProcessLeader; // Flag indicating whether this is the main thread of a process

    // Tool/Debugger control 
    uint8_t  watch1Type; // DAC1/DAC2 pair has a watchpoint active. Value of bgcios::toolctl::WatchType
    uint8_t  watch2Type; // DAC3/DAC4 pair has a watchpoint active. Value of bgcios::toolctl::WatchType
    uint8_t  pad1;         // (avail)  
                           //
    // Hardware pid value to be used during migration. 
    uint16_t  physical_pid; // physical pid used during dispatch. 
                                               
    struct HWThreadState_t* pHWThread;  // State for this h/w thread's scheduler, etc.
    struct CoreState_t*     pCoreState;
    struct AppProcess_t*    pAppProc;  // Can be NULL when this thread is not part of any process
    void*                   pUserStack_Top;
    void*                   pUserStack_Bot;


    // Info required by NPTL
    void   *pTLS_Area;   // points into user-space
    void   *pParent_TID; // points into user-space
    void   *pChild_TID;  // points into user-space
    void   *TID_Address; // set by sc_set_tid_address
    int32_t Clone_Flags;  // save for debugging
                          //
    uint32_t pad;         // available.

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Migration data.
    union {
        volatile uint32_t dword; 
        struct {
            int8_t  active; // Non-zero value indicates migration data is active
            int8_t  pad1;
            int8_t  targetCPU; // Target processor id for the migration
            int8_t  slot;      // Target slot identifying the kthread to consume.
            uint16_t physical_pid; 
            uint16_t pad2;
        } b;
    } MigrationData;

    // Futex Wait Queue: a thread can be blocked on at most 1 Futex.
    struct KThread_t *FutexQueueNext;
    Futex_t    *FutexVAddr;   // VA of Futex this thread is blocking on
    Futex_t     FutexValue;   // Futex value at time of sleep
    int         FutexIsShared;// Is the blocked futex a shared futex
    volatile uint64_t FutexTimeout; // Futex timeout (in absolute pclk)
#endif                                                                                                                 // FUSEDOS

    // Support for nanosleep.
    uint64_t SleepTimeout; // Sleep timeout (in absolute pclk)
    struct timespec *SleepRemainder; // if non-NULL, place to put remaining time
    
    struct ShmMgrEntry_t* flockBlockedFile;
    
    // Stack Guarding
    uint64_t    GuardBaseAddress;
    uint64_t    GuardEnableMask;

    // Hold/Release control
    uint64_t    holdCounter;

    uint64_t    KernelInternal;  // True when kernel is using internal syscall interfaces


    char whoami[24]; // Buffer for generating id string.

    // UPCI Ptr to active Upci_SaveCfg block within kernel memory.
    // A non-null value indicates that scheduler should call Upci_Swapout(pUpci_Cfg), or
    // Upci_Swapin(pUpci_Cfg) during swaps.
    void *pUpci_Cfg;

    // Number of alignment exceptions that have occurred on this kthread
    uint64_t AlignmentExceptionCount;
    // Most recent syscall executed on this thread
    uint64_t syscallNum;

#ifdef __FUSEDOS__                                                                                                    // FUSEDOS
	pthread_t cl_thread;                                                                                              // FUSEDOS
	enum cl_thread_state_t cl_thread_state;
	int in_spc_ioctl;
#endif                                                                                                                 // FUSEDOS

}
ALIGN_L1D_CACHE KThread_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#define KTHREAD_SETAFFINITY_SUCCESS 1
#define KTHREAD_SETAFFINITY_FAIL    2

enum KThreadMigrateStatus
{
    KThreadMigrateStatus_Success,
    KThreadMigrationStatus_HwthreadError,
    KThreadMigrationStatus_CommConflict,
    KThreadMigrationStatus_StateError
};


// see start.S
void LC_ContextLaunch( Regs_t *pRegs );
#endif                                                                                                                 // FUSEDOS

int KThread_LaunchAppSibling( KThread_t *pKThread,   // thread calling pthread_create
                               void *cl_child_stack, // the child thread's stack pointer
                               void *cl_parent_tid,  // write new TID here
                               void *cl_tls,         // into GPR[13] for PPC64 ABI, else GPR[2] for PPC32 ABI.
                               void *cl_child_tid,   // clear this on thread exit
                               int  cl_flags);        // remember the creation flags

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Change the thread policy of a Kthread
int KThread_ChangePolicy(KThread_t*pKThread, int newPolicy, int newPriority);

// Change the thread priority of a Kthread and modify the associated SchedOrder object for use by the scheduler
int KThread_ChangePriority(KThread_t*pKThread, int newPriority);
void KThread_refreshPriorityData(HWThreadState_t *pHwthreadState);

// Move the kthread to from its current hardware thread home to a new hardware thread
void KThread_MoveToHwThread(KThread_t *targetKthread);

// Reset fields in the HWThreadState object, including all the KThreads that it controls
void KThread_InitHwThread();

// Find an unused kthread on the target hardware thread and reserved it for migration.
int KThread_ReserveForMigration(uint32_t hwthread_index);

// Set migration information into a kthread. If failure returns -1. 
int KThread_SetMigrationTarget(KThread_t *kthread, uint32_t hwthread_index, int reserved_slot);

// Remove the migration reservation set by the KThread_ReserveForMigration() function.
int KThread_ReleaseForMigration(uint32_t hwthread_index, int reserved_slot);
 
// Complete migration
void KThread_CompleteMigration(KThread_t *newKThread); 




#endif // __ASSEMBLY__
#endif                                                                                                                 // FUSEDOS

#endif // __KERNEL__

#endif // Add nothing below this line.

