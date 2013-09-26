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
#include <poll.h>
#include "Kernel.h"
#include "fs/virtFS.h"

//! \brief  Run the poll system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_poll(SYSCALL_FCN_ARGS)
{
   struct pollfd *fds = (struct pollfd *)r3;
   nfds_t nfds = (nfds_t)r4;
   int timeout = r5;

   TRACESYSCALL(("(I) %s%s: nfds=%lu, timeout=%d\n", __func__, whoami(), nfds, timeout ));

   // Check for error conditions.
   if ( nfds > (uint32_t)GetMyProcess()->App_Descriptors.maxfds ) {
      return CNK_RC_FAILURE(EINVAL);
   }

   if ( !VMM_IsAppAddress(fds, nfds * sizeof(struct pollfd)) ) {
      return CNK_RC_FAILURE(EFAULT);
   }

   // Check the descriptors in the array of structures.
   for (nfds_t index = 0; index < nfds; ++index) {
      // Make sure the descriptor is valid.
      if (!File_IsFDAllocated(fds[index].fd)) {
         return CNK_RC_FAILURE(EBADF);
      }

      // Only files and sockets can be used with poll().
      int type = File_GetFDType( fds[index].fd );
      if ( (type == FD_FILE) || (type == FD_SOCKET) ){ 
        continue;
      }
      else{
         return CNK_RC_FAILURE(EINVAL);
      }
   }

   timeout=0; //override the user--no hanging in poll
   uint64_t rc = File_GetFSPtr(fds[0].fd)->poll(fds,  nfds, timeout);
   
   return rc;
}
