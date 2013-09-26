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

#if STANDALONE
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#define TRACE(x, str) printf str
#endif
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <assert.h>                                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <arpa/inet.h>
extern "C"
{
#if STANDALONE
#include "vmm.h"
#include "cnk/include/AppState.h"
#include "hwi/include/common/bgq_bitnumbers.h"
#include "hwi/include/bqc/A2_inlines.h"
#include "firmware/include/VirtualMap.h"
#include "atomicops.h"
#define Kernel_WriteFlightLog(a,b,c,d,e,f)
#else
#include "Kernel.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "hwi/include/bqc/testint_inlines.h"
#include "spi/include/l2/atomic.h"
#else                                                                                                                  // FUSEDOS
#include "firmware/include/VirtualMap.h"                                                                               // FUSEDOS
#include "fusedos_config.h"                                                                                            // FUSEDOS
#include "fusedos.h"                                                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#endif
}

/*******************************************************
 **  Mapper Configuration
 ******************************************************/
#define ENABLE_MMU_WORKAROUNDS      false
#define ENABLE_MAMBO_WORKAROUNDS    false
#define ENABLE_L1P_PREFETCH_WORKAROUND true
#define MAX_MEMORY_PER_NODE         16384   //!< Total physical memory per node (in 1MB pages)
#define MAX_PHYSICALMEMORY_PER_NODE 65536   //!< Architectual limit of memory per node (in 1MB pages)
#define MAX_PROCESS_COUNT              67   //!< Max. processes (includes agents and 'bogus' process used to reserve memory for larged sharedmem requests)
#define MAX_CORE_COUNT                 17   //!< Max. number of cores
#define MAX_APP_CORE_COUNT             16
#define MAX_HWTHREADS_PER_CORE          4
#define MAX_SEGMENTCOUNT             (MAX_PROCESS_COUNT*2+1+1+1+8)   // (1 data + 1 ld.so text/data)*maxprocess + text + shared + shared_atomic + 8 padding
#define MAX_TLBSLOTS                  512
#define MMU_ASSOCIATIVITY               4
#define USE_EXCLUSION               (true && (!ENABLE_MMU_WORKAROUNDS))
#define USE_PARTITION_WITHOUT_HOLE  false
#define USE_PARTITION_WITH_HOLE      true
#define USE_HEAP_BALANCER           false
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#define MAP_SPECULATIVEALIASES      (false && USE_EXCLUSION)                                                           // FUSEDOS
#else                                                                                                                  // FUSEDOS
#define MAP_SPECULATIVEALIASES      (true && USE_EXCLUSION)
#endif                                                                                                                 // FUSEDOS
#define MAP_PROCESSWINDOWS          (true && (!ENABLE_MMU_WORKAROUNDS))
#define MAP_CATOMICS                false
#define MEMORY_PA_OFFSET                0
#define MEMORY_UA                       0
#define NUMTLBSIZES                     3
#define MMU_AVAILABLE_BUCKETS           MMU_ASSOCIATIVITY
#define MMU_FORCEPID0               false
#define MAX_EXTRAPROCESS_LIMIT          4
#define MMU_TLBSIZES_POW2            true
#define DO_NOT_COPY                0xffff
#define STANDALONE_MEMSIZE_DEFAULT   MAX_MEMORY_PER_NODE
#define STANDALONE_MAPOFFSET_DEFAULT    0
#define COPYPRECALCULATEDMAP         true
#define ENABLE_NONCONTIGUOUSMEM     false

#define DISPLAYTLBMAP               false

#define APPAGENT_COREID                16

const unsigned TLBsizes[NUMTLBSIZES]          = {1024, 16, 1};
const char typexlate[IS_SEGMENTTYPECOUNT][32] = {"NULL", "TEXT", "DATA", "HEAP", "SHARED", "PROCWIN", "DYNAM", "ATOMIC",
                                                 "KERNEL", "MMIO", "SPEC", "RAMDISK", "SCRUB", "USERMMIO", "COMMON"};

#if USE_EXCLUSION
#define NUMTLBSIZES_EXCL               11
const unsigned TLBsizes_excl[NUMTLBSIZES_EXCL] = {1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};
#else
#define NUMTLBSIZES_EXCL   NUMTLBSIZES
#define TLBsizes_excl      TLBsizes
#endif

/******************************************************
 ** Tuning Parameters
 *****************************************************/
#define WASTE_START                     0
#define WASTE_LIMIT                     MAX(MAX_MEMORY_PER_NODE, numsegs*1024)
#define HEAP_DELTA_THRESHOLD            8  //!< Allowed imbalance factor between heap allocations across processes    


/*******************************************************
 ** Standalone tester parameters
 ******************************************************/
#define EXHAUSTIVE          true         //!< Boolean.  Whether to search through all combinations of text/data/shm sizes or randomize
#define REVERSE            false         //!< Boolean.  Whether to searc
#define NUMPROCESSES          64         //!< Number of processes to map. 
#define TEXTLIMIT             32
#define DATALIMIT             32
#define SHMLIMIT               1

#define CONTIGUOUS_TEXT     true
#define CONTIGUOUS_DATA     true
#define CONTIGUOUS_SHM      true
#define EXIT_ON_FAILURE     true

#define SA_KERNEL_START        0x0000000
uint64_t SA_KERNEL_END       = 0x0d00000;
#define SA_KERNEL_ATOMIC_START 0x0100000
#define SA_KERNEL_ATOMIC_END   0x0180000

#define MAX_ITERATIONS         100000000
#define INITIAL_TEXTSIZE       1
#define INITIAL_DATASIZE       1
#define INITIAL_SHMSIZE        0

#define NUMPROCESSGROUPS       1        //!< HTC mode
/*************************************************************/


#define MTRACE if(lastIteration) printf

/******************************************************
 ** Handy macros
 *****************************************************/
#ifndef ALIGNUP
#define ALIGNUP(p,size) ((((unsigned long)p%size)?((unsigned long)p+size-((unsigned long)p%size)):(unsigned long)p))
#endif

#ifndef MAX
#define MAX(a,b) ((a>b)?a:b)
#endif

#ifndef MIN
#define MIN(a,b) ((a<b)?a:b)
#endif

#if STANDALONE
FILE* EraseFile = stdout;
#endif

class Segment
    {
    public:
        bool valid;
        enum SegmentType type;
        unsigned int vaddr;
        unsigned int size;
        unsigned int origorder;
        unsigned int origvaddr;
        unsigned int origsize;
        int      process;      // process# for the segment.  process -1 == shared with all processes
        unsigned int source_paddr;
        unsigned int coremask;
        unsigned int pad;
        
        bool     contains_heap;
        
        bool     pinned_vaddr; // vaddr for shared memory is picked by kernel
        bool     contiguous;   // segment must have contiguous physical pages
        bool     roundable;    // share and data sections are roundable
        bool     isMapped;
        bool     loaded;
        bool     sharedPID;
        bool     allowHeap;
        unsigned int temporary;
        
#if ENABLE_NONCONTIGUOUSMEM
        int      numassigned;
        char     assigned[MAX_TLBSLOTS + 1];
#endif
        unsigned int maxtlb;
        unsigned int maxoffset;
        
        void PrintSeg(void* mapper, int designator)
        {
            TRACE(TRACE_StaticMap, ("MAPPER%8p  AddSegment %2d:  type=%6s, vaddr=%4d, source_paddr=%4d, size=%4d proc=%x, coremask=%x, cont=%d, round=%d pinned=%d loaded=%d  sharedPID=%d\n", mapper, designator, typexlate[type], vaddr, source_paddr, (unsigned int)size, process, coremask, contiguous, roundable, pinned_vaddr, loaded, sharedPID));
        }
    };

class TLBslot
    {
    private:
        enum SegmentType type;
    public:
        uint32_t refcnt;
        uint32_t pid;
        uint32_t vaddr;
        uint32_t paddr;
        uint32_t size;
        uint32_t exclsize;
        uint32_t origsize;
        uint32_t vaddr_pinned;
        uint32_t source_paddr;
        uint32_t origvaddr;
        uint32_t loaded;
        
        enum SegmentType getType() { return type; };
        void setType(enum SegmentType _type) { type = _type; };
        
        void ToNetworkEndian()
        {
            type = (SegmentType)(htonl((unsigned int)type));
#define ENDIAN(x) x=htonl(x)
            ENDIAN(pid); ENDIAN(vaddr); ENDIAN(paddr); ENDIAN(size); ENDIAN(exclsize); ENDIAN(origsize); ENDIAN(vaddr_pinned); ENDIAN(source_paddr); ENDIAN(origvaddr); ENDIAN(loaded);
#undef ENDIAN
        }
        
        void setSegment(Segment* ptr) 
        {
            if(ptr == NULL) 
            {
                type     = IS_HEAP;
                origsize = 0;
                vaddr_pinned = false;
                source_paddr = 0;
                origvaddr    = 0;
                loaded       = false;
                refcnt       = 1;
            }
            else
            {
                refcnt   = 1;
                type     = ptr->type;
                if(ptr->contains_heap)
                {
                    if(vaddr + exclsize - ptr->vaddr < ptr->origsize)
                    {
                        origsize = MIN(size, ptr->origsize - (vaddr + exclsize - ptr->vaddr));
                    }
                    else
                    {
                        origsize = 0;
                    }
                }
                else
                {
                    origsize = size;
                }
                loaded       = ptr->loaded;
                vaddr_pinned = ptr->pinned_vaddr;
                source_paddr = ptr->source_paddr;
                origvaddr    = ptr->origvaddr;
            }
        };
    };

class TLBstate
    {
        unsigned char index[MAX_TLBSLOTS / MMU_ASSOCIATIVITY];
    public:
        static int CalculateSlot(unsigned pid32, unsigned va, unsigned tlbsize);
        int  AcquireSlot(unsigned pid32, unsigned va, unsigned tlbsize);
        void FreeSlot(int slot);

        unsigned int GetActive(int bucket) { return index[bucket]; }
        void ToNetworkEndian() { }
        
        uint64_t calculateScore(TLBstate& original)
        {
            int bucket;
            int hitcount;
            const uint32_t score[] = { 1, 10, 20, 80, 200 };
            for(bucket=0, hitcount=0; bucket<MAX_TLBSLOTS / MMU_ASSOCIATIVITY; bucket++)
            {
                if(GetActive(bucket) != original.GetActive(bucket))
                {
                    hitcount += score[ GetActive(bucket) ];
                }
            }
            return hitcount;
        }

    };

class MMU
    {
    public:
        TLBstate tlbstate;
        TLBslot  tlbarray[MAX_TLBSLOTS];
        
        int getMMUIndex();
        void ToNetworkEndian()
        {
            int x;
            tlbstate.ToNetworkEndian();  /*no effect*/
            for(x=0; x<MAX_TLBSLOTS; x++)
            {
                tlbarray[x].ToNetworkEndian();
            }
        }
        int SnapshotState(TLBstate& oldstate)
        {
            oldstate = tlbstate;
            return 0;
        }
        int RestoreState(const TLBstate& newstate)
        {
            tlbstate = newstate;
            return 0;
        }
        int ClearMMU()
        {
            memset(this, 0, sizeof(MMU));
            
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            int slot;                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
            int x, slot;
            uint64_t l2addr;
#endif                                                                                                                 // FUSEDOS
            Segment kernel;
            kernel.valid = true;
            kernel.process   = -1;
            kernel.coremask  = VMM_SEGMENTCOREMASK_ALL;
            kernel.contains_heap = false;
            kernel.pinned_vaddr = true;
            kernel.isMapped = true;
            kernel.type  = IS_KERNEL;
            kernel.loaded  = true;

            
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            // Grab a slot for the SPC monitor (kernel)                                                                // FUSEDOS
            slot = AcquireSlot(0, 0, 1); // 16 MB                                                                      // FUSEDOS
            if (slot != -1) {                                                                                          // FUSEDOS
                tlbarray[slot].vaddr = tlbarray[slot].paddr = kernel.source_paddr = kernel.origvaddr = 0;              // FUSEDOS
                tlbarray[slot].size = 16;                                                                              // FUSEDOS
                tlbarray[slot].exclsize = 0;                                                                           // FUSEDOS
                tlbarray[slot].setSegment(&kernel);                                                                    // FUSEDOS
            }                                                                                                          // FUSEDOS
#else                                                                                                                  // FUSEDOS
#if STANDALONE
            for(x=((uint64_t)SA_KERNEL_START)/CONFIG_CNK_PAGE_SIZE; (uint64_t)x<((uint64_t)SA_KERNEL_END)/CONFIG_CNK_PAGE_SIZE; x++)
#else
            for(x=((uint64_t)&__KERNEL_TEXT_START)/CONFIG_CNK_PAGE_SIZE; (uint64_t)x<(((uint64_t)&__KERNEL_END) + CONFIG_CNK_PAGE_SIZE)/CONFIG_CNK_PAGE_SIZE; x++)  /*out of range pointer*/
#endif
            {
                slot = AcquireSlot(0, x, 2);
                if(slot != -1)
                {
                    tlbarray[slot].vaddr = tlbarray[slot].paddr = kernel.source_paddr = kernel.origvaddr = kernel.vaddr = x;
                    tlbarray[slot].size = kernel.origsize = kernel.size = 1;
                    tlbarray[slot].exclsize = 0;
                    tlbarray[slot].setSegment(&kernel);
                }
            }
            
            // Kernel atomics.  
            // For standalone mode, map the first 0.5MBs
#if STANDALONE
            for(l2addr = (uint64_t)SA_KERNEL_ATOMIC_START * 32 + VA_MINADDR_CATOMICOPS; l2addr < (uint64_t)SA_KERNEL_ATOMIC_END*32 + VA_MINADDR_CATOMICOPS; l2addr+=16*1024*1024)
#else
            for(l2addr = (uint64_t)__l2_op_ptr(&__KERNEL_ATOMICS_START, 0); l2addr < (uint64_t)__l2_op_ptr(&__KERNEL_ATOMICS_END, 0); l2addr+=16*1024*1024)
#endif
            {
                slot = AcquireSlot(0, l2addr/CONFIG_CNK_PAGE_SIZE, 1);
                if(slot != -1)
                {
                    tlbarray[slot].vaddr = tlbarray[slot].paddr = kernel.source_paddr = kernel.origvaddr = kernel.vaddr = l2addr/CONFIG_CNK_PAGE_SIZE;
                    tlbarray[slot].size = kernel.origsize = kernel.size = 16;
                    tlbarray[slot].exclsize = 0;
                    tlbarray[slot].setSegment(&kernel);
                }
            }
#endif                                                                                                                 // FUSEDOS

            // Reserve slot for privileged MMIO
            kernel.type  = IS_MMIO;
            slot = AcquireSlot(0, ((VA_MINADDR_MMIO | VA_PRIVILEGEDOFFSET)/CONFIG_CNK_PAGE_SIZE), 0);
            if(slot != -1)
            {
                tlbarray[slot].vaddr = kernel.origvaddr = kernel.vaddr = ((VA_MINADDR_MMIO | VA_PRIVILEGEDOFFSET)/CONFIG_CNK_PAGE_SIZE);
                tlbarray[slot].paddr = kernel.source_paddr             = ((PHYMAP_MINADDR_MMIO | PHYMAP_PRIVILEGEDOFFSET)/CONFIG_CNK_PAGE_SIZE);
                tlbarray[slot].size  = kernel.origsize = kernel.size   = 1024;
                tlbarray[slot].exclsize = 0;
                tlbarray[slot].setSegment(&kernel);
            }

	    // Reserve slots for "fast" L1P and L2 MMIO mappings
            kernel.type  = IS_MMIO;
            slot = AcquireSlot(0, CONFIG_FAST_L1P_BASE/CONFIG_CNK_PAGE_SIZE, 2);
            if(slot != -1)
            {
                tlbarray[slot].vaddr = kernel.origvaddr = kernel.vaddr = CONFIG_FAST_L1P_BASE/CONFIG_CNK_PAGE_SIZE;
                tlbarray[slot].paddr = kernel.source_paddr             = PHYMAP_MINADDR_L1P/CONFIG_CNK_PAGE_SIZE;
                tlbarray[slot].size  = kernel.origsize= kernel.size    = 1;
                tlbarray[slot].exclsize = 0;
                tlbarray[slot].setSegment(&kernel);
            }            
            slot = AcquireSlot(0, CONFIG_FAST_L2_BASE/CONFIG_CNK_PAGE_SIZE, 2);
            if(slot != -1)
            {
                tlbarray[slot].vaddr = kernel.origvaddr = kernel.vaddr = CONFIG_FAST_L2_BASE/CONFIG_CNK_PAGE_SIZE;
                tlbarray[slot].paddr = kernel.source_paddr             = PHYMAP_MINADDR_L2/CONFIG_CNK_PAGE_SIZE;
                tlbarray[slot].size  = kernel.origsize= kernel.size    = 1;
                tlbarray[slot].exclsize = 0;
                tlbarray[slot].setSegment(&kernel);
            }            
            
#if ENABLE_MAMBO_WORKAROUNDS
            slot = AcquireSlot(0, ((0x0000600000000000LL)/CONFIG_CNK_PAGE_SIZE), 0);
            if(slot != -1)
            {
                tlbarray[slot].vaddr = kernel.origvaddr = kernel.vaddr = ((0x0000600000000000LL)/CONFIG_CNK_PAGE_SIZE);
                tlbarray[slot].paddr = kernel.source_paddr             = ((0x0000600000000000LL)/CONFIG_CNK_PAGE_SIZE);
                tlbarray[slot].size = kernel.origsize = kernel.size    = 1024;
                tlbarray[slot].exclsize = 0;
                tlbarray[slot].setSegment(&kernel);
            }
#endif
            return 0;
        }
        int AcquireSlot(unsigned pid32, unsigned va, unsigned tlbsize)
        {
            int slot = tlbstate.AcquireSlot(pid32, va, tlbsize);
            if(slot != -1)
            {
                tlbarray[slot].pid = pid32;
            }
            return slot;
        }
        void FreeSlot(int slot)
        {
            tlbstate.FreeSlot(slot);
        }
        
        int CalculateAliasPID(unsigned origpid)
        {
            int x;
            int pid;
            int set;
            int way;
            int maxway;
            int slot;
            char hit[MAX_TLBSLOTS/MMU_ASSOCIATIVITY];
            int min;
            int pidcount[MAX_TLBSLOTS / MMU_ASSOCIATIVITY];
            int tmppid;
            int tmphitcount;
            
            origpid &= 0x7f;
            for(pid=0; pid<MAX_TLBSLOTS / MMU_ASSOCIATIVITY; pid++)
            {
                pidcount[pid] = 0;
                memset(hit, 0, sizeof(hit));
		
                tmppid = pid ^ origpid;
                for(set=0; set<MAX_TLBSLOTS/MMU_ASSOCIATIVITY; set++)
                {
                    maxway = tlbstate.GetActive(set);
                    for(way=0, tmphitcount=0; way<maxway; way++)
                    {
                        slot = set*MMU_ASSOCIATIVITY + way;
                        if((tlbarray[slot].getType() != IS_DATA) &&
                           (tlbarray[slot].getType() != IS_HEAP) &&
                           (tlbarray[slot].getType() != IS_SHAR))
                            continue;
                        
                        if((tlbarray[slot].pid&0x7f) == origpid)
                        {
                            tmphitcount++;
                        }
                    }
                    if(tmphitcount)
                        hit[tmppid ^ set] += tmphitcount;
                }
                for(slot=0; slot<MAX_TLBSLOTS/MMU_ASSOCIATIVITY; slot++)
                {
                    if(hit[slot])
                        pidcount[pid] += hit[slot] + tlbstate.GetActive(slot);
                    if(hit[slot] + tlbstate.GetActive(slot) > MMU_ASSOCIATIVITY)
                        pidcount[pid] = 0x7f000000;
                }
            }
            for(min=0, x=1; x<MAX_TLBSLOTS / MMU_ASSOCIATIVITY; x++)
            {
                if(pidcount[x] < pidcount[min])
                    min = x;
            }
            if(pidcount[min] >= 0x7f000000)
                return -1;
            return min;
        }            
    };

