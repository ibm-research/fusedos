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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bgq_hw_abstraction.h>

#ifdef USERSPACE_ROQ
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
#include <roq_device_interface.h>
#endif /* USERSPACE_ROQ */

#include <transcon.h>
#include <roq_debug.h>
#include <roq_fw.h>


void
tc_init(struct RoQ_TransportConnection *tc,
     int aRank, uint32_t aPeerRank,
     struct RoQ_TX_Arguments *RoQ_TX_Args,
     struct RoQ_RX_Arguments *RoQ_RX_Args,
     struct RoQ_NetworkDescriptor *RoQ_NetworkDesc)
{
	uint32_t HereIpAddr;
	uint32_t PeerIpAddr;
	uint32_t pa, pb, pc, pd, pe;

	bzero(tc, sizeof(struct RoQ_TransportConnection));

	RoQ_Rank_To_IPV4Addr(RoQ_NetworkDesc, aRank, &HereIpAddr);
	RoQ_Rank_To_IPV4Addr(RoQ_NetworkDesc, aPeerRank, &PeerIpAddr);
	RoQ_Rank_To_TorusCoords(RoQ_NetworkDesc, aPeerRank, &pa,
				  &pb, &pc, &pd, &pe);

	tc->subgroupId = firstLocalSubGrp;
	tc->mRxPortId = RoQ_RX_Args->mPortId;
	tc->mTxPortId = RoQ_TX_Args->mPortId;

	if (roq_us_map->trans_idx >= MAX_NODE_COUNT) {
		DPRINTF(1, "ERROR MAX NODE COUNT REACHED!!!!");
		exit(-1);
	}

	tc->mTX_BurstDescriptorArray    = (struct RoQ_TX_BurstDescriptor *)	roq_us_map->tx_burstdesc_dma[roq_us_map->trans_idx];
	tc->mTX_BGQ_MU_DescriptorArray  = (MUHWI_Descriptor_Aligned_t *)		roq_us_map->mu_burstdesc_dma[roq_us_map->trans_idx];
	tc->mTX_BGQ_MU_AckDescriptor    = (MUHWI_Descriptor_Aligned_t *)		roq_us_map->mu_ackdesc_dma[roq_us_map->trans_idx];
	tc->mRX_BurstAckDescriptorArray = (struct RoQ_RX_BurstAckDescriptor *)	roq_us_map->rx_burstdesc_dma[roq_us_map->trans_idx];
	roq_us_map->trans_idx++;

