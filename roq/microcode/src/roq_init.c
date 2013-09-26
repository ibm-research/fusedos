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

#if defined(USERSPACE_ROQ)
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <hwi/include/bqc/nd_500_dcr.h>

union cn_bridge_t {
	uint64_t value;
	uint8_t v[8];
	struct {
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
		uint8_t e;
		uint8_t unused;
		uint16_t rank;
	} cn_bridge;
};
struct net_info_exchange_t {
	struct RoQ_NetworkDescriptor RoQ_NetworkDesc;
	union cn_bridge_t cnb;
};
struct net_info_exchange_world_t {
	struct RoQ_NetworkDescriptor RoQ_NetworkDesc;
	union cn_bridge_t cnb_link1[8];
	union cn_bridge_t cnb_link2[8];
};
static struct net_info_exchange_world_t net_info_exchange_world;

#ifdef USERSPACE_ROQ
void sighandler(int signum)
{
	void *array[10];
	size_t size;
	char **strings;
	int i;

	printf("ROQ Caught signal = %d\n",signum); fflush(stdout);

	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);

	for (i = 0; i < size; i++)
		printf("%s\n", strings[i]);

	fflush(stdout);

	DASSERT(0, "%s %lx\n", "ROQ Caught signal =", (uint64_t)signum);

	sync();

	exit(0);
}

void atexithandler(void)
{
	printf("ROQ Exit\n");

	DASSERT(0, "%s\n", "ROQ Exit");

	fflush(stdout);
}
#endif /* USERSPACE_ROQ */

static void
roq_init_remote(int remote_cnt)
{
	int i;
	roq_us_map->rem.trans_idx = 0;

	for (i = 0; i < remote_cnt; ++i)
		tc_init_remote(&RoQ_rem_TransportConnection[i], RoQ_LocalRank, i, &RoQ_TX_Args[0], &RoQ_RX_Args[0], &RoQ_NetworkDesc_rem, personality);

	return;
}

