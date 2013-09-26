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


#ifndef UPC_ATOMIC_H
#define UPC_ATOMIC_H

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC Atomic operations
 *
 * Atomic operations based on Larx & Stx operations used in tests
 * and UPCI operations.
 *
 */
//@}



#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/bqc/A2_inlines.h>


__BEGIN_DECLS

// Note: volatile attribute ensures value is not cached in a register.
//       Uses up a whole 64 byte cache line to keep reservations unique.
typedef ALIGN_L1D_CACHE volatile uint32_t UPC_Lock_t;
typedef ALIGN_L1D_CACHE volatile uint64_t UPC_Barrier_t;
typedef ALIGN_L1D_CACHE volatile int UPC_Atomic_Int_t;


/** UPC_Lock
 * Acquire lock ownership (blocking)
 * @param[in]  *pLock    pointer to shared memory lock to use that is common to all threads.
 */
void UPC_Lock(UPC_Lock_t *pLock);


/** UPC_Lock_woBlock
 * Acquire lock ownership or return
 * @param[in]  *pLock    pointer to shared memory lock to use that is common to all threads.
 * @return     =0  for success - lock aquired
 *             <0 for fail - lock owned by someone else.
 */
int UPC_Lock_woBlock(UPC_Lock_t *pLock);


/** UPC_Unlock
 * Free lock - assert if unlocking thread was not the same as locking thread.
 * @param[in]  *pLock    pointer to shared memory lock to use that is common to all threads.
 */
void UPC_Unlock(UPC_Lock_t *pLock);


/** UPC_Unlock_Any
 * Free lock - allow unlock by any thread even if not the same as thread which originally locked.
 *             requires care by caller to insure that there is only one thread doing the unlock.
 * @param[in]  *pLock    pointer to shared memory lock to use that is common to all threads.
 */
__INLINE__ void UPC_Unlock_Any(UPC_Lock_t *pLock) { *pLock = 0; mbar(); }





/** UPC_Barrier
 * Barrier the given target number of threads
 * @param[in]  *pLock       pointer to shared memory lock to use that is common to all threads.
 *                          The value must be initialized to zero before 1st call.
 *                          A successful completion leaves *pLock == 0.
 * @param[in]  num_threads  is number of threads in barrier.  All threads must pass
 *                          the same value for it to work.  There is no "master" thread.
 * @param[in]  timeout      Number of cycles to wait.  if 0, the timeout is 20000 cycles.
 *
 * @returns    number of threads which haven't completed. 0 on good, >0 on timeout.
 */
short UPC_Barrier(UPC_Barrier_t *pLock, short num_threads, uint64_t timeout);



//! LeaderLatch
//! Perform lock and barrier type latch based on status register bits.
//!
//! When a significiant status bit is enabled, LeaderLatch will return
//!    LLATCH_LEADER for the leading thread.  This leading thread is
//!    responsible to process the resource protected by the latch,
//!    clear the appropriate status bits, and
//!    hen call LeaderUnLatch() to free the latch.
//! "Follower" threads will block while the latch is owned.  When the
//!    the latch is freed, the return status will be LLATCH_CONTINUE
//! If the significant status bits are disabled, then LeaderLatch will
//!    return LLATCH_CONTINUE to all threads without blocking.
//!
//! @param[in]  *pLock    pointer to shared memory lock to use - common to all threads.
//! @param[in]  pstatus   pointer to status register to check
//! @param[in]  mask      mask of significant bits within status register
//! @return     LLATCH_LEADER or LLATCH_CONTINUE
#define LLATCH_CONTINUE 0
#define LLATCH_LEADER   1
short LeaderLatch(UPC_Barrier_t *pLock, volatile uint64_t *pstatus, uint64_t mask);



//! LeaderLatch2
//! Perform lock and barrier type latch based on 2 separate status regs - all bits
//!
//! @param[in]  *pLock    pointer to shared memory lock to use - common to all threads.
//! @param[in]  pstatus   pointer to status register to check (all bits)
//! @param[in]  pstatus2  pointer to 2nd status register to check (all bits)
//! @return     LLATCH_LEADER or LLATCH_CONTINUE
short LeaderLatch2(UPC_Barrier_t *pLock, volatile uint64_t *pstatus, volatile uint64_t *pstatus2);




