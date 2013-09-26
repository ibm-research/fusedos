#include "roq_resiliency.h"
#include "supernodes/torus.h"
#include "supernodes/common.h"

#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>

// 8196 packets can wait in the fifos
#define RR_FORWARDER_REC_MEMORY_FIFO_SIZE (32*8196*(512+32)) // 16MB payload + header
#define RR_FORWARDER_INJ_MEMORY_FIFO_SIZE (8196*64)          // 256 headers

static uint32_t gid  = 0;
static uint32_t sgid = 0;

static uint32_t nFifos = 1; // 1 for bursts, 1 for acks
static uint32_t FifoIDs __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = 0;

static MUSPI_RecFifoSubGroup_t * rFifoSubgrp  __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static MUSPI_InjFifoSubGroup_t * iFifoSubgrp  __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;

static Kernel_RecFifoAttributes_t rFifoAttrs __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));
static Kernel_InjFifoAttributes_t iFifoAttrs __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));

static Kernel_MemoryRegion_t * rMemRegion __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static Kernel_MemoryRegion_t * iMemRegion __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;

static MUHWI_Descriptor_Aligned_t * iFifoDesc    __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static MUHWI_Descriptor_Aligned_t * iFifoDescDma __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;

static MUSPI_Pt2PtMemoryFIFODescriptorInfo_t iFifoDescInfo __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));

static void * rFifo    __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static void * rFifoDma __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static void * iFifo    __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static void * iFifoDma __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;

static MUSPI_InjFifo_t * InjFifoDma __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;
static MUSPI_RecFifo_t * RecFifoDma __attribute__ ((aligned (__BIGGEST_ALIGNMENT__))) = NULL;

static uint64_t *forwardBurstCounter = NULL;
static uint64_t *forwardAckCounter = NULL;

static uint32_t lastDstRank[16]; // 8 for ack and burst each

static uint32_t failover = 0;

static struct RoQ_NetworkDescriptor netdesc;

static union recFifoState_t {
	struct {
		uint32_t proc;
		uint32_t avail;
	}__attribute__((packed));
	uint64_t all;
} recFifoState __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));

inline uint32_t Forwarder_getFifoId() {
	return ((sgid+gid*4)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + FifoIDs);
}

uint32_t hole = 0;

static inline void* Forwarder_getNextPacket() {
	uint32_t wrap = 0;
	uint32_t cur_bytes = 0;
	uint32_t b = 0;
	void *clientdata = NULL;
	
	if (likely(recFifoState.proc < recFifoState.avail)) {
		clientdata = MUSPI_getNextPacketOptimized(RecFifoDma, &cur_bytes);

		recFifoState.proc += cur_bytes;
		if (recFifoState.proc >= recFifoState.avail)
			MUSPI_syncRecFifoHwHead(RecFifoDma);
	} else if ((b = MUSPI_getAvailableBytes(RecFifoDma, &wrap)) != 0) {
		if (wrap == 0) {
			/* No fifo wrap.  Process each packet. */
			recFifoState.all = b; // proc = 0, avail = b

			clientdata = MUSPI_getNextPacketOptimized(RecFifoDma, &cur_bytes);

			recFifoState.proc += cur_bytes;
			if (recFifoState.proc >= recFifoState.avail)
				MUSPI_syncRecFifoHwHead(RecFifoDma);

		} else {
			/* Packets wrap around to the top of the fifo.  Handle the one packet
			* that wraps.
			*/
			clientdata = MUSPI_getNextPacketWrap(RecFifoDma, &cur_bytes);

			/* Store the updated fifo head. */
			MUSPI_syncRecFifoHwHead(RecFifoDma);
			recFifoState.all = 0; // proc = 0, avail = 0
		}
	}
	return clientdata;
}


