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

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <unistd.h>                                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

uint64_t  sc_setuid( SYSCALL_FCN_ARGS)
{
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    uid_t uid = (size_t)r3;                                                                                            // FUSEDOS
                                                                                                                       // FUSEDOS
    TRACESYSCALL(("(I) %s%s: uid_t=%d\n", __func__, whoami(), uid));                                                   // FUSEDOS
                                                                                                                       // FUSEDOS
    int rc = setuid(uid);                                                                                              // FUSEDOS
    if (rc == -1) {                                                                                                    // FUSEDOS
        return CNK_RC_FAILURE(errno);                                                                                  // FUSEDOS
    } else {                                                                                                           // FUSEDOS
        return CNK_RC_SUCCESS(0);                                                                                      // FUSEDOS
    }                                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Failing with EPERM=%d.\n", __func__, ProcessorID(), EPERM ));

    return CNK_RC_FAILURE(EPERM);
#endif                                                                                                                 // FUSEDOS
}


uint64_t  sc_getuid( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: returning %d.\n",
                  __func__, ProcessorID(), GetMyAppState()->UserID ));

    return CNK_RC_SUCCESS(GetMyAppState()->UserID);
}


uint64_t  sc_setgid( SYSCALL_FCN_ARGS)
{
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    gid_t gid = (size_t)r3;                                                                                            // FUSEDOS
                                                                                                                       // FUSEDOS
    TRACESYSCALL(("(I) %s%s: gid_t=%d\n", __func__, whoami(), gid));                                                   // FUSEDOS
                                                                                                                       // FUSEDOS
    int rc = setgid(gid);                                                                                              // FUSEDOS
    if (rc == -1) {                                                                                                    // FUSEDOS
        return CNK_RC_FAILURE(errno);                                                                                  // FUSEDOS
    } else {                                                                                                           // FUSEDOS
        return CNK_RC_SUCCESS(0);                                                                                      // FUSEDOS
    }                                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
  TRACESYSCALL(("(I) %s[%d]: Failing with EPERM=%d.\n", __func__, ProcessorID(), EPERM ));

    return CNK_RC_FAILURE(EPERM);
#endif                                                                                                                 // FUSEDOS
}


uint64_t  sc_getgid( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: returning %d.\n",
                  __func__, ProcessorID(), GetMyAppState()->GroupID ));

    return CNK_RC_SUCCESS(GetMyAppState()->GroupID);
}


uint64_t  sc_geteuid( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: returning %d.\n",
                  __func__, ProcessorID(), GetMyAppState()->UserID ));

    return CNK_RC_SUCCESS(GetMyAppState()->UserID);
}


uint64_t  sc_getegid( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: returning %d.\n",
                  __func__, ProcessorID(), GetMyAppState()->GroupID ));

    return CNK_RC_SUCCESS(GetMyAppState()->GroupID);
}

