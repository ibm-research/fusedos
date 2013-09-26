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
/* (C) Copyright IBM Corp.  2009, 2012                              */
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

#ifndef _CNK_VIRTFS_H_ // Prevent multiple inclusion
#define _CNK_VIRTFS_H_


// Includes
#include <inttypes.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/socket.h>
#include <utime.h>
#include <stdio.h>
#include <poll.h>
#include "Trace.h"

// get file system pointer prototypes - functions in filesetup.cc
class virtFS;    // forward reference for virtFS pointers in prototypes
extern virtFS*  File_GetFSPtr( int local_fd );
extern virtFS*  File_GetFSPtrFromPath( const char* pathname);
extern virtFS*  File_GetFSPtrFromType( int type );

// List of pointers to file system objects.
extern virtFS *virtFSPtr[];

// virtual base class - each function should be defined here
// Please keep functions in alphabetical order for ease of maintenance
//
class virtFS {

   public:
      virtFS(void) {};

      virtual int init(void)
      {
          return 0;
      }

      virtual int term(void)
      {
          return 0;
      }

      virtual int setupJob(int fs)
      {
          return 0;
      }

      virtual int cleanupJob(int fs)
      {
          return 0;
      }

      virtual bool isMatch(const char *path)
      {
          return false;
      }

