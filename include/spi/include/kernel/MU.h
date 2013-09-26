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

#ifndef	_KERNEL_MU_H_ /* Prevent multiple inclusion */
#define	_KERNEL_MU_H_

/**
 * \file MU.h
 *
 * \brief C Header File containing Kernel Message Unit SPI Definitions.
 *
 */


#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/dcr_support.h>
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/dcr_names.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/kernel/memory.h>


/* Messaging Unit Kernel Interfaces */
__BEGIN_DECLS


/**
 * \brief Injection Fifo Attributes
 * 
 * Defines the attributes of an injection fifo:
 *
 * - Whether it is reserved for use by remote get operations.
 *   A remote get injection fifo can only be used by the MU
 *   for remote gets.  The cores cannot inject descriptors into them.
 *
 * - Whether it is reserved for use by the system (privileged code).
 *   A system injection fifo can only be used by privileged code for
 *   system purposes.  User-mode cannot inject descriptors into them.
 */
typedef struct Kernel_InjFifoAttributes
{
  uint32_t RemoteGet : 1; /**< The injection fifo is reserved
			       for use by the MU to inject
			       descriptors specified on remote
			       get operations. */
  uint32_t System    : 1; /**< The injection fifo is reserved
			       for use by privileged system
			       code.  Can only be set to 1 if
			       running in privileged mode. */
  uint32_t Priority  : 1; /**< This injection fifo is a high priority fifo */

} Kernel_InjFifoAttributes_t;


/**
 * \brief Injection Fifo Interrupt Configuration
 * 
 * Defines the interrupt configuration of an injection fifo:
 *
 * - Whether an interrupt should fire when an injection fifo threshold is
 *   crossed:
 *   - If it is a "normal" injection fifo, an interrupt will fire when the
 *     fifo's free space becomes greater than a threshold.
 *   - If it is a "remote get" injection fifo, an interrupt will fire when
 *     the fifo's free space becomes less than a threshold.
 */
typedef struct Kernel_InjFifoInterrupts
{
  uint32_t Threshold_Crossing : 1; /**< An interrupt should fire when an
				        injection fifo threshold is crossed. */
} Kernel_InjFifoInterrupts_t;


/**
 * \brief Reception Fifo Attributes
 * 
 * Defines the attributes of an injection fifo:
 *
 * - Whether it is reserved for use by the system (privileged code).
 *   A system reception fifo can only be used by privileged code for
 *   system purposes.  User-mode cannot use them.
 */
typedef struct Kernel_RecFifoAttributes
{
  uint32_t System    : 1; /**< The reception fifo is reserved
			       for use by privileged system
			       code.  Can only be set to 1 if
			       running in privileged mode. */
} Kernel_RecFifoAttributes_t;


/**
 * \brief Injection Fifo Activate
 * 
 * Indicates to Kernel_InjFifoActivate() that the injection fifos should
 * be activated.
 */
#define KERNEL_INJ_FIFO_ACTIVATE 1


/**
 * \brief Injection Fifo Deactivate
 * 
 * Indicates to Kernel_InjFifoActivate() that the injection fifos should
 * be deactivated.
 */
#define KERNEL_INJ_FIFO_DEACTIVATE 0


/**
 * \brief Reception Fifo Interrupt Configuration
 * 
 * Defines the interrupt configuration of a reception fifo:
 *
 * - Whether an interrupt should fire when a reception fifo threshold is
 *   crossed:  when the fifo free space drops below the threshold value.
 *
 * - Whether an interrupt should fire when a packet whose header has the
 *   "interrupt" bit set arrives in the reception fifo.
 */
typedef struct Kernel_RecFifoInterrupts
{
  uint32_t Threshold_Crossing : 1; /**< An interrupt should fire when an
				        injection fifo threshold is crossed. */
  uint32_t Packet_Arrival     : 1; /**< An interrupt should fire when an
				        "interrupt" packet arrives in the
					fifo. */
} Kernel_RecFifoInterrupts_t;


/**
 * \brief Subgroup Participates in Global Event Aggregation
 * 
 * Indicates that interrupts that occur in a subgroup also raise an interrupt
 * in the Global Event Aggregator (GEA) unit.
 */
#define KERNEL_SUBGROUP_INTERRUPTS_GO_TO_GEA 1


/**
 * \brief Subgroup Does Not Participate in Global Event Aggregation
 * 
 * Indicates that interrupts that occur in a subgroup do not raise an interrupt
 * in the Global Event Aggregator (GEA) unit.
 */
#define KERNEL_SUBGROUP_INTERRUPTS_DO_NOT_GO_TO_GEA 0


/**
 * \brief Query Deallocated MU InjFifos within a SubGroup
 *
 * Query the allocation status of injection fifos in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated fifos.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid       SubGroup number being queried 
 *                             (0 to MU_NUM_INJ_FIFOS_PER_SUBGROUP-1)
 * \param[out]  nfifos         Pointer to an int where the number of deallocated
 *                             fifos in the specified SubGroup is returned
 * \param[out]  fifoids        Pointer to an array of num_fifos ints where
 *                             the list of deallocated fifos is returned.
 *                             Each entry is the fifo number 
 *                             (0 to DMA_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *                             The caller must provide space for 
 *                             MU_NUM_INJ_FIFOS_PER_SUBGROUP ints,
 *                             in case the entire fifo SubGroup is deallocated.
 *
 * \retval  0  Successful.     nfifos and fifoids array set as described.
 * \retval  errno Unsuccessful. 
 *
 */
