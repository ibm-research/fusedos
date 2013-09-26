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

#define SOCKETCALL_SOCKET            1
#define SOCKETCALL_BIND              2
#define SOCKETCALL_CONNECT           3
#define SOCKETCALL_LISTEN            4
#define SOCKETCALL_ACCEPT            5
#define SOCKETCALL_GETSOCKNAME       6
#define SOCKETCALL_GETPEERNAME       7
#define SOCKETCALL_SOCKETPAIR        8
#define SOCKETCALL_SEND              9
#define SOCKETCALL_RECV             10
#define SOCKETCALL_SENDTO           11
#define SOCKETCALL_RECVFROM         12
#define SOCKETCALL_SHUTDOWN         13
#define SOCKETCALL_SETSOCKOPT       14
#define SOCKETCALL_GETSOCKOPT       15
#define SOCKETCALL_SENDMSG          16
#define SOCKETCALL_RECVMSG          17
#define SOCKETCALL_ACCEPT4          18

//! \brief  Implement the socketcall system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return Return value from specific socket function.

uint64_t sc_socketcall(SYSCALL_FCN_ARGS)
{
   int operation = r3;
   uint64_t *args = (uint64_t *)r4;

   int sockfd = (int)args[0];
#if defined __FUSEDOS__                                                                                               // FUSEDOS
   TRACESYSCALL(("(I) %s%s: operation=%d sockfd=%d\n", __func__, whoami(), operation, sockfd));                        // FUSEDOS
#else                                                                                                                  // FUSEDOS
   TRACESYSCALL(("(I) %s%s: operation=%ld sockfd=%d\n", __func__, whoami(), operation, sockfd));
#endif                                                                                                                 // FUSEDOS

   if (operation != SOCKETCALL_SOCKET)
   {
      // Make sure the descriptor is valid.
      if (!File_IsFDAllocated(sockfd))
      {
          return CNK_RC_FAILURE(EBADF);
      }

      // Make sure the descriptor is a socket.
      if (File_GetFDType(sockfd) == FD_RDMA){
        switch(operation)
        {
          case SOCKETCALL_RECV:
          case SOCKETCALL_SEND:
          break;
          default: return CNK_RC_FAILURE(ENOTSOCK);
        }
      }
      else if (File_GetFDType(sockfd) != FD_SOCKET)
      {
          return CNK_RC_FAILURE(ENOTSOCK);
      }
   }

   uint64_t rc = 0;
   switch (operation)
   {
       case SOCKETCALL_ACCEPT:
       {
           struct sockaddr *addr = (struct sockaddr *)args[1];
           socklen_t *addrlen = (socklen_t *)args[2];

           // Make sure the address length is valid.
           if (!VMM_IsAppAddress(addrlen, sizeof(socklen_t)))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, *addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->accept(sockfd, addr, addrlen);
           break;
       }

       case SOCKETCALL_BIND:
       {
           const struct sockaddr *addr = (const struct sockaddr *)args[1];
           socklen_t addrlen = (socklen_t)args[2];

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->bind(sockfd, addr, addrlen);
           break;
       }

       case SOCKETCALL_CONNECT:
       {
           const struct sockaddr *addr = (const struct sockaddr *)args[1];
           socklen_t addrlen = (socklen_t)args[2];

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->connect(sockfd, addr, addrlen);
           break;
       }

       case SOCKETCALL_GETPEERNAME:
       {
           struct sockaddr *addr = (struct sockaddr *)args[1];
           socklen_t *addrlen = (socklen_t *)args[2];

           // Make sure the address length is valid.
           if (!VMM_IsAppAddress(addrlen, sizeof(socklen_t)))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, *addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->getpeername(sockfd, addr, addrlen);
           break;
       }

       case SOCKETCALL_GETSOCKNAME:
       {
           struct sockaddr *addr = (struct sockaddr *)args[1];
           socklen_t *addrlen = (socklen_t *)args[2];

           // Make sure the address length pointer is valid.
           if (!VMM_IsAppAddress(addrlen, sizeof(socklen_t)))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, *addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->getsockname(sockfd, addr, addrlen);
           break;
       }

       case SOCKETCALL_GETSOCKOPT:
       {
           int level = (int)args[1];
           int optname = (int)args[2];
           void *optval = (void *)args[3];
           socklen_t *optlen = (socklen_t *)args[4];

           // Make sure the option length is valid.
           if (!VMM_IsAppAddress(optlen, sizeof(socklen_t)))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the option value is valid.
           if (!VMM_IsAppAddress(optval, *optlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->getsockopt(sockfd, level, optname, optval, optlen);
           break;
       }

       case SOCKETCALL_LISTEN:
       {
           int backlog = (int)args[1];
           rc = File_GetFSPtr(sockfd)->listen(sockfd, backlog);
           break;
       }

       case SOCKETCALL_RECV:
       {
           void *buffer = (void *)args[1];
           size_t length = (size_t)args[2];
           int flags = (int)args[3];

           // Make sure buffer is valid.
           if (!VMM_IsAppAddress(buffer, length))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->recv(sockfd, buffer, length, flags);
           break;
       }

       case SOCKETCALL_RECVFROM:
       {
           void *buffer = (void *)args[1];
           size_t length = (size_t)args[2];
           int flags = (int)args[3];
           struct sockaddr *addr = (struct sockaddr *)args[4];
           socklen_t *addrlen = (socklen_t *)args[5];

           // Make sure buffer is valid.
           if (!VMM_IsAppAddress(buffer, length))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address length is valid.
           if (!VMM_IsAppAddress(addrlen, sizeof(socklen_t)))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, *addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->recvfrom(sockfd, buffer, length, flags, addr, addrlen);
           break;
       }

       case SOCKETCALL_SEND:
       {
           const void *buffer = (const void *)args[1];
           size_t length = (size_t)args[2];
           int flags = (int)args[3];

           // Make sure buffer pointer is valid.
           if (!VMM_IsAppAddress(buffer, length))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->send(sockfd, buffer, length, flags);
           break;
       }

       case SOCKETCALL_SENDTO:
       {
           const void *buffer = (void *)args[1];
           size_t length = (size_t)args[2];
           int flags = (int)args[3];
           const struct sockaddr *addr = (struct sockaddr *)args[4];
           socklen_t addrlen = (socklen_t)args[5];

           // Make sure buffer is valid.
           if (!VMM_IsAppAddress(buffer, length))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           // Make sure the address is valid.
           if (!VMM_IsAppAddress(addr, addrlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->sendto(sockfd, buffer, length, flags, addr, addrlen);
           break;
       }

       case SOCKETCALL_SETSOCKOPT:
       {
           int level = (int)args[1];
           int optname = (int)args[2];
           const void *optval = (const void *)args[3];
           socklen_t optlen = (socklen_t)args[4];

           // Make sure the option value is valid.
           if (!VMM_IsAppAddress(optval, optlen))
           {
               return CNK_RC_FAILURE(EFAULT);
           }

           rc = File_GetFSPtr(sockfd)->setsockopt(sockfd, level, optname, optval, optlen);
           break;
       }

       case SOCKETCALL_SHUTDOWN:
       {
           int how = (int)args[1];
           rc = File_GetFSPtr(sockfd)->shutdown(sockfd, how);
           break;
       }

       case SOCKETCALL_SOCKET:
       {
           rc = File_GetFSPtrFromType(FD_SOCKET)->socket((int)args[0], (int)args[1], (int)args[2]);
           break;
       }

       default:
            rc = CNK_RC_FAILURE(EINVAL);
            break;
   }

   return rc;
}

