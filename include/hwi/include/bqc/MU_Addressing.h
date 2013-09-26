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

#ifndef	_MUHWI_ADDRESSING_H_ /* Prevent multiple inclusion */
#define	_MUHWI_ADDRESSING_H_


/**
 * \file MU_Addressing.h
 *
 * \brief C Header File containing Message Unit Addressing Definitions
 *
 * Message Unit addressing for direct puts uses the "base address table".
 * This is a set of base physical addresses that are used to locate a 
 * direct put reception buffer and the direct put counter.  A direct put
 * reception buffer is specified in the MU descriptor as a base address
 * table ID and an offset.  The physical address of the buffer is the
 * sum of the physical address in the specified base address table slot and
 * the put offset.  Similarly, the physical address of the counter is the 
 * sum of the physical address in the specified base address table slot and
 * the counter offset.
 *
 * There are 8 base address table slots per subgroup, which is 32 per group
 * and 544 total per node.  The base address table ID is specified in the
 * descriptor as a 10 bit value ranging from 0 to 543.  IDs 0 to 7 belong
 * to group 0, subgroup 0.  IDs 8 to 15 belong to group 0, subgroup 1.
 * IDs 536 to 543 belong to group 16, subgroup 3.
 *
 * Atomic addressing definitions are also defined here, used to construct
 * an MU-style atomic address.  The following describes the various
 * ways atomics are used with the MU:
 *
 * 1. In the direct put descriptor, the counter offset or the value in 
 *    the counter's base address table slot must be formatted as an MU
 *    atomic address.  The MU sums the value in the base address table slot
 *    with the counter offset to produce an MU atomic address that references
 *    an 8-byte counter.  When a packet arrives, the MU stores the negaive of
 *    the number of bytes in the packet to that address, decrementing the
 *    counter.  The atomic opcode must be one of the following:
 *    - MUHWI_ATOMIC_OPCODE_STORE_ADD
 *    - MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO
 *
 * 2. Send from an atomic field.  When the physical address of the send
 *    buffer in the descriptor is formatted as an MU atomic address (with
 *    a "load" type of opcode), the message length must be 8 bytes.  The
 *    MU performs the atomic operation on the 8-byte send buffer, sending
 *    the loaded value as the data.
 *
 * 3. Receive into an atomic field.  For a direct put, when the put offset or the
 *    value in the put offset's base address table slot is formatted as an
 *    MU atomic address, the message length must be 8 bytes.  The MU sums the
 *    value in the base address table slot with the put offset to produce an 
 *    MU atomic address that references an 8-byte field.  When a packet 
 *    arrives, The MU stores the received data to that field, performing the 
 *    atomic operation on that field.  The atomic opcode must be one of the
 *    "store" opcodes.
 */


#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>


__BEGIN_DECLS


/**
 * \brief Base Address Table Entry
 *
 * A base address table entry.  It contains one 37-bit physical address.
 *
 * BGQ_MU_DATA_COUNTER_BASE_ADDRESS_OFFSET(g,sg,c) is the address of the base
 * address table entry in group g, subgroup sg, slot c.  See MU_Macros.h.
 *
 */
typedef volatile uint64_t MUHWI_BaseAddress_t;


/**
 * \brief Base Address Table
 *
 * Base address table for a subgroup.  It contains an array of 
 * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP 37-bit physical addresses.
 *
 * BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET(g,sg) is the address of the
 * base address table in group g, subgroup sg.
 *
 */
typedef MUHWI_BaseAddress_t *MUHWI_BaseAddressTable_t;


/**
 * \brief MU Atomic Address Indicator
 *
 * A Message Unit Atomic Address consists of the following:
 *
 * - bits 0-26: Not used.  Should be zero.
 * - bit 27: Indicator that this is an MU Atomic address.  Set to 1.
 * - bits 28-63: 36-bit physical address of the 8-byte-aligned 8-byte atomic
 *   field.  Because the atomic field must be 8-byte-aligned, bits 61-63 must
 *   be zero, enabling the op code (see next field) to be stored there.
 * - bits 61-63: MU Atomic operation code.
 *
 * The MU Atomic Address Indicator (bit 27) is defined here.
 */
