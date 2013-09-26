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
#include <sys/utsname.h>
#include "Kernel.h"

//! \brief  Implement the uname system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(descriptor number) or CNK_RC_FAILURE(errno).

uint64_t sc_uname(SYSCALL_FCN_ARGS)
{
    struct utsname *name = (struct utsname *)r3;

    TRACESYSCALL(("(I) %s%s: name=0x%p\n", __func__, whoami(), name));

    // Check for error conditions.
    if (!VMM_IsAppAddress(name, sizeof(*name)))
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    // Copy the fields into the caller's storage.
    strncpy(name->sysname,  "CNK", sizeof(name->sysname));
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    strncpy(name->nodename, NodeState.NodeName, sizeof(name->nodename));
    strncpy(name->release, NodeState.Release, sizeof(name->release));
#else                                                                                                                  // FUSEDOS
	struct utsname linux_uname;
	if( uname(&linux_uname) )
			return CNK_RC_FAILURE(EFAULT);
	strncpy(name->nodename, linux_uname.nodename, sizeof(name->nodename));
	strncpy(name->release,  linux_uname.release,  sizeof(name->release));
#endif                                                                                                                 // FUSEDOS
    strncpy(name->version, "1", sizeof(name->version));
    strncpy(name->machine, "BGQ", sizeof(name->machine));
    
    return CNK_RC_SUCCESS(0);
}