class Process
    {
    public:
        uint32_t paddr_min;
        uint32_t paddr_max;
        uint32_t paddr_cur;
        uint32_t heapvaddr;
        uint32_t heapvaddr_min;
        uint32_t PID;        
        uint32_t maxva;
        uint32_t mmuindex;
        uint32_t isAgent;
        uint32_t pad;
        
        uint32_t SpecPID[MMU_ASSOCIATIVITY];
        union
        {
            unsigned char mmu_pad[8];
            MMU* mmu;
        };
        
        void ToNetworkEndian()
        {
            int index;
#define ENDIAN(x) x=htonl(x)
            ENDIAN(paddr_min); ENDIAN(paddr_max); ENDIAN(paddr_cur); ENDIAN(heapvaddr); ENDIAN(heapvaddr_min); ENDIAN(PID); ENDIAN(maxva); ENDIAN(isAgent);
            for(index=0; index<MMU_ASSOCIATIVITY; index++)
            {
                ENDIAN(SpecPID[index]);
            }
            if(mmu)
            {
                mmuindex = mmu->getMMUIndex();
                ENDIAN(mmuindex);
            }
            else 
            {
                mmuindex = 0;
            }
	    mmu = NULL;
#undef ENDIAN
        }
        
        int CountTLBs(enum SegmentType segtype)
        {
            int count = 0;
            for(unsigned x = 0; x<MAX_TLBSLOTS; x++)
            {
                if(mmu->tlbarray[x].getType() == segtype)
                    count++;
            }
            return count;
        }
        void PrintTLBs(void* mapper)
        {
            const char* extendeddata = NULL;
            unsigned int bincount[MMU_ASSOCIATIVITY] = {0,};
            for(unsigned int x = 0; x<MAX_TLBSLOTS; x++)
            {
                if(mmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > x%MMU_ASSOCIATIVITY)
                {
                    bincount[x%MMU_ASSOCIATIVITY]++;
                    if((mmu->tlbarray[x].origsize != mmu->tlbarray[x].size) && (mmu->tlbarray[x].getType() == IS_DATA))
                    {
                        if(mmu->tlbarray[x].origsize == 0)
                        {
                            extendeddata = "HEAP";
                        }
                        else 
                        {
                            extendeddata = "DATA+HEAP";
                        }
                    }
                    else 
                    {
                        extendeddata = typexlate[mmu->tlbarray[x].getType()];
                    }
                    
#if STANDALONE
#if 0
                    if((mmu->tlbarray[x].getType() == IS_DATA) || (mmu->tlbarray[x].getType() == IS_HEAP) || (mmu->tlbarray[x].getType() == IS_SHAR))
                    {
                        if(mmu->tlbarray[x].size - mmu->tlbarray[x].origsize > 0)
                        {
                            fprintf(EraseFile, "DDRInit: erase 0x%lx for %ld bytes\n", ((uint64_t)(mmu->tlbarray[x].paddr + mmu->tlbarray[x].exclsize + mmu->tlbarray[x].origsize)) * CONFIG_CNK_PAGE_SIZE, ((uint64_t)(mmu->tlbarray[x].size - mmu->tlbarray[x].origsize - mmu->tlbarray[x].exclsize)) * CONFIG_CNK_PAGE_SIZE);
                        }
                    }
#endif
#endif
                    TRACE(TRACE_StaticMap, ("MAPPER%8p Slot %3d:  pid=%4d  vaddr=%10d  paddr=%10d  size=%4d (orig: %4d) excl=%4d  type=%s\n", mapper, x, mmu->tlbarray[x].pid, mmu->tlbarray[x].vaddr, mmu->tlbarray[x].paddr, mmu->tlbarray[x].size, mmu->tlbarray[x].origsize, mmu->tlbarray[x].exclsize, extendeddata));
                }
            }
            TRACE(TRACE_StaticMap, ("MAPPER%8p pid %d MMU_BucketCounts:  B0=%d (%d)  B1=%d (%d)  B2=%d (%d)  B3=%d  Total=%d\n", mapper, PID, bincount[0]-bincount[1], bincount[0], bincount[1]-bincount[2], bincount[1], bincount[2]-bincount[3], bincount[2], bincount[3], bincount[0]+bincount[1]+bincount[2]+bincount[3]));
        }
        int getMaxVAWindow(unsigned int* vaddrmin, unsigned int* vaddrmax);
    };



struct TLBMapperSharedData {
    uint64_t        relocate[MAX_MEMORY_PER_NODE*2];
    unsigned short  copymap[MAX_MEMORY_PER_NODE];
    unsigned char   srcdemand[MAX_MEMORY_PER_NODE];
    unsigned char   eraselock[MAX_MEMORY_PER_NODE];
};



    class TLBMapper
    {
    private:
        char     MAGIC[4];
        uint32_t NUMTLBSLOTS;
        uint32_t totalmem;
        uint32_t kernelsize;
        uint32_t mapper_paddr_offset;
        uint32_t maxvaddr;
        uint32_t applayoutversion;
        uint32_t maxva;

        uint64_t       *relocate;
        unsigned short *copymap;
        unsigned char  *srcdemand;
        unsigned char  *eraselock;


        int      numagentproc;
        int      numproc;
        Process  proc[MAX_PROCESS_COUNT];
        
        uint32_t  coremask_job;  // Core mask indicating what cores are participating in this job
        uint32_t numrelocations;
        
        int      orignumproc;
        
        uint32_t externalcount;
        uint32_t mmusperprocess;
        uint32_t threadsperprocess;

        int32_t  externalpid2pindex[MAX_PROCESS_COUNT];
        uint32_t pad4[MAX_PROCESS_COUNT%2 + 1];
        
        /* Do not worry about Endian Convert variables below this comment */
        
        uint32_t extraprocess;
        uint32_t extraprocessMemory;
        uint32_t lastIteration;
        unsigned int     maxWaste;
        unsigned int     curWaste;
        unsigned int     PHASE;
        
        
        int     numsegs;
        Segment segs[MAX_SEGMENTCOUNT];
        
        bool             precalculated;
        bool             copymapCalculated;
        bool             pad2[6];
        
        TLBstate tmp_mmustate[MAX_PROCESS_COUNT];
        TLBstate tmp_mmustate2[MAX_PROCESS_COUNT];
        TLBstate tmp_oldstate[MAX_PROCESS_COUNT];
        
        bool isMapped(Segment* ptr);
        int MakeTLB(Segment* ptr, unsigned int vaddr, unsigned int paddr, unsigned int tlbsize, unsigned int exclsize);
        int calculateTLBSize(unsigned address);
        unsigned calculateSkew(unsigned vaddr, unsigned paddr, unsigned tlbsize);
        int CountTLBs(unsigned vaddr, unsigned size);
        int MapRange(Segment* ptr, unsigned int vaddr, unsigned int paddr, unsigned int size);
        int getAlignment(Segment* ptr, unsigned paddr);
        bool haveTLBSizeToMap(Segment* ptr, int tlbsize, int selproc);
        int mapSegment(Segment* ptr, int tlbsize, int selproc);
        void buildAssignedTLBList(Segment* ptr);
        void expandDataSegment(Segment* ptr, int selproc);
        void preprocessSegments();
        int getSegmentRoundoff(Segment* ptr, uint32_t roundoff_allocation);
        int allocateAppTLBs();
        int mapPinnedSegments();
        
        uint32_t allocateHeapTLBs(Process* p, int startP, int endP, bool& madeprogress);
        int partition_without_hole(uint32_t* heap);
        int partition_with_hole(uint32_t* heap);
        
        int copySegment(uint32_t src, uint32_t dst);
        int eraseSegment(uint32_t src);
        int exchangeSegment(uint32_t src, uint32_t dst);
        int ApplyMapperPhysicalOffset();
        bool isHeapBalanced(uint32_t minHeap, uint32_t maxHeap);
        int createSpeculativeAliases(int proc);
        int MapAtomics(int proc, SegmentType);
        int MapScrubWindow(int proc);
        int MapUserMMIO();
        void CalculatePID();
        void calculatePinnedAddress(Segment* ptr, int selproc);
        void CalculateMaxHeapVaddr(int x, int tlbsize, int tlbentries);
                
            
    public:
        int addSegment(SegmentType, unsigned int, unsigned int, size_t, int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int& newindex);
        int rmvSegment(unsigned int);
        int initializeProcesses();
        int partitioned_mapper(_BGP_VMM_RasData*);
        int installStaticEntry(int pindex, unsigned x);
        int installStaticTLBMap(int pindex);
        int installStaticTLBMapPartial(int pindex, uint32_t pid);
        int uninstallStaticTLBMap(int pindex);
        int copyProcessToOtherMMU(int sp, int dp, uint32_t* destpid);
        int resetStaticMap(int, int, int);
        int getSegment(uint32_t, SegmentType, uint64_t*, uint64_t*, size_t*);
        int getSegmentEntry(uint32_t pindex, uint64_t vaddr, uint64_t* paddr, size_t* vsize, uint64_t* tlbslot);
        int getSegmentEntryBySlot(uint32_t pindex, uint64_t slot, enum SegmentType* type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize);
        int copySegments();
        void dotest(int numprocessgroups, int processgroup);
        int getMaxVAWindow(int pid, unsigned int* vaddrmin, unsigned int* vaddrmax);
        
        uint32_t getPhysicalOffset(void) { return (mapper_paddr_offset * CONFIG_CNK_PAGE_SIZE); }
        uint32_t getTotalMem(void) { return totalmem; }
        
        int getPID(int pindex, uint64_t* pid) { *pid = proc[pindex].PID; return 0; };
        int getSpecPID(int pindex, int specindex, uint64_t* pid) { *pid = proc[pindex].SpecPID[specindex]; return 0; };
        int allocateAtomic(unsigned pindex, uint64_t vaddr, size_t* sizeMapped);
        
        bool isPreCalculated() { return precalculated; };
        int  CalculateCopymap();
        void GetCopymap(uint64_t **reloc, uint32_t *numreloc) {*reloc = relocate; *numreloc = numrelocations; };
        int translate2pindex(int process)
        {
            uint32_t x;
            if(process == -1)
                return -1;
            
            for(x=0; x<externalcount; x++)
            {
                if(externalpid2pindex[x] == process)
                    return x;
            }
            externalpid2pindex[externalcount++] = process;
            return externalcount-1;
        }
        void ToNetworkEndian()
        {
            int x;
            for(x=0; x<MAX_PROCESS_COUNT; x++)
            {
                proc[x].ToNetworkEndian();
            }
#define ENDIAN(x) x=htonl(x)
            ENDIAN(NUMTLBSLOTS); ENDIAN(totalmem); ENDIAN(kernelsize); ENDIAN(mapper_paddr_offset); ENDIAN(maxvaddr); 
            ENDIAN(applayoutversion); ENDIAN(maxva); ENDIAN(numproc); ENDIAN(numagentproc); ENDIAN(orignumproc);
            ENDIAN(externalcount);
            ENDIAN(mmusperprocess);
            ENDIAN(threadsperprocess);
            for(x=0; x<MAX_PROCESS_COUNT; x++)
            {
                ENDIAN(externalpid2pindex[x]);
            }
#undef ENDIAN
            for(x=0; x<MAX_MEMORY_PER_NODE; x++)
            {
#define ENDIAN(x) x=htons(x)
                ENDIAN(copymap[x]);
#undef ENDIAN                
            }
            numrelocations = htonl(numrelocations);
            if(htonl(0x11223344) != 0x11223344)
            {
                for(x=0; x<MAX_MEMORY_PER_NODE; x++)
                {
#define ENDIAN(x) x = htonl(x>>32) | (((uint64_t)(htonl(x&0xffffffff)))<<32)
                    ENDIAN(relocate[x]);
                }
            }
        }
        
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        friend int vmm_setupTLBMapper(int my_kernel_size);                                                             // FUSEDOS
#else                                                                                                                  // FUSEDOS
        friend int vmm_setupTLBMapper();
#endif                                                                                                                 // FUSEDOS
        friend int vmm_resetStaticMap(int , int, int);
    };

static MMU        mmuinstance[MAX_CORE_COUNT+1];
static MMU*       activemmu;


#define MAX_TLBMAPPERS MAX_APP_CORE_COUNT

static TLBMapper  tlbmapper[MAX_TLBMAPPERS];
static TLBMapper* tlbmapper_precalculated;
static TLBMapper* core2mapinstance[MAX_CORE_COUNT];
static uint64_t mapperFlags[MAPPERFLAGS_COUNT];
#if STANDALONE
static unsigned long STANDALONE_MEMSIZE = STANDALONE_MEMSIZE_DEFAULT;
static unsigned long STANDALONE_MAPOFFSET = STANDALONE_MAPOFFSET_DEFAULT;
static TLBMapperSharedData standalone_TLBMapperSharedData;
#endif


/********************************************************************************
 *
 * Utility Functions
 *
 ********************************************************************************/

int MMU::getMMUIndex()
{
    return ((((uint64_t)this) -  ((uint64_t)activemmu)) / sizeof(MMU));
}

int TLBstate::CalculateSlot(unsigned pid32, unsigned va, unsigned tlbsize)
{
    int bucket = 0;
    uint64_t pid   = pid32;
    uint64_t vaddr = ((uint64_t)va)<<20;
    // \todo There are probably much faster ways to do this....
    
#define PIDBIT(x,b)  (((x)>>(13-(b)))&0x1)
    bucket = ((PIDBIT(pid, 13)<<0) ^
              (PIDBIT(pid, 12)<<1) ^
              (PIDBIT(pid, 11)<<2) ^
              (PIDBIT(pid, 10)<<3) ^
              (PIDBIT(pid,  9)<<4) ^
              (PIDBIT(pid,  8)<<5) ^
              (PIDBIT(pid,  7)<<6));
    switch(TLBsizes[tlbsize])
    {
        case 1024:
            bucket ^= ((_GN(vaddr, 33)<<0) ^ 
                       (_GN(vaddr, 32)<<1) ^ 
                       (_GN(vaddr, 31)<<2) ^
                       (_GN(vaddr, 30)<<3) ^
                       (_GN(vaddr, 29)<<4) ^
                       (_GN(vaddr, 28)<<5) ^
                       (_GN(vaddr, 27)<<6));
            break;
        case 16:
            bucket ^= ((_GN(vaddr, 39)<<0) ^ 
                       (_GN(vaddr, 38)<<1) ^  
                       (_GN(vaddr, 37)<<2) ^ 
                       (_GN(vaddr, 36)<<3) ^ (_GN(vaddr, 32)<<3) ^
                       (_GN(vaddr, 35)<<4) ^ (_GN(vaddr, 31)<<4) ^
                       (_GN(vaddr, 34)<<5) ^ (_GN(vaddr, 30)<<5) ^
                       (_GN(vaddr, 33)<<6) ^ (_GN(vaddr, 29)<<6));
            break;
        case 1:
            bucket ^= ((_GN(vaddr, 43)<<0) ^ (_GN(vaddr, 36)<<0) ^
                       (_GN(vaddr, 42)<<1) ^ (_GN(vaddr, 35)<<1) ^
                       (_GN(vaddr, 41)<<2) ^ (_GN(vaddr, 34)<<2) ^
                       (_GN(vaddr, 40)<<3) ^ (_GN(vaddr, 33)<<3) ^
                       (_GN(vaddr, 39)<<4) ^ (_GN(vaddr, 32)<<4) ^
                       (_GN(vaddr, 38)<<5) ^ (_GN(vaddr, 31)<<5) ^
                       (_GN(vaddr, 37)<<6) ^ (_GN(vaddr, 30)<<6));
            break;
        default:
            printf("Error:  Unsupported TLB size %d\n", tlbsize);
            break;
    }
    return bucket;
}

int TLBstate::AcquireSlot(unsigned pid32, unsigned va, unsigned tlbsize)
{
    assert(tlbsize < NUMTLBSIZES);
    
    int bucket = CalculateSlot(pid32, va, tlbsize);
    if(index[bucket] < MMU_AVAILABLE_BUCKETS)
    {
        index[bucket]++;
        return bucket*MMU_ASSOCIATIVITY + index[bucket] - 1;
    }
    return -1;
}

void TLBstate::FreeSlot(int slot)
{
    if(slot == -1)
        return;
    index[slot/MMU_ASSOCIATIVITY]--;
}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int Process::getMaxVAWindow(unsigned int* vaddrmin, unsigned int* vaddrmax)
{
    unsigned int x;
    unsigned int y;
    unsigned int windowsize = 0;
    unsigned int bestwindowvaddr = 16;
    unsigned int bestwindowsize  = 0;
    
    for(x=0; x<MAX_TLBSLOTS; x++)
    {
        windowsize = maxva - (mmu->tlbarray[x].vaddr + mmu->tlbarray[x].size);
        for(y=0; y<MAX_TLBSLOTS; y++)
        {
            if(x == y)
                continue;
            
            if(mmu->tlbarray[x].vaddr + mmu->tlbarray[x].size <= mmu->tlbarray[y].vaddr)
            {
                windowsize = MIN(mmu->tlbarray[y].vaddr - (mmu->tlbarray[x].vaddr + mmu->tlbarray[x].size), windowsize);
            }
        }
        if(bestwindowsize < windowsize)
        {
            bestwindowvaddr= mmu->tlbarray[x].vaddr + mmu->tlbarray[x].size;
            bestwindowsize = windowsize;
        }
    }
    *vaddrmin = bestwindowvaddr;
    *vaddrmax = bestwindowvaddr + bestwindowsize - 1;
    return 0;
}
#endif                                                                                                                 // FUSEDOS

/*! \brief Returns true/false if the segment has been completely mapped
 */
bool TLBMapper::isMapped(Segment* ptr)
{
    if((ptr->type == IS_DATA)&&(PHASE == 0))
        return true;
    
    if(ptr->contiguous)
    {
        return ptr->isMapped;
    }
    else
    {
#if ENABLE_NONCONTIGUOUSMEM
        int x;
        for(x=0; x<ptr->numassigned; x++)
        {
            if((ptr->assigned[x] & 0x80) == 0)
                return false;
        }
#else
        assert(0);
#endif
    }
    return true;
}

/*! \brief Utility routine to make a TLB entry
 */
int TLBMapper::MakeTLB(Segment* ptr, unsigned int vaddr, unsigned int paddr, unsigned int tlbsize, unsigned int exclsize)
{
    int x;
    int slot;
    if(ptr->process == -1)
    {
        unsigned mmuset = 0;
        for(x=0; x<numproc; x++)
        {
            
            if(((  (1 << proc[x].mmu->getMMUIndex()) & (~mmuset) & ptr->coremask) != 0) ||
               (ptr->sharedPID == 0))
            {
                slot = proc[x].mmu->AcquireSlot(  ((ptr->sharedPID != 0)?0:proc[x].PID)  , vaddr, tlbsize);                
                if(slot == -1)
                    return -1;

                proc[x].mmu->tlbarray[slot].vaddr = vaddr;
                proc[x].mmu->tlbarray[slot].paddr = paddr;
                proc[x].mmu->tlbarray[slot].size  = TLBsizes[tlbsize];
                proc[x].mmu->tlbarray[slot].exclsize = exclsize;
                proc[x].mmu->tlbarray[slot].setSegment(ptr);                
                mmuset |= (1 << proc[x].mmu->getMMUIndex());
            }
        }
    }
    else
    {
        slot = proc[ptr->process].mmu->AcquireSlot(proc[ptr->process].PID, vaddr, tlbsize);
        if(slot == -1)
            return -1;
        proc[ptr->process].mmu->tlbarray[slot].vaddr = vaddr;
        proc[ptr->process].mmu->tlbarray[slot].paddr = paddr;
        proc[ptr->process].mmu->tlbarray[slot].size  = TLBsizes[tlbsize];
        proc[ptr->process].mmu->tlbarray[slot].exclsize = exclsize;
        proc[ptr->process].mmu->tlbarray[slot].setSegment(ptr);
    }
    return 0;
}

/*! \brief Calculates the largest TLB size that can be mapped at the given address alignment
 */
int TLBMapper::calculateTLBSize(unsigned address)
{
    int x;
    for(x=0; x<NUMTLBSIZES; x++)
    {
        if(address % TLBsizes[x] == 0)
            return x;
    }
    return -1;
}

/*! \brief Calculates the skew between virtual and physical addresses
 * Determines the size of a physical memory hole that could be created to align the physical and virtual addresses such that
 * the given tlbsize can be used.
 */
unsigned TLBMapper::calculateSkew(unsigned vaddr, unsigned paddr, unsigned tlbsize)
{
    int skew;
    int voffset;
    int poffset;
    
    voffset = vaddr & (TLBsizes[tlbsize]-1);
    poffset = paddr & (TLBsizes[tlbsize]-1);
    if(poffset > voffset)
        skew = TLBsizes[tlbsize] - poffset + voffset;
    else
        skew = voffset - poffset;
    return skew;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
/*! \brief Counts the number of TLBs that will be needed to map a region of memory at a given virtual address and size
 */
int TLBMapper::CountTLBs(unsigned vaddr, unsigned size)
{
    int x;
    int count = 0;
    unsigned int exclsize;
    while(size) /*infinite loop*/
    {
    resume:
        for(x=0; x<NUMTLBSIZES; x++)
        {
#if USE_EXCLUSION
            for(exclsize=TLBsizes[x]/2; ; exclsize /= 2)
#else
            for(exclsize=0; ; exclsize /= 2)
#endif
            {
#if MMU_TLBSIZES_POW2
                if(((vaddr & (TLBsizes[x]-1)) == exclsize)&&(size+exclsize >= TLBsizes[x]))
#else
                if((vaddr % TLBsizes[x] == exclsize)&&(size+exclsize >= TLBsizes[x]))
#endif
                {
                    vaddr += TLBsizes[x]-exclsize;
                    size  -= TLBsizes[x]-exclsize;
                    count++;
                    goto resume;
                }
                if(exclsize == 0) break;
            }
        }
    }   
    return count;
}
#endif                                                                                                                 // FUSEDOS

/* \brief Creates TLB mappings for a vaddr/paddr and size for either 1 process or all processes.
 */
int TLBMapper::MapRange(Segment* ptr, unsigned int vaddr, unsigned int paddr, unsigned int size)
{
    int x;
    int y;
    unsigned int exclsize;
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->SnapshotState(tmp_oldstate[x]);
    }
    while(size) /*infinite loop*/
    {
    resume:
        for(x=0; x<NUMTLBSIZES; x++)
        {
#if USE_EXCLUSION
            for(exclsize=TLBsizes[x]/2; ; exclsize /= 2)
#else
                for(exclsize=0; ; exclsize /= 2)
#endif
                {
                    if((vaddr % TLBsizes[x] == exclsize)&&(size+exclsize >= TLBsizes[x])&&(paddr % TLBsizes[x] == exclsize))
                    {
                        if(MakeTLB(ptr, vaddr-exclsize, paddr-exclsize, x, exclsize))
                        {
                            if(ptr->size - size < ptr->origsize+1)
                            {
                                for(y=0; y<numproc; y++)
                                {
                                    proc[y].mmu->RestoreState(tmp_oldstate[y]);
                                }
                                return -1;
                            }
                            else
                            {
                                ptr->size = ptr->size - size;
                                return 0;
                            }
                        }
                        vaddr += TLBsizes[x]-exclsize;
                        paddr += TLBsizes[x]-exclsize;
                        size  -= TLBsizes[x]-exclsize;
#if 1
                        if((ptr->size - size >= ptr->origsize) && (ptr->contains_heap == false))
                        {
                            // Everything important has been mapped from this segment, no need to add more tlbs
                            ptr->size = ptr->size - size;
                            return 0;
                        }
#endif
                        goto resume;
                    }
                    if(exclsize == 0) break;
                }
        }
    }   

    return 0;
}

/*! \brief Helper utility to add application segments
 */
