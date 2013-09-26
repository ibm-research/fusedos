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

#ifndef	_KERNEL_FIRMWARE_MEMORY_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_FIRMWARE_MEMORY_IMPL_H_

#include <firmware/include/VirtualMap.h>
#include <hwi/include/bqc/l1p_mmio.h>
#include <hwi/include/bqc/A2_inlines.h>

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
__INLINE__
uint32_t Kernel_CreateMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                   void*                  base_virtualaddress,
                                   size_t                 requested_size)
{
    int rc = 0;

    memregion->BaseVa = base_virtualaddress;
    memregion->BasePa = base_virtualaddress;
    memregion->Bytes  = requested_size;

    return rc;
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
  //*glob_memregion = *memregion;
  //glob_memregion->BaseVa = (void*)(((uint64_t)memregion->BasePa) + 0x8000000000000000);

  //Is this supported in firmware ????????

  return 1;
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
  *VA = PA;
  return 0;
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
  /* Is this supported in firmware? */
  return 1;
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
    // We don't have a non-guarded mapping available in a firmware client.
    // Just use the regular guarded address.
    return (VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET);
}

/*!
 * \brief Returns the base virtual address for non-guarded L2 MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL2BaseAddress(void)
{
    // We don't have a non-guarded mapping available in a firmware client.
    // Just use the regular guarded address.
    return VA_MINADDR_L2;
}


/*!
 * \brief Sets the virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetPatternAddress(void* readptr, void* writeptr, size_t size)
{
    if((readptr == NULL) && (writeptr == NULL))
    {
        *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_WRITE)) = 0;
        *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_READ))  = 0;
        *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_MAXSIZE))    = 0;
        return 0;
    }
    
    if(size<128) 
        return EINVAL;
    
    *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_WRITE)) = (((uint64_t)writeptr) + 63)&~(63);
    *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_READ))  = (((uint64_t)readptr)  + 63)&~(63);
    *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_MAXSIZE))    = (((uint64_t)size) - 64)&~(63);
    return 0;
}


/*!
 * \brief Gets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pGetCurrentPatternAddress(void** readptr, void** writeptr)
{
    *writeptr = (void*) (*((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_CURWRITE)) * 16);
    *readptr  = (void*) (*((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_CURREAD)) * 16);
    return 0;
}

/*!
 * \brief Sets the current virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetCurrentPatternAddress(void* readptr, void* writeptr)
{
    *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_CURWRITE)) = ((uint64_t)writeptr)/16;
    *((uint64_t*)(VA_MINADDR_L1P + VA_PRIVILEGEDOFFSET + L1P_PP_THREADOFFSET * PhysicalThreadID() + L1P_PP_ADDR_CURREAD))  = ((uint64_t)readptr)/16;
    return 0;
}


/*!
 * \brief Returns the base virtual address for L2 atomic operations
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsBaseAddress(void)
{
    return VA_MINADDR_CATOMICOPS;
}

/*!
 * \brief Returns the first of 4 WAC registers that the 4 threads of this
 *        core are to use for wakeup-on-address-range operations
 *
 */
__INLINE__
uint64_t Kernel_FirstThreadWakeupWAC(void)
{
    return 0;
}

__INLINE__
uint64_t Kernel_GetPrivilegeMode(void)
{
    return 1;
}

#endif /* _KERNEL_FIRMWARE_MEMORY_IMPL_H_ */
