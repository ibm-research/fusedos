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

#ifndef	_KERNEL_CNK_SPEC_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_SPEC_IMPL_H_

#include "cnk/include/SPI_syscalls.h"

__INLINE__
uint32_t Kernel_SetSpecState(size_t specStateSize, const SpecState_t* specStatePtr)
{
    return (uint32_t) CNK_SPI_SYSCALL_2(SETSPECSTATE, specStateSize, specStatePtr);
}


__INLINE__
uint32_t Kernel_EnterSpecJailMode(int isLongRunningSpeculation)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(ENTERJAILMODE, isLongRunningSpeculation);
}

__INLINE__
uint32_t Kernel_ExitSpecJailMode(void)
{
    return (uint32_t) CNK_SPI_SYSCALL_0(EXITJAILMODE);
}

__INLINE__
uint32_t Kernel_AllocateSpecDomain(unsigned int* domain)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(SPECALLOCATEDOMAIN, domain);
}

__INLINE__
uint32_t Kernel_SetNumSpecDomains(uint32_t new_domain_count)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(SETNUMSPECDOMAINS, new_domain_count);
}

__INLINE__
uint32_t Kernel_GetNumSpecDomains(uint32_t* current_domain_count)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(GETNUMSPECDOMAINS, current_domain_count);
}

__INLINE__
uint32_t Kernel_GetNumKernelSpecDomains(uint32_t* current_domain_count)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(GETNUMKERNELSPECDOMAINS, current_domain_count);
}

__INLINE__
uint32_t Kernel_SetSpecDomainMode(unsigned int domain, uint64_t domainmode)
{
    return (uint32_t) CNK_SPI_SYSCALL_2(SPECSETDOMAINMODE, domain, domainmode);
}


__INLINE__
uint32_t Kernel_SetL2SpecScrubRate(uint64_t scrub_rate)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(SETL2SCRUBRATE, scrub_rate);
}

__INLINE__
uint32_t Kernel_GetSpecConflictInfo(size_t specConflictSize, SpecConflictInfo_t* info)
{
    return (uint32_t) CNK_SPI_SYSCALL_2(GETSPECCONFLICTINFO, specConflictSize, info);
}

__INLINE__
uint32_t Kernel_ClearSpecConflictInfo(uint32_t l2slice)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(CLEARSPECCONFLICTINFO, l2slice);
}

__INLINE__
uint32_t Kernel_EnableFastSpeculationPaths()
{
    return (uint32_t) CNK_SPI_SYSCALL_0(ENABLEFASTSPECULATIONPATHS);
}

__INLINE__
uint32_t Kernel_GetSpeculationIDSelf(uint64_t* specid)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(GETSPECIDSELF, specid);
}


#endif /* _KERNEL_CNK_SPEC_IMPL_H_ */