int TLBMapper::addSegment(enum SegmentType type, unsigned vaddr, unsigned paddr_source, size_t size, int process, unsigned coremask, 
                          unsigned contiguous, unsigned roundable, unsigned pinned, unsigned loaded, unsigned sharedPID, unsigned allowHeap, unsigned temporary, unsigned& index)
{
    int x;
	TRACE(TRACE_StaticMap, ("TLBMapper::addSegment vaddr %u, paddr_source %u, size %ld, process %d, loaded %u\n",
					vaddr, paddr_source, size, process, loaded));

    for(x=0; x<MAX_SEGMENTCOUNT; x++)
    {
        if(segs[x].valid == 0)
            break;
    }
    if(x == MAX_SEGMENTCOUNT)
        return -1;
#if EDRAM
    if(pinned)
    {
        paddr_source -= MEMORY_PA_OFFSET;
    }
#endif
    
    if(pinned)
    {
        // TLB overlap detection    
#define INRANGE(low,high) if(((vaddr >= low)&&(vaddr < high)) || ((vaddr+size > low)&&(vaddr+size <= high))) {         Kernel_WriteFlightLog(FLIGHTLOG_high, FL_TLBSEGOVL, x, vaddr, low, high);  return -1; }
#if !STANDALONE
        INRANGE( ((uint64_t)&__KERNEL_TEXT_START)/CONFIG_CNK_PAGE_SIZE, (((uint64_t)&__KERNEL_END) + CONFIG_CNK_PAGE_SIZE - 1)/CONFIG_CNK_PAGE_SIZE);
        INRANGE(VA_MINADDR_MMIO/CONFIG_CNK_PAGE_SIZE, VA_MAXADDR_MMIO/CONFIG_CNK_PAGE_SIZE);
#endif
        for(x=0; x<numsegs; x++)
        {
            if(((segs[x].process == process) || (segs[x].process == -1) || (process == -1)) && 
               ((segs[x].coremask & coremask) != 0))  // overlap possible only when coremask overlaps
            {
                INRANGE(segs[x].vaddr, segs[x].vaddr+segs[x].size);
            }
        }
#undef INRANGE    
    }
    
#if !STANDALONE
    if((loaded) && (paddr_source < ((uint64_t)&__KERNEL_END)/CONFIG_CNK_PAGE_SIZE))
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        return -1;
#else                                                                                                                  // FUSEDOS
	TRACE(TRACE_StaticMap, ("FusedOS: skipping check in TLBMapper::addSegment, paddr_source %u < kernel end %lu\n",    // FUSEDOS
							paddr_source, ((uint64_t)&__KERNEL_END)/CONFIG_CNK_PAGE_SIZE) );                           // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    }
#endif
    
    segs[x].valid = 1;
    segs[x].origorder = numsegs;
    segs[x].type       = type;
    segs[x].origvaddr = segs[numsegs].vaddr      = vaddr;
    segs[x].source_paddr = paddr_source;
    segs[x].size = segs[numsegs].origsize   = size;
    segs[x].process    = process;
    segs[x].contiguous = contiguous;
    segs[x].roundable  = roundable;
    segs[x].pinned_vaddr= pinned;
    segs[x].coremask = coremask & coremask_job;
    segs[x].loaded   = loaded;
    segs[x].sharedPID= sharedPID;
    segs[x].allowHeap = allowHeap;
    segs[x].temporary= temporary;
    
    index = x;
    if(temporary == 0)
    {
        Kernel_WriteFlightLog(FLIGHTLOG_high, FL_TLBADDSEG, type, ((uint64_t)vaddr) * CONFIG_CNK_PAGE_SIZE, ((uint64_t)size)*CONFIG_CNK_PAGE_SIZE, process);
        
#if 1
        segs[x].PrintSeg(this, numsegs);
#endif
        
        if(x >= numsegs)
            numsegs = x+1;
    }
    return 0;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
/*! \brief Helper utility to remove application segments
 */
int TLBMapper::rmvSegment(unsigned vaddr)
{
    int x;
    for(x=0; x<MAX_SEGMENTCOUNT; x++)
    {
        if(vaddr == segs[x].vaddr)
            segs[x].valid = 0;
    }
    return 0;
}
#endif                                                                                                                 // FUSEDOS

/*! \brief Determines the alignment needed to converge on large TLB sizes given a segment and starting physical address.
 */
int TLBMapper::getAlignment(Segment* ptr, unsigned paddr)
{
    int vaddr= ptr->vaddr;
    int maxtlb = NUMTLBSIZES;
    int max_vaddr=0;
    int max_paddr=0;
    unsigned int minskew=UINT_MAX;
#if ENABLE_NONCONTIGUOUSMEM
    int x;
    unsigned int skew;
#endif
    
    if(ptr->pinned_vaddr == false)
    {
        maxtlb = calculateTLBSize(ptr->size);
        max_vaddr = maxva;
        max_paddr = paddr + ptr->size;
    }
    else
    {
        if(ptr->contiguous)
        {
            maxtlb = ptr->maxtlb;
            max_vaddr = vaddr + ptr->maxoffset;
            max_paddr = paddr + ptr->maxoffset;
        }
        else
        {
#if ENABLE_NONCONTIGUOUSMEM
            for(x=0; x<ptr->numassigned; x++)
            {
                if((ptr->assigned[x]&0x80)==0)
                {
                    skew = calculateSkew(vaddr, paddr, ptr->assigned[x]);
                    if(minskew > skew)
                        minskew = skew;
                }
                vaddr += TLBsizes[ptr->assigned[x]&0x7f];
            }
#else
            assert(0);
#endif
            return minskew;
        }
    }
    if(maxtlb == NUMTLBSIZES)
        return 0;
    return calculateSkew(max_vaddr, max_paddr, maxtlb);
}

/*! \brief Returns true/false if the segment can be at least partially mapped with the given tlbsize
 * \note Cannot partially map a contiguous segment
 */
bool TLBMapper::haveTLBSizeToMap(Segment* ptr, int tlbsize, int selproc)
{
#if ENABLE_NONCONTIGUOUSMEM
    int x;
#endif
    if(isMapped(ptr))
        return false;
    if(ptr->contiguous)
    {
        if(getAlignment(ptr, proc[selproc].paddr_cur) != 0)
            return false;
        return true;
    }
    else
    {
#if ENABLE_NONCONTIGUOUSMEM
        for(x=0; x<ptr->numassigned; x++)
        {
            if(ptr->assigned[x] == tlbsize)
            {
                return true;
            }
        }
#else
        assert(0);
#endif
    }
    return false;
}

/*! \brief Maps a segment using the given processes physical memory
 * 
 */
int TLBMapper::mapSegment(Segment* ptr, int tlbsize, int selproc)
{
#if ENABLE_NONCONTIGUOUSMEM
    int x;
    int y;
#endif
    if(ptr->contiguous)
    {
        if(proc[selproc].paddr_max - proc[selproc].paddr_cur < ptr->size)
            return -1;
        if(ptr->pinned_vaddr == false)
        {
            calculatePinnedAddress(ptr, selproc);
        }
        if(MapRange(ptr, ptr->vaddr, proc[selproc].paddr_cur, ptr->size) != 0)
            return -1;
        proc[selproc].paddr_cur += ptr->size;
        ptr->isMapped = true;
    }
    else
    {
#if ENABLE_NONCONTIGUOUSMEM
        unsigned int vaddr = ptr->vaddr;        
        for(x=0; x<numproc; x++)
        {
            proc[x].mmu->SnapshotState(tmp_oldstate[x]);
        }
        for(x=0; x<ptr->numassigned; vaddr += TLBsizes[ptr->assigned[x++]&0x7f])
        {
            if(ptr->assigned[x] == tlbsize)
            {
                ptr->assigned[x] |= 0x80;
                
                if(MakeTLB(ptr, vaddr, proc[selproc].paddr_cur, tlbsize, 0))
                {
                    for(y=0; y<numproc; y++)
                    {
                        proc[y].mmu->RestoreState(tmp_oldstate[y]);
                    }
                    return -1;
                }
                proc[selproc].paddr_cur += TLBsizes[tlbsize];
                break;
            }
        }
#else
        assert(0);
#endif
    }
    
    return 0;
}


/********************************************************************************
 *
 * Initialization Routines
 *
 ********************************************************************************/

/*! \brief Initialize the assigned variable in the segment structure
 */
void TLBMapper::buildAssignedTLBList(Segment* ptr)
{
    int x;
    unsigned size       = ptr->size;
    unsigned vaddr      = ptr->vaddr;
    int maxtlb          = NUMTLBSIZES;
    unsigned max_offset = 0;

#if ENABLE_NONCONTIGUOUSMEM
    ptr->numassigned = 0;
#endif
    
    while(size)
    {
#if ENABLE_NONCONTIGUOUSMEM
        if(ptr->numassigned >= (int)NUMTLBSLOTS)
            break;
#endif
        for(x=0; x<NUMTLBSIZES; x++)
        {
#if MMU_TLBSIZES_POW2       // assume power-of-2 TLB sizes.  Performance optimization.
            if(((vaddr & (TLBsizes[x]-1)) == 0)&&(size >= TLBsizes[x]))
#else
            if((vaddr % TLBsizes[x] == 0)&&(size >= TLBsizes[x]))
#endif
            {	    
#if ENABLE_NONCONTIGUOUSMEM
                ptr->assigned[ptr->numassigned++] = x;
#endif
                if(maxtlb > x)
                {
                    maxtlb = x;
                    max_offset = vaddr - ptr->vaddr;
                }
                vaddr += TLBsizes[x];
                size  -= TLBsizes[x];
                break;
            }
        }
    }
    
    ptr->maxtlb    = maxtlb;
    ptr->maxoffset = max_offset;
}

void TLBMapper::expandDataSegment(Segment* ptr, int selproc)
{
    uint32_t heap;
    if((isMapped(ptr) == false) && (ptr->roundable))
    {
        heap = proc[selproc].paddr_max - proc[selproc].paddr_cur;
        ptr->size = MAX(ptr->origsize+1,heap);
        ptr->contains_heap = true;
        buildAssignedTLBList(ptr);
    }
}

/*! \brief Performs a pre-processing step on all the application segments
 */
void TLBMapper::preprocessSegments()
{
    int x,y;
    Segment tmp;
    int trailer;
    maxva = 3 * MAX_PHYSICALMEMORY_PER_NODE;
    
    // remove any temporary segments
    for(x=0; x<MAX_SEGMENTCOUNT; x++)
    {
        if(segs[x].valid && segs[x].temporary)
            segs[x].valid = 0;
    }
    
    // sort by original order.
    for(x=0; x<numsegs; x++)
    {
        for(y=x+1; y<numsegs; y++)
        {
            if(segs[x].origorder > segs[y].origorder)
            {
                tmp = segs[y];
                segs[y] = segs[x];
                segs[x] = tmp;
            }
        }
    }
    
    for(x=0; x<numsegs; x++)
    {
        segs[x].vaddr= segs[x].origvaddr;
        segs[x].size = segs[x].origsize;
        segs[x].isMapped = false;
        segs[x].contains_heap = false;
        
        if(segs[x].pinned_vaddr && (segs[x].vaddr + segs[x].size >= maxva))
        {
            maxva = ((segs[x].vaddr + segs[x].size) + MAX_PHYSICALMEMORY_PER_NODE) & (~(MAX_PHYSICALMEMORY_PER_NODE-1));
        }
    }
    
    uint64_t proratedWaste = (maxWaste-curWaste+numsegs)/numsegs;
    for(x=0; x<numsegs; x++)
    {  
        //  check last TLB...  it may be possible to round up to the next largest and save 15 TLB entries!!!
        if(segs[x].roundable)
        {
            trailer = getSegmentRoundoff(&segs[x], proratedWaste);
            if(maxWaste > curWaste + trailer)
            {
                segs[x].size += trailer;
                curWaste += trailer;
            }
        }
    }
    
    // sort largest segment first.
    for(x=0; x<numsegs; x++)
    {
        for(y=x+1; y<numsegs; y++)
        {
#if USE_EXCLUSION
            if((((segs[x].type == segs[y].type) && (segs[x].size < segs[y].size)) ||
               ((segs[y].type == IS_TEXT) && (segs[x].type != segs[y].type)) ||
               ((segs[y].type == IS_DATA) && (segs[x].type != IS_TEXT) && (segs[x].type != segs[y].type))) &&
               (segs[y].coremask != VMM_SEGMENTCOREMASK_16))
#else
            if(segs[x].size < segs[y].size)
#endif
            {
                tmp = segs[y];
                segs[y] = segs[x];
                segs[x] = tmp;
            }
        }
    }
    
    maxvaddr = 0;
    for(x=0; x<numsegs; x++)
    {
        buildAssignedTLBList(&segs[x]);
        if((segs[x].pinned_vaddr)&&(maxvaddr < segs[x].vaddr))
        {
            maxvaddr = segs[x].vaddr + segs[x].size;
        }
    }
}


void TLBMapper::CalculateMaxHeapVaddr(int x, int tlbsize, int tlbentries)
{
    uint32_t sum = 0;
    uint32_t bestsum = ~(0);
    int bucket;
    int active;
    const uint32_t cost[MMU_ASSOCIATIVITY] = { 0, 10, 100, 1000000 };
    uint32_t max_acquired = 0;
    uint32_t minva;
    uint32_t bestva = 2*MAX_PHYSICALMEMORY_PER_NODE;
    
    minva = 2*MAX_PHYSICALMEMORY_PER_NODE - TLBsizes[tlbsize];
    while(1)
    {
        if(minva < MAX_PHYSICALMEMORY_PER_NODE)
        {
            while(max_acquired > minva)
            {
                bucket = proc[x].mmu->tlbstate.CalculateSlot(proc[x].PID, max_acquired, tlbsize);
                proc[x].mmu->tlbstate.FreeSlot(bucket*MMU_ASSOCIATIVITY);
                
                max_acquired -= TLBsizes[tlbsize];
            }
            max_acquired = bestva;
            proc[x].heapvaddr = max_acquired + TLBsizes[tlbsize];
            
            minva = max_acquired - TLBsizes[tlbsize] * tlbentries;
            while(max_acquired > minva)
            {
                bucket = proc[x].mmu->tlbstate.CalculateSlot(proc[x].PID, max_acquired, tlbsize);
                proc[x].mmu->AcquireSlot(proc[x].PID, max_acquired, tlbsize);
                
                max_acquired -= TLBsizes[tlbsize];
            }            
            return;
        }
        
        bucket = proc[x].mmu->tlbstate.CalculateSlot(proc[x].PID, minva, tlbsize);
        active = proc[x].mmu->tlbstate.GetActive(bucket);
        if(active == MMU_ASSOCIATIVITY)
        {
            while(max_acquired > minva)
            {
                bucket = proc[x].mmu->tlbstate.CalculateSlot(proc[x].PID, max_acquired, tlbsize);
                proc[x].mmu->tlbstate.FreeSlot(bucket*MMU_ASSOCIATIVITY);
                active = proc[x].mmu->tlbstate.GetActive(bucket);
                max_acquired -= TLBsizes[tlbsize];
            }
            max_acquired = 0;
            sum   = 0;
            minva -= TLBsizes[tlbsize];
        }
        else
        {
            sum += cost[active];
            proc[x].mmu->AcquireSlot(proc[x].PID, minva, tlbsize);
            if(max_acquired == 0)
                max_acquired = minva;
            if(max_acquired - minva + TLBsizes[tlbsize] >= tlbentries * TLBsizes[tlbsize])
            {
                if(bestsum > sum)
                {
                    bestva = max_acquired;
                    bestsum = sum;
                }
                bucket = proc[x].mmu->tlbstate.CalculateSlot(proc[x].PID, max_acquired, tlbsize);
                proc[x].mmu->tlbstate.FreeSlot(bucket*MMU_ASSOCIATIVITY);
                active = proc[x].mmu->tlbstate.GetActive(bucket);
                sum -= cost[active];
                
                max_acquired -= TLBsizes[tlbsize];
                if(max_acquired == minva)
                    max_acquired = 0;
            }
            minva -= TLBsizes[tlbsize];
        }
    }
}

int TLBMapper::copyProcessToOtherMMU(int sp, int dp, uint32_t* destpid)
{
    int x;
    int y;
    int slot;
    uint32_t tmppid;
    bool failure = false;
    uint32_t sourcepid = proc[sp].PID;
    uint32_t maxpid = 1024+128;
    
    for(x=0; x<MAX_TLBSLOTS; x++)
    {
        if(proc[sp].mmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > (unsigned)x%MMU_ASSOCIATIVITY)
        {
            if((proc[sp].mmu->tlbarray[x].pid == sourcepid) && (proc[sp].mmu->tlbarray[x].getType() == IS_DATA))            
            {
                // we found a TLB entry that should be mapped on the remote MMU,
                // there should be an entry over there that has the same paddr (if previously copied).
                for(y=0; y<MAX_TLBSLOTS; y++)
                {
                    if(proc[dp].mmu->tlbstate.GetActive(y/MMU_ASSOCIATIVITY) > (unsigned)y%MMU_ASSOCIATIVITY)
                    {
                        if(proc[sp].mmu->tlbarray[x].paddr + proc[sp].mmu->tlbarray[x].exclsize == proc[dp].mmu->tlbarray[y].paddr + proc[dp].mmu->tlbarray[y].exclsize)
                        {
                            if ( proc[dp].mmu->tlbarray[y].getType() == IS_DATA)
                            {
                                // So we already have a PID-specific mapping for this physical address
                                // Therefore, this must be a PID that correponds to the process
                                // (otherwise there was an earlier error in mapping). 
                                // 
                                //printf("Match found. paddr %016x pid %d. SOURCE type:%d exclsize:%d virt:%d slot:%d DEST type:%d exclsize:%d virt:%d slot:%d \n", proc[sp].mmu->tlbarray[x].paddr,proc[dp].mmu->tlbarray[y].pid, proc[sp].mmu->tlbarray[x].getType(), proc[sp].mmu->tlbarray[x].exclsize, proc[sp].mmu->tlbarray[x].vaddr, x, proc[dp].mmu->tlbarray[y].getType(), proc[dp].mmu->tlbarray[y].exclsize, proc[dp].mmu->tlbarray[y].vaddr, y); 
                                *destpid = proc[dp].mmu->tlbarray[y].pid;
                                return 0;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    // If a process was already copied to this MMU, fail the request. We only allow one.
    for(x=0; x<MAX_TLBSLOTS; x++)
    {
        if(proc[dp].mmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > (unsigned)x%MMU_ASSOCIATIVITY)
        {
            if((proc[dp].mmu->tlbarray[x].pid >= maxpid) && proc[dp].mmu->tlbarray[x].pid < (maxpid + MAX_TLBSLOTS / MMU_ASSOCIATIVITY))
            {
                // A copy of some other process to this MMU has already been done. 
                printf("Another process has already been mapped to this MMU. pid %d\n", proc[dp].mmu->tlbarray[x].pid);
                return -1;
            }
        }
    }
    for(tmppid = maxpid; tmppid < maxpid + MAX_TLBSLOTS / MMU_ASSOCIATIVITY; tmppid++)
    {
        failure = false;
        proc[dp].mmu->SnapshotState(tmp_mmustate[dp]);
        
        for(x=0; x<MAX_TLBSLOTS; x++)
        {
            if(proc[sp].mmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > (unsigned)x%MMU_ASSOCIATIVITY)
            {
                if(proc[sp].mmu->tlbarray[x].pid == sourcepid) // pid==0 is already accessible and other pids are not accessible and therefore do not need to be copied.  
                {
                    int tlbsize = 3;
                    switch(proc[sp].mmu->tlbarray[x].size)
                    {
                        case 1:
                            tlbsize = 2;
                            break;
                        case 16:
                            tlbsize = 1;
                            break;
                        case 1024:
                            tlbsize = 0;
                            break;
                    }
                    
                    slot = proc[dp].mmu->AcquireSlot(tmppid, proc[sp].mmu->tlbarray[x].vaddr, tlbsize);
                    if(slot == -1)
                    {
                        failure = true;
                        break;
                    }
                    proc[dp].mmu->tlbarray[slot] = proc[sp].mmu->tlbarray[x];
                    proc[dp].mmu->tlbarray[slot].pid = tmppid;
               }
            }
        }
        if(failure == false)
        {
            *destpid = tmppid;
            return 0;
        }
        
        proc[dp].mmu->RestoreState(tmp_mmustate[dp]);
    }
    return -1;
}

void TLBMapper::calculatePinnedAddress(Segment* ptr, int selproc)
{
    int rc;
    int gb;
    int bestgb = 0;
    unsigned int besthit = (~0);
    unsigned int hitcount= (~0);
    int x;
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->SnapshotState(tmp_mmustate[x]);
    }
    
    for(gb=0; gb<16; gb++)
    {
        for(x=0; x<numproc; x++)
        {
            proc[x].mmu->RestoreState(tmp_mmustate[x]);
        }
        ptr->vaddr = ptr->origvaddr = maxva + proc[selproc].paddr_cur + gb*1024;
        if(MapRange(ptr, ptr->vaddr, proc[selproc].paddr_cur, ptr->size) != 0)
        {
            continue;
        }
        if(ptr->type == IS_SHAR)
        {
            rc = MapAtomics(0, IS_SHAR);
            if(rc)
                continue;
        }
        for(x=0, hitcount=0; x<numproc; x++)
        {
            hitcount += proc[x].mmu->tlbstate.calculateScore(tmp_mmustate[x]);
        }
        if(hitcount < besthit)
        {
            bestgb = gb;
            besthit = hitcount;
        }
    }
    ptr->vaddr = ptr->origvaddr = maxva + proc[selproc].paddr_cur + bestgb*1024;
    
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->RestoreState(tmp_mmustate[x]);
    }
}

void TLBMapper::CalculatePID()
{
    int x;
#if 0
    for(x=0; x<numproc; x++)
    {
        proc[x].PID = (x%4)*32+64;
    }
#else
    int seg;
    int rc;
    int bestpid = -1;
    const uint32_t score[] = { 1, 10, 20, 80, 200 };
    uint32_t besthit = (MAX_TLBSLOTS / MMU_ASSOCIATIVITY) * score[3];
    uint32_t hitcount;
    int contention = 1;
    switch(threadsperprocess)
    {
        case 1: contention=4; break;
        case 2: contention=2; break;
        default:
            contention=1; break;
    }
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->SnapshotState(tmp_mmustate[x]);
    }
    for(seg=0; seg<numsegs; seg++)
    {
        // map pinned shared memory first
        if((segs[seg].pinned_vaddr) && (segs[seg].process == -1) && (segs[seg].sharedPID == true))
        {
            MapRange(&segs[seg], segs[seg].vaddr, segs[seg].vaddr, segs[seg].size);
        }
    }
    for(x=0; x<orignumproc + numagentproc; x++)
    {
        int pid;
        bestpid = -1;
        besthit = (MAX_TLBSLOTS / MMU_ASSOCIATIVITY) * score[3];
        
        proc[x].mmu->SnapshotState(tmp_mmustate2[x]);
        for(pid=0, rc=0; pid<MAX_TLBSLOTS / MMU_ASSOCIATIVITY; pid++)
        {
            proc[x].mmu->RestoreState(tmp_mmustate2[x]);
            proc[x].PID = (pid + (MAX_TLBSLOTS / MMU_ASSOCIATIVITY / contention) * (x%contention))%(MAX_TLBSLOTS / MMU_ASSOCIATIVITY) + 
                (MAX_TLBSLOTS / MMU_ASSOCIATIVITY) * (x%4 + 1);
            for(seg=0; seg<numsegs; seg++)
            {
                if((segs[seg].pinned_vaddr) && (segs[seg].process == x))
                {
                    rc |= MapRange(&segs[seg], segs[seg].vaddr, segs[seg].vaddr, segs[seg].size);
                }
            }
            rc |= MapAtomics(x, IS_DATA);
            if(rc)
                continue;
            
            hitcount = proc[x].mmu->tlbstate.calculateScore(tmp_mmustate2[x]);
            if(hitcount < besthit)
            {
                bestpid = pid;
                besthit = hitcount;
            }
        }
        proc[x].mmu->RestoreState(tmp_mmustate2[x]);
        
        proc[x].PID = (bestpid + (MAX_TLBSLOTS / MMU_ASSOCIATIVITY / contention) * (x%contention))%(MAX_TLBSLOTS / MMU_ASSOCIATIVITY) + 
            (MAX_TLBSLOTS / MMU_ASSOCIATIVITY) * (x%4 + 1);
        for(seg=0; seg<numsegs; seg++)
        {
            if((segs[seg].pinned_vaddr) && (segs[seg].process == x))
            {
                rc = MapRange(&segs[seg], segs[seg].vaddr, segs[seg].vaddr, segs[seg].size);
            }
        }
        MapAtomics(x, IS_DATA);
    }
    
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->RestoreState(tmp_mmustate[x]);
    }
#endif
}

/*! \brief Initialize the process structure
 */
int TLBMapper::initializeProcesses()
{
    int x;
    memset(proc, 0, sizeof(proc));
    numproc = orignumproc;

    switch(numproc)
    {
    case 1: mmusperprocess = 16; threadsperprocess=64; break;
    case 2: mmusperprocess = 8;  threadsperprocess=32; break;
    case 4: mmusperprocess = 4;  threadsperprocess=16; break;
    case 8: mmusperprocess = 2;  threadsperprocess=8;  break;
    case 16: mmusperprocess = 1; threadsperprocess=4;  break;
    case 32: mmusperprocess = 1; threadsperprocess=2;  break;
    case 64: mmusperprocess = 1; threadsperprocess=1;  break;
    default:
      mmusperprocess = 1; break;
    }
    
    extraprocessMemory = totalmem - (totalmem >> extraprocess);

    if((extraprocess == 0) && (numproc >= 16) && (mapperFlags[MAPPERFLAGS_ALIGN16]))
    {
        extraprocessMemory = 0;
        for(x=0; x<numsegs; x++)
        {
            if((segs[x].process == -1) || (segs[x].coremask == VMM_SEGMENTCOREMASK_16))
            {
                extraprocessMemory += segs[x].size;
            }
        }
        extraprocessMemory += 32;
    }
    uint32_t minSegmentStart[IS_SEGMENTTYPECOUNT] = {0,};
    uint32_t maxSegmentSize[IS_SEGMENTTYPECOUNT] = {0,};
    memset(minSegmentStart, 0, sizeof(minSegmentStart));
    memset(maxSegmentSize,  0, sizeof(maxSegmentSize));
    for(x=0; x<numsegs; x++)
    {
        if(segs[x].coremask != VMM_SEGMENTCOREMASK_16)
        {
            minSegmentStart[segs[x].type] = segs[x].vaddr;
            maxSegmentSize[segs[x].type] = MAX(maxSegmentSize[segs[x].type], segs[x].size);
        }
    }
    
    for(x=0; x<numproc; x++)
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#if STANDALONE
        int zcnt = 0;
        int bitmask = 0x80000000;
        for (int i=0; i<32; i++, zcnt++)
        {
            if (coremask_job & bitmask>>zcnt) break;
        }
        int mmu_index_origin =  (coremask_job == VMM_SEGMENTCOREMASK_ALL) ? 0 : (31 - zcnt);
#else
        int mmu_index_origin =  (coremask_job == VMM_SEGMENTCOREMASK_ALL) ? 0 : (31 - cntlz32(coremask_job));
#endif
        proc[x].mmu = &activemmu[mmu_index_origin + ((x*threadsperprocess) / MAX_HWTHREADS_PER_CORE)];
#else                                                                                                                  // FUSEDOS
        proc[x].mmu = &activemmu[x / MAX_HWTHREADS_PER_CORE];                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
        if(x==0)                                                                                                       // FUSEDOS
        	proc[x].paddr_min = kernelsize + mapper_paddr_offset;                                                      // FUSEDOS
        else                                                                                                           // FUSEDOS
        	proc[x].paddr_min = x * totalmem /  (numproc * (1<<extraprocess)) + mapper_paddr_offset;                   // FUSEDOS
        proc[x].paddr_max = (x+1) * totalmem / (numproc * (1<<extraprocess)) + mapper_paddr_offset;                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
        if(x==0)
            proc[x].paddr_min = kernelsize;
        else
        {
            proc[x].paddr_min = proc[x-1].paddr_max;
        }
        
        if(mapperFlags[MAPPERFLAGS_COMMONHEAP])
        {
            // process 0 has room to include text segment + padding for 16MB alignment.
            // processes 1-n just include data segment + padding for 16MB alignment.
            proc[x].paddr_max = proc[x].paddr_min + maxSegmentSize[IS_DATA];
            
            if((proc[x].paddr_min & (~0xf)) != (proc[x].paddr_max & (~0xf)))
            {
                proc[x].paddr_min += (16 + (minSegmentStart[IS_DATA] & 0x0f) - (proc[x].paddr_min & 0x0f)) & 0x0f;
                proc[x].paddr_max = proc[x].paddr_min + maxSegmentSize[IS_DATA];
                proc[x].paddr_max = (15 + proc[x].paddr_max) & (~0x0f);
            }
        }
        else
        {
            proc[x].paddr_max = (x+1) * (totalmem - extraprocessMemory) / numproc;
            proc[x].paddr_max &= ~0xf; // always end on 16MB alignment
        }
#endif                                                                                                                 // FUSEDOS
        proc[x].paddr_cur = proc[x].paddr_min;
        proc[x].mmu->ClearMMU();
        proc[x].maxva     = maxva;
        proc[x].PID = (x%4)*32+64;
        
        TRACE(TRACE_StaticMap,                                                                                         // FUSEDOS
              ("FUSEDOS initializeProcesses: x %d, paddr_min 0x%x, paddr_max 0x%x, kernelsize 0x%x, mmu %p\n",        // FUSEDOS
              x, proc[x].paddr_min, proc[x].paddr_max, kernelsize, proc[x].mmu));                                      // FUSEDOS
        TRACE(TRACE_StaticMap, ("FUSEDOS initializeProcesses: totalmem 0x%x, numproc %d, extraprocess %d\n",          // FUSEDOS
              totalmem, numproc, extraprocess));                                                                       // FUSEDOS

        if(proc[x].paddr_min >= proc[x].paddr_max)
            return -1;
    }

    for(int xx=0; xx<numagentproc; xx++)
    {
        proc[numproc].mmu = &activemmu[APPAGENT_COREID];
        proc[numproc].paddr_min = totalmem;         // no memory allocation
        proc[numproc].paddr_cur = totalmem;         // no memory allocation
        proc[numproc].paddr_max = totalmem;         // no memory allocation
        proc[numproc].mmu->ClearMMU();
        proc[numproc].maxva     = maxva;
        proc[numproc].isAgent   = true;
#if MMU_FORCEPID0
        proc[numproc].PID       = 0;
#else
        proc[numproc].PID       = 1 + numproc;
#endif
        numproc++;
    }

#if MAP_PROCESSWINDOWS
    Segment procwin;
    procwin.valid = true;
    procwin.process   = -1;
    procwin.coremask  = VMM_SEGMENTCOREMASK_ALL;
    procwin.contains_heap = false;
    procwin.pinned_vaddr = true;
    procwin.isMapped = true;
    procwin.type  = IS_PROCESSWINDOW;
    procwin.origsize = procwin.size = totalmem;
    procwin.sharedPID = true;
    
    procwin.source_paddr = procwin.origvaddr = procwin.vaddr = x;
#if defined __FUSEDOS__                                                                                               // FUSEDOS
	unsigned int pmem_origin = (unsigned int) NodeState.DomainDesc.ddrOrigin/CONFIG_CNK_PAGE_SIZE;                     // FUSEDOS
	unsigned int procwin_vaddr_start =  7*64*1024 + pmem_origin;                                                       // FUSEDOS
	TRACE(TRACE_StaticMap, ("FUSEDOS map ProcessWindow of size %d, origin %u, vaddr %d [MB]\n", procwin.origsize,   // FUSEDOS
				pmem_origin, procwin_vaddr_start));                                                                    // FUSEDOS
	MapRange(&procwin, procwin_vaddr_start, pmem_origin, totalmem);                                                    // FUSEDOS
#else                                                                                                                  // FUSEDOS
    MapRange(&procwin, 7*64*1024 + kernelsize, kernelsize, totalmem - kernelsize);
#endif                                                                                                                 // FUSEDOS
#endif    
    
    if(extraprocessMemory || mapperFlags[MAPPERFLAGS_COMMONHEAP])
    {
        proc[numproc].mmu = &activemmu[MAX_CORE_COUNT];
        proc[numproc].paddr_min = (proc[orignumproc-1].paddr_max + 31) & (~0x1f);
        proc[numproc].paddr_cur = proc[numproc].paddr_min;
        proc[numproc].paddr_max = totalmem;
        proc[numproc].mmu->ClearMMU();
        proc[numproc].maxva     = maxva;
        proc[numproc].PID       = 1 + numproc;
        numproc++;
    }
    return 0;
}


/********************************************************************************
 *
 * Partitioned Mapper routines
 *
 ********************************************************************************/

/*! \brief Determines the amount of padding that can be added to the tail of the segment
 *
 * Add padding to the tail of the segment such that we don't need to use lots of small 1MB/16MB TLBs in order
 * to tightly map the segment.  
 *
 */
int TLBMapper::getSegmentRoundoff(Segment* ptr, uint32_t roundoff_allocation)
{
    int x;
    unsigned size = ptr->size;
    unsigned vaddr= ptr->vaddr;
    unsigned newsize;
    unsigned bestskew = 0;
#if 0
    unsigned totalcost = UINT_MAX;
    unsigned cost;
    int      cnttlbs;
#endif
    if(ptr->pinned_vaddr)
        vaddr = ptr->vaddr;
    else
        vaddr = 0;
    
    for(x=0; x<NUMTLBSIZES; x++)
    {
#if MAP_CATOMICS
        if(TLBsizes[x] == 16)
            newsize = ALIGNUP((vaddr + size), TLBsizes[x]*2) - vaddr;
        else
            newsize = ALIGNUP((vaddr + size), TLBsizes[x]) - vaddr;
#else
        newsize = ALIGNUP((vaddr + size), TLBsizes[x]) - vaddr;
#endif        
        if(newsize - size < roundoff_allocation)
        {
            bestskew = newsize - size;
            break;
        }
#if 0        
        /* \note Roundoff Algorithm assumes that percent-of-memory is equivalent to percent-of-TLB slots (and therefore, we can add them to
         * produce 1 value for cost)
         */
        cnttlbs = CountTLBs(vaddr, newsize);
        int memfraction = (1000 - 1000 * maxWaste / WASTE_LIMIT) * (newsize - size) / (totalmem/numproc);
        int tlbfraction = (1000 * maxWaste / WASTE_LIMIT) * cnttlbs / MAX_TLBSLOTS;
        cost = memfraction + tlbfraction;
        
        printf("roundoff: tlbsize=%d  size=%d newsize=%d  cost=%d  totalcost=%d   curwaste=%d  maxwaste=%d  cnttlbs=%d  memfraction=%d  tlbfraction=%d\n", TLBsizes[x], size, newsize, cost, totalcost, curWaste, maxWaste, cnttlbs, memfraction, tlbfraction);
        if((totalcost > cost)&&(newsize - size < (maxWaste-curWaste)))
        {
            totalcost = cost;
            bestskew  = newsize - size;
        }
#endif
    }
    return bestskew;
}

int TLBMapper::mapPinnedSegments()
{
    int x;
    int skew;
    int rc = 0;
    uint32_t roundup;
    uint32_t skewup;
    
    if((mapperFlags[MAPPERFLAGS_ALIGN16]) || (mapperFlags[MAPPERFLAGS_COMMONHEAP]))
    {
        if(orignumproc + numagentproc != numproc)
        {
            for(x=0; x<numsegs; x++)
            {
                if(((segs[x].process == -1) || (segs[x].coremask == VMM_SEGMENTCOREMASK_16)) && (segs[x].type == IS_SHAR)) // map this first, due to L2 atomics alignment
                {
                    rc |= mapSegment(&segs[x], 2, numproc - 1);
                    rc |= MapAtomics(0, IS_SHAR);
                }
            }
            
            for(x=0; x<numsegs; x++)
            {
                if(isMapped(&segs[x]))
                    continue;
                
                if((segs[x].process == -1) || (segs[x].coremask == VMM_SEGMENTCOREMASK_16))
                {
                    if(segs[x].pinned_vaddr == false)
                    {
                        skewup = segs[x].vaddr % 16;
                        if(proc[numproc - 1].paddr_cur % 16 != segs[x].vaddr % 16)
                        {
                            skewup = (16 + (segs[x].vaddr % 16) - (proc[numproc - 1].paddr_cur % 16)) % 16;
                            proc[numproc - 1].paddr_cur += skewup;
                        }
                    }
                    rc |= mapSegment(&segs[x], 2, numproc - 1);
                }
            }
        }
    }
    
    if(orignumproc > 1)
    {
        for(x=0; x<numsegs; x++)
        {
            if(isMapped(&segs[x]))
                continue;
            if(segs[x].coremask == VMM_SEGMENTCOREMASK_16)
            {
                rc |= mapSegment(&segs[x], 2, (segs[x].process - orignumproc + 1) % orignumproc);
            }
        }
    }
    for(x=0; x<numsegs; x++)
    {
        if(isMapped(&segs[x]))
            continue;
        if(segs[x].coremask == VMM_SEGMENTCOREMASK_16)
            continue;
        
        if(segs[x].type == IS_TEXT)
        {
            rc |= mapSegment(&segs[x], 2, 0);
            break;
        }
    }
    for(x=0; x<numsegs; x++)
    {
        if(isMapped(&segs[x]))
            continue;
        if(segs[x].coremask == VMM_SEGMENTCOREMASK_16)
            continue;

        if((segs[x].type == IS_DATA) && (segs[x].process >= 0))
        {
            skewup = 0;
            if(proc[segs[x].process].paddr_cur % 16 != segs[x].vaddr % 16)
            {
                skewup = (16 + (segs[x].vaddr % 16) - (proc[segs[x].process].paddr_cur % 16)) % 16;
                if(proc[segs[x].process].paddr_cur + skewup >= proc[segs[x].process].paddr_max)
                    skewup = 0;
            }
            
            skew = (proc[segs[x].process].paddr_cur + segs[x].size + skewup) % 16;
            roundup = 0;
            if((skew > 0) && (segs[x].roundable))
            {
                roundup = 16 - skew;
            }
            if((maxWaste - curWaste)/1 >= roundup + skewup)
            {
                segs[x].size += roundup;
                proc[segs[x].process].paddr_cur += skewup;
                curWaste += roundup + skewup;
            }
            rc |= mapSegment(&segs[x], 2, segs[x].process % orignumproc);
        }
    }
    return rc;
}

/*! \brief Allocates all the application's segments on all processes into physical memory
 */
int TLBMapper::allocateAppTLBs()
{
    int maxtlbsize;
    int optimal;
    int selproc;
    bool mapped;
    int x;
    int tlbsize;
    int seg;
    bool everythingMapped;
    
    bool didwork  = true;
    int  idleloop = 0;
    do
    {
        if(didwork == false)
        {
            idleloop++;
            if(idleloop > MAX_PROCESS_COUNT * 2)
                return -1;
        }
        else
            idleloop = 0;
        didwork = false;
        
        selproc = 0;
        maxtlbsize = calculateTLBSize(proc[selproc].paddr_cur);
        for(x=0; x<numproc; x++)
        {
            optimal = calculateTLBSize(proc[x].paddr_cur);
            
            if(proc[selproc].paddr_max - proc[selproc].paddr_cur < proc[x].paddr_max - proc[x].paddr_cur)
            {
                selproc = x;
                maxtlbsize = optimal;
            }
        }
        if(idleloop > MAX_PROCESS_COUNT)
        {
            selproc = (selproc + idleloop) % numproc;
        }
        
        // find segment to map on process 'selproc'
        for(tlbsize = maxtlbsize, mapped = false; (tlbsize<NUMTLBSIZES)&&(!mapped); tlbsize++)
        {
            for(seg=0; seg<numsegs; seg++)
            {
                if((PHASE == 1)&&(segs[seg].type == IS_DATA))
                {
                    expandDataSegment(&segs[seg], selproc);
                }
                if(haveTLBSizeToMap(&segs[seg], tlbsize, selproc))
                {
                    if(mapSegment(&segs[seg], tlbsize, selproc) == 0)
                    {
                        mapped  = true;
                        break;
                    }
                }
            }
        }
        if(mapped == false)
        {
            int maxskew = totalmem;
            int skew;
            int maxseg  = -1;
            
            // find segment to map on process 'selproc', if a minimal skew can be applied.
            for(seg=0; seg<numsegs; seg++)
            {
                if(isMapped(&segs[seg]))
                    continue;
                if((PHASE == 1)&&(segs[seg].type == IS_DATA))
                {
                    expandDataSegment(&segs[seg], selproc);
                }
                skew = getAlignment(&segs[seg], proc[selproc].paddr_cur);
                if((maxskew > skew) && ( (int)(proc[selproc].paddr_max - proc[selproc].paddr_cur) > skew))
                {
                    maxskew = skew;
                    maxseg = seg;
                }
            }
            if(maxseg >= 0)
            {
                if((maxWaste > curWaste + maxskew)||(PHASE == 1))
                {
                    curWaste += maxskew;
                    proc[selproc].paddr_cur += maxskew;
                    
                    if((PHASE == 1)&&(segs[maxseg].type == IS_DATA))
                    {
                        expandDataSegment(&segs[maxseg], selproc);
                    }
                    optimal = calculateTLBSize(proc[selproc].paddr_cur);
                    if(mapSegment(&segs[maxseg], optimal, selproc) == 0)
                        didwork = true;
                }
            }
        }
        else
            didwork = true;
        
        if(didwork == false)
        {
            int maxskew = totalmem;
            int skew;
            int maxseg  = -1;
            
            uint32_t maxpad = 0;
            uint32_t tmp_maxwaste = maxWaste - curWaste;
            for(seg=0; seg<numsegs; seg++)
            {
                if(isMapped(&segs[seg]))
                    continue;
                
                for(x=0; x<NUMTLBSIZES; x++) // todo
                {
                    uint32_t round_offset = (segs[seg].vaddr + segs[seg].size) % TLBsizes[x];
                    uint32_t wastecalc = 0;
                    if(segs[seg].roundable && (round_offset != 0))
                    {
                        wastecalc = TLBsizes[x] - round_offset;
                        round_offset = 0;
                    }
                    if(round_offset == 0)
                    {
                        skew = (TLBsizes[x] + (segs[seg].vaddr % TLBsizes[x]) - (proc[selproc].paddr_cur % TLBsizes[x])) % TLBsizes[x];
                        wastecalc += skew;
                        if((tmp_maxwaste > wastecalc) && (skew > 0) && (  (int)(proc[selproc].paddr_max - proc[selproc].paddr_cur) > skew))
                        {
                            maxskew = skew;
                            maxseg  = seg;
                            maxpad  = wastecalc - skew;
                            tmp_maxwaste = wastecalc;
                        }
                    }
                }
            }
            if(maxseg != -1)
            {
                curWaste += maxskew;
                proc[selproc].paddr_cur += maxskew;
                segs[seg].size += maxpad;
                
                if((PHASE == 1)&&(segs[maxseg].type == IS_DATA))
                {
                    expandDataSegment(&segs[maxseg], selproc);
                }
                optimal = calculateTLBSize(proc[selproc].paddr_cur);
                if(mapSegment(&segs[maxseg], optimal, selproc) == 0)
                    didwork = true;
            }
        }
        
        everythingMapped = true;
        for(seg=0; seg<numsegs; seg++)
        {
            if(isMapped(&segs[seg]) == false)
            {
                everythingMapped = false;
                break;
            }
        }
    } while(! everythingMapped);
    
    return 0;
}

/*! \brief Allocates the heap using the remaining physical memory and TLBs
 */
uint32_t TLBMapper::allocateHeapTLBs(Process* p, int startProc, int endProc, bool& madeprogress)
{
    int slot = -1;
    int x;
    int pid;
    bool localProgress = true;
#if USE_EXCLUSION   
    unsigned exclsize;    
#endif
    if(p->isAgent)
        return 0;
    
    if((p->paddr_max <= p->paddr_cur) || (p->heapvaddr <= p->heapvaddr_min))
        return 0;

    for(x=0; x<NUMTLBSIZES; x++)
    {
        if((p->heapvaddr % TLBsizes[x] != 0) || (((p->paddr_max - TLBsizes[x])%TLBsizes[x]) != 0))
            continue;
        if((p->paddr_max - p->paddr_cur >= TLBsizes[x]) && (p->heapvaddr >= p->heapvaddr_min + TLBsizes[x]))
        {
            localProgress = false;
            for(pid=startProc; pid < endProc; pid++)
            {
                slot = proc[pid].mmu->AcquireSlot(proc[pid].PID, p->heapvaddr - TLBsizes[x], x);
                if(slot != -1)
                {
                    proc[pid].mmu->tlbarray[slot].vaddr = p->heapvaddr - TLBsizes[x];
                    proc[pid].mmu->tlbarray[slot].paddr = p->paddr_max - TLBsizes[x];
                    proc[pid].mmu->tlbarray[slot].size  = TLBsizes[x];
                    proc[pid].mmu->tlbarray[slot].exclsize = 0;
                    proc[pid].mmu->tlbarray[slot].setSegment(NULL);
                    localProgress = true;
                }
                else
                {
                    if(localProgress)
                    {
                        for(pid=startProc; pid < endProc; pid++)
                        {
                            proc[pid].mmu->RestoreState(tmp_mmustate[pid]);
                        }
                    }
                    localProgress = false;
                    
                    break;
                }
            }
            if(localProgress)
            {
                p->heapvaddr -= TLBsizes[x];
                p->paddr_max -= TLBsizes[x];
                madeprogress = true;
                return TLBsizes[x];
            }
        }
        
#if USE_EXCLUSION   
        unsigned int hsize = MIN(p->paddr_max - p->paddr_cur, p->heapvaddr - p->heapvaddr_min);
        for(exclsize=1; exclsize < TLBsizes[x]; exclsize *= 2)
        {
            if(hsize >= TLBsizes[x] - exclsize)
            {
                localProgress = false;
                for(pid=startProc; pid < endProc; pid++)
                {
                    slot = proc[pid].mmu->AcquireSlot(proc[pid].PID, p->heapvaddr - (TLBsizes[x] - exclsize), x);
                    if(slot != -1)
                    {                        
                        proc[pid].mmu->tlbarray[slot].vaddr = p->heapvaddr - TLBsizes[x];
                        proc[pid].mmu->tlbarray[slot].paddr = p->paddr_max - TLBsizes[x];
                        proc[pid].mmu->tlbarray[slot].size  = TLBsizes[x];
                        proc[pid].mmu->tlbarray[slot].exclsize = exclsize;
                        proc[pid].mmu->tlbarray[slot].setSegment(NULL);
                        localProgress = true;
                    }
                    else
                    {
                        if(localProgress)
                        {
                            for(pid=startProc; pid < endProc; pid++)
                            {
                                proc[pid].mmu->RestoreState(tmp_mmustate[pid]);
                            }
                        }
                        localProgress = false;
                        break;
                    }
                }

                if(localProgress)
                {
                    p->heapvaddr -= TLBsizes[x] - exclsize;
                    p->paddr_max -= TLBsizes[x] - exclsize;
                    
                    madeprogress = true;
                    return TLBsizes[x] - exclsize;
                }
            }
        }
#endif 
    }
    
    return 0;
}

int TLBMapper::createSpeculativeAliases(int x)
{
    if(mapperFlags[MAPPERFLAGS_NOALIASES])
        return 0;

    unsigned pid;
    unsigned set;
    unsigned way;
    unsigned maxway;
    unsigned slot;
    int newslot;
    int tlbsize;
    int specalias;
    for(specalias = 1; specalias <= 4 - (numproc/32)*2; specalias++)
    {
        pid = proc[x].mmu->CalculateAliasPID(proc[x].PID);
        if(pid == 0xffffffff)
            return -1;
        pid += (MAX_TLBSLOTS/MMU_ASSOCIATIVITY) * (specalias + 4);
        proc[x].SpecPID[specalias-1] = pid;
        for(set=0; set<MAX_TLBSLOTS/MMU_ASSOCIATIVITY; set++)
        {
            maxway = proc[x].mmu->tlbstate.GetActive(set);
            for(way=0; way<maxway; way++)
            {
                slot = set*MMU_ASSOCIATIVITY + way;
                if(((proc[x].mmu->tlbarray[slot].getType() == IS_HEAP) || (proc[x].mmu->tlbarray[slot].getType() == IS_DATA) || (proc[x].mmu->tlbarray[slot].getType() == IS_SHAR)) &&
                   (proc[x].PID == proc[x].mmu->tlbarray[slot].pid))
                {
                    for(tlbsize=0; tlbsize<NUMTLBSIZES; tlbsize++)
                    {
                        if(TLBsizes[tlbsize] == proc[x].mmu->tlbarray[slot].size)
                            break;
                    }
                    if(tlbsize == NUMTLBSIZES)
                    {
                        printf("Invalid TLB array size found\n");
                        return -1;
                    }
                    
                    newslot = proc[x].mmu->tlbstate.AcquireSlot(pid, proc[x].mmu->tlbarray[slot].vaddr, tlbsize);
                    if(newslot != -1)
                    {
                        memcpy(&proc[x].mmu->tlbarray[newslot], &proc[x].mmu->tlbarray[slot], sizeof(TLBslot));
                        proc[x].mmu->tlbarray[newslot].setType(IS_SPEC);
                        proc[x].mmu->tlbarray[newslot].pid = pid;
                        proc[x].mmu->tlbarray[newslot].paddr += 64*1024*specalias;
                    }
                    else
                    {
                        //                        printf("Unable to create enough speculative aliases  (pid=%d  vaddr=%d  tlbsize=%d)\n", pid, proc[x].mmu->tlbarray[slot].vaddr, tlbsize);
                        //                        proc[x].PrintTLBs(this);
                        return -1;
                    }
                }
                
            }
        }
    }
    return 0;
}

int TLBMapper::MapUserMMIO()
{
    int x, slot;
    Segment kernel;
    kernel.valid = true;
    kernel.process   = -1;
    kernel.coremask  = VMM_SEGMENTCOREMASK_ALL;
    kernel.contains_heap = false;
    kernel.pinned_vaddr = true;
    kernel.isMapped = true;
    kernel.type  = IS_USERMMIO;
    kernel.loaded  = true;
    int pid = 0;
    
    for(x=0; x<numproc; x++)
    {
        pid = proc[x].PID;
        if(numproc >= 64)
        {
            pid = 0;
            if((x % 4 != 0) && (x != numproc))
                continue;
        }
        slot = proc[x].mmu->AcquireSlot(pid, VA_MINADDR_MMIO/CONFIG_CNK_PAGE_SIZE, 0);
        if(slot != -1)
        {
            proc[x].mmu->tlbarray[slot].vaddr = kernel.origvaddr = kernel.vaddr = VA_MINADDR_MMIO/CONFIG_CNK_PAGE_SIZE;
            proc[x].mmu->tlbarray[slot].paddr = kernel.source_paddr             = PHYMAP_MINADDR_MMIO/CONFIG_CNK_PAGE_SIZE;
            proc[x].mmu->tlbarray[slot].size  = kernel.origsize= kernel.size    = 1024;
            proc[x].mmu->tlbarray[slot].exclsize = 0;
            proc[x].mmu->tlbarray[slot].setSegment(&kernel);
        }
    }
    return 0;
}


int TLBMapper::partition_without_hole(uint32_t* heap)
{
    int x;
    curWaste = 0;
    *heap = 0;
    PHASE = 0;
    uint32_t minHeapSize = UINT_MAX;
    uint32_t maxHeapSize = 0;
    
    preprocessSegments();
    if(initializeProcesses())
        return -1;
    CalculatePID();
    MapUserMMIO();
    
    // Allocate app segments
    if(allocateAppTLBs())
        return -1;
    
    PHASE = 1;
    // Allocate heap segments
    if(allocateAppTLBs())
        return -1;
    
    for(x=0; x<numsegs; x++)
    {
        
        if((segs[x].type == IS_DATA) && (segs[x].allowHeap == true))
        {
            *heap += segs[x].size - segs[x].origsize;
            if (segs[x].size < minHeapSize)
                minHeapSize = segs[x].size;
            if (segs[x].size > maxHeapSize)
                maxHeapSize = segs[x].size;
        }
    }
    // If heap allocations between processes indicate a major imbalance, return unsuccessful indication
#if 0
    printf("partition_without_hole heap min/max: %d, %d\n",minHeapSize, maxHeapSize);
#endif
    if (!isHeapBalanced(minHeapSize, maxHeapSize))
        return -1;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    if(MapAtomics(0, IS_SHAR) != 0)
        return -1;
    
    for(x=0; x<numproc; x++)
    {
        if(MapAtomics(x, IS_DATA) != 0)
            return -1;
        if(MapAtomics(x, IS_HEAP) != 0)
            return -1;
        if(createSpeculativeAliases(x) != 0)
            return -1;
    }
#endif                                                                                                                 // FUSEDOS
    
    return 0;
}

int TLBMapper::partition_with_hole(uint32_t* totalheap)
{
    int x;
    int p;
    unsigned y;
    uint32_t origheapvaddr[MAX_PROCESS_COUNT];
    uint32_t origpcur[MAX_PROCESS_COUNT];
    uint32_t origpmax[MAX_PROCESS_COUNT];
    uint32_t origheap[MAX_PROCESS_COUNT];
    
    uint32_t heap[MAX_PROCESS_COUNT] = {0,};
    uint32_t minHeapSize = UINT_MAX;
    uint32_t maxHeapSize = 0;
    bool madeprogress; 
    bool mapfailure;
    bool mappedShared;
    uint32_t tlbsize;
    
    PHASE = 2;
    
    curWaste = 0;
    
    *totalheap = 0;
    preprocessSegments();
    if(initializeProcesses())
        return -1;
    CalculatePID();
    MapUserMMIO();
    
    mapPinnedSegments();
    
    mappedShared = false;
    for(x=0; x<numsegs; x++)
    {
        if(isMapped(&segs[x]) && segs[x].type == IS_SHAR)
        {
            mappedShared = true;
        }
    }
    
    // Allocate app segments
    if(allocateAppTLBs())
        return -1;
    
    for(x=0; x<numproc; x++)
    {
        if(MapAtomics(x, IS_DATA))
            return -1;
    }
    if(mappedShared == false)
    {
        if(MapAtomics(0, IS_SHAR))
            return -1;
    }
    
    for(x=0; x<numproc; x++)
    {
        heap[x] = 0;
        proc[x].mmu->SnapshotState(tmp_mmustate[x]);
    }
    
    int startProc = 0;
    int endProc   = orignumproc;
    if(mapperFlags[MAPPERFLAGS_COMMONHEAP])
    {
        startProc = numproc - 1;
        endProc   = numproc;
    }
    for(x=startProc; x<endProc; x++)
    {
        tlbsize = 0;
        for(y=0; y<NUMTLBSIZES; y++)
        {
#if USE_EXCLUSION
            if(proc[x].paddr_max - proc[x].paddr_cur >= TLBsizes[y]/2)  // using exclusion, half of the TLB range could be excludible
#else
            if(proc[x].paddr_max - proc[x].paddr_cur >= TLBsizes[y])
#endif
            {
                tlbsize = y;
                break;
            }
        }
        if(y==NUMTLBSIZES)
        {
            return -1;
        }
        CalculateMaxHeapVaddr(x, tlbsize, (proc[x].paddr_max-proc[x].paddr_cur) / TLBsizes[tlbsize]);
    }
    for(x=0; x<numproc; x++)
    {
        proc[x].mmu->RestoreState(tmp_mmustate[x]);
    }
    
    // Allocate heap segments
    do
    {
        madeprogress = false;
        for(x=0; x<numproc; x++)
        {
            proc[x].mmu->SnapshotState(tmp_mmustate[x]);
            origheapvaddr[x] = proc[x].heapvaddr;
            origpmax[x] = proc[x].paddr_max;
            origpcur[x] = proc[x].paddr_cur;
            origheap[x] = heap[x];
        }
        for(x=startProc; x<endProc; x++)
        {
            if(mapperFlags[MAPPERFLAGS_COMMONHEAP])
            {
                heap[x] += allocateHeapTLBs(&proc[x], 0, orignumproc, madeprogress);
            }
            else
            {
                heap[x] += allocateHeapTLBs(&proc[x], x, x+1, madeprogress);
            }
#if MAP_CATOMICS
            if((heap[x]-origheap[x] >1) && (heap[x]-origheap[x] <= 16))
                heap[x] += allocateHeapTLBs(&proc[x], startProc, endProc, madeprogress);
#endif
        }
        for(x=0; x<numproc; x++)
        {
            proc[x].mmu->SnapshotState(tmp_mmustate2[x]);
        }
        
        mapfailure = false;
        for(x=startProc; x<endProc; x++)
        {
            if(MapAtomics(x, IS_HEAP) == -1)
            {
                mapfailure = true;

                if(mapperFlags[MAPPERFLAGS_COMMONHEAP])
                {
                    proc[numproc-1].paddr_cur = (origpmax[numproc-1] + origpcur[numproc-1]+1)/2;
                    if(proc[numproc-1].paddr_cur == origpmax[numproc-1])
                        madeprogress = false;
                }
                else
                {
                    for(p=startProc; p<endProc; p++)
                    {
                        if(proc[x].mmu == proc[p].mmu)
                        {
                            proc[p].paddr_cur = (origpmax[p] + origpcur[p]+1)/2;
                            if(proc[p].paddr_cur == origpmax[p])
                                madeprogress = false;
                        }
                    }
                }
            }
            else if(createSpeculativeAliases(x) == -1)
            {
                mapfailure = true;
                if(mapperFlags[MAPPERFLAGS_COMMONHEAP])
                {
                    proc[numproc-1].paddr_cur = (origpmax[numproc-1] + origpcur[numproc-1]+1)/2;
                    if(proc[numproc-1].paddr_cur == origpmax[numproc-1])
                        madeprogress = false;
                }
                else
                {
                    for(p=startProc; p<endProc; p++)
                    {
                        if(proc[x].mmu == proc[p].mmu)
                        {
                            proc[p].paddr_cur = (origpmax[p] + origpcur[p]+1)/2;
                            if(proc[p].paddr_cur == origpmax[p])
                                madeprogress = false;
                        }
                    }
                }
            }
        }
        if(mapfailure)
        {
            for(x=0; x<numproc; x++)
            {
                proc[x].mmu->RestoreState(tmp_mmustate[x]);
                proc[x].paddr_max = origpmax[x];
                proc[x].heapvaddr = origheapvaddr[x];
                heap[x] = origheap[x];
            }
            continue;
        }
        else
        {         
            for(x=0; x<numproc; x++)
            {
                proc[x].mmu->RestoreState(tmp_mmustate2[x]);
            }
        }
    } while(madeprogress);
    
    for(x=startProc; x<endProc; x++)
    {
        if(createSpeculativeAliases(x))
            return -1;
        if(MapAtomics(x, IS_HEAP))
            return -1;
    }
    
    for(x=startProc, *totalheap=0; x<endProc; x++)
    {
        if((heap[x] == 0) && (!proc[x].isAgent))
        {
            // printf("Process %d has no heap\n");
            return -1;
        }
        *totalheap += heap[x];
        // collect min and max heap sizes
        if (heap[x] < minHeapSize)
            minHeapSize = heap[x];
        if (heap[x] > maxHeapSize)
            maxHeapSize = heap[x];
    }
    if(minHeapSize == 0)
        return -1;
    
    for(x=0; x<numproc; x++)
    {
        if(proc[x].isAgent)
        {
            if(MapScrubWindow(x))
                return -1;
            break;
        }
    }
    
    // If heap allocations between processes indicate a major imbalance, return unsuccessful indication
#if 0
    printf("partition_with_hole heap min/max: %d, %d\n",minHeapSize, maxHeapSize);
#endif
    if (!isHeapBalanced(minHeapSize, maxHeapSize))
        return -1;
    
    return 0;
}

int bestMode = -1;

/*! \brief Performs the TLB mapping using the partitioned mapper algorithm
 */
int TLBMapper::partitioned_mapper(_BGP_VMM_RasData* pRasData)
{
    int x;
    uint32_t totalheap  = 0;
    int minappsize = 0;
    int minappsize2= 0 ;
    bool success   = false;
    unsigned  waste;
    
    unsigned minwasted = UINT_MAX;
    unsigned minwastedMode[2] = {UINT_MAX, UINT_MAX};
    unsigned bestwaste = UINT_MAX;
    bestMode = -1;

    Kernel_WriteFlightLog(FLIGHTLOG, FL_TLBMAPPER, 0,0,0,0);
    for(x=0; x<numsegs; x++)
    {
        minappsize += segs[x].origsize;
        
        if(segs[x].process == 0)    // non-shared, process 0 specific data.
            minappsize2 += segs[x].origsize;
    }
    
    orignumproc = numproc;
    extraprocess = 0;
    do
    {
#if 0
        printf("MAPPER%8p extraprocess=%d\n", this, extraprocess);
#endif
        if(extraprocess == MAX_EXTRAPROCESS_LIMIT)
            break;
        
        lastIteration = false;
        for(maxWaste=WASTE_START; (maxWaste<=WASTE_LIMIT)&&(lastIteration==false); maxWaste += ((maxWaste<4)?1:maxWaste/2))
        {
            if((maxWaste == WASTE_LIMIT) || (maxWaste > minwasted))
            {
                if(success)
                {
                    maxWaste = bestwaste;
                }
                lastIteration=true;
            }
            
#if USE_PARTITION_WITHOUT_HOLE
            if((lastIteration == false)||(bestMode == 0))
            {
                if(partition_without_hole(&totalheap) == 0)
                {
                    if (totalmem >= totalheap+minappsize+kernelsize)
                    {
                        success = true;	    
                        waste = totalmem-totalheap-minappsize-kernelsize;
                        if(minwastedMode[0] > waste)
                            minwastedMode[0] = waste;
                        if(minwasted > waste)
                        {
                            minwasted = waste;
                            bestwaste = maxWaste;
                            bestMode  = 0;
                            
                            if(minwasted == 0) // early exit since minwasted can't be less-than-zero (though its possible another mapping uses fewer TLBs)
                                break;
                            if(maxWaste*2 < minwasted)
                            {
                                maxWaste = minwasted / 3;
                            }
                        }
                    }
                }
            }
#endif
            
#if USE_PARTITION_WITH_HOLE
            if((lastIteration == false)||(bestMode == 1))
            {
                if(partition_with_hole(&totalheap) == 0)
                {
                    success = true;	    
                    waste = totalmem-totalheap-minappsize-kernelsize;
                    if(minwastedMode[1] > waste)
                        minwastedMode[1] = waste;
                    if(minwasted > waste)
                    {
                        minwasted = waste;
                        bestwaste = maxWaste;
                        bestMode  = 1;
                        
                        if(minwasted == 0) // early exit since minwasted can't be less-than-zero (though its possible another mapping uses fewer TLBs)
                            break;
                        if(maxWaste*2 < minwasted)
                        {
                            maxWaste = minwasted / 3;
                        }
                    }
                }
            }
#endif
        }
    }
    while((!success)&&((minappsize2 + kernelsize + 1) < (totalmem >> (++extraprocess))));
    
    if(!success)
    {
        extraprocess = 0;
        initializeProcesses();
        Kernel_WriteFlightLog(FLIGHTLOG, FL_TLBMAPFAI, minappsize, minappsize2, 0, 0);
        return -1;
    }
    
    ApplyMapperPhysicalOffset();
    
    if(extraprocess)
    {
        numproc = orignumproc + numagentproc;
        extraprocess = 0;
    }
    
    Kernel_WriteFlightLog(FLIGHTLOG_high, FL_TLBMAPCMP, totalmem-totalheap-minappsize-kernelsize,totalheap,bestwaste,bestMode);
    
    TRACE(TRACE_StaticMap, ("MAPPER%8p Wasted space: %d  (totalmem=%d  totalheap=%d  numproc=%d  minappsize=%d  kernelsize=%d  bestwaste=%d  minwasted=%d  bestmode=%d  mode0=%d  mode1=%d)\n", this, totalmem-totalheap-minappsize-kernelsize, totalmem, totalheap, numproc, minappsize, kernelsize, bestwaste, minwasted, bestMode, minwastedMode[0], minwastedMode[1]));
    for(x=0; x<numproc; x++)
    {
        uint64_t vaddr, paddr;
        size_t vsize;
        vaddr=paddr=vsize=0;
        getSegment(x, IS_HEAP, &vaddr, &paddr, &vsize);
        TRACE(TRACE_StaticMap, ("MAPPER%8p process %d (pid=%d) HEAP vaddr=%lx paddr=%lx vsize=%ld\n", this, x, proc[x].PID, vaddr, paddr, vsize/CONFIG_CNK_PAGE_SIZE));
        if( ((threadsperprocess==1) && ((x % 4) == 3)) ||
            ((threadsperprocess==2) && ((x % 2) == 1)) ||
            ((threadsperprocess>2)) ||
            (x >= orignumproc))
        {
            proc[x].PrintTLBs(this);
        }
    }
    return 0;
}


#if !STANDALONE
extern "C" 
{
void Kernel_Barrier_Internal(Lock_Atomic_t* pLock, uint32_t rendezvous_num);
}

int TLBMapper::allocateAtomic(unsigned pindex, uint64_t vaddr, size_t* sizeMapped)
{
    int rc;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int core;
#endif                                                                                                                 // FUSEDOS
    bool foundVaddr = false;
    unsigned atomvaddr = ((vaddr<<5) | CONFIG_APP_L2ATOMIC_BASE)/CONFIG_CNK_PAGE_SIZE;
    uint64_t paddr = 0;
    unsigned PID = 0xffffffff;
    MMU* coremmu = proc[pindex].mmu;
    int slot;
    unsigned x;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    unsigned xx;
#endif                                                                                                                 // FUSEDOS
    int y;

    
    Kernel_WriteFlightLog(FLIGHTLOG_high, FL_L2ATOMICS, (uint64_t)pindex, vaddr, (uint64_t)*sizeMapped, 0);
    *sizeMapped = 0;
    for(x=0; x<NUMTLBSLOTS; x++)
    {
        if(coremmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > x%MMU_ASSOCIATIVITY)
        {
            if(((coremmu->tlbarray[x].vaddr+coremmu->tlbarray[x].exclsize)*1ULL*CONFIG_CNK_PAGE_SIZE <= vaddr) && 
               ((coremmu->tlbarray[x].vaddr+coremmu->tlbarray[x].size)*1ULL*CONFIG_CNK_PAGE_SIZE > vaddr) && 
               ((coremmu->tlbarray[x].pid == 0) || (coremmu->tlbarray[x].pid == proc[pindex].PID)))
            {
                foundVaddr = true;
                PID = coremmu->tlbarray[x].pid;
                paddr = coremmu->tlbarray[x].paddr*1ULL*CONFIG_CNK_PAGE_SIZE + (vaddr - coremmu->tlbarray[x].vaddr * 1ULL * CONFIG_CNK_PAGE_SIZE);
#if defined __FUSEDOS__                                                                                               // FUSEDOS
                TRACE(TRACE_StaticMap, ("FUSEDOS allocateAtomic: paddr 0x%lx, tlbarray paddr 0x%x\n",                 // FUSEDOS
                      paddr, coremmu->tlbarray[x].paddr ));                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            }
            if((coremmu->tlbarray[x].vaddr <= atomvaddr) && 
               (coremmu->tlbarray[x].vaddr+coremmu->tlbarray[x].size > atomvaddr) && 
               ((coremmu->tlbarray[x].pid == 0) || (coremmu->tlbarray[x].pid == proc[pindex].PID)))
            {
                coremmu->tlbarray[x].refcnt++;
                *sizeMapped = (((coremmu->tlbarray[x].vaddr+coremmu->tlbarray[x].size) * 1ULL * CONFIG_CNK_PAGE_SIZE) & (~CONFIG_APP_L2ATOMIC_BASE)) / 32 - vaddr;
                Kernel_WriteFlightLog(FLIGHTLOG_high, FL_L2ATOMICS, (uint64_t)pindex, vaddr, (uint64_t)*sizeMapped, 1);
                return 0;
            }
        }
    }
    if(! foundVaddr)
    {
        Kernel_WriteFlightLog(FLIGHTLOG_high, FL_L2ATOMICS, (uint64_t)pindex, vaddr, (uint64_t)*sizeMapped, 2);
        return EFAULT;
    }
    vaddr = vaddr/CONFIG_CNK_PAGE_SIZE;
    paddr = 0x00100000 + (paddr << 5)/CONFIG_CNK_PAGE_SIZE; // addition required
    
    for(x=0; x<NUMTLBSIZES; x++)
    {
        if (((atomvaddr ^ paddr) & (TLBsizes[x]-1)) != 0)
        {
            // virtual and physical addresses have different page offsets
            continue;
        }
        unsigned tst_vaddr = atomvaddr & ~(TLBsizes[x]-1);
        unsigned tst_paddr = paddr & ~(TLBsizes[x]-1);

        if((tst_paddr + TLBsizes[x]) > (0x00100000 + (totalmem << 5)))
        {
            // mapping would extend beyond physical memory
            continue;
        }
        
        uint64_t exclsize = 0;
        if(tst_paddr < (0x00100000 + (kernelsize << 5)))
        {
            if((kernelsize << 5) >= TLBsizes[x])  // exclude region is larger than TLB, bail.
                continue;
            
            exclsize = (kernelsize << 5);
            // mapping would overlap the kernel, exclude the kernel region
        }
        
        slot = proc[pindex].mmu->AcquireSlot(PID, tst_vaddr, x);
        if(slot != -1)
        {
            proc[pindex].mmu->FreeSlot(slot);
            Segment atomic;
            atomic.valid = true;
            atomic.process   = ((PID==0)?-1:pindex);
            atomic.coremask  = VMM_SEGMENTCOREMASK_ALL;
            atomic.contains_heap = false;
            atomic.pinned_vaddr = true;
            atomic.isMapped = true;
            atomic.type  = IS_ATOMICRGN;
            atomic.origsize = atomic.size = totalmem;
            atomic.sharedPID = (PID==0);
            atomic.origvaddr = atomic.vaddr = tst_vaddr;
            atomic.source_paddr = tst_paddr;
            
            for(y=0; y<numproc; y++)
            {
                proc[y].mmu->SnapshotState(tmp_oldstate[y]);
            }
            
            rc = MakeTLB(&atomic, tst_vaddr, tst_paddr, x, exclsize);
            if(rc == 0)
            {
                rc = installStaticEntry(pindex, slot);
                if(rc == 0)
                {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
                    for(core=0; (unsigned)core<NodeState.NumCoresEnabled; core++)
                    {
                        if(ProcessorCoreID() == core)  // already setup this core, skip
                            continue;
                        
                        if(((unsigned)core == NodeState.NumCoresEnabled-1) && (numagentproc == 0))  // no agents on 17th core, skip
                            continue;
                        
                        AppProcess_t* p = GetProcessByProcessorID(core*4);   // no process active on core, skip
                        if(p == NULL)
                            continue;
                        
                        coremmu = proc[translate2pindex(p->Tcoord)].mmu;
                        
                        for(xx=0; xx<NUMTLBSLOTS; xx++)
                        {
                            if(coremmu->tlbstate.GetActive(xx/MMU_ASSOCIATIVITY) > xx%MMU_ASSOCIATIVITY)
                            {
                                if((coremmu->tlbarray[xx].vaddr == tst_vaddr) &&
                                   (coremmu->tlbarray[xx].pid == PID))
                                {
                                    ppc_msync();
                                    IPI_update_MMU(core*4, (((uint64_t)p->Tcoord)<<32) | xx);
                                    break;
                                }
                            }
                        }
                    }
#endif                                                                                                                 // FUSEDOS
                    *sizeMapped = (TLBsizes[x] - (atomvaddr - tst_vaddr)) * 1ULL * CONFIG_CNK_PAGE_SIZE / 32;
                    Kernel_WriteFlightLog(FLIGHTLOG_high, FL_L2ATOMICS, (uint64_t)pindex, vaddr, (uint64_t)*sizeMapped, 3);
                    return 0;
                }
            }
            else
            {
                for(y=0; y<numproc; y++)
                {
                    proc[y].mmu->RestoreState(tmp_oldstate[y]);
                }
            }
        }
    }
    Kernel_WriteFlightLog(FLIGHTLOG_high, FL_L2ATOMICS, (uint64_t)pindex, vaddr, (uint64_t)*sizeMapped, 4);
    return ENOMEM;
}

#endif

int TLBMapper::MapScrubWindow(int p)
{
    int rc = 0;
    int64_t minslot = -1;
    uint64_t mincount= 999;
    uint64_t slot;
    unsigned int newindex;
    for(slot=0; slot<NUMTLBSLOTS; slot++)
    {
        if(proc[p].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY) < mincount)
        {
            mincount = proc[p].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY);
            minslot  = slot;
        }                
    }
    if(minslot == -1)
        return -1;
    
    uint64_t vaddr = 0x2000000000000ull | ((minslot/MMU_ASSOCIATIVITY)<<(63-33));
    uint64_t paddr = 0;
    uint64_t size  = 1024;
    
    addSegment(IS_SCRUBWINDOW, vaddr/CONFIG_CNK_PAGE_SIZE, 0, size, -1, VMM_SEGMENTCOREMASK_16, true, false, true, false, true, false, 1, newindex);
    rc |= MapRange(&segs[newindex], vaddr/CONFIG_CNK_PAGE_SIZE, paddr, size);
    segs[newindex].valid = false;
    
    return rc;
}

int TLBMapper::MapAtomics(int p, enum SegmentType type)
{
    int rc = 0;
#if MAP_CATOMICS
    bool retain = false;
    unsigned int newindex;
    if(totalmem <= 4096)
    {
	uint64_t vaddr = 0;
	uint64_t paddr = 0;
	size_t size    = 0;
        
        rc = getSegment(p, type, &vaddr, &paddr, &size);
        vaddr /= CONFIG_CNK_PAGE_SIZE;
        paddr /= CONFIG_CNK_PAGE_SIZE;
        size  /= CONFIG_CNK_PAGE_SIZE;
        
        if(size > 0)
        {
            vaddr = 0x40000000 | (vaddr << 5);
            paddr = 0x00100000 + (paddr << 5); // addition required
            size = size<<5;
            switch(type)
            {
                case IS_DATA:
                    addSegment(IS_ATOMICRGN, vaddr, paddr, size, p, VMM_SEGMENTCOREMASK_ALL, true, false, true, true, false, false, 1, newindex); 
                    break;
                case IS_HEAP:
                    addSegment(IS_ATOMICRGN, vaddr, paddr, size, p, VMM_SEGMENTCOREMASK_ALL, true, false, true, false, false, false, 1, newindex); 
                    break;
                case IS_SHAR:
                    addSegment(IS_ATOMICRGN, vaddr, paddr, size, -1, VMM_SEGMENTCOREMASK_ALL, true, false, true, false, ((numproc>=64)?true:false), false, 1, newindex); 
                    break;
                default:
                    printf("invalid type=%d\n", type);
                    return -1;
            }
            rc |= MapRange(&segs[newindex], vaddr, paddr, size);
            if(retain == false)
            {
                segs[newindex].valid = false;
            }
        }
    }
#else
    uint64_t vaddr = 0;
    uint64_t paddr = 0;
    size_t size    = 0;
    unsigned int newindex;
    
    if(type == IS_SHAR)
    {
        rc = getSegment(p, type, &vaddr, &paddr, &size);
        vaddr /= CONFIG_CNK_PAGE_SIZE;
        paddr /= CONFIG_CNK_PAGE_SIZE;
        size  /= CONFIG_CNK_PAGE_SIZE;
        if(size >= CNK_STATICSHMATOMICSIZE)
        {
            vaddr += size;
            paddr += size;
            size = CNK_STATICSHMATOMICSIZE;
            vaddr -= size;
            paddr -= size;
            
            vaddr = 0x40000000 | (vaddr << 5);
            paddr = 0x00100000 + (paddr << 5); // addition required                                                                                                                                                                    
            size = size<<5;
            addSegment(IS_ATOMICRGN, vaddr, paddr, size, -1, VMM_SEGMENTCOREMASK_ALL, true, false, true, false, ((numproc>=64)?true:false), false, 1, newindex);
            rc |= MapRange(&segs[newindex], vaddr, paddr, size);
            segs[newindex].valid = false;
        }
    }
#endif
    return rc;
}

int TLBMapper::ApplyMapperPhysicalOffset()
{
#ifndef __FUSEDOS__                                                                                  // FUSEDOS
    int x;
    uint32_t slot;
    for(x=0; x<numproc; x++)
    {
        for(slot=0; slot<NUMTLBSLOTS; slot++)
        {
            proc[x].mmu->tlbarray[slot].paddr += mapper_paddr_offset;
        }
    }
#endif                                                                                               // FUSEDOS
    return 0;
}

/*! \brief Returns true/false if the heap allocations across processes are acceptable
 */
bool TLBMapper::isHeapBalanced(uint32_t minHeapSize, uint32_t maxHeapSize)
{
#if USE_HEAP_BALANCER
    // If the size of the heap segments vary greatly between processes return false
    if ((maxHeapSize / HEAP_DELTA_THRESHOLD) > minHeapSize)
        return false;
#endif
    return true;
}

int TLBMapper::CalculateCopymap()
{
    int p;
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int slot;
    unsigned int offset;
    int numcopies;
    memset(copymap, 0xff, sizeof(((TLBMapperSharedData*)0)->copymap));
    memset(srcdemand, 0x00, sizeof(((TLBMapperSharedData*)0)->srcdemand));
    memset(eraselock, 0x00, sizeof(((TLBMapperSharedData*)0)->eraselock));
    for(p=0; p<numproc; p++)
    {
//            printf("processing process %d\n", p);
        for(slot=0; slot<NUMTLBSLOTS; slot++)
        {
            if(proc[p].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY) <= slot%MMU_ASSOCIATIVITY)
                continue;
            
            if((proc[p].mmu->tlbarray[slot].getType() == IS_KERNEL) || 
               (proc[p].mmu->tlbarray[slot].getType() == IS_MMIO) ||
               (proc[p].mmu->tlbarray[slot].getType() == IS_SPEC) )
                continue;
            if(proc[p].mmu->tlbarray[slot].loaded)
            {
//                	    printf("pinned_vaddr = proc%d slot%d\n", p, slot);
 //               printf("spaddr=%d  vaddr=%d  excl=%d  origvaddr=%d\n", proc[p].mmu->tlbarray[slot].source_paddr, proc[p].mmu->tlbarray[slot].vaddr, proc[p].mmu->tlbarray[slot].exclsize, proc[p].mmu->tlbarray[slot].origvaddr);
                for(y=proc[p].mmu->tlbarray[slot].source_paddr + proc[p].mmu->tlbarray[slot].vaddr + proc[p].mmu->tlbarray[slot].exclsize - proc[p].mmu->tlbarray[slot].origvaddr, offset=proc[p].mmu->tlbarray[slot].exclsize; 
#ifdef __FUSEDOS__ // FUSEDOS
                	offset<proc[p].mmu->tlbarray[slot].size  && y < (totalmem + mapper_paddr_offset);
#else // EXAPRORO
                    offset<proc[p].mmu->tlbarray[slot].size  && y<totalmem; 
#endif // FUSEDOS
                    y++, offset++)
                {
                    if(copymap[proc[p].mmu->tlbarray[slot].paddr + offset] == DO_NOT_COPY)
                    {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
                        z = y;                                                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
                        z = y + mapper_paddr_offset;
#endif                                                                                                                 // FUSEDOS
//                        		  printf("copymap[%d] = %d\n", proc[p].mmu->tlbarray[slot].paddr + offset, z);
                        copymap[proc[p].mmu->tlbarray[slot].paddr + offset] = z;   // copymap[ destination_paddr ] = source_paddr
                        srcdemand[z]++;
                    }
                }
            }
        }
    }
#if 0
    printf("kernelsize=%d\n", kernelsize);
    printf("totalmem=%d\n", totalmem);
    printf("maxmem=%d\n", MAX_MEMORY_PER_NODE);
    for(x=0+mapper_paddr_offset; x<totalmem+mapper_paddr_offset; x++)
    {
        if((copymap[x] != DO_NOT_COPY) || srcdemand[x] || eraselock[x])
        {
            printf("data: x=%d   %d %d %d\n", x, copymap[x], srcdemand[x], eraselock[x]);
        }
    }
#endif  
    
#define RELOCATEOP(type, x, y) (( ((uint64_t)type)<<32)|( ((uint64_t)x)<<16)|( ((uint64_t)y)))
    numrelocations = 0;
    do 
    {
        numcopies = 0;
        for(x=kernelsize+mapper_paddr_offset; x<totalmem+mapper_paddr_offset; x++)
        {
            if(copymap[x] == x)
            {
                numcopies++;
                srcdemand[x]--;
                eraselock[x] = 1;
                copymap[x] = DO_NOT_COPY;
            }
            else if((copymap[x] != DO_NOT_COPY) && (srcdemand[x] == 0)) // need to copy data into this location, and no one needs data from it.
            {
                numcopies++;
                relocate[numrelocations++] = RELOCATEOP(1, copymap[x], x);
//              copySegment(copymap[x], x);
                
                srcdemand[copymap[x]]--;
                
                if((srcdemand[copymap[x]] == 0)&&(eraselock[copymap[x]] == 0))
                {
                    relocate[numrelocations++] = RELOCATEOP(2, copymap[x], 0);
//                  eraseSegment(copymap[x]);
                }
                copymap[x] = DO_NOT_COPY;
            }
            else if((copymap[x] != DO_NOT_COPY) && (copymap[copymap[x]] == x) && (srcdemand[x] == 1) && (srcdemand[copymap[x]] == 1))
            {
                numcopies+=2;
//                exchangeSegment(copymap[x], x);
                relocate[numrelocations++] = RELOCATEOP(3, copymap[x], x);
                
                srcdemand[copymap[x]]--;
                srcdemand[x]--;
                copymap[copymap[x]] = DO_NOT_COPY;
                copymap[x] = DO_NOT_COPY;
            }
        }
    }
    while(numcopies);
    
    for(x=kernelsize+mapper_paddr_offset; x<totalmem+mapper_paddr_offset; x++)
    {
        if((copymap[x] != DO_NOT_COPY)&&(x != copymap[x]))
        {
//            printf("exchange %d and %d  (%d is in the right place, %d is not)\n", copymap[x], x, copymap[x], x);
//            exchangeSegment(copymap[x], x);
            relocate[numrelocations++] = RELOCATEOP(3, copymap[x], x);
            
            srcdemand[copymap[x]]--;
            for(y=kernelsize; y<MAX_MEMORY_PER_NODE; y++)
            {
                if(copymap[y] == x)
                {
                    copymap[y] = copymap[x];
                    srcdemand[copymap[x]] = srcdemand[x];
                }
            }
            copymap[x] = DO_NOT_COPY;
        }
    }
    
    copymapCalculated = true;
    return 0;
}

