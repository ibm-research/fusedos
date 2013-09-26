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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <asm/unistd.h>
#include <sys/time.h>
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <unistd.h>                                                                                                    // FUSEDOS
#include <sys/syscall.h>                                                                                               // FUSEDOS
#else                                                                                                                  // FUSEDOS
#include <spi/include/l1p/pprefetch.h>

#ifndef SA_INTERRUPT

/* When moving to the current Linux headers, this define is no longer provided.
 * Temporarily kept here for consistency with glibc setting. */

#define SA_INTERRUPT    0x20000000u /* dummy -- ignored */
#endif

uint64_t sc_sigreturn(SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry\n", __func__, ProcessorID()));
    return Signal_Return();
}

uint64_t sc_rt_sigreturn(SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry\n", __func__, ProcessorID()));
    return Signal_Return();
}

static uint64_t sigaction(const char *func, int core, int sig,
			  const kern_sigaction_t *act,
			  kern_sigaction_t *oldact)
{
    if (act != NULL)
    {
	if (!VMM_IsAppAddress(act, sizeof(*act)))
	{
	    return CNK_RC_FAILURE(EFAULT);
	}
	TRACESYSCALL(("(I) %s[%d]: act{ sa_handler:%p, flags=%lx }\n",
			func, core, act->sa_handler, act->sa_flags));
	if ((act->sa_handler != SIG_DFL) && (act->sa_handler != SIG_IGN) &&
	    !VMM_IsAppAddress((uint64_t *) act->sa_handler,
			      2 * sizeof(uint64_t)))
	{
	    return CNK_RC_FAILURE(EFAULT);
	}
    }

    if ((oldact != NULL) && (!VMM_IsAppAddress(oldact, sizeof(*oldact))))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    if ((sig < 0) || (sig > KERN_NSIG))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    int supported_flags = SA_SIGINFO | SA_RESETHAND | SA_NODEFER
	| SA_INTERRUPT /* Issue 1820 ... this appears to be a no-op */
	| SA_RESTART /* NOTE: glibc signal() appears to set this flag
			      implicitly ... ignore it for now.  If we have
			      to support SA_RESTART we have lots of work
			      to do :) */
	| SA_BGQ_PP_PAUSE /* disable list prefetcher during signal delivery */
	;

    if ((act != NULL) && (act->sa_flags & ~supported_flags))
    {
	TRACESYSCALL(("(I) %s[%d] unsupported flags: %lx\n",
			func, core, act->sa_flags));
	return CNK_RC_FAILURE(EINVAL);
    }

    if ((sig == SIGKILL) || (sig == SIGSTOP) || (sig == SIGSTKFLT))
    {
	TRACESYSCALL(("(I) %s[%d]: cannot catch SIGKILL, "
		      "SIGSTOP, or SIGSTKFLT.\n",
			func, core ));
	return CNK_RC_FAILURE(EINVAL);
    }

    return Signal_SetAction(sig, act, oldact);
}

uint64_t sc_sigaction(SYSCALL_FCN_ARGS )
{
    int processorID = ProcessorID();
    int                     sig    = (int) r3;
    const kern_sigaction_t* act    = (const kern_sigaction_t *) r4;
    kern_sigaction_t*       oldact = (kern_sigaction_t *) r5;
    TRACESYSCALL(("(I) %s[%d]: sig=%d, act=0x%08lx, oldact=0x%08lx.\n",
		    __func__, processorID, sig, (uint64_t)act, (uint64_t)oldact ));
    return sigaction(__func__, processorID, sig, act, oldact);
}
#endif                                                                                                                 // FUSEDOS

