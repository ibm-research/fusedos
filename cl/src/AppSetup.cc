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
#include "JobControl.h"
#include "ToolControl.h"
#include "fs/filesetup.h"
#endif                                                                                                                 // FUSEDOS
#include "ctype.h"
#include "rank.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "flih.h"
#endif                                                                                                                 // FUSEDOS
#include <stdlib.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/l2_util.h>
#include <hwi/include/bqc/testint_tdr.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/testint_inlines.h>
#endif                                                                                                                 // FUSEDOS
#include <elf.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <ramdisk/include/services/MessageHeader.h>
#include <ramdisk/include/services/JobctlMessages.h>

extern NodeState_t NodeState;

static uint64_t preload_jobid;

#ifndef true
#define true  1
#define false 0
#endif

#define ROUND_UP_PAGESIZE(p) (((p) + (CONFIG_MEM_PAGE_SIZE-1))  & ~CONFIG_MEM_PAGE_SIZE)
#define ROUND_DN_PAGESIZE(p) ((p) & ~(CONFIG_MEM_PAGE_SIZE-1))
#endif                                                                                                                 // FUSEDOS

#define NUM_NEEDED_PHDRS (3) // Code, Data, TLS. (Ignore Note and Stack)

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern int Process_SetupMap(bgcios::jobctl::LoadJobMessage *loadMsg, AppState_t **ppAppState);
extern int Process_SetupAgentMap(bgcios::jobctl::LoadJobMessage *loadMsg, AppState_t *parentAppState);
extern int Process_SetupJob(bgcios::jobctl::SetupJobMessage *setupJobMsg);
extern int elf_loadimage(AppState_t *appState, uint64_t physical_offset, uint64_t* agent_poffset_adjust);
extern "C" void UPC_Init(uint64_t jobID);
extern uint32_t GetEnvString(bgcios::jobctl::LoadJobMessage *msg, const char* envname, char** value);
extern uint32_t GetEnvValue(bgcios::jobctl::LoadJobMessage *msg, const char* envname, uint32_t* value);
extern uint32_t GetEnvValue64(bgcios::jobctl::LoadJobMessage *msg, const char* envname, uint64_t* value);
extern void App_SetupLoadLeader();
extern int  App_CheckGloballyForLoadFailure();

int ProcFS_ProcessInit(AppProcess_t* proc);
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "fusedos_config.h"                                                                                                // FUSEDOS
#include "fusedos.h"                                                                                                       // FUSEDOS
#include "cl_debug.h"                                                                                                  // FUSEDOS

extern int spc_0;
extern int running_spcml;                                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS



/*! \brief Returns the string of an environment variable
 *
 * \param[in] envname NULL terminated string containing the environment variable
 * \param[out] value pointer to store a pointer-to-a-string.
 * \return Boolean.  Does environment variable exist?  true/false.
 */
uint32_t  App_GetEnvString(const char* envname, const char** value)
{
    char* ptr = &(GetMyAppState()->App_Env[0]);
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
uint32_t App_GetEnvValue(const char* envname, uint32_t* value)
{
    const char* ptr;
    int rc = App_GetEnvString(envname, &ptr);
    if (rc==1)
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        *value = (uint32_t)atoi_(ptr);
#else                                                                                                                  // FUSEDOS
        *value = (uint32_t)atoi(ptr);                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    }
    return rc;
}

//#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint32_t App_GetEnvValue64(const char* envname, uint64_t* value)
{
    int base = 10;
    const char* ptr;
    int rc = App_GetEnvString(envname, &ptr);
    if (rc==1)
    {
        if((ptr[0] == '0')&&(toupper(ptr[1]) == 'B'))
            base = 2;
        if((ptr[0] == '0')&&(toupper(ptr[1]) == 'X'))
            base = 16;
        
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        *value = strtoull_(ptr, NULL, base);
#else                                                                                                                  // FUSEDOS
        *value = strtoull(ptr, NULL, base);
#endif                                                                                                                 // FUSEDOS
    }
    return rc;
}
//#endif                                                                                                                 // FUSEDOS


//
// App_MakePhdrs: create fake ELF Phdrs needed for GLIBC to learn
//    about the static TLS segment.
//
void  App_MakePhdrs( AppProcess_t *pProc, Elf64_Phdr *pPhdrs )
{
    // Phdr[0]: Code segment
    pPhdrs->p_type   = PT_LOAD;
    pPhdrs->p_offset = 0;
    pPhdrs->p_vaddr  = pProc->Text_VStart;
    pPhdrs->p_paddr  = pProc->Text_VStart;
    pPhdrs->p_filesz = (pProc->Text_VEnd - pProc->Text_VStart + 1);
    pPhdrs->p_memsz  = (pProc->Text_VEnd - pProc->Text_VStart + 1);
    pPhdrs->p_flags  = (PF_X | PF_R);
    pPhdrs->p_align  = 16;

    // Phdr[1]: Data segment
    pPhdrs++;
    pPhdrs->p_type   = PT_LOAD;
    pPhdrs->p_offset = 0;
    pPhdrs->p_vaddr  = pProc->Data_VStart;
    pPhdrs->p_paddr  = pProc->Data_VStart;
    pPhdrs->p_filesz = (pProc->Data_VEnd - pProc->Data_VStart + 1);
    pPhdrs->p_memsz  = (pProc->Data_VEnd - pProc->Data_VStart + 1);
    pPhdrs->p_flags  = (PF_W | PF_R);
    pPhdrs->p_align  = 16;

    // Phdr[2]: TLS segment (if any)
    pPhdrs++;
    pPhdrs->p_type   = PT_TLS;
    pPhdrs->p_offset = 0;
    pPhdrs->p_vaddr  = pProc->TLS_VStart;
    pPhdrs->p_paddr  = pProc->TLS_VStart;
    pPhdrs->p_filesz = pProc->TData_Size;
    pPhdrs->p_memsz  = (pProc->TLS_VEnd - pProc->TLS_VStart + 1); // sum of .tdata and .tbss
    pPhdrs->p_flags  = (PF_R);
    pPhdrs->p_align  = 8;

}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
void App_CacheLineLockingConfig( AppProcess_t *pProc )
{
    uint32_t value;

    if (App_GetEnvValue("BG_ALLOW_CACHELINE_LOCKING", &value) && (value != 0))
    {
	pProc->msrRequired |= MSR_UCLE;
	pProc->msrForbidden &= ~MSR_UCLE;
    }

    if (!App_GetEnvValue("BG_L2_OVERLOCK_THRESHOLD", &value) || (value >= 16))
    {
        value = 0;   // restore default
    }
    l2_set_overlock_threshold(value);
    l2_set_spec_threshold(value);

    if (!App_GetEnvValue("BG_L2LOCK_L1_ONLY", &value) || (value != 1))
    {
	value = 0;
    }
    l2_l1pcfg_lock_l1_only(value);

    if (!App_GetEnvValue("BG_L2LOCK_CLEAR_SET", &value) || (value != 1))
    {
	value = 0;
    }
    l2_l1pcfg_clear_locks_in_set(value);

    // Clear any L2 locks that have been left in place.
    // The following call needs a virtual address that is mapped to a
    // contiguous physical region 2 megabytes in size and aligned on a
    // 2-megabyte boundary.  We can't use 0 because that address is sometimes
    // not mapped (for the sake of catching NULL-pointer dereferences).
    l2_unlock_all_with_address((void *) 0x200000);
}
#endif                                                                                                                 // FUSEDOS

