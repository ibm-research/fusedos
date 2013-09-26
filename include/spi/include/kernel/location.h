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

#ifndef	_KERNEL_LOCATION_H_ /* Prevent multiple inclusion */
#define	_KERNEL_LOCATION_H_

/**
 * \file location.h
 *
 * \brief C Header File containing Kernel interfaces to provide location information
 *
 */

#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <spi/include/kernel/process.h>


///
/// \brief Blue Gene/Q coordinate structure
///
/// This structure takes 32-bits on any 32/64 bit system. The a, b, c,
/// and d fields are the same size and in the same location as the MU
/// descriptor structure. The thread/core fields are sized for 16 cores
/// with 4 hardware threads each, though the reserved bit can be stolen
/// for the 17th core if it needs a rank. The e dimension is sized to the
/// current node-layout maximum, though the MU hardware supports the full
/// 6 bits.
///
/// \see MUHWI_Destination_t
///
typedef struct
{
    uint32_t e        : 1; ///< Torus 'e' dimension, two nodes per node card
    uint32_t reserved : 1; ///< Reserved - possibly to identify the 17th core
    uint32_t a        : 6; ///< Torus 'a' dimension
    uint32_t b        : 6; ///< Torus 'b' dimension
    uint32_t c        : 6; ///< Torus 'c' dimension
    uint32_t d        : 6; ///< Torus 'd' dimension
    uint32_t t        : 6; ///< Hardware thread id, 4 threads per core
} BG_CoordinateMapping_t;


///
/// \brief Blue Gene/Q job coordinates structure
///  
/// This structure defines the coordinates of a job. The corner structure identifies the 
/// corner coordinate of the job. The shape structure defines the dimensions of the job 
/// relative to the corner. The value in the coordinate core field indicates the first 
/// core index in the node. The core value in the shape core field indicates
/// the number of cores of this node that are contained in this job. 
/// 

typedef struct
{
    struct 
    {
            uint8_t a   ; ///< Torus 'a' coordinate
            uint8_t b   ; ///< Torus 'b' coordinate
            uint8_t c   ; ///< Torus 'c' coordinate
            uint8_t d   ; ///< Torus 'd' coordinate
            uint8_t e   ; ///< Torus 'e' coordinate
            uint8_t core;
    } corner;
    struct 
    {
            uint8_t a   ; ///< Torus 'a' dimension
            uint8_t b   ; ///< Torus 'b' dimension
            uint8_t c   ; ///< Torus 'c' dimension
            uint8_t d   ; ///< Torus 'd' dimension
            uint8_t e   ; ///< Torus 'e' dimension
            uint8_t core;
    } shape;
    uint8_t isSubBlock; ///< true if job occupies a proper subset of its block
} BG_JobCoords_t;


__BEGIN_DECLS

/*! \brief Returns the processor core ID of the running A2 core.
 *  \return core ID (0-16)
 */

__INLINE__ uint32_t Kernel_ProcessorCoreID( void );


/*! \brief Returns the hardware thread ID relative to the running A2 core.
 *  \return Thread ID (0-3)
 */

__INLINE__ uint32_t Kernel_ProcessorThreadID( void );


/*! \brief Returns the thread ID relative to the node.
 * \return Hardware thread ID (0-67)
 */

__INLINE__ uint32_t Kernel_ProcessorID( void );


/*! \brief Returns the physical processor ID of the running A2 core.
 *  \deprecated Use Kernel_ProcessorCoreID.
 *  \return Physical processor ID (0-16)
 */

__INLINE__ uint32_t Kernel_PhysicalProcessorID( void );


/*! \brief Returns the physical processor ID of the running A2 core.
 *  \return Physical hardware thread ID (0-3)
 *  \deprecated Use Kernel_ProcessorThreadID()
 */

__INLINE__ uint32_t Kernel_PhysicalHWThreadID( void );


/*! \brief Returns the physical thread identifier
 * \return Physical hardware thread ID (0-67)
 * \deprecated use Kernel_ProcessorID() 
 */

__INLINE__ uint32_t Kernel_PhysicalHWThreadIndex( void );



/*! \brief returns the mapping of all the ranks into their torus coordinates
 *
 * 
 */
__INLINE__ uint32_t Kernel_RanksToCoords(size_t mapsize, BG_CoordinateMapping_t* map, uint64_t* numentries);


/*! \brief stores the job coordinates within the supplied structure 
 * 
 * \return Indication on whether the current job occuppies a sub-block smaller than the entire block
 * \retval 1  This job occupies a sub-block that is smaller than the entire block
 * \retval 0  This job occupies the entire block
 *
 * 
 */
__INLINE__ uint32_t Kernel_JobCoords(BG_JobCoords_t* sblock);

/*! \brief stores the processor version register into the supplied pointer
 *
 * \return Failure
 */
__INLINE__ uint32_t Kernel_GetPVR(uint32_t* pvr);


/**
 * \brief Include implementations of the above functions.
 */
#include "location_impl.h"

/**
 * \brief Generic implementations.
 */

/*! \brief Returns a block-unique identifier for the caller's hwthread
 *
 * \return Block-unique combination of torus coordinates and hwthread index
 */
__INLINE__ uint32_t Kernel_BlockThreadId( void )
{
    uint32_t id = 0;
    uint32_t rc;
    Personality_t pers;
    rc = Kernel_GetPersonality(&pers, sizeof(pers));
    if (rc == 0)
    {
	Personality_Networks_t *net = &pers.Network_Config;
	id = ((((((((net->Acoord
			* net->Bnodes) + net->Bcoord)
			* net->Cnodes) + net->Ccoord)
			* net->Dnodes) + net->Dcoord)
			* net->Enodes) + net->Ecoord);
    }
    id = (id * 68) + Kernel_ProcessorID();
    return id;
}

/*! \brief returns the T coordinate associated with the current thread
 *
 * 
 */
__INLINE__ uint32_t Kernel_MyTcoord();

/*! \brief returns the Rank associated with the current process
 *
 * 
 */
__INLINE__ uint32_t Kernel_GetRank();


/*! \brief returns the coordinate rank mapping algorithm.
 *
 * \param[in] maporderlen size of the provided maporder buffer in bytes
 * \param[inout] maporder pointer to a buffer that will contain the ABCDET mapping
 * \param[inout] isFile pointer to a boolean that will be set to indicate whether maporder refers to a custom mapfile
 */

__INLINE__ uint32_t Kernel_GetMapping(size_t maporderlen, char* maporder, uint32_t* isFile);



__END_DECLS

#endif /* _KERNEL_LOCATION_H_ */
