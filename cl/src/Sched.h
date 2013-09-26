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

#ifndef _SCHED_H_ // Prevent multiple inclusion.
#define _SCHED_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdint.h>

#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/common/bgq_bitnumbers.h>
#include <firmware/include/personality.h>

#include <cnk/include/Config.h>
#include "KThread.h"
#include "Regs.h"
#include <sched.h>


#define SCHED_STATE_RUN         (0)      // Exists and is not blocked.

#define SCHED_STATE_FREE       _BN(63)  // Available for allocation
#define SCHED_STATE_SLEEP      _BN(62)  // Sleeping waiting for a time period to expire
#define SCHED_STATE_FUTEX      _BN(61)  // Waiting on a Futex
#define SCHED_STATE_RESET      _BN(60)  // Transitional state used when preparing a main process thread and a KThread for clone or migration
#define SCHED_STATE_HOLD       _BN(59)  // A tool has put this thread into a hold state 
#define SCHED_STATE_APPEXIT    _BN(58)  // Application Exit Phase 1 complete awaiting final phase of termination.
#define SCHED_STATE_POOF       _BN(57)  // Comm thread in poof state, awaiting an interrupt
#define SCHED_STATE_SUSPEND    _BN(56)  // A thread has been suspended due to a signal notification event to a tool
#define SCHED_STATE_FLOCK      _BN(55)  // A thread has been blocked due to a blocking flock
#define SCHED_STATE_RESERVED   _BN(54)  // A thread is free but reserved (for swapping with a migrating thread)

// Redefine the comm thread behavior to the SCHED_RR policy
#define SCHED_COMM   SCHED_RR

// Bit values for KThread SchedulerDivert field
#define SCHED_DIVERT_TOOL      _BN(63)
#define SCHED_DIVERT_TERMINATE _BN(62)
#define SCHED_DIVERT_COREDUMP  _BN(61)

// Enter the scheduler and search for kthread to dispatch. If there is no kthread to 
// dispatch on this hardware thread, enter a hardware thread wait
void __NORETURN Scheduler( void );

// Mark the kthread state to be in a wait condition based on the passed in SCHED_STATE definition
// This function may issue an IPI interrupt to a hardware thread
void Sched_Block( struct KThread_t *pKThr, uint32_t block_state );

// Wake a kthread that is currently in a waiting scheduler state
// This function may issue an IPI interrupt to a hardware thread
void Sched_Unblock( struct KThread_t *pKThr, uint32_t unblock_state ); 

// Reset the scheduler state after a job exits
void Sched_Reset( void );

// Block all kthreads on the current hardware thread for application exit
void Sched_BlockForExit();

#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.