// Set the Application Load State and associated data
void App_SetLoadState(uint32_t loadState, uint32_t returnCode, uint32_t errorCode)
{
    AppState_t *app = GetMyAppState();
    app->LoadStateReturnCode = returnCode;
    app->LoadStateErrorCode = errorCode;
    app->LoadState = loadState;
    ppc_msync();
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Test the progress of the Application load that is taking place. If an error has been
// detected, gracefully exit the loading sequence
static void App_TestLoadProgress(AppState_t *pAppState,uint32_t returnCode, uint32_t errorcode)
{
    Kernel_Barrier(Barrier_HwthreadsInApp);
    if ( IsAppLeader() )
    {
        if (App_CheckGloballyForLoadFailure() != 0)
        {
            if(GetMyAppState()->LoadState != AppState_LoadFailed)
            {
                App_SetLoadState(AppState_LoadFailed, returnCode, errorcode);
            }
        }
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);
    // Test to see if an application load error has been detected. 
    if (pAppState->LoadState == AppState_LoadFailed)
    {
        if (IsProcessLeader())
        {
            App_ProcessExit(0); // just provide normal exit status. The LoadError state info will indicate the load problem
            // Will not reach here.
        }
        else
        {
            Scheduler(); // All other threads enter the scheduler so that we can be interrupted by the App_ProcessExit handling 
        }
        // Will not reach here.
    }
}
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif                                                                                                                 // FUSEDOS

//
// App_CreateMainThread: Create the main() application thread.
//
// The ppc process initialization abi requires the following
//  r1   = initial stack pointer, 16-byte aligned and pointing to a NULL
//  r3   = argc = number of argument pointers
//  r4   = argv = pointer to array of argument pointers, last is NULL
//  r5   = envp = pointer to array of environment pointers, last is NULL
//  r6   = auxp = pointer to array of aux entries, last is NULL
//  r7   = 0 (or a termination function to be registered with atexit)
// fpscr = 0 (round-to-nearest, ieee-mode, fp-exceptions-disabled)
//
// The stack image looks like this:
//
// Hi Addrs           PHdrs
//                    Environment
//                    Argument strings
//                    Add 4K Page for local stack space. (glibc shouldn't be writing here but does)
//                    argc               <---- R1
//                    argv[0]            <---- R4
//                    ...
//                    NULL
//                    env var pointers   <---- R5
//                    ...
//                    NULL
//                    auxvec             <---- R6
//                    ...
//                    AT_NULL
//                    0
//                    ...                stack grows toward zero
//  Lower Addrs
//
// get app process from core id an thread number
void  App_CreateMainThread( uint64_t app_start, int priv )
{
    int threadID = ProcessorThreadID();
    int coreID   = ProcessorCoreID();

    HWThreadState_t *pHWThreadState = GetHWThreadState(coreID, threadID);
    AppState_t *pAppState = GetMyAppState();
    int pcount = pAppState->Active_Processes;
    // We blindly consume the first slot entry in this hardware thread for the Process Leader
    KThread_t *pKThr = pHWThreadState->SchedSlot[CONFIG_SCHED_SLOT_FIRST];
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS    
    incrementSlotAllocatedCount(pHWThreadState);
#endif                                                                                                                 // FUSEDOS

    // Set indicator within this KThread that it is a process leader thread
    pKThr->isProcessLeader = 1;
    
    Regs_t  *pRegs = &pKThr->Reg_State;
    AppProcess_t *pProc = pKThr->pAppProc;
    pProc->ProcessLeader_KThread = pKThr;
    pProc->PID = GetTID(pKThr);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS    
    ProcFS_ProcessInit(pProc);  // we now know the PID, create /proc files
#endif                                                                                                                 // FUSEDOS
    
    pProc->msrRequired = priv ? (MSR_CNK|MSR_EE|MSR_FP) : MSR_APP_REQUIRED;
    pProc->msrForbidden = priv ? 0 : MSR_APP_FORBIDDEN;
    uint64_t app_sp = 0, app_tos = 0, app_bos = 0;
    char *pEnv, *pArgs, *pC;
    uint64_t *pI;
    uint64_t app_r3, app_r4, app_r5, app_r6, app_r13 = 0;
    Elf64_Phdr *pPhdrs;
    int i;

    app_bos = (pProc->Heap_VEnd - 128);
    app_bos = ((app_bos+127) & ~127);  
    app_tos = (pProc->Heap_VStart);
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACE(TRACE_Process, ("FUSEDOS App_CreateMainThread: app_tos 0x%016lx, app_bos 0x%016lx\n",                       // FUSEDOS
             app_tos, app_bos));                                                                                       // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    // initialize the heap segment
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    if ( !Personality_IsSimulation() )
#endif                                                                                                                 // FUSEDOS
    {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
	  if(!running_spcml)                                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      memset((void*)app_tos, 0, app_bos-app_tos);
    }
    
    TRACE( TRACE_Process, ("(I) core/thread:[%d/%d]: app_tos=%016lx  app_bos=%016lx\n", coreID, threadID, app_tos, app_bos ));

    // remember stack bounds and init TLS info
    pKThr->pUserStack_Top      = (void*)app_tos;
    pKThr->pUserStack_Bot      = (void*)app_bos;
    pKThr->Clone_Flags    = 0;
    pKThr->pTLS_Area      = 0;
    pKThr->pParent_TID    = 0;
    pKThr->pChild_TID     = 0;
    pKThr->TID_Address    = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    pKThr->FutexQueueNext = (KThread_t *)0;
#endif                                                                                                                 // FUSEDOS
    pKThr->Pending        = 0;
    pKThr->AlignmentExceptionCount = 0;
    pKThr->physical_pid = pProc->PhysicalPID;
    if (pProc->Guard_Enable)
    {
        // Setup the guard register values for this kthread. Must use real addresses. Since the 
        // heap/stack segment contains contiguous real memory, the conversion is simple.
        pKThr->GuardBaseAddress = pProc->Heap_PStart + (pProc->Guard_Start - pProc->Heap_VStart); 
        pKThr->GuardEnableMask = ~(((uint64_t)pProc->Guard_Size)-1);
    }
    // Set the default Policy and priority
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    pKThr->Policy = SCHED_FIFO;
#endif                                                                                                                 // FUSEDOS
    pKThr->Priority = CONFIG_SCHED_MIN_FIFO_PRIORITY;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    App_CacheLineLockingConfig( pProc );
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    if(!running_spcml) {                                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    // Create PHdrs on the top of the Stack, which will be passed to App via AuxVec's
    app_sp = app_bos;
    app_sp -= (sizeof(Elf64_Phdr) * NUM_NEEDED_PHDRS);
    pPhdrs = (Elf64_Phdr *)app_sp;
    App_MakePhdrs( pProc, pPhdrs );

    // Env Vars
    app_sp -= APP_MAX_ENVVARS;
    pEnv   = (char *)app_sp;
    memcpy( pEnv, &(pAppState->App_Env[0]), APP_MAX_ENVVARS );

    // Arg Strings
    app_sp -= APP_MAX_ARGSTR;
    pArgs  = (char *)app_sp;
    app_r3 = pAppState->App_Argc;
    
    int loaderNameLength = 0;
    if(pAppState->invokeWithInterpreter)
    {
        loaderNameLength = strlen(pAppState->InterpreterPathAndName) + 1; // Length of the loader path/name including the terminating NULL character
        strncpy(pArgs,pAppState->InterpreterPathAndName, loaderNameLength);
        app_r3++;
    }
    memcpy( pArgs + pAppState->invokeWithInterpreter * loaderNameLength, &(pAppState->App_Args[0]), APP_MAX_ARGSTR-pAppState->invokeWithInterpreter * loaderNameLength );
    
    // glibc does a __sigemptyset() that wants to overwrite
    //  the current stack area.  So leave a gap for this.
    app_sp -= 4096;

    app_sp &= ~0x1F; // round down to 32B boundary
    pI = (uint64_t *)app_sp;
    
    // Argc, followed by Argv pointers plus NULL
    *pI++  = app_r3;
    app_r4 = (uint64_t)pI;
    for ( pC = pArgs, i = 0 ; i < (int)app_r3 ; i++ )
    {
        *pI++ = (uint64_t)pC;
        pC += (strlen(pC) + 1);
    }
    *pI++ = 0;
    
    // Array of Ptrs to Environment variables
    app_r5 = (uint64_t)pI;
    for ( pC = pEnv ; *pC ; )
    {
        *pI++ = (uint64_t)pC;
        pC += (strlen(pC) + 1);
    }
    *pI++ = 0;

    // Aux Vectors, terminated by AT_NULL.
    app_r6 = (uint64_t)pI;
    pProc->pAuxVectors = pI;

    *pI++ = AT_DCACHEBSIZE;
    *pI++ = L1D_CACHE_LINE_SIZE;
    *pI++ = AT_PAGESZ;
    *pI++ = CONFIG_MEM_PAGE_SIZE;  // when malloc calls mmap for more mem, it adds pagesize for mgt space. so keep this small.
    *pI++ = AT_PHNUM;
    *pI++ = NUM_NEEDED_PHDRS;
    *pI++ = AT_PHDR;
    *pI++ = (uint64_t)pPhdrs;
    *pI++ = AT_UID;
    *pI++ = (pAppState->UserID ? pAppState->UserID : 1);
    *pI++ = AT_EUID;
    *pI++ = (pAppState->UserID ? pAppState->UserID : 1);
    *pI++ = AT_GID;
    *pI++ = (pAppState->GroupID ? pAppState->GroupID : 1);
    *pI++ = AT_EGID;
    *pI++ = (pAppState->GroupID ? pAppState->GroupID : 1);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Dynamic app?
    if ( pProc->sharedObjectFile || pAppState->invokeWithInterpreter )
    {
        *pI++ = AT_BASE;
        *pI++ = pProc->interpreterBase;
    }
#endif                                                                                                                 // FUSEDOS
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#define AT_BG_PRCOUNT 38                                                                                               // FUSEDOS
#define AT_BG_CPUCOUNT 39                                                                                              // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    *pI++ = AT_BG_PRCOUNT;
    *pI++ = pcount;
    *pI++ = AT_BG_CPUCOUNT;
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    *pI++ = NodeState.cl_numspcs;                                                                                      // FUSEDOS
#else                                                                                                                  // FUSEDOS
    *pI++ = pProc->HWThread_Count;
#endif                                                                                                                 // FUSEDOS
    *pI++ = AT_CLKTCK;
    *pI++ = CONFIG_TICK_RATE;
    *pI++ = AT_NULL;
    *pI++ = 0;

#if defined __FUSEDOS__                                                                                               // FUSEDOS
     } else {                                                                                                          // FUSEDOS
       app_r3 = app_r4 = app_r5 = app_r6 = 0;                                                                          // FUSEDOS
     }                                                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    // With NPTL, R13 is the ABI's Thread Local Storage (TLS) pointer.
    //  There is a catch-22 during early glibc startup, where __sbrk()
    //  is called to allocate the per-thread TLS structures, which among
    //  other things, contains errno (via __errno_location()).  If the
    //  first glibc __sbrk() (for each/every thread) should fail for any
    //  reason, glibc will store offset ~7k from uninitialized variable to write
    //  errno.  Setting r2 to zero doesn't help with that, but makes it more
    //  likely that an errant errno store will hit the kernel (currently located
    //  starting at VAddr zero) causing a DSI.
    //
    //  **** Glibc needs to be fixed. ****


/////////////// _PRE_TCB_SIZE   sizeof (struct _pthread_descr_struct)
/////////////// +
/////////////// +/* the following assumes that TP (R13) is points to the end of the TCB
/////////////// +   + 0x7000 (per the ABI).  This implies that tcb address is
/////////////// +   R13-(TLS_TCB_SIZE + 0x7000).  As we define TLS_DTV_AT_TP we can
/////////////// +   assume that the pthread_descr is allocate immediately ahead of the
/////////////// +   TCB.  This implies that the pthread_descr address is
/////////////// +   R13-(TLS_PRE_TCB_SIZE + TLS_TCB_SIZE + 0x7000).  */
/////////////// +#  define TLS_TCB_OFFSET 0x7000
//   app_r13 = 0x0a000000;
    
    // Fill in thread's register state
    memset( pRegs, 0, sizeof(Regs_t) );
    pRegs->gpr[1]  = app_sp;
    pRegs->gpr[2]  = ((uint64_t*)app_start)[1] + pProc->interpreterBase; // TOC pointer
    pRegs->gpr[3]  = app_r3;
    pRegs->gpr[4]  = app_r4;
    pRegs->gpr[5]  = app_r5;
    pRegs->gpr[6]  = app_r6;
    pRegs->gpr[7]  = 0; // ABI required
    pRegs->gpr[13] = app_r13; // Thread Local Storage
    pRegs->ip      = ((uint64_t*)app_start)[0] + pProc->interpreterBase;
    pRegs->msr     = pProc->msrRequired | (pProc->toolAttached ? MSR_DE : 0);
    pRegs->pid     = pProc->PhysicalPID;
    pRegs->gsprg[0]  = 0;                                    // See cnk/bgp_SPRG_Usage.h
    pRegs->gsprg[1]  = 0;                                    // See cnk/bgp_SPRG_Usage.h
    pRegs->gsprg[2]  = 0;                                    // See cnk/bgp_SPRG_Usage.h
    pRegs->gsprg[3]  = 0;                                    // See cnk/bgp_SPRG_Usage.h
    pRegs->sprg[SPRG_Index_CritR3] = 0;
    pRegs->sprg[SPRG_Index_CritR4] = 0;
    pRegs->sprg[SPRG_Index_CritR5] = 0;
    pRegs->sprg[SPRG_Index_StdR3] =  0;
    pRegs->sprg[SPRG_Index_StdR4] =  0;
    pRegs->sprg[SPRG_Index_StdR5] =  0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    pRegs->sprg[SPRG_Index_HWThread]= mfspr(SPRG_pHWThread); //  see SPRG_Usage.h
#endif                                                                                                                 // FUSEDOS
    TRACE( TRACE_Jobctl, ("(I) App_CreateMainThread: ip=%lx msr=%lx sp=%lx pid=%lu\n", pRegs->ip, pRegs->msr, pRegs->gpr[1], pRegs->pid) );       
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    spc_context_t* pc = get_spc_context(this_spc());                                                                   // FUSEDOS
    memset(&(pc->regs), 0, sizeof(regs_t));                                                                            // FUSEDOS
    pc->regs.gpr[1] = pRegs->gpr[1];                                                                                   // FUSEDOS
    pc->regs.gpr[2] = pRegs->gpr[2];                                                                                   // FUSEDOS
    pc->regs.gpr[3] = pRegs->gpr[3];                                                                                   // FUSEDOS
    pc->regs.gpr[4] = pRegs->gpr[4];                                                                                   // FUSEDOS
    pc->regs.gpr[5] = pRegs->gpr[5];                                                                                   // FUSEDOS
    pc->regs.gpr[6] = pRegs->gpr[6];                                                                                   // FUSEDOS
    pc->regs.gpr[7] = pRegs->gpr[7];                                                                                   // FUSEDOS
    pc->regs.gpr[13] = pRegs->gpr[13];                                                                                 // FUSEDOS
    pc->regs.ip = pRegs->ip;                                                                                           // FUSEDOS
    pc->regs.msr = pRegs->msr;                                                                                         // FUSEDOS
    pc->regs.pid = pRegs->pid;                                                                                         // FUSEDOS
    pc->regs.gsprg[0] = pRegs->gsprg[0];                                                                               // FUSEDOS
    pc->regs.gsprg[1] = pRegs->gsprg[1];                                                                               // FUSEDOS
    pc->regs.gsprg[2] = pRegs->gsprg[2];                                                                               // FUSEDOS
    pc->regs.gsprg[3] = pRegs->gsprg[3];                                                                               // FUSEDOS
    pc->regs.sprg[SPRG_Index_CritR3] = pRegs->sprg[SPRG_Index_CritR3];                                                 // FUSEDOS
    pc->regs.sprg[SPRG_Index_CritR4] = pRegs->sprg[SPRG_Index_CritR4];                                                 // FUSEDOS
    pc->regs.sprg[SPRG_Index_CritR5] = pRegs->sprg[SPRG_Index_CritR5];                                                 // FUSEDOS
    pc->regs.sprg[SPRG_Index_StdR3] = pRegs->sprg[SPRG_Index_StdR3];                                                   // FUSEDOS
    pc->regs.sprg[SPRG_Index_StdR4] = pRegs->sprg[SPRG_Index_StdR4];                                                   // FUSEDOS
    pc->regs.sprg[SPRG_Index_StdR5] = pRegs->sprg[SPRG_Index_StdR5];                                                   // FUSEDOS
    pc->regs.sprg[SPRG_Index_SPIinfo] =                                                                                // FUSEDOS
        (((uint64_t)(pAppState->AppStateIndex)) << SPRG_SPIinfo_AppLeaderCore) | // byte3 bits 0:3 App Leader core index. // FUSEDOS
        (((uint64_t)(pAppState->Active_Cores-1)) << SPRG_SPIinfo_NumCores) |     // byte3 bits 4:7 Number of cores in App - 1 // FUSEDOS
        (pAppState->Active_Processes << SPRG_SPIinfo_NumProcs) |                 // byte6: number of processes         // FUSEDOS
        (ProcessorID());                                                         // byte7: hw thread index             // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    ppc_msync();
    // Is this thread an AppAgent?    
    if (!IsAppAgent())
    {
        // Barrier with the other process leaders
        Kernel_Barrier(Barrier_ProcessLeadersInApp);
    }

    // If we are the AppLeader, set flag in the AppState object to indicate that all 
    // Process leaders are loaded and ready to be started
    if (IsAppLeader())
    {

        App_SetLoadState(AppState_LoadComplete, 0, 0);

        // Send message to the Node Controller that the load of the job is completed 
        if (Personality_CiosEnabled()) 
        {
            if (!IsAppAgent())
            {
                jobControl.loadJobAck(pAppState->JobID, bgcios::Success, 0);
            }
        }

	uint32_t doCrcExchange = true;
	App_GetEnvValue("BG_CRC_EXCHANGE", &doCrcExchange);
        if(Personality_IsFPGA() || ((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_ND) == 0))
            doCrcExchange = false;
	if(doCrcExchange != 0)
	{
	    int rc = 0;
	    if ( ( rc = crcx_initialize() ) != 0 ) {
		printf("(E) crcx_initialize failed with rc=%d\n", rc);
	    }
	}
    }

    // Set the kthread state
    pKThr->State = SCHED_STATE_RESET;
    // Set the state of this KThread to indicated that it is loaded and ready to be run
    Scheduler();
    // Previous call does not return
#else                                                                                                                  // FUSEDOS
    return;                                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
//
// Initialize the KernelInfo structure that is appended to the app's
// text segment.
//
void App_InitKernelInfo(AppProcess_t *proc, AppState_t *app)
{
    TRACE( TRACE_Process, ("(I) App_InitKernelInfo%s: address=0x%lx\n",
	   whoami(), app->KernelInfoAddress) );

    // carry over address of KernelInfo structure
    proc->KernelInfoAddress = app->KernelInfoAddress;

    if (IsAppLeader() || !app->SharedTextSeg)
    {
	KernelInfo_t *ki = (KernelInfo_t *) proc->KernelInfoAddress;
	Signal_TrampolineInit(&ki->SigTramp);
    }
}
#endif                                                                                                                 // FUSEDOS

struct SegmentRanges
{
    uint64_t text_start; 
    uint64_t text_pstart; 
    uint64_t text_end; 
    uint64_t text_size; 
    uint64_t data_start; 
    uint64_t data_pstart; 
    uint64_t data_end; 
    uint64_t data_size; 
    uint64_t heap_start; 
    uint64_t tls_start; 
    uint64_t tbss_start; 
    uint64_t dyn_start; 
    uint64_t dyn_end; 
    uint64_t dyn_size; 
    uint64_t dyn_pstart; 

    uint32_t text_startMB;
    uint32_t text_pstartMB;
    uint32_t text_endMB;
    uint32_t text_sizeMB;
    uint32_t data_startMB;
    uint32_t data_pstartMB;
    uint32_t data_endMB;
    uint32_t data_sizeMB;
    uint32_t heap_startMB;
    uint32_t tdata_size;
    uint32_t tbss_size;
    uint32_t dyn_startMB;
    uint32_t dyn_endMB;
    uint32_t dyn_sizeMB;
    uint32_t dyn_pstartMB;
};

int App_GenSegmentRanges(AppState_t &appState, SegmentRanges &segment_range)
{
    int      appsegs  = appState.App_NumSegs;
    // run through the app's segment list, accumulating text/data segment bounds
    segment_range.dyn_start = -1ull;
    segment_range.data_start = -1ull;
    segment_range.text_start = -1ull;
    segment_range.dyn_end = 0ull;
    segment_range.data_end = 0ull;
    segment_range.text_end = 0ull;
    for (int i = 0 ; i < appsegs ; i++ )
    {
        AppSeg_t& appSeg = appState.App_Segments[i];
        uint32_t  segflags  = appSeg.seg_flags;
        uint64_t  segstart  = appSeg.seg_va;
        uint64_t  segpstart = appSeg.seg_pa;
        uint64_t  segmemsz  = appSeg.seg_memsz;
        uint64_t  segfilesz = appSeg.seg_filesz;
        uint64_t  segend    = (segstart + segmemsz);

        TRACE( TRACE_VMM,
                ("(I) App_Seg[%2d]: %c%c%c%c%c %ld bytes at V=0x%016lx P=0x%016lx (%ld bytes initialized).\n", i,
                 (segflags & APP_FLAGS_R   ? 'r' : '-'),
                 (segflags & APP_FLAGS_W   ? 'w' : '-'),
                 (segflags & APP_FLAGS_X   ? 'x' : '-'),
                 (segflags & APP_FLAGS_TLS ? 'T' : '-'),
                 (segflags & APP_FLAGS_DYN ? 'D' : '-'),
                 appSeg.seg_memsz, appSeg.seg_va, appSeg.seg_pa, appSeg.seg_filesz ) );

        if ( segflags & APP_FLAGS_DYN )
        {
            if ( segstart < segment_range.dyn_start )
            {
                segment_range.dyn_start = segstart;
                segment_range.dyn_pstart= segpstart;
            }
            if ( segend > segment_range.dyn_end )
                segment_range.dyn_end = segend;
        }
        else if ( segflags & APP_FLAGS_TLS )
        {
	    segment_range.tls_start  = segstart;
	    segment_range.tdata_size = segfilesz;
	    segment_range.tbss_start = segstart + segfilesz;
	    segment_range.tbss_size  = segmemsz - segfilesz;
	}
        else if ( segflags & APP_FLAGS_W ) // \note rodata segments are marked R-X.
        {
            if ( segstart < segment_range.data_start )
            {
                segment_range.data_start = segstart;
                segment_range.data_pstart = segpstart;
            }

            if ( segend > segment_range.data_end )
                segment_range.data_end = segend;
        }
        else // non-writeable segment
        {
            if ( segstart < segment_range.text_start )
            {
                segment_range.text_start = segstart;
                segment_range.text_pstart = segpstart;
            }
            if ( segend > segment_range.text_end )
                segment_range.text_end = segend;
            //printf("text start %016lx end %016lx size %016lx\n", segment_range.text_start, segment_range.text_end, segment_range.text_size);
        }
    } // rof appsegs

    // Carve out space for miscellaneous kernel information to be inserted
    // into the application text segment.
    // NOTE: at this point app_text_end points one byte BEYOND the end of text.
    uint64_t kernel_info = (segment_range.text_end + (KERNEL_INFO_SPACE-1)) &
						~(KERNEL_INFO_SPACE-1);
    segment_range.text_end = kernel_info + KERNEL_INFO_SPACE;
    if ((kernel_info <= segment_range.data_start) &&
	(segment_range.text_end > segment_range.data_start))
    {
	printf("(E) No room for KernelInfo between text and data segments.\n");
	return(-1);
    }

    // describe Process 0's virtual space
    segment_range.text_start   = ROUND_DN_1M(segment_range.text_start);
    segment_range.text_pstart  = ROUND_DN_1M(segment_range.text_pstart);
    segment_range.text_end     = (ROUND_UP_1M(segment_range.text_end) - 1);
    segment_range.text_size    = (segment_range.text_end - segment_range.text_start + 1);
    segment_range.text_startMB = (segment_range.text_start / CONFIG_CNK_PAGE_SIZE);
    segment_range.text_pstartMB = (segment_range.text_pstart / CONFIG_CNK_PAGE_SIZE);
    segment_range.text_endMB   = (segment_range.text_end   / CONFIG_CNK_PAGE_SIZE);
    segment_range.text_sizeMB  = (segment_range.text_endMB - segment_range.text_startMB + 1);

    segment_range.data_start   = ROUND_DN_1M(segment_range.data_start);
    segment_range.data_pstart  = ROUND_DN_1M(segment_range.data_pstart);
    segment_range.data_end     = (ROUND_UP_1M(segment_range.data_end) - 1);
    segment_range.data_size    = (segment_range.data_end - segment_range.data_start + 1);
    segment_range.data_startMB = (segment_range.data_start / CONFIG_CNK_PAGE_SIZE);
    segment_range.data_pstartMB = (segment_range.data_pstart / CONFIG_CNK_PAGE_SIZE);
    segment_range.data_endMB   = (segment_range.data_end   / CONFIG_CNK_PAGE_SIZE);
    segment_range.data_sizeMB  = (segment_range.data_endMB - segment_range.data_startMB + 1);

    segment_range.dyn_start    = ROUND_DN_1M(segment_range.dyn_start);
    segment_range.dyn_end      = (ROUND_UP_1M(segment_range.dyn_end) - 1);
    segment_range.dyn_size     = (segment_range.dyn_end - segment_range.dyn_start + 1);
    segment_range.dyn_startMB  = (segment_range.dyn_start / CONFIG_CNK_PAGE_SIZE);
    segment_range.dyn_pstartMB = (segment_range.dyn_pstart / CONFIG_CNK_PAGE_SIZE);
    segment_range.dyn_endMB    = (segment_range.dyn_end   / CONFIG_CNK_PAGE_SIZE);
    segment_range.dyn_sizeMB   = (segment_range.dyn_endMB - segment_range.dyn_startMB + 1);

    segment_range.heap_start   = (segment_range.data_end + 1);
    segment_range.heap_startMB = segment_range.data_endMB;

    // record address of KernelInfo structure
    appState.KernelInfoAddress = kernel_info;
    
    // TLS virtual
    appState.TLS_VStart  = segment_range.tls_start;
    appState.TLS_VEnd    = (segment_range.tls_start + segment_range.tdata_size + segment_range.tbss_size - 1);
    
    // record the TData and TBSS start and size for use in thread creation
    appState.pTData_Start = (void *)segment_range.tls_start;
    appState.TData_Size   = segment_range.tdata_size;
    appState.pTBSS_Start  = (void *)segment_range.tbss_start;
    appState.TBSS_Size    = segment_range.tbss_size;

    return 0;
}


//
// Use the Personality and the ProcessMap Setup to apportion
//  the memory among the processes.
//
//     We need to come up with an
//     allocation that makes most efficient use of the available
//     TLB slots. That means sacrificing some (perhaps lots) memory
//     to ensure similar Virtual and Physical alignment for pages so
//     the TLB mapping can ever make use of 16M, 256M and 1G pages.
//
// Called by Init HwThread while other cores/threads wait in a barrier.
//
// Responsibilities of this routine:
//  1) Learn what memory is available (exclude kernel memory and "reserved" memory)
//  2) Assign Heap Space to Each Process.
//
// Note: ABI places requirements regarding Stacks in multi-threaded programs.
//
int  App_SetupMemory( Personality_t *pPers)
{
    AppState_t *pAppState = GetMyAppState();
    uint32_t numProcs = pAppState->Active_Processes;
    int      numAgents = popcnt64(NodeState.AppAgents);
    int rc;
    int i = 0;

    SegmentRanges appSegmentRange;
    SegmentRanges agentSegmentRange[CONFIG_MAX_APP_AGENTS];
    // Initialize the data structures
    memset (&appSegmentRange, 0x00, sizeof(appSegmentRange));
    memset (agentSegmentRange, 0x00, sizeof(agentSegmentRange));

#if defined __FUSEDOS__                                                                                               // FUSEDOS
	// add MMIO segment for MU memory so that TLB mapping algorithm                                                    // FUSEDOS
	// considers the resulting TLB entry                                                                               // FUSEDOS
	if( ! running_spcml ) {                                                                                              // FUSEDOS
			/* TODO MARIUS experiment
		vmm_addSegment(IS_MMIO, PHYMAP_MINADDR_MU/CONFIG_CNK_PAGE_SIZE,                                                // FUSEDOS
			PHYMAP_MINADDR_MU/CONFIG_CNK_PAGE_SIZE, 16, -1, VMM_SEGMENTCOREMASK_APP,                                   // FUSEDOS
			true, false, true, true, true, false);                                                                     // FUSEDOS
		vmm_addSegment(IS_MMIO, PHYMAP_MINADDR_DCR/CONFIG_CNK_PAGE_SIZE,                                               // FUSEDOS
			PHYMAP_MINADDR_DCR/CONFIG_CNK_PAGE_SIZE, 16, -1, VMM_SEGMENTCOREMASK_APP,                                  // FUSEDOS
			true, false, true, true, true, false);                                                                     // FUSEDOS
			*/
	}                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    // Generate the segment ranges for the main application
    rc = App_GenSegmentRanges(*pAppState, appSegmentRange);
    if(rc)
        return rc;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Generate the segment ranges for each of the Application agents
    for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
    {
        if (NodeState.AppAgents & _BN(i))
        {
	    AppState_t *agentAppState =
		&NodeState.AppState[CONFIG_FIRST_AGENT_APPSTATE_INDEX+i];
            rc = App_GenSegmentRanges(*agentAppState, agentSegmentRange[i]);
	    if(rc)
		return rc;
	    agentAppState->SharedTextSeg = 0;
        }
    }
#endif                                                                                                                 // FUSEDOS

    //printf("App addSegment: vtext: %d ptext: %d size: %d\n", appSegmentRange.text_startMB, appSegmentRange.text_pstartMB, appSegmentRange.text_sizeMB );
    
    uint32_t nosharedtext = !CNK_SHAREDTEXT_DEFAULT;
    App_GetEnvValue("BG_DISABLESHAREDTEXT", &nosharedtext);  // non-shared text is experimental
    pAppState->SharedTextSeg = !nosharedtext;
    
    if(pAppState->SharedTextSeg)
    {
        rc = vmm_addSegment(IS_TEXT, appSegmentRange.text_startMB, appSegmentRange.text_pstartMB, appSegmentRange.text_sizeMB, -1, VMM_SEGMENTCOREMASK_APP, true, false, true, !pAppState->invokeWithInterpreter, true, false);
		CL_DEBUG("rc = %d\n", rc);
        if(rc)
            return rc;
    }
    else
    {
        for ( i = 0 ; (i < (int)numProcs) && (i < CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE) ; i++ )
        {
            rc = vmm_addSegment(IS_TEXT, appSegmentRange.text_startMB, appSegmentRange.text_pstartMB, appSegmentRange.text_sizeMB, i, VMM_SEGMENTCOREMASK_APP, true, false, true, !pAppState->invokeWithInterpreter, true, false);
		CL_DEBUG("rc = %d\n", rc);
            if(rc)
                return rc;
        }
    }

    for ( i = 0 ; (i < (int)numProcs) && (i < CONFIG_MAX_CORES * CONFIG_HWTHREADS_PER_CORE) ; i++ )
    {
        //printf("App addSegment: vdata: %d pdata: %d size: %d\n", appSegmentRange.data_startMB, appSegmentRange.data_pstartMB, appSegmentRange.data_sizeMB );
        rc = vmm_addSegment(IS_DATA, appSegmentRange.data_startMB, appSegmentRange.data_pstartMB, appSegmentRange.data_sizeMB, i, VMM_SEGMENTCOREMASK_APP, true, true, true, !pAppState->invokeWithInterpreter, false, true);
		CL_DEBUG("rc = %d\n", rc);

        if(rc)
	    return rc;
        if (pAppState->invokeWithInterpreter) 
        {
            rc = vmm_addSegment(IS_DYNAM, appSegmentRange.dyn_startMB, appSegmentRange.dyn_pstartMB, appSegmentRange.dyn_sizeMB, i, VMM_SEGMENTCOREMASK_APP, true, true, false, true, false, false);
		CL_DEBUG("rc = %d\n", rc);
	    if(rc)
                return rc;
        }
    }

    // Add the segments for the Application agents.  
    // CURRENT LIMITATION: assumes App agents are only running on a fully configured 17 core node
    if (numAgents)
    {
        uint32_t agentHeapSize[CONFIG_MAX_APP_AGENTS];
        for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
        {
            agentHeapSize[i] = CONFIG_AGENT_HEAPSIZE_DEFAULT;
        }
        const char *agentPath;
        if ((App_GetEnvString("BG_APPAGENTCOMM", &agentPath) == 1) &&
            (strncmp(agentPath, "DISABLE", strlen("DISABLE")) == 0))
        {
            // Comm agent is disabled. The User Agent heap specification goes into the first slot
            App_GetEnvValue("BG_AGENTHEAPSIZE", &agentHeapSize[0]);
        }
        else
        {
            App_GetEnvValue("BG_AGENTCOMMHEAPSIZE", &agentHeapSize[0]);
            App_GetEnvValue("BG_AGENTHEAPSIZE", &agentHeapSize[1]);
        }

        for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
        {
            if (NodeState.AppAgents & _BN(i))
            {
                //printf("Agent addSegment: vtext: %d ptext: %d size: %d\n", agentSegmentRange[i].text_startMB, agentSegmentRange[i].text_pstartMB, agentSegmentRange[i].text_sizeMB );
                //printf("Agent addSegment: vdata: %d pdata: %d size: %d\n", agentSegmentRange[i].data_startMB, agentSegmentRange[i].data_pstartMB, agentSegmentRange[i].data_sizeMB );
                rc = vmm_addSegment(IS_TEXT, agentSegmentRange[i].text_startMB, agentSegmentRange[i].text_pstartMB, agentSegmentRange[i].text_sizeMB, CONFIG_MAX_APP_PROCESSES+i, VMM_SEGMENTCOREMASK_16, true, false,true, true, true, false);
                if(rc) return rc;
                rc = vmm_addSegment(IS_DATA, agentSegmentRange[i].data_startMB, agentSegmentRange[i].data_pstartMB, agentSegmentRange[i].data_sizeMB, CONFIG_MAX_APP_PROCESSES+i, VMM_SEGMENTCOREMASK_16, true, false, true, true, false, false);
                if(rc) return rc;
                rc = vmm_addSegment(IS_HEAP, 0,                                 0,                                  agentHeapSize[i],    CONFIG_MAX_APP_PROCESSES+i,              VMM_SEGMENTCOREMASK_16, true, true, false,false,false, false);
                if(rc) return rc;
            }
        }
    }

    uint32_t sharedmem = ((pAppState->Active_Processes == 1) ? CONFIG_DEFAULT_MIN_SHAREDMEMSIZE : CONFIG_DEFAULT_SHAREDMEMSIZE); 
    uint32_t persistmem = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint32_t persistReset = 0;
    const char *sharedmemstring;
    // Was the shared memory env var specified?
    if (App_GetEnvString("BG_SHAREDMEMSIZE", &sharedmemstring))
    {
        // Are we being requested to add to the default allocation
        if (*sharedmemstring == '+')
        {
            sharedmem += (uint32_t)atoi_(sharedmemstring); // add to current default. The atoi_ code will eat the '+' character
        }
        else
        {
            sharedmem = (uint32_t)atoi_(sharedmemstring); // set the shared memory to the value provided
        }
    }
#else                                                                                                                  // FUSEDOS
	sharedmem = NodeState.cl_shm_size;                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    App_GetEnvValue( "BG_PERSISTMEMRESET", &persistReset );
    if (!IsSubNodeJob()) // Persistent memory is not supported for subnode jobs
    {
        App_GetEnvValue("BG_PERSISTMEMSIZE", &persistmem);
    }
    
    // If persistent memory reset has been requested and a previous allocation existed, or the size of 
    // the new persistent memory has changed (including a size of 0) we must clear that data.
    if (NodeState.PersistentMemory.VStart &&  // Test for an allocation from a previous job 
        (persistReset || (persistmem != (NodeState.PersistentMemory.Size/VMM_PAGE_SIZE_1M)))) // Reset requested or a size change
    {
        uint64_t startPage = NodeState.PersistentMemory.PStart/VMM_PAGE_SIZE_1M;
        uint64_t numPages  = NodeState.PersistentMemory.Size/VMM_PAGE_SIZE_1M;
        uint64_t tempV     = 0x0000000400000000ull;
        
        for(uint64_t i=0; i < numPages; i++)
        {
            vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                              (void*)((startPage+i)*VMM_PAGE_SIZE_1M),
                              (void*)tempV,
                              VMM_PAGE_SIZE_1M,0,0);
            memset((void*)tempV,0x00,VMM_PAGE_SIZE_1M);
            
            vmm_UnmapUserSpace((void*)tempV, VMM_PAGE_SIZE_1M, 0);
        }
        
        // initialize the control information associated with the allocated persistent memory
        virtFS* persistFS = File_GetFSPtrFromType(FD_PERSIST_MEM);
        persistFS->clearAllocations();
    }
    //printf("Shared mem: %d Persist mem: %d\n", sharedmem, persistmem);
#endif                                                                                                                 // FUSEDOS
    if (sharedmem || persistmem)
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        if (sharedmem+persistmem > pPers->DDR_Config.DDRSizeMB)
        {
            return(-1);
        }
#endif                                                                                                                 // FUSEDOS
        // Set the size of the shared memory block
        NodeState.SharedMemory.Size = (uint64_t)sharedmem * VMM_PAGE_SIZE_1M;
        
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        // Are we preserving a persistent memory allocation from a previous job?
        if (NodeState.PersistentMemory.VStart)
        {
            uint32_t vstart = NodeState.PersistentMemory.VStart/VMM_PAGE_SIZE_1M;
            uint32_t pstart = NodeState.PersistentMemory.PStart/VMM_PAGE_SIZE_1M;
            rc = vmm_addSegment(IS_SHAR, vstart, pstart, sharedmem+persistmem, -1, VMM_SEGMENTCOREMASK_ALL, true, true, true,  true, ((numProcs==64)?true:false), false);
        }
        else
        {
            // Set the size of the persistent memory block
            NodeState.PersistentMemory.Size = (uint64_t)persistmem * VMM_PAGE_SIZE_1M;
            // Add the segment
            rc = vmm_addSegment(IS_SHAR, 777, 0, sharedmem+persistmem, -1, VMM_SEGMENTCOREMASK_ALL, true, true, false, false, ((numProcs==64)?true:false), false);
        }
#else                                                                                                                  // FUSEDOS
		// no persistent memory in FusedOS (yet?!), but we want shared                                                 // FUSEDOS
		// memory shared between all cl instances                                                                      // FUSEDOS
		if(NodeState.SharedMemory.PStart) {                                                                            // FUSEDOS
			rc = vmm_addSegment(IS_SHAR, 777, NodeState.SharedMemory.PStart/VMM_PAGE_SIZE_1M,                          // FUSEDOS
					NodeState.SharedMemory.Size, -1, VMM_SEGMENTCOREMASK_ALL, true, true, true,  true, true, false);   // FUSEDOS
		} else {                                                                                                       // FUSEDOS
            rc = vmm_addSegment(IS_SHAR, 777, 0, sharedmem, -1, VMM_SEGMENTCOREMASK_ALL, true, true, false, false, true, false); // FUSEDOS
		}                                                                                                              // FUSEDOS
#endif                                                                                                                 // FUSEDOS
		CL_DEBUG("rc = %d\n", rc);
        if(rc)
            return rc;
    }
//#endif                                                                                                                 // FUSEDOS

    TRACE( TRACE_Process, ("(I) App_SetupMemory%s generating static TLB map\n", whoami()) );
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Lock the Shared work area. The partitionedMapper and copySegments uses this large data area as temporary work space.
    // Information generated in the partitionedMapper is consumed by copySegments therefore we lock the area across both of
    // these calls. In a sub-node job environment, multiple static mappers may be running simultaneously, therefore the lock
    // is required.
    Kernel_Lock(&NodeState.sharedDataAreaLock);
#endif                                                                                                                 // FUSEDOS

    if (vmm_partitionedMapper(NULL) != 0)
    {
        // An mapping error occurred. It is likely that user specifications have exceeded available physical memory
        TRACE( TRACE_Process, ("(E) App_SetupMemory%s failed to generate a static TLB map\n", whoami()) );
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        // Unlock the shared work area
        Kernel_Unlock(&NodeState.sharedDataAreaLock);
#endif                                                                                                                 // FUSEDOS
		CL_DEBUG("vmm_partitionedMapper(NULL) != 0");
        return(-1);
    }

    TRACE( TRACE_Process, ("(I) App_SetupMemory%s copying segments in physical memory\n", whoami()) );
    rc = vmm_copySegments();
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Unlock the shared work area
    Kernel_Unlock(&NodeState.sharedDataAreaLock);
#endif                                                                                                                 // FUSEDOS

		CL_DEBUG("rc = %d\n", rc);
    if(rc)
        return rc;

    return( 0 );
}

