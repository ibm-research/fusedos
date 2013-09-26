/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */
#define ROQ_BURST_NEW_COMPLETION_PATH 1
#define ROQ_BURST_ACKED 0
#define ROQ_BURST_IN_PROGRESS 1
#define ROQ_BURST_COMPLETE 2


#include <bgq_hw_abstraction.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <asm/types.h>

#ifdef USERSPACE_ROQ
#include <execinfo.h>
#include <signal.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/process.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/MU.h>
#define _NEED_ROQ_MAGIC_LOCATION
#include <roq_device_interface.h>
#endif /* USERSPACE_ROQ */

#include <transcon.h>
#include <roq_fw.h>
#include <SRSW_QueueSet.h>

#include <roq_kernel.h>

#include <hwi/include/bqc/gea_dcr.h>
#include <hwi/include/bqc/mu_dcr.h>

#if defined(USERSPACE_ROQ)
#include <sys/mman.h>
#include <unistd.h>
#endif

#if defined(PKTRACE_ON)
#include <roq_fw_trace.h>
#endif

#define FXLOG_REMOTE_GET_DCR 0

extern void display_mu_registers(void);

enum {
	MaxDmaGet = 16384
};

static struct {
 	volatile uint64_t	Pending[MaxDmaGet] __attribute__((aligned(8)));
	MUHWI_Descriptor_t	DirectPutDescriptor[MaxDmaGet];
	MUHWI_Descriptor_t	RemoteGetDescriptor[MaxDmaGet];
        unsigned int		OldestCounter;
        unsigned int		FreeCounter;

	/* associates a counter with ack/completion infrastructure */
	struct RoQ_TransportConnection	*TranConn[MaxDmaGet];
	int				BurstAckDescriptorIndex[MaxDmaGet];
} DmaGetCounter;

void init_DmaGetCounters(void)
{
        int i;

	/* 0 is done, -1 is not valid, > 0 valid working */
	for (i = 0; i < MaxDmaGet; i++)
		 DmaGetCounter.Pending[i] = -1;

	DmaGetCounter.OldestCounter = 0;
	DmaGetCounter.FreeCounter = 0;
}

static char RoQ_Trashcan[1024 * 64];


static inline uint64_t virt_to_dma(uint64_t virtual_address)
{
	return domain_physddr_start() + (virtual_address & ~ROQ_VIRTUAL_MAPPING);
}

static inline void create_descriptor(Personality_t *pers,
			MUHWI_Descriptor_t  *DirectPutDescriptor,
			MUHWI_Descriptor_t  *RemoteGetDescriptor,
			uint64_t             CounterPAddr,
   			MUHWI_Destination_t  PeerNodeAddr,
   			uint64_t receiveBufPAddr,
      			uint64_t sendBufPAddr,
         		uint64_t messageSizeInBytes) 		
{
	uint64_t FifoMap;

	/* zone routing bits */
	uint8_t		zoneRoutingMask = 0;

	/* stay on bubble bits */
	uint8_t		stayOnBubbleMask  = 0;

	uint32_t	rgfifo;

	MUSPI_Pt2PtRemoteGetDescriptorInfo_t GetDesc;
	MUSPI_Pt2PtDirectPutDescriptorInfo_t PutDesc;

	int rc;

	static uint32_t RoundRobinCount;
	const int	do_dynamic = 1;

	/*
	 * Enable all five Torus dimensions for Compute Nodes but
	 * only the three available dimensions for IO Nodes.
	 */
	FifoMap = personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode ?
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP
		:
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;

	/*
	 * Populate Injection Direct Put Descriptor Information Structure
	 */
	memset((void *)&PutDesc, 0, sizeof PutDesc);

	DPRINTF(FXLOG_REMOTE_GET,"Create Descriptors at addresses dput %p rget %p counter phys=0x%016x\n",
		DirectPutDescriptor, RemoteGetDescriptor,CounterPAddr);

	PutDesc.Base.Payload_Address = sendBufPAddr;
	PutDesc.Base.Message_Length = messageSizeInBytes;
	PutDesc.Base.Torus_FIFO_Map = FifoMap;

	/* The message is put back to my node */
	PutDesc.Base.Dest.Destination.A_Destination = pers->Network_Config.Acoord;
	PutDesc.Base.Dest.Destination.B_Destination = pers->Network_Config.Bcoord;
	PutDesc.Base.Dest.Destination.C_Destination = pers->Network_Config.Ccoord;
	PutDesc.Base.Dest.Destination.D_Destination = pers->Network_Config.Dcoord;
	PutDesc.Base.Dest.Destination.E_Destination = pers->Network_Config.Ecoord;

	DPRINTF(FXLOG_REMOTE_GET,"Back to <%d,%d,%d,%d,%d>\n",
		PutDesc.Base.Dest.Destination.A_Destination,
		PutDesc.Base.Dest.Destination.B_Destination,
		PutDesc.Base.Dest.Destination.C_Destination,
		PutDesc.Base.Dest.Destination.D_Destination,
		PutDesc.Base.Dest.Destination.E_Destination);

	if (do_dynamic) {
		PutDesc.Pt2Pt.Misc1 =
			MUHWI_PACKET_USE_DYNAMIC_ROUTING |
			MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE;

		PutDesc.Pt2Pt.Misc2 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC |
			zoneRoutingMask |
			stayOnBubbleMask;
	} else {
		PutDesc.Pt2Pt.Misc1 =
			MUHWI_PACKET_USE_DETERMINISTIC_ROUTING  |
			MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE;

		PutDesc.Pt2Pt.Misc2 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC |
			zoneRoutingMask |
			stayOnBubbleMask;
	}

	/* for checksumming, skip the header */
	PutDesc.Pt2Pt.Skip = 8;
	
	PutDesc.DirectPut.Rec_Payload_Base_Address_Id = firstLocalBaseAddressTable;
	PutDesc.DirectPut.Rec_Counter_Base_Address_Id = firstLocalBaseAddressTable;
	PutDesc.DirectPut.Rec_Payload_Offset = receiveBufPAddr;
	PutDesc.DirectPut.Rec_Counter_Offset = MUSPI_GetAtomicAddress(CounterPAddr, MUHWI_ATOMIC_OPCODE_STORE_ADD);

	PutDesc.DirectPut.Pacing = MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;

	rc = MUSPI_CreatePt2PtDirectPutDescriptor(DirectPutDescriptor, &PutDesc);

	DASSERT(rc == 0, "MUSPI_CreatePt2PtDirectPutDescriptor failed with rc=%d\n",rc);

	/*
	 * Populate Remote Get Descriptor Information Structure
	 */
	GetDesc.Base.Pre_Fetch_Only = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
	GetDesc.Base.Payload_Address = virt_to_dma((uint64_t)DirectPutDescriptor);

	DPRINTF(FXLOG_REMOTE_GET,"descriptor virtual=%p\n",DirectPutDescriptor);
	DPRINTF(FXLOG_REMOTE_GET,"DirectPutDescriptor=%016lx GetDesc.Base.Payload_Address=%016lx\n",
		DirectPutDescriptor, GetDesc.Base.Payload_Address );

	GetDesc.Base.Message_Length = sizeof(MUHWI_Descriptor_t);
	GetDesc.Base.Torus_FIFO_Map = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_PRIORITY;
	GetDesc.Base.Dest = PeerNodeAddr;

	GetDesc.Pt2Pt.Hints_ABCD = 0;

	GetDesc.Pt2Pt.Misc1 = MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE |
			      MUHWI_PACKET_USE_DETERMINISTIC_ROUTING |
			      MUHWI_PACKET_DO_NOT_DEPOSIT;

	GetDesc.Pt2Pt.Misc2 = MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY;

	GetDesc.Pt2Pt.Skip = 0;
	GetDesc.RemoteGet.Type = MUHWI_PACKET_TYPE_GET;

	rgfifo = (((pers->Network_Config.Acoord * pers->Network_Config.Bnodes
		+ pers->Network_Config.Bcoord) * pers->Network_Config.Cnodes
		+ pers->Network_Config.Ccoord) * pers->Network_Config.Dnodes
		+ pers->Network_Config.Dcoord) * pers->Network_Config.Enodes
		+ pers->Network_Config.Ecoord;

	rgfifo = (rgfifo << 2) + (RoundRobinCount++ & 3);

	GetDesc.RemoteGet.Rget_Inj_FIFO_Id = firstLocalInjFifo + 3 + (rgfifo % 32);

	rc = MUSPI_CreatePt2PtRemoteGetDescriptor(RemoteGetDescriptor, &GetDesc);

	DASSERT(rc == 0 ,"MUSPI_CreatePt2PtRemoteGetDescriptor rc=%d\n",rc);

#ifdef DEBUG_REMOTE_GET
	MUSPI_DescriptorDumpHex("Direct Put Descriptor", &PutDesc);
	MUSPI_DescriptorDumpHex("Remote Get Descriptor", &GetDesc);
#endif
}

static inline void create_descriptor_to_ionode(Personality_t *pers,
					MUHWI_Descriptor_t  *DirectPutDescriptor,
					MUHWI_Descriptor_t  *RemoteGetDescriptor,
					uint64_t             CounterPAddr,
					MUHWI_Destination_t  PeerNodeAddr,
					uint64_t receiveBufPAddr,
					uint64_t sendBufPAddr,
					uint64_t messageSizeInBytes)

{
	/* Injection Direct Put Descriptor Information Structure */
	MUSPI_Pt2PtDirectPutDescriptorInfo_t dinfo;

	DPRINTF(FXLOG_REMOTE_GET,"Create Descriptors To IONode at addresses dput %p rget %p counter phys=0x%016x sendBufPAddr=0x%016lx\n",
					 DirectPutDescriptor, RemoteGetDescriptor,CounterPAddr,sendBufPAddr);

	memset((void *)&dinfo, 0, sizeof dinfo);

	dinfo.Base.Payload_Address = sendBufPAddr;
	dinfo.Base.Message_Length  = messageSizeInBytes;

	dinfo.Base.Torus_FIFO_Map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;

	//Need to provide the coords of the paired compute node
	dinfo.Base.Dest.Destination.A_Destination = RoQ_DevMem->mNetworkDesc_rem.Acoord;
	dinfo.Base.Dest.Destination.B_Destination = RoQ_DevMem->mNetworkDesc_rem.Bcoord;
	dinfo.Base.Dest.Destination.C_Destination = RoQ_DevMem->mNetworkDesc_rem.Ccoord;
	dinfo.Base.Dest.Destination.D_Destination = RoQ_DevMem->mNetworkDesc_rem.Dcoord;
	dinfo.Base.Dest.Destination.E_Destination = RoQ_DevMem->mNetworkDesc_rem.Ecoord;
	DPRINTF(FXLOG_REMOTE_GET,"Back to <%d,%d,%d,%d,%d>\n",
	dinfo.Base.Dest.Destination.A_Destination,
	dinfo.Base.Dest.Destination.B_Destination,
	dinfo.Base.Dest.Destination.C_Destination,
	dinfo.Base.Dest.Destination.D_Destination,
	dinfo.Base.Dest.Destination.E_Destination);
	//BGF start
	// zone routing bits
	uint8_t  zoneRoutingMask = 0;
	unsigned zoneRoutingId   = 0;

	// stay on bubble bits
	uint8_t stayOnBubbleMask  = 0;
	unsigned stayOnBubbleFlag = 0;
	//BGF end

	//   dinfo.Pt2Pt.Hints_ABCD = peer.hintsABCD;
	//    if ( do_dynamic)
	//    {
	//
	//      dinfo.Pt2Pt.Misc1 =
	// //    peer.hintsE |
	//     MUHWI_PACKET_USE_DYNAMIC_ROUTING |
	//     MUHWI_PACKET_ROUTE_TO_IO_NODE;
	//
	//      dinfo.Pt2Pt.Misc2 =
	//     MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC |
	//     zoneRoutingMask |
	//     stayOnBubbleMask;
	//      //if ( ( myRank == 0) && ( did_print==0) ) printf(" Using Dynamic Routing\n");
	//    }
	//
	//    else
	{

      dinfo.Pt2Pt.Misc1 =
 //    peer.hintsE |
     MUHWI_PACKET_USE_DETERMINISTIC_ROUTING  |
     MUHWI_PACKET_ROUTE_TO_IO_NODE;

      dinfo.Pt2Pt.Misc2 =
     MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM |
     zoneRoutingMask |
     stayOnBubbleMask;
      //if ( ( myRank == 0) && ( did_print==0) ) printf(" Using Deterministic Routing\n");

    }


    dinfo.Pt2Pt.Skip  = 8; // for checksumming, skip the header

    // BGF start
    // base addess table slot for receive buffer and counter
    //uint32_t recvBufBatId = 0, recvCntrBatId_0 = 1, recvCntrBatId_1 = 2;
    // BGF end

    dinfo.DirectPut.Rec_Payload_Base_Address_Id = firstLocalBaseAddressTable;
    dinfo.DirectPut.Rec_Payload_Offset          = receiveBufPAddr;
    uint64_t paAddrAtomic                       =  MUSPI_GetAtomicAddress(CounterPAddr,MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO);
    dinfo.DirectPut.Rec_Counter_Base_Address_Id = firstLocalBaseAddressTable;
    dinfo.DirectPut.Rec_Counter_Offset          = paAddrAtomic;

    dinfo.DirectPut.Pacing = MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;

    int rc = MUSPI_CreatePt2PtDirectPutDescriptor( DirectPutDescriptor, &dinfo );

 //   if (rc != 0)
 //     {
 //       printf("MUSPI_CreatePt2PtDirectPutDescriptor failed with rc=%d\n",rc);
 //       msg_bisect_exit(1);
 //     }
    DASSERT(rc == 0, "MUSPI_CreatePt2PtDirectPutDescriptor failed with rc=%d\n",rc);

 //    Kernel_MemoryRegion_t memRegion;
 //    rc = Kernel_CreateMemoryRegion ( &memRegion,
 //             &DirectPutDescriptor,
 //             sizeof(DirectPutDescriptor));

     MUSPI_Pt2PtRemoteGetDescriptorInfo_t ginfo;
    MUSPI_Pt2PtRemoteGetDescriptorInfo_t *RemoteGetDescriptorInfo = &ginfo;
 //     (MUSPI_Pt2PtRemoteGetDescriptorInfo_t*)
 //     malloc ( sizeof(MUSPI_Pt2PtRemoteGetDescriptorInfo_t) );
 //   memset((void*)&ginfo,0,sizeof(ginfo));

 //   RemoteGetDescriptorInfo->Base.
    RemoteGetDescriptorInfo->Base.Pre_Fetch_Only  = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
    RemoteGetDescriptorInfo->Base.Payload_Address =
        virt_to_dma((uint64_t)DirectPutDescriptor);
 //       (uint64_t)DirectPutDescriptor - (uint64_t)memRegion.BaseVa
 //     + (uint64_t)memRegion.BasePa;
    DPRINTF(FXLOG_REMOTE_GET,"descriptor virtual=%p\n",DirectPutDescriptor);
    DPRINTF(FXLOG_REMOTE_GET,"DirectPutDescriptor=%016lx RemoteGetDescriptorInfo->Base.Payload_Address=%016lx\n",
        DirectPutDescriptor, RemoteGetDescriptorInfo->Base.Payload_Address );
    RemoteGetDescriptorInfo->Base.Message_Length  = sizeof(MUHWI_Descriptor_t);
    RemoteGetDescriptorInfo->Base.Torus_FIFO_Map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;
    RemoteGetDescriptorInfo->Base.Dest            = PeerNodeAddr;
 #if 0
 // Force + hints on to test if the fabric is a torus
    RemoteGetDescriptorInfo->Pt2Pt.Hints_ABCD     =
   MUHWI_PACKET_HINT_AP |
   MUHWI_PACKET_HINT_BP |
   MUHWI_PACKET_HINT_CP |
   MUHWI_PACKET_HINT_DP;
 #else
    RemoteGetDescriptorInfo->Pt2Pt.Hints_ABCD     = 0;
 #endif
    RemoteGetDescriptorInfo->Pt2Pt.Misc1          = MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE |
      MUHWI_PACKET_USE_DETERMINISTIC_ROUTING |
      MUHWI_PACKET_DO_NOT_DEPOSIT | MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;
    RemoteGetDescriptorInfo->Pt2Pt.Misc2          = MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM;

    RemoteGetDescriptorInfo->Pt2Pt.Skip       = 0;
    RemoteGetDescriptorInfo->RemoteGet.Type             = MUHWI_PACKET_TYPE_GET;
 //   RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id = 2; // Fifo 2 is for remote get use
// #if 1
// //   int rgfifo=pers->Network_Config.Acoord ^
// //             pers->Network_Config.Bcoord ^
// //              pers->Network_Config.Ccoord ^
// //              pers->Network_Config.Dcoord ^
// //              pers->Network_Config.Ecoord;
//    int rgfifo = (((pers->Network_Config.Acoord*pers->Network_Config.Bnodes
//                +pers->Network_Config.Bcoord)*pers->Network_Config.Cnodes
//                +pers->Network_Config.Ccoord)*pers->Network_Config.Dnodes
//                +pers->Network_Config.Dcoord)*pers->Network_Config.Enodes
//                +pers->Network_Config.Ecoord;
// #if 1
//    rgfifo = rgfifo*4 + (RoundRobinCount&3);
//    RoundRobinCount += 1;
// #endif
// #elif 0
//    int rgfifo=RoundRobinCount;
//    RoundRobinCount += 1;
// #else
//    int rgfifo=0;
// #endif
//    RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id = 3 + (rgfifo & 31);

    RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id =
	firstLocalInjFifo + 35;

    // Prepares Injection Memory FIFO Descriptor (RemoteGet)
    rc = MUSPI_CreatePt2PtRemoteGetDescriptor( RemoteGetDescriptor,
                 RemoteGetDescriptorInfo );
 /*   if (rc != 0)
      {
        printf("MUSPI_CreatePt2PtRemoteGetDescriptor failed with rc=%d\n",rc);
        msg_bisect_exit(1);
      }
  */
    DASSERT(rc == 0 ,"MUSPI_CreatePt2PtRemoteGetDescriptor rc=%d\n",rc);

    //TRACE(("%d: Create desc(%p) to proc %d, dest:%d, hintsABCD:%04x, hintsE:%04x\n",myRank,DirectPutDescriptor,peer.rank,*(unsigned *)&peer.muDest,peer.hintsABCD,peer.hintsE));

 //#ifndef __FWEXT__
 //  MUSPI_DescriptorDumpHex("Direct Put Descriptor",DirectPutDescriptor);
 //#endif

   //TRACE(("==========================================\n"));
  }
