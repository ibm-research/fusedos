/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* (C) Copyright IBM Corp.  2008, 2008                              */
/* IBM CPL License                                                  */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#ifndef	_KERNEL_KLINUX_LOCATION_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_KLINUX_LOCATION_IMPL_H_

/**
 * \file klinux/location_impl.h
 *
 * \brief C Header File containing location SPI
 *        implementations for the Linux kernel.
 *
 */
#include "kernel_impl.h"

__BEGIN_DECLS

/*! \brief Returns the physical processor ID of the running A2 core.
 *
 * \return Physical processor ID (0-16)
 */
__INLINE__ uint32_t Kernel_ProcessorCoreID( void )
{
    return ((mfspr(SPRN_PIR) >> 2)&0x1f);
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
    return ((mfspr(SPRN_PIR))&0x3);
}

/*!
 * \deprecated
 */
__INLINE__ uint32_t Kernel_PhysicalHWThreadID( void ) {
}

/*! \brief Returns the physical thread index.
 *
 * \return Physical hardware thread ID (0-67)
 */
__INLINE__ uint32_t Kernel_ProcessorID( void )
{
    return ((mfspr(SPRN_PIR))&0x7f);
}

/*!
 * \deprecated
 */
__INLINE__ uint32_t Kernel_PhysicalHWThreadIndex( void ) {
  return Kernel_ProcessorID();
}

__END_DECLS


#endif /* _KERNEL_KLINUX_LOCATION_IMPL_H_ */
