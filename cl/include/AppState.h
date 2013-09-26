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
#ifndef _APPSTATE_H_
#define _APPSTATE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>
#include <cnk/include/Config.h>

#define APP_MAX_ARGC       (64)
#define APP_MAX_ARGSTR   (4096)
#define APP_MAX_ENVVARS  (8192)
#define APP_MAX_ENVSTR    (512) // no single environment variable > this.
#define APP_MAX_PATHNAME  (512) // max string length for executable path and name
#define APP_MAX_MAPPATHNAME (512) // max string length for mapping file path and name
#define APP_MAX_GROUPS     (64) // max number of secondary groups

#define APP_MAX_SEGS       (8)  // Max Application program header segments

#define APP_MAX_AUXC       (4)  // Max Application Aux Vectors

#define APP_FLAGS_R                 0x001
#define APP_FLAGS_W                 0x002
#define APP_FLAGS_X                 0x004
#define APP_FLAGS_LOAD              0x008
#define APP_FLAGS_TLS               0x010
#define APP_FLAGS_DYN               0x020
#define APP_FLAGS_NONSPECULATIVE    0x040
#define APP_FLAGS_GUARDED           0x080
#define APP_FLAGS_LISTENABLE        0x100
#define APP_FLAGS_FLUSHSTORES       0x200
#define APP_FLAGS_INHIBITED         0x400
#define APP_FLAGS_ESEL(x)           ((x)<<16)


#define APP_STANDALONE   0x42475153544e4400LL

typedef struct AppSeg_t
{
    uint64_t seg_va;     // Virtual  Address of segment start
    uint64_t seg_pa;     // Physical Address (lower 32bits) of segment start
    uint64_t seg_filesz; // Size in bytes of segment in executable file
    uint64_t seg_memsz;  // Size in bytes of segment in memory
                         // Difference between seg_filesz and seg_memsz is bss
    uint32_t seg_csum;   // Checksum of this segment (if 0, not set/checked)
    uint32_t seg_flags;  // RWX, BSS, etc.
}
AppSeg_t;

typedef struct AppNodeCoordinates_t
{
   uint8_t aCoord;                    //!< Torus 'a' coordinate.
   uint8_t bCoord;                    //!< Torus 'b' coordinate.
   uint8_t cCoord;                    //!< Torus 'c' coordinate.
   uint8_t dCoord;                    //!< Torus 'd' coordinate.
   uint8_t eCoord;                    //!< Torus 'e' coordinate.
   uint8_t core;                      //!< Core number.
}
AppNodeCoordinates_t;

typedef union CollectiveLoadStatus_t
{
    volatile uint64_t dword;
    struct {
        volatile uint32_t returncode;  // Each broadcast is followed by a corresponding return code stored in this location
        volatile uint32_t iteration;   // Controls sequencing of broadcasts. Zero value implies collective broadcast is not active
    } word;
} 
CollectiveLoadStatus_t;


typedef struct JobLeaderData_t {
        uint64_t NodesInJob;
        volatile uint64_t RdmaCounterForExit;
        volatile uint64_t AbnormalProcessExitCount; // number of processes ending with a RC=1 or signal
        volatile uint64_t JobExitStatus; // largest exit status across all tasks in the job
        CollectiveLoadStatus_t collectiveLoadStatus;
        volatile uint64_t AbnormalExitCountBroadcastStatus; // indicator that broadcast of exit status to all nodes is complete 
        volatile uint64_t corepacesem[2];  // Semphore used for concurrent coredump control
} 
JobLeaderData_t;


