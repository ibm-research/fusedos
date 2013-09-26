#include "roq_resiliency.h"
#include "supernodes/torus.h"

#include <stdio.h>

#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>

/* Notify partition of a bad node using peer-to-peer communication 
 * 
 * This notifier passes the information about a bad node on to all its neighbors.
 * The neighbors then also pass on this information to all their neighbors. This
 * way the information about a bad node will reach every node even if there is only
 * a single working link between two sub-partitions.
 */

/* Data structure to store bad link information
 *
 * --------------------------------- REQUIREMENTS ---------------------------------
 * 1. It must be very efficient to decide if a route is possible or not.
 * 2. It must be very efficient to find a valid intermediate node.
 * 3. It shouldn't be too hard to decide whether a link is already know to be bad
 *
 * ------------------------------- ROUTING ALGORITHM ------------------------------
 * Idea 0: Naive approach: Store the bad links in a list and compare dimensions.
 * Probably the most efficient approach for only a few bad links. But it doesn't
 * scale well.
 *
 * Idea 1: Use matrices and binary operations on it.
 * Represent route and defects as matrix. Each entry represents a node by a 8 bit
 * value (1 for each direction - we will have at most a 4D torus).
 * The route has an entry for each node that it passes. The bit that represents the
 * links taken is set to 1. The bad link matrix has a 1 for every bad link of each
 * node. If we AND both matrices we know if there is any overlapping.
 * Let's say we have a 16x16x16x16x1 torus (65k nodes, which is larger than what
 * we're going for at this moment).
 * Performance: This approach is independent of the amount of bad links, but it
 * depends on the partition size. 
 *
 * Idea 2: Set operations. Represent the route as a set of links. Do the same for
 * the bad link list. Perform a set intersection to determin validity of the route.
 * Probably only works for a few bad links. But not if there are many bad ones.
 */

#define RR_NOTIFIER_REC_MEMORY_FIFO_SIZE (128*(512+32)) // 128 packets with max payload (512) + header (32)
#define RR_NOTIFIER_INJ_MEMORY_FIFO_SIZE (128*64) // 128 injection descriptors (64)

//#define BENCHMARK 1

typedef union {
	uint8_t raw[14];

	struct {
		dim_t    link;       // failing link (5 bytes)
		uint8_t  dir;        // direction this information was sent out to.
		uint16_t seq;        // sequence number of the packet = hop count
		uint16_t padding[2];
	} __attribute__((__packed__));
	
} NotifyPacket_t;

static uint16_t lcl_badLinksKnown = 0;

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

static uint32_t failover = 0;

static uint64_t periods = 0;

static int Notifier_Send(dim_t *link, uint8_t sd, uint16_t seq);

static union recFifoState_t {
	struct {
		uint32_t proc;
		uint32_t avail;
	}__attribute__((packed));
	uint64_t all;
} recFifoState __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));


static inline void* Notifier_getNextPacket() {
	uint32_t wrap = 0;
	uint32_t cur_bytes = 0;
	uint32_t b = 0;
	void *clientdata = NULL;
	
	if (likely(recFifoState.proc < recFifoState.avail)) {
		clientdata = MUSPI_getNextPacketOptimized(RecFifoDma, &cur_bytes);
		recFifoState.proc += cur_bytes;
	} else {

		if (recFifoState.proc >= recFifoState.avail)
			MUSPI_syncRecFifoHwHead(RecFifoDma);

		if ((b = MUSPI_getAvailableBytes(RecFifoDma, &wrap)) != 0) {

			if (wrap == 0) {
				/* No fifo wrap.  Process each packet. */
				recFifoState.all = b; // avail = b, proc = 0
			} else {
				/* Packets wrap around to the top of the fifo.  Handle the one packet
				 * that wraps.
				 */
				clientdata = MUSPI_getNextPacketWrap(RecFifoDma, &cur_bytes);
				/* Store the updated fifo head. */
				recFifoState.all = 1 << 32; // proc = 1, avail = 0
			}
		}
	}
	return clientdata;
}


