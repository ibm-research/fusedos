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
/* (C) Copyright IBM Corp.  2009, 2012                              */
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
#ifndef _L2_ATOMIC_H_ // Prevent multiple inclusion.
#define _L2_ATOMIC_H_

/*!
 * \file atomic.h
 *
 * \brief C Header File containing interfaces for the L2 atomic operations
 *
 */

#include <hwi/include/common/compiler_support.h>

__BEGIN_DECLS

#include <sys/types.h>
#include <stdint.h>
#include <spi/include/kernel/location.h>
#include <spi/include/kernel/memory.h>

#define L2_ATOMIC_OPCODE_LOAD				0
#define L2_ATOMIC_OPCODE_LOAD_CLEAR			1
#define L2_ATOMIC_OPCODE_LOAD_INCREMENT			2
#define L2_ATOMIC_OPCODE_LOAD_DECREMENT			3
#define L2_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED		4
#define L2_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED		5
#define L2_ATOMIC_OPCODE_LOAD_INCREMENT_IF_EQUAL	6

#define L2_ATOMIC_OPCODE_STORE				0
#define L2_ATOMIC_OPCODE_STORE_TWIN			1
#define L2_ATOMIC_OPCODE_STORE_ADD			2
#define L2_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO	3
#define L2_ATOMIC_OPCODE_STORE_OR			4
#define L2_ATOMIC_OPCODE_STORE_XOR			5
#define L2_ATOMIC_OPCODE_STORE_MAX			6
#define L2_ATOMIC_OPCODE_STORE_MAX_SIGN_VALUE		7



/*!
 * \brief Convert a pointer to the equivalent L2 atomic-op pointer suitable
 *        for the specified thread
 *
 * Internal utility function.  No argument checking.
 *
 * \param[in]  ptr  Storage location to be operated on.
 * \param[in]  op   Operation to be performed.
 * \param[in]  tid  Thread id to be embedded in pointer.
 */
__INLINE__ volatile uint64_t *__l2_op_tid_ptr(volatile uint64_t *ptr,
					      const uint64_t op,
					      const uint32_t tid)
{
    return (volatile uint64_t *) (((Kernel_L2AtomicsBaseAddress() +
				    ((((uint64_t) ptr) << 5) & ~0xfful)) |
				   (tid << 6)) +
				  (op << 3));
    // NOTE: the choices of '+' and '|' in the above expression, as well
    //       as choices for parentheses, were made in an empirical effort
    //       to get gcc to generate good code.  (No loads from the TOC.)
}

/*!
 * \brief Convert a pointer to the equivalent L2 atomic-op pointer suitable
 *        for the current thread
 *
 * Internal utility function.
 *
 * \param[in]  ptr  Storage location to be operated on.
 * \param[in]  op   Operation to be performed.
 */
__INLINE__ volatile uint64_t *__l2_op_ptr(volatile uint64_t *ptr,
					  const uint64_t op)
{
    return __l2_op_tid_ptr(ptr, op, Kernel_ProcessorThreadID());
}

/*!
 * \brief Perform a load-based L2 atomic operation
 *
 * Internal utility function.
 *
 * \param[in]  ptr  Storage location to be operated on.
 * \param[in]  op   Operation to be performed.
 */
__INLINE__ uint64_t __l2_load_op(volatile uint64_t *ptr, const uint64_t op)
{
    return *__l2_op_ptr(ptr, op);
}

/*!
 * \brief Perform a store-based L2 atomic operation
 *
 * Internal utility function.
 *
 * \param[in]  ptr  Storage location to be operated on.
 * \param[in]  op   Operation to be performed.
 */
__INLINE__ void __l2_store_op(volatile uint64_t *ptr, uint64_t value,
			      const uint64_t op)
{
    *__l2_op_ptr(ptr, op) = value;
}

/*!
 * \brief Perform an L2 atomic Load operation
 *
 * Fetch current value from storage location.
 *
 * \param[in]  ptr  Storage location to be fetched.
 */
__INLINE__ uint64_t L2_AtomicLoad(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD);
}

/*!
 * \brief Perform an L2 atomic Load Clear operation
 *
 * Fetch current value and clear storage location.
 *
 * \param[in]  ptr  Storage location to be fetched and cleared.
 */
__INLINE__ uint64_t L2_AtomicLoadClear(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_CLEAR);
}

/*!
 * \brief Perform an L2 atomic Load Increment operation
 *
 * Fetch current value and increment storage location.
 *
 * \param[in]  ptr  Storage location to be fetched and incremented.
 */
__INLINE__ uint64_t L2_AtomicLoadIncrement(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_INCREMENT);
}

/*!
 * \brief Perform an L2 atomic Load Decrement operation
 *
 * Fetch current value and decrement storage location.
 *
 * \param[in]  ptr  Storage location to be fetched and decremented.
 */
__INLINE__ uint64_t L2_AtomicLoadDecrement(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_DECREMENT);
}

