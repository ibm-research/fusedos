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

#ifndef	_KERNEL_CNK_LOCATION_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_LOCATION_IMPL_H_

/**
 * \file cnk/location_impl.h
 *
 * \brief C Header File containing Compute Node Kernel (CNK) location SPI
 *        implementations.
 *
 */
#include <stdint.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <cnk/include/SPRG_Usage.h>
#include "process_impl.h"

__BEGIN_DECLS

/*! \brief Returns the physical processor ID of the running A2 core.
 *
 * \return Physical processor ID (0-16)
 */
__INLINE__ uint32_t Kernel_ProcessorCoreID( void )
{
    return ((mfspr(SPRG_SPIinfo_ro) >> 2)&0x1f);
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
    return ((mfspr(SPRG_SPIinfo_ro))&0x3);
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
  return ((mfspr(SPRG_SPIinfo_ro))&0x7f);
}

/*!
 * \deprecated
 */

__INLINE__ uint32_t Kernel_PhysicalHWThreadIndex( void ) {
  return Kernel_ProcessorID();
}

/*! \brief returns the mapping of all the ranks into their torus coordinates
 *
 * 
 */
__INLINE__ uint32_t Kernel_RanksToCoords(size_t mapsize, BG_CoordinateMapping_t* map, uint64_t* numentries)
{
    return (uint32_t) CNK_SPI_SYSCALL_3(RANKS2COORDS, mapsize, map, numentries);
}


/*! \brief stores the definition of the sub-block within the supplied structure 
 * 
 * \return Indication on whether the current job occuppies a sub-block smaller than the entire block
 * \retval 1  This job occupies a sub-block that is smaller than the entire block
 * \retval 0  This job occupies the entire block
 *
 * 
 */
__INLINE__ uint32_t Kernel_JobCoords(BG_JobCoords_t* sblock)
{
    return (uint32_t) CNK_SPI_SYSCALL_1(JOBCOORDS, sblock);
}

/*! \brief returns the T coordinate associated with the current thread
 *
 * 
 */
__INLINE__ uint32_t Kernel_MyTcoord()
{
  uint64_t sprValue = mfspr( SPRG_SPIinfo_ro );
  uint8_t myProcessorID = (uint8_t)(mfspr(SPRG_SPIinfo_ro))&0x7f; // Processor ID
  uint32_t numprocs = (uint32_t)((sprValue >> SPRG_SPIinfo_NumProcs) & 0xFF); // number of processes in App/Job
  uint64_t threadMask = 0;
  uint32_t t;
  for (t=0; t<numprocs; t++)
  {
      threadMask = Kernel_ThreadMask(t);
      if (_BN(myProcessorID) & threadMask)
          return t;
  }
  // should not reach here.
  return 0; 
}

/*! \brief returns the Rank associated with the current process
 *
 * 
 */
__INLINE__ uint32_t Kernel_GetRank()
{
    return (uint32_t) CNK_SPI_SYSCALL_0(GETRANK);
}


__INLINE__ uint32_t Kernel_GetPVR(uint32_t* pvr)
{
    return (uint32_t)CNK_SPI_SYSCALL_1(GETPVR, pvr);
}


__INLINE__ uint32_t Kernel_GetMapping(size_t namesize, char* name, uint32_t* isFile)
{
    return (uint32_t)CNK_SPI_SYSCALL_3(GETMAPPING,namesize, name, isFile);
}


__END_DECLS


#endif /* _KERNEL_CNK_LOCATION_IMPL_H_ */
