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

#ifndef	_KERNEL_CNK_PROCESS_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_PROCESS_IMPL_H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <cnk/include/SPRG_Usage.h>
#include <cnk/include/SPI_syscalls.h>

/*!
 * \brief Return number of processes within the node
 *
 * Returns the number of processes that are associated with the currently
 * running application/job within the current physical node. 
 * 
 * \retval  n  The number of processes
 *
 */

__INLINE__
uint32_t Kernel_ProcessCount()
{
    // extract the process count info from the SPR
    uint64_t sprValue = mfspr( SPRG_SPIinfo_ro );
    uint32_t processCount = (uint32_t)((sprValue >> SPRG_SPIinfo_NumProcs) & 0xFF);
   // return the process count
    return processCount;
}

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
uint32_t Kernel_ProcessorCount()
{
    uint64_t sprValue = mfspr( SPRG_SPIinfo_ro );
    uint32_t numprocs = (uint32_t)((sprValue >> SPRG_SPIinfo_NumProcs) & 0xFF); // number of processes in App/Job
    uint32_t numCores = (uint32_t)((sprValue >> SPRG_SPIinfo_NumCores) & 0x0F) + 1; // number of cores in App/Job.
    return ((numCores*4)/numprocs);  
}




/*! \brief Returns a copy of the node's personality
 *
 * \param[out] personality Location of personality structure that will be filled in by Kernel_GetPersonality
 * \param[in]  size Size, in bytes, that was allocated to hold the personality structure
 * \return Error indication
 * \retval  0 Success
 * \retval -1 Invalid parameters
 */
__INLINE__ int Kernel_GetPersonality(Personality_t* personality, size_t size)
{
    return (int) CNK_SPI_SYSCALL_2(GET_PERSONALITY, personality, size);
}

/*!
 * \brief Return a 64 bit thread mask indicating which threads are associated with a process
 * 
 * \param[in]      T   The coordinate representing a process within a node.
 * 
 * The bit numbers in the returned 64 bit mask represent the ProcessorID value for each hardware thread
 * in the node. If a bit position = 1, then that hardware thread is owned by the process identified by the provided 
 * T coordinate. The T coordinate represents a sequential process indentification number ranging from 0 to BG_PROCESSESPERNODE-1
 *
 */

__INLINE__
uint64_t Kernel_ThreadMask(uint32_t t)
{
    uint64_t sprValue = mfspr( SPRG_SPIinfo_ro );
    uint64_t numprocs = (uint64_t)((sprValue >> SPRG_SPIinfo_NumProcs) & 0xFF); // number of processes in App/Job
    uint64_t numCores = (uint64_t)((sprValue >> SPRG_SPIinfo_NumCores) & 0x0F) + 1; // number of cores in App/Job.  
    uint64_t AppLeaderCore = (uint64_t)((sprValue >> SPRG_SPIinfo_AppLeaderCore) & 0x0F); // Normally 0. Can be non-zero for sub-block jobs
    uint64_t thds = (numCores*4)/numprocs; // number of threads in the Job on this node
    uint64_t thdcnt = thds;                
    uint64_t mask = 0;
    if (t >= numprocs) return 0; // T coordinate is out of range. Return mask with no bits set
    while (thdcnt--)
    {
        mask <<= 1;
        mask |= 1;
    }
    mask <<= (64 - thds - t * thds);
    mask >>= AppLeaderCore*4; // potential adjust for sub-node sub-block job              
    return mask;
}

__INLINE__
uint64_t Kernel_GetAXUCR0()
{
   return CNK_SPI_SYSCALL_0(GETAXUCR0);
}

__INLINE__
uint64_t Kernel_SetAXUCR0(uint64_t value)
{
   return CNK_SPI_SYSCALL_1(SETAXUCR0, value);
}

__INLINE__
uint64_t Kernel_GetJobID()
{
   return CNK_SPI_SYSCALL_0(GETJOBID);
}

__INLINE__
uint64_t Kernel_IsGuestThread()
{
    return(CNK_SPI_SYSCALL_0(ISGUESTTHREAD));
}

#endif /* _KERNEL_CNK_PROCESS_IMPL_H_ */
