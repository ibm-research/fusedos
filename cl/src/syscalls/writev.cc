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
#include <limits.h>
#include <sys/uio.h>

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#define IOV_MAX 1024                                                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

//! \brief  Run the writev system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(number of bytes written) or CNK_RC_FAILURE(errno).

uint64_t sc_writev(SYSCALL_FCN_ARGS)
{
    int fd = r3;
    const struct iovec *iov    = (const struct iovec *)r4;
    int iovcnt = r5;

    TRACESYSCALL(("(I) %s%s: fd=%d, iov=0x%p, iovcnt=%d\n", __func__, whoami(), fd, iov, iovcnt));

    // Make sure the descriptor is valid.
    if (!File_IsFDAllocated(fd)) {
       return CNK_RC_FAILURE(EBADF);
    }

    // Check for error conditions.
    if ( (iovcnt < 0) || (iovcnt > IOV_MAX) ) {
        return CNK_RC_FAILURE(EINVAL);
    }

    // Just return if the vector count is zero.
    if (iovcnt == 0) {
        return CNK_RC_SUCCESS(0);
    }

    // Make sure the vector is valid.
    if ( !VMM_IsAppAddress( iov, (sizeof(struct iovec) * iovcnt)) ) {
        return CNK_RC_FAILURE(EFAULT);
    }

    // Make sure the total length of all the io vectors will not overflow the return value.
    size_t totalLen = 0;
    for (int index = 0; index < iovcnt; ++index) {
        if (iov[index].iov_len > INT_MAX) {
            return CNK_RC_FAILURE(EINVAL);
        }
        totalLen += iov[index].iov_len;
        if (totalLen > INT_MAX) {
            return CNK_RC_FAILURE(EINVAL);
        }
    }

    // Make sure the buffers in the io vector are valid.
    for (int index = 0; index < iovcnt; ++index) 
    {
        if((iov[index].iov_base == NULL) && (iov[index].iov_len == 0))
            continue;
        
        if ( !VMM_IsAppAddress( iov[index].iov_base, iov[index].iov_len ) ) {
            return CNK_RC_FAILURE(EFAULT);
        }
    }

    // Run file system's writev() method.
    return File_GetFSPtr(fd)->writev(fd, iov, iovcnt);
}
