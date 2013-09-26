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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Kernel.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/wu_mmio.h>
#endif                                                                                                                 // FUSEDOS


#ifndef __INLINE__
#define __INLINE__ extern inline
#endif

#ifndef PAGESIZE
#define PAGESIZE (4096)
#endif

#ifndef PAGEMASK
#define PAGEMASK (PAGESIZE - 1)
#endif

#define ROUND_UP_PAGESIZE(a) (((a) + PAGEMASK) & ~PAGEMASK)
#define MMAP_ROUND_UP(a)  (((a) + ((uint64_t)PAGESIZE-1)) & ~((uint64_t)PAGESIZE-1))
#define MMAP_ROUND_DN(a)  ((a) & ~((uint64_t)PAGESIZE-1))
#define MAX(a,b) ((a>b)?a:b)

#define MEMMAP_DEBUG (1)
#undef MEMMAP_DEBUG

#ifndef true
#define true 1
#define false 0
#endif

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
Lock_Atomic_t MMapReadLock;
#else                                                                                                                  // FUSEDOS
pthread_mutex_t MMapRead_mutex;                                                                                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS
typedef struct 
{
    uint64_t filled;
    uint64_t startAddress;
    uint64_t endAddress;
} MMapRead_t;

#define MAXMMAPFILE 64
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
Lock_Atomic_t mappedFilesLock;
#else                                                                                                                  // FUSEDOS
pthread_mutex_t mappedFiles_mutex;                                                                                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS
MMapRead_t MMapRead[MAXMMAPFILE];
#if defined __FUSEDOS__                                                                                               // FUSEDOS
uint64_t MMapReadDone[MAXMMAPFILE];                                                                                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
uint64_t MMapReadDone[MAXMMAPFILE] K_ATOMIC;
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern int MMap_fileCacheClear()
{
    int x;
    for(x=0; x<MAXMMAPFILE; x++)
    {
        MMapRead[x].filled = false;
        L2_AtomicStore(&MMapReadDone[x], 0);
    }
    return 0;
}
#endif                                                                                                                 // FUSEDOS

extern int MMap_readFile(void* address, uint64_t mm_fd, uint64_t mm_ofs, uint64_t mm_len)
{
    int rc;
    int x;
    uint64_t readOffset = mm_ofs;
    uint64_t bytesLeft = mm_len;
    char*    readBuffer = (char*)address;
    bool     slotSet = false;
    bool     waitComplete = false;
    uint64_t slot = 0;
    if((!VMM_IsAppProcessUniqueAddress((void*)((uint64_t)address + mm_ofs), mm_len)) && 
       (VMM_IsAppAddress((void*)((uint64_t)address + mm_ofs), mm_len)))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Lock(&MMapReadLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_lock( &MMapRead_mutex );                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
        for(x=0; x<MAXMMAPFILE; x++)
        {
            if((MMapRead[x].filled) && 
               (MMapRead[x].startAddress >= (uint64_t)address + mm_ofs) && 
               (MMapRead[x].endAddress <= (uint64_t)address + mm_len + mm_ofs))
            {
                slotSet = true;
                slot = x;
                waitComplete = true;
                break;
            }
        }
        if(slotSet == false)
        {
            for(x=0; x<MAXMMAPFILE; x++)
            {
                if(MMapRead[x].filled == false)
                {
                    MMapRead[x].filled = true;
                    MMapRead[x].startAddress = (uint64_t)address + mm_ofs;
                    MMapRead[x].endAddress   = (uint64_t)address + mm_ofs + mm_len;
                    slotSet = true;
                    slot = x;
                    waitComplete = false;
                    break;
                }
            }
        }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&MMapReadLock);
#else                                                                                                                  // FUSEDOS
		pthread_mutex_unlock( &MMapRead_mutex );                                                                       // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    }
    
    if(waitComplete)
    {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        while(&MMapReadDone[slot] == 0)                                                                                // FUSEDOS
#else                                                                                                                  // FUSEDOS
        while(L2_AtomicLoad(&MMapReadDone[slot]) == 0)
#endif                                                                                                                 // FUSEDOS
        {
        }
        ppc_msync();
    }
    else
    {
        memset( (void *)address, 0, ROUND_UP_PAGESIZE( mm_len ) );
        
        do
        {
            rc = internal_pread64(mm_fd, readBuffer, bytesLeft, readOffset);
            if (CNK_RC_IS_FAILURE(rc)) {
                return rc;
            }
            uint64_t nbytes = (uint64_t) CNK_RC_VALUE(rc);
            if (nbytes == 0) 
            {
                break;
            }
            
            bytesLeft -= nbytes;
            readBuffer += nbytes;
            readOffset += nbytes;
        }
        while (bytesLeft > 0);
        
        if(slotSet)
        {
            ppc_msync();
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            MMapReadDone[slot] = 1;                                                                                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
            L2_AtomicStore(&MMapReadDone[slot], 1);
#endif                                                                                                                 // FUSEDOS
        }
    }
    
	// TODO FusedOS: the following code needs review
    char filename[APP_MAX_PATHNAME];
    // need to lookup the filename on the ionode using /proc/self since CNK does not internally store-away
    // the filename after the open.
    snprintf(filename, sizeof(filename), "/proc/self/fd/%d", File_GetRemoteFD(mm_fd));
    int i;
    AppProcess_t* proc = GetMyProcess();
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&mappedFilesLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_lock( &mappedFiles_mutex );                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    for(i=0; i<CONFIG_NUM_MAPPED_FILENAMES; i++)
    {
        if(((uint64_t)address > proc->mappedFiles[i].vaddr) && ((uint64_t)address < proc->mappedFiles[i].vaddr + proc->mappedFiles[i].size))
        {
            proc->mappedFiles[i].size = (uint64_t)address - proc->mappedFiles[i].vaddr;
        }
    }
    for(i=0; i<CONFIG_NUM_MAPPED_FILENAMES; i++)
    {
        if(proc->mappedFiles[i].vaddr == 0)
        {
            proc->mappedFiles[i].isShar= !VMM_IsAppProcessUniqueAddress((void*)((uint64_t)address + mm_ofs), mm_len);
            proc->mappedFiles[i].fd    = mm_fd;
            proc->mappedFiles[i].off   = mm_ofs;
            proc->mappedFiles[i].vaddr = (uint64_t)address;
            proc->mappedFiles[i].size  = mm_len;
#if (CONFIG_AVOID_READLINK==0) 
            // Note: /proc/self/fd/* file descriptors for sysiod are owned by root and readlink from 
            //       compute nodes returns EACCESS.  Trying another method.
            rc = internal_readlink(filename, proc->mappedFiles[i].filename, sizeof(proc->mappedFiles[i].filename));
#else
            if(proc->LAST_OPEN_FD == File_GetRemoteFD(mm_fd))
            {
                strncpy(proc->mappedFiles[i].filename, proc->LAST_OPEN_FILENAME, sizeof(proc->mappedFiles[i].filename));
            }
#endif
            break;
        }
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&mappedFilesLock);
#else                                                                                                                  // FUSEDOS
	pthread_mutex_unlock( &mappedFiles_mutex );                                                                        // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    
    return 0;
}

