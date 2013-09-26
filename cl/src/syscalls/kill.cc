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

// Includes
#include "Kernel.h"

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <signal.h>                                                                                                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
static uint64_t tkill(int tid, int sig)
{
    AppProcess_t *pProc  = GetMyProcess();

    if ((sig < 0) || (sig > KERN_NSIG))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    // Locate the KThread for this tid.
    KThread_t *kthr = GetKThreadFromTid(tid);
    if (kthr == NULL)
    {
	return CNK_RC_FAILURE(ESRCH);
    }

    // Verify that the KThread is part of the current process.
    if (kthr->pAppProc != pProc)
    {
	return CNK_RC_FAILURE(ESRCH);
    }

    // Verify that the target tid is a thread in the correct state to
    // process the request.
    if ((kthr->State &
	    ~(SCHED_STATE_FUTEX |
	      SCHED_STATE_SLEEP |
	      SCHED_STATE_FLOCK |
	      SCHED_STATE_HOLD |
	      SCHED_STATE_SUSPEND)) != 0)
    {
	return CNK_RC_FAILURE(ESRCH);
    }

    if (sig != 0)  // Signal 0 just checks for errors.
    { 
	Signal_Deliver(pProc, GetTID(kthr), sig); 
    }

    return CNK_RC_SUCCESS(0);
}
#endif                                                                                                                 // FUSEDOS

//! \brief  Implement the tkill system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_tkill(SYSCALL_FCN_ARGS)
{
    int processorID = ProcessorID();
    int tid  = r3;
    int sig  = r4;

    TRACESYSCALL(("(I) %s[%d]: tid=%d, sig=%d\n", __func__, processorID, tid, sig ));

    return tkill(tid, sig);
}
#endif                                                                                                                 // FUSEDOS

//! \brief  Implement the tgkill system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_tgkill(SYSCALL_FCN_ARGS)
{
    int processorID = ProcessorID();
    AppProcess_t *pProc  = GetMyProcess();
#if defined __FUSEDOS__                                                                                               // FUSEDOS
	KThread_t *my_KThr = GetMyKThread();                                                                               // FUSEDOS
	KThread_t *target_KThr;                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    int tgid = r3;
    int tid  = r4;
    int sig  = r5;

    TRACESYSCALL(("(I) %s[%d]: tgid=%d, tid=%d, sig=%d\n", __func__, processorID, tgid, tid, sig ));

    // Reject the call if the supplied tgid doesn't match my own tgid or
    // the special -1 value.
    if ((tgid != -1) && (tgid != (int) pProc->PID))
    {
	return CNK_RC_FAILURE(ESRCH);
    }
#if defined __FUSEDOS__                                                                                               // FUSEDOS
	target_KThr = GetKThreadFromTid(tid);                                                                              // FUSEDOS
	if(my_KThr != target_KThr || sig != SIGABRT) {                                                                     // FUSEDOS
		TRACESYSCALL(("(I) %s[%d]: tgkill only supports sending ABORT signal to yourself\n", __func__, processorID));  // FUSEDOS
		return CNK_RC_FAILURE(EINVAL);                                                                                 // FUSEDOS
	}                                                                                                                  // FUSEDOS
	TRACESYSCALL(("(I) %s[%d]: tgkill calling sc_exit to kill myself\n", __func__, processorID));
	return sc_exit(-1, 0, 0, 0, 0, 0); // just kill myself                                                             // FUSEDOS
#else                                                                                                                  // FUSEDOS
    return tkill(tid, sig);
#endif                                                                                                                 // FUSEDOS
}

//! \brief  Implement the kill system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_kill(SYSCALL_FCN_ARGS)
{
    pid_t pid = r3;
    int sig = r4;
    int processorID = ProcessorID();

    TRACESYSCALL(("(I) %s[%d]: pid=%d, sig=%d\n", __func__, processorID, pid, sig ));

    return tkill(pid, sig);
}
#endif                                                                                                                 // FUSEDOS