#if !STANDALONE

int TLBMapper::copySegment(uint32_t src, uint32_t dst)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int rc;
#endif                                                                                                                 // FUSEDOS
    const size_t segsize = CONFIG_CNK_PAGE_SIZE;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    char* srcaddr = (char*)(0x80000000);
    char* dstaddr = (char*)(0x90000000);
    
    rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                               (void*)(src * segsize),
                               srcaddr,
                               segsize,
                               0,
                               0);                                                                                 
    if(rc)
    {
        printf("copySegment could not map: src %d segsize=%ld\n", src, segsize);
    }
    rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)(dst * segsize),
                          dstaddr,
                          segsize,
                          0,
                          0); 
    if(rc)
    {
        printf("copySegment could not map: dst %d segsize=%ld\n", dst, segsize);
    }
#else                                                                                                                  // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS copySegment: src %d, dst %d\n", src, dst));                                      // FUSEDOS
    char* srcaddr = (char*)(src * segsize);                                                                            // FUSEDOS
    char* dstaddr = (char*)(dst * segsize);                                                                            // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS copySegment: srcaddr %p, dstaddr %p\n", srcaddr, dstaddr));                      // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    memcpy(dstaddr, srcaddr, segsize);
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vmm_UnmapUserSpace(srcaddr, segsize, 0);
    vmm_UnmapUserSpace(dstaddr, segsize, 0);