//! LeaderUnLatch
//! free the latch, unblocking any "follower" threads.
void  LeaderUnLatch(UPC_Barrier_t *pLock);






//#define SPRN_CCR3                 (0x3FF)          // Core Configuration Register 3
#define  CCR3_EN_EEPRI             _BN(62)         //      Raise Priority when external interrupts are disabled
//#define  CCR3_SI                   _BN(63)         //      Single Instruction Mode - execute one instruction at a time

#define SPRN_PPR32                (0x382)          // Program Priority Register
#define  PPR32_PRI(x)              _B3(45,x)       //      thread priority


//! Set thread priority and return prior value.
__INLINE__ uint64_t Set_ThreadPriority_Medium( void )
{
    uint64_t ppr32 = mfspr(SPRN_PPR32);
    asm volatile ( "or 2,2,2" : : : "memory" );
    return ppr32;
}

__INLINE__ uint64_t Set_ThreadPriority_MediumLow( void )
{
    uint64_t ppr32 = mfspr(SPRN_PPR32);
    asm volatile ( "or 6,6,6" : : : "memory" );
    return ppr32;
}

__INLINE__ uint64_t Set_ThreadPriority_Low( void )
{
    uint64_t ppr32 = mfspr(SPRN_PPR32);
    asm volatile ( "or 1,1,1" : : : "memory" );
    return ppr32;
}

__INLINE__ void Restore_ThreadPriority( uint64_t pri )
{
    mtspr(SPRN_PPR32, pri);
}



__INLINE__ void Upci_Delay(uint64_t trgClks) {
    trgClks -= trgClks > 125 ? 125 : 0;   // fudge adjustment when above minimum overhead
    uint64_t btStart = GetTimeBase();
    uint64_t savpri = Set_ThreadPriority_Low();
    while ((GetTimeBase() - btStart) < trgClks);
    Restore_ThreadPriority(savpri);
}





/** UPC_Atomic_Incr
 * Atomically increment the value and return a unique increment result for this thread.
 * That is, when multiple threads call this routine,
 * the value returned may not be incremented by just one since the start.
 * UPC_Atomic_Incr returns the value that "this" thread successfully
 * stored, and the returned value ends up unique for all threads.
 *
 * @param[in]  *pAInt            pointer to shared memory integer (common to all threads).
 * @returns    incremented value, possibly from multiple threads.
 *
 */
__INLINE__ int UPC_Atomic_Incr(UPC_Atomic_Int_t *pAInt)
{
    int curValue;
    do {
        curValue = (int)LoadReserved32( (uint32_t*)pAInt );
        curValue++;
    } while (!StoreConditional32((uint32_t*)pAInt, (uint32_t)curValue));
    return curValue;
}



/** UPC_Atomic_Decr
 * Atomically decrement the value and return a unique increment result for this thread.
 * That is, when multiple threads call this routine,
 * the value returned may not be decremented by just one since the start.
 * UPC_Atomic_Decr returns the value that "this" thread successfully
 * stored, and the returned value ends up unique for all threads.
 *
 * @param[in]  *pAInt            pointer to shared memory integer (common to all threads).
 * @returns    decremented value, possibly from multiple threads.
 *
 */
__INLINE__ int UPC_Atomic_Decr(UPC_Atomic_Int_t *pAInt)
{
    int curValue;
    do {
        curValue = (int)LoadReserved32( (uint32_t*)pAInt );
        curValue--;
    } while (!StoreConditional32((uint32_t*)pAInt, (uint32_t)curValue));
    return curValue;
}



// Use in conjunction with UPC_Atomic_Incr to cause thread to wait
// until the value reaches the target value before returning.
__INLINE__ void UPC_Atomic_Wait(UPC_Atomic_Int_t *pAInt, int targ)
{
    int curValue;
    while ((curValue = *(int*)pAInt) < targ) {
        Upci_Delay(5);
    }
}



// Use in conjunction with UPC_Atomic_Incr/UPC_Atomic_Decr to cause thread to wait
// until the the value is 0.
__INLINE__ void UPC_Atomic_WaitTillZero(UPC_Atomic_Int_t *pAInt)
{
    int curValue = *(int*)pAInt;
    if (curValue != 0) {
        while ((curValue = *(int*)pAInt) != 0) {
            Upci_Delay(5);
        }
    }
}







__END_DECLS

#endif // Add nothing below this line
