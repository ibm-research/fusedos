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

uint64_t sc_setrlimit(SYSCALL_FCN_ARGS)
{
    int resource = r3;
    struct rlimit *rlp = (struct rlimit *)r4;

    TRACESYSCALL(("(I) %s[%d]: resource=%d, rlp=0x%016lx\n", __func__, ProcessorID(), resource, (unsigned long)rlp));

    // Check for error conditions.
    if (resource < 0 || resource >= RLIMIT_NLIMITS)
    {
        return CNK_RC_FAILURE(EINVAL);
    }

    if (rlp == NULL || !VMM_IsAppAddress(rlp, sizeof(*rlp)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    // For now we do not support setting resource limits.
    return CNK_RC_FAILURE(EINVAL);

    // We only support one resource limit for now.
    if (resource != RLIMIT_CORE)
    {
        return CNK_RC_FAILURE(EINVAL);
    }
    
    // Make sure the soft limit will not be greater than the hard limit.
    AppProcess_t* proc = GetMyProcess();
    rlim_t rlim_cur = ((rlp->rlim_cur == RLIM_SAVED_CUR) ? proc->resourceLimits[resource].rlim_cur : rlp->rlim_cur);
    rlim_t rlim_max = ((rlp->rlim_max == RLIM_SAVED_MAX) ? proc->resourceLimits[resource].rlim_max : rlp->rlim_max);
    if (rlim_cur > rlim_max)
    {
        return CNK_RC_FAILURE(EINVAL);
    }
    
    // Set the new resource limit values.  Note we don't restrict raising the hard limit.
    proc->resourceLimits[resource].rlim_cur = rlim_cur;
    proc->resourceLimits[resource].rlim_max = rlim_max;

    return CNK_RC_SUCCESS(0);
}