int Process_SetupMemory()
{
    uint64_t dma_lower, dma_upper;
    dma_lower = 0xffffffffffffffffLL;
    dma_upper = 0;
    
    AppState_t *pAppState  = GetMyAppState();
    AppProcess_t* pAppProc = GetProcessByProcessorID(ProcessorID());
    if(pAppProc)
    {
        uint64_t vaddr, paddr;
        size_t vsize;
        
        vmm_getSegment(pAppProc->Tcoord, IS_TEXT, &vaddr, &paddr, &vsize);
        pAppProc->Text_VStart = vaddr;
        pAppProc->Text_VEnd   = vaddr + ((vsize>0)?vsize-1:0);
        pAppProc->Text_PStart = paddr;
        pAppProc->Text_PEnd   = paddr + ((vsize>0)?vsize-1:0);
        if(vsize != 0)
        {
            dma_lower = MIN(dma_lower, pAppProc->Text_PStart);
            dma_upper = MAX(dma_upper, pAppProc->Text_PEnd);
        }
        TRACE( TRACE_Process, ("(I) Process_SetupMemory%s text   vstart=0x%016lx vend=0x%016lx pstart=0x%016lx pend=0x%016lx\n",
                               whoami(), pAppProc->Text_VStart, pAppProc->Text_VEnd, pAppProc->Text_PStart, pAppProc->Text_PEnd) );

        vmm_getSegment(pAppProc->Tcoord, IS_DATA, &vaddr, &paddr, &vsize);
        pAppProc->Data_VStart = vaddr;
        pAppProc->Data_VEnd   = vaddr + ((vsize>0)?vsize-1:0);
        pAppProc->Data_PStart = paddr;
        pAppProc->Data_PEnd   = paddr + ((vsize>0)?vsize-1:0);
        if(vsize != 0)
        {
            dma_lower = MIN(dma_lower, pAppProc->Data_PStart);
            dma_upper = MAX(dma_upper, pAppProc->Data_PEnd);
        }
        TRACE( TRACE_Process, ("(I) Process_SetupMemory%s data   vstart=0x%016lx vend=0x%016lx pstart=0x%016lx pend=0x%016lx\n",
                               whoami(), pAppProc->Data_VStart, pAppProc->Data_VEnd, pAppProc->Data_PStart, pAppProc->Data_PEnd) );
        
        pAppProc->interpreterBase = 0;
        if(pAppState->invokeWithInterpreter)
        {
            vmm_getSegment(pAppProc->Tcoord, IS_DYNAM, &vaddr, &paddr, &vsize);
            pAppProc->interpreterBase = vaddr;
            pAppProc->DYN_VStart = vaddr;
            pAppProc->DYN_VEnd   = vaddr + ((vsize>0)?vsize-1:0);
            pAppProc->DYN_PStart = paddr;
            pAppProc->DYN_PEnd   = paddr + ((vsize>0)?vsize-1:0);
            TRACE( TRACE_Process, ("(I) Process_SetupMemory%s dyn    vstart=0x%016lx vend=0x%016lx pstart=0x%016lx pend=0x%016lx\n",
                                   whoami(), pAppProc->DYN_VStart, pAppProc->DYN_VEnd, pAppProc->DYN_PStart, pAppProc->DYN_PEnd) );
        }
        pAppProc->Proc_EntryPoint = pAppState->App_EntryPoint + pAppProc->interpreterBase;
        vmm_getSegment(pAppProc->Tcoord, IS_SHAR, &vaddr, &paddr, &vsize);
        
        pAppProc->Shared_VStart = vaddr;
        pAppProc->Shared_VEnd   = vaddr + ((vsize>0)?vsize-1:0);
        pAppProc->Shared_PStart = paddr;
        pAppProc->Shared_PEnd   = paddr + ((vsize>0)?vsize-1:0);
        if(vsize != 0)
        {
            dma_lower = MIN(dma_lower, pAppProc->Shared_PStart);
            dma_upper = MAX(dma_upper, pAppProc->Shared_PEnd);
        }
        TRACE( TRACE_Process, ("(I) Process_SetupMemory%s shared vstart=0x%016lx vend=0x%016lx pstart=0x%016lx pend=0x%016lx\n",
                               whoami(), pAppProc->Shared_VStart, pAppProc->Shared_VEnd, pAppProc->Shared_PStart, pAppProc->Shared_PEnd) );

        if (IsAppLeader())
        {
            // first setup the persistent memory info
            ShmMgr_t *persistMgr = &(NodeState.PersistentMemory);
            // Is there a persistent memory allocation in the current job
            if (persistMgr->Size)
            {
                // Persistent memory starts at the beginning of the IS_SHAR segment
                persistMgr->VStart = vaddr;
                persistMgr->PStart = paddr;
            }
            
            // Now setup the shared memory region
            ShmMgr_t *sharedMgr = &(NodeState.SharedMemory);
            // Is there a shared memory allocation in the current job
            if (sharedMgr->Size)
            {
                // Skip persistent memory piece of shared memory
                sharedMgr->VStart   = vaddr + persistMgr->Size;
                sharedMgr->PStart   = paddr + persistMgr->Size;
                sharedMgr->Size     = vsize - persistMgr->Size;
            }
        }
        vmm_getSegment(pAppProc->Tcoord, IS_HEAP, &vaddr, &paddr, &vsize);
        
        uint64_t localfssize = 0;
        App_GetEnvValue64("BG_LOCALFSSIZE", &localfssize);
        if((vsize > (localfssize + 1)*1024*1024) && (localfssize > 0))
        {
            pAppProc->LocalDisk.VStart = (uint64_t)vaddr;
            pAppProc->LocalDisk.Size   = localfssize * 1024*1024;
            
            vsize -= localfssize * 1024*1024;
            vaddr += localfssize * 1024*1024;
        }
        else
        {
            pAppProc->LocalDisk.VStart = 0;
            pAppProc->LocalDisk.Size   = 0;
        }
        pAppProc->Heap_Break = pAppProc->Heap_VStart = pAppProc->Heap_Start = vaddr;
        pAppProc->Heap_VEnd = pAppProc->Heap_End  = vaddr + ((vsize>0)?vsize-1:0);
        pAppProc->Heap_PStart = paddr;
        pAppProc->Heap_PEnd   = paddr + ((vsize>0)?vsize-1:0);

        if(vsize != 0)
        {
            dma_lower = MIN(dma_lower, pAppProc->Heap_PStart);
            dma_upper = MAX(dma_upper, pAppProc->Heap_PEnd);
        }
        TRACE( TRACE_Process, ("(I) Process_SetupMemory%s heap   vstart=0x%016lx vend=0x%016lx pstart=0x%016lx pend=0x%016lx\n",
                               whoami(), pAppProc->Heap_VStart, pAppProc->Heap_VEnd, pAppProc->Heap_PStart, pAppProc->Heap_PEnd) );
        
        vmm_getPID(pAppProc->Tcoord, &(pAppProc->PhysicalPID));
        TRACE( TRACE_Process, ("(I) Process_SetupMemory%s PhysicalPID=%lu\n", whoami(), pAppProc->PhysicalPID) );

        // TLS virtual
        pAppProc->TLS_VStart  = pAppState->TLS_VStart;
        pAppProc->TLS_VEnd    = pAppState->TLS_VEnd;
        
        // record the TData and TBSS start and size for use in thread creation
        pAppProc->pTData_Start = pAppState->pTData_Start;
        pAppProc->TData_Size   = pAppState->TData_Size;
        pAppProc->pTBSS_Start  = pAppState->pTBSS_Start;
        pAppProc->TBSS_Size    = pAppState->TBSS_Size;
        
        pAppProc->MMap_Start  = 0;
        pAppProc->MMap_End    = 0;

        pAppProc->DMA_Lower = dma_lower;
        pAppProc->DMA_Upper = dma_upper;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
	if(!Personality_IsFPGA() && (pAppProc->Tcoord >= 64)) // Is this a AppAgent? (the fpga does not support the MU hardware)
	{
            //printf("(I) MU DMA range %d:  %lx -> %lx\n", pAppProc->PID, dma_lower, dma_upper);
            // Allow the app agent address range for DMA. We reserve the last 2 entries within the 20 entry range for app agents
            // (the remaining process address ranges, up to 64, will later be compressed into the remaining 18 range pairs)
            DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE) + pAppProc->Tcoord-46, dma_lower);
            DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE) + pAppProc->Tcoord-46, dma_upper);
	}