#define MUHWI_ATOMIC_ADDRESS_INDICATOR  0x0000001000000000ULL


/**
 * \brief MU Atomic Op Code:  Load
 *
 * Fetch the value from the atomic field.
 */
#define MUHWI_ATOMIC_OPCODE_LOAD        0x0000000000000000ULL


/**
 * \brief MU Atomic Op Code:  Load Clear
 *
 * Fetch the value from the atomic field and then clear the atomic field to zero.
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_CLEAR  0x0000000000000001ULL


/**
 * \brief MU Atomic Op Code:  Load Increment
 *
 * Load the value from the atomic field and then increment the atomic field.
 *
 * 0xFFFF FFFF FFFF FFFF rolls over to 0.  So when the atomic field is used as 
 * unsigned, +2^64-1 rolls over to 0.  Thanks to two's complement, the atomic
 * field can be used as signed or unsigned.  When using as signed, +2^63-1 rolls
 * over to -2^63.
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT  0x0000000000000002ULL


/**
 * \brief MU Atomic Op Code:  Load Decrement
 *
 * Load the value from the atomic field and then decrement the atomic field.
 *
 * 0 rolls over to 0xFFFF FFFF FFFF FFFF.  So when the atomic field is used as
 * unsigned, 0 rolls over to +2^64-1.  Thanks to two's complement, the atomic
 * field can be used as signed or unsigned.  When using as signed, -2^63 rolls
 * over to 2^63-1.
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT  0x0000000000000003ULL


/**
 * \brief MU Atomic Op Code:  Load Increment Bounded
 *
 * The counter is the address given and the boundary is the SUBSEQUENT address.
 * If the counter and the boundary values differ, increment the counter and
 * return the old value, else return 0x8000 0000 0000 0000.
 *
 * if (*ptrCounter==*(ptrCounter+1)){
 *   return 0x8000 0000 0000 0000;
 *   // +2^63 unsigned
 *   // -2^63 signed
 * } else {
 *   oldValue = *ptrCounter;
 *   ++*ptrCounter;
 *   return oldValue;
 * }
 *
 * The 8B counter and its 8B boundary efficiently support producer/consumer
 * queue/stack/deque with multiple producers and multiple consumers.  The
 * counter and boundary pair must be within a 32Byte line.  Rollover and
 * signed/unsigned software use are as for the 'load increment' instruction.
 *
 * On boundary, 0x8000 0000 0000 0000 is returned.
 *
 * So, unsigned use is also restricted to the upper value 2^63-1, instead of
 * the optimal 2^64-1.  This factor of 2 loss is not expected to be a problem
 * in practice. 
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED  0x0000000000000004ULL


/**
 * \brief MU Atomic Op Code:  Load Decrement Bounded
 *
 * The counter is the address given and the boundary is the PREVIOUS address. 
 * If the counter and the boundary values differ, decrement the counter and
 * return the old value, else return 0x8000 0000 0000 0000.
 *
 * if (*ptrCounter==*(ptrCounter-1)){
 *   return 0x8000 0000 0000 0000;
 *   // +2^63 unsigned
 *   // -2^63 signed
 * } else {
 *   oldValue = *ptrCounter;
 *   --*ptrCounter;
 *   return oldValue;
 * }
 *
 * The 8B counter and its 8B boundary efficiently support producer/consumer
 * queue/stack/deque with multiple producers and multiple consumers.  The
 * counter and boundary pair must be within a 32Byte line.  Rollover and
 * signed/unsigned software use are as for the 'load decrement' instruction.
 *
 * On boundary, 0x8000 0000 0000 0000 is returned.
 *
 * So, unsigned use is also restricted to the upper value 2^63-1, instead of
 * the optimal 2^64-1.  This factor of 2 loss is not expected to be a problem
 * in practice. 
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED  0x0000000000000005ULL


/**
 * \brief MU Atomic Op Code:  Load Increment If Equal
 *
 * The counter is the address given and the compare value is the SUBSEQUENT
 * address.  If the counter and the boundary values are equal, increment the
 * counter and return the old value, else return 0x8000 0000 0000 0000.
 *
 * if (*ptrCounter!=*(ptrCounter+1)){
 *   return 0x8000 0000 0000 0000;
 *   // +2^63 unsigned
 *   // -2^63 signed
 * } else {
 *   oldValue = *ptrCounter;
 *   ++*ptrCounter;
 *   return oldValue;
 * }
 *
 * The 8B counter and its 8B compare value efficiently support trylock
 * operations for mutex locks.  The counter and boundary pair must be within a
 * 32-byte line.
 *
 * Rollover and signed/unsigned software use are as for the 'load increment'
 * instruction.
 *
 * On mismatch, 0x8000 0000 0000 0000 is returned.
 *
 * So, unsigned use is also restricted to the upper value 2^63-1, instead of the
 * optimal 2^64-1.  This factor 2 loss is not expected to be a problem in
 * practice. 
 */
