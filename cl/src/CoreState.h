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

#ifndef _CORESTATE_H_ // Prevent multiple inclusion.
#define _CORESTATE_H_

#include <hwi/include/common/compiler_support.h>
#include "Regs.h"

#if defined(__KERNEL__)

// We have two kernel stacks for each hardware thread, one for the "standard"
// environment and one for the "critical" environment.  StandardStack is used
// unconditionally for syscalls and standard external interrupts, and whenever
// we've taken an exception while executing in user mode.  CriticalStack is
// used unconditionally for critical-input interrupts and whenever we've taken
// an exception while executing in kernel mode.  Neither stack ever holds a
// register context.  In the standard environment, any necessary state will
// have been saved in the current KThread structure.  In the critical
// environment, a full register context will have been saved in the HWThread
// structure.  For now, we make both stacks the same size.  The size must
// be a multiple of L1D_CACHE_LINE_SIZE (64).

#define KSTACK_SIZE  (4 * 1024)

#if !defined(__ASSEMBLY__)


#include <cnk/include/Config.h>

#include <stdint.h>

#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/common/bgq_bitnumbers.h>

#include <cnk/include/SPRG_Usage.h>
#include "KThread.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "Sched.h"
#endif                                                                                                                 // FUSEDOS
#include <cnk/include/AppState.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <spi/include/kernel/thread.h>

typedef void (*IPIHANDLER_Fcn_t)( uint64_t , uint64_t );
IPIHANDLER_Fcn_t IPI_DeliverInterrupt(int thread_index, IPIHANDLER_Fcn_t handler, uint64_t parm1, uint64_t parm2 );

// IPI Message Entry
typedef struct IPI_Message_t
{
    IPIHANDLER_Fcn_t fcn;
    uint64_t param1;
    uint64_t param2;    
} IPI_Message_t;
#endif                                                                                                                 // FUSEDOS

// A full and aligned L1 CacheLine for Dummy Stwcx's by this core.
typedef struct DummyStwcx
                {
                volatile uint64_t data[ 1 ];
                }
                ALIGN_L1D_CACHE DummyStwcx;

#define ITIMER_NONE -1

typedef struct ItimerState_t
{
  int signal;
  struct KThread_t *owner;
  uint64_t expiration;
  uint64_t interval;
} ItimerState_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
typedef struct T_CommThreadData
{
    Kernel_CommThreadHandler funcptr; 
    uint64_t gpr[NUM_COMMTHREAD_GPRS];
    uint64_t puea_map_save;
    uint64_t puea_map_set;
    uint64_t puea_map_mask;
} CommThreadData;
#endif                                                                                                                 // FUSEDOS

typedef struct LaunchContextFlags_t
{
    union
    {
        uint64_t dword;
        struct {
            uint64_t GuardActive      : 1; // Stack guarding is active. Must restore stack guarding register on launch
            uint64_t DebugControlRegs : 1; // Debug control register modifications are active. Must restore debug control registers
            uint64_t unused           :62; 
        } flag;
    };
} LaunchContextFlags_t;

