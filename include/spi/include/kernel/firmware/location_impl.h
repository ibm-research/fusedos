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

#ifndef	_KERNEL_FIRMWARE_LOCATION_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_FIRMWARE_LOCATION_IMPL_H_

/**
 * \file firmware/location_impl.h
 *
 * \brief C Header File containing firmware location SPI
 *        implementations.
 *
 */
#include <stdint.h>
#include <hwi/include/bqc/A2_inlines.h>

__BEGIN_DECLS

/*! \brief Returns the physical processor ID of the running A2 core.
 *
 * \return Physical processor ID (0-16)
 */
__INLINE__ uint32_t Kernel_ProcessorCoreID( void )
{
    return ((mfspr(SPRN_SPRG7) >> 2)&0x1f);
}

/*!
 * \deprecated
 */
__INLINE__ uint32_t Kernel_PhysicalProcessorID( void ) {
  return Kernel_ProcessorCoreID();
}


/*! \brief Returns the physical processor ID of the running A2 core.
 *
 * \return Physical hardware thread ID (0-3)
 */
__INLINE__ uint32_t Kernel_ProcessorThreadID( void )
{
    return ((mfspr(SPRN_SPRG7))&0x3);
}

/*!
 * \deprecated
 */
__INLINE__ uint32_t Kernel_PhysicalHWThreadID( void ) {
  return Kernel_ProcessorThreadID();
}

/*! \brief Returns the physical thread index.
 *
 * \return Physical hardware thread ID (0-67)
 */
__INLINE__ uint32_t Kernel_ProcessorID( void )
{
    return ((mfspr(SPRN_SPRG7))&0x7f);
}

/*!
 * \deprecated
 */
__INLINE__ uint32_t Kernel_PhysicalHWThreadIndex( void ) {
  return Kernel_ProcessorID();
}

__END_DECLS


#endif /* _KERNEL_FIRMWARE_LOCATION_IMPL_H_ */