static inline void create_descriptor_from_ionode(Personality_t *pers,
       MUHWI_Descriptor_t  *DirectPutDescriptor,
       MUHWI_Descriptor_t  *RemoteGetDescriptor,
       uint64_t             CounterPAddr,
         MUHWI_Destination_t  PeerNodeAddr,
         uint64_t receiveBufPAddr,
             uint64_t sendBufPAddr,
             uint64_t messageSizeInBytes
       )

  {
//    uint64_t anyFifoMap =
//      MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
//      MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
//      MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
//      MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
//             MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP
//;

//   int do_dynamic=1;
    // Injection Direct Put Descriptor Information Structure
    MUSPI_Pt2PtDirectPutDescriptorInfo_t dinfo;

    //TRACE(("main(): Create Descriptors at addresses dput %p rget %p\n",DirectPutDescriptor, RemoteGetDescriptor));

   DPRINTF(FXLOG_REMOTE_GET,"Create Descriptors From IONode at addresses dput %p rget %p counter phys=0x%016x sendBufPAddr=0x%016lx\n",
                                         DirectPutDescriptor, RemoteGetDescriptor,CounterPAddr,sendBufPAddr);
    memset( (void*)&dinfo, 0x00, sizeof(dinfo) );


     dinfo.Base.Payload_Address = sendBufPAddr;
    dinfo.Base.Message_Length  = messageSizeInBytes;

    dinfo.Base.Torus_FIFO_Map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;

    //The message is put back to my node
    dinfo.Base.Dest.Destination.A_Destination = pers->Network_Config.Acoord;
    dinfo.Base.Dest.Destination.B_Destination = pers->Network_Config.Bcoord;
    dinfo.Base.Dest.Destination.C_Destination = pers->Network_Config.Ccoord;
    dinfo.Base.Dest.Destination.D_Destination = pers->Network_Config.Dcoord;
    dinfo.Base.Dest.Destination.E_Destination = pers->Network_Config.Ecoord;
    DPRINTF(FXLOG_REMOTE_GET,"Back to <%d,%d,%d,%d,%d>\n",
        dinfo.Base.Dest.Destination.A_Destination,
        dinfo.Base.Dest.Destination.B_Destination,
        dinfo.Base.Dest.Destination.C_Destination,
        dinfo.Base.Dest.Destination.D_Destination,
        dinfo.Base.Dest.Destination.E_Destination);
 //BGF start
 // zone routing bits
 uint8_t  zoneRoutingMask = 0;
 unsigned zoneRoutingId   = 0;

 // stay on bubble bits
 uint8_t stayOnBubbleMask  = 0;
 unsigned stayOnBubbleFlag = 0;
 //BGF end

 //   dinfo.Pt2Pt.Hints_ABCD = peer.hintsABCD;
//    if ( do_dynamic)
//    {
//
//      dinfo.Pt2Pt.Misc1 =
// //    peer.hintsE |
//     MUHWI_PACKET_USE_DYNAMIC_ROUTING |
//     MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE | MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;
//
//      dinfo.Pt2Pt.Misc2 =
//          MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM |
//     zoneRoutingMask |
//     stayOnBubbleMask;
//      //if ( ( myRank == 0) && ( did_print==0) ) printf(" Using Dynamic Routing\n");
//    }
//
//    else
    {

      dinfo.Pt2Pt.Misc1 =
 //    peer.hintsE |
     MUHWI_PACKET_USE_DETERMINISTIC_ROUTING  |
     MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE | MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;

      dinfo.Pt2Pt.Misc2 =
          MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM |
     zoneRoutingMask |
     stayOnBubbleMask;
      //if ( ( myRank == 0) && ( did_print==0) ) printf(" Using Deterministic Routing\n");

    }


    dinfo.Pt2Pt.Skip  = 8; // for checksumming, skip the header

    // BGF start
    // base addess table slot for receive buffer and counter
    //uint32_t recvBufBatId = 0, recvCntrBatId_0 = 1, recvCntrBatId_1 = 2;
    // BGF end

    dinfo.DirectPut.Rec_Payload_Base_Address_Id = firstLocalBaseAddressTable;
    dinfo.DirectPut.Rec_Payload_Offset          = receiveBufPAddr;
    uint64_t paAddrAtomic                       =  MUSPI_GetAtomicAddress(CounterPAddr,MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO);
    dinfo.DirectPut.Rec_Counter_Base_Address_Id = firstLocalBaseAddressTable;
    dinfo.DirectPut.Rec_Counter_Offset          = paAddrAtomic;

    dinfo.DirectPut.Pacing = MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;

    int rc = MUSPI_CreatePt2PtDirectPutDescriptor( DirectPutDescriptor, &dinfo );

 //   if (rc != 0)
 //     {
 //       printf("MUSPI_CreatePt2PtDirectPutDescriptor failed with rc=%d\n",rc);
 //       msg_bisect_exit(1);
 //     }
    DASSERT(rc == 0, "MUSPI_CreatePt2PtDirectPutDescriptor failed with rc=%d\n",rc);

 //    Kernel_MemoryRegion_t memRegion;
 //    rc = Kernel_CreateMemoryRegion ( &memRegion,
 //             &DirectPutDescriptor,
 //             sizeof(DirectPutDescriptor));

     MUSPI_Pt2PtRemoteGetDescriptorInfo_t ginfo;
    MUSPI_Pt2PtRemoteGetDescriptorInfo_t *RemoteGetDescriptorInfo = &ginfo;
 //     (MUSPI_Pt2PtRemoteGetDescriptorInfo_t*)
 //     malloc ( sizeof(MUSPI_Pt2PtRemoteGetDescriptorInfo_t) );
 //   memset((void*)&ginfo,0,sizeof(ginfo));

 //   RemoteGetDescriptorInfo->Base.
    RemoteGetDescriptorInfo->Base.Pre_Fetch_Only  = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
    RemoteGetDescriptorInfo->Base.Payload_Address =
        virt_to_dma((uint64_t)DirectPutDescriptor);
 //       (uint64_t)DirectPutDescriptor - (uint64_t)memRegion.BaseVa
 //     + (uint64_t)memRegion.BasePa;
    DPRINTF(FXLOG_REMOTE_GET,"descriptor virtual=%p\n",DirectPutDescriptor);
    DPRINTF(FXLOG_REMOTE_GET,"DirectPutDescriptor=%016lx RemoteGetDescriptorInfo->Base.Payload_Address=%016lx\n",
        DirectPutDescriptor, RemoteGetDescriptorInfo->Base.Payload_Address );
    DPRINTF(FXLOG_REMOTE_GET,"PeerNodeAddr=%08x\n",PeerNodeAddr.Destination.Destination);
    RemoteGetDescriptorInfo->Base.Message_Length  = sizeof(MUHWI_Descriptor_t);
    RemoteGetDescriptorInfo->Base.Torus_FIFO_Map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;
    RemoteGetDescriptorInfo->Base.Dest            = PeerNodeAddr;
 #if 0
 // Force + hints on to test if the fabric is a torus
    RemoteGetDescriptorInfo->Pt2Pt.Hints_ABCD     =
   MUHWI_PACKET_HINT_AP |
   MUHWI_PACKET_HINT_BP |
   MUHWI_PACKET_HINT_CP |
   MUHWI_PACKET_HINT_DP;
 #else
    RemoteGetDescriptorInfo->Pt2Pt.Hints_ABCD     = 0;
 #endif
    RemoteGetDescriptorInfo->Pt2Pt.Misc1          = MUHWI_PACKET_ROUTE_TO_IO_NODE |
      MUHWI_PACKET_USE_DETERMINISTIC_ROUTING |
      MUHWI_PACKET_DO_NOT_DEPOSIT;
    RemoteGetDescriptorInfo->Pt2Pt.Misc2          = MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM;

    RemoteGetDescriptorInfo->Pt2Pt.Skip       = 0;
    RemoteGetDescriptorInfo->RemoteGet.Type             = MUHWI_PACKET_TYPE_GET;
 //   RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id = 2; // Fifo 2 is for remote get use
// #if 1
// //   int rgfifo=pers->Network_Config.Acoord ^
// //             pers->Network_Config.Bcoord ^
// //              pers->Network_Config.Ccoord ^
// //              pers->Network_Config.Dcoord ^
// //              pers->Network_Config.Ecoord;
//    int rgfifo = (((pers->Network_Config.Acoord*pers->Network_Config.Bnodes
//                +pers->Network_Config.Bcoord)*pers->Network_Config.Cnodes
//                +pers->Network_Config.Ccoord)*pers->Network_Config.Dnodes
//                +pers->Network_Config.Dcoord)*pers->Network_Config.Enodes
//                +pers->Network_Config.Ecoord;
// #if 1
//    rgfifo = rgfifo*4 + (RoundRobinCount&3);
//    RoundRobinCount += 1;
// #endif
// #elif 0
//    int rgfifo=RoundRobinCount;
//    RoundRobinCount += 1;
// #else
//    int rgfifo=0;
// #endif
//    RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id = 3 + (rgfifo & 31);

    RemoteGetDescriptorInfo->RemoteGet.Rget_Inj_FIFO_Id =
	firstLocalInjFifo + 35;
    // Prepares Injection Memory FIFO Descriptor (RemoteGet)
    rc = MUSPI_CreatePt2PtRemoteGetDescriptor( RemoteGetDescriptor,
                 RemoteGetDescriptorInfo );
 /*   if (rc != 0)
      {
        printf("MUSPI_CreatePt2PtRemoteGetDescriptor failed with rc=%d\n",rc);
        msg_bisect_exit(1);
      }
  */
    DASSERT(rc == 0 ,"MUSPI_CreatePt2PtRemoteGetDescriptor rc=%d\n",rc);

    //TRACE(("%d: Create desc(%p) to proc %d, dest:%d, hintsABCD:%04x, hintsE:%04x\n",myRank,DirectPutDescriptor,peer.rank,*(unsigned *)&peer.muDest,peer.hintsABCD,peer.hintsE));

 //#ifndef __FWEXT__
 //  MUSPI_DescriptorDumpHex("Direct Put Descriptor",DirectPutDescriptor);
 //#endif

   //TRACE(("==========================================\n"));
  }


#if defined(PKTRACE_ON)
	TraceClient DMA_MU_GET_Start;
	TraceClient DMA_MU_GET_Finis;
#endif
static uint64_t * tgt_buffer_p;
void  TX_MU_RemoteGet(MUHWI_Destination_t PeerNodeAddr,
			uint64_t RoQ_DmaAddr,
			uint64_t Remote_DmaAddr,
			uint32_t Remote_DmaLen,
			struct   RoQ_TransportConnection* aTranConn,
			int      aBurstAckDescriptorIndex)
{
	DASSERT(RoQ_DmaAddr && Remote_DmaAddr && Remote_DmaLen,
		"RoQ_DmaAddr %p Remote_DmaAddr %p Remote_DmaLen %d\n",
		RoQ_DmaAddr, Remote_DmaAddr, Remote_DmaLen );
//  fflush(stdout);
// Disable interrupts from the MU
//  uint64_t u;u = DCRReadPriv(GEA_DCR(GEA_INTERRUPT_MAP1));
//  GEA_DCR__GEA_INTERRUPT_MAP1__MU_MAP6_insert(u, 15);
//  DCRWritePriv(GEA_DCR(GEA_INTERRUPT_MAP1), u);
//  u = DCRReadPriv(GEA_DCR(GEA_INTERRUPT_MAP9));
//  GEA_DCR__GEA_INTERRUPT_MAP9__MU_MAP5_insert(u, 15); // MU int #5 -> lane 2
//  DCRWritePriv(GEA_DCR(GEA_INTERRUPT_MAP9), u);

	uint64_t RoQ_VirtAddr  = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);
	tgt_buffer_p = (uint64_t *) RoQ_VirtAddr;
	*tgt_buffer_p = 0xdeadbeef;
	DPRINTF(FXLOG_REMOTE_GET,"buffer_p=%p\n",tgt_buffer_p);
	int CounterToUse=DmaGetCounter.FreeCounter;

	DmaGetCounter.FreeCounter=(CounterToUse+1) & (MaxDmaGet-1);

	DASSERT(DmaGetCounter.FreeCounter != DmaGetCounter.OldestCounter, "FreeCounter %d no free MU Get counters\n",DmaGetCounter.FreeCounter);

	uint64_t CounterPhysical = virt_to_dma((uint64_t)(DmaGetCounter.Pending + CounterToUse));

	DPRINTF(FXLOG_REMOTE_GET,"Counter [%d]: virtual=%p physical=%016lx\n", CounterToUse, DmaGetCounter.Pending + CounterToUse, CounterPhysical);
	if (aTranConn->use_iolink) {
		if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) {

			DPRINTF(FXLOG_REMOTE_GET,"** Link crossing towards IO node");

			create_descriptor_to_ionode(personality,
						    DmaGetCounter.DirectPutDescriptor + CounterToUse,
						    DmaGetCounter.RemoteGetDescriptor + CounterToUse,
						    CounterPhysical,
						    PeerNodeAddr,
						    RoQ_DmaAddr,
						    Remote_DmaAddr,
						    Remote_DmaLen);
		} else {
			DPRINTF(FXLOG_REMOTE_GET,"** Link crossing away from IO node");

			create_descriptor_from_ionode(personality,
						      DmaGetCounter.DirectPutDescriptor + CounterToUse,
						      DmaGetCounter.RemoteGetDescriptor + CounterToUse,
						      CounterPhysical,
						      PeerNodeAddr,
						      RoQ_DmaAddr,
						      Remote_DmaAddr,
						      Remote_DmaLen);
		}
	} else {
		create_descriptor(personality,
				  DmaGetCounter.DirectPutDescriptor + CounterToUse,
				  DmaGetCounter.RemoteGetDescriptor + CounterToUse,
				  CounterPhysical,
				  PeerNodeAddr,
				  RoQ_DmaAddr,
				  Remote_DmaAddr,
				  Remote_DmaLen);
	}
	DmaGetCounter.DirectPutDescriptor[CounterToUse].Message_Length=Remote_DmaLen;
	DmaGetCounter.DirectPutDescriptor[CounterToUse].Pa_Payload=Remote_DmaAddr;
	DmaGetCounter.Pending[CounterToUse] = Remote_DmaLen;

	/*  record info for burst completion call */
	DmaGetCounter.TranConn[CounterToUse] = aTranConn;
	DmaGetCounter.BurstAckDescriptorIndex[CounterToUse] = aBurstAckDescriptorIndex;

	DPRINTF(FXLOG_REMOTE_GET,"About to call ROQ_Send");
#if defined(PKTRACE_ON)
	HitOE2(DMA_MU_GET_Start,"DMA_MU_GET_Start_PeerRank=%04d_BurstSlot=%03d", 0, (int)aTranConn->mPeerRank, aBurstAckDescriptorIndex);
#endif
	ROQ_Send(0, 2, &DmaGetCounter.RemoteGetDescriptor[CounterToUse]);

	DPRINTF(FXLOG_REMOTE_GET,"(<) TX_MU_RemoteGet\n");
}

void tc_ProcessBurstAck(struct RoQ_TransportConnection *);

inline void
tc_FinishDelayedBurstAck(struct RoQ_TransportConnection* aTranConn, int BurstAckSlot)
 {
	struct RoQ_RX_BurstAckDescriptor* bad = &aTranConn->mRX_BurstAckDescriptorArray[BurstAckSlot];

	/* Mark burst as completed */
	bad->mStatus = 2; 
	/*
	 * We have completed a ROQ_BURST_IN_PROGRESS. This will be decremented
	 * when this burst is complete. A 0 value allows faster READ processing.
	 */
	bad->mPerQP->mAsyncBurstsInProgress--;

	DPRINTF(DEBUG_REMOTE_GET, "tc_FinishDelayedBurstAck(): Slot %3d QPN %3d bad->mStatus %d ->mBurstType %3X ->FPAck %d RxBurstCount %5ld LastAcked %5ld NextToAck %5ld NextToRecv %5ld\n",
		BurstAckSlot, bad->mPerQP->mQP_Num, bad->mStatus, bad->mBurstType, bad->mForceProtocolAck,
		aTranConn->mBurstCount,
		aTranConn->mBurstSeqNo_LastAcked, aTranConn->mBurstSeqNo_ReadyToAck , aTranConn->mBurstSeqNo_NextToRecv );
	tc_ProcessBurstAck(aTranConn);
}