// 0..3: one per h/w thread of each CoreState
typedef struct HWThreadState_t
{
    // CriticalState MUST be located at the beginning of HWThreadState_t.
    // It holds the saved register state in the critical environment.
    Regs_t CriticalState;
        
    // Kernel stack for the critical environment.
    char ALIGN_L1D_CACHE CriticalStack[ KSTACK_SIZE ];
        
    // Kernel stack for the standard environment.
    char ALIGN_L1D_CACHE StandardStack[ KSTACK_SIZE ];

    // Roll-back h/w state for soft-error recovery
    Regs_t RollbackState;

    // Values cached for use in fast speculation syscall and interrupt paths.
    // For performance reasons, these fields should be located in the same
    // L1 cache line.
    uint64_t ALIGN_L1D_CACHE
	     SpecStateAddr;	// User's speculation-state address
    uint64_t PhysicalPID;	// A2 processor PID value for primary alias
    uint64_t PhysicalSpecPID;	// A2 processor PID value for speculative alias
    uint64_t L1P_cfg_pf_sys;	// L1P config value (used for invalidations)
    uint64_t SpecSaveR0;	// Temporary save area
    uint64_t SpecSaveR6;	// Temporary save area
    uint64_t SpecSaveR7;	// Temporary save area
    
    struct KThread_t *pCurrentThread;

    // Migration lock. Required for managing the kthread pointers in the SchedSlot array
    Lock_Atomic_t migrationLock;

    struct CoreState_t *pCore;

    // Timer state on hardware thread
    struct ItimerState_t itimer;

    // UDECR expiration time on hardware thread
    uint64_t udecr_expiration;

    // Scheduler slot ordering based on priority. Priority adjustments will modify
    // this doubleword. Each byte in the doubleword provides a relative thread priority
    // and a slot index.The upper nibble of each byte is the relative priority of the
    // entry with respect to other entries. That is, slots that have the same
    // priority will all contain the same value in this upper nibble. The lower nibble of 
    // each byte represents the index into the SchedSlot array of KThread pointers. 
    union {
        volatile uint64_t SchedOrder;
        volatile uint8_t  SchedOrderEntry[8];
    };

    // Scheduler Slots to hold Kthreads associated with this hardware thread
    struct KThread_t *SchedSlot[ CONFIG_SCHED_SLOTS_PER_HWTHREAD ]; 

    // Dummy Stwcx location
    DummyStwcx dummyStwcx;

    // Count of the number of slots consumed.
    uint64_t SlotsAllocated;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // IPI Message area
    volatile struct IPI_Message_t ipi_message[CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE]; 
#endif                                                                                                                 // FUSEDOS

    // Flags used to indicate modifications to the restored thread context to be performed by the scheduler 
   struct LaunchContextFlags_t launchContextFlags;

    // Fast-path pointer to comm thread if one exists on this hardware thread
    struct KThread_t *pCommThread; 

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Allocate a CommThreadData structure for each interrupt type
    CommThreadData commThreadData[KERNEL_COMM_NUM_INTS];
#endif                                                                                                                 // FUSEDOS

    // KThread which owns the UPC Hardware
    struct KThread_t *pUpci_ActiveKThread;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Per-hardware thread performance counters
    uint64_t PerformanceCounter[CNK_HWTPERFCOUNT_NUM];

    // IPI Message checker expiration
    uint64_t ipiMessageCheckerExpiration;

    // Context pointer of a pending speculation restart
    Regs_t * pendingSpecRestartContext;
#endif                                                                                                                 // FUSEDOS

    // Field indicating which phase of application exit we have reached (phase1 = 1, phase2 = 2)
    volatile uint8_t appExitPhase;

    // Indicator the the kthread priorities have changed requiring regeneration of the SchedOrder object.
    volatile uint8_t priorityRefreshPending;

    // Indicator of the state of the nano timer interrupt enablement
    volatile uint8_t nanoTimerEnabled;

    // Indicator than an process exit IPI interrupt is pending against this hardware thread
    volatile uint8_t IpiExitInterruptPending;

    // Indicator that all software threads on this hardware thread are suspended
    volatile uint8_t suspended;

}
ALIGN_L1D_CACHE HWThreadState_t;


// 0..16
typedef struct CoreState_t
{
    // This Core's Hardware Threads
    struct HWThreadState_t HWThreads[ CONFIG_HWTHREADS_PER_CORE ];
    Lock_Atomic_t coreBarrier; // used for core scope barrier operations.
    Lock_Atomic_t coreLock;    // used to control access to core scoped hardware resources. 
    uint64_t default_l1p_init;
    uint64_t default_l1p_cfgspec;
    uint64_t default_l1p_cfgpfusr;
    uint64_t default_l1p_cfgpfsys;
    
    // Roll-back h/w state for soft-error recovery
    //  this state includes a golden (last committed) state and a change list
    //  the change list is updated on each mtspr(), etc.
}
ALIGN_L2_CACHE CoreState_t;


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__ uint64_t incrementSlotAllocatedCount(HWThreadState_t* phwtState)
{
    register uint64_t old_val, tmp_val;
    ppc_msync();
    do
    {
        old_val = LoadReserved( &(phwtState->SlotsAllocated) );
        tmp_val = old_val + 1;
    }
    while ( !StoreConditional( &(phwtState->SlotsAllocated), tmp_val ) );
    return( old_val );
}

__INLINE__ uint64_t decrementSlotAllocatedCount(HWThreadState_t* phwtState)
{
    register uint64_t old_val, tmp_val;
    ppc_msync();
    do
    {
        old_val = LoadReserved( &(phwtState->SlotsAllocated) );
        if (!old_val) break;  // if value is zero, just return
        tmp_val = old_val-1;
    }
    while ( !StoreConditional( &(phwtState->SlotsAllocated), tmp_val ) );
    return( old_val );
}
#endif                                                                                                                 // FUSEDOS

#endif // __ASSEMBLY__

#endif // __KERNEL__

#endif // Add nothing below this line.

