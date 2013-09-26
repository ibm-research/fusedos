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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdint.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Kernel.h"
#include "fs/virtFS.h"

// Calls to mmap must be multiple of pagesize in length, start and offset, else EINVAL
#ifndef PAGESIZE
#define PAGESIZE (4096)
#endif

#ifndef PAGEMASK
#define PAGEMASK (PAGESIZE - 1)
#endif

#define ROUND_UP_PAGESIZE(a) (((a) + PAGEMASK) & ~PAGEMASK)

// Access temporarily locking mechanism in cnk/mmap.c for thread safety
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern "C" Lock_Atomic_t MMapLock[64];
extern "C" Lock_Atomic_t MprotectLock[64];
extern "C" Lock_Atomic_t GuardAreaLock[64];
extern "C" void GetSpinLock(Lock_Atomic_t *pLock);
extern "C" void Mmap_ReleaseSpinLock(Lock_Atomic_t *pLock);
#endif                                                                                                                 // FUSEDOS

// int brk( void *end_data_segment )
uint64_t  sc_brk( SYSCALL_FCN_ARGS)
{
    AppProcess_t *pAppProc = GetMyProcess();
    uint64_t rc;
    uint64_t brk_val = r3;

    TRACESYSCALL(("(I) sc_brk[%d]: brk_val=r3=0x%08lx\n",
                  ProcessorID(), brk_val));
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    assert(pAppProc);
#endif                                                                                                                 // FUSEDOS
    if ( !brk_val )
    {
        Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCBRKI, ROUND_UP_L1_CACHELINE(pAppProc->Heap_Break),0,0,0);
        rc = CNK_RC_SUCCESS(ROUND_UP_L1_CACHELINE(pAppProc->Heap_Break));
    }
    else if ( (brk_val >= pAppProc->Heap_Start) && (brk_val <= pAppProc->Heap_End) )
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Lock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_lock(&pAppProc->MMap_mutex);                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCBRK_, (int64_t)(brk_val - pAppProc->Heap_Break), brk_val, 0, 0);
        
        MoveGuardDirection_t direction = ((brk_val > pAppProc->Heap_Break) ? MoveGuardUp : MoveGuardDown);
        // !!! Note: the following call to adjust the guard was done after releasing the mmap lock in BGP, however, since the guard will fire
        // when in privileged state in BGQ, we need to make sure we adjust the guard before we memset the area within the break range.
        // Potential Deadlock with the MMAP lock. Should be OK, however may have been doing this outside the lock in BGP for a reason
        if (!pAppProc->MMap_Start) // only call for adjustment if no MMAPs have been done and guarding is enabled
        {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
            Guard_Adjust(brk_val, direction, 0); // adjust the stack guard area
#endif                                                                                                                 // FUSEDOS
        }

        if ( direction == MoveGuardUp )
        {
            char *addr = (char *)(pAppProc->Heap_Break);
            uint64_t delta = (brk_val - pAppProc->Heap_Break);
            memset( addr, 0, delta );
            direction = MoveGuardUp;
        }
        pAppProc->Heap_Break = brk_val;
        rc = CNK_RC_SUCCESS(brk_val);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_unlock(&pAppProc->MMap_mutex);                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    }
    else
    {
        // brk should return the current "brk" value if it was unable to increase the size
	rc = CNK_RC_SUCCESS(pAppProc->Heap_Break);
    }    

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) sc_brk[%d]: brk_val=0x%08lx, returning 0x%08x.\n",
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) sc_brk[%d]: brk_val=0x%08lx, returning 0x%08lx.\n",                                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS
                 ProcessorID(), brk_val, rc ));

    return( rc );
}



