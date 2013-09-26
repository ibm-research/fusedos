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
/* (C) Copyright IBM Corp.  2008, 2011                              */
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

#ifndef	_KERNEL_CNK_TRACE_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_TRACE_IMPL_H_

#include "cnk/include/SPI_syscalls.h"

__INLINE__
int Kernel_GetFlightLog(size_t bufferSize,  char* buffer)
{
    return (int) CNK_SPI_SYSCALL_2(GETFLIGHTLOG, bufferSize, buffer);
}

#endif /* _KERNEL_CNK_SPEC_IMPL_H_ */