inline int
TX_PollCounterForBurstCompletion()
{
	// next two lines really sholdn't be here... we can cache the coutner to poll in the main loop
	int CounterToPoll = DmaGetCounter.OldestCounter;
	int rc = ! DmaGetCounter.Pending[CounterToPoll];

	DASSERT(rc != -1, "TX_PollCounterForBurstCompletion() Counter %d is -1 (not in use, invalid.\n",
		CounterToPoll);

	if (rc) {
#if defined(PKTRACE_ON)
		HitOE2(DMA_MU_GET_Finis,"DMA_MU_GET_Finis_PeerRank=%04d_BurstSlot=%03d", 0,
			DmaGetCounter.TranConn[CounterToPoll]->mPeerRank, DmaGetCounter.BurstAckDescriptorIndex[CounterToPoll]);
#endif
		tc_FinishDelayedBurstAck(DmaGetCounter.TranConn[CounterToPoll],
					 DmaGetCounter.BurstAckDescriptorIndex[CounterToPoll]);

		DmaGetCounter.OldestCounter = (CounterToPoll+1) & (MaxDmaGet-1);
		DmaGetCounter.Pending[CounterToPoll] = -1; // set it to something so we don't think it's done!!!

		return 1;
	}
	return 0;
}

#define ROQ_BURST_COMPLETE_NO_ACTION       0
#define ROQ_BURST_COMPLETE_CNTL            1
#define ROQ_BURST_COMPLETE_WRITE_FOR_READ  2
#define ROQ_BURST_COMPLETE_RECV            3
#define ROQ_BURST_COMPLETE_READ_REQUEST    4

/*
 * This function is called when the BGQ MU DMA unit or potentially other
 * data mover has been observed to have finished a transfer.
 * This will complete a burst and may require processing the finalize actions for
 * other bursts that have already completed.
 */
inline void
tc_ProcessBurstAck(struct RoQ_TransportConnection* aTranConn)
{
	struct RoQ_RX_BurstAckDescriptor *bad;
	int DoProtocolDrivenAck = 0;

	/*
	 * NOTE: bad->mPerQP can be accessed but remember that the QP maybe
	 * processing a different message than the one evaluated here.
	 */

#if 0
	static int call_count = 0;
	DPRINTF(FXLOG_UCODE_PER_MESSAGE,"tc_ProcessBurstAck(): Called %d LastAcked %ld ReadyToAck %ld NextToRecv %ld\n",
	       call_count++, aTranConn->mBurstSeqNo_LastAcked, aTranConn->mBurstSeqNo_ReadyToAck, aTranConn->mBurstSeqNo_NextToRecv);
#endif

	while(aTranConn->mBurstSeqNo_ReadyToAck < aTranConn->mBurstSeqNo_NextToRecv) {
		int BurstAckSlot = (aTranConn->mBurstSeqNo_ReadyToAck + 1 ) & RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT_MASK;
		struct RoQ_RX_BurstAckDescriptor *bad = &aTranConn->mRX_BurstAckDescriptorArray[BurstAckSlot];

#if 1
		if (aTranConn->mBurstSeqNo_ReadyToAck + 1 < aTranConn->mBurstSeqNo_NextToRecv)
			DPRINTF(FXLOG_UCODE_PER_MESSAGE,"tc_ProcessBurstAck(): Slot %3d QPN %3d bad->mStatus %d ->mBurstType %3X ->FPAck %d "
							"RxBurstCount %5ld LastAcked %5ld NextToAck %5ld NextToRecv %5ld\n",
							BurstAckSlot, bad->mPerQP->mQP_Num, bad->mStatus, bad->mBurstType,
							bad->mForceProtocolAck, aTranConn->mBurstCount,
							aTranConn->mBurstSeqNo_LastAcked, aTranConn->mBurstSeqNo_ReadyToAck + 1,
							aTranConn->mBurstSeqNo_NextToRecv);
#endif

		if (bad->mStatus != 2) {
			/*
			 * This is the way out. burst is either not used (acked) or set in-progress
			 */
#if 1
			if(aTranConn->mBurstSeqNo_ReadyToAck + 1 < aTranConn->mBurstSeqNo_NextToRecv)
				DPRINTF(FXLOG_UCODE_PER_MESSAGE,"tc_ProcessBurstAck(): ACKs WAITING for BurstSlot %3d QPN %3d bad->mStatus %d ->mBurstType %3X "
								"->FPAck %d RxBurstCount %5ld LastAcked %5ld NextToAck %5ld NextToRecv %5ld\n",
								BurstAckSlot, bad->mPerQP->mQP_Num, bad->mStatus, bad->mBurstType, bad->mForceProtocolAck,
								aTranConn->mBurstCount,
								aTranConn->mBurstSeqNo_LastAcked, aTranConn->mBurstSeqNo_ReadyToAck + 1,
								aTranConn->mBurstSeqNo_NextToRecv );
#endif
			break;
		}
		/*
		 * Set up a protocol driven ack (as opposed to a rolling window driven ack)
		 * if any completed burst needs it
		 */
		DoProtocolDrivenAck |= bad->mForceProtocolAck;

		/*
		 * Do action on completion based on BurstType
		 */
		switch(bad->mBurstType) {

		case ROQ_BURST_COMPLETE_WRITE_FOR_READ:
			DASSERT(bad->wr_id == -1,"Old flag for READ FOR WRITE completion not set\n");

			RoQ_RX_RDMA_WRITE_Completes_RDMA_READ_Request(bad->mPerQP, bad->mRdmaCompletionLen);
			break;

		case ROQ_BURST_COMPLETE_RECV:
			/* wr_id has been used to hold CQE pointer */
			if (bad->wr_id) {
				struct RoQ_DM_QP	*qp = RoQ_GetQPPtr_Idx(bad->mPerQP->mQP_Num);
				struct roq_dm_cq	*roq_cq = (struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->mRecvCQ_DmaPtr);
				struct roq_cqe		*cqe = (struct roq_cqe *)bad->wr_id;

				DPRINTF(FXLOG_UCODE_PER_MESSAGE,
					"ROQ_OP_SEND FINISHED Placed %lx wr_id %lx bytes PerQP@ %p PerQP->mQP_Num %ld qp@ 0x%p qp->mRecv_DmaPtr 0x%p\n",
					(uint64_t)bad->mRdmaCompletionLen, bad->wr_id,
					bad->mPerQP, (uint64_t)bad->mPerQP->mQP_Num, (void *)qp, (void *)qp->mRecvCQ_DmaPtr);

				RoQ_Complete_CQE(roq_cq, cqe, 1);
			} else
				DPRINTF(1, "ROQ_BURST_COMPLETE_RECV: no CQE!\n");

			break;

		case ROQ_BURST_COMPLETE_READ_REQUEST:
			/*
			 * Advance the InboundReadQueue_Ready counter to allow processing of the
			 * READ associated with this burst.
			 * Ring the doorbell of the TX thread here on the read remote src
			 *
			 * NEED: to arrange to hold off READ processing when there are delayed burst completions
			 */
			bad->mPerQP->mInboundReadQueue_Ready++;

			/* ensure the Ready++ gets out to the TX thread. */
			ppc_msync();

			RoQ_MC_RingTxDoorbell(bad->mPerQP->mQP_Num, 0);

			DPRINTF(FXLOG_UCODE_PER_MESSAGE, "RX RDMA_READ TX Doorbell rung!\n");

			break;

		case ROQ_BURST_COMPLETE_NO_ACTION:
			/* NOP */
			break;

		case ROQ_BURST_COMPLETE_CNTL:

			roq_post_net_event(NET_EVENT_QP_DISCONNECT, &bad->mPerQP->mQP_Num, 1);

			DPRINTF(FXLOG_UCODE_CNTL, "RX CNTL DISCONECT RECEIVED AND PROCESSED\n");

			break;

		default:
			DPRINTF(FXLOG_UCODE_PER_MESSAGE,"tc_ProcessBurstAck:: unknown action: %d\n", bad->mBurstType);

			break;
		}
		aTranConn->mBurstSeqNo_ReadyToAck++;
		bad->mStatus = 0; // set slot to acked/free
	}

	if (aTranConn->mBurstSeqNo_ReadyToAck > aTranConn->mBurstSeqNo_LastAcked) {
		tc_TX_Ack_Blunt(aTranConn, 0xffffffff, aTranConn->mBurstSeqNo_ReadyToAck);
		aTranConn->mBurstSeqNo_LastAcked = aTranConn->mBurstSeqNo_ReadyToAck;
	}
}


/*
 * This routine is called by the RX state machine and posts a completion for a burst.
 * This is in order with TX -> RX state machine protocol however status might be set to 'burst in progres'.
 * Acks will be sent to the other side for any completed burst
 * only when all previous bursts have complete.
 * A burst may require a finalize function be called after
 * all previous bursts complete as well.
 */

inline
void
tc_PostBurstCompletion(struct RoQ_TransportConnection*	        aTranConn,   // need to bring in the tranconn becaause this is called from contexts w/o a qp
                       struct RoQ_DevicePrivatePerQPState*     aPerQP,      // need to bring in the qp for contexts tha twill need to do a completion
                       int                                     aStatus,     // 0 acked (or not used), 1 burst in progress, 2 burst is ready to ack
                       char                                    aBurstType,  // need to do different thing for different types -- particularly last bursts of WRITE_FOR_READ and RECEIVES
                       int                                     aRdmaCompletionLen,
                       uint64_t                                wr_id,        // this is really a general 64 bit parameter -- sometimes wr_id
                       int                                     aForceProtocolAck)       // Allow upper layer to flag necessity of an ack to drive protocol
  {
  // record this in the RX_BurstAck circular buffer
  // record all bursts because we won't ack everyone and a delayed burst could start any time
  // once a delayed burst is started, to ack we'll need this information.
  uint64_t CurrentBurstToAckSeqNo = aTranConn->mBurstSeqNo_NextToRecv;
  uint64_t BurstAckSlot           = CurrentBurstToAckSeqNo & RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT_MASK;

  struct RoQ_RX_BurstAckDescriptor* bad = &aTranConn->mRX_BurstAckDescriptorArray[BurstAckSlot];

  DPRINTF(FXLOG_UCODE_PER_MESSAGE,"BurstAckSlot=%d bad=%p\n",BurstAckSlot,bad);
  ////DASSERT( aStatus == 2, "tc_PostBurstCompletion(): aStatus %d\n", (int) aStatus);
  //DASSERT( ((aTranConn->mBurstSeqNo_NextToRecv - 1) == aTranConn->mBurstSeqNo_ReadyToAck),
  //          "tc_PostBurstCompletion(): aTranConn->mBurstSeqNo_NextToRecv %ld  != aTranConn->mBurstSeqNo_ReadyToAck %ld BurstType %X CompLen %d wr_id %ld\n",
  //          (aTranConn->mBurstSeqNo_NextToRecv - 1) , aTranConn->mBurstSeqNo_ReadyToAck, (int) aBurstType, aRdmaCompletionLen, wr_id);

  bad->mStatus            = aStatus;
  bad->mBurstType         = aBurstType;
  bad->mPerQP             = aPerQP;
  bad->mRdmaCompletionLen = aRdmaCompletionLen;
  bad->wr_id              = wr_id;
  bad->mForceProtocolAck  = aForceProtocolAck;

  tc_ProcessBurstAck( aTranConn );

  aTranConn->mBurstSeqNo_NextToRecv++;
  aTranConn->mExpectedHardwarePutOffset.mBurstSeqNo = aTranConn->mBurstSeqNo_NextToRecv;
  aTranConn->mExpectedHardwarePutOffset.mByteOffset = 0;
  #if 0
  if( aStatus == ROQ_BURST_IN_PROGRESS )
    printf("tc_PostBurstCompletion(): BURST-IN_PROG BurstAckSlot %d ForcePrAck %d qpn %d BurstType %08x MsgLen %d LastAcked %ld ReadyToAck %ld Current(NextToRecv) %ld \n",
            BurstAckSlot, aForceProtocolAck, aPerQP->mQP_Num, aBurstType, aRdmaCompletionLen,
            aTranConn->mBurstSeqNo_LastAcked, aTranConn->mBurstSeqNo_ReadyToAck, CurrentBurstToAckSeqNo);
    fflush(stdout);
  #endif
    DPRINTF(FXLOG_UCODE_PER_MESSAGE,"\n");
  return;
  }

static void dump_wq(int queue_size, int item_size, char *queue)
{
	char	buf[64];
	struct whdr {
		uint64_t id;
		uint32_t flags;
	};
	int	idx = 0,
		last_empty = -1,
		last_complete = -1,
		is_cq,
		unknown_opcode = ROQ_OP_WRITE;

	is_cq = (item_size == sizeof(struct roq_cqe)) ? 1 : 0;

	DPRINTF(1, "size = %d, READ (Get): G, WRITE: W, SEND: S, RECEIVE: R\n", queue_size);

	while (idx < queue_size) {
		if (is_cq) {
			struct roq_cqe *cqe = (struct roq_cqe *)&queue[idx * item_size];
			int free = _load_shared(cqe->flags) & ROQ_WC_FREE;
			char op;

			if	(cqe->opcode == ROQ_OP_WRITE)	op = (free != 0) ? 'w' : 'W';
			else if	(cqe->opcode == ROQ_OP_READ)	op = (free != 0) ? 'g' : 'G';
			else if	(cqe->opcode == ROQ_OP_SEND)	op = (free != 0) ? 's' : 'S';
			else if	(cqe->opcode == ROQ_OP_RECEIVE)	op = (free != 0) ? 'r' : 'R';
			else {
				op = (free != 0) ? 'x' : 'X';
				unknown_opcode = cqe->opcode;
			}
			if (!free)
				last_complete = idx;

			buf[idx % 64] = op;

			if (last_empty == -1 && free == 0)
				last_empty = idx - 1;
		} else {
			struct whdr *hp = (struct whdr *)&queue[idx * item_size];
			buf[idx % 64] = _load_shared(hp->flags) & ROQ_WQE_VALID ? '1' : '0';
		}
		if (++idx % 64 == 0)
			DPRINTF(1, "%d\t%s", idx - 64, buf);
	}
	if (idx % 64 != 0) {
		buf[idx % 64] = '\0';
		DPRINTF(1, "%d\t%s", idx - (idx % 64), buf);
	}
	if (last_empty > 0 && last_empty < queue_size - 1) {
		struct roq_cqe *cq = (struct roq_cqe *)queue;
		struct roq_cqe *cqe = &cq[last_empty];
		while (last_empty <= last_complete + 1) {
			DPRINTF(1, "CQE at [%d]: id 0x%llx, flags 0x%x, op %d, status %d, imm_data %llu, bytes %u, QP %d\n",
				last_empty, cqe->id, cqe->flags, cqe->opcode, cqe->status, cqe->imm_data, cqe->bytes, cqe->qp_num);
			cqe++;
			last_empty++;
			if (last_empty == queue_size)
				break;
		}
	}
	if (unknown_opcode != ROQ_OP_WRITE)
		DPRINTF(1, "found unknown opcode %d\n", unknown_opcode);
}

static void roq_print_workqueues(uint32_t PktDestQPId)
{
	struct RoQ_DevicePrivatePerQPState *PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
	struct RoQ_DM_QP *qp = RoQ_GetQPPtr_Idx(PktDestQPId);

	struct KISS_SRSWQ *sqp = &qp->mSendQueue;
	struct KISS_SRSWQ *rqp = &qp->mRecvQueue;
	struct roq_sqe *sqe = (struct roq_sqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)(sqp->mItemArrayDmaAddr));
	struct roq_rqe *rqe = (struct roq_rqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)(rqp->mItemArrayDmaAddr));

	struct roq_dm_cq *rcq = (struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->mRecvCQ_DmaPtr);
	struct roq_dm_cq *scq = (struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->mSendCQ_DmaPtr);
	struct roq_cqe *rcqe  = (struct roq_cqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)rcq->cq_phys);
	struct roq_cqe *scqe  = (struct roq_cqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)scq->cq_phys);

	int notify_at_start_s = scq->notify;
	int notify_at_start_r = rcq->notify;
	int index_at_start_s = scq->DmQueue.mPut % scq->DmQueue.mItemCount;
	int index_at_start_r = rcq->DmQueue.mPut % rcq->DmQueue.mItemCount;

	DPRINTF(1, "RQ:: index = %d\n", PerQP->mCurrent_RQE_Index);
	dump_wq(rqp->mItemCount, sizeof *rqe, (char *)rqe);

	DPRINTF(1, "SQ:: index = %d\n", sqp->mGot % sqp->mItemCount);
	dump_wq(sqp->mItemCount, sizeof *sqe, (char *)sqe);

	if (rcq == scq) {
		DPRINTF(1, "CQ[%d]:: index = %d:%d, notify = %s:%s\n", scq->dd_cq_id, index_at_start_s,
			scq->DmQueue.mPut % scq->DmQueue.mItemCount, _load_shared(scq->notify) ? "Y":"N", notify_at_start_s ? "Y":"N");
		dump_wq(scq->DmQueue.mItemCount, sizeof *scqe, (char *)scqe);
	} else {
		DPRINTF(1, "SCQ[%d]:: index = %d:%d, notify = %s:%s\n", scq->dd_cq_id, index_at_start_s,
			scq->DmQueue.mPut % scq->DmQueue.mItemCount, _load_shared(scq->notify) ? "Y":"N", notify_at_start_s ? "Y":"N");
		dump_wq(scq->DmQueue.mItemCount, sizeof *scqe, (char *)scqe);

		DPRINTF(1, "RCQ[%d]:: index = %d:d, notify = %s:%s\n", rcq->dd_cq_id, index_at_start_r,
			rcq->DmQueue.mPut % rcq->DmQueue.mItemCount, _load_shared(rcq->notify) ? "Y":"N", notify_at_start_r ? "Y":"N");
		dump_wq(rcq->DmQueue.mItemCount, sizeof *rcqe, (char *)rcqe);
	}
}


