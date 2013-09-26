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
#endif

#include <roq_fw.h>
#if defined(PKTRACE_ON)
#include <roq_fw_trace.h>
static TraceClient TraceOnStart;
static TraceClient TraceOnFinis;
static TraceClient gTraceClientSync;
static TraceClient TraceCountStart;
static TraceClient TraceCountFinis;
static TraceClient TraceCount2Start;
static TraceClient TraceCount2Finis;
static TraceClient TraceCount3Start;
static TraceClient TraceCount3Finis;
#define PKTRACE_TXACTIVE
static TraceClient TransmitActiveStart;
static TraceClient TransmitActiveFinis;
#define PKTRACE_TXTHREADACTIVE
static TraceClient TransmitThreadActiveStart;
static TraceClient TransmitThreadActiveFinis;
#define PKTRACE_TXMESSAGE
static TraceClient TxMessageStart;
static TraceClient TxMessageFinis;
#define PKTRACE_TXMESSAGEACTIVE
static TraceClient TransmitMessageActiveStart;
static TraceClient TransmitMessageActiveFinis;
#define PKTRACE_MESSAGE
static TraceClient MessageStart ;
static TraceClient MessageFinis ;
#endif

/* look at roq_common.h, roq_opcode to ensure this value doesn't map to any used value */
#define ROQ_OP_CTL_CLOSE ((enum roq_opcode)0xFFFFFFFF)
#define STALL_REP_COUNT (100*1000)

volatile union roq_tx_db_req	*RoQ_SQ_DB = NULL;
volatile union roq_tx_db_req	*RoQ_RQ_DB = NULL;
union roq_tx_db_req	*RoQ_SQ_USER_DB[ROQ_MAX_MAPPED_DB];
int user_db_get[ROQ_MAX_MAPPED_DB];
static int num_user_db = 0;

struct roq_net_event	*RoQ_NET_EVENT_DB = NULL;
int roq_net_event_db_idx;
volatile uint32_t *RoQ_CQ_DB = NULL;
RoQ_Atomic_t roq_cq_db_idx;

extern void display_mu_registers(void);

static int RoQ_ModifyQP(struct RoQ_CNTL_Command_ModifyQP *req, uint32_t *qp_id)
{
	struct roq_qp_attr			*qp_attr = &req->qp_attr;
	enum roq_qp_cmd_mask			cmd_mask = req->cmd_mask;
	int					QP_Num   = req->QP_Num;
	struct RoQ_DevicePrivatePerQPState	*PerQP   = &RoQ_DevicePrivatePerQPState[QP_Num];

	uint32_t bit_mask;
	int DestNode, rv = 0;

	for (bit_mask = 1; bit_mask; bit_mask <<= 1) {

		switch (bit_mask & cmd_mask) {
			
		case 0:
			/* bit not set; */
			break;

		case ROQ_QP_SET_STATE:

			DPRINTF(FXLOG_UCODE_CNTL,
				"ModifyQP QP(%lx) IB_QP_STATE PerQP->mState %lx ReqState %lx dm_qp->state %lx\n",
				(uint64_t)QP_Num, (uint64_t)PerQP->mState, (uint64_t)qp_attr->qp_state,
				(uint64_t)RoQ_DevMem->QP_Array[QP_Num].qp_state);

			if (cmd_mask & ROQ_QP_CUR_STATE && PerQP->mState != qp_attr->cur_qp_state) {

				DPRINTF(FXLOG_UCODE_CNTL,
					"*** ERROR *** ModifyQP", "IB_QP_CUR_STATE is set but RoQ  PerQp State %lx Expected State %lx\n",
					(uint64_t)PerQP->mState, (uint64_t)qp_attr->cur_qp_state);

			} else {
				/* need to make this the full matrix of transitions... for now just stomp */

				switch (qp_attr->qp_state) {

				case ROQ_QPS_RESET:
					if (PerQP->mState == ROQ_QPS_RTS || PerQP->mState == ROQ_QPS_SQD)
						*qp_id = QP_Num;

					PerQP->mState = ROQ_QPS_RESET;
					cmd_mask = (enum roq_qp_cmd_mask)0;

					DPRINTF(1, "QP RESET QP %lx WILL NOT PROCESS FOR ANY OTHER ATTR_MASK FLAGS\n", (uint64_t)QP_Num);

					break;

				case ROQ_QPS_INIT:
					if (PerQP->mState != ROQ_QPS_RESET && PerQP->mState != ROQ_QPS_INIT) {
						DPRINTF(1, "Illegal transition to ROQ_QPS_INIT QP %lx going to ROQ_QPS_ERROR\n", (uint64_t)QP_Num);
						PerQP->mState = ROQ_QPS_ERROR;
					} else {
						memset(PerQP, 0, sizeof(struct RoQ_DevicePrivatePerQPState));
						PerQP->mState  = ROQ_QPS_INIT;
						PerQP->mQP_Num = QP_Num;
						PerQP->mDM_QP  = &RoQ_DevMem->QP_Array[QP_Num];
					}
					break;

				case ROQ_QPS_RTR:
					PerQP->mState = ROQ_QPS_RTR;

					DPRINTF(FXLOG_UCODE_CNTL, "==> ROQ_QPS_RTR QP %lx transitioned to ROQ_QPS_RTR\n", (uint64_t)QP_Num);
					PerQP->mCurrent_Setup_Type = -1;
					PerQP->mNextIndex = -1;
					PerQP->mPrevIndex = -1;

					break;

				case ROQ_QPS_RTS:
					PerQP->mState = ROQ_QPS_RTS;
					DPRINTF(FXLOG_UCODE_CNTL, "==> ROQ_QPS_RT QP %lx transitioned to ROQ_QPS_RTS\n", (uint64_t)QP_Num);
					PerQP->mCurrent_Setup_Type = -1;
					PerQP->mNextIndex = -1;
					PerQP->mPrevIndex = -1;

					break;

				case ROQ_QPS_SQD:
					PerQP->mState = ROQ_QPS_SQD;
					DPRINTF(FXLOG_UCODE_CNTL, "==> ROQ_QPS_SQD QP %lx transitioned to ROQ_QPS_SQD (Closing)\n", (uint64_t)QP_Num);

					break;

				case ROQ_QPS_SQE:
					if (PerQP->mState == ROQ_QPS_RTS || PerQP->mState == ROQ_QPS_SQD)
						*qp_id = QP_Num;

					PerQP->mState = ROQ_QPS_SQE;
					DPRINTF(FXLOG_UCODE_CNTL, "==> ROQ_QPS_SQE QP %lx transitioned to ROQ_QPS_Terminate\n", (uint64_t)QP_Num);

					break;

				case ROQ_QPS_ERROR:
					if (PerQP->mState == ROQ_QPS_RTS || PerQP->mState == ROQ_QPS_SQD)
						*qp_id = QP_Num;

					/*
					 * NEED actually need to generate the flushed WCs here
					 */
					PerQP->mState = ROQ_QPS_ERROR;
					DPRINTF(FXLOG_UCODE_CNTL, "==> ROQ_QPS_ERROR QP %lx transitioned to ROQ_QPS_Error\n", (uint64_t)QP_Num);

					break;
				}
			}
			break;

		case ROQ_QP_SET_DEST_NODE:
			DestNode = qp_attr->dest_node_id & 0x7fff;
			PerQP->mTranConn = (qp_attr->dest_node_id & 0x8000) ? &RoQ_rem_TransportConnection[DestNode] : &RoQ_loc_TransportConnection[DestNode];

			DPRINTF(FXLOG_UCODE_CNTL,
				"ModifyQP QP %lx IB_QP_AV (peer address vector) dest node 0x%lx PerQP@ 0x%p TranConn@ 0x%p\n",
				(uint64_t)QP_Num, (uint64_t) qp_attr->dest_node_id, (void *) PerQP, (void *)PerQP->mTranConn);

			break;

		case ROQ_QP_SET_DEST_QP:
			DPRINTF(FXLOG_UCODE_CNTL,
				"ModifyQP QP %lx IB_QP_DEST_QPN Setting Dest QP to %lx PerQP@ 0x%p\n",
				(uint64_t)QP_Num, (uint64_t)qp_attr->dest_qp_num, (void *) PerQP);

			if (qp_attr->dest_qp_num < RoQ_MAX_QP ) {
				PerQP->mPeerQPId.mWhole = 0;
				PerQP->mPeerQPId.mIndex = qp_attr->dest_qp_num;
			} else {
				/*
				 * XXX
				 * While an error should be reported, for now QP is just set unusable.
				 */
				PerQP->mState = ROQ_QPS_ERROR;
#if 0
				DPRINTF(FXLOG_UCODE_CNTL, "QP[%d]: Bad Dest QP %u\n", QP_Num, qp_attr->dest_qp_num);
				rv = -EINVAL;
#endif
			}
			break;

		case ROQ_QP_SET_ACCESS_FLAGS:
			DPRINTF(FXLOG_UCODE_CNTL, "ModifyQP QP %lx ROQ_QP_SET_ACCESS_FLAGS PerQP@ 0x%p\n", (uint64_t)QP_Num, (void *) PerQP);

			break;

		case ROQ_QP_START_TRACE:
#if defined(PKTRACE_ON)
//#define PKTRACE_TEST_EXTENSION
			DPRINTF(FXLOG_UCODE_CNTL, "ModifyQP QP %lx TraceOn\n", (uint64_t)QP_Num);
			TraceOn(qp_attr->trace_key);
			int a ;
			HitOE(gTraceClientSync,"TraceClientSync", 0);
			for(a = 0; a < 10; a += 1) {
#if defined(PKTRACE_TEST_EXTENSION)
			    HitOE1(TraceCountStart,"TraceCount_%02x",0,a % 16) ;
			    HitOE2(TraceCount2Start,"TraceCount_%02x_%02x",0,a%16,(a/2)%16) ;
			    HitOE3(TraceCount3Start,"TraceCount_%02x_%02x_%02x",0,a%16,(a/2)%16,(a/4)%16) ;
#endif
			    HitOE(TraceOnStart,"TraceOn", 0);
			    HitOE(TraceOnFinis,"TraceOn", 0);
#if defined(PKTRACE_TEST_EXTENSION)
			    HitOE3(TraceCount3Finis,"TraceCount_%02x_%02x_%02x",0,a%16,(a/2)%16,(a/4)%16) ;
			    HitOE2(TraceCount2Finis,"TraceCount_%02x_%02x",0,a%16,(a/2)%16) ;
			    HitOE1(TraceCountFinis,"TraceCount_%02x",0,a % 16) ;
#endif
			  }

			break;
#endif
		case ROQ_QP_STOP_TRACE:
#if defined(PKTRACE_ON)
			DPRINTF(FXLOG_UCODE_CNTL, "ModifyQP QP %lx TraceOff\n", (uint64_t)QP_Num);
			TraceOff();

			break;
#endif
		default:
			DPRINTF(FXLOG_UCODE_CNTL, "ModifyQP unhandled modify request -- bit mask %x PerQP@ 0x%p\n",
				bit_mask, (void *) PerQP);

			break;
		}
	}
	if (rv == 0)
		RoQ_DevMem->QP_Array[QP_Num].qp_state = PerQP->mState;

	return rv;
}