#endif                                                                                                                 // FUSEDOS
    }
    return 0;
}

/*********
          IU Livelock Buster configuration
          Every 'n' cycles, the livelock buster stalls a pseudorandomly selected hardware thread on the core for up to 'm' cycles.  
          There are no known scenarios that require this avoidance circuitry to be active.  However, the A2 team still recommends a 
          period of 8 (~135,000 cycles) and a delay of 32 (32*16 = 512 cycles).  The ftq/fwq benchmarks highlight this jitter and for CNK, and therefore
          we are defaulting the IU livelock buster to disabled.  The environment variables can reactivate it, if needed.  
          
          Note:  This is NOT the same as the XU "livelock" buster (more accurately called a deadlock buster).  The XU buster detects if a 
          given instruction has not completed in 'n' cycles (e.g., if the hwthread kept being victimized by pipeline flushes in other threads).
          Once it detects this situation, all other threads are suspended until the hardware thread completes the instruction.  
          
          The XU buster is always enabled.
 ********/
void App_SetIULivelockConfig()
{
    uint64_t period = 0x8;
    uint64_t delay  = 0;
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    if (getenv("BG_IULLAVOIDPERIOD")) period = atol(getenv("BG_IULLAVOIDPERIOD"));                                     // FUSEDOS
    if (getenv("BG_IULLAVOIDDELAY")) delay = atol(getenv("BG_IULLAVOIDDELAY"));                                        // FUSEDOS
    spc_context_t* pc = get_spc_context(this_spc());                                                                   // FUSEDOS
    pc->BG_IULLAVOIDPERIOD = period;                                                                                   // FUSEDOS
    pc->BG_IULLAVOIDDELAY = delay;                                                                                     // FUSEDOS
#else                                                                                                                  // FUSEDOS
    App_GetEnvValue64("BG_IULLAVOIDPERIOD", &period);
    App_GetEnvValue64("BG_IULLAVOIDDELAY",  &delay);
    
    uint64_t iullcr = mfspr( SPRN_IULLCR );
    iullcr &= ~IULLCR_IULL_EN;
    iullcr &= ~IULLCR_LL_HOLD_DLY(-1);
    iullcr &= ~IULLCR_LL_TRIG_DLY(-1);
    if((period != 0) && (delay != 0))
        iullcr |= (IULLCR_IULL_EN | IULLCR_LL_HOLD_DLY(delay) | IULLCR_LL_TRIG_DLY(period));
    mtspr( SPRN_IULLCR, iullcr );
    isync();
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
void App_SetDmaRanges()
{
    AppState_t &appState = *GetMyAppState();
    // If the number of processes is 16 or less, then our job is easy. Just use each processes PID to set its range
    if (appState.Active_Processes <= 16) // PIDs range from 0 to 15. We can use the PID as the index into the register pair
    {
        for (int i=0; i<64; i++)
        {
            if (_BN(i) &  appState.ProcessLeaderMask)
            {
                AppProcess_t &proc = NodeState.AppProcess[i];
                if(!Personality_IsFPGA())
                {
                    //printf("(I) MU DMA range:%lx - %lx\n", proc.DMA_Lower,proc.DMA_Upper );
                    DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE) + proc.Tcoord, proc.DMA_Lower);
                    DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE) + proc.Tcoord, proc.DMA_Upper);
                }
            }
        }
    }
    else
    {
        // For now just determine the lowest and highest physical addresses allocated to all the processes and use that
        // as the range. In the future we could (should) be smarter about this and look for adjacent or overlapping ranges 
        // to generate a more specific set of ranges and then only revert to this more general range algorithm if we could 
        // not contain all the processes within the 18 remaining MU dma range registers (2 of the 20 ranges are potentially 
        // consumed by AppAgents).
        uint64_t dma_lower, dma_upper;
        dma_lower = 0xffffffffffffffffLL;
        dma_upper = 0;
        for (int i=0; i<64; i++)
        {
            if (_BN(i) &  appState.ProcessLeaderMask)
            {
                AppProcess_t &proc = NodeState.AppProcess[i];
                if (proc.DMA_Lower < dma_lower)
                {
                    dma_lower = proc.DMA_Lower;
                }
                if (proc.DMA_Upper > dma_upper)
                {
                    dma_upper = proc.DMA_Upper;
                }
            }
        }
        if(!Personality_IsFPGA())
        {
            //printf("(I) MU DMA range:%lx - %lx\n", dma_lower,dma_upper );
            DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE), dma_lower);
            DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE), dma_upper);
        }
    }
}