static void Notifier_packetHandler (MUHWI_PacketHeader_t *header) {
	NotifyPacket_t *pkt = (NotifyPacket_t *)(header->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2);
	dim_t           l   = pkt->link;
	uint8_t         dir = pkt->dir;
	uint32_t        seq = pkt->seq;

#ifdef ROQ_RESILIENCY_DEBUG
	uint8_t d = lnk_getDimension(l);

	dim_t   n0, n1;
	lnk_getEndpoints(l, &n0, &n1);

	uint8_t l0, l1;
	l0 = (d << 1) + 1; // plus
	l1 = (d << 1);     // minus
	
	printf("Someone told me that (%d,%d,%d,%d,%d) = (%d,%d,%d,%d,%d).%s <--> %s.(%d,%d,%d,%d,%d) is broken (period %d, seq %d, time %lld)\n",
		   l.A, l.B, l.C, l.D, l.E,
		   n0.A, n0.B, n0.C, n0.D, n0.E,
		   linkNum2Str[l0], linkNum2Str[l1],
		   n1.A, n1.B, n1.C, n1.D, n1.E, periods, seq, GetTimeBase());
#endif

#ifdef BENCHMARK
	if (l.A == 0xFF || pushOnce(&gbl_badLinksNew, &l) == 0) {
#else
	if (pushOnce(&gbl_badLinksNew, &l) == 0) {
#endif
		Notifier_Send(&l, dir2inv[dir], seq);
		// activate router thread to update supernode graph (and routes eventually)
		wakeupThread(ROQ_ROUTER_THREAD);
	}

	if (!failover) {
		wakeupThread(ROQ_FORWARDER_THREAD);
		failover = 1;
	}
}

/* - link : failed link
   - sd   : skip sending the information out to this direction/link
*/
static int Notifier_Send(dim_t *link, uint8_t sd, uint16_t seq) {
	int rc = 0;
	int i  = 0;
	
	NotifyPacket_t pkt;
	pkt.link = *link;
	pkt.seq  = seq+1; // increment sequence number

#ifdef BENCHMARK
	if (seq > 10 && link->A == 0xFF) return;
#endif

	for (i = 0; i < global.dimensions*2; i++) {
		if (global.neighbors[i] == NULL || i == sd) continue;

		pkt.dir = i;

		MUSPI_SetSoftwareBytes(iFifoDescDma, pkt.raw);
		
		MUHWI_Destination_t dest;
		dest.Destination.Destination = ((global.neighbors[i]->A << 24) |
										(global.neighbors[i]->B << 18) |
										(global.neighbors[i]->C << 12) |
										(global.neighbors[i]->D <<  6) |
										(global.neighbors[i]->E      ));
		MUSPI_SetDestination (iFifoDescDma, dest);

		// force the package to go over a specific link in case the default one is broken.
		uint16_t hints = dir2hint[i];
		MUSPI_SetHints (iFifoDescDma, hints >> 2, hints & 0x3);

		DSTRONG_ASSERT(InjFifoDma->hw_injfifo != NULL, "InjFifoDma->hw_injfifo == NULL\n");
		DSTRONG_ASSERT(&InjFifoDma->hw_injfifo->freeSpace != NULL, "&InjFifoDma->hw_injfifo->freeSpace == NULL\n");

#ifdef BENCHMARK
		printf("Sending/forwarding notification about bad link (%d,%d,%d,%d,%d) via link %s (period %d, seq %d)\n", 
			   link->A, link->B, link->C, link->D, link->E, dir2str[i], periods, seq);
#endif
		do {
			rc = MUSPI_InjFifoInject (InjFifoDma, iFifoDescDma);
		} while (rc == -1);
	}

#ifndef BENCHMARK
	printf("Sending/forwarding notification about bad link (%d,%d,%d,%d,%d) (period %d, seq %d)\n", 
		   link->A, link->B, link->C, link->D, link->E, periods, seq);
#endif
	return rc;
}

static int Notifier_Setup() {
	int rc = 0;
	int i  = 0;

	uint64_t rFifoEnableBits = 0;

	DEBUG("Setting up Notifier\n");

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
	iFifoAttrs.Priority  = 1;
		
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

	INFO("iFifoSubgrp = %p, rFifoSubgrp = %p\n", iFifoSubgrp, rFifoSubgrp);
	INFO("rFifoSubgrp = %p, ->_recfifos[0] = %p, ->_recfifos[0]._fifo = %p, ->_recfifos[0]._fifo.hwfifo = %p, ->_recfifos[0]._fifo.hwfifo->pa_start = %p\n",
		 rFifoSubgrp,
		 &rFifoSubgrp->_recfifos[0],
		 &rFifoSubgrp->_recfifos[0]._fifo,
		 rFifoSubgrp->_recfifos[0]._fifo.hwfifo,
		 &rFifoSubgrp->_recfifos[0]._fifo.hwfifo->pa_start);
	INFO("rFifoSubgrp = %p, ->_recfifos[1] = %p, ->_recfifos[1]._fifo = %p, ->_recfifos[1]._fifo.hwfifo = %p, ->_recfifos[1]._fifo.hwfifo->pa_start = %p\n",
		 rFifoSubgrp,
		 &rFifoSubgrp->_recfifos[1],
		 &rFifoSubgrp->_recfifos[1]._fifo,
		 rFifoSubgrp->_recfifos[1]._fifo.hwfifo,
		 &rFifoSubgrp->_recfifos[1]._fifo.hwfifo->pa_start);

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

	INFO("rFifoSubgrp = %p, ->_recfifos[0] = %p, ->_recfifos[0]._fifo = %p, ->_recfifos[0]._fifo.hwfifo = %p, ->_recfifos[0]._fifo.hwfifo->pa_start = %p\n",
		 rFifoSubgrp,
		 &rFifoSubgrp->_recfifos[0],
		 &rFifoSubgrp->_recfifos[0]._fifo,
		 rFifoSubgrp->_recfifos[0]._fifo.hwfifo,
		 &rFifoSubgrp->_recfifos[0]._fifo.hwfifo->pa_start);
	INFO("rFifoSubgrp = %p, ->_recfifos[1] = %p, ->_recfifos[1]._fifo = %p, ->_recfifos[1]._fifo.hwfifo = %p, ->_recfifos[1]._fifo.hwfifo->pa_start = %p\n",
		 rFifoSubgrp,
		 &rFifoSubgrp->_recfifos[1],
		 &rFifoSubgrp->_recfifos[1]._fifo,
		 rFifoSubgrp->_recfifos[1]._fifo.hwfifo,
		 &rFifoSubgrp->_recfifos[1]._fifo.hwfifo->pa_start);
	

	iFifoDesc    = malloc(sizeof(MUHWI_Descriptor_Aligned_t));
	iFifoDescDma = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)iFifoDesc & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(iFifoDesc != NULL, "ERROR: iFifoDesc[%u] == NULL\n", i);
	
	rFifo    = (void *)(((uint64_t)malloc(RR_NOTIFIER_REC_MEMORY_FIFO_SIZE+32) + 32) & ~(32-1));
	rFifoDma = (void *)(domain_physddr_start() + ((uint64_t)rFifo & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(rFifo != (void *) 32, "ERROR: rFifo == NULL\n");
	
	iFifo    = (void *)(((uint64_t)malloc(RR_NOTIFIER_INJ_MEMORY_FIFO_SIZE+64) + 64) & ~(64-1));
	iFifoDma = (void *)(domain_physddr_start() + ((uint64_t)iFifo & ~ROQ_VIRTUAL_MAPPING));
	DSTRONG_ASSERT(iFifo != (void *) 64, "ERROR: iFifo == NULL\n");
	
	rMemRegion = (Kernel_MemoryRegion_t *) malloc(sizeof(Kernel_MemoryRegion_t));
	iMemRegion = (Kernel_MemoryRegion_t *) malloc(sizeof(Kernel_MemoryRegion_t));
	
	DSTRONG_ASSERT(rMemRegion != NULL, "ERROR: rMemRegion[%u] == NULL\n", i);
	DSTRONG_ASSERT(iMemRegion != NULL, "ERROR: iMemRegion[%u] == NULL\n", i);
	
	rMemRegion->BaseVa = (void *) rFifo;
	rMemRegion->BasePa = (void *) rFifoDma;
	rMemRegion->Bytes  = RR_NOTIFIER_REC_MEMORY_FIFO_SIZE-1;
	INFO("rMemRegion[%u]->BaseVa = %p BasePa = %p Bytes = %llx\n", i,
		 rMemRegion->BaseVa,
		 rMemRegion->BasePa,
		 rMemRegion->Bytes);
	
	iMemRegion->BaseVa = (void *) iFifo;
	iMemRegion->BasePa = (void *) iFifoDma;
	iMemRegion->Bytes  = RR_NOTIFIER_INJ_MEMORY_FIFO_SIZE-1;
	INFO("iMemRegion[%u]->BaseVa = %p BasePa = %p Bytes = %llx\n", i,
		 iMemRegion->BaseVa,
		 iMemRegion->BasePa,
		 iMemRegion->Bytes);
	
	uint64_t startoffset = 0;
	rc = Kernel_RecFifoInit(rFifoSubgrp,
							FifoIDs,
							rMemRegion,
							startoffset,
							RR_NOTIFIER_REC_MEMORY_FIFO_SIZE-1);
	if (rc) ERROR ("%s:%s:%d Kernel_RecFifoInit failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rc = Kernel_InjFifoInit(iFifoSubgrp,
							FifoIDs,
							iMemRegion,
							startoffset,
							RR_NOTIFIER_INJ_MEMORY_FIFO_SIZE-1);
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

	iFifoDescInfo.Base.Payload_Address   = 0;
	iFifoDescInfo.Base.Message_Length    = 0;
	iFifoDescInfo.MemFIFO.Rec_Put_Offset = 0;
	iFifoDescInfo.MemFIFO.Rec_FIFO_Id    = (sgid+gid*4)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + FifoIDs;

	MUSPI_CreatePt2PtMemoryFIFODescriptor(iFifoDescDma, &iFifoDescInfo);
	MUSPI_SetSize(iFifoDescDma);
	
	MUHWI_Destination_t dest;
	dest.Destination.Destination = 0;
	MUSPI_SetDestination(iFifoDescDma, dest);
	
	MUSPI_SetTorusInjectionFIFOMap(iFifoDescDma, MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_PRIORITY);
	
	InjFifoDma = (MUSPI_InjFifo_t *)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToInjFifo(FifoIDs, iFifoSubgrp));
	RecFifoDma = (MUSPI_RecFifo_t *)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToRecFifo(FifoIDs, rFifoSubgrp));

	rc = Kernel_InjFifoActivate(iFifoSubgrp,
								nFifos,
								&FifoIDs,
								KERNEL_INJ_FIFO_ACTIVATE);
	if (rc) ERROR ("%s:%s:%d Kernel_InjFifoActivate failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);
	
	rc = Kernel_RecFifoEnable(gid, rFifoEnableBits);
	if (rc) ERROR ("%s:%s:%d Kernel_RecfifoEnable failed (rc=%d)\n",
				   __FILE__, __FUNCTION__, __LINE__, rc);

	INFO("domain_physddr_start() = %016x\n", domain_physddr_start());

	recFifoState.all = 0;

	RoQ_LockRelease(&global.setupLock);
	
	if (rc == 0) {
		INFO("Notified successfully set up\n");
	} else {
		ERROR("Notifier setup failed\n");
	}
	
	return rc;
}

static inline void Notifier_poll() {
	// there seems to be some kind of bug in Notifier_getNextPacket that sometimes
	// requires polling twice to get a packet. It seems to me that this happens every 3rd
	// time, e.g. 110110110, i.e. every 3rd time I get no packet from polling. it's never
	// lost, just delayed by 1

	int i;
	void * PktPointer = NULL;

	for(i = 0; i < 2; i++) {
		PktPointer = Notifier_getNextPacket();
		if (PktPointer != NULL) break;
	}

	if (likely (PktPointer == NULL)) return;

	while (1) {
		
		do {
			Notifier_packetHandler((MUHWI_PacketHeader_t *)PktPointer);
			PktPointer = Notifier_getNextPacket();
		} while (likely(PktPointer != NULL));

		// sometimes the user decrementer is not available. reenable it.
		mtspr( SPRN_TCR, mfspr(SPRN_TCR) | TCR_UD );
        // set user decrementer/timer to wait 0.1 seconds for another packet, then go
		// back to sleep.
		mtspr( SPRN_UDEC, config.broadcast.wait*global.freqMhz ); 

		do {
			if (mfspr(SPRN_UDEC) == 0) return;
			PktPointer = Notifier_getNextPacket();
		} while (likely(PktPointer == NULL));
	}
}

static void Notifier_decrementerInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	/* Interrupts disable the floating point unit but we need that for efficient
	 * memcpy. Reenable it here.
	 */
	mtspr(SPRN_TSR, TSR_DIS);  // Ack decrementer interrupt
	mtmsr(mfmsr() | MSR_FP);   // re-enable FPU for fast memcopy

	periods++;

	Notifier_poll();

	static uint64_t n = 1;
	
	if (torus.nodes <= 128)  {
		// print bad link list once a minute
		if (get_time()/60000000 >= n) {
			n++;
			Router_printBadLinkList();
			Router_printRoutes();
		}
	}

#ifdef BENCHMARK
	// start with the first node in period 100. Then all the other nodes in periods 200, 300, ...
	static int b = 0;
	if (b == 0 && torus.local.phy.raw == 0 && get_time()/60000000 >= 3) {
		b = 1;
		printf("starting benchmark (period %d)...\n", periods);
		dim_t link = (dim_t) {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
		Notifier_Send(&link, 5, 0);
	}
#endif
}

static void Notifier_externalInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	mtmsr(mfmsr() | MSR_FP); // re-enable FPU for fast memcopy

	// 0. Clear BIC external interrupts
	unsigned int tid = ProcessorID();

	BIC_REGISTER reg0 = BIC_ReadStatusExternalRegister0(tid);
	BIC_REGISTER reg1 = BIC_ReadStatusExternalRegister1(tid);

	if (reg0 != 0) BIC_WriteClearExternalRegister0(tid, reg0);
	if (reg1 != 0) BIC_WriteClearExternalRegister1(tid, reg1);
	
    // no locks needed here, I only read.
	uint16_t lcl_badLinksNew = lcl_badLinks ^ lcl_badLinksKnown;
	// remember already processed links
	lcl_badLinksKnown |= lcl_badLinksNew;

/*
	uint64_t bla = GetTimeBase();
	while (bla+(150000*1600) > GetTimeBase()) {
		printf("waiting...\n");
	}
*/

	while (lcl_badLinksNew != 0) {
		int   dir  = __builtin_ffs(lcl_badLinksNew)-1; // get broken link
		dim_t link = lnk_getLinkByDir(dir);

		// Forward message to all direct neighbors
		Notifier_Send(&link, -1, 0);

		// push link onto stack. will be popped by router thread
#ifdef BENCHMARK
		if (link.A != 0xFF) {
#endif
		pushOnce(&gbl_badLinksNew, &link);

		// activate router thread to update supernode graph (and routes eventually)
		wakeupThread(ROQ_ROUTER_THREAD);

		// deactivate link, reset tokens, ...
		shutdownLink(dir);
		
		// mark link as processed
		lcl_badLinksNew ^= 1 << dir;

		INFO("Marking link (%d,%d,%d,%d,%d) = (%d,%d,%d,%d,%d).%s <--> %s.(%d,%d,%d,%d,%d) as bad\n",
			 link.A, link.B, link.C, link.D, link.E, 
			 global.coords.A, global.coords.B, global.coords.C, global.coords.D, global.coords.E,
			 linkNum2Str[dir], linkNum2Str[dir+(dir%2 ? -1 : 1)],
			 global.neighbors[dir]->A, global.neighbors[dir]->B, global.neighbors[dir]->C, global.neighbors[dir]->D, global.neighbors[dir]->E);
#ifdef BENCHMARK
		}
#endif
	}

	if (!failover) {
		wakeupThread(ROQ_FORWARDER_THREAD);
		failover = 1;
	}
}

