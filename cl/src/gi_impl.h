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

#ifndef  _KERNEL_CL_GI_IMPL_H_
#define  _KERNEL_CL_GI_IMPL_H_

// CL-specific includes
#include "cl_debug.h"
#include "cl_mu.h" // CL-specific support code for interfacing MUFS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


static int open_gi_cr_file(uint32_t id, int flags)
{
    char filename[25];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/mu/classroute/gi_%02u", id);

    return open(filename, flags);
}



/**
 * \file CL gi_impl.h
 *
 * \brief C Header File containing  implementation of the Global
 *        Interrupt SPIs in CL
 *
 */

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <spi/include/mu/Util.h>

#ifndef _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS
#define _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS ( BGQ_CLASS_LINK_AM | \
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
#endif

/**
 * \brief Query the Global Interrupt Class Routes that are Free to be Allocated
 *
 * Returns the Ids of the global interrupt class routes that are "free" to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of global interrupt class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param[out] nClassRoutes  Pointer to a single uint32_t where the number of free
 *                           global interrupt class routes is returned
 *                           (0 through BGQ_GI_CLASS_MAX_CLASSROUTES).
 *
 * \param[out] classRouteIds  Pointer to BGQ_GI_CLASS_MAX_CLASSROUTES uint32_t's
 *                            where the free global interrupt class route Ids are
 *                            returned.  Each returned Id has a value 0 through 
 *                            BGQ_GI_CLASS_MAX_CLASSROUTES-1.
 *
 * \param[in] sizeOfClassRouteIds  The number of bytes pointed to by classRouteIds.
 *                                 This is used to verify that there is enough
 *                                 storage pointed to by classRouteIds to hold all
 *                                 of the global interrupt class route Ids that are
 *                                 free.  If there is not enough storage, as many
 *                                 free Ids as possible are returned.
 *
 * \retval  0  Successful.  nClassRoutes global interrupt class route Ids are
 *             returned in classRouteIds.
 * \retval  errno  Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_QueryGlobalInterruptClassRoutes ( uint32_t * nClassRoutes,
                                                 uint32_t * classRouteIds,
                                                 size_t     sizeOfClassRouteIds )
{
    int32_t rc=0;
    int query_fd;

    query_fd = open("/mu/classroute/gi_query", O_RDONLY);

    if(query_fd == -1) {
        CL_ERROR("error %d opening gi_query file %s\n", errno, strerror(errno));
        return EIO;
    }

    rc = read(query_fd, classRouteIds, sizeOfClassRouteIds);
    if(rc == -1) {
        CL_ERROR("error %d reading gi_query file: %s.\n", errno, strerror(errno));
        return EIO;
    }

    // read delivers all unallocated class route ids -> number of bytes read
    // indicates the number of unallocated class routes
    *nClassRoutes = rc / sizeof(uint32_t);


    return 0;
}


/**
 * \brief Allocate a Global Interrupt Class Route Id
 *
 * The specified global interrupt class route Id is allocated (reserved) for
 * use, if it is not already allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of global interrupt class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *  
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \param [in]  attributes  Pointer to a structure containing attributes
 *                          for the global interrupt class route being
 *                          allocated.  If the default attributes are
 *                          desired (refer to the structure definition
 *                          for the default values), a NULL pointer may be 
 *                          specified.
 *
 * \retval  0  Successful.    Global Interrupt class route was successfully
 *             allocated.
 * \retval  errno Unsuccessful.  The ID is already allocated.
 *
 */
__INLINE__
int32_t Kernel_AllocateGlobalInterruptClassRoute ( 
             uint32_t id,
             Kernel_GlobalInterruptClassRouteAttributes_t *attributes )
{
    int fd;

    // check parameters
    if(id >= BGQ_GI_CLASS_MAX_CLASSROUTES)
        return EINVAL;

    // open file 
    fd = open_gi_cr_file(id, O_RDWR);
    
    // save fd if != -1
    if(fd == -1) {
        return errno;
    } else {
        set_gi_cr_fd( id, fd );
        return 0;
    }
}