extern uint8_t hw_thread; // defined in roq_microcode/entry.c
void display_mu_registers(void)
  {
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MOD_HALTED)",DCRReadPriv(MU_DCR(MOD_HALTED))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MCSRAM_BD_FETCHING_DESC)",DCRReadPriv(MU_DCR(MCSRAM_BD_FETCHING_DESC))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(IME_STATE)",DCRReadPriv(MU_DCR(IME_STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(RME_STATE0)",DCRReadPriv(MU_DCR(RME_STATE0))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(RME_STATE1)",DCRReadPriv(MU_DCR(RME_STATE1))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(INJ_NDFIFO_AVAIL)",DCRReadPriv(MU_DCR(INJ_NDFIFO_AVAIL))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MASTER_PORT0_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT0_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MASTER_PORT1_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT1_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MASTER_PORT2_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT2_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(SLAVE_PORT_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(SLAVE_PORT_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(XS_ERR_INFO)",DCRReadPriv(MU_DCR(XS_ERR_INFO))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MMREGS_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MMREGS_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MCSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(MCSRAM_ERR_FIFO_ID)",DCRReadPriv(MU_DCR(MCSRAM_ERR_FIFO_ID))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(RCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RCSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(RMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(RMFIFO_ACCESS_ERROR_ID))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(RPUTSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RPUTSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(ICSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(ICSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(IMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(IMFIFO_ACCESS_ERROR_ID))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(XM0_ERR_INFO)",DCRReadPriv(MU_DCR(XM0_ERR_INFO))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(XM1_ERR_INFO)",DCRReadPriv(MU_DCR(XM1_ERR_INFO))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","MU_DCR(XM2_ERR_INFO)",DCRReadPriv(MU_DCR(XM2_ERR_INFO))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","ND_RESE_DCR(10,RESET)",DCRReadPriv(ND_RESE_DCR(10,RESET))) ;
    DPRINTF(FXLOG_UCODE_INIT, "%s 0x%016lx\n","ND_RESE_DCR(10,CTRL)",DCRReadPriv(ND_RESE_DCR(10,CTRL))) ;


  }

inline unsigned int min(unsigned int a, unsigned int b) {
    return a<b ? a : b;
}

inline unsigned int dist_to_myself(union cn_bridge_t cnb) {

    unsigned int dist = 0;

    // what would I give for a class TorusCoordinate !
    // (and C++ fancy / ugly operator overloading)
#define DIST_COORD(x,X) ( min( cnb.cn_bridge.x - personality->Network_Config.X##coord, \
	    personality->Network_Config.X##coord - cnb.cn_bridge.x ) )
    dist += DIST_COORD(a,A);
    dist += DIST_COORD(b,B);
    dist += DIST_COORD(c,C);
    dist += DIST_COORD(d,D);
    dist += DIST_COORD(e,E);
    return dist;
#undef DIST_COORD
}

/*********************************************
 * Discovery Process
 *
 * 1) RANK0 puts all CN nodes into discovery mode
 * 2) All CN Bridges exchange network information
 *    with the corresponding IO node
 * 3) CN Bridges send their information to RANK0
 * 4) RANK0 collects all information and distributed them
 *    inside the cluster.
 * 5) All nodes reset the send counters and send an interrupt
 *    to the linux driver which executes and script to setup
 *    the host file.
 *********************************************/
void
RoQ_discovery()
{
	int i, rc;
	void *buf;
	struct RoQ_TransportConnection *tc;
	RoQ_QP_Id_t destid;
	struct net_info_exchange_t *nie;
	struct net_info_exchange_world_t *niew;
	uint64_t iorank, iosize;
	uint32_t valid_cnt = 0;

	DPRINTF(FXLOG_UCODE_INIT, "Discovery starting\n") ;

	destid.mIndex = 1;
	RoQ_discovery_act = 1;

	bzero(&net_info_exchange_world, sizeof(net_info_exchange_world));

	DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
	/* IO NODE */
	if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) {

		while (1) {
		  DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			/* wait till CN comes up and need the cnBridge/io-rank mapping */
			while ((rc = ROQ_getnext_packet(1, RoQ_RX_Args[0].rem_recfifo_ptr, &buf, 1)) == 0) ;
		  DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);

			/* skip discovery start packets */
			if (rc == 64 && ((uint64_t*)(((char*)buf) + 32))[0] == ROQ_DISCOVERY_MAGIC) {
				continue;
			}

			nie = (struct net_info_exchange_t *)(((char*)buf) + 32);
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d nie->cnb.value=%ld valid_cnt=%d\n",__LINE__,nie->cnb.value,valid_cnt);
			if (nie->cnb.value == (uint64_t)-1 || nie->cnb.cn_bridge.rank == (uint16_t)16384) {
				valid_cnt--;
        DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d valid_cnt=%d\n",__LINE__,valid_cnt);
				if (valid_cnt <= 0)
				  {
			      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
					  break;
				  }
	      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				continue;
			}
			valid_cnt++;

			memcpy(&RoQ_NetworkDesc_rem, &nie->RoQ_NetworkDesc, sizeof(struct RoQ_NetworkDescriptor));
			#if 1
			DPRINTF(FXLOG_UCODE_INIT, "<IO-%d> cn_bridge .a=%d .b=%d .c=%d .d=%d .e=%d rank=%d, size=%d, ioport=%d\n",
			       RoQ_LocalRank, nie->cnb.cn_bridge.a, nie->cnb.cn_bridge.b, nie->cnb.cn_bridge.c,
			       nie->cnb.cn_bridge.d, nie->cnb.cn_bridge.e, nie->cnb.cn_bridge.rank,
			       RoQ_NetworkSize(&RoQ_NetworkDesc_rem), RoQ_NetworkDesc_rem.IOport);
			DPRINTF(FXLOG_UCODE_INIT, "<IO-%d> Nodes: A: %d, B: %d, C: %d, D: %d, E: %d\n", RoQ_LocalRank,
				  nie->RoQ_NetworkDesc.Anodes, nie->RoQ_NetworkDesc.Bnodes,
				  nie->RoQ_NetworkDesc.Cnodes, nie->RoQ_NetworkDesc.Dnodes,
				  nie->RoQ_NetworkDesc.Enodes);
			DPRINTF(FXLOG_UCODE_INIT, "<IO-%d> COORD: A: %d, B: %d, C: %d, D: %d, E: %d\n", RoQ_LocalRank,
				  nie->RoQ_NetworkDesc.Acoord, nie->RoQ_NetworkDesc.Bcoord,
				  nie->RoQ_NetworkDesc.Ccoord, nie->RoQ_NetworkDesc.Dcoord,
				  nie->RoQ_NetworkDesc.Ecoord);
			#endif

			RoQ_DevMem->mNetworkDesc_rem = RoQ_NetworkDesc_rem;
			RoQ_NetworkDesc_rem          = RoQ_NetworkDesc_rem;

      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			roq_init_remote(RoQ_NetworkSize(&RoQ_NetworkDesc_rem));
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			rc = RoQ_NetworkSize(&RoQ_NetworkDesc);
//			for (i=0; i < RoQ_NetworkSize(&RoQ_NetworkDesc_rem); i++)
//				bzero(&RoQ_DevicePrivatePerQPState[rc+i], sizeof(struct RoQ_DevicePrivatePerQPState));

			/* send IO Cluster RANK to CN Cluster */
			tc = &RoQ_rem_TransportConnection[nie->cnb.cn_bridge.rank];
			nie = (struct net_info_exchange_t *)&roq_us_map->dmabuf[0];
			memcpy(&nie->RoQ_NetworkDesc, &RoQ_NetworkDesc, sizeof(struct RoQ_NetworkDescriptor));
			nie->cnb.value           = 0;
			nie->cnb.cn_bridge.rank  = RoQ_LocalRank;
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], sizeof(struct net_info_exchange_t));
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
		}
		for (i=0; i < RoQ_NetworkSize(&RoQ_NetworkDesc_rem); i++) {
			RoQ_rem_TransportConnection[i].mExpectedHardwarePutOffset.mWhole = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextToSend = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextExpectedAck = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextToRecv = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_LastAcked = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_ReadyToAck = -1 ;
		}
	} else {

		DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);

		roq_init_remote(1) ;
		/* SEND DISCOVERY REQUEST */
		if (RoQ_LocalRank == 0) {
			DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d roq_us_map=%p\n",__LINE__,roq_us_map);
			DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d roq_us_map->dmabuf=%p\n",__LINE__,roq_us_map->dmabuf);
			((uint64_t*)&roq_us_map->dmabuf[0])[0] = ROQ_DISCOVERY_MAGIC;
			DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d RoQ_NetworkSize(&RoQ_NetworkDesc)=%d\n",__LINE__,RoQ_NetworkSize(&RoQ_NetworkDesc));
			for (i=1; i < RoQ_NetworkSize(&RoQ_NetworkDesc); i++) {
				DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				tc = &RoQ_loc_TransportConnection[i];
				tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], 8);
			}
			DPRINTF(FXLOG_UCODE_INIT, "Discovery: sent requests\n") ;
		}
		DPRINTF(FXLOG_UCODE_INIT, "Discovery: CN waypoint\n") ;

		/* CN NODE */
		if (personality->Network_Config.cnBridge_A == personality->Network_Config.Acoord
		 && personality->Network_Config.cnBridge_B == personality->Network_Config.Bcoord
		 && personality->Network_Config.cnBridge_C == personality->Network_Config.Ccoord
		 && personality->Network_Config.cnBridge_D == personality->Network_Config.Dcoord
		 && personality->Network_Config.cnBridge_E == personality->Network_Config.Ecoord) {

			DPRINTF(FXLOG_UCODE_INIT, "Discovery: This is bridge node\n") ;
      display_mu_registers() ;
			/* SEND DISCOVERY REQUEST */
			((uint64_t*)&roq_us_map->dmabuf[0])[0] = ROQ_DISCOVERY_MAGIC;
			tc = &RoQ_rem_TransportConnection[0];
			tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], 8);
			tc->mBurstSeqNo_NextToSend = 0;
			tc->mExpectedHardwarePutOffset.mBurstSeqNo = 0;
			tc->mExpectedHardwarePutOffset.mByteOffset = 0;

			display_mu_registers() ;
			/* set information */
			nie = (struct net_info_exchange_t *)&roq_us_map->dmabuf[0];
			memcpy(&nie->RoQ_NetworkDesc, &RoQ_NetworkDesc, sizeof(struct RoQ_NetworkDescriptor));
			nie->cnb.cn_bridge.a    = personality->Network_Config.cnBridge_A;
			nie->cnb.cn_bridge.b    = personality->Network_Config.cnBridge_B;
			nie->cnb.cn_bridge.c    = personality->Network_Config.cnBridge_C;
			nie->cnb.cn_bridge.d    = personality->Network_Config.cnBridge_D;
			nie->cnb.cn_bridge.e    = personality->Network_Config.cnBridge_E;
			nie->cnb.cn_bridge.rank = RoQ_LocalRank;

			/* SEND CN CLUSTER INFO */
			tc = &RoQ_rem_TransportConnection[0];
			tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], sizeof(struct net_info_exchange_t));

			/* WAIT FOR IO NODE RANK */
			DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			while ((rc = ROQ_getnext_packet(1, RoQ_RX_Args[0].rem_recfifo_ptr, &buf, 1)) == 0) ;

			DPRINTF(FXLOG_UCODE_INIT, "Discovery: have IO node rank\n") ;
			nie = (struct net_info_exchange_t *)(((char*)buf) + 32);
			memcpy(&RoQ_NetworkDesc_rem, &nie->RoQ_NetworkDesc, sizeof(struct RoQ_NetworkDescriptor));
			iorank = nie->cnb.cn_bridge.rank;
			iosize = RoQ_NetworkSize(&RoQ_NetworkDesc_rem);

			/* SEND IO INFO TO RANK0 */
			nie = (struct net_info_exchange_t *)&roq_us_map->dmabuf[0];
			memcpy(&nie->RoQ_NetworkDesc, &RoQ_NetworkDesc_rem, sizeof(struct RoQ_NetworkDescriptor));
			nie->cnb.cn_bridge.a = personality->Network_Config.cnBridge_A;
			nie->cnb.cn_bridge.b = personality->Network_Config.cnBridge_B;
			nie->cnb.cn_bridge.c = personality->Network_Config.cnBridge_C;
			nie->cnb.cn_bridge.d = personality->Network_Config.cnBridge_D;
			nie->cnb.cn_bridge.e = personality->Network_Config.cnBridge_E;
			nie->cnb.cn_bridge.rank = iorank;
			tc = &RoQ_loc_TransportConnection[0];
			tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], sizeof(struct net_info_exchange_t));
		}

    DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
		if (RoQ_LocalRank == 0) {
			int first_slot_full[8];
			i=0;
			for(i=0; i < 8; i++)
			    first_slot_full[i] = 0;
			while (1) {
				/* wait for cn bridge info */
				while ((rc = ROQ_getnext_packet(0, RoQ_RX_Args[0].recfifo_ptr, &buf, 0)) == 0) ;
	      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				if (((uint64_t *)(((char *)buf) + 32))[0] == ROQ_DISCOVERY_MAGIC)
					break;
				i++;
				/* extract information here */
				nie = (struct net_info_exchange_t *)(((char*)buf) + 32);

				memcpy(&net_info_exchange_world.RoQ_NetworkDesc, &nie->RoQ_NetworkDesc, sizeof(struct RoQ_NetworkDescriptor));
	
				// store in 1st or 2nd slot of IO link
				// info for that IO rank
				if( ! first_slot_full[nie->cnb.cn_bridge.rank] ) {
				    net_info_exchange_world.cnb_link1[nie->cnb.cn_bridge.rank].value = nie->cnb.value;
				    first_slot_full[nie->cnb.cn_bridge.rank] = 1;
				    // hijack unused field to indicate
				    // that the information is present
				    net_info_exchange_world.cnb_link1[nie->cnb.cn_bridge.rank].cn_bridge.unused = 1;
				} else {
				    net_info_exchange_world.cnb_link2[nie->cnb.cn_bridge.rank].value = nie->cnb.value;
				    net_info_exchange_world.cnb_link2[nie->cnb.cn_bridge.rank].cn_bridge.unused = 1;
				}
			}
			DPRINTF(FXLOG_UCODE_INIT, "Discovery: sending info to other nodes\n") ;

			/* send infos to all other nodes */
			memcpy(&roq_us_map->dmabuf[0], &net_info_exchange_world, sizeof(net_info_exchange_world));
			for (i=1; i < RoQ_NetworkSize(&RoQ_NetworkDesc); i++) {
				tc = &RoQ_loc_TransportConnection[i];
	      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], sizeof(net_info_exchange_world));
			}
		} else {
			/* SEND FINISHED BROADCAST */
			((uint64_t*)&roq_us_map->dmabuf[0])[0] = ROQ_DISCOVERY_MAGIC;
			if (RoQ_NetworkSize(&RoQ_NetworkDesc)-1  == RoQ_LocalRank) {
				tc = &RoQ_loc_TransportConnection[RoQ_LocalRank-1];
	      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], 8);
			} else {
				while ((rc = ROQ_getnext_packet(0, RoQ_RX_Args[0].recfifo_ptr, &buf, 0)) == 0) ;
				tc = &RoQ_loc_TransportConnection[RoQ_LocalRank-1];
	      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
				tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], 8);
			}

      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			/* wait till CN RANK0 sends the information packet */
			while ((rc = ROQ_getnext_packet(0, RoQ_RX_Args[0].recfifo_ptr, &buf, 0)) == 0) ;
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);

			/* BUF CONTAINS ALL CN BRIDGES!!!! */
			niew = (struct net_info_exchange_world_t*)(((char*)buf) + 32);
			memcpy(&net_info_exchange_world, niew, sizeof(net_info_exchange_world));
		}

		/* FINISH NOTIFICATION (IO NODES) */
		if (personality->Network_Config.cnBridge_A == personality->Network_Config.Acoord
		 && personality->Network_Config.cnBridge_B == personality->Network_Config.Bcoord
		 && personality->Network_Config.cnBridge_C == personality->Network_Config.Ccoord
		 && personality->Network_Config.cnBridge_D == personality->Network_Config.Dcoord
		 && personality->Network_Config.cnBridge_E == personality->Network_Config.Ecoord) {
			/* set information */
			nie = (struct net_info_exchange_t *)&roq_us_map->dmabuf[0];
			nie->cnb.value = -1;
			/* SEND CN CLUSTER INFO */
			tc = &RoQ_rem_TransportConnection[0];
      DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
			tc_TX_Burst(tc, tc->mBurstSeqNo_NextToSend, destid, 0, (uint64_t)&roq_us_map->dmabuf[0], sizeof(struct net_info_exchange_t));
		}

    DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
		/* init remote */
		uint32_t tmp[5];
		tmp[0] = personality->Network_Config.cnBridge_A;
		tmp[1] = personality->Network_Config.cnBridge_B;
		tmp[2] = personality->Network_Config.cnBridge_C;
		tmp[3] = personality->Network_Config.cnBridge_D;
		tmp[4] = personality->Network_Config.cnBridge_E;
		roq_us_map->rem.trans_idx = 0;
		rc = RoQ_NetworkSize(&RoQ_NetworkDesc);
		for (i=0; i < RoQ_NetworkSize(&net_info_exchange_world.RoQ_NetworkDesc); i++) {
			// for each IO rank, decide if we use the first
			// or the second IO link information
			if( net_info_exchange_world.cnb_link2[i].cn_bridge.unused // info present
				&& dist_to_myself(net_info_exchange_world.cnb_link2[i])
				<  dist_to_myself(net_info_exchange_world.cnb_link1[i])) {
			    // use second link info
			    personality->Network_Config.cnBridge_A = net_info_exchange_world.cnb_link2[i].cn_bridge.a;
			    personality->Network_Config.cnBridge_B = net_info_exchange_world.cnb_link2[i].cn_bridge.b;
			    personality->Network_Config.cnBridge_C = net_info_exchange_world.cnb_link2[i].cn_bridge.c;
			    personality->Network_Config.cnBridge_D = net_info_exchange_world.cnb_link2[i].cn_bridge.d;
			    personality->Network_Config.cnBridge_E = net_info_exchange_world.cnb_link2[i].cn_bridge.e;
			} else { // use first link info
			    personality->Network_Config.cnBridge_A = net_info_exchange_world.cnb_link1[i].cn_bridge.a;
			    personality->Network_Config.cnBridge_B = net_info_exchange_world.cnb_link1[i].cn_bridge.b;
			    personality->Network_Config.cnBridge_C = net_info_exchange_world.cnb_link1[i].cn_bridge.c;
			    personality->Network_Config.cnBridge_D = net_info_exchange_world.cnb_link1[i].cn_bridge.d;
			    personality->Network_Config.cnBridge_E = net_info_exchange_world.cnb_link1[i].cn_bridge.e;
			}
			tc_init_remote(&RoQ_rem_TransportConnection[i], RoQ_LocalRank, i, &RoQ_TX_Args[0], &RoQ_RX_Args[0], &net_info_exchange_world.RoQ_NetworkDesc, personality);
			RoQ_rem_TransportConnection[i].mExpectedHardwarePutOffset.mWhole = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextToSend = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextExpectedAck = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_NextToRecv = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_LastAcked = 0;
			RoQ_rem_TransportConnection[i].mBurstSeqNo_ReadyToAck = -1 ;
			bzero(&RoQ_DevicePrivatePerQPState[rc+i], sizeof(struct RoQ_DevicePrivatePerQPState));
		}
		personality->Network_Config.cnBridge_A = tmp[0];
		personality->Network_Config.cnBridge_B = tmp[1];
		personality->Network_Config.cnBridge_C = tmp[2];
		personality->Network_Config.cnBridge_D = tmp[3];
		personality->Network_Config.cnBridge_E = tmp[4];
		RoQ_DevMem->mNetworkDesc_rem = net_info_exchange_world.RoQ_NetworkDesc;
		RoQ_NetworkDesc_rem = net_info_exchange_world.RoQ_NetworkDesc;
	}
	for (i=0; i < RoQ_NetworkSize(&RoQ_NetworkDesc); i++) {
			RoQ_loc_TransportConnection[i].mExpectedHardwarePutOffset.mWhole = 0;
			RoQ_loc_TransportConnection[i].mBurstSeqNo_NextToSend = 0;
			RoQ_loc_TransportConnection[i].mBurstSeqNo_NextExpectedAck = 0;
			RoQ_loc_TransportConnection[i].mBurstSeqNo_NextToRecv = 0;
			RoQ_loc_TransportConnection[i].mBurstSeqNo_LastAcked = 0;
			RoQ_loc_TransportConnection[i].mBurstSeqNo_ReadyToAck = -1;
	}
  DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);

	ROQ_getnext_packet_reset();
  DPRINTF(FXLOG_UCODE_INIT, "Discovery line %d\n",__LINE__);
	
    // Added for FusedOS: determine closest IO node (that we know
    // about! -- discovery discards info about many IO links)
    unsigned int min_dist = personality->Network_Config.Anodes
	+ personality->Network_Config.Bnodes
	+ personality->Network_Config.Cnodes
	+ personality->Network_Config.Dnodes
	+ personality->Network_Config.Enodes;
    uint16_t min_rank = 0;

    if (! (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode) ) {
        for(i=0; i < RoQ_NetworkSize(&net_info_exchange_world.RoQ_NetworkDesc); i++) {
    	// argh, we only have information about (at most) 8 IO links by
    	// default
    	//  FusedOS hack: also store info about 2nd IO link
    	unsigned int dist = 0;
    	unsigned int dist_link1 = 0, dist_link2 = 0;

    	dist_link1 = dist_to_myself(net_info_exchange_world.cnb_link1[i]);
    	dist_link2 = dist_to_myself(net_info_exchange_world.cnb_link2[i]);

    	if(net_info_exchange_world.cnb_link2[i].cn_bridge.unused) {
    	    dist = min( dist_link1, dist_link2 );
    	} else {
    	    dist = dist_link1;
    	}

	DPRINTF(FXLOG_UCODE_INIT, "IO link (rank, coords, dist) :: %d :: %u %u %u %u %u :: %u\n",
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.rank,
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.a,
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.b,
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.c,
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.d,
    	    net_info_exchange_world.cnb_link1[i].cn_bridge.e,
	    dist_link1);

	DPRINTF(FXLOG_UCODE_INIT, "IO link (rank, coords, dist) :: %d :: %u %u %u %u %u :: %u\n",
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.rank,
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.a,
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.b,
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.c,
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.d,
    	    net_info_exchange_world.cnb_link2[i].cn_bridge.e,
	    dist_link2);


    	if( dist < min_dist ) {
    	    min_dist = dist;
    	    min_rank = net_info_exchange_world.cnb_link1[i].cn_bridge.rank;
    	}
    	// try to balance nodes when several IO links have the same distance
    	if( dist == min_dist) {
    	    int ions = RoQ_NetworkSize(&net_info_exchange_world.RoQ_NetworkDesc);
    	    int remote_rank = net_info_exchange_world.cnb_link1[i].cn_bridge.rank;
    	    int local_rank = RoQ_LocalRank;
    	    if( remote_rank % ions == local_rank % ions ) {
    		min_rank = remote_rank;
    	    }
    	}
        }
        DPRINTF(FXLOG_UCODE_INIT, "IO link, chosen rank: %d\n", min_rank);

        RoQ_DevMem->nearest_io_rank = min_rank;
    }

    RoQ_discovery_act = 0;
	ppc_msync();
	RoQ_Post_DM_Event(RoQ_DB_LINK, 0);
	DPRINTF(FXLOG_UCODE_INIT, "DISCOVERY END");
}

