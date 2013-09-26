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
#include "ctype.h"
#include "rank.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/wu_mmio.h>

#define JOBLEADERENABLED 1
#endif                                                                                                                 // FUSEDOS

using namespace bgcios::jobctl;


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#define MAX(a,b) ((a>b)?a:b)

uint32_t GetEnvString(struct LoadJobMessage *msg, const char* envname, char** value);
uint32_t GetEnvValue(struct LoadJobMessage *msg, const char* envname, uint32_t* value);
uint32_t GetEnvValue64(struct LoadJobMessage *msg, const char* envname, uint64_t* value);

// Default comm app agent
static const char *default_comm_agent = "/bgsys/drivers/ppcfloor/agents/bin/comm.elf";

KThread_t *GrabFreeKThread(AppProcess_t *proc, int processorID)
{
    KThread_t* pKThr = NULL;
    HWThreadState_t *pHwt = GetHWThreadStateByProcessorID(processorID);
    int affinity_slots = CONFIG_AFFIN_SLOTS_PER_HWTHREAD;
    // Are we preventing over commitment of application threads on a hardware thread?
    if (proc->ThreadModel == CONFIG_THREAD_MODEL_BGP)
    {
	// If we have a comm thread, allow an app thread to co-exist with it.
	affinity_slots = (pHwt->pCommThread ? 2 : 1);
    }
    // Search the list of kthread slots and attempt to aquire a kthread
    if (pHwt->SlotsAllocated < (uint64_t)affinity_slots)
    {
	Kernel_Lock(&pHwt->migrationLock);
	// Refresh the affinity_slots field now that we have the lock
	if (proc->ThreadModel == CONFIG_THREAD_MODEL_BGP)
	{
	    affinity_slots = (pHwt->pCommThread ? 2 : 1); 
	}
	for (int sched_slot=0; sched_slot < affinity_slots; sched_slot++)
	{
	    KThread_t *candidate_kthread = pHwt->SchedSlot[sched_slot];
	    uint32_t *pState = &(candidate_kthread->State);
	    uint32_t oldState, newState;
	    // if state is FREE, change it atomically to RESET,
	    // preserving any other bits (e.g. SUSPEND).
	    do
	    {
		oldState = LoadReserved32(pState);
		if (!(oldState & SCHED_STATE_FREE) ||
		    (oldState & SCHED_STATE_RESERVED)) goto trynext;
		newState = (oldState & ~SCHED_STATE_FREE) | SCHED_STATE_RESET;
	    } while (!StoreConditional32(pState, newState));
	    // we succeeded in allocating candidate_kthread.
	    pKThr = candidate_kthread;
	    ppc_msync();
	    incrementSlotAllocatedCount(GetHWThreadStateByProcessorID(pKThr->ProcessorID));
	    break;  // exit the for loop, we are done
	trynext:
	    continue;
	}
	Kernel_Unlock(&pHwt->migrationLock);
    }
    return pKThr;
}

// Get an available kthread
KThread_t *Process_GetAvailableKThread(AppProcess_t *proc)
{
    HwtRecycleList_Mgr_t *hwtlistmgr = &(proc->HwtRecycleListMgr);
    KThread_t* pKThr_Child = NULL;
    // Test to see if there is a hwthread index on the recycle list before assigning a new one
    // note: keep this initial test fast, so don't do at atomically
    if (hwtlistmgr->indexTail > hwtlistmgr->indexHead)
    {
        // Attempt to remove entry from the recycle list. 
        Kernel_Lock(&hwtlistmgr->lock);
        while (hwtlistmgr->indexTail > hwtlistmgr->indexHead)
        {
            // An entry exists on the list. We will attempt to use it.
            int processorID = hwtlistmgr->hwtindex[(hwtlistmgr->indexHead++ % HWTLISTMGRENTRIES)];
	    pKThr_Child = GrabFreeKThread(proc, processorID);
            if (pKThr_Child) break; // If we exited the above loop with a kthread, exit the while loop.
        } // End while entries on the recycle list
        Kernel_Unlock(&hwtlistmgr->lock);

        // If we have a kthread at this point, our work is done
        if (pKThr_Child)
        {
            return pKThr_Child;
        }
    }
    // If we reach this point, we did not find a kthread from the hwt recycle list. Use the layout counter to
    // determine the hardware thread search order.
    int processorID, skipIndex, index_adjust;
    uint64_t loop_count = 0; // Only search the the hardware threads within the process once for an available kthread
    uint64_t hwt_count = (uint64_t)proc->HWThread_Count;

    while(loop_count++ < hwt_count)    
    {
        skipIndex = 0;
        index_adjust = 0;

        // Increment the thread layout counter and return old value    
        uint64_t thread_counter = Fetch_and_Add( &hwtlistmgr->threadLayout_count, 1 );
        uint64_t scoped_counter = thread_counter % hwt_count;

        if ( (hwt_count <= CONFIG_HWTHREADS_PER_CORE) || hwtlistmgr->layout_type == CONFIG_SCHED_DEPTH_LAYOUT ) 
        {
            processorID = (scoped_counter) + proc->ProcessLeader_ProcessorID; 
            // Make adjustments to the hwthread index for UPC counting considerations. We will hand out
            // the even hwthread indexes before the odd hwthread indexes when possible
            if (hwt_count >=CONFIG_HWTHREADS_PER_CORE)
            {
                // Is the counter positioned to thread id 1 within the core
                if ((scoped_counter%CONFIG_HWTHREADS_PER_CORE) == 1)
                {
                    // adjust 
                    index_adjust = 1;
                }
                else if ((scoped_counter%CONFIG_HWTHREADS_PER_CORE) == 2)
                {
                    index_adjust = (-1);
                }
            }
        }
        else 
        {
            uint64_t core_cycle = hwt_count/CONFIG_HWTHREADS_PER_CORE; 
            processorID = (thread_counter % core_cycle)*CONFIG_HWTHREADS_PER_CORE + 
                       ((thread_counter%(hwt_count))/core_cycle) +
                       proc->ProcessLeader_ProcessorID;

            // Make adjustments to the hwthread index for UPC counting considerations. We will hand out
            // the even hwthread indexes before the odd hwthread indexes when possible

            // Is the counter in the second quarter of the range
            if ((scoped_counter >= hwt_count>>2) && 
                (scoped_counter < hwt_count>>1))
            {
                // Set the adjustment to the next even hwthread index in this core
                index_adjust = 1;
            }
            // Is the counter in the third quarter of the range
            else if ((scoped_counter >= hwt_count>>1) &&  
                     (scoped_counter < ((hwt_count>>1)+((uint64_t)hwt_count>>2))))
            {
                // Set the adjustment to the previous odd hwthread index
                index_adjust = (-1);
            }
        }
        // Apply any adjustments to the hwtIndex that may have been calculated
        processorID += index_adjust;

        // Search for a avaialable kthread
	pKThr_Child = GrabFreeKThread(proc, processorID);
        // If we aquired a kthread, return now.
        if (pKThr_Child) 
            return pKThr_Child; 
    }
    return NULL; // No kthreads are available
}