uint64_t  sc_mmap( SYSCALL_FCN_ARGS)
{
    AppProcess_t *pAppProc = GetMyProcess();
    uint64_t mm_addr  = r3;
    uint64_t mm_len   = r4;
    uint32_t mm_prot  = r5;
    uint32_t mm_flags = r6;
    int mm_fd         = (int)r7;
    uint64_t mm_ofs   = r8;
    uint64_t rc;
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: addr=0x%016lx, len=0x%08x, prot=0x%08x(%c%c%c%c), flags=0x%08x=(%s%s%s%s%s%s%s%s%s%s), filedes=0x%08x, off=0x%08x, MMap_Start=0x%016lx, MMap_End=0x%016lx.\n",
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: addr=0x%016lx, len=0x%08lx, prot=0x%08x(%c%c%c%c), flags=0x%08x=(%s%s%s%s%s%s%s%s%s%s), filedes=0x%08x, off=0x%08lx, MMap_Start=0x%016lx, MMap_End=0x%016lx.\n",
#endif                                                                                                                 // FUSEDOS
                  __func__, ProcessorID(),
                  mm_addr, mm_len,
                  mm_prot,
                  (mm_prot & PROT_READ  ? 'R' : '-'),
                  (mm_prot & PROT_WRITE ? 'W' : '-'),
                  (mm_prot & PROT_EXEC  ? 'X' : '-'),
                  (mm_prot & PROT_NONE  ? 'N' : '-'),
                  mm_flags,
                  (mm_flags & MAP_FIXED      ? "Fixed  "      : ""),
                  (mm_flags & MAP_SHARED     ? "Shared "      : ""),
                  (mm_flags & MAP_PRIVATE    ? "Private "     : ""),
                  (mm_flags & MAP_EXECUTABLE ? "Executable "  : ""),
                  (mm_flags & MAP_NORESERVE  ? "NoReserve "   : ""),
                  (mm_flags & MAP_LOCKED     ? "Locked "      : ""),
                  (mm_flags & MAP_GROWSDOWN  ? "GrowsDown "   : ""),
                  (mm_flags & MAP_ANONYMOUS  ? "Anonymous "   : ""),
                  "",//(mm_flags & MAP_32BIT      ? "32bit "       : ""),
                  (mm_flags & MAP_FILE       ? "File "        : ""),
                  mm_fd, mm_ofs,
                  pAppProc->MMap_Start, pAppProc->MMap_End ));

    if ( (mm_addr & PAGEMASK) || (mm_ofs & PAGEMASK) )
    {
        printf("(E) mmap user error: addr or offset not multiple of pagesize. Returning EINVAL.\n");
        return CNK_RC_FAILURE(EINVAL);
    }

    if ( mm_flags & (MAP_EXECUTABLE | MAP_GROWSDOWN | MAP_FILE) )
    {
        return CNK_RC_FAILURE(ENOMEM);
    }
    
    if(mm_len == 0)
    {
        return CNK_RC_FAILURE(EINVAL);
    }
    
    // MAP_SHARED?
    if ( mm_flags & MAP_SHARED )
    {
        // See if able to allocate shared memory.
        rc = File_GetFSPtr(mm_fd)->mmap( mm_fd, mm_len, mm_ofs );
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        TRACESYSCALL(("(I) %s[%d]: Returning 0x%08x for len=0x%08x fd=0x%d.\n", __func__, ProcessorID(), rc, mm_len, mm_fd ));
#else                                                                                                                  // FUSEDOS
        TRACESYSCALL(("(I) %s[%d]: Returning 0x%08lx for len=0x%08lx fd=0x%d.\n", __func__, ProcessorID(), rc, mm_len, mm_fd ));
#endif                                                                                                                 // FUSEDOS

        return rc;
    }  // End map shared

    if ( !(pAppProc->MMap_Start) )
    {
        // grab the allocator lock and check again. Another thread may have initialized or be initializing MMap
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Lock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_lock(&pAppProc->MMap_mutex);                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        if ( !pAppProc->MMap_Start )
        {
            uint64_t mmap_end   = ROUND_DN_1M( pAppProc->Heap_VEnd - pAppProc->MainStackReserve);
            uint64_t mmap_start = ROUND_UP_1M( (pAppProc->Heap_Break + (4 * 1024)) );
            // see if there is any space left to create the mmap environment
            if (mmap_end <= mmap_start)
            {
                // no space left. We tried to give heap_brk 4M of growing room. It obviously has already used much more
                // than its fair share. Lets take away its 4*1024*1024 of growing room and see if we can proceed with
                // the creating of the MMAP environment
                mmap_start = ROUND_UP_1M( (pAppProc->Heap_Break) );
            }
            if (mmap_end > mmap_start)
            {
                pAppProc->MMap_End   = mmap_end;
                pAppProc->MMap_Start = mmap_start;
                pAppProc->Heap_End  = (pAppProc->MMap_Start - 1);
                // Make the testing and setting of the MMap_HighMark value atomic. Any thread (core) in the process
                // may be trying to do this. We need to ensure the highest value attempting to be set is not lost
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
                Kernel_Lock(&pAppProc->GuardAreaLock);
#endif                                                                                                                 // FUSEDOS
                if (mmap_start > pAppProc->Guard_HighMark)
                    pAppProc->Guard_HighMark = mmap_start;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
                Kernel_Unlock(&pAppProc->GuardAreaLock);
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
                TRACESYSCALL(("(I) %s[%d]: Brk: 0x%08x - 0x%08x. MMap: 0x%08x - 0x%08x.\n",
#else                                                                                                                  // FUSEDOS
                TRACESYSCALL(("(I) %s[%d]: Brk: 0x%08lx - 0x%08lx. MMap: 0x%08lx - 0x%08lx.\n",                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS
                              __func__, ProcessorID(),
                             pAppProc->Heap_Start, pAppProc->Heap_End,
                             pAppProc->MMap_Start, pAppProc->MMap_End   ));

                MMap_Init( pAppProc->MMap_Start, (pAppProc->MMap_End - pAppProc->MMap_Start) );
            }
        }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_unlock(&pAppProc->MMap_mutex);                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    }

    uint64_t mm_rc;

    // MAP_FIXED?
    /*
       For dynamic linking support, ld.so will make a mmap call with MAP_FIXED.  This is to load the
       application that is passed as a parameter.  The MAP_FIXED either needs to be honored or an ENOMEM
       needs to be sent back to the caller.
    */
    if (!pAppProc->MMap_Start)  // MMap_Start will not be set if heap allocations did not give us enough room to init the mmap environment
    {
        mm_rc = 0;  // set to zero to indicate that no memory was allocated
    }
    else if ( mm_flags & MAP_FIXED )
    {
        mm_rc = (uint64_t)MMap_Malloc_Addr( mm_len, mm_addr);  // mm_addr
    }  // End map fixed
    else
    {
        mm_rc = (uint64_t)MMap_Malloc( mm_len);

    }  // End not MAP_FIXED case

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    if ( mm_rc )
    {
#else                                                                                                                  // FUSEDOS
    if ( mm_rc  &&
         (Guard_Adjust(mm_rc+ROUND_UP_PAGESIZE( mm_len ),MoveGuardUp,0)== 0))
    {
        if (((mm_flags & MAP_UNINITIALIZED) == 0) && (mm_flags & MAP_ANONYMOUS) &&
            ((mm_flags & MAP_FIXED) || (pAppProc->MmapMgr.low_unmap < (mm_rc+mm_len))))
        {
            //printf("Clearing %016lx for %ld bytes. flags=%08x fd=%d\n", mm_rc, mm_len, mm_flags, mm_fd);
            // Initialize the memory that is being returned to zero.
            memset( (void *)mm_rc, 0, ROUND_UP_PAGESIZE( mm_len ) );
        }
#endif                                                                                                                 // FUSEDOS
        // File descriptor passed in?
        if ( ((mm_flags & MAP_ANONYMOUS) == 0) &&(mm_fd != -1) && 
           ( (File_GetFDType( mm_fd ) == FD_FILE )
           ||(File_GetFDType( mm_fd ) == FD_MEMFILE )))
        {
            rc = MMap_readFile((void*)mm_rc, mm_fd, mm_ofs, mm_len);
            if (CNK_RC_IS_FAILURE(rc)) 
            {
                return rc;
            }
        }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        TRACESYSCALL(("(I) %s[%d]: Returning 0x%08x for len=0x%08x.\n", __func__, ProcessorID(), mm_rc, mm_len ));
#else                                                                                                                  // FUSEDOS
        TRACESYSCALL(("(I) %s[%d]: Returning 0x%08lx for len=0x%08lx.\n", __func__, ProcessorID(), mm_rc, mm_len ));
#endif                                                                                                                 // FUSEDOS
        return CNK_RC_SUCCESS(mm_rc);
    }  // End memory available
    else
    {
        return CNK_RC_FAILURE(ENOMEM);
    }
}


uint64_t  sc_munmap( SYSCALL_FCN_ARGS)
{
    void *mm_start = (void *)r3;
    size_t mm_len  = (size_t)r4;
    int rc;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: start=0x%08x, len=0x%08x.\n",
                  __func__, ProcessorID(), (uint64_t)mm_start, mm_len ));
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: start=0x%08lx, len=0x%08lx.\n",                                                         // FUSEDOS
                  __func__, ProcessorID(), (uint64_t)mm_start, mm_len ));                                              // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    if ( ((uint64_t)mm_start & PAGEMASK) )
    {
        printf("(E) munmap user error: addr not multiple of pagesize. Returning EINVAL.\n");
        return CNK_RC_FAILURE(EINVAL);
    }

    rc = MMap_Free( mm_start, mm_len );

    return (rc ? CNK_RC_FAILURE(EINVAL) : CNK_RC_SUCCESS(0));
}


