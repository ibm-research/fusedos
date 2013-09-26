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
#include <limits.h>

//! \brief  Run the getcwd system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(number of bytes returned) or CNK_RC_FAILURE(errno).

uint64_t sc_getcwd(SYSCALL_FCN_ARGS)
{
   char *buf = (char *)r3;
   size_t len = r4;

   TRACESYSCALL(("(I) %s%s: buf=0x%p, len=%lu\n", __func__, whoami(), buf, len));

   // Check for error conditions.
   if (!VMM_IsAppAddress(buf, len)) {
      return CNK_RC_FAILURE(EFAULT);
   }

   //! \todo Still need resolve what lock to use when accessing CurrentDir field.
   AppProcess_t *process = GetMyProcess();
   size_t dirlen = strlen(process->CurrentDir) + 1;

   // Make sure the working directory fits in the caller's buffer.
   if (dirlen > len) {
      return CNK_RC_FAILURE(ERANGE);
   }

   memcpy(buf, process->CurrentDir, dirlen);
   return CNK_RC_SUCCESS(dirlen);
}
