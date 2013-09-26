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

// Includes
#include "Kernel.h"
#include "util.h"
#include "virtFS.h"
#include "sharedmemFS.h"

#include <sys/stat.h>
#include <fcntl.h>


// todo:         munmap

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
Lock_Atomic_t ShareLock;
ShmMgrEntry_t SharedPool[ SHM_MAX_OPENS ];
#else                                                                                                                  // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS
extern pthread_mutex_t * shm_lock;                                                                                     // FUSEDOS
extern ShmMgrEntry_t * SharedPool;                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#define SHMTOINDEX(entry) ((((uint64_t)entry) - ((uint64_t)&SharedPool[0])) / sizeof(ShmMgrEntry_t))
#define touchAccess(e) e->lastAccess = GetTimeBase()
#define touchMod(e)    e->lastMod    = GetTimeBase()

//
//  shm class - support functions for shared and persistent memory 
//
// Please keep functions in alphabetical order for ease of maintenance

int shmFS::nextAddress(const ShmMgrEntry_t* ptr, ShmMgrEntry_t*& next)
{
    uint64_t maxva = ~0ULL;
    int x;
    next = NULL;
    for(x=0; x<SHM_MAX_OPENS; x++)
    {
        if((SharedPool[x].owner == getShmManager()) &&
           (SharedPool[x].AllocatedAddr > ptr->AllocatedAddr) &&
           (SharedPool[x].AllocatedAddr != 0) &&
           (maxva >= SharedPool[x].AllocatedAddr))
        {
            maxva = SharedPool[x].AllocatedAddr;
            next = &SharedPool[x];
        }
    }
    return 0;
}
int shmFS::previousAddress(uint64_t address, ShmMgrEntry_t*& previous)
{
    uint64_t minva = 0;
    int x;
    previous = NULL;
    for(x=0; x<SHM_MAX_OPENS; x++)
    {
        if((SharedPool[x].owner == getShmManager()) &&
           (SharedPool[x].AllocatedAddr != 0) &&
           (SharedPool[x].AllocatedAddr < address) &&
           (SharedPool[x].AllocatedAddr > minva))
        {
            minva = SharedPool[x].AllocatedAddr;
            previous = &SharedPool[x];
        }
    }
    return 0;
}

void* memmove_up(void* vdst, const void* vsrc, size_t bytes)
{
    Kernel_WriteFlightLog(FLIGHTLOG, FL_MEMMOVEUP, (uint64_t)vsrc, (uint64_t)vdst, bytes, 0);
    
    char* dst = (char*)vdst;
    char* src = (char*)vsrc;
    while(bytes--)
    {
        dst[bytes] = src[bytes];
    }
    return 0;
}

