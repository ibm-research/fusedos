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

#ifndef	_MUSPI_GI_BARRIER_H_ /* Prevent multiple inclusion */
#define	_MUSPI_GI_BARRIER_H_


/**
 * \file GIBarrier.h
 *
 * \brief C Header File containing Message Unit SPI Global Interrupt Barrier Definitions
 *
 * Barriers built upon the MU Global Interrupts are referred to as Global
 * Interrupt Barriers (GI barriers).
 *
 * For each global interrupt class route, the MU has a control register and a
 * status register.  Each register contains 3 bits.  The control register
 * represents this node's contribution.  The status register contains an OR of
 * each bit across all participating nodes defined by the class route.
 *
 * For each class route, there are 3 barrier states:  '100', '010', and '001'
 * corresponding to each bit in the registers.  The control register is set to
 * one of three patterns: '100', '010', '001', also corresponding to each state.
 * The barrier is considered complete when the bit corresponding to the current
 * state is 0 in the status register.
 *
 * The initial barrier state is '100', which is established when the barrier is
 * initialized.  This initialization must use the following technique.  Note
 * that to initialize this GI Barrier, another barrier across the same (or a
 * superset) of the nodes is used.
 * 1. Barrier across the nodes involved in the barrier being initialized.
 * 2. Set the control register to the initial pattern '100'.
 * 3. Barrier.
 * 4. Read the status register until it has the initial pattern '100'.
 * 5. Barrier.
 * The GI Barrier that was initialized is now in a "not complete" state because
 * the first bit in the status register is still a 1.
 *
 * When a barrier is entered on a particular node, the NEXT pattern (round-robin
 * among the three patterns) is set into the control register.  For example, the
 * barrier state is '100', so pattern 2 '010' is set into the control register.
 * This has two effects:
 * 1. It notifies all nodes that our node has entered the barrier.  The first
 *    bit has changed to 0 for our node.
 * 2. It notifies all nodes that our node has not entered the NEXT barrier.  The
 *    second bit is a 1 for our node.
 *
 * To check for completion of the barrier, the status register bit corresponding
 * to the barrier state is monitored until it becomes 0, and the barrier is
 * considered complete.  For example, the barrier state is '100'; after all nodes
 * have entered the barrier and written pattern '010' to their control register,
 * the logical OR of the first bit across all nodes becomes 0, completing the
 * barrier.  After the barrier completes, the barrier moves to the next state/bit
 * (round-robin among the states/bits).
 *
 * Here's an example of 2 nodes using a GI barrier.
 *
   \verbatim
 
                           ---------- Node1 ---------  --------- Node2 ---------
                           State   Control   Status    State  Control   Status
                                   Register  Register         Register  Register
                                   Pattern                    Pattern

   After initialization    100     100       100       100    100       100

   Node 1 enters barrier1  100     010       110       100    100       110

   Node 2 enters barrier1  100     010       010       100    010       010

   Node 1 checks and finds
     barrier1 complete     010     010       010       100    010       010

   Node 2 checks and finds
     barrier1 complete     010     010       010       010    010       010

   Node 1 enters barrier2  010     001       011       010    010       011

   Node 2 enters barrier2  010     001       001       010    001       001

   Node 2 checks and finds
     barrier2 complete     010     001       001       001    001       001

   Node 2 enters barrier3  010     001       101       001    100       101

   Node 1 checks and finds
     barrier2 complete     001     001       101       001    100       101

   Node 1 enters barrier3  001     100       100       001    100       100

   Node 2 checks and finds
     barrier3 complete     001     100       100       100    100       100

   Node 1 checks and finds
     barrier3 complete     100     100       100       100    100       100

   \endverbatim

 *
 * Usage Notes:
 *
 * To use a GI Barrier, it must be initialized first.  There are two
 * initialization functions that must be called:
 *
 * 1. MUSPI_GIBarrierInitMU() - Puts the MU hardware in the initial barrier
 *    state.  Note that the kernel calls this function to initialize a special
 *    barrier across all nodes before the application starts.
 *    Kernel_GetGlobalBarrierClassRouteId() returns the class route id of this
 *    barrier.  In this case, this function should not be called by user mode
 *    for that barrier.  Instead, just MUSPI_GIBarrierInit() needs to be called
 *    to complete the software initialization of the barrier.
 *
 * 2. MUSPI_GIBarrierInit() - Initializes software so that the other GI Barrier
 *    SPIs can be used to enter the barrier and poll for its completion.
 *
 * 3. After the two initialization functions have been called, the following
 *    functions may be called to enter the barrier and poll for its completion:
 *    - MUSPI_GIBarrierEnter()
 *    - MUSPI_GIBarrierPoll()
 *    - MUSPI_GIBarrierPollWithTimeout()
 */