/*!
 * \brief Perform an L2 atomic Load Increment Bounded operation
 *
 * Fetch current value and increment storage location, but only if the
 * current value does not match the value in the SUBSEQUENT storage location.
 * Return 0x8000 0000 0000 0000 if the values match.
 *
 * \param[in]  ptr  Storage location to be fetched and incremented.
 *                  ptr[0] and ptr[1] must be in the same 32-byte line.
 */
__INLINE__ uint64_t L2_AtomicLoadIncrementBounded(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED);
}

/*!
 * \brief Perform an L2 atomic Load Decrement Bounded operation
 *
 * Fetch current value and decrement storage location, but only if the
 * current value does not match the value in the PREVIOUS storage location.
 * Return 0x8000 0000 0000 0000 if the values match.
 *
 * \param[in]  ptr  Storage location to be fetched and decremented.
 *                  ptr[0] and ptr[-1] must be in the same 32-byte line.
 */
__INLINE__ uint64_t L2_AtomicLoadDecrementBounded(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_DECREMENT_BOUNDED);
}

/*!
 * \brief Perform an L2 atomic Load Increment If Equal operation
 *
 * Fetch current value and increment storage location, but only if the
 * current value matches the value in the SUBSEQUENT storage location.
 * Return 0x8000 0000 0000 0000 if the values do not match.
 *
 * \param[in]  ptr  Storage location to be fetched and incremented.
 *                  ptr[0] and ptr[1] must be in the same 32-byte line.
 */
__INLINE__ uint64_t L2_AtomicLoadIncrementIfEqual(volatile uint64_t *ptr)
{
    return __l2_load_op(ptr, L2_ATOMIC_OPCODE_LOAD_INCREMENT_IF_EQUAL);
}


/*!
 * \brief Perform an L2 atomic Store operation
 *
 * Store value to storage location.
 *
 * \param[in]  ptr    Storage location to be set.
 * \param[in]  value  Value to be stored.
 */
__INLINE__ void L2_AtomicStore(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE);
}

/*!
 * \brief Perform an L2 atomic Store Twin operation
 *
 * Store value to storage location and to the SUBSEQUENT storage location,
 * but only if the current values in the two locations match.
 *
 * \param[in]  ptr    Storage location to be set.
 * \param[in]  value  Value to be stored.
 */
__INLINE__ void L2_AtomicStoreTwin(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_TWIN);
}

/*!
 * \brief Perform an L2 atomic Store Add operation
 *
 * Add value to storage location.
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be added.
 */
__INLINE__ void L2_AtomicStoreAdd(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_ADD);
}

/*!
 * \brief Perform an L2 atomic Store Add -- Coherence On Zero operation
 *
 * Add value to storage location, but don't keep L1 caches coherent until
 * storage location reaches zero.
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be added.
 */
__INLINE__ void L2_AtomicStoreAddCoherenceOnZero(volatile uint64_t *ptr,
						 uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO);
}

/*!
 * \brief Perform an L2 atomic Store Or operation
 *
 * Or value into storage location.
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be or'd.
 */
__INLINE__ void L2_AtomicStoreOr(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_OR);
}

/*!
 * \brief Perform an L2 atomic Store Xor operation
 *
 * Xor value into storage location.
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be xor'd.
 */
__INLINE__ void L2_AtomicStoreXor(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_XOR);
}

/*!
 * \brief Perform an L2 atomic Store Max operation
 *
 * Store value to storage location if it is larger than the location's
 * current value.  Values are interpreted as 64-bit unsigned integers.
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be potentially stored.
 */
__INLINE__ void L2_AtomicStoreMax(volatile uint64_t *ptr, uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_MAX);
}

/*!
 * \brief Perform an L2 atomic Store Max Sign/Value operation
 *
 * Store value to storage location if it is larger than the location's
 * current value.  Values are interpreted as 1-bit sign and 63-bit absolute
 * value.  (Allows Max of floating point numbers.)
 *
 * \param[in]  ptr    Storage location to be modified.
 * \param[in]  value  Value to be potentially stored.
 */
__INLINE__ void L2_AtomicStoreMaxSignValue(volatile uint64_t *ptr,
					   uint64_t value)
{
    __l2_store_op(ptr, value, L2_ATOMIC_OPCODE_STORE_MAX_SIGN_VALUE);
}

/*!
 * \brief Public interface to convert a pointer to the equivalent L2
 *        atomic-op pointer suitable for the specified thread
 *
 * \param[in]  ptr  Storage location to be operated on.
 * \param[in]  op   Operation to be performed.
 * \param[in]  tid  Thread id to be embedded in pointer.
 */
__INLINE__ volatile uint64_t *L2_AtomicOpTidPointer(volatile uint64_t *ptr,
						    const uint64_t op,
						    const uint32_t tid)
{
    return __l2_op_tid_ptr(ptr, (op & 0x7), (tid & 0x3));
}


__END_DECLS

#endif // Add nothing below this line.