// Make a hardware thread available for assignment within the specified process
void Process_MakeHwThreadAvail(AppProcess_t *pAppProcess, int processorID)
{
    HwtRecycleList_Mgr_t *hwtlistmgr = &(pAppProcess->HwtRecycleListMgr);
    Kernel_Lock(&hwtlistmgr->lock);
    hwtlistmgr->hwtindex[hwtlistmgr->indexTail++ % HWTLISTMGRENTRIES] = processorID;
    // Sort the available list in order of the level of software thread over-commitment
    //Process_SortRecycleList(hwtlistmgr);

    // Indicate that a slot is now available within the hardware thread.
    decrementSlotAllocatedCount(GetHWThreadStateByProcessorID(processorID));
    Kernel_Unlock(&hwtlistmgr->lock);
}

// Initialize the AppProcess_Mgr structure within the process
void Process_InitHwtListMgr(AppProcess_t *pAppProcess, int LayoutType)
{
    HwtRecycleList_Mgr_t *hwtlistmgr = &(pAppProcess->HwtRecycleListMgr);
    hwtlistmgr->layout_type = LayoutType;
    hwtlistmgr->threadLayout_count = 1; // Start counter at one since the zero value maps first slot of the process leader core which has implicitly been assigned
    hwtlistmgr->indexHead = 0;
    hwtlistmgr->indexTail = 0;
}
#endif                                                                                                                 // FUSEDOS


