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

// Includes
#include "Kernel.h"
#include "fs/virtFS.h"

//! \brief  Run the pread64 system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(number of bytes read) or CNK_RC_FAILURE(errno)..

uint64_t sc_pread64(SYSCALL_FCN_ARGS)
{
   int fd = r3;
   void *buf = (void *)r4;
   size_t cnt = r5;
   off64_t position = (off64_t)(r6);

   TRACESYSCALL(("(I) %s%s: fd=%d, buf=0x%p, cnt=%lu, position=%ld\n", __func__, whoami(), fd, buf, cnt, position));

   // Make sure the descriptor is valid.
   if (!File_IsFDAllocated(fd)) {
      return CNK_RC_FAILURE(EBADF);
   }

   // A read of 0 bytes is a special case, just return.
   if (cnt == 0) {
      return CNK_RC_SUCCESS(0);
   }

   // Check for error conditions.
   if (!VMM_IsAppAddress(buf, cnt)) {
      return CNK_RC_FAILURE(EFAULT);
   }

#if TODO
   if (type == FD_STDIN) {
      return CNK_RC_FAILURE(ESPIPE);
   }
#endif

   // Run file system's pread64() method.
   return File_GetFSPtr(fd)->pread64(fd, buf, cnt, position);
}

uint64_t internal_pread64(int fd, void *buffer, size_t cnt, off64_t position)
{
   uint64_t rc;
   BEGIN_KERNELINTERNALSC();
   rc = sc_pread64(fd, (uint64_t)buffer, cnt, position, 0, 0);
   END_KERNELINTERNALSC();
   return rc;
}