//
// Begin Debug/Trace code
//
extern inline void MMap_showByAddrList( MmapMgr_t *mgr )
{
   int i;
    uint64_t addr = 0;
    MMapChunk_t *ch = mgr->ByAddr_anchor;

    printf("MMap_showByAddrList: start ch=%p\n", ch );
   for ( i = 0 ; ch ; ch = ch->byaddr_next, i++ )
      {
        printf("[%3d]: 0x%08lx@0x%08lx %s %s\n", i, ch->size, ch->addr,
               ((ch->addr > addr) ? "Ok" : "ERROR"),
             ((ch->addr & MMAPCHUNK_ADDR_FREE) ? "Free" : "Busy") );
        addr = ch->addr;
    }
    printf("MMap_showByAddrList: done.\n");

}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern inline void MMap_showStats( MmapMgr_t *mgr )
{
    printf("MMap_showStats: mem_start = 0x%08lx, mem_size = 0x%08lx\n",
         mgr->mem_start, mgr->mem_size );
    printf("MMap_showStats: mem_avail = 0x%08lx, mem_busy = 0x%08lx.\n",
         mgr->mem_avail, mgr->mem_busy );

    MMap_showByAddrList( mgr );
}

extern inline void MMap_showBusy( MmapMgr_t *mgr )
{
    printf("MMap_showStats: mem_avail = %lu, mem_busy = %lu.\n",
         mgr->mem_avail, mgr->mem_busy );
}
//
// End Debug/Trace code
//

__INLINE__ int  MMapMgr_BusyAddrHash( uint64_t v )
{
   int a,b;

   // Large allocations given equal weight as small allocations
   a = ((v & 0x03FC0000) >> 18); // big ones
   b = ((v & 0x00007F80) >>  7); // small ones

   return( (a + b) & 0xFF );
}


__INLINE__ int  MMapMgr_GetBucket( uint64_t v )
{
   int b = cntlz64(v);

   if ( b == 0 )
      return( 31 );
   else if ( b > 27 )
      return( 5 );
   else
      return( 32 - b );
}


__INLINE__ void  MMapMgr_InitQByAddr( MMapChunk_t *chunk )
{
   chunk->byaddr_next = NULL;
   chunk->byaddr_prev = NULL;
}
#endif                                                                                                                 // FUSEDOS


__INLINE__ void  MMapMgr_EnQByAddrAfter( MmapMgr_t *mgr,
                                              MMapChunk_t *n,
                                              MMapChunk_t *o )
{
   if ( n->byaddr_next || n->byaddr_prev )
        printf("(E) MMapMgr_EnQByAddrAfter: Internal Error: chunk already queued byaddr_next/byaddr_prev.\n");

   if ( n->addr < o->addr )
      {
        printf("(E) MMapMgr_EnQByAddrAfter: Internal Error: _EnQByAddr: new < old.\n");
    }

   if ( ! mgr->ByAddr_anchor )
      mgr->ByAddr_anchor = n;

   if ( (n->byaddr_next = o->byaddr_next) )
      n->byaddr_next->byaddr_prev = n;
   o->byaddr_next = n;
   n->byaddr_prev = o;
}


