#include "roq_resiliency.h"
#include "supernodes/sng_common.h"
#include "supernodes/torus.h"

#include <hwi/include/bqc/nd_500_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <roq_fw.h>

#ifdef ROQ_RESILIENCY_DEBUG
#define ROUTER_DEBUG 1
#else
#define ROUTER_DEBUG 0
#endif

supernode_matrix_t sng;   // supernode graph
torus_t            torus; // torus properties

static uint32_t linkIndex = 0;
static struct RoQ_NetworkDescriptor netdesc;

extern uint32_t hole;

int Router_injectFault(dim_t l) {
	int r = lnk_injectFault(l);
    sng_print();

	return r;
}

/* - n     : destination node
   - fw    : forwarding node
   - hint  : network hint bits
   - flags : packet needs forwarding (0x2), custom hint bits (0x2), both (0x3) or is cut off (0x4)
*/
void Router_setRoute(dim_t n, dim_t fw, uint16_t hint, int flags) {

	uint32_t nr;
	RoQ_TorusCoords_To_Rank(&netdesc, n.A, n.B, n.C, n.D, n.E, &nr);

	DPRINTF(ROUTER_DEBUG, "Setting (%d,%d,%d,%d,%d) as forwarding node for (%d,%d,%d,%d,%d), rank %d, flags %x, hint %02x, size = (%d,%d,%d,%d,%d), rdo = (%d,%d,%d,%d,%d)\n",
			fw.A, fw.B, fw.C, fw.D, fw.E,
			n.A, n.B, n.C, n.D, n.E, nr, flags, hint,
			netdesc.Anodes, netdesc.Bnodes, netdesc.Cnodes, netdesc.Dnodes, netdesc.Enodes, torus.rdo.A, torus.rdo.B, torus.rdo.C, torus.rdo.D, torus.rdo.E);
	int r;

	// TODO: Rework the routing table so that only one entry per destination node has to be set.
	for (r = 0; r < 1; r++) {	
		struct RoQ_TransportConnection *tc = NULL;

		if (r == 0) {
			tc   = &RoQ_loc_TransportConnection[nr];
		} else {
			tc   = &RoQ_rem_TransportConnection[nr];
		}
		tc->mForwardFlags       = flags;
		tc->mForwardHintBits    = hint;
		tc->mForwardFifoId      = Forwarder_getFifoId();

		tc->mForwardIntermediate.Destination.Destination = (((fw.A & 0x3F) << 24) |
															((fw.B & 0x3F) << 18) |
															((fw.C & 0x3F) << 12) |
															((fw.D & 0x3F) <<  6) |
															((fw.E & 0x3F)      ));

		int a,j;
		for(a = 0; a < 2; a++) { // NO ACKS. set "a < 2" for ACKS!!!
			MUHWI_Descriptor_Aligned_t *blup = (a == 0) ? tc->mTX_BGQ_MU_DescriptorArray : tc->mTX_BGQ_MU_AckDescriptor;
					
			int cnt = (a == 0) ? RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT : RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT;
			for (j = 0; j < cnt; j++) {
				MUHWI_Descriptor_Aligned_t *MUHWI_DescPtr = &blup[j];
						
				// get to the SwHdr bytes in the packet header and set up the fields that don't change
				MUHWI_PacketHeader_t *PktHdr = &(MUHWI_DescPtr->PacketHeader);
						
				/* Set pointer to message unit header within the packet header. */
				MUHWI_Pt2PtNetworkHeader_t *Pt2PtHdr = &(PktHdr->NetworkHeader.pt2pt);
				
				// Set intermediate node as destination node
				if (tc->mForwardFlags & 0x1)
					Pt2PtHdr->Destination = tc->mForwardIntermediate;
				else
					Pt2PtHdr->Destination = tc->PeerNodeAddr;

				// Set hint bits
				if (tc->mForwardFlags & 0x2) {
					Pt2PtHdr->Hints              = (tc->mForwardHintBits >> 2);
					// WARNING! Plus and Minus seem to be mixed up here! Maybe a bug in the driver, not here!
					Pt2PtHdr->Byte2.Hint_E_plus  = (tc->mForwardHintBits >> 1) & 0x1;
					Pt2PtHdr->Byte2.Hint_E_minus = (tc->mForwardHintBits     ) & 0x1;
				} else {
					Pt2PtHdr->Hints              = 0;
					Pt2PtHdr->Byte2.Hint_E_minus = 0;
					Pt2PtHdr->Byte2.Hint_E_plus  = 0;
				}
			}
		}
	}

	// Clear forwading target cache
	wakeupThread(ROQ_FORWARDER_THREAD);

	// Resend rolling window buffer or some packets might get stuck there forever in case
	// the ACK to it got lost.		
	for (r = 0; r < 1; r++) {	
		struct RoQ_TransportConnection *tc = NULL;

		if (r == 0) {
			tc   = &RoQ_loc_TransportConnection[nr];
		} else {
			tc   = &RoQ_rem_TransportConnection[nr];
		}

		if (tc->mBurstSeqNo_NextToSend-tc->mBurstSeqNo_NextExpectedAck != 0) {
			printf("retransmitting %d packets to node rank %d\n", tc->mBurstSeqNo_NextToSend-tc->mBurstSeqNo_NextExpectedAck, nr);
			int b;
			for (b = (int)tc->mBurstSeqNo_NextExpectedAck; b < (int)tc->mBurstSeqNo_NextToSend; b++) {
				tc_ReTX_Burst(tc, b & RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK);
			}
		}
	}
}