typedef struct AppState_t
{
    uint64_t MAGIC;     // Magic String

    // Application Arguments to main().  **Must be first in AppState for external configuration to calc. Argv array **
    char     App_Args[ APP_MAX_ARGSTR ];
    
    // Executable info.  Includes load method info for:
    //    "push" (simulation, standalone): sim env or ctrl sys preload
    //    "pull" (full ciod): load executables from 'mounted' filesystems.
    
    // UserID Information
    uint32_t UserID;
    uint32_t GroupID;
    uint32_t NumSecondaryGroups;
    uint32_t SecondaryGroups[ APP_MAX_GROUPS ];
    uint64_t JobID;
    uint32_t PreloadedApplication;  // boolean
    uint32_t invokeWithInterpreter;  //boolean.  True if dynamic app needing invocation via interpreter
    uint32_t SharedTextSeg;  // boolean
    
    // Application Program Mapping
    uint64_t    App_EntryPoint;
    uint32_t    App_NumSegs;
    uint64_t    App_InterpreterTextBase;
    AppSeg_t App_Segments[ APP_MAX_SEGS ];

    // Application Arguments to main()
    int32_t  App_Argc;
    char    *App_Argv[ APP_MAX_ARGC   ]; // First argument is executable path

    // Application Environment Variables
    //  array of null terminated strings, with final double null.
    int32_t  Env_Total_Length;
    char     App_Env[ APP_MAX_ENVVARS ];

    // Process configuration information
    int32_t  Active_Processes;     // Active processes associated with this AppState
    uint32_t ActiveProcessesInJob; //!< The total number of processes for the job
    uint32_t Active_Cores;         // Total number of cores associated with the AppState
    int      PersistMemIndex; // index into Persistent Memory objects within the Node_State
    uint64_t ProcessLeaderMask;    // Mask of process leader threads 
    uint64_t AppThreadsMask;    // Mask of all the hwthreads participating in this job/application 
    uint64_t Timeshift; //!< Adjustment to synchronize time of day.
    uint64_t JobStartTime; //! Time that last job started.
    int32_t  outOfMemDuringLoad; // indicator for an out of memory condition detected when trying to load segments into physical memory
    int32_t  AppStateIndex; // Index of this AppState object into the array of AppState objects managed by NodeState
    int16_t  ProcStateIndex[ CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE ];

    // User address of KernelInfo structure appended to text segment.
    uint64_t KernelInfoAddress;

    // Virtual address range of the TLS segment
    uint64_t TLS_VStart;
    uint64_t TLS_VEnd;

    // Remember TLS TBSS and TData addresses and size
    //  On thread create, each thread's TLS areas must be (re)initialized.
    void     *pTData_Start;
    void     *pTBSS_Start;
    uint32_t TData_Size;
    uint32_t TBSS_Size; 

    // The sequence number of this node when reporting abnormal process exit conditions to the job leader
    volatile uint32_t AbnormalTerminationSequenceNum; 
    
    // Executable path and name taken from first argument in the argument list

    // Interpreter path and name (if this is a dynamic app, this is the ld.so path)
    char InterpreterPathAndName[ APP_MAX_PATHNAME ];

    // Physical Address Range owned by this Application
    uint64_t paddrStart;
    uint64_t paddrEnd;

    // Load State of the application
    volatile uint32_t LoadState; 

    // Combined exit status. This represents the most significant process exit status across the processes in the node
    volatile uint32_t ExitStatus;
    
    // Coordinates of the Job Leader Node. 
    AppNodeCoordinates_t jobLeaderCoord;

    // Additional data associated with the LoadState
    // Value stored is dependent on the specific LoadState value
    volatile uint32_t LoadStateReturnCode;
    volatile uint32_t LoadStateErrorCode;

    // Block or Subblock Corner and Shape information
    uint32_t subBlockActive; // Is this application running in a sub-block 
    AppNodeCoordinates_t  corner; // Corner coordinates of block/sub-block A,B,C,D,E and AppLeaderCoreIndex
    AppNodeCoordinates_t shape;   // Dimensions of block/sub-block  A x B x C x D x E x 'Number of Cores'

    // If this AppState represents an AppAgent, the following is a pointer to the App that this Agent is associated with
    void *parentAppState;

    // Number of ranks in this Job.
    uint32_t ranksTotal;   // Number of ranks dictated by the dimensions of the jobs (sub)block coordinates.
    uint32_t ranksActive;  // Number of ranks adjusted with a -NP specification
    uint32_t ranksPerNode; // The number of ranks per node for this job

    // Rank that is allowed to read from stdin
    uint32_t RankForStdin;

    // Rank that has system call tracing enabled
    int32_t RankForStrace;

    // Rank to Coord mapping order (e.g. ABCDET)
    char mapOrder[8];

    // Map file path if using a mapping file
    char mapFilePath[APP_MAX_MAPPATHNAME];
    
    // Job Leader data
    JobLeaderData_t jobLeaderData;

    // Number of threads that have reached the end of AppExit phase 1.
    uint64_t numThreadsAppExitPhase1;

    // At least one process has reported an abnormal exit from this node.
    volatile uint32_t processExitReported;     

    // True when stdiod has been notified that all standard I/O output is finished.
    uint16_t closedStdio;

    // Exit processing
    uint8_t jobControlIssuedSIGKILL;  // Job Controller issued kill
    uint8_t pad;                      // pad

    uint8_t disableCorefileWrite;     // Indicator that internal write operations for core file generation has been disabled
    
    volatile uint64_t jobControlSIGKILLstart;  // timebase when Job Controller requested kill
    
    uint32_t LoadSequence;
    uint32_t MaxSequence;
    uint32_t IsLoadLeader;
    uint32_t LoadNodeCount;
    uint32_t prohibitLoad;
}
AppState_t;

#endif