__INLINE__ void  MMapMgr_EnQByAddrBefore( MmapMgr_t *mgr,
                                               MMapChunk_t *n,
                                               MMapChunk_t *o )
{
   if ( n->byaddr_next || n->byaddr_prev )
        printf("(E) MMapMgr_EnQByAddrBefore: Internal Error: chunk already queued byaddr_next/byaddr_prev.\n");

   if ( o->addr < n->addr )
      {
        printf("(E) MMapMgr_EnQByAddrBefore: Internal Error: _EnQByAddr: old < new.\n");
    }

   if ( mgr->ByAddr_anchor == o )
      mgr->ByAddr_anchor = n;

   if ( (n->byaddr_prev = o->byaddr_prev) )
      n->byaddr_prev->byaddr_next = n;
   n->byaddr_next = o;
   o->byaddr_prev = n;
}


__INLINE__ void  MMapMgr_DeQByAddr( MmapMgr_t *mgr, MMapChunk_t *c )
{
   if ( mgr->ByAddr_anchor == c )
      {
      if ( c->byaddr_prev )
         {
            printf("(W) MMapMgr_DeQByAddr: Internal Error: ByAddr_anchor was not at head!\n");
         mgr->ByAddr_anchor = c->byaddr_prev;
         }
      else if ( c->byaddr_next )
         {
         mgr->ByAddr_anchor = c->byaddr_next;
         }
      else
         {
         mgr->ByAddr_anchor = NULL_MMAP_CHUNK;
         }
      }

   if ( c->byaddr_prev )
      c->byaddr_prev->byaddr_next = c->byaddr_next;

   if ( c->byaddr_next )
      c->byaddr_next->byaddr_prev = c->byaddr_prev;

   c->byaddr_next = NULL_MMAP_CHUNK;
   c->byaddr_prev = NULL_MMAP_CHUNK;
}


//
// MMap_Push_FreeChunk: move a chunk to the free pool.
//
//   Chunk must have already been dequeued from any queues it may be on!
//
__INLINE__ void  MMap_Push_FreeChunk( MmapMgr_t *mgr, MMapChunk_t *pch )
{
#if MEMMAP_DEBUG
   if ( pch->next || pch->prev )
      printf("(E) MMap_Push_FreeChunk: chunk still queued next/prev.\n" );

   if ( pch->byaddr_next || pch->byaddr_prev )
      printf("(E) MMap_Push_FreeChunk: chunk still queued byaddr_next/byaddr_prev.\n");

   if( pch->addr || pch->size )
      printf("(E) MMap_Push_FreeChunk: chunk addr=0x%08x, size=%u.\n", pch->addr, pch->size );
#endif // MEMMAP_DEBUG

   pch->next = mgr->FreeChunk_anchor;
   mgr->FreeChunk_anchor = pch;
   mgr->chunks_left++;
}


//
// MMap_Pop_FreeChunk: return a chunk to the free pool.
//
__INLINE__ MMapChunk_t  *MMap_Pop_FreeChunk( MmapMgr_t *mgr )
{
    MMapChunk_t *pch = mgr->FreeChunk_anchor;

   if ( pch )
      {
      mgr->FreeChunk_anchor = pch->next;

      pch->next = pch->prev = NULL_MMAP_CHUNK;

      mgr->chunks_left--;

#if MEMMAP_DEBUG
      if ( pch->next || pch->prev )
         printf("(E) MMap_Pop_FreeChunk: chunk still queued next/prev.\n" );

      if ( pch->byaddr_next || pch->byaddr_prev )
         printf("(E) MMap_Pop_FreeChunk: chunk still queued byaddr_next/byaddr_prev.\n");

      if( pch->addr || pch->size )
         printf("(E) MMap_Pop_FreeChunk: chunk addr=0x%08x, size=%u.\n", pch->addr, pch->size );
#endif // MEMMAP_DEBUG
      }

   return( pch );
}


// Flush L1-D Cache over Mgr and Release Lock
static void  MMapMgr_Export( MmapMgr_t *mgr )
{
    AppProcess_t *pAppProc = GetMyProcess();
   // flush L1-D
    // dcache_flush_bytes( (uint64_t)mgr, ROUND_DN_L1_CACHELINE(sizeof(MmapMgr_t)) );

    TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Avail=0x%016lx=~%luM, Busy=0x%016lx=~%luM, chunks=%lu.\n",
                                  __func__, ProcessorID(),
              mgr->mem_avail, (mgr->mem_avail / (1024 * 1024)),
              mgr->mem_busy,  (mgr->mem_busy  / (1024 * 1024)),
              mgr->chunks_left ));

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   TRACEV_FCN( TRACE_MemAlloc, {
                 MMap_showByAddrList( mgr );
                                         } );
#endif                                                                                                                 // FUSEDOS

   ppc_msync();

   // release mmap allocator lock
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Unlock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(&pAppProc->MMap_mutex);                                                                       // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}