void Notifier_Peer2Peer() {
	int rc = 0;

	rc = Notifier_Setup();

	if (rc) {
		ERROR("RoQ Resiliency Notifier Setup failed. Cannot start notifier daemon.\n");
		return;
	}

	// Allow all cores to sleep
	mtspr(SPRN_CCR0, CCR0_WEM(0xF));

	setInterruptHandler(Notifier_decrementerInterruptHandler, IVO_DEC);
	setInterruptHandler(Notifier_externalInterruptHandler, IVO_EI);

	// if the sum of the dimensions is odd, delay polling for half of
	// the period. This way the periods of all neighbor nodes should
    // interleave nicely, thus reducing the broadcast latency

	uint32_t period = config.broadcast.period * global.freqMhz;
	uint32_t init   = config.broadcast.period * global.freqMhz;

	// force a shift in polling time to speed up packet forwarding (at
	// least in theory, it doesn't seem to work)
	if ((global.coords.A + 
		 global.coords.B + 
		 global.coords.C + 
		 global.coords.D + 
		 global.coords.E) & 0x1) init += init >> 1;

	enableDecrementerInterrupt(period, init, 1); // 1 = autoreload

	while (1) {
		// go back to sleep and wait for next interrupt to wake me up again.
		asm volatile ("wait" : : : "memory" );
	}
}
