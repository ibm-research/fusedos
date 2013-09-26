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

#ifndef	_KERNEL_MEMORY_H_ /* Prevent multiple inclusion */
#define	_KERNEL_MEMORY_H_

/*!
 * \file memory.h
 *
 * \brief C Header File containing SPI Addressing Inline Functions
 *
 */

#include "kernel_impl.h" 

#include "hwi/include/common/compiler_support.h"

/*!
 * \brief Memory Region
 *
 * The memory region structure contains information about a block of
 * contiguous memory.  
 * 
 */
typedef struct Kernel_MemoryRegion
{
    size_t    Bytes;  /*!< Number of bytes in the memory region. */
    void*     BaseVa; /*!< Virtual address of the memory region. */
    void*     BasePa; /*!< The physical address corresponding to the BaseVa */
} __attribute__((__packed__)) Kernel_MemoryRegion_t;

__BEGIN_DECLS

/*!
 * \brief __INLINE__ definition
 * 
 * Option 1:
 * Make all functions be "static inline":
 * - They are inlined if the compiler can do it
 * - If the compiler does not inline it, a single copy of the function is
 *   placed in the translation unit (eg. xxx.c)for use within that unit.
 *   The function is not externalized for use by another unit...we want this
 *   so we don't end up with multiple units exporting the same function,
 *   which would result in linker errors.
 *
 * Option 2:
 * A GNU C model: Use "extern inline" in a common header (this one) and provide
 * a definition in a .c file somewhere, perhaps using macros to ensure that the
 * same code is used in each case. For instance, in the header file:
 *
 * \verbatim
   #ifndef INLINE
   # define INLINE extern inline
   #endif
   INLINE int max(int a, int b) {
     return a > b ? a : b;
   }
   \endverbatim
 *
 * ...and in exactly one source file (in runtime/SPI), that is included in a
 * library...
 *
 * \verbatim
   #define INLINE
   #include "header.h"
   \endverbatim
 * 
 * This allows inlining, where possible, but when not possible, only one 
 * instance of the function is in storage (in the library).
 */
#ifndef __INLINE__
#define __INLINE__ extern inline
#endif


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
 * \param[in]      base_virtualaddress Virtual address of the buffer.
 * \param[in]      requested_size      Length of the buffer (in bytes).
 *                                     This is the number of bytes requested
 *                                     to be in the memory region.
 *
 * \warning After creating the memory region it is illegal to write into or
 *          deallocate the opaque memory region object.  Additionally, it is
 *          illegal to free or otherwise deallocate the memory buffer managed
 *          by the memory region.
 *
 * \see Kernel_DestroyMemoryRegion
 * \see Kernel_QueryMemoryRegion
 *
 * \retval  0  The entire memory region, or a portion of the memory
 *             region was pinned. The actual number of bytes pinned
 *             from the start of the buffer is stored in the
 *             \c Bytes field of the memory region structure.
 *             The memory region must be destroyed with 
 *             Kernel_DestroyMemoryRegion().
 *
 * \retval 1  The memory region was not pinned due to an unavailable
 *             resource. The memory region does not need to be destroyed
 *             with Kernel_DestroyMemoryRegion().
 *
 * \retval 2  An invalid parameter value was specified.
 *
 * \retval 3  A general error occurred.  The memory region was not pinned
 *             and does not need to be destroyed with
 *             Kernel_DestroyMemoryRegion().
 *
 */

__INLINE__
uint32_t Kernel_CreateMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                   void*                  base_virtualaddress,
                                   size_t                 requested_size);

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
 * \see Kernel_CreateMemoryRegion
 * \see Kernel_QueryMemoryRegion
 */

__INLINE__
uint32_t Kernel_DestroyMemoryRegion(Kernel_MemoryRegion_t* memregion);

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
 * \see Kernel_CreateMemoryRegion
 * \see Kernel_DestroyMemoryRegion
 */

__INLINE__
uint32_t Kernel_QueryMemoryRegion(Kernel_MemoryRegion_t* memregion,
                                  void**                 base,
                                  size_t*                bytes);



/*!
 * \brief Converts a memory region into global address space
 *
 * \param[in]  memregion  Pointer to the memory region
 * \param[out] glob_memregion Pointer to a memory region that is domain-wide
 *
 */
__INLINE__
uint32_t Kernel_CreateGlobalMemoryRegion(const Kernel_MemoryRegion_t* memregion, 
                                               Kernel_MemoryRegion_t* glob_memregion);



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
				 void **VA);



/*!
 * \brief Converts a Physical Address to a Global Virtual Address
 *
 * The global virtual address can be used by any process to access the memory
 * corresponding to the specified physical address.
 *
 * \param[in]   PA  Physical address.
 * \param[out]  GVA  Pointer to the location where the global virtual address
 *                  corresponding to the specified PA is returned.
 *
 * \retval  0             Successful
 * \retval  error_number  Unsuccessful
 *
 */