//
// Obtain MmapMgr Lock and Invalidate L1-D Cache over Mgr
//
static MmapMgr_t  *MMapMgr_Import( void )
{
    int hwtindex = ProcessorID();
    AppProcess_t *pAppProc = GetMyProcess();
    MmapMgr_t *mgr = &pAppProc->MmapMgr;

   // invalidate L1-D
    // dcache_invalidate_bytes( (uint64_t)mgr, ROUND_DN_L1_CACHELINE(sizeof(MmapMgr_t)) );

   ppc_msync();

   // grab mmap allocator lock (dumb, no timeout!)
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    Kernel_Lock(&pAppProc->MMapLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_lock(&pAppProc->MMap_mutex);                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Avail=0x%016lx=~%luM, Busy=0x%016lx=~%luM, chunks=%lu.\n",
                                  __func__, hwtindex,
              mgr->mem_avail, (mgr->mem_avail / (1024 * 1024)),
              mgr->mem_busy,  (mgr->mem_busy  / (1024 * 1024)),
              mgr->chunks_left ));

   return( mgr );
}

//
// MMapMgr_CombineNeighbors 
// 
//  Attempt to merge this chunk of memory with up to 2 neighboring chunks
//
static MMapChunk_t * MMapMgr_CombineNeighbors(MmapMgr_t *mgr, MMapChunk_t* ch)
{
   MMapChunk_t *temp_ch;
   int ch_busystate = ((ch->addr & MMAPCHUNK_ADDR_FREE) ? 0 : 1);

   // first look at the previous chunk
   if ( (temp_ch = ch->byaddr_prev) )
   {
      int prev_busystate = ((temp_ch->addr & MMAPCHUNK_ADDR_FREE) ? 0 : 1);
      if (ch_busystate == prev_busystate)
      {
         // Busy states match. We can combine chunks
         //printf("Phase 1 Combine %08x - %08x with %08x - %08x busystate=%d\n",
         //       temp_ch->addr, temp_ch->addr+temp_ch->size-1,
         //       ch->addr, ch->addr+ch->size-1, ch_busystate);
         temp_ch->size += ch->size;
         ch->addr = 0;
         ch->size = 0;
         // dequeue from the active list
         MMapMgr_DeQByAddr(mgr, ch);
         // add to the free chunk list
         MMap_Push_FreeChunk(mgr,ch);
         // make this chunk the current chunk
         ch = temp_ch;
      }
   }
   // Now look at the next chunk
   if ( (temp_ch = ch->byaddr_next) )
   {
      int next_busystate = ((temp_ch->addr & MMAPCHUNK_ADDR_FREE) ? 0 : 1);
      if (ch_busystate == next_busystate)
      {
         // Busy states match. We can combine chunks
         //printf("Phase 2 Combine %08x - %08x with %08x - %08x busystate=%d\n",
         //       ch->addr, ch->addr+ch->size-1,
         //      temp_ch->addr, temp_ch->addr+temp_ch->size-1, ch_busystate);
         ch->size += temp_ch->size;
         temp_ch->addr = 0;
         temp_ch->size = 0;

         // dequeue the next_ch from the active list since ch now covers this memory
         MMapMgr_DeQByAddr(mgr, temp_ch);
         // add to the free chunk list
         MMap_Push_FreeChunk(mgr, temp_ch);
      }
   }
   return ch;
}

