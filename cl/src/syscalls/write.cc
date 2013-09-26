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
#include <errno.h>
#include "fs/virtFS.h"

//! \brief  Run the write system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(number of bytes written) or CNK_RC_FAILURE(errno).

uint64_t sc_write(SYSCALL_FCN_ARGS)
{
    int fd = r3;
    char *buf = (char *)r4;
    size_t cnt = r5;

    TRACESYSCALL(("(I) %s%s: fd=%d, buf=0x%p, cnt=%ld\n", __func__, whoami(), fd, buf, cnt));

    // Make sure the descriptor is valid.
    if (!File_IsFDAllocated(fd)) {
       return CNK_RC_FAILURE(EBADF);
    }

    // Check for valid address.
    if ( !VMM_IsAppAddress(buf, cnt) ) {
        return CNK_RC_FAILURE(EFAULT);
    }

    // Run file system's write() method.
    return File_GetFSPtr(fd)->write(fd, buf, cnt);
}

uint64_t internal_write(int fd, const void* buffer, size_t cnt)
{
   uint64_t rc;
   BEGIN_KERNELINTERNALSC();
   rc = sc_write(fd, (uint64_t)buffer, cnt, 0, 0, 0);
   END_KERNELINTERNALSC();
   return rc;
}

