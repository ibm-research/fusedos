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

#ifndef	_DEBUG_CNK_THREAD_IMPL_H_ /* Prevent multiple inclusion */
#define	_DEBUG_CNK_THREAD_IMPL_H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <cnk/include/SPRG_Usage.h>
#include <cnk/include/SPI_syscalls.h>


__INLINE__
int Kernel_InjectRAWRAS(uint32_t message_id, size_t raslength, const uint64_t* rasdata)
{
    return (int) CNK_SPI_SYSCALL_3(INJECTRAWRAS, message_id, raslength, rasdata);
}

__INLINE__
int Kernel_InjectASCIIRAS(uint32_t message_id, const uint8_t* text)
{
    return (int) CNK_SPI_SYSCALL_2(INJECTASCIIRAS, message_id, text);
}

__INLINE__
int32_t Kernel_GetDebugReg(enum Kernel_Debug_Register debug_register, uint64_t *value_ptr)
{
   return (int) CNK_SPI_SYSCALL_2(GETDEBUGREG, debug_register, value_ptr);
}

__INLINE__
int32_t Kernel_SetDebugReg(enum Kernel_Debug_Register debug_register, uint64_t value)
{
   return (int) CNK_SPI_SYSCALL_2(SETDEBUGREG, debug_register, value);
}


#endif /* _KERNEL_CNK_THREAD_IMPL_H_ */
