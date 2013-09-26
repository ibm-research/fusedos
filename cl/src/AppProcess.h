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
#ifndef _APPPROCESS_H_ // Prevent multiple inclusion.
#define _APPPROCESS_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)


#include <cnk/include/Config.h>
#include <cnk/include/klibtypes.h>
#include "Regs.h"
#include "KThread.h"
#include "CoreState.h"
#include "util.h"
#include "shm.h"
#include <sys/resource.h>
#include <cnk/include/AppState.h>

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

//! Number of futex table entries. Will never need more table entries than there are kthreads to block on them
#define HWTLISTMGRENTRIES ( CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE * CONFIG_SCHED_SLOTS_PER_HWTHREAD)

// Isolate the Exit return code value and the signal value from the Process Exit Status field.
#define EXITSTATUS_SIGNAL(x)  ( x & 0x7F )
#define EXITSTATUS_RCODE(x) ( (x >> 8) & 0xFF  )

typedef struct HwtRecycleList_Mgr_t
{
    Lock_Atomic_t lock;
    uint64_t threadLayout_count;
    uint64_t layout_type;
    uint32_t indexHead;
    uint32_t indexTail;
    uint8_t hwtindex[HWTLISTMGRENTRIES]; 
} ALIGN_L1D_CACHE HwtRecycleList_Mgr_t;


typedef struct FutexState_t
{
    Futex_t* futex_vaddr;

    // Queue of waiters on this futex
    //  Note that there is rarely ever more than 1 waiter,
    //  and even then, not in performance critical code,
    //  so a singly-directed queue is ok.
    KThread_t *pKThr_Waiter_Next;
}
Futex_State_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// A KernelInfo structure is appended to the application's text segment.
// It can be used to make available to the application any miscellaneous
// read-only information.
typedef struct
{
    SignalTrampoline_t SigTramp;
}
KernelInfo_t;
#endif                                                                                                                 // FUSEDOS


typedef struct
{
    int fd;
    int isShar;
    char filename[64];
    uint64_t vaddr;
    uint64_t off;
    uint64_t size;
}
KernelMappedFiles_t;

// We reserve a fixed amount of space in the text segment for the
// KernelInfo structure.  Must be a power of 2.  Also, must be at least 4KB
// so the structure remains outside the mmap'd text segment in dynamically
// linked programs.
#define KERNEL_INFO_SPACE 0x1000

enum {
   Bg_StackGuardEnable_LowValue = 0,    // Low end of the valid range 
   Bg_StackGuard_Disabled = 0,          // stack guarding is disabled
   Bg_StackGuard_Enabled  = 1,          // allow debugger to disable guarding if it wants to set debug registers
   Bg_StackGuardEnable_HighValue = 1    // Upper end of valid range
};

enum ProcessState {
    ProcessState_Free = 0,      // Process object is not in use
    ProcessState_ExitPending,   // Process has begun to exit
    ProcessState_Active,        // Process object belongs to an active rank in the block. Process has not yet started to run
    ProcessState_RankInactive,  // Process object belongs to an inactive rank within the block
    ProcessState_AgentInactive,  // Process object belongs to an inactive agent
    ProcessState_Started        // The main thread of the process has started to execute
};

enum LoadState {
    AppState_Uninitialized = 0, // Cleared, not initialized for any job.
    AppState_Initialized,       // Initialized for use by a Job.
    AppState_Loading,           // Transitional state, load of the application is in progress.
    AppState_LoadFailed,        // A failure during the load operation was detected by CNK.
    AppState_LoadComplete,      // The application has been sucessfully loaded into all nodes of the job.
    AppState_StartIssued        // A start message operation was initiated.
};

enum JobLeaderCounter {
     JobExit_NodeAvailableCount,
     JobExit_AbnormalProcessExitCount,
};

enum AppExitPhase {
    AppExit_Inactive = 0,
    AppExit_Phase1,
    AppExit_Phase2
};