int32_t Kernel_QueryInjFifos (uint32_t   subgrpid,
			      uint32_t * nfifos,
			      uint32_t * fifoids);


/**
 * \brief Query Deallocated MU RecFifos within a SubGroup
 *
 * Query the allocation status of reception fifos in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated fifos.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid       SubGroup number being queried 
 *                             (0 to MU_NUM_REC_FIFOS_PER_SUBGROUP-1)
 * \param[out]  nfifos         Pointer to an int where the number of deallocated
 *                             fifos in the specified SubGroup is returned
 * \param[out]  fifoids        Pointer to an array of num_fifos ints where
 *                             the list of deallocated fifos is returned.
 *                             Each entry is the fifo number 
 *                             (0 to MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 *                             The caller must provide space for 
 *                             MU_NUM_REC_FIFOS_PER_SUBGROUP ints,
 *                             in case the entire fifo SubGroup is deallocated.
 *
 * \retval  0  Successful.     num_fifos and fifo_ids array set as described.
 * \retval  errno Unsuccessful.
 *
 */
int32_t Kernel_QueryRecFifos (uint32_t   subgrpid,
			      uint32_t * nfifos,
			      uint32_t * fifoids);


/**
 * \brief Query Deallocated Base Address Table within a SubGroup
 *
 * Query the allocation status of base address table entries in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated entries.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped lock,
 * if necessary.
 *
 * \param[in]   subgrpid  SubGroup being queried (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  nbatids   Pointer to the number IDs returned in batids 
 *                        (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).  Zero means
 *                        that no IDs are deallocated in this SubGroup.
 * \param[out]  batids    Pointer to an array of Base Address Table IDs where
 *                        the list of deallocated Base Address Table IDs is
 *                        returned.  Each entry is the Base Address Table ID
 *                        (0 to MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *                        The caller must provide space for
 *                        BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP IDs to be returned.
 *
 * \retval  0         Success
 * \retval  errno     Unsuccessful.
 *
 * \see Kernel_AllocateBaseAddressTable
 * \see Kernel_DeallocateBaseAddressTable
 *                          
 */
int32_t Kernel_QueryBaseAddressTable (uint32_t   subgrpid,
				      uint32_t * nbatids,
				      uint32_t * batids);


/**
 * \brief Allocate Injection Fifos
 *
 * Allocate specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being allocated (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  sg_ptr    Pointer to a structure that is filled in upon
 *                        successful return for use in other inline 
 *                        functions to operate on the allocated fifos. 
 *                        This call first zeros all fields in the 
 *                        subgrp structure before filling in the 
 *                        allocated fifos.
 *                        \li fifos - Array of fifo structures. Structures
 *                                    for allocated fifos are initialized 
 *                                    as documented below.  Structures 
 *                                    for fifos not allocated by this 
 *                                    instance of this syscall are 
 *                                    initialized to binary zeros.  
 *                                    Allocated fifos are enabled.
 *                        \li status_ptr  - Points to status area within 
 *                                          the MU memory map.
 *                        \li permissions - Bits indicating which fifos 
 *                                          were allocated during this 
 *                                          syscall.
 *                        \li group_id    - The id of this group.
 * \param[in] nfifos     Number of fifos to be allocated from the group
 *                       (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in] fifoids    Pointer to an array of nfifos uint32_t's where
 *                       the list of fifos to be allocated is provided.
 *                       Each int32_t is the fifo number within the SubGroup
 *                       (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoAttrs  Pointer to an array of nfifos
 *                       Kernel_InjFifoAttributes_t's where the attributes of
 *                       each fifo are specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 */
int32_t Kernel_AllocateInjFifos (uint32_t                     subgrpid,
				 MUSPI_InjFifoSubGroup_t    * sg_ptr,
				 uint32_t                     nfifos,
				 uint32_t                   * fifoids,
				 Kernel_InjFifoAttributes_t * fifoAttrs);


/**
 * \brief Allocate Reception Fifos
 *
 * Allocate specified reception fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being allocated (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  sg_ptr    Pointer to a structure that is filled in upon
 *                        successful return for use in other inline 
 *                        functions to operate on the allocated fifos. 
 *                        This call first zeros all fields in the 
 *                        subgrp structure before filling in the 
 *                        allocated fifos.
 *                        \li fifos - Array of fifo structures. Structures
 *                                    for allocated fifos are initialized 
 *                                    as documented below.  Structures 
 *                                    for fifos not allocated by this 
 *                                    instance of this syscall are 
 *                                    initialized to binary zeros.  
 *                                    Allocated fifos are enabled.
 *                        \li status_ptr  - Points to status area within 
 *                                          the MU memory map.
 *                        \li permissions - Bits indicating which fifos 
 *                                          were allocated during this 
 *                                          syscall.
 *                        \li group_id    - The id of this group.
 * \param[in] nfifos     Number of fifos to be allocated from the group
 *                       (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in] fifoids    Pointer to an array of nfifos int32_t's where
 *                       the list of fifos to be allocated is provided.
 *                       Each int32_t is the fifo number within the SubGroup
 *                       (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoAttrs  Pointer to an array of nfifos
 *                       Kernel_RecFifoAttributes_t's where the attributes of
 *                       each fifo are specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 */

