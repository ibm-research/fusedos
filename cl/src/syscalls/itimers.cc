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

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#ifndef MILLION
#define MILLION 1000000
#endif

uint64_t sc_getitimer(SYSCALL_FCN_ARGS)
{
    int               which = (int) r3;
    struct itimerval *value = (struct itimerval *) r4;

    TRACESYSCALL(("(I) %s[%d]: which=%d value=0x%p\n", __func__,
		    ProcessorID(), which, value));

    // We support the real and profiler timers only
    if ((which != ITIMER_REAL) && (which != ITIMER_PROF))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    if ((value == NULL) || !VMM_IsAppAddress(value, sizeof(*value)))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    uint64_t remainingMicros, intervalMicros;

    uint64_t rc = Timer_GetTimer(((which == ITIMER_REAL) ? SIGALRM : SIGPROF),
			         &remainingMicros, &intervalMicros);

    value->it_value.tv_sec     = remainingMicros / MILLION;
    value->it_value.tv_usec    = remainingMicros % MILLION;
    value->it_interval.tv_sec  = intervalMicros / MILLION;
    value->it_interval.tv_usec = intervalMicros % MILLION;

    return rc;
}

uint64_t sc_setitimer(SYSCALL_FCN_ARGS)
{
    int               which  = (int) r3;
    struct itimerval *value = (struct itimerval *) r4;
    struct itimerval *ovalue = (struct itimerval *) r5;

    TRACESYSCALL(("(I) %s[%d]: which=%d value=%p ovalue=%p.\n", __func__,
		    ProcessorID(), which, value, ovalue  ));

    // We support the real and profiler timers only
    if ((which != ITIMER_REAL) && (which != ITIMER_PROF))
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    if ((value != NULL) && !VMM_IsAppAddress(value, sizeof(*value)))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    if ((ovalue != NULL) && !VMM_IsAppAddress(ovalue, sizeof(*ovalue)))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    uint64_t valueMicros;
    uint64_t intervalMicros;

    if (value != NULL)
    {
	// Make sure the time values are valid.
	if ((value->it_value.tv_sec < 0) ||
	    (value->it_interval.tv_sec < 0) ||
	    (value->it_value.tv_usec < 0) ||
	    (value->it_value.tv_usec >= MILLION) ||
	    (value->it_interval.tv_usec < 0) ||
	    (value->it_interval.tv_usec >= MILLION))
	{
	    return CNK_RC_FAILURE(EINVAL);
	}
	valueMicros = (((uint64_t) value->it_value.tv_sec) * MILLION) +
				    ((uint64_t) value->it_value.tv_usec);
	intervalMicros = (((uint64_t) value->it_interval.tv_sec) * MILLION) +
					((uint64_t) value->it_interval.tv_usec);
    }
    else
    {
	/* ====================================================================
	 * QUESTION: what to do if value is NULL?  The specification is not at
	 *           all precise on this issue.  A simple test on linux
	 *           indicates that the current implementation behaves as if
	 *           value contains all zeroes (i.e. the timer is stopped).
	 *           The following implementation preserves that behavior.
	 * ================================================================= */
	valueMicros = 0;
	intervalMicros = 0;
    }

    uint64_t remainingMicros, oldIntervalMicros;

    uint64_t rc = Timer_SetTimer(((which == ITIMER_REAL) ? SIGALRM : SIGPROF),
				 valueMicros, intervalMicros,
			         &remainingMicros, &oldIntervalMicros);

    if (ovalue != NULL)
    {
	ovalue->it_value.tv_sec     = remainingMicros / MILLION;
	ovalue->it_value.tv_usec    = remainingMicros % MILLION;
	ovalue->it_interval.tv_sec  = oldIntervalMicros / MILLION;
	ovalue->it_interval.tv_usec = oldIntervalMicros % MILLION;
    }

    return rc;
}
#endif                                                                                                                 // FUSEDOS

uint64_t sc_alarm(SYSCALL_FCN_ARGS)
{
    unsigned int seconds = (unsigned int) r3;

    TRACESYSCALL(("(I) %s[%d]: seconds = %d.\n", __func__,
		    ProcessorID(), seconds));

#if 0
    uint64_t micros = seconds * MILLION;
    uint64_t remainingMicros;

    uint64_t rc = Timer_SetTimer(SIGALRM, micros, 0, &remainingMicros, NULL);
    if (CNK_RC_IS_FAILURE(rc))
    {
	return rc;
    }

   seconds = (unsigned int) ((remainingMicros + (MILLION/2)) / MILLION);
 
   return CNK_RC_SUCCESS(seconds); // return number of remaining seconds
#else
   return CNK_RC_FAILURE(ENOSYS);
#endif
}

uint64_t sc_nanosleep(SYSCALL_FCN_ARGS)
{
    struct timespec* req = (struct timespec*) r3;
    struct timespec* rem = (struct timespec*) r4;

    TRACESYSCALL(("(I) %s[%d]: req=%p rem=%p\n", __func__,
		    ProcessorID(), req, rem ));

    if (req == NULL)
    {
	return CNK_RC_FAILURE(EINVAL);
    }

    if (!VMM_IsAppAddress(req, sizeof(*req)))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

    if ((rem != NULL) && !VMM_IsAppAddress(req, sizeof(*req)))
    {
	return CNK_RC_FAILURE(EFAULT);
    }

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    return nanosleep(req, rem);                                                                                        // FUSEDOS
#else                                                                                                                  // FUSEDOS
    return Timer_NanoSleep(req, rem);
#endif                                                                                                                 // FUSEDOS
}
