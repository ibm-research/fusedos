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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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

#ifndef	_KERNEL_CNK_MEMORY_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_MEMORY_IMPL_H_

#include <firmware/include/VirtualMap.h>
#include <hwi/include/bqc/l1p_mmio.h>
#include <hwi/include/bqc/A2_inlines.h>
#include "cnk/include/SPI_syscalls.h"
#include "cnk/include/Config.h"
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>


/*!
 * \brief CNK Memory Region Cache Element
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 */
typedef struct CNKSPI_MemoryRegionCacheElement
{
  void     *startPtr;  /**< Region starting virtual address     */
  void     *endPtr;    /**< Region ending   virtual address     */
  void     *PA;        /**< Region starting physical address    */
  uint64_t  bytes;     /**< Size of the memory region, in bytes */

} CNKSPI_MemoryRegionCacheElement_t;


/*!
 * \brief CNK Memory Region Cache Number of Elements
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 */
#define CNKSPI_MAX_NUM_CACHED_MEMORY_REGIONS 5


/*!
 * \brief Declaration of the CNK Memory Region Cache.
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 * The memory cache has a fixed number of slots, one for each CNK
 * memory region (text, data, heap, shared memory, extra).
 *
 * Declared in spi/src/kernel/cnk/memory_impl.c, libSPI_cnk.a
 */
extern CNKSPI_MemoryRegionCacheElement_t   _cnkspi_MemoryRegionCache[];


/*!
 * \brief Number of Elements in the CNK Memory Region Cache
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 *
 * Declared in spi/src/kernel/cnk/memory_impl.c, libSPI_cnk.a
 */
extern uint64_t _cnkspi_MemoryRegionCacheNumElements;


/*!
 * \brief Last Accessed Cache Element Number
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 *
 * Declared in spi/src/kernel/cnk/memory_impl.c, libSPI_cnk.a
 */
extern uint64_t _cnkspi_MemoryRegionCacheLastAccessedElementNumber;


/*!
 * \brief Lock variable for the CNK Memory Region Cache
 *
 * This is just a variable that is zero when the CNK memory region cache is
 * unlocked, and non-zero when it is locked.  Fetch_and_Add() is used to 
 * lock it.  Whoever sees zero has the lock.  Zero it out to unlock it, 
 * when holding the lock.
 *
 * Used by Kernel_CreateMemoryRegion() for caching CNK memory regions.
 *
 * Declared in spi/src/kernel/cnk/memory_impl.c, libSPI_cnk.a
 */
extern uint64_t _cnkspi_MemoryRegionCacheLock;


/*!
 * \brief Create A Memory Region
 *
 * Create a memory region for a specified buffer.  The implementation may
 * do one or more of the following:
 *
 * - Determine the number of physically contiguous bytes starting at the
 *   beginning of the buffer, and create the memory region only for that
 *   portion of the buffer.
 * - Pin in storage the portion of the buffer associated with this memory
 *   region.
 * - Store information within the returned memory region.
 *
 * \param[in,out]  memregion  Pointer to the memory region to be created.
 *                            Upon success, this memoryRegion has been
 *                            initialized with information about the buffer.
 * \param[in]      bytes_in   Length of the buffer (in bytes).  This is the
 *                            number of bytes requested to be in the memory
 *                            region.
 * \param[out]     bytes_out  Pointer to a length field to be updated by
 *                            this function.  It will contain the number
 *                            of bytes that this memory region actually 
 *                            covers.  It will be the number of bytes
 *                            actually pinned (if something was pinned)
 *                            and/or the number of physically contiguous
 *                            bytes of storage starting from the beginning
 *                            of the buffer.  bytes_out will be less than or
 *                            equal to bytes_in.
 * \param[in]      base       Virtual address of the buffer.
 *
 * \warning A memory region may start before the requested virtual address.
 * \warning A memory region's total size could be greater or smaller than the requested size.
 *
 * \warning After creating the memory region it is illegal to write into or
 *          deallocate the opaque memory region object.  Additionally, it is
 *          illegal to free or otherwise deallocate the memory buffer managed
 *          by the memory region.
 *
 * \see Kernel_DestoryMemoryRegion
 * \see Kernel_QueryMemoryRegion
 *
 * \retval  0  The entire memory region, or a portion of the memory
 *             region was pinned. The actual number of bytes pinned

 *             from the start of the buffer is returned in the
 *             \c bytes_out field. The memory region must be destroyed
 *             with Kernel_MemoryRegionDestroy().
 *
 * \retval -1  A general error occurred.  The memory region was not pinned
 *             and does not need to be destroyed with
 *             Kernel_MemoryRegionDestroy().
 *
 */