#endif                                                                                                                 // FUSEDOS
    return 0;
}

int TLBMapper::eraseSegment(uint32_t src)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int rc;
#endif                                                                                                                 // FUSEDOS
    const size_t segsize = CONFIG_CNK_PAGE_SIZE;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    char* srcaddr = (char*)(0x80000000);

    rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                               (void*)(src * segsize),
                               srcaddr,
                          segsize,
                          0,
                          0);                                                                              
#else                                                                                                                  // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS eraseSegment: src %d\n", src));                                                  // FUSEDOS
    char* srcaddr = (char*)(src * segsize);                                                                            // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS eraseSegment: srcaddr %p\n", srcaddr));                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    memset(srcaddr, 0, segsize);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vmm_UnmapUserSpace(srcaddr, segsize, 0);
#endif                                                                                                                 // FUSEDOS
    
    return 0;
}

int TLBMapper::exchangeSegment(uint32_t src, uint32_t dst)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int rc;
    size_t segsize = CONFIG_CNK_PAGE_SIZE;
    char* srcaddr = (char*)(0x80000000);
    char* dstaddr = (char*)(0x90000000);
    
    rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)(src * segsize),
                          srcaddr,
                          segsize,
                          0,
                          0);                                                                               
    rc = vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)(dst * segsize),
                          dstaddr,
                          segsize,
                          0,
                          0);                                                                               