	tc->use_iolink = 0;
	tc->ackslot = 0;
	tc->mBurstSeqNo_ReadyToAck = -1; // this needs to be incremented to show ready to ack on 0 burst
	tc->mBurstSeqNo_LastAcked  = -1; // this needs to be incremented to show ready to ack on 0 burst
	// Initialize the BG/Q MU resend descriptor array
	// to the values that will be constant for the
	// life of this transport connection path.
	bzero(tc->mTX_BurstDescriptorArray, sizeof(struct RoQ_TX_BurstDescriptor) * RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
	bzero(tc->mTX_BGQ_MU_DescriptorArray, sizeof(MUHWI_Descriptor_Aligned_t) * RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
	bzero(tc->mTX_BGQ_MU_AckDescriptor, sizeof(MUHWI_Descriptor_Aligned_t) * RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT);

	MUSPI_SetUpDestination(&tc->PeerNodeAddr, pa, pb, pc, pd, pe);

	int a, i;
	for (a = 0; a < 2; a++) {
		MUHWI_Descriptor_Aligned_t *blup = (a == 0) ? tc->mTX_BGQ_MU_DescriptorArray : tc->mTX_BGQ_MU_AckDescriptor;
		int cnt = (a == 0) ? RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT : RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT;
		for (i = 0; i < cnt; i++) {
			MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &blup[i];

			// need to set up TCAddr base on physical addressing scheme.
			// form the bgq headers for both the setup and the body (if there is a body)
			MUSPI_Pt2PtMemoryFIFODescriptorInfo_t Info;
			memset(&Info, 0x00, sizeof(Info));

			// much of this would stay the same in use after use -- figure out how to exploit that
			Info.Pt2Pt.Hints_ABCD = 0;	// MUHWI_PACKET_HINT_AP;
			Info.Pt2Pt.Misc1 = MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE |
			                   MUHWI_PACKET_USE_DETERMINISTIC_ROUTING |
			                   MUHWI_PACKET_DO_NOT_DEPOSIT;
			Info.Pt2Pt.Misc2 = MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
			Info.Pt2Pt.Skip = 0;
			Info.MemFIFO.Rec_FIFO_Id = firstLocalRecFifo;	// NOTE: FIFO ID MIGHT BE SET FOR MULTIPLE THREADS, ETC //aTCAddr.mFifoId;
			Info.MemFIFO.Rec_Put_Offset = -1;	//this will be set on each segemnt
			Info.Base.Pre_Fetch_Only = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
			Info.MemFIFO.Interrupt = MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
//			Info.MemFIFO.Interrupt      = MUHWI_PACKET_INTERRUPT_ON_PACKET_ARRIVAL;
			Info.MemFIFO.SoftwareBit = 0;
			// Haven't filled out the software bytes yet
			//memcpy( Info.MemFIFO.SoftwareBytes, SoftwareBytes.bytes, sizeof( Info.MemFIFO.SoftwareBytes ) );

			//Warning assume V == P
			Info.Base.Payload_Address = (uint64_t) NULL;

			Info.Base.Message_Length = 0;
			uint64_t torusInjectionFifoMap = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM;	// bit 0 ===> A- Torus FIFO
			Info.Base.Torus_FIFO_Map = torusInjectionFifoMap;

			// temp out for udp prototype XXX:
			MUSPI_CreatePt2PtMemoryFIFODescriptor(MUHWI_DescPtr, &Info);

			MUHWI_Destination_t dest;
			MUSPI_SetUpDestination(&dest, pa, pb, pc, pd, pe);
			MUSPI_SetDestination(MUHWI_DescPtr, dest);

			// get to the SwHdr bytes in the packet header and set up the fields that don't change
			MUHWI_PacketHeader_t *PktHdr = &(MUHWI_DescPtr->PacketHeader);

			/* Set pointer to message unit header within the packet header. */
			MUHWI_MessageUnitHeader_t *qnet_hdr_mu = &(PktHdr->messageUnitHeader);

			// get access to the 14 software protocol bytes in the hw header
			union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr *) & qnet_hdr_mu->Packet_Types.Memory_FIFO.Unused2;

			MU_SwHdr->mSource = aRank;
			MU_SwHdr->mDestination = aPeerRank;
			// temp out for udp prototype MUSPI_DescriptorDumpHex ( "SetupDesc", MUHWI_DescPtr );
		}
	}
}

void
tc_init_remote(struct RoQ_TransportConnection *tc,
		int aRank, uint32_t aPeerRank,
		struct RoQ_TX_Arguments *RoQ_TX_Args,
		struct RoQ_RX_Arguments *RoQ_RX_Args,
		struct RoQ_NetworkDescriptor *RoQ_NetworkDesc,
		Personality_t *personality)
{
	MUSPI_Pt2PtMemoryFIFODescriptorInfo_t info;
	int cnt, a, i;
	uint32_t io_port;
	uint64_t io_map;
	uint32_t pa, pb, pc, pd, pe;

	memset(&info, 0, sizeof info);
	memset(tc, 0, sizeof *tc);

	/* discover io port */
	io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;
	io_map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;

	if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) {
		if (RoQ_NetworkDesc->IOport == 6) {
			io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6;
			io_map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM;
		}
		if (RoQ_NetworkDesc->IOport == 7) {
			io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7;
			io_map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP;
		}
		if (RoQ_NetworkDesc->IOport == 10) {
			io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;
			io_map  = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;
		}
	}

//	// 20121017 tjcw test was complaining that io_map was empty. Try a known value
//	io_map |= MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM | MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP ;

	RoQ_Rank_To_TorusCoords(RoQ_NetworkDesc, aPeerRank, &pa,
				  &pb, &pc, &pd, &pe);

