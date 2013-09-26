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
#include <sys/time.h>

#define MILLION 1000000 


uint64_t  sc_gettimeofday(SYSCALL_FCN_ARGS)
{
    struct timeval  *tv = (struct timeval  *)r3;
    struct timezone *tz = (struct timezone *)r4;

    TRACESYSCALL(("(I) %s[%d]: tv=0x%016lx, tz=0x%016lx\n", __func__, ProcessorID(), (unsigned long)tv, (unsigned long)tz));

    // Check for error conditions.
    if (tv && !VMM_IsAppAddress(tv, sizeof(*tv)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    if (tz && !VMM_IsAppAddress(tz, sizeof(*tz)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    int rc = gettimeofday(tv, tz);                                                                                     // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: tv_sec=0x%016lx, tz_usec=0x%016lx\n",                                                   // FUSEDOS
                 __func__, ProcessorID(), tv->tv_sec, tv->tv_usec));                                                   // FUSEDOS
    if (rc == 0) return CNK_RC_SUCCESS(0);                                                                             // FUSEDOS
    else return CNK_RC_SUCCESS(errno);                                                                                 // FUSEDOS
#else                                                                                                                  // FUSEDOS
    // The clock is initialized to the current time when the job starts.
    if (tv)
    {
        uint64_t now = GetCurrentTimeInMicroseconds();
        tv->tv_sec  = now / MILLION;
        tv->tv_usec = now % MILLION;
    }
    if (tz)
    {
        tz->tz_minuteswest = 0; // minutes west of Greenwich
        tz->tz_dsttime     = 0; // type of daylight savings time correction
    }

    return CNK_RC_SUCCESS(0);
#endif                                                                                                                 // FUSEDOS
}