__INLINE__
uint32_t Kernel_CreateMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                   void*                  base_virtualaddress,
                                   size_t                 requested_size)
{
  uint64_t i,j;
  uint32_t rc;

  /* Calculate ending virtual address of user's buffer */
  void * end_virtualaddress = (void*)((uint64_t)base_virtualaddress + requested_size - 1);

  /* Point to cache */
  CNKSPI_MemoryRegionCacheElement_t *cachePtr = _cnkspi_MemoryRegionCache;
  j = _cnkspi_MemoryRegionCacheLastAccessedElementNumber;

  /* Look in the cache.
   * This could be in a loop, but it is faster to use an unrolled loop.
   */

  /* Look in the last accessed element of the cache */
  if ( ( base_virtualaddress >= cachePtr[j].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[j].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[j].bytes;
      memregion->BaseVa = cachePtr[j].startPtr;
      memregion->BasePa = cachePtr[j].PA;
      return 0;
    }

  /* Look in element 0 of the cache */
  if ( ( base_virtualaddress >= cachePtr[0].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[0].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[0].bytes;
      memregion->BaseVa = cachePtr[0].startPtr;
      memregion->BasePa = cachePtr[0].PA;
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 0;
      return 0;
    }

  /* Look in element 1 of the cache */
  if ( ( base_virtualaddress >= cachePtr[1].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[1].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[1].bytes;
      memregion->BaseVa = cachePtr[1].startPtr;
      memregion->BasePa = cachePtr[1].PA;
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 1;
      return 0;
    }

  /* Look in element 2 of the cache */
  if ( ( base_virtualaddress >= cachePtr[2].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[2].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[2].bytes;
      memregion->BaseVa = cachePtr[2].startPtr;
      memregion->BasePa = cachePtr[2].PA;
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 2;
      return 0;
    }

  /* Look in element 3 of the cache */
  if ( ( base_virtualaddress >= cachePtr[3].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[3].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[3].bytes;
      memregion->BaseVa = cachePtr[3].startPtr;
      memregion->BasePa = cachePtr[3].PA;
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 3;
      return 0;
    }

  /* Look in element 4 of the cache */
  if ( ( base_virtualaddress >= cachePtr[4].startPtr ) &&
       ( end_virtualaddress  <= cachePtr[4].endPtr   ) )
    {
      memregion->Bytes  = cachePtr[4].bytes;
      memregion->BaseVa = cachePtr[4].startPtr;
      memregion->BasePa = cachePtr[4].PA;
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 4;
      return 0;
    }

  /* It is not in the cache.  Perform the syscall to get the region. */
  rc = (uint32_t) CNK_SPI_SYSCALL_3(GETMEMORYREGION, memregion,
						      base_virtualaddress,
						      requested_size);
  if ( rc == 0)
    {
      /* Put the obtained region into the cache, if it is not full.
       * 1. Lock, using fetch and inc.
       * 2. Find an empty slot.
       * 3. Update the slot.
       * 4. Unlock.
       */
      /* 1. Lock */
      while ( Fetch_and_Add ( &_cnkspi_MemoryRegionCacheLock, 1ULL ) != 0 );
      
      i = _cnkspi_MemoryRegionCacheNumElements; /* Index into cache of next slot */
      
      /* 2. Find an empty slot. */
      /* 3. Update the slot.    */
      if ( i < CNKSPI_MAX_NUM_CACHED_MEMORY_REGIONS )
	{
	  _cnkspi_MemoryRegionCache[i].startPtr = memregion->BaseVa;
	  _cnkspi_MemoryRegionCache[i].endPtr   = (void*)((uint64_t)memregion->BaseVa + memregion->Bytes - 1);
	  _cnkspi_MemoryRegionCache[i].PA       = memregion->BasePa;
	  _cnkspi_MemoryRegionCache[i].bytes    = memregion->Bytes;
	  _cnkspi_MemoryRegionCacheNumElements++;
	  _cnkspi_MemoryRegionCacheLastAccessedElementNumber = i;
	}
      
      /* 4. Unlock */
      _cnkspi_MemoryRegionCacheLock = 0;
    }

  return rc;
  
}


/*!
 * \brief Destroy a memory region
 *
 * After a memory region is destroyed it is again legal to write into or
 * deallocate the memory used by the memory region object.
 *
 * \warning It is illegal to query the contents of the memory region with the
 *          Kernel_QueryMemoryRegion() function after destroying the memory region.
 *
 * \param[in]  memregion  Pointer to the memory region to be destroyed.
 *
 * \see Kernel_MemoryRegionCreate
 * \see Kernel_MemoryRegionQuery
 */

__INLINE__
uint32_t Kernel_DestroyMemoryRegion(Kernel_MemoryRegion_t* memregion)
{
    // No reason to deallocate memory regions on CNK
    return 0;
}


/*!
 * \brief Query a memory region
 *
 * \warning This function may fail in an unpredicatable way if an uninitialized
 *          memory region object is queried.
 *
 * \param[in]   memregion  Pointer to the memory region to be queried
 * \param[out]  bytes      Number of bytes in the memory region
 * \param[out]  base       Base virtual address of the memory region
 *
 * \see Kernel_MemoryRegionCreate
 * \see Kernel_MemoryRegionDestroy
 */

__INLINE__
uint32_t Kernel_QueryMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                  void**                 base,
                                  size_t*                bytes)
{
    *base = memregion->BaseVa;
    *bytes = memregion->Bytes;
    return 0;
}


/*!
 * \brief Convert a memory region into global address space
 *
 * \param[in]  memregion  Pointer to the memory region
 * \param[out] globmemregion Pointer to a memory region that is domain-wide
 *
 */
__INLINE__
uint32_t Kernel_CreateGlobalMemoryRegion(const Kernel_MemoryRegion_t* memregion, 
                                               Kernel_MemoryRegion_t* glob_memregion)
{
    *glob_memregion = *memregion;
    glob_memregion->BaseVa = (void*)(((uint64_t)memregion->BasePa) + CONFIG_GLOBAL_BASE);
    return 0;
}


/*!
 * \brief Converts a Physical Address to a Virtual Address
 *
 * The virtual address can only be used within the calling process.
 *
 * \param[in]   PA  Physical address.
 * \param[out]  VA  Pointer to the location where the virtual address
 *                  corresponding to the specified PA is returned.
 *
 * \retval  0             Successful
 * \retval  error_number  Unsuccessful
 *
 */
__INLINE__
uint32_t Kernel_Physical2Virtual(void *PA,
				 void **VA)
{
  uint64_t  j;
  void     *cachePA;

  /* Look in the memory region cache.  It should be there, since to get the
   * original PA, the cache had to be primed.
   */

  /* Point to cache */
  CNKSPI_MemoryRegionCacheElement_t *cachePtr = _cnkspi_MemoryRegionCache;
  j = _cnkspi_MemoryRegionCacheLastAccessedElementNumber;

  /* Look in the cache.
   * This could be in a loop, but it is faster to use an unrolled loop.
   */

  /* Look in the last accessed element of the cache */
  cachePA = cachePtr[j].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[j].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[j].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      return 0;
    }

  /* Look in element 0 of the cache */
  cachePA = cachePtr[0].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[0].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[0].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 0;
      return 0;
    }

  /* Look in element 1 of the cache */
  cachePA = cachePtr[1].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[1].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[1].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 1;
      return 0;
    }

  /* Look in element 2 of the cache */
  cachePA = cachePtr[2].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[2].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[2].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 2;
      return 0;
    }

  /* Look in element 3 of the cache */
  cachePA = cachePtr[3].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[3].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[3].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 3;
      return 0;
    }

  /* Look in element 4 of the cache */
  cachePA = cachePtr[4].PA;
  if ( ( PA >= cachePA ) &&
       ( PA < (void*)((uint64_t)cachePA + cachePtr[4].bytes) ) )
    {
      *VA = (void*)((uint64_t)cachePtr[4].startPtr + ( (uint64_t)PA - (uint64_t)cachePA ));
      _cnkspi_MemoryRegionCacheLastAccessedElementNumber = 4;
      return 0;
    }

  return ENOENT; /* Not found in the cache */
}