#else                                                                                                                  // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS exchangeSegment: src %d, dst %d\n", src, dst));                                  // FUSEDOS
    char* srcaddr = (char*)(src * CONFIG_CNK_PAGE_SIZE);                                                               // FUSEDOS
    char* dstaddr = (char*)(dst * CONFIG_CNK_PAGE_SIZE);                                                               // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS exchangeSegment: srcaddr %p, dstaddr %p\n", srcaddr, dstaddr));                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    uint64_t n;
    for (n = 0; n < (CONFIG_CNK_PAGE_SIZE / sizeof(uint64_t)); n++)
    {
	uint64_t tmp;
	tmp = ((uint64_t *) dstaddr)[n];
	((uint64_t *) dstaddr)[n] = ((uint64_t *) srcaddr)[n];
	((uint64_t *) srcaddr)[n] = tmp;
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vmm_UnmapUserSpace(srcaddr, segsize, 0);
    vmm_UnmapUserSpace(dstaddr, segsize, 0);
#endif                                                                                                                 // FUSEDOS
    return 0;
}


int TLBMapper::copySegments()
{
    int rc;
    unsigned int x;
    
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS copySegments: this %p, copymapCalculated %d, numrelocations %d\n",               // FUSEDOS
          this, copymapCalculated, numrelocations));                                                                   // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    if(copymapCalculated == false)
    {
        rc = CalculateCopymap();
        if(rc) 
            return rc;
    }
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS copySegments: copymapCalculated %d, numrelocations %d\n",                        // FUSEDOS
          copymapCalculated, numrelocations));                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    for(x=0; x<numrelocations; x++)
    {
//        printf("relocate[%d]:  opcode=%lx  src=%ld  dst=%ld\n", x, relocate[x]>>32, (relocate[x]>>16)&0xffff, (relocate[x]&0xffff));
        switch(relocate[x]>>32)
        {
            case 1:
                copySegment((relocate[x]>>16)&0xffff, relocate[x]&0xffff);
                break;
            case 2:
                eraseSegment((relocate[x]>>16)&0xffff);
                break;
            case 3:
                exchangeSegment((relocate[x]>>16)&0xffff, relocate[x]&0xffff);
                break;
            default:
                printf("Error in copySegments.  relocate[%d] = %lx\n", x, relocate[x]);
        }
    }
    
    return 0;
}