void  Process_Init()
{
    int i,j;
    // Iterate through the number of AppStates
    for (i = 0; i < CONFIG_MAX_APPSTATES; i++)
    {
        // Initialization of the indexes in the process and appstate objects.
        for (j = 0; j < CONFIG_MAX_PROCESSES; j++)
        {
            NodeState.AppState[i].ProcStateIndex[j] = -1;
        }
    }
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Compare new request to current state of the node state and decide if this is a valid configuration transition
// Must look at what threads are currently assigned to active Processes/Apps and if the requested changes are 
// limited to threads that are not associated with Active processes, then allow the configuration modifications to 
// proceed. 
// Output: 
//  Return value defined by enumeration Process_Map_Validate_Return         
int Process_SetupValidate(int appLeaderCoreID, int numProcesses, int numCores)
{
    // Validate the parameter values
    if ((appLeaderCoreID < 0) || (appLeaderCoreID > CONFIG_MAX_APP_THREADS))
    {
        return bgcios::CornerCoreError;
    }
    // Verify that the numcores specified is a power of 2 and non-zero
    if (popcnt64(numCores) != 1)
    {
        return bgcios::NumCoresInProcessError;
    }
    // Verify that the index and number of cores specified are within the bounds of the node
    if (((appLeaderCoreID + numCores) * CONFIG_HWTHREADS_PER_CORE) > CONFIG_MAX_APP_THREADS)
    {
        return bgcios::NumCoresInProcessError;
    }
    // Verify that a valid number of processes was specified.
    if ((popcnt64(numProcesses) != 1) ||      // verify non-zero power of 2 processes specified
        numProcesses > (numCores*CONFIG_HWTHREADS_PER_CORE)) // verify the number of processes is not greater than the number of hwthreads
    {
        return bgcios::NumProcessesError;
    }

    // Generate a mask representing all the hardware threads that are contained in the request.
    uint64_t hwthread_mask = 0;
    int i;
    for (i=0; i< numCores; i++ )
    {
        hwthread_mask |= _B4((((appLeaderCoreID+i)*4)+3),0xF);
    }

    // Test the targeted hardware threads to verify that they are available
    uint64_t temp_mask = hwthread_mask;
    for (i=0; temp_mask; i++)
    {
        uint64_t test_mask = _BN(i);
        if (temp_mask & test_mask)
        {
            // Get the pointer to the process object that owns this hardware thread. 
            // note: cannot use GetProcessByProcessorID() here since we may not be running in an App thread
            HWThreadState_t *hwt = GetHWThreadStateByProcessorID(i);
            AppProcess_t *proc = hwt->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]->pAppProc;
            // If this thread is not currently allocated to any process, the process pointer will be NULL
            if (proc)
            {
                // This thread was a member of a process. Verify that this process is no longer active
                if (proc->State != ProcessState_Free)
                {
                    // We have found a problem. This hardware thread is not part of a free process. Return with error
                    return bgcios::ProcessActive;
                }
            }
            temp_mask &= ~test_mask;
        }
    }
    // If we are configured as a subblock subnode job (i.e. one core), ensure that only 1 process is requested
    if ((numCores == 1) && (numProcesses != 1 ) )
    {
        return bgcios::NumProcessesError;
    }

    return bgcios::Success;
}