  if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) {
    MUSPI_SetUpDestination(&tc->PeerNodeAddr, pa, pb, pc, pd, pe);
  } else {
    MUSPI_SetUpDestination(&tc->PeerNodeAddr, personality->Network_Config.cnBridge_A,
                    personality->Network_Config.cnBridge_B,
                    personality->Network_Config.cnBridge_C,
                    personality->Network_Config.cnBridge_D,
                    personality->Network_Config.cnBridge_E);
  }
  tc->subgroupId = firstRemoteSubGrp;
	tc->mRxPortId = RoQ_RX_Args->mPortId; // 4 * sgid + fifoid
	tc->mTxPortId = RoQ_TX_Args->mPortId;

  DPRINTF(FXLOG_UCODE_INIT,"tc_init_remote io_map=%016lx mRxPortId=%d mTxPortId=%d\n",io_map,tc->mRxPortId,tc->mTxPortId) ;

	if (roq_us_map->rem.trans_idx >= MAX_NODE_COUNT) {
		DPRINTF(FXLOG_UCODE_INIT, "ERROR MAX NODE COUNT REACHED!!!!");
		exit(-1);
	}

	tc->mTX_BurstDescriptorArray   = (struct RoQ_TX_BurstDescriptor *)roq_us_map->rem.tx_burstdesc_dma[roq_us_map->rem.trans_idx];
	tc->mTX_BGQ_MU_DescriptorArray = (MUHWI_Descriptor_Aligned_t *)roq_us_map->rem.mu_burstdesc_dma[roq_us_map->rem.trans_idx];
	tc->mTX_BGQ_MU_AckDescriptor   = (MUHWI_Descriptor_Aligned_t *)roq_us_map->rem.mu_ackdesc_dma[roq_us_map->rem.trans_idx];
	// tjcw 20121026 added next line
  tc->mRX_BurstAckDescriptorArray = (struct RoQ_RX_BurstAckDescriptor *)  roq_us_map->rem.rx_burstdesc_dma[roq_us_map->rem.trans_idx];
	roq_us_map->rem.trans_idx++;

	tc->use_iolink = 1;
	tc->ackslot = 0;
	// Initialize the BG/Q MU resend descriptor array
	// to the values that will be constant for the
	// life of this transport connection path.
	bzero(tc->mTX_BurstDescriptorArray, sizeof(struct RoQ_TX_BurstDescriptor) * RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
	bzero(tc->mTX_BGQ_MU_DescriptorArray, sizeof(MUHWI_Descriptor_Aligned_t) * RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
	bzero(tc->mTX_BGQ_MU_AckDescriptor, sizeof(MUHWI_Descriptor_Aligned_t) * RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT);

	for (a = 0; a < 2; a++) {
		MUHWI_Descriptor_Aligned_t *blup = (a == 0) ? tc->mTX_BGQ_MU_DescriptorArray : tc->mTX_BGQ_MU_AckDescriptor;
		cnt = (a == 0) ? RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT : RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT;
		for (i = 0; i < cnt; i++) {
			MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &blup[i];

			if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode)
				info.Pt2Pt.Misc1        = MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE | MUHWI_PACKET_USE_DETERMINISTIC_ROUTING | io_port;
			else
				info.Pt2Pt.Misc1        = MUHWI_PACKET_USE_DETERMINISTIC_ROUTING | MUHWI_PACKET_ROUTE_TO_IO_NODE;
			info.Pt2Pt.Misc2            = MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM;
			info.Pt2Pt.Skip             = 0;
			info.Pt2Pt.Hints_ABCD       = 0;
			info.MemFIFO.Rec_FIFO_Id    = firstLocalRecFifo + 1;
			info.MemFIFO.Rec_Put_Offset = -1;
			info.Base.Pre_Fetch_Only    = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
			info.MemFIFO.Interrupt      = MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
			info.MemFIFO.SoftwareBit    = 0;
			info.Base.Payload_Address   = (uint64_t)NULL;
			info.Base.Message_Length    = 0;
			info.Base.Torus_FIFO_Map    = io_map;

			MUSPI_CreatePt2PtMemoryFIFODescriptor(MUHWI_DescPtr, &info);

			MUHWI_Destination_t dest;
			if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) {
				MUSPI_SetUpDestination(&dest, pa, pb, pc, pd, pe);
			} else {
				MUSPI_SetUpDestination(&dest, personality->Network_Config.cnBridge_A,
											  personality->Network_Config.cnBridge_B,
											  personality->Network_Config.cnBridge_C,
											  personality->Network_Config.cnBridge_D,
											  personality->Network_Config.cnBridge_E);
			}
			MUSPI_SetDestination(MUHWI_DescPtr, dest);

			// get to the SwHdr bytes in the packet header and set up the fields that don't change
			MUHWI_PacketHeader_t *PktHdr = &(MUHWI_DescPtr->PacketHeader);

			/* Set pointer to message unit header within the packet header. */
			MUHWI_MessageUnitHeader_t *qnet_hdr_mu = &(PktHdr->messageUnitHeader);

			// get access to the 14 software protocol bytes in the hw header
			union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr *) & qnet_hdr_mu->Packet_Types.Memory_FIFO.Unused2;

			MU_SwHdr->mSource = aRank;
			MU_SwHdr->mDestination = aPeerRank;
			MU_SwHdr->mIOLinkBoundPacketFlag = 1;
		}
	}
}

