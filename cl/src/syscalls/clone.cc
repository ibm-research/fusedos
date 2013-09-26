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
#include <limits.h>
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "cl_debug.h"                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

// TODO: adapt this to FusedOS, if/when required 	// FUSEDOS
__C_LINKAGE uint64_t sc_set_tid_address( SYSCALL_FCN_ARGS )
{
    KThread_t *pKThr = GetMyKThread();
    int *tid_addr = (int *)r3;
    int my_pid = GetPID();

    TRACESYSCALL(("(I) %s[%2d]: Entry: tid_addr=0x%p. Returning PID=%d.\n",
                  __func__, ProcessorID(), tid_addr, my_pid ));
    
    if ( !VMM_IsAppAddress( (void *)tid_addr, sizeof(uint32_t)  ) )
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    pKThr->TID_Address = (void *)tid_addr;

    return CNK_RC_SUCCESS((uint64_t)my_pid);
}


__C_LINKAGE uint64_t sc_clone( SYSCALL_FCN_ARGS)
{
    int  cl_flags        = (int)r3;
    void *cl_child_stack = (void *)r4;
    void *cl_parent_tid  = (void *)r5;
    void *cl_tls         = (void *)r6;
    void *cl_child_tid   = (void *)r7;

    TRACESYSCALL(("(I) %s[%2d]: flags=0x%08x, child_stack=0x%016lx, pTID=0x%016lx, tls=0x%016lx, cTID=0x%016lx.\n",
                 __func__, ProcessorID(),
                  cl_flags, (uint64_t)cl_child_stack, (uint64_t)cl_parent_tid, (uint64_t)cl_tls, (uint64_t)cl_child_tid ));
    
    
#define __CLONE_NEED_FLAGS (CLONE_VM             |\
                            CLONE_FS             |\
                            CLONE_FILES          |\
                            CLONE_SIGHAND        |\
                            CLONE_THREAD         |\
                            CLONE_SYSVSEM        |\
                            CLONE_PARENT_SETTID  |\
                            CLONE_CHILD_CLEARTID |\
                            CLONE_SETTLS         |\
                            CLONE_DETACHED)

    // pthread_create or fork etc?
    if((cl_flags | CLONE_SETTLS) != __CLONE_NEED_FLAGS)
    {
        printf("(W) sc_clone: BAD Flags: %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s.\n",
               (cl_flags & CLONE_VM             ? " VM"             : ""),
               (cl_flags & CLONE_FS             ? " FS"             : ""),
               (cl_flags & CLONE_FILES          ? " FILES"          : ""),
               (cl_flags & CLONE_SIGHAND        ? " SIGHAND"        : ""),
               (cl_flags & CLONE_PTRACE         ? " PTRACE"         : ""),
               (cl_flags & CLONE_VFORK          ? " VFORK"          : ""),
               (cl_flags & CLONE_PARENT         ? " PARENT"         : ""),
               (cl_flags & CLONE_THREAD         ? " THREAD"         : ""),
               (cl_flags & CLONE_NEWNS          ? " NEWNS"          : ""),
               (cl_flags & CLONE_SYSVSEM        ? " SYSVSEM"        : ""),
               (cl_flags & CLONE_SETTLS         ? " SETTLS"         : ""),
               (cl_flags & CLONE_PARENT_SETTID  ? " PARENT_SETTID"  : ""),
               (cl_flags & CLONE_CHILD_CLEARTID ? " CHILD_CLEARTID" : ""),
               (cl_flags & CLONE_DETACHED       ? " DETACHED"       : ""),
               (cl_flags & CLONE_UNTRACED       ? " UNTRACED"       : ""),
               (cl_flags & CLONE_CHILD_SETTID   ? " CHILD_SETTID"   : "")  );

        return CNK_RC_FAILURE(EINVAL);
    }

    // Only verify the cl_tls value if the CLONE_TLS flag is set
    if(cl_flags & CLONE_SETTLS)
    {
        // Validate the address range that will be initialized in
        // Process_InitializeThreadLocalStorage().
        AppProcess_t *proc = GetMyProcess();
        if ((proc->pTData_Start != 0) &&
            !VMM_IsAppAddress((void *) (((uint64_t) cl_tls) - GLIBC_TCB_OFFSET),
                              proc->TData_Size + proc->TBSS_Size))
        {
            return CNK_RC_FAILURE(EINVAL);
        }
    }
    
    // The stack address points to the end of the user stack space.  We don't
    // know the actual size of the stack so just validate the minimum size
    // allowed by the toolchain (128K).
    if ( !VMM_IsAppAddress( (void *)(((uint64_t) cl_child_stack) - PTHREAD_STACK_MIN), PTHREAD_STACK_MIN) ||
         !VMM_IsAppAddress( (void *)cl_parent_tid,  sizeof(uint32_t)  ) ||
         !VMM_IsAppAddress( (void *)cl_child_tid,   sizeof(uint32_t)  ))
    {
        return CNK_RC_FAILURE(EINVAL);
    }

    KThread_t *pKThr = GetMyKThread();
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    int rc = pthread_mutex_lock( &NodeState.cl_thread_mutex );                                                         // FUSEDOS
    if (rc) {                                                                                                          // FUSEDOS
        CL_ERROR("error acquiring cl thread mutex for KThread allocation, %d, %s\n", rc, strerror(rc));                // FUSEDOS
        return CNK_RC_FAILURE(EIO);                                                                                    // FUSEDOS
    }                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    int tid = KThread_LaunchAppSibling( pKThr,           // thread calling pthread_create
                                                cl_child_stack, // the child thread's stack pointer
                                                cl_parent_tid,  // write new TID here
                                                cl_tls,         // into GPR[2], the TLS pointer @MG: GPR[13]?
                                                cl_child_tid,   // clear this on thread exit
                                                cl_flags);       // remember the creation flags

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    rc = pthread_mutex_unlock( &NodeState.cl_thread_mutex );                                                           // FUSEDOS
    if (rc) {                                                                                                          // FUSEDOS
        CL_ERROR("error releasing cl thread mutex for KThread allocation, %d, %s\n", rc, strerror(rc));                // FUSEDOS
        return CNK_RC_FAILURE(EIO);                                                                                    // FUSEDOS
    }                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    if ( tid < 0 )
    {
        return CNK_RC_FAILURE(ENOMEM);
    }

    uint32_t *ppt = (uint32_t *)cl_parent_tid;
    *ppt = tid;
    return CNK_RC_SUCCESS(tid);
}