static inline void
roq_rx_close_on_error(struct RoQ_DevicePrivatePerQPState *PerQP, enum net_event_type event)
{
		struct RoQ_TransportConnection	*TranConn = PerQP->mTranConn;
		
		DPRINTF(1, "ROQ RX: QP[%d]: Report error %d\n", PerQP->mQP_Num, event);

		roq_post_net_event(event, &PerQP->mQP_Num, 1);

		tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1);

		PerQP->mCurrent_Setup_Type == -1;
		TranConn->mCurrentRecvBurst_QP_Ptr = NULL;

		PerQP->state_flags |= ROQ_RX_SUSPENDED;
}



void *
RoQ_RX(void *args)
{
	uint32_t PktSource;
	uint32_t PktBurstType;
	uint32_t PktDestQPId;
	uint32_t PktBytesInLastPacket;
	uint32_t PktForwardCount;
	uint32_t PktIOLinkBoundPacketFlag;
	uint16_t PktPacketsInBurst;
	uint16_t PktNumberInBurst;
	uint16_t PktBurstSeqNo;
	uint16_t PktByteOffsetInBurst;
	uint32_t PktMaxPayloadSize = 512;
	void	*PktPayloadPtr;
	int rc;
#define PktBytesInPayload \
	((PktNumberInBurst == PktPacketsInBurst) ? PktBytesInLastPacket:PktMaxPayloadSize)

#if defined(PKTRACE_ON)
	static int ProcessingPacket = 0;

	TraceClient PacketReceivedStart;
	TraceClient PacketReceivedFinis;
	TraceClient BurstRecvStart;
	TraceClient BurstRecvFinis;
	TraceClient RDMA_WRITE_Start;
	TraceClient RDMA_WRITE_Finis;
	TraceClient RDMA_WRITE_MEMCPY_Start;
	TraceClient RDMA_WRITE_MEMCPY_Finis;

	PkTraceStreamInit(&PacketReceivedStart);
	PkTraceStreamInit(&PacketReceivedFinis);
	PkTraceStreamInit(&BurstRecvStart);
	PkTraceStreamInit(&BurstRecvFinis);
	PkTraceStreamInit(&RDMA_WRITE_Start);
	PkTraceStreamInit(&RDMA_WRITE_Finis);
	PkTraceStreamInit(&RDMA_WRITE_MEMCPY_Start);
	PkTraceStreamInit(&RDMA_WRITE_MEMCPY_Finis);
        // these are in static space
	PkTraceStreamInit(&DMA_MU_GET_Start);
	PkTraceStreamInit(&DMA_MU_GET_Finis);

	PkTraceStreamInit(&Complete_CQE_Start);
	PkTraceStreamInit(&Complete_CQE_Finis);


	int i;

#define PKTRACE_PACKET_RECEIVED 1
#define PKTRACE_BURST_RECV 1
#define PKTRACE_RDMA_WRITE 1
#define PKTRACE_RDMA_WRITE_MEMCPY 1

#endif
	/*
	 * A stack backing for the received packet --
	 * no packet should never get to main memory before
	 * being overwritten by the net.
	 */
	void *RecvPkt = NULL;
#if MES_POLLS_RETURNS==1
	uint64_t EmptyPolls;
	uint64_t FullPolls;
#endif
	struct RoQ_RX_Arguments *RxArgs = ((struct RoQ_RX_Arguments *)args);

	DSTRONG_ASSERT(args, "thread args pointer NULL %p\n", args);

	DPRINTF(FXLOG_UCODE_INIT, "Starting. Args@ 0x %p PortId %lx\n", (void *)args, (uint64_t)RxArgs->mPortId);

	ROQ_set_affinity(16);

#if defined(PKTRACE_ON)
	for (i = 0; i < 100; i++) {
		HitOE(PacketReceivedStart,"PacketReceived", 0);
		HitOE(PacketReceivedFinis,"PacketReceived", 0);
	}

	//  pkTraceServer::FlushBuffer();
	//  pkTraceServer::ResetAndTouch();
#endif

	/* Main data receive path loop */
	for (;;) {
		union RoQ_BGQ_MU_FifoPkt_SwHdr		*BGQ_SwHdr;
		union RoQ_BGQ_MU_HW_PutOffset		PktPutOffset;
		struct RoQ_TransportConnection		*TranConn;
		struct RoQ_DevicePrivatePerQPState	*PerQP;
		struct RoQ_DM_QP	*qp;
		struct roq_rqe		*rqe;
		static uint64_t		packet_count = 0;
		uint64_t 		MsgLastByteAddr,
					LastByteOfMR,
					RoQ_DmaAddr,
					RoQ_VirtAddr;
		uint32_t		*db_id_p;
#if MES_MEMCPY == 1
		static uint64_t fast_move = 0;
		static uint64_t slow_move = 0;
#endif
		uint8_t PacketType = 0;

#if defined(PKTRACE_ON)
#if PKTRACE_PACKET_RECEIVED
		if (ProcessingPacket) {
			HitOE(PacketReceivedFinis,"PacketReceived", 0);
			ProcessingPacket = 0;
		}
#endif
#endif

		while (1) {
			const int show_dmaget_histogram = 0;
get_next_packet:
			if (show_dmaget_histogram == 0) {
				int counter = 0;

				while (TX_PollCounterForBurstCompletion()) {
					/* reap all pending burst completions */
					if (counter++ > MaxDmaGet) {
						printf("DMA_GET RX BUG\n");
						while (1)
							sleep (1);
					}
				}
			} else {
				static long int nextshow = 10000000;
				static long int iteration = 0;
				static long int pollhistogram[16];

				int pollhits = 0;

				/* see if asycn mu gets have completed and handle */
				while (TX_PollCounterForBurstCompletion())
					pollhits += 1; 

				if (pollhits < 16)
					pollhistogram[pollhits]++;
				else
					pollhistogram[15]++;

				if (++iteration >= nextshow && pollhits > 1) {
					nextshow += 10000000;

					DPRINTF(1, " %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
						pollhistogram[0],pollhistogram[1],pollhistogram[2],pollhistogram[3],
						pollhistogram[4],pollhistogram[5],pollhistogram[6],pollhistogram[7],
						pollhistogram[8],pollhistogram[9],pollhistogram[10],pollhistogram[11],
						pollhistogram[12],pollhistogram[13],pollhistogram[14],pollhistogram[15]);
				}
			}

			rc = ROQ_getnext_packet(0, RxArgs->recfifo_ptr, &RecvPkt, 1);
#ifndef USERSPACE_ROQ
                        if (rc == 0)
                        {
//                            static int repcount;
//                            if( repcount < 5 )
//                              {
//                                repcount += 1;
//                                printf("ROQ_getnext_packet remote\n");
//                              }
                                rc = ROQ_getnext_packet(1, RxArgs->rem_recfifo_ptr, &RecvPkt, 1);
//                                if(rc) printf("Remote packet, %d bytes\n",rc);
                        }
#endif
			PktPayloadPtr = (void *)(((char *) RecvPkt) + 32);

//			if(rc > 0)
//			  {
//			    printf("Incoming packet, size=%d, payload starts %016lx\n", rc, ((uint64_t*)PktPayloadPtr)[0]);
//			  }

			if ((rc > 0 && ((uint64_t*)PktPayloadPtr)[0] == ROQ_DISCOVERY_MAGIC) ||
			    RoQ_discovery_act == 1) {

				DPRINTF(FXLOG_UCODE_INIT, "DISCOVERY (%d) RoQ_discovery_act=%d", rc, RoQ_discovery_act);
				RoQ_discovery();
				continue;
			}

#if MES_POLLS_RETURNS==1
			if (rc > 0)
				FullPolls++;
			else
				EmptyPolls++;
#endif
			if (rc > 0) {
				MUHWI_PacketHeader_t *packet_ptr = (MUHWI_PacketHeader_t *)RecvPkt;
#if (FXLOG_ENABLE == 1)
				unsigned int *BufAsIntPtr = (unsigned int *)RecvPkt;
#endif

#if defined(PKTRACE_ON)
#if PKTRACE_PACKET_RECEIVED
				ProcessingPacket = 1;
				HitOE(PacketReceivedStart,"PacketReceived", 0);
#endif
#endif


				BGQ_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr *)&packet_ptr->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2;
				PktPutOffset.mWhole = RoQ_MUHWI_MUHdr_PutOffset_LSB(packet_ptr);

				PktSource                = BGQ_SwHdr->mSource;
				PktBurstType             = BGQ_SwHdr->mBurstType;
    				PacketType = PktBurstType & 0x3F; // only six bits go into the state machine. Higher bits in the on-wire BurstType used when needed.
				PktDestQPId              = BGQ_SwHdr->mDestConnectionId;
				PktPacketsInBurst        = BGQ_SwHdr->mPacketsInBurst;
				PktBytesInLastPacket     = BGQ_SwHdr->mBytesInLastPacket;
				PktForwardCount          = 0;
				PktIOLinkBoundPacketFlag = BGQ_SwHdr->mIOLinkBoundPacketFlag;
				PktBurstSeqNo            = PktPutOffset.mBurstSeqNo;
				PktByteOffsetInBurst     = PktPutOffset.mByteOffset;
				PktNumberInBurst         = PktByteOffsetInBurst / PktMaxPayloadSize;



#ifdef ROQ_RESILIENCY_DEBUG
				{
					uint32_t pa, pb, pc, pd, pe;
					
					RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc, PktSource, &pa,
											&pb, &pc, &pd, &pe);
					
					DPRINTF(ROQ_RESILIENCY_DEBUG, "Receiving BURST packet from (%d,%d,%d,%d,%d), offset = %x, this = (%d,%d,%d,%d,%d)\n",
						   pa, pb, pc, pd, pe,
						   RoQ_MUHWI_MUHdr_PutOffset_LSB(packet_ptr),
						   personality->Network_Config.Acoord,
						   personality->Network_Config.Bcoord,
						   personality->Network_Config.Ccoord,
						   personality->Network_Config.Dcoord,
						   personality->Network_Config.Ecoord);
				}
#endif

				DPRINTF(FXLOG_UCODE_PER_PACKET,
				   "Got Pkt! PktSource %lx PktNumberInBurst %lx PktBurstSeqNo %lx PktBurstType %lx IsLast %lx "
					   "PktBytesInLastPacket %lx PktDestQPId %lx PktPacketsInBurst %lx PktForwardCount %lx "
					   "PktIOLinkBoundPacketFlag %lx\n",
				   (uint64_t)PktSource, (uint64_t)PktNumberInBurst, (uint64_t)PktBurstSeqNo,
				   (uint64_t)PktBurstType, (uint64_t)(PktNumberInBurst == PktPacketsInBurst),
				   (uint64_t)PktBytesInLastPacket, (uint64_t)PktDestQPId, (uint64_t)PktPacketsInBurst,
				   (uint64_t)PktForwardCount, (uint64_t)PktIOLinkBoundPacketFlag);
#if ROQ_DO_COSTLY_PROTOCOL_CHECKS
				DASSERT(PktBytesInPayload,
				   "Got Pkt! PktSource%lx PktNumberInBurst %lx PktBurstSeqNo %lx PktType %lx IsLast %lx "
					   "PktBytesInLastPacket %lx PktDestQPId %lx PktPacketsInBurst %lx PktForwardCount %lx "
					   "PktIOLinkBoundPacketFlag %lx PktRdmaSetupFlag %lx\n",
				   (uint64_t)PktSource, (uint64_t)PktNumberInBurst, (uint64_t)PktBurstSeqNo,
				   (uint64_t)PktType, (uint64_t)(PktNumberInBurst == PktPacketsInBurst),
				   (uint64_t)PktBytesInLastPacket, (uint64_t)PktDestQPId, (uint64_t)PktPacketsInBurst,
				   (uint64_t)PktForwardCount, (uint64_t)PktIOLinkBoundPacketFlag, (uint64_t)PktRdmaSetupFlag);
#endif

				break;
			}
		}

		/* get use the SourceNodeId to select a transport connection */

		if (PktIOLinkBoundPacketFlag)
			TranConn = &RoQ_rem_TransportConnection[PktSource];
		else
			TranConn = &RoQ_loc_TransportConnection[PktSource];

		packet_count++;

		/* Check for last Pkt. in a Burst */
		if (PktNumberInBurst == PktPacketsInBurst) {
			TranConn->mBurstCount++;
			if(0) printf(
				"Got LAST Pkt! PktSource %lx PktNumberInBurst %lx PktBurstSeqNo %lx PktBurstType %lx IsLast %lx "
				   "PktBytesInLastPacket %lx PktDestQPId %lx PktPacketsInBurst %lx PktForwardCount %lx "
				   "PktIOLinkBoundPacketFlag %lx\n",
				(uint64_t)PktSource, (uint64_t)PktNumberInBurst, (uint64_t)PktBurstSeqNo,
				(uint64_t)PktBurstType, (uint64_t)(PktNumberInBurst == PktPacketsInBurst),
				(uint64_t)PktBytesInLastPacket, (uint64_t)PktDestQPId, (uint64_t)PktPacketsInBurst,
				(uint64_t)PktForwardCount, (uint64_t)PktIOLinkBoundPacketFlag);
			
		}

		/*
		 * On Blue Gene networks with deterministic routing, we expect exactly
		 * the next SEQ#.
		 * So far, this is the only check we put on packets... but might need more
		 */
		if (PktPutOffset.mWhole != TranConn->mExpectedHardwarePutOffset.mWhole) {
			/*
			 * likely we discard as a dup. could do more checking
			 * check if we need to send a NACK
			 */
			//DPRINTF(PKFXLOG_NACK_HANDLING && TranConn->nackin,
			DPRINTF(1,
				"OUT OF ORDER - DISCARDING packet_count %lx PktPutOffset.mWhole %lx %lx %lx TranConn->mExpectedHardwarePutOffset.mWhole %lx %lx %lx\n",
				(uint64_t)packet_count, (uint64_t)PktPutOffset.mWhole, (uint64_t)PktPutOffset.mBurstSeqNo,
				(uint64_t)PktPutOffset.mByteOffset, (uint64_t)TranConn->mExpectedHardwarePutOffset.mWhole,
				(uint64_t)TranConn->mExpectedHardwarePutOffset.mBurstSeqNo, (uint64_t)TranConn->mExpectedHardwarePutOffset.mByteOffset);

			if (!TranConn->nackin) {
				/* NACK causing retrans from last received */
				//tc_TX_Ack(TranConn, 0xFFFFFFFE);
				tc_TX_Ack_Blunt(TranConn, 0xFFFFFFFE,TranConn->mExpectedHardwarePutOffset.mBurstSeqNo); // nack back to what was expected
				TranConn->nackin = 1;

				DPRINTF(PKFXLOG_NACK_HANDLING, "\n\n***** RoQ FIRMWARE : WARNING : SENDING NACK at recv "
					"pkt number %ld. Got PutOffset %08X. Expected PutOffset %08X\n\n",
					packet_count, PktPutOffset.mWhole, TranConn->mExpectedHardwarePutOffset.mWhole);

				DPRINTF(PKFXLOG_NACK_HANDLING, "WARNING *** SENT NACK **** packet_count %lx PktPutOffset.mWhole %lx %lx %lx "
					"TranConn->mExpectedHardwarePutOffset.mWhole %lx %lx %lx PktIOLinkBoundPacketFlag %d PktSource %d\n",
					(uint64_t)packet_count, (uint64_t)PktPutOffset.mWhole, (uint64_t)PktPutOffset.mBurstSeqNo,
					(uint64_t)PktPutOffset.mByteOffset, (uint64_t)TranConn->mExpectedHardwarePutOffset.mWhole,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mBurstSeqNo,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mByteOffset, PktIOLinkBoundPacketFlag, PktSource);

				DPRINTF(PKFXLOG_NACK_HANDLING, "BGQ_SwHdr->mSource %d, QP[%d] state %d\n", BGQ_SwHdr->mSource, PktDestQPId, RoQ_DevicePrivatePerQPState[PktDestQPId].mState);
			}
			continue;
		} else {
			if (TranConn->nackin) {
				DPRINTF(PKFXLOG_NACK_HANDLING,
					"GOT NACKED PACKET packet_count %lx PktPutOffset.mWhole %lx %lx  %lx TranConn->mExpectedHardwarePutOffset.mWhole %lx %lx %lx\n",
					(uint64_t)packet_count, (uint64_t)PktPutOffset.mWhole, (uint64_t)PktPutOffset.mBurstSeqNo, (uint64_t)PktPutOffset.mByteOffset,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mWhole, (uint64_t)TranConn->mExpectedHardwarePutOffset.mBurstSeqNo,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mByteOffset);
				DPRINTF(PKFXLOG_NACK_HANDLING,
					"Closed HOLE: packet_count %lx PktPutOffset.mWhole %lx %lx %lx "
					"TranConn->mExpectedHardwarePutOffset.mWhole %lx %lx %lx PktIOLinkBoundPacketFlag %d PktSource %d, QP[%d] State %d\n",
					(uint64_t)packet_count, (uint64_t)PktPutOffset.mWhole, (uint64_t)PktPutOffset.mBurstSeqNo,
					(uint64_t)PktPutOffset.mByteOffset, (uint64_t)TranConn->mExpectedHardwarePutOffset.mWhole,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mBurstSeqNo,
					(uint64_t)TranConn->mExpectedHardwarePutOffset.mByteOffset, PktIOLinkBoundPacketFlag, PktSource,
					PktDestQPId, RoQ_DevicePrivatePerQPState[PktDestQPId].mState);
				TranConn->nackin = 0;
			}
		}

		if (PktDestQPId >= RoQ_MAX_QP) {
			TranConn->mExpectedHardwarePutOffset.mByteOffset += PktMaxPayloadSize;
			printf("RoQ RX: Destination QP %d out of bounds\n", PktDestQPId);
			
			continue;
		}
		/* Won't be forwarding... access PerQP stuct and discard if qp != RTR or RTS */
		PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
		/*
		 * Do not process packets if QP is out of one of its RX-active states RTR, RTS, SQD.
		 */
		if (PerQP->mState == ROQ_QPS_RESET || PerQP->mState > ROQ_QPS_SQD) {
			DPRINTF(1 || FXLOG_UCODE_QP_STATE,
				"** NOT RTR or RTS or SQD *** DISCARDING FRAME: PerQP->mState %lx PktSource %lx PktNumberInBurst %lx "
				"PktBurstSeqNo %lx PktBytesInPayload %lx PktType  PktDestQPId %lx PktPutOffset.mWhole %lx %lx %lx "
				"TranConn->PutOffset.mWhole %lx\n",
				(uint64_t)PerQP->mState, (uint64_t)PktSource, (uint64_t)PktNumberInBurst,
				(uint64_t)PktBurstSeqNo, (uint64_t)PktBytesInPayload, // (uint64_t)PktType,
				(uint64_t)PktDestQPId, (uint64_t) PktPutOffset.mWhole, (uint64_t) PktPutOffset.mBurstSeqNo,
				(uint64_t) PktPutOffset.mByteOffset, (uint64_t) TranConn->mExpectedHardwarePutOffset.mWhole);
			/*
			 * discard the frame by starting to expect the next frame --
			 * any remainging packets arriving from this burst will be dropped like dups
			 */
			if (PktNumberInBurst == PktPacketsInBurst) {
				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1 );
			} else {
				TranConn->mExpectedHardwarePutOffset.mByteOffset += PktMaxPayloadSize;
			}
			continue;
		}
		/*
		 * Do not process non-control-type packets if QP has RX supended due to previous error
		 */
		if ((PerQP->state_flags & ROQ_RX_SUSPENDED) && (PacketType != (PktT_SetupON | PktT_FirstON | PktT_LastON | PktT_CNTL))) {
			tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1 );

			continue;
		}
		/*
		 * Can this shake out in the state machine?
		 *
		 * o QP is in setup mode but packet is non-setup, or
		 * o QP is not in setup mode but packet is setup.
		 *
		 * -- discard burst
		 */
		if ((PerQP->mCurrent_Setup_Type == -1 && !(PacketType & PktT_SetupON)) ||
		    (PerQP->mCurrent_Setup_Type != -1 && (PacketType & PktT_SetupON))) {
			/*
			 * not currently setup and got non-setup burst or
			 * currently setup and got setup burst
			 */
			DPRINTF(1, "ERROR *** QP[%d] NOT SETUP ** DISCARDING NON-SETUP FRAME ** \n"
				"\tPerQP->mCurrent_Setup_Type %d PerQP->mState %lx PktSource %lx\n"
				"\tPktNumberInBurst %ld PktBurstSeqNo %ld PktBytesInPayload %ld \n"
				"\tBurstType 0x%lx PktDestQPId %ld PktPutOffset.mWhole 0x%lx 0x%lx 0x%lx \n"
				"\tTranConn->PutOffset.mWhole 0x%lx\n",
				PktDestQPId,
				PerQP->mCurrent_Setup_Type,
				(uint64_t)PerQP->mState, (uint64_t)PktSource, (uint64_t)PktNumberInBurst, (uint64_t)PktBurstSeqNo,
				(uint64_t)PktBytesInPayload, (uint64_t)PktBurstType, (uint64_t)PktDestQPId,
				(uint64_t)PktPutOffset.mWhole, (uint64_t)PktPutOffset.mBurstSeqNo, (uint64_t)PktPutOffset.mByteOffset,
				(uint64_t)TranConn->mExpectedHardwarePutOffset.mWhole);

			tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1 );
			roq_post_net_event(NET_EVENT_PROTOCOL_ERROR, &PktDestQPId, 0);
			continue;
		}
		/*
		 * There's no going back from here ... the packet will be processed as received.
		 * This packet will never be processed by the state machine again even if it is resent.
		 */

		/*
		 * figure out the first and last packet conditions and
		 * add them to the packet type
		 */
		if (PktNumberInBurst == 0) {

			PktT_SetFirst(PacketType);
#if defined(PKTRACE_ON)
			HitOE2(BurstRecvStart,"BurstRecv_PeerRank=%04dQPId_=%04d", 0, (int)TranConn->mPeerRank, PktDestQPId);
			// BurstRecvStart.HitOE(PKTRACE_BURST_RECV, "BurstRecv", 0, BurstRecvStart);
#endif
		}

		if (PktNumberInBurst == PktPacketsInBurst) {
			PktT_SetLast(PacketType);
                        // probably the burst offset should be advanced here. it's done inside the burst completion infrastructure right now.
                        }

		TranConn->mExpectedHardwarePutOffset.mByteOffset += PktMaxPayloadSize;

		DPRINTF(FXLOG_UCODE_PER_PACKET,
			"switch(PacketType) %lx PktT_Setup %lx PktT_First %lx PktT_Last %lx PktT_IB %lx "
			"IB WR OP CODE %lx (ROQ_OP_WRITE %lx ROQ_OP_SEND %lx ROQ_OP_READ %lx)\n",
			(uint64_t)PacketType, (uint64_t)(PacketType & PktT_SetupON ? 1 : 0),
			(uint64_t)(PacketType & PktT_FirstON ? 1 : 0), (uint64_t)(PacketType & PktT_LastON ? 1 : 0),
			(uint64_t)(PacketType & PktT_IB ? 1 : 0), (uint64_t)(PacketType & 0xF),
			(uint64_t)ROQ_OP_WRITE, (uint64_t)ROQ_OP_SEND, (uint64_t)ROQ_OP_READ);