/* change this to 0xFFFFFFFE to nack */
void
tc_TX_Ack_Blunt(struct RoQ_TransportConnection *tc, uint32_t aMagicAckValue, uint64_t aAckSeqNo)
{
        //printf("tc_TX_Ack_Blunt(): tc@ %p aAckSeqNo %lx\n", tc, aAckSeqNo );
	//if injFIFO avail
	// put the finishing touches on a BG/Q MU descriptor and inject it to send a burst
	MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &tc->mTX_BGQ_MU_AckDescriptor[tc->ackslot];

	MUHWI_Descriptor_Aligned_t *desc   = MUHWI_DescPtr;
	MUHWI_PacketHeader_t       *PktHdr = &(desc->PacketHeader);

	/* Set pointer to packet header portion of the descriptor. */
#if (FXLOG_ENABLE == 1)
	MUHWI_Pt2PtNetworkHeader_t *qnet_hdr_p2p = &(PktHdr->NetworkHeader.pt2pt);
#endif

	/* Set pointer to message unit header within the packet header. */
	MUHWI_MessageUnitHeader_t *qnet_hdr_mu = &(PktHdr->messageUnitHeader);

	// get access to the 14 software protocol bytes in the hw header
	union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr *) & qnet_hdr_mu->Packet_Types.Memory_FIFO.Unused2;

	MU_SwHdr->mAckedBurstAbsoluteSeqNo = aAckSeqNo;

	MUSPI_SetSize(MUHWI_DescPtr);

	uint64_t PutOffsetValue = aMagicAckValue;	// 0xFFFFFFFF;  // the magic ack value

#if 0
//NEED TO LOOK AT THIS: why is tc->ackslot being used to index something shared among all TranConns???
//What's the use of this buffer array? Are Zero length sends not allowed?
	roq_us_map->dmabuf[tc->ackslot] = aMagicAckValue;	// 0xFFFFFFFF;
	uint64_t payloadAddress = (uint64_t) &roq_us_map->dmabuf[tc->ackslot] & ~ROQ_VIRT_CACHE_OFF;
	uint64_t messageLength = sizeof(PutOffsetValue);
#else
	//roq_us_map->dmabuf[tc->ackslot] = aMagicAckValue;	// 0xFFFFFFFF;
	uint64_t payloadAddress = 0; ///////(uint64_t) &roq_us_map->dmabuf[0] & ~ROQ_VIRT_CACHE_OFF; // give a nice addr
	uint64_t messageLength = 0; ////sizeof(PutOffsetValue); // then say zero len
#endif

	MUSPI_SetPayload(MUHWI_DescPtr, payloadAddress, messageLength);

	// set message len in descriptor and payload chunk count in packet header
	MUSPI_SetRecPutOffset(MUHWI_DescPtr, PutOffsetValue | 0xA00000000);

#ifdef ROQ_RESILIENCY
	if (tc->mForwardFlags & 0x1) { // Forwarding via intermediate node
#ifdef ROQ_RESILIENCY_DEBUG
		union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr_t *) & MUHWI_DescPtr->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Unused2;

		uint32_t a,b,c,d,e;
		RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc, MU_SwHdr->mDestination, &a, &b, &c, &d, &e);

		DPRINTF(ROQ_RESILIENCY_DEBUG, "Sending ACK packet to (%d,%d,%d,%d,%d) via (%d,%d,%d,%d,%d), hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   a,b,c,d,e,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
#endif
 	    MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mForwardFifoId); /* send to RX FIFO */
	} else { // Operation in case of an error (resiliency)
#ifdef ROQ_RESILIENCY_DEBUG
		DPRINTF(ROQ_RESILIENCY_DEBUG, "Sending ACK packet to (%d,%d,%d,%d,%d) directly, hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
#endif
		MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mTxPortId+(4*tc->subgroupId)); /* send to TX FIFO */
	}
