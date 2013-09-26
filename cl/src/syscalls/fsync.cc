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
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

//! \brief  Run the fsync system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_fsync(SYSCALL_FCN_ARGS)
{
   int fd = r3;

   TRACESYSCALL(("(I) %s%s: fd=%d\n", __func__, whoami(), fd));

   // Make sure the descriptor is valid.
   if (!File_IsFDAllocated(fd)) {
      return CNK_RC_FAILURE(EBADF);
   }

   // Run file system's fsync() method.
   return File_GetFSPtr(fd)->fsync(fd);
}

uint64_t internal_fsync(int fd)
{
    uint64_t rc;
    BEGIN_KERNELINTERNALSC();
    rc = sc_sync(fd, 0, 0, 0, 0, 0);
    END_KERNELINTERNALSC();
    return rc;
}