static inline uint32_t RoQ_CntlMsgPoll()
{
	struct RoQ_CNTL_Command	*Command;
	uint32_t		t = RoQ_AtomicRead(&RoQ_DevMem->mTestTime);
	uint64_t		db_id;
	int			ctrl_q_idx, rv;
	uint32_t		qp_id = 0;
	
	if (t != BootTime) {

		/*
		 * The difference between the MU DevMem and the DD DevMem --
		 * this can be added to DD DevMem to produce an MU viable address
		 */
		RoQ_DD2MC_AddressTranslation = (long long)RoQ_DevMem - (long long)RoQ_DevMem->mDD_DevMem_Address;

		DPRINTF(FXLOG_UCODE_CNTL,
			"*!*!* Sensed Device Driver Boot *!*!* got %d had %p RoQ_DevMem->mDD_DevMem_Address 0x%p "
			"MU RoQ_DevMem 0x %p RoQ_DD2MC_AddressTranslation %lx\n",
			t, (void *)BootTime, (void *) RoQ_DevMem->mDD_DevMem_Address,
			(void *) RoQ_DevMem, (uint64_t)RoQ_DD2MC_AddressTranslation);

		BootTime = t;
	}

	if (RoQ_DevMem->CNTL_RequestPosted == RoQ_DevMem->CNTL_RequestCompleted)
		return 0;

	ctrl_q_idx = RoQ_DevMem->CNTL_RequestCompleted % ROQ_CTRL_Q_SIZE;

	Command = &RoQ_DevMem->CNTL_Command[ctrl_q_idx];

	DPRINTF(FXLOG_UCODE_CNTL, "Got Host Request at idx %d\n", ctrl_q_idx);

	switch (Command->Type) {

	case RoQ_CNTL_Command_ModifyQP:
		rv = RoQ_ModifyQP(&Command->Request.ModifyQP, &qp_id);
		Command->Return.ModifyQP.mRc = rv;
		if (rv)
			DPRINTF(FXLOG_UCODE_CNTL, "QP[%d]: Report back ModifyQP failure: %d\n", Command->Return.ModifyQP.QP_Num, rv);
		break;

	case RoQ_CNTL_Command_QueryQP:
		DSTRONG_ASSERT(0, "%s\n", "CNTL Query QP not implemented");
		break;

	case RoQ_CNTL_Command_Discovery:
		RoQ_discovery_act = 1;
		break;

	case RoQ_CNTL_Command_RegisterDB:

		switch (Command->Request.RegisterDB.db_type) {

		case ROQ_DB_QP_PRIV_CLIENT:
			/*
			 * Privileged doorbell (Linux kernel)
			 */
			RoQ_SQ_DB = (union roq_tx_db_req *)Command->Request.RegisterDB.SQ_Addr;
			RoQ_SQ_DB = (union roq_tx_db_req *)RoQ_CnkDmaAddr_To_CnkVirtAddr((uint64_t)RoQ_SQ_DB);

			RoQ_RQ_DB = (union roq_tx_db_req *)Command->Request.RegisterDB.RQ_Addr;
			RoQ_RQ_DB = (union roq_tx_db_req *)RoQ_CnkDmaAddr_To_CnkVirtAddr((uint64_t)RoQ_RQ_DB);

			_store_shared(Command->Return.RegisterDB.db_id, (uint64_t)RoQ_SQ_DB);

			DPRINTF(FXLOG_UCODE_CNTL, "QP DB:: Send DB at 0x%llx, Read DB at 0x%llx, item size %d",
				RoQ_SQ_DB, RoQ_RQ_DB, sizeof(union roq_tx_db_req));

			break;

		case ROQ_DB_QP_USER_CLIENT:
			/*
			 * User level doorbell
			 */
			if (num_user_db < ROQ_MAX_MAPPED_DB) {
				uint64_t db_address = Command->Request.RegisterDB.SQ_Addr;
				db_address = RoQ_CnkDmaAddr_To_CnkVirtAddr((uint64_t)(db_address));
				/*
				 * New DB is always at highest index in DB array
				 */
				RoQ_SQ_USER_DB[num_user_db] = (union roq_tx_db_req *)db_address;
				user_db_get[num_user_db] = 0;

				_store_shared(Command->Return.RegisterDB.db_id, db_address);

				num_user_db++;
			} else {
				DPRINTF(FXLOG_UCODE_CNTL, "User mapped SQ: out of resources\n");
				 *(volatile uint64_t *)&Command->Return.RegisterDB.db_id = ROQ_INVALID_DB;
			}
			break;

		case ROQ_DB_CQ_CLIENT:

			RoQ_CQ_DB = (uint32_t *)Command->Request.RegisterDB.SQ_Addr;
			RoQ_CQ_DB = (uint32_t *)RoQ_CnkDmaAddr_To_CnkVirtAddr((uint64_t)RoQ_CQ_DB);

			_store_shared(Command->Return.RegisterDB.db_id, (uint64_t)RoQ_CQ_DB);
			roq_cq_db_idx = ROQ_CQ_DB_SIZE - 1;

			DPRINTF(FXLOG_UCODE_CNTL, "CQ DB:: DB at 0x%llx", RoQ_CQ_DB);

			break;

		case ROQ_DB_NET_EVENT:
			if (RoQ_NET_EVENT_DB == NULL) {
				RoQ_NET_EVENT_DB = (struct roq_net_event *)Command->Request.RegisterDB.SQ_Addr;
				RoQ_NET_EVENT_DB = (struct roq_net_event *)RoQ_CnkDmaAddr_To_CnkVirtAddr((uint64_t)RoQ_NET_EVENT_DB);

				 *(volatile uint64_t *)&Command->Return.RegisterDB.db_id = (uint64_t)RoQ_NET_EVENT_DB;
				roq_net_event_db_idx = 0;

				DPRINTF(FXLOG_UCODE_CNTL, "NET EVENT DB:: DB at 0x%llx", RoQ_NET_EVENT_DB);
			} else {
				DPRINTF(FXLOG_UCODE_CNTL, "NET EVENT DB:: Net event DB already assigned at 0x%llx\n",
					RoQ_NET_EVENT_DB);
				_store_shared(Command->Return.RegisterDB.db_id, ROQ_INVALID_DB);
			}

			break;
		default:
			DPRINTF(FXLOG_UCODE_CNTL, "Unknown DB registration client: %d\n",
				Command->Request.RegisterDB.db_type);

			_store_shared(Command->Return.RegisterDB.db_id, ROQ_INVALID_DB);
		}

		break;

	case RoQ_CNTL_Command_RevokeDB:
		switch (Command->Request.RevokeDB.db_type) {

		case ROQ_DB_QP_PRIV_CLIENT:
			DPRINTF(FXLOG_UCODE_CNTL, "Revoking privileged DB\n");
			RoQ_SQ_DB = NULL;
			RoQ_RQ_DB = NULL;

			break;

		case ROQ_DB_QP_USER_CLIENT:
			if (Command->Request.RevokeDB.db_id != 0) {
				uint64_t db_id = Command->Request.RevokeDB.db_id;
				int db_idx = 0;

				for (db_idx = 0; db_idx < ROQ_MAX_MAPPED_DB; db_idx++) {
					if (db_id == (uint64_t)RoQ_SQ_USER_DB[db_idx])
						break;
				}
				if (db_idx < ROQ_MAX_MAPPED_DB) {

					num_user_db--;

					if (db_idx != num_user_db) {
						/*
						 * fill DB gap if DB to be removed
						 * was not at highest index.
						 */
						RoQ_SQ_USER_DB[db_idx] = RoQ_SQ_USER_DB[num_user_db];
						user_db_get[db_idx] = user_db_get[num_user_db];
					}
					RoQ_SQ_USER_DB[num_user_db] = NULL;
					user_db_get[num_user_db] = 0;
				} else
					DPRINTF(FXLOG_UCODE_CNTL, "Revoking user DB: stale user ID %llx\n", db_id);
			}
			break;

		case ROQ_DB_CQ_CLIENT:
			DPRINTF(FXLOG_UCODE_CNTL, "Revoking CQ DB not yet supported\n");

			break;
		}
		break;
	}
	/*
	 * Inform DD about completed CTRL Request
	 */
	RoQ_DevMem->CNTL_RequestCompleted++;
	RoQ_Post_DM_Event(RoQ_DB_CTRL, 0);

	/*
	 * Return valid QP ID if ModifyQP results in termination packet to be
	 * sent by TX thread
	 */
	return qp_id;
}

static void RoQ_ProcessAck(struct RoQ_TX_Arguments *TxArgs)
{
	union RoQ_BGQ_MU_FifoPkt_SwHdr	*BGQ_SwHdr;
	struct RoQ_TransportConnection	*TranConn;

	MUHWI_PacketHeader_t		*pkt;

	uint32_t			AckPkt_Source,
					AckPktIOLinkBoundPacketFlag,
					AckPkt_PutOffsetWhole;
	uint64_t			AckPkt_AckedSeqNo;

	void *buf = NULL;
	int   b = ROQ_getnext_packet(2, TxArgs->recfifo_ptr, &buf, 0);

#ifndef USERSPACE_ROQ
	if (b == 0)
		b = ROQ_getnext_packet(3, TxArgs->rem_recfifo_ptr, &buf, 0);
#endif

	if (b <= 0)
		return;
	pkt				= (MUHWI_PacketHeader_t *)buf;
	BGQ_SwHdr			= (union RoQ_BGQ_MU_FifoPkt_SwHdr *) &pkt->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2;
	AckPkt_Source			= BGQ_SwHdr->mSource;
	AckPktIOLinkBoundPacketFlag	= BGQ_SwHdr->mIOLinkBoundPacketFlag;
	AckPkt_AckedSeqNo		= BGQ_SwHdr->mAckedBurstAbsoluteSeqNo;
	AckPkt_PutOffsetWhole		= RoQ_MUHWI_MUHdr_PutOffset_LSB(pkt);
	TranConn			= (AckPktIOLinkBoundPacketFlag) ?
						&RoQ_rem_TransportConnection[AckPkt_Source] : &RoQ_loc_TransportConnection[AckPkt_Source];

#ifdef ROQ_RESILIENCY_DEBUG
	{
		uint32_t pa, pb, pc, pd, pe;
		
		RoQ_Rank_To_TorusCoords(&RoQ_NetworkDesc, AckPkt_Source, &pa,
								&pb, &pc, &pd, &pe);
		
		DPRINTF(FXLOG_PER_MESSAGE, "Receiving ACK from (%d,%d,%d,%d,%d), offset = %x, this = (%d,%d,%d,%d,%d)\n",
			   pa, pb, pc, pd, pe,
			   RoQ_MUHWI_MUHdr_PutOffset_LSB(pkt),
			   personality->Network_Config.Acoord,
			   personality->Network_Config.Bcoord,
			   personality->Network_Config.Ccoord,
			   personality->Network_Config.Dcoord,
			   personality->Network_Config.Ecoord);
	}
#endif

	switch (AckPkt_PutOffsetWhole) {

	default:
		DPRINTF(PKFXLOG_NACK_HANDLING,
			"*** DISCARDING ACK/NACK *** PROTOCOL ERROR **** PutOffset LSB %lx MSB %lx SourceNodeId %lx Pkt AckSeqNo %lx "
			"NextExpectedAck %lx NextToSend %lx SeqNo in AckPkt %lx TranConn@ 0x%p\n",
			AckPkt_PutOffsetWhole, (uint64_t)(int) RoQ_MUHWI_MUHdr_PutOffset_MSB(pkt),
			(uint64_t)AckPkt_Source, AckPkt_AckedSeqNo, TranConn->mBurstSeqNo_NextExpectedAck,
			TranConn->mBurstSeqNo_NextToSend, BGQ_SwHdr->mAckedBurstAbsoluteSeqNo, (void *)TranConn);

	case 0xFFFFFFFE:
		DPRINTF(PKFXLOG_NACK_HANDLING,
			"Received NACK will ReTX PutOffset %lx SourceNodeId %lx Pkt AckSeqNo %lx NextExpectedAck %lx "
			"NextToSend %lx SeqNo in AckPkt %lx TranConn@ 0x%p\n",
			AckPkt_PutOffsetWhole, (uint64_t)AckPkt_Source,  AckPkt_AckedSeqNo, TranConn->mBurstSeqNo_NextExpectedAck,
			TranConn->mBurstSeqNo_NextToSend, BGQ_SwHdr->mAckedBurstAbsoluteSeqNo, (void *)TranConn);
		/*
		 * What the NACK means. It is a go back N signal.
		 * It carries the last seq no of the discarded burst.
		 * We could use it to complete what went before which is implicitly acked.
		 */
		for (b = AckPkt_AckedSeqNo; b < (int)TranConn->mBurstSeqNo_NextToSend; b++)
			tc_ReTX_Burst(TranConn, b & RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK);

		/*
		 * The actual wrap count will be incremented in the loop below as the wrapping ack is accepted.
		 * This could all be better
		 */
		DPRINTF(PKFXLOG_NACK_HANDLING,
			"Received Dup, Wrap, or Err ACK PutOffset %lx SourceNodeId %lx Pkt AckSeqNo %lx NextExpectedAck %lx "
			"SeqNo in AckPkt %lx TranConn@ 0x%p\n",
			AckPkt_PutOffsetWhole, (uint64_t)AckPkt_Source, AckPkt_AckedSeqNo, TranConn->mBurstSeqNo_NextExpectedAck,
			BGQ_SwHdr->mAckedBurstAbsoluteSeqNo, (void *)TranConn);

		break;

	case 0xFFFFFFFF:
		DASSERT(AckPkt_PutOffsetWhole == 0xFFFFFFFF, "protocol error on ack packet %x\n", AckPkt_PutOffsetWhole);
		/*
		 * likely we discard as a dup. could do more checking
		 * check if we need to send an nack
		 */

		/*
		 * Could be a race cond on mBurstSeqNo_NextToSend -- does it need an msync on the other side?
		 * cant seem to look over at NextToSend in the TX thread      AssertLogLine(AckPkt_AckedSeqNo  < TranConn->mBurstSeqNo_NextToSend)
		 */
                //	DASSERT( AckPkt_AckedSeqNo < TranConn->mBurstSeqNo_NextToSend,
                //		"Protocol ERROR AckedSeqNo %lx >= mBurstSeqNo_NextToSend %lx, NextExpectedAck %lx, TranConn@ 0x%p\n",
                //		(uint64_t)AckPkt_AckedSeqNo, (uint64_t)TranConn->mBurstSeqNo_NextToSend, (uint64_t) TranConn->mBurstSeqNo_NextExpectedAck, (void *)TranConn);
		DASSERT( AckPkt_AckedSeqNo >= TranConn->mBurstSeqNo_NextExpectedAck,
			"Protocol ERROR AckedSeqNo %ld < NextExpectedAck %ld, mBurstSeqNo_NextToSend %ld, TranConn@ 0x%p\n",
			(uint64_t)AckPkt_AckedSeqNo, (uint64_t) TranConn->mBurstSeqNo_NextExpectedAck,(uint64_t)TranConn->mBurstSeqNo_NextToSend, (void *)TranConn);

		/*
		 * Spin through the acked burst slots figuring out of a completion is in order
		 * It would be nice if the other side told us if we need to generate a completion
		 * but since acks could lost, we need to scan
		 */
		while (TranConn->mBurstSeqNo_NextExpectedAck <= AckPkt_AckedSeqNo) {
			int			      AckedBurstSlot = TranConn->mBurstSeqNo_NextExpectedAck & RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK;
			struct RoQ_TX_BurstDescriptor *BurstDescPtr  = &TranConn->mTX_BurstDescriptorArray[AckedBurstSlot];

			DPRINTF(FXLOG_UCODE_PER_BURST,
				"Processing ACK Ack received %lx AckedBurstSlot %lx BurstDescPtr@ 0x%p ->mLastBurstOfMessage %lx ->mIsInternalReadResponse %lx\n",
				(uint64_t)TranConn->mBurstSeqNo_NextExpectedAck, (uint64_t)AckedBurstSlot, (void *) BurstDescPtr,
				(uint64_t)BurstDescPtr->mLastBurstOfMessage, (uint64_t)BurstDescPtr->mIsInternalReadResponse);

			struct RoQ_DevicePrivatePerQPState *AckedPerQP = &RoQ_DevicePrivatePerQPState[BurstDescPtr->mQP_Index];
			AckedPerQP->mTxBurstsInProgress--;

			DASSERT( AckedPerQP->mTxBurstsInProgress >= 0, "AckedPerQP->mTxBurstsInProgress < 0 ... not good\n");

			if (BurstDescPtr->mLastBurstOfMessage) {
				/*
				 * NOTE: you get in here when you get the ack for the
				 * last burst of an RDMA WRITE that was generated by and RDMA READ
				 */
				struct RoQ_DM_QP		   *AckedQP	 = &RoQ_DevMem->QP_Array[BurstDescPtr->mQP_Index];

				if (BurstDescPtr->mIsInternalReadResponse) {
					/*
					 * The WRITE that is a response to a READ
					 * just acked but there is no one to tell locally.
					 */
					DPRINTF(FXLOG_UCODE_PER_MESSAGE,
						"qpn %lx Processing ACK SENSED READ RESP (AKA WRITE) COMPLETION "
						"AckBurstSlot %lx Ack received %lx QP@ 0x%p BurstDescPtr->mWorkRequestIndex %lx\n",
						(uint64_t)BurstDescPtr->mQP_Index, (uint64_t)AckedBurstSlot,
						(uint64_t)TranConn->mBurstSeqNo_NextExpectedAck, (void *)AckedQP,
						(uint64_t)BurstDescPtr->mWorkRequestIndex);

					/*
					 * modify metadata on SRSQ between TX and RX threads that an inbound read slot is opened
					 * this was done when we accepted this write into the transmit flow
					 * AckedPerQP->mInboundReadQueue_Retired++;
					 */
				} else if (BurstDescPtr->mIsDisconnect) {

					if (AckedQP->dd_qp)
						roq_post_net_event(NET_EVENT_QP_DISCONNECT, &BurstDescPtr->mQP_Index, 0);

					DPRINTF(FXLOG_UCODE_PER_MESSAGE,
						"qpn %lx Processing ACK SENSED DISCONNECT ACK AckBurstSlot %lx Ack received %lx QP@ 0x%p BurstDescPtr->mWorkRequestIndex %lx\n",
						(uint64_t)BurstDescPtr->mQP_Index, (uint64_t)AckedBurstSlot, (uint64_t)TranConn->mBurstSeqNo_NextExpectedAck,
						(void *)AckedQP, (uint64_t)BurstDescPtr->mWorkRequestIndex);

				} else if (BurstDescPtr->mIsRdmaReadRequest) {
					/*
					 * SEND and WRITE complete in here
					 * This ack is for the burst that was the last of a send work
					 * request -- do completion action and advance send queue tail
					 */
					DPRINTF(FXLOG_UCODE_PER_MESSAGE,
						"qpn %lx Processing ACK SENSED SEND QUEUE WR COMPLETION "
						"AckBurstSlot %lx Ack received %lx QP@ 0x%p BurstDescPtr->mWorkRequestIndex %lx\n",
						(uint64_t)BurstDescPtr->mQP_Index, (uint64_t)AckedBurstSlot, (uint64_t)TranConn->mBurstSeqNo_NextExpectedAck,
						(void *)AckedQP, (uint64_t)BurstDescPtr->mWorkRequestIndex);
				} else {
					DPRINTF(FXLOG_UCODE_PER_MESSAGE,
						"qpn %lx RDMA SEND/WRITE COMPLETION WR Index %lx SQ now: Posted %lx NextToSend %lx Retired %lx\n",
						(uint64_t)BurstDescPtr->mQP_Index, (uint64_t)BurstDescPtr->mWorkRequestIndex,
						(uint64_t)AckedQP->mSendQueue.mPut, (uint64_t)AckedPerQP->mSendQueue_SQE_NextToSend,
						(uint64_t)AckedQP->mSendQueue.mGot);
					/*
					 * this block handles SEND/WRITE completions to the user
					 */
					PushCompletionForWorkRequest(AckedQP, BurstDescPtr->mWorkRequestIndex,
								     AckedPerQP->mSendQueue_SQE_NextToSend, 0);
				}
			}

			/*
			 * clear info in the BurstDesc. Not clear this is necessary.
			 * Putting in for debugging. NEED to consider.
			 */
			BurstDescPtr->mQP_Index = 0;
			BurstDescPtr->mWorkRequestIndex = -1;
			BurstDescPtr->mFlags    = 0;
	
			TranConn->mBurstSeqNo_NextExpectedAck++;
		}
		break;
	} /* switch */
}