int32_t Kernel_AllocateRecFifos (uint32_t                     subgrpid,
				 MUSPI_RecFifoSubGroup_t    * sg_ptr,
				 uint32_t                     nfifos,
				 uint32_t                   * fifoids,
				 Kernel_RecFifoAttributes_t * fifoAttrs);


/**
 * \brief Allocate Base Address Table
 *
 * Allocate specified base address table entries in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped 
 * lock, if necessary.
 *
 * \param[in]  subgrpid  SubGroup being allocated (0 to 
 *                       BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out] sg_ptr    Pointer to a base address table SubGroup structure
 *                       that is initialized by this function.
 * \param[in]  nbatids   Number of IDs to be allocated (1 to 
 *                       BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).
 * \param[in]  batids    Array of Base Address Table IDs to be allocated.
 * \param[in]  system    Indicates whether the Base Address Table IDs being allocated
 *                       are for "system" use or for "user" use.
 *                       0 = User use.
 *                       1 = System use.
 *                       Note that all Base Address Table IDs in a subgroup have the
 *                       same "system" attribute.  That is, they are all either for
 *                       "system" use or the are all for "user" use.  Thus, if
 *                       there are some IDs already allocated, the "system" attribute
 *                       has already been determined.  If the "system" attribute
 *                       attribute specified on this call conflicts with that,
 *                       this call will fail and no IDs will be allocated.
 *
 * \retval  0  Successful.    batids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 * \retval  errno Unsuccessful.  There are already some IDs allocated in this subgroup
 *                               and the specified "system" parameter is inconsistent
 *                               with the "system" state of the already allocated IDs.
 *
 * \see Kernel_QueryBaseAddressTable
 * \see Kernel_DeallocateBaseAddressTable
 *                          
 */
int32_t Kernel_AllocateBaseAddressTable (uint32_t                           subgrpid,
					 MUSPI_BaseAddressTableSubGroup_t * sg_ptr,
					 uint32_t                           nbatids,
					 uint32_t                         * batids,
					 uint32_t                           system);


/**
 * \brief MU InjFifo Initialization 
 *
 * - For an allocated injection MU fifo, initialize its start, 
 *   head, tail, and end.  
 * - Initialize fifo size and free space and desc count
 *
 * \param[in]  sg_ptr       Pointer to fifo SubGroup structure.
 * \param[in]  fifoid       Id of the fifo to be initialized
 *                          (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  memregion    Memory region of the injection fifo.
 * \param[in]  startoffset  The offset in the memory region where the start, 
 *                          head and tail pointers are initialized to.
 *                          The fifo start address must be aligned on a 64-byte
 *                          boundary.
 * \param[in]  size_1       Size of the injection fifo minus 1, in bytes.
 *                          - size_1+1 must be a multiple of 64 bytes.
 *                          - size_1+1 must be at least
 *                            - BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES bytes for a
 *                              non-remote-get fifo.
 *                            - BGQ_MU_MIN_RGETFIFO_SIZE_IN_BYTES bytes for a
 *                              remote-get fifo.
 *
 * \retval   0  Successful.
 * \retval  errno  Unsuccessful.  Error checks include
 *              - The fifo's start address is aligned on a 64-byte boundary.
 *              - The fifo's size is a multiple of 64 bytes.
 *              - The fifo's size is at least BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES
 *                bytes.
 *              Note:  The minimum size of a remote get fifo is not enforced on
 *                     this interface.  Be careful to meet the minimum size
 *                     requirements documented in the size_1 parameter.
 *
 */
int32_t Kernel_InjFifoInit (MUSPI_InjFifoSubGroup_t * sg_ptr,
			    uint32_t                  fifoid,
			    Kernel_MemoryRegion_t   * memregion, 
			    uint64_t                  startoffset, 
			    uint64_t                  size_1);


/**
 * \brief MU RecFifo Initialization 
 *
 * - For an allocated reception fifo, initialize its start, head, tail, and 
 *   end.  
 *
 * \param[in]  sg_ptr       Pointer to fifo SubGroup structure.
 * \param[in]  fifoid       Id of the fifo to be initialized
 *                          (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  memregion    Memory region of the reception fifo.
 * \param[in]  startoffset  The offset in the memory region where the start, 
 *                          head and tail pointers are initialized to.
 *                          The fifo start address must be aligned on a 32-byte
 *                          boundary.
 * \param[in]  size_1       Size of the reception fifo minus 1, in bytes.
 *                          - size_1+1 must be a multiple of 32 bytes.
 *                          - size_1+1 must be at least
 *                            BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES bytes.
 *
 * \retval   0  Successful.
 * \retval  errno  Unsuccessful.  Error checks include
 *              - The fifo's start address is aligned on a 32-byte boundary.
 *              - The fifo's size is a multiple of 32 bytes.
 *              - The fifo's size is at least BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES
 *                bytes.
 */

int32_t Kernel_RecFifoInit (MUSPI_RecFifoSubGroup_t * sg_ptr,
			    uint32_t                  fifoid,
			    Kernel_MemoryRegion_t   * memregion,
			    uint64_t                  startoffset, 
			    uint64_t                  size_1);


/**
 * \brief Activate MU InjFifo 
 *
 * Activate a MU injection fifo, given a fifo SubGroup and fifo id.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with activations and deactivations
 * and deallocations of injection fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]  nfifos    Number of fifos to be activated (or deactivated)
 *                       (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids   Ids of the fifos within the group
 *                       (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  activate  Flag indicating whether to activate or deactivate:
 *                       - KERNEL_INJ_FIFO_ACTIVATE
 *                       - KERNEL_INJ_FIFO_DEACTIVATE
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 *
 */