typedef struct AppProcess_t
{
    // Memory segmentation/map info
    //
    //  Note: Decide if we should support multiple copies of code seg to
    //        ease L2 slice pressure in the case of, e.g., 64-way SMP app.
    //        (If desired, app would set an environment variable to enable that.)

    // Track available and in-use hardware threads that are available to this process
    // This is used whenever a hardware thread is assigned or freed from a KThread
    HwtRecycleList_Mgr_t HwtRecycleListMgr;

    // Is the State of this Process
    volatile int State;

    // Tcoord 0..63. Numbered sequencially from 0 to the num processes configured in the node/job
    int32_t  Tcoord;
    
    // PhysicalPID is the A2 processor PID value (for mtspr(SPRN_PID, value))
    uint64_t PhysicalPID;
    
    // Process Indentifier (PID) and Thread Group Indentifier TGID 
    uint32_t PID; 

    // Rank of this process within the block
    uint32_t Rank;

    // Pointer to parent AppState object
    AppState_t *app;

    // Pointer to the next AppProcess object in this Job
    struct AppProcess_t *nextProcess;

    // ProcessorID of the process leader
    int ProcessLeader_ProcessorID;

    // Request to core dump this specific rank
    uint8_t coreDumpRank; 
    
    // KThread of the process leader
    KThread_t *ProcessLeader_KThread;
    // hwthread mask of hwthreads in this process
    uint64_t HwthreadMask;  // bit significant for hwthread 0 through 63.
    // hwthread mask of remote pthread_setaffinity targets
    uint64_t HwthreadMask_Remote; 

    // HWThread Count: how many hardware threads are in this process
    int      HWThread_Count;
    // The active thread model.
    int      ThreadModel;
    // Heap_Break: current Heap Break Value
    //  always >= Heap_Start and <= Heap_End
    //  protected by critical section and lockbox
    uint64_t          Heap_Break;
    // Lower bound for the heap break operations. Defines the start of the managed heap
    uint64_t          Heap_Start;
    // Upper bound for heap break. Defines end of the managed heap and start of mmap area  
    uint64_t          Heap_End;    
    // Start of the MMap area. Also used as indicator to init MMap environment
    volatile uint64_t MMap_Start;
    // End of the MMap area available for allocation.
    uint64_t          MMap_End;
    // Guard High Mark: current hightest allocated storage address for both MMap and Heap
    uint64_t Guard_HighMark;
    // Size reserved for main thread stack.  in bytes.
    uint64_t MainStackReserve;
    
    // Start of the guard page for the process leader's stack
    uint64_t Guard_Start;
    // Indicator for enablement of Stack Guarding
    // 0: stack guarding disabled
    // 1: allow debugger to disable guarding if it wants to set debug registers
    // 2: stack guarding enabled. debugger hardware watches/breakpoints disabled
    uint64_t Guard_Enable;
    // Size of the guard page for the process leader's  stack
    uint32_t Guard_Size;

    // Virtual Address Bounds for Text/Data/Heap/Shared Segments,
    uint64_t Text_VStart,
    Text_VEnd,
    Data_VStart,
    Data_VEnd,
    Heap_VStart,
    Heap_VEnd,
    Shared_VStart,
    Shared_VEnd,
    TLS_VStart,
    TLS_VEnd,
    DYN_VStart,
    DYN_VEnd;

    // Physical Address Bounds for Text/Data/Heap/Shared Segments,
    uint64_t Text_PStart,
    Text_PEnd,
    Data_PStart,
    Data_PEnd,
    Heap_PStart,
    Heap_PEnd,
    Shared_PStart,
    Shared_PEnd,
    TLS_PStart,
    TLS_PEnd,
    DYN_PStart,
    DYN_PEnd,
    DMA_Lower,
    DMA_Upper;
    
    uint64_t Proc_EntryPoint;
    uint64_t interpreterBase;
    
    // User address of KernelInfo structure appended to text segment.
    uint64_t KernelInfoAddress;

    // Remember TLS TBSS and TData addresses and size
    //  On thread create, each thread's TLS areas must be (re)initialized.
    void     *pTData_Start;
    uint32_t TData_Size;
    void     *pTBSS_Start;
    uint32_t TBSS_Size;
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    const SpecState_t* SpecState;
#endif                                                                                                                 // FUSEDOS
    
    MmapMgr_t MmapMgr;
    // mprotect manager
    MprotectMgr_t MprotectMgr;
    
    // Local Disk FS
    ShmMgr_t LocalDisk;
    
    // file descriptors
    CNK_Descriptors_t App_Descriptors;
    
#if CONFIG_AVOID_READLINK
    // hack around readlink issue
    int LAST_OPEN_FD;
    char LAST_OPEN_FILENAME[APP_MAX_PATHNAME];
#endif
    
    // Current working directory
    char CurrentDir[ APP_MAX_PATHNAME ];
    
    // Current umask value
    mode_t CurrentUmask;
    
    // List of files that have been mmap'd into the process address space
    KernelMappedFiles_t mappedFiles[CONFIG_NUM_MAPPED_FILENAMES];
    
    // Futex state table for PRIVATE futexes scoped to the process. 
    Futex_State_t Futex_Table_Private[NUM_FUTEX];
    
    // Address of stored AuxVecs. This points into the area at the bottom of the stack where the aux vecs were stored
    // at main thread start. The debugger can ask for these vectors.
    uint64_t *pAuxVectors;

    // Number of signals received by the process.
    uint32_t signalCount;

    // Core dump control
    uint32_t coreDumpOnExit;
    KThread_t* coredump_kthread;
    uint8_t  binaryCoredump; // 0:not binary, 1:explicit request for binary on this rank, 2:requested binary on all ranks

    // Flags for dynamic library support
    uint8_t sharedObjectFile;  //!< True if this is a shared object application invocation
    uint8_t straceEnabled;     //!< True if system call trace is enabled in the process.

    // Tool control
    int8_t     propogateWatchpoints;  // Is the tool requesting that we propogate watchpoints on clone
    int8_t     toolAttached;          // Is a tool currently attached to this process
    int8_t     toolAttachSignal;      // The signal to be used in the notify message when tool attaches 
    int8_t     continuationSignal;    // The new signal that will be set when a notify condition is satisfied by the tool
    int8_t     pendingAction;         // Field indicating which action is pending
    KThread_t* notifiedKThread;       // The last thread that has notified the tool
    KThread_t* actionPendingKthread;  // The thread that is the target of a pending tool action
    uint32_t   toolIdAttachedAtStart; // The tool that wishes to attach before the job starts


    // MSR constraints
    uint64_t msrRequired;
    uint64_t msrForbidden;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // signal management
    SignalData_t SignalData[KERN_NSIG];
#endif                                                                                                                 // FUSEDOS

    // Process Exit Status
    //   If the process exits without a signal, the exit value will be in the third byte of the 4 byte field.  
    //   If the process exits due to a signal, the signal number is stored in the 4th byte of the field. 
    uint32_t ExitStatus;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // lockword used for process scoped barrier
    Lock_Atomic_t processBarrier;
    // lockword use for MMap operations
    Lock_Atomic_t MMapLock;
    // lockword to manage the Mprotect manager object
    Lock_Atomic_t MprotectLock;
    // lockword to manage stack guarding
    Lock_Atomic_t GuardAreaLock;
    // lockword to control core dumps
    Lock_Atomic_t DumpCoreLock;
    // lockword to manage signal delivery
    Lock_Atomic_t SignalLock;
    // lockword to manage App_Descriptors object
    Lock_Atomic_t DescriptorTableLock;
    // lockwords to manage private futex operations
    Lock_Atomic64_t FutexLockPrivate;
    Lock_Atomic64_t FutexOrderLockPrivate;
    // lockwords to manage process suspend/resume operations
    Lock_Atomic64_t NotifyControlMask;
    Lock_Atomic_t   suspendResumeLock;
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    pthread_mutex_t MMap_mutex;                                                                                        // FUSEDOS
    pthread_mutex_t Mprotect_mutex;                                                                                    // FUSEDOS
    pthread_mutex_t DescriptorTable_mutex;                                                                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Process-wide performance counters
    uint64_t PerformanceCounter[CNK_PROCPERFCOUNT_NUM];
#endif                                                                                                                 // FUSEDOS

    // Limit on alignment exceptions that will be handled before raising signals
    uint64_t AlignmentExceptionMax;
    
    struct rlimit resourceLimits[RLIMIT_NLIMITS]; //!< Current resource limits
    
    // Are Get/SetDebugReg() syscalls allowed?
    uint64_t DebugRegSyscallsEnabled;
} AppProcess_t;

