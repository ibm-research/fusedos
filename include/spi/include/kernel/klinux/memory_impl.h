/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/*                                                                  */
/* US Government Users Restricted Rights - Use, duplication or      */
/* disclosure restricted by GSA ADP Schedule Contract with IBM      */
/* Corporation.                                                     */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */

#ifndef	_KERNEL_KLINUX_MEMORY_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_KLINUX_MEMORY_IMPL_H_

/**
 * \file klinux/memory_impl.h
 *
 * \brief C Header File containing memory SPI
 *        implementations for the Linux kernel.
 *
 */

#include "kernel_impl.h"
#include <linux/dma-mapping.h>

extern struct device* bg_mu_dev;

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
 * \retval 1  The memory region was not pinned due to an unavailable
 *             resource. The memory region does not need to be destroyed
 *             with Kernel_MemoryRegionDestroy().
 *
 * \retval 2  An invalid parameter value was specified.
 *
 * \retval 3  A general error occurred.  The memory region was not pinned
 *             and does not need to be destroyed with
 *             Kernel_MemoryRegionDestroy().
 *
 */
static inline
uint32_t Kernel_CreateMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                   void*                  base_virtualaddress,
                                   size_t                 requested_size)
{

	dma_addr_t phys_addr;

	phys_addr = dma_map_single(bg_mu_dev, base_virtualaddress, requested_size, DMA_BIDIRECTIONAL);

	/* For now we should BUG if the map failed. */
	BUG_ON(dma_mapping_error(bg_mu_dev, phys_addr));

	if(!phys_addr) {
	  /*Need to set memregion's values to something to avoid compile warnings. */
	  memregion->BaseVa = 0;
	  memregion->BasePa = 0;
	  memregion->Bytes = 0;	  

	  return 3;

	} else {
	
	  memregion->BaseVa = base_virtualaddress;
	  memregion->BasePa = (void *)phys_addr;
	  memregion->Bytes = requested_size;

	  return 0; 
	}
}


/*!
 * \brief Destroy a memory region
 *
 * After a memory region is destroyed it is again legal to write into or
 * deallocate the memory used by the memory region object.
 *
 * \warning It is illegal to query the contents of the memory region with the
 *          DCMF_Memregion_query() function after destroying the memory region.
 *
 * \param[in]  memregion  Pointer to the memory region to be destroyed.
 *
 * \see Kernel_MemoryRegionCreate
 * \see Kernel_MemoryRegionQuery
 */

static inline
uint32_t Kernel_DestroyMemoryRegion(Kernel_MemoryRegion_t* memregion)
{
	dma_unmap_single(bg_mu_dev, (dma_addr_t)memregion->BasePa, memregion->Bytes, DMA_BIDIRECTIONAL);
	
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
    return ENOSYS;
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
  return ENOSYS;
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
  return ENOSYS;
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
  return ENOSYS;
}

/*!
 * \brief Returns the base virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pBaseAddress(void)
{
    return ENOSYS;
}

/*!
 * \brief Returns the base virtual address for non-guarded L1P MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL1pBaseAddress(void)
{
    return ENOSYS;
}

/*!
 * \brief Returns the base virtual address for non-guarded L2 MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL2BaseAddress(void)
{
    return ENOSYS;
}


/*!
 * \brief Sets the virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetPatternAddress(void* readptr, void* writeptr, size_t size)
{
    return ENOSYS;
}


/*!
 * \brief Gets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pGetCurrentPatternAddress(void** readptr, void** writeptr)
{
    return ENOSYS;
}

/*!
 * \brief Sets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetCurrentPatternAddress(void* readptr, void* writeptr)
{
    return ENOSYS;
}


/*!
 * \brief Returns the base virtual address for L2 atomic operations
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsBaseAddress(void)
{
    return ENOSYS;
}

/*!
 * \brief Returns the first of 4 WAC registers that the 4 threads of this
 *        core are to use for wakeup-on-address-range operations
 *
 */
__INLINE__
uint64_t Kernel_FirstThreadWakeupWAC(void)
{
    return ENOSYS;
}

__INLINE__
uint64_t Kernel_GetPrivilegeMode(void)
{
    return 1;
}


#endif /* _KERNEL_KLINUX_MEMORY_IMPL_H_ */
