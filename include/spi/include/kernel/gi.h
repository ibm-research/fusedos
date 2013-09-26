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

#ifndef	_KERNEL_GI_H_ /* Prevent multiple inclusion */
#define	_KERNEL_GI_H_

__BEGIN_DECLS

/**
 * \file gi.h
 *
 * \brief C Header File containing Kernel Global Interrupt Definitions.
 *
 * These interfaces manage the allocation of global interrupt class route IDs,
 * and set the configuration of global interrupt class routes.
 *
 * Global interrupt class routes are used for two purposes:
 *
 * 1.  To implement barriers.  The class routes managed by interfaces in this
 *     header file are used to route barrier packets among the nodes
 *     participating in the barrier.  Refer to spi/include/mu/GIBarrier.h for
 *     the interfaces that control the barrier itself.
 *
 * 2.  To implement global interrupts.  The class routes managed by interfaces
 *     in this header file are used to route global interrupt packets among the
 *     nodes participating in the global interrupt.  This functionality provides
 *     the ability to cause an interrupt on all participating nodes.  Up to 3
 *     separate interrupts may be generated per class route.  An interface is
 *     provided to separately control the enablement of these interrupts.
 *     Refer to spi/include/mu/GI.h for the interfaces that control the global
 *     interrupts themselves.
 *
 */
#include "kernel_impl.h"
#include <hwi/include/bqc/classroute.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <spi/include/kernel/memory.h>


/*!
 * \brief __INLINE__ definition
 * 
 * Option 1:
 * Make all functions be "static inline":
 * - They are inlined if the compiler can do it
 * - If the compiler does not inline it, a single copy of the function is
 *   placed in the translation unit (eg. xxx.c)for use within that unit.
 *   The function is not externalized for use by another unit...we want this
 *   so we don't end up with multiple units exporting the same function,
 *   which would result in linker errors.
 *
 * Option 2:
 * A GNU C model: Use "extern inline" in a common header (this one) and provide
 * a definition in a .c file somewhere, perhaps using macros to ensure that the
 * same code is used in each case. For instance, in the header file:
 *
 * \verbatim
   #ifndef INLINE
   # define INLINE extern inline
   #endif
   INLINE int max(int a, int b) {
     return a > b ? a : b;
   }
   \endverbatim
 *
 * ...and in exactly one source file (in spi/src), that is included in a
 * library...
 *
 * \verbatim
   #define INLINE
   #include "header.h"
   \endverbatim
 * 
 * This allows inlining, where possible, but when not possible, only one 
 * instance of the function is in storage (in the library).
 */
#ifndef __INLINE__
#define __INLINE__ extern inline
#endif


/**
 * \brief Global Interrupt Class Route Attributes Structure
 *
 * Contains attributes for global interrupt class routes.
 *
 */
typedef struct Kernel_GlobalInterruptClassRouteAttributes
{
  uint32_t System    : 1; /**< 1:  The global interrupt class route is reserved
			           for use by privileged system code.  Can 
                                   only be set to 1 if running in privileged
                                   mode. 
                               0:  The global interrupt class route may be
                                   used by user or by privileged system code.
                               The default, if this attributes structure is not
			       specified, is 0.  */

} Kernel_GlobalInterruptClassRouteAttributes_t;


/**
 * \brief Global Interrupt Interrupt Flag
 *
 */
typedef enum Kernel_GIInterruptFlag
{
  KERNEL_GI_INTERRUPT_FLAG_DISABLE, /**< Disable the interrupt */
  KERNEL_GI_INTERRUPT_FLAG_ENABLE   /**< Enable  the interrupt */
} Kernel_GIInterruptFlag_t;


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
int32_t Kernel_QueryGlobalInterruptClassRoutes ( uint32_t *nClassRoutes,
                                                 uint32_t *classRouteIds,
                                                 size_t    sizeOfClassRouteIds );


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
		     Kernel_GlobalInterruptClassRouteAttributes_t *attributes );