int ProcFS_ProcessInit(AppProcess_t* proc)
{
    int fd;
    char fn[APP_MAX_PATHNAME];
    char* ptr;
    snprintf(fn, sizeof(fn), "/proc/%d/cmdline", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    ptr = &proc->app->App_Args[0];
    while(*ptr)
    {
        internal_write(fd, ptr, strlen(ptr));
        ptr += strlen(ptr)+1;
        if(*ptr != 0)
            internal_write(fd, " ", 1);
    }
    internal_close(fd);
    
    snprintf(fn, sizeof(fn), "/proc/%d/environ", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    ptr = &proc->app->App_Env[0];
    while(*ptr)
    {
        internal_write(fd, ptr, strlen(ptr));
        ptr += strlen(ptr)+1;
        if(*ptr != 0)
            internal_write(fd, " ", 1);
    }
    internal_close(fd);
    
    // Create empty placeholder files for maps, exe, cwd.  
    snprintf(fn, sizeof(fn), "/proc/%d/maps", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    internal_close(fd);
    
    snprintf(fn, sizeof(fn), "/proc/%d/exe", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    internal_close(fd);
    
    snprintf(fn, sizeof(fn), "/proc/%d/cwd", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    internal_close(fd);

    return 0;
}

extern Lock_Atomic_t ShareLock;
extern Lock_Atomic_t mappedFilesLock;
extern ShmMgrEntry_t SharedPool[ SHM_MAX_OPENS ];

int ProcFS_GenMaps(AppProcess_t* proc)
{
    int fd;
    char fn[APP_MAX_PATHNAME];
    
    snprintf(fn, sizeof(fn), "/proc/%d/maps", proc->PID);
    fd = internal_open(fn, O_CREAT | O_TRUNC | O_WRONLY | O_LARGEFILE, 0666);
    char line[256];
    off64_t  lineoffset;
    bool     mappedheap;
    uint64_t heapl;
    uint64_t heaph = proc->Heap_VEnd+1;
    uint64_t bestheapl = proc->Heap_VStart;
    uint64_t heapslice = 0;
#define MKMAPSENTRY(vstart, vend, perms, offset, filename, indexadjust)  index+=indexadjust; if((vstart >= rejectaddr) && (minaddr > vstart) && ((vstart > rejectaddr) || (minindex < index))) { minaddr = vstart; didwork = true; pickindex = index; lineoffset=0; if((vstart > bestheapl) && (vend <= heaph)) { lineoffset = snprintf(line, sizeof(line), "%lx-%lx rwxp %08lx 00:00 0 [heap%ld]\n", bestheapl, vstart, (uint64_t)0, heapslice); heapl=vend; mappedheap=true; } snprintf(&line[lineoffset], sizeof(line)-lineoffset, "%lx-%lx %s %08lx 00:00 0 %s\n", vstart, vend, perms, (uint64_t)offset, filename); }

#define MKMAPSENTRY2(vstart, vend, perms, offset, filename, indexadjust) index+=indexadjust; if((vend >= rejectaddr) && (minaddr > vend) && ((vend > rejectaddr) || (minindex < index))) { minaddr = vend; didwork = true; pickindex = index; lineoffset=0; snprintf(&line[lineoffset], sizeof(line)-lineoffset, "%lx-%lx %s %08lx 00:00 0 %s\n", vstart, vend, perms, (uint64_t)offset, filename); }
    
    // This code is a bit unnecessary, but there seems to be an implicit requirement 
    // to produce a sorted /proc/self/maps file.
    uint64_t rejectaddr = 0;
    uint64_t minaddr    = (uint64_t)&__KERNEL_END;  // ignore addresses in kernel space
    uint64_t pickindex  = 0;
    uint64_t minindex   = 0;
    bool     didwork;
    uint64_t index;
    do
    {
        index = 0;
        rejectaddr = minaddr;
        minaddr  = ~0;
        minindex = pickindex;
        didwork      = false;
        heapl = bestheapl;
        mappedheap = false;
        
        MKMAPSENTRY((uint64_t)VA_MINADDR_MMIO,      (uint64_t)VA_MINADDR_MMIO + 1024*1024*1024, "rw-s", 0, "[mmio]", 1);
        MKMAPSENTRY((uint64_t)CONFIG_FAST_L1P_BASE, (uint64_t)CONFIG_FAST_L1P_BASE + 1024*1024, "rw-s", 0, "[l1p]", 1);
        MKMAPSENTRY((uint64_t)CONFIG_FAST_L2_BASE,  (uint64_t)CONFIG_FAST_L2_BASE  + 1024*1024, "rw-s", 0, "[l2]", 1);
        
        if(proc->app->invokeWithInterpreter)
        {
            MKMAPSENTRY(proc->DYN_VStart,  proc->DYN_VEnd+1,  "rwxp", 0, proc->app->InterpreterPathAndName, 1);
        }
        else
        {
            MKMAPSENTRY(proc->Text_VStart, proc->Text_VEnd+1, "r-xs", 0, proc->app->App_Args, 1);   // text segment
            MKMAPSENTRY(proc->Data_VStart, proc->Data_VEnd+1, "rw-p", 0, proc->app->App_Args, 1);   // data segment
        }
        MKMAPSENTRY2(bestheapl, proc->Heap_VEnd+1, "rwxp", 0, "[heap_and_stack]", 1);
        
        int i;
        char name[64];
        uint64_t l2atomicindex = 0;
        uint64_t processwinindex = 0;
        for(i=0; i<512; i++)
        {
            index++;
            uint64_t vaddr, paddr, size;
            enum SegmentType type;
            int rc = vmm_getSegmentEntryBySlot(proc->Tcoord, i, &type, &vaddr, &paddr, &size);
            if((rc == 0) && (type == IS_ATOMICRGN))
            {
                snprintf(name, sizeof(name), "[l2atomic%ld]", l2atomicindex++);
                MKMAPSENTRY(vaddr, vaddr+size, "rw-p", 0, name, 0);
            }
            else if((rc == 0) && (type == IS_PROCESSWINDOW))
            {
                snprintf(name, sizeof(name), "[process_window%ld]", processwinindex++);
                MKMAPSENTRY(vaddr, vaddr+size, "r--s", 0, name, 0);
            }
        }
        
        // \todo this could be cleaned up.  AppSetup.cc shouldn't need to know the internal structures of shmFS
        Kernel_Lock(&ShareLock);
        for(i=0; i<SHM_MAX_OPENS; i++)
        {
            uint64_t vaddr, size;
            index++;
            if(SharedPool[i].AllocatedAddr != 0)
            {
                vaddr = SharedPool[i].AllocatedAddr;
                size  = SharedPool[i].AllocatedSize;
                strncpy(fn, SharedPool[i].FileName, sizeof(fn));
                Kernel_Unlock(&ShareLock);
                MKMAPSENTRY(vaddr, vaddr+size, "rw-s", 0, fn, 0);
                Kernel_Lock(&ShareLock);
            }
        }
        Kernel_Unlock(&ShareLock);
        
        Kernel_Lock(&mappedFilesLock);
        for(i=0; i<CONFIG_NUM_MAPPED_FILENAMES; i++)
        {
            index++;
            if(proc->mappedFiles[i].vaddr != 0)
            {
                MKMAPSENTRY(proc->mappedFiles[i].vaddr, proc->mappedFiles[i].vaddr + proc->mappedFiles[i].size, ((proc->mappedFiles[i].isShar)?"rwxs":"rwxp"), proc->mappedFiles[i].off, proc->mappedFiles[i].filename, 0);
            }
        }
        Kernel_Unlock(&mappedFilesLock);
        
        bestheapl = heapl;
        if(mappedheap)
        {
            heapslice++;
        }
        if(didwork)
        {
            internal_write(fd, line, strlen(line));
        }
    }
    while(didwork);
    
    internal_close(fd);
    return 0;
}


//
// App_Load():
//   This is the target of the ipi load application request and runs on all the hwthreads of the App
//   
//
void App_Load()
{

    int rc = 0;
    Personality_t *pPers = &NodeState.Personality;
    AppState_t *pAppState = GetMyAppState();
    AppProcess_t *pProc = GetMyProcess();
    uint64_t agent_poffset_adjust = 0;
    
    assert(pAppState && pProc);
    
    l1p_init();
    
    // Initialize job-scoped resources.
    if ( IsAppLeader() )
    {
        // Initialize speculation
        Speculation_Init();
        
        // Set the state of the Application to indicate that a Load is in progress
        App_SetLoadState(AppState_Loading, 0, 0);
        
        // Setup the path to the appropriate static mapper objects for this job and initialize it. 
        uint32_t numProcs = pAppState->Active_Processes;
        int      numAgents = popcnt64(NodeState.AppAgents);
        
        // set default allowing mapping processes aligned to 16MB boundaries (as opposed to 16GB)
        uint32_t align16 = 0;
        if(numProcs > 16)      // set default for ppn=32, ppn=64.  
            align16 = 1;
        App_GetEnvValue("BG_MAPALIGN16", &align16);
        vmm_setFlags(MAPPERFLAGS_ALIGN16, align16);
        
        uint32_t noaliases = 0;
        App_GetEnvValue("BG_MAPNOALIASES", &noaliases);  // \todo find better name for option
        vmm_setFlags(MAPPERFLAGS_NOALIASES, noaliases);
        
        uint32_t commonheap = 0;
        App_GetEnvValue("BG_MAPCOMMONHEAP", &commonheap);  // \todo find better name for option
        vmm_setFlags(MAPPERFLAGS_COMMONHEAP, commonheap);
        
        vmm_resetStaticMap(numProcs, numAgents, ((IsSubNodeJob()) ? ProcessorCoreID() : -1));

        // Initialize file systems.
        if (File_JobSetup() != 0)
        {
            TRACE( TRACE_Jobctl, ("(E) Bad return code from File_JobSetup()\n") );
        }
        GetMyAppState()->LoadSequence = (~0);
    }
    
    // barrier necessary to ensure file system is setup first.  
    Kernel_Barrier(Barrier_HwthreadsInApp);
    
    if ( IsProcessLeader() )
    {
        // Initialize process-scoped file system resources.
        if (File_ProcessSetup() != 0)
        {
            TRACE( TRACE_Jobctl, ("(E) Bad return code from File_ProcessSetup()\n") );
        }

        // Find the rank of this process.
        rc = getMyRank(&pProc->Rank);
        if(rc)
            App_SetLoadState(AppState_LoadFailed, bgcios::AppInvalidorMissingMapfile, 0);
        
        // Determine if this process is located in an active rank
        if (pProc->Rank >= pAppState->ranksActive )
        {
            // This process is Rank Inactive
            pProc->State = ProcessState_RankInactive;
        }
    }
    // Test to see if an application load error has been detected. 
    App_TestLoadProgress(pAppState, bgcios::AppInvalidorMissingMapfile, 0); // This call will not return if an application load error was detected
    
    // We will setup memory in the AppLeader thread other threads wait.     
    if ( IsAppLeader() )
    {
        int rc = 0;

        // Determine if we are going to proceed with a collective job load or is each App Leader going to read the elf
        if(pAppState->PreloadedApplication == 0) 
        {
            uint32_t enableCollectiveLoad = CNK_COLLECTIVE_APPLOAD;
            GetMyAppState()->MaxSequence = 0;
            
            // If the job contains more than 1 node, it is a candidate for collective loading
            if (pAppState->jobLeaderData.NodesInJob > 1)
            {
                App_GetEnvValue("BG_COLLECTIVELOAD", &enableCollectiveLoad);
                // Test to see if collective loading is enabled
                if (enableCollectiveLoad)
                {
                    // Actviate collective loading
                    App_ActivateCollectiveLoad();
                    //if (App_IsJobLeaderNode())
                    //{
                    //    printf("Collective job load activated.\n");
                    //}
                }
            }
            uint32_t sequence;
            for(sequence=0; sequence <= GetMyAppState()->MaxSequence; sequence++)
            {
                Kernel_WriteFlightLog(FLIGHTLOG, FL_MPMDSEQST, sequence, GetMyAppState()->LoadSequence, GetMyAppState()->MaxSequence,0);
                if(enableCollectiveLoad)
                {
                    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                    
                    if(sequence == GetMyAppState()->LoadSequence)
                        configureLoadJobClassroutes(1);
                    else
                        configureLoadJobClassroutes(0);
                    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                    if(sequence == GetMyAppState()->LoadSequence)
                    {
                            App_SetupLoadLeader();
                    }
                    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                }
                
                if((sequence == GetMyAppState()->LoadSequence) || (enableCollectiveLoad == 0))
                {
                    uint64_t physical_offset = pAppState->corner.core * ((NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin)/ CONFIG_MAX_APP_CORES);
                    rc = elf_loadimage(pAppState, physical_offset, &agent_poffset_adjust);
                    if (rc != 0)
                    {
                        TRACE( TRACE_Jobctl, ("(E) Bad return code from elf_loadimage.  rc=%d\n", rc) );
                    }
                }
                if(enableCollectiveLoad)
                {
                    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                    rc = App_CheckGloballyForLoadFailure();
                    if (rc)
                    {
                        if(GetMyAppState()->LoadState != AppState_LoadFailed)
                        {
                            App_SetLoadState(AppState_LoadFailed, bgcios::AppInvalidorMissingMapfile, 0);
                        }
                        break;
                    }
                }
            }
            if(enableCollectiveLoad)
            {
                configureLoadJobClassroutes(1);
                MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
                App_SetupLoadLeader();
                MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
            }
        }
        if (rc == 0)
        {
            // Load images for any application agents that are configured for this application
            int agentAppStateIndex = CONFIG_FIRST_AGENT_APPSTATE_INDEX;
            for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++,agentAppStateIndex++)
            {
                if (NodeState.AppAgents & _BN(i))
                {
                    AppState_t *agentAppState = &NodeState.AppState[agentAppStateIndex];
                    uint64_t temp_poffset_adjust = agent_poffset_adjust;
                    rc = elf_loadimage(agentAppState, agent_poffset_adjust, &temp_poffset_adjust);
                    if (rc != 0)
                    {
                        printf("(E) Bad return code from elf_loadimage for agent.  rc=%d\n", rc);
                        break;
                    }

                    // Dynamically linked app agents are not supported.
                    if (agentAppState->invokeWithInterpreter) {
                       printf("(E) Dynamically linked app agent is not supported.\n");
                       rc = ENOTSUP;
                       App_SetLoadState(AppState_LoadFailed, bgcios::RequestFailed, rc);
                    }
                }
            }
        }
        if (rc == 0)
        {
            // Assign Memory to Processes (not yet TLB mapped, each core does that below)
            // The application size and/or shared/persistent memory size specifications may lead to a condition
            // in which not enough physical memory is available to satisfy the application requirements. The call to
            // App_SetupMemory will return 0 if the setup was successful
            rc = App_SetupMemory(pPers);
            if (rc != 0)
            {
                printf("(E) Bad return code from App_SetupMemory: %d\n", rc);

                // Indicate that we encountered an error during the creation of the Static TLB Map
                App_SetLoadState(AppState_LoadFailed, bgcios::AppStaticTLBError, 0);
            }
        }
    }
    // Test to see if an application load error has been detected. 
    App_TestLoadProgress(pAppState, bgcios::RequestFailed, 0); // This call will not return if an application load error was detected

    // If there are Application Agents to set up, start them now. Their Static TLB mappings are now completed
    if ( IsAppLeader() )
    {
        // Are there AppAgents configured?
        if (NodeState.AppAgents)
        {
            // Wake up hardware threads of any configured application agents
            for (int i=0; i<CONFIG_MAX_APP_AGENTS; i++)
            {
                if (NodeState.AppAgents & _BN(i))
                {
                    IPI_load_agent(NodeState.FirstAppAgentProcessorID + i);
                }
            }
        }
        else
        {
            uint64_t jitterBugValue = 0; // default parameter value 
            if (App_GetEnvValue64("BG_JITTERBUG", &(jitterBugValue))) // override if env variable was specified
            {
                // If jitterbug env was supplied, send IPI to fire up the test tool
                IPI_start_jitterbug(jitterBugValue);
            }
        }
    }

    if(IsProcessLeader())
    {
        rc = Process_SetupMemory();        
        if (rc != 0)
        {
            printf("(E) Bad return code from Process_SetupMemory: %d\n", rc);

            // Indicate that we encountered an error during SetupMemory
            App_SetLoadState(AppState_LoadFailed, bgcios::AppMemoryError, rc);

        }
        else
        {
            uint32_t mainStackSize = CNA_MAIN_THREAD_STACK_RESERVED;
            App_GetEnvValue("BG_MAINSTACKSIZE", &mainStackSize);
            pProc->MainStackReserve = mainStackSize;
            
            // set Guard Enablement
            uint32_t requestedEnablement = CNK_STACK_GUARD_ENABLE_DEFAULT; // default enablement for stack guarding 
            if (App_GetEnvValue("BG_STACKGUARDENABLE", &(requestedEnablement))) // override if env variable was specified
            {
                // Check supplied value of env variable for validity. 
                if ((requestedEnablement > Bg_StackGuardEnable_HighValue) || (requestedEnablement < Bg_StackGuardEnable_LowValue))
                    // unsupported value was supplied. Set guarding to default behavior as defined in cnk/include/Configure.h
                    requestedEnablement = CNK_STACK_GUARD_ENABLE_DEFAULT;
            }
            pProc->Guard_Enable = requestedEnablement; // set the enablement in the process structure
            if (pProc->Guard_Enable)
            {
                if (!(App_GetEnvValue("BG_STACKGUARDSIZE", &(pProc->Guard_Size))))
                    pProc->Guard_Size = CNK_MAIN_THREAD_GUARD_SIZE; // default stack guard size
                else
                {
                    if (pProc->Guard_Size)
                    {
                        if (pProc->Guard_Size < CNK_MIN_GUARD_SIZE)
                            pProc->Guard_Size = CNK_MIN_GUARD_SIZE;
                        else
                        {
                            // Test the size of the user-supplied guard. It must be a power of 2 
                            uint64_t guard_boundary = popcnt64(pProc->Guard_Size);
                            if (guard_boundary > 1)
                            {
                                // we need to do some adjusting. Round down to a power of 2
                                uint64_t leadingZeros = cntlz64(pProc->Guard_Size);
                                pProc->Guard_Size = _BN(leadingZeros);
                            }
                        }
                    }
                    else
                    {
                        pProc->Guard_Enable = Bg_StackGuard_Disabled;
                    }
                }
            }
            if (pProc->Guard_Enable)
            {
//                printf("Stack Guarding is Enabled \n \n");
                 // Initialize to heap break. Will change after MMaps occur
                pProc->Guard_HighMark = pProc->Heap_Break;
                // The guard start must  begin on an address having the same boundary as the guard size.
                // Adjust the start of the guard.
                uint64_t guardMask = pProc->Guard_Size-1;

                pProc->Guard_Start = (pProc->Guard_HighMark + guardMask) & ~guardMask;

            }
        
            pProc->binaryCoredump = coredump_binary_for_rank(pProc);
            pProc->coreDumpRank = coredump_for_rank(pProc);

	    // per-thread limit on number of alignment exceptions that will be
	    // handled before signals are generated
	    uint32_t maxcnt;
	    if (!App_GetEnvValue("BG_MAXALIGNEXP", &maxcnt))
	    {
		// Environment variable not set.  Choose a default value.
		maxcnt = 1000;
	    }
	    pProc->AlignmentExceptionMax = maxcnt;

	    // Get/SetDebugReg syscalls are disabled by default.
	    pProc->DebugRegSyscallsEnabled = 0;
	    uint32_t dbgregs;
	    if (App_GetEnvValue("BG_DEBUGREGSYSCALLSENABLED", &dbgregs) &&
		(dbgregs != 0))
	    {
		pProc->DebugRegSyscallsEnabled = 1;
	    }
        }
	Signal_ProcessInit(pProc->SignalData);
        memset(pProc->resourceLimits, 0xff, sizeof(pProc->resourceLimits));
    }
    // Test to see if an application load error has been detected. 
    App_TestLoadProgress(pAppState, bgcios::RequestFailed, 0); // This call will not return if an application load error was detected

    if(pProc)
    {
        if (ProcessorThreadID() == 0) // TLBs only need to be loaded once on each core
        {
            vmm_installStaticTLBMap(pProc->Tcoord);
            App_SetIULivelockConfig();
        }
        GetMyKThread()->physical_pid = pProc->PhysicalPID; // Initialize the process pid value into the kernel kthread.
        mtspr(SPRN_PID, pProc->PhysicalPID);
        isync();

        // Cache the status of guarding in the hwthread state object for use by the scheduler
        GetMyHWThreadState()->launchContextFlags.flag.GuardActive = pProc->Guard_Enable;

    }

    if (IsAppLeader())
    {
        // Walk through all processes.  If this node is not participating in the job, reactivate the GI user classroute0 contribution for the node
        uint32_t active = 0;
        AppProcess_t* p = pProc;
        while(p != NULL)
        {
            if(p->State != ProcessState_RankInactive)
                active++;
            p = p->nextProcess;
        }
        if(active == 0)
        {
            deconfigureUserGI();
        }
        // Set the allowable physical address dma ranges for all processes within the application
        App_SetDmaRanges();

        PowerManagement_Init();

        // Initialize tool control if CIOS is enabled.
        if (Personality_CiosEnabled())
        {
            if (toolControl.setupJob() != 0)
            {
                TRACE( TRACE_Jobctl, ("(E) Bad return code from tool control setupJob()\n") );
            }
        }

        // If this is not a subnode job, initialize the breakpoint table used by tools. This shares a large data area used by the 
        // static mapper therefore we must perform this after the mapper has run and after the process memmory is setup.
        if (!IsSubNodeJob())
        {
            toolControl.breakpointController().trapTableAllocate();
        }
    }
    Kernel_Barrier(Barrier_HwthreadsInApp);
    if (IsProcessLeader())
    {
	App_InitKernelInfo(pProc, pAppState);
    }

    // Test to see if an application load error has been detected. 
    App_TestLoadProgress(pAppState, bgcios::RequestFailed, 0); // This call will not return if an application load error was detected
    
    // Setup the SPR used by the kernel to implement user-state kernel SPIs. We are setting this value directly
    // into the hardware SPR register. The low-core register state restore flows will not write to the hardware SPRs.
    // This value will be maintained/updated directly in the hardware SPRs since the information is not kthread scoped.
        // setup sprg containing info used by user-state SPIs
    uint64_t spiInfo = (((uint64_t)(pAppState->AppStateIndex)) << SPRG_SPIinfo_AppLeaderCore) | // byte3 bits 0:3 App Leader core index.
                       (((uint64_t)(pAppState->Active_Cores-1)) << SPRG_SPIinfo_NumCores) |     // byte3 bits 4:7 Number of cores in App - 1
                       (pAppState->Active_Processes << SPRG_SPIinfo_NumProcs) |                 // byte6: number of processes  
                       (ProcessorID());                                                         // byte7: hw thread index
    mtspr(SPRG_SPIinfo, spiInfo);

    // Make sure QPX control parameters are at their defaults
    if (ProcessorThreadID() == 0) // Only needs to be processed on one thread within the core
    {
        uint32_t qpxNANexcept = CONFIG_QPX_NAN_EXCEPT;  // Set default NAN exception enablement
        uint32_t qpxINFexcept = CONFIG_QPX_INF_EXCEPT;  // Set default INF exception enablement
        uint64_t auxcr0 = 0;
        if (qpxNANexcept == 1)
        {
            auxcr0 |= AXUCR0_ENAB_NAN_IND_0 | AXUCR0_ENAB_NAN_IND_1 | AXUCR0_ENAB_NAN_IND_2 | AXUCR0_ENAB_NAN_IND_3;
        }
        if (qpxINFexcept == 1)
        {

            auxcr0 |= AXUCR0_ENAB_INF_IND_0 | AXUCR0_ENAB_INF_IND_1 | AXUCR0_ENAB_INF_IND_2 | AXUCR0_ENAB_INF_IND_3;
        }
        mtspr(SPRN_AXUCR0, auxcr0);
    }

    // Make sure UPC hardware is set to defaults
    // Only needs to be processed on one thread within the core.
    // Also have similar code in JobControl.cc
    if (ProcessorThreadID() == 0)
    {
        UPC_Init(pAppState->JobID);
    }

    // Setup the CCR3[EEPRI] value.     
    uint32_t enable_eepri = CNK_EEPRI_DEFAULT;
    App_GetEnvValue("BG_EEPRIORITY", &enable_eepri);
    uint64_t ccr3_value = mfspr(SPRN_CCR3);
    if (enable_eepri) {
        ccr3_value |= CCR3_EEPRI;
    } else {
        ccr3_value &= ~(CCR3_EEPRI);
    }
    mtspr(SPRN_CCR3, ccr3_value); // Set the requested value in the CCR3 register. 

    // Cache certain values in hwthread state for use in fast speculation
    // syscall and interrupt paths.
    uint64_t proc_threads_per_core = (pAppState->Active_Processes <= 16) ?
					4 : (64 / pAppState->Active_Processes);
    uint64_t specindex = ProcessorThreadID() % proc_threads_per_core;
    HWThreadState_t *pHWT = GetMyHWThreadState();
    vmm_getPID(pProc->Tcoord, &pHWT->PhysicalPID);
    vmm_getSpecPID(pProc->Tcoord, specindex, &pHWT->PhysicalSpecPID);
    pHWT->L1P_cfg_pf_sys = *((volatile uint64_t *) L1P_CFG_PF_SYS);


    // @deprecated - intended for DD2 BU only
    uint32_t livelockBusterInterval = -1;

    if ( App_GetEnvValue("BG_LLBCHECK", &livelockBusterInterval) ) {

	if ( ( ProcessorThreadID() == 0 ) && ( ! TI_isDD1() ) ) {

	    uint64_t xucr1 = mfspr( SPRN_XUCR1 );
	    xucr1 &= ~XUCR1_LL_TB_SEL(-1);
	    xucr1 |=  XUCR1_LL_TB_SEL(livelockBusterInterval);  
	    mtspr(SPRN_XUCR1,xucr1);

	    // Clear the error counters:
	    unsigned targetSlave = BGQCFAM_CoreToSlave(ProcessorCoreID());
	    static uint64_t llbits = _BN(56) | _BN(57) | _BN(58) | _BN(59);
	    BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_AND(targetSlave), (~llbits));

	    uint64_t pccr0 = BgDcrPcbRead (BGQ_TI_SCOM_CFAM_A2SCOM_PCCR0(targetSlave));
	    pccr0 &= ~(_B4(51, 0xF));
	    BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_A2SCOM_PCCR0(targetSlave), pccr0);
	}
    }
    
    //

    uint32_t ipiMessagePoll = 0;
    App_GetEnvValue("BG_IPIMESSAGEPOLL", &ipiMessagePoll);
    if (ipiMessagePoll && (pProc->State != ProcessState_RankInactive))
    {
        // Enable monitoring of IPI message delivery
        IPI_enableMessageChecker();
    }

    // Launch application on process(es) leader, and idle other cores until pthread_create() is called.
    // All cores make a trip through the scheduler.
    if ( IsProcessLeader() )
    {
        // Determine if this process is enabled for system call tracing.
        if(((uint32_t)pAppState->RankForStrace == pProc->Rank) && (pProc->State != ProcessState_RankInactive))
        {
            pProc->straceEnabled = true;
        }

        // Set pattern into the guarded area and remove the area from the L1 cache
        Guard_Prepare(NULL, (void*)pProc->Guard_Start, pProc->Guard_Size);

        TRACE( TRACE_Process, ("(I) Creating App main() thread on physical thread index %d .\n", ProcessorID() ));
        App_CreateMainThread( pProc->Proc_EntryPoint, 0);
        // Does not return
    }
    else
    {   
        if (pProc->State == ProcessState_RankInactive)
        {
            // Begin exit flow for all threads of a process that are "Rank Inactive"
            App_Exit(AppExit_Phase1, 1);
            // Previous call will not return
        }

        TRACE( TRACE_Process, ("(I) App_Run[%2d:%d]: Secondary thread entering scheduler.\n",
                                 ProcessorCoreID(), ProcessorThreadID()) );

        // Enter the scheduler, polling in the Kernel KThread looking for an application kThread to dispatch.
        Scheduler();
        // Does not return
    }
}

// Find the AppState object associated with a give Job Id
AppState_t* App_GetAppFromJobid(uint32_t jobid)
{
    // Search the AppState objects to find a matching job number
    AppState_t *appState;
    int appIndex;
    for (appIndex=0; appIndex<CONFIG_MAX_APP_APPSTATES; appIndex++)
    {
        appState = &(NodeState.AppState[appIndex]);
        if (appState->JobID == jobid)
            return appState;
    }
    return NULL;
}


// This interface is used when the job controller wishes to send a signal to a job.
int App_SignalJob(uint32_t jobid, int signum)
{
    // Find the App State object associated with this Job Id
    AppState_t *appState = App_GetAppFromJobid(jobid);
    if (!appState)
    {
        // No active AppState object was found for the supplied jobid.
        // Not sure what we need to do here. For now, just return a failure.
        return -1; 
    }
    // If the job controller is killing the entire job, set a flag in the AppState object
    // for the process exit flows
    if (signum == SIGKILL)
    {
        appState->jobControlIssuedSIGKILL = 1;
        appState->jobControlSIGKILLstart = GetTimeBase();
    }
    int i;
    uint64_t selectMask = _BN(0);
    uint64_t processLeaderMask = appState->ProcessLeaderMask;
    for (i=0; i< 64; i++)
    {
        if (selectMask & processLeaderMask)
        {
            HWThreadState_t *hwt = GetHWThreadStateByProcessorID(i);
            KThread_t *kthr = hwt->SchedSlot[CONFIG_SCHED_SLOT_FIRST];
            // Only send the signal to the process if it is in an active state
            if (kthr->pAppProc->State == ProcessState_Started)
            {
                // Send IPI to the hardware thread that is/was the home of the process leader.
                Signal_Deliver(kthr->pAppProc, 0, signum);
            }
            // reset mask in hopes of an early exit from the loop
            processLeaderMask &= ~selectMask;
            if (!processLeaderMask) break;
        }
        selectMask >>= 1;
    }
    return 0;
}

//
//   This is the target of the ipi load agent request 
//   
//
void App_LoadAgent()
{
    AppState_t *pAgentAppState = GetMyAppState();
    AppProcess_t *pAgentProc = GetMyProcess();

    // Initialize job-scoped resources.
    // Set the state of the Application to indicate that a Load is in progress
    App_SetLoadState(AppState_Loading, 0, 0);

    // Determine if there are any active ranks on this node.
    AppState_t *parentAppState = (AppState_t*)pAgentAppState->parentAppState;
    AppProcess_t *parentAppProc = GetFirstProcess(parentAppState);
    int agentActive = 0;
    while (parentAppProc)
    {
       if (parentAppProc->State == ProcessState_Active )
       {
           agentActive = 1;
           break;
       }
       parentAppProc = parentAppProc->nextProcess;
    }
    if (!agentActive)
    {
        pAgentProc->State = ProcessState_AgentInactive;
    }

    // !!! Should be checking return code and figure out how to terminate. Currently the following method always returns 0      
    Process_SetupMemory();

    if (File_ProcessSetup() != 0)
    {
        // !!!! Need to define some way to surface this error. The state in the AppState object for the Agent will reflect
        // !!!! the error but this is not looked at. The ACK for the loadjob has already been completed. 
        // !!!! This should not be a user-caused error therefore maybe we can bail with a KERNEL RAS event
        printf("(E) Bad return code from File_ProcessSetup()\n");
    }
    // If an app agent is configured on thread id 0, install the TLBs from thread id 0. If there is only one 
    // app agent defined, then regardless of what thread it is on, we need to install the TLBs
    if ((ProcessorThreadID() == 0) ||  
        (popcnt64(NodeState.AppAgents) == 1))
    {
        // install the TLBs
        vmm_installStaticTLBMap(pAgentProc->Tcoord);
        
        // Set background scrub TLB location                                                                                                    
        vmm_setScrubSlot();
    }
    mtspr(SPRN_PID, pAgentProc->PhysicalPID);
    isync();
    Kernel_Barrier(Barrier_AppAgents);
    App_InitKernelInfo(pAgentProc, pAgentAppState);
    
    // Setup the SPR used by the kernel to implement user-state kernel SPIs. We are setting this value directly
    // into the hardware SPR register. The low-core register state restore flows will not write to the hardware SPRs.
    // This value will be maintained/updated directly in the hardware SPRs since the information is not kthread scoped.
    // setup sprg containing info used by user-state SPIs

    // ?????????????   DO we need the SPI register loaded up with the Application information or AppAgent process information????
    //AppState_t *parentAppState = pAppState->parentAppState;
    uint64_t spiInfo = (((uint64_t)(pAgentAppState->AppStateIndex)) << SPRG_SPIinfo_AppLeaderCore) | // byte3 bits 0:3 App Leader core index.
                       (((uint64_t)(pAgentAppState->Active_Cores-1)) << SPRG_SPIinfo_NumCores) |     // byte3 bits 4:7 Number of cores in App - 1
                       (pAgentAppState->Active_Processes << SPRG_SPIinfo_NumProcs) |                 // byte6: number of processes  
                       (ProcessorID());                                                         // byte7: hw thread index
    mtspr(SPRG_SPIinfo, spiInfo);

    App_CreateMainThread( pAgentProc->Proc_EntryPoint, 0 );
    // Does not return
}

int App_Start(AppState_t *appState)
{
    // Verify the load state of the app is correct
    if (appState->LoadState != AppState_LoadComplete)
    {
        return -1;
    }
    // Change the state of the App
    appState->LoadState = AppState_StartIssued;

    ppc_msync();

    uint64_t processLeaderMask = appState->ProcessLeaderMask;
    uint64_t selectMask;
    int i = 0; 
    //printf("Starting jobid %016lx on core %d\n", appState->JobID, appState->AppStateIndex);
    while (processLeaderMask)
    {
        selectMask = _BN(i);
        if (processLeaderMask & selectMask)
        {   
            // Found a process leader. Get the process leader kthread on the selected hardware thread
            HWThreadState_t *hwt = GetHWThreadStateByProcessorID(i);
            KThread_t *kthread = hwt->SchedSlot[CONFIG_SCHED_SLOT_FIRST];
            IPI_run_scheduler(i, kthread, SCHED_STATE_RESET);
            processLeaderMask &= ~selectMask; // turn off the bit we processed with the hopes of exiting the while loop early
        }
        i++;
    }
    // Start any AppAgents that may have been configured
    for (i=0; i<CONFIG_MAX_APP_AGENTS; i++)
    {
        if (NodeState.AppAgents & _BN(i))
        {
            int appAgentProcessorId = NodeState.FirstAppAgentProcessorID + i;
            HWThreadState_t *hwt = GetHWThreadStateByProcessorID(appAgentProcessorId);
            KThread_t *kthread = hwt->SchedSlot[CONFIG_SCHED_SLOT_FIRST];
            IPI_run_scheduler(appAgentProcessorId, kthread, SCHED_STATE_RESET);
        }
    }
    return 0;
}
#endif                                                                                                                 // FUSEDOS

int App_RunPreLoadedApp(void)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   int numCores = 0;
   void* standalone_appstate_desc = NULL;

   // The App was pre-loaded. Obtain the application information from the preloaded location in memory
   if(((NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin)/CONFIG_CNK_PAGE_SIZE) > 2048)
       standalone_appstate_desc = (void*)CONFIG_STANDALONE_APPSTATE;
   else
       standalone_appstate_desc = (void*)CONFIG_STANDALONE_APPSTATE_L2;

   vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                         (void*)(ROUND_DN_1M((uint64_t)standalone_appstate_desc + NodeState.DomainDesc.ddrOrigin)),
                         (void*)(ROUND_DN_1M((uint64_t)standalone_appstate_desc)),
                         1024*1024,
                         0,
                         0);                                                                                 
   if ( (uint64_t)(((AppState_t *)standalone_appstate_desc)->MAGIC) != APP_STANDALONE)
   {
       vmm_UnmapUserSpace(standalone_appstate_desc, 1024*1024, 0);
       standalone_appstate_desc = (void*)CONFIG_STANDALONE_APPSTATE_L2;
       vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                        (void*)ROUND_DN_1M((uint64_t)standalone_appstate_desc + NodeState.DomainDesc.ddrOrigin),
                        (void*)ROUND_DN_1M((uint64_t)standalone_appstate_desc),
                        1024*1024,
                        0,
                        0);                                                                                 

       if ( (uint64_t)(((AppState_t *)standalone_appstate_desc)->MAGIC) != APP_STANDALONE)
       {
           printf("Magic Value not recognized! value:0x%016lx. expecting:0x42475153544e4400\n", ((AppState_t *)standalone_appstate_desc)->MAGIC);
           while(1) { }
       }
   }
   AppState_t *pFirstAppState = &NodeState.AppState[0]; // preloaded apps always use the first AppState object in the array
   // Move the AppState structure into the area we care about
   memcpy(pFirstAppState, (AppState_t *)standalone_appstate_desc, sizeof(NodeState.AppState[0]));
   // Save the job id for the  run message.
   preload_jobid = pFirstAppState->JobID;
   // Set the number of cores to all the enabled cores in the node minus the one reserved kernel core
   numCores = NodeState.NumCoresEnabled - 1; // number of cores for applicate use. One core is always reserved for the kernel 

   // Build the 'job setup' message in the space that contained the standalone app state.
   bgcios::jobctl::SetupJobMessage* setupMsg = (bgcios::jobctl::SetupJobMessage*)(standalone_appstate_desc);
   memset( setupMsg, 0, sizeof(bgcios::jobctl::SetupJobMessage));

   Personality_t* pers = GetPersonality();
   // Setup the SetupJobMessage 
   setupMsg->header.jobId =  pFirstAppState->JobID;
   setupMsg->corner.aCoord = 0;
   setupMsg->corner.bCoord = 0;
   setupMsg->corner.cCoord = 0;
   setupMsg->corner.dCoord = 0;
   setupMsg->corner.eCoord = 0;
   setupMsg->corner.core = 0;
   setupMsg->shape.aCoord = pers->Network_Config.Anodes;
   setupMsg->shape.bCoord = pers->Network_Config.Bnodes;
   setupMsg->shape.cCoord = pers->Network_Config.Cnodes;
   setupMsg->shape.dCoord = pers->Network_Config.Dnodes;
   setupMsg->shape.eCoord = pers->Network_Config.Enodes;
   setupMsg->shape.core = numCores;
   // Set ourself as the job leader node
   setupMsg->jobLeader.aCoord = pers->Network_Config.Acoord;
   setupMsg->jobLeader.bCoord = pers->Network_Config.Bcoord;
   setupMsg->jobLeader.cCoord = pers->Network_Config.Ccoord;
   setupMsg->jobLeader.dCoord = pers->Network_Config.Dcoord;
   setupMsg->jobLeader.eCoord = pers->Network_Config.Ecoord;
   setupMsg->jobLeader.core = 0;

   // Keep the file path field NULL. This will cause us to default to an ABCDET mapping
   //setupMsg->mapFilePath

   // Setup the class route information for configuring the MU. 
   // Since the setup code runs in each node, we only need to put our node in the array
   setupMsg->numNodes = 1; 
   setupMsg->nodesInJob[0].npClassRoute.input = pers->Network_Config.PrimordialClassRoute.CollectiveTypeAndUpPortInputs;
   setupMsg->nodesInJob[0].subBlockClassRoute.input = pers->Network_Config.PrimordialClassRoute.GlobIntUpPortInputs;
   setupMsg->nodesInJob[0].npClassRoute.output = pers->Network_Config.PrimordialClassRoute.CollectiveUpPortOutputs;
   setupMsg->nodesInJob[0].subBlockClassRoute.output = pers->Network_Config.PrimordialClassRoute.GlobIntUpPortOutputs;
   setupMsg->nodesInJob[0].coords.aCoord = pers->Network_Config.Acoord;
   setupMsg->nodesInJob[0].coords.bCoord = pers->Network_Config.Bcoord;
   setupMsg->nodesInJob[0].coords.cCoord = pers->Network_Config.Ccoord;
   setupMsg->nodesInJob[0].coords.dCoord = pers->Network_Config.Dcoord;
   setupMsg->nodesInJob[0].coords.eCoord = pers->Network_Config.Ecoord;
   setupMsg->nodesInJob[0].coords.core = 0;

   // Setup the load job message for the process setup. Reuse the space that the appstate description occupied. 
   bgcios::jobctl::LoadJobMessage* loadMsg = (bgcios::jobctl::LoadJobMessage*)((uint64_t)standalone_appstate_desc + sizeof(bgcios::jobctl::SetupJobMessage));
   memset(loadMsg, 0, sizeof(bgcios::jobctl::LoadJobMessage));
   
   // Setup the loadMsg for process setup
   loadMsg->header.jobId = pFirstAppState->JobID;
   loadMsg->numArguments = pFirstAppState->App_Argc;
   memcpy(loadMsg->arguments, pFirstAppState->App_Args, sizeof(loadMsg->arguments));
   memcpy(loadMsg->variables, pFirstAppState->App_Env, sizeof(loadMsg->variables));
   loadMsg->variableSize = MIN( sizeof(pFirstAppState->App_Env), sizeof(loadMsg->variables));
   loadMsg->header.jobId = pFirstAppState->JobID;
   loadMsg->userId = pFirstAppState->UserID;
   loadMsg->groupId = pFirstAppState->GroupID;
   strncpy(loadMsg->workingDirectoryPath, ".", sizeof(loadMsg->workingDirectoryPath));
   loadMsg->umask = 0;