uint64_t  sc_mremap( SYSCALL_FCN_ARGS)
{
    int rc;
    void *mr_addr = (void *)r3;
    size_t mr_oldLen = (size_t)r4;
    size_t mr_newLen = (size_t)r5;
    uint32_t mr_flags = r6;
    uint64_t mr_rc = 0;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: mr_addr=0x%08x, old len=0x%08x, new len=0x%08x, flags=0x%08x, %s\n",
                  __func__, ProcessorID(), (uint64_t)mr_addr, mr_oldLen, mr_newLen, mr_flags,
                  (mr_flags & MREMAP_MAYMOVE ? "May Move" : "")  ));
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: mr_addr=0x%08lx, old len=0x%08lx, new len=0x%08lx, flags=0x%08x, %s\n",          // FUSEDOS
                  __func__, ProcessorID(), (uint64_t)mr_addr, mr_oldLen, mr_newLen, mr_flags,                          // FUSEDOS
                  (mr_flags & MREMAP_MAYMOVE ? "May Move" : "")  ));                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    if ( ((uint64_t)mr_addr & PAGEMASK) )
    {
        printf("(E) mremap user error: addr not multiple of pagesize. Returning EINVAL.\n");
        return CNK_RC_FAILURE(EINVAL);

    }

    // If called to shrink the allocation, free the remaining portion of the original allocation
    if ( mr_oldLen > mr_newLen )
    {
        rc = MMap_Free( (void*)( ((uint64_t)mr_addr) +mr_newLen), (mr_oldLen-mr_newLen) );

        return CNK_RC_SUCCESS((uint64_t)mr_addr);

    }  // End called to shrink the allocation

    // Going to just request a new address range for the new length specified.  If there is memory for the new
    // allocation, the old data will be copied over to the new memory area and then the old memory will be returned
    // to the free memory pool.
    mr_rc = (uint64_t)MMap_Malloc( mr_newLen );

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    if ( mr_rc )
#else                                                                                                                  // FUSEDOS
    if ( mr_rc &&
         (Guard_Adjust(mr_rc+ROUND_UP_PAGESIZE( mr_newLen ),MoveGuardUp,0)==0))
