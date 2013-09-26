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

#ifndef	_MUSPI_GI__H_ /* Prevent multiple inclusion */
#define	_MUSPI_GI__H_


/**
 * \file GI.h
 *
 * \brief C Header File containing Message Unit SPI Global Interrupt Definitions
 *
 * Global interrupts built upon the MU global interrupts are simply referred to
 * as Global Interrupts.
 *
 * For each global interrupt class route, each of the three bits in the status
 * register can be individually enabled to cause an interrupt when its value is
 * 1.  The interrupt will continue to fire until the bit value goes to 0.
 *
 * The initial value of the bit is 0.  When one node wants to send a global
 * interrupt to all of the other participating nodes defined by the class route,
 * it sets one of the bits in its control register to 1.  The OR of all of the
 * nodes contributions becomes a 1, as reflected in the status register on all
 * participating nodes, so the interrupt fires on all nodes (assuming it has
 * been enabled to fire - see Kernel_ConfigureGIInterrupts).  The status bit
 * will continue to be a 1 on all participating nodes, and the interrupt will
 * continue to fire on all nodes that have enabled the interrupt to fire, until
 * the sending node sets the bit in its control register back to 0.
 *
 * Example without using interrupts, using the first of the three bits:
 *
   \verbatim
  
  	                                     Node1               Node2
                                       ------------------  ------------------
                                       Control   Status    Control   Status
                                       Register  Register  Register  Register
                                       Pattern             Pattern
  
   Unknown state before initializaiton   100       100       100       100
  
   After all nodes do MUSPI_GIInitMU     000       000       000       000
  
   Send:  After Node1 does MUSPI_GISend  100       100       000       100
  
   Node2 uses a MUSPI_GIPoll function
     to detect the global interrupt
     arrival                             100       100       000       100
  
   All nodes somehow sync up with Node1
     so Node1 knows that it can clear
     the GI
  
   After Node1 does MUSPI_GISendClear    000       000       000       000
  
   Go to "Send" to issue more GIs...
  
   \endverbatim
  
   Example using interrupts, using the first bit:
  
   \verbatim
  
  	                                     Node1               Node2
                                       ------------------  ------------------
                                       Control   Status    Control   Status
                                       Register  Register  Register  Register
                                       Pattern             Pattern
  
   Unknown state before initialization   100       100       100       100
  
   After all nodes do MUSPI_GIInitMU     000       000       000       000
  
   Send:  After Node1 does MUSPI_GISend	 100       100       000       100
  
   Interrupt fires on all nodes.
     Within the interrupt handler, 
  
      - All nodes somehow sync up with
        Node1 so Node1 knows that it
        can clear the GI                 100       100       000       100
  
      - After Node1 does
        MUSPI_GISendClear                000       000       000       000
  
      - All nodes somehow sync up with
        Node1 so they know the interrupt
        has been cleared                 000       000       000       000
  
   All nodes exit the interrupt handler  000       000       000       000
  
   Go to "Send" to issue more GIs
  
   \endverbatim
  
   Example using GI Barrier after global interrupts on same class route:
  
   \verbatim
  
  	                                     Node1               Node2
                                       ------------------  ------------------
                                       Control   Status    Control   Status
                                       Register  Register  Register  Register
                                       Pattern             Pattern
  
   After using global interrupts on
     this class route                    000       000       000       000

   After MUSPI_GIBarrierInitMU           100       100       100       100

   Now GI Barrier interfaces can be
     used to perform barriers            100       100       100       100
   
   \endverbatim
 *
 * Usage Notes:
 *
 * To use a global interrupt, it must be initialized first.  There are three
 * initialization functions:
 *
 * 1. MUSPI_GIInitMU() - Puts the MU hardware in the initial global interrupt
 *    state.
 *
 * 2. Kernel_ConfigureGIInterrupts() - Enable an interrupt to fire when the
 *    global interrupt is sent.  This is optional.  The alternative is to
 *    call MUSPI_GIPoll() or MUSPI_GIPollWithTimeout() to check for the GI's
 *    arrival.
 *
 * 3. MUSPI_GIInit() - Initializes software so that the other GI Barrier
 *    SPIs can be used to send the GI and poll for its arrival.
 *
 * 4. After the initialization functions have been called, the following
 *    functions may be called to send the global interrupt and poll for its
 *    arrival:
 *    - MUSPI_GIEnter()
 *    - MUSPI_GIPoll()
 *    - MUSPI_GIPollWithTimeout()
 */

#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <spi/include/mu/Util.h>
#include <spi/include/kernel/memory.h>