#ifdef DEBUG_REMOTE_GET
		if (PacketType & ROQ_OP_READ) {
			DPRINTF(FXLOG_UCODE_PER_PACKET, "Got READ pkt. Setup = %s, First = %s, Last = %s, DMA_GET = %s\n",
				PacketType & PktT_SetupON ? "Y" : "N", PacketType & PktT_FirstON ? "Y" : "N", 
				PacketType & PktT_LastON ? "Y" : "N", PacketType & PktT_DMA_ON ? "Y" : "N");
			fflush(stdout);
		}
		if ((PacketType & 0x3) == 0) { 
			DPRINTF(FXLOG_UCODE_PER_PACKET, "Got WRITE pkt. Setup = %s, First = %s, Last = %s, DMA_GET = %s\n",
				PacketType & PktT_SetupON ? "Y" : "N", PacketType & PktT_FirstON ? "Y" : "N", 
				PacketType & PktT_LastON ? "Y" : "N", PacketType & PktT_DMA_ON ? "Y" : "N");
			fflush(stdout);
		}
#endif

		switch (PacketType) {

		case (PktT_SetupON | PktT_FirstON | PktT_LastON | PktT_CNTL | ROQ_OP_NULL):
			/*****************
			 *** RDMA CNTL ***
			 *****************/
			DPRINTF(FXLOG_UCODE_CNTL,
				"%s %p\n",
				"RX CNTL DISCONNECT case (PktT_SetupON | PktT_FirstON | PktT_LastON | PktT_CNTL | ROQ_OP_NULL) :",
				(void *)(PktT_SetupON|PktT_FirstON|PktT_LastON|PktT_CNTL|ROQ_OP_NULL));

			ppc_msync();

			TranConn->mCurrentRecvBurst_QP_Ptr = NULL;

			tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_CNTL,
					       PerQP->mCurrentReceiveOffset, (uint64_t) &RoQ_DevMem->mDD_DoorbellQueueSet, 1);

			break;

		case (PktT_SetupON | PktT_FirstON | PktT_LastON | PktT_IB | ROQ_OP_WRITE):
			/*****************
			 *** RDMA WRITE **
			 *****************/

			DPRINTF(FXLOG_UCODE_PER_MESSAGE,
				"case (PktT_SetupON|PktT_FirstON|PktT_LastON|PktT_IB|ROQ_OP_WRITE [%d]) :\n", 0);
#if defined(PKTRACE_ON)
			HitOE(RDMA_WRITE_Start,"RDMA_WRITE", 0);
			// RDMA_WRITE_Start.HitOE(PKTRACE_RDMA_WRITE, "RDMA_WRITE", 0, RDMA_WRITE_Start);
#endif
			/*
			 * this case to handle writes that fit in one packet or to start multipacket writes
			 * and writes that need set up for a segment dma
			 * segments are transfers up to one virtual memory page worth of data
			 * transfer segments will not cross page boundaries
			 * expect 64KB pages and max 64KB segments
			 * This is a two chunk, one packet, single segment
			 * Note: we are using the HW PutOffset to tell when the last packet of the segment
			 */

			qp = RoQ_GetQPPtr_Idx(PktDestQPId);

			DASSERT(PerQP->mPeerQPId.mWhole, "qp->mPeerQPId is (%d) zero ... not allowed", PerQP->mPeerQPId.mWhole);

			/* NEED: check the state -- make sure QP is idle, etc */
			DASSERT(PerQP->mTranConn == TranConn, "%p %p %d %d",
				PerQP->mTranConn, TranConn, PerQP->mCurrent_Setup_Type, PerQP->mState);

			TranConn->mCurrentRecvBurst_QP_Ptr = qp;

			/*
			 * copy the chunks required to the TransportConnection buffer
			 */
			PerQP->mCurrent_RDMA_OP_DDP_Fields = *((struct RoQ_DDP_Fields *)PktPayloadPtr);

			DPRINTF(FXLOG_UCODE_PER_BURST,
				"RX WRITE SETUP qpn %lx qp@ 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx "
				"->DDP.Write.SinkTag 0x%lx ->DDP.RDMA_Flags 0x%lx\n",
				(uint64_t)PktDestQPId, (uint64_t)qp,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);

			/*
			 * Since setup is QP specific and the setup burst maybe
			 * followed by a differnt qp's burst, the QP and MR version
			 * number cannot be stored in TranConn
			 */
			TranConn->mCurrentRecvBurst_QP_VersionNumber =
				TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber;

			if (RoQ_Get_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key) <= 0) {
				roq_rx_close_on_error(PerQP, NET_EVENT_MEM_ACCESS_ERROR);

				break;
			}
			/* use the tag to get a memory region pointer */
			TranConn->mCurrentRecvBurst_MR_Ptr =
				RoQ_Get_MR_Ptr_From_RDMA_Key(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);

			/* XXX That of course must be delayed until WRITE got finished */
			RoQ_Put_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);

			DSTRONG_ASSERT(TranConn->mCurrentRecvBurst_MR_Ptr, "%p", TranConn->mCurrentRecvBurst_MR_Ptr);

			DPRINTF(FXLOG_UCODE_PER_BURST,
				"RX WRITE SETUP mr@ 0x%lx ->key 0x%lx ->rkey 0x%lx ->mBaseVirtualAddress 0x%lx "
				"->mSize 0x%lx ->mRights 0x%lx ->mVersionNumber 0x%lx\n",
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->lkey,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->rkey,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mSize,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mRights,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mVersionNumber);

#ifdef ROQ_ASSERT_ON_ZERO_WR_VALUES
			DASSERT(TranConn->mCurrentRecvBurst_MR_Ptr->lkey, "");
			DASSERT(TranConn->mCurrentRecvBurst_MR_Ptr->rkey, "");