//
// MMapMgr_FreeMemory: return memory to the free memory pool
//
//  Attempt to merge this chunk of memory with up to 2 neighboring chunks
//
static MMapChunk_t *  MMapMgr_FreeMemory( MmapMgr_t *mgr,
                                          MMapChunk_t* chunk,
                                          size_t len, 
                                          uint64_t addr)
{
   MMapChunk_t *t;
   int freeError = false;
   size_t rbytes = MMAP_ROUND_UP(len);

   if ( !chunk->addr || !chunk->size )
      {
        printf("(E) MMapMgr_FreeMemory[%d]: chunk->addr = 0x%08lx chunk->size = 0x%08lx\n",
               ProcessorID(), chunk->addr, chunk->size );
      }

   if ( chunk->addr & MMAPCHUNK_ADDR_FREE )
        printf("(E) MMapMgr_FreeMemory[%d]: chunk already marked free. addr=0x%08lx\n",
               ProcessorID(), chunk->addr );

   // is this a partial free, i.e. do we need to create additional chunks
   if (chunk->size > rbytes)
   {
      //printf("Partial Free: chunk_addr: %08x chunk_size: %08x free_addr: %08x free_size: %08x\n",
      //       chunk->addr, chunk->size, addr, rbytes);
      // do we need to add a chunk of busy memory on the front end
      if (chunk->addr < addr)
      {
         // get a chunk off the free list
         if ( (t = MMap_Pop_FreeChunk(mgr)) )
         {
            t->addr = chunk->addr; // set the address field
            t->size = addr - chunk->addr; // set size equal to difference between free start and chunk start
            chunk->addr += t->size; // move start address of the original chunk
            chunk->size -= t->size; // decrement the size of the original chunk
            MMapMgr_EnQByAddrBefore(mgr, t, chunk); // enqueue the new chunk 
            //printf("Create busy chunk before. chunk_addr: %08x chunk_size: %08x\n",t->addr, t->size);
         }
         else
         {
            
            // We have a design deficiency here. If we used up all of our free chuncks when allocating, we may not be 
            // able to free a block of memory since we need a free chunk to free allocations!
            //printf("(E) MmapMgr_FreeMemory[%d]: (1) Failed to obtain chunk from the free list\n", ProcessorID() );
            Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCFRFL, (uint64_t)addr, len, 0, 0);

             // Set indicator to force this entire chunk to remain busy since portions of the memory are still allocated
            freeError = true;
         }
      }
      // Do we need to add a chunk of busy memory at the end of the free range? 
      if (chunk->addr+chunk->size >  (addr+rbytes))
      {
         // get a chunk off the free list
         if ( (t = MMap_Pop_FreeChunk(mgr)) )
         {
            t->addr = chunk->addr + rbytes; // set address for remaining chunk.
            t->size = chunk->size - rbytes; // set size of the remaining chunk.
            chunk->size -= t->size;      // decrement chunk size by the remaining chunk size (should equal rbytes)
            MMapMgr_EnQByAddrAfter(mgr, t, chunk); // enqueue the new chunk
            //printf("Create busy chunk after. chunk_addr: %08x chunk_size: %08x\n",t->addr, t->size);

         }
         else
         {
             // We have a design deficiency here. If we used up all of our free chuncks when allocating, we may not be 
             // able to free a block of memory since we need a free chunk to free allocations!
             //printf("(E) MmapMgr_FreeMemory[%d]: (2) Failed to obtain chunk from the free list\n", ProcessorID() );
             Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCFRFL, (uint64_t)addr, len, 0, 0);

             // Set indicator to force this entire chunk to remain busy since portions of the memory are still allocated
            freeError = true;
         }
      }
   }
   // mark the chunk as free if there was no problem carving up the free/busy space
   if (!freeError)
   {
      chunk->addr |= MMAPCHUNK_ADDR_FREE;
      if (addr < mgr->low_unmap)
      {
          mgr->low_unmap = addr;
      }
      if ((addr+rbytes) > mgr->high_unmap)
      {
          mgr->high_unmap = addr+rbytes;
      }
   }
   // can we merge this chunk with free neighbors?
   chunk = MMapMgr_CombineNeighbors(mgr,chunk);

   // Return the chunk that now represents the freed block after any merging that may have occurred. The orginal chunk passed into
   // this function may have been enqueued onto the freeChunk list.
   return chunk; 
}

// low-level malloc: rbytes pre-checked and new_chunk pre-allocated
static void  *MMapMgr_Malloc( MmapMgr_t *mgr,
                                   size_t rbytes,
                                   MMapChunk_t *new_chunk )
{
    MMapChunk_t *ch;
    uint64_t cbase = 0;
    uint64_t climit = 0;
    uint64_t returnAddr = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    AppProcess_t *pProc = GetMyProcess();
#endif                                                                                                                 // FUSEDOS

    //MMap_showByAddrList( mgr );
  for ( ch = mgr->ByAddr_anchor; ch ; ch = ch->byaddr_next )
  {
    cbase = (ch->addr & ~MMAPCHUNK_ADDR_FREE);
    climit = (cbase + ch->size - 1);

    // Found a free chunk of appropriate size?
    if ( (ch->addr & MMAPCHUNK_ADDR_FREE) &&
         (ch->size >= rbytes) )
    {
      break;

    }  // End free chunk found of appropriate size

  }  // End loop thru chunks trying to find free chunk

  // Was a corresponding chunk found?
  if ( ch )
  {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    // Determine if the requested allocation will push the guard page into a stack conflict.
    // Note that we can accurately make this test only when running on the same core as the process leader 
    // thread because if we are not running in the same core, we cannot access the dynamically changing 
    // location of the process leader stack. We will use the saved R1 value even if we are not 
    // on the same core since it does represent a recent indication of the process leader stack.
    HWThreadState_t *pHwtState = GetHWThreadStateByProcessorID( GetProcessLeaderProcessorID());
    uint64_t curProcLeaderStack = pHwtState->SchedSlot[CONFIG_SCHED_SLOT_FIRST]->Reg_State.gpr[1]; // process leader R1
    // The process leader may have switched to a user stack within heap storage. Determine if the current stack pointer
    // on the process leader is on the 'stack" side of the current guard page. If it is, then we are not
    // on a special user stack and therefore we can continue with the conflict checking.
    if((pProc->Guard_Enable) && (curProcLeaderStack > (pProc->Guard_Start+pProc->Guard_Size)))
    {
        // Now test to see if the requested allocation will push the guard area into the active stack of the process leader 
        if (curProcLeaderStack < (ch->addr + rbytes + pProc->Guard_Size))
        {
            // A conflict was detected. The requested allocation will overlap the process leader stack
            MMap_Push_FreeChunk( mgr, new_chunk ); // return the unused chunk
            return( NULL );  // Return null to indicate an allocation failure
        }
    }
#endif                                                                                                                 // FUSEDOS
    // Is the chunk larger than what is necessary?
    if ( ch->size > rbytes )
    {
      // split this chunk into ch (the allocation) and new_chunk (the left-over)
      new_chunk->addr = ch->addr + rbytes;
      new_chunk->size = ch->size - rbytes;
      ch->size        = rbytes;
      new_chunk->addr |= MMAPCHUNK_ADDR_FREE;  // Indicate this is a free chunk
            MMapMgr_EnQByAddrAfter( mgr, new_chunk, ch );

    }  // End chunk too big, return extra to free pool
    else
    {
      // we didn't need new_chunk afterall, so free it
      MMap_Push_FreeChunk( mgr, new_chunk );

    }  // Matching size so free up the new_chunk

    // clear the free flag
    ch->addr &= ~MMAPCHUNK_ADDR_FREE;
    // Adjust the amount of memory 
    mgr->mem_avail -= ch->size;
    mgr->mem_busy += ch->size;
    // Save the address of this new allocation prior to merging of chunks
    returnAddr = ch->addr;

    // Determine if we can combine this chunk with an adjacent busy chunks
    ch = MMapMgr_CombineNeighbors(mgr,ch);
  }  // End corresponding chunk found
  else
  {
    MMap_Push_FreeChunk( mgr, new_chunk );
    return( NULL );
  }

  //MMap_showByAddrList( mgr );
  return( (void *)returnAddr );

}