__BEGIN_DECLS


/**
 * \brief GI Structure
 *
 * Structure holding information about the global interrupt.
 *
 */
typedef struct MUSPI_GI
{
  uint64_t           classRouteId;   /**< Class Route ID associated
                                          with the global interrupt */
  volatile uint64_t *controlRegPtr;  /**< Control Register Pointer */
  volatile uint64_t *statusRegPtr;   /**< Status  Register Pointer */
  uint64_t           mask;           /**< The mask containing the GI bit that is
                                          being used for this global interrupt */
} MUSPI_GI_t __attribute__ ((aligned ( 32)));


/**
 * \brief Global Interrupt Initialization Barrier Function Pointer
 *
 * This defines a type for the barrier function that MUSPI_GIInit() 
 * calls while it is initializing the global interrupt.  This barrier function
 * must use some kind of barrier to perform a barrier among at least the nodes
 * involved in the global interrupt being initialized.
 *
 * \param[in]  parm  A pointer to opaque storage.  This pointer was provided
 *                   on the call to MUSPI_GIInitMU().  This pointer
 *                   can be NULL.
 *
 * \retval  0  Successful
 * \retval  -1 Error.
 */
typedef int32_t (*MUSPI_GIInitMUBarrierFunction_t)( void *parm );


/**
 * \brief Global Interrupt Initialize Message Unit
 *
 * This function puts the Message Unit (MU) in the initial global interrupt
 * state for the specified class route and specified global interrupt within
 * that class route.  It must be called before any of the other
 * MUSPI_GI functions.
 *
 * \param [in]   classRouteId  The Id of the global interrupt class route to be
 *                             used for this global interrupt
 *                             (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 * \param [in]   GIId          The Id of the global interrupt to be used.
 *                             There are three Ids for each class route.
 *                             (0 ... BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE-1).
 * \param [in]   barrierFunction  A pointer to a function that is called by
 *                                MUSPI_GIInitMU() to synchronize the reset of
 *                                the MU global interrupt hardware.
 * \param [in]   barrierFunctionParm A pointer to opaque storage.  This
 *                                    pointer is passed to the barrierFunction.
 *                                    May be NULL.
 * \param [in]   timeoutInCycles  The maximum number of cycles to wait for the
 *                                GI to be initialized across the nodes.
 *
 * \retval  0  Successful.    The GI MU hardware has been initialized for the
 *                            specified class route and GI id.
 * \retval -1  Unsuccessful.  A timeout occurred while waiting for the global
 *                            interrupt to reach the initialized state, or the
 *                            barrierFunction failed.
 */
__INLINE__
int32_t MUSPI_GIInitMU ( 
			uint32_t                         classRouteId,
			uint32_t                         GIId,
			MUSPI_GIInitMUBarrierFunction_t  barrierFunction,
			void                            *barrierFunctionParm,
			uint64_t                         timeoutInCycles )
{
  int32_t  rc;
  uint64_t system;

  MUSPI_assert ( classRouteId < BGQ_GI_CLASS_MAX_CLASSROUTES );
  MUSPI_assert ( GIId < BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE );

  /* Determine whether we are running in "user" or "privileged" mode.
   * Set a "system" mask that will be used to mask the privileged bit in the
   * MU MMIO addresses.  Initialize a mask to assume it is for "system" use.
   * This will be used to mask against the privileged addresses.
   * So, for "system" use, it won't turn off the privileged bit.
   */
  system = 0xFFFFFFFFFFFFFFFFULL;
  if ( Kernel_GetPrivilegeMode() == 0 )
    system = ~PHYMAP_PRIVILEGEDOFFSET;

  /* Point to the first group's control registers, and offset into that by the
   * classRouteId to get the control and status register pointers.  The
   * registers are mirrored in all subgroups, so it does not matter which 
   * subgroup we use.
   */
  volatile uint64_t *controlRegPtr = (volatile uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,classRouteId) & system);
  volatile uint64_t *statusRegPtr  = (volatile uint64_t*)(BGQ_MU_GI_STATUS_OFFSET(0,classRouteId)  & system);

  /* Calculate the correct bit based on the specified GI id */
  uint64_t mask           = 0x4 >> (GIId);

  /* Step 1: Perform barrier */
  rc = barrierFunction( barrierFunctionParm );
  if ( rc ) return rc;

  /* Step 2: Reset the control register to the initial state by turning off the
   * bit corresponding to the GI Id.
   */
  *controlRegPtr = *controlRegPtr & ~(mask);
      
  /* Step 3: Perform barrier */
  rc = barrierFunction( barrierFunctionParm );
  if ( rc ) return rc;

  /* Step 4: Read the status register until it is in the initial state
   * or until a timeout occurs */
  uint64_t startTime = GetTimeBase();

  rc = -1;
  
  do {
    if ( (*statusRegPtr & mask) == 0 )
      {
	rc = 0;
	break;
      }
  } while ( (GetTimeBase() - startTime) < timeoutInCycles );
  
  /* Step 5: If no timeout, barrier one final time */
  if ( rc == 0 )
    {
      rc = barrierFunction( barrierFunctionParm );
    }
  
  return rc;
}