#endif                                                                                                                 // FUSEDOS
    {
        // Clear the memory and then copy in the old information
        memset( (void *)mr_rc, 0, ROUND_UP_PAGESIZE( mr_newLen ) );
        memcpy( (void *)mr_rc, (void *)mr_addr, mr_oldLen );

        // Free the old memory segment
        rc = MMap_Free( mr_addr, mr_oldLen );

        return CNK_RC_SUCCESS(mr_rc);

    }  // End able to allocate a new memory segment
    else
    {
        return CNK_RC_FAILURE(ENOMEM);
    }  // End unable to allocate new memory segment

}



uint64_t  sc_mprotect( SYSCALL_FCN_ARGS)
{
    AppProcess_t *pProcess = GetMyProcess();
    uint64_t mp_addr = r3;
    uint64_t mp_len  = r4;
    uint32_t mp_prot = r5;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: mp_addr=0x%08x, mp_len=0x%08x, mp_prot=0x%08x.\n",
                  __func__, ProcessorID(), mp_addr, mp_len, mp_prot ));
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s[%d]: Entry: mp_addr=0x%08lx, mp_len=0x%08lx, mp_prot=0x%08x.\n",                             // FUSEDOS
                  __func__, ProcessorID(), mp_addr, mp_len, mp_prot ));                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    // Lock access to this table since other threads in the process may also be accessing it
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&pProcess->MprotectLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_lock(&pProcess->Mprotect_mutex);                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    if ( (mp_prot == 0) && mp_len) // Are no permissions being granted?
    {
        // This is likely a guard assignment since no permissions are being granted. Store this into our manager object

        // First we must verify that the supplied address and size meet our restrictions. The address must be boundary 
        // aligned with specified length and the length must be a power of 2.

        // Test the size of the user-supplied guard. It must be a power of 2 
        uint64_t guard_boundary = popcnt64(mp_len);
        uint64_t adjustedLength = mp_len;
        if (guard_boundary > 1)
        {
            // we need to do some adjusting. Round down to a power of 2
            uint64_t leadingZeros = cntlz64(mp_len);
            adjustedLength = 1 << (63 - leadingZeros);
        }
        
        pProcess->MprotectMgr.address[pProcess->MprotectMgr.index] = mp_addr;
        pProcess->MprotectMgr.length[pProcess->MprotectMgr.index] = adjustedLength;
        if ( ++pProcess->MprotectMgr.index >= MPROTECT_MGR_ENTRIES )
            pProcess->MprotectMgr.index = 0; // reset the index since we are at the end of the table
        //printf("Protect Request: addr %016lx size %016lx index:%d\n", mp_addr, mp_len, pProcess->MprotectMgr.index);
    }
    else
    {
        // At least some permissions are being granted. If this was a tracked address, remove it from our list.
        // We could be smarter about compressing the list to remove zeroed entries and adjust the index, however
        // it is unlikely that there will be many situations when glib
        // removes existing guard pages, we will just keep it simple and zero out the entry.
        int i;
        for (i=0; i < MPROTECT_MGR_ENTRIES; i++ )
        {
            if ( pProcess->MprotectMgr.address[i] == mp_addr )
            {
                //printf("Removing protected address %016lx\n",mp_addr);
                pProcess->MprotectMgr.address[i] = 0;
                break;
            }
        }
    }
    // Release the lock
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&pProcess->MprotectLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(&pProcess->Mprotect_mutex);                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    return CNK_RC_SUCCESS(0);
}



uint64_t  sc_mlock( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID() ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_munlock( SYSCALL_FCN_ARGS)
{
  TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID() ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_mlockall( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID()  ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_munlockall( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID()  ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_msync( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID()  ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_mmap2( SYSCALL_FCN_ARGS)
{
  TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID() ));

    return CNK_RC_FAILURE(ENOSYS);
}


uint64_t  sc_madvise( SYSCALL_FCN_ARGS)
{
    TRACESYSCALL(("(I) %s[%d]: Entry (add args!).\n", __func__, ProcessorID()  ));
    
    // syscall is advisory to the kernel.  Just return success.  
    return CNK_RC_SUCCESS(0);
}