int32_t Kernel_InjFifoActivate (MUSPI_InjFifoSubGroup_t  *  sg_ptr,
				uint32_t                    nfifos,
				uint32_t                 *  fifoids,
				uint32_t                    activate);


/**
 * \brief Enable MU RecFifos
 *
 * Enable MU reception fifos, given a fifo group id and
 * a bit mask indicating which reception fifos in the group
 * to enable.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling and disabling
 * and deallocations of reception fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  groupid     Id of the fifo group
 *                         (0 to BGQ_MU_NUM_FIFO_GROUPS-1).
 * \param[in]  enableBits  Bit mask indicating which fifos
 *                         to enable.  There are 16 fifos
 *                         in a group.  The low-order 16 bits
 *                         are used:
 *                         - Bit 48 is fifo 0
 *                         - Bit 49 is fifo 1
 *                         - and so on...
 *                         - Bit 63 is fifo 15.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 *
 */
__INLINE__
int32_t Kernel_RecFifoEnable (  uint32_t groupid,
				uint64_t enableBits );


/**
 * \brief Disable MU RecFifos
 *
 * Disable MU reception fifos, given a fifo group id and
 * a bit mask indicating which reception fifos in the group
 * to disable.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling and disabling
 * and deallocations of reception fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  groupid     Id of the fifo group
 *                         (0 to BGQ_MU_NUM_FIFO_GROUPS-1).
 * \param[in]  disableBits Bit mask indicating which fifos
 *                         to disable.  There are 16 fifos
 *                         in a group.  The low-order 16 bits
 *                         are used:
 *                         - Bit 48 is fifo 0
 *                         - Bit 49 is fifo 1
 *                         - and so on...
 *                         - Bit 63 is fifo 15.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 *
 */
__INLINE__
int32_t Kernel_RecFifoDisable (  uint32_t groupid,
				 uint64_t disableBits );


/**
 * \brief Configure Injection Fifo Interrupts
 *
 * Configure interrupts for specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos in the same group.  
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]   nfifos    Number of fifos to be configured within the subgroup.
 *                        (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]   fifoids   Pointer to an array of nfifos int32_t's where
 *                        the list of fifo numbers to be configured is provided.
 *                        Each int32_t is the fifo number within the SubGroup
 *                        (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoInterrupts  Pointer to an array of nfifos 
 *                            Kernel_InjFifoInterrupts_t's where
 *                            the interrupt configuration of each fifo is 
 *                            specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoThresholds
 */
int32_t Kernel_ConfigureInjFifoInterrupts (MUSPI_InjFifoSubGroup_t    * sg_ptr,
					   uint32_t                     nfifos,
					   uint32_t                   * fifoids,
					   Kernel_InjFifoInterrupts_t * fifoInterrupts);


/**
 * \brief Configure Reception Fifo Interrupts
 *
 * Configure interrupts for specified reception fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos in the same group.  
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]   nfifos    Number of fifos to be configured within the subgroup.
 *                        (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in]   fifoids   Pointer to an array of nfifos int32_t's where
 *                        the list of fifo numbers to be configured is provided.
 *                        Each int32_t is the fifo number within the SubGroup
 *                        (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoInterrupts  Pointer to an array of nfifos 
 *                            Kernel_RecFifoInterrupts_t's where
 *                            the interrupt configuration of each fifo is 
 *                            specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoThreshold
 */
int32_t Kernel_ConfigureRecFifoInterrupts (MUSPI_RecFifoSubGroup_t    * sg_ptr,
					   uint32_t                     nfifos,
					   uint32_t                   * fifoids,
					   Kernel_RecFifoInterrupts_t * fifoInterrupts);


/**
 * \brief Configure the MU GEA for a Subgroup
 *
 * Configure the MU Global Event Aggregator for a subgroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with configuring the MU GEA
 * for other subgroups.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being configured (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[in]   gea_flag  Global Event Aggregation flag.  Indicates whether
 *                        interrupts in this subgroup participate in raising
 *                        one of the interrupt lines connected to the
 *                        global event aggregator unit.
 *                        - KERNEL_SUBGROUP_INTERRUPTS_GO_TO_GEA means "yes".
 *                        - KERNEL_SUBGROUP_INTERRUPTS_DO_NOT_GO_TO_GEA means "no".
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 */
int32_t Kernel_Configure_MU_GEA(uint32_t subgrpid,
				uint64_t gea_flag);



/**
 * \brief Get the Injection Fifo Thresholds
 *
 * Get the normal and remote get Injection Fifo thresholds.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in an injection fifo becomes greater than the threshold value, 
 * regardless of whether the the fifo is activated.  The interrupt is raised if the 
 * fifo has been configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out] normalThreshold     Pointer to the interrupt threshold for all "normal"
 *                                 injection fifos on the node.  This value is in
 *                                 units of descriptors (64-byte units).
 *                                 If NULL, this threshold is not retured.
 * \param[out] remoteGetThreshold  Pointer to the interrupt threshold for all "remote get"
 *                                 injection fifos on the node.  This
 *                                 value is in units of descriptors (64-byte 
 *                                 units).  If NULL, this threshold is not returned.
 *
 * \retval  0  Successful.  The threshold values are returned in the normalThreshold
 *                          and remoteGetThreshold parameters.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoInterrupts
 * \see Kernel_ConfigureInjFifoThresholds
 */
