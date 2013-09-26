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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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
#ifndef _CNK_SYSCALLS_H_ // Prevent multiple inclusion
#define _CNK_SYSCALLS_H_

#define CNK_ERRNO_RANGE 4096

#ifndef __ASSEMBLY__

#define SYSCALL_FCN_ARGS uint64_t r3, uint64_t r4, uint64_t r5, \
			 uint64_t r6, uint64_t r7, uint64_t r8

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
typedef uint64_t (*SysCall_Fcn_t)(SYSCALL_FCN_ARGS);

typedef struct 
{
        SysCall_Fcn_t call;
        bool          generateJMV;
} SyscallTable_t;
#endif                                                                                                                 // FUSEDOS

/**
 * \brief Return codes for the syscall handlers encode both failures (with
 * errno's) and successes (with slightly-limited 64-bit return values).
 *
 * FAILURE rc's are the last CNK_ERRNO_RANGE numbers in the unsigned 64-bit
 * integer range.  All other rc's indicate SUCCESS.
 *
 * A failure rc is constructed by adding (2^64 - CNK_ERRNO_RANGE) to an
 * errno.  With unsigned arithmetic, this addition is equivalent to
 * subtracting CNK_ERRNO_RANGE from the errno.
 *
 * Inline functions for constructing a success or failure rc, for extracting
 * the return value or errno from an rc, and for testing whether an rc
 * indicates success or failure are provided here.  Also provided (but not
 * inlined) are functions for formatting a return code for printing with %s.
 */

__INLINE__ uint64_t CNK_RC_SUCCESS(uint64_t value) {
    return value;
}
__INLINE__ uint64_t CNK_RC_FAILURE(int errnum) {
    return (((uint64_t) errnum) - CNK_ERRNO_RANGE);
}
__INLINE__ uint64_t CNK_RC_VALUE(uint64_t rc) {
    return rc;
}
__INLINE__ int CNK_RC_ERRNO(uint64_t rc) {
    return (int) (rc + CNK_ERRNO_RANGE);
}
__INLINE__ int CNK_RC_IS_SUCCESS(uint64_t rc) {
    return ((rc + CNK_ERRNO_RANGE) >= CNK_ERRNO_RANGE);
}
__INLINE__ int CNK_RC_IS_FAILURE(uint64_t rc) {
    return ((rc + CNK_ERRNO_RANGE) < CNK_ERRNO_RANGE);
}

/**
 * \brief Handlers for the SPI syscalls do not use the errno mechanism.
 * Return values are passed back unmodified.  An inline function is provided
 * here just for consistency with the standard system call handlers.
 * In general, a return value of 0 represents success, and a positive errno
 * value represents failure.  Small negative values (between -CNK_ERRNO_RANGE
 * and 0) are not allowed, as such values trigger the errno mechanism.
 */
__INLINE__ uint64_t CNK_RC_SPI(uint64_t value) {
    return value;
}

char *CNK_RC_STRING(uint64_t rc);
char *CNK_RC_STRING_HEX(uint64_t rc);

#define SYSCALL(name, spec) uint64_t sc_##name(SYSCALL_FCN_ARGS);
#include "syscalls/stdsyscalls.h"

#endif // !__ASSEMBLY__


#endif // Add nothing below this line