// memcpy(loadMsg->secondaryGroups, appState->SecondaryGroups, sizeof(gid_t) * loadMsg->numSecondaryGroups);
   loadMsg->rankForStrace = bgcios::jobctl::DisableStrace;
   
   // Set the number of Ranks Per Node. Override if environment variable is specified.
   uint32_t ranksPerNode = 1; 
   GetEnvValue(loadMsg, "BG_PROCESSESPERNODE", &ranksPerNode);
   setupMsg->numRanksPerNode = (uint16_t)ranksPerNode; 

   // Setup the number of ranks active in this job.
   setupMsg->numRanks =  pers->Network_Config.Anodes * pers->Network_Config.Bnodes * pers->Network_Config.Cnodes * pers->Network_Config.Dnodes * pers->Network_Config.Enodes * setupMsg->numRanksPerNode;
   GetEnvValue(loadMsg, "BG_NP", &setupMsg->numRanks);

   // Call the interface to setup the requested process configuration. If there is a problem with the request, the
   // following function will return an error.
   int map_results = Process_SetupJob(setupMsg);
   assert(map_results == bgcios::Success);
   
   AppState_t *appState;
   map_results = Process_SetupMap(loadMsg, &appState);
   assert(map_results == bgcios::Success);
   
   // Setup the process structure for the App Agent processes. Pass pointer to the parent appState object
   Process_SetupAgentMap(loadMsg, appState);

   // Set fields so uname system call works as expected.
   strncpy(NodeState.NodeName, "localhost", sizeof(NodeState.NodeName));
   strncpy(NodeState.Release, "2.6.32-131.0.15.el6.ppc64", sizeof(NodeState.Release));

   // Clear and unmmap this area so that it can be used/mapped later by cnk
   memset(standalone_appstate_desc, 0, 1024*1024); 
   vmm_UnmapUserSpace(standalone_appstate_desc, 1024*1024, 0);

   // Make sure UPC hardware is set to defaults
   // Only needs to be processed on one thread within the core.
   // Also have similar code in JobControl.cc
   //if (ProcessorThreadID() == 0)
   //{
       UPC_Init(appState->JobID);
   //}

   // Signal the hardware threads in this application
   uint64_t appThreadMask = appState->AppThreadsMask;
   int threadCount = appState->Active_Cores * 4;
   for (int i = cntlz64(appThreadMask); threadCount; i++)
   {
       if (appThreadMask & _BN(i))
       {
           IPI_load_application(i);
       }
       threadCount--;
   }

   // Spin waiting for the Application to complete the load sequence
   int load_complete = 0;
   while(!(load_complete == AppState_LoadComplete))
   {
       load_complete = appState->LoadState;
   }

   appState = App_GetAppFromJobid(preload_jobid);
   assert(appState != NULL);
   assert(appState->LoadState == AppState_LoadComplete);
   
   App_Start(appState);
   while(1) { /* Spin. For pre-loaded apps, our job is done */ }
