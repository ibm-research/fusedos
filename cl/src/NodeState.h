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

#ifndef _NODESTATE_H_ // Prevent multiple inclusion.
#define _NODESTATE_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdint.h>

#include <cnk/include/Config.h>

#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/common/bgq_bitnumbers.h>
#include <firmware/include/personality.h>
#include <firmware/include/Firmware_Interface.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <mudm/include/mudm_atomic.h>
#include <mudm/include/mudm_bcast.h>
#endif                                                                                                                 // FUSEDOS

#include "Regs.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "Sched.h"
#endif                                                                                                                 // FUSEDOS
#include "CoreState.h"
#include "KThread.h"
#include <cnk/include/AppState.h>
#include "AppProcess.h"
#include "shm.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "CrcExchange.h"
#endif                                                                                                                 // FUSEDOS

#ifdef __FUSEDOS__                                                                                                    // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#define SHARED_WORKAREA_SIZE 0x60000

typedef struct NodeState_t
{
    // Per-Core state information
    CoreState_t  CoreState[ CONFIG_MAX_CORES ];

    // Per-Process state information
    AppProcess_t AppProcess[ CONFIG_MAX_PROCESSES ];

    // App States that are mapped to each Program. 
    AppState_t AppState[CONFIG_MAX_APPSTATES];

    // Array of _bgp_AppState indexes, index by hardware thread. For HPC operation, each entry will
    // be assigned the same index value (0) to access the same _AppState object. For HTC or MPMD HPC
    // each index could reference a different _AppState object.
    AppState_t *pAppState[ CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE ];

    // Local copy of personality
    Personality_t Personality;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Local copy of Firmware Interface
    Firmware_Interface_t FW_Interface;
#endif                                                                                                                 // FUSEDOS

    // Index of the hardware thread serving as the first application agent
    uint32_t FirstAppAgentProcessorID;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Index of the hardware thread used for network managment
    uint32_t NetworkManagerProcessorID;

    // Index of the node leader thread used for job control and tool support
    uint32_t NodeLeaderProcessorID;
#endif                                                                                                                 // FUSEDOS

    // Kernel domain limits 
    uint32_t NumCoresEnabled;
    Firmware_DomainDescriptor_t DomainDesc;
    
    // Hardware thread assignments
    //
    // Application Leader hardware thread(s). The application leader thread controls 1 to 64 processes
    // that may be associated with the Application. bit0=[hwthread index 0], bit63=[hwthread index 63] 
    uint64_t AppLeaders;
    // Process Leader hardware thread(s). The process leader thread controls 1 64 hardware threads
    uint64_t ProcessLeaders;
    // Hardware threads that are enabled for use as Application Threads
    uint64_t EnabledAppThreads;
    // Hardware threads that are Application agents. bit=0 maps to hwindex=64
    uint64_t AppAgents;

    // The pool of KThreads
    KThread_t KThreads[ NUM_KTHREADS ];

    // Shared/Persistent memory information. Note that we do not support shared/persistent memory in sub-node sub-block jobs
    // therefore we only need one nodal instance of these objects. If we add this support in the future, then
    // we would need an instances of these objects for each AppState object
    ShmMgr_t   SharedMemory;
    ShmMgr_t   PersistentMemory;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Futex state table for shared futexes scoped to the job. Note that we do not currently support
    // shared memory in sub-node sub-block jobs therefore we do not need to maintain a separate
    // futex table for each AppState.   
    Futex_State_t Futex_Table_Shared[ NUM_FUTEX ];

    // Futex state table for private futexes. During process setup, each process will be assigned 
    // a portion of this table, based on the number of kthreads configured to that process.
    Futex_State_t  Futex_Table_Private[ NUM_FUTEX ];

    // MUDUM interface for performing remote atomic load operations to the Job leader node
    struct remoteget_atomic_controls remoteget;
    // MUDM interface for performing direct store operations 
    struct directput_atomic_controls directput;
    // lockword used to implement the node scoped barrier;
    Lock_Atomic_t nodeBarrier;
    // lockwords used to implement the application scoped barriers
    Lock_Atomic_t appBarrier[CONFIG_MAX_APPSTATES];
    // lockword used to order the coredump output in simulation environments
    Lock_Atomic_t coredumpLock;
    // lockword used to barrier the AppLeader with its AppAgents. Since AppAgents
    // are not supported with sub-node subblock jobs, only one lockword within the node is necessary
    Lock_Atomic_t agentAndAppLeaderBarrier;        
     // lockword used to barrier AppAgents
    Lock_Atomic_t agentBarrier;
    // lockwords to manage shared futex operations
    Lock_Atomic64_t FutexLockShared;
    Lock_Atomic64_t FutexOrderLockShared;
#endif                                                                                                                 // FUSEDOS

    // Lockword to manage access to the shared work area
    Lock_Atomic_t sharedDataAreaLock; 
    // Shared work  area
    unsigned char sharedWorkArea[SHARED_WORKAREA_SIZE];

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // A flag to steer core files through JTAG (instead of out through ciod)
    int DumpCoreToMailbox;
    
    unsigned int NumSpecDomains;
    unsigned int RollbackDomain;
    uint64_t*    RollbackIndicator;
    uint64_t     RollbackKernel;
    
    // MUDM context
    void*        MUDM;
#endif                                                                                                                 // FUSEDOS

    // Enable / disable trace categories (which are defined in Trace.h).
    uint64_t TraceConfig;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Unique id of this node for connecting to I/O services.
    uint32_t ServiceId;

    // IP address of bgvrnic device for connecting to I/O services.
    uint32_t ServiceDeviceAddr;

    uint32_t sysiodPortAddressDestination;
    uint32_t toolctldPortAddressDestination;

    // Node-wide performance counters
    uint64_t PerformanceCounter[CNK_NODEPERFCOUNT_NUM];

    // CRC Exchange Data
    CrcExchange_Data_t crcExchange;

    // Torus IP address used by MUDM/CN Verbs
    IP_Addr_t torusIpAddress;

    // Value for nodename field of struct utsname.
    char NodeName[CONFIG_MAX_UTSNAME_SIZE];

    // Value for release field of struct utsname.
    char Release[CONFIG_MAX_UTSNAME_SIZE];

    // Last job leader coordinates
    uint8_t JobLeaderCoords[5];
#endif                                                                                                                 // FUSEDOS
#ifdef __FUSEDOS__							// FUSEDOS
	pthread_mutex_t cl_thread_mutex;		// FUSEDOS
	pthread_cond_t  cl_thread_cond;         // FUSEDOS

	int cl_firstspc;                        // FUSEDOS
	int cl_numspcs;                         // FUSEDOS

	int cl_shm_size;                        // FUSEDOS
#endif										// FUSEDOS

}
ALIGN_L2_CACHE NodeState_t;