#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/classroute.h>
//#include <hwi/include/bqc/A2_inlines.h>
#include <spi/include/mu/Util.h>
#include <spi/include/kernel/memory.h>


__BEGIN_DECLS


/**
 * \brief GI Barrier Structure
 *
 * Structure holding information about the global interrupt barrier.
 * Align it to a 32-byte boundary so it does not cross a cache line.
 *
 */
typedef struct MUSPI_GIBarrier
{
  uint64_t           classRouteId;   /**< Class Route ID associated
                                          with the barrier.        */
  volatile uint64_t *controlRegPtr;  /**< Control Register Pointer */
  volatile uint64_t *statusRegPtr;   /**< Status  Register Pointer */
  uint64_t           state;          /**< The state of the barrier.  One of
                                          - '100'
                                          - '010'
                                          - '001' */
} MUSPI_GIBarrier_t __attribute__ ((aligned ( 32)));


/**
 * \brief Global Interrupt Barrier Initialization Barrier Function Pointer
 *
 * This defines a type for the barrier function that MUSPI_GIBarrierInit() 
 * calls while it is initializing the GI Barrier.  This barrier function must
 * use some other barrier to perform a barrier among at least the nodes
 * involved in the GI Barrier being initialized.
 *
 * \param[in]  parm  A pointer to opaque storage.  This pointer was provided
 *                   on the call to MUSPI_GIBarrierInitMU().  This pointer
 *                   can be NULL.
 *
 * \retval  0  Successful
 * \retval  -1 Error.
 */
typedef int32_t (*MUSPI_GIBarrierInitMUBarrierFunction_t)( void *parm );


/**
 * \brief GI Barrier Initial State
 *
 * This is equivalent to binary 100, which is the initial state for
 * a GI barrier.
 */
#define MUSPI_GIBARRIER_INITIAL_STATE 0x4


/**
 * \brief Stage 1 of Global Interrupt Barrier Initialize Message Unit
 *
 * This function is intended for use in building the initialization sequence
 * for a classroute on the MU Global Interrupt Barrier. Synchronization is
 * required before and after both (MUSPI_GIBarrierInitMU1 and MUSPI_GIBarrierInitMU2).
 * See MUSPI_GIBarrierInitMU for an example. The sequence is:
 * 
 *	synchronize (real or implied)
 *	MUSPI_GIBarrierInitMU1(classRouteId);
 *	synchronize
 *	MUSPI_GIBarrierInitMU2(classRouteId, timeout);
 *	synchronize (real or implied)
 *
 * In each case, "synchronize" means that all nodes contained in a classroute
 * must barrier at the points indicated.
 *
 * The reason each step is exposed separately is to permit a non-blocking approach
 * or for cases where the first and/or last synchronization is inherent in the
 * caller's code.
 *
 * This function sets up a classroute's GI Barrier initial state register.
 * All participants in the classroute must have this register set before
 * *any* participant can query the state (MUSPI_GIBarrierInitMU2).
 *
 * \param [in]  classRouteId         The Id of the global interrupt class route
 *                                   to be used for this global interrupt
 *                                   barrier
 *                                   (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.    The GIBarrier MU initial state has been reset for
 *&                           the specified class route.
 * \retval -1  Unsuccessful.
 */
__INLINE__
int32_t MUSPI_GIBarrierInitMU1 ( uint32_t classRouteId )
{
  uint64_t  system;
  volatile uint64_t *controlRegPtr;

  MUSPI_assert ( classRouteId < BGQ_GI_CLASS_MAX_CLASSROUTES );

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
  controlRegPtr = (uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,classRouteId) & system);

  /* Step 2: Reset the control register to the initial state */
  *controlRegPtr = MUSPI_GIBARRIER_INITIAL_STATE;

  return 0;
}

