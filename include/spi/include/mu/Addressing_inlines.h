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

#ifndef	_MUSPI_ADDRESSING_INLINES_H_ /* Prevent multiple inclusion */
#define	_MUSPI_ADDRESSING_INLINES_H_


/**
 * \file Addressing_inlines.h
 *
 * \brief C Header File containing Message Unit SPI Addressing Inline
 *        Implementations
 *
 * - Base Address Table inlines are provided.
 * - Message Unit atomic address inlines are provided.
 *
 * \see Addressing.h
 *
 */

#include "kernel_impl.h"
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/MU_Addressing.h>
#include <spi/include/mu/Util.h>
#include <spi/include/mu/Addressing.h>


__BEGIN_DECLS


/**
 * \brief Set Base Address
 * 
 * Set the value of a base address table slot.
 *
 * \param[in]  bat  Pointer to a base address table SubGroup structure
 *                  that was returned by Kernel_AllocateBaseAddressTable().
 * \param[in]  entryid      The ID of the base address table entry to be set
 *                          (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *                          This is relative to the SubGroup being
 *                          managed by the specified bat structure.
 * \param[in]  baseAddress  The base address value to be set into the 
 *                          base address table.
 *
 * \retval  0         Success
 * \retval  non-zero  Failure.  Most likely because the specified entryid is not
 *                    being managed by the specified bat structure.
 */
