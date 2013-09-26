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
#include <sys/resource.h>

uint64_t sc_ugetrlimit(SYSCALL_FCN_ARGS)
{
    int resource = r3;
    struct rlimit *rlp = (struct rlimit *)r4;
    AppProcess_t *app = GetMyProcess();
    CNK_Descriptors_t *pFD = &(app->App_Descriptors);
    
    TRACESYSCALL(("(I) %s[%d]: resource=%d, rlp=0x%016lx\n", __func__, ProcessorID(), resource, (unsigned long)rlp));

    // Check for error conditions.
    if ( (resource < 0) || (resource >= RLIMIT_NLIMITS) )
    {
        return CNK_RC_FAILURE(EINVAL);
    }

    if ( (rlp == NULL) || (!VMM_IsAppAddress(rlp, sizeof(*rlp))) )
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    // For now we do not support setting resource limits so we just set the values directly in the
    // caller's rlimit structure.
    switch (resource)
    {
        case RLIMIT_NPROC:
            rlp->rlim_cur = rlp->rlim_max = 1;
            break;

        case RLIMIT_NOFILE:
            rlp->rlim_cur = rlp->rlim_max = CWD_FILENO; // CWD filenumber is the last file descriptor
            break;

        case RLIMIT_SIGPENDING:
        case RLIMIT_MSGQUEUE:
            rlp->rlim_cur = rlp->rlim_max = 0;
            break;
        case RLIMIT_NICE:
        case RLIMIT_RTPRIO:
        default:
            rlp->rlim_cur = rlp->rlim_max = RLIM_INFINITY;
            break;
    }

    return CNK_RC_SUCCESS(0);
}

