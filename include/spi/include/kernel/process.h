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

#ifndef	_KERNEL_PROCESS_H_ /* Prevent multiple inclusion */
#define	_KERNEL_PROCESS_H_

/*!
 * \file process.h
 *
 * \brief C Header File containing SPI Addressing Inline Functions
 *
 */



#include "kernel_impl.h"

#include "hwi/include/common/compiler_support.h"
#include "firmware/include/personality.h"



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
 * \brief Return number of processes within the node
 *
 * Returns the number of processes that are associated with the currently
 * running application/job within the current physical node. 
 * 
 * \return  n  The number of processes
 *
 */

__INLINE__
uint32_t Kernel_ProcessCount();

/*!
 * \brief Return number of processors (hardware threads) within the current process
 *
 * Returns the number of processors that are associated with the currently
 * running process. 
 * 
 * \return  n  The number of processors
 *
 */

__INLINE__
uint32_t Kernel_ProcessorCount();


/*! \brief Returns a copy of the node's personality
 *
 * \param[out] personality Location of personality structure that will be filled in by Kernel_GetPersonality
 * \param[in]  size Size, in bytes, that was allocated to hold the personality structure
 * \return Error indication
 * \retval  0 Success
 * \retval -1 Invalid parameters
 */
__INLINE__ 
int Kernel_GetPersonality(Personality_t* personality, size_t size);

/*!
 * \brief Return a 64 bit thread mask indicating which hardware threads are associated with a process
 * 
 * \param[in]      t   The coordinate representing a process within a node. 
 *
 */

__INLINE__
uint64_t Kernel_ThreadMask(uint32_t t);


/*!
 * \brief Determine if this software thread is running on a hardware thread owned by a different process.
 * 
 * 
 * \return guest indicator
 * \retval 0 is not a guest thread 
 * \retval 1 is a guest thread 
 */

__INLINE__
uint64_t Kernel_IsGuestThread();


/*!
 * \brief Return the value of the AXUCR0 register.
 *
 * If the caller's process owns the whole core (that is, if the node is
 * configured with at most 16 processes), access to the stochastic rounding
 * control bits and to all the thread signalling control bits is provided.
 * Otherwise access is restricted to just the calling thread's signalling
 * control bits.
 * 
 * \return AXUCR0 value (masked to just those bits the caller is allowed to access)
 */
__INLINE__
uint64_t Kernel_GetAXUCR0(void);

/*!
 * \brief Set the value of the AXUCR0 register.
 *
 * If the caller's process owns the whole core (that is, if the node is
 * configured with at most 16 processes), changes to the stochastic rounding
 * control bits and to all the thread signalling control bits are allowed.
 * Otherwise changes are restricted to just the calling thread's signalling
 * control bits.
 *
 * \param[in]  value   The value to be set into AXUCR0.
 * 
 * \return Error indication
 * \retval  0 success
 * \retval  EINVAL if disallowed bits are included in value
 * \retval  ENOSYS if not supported
 *
 */

__INLINE__
uint64_t Kernel_SetAXUCR0(uint64_t value);

/*! \brief Returns the JobID of the currently active job
 *
 * \return job identifier
 */
__INLINE__
uint64_t Kernel_GetJobID();


/**
 * \brief Include implementations of the above functions.
 */
#include "process_impl.h"

__END_DECLS

#endif /* _KERNEL_PROCESS_H_ */