/**
 * \brief De-allocate a Global Interrupt Class Route Id
 *  
 * The specified global interrupt class route Id is de-allocated, such that it
 * is available to be allocated again.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of global interrupt class routes.  This can be done by obtaining a node-scoped
 * lock, if necessary.
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
int32_t Kernel_DeallocateGlobalInterruptClassRoute ( int32_t id );


/**
 * \brief Check a Global Interrupt Class Route Configuration
 *
 * This function checks the specified class route to see if it is correct and
 * compatible with other class routes that are already set.  It does not 
 * actually set the class route.  Use Kernel_SetGlobalInterruptClassRoute() to
 * do that.
 *
 * There are 32 bits in a global interrupt class route configuration.  This
 * interface checks those bits for a specified global interrupt class route Id.
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
 *                  global interrupt class route configuration to be checked.
 *
 * \retval  0  Successful.  Collective class route was successfully checked and
 *                          found to be correct and have no conflicts.
 * \retval  1  Unsuccessful.  The class route output has more than 1 bit set.
 * \retval  2  Unsuccessful.  The class route output local bit must be 0.
 * \retval  3  Unsuccessful.  The same bit is set in both input and output.
 * \retval  4  Unsuccessful.  Class routes i and i+8 share links.
 *
 */
__INLINE__
int32_t Kernel_CheckGlobalInterruptClassRoute ( int32_t       id,
						ClassRoute_t *cr );


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
					      ClassRoute_t *cr );


/**
 * \brief Set a Global Interrupt Class Route Configuration to Empty Status
 *
 * There are 32 bits in a global interrupt class route configuration.  This
 * interface sets those bits for a specified global interrupt class route Id
 * such that it the class route appears to be allocated, but not set.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with setting global interrupt
 * class routes on the same node in another thread or process.  This can be
 * done by obtaining a node-scoped lock, if necessary.
 *
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.  Collective class route was successfully set to an
 *                          empty but still allocated state.
 */
__INLINE__
int32_t Kernel_SetEmptyGlobalInterruptClassRoute ( int32_t id );


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
              Kernel_GIInterruptFlag_t interruptFlag );


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
__INLINE__
int32_t Kernel_GetGlobalBarrierUserClassRouteId ( uint32_t *classRouteId );


/**
 * \brief Include implementations of the above functions.
 */
#include "gi_impl.h"


/**
 * \brief Generic implementations.
 */


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
 * \brief Check a Global Interrupt Class Route Configuration
 *
 * This function checks the specified class route to see if it is correct and
 * compatible with other class routes that are already set.  It does not 
 * actually set the class route.  Use Kernel_SetGlobalInterruptClassRoute() to
 * do that.
 *
 * There are 32 bits in a global interrupt class route configuration.  This
 * interface checks those bits for a specified global interrupt class route Id.
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
 *                  global interrupt class route configuration to be checked.
 *
 * \retval  0  Successful.  Collective class route was successfully checked and
 *                          found to be correct and have no conflicts.
 * \retval  1  Unsuccessful.  The class route output has more than 1 bit set.
 * \retval  2  Unsuccessful.  The class route output local bit must be 0.
 * \retval  3  Unsuccessful.  The same bit is set in both input and output.
 * \retval  4  Unsuccessful.  Class routes i and i+8 share links.
 *
 */