// Declarations
void __NORETURN App_AgentExit(int phase);
void            App_ClearMemory();
void            App_Exit(int phase, int noReturn);
AppState_t *    App_GetAppFromJobid(uint32_t jobid);
uint32_t        App_GetEnvString(const char* envname, const char** value);
uint32_t        App_GetEnvValue(const char* envname, uint32_t* value);
uint32_t        App_GetEnvValue64(const char* envname, uint64_t* value);
void            App_Load();
void            App_LoadAgent();
void            App_ProcessExit( uint32_t status);
void            App_Reset();
void            App_SetLoadState(uint32_t loadState, uint32_t returnCode, uint32_t errorCode);
int             App_SignalJob(uint32_t jobid, int signum);
#if defined __FUSEDOS__                                                                                               // FUSEDOS
void            App_ThreadExit( int status, KThread_t *kthread );                                                                          // FUSEDOS
#else                                                                                                                  // FUSEDOS
void __NORETURN App_ThreadExit( int status, KThread_t *kthread );
#endif                                                                                                                 // FUSEDOS
int             App_Start(AppState_t *appState);
int             App_RunPreLoadedApp(void);
void            App_ReportProcessExit(AppProcess_t *proc);
int             App_ReportJobExit();
int             App_IsJobLeaderNode();
int             App_IsLoadLeaderNode();
void            App_RegisterNodeAvailable();
uint64_t        App_RegisterAbnormalProcessExit();
void            App_SavePersistent();
int             App_IsCollectiveLoadActive();
void            App_ActivateCollectiveLoad();
void            App_RemoteThreadExit( AppProcess_t *proc);
int             App_DumpCorePacingBegin();
int             App_DumpCorePacingEnd();

int             Guard_Adjust( uint64_t high_mark, MoveGuardDirection_t direction, int ipiRedrive );
void            Guard_Prepare(void *original_guardbase, void *new_guardbase, int size);
void            Guard_Reset();

void            Process_DumpThreadInfo(AppProcess_t *pAppProc);
KThread_t*      Process_GetAvailableKThread(AppProcess_t *pAppProcess);
void            Process_Init();
void            Process_InitializeThreadLocalStorage( AppProcess_t *pProc, void *pTLS );
void            Process_MakeHwThreadAvail(AppProcess_t *pAppProcess, int hwthread_index);
void            Process_MakeHwThreadAvail(AppProcess_t*,int);
void            Process_SetStackGuard();
int             Process_SetupValidate(int appLeaderCoreIndex, int numProcesses, int numCores);

extern int ProcFS_GenMaps(AppProcess_t* proc);

#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.