#else                                                                                                                  // FUSEDOS
   return 0;                                                                                                           // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int App_IsCollectiveLoadActive()
{
    // If the iteration count is non-zero, then collective load is active.
    return ((GetMyAppState()->jobLeaderData.collectiveLoadStatus.word.iteration) ? 1 : 0);
}

int App_CheckGloballyForLoadFailure()
{
    MUHWI_Destination_t dest;
    dest.Destination.Destination = 0;
    volatile uint64_t errCount = 0;
    if(GetMyAppState()->LoadState == AppState_LoadFailed)
        errCount = 1;
    
    if ( ( NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_MU) != 0 )
    {
        int rc;
        rc = mudm_bcast_reduce(NodeState.MUDM, 
                               MUDM_REDUCE_ALL,
                               MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD,
                               (void*)&errCount,
                               sizeof(errCount),
                               dest,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                               14,
                               MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                               (void*)&errCount);
        MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
        ppc_msync();
    }
    return errCount;
}

void App_ActivateCollectiveLoad()
{
    // Setting the iteration count to 1 in the collective status object activates collective loading
    GetMyAppState()->jobLeaderData.collectiveLoadStatus.word.iteration = 1;

    if ( ( NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_MU) == 0 )
    {
        return;
    }
    
    GetMyAppState()->prohibitLoad = 0;
    if(GetMyAppState()->LoadSequence == ~(0u))
    {
        GetMyAppState()->LoadSequence = 0;
        GetMyAppState()->prohibitLoad = 1;
    }
    
    MUHWI_Destination_t dest;
    dest.Destination.Destination = 0;
    volatile uint64_t maxSequence = GetMyAppState()->LoadSequence;
    int rc;
    rc = mudm_bcast_reduce(NodeState.MUDM, 
                           MUDM_REDUCE_ALL,
                           MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MAX,
                           (void*)&maxSequence,
                           sizeof(maxSequence),
                           dest,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                           14,
                           MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                           (void*)&maxSequence);
    MUSPI_GIBarrierEnterAndWait( &systemJobGIBarrier );
    
    ppc_msync();
    
    GetMyAppState()->MaxSequence = maxSequence;
    Kernel_WriteFlightLog(FLIGHTLOG, FL_MPMDSEQMX, maxSequence,0,0,0);
}