int TLBMapper::installStaticEntry(int pindex, unsigned x)
{
#if 0 && defined __FUSEDOS__                                                                                          // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS installStaticEntry: pindex %d, x %d\n", pindex, x));                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    int rc;
    MMU* coremmu = proc[pindex].mmu;
    int flags;
    size_t segsize = CONFIG_CNK_PAGE_SIZE;
    
    if(coremmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) <= x%MMU_ASSOCIATIVITY)
        return 0;

    switch(coremmu->tlbarray[x].getType())
    {
        case IS_TEXT:
            flags = APP_FLAGS_R | APP_FLAGS_X | APP_FLAGS_NONSPECULATIVE | APP_FLAGS_LISTENABLE;
            if(coremmu->tlbarray[x].loaded == false)
            {
//                flags |= APP_FLAGS_W;  // Note:  CNK writes the dynamically-loaded libraries, therefore do not need to make user-writable.
            }
            break;
        case IS_SHAR:
        case IS_DATA:
        case IS_DYNAM:
        case IS_HEAP:
            flags = APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_X | APP_FLAGS_LISTENABLE;
            break;
        case IS_KERNEL:
        case IS_MMIO:
            return 0;
        case IS_USERMMIO:
            flags = APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_INHIBITED | APP_FLAGS_GUARDED | APP_FLAGS_FLUSHSTORES;
            break;
        case IS_SPEC:
            flags = APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_X;
            break;
        case IS_PROCESSWINDOW:
            flags = APP_FLAGS_R | APP_FLAGS_LISTENABLE;
            break;
        case IS_ATOMICRGN:
            flags = APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_INHIBITED;
            break;
        case IS_SCRUBWINDOW:
            flags = APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_INHIBITED | APP_FLAGS_GUARDED;
            break;
        case IS_RAMDISK:
            flags = APP_FLAGS_NONSPECULATIVE;
            break;
        default:
            printf("MAPPER%8p Invalid segment type.  core=%d  slot=%d  type=%d\n", this, ProcessorCoreID(), x, coremmu->tlbarray[x].getType());
            return -1;
    }
    rc = vmm_MapUserSpace(flags  | APP_FLAGS_ESEL(x&3),
                          (void*)(coremmu->tlbarray[x].paddr * segsize),
                          (void*)(coremmu->tlbarray[x].vaddr * segsize),
                          coremmu->tlbarray[x].size  * segsize,
                          coremmu->tlbarray[x].pid,
                          coremmu->tlbarray[x].exclsize * segsize);
    return rc;
}

int TLBMapper::installStaticTLBMap(int pindex)
{
    unsigned int x;
    int rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vmm_tlb_touch_tlbs();
#endif                                                                                                                 // FUSEDOS
    
    TRACE(TRACE_StaticMap, ("MAPPER%8p installing static map for core %d\n", this, ProcessorCoreID()) );
    MMU* coremmu = proc[pindex].mmu;
    for(x=0; x<NUMTLBSLOTS; x++)
    {
        if(coremmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > x%MMU_ASSOCIATIVITY)
            rc = installStaticEntry(pindex, x);
        else
            rc = 0;
        if(rc)
        {
            printf("MAPPER%8p Error installing TLB\n", this);
            return rc;
        }
    }
    
    return 0;
}

int TLBMapper::installStaticTLBMapPartial(int pindex, uint32_t pid)
{
    unsigned int x;
    int rc = 0;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    vmm_tlb_touch_tlbs();
#endif                                                                                                                 // FUSEDOS
    
    TRACE(TRACE_StaticMap, ("MAPPER%8p installing static map for core %d\n", this, ProcessorCoreID()) );
    MMU* coremmu = proc[pindex].mmu;
    for(x=0; x<NUMTLBSLOTS; x++)
    {
        if((coremmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > x%MMU_ASSOCIATIVITY) &&
           (coremmu->tlbarray[x].pid == pid))
        {
            rc = installStaticEntry(pindex, x);
        }
        else
            rc = 0;
        if(rc)
        {
            printf("MAPPER%8p Error installing TLB\n", this);
            return rc;
        }
    }
    
    return 0;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS

int TLBMapper::uninstallStaticTLBMap(int pindex)
{
    unsigned int x;
    int rc = 0;
    size_t segsize = CONFIG_CNK_PAGE_SIZE;
    
    vmm_tlb_touch_tlbs();
    MMU* coremmu = proc[pindex].mmu;
    for(x=0; x<NUMTLBSLOTS; x++)
    {
        if(coremmu->tlbstate.GetActive(x/MMU_ASSOCIATIVITY) > x%MMU_ASSOCIATIVITY)
        {
            switch(coremmu->tlbarray[x].getType())
            {
                case IS_TEXT:
                case IS_SHAR:
                case IS_DATA:
                case IS_DYNAM:
                case IS_HEAP:    
                case IS_SPEC:
                case IS_PROCESSWINDOW:
                case IS_ATOMICRGN:
                case IS_RAMDISK:
                case IS_SCRUBWINDOW:
                case IS_USERMMIO:
                    break;
                case IS_KERNEL:
                case IS_MMIO:
                    continue;
                default:
                    printf("MAPPER%8p Invalid segment type.  core=%d  slot=%d  type=%d\n", this, ProcessorCoreID(), x, coremmu->tlbarray[x].getType());
                    return -1;
            }

            // Input virtual address needs to take exclusion range into account.
            uint64_t va = (coremmu->tlbarray[x].vaddr * segsize) + (coremmu->tlbarray[x].exclsize * segsize);
            rc = vmm_UnmapUserSpace((void*)va, coremmu->tlbarray[x].size  * segsize, coremmu->tlbarray[x].pid);

        }
        else
        {
            rc = 0;
        }        
        if(rc)
        {
            printf("MAPPER%8p Error installing TLB\n", this);
        }
    }
    
    return 0;
}
#endif                                                                                                                 // FUSEDOS


#endif


int TLBMapper::resetStaticMap(int numprocesses, int numagents, int core_index)
{
    Kernel_WriteFlightLog(FLIGHTLOG, FL_TLBMAPRST, numprocesses, numagents, core_index, 0);

    numsegs = 0;
    numproc = numprocesses;
    numagentproc = numagents;
    externalcount = 0;
    
    applayoutversion = 2;   
    memset(segs, 0, sizeof(segs));   
    
    NUMTLBSLOTS = MAX_TLBSLOTS;
    
    copymapCalculated = false;

    if (core_index < 0)
    {
        coremask_job = VMM_SEGMENTCOREMASK_ALL;
    }
    else
    {
        coremask_job = ((uint32_t)(1)) << core_index;
    }
    return 0;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#if !STANDALONE
int TLBMapper::getMaxVAWindow(int pid, unsigned int* vaddrmin, unsigned int* vaddrmax)
{
    return proc[pid].getMaxVAWindow(vaddrmin, vaddrmax);
}

#endif
#endif                                                                                                                 // FUSEDOS

int TLBMapper::getSegment(uint32_t pindex, enum SegmentType type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize)
{
    uint32_t min_vaddr, max_vaddr;
    uint32_t min_paddr;
    unsigned int slot;
    const size_t segsize = CONFIG_CNK_PAGE_SIZE;
    uint32_t pid = proc[pindex].PID;
    
    min_vaddr = min_paddr = 0xffffffff;
    max_vaddr = 0;
    
    if(proc[pindex].mmu == NULL)
    {
       *vaddr = 0;
       *paddr = 0;
       *vsize = 0;
       return -1;
    }
    for(slot=0; slot<NUMTLBSLOTS; slot++)
    {
        if(proc[pindex].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY) > slot%MMU_ASSOCIATIVITY)
        {   
            if((type == IS_HEAP) &&
	       ((proc[pindex].mmu->tlbarray[slot].pid == pid) || (proc[pindex].mmu->tlbarray[slot].pid==0)) &&
                (((proc[pindex].mmu->tlbarray[slot].getType() == IS_DATA) && 
                 (proc[pindex].mmu->tlbarray[slot].origsize != proc[pindex].mmu->tlbarray[slot].size))))
            {
                min_vaddr = MIN(min_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].exclsize + proc[pindex].mmu->tlbarray[slot].origsize);
                min_paddr = MIN(min_paddr, proc[pindex].mmu->tlbarray[slot].paddr + proc[pindex].mmu->tlbarray[slot].exclsize + proc[pindex].mmu->tlbarray[slot].origsize);
                max_vaddr = MAX(max_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].size);
            }
            else if((type == IS_HEAP) &&
	       ((proc[pindex].mmu->tlbarray[slot].pid == pid) || (proc[pindex].mmu->tlbarray[slot].pid==0)) &&
	       (proc[pindex].mmu->tlbarray[slot].getType() == IS_HEAP))
            {
                min_vaddr = MIN(min_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].exclsize);
                min_paddr = MIN(min_paddr, proc[pindex].mmu->tlbarray[slot].paddr + proc[pindex].mmu->tlbarray[slot].exclsize);
                max_vaddr = MAX(max_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].size);
            }
            else if((proc[pindex].mmu->tlbarray[slot].getType() == type) && ((proc[pindex].mmu->tlbarray[slot].pid == pid) || (proc[pindex].mmu->tlbarray[slot].pid==0)))
            {
                if(proc[pindex].mmu->tlbarray[slot].origsize != 0)
                {
                    min_vaddr = MIN(min_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].exclsize);
                    min_paddr = MIN(min_paddr, proc[pindex].mmu->tlbarray[slot].paddr + proc[pindex].mmu->tlbarray[slot].exclsize);
                    max_vaddr = MAX(max_vaddr, proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].size);
                }
            } 
        }
    }
    if(max_vaddr == 0)
    {
        *vaddr = 0;
        *paddr = 0;
        *vsize = 0;
    }
    else
    {
        *vaddr = min_vaddr * segsize;
        *paddr = ((unsigned long long)min_paddr * segsize);
        *vsize = (max_vaddr - min_vaddr) * segsize;
        
        if((mapperFlags[MAPPERFLAGS_COMMONHEAP]) && (pindex < (uint32_t)orignumproc) && (type == IS_HEAP))
        {
            *vaddr += pindex * (*vsize) / orignumproc;
            *paddr += pindex * (*vsize) / orignumproc;
            *vsize  = (*vsize) / orignumproc;
        }
    }
#if 0
    TRACE(TRACE_StaticMap, ("MAPPER%8p segment: pid=%d  type=%d  vaddr=%lx  paddr=%lx  size=%ld\n", this, pid, type, *vaddr, *paddr, *vsize));
#endif
    return 0;
}

int TLBMapper::getSegmentEntry(uint32_t pindex, uint64_t vaddr, uint64_t* paddr, size_t* vsize, uint64_t* tlbslot)
{
    unsigned int slot;
    uint32_t pid = proc[pindex].PID;
    
    vaddr /= CONFIG_CNK_PAGE_SIZE;
    *tlbslot = 1024;
    if(proc[pindex].mmu == NULL)
    {
       *paddr = 0;
       *vsize = 0;
       return -1;
    }
    for(slot=0; slot<NUMTLBSLOTS; slot++)
    {
        if(proc[pindex].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY) > slot%MMU_ASSOCIATIVITY)
        {   
            if(((proc[pindex].mmu->tlbarray[slot].pid == pid) || (proc[pindex].mmu->tlbarray[slot].pid==0)) &&
               (proc[pindex].mmu->tlbarray[slot].vaddr <= vaddr) && 
               (proc[pindex].mmu->tlbarray[slot].vaddr + proc[pindex].mmu->tlbarray[slot].origsize > vaddr))
            {
                *paddr = proc[pindex].mmu->tlbarray[slot].paddr * CONFIG_CNK_PAGE_SIZE;
                *vsize  = proc[pindex].mmu->tlbarray[slot].origsize * CONFIG_CNK_PAGE_SIZE;
                *tlbslot  = slot;
                return 0;
            }
        }
    }
    return -1;
}

int TLBMapper::getSegmentEntryBySlot(uint32_t pindex, uint64_t slot, enum SegmentType* type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize)
{
    if(proc[pindex].mmu == NULL)
    {
        *type  = IS_INVALID;
        *vaddr = 0;
        *paddr = 0;
        *vsize = 0;
        return -1;
    }
    
    if(slot >= NUMTLBSLOTS)
        return -1;
    
    if(proc[pindex].mmu->tlbstate.GetActive(slot/MMU_ASSOCIATIVITY) > slot%MMU_ASSOCIATIVITY)
    {   
        *type  = proc[pindex].mmu->tlbarray[slot].getType();
        *vaddr = (uint64_t)proc[pindex].mmu->tlbarray[slot].vaddr    * CONFIG_CNK_PAGE_SIZE;
        *paddr = (uint64_t)proc[pindex].mmu->tlbarray[slot].paddr    * CONFIG_CNK_PAGE_SIZE;
        *vsize = (uint64_t)proc[pindex].mmu->tlbarray[slot].origsize * CONFIG_CNK_PAGE_SIZE;
        return 0;
    }
    return -1;
}


/************************************************************/

TLBMapper* GetMapper()
{
    if(tlbmapper_precalculated != NULL)
        return tlbmapper_precalculated;
#if STANDALONE
    return core2mapinstance[0];
#else
    return core2mapinstance[ProcessorCoreID()];
#endif
}

#if defined __FUSEDOS__                                                                                               // FUSEDOS
int vmm_setupTLBMapper(int my_kernel_size)                                                                             // FUSEDOS
#else                                                                                                                  // FUSEDOS
int vmm_setupTLBMapper()
#endif                                                                                                                 // FUSEDOS
{
    int x;
    tlbmapper_precalculated = NULL;
    activemmu = &mmuinstance[0];
#if 0
    printf("Mapper offsets:\n");
#define OFFSETOF(x) printf("Offset %s = %d\n", #x, &tlbmapper_precalculated->x);
    OFFSETOF(MAGIC);
    OFFSETOF(NUMTLBSLOTS);
    OFFSETOF(copymap);
    OFFSETOF(proc);
    OFFSETOF(extraprocess);
    OFFSETOF(segs);
    OFFSETOF(precalculated);
#endif
    
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#if !STANDALONE
    if(((NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin)/CONFIG_CNK_PAGE_SIZE) > 2048)
        tlbmapper_precalculated = (TLBMapper*)CONFIG_STANDALONE_TLBMAPPER;
    else
        tlbmapper_precalculated = (TLBMapper*)CONFIG_STANDALONE_TLBMAPPER_L2;
    
    vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                          (void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated + NodeState.DomainDesc.ddrOrigin)),
                          (void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated)),
                          1024*1024,
                          0,
                          0);                                                                                 
    
    if(! ((tlbmapper_precalculated->MAGIC[0] == 'T') &&
          (tlbmapper_precalculated->MAGIC[1] == 'L') &&
          (tlbmapper_precalculated->MAGIC[2] == 'B') &&
          (tlbmapper_precalculated->MAGIC[3] == 'M')))
    {
        vmm_UnmapUserSpace((void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated)), 1024*1024, 0);
        tlbmapper_precalculated = (TLBMapper*)CONFIG_STANDALONE_TLBMAPPER_L2;
        
        vmm_MapUserSpace( APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_ESEL(3) | APP_FLAGS_NONSPECULATIVE,
                         (void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated + NodeState.DomainDesc.ddrOrigin)),
                         (void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated)),
                         1024*1024,
                         0,
                         0);                                                                                 
    }
    
    if((tlbmapper_precalculated->MAGIC[0] == 'T') &&
       (tlbmapper_precalculated->MAGIC[1] == 'L') &&
       (tlbmapper_precalculated->MAGIC[2] == 'B') &&
       (tlbmapper_precalculated->MAGIC[3] == 'M'))
    { 
        tlbmapper_precalculated->precalculated = true;
        activemmu = (MMU*)((uint64_t)tlbmapper_precalculated + sizeof(TLBMapper));
        assert(tlbmapper_precalculated->copymapCalculated);
        tlbmapper_precalculated->relocate = (uint64_t *) (((uint64_t) activemmu) + sizeof(mmuinstance));
        tlbmapper_precalculated->copymap   = NULL;
        tlbmapper_precalculated->eraselock = NULL;
        tlbmapper_precalculated->srcdemand = NULL;
        
#if COPYPRECALCULATEDMAP
        memcpy(&tlbmapper[0], tlbmapper_precalculated, sizeof(tlbmapper[0]));
        memcpy(&mmuinstance[0], activemmu, sizeof(mmuinstance));
        
        tlbmapper[0].relocate = ((TLBMapperSharedData *)&NodeState.sharedWorkArea)->relocate;
        memcpy(tlbmapper[0].relocate, tlbmapper_precalculated->relocate,
               tlbmapper_precalculated->numrelocations * sizeof(uint64_t));
        
        vmm_UnmapUserSpace((void*)(ROUND_DN_1M((uint64_t)tlbmapper_precalculated)), 1024*1024, 0);
        tlbmapper_precalculated = tlbmapper;
        activemmu = &mmuinstance[0];
#endif
        
        for(x=0; x<tlbmapper_precalculated->numproc; x++)
        {
            tlbmapper_precalculated->proc[x].mmu = &activemmu[tlbmapper_precalculated->proc[x].mmuindex];
        }        
        return 0;
    }
    else
#endif
#endif                                                                                                                 // FUSEDOS
    {
        tlbmapper_precalculated = NULL;
    }
    memset(tlbmapper, 0, sizeof(tlbmapper));
    for(x=0; x<MAX_TLBMAPPERS; x++)      /*loop doesn't iterate*/
    {
        tlbmapper[x].MAGIC[0]   = 'T';
        tlbmapper[x].MAGIC[1]   = 'L';
        tlbmapper[x].MAGIC[2]   = 'B';
        tlbmapper[x].MAGIC[3]   = 'M';
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        tlbmapper[x].kernelsize = my_kernel_size;                                                                      // FUSEDOS
#else                                                                                                                  // FUSEDOS
        tlbmapper[x].kernelsize = 16;
#endif                                                                                                                 // FUSEDOS
#if !STANDALONE
        assert(sizeof(TLBMapperSharedData) <= sizeof(NodeState.sharedWorkArea));
        tlbmapper[x].copymap = ((TLBMapperSharedData *)&NodeState.sharedWorkArea)->copymap;
        tlbmapper[x].relocate = ((TLBMapperSharedData *)&NodeState.sharedWorkArea)->relocate;
        tlbmapper[x].eraselock = ((TLBMapperSharedData *)&NodeState.sharedWorkArea)->eraselock;
        tlbmapper[x].srcdemand = ((TLBMapperSharedData *)&NodeState.sharedWorkArea)->srcdemand;
#else
        tlbmapper[x].copymap = standalone_TLBMapperSharedData.copymap;
        tlbmapper[x].relocate = standalone_TLBMapperSharedData.relocate;
        tlbmapper[x].eraselock = standalone_TLBMapperSharedData.eraselock;
        tlbmapper[x].srcdemand = standalone_TLBMapperSharedData.srcdemand;

#endif
    }
    return 0;
}

#if !STANDALONE

int vmm_copySegments()
{
    return GetMapper()->copySegments();
}

int vmm_installStaticTLBMap(int process)
{
    int pindex = GetMapper()->translate2pindex(process);
#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACE(TRACE_StaticMap, ("FUSEDOS vmm_installStaticTLBMap: process %d, pindex %d\n",                  // FUSEDOS
          process, pindex));                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    return GetMapper()->installStaticTLBMap(pindex);
}
int vmm_installStaticTLBMapPartial(int process, uint32_t pid)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->installStaticTLBMapPartial(pindex, pid);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int vmm_uninstallStaticTLBMap(int process)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->uninstallStaticTLBMap(pindex);
}

int vmm_copyProcessToOtherMMU(int sp, int dp, uint32_t* destpid)
{
    int spindex = GetMapper()->translate2pindex(sp);
    int dpindex = GetMapper()->translate2pindex(dp);
    return GetMapper()->copyProcessToOtherMMU(spindex, dpindex, destpid);
}


int vmm_calculateSlot(unsigned pid32, uint64_t va, uint64_t size)
{
    int tlbsize = 0;
    switch(size)
    {
        // comment out duplicate values to workaround compiler msg.  MAS1_TSIZE_16MB == MAS6_ISIZE_16MB
//        case MAS1_TSIZE_1GB:
        case MAS6_ISIZE_1GB:
            tlbsize = 0;
            break;
//        case MAS1_TSIZE_16MB:
        case MAS6_ISIZE_16MB:
            tlbsize = 1;
            break;
//        case MAS1_TSIZE_1MB:
        case MAS6_ISIZE_1MB:
            tlbsize = 2;
            break;
    }
    return TLBstate::CalculateSlot(pid32, va/CONFIG_CNK_PAGE_SIZE, tlbsize);
}
#endif                                                                                                                 // FUSEDOS

#endif

int vmm_getSegment(uint32_t process, enum SegmentType type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize)
{
    int rc;
    int pindex = GetMapper()->translate2pindex(process);
    rc = GetMapper()->getSegment(pindex, type, vaddr, paddr, vsize);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#if !STANDALONE
    if(((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_DD1_Workarounds) != 0) || (TI_isDD1()))
    {
#endif
    
#if ENABLE_L1P_PREFETCH_WORKAROUND
#if STANDALONE
    if(((*paddr + *vsize) / CONFIG_CNK_PAGE_SIZE >= GetMapper()->getTotalMem()) && (GetMapper()->getTotalMem() > 32))
#else
        if(((*paddr + *vsize)/CONFIG_CNK_PAGE_SIZE >= ((NodeState.DomainDesc.ddrEnd + 1)/CONFIG_CNK_PAGE_SIZE)) && (((NodeState.DomainDesc.ddrEnd + 1)/CONFIG_CNK_PAGE_SIZE) > 32))
#endif
    {
        *vsize -= 1024*1024;
    }
#endif
    
#if !STANDALONE
    }
#endif
#endif                                                                                                                 // FUSEDOS

    
    return rc;
}

int vmm_getSegmentEntry(uint32_t process, uint64_t vaddr, uint64_t* paddr, size_t* vsize, uint64_t* slot)
{
    int rc;
    int pindex = GetMapper()->translate2pindex(process);
    rc = GetMapper()->getSegmentEntry(pindex, vaddr, paddr, vsize, slot);
    return rc;
}

int vmm_getSegmentEntryBySlot(uint32_t process, uint64_t slot, enum SegmentType* type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize)
{
    int rc;
    int pindex = GetMapper()->translate2pindex(process);
    rc = GetMapper()->getSegmentEntryBySlot(pindex, slot, type, vaddr, paddr, vsize);
    return rc;
}

