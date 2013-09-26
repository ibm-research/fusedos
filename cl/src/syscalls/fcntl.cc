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
/* (C) Copyright IBM Corp.  2010, 2012                              */
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
#include <fcntl.h>

//! \brief  Run the fcntl system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(descriptor number) or CNK_RC_FAILURE(errno).

uint64_t sc_fcntl(SYSCALL_FCN_ARGS)
{
   int fd = r3;
   int cmd = r4;
   uint64_t parm3 = r5;

   TRACESYSCALL(("(I) %s%s: fd=%d, cmd=%d, parm3=0x%lx\n", __func__, whoami(), fd, cmd, parm3));

   // Make sure the descriptor is valid.
   if (!File_IsFDAllocated(fd)) {
      return CNK_RC_FAILURE(EBADF);
   }

   // Validate optional third parameter based on command.
   switch (cmd) {
      case F_GETFL:
      case F_SETFL:                                                                                                    // FUSEDOS
      case F_GETFD:
      case F_SETFD:
         break;

      case F_DUPFD:
      {
         int newfd = (int)parm3;
         if ( (newfd < 0) || (newfd >= GetMyProcess()->App_Descriptors.maxfds) ) {
            return CNK_RC_FAILURE(EINVAL);   // NOTE: different error than dup2
         }
         break;
      }

      case F_GETLK:
      case F_SETLK:
      case F_SETLKW:
      {
         struct flock *lock = (struct flock *)parm3;
         if (!VMM_IsAppAddress(lock, sizeof(*lock))) {
            return CNK_RC_FAILURE(EFAULT);
         }
         break;
      }

      case F_GETLK64:
      case F_SETLK64:
      case F_SETLKW64:
      {
         struct flock64 *lock64 = (struct flock64 *)parm3;
         if (!VMM_IsAppAddress(lock64, sizeof(*lock64))) {
            return CNK_RC_FAILURE(EFAULT);
         }
         break;
      }

      default:
         return CNK_RC_FAILURE(EINVAL);
         break;
   }

   // Run file system's fcntl() method.
   return File_GetFSPtr(fd)->fcntl(fd, cmd, parm3);
}
