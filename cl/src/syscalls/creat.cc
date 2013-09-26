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
#include <fcntl.h>
#include "fs/virtFS.h"

//! \brief  Run the creat system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(descriptor number) or CNK_RC_FAILURE(errno).

uint64_t sc_creat(SYSCALL_FCN_ARGS)
{
   char *pathname = (char *)r3;
   mode_t mode = r4;

   // Check for error conditions.
   uint64_t pathlen = validatePathname(pathname);
   if (CNK_RC_IS_FAILURE(pathlen)) {
      return pathlen;
   }

   TRACESYSCALL(("(I) %s%s: pathname=\"%s\", mode=0x%08x\n", __func__, whoami(), pathname, mode));

   // Run file system's open() method with pre-defined oflags.
   int oflags = O_WRONLY|O_CREAT|O_TRUNC;
#if defined __FUSEDOS__                                                                                            // FUSEDOS
	virtFS * vfs = File_GetFSPtrFromPath(pathname);                                                             // FUSEDOS
	TRACESYSCALL(("(I) %s%s: got FS ptr %p\n",  __func__, whoami(), vfs));                                      // FUSEDOS
	uint64_t ret = vfs->open(pathname, oflags, mode);    

        TRACESYSCALL(("(I) %s%s: called creat, return code %lu\n", __func__, whoami(), ret));

	return ret;                                                                                                 // FUSEDOS
#else                                                                                                               // FUSEDOS
    return File_GetFSPtrFromPath(pathname)->open(pathname, oflags, mode);
#endif                                                                                                              // FUSEDOS
}