/*!
 * \brief Converts a Physical Address to a Global Virtual Address
 *
 * The global virtual address can be used by any process to access the memory
 * corresponding to the specified physical address.
 *
 * \param[in]   PA  Physical address.
 * \param[out]  VA  Pointer to the location where the global virtual address
 *                  corresponding to the specified PA is returned.
 *
 * \retval  0             Successful
 * \retval  error_number  Unsuccessful
 *
 */
__INLINE__
uint32_t Kernel_Physical2GlobalVirtual(void *PA,
				       void **GVA)
{
  *GVA = (void*)((uint64_t)PA + CONFIG_GLOBAL_BASE);
  return 0;
}

/*!
 * \brief Returns the base virtual address for L2 atomic operations
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsBaseAddress(void)
{
    return CONFIG_APP_L2ATOMIC_BASE;
}

/*!
 * \brief Returns the first of 4 WAC registers that the 4 threads of this
 *        core are to use for wakeup-on-address-range operations
 *
 */
__INLINE__
uint64_t Kernel_FirstThreadWakeupWAC(void)
{
    return CNK_WAKEUP_SPI_FIRST_WAC;
}

/*!
 * \brief Returns the base virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pBaseAddress(void)
{
    return VA_MINADDR_L1P;
}

/*!
 * \brief Returns the base virtual address for non-guarded L1P MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL1pBaseAddress(void)
{
    return CONFIG_FAST_L1P_BASE;
}

/*!
 * \brief Returns the base virtual address for non-guarded L2 MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL2BaseAddress(void)
{
    return CONFIG_FAST_L2_BASE;
}


/*!
 * \brief Sets the virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetPatternAddress(void* readptr, void* writeptr, size_t size)
{
    return (uint64_t) CNK_SPI_SYSCALL_3(SETL1PPATTERNADDR, readptr, writeptr, size);
}


/*!
 * \brief Gets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pGetCurrentPatternAddress(void** readptr, void** writeptr)
{
    return (uint64_t) CNK_SPI_SYSCALL_2(GETL1PCURPATTERNADDR, readptr, writeptr);
}

/*!
 * \brief Sets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetCurrentPatternAddress(void* readptr, void* writeptr)
{
    return (uint64_t) CNK_SPI_SYSCALL_2(SETL1PCURPATTERNADDR, readptr, writeptr);
}

/*!
 * \brief Allocates storage for L2 atomic access
 * atomic_vaddress is the virtual address of the L2 atomic storage
 * length is the length of the L2 atomic storage range, in bytes
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsAllocate(void* atomic_vaddress, size_t length)
{
    uint64_t rc;
    do
    {
        rc = CNK_SPI_SYSCALL_2(ALLOCATEL2ATOMIC, atomic_vaddress, length);
    }
    while(rc == EAGAIN);
    return rc;
}


/*!
 * \brief Sets the L2 atomic address used by the messaging software to indicate rollback window is invalid
 *
 */
__INLINE__
uint64_t Kernel_SetRollbackIndicator(volatile uint64_t* indicator)
{
    return (uint64_t) CNK_SPI_SYSCALL_1(ROLLBACKINDICATOR, indicator);
}


__INLINE__
uint64_t Kernel_GetPrivilegeMode(void)
{
    return 0;
}

__INLINE__ int persist_open( char *name, int oflag, mode_t mode )
{
   char pathName[PATH_MAX];
   strcpy(pathName, "/dev/persist/");
   strncat(pathName, name, PATH_MAX - strlen("/dev/persist/") - 1);
   return open(pathName, oflag, mode);
}

__INLINE__
int Kernel_GetMemorySize(enum KERNEL_MEMSIZETYPE type, uint64_t* size)
{
    return (int) CNK_SPI_SYSCALL_2(GETMEMORYSIZE, type, size);
}

__INLINE__
int Kernel_SetLocalFSWindow(size_t fs_size, void* fs_baseaddr)
{
    return (int) CNK_SPI_SYSCALL_2(SETLOCALFSWINDOW, fs_size, fs_baseaddr);
}

#endif /* _KERNEL_CNK_MEMORY_IMPL_H_ */
