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

#ifndef	_KERNEL_FIRMWARE_THREAD_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_FIRMWARE_THREAD_IMPL_H_

__INLINE__ uint32_t Kernel_SnoopNumThreads( void )
{
    return 0;
}

__INLINE__ uint32_t Kernel_SnoopRunnable( void )
{
    return 0;
}

__INLINE__
int32_t Kernel_CommThreadInstallHandler(int mu_int, Kernel_CommThreadHandler func, uint64_t parm1, uint64_t parm2, uint64_t parm3, uint64_t parm4 )
{
    return ENOSYS;
}

__INLINE__
int32_t Kernel_CommThreadPoof(void)
{
    return ENOSYS;
}

__INLINE__
int32_t Kernel_CommThreadEnableInterrupts(void)
{
    return ENOSYS;
}

__INLINE__
int32_t Kernel_CommThreadDisableInterrupts(void)
{
    return ENOSYS;
}



#endif /* _KERNEL_FIRMWARE_THREAD_IMPL_H_ */