int32_t Kernel_GetInjFifoThresholds( uint64_t * normalThreshold,
				     uint64_t * remoteGetThreshold );


/**
 * \brief Configure the Injection Fifo Thresholds
 *
 * Configure the normal and remote get Injection Fifo thresholds.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in an injection fifo becomes greater than the threshold value, 
 * regardless of whether the the fifo is activated.  The interrupt is raised if the 
 * fifo has been configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in] normalThreshold     Pointer to the interrupt threshold for all "normal"
 *                                injection fifos on the node.  This value is in
 *                                units of descriptors (64-byte units).
 *                                If NULL, this threshold is not set.
 * \param[in] remoteGetThreshold  Pointer to the interrupt threshold for all "remote get"
 *                                injection fifos on the node.  This
 *                                value is in units of descriptors (64-byte 
 *                                units).  If NULL, this threshold is not set.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoInterrupts
 */
int32_t Kernel_ConfigureInjFifoThresholds( uint64_t * normalThreshold,
					   uint64_t * remoteGetThreshold );


/**
 * \brief Get the Reception Fifo Threshold
 *
 * Configure the Reception Fifo threshold.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in a reception fifo drops below this value, regardless of whether
 * the fifo is enabled.  The interrupt is raised if the fifo has been
 * configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out] threshold     The interrupt threshold for all reception
 *                           fifos on the node.  This value is the number of 
 *                           32-byte packet chunks.
 *
 * \retval  0  Successful.  The threshold value is returned in the threshold
 *                          parameter.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoInterrupts
 * \see Kernel_ConfigureRecFifoThreshold
 */
int32_t Kernel_GetRecFifoThreshold( uint64_t * threshold );


/**
 * \brief Configure the Reception Fifo Threshold
 *
 * Configure the Reception Fifo threshold.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in a reception fifo drops below this value, regardless of whether
 * the fifo is enabled.  The interrupt is raised if the fifo has been
 * configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in] threshold     The interrupt threshold for all reception
 *                          fifos on the node.  This value is the number of 
 *                          32-byte packet chunks.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoInterrupts
 */
int32_t Kernel_ConfigureRecFifoThreshold( uint64_t threshold );


/**
 * \brief Deallocate Injection Fifos
 *
 * Deallocate specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  sg_ptr   Pointer to fifo SubGroup structure.
 * \param[in]  nfifos   Number of fifos to be deallocated from the group
 *                      (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids  Pointer to an array of nfifos int32_t's where
 *                      the list of fifos to be deallocated is provided.
 *                      Each int32_t is the fifo number within the SubGroup
 *                      (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All fifos were deallocated.
 * \retval  errno  Unsuccessful.
 *
 *
 */
int32_t Kernel_DeallocateInjFifos (MUSPI_InjFifoSubGroup_t * sg_ptr,
				   uint32_t                  nfifos,
				   uint32_t                * fifoids);


/**
 * \brief Deallocate Reception Fifos
 *
 * Deallocate specified reception fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  sg_ptr   Pointer to fifo SubGroup structure.
 * \param[in]  nfifos   Number of fifos to be deallocated from the group
 *                      (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids  Pointer to an array of nfifos int32_t's where
 *                      the list of fifos to be deallocated is provided.
 *                      Each int32_t is the fifo number within the SubGroup
 *                      (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All fifos were deallocated.
 * \retval  errno  Unsuccessful.
 *
 *
 */
int32_t Kernel_DeallocateRecFifos (MUSPI_RecFifoSubGroup_t * sg_ptr,
				   uint32_t                  nfifos,
				   uint32_t                * fifoids);


/**
 * \brief Deallocate Base Address Table
 *
 * Deallocate specified base address table entries in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param[in]  sg_ptr   Pointer to Base Address Table SubGroup structure.
 * \param[in]  nbatids  Number of IDs to be deallocated (1 to
 *                      BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).
 * \param[in]  batids   Pointer to an array of nfifos int32_t's where
 *                      the list of Base Address Table IDs to be deallocated
 *                      is provided.  Each int32_t is the Base Address Table ID
 *                      within the SubGroup
 *                      (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All IDs were deallocated.
 * \retval  errno  Unsuccessful.
 *
 * \see Kernel_QueryBaseAddressTable
 * \see Kernel_AllocateBaseAddressTable
 *                          
 */
int32_t Kernel_DeallocateBaseAddressTable (MUSPI_BaseAddressTableSubGroup_t *sg_ptr,
					   uint32_t                          nbatids,
					   uint32_t                         *batids);


/**
 * \brief Include platform-specific implementations of the above functions.
 */
#include "MU_impl.h"

/**
 * \brief Generic implementations.
 */

