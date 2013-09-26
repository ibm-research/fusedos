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
#include <cnk/include/Config.h>

#define PROVIDE(symbol) extern uint64_t symbol
PROVIDE( __KERNEL_TEXT_START);
PROVIDE( __KERNEL_TEXT_END);
PROVIDE( __KERNEL_DATA_START);
PROVIDE( __KERNEL_ATOMICS_START);
PROVIDE( __KERNEL_ATOMICS_END);
PROVIDE( __CNK_TOC_BASE);
PROVIDE( __KERNEL_DATA_END);
PROVIDE( __KERNEL_BSS_START);
PROVIDE( __KERNEL_BSS_END);
PROVIDE( __KERNEL_END);
#undef PROVIDE

// Bit Masks for Effective Page Number by Page Size
#define VMM_MASK_1K     (0xFFFFFFFFFFFFFC00) // save epn[0..21]
#define VMM_MASK_4K     (0xFFFFFFFFFFFFF000) // save epn[0..19]
#define VMM_MASK_16K    (0xFFFFFFFFFFFFC000) // save epn[0..17]
#define VMM_MASK_64K    (0xFFFFFFFFFFFF0000) // save epn[0..15]
#define VMM_MASK_256K   (0xFFFFFFFFFFFC0000) // save epn[0..13]
#define VMM_MASK_1M     (0xFFFFFFFFFFF00000) // save epn[0..11]
#define VMM_MASK_4M     (0xFFFFFFFFFFC00000) // save epn[0.. 9] (only ppc405)
#define VMM_MASK_16M    (0xFFFFFFFFFF000000) // save epn[0.. 7]
#define VMM_MASK_64M    (0xFFFFFFFFFC000000) // save epn[0.. 5] (neither ppc405 nor ppc440, only slab)
#define VMM_MASK_256M   (0xFFFFFFFFF0000000) // save epn[0.. 3] (ppc440, ppc450)
#define VMM_MASK_1G     (0xFFFFFFFFC0000000) // save epn[0.. 1] (only ppc450)


#define ROUND_UP_1G(p) (((p) + ~VMM_MASK_1G) & VMM_MASK_1G)
#define ROUND_UP_16M(p) (((p) + ~VMM_MASK_16M) & VMM_MASK_16M)
#define ROUND_UP_1M(p)  (((p) + ~VMM_MASK_1M)  & VMM_MASK_1M)
#define ROUND_UP_256K(p)(((p) + ~VMM_MASK_256K)  & VMM_MASK_256K)
#define ROUND_UP_64K(p) (((p) + ~VMM_MASK_64K) & VMM_MASK_64K)
#define ROUND_UP_16K(p) (((p) + ~VMM_MASK_16K) & VMM_MASK_16K)
#define ROUND_UP_4K(p)  (((p) + ~VMM_MASK_4K)  & VMM_MASK_4K)
#define ROUND_UP_1K(p)  (((p) + ~VMM_MASK_1K)  & VMM_MASK_1K)
#define ROUND_UP_L1_CACHELINE(p) (((p) + 0x1F)  & ~0x1F)
#define ROUND_UP_MEMPAGE(p)      (((p) + CONFIG_MEM_PAGE_SIZE - 1) & (~(CONFIG_MEM_PAGE_SIZE-1)))

#define ROUND_DN_1G(p) ((p) & VMM_MASK_1G)
#define ROUND_DN_16M(p) ((p) & VMM_MASK_16M)
#define ROUND_DN_1M(p)  ((p) & VMM_MASK_1M)
#define ROUND_DN_256K(p) ((p) & VMM_MASK_256K)
#define ROUND_DN_64K(p) ((p) & VMM_MASK_64K)
#define ROUND_DN_16K(p) ((p) & VMM_MASK_16K)
#define ROUND_DN_4K(p)  ((p) & VMM_MASK_4K)
#define ROUND_DN_1K(p)  ((p) & VMM_MASK_1K)
#define ROUND_DN_L1_CACHELINE(p) ((p) & ~0x1F)
#define ROUND_DN_MEMPAGE(p)      ((p) & (~(CONFIG_MEM_PAGE_SIZE-1)))


#define VMM_PAGE_SIZE_1M  (1024*1024)
#define VMM_PAGE_SIZE_16M (16*1024*1024)
#define VMM_PAGE_SIZE_1G  (1024*1024*1024)

enum SegmentType
{
    IS_INVALID = 0,
    IS_TEXT,
    IS_DATA,
    IS_HEAP,
    IS_SHAR,
    IS_PROCESSWINDOW,
    IS_DYNAM,
    IS_ATOMICRGN,
    IS_KERNEL,
    IS_MMIO,
    IS_SPEC,
    IS_RAMDISK,
    IS_SCRUBWINDOW,
    IS_USERMMIO,
    IS_COMMONHEAP,
    IS_SEGMENTTYPECOUNT // Add new types before this one
};

