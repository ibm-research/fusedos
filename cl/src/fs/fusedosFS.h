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
#ifndef _CNK_FUSEDOSFS_H_ // Prevent multiple inclusion
#define _CNK_FUSEDOSFS_H_

#include <sys/ioctl.h>
#include <unistd.h>

#include "virtFS.h"

class fusedosFS : public virtFS {
  public:
    fusedosFS(void) : virtFS() {};
    int init(void) {
      return 0;
    }
    int term(void) {
      return 0;
    }
    int setupJob(void) {
      return 0;
    }
    int cleanupJob(void) {
      return 0;
    }
    bool isMatch(const char* path) {
      TRACE( TRACE_VirtFS, ("(I) path %s\n", path) );
      return true;
    }
    uint64_t accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs accept, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs accept, remote_fd %d\n", remote_fd));
      int fd = ::accept(remote_fd, addr, addrlen);
      if (fd == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs accept failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        int localFD = File_GetFD(0);
        if (localFD != -1) {
          File_SetFD(localFD, fd, FD_SOCKET);
          return CNK_RC_SUCCESS(localFD);
	} else {
          // Assume no free local descriptor
          ::close(fd);
          return CNK_RC_FAILURE(EMFILE);
	}
      }
    }
    uint64_t access(const char* path, int mode) {
      TRACE( TRACE_VirtFS, ("(E) access method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs bind, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs bind, remote_fd %d\n", remote_fd));
      ssize_t rc = ::bind(remote_fd, addr, addrlen);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs bind failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t chdir(const char* path) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs chdir, path %s\n", path));
      if (::chdir(path) != 0) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs chdir failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    }
    uint64_t chmod(const char* path, mode_t mode) {
      TRACE( TRACE_VirtFS, ("(E) chmod method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t chown(const char* path, uid_t uid, gid_t gid) {
      TRACE( TRACE_VirtFS, ("(E) chown method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t close(int fd) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs close, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs close, remote_fd %d\n", remote_fd));
      if (::close(remote_fd) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs close failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        File_FreeFD(fd);
        return CNK_RC_SUCCESS(0);
      }
    } 
    uint64_t connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs connect, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs connect, remote_fd %d\n", remote_fd));
      ssize_t rc = ::connect(remote_fd, addr, addrlen);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs connect failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t creat(const char* path, mode_t mode) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs creat, path %s\n", path));
      int fd = ::creat(path, mode);
      int localFD = File_AllocateFD(fd, FD_FILE);
      if (localFD != -1) {
          return CNK_RC_SUCCESS(localFD);
      } else {
          // Assume no free local descriptor
          ::close(fd);
          return CNK_RC_FAILURE(EMFILE);
      }
    }           
    uint64_t fadvise(int fd, off_t offset, off_t len, int advice) {
      TRACE( TRACE_VirtFS, ("(E) fadvise method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fchmod(int fd, mode_t mode) {
      TRACE( TRACE_VirtFS, ("(E) fchmod method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fchown(int fd, uid_t uid, gid_t gid) {
      TRACE( TRACE_VirtFS, ("(E) fchown method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fcntl(int fd, int cmd, uint64_t parm3) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fcntl, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fcntl, remote_fd %d\n", remote_fd));
      ssize_t rc = ::fcntl(remote_fd, cmd, parm3);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs fcntl failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t flock(int fd, int operation) {
      TRACE( TRACE_VirtFS, ("(E) flock method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fstat(int fd, struct stat* statbuf) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fstat, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fstat, remote_fd %d\n", remote_fd));
      if (::fstat(remote_fd, statbuf) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs fstat failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    }
    uint64_t fstat64(int fd, struct stat64* statbuf) {
      TRACE( TRACE_VirtFS, ("(E) fstat64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fstatfs(int fd, struct statfs* statbuf) {
      TRACE( TRACE_VirtFS, ("(E) fstatfs method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fstatfs64(int fd, struct statfs64* statbuf) {
      TRACE( TRACE_VirtFS, ("(E) fstatfs64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t fsync(int fd) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fsync, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs fsync, remote_fd %d\n", remote_fd));
      if (::fsync(remote_fd) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs fsync failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    } 
    uint64_t ftruncate(int fd, off_t offset) {
      TRACE( TRACE_VirtFS, ("(E) ftruncate method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t ftruncate64(int fd, off_t offset) {
      TRACE( TRACE_VirtFS, ("(E) ftruncate64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getcwd(const char* path, size_t len) {
      TRACE( TRACE_VirtFS, ("(E) getcwd method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getdents(int fd, struct dirent* buf, unsigned int len) {
      TRACE( TRACE_VirtFS, ("(E) getdents method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getdents64(int fd, struct dirent* buf, unsigned int len) {
      TRACE( TRACE_VirtFS, ("(E) getdents64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getpeername(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
      TRACE( TRACE_VirtFS, ("(E) getpeername method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getsockname(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
      TRACE( TRACE_VirtFS, ("(E) getsockname method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getsockopt(int sockfd, int level, int optname, void* optval, socklen_t* optlen) {
      TRACE( TRACE_VirtFS, ("(E) getsockopt method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t ioctl(int fd, unsigned long int cmd, void* parm3) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs ioctl, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs ioctl, remote_fd %d\n", remote_fd));
      if (::ioctl(remote_fd, cmd, parm3) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs ioctl failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    }
    uint64_t lchown(const char* path, uid_t uid, gid_t gid) {
      TRACE( TRACE_VirtFS, ("(E) lchown method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t link(const char* path, const char* link) {
      TRACE( TRACE_VirtFS, ("(E) link method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t listen(int sockfd, int backlog) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs listen, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs listen, remote_fd %d\n", remote_fd));
      if (::listen(remote_fd, backlog) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs listen failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    }
    uint64_t llseek(int fd, off64_t offset, off64_t* result, int whence) {
      TRACE( TRACE_VirtFS, ("(E) llseek method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t lseek(int fd, off_t offset, int whence) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs lseek, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs lseek, remote_fd %d\n", remote_fd));
      ssize_t rc = ::lseek(remote_fd, offset, whence);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs lseek failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t lstat(const char* __restrict__ path, struct stat* __restrict__ statbuf) {
      TRACE( TRACE_VirtFS, ("(E) lstat method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t lstat64(const char* path, struct stat64* statbuf) {
      TRACE( TRACE_VirtFS, ("(E) lstat64 method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t mkdir(const char* path, mode_t mode) {
      TRACE( TRACE_VirtFS, ("(E) mkdir method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t mmap(int fd, uint64_t length, uint64_t offset) {
      TRACE( TRACE_VirtFS, ("(E) mmap method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t open(const char* path, int oflags, mode_t mode) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs open, path %s\n", path));
      int fd = ::open(path, oflags, mode);
      if (fd == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs open failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        int localFD = File_AllocateFD(fd, FD_FILE);
        if (localFD != -1) {
          return CNK_RC_SUCCESS(localFD);
	} else {
          // Assume no free local descriptor
          ::close(fd);
          return CNK_RC_FAILURE(EMFILE);
	}
      }
    }
    uint64_t poll(struct pollfd* fds, nfds_t nfds, int timeout) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs poll, fds[0].fd %d, fds[0].events 0x%x, fds[0].revents 0x%x, nfds %lu\n",
                            fds[0].fd, fds[0].events, fds[0].revents, nfds));
#define MAX_POLL_FDS 256
      if (nfds > MAX_POLL_FDS) return CNK_RC_FAILURE(EINVAL);
      struct pollfd remote_fds[MAX_POLL_FDS];
      for (nfds_t i = 0; i < nfds; i++) {
          remote_fds[i].fd = File_GetRemoteFD(fds[i].fd);
          remote_fds[i].events = fds[i].events;
          remote_fds[i].revents = fds[i].revents;
      }
      TRACE(TRACE_VirtFS, ("(I) fusedos fs poll, remote_fds[0].fd %d\n", remote_fds[0].fd));
      int rc = ::poll(&remote_fds[0], nfds, -1);
      for (nfds_t i = 0; i < nfds; i++) {
          fds[i].revents = remote_fds[i].revents;
      }
      TRACE(TRACE_VirtFS, ("(I) fusedos fs poll, remote_fds[0].revents 0x%x\n", remote_fds[0].revents));
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs poll failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t pread64(int fd, void* buffer, size_t length, off64_t position) {
      TRACE( TRACE_VirtFS, ("(E) pread64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t pwrite64(int fd, const void* buffer, size_t length, off64_t position) {
      TRACE( TRACE_VirtFS, ("(E) pwrite64 method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t read(int fd, void* buffer, size_t cnt) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs read, fd %d\n", fd));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs read, remote_fd %d\n", remote_fd));
      ssize_t rc = ::read(remote_fd, buffer, cnt);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs read failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t readlink(const char* path, char* buf, size_t bufsiz) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs readlink, path %s\n", path));
      ssize_t rc = ::readlink(path, buf, bufsiz);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs readlink, rc %lu\n", rc));
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs readlink failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t readv(int fd, const struct iovec* iov, int iovcnt) {
      TRACE( TRACE_VirtFS, ("(E) readv method not supported by fusedos file system for descriptor %d\n", fd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t recv(int sockfd, void* buffer, size_t length, int flags) {
      TRACE( TRACE_VirtFS, ("(E) recv method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t recvfrom(int sockfd, void* buffer, size_t length, int flags, struct sockaddr* addr, socklen_t* addrlen) {
      TRACE( TRACE_VirtFS, ("(E) recvfrom method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t rename(const char* oldpath, const char* newpath) {
      TRACE( TRACE_VirtFS, ("(E) rename method not supported by fusedos file system for path %s\n", oldpath) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t rmdir(const char* path) {
      TRACE( TRACE_VirtFS, ("(E) rmdir method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t send(int sockfd, const void* buffer, size_t length, int flags) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs send, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs send, remote_fd %d\n", remote_fd));
      ssize_t rc = ::send(remote_fd, buffer, length, flags);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs send failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t sendto(int sockfd, const void* buffer, size_t length, int flags, const struct sockaddr* addr, socklen_t addrlen) {
      TRACE( TRACE_VirtFS, ("(E) send method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t setsockopt(int sockfd, int level, int optname, const void* optval, socklen_t optlen) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs setsockopt, sockfd %d\n", sockfd));
      int remote_fd = File_GetRemoteFD(sockfd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs setsockopt, remote_fd %d\n", remote_fd));
      ssize_t rc = ::setsockopt(remote_fd, level, optname, optval, optlen);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs setsockopt failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t shutdown(int sockfd, int how) {
      TRACE( TRACE_VirtFS, ("(E) shutdown method not supported by fusedos file system for descriptor %d\n", sockfd) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t socket(int domain, int type, int protocol) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs socket, %d %d %d\n", domain, type, protocol));
      int fd = ::socket(domain, type, protocol);
      if (fd == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs socket failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        int localFD = File_GetFD(0);
        if (localFD != -1) {
          File_SetFD(localFD, fd, FD_SOCKET);
          return CNK_RC_SUCCESS(localFD);
	} else {
          // Assume no free local descriptor
          ::close(fd);
          return CNK_RC_FAILURE(EMFILE);
	}
      }
    }
    uint64_t stat(const char* __restrict__ path, struct stat* __restrict__ statbuf) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs stat, path %s\n", path));
      if (::stat(path, statbuf) == -1) {
        TRACE(TRACE_VirtFS, ("(I) fusedos fs stat failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(0);
      }
    }
    uint64_t stat64(const char* __restrict__ path, struct stat64* __restrict__statbuf) {
      TRACE( TRACE_VirtFS, ("(E) stat64 method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t statfs(const char* __restrict__ path, struct statfs* __restrict__ statbuf) {
      TRACE( TRACE_VirtFS, ("(E) statfs method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t statfs64(const char* __restrict__ path, struct statfs64* __restrict__statbuf) {
      TRACE( TRACE_VirtFS, ("(E) statfs64 method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t symlink(const char* path, const char* link) {
      TRACE( TRACE_VirtFS, ("(E) symlink method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t truncate(const char* path, off_t len) {
      TRACE( TRACE_VirtFS, ("(E) truncate method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t truncate64(const char* path, off64_t len) {
      TRACE( TRACE_VirtFS, ("(E) truncate64 method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t utime(const char* path, const struct utimbuf* utimes) {
      TRACE( TRACE_VirtFS, ("(E) utime method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t unlink(const char* path) {
      TRACE( TRACE_VirtFS, ("(E) unlink method not supported by fusedos file system for path %s\n", path) );
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t write(int fd, const void* buffer, size_t cnt) {
      int remoteFD = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs write, fd %d (%d), buffer %*s\n", fd, remoteFD, (int)cnt, (char*)buffer));
      ssize_t rc = ::write(remoteFD, buffer, cnt);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs write failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t writev(int fd, const struct iovec* iov, int iovcnt) {
      TRACE(TRACE_VirtFS, ("(I) fusedos fs writev, fd %d, iovcnt %d\n", fd, iovcnt));
      int remote_fd = File_GetRemoteFD(fd);
      TRACE(TRACE_VirtFS, ("(I) fusedos fs writev, remote_fd %d\n", remote_fd));
      ssize_t rc = ::writev(remote_fd, iov, iovcnt);
      if (rc == -1) {
        TRACE(TRACE_VirtFS, ("(E) fusedos fs writev failed, errno %d\n", errno));
        return CNK_RC_FAILURE(errno);
      } else {
        return CNK_RC_SUCCESS(rc);
      }
    }
    uint64_t getFilenames(uint32_t& numfiles, char filename[][128]) {
      return CNK_RC_FAILURE(ENOSYS);
    }
    uint64_t getFileContents(const char* pathname, size_t offset, size_t& numbytes, char* data) {
      return CNK_RC_FAILURE(ENOSYS);
    }
    int clearAllocations(void) {
      return CNK_RC_FAILURE(ENOSYS);
    }
};

#endif
