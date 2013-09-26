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

#ifndef _CNK_SIGNAL_H_ // Prevent multiple inclusion
#define _CNK_SIGNAL_H_

//
// Inside CNK we have to use the Linux-kernel versions of the various
// signal structures, rather than the user versions, because that's what
// glibc passes through the syscall interface.  The following rather ugly
// series of preprocessor directives attempts to pick up the Linux-kernel
// definitions while avoiding name conflicts with user versions.
//
#undef __KERNEL__
#define sigset_t kern_sigset_t
#define sigaction kern_sigaction_t
#define ucontext kern_ucontext_t
#define siginfo_t kern_siginfo_t
#define clock_t long
#undef _NSIG
#include <asm/signal.h>
#include <asm/ucontext.h>
#include <asm/siginfo.h>
#undef sigset_t
#undef sigaction
#undef ucontext
#undef siginfo_t
#undef clock_t
#if _NSIG != 64
    #error "sizeof(kern_sigset_t) != sizeof(uint64_t)"
#endif
enum { KERN_NSIG = _NSIG };
#undef _NSIG
#define __KERNEL__ 1
#define  _SIGNAL_H
#define sigset_t kern_sigset_t

//! Special signal number to indicate a post job integrity check (message crc/terminate checks, etc)
#define SIGJOBINTEGRITY 39

typedef enum {
    SIG_IGNORE,
    SIG_TERMINATE,
    SIG_COREDUMP,
    SIG_HANDLER,
} SignalAction_t;

typedef struct SignalData_t
{
    SignalAction_t action;
    struct {
	uint64_t descriptor;
	uint64_t address;
	uint64_t toc;
    } handler;
    uint64_t mask;
    int flags;             // Flags (a la sigaction.sa_flags)
} SignalData_t;

typedef struct {
    // The signal trampoline consists of four instructions.
    uint32_t instruction[4];
} SignalTrampoline_t;

// Using "struct KThread_t" rather than the typedef to avoid a circularity.
void Signal_Deliver(struct AppProcess_t *process, int tid, int sig);
void Signal_Exception(int code, uint64_t timebase);
void Signal_TrampolineInit(SignalTrampoline_t *tramp);

uint64_t Signal_Return();
uint64_t Signal_SetAction(int sig,
			  const struct kern_sigaction_t *act,
			  struct kern_sigaction_t *oldact);
uint64_t Signal_SetMask(int how,
			const kern_sigset_t *set,
			kern_sigset_t *oldset);

void Signal_ProcessInit(SignalData_t *sd);

#endif // Add nothing below this line