/**
 * \brief Set a Global Interrupt Class Route Configuration
 *
 * There are 32 bits in a global interrupt class route configuration.  This
 * interface sets those bits for a specified global interrupt class route Id.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with setting global interrupt
 * class routes on the same node in another thread or process.  This can be
 * done by obtaining a node-scoped lock, if necessary.
 *
 * The following consistency checks are performed:
 * 1. The class route output must have at most one bit set.
 * 2. The class route output local bit must be 0.
 * 3. If a class route output bit is 1, the corresponding input bit cannot be 1.
 *    This is further explained as follows:
 *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
 *      the class route input and output.  Ignore the local bit.
 *    - If (v_in(i) & v_out(i)) !=0 then input and output share a link, which is
 *      invalid.
 * 4. Class route i and class route i+8 cannot share any links.
 *    This is further explained as follows:
 *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
 *      the class route input and output.  Ignore the local bit.
 *    - if  ( ( v_in(i) | v_out(i) ) & (v_in(i+8) | v_out(i+8)) != 0 then
 *      class routes i and i+8 share links, which is invalid.
 *
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \param [in]  cr  Pointer to the class route structure containing the 
 *                  global interrupt class route configuration to be set.
 *
 * \retval  0  Successful.  Collective class route was successfully configured.
 * \retval  1  Unsuccessful.  The class route output has more than 1 bit set.
 * \retval  2  Unsuccessful.  The class route output local bit must be 0.
 * \retval  3  Unsuccessful.  The same bit is set in both input and output.
 * \retval  4  Unsuccessful.  Class routes i and i+8 share links.
 *
 */
__INLINE__
int32_t Kernel_SetGlobalInterruptClassRoute ( int32_t       id,
                          ClassRoute_t *cr )
{
    int rc;

    if( ! have_gi_cr_fd(id) )
        return EINVAL;

    rc = write(get_gi_cr_fd(id), cr, sizeof(*cr));
    if(rc != sizeof(*cr))
        return errno;
    else
        return 0;
}


/**
 * \brief De-allocate a Global Interrupt Class Route Id
 *  
 * The specified global interrupt class route Id is de-allocated, such that it
 * is available to be allocated again.
 *
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.    Global Interrupt class route was successfully
 *                            de-allocated.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_DeallocateGlobalInterruptClassRoute ( int32_t id )
{
    int rc;

    if(!have_gi_cr_fd(id))
        return EINVAL;

    rc = close( get_gi_cr_fd(id) );

    set_gi_cr_fd(id, -1);

    return rc;
}


/**
 * \brief Configure Interrupts for a Global Interrupt Class Route
 *
 * For each global interrupt class route, there are three global interrupts
 * that can be individually enabled to cause an interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with configuring global interrupts
 * for other class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \param [in]  giId  Global Interrupt Id.  Within each class route, there can
 *                    be three individual global interrupts.  This specifies
 *                    which global interrupt is to have its interrupt
 *                    enabled (0 ... BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE-1).
 *
 * \param [in]  interruptFlag  Indicator of whether the interrupt should be
 *                             enabled (KERNEL_GI_INTERRUPT_FLAG_ENABLE) or 
 *                             disabled (KERNEL_GI_INTERRUPT_FLAG_DISABLE).
 *
 * \retval  0  Successful.  Global Interrupt class route was successfully
 *             configured for interrupts.
 * \retval errno  Unsuccessful.
 */
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__
int32_t Kernel_ConfigureGIInterrupts ( 
              uint32_t                 id,
              uint32_t                 giId,
              Kernel_GIInterruptFlag_t interruptFlag )
{
  return ENOSYS;
}
#endif                                                                                                                 // FUSEDOS


/**
 * \brief Get Global Barrier User Class Route Id
 *
 * Get the class route Id associated with a global barrier.  This class route...
 * - Is a global interrupt class route.
 * - Is a "user" class route for use by user mode applications.
 * - Includes all of the nodes in the job.
 * - Has an initialized global interrupt barrier associated with it.  This
 *   initialization takes place before the user mode application receives
 *   control.
 *
 * Usage Notes:
 * - Common usage is for a "master process" on each node to call this function
 *   to obtain this class route Id.
 * - The master process calls MUSPI_GIBarrierInit() to initialize software
 *   structures to use the barrier associated with that class route.  The
 *   kernel has previously called MUSPI_GIBarrierInitMU() to initialize the
 *   MU for the barrier.
 * - When processes on the node need to use the GIBarrier, they perform a
 *   local barrier first, then the master process enters the GIBarrier and polls
 *   for its completion, and finally, all processes perform the local barrier
 *   again.
 *
 * \param [out]  classRouteId  Pointer to a uint32_t where the Id of the user
 *                             global interrupt class route that includes all
 *                             nodes in the job is returned.
 *
 * \retval   0  Successful.
 * \retval   errno  Error.
 */
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__
int32_t Kernel_GetGlobalBarrierUserClassRouteId ( uint32_t *classRouteId )
{
  return ENOSYS; /* Firmware does not initialize this class route.  Only the
              * kernels do. */
}
#endif                                                                                                                 // FUSEDOS

#endif /* _KERNEL_FIRMWARE_GI_IMPL_H_ */
