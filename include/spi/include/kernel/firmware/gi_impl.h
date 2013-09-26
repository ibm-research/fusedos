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

#ifndef  _KERNEL_FIRMWARE_GI_IMPL_H_
#define  _KERNEL_FIRMWARE_GI_IMPL_H_

/**
 * \file firmware/gi_impl.h
 *
 * \brief C Header File containing firmware implementation of the Global
 *        Interrupt SPIs
 *
 */

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <stdint.h>
#endif                                                                                                                 // FUSEDOS
#include <hwi/include/common/compiler_support.h>
#if !(defined __KERNEL__)                                                                                              // FUSEDOS
#include <hwi/include/bqc/A2_core.h>
#endif                                                                                                                 // FUSEDOS
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
  MUSPI_assert ( nClassRoutes  != NULL );
  MUSPI_assert ( classRouteIds != NULL );

  /* Initialize the actual number of free class route Ids to zero.
   * Initialize the loop counter. 
   */
  uint32_t actualNum = 0;
  uint32_t i;
  uint64_t dcr_value;
  uint32_t starting_bit;

  /* Determine the number of free global interrupt class route Ids that can fit into
   * the caller's classRouteIds array.
   */
  uint32_t capacity = sizeOfClassRouteIds / sizeof(uint32_t);

  /* For each global interrupt class route Id that is free,
   * - check to be sure it fits into the caller's classRouteIds array...quit
   *   if it doesn't.
   * - set the Id into the caller's classRouteIds array.
   * - increment the number of class route Ids returned.
   */
  for ( i=0; i<BGQ_GI_CLASS_MAX_CLASSROUTES; i++ )
    {
      /* A class route value of zeros means the class route is free.
       * There are 2 class routes in each DCR - 32 bits each.
       *   - Even IDs are in the high order 32 bits
       *   - Odd  IDs are in the low  order 32 bits.
       * Isolate to the correct class route.
       */
      dcr_value    = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01)+(i/2));
      starting_bit = (i & 0x1) * 32; /* Either 0 or 32 */

      if ( ( dcr_value & (0xFFFFFFFF00000000ULL >> starting_bit ) ) == 0 )
      {
	if ( actualNum >= capacity ) break;
	
	classRouteIds[actualNum] = i;
	actualNum++;
      }
    }

  /* Return the actual number of global interrupt class route Ids that are being
   * returned, and a successful return code.
   */
  *nClassRoutes = actualNum;

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
  /* Set up for error - ID already allocated */
  int32_t rc = EAGAIN;
  uint64_t dcr_value;
  uint64_t new_value;
  uint64_t starting_bit;

  MUSPI_assert ( id < BGQ_GI_CLASS_MAX_CLASSROUTES );

  /* A class route value of zeros means the class route is free.
   * There are 2 class routes in each DCR - 32 bits each.
   *   - Even IDs are in the high order 32 bits
   *   - Odd  IDs are in the low  order 32 bits.
   * Isolate to the correct class route.
   */
  dcr_value    = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2));
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Verify the class route is free */
  if ( ( dcr_value & (0xFFFFFFFF00000000ULL >> starting_bit ) ) == 0 )
  {
    /* Set a dummy non-zero value into the DCR to indicate that the class route
     * is allocated.  The VC is no used for GI class routes.
     */
    new_value = (dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) |
      ( (((uint64_t)(BGQ_CLASS_INPUT_LINK_LOCAL<<16))) << (32-starting_bit) );
    DCRWritePriv (ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2), new_value);

/*   printf("Kernel_AllocateGlobalInterruptClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
    /* 	 (long long unsigned int)new_value); */

    /* Set the "system" attribute into the DCR.
     * The caller is responsible for ensuring that if a "system" class route is
     * desired, that it is valid.  So, at this point, we blindly honor the
     * "system" attribute.
     *
     * - Get the current value of the DCR.
     * - Set it with the "system" bit set to the specified value.
     */
    dcr_value = DCRReadPriv(MU_DCR(SYS_BARRIER));

/*     printf("Kernel_AllocateGlobalInterruptClassRoute:  Original System DCR is 0%016lx\n",dcr_value); */

    if ( (attributes != NULL) && (attributes->System == 1) )
      dcr_value |= ( 1ULL << (BGQ_GI_CLASS_MAX_CLASSROUTES-1-id) );
    else
      dcr_value &= ~( 1ULL << (BGQ_GI_CLASS_MAX_CLASSROUTES-1-id) );

