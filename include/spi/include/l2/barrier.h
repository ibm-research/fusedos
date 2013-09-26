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
#ifndef _L2_BARRIER_H_ // Prevent multiple inclusion.
#define _L2_BARRIER_H_

/*!
 * \file barrier.h
 *
 * \brief C Header file containing SPI L2-atomics-based barrier implementation
 *
 */

#include <hwi/include/common/compiler_support.h>

#include <sys/types.h>
#include <stdint.h>
#include <spi/include/l2/atomic.h>

/*!
 * \brief L2 Barrier
 *
 * The L2_Barrier_t structure.
 *
 * The structure contains two fields, start and count, which are assumed to
 * be initially zero.  Every thread increments count atomically when it
 * enters the barrier.  The "last" thread to enter bumps start up to the
 * current value of count, which releases the other threads waiting in the
 * barrier and initializes the barrier for the next round.
 *
 * No initialization or re-initialization is required between rounds.  The
 * fields grow monotonically but will not wrap for hundreds of years.
 *
 * The start and count fields occupy separate cache lines.  The count field
 * on which waiters spin is updated just once per round, so waiters will not
 * see unnecessary invalidates.
 *
 * This barrier implementation does not require any memory synchronization
 * operations and makes no memory consistency guarantees for its callers.
 *
 * The sets of threads participating in the barrier in different rounds may
 * be different.  However, if a thread joins the current round but did not
 * participate in the previous round, it must have been signaled one way or
 * another by a thread that DID participate in the previous round.  The
 * signaling must include memory synchronization so that the new thread
 * doesn't pick up stale values for the barrier fields.
 */
typedef struct {
    volatile __attribute__((aligned(L1D_CACHE_LINE_SIZE)))
	uint64_t start;  /*!< Thread count at start of current round. */
    volatile __attribute__((aligned(L1D_CACHE_LINE_SIZE)))
	uint64_t count;  /*!< Current thread count. */
} L2_Barrier_t;

#define L2_BARRIER_INITIALIZER {0,0}


__BEGIN_DECLS

/*!
 * \brief Join a barrier with a specified number of threads
 *
 * \param[in]  b           Pointer to the shared barrier structure.
 * \param[in]  numthreads  Number of threads expected to participate
 *                         in this barrier round.
 */

__INLINE__ void L2_Barrier(L2_Barrier_t *b, int numthreads)
{
    uint64_t target = b->start + numthreads;
    uint64_t current = L2_AtomicLoadIncrement(&b->count) + 1;

    if (current == target) {
	b->start = current;  // advance to next round
    } else {
	while (b->start < current);  // wait for advance to next round

	// NOTE: It's critical to compare b->start with current, NOT with
	//       target.  It's improbable, but target could possibly be based
	//       on a b->start that was already advanced to the next round.
    }
}

/*!
 * \brief Join a barrier with a specified number of threads, and return
 *        a unique (to this round) "ticket" for this thread
 *
 * \param[in]  b           Pointer to the shared barrier structure.
 * \param[in]  numthreads  Number of threads expected to participate
 *                         in this barrier round.
 */

__INLINE__ int L2_BarrierWithTicket(L2_Barrier_t *b, int numthreads)
{
    uint64_t start = b->start;
    ppc_msync();  // make sure we pick up the correct start for this round
    uint64_t count = L2_AtomicLoadIncrement(&b->count);

    uint64_t target = start + numthreads;
    uint64_t current = count + 1;

    if (current == target) {
	b->start = current;  // advance to next round
    } else {
	while (b->start < current);  // wait for advance to next round
    }

    return (int) (count - start);
}

__END_DECLS

#endif // Add nothing below this line.
