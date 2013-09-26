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
#include <errno.h>
#include <sys/ioctl.h>

//! \brief  Run the ioctl system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return >=CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno) (value depends on cmd).

uint64_t sc_ioctl(SYSCALL_FCN_ARGS)
{
    int fd = r3;
    unsigned long int cmd = r4;
    int *arg = NULL;
    struct termios *termios = NULL;
    void *parm3 = NULL;

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACESYSCALL(("(I) %s%s: fd=%d, cmd=%lu\n", __func__, whoami(), fd, cmd));                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s%s: fd=%d, cmd=%d\n", __func__, whoami(), fd, cmd));
#endif                                                                                                                 // FUSEDOS

    // Make sure the descriptor is valid.
    if (!File_IsFDAllocated(fd))
    {
        return CNK_RC_FAILURE(EBADF);
    }

    switch (cmd)
    {
        case FIONBIO:
            arg = (int *)r5;
            if (!VMM_IsAppAddress(arg, sizeof(*arg)))
            {
                return CNK_RC_FAILURE(EFAULT);
            }
            parm3 = arg;
            break;

        case FIONREAD:
            arg = (int *)r5;
            if (!VMM_IsAppAddress(arg, sizeof(*arg)))
            {
                return CNK_RC_FAILURE(EFAULT);
            }
            parm3 = arg;
            break;

        case TCGETA:
        case TCGETS:
        case 0x402c7413: // Special value used by isatty()
            termios = (struct termios *)r5;
            if (!VMM_IsAppAddress(termios, sizeof(*termios)))
            {
                return CNK_RC_FAILURE(EFAULT);
            }
            parm3 = termios;
            break;
        
        case FIOBGQATOMIC:
            parm3 = (void*)r5;
            if(!VMM_IsAppAddress(parm3, sizeof(uint64_t)))
            {
                return CNK_RC_FAILURE(EFAULT);
            }
            break;
            
        default:
            return CNK_RC_FAILURE(EINVAL);
            break;
    }

    // Run file system's ioctl() method.
    return File_GetFSPtr(fd)->ioctl(fd, cmd, parm3);
}

