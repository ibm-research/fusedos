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
#ifndef CONFIG_H_ // Prevent multiple inclusion.
#define CONFIG_H_
 
#include <hwi/include/common/compiler_support.h>
 
// BGQ Node hardware configuration
#define CONFIG_HWTHREADS_PER_CORE           (4)   // Hardware threads per node
#define CONFIG_MAX_CORES                    (17)  // Maximum number of cores in the node
#define CONFIG_MAX_HWTHREADS                (CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE)
#define CONFIG_MAX_CORES_MASK               (0xFFFF8000) // Full core mask for all possible cores in the node
#define CONFIG_MAX_APP_CORES                (16)  // Reserve 17th core for System Use. 
#define CONFIG_MAX_APP_THREADS              (CONFIG_MAX_APP_CORES * CONFIG_HWTHREADS_PER_CORE) 
#define CONFIG_MAX_APP_PROCESSES            (CONFIG_MAX_APP_CORES * CONFIG_HWTHREADS_PER_CORE)
#define CONFIG_MAX_PROCESSES                (CONFIG_MAX_APP_PROCESSES + CONFIG_MAX_APP_AGENTS) 
#define CONFIG_MAX_APP_APPSTATES            (CONFIG_MAX_APP_CORES)                         // max appstate objects for Application use
#define CONFIG_MAX_APPSTATES                (CONFIG_MAX_APP_CORES + CONFIG_MAX_APP_AGENTS) // max appstate objects for both App and Agent use

// Application Agents
#define CONFIG_MAX_APP_AGENTS               (2) // The max number of application agents allowed to run on the reserved core
#define CONFIG_FIRST_AGENT_APPSTATE_INDEX   CONFIG_MAX_APP_APPSTATES
#define CONFIG_AGENT_HEAPSIZE_DEFAULT       14  // MBytes allocated for the application agent's combined heap and stack

// SCHEDULER 
#define CONFIG_AFFIN_SLOTS_PER_HWTHREAD  (5)  // Number of affinitiy slots per hardware thread
#define CONFIG_SCHED_SLOTS_PER_HWTHREAD ( CONFIG_AFFIN_SLOTS_PER_HWTHREAD + 1 ) // affinity slots plus kernel thread
#define CONFIG_SCHED_SLOT_FIRST          (0)  // Slot index of first AFFIN slot (process leader or pthread_create)
#define CONFIG_SCHED_KERNEL_SLOT_INDEX  ( CONFIG_AFFIN_SLOTS_PER_HWTHREAD) // Last slot is reserved for Kernel thread
#define CONFIG_SCHED_BREADTH_LAYOUT      (1)  // indicate a BREADTH layoutof harware threads in the process (core0thread0, core1thread0, ...)
#define CONFIG_SCHED_DEPTH_LAYOUT        (2)  // indicate a DEPTH layout of hardware threads in the process (core0thread0, core0thread1, ...)
#define CONFIG_SCHED_MAX_FIFO_PRIORITY   (98) // this is the "most favored" priority. Higher numbers are higher importance threads
#define CONFIG_SCHED_MIN_FIFO_PRIORITY   (2)  // this is the "least favored" priority. Lower numbers are for threads of lower importance
#define CONFIG_SCHED_MAX_COMM_PRIORITY   (99) // this is the "most favored" priority. Higher numbers are higher importance threads
#define CONFIG_SCHED_MIN_COMM_PRIORITY   (1)  // this is the "least favored" priority. Lower numbers are for threads of lower importance
#define CONFIG_SCHED_NUM_NOSCHED_HWTHREADS (2) // The number of hardware threads in the node that will never enter the scheduler (special purpose hwthreads)

// Thread model
#define CONFIG_THREAD_MODEL_LTM  (0)  // Livermore threading model
#define CONFIG_THREAD_MODEL_BGP  (1)  // BGP style thread model. One application/omp pthread per hardware thread
#define CONFIG_THREAD_MODEL_ETA  (2)  // Enhanced thread affinity model
#define CONFIG_THREAD_MODEL_DEFAULT CONFIG_THREAD_MODEL_LTM

#define CONFIG_TRACE_ENABLE              (1)

#define CONFIG_CNK_PAGE_SIZE (1024 * 1024)     // Pagesize with respect to TLB boundaries
#define CONFIG_MEM_PAGE_SIZE (4096)            // Pagesize with respect to user-visible boundaries/alignment