void App_SetupLoadLeader()
{
    Personality_t* pers = GetPersonality();
    MUHWI_Destination_t mycoord;
    volatile uint64_t nodeCount = 1;
    volatile uint64_t minSequence;
    mycoord.Destination.Destination = 0;
    mycoord.Destination.A_Destination = pers->Network_Config.Acoord;
    mycoord.Destination.B_Destination = pers->Network_Config.Bcoord;
    mycoord.Destination.C_Destination = pers->Network_Config.Ccoord;
    mycoord.Destination.D_Destination = pers->Network_Config.Dcoord;
    mycoord.Destination.E_Destination = pers->Network_Config.Ecoord;
    
    minSequence = mycoord.Destination.Destination;
    if(GetMyAppState()->prohibitLoad)
    {
        minSequence = ~(0);
    }
    
    int rc;
    MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
    rc = mudm_bcast_reduce(NodeState.MUDM, 
                           MUDM_REDUCE_ALL,
                           MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN,
                           (void*)&minSequence,
                           sizeof(minSequence),
                           mycoord,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                           13,
                           MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                           (void*)&minSequence);
    MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
    rc = mudm_bcast_reduce(NodeState.MUDM, 
                           MUDM_REDUCE_ALL,
                           MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD,
                           (void*)&nodeCount,
                           sizeof(nodeCount),
                           mycoord,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                           13,
                           MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                           (void*)&nodeCount);
    MUSPI_GIBarrierEnterAndWait( &systemLoadJobGIBarrier );
    
    if(minSequence == mycoord.Destination.Destination)
        GetMyAppState()->IsLoadLeader = 1;
    else
        GetMyAppState()->IsLoadLeader = 0;
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_MPMDSETUP, nodeCount, GetMyAppState()->IsLoadLeader, minSequence,0);
    
    GetMyAppState()->LoadNodeCount = nodeCount;
}

#endif                                                                                                                 // FUSEDOS
