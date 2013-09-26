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
#ifndef _MMAP_H_ // Prevent multiple inclusion
#define _MMAP__H_

#if !defined(__ASSEMBLY__)

// This malloc manages cachelines (32 bytes), so
//  lower 5 bits of addr are available for flags
#define MMAPCHUNK_ADDR_FREE (0x00000001ULL)

#ifndef MAP_UNINITIALIZED
#define MAP_UNINITIALIZED 0x4000000    /* For anonymous mmap, memory could be uninitialized */
#endif

#define NULL_MMAP_CHUNK (MMapChunk_t *)0

typedef enum
{
    MoveGuardUp = 0,
    MoveGuardDown = 1
} MoveGuardDirection_t;

typedef struct {
        uint64_t high_mark;
        MoveGuardDirection_t direction;
        int returnCode;
    } GuardIpiParm_t;


// MemChunk: track free/busy memory
//  MemChunks can be in 3 states:
//   1) in the pool of free chunks
//   2) in the FreeMem_chunks list organized by log2 of size. sorted by size within the list
//   3) in BusyMem_chunks, hashed by address for quick retrieval
//  Allocations smaller than a chunk split the chunk into 2 chunks
//  Frees of chunks attempt to coalesce chunks with free neighboring chunks
//  So, all chunks have to be simultaneously free/busy and sorted by address
typedef struct MMapChunk_t
{
    struct MMapChunk_t *next,
    *prev,
    *byaddr_next,
    *byaddr_prev;

    uint64_t addr; // 32bit virtual address
    size_t   size; // size in bytes

    //uint32_t prot, // protection
    //         core; // core that allocated this
}
MMapChunk_t;


typedef struct MmapMgr_t
{
    uint64_t mem_start;
    size_t   mem_size;
    size_t   mem_avail;
    size_t   mem_busy;
    uint64_t chunks_left;
    uint64_t high_mark;
    uint64_t high_mark_max;
    uint64_t low_unmap;
    uint64_t high_unmap;


    MMapChunk_t   *FreeChunk_anchor;      // free chunks queued here
    MMapChunk_t   *ByAddr_anchor;         // busy chunks sorted by address

    MMapChunk_t   chunks[ MMAP_MAX_ALLOCS ];

    uint32_t pad[8];
}
MmapMgr_t;

#define  MPROTECT_MGR_ENTRIES ((CONFIG_AFFIN_SLOTS_PER_HWTHREAD+1) * CONFIG_HWTHREADS_PER_CORE * CONFIG_MAX_CORES)  
typedef struct MprotectMgr_t
{
    uint32_t index;
    uint64_t address[MPROTECT_MGR_ENTRIES];
    size_t length[MPROTECT_MGR_ENTRIES];
}  MprotectMgr_t;


void MMap_Init( uint64_t start, size_t bytes );
void *MMap_Malloc( size_t bytes );
void *MMap_Malloc_Addr( size_t bytes, uint64_t addr );
int MMap_Free( void *addr, size_t len );
int MMap_readFile(void* address, uint64_t mm_fd, uint64_t mm_ofs, uint64_t mm_len);
int MMap_fileCacheClear();

#endif // __ASSEMBLY__

#endif // Add nothing below this line