inline static void Forwarder_TX(MUHWI_PacketHeader_t *header) {
	uint32_t dstRank; // destination rank
	uint32_t dstRankClass; // 2x8 classes, depending on the last 3 bits and the packet type
	uint32_t pktType; // ack or burst packet
	uint32_t Put_Offset_LSB;

	// this one needs ~15 cycles on average (including benchmarking overhead).
	dstRank      = *((uint32_t*)(&header->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2[10])) & 0x000FFFFF; // access is 32bit aligned

	// ACK = 0xFFFFFFFF, NACK = 0xFFFFFFFE, BURST = everything else
	// pktType: 0 = ACK/NACK, 1 = BURST
	Put_Offset_LSB = header->messageUnitHeader.Packet_Types.Memory_FIFO.Put_Offset_LSB;
	pktType = ((Put_Offset_LSB ^ 0xFFFFFFFF) & 0xFFFFFFFE) != 0;

	dstRankClass = (dstRank & 0x3) | (pktType << 3);

	// optimize for bursts where the same address comes up multiple times in a row.
	if (unlikely(lastDstRank[dstRankClass] != dstRank)) {
		lastDstRank[dstRankClass] = dstRank;

		uint16_t Hints       = 0;
		uint32_t Rec_FIFO_Id = -1;

		struct RoQ_TransportConnection *tc  = &RoQ_loc_TransportConnection[dstRank];
		MUHWI_Destination_t Destination;
	
		// Check if any more forwarding or custom hint bits are needed.
		if (tc->mForwardFlags & 0x1) {
			Destination = tc->mForwardIntermediate;
			Rec_FIFO_Id = Forwarder_getFifoId();
		} else {
			Destination = tc->PeerNodeAddr;
			Rec_FIFO_Id = pktType; // 1 = burst, 0 = ack
		}

		if (tc->mForwardFlags & 0x2) Hints = tc->mForwardHintBits;

		iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination                         = Destination;
		iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Hints                               = (Hints >> 2);
		// WARNING: I think there's a bug in the driver. E plus/minus seem to be swapped!
		iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus                   = (Hints >> 1) & 0x1;
		iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus                  = (Hints     ) & 0x1;
		iFifoDescDma[dstRankClass].PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id  = Rec_FIFO_Id;
	}

#ifdef ROQ_RESILIENCY_DEBUG 
	struct RoQ_TransportConnection *tc  = &RoQ_loc_TransportConnection[dstRank];

	DPRINTF(ROQ_RESILIENCY_DEBUG, "Forwarding packet to (%d,%d,%d,%d,%d) via (%d,%d,%d,%d,%d), fifo id = %d, offset = %x, hints = %03x, offset = %08x, type = %d, this = (%d,%d,%d,%d,%d)\n",
		   tc->PeerNodeAddr.Destination.A_Destination,
		   tc->PeerNodeAddr.Destination.B_Destination,
		   tc->PeerNodeAddr.Destination.C_Destination,
		   tc->PeerNodeAddr.Destination.D_Destination,
		   tc->PeerNodeAddr.Destination.E_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination,
		   iFifoDescDma[dstRankClass].PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id,
		   RoQ_MUHWI_MUHdr_PutOffset_LSB(header),
		   (((iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Hints              & 0xFF) << 2) |
			((iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus & 0x01) << 1) |
			((iFifoDescDma[dstRankClass].PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus  & 0x01) << 0)),
		   header->messageUnitHeader.Packet_Types.Memory_FIFO.Put_Offset_LSB, pktType,
		   global.coords.A, global.coords.B, global.coords.C, global.coords.D, global.coords.E);

#endif

	void *PktPayloadPtr = (void*) (((char*) header) + 32);

	MUSPI_SetPayload(&iFifoDescDma[dstRankClass],
					 (uint64_t)(domain_physddr_start() + ((uint64_t)PktPayloadPtr & ~ROQ_VIRTUAL_MAPPING)), // needs to be the physical address
					 header->NetworkHeader.pt2pt.Byte8.Size << 5); // * 32

    // RoQ currently doesn't use the bits above 32 (i.e. no Put_Offset_MSB)
	iFifoDescDma[dstRankClass].PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Put_Offset_LSB = Put_Offset_LSB;

	// Copy software bytes
	MUSPI_SetSoftwareBytes(&iFifoDescDma[dstRankClass],
						   &(header->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2[0]));

	uint64_t desc;
	do {
		desc = MUSPI_InjFifoInject (InjFifoDma, &iFifoDescDma[dstRankClass]);
	} while (unlikely(desc == -1));
}