/*
 * Build a work request for an inbound READ.req
 */
static int RoQ_PrepareReadReply(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex)
{
	struct RoQ_DDP_Fields *ReadInfo;
	int SelectRead, waiting = 10000, rv = 0;

	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "RDMA READ request forwarded from RxThread qpn %lx\n", (uint64_t)SelectTXQPIndex);
	/*
	 * this is a hack to set us two the max allowed...
	 * this needs to be handled better starting with what the user askes for
	 */
	SelectTXQP->mInboundReadQueue_Count = RoQ_MAX_OUTSTANDING_RDMA_READS;

	SelectRead = SelectTXQP->mInboundReadQueue_Retired % SelectTXQP->mInboundReadQueue_Count;
	ReadInfo = &SelectTXQP->mInboundReadQueue_Array[SelectRead];

	DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
		"TX ACTIVATE READ RES qpn %lx DDP_Fields IRD Slot %lx .RDMA_Flags %lx .SrcOffset %lx .SrcSTag %lx .SinkOffset %lx .SinkSTag %lx "
		"MessageLength %lx SelectTXQP->mInboundReadQueue_Retired %lx ->mInboundReadQueue_Posted %lx\n",
		(uint64_t)SelectTXQPIndex,  (uint64_t)SelectRead, (uint64_t)ReadInfo->RDMA_Flags, (uint64_t) ReadInfo->Read.SrcOffset,
		(uint64_t) ReadInfo->Read.SrcSTag, (uint64_t) ReadInfo->Read.SinkSGE.addr, (uint64_t) ReadInfo->Read.SinkSGE.key,
		(uint64_t)ReadInfo->Read.SinkSGE.length, (uint64_t)SelectTXQP->mInboundReadQueue_Retired, (uint64_t)SelectTXQP->mInboundReadQueue_Posted);

	/* Allow for READ responses even if local SQ is already in SQD */
	if (SelectTXQP->mState > ROQ_QPS_SQD) {
		DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
            "RoQ_PrepareReadReply:: QP[%d]: state %d\n", SelectTXQPIndex, SelectTXQP->mState);
		rv = -1;
		goto out;
	}
#if 0
retry:
		ppc_msync();

		if (_load_shared(SelectTXQP->mState) == ROQ_QPS_RTS)
			goto ok;

		if (SelectTXQP->mState > ROQ_QPS_RTS) {
			rv = -1;
			goto out;
		}
		if (!waiting--) {
			DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX, "RoQ_PrepareReadReply:: QP[%d] not ready after retry\n", SelectTXQPIndex);
			rv = -1;
			goto out;
		}
		goto retry;
	}
ok:
#endif

#if 0
	DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX, "QP[%d]:: 0x%llx Prepare ReadReply: laddr 0x%llx, len %llu, lkey 0x%x, rkey 0x%x, r_posted %llu, r_retired %llu\n",
		SelectTXQPIndex, ReadInfo->Read.SinkSGE.addr, ReadInfo->Read.SrcOffset, ReadInfo->Read.SinkSGE.length,
		ReadInfo->Read.SrcSTag, ReadInfo->Read.SinkSGE.key,
		SelectTXQP->mInboundReadQueue_Posted, SelectTXQP->mInboundReadQueue_Retired);
#endif
	/*
	 * take the read request and setup the current work request
	 */
	SelectTXQP->Current_SQE.opcode = ROQ_OP_WRITE;
	/*
	 * copy the SinkSGE out of the internal read request queue
	 * NEED: this isn't required since we could handle read req queue head/cur/tail
	 * indexs to allow just using the queue element image of these values
	 */
	SelectTXQP->Current_SQE.sge[0].addr	= ReadInfo->Read.SrcOffset;
	SelectTXQP->Current_SQE.sge[0].length	= ReadInfo->Read.SinkSGE.length;
	SelectTXQP->Current_SQE.sge[0].key	= ReadInfo->Read.SrcSTag;

	SelectTXQP->Current_SendSgList = &(SelectTXQP->Current_SQE.sge[0]);
	/*
	 * WRITE TX wants to keep a byte count in a CQE... it can do it here though
	 * this won't be used for the READ CQE since it's on the other side
	 */
	SelectTXQP->Current_SQE.wr.rdma.bytes		= 0;
	SelectTXQP->Current_SQE.flags			&= ~ROQ_WQE_COMPLETED;
	SelectTXQP->mCurrent_SQE_SGE_List_Index		= 0;
	SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag	= 0;
	SelectTXQP->mCurrent_SQE_SGE_Offset		= 0;
	SelectTXQP->Current_SQE.num_sge			= 1;
	SelectTXQP->Current_SQE.wr.rdma.remote_addr	= ReadInfo->Read.SinkSGE.addr;
	SelectTXQP->Current_SQE.wr.rdma.rkey		= ReadInfo->Read.SinkSGE.key;
	SelectTXQP->Current_SQE.flags			= ROQ_WQE_ABSOLUTE_ADDR | ROQ_WQE_READ_RESPONSE;

	/* flag that the currrent WQE is NOT as SendQueue_Array item. */
	SelectTXQP->mCurrent_SQE_Index			= -1;

out:
	SelectTXQP->mInboundReadQueue_Retired++;
	ppc_msync(); /* NEEDED? */

	return rv;
}

static inline int RoQ_PrepareTxSQ(struct RoQ_DevicePrivatePerQPState *SelectTXQP, struct RoQ_DM_QP *SelectTXQP_OS, int SelectTXQPIndex)
{
	struct roq_sqe *SelectTXQP_SQ;
	int rv = 0;

	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "WORK REQUEST FROM USER mPeerQPId 0x%lx\n", SelectTXQP->mPeerQPId.mWhole);
	/*
	 * NEED: we save the current send queue index -- but really this is
	 * the same as NextToSend -- fix up to reduce qp foot print
	 */
	SelectTXQP->mCurrent_SQE_Index = SelectTXQP->mSendQueue_SQE_NextToSend % SelectTXQP_OS->mSendQueue.mItemCount;
	/*
	 * copy the currrent sqe to what should be "safe" memory ... device
	 * will validate and work from this copy
	 */
	KISS_SRSWQ_ReaccessReadItem(&SelectTXQP_OS->mSendQueue, SelectTXQP->mCurrent_SQE_Index);

	SelectTXQP_SQ = (struct roq_sqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t*) SelectTXQP_OS->mSendQueue.mItemArrayDmaAddr);

	DPRINTF(FXLOG_UCODE_PER_MESSAGE,
		"Access SQ@ Select_TXQP_OS->mSendQueue.mItemArrayDmaAddr 0x%p Translated to 0x%p\n",
		(void *) SelectTXQP_OS->mSendQueue.mItemArrayDmaAddr, (void *) SelectTXQP_SQ);

	SelectTXQP->Current_SQE = SelectTXQP_SQ[SelectTXQP->mCurrent_SQE_Index];
	SelectTXQP->Current_SQE.wr.rdma.bytes	= 0;
	SelectTXQP->Current_SQE.flags		&= ~ROQ_WQE_COMPLETED;
	SelectTXQP->Current_RDMA_OP_Timestamp	= get_time();

	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "Made local copy of Send Queue Entry mPeerQPId 0x%lx\n", SelectTXQP->mPeerQPId.mWhole);
	SelectTXQP->mCurrent_SQE_SGE_List_Index    = 0;
	SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag = 0;
	SelectTXQP->mCurrent_SQE_SGE_Offset        = 0;
	/*
	 * translate the sg_list pointer so it can be used below.
	 * on read, the sg_list is provided internally so we can't translate anywhere else.
	 */
	SelectTXQP->Current_SendSgList = &SelectTXQP->Current_SQE.sge[0];

	DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
		"TX ACTIVATE SEND WR qpn %lx mPeerQPId 0x%lx SelectTXQP->mSendQueue_SQE_NextToSend %lx ->mSendQueue.mPut %lx\n",
		(uint64_t)SelectTXQPIndex, SelectTXQP->mPeerQPId.mWhole, (uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend,
		(uint64_t)SelectTXQP_OS->mSendQueue.mPut);
	/*
	 *  now that this request is in the validated area, advance the work queue index
	 */
	SelectTXQP->mSendQueue_SQE_NextToSend++;

	if (SelectTXQP->Current_SQE.num_sge > (uint32_t)ROQ_MAX_SGE) {
		SelectTXQP->state_flags |= ROQ_TX_SUSPENDED;
		DPRINTF(1, "QP[%d]: Too many SGE's: %d > %d\n", SelectTXQPIndex, SelectTXQP->Current_SQE.num_sge, (int)ROQ_MAX_SGE);
		rv = -1;
	}
	return rv;
}