// Setup the relationships between the nodes within the job
int Process_SetupJob(struct SetupJobMessage *setupJobMsg)
{

    // Validate the core specification 
    if ((setupJobMsg->shape.core != 1) &&  // Current policy is we only support sub-node sub-block partitions containing 1 core.
        (setupJobMsg->shape.core != CONFIG_MAX_APP_CORES) && // Allow a specification of all cores.
        (setupJobMsg->shape.core != (NodeState.NumCoresEnabled-1))) // for simulation and bringup, support less than max cores
    {
        return bgcios::SubBlockJobError;
    }
    // If a core shape value indicates a sub-node job, verify the remaining shape values
    if (setupJobMsg->shape.core == 1)
    {
        if ( (setupJobMsg->shape.aCoord != 1) ||
             (setupJobMsg->shape.bCoord != 1) ||
             (setupJobMsg->shape.cCoord != 1) ||
             (setupJobMsg->shape.dCoord != 1) ||
             (setupJobMsg->shape.eCoord != 1))
        {
            return bgcios::SubBlockJobError;
        }
    }
    // Verify we are not exceeding the dimensions of the block.
    Personality_t* pers = GetPersonality();
    if (((setupJobMsg->corner.aCoord + setupJobMsg->shape.aCoord) > pers->Network_Config.Anodes) ||
        ((setupJobMsg->corner.bCoord + setupJobMsg->shape.bCoord) > pers->Network_Config.Bnodes) ||
        ((setupJobMsg->corner.cCoord + setupJobMsg->shape.cCoord) > pers->Network_Config.Cnodes) ||
        ((setupJobMsg->corner.dCoord + setupJobMsg->shape.dCoord) > pers->Network_Config.Dnodes) ||
        ((setupJobMsg->corner.eCoord + setupJobMsg->shape.eCoord) > pers->Network_Config.Enodes))
    {
        return bgcios::SubBlockJobError;
    }
    // Select an AppState object for this job. The CoreID of the Application Leader is used
    // as an index to select the appState object.
    int appStateIndex = setupJobMsg->corner.core;
    AppState_t *appState = &NodeState.AppState[appStateIndex];

    // Is this a sub-node job? 
    if ((setupJobMsg->shape.aCoord < pers->Network_Config.Anodes) || // Not all nodes used in A dimension 
        (setupJobMsg->shape.bCoord < pers->Network_Config.Bnodes) || // Not all nodes used in B dimension
        (setupJobMsg->shape.cCoord < pers->Network_Config.Cnodes) || // Not all nodes used in C dimension
        (setupJobMsg->shape.dCoord < pers->Network_Config.Dnodes) || // Not all nodes used in D dimension
        (setupJobMsg->shape.eCoord < pers->Network_Config.Enodes) || // Not all nodes used in E dimension
        (setupJobMsg->shape.core == 1))  // not all cores used (sub-node block)
    {
        appState->subBlockActive = 1;
    }
    // Populate the corner and shape info in the AppState object
    appState->corner.aCoord = setupJobMsg->corner.aCoord;
    appState->corner.bCoord = setupJobMsg->corner.bCoord;
    appState->corner.cCoord = setupJobMsg->corner.cCoord;
    appState->corner.dCoord = setupJobMsg->corner.dCoord;
    appState->corner.eCoord = setupJobMsg->corner.eCoord;
    appState->corner.core = setupJobMsg->corner.core;

    appState->shape.aCoord = setupJobMsg->shape.aCoord;
    appState->shape.bCoord = setupJobMsg->shape.bCoord;
    appState->shape.cCoord = setupJobMsg->shape.cCoord;
    appState->shape.dCoord = setupJobMsg->shape.dCoord;
    appState->shape.eCoord = setupJobMsg->shape.eCoord;
    appState->shape.core = setupJobMsg->shape.core;
    // Initialize information related to the number of ranks in this job
    appState->ranksPerNode = setupJobMsg->numRanksPerNode;
    appState->ranksTotal =  appState->shape.aCoord * appState->shape.bCoord * appState->shape.cCoord * appState->shape.dCoord * appState->shape.eCoord * appState->ranksPerNode;
    appState->ranksActive = setupJobMsg->numRanks;
    // Validate the rank specification 
    if (!appState->ranksActive || (appState->ranksActive > appState->ranksTotal))
    {
        return bgcios::RanksInJobError;
    }
    // Initialize the rank mapping to a default value if no mapping information was provided
    if (strlen(setupJobMsg->mapping) == 6)
    {
        // Move the ABCDET coordinate values
        strncpy(appState->mapOrder, setupJobMsg->mapping, 6); 
    }
    else
    {
        // Setup a default mapping
        strncpy(appState->mapOrder, "ABCDET", 6);
    }
    if (strlen(setupJobMsg->mapFilePath) != 0)
    {
        strncpy(appState->mapFilePath, setupJobMsg->mapFilePath, sizeof(appState->mapFilePath) ); 
    }
     
    // Store the JobID into the AppState object to indicate that setup is completed
    appState->JobID = setupJobMsg->header.jobId;

    // Setup the job leader location
    appState->jobLeaderCoord.aCoord = NodeState.JobLeaderCoords[0] = setupJobMsg->jobLeader.aCoord;
    appState->jobLeaderCoord.bCoord = NodeState.JobLeaderCoords[1] = setupJobMsg->jobLeader.bCoord;
    appState->jobLeaderCoord.cCoord = NodeState.JobLeaderCoords[2] = setupJobMsg->jobLeader.cCoord;
    appState->jobLeaderCoord.dCoord = NodeState.JobLeaderCoords[3] = setupJobMsg->jobLeader.dCoord;
    appState->jobLeaderCoord.eCoord = NodeState.JobLeaderCoords[4] = setupJobMsg->jobLeader.eCoord;

    // Initialize Node scoped job leader counters. We do this in all the nodes but it is only necessary in the job leader node
    if (appState->shape.core == NodeState.NumCoresEnabled-1) // Does this job contain all application cores enabled in the node?
    {
#if JOBLEADERENABLED
        if (!Personality_ApplicationPreLoaded())
        {
            // The job contains at least one full node (not a sub-node job).
            appState->jobLeaderData.NodesInJob = appState->shape.aCoord * appState->shape.bCoord * appState->shape.cCoord * appState->shape.dCoord * appState->shape.eCoord;        
        }
        else
#endif
        {
            // fake out the job exit/setup flow to think there is only one node in the job
            appState->jobLeaderData.NodesInJob = 1;        
        }
    }
    else
    {
        // This is a sub-node job
        appState->jobLeaderData.NodesInJob = 0; // zero value indicates that the job does not consume an entire node        
    }

    // Initialize the job leader  counters in each node, regardless of whether it is a job leader node.
    appState->jobLeaderData.RdmaCounterForExit = appState->jobLeaderData.NodesInJob * 8; // This counter is used by the RDMA operations. 8 bytes per active node.
    appState->jobLeaderData.AbnormalProcessExitCount = 0;
    appState->jobLeaderData.JobExitStatus = 0;
    appState->jobLeaderData.corepacesem[1] = CONFIG_COREDUMP_CONCURRENT; // set default number of concurrent coredumps
    appState->jobLeaderData.corepacesem[0] = 0; // set lower bound

    // Initialize the MUDM structures in NodeState with job leader information
    NodeState.remoteget.torus_destination.Destination.A_Destination = setupJobMsg->jobLeader.aCoord;
    NodeState.remoteget.torus_destination.Destination.B_Destination = setupJobMsg->jobLeader.bCoord;
    NodeState.remoteget.torus_destination.Destination.C_Destination = setupJobMsg->jobLeader.cCoord;
    NodeState.remoteget.torus_destination.Destination.D_Destination = setupJobMsg->jobLeader.dCoord;
    NodeState.remoteget.torus_destination.Destination.E_Destination = setupJobMsg->jobLeader.eCoord;
    NodeState.directput.torus_destination.Destination.Destination = NodeState.remoteget.torus_destination.Destination.Destination;

    // Setup the job class routes
    if (configureJobClassroutes(setupJobMsg) != 0)
    {
        return bgcios::ClassRouteDataError;
    }

    return 0;
}