__INLINE__
int32_t MUSPI_SetBaseAddress ( MUSPI_BaseAddressTableSubGroup_t *bat,
			       uint8_t                           entryid,
			       MUHWI_BaseAddress_t               baseAddress )
{
  MUSPI_assert ( bat != NULL );
  MUSPI_assert ( entryid < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
  MUSPI_assert ( bat->permissions & (_BN(entryid)>>56) );

  bat->hw_bat[entryid] = baseAddress; /* Set hardware BAT entry */

  bat->sw_bat[entryid] = baseAddress; /* Set software shadow BAT entry */

 /*
  printf ("Setting hw base address %p, software shadow %p to %p\n",  &bat->hw_bat[0][entryid], 
	  &bat->sw_bat[entryid], baseAddress);
*/
  return 0;  
}


/**
 * \brief Get Base Address
 * 
 * Get the value of a base address table slot.
 *
 * \param[in]  bat  Pointer to a base address table SubGroup structure
 *                  that was returned by Kernel_AllocateBaseAddressTable().
 * \param[in]  entryid      The ID of the base address table entry to be fetched
 *                          (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *                          This is relative to the SubGroup being
 *                          managed by the specified bat structure.
 * \retval  baseAddress  The base address value.
 */
__INLINE__
MUHWI_BaseAddress_t MUSPI_GetBaseAddress ( MUSPI_BaseAddressTableSubGroup_t *bat,
					   uint8_t                           entryid )
{
  MUSPI_assert ( bat != NULL );
  MUSPI_assert ( entryid < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
  MUSPI_assert ( bat->permissions & (_BN(entryid)>>56) );

  return ( bat->sw_bat[entryid] ); /* Return shadow value */
}


/**
 * \brief Get Offset From Base Address
 * 
 * Get the offset of a specified physical address from the base address in the
 * specified base address table slot.
 *
 * \param[in]  bat          Pointer to a base address table SubGroup structure
 *                          returned by Kernel_AllocateBaseAddressTable().
 * \param[in]  entryid      The ID of the base address table entry where the
 *                          base address is stored (0 to 
 *                          BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).  This is
 *                          relative to the SubGroup being managed by the
 *                          specified bat structure.
 * \param[in]  pa           Physical address whose offset from the base address
 *                          is to be returned.
 *
 * \retval  offset    Offset of the specified pa from the base address in the 
 *                    specified base address table entry.
 */
__INLINE__
uint64_t MUSPI_GetOffsetFromBaseAddress ( 
				    MUSPI_BaseAddressTableSubGroup_t *bat,
				    uint8_t                           entryid,
				    uint64_t                          pa )
{
  MUSPI_assert ( bat != NULL );
  MUSPI_assert ( entryid < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
  MUSPI_assert ( bat->permissions & (_BN(entryid)>>56) );

  return ( pa - MUSPI_GetBaseAddress ( bat, entryid ) );
}


/**
 * \brief Get Atomic Offset From Base Address
 * 
 * Given a physical address and an atomic operation code, return the offset of
 * the physical address from the base address in the specified base address
 * table slot.  The returned offset is encoded with the appropriate MU atomic
 * information such that the base address + returned offset = MU-style atomic
 * address used by the message unit to perform the atomic operation on the
 * 8-byte field referenced by the physical address.  The returned offset can be
 * stored into an offset field of a descriptor to be used as an offset from
 * the base address table entry.
 *
 * Note that if the base address table entry contains an atomic opcode, then
 * then the "op" parameter is ignored, and the opcode in the base address
 * table is used.  If the base address table entry contains the atomic
 * indicator, then the offset does not have the indicator, whereas if the base
 * address table entry does not contain the atomic indicator, then the offset
 * does.
 *
 * \param[in]  bat          Pointer to a base address table SubGroup structure
 *                          returned by Kernel_AllocateBaseAddressTable().
 * \param[in]  entryid      The ID of the base address table entry where the
 *                          base address is stored (0 to 
 *                          BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).  This is
 *                          relative to the SubGroup being managed by the
 *                          specified bat structure.  The address in this base
 *                          address table entry must be an 8-byte-aligned
 *                          address.  It may already contain an atomic opcode
 *                          and the MU atomic address indicator.
 * \param[in]  pa           Physical address whose offset from the base address
 *                          is to be returned.  This is the physical address of
 *                          the 8-byte-aligned 8-byte atomic field.  That is,
 *                          the least significant 3 bits of this pa must be
 *                          zero.
 * \param[in]  op           Atomic operation code.
 *                          - MUHWI_ATOMIC_OPCODE_LOAD
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_CLEAR
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED
 *                          - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_IF_EQUAL
 *                          - MUHWI_ATOMIC_OPCODE_STORE
 *                          - MUHWI_ATOMIC_OPCODE_STORE_TWIN
 *                          - MUHWI_ATOMIC_OPCODE_STORE_ADD
 *                          - MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO
 *                          - MUHWI_ATOMIC_OPCODE_STORE_OR
 *                          - MUHWI_ATOMIC_OPCODE_STORE_XOR
 *                          - MUHWI_ATOMIC_OPCODE_STORE_MAX_UNSIGNED
 *                          - MUHWI_ATOMIC_OPCODE_STORE_MAX_SIGNED
 *
 * \retval  offset    Offset of the specified pa from the base address in the 
 *                    specified base address table entry, with bits set as 
 *                    needed such that the base address table entry contents +
 *                    offset = MU-style atomic address.
 */
__INLINE__
uint64_t MUSPI_GetAtomicOffsetFromBaseAddress ( 
				    MUSPI_BaseAddressTableSubGroup_t *bat,
				    uint8_t                           entryid,
				    uint64_t                          pa,
				    uint64_t                          op )
{
  uint64_t            base_address_pa;
  uint64_t            offset;

  MUHWI_BaseAddress_t base_address;

  MUSPI_assert ( ( pa & 0x7 ) == 0 );
  MUSPI_assert ( op <= 7 );

  base_address    = MUSPI_GetBaseAddress ( bat, entryid );
  base_address_pa = base_address & 0xFFFFFFFF8; /* Mask out atomic indicator 
						   and opcode */

  /* Set atomic indicator into the offset, based on whether the base address
   * table entry has it set or not.
   */
  offset = ( ~(base_address & MUHWI_ATOMIC_ADDRESS_INDICATOR) ) &
           MUHWI_ATOMIC_ADDRESS_INDICATOR;
  
  /* If the base address table entry has an atomic opcode, then ignore the
   * caller's.
   */
  if ( base_address & 7 ) op = 0;

  /* Construct the offset */
  return ( offset |= ( (pa - base_address_pa) | op) );
}


/**
 * \brief Get an MU-style Atomic Address
 *
 * Given a physical address and an atomic operation code, this function returns
 * an MU-style atomic address that can be used by the message unit to perform
 * atomic operations on the 8-byte field referenced by the physical address.
 *
 * The returned address can be 
 * - set into a base address table entry, or 
 * - stored into an offset field of a descriptor to be used as an offset from a
 *   base address table entry whose value is zero.  THE RETURNED ADDRESS SHOULD
 *   NOT BE USED TO DIRECTLY ADDRESS THE 8-BYTE FIELD.  Rather, a "normal"
 *   address should be used.
 * - stored into the Pa_Payload field of a descriptor, such that the MU will
 *   perform the atomic operation on the 8-byte payload when the descriptor is
 *   injected into a FIFO.
 *
 * \param[in]  pa  Physical address of the 8-byte-aligned 8-byte atomic field.
 *                 That is, the least significant 3 bits of this pa must be
 *                 zero.
 * \param[in]  op  Atomic operation code.  
 *                 - MUHWI_ATOMIC_OPCODE_LOAD
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_CLEAR
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED
 *                 - MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_IF_EQUAL
 *                 - MUHWI_ATOMIC_OPCODE_STORE
 *                 - MUHWI_ATOMIC_OPCODE_STORE_TWIN
 *                 - MUHWI_ATOMIC_OPCODE_STORE_ADD
 *                 - MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO
 *                 - MUHWI_ATOMIC_OPCODE_STORE_OR
 *                 - MUHWI_ATOMIC_OPCODE_STORE_XOR
 *                 - MUHWI_ATOMIC_OPCODE_STORE_MAX_UNSIGNED
 *                 - MUHWI_ATOMIC_OPCODE_STORE_MAX_SIGNED
 *
 * \retval  MU-atomic-address  The MU-style atomic address is returned.
 */
__INLINE__
uint64_t MUSPI_GetAtomicAddress ( uint64_t pa,
				  uint64_t op )
{
  uint64_t MU_AtomicAddress;

  MUSPI_assert ( ( pa & 0x7 ) == 0 );
  MUSPI_assert ( op <= 7 );

  /* Set the 'atomic' indicator and the op code into the address */
  MU_AtomicAddress = MUHWI_ATOMIC_ADDRESS_INDICATOR | pa | op;

  return MU_AtomicAddress;
}


__END_DECLS


#endif /* _MUSPI_ADDRESSING_INLINES_H_  */
