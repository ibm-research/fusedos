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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

// Includes
#include "Kernel.h"
#include <firmware/include/personality.h>
#include <firmware/include/personality_v7.h>

uint64_t sc_GET_PERSONALITY(SYSCALL_FCN_ARGS)
{
   Personality_t *dst  = (Personality_t *)r3;
   unsigned            size = r4;

   TRACESYSCALL(("(I) %s[%d]: dst=0x%016lx, size=%u.\n", __func__, ProcessorID(), (unsigned long)dst, size));

   // Check for error conditions.
   if ( !VMM_IsAppAddress(dst, size) )
      {
      return CNK_RC_SPI(EFAULT);
      }

   // Make the specified size matches the actual size.
   if ( size != sizeof(Personality_t) )
   {

       // Maintain N-1 compatibilty with the Version 7 personality.  This
       // is done by a manual copy of the new (Version 8) personality stucture
       // onto a V7 structure.

       if ( size == sizeof(Personality_v7_t) ) 
       {
	   Personality_v7_t* to = (Personality_v7_t*)dst;
	   Personality_t* from = GetPersonality();

	   memset( to, 0, sizeof(Personality_v7_t) );

	   to->CRC = from->CRC;
	   to->Version = 7;
	   to->PersonalitySizeWords = sizeof(Personality_v7_t)/4;

	   to->Kernel_Config.UCI = from->Kernel_Config.UCI;
	   to->Kernel_Config.NodeConfig = from->Kernel_Config.NodeConfig;
	   to->Kernel_Config.TraceConfig = from->Kernel_Config.TraceConfig;
	   to->Kernel_Config.RASPolicy = from->Kernel_Config.RASPolicy;
	   to->Kernel_Config.FreqMHz = from->Kernel_Config.FreqMHz;
	   to->Kernel_Config.ClockStop = from->Kernel_Config.ClockStop;

	   to->DDR_Config.DDRFlags = from->DDR_Config.DDRFlags;
	   to->DDR_Config.DDRSizeMB = from->DDR_Config.DDRSizeMB;

	   to->Network_Config.BlockID = from->Network_Config.BlockID;
	   to->Network_Config.NetFlags = from->Network_Config.NetFlags;
	   to->Network_Config.NetFlags2 = from->Network_Config.NetFlags2;
	   to->Network_Config.Anodes = from->Network_Config.Anodes;
	   to->Network_Config.Bnodes = from->Network_Config.Bnodes;
	   to->Network_Config.Cnodes = from->Network_Config.Cnodes;
	   to->Network_Config.Dnodes = from->Network_Config.Dnodes;
	   to->Network_Config.Enodes = from->Network_Config.Enodes;
	   to->Network_Config.Acoord = from->Network_Config.Acoord;
	   to->Network_Config.Bcoord = from->Network_Config.Bcoord;
	   to->Network_Config.Ccoord = from->Network_Config.Ccoord;
	   to->Network_Config.Dcoord = from->Network_Config.Dcoord;
	   to->Network_Config.Ecoord = from->Network_Config.Ecoord;

	   to->Network_Config.ClassRoutes[0].GlobIntUpPortInputs = from->Network_Config.PrimordialClassRoute.GlobIntUpPortInputs;
	   to->Network_Config.ClassRoutes[0].GlobIntUpPortOutputs = from->Network_Config.PrimordialClassRoute.GlobIntUpPortOutputs;
	   to->Network_Config.ClassRoutes[0].CollectiveTypeAndUpPortInputs = from->Network_Config.PrimordialClassRoute.CollectiveTypeAndUpPortInputs;
	   to->Network_Config.ClassRoutes[0].CollectiveUpPortOutputs = from->Network_Config.PrimordialClassRoute.CollectiveUpPortOutputs;
	   to->Network_Config.ZoneRoutingMasks[0] = from->Network_Config.ZoneRoutingMasks[0];
	   to->Network_Config.ZoneRoutingMasks[1] = from->Network_Config.ZoneRoutingMasks[1];
	   to->Network_Config.ZoneRoutingMasks[2] = from->Network_Config.ZoneRoutingMasks[2];
	   to->Network_Config.ZoneRoutingMasks[3] = from->Network_Config.ZoneRoutingMasks[3];
	   to->Network_Config.MuFlags = from->Network_Config.MuFlags;
	   to->Network_Config.cnBridge_A = from->Network_Config.cnBridge_A; 
	   to->Network_Config.cnBridge_B = from->Network_Config.cnBridge_B;
	   to->Network_Config.cnBridge_C = from->Network_Config.cnBridge_C;
	   to->Network_Config.cnBridge_D = from->Network_Config.cnBridge_D;
	   to->Network_Config.cnBridge_E = from->Network_Config.cnBridge_E;
	   to->Network_Config.latencyFromRoot = from->Network_Config.latencyFromRoot;

	   return CNK_RC_SPI(0);
       }
       else 
       {
	   return CNK_RC_SPI(EINVAL);
       }
   }

   // Copy the personality to the caller's buffer.
   memcpy( dst, GetPersonality(), size );

   return CNK_RC_SPI(0);
}