#endif

			TranConn->mCurrentRecvBurst_MR_VersionNumber = TranConn->mCurrentRecvBurst_MR_Ptr->mVersionNumber;

			/*
			 * Validate this MR for use with this QP
			 *
			 * NEED: do something real -- for now, let them all be valid
			 *
			 * if the RDMA op is absolute addressing, we need to
			 * subtract the mrs virtual memory base to get an absoute offest
			 */
			if(!(PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags & ROQ_WQE_ABSOLUTE_ADDR)) {
				DPRINTF(1,
					"RX WRITE SETUP qpn %lx qp@ 0x%p RELATIVE ADDRESSING\n",
					(uint64_t)PktDestQPId, (void *)qp);

				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr -=
					TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress;
			}

			MsgLastByteAddr = PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr
					  + PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length - 1;

			LastByteOfMR = TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress
				       + TranConn->mCurrentRecvBurst_MR_Ptr->mSize - 1;

			if (MsgLastByteAddr > LastByteOfMR) {
				DPRINTF(1, "RX ERROR: This segment would write data outside the MR MsgLastByteAddr 0x%p LastByteOfMR 0x%p "
					"TranConn->mCurrentRecvBurst_MR_Ptr@ 0x%p ->mSize %lx\n",
					(void *)MsgLastByteAddr, (void *)LastByteOfMR,
					(void *)&TranConn->mCurrentRecvBurst_MR_Ptr,
					(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mSize);

				roq_rx_close_on_error(PerQP, NET_EVENT_MEM_BASE_BOUNDS_ERROR);

				break;
			}

#ifdef RoQ_SEND_USER_DATA_IN_SETUP
			/*
			 * check if we'll be completing this RDMA WRITE with data
			 * in the setup packet.
			 */
			if (PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length <= RoQ_USER_DATA_IN_SETUP_SIZE && (PktBurstType & PktT_LastBurstOfMsgON)) {
//			if (PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length <= RoQ_USER_DATA_IN_SETUP_SIZE) {
				DPRINTF(FXLOG_UCODE_PER_BURST,
					"RX SHORT WRITE place data from 0x%lx to (inclusive) 0x%lx (length) %lx in STag %lx from 0x%lx to (inclusive) 0x%lx (MR size) %lx\n",
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
					(uint64_t)MsgLastByteAddr,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
					(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress,
					(uint64_t)(TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress + TranConn->mCurrentRecvBurst_MR_Ptr->mSize - 1),
					(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mSize);

				DPRINTF(FXLOG_UCODE_PER_PACKET,
					"SHORT WRITE BODY qpn %lx ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength%lx ->DDP.RDMA_Flags 0x%lx\n",
					(uint64_t)PktDestQPId, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);


				if (PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length) {
					int virtual_page_a = PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr & ~RoQ_BURST_MASK;
					int virtual_page_b = MsgLastByteAddr & ~RoQ_BURST_MASK;

					char *payload_ptr = ((char*)PktPayloadPtr) + 32; /* plus sizeof setup chunk -- shouldn't be a literal really. */
					uint32_t key = PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key;

					if (RoQ_Get_MR_Write(key) <= 0) {
						roq_rx_close_on_error(PerQP, NET_EVENT_MEM_ACCESS_ERROR);

						break;
					}

					if (virtual_page_a == virtual_page_b) {
						/*
						 * Short WRITE does not span page boundary
						 */
						RoQ_DmaAddr = RoQ_HostVa2DmaAddr(key,
										 PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
										 PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
										 __LINE__);
						RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

						DPRINTF(FXLOG_UCODE_PER_BURST,
							"RX SHORT WRITE PLACING DATA qpn %lx stag %lx vaddr 0x%lx paddr 0x%lx len %lx\n",
							(uint64_t)PktDestQPId, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
							(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
							(uint64_t)RoQ_DmaAddr, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length);

						if (RoQ_VirtAddr)
							memcpy((void *)RoQ_VirtAddr, (void *)payload_ptr, PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length);

						RoQ_Log16Words("RX SHORT WRITE DATA", (char*)RoQ_VirtAddr, PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length);
					} else {
						/*
						 * Short WRITE spans a page boundary and will
						 * require two page lookup/memcpy calls
						 */
						int len2 = (MsgLastByteAddr & RoQ_BURST_MASK) + 1;
						int len1 = PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length - len2;

						uint64_t RoQ_DmaAddr2, RoQ_VirtAddr2;

						RoQ_DmaAddr = RoQ_HostVa2DmaAddr(key,
										 PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
										 len1,
										 __LINE__);
						RoQ_DmaAddr2 = RoQ_HostVa2DmaAddr(key,
										  PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr + len1,
										  len2,
										  __LINE__);
						RoQ_VirtAddr  = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);
						RoQ_VirtAddr2 = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr2);

						DPRINTF(FXLOG_UCODE_PER_BURST,
							"RX SHORT WRITE PLACING DATA (1 of 2 PAGES) qpn %lx stag %lx vaddr 0x%lx paddr 0x%lx len %lx\n",
							(uint64_t)PktDestQPId,
							(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
							(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
							(uint64_t)RoQ_DmaAddr, (uint64_t)len1);

						if (RoQ_VirtAddr)
							memcpy((void *)RoQ_VirtAddr, (void *)payload_ptr, len1);

						RoQ_Log16Words("RX SHORT WRITE DATA", (char*)RoQ_VirtAddr, len1);

						DPRINTF(FXLOG_UCODE_PER_BURST,
							"RX SHORT WRITE PLACING DATA (1 of 2 PAGES) qpn %lx stag %lx vaddr 0x%lx paddr 0x%lx len %lx\n",
							(uint64_t)PktDestQPId,
							(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
							(uint64_t)(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr + len1),
							(uint64_t)RoQ_DmaAddr2, (uint64_t)len2);

						if (RoQ_VirtAddr2)
							memcpy((void *)RoQ_VirtAddr2, (void *)(payload_ptr + len1), len2);

						RoQ_Log16Words("RX SHORT WRITE DATA", (char*)RoQ_VirtAddr2, len1);
					}
					RoQ_Put_MR_Write(key);
				}

                                DASSERT( PktBurstType & PktT_LastBurstOfMsgON, "PktT_LastBurstOfMsgON was not on where it should have been.\n");
				// We just handled in-line data in the setup burst so we post an ack here and complete an RDMA read perhaps
				if (PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags & ROQ_WQE_READ_RESPONSE)
       					tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_WRITE_FOR_READ, PerQP->mCurrentReceiveOffset, -1, 1);
       				else
       					tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1);

       				TranConn->mCurrentRecvBurst_QP_Ptr = NULL;
       				PerQP->mCurrent_Setup_Type = -1; // probably redundant
			} else
#endif // in Setup data option
				{
				/*
				 * offset of arriving data into receive message area
				 * NEED: this might be useful or even duplicated for send or ip messages.
				 */
				PerQP->mCurrentReceiveOffset = 0;

				TranConn->mRecvBurstNextPhysAddr = PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr;

				DPRINTF(FXLOG_UCODE_PER_BURST,
					"RX WRITE SETUP place data from 0x%lx to (inclusive) 0x%lx (length) %lx in STag %lx from 0x%lx to (inclusive) 0x%lx (size) %lx\n",
					(uint64_t)TranConn->mRecvBurstNextPhysAddr, (uint64_t)MsgLastByteAddr,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
					(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress,
					(uint64_t)(TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress + TranConn->mCurrentRecvBurst_MR_Ptr->mSize - 1),
					(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mSize);

				PerQP->mCurrent_Setup_Type = ROQ_OP_WRITE;
                                // Setup burst complete -- message still to come in following burst(s).
				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 0 );
			}
			TranConn->mCurrentRecvBurst_QP_Ptr = NULL;

			break;

		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastOFF | PktT_IB | /**/ PktT_DMA_ON /**/ | ROQ_OP_WRITE):
                        // consider asserting here assuming we have a multi-packet threashold for using DMA -- LastON would be wrong.
			DASSERT(0,"Bad PacketType");
		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastON  | PktT_IB | /**/ PktT_DMA_ON /**/ | ROQ_OP_WRITE):
			/*
			 * Starts a burst that will be handled by BG/Q DMA MU GET.
			 */
			PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
			qp    =  RoQ_GetQPPtr_Idx(PktDestQPId);

			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || PerQP->mPeerQPId.mWhole, "qp->mPeerQPId is %d zero ... not allowed", PerQP->mPeerQPId.mWhole);
			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || PerQP->mTranConn == TranConn, "%p %p", PerQP->mTranConn, TranConn);

			TranConn->mCurrentRecvBurst_QP_Ptr = qp;

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"FIRST OF WRITE BODY BURST qpn %lx qp@ 0x%lx ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx ->DDP.RDMA_Flags 0x %lx\n",
				(uint64_t)PktDestQPId, (uint64_t)qp,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);
			fflush(stdout);

			DSTRONG_ASSERT(TranConn->mCurrentRecvBurst_QP_VersionNumber == TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber,
					"%d %lld", TranConn->mCurrentRecvBurst_QP_VersionNumber, TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber);

			DASSERT(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr != 0 && PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				"XXX 0x%llu sink address, key %u\n",
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);
			/*
			 * Post BGQ MU DMA GET here. Main loop will poll for counter and generate a fake "last packet in burst" event
			 */
			// Get the next available counter -- big problem if we don't have any more!!!
			//
			union RDMA_SetupChunk		*SetupChunk = (union RDMA_SetupChunk*)PktPayloadPtr;
			uint64_t Remote_DmaAddr = SetupChunk->BGQ_MU_DMA_Get_Setup.mDmaAddr;
			uint32_t Remote_DmaLen  = SetupChunk->BGQ_MU_DMA_Get_Setup.mDmaLen;

			if (RoQ_Get_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key) <= 0) {
				RoQ_DmaAddr = (uint64_t)RoQ_Trashcan;
			} else {
				RoQ_DmaAddr = RoQ_HostVa2DmaAddr(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
								 PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr + PerQP->mCurrentReceiveOffset,
								 Remote_DmaLen, //// PktPacketsInBurst * 512 + PktBytesInLastPacket,
								 __LINE__);

				/* XXX That of course must be delayed until DMA got finished */
				RoQ_Put_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);
			}
			if (!RoQ_DmaAddr) {
				RoQ_DmaAddr = (uint64_t)RoQ_Trashcan;
				DPRINTF(DEBUG_APPL_OOB, "XXX 0 sink address:: offset %d, PktPacketsInBurst %d, PktBytesInLastPacket %d, key %u, addr %llx, RR %u\n",
					PerQP->mCurrentReceiveOffset, PktPacketsInBurst, PktBytesInLastPacket,
					PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
					PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
					PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags & ROQ_WQE_READ_RESPONSE);
			}
			RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

			///DPRINTF(FXLOG_UCODE_PER_PACKET,
			DASSERT( RoQ_DmaAddr,
				"FIRST OF WRITE BODY BURST qpn %lx qp@ 0x%lx ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx ->DDP.RDMA_Flags 0x %lx\n",
				(uint64_t)PktDestQPId, (uint64_t)qp,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);
			/*
			 * The transport connection from the peer to this node can now
			 * be focused on receiving packets and placing them in memory
			 */
			//////TranConn->mRecvBurstNextPhysAddr = RoQ_VirtAddr;

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"RX WRITE BODY FIRST OF BURST qpn %lx qp@ 0x%lx TranConn->mRecvBurstNextPhysAddr %lx SinkOffset %lx PerQP->mCurrentReceiveOffset %lx\n",
				(uint64_t)PktDestQPId, (uint64_t)qp,
				(uint64_t)TranConn->mRecvBurstNextPhysAddr,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				(uint64_t)PerQP->mCurrentReceiveOffset);


///			DPRINTF(FXLOG_REMOTE_GET,"(*)(*) Got DMA ON WRITE. Before RemoteGet DMA Post addr %p, len %d\n", Remote_DmaAddr, Remote_DmaLen );
//   			fflush(stdout);

			// These two line are here and then also in the tc_PostBurstCompletion() -- might want to think about that
			uint64_t CurrentBurstToAckSeqNo = TranConn->mBurstSeqNo_NextToRecv;
			uint64_t BurstAckSlot           = CurrentBurstToAckSeqNo & RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT_MASK;

			TranConn->mPeerRank = PktSource;
			TX_MU_RemoteGet( TranConn->PeerNodeAddr, RoQ_DmaAddr, Remote_DmaAddr, Remote_DmaLen, TranConn, BurstAckSlot );
			TranConn->mRecvBurstNextPhysAddr = 0;

			/* update the tally of bytes received for this message */
			PerQP->mCurrentReceiveOffset += Remote_DmaLen;

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"RX WRITE BODY BURST LAST PKT qpn %lx qp@ 0x%p mRecvBurstNextPhysAddr 0x%p PktByteOffsetInBurst %lx "
				"PktBytesInLastPacket %lx PerQP->mCurrentReceiveOffset %lx Expected Total Msg Len %lx\n",
				(uint64_t)PktDestQPId, (void *)qp, (void *)TranConn->mRecvBurstNextPhysAddr,
				(uint64_t)PktByteOffsetInBurst, (uint64_t)PktBytesInLastPacket,
				(uint64_t)PerQP->mCurrentReceiveOffset,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length);

			/*
			 * sense the end of an RDMA op -- note that this can only happen
			 * on a PtT_LastON pkt so this branch can be handled in the switch
			 */
			DASSERT(PerQP->mCurrentReceiveOffset <= PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length, "over ran message length (%d)\n", 0);

			if( PktBurstType & PktT_LastBurstOfMsgON ) {

				DASSERT(PerQP->mCurrentReceiveOffset == PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length, " Burst End Flag on -- bytes not placed yet ");

				// msg is done -- this was the last burst
				if (PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags & ROQ_WQE_READ_RESPONSE)
					tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_IN_PROGRESS, ROQ_BURST_COMPLETE_WRITE_FOR_READ, PerQP->mCurrentReceiveOffset, -1 /* WRITE FOR READ */, 1 );
				else
					tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_IN_PROGRESS, ROQ_BURST_COMPLETE_NO_ACTION, PerQP->mCurrentReceiveOffset, 0 /* WRITE */, 1 );

				PerQP->mCurrent_Setup_Type = -1;
			} else {
				// msg isn't done -- more bursts to come
				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_IN_PROGRESS, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 0 );
			}

			PerQP->mAsyncBurstsInProgress++;   // We have posted ROQ_BURST_IN_PROGRESS. This will be decremented when this burst is complete. A 0 value allows faster READ processing.
			TranConn->mCurrentRecvBurst_QP_Ptr = NULL;

			  DPRINTF(FXLOG_REMOTE_GET,"(*)(*) Got DMA ON WRITE. Done RemoteGet DMA addr %p, len %d\n", Remote_DmaAddr, Remote_DmaLen );

			break;

		case (PktT_SetupOFF | PktT_FirstOFF  | PktT_LastOFF | PktT_IB | /**/ PktT_DMA_ON /**/ | ROQ_OP_WRITE):
				// this is an assert. DMA generates a FirstOFF LastON packet and no intermediates.
				DASSERT(0,"Bad PacketType");
		case (PktT_SetupOFF | PktT_FirstOFF  | PktT_LastON  | PktT_IB | /**/ PktT_DMA_ON /**/ | ROQ_OP_WRITE):
				// this will be similar in function to the same tansition without DMA on
				DASSERT(0,"transition not implemented");

		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastON  | PktT_IB | ROQ_OP_WRITE):
		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastOFF | PktT_IB | ROQ_OP_WRITE):
				/*
				 * This case will handle both body and last packet since the code is fairly similar
				 * NEED: Pull out WRITE_BODY for non-last that has few branches and known numbers of placed bytes
				 */
				PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
				qp = RoQ_GetQPPtr_Idx(PktDestQPId);

				DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || PerQP->mPeerQPId.mWhole, "qp->mPeerQPId is %d zero ... not allowed", PerQP->mPeerQPId.mWhole);
				DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || PerQP->mTranConn == TranConn, "%p %p", PerQP->mTranConn, TranConn);

				TranConn->mCurrentRecvBurst_QP_Ptr = qp;


				DPRINTF(FXLOG_UCODE_PER_PACKET,
					"FIRST OF WRITE BODY BURST qpn %lx qp@ 0x%lx ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx ->DDP.RDMA_Flags 0x %lx\n",
					(uint64_t)PktDestQPId, (uint64_t)qp,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);

				DSTRONG_ASSERT(TranConn->mCurrentRecvBurst_QP_VersionNumber == TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber,
						"%d %lld", TranConn->mCurrentRecvBurst_QP_VersionNumber, TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber);

				if (RoQ_Get_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key) <= 0) {
					roq_rx_close_on_error(PerQP, NET_EVENT_MEM_ACCESS_ERROR);

					goto get_next_packet;
				} else {
					RoQ_DmaAddr = RoQ_HostVa2DmaAddr(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
									 PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr + PerQP->mCurrentReceiveOffset,
									 PktPacketsInBurst * 512 + PktBytesInLastPacket,
									 __LINE__);
					if (!RoQ_DmaAddr) {
						RoQ_Put_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);
						roq_rx_close_on_error(PerQP, NET_EVENT_MEM_BASE_BOUNDS_ERROR);

						goto get_next_packet;
					}
					RoQ_Put_MR_Write(PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key);
				}
			RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

			/*
			 * The transport connection from the peer to this node can now
			 * be focused on receiving packets and placing them in memory
			 */
			TranConn->mRecvBurstNextPhysAddr = RoQ_VirtAddr;

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"RX WRITE BODY FIRST OF BURST qpn %lx qp@ 0x%lx TranConn->mRecvBurstNextPhysAddr %lx SinkOffset %lx PerQP->mCurrentReceiveOffset %lx\n",
				(uint64_t)PktDestQPId, (uint64_t)qp,
				(uint64_t)TranConn->mRecvBurstNextPhysAddr,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				(uint64_t)PerQP->mCurrentReceiveOffset);

			/* Fall through */

		case (PktT_SetupOFF | PktT_FirstOFF | PktT_LastON  | PktT_IB | ROQ_OP_WRITE):
		case (PktT_SetupOFF | PktT_FirstOFF | PktT_LastOFF | PktT_IB | ROQ_OP_WRITE):

			PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];

			/*
			 * CURRENTLY NOT HANDLING MISALIGNED PACKETS
			 * EXPECT TO USE QUADWORD MEMCPY ONLY RIGHT NOW
			 * need to place the first part of the packet and the last part
			 * this allows the middle to be aligned for quad word moves.
			 */

			/*  get the qp from the tranport connection state -- NEED: assert this is right */
			qp = TranConn->mCurrentRecvBurst_QP_Ptr;

			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || qp, "%p", qp);
			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || (qp == RoQ_GetQPPtr_Idx(PktDestQPId)), "%p %p", qp, RoQ_GetQPPtr_Idx(PktDestQPId));
			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || (PerQP->mTranConn == TranConn), "%p %p", PerQP->mTranConn, TranConn);

			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || (PerQP->mCurrent_Setup_Type == ROQ_OP_WRITE),
				"RX WRITE BODY qpn %lx qp@ 0x%p ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx ->DDP.RDMA_Flags 0x%lx\n",
				(uint64_t)PktDestQPId, (void *)qp,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"RX WRITE BODY qpn %lx qp@ 0x%p ->DDP.Write.SinkTag 0x%lx ->DDP.Write.SinkOffset 0x%lx ->DDP.MessageLength %lx ->DDP.RDMA_Flags 0x%lx\n",
				(uint64_t)PktDestQPId, (void *)qp,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.key,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.addr,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
				PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags);

			DASSERT(!ROQ_DO_COSTLY_PROTOCOL_CHECKS || (TranConn->mCurrentRecvBurst_MR_Ptr), "%p", TranConn->mCurrentRecvBurst_MR_Ptr);

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"RX WRITE BODY mr@ 0x%lx ->lkey 0x%lx ->rkey 0x%lx ->mBaseVirtualAddress 0x%lx ->mSize 0x%lx ->mRights 0x%lx ->mVersionNumber 0x%lx\n",
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->lkey,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->rkey,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mBaseVirtualAddress,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mSize,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mRights,
				(uint64_t)TranConn->mCurrentRecvBurst_MR_Ptr->mVersionNumber);

			DSTRONG_ASSERT(TranConn->mCurrentRecvBurst_QP_VersionNumber == TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber,
					"%d %lld", TranConn->mCurrentRecvBurst_QP_VersionNumber, TranConn->mCurrentRecvBurst_QP_Ptr->mVersionNumber);

#if ROQ_DO_COSTLY_PROTOCOL_CHECKS==1
			DPRINTF((TranConn->mRecvBurstNextPhysAddr & ~RoQ_BURST_MASK) != ((TranConn->mRecvBurstNextPhysAddr + PktBytesInPayload - 1) & ~RoQ_BURST_MASK),
				"RDMA WRITE BURST CROSSES PAGE BOUNDARY ON RX starting at 0x%p placing %lx bytes PAGE_MASK 0x%p\n",
				(void *)TranConn->mRecvBurstNextPhysAddr, (uint64_t)PktBytesInPayload, (void *)RoQ_BURST_MASK);