/**
 * \brief MU InjFifo Initialization 
 *
 * - For an allocated injection MU fifo, initialize its start, 
 *   head, tail, and end.  
 * - Initialize fifo size and free space and desc count
 *
 * \param[in]  sg_ptr       Pointer to fifo SubGroup structure.
 * \param[in]  fifoid       Id of the fifo to be initialized
 *                          (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  memregion    Memory region of the injection fifo.
 * \param[in]  startoffset  The offset in the memory region where the start, 
 *                          head and tail pointers are initialized to.
 *                          The fifo start address must be aligned on a 64-byte
 *                          boundary.
 * \param[in]  size_1       Size of the injection fifo minus 1, in bytes.
 *                          - size_1+1 must be a multiple of 64 bytes.
 *                          - size_1+1 must be at least
 *                            - BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES bytes for a
 *                              non-remote-get fifo.
 *                            - BGQ_MU_MIN_RGETFIFO_SIZE_IN_BYTES bytes for a
 *                              remote-get fifo.
 *
 * \retval   0  Successful.
 * \retval  errno  Unsuccessful.  Error checks include
 *              - The fifo's start address is aligned on a 64-byte boundary.
 *              - The fifo's size is a multiple of 64 bytes.
 *              - The fifo's size is at least BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES
 *                bytes.
 *              Note:  The minimum size of a remote get fifo is not enforced on
 *                     this interface.  Be careful to meet the minimum size
 *                     requirements documented in the size_1 parameter.
 *
 */
__INLINE__
int32_t Kernel_InjFifoInit (MUSPI_InjFifoSubGroup_t * sg_ptr,
			    uint32_t                  fifoid,
			    Kernel_MemoryRegion_t   * memregion,
			    uint64_t                  startoffset, 
			    uint64_t                  size_1)
{
  int32_t rc=0;
  void * va_start;
  uint64_t pa_start;

  MUSPI_assert( sg_ptr != NULL );
  MUSPI_assert( fifoid < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP );

  va_start = (void*)((uint64_t)memregion->BaseVa + startoffset);
  pa_start = (uint64_t)memregion->BasePa + startoffset;

  /* Start pointers must be 64-byte aligned.
   * Size must be at least the minimum.
   * Size must be a multiple of 64 bytes.
   */
  MUSPI_assert((((uint64_t) va_start) & 0xFFFFFFFFFFFFFFC0ULL) == (uint64_t) va_start);
  MUSPI_assert( (size_1+1) >= BGQ_MU_MIN_INJFIFO_SIZE_IN_BYTES );
  MUSPI_assert( ( (size_1+1) & 0x3F ) == 0 );
  MUSPI_assert( va_start < (void*)((uint64_t)(memregion->BaseVa) + memregion->Bytes ));

  sg_ptr->_injfifos[fifoid]._fifo.va_start = va_start;
  sg_ptr->_injfifos[fifoid]._fifo.pa_start =
    sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_start = pa_start;

  sg_ptr->_injfifos[fifoid]._fifo.va_head = va_start;
  sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_head = pa_start;

  /* Initialize tail to the input va_head pointer*/
  sg_ptr->_injfifos[fifoid]._fifo.va_tail = va_start;
  sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_tail = pa_start;

  sg_ptr->_injfifos[fifoid]._fifo.size_1 = size_1;
  sg_ptr->_injfifos[fifoid]._fifo.hwfifo->size_1 = size_1;

  sg_ptr->_injfifos[fifoid]._fifo.va_end = (void*)((uint64_t)va_start + size_1 + 1);

  sg_ptr->_injfifos[fifoid].freeSpace = sg_ptr->_injfifos[fifoid].hw_injfifo->freeSpace;

  /* printf("Stored 0x%llx at %p for start\n       0x%llx at %p for head\n       0x%llx at %p for tail\n       0x%llx at %p for size, freespace=0x%lx\n", */
  /* 	 (unsigned long long int)pa_start, */
  /* 	 &(sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_start), */
  /* 	 (unsigned long long int)pa_start, */
  /* 	 &(sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_head), */
  /* 	 (unsigned long long int)pa_start, */
  /* 	 &(sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_tail), */
  /* 	 (unsigned long long int)size_1, */
  /* 	 &(sg_ptr->_injfifos[fifoid]._fifo.hwfifo->size_1), */
  /* 	 sg_ptr->_injfifos[fifoid].freeSpace); */

  return rc;
}


/**
 * \brief MU RecFifo Initialization 
 *
 * - For an allocated reception fifo, initialize its start, head, tail, and 
 *   end.  
 *
 * \param[in]  sg_ptr       Pointer to fifo SubGroup structure.
 * \param[in]  fifoid       Id of the fifo to be initialized
 *                          (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  memregion    Memory region of the reception fifo.
 * \param[in]  startoffset  The offset in the memory region where the start, 
 *                          head and tail pointers are initialized to.
 *                          The fifo start address must be aligned on a 32-byte
 *                          boundary.
 * \param[in]  size_1       Size of the reception fifo minus 1, in bytes.
 *                          - size_1+1 must be a multiple of 32 bytes.
 *                          - size_1+1 must be at least
 *                            BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES bytes.
 *
 * \retval   0  Successful.
 * \retval  errno  Unsuccessful.  Error checks include
 *              - The fifo's start address is aligned on a 32-byte boundary.
 *              - The fifo's size is a multiple of 32 bytes.
 *              - The fifo's size is at least BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES
 *                bytes.
 *
 * Doesn't need to be a syscall...it only updates MU SRAM fields.
 *
 */