/* - n     : destination node
*/
void Router_setCutOff(dim_t n)
{
	struct sockaddr_in in_addr;
	uint32_t rank;

	memset(&in_addr, 0, sizeof in_addr);

	RoQ_TorusCoords_To_Rank(&netdesc, n.A, n.B, n.C, n.D, n.E, &rank);
	RoQ_Rank_To_IPV4Addr(&netdesc, rank, &in_addr.sin_addr.s_addr);

	printf("Node (%d,%d,%d,%d,%d) has been cut-off from the network. It cannot be reached at all anymore!\n",
		   n.A, n.B, n.C, n.D, n.E);

	// TODO: Send RAS event to the control system about the failed node.
	// TODO: Notify RoQ to close connections to this node. Otherwise applications
	//       (e.g. GPFS) may wait forever for an answer that will never come. And
	//       that's just tragic.
	
	roq_post_net_event(NET_EVENT_HOSTUNREACHABLE, &in_addr, 0);
}


void Router_printBadLinkList() {
	int i = 0;
	for (i = 0; i < gbl_badLinksNew.head; i++) { // double entries are most likely at the end of the list.
		dim_t l = gbl_badLinksNew.list[i];

		uint8_t d = lnk_getDimension(l);
		
		dim_t   n0, n1;
		lnk_getEndpoints(l, &n0, &n1);
		
		uint8_t l0, l1;
		l0 = (d << 1) + 1; // plus
		l1 = (d << 1);     // minus
		
		INFO("Known bad link #%d : (%d,%d,%d,%d,%d) = (%d,%d,%d,%d,%d).%s <--> %s.(%d,%d,%d,%d,%d)\n", i,
			 l.A, l.B, l.C, l.D, l.E,
			 n0.A, n0.B, n0.C, n0.D, n0.E,
			 linkNum2Str[l0], linkNum2Str[l1],
			 n1.A, n1.B, n1.C, n1.D, n1.E);
	}
	fflush(stdout);
}


void Router_printRoutes() {
	DPRINTF(1, "Failover Routes for node (%d,%d,%d,%d,%d) / {%d,%d,%d,%d,%d}\n",
			torus.local.log.A,
			torus.local.log.B,
			torus.local.log.C,
			torus.local.log.D,
			torus.local.log.E,
			torus.local.phy.A,
			torus.local.phy.B,
			torus.local.phy.C,
			torus.local.phy.D,
			torus.local.phy.E);
	int a,j,i;
	for (i = 0; i < RoQ_NetworkSize(&RoQ_NetworkDesc); i++) {
		struct RoQ_TransportConnection *tc  = &RoQ_loc_TransportConnection[i];
			
		MUHWI_Descriptor_Aligned_t *desc = (MUHWI_Descriptor_Aligned_t *)roq_us_map->mu_burstdesc[i];
		MUHWI_Destination_t        *dest = &desc->PacketHeader.NetworkHeader.pt2pt.Destination;
		
		if (tc->mForwardFlags != 0) {
			DPRINTF(1, "Packets to (%u,%u,%u,%u,%u) settings: fw=(%u,%u,%u,%u,%u), hint=0x%03x, flags=0x%x",
					tc->PeerNodeAddr.Destination.A_Destination,
					tc->PeerNodeAddr.Destination.B_Destination,
					tc->PeerNodeAddr.Destination.C_Destination,
					tc->PeerNodeAddr.Destination.D_Destination,
					tc->PeerNodeAddr.Destination.E_Destination,
					dest->Destination.A_Destination,
					dest->Destination.B_Destination,
					dest->Destination.C_Destination,
					dest->Destination.D_Destination,
					dest->Destination.E_Destination,
					tc->mForwardHintBits,
					tc->mForwardFlags);
		}
	}
}


