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

//! \brief  Function ship the open system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(descriptor number) or CNK_RC_FAILURE(errno).

uint64_t sc_open(SYSCALL_FCN_ARGS)
{
    char *pathname = (char *)r3;
    int oflags = r4;
    mode_t mode = r5;

    // Check for error conditions.
    uint64_t pathlen = validatePathname(pathname);
    if (CNK_RC_IS_FAILURE(pathlen))
    {
	return pathlen;
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s%s: pathname=\"%s\", flags=0x%08x mode=\0x%08xn", __func__, whoami(), pathname, oflags,mode));
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s%s: pathname=\"%s\", flags=0x%08x mode=0x%08xn", __func__, whoami(), pathname, oflags,mode));
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    //! \todo Nobody remembers why we have this code but don't want to break something that is working ...
    if ( !Personality_CiosEnabled() )
    {
        if ( strncmp(pathname, "/dev/tty", strlen("/dev/tty")) == 0 )
        {
            return CNK_RC_SUCCESS(1); // Descriptor 1 is already allocated to stdout.
        }
    }
#endif                                                                                                                 // FUSEDOS

    // Run the file system's open() method.
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    virtFS* vfs = File_GetFSPtrFromPath(pathname);                                                                     // FUSEDOS
    TRACESYSCALL(("(I) %s%s: got FS ptr %p\n",  __func__, whoami(), vfs));                                             // FUSEDOS
    uint64_t ret = vfs->open(pathname, oflags, mode);                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s%s: called open, return code %lu\n", __func__, whoami(), ret));                               // FUSEDOS
    return ret;                                                                                                        // FUSEDOS
#else                                                                                                                  // FUSEDOS
    return File_GetFSPtrFromPath(pathname)->open(pathname, oflags, mode);
#endif                                                                                                                 // FUSEDOS
}

uint64_t internal_open(const char *path, int oflag, mode_t mode)
{
    uint64_t rc;
    BEGIN_KERNELINTERNALSC();
    rc = sc_open((uint64_t)path, oflag, mode, 0, 0, 0);
    END_KERNELINTERNALSC();
    return rc;
}