/**
 * \brief Global Interrupt Initialization
 *
 * This function fills in the GI structure which is needed on subsequent calls
 * to use this global interrupt.
 *
 * \param [out]  GI            Pointer to the structure that is filled in by
 *                             this function with information needed to use a
 *                             global interrupt on the specified class route.
 * \param [in]   classRouteId  The Id of the global interrupt class route to be
 *                             used for this global interrupt
 *                             (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 * \param [in]   GIId          The Id of the global interrupt to be used.
 *                             There are three Ids for each class route.
 *                             (0 ... BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE-1).
 *
 * \retval  0  Successful.    The GI structure has been initialized.
 * \retval -1  Unsuccessful. 
 */
__INLINE__
int32_t MUSPI_GIInit ( MUSPI_GI_t                    *GI,
		       uint32_t                       classRouteId,
		       uint32_t                       GIId )
{
  uint64_t system;

  MUSPI_assert ( GI != NULL );
  MUSPI_assert ( classRouteId < BGQ_GI_CLASS_MAX_CLASSROUTES );
  MUSPI_assert ( GIId < BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE );

  /* Determine whether we are running in "user" or "privileged" mode.
   * Set a "system" mask that will be used to mask the privileged bit in the
   * MU MMIO addresses.  Initialize a mask to assume it is for "system" use.
   * This will be used to mask against the privileged addresses.
   * So, for "system" use, it won't turn off the privileged bit.
   */
  system = 0xFFFFFFFFFFFFFFFFULL;
  if ( Kernel_GetPrivilegeMode() == 0 )
    system = ~PHYMAP_PRIVILEGEDOFFSET;

  /* Point to the first group's control registers, and offset into that by the
   * classRouteId to get the control and status register pointers.  The
   * registers are mirrored in all subgroups, so it does not matter which 
   * subgroup we use.
   */
  GI->controlRegPtr = (uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,classRouteId) & system);
  GI->statusRegPtr  = (uint64_t*)(BGQ_MU_GI_STATUS_OFFSET(0,classRouteId) & system);

  GI->classRouteId  = classRouteId; /* Save the class route id */

  /* Set the bit corresponding to the global interrupt Id */
  GI->mask          = 0x4 >> (GIId);

  return 0;
}


/**
 * \brief Send a Global Interrupt
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 *
 * \retval  0  Successful.  Global interrupt has been sent.
 * \retval -1  Unsuccessful.
 *
 */
__INLINE__
int32_t MUSPI_GISend ( MUSPI_GI_t *GI )
{
  /* Send the global interrupt by turning on the bit corresponding to the
   * GI Id.
   */
  *GI->controlRegPtr = *GI->controlRegPtr | GI->mask;
  
  return 0;  
}


/**
 * \brief Clear a Global Interrupt
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 *
 * \retval  0  Successful.  Global interrupt has been cleared.
 * \retval -1  Unsuccessful.
 *
 */
__INLINE__
int32_t MUSPI_GISendClear ( MUSPI_GI_t *GI )
{
  /* Send the global interrupt clear by turning off the bit corresponding to the
   * GI Id.
   */
  *GI->controlRegPtr = *GI->controlRegPtr & ~(GI->mask);
  
  return 0;  
}


/**
 * \brief Poll for a Global Interrupt Arrival
 *
 * Poll the specified global interrupt for arrival.  This function blocks until
 * the global interrupt arrives.
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 *
 * \retval  0  Successful.  Global interrupt has arrived.
 * \retval -1  Unsuccessful.  Global interrupt has not arrived.
 *
 */
__INLINE__
int32_t MUSPI_GIPollForArrival ( MUSPI_GI_t *GI )
{
  uint64_t mask = GI->mask;
  uint64_t statusRegValue = *GI->statusRegPtr;

  /* The "mask" contains the single bit that we are focusing on.
   * If that bit is ON in the status register, then the global interrupt has
   * arrived.
   * If that bit is OFF, then the global interrupt has not arrived.
   */
/*   printf("MUSPI_GIPollForArrival: statusRegister = 0x%016lx\n",statusRegValue); */

  if ( statusRegValue & mask )
    return 0;
  else
    return -1;
}