__INLINE__
int32_t Kernel_RecFifoInit (MUSPI_RecFifoSubGroup_t * sg_ptr,
			    uint32_t                  fifoid,
			    Kernel_MemoryRegion_t   * memregion,
			    uint64_t                  startoffset, 
			    uint64_t                  size_1)
{
  int32_t rc=0;

  void * va_start; 
  uint64_t pa_start; 

  MUSPI_assert( sg_ptr != NULL );
  MUSPI_assert( fifoid < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP );

  va_start = (void*)((uint64_t)memregion->BaseVa + startoffset);
  pa_start = (uint64_t)memregion->BasePa + startoffset;
  
  /* Start pointers must be 32-byte aligned.
   * Size must be at least the minimum size.
   * Size must be a multiple of 32 bytes.
   */
  MUSPI_assert( (((uint64_t) va_start) & 0xFFFFFFFFFFFFFFE0ULL)  == (uint64_t) va_start );
  MUSPI_assert( (size_1+1) >= BGQ_MU_MIN_RECFIFO_SIZE_IN_BYTES );
  MUSPI_assert( ( (size_1+1) & 0x1F ) == 0 );
  
  sg_ptr->_recfifos[fifoid]._fifo.va_start = va_start;
  sg_ptr->_recfifos[fifoid]._fifo.pa_start = 
    sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start = pa_start;
  
  sg_ptr->_recfifos[fifoid]._fifo.va_head = va_start;
  sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_head = pa_start;
  
  /* Initialize tail to the input va_head pointer*/
  sg_ptr->_recfifos[fifoid]._fifo.va_tail = va_start;
  sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_tail = pa_start;
  
  sg_ptr->_recfifos[fifoid]._fifo.size_1 = size_1;
  sg_ptr->_recfifos[fifoid]._fifo.hwfifo->size_1 = size_1;  
  
  sg_ptr->_recfifos[fifoid]._fifo.va_end = (void*)((uint64_t)va_start + size_1 + 1); 

/*   printf("Stored 0x%llx at %p for start\n       0x%llx at %p for head\n       0x%llx at %p for tail\n       0x%llx at %p for size\n", */
/* 	 (unsigned long long int)pa_start, */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start), */
/* 	 (unsigned long long int)pa_start, */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_head), */
/* 	 (unsigned long long int)pa_start, */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_tail), */
/* 	 (unsigned long long int)size_1, */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->size_1)); */
/*   printf("Read   0x%llx at %p for start\n       0x%llx at %p for head\n       0x%llx at %p for tail\n       0x%llx at %p for size\n", */
/* 	 (unsigned long long int)(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start), */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start), */
/* 	 (unsigned long long int)(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_head), */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_head), */
/* 	 (unsigned long long int)(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_tail), */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_tail), */
/* 	 (unsigned long long int)(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->size_1), */
/* 	 &(sg_ptr->_recfifos[fifoid]._fifo.hwfifo->size_1)); */

/*    printf ("Initializing reception fifo hwfifo1 = %llx, pa_start = %llx, pa_head = %llx, pa_tail=%llx, size %x\n",  */
/*     sg_ptr->_recfifos[fifoid]._fifo.hwfifo, */
/*     sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start, */
/*     sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_head, */
/*     sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_tail, */
/*     sg_ptr->_recfifos[fifoid]._fifo.hwfifo->size_1); */

  return rc;
}


/**
 * \brief Activate MU InjFifo 
 *
 * Activate a MU injection fifo, given a fifo SubGroup and fifo id.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with activations and deactivations
 * and deallocations of injection fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]  nfifos    Number of fifos to be activated (or deactivated)
 *                       (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids   Ids of the fifos within the group
 *                       (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in]  activate  Flag indicating whether to activate or deactivate:
 *                       - KERNEL_INJ_FIFO_ACTIVATE
 *                       - KERNEL_INJ_FIFO_DEACTIVATE
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_InjFifoActivate (MUSPI_InjFifoSubGroup_t * sg_ptr,
				uint32_t                  nfifos,
				uint32_t                * fifoids,
				uint32_t                  activate)
{
  int32_t rc=0;
  uint32_t i;
  uint64_t bits = 0; 
  int gfid, sgid;
  
  MUSPI_assert ( (activate == KERNEL_INJ_FIFO_ACTIVATE) ||
		 (activate == KERNEL_INJ_FIFO_DEACTIVATE) );

  for (i = 0; i < nfifos; i ++)
    MUSPI_assert (fifoids[i] < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);
  
  sgid = sg_ptr->subgroupid % BGQ_MU_NUM_FIFO_SUBGROUPS;
  
  for (i = 0; i < nfifos; i ++) {
    gfid = fifoids[i] + sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP;
    bits |= ( 0x0000000000000001ULL << (31 - gfid) );
  }

  if ( activate == KERNEL_INJ_FIFO_ACTIVATE )
  {
    sg_ptr->groupStatus->enable  = bits;
/*     printf("Storing 0x%llx at %p for enable\n", */
/* 	   (unsigned long long int)(bit), */
/* 	   &(sg_ptr->groupStatus->enable)); */
  }
  else
    sg_ptr->groupStatus->disable = bits;
  
  return rc;
}


/**
 * \brief Get the Injection Fifo Thresholds
 *
 * Get the normal and remote get Injection Fifo thresholds.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in an injection fifo becomes greater than the threshold value, 
 * regardless of whether the the fifo is activated.  The interrupt is raised if the 
 * fifo has been configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out] normalThreshold     Pointer to the interrupt threshold for all "normal"
 *                                 injection fifos on the node.  This value is in
 *                                 units of descriptors (64-byte units).
 *                                 If NULL, this threshold is not retured.
 * \param[out] remoteGetThreshold  Pointer to the interrupt threshold for all "remote get"
 *                                 injection fifos on the node.  This
 *                                 value is in units of descriptors (64-byte 
 *                                 units).  If NULL, this threshold is not returned.
 *
 * \retval  0  Successful.  The threshold values are returned in the normalThreshold
 *                          and remoteGetThreshold parameters.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoInterrupts
 * \see Kernel_ConfigureInjFifoThresholds
 */