// Setup the relationships between cores, hardware threads, AppState objects and AppProcess objects
//  Return values: 
//    0:  Process Map Success
//    1:  Process Map Failure due to error in application leader index
//    2:  Process Map Failure due to error in the number of cores requested
//    3:  Process Map Failure due to targeted cores containing active processes
// 
int Process_SetupMap(struct LoadJobMessage *loadMsg, AppState_t **ppAppState)
{
    AppState_t *pAppState = NULL;
    // Find the AppState object that has been setup for this job
    for (int i=0; i<CONFIG_MAX_APP_APPSTATES; i++)
    {
        if (NodeState.AppState[i].JobID == loadMsg->header.jobId)
        {
            pAppState = &NodeState.AppState[i];
            break;
        }
    }
    if (!pAppState) // Did we find the AppState for our JobID
    {
        return bgcios::CornerCoreError;
    }
    // Override the number of allowed concurrent core dumps if env is specifed
    GetEnvValue64(loadMsg, "BG_COREDUMPCONCURRENT", (uint64_t*)(&pAppState->jobLeaderData.corepacesem[1]));

    // Setup the application core id and the number of cores 
    int appLeaderCoreID = pAppState->corner.core;
    int numCores = MIN( (uint32_t)(pAppState->shape.core), (uint32_t)NodeState.NumCoresEnabled-1 ); 

    NodeState.TraceConfig = TraceConfigDefault;
    GetEnvValue64(loadMsg, "BG_TRACE", &NodeState.TraceConfig);
    if ((NodeState.TraceConfig) && ((GetPersonality()->Kernel_Config.NodeConfig & (PERS_ENABLE_NodeRepro | PERS_ENABLE_PartitionRepro)) == 0))
    {
        printf("(I) NodeState.TraceConfig = %016lx\n", NodeState.TraceConfig);
    }
    // Before we do any setup, validate the request
    int rc = Process_SetupValidate(appLeaderCoreID, pAppState->ranksPerNode, numCores);
    if (rc != bgcios::Success)
    {   
        TRACE( TRACE_Process, ("(E) Process_SetupMap: error %d validating process map\n", rc) );
        return rc;
    }
    // Initialize some local variables
    int i,processorID;
    uint32_t locT = 0;
    int prevProcessIndex = -1;
    int processLeaderProcessorID = -1;
    AppProcess_t *pAppProcess = NULL;
    int appLeaderProcessorID = CONFIG_HWTHREADS_PER_CORE * appLeaderCoreID;

    // Determine the number of threads that we will allow per hardware thread. This does not include remote threads or the special kernel thread
    uint32_t thread_model = CONFIG_THREAD_MODEL_DEFAULT;
    if (GetEnvValue(loadMsg, "BG_THREADMODEL",  &thread_model))
    {
        // If an unexpected thread model variable was provided, set the default thread model
        if ((thread_model != CONFIG_THREAD_MODEL_LTM) &&
            (thread_model != CONFIG_THREAD_MODEL_BGP) &&
            (thread_model != CONFIG_THREAD_MODEL_ETA))
        {
            thread_model = CONFIG_THREAD_MODEL_DEFAULT;
        }
    }
    int layout_type = CONFIG_SCHED_BREADTH_LAYOUT;
    uint32_t user_thread_layout;
    if (GetEnvValue(loadMsg, "BG_THREADLAYOUT",  &user_thread_layout))
    {
        if ((user_thread_layout == CONFIG_SCHED_BREADTH_LAYOUT) || (user_thread_layout == CONFIG_SCHED_DEPTH_LAYOUT))
        {
            layout_type = user_thread_layout;
        }
    }
    // Generate a mask representing all the hardware threads that we are going to configure in this request.
    uint64_t hwthread_mask = 0;
    for (i=0; i< numCores; i++ )
    {
        hwthread_mask |= _B4((((appLeaderCoreID+i)*4)+3),0xF);
    }
    // Generate a mask containing the Process Leader hardware threads
    int numHwThreads = popcnt64(hwthread_mask);
    int processRatio = numHwThreads / pAppState->ranksPerNode;
    // first process is the first hardware thread on the appLeaderCore
    uint64_t processLeaders = _BN(appLeaderCoreID*4);
    for (i=1; i < (int)pAppState->ranksPerNode; i++)
    {
        processLeaders |= (processLeaders >> processRatio);
    }

    // Update the masks in the NodeState based on this process mapping
    NodeState.AppLeaders &= ~hwthread_mask; // Initialize bits corresponding to all threads across processes in this Job
    NodeState.ProcessLeaders &= ~hwthread_mask; // Initialize bits corresponding to threads across processes in this Job
    NodeState.AppLeaders |= _BN(appLeaderCoreID*4); // Set the App Leader thread for this Job
    NodeState.ProcessLeaders |= processLeaders; // Set the Process leader threads for this Job

    // Setup masks in the AppState Structure
    pAppState->AppStateIndex = appLeaderCoreID;
    pAppState->ProcessLeaderMask = processLeaders;
    pAppState->AppThreadsMask = hwthread_mask;
    pAppState->Active_Cores = numCores;

    // Copy data from LoadJob message to the AppState object.
    pAppState->JobID = loadMsg->header.jobId;
    pAppState->App_Argc = loadMsg->numArguments;
    memcpy(pAppState->App_Args, loadMsg->arguments, loadMsg->argumentSize);
    memcpy(pAppState->App_Env, loadMsg->variables, loadMsg->variableSize);
    pAppState->Env_Total_Length = loadMsg->variableSize;
    pAppState->JobID = loadMsg->header.jobId;
    pAppState->UserID = loadMsg->userId;
    pAppState->GroupID = loadMsg->groupId;
    pAppState->NumSecondaryGroups = loadMsg->numSecondaryGroups;
    memcpy(pAppState->SecondaryGroups, loadMsg->secondaryGroups, sizeof(gid_t) * loadMsg->numSecondaryGroups);
    pAppState->RankForStdin = loadMsg->rankForStdin;
    pAppState->RankForStrace = loadMsg->rankForStrace;

    // Iterate thru the hardware thread indexes, gluing together the process structures. Start looking
    // at the thread index of the application leader thread so we do not disturb other applications 
    // that may be configured and running
    uint64_t selectionMask = (uint64_t)_BN(0); // bit mask with bit 0 set
    selectionMask = selectionMask >> appLeaderProcessorID; // adjust the mask to our starting location

    for (processorID=appLeaderProcessorID; processorID< CONFIG_MAX_APP_THREADS; processorID++)
    {
        // If this hardware thread index is not enabled within this kernel, or 
        // if the hardware thread index is not a thread within this configuration request,
        //  bypass processing
        if (!(NodeState.EnabledAppThreads & selectionMask) ||
            !(hwthread_mask & selectionMask))
        {
            selectionMask = selectionMask >> 1;  // move mask to next position
            continue;
        }
        // Set the pointer to the app state object for this thread index
        NodeState.pAppState[processorID] = pAppState; 

        // Generate the index into the array of AppProcess objects for this hardware thread index
        if (processLeaders & selectionMask) // is this hwthread marked as a process leader?
        {
            // This is a process leader. Indicate we have a new process leader thread index
            processLeaderProcessorID = processorID;

            // If this is not the first process found, set the next pointer to this new process 
            // into the current process, chaining together the AppProcess structures for this App
            if (pAppProcess)
            {
                pAppProcess->nextProcess = &(NodeState.AppProcess[processorID]);
            }

        }
        pAppProcess = &(NodeState.AppProcess[processLeaderProcessorID]);

        // Set the index of the AppProcess object in the array of AppProc indexes in the AppState object
        pAppState->ProcStateIndex[processorID] = processLeaderProcessorID;

        // See if the appProcessIndex changed. If so, this thread is the leader of a new process
        if (prevProcessIndex != processLeaderProcessorID)
        {
            // We are the process leader. Setup fields in the appProcess object
            pAppProcess->app = pAppState;
            pAppProcess->Tcoord = locT;
            locT++; 
            pAppProcess->HWThread_Count = 1;
            pAppProcess->ThreadModel = thread_model; 
            pAppProcess->State = ProcessState_Active;
            pAppProcess->ProcessLeader_ProcessorID = processorID;
            pAppState->Active_Processes++; // bump the count of active processes within the current AppState object
            pAppProcess->HwthreadMask = _BN( processorID ); // set this field with the first hwthread in the process
            pAppProcess->CurrentUmask = loadMsg->umask;
            strncpy(pAppProcess->CurrentDir, loadMsg->workingDirectoryPath, sizeof(pAppProcess->CurrentDir));
            // Initialize the hardware thread manager. 
            Process_InitHwtListMgr(pAppProcess, layout_type);  
        }
        else
        {
            // Update the hwthread count field and affinity slot count fields in the process object.
            pAppProcess->HWThread_Count++;

            // Update the hwthread mask to indicate that this hardware thread is a part of the process
            pAppProcess->HwthreadMask |= _BN( processorID );
        }
        // Set the AppProcess pointer and sched policy in the KThread objects for each affinity slot
        HWThreadState_t *pHWThreadState = GetHWThreadStateByProcessorID(processorID);
        for ( i = 0; i < CONFIG_AFFIN_SLOTS_PER_HWTHREAD; i++)
        {    
            pHWThreadState->SchedSlot[i]->pAppProc = pAppProcess;
            pHWThreadState->SchedSlot[i]->Policy = SCHED_FIFO;
        }
        pHWThreadState->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]->pAppProc = pAppProcess;

        // Setup for next iteration
        prevProcessIndex = processLeaderProcessorID; 
        // Shift the select mask
        selectionMask = selectionMask >> 1;
    }
    
    // Store the pointer to the appstate
    *ppAppState = pAppState;

    TRACE( TRACE_Entry, ("(I) Process_SetupMap[%2d:%d]: completed\n",
                               ProcessorCoreID(), ProcessorThreadID()) );

    return 0;
}