/**
 * \brief Poll for a Global Interrupt Arrival with Timeout
 *
 * Poll the specified global interrupt for its arrival.  Block up to the
 * specified number of cycles while waiting for the global interrupt arrival
 * before timing out and returning an error.
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 * \param [in]  timeoutInCycles  Number of cycles to wait for the global
 *                               interrupt to arrive.
 *
 * \retval  0  Successful.  Global interrupt has arrived.
 * \retval -1  Unsuccessful.  Global interrupt did not arrive within the
 *                            specified timeout.
 *
 */
__INLINE__
int32_t MUSPI_GIPollForArrivalWithTimeout ( MUSPI_GI_t *GI,
					    uint64_t    timeoutInCycles )
{
  uint64_t statusRegValue;
  uint64_t diffTime;

  uint64_t mask = GI->mask;

  int32_t  rc = -1; /* Init to "timeout" error */

  uint64_t startTime = GetTimeBase();
  
  do {

    /* The "mask" contains the single bit that we are focusing on.
     * If that bit is ON in the status register, then the global interrupt has
     * arrived.
     * If that bit is OFF, then the global interrupt has not arrived.
     */
    statusRegValue = *GI->statusRegPtr;

/*     printf("MUSPI_GIPollForArrivalWithTimeout: statusRegister = 0x%016lx\n",statusRegValue); */

    if ( statusRegValue & mask )
      {
	rc = 0; /* Global interrupt arrived */
	break;
      }
    
    diffTime = GetTimeBase() - startTime;

  } while ( diffTime < timeoutInCycles );

  return rc;
}


/**
 * \brief Poll for a Global Interrupt Being Cleared
 *
 * Poll the specified global interrupt for being cleared.  This function blocks
 * until the global interrupt clears.
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 *
 * \retval  0  Successful.  Global interrupt has cleared.
 * \retval -1  Unsuccessful.  Global interrupt has not cleared.
 *
 */
__INLINE__
int32_t MUSPI_GIPollForClear ( MUSPI_GI_t *GI )
{
  uint64_t mask = GI->mask;
  uint64_t statusRegValue = *GI->statusRegPtr;

  /* The "mask" contains the single bit that we are focusing on.
   * If that bit is ON in the status register, then the global interrupt
   * is not clear.
   * If that bit is OFF, then the global interrupt is clear.
   */
/*   printf("MUSPI_GIPollForClear: statusRegister = 0x%016lx\n",statusRegValue); */

  if ( (statusRegValue & mask) == 0 )
    return 0;
  else
    return -1;
}


/**
 * \brief Poll for a Global Interrupt Being Cleared with Timeout
 *
 * Poll the specified global interrupt for being cleared.  Block up to the
 * specified number of cycles while waiting for the global interrupt to
 * clear before timing out and returning an error.
 *
 * \param [in]  GI  Pointer to the structure that contains information
 *                  about this global interrupt.  Previously filled in by
 *                  MUSPI_GIInit().
 * \param [in]  timeoutInCycles  Number of cycles to wait for the global
 *                               interrupt to clear.
 *
 * \retval  0  Successful.  Global interrupt has cleared.
 * \retval -1  Unsuccessful.  Global interrupt did not clear within the
 *                            specified timeout.
 *
 */
__INLINE__
int32_t MUSPI_GIPollForClearWithTimeout ( MUSPI_GI_t *GI,
					  uint64_t    timeoutInCycles )
{
  uint64_t statusRegValue;
  uint64_t diffTime;

  uint64_t mask = GI->mask;

  int32_t  rc = -1; /* Init to "timeout" error */

  uint64_t startTime = GetTimeBase();
  
  do {

    /* The "mask" contains the single bit that we are focusing on.
     * If that bit is ON in the status register, then the global interrupt has
     * not cleared.
     * If that bit is OFF, then the global interrupt has cleared.
     */
    statusRegValue = *GI->statusRegPtr;

/*     printf("MUSPI_GIPollForClearWithTimeout: statusRegister = 0x%016lx\n",statusRegValue); */

    if ( (statusRegValue & mask) == 0 )
      {
	rc = 0; /* Global interrupt cleared */
	break;
      }
    
    diffTime = GetTimeBase() - startTime;

  } while ( diffTime < timeoutInCycles );

  return rc;
}


__END_DECLS


#endif /* _MUSPI_GI_H_  */