//#if defined(TORUS_WRAPPED)
#include <hwi/include/bqc/nd_500_dcr.h>
//#endif /* TORUS_WRAPPED */

void
RoQ_MC_Init()
{
	pthread_t tid[8];
	int rc;

#if defined(DEBUG_BUFFER) && DEBUG_BUFFER == 1
	debug_buffer_init();
#endif

#ifdef ROQ_RESILIENCY
#include <roq_kernel.h>
#endif



#ifdef USERSPACE_ROQ
	struct sigaction new_action;
	new_action.sa_handler = sighandler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;

	for (rc = 1; rc < 33; rc++) {
		if (rc == 6)
			continue;
		sigaction(rc, (const struct sigaction*)&new_action, NULL);
	}
	rc = 0;

	atexit(atexithandler);

	int fd = open("/dev/bgpers", O_TRUNC);
	if (fd == -1) {
		printf("ERROR: open file /dev/bgpers\n");
		return;
	}

	personality = (Personality_t *) mmap(NULL, sizeof(Personality_t), PROT_READ, MAP_SHARED, fd, 0);
	if ((int64_t)personality == -1) {
		printf("ERROR: mmap pers %p\n", personality);
		return;
	}

	int fd1 = open("/dev/roq_usmu", O_RDWR);
	if (fd1 == -1) {
		printf("ERROR: open file /dev/roq_usmu\n");
		return;
	}

	/* map MU area */
	void *mu0 = mmap((void*)(_BGQ_BASE_DMA ^ PHYMAP_PRIVILEGEDOFFSET), _BGQ_SIZE_DMA, PROT_READ |
			PROT_WRITE, MAP_SHARED, fd1, _BGQ_BASE_DMA ^ PHYMAP_PRIVILEGEDOFFSET);
	if ((int64_t)mu0 != (_BGQ_BASE_DMA ^ PHYMAP_PRIVILEGEDOFFSET)) {
		printf("ERROR: mmap mu %p\n", mu0);
		return;
	}

	void *devmem = mmap((void*)(0x23000000), sizeof(struct RoQ_DeviceMemory), PROT_READ |
			PROT_WRITE, MAP_SHARED, fd1, NULL);
	if ((int64_t)devmem != 0x23000000) {
		printf("ERROR: mmap devmem %p\n", devmem);
		return;
	}
	RoQ_DevMem = (struct RoQ_DeviceMemory *)devmem;

	/* open cached */
	int fd2 = open("/dev/mem", O_RDWR);
	if (fd2 == -1) {
		printf("ERROR: open file /dev/mem\n");
		return;
	}
	void *mmmm;
	mmmm = mmap((void*)(ROQ_VIRT_CACHE_OFF), 16 * 0x40000000ULL, PROT_READ |
			PROT_WRITE, MAP_SHARED, fd2, 0);
	if ((int64_t)mmmm != ROQ_VIRT_CACHE_OFF) {
		printf("ERROR: mmap mmmm %p\n", mmmm);
		return;
	}

	/* open uncached */
	int fd3 = open("/dev/mem", O_RDWR | O_DSYNC);
	if (fd3 == -1) {
		printf("ERROR: open file /dev/mem\n");
		return;
	}
	void *uuuu;
	uuuu = mmap((void*)(ROQ_VIRT_UNCACHE_OFF), 16 * 0x40000000ULL, PROT_READ |
			PROT_WRITE, MAP_SHARED, fd3, 0);
	if ((int64_t)uuuu != ROQ_VIRT_UNCACHE_OFF) {
		printf("ERROR: mmap mmmm %p\n", uuuu);
		return;
	}
#else
	Personality_t pers;
	personality = &pers;
	Kernel_GetPersonality(personality, sizeof(Personality_t));
#endif /* USERSPACE_ROQ */

//#if defined(TORUS_WRAPPED)
	uint64_t ctrl_int = DCRReadPriv(ND_500_DCR(CTRL_INTERNAL));

	DPRINTF(FXLOG_UCODE_INIT, "CTRL_INTERNAL read: %016x\n", ctrl_int);

	// DCRWritePriv(ND_500_DCR(CTRL_INTERNAL), 0x1ffdf40000000000);
	
	if( personality->Network_Config.Anodes == 2 && (ctrl_int & 0x0200000000000000L))
	   ctrl_int |= 0x0010000000000000L ;
	if( personality->Network_Config.Bnodes == 2 && (ctrl_int & 0x0100000000000000L))
	   ctrl_int |= 0x0008000000000000L ;
	if( personality->Network_Config.Cnodes == 2 && (ctrl_int & 0x0080000000000000L))
	   ctrl_int |= 0x0004000000000000L ;
	if( personality->Network_Config.Dnodes == 2 && (ctrl_int & 0x0040000000000000L))
	   ctrl_int |= 0x0002000000000000L ;
	if( personality->Network_Config.Enodes == 2 && (ctrl_int & 0x0020000000000000L))
	   ctrl_int |= 0x0001000000000000L ;

	DPRINTF(FXLOG_UCODE_INIT, "CTRL_INTERNAL updated to %016lx\n", ctrl_int) ;

	DCRWritePriv(ND_500_DCR(CTRL_INTERNAL),ctrl_int) ; 

	/* Ben suggested this but it's not working for me */
	/* DCRWritePriv(ND_500_DCR(CTRL_INTERNAL),ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_insert(ctrl_int, 0x1c)); */

	ctrl_int = DCRReadPriv(ND_500_DCR(CTRL_INTERNAL));

	DPRINTF(FXLOG_UCODE_INIT, "CTRL_INTERNAL after set: %016x\n", ctrl_int);

	uint64_t ctrl_cut=DCRReadPriv(ND_500_DCR(CTRL_CUTOFFS)) ;
	DPRINTF(FXLOG_UCODE_INIT, "CTRL_CUTOFFS is %016lx\n",ctrl_cut) ;

	if( personality->Network_Config.Anodes == 2 && (ctrl_int & 0x0200000000000000L))
	{
	  if(personality->Network_Config.Acoord == 0 )
	    ctrl_cut |= 0x0100000000000000L ;
	  else
	    ctrl_cut &= ~0x3f000000 ;
	}
	if( personality->Network_Config.Bnodes == 2 && (ctrl_int & 0x0100000000000000L))
	{
	  if(personality->Network_Config.Bcoord == 0 )
	    ctrl_cut |= 0x0004000000000000L ;
	  else
	    ctrl_cut &= ~0x00fc0000 ;
	}
	if( personality->Network_Config.Cnodes == 2 && (ctrl_int & 0x0080000000000000L))
	{
	  if(personality->Network_Config.Ccoord == 0 )
	    ctrl_cut |= 0x0000100000000000L ;
	  else
	    ctrl_cut &= ~0x0003f000 ;
	}
	if( personality->Network_Config.Dnodes == 2 && (ctrl_int & 0x0040000000000000L))
	{
	  if(personality->Network_Config.Dcoord == 0 )
	    ctrl_cut |= 0x0000004000000000L ;
	  else
	    ctrl_cut &= ~0x00000fc0 ;
	}
	if( personality->Network_Config.Enodes == 2 && (ctrl_int & 0x0020000000000000L))
	{
	  if(personality->Network_Config.Ecoord == 0 )
	    ctrl_cut |= 0x0000000100000000L ;
	  else
	    ctrl_cut &= ~0x0000003f ;
	}
	DPRINTF(FXLOG_UCODE_INIT, "Updating CTRL_CUTOFFS to %016lx\n", ctrl_cut) ;
	DCRWritePriv(ND_500_DCR(CTRL_CUTOFFS),ctrl_cut) ; 
	
#if 0
// Make the torus behave as a mesh
	{
	  uint64_t dcr_addr=ND_500_DCR(CTRL_INTERNAL) ;
	  uint64_t dcr_val=DCRReadPriv(dcr_addr) ; 
	  ND_500_DCR__CTRL_INTERNAL__IS_TORUS_insert(dcr_val,0) ;
	  DCRWritePriv(dcr_addr,dcr_val) ; 
	}
	{
	  uint64_t dcr_addr=ND_500_DCR(CTRL_CUTOFFS) ;
	  uint64_t dcr_val=(((uint64_t)personality->Network_Config.Anodes-1) << (32+6*4))
			|  (((uint64_t)personality->Network_Config.Bnodes-1) << (32+6*3))
			|  (((uint64_t)personality->Network_Config.Cnodes-1) << (32+6*2))
			|  (((uint64_t)personality->Network_Config.Dnodes-1) << (32+6*1))
			|  (((uint64_t)personality->Network_Config.Enodes-1) << (32+6*0)) ;
	 DCRWritePriv(dcr_addr,dcr_val) ; 
	}
#endif
//	fflush(stdout);
//#endif /* TORUS_WRAPPED */

	DPRINTF(0, "Zero'ing RoQ_DevMem", NULL);

	bzero( RoQ_DevMem, sizeof(*RoQ_DevMem) );

	RoQ_NetworkDesc.Acoord = personality->Network_Config.Acoord;
	RoQ_NetworkDesc.Bcoord = personality->Network_Config.Bcoord;
	RoQ_NetworkDesc.Ccoord = personality->Network_Config.Ccoord;
	RoQ_NetworkDesc.Dcoord = personality->Network_Config.Dcoord;
	RoQ_NetworkDesc.Ecoord = personality->Network_Config.Ecoord;
	RoQ_NetworkDesc.Anodes = personality->Network_Config.Anodes;
	RoQ_NetworkDesc.Bnodes = personality->Network_Config.Bnodes;
	RoQ_NetworkDesc.Cnodes = personality->Network_Config.Cnodes;
	RoQ_NetworkDesc.Dnodes = personality->Network_Config.Dnodes;
	RoQ_NetworkDesc.Enodes = personality->Network_Config.Enodes;

	if (personality->Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_6)
		RoQ_NetworkDesc.IOport = 6;

	if (personality->Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_7)
		RoQ_NetworkDesc.IOport = 7;

	if (personality->Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_10)
		RoQ_NetworkDesc.IOport = 10;

#if 0
	printf("cnBridge: A: %d, B: %d, C: %d, D: %d, E: %d (port: %d)\n",
		personality->Network_Config.cnBridge_A,
		personality->Network_Config.cnBridge_B,
		personality->Network_Config.cnBridge_C,
		personality->Network_Config.cnBridge_D,
		personality->Network_Config.cnBridge_E,
		RoQ_NetworkDesc.IOport);
	printf("Nodes: A: %d, B: %d, C: %d, D: %d, E: %d\n",
		personality->Network_Config.Anodes, personality->Network_Config.Bnodes,
		personality->Network_Config.Cnodes, personality->Network_Config.Dnodes,
		personality->Network_Config.Enodes);
	printf("COORD: A: %d, B: %d, C: %d, D: %d, E: %d\n",
		personality->Network_Config.Acoord, personality->Network_Config.Bcoord,
		personality->Network_Config.Ccoord, personality->Network_Config.Dcoord,
		personality->Network_Config.Ecoord);
#endif

	uint32_t rank;

	RoQ_TorusCoords_To_Rank(&RoQ_NetworkDesc, RoQ_NetworkDesc.Acoord,
				RoQ_NetworkDesc.Bcoord, RoQ_NetworkDesc.Ccoord,
				RoQ_NetworkDesc.Dcoord, RoQ_NetworkDesc.Ecoord, &rank);

// tjcw This code section has to be 'in' for discovery to be driven. Don't know why.
	//#if FXLOG_ENABLE
	uint64_t StartTime = get_time();

	sleep(1);

	uint64_t EndTime = get_time();
	double ElapsedTime = (EndTime - StartTime);

	DPRINTF(rank == 0, "Timer test. StartTime %lx EndTime %lx EndTime - StartTime %lx ElapsedTime %lx\n",
	(uint64_t)StartTime, (uint64_t)EndTime, (uint64_t)EndTime - StartTime, (uint64_t)ElapsedTime);
//#endif

	RoQ_LocalRank = rank; // index into the above array for the lcoal nodes info

	RoQ_GetLocalIPV4Addr( &RoQ_NetworkDesc, &RoQ_LocalIPV4Addr );

	DPRINTF(FXLOG_UCODE_INIT || RoQ_LocalRank == 0,
		"RoQ_LocalRank %lx nodes %lx RoQ_LocalIPV4Addr %lx coords %lx %lx %lx %lx %lx torus %lx %lx %lx %lx %lx\n",
		(uint64_t)RoQ_LocalRank, (uint64_t)RoQ_NetworkSize( &RoQ_NetworkDesc ), (uint64_t) RoQ_LocalIPV4Addr,
		(uint64_t)RoQ_NetworkDesc.Acoord, (uint64_t)RoQ_NetworkDesc.Bcoord,
		(uint64_t)RoQ_NetworkDesc.Ccoord, (uint64_t)RoQ_NetworkDesc.Dcoord,
		(uint64_t)RoQ_NetworkDesc.Ecoord, (uint64_t)RoQ_NetworkDesc.Anodes,
		(uint64_t)RoQ_NetworkDesc.Bnodes, (uint64_t)RoQ_NetworkDesc.Cnodes,
		(uint64_t)RoQ_NetworkDesc.Dnodes, (uint64_t)RoQ_NetworkDesc.Enodes);

	uint32_t TestA, TestB, TestC, TestD, TestE;
	RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc,rank, &TestA, &TestB, &TestC, &TestD, &TestE );

	DSTRONG_ASSERT(TestA == RoQ_NetworkDesc.Acoord, "%lx %lx\n", (uint64_t)TestA, (uint64_t)RoQ_NetworkDesc.Acoord);
	DSTRONG_ASSERT(TestB == RoQ_NetworkDesc.Bcoord, "%lx %lx\n", (uint64_t)TestB, (uint64_t)RoQ_NetworkDesc.Bcoord);
	DSTRONG_ASSERT(TestC == RoQ_NetworkDesc.Ccoord, "%lx %lx\n", (uint64_t)TestC, (uint64_t)RoQ_NetworkDesc.Ccoord);
	DSTRONG_ASSERT(TestD == RoQ_NetworkDesc.Dcoord, "%lx %lx\n", (uint64_t)TestD, (uint64_t)RoQ_NetworkDesc.Dcoord);
	DSTRONG_ASSERT(TestE == RoQ_NetworkDesc.Ecoord, "%lx %lx\n", (uint64_t)TestE, (uint64_t)RoQ_NetworkDesc.Ecoord);
	DSTRONG_ASSERT(RoQ_NetworkSize( &RoQ_NetworkDesc ) <= ROQ_MAX_IO_NODES, "%lx\n", (uint64_t)ROQ_MAX_IO_NODES);

	RoQ_TestAddressingConsistency( &RoQ_NetworkDesc );

	/* copy the MU private network desc into the dev mem for the driver to use */
	RoQ_DevMem->mNetworkDesc = RoQ_NetworkDesc;

	bzero(&RoQ_TX_Args[0], sizeof (struct RoQ_TX_Arguments));
	bzero(&RoQ_RX_Args[0], sizeof (struct RoQ_RX_Arguments));

	RoQ_TX_Args[0].mTxThreadNumber = 0;

	uint32_t port_ids[8] = { 0, 1, 2, 3, 4, 5, 6, 7};
  uint32_t RemoteGetEnable[36] = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
      , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };
	uint32_t SystemFIFO[36] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	    , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
	};

	/* configure the ports */
	RoQ_TX_Args[0].mPortId = port_ids[0];
	RoQ_RX_Args[0].mPortId = port_ids[1];

	ROQ_BGQ_MU_Torus_init(&port_ids[0], RemoteGetEnable, SystemFIFO, 36, 2);
	ROQ_ion_init(&port_ids[0], 2);

	RoQ_TX_Args[0].recfifo_ptr = ROQ_RecFifo(RoQ_TX_Args[0].mPortId);
	RoQ_RX_Args[0].recfifo_ptr = ROQ_RecFifo(RoQ_RX_Args[0].mPortId);
