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
#include <sys/stat.h>
#include <unistd.h>

//! \brief  Run the stat system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SUCCESS(0) or CNK_RC_FAILURE(errno).

uint64_t sc_stat(SYSCALL_FCN_ARGS)
{
   char *pathname = (char *)r3;
   struct stat *buf= (struct stat *)r4;
   
   // Check for error conditions.
   uint64_t pathlen = validatePathname(pathname);
   if (CNK_RC_IS_FAILURE(pathlen))
   {
      return pathlen;
   }
   
   TRACESYSCALL(("(I) %s%s: path=\"%s\", buf=0x%p\n", __func__, whoami(), pathname, buf));
   
   if (!VMM_IsAppAddress(buf, sizeof(struct stat)))
   {
      return CNK_RC_FAILURE(EFAULT);
   }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   // Special case when I/O node is not available.
   if (!Personality_CiosEnabled())
   {
      if ( strncmp( pathname, "/dev/null", strlen("/dev/null")) == 0 )
      {
         // fake /dev/null with inode value no valid file will have
         memset(buf, 0, sizeof(struct stat));
         buf->st_blksize = 512;
         buf->st_mode = S_IRWXU | S_IRWXG | S_IRWXO;
         buf->st_blocks = 1;
         buf->st_ino = 9999;
         buf->st_dev = 9999;
         return CNK_RC_SUCCESS(0);
      }
   }
#endif                                                                                                                 // FUSEDOS
   
   // Run file system's stat() method.
   return File_GetFSPtrFromPath(pathname)->stat(pathname,buf);
}
