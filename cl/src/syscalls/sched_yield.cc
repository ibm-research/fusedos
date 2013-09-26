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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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

uint64_t  sc_sched_yield(SYSCALL_FCN_ARGS)
{
#if !defined(__FUSEDOS__)                                                                        // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Scheduler yield (null)\n", __func__, ProcessorID()));
    // Set a Pending bit to cause this thread to call Scheduler() before it is resumed.
    GetMyKThread()->Pending |= KTHR_PENDING_YIELD;
#endif                                                                                            // FUSEDOS
    return CNK_RC_SUCCESS(0);
}
