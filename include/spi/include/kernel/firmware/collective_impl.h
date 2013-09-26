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
#ifndef  _KERNEL_FIRMWARE_COLLECTIVE_IMPL_H_
#define  _KERNEL_FIRMWARE_COLLECTIVE_IMPL_H_

/**
 * \file firmware/collective_impl.h
 *
 * \brief C Header File containing firmware implementation of the Collective SPIs
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
  /* Initialize the actual number of free class route Ids to zero.
   * Initialize the loop counter. 
   */
  uint32_t actualNum = 0;
  uint32_t i;
  uint64_t dcr_value;
  uint32_t starting_bit;
  uint32_t capacity;

  MUSPI_assert ( nClassRoutes  != NULL );
  MUSPI_assert ( classRouteIds != NULL );

  /* Determine the number of free collective class route Ids that can fit into
   * the caller's classRouteIds array.
   */
  capacity = sizeOfClassRouteIds / sizeof(uint32_t);

  /* For each collective class route Id that is free,
   * - check to be sure it fits into the caller's classRouteIds array...quit
   *   if it doesn't.
   * - set the Id into the caller's classRouteIds array.
   * - increment the number of class route Ids returned.
   */
  for ( i=0; i<BGQ_COLL_CLASS_MAX_CLASSROUTES; i++ )
    {
      /* A class route value of zeros means the class route is free.
       * There are 2 class routes in each DCR - 32 bits each.
       *   - Even IDs are in the high order 32 bits
       *   - Odd  IDs are in the low  order 32 bits.
       * Isolate to the correct class route.
       */
      dcr_value    = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01)+(i/2));
      starting_bit = (i & 0x1) * 32; /* Either 0 or 32 */

      if ( ( dcr_value & (0xFFFFFFFF00000000ULL >> starting_bit ) ) == 0 )
      {
	if ( actualNum >= capacity ) break;
	
	classRouteIds[actualNum] = i;
	actualNum++;
      }
    }

  /* Return the actual number of collective class route Ids that are being
   * returned, and a successful return code.
   */
  *nClassRoutes = actualNum;

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
  /* Set up for error - ID is already allocated */
  int32_t rc = EAGAIN;
  uint64_t dcr_value;
  uint64_t new_value;
  
  uint64_t starting_bit;

  MUSPI_assert ( id < BGQ_COLL_CLASS_MAX_CLASSROUTES );

  /* A class route value of zeros means the class route is free.
   * There are 2 class routes in each DCR - 32 bits each.
   *   - Even IDs are in the high order 32 bits
   *   - Odd  IDs are in the low  order 32 bits.
   * Isolate to the correct class route.
   */
  dcr_value    = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2));
  new_value   = dcr_value;
  
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Verify the class route is free */
  if ( ( dcr_value & (0xFFFFFFFF00000000ULL >> starting_bit ) ) == 0 )
  {
    /* Set a dummy non-zero value into the DCR to indicate that the class route
     * is allocated.
     */
    if ( starting_bit == 0) ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_insert(new_value,3);
    else ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_insert(new_value,3);
    //  new_value = (dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) | 
    //  ( ((uint64_t)(BGQ_CLASS_LINK_AM)) << (32-starting_bit) );
     DCRWritePriv (ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2), new_value);
    rc = 0;