// low-level malloc: rbytes pre-checked and new_chunk pre-allocated
static void  *MMapMgr_Malloc_Addr( MmapMgr_t *mgr,
                                        size_t rbytes,
                                        MMapChunk_t *new_chunk,
                                        uint64_t addr )
{
    MMapChunk_t *ch;
    size_t rlimit = (addr + rbytes - 1);
    uint64_t cbase = 0;
    uint64_t climit = 0;
    uint64_t busychunk;

  // For MAP_FIXED allocations, check to see if the allocation was previously mapped.  If it was,
  // need to free the passed in chunk and just return address as only one mapping is necessary to track
  for ( ch = mgr->ByAddr_anchor; ch ; ch = ch->byaddr_next )
  {
    cbase = (ch->addr & ~MMAPCHUNK_ADDR_FREE);
    climit = (cbase + ch->size - 1);

    if ( (cbase == addr) &&
         (climit == rlimit) )
    {
      ch->addr &= ~MMAPCHUNK_ADDR_FREE;  // Mark allocated as busy
      // we didn't need new_chunk afterall, so free it
      MMap_Push_FreeChunk( mgr, new_chunk );

      // Combine neighboring chunks if possible
      ch = MMapMgr_CombineNeighbors(mgr,ch);

      // Return passed in address
      return( (void *)addr );

    }  // End matching chunk found

  }  // End loop thru looking for already allocated chunk

  cbase = climit = 0;

  // search thru allocations to get appropriate location for MAP_FIXED request
  /* NOTE: MAP_FIXED says to allocate at the specific address.  The range check already occured in the
           calling routine.  So a search for the correct block if it has been mapped is done and the new
           request is inserted between the appropriate addresses.
   */
  for ( ch = mgr->ByAddr_anchor; ch ; ch = ch->byaddr_next )
  {
    cbase = (ch->addr & ~MMAPCHUNK_ADDR_FREE);
    climit = (cbase + ch->size - 1);

    // Found a chunk in the appropriate range?
    if ( ( cbase <= addr ) &&
         ( climit >= rlimit ) )
    {
      break;

    }  // End chunk found containing the address range

  }  // End loop thru chunks trying to find address range

  // Was a corresponding chunk found?
  if ( !ch )
  {
    MMap_Push_FreeChunk( mgr, new_chunk );
    return( NULL );
  }

    MMapChunk_t *prevch = NULL;
  busychunk = ((ch->addr & MMAPCHUNK_ADDR_FREE) ? 0 : 1);

  // Was this a free chunk?
  if ( !busychunk )
  {
    // Does the allocation occur before address wanted?
    if ( cbase < addr )
    {
      if ( (prevch = MMap_Pop_FreeChunk(mgr)) )
      {
        // Adjust previous chunk
        prevch->addr = ch->addr;  // Set to the original address with the free bit set
        prevch->size = addr - (prevch->addr & ~MMAPCHUNK_ADDR_FREE); // Do not include the FREE flag in the calculation

        // Adjust existing chunk
        ch->addr = addr;
        ch->size -= prevch->size;

        // Note: Just doing EnQ of the chunk.  Previous state is maintained for free/busy chunk
                MMapMgr_EnQByAddrBefore( mgr, prevch, ch );

            }
      else
      {
        return( NULL );
      }

    }  // End previous allocation needs to be adjusted

    // Is the chunk larger than what is necessary?
    if ( ch->size > rbytes )
    {
      // split this chunk into ch (the allocation) and new_chunk (the left-over)
      new_chunk->addr = ch->addr + rbytes;
      new_chunk->size = ch->size - rbytes;
      ch->size        = rbytes;

      new_chunk->addr |= MMAPCHUNK_ADDR_FREE;  // Since was not busy, indicate chunk is free
            MMapMgr_EnQByAddrAfter( mgr, new_chunk, ch );

    }  // End chunk too big, return extra to free pool
    else
    {
      // we didn't need new_chunk afterall, so free it
      MMap_Push_FreeChunk( mgr, new_chunk );

    }  // Matching size so free up the new_chunk

    ch->addr &= ~MMAPCHUNK_ADDR_FREE;  // Mark allocated as busy
    // Adjust the amount of memory
    mgr->mem_avail -= ch->size;
    mgr->mem_busy += ch->size;

    // Determine if our new busy chunk can be combined with neighboring chunks
    ch = MMapMgr_CombineNeighbors(mgr,ch);

  }  // End free chunk being adjusted
  else
  {
     // The requested allocation fits within a block that is already marked as busy.
     // We didn't need new_chunk afterall, so free it
     MMap_Push_FreeChunk(mgr, new_chunk);

  }  // End original chunk was busy

    //MMap_showByAddrList( mgr );

  // return the passed in address
  return( (void*)addr );

}


