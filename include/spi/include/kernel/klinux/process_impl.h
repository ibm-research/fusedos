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

#ifndef	_KERNEL_KLINUX_PROCESS_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_KLINUX_PROCESS_IMPL_H_

/**
 * \file klinux/process_impl.h
 *
 * \brief C Header File containing process SPI
 *        implementations for the Linux kernel.
 *
 */

#include <hwi/include/common/compiler_support.h>
#include <firmware/include/Firmware_Interface.h>


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
   // return the process count
    return 1;
}


/*! \brief Returns a copy of the node's personality
 *
 * \param[out] personality Location of personality structure that will be filled in by Kernel_GetPersonality
 * \param[in]  size Size, in bytes, that was allocated to hold the personality structure
 * \return Error indication
 * \retval  0 Success
 * \retval -1 Invalid parameters
 */
#ifndef __UPCFS__                                                                                                      // FUSEDOS
__INLINE__ int Kernel_GetPersonality(Personality_t* personality, size_t size)
{
   return bluegene_getPersonality(personality, size);
}
#endif                                                                                                                 // FUSEDOS

__INLINE__
uint64_t Kernel_GetAXUCR0()
{
   return ENOSYS;
}

__INLINE__
uint64_t Kernel_SetAXUCR0(uint64_t value)
{
   return ENOSYS;
}

__INLINE__
uint64_t Kernel_IsGuestThread()
{
    return 0;
}

#endif /* _KERNEL_KLINUX_PROCESS_IMPL_H_ */
