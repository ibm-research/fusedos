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


#ifndef _SHMFS_H_ // Prevent multiple inclusion.
#define _SHMFS_H_

// Includes
#include "virtFS.h"

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern Lock_Atomic_t ShareLock;
extern ShmMgrEntry_t SharedPool[ SHM_MAX_OPENS ];
#endif                                                                                                                 // FUSEDOS

//  shm class - This class supports both the shared memory class and the 
//              persistent memory class. Implementation is in shmFS.cc
// Please keep functions in alphabetical order for ease of maintenance

class shmFS : public virtFS
{
public:

   //! \brief  Default constructor.

   shmFS(void) : virtFS() { };


   //! \brief  Cleanup after running a job.
   //! \param  fs File system type.
   //! \return 0 when successful, errno when unsuccessful.

   int cleanupJob(int fs);

   int clearAllocations(void);

   uint64_t access(const char *path, int mode);
   
   uint64_t chdir(const char *path);
   uint64_t close(int fd);
   uint64_t chmod(const char *path, mode_t mode);
   uint64_t chown(const char *path, uid_t uid, gid_t gid);
   
   uint64_t fadvise(int fd, off_t offset, off_t len, int advice);
   uint64_t fcntl(int fd, int cmd, uint64_t parm3);
   uint64_t fchmod(int fd, mode_t mode);
   uint64_t fchown(int fd, uid_t uid, gid_t gid);
   uint64_t flock(int fd, int operation);
   uint64_t fstat(int fd, struct stat *statbuf);
   uint64_t fstat64(int fd, struct stat64 *statbuf);
   uint64_t fstatfs(int fd, struct statfs *buf);
   uint64_t fstatfs64(int fd, struct statfs64 *buf);
   uint64_t ftruncate(int fd, off_t offset);
   uint64_t ftruncate64(int fd, off_t offset);
   uint64_t ioctl(int, long unsigned int, void*);
   uint64_t lchown(const char *path, uid_t uid, gid_t gid);
   uint64_t lseek(int fd, off_t offset, int whence);
   uint64_t mkdir(const char *path, mode_t mode);
   uint64_t mmap(int fd, uint64_t length, uint64_t offset);
   uint64_t open(const char *pathname, int oflags, mode_t mode);
   uint64_t read(int fd, void* buff, size_t cnt);
   uint64_t readv(int fd, const struct iovec *iov, int iovcnt);
   uint64_t rmdir(const char *path);
   uint64_t stat(const char *pathname, struct stat *statbuf);
   uint64_t stat64(const char *pathname, struct stat64 *statbuf);
   uint64_t statfs(const char *pathname, struct statfs *statbuf);
   uint64_t statfs64(const char *pathname, struct statfs64 *statbuf);
   uint64_t unlink(const char *pathname);
   uint64_t write(int fd, const void* buff, size_t cnt);
   uint64_t writev(int fd, const struct iovec *iov, int iovcnt);
   uint64_t utime(const char *path, const struct utimbuf *utimes);
   uint64_t getFilenames(uint32_t& numfiles, char filenames[][128]);
   uint64_t getFileContents(const char* pathname, size_t offset, size_t& numbytes, char* data);
   
protected:
   
   static const blksize_t BlockSize = CONFIG_MEM_PAGE_SIZE;
   int _FD_type;
   
   int nextAddress(const ShmMgrEntry_t* ptr, ShmMgrEntry_t*& next);
   int previousAddress(uint64_t va, ShmMgrEntry_t*& next);
   int defineMemorySpace(ShmMgrEntry_t* ptr, size_t space);
   bool compactMemorySpace();
   
   uint64_t findByName(const char *pathname, ShmMgrEntry_t*& ptr);
   uint64_t findByDescriptor(int fd, ShmMgrEntry_t*& ptr);
   void     fillStatStructure(struct stat64 *statbuf, ShmMgrEntry_t* ptr);
   virtual ShmMgr_t* getShmManager() { return NULL; };
   virtual uint64_t  getDeviceID()   { return 0; };
};

#endif
