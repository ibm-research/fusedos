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
/* (C) Copyright IBM Corp.  2010, 2012                              */
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

#ifndef	_KERNEL_CNK_COLLECTIVE_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_COLLECTIVE_IMPL_H_


/**
 * \file cnk/collective_impl.h
 *
 * \brief C Header File containing Compute Node Kernel (CNK) implementation of the Collective SPIs
 *
 */

#include <errno.h>
#include "cnk/include/SPI_syscalls.h"
#include <hwi/include/bqc/nd_500_dcr.h>



/**
 * \brief Query the Collective Class Routes that are Free to be Allocated
 *
 * Returns the Ids of the collective class routes that are "free" to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of collective class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param[out] nClassRoutes  Pointer to a single uint32_t where the number of free
 *                           collective class routes is returned
 *                           (0 through BGQ_COLL_CLASS_MAX_CLASSROUTES).
 *
 * \param[out] classRouteIds Pointer to BGQ_COLL_CLASS_MAX_CLASSROUTES uint32_t's
 *                           where the free collective class route Ids are returned.
 *                           Each returned Id has a value 0 through
 *                           BGQ_COLL_CLASS_MAX_CLASSROUTES-1.
 *
 * \param[in] sizeOfClassRouteIds   The number of bytes pointed to by classRouteIds.
 *                                  This is used to verify that there is enough
 *                                  storage pointed to by classRouteIds to hold all
 *                                  of the collective class route Ids that are free.
 *                                  If there is not enough storage, as many free
 *                                  Ids as possible are returned.
 *
 * \retval  0  Successful.  nClassRoutes collective class route Ids are returned
 *             in classRouteIds.
 * \retval errno  Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_QueryCollectiveClassRoutes ( uint32_t * nClassRoutes,
                                            uint32_t * classRouteIds,
                                            size_t     sizeOfClassRouteIds )
{
    return (int32_t) CNK_SPI_SYSCALL_3(QUERYCOLLECTIVECLASSROUTE, nClassRoutes, classRouteIds, sizeOfClassRouteIds);
}


/**
 * \brief Allocate a Collective Class Route Id
 *
 * The specified collective class route Id is allocated (reserved) for use,
 * if it is not already allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of collective class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *  
 * \param [in]  id  The identifier of the collective class route
 *                  (0 through BGQ_COLL_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.    Collective class route was successfully allocated.
 * \retval  errno Unsuccessful.  The ID is already allocated.
 */
__INLINE__
int32_t Kernel_AllocateCollectiveClassRoute ( uint32_t id )
{
    return (int32_t) CNK_SPI_SYSCALL_1(ALLOCATECOLLECTIVECLASSROUTE, id);
}


/**
 * \brief Set a Collective Class Route Configuration
 *
 * There are 32 bits in a collective class route configuration.  This interface
 * sets those bits for a specified collective class route Id.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with setting collective class routes
 * on the same node in another thread or process.  This can be done by obtaining
 * a node-scoped lock, if necessary.
 *
 * The following consistency checks are performed:
 * 1. The class route output must have at most one bit set.
 * 2. The class route output local bit must be 0.
 * 3. The class route input local bit must be 1.
 * 4. If a class route output bit is 1, the corresponding input bit cannot be 1.
 *    This is further explained as follows:
 *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
 *      the class route input and output.  Ignore the local bit.
 *    - If (v_in(i) & v_out(i)) !=0 then input and output share a link, which is
 *      invalid.
 * 5. A system class route and a user commworld class route cannot share any links.
 *    This is further explained as follows:
 *    - Let i be a system class route that is already set or is being set.
 *    - Let j be a user commworld class route that is already set or is being set.
 *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
 *      the system class route input and output.  Ignore the local bit.
 *    - Let v_in(j) and v_out(j) be those bits in the user commworld class route
 *      input and output.
 *    - if  ( ( v_in(i) | v_out(i) ) & (v_in(j) | v_out(j)) != 0 then
 *      class routes i and j share links, which is invalid.
 *
 * \param [in]  id  The idenfitier of the collective class route
 *                  (0 through BGQ_COLL_CLASS_MAX_CLASSROUTES-1).
 *
 * \param [in]  cr  Pointer to the class route structure containing the 
 *                  collective class route configuration to be set.
 *
 * \retval  0  Successful.  Collective class route was successfully configured.
 * \retval  1  Unsuccessful.  The class route output has more than 1 bit set.
 * \retval  2  Unsuccessful.  The class route output local bit must be 0.
 * \retval  3  Unsuccessful.  The class route input local bit must be 1.
 * \retval  4  Unsuccessful.  The same bit is set in both input and output.
 * \retval  5  Unsuccessful.  A system class route and a user commworld class
 *                            route share links.
 *
 */
__INLINE__
int32_t Kernel_SetCollectiveClassRoute ( uint32_t      id,
					 ClassRoute_t *cr )
{
    return (int32_t) CNK_SPI_SYSCALL_2(SETCOLLECTIVECLASSROUTE, id, cr);
}




/**
 * \brief De-allocate a Collective Class Route Id
 *  
 * The specified collective class route Id is de-allocated, such that it is
 * available to be allocated again.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of collective class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param [in]  id  The idenfitier of the collective class route
 *                  (0 through BGQ_COLL_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.    Collective class route was successfully
 *                            de-allocated.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_DeallocateCollectiveClassRoute ( uint32_t id )
{
    return (int32_t) CNK_SPI_SYSCALL_1(DEALLOCATECOLLECTIVECLASSROUTE, id);
}


__INLINE__
int32_t Kernel_ClearInjectionChecksum(uint64_t linkID)
{
    return (int32_t) CNK_SPI_SYSCALL_1(CLEARINJECTIONCHECKSUM, linkID);
}


__INLINE__
int32_t Kernel_GetInjectionChecksum(uint64_t linkID, uint64_t* checksum)
{
    *checksum = DCRReadUser(ND_500_DCR(INJ_FIFO_CHKSUM) + linkID);
    return 0;
}

#endif /* _KERNEL_CNK_COLLECTIVE_IMPL_H_ */