__INLINE__
int32_t Kernel_CheckGlobalInterruptClassRoute ( int32_t       id,
						ClassRoute_t *cr )
{
  uint32_t bitCount = 0;
  uint16_t outBits1, outBits2;
  uint16_t v_in, v_out, v_in8, v_out8;
  uint32_t id8;
  uint64_t dcr_value;
  uint32_t starting_bit;

  /* Checks for class route inputs */
  MUSPI_assert ( id < BGQ_GI_CLASS_MAX_CLASSROUTES );

  /* Perform consistency checks: */

  /*
   * 1. The class route output must have at most one bit set.
   */
  outBits1 = outBits2 = cr->output;
  for(; outBits1; outBits1=outBits1>>1)
    {
      bitCount += outBits1 & 0x1u;
    }
  if ( bitCount > 1 )
    {
      return 1;
    }

  /*
   * 2. The class route output local bit must be 0.
   */
  if ( outBits2 & BGQ_CLASS_INPUT_LINK_LOCAL ) 
    {
      return 2;
    }

  /*
   * 3. If a class route output bit is 1, the corresponding input bit cannot be 1.
   *    This is further explained as follows:
   *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
   *      class route i's input and output.  Ignore the local bit.
   *    - If (v_in(i) & v_out(i)) !=0 then input and output share a link, which is
   *      invalid.
   */
  v_in  = cr->input  & _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS;
  v_out = outBits2  & _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS;

  if ( v_in & v_out ) 
    {
      return 3;  
    }
   
  /*
   * 4. Class route i and class route i+8 cannot share any links.
   *    This is further explained as follows:
   *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
   *      class route i's input and output.  Ignore the local bit.
   *    - if  ( ( v_in(i) | v_out(i) ) & (v_in(i+8) | v_out(i+8)) != 0 then
   *      class routes i and i+8 share links, which is invalid.
   */
  id8 = (id + 8) & 0xF; /* Calculate corresponding class route id i+8 */

  /* 4.1 Get the current value of id8's DCR. */
  if ( Kernel_GetPrivilegeMode() == 0 )
    dcr_value = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(id8/2));
  else
    dcr_value = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01)+(id8/2));

  /* 4.2 Determine the 32 bits corresponding to id8.
   *   - Even IDs are in the high order 32 bits
   *   - Odd  IDs are in the low  order 32 bits.
   * 4.3 Adjust dcr value to bits on the low order.
   */
  starting_bit = (id8 & 0x1) * 32; /* Either 0 or 32 */
  dcr_value = dcr_value >> (32-starting_bit);  

  /* 4.4 Extract the input and output bits we are interested in */
  v_in8  = (dcr_value >> 16)  & _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS;
  v_out8 = dcr_value          & _KERNEL_GI_CLASS_ROUTE_A_E_IO_BITS;

  /* 4.5 Do the check of i and i+8 sharing links */
  if  ( ( v_in | v_out ) & (v_in8 | v_out8) )
    {
      /* { */
      /* 	uint64_t d01 = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(0)); */
      /* 	uint64_t d23 = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(1)); */
      /* 	uint64_t d45 = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(2)); */
      /* 	uint64_t d67 = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(3)); */
      /* 	uint64_t d89 = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(4)); */
      /* 	uint64_t dab = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(5)); */
      /* 	uint64_t dcd = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(6)); */
      /* 	uint64_t def = DCRReadUser(ND_500_DCR(CTRL_GI_CLASS_00_01)+(7)); */
      /* 	printf("Check rc=4, id=%d, cr=%x %x, d01=%lx, d23=%lx, d45=%lx, d67=%lx, d89=%lx, dab=%lx, dcd=%lx, def=%lx\n",id, cr->input,cr->output,d01,d23,d45,d67,d89,dab,dcd,def); */
      /* } */
      return 4;
    }

  return 0;
}


/**
 * \brief Set a Global Interrupt Class Route Configuration to Empty Status
 *
 * There are 32 bits in a global interrupt class route configuration.  This
 * interface sets those bits for a specified global interrupt class route Id
 * such that it the class route appears to be allocated, but not set.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with setting global interrupt
 * class routes on the same node in another thread or process.  This can be
 * done by obtaining a node-scoped lock, if necessary.
 *
 * \param [in]  id  The identifier of the global interrupt class route
 *                  (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.  Collective class route was successfully set to an
 *                          empty but still allocated state.
 */
__INLINE__
int32_t Kernel_SetEmptyGlobalInterruptClassRoute ( int32_t id )
{
  int32_t rc;
  ClassRoute_t cr;

  /* Create a class route structure containing values that indicate the
   * class route is empty but still allocated.
   */
  cr.input  = BGQ_CLASS_INPUT_LINK_LOCAL;
  cr.output = 0;

  /* Set that class route */
  rc = Kernel_SetGlobalInterruptClassRoute ( id,
					     &cr );
  return rc;
}


__END_DECLS

#endif /* _KERNEL_GI_H_ */