//----------------------------------------------------------------
// Inlines
//----------------------------------------------------------------

extern NodeState_t NodeState;

__INLINE__ Personality_t *GetPersonality( void )
{
    return &(NodeState.Personality);
}

// Return the AppState object associated with the currently running hardware thread.
__INLINE__ AppState_t * GetMyAppState( void )
{
    return(NodeState.pAppState[ProcessorID()]);
}
// Return the AppState object associated with the currently running hardware thread.
// If the current thread is an Application Agent, return the AppState object of application, not the agent
__INLINE__ AppState_t * GetParentAppState( void )
{
    AppState_t *app = GetMyAppState();
    if (app && app->parentAppState)
    {
        return (AppState_t *)(app->parentAppState);
    }
    return app;
}

// Return the home Process object associated with the specified processor ID (0..67).
__INLINE__ AppProcess_t *GetProcessByProcessorID( int processorID )
{
    int procIndex = NodeState.pAppState[processorID]->ProcStateIndex[processorID];
    if (procIndex < 0) return NULL;
    return &(NodeState.AppProcess[procIndex]);
}


#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS
extern pthread_key_t spc_key;                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS

// Returns kernel thread object for the current hardware thread.
__INLINE__ KThread_t *GetMyKThread( void )
{
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    int spc = (int)(*(int*)(pthread_getspecific(spc_key)));                                                            // FUSEDOS
    return &(NodeState.KThreads[spc]);                                                                                 // FUSEDOS
#else                                                                                                                  // FUSEDOS
    HWThreadState_t *pHWT = (struct HWThreadState_t *)mfspr( SPRG_pHWThread );
    return (pHWT->pCurrentThread);
#endif                                                                                                                 // FUSEDOS
}

