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

#ifndef	_KERNEL_CNK_THREAD_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_THREAD_IMPL_H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <cnk/include/SPRG_Usage.h>
#include <cnk/include/SPI_syscalls.h>

__INLINE__ uint32_t Kernel_SnoopNumThreads( void )
{
    return (((mfspr(SPRG_SPIinfo_ro))>>SPRG_SPIinfo_NumThds)&0xf);
}

__INLINE__ uint32_t Kernel_SnoopRunnable( void )
{
    return (((mfspr(SPRG_SPIinfo_ro))>>SPRG_SPIinfo_Runnable)&0xf);
}


__INLINE__
int32_t Kernel_CommThreadInstallHandler(int mu_int, Kernel_CommThreadHandler func, uint64_t parm1, uint64_t parm2, uint64_t parm3, uint64_t parm4 )
{
    return (int) CNK_SPI_SYSCALL_6(COMMTHREADINSTALLHANDLER,  mu_int, func, parm1, parm2, parm3, parm4);
}


__INLINE__
int32_t Kernel_CommThreadPoof(void)
{
    return (int) CNK_SPI_SYSCALL_0(COMMTHREADPOOF);
}


__INLINE__
int32_t Kernel_CommThreadEnableInterrupts(void)
{
   return (int) CNK_SPI_SYSCALL_0(COMMTHREADENABLEINTS);
}

__INLINE__
int32_t Kernel_CommThreadDisableInterrupts(void)
{
   return (int) CNK_SPI_SYSCALL_0(COMMTHREADDISABLEINTS);
}



#endif /* _KERNEL_CNK_THREAD_IMPL_H_ */