#endif

			if (!(PacketType & PktT_LastON)) {
				/* NOT the last packet of the burst */
				DPRINTF(FXLOG_UCODE_PER_PACKET,
					"RX WRITE BODY MIDDLE PKT qpn %lx qp@ 0x%p NextPhysAddr 0x%p PktMaxPayloadSize %lx\n",
					(uint64_t)PktDestQPId, (void *)qp, (void *)TranConn->mRecvBurstNextPhysAddr,
					(uint64_t)PktMaxPayloadSize);

#if defined(PKTRACE_ON)
				HitOE(RDMA_WRITE_MEMCPY_Start, "RDMA_WRITE_MEMCPY", 0);
				// RDMA_WRITE_MEMCPY_Start.HitOE(PKTRACE_RDMA_WRITE_MEMCPY, "RDMA_WRITE_MEMCPY", 0, RDMA_WRITE_MEMCPY_Start);
#endif
				if (((uint64_t)TranConn->mRecvBurstNextPhysAddr & 0x1F) == 0) {
					/* we believe the in bound packet payload is 32B aligned */
					FastMove_512((unsigned long long *)TranConn->mRecvBurstNextPhysAddr, (unsigned long long *)PktPayloadPtr);
				} else {
					memcpy((void *)TranConn->mRecvBurstNextPhysAddr, (void *)PktPayloadPtr, PktMaxPayloadSize);
				}

#if defined(PKTRACE_ON)
				HitOE(RDMA_WRITE_MEMCPY_Finis,"RDMA_WRITE_MEMCPY", 0);
				// RDMA_WRITE_MEMCPY_Finis.HitOE(PKTRACE_RDMA_WRITE_MEMCPY, "RDMA_WRITE_MEMCPY", 0, RDMA_WRITE_MEMCPY_Finis);
#endif
				TranConn->mRecvBurstNextPhysAddr += PktMaxPayloadSize;
			} else {
				/* this is the last packet of the burst */
				DASSERT(TranConn->mRecvBurstNextPhysAddr,
					"data placement address is NULL (%d)\n", 0);
				memcpy((void *)TranConn->mRecvBurstNextPhysAddr, (void *)PktPayloadPtr, PktBytesInLastPacket);

				TranConn->mRecvBurstNextPhysAddr = 0;

				/* update the tally of bytes received for this message */
// can't do this here
// and can't leave things undone in the PerQP
				PerQP->mCurrentReceiveOffset += PktBytesInLastPacket + PktByteOffsetInBurst;

				DPRINTF(FXLOG_UCODE_PER_PACKET,
					"RX WRITE BODY BURST LAST PKT qpn %lx qp@ 0x%p mRecvBurstNextPhysAddr 0x%p PktByteOffsetInBurst %lx "
					"PktBytesInLastPacket %lx PerQP->mCurrentReceiveOffset %lx Expected Total Msg Len %lx\n",
					(uint64_t)PktDestQPId, (void *)qp, (void *)TranConn->mRecvBurstNextPhysAddr,
					(uint64_t)PktByteOffsetInBurst, (uint64_t)PktBytesInLastPacket,
					(uint64_t)PerQP->mCurrentReceiveOffset,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length);

				/*
				 * sense the end of an RDMA op -- note that this can only happen
				 * on a PtT_LastON pkt so this branch can be handled in the switch
				 */
				DASSERT(PerQP->mCurrentReceiveOffset <= PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length,
					"over ran message length (%d)\n", 0);

        			if( PacketType & PktT_LastON ) {
//					/* Allow for SGL's */
//					PerQP->mCurrent_Setup_Type = -1;
                                        if( PktBurstType & PktT_LastBurstOfMsgON ) {
						DASSERT(PerQP->mCurrentReceiveOffset == PerQP->mCurrent_RDMA_OP_DDP_Fields.Write.SinkSGE.length, " Burst End Flag on -- bytes not placed yet ");
						// msg is done -- this was the last burst
						if (PerQP->mCurrent_RDMA_OP_DDP_Fields.RDMA_Flags & ROQ_WQE_READ_RESPONSE)
        						tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_WRITE_FOR_READ, PerQP->mCurrentReceiveOffset, -1, 1 );
						else
        						tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, PerQP->mCurrentReceiveOffset, 0, 1 );

        					PerQP->mCurrent_Setup_Type = -1;
        				} else {
        					// msg isn't done -- more bursts to come
        					tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 0 );
        				}
        				TranConn->mCurrentRecvBurst_QP_Ptr = NULL;
        			}
				/*
				 * TranConn done with burst for this endpoint pair
				 * (that is on this qp)
				 */
				TranConn->mCurrentRecvBurst_QP_Ptr = NULL;
			}

			break;

	      case (PktT_SetupON  | PktT_FirstON | PktT_LastON  | PktT_IB | ROQ_OP_SEND):
			/****************
			 *** RDMA SEND **
			 ****************/
			PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
			qp = RoQ_GetQPPtr_Idx(PktDestQPId);

			if (PerQP->mState > ROQ_QPS_SQD) {
				/*
				 * TODO: Surfacing asynchronous event to Linux DD
				 */
				DPRINTF(1, "QP[%d]: State %d: Cannot handle SEND\n", PktDestQPId, PerQP->mState);	
				goto get_next_packet;
			}

			DPRINTF(FXLOG_UCODE_PER_MESSAGE,
				"case (PktT_SetupON  | PktT_FirstON | PktT_LastON  | PktT_IB | ROQ_OP_SEND) : PktSource %lx qpn %lx\n",
				(uint64_t)PktSource, (uint64_t)PktDestQPId);

			DASSERT(qp, "%p", qp);

			/*
			 * NEED: check the state -- make sure QP is idle, etc
			 * this is done when the Pkt_FirstON is set AssertLogLine(TranConn->mCurrentRecvBurst_QP_Ptr == NULL) << EndLogLine;
			 * setup bursts are single packet and do not set the current ptr TranConn->mCurrentRecvBurst_QP_Ptr = qp;
			 *
			 * copy the chunks required to the TransportConnection buffer
			 * ChunkMemCpy((uint64_t) &PerQP->mCurrent_RDMA_OP_DDP_Fields.AsChunk, (uint64_t)MU_PayloadAsChunks, 1);
			 */
			PerQP->mCurrent_RDMA_OP_DDP_Fields = *((struct RoQ_DDP_Fields *)PktPayloadPtr);

			DPRINTF(FXLOG_UCODE_PER_MESSAGE,
				"qpn %lx RDMA SETUP  Current DDP_Fields.Send... .MSN %lx .MessageLength %lx .MO %lx\n",
				(uint64_t)PktDestQPId,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MSN,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MO);

			/* get the next buffer available to the rdma connection */
			struct KISS_SRSWQ   *RecvQueuePtr;

			if (qp->SharedRecvQueue) {
				SpinLock(&qp->mRecvQueueLock);
				RecvQueuePtr = (struct KISS_SRSWQ *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->SharedRecvQueue);
			} else
				RecvQueuePtr = &qp->mRecvQueue;

			/*
			 * Start a new receive only if no receive is currently in progress
			 */
			if (!(PerQP->mCurrent_RQE_ValidatedCopy.flags & ROQ_WQE_VALID))  {
				volatile struct roq_rqe *rq_array;
				int retry_cnt = 0;

				rq_array = (volatile struct roq_rqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS(
								(uint64_t *)(RecvQueuePtr->mItemArrayDmaAddr));

				PerQP->mCurrent_RQE_Index = RecvQueuePtr->mGot % RecvQueuePtr->mItemCount;
retry_rq:
				if (_load_shared(rq_array[PerQP->mCurrent_RQE_Index].flags) & ROQ_WQE_VALID) {

					memcpy((void *)&PerQP->mCurrent_RQE_ValidatedCopy, (void *)&rq_array[PerQP->mCurrent_RQE_Index],
					       sizeof(struct roq_rqe));

					rqe = &PerQP->mCurrent_RQE_ValidatedCopy;
					/*
					 * Mark user RQE as free
					 */
					_store_shared(rq_array[PerQP->mCurrent_RQE_Index].flags, ROQ_WQE_COMPLETED);
					RecvQueuePtr->mGot++;

					/*
					 * first swizzle the sg_list which is a pointer to a
					 * s_list in the work request
					 */
					if (rqe->num_sge > ROQ_MAX_SGE) {
						DASSERT(1, "RX: Too many SGE's, trimming: %d -> %d\n", rqe->num_sge, (int)ROQ_MAX_SGE);
						rqe->num_sge = ROQ_MAX_SGE;
					}
					PerQP->Current_RecvSgList = &rqe->sge[0];

					DPRINTF(FXLOG_UCODE_PER_PACKET,
						"rqe 0x%p in dd@ 0x%p ->num_sge %lx ->sg_list 0x%p\n",
						(void *)rqe, (void *)&rq_array[PerQP->mCurrent_RQE_Index],
						(uint64_t)rqe->num_sge,
						PerQP->Current_RecvSgList);


					/* These structures track the receive operation which may be many bursts */
					PerQP->mCurrent_RQE_SGE_List_Index     = 0;
					PerQP->mCurrent_RQE_SGE_Offset         = 0;
					PerQP->mCurrent_RQE_TotalReceivedBytes = 0;

					if (qp->SharedRecvQueue)
						SpinUnlock(&qp->mRecvQueueLock);
				} else {
					if (++retry_cnt <= 1000) {
						ppc_msync();
						goto retry_rq;
					} else {
						if (retry_cnt > 1002) {
#if 0
							roq_print_workqueues(PktDestQPId);
#endif
							DPRINTF(1, "QP[%d]: State %d: Error: RX got SEND message with no posted recv "
								"-- discarding setup burst\n", PktDestQPId, PerQP->mState);

							roq_rx_close_on_error(PerQP, NET_EVENT_RQ_EMPTY);

							goto get_next_packet;
						} else {
#if 1
							sleep(1);
#endif
							ppc_msync();
							goto retry_rq;
						}
					}

				}
			} else {
				/*
				 * Parallel receive operations unsupported
				 */
				DPRINTF(1, "Error:: QP[%d]: Cannot start new Receive while last incomplete\n", (int)PktDestQPId);

				if (qp->SharedRecvQueue)
					SpinUnlock(&qp->mRecvQueueLock);

				roq_rx_close_on_error(PerQP, NET_EVENT_PROTOCOL_ERROR);

				goto get_next_packet;
			}
#ifdef RoQ_SEND_USER_DATA_IN_SETUP
			/*
			 * check if we'll be completing this RDMA Send with data
			 * in the setup packet.
			 */
			if (PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength <= RoQ_USER_DATA_IN_SETUP_SIZE && (PktBurstType & PktT_LastBurstOfMsgON)) {
//			if (PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength <= RoQ_USER_DATA_IN_SETUP_SIZE) {
				int payload_len = PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength;
				struct roq_dm_cq *roq_cq =
					(struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->mRecvCQ_DmaPtr);
				char *payload_ptr = ((char *)PktPayloadPtr) + 32; /* plus sizeof setup chunk */

				while (payload_len > 0) {
					struct roq_sge *sge = &PerQP->Current_RecvSgList[PerQP->mCurrent_RQE_SGE_List_Index];
					int BytesToPlaceInThisSGE = payload_len;

					if (RoQ_Get_MR_Write(sge->key) <= 0) {
						roq_rx_close_on_error(PerQP, NET_EVENT_MEM_ACCESS_ERROR);

						goto get_next_packet;
					}

					DASSERT(PerQP->mCurrent_RQE_SGE_Offset <= sge->length,
						"over ran sge area  offest %lx sge->length %lx\n",
						(uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)sge->length);
					/*
					 * advance the iterator info going through the SGE if required
					 * note -- let unadvanced at the bottom of the loop or with a zero
					 * len sg element that isn't the end of the list
					 */
					if (PerQP->mCurrent_RQE_SGE_Offset == sge->length) {
						PerQP->mCurrent_RQE_SGE_List_Index++;
						PerQP->mCurrent_RQE_SGE_Offset = 0;

						if (PerQP->mCurrent_RQE_SGE_List_Index == rqe->num_sge) {
							RoQ_Put_MR_Write(sge->key);
							roq_rx_close_on_error(PerQP, NET_EVENT_MEM_BASE_BOUNDS_ERROR);

							goto get_next_packet;
						}
					}
					if (BytesToPlaceInThisSGE > sge->length - PerQP->mCurrent_RQE_SGE_Offset)
						BytesToPlaceInThisSGE = sge->length - PerQP->mCurrent_RQE_SGE_Offset;

					/* place bytes making sure not memcpy across page boundaries */
					uint64_t OffsetAddr = sge->addr + PerQP->mCurrent_RQE_SGE_Offset;
					uint32_t BytesFreeOnCurrentPage = RoQ_BURST_SIZE - (OffsetAddr & RoQ_BURST_MASK);

					if (BytesFreeOnCurrentPage < BytesToPlaceInThisSGE)
						BytesToPlaceInThisSGE = BytesFreeOnCurrentPage;
					/*
					 * NEED: ? check the sge.key for perms or validity?
					 * NEED: will this burst overrun this SGE and cause us to need a transition?
					 */
					RoQ_DmaAddr = RoQ_HostVa2DmaAddr(sge->key, OffsetAddr, BytesToPlaceInThisSGE, __LINE__);
					RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

					if (RoQ_VirtAddr)
						memcpy((void *)RoQ_VirtAddr,
						       (void *)((uint64_t)payload_ptr + PerQP->mCurrent_RQE_TotalReceivedBytes),
						       BytesToPlaceInThisSGE);

					PerQP->mCurrent_RQE_SGE_Offset         += BytesToPlaceInThisSGE;
					PerQP->mCurrent_RQE_TotalReceivedBytes += BytesToPlaceInThisSGE;

					payload_len -= BytesToPlaceInThisSGE;
					DASSERT(payload_len >= 0, "msg len %u, remaining bytes = %u",
						PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
						BytesToPlaceInThisSGE);

					RoQ_Put_MR_Write(sge->key);
				}

				DASSERT( PacketType & PktT_LastON, "Not LastON where it should be\n");
 	                        DASSERT( PktBurstType & PktT_LastBurstOfMsgON, "Not LastBurstOfMsgON where it should be\n");
				DASSERT( PerQP->mCurrent_RQE_TotalReceivedBytes == PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength, "last burts of message but all bytes not here\n");

				// this needs to be fixed in the right place.
				if (PerQP->mQP_Num == 0) {
					//printf("Setting PerQP->mQP_Num fro 0 to %d. Probably a UD QP for EthEm not quite setup right\n", PktDestQPId );
					PerQP->mQP_Num = PktDestQPId;
				}

				DPRINTF(FXLOG_UCODE_PER_MESSAGE,
					"rqe@ 0x%p rqe->id %lx ROQ_OP_SEND FINISHED Placed %lx bytes qp@ 0x%p qp->mRecv_DmaPtr 0x%p\n",
					(void *)rqe, (uint64_t)rqe->id, (uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes,
					PerQP, (uint64_t) PerQP->mQP_Num, (void *)qp, (void *)qp->mRecvCQ_DmaPtr);

				struct roq_cqe* cqe = RoQ_Reserve_CQE(roq_cq, qp, rqe->id, ROQ_OP_RECEIVE, ROQ_WC_SUCCESS,
								      PerQP->mCurrent_RQE_TotalReceivedBytes, 0, 1);
				if (cqe) {
					tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE,
							       ROQ_BURST_COMPLETE_RECV, 0, (uint64_t) cqe, 1);
					rqe->flags = ROQ_WQE_COMPLETED;
				}
				DPRINTF(FXLOG_UCODE_PER_MESSAGE,"\n");
				PerQP->mCurrent_Setup_Type = -1;
				TranConn->mCurrentRecvBurst_QP_Ptr = NULL;
    				
			} else {
#else
			{
#endif
				PerQP->mCurrent_Setup_Type = ROQ_OP_SEND;
				TranConn->mCurrentRecvBurst_QP_Ptr = qp;
				tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 0);
                        }
			break;

		/*
		 * case RoQ_Packet_t::Hdr_t::RDMA_SEND_BODY:
		 * NEED to deliver data into multiple scatter gather buffers,
		 * we'll need a non-setup PktT_First bit and case
		 */
		case (PktT_SetupOFF | PktT_FirstOFF | PktT_LastOFF | PktT_IB | ROQ_OP_SEND):
		case (PktT_SetupOFF | PktT_FirstOFF | PktT_LastON  | PktT_IB | ROQ_OP_SEND):

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"*** NEW NON FIRST PKT USE OF SEND *** qpn %lx\n", (uint64_t)PktDestQPId);

		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastOFF | PktT_IB | ROQ_OP_SEND):
		case (PktT_SetupOFF | PktT_FirstON  | PktT_LastON  | PktT_IB | ROQ_OP_SEND):

			PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];
			/*
			 * this is the first packet of a stat (not setup) burst --
			 * need to figure out the physical addr and move data
			 * RDMA SEND OP must already have been set up on this QP
			 * note that the tranconn could have been handling a burst
			 * for another qp before -- the Setup MIGHT NOT HAVE BEEN
			 * a reason to do this on the first packet of a burst rather
			 * than in setup is so we only need one setup per op
			 */
			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"case (PktT_SetupOFF  | PktT_FirstON | PktT_LastON/OFF | PktT_IB | ROQ_OP_SEND) : qpn %lx\n",
				(uint64_t)PktDestQPId);

			qp = RoQ_GetQPPtr_Idx(PktDestQPId);

			DASSERT(qp, "%p", qp);

			/*
			 * this is a first packet of a burst... tranconn will in this burst on until it ends.
			 * NEED: check the state -- make sure QP is idle, etc
			 * NOTE: if we separate cases for Pkt_LastON/OFF, this doesn't need to be done for off.
			 */
			TranConn->mCurrentRecvBurst_QP_Ptr = qp;

			/*
			 * NEED: to record the qp version in perhaps a private rx_thread qp state struct
			 *       version number of qp recorded in the tranconn at op setup and checked along the way
			 */
			DSTRONG_ASSERT(TranConn->mCurrentRecvBurst_QP_VersionNumber == qp->mVersionNumber,
					"QP has changed between setup and use qpn %lx\n", (uint64_t)PktDestQPId);