inline void Forwarder_RX() {
	/*
	  The purpose of this code is this: 1. Run as fast as you can, if there are packets in
	  the fifo. If not, then you should go back to sleep. But wait a bit, in case this is
	  just a short interruption in the packet flow. However, if there was no packet in the
	  fifo at the beginning, then it's unlikely that one will arrive anytime soon. Go back
	  to sleep immediately.

	  the wait time of 0.1 seconds iterations was choosen arbitrarily. One should do some
	  tests to figure out how long a typical interruption is (which probably depends on
	  the application, I'd focus on high-throughput here).
	 */

	void * PktPointer = NULL;

	PktPointer = Forwarder_getNextPacket();
	
	if (likely (PktPointer == NULL)) return;

	while (1) {
		
		do {
			Forwarder_TX((MUHWI_PacketHeader_t *)PktPointer);
			PktPointer = Forwarder_getNextPacket();
		} while (likely(PktPointer != NULL));

		hole++;

		// sometimes the user decrementer is not available. reenable it.
		mtspr( SPRN_TCR, mfspr(SPRN_TCR) | TCR_UD );
        // set user decrementer/timer to wait 0.1 seconds for another packet, then go
		// back to sleep.
		mtspr( SPRN_UDEC, config.forwarder.wait*global.freqMhz ); 

		do {
			if (mfspr(SPRN_UDEC) == 0) return;
			PktPointer = Forwarder_getNextPacket();
		} while (likely(PktPointer == NULL));
	}
}