void Process_SetupAgentMap(struct LoadJobMessage *loadMsg, AppState_t *parentAppState)
{
    // Setup the mapping for the application agents
    int appStateIndex = CONFIG_FIRST_AGENT_APPSTATE_INDEX;
    int agentIndex = 0; 
    int locT = CONFIG_MAX_APP_PROCESSES; // start the PID after the last application process
    int processorID = NodeState.FirstAppAgentProcessorID;
    AppState_t *pAgentAppState[CONFIG_MAX_APP_AGENTS];
    char *agent_path[CONFIG_MAX_APP_AGENTS];
    // Initialize the agent mask
    NodeState.AppAgents = 0;
    // If this is a subnode job, do not attempt to start an agent. This is not supported.
    if (parentAppState->jobLeaderData.NodesInJob == 0)
    {
        return;
    }
    
    // Initialize array of AppState pointers and pathnames for configured Agents
    for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
    {
        pAgentAppState[i] = NULL;
        agent_path[i] = NULL;
    }
    // Was the COMM agent env variable specified
    if (GetEnvString(loadMsg, "BG_APPAGENTCOMM", &(agent_path[agentIndex])) == 1)
    {
        if (strncmp(agent_path[agentIndex], "DISABLE", strlen("DISABLE")) != 0)
        {
            // A path was specified. Use it.
            pAgentAppState[agentIndex++] = &NodeState.AppState[appStateIndex++]; 
        }
        else
        {
            // The special DISABLE keyword was specified. Do not activate a comm agent.
            agent_path[agentIndex] = NULL;
        }
    }
    else
    {
        // No env var was specified. If CIOS is enabled, setup the default application agent.
        if (Personality_CiosEnabled())
        {
             agent_path[agentIndex] = (char *)default_comm_agent;
             pAgentAppState[agentIndex++] = &NodeState.AppState[appStateIndex++]; 
        }
    }
    if (GetEnvString(loadMsg, "BG_APPAGENT", &(agent_path[agentIndex])) == 1)
    {
        pAgentAppState[agentIndex] = &NodeState.AppState[appStateIndex];
        
    }
    for (int i=0; i < CONFIG_MAX_APP_AGENTS; i++, processorID++, locT++)
    {
        AppState_t *pAppState = pAgentAppState[i];
        if (pAppState)
        {
            NodeState.AppAgents |= _BN(i); // Set App Agent mask 
            NodeState.pAppState[processorID] = pAppState;
            AppProcess_t *pAppProcess = &(NodeState.AppProcess[processorID]);
            // Set the index of the AppProcess object in the array of AppProc indexes in the AppState object
            pAppState->parentAppState = parentAppState;
            pAppState->ProcStateIndex[processorID] = processorID;
            pAppState->Active_Processes = 1;  
            // We are the process leader. Setup fields in the appProcess object
            pAppProcess->app = pAppState;
            pAppProcess->Tcoord = locT;
            pAppProcess->HWThread_Count = 1;
            pAppProcess->ThreadModel = CONFIG_THREAD_MODEL_LTM; 
            pAppProcess->State = ProcessState_Active;
            pAppProcess->ProcessLeader_ProcessorID = processorID;
            pAppProcess->CurrentUmask = loadMsg->umask;
            strncpy(pAppProcess->CurrentDir, loadMsg->workingDirectoryPath, sizeof(pAppProcess->CurrentDir));

            // Initialize the hardware thread manager. 
            Process_InitHwtListMgr(pAppProcess, 1);  
            // Set the AppProcess pointer and sched policy in the KThread objects for each affinity slot
            HWThreadState_t *pHWThreadState = GetHWThreadStateByProcessorID(processorID);
            for (int j = 0; j < CONFIG_AFFIN_SLOTS_PER_HWTHREAD; j++)
            {    
               pHWThreadState->SchedSlot[j]->pAppProc = pAppProcess;
               pHWThreadState->SchedSlot[j]->Policy = SCHED_FIFO;
            }
            pHWThreadState->SchedSlot[CONFIG_SCHED_KERNEL_SLOT_INDEX]->pAppProc = pAppProcess;

            // Initialize additional AppState fields.
            pAppState->JobID = loadMsg->header.jobId;
            pAppState->App_Argc = 1; // only argument is the pathname of the executable
            memcpy(pAppState->App_Args, agent_path[i], strlen(agent_path[i]));
            memcpy(pAppState->App_Env, loadMsg->variables, loadMsg->variableSize);
            pAppState->Env_Total_Length = loadMsg->variableSize;
            pAppState->JobID = loadMsg->header.jobId;
            pAppState->UserID = loadMsg->userId;
            pAppState->GroupID = loadMsg->groupId;
        }
    }
}
#endif                                                                                                                 // FUSEDOS