/*     printf("Kernel_AllocateGlobalInterruptClassRoute:  Setting System DCR to 0%016lx\n",dcr_value); */

    DCRWritePriv( MU_DCR(SYS_BARRIER), dcr_value );

    /* Set a good return code. */
    rc = 0;
  }

  return rc;
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
  uint32_t bitCount = 0;
  uint16_t outBits1, outBits2;
  uint16_t v_in, v_out, v_in8, v_out8;
  uint32_t id8;
  uint64_t dcr_value, new_value;
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
      return 4;
    }

  /******* Done with consistency checks *******/

  /* Get the current value of the DCR. */
  dcr_value = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2));

  /* Determine the 32 bits corresponding to the specified ID.
   * - Even IDs are in the high order 32 bits
   * - Odd  IDs are in the low  order 32 bits.
   */
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Build the new value in the right position: 16 bits of input, 16 bits of output */
  new_value = 
    ((uint64_t)cr->input  << (48 - starting_bit)) |
    ((uint64_t)cr->output << (32 - starting_bit));

  /* Zero out the previous bits, and OR the new value into the DCR */
  DCRWritePriv (ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2), 
		(dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) | new_value);      
  /* Read to ensure write finished. */

/*   printf("Kernel_SetGlobalInterruptClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
/* 	 (long long unsigned int)((dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) | new_value)); */

  /** \todo Release the lock. */
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
  uint64_t dcr_value;
  uint64_t new_value;
  uint32_t starting_bit;

  MUSPI_assert ( id < BGQ_GI_CLASS_MAX_CLASSROUTES );

  /* Get the current value of the DCR. */
  dcr_value = DCRReadPriv(ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2));

  /* Determine the 32 bits corresponding to the specified ID.
   * - Even IDs are in the high order 32 bits
   * - Odd  IDs are in the low  order 32 bits.
   */
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Derive new DCR value by zeroing the previous bits */
  new_value = dcr_value & (0x00000000FFFFFFFFULL << starting_bit);

  /* Set DCR value */
  DCRWritePriv ( ND_500_DCR(CTRL_GI_CLASS_00_01)+(id/2), 
		 new_value );

/*   printf("Kernel_DeallocateCollectiveClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
    /* 	 (long long unsigned int)new_value); */

  /* Reset the "system" attribute in the DCR to 1s (system). */

  dcr_value = DCRReadPriv(MU_DCR(SYS_BARRIER));

/*     printf("Kernel_DeallocateGlobalInterruptClassRoute:  Original System DCR is 0%016lx\n",dcr_value); */

  dcr_value |= ( 1ULL << (BGQ_GI_CLASS_MAX_CLASSROUTES-1-id) );

/*     printf("Kernel_DeallocateGlobalInterruptClassRoute:  Setting System DCR to 0%016lx\n",dcr_value); */

  DCRWritePriv( MU_DCR(SYS_BARRIER), dcr_value );
    
  return 0;
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
__INLINE__
int32_t Kernel_ConfigureGIInterrupts ( 
              uint32_t                 id,
              uint32_t                 giId,
              Kernel_GIInterruptFlag_t interruptFlag )
{
  uint64_t new_dcr_value;
  uint64_t dcr_value;
  uint64_t bit;

  /* Checks for class route inputs */
  MUSPI_assert ( id < BGQ_GI_CLASS_MAX_CLASSROUTES );
  MUSPI_assert ( giId < BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE );

  /* Get the current value of the DCR. */
  dcr_value = DCRReadPriv(MU_DCR(BARRIER_INT_EN));

  /* Determine the bit corresponding to the specified class route and GI id.
   * There is a nibble (4 bits) per class route.
   * The first bit in the nibble is unused.
   * The next 3 bits correspond to GI ids 0, 1, 2 respectively.
   */
  bit = 1ULL << ( 63 - ( (id * 4) + giId + 1 ) );

  /* Update the dcr_value depending on if enabling or disabling the interrupt. */
  if ( interruptFlag == KERNEL_GI_INTERRUPT_FLAG_ENABLE )
    new_dcr_value = dcr_value | bit;
  else
    new_dcr_value = dcr_value & ~bit; 

  /* Store the updated DCR value */
  DCRWritePriv (MU_DCR(BARRIER_INT_EN), new_dcr_value);

/*   printf("Kernel_ConfigureGIInterrupts: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
/* 	 (long long unsigned int)new_dcr_value); */

  return 0;
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
 * \retval   errno  Error.
 */
__INLINE__
int32_t Kernel_GetGlobalBarrierUserClassRouteId ( uint32_t *classRouteId )
{
  return ENOSYS; /* Firmware does not initialize this class route.  Only the
	          * kernels do. */
}

#endif /* _KERNEL_FIRMWARE_GI_IMPL_H_ */
