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
#include "spi/include/kernel/memory.h"

//! \brief  Implement the sc_GETMEMORYREGION system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SPI(0) or CNK_RC_SPI(errno).

uint64_t  sc_GETMEMORYREGION(SYSCALL_FCN_ARGS)
{
    Kernel_MemoryRegion_t *ptr = (Kernel_MemoryRegion_t *) r3;
    uint64_t vabase = (uint64_t) r4;
    //size_t requestsize = (size_t) r5;

    // Check for valid address.
    if ( !VMM_IsAppAddress(ptr, sizeof(Kernel_MemoryRegion_t))) 
    {
        return CNK_RC_SPI(EFAULT);
    }
    
    AppProcess_t* proc = GetMyProcess();
    int okay;
#define INRANGE(start, end, startp) if((vabase>=proc->start) && (vabase<=proc->end)) {     ptr->BaseVa = (void*)proc->start; ptr->BasePa = (void*)proc->startp; ptr->Bytes = proc->end - proc->start + 1; okay = 1; }
    okay = 0;
    INRANGE(Text_VStart, Text_VEnd, Text_PStart);
    INRANGE(Data_VStart, Data_VEnd, Data_PStart);
    INRANGE(Heap_VStart, Heap_VEnd, Heap_PStart);
    INRANGE(Shared_VStart, Shared_VEnd, Shared_PStart);
    if (!okay)
    {
	return CNK_RC_SPI(EINVAL);
    }
#undef INRANGE
    
    return CNK_RC_SPI(0);
}
