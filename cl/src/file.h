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

#ifndef _CNK_FILE_H_ // Prevent multiple inclusion
#define _CNK_FILE_H_

#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include "Kernel.h"

// Linux defaults to 1024 but currently we limit to 256 in CNK
#define CNK_BITS_PER_LONG (8*sizeof(unsigned long))

// Descriptor types
enum { 
    FD_ERROR = 0,       // Must be first and index 0
    FD_STDIN,
    FD_STDOUT,
    FD_STDERR,
    FD_SHARED_MEM,
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    FD_PERSIST_MEM,
    FD_PROC_MEM,
#endif                                                                                                                 // FUSEDOS
    FD_MEMFILE,
    FD_CONSOLE,
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    FD_LOCAL,
#endif                                                                                                                 // FUSEDOS
    FD_SOCKET,
    FD_RDMA,
    FD_FILE,            // Must be last as "default" file system
    FD_TOTAL_FILESYS
};

// Pre-defined descriptor number for current working directory descriptor.
#define CWD_FILENO (pFD->maxfds - 1)

typedef struct CNK_Descriptor_Info
{
    void *FileSysPtr;    // use void because we can't use C++ class in C code
    int Remote_FD;
    int Type;
    off_t CurrOffset;     // current offset into file for this descriptor
} CNK_Descriptor_Info_t;

typedef struct CNK_Descriptors_t
{
    int                    maxfds;
    unsigned long*         cnk_fd_bits;
    CNK_Descriptor_Info_t* cnk_local_fd;
} CNK_Descriptors_t;

//! \brief  Setup process-scoped file system resources at beginning of job.
//! \note   This function must be called from a process leader hardware thread.
//! \return Nothing.

int File_ProcessSetup(void);

//! \brief  Cleanup process-scoped file system resources at end of job.
//! \note   This function must be called from a process leader hardware thread.
//! \return Nothing.

void File_ProcessCleanup(void);

//! \brief  Check if a descriptor is allocated.
//! \param  fd File descriptor.
//! \return True when descriptor is allocated, false when descriptor is free.

extern int File_IsFDAllocated(int fd);

//! \brief  Allocate a new file descriptor and set its attributes.
//! \param  remote_fd Remote file descriptor number.
//! \param  type Type of descriptor.
//! \return New file descriptor when successful, -1 when no descriptors available.

extern int File_AllocateFD(int remote_fd, int type);

//! \brief  Get a new file descriptor from the descriptor table.
//! \param  offset Offset in descriptor table to start searching from.
//! \return New file descriptor when successful, -1 when no descriptors available.

extern int File_GetFD(int offset);

//! \brief  Set the attributes for an allocated descriptor.
//! \param  fd File descriptor.
//! \param  remote_fd Remote file descriptor.
//! \param  type Type of file descriptor (from descriptor type list).
//! \return File descriptor when successful, -1 when file descriptor is invalid.

extern int File_SetFD(int fd, int remote_fd, int type);

//! \brief  Free an allocated file descriptor.
//! \param  fd File descriptor.
//! \return 0 when successful, -1 when file descriptor is invalid.

extern int File_FreeFD(int fd);

//! \brief  Get the remote file descriptor for an allocated file descriptor.
//! \param  fd File descriptor.
//! \return Remote file descriptor when successful, -1 when file descriptor is invalid.

extern int File_GetRemoteFD(int fd);

//! \brief  Get the type of an allocated file descriptor.
//! \param  fd File descriptor.
//! \return Descriptor type when successful, -1 when file descriptor is invalid.

extern int File_GetFDType(int fd);

//! \brief  Get the current file offset of an allocated file descriptor.
//! \note   The file offset is only maintained for local file systems.
//! \param  fd File descriptor.
//! \return Current offset when successful, -1 when file descriptor is invalid.

extern long File_GetCurrentOffset( int fd );

//! \brief  Set the current file offset of an allocated file descriptor.
//! \note   The file offset is only maintained for local file systems.
//! \param  fd File descriptor.
//! \param  offset New value for file offset.
//! \return 0 when successful, -1 when file descriptor is invalid.

extern int File_SetCurrentOffset( int fd, long offset);

//! \brief  Get the file descriptor of the current working directory for the process.
//! \return File descriptor.

extern int File_GetCurrentDirFD( void );

//! \brief  Get the file system pointer of the current working directory for the process.
//! \return Pointer to file system object.

extern void *File_GetCurrentDirFSPtr( void );

//! \brief  Print system call trace output to standard error.
//! \param  fmt Format string.
//! \param  ... Variable arguments for substitution.
//! \return Length of output string.

extern int printstrc(const char *fmt, ...);

#endif // Add nothing below this line