/**
 * \brief Stage 2 of Global Interrupt Barrier Initialize Message Unit
 *
 * \see MUSPI_GIBarrierInitMU1
 *
 * This function queries the classroute GI Barrier status until it reaches the
 * "initial state". No participants can begin a barrier on this classroute until
 * *all* participants have reached the "initial state" status.
 *
 * \param [in]  classRouteId         The Id of the global interrupt class route
 *                                   to be used for this global interrupt
 *                                   barrier
 *                                   (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 * \param [in]  timeoutInCycles      The maximum number of cycles to wait for
 *                                   the GI Barrier to be initialized across
 *                                   the nodes.
 *
 * \retval  0  Successful.    The GIBarrier MU initial state has been reached for
 *&                           the specified class route.
 * \retval -1  Unsuccessful.  A timeout occurred while waiting for the GIBarrier
 *                            to reach the initialized state.
 */
__INLINE__
int32_t MUSPI_GIBarrierInitMU2 ( uint32_t classRouteId, uint64_t timeoutInCycles )
{
  int32_t   rc;
  uint64_t  system;
  volatile uint64_t *statusRegPtr;
  uint64_t  startTime;

  MUSPI_assert ( classRouteId < BGQ_GI_CLASS_MAX_CLASSROUTES );

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
  statusRegPtr  = (uint64_t*)(BGQ_MU_GI_STATUS_OFFSET(0,classRouteId)  & system);

  /* Step 4: Read the status register until it is in the initial state
   * or until a timeout occurs */

  rc = -1; /* Init to "timeout" error */

  startTime = GetTimeBase();

  do {
    if ( (*statusRegPtr & 0x7) == MUSPI_GIBARRIER_INITIAL_STATE )
      {
	rc = 0;
	break;
      }
  } while ( (GetTimeBase() - startTime) < timeoutInCycles );
  
  return rc;
}

/**
 * \brief Self-synchronizing Global Interrupt Barrier Initialize Message Unit
 *
 * This function puts the Message Unit (MU) in the initial barrier state for the
 * specified class route.  It must be called before any of the other
 * MUSPI_GIBarrier functions.
 *
 * \see MUSPI_GIBarrierInitMU1
 * \see MUSPI_GIBarrierInitMU2
 *
 * Caller must provide a barrier/synchronize function (that DOES NOT use this
 * GI Barrier classroute) in order to synchronize between stages. This function
 * is blocking. For situations that require non-block behavior, or already have
 * (real or implicit) synchronization before and/or after, the caller may use
 * the stages MUSPI_GIBarrierInitMU1 and MUSPI_GIBarrierInitMU2 directly - provided
 * there is sufficient synchronization between the stages as well.
 *
 * \param [in]  classRouteId         The Id of the global interrupt class route
 *                                   to be used for this global interrupt
 *                                   barrier
 *                                   (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 * \param [in]  barrierFunction      A pointer to a function that is called by
 *                                   MUSPI_GIBarrierInitMU() to synchronize the
 *                                   initialization of the MU barrier hardware.
 *                                   This function returns a return code.  If
 *                                   this return code is not 0, 
 *                                   MUSPI_GIBarrierInitMU() will stop and
 *                                   return that error as its return code.
 * \param [in]  barrierFunctionParm  A pointer to opaque storage.  This pointer 
 *                                   is passed to the barrierFunction.  May be
 *                                   NULL.
 * \param [in]  timeoutInCycles      The maximum number of cycles to wait for
 *                                   the GI Barrier to be initialized across
 *                                   the nodes.
 *
 * \retval  0  Successful.    The GIBarrier MU hardware has been initialized for
 *&                           the specified class route.
 * \retval -1  Unsuccessful.  A timeout occurred while waiting for the GIBarrier
 *                            to reach the initialized state, or the 
 *                            barrierFunction failed.
 */
__INLINE__
int32_t MUSPI_GIBarrierInitMU ( 
		uint32_t                                classRouteId,
                MUSPI_GIBarrierInitMUBarrierFunction_t  barrierFunction,
		void                                   *barrierFunctionParm,
                uint64_t                                timeoutInCycles )
{
  int32_t   rc;

  /* Step 1: Perform barrier */
  rc = barrierFunction( barrierFunctionParm );
  if ( rc ) return rc;

  /* Step 2: Reset the control register to the initial state */
  rc = MUSPI_GIBarrierInitMU1(classRouteId);
  if ( rc ) return rc;
      
  /* Step 3: Perform barrier */
  rc = barrierFunction( barrierFunctionParm );
  if ( rc ) return rc;

  /* Step 4: Read the status register until it is in the initial state
   * or until a timeout occurs */
  
  rc = MUSPI_GIBarrierInitMU2(classRouteId, timeoutInCycles);

  /* Step 5: If no timeout, barrier one final time */
  if ( rc == 0 )
    {
      rc = barrierFunction( barrierFunctionParm );
    }

  return rc;
}