#else
//	printf("Sending to TX FIFO %d\n",tc->mTxPortId+(4*tc->subgroupId)) ;
	MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mTxPortId+(4*tc->subgroupId)); /* send to TX FIFO */
#endif

	//printf("SEND ACK Blunt - slot %d - len %d\n", tc->ackslot, messageLength);
	ROQ_Send(tc->use_iolink, tc->mRxPortId, MUHWI_DescPtr);
	tc->ackslot = (tc->ackslot + 1) & RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT_MASK;
}

void
tc_ReTX_Burst(struct RoQ_TransportConnection *tc, uint32_t aBurstSlot)
{
	MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &tc->mTX_BGQ_MU_DescriptorArray[aBurstSlot];

#ifdef ROQ_RESILIENCY
	if (tc->mForwardFlags & 0x1) { // Forwarding via intermediate node
#ifdef ROQ_RESILIENCY_DEBUG
		union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr_t *) & MUHWI_DescPtr->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Unused2;

		uint32_t a,b,c,d,e;
		RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc, MU_SwHdr->mDestination, &a, &b, &c, &d, &e);

		DPRINTF(FXLOG_UCODE_PER_PACKET, "Resending BURST packet to (%d,%d,%d,%d,%d) via (%d,%d,%d,%d,%d), hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   a,b,c,d,e,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
#endif
		MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mForwardFifoId); /* send to RX FIFO */
	} else {
#ifdef ROQ_RESILIENCY_DEBUG
		DPRINTF(ROQ_RESILIENCY_DEBUG, "Resending BURST packet to (%d,%d,%d,%d,%d) directly, hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
#endif
	}
#endif
	ROQ_Send(tc->use_iolink, tc->mTxPortId, MUHWI_DescPtr);
}

/*
static void dumphex(const char *name,void *p, int len)
  {
    uint64_t *lp=(uint64_t *)p ;
    int i ;
    printf("%s for %d bytes\n",name,len) ;
    for(i=0;i<4*((len+31)/32);i+=4)
      {
        printf("%016lx %016lx %016lx %016lx\n",lp[i],lp[i+1],lp[i+2],lp[i+3]) ;
      }
  }
*/
void
tc_TX_Burst(	struct RoQ_TransportConnection *tc,
		uint32_t 			aBurstSlot,
		RoQ_QP_Id_t			aDestConnId,
		unsigned			aBurstType,
		uint64_t 			aData,
		uint32_t 			aLen)
{

//  printf("tc_TX_Burst tc=%p aBurstSlot=%08x aData=%016lx aLen=%d\n",tc,aBurstSlot,aData,aLen) ;

  DASSERT(aBurstType < 1<<7, "BurstType %08X to large for field\n", aBurstType);
	DASSERT(aLen <= (64 * 1024), "Burst lenght to large %d\n", aLen);
	DASSERT(aDestConnId.mIndex > 0, "Dest QPN <= zero -- not allowed -- %d", aDestConnId.mIndex);

	// The RoQ uses BG/Q headers to keep track of burst only
	// making a BGP MU descriptor and header when sending a burst
	// put the finishing touches on a BG/Q MU descriptor and inject it to send a burst

	MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &tc->mTX_BGQ_MU_DescriptorArray[aBurstSlot];

	MUHWI_Descriptor_Aligned_t *desc = MUHWI_DescPtr;
	// here assume udp
	MUHWI_PacketHeader_t *PktHdr = &(desc->PacketHeader);

	/* Set pointer to packet header portion of the descriptor. */
#if (FXLOG_ENABLE == 1)
	MUHWI_Pt2PtNetworkHeader_t *qnet_hdr_p2p = &(PktHdr->NetworkHeader.pt2pt);
#endif

	/* Set pointer to message unit header within the packet header. */
	MUHWI_MessageUnitHeader_t *qnet_hdr_mu = &(PktHdr->messageUnitHeader);

	// get access to the 14 software protocol bytes in the hw header
	union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr *) & qnet_hdr_mu->Packet_Types.Memory_FIFO.Unused2;

	DASSERT(sizeof(union RoQ_BGQ_MU_FifoPkt_SwHdr) <= 14, "%ld", sizeof(union RoQ_BGQ_MU_FifoPkt_SwHdr));

	MU_SwHdr->mBurstType = aBurstType;
	MU_SwHdr->mSpareBits = 0; /* probably not needed and a wasted cycle */
	MU_SwHdr->mDestConnectionId = aDestConnId.mIndex;
	MU_SwHdr->mPacketsInBurst = aLen / 512;	// note -- this is actual packet count - 1.

	MU_SwHdr->mBytesInLastPacket = aLen % 512;
	if (MU_SwHdr->mBytesInLastPacket == 0) {	// Here we spend a header bit to avoid a branch on the receive side a full last chunk send the value 32
		MU_SwHdr->mPacketsInBurst -= 1;
		MU_SwHdr->mBytesInLastPacket = 512;
	}

	uint64_t messageLength = aLen;

	MUSPI_SetSize(MUHWI_DescPtr);

	// set message len in descriptor and payload chunk count in packet header
	uint64_t payloadAddress = (uint64_t) aData & ~ROQ_VIRT_CACHE_OFF;
	MUSPI_SetPayload(MUHWI_DescPtr, (uint64_t) payloadAddress, messageLength);

	union RoQ_BGQ_MU_HW_PutOffset BGQ_PutOffset;
	BGQ_PutOffset.mBurstSeqNo = tc->mBurstSeqNo_NextToSend;
	DASSERT(BGQ_PutOffset.mBurstSeqNo == tc->mBurstSeqNo_NextToSend || tc->mBurstSeqNo_NextToSend > 0x0000FFFF,
	       "%d %ld", BGQ_PutOffset.mBurstSeqNo, tc->mBurstSeqNo_NextToSend);
	tc->mBurstSeqNo_NextToSend++;	// NEED: to wrap this based on bits avail in header
	BGQ_PutOffset.mByteOffset = 0;
	MUSPI_SetRecPutOffset(MUHWI_DescPtr, BGQ_PutOffset.mWhole);