//
// Public routines:
//
void  MMap_Init( uint64_t start, size_t bytes )
{
    int i;
    MmapMgr_t *mgr = &GetMyProcess()->MmapMgr;
    MMapChunk_t *pch;

   if ( !bytes )
      return;

   mgr->mem_start   = MMAP_ROUND_UP( start );
   mgr->mem_size    = MMAP_ROUND_DN( bytes );
   mgr->mem_avail   = mgr->mem_size;  
   mgr->mem_busy    = 0;
   mgr->chunks_left = 0;
   
   Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCINIT, mgr->mem_start,  mgr->mem_size, mgr->mem_avail, mgr->mem_busy);
   
   // init the free chunk pool
   for ( i = 0 ; i < MMAP_MAX_ALLOCS ; i++ )
   {
      pch = &(mgr->chunks[i]);
      MMap_Push_FreeChunk( mgr, pch );
   }
   if ( (pch = MMap_Pop_FreeChunk( mgr )) )
   {
      pch->addr = mgr->mem_start;
      pch->size = mgr->mem_size;
      //pch->prot = 0;

      mgr->ByAddr_anchor = pch;
//printf("calling free memory. pch:%016lx size:%016lx addr:%016lx\n",(uint64_t)pch, pch->size, pch->addr); 
      MMapMgr_FreeMemory( mgr, pch, pch->size, pch->addr); 
      // These must be initialized after the above call to FreeMemory
      mgr->low_unmap   = mgr->mem_start+mgr->mem_size;
      mgr->high_unmap  = mgr->mem_start;
   }
}


void  *MMap_Malloc( size_t bytes )
{
    MmapMgr_t *mgr = MMapMgr_Import();
    MMapChunk_t *new_chunk;
    size_t rbytes = MMAP_ROUND_UP( bytes );
   void *rc_mem = (void *)0;

    TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Allocate 0x%08lx bytes.\n",
                                  __func__, ProcessorID(), bytes ));

   if ( (bytes > 0) && (mgr->mem_avail >= rbytes) )
      {
      // get chunk from free chunk pool (if none, fail allocation)
      if ( (new_chunk = MMap_Pop_FreeChunk(mgr)) )
         {
            rc_mem = MMapMgr_Malloc( mgr, rbytes, new_chunk );
            
            if ( rc_mem )
            {
                if ((uint64_t)rc_mem+rbytes > mgr->high_mark)
                {
                     mgr->high_mark = (uint64_t)rc_mem+rbytes;
                     if (mgr->high_mark > mgr->high_mark_max)
                     {
                         mgr->high_mark_max = mgr->high_mark;
                     }
                }
            }
        }
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   TRACEV_FCN( TRACE_MemAlloc,
         ({
          if ( !rc_mem )
             {
             printf("Malloc: No Memory for 0x%08lx bytes, avail=0x%08lx.\n", bytes, mgr->mem_avail );
                 MMap_showByAddrList( mgr );
                 }
          }));
#endif                                                                                                                 // FUSEDOS
   
   Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCANON, bytes, rbytes, mgr->mem_avail, (uint64_t)rc_mem);
   
   MMapMgr_Export( mgr );
   
//   if ( rc_mem )
//      memset( rc_mem, 0, rbytes );

   return( rc_mem );
}


void  *MMap_Malloc_Addr( size_t bytes, uint64_t addr )
{
    MmapMgr_t *mgr = MMapMgr_Import();
    MMapChunk_t *new_chunk;
    size_t rbytes = MMAP_ROUND_UP( bytes );
   void *rc_mem = (void *)0;

    TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Allocate 0x%08lx bytes.\n",
                                  __func__, ProcessorID(), bytes ));