// Return the AppProcess object associated with the currently running kthread.
__INLINE__ AppProcess_t *GetMyProcess( void )
{
    return(GetMyKThread()->pAppProc);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Returns true if the current hardware thread is the host of the process leader kthread. 
__INLINE__ int IsProcessLeader( void )
{
    int processorID = ProcessorID();
    if (processorID < CONFIG_MAX_APP_THREADS)
        return ((NodeState.ProcessLeaders & _BN(processorID)) ? 1 : 0);
    else
        return ((NodeState.AppAgents & _BN(processorID-CONFIG_MAX_APP_THREADS)) ? 1 : 0);
}
#endif                                                                                                                 // FUSEDOS

// Returns true if the current hardware thread is the host of the applicaton leader kthread.
__INLINE__ int IsAppLeader( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int processorID = ProcessorID();
    if (processorID < CONFIG_MAX_APP_THREADS)
        return ((NodeState.AppLeaders & _BN(processorID)) ? 1 : 0);
    else
        return ((NodeState.AppAgents & _BN(processorID-CONFIG_MAX_APP_THREADS)) ? 1 : 0);
#else                                                                                                                  // FUSEDOS
    return (1);                                                                                                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Returns core state by core ID (0..16).
__INLINE__ CoreState_t *GetCoreStateByCore( int coreID )
{
    return &(NodeState.CoreState[coreID]);
}

// Returns core state by processor ID (0..67).
__INLINE__ CoreState_t *GetCoreStateByProcessorID( int processorID )
{
    return(GetCoreStateByCore( processorID>>2 ));
}
#endif                                                                                                                 // FUSEDOS

// Returns hardware thread state for the specified core ID and hardware thread ID.
__INLINE__ HWThreadState_t *GetHWThreadState( int coreID, int threadID )
{
    return &(NodeState.CoreState[coreID].HWThreads[threadID]);
}

// Returns hardware thread state object for the specified processor ID (0..67).
__INLINE__ HWThreadState_t *GetHWThreadStateByProcessorID( int processorID )
{
    return &(NodeState.CoreState[processorID/CONFIG_HWTHREADS_PER_CORE].HWThreads[processorID % CONFIG_HWTHREADS_PER_CORE]);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Returns hardware thread state object for the current hardware thread.
__INLINE__ HWThreadState_t *GetMyHWThreadState( void )
{
    HWThreadState_t *pHWT = (struct HWThreadState_t *)mfspr( SPRG_pHWThread );
    return pHWT;
}
#endif                                                                                                                 // FUSEDOS

// Returns PID of the current kthread.
__INLINE__ uint32_t GetPID()
{
    AppProcess_t* proc = GetMyProcess();
    if (proc)
    {
        return proc->PID;
    }
    return 0;
}

__INLINE__ int GetProcessLeaderProcessorID( void )
{
    AppProcess_t* proc = GetMyProcess();
    if (proc)
    {
        return proc->ProcessLeader_ProcessorID;
    }
    return -1;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Convert a core ID and hardware thread ID to a processor ID.
__INLINE__ int GetProcessorID( int coreID, int threadID )
{
    return(coreID*CONFIG_HWTHREADS_PER_CORE + threadID);
}
#endif                                                                                                                 // FUSEDOS

// Return the thread id associated with this kthread object
__INLINE__ int GetTID(KThread_t *pKThread)
{
    // use the index into the KThread as the basis for the tid. Add 1 to make the lowest tid non-zero
    return((((uint64_t)pKThread - (uint64_t)&NodeState.KThreads[0])/sizeof(KThread_t))+1);
}

// Return the KThread associated with a thread id. This function is used by system call interfaces
// that supply an pid/tid. If supplied tid is 0, using current tid as per syscall convention.
__INLINE__ KThread_t* GetKThreadFromTid(int tid)
{
    KThread_t* kthread = NULL;
    if (!tid)
      return(GetMyKThread());

    else if (tid > 0 && tid <= NUM_KTHREADS)
    {
       kthread = &(NodeState.KThreads[tid-1]);
       if ((kthread->State & SCHED_STATE_FREE) ||   //  Is this kthread not yet assigned
           (kthread->SlotIndex == CONFIG_SCHED_KERNEL_SLOT_INDEX)) // Is this kthread the special kernel thread
       {
           return NULL; // This kthread is not assigned to a pthread
       }
    }
    return kthread; 
}


// Generate a string identifying current thread of execution.

__INLINE__ char *whoami(void)
{
    KThread_t *kthread = GetMyKThread();
    char *buf = kthread->whoami;
    snprintf(buf, sizeof(kthread->whoami), "[%d.%d:%u]", kthread->ProcessorID>>2, (kthread->ProcessorID) & 0x03, GetTID(kthread));
    return buf;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__ void CountException(uint64_t ctr)
{
    GetMyHWThreadState()->PerformanceCounter[ctr]++;
}

__INLINE__ void CountNodeEvent(uint64_t ctr)
{
    NodeState.PerformanceCounter[ctr]++;
}

#endif                                                                                                                 // FUSEDOS

__INLINE__ int IsAppAgent( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    return(((uint32_t)ProcessorID() < NodeState.FirstAppAgentProcessorID) ? 0 : 1);
#else                                                                                                                  // FUSEDOS
	return 0;                                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS

// Returns the following values
//   0: not an app agent 
//   1: AppAgent1
//   2: AppAgent2  
__INLINE__ uint32_t AppAgentIdentifier( AppProcess_t *proc )
{
    if ((uint32_t)proc->ProcessLeader_ProcessorID >= NodeState.FirstAppAgentProcessorID)
    {
        return ((uint32_t)proc->ProcessLeader_ProcessorID - NodeState.FirstAppAgentProcessorID + 1);
    }
    return 0;
}


__INLINE__ AppProcess_t *GetFirstProcess(AppState_t *app)
{
    return (&(NodeState.AppProcess[cntlz64(app->ProcessLeaderMask)]));
}

// Returns indicator of whether a process is currently active on the current kthread. A 
// process in the ExitPending state or Rank Inactive state is not considered active.
__INLINE__ int IsMyProcessActive()
{
    AppProcess_t *proc = GetMyProcess();
    if (proc && (proc->State >= ProcessState_Active))
    {
        return 1;
    }
    return 0;
}

// Returns indicator of whether a process associated with the specified kthread is active. A 
// process in the ExitPending state or Rank Inactive state is not considered active.
__INLINE__ int IsProcessActive(KThread_t *kthread)
{
    AppProcess_t *proc = kthread->pAppProc;
    if (proc && (proc->State >= ProcessState_Active))
    {
        return 1;
    }
    return 0;
}


__INLINE__ int IsSubNodeJob()
{
    return(GetParentAppState()->jobLeaderData.NodesInJob ? 0 : 1);
}
#endif                                                                                                                 // FUSEDOS


#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.