/*
 * Closing state has been set and internal read and work queues are empty.
 * Move QP to error.
 */
static int RoQ_EnterReset(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex)
{
	DPRINTF(FXLOG_UCODE_CNTL, "CLOSING qpn %lx\n", (uint64_t)SelectTXQPIndex);
	/*
	 * take the read request and setup the current work request
	 */
	/*
	 * can't still be in ROQ_QPS_SQD (closing) so set to Reset
	 * state - or we nedeed another flag to show we're waiting for the ack.
	 */
	SelectTXQP->mState = ROQ_QPS_RESET; /* for now, set to idle */
	ppc_msync();
       	SelectTXQP->Current_SQE.opcode = ROQ_OP_CTL_CLOSE;

	SelectTXQP->state_flags |= ROQ_TX_SUSPENDED;
	/*
	 * copy the SinkSGE out of the internal read request queue
	 * NEED: this isn't required since we could handle read req queue head/cur/tail
	 * indexs to allow just using the queue element image of these values
	 */
	SelectTXQP->Current_SQE.sge[0].addr	= 0;
	SelectTXQP->Current_SQE.sge[0].key	= 0xFFFFFFFF;
	SelectTXQP->Current_SQE.sge[0].length	= 0;

	SelectTXQP->Current_SendSgList				= NULL;
	SelectTXQP->mCurrent_SQE_SGE_List_Index			= 0;
	SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag		= 0;
	SelectTXQP->mCurrent_SQE_SGE_Offset			= 0;
	/*  flag that the currrent validated wr is NOT as SendQueue_Array item. */
	SelectTXQP->mCurrent_SQE_Index				= -1;

	SelectTXQP->Current_SQE.num_sge		= 0;
	SelectTXQP->Current_SQE.wr.rdma.remote_addr = 0;
	SelectTXQP->Current_SQE.wr.rdma.rkey	= 0;
	SelectTXQP->Current_SQE.flags		= 0;
}



static void RoQ_StartTxSend(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex,
			    struct RoQ_TX_BurstDescriptor *BurstDescPtr, int BurstSlot)
{
	struct roq_sqe			*SQE_Ptr    = &SelectTXQP->Current_SQE;
	struct roq_sge			*SGE_Ptr    = &SelectTXQP->Current_SendSgList[SelectTXQP->mCurrent_SQE_SGE_List_Index];
	union RoQ_ShortSegmentPacket	*SetupPkt   = &BurstDescPtr->mSetupPacket;
	union RDMA_SetupChunk		*SetupChunk = &SetupPkt->RDMA.SetupChunk;
	uint32_t			BurstLength = sizeof *SetupChunk;

	/*
	 * really looks like the setup chunk could be part of the work request itself
	 * but that wouldn't help get the aligment info chunks in
	 */
	DASSERT(!SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag, "Restart TxSend?\n");
	DASSERT(SelectTXQP->mSendWorkRequestActiveFlag, "TxSend: not active?\n");

	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "ROQ_OP_SEND SENDING SETUP BURST SQE_SGE_Index %lx\n",
		(uint64_t)SelectTXQP->mCurrent_SQE_SGE_List_Index);

	DASSERT(SelectTXQP->mCurrent_SQE_SGE_Offset == 0, "TX Segment handling protocol error %d\n",
		SelectTXQP->mCurrent_SQE_SGE_Offset);

	/* Pretty sure this wants to auto wrap Posted into 32 bits of MSN */
	if (SelectTXQP->mCurrent_SQE_SGE_List_Index == 0) {
		SetupChunk->DDP_Fields.Send.MSN = SelectTXQP->mSendQueue_SQE_NextToSend - 1;
		SetupChunk->DDP_Fields.Send.MO = 0;
	}
	/*
	 * NEED: segment lengths are a little more difficult to work out that this
	 * NEED: Message length needs to be calcuated from a spin of the sg_list lens
	 * NEED: pull missaligned ends of SG element into this burst so the remainder bulk path can assume alignment
	 */
	DSTRONG_ASSERT(SGE_Ptr->length < SEGMENT_SIZE, "SGE length is greater than segment size -- not supported (%d)\n", SGE_Ptr->length);

#ifdef RoQ_SEND_USER_DATA_IN_SETUP
	if (SQE_Ptr->num_sge == 1 && SGE_Ptr->length <= RoQ_USER_DATA_IN_SETUP_SIZE) {

		DPRINTF(FXLOG_UCODE_PER_BURST, "TX SHORT WRITE SETUP PeerQPId.mWhole %lx len %lx\n",
			(uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t)SGE_Ptr->length);

		if (SGE_Ptr->length) {
			int      virtual_page_a  = SGE_Ptr->addr & ~RoQ_BURST_MASK;
			uint64_t MsgLastByteAddr = SGE_Ptr->addr + SGE_Ptr->length - 1;
			int      virtual_page_b  = MsgLastByteAddr & ~RoQ_BURST_MASK;
			uint64_t RoQ_DmaAddr, RoQ_VirtAddr;

			if (virtual_page_a == virtual_page_b) {
				/* short send does not span page boundary */
				RoQ_DmaAddr = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr, SGE_Ptr->length, __LINE__);
				RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

				DPRINTF(FXLOG_UCODE_PER_BURST, "TX SHORT SEND ACCESSING DATA qpn %lx PeerQPId.mWhole %lx stag %lx sge.addr 0x%lx sge.paddr 0x%lx len %lx\n",
					(uint64_t)SelectTXQP->mQP_Num, (uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SGE_Ptr->key,
					(uint64_t) SGE_Ptr->addr, (uint64_t) RoQ_DmaAddr, (uint64_t)SGE_Ptr->length);

				memcpy((void *)SetupPkt->RDMA.Payload, (void *)RoQ_VirtAddr, SGE_Ptr->length);

				RoQ_Log16Words("TX SHORT SEND DATA", (char *)SetupPkt->RDMA.Payload, SGE_Ptr->length);
			} else {
				/* short send spans a page boundary and will require two page lookup/memcpy calls */
				uint32_t len2 = (MsgLastByteAddr & RoQ_BURST_MASK) + 1;
				uint32_t len1 = SGE_Ptr->length - len2;

				/* NEED: This assert is probably not required and should probably come out for  per reasons */
				DASSERT((SGE_Ptr->length == (len1+len2)), "%lx %lx %lx %lx %lx \n",
					SGE_Ptr->key, SGE_Ptr->addr, SGE_Ptr->length, len1, len2);

				RoQ_DmaAddr  = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr, len1, __LINE__);
				RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

				memcpy((void *)SetupPkt->RDMA.Payload, (void *)RoQ_VirtAddr, len1);

				DASSERT(((((uint64_t)(SGE_Ptr->addr) + len1) & RoQ_BURST_MASK) == 0), "%lx %lx\n", SGE_Ptr->addr, len1);

				RoQ_DmaAddr  = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr + len1, len2, __LINE__);
				RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

				memcpy((void *)(((char *)SetupPkt->RDMA.Payload) + len1), (void *)RoQ_VirtAddr, len2);

				DPRINTF(FXLOG_UCODE_PER_BURST,
					"TX SHORT WRITE ACCESSING DATA (TWO MEMCPY) qpn %lx PeerQPId.mWhole %lx stag %lx sge.addr 0x %lx sge.length %lx DmaAddr 0x%lx\n",
					(uint64_t)SelectTXQP->mQP_Num, (uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SGE_Ptr->key,
					(uint64_t) SGE_Ptr->addr, (uint64_t)SGE_Ptr->length, (uint64_t) RoQ_DmaAddr);
			}
			BurstLength += SGE_Ptr->length;
		}
		/*
		 * NOTE: this flag is what will allow generating a user completion when this burst is acked.
		 * NOTE: Since resends are from user buffers, completion must wait for ack!
		 */
		BurstDescPtr->mLastBurstOfMessage = 1;

		DASSERT(SelectTXQP->mSendQueue_SQE_NextToSend <= RoQ_DevMem->QP_Array[SelectTXQPIndex].mSendQueue.mPut,
			"%s\n", "Protocol error");

		SelectTXQP->mSendWorkRequestActiveFlag = 0;

		DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
			"TX SHORT WRITE FINISHED WR TX PACKET DATA LOAD IsReadResponse %lx BurstLength %lx sge length %lx CurSentSetup %lx\n",
			(uint64_t)BurstDescPtr->mIsInternalReadResponse, (uint64_t)BurstLength, (uint64_t)SGE_Ptr->length,
			(uint64_t)SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag);
	}
#endif /* RoQ_SEND_USER_DATA_IN_SETUP */
	SetupChunk->DDP_Fields.Send.MessageLength = SGE_Ptr->length;

	DPRINTF(FXLOG_UCODE_PER_MESSAGE,
		"RDMA SETUP DDP_Fields.Send... PeerQPId %lx SetupPkt 0x%p BurstLen %lx .MSN %lx .MessageLength %lx .MO %lx\n",
		(uint64_t)SelectTXQP->mPeerQPId.mWhole, (void *) SetupPkt, (uint64_t)BurstLength, (uint64_t)SetupChunk->DDP_Fields.Send.MSN,
		(uint64_t)SetupChunk->DDP_Fields.Send.MessageLength, (uint64_t)SetupChunk->DDP_Fields.Send.MO);

	/* here fill out the alignment chunks so the rest of the transfer can be be  aligned with the help of the MU */
	RoQ_Log16Words("TX RDMA SEND SETUP", (char*)SetupPkt, BurstLength);

	unsigned BurstType;
        if( SelectTXQP->mSendWorkRequestActiveFlag ) // this should be done better -- actually as part of above code path
        	BurstType = 0 |  PktT_LastBurstOfMsgOFF | PktT_SetupON | PktT_IB | PktT_DMA_OFF | ROQ_OP_SEND;
        else
        	BurstType = 0 | PktT_LastBurstOfMsgON  | PktT_SetupON | PktT_IB | PktT_DMA_OFF | ROQ_OP_SEND;

	if (SQE_Ptr->num_sge > 1)
		DPRINTF(FXLOG_UCODE_PER_MESSAGE, "QP%d: -> QP%d SEND: (af=%s), num_sge = %d, %llu, %llu, %llu, %llu, %lx, LB %s\n",
			SelectTXQPIndex, SelectTXQP->mPeerQPId.mWhole,
			SelectTXQP->mSendWorkRequestActiveFlag ? "Y" : "N",
			SQE_Ptr->num_sge, BurstLength, SetupChunk->DDP_Fields.Send.MSN,
			SetupChunk->DDP_Fields.Send.MessageLength, SetupChunk->DDP_Fields.Send.MO,
			BurstType, BurstType & PktT_LastBurstOfMsgON ? "Y" : "N");

	tc_TX_Burst(SelectTXQP->mTranConn, BurstSlot, SelectTXQP->mPeerQPId, BurstType,
		    (uint64_t)SetupPkt, BurstLength);

	if (SelectTXQP->mSendWorkRequestActiveFlag)
		SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag = 1;
}


static void RoQ_TxDisconnect(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex,
			     struct RoQ_TX_BurstDescriptor *BurstDescPtr, int BurstSlot)
{
	union RoQ_ShortSegmentPacket	*SetupPkt   = &BurstDescPtr->mSetupPacket;
	union RDMA_SetupChunk	*SetupChunk = &SetupPkt->RDMA.SetupChunk;
	
	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "%s\n", "TX CNTL CLOSED TX case SetupON | PktT_CLOSE | ROQ_OP_NULL");

	SetupChunk->DDP_Fields.RDMA_Flags           = 0;
	SetupChunk->DDP_Fields.Write.SinkSGE.addr   = 0;
	SetupChunk->DDP_Fields.Write.SinkSGE.key    = 0;
	SetupChunk->DDP_Fields.Write.SinkSGE.length = 0;

	BurstDescPtr->mLastBurstOfMessage = 1;
	BurstDescPtr->mIsDisconnect       = 1;

	/*
	 *  here fill out the alignment chunks so the rest of the
	 *  transfer can be be aligned with the help of the MU
	 */
	RoQ_Log16Words("TX CNTL SETUP", (char *)SetupPkt, sizeof *SetupChunk);


	unsigned BurstType = 0 | PktT_LastBurstOfMsgOFF | PktT_SetupON | PktT_CNTL | PktT_DMA_OFF | ROQ_OP_NULL;

	tc_TX_Burst(SelectTXQP->mTranConn,
		    BurstSlot,
		    SelectTXQP->mPeerQPId,
		    BurstType,
		    (uint64_t)SetupPkt,
		    sizeof *SetupChunk);

	SelectTXQP->mSendWorkRequestActiveFlag = 0;
}

/*
 * Build one burst out of the current SGE. Advance SGL
 * state and set work completion if appropriate.
 */
static void RoQ_ResumeTxSGE(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex,
			    struct RoQ_TX_BurstDescriptor *BurstDescPtr, int BurstSlot,
			    enum roq_opcode opcode)
{
	struct roq_sqe			*SQE_Ptr    = &SelectTXQP->Current_SQE;
	struct roq_sge			*SGE_Ptr    = &SelectTXQP->Current_SendSgList[SelectTXQP->mCurrent_SQE_SGE_List_Index];
	union RoQ_ShortSegmentPacket	*SetupPkt   = &BurstDescPtr->mSetupPacket;
	union RDMA_SetupChunk		*SetupChunk = &SetupPkt->RDMA.SetupChunk;
	
	uint32_t			BurstLength = SGE_Ptr->length - SelectTXQP->mCurrent_SQE_SGE_Offset;