#define CONFIG_STANDALONE_APPSTATE        ( 0x80000000 )
#define CONFIG_STANDALONE_TLBMAPPER       ( 0x80010000 )
#define CONFIG_MEMFS_BASEADDR             ( 0xC0000000 )
#define CONFIG_STANDALONE_APPSTATE_L2     ( 0x00F00000 )
#define CONFIG_STANDALONE_TLBMAPPER_L2    ( 0x00F10000 )

#define CONFIG_GLOBAL_BASE       0x0000007000000000ul
#define CONFIG_APP_L2ATOMIC_BASE 0x0004000000000000ul
//
// The addresses below are carefully chosen:
//     1)  They're above the L2-atomic virtual space and don't interfere with
//         other things.
//     2)  They land in TLB sets that aren't otherwise used for kernel
//         mappings.
//     3)  Both addresses can be instantiated with just three instructions:
//             lis     rA,0x0150
//             rldimi  rA,rA,28,13
//             addis   rB,rA,0x0010
//
#define CONFIG_FAST_L1P_BASE     0x0005000001500000ul
#define CONFIG_FAST_L2_BASE      0x0005000001600000ul

// Core Dump control
#define CONFIG_CORE_MAX_STKTRC_DEPTH (32)
#define CONFIG_CORE_BUFFER_SIZE  0x10000 /* 64K byte buffer */
// Control the scope of a core dump to either the entire process or to just the failing thread
#define CONFIG_CORE_SCOPE_PROCESS 0
#define CONFIG_CORE_SCOPE_THREAD  1
#define CONFIG_CORE_SCOPE_DEFAULT CONFIG_CORE_SCOPE_PROCESS  

#define CONFIG_TICK_RATE (100)

//  This enables posting flightlog data in the IPI path.  Better debug/fail info vs. performance.
#define CONFIG_IPI_FLIGHTLOG 1

//  This amount of memory is subtracted from the heap and dedicated to the
//  main() thread's stack.  Note that the main() thread's stack can still
//  grow beyond this point.
#define CNA_MAIN_THREAD_STACK_RESERVED (4 * 1024 * 1024)

// Default size for the guard page of the process leader thread. If this
// value is set to zero, then page guarding on the process leader
// will be disabled. The default size of 4K was chosen because
// it is the pthread's default guard page size.
#define CNK_MAIN_THREAD_GUARD_SIZE (4 * 1024)

// Minimum allowed guard size (in bytes). This size was choosen to be greater than
// the size of the smallest stack frame
#define CNK_MIN_GUARD_SIZE (512)

// Default value for enablement of stack guard area
// 0 : all guard paging for process leader and pthreads is disabled.
// 1:  allow debugger to disable guarding 
// 2:  guarding always enabled
#define CNK_STACK_GUARD_ENABLE_DEFAULT (0)

// Starting WAC register for use in stack gaurding. This register value
// plus the next 3 consecutive registers will be used for 
// thread ids 0,1,2,3 respectively. There are 12 WAC registers, 0 - 11
// that must be shared by wakeup SPIs, performance tools (UPC), and stack guarding
// WACs 10,11 signals are sent to the UPC.
#define CNK_STACK_GUARD_FIRST_WAC (4)
// Starting WAC register for use in wakeup SPIs. Kernel will enable this WAC and  
// plus the next 3 consecutive registers for use by the WAKEUP SPI. This cannot
// conflict with the  definition for the Stack Guard.
#define CNK_WAKEUP_SPI_FIRST_WAC  (0)

// Allow reading from NULL pointer from within CNK kernel
#define CNK_ALLOW_KERNELNULLREAD  (0)

// Default value for CNK creating shared text regions (non-shared text is experimental)
#define CNK_SHAREDTEXT_DEFAULT  (1)

// Number of file descriptors per hardware thread.  
// Note:  CNK_MAX_FDS*CONFIG_MAX_APP_PROCESSES should match the ionode's sysio ulimit.
// Note2: Fortran runtime allows up to 2000 open files.
#define CNK_MAX_FDS           (128)

#define MMAP_MAX_ALLOCS      (1096)

// Define the number of shm_opens of different file handles.  Currently capped at 16 per process
// NOTE: A file handle is a different named file in the shm_open() invocation
#define SHM_MAX_OPENS (CONFIG_MAX_PROCESSES*16 + 16)