int vmm_getPID(int process, uint64_t* pid)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->getPID(pindex, pid);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int vmm_getSpecPID(int process, int specindex, uint64_t* pid)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->getSpecPID(pindex, specindex, pid);
}
#endif                                                                                                                 // FUSEDOS

int vmm_setFlags(enum MapperFlags flagid, uint64_t newvalue)
{
    assert(flagid < MAPPERFLAGS_COUNT);
    mapperFlags[flagid] = newvalue;
    return 0;
}

int vmm_resetStaticMap(int numprocesses, int numagents, int core_index)
{
#if EDRAM
    tlbmapper[0].totalmem = 6*(1024/64) - (256/64);
    tlbmapper[0].mapper_paddr_offset = 2*(1024/64) + (256/64);
    for(int x=0; x<MAX_CORE_COUNT; x++)
    {
        core2mapinstance[x] = &tlbmapper[0];
    }
#else
  #if STANDALONE
    tlbmapper[0].totalmem = STANDALONE_MEMSIZE;
    tlbmapper[0].mapper_paddr_offset = STANDALONE_MAPOFFSET;
    for(int x=0; x<MAX_CORE_COUNT; x++)
    {
        core2mapinstance[x] = &tlbmapper[0];
    }
  #else
    if (core_index >= 0)
    {
        // The job is scoped to just one core
        assert(core_index < CONFIG_MAX_CORES);
        core2mapinstance[core_index] = &tlbmapper[core_index];
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        tlbmapper[core_index].totalmem     = (NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin) /      // FUSEDOS
                                             CONFIG_CNK_PAGE_SIZE;                                                     // FUSEDOS
#else                                                                                                                  // FUSEDOS
        tlbmapper[core_index].totalmem     = ((NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin)/ CONFIG_MAX_APP_CORES) / CONFIG_CNK_PAGE_SIZE;
#endif                                                                                                                 // FUSEDOS
        tlbmapper[core_index].mapper_paddr_offset = tlbmapper[core_index].totalmem*core_index;
    }
    else
    {
        // The job is scoped to all cores on the node
        tlbmapper[0].totalmem            = (NodeState.DomainDesc.ddrEnd + 1 - NodeState.DomainDesc.ddrOrigin) / CONFIG_CNK_PAGE_SIZE;
        tlbmapper[0].mapper_paddr_offset = NodeState.DomainDesc.ddrOrigin / CONFIG_CNK_PAGE_SIZE;
        for(int x=0; x<MAX_CORE_COUNT; x++)
        {
            core2mapinstance[x] = &tlbmapper[0];
        }
    }
    TRACE(TRACE_StaticMap, ("vmm_resetStaticMap core_index %d, totalmem %u, mapper_paddr_offset %u\n", core_index, // FUSEDOS
             core_index >= 0 ? tlbmapper[core_index].totalmem : tlbmapper[0].totalmem, // FUSEDOS
             core_index >= 0 ? tlbmapper[core_index].mapper_paddr_offset : tlbmapper[0].mapper_paddr_offset)); // FUSEDOS
  #endif
#endif
    if(GetMapper()->isPreCalculated()) 
        return 0;
    return GetMapper()->resetStaticMap(numprocesses, numagents, core_index);
}

int vmm_addSegment(enum SegmentType type, unsigned vaddr, unsigned paddr_source, size_t size, int process, unsigned coremask, unsigned contiguous, unsigned roundable, unsigned pinned, unsigned loaded, unsigned sharedPID, unsigned allowHeap)
{
    if(GetMapper()->isPreCalculated()) 
        return 0;
    unsigned int newindex;
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->addSegment(type, vaddr, paddr_source, size, pindex, coremask, contiguous, roundable, pinned, loaded, sharedPID, allowHeap, false, newindex);
}

#if !STANDALONE
int vmm_allocateAtomic(int process, uint64_t vaddr, size_t* sizeMapped)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->allocateAtomic(pindex, vaddr, sizeMapped);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int vmm_UpdateMMU(int process, unsigned slot)
{
    int pindex = GetMapper()->translate2pindex(process);
    return GetMapper()->installStaticEntry(pindex, slot);
}
#endif
#endif                                                                                                                 // FUSEDOS

int vmm_partitionedMapper(_BGP_VMM_RasData* pRasData)
{
    if(GetMapper()->isPreCalculated()) 
        return 0;
    return GetMapper()->partitioned_mapper(pRasData);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#if STANDALONE
/********************************************************************************
 *
 * Testcase
 *
 ********************************************************************************/

void TLBMapper::dotest(int numprocessgroups, int processgroup)
{
    int loop = 0;
    int rc = 0;
    int p;
    unsigned textsize;
    unsigned datasize;
    unsigned shmsize;
    unsigned newindex;
    unsigned numprocesses;
    _BGP_VMM_RasData rasdata; 
    
#if EXHAUSTIVE
#if REVERSE
    for(textsize=TEXTLIMIT; textsize>=INITIAL_TEXTSIZE; textsize--)
    {
        for(datasize=DATALIMIT; datasize>=INITIAL_DATASIZE; datasize--)
        {
            for(shmsize=SHMLIMIT; shmsize>INITIAL_SHMSIZE; shmsize/=2, loop++)
            {
#else
    for(textsize=INITIAL_TEXTSIZE, loop=0; textsize<=TEXTLIMIT; textsize++)
    {
        for(datasize=INITIAL_DATASIZE; datasize<=DATALIMIT; datasize++)
        {
            for(shmsize=SHMLIMIT; shmsize>INITIAL_SHMSIZE; shmsize/=2, loop++)
            {
#endif
                for(numprocesses=1; numprocesses<=NUMPROCESSES; numprocesses*=2, loop++)
                {
                    if(MAX_ITERATIONS == loop)
                        exit(0);
#else
                for(loop=0; loop < MAX_ITERATIONS; loop++)
                {
#endif                
                printf("MAPPER%8p Iteration %d\n", this, loop);
                
                struct timeval tvstart, tvend;
                gettimeofday(&tvstart, NULL);
                
                totalmem     = STANDALONE_MEMSIZE/numprocessgroups;
                
                if(processgroup == 0)
                {
#if EDRAM
                    kernelsize = 0;
#else
                    kernelsize = 16;   
#endif
                }
                else
                    kernelsize = 0;
                
                mapper_paddr_offset = totalmem*processgroup;
                
                resetStaticMap(numprocesses, 1, -1);
                
#if (EXHAUSTIVE==0)
                textsize = rand()%TEXTLIMIT + 1;
                datasize = rand()%DATALIMIT + 1;
                shmsize  = rand()%SHMLIMIT;
#endif
                
                printf("MAPPER%8p \t \t numprocesses: %d  textsize: %d  datasize: %d  shmsize:%d\n", this, numprocesses, textsize, datasize, shmsize/2);
                addSegment(IS_TEXT, kernelsize, kernelsize, textsize, -1, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_TEXT, false, true, true, true, false, 0, newindex);
                addSegment(IS_DATA, segs[0].vaddr+segs[0].size, segs[0].vaddr+segs[0].size, datasize, 0, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_DATA, true, true, true, false, true, 0, newindex);
                for(p=1; p<numproc; p++)
                {
                    addSegment(IS_DATA, segs[0].vaddr+segs[0].size, segs[0].vaddr+segs[0].size, segs[1].size, p, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_DATA, true, true, true, false, true, 0, newindex);
                }

                int x;
                size_t agenttextsize[4]   = {1,0,0,0};
                size_t agentdatasize[4]   = {1,0,0,0};
                size_t agentheapsize[4]   = {16-agenttextsize[0]-agentdatasize[0],0,0,0};
                unsigned long agentbaseaddr[4] = {0x41000000/CONFIG_CNK_PAGE_SIZE,0,0,0};
                for(x=0; x<1; x++)
                {
                    addSegment(IS_TEXT, agentbaseaddr[x], agentbaseaddr[x], agenttextsize[x], numproc + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_TEXT, false, true, true, true, false, 0, newindex);
                    addSegment(IS_DATA, agenttextsize[x] + agentbaseaddr[x], agenttextsize[x] + agentbaseaddr[x], agentdatasize[x], numproc + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_DATA, false, true, true, false, false, 0, newindex);
                    addSegment(IS_HEAP, 0, 0, agentheapsize[x], numproc + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_DATA, true, false, false, false, false, 0, newindex);
                }
                addSegment(IS_SHAR, 777, ((numproc==1)?32:64) + shmsize/2, ((numproc==1)?32:64) + shmsize/2, -1, VMM_SEGMENTCOREMASK_ALL, CONTIGUOUS_SHM, true, false, false, ((numproc==64)?true:false), false, 0, newindex);
                
                rc = partitioned_mapper(&rasdata);
                gettimeofday(&tvend, NULL);
                if(rc)
                {
                    printf("MAPPER%8p partitioned_mapper rc = %d FAILED  textsize=%d  datasize=%d  shmsize=%d\n", this, rc, textsize, datasize, shmsize);
                }
                else
                {
                    printf("MAPPER%8p partitioned_mapper rc = %d  (time=%ld ms)\n", this, rc, (tvend.tv_sec*1000 + tvend.tv_usec/1000) - (tvstart.tv_sec*1000 + tvstart.tv_usec/1000));
                }
                
#if EXIT_ON_FAILURE
                if(rc)
                    exit(rc);
#endif
            }
#if EXHAUSTIVE
        }
    }    
    }
#endif
}


/*! \brief Runs a bunch of random text/data/shared memory sizes through the mapper, fails if mapping would not be successful.
 *  To build standalone:  make staticmapper
 */
class threaddata
    {
    public:
        unsigned int numprocessgroups;
        unsigned int processgroup;
        TLBMapper* mapper;
    };

void* threadedmapper(void* ptr)
{
    threaddata* data = (threaddata*)ptr;
    printf("starting\n");
    
    data->mapper->dotest(data->numprocessgroups, data->processgroup);
    return NULL;
} 

void regression()
{
    int x;
    threaddata data[NUMPROCESSGROUPS];
    pthread_t tid[NUMPROCESSGROUPS];
    void* rc;
    for(x=0; x<NUMPROCESSGROUPS; x++)
    {
        data[x].numprocessgroups = NUMPROCESSGROUPS;
        data[x].processgroup = x;
        data[x].mapper = &tlbmapper[x];
        pthread_create(&tid[x], NULL, threadedmapper, (void*)&data[x]);
    }
    for(x=0; x<NUMPROCESSGROUPS; x++)
    {
        pthread_join(tid[x], &rc);
    }
    
}
                
int main(int argc, char** argv)
{
//    printf("tlbmapper=%d\n", sizeof(TLBMapper));
//    printf("process  =%d\n", sizeof(Process));
//    printf("segment  =%d\n", sizeof(Segment));
//    exit(0);

    int x;
    unsigned long i;
    unsigned long regress      = 0;
    unsigned long generate     = 0;
    size_t textsize   = CONFIG_CNK_PAGE_SIZE;
    size_t datasize   = CONFIG_CNK_PAGE_SIZE;
    size_t agenttextsize[4]   = {CONFIG_CNK_PAGE_SIZE,0,0,0};
    size_t agentdatasize[4]   = {CONFIG_CNK_PAGE_SIZE,0,0,0};
    size_t agentheapsize[4]   = {16*CONFIG_CNK_PAGE_SIZE - agenttextsize[0] - agentdatasize[0],0,0,0};
    unsigned long agentbaseaddr[4] = {0x41000000,0,0,0};
    size_t shmsize    = 32*1024*1024;
    unsigned long numproc       = 1;
    unsigned long numagentproc  = 1;
    bool showhelp     = false;
    char genstring[256];
    bool sharedMBSet = false;
    bool loopForProfiler = false;
    bool dynamicLib = false;
    
    char* str = getenv("BG_DDRINIT");
    if(str)
    {
        printf("Opening erase file: %s\n", str);
        EraseFile = fopen(str, "w");
    }
    memset(tlbmapper, 0, sizeof(tlbmapper));
    for(x=1; x<argc; x++)
    {
#define KEYWORD(name) (strncmp(argv[x], name, strlen(name)) == 0)
        printf("argv[%d] = %s\n", x, argv[x]);
        if(KEYWORD("-regress"))
        {
            regress = 1;
        }
        else if(KEYWORD("-generate="))
        {
            generate = 1;
            sscanf(argv[x], "-generate=%s", genstring);
        }
        else if(KEYWORD("-text="))       sscanf(argv[x], "-text=%ld",     &textsize);
        else if(KEYWORD("-data="))       sscanf(argv[x], "-data=%ld",     &datasize);
        else if(KEYWORD("-shared="))     { sscanf(argv[x], "-shared=%ld",   &shmsize); sharedMBSet=true; }
        else if(KEYWORD("-agent0text="))  sscanf(argv[x], "-agent0text=%ld",   &agenttextsize[0]); 
        else if(KEYWORD("-agent0data="))  sscanf(argv[x], "-agent0data=%ld",   &agentdatasize[0]);
        else if(KEYWORD("-agent0heap="))  sscanf(argv[x], "-agent0heap=%ld",   &agentheapsize[0]);
        else if(KEYWORD("-agent0base="))  sscanf(argv[x], "-agent0base=%lx",   &agentbaseaddr[0]);

        else if(KEYWORD("-agent1text="))  sscanf(argv[x], "-agent1text=%ld",   &agenttextsize[1]); 
        else if(KEYWORD("-agent1data="))  sscanf(argv[x], "-agent1data=%ld",   &agentdatasize[1]);
        else if(KEYWORD("-agent1heap="))  sscanf(argv[x], "-agent1heap=%ld",   &agentheapsize[1]);
        else if(KEYWORD("-agent1base="))  sscanf(argv[x], "-agent1base=%lx",   &agentbaseaddr[1]);
        
        else if(KEYWORD("-agent2text="))  sscanf(argv[x], "-agent2text=%ld",   &agenttextsize[2]); 
        else if(KEYWORD("-agent2data="))  sscanf(argv[x], "-agent2data=%ld",   &agentdatasize[2]);
        else if(KEYWORD("-agent2heap="))  sscanf(argv[x], "-agent2heap=%ld",   &agentheapsize[2]);
        else if(KEYWORD("-agent2base="))  sscanf(argv[x], "-agent2base=%lx",   &agentbaseaddr[2]);

        
        else if(KEYWORD("-agent3text="))  sscanf(argv[x], "-agent3text=%ld",   &agenttextsize[3]); 
        else if(KEYWORD("-agent3data="))  sscanf(argv[x], "-agent3data=%ld",   &agentdatasize[3]);
        else if(KEYWORD("-agent3heap="))  sscanf(argv[x], "-agent3heap=%ld",   &agentheapsize[3]);
        else if(KEYWORD("-agent3base="))  sscanf(argv[x], "-agent3base=%lx",   &agentbaseaddr[3]);
        
        else if(KEYWORD("-memsize="))    sscanf(argv[x], "-memsize=%ld",  &STANDALONE_MEMSIZE);
        else if(KEYWORD("-poffset="))    sscanf(argv[x], "-poffset=%ld",  &STANDALONE_MAPOFFSET);
        
        else if(KEYWORD("-dyn"))        { dynamicLib = 1; }
        
        else if(KEYWORD("-numprocesses="))
        {
            sscanf(argv[x], "-numprocesses=%ld", &numproc);
        }
        
        else if(KEYWORD("-textMB="))   { sscanf(argv[x], "-textMB=%ld",   &textsize); textsize *= 1024*1024; }
        else if(KEYWORD("-dataMB="))   { sscanf(argv[x], "-dataMB=%ld",   &datasize); datasize *= 1024*1024; }
        else if(KEYWORD("-sharedMB=")) { sscanf(argv[x], "-sharedMB=%ld", &shmsize);  shmsize  *= 1024*1024; sharedMBSet=true; }
        else if(KEYWORD("-textmb="))   { sscanf(argv[x], "-textmb=%ld",   &textsize); textsize *= 1024*1024; }
        else if(KEYWORD("-datamb="))   { sscanf(argv[x], "-datamb=%ld",   &datasize); datasize *= 1024*1024; }
        else if(KEYWORD("-sharedmb=")) { sscanf(argv[x], "-sharedmb=%ld", &shmsize);  shmsize  *= 1024*1024; sharedMBSet=true; }
        
        else if(KEYWORD("-kernelsize=")) { sscanf(argv[x], "-kernelsize=%ld", &SA_KERNEL_END); SA_KERNEL_END = ROUND_UP_1M(SA_KERNEL_END); }
        
        else if(KEYWORD("-loop")) { loopForProfiler = true; }
        else if(KEYWORD("-mapalign16")) { vmm_setFlags(MAPPERFLAGS_ALIGN16, 1); }
        else if(KEYWORD("-commonheap")) { vmm_setFlags(MAPPERFLAGS_COMMONHEAP, 1); }
        else if(KEYWORD("-noaliases")) { vmm_setFlags(MAPPERFLAGS_NOALIASES, 1); }
        
        else if(KEYWORD("-h"))
        {
            showhelp = true;
        }
        else
        {
            printf("Invalid parameter: %s\n", argv[x]);
            showhelp = true;
        }
        if(showhelp)
        {
            printf("Usage: %s <-regress> -text=<size_in_bytes> -data=<size_in_bytes> -shared=<size_in_bytes> -memsize=<size_in_megabytes> -numprocesses=<n> -generate=<filename>\n", argv[0]);
            printf("\n");
            
#define DEFAULTS(n,v)  printf("\t%s=%ld\n", n, v);
            printf("Defaults:\n");
                DEFAULTS("-regress",        regress);
                DEFAULTS("-text",           textsize);
                DEFAULTS("-data",           datasize);
                DEFAULTS("-shared",         shmsize);
                DEFAULTS("-memsize",        STANDALONE_MEMSIZE);
                DEFAULTS("-numprocesses",   numproc);

            exit(0);
        }
    }
    if(!sharedMBSet && (numproc > 1))
    {
        shmsize = 64*1024*1024;
    }
    
    if(regress)
    {
        vmm_setupTLBMapper();   
        regression();
    }
    else
    {
        int rc;
        _BGP_VMM_RasData rasdata; 
        size_t kernelsize = 16;
        textsize = (textsize+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;
        datasize = (datasize+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;
        shmsize  = (shmsize+CONFIG_CNK_PAGE_SIZE-1)  / CONFIG_CNK_PAGE_SIZE;
        
        numagentproc = 0;
        for(x=0; x<4; x++)
        {
            agenttextsize[x] = (agenttextsize[x]+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;
            agentdatasize[x] = (agentdatasize[x]+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;
            agentheapsize[x] = (agentheapsize[x]+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;
            agentbaseaddr[x] = (agentbaseaddr[x]+CONFIG_CNK_PAGE_SIZE-1) / CONFIG_CNK_PAGE_SIZE;

            if(agenttextsize[x] | agentdatasize[x])
            {
                numagentproc++;
            }
        }
        vmm_setupTLBMapper();
        
        printf("BGQ Staticmapper textsize: %ld  datasize: %ld  shmsize:%ld\n", textsize, datasize, shmsize);
        
        do {
            vmm_resetStaticMap(numproc, numagentproc, -1);
            
            
            if(textsize)
            {
                vmm_addSegment(IS_TEXT, kernelsize, kernelsize, textsize, -1, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_TEXT, false, true, true, true, false);
            }
            else
            {
                printf("Text size must be greater than zero\n");
                return -1;
            }
            if(datasize)
            {
                for(i=0; i<numproc; i++)
                {
                    vmm_addSegment(IS_DATA, kernelsize + textsize, kernelsize + textsize, datasize, i, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_DATA, true, true, true, false, true);
                    if(dynamicLib)
                    {
                        vmm_addSegment(IS_DYNAM, 0, kernelsize+textsize+datasize, 2, i, VMM_SEGMENTCOREMASK_APP, CONTIGUOUS_DATA, true, false, true, false, false);
                    }
                }
            }
            
            for(x=0; x<4; x++)
            {
                if(agenttextsize[x])
                {
                    vmm_addSegment(IS_TEXT, agentbaseaddr[x], agentbaseaddr[x], agenttextsize[x], 64 + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_TEXT, false, true, true, true, false);
                }
                if(agentdatasize[x])
                {
                    vmm_addSegment(IS_DATA, agenttextsize[x] + agentbaseaddr[x], agenttextsize[x] + agentbaseaddr[x], agentdatasize[x], 64 + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_DATA, false, true, true, false, false);
                }
                if(agentheapsize[x])
                {
                    vmm_addSegment(IS_HEAP, 0, 0, agentheapsize[x], 64 + x, VMM_SEGMENTCOREMASK_16, CONTIGUOUS_DATA, true, false, false, false, false);
                }
            }
            
            if(shmsize)
            {
                vmm_addSegment(IS_SHAR, 777, shmsize, shmsize, -1, VMM_SEGMENTCOREMASK_ALL, CONTIGUOUS_SHM, true, false, false, ((numproc==64)?true:false), false);
            }
	    
	    struct timeval start, end;
	    gettimeofday(&start, NULL);
            rc = vmm_partitionedMapper(&rasdata);
	    gettimeofday(&end, NULL);
	    printf("time delta = %g\n", (end.tv_sec + end.tv_usec/1000000.0) - (start.tv_sec + start.tv_usec/1000000.0));
	    
            if(rc)
            {
                printf("Unable to generate a mapping\n");
                exit(-1);
            }
            
            if(generate)
            {
                FILE* fp;
                uint64_t *reloc;
                uint32_t numreloc;
                
                tlbmapper[0].CalculateCopymap();
                tlbmapper[0].GetCopymap(&reloc, &numreloc);
                tlbmapper[0].ToNetworkEndian();
                for(x=0; x<MAX_CORE_COUNT; x++)
                {
                    mmuinstance[x].ToNetworkEndian();
                }
                fp = fopen(genstring, "w");
                fwrite(&tlbmapper[0], 1, sizeof(tlbmapper[0]), fp);
                fwrite(&mmuinstance[0], 1, sizeof(mmuinstance), fp);
                fwrite(reloc, numreloc, sizeof(uint64_t), fp);
                fclose(fp);
            }
        } while (loopForProfiler);

    }
    
    return 0;
}
#endif
#endif                                                                                                                 // FUSEDOS
