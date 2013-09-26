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

// Includes
#include "Kernel.h" 
#include <sched.h>
#include "../cl_debug.h"

uint64_t sc_sched_getaffinity(SYSCALL_FCN_ARGS)
{
    int  tid   = (int)r3;
    size_t cpumasksize = (size_t)r4;
    cpu_set_t *cpu_mask = (cpu_set_t*)r5;
    
    // Get the Kthread associated with this tid. Function will return a NULL if this is an invalid tid
    KThread_t* targetKThread = GetKThreadFromTid(tid);

    // Did we find a valid KThread
    if (!targetKThread)
    {
        return CNK_RC_FAILURE(ESRCH); // no corresponding tid in this process was found
    }
    
    // Find the hardware thread index associated with the target kthread
    unsigned int hwthread_index = targetKThread->ProcessorID;
#if defined __FUSEDOS__
	CL_DEBUG("sched_getaffinity for TID %d, hwthread_index %d\n", tid, hwthread_index);
#endif
    
    // Validate the user-supplied address
    if (!VMM_IsAppAddress(cpu_mask, cpumasksize))
    {
        return CNK_RC_FAILURE(EFAULT); // Supplied pointer to the CPU mask was not valid.
    }

    // Initialize the mask bits to zero
    CPU_ZERO_S(cpumasksize, cpu_mask);
    // Set the index value into the supplied cpu mask object
    CPU_SET_S(hwthread_index, cpumasksize, cpu_mask);

    // Return the size in bytes of the kernel's cpu mask. glibc will zero all bits in the data after the kernel mask and up to
    // the end of the size of cpu_set_t  
    return CNK_RC_SUCCESS(cpumasksize);
}
