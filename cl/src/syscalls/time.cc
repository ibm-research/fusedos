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
#define MILLION 1000000

uint64_t  sc_time(SYSCALL_FCN_ARGS)
{
    time_t *buf = (time_t *)r3;

    TRACESYSCALL(("(I) %s[%d]: buf=0x%016lx\n", __func__, ProcessorID(), (unsigned long)buf));

    // Check for error conditions.
    if (buf && !VMM_IsAppAddress(buf, sizeof(*buf)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    // Calculate the current time in  seconds.
    uint64_t now = GetCurrentTimeInMicroseconds() / MILLION;
    if (buf)
    {
        *buf = (time_t)now;
    }

    return CNK_RC_SUCCESS(now);
}