	uint64_t			Local_VirtAddr = SGE_Ptr->addr + SelectTXQP->mCurrent_SQE_SGE_Offset,
					RoQ_DmaAddr,
					RoQ_VirtAddr;


	if (!BurstLength) {
		DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST, "zero len burst resume:: opcode %d, sge_idx %d, sge_off %d, num sge %d, key 0x%lx cur laddr 0x%lx "
			"cur len 0x%lx SQE@ 0x%lx ->send_flags 0x %lx ->...rkey 0x%lx ->...remote_addr 0x%lx\n",
			SQE_Ptr->opcode, SelectTXQP->mCurrent_SQE_SGE_List_Index, SelectTXQP->mCurrent_SQE_SGE_Offset,
			SQE_Ptr->num_sge, SGE_Ptr->key, SQE_Ptr->sge[SelectTXQP->mCurrent_SQE_SGE_List_Index].addr,
			SQE_Ptr->sge[SelectTXQP->mCurrent_SQE_SGE_List_Index].length,
			SQE_Ptr, SQE_Ptr->flags, SQE_Ptr->wr.rdma.rkey,
			SQE_Ptr->wr.rdma.remote_addr);
#if 0
		while (1)
			sleep (1);
#endif
	}
	DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST,
		"TX WRITE BODY qp->SQE_SGE Index %lx qp->SQE_SGE_Offest %lx SGE->num_sge %lx cur key 0x%lx cur laddr 0x%lx "
		"cur len 0x%lx SQE@ 0x%lx ->send_flags 0x %lx ->...rkey 0x%lx ->...remote_addr 0x%lx\n",
		(uint64_t)SelectTXQP->mCurrent_SQE_SGE_List_Index, (uint64_t)SelectTXQP->mCurrent_SQE_SGE_Offset,
		(uint64_t)SQE_Ptr->num_sge, (uint64_t) SGE_Ptr->key,
		(uint64_t)SQE_Ptr->sge[SelectTXQP->mCurrent_SQE_SGE_List_Index].addr,
		(uint64_t)SQE_Ptr->sge[SelectTXQP->mCurrent_SQE_SGE_List_Index].length,
		(uint64_t)SQE_Ptr, (uint64_t)SQE_Ptr->flags, (uint64_t)SQE_Ptr->wr.rdma.rkey,
		(uint64_t)SQE_Ptr->wr.rdma.remote_addr);

        // this method should not be called w/o the SendWorkRequestActiveFlag set.
	DASSERT( SelectTXQP->mSendWorkRequestActiveFlag , "RoQ_ResumeTxSGE(), SelectTXQP->mSendWorkRequestActiveFlag == 0\n");

	if (BurstLength > RoQ_BURST_SIZE)
		BurstLength = RoQ_BURST_SIZE;

	DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST,
		"TX WRITE BODY BurstLength %lx (remvaddr + BurstLength & MASK) %p\n",
		(uint64_t)BurstLength, (void *)((SelectTXQP->mCurrent_Remote_VirtAddr + BurstLength) & RoQ_BURST_MASK));

	/*
	 * Currently, need to look at both the source and sink sides and not cross page boundaries in a single burst
	 * So limit BurstLength to last byte on same page as the Remote_Offset -- ((last byte to place % page size) - start)
	 * BMT: XXX Do that also for Send's: Sometimes we may know target address from setup for 0copy RX.
	 */
	if (opcode == ROQ_OP_WRITE
	    && ((SelectTXQP->mCurrent_Remote_VirtAddr & RoQ_BURST_MASK) + BurstLength ) > RoQ_BURST_SIZE)
		BurstLength -= ((SelectTXQP->mCurrent_Remote_VirtAddr + BurstLength) & RoQ_BURST_MASK);

	if (((Local_VirtAddr & RoQ_BURST_MASK) + BurstLength) > RoQ_BURST_SIZE)
		BurstLength -= ((Local_VirtAddr + BurstLength) & RoQ_BURST_MASK);

	DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST,
		"TX WRITE BODY Local_VirtAddr 0x%lx BurstLength 0x%lx SGE_Offset 0x%lx\n",
		(uint64_t) Local_VirtAddr, (uint64_t) BurstLength, (uint64_t) SelectTXQP->mCurrent_SQE_SGE_Offset);

	RoQ_DmaAddr = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr + SelectTXQP->mCurrent_SQE_SGE_Offset,
					 BurstLength, __LINE__);

	RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

	DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST,
		"TX SGE BODY PeerQPId.mWhole %lx UserMemAddr 0x%lx RoQ_DmaAddr 0x%lx "
		"RoQ_VirtAddr 0x%lx LocOffset 0x%lx RemAddr 0x%lx BurstLenght %lx BurstSlot %lx\n",
		(uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SGE_Ptr->addr,
		(uint64_t) RoQ_DmaAddr, (uint64_t) RoQ_VirtAddr, (uint64_t) SelectTXQP->mCurrent_SQE_SGE_Offset,
		(uint64_t) SelectTXQP->mCurrent_Remote_VirtAddr, (uint64_t)BurstLength, (uint64_t)BurstSlot);


	if (SQE_Ptr->num_sge > ROQ_MAX_SGE) {
		DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST, "resume sge:: %lu, %lu (%d :: %d)\n",
			SGE_Ptr->length, BurstLength + SelectTXQP->mCurrent_SQE_SGE_Offset,
			SelectTXQP->mCurrent_SQE_SGE_List_Index + 1, SQE_Ptr->num_sge);
	}

	if (SGE_Ptr->length == BurstLength + SelectTXQP->mCurrent_SQE_SGE_Offset) {
		/*
		 * SGE done
		 */
		DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
			"QP[%d]: LAST SGE TX TranConn->mSegInd %lx Offset 0x%lx BurstLenght %lx SQE In SQ %lx "
			"SGE Len %lx ->num_sge %lx, opcode %d, ->send_flags %lx ->...rkey %lx ->...remote_addr %lx\n",
			SelectTXQPIndex,
			(uint64_t)SelectTXQP->mCurrent_SQE_SGE_List_Index, (uint64_t)SelectTXQP->mCurrent_SQE_SGE_Offset,
			(uint64_t)BurstLength, (uint64_t) SQE_Ptr,
			(uint64_t)SGE_Ptr->length,
			(uint64_t)SQE_Ptr->num_sge, SQE_Ptr->opcode, (uint64_t)SQE_Ptr->flags,
			(uint64_t)SQE_Ptr->wr.rdma.rkey, (uint64_t) SQE_Ptr->wr.rdma.remote_addr);

		if (SelectTXQP->mCurrent_SQE_SGE_List_Index + 1 == SQE_Ptr->num_sge) {
			/*
			 * Last SGE completed
			 *
			 * Flag WQE completion: allows generating a CQE when this burst is acked.
			 * NOTE: Since resends are from user buffers, completion must wait for ack.
			 */
			BurstDescPtr->mLastBurstOfMessage = 1;

			/* this flag allows read completion on the other side if this was a read response */
			if (SQE_Ptr->flags & ROQ_WQE_READ_RESPONSE)
				BurstDescPtr->mIsInternalReadResponse = 1;

			DASSERT(SelectTXQP->mSendQueue_SQE_NextToSend <= RoQ_DevMem->QP_Array[SelectTXQPIndex].mSendQueue.mPut, "%s\n", "Protocol error");

			SelectTXQP->mSendWorkRequestActiveFlag = 0;

			DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX, "WQE TX FINISHED, IsReadResponse %lx\n",
				(uint64_t)BurstDescPtr->mIsInternalReadResponse);
		} else {
			/*
			 * More SGE's to process
			 */
			SelectTXQP->mCurrent_SQE_SGE_List_Index++;
			SelectTXQP->mCurrent_SQE_SGE_Offset = 0;
			SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag = 0;
		}
	} else
		SelectTXQP->mCurrent_SQE_SGE_Offset += BurstLength;

	if (opcode == ROQ_OP_WRITE)
		SelectTXQP->mCurrent_Remote_VirtAddr += BurstLength;

	if (opcode == ROQ_OP_SEND) /* XXXXXXX */
		SetupChunk->DDP_Fields.Send.MO = SelectTXQP->Current_SQE.wr.rdma.bytes;


	/*
	 * Add total bytes sent - to be reported in CQE
	 */
	SelectTXQP->Current_SQE.wr.rdma.bytes += BurstLength;

	if (BurstLength > 0) {
		/* NEED: might need to let zero len sends go through as a special case. */
		RoQ_Log16Words("TX SGE", (char *)RoQ_VirtAddr, BurstLength);

                unsigned BurstType = 0;
                if( SelectTXQP->mSendWorkRequestActiveFlag )
                	BurstType = PktT_LastBurstOfMsgOFF | PktT_SetupOFF | PktT_IB | PktT_DMA_OFF | opcode;
                else
                	BurstType = PktT_LastBurstOfMsgON  | PktT_SetupOFF | PktT_IB | PktT_DMA_OFF | opcode;

//#ifndef ROQ_RESILIENCY // roq resiliency does not support remote get yet
#define RoQ_MESSAGE_UNIT_DMA_GET_ENABLED // turn on using MU Get
//#endif


#define RoQ_MESSAGE_UNIT_DMA_GET_THRESHOLD 4*1024 // this is where DMA gets kick in. This maybe way too small. Might even need to look at msg size.
#ifdef RoQ_MESSAGE_UNIT_DMA_GET_ENABLED
		/*
		 * Modify so we send information enabling the RX side to do a BG/Q MU DMA GET
		 * rather than directly tranmitting the packets to the RX memory fifo.
		 * Just doing ROQ_OP_WRITE by DMA GET for now.
		 */
		
		if (BurstLength >= RoQ_MESSAGE_UNIT_DMA_GET_THRESHOLD && opcode == ROQ_OP_WRITE ) {
			// setup the info needed for the remote side to MU DMA GET the data
			SetupChunk->BGQ_MU_DMA_Get_Setup.mDmaAddr = RoQ_DmaAddr;
			SetupChunk->BGQ_MU_DMA_Get_Setup.mDmaLen  = BurstLength;

			/* To be enabled for debugging if dma_get freezes data source */
#if 0
			DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_HIGH), 1);
			DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_LOW), 0);
#endif

			DPRINTF(FXLOG_REMOTE_GET,
			  "(*)(*) TX DMA SETUP Local Data Addr %p Len %ld\n", RoQ_DmaAddr, BurstLength) ;
			// splatter the args to tc_TX_Burst() so that we sends the MU DMA setup info rather than the actual data
			RoQ_DmaAddr = (uint64_t) SetupChunk;
			BurstLength = sizeof *SetupChunk;

			// change the burst type
			BurstType |= PktT_DMA_ON;
		}
#endif
		tc_TX_Burst(SelectTXQP->mTranConn,
			    BurstSlot,
			    SelectTXQP->mPeerQPId,
			    BurstType,
			    RoQ_DmaAddr,
			    BurstLength);
	}
}

static void RoQ_StartTxRead(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex,
			    struct RoQ_TX_BurstDescriptor *BurstDescPtr, int BurstSlot)
{
	struct roq_sqe			*SQE_Ptr    = &SelectTXQP->Current_SQE;
	struct roq_sge			*SGE_Ptr    = &SelectTXQP->Current_SendSgList[SelectTXQP->mCurrent_SQE_SGE_List_Index];
	union RoQ_ShortSegmentPacket	*SetupPkt   = &BurstDescPtr->mSetupPacket;
	union RDMA_SetupChunk	*SetupChunk = &SetupPkt->RDMA.SetupChunk;
	int OBRQ_Index;

	/*
	 * this read works with only one contig run of virtual memory transfer from
	 * remote source to local sink. therefore, it turns into an
	 * rdma write like op on the remote side.
	 */
	DPRINTF(FXLOG_UCODE_PER_MESSAGE,
		"ROQ_OP_READ SETUP BURST SQE_SGE_Index %lx SQE_Ptr 0x%p wr_id %lx READ sg_list[0]@ 0x%p remote addr 0x%p "
		"remote key %lx local addr 0x%08x ORQ Posted %lx ORQ Retired %lx\n",
		(uint64_t)SelectTXQP->mCurrent_SQE_SGE_List_Index, (void *) SQE_Ptr, (uint64_t)SQE_Ptr->id,
		(void *) &SQE_Ptr->sge[0], (void *) SQE_Ptr->wr.rdma.remote_addr,
		(uint64_t)SQE_Ptr->wr.rdma.rkey,
		SQE_Ptr->sge[0].addr, (uint64_t)SelectTXQP->mOutboundReadQueue_Posted,
		(uint64_t)SelectTXQP->mOutboundReadQueue_Retired);

	/* NEED: to manage this value correctly */
	SelectTXQP->mOutboundReadQueue_Count = RoQ_MAX_OUTSTANDING_RDMA_READS;

	/* NEED: to be able to skip the send and come back to it later. */
	while (!((SelectTXQP->mOutboundReadQueue_Posted - SelectTXQP->mOutboundReadQueue_Retired) < SelectTXQP->mOutboundReadQueue_Count)) {
		sleep(1);
		DPRINTF(FXLOG_UCODE_PER_MESSAGE, "RoQ TX Thread: QP[%d]: waiting for ORD slot\n", SelectTXQPIndex);
		DPRINTF(1, "*** WAIT full ORQ slot %lx %lx %lx\n", (uint64_t)SelectTXQP->mOutboundReadQueue_Posted,
			(uint64_t)SelectTXQP->mOutboundReadQueue_Retired, (uint64_t)SelectTXQP->mOutboundReadQueue_Count);
	}
	OBRQ_Index = SelectTXQP->mOutboundReadQueue_Posted % SelectTXQP->mOutboundReadQueue_Count;