#if 0 // actuall this seems to be picked up above where all 'body' type bursts are discarded if TranConn isn't setup
                        // If not setup for a SEND, discard the burst
			if( PerQP->mCurrent_Setup_Type != ROQ_OP_SEND ) {
				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1 );
				printf("Protocal WARNING -- DISCARDING SEND BODY : "
					"PktSource %lx qpn %lx qp@ 0x%p PerQP@ 0x%p ->mCurrent_RQE_Index %lx ->mCurrent_RQE_SGE_List_Index %lx "
					"->mCurrent_RQE_SGE_Offset %lx PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx "
					"PerQP->mCurrent_RQE_TotalReceivedBytes %lx PerQP->mCurrent_Setup_Type %lx TranConn@ 0x%p\n",
					(uint64_t)PktSource, (uint64_t)PktDestQPId, (void *)qp, (void *)PerQP,
					(uint64_t)PerQP->mCurrent_RQE_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_List_Index,
					(uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
					(uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes, (uint64_t)PerQP->mCurrent_Setup_Type,
					(void *)TranConn);
				break;
                        }
#else
			/* Better be in the middle of receiving a send/recv message if we get this type of packet */
			DASSERT(PerQP->mCurrent_Setup_Type == ROQ_OP_SEND,
				"PktSource %lx qpn %lx qp@ 0x%p PerQP@ 0x%p ->mCurrent_RQE_Index %lx ->mCurrent_RQE_SGE_List_Index %lx "
				"->mCurrent_RQE_SGE_Offset %lx PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx "
				"PerQP->mCurrent_RQE_TotalReceivedBytes %lx PerQP->mCurrent_Setup_Type %lx TranConn@ 0x%p\n",
				(uint64_t)PktSource, (uint64_t)PktDestQPId, (void *)qp, (void *)PerQP,
				(uint64_t)PerQP->mCurrent_RQE_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_List_Index,
				(uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
				(uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes, (uint64_t)PerQP->mCurrent_Setup_Type,
				(void *)TranConn);
#endif

			DPRINTF(FXLOG_UCODE_PER_PACKET,
				"PktSource %lx qpn %lx qp@ 0x%p PerQP@ 0x%p ->mCurrent_RQE_Index %lx ->mCurrent_RQE_SGE_List_Index %lx "
				"->mCurrent_RQE_SGE_Offset %lx PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx "
				"PerQP->mCurrent_RQE_TotalReceivedBytes %lx PerQP->mCurrent_Setup_Type %lx TranConn@ 0x%p\n",
				(uint64_t)PktSource, (uint64_t)PktDestQPId, (void *)qp, (void *)PerQP,
				(uint64_t)PerQP->mCurrent_RQE_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_List_Index,
				(uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
				(uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes, (uint64_t)PerQP->mCurrent_Setup_Type,
				(void *)TranConn);

			uint32_t BytesLeftToPlace = (uint32_t)(PktBytesInPayload);

			DSTRONG_ASSERT(BytesLeftToPlace <= (PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength - PerQP->mCurrent_RQE_TotalReceivedBytes),
					"PktSource %lx qpn %lx qp@ 0x%p BytesLeftToPlace %lx"
					"PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx PerQP->mCurrent_RQE_TotalReceivedBytes %lx\n",
					(uint64_t)PktSource, (uint64_t)PktDestQPId, (void *)qp, (uint64_t)BytesLeftToPlace,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
					(uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes);

			rqe = &PerQP->mCurrent_RQE_ValidatedCopy;

			DASSERT(rqe, "%p", rqe);

			while (BytesLeftToPlace) {
				struct roq_sge *sge = &PerQP->Current_RecvSgList[PerQP->mCurrent_RQE_SGE_List_Index];

				if (RoQ_Get_MR_Write(sge->key) <= 0) {
					roq_post_net_event(NET_EVENT_MEM_ACCESS_ERROR, &PktDestQPId, 1);
					PerQP->mCurrent_RQE_TotalReceivedBytes += BytesLeftToPlace;

					break;
				}

				DPRINTF(FXLOG_UCODE_PER_PACKET,
					"BytesLeftToPlace %lx rqe@ 0x%p PerQP->mCurrent_RQE_Index %lx ->num_sge %lx "
					"sge@ 0x%p ->addr 0x%p ->length %lx PerQP->mCurrent_RQE_SGE_List_Index %lx PerQP->mCurrent_RQE_SGE_Offset %lx\n",
					(uint64_t)BytesLeftToPlace, (void *)rqe, (uint64_t)PerQP->mCurrent_RQE_Index,
					(uint64_t)rqe->num_sge, (void *)sge, (void *)sge->addr, (uint64_t)sge->length,
					(uint64_t)PerQP->mCurrent_RQE_SGE_List_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_Offset);

				DASSERT(PerQP->mCurrent_RQE_SGE_Offset <= sge->length,
					"over ran sge area  offest %lx sge->length %lx\n",
					(uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)sge->length);
				/*
				 * advance the iterator info going through the SGE if required
				 * note -- let unadvanced at the bottom of the loop or with a zero
				 * len sg element that isn't the end of the list
				 */
				if (PerQP->mCurrent_RQE_SGE_Offset == sge->length) {
					/*
					 * cost of this branch could be avoided using
					 * packet type bit for cases when known not possible
					 */
					PerQP->mCurrent_RQE_SGE_List_Index++;
					PerQP->mCurrent_RQE_SGE_Offset = 0;

					if (PerQP->mCurrent_RQE_SGE_List_Index == rqe->num_sge) {
						roq_post_net_event(NET_EVENT_MEM_BASE_BOUNDS_ERROR, &PktDestQPId, 1);
						DPRINTF(1, "RX error: overran receive SG list %x|%x %d==%d\n",
								PerQP->mCurrent_RQE_SGE_List_Index,
								rqe->num_sge, PerQP->mCurrent_RQE_SGE_Offset,
								sge->length);
						/*
						 * NEED: this is a problem since the send just overran
						 *       the sg_list data areas.
						 */
					}
					RoQ_Put_MR_Write(sge->key);
					continue;
				}

				/* copy as many bytes as possible into place */
				uint32_t BytesToPlaceInThisSGE = BytesLeftToPlace;
				if (BytesToPlaceInThisSGE > sge->length - PerQP->mCurrent_RQE_SGE_Offset)
					BytesToPlaceInThisSGE = sge->length - PerQP->mCurrent_RQE_SGE_Offset;

				/* place bytes making sure not memcpy across page boundaries */
				uint64_t OffsetAddr = sge->addr + PerQP->mCurrent_RQE_SGE_Offset;
				uint32_t BytesFreeOnCurrentPage = RoQ_BURST_SIZE - (OffsetAddr & RoQ_BURST_MASK);

				if (BytesFreeOnCurrentPage < BytesToPlaceInThisSGE)
					BytesToPlaceInThisSGE = BytesFreeOnCurrentPage;
				/*
				 * NEED: ? check the sge.key for perms or validity?
				 * NEED: will this burst overrun this SGE and cause us to need a transition?
				 */
				RoQ_DmaAddr = RoQ_HostVa2DmaAddr(sge->key, OffsetAddr, BytesToPlaceInThisSGE, __LINE__);
				RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

				/* NEED: think about moving chunks not bytes */
				if (RoQ_VirtAddr)
					memcpy((void *)RoQ_VirtAddr,
					      (void *)((uint64_t)PktPayloadPtr + (PktBytesInPayload - BytesLeftToPlace)),
					      BytesToPlaceInThisSGE);

				PerQP->mCurrent_RQE_SGE_Offset         += BytesToPlaceInThisSGE;
				PerQP->mCurrent_RQE_TotalReceivedBytes += BytesToPlaceInThisSGE;

				BytesLeftToPlace -= BytesToPlaceInThisSGE;

				DPRINTF(0,
					"qpn %lx Payload Placed PktBytesInPayload %lx mCurrent_RQE_TotalReceivedBytes %lx mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx "
					"mCurrent_RQE_Index %lx mCurrent_RQE_SGE_List_Index %lx mCurrent_RQE_SGE_Offset %lx BytesLeftToPlace %lx BytesToPlaceInThisSGE %lx "
					"OffsetAddr %lx VirtAddr %lx\n",
					(uint64_t)PktDestQPId, (uint64_t)PktBytesInPayload, (uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes,
					(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength, (uint64_t)PerQP->mCurrent_RQE_Index,
					(uint64_t)PerQP->mCurrent_RQE_SGE_List_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_Offset, (uint64_t)BytesLeftToPlace,
					(uint64_t)BytesToPlaceInThisSGE, (uint64_t)OffsetAddr, (uint64_t)RoQ_VirtAddr);

				RoQ_Put_MR_Write(sge->key);
			}

			DPRINTF((PacketType & PktT_LastON) && PerQP->mCurrent_RQE_TotalReceivedBytes != PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
				"qpn %lx Payload Placed PktBytesInPayload %lx mCurrent_RQE_TotalReceivedBytes %lx mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength %lx "
				"mCurrent_RQE_Index %lx mCurrent_RQE_SGE_List_Index %lx mCurrent_RQE_SGE_Offset %lx\n",
				(uint64_t)PktDestQPId, (uint64_t)PktBytesInPayload, (uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes,
				(uint64_t)PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength, (uint64_t)PerQP->mCurrent_RQE_Index,
				(uint64_t)PerQP->mCurrent_RQE_SGE_List_Index, (uint64_t)PerQP->mCurrent_RQE_SGE_Offset);

			if( PacketType & PktT_LastON ) {
                                if( PktBurstType & PktT_LastBurstOfMsgON ) {
					DASSERT(PerQP->mCurrent_RQE_TotalReceivedBytes == PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
						"last burts of message but all bytes not here\n");

					// this needs to be fixed in the right place.
					if (PerQP->mQP_Num == 0) {
						  //printf("Setting PerQP->mQP_Num fro 0 to %d. Probably a UD QP for EthEm not quite setup right\n", PktDestQPId );
						PerQP->mQP_Num = PktDestQPId;
					}

					DPRINTF(FXLOG_UCODE_PER_MESSAGE,
						"rqe@ 0x%p rqe->id %lx ROQ_OP_SEND FINISHED Placed %lx bytes qp@ 0x%p qp->mRecv_DmaPtr 0x%p\n",
						(void *)rqe, (uint64_t)rqe->id, (uint64_t)PerQP->mCurrent_RQE_TotalReceivedBytes,
						PerQP, (uint64_t) PerQP->mQP_Num, (void *)qp, (void *)qp->mRecvCQ_DmaPtr);

					/* msg done */
					struct roq_dm_cq *roq_cq = (struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)qp->mRecvCQ_DmaPtr);
					struct roq_cqe   *cqe = RoQ_Reserve_CQE(roq_cq, qp, rqe->id, ROQ_OP_RECEIVE, ROQ_WC_SUCCESS,
         									PerQP->mCurrent_RQE_TotalReceivedBytes, 0, 1);
					if (cqe) {
        	 				tc_PostBurstCompletion(TranConn, PerQP, ROQ_BURST_COMPLETE,
								       ROQ_BURST_COMPLETE_RECV, 0, (uint64_t) cqe, 1);
						rqe->flags = ROQ_WQE_COMPLETED;
					}
					PerQP->mCurrent_Setup_Type = -1;

				} else {
                                //DASSERT(0,"RX a SEND/RECV -- Burst is done but receive data isn't done. Not supported.\n");
					/*not done*/
					tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 0 );
				}
				TranConn->mCurrentRecvBurst_QP_Ptr = NULL;
//				PerQP->mCurrent_Setup_Type = -1;
			}
			break;

	      case (PktT_SetupON  | PktT_FirstON | PktT_LastON  | PktT_IB | ROQ_OP_READ):
			/*****************
		 	 *** RDMA READ ***
			 ******************/
			PerQP = &RoQ_DevicePrivatePerQPState[PktDestQPId];

			DPRINTF(FXLOG_UCODE_PER_MESSAGE,
				"RX RDMA_READ case (PktT_SetupON  | PktT_FirstON | PktT_LastON | PktT_IB | ROQ_OP_READ) : perqp 0x\n", PerQP);

			/*
			 * NEED: check the state -- make sure QP is idle, etc
			 *
			 * this is a hack to set us to the max allowed... this needs
			 * to be handled better starting with what the user askes for
			 */
			PerQP->mInboundReadQueue_Count = RoQ_MAX_OUTSTANDING_RDMA_READS;

			DSTRONG_ASSERT((PerQP->mInboundReadQueue_Posted - PerQP->mInboundReadQueue_Retired) < PerQP->mInboundReadQueue_Count,
					"%lx %lx %lx\n", (uint64_t)PerQP->mInboundReadQueue_Posted,
					(uint64_t)PerQP->mInboundReadQueue_Retired,
					(uint64_t)PerQP->mInboundReadQueue_Count);

			int InboundReadIndex = PerQP->mInboundReadQueue_Posted % PerQP->mInboundReadQueue_Count;

			PerQP->mInboundReadQueue_Array[InboundReadIndex] = *((struct RoQ_DDP_Fields *)PktPayloadPtr);

			/*
			 * push out the body of the inbound read before
			 * updating the posted count below.
			 */
			_bgp_msync();

			PerQP->mInboundReadQueue_Posted++;
			////ppc_msync(); //NEEDED?

                        DASSERT( PktBurstType & PktT_LastBurstOfMsgON, "PktT_LastBurstOfMsgON was not on where it should have been\n");

                        /*
                         * If there are no async bursts in progress (BGQ MU DMA Gets) for this QP, signal the TX thread right away.
                         * This is only a slight savings in code path but could be a significant savings where there would
                         * otherwise be head-of-line blocking at the TranConn. This is because burst management doesn't interact with QP
                         * based fencing... if a RDMA READ startes to wait on outstanding MU GETS, it waits for ALL MU prior gets from all QPs to a given peer.
                         */
                        if (1 && PerQP->mAsyncBurstsInProgress == 0) {

                                PerQP->mInboundReadQueue_Ready++;
                                ppc_msync(); // ensure the Ready++ gets out to the TX thread.

                                /* NEED: switch this to use PerQP */
                                int RxThreadNumber = 0; /* this is used to select a doorbell queue */

                                RoQ_MC_RingTxDoorbell(PerQP->mQP_Num, RxThreadNumber);

                                DPRINTF(FXLOG_UCODE_PER_MESSAGE, "RX RDMA_READ TX Doorbell rung! qp num %lx\n", (uint64_t)PktDestQPId);


				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_NO_ACTION, 0, 0, 1 );
                        } else {
				tc_PostBurstCompletion( TranConn, PerQP, ROQ_BURST_COMPLETE, ROQ_BURST_COMPLETE_READ_REQUEST, 0, 0, 1 );
                        }


			TranConn->mCurrentRecvBurst_QP_Ptr = NULL;

			break;

		default:

			DPRINTF(1,
				"PacketType %lx PktT_Setup %lx PktT_First %lx PktT_Last %lx PktT_IB %lx IB WR OP CODE %lx "
				"(ROQ_OP_WRITE %lx ROQ_OP_SEND %lx ROQ_OP_READ %lx)\n",
				(uint64_t)PacketType, (uint64_t)(PacketType & PktT_SetupON ? 1 : 0),
				(uint64_t)(PacketType & PktT_FirstON ? 1 : 0), (uint64_t)(PacketType & PktT_LastON ? 1 : 0),
				(uint64_t)(PacketType & PktT_IB ? 1 : 0), (uint64_t)(PacketType & 0xF),
				(uint64_t)ROQ_OP_WRITE, (uint64_t)ROQ_OP_SEND, "ROQ_OP_READ", (uint64_t)ROQ_OP_READ);

			DASSERT(0, "Packet received has unrecognized type 0x%lx\n", PacketType);

		} /* End of switch (PacketType) */

		if (PacketType & PktT_LastON) {
#if defined(PKTRACE_ON)
			HitOE2(BurstRecvFinis,"BurstRecv_PeerRank=%04d_QPId=%04d", 0, (int)TranConn->mPeerRank, PktDestQPId);
#endif
			DASSERT(TranConn->mCurrentRecvBurst_QP_Ptr == NULL
				|| PerQP->mCurrent_RQE_TotalReceivedBytes != PerQP->mCurrent_RDMA_OP_DDP_Fields.Send.MessageLength,
				"QPID %d, PacketType %lx PktT_Setup %lx PktT_First %lx PktT_Last %lx PktT_IB %lx "
				"IB WR OP CODE %lx (ROQ_OP_WRITE %lx ROQ_OP_SEND %lx ROQ_OP_READ %lx)\n",
				PktDestQPId, PacketType, (uint64_t)(PacketType & PktT_SetupON ? 1 : 0), (uint64_t)(PacketType & PktT_FirstON ? 1 : 0),
				(uint64_t)(PacketType & PktT_LastON ? 1 : 0), (uint64_t)(PacketType & PktT_IB ? 1 : 0),
				(uint64_t)(PacketType & 0xF), (uint64_t)ROQ_OP_WRITE, (uint64_t)ROQ_OP_SEND, (uint64_t)ROQ_OP_READ);
		}
	}
	return NULL;
}