#define MUHWI_ATOMIC_OPCODE_LOAD_INCREMENT_IF_EQUAL  0x0000000000000006ULL


/**
 * \brief MU Atomic Op Code:  Store
 *
 * Store the value into the atomic field.
 */
#define MUHWI_ATOMIC_OPCODE_STORE        0x0000000000000000ULL


/**
 * \brief MU Atomic Op Code:  Store Twin
 *
 * Store 8B value to 8B address given and to the SUBSEQUENT 8B address, if these
 * two locations previously had the equal values.
 *
 * Used for fast deque implementations.
 * The address pair must be within a 32Byte line. 
 */
#define MUHWI_ATOMIC_OPCODE_STORE_TWIN   0x0000000000000001ULL


/**
 * \brief MU Atomic Op Code:  Store Add
 *
 * Add store value to storage.
 *
 * 0xFFFF FFFF FFFF FFFF and earlier rolls over to 0 and beyond. Vice versa in
 * the other direction.  So when software uses the counter as unsigned, +2^64-1
 * and earlier rolls over to 0 and beyond.
 * 
 * Thanks to two's complement, software can use the counter and 'store value' as
 * signed or unsigned.  When using as signed, and adding a positive store value,
 * then '+2^63-1 and earlier rolls over to -2^63 and beyond. Vice versa, when
 * adding a negative store value.
 */
#define MUHWI_ATOMIC_OPCODE_STORE_ADD    0x0000000000000002ULL


/**
 * \brief MU Atomic Op Code:  Store Add Coherence on Zero
 *
 * Same as "store add", but does not keep L1-caches coherent unless the result
 * of the store add is equal to zero.
 */
#define MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO    0x0000000000000003ULL


/**
 * \brief MU Atomic Op Code:  Store OR
 *
 * Logical OR value to storage.
 */
#define MUHWI_ATOMIC_OPCODE_STORE_OR    0x0000000000000004ULL


/**
 * \brief MU Atomic Op Code:  Store XOR
 *
 * Logical XOR value to storage.
 */
#define MUHWI_ATOMIC_OPCODE_STORE_XOR   0x0000000000000005ULL


/**
 * \brief MU Atomic Op Code:  Store Max Unsigned
 *
 * Store Max of value and storage, values are interpreted as unsigned binary.
 */
#define MUHWI_ATOMIC_OPCODE_STORE_MAX_UNSIGNED  0x0000000000000006ULL


/**
 * \brief MU Atomic Op Code:  Store Max Signed
 *
 * Store Max of value and storage, values are interpreted as 1b sign and 63b
 * absolute value.
 *
 * Allows Max of floating point numbers (NAN with 1b sign set).
 */
#define MUHWI_ATOMIC_OPCODE_STORE_MAX_SIGNED    0x0000000000000007ULL


__END_DECLS


#endif /* _MUHWI_ADDRESSING_H_  */