/**
 * \brief Global Interrupt Barrier Initialization
 *
 * This function fills in the GIBarrier structure which is needed on subsequent
 * calls to use this barrier.
 *
 * \param [out]  GIBarrier     Pointer to the structure that is filled in by
 *                             this function with information needed to use a
 *                             global interrupt barrier on the specified class
 *                             route.
 * \param [in]   classRouteId  The Id of the global interrupt class route to be
 *                             used for this global interrupt barrier
 *                             (0 through BGQ_GI_CLASS_MAX_CLASSROUTES-1).
 *
 * \retval  0  Successful.    The GIBarrier structure has been initialized.
 * \retval -1  Unsuccessful.
 */
__INLINE__
int32_t MUSPI_GIBarrierInit ( 
                MUSPI_GIBarrier_t                    *GIBarrier,
		uint32_t                              classRouteId )
{
  uint64_t system;

  MUSPI_assert ( GIBarrier != NULL );
  MUSPI_assert ( classRouteId < BGQ_GI_CLASS_MAX_CLASSROUTES );

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
  GIBarrier->controlRegPtr = (uint64_t*)(BGQ_MU_GI_CONTROL_OFFSET(0,classRouteId) & system);
  GIBarrier->statusRegPtr  = (uint64_t*)(BGQ_MU_GI_STATUS_OFFSET(0,classRouteId)  & system);

  GIBarrier->classRouteId  = classRouteId; /* Save the class route id */
  
  /* Set the state based on the current contents of the control register */
  GIBarrier->state = *(GIBarrier->controlRegPtr) & 0x7;

  return 0;
}


/**
 * \brief Enter a Global Interrupt Barrier
 *
 * Enter the specified barrier.  This function is non-blocking and must be
 * followed by a call to one of the MUSPI_GIBarrierPoll functions to determine
 * when the barrier is complete.
 *
 * \param [in]  GIBarrier  Pointer to the structure that contains information
 *                         about this barrier.  Previously filled in by
 *                         MUSPI_GIBarrierInit().
 *
 * \retval  0  Successful.  Barrier has been entered.
 * \retval -1  Unsuccessful.
 *
 */
__INLINE__
int32_t MUSPI_GIBarrierEnter ( MUSPI_GIBarrier_t *GIBarrier )
{
  uint64_t nextPattern = GIBarrier->state >> 1; /* Shift to 'next' pattern    */

  /* Start back at initial pattern */
  if ( nextPattern == 0 ) nextPattern = MUSPI_GIBARRIER_INITIAL_STATE; 

  *GIBarrier->controlRegPtr = nextPattern;   /* Enter the barrier by storing
						the 'next' pattern to the
						control register              */
  
  return 0;  
}


/**
 * \brief Poll a Global Interrupt Barrier
 *
 * Poll the specified barrier for completion.  This function blocks until
 * the barrier has been entered on all other nodes defined by the class route.
 *
 * \param [in]  GIBarrier  Pointer to the structure that contains information
 *                         about this barrier.  Previously filled in by
 *                         MUSPI_GIBarrierInit().
 *
 * \retval  0  Successful.  Barrier has been entered on all nodes.
 * \retval -1  Unsuccessful.  The barrier has not been entered on all nodes.
 *
 */
__INLINE__
int32_t MUSPI_GIBarrierPoll ( MUSPI_GIBarrier_t *GIBarrier )
{
  uint64_t state = GIBarrier->state;
  uint64_t nextPattern;
  uint64_t statusRegValue = *GIBarrier->statusRegPtr;

  /* The "state" contains the single bit that we are focusing on.
   * If that bit is ON in the status register, then the barrier is NOT complete.
   * If that bit is OFF, then the barrier is complete.
   */
/*   printf("MUSPI_GIBarrierPoll: statusRegister = 0x%016lx\n",statusRegValue); */

  if ( statusRegValue & state )
    return -1;
  else
    {
      /* Calculate the next pattern and update the state with it.
       * This prepares for the next instance of the barrier.
       */
      nextPattern = state >> 1; /* Shift to next pattern      */

      /* Start back at initial pattern */
      if ( nextPattern == 0 ) nextPattern = MUSPI_GIBARRIER_INITIAL_STATE; 

      GIBarrier->state = nextPattern;
      
      return 0;
    }
}