#ifndef USERSPACE_ROQ
	RoQ_TX_Args[0].rem_recfifo_ptr = ROQ_ion_RecFifo(RoQ_TX_Args[0].mPortId);
	RoQ_RX_Args[0].rem_recfifo_ptr = ROQ_ion_RecFifo(RoQ_RX_Args[0].mPortId);
#endif

	int i;
	for (i = 0; i < RoQ_NetworkSize(&RoQ_NetworkDesc); i++) {
		DPRINTF(FXLOG_UCODE_ALMOST_NEVER_NEEDED, "Init TranConn FROM Rank %lx TO Rank %lx\n",
			(uint64_t)RoQ_LocalRank, (uint64_t)i);

		/* personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode */
		tc_init(&RoQ_loc_TransportConnection[i], RoQ_LocalRank, i, &RoQ_TX_Args[0], &RoQ_RX_Args[0], &RoQ_NetworkDesc);
	}

	/* FIXME: need to figure out how big the"remote"cluster is! */
#ifndef USERSPACE_ROQ
	if (!(personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode))
		tc_init_remote(&RoQ_rem_TransportConnection[0], RoQ_LocalRank,
				0, &RoQ_TX_Args[0], &RoQ_RX_Args[0],
				&RoQ_NetworkDesc, personality);

	if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode)
		roq_init_remote(512);