      virtual uint64_t accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) accept method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t access(const char *path, int mode)
      {
         TRACE( TRACE_VirtFS, ("(E) access method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) bind method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t chdir(const char *path)
      {
         TRACE( TRACE_VirtFS, ("(E) chdir method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t chmod(const char *path, mode_t mode)
      {
         TRACE( TRACE_VirtFS, ("(E) chmod method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t chown(const char *path, uid_t uid, gid_t gid)
      {
         TRACE( TRACE_VirtFS, ("(E) chown method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t close(int fd)
      {
         TRACE( TRACE_VirtFS, ("(E) close method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) connect method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fadvise(int fd, off_t offset, off_t len, int advice)
      {
         TRACE( TRACE_VirtFS, ("(E) fadvise method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }
      
      virtual uint64_t fchmod(int fd, mode_t mode)
      {
         TRACE( TRACE_VirtFS, ("(E) fchmod method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fchown(int fd, uid_t uid, gid_t gid)
      {
         TRACE( TRACE_VirtFS, ("(E) fchown method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fcntl(int fd, int cmd, uint64_t parm3)
      {
         TRACE( TRACE_VirtFS, ("(E) fcntl method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }
      
      virtual uint64_t flock(int fd, int operation)
      {
         TRACE( TRACE_VirtFS, ("(E) flock method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }
      
      virtual uint64_t fstat(int fd, struct stat *statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) fstat method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fstat64(int fd, struct stat64 *statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) fstat64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fstatfs(int fd, struct statfs *statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) fstatfs method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fstatfs64(int fd, struct statfs64 *statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) fstatfs64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t fsync(int fd)
      {
         TRACE( TRACE_VirtFS, ("(E) fsync method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t ftruncate(int fd, off_t offset)
      {
         TRACE( TRACE_VirtFS, ("(E) ftruncate method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t ftruncate64(int fd, off_t offset)
      {
         TRACE( TRACE_VirtFS, ("(E) ftruncate64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getcwd(const char *path, size_t len)
      {
         TRACE( TRACE_VirtFS, ("(E) getcwd method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getdents(int fd, struct dirent *buf, unsigned int len)
      {
         TRACE( TRACE_VirtFS, ("(E) getdents method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getdents64(int fd, struct dirent *buf, unsigned int len)
      {
         TRACE( TRACE_VirtFS, ("(E) getdents64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) getpeername method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) getsockname method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
      {
         TRACE( TRACE_VirtFS, ("(E) getsockopt method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t gpfsfcntl(int fd, const void* ptr, size_t length, int* gpfsresult)
      {
         TRACE( TRACE_VirtFS, ("(E) gpfsfcntl method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t ioctl(int fd, unsigned long int cmd, void *parm3)
      {
         TRACE( TRACE_VirtFS, ("(E) ioctl method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t lchown(const char *path, uid_t uid, gid_t gid)
      {
         TRACE( TRACE_VirtFS, ("(E) lchown method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t link(const char *path, const char *link)
      {
         TRACE( TRACE_VirtFS, ("(E) link method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t listen(int sockfd, int backlog)
      {
         TRACE( TRACE_VirtFS, ("(E) listen method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t llseek(int fd, off64_t offset, off64_t *result, int whence)
      {
         TRACE( TRACE_VirtFS, ("(E) llseek method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t lseek(int fd, off_t offset, int whence)
      {
         TRACE( TRACE_VirtFS, ("(E) lseek method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t lstat(const char *__restrict__ path, struct stat  *__restrict__ statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) lstat method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t lstat64(const char *path, struct stat64 *statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) lstat64 method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t mkdir(const char *path, mode_t mode)
      {
         TRACE( TRACE_VirtFS, ("(E) mkdir method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t mmap(int fd, uint64_t length, uint64_t offset)
      {
         TRACE( TRACE_VirtFS, ("(E) mmap method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t open(const char *path, int oflags, mode_t mode)
      {
         TRACE( TRACE_VirtFS, ("(E) open method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t poll(struct pollfd *fds, nfds_t nfds, int timeout)
      {
         TRACE( TRACE_VirtFS, ("(E) poll method not supported by file system\n") );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t pread64(int fd, void *buffer, size_t length, off64_t position)
      {
         TRACE( TRACE_VirtFS, ("(E) pread64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t pwrite64(int fd, const void *buffer, size_t length, off64_t position)
      {
         TRACE( TRACE_VirtFS, ("(E) pwrite64 method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t read(int fd, void *buffer, size_t cnt)
      {
         TRACE( TRACE_VirtFS, ("(E) read method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t readlink(const char *path, void *buf, size_t bufsiz)
      {
         TRACE( TRACE_VirtFS, ("(E) readlink method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t readv(int fd, const struct iovec *iov, int iovcnt)
      {
         TRACE( TRACE_VirtFS, ("(E) readv method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t recv(int sockfd, void *buffer, size_t length, int flags)
      {
         TRACE( TRACE_VirtFS, ("(E) recv method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *addr, socklen_t *addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) recvfrom method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t rename(const char *oldpath, const char *newpath)
      {
         TRACE( TRACE_VirtFS, ("(E) rename method not supported by file system for path %s\n", oldpath) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t rmdir(const char *path)
      {
         TRACE( TRACE_VirtFS, ("(E) rmdir method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t send(int sockfd, const void *buffer, size_t length, int flags)
      {
         TRACE( TRACE_VirtFS, ("(E) send method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t sendto(int sockfd, const void *buffer, size_t length, int flags, const struct sockaddr *addr, socklen_t addrlen)
      {
         TRACE( TRACE_VirtFS, ("(E) send method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
      {
         TRACE( TRACE_VirtFS, ("(E) setsockopt method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t shutdown(int sockfd, int how)
      {
         TRACE( TRACE_VirtFS, ("(E) shutdown method not supported by file system for descriptor %d\n", sockfd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t socket(int domain, int type, int protocol)
      {
         TRACE( TRACE_VirtFS, ("(E) socket method not supported by file system\n") );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t stat(const char *__restrict__ path, struct stat  *__restrict__ statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) stat method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t stat64(const char *__restrict__ path, struct stat64 *__restrict__statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) stat64 method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t statfs(const char *__restrict__ path, struct statfs  *__restrict__ statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) statfs method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t statfs64(const char *__restrict__ path, struct statfs64 *__restrict__statbuf)
      {
         TRACE( TRACE_VirtFS, ("(E) statfs64 method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t symlink(const char *path, const char *link)
      {
         TRACE( TRACE_VirtFS, ("(E) symlink method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t truncate(const char* path, off_t len)
      {
         TRACE( TRACE_VirtFS, ("(E) truncate method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t truncate64(const char *path, off64_t len)
      {
         TRACE( TRACE_VirtFS, ("(E) truncate64 method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t utime(const char *path, const struct utimbuf *utimes)
      {
         TRACE( TRACE_VirtFS, ("(E) utime method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t unlink(const char* path)
      {
         TRACE( TRACE_VirtFS, ("(E) unlink method not supported by file system for path %s\n", path) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t write(int fd, const void *buffer, size_t cnt)
      {
         TRACE( TRACE_VirtFS, ("(E) write method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t writev(int fd, const struct iovec *iov, int iovcnt)
      {
         TRACE( TRACE_VirtFS, ("(E) writev method not supported by file system for descriptor %d\n", fd) );
         return CNK_RC_FAILURE(ENOSYS);
      }
      
      virtual uint64_t getFilenames(uint32_t& numfiles, char filename[][128])
      {
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getFileContents(const char* pathname, size_t offset, size_t& numbytes, char* data)
      {
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual int clearAllocations(void)
      {
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual int sendx(char * mInput)
      {  
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t deregisterMemory(int fd, void * usingRegion4RDMA){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t registerMemory(int fd, void * usingRegion4RDMA){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t setxattr(const char * path, const char *name, const void *value, size_t size, int flags){
         TRACE( TRACE_VirtFS, ("(E) setxattr method not supported by file system for path %s\n", path) );
         printf("setxattr method not supported by file system for path %s\n", path);
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t lsetxattr(const char * path, const char *name, const void *value, size_t size, int flags){
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t fsetxattr(int fd, const char *name, const void *value, size_t size, int flags){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t getxattr(const char * path, const char *name, void *value, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t lgetxattr(const char * path, const char *name, void *value, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t fgetxattr(int fd, const char *name, void *value, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t removexattr(const char * path, const char *name){
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t lremovexattr(const char * path, const char *name){
         return CNK_RC_FAILURE(ENOSYS);
      }
      virtual uint64_t fremovexattr(int fd, const char *name){
         return CNK_RC_FAILURE(ENOSYS);
      }
      

      virtual uint64_t listxattr(const char * path, char *list, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t llistxattr(const char * path, char *list, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }

      virtual uint64_t flistxattr(int fd, char *list, size_t size){
         return CNK_RC_FAILURE(ENOSYS);
      }


};

#endif