	/*
	 * NEED: THIS IS ACTUALLY A SRSW Q TO THE RX THREAD FOR THIS QP -- THINK OF HANDLING THAT WAY
	 * ESP THINK ABOUT MEMORY BARRIERS -- since there's an almost impossible race cond here.
	 * SelectTXQP->mOutboundReadQueue_Array[OBRQ_Index] = SQE_Ptr->id;
	 */
	/* index of the read request WQE in the Send Queue */
	SelectTXQP->mOutboundReadQueue_Array[OBRQ_Index] = SelectTXQP->mCurrent_SQE_Index;

	ppc_msync(); /* NEEDED? */
	SelectTXQP->mOutboundReadQueue_Posted++;
	ppc_msync(); /*NEEDED? */

	DASSERT(SelectTXQP-> mCurrent_SQE_SGE_Offset == 0, "%s\n", "TX Segment handling protocol error");

	SetupChunk->DDP_Fields.RDMA_Flags = SQE_Ptr->flags;
	SetupChunk->DDP_Fields.Read.SrcOffset = SQE_Ptr->wr.rdma.remote_addr;
	SetupChunk->DDP_Fields.Read.SrcSTag = SQE_Ptr->wr.rdma.rkey;
	/* NEED: to validate this sink memory area? */
	SetupChunk->DDP_Fields.Read.SinkSGE = SQE_Ptr->sge[0];

#if 0
	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "QP[%d]:: ReadRequest: laddr 0x%llx, raddr 0x%llx, len %llu, lkey 0x%x, rkey 0x%x",
		SelectTXQPIndex, SQE_Ptr->sge[0].addr, SQE_Ptr->wr.rdma.remote_addr, SQE_Ptr->sge[0].length,
		SQE_Ptr->sge[0].key, SQE_Ptr->wr.rdma.rkey);
#endif

	DPRINTF(FXLOG_UCODE_PER_MESSAGE,
		"RDMA READ SETUP DDP_Fields PeerQPId.mWhole %lx wr_id %lx OBRQ_Index %lx .RDMA_Flags %lx .SrcOffset 0x%lx "
		".SrcSTag 0x%lx .SinkOffset 0x%lx .SinkSTag 0x%lx .MessageLength %lx\n",
		(uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t)SQE_Ptr->id, (uint64_t)OBRQ_Index,
		(uint64_t)SetupChunk->DDP_Fields.RDMA_Flags, (uint64_t) SetupChunk->DDP_Fields.Read.SrcOffset,
		(uint64_t)SetupChunk->DDP_Fields.Read.SrcSTag, (uint64_t) SetupChunk->DDP_Fields.Read.SinkSGE.addr,
		(uint64_t)SetupChunk->DDP_Fields.Read.SinkSGE.key, (uint64_t)SetupChunk->DDP_Fields.Read.SinkSGE.length);

	BurstDescPtr->mLastBurstOfMessage = 1;
	BurstDescPtr->mIsRdmaReadRequest  = 1;

	/*
	 * here fill out the alignment chunks so the rest
	 * of the transfer can be aligned with the help of the MU
	 */
	RoQ_Log16Words("TX RDMA READ SETUP", (char *)SetupPkt, sizeof *SetupChunk);

	unsigned BurstType = 0 | PktT_LastBurstOfMsgON | PktT_SetupON | PktT_IB | PktT_DMA_OFF | ROQ_OP_READ;

	tc_TX_Burst(SelectTXQP->mTranConn,
		    BurstSlot,
		    SelectTXQP->mPeerQPId,
		    BurstType,
		    (uint64_t)SetupPkt,
		    sizeof *SetupChunk);
	/*
	 * read request is done here -- signal
	 * that another work request may be begun.
	 */
	SelectTXQP->mSendWorkRequestActiveFlag = 0;
}

/*
 * NEED: on first, send the setup, then keep track,
 * then notice the end and mark the TX_BurstDescriptor_t so that a completion will be perfomed on ACK
 *
 * NEED NOTE: if the setup frame has alginment info / fix up data in it, we may need one per SGE
 */
static void RoQ_StartTxWrite(struct RoQ_DevicePrivatePerQPState *SelectTXQP, int SelectTXQPIndex,
			     struct RoQ_TX_BurstDescriptor *BurstDescPtr, int BurstSlot)
{
	struct roq_sqe			*SQE_Ptr    = &SelectTXQP->Current_SQE;
	struct roq_sge			*SGE_Ptr    = &SelectTXQP->Current_SendSgList[SelectTXQP->mCurrent_SQE_SGE_List_Index];
	union RoQ_ShortSegmentPacket	*SetupPkt   = &BurstDescPtr->mSetupPacket;
	union RDMA_SetupChunk		*SetupChunk = &SetupPkt->RDMA.SetupChunk;

	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "%s\n", "TX case (PktT_SetupON | PktT_IB | ROQ_OP_WRITE)");
	/*
	 * really looks like the setup chunk could be part of the work request itself
	 * but that wouldn't help get the aligment info chunks in
	 */
	if (!SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag) {
		/*  this is for the setup chunk -- may be incremented below */
		uint32_t BurstLength = sizeof *SetupChunk;

		DPRINTF(FXLOG_UCODE_PER_MESSAGE, "%s\n", "TX WRITE SETUP RDMA_WRITE SETUP BURST");

		DASSERT(SelectTXQP->mCurrent_SQE_SGE_Offset == 0, "%s\n", "TX Segment handling protocol error");

		/* Pretty sure this wants to auto wrap Posted into 32 bis of MSN */
		SetupChunk->DDP_Fields.RDMA_Flags           = SQE_Ptr->flags;
      		SetupChunk->DDP_Fields.Write.SinkSGE.addr   = SQE_Ptr->wr.rdma.remote_addr;
		SetupChunk->DDP_Fields.Write.SinkSGE.key    = SQE_Ptr->wr.rdma.rkey;
		SetupChunk->DDP_Fields.Write.SinkSGE.length = SGE_Ptr->length;
		/*
		 * NEED: segment lengths are a little more difficult to work out that this
		 * NEED: pull missaligned ends of SG element into this burst so the remainder bulk path can assume alignment
		 */
		DPRINTF(FXLOG_UCODE_PER_MESSAGE,
			"TX WRITE SETUP PeerQPId.mWhole %lx RDMA SETUP DDP_Fields ->DDP_Fields.RDMA_Flags 0x%lx ->DDP_Fields.Write.SinkSGE.addr 0x%lx "
			"->DDP_Fields.Write.SinkSGE.key 0x%lx ->DDP_Fields.MessageLength %lx, \n",
			(uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SetupChunk->DDP_Fields.RDMA_Flags,
			(uint64_t) SetupChunk->DDP_Fields.Write.SinkSGE.addr, (uint64_t) SetupChunk->DDP_Fields.Write.SinkSGE.key,
			(uint64_t)SetupChunk->DDP_Fields.Write.SinkSGE.length);

		DASSERT(sizeof(union RDMA_SetupChunk) == 32, "setup chunck not chunks sized (32) but rather: %lx sizeof(roq_sge) %lx\n",
			(uint64_t)sizeof(union RDMA_SetupChunk), (uint64_t)sizeof(struct roq_sge));

		/* here fill out the alignment chunks so the rest of the transfer can be be  aligned with the help of the MU */
		RoQ_Log16Words("TX RDMA WRITE SETUP", (char*)SetupPkt, BurstLength);

#ifdef RoQ_SEND_USER_DATA_IN_SETUP
		/*
		 * can we finish in
		 * NOTE: setup size can be zero
		 */
		if (SQE_Ptr->num_sge == 1 && SGE_Ptr->length <= RoQ_USER_DATA_IN_SETUP_SIZE) {

			DPRINTF(FXLOG_UCODE_PER_BURST, "TX SHORT WRITE SETUP PeerQPId.mWhole %lx len %lx\n",
				(uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t)SGE_Ptr->length);
			/*
			 * The receive side will take care of placing this data even if it spans physical pages.
			 * The send side needs to pick up the data even if it spans pages.
			 * It is assumed: int len1 = SGE_Ptr[0].length;
			 */

			if (SGE_Ptr->length) {
				int      virtual_page_a  = SGE_Ptr->addr & ~RoQ_BURST_MASK;
				uint64_t MsgLastByteAddr = SGE_Ptr->addr + SGE_Ptr->length - 1;
				int      virtual_page_b  = MsgLastByteAddr & ~RoQ_BURST_MASK;
				uint64_t RoQ_DmaAddr, RoQ_VirtAddr;

				if (virtual_page_a == virtual_page_b) {
					/* short write does not span page boundary */
					RoQ_DmaAddr = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr, SGE_Ptr->length, __LINE__);
					RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

					DPRINTF(FXLOG_UCODE_PER_BURST, "TX SHORT WRITE ACCESSING DATA qpn %lx PeerQPId.mWhole %lx stag %lx sge.addr 0x%lx sge.paddr 0x%lx len %lx\n",
						(uint64_t)SelectTXQP->mQP_Num, (uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SGE_Ptr->key,
						(uint64_t) SGE_Ptr->addr, (uint64_t) RoQ_DmaAddr, (uint64_t)SGE_Ptr->length);

					memcpy((void *)SetupPkt->RDMA.Payload, (void *) RoQ_VirtAddr, SGE_Ptr->length);

					RoQ_Log16Words("TX SHORT WRITE DATA", (char*)SetupPkt->RDMA.Payload, SGE_Ptr->length);
				} else {
					/* short write spans a page boundary and will require two page lookup/memcpy calls */
					uint32_t len2 = (MsgLastByteAddr & RoQ_BURST_MASK) + 1;
					uint32_t len1 = SGE_Ptr->length - len2;

					/* NEED: This assert is probably not required and should probably come out for  per reasons */
					DASSERT((SGE_Ptr->length == (len1+len2)), "%lx %lx %lx %lx %lx \n",
						SGE_Ptr->key, SGE_Ptr->addr, SGE_Ptr->length, len1, len2);

					RoQ_DmaAddr  = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr, len1, __LINE__);
					RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

					memcpy((void *) &SetupPkt->RDMA.Payload[0], (void *) RoQ_VirtAddr, len1);

					DASSERT(((((uint64_t)(SGE_Ptr->addr) + len1) & RoQ_BURST_MASK) == 0), "%lx %lx\n", SGE_Ptr->addr, len1);

					RoQ_DmaAddr  = RoQ_HostVa2DmaAddr(SGE_Ptr->key, SGE_Ptr->addr + len1, len2, __LINE__);
					RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr(RoQ_DmaAddr);

					memcpy((void *) (((char*)&SetupPkt->RDMA.Payload[0])+len1), (void *) RoQ_VirtAddr, len2);

					DPRINTF(FXLOG_UCODE_PER_BURST,
						"TX SHORT WRITE ACCESSING DATA (TWO MEMCPY) qpn %lx PeerQPId.mWhole %lx stag %lx sge.addr 0x %lx sge.length %lx DmaAddr 0x%lx\n",
						(uint64_t)SelectTXQP->mQP_Num, (uint64_t)SelectTXQP->mPeerQPId.mWhole, (uint64_t) SGE_Ptr->key,
						(uint64_t) SGE_Ptr->addr, (uint64_t)SGE_Ptr->length, (uint64_t) RoQ_DmaAddr);
				}
				BurstLength += SGE_Ptr->length;
			}
			/*
			 * NOTE: this flag is what will allow generating a user completion when this burst is acked.
			 * NOTE: Since resends are from user buffers, completion must wait for ack!
			 */
			BurstDescPtr->mLastBurstOfMessage = 1;

			/* this flag allows read completion on the other side if this was a read response */
			if (SelectTXQP->Current_SQE.flags & ROQ_WQE_READ_RESPONSE)
				BurstDescPtr->mIsInternalReadResponse = 1;

			DASSERT(SelectTXQP->mSendQueue_SQE_NextToSend <= RoQ_DevMem->QP_Array[SelectTXQPIndex].mSendQueue.mPut,
				"%s\n", "Protocol error");

			SelectTXQP->mSendWorkRequestActiveFlag = 0;


			DPRINTF(FXLOG_UCODE_PER_MESSAGE || FXLOG_UCODE_TX,
				"TX SHORT WRITE FINISHED WR TX PACKET DATA LOAD IsReadResponse %lx BurstLength %lx sge length %lx CurSentSetup %lx\n",
				(uint64_t)BurstDescPtr->mIsInternalReadResponse, (uint64_t)BurstLength, (uint64_t)SGE_Ptr->length,
				(uint64_t)SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag);
		}
#endif
		unsigned BurstType = 0;
		if (SelectTXQP->mSendWorkRequestActiveFlag) {
			SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag = 1;
			/* bmt    SelectTXQP->mCurrent_SQE_SGE_List_Index    = 0; */
			SelectTXQP->mCurrent_SQE_SGE_Offset        = 0;
			/* bmt   if (SelectTXQP->mCurrent_SQE_SGE_List_Index == 0) */
			SelectTXQP->mCurrent_Remote_VirtAddr       = SetupChunk->DDP_Fields.Write.SinkSGE.addr ;
			BurstType = 0 | PktT_LastBurstOfMsgOFF | PktT_SetupON | PktT_IB | PktT_DMA_OFF | ROQ_OP_WRITE;
		} else {
			BurstType = 0 | PktT_LastBurstOfMsgON  | PktT_SetupON | PktT_IB | PktT_DMA_OFF | ROQ_OP_WRITE;
		}

		tc_TX_Burst(SelectTXQP->mTranConn,
			    BurstSlot,
			    SelectTXQP->mPeerQPId,
			    BurstType,
			    (uint64_t)SetupPkt,
			    BurstLength);
	}
}