// 20121017 tjcw ??? init_remote needs to be integrated with discovery otherwise discovery ends up with
// an empty injection FIFO map
#if 0
	if (personality->Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode)
		roq_init_remote(512);
	else
		roq_init_remote(1);
#endif
#endif

	DPRINTF(FXLOG_UCODE_INIT, "Rank %lx LocalIPV4Addr %lx\n", (uint64_t)RoQ_LocalRank, (uint64_t)RoQ_LocalIPV4Addr);

	/*
	 * Should zero out dev mem here. Nothing should be active in it yet
	 * -- in fact, it should be zero'd by power on
	 */
	RoQ_DevMem->mLocalRank = RoQ_LocalRank;
	RoQ_DevMem->mLocalIPV4Addr = RoQ_LocalIPV4Addr;

	/*
	 * Init the QueueSet that will be used by the TxThread to
	 * receive doorbells from the DD and from the RxThread.
	 *
	 * Currently two doorbells are needed:
	 * One from the DD to the DM and one from the single RxThread to the DD.
	 */
	SRSW_QueueSet_Init(&RoQ_DevMem->mTxDoorbellQueueSet[0],
			   sizeof(uint32_t),	/* sizeof item */
			   2);			/* need 2 doorbell queues. */

	/*
	 * Connect the doorbell queue for internal reads
	 * -- written by the RxThread, read by the TxThread
	 */
	SRSW_QueueSet_WriterConnect(&RoQ_DevMem->mTxDoorbellQueueSet[0], /* Queue Set pointer */
				    1,		/* queue index */
				    (4 * 1024),	/* queue size */
				    NULL,	/* queue memory area -- queue set will call kmalloc */
				    sizeof(uint32_t));	/* sizeof item in queue */

	DPRINTF(FXLOG_UCODE_INIT, "%s\n", "done with tx doorbell. doing cq");

	/*
	 * These are the CQ Doorbells.
	 * When a CQ is posted either by RX or TX threads,
	 * the CQ pointer is put in these queues and a interrupt
	 * is generated.
	 *
	 * The Linux driver is the reader of these queuesets.
	 */
	SRSW_QueueSet_WriterConnect(&RoQ_DevMem->mDD_DoorbellQueueSet,	/* Queue Set pointer */
				    0,		/* queue index */
				    (16 * 1024),/* queue size */
				     /* queue memory area -- queue set will call kmalloc */
				     (void *)(&RoQ_DevMem->DD_DoorbellQueue[0]),
				     sizeof(struct RoQ_CQ_t *)); /* sizeof item in queue */

	DPRINTF(FXLOG_UCODE_INIT, "%s\n", "done with cq connect 0 (TX)");

	SRSW_QueueSet_WriterConnect(&RoQ_DevMem->mDD_DoorbellQueueSet, /* Queue Set pointer */
				    1,		/* queue index */
				    (4 * 1024),	/* queue size */
				    /* queue memory area -- queue set will call kmalloc */
				    (void *)(&RoQ_DevMem->DD_DoorbellQueue[1]),
				    sizeof(struct RoQ_CQ_t *));	/* sizeof item in queue */

	DPRINTF(FXLOG_UCODE_INIT, "%s\n", "done with cq connect 1 (RX)");

	/* Initialise counters */
	init_DmaGetCounters();