__INLINE__
uint32_t Kernel_Physical2GlobalVirtual(void *PA,
				       void **GVA);


/*!
 * \brief Returns the base virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pBaseAddress(void);

/*!
 * \brief Returns the base virtual address for non-guarded L1p MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL1pBaseAddress(void);

/*!
 * \brief Returns the base virtual address for non-guarded L2 MMIO access.
 *
 */
__INLINE__
uint64_t Kernel_FastL2BaseAddress(void);


/*!
 * \brief Sets the virtual address for L1p operations
 *
 */
__INLINE__
uint64_t Kernel_L1pSetPatternAddress(void* readptr, void* writeptr, size_t size);


/*!
 * \brief Gets the current list read/write pointer's current working address.  
 
 This indicates how far the L1p perfect prefetcher has progressed in memory.  
 *
 */
__INLINE__
uint64_t Kernel_L1pGetCurrentPatternAddress(void** readptr, void** writeptr);

/*!
 * \brief Sets the current list's read/write pointers to the specified address
 
 This allows the prefetcher to resume list operations at a specific point.
 *
 */
__INLINE__
uint64_t Kernel_L1pSetCurrentPatternAddress(void* readptr, void* writeptr);



/*!
 * \brief Returns the base virtual address for L2 atomic operations
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsBaseAddress(void);


/*!
 * \brief Returns the first of 4 WAC registers that the 4 threads of this
 *        core are to use for wakeup-on-address-range operations
 *
 */
__INLINE__
uint64_t Kernel_FirstThreadWakeupWAC(void);


/*!
 * \brief Returns true if the current thread is privileged.
 *
 */
__INLINE__
uint64_t Kernel_GetPrivilegeMode(void);


/*!
 * \brief Allocates storage for L2 atomic access
 * atomic_vaddress is the virtual address of the L2 atomic storage
 * length is the length of the L2 atomic storage range, in bytes
 *
 */
__INLINE__
uint64_t Kernel_L2AtomicsAllocate(void* atomic_vaddress, size_t length);


/*!
 * \brief Sets the L2 atomic address used by the messaging software to indicate rollback window is invalid
 *
 */
__INLINE__
uint64_t Kernel_SetRollbackIndicator(volatile uint64_t* ptr);

/*! \brief Persistent Shared Memory interface to application. Currently, simlpy a wrapper to open(2),
 *         with a prefix of /dev/persist
 */
__INLINE__
int persist_open( char *name, int oflag, uint32_t mode );


/*! \brief Memory region types that can be used for Kernel_GetMemorySize. 
*/
enum KERNEL_MEMSIZETYPE
{
    KERNEL_MEMSIZE_SHARED = 200,       /*!< Size in bytes of shared memory */
    KERNEL_MEMSIZE_PERSIST,            /*!< Size in bytes of persistent memory */
    KERNEL_MEMSIZE_HEAPAVAIL,          /*!< Size in bytes of available heap (must be process leader (a.k.a main) thread) */
    KERNEL_MEMSIZE_ESTHEAPAVAIL,       /*!< Estimated size in bytes of the available heap */
    KERNEL_MEMSIZE_STACKAVAIL,         /*!< Size in bytes available to the process leader's stack. (must be process leader (a.k.a. main) thread) */
    KERNEL_MEMSIZE_ESTSTACKAVAIL,      /*!< Estimated size in bytes available to the process leader's stack */
    KERNEL_MEMSIZE_STACK,              /*!< Size in bytes of the process leader's stack (must be process leader (a.k.a. main) thread) */
    KERNEL_MEMSIZE_ESTSTACK,           /*!< Estimated size in bytes available to the process leader's stack */
    KERNEL_MEMSIZE_HEAP,               /*!< Size in bytes of the heap size*/
    KERNEL_MEMSIZE_GUARD,              /*!< Size in bytes of the heap guardpage */
    KERNEL_MEMSIZE_HEAPMAX,            /*!< Size in bytes of the maximum heap size */
    KERNEL_MEMSIZE_MMAP                /*!< Size in bytes of memory currently consumed by anonymous mmaps */

};

/*! \brief Returns size of the specified memory region. 
 */
__INLINE__ 
int Kernel_GetMemorySize(enum KERNEL_MEMSIZETYPE type, uint64_t* size);

/*! \brief Defines a region in user memory that the kernel can use for an arbitrarily sized local file system.

  \note On CNK, this file system is mounted at /dev/local
 
  \param[in] fs_size Size of the region in bytes
  \param[in] fs_baseaddr The pointer to the memory region
 */
__INLINE__
int Kernel_SetLocalFSWindow(size_t fs_size, void* fs_baseaddr);


/**
 * \brief Include implementations of the above functions.
 
 */
#include "memory_impl.h"

__END_DECLS

#endif /* _KERNEL_MEMORY_H_ */