/* Only one reader */
static inline union roq_tx_db_req RoQ_Get_Active_QP(int *is_read)
{
	static int read_db_index = 0;
	static int send_db_index = 0;
	union roq_tx_db_req req;

	/* We promote the ReadQ */
	req = _load_shared(RoQ_RQ_DB[read_db_index]);
	if (req.item) {
		_store_shared(RoQ_RQ_DB[read_db_index++].item, 0);
		read_db_index %= ROQ_RQ_DB_SIZE;
		*is_read = 1;
	} else {
		req = _load_shared(RoQ_SQ_DB[send_db_index]);
		if (req.item) {
			_store_shared(RoQ_SQ_DB[send_db_index++].item, 0);
			send_db_index %= ROQ_SQ_DB_SIZE;
		} else if (num_user_db) {
			/* Handle any user mapped DB */
			static int user_db = 0;
			static int num_db_last = 0;
			int i = 0;
			if (num_db_last != num_user_db) {
				/* restart if changed number of DB's */
				user_db = 0;
				num_db_last = num_user_db;
			}
			while (i++ < num_user_db) {
				union roq_tx_db_req *u_req;
				u_req = RoQ_SQ_USER_DB[user_db];
				if (u_req) {
					req = _load_shared(u_req[user_db_get[user_db]]);
					if (req.item) {
						_store_shared(u_req[user_db_get[user_db]++].item, 0);
						user_db_get[user_db] %= ROQ_DB_ARRAY_SIZE;
					}
				}
				if (++user_db == num_user_db)
					user_db = 0;
				if (req.item)
					break;
			}
		}
	}
	return req;
}

/*
 * RoQ_TX()
 *
 * There are two ways in to this function:
 * Completing WRITE/SEND RDMA ops and completing READ rdma ops.
 */