// 	int rc2=Kernel_CreateMemoryRegion(&Counters.mCounterRegion, (void *)Counters.mCounterValues,sizeof(Counters.mCounterValues)) ;
//  	DPRINTF(1,"Kernel_CreateMemoryRegion rc=%d\n",rc2) ;
//   	Counters.mOldestCounter=0 ;
 //   	Counters.mFreeCounter=0 ;
//        { int i; for( i=0; i < kCounterQty; i++) Counters.mCounterValues[i] = -1; } // zero is done, -1 is not valid, >0 valid working

	/* Place the magic numbers */
	RoQ_DevMem->mMagic0 = RoQ_DeviceMemory_Magic0;
	RoQ_DevMem->mMagic1 = RoQ_DeviceMemory_Magic1;
	RoQ_DevMem->mMagic2 = RoQ_DeviceMemory_Magic0 & RoQ_DeviceMemory_Magic1;
	RoQ_DevMem->mMagic3 = RoQ_DeviceMemory_Magic0 ^ RoQ_DeviceMemory_Magic1;
	RoQ_DevMem->mMagic4 = sizeof(*RoQ_DevMem) ^ RoQ_DeviceMemory_Magic1;
	RoQ_DevMem->mSizeOfDevMemInDevice = sizeof( *RoQ_DevMem );
	//RoQ_DevMem->mPartitionSize;

	_bgp_msync();

	DPRINTF(1,"Magic in place RoQ_DevMem @ 0x%lx ->mLocalIPV4Addr %lx\n",
		(uint64_t)RoQ_DevMem, (uint64_t)RoQ_DevMem->mLocalIPV4Addr);

	DPRINTF(FXLOG_UCODE_INIT, "mMagic0 values 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		(uint64_t)RoQ_DevMem->mMagic0, (uint64_t)RoQ_DevMem->mMagic1,
		(uint64_t)RoQ_DevMem->mMagic2, (uint64_t)RoQ_DevMem->mMagic3,
		(uint64_t)RoQ_DevMem->mMagic4);

	DPRINTF(FXLOG_UCODE_INIT, "mMagic0 addresses 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		(uint64_t)&RoQ_DevMem->mMagic0, (uint64_t)&RoQ_DevMem->mMagic1,
		(uint64_t)&RoQ_DevMem->mMagic2, (uint64_t)&RoQ_DevMem->mMagic3,
		(uint64_t)&RoQ_DevMem->mMagic4);

	DPRINTF(FXLOG_UCODE_INIT, "mMagic0 offsets 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		(uint64_t)(((uint64_t)(&RoQ_DevMem->mMagic0))- ((uint64_t)(RoQ_DevMem))),
		(uint64_t)(((uint64_t)(&RoQ_DevMem->mMagic1))- ((uint64_t)(RoQ_DevMem))),
		(uint64_t)(((uint64_t)(&RoQ_DevMem->mMagic2))- ((uint64_t)(RoQ_DevMem))),
		(uint64_t)(((uint64_t)(&RoQ_DevMem->mMagic3))- ((uint64_t)(RoQ_DevMem))),
		(uint64_t)(((uint64_t)(&RoQ_DevMem->mMagic4))- ((uint64_t)(RoQ_DevMem))));