uint64_t sc_rt_sigaction(SYSCALL_FCN_ARGS)
{
    int processorID = ProcessorID();
    int                     sig        = (int) r3;
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    const struct sigaction* act        = (const struct sigaction*)r4;                                                  // FUSEDOS
    struct sigaction*       oldact     = (struct sigaction*)r5;                                                        // FUSEDOS
#else                                                                                                                  // FUSEDOS
    const kern_sigaction_t* act        = (const kern_sigaction_t *) r4;
    kern_sigaction_t*       oldact     = (kern_sigaction_t *) r5;
#endif                                                                                                                 // FUSEDOS
    size_t                  sigsetsize = (size_t) r6;
    TRACESYSCALL(("(I) %s[%d]: sig=%d, act=0x%08lx, oldact=0x%08lx, "
		  "sigsetsize=%ld.\n",
		    __func__, processorID, sig, (uint64_t)act, (uint64_t)oldact,
		    sigsetsize ));

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    return syscall(SYS_rt_sigaction, r3, r4, r5, r6);                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
    if (sigsetsize != sizeof(kern_sigset_t))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    return sigaction(__func__, processorID, sig, act, oldact);
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
static uint64_t sigprocmask(const char *func, int core, int how,
			    const kern_sigset_t *set, kern_sigset_t *oldset)
{
    if ((set != NULL) && (!VMM_IsAppAddress(set, sizeof(*set))))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    if ((oldset != NULL) && (!VMM_IsAppAddress(oldset, sizeof(*oldset))))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    return Signal_SetMask(how, set, oldset);
}

uint64_t sc_sigprocmask(SYSCALL_FCN_ARGS)
{
    int processorID = ProcessorID();
    int            how    = (int) r3;
    kern_sigset_t* set    = (kern_sigset_t*) r4;
    kern_sigset_t* oldset = (kern_sigset_t*) r5;

    TRACESYSCALL(("(I) %s[%d]: how=%d set=%p oldset=%p\n",
		    __func__, processorID, how, set, oldset ));

    return sigprocmask(__func__, processorID, how, set, oldset);
}
#endif                                                                                                                 // FUSEDOS

uint64_t sc_rt_sigprocmask(SYSCALL_FCN_ARGS)
{
    int processorID = ProcessorID();
    int            how        = (int) r3;
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    sigset_t* set             = (sigset_t*)r4;                                                                         // FUSEDOS
    sigset_t* oldset          = (sigset_t*)r5;                                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
    kern_sigset_t* set        = (kern_sigset_t*) r4;
    kern_sigset_t* oldset     = (kern_sigset_t*) r5;
#endif                                                                                                                 // FUSEDOS
    size_t         sigsetsize = (size_t) r6;

    TRACESYSCALL(("(I) %s[%d]: how=%d set=%p oldset=%p\n",
		    __func__, processorID, how, set, oldset ));

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: sigsetsize=%d\n", __func__, processorID, (int)sigsetsize));                             // FUSEDOS
    return syscall(SYS_rt_sigprocmask, r3, r4, r5, r6);                                                                // FUSEDOS
#else                                                                                                                  // FUSEDOS
    if (sigsetsize != sizeof(kern_sigset_t))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    return sigprocmask(__func__, processorID, how, set, oldset);
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// **** Remainder not yet implemented ****

uint64_t sc_restart_syscall( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_waitpid( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_pause( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}






uint64_t sc_sgetmask( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}

uint64_t sc_ssetmask( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sigsuspend( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sigpending( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_wait4( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}







uint64_t sc_rt_sigpending( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_rt_sigtimedwait( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_rt_sigqueueinfo( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_rt_sigsuspend( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_sigaltstack( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}



uint64_t sc_waitid( SYSCALL_FCN_ARGS)
{
   int processorID = ProcessorID();

   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}


uint64_t sc_signal( SYSCALL_FCN_ARGS)
{
   // glibc does not make a signal syscall anymore.  No reason to implement it then.
   int processorID = ProcessorID();
   
   TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  ));
   printf("(I) %s[%d]: Entry (add args!).\n", __func__, processorID  );
   return CNK_RC_FAILURE(ENOSYS);
}
#endif                                                                                                                 // FUSEDOS