void Process_InitializeThreadLocalStorage( AppProcess_t *pAppProc, void *pTLS )
{
    /*
      We must do two things here:
      + Kernel must copy the .tdata section for each new thread
      + Kernel must memzero the .tbss section for each new thread

      Additionally, the tdata and tbss sections are 0x7000 bytes prior to the value in GPR 2:PPC32 or 13:PPC64.
      This is a hardcoded value in GLIBC.  Not sure how to automatically determine if it changes
      in another glibc version.
    */

    void *tdata_start = pAppProc->pTData_Start;
    size_t tdata_size = pAppProc->TData_Size;
    void *tbss_start  = pAppProc->pTBSS_Start;
    size_t tbss_size  = pAppProc->TBSS_Size;

    if ( tdata_start != 0 )
    {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        memcpy((void*)((size_t)pTLS-(size_t)GLIBC_TCB_OFFSET), (void*)tdata_start, tdata_size);                        // FUSEDOS
#else                                                                                                                  // FUSEDOS
        memcpy_Inline( (void*)((size_t)pTLS-(size_t)GLIBC_TCB_OFFSET), (void*)tdata_start, tdata_size );
#endif                                                                                                                 // FUSEDOS
        if ( tbss_start != 0 )
        {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            bzero((void*)((size_t)pTLS-(size_t)GLIBC_TCB_OFFSET + (size_t)tbss_start - (size_t)tdata_start),           // FUSEDOS
                  tbss_size);                                                                                          // FUSEDOS
#else                                                                                                                  // FUSEDOS
            bzero_Inline( (void*)((size_t)pTLS-(size_t)GLIBC_TCB_OFFSET + (size_t)tbss_start - (size_t)tdata_start), tbss_size );
#endif                                                                                                                 // FUSEDOS
        }
    }
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
void Process_DumpThreadInfo(AppProcess_t *pAppProc) {
    //  Loop through hardware threads in the process
    int processorID;
    int i;
    KThread_t *pKThread;
    uint64_t selectionMask = (uint64_t)_BN(0); // bit mask with bit 0 set
    // Print entries in the available hwthreadindex table
    HwtRecycleList_Mgr_t *hwtlistmgr = &(pAppProc->HwtRecycleListMgr);
    printf("ThreadLayout count %ld head %d tail %d Avail Hwts:", hwtlistmgr->threadLayout_count, hwtlistmgr->indexHead, hwtlistmgr->indexTail);
    if (hwtlistmgr->indexTail > hwtlistmgr->indexHead)
    {
        printf("Available hardware thread indexes:");
        // Attempt to remove entry from the recycle list. 
        Kernel_Lock(&hwtlistmgr->lock);
        uint32_t listhead = hwtlistmgr->indexHead;
        while (hwtlistmgr->indexTail > listhead)
        {
           printf("%d ",hwtlistmgr->hwtindex[listhead]);
           listhead++;
        }
        Kernel_Unlock(&hwtlistmgr->lock);
        printf(" \n");
    }
    for (processorID=0; processorID< CONFIG_MAX_APP_THREADS; processorID++, selectionMask = selectionMask >> 1)
    {
        // If this hardware thread index is not enabled within this kernel, bypass processing
        if (!(NodeState.EnabledAppThreads & selectionMask))
            continue;
        HWThreadState_t *pHWT = GetHWThreadStateByProcessorID(processorID);
        printf("Core %d Thread %d:\n", processorID>>2, processorID &0x3);
        printf("   SchedOrder %016lx \n", pHWT->SchedOrder);
        for (i=0; i< CONFIG_AFFIN_SLOTS_PER_HWTHREAD; i++)
        {
            pKThread = pHWT->SchedSlot[i];
            printf("  Slot0 kthread %016lx\n",(uint64_t)pKThread);
            if (pKThread)
            {
                printf("    State %d, ProcessorID %d \n",pKThread->State, pKThread->ProcessorID);
            }
        }
    }
}


/*! \brief Returns the string of an environment variable
 *
 * \param[in] envname NULL terminated string containing the environment variable
 * \param[out] value pointer to store a pointer-to-a-string.
 * \return Boolean.  Does environment variable exist?  true/false.
 */
uint32_t  GetEnvString(LoadJobMessage *msg, const char* envname, char** value)
{
    char* ptr = msg->variables;
    uint32_t len = strlen(envname);
    while (*ptr)
    {
        if ((strncmp(envname, ptr, len)==0)&&(ptr[len]=='='))
        {
            *value = &ptr[len+1];
            return 1;
        }
        ptr += (strlen(ptr) + 1);
    }
    return 0;
}

/*! \brief Returns the string of an environment variable
 *
 * \param[in] envname NULL terminated string containing the environment variable
 * \param[out] value pointer to store that will contain the value
 * \return Boolean.  Does environment variable exist?  true/false.
 */
uint32_t GetEnvValue(LoadJobMessage *msg, const char* envname, uint32_t* value)
{
    char* ptr;
    int rc = GetEnvString(msg, envname, &ptr);
    if (rc==1)
    {
        *value = (uint32_t)atoi_((const char *)ptr);
    }
    return rc;
}
uint32_t GetEnvValue64(LoadJobMessage *msg, const char* envname, uint64_t* value)
{
    int base = 10;
    char* ptr;
    int rc = GetEnvString(msg, envname, &ptr);
    if (rc==1)
    {
        if((ptr[0] == '0')&&(toupper(ptr[1]) == 'B'))
            base = 2;
        if((ptr[0] == '0')&&(toupper(ptr[1]) == 'X'))
            base = 16;
        
        *value = strtoull_((const char *)ptr, NULL, base);
    }
    return rc;
}
#endif                                                                                                                 // FUSEDOS