static int Router_setup() {
  int rc = 0;
  
  uint64_t dcrval;

  // Supernode Setup
  
  // 1. Set number of dimensions
  torus_setDimensions(global.dimensions);


  // 2. Set routing dimension order. This needs to be set before everything else!
  dim_t torus_rdo;

  dcrval = DCRReadPriv(ND_500_DCR(CTRL_DET_ORDER));

  torus_rdo.A = unary2Int(ND_500_DCR__CTRL_DET_ORDER__MASK0_get(dcrval));
  torus_rdo.B = unary2Int(ND_500_DCR__CTRL_DET_ORDER__MASK1_get(dcrval));
  torus_rdo.C = unary2Int(ND_500_DCR__CTRL_DET_ORDER__MASK2_get(dcrval));
  torus_rdo.D = unary2Int(ND_500_DCR__CTRL_DET_ORDER__MASK3_get(dcrval));
  torus_rdo.E = unary2Int(ND_500_DCR__CTRL_DET_ORDER__MASK4_get(dcrval));
  
  torus_setDimensionOrder(torus_rdo);


  // 3. Set size of each dimension of the torus.
  dim_t torus_size;
 
  dcrval = DCRReadPriv(ND_500_DCR(CTRL_COORDS));

  torus_size.A = ND_500_DCR__CTRL_COORDS__MAX_COORD_A_get(dcrval)+1;
  torus_size.B = ND_500_DCR__CTRL_COORDS__MAX_COORD_B_get(dcrval)+1;
  torus_size.C = ND_500_DCR__CTRL_COORDS__MAX_COORD_C_get(dcrval)+1;
  torus_size.D = ND_500_DCR__CTRL_COORDS__MAX_COORD_D_get(dcrval)+1;
  torus_size.E = ND_500_DCR__CTRL_COORDS__MAX_COORD_E_get(dcrval)+1;

  torus_setSize(torus_size);

  // 3. Set torus dimensions (vs. mesh dimensions)
  uint64_t torus_closed;

  dcrval = DCRReadPriv(ND_500_DCR(CTRL_INTERNAL));

  torus_closed = ND_500_DCR__CTRL_INTERNAL__IS_TORUS_get(dcrval);

  torus_setTorus(torus_closed);


  // 4. Set offset. Needs to be set before setting the local node!
  dim_t torus_offset;

  dcrval = DCRReadPriv(ND_500_DCR(CTRL_CUTOFFS));
/*
    logical coordinate = max coord - cutoff high + physical coordinate - cutoff low
                       = (max coord + physical coordinate) - (cutoff high + cutoff low)

					   = physical coordinate + (max coord - cutoff high - cutoff low)

   --> offset = max coord - (cutoff high + cutoff low)
 */
  torus_offset.A = torus_size.A-1 - (ND_500_DCR__CTRL_CUTOFFS__A_PLUS_get(dcrval) +
									 ND_500_DCR__CTRL_CUTOFFS__A_MINUS_get(dcrval));
  torus_offset.B = torus_size.B-1 - (ND_500_DCR__CTRL_CUTOFFS__B_PLUS_get(dcrval) +
									 ND_500_DCR__CTRL_CUTOFFS__B_MINUS_get(dcrval));
  torus_offset.C = torus_size.C-1 - (ND_500_DCR__CTRL_CUTOFFS__C_PLUS_get(dcrval) +
									 ND_500_DCR__CTRL_CUTOFFS__C_MINUS_get(dcrval));
  torus_offset.D = torus_size.D-1 - (ND_500_DCR__CTRL_CUTOFFS__D_PLUS_get(dcrval) +
									 ND_500_DCR__CTRL_CUTOFFS__D_MINUS_get(dcrval));
  torus_offset.E = torus_size.E-1 - (ND_500_DCR__CTRL_CUTOFFS__E_PLUS_get(dcrval) +
									 ND_500_DCR__CTRL_CUTOFFS__E_MINUS_get(dcrval));
  torus_setOffset(torus_offset);


  // 5. Set local node
  dim_t torus_local;

  dcrval = DCRReadPriv(ND_500_DCR(CTRL_COORDS));

  torus_local.A = ND_500_DCR__CTRL_COORDS__NODE_COORD_A_get(dcrval);
  torus_local.B = ND_500_DCR__CTRL_COORDS__NODE_COORD_B_get(dcrval);
  torus_local.C = ND_500_DCR__CTRL_COORDS__NODE_COORD_C_get(dcrval);
  torus_local.D = ND_500_DCR__CTRL_COORDS__NODE_COORD_D_get(dcrval);
  torus_local.E = ND_500_DCR__CTRL_COORDS__NODE_COORD_E_get(dcrval);

  torus_setLocal(torus_local);

  // 6. Initialize supernode and routing functions
  sng_buildSetup();
  sng_routeSetup();
  
  // 7. Just a check.
  sng_print();


  netdesc = (struct RoQ_NetworkDescriptor) {torus_local.A,
											torus_local.B,
											torus_local.C,
											torus_local.D,
											torus_local.E,
											torus_size.A,
											torus_size.B,
											torus_size.C,
											torus_size.D,
											torus.size.E,
											0};


  linkIndex = 0;

  return rc;
}