int setupForwarder() {
	int rc = 0;
	int i  = 0;

	uint64_t rFifoEnableBits = 0;

	DEBUG("setupForwarder\n");
	
	// select group and subgroup according to the recommendation in BGQ_mu_hld_latest.pdf.
	gid  = ProcessorCoreID();
	sgid = ProcessorThreadID();
	
	DEBUG("gid=%d, sgid=%d\n", gid, sgid);

	DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */
	DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */
		
	rFifoAttrs.System    = 0;

	iFifoAttrs.RemoteGet = 0;
	iFifoAttrs.System    = 0;
	iFifoAttrs.Priority  = 0;
		
	//  DCRWritePriv(MU_DCR(IMFIFO_SYSTEM)+gid, 0xFFFFFFFFFFFFFFFFULL);
	//  DCRWritePriv(MU_DCR(RMFIFO_SYSTEM)+gid, 0xFFFFFFFFFFFFFFFFULL);
	
	/* From BGQ_mu_hld_latest.pdf:
	 * There are 17 “groups” of injection FIFOS, with 32 imFIFOs per group. In addition,
	 * there are 4 subgroups per group. This allows software to assign processors and
	 * threads to groups or subgroups. Note that any processor can be assigned to any
	 * group. However, as will be seen later, for handling interrupts, it is most natural
	 * to assign processor p to group p.
	 */

	RoQ_LockAcquire(&global.setupLock);

	rFifoSubgrp = (MUSPI_RecFifoSubGroup_t *) (ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)malloc(sizeof(MUSPI_RecFifoSubGroup_t)) & ~ROQ_VIRTUAL_MAPPING));
	iFifoSubgrp = (MUSPI_InjFifoSubGroup_t *) (ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)malloc(sizeof(MUSPI_InjFifoSubGroup_t)) & ~ROQ_VIRTUAL_MAPPING));

	rc = Kernel_AllocateRecFifos(sgid+gid*4,
								 rFifoSubgrp,
								 nFifos,
								 &FifoIDs,
								 &rFifoAttrs);
	if (rc) ERROR ("%s:%s:%d Kernel_AllocateRecFifo failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rc = Kernel_AllocateInjFifos(sgid+gid*4,
								 iFifoSubgrp,
								 nFifos,
								 &FifoIDs,
								 &iFifoAttrs);
	if (rc) ERROR ("%s:%s:%d Kernel_AllocateInjFifo failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);

	iFifoDesc    = malloc(sizeof(MUHWI_Descriptor_Aligned_t)*16);
	iFifoDescDma = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)iFifoDesc & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(iFifoDesc != NULL, "ERROR: iFifoDesc[%u] == NULL\n", i);
	
	rFifo    = (void *)(((uint64_t)malloc(RR_FORWARDER_REC_MEMORY_FIFO_SIZE+32) + 32) & ~(32-1));
	rFifoDma = (void *)(domain_physddr_start() + ((uint64_t)rFifo & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(rFifo != (void *) 32, "ERROR: rFifo == NULL\n");
	
	iFifo    = (void *)(((uint64_t)malloc(RR_FORWARDER_INJ_MEMORY_FIFO_SIZE+64) + 64) & ~(64-1));
	iFifoDma = (void *)(domain_physddr_start() + ((uint64_t)iFifo & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(iFifo != (void *) 64, "ERROR: iFifo == NULL\n");
	
	rMemRegion = (Kernel_MemoryRegion_t *) malloc(sizeof(Kernel_MemoryRegion_t));
	iMemRegion = (Kernel_MemoryRegion_t *) malloc(sizeof(Kernel_MemoryRegion_t));
	
	DSTRONG_ASSERT(rMemRegion != NULL, "ERROR: rMemRegion[%u] == NULL\n", i);
	DSTRONG_ASSERT(iMemRegion != NULL, "ERROR: iMemRegion[%u] == NULL\n", i);
	
	rMemRegion->BaseVa = (void *) rFifo;
	rMemRegion->BasePa = (void *) rFifoDma;
	rMemRegion->Bytes  = RR_FORWARDER_REC_MEMORY_FIFO_SIZE-1;
	
	iMemRegion->BaseVa = (void *) iFifo;
	iMemRegion->BasePa = (void *) iFifoDma;
	iMemRegion->Bytes  = RR_FORWARDER_INJ_MEMORY_FIFO_SIZE-1;
	
	uint64_t startoffset = 0;
	rc = Kernel_RecFifoInit(rFifoSubgrp,
							FifoIDs,
							rMemRegion,
							startoffset,
							RR_FORWARDER_REC_MEMORY_FIFO_SIZE-1);
	if (rc) ERROR ("%s:%s:%d Kernel_RecFifoInit failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rc = Kernel_InjFifoInit(iFifoSubgrp,
							FifoIDs,
							iMemRegion,
							startoffset,
							RR_FORWARDER_INJ_MEMORY_FIFO_SIZE-1);
	if (rc) ERROR ("%s:%s:%d Kernel_InjFifoInit failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rFifoEnableBits |= (0x0000000000000001ULL << (15 - (sgid*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + FifoIDs)));
	
	/* Initialize static fields to save some time in the forwarding function */
	bzero(&iFifoDescInfo, sizeof(MUSPI_Pt2PtMemoryFIFODescriptorInfo_t));
	
	iFifoDescInfo.Pt2Pt.Misc2            = MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
	iFifoDescInfo.Pt2Pt.Skip             = 0;
	iFifoDescInfo.Pt2Pt.Misc1            = (MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE   |
											MUHWI_PACKET_USE_DETERMINISTIC_ROUTING |
											MUHWI_PACKET_DO_NOT_DEPOSIT);
	iFifoDescInfo.Base.Pre_Fetch_Only    = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
	iFifoDescInfo.MemFIFO.Interrupt      = MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
	
	int c;
	for(c = 0; c < 8; c++) {
		MUSPI_CreatePt2PtMemoryFIFODescriptor(&iFifoDescDma[c],       &iFifoDescInfo);
		MUSPI_CreatePt2PtMemoryFIFODescriptor(&iFifoDescDma[c | 0x8], &iFifoDescInfo);

		MUSPI_SetSize(&iFifoDescDma[c]);
		MUSPI_SetSize(&iFifoDescDma[c | 0x8]);

		// use fifos B- to E+. Note that the names are missleading. It has nothing to do
		// with actual links. These are just injection fifo engine names and each can 
        // serve any link.
		MUSPI_SetTorusInjectionFIFOMap(&iFifoDescDma[c],       MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM >> c);
		MUSPI_SetTorusInjectionFIFOMap(&iFifoDescDma[c | 0x8], MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM >> c);
	}
	
	InjFifoDma = (MUSPI_InjFifo_t *)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToInjFifo(FifoIDs, iFifoSubgrp));
	RecFifoDma = (MUSPI_RecFifo_t *)MUSPI_IdToRecFifo(FifoIDs, rFifoSubgrp);

	rc = Kernel_InjFifoActivate(iFifoSubgrp,
								nFifos,
								&FifoIDs,
								KERNEL_INJ_FIFO_ACTIVATE);
	if (rc) ERROR ("%s:%s:%d Kernel_InjFifoActivate failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rc = Kernel_RecFifoEnable(gid, rFifoEnableBits);
	if (rc) ERROR ("%s:%s:%d Kernel_RecfifoEnable failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);

	forwardBurstCounter = malloc(sizeof(uint64_t)*global.nodeCount);
	bzero(forwardBurstCounter, sizeof(uint64_t)*global.nodeCount);
	forwardAckCounter = malloc(sizeof(uint64_t)*global.nodeCount);
	bzero(forwardAckCounter, sizeof(uint64_t)*global.nodeCount);

	recFifoState.all = 0;

	netdesc = (struct RoQ_NetworkDescriptor) {torus.local.phy.A,
											  torus.local.phy.B,
											  torus.local.phy.C,
											  torus.local.phy.D,
											  torus.local.phy.E,
											  torus.size.dim[torus.rdo.dim[torus.rdo.A]],
											  torus.size.dim[torus.rdo.dim[torus.rdo.B]],
											  torus.size.dim[torus.rdo.dim[torus.rdo.C]],
											  torus.size.dim[torus.rdo.dim[torus.rdo.D]],
											  torus.size.dim[torus.rdo.dim[torus.rdo.E]],
											  0};
	
	DEBUG("log size = (%d,%d,%d,%d,%d), phy size = (%d,%d,%d,%d,%d), rdo = (%d,%d,%d,%d,%d)\n",
		  torus.size.A, torus.size.B, torus.size.C, torus.size.D, torus.size.E,
		  torus.size.dim[torus.rdo.dim[torus.rdo.A]],
		  torus.size.dim[torus.rdo.dim[torus.rdo.B]],
		  torus.size.dim[torus.rdo.dim[torus.rdo.C]],
		  torus.size.dim[torus.rdo.dim[torus.rdo.D]],
		  torus.size.dim[torus.rdo.dim[torus.rdo.E]],
		  torus.rdo.A, torus.rdo.B, torus.rdo.C, torus.rdo.D, torus.rdo.E);
	
	DEBUG("local = (%d,%d,%d,%d,%d), size = (%d,%d,%d,%d,%d)\n",
		  netdesc.Acoord, netdesc.Bcoord, netdesc.Ccoord, netdesc.Dcoord, netdesc.Ecoord,
		  netdesc.Anodes, netdesc.Bnodes, netdesc.Cnodes, netdesc.Dnodes, netdesc.Enodes);
		  
	memset(lastDstRank, (uint32_t)-1, sizeof(uint32_t[16]));

	RoQ_LockRelease(&global.setupLock);

	if (rc == 0) {
		INFO("Forwarder successfully set up\n");
	} else {
		ERROR("Forwarder setup failed\n");
	}
  return rc;
}


static void Forwarder_decrementerInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	/* Interrupts disable the floating point unit but we need that for efficient
	 * memcpy. Reenable it here.
	 */
	mtmsr(mfmsr() | MSR_FP);

	Forwarder_RX(); // run forwarder as long as there are packets in the fifo.

	// clear interrupts
	mtspr(SPRN_TSR, TSR_DIS);  // Ack decrementer interrupt
}

static void Forwarder_externalInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	// re-enable FPU for fast memcopy, also switch into privileged mode to enable the
	// decrementer interrupt.
	mtmsr(mfmsr() | MSR_FP);
	
	// 0. Clear BIC external interrupts
	unsigned int tid = ProcessorID();
	
	BIC_REGISTER reg0 = BIC_ReadStatusExternalRegister0(tid);
	BIC_REGISTER reg1 = BIC_ReadStatusExternalRegister1(tid);
	
	if (reg0 != 0) BIC_WriteClearExternalRegister0(tid, reg0);
	if (reg1 != 0) BIC_WriteClearExternalRegister1(tid, reg1);
	if (!failover) {
		uint32_t decr = config.forwarder.period*global.freqMhz; // check for new packets every .1 seconds
		enableDecrementerInterrupt(decr, decr, 1);   // 1 = autoreload

		failover = 1;
	} else {
		// Reset these to invalidate the target cache of the Forwarder_TX function.
		memset(lastDstRank, (uint32_t)-1, sizeof(uint32_t[16]));
	}
}


void Forwarder() {
  int rc = 0;

  rc = setupForwarder();

  if (rc) {
    PRINTF(RR_FORWARDER_ERROR, "Setting up forwarding thread failed (rc=%d)\n", rc);
    return;
  }
  
  // Allow all cores to sleep
  mtspr(SPRN_CCR0, CCR0_WEM(0xF));
  
  setInterruptHandler(Forwarder_decrementerInterruptHandler, IVO_DEC);
  setInterruptHandler(Forwarder_externalInterruptHandler, IVO_EI);

  asm volatile ("wrteei 1"); // set external enable bit

  // Make sure the user decrementer interrupt doesn't actually cause
  // an interrupt and is available. I use it as a cheap method to poll
  // for a certain amount of time and not really as an interrupt.
  mtspr( SPRN_TCR, (mfspr(SPRN_TCR) & ~TCR_UDIE) | TCR_UD );
  
  while (1) {
	  // go back to sleep and wait for next interrupt to wake me up again.
	  asm volatile ("wait" : : : "memory" );
  }
}
