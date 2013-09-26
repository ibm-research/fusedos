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

#ifndef	_KERNEL_FIRMWARE_DEBUG_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_FIRMWARE_DEBUG_IMPL_H_

__INLINE__
int32_t Kernel_GetDebugReg(enum Kernel_Debug_Register debug_register, uint64_t *value_ptr)
{
   return(ENOSYS);
}

__INLINE__
int32_t Kernel_SetDebugReg(enum Kernel_Debug_Register debug_register, uint64_t value)
{
   return(ENOSYS);
}


#endif /* _KERNEL_FIRMWARE_DEBUG_IMPL_H_ */
