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

//! \brief  Run the chdir system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_chdir(SYSCALL_FCN_ARGS)
{
   char *pathname = (char *)r3;

   // Check for error conditions.
   uint64_t pathlen = validatePathname(pathname);
   if (CNK_RC_IS_FAILURE(pathlen)) {
      return pathlen;
   }

   // Check for a path that is too long.
   if (pathlen > APP_MAX_PATHNAME) {
      return CNK_RC_FAILURE(ENAMETOOLONG);
   }

   if (pathname[0] != '/') {
      if ((strlen(GetMyProcess()->CurrentDir) + pathlen + 2) > APP_MAX_PATHNAME) {
         return CNK_RC_FAILURE(ENAMETOOLONG);
      }
   }

   TRACESYSCALL(("(I) %s%s: path=\"%s\"\n", __func__, whoami(), pathname));

   // Run file system's chdir() method.
   return File_GetFSPtrFromPath(pathname)->chdir(pathname);
}

