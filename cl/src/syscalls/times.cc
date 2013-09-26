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
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/times.h> 

uint64_t sc_times(SYSCALL_FCN_ARGS)
{
    struct tms *buf = (tms *)r3;

    TRACESYSCALL(("(I) %s[%d]: buf=0x%016lx\n", __func__, ProcessorID(), (unsigned long)buf));

    // Check for error conditions.
    if (!VMM_IsAppAddress(buf, sizeof(*buf)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    // All time is attributed to the user application.  We divide by 10000 to convert
    // to hundreths of a second to match Linux.
    uint64_t now = (GetCurrentTimeInMicroseconds() - GetMyAppState()->JobStartTime) / (1000000 / CONFIG_TICK_RATE);
    buf->tms_utime = (clock_t) now;
    buf->tms_stime =  0;                          
    buf->tms_cutime = 0;                          
    buf->tms_cstime = 0;                          

    return CNK_RC_SUCCESS(buf->tms_utime);
}