#ifdef USERSPACE_ROQ
	ROQ_activate();
#ifdef ROQ_USE_PTHREAD
	rc = pthread_create( &tid[0], NULL, (void* (*)(void*)) RoQ_RX, &RoQ_RX_Args[0] );
#else
#define STACKSIZE (1024 * 1024)

	void *child_stack[4];

	child_stack[0] = (void *) malloc(STACKSIZE);
	/* stack grows from high to low addresses, store/use highest stack address */
	child_stack[2] = (void *)&(((char *)child_stack[0])[STACKSIZE-1]);

	rc = clone((int (*)(void*))RoQ_RX, child_stack[2], CLONE_FILES | CLONE_FS | CLONE_IO | CLONE_SIGHAND | CLONE_SYSVSEM | CLONE_VM, &RoQ_RX_Args[0]);
#endif
#else
	rc = pthread_create( &tid[0], NULL, RoQ_RX, &RoQ_RX_Args[0] );

#ifdef STANDALONE_UD_QP_TEST
	rc = pthread_create( &tid[2], NULL, test_case, &RoQ_DevMem );
#endif /* STANDALONE_UD_QP_TEST */
#endif /* USERSPACE_ROQ */

	DPRINTF(FXLOG_UCODE_INIT, "%s\n", "RX thread or procs started -- main becoming control thread");