int shmFS::defineMemorySpace(ShmMgrEntry_t* ptr, size_t space)
{
    uint64_t newVaddr;
    ShmMgrEntry_t* next;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t oldaddr = ptr->AllocatedAddr;
    uint64_t oldsize = ptr->AllocatedSize;
#endif                                                                                                                 // FUSEDOS
    
    if(ptr->AllocatedAddr != 0)
    {
        // Shrink or same size...  always works.
        if(ptr->AllocatedSize >= space)
        {
            ptr->AllocatedSize = space;
            Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
            return 0;
        }
    }
    
    nextAddress(ptr, next);
    if(next != NULL)
    {
        // Check to see if we can expand the current file to start of next file.
        if(ptr->AllocatedAddr != 0)
        {
            if(next->AllocatedAddr >= space + ptr->AllocatedAddr) 
            {
                memset((void*)(ptr->AllocatedAddr + ptr->AllocatedSize), 0, space-ptr->AllocatedSize);
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
        else
        {
            if(next->AllocatedAddr >= space + ROUND_UP_MEMPAGE(getShmManager()->VStart))
            {
                newVaddr = ROUND_UP_MEMPAGE(getShmManager()->VStart);
                memset((void*)newVaddr, 0, space);
                if(ptr->AllocatedAddr != 0)
                    memmove_up((void*)newVaddr, (void*)ptr->AllocatedAddr, ptr->AllocatedSize);
                ptr->AllocatedAddr = newVaddr;
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
    }
    else
    {
        // Start address was not NULL, end address was NULL, no files to end of region.
        if(ptr->AllocatedAddr != 0)
        {
            // Check to see if we can expand the current file to the end of the region
            if(getShmManager()->VStart + getShmManager()->Size >= space + ptr->AllocatedAddr)
            {
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
        else
        {
            // Start address was NULL and end address was NULL, no files defined.
            if(getShmManager()->VStart + getShmManager()->Size >= space + ROUND_UP_MEMPAGE(getShmManager()->VStart) )
            {
                newVaddr = ROUND_UP_MEMPAGE(getShmManager()->VStart);
                ptr->AllocatedAddr = newVaddr;
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
    }
    
    if(ptr->pinned)
    {
        compactMemorySpace();
    }
    int x;
    bool ownerFound = false;
    next = NULL;
    for(x=0; x<SHM_MAX_OPENS; x++)
    {
        if((SharedPool[x].owner != getShmManager()) || (SharedPool[x].AllocatedAddr == 0))
            continue;
        ownerFound = true;
        nextAddress(&SharedPool[x], next);
        if(next != NULL)
        {
            if(next->AllocatedAddr >= space + ROUND_UP_MEMPAGE(SharedPool[x].AllocatedAddr + SharedPool[x].AllocatedSize))
            {
                newVaddr = ROUND_UP_MEMPAGE(SharedPool[x].AllocatedAddr + SharedPool[x].AllocatedSize);
                memset((void*)newVaddr, 0, space);
                if(ptr->AllocatedAddr != 0)
                    memmove_up((void*)newVaddr, (void*)ptr->AllocatedAddr, ptr->AllocatedSize);
                ptr->AllocatedAddr = newVaddr;
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
        else
        {
            if(getShmManager()->VStart + getShmManager()->Size >= space + ROUND_UP_MEMPAGE(SharedPool[x].AllocatedAddr + SharedPool[x].AllocatedSize))
            {
                newVaddr = ROUND_UP_MEMPAGE(SharedPool[x].AllocatedAddr + SharedPool[x].AllocatedSize);
                memset((void*)newVaddr, 0, space);
                if(ptr->AllocatedAddr != 0)
                    memmove_up((void*)newVaddr, (void*)ptr->AllocatedAddr, ptr->AllocatedSize);
                ptr->AllocatedAddr = newVaddr;
                ptr->AllocatedSize = space;
                Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
                return 0;
            }
        }
    }
    if(!ownerFound)
    {
        if(getShmManager()->VStart + getShmManager()->Size >= ROUND_UP_MEMPAGE(getShmManager()->VStart) + space)
        {
            newVaddr = ROUND_UP_MEMPAGE(getShmManager()->VStart);
            memset((void*)newVaddr, 0, space);
            if(ptr->AllocatedAddr != 0)
                memmove_up((void*)newVaddr, (void*)ptr->AllocatedAddr, ptr->AllocatedSize);
            ptr->AllocatedAddr = newVaddr;
            ptr->AllocatedSize = space;
            Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMDEFSPC, oldaddr, oldsize, ptr->AllocatedAddr, ptr->AllocatedSize);
            return 0;
        }
    }
    
    if(compactMemorySpace())
    {
        return defineMemorySpace(ptr, space);
    }
    return ENOSPC;
}

bool shmFS::compactMemorySpace()
{
    size_t bytesMoved = 0;
    uint64_t maxva = getShmManager()->VStart + getShmManager()->Size;
    ShmMgrEntry_t* previous;
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMCMPSPB, 0,0,0,0);
    
    while(1)
    {
        previousAddress(maxva, previous);
        if(previous == NULL)
            break;
        if(previous->pinned)
            break;
        if(previous->AllocatedAddr != maxva - previous->AllocatedSize)
        {
            memmove_up((void*)(maxva - previous->AllocatedSize), (void*)(previous->AllocatedAddr), previous->AllocatedSize);
            bytesMoved += maxva - previous->AllocatedSize - previous->AllocatedAddr;
            maxva = previous->AllocatedAddr = maxva - previous->AllocatedSize;
        }
        else
        {
            maxva = previous->AllocatedAddr;
        }
    }
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SHMCMPSPF, bytesMoved, maxva - getShmManager()->VStart,0,0);
    if(bytesMoved > 0)
        return true;
    return false;
}

uint64_t shmFS::access(const char *pathname, int mode)
{
    ShmMgrEntry_t* entry = NULL;
    uint64_t rc = findByName(pathname, entry);
    if ( CNK_RC_IS_SUCCESS(rc) )
    {
        touchAccess(entry);
    }
    return rc;
}

uint64_t shmFS::chdir(const char *path)
{
   uint64_t rc;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Allow a "change" to the same directory.
   if (strncmp(path, ".", MIN(APP_MAX_PATHNAME, sizeof("."))) == 0) {
      rc = CNK_RC_SUCCESS(0);
   }
   
   // Allow a change to the root directory of file system.
   else if (strncmp(path, DEVSHM_ROOT, MIN(APP_MAX_PATHNAME, sizeof(DEVSHM_ROOT))) == 0) {
      File_SetFD(CWD_FILENO, -1, _FD_type);
      strncpy(GetMyProcess()->CurrentDir, path, sizeof(GetMyProcess()->CurrentDir));
      rc = CNK_RC_SUCCESS(0);
   }
   
   // Otherwise there is no directory support for the file system.
   else {
      rc = CNK_RC_FAILURE(ENOSYS);
   }
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::chdir%s: path=%s rc=%s\n", whoami(), path, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::chmod(const char *pathname, mode_t mode)
{
    ShmMgrEntry_t* entry = NULL;
    uint64_t rc = findByName(pathname, entry);
    if ( CNK_RC_IS_SUCCESS(rc) )
    {
        touchAccess(entry);
        touchMod(entry);
    }
    return rc;
}

uint64_t shmFS::chown(const char *pathname, uid_t uid, gid_t gid)
{
    ShmMgrEntry_t* entry = NULL;
    uint64_t rc = findByName(pathname, entry);
    if ( CNK_RC_IS_SUCCESS(rc) )
    {
        touchAccess(entry);
        touchMod(entry);
    }
    return rc;
}

uint64_t shmFS::close(int fd)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    touchAccess(entry);
    entry->LinkCount--;
    File_FreeFD(fd);
    
    if((entry->LinkCount > 0x80000000)&&((entry->LinkCount & 0xffff) == 0))  // file was marked for deletion
    {
        entry->owner = NULL;
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    
    return CNK_RC_SUCCESS(0);
}

uint64_t shmFS::fadvise(int fd, off_t offset, off_t len, int advice)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}

uint64_t shmFS::fchmod(int fd, mode_t mode)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}

uint64_t shmFS::fchown(int fd, uid_t uid, gid_t gid)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}


uint64_t shmFS::fcntl(int fd, int cmd, uint64_t parm3)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   ShmMgrEntry_t* entry;
   uint64_t rc = findByDescriptor(fd, entry);
   switch (cmd)
   {
      // Set the return to the same value as the flag passed in
      case F_GETFD:
         rc = CNK_RC_SUCCESS(entry->Oflags);
         break;
         
      // Indicate success to caller
      case F_SETFD:
         rc = CNK_RC_SUCCESS(0);
         break;

      default:
         rc = CNK_RC_FAILURE(EINVAL);
         break;
   }  // End switch on cmd
   
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::fcntl%s: fd=%d cmd=%d rc=%s\n", whoami(), fd, cmd, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::flock(int fd, int operation)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
    
    int t;
    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    KThread_t* thread;
    
    switch(operation)
    {
#define LOCK_EXNB (LOCK_EX|LOCK_NB)
#define LOCK_UNNB (LOCK_UN|LOCK_NB)
        case LOCK_EX:
        case LOCK_EXNB:
            thread = GetMyKThread();
            
            if(entry->locked)
            {
                if((operation & LOCK_NB) != 0)
                {
                    rc = CNK_RC_FAILURE(EWOULDBLOCK);
                }
                else
                {
                    rc = CNK_RC_SUCCESS(0);
                    thread->flockBlockedFile = entry;
                    Sched_Block(GetMyKThread(), SCHED_STATE_FLOCK );
                    GetMyKThread()->Pending |= KTHR_PENDING_YIELD;
                }
            }
            else
            {
                thread->flockBlockedFile = NULL;
                entry->locked = 1;
                rc = CNK_RC_SUCCESS(0);
            }
            break;
        case LOCK_UN:
        case LOCK_UNNB:
            if(!entry->locked)
                rc = CNK_RC_FAILURE(EBADF);
            else
            {
                rc = CNK_RC_SUCCESS(0);
                entry->locked = 0;
                for(t=0; t<NUM_KTHREADS; t++)
                {
                    thread = GetKThreadFromTid(t);
                    if(thread != NULL)
                    {
                        if(thread->flockBlockedFile == entry)
                        {
                            entry->locked = 1;
                            thread->flockBlockedFile = NULL;
                            Sched_Unblock(thread, SCHED_STATE_FLOCK);
                            break;
                        }
                    }
                }
            }
            break;
        default:
            rc = CNK_RC_FAILURE(EINVAL);
            break;
    }
    Kernel_Unlock(&ShareLock);
    return rc;
#else                                                                                                                  // FUSEDOS
	return CNK_RC_FAILURE(EINVAL);                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#define CR_ERROR  (0x10000000)

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern "C" void FLock_Interrupt(KThread_t *kthr)
{
    Kernel_Lock(&ShareLock);
    if (kthr->flockBlockedFile != NULL)
    {
	kthr->Reg_State.cr     |= CR_ERROR; // syscall failed
	kthr->Reg_State.gpr[3] = EINTR; // result of the flock syscall
	kthr->flockBlockedFile = NULL;
	Sched_Unblock(kthr, SCHED_STATE_FLOCK);
    }
    Kernel_Unlock(&ShareLock);
}
#endif                                                                                                                 // FUSEDOS

uint64_t shmFS::fstat(int fd, struct stat *statbuf)
{
   return fstat64(fd, (struct stat64 *)statbuf);
}

uint64_t shmFS::fstat64(int fd, struct stat64 *statbuf)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   ShmMgrEntry_t* entry;
   uint64_t rc = findByDescriptor(fd, entry);
   
   if (CNK_RC_IS_SUCCESS(rc)) 
   {
       touchAccess(entry);
       fillStatStructure(statbuf, entry);
   }
   
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   TRACE( TRACE_ShmFS, ("(I) shmFS::fstat64%s: fd=%d statbuf=%p rc=%s\n", whoami(), fd, statbuf, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::fstatfs(int fd, struct statfs *statbuf)
{
   uint64_t rc = CNK_RC_FAILURE(EINVAL);
   TRACE( TRACE_ShmFS, ("(I) shmFS::fstatfs%s: fd=%d statbuf=%p rc=%s\n", whoami(), fd, statbuf, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::fstatfs64(int fd, struct statfs64 *statbuf)
{
   uint64_t rc = CNK_RC_FAILURE(EINVAL);
   TRACE( TRACE_ShmFS, ("(I) shmFS::fstatfs64%s: fd=%d statbuf=%p rc=%s\n", whoami(), fd, statbuf, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::ftruncate(int fd, off_t offset)
{
   return ftruncate64(fd, offset);
}

uint64_t shmFS::ftruncate64(int fd, off_t offset)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);

    if (CNK_RC_IS_SUCCESS(rc)) 
    {
        touchAccess(entry);
        touchMod(entry);
        entry->AllocatedSize = offset;
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   TRACE( TRACE_ShmFS, ("(I) shmFS::ftruncate64%s: fd=%d offset=%ld rc=%s\n", whoami(), fd, offset, CNK_RC_STRING(rc)) );
   return rc;
}

#include <sys/ioctl.h>

uint64_t shmFS::ioctl(int fd, unsigned long int cmd, void *parm3)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_SUCCESS(rc)) 
    {
        switch(cmd)
        {
            case FIOBGQATOMIC:
                entry->needAtomic = true;
                rc = 0;
                break;
            default:
                rc = EINVAL;
                break;
        }
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    
    return 0;
}


uint64_t shmFS::lchown(const char *path, uid_t uid, gid_t gid)
{
    return shmFS::chown(path, uid, gid);
}

uint64_t shmFS::lseek(int fd, off_t offset, int whence)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    touchAccess(entry);
    touchMod(entry);
    off_t position = File_GetCurrentOffset(fd);
    
    switch(whence)
    {
        case SEEK_SET:
            if(((uint64_t)offset > entry->AllocatedSize) || (offset < 0))
                rc = CNK_RC_FAILURE(EINVAL);
            else
            {
                File_SetCurrentOffset(fd, offset);
                rc = CNK_RC_SUCCESS(File_GetCurrentOffset(fd));
            }
            break;
        case SEEK_CUR:
            if(((uint64_t)(position + offset) > entry->AllocatedSize) || (position + offset < 0))
                rc = CNK_RC_FAILURE(EINVAL);
            else
            {
                File_SetCurrentOffset(fd, position+offset);              
                rc = CNK_RC_SUCCESS(File_GetCurrentOffset(fd));
            }
            break;
        case SEEK_END:
            if((entry->AllocatedSize + offset > entry->AllocatedSize) || ((int64_t)entry->AllocatedSize + offset < 0))
                rc = CNK_RC_FAILURE(EINVAL);
            else
            {
                File_SetCurrentOffset(fd, entry->AllocatedSize + offset);
                rc = CNK_RC_SUCCESS(File_GetCurrentOffset(fd));
            }
            break;
        default:
            rc = CNK_RC_FAILURE(EINVAL);
            break;
    }
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}

uint64_t shmFS::mkdir(const char *path, mode_t mode)
{
    uint64_t rc = 0;
    return rc;
}

uint64_t shmFS::mmap(int fd, uint64_t length, uint64_t offset)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   ShmMgrEntry_t* entry;
   uint64_t rc = findByDescriptor(fd, entry);
   if (CNK_RC_IS_FAILURE(rc)) 
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
       Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
       return rc;
   }
   
   // Was the address already given out from previous call to mmap?
   if ( entry->AllocatedAddr != 0 )
   {
       rc = CNK_RC_SUCCESS(entry->AllocatedAddr);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
       Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	   pthread_mutex_unlock(shm_lock);                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
       return rc;
   }
   
   if(entry->needAtomic)
   {
#define L2ATOMICSIZE (CNK_STATICSHMATOMICSIZE * CONFIG_CNK_PAGE_SIZE)
       if(getShmManager()->Size < L2ATOMICSIZE)
       {
           rc = CNK_RC_FAILURE(ENOMEM);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
           Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
           pthread_mutex_unlock(shm_lock);                                                                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS
           return rc;
       }
       
       ShmMgrEntry_t* previous;
       previousAddress(getShmManager()->VStart + getShmManager()->Size, previous);
       if(previous)
           entry->AllocatedAddr = MAX(previous->AllocatedAddr + previous->AllocatedSize, getShmManager()->VStart + getShmManager()->Size - L2ATOMICSIZE);
       else
           entry->AllocatedAddr = getShmManager()->VStart + getShmManager()->Size - L2ATOMICSIZE;
       entry->AllocatedSize = 0;
   }
   entry->pinned = true;
   rc = defineMemorySpace(entry, length);
   
   touchAccess(entry);   
   if(rc)
       rc = CNK_RC_FAILURE(ENOSPC);
   else
       rc = CNK_RC_SUCCESS(entry->AllocatedAddr);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_unlock(shm_lock);                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   TRACE( TRACE_ShmFS, ("(I) shmFS::mmap%s: fd=%d length=%lu offset=%lu rc=%s\n", whoami(), fd, length, offset, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::open(const char* pathname, int oflags, mode_t mode)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   // See if handle for that pathname already assigned.
   ShmMgrEntry_t* entry = NULL;
   uint64_t rc = findByName(pathname, entry);
   if ( CNK_RC_IS_SUCCESS(rc) )
   {
       touchAccess(entry);
       if ((oflags & O_CREAT) && (oflags & O_EXCL) )
       {
           // Request was to create the file exclusively and it already exists.
           rc = CNK_RC_FAILURE(EEXIST);
       }
       else 
       {
           // Allocate a new descriptor for the existing shared memory file.
           entry->LinkCount++;
           int fd = File_AllocateFD(SHMTOINDEX(entry), _FD_type);
           if (fd == -1) 
               rc = CNK_RC_FAILURE(EMFILE);
           else 
           {
               File_SetCurrentOffset(fd, 0);
               rc = CNK_RC_SUCCESS(fd);
           }
       }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
       Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
       pthread_mutex_unlock(shm_lock);                                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
       
       // Need to allocate descriptor for existing file.
       TRACE( TRACE_ShmFS, ("(I) shmFS::open%s: found existing file path=%s oflags=%08x rc=%s\n", whoami(), pathname, oflags, CNK_RC_STRING(rc)) );
       return rc;
   }
   
   if ( CNK_RC_ERRNO(rc) != ENOENT )
   {
      // The failure was something other than "file not found".  Just return it.
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(shm_lock);                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      return rc;
   }
   
   int x;
   entry = NULL;
   for(x=0; x<SHM_MAX_OPENS; x++)
   {
       if(SharedPool[x].owner == NULL)
       {
           memset(&SharedPool[x], 0, sizeof(SharedPool[x]));
           
           entry = &SharedPool[x];
           entry->owner = getShmManager();
           touchAccess(entry);
           break;
       }
   }
   
   // Is there a slot available for allocating this descriptor?
   if(entry != NULL)
   {
       // Allocate a new descriptor.
       int fd = File_AllocateFD(SHMTOINDEX(entry), _FD_type );
       
       // Able to get a descriptor?
       if ( fd == -1 )
       {
           rc = CNK_RC_FAILURE(EMFILE);  // Process already has max number of files open
       }
       else 
       {
           // Init the shared memory file descriptors.
           // Fill in the appropriate file information in the available slot.
           // The memory assignment will occur on the call to mmap with the fd.
           strncpy(entry->FileName, pathname, sizeof(entry->FileName));
           entry->Oflags = oflags;
           entry->Mode = mode;
           entry->LinkCount += 1;
           entry->AllocatedAddr = 0;
           entry->AllocatedSize = 0;
           entry->UserID = GetMyAppState()->UserID;
           entry->GroupID = GetMyAppState()->GroupID;
           File_SetCurrentOffset(fd, 0);
#if CONFIG_SHMFILEIMPLICITL2ATOMIC
           if(strstr(entry->FileName, "l2atomic") != 0)
           {
               entry->needAtomic = true;
           }
#endif
           rc = CNK_RC_SUCCESS(fd);
       }
   }
   else
   {
      rc = CNK_RC_FAILURE(EMFILE);  // Process already has max number of files open
   }
   
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_unlock(shm_lock);                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::open%s: path=%s oflags=%08x mode=%08x rc=%s\n", whoami(), pathname, oflags, mode, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::read(int fd, void* buffer, size_t length)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    if(entry->AllocatedAddr == 0)
    {
        rc = CNK_RC_SUCCESS(0);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    touchAccess(entry);
    length = MIN(length, entry->AllocatedSize - File_GetCurrentOffset(fd));
    memcpy(buffer, (void*)(entry->AllocatedAddr + File_GetCurrentOffset(fd)), length);
    File_SetCurrentOffset(fd, File_GetCurrentOffset(fd) + length);
    rc = CNK_RC_SUCCESS(length);
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}

uint64_t shmFS::readv(int fd, const struct iovec *iov, int iovcnt)
{
    uint64_t rc = 0;
    uint64_t totalbytes = 0;
    int x;
    for(x=0; x<iovcnt; x++)
    {
        rc = shmFS::read(fd, iov[x].iov_base, iov[x].iov_len);
        if (CNK_RC_IS_FAILURE(rc))
        {
            return rc;
        }
        totalbytes += rc;
    }
    return CNK_RC_SUCCESS(totalbytes);
}

uint64_t shmFS::rmdir(const char *path)
{
    uint64_t rc = 0;
    return rc;
}

uint64_t shmFS::stat(const char *pathname, struct stat *statbuf)
{
   return stat64(pathname, (struct stat64 *)statbuf);
}

uint64_t shmFS::stat64(const char *pathname, struct stat64 *statbuf)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   // See if the pathname is a valid shared memory file.
   ShmMgrEntry_t* entry;
   uint64_t rc = findByName(pathname, entry);

   if ( CNK_RC_IS_SUCCESS(rc) )
   {
       touchAccess(entry);
       fillStatStructure(statbuf, entry);
   }
   
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_unlock(shm_lock);                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   TRACE( TRACE_ShmFS, ("(I) shmFS::stat64%s: path=%s statbuf=%p rc=%s\n", whoami(), pathname, statbuf, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::statfs(const char *pathname, struct statfs *statbuf)
{
   return statfs64(pathname, (struct statfs64 *)statbuf);
}

uint64_t shmFS::statfs64(const char *pathname, struct statfs64 *statbuf)
{
   // shm_open() expects the magic number for the shared memory file system in the f_type field.
   memset(statbuf, 0, sizeof(struct statfs64));
   statbuf->f_type = getDeviceID();
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::statfs64%s: path=%s statbuf=%p rc=%s\n", whoami(), pathname, statbuf, CNK_RC_STRING(0)) );
   return CNK_RC_SUCCESS(0);
}

uint64_t shmFS::unlink(const char* pathname)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   ShmMgrEntry_t* entry;
   uint64_t rc = findByName(pathname, entry);
   
   if(entry != NULL)
   {
       if(entry->LinkCount == 0)
       {
           entry->owner = NULL;
       }
       else
       {
           entry->LinkCount |= 0xf0000000;
       }
   }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_unlock(shm_lock);                                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::unlink%s: path=%s rc=%s\n", whoami(), pathname, CNK_RC_STRING(rc)) );
   return rc;
}

uint64_t shmFS::utime(const char *pathname, const struct utimbuf *utimes)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    ShmMgrEntry_t* entry;
    uint64_t rc = findByName(pathname, entry);
    if ( CNK_RC_IS_SUCCESS(rc) )
    {
        entry->lastAccess = ConvertTimeToCycles(utimes->actime);
        entry->lastMod    = ConvertTimeToCycles(utimes->modtime);
        rc = CNK_RC_SUCCESS(0);
    }
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;
}

uint64_t shmFS::write(int fd, const void* buffer, size_t length)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByDescriptor(fd, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    touchAccess(entry);
    touchMod(entry);
    rc = defineMemorySpace(entry, MAX(entry->AllocatedSize, File_GetCurrentOffset(fd) + length));
    if(rc)
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return CNK_RC_FAILURE(ENOSPC);
    }
    memcpy((void*)(entry->AllocatedAddr + File_GetCurrentOffset(fd)), buffer, length);
    File_SetCurrentOffset(fd, File_GetCurrentOffset(fd) + length);
    rc = CNK_RC_SUCCESS(length);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    
    return rc;
}

uint64_t shmFS::writev(int fd, const struct iovec *iov, int iovcnt)
{
    uint64_t rc = 0;
    uint64_t totalbytes = 0;
    int x;
    for(x=0; x<iovcnt; x++)
    {
        rc = shmFS::write(fd, iov[x].iov_base, iov[x].iov_len);
        if (CNK_RC_IS_FAILURE(rc))
        {
            return rc;
        }
        totalbytes += rc;
    }
    return CNK_RC_SUCCESS(totalbytes);
}

uint64_t shmFS::getFilenames(uint32_t& numfiles, char filenames[][128])
{
    ShmMgrEntry_t* entry = NULL;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    numfiles = 0;
    for ( uint32_t index = 0; index < SHM_MAX_OPENS; index++ )
    {
        if(numfiles >= 256)
            break;
        if( SharedPool[ index ].owner == getShmManager())
        {
            entry = &SharedPool[index];
            strncpy(filenames[numfiles++], entry->FileName, sizeof(filenames[0]));
        }
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return CNK_RC_SUCCESS(0);
}

uint64_t shmFS::getFileContents(const char* pathname, size_t offset, size_t& length, char* buffer)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    ShmMgrEntry_t* entry;
    uint64_t rc = findByName(pathname, entry);
    if (CNK_RC_IS_FAILURE(rc))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    if(entry->AllocatedAddr == 0)
    {
        rc = CNK_RC_SUCCESS(0);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    if(offset > entry->AllocatedSize)
    {
        rc = CNK_RC_FAILURE(EINVAL);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock(shm_lock);                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        return rc;
    }
    
    touchAccess(entry);
    
    length = MIN(length, entry->AllocatedSize - offset);
    memcpy(buffer, (void*)(entry->AllocatedAddr + offset), length);
    rc = CNK_RC_SUCCESS(length);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return rc;

}

uint64_t shmFS::findByName(const char *pathname, ShmMgrEntry_t*& entry)
{
   entry = NULL;
   
   // Loop thru the already allocated /dev/shm/<file> allocations to see if this file already handled
   for ( uint32_t index = 0; index < SHM_MAX_OPENS; index++ )
   {
       // Matching pathname?
       if(( SharedPool[ index ].owner == getShmManager()) &&
          ( strncmp(SharedPool[ index ].FileName, pathname, APP_MAX_PATHNAME) == 0 ))
       {
           entry = &SharedPool[index];
           TRACE( TRACE_ShmFS, ("(I) shmFS::findByName%s: found %s at slot %d\n", whoami(), pathname, index) );
           return CNK_RC_SUCCESS(0);
       }
   }
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::findByName%s: %s was not found\n", whoami(), pathname) );
   return CNK_RC_FAILURE(ENOENT);
}

uint64_t shmFS::findByDescriptor(int fd, ShmMgrEntry_t*& entry)
{
   // Extract the index of the shared memory element from the remote fd field of the descriptor.
   int slot = File_GetRemoteFD(fd);
   entry = NULL;

   // Obtain pointer to the shared memory object.
   uint64_t rc = CNK_RC_SUCCESS(0);
   if (slot == -1) 
      rc = CNK_RC_FAILURE(EBADF);
   else
   {
       entry = &SharedPool[slot];
   }
   
   TRACE( TRACE_ShmFS, ("(I) shmFS::findByDescriptor%s: descriptor %d is at slot %d\n", whoami(), fd, slot) );
   return rc;
}

void shmFS::fillStatStructure(struct stat64 *statbuf, ShmMgrEntry_t* entry)
{
   memset(statbuf, 0, sizeof(struct stat64));
   statbuf->st_dev     = getDeviceID();
   statbuf->st_ino     = SHMTOINDEX(entry);
   statbuf->st_size    = entry->AllocatedSize;
   statbuf->st_blksize = BlockSize;
   statbuf->st_blocks  = entry->AllocatedSize / BlockSize;
   statbuf->st_mode    = entry->Mode;
   statbuf->st_atime   = ConvertTimeToMicroseconds(entry->lastAccess)/1000000;
   statbuf->st_mtime   = ConvertTimeToMicroseconds(entry->lastMod)/1000000;
   statbuf->st_uid     = entry->UserID;
   statbuf->st_gid     = entry->GroupID;

   return;
}

int shmFS::cleanupJob(int fs)
{
    return(clearAllocations());
}

int shmFS::clearAllocations(void)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&ShareLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock(shm_lock);                                                                                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    for (uint32_t index = 0; index < SHM_MAX_OPENS; index++ )
    {
        if(SharedPool[index].owner == getShmManager())
        {
            memset(&SharedPool[index], 0, sizeof(SharedPool[index]));
        }
    }
    
    // Initialize the shared memory object. 
    memset(getShmManager(), 0x00, sizeof(ShmMgr_t));
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&ShareLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(shm_lock);                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return 0;
}

