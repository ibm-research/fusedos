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


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/futex.h>
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <unistd.h>                                                                                                    // FUSEDOS
#include <sys/syscall.h>                                                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#include "Kernel.h"

//#define COUNT_FUTEX_CALL                                                                                            // FUSEDOS
int futex_counts[64][6];                                                                                              // FUSEDOS
#ifdef COUNT_FUTEX_CALLS                                                                                              // FUSEDOS
int do_futex_counts = 1;                                                                                              // FUSEDOS
#else                                                                                                                 // FUSEDOS
int do_futex_counts = 0;                                                                                              // FUSEDOS
#endif                                                                                                                // FUSEDOS

uint64_t sc_futex( SYSCALL_FCN_ARGS )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t current_time = GetTimeBase();
    KThread_t *pKThr = GetMyKThread();
#endif                                                                                                                 // FUSEDOS
    Futex_t* ft_uaddr  = (Futex_t*)r3;
    uint32_t ft_op_and_flags = r4;
    // allow the private flag to be set since our implementation is already (process) private 
    uint32_t ft_op = ft_op_and_flags & FUTEX_CMD_MASK;
    int32_t ft_val     = r5;
    const struct timespec* ft_timeout = (const struct timespec*)r6;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    int32_t max2wake = (int32_t)r6;
#endif                                                                                                                 // FUSEDOS
    Futex_t* ft_uaddr2 = (Futex_t*) r7;
    int32_t ft_val3     = (int32_t)r8;
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t futex_timeout;
#endif                                                                                                                 // FUSEDOS

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    int processor_id = ProcessorID();                                                                                  // FUSEDOS
    TRACESYSCALL(                                                                                                      // FUSEDOS
                ("(I) %s[%d]: op(%d)=%s, uaddr=0x%08lx, val=%d timeout=0x%08lx *uaddr=%d uaddr2=0x%08lx "              // FUSEDOS
                 "val3=0x%08x.\n",                                                                                     // FUSEDOS
                 __func__, processor_id, ft_op_and_flags,                                                              // FUSEDOS
                 (ft_op == FUTEX_WAIT ? "WAIT" : (ft_op == FUTEX_WAKE ? "WAKE" : "Other")),                            // FUSEDOS
                 (uint64_t)ft_uaddr, ft_val, (uint64_t)ft_timeout, *ft_uaddr, (uint64_t)ft_uaddr2, ft_val3 ));         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#if 0
    int processor_id = ProcessorID();
    TRACESYSCALL(
                ("(I) %s[%d]: op(%d)=%s, uaddr=0x%08x, val=%d timeout=0x%08x *uaddr=%d uaddr2=0x%08x val3=0x%08x.\n",
                 __func__, processor_id, ft_op_and_flags,
                 (ft_op == FUTEX_WAIT ? "WAIT" : (ft_op == FUTEX_WAKE ? "WAKE" : "Other")),
                 (uint32_t)ft_uaddr, ft_val, (uint32_t)ft_timeout, *ft_uaddr, ft_uaddr2, ft_val3 ));
#endif
    if ( !VMM_IsAppAddress( (const void*)((Futex_t*)ft_uaddr), sizeof(Futex_t) ) )
    {
        return CNK_RC_FAILURE(EFAULT);
    }

    switch ( ft_op )
    {
        case FUTEX_WAIT:
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
            if (ft_timeout)
            {
                if (!VMM_IsAppAddress(ft_timeout, sizeof(const struct timespec)))
                {
                    return CNK_RC_FAILURE(EFAULT);
                }

                /* strace output in Linux reveals that timeout param. is relative time, not absolute.  Convert to abs. for scheduler */
                futex_timeout = ((uint64_t)ft_timeout->tv_sec * 1000000 + (uint64_t)ft_timeout->tv_nsec/1000) * GetPersonality()->Kernel_Config.FreqMHz + current_time;
	 }
            else
            {
                futex_timeout = 0;
            }
#endif                                                                                                                 // FUSEDOS

            //printf("[%d] before: futex@%p = %d\n", core, ft_uaddr, *((int*)ft_uaddr));
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#ifdef COUNT_FUTEX_CALLS                                                                                               // FUSEDOS
            futex_counts[this_spc()][ft_op]++;                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            return syscall(SYS_futex, r3, r4, r5, r6, r7, r8);                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
            return Futex_Wait( ft_op_and_flags, pKThr, ft_uaddr, ft_val, futex_timeout, current_time );
#endif                                                                                                                 // FUSEDOS
            break;

        case FUTEX_WAKE: // val is max number of threads to wake
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#ifdef COUNT_FUTEX_CALLS                                                                                               // FUSEDOS
            futex_counts[this_spc()][ft_op]++;                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            return syscall(SYS_futex, r3, r4, r5, r6, r7, r8);                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
            return Futex_Wake( ft_op_and_flags, ft_uaddr, ft_val, 0,  -1, -1);
#endif                                                                                                                 // FUSEDOS
            break;

        case FUTEX_WAKE_OP:
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#ifdef COUNT_FUTEX_CALLS                                                                                               // FUSEDOS
            futex_counts[this_spc()][ft_op]++;                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            return syscall(SYS_futex, r3, r4, r5, r6, r7, r8);                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
            return Futex_Wake( ft_op_and_flags, ft_uaddr, ft_val, ft_uaddr2, max2wake, ft_val3);
#endif                                                                                                                 // FUSEDOS
            break;

        case FUTEX_REQUEUE:
        case FUTEX_CMP_REQUEUE:
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#ifdef COUNT_FUTEX_CALLS                                                                                               // FUSEDOS
            futex_counts[this_spc()][ft_op]++;                                                                         // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            return syscall(SYS_futex, r3, r4, r5, r6, r7, r8);                                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
            return Futex_Wake( ft_op_and_flags, ft_uaddr, ft_val, ft_uaddr2, -1, ft_val3);
#endif                                                                                                                 // FUSEDOS
            break;

        default:
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            //printf("(W) %s: Unknown futex op: %d\n", __func__, ft_op);                                               // FUSEDOS
#else                                                                                                                  // FUSEDOS
            //printf("(W) %s[%d]: Unknown futex op: %d\n", __func__, core, ft_op);
#endif                                                                                                                 // FUSEDOS
            break;
    }

    return CNK_RC_FAILURE(ENOSYS);
}