__INLINE__
int32_t Kernel_GetInjFifoThresholds( uint64_t * normalThreshold,
				     uint64_t * remoteGetThreshold )
{
  if ( normalThreshold )
    {
      if ( Kernel_GetPrivilegeMode() == 0 )
	*normalThreshold = DCRReadUser(MU_DCR(IMU_THOLD));
      else
	*normalThreshold = DCRReadPriv(MU_DCR(IMU_THOLD));
/*       muDCR_threshold_ptr = */
/* 	(uint64_t *)(BGQ_MU_DCR_iDMA_THRESHOLD_OFFSET); */
/*       *normalThreshold = *(muDCR_threshold_ptr); */
    }

  if ( remoteGetThreshold )
    {
      if ( Kernel_GetPrivilegeMode() == 0 )
	*remoteGetThreshold = DCRReadUser(MU_DCR(RGET_THOLD));
      else
	*remoteGetThreshold = DCRReadPriv(MU_DCR(RGET_THOLD));
/*       muDCR_threshold_ptr = */
/* 	(uint64_t *)(BGQ_MU_DCR_iDMA_RGET_THRESHOLD_OFFSET); */
/*       *remoteGetThreshold = *(muDCR_threshold_ptr); */
    }
  return 0;
}


/**
 * \brief Get the Reception Fifo Threshold
 *
 * Configure the Reception Fifo threshold.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in a reception fifo drops below this value, regardless of whether
 * the fifo is enabled.  The interrupt is raised if the fifo has been
 * configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out] threshold     The interrupt threshold for all reception
 *                           fifos on the node.  This value is the number of 
 *                           32-byte packet chunks.
 *
 * \retval  0  Successful.  The threshold value is returned in the threshold
 *                          parameter.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoInterrupts
 * \see Kernel_ConfigureRecFifoThreshold
 */
__INLINE__
int32_t Kernel_GetRecFifoThreshold( uint64_t * threshold )
{
  if ( Kernel_GetPrivilegeMode() == 0 )
    *threshold = DCRReadUser(MU_DCR(RMU_THOLD));
  else
    *threshold = DCRReadPriv(MU_DCR(RMU_THOLD));

  return 0;
}


/**
 * \brief Tell the ND to drop packets that attempt to go over a link
 *
 * This is used for separating sub-block user partitions.  It can prevent
 * user (and/or system) packets from crossing from one sub-block job into another.
 * This is not meant to be called from user space
 *
 * \param[in]  link_id      The id of the link on which to drop packets, 
 * \param[in]  drop_user    If non-zero, drop user packets
 * \param[in]  drop_system  If non-zero, drop system packets
 *
 */
__INLINE__
int32_t Kernel_DropPacketsOnLink (uint32_t  link_id,
				  uint32_t  drop_user,
				  uint32_t  drop_system);


/**
 * \brief Enable Sender Only and tell the network to drop packets that attempt to go over a link
 *
 * This can be used at the edge of mesh to detect and drop packets that request senders 
 * at the mesh edge
 * This is not meant to be called from user space
 *
 * \param[in]  link_id      The id of the link on which to drop packets, 
 * \param[in]  drop_user    If non-zero, drop user packets
 * \param[in]  drop_system  If non-zero, drop system packets
 *
 */
__INLINE__
int32_t Kernel_EnableSenderAndDropPacketsOnLink (uint32_t  link_id,
						 uint32_t  drop_user,
						 uint32_t  drop_system);



/**
 * \brief Gets the Expected number of Network Tokens that are programmed for a given link and VC
 *
 * if the return code is 0, token_ptr contains valid data
 * if non-zero, there is an error in either the input or the programming of the DCR
 * Warning, for vc !=0, the check is only valid on DD2 or higher
 * \param[in]  link_id      The id of the link
 * \param[in]  vc           The virtual channel
 * \param[out] token_ptr    pointer to the number of tokens for this link and VC
 *
 */
__INLINE__
int32_t Kernel_GetNDExpectedTokens (uint32_t  link_id,
				    uint32_t  vc,
				    uint32_t  *token_ptr);

/**
 * \brief Constructs the expected TOKENS DCR based on the token reduction DCRS
 *
 * if the return code is 0, token_ptr contains valid data
 * if non-zero, there is an error in either the input or the programming of the DCR
 * Warning, for vc !=0, the check is only valid on DD2 or higher
 * \param[in]  link_id      The id of the link
 * \param[out] token_ptr    pointer to the expected TOKENS DCR dcrs for this link
 *
 */
__INLINE__
int32_t Kernel_GetNDExpectedTokensDCR (uint32_t  link_id,
				       uint64_t  *token_ptr);

  


/**
 * \brief Termination Check
 *
 * \retval  0  Successful.
 * \retval -1  Unsuccessful.
 */

int32_t Kernel_MU_TermCheck(void);

//! Special signal number indicating an MU/ND fatal interrupt due to software. Job will be terminated.
#define SIGMUNDFATAL  36 

//! Special signal number indicating an MU FIFO FULL condition. Non-fatal informational signal.
#define SIGMUFIFOFULL 37 

__END_DECLS

#endif /* _KERNEL_MU_H_ */