static void Router_decrementerInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	mtspr(SPRN_TSR, TSR_DIS);  // Ack decrementer interrupt

	/* Interrupts disable the floating point unit but we need that for efficient
	 * memcpy. Reenable it here.
	 */
	mtmsr(mfmsr() | MSR_FP);

	// 2. Recalculate routes
	sng_route();

#ifdef ROUTER_DEBUG
	sng_routeCheck();
	sng_printLatex();
	
	torus_routeCheck();
	torus_printLatex(1);
#endif
	
	// 3. Reset route calculation data structures for next time.
	sng_routeReset();
}

static void Router_externalInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	mtmsr(mfmsr() | MSR_FP); // re-enable FPU for fast memcopy

	// 0. Clear BIC external interrupts
	unsigned int tid = ProcessorID();

	while (BIC_ReadExternalIntSummary(tid) != 0) {
		BIC_REGISTER reg0 = BIC_ReadStatusExternalRegister0(tid);
		BIC_REGISTER reg1 = BIC_ReadStatusExternalRegister1(tid);
		BIC_REGISTER sum  = BIC_ReadExternalIntSummary(tid);
		
		if (reg0 != 0) BIC_WriteClearExternalRegister0(tid, reg0);
		if (reg1 != 0) BIC_WriteClearExternalRegister1(tid, reg1);
		
		// 1. Update supernode graph
		while(linkIndex < gbl_badLinksNew.head) {
			dim_t link = gbl_badLinksNew.list[linkIndex];
			DPRINTF(ROUTER_DEBUG, "Injecting failure %d/%d = (%d,%d,%d,%d,%d)\n", linkIndex, gbl_badLinksNew.head-1, link.A, link.B, link.C, link.D, link.E);
			linkIndex++;
			
			Router_injectFault(link);
		}
	}

	// Set timer to update routes.
	uint32_t ticks = config.router.wait*global.freqMhz;
	enableDecrementerInterrupt(ticks, ticks, 0);
}


void Router() {
	int rc = 0;
	rc = Router_setup();
	if (rc) {
		ERROR("RoQ Resiliency Router Setup failed. Cannot start router thread.\n");
		return;
	}
	
	// Allow all cores to sleep
	mtspr(SPRN_CCR0, CCR0_WEM(0xF));
	
	setInterruptHandler(Router_decrementerInterruptHandler, IVO_DEC);
	setInterruptHandler(Router_externalInterruptHandler, IVO_EI);
	
	asm volatile ("wrteei 1"); // set external enable bit
	
	while (1) {
		asm volatile ("wait" : : : "memory" );
	}
}