/*   printf("Kernel_AllocateCollectiveClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
/*     	 (long long unsigned int)new_value); */
  }

  return rc;
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
  uint32_t bitCount = 0;
  uint16_t outBits1, outBits2;
  uint16_t inBits;
  uint16_t v_in, v_out, v_in_j, v_out_j;
  uint32_t lookingFor;
  uint32_t j;
  uint64_t dcr_value, new_value;
  uint32_t starting_bit;

  /* Checks for class route inputs */
  MUSPI_assert ( id < BGQ_COLL_CLASS_MAX_CLASSROUTES );

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
   * 3. The class route input local bit must be 1.
   */
  inBits = cr->input;
  if ( (inBits & BGQ_CLASS_INPUT_LINK_LOCAL) == 0 )
    {
      return 3;
    }

  /*
   * 4. If a class route output bit is 1, the corresponding input bit cannot be 1.
   *    This is further explained as follows:
   *    - Let v_in(i) and v_out(i) be the 11 bits corresponding to A-,..., E+,IO in
   *      class route i's input and output.  Ignore the local bit.
   *    - If (v_in(i) & v_out(i)) !=0 then input and output share a link, which is
   *      invalid.
   */
  v_in  = inBits   & _KERNEL_COLL_CLASS_ROUTE_A_E_IO_BITS;
  v_out = outBits2 & _KERNEL_COLL_CLASS_ROUTE_A_E_IO_BITS;

  if ( v_in & v_out ) 
    {
      return 4;  
    }
   
  /*
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
   */
  
  /* 5.1 Determine which "already set" VC we are looking for:
   *     - If our VC is USER, then we are looking for SYSTEM
   *     - If our VC is SYSTEM, then we are looking for USER
   *     - Otherwise, we don't have anything to look for.
   */
  lookingFor = 0xFF; /* The already-set VC we are looking for */
  if ( (inBits & BGQ_CLASS_INPUT_VC_MASK) == BGQ_CLASS_INPUT_VC_USER )
    lookingFor = BGQ_CLASS_INPUT_VC_SYSTEM;
  else
    if ( (inBits & BGQ_CLASS_INPUT_VC_MASK) == BGQ_CLASS_INPUT_VC_SYSTEM )
      lookingFor = BGQ_CLASS_INPUT_VC_USER;

  /* 5.2 If we need to look for an "already set" class route, loop through
   *     the class routes and check each matching one.
   */
  if ( lookingFor != 0xFF )
    {
      for ( j=0; j<BGQ_COLL_CLASS_MAX_CLASSROUTES; j++)
	{
	  if ( j == id ) continue;

	  /* 5.3 Get the current value of j's DCR. */
	  dcr_value = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01)+(j/2));

	  /* 5.4 Determine the 32 bits corresponding to j.
	   *   - Even IDs are in the high order 32 bits
	   *   - Odd  IDs are in the low  order 32 bits.
	   * 5.5 Adjust dcr value to bits on the low order.
	   */
	  starting_bit = (j & 0x1) * 32; /* Either 0 or 32 */
	  dcr_value = dcr_value >> (32-starting_bit);  

	  /* 5.6 Extract the input and output bits we are interested in */
	  v_in_j  = (dcr_value >> 16)  & _KERNEL_COLL_CLASS_ROUTE_A_E_IO_BITS;
	  v_out_j = dcr_value          & _KERNEL_COLL_CLASS_ROUTE_A_E_IO_BITS;

	  /* 5.7 Test if this is the type of class route we are looking for */
	  if (  ((dcr_value >> 16) & BGQ_CLASS_INPUT_VC_MASK) == lookingFor )
	    {
	      /* 5.8 Do the check of i and j sharing links */
	      if  ( ( v_in | v_out ) & (v_in_j | v_out_j) )
		{
/* 		  printf("crid=%d, cr->input=0x%04x, cr->output=0x%04x,j=%u, dcr_value=0x%lx, lookingFor=0x%x, v_in=0x%04x, v_in_j=0x%04x, v_out=0x%04x, v_out_j=0x%04x\n",id,cr->input,cr->output,j,dcr_value,lookingFor,v_in,v_in_j,v_out,v_out_j); */
		  return 5;
		}
	    } /* End: Test matching class route */
	} /* End: Loop through existing class routes */
    } /* Done looking */

  /******* Done with consistency checks *******/

  /* Get the current value of the DCR. */
  dcr_value = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2));

  /* Determine the 32 bits corresponding to the specified ID.
   * - Even IDs are in the high order 32 bits
   * - Odd  IDs are in the low  order 32 bits.
   */
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Build the new value in the right position: 16 bits of input, 16 bits of output */
  new_value = 
    ((uint64_t)inBits  << (48 - starting_bit)) |
    ((uint64_t)cr->output << (32 - starting_bit));

  /* Zero out the previous bits, and OR the new value into the DCR */
  DCRWritePriv (ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2), 
		(dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) | new_value);      

/*   printf("Kernel_SetCollectiveClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
/* 	 (long long unsigned int)((dcr_value & (0x00000000FFFFFFFFULL << starting_bit)) | new_value)); */

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
  uint64_t dcr_value;
  uint64_t new_value;
  uint32_t starting_bit;

  MUSPI_assert ( id < BGQ_COLL_CLASS_MAX_CLASSROUTES );

  /* Get the current value of the DCR. */
  dcr_value = DCRReadPriv(ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2));

  /* Determine the 32 bits corresponding to the specified ID.
   * - Even IDs are in the high order 32 bits
   * - Odd  IDs are in the low  order 32 bits.
   */
  starting_bit = (id & 0x1) * 32; /* Either 0 or 32 */

  /* Derive new DCR value by zeroing the previous bits */
  new_value = dcr_value & (0x00000000FFFFFFFFULL << starting_bit);

  /* Set DCR value */
  DCRWritePriv ( ND_500_DCR(CTRL_COLL_CLASS_00_01)+(id/2), 
		 new_value );

/*   printf("Kernel_DeallocateCollectiveClassRoute: Updating DCR from old value 0x%llx to new value 0x%llx\n", */
/* 	 (long long unsigned int)dcr_value, */
    /* 	 (long long unsigned int)new_value); */

  return 0;
}

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

#endif /* _KERNEL_FIRMWARE_COLLECTIVE_IMPL_H_ */
