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

#include "Kernel.h"

__BEGIN_DECLS

uint64_t sc_GETRANK(SYSCALL_FCN_ARGS)
{
    AppProcess_t *proc = GetMyProcess();	
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Returning %ld\n", __func__, ProcessorID(), proc->Rank));
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Returning %d\n", __func__, ProcessorID(), proc->Rank));                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    return CNK_RC_SUCCESS(proc->Rank);
}


__END_DECLS
