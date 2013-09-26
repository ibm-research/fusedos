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

#ifndef _CNK_INTERNALSYSCALLS_H
#define _CNK_INTERNALSYSCALLS_H

uint64_t internal_close(int fd);
uint64_t internal_fstat(int fd, struct stat* buf);
uint64_t internal_lseek(int fildes, off_t offset, int whence);
uint64_t internal_open(const char *path, int oflag, mode_t mode);
uint64_t internal_pread64(int fd, void* buffer, size_t length, off_t position);
uint64_t internal_read(int fd, void* buffer, size_t length);
uint64_t internal_readlink(const char* pathname, char* buffer, size_t length);
uint64_t internal_write(int fd, const void* buffer, size_t length);

#endif // _CNK_INTERNALSYSCALLS_H

