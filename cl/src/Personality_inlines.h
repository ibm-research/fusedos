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

#ifndef _PERSONALITY_INLINES_H_ // Prevent multiple inclusion.
#define _PERSONALITY_INLINES_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdint.h>

#include <firmware/include/personality.h>
#include <cnk/include/Config.h>

extern NodeState_t NodeState;

__INLINE__ uint64_t Pers_GetTraceConfig( void )
{
//   return(-1);
   return(0);
}

__INLINE__ int Personality_IsMambo( void )
{
    return ((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_Mambo) ? 1 : 0);
}

__INLINE__ int Personality_IsFPGA( void )
{
    return ((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_FPGA) ? 1 : 0);
}

__INLINE__ int Personality_IsCycleSim( void )
{
    return((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_Simulation) ? 1 : 0);
}

__INLINE__ int Personality_IsSimulation( void )
{
    return ((Personality_IsMambo() || Personality_IsFPGA() || Personality_IsCycleSim()) ? 1 : 0);
}

__INLINE__ int Personality_CiosEnabled( void )
{
    return((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_IOServices) ? 1 : 0);
}

__INLINE__ int Personality_ApplicationPreLoaded( void )
{
    return ((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_AppPreload) ? 1 : 0);
}

__INLINE__ int Personality_IsJTagConsole( void )
{
    return ((GetPersonality()->Kernel_Config.NodeConfig & PERS_ENABLE_JTagConsole) ? 1 : 0);
}

#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.