#ifdef ROQ_RESILIENCY
  /* Starting the RoQ Resiliency code */
  bzero(&RoQ_Resiliency_Args, sizeof (struct RoQ_Resiliency_Args_t));

  RoQ_Resiliency_Init((void *) &RoQ_Resiliency_Args);

  hw_thread = ROQ_CHECKER_THREAD;
  rc = pthread_create( &tid[3], NULL, RoQ_Resiliency_Checker, (void *) &RoQ_Resiliency_Args );

  hw_thread = ROQ_NOTIFIER_THREAD;
  rc = pthread_create( &tid[4], NULL, RoQ_Resiliency_Notifier, (void *) &RoQ_Resiliency_Args );

  hw_thread = ROQ_FORWARDER_THREAD;
  rc = pthread_create( &tid[5], NULL, RoQ_Resiliency_Forwarder, (void *) &RoQ_Resiliency_Args );

  hw_thread = ROQ_ROUTER_THREAD;
  rc = pthread_create( &tid[5], NULL, RoQ_Resiliency_Router, (void *) &RoQ_Resiliency_Args );
  /* That's all for the resiliency code */
#endif

	/*
	 * here we're not going back to the app driver so
	 * turn the core into the TX thread
	 */
	RoQ_TX( (void*) &RoQ_TX_Args[0] );

#ifdef USERSPACE_ROQ

out:
	munmap(personality, sizeof(Personality_t));
	close(fd);
	munmap(mu0, PHYMAP_SIZE_MU);
	munmap(devmem, sizeof(*RoQ_DevMem));
	close(fd1);
	close(fd2);
	close(fd3);
#ifndef ROQ_USE_PTHREAD
	free(child_stack[0]);
	free(child_stack[1]);
#endif
#endif /* USERSPACE_ROQ */
	exit(-99); /* shouldn't get here */
}