#define NUM_COMMTHREAD_GPRS   4    // Number of parameters that will be passed to CommThread

#define MALLOC_ARENASIZE  512*1024
#define MALLOC_ALIGNMENT  128

#define CNK_STATICSHMATOMICSIZE 32

// Maximum number of protection domains on a node.
#define CNV_MAX_PD (8)

// Maximum number of memory regions on a node.
#define CNV_MAX_MR (128)

// Maximum number of completion queues on a node.
#define CNV_MAX_CQ (8)

// Maximum number of work completions in a completion queue.
#define CNV_MAX_WC (128)

// Maximum number of queue pairs on a node.
#define CNV_MAX_QP (8)

// Maximum number of work requests.
#define CNV_MAX_WR (1)

// Maximum number of scatter/gather elements per work request.
#define CNV_MAX_SGE (5)

// Enable MUDM usage of the wakeup unit during poll
#define CNK_ENABLE_MUDM_WAKEUP_POLL (1)

// Default value for CCR3[EEPRI]
#define CNK_EEPRI_DEFAULT 1

// Default enablement for collective job load
#define CNK_COLLECTIVE_APPLOAD 1

// QPX NAN and INFINITY exception enablement
#define CONFIG_QPX_NAN_EXCEPT 1
#define CONFIG_QPX_INF_EXCEPT 1

#define CONFIG_TIMER_NANOSLEEP_PREEMPT 100

// Maximum number of bytes for fields in struct utsname used by uname system call.
#define CONFIG_MAX_UTSNAME_SIZE (65)

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Enable sub-block ND privacy fence
#define CONFIG_ND_PRIVACY_FENCE 1
#endif                                                                                                                 // FUSEDOS

// Default Shared mem size
#define CONFIG_DEFAULT_SHAREDMEMSIZE      64
#define CONFIG_DEFAULT_MIN_SHAREDMEMSIZE  32

// IPI message checker interval in microseconds
#define CONFIG_IPIMESSAGECHECKER_INTERVAL 20000

// Default maximum number of abnormally terminating nodes that will generate core files
#define CONFIG_COREDUMPMAXNODES_DEFAULT 2048

// Default max time in seconds from SIGKILL to allow coredumps to proceed
#define CONFIG_COREDUMP_WINDOW  50

// Default number of concurrently writing coredumps with the job
#define CONFIG_COREDUMP_CONCURRENT 256

// Maximum number of mmap'd files with tracked filenames per process
#define CONFIG_NUM_MAPPED_FILENAMES 64

// Avoid readlink for mmap filename tracking
#define CONFIG_AVOID_READLINK 1

// Maximum size of procFS storage
#define CONFIG_PROCFS_SIZE  512*1024   // 512kB

// Print size of the file system classes to the mailbox upon boot
#define CONFIG_PRINTFSSIZE 0

// Total number of kthreads supported
#define NUM_KTHREADS ( CONFIG_SCHED_SLOTS_PER_HWTHREAD * CONFIG_HWTHREADS_PER_CORE * CONFIG_MAX_CORES )

// Should never need more futex table entries than the number of kthreads.
#define NUM_FUTEX (NUM_KTHREADS)

// Enable for using a distributed MPI mapfile
#define CONFIG_DISTRO_MAPFILE 1

// Default size of the MPI mapfile chunk storage on each node
#define CONFIG_MAPFILECHUNKSIZE (128*1024)

// Number of simultaneous mapping file reads
#define CONFIG_MAPFILETRACKS  4

// Size of the mapfile buffer used for processing
#define CONFIG_MAPFILESTORAGE (CONFIG_MAPFILECHUNKSIZE * CONFIG_MAPFILETRACKS * 2)

// Number of concurrent readers of a chunk
#define CONFIG_MAPFILEREMOTEPACING 2

// Always allow printf to console
#define CONFIG_ALLOWPRINTF 0

// MUDM connect timeout in seconds
#define CONFIG_MUDMCONNECTTIMEOUT (3*60)

// Enable implicit side-effect for shared filename "l2atomic", triggers file to be L2 atomic capable w/o ioctl call.
#define CONFIG_SHMFILEIMPLICITL2ATOMIC  0

#endif // Add nothing below this line.