enum MapperFlags
{
    MAPPERFLAGS_ALIGN16,
    MAPPERFLAGS_NOALIASES,
    MAPPERFLAGS_COMMONHEAP,
    MAPPERFLAGS_COUNT
};

#define VMM_SEGMENTCOREMASK_APP  0x0ffff
#define VMM_SEGMENTCOREMASK_ALL  0x1ffff
#define VMM_SEGMENTCOREMASK_0    0x00001
#define VMM_SEGMENTCOREMASK_16   0x10000

#define BEGIN_KERNELINTERNALSC() { GetMyKThread()->KernelInternal = 1;
#define END_KERNELINTERNALSC()     GetMyKThread()->KernelInternal = 0; }

#define VMM_APP_LOAD_VADDR (0x4000000000000000LL)

typedef struct T_BGP_VMM_RasData
{
    uint32_t tlb_slots_used[CONFIG_MAX_CORES];
    uint32_t textseg_start;
    uint32_t textseg_size;
    uint32_t dataseg_start;
    uint32_t dataseg_size;
    uint32_t shared_size;
    uint32_t persist_size;
} _BGP_VMM_RasData;

void VMM_Init();
void vmm_tlbwe_slot(uint64_t slot, uint64_t mas1, uint64_t mas2, uint64_t mas7_3, uint64_t mas8, uint64_t mmucr3);
void vmm_tlbilx(void* va, uint64_t size);

#if defined __FUSEDOS__                                                                                               // FUSEDOS
int vmm_setupTLBMapper(int my_kernel_size);                                                                            // FUSEDOS
#else                                                                                                                  // FUSEDOS
int vmm_setupTLBMapper();
#endif                                                                                                                 // FUSEDOS
int vmm_partitionedMapper(_BGP_VMM_RasData *);
int vmm_copySegments();
int vmm_calculateSlot(uint32_t pid, uint64_t va, uint64_t size);
int vmm_installStaticTLBMap(int process);
int vmm_installStaticTLBMapPartial(int process, uint32_t pid);
int vmm_uninstallStaticTLBMap(int process);
int vmm_copyProcessToOtherMMU(int source_process, int dest_process, uint32_t* destpid);
int vmm_getSegment(uint32_t process, enum SegmentType type, uint64_t* vaddr, uint64_t* paddr, size_t* size);
int vmm_getSegmentEntry(uint32_t process, uint64_t vaddr, uint64_t* paddr, size_t* vsize, uint64_t* slot);
int vmm_getSegmentEntryBySlot(uint32_t process, uint64_t slot, enum SegmentType* type, uint64_t* vaddr, uint64_t* paddr, size_t* vsize);
int vmm_findSegment(int process, unsigned int req_vaddr, unsigned int* vaddr, unsigned long long* paddr, unsigned int* vsize);
int vmm_addSegment(enum SegmentType type, unsigned vaddr, unsigned paddr_source, size_t size, int process, unsigned coremask, 
                   unsigned contiguous, unsigned roundable, unsigned pinned, unsigned loaded, unsigned sharedPID, unsigned allowHeap);
int vmm_resetStaticMap(int numprocesses, int numagents, int core_index);
int vmm_MapUserSpace(uint64_t flags, void* physicaladdress, void* virtualaddress, size_t tlbsize, unsigned pid, size_t exclude);
int vmm_UnmapUserSpace(void* virtualaddress, size_t size, int pid);
int vmm_getPID(int pindex, uint64_t* pid);
int vmm_getSpecPID(int pindex, int specindex, uint64_t* pid);
int VMM_IsAppAddress(const void* address, size_t size);
int VMM_IsAppProcessUniqueAddress(const void* va, size_t size);
int vmm_TranslateAddress(const void* va, uint64_t* base_va, uint64_t* base_pa, size_t* base_sz);
int vmm_allocateAtomic(int process, uint64_t vaddr, size_t* sizeMapped);
int vmm_UpdateMMU(int process, unsigned slot);
void vmm_tlb_inject_parity(uint64_t setway);
void vmm_tlb_touch_tlbs();
int vmm_SetupBackgroundScrub();
int vmm_setScrubSlot();
int vmm_clearScrubSlot();
int VMM_IsAppAddressForProcess(const void* address, size_t size, struct AppProcess_t *proc);
int vmm_setFlags(enum MapperFlags flagid, uint64_t newvalue);