/**
 * \brief Poll a Global Interrupt Barrier with Timeout
 *
 * Poll the specified barrier for completion.  Block up to the specified number
 * of cycles while waiting for the barrier completion before timing out and
 * returning an error.
 *
 * \param [in]  GIBarrier  Pointer to the structure that contains information
 *                         about this barrier.  Previously filled in by
 *                         MUSPI_GIBarrierInit().
 * \param [in]  timeoutInCycles  Number of cycles to wait for the barrier to
 *                               complete.
 *
 * \retval  0  Successful.  Barrier has been entered on all nodes.
 * \retval -1  Unsuccessful.  Barrier did not complete within the specified
 *                            timeout.
 *
 */
__INLINE__
int32_t MUSPI_GIBarrierPollWithTimeout ( MUSPI_GIBarrier_t *GIBarrier,
                                         uint64_t           timeoutInCycles )
{
  uint64_t statusRegValue;
  uint64_t diffTime;
  uint64_t nextPattern;
  uint64_t state = GIBarrier->state;

  int32_t  rc = -1; /* Init to "timeout" error */

  uint64_t startTime = GetTimeBase();
  
  do {

    /* The "state" contains the single bit that we are focusing on.
     * If that bit is ON in the status register, then the barrier is NOT complete.
     * If that bit is OFF, then the barrier is complete.
     */
    statusRegValue = *GIBarrier->statusRegPtr;

/*     printf("MUSPI_GIBarrierPollWithTimeout: statusRegister = 0x%016lx\n",statusRegValue); */

    if ( ( statusRegValue & state ) == 0 )
      {
	rc = 0; /* Barrier completed */

	/* Calculate the next pattern and update the state with it.
	 * This prepares for the next instance of the barrier.
	 */
	nextPattern = state >> 1; /* Shift to next pattern      */

	/* Start back at initial pattern */
	if ( nextPattern == 0 ) nextPattern = MUSPI_GIBARRIER_INITIAL_STATE; 

	GIBarrier->state = nextPattern;

	break;
      }
    
    diffTime = GetTimeBase() - startTime;

  } while ( diffTime < timeoutInCycles );

  return rc;
}


/**
 * \brief Enter and Wait on a Global Interrupt Barrier
 *
 * Enter the specified barrier and wait for its completion.  This function
 * blocks until the barrier has been entered on all other nodes defined by the
 * class route.
 *
 * \param [in]  GIBarrier  Pointer to the structure that contains information
 *                         about this barrier.  Previously filled in by
 *                         MUSPI_GIBarrierInit().
 *
 * \retval  0  Successful.  Barrier has been entered on all nodes.
 * \retval -1  Unsuccessful.  An error occurred.
 *
 */
__INLINE__
int32_t MUSPI_GIBarrierEnterAndWait ( MUSPI_GIBarrier_t *GIBarrier )
{
  int32_t rc;

  rc = MUSPI_GIBarrierEnter ( GIBarrier );

  if ( rc ) return rc;

  rc = -1;
  while ( rc != 0 )
    {
      rc = MUSPI_GIBarrierPoll ( GIBarrier );
    }

  return rc;
}


/**
 * \brief Enter and Wait on a Global Interrupt Barrier with Timeout
 *
 * Enter the specified barrier and wait for its completion.  Block up to the
 * specified number of cycles while waiting for the barrier completion before
 * timing out and returning an error.
 *
 * \param [in]  GIBarrier  Pointer to the structure that contains information
 *                         about this barrier.  Previously filled in by
 *                         MUSPI_GIBarrierInit().
 * \param [in]  timeoutInCycles  Number of cycles to wait for the barrier to
 *                               complete.
 *
 * \retval  0  Successful.  Barrier has been entered on all nodes.
 * \retval -1  Unsuccessful.  Barrier did not complete within the specified
 *                            timeout.
 *
 */
__INLINE__
int32_t MUSPI_GIBarrierEnterAndWaitWithTimeout ( MUSPI_GIBarrier_t *GIBarrier,
						 uint64_t           timeoutInCycles )
{
  int32_t rc;
  
  rc = MUSPI_GIBarrierEnter ( GIBarrier );
  
  if ( rc ) return rc;

  rc = MUSPI_GIBarrierPollWithTimeout ( GIBarrier, timeoutInCycles );

  return rc;
}


__END_DECLS


#endif /* _MUSPI_GI_BARRIER_H_  */
