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

#ifndef  _KERNEL_CL_COLLECTIVE_IMPL_H_
#define  _KERNEL_CL_COLLECTIVE_IMPL_H_

// CL-specific includes
#include "cl_debug.h"
#include "cl_mu.h" // CL-specific support code for interfacing MUFS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static int open_coll_cr_file(uint32_t id, int flags)
{
    char filename[25];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/mu/classroute/coll_%02u", id);

    return open(filename, flags);
}


/**
 * \file CL collective_impl.h
 *
 * \brief C Header File containing implementation of the Collective SPIs
 * in CL
 *
 */

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <spi/include/mu/Util.h>

/**
 * \brief Class Route Bits For A through E and IO Directions
 *
 * This is used to determine if class routes share any links
 */
#define _KERNEL_COLL_CLASS_ROUTE_A_E_IO_BITS ( BGQ_CLASS_LINK_AM | \
                                               BGQ_CLASS_LINK_AP | \
                                               BGQ_CLASS_LINK_BM | \
                           BGQ_CLASS_LINK_BP | \
                           BGQ_CLASS_LINK_CM | \
                           BGQ_CLASS_LINK_CP | \
                           BGQ_CLASS_LINK_DM | \
                           BGQ_CLASS_LINK_DP | \
                           BGQ_CLASS_LINK_EM | \
                           BGQ_CLASS_LINK_EP | \
                           BGQ_CLASS_LINK_IO );


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
    int32_t rc=0;
    int query_fd;

    query_fd = open("/mu/classroute/coll_query", O_RDONLY);

    if(query_fd == -1) {
        CL_ERROR("error %d opening coll_query file %s\n", errno, strerror(errno));
        return EIO;
    }

    rc = read(query_fd, classRouteIds, sizeOfClassRouteIds);
    if(rc == -1) {
        CL_ERROR("error %d reading coll_query file: %s.\n", errno, strerror(errno));
        return EIO;
    }

    // read delivers all unallocated class route ids -> number of bytes read
    // indicates the number of unallocated class routes
    *nClassRoutes = rc / sizeof(uint32_t);

    return 0;
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
 *
 */
__INLINE__
int32_t Kernel_AllocateCollectiveClassRoute ( uint32_t id )
{
    int fd;

    // check parameters
    if(id >= BGQ_COLL_CLASS_MAX_CLASSROUTES)
        return EINVAL;

    // open file 
    fd = open_coll_cr_file(id, O_RDWR);

    // save fd if != -1
    if(fd == -1) {
        return errno;
    } else {
        set_coll_cr_fd( id, fd );
        return 0;
    }
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
    int rc;

    if( ! have_coll_cr_fd(id) )
        return EINVAL;

    rc = write(get_coll_cr_fd(id), cr, sizeof(*cr));
    if(rc != sizeof(*cr))
        return errno;
    else
        return 0;
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
    int rc;

    if(!have_coll_cr_fd(id))
        return EINVAL;

    rc = close( get_coll_cr_fd(id) );

    set_coll_cr_fd(id, -1);

    return rc;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__
int32_t Kernel_ClearInjectionChecksum(uint64_t linkID)
{
    if(linkID >= ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_width)
        return -1;
    
    DCRWritePriv(ND_500_DCR(CTRL_CLEAR1), ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_set( 1ULL << (ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_width-linkID-1)));
    DCRWritePriv(ND_500_DCR(CTRL_CLEAR1), 0);
    return 0;
}

__INLINE__
int32_t Kernel_GetInjectionChecksum(uint64_t linkID, uint64_t* checksum)
{
    if(linkID >= ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_width)
        return -1;
    
    *checksum = DCRReadUser(ND_500_DCR(INJ_FIFO_CHKSUM) + linkID);
    return 0;
}
#endif                                                                                                                 // FUSEDOS

#endif /* _KERNEL_FIRMWARE_COLLECTIVE_IMPL_H_ */