// \note MAP_FIXED indicates to reallocate mapping.  So limit check should be sufficient
   if ( ( addr >= mgr->mem_start ) && ( (addr + bytes) < (mgr->mem_start + mgr->mem_size) ) )
   {
      // get chunk from free chunk pool (if none, fail allocation)
      if ( (new_chunk = MMap_Pop_FreeChunk(mgr)) )
      {
          rc_mem = MMapMgr_Malloc_Addr( mgr, rbytes, new_chunk, addr );
          if ( rc_mem )
          {
              if ((uint64_t)rc_mem+rbytes > mgr->high_mark)
              {
                   mgr->high_mark = (uint64_t)rc_mem+rbytes;
                   if (mgr->high_mark > mgr->high_mark_max)
                   {
                       mgr->high_mark_max = mgr->high_mark;
                   }
              }
          }
      }
   }
   else 
   {
      // This may be an mmap-fixed request being made by ld.so loading main. This is done within a specially mapped address range
      // outside of the normal MMAP start/end range since its address is not relocatable.
        uint64_t vaddr_start = 0;
        uint64_t paddr_start = 0;
        size_t vsize = 0;
        uint64_t vaddr_start2 = 0;
        uint64_t paddr_start2 = 0;
        size_t vsize2 = 0;
        AppProcess_t* proc = GetMyProcess();
        vmm_getSegment(proc->Tcoord, IS_TEXT,&vaddr_start, &paddr_start, &vsize);
        vmm_getSegment(proc->Tcoord, IS_DATA,&vaddr_start2, &paddr_start2, &vsize2);

      // see if the requested address is within the range of the IS_DYNAM segment
      if ((addr >= vaddr_start) && ((addr+bytes) < (vaddr_start+vsize+vsize2)))
      {
         // Yes, we are in the IS_DYNAM range. Return the requested address. Do not track that allocation in our
         // chunks since it is outside of the mmap range
         rc_mem = (void*)addr;
      }
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   TRACEV_FCN( TRACE_MemAlloc,
                    ({
                     if ( !rc_mem ){
                     printf("Malloc_Addr: No Memory for 0x%016lx bytes, avail=0x%016lx.\n", bytes, mgr->mem_avail );
                 MMap_showByAddrList( mgr );
                 }
                    }));
#endif                                                                                                                 // FUSEDOS

   Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCADDR, bytes, addr, mgr->mem_avail, (uint64_t)rc_mem);
   
   MMapMgr_Export( mgr );
   
   return( rc_mem );
}


int  MMap_Free( void *addr, size_t len )
{
    MmapMgr_t *mgr = MMapMgr_Import();
    //MMapChunk_t *chunk;
    uint64_t rbase  = (uint64_t)addr;
   uint64_t rbytes = MMAP_ROUND_UP(len);
    uint64_t rlimit = (rbase + len - 1);
   int rc = 0;
    uint64_t newMMap_HighMark = mgr->mem_start; // initialize to beginning of mmap storage

    TRACE( TRACE_MemAlloc, ("(I) %s[%d]: Free 0x%016lx thru 0x%016lx.\n",
                                  __func__, ProcessorID(), rbase, rlimit ));

    MMapChunk_t *ch;
    uint64_t cbase = 0;
    uint64_t climit = 0;

  // Loop thru looking for address to free
  for ( ch = mgr->ByAddr_anchor; ch ; ch = ch->byaddr_next )
  {
    cbase = (ch->addr & ~MMAPCHUNK_ADDR_FREE);
    climit = (cbase + ch->size - 1);

    // Is the requested free range contained within this chunk 
    // note: previously checked for exact size match; now support partial munmaps
    if ((rbase >= cbase) && (rlimit <= climit))
    {
      // Adjust the amount of memory
      mgr->mem_avail += rbytes;
      mgr->mem_busy -= rbytes;  // SHOK
      // Free the memory and try to merge free chunks. Update ch so that it is a chunk on our chain since
      // the original ch may be moved to the free list if it was merged with a neighboring chunk.
      ch = MMapMgr_FreeMemory( mgr, ch, rbytes, rbase);  // Free the memory and try to merge free chunks
      // we must check the previous chunk for the tally of the max allocated address since it may be a new 
      // busy chunk generated by the previous call to free memory if it was a partial free.
      MMapChunk_t *pch = ch->byaddr_prev;
      if (pch) // If exists, we know it must be a busy chunk since all free chunks would have been combined
      {
         uint64_t localHighMark = pch->addr + pch->size;
         if (localHighMark > newMMap_HighMark)
            newMMap_HighMark = localHighMark;
      }
            //MMap_showByAddrList( mgr );
    }  // End matching chunk found
    // Keep a tally of the max allocated address
    if (!(ch->addr & MMAPCHUNK_ADDR_FREE))
    {
                uint64_t localHighMark = ch->addr+ch->size;
       if (localHighMark > newMMap_HighMark)
          newMMap_HighMark = localHighMark;
    }
  }  // End loop thru chunks trying to free chunk
  mgr->high_mark = newMMap_HighMark;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  // call to conditionally adjust the guard. We are making this call before we release the
  // mmap allocator lock so that an allocation can't sneak. 
  Guard_Adjust( newMMap_HighMark, MoveGuardDown, 0);  // slide the guard down
#endif                                                                                                                 // FUSEDOS

  Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLCFREE, (uint64_t)addr, len, mgr->mem_avail, 0);
    
  MMapMgr_Export( mgr );

  return( rc );
}