#ifdef ROQ_RESILIENCY
	if (tc->mForwardFlags & 0x1) { // Forwarding via intermediate node
#ifdef ROQ_RESILIENCY_DEBUG
		union RoQ_BGQ_MU_FifoPkt_SwHdr *MU_SwHdr = (union RoQ_BGQ_MU_FifoPkt_SwHdr_t *) & MUHWI_DescPtr->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Unused2;

		uint32_t a,b,c,d,e;
		RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc, MU_SwHdr->mDestination, &a, &b, &c, &d, &e);

		DPRINTF(ROQ_RESILIENCY_DEBUG, "Sending BURST packet to (%d,%d,%d,%d,%d) via (%d,%d,%d,%d,%d), hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   a,b,c,d,e,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
#endif
		MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mForwardFifoId); /* send to RX FIFO */
	} else { // Operation in case of an error (resiliency)
#ifdef ROQ_RESILIENCY_DEBUG
		DPRINTF(ROQ_RESILIENCY_DEBUG, "Sending BURST packet to (%d,%d,%d,%d,%d) directly, hints = %03x, offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
			   MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
			   (((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
				((MUHWI_DescPtr->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(& MUHWI_DescPtr->PacketHeader),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);

#endif
		MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mRxPortId + (4*tc->subgroupId)); /* send to RX FIFO */
	}
#else
	MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mRxPortId + (4*tc->subgroupId)); /* send to RX FIFO */
#endif
//  MUSPI_SetRecFIFOId(MUHWI_DescPtr, tc->mRxPortId) ; // tjcw temp: ignore the subgroup ID
//	printf("Sending to RX FIFO %d\n",tc->mRxPortId + (4*tc->subgroupId));
//	dumphex("injection descriptor",MUHWI_DescPtr,sizeof(*MUHWI_DescPtr)) ;
//
//	printf("Injecting into port %d use_iolink=%d mRxPortId=%d subgroupId=%d\n",tc->mTxPortId,tc->use_iolink,tc->mRxPortId,tc->subgroupId) ;

	ROQ_Send(tc->use_iolink, tc->mTxPortId, MUHWI_DescPtr);
	//printf("TX_Burst(): SENT BURST - slot %d - pl %llx - len %d\n", aBurstSlot, payloadAddress, messageLength);
}
