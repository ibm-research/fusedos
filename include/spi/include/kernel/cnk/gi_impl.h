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

#ifndef	_KERNEL_CNK_GI_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_GI_IMPL_H_


/**
 * \file cnk/gi_impl.h
 *
 * \brief C Header File containing Compute Node Kernel (CNK) implementation of
 *        the Global Interrupt SPIs
 *
 */

#include <errno.h>
#include "cnk/include/SPI_syscalls.h"


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
    return (int32_t) CNK_SPI_SYSCALL_3(QUERYGINTCLASSROUTE, nClassRoutes, classRouteIds, sizeOfClassRouteIds);
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
    return (int32_t) CNK_SPI_SYSCALL_2(ALLOCATEGINTCLASSROUTE, id, attributes);
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
    return (int32_t) CNK_SPI_SYSCALL_2(SETGINTCLASSROUTE, id, cr);
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
 * \retval  errno Unsuccessful.  errno gives the reason.
 *
 */
__INLINE__
int32_t Kernel_DeallocateGlobalInterruptClassRoute ( int32_t id )
{
    return (int32_t) CNK_SPI_SYSCALL_1(DEALLOCATEGINTCLASSROUTE, id);
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
 * \retval  errno  Unsuccessful.
 */
__INLINE__
int32_t Kernel_ConfigureGIInterrupts ( 
	      uint32_t                 id,
              uint32_t                 giId,
              Kernel_GIInterruptFlag_t interruptFlag )
{
    return (int32_t) CNK_SPI_SYSCALL_3(CONFIGUREGIINTERRUPTS, id, giId, interruptFlag);
}


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
 * \retval  errno  Error.
 */
__INLINE__
int32_t Kernel_GetGlobalBarrierUserClassRouteId ( uint32_t *classRouteId )
{
  /**
   * \todo Should this be a syscall so this Id does not get hard-coded into
   *       the application via this inline?
   */
  *classRouteId = 0; /* Use hard-coded GI class route Id. */

  return 0; /* Success */
}


#endif /* _KERNEL_CNK_GI_IMPL_H_ */