void *RoQ_TX(void *args)
{
	struct RoQ_TX_Arguments *TxArgs = (struct RoQ_TX_Arguments *)args;

	/* Assume at most one process per core */
	int	TxThreadNumber = TxArgs->mTxThreadNumber;
	int	ActiveTXQP_RingHead = -1,
		SelectTXQPIndex = -1;

	uint32_t ActiveConnections = 0;

	ROQ_set_affinity(15);

	DPRINTF(FXLOG_UCODE_INIT, "args %p TxArgs->mTxThreadNumber %lx TxArgs->mPortId %lx\n",
		(void *) args, (uint64_t)TxArgs->mTxThreadNumber, (uint64_t)TxArgs->mPortId);

//#define RANDOMIZE_TX_RING
#ifdef RANDOMIZE_TX_RING
        int PartitionSize = RoQ_NetworkSize(&RoQ_NetworkDesc);
#endif

	
	for (;;) {
		struct RoQ_DevicePrivatePerQPState	*SelectTXQP;
		struct RoQ_DM_QP			*SelectTXQP_OS;   /* NOTE: probably shouldn't need this access in the long run */
		struct RoQ_TransportConnection		*TranConn;
		uint32_t				Doorbell_QP_Index;
		int					DoorbellEvents;

		/* wait here ... discovery mode */
		while (RoQ_discovery_act == 1)
			(void)RoQ_CntlMsgPoll();

		/* poll the control message queue */
		Doorbell_QP_Index = RoQ_CntlMsgPoll();
		if (Doorbell_QP_Index) {
			/* Must send Terminate Message to peer as next message */
			if (Doorbell_QP_Index <= RoQ_MAX_QP) {
				SelectTXQP = &RoQ_DevicePrivatePerQPState[Doorbell_QP_Index];
				SelectTXQP->state_flags |= ROQ_SEND_TERMINATE;
			} else {
				DPRINTF(1, "TX:: Control QP index out of bounds\n", SelectTXQPIndex);
				continue;
			}
		}

		while (RoQ_SQ_DB != NULL) {
			struct RoQ_DM_QP			*ConnPtr;
			struct RoQ_DevicePrivatePerQPState	*PerQP;
			int					is_read = 0,
								num_wqe = 0;

			/*
			 * Start/resume regular SQ processing if no control must be sent
			 */
			if (!Doorbell_QP_Index) {
				union roq_tx_db_req req = RoQ_Get_Active_QP(&is_read);

				Doorbell_QP_Index = req.qp_id;
				if (Doorbell_QP_Index) {
					if (!is_read) {
						num_wqe = req.num_wqe;

						/* Correct for potential application error */
						if (!num_wqe) {
							DPRINTF(1, "QP[%d]: User TX error: Active QP with no WQE\n", Doorbell_QP_Index);
							Doorbell_QP_Index = 0;
						}
					}
				}
			}

			if (!Doorbell_QP_Index)
				break;

			if (Doorbell_QP_Index > RoQ_MAX_QP) {
				DPRINTF(1, "TX Stop:: BAD QP Index %d, #user DB's: %d\n", Doorbell_QP_Index, num_user_db);
				while (1)
					sleep (1);
				break;
			}
			ConnPtr = &RoQ_DevMem->QP_Array[Doorbell_QP_Index];
			PerQP   = &RoQ_DevicePrivatePerQPState[Doorbell_QP_Index];

			if (PerQP->state_flags & ROQ_TX_SUSPENDED && !is_read) {
				/* Allow Read Responses still to be processed */
				Doorbell_QP_Index = 0;
				continue;
			}

 			ConnPtr->mSendQueue.mPut += num_wqe;

			/*
			 * Doorbell can happen after shift to closing
			 * StrongAssertLogLine(PerQP->mState == ROQ_QPS_RTS) <<"TX Doorbell on QP not in ROQ_QPS_RTS state."<< Doorbell_QP_Index << EndLogLine;
			 * NOTE: we let SQD in here because we send a disconnect packet triggered below by that state
			 */
			if (PerQP->mState == ROQ_QPS_RTS || PerQP->mState == ROQ_QPS_SQD || (PerQP->mState & ROQ_SEND_TERMINATE)) {
				/*
				 * If this connection is not in the active TX ring... put it in.
				 */
				if (PerQP->mNextIndex != -1) {
					DASSERT(ActiveConnections > 0, "%d", ActiveConnections);
					/*
					 * QP is already in the ring.
					 */
				} else {
					/*
					 * qp needs to be put in the ring
					 */
					DASSERT(PerQP->mSendWorkRequestActiveFlag == 0, "%x", PerQP->mSendWorkRequestActiveFlag);
#if defined(PKTRACE_TXACTIVE)
        //needs to go elsewhere HitOE(TxMessageStart, "TxMessage",0) ;
					HitOE1(TransmitActiveStart,"TransmitActive_QPId=%d",0,(uint32_t)Doorbell_QP_Index) ;
#endif
					/*
					 * put it before the last entry
					*/
					if (ActiveTXQP_RingHead == -1) {

						DASSERT(ActiveConnections == 0, "%d", ActiveConnections);

						ActiveTXQP_RingHead = Doorbell_QP_Index;
						PerQP->mNextIndex   = Doorbell_QP_Index;
						PerQP->mPrevIndex   = Doorbell_QP_Index;
						SelectTXQPIndex     = Doorbell_QP_Index;

						DPRINTF(FXLOG_UCODE_PER_MESSAGE, "qpn %lx Inserted into empty ring\n", (uint64_t)Doorbell_QP_Index);
#if defined(PKTRACE_TXTHREADACTIVE)
						HitOE(TransmitThreadActiveStart,"TransmitThreadActive",0) ;
#endif
					} else {
						/*
						 * insert into the ring --
						 * get additional pointers to qp's affected and then stich in the new
						 */
#ifdef RANDOMIZE_TX_RING  // Here we randomize where the qp is inserted since w/o this we're getting an ordered list
// NOTE THIS WHOLE PATH IS LATENCY!!!!
                                                int Save_ActiveTXQP_RingHead = ActiveTXQP_RingHead;
						int i;
                                                struct RoQ_DevicePrivatePerQPState *RingPerQPState;
						int Skips = (PerQP->mTranConn->mPeerRank + ActiveConnections) % 11;

						for (i = 0; i < Skips; i++ ) {
							RingPerQPState = &RoQ_DevicePrivatePerQPState[ActiveTXQP_RingHead];
							ActiveTXQP_RingHead = RingPerQPState->mNextIndex;
						}
#endif
						struct RoQ_DevicePrivatePerQPState *RingHeadPerQP     = &RoQ_DevicePrivatePerQPState[ActiveTXQP_RingHead];
						struct RoQ_DevicePrivatePerQPState *RingHeadPrevPerQP = &RoQ_DevicePrivatePerQPState[RingHeadPerQP->mPrevIndex];

						DASSERT(ActiveTXQP_RingHead > 0 && ActiveTXQP_RingHead < RoQ_MAX_QP, "%d %d", ActiveTXQP_RingHead, RoQ_MAX_QP);
						DASSERT(SelectTXQPIndex > 0 && SelectTXQPIndex < RoQ_MAX_QP, "%d %d", SelectTXQPIndex, RoQ_MAX_QP);
						DASSERT(ActiveConnections > 0, "%d", ActiveConnections);

						PerQP->mNextIndex             = ActiveTXQP_RingHead;
						PerQP->mPrevIndex             = RingHeadPerQP->mPrevIndex;
						RingHeadPrevPerQP->mNextIndex = Doorbell_QP_Index;
						RingHeadPerQP->mPrevIndex     = Doorbell_QP_Index;
#ifdef RANDOMIZE_TX_RING
// This favors fairness at the expense of latency for short messages
						ActiveTXQP_RingHead = Save_ActiveTXQP_RingHead;
#else
// This would favor the newly inserted item but mess with fairness of service
						ActiveTXQP_RingHead           = Doorbell_QP_Index;
#endif
						/*
						 * NOTE: QP has been inserted into ring and it becomes the
						 * RingHead but must wait it's turn to be come the SelectTXQPIndex
						 */
					}
					ActiveConnections++;
				}
			}
			Doorbell_QP_Index = 0;
		}

		/* poll ack fifo */
		RoQ_ProcessAck(TxArgs);

		/*
		 * Do we have an active QP?
		 */
		if (ActiveTXQP_RingHead == -1)
			continue;

		/*
		 * Do we have room to inject into the hardware fifo - expect yes.
		 * this could be checked a few cycles later maybe
		 */
		if (RoQ_InjectionFifoFull())
			continue;

		DASSERT(SelectTXQPIndex != -1, "%x", SelectTXQPIndex);

		DPRINTF(FXLOG_UCODE_PER_ITER, "Working on SelectTXQPIndex %lx ActiveTXQP_RingHead %lx\n",
			(uint64_t)SelectTXQPIndex, (uint64_t)ActiveTXQP_RingHead);

		SelectTXQP = &RoQ_DevicePrivatePerQPState[SelectTXQPIndex];
		SelectTXQP_OS = &RoQ_DevMem->QP_Array[SelectTXQPIndex];

		DASSERT(SelectTXQP, "%p", SelectTXQP);

		DPRINTF(FXLOG_UCODE_PER_ITER, "Working on SelectTXQPIndex %lx SelectTXQP@ 0x%p SelectTXQP_OS@ 0x%p\n",
			(uint64_t)SelectTXQPIndex, (void *)SelectTXQP, (void *)SelectTXQP_OS);

		DPRINTF(FXLOG_UCODE_PER_ITER,
			"qpn %lx SelectTXQP@ 0x%p mPeerQPId 0x%lx TranConn@ 0x%p ->mInboundReadQueue_Posted %lx SelectTXQP->mInboundReadQueue_Retired %lx "
			"->mInboundReadQueue_Count %lx ->mSendQueue_SQE_NextToSend %lx ->mSendQueue.mPut %lx\n",
			(uint64_t)SelectTXQPIndex, (void *) SelectTXQP, SelectTXQP->mPeerQPId.mWhole, (void *) SelectTXQP->mTranConn,
			(uint64_t)SelectTXQP->mInboundReadQueue_Posted, (uint64_t)SelectTXQP->mInboundReadQueue_Retired,
			(uint64_t)SelectTXQP->mInboundReadQueue_Count, (uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend,
			(uint64_t)SelectTXQP_OS->mSendQueue.mPut);

		/*
		 * this qp is in the ring but not active -- need to get a work request ready
		 */
		if (SelectTXQP->mSendWorkRequestActiveFlag) {

			DPRINTF(FXLOG_UCODE_PER_ITER,
				"TX ACTIVE ON WR qpn %lx qp->SQE_SGE Index %lx qp->SQE_SGE_Offest %lx SelectTXQP->mInboundReadQueue_Retired %lx "
				"->mInboundReadQueue_Posted %lx ->mSendQueue_SQE_NextToSend %lx ->mSendQueue.mPut %lx\n",
				(uint64_t)SelectTXQPIndex, (uint64_t)SelectTXQP->mCurrent_SQE_SGE_List_Index,
				(uint64_t)SelectTXQP->mCurrent_SQE_SGE_Offset, (uint64_t)SelectTXQP->mInboundReadQueue_Retired,
				(uint64_t)SelectTXQP->mInboundReadQueue_Posted, (uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend,
				(uint64_t)SelectTXQP_OS->mSendQueue.mPut);
		} else {
			if (SelectTXQP->mState == ROQ_QPS_RTS) {
				/*
				 * Actually this is seen during an error shut down
				 */
				DASSERT(SelectTXQP->mPeerQPId.mWhole,
					"qp->mPeerQPId is 0 zero ... not allowed qpn %lx SelectTXQP@ 0x%p mPeerQPId 0x%x TranConn@ "
					"0x%p ->mInboundReadQueue_Posted %lx SelectTXQP->mInboundReadQueue_Retired %lx "
					"->mInboundReadQueue_Count %lx ->mSendQueue_SQE_NextToSend %lx ->mSendQueue.mPut %lx\n",
					(uint64_t)SelectTXQPIndex, (void *) SelectTXQP, SelectTXQP->mPeerQPId.mWhole,
					(void *) SelectTXQP->mTranConn, (uint64_t)SelectTXQP->mInboundReadQueue_Posted,
					(uint64_t)SelectTXQP->mInboundReadQueue_Retired, (uint64_t)SelectTXQP->mInboundReadQueue_Count,
					(uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend, (uint64_t)SelectTXQP_OS->mSendQueue.mPut);
			}
			/*
			 * Prepare for a new transmission
			 *
			 * NOTE: the order of the following if block prefers READs to localy generated WRs
			 */
			if (SelectTXQP->mInboundReadQueue_Retired < SelectTXQP->mInboundReadQueue_Ready) {
				/*
				 * Also serves READ requests which are already pending when user reqeuested TERMINATE
				 */
				if (RoQ_PrepareReadReply(SelectTXQP, SelectTXQPIndex) < 0)
					continue;

			} else if (SelectTXQP->state_flags & ROQ_SEND_TERMINATE) {

				RoQ_EnterReset(SelectTXQP, SelectTXQPIndex);
				SelectTXQP->state_flags &= ~ROQ_SEND_TERMINATE;

			} else if (SelectTXQP->mSendQueue_SQE_NextToSend < _load_shared(SelectTXQP_OS->mSendQueue.mPut)) {

				if (RoQ_PrepareTxSQ(SelectTXQP, SelectTXQP_OS, SelectTXQPIndex) < 0) {
					roq_post_net_event(NET_EVENT_SQ_ERROR, &SelectTXQPIndex, 0);
					RoQ_EnterReset(SelectTXQP, SelectTXQPIndex);
				}
			} else {
				/*
				 * take this conn out of the active ring
				 */
				ActiveConnections--;

				DPRINTF(FXLOG_UCODE_PER_MESSAGE,
					"TX DEACTIVATE qpn %lx SelectTXQP->mSendQueue_SQE_NextToSend %lx ->mSendQueue.mPut "
					"%lx ->mSendQueue.mGot %lx SelectTXQP->mInboundReadQueue_Retired %lx "
					"->mInboundReadQueue_Posted %lx\n",
					(uint64_t)SelectTXQPIndex, (uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend,
					(uint64_t)SelectTXQP_OS->mSendQueue.mPut, (uint64_t)SelectTXQP_OS->mSendQueue.mGot,
					(uint64_t)SelectTXQP->mInboundReadQueue_Retired, (uint64_t)SelectTXQP->mInboundReadQueue_Posted);

#if defined(PKTRACE_TXACTIVE)
					HitOE1(TransmitActiveFinis,"TransmitActive_QPId=%d",0,(uint32_t)SelectTXQPIndex) ;
#endif
				if (ActiveConnections == 0) {
					ActiveTXQP_RingHead = -1;
					SelectTXQPIndex = -1; /* this may not be required*/
#if defined(PKTRACE_TXTHREADACTIVE)
					HitOE(TransmitThreadActiveFinis,"TransmitThreadActive",0) ;
#endif
				} else {
					/*
					 * not yanking the last out
					 * if the ring head is getting yanked out, move the ringhead to next
					 */

					if (ActiveTXQP_RingHead == SelectTXQPIndex) {
						struct RoQ_DevicePrivatePerQPState *RingHeadPerQP = &RoQ_DevicePrivatePerQPState[ActiveTXQP_RingHead];

						DASSERT(ActiveTXQP_RingHead != RingHeadPerQP->mNextIndex, "%lx %lx %lx\n",
							(uint64_t)ActiveTXQP_RingHead, (uint64_t)SelectTXQPIndex, (uint64_t)ActiveConnections);

						ActiveTXQP_RingHead = RingHeadPerQP->mNextIndex;
					}
					/*
					 *  pull out the select QP
					 */
					RoQ_DevicePrivatePerQPState[SelectTXQP->mPrevIndex].mNextIndex = SelectTXQP->mNextIndex;
					RoQ_DevicePrivatePerQPState[SelectTXQP->mNextIndex].mPrevIndex = SelectTXQP->mPrevIndex;
					/*
					 * advance select indext to next
					 */
					SelectTXQPIndex = SelectTXQP->mNextIndex;
				}
				/* NOTE: these values used to tell if QP is in ring and therefore active */
				SelectTXQP->mNextIndex = -1;
				SelectTXQP->mPrevIndex = -1;
				SelectTXQP = NULL;
				/*
				 * go to the top of the loop and look for more send work
				 * Note - nothing is active after this.
				 */
				continue;
			}
			SelectTXQP->mSendWorkRequestActiveFlag = 1;
#if defined(PKTRACE_TXMESSAGEACTIVE)
			HitOE2(TransmitMessageActiveStart,"TransmitMessageActive_QPId=%d_opcode=%d",0,SelectTXQPIndex, SelectTXQP->Current_SQE.opcode);
#endif
		}

		DPRINTF(FXLOG_UCODE_PER_ITER,
			"Have Active QP qpn %lx SelectTXQP@ 0x%p SelectTXQP->mPeerQPId.mWhole %lx ActiveConnections %lx\n",
			(uint64_t)SelectTXQPIndex, (void *)SelectTXQP, SelectTXQP->mPeerQPId.mWhole, (uint64_t)ActiveConnections);

		DASSERT(SelectTXQP->mSendWorkRequestActiveFlag,
			"QP must be active to get here or we got an error qpn %lx\n",
			(uint64_t)SelectTXQPIndex);
		/*
		 * get out the pointer to the transport connection -- this is static based on peer
		 * for now we're going to poll active conns against tran conns for ability to send...
		 * this match could be done better
		 */
		DASSERT(SelectTXQP->mTranConn, "%p", SelectTXQP->mTranConn);

		TranConn = SelectTXQP->mTranConn;

		/* can the TranConn support another outstanding burst sent? */
		if (TranConn->mBurstSeqNo_NextToSend - TranConn->mBurstSeqNo_NextExpectedAck == RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT) {
			/*
			 * connection (qp) cannot support more outstanding bursts -- continue
			 */
			DPRINTF(TranConn->mStallCount == STALL_REP_COUNT,
				"START STALL TranConn Selected TranConn@ 0x%p ActiveConnections %lx "
				"mBurstSeqNo_NextToSend %lx mBurstSeqNo_NextExpectedAck %lx "
				"qpn %lx SelectTXQP@ 0x%p NextToSend %lx mCurrent_SQE_Index %lx "
				"WRActiveFlag %lx wr.opcode %lx\n",
				(void *)TranConn, (uint64_t)ActiveConnections, (uint64_t)TranConn->mBurstSeqNo_NextToSend,
				(uint64_t)TranConn->mBurstSeqNo_NextExpectedAck, (uint64_t)SelectTXQPIndex, (void *) SelectTXQP,
				(uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend, (uint64_t)SelectTXQP->mCurrent_SQE_Index,
				(uint64_t)SelectTXQP->mSendWorkRequestActiveFlag,
				(uint64_t)SelectTXQP->Current_SQE.opcode);

			TranConn->mStallCount++;
		} else if ( SelectTXQP->mTxBurstsInProgress >= 32 ) {

		} else {
			/*
			 * Start or resume data transmission, or send CTL information
			 */
			enum roq_opcode	opcode = SelectTXQP->Current_SQE.opcode;

			int		BurstSlot = TranConn->mBurstSeqNo_NextToSend & RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK,
					BurstType = 0;

			struct RoQ_TX_BurstDescriptor *BurstDescPtr = &TranConn->mTX_BurstDescriptorArray[BurstSlot];

			if (TranConn->mStallCount) {
				DPRINTF(TranConn->mStallCount > STALL_REP_COUNT,
					"END STALL TranConn iters %lx Selected TranConn@ 0x%p ActiveConnections %lx "
					"mBurstSeqNo_NextToSend %lx mBurstSeqNo_NextExpectedAck %lx qpn %lx "
					"SelectTXQP@ 0x%p NextToSend %lx mCurrent_SQE_Index %lx WRActiveFlag %lx opcode %lx\n",
					(uint64_t)TranConn->mStallCount, (void *)TranConn, (uint64_t)ActiveConnections,
					(uint64_t)TranConn->mBurstSeqNo_NextToSend, (uint64_t)TranConn->mBurstSeqNo_NextExpectedAck,
					(uint64_t)SelectTXQPIndex, (void *) SelectTXQP, (uint64_t)SelectTXQP->mSendQueue_SQE_NextToSend,
					(uint64_t)SelectTXQP->mCurrent_SQE_Index, (uint64_t)SelectTXQP->mSendWorkRequestActiveFlag,
					(uint64_t)opcode);

				TranConn->mStallCount = 0;
			}
			/* this segment will be tranfering data for this connection (qp or ip frame) */
			BurstDescPtr->mQP_Index = SelectTXQPIndex;
			BurstDescPtr->mFlags = 0;
			BurstDescPtr->mWorkRequestIndex = SelectTXQP->mCurrent_SQE_Index;

			if (opcode != ROQ_OP_CTL_CLOSE) {
				PktT_SetIB(BurstType);
				PktT_SetOp(BurstType, opcode);

				if (!SelectTXQP->mCurrent_SQE_SGE_SentSetupFlag)
					PktT_SetSetup(BurstType);
			} else {
				/*
				 * put a CNTL Burst into the stream
				 */
				DPRINTF(FXLOG_UCODE_PER_BURST, "%s\n", "TX CNTL BURST FOR DISCONNECT");

				PktT_SetSetup(BurstType);
				PktT_SetCNTL(BurstType);
				PktT_SetOp(BurstType, 0);  /* op zero ... but could be a CNTL type */
			}
#if defined(PKTRACE_ON)
#if defined(PKTRACE_MESSAGE)
			if (BurstType & PktT_SetupON)
			  {
			    HitOE2(MessageStart,"QPId=%d_opcode=%d",0, SelectTXQPIndex,opcode ) ;
			  }
#endif
#endif
			switch (BurstType) {

			case (PktT_SetupON  | PktT_CNTL | 0):
				RoQ_TxDisconnect(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot);
				break;

			case (PktT_SetupON | PktT_IB | ROQ_OP_READ):
				RoQ_StartTxRead(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot);
				break;

			case (PktT_SetupON | PktT_IB | ROQ_OP_SEND):
				RoQ_StartTxSend(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot);
				break;

			case (PktT_SetupOFF | PktT_IB | ROQ_OP_SEND):
				RoQ_ResumeTxSGE(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot, ROQ_OP_SEND);
				break;

			case (PktT_SetupON | PktT_IB | ROQ_OP_WRITE):
				RoQ_StartTxWrite(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot);
				break;

			case (PktT_SetupOFF | PktT_IB | ROQ_OP_WRITE):
				RoQ_ResumeTxSGE(SelectTXQP, SelectTXQPIndex, BurstDescPtr, BurstSlot, ROQ_OP_WRITE);
				break;

			default:
				DSTRONG_ASSERT(0, "SQE (Send Queue Element) with BAD WR.OPCODE!!! "
					       "BurstType 0x%lx SelectTXQP@ 0x%lx WorkReqActive %lx opcode %lx\n",
					       BurstType, (uint64_t)SelectTXQP,
					       (uint64_t)SelectTXQP->mSendWorkRequestActiveFlag,
					       opcode);
				break;
			}
			SelectTXQP->mTxBurstsInProgress++; // track number of outstanding bursts in progress on this connection
//DASSERT( AckedPerQP->mTxBurstsInProgress <= 128, "AckedPerQP->mTxBurstsInProgress > 128 ... not good\n");
//			SelectTXQP->mSendWorkRequestActiveFlag = 1;
#if defined(PKTRACE_TXMESSAGEACTIVE)
			HitOE2(TransmitMessageActiveFinis,"TransmitMessageActive_QPId=%d_opcode=%d",0,SelectTXQPIndex,opcode) ;
#endif
#if defined(PKTRACE_ON)
#if defined(PKTRACE_MESSAGE)
			if (! SelectTXQP->mSendWorkRequestActiveFlag) {
				HitOE2(MessageFinis,"QPId=%d_opcode=%d",0, SelectTXQPIndex,opcode ) ;
			}
#endif
#endif
			DPRINTF(FXLOG_UCODE_PER_BURST, "ThrNum %lx TX Loop finished WorkReqActiveFlag %lx\n",
				(uint64_t)TxThreadNumber, (uint64_t)SelectTXQP->mSendWorkRequestActiveFlag);
		}
		// THIS LINE IS REQUIRED TO ROUND ROBIN BURSTS WITH MULTIPLE CONNECTIONS ACTIVE IN TX
		// IT SEEMS TO HAVE GONE MISSING TILL NOW -- NEED TO DOUBLE CHECK THAT THIS IS THE RIGHT PLACE TO DO THIS
		SelectTXQPIndex = SelectTXQP->mNextIndex;
	}
	return NULL;
}
