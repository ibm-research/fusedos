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
#include "fs/virtFS.h"

//! \brief  Run the mkdir system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_mkdir(SYSCALL_FCN_ARGS)
{
   char *pathname = (char *)r3;
   mode_t mode = (mode_t)r4;

   // Check for error conditions.
   uint64_t pathlen = validatePathname(pathname);
   if (CNK_RC_IS_FAILURE(pathlen)) {
      return pathlen;
   }

   TRACESYSCALL(("(I) %s%s: path=\"%s\", mode=0x%08x\n", __func__, whoami(), pathname, mode));

   // Run file system's mkdir() method.
   return File_GetFSPtrFromPath(pathname)->mkdir(pathname, mode);
}

