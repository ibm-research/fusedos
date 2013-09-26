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

////////// .hpp here ////
#ifndef __ROQ_FW_H__
#define __ROQ_FW_H__

/*** INCLUDES ******************************************************/
#ifndef __KERNEL__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <asm/types.h>
#include <signal.h>
#include <roq_debug.h>
#endif /* !__KERNEL__ */

#if !defined(USERSPACE_ROQ)
#if !defined(__KERNEL__)
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <spi/include/kernel/firmware/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/process.h>
#include <spi/include/kernel/location.h>
#include <spi/include/kernel/firmware/location_impl.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/firmware/memory_impl.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/kernel/firmware/MU_impl.h>

#endif /* !__KERNEL__ */
#endif /* !USERSPACE_ROQ */

#include <roq_device_interface.h>
#include <roq_common.h>

/**
 * Key Concepts
 *
 * page            A page is the unit of virtual memory management used by the host.
 *
 * burst           A set of packets that are sent from a single continious region of physical
 *                 memory and which are received into one or two continious regions of memory.
 *                 A burst transfers upto one page of data and is received into one or two physical pages.
 *                 A burst is transmitted in FIFO packet mode bythe Blue Gene Messaging Unit
 *                 descriptor using deterministic, bubble routing. All the packets in a burst arrive
 *                 in order with BG/Q packet headers which differ only by the PutOffset field.
 *                 Bursts are received sequentially in the order they are transmitted.
 *                 Protocols may depend on the ordered arrival of bursts.
 *
 * segment         A segment is a protocol data unit. A RoQ segment may not be a TCP segment.
 *                 A segment may require multiple bursts to transfer including metadata bursts
 *                 to carry protocol header and alignment management information.
 *                 Segment data may be aligned on both the source node and the target node
 *                 or they may be misaligned on memory pages or bytes or both.
 *                 Page misalignment ...
 *                 Byte misalignment may be corrected during transmission if the target alignment
 *                 is known at the source where burt is used to transmit the ends of the segment
 *                 and the BG MU is used to align the body of the segment.
 *
 */

/*** DEFINES *******************************************************/

#if !defined(STANDALONE_UD_QP_TEST)
#define CNTL_THREAD_INTERRUPT 1
#endif /* !STANDALONE_UD_QP_TEST */

#define RoQ_BURST_BITS 16
#define RoQ_BURST_SIZE (0x01<<RoQ_BURST_BITS)
#define RoQ_BURST_MASK (RoQ_BURST_SIZE-1)

#define RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT (0x01 << 7)
#define RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK (RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT - 1)
#define RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT
#define RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT_MASK RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT_MASK

#define DDP_UDP_MTU_SIZE (32+512) // BG/Q header + payload


/*
 * PktT_* are RoQ type packet type fields.
 * The RX finite state machine uses packet type to determin
 * which transition to use for each received packet.
 * Most flags can be set by the TX side. However, since each
 * packet in an MU burst has a common header except for payload offset index,
 * the sensing the first and last packets of a burst must be done
 * in software on the receive side. All packets in a burst arrive
 * with the same type and the RX engine ors in first and last status.
 *
 * There is an opportunity to
 * 	- compress the overall size of the Type range and
 *	- reduce the amount of churn in the RX prep of the Type value
 *	by folding all packet typing except first and last into enumerated values.
 *	This would mean the RDMA OP TYPE mgiht not be a bit field.
 *
 * QUESTION : are we forcing branches in the RX state machine switch? or is a jump table used?
 */
// IF THIS CHANGES, LOOK IN RX STATE MACHINE WHERE LOOP PULLS APART PACKET HEADER.
// These bits are not on the wire but are created at the top of the rx state machine.
#define PktT_FirstON	(1<<7) // NOT ON WIRE : only set in the rx state machine by software looking at the MU put count in the header
#define PktT_FirstOFF	0
#define PktT_LastON	(1<<6) // NOT ON WIRE : used in rx state machine by software looking at the MU put count in the header
#define PktT_LastOFF	0
// These bits are on the wire
#define PktT_LastBurstOfMsgON  (1<<6) // ON WIRE : swapped out in the RX state machine for the LastON/LastOFF
#define PktT_LastBurstOfMsgOFF	0
#define PktT_SetupON	(1<<5)
#define PktT_SetupOFF	0
#define PktT_IB		(1<<4)
#define PktT_CNTL       0
#define PktT_DMA_ON	(1<<3) // Flags a mode where BG/Q MU DMA GETs will be used to transfer burst data
#define PktT_DMA_OFF    0
#define ROQ_OP_NULL	0

#define PktT_SetFirst(x)   (x|=PktT_FirstON)
#define PktT_SetLast(x)    (x|=PktT_LastON)
#define PktT_SetSetup(x)   (x|=PktT_SetupON)
#define PktT_SetIB(x)      (x|=PktT_IB)
#define PktT_SetCNTL(x)    (x&=~PktT_IB)
#define PktT_SetDMA_ON(x)  (x|=PktT_DMA_ON)
//#define PktT_SetOp(x,op) (assert(op>>2==0),(x|=op)) // assert to make sure OFED doesn't throw us higher valued rdma ops
#define PktT_SetOp(x,op)   (x|=op)

#define ROQ_OP_UD_SEND (0x0F)

#define RoQ_HEADER_BITS_BURST_PACKET_COUNT       (8)  // enough for 64KB in 512 byte payloads
#define RoQ_HEADER_BITS_BURST_SEQNO             (16)
#define RoQ_HEADER_BITS_BURST_SOURCE            (20)
#define RoQ_HEADER_BITS_BURST_BYTE_OFFSET       (16)
#define RoQ_HEADER_BITS_BURST_FORWARD_COUNT      (1)

#define RoQ_HEADER_BITS_FORWARD_DESTINATION     (20)

#define ALIGN_QUADWORD    __attribute__ ((aligned (16)))

/* Size of maximum link level transfer in bytes
 * This might want to be sized on the order of one 1/2*BW*RTT
 */
#define SEGMENT_SIZE (0x01<<16)
#define SEGMENT_MASK (SEGMENT_SIZE - 1)

/* How many link level transfers can be active
 * This should be minmally the number of IP frames for to cover BW*RTT
 */
#define SEGMENT_QUEUE_COUNT (0x01<<10)
#define SEGMENT_QUEUE_MASK (SEGMENT_QUEUE_COUNT - 1)

#define MES_MEMCPY    0
#define MES_INTERNAL_PERF 0
#define ROQ_DO_COSTLY_PROTOCOL_CHECKS 0  // set true to turn on detailed protocol and debuging checks
#define ROQ_DISCOVERY_MAGIC 0x2342aa55dcbd55aaULL


#define _my_mb()		asm volatile ("sync" : : : "memory")
#define _load_shared(a)		(*(volatile typeof(a) *)&(a))
#define _store_shared(a, b)	do { _load_shared(a) = (b); _my_mb(); } while (0)

/* the 'put count' field is a byte count from a base
 * the RoQ will have a maximum single transfer segment size - probably something on the order of 64KB.
 * part of the offset will be a segment number and part byte offset used for packet count -
 * all packets in a segment are full expet maybe the last ack is on segments number not bytes
 * a separate send side dma descriptor will be formed for each segment
 * SendBurstDescriptors will be kept in a circular queue - resends will simply re-inject the segment descriptors
 * BurstAcks move the tail pointer along the SendBurstDescriptorQueue
 * Fairness will be achieved by round-robining segments from active transport connections
 */

enum {
	RoQ_PktType_Ack = 0,
	RoQ_PktType_RDMA,
	RoQ_PktType_IP,
	RoQ_PktType_CNTL
};

typedef uint64_t _QuadWord_t;

union RoQ_BGQ_MU_HW_PutOffset {
	uint32_t mWhole;

	struct {
		uint16_t mBurstSeqNo;
		uint16_t mByteOffset;
	};
};

union RoQ_BGQ_MU_FifoPkt_SwHdr {
	uint8_t AsBytes[14];

	struct {
		/*
		 * NEED: a forward count in here too
		 *
		 * this needs to fit if we want to be able
		 * to piggy back acks -- otherwise, make a union
		 */
		unsigned long mAckedBurstAbsoluteSeqNo : 60;
		unsigned mIOLinkBoundPacketFlag	: 1;
                unsigned mBurstType             : 7;    /* the size bits of burst type set tx, used in rx -- first and last bits are set on rx side */
		unsigned mSpareBits             : 4;    /* Keep spare bits here to preserve packed alignment */
		/*
		 * common fields use for packet routing
		 */
		unsigned mSource	: RoQ_HEADER_BITS_BURST_SOURCE;
		unsigned mDestination	: RoQ_HEADER_BITS_FORWARD_DESTINATION;
	}__attribute__((packed));

	struct {
		struct {
			unsigned mPacketsInBurst	:  8;	/* could be 7 bits for 64KB */
			/*
			 * this is no doubt too small to directly index
			 * the qp -- source and this value are required to
			 * identify the qp
			 */
			unsigned mDestConnectionId	: 24;
			/*
			 * this wasts a bit. half a bit here and half a bit in
			 * the BurstPacketCount field. the alternative is to
			 * merge these fields into 16 bits and do modulo 240
			 * to get the packet count.
			 */
			unsigned mBytesInLastPacket	: 10;
			unsigned padding0		: 22;
		}__attribute__((packed));

		unsigned long padding1 : 48;

	}__attribute__((packed));

}__attribute__((packed));

/* this is whatever size */
typedef union T_RoQ_OctWord {
	uint8_t		ub[32];
	uint16_t	us[16];
	uint32_t	ul[8];
	uint64_t	ull[4];
	float		f[8];
	double		d[4];
} __attribute__ ((aligned (32))) RoQ_OctWord_t;

/* the bgq struct is the generic form */
typedef union RoQ_BGQ_MU_HW_PutOffset PutOffset_t;
typedef __attribute((aligned(64))) MUHWI_Descriptor_t MUHWI_Descriptor_Aligned_t;
typedef _QuadWord_t RoQ_Chunk_t;
#define RoQ_CHUNK_SIZE (sizeof(RoQ_Chunk_t))
#define RoQ_CHUNK_ALIGNMENT_MASK (RoQ_CHUNK_SIZE - 1)
#define RoQ_PAYLOAD_MAX_CHUNKS (512/sizeof(RoQ_Chunk_t))

/* this should probably be cache aligned. */
typedef _QuadWord_t RoQ_BGQ_PacketAsChunks_t[512 / sizeof(RoQ_Chunk_t)];

struct RoQ_RX_Arguments {
	/* what hw resources should this thread use */
	int  mDataMoverThreadNumber;
	int  mPortId;
	void *recfifo_ptr;
	void *rem_recfifo_ptr;
};

struct RoQ_TX_Arguments {
	uint32_t mTxThreadNumber;
	int mPortId;
	void *recfifo_ptr;
	void *rem_recfifo_ptr;
};

struct RoQ_DDP_Fields {
	/*
	 * Note: this is packed but for alignment's sake,
	 * larger base types first
	 */
	union {
		struct {
			/* RDMA RECV SIDE */
			uint32_t MO;             // message offset
			uint32_t MSN;            // message sequence number
			uint32_t MessageLength;  // total byte length of this RDMA opp
		}  __attribute__((packed))Send;
		struct {
			/* this could be a write or a read response */
			struct roq_sge SinkSGE;
		}  __attribute__((packed))Write;
		struct {
			/* This is structured to get 64 bit entities first
			 * and doesn't use two SGEs because that would dup
			 * the lenth field
			 */
			uint64_t SrcOffset;
			struct roq_sge SinkSGE;
			uint32_t SrcSTag;           /* tagged buffer id */
		} __attribute__((packed))Read;
	};
	uint8_t RDMA_Flags;
}__attribute__((packed));

// The setup chunk is used to setup RDMA messages -- one sent per SGE.
// But the setup chunk is also used to set up Bursts for BG/Q MU Get opperations.
/* this better not turn up other than equal in size to a chunk */
union RDMA_SetupChunk
{
	RoQ_Chunk_t		AsChunk;
	struct RoQ_DDP_Fields	DDP_Fields;
	struct {
		uint64_t	mDmaAddr;
		uint32_t	mDmaLen;
		} __attribute__((packed)) BGQ_MU_DMA_Get_Setup;

};


/*
 * The chunks reserved for buffers in short packets are a significant
 * amount of memroy footprint since each TransportConnection allocss a number of them.
 * The number of chunks so allocated may well need to be smaller. Much smaller.
 * There is however a minimum of two or three chunks per for RDMA.
 */
#define ROQ_SHORTSEGMENTPACKET_CHUNKCOUNT 16

#define RoQ_SEND_USER_DATA_IN_SETUP	1  // needed for zero length sends
#define RoQ_USER_DATA_IN_SETUP_CHUNKS	(ROQ_SHORTSEGMENTPACKET_CHUNKCOUNT - 1)  // -1 becuase of the setup chunk
#define RoQ_USER_DATA_IN_SETUP_SIZE	(RoQ_USER_DATA_IN_SETUP_CHUNKS * sizeof(RoQ_Chunk_t))
#define RoQ_USER_DATA_IN_SETUP_MASK	(RoQ_USER_DATA_IN_SETUP_SIZE-1)
#define RoQ_SETUP_CHUNK_SIZE		(sizeof (union RDMA_SegSetupChunk))

/*
 * These chunks are used for three purposes
 * 1) fix up alignment -- need at least enough to allow body packets
 *    to be aligned for FPU memcpy
 * 2) transfer short messages with one packet
 * 3) transfer enough data so when main segment overlaps another
 *    physical page on the target we can avoid two segments the number
 *    of chuncks here goes right to RoQ memory footprint
 */
union RoQ_ShortSegmentPacket {
	RoQ_Chunk_t RawPacketPayload[ROQ_SHORTSEGMENTPACKET_CHUNKCOUNT];

	struct {
		union RDMA_SetupChunk	SetupChunk;
		RoQ_Chunk_t		Payload[ROQ_SHORTSEGMENTPACKET_CHUNKCOUNT - 1];
	} RDMA;
};

/*
 * This structure associates BG/Q MU descriptors with segments
 * a segment may be transmitted using 1 or 2 bgq mu descriptors
 * this size of this structure is a significant part of the RoQ footprint
 */
struct RoQ_TX_BurstDescriptor {
	/*
	 * the Setup descriptor will point to this area for tranfer
	 * alignment bytes and short tranfers will be copied here
	 */
	union RoQ_ShortSegmentPacket mSetupPacket;
	/*
	 * NEED: recode to reference the data as directly as possible
	 * the good thing about doing it this way is re-evaluating
	 * whether the data is there is possible
	 */
	int mQP_Index;
	/* which send queue entry (or send work request) */
	int mWorkRequestIndex;

	union {
		/*
		 *
		 * this is only 32 bits to make union with bits fields
		 * work work -- probably should break out to bit masks
		 * force into a byte to save space
		 */
		uint32_t mFlags;

		struct {
			/*
			 * an ack on this one will require RDMA
			 * completion processing
			 */
			unsigned mLastBurstOfMessage	: 1;
			unsigned mIsInternalReadResponse : 1;
			unsigned mIsDisconnect		: 1;
			unsigned mIsRdmaReadRequest	: 1;
			unsigned mIsRdmaWrite		: 1;
			unsigned mIsRdmaSend		: 1;
		};
	};
};



/*
 * This structure associates BG/Q MU descriptors with bursts
 * This need for this sturcture is driven by out of band dma transfers.
 */
struct RoQ_RX_BurstAckDescriptor {
	char mStatus;
        char mBurstType;
        char mForceProtocolAck; // this is just a flag... should be folded in with other members
        int  mRdmaCompletionLen;
        uint64_t wr_id;
	struct RoQ_DevicePrivatePerQPState* 	mPerQP; // this could be the index to save space
};

extern uint32_t RoQ_LocalIPV4Addr; //  = 0;
extern int RoQ_LocalRank  ; //  = -1; // index into the above array for the lcoal nodes info
extern struct RoQ_NetworkDescriptor RoQ_NetworkDesc;
extern struct roq_us_map_t *roq_us_map;
extern void init_DmaGetCounters(void);

#ifndef __KERNEL__

struct RoQ_TransportConnection {
	/*
	 *  Serious thought needs to be given to the potential for
	 *  FALSE SHARING in this struct.  May need padding. Or, we may
	 *  need to break it up in to TX, RX, and Shared structs to be held
	 *  in arrays.
	 */
	int mState;
	int mTxThreadNumber;
	uint32_t mPeerRank;

	/*
 	 *  absolute segment sequence numbers -- frames will not carry all the bits
	 *  NOTE: this is to shoehorn into BGP headers and should be undone for BGQ
	 *        Use 16bit get the right "wrapping"
	 */
	int64_t mBurstSeqNo_NextToSend;
	int64_t mBurstSeqNo_NextExpectedAck;
	int64_t mBurstSeqNo_NextToRecv;
	int64_t mBurstSeqNo_LastAcked; // initialized to -1
	int64_t mBurstSeqNo_ReadyToAck;
	int64_t mBurstCount; // debugging
	int64_t mStallCount;
	/*
	 *  BGP:
	 *  when we have reduced precission for ack
	 *  seqnos inheaders, keep track of the wraps
	 */
	uint32_t mBurstSeqNo_AckWraps;

	//////////////// TX SIDE ///////////////////////////
	MUHWI_Descriptor_Aligned_t *mTX_BGQ_MU_AckDescriptor;
	MUHWI_Descriptor_Aligned_t *mTX_BGQ_MU_DescriptorArray;
	struct RoQ_TX_BurstDescriptor *mTX_BurstDescriptorArray;

	struct RoQ_RX_BurstAckDescriptor *mRX_BurstAckDescriptorArray;
        int OutOfBandDMAActiveCount; // This is the count of unretired out-of-band DMAs on the DMA list. inc/dec by RX thread

	int ackslot;
	int use_iolink;
	int nackin;

	/////////////// RX SIDE //////////
	uint64_t BurstMemoryCopyTimeStart;
	uint64_t BurstMemoryCopyTimeTotal;
	PutOffset_t mExpectedHardwarePutOffset;
	/* this is really a pointer to RoQ_Chunk_t */
	uint64_t mRecvBurstNextPhysAddr;

	int32_t mTxPortId;
	int32_t mRxPortId;
	int32_t subgroupId;

	struct RoQ_DM_QP *mCurrentRecvBurst_QP_Ptr;
	uint32_t mCurrentRecvBurst_QP_VersionNumber;
	int mCurrentRecvBurst_QP_ConnectionId;

	struct roq_dm_mr *mCurrentRecvBurst_MR_Ptr;
	/* This is a field in the MR that will be incremented every time it is modified. */
	int mCurrentRecvBurst_MR_VersionNumber;

 	MUHWI_Destination_t PeerNodeAddr;

	// Resiliency Fields
	int                 mForwardFlags;        // 1 = forward via intermediate node, 2 = reroute via hint bits, 3 = both, 4 = node is unreachable.
	uint8_t             mForwardFifoId;       // burst fifo on intermediate node
	MUHWI_Destination_t mForwardIntermediate; // intermediate node that forwards the packet
	uint16_t            mForwardHintBits;     // hint bits from this to the forwarding intermediate node
};

#ifdef __cplusplus
extern "C" {
#endif

void tc_TX_Burst(struct RoQ_TransportConnection *tc,
		uint32_t aBurstSlot, RoQ_QP_Id_t aDestConnId,
		unsigned aBurstType,
		uint64_t aData, uint32_t aLen);

void tc_ReTX_Burst(struct RoQ_TransportConnection *tc, uint32_t aBurstSlot);
void tc_TX_Ack(struct RoQ_TransportConnection *tc, uint32_t aMagicAckValue);

void tc_init(struct RoQ_TransportConnection *, int, uint32_t,
	     struct RoQ_TX_Arguments *, struct RoQ_RX_Arguments *,
	     struct RoQ_NetworkDescriptor *);

void tc_AdvanceToExpectNextBurst(struct RoQ_TransportConnection *);

void tc_init_remote(struct RoQ_TransportConnection *, int, uint32_t,
		    struct RoQ_TX_Arguments *, struct RoQ_RX_Arguments *,
		    struct RoQ_NetworkDescriptor *, Personality_t *);
#ifdef __cplusplus
}
#endif

enum roq_qp_state_flag {
	ROQ_TX_SUSPENDED	= 1 << 0,
	ROQ_RX_SUSPENDED	= 1 << 1,
	ROQ_SEND_TERMINATE	= 1 << 2,/* Send connection termination as next message */
	ROQ_DRAIN_SQ		= 1 << 3 /* User requested SQ drain, but SQ still active */
};

//NEED: THINK ABOUT FALSE SHARING THAT MIGHT BE GOING ON IN THIS STRUCT
struct RoQ_DevicePrivatePerQPState {
	/******** CNTL *******/
	enum roq_qp_cmd_mask	qp_attr_mask; /* valid attributes */
	struct roq_qp_attr	*qp_attr;
	enum roq_qp_state	mState;
	uint32_t		state_flags;
	int			mQP_Num; /* could be calculated */
	RoQ_QP_Id_t		mPeerQPId;
	struct RoQ_TransportConnection *mTranConn;
	struct RoQ_DM_QP	*mDM_QP;

	/******** TX side *******/
	int		mNextIndex;
	int		mPrevIndex;
	int             mTxBurstsInProgress;
	/*
	 * the next field is an index between mSendQueue.mGot and mSendQueue.mPut
	 * indicating which work request is next on the wire
	 * For internal RdmaRead queue it is -1
	 */
	uint64_t	mSendQueue_SQE_NextToSend;
	int		mCurrent_SQE_Index;		/* Index into DD QP's SQ of the current validated copy of a work request */
	struct roq_sqe	Current_SQE;			/* Currently processed SQE - copy of consumer WQE */
	struct roq_sge	*Current_SendSgList;		/* Points to SG list of SQE currently processed, redundant with List_Index */
	uint32_t	mCurrent_SQE_SGE_SentSetupFlag; /* Every SGE will require it's own setup. */
	int		mSendWorkRequestActiveFlag;     /* Flag for active send wr... active one has been copied to ValidatedCopy */
	int		mCurrent_SQE_SGE_List_Index;
	uint32_t	mCurrent_SQE_SGE_Offset;
	uint64_t	mCurrent_Remote_VirtAddr;       /* Keep track of remote address during RDMA WRITE */
	uint64_t	Current_RDMA_OP_Timestamp;	/* Time current tx rdma op started -- not needed functionally */

	/******** RX side *******/
	int		mCurrent_Setup_Type;            /* -1 when not setup, an rdma transport type when setup. */
	uint32_t	mCurrent_RQE_Index;             /* Index into the DD QP receive queue */
	struct roq_rqe	mCurrent_RQE_ValidatedCopy;
	struct roq_sge	*Current_RecvSgList;		/* Points to SG list of RQE currently processed, redundant with List_Index */
	int32_t		mCurrent_RQE_SGE_List_Index;
	uint32_t	mCurrent_RQE_SGE_Offset;
	uint32_t	mCurrent_RQE_TotalReceivedBytes;
	uint32_t	mCurrentReceiveOffset;
	struct RoQ_DDP_Fields mCurrent_RDMA_OP_DDP_Fields;
	uint64_t	mCurrent_RDMA_OP_RX_StartTime;	/* Time current rx rdma op started -- not needed functionally */
	uint32_t	mAsyncBurstsInProgress;              /* Track the number of async bursts (BGQ MU GETs) in progress -- accelerate inbound RDMA read when zero. */

	/**** RDMA READ INTERNAL WRITE QUEUES ****/

	/*
	 * manage circular queue to bring received RDMA READ requests
	 * from the RX thread to the TX thread.
	 * Inbound/outbound are a matched set. Outbound on one side
	 * controls inbound on the other. These are only accessed by
	 * the RoQ itself.
	 * Outbound is array entries are populated when the
	 * RDMA READ REQ SETUP TX occurs.
	 * Inbound is populated when an RDMA READ SETUP is received
	 */
	struct RoQ_DDP_Fields	mInboundReadQueue_Array[RoQ_MAX_OUTSTANDING_RDMA_READS];
	uint32_t	mInboundReadQueue_Count;	/* total number of elements allocated */
	uint64_t	mInboundReadQueue_Posted;	/* incremented every time a wr is posted */
	uint64_t	mInboundReadQueue_Ready;	/* incremented every time a wr is made ready by ordered burst completion */
	uint64_t	mInboundReadQueue_Retired;	/* ... retired (delivered to the user */

	uint32_t	mOutboundReadQueue_Array[RoQ_MAX_OUTSTANDING_RDMA_READS];
	uint32_t	mOutboundReadQueue_Count;	/* total number of elements allocated */
	uint64_t	mOutboundReadQueue_Posted;	/* incremented every time a wr is posted */
	uint64_t	mOutboundReadQueue_Retired;	/* ... retired (delivered to the user */
};
#endif

/*** HELPER FUNCTIONS **********************************************/
#if defined(USERSPACE_ROQ)
static inline int Kernel_PhysicalProcessorID() {
	return 1;
}
#endif

/* TIME MEASURE FUNCTIONS */
static inline uint64_t
get_time(void)
{
  uint64_t usec;
#if 0
  struct timeval tv;
  gettimeofday(&tv, NULL);
  usec = (tv.tv_sec * 1000 * 1000) + tv.tv_usec;
#else
  asm volatile("mftb %0":"=r"(usec));
  usec /= 1600;
#endif
  return usec;
}

//The BGQ PutOffset is a few bits larger than 32. The MSB field are those bits.
//The RoQ currently doesn't need the upper bits.
static inline uint32_t RoQ_MUHWI_MUHdr_PutOffset_LSB(MUHWI_PacketHeader_t *packet_ptr)
{
	MUHWI_MessageUnitHeader_t  *messageUnitHeader = &(packet_ptr->messageUnitHeader);
	uint32_t Put_Offset_LSB = messageUnitHeader->Packet_Types.Memory_FIFO.Put_Offset_LSB;
	return Put_Offset_LSB;
}

static inline uint32_t RoQ_MUHWI_MUHdr_PutOffset_MSB(MUHWI_PacketHeader_t *packet_ptr)
{
	MUHWI_MessageUnitHeader_t *messageUnitHeader = &(packet_ptr->messageUnitHeader);
	uint32_t Put_Offset_MSB = messageUnitHeader->Packet_Types.Memory_FIFO.Put_Offset_MSB;
	return Put_Offset_MSB;
}

static inline void
RoQ_Log16Words(const char* reason, const char *aData, int aLen)
{
#if FXLOG_UCODE_PER_BURST

#define MaxPrintDataLen 8
	void *Data[MaxPrintDataLen];
	int di;

	for(di = 0; di < MaxPrintDataLen; di++)
		Data[di] = (void *)0xABCDEFABCDEFABCDull;
	int CLen = aLen;

	if (CLen > MaxPrintDataLen * sizeof(Data[0]))
		CLen = MaxPrintDataLen * sizeof(Data[0]);

	memcpy((char*) Data, (void *)aData, CLen );

	DPRINTF(FXLOG_UCODE_PER_BURST,
		"%s Data@ 0x%p Len=%lx %lx %lx %lx %lx %lx %lx %lx %lx\n",
		reason, (void *) aData, (uint64_t)aLen,
		(uint64_t)Data[0], (uint64_t)Data[1], (uint64_t)Data[2],
		(uint64_t)Data[3], (uint64_t)Data[4], (uint64_t)Data[5],
		(uint64_t)Data[6], (uint64_t)Data[7]);
#endif
}

static inline uint64_t*
RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS(uint64_t* aDD_Addr)
 {
#if defined(STANDALONE_UD_QP_TEST)
	return aDD_Addr;
#else
	uint64_t *MC_Addr = (uint64_t*) ((uint64_t) aDD_Addr | ROQ_VIRT_CACHE_OFF);  // translation from linux kmalloc/dma_map_single() addresses

	DPRINTF(FXLOG_UCODE_PER_BURST,
		"aDD_KDmaAddr 0x%p --> MC_VirtAddr 0x%p translation is DD + 0x%p\n",
		(void *) aDD_Addr, (void *) MC_Addr, (void *) ROQ_VIRT_CACHE_OFF);

	return MC_Addr;
#endif
}

static inline struct RoQ_DM_QP *
RoQ_GetQPPtr(RoQ_QP_Id_t QP_Id)
{
	struct RoQ_DM_QP *rc = &RoQ_DevMem->QP_Array[QP_Id.mIndex];
	return rc;
}

static inline struct RoQ_DM_QP *
RoQ_GetQPPtr_Idx(unsigned int QP_Index)
{
	struct RoQ_DM_QP *rc = &RoQ_DevMem->QP_Array[QP_Index];
	return rc;
}

/*
 * ******* Work Requests *******
 *
 * lock less, Signal SRSW queue based propogation of RoQ state from
 * controller to datamovers. there is probably a way to barrier individual
 * RDMA objects rather than the whole state when the controller thread
 * wants to update an individual object, it incremnets the objects version
 * -- say to an even num:
 * o the controller then records the loop iteration value for each data mover
 * o the controller then waits to see each data mover increment it's loop
 *   iteration value at least once (although not stop running)
 * o the controller updates the object in question
 * o the controller increments the updated object's version
 *    -- to say an even number
 * o datamovers threads take a small, finite amount of time to loop
 * o datamovers update a count every time around their main loop
 * o datamovers always check that object version numbers are even
 *   before using them in an iteration
 * o datamovers notice if an object has been updated and reload
 *   that object info (protected in the body of loop iter by controller
 *   waiting on iters)
 *
 * This structure keeps information that is accessed only by the
 * RoQ receive thread and is private to the device.
 * This allows validating state such as MR and QP once
 * at the beginning of an RDMA op and then saving state to
 * allow the receive thread to process burst for other QPs on
 * the same TranConn.
 * This allows a small message between two nodes to be interleaved
 * within a large message at the burst level.
 *
 * THIS STRUCTURE IS WHERE THE TRANCONN RECV COME ON A FIRST PACKETcw
 * TO REMEMBER WHAT IT WAS DOING FOR THE APPCONN.
 */
static inline void
RoQ_do_interrupt()
{
#if defined(CNTL_THREAD_INTERRUPT) && CNTL_THREAD_INTERRUPT == 1
	if (1 || !RoQ_DevMem->interrupt_pending) {
		RoQ_DevMem->interrupt_pending = 1;
		ROQ_interrupt();
	}
#endif
}

extern volatile union roq_tx_db_req *RoQ_RQ_DB;

// This is used by Rx thread(s) when an RDMA read is received
static inline void
RoQ_MC_RingTxDoorbell(uint32_t aQP_Num, int RxThread)
{
	static int		qp_db_index = 0;
	int idx = qp_db_index % ROQ_RQ_DB_SIZE;
	int db_full = 0;
retry:
	if (RoQ_RQ_DB) {
		union roq_tx_db_req req = {{
			.qp_id = aQP_Num,
			.num_wqe = 1
		}};

		_bgp_msync();
		if (RoQ_RQ_DB[idx].item != 0) {
			DPRINTF(1, "READ Doorbell full for %d\n", aQP_Num);
			db_full = 1;
			sleep(1); /* Reconsider: maybe usleep */
			goto retry;
		}
		*(&RoQ_RQ_DB[idx].item) = req.item;
		qp_db_index++;
	} else
		DPRINTF(1, "READ Doorbell not initialized for %d", aQP_Num);

	return;
}

static inline int
RoQ_Get_MR_Write(uint32_t key)
{
	int index = key >> (8 + 4);
	struct roq_dm_mr *dm_mr = &RoQ_DevMem->mMR_Array[index];

	if (!key)
		return 1;

	if (index && index < RoQ_MAX_MR) {
		dm_mr->inuse_rx++;
		if (dm_mr->dd_mr)
			return 1;
		dm_mr->inuse_rx--;
		DPRINTF(1, "RoQ_Get_MR_Write: invalid key: 0x%x\n", key);
		return 0;
	}
	DPRINTF(1, "RoQ_Get_MR_Write: key out of bounds: 0x%x\n", key);
	return -EINVAL;
}

static inline void
RoQ_Put_MR_Write(uint32_t key)
{
	int index = key >> (8 + 4);
	struct roq_dm_mr *dm_mr = &RoQ_DevMem->mMR_Array[index];
	if (key)
		dm_mr->inuse_rx--;
}


static inline int
RoQ_Get_MR_Read(uint32_t key)
{
	int index = key >> (8 + 4);
	struct roq_dm_mr *dm_mr = &RoQ_DevMem->mMR_Array[index];

	if (!key)
		return 1;

	if (index && index < RoQ_MAX_MR) {
		dm_mr->inuse_tx++;
		if (dm_mr->dd_mr)
			return 1;
		dm_mr->inuse_tx--;
		DPRINTF(1, "RoQ_Get_MR_Read: invalid key: 0x%x\n", key);
		return 0;
	}
	DPRINTF(1, "RoQ_Get_MR_Read: key out of bounds: 0x%x\n", key);
	return -EINVAL;
}

static inline void
RoQ_Put_MR_Read(uint32_t key)
{
	int index = key >> (8 + 4);
	struct roq_dm_mr *dm_mr = &RoQ_DevMem->mMR_Array[index];
	if (key)
		dm_mr->inuse_tx--;
}


static inline struct roq_dm_mr *
RoQ_Get_MR_Ptr_From_RDMA_Key(uint32_t key)
{
	int mr_index = key >> (8 + 4);
	struct roq_dm_mr *dm_mr = &RoQ_DevMem->mMR_Array[mr_index];

	DPRINTF(FXLOG_UCODE_PER_BURST, "lkey %d mr_index %d\n", key, mr_index);
	DSTRONG_ASSERT(mr_index < RoQ_MAX_MR, "%d %d\n", key, mr_index);

	return dm_mr;
}

static inline uint64_t
RoQ_CnkDmaAddr_To_CnkVirtAddr(uint64_t CnkDmaAddr) {
#if defined(STANDALONE_UD_QP_TEST)
	return CnkDmaAddr;
#else
	unsigned long long t = ROQ_VIRT_CACHE_OFF;
	uint64_t RoQ_VirtAddr = CnkDmaAddr | t;

	return RoQ_VirtAddr;
#endif /* STANDALONE_UD_QP_TEST */
  }


static inline uint64_t
RoQ_HostVa2DmaAddr(uint32_t lkey, uint64_t UserVirtualAddr, int64_t aReqLen, int line)
{
	uint64_t		RoQ_DmaAddr = 0;
	struct roq_dm_mr	*dm_mr;
	uint64_t		*PageArray, DataOffsetInMr, FirstByteOffset;
	int			PageIndex;

	DASSERT(aReqLen > 0,
		"%ld Not allowed to come into this function with 0 len. 7.10.6.2 "
		"Error Checking at Window Access Time: If the length of the access "
		"is zero, the RI MUST NOT perform any of the above checks on the Memory Window.\n",
		(uint64_t)line);

	if (lkey == 0) {
		/* lkey 0 means the address is already physical */
		RoQ_DmaAddr = UserVirtualAddr;

		DPRINTF(FXLOG_UCODE_PER_BURST, "lkey 0x00000000 DmaAddr@ 0x%p DmaAddr@ %lx\n",
			(void *)RoQ_DmaAddr, (uint64_t)RoQ_DmaAddr);
	} else {
		dm_mr = RoQ_Get_MR_Ptr_From_RDMA_Key(lkey);

		PageArray =  RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t*)dm_mr->mHostPageArrayDmaAddr);

		DataOffsetInMr = UserVirtualAddr - dm_mr->mBaseVirtualAddress;

		if (UserVirtualAddr < dm_mr->mBaseVirtualAddress || DataOffsetInMr + aReqLen > dm_mr->mSize) {
			DPRINTF(DEBUG_APPL_OOB, "MR access out of bounds: base addr 0x%llx, req addr 0x%llx, len %llu\n",
				dm_mr->mBaseVirtualAddress, UserVirtualAddr, aReqLen);
			goto out;
		}
		FirstByteOffset = dm_mr->mBaseVirtualAddress % dm_mr->pagesize;

		/*
		 * this produces a CNK virtual address uint64_t RoQ_VirtAddr =
		 * 0x10000000 + PageArray[0] + (UserVirtualAddr % PAGE_SIZE);
		 */
		PageIndex = (FirstByteOffset + DataOffsetInMr) / dm_mr->pagesize;

		/* NEED: if this page index is out of bounds, send up a signal somehow!! */
		RoQ_DmaAddr = PageArray[PageIndex] + (UserVirtualAddr % dm_mr->pagesize);

		DPRINTF(DEBUG_APPL_OOB && line == 0,
			"UserVirtualAddr 0x%08x, dm_mr->mBaseVirtualAddress 0x%08x, DataOffsetInMr 0x%08x, FBO 0x%08x, PI %d, Paddr 0x%08x\n",
			UserVirtualAddr, dm_mr->mBaseVirtualAddress, DataOffsetInMr, FirstByteOffset, PageIndex, PageArray[PageIndex]);

		DPRINTF(FXLOG_UCODE_MEMORY_ACCESS || FXLOG_UCODE_PER_BURST,
			"line %d lkey %d aReqLen %ld mr->BaseVA 0x%p UVA 0x%p DataOffsInMr 0x%p FirstByteOffset 0x%p PageIndex %d "
			"PageArray [%d] 0x%p dm_mr@ 0x%p DmaAddr@ 0x%p ~RoQ_BURST_MASK %p\n",
			line + 10000, lkey, aReqLen, (void *) dm_mr->mBaseVirtualAddress,
			(void *) UserVirtualAddr, (void *) DataOffsetInMr, (void *) FirstByteOffset,
			(uint64_t)PageIndex, (uint64_t)PageIndex, (void *)PageArray[PageIndex], (void *) dm_mr,
			(void *) RoQ_DmaAddr, (void *)~RoQ_BURST_MASK);

		/* assert that the requested area doesn't cross a page boundary */
		DASSERT((UserVirtualAddr & ~RoQ_BURST_MASK) == ((UserVirtualAddr + aReqLen - 1) & ~RoQ_BURST_MASK),
			"line %d lkey  %d UVA %p ReqLen %lx mask %p\n",
			line, lkey, (void *)UserVirtualAddr, aReqLen, (void *)RoQ_BURST_MASK);
	}
out:
	DPRINTF(DEBUG_APPL_OOB && !RoQ_DmaAddr, "%x -- %llx -- %d -- %llu -- %llu -- %llx, -- %llx -- %llx -- %d",
		lkey, UserVirtualAddr, dm_mr->pagesize, DataOffsetInMr,
		FirstByteOffset, dm_mr->mBaseVirtualAddress, dm_mr->mHostPageArrayDmaAddr,
		PageArray, PageIndex);
	return RoQ_DmaAddr;
}

static inline int
RoQ_InjectionFifoFull()
{
	/*
	 * can descriptors be added to the
	 * injection fifo queue?
	 */
	return 0;
}

#define rmb	ppc_msync /* To be cleaned up */
#define wmb	ppc_msync /* To be cleaned up */

extern struct RoQ_DevicePrivatePerQPState RoQ_DevicePrivatePerQPState[RoQ_MAX_QP];

/*** MAIN FUNCTIONALITY ********************************************/


#if defined(PKTRACE_ON)
#include <roq_fw_trace.h>
	TraceClient Complete_CQE_Start;
	TraceClient Complete_CQE_Finis;
#endif

extern volatile uint32_t *RoQ_CQ_DB;
extern RoQ_Atomic_t roq_cq_db_idx;

static inline void RoQ_Post_DM_Event(enum roq_doorbell_type ev_type, int producer)
{
	volatile uint32_t *event = (uint32_t *)SRSW_QueueSet_ReserveNextWriteItem(&RoQ_DevMem->mDD_DoorbellQueueSet,
										    producer);

	if (event) {
		 _store_shared(*event, ev_type);
		SRSW_QueueSet_PostNextWriteItem(&RoQ_DevMem->mDD_DoorbellQueueSet, producer);
		RoQ_do_interrupt();
	} else
		DPRINTF(1, "RoQ_Post_DM_Event: DB queue full\n");
}

static inline void
RoQ_Complete_CQE(struct roq_dm_cq *dm_cq, struct roq_cqe *cqe, int ThreadId )
{
	/* Publish the CQE to the consumer */
	_store_shared(cqe->flags, 0);

#if defined(PKTRACE_ON)
     HitOE2(Complete_CQE_Start,"Complete_CQE_Start_QPId=%04d_opcode=%03d", 0, cqe->qp_num, cqe->opcode);
#endif
	ppc_msync();

	if (1 && _load_shared(dm_cq->notify) == ROQ_NOTIFY_NOT)
		return;

	_store_shared(dm_cq->notify, ROQ_NOTIFY_NOT);

	int idx = RoQ_AtomicIncrementAndReturn(&roq_cq_db_idx) % ROQ_CQ_DB_SIZE;
	int db_full = 0;
retry:
	if (_load_shared(RoQ_CQ_DB[idx]) == 0)
		_store_shared(RoQ_CQ_DB[idx], dm_cq->dd_cq_id);
	else {
		if (db_full) {
			DPRINTF(1, "CQ DB:: Warning: CQ DB full, not posting for CQ %d\n",
				dm_cq->dd_cq_id);
			return;
		}
		DPRINTF(1, "CQ DB:: Waiting 1 sec to get entry: idx %d, Thread %d, Size %u\n",
			idx, ThreadId, ROQ_CQ_DB_SIZE);
		sleep(1);
		ppc_msync();
		db_full = 1;
		goto retry;
	}
	RoQ_Post_DM_Event(RoQ_DB_CQ, ThreadId);
}

static inline
struct roq_cqe*
RoQ_Reserve_CQE(struct roq_dm_cq *dm_cq, struct RoQ_DM_QP *qp, uint64_t wr_id,
		    enum roq_opcode opcode, enum roq_wc_status status, int byte_len,
		    int vendor_err, int ThreadId)
{
	struct roq_cqe	*dm_wcq;
	struct roq_cqe	*wc = NULL;
	uint32_t	*cq_id_p;
	int retries = 0;

	/*
	 * we lock the CQ because other RoQ threads could post completions
	 */
	SpinLock(&dm_cq->lock);

	if (!dm_cq->cq_phys) {
		DPRINTF(1, "Warning:: CQ[%d]: no DD CQ attached\n", dm_cq->dd_cq_id);
		goto out;
	}


	dm_wcq = (struct roq_cqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)dm_cq->cq_phys);

	/*
	 * No consumer 'Got' pointer maintained:
	 * CQE completion flag provides status
	 */
	wc = &dm_wcq[dm_cq->DmQueue.mPut % dm_cq->DmQueue.mItemCount];
retry:
	ppc_msync();

	if (!(_load_shared(wc->flags) & ROQ_WC_FREE)) {
		if (!_load_shared(dm_cq->cq_phys)) {
			DPRINTF(1, "DD CQ[%d] gone (mapped)\n", dm_cq->dd_cq_id);
			wc = NULL;
			goto out;
		}
		DPRINTF(1, "Warning:: CQ[%d] overflow: idx %d, %llu, %llu, %llu, 0x%p\n",
			dm_cq->dd_cq_id,
			dm_cq->DmQueue.mPut % dm_cq->DmQueue.mItemCount,
			dm_cq->DmQueue.mGot, dm_cq->DmQueue.mPut,
			dm_cq->DmQueue.mItemCount,
			dm_cq->cq_phys);
		if (++retries == 1000 * 1000) {
			retries = 0;
			RoQ_do_interrupt();
		}
		/*
		 * Todo:
		 * We actually _must_ just close that affected connection!!
		 */
		goto retry;
	}
	DPRINTF(FXLOG_UCODE_PER_MESSAGE, "wc@ 0x%p\n", (void *)wc);

	/*
	 * Write CQE but not yet clear ROQ_WC_FREE flag: CQE still in progress
	 */
	wc->id		= wr_id;
	wc->opcode	= opcode;
	wc->status	= status;
	wc->bytes	= byte_len;
	wc->qp_num	= qp->dd_qp_id;
	wc->vendor_err	= vendor_err;

	dm_cq->DmQueue.mPut++;

	wmb();

#if defined(PKTRACE_ON)
     HitOE2(Complete_CQE_Start,"Complete_CQE_Start_QPId=%04d_opcode=%03d", 0, wc->qp_num, wc->opcode);
#endif

out:
	SpinUnlock(&dm_cq->lock);

	return wc;
}


static inline void
RoQ_Post_CQ(struct roq_dm_cq *dm_cq, struct RoQ_DM_QP *qp, uint64_t wr_id,
	    enum roq_opcode opcode, enum roq_wc_status status, int byte_len,
	    int vendor_err, int ThreadId)
{
	struct roq_cqe	*cqe =
		RoQ_Reserve_CQE(dm_cq, qp, wr_id, opcode, status, byte_len, vendor_err, ThreadId);

	if (cqe)
	        RoQ_Complete_CQE(dm_cq, cqe, ThreadId);

}

/*
 * All CQE's are generated in the order
 * Work Requests are posted to the SQ
 */
static inline int
PushCompletionForWorkRequest(struct RoQ_DM_QP *DmQP , uint64_t WR_Index, uint64_t NextToSend, int ThreadId)
{
#if MES_INlTERNAL_PERF == 1
	uint64_t raw_endt = get_time();
#endif
	/* NEED A LOCK ON THE SEND QUEUE OR QP */
	KISS_SRSWQ_ReaccessReadItem(&DmQP->mSendQueue, WR_Index);

	struct roq_sqe *QP_SQ   = (struct roq_sqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)DmQP->mSendQueue.mItemArrayDmaAddr);
	struct roq_sqe *SQE_Ptr = &QP_SQ[WR_Index];
	struct roq_dm_cq *dm_cq  = (struct roq_dm_cq *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t*)DmQP->mSendCQ_DmaPtr);

	/*
	 * Used to know that this is done when the ack comes
	 * in in case it is skipped over due to wr completion ordering.
	 * If out of order acks cause mean that we can't complete,
	 * set this and it will be done on a later ack/write_for_read complete
	 */
	_store_shared(SQE_Ptr->flags, _load_shared(SQE_Ptr->flags) |  ROQ_WQE_COMPLETED);

	/* spin forward doing completions -- this picks up out of order completions */
	SpinLock(&DmQP->mSendQueueLock);

	ppc_msync();

	while (DmQP->mSendQueue.mGot < NextToSend) {
		int SQE_Index = DmQP->mSendQueue.mGot % DmQP->mSendQueue.mItemCount;
		struct roq_sqe *sqe = &QP_SQ[SQE_Index];
		volatile int flags;

		flags = _load_shared(sqe->flags);

		if (flags & ROQ_WQE_COMPLETED) {
			if (flags & ROQ_WQE_SIGNALLED)
				/*
				 * this is set when the qp has sq_sig_all==1 or
				 * when sq_sig_all==0 by the verbs consumer (user)
				 */
				RoQ_Post_CQ(dm_cq, DmQP, sqe->id, sqe->opcode, ROQ_WC_SUCCESS,
					    sqe->wr.rdma.bytes, 0, ThreadId);
			/*
			 * Bump the counter that will allow new
			 * wr's to be posted to this SendQueue
			 */
			DmQP->mSendQueue.mGot++;
			_store_shared(sqe->flags, 0);
		} else
			break;
	}
	SpinUnlock(&DmQP->mSendQueueLock);

#if MES_INTERNAL_PERF == 1
	{
		uint64_t endt = get_time();
		double raw_elapsed_time = (raw_endt - Acked_SQE_Ptr->timestamp)/1000000.0;
		double elapsed_time = (endt - Acked_SQE_Ptr->timestamp)/1000000.0;
		DPRINTF(1, "OpType %d id 0x%llx raw_elapsed_time %f elapsed_time %f bytes %d bandwidth %fMiB/s  \n\n",
			wc->opcode, wc->wr_id, raw_elapsed_time, elapsed_time,
			wc->byte_len, (wc->byte_len/elapsed_time)/(1024*1024));
	}
#endif
	return 0;
}

static inline void
RoQ_RX_RDMA_WRITE_Completes_RDMA_READ_Request(struct RoQ_DevicePrivatePerQPState *PerQP, int len)
{
	/*
	 * If this RDMA WRITE was a RDMA READ response, we need to issue a completion
	 * NEED: what's the path for RDMA READ errors?
	 */
	DASSERT(PerQP, "%p", PerQP);
	DASSERT(PerQP->mDM_QP, "%p", PerQP->mDM_QP);

	// dig out the Send Queue Entry containting the work request and save the stuff needed for a CQE
	int OBRQ_Index = PerQP->mOutboundReadQueue_Retired % PerQP->mOutboundReadQueue_Count;
	int SQE_Index = PerQP->mOutboundReadQueue_Array[OBRQ_Index];

	/* creating the WC retires an outbound read queue slot */
	PerQP->mOutboundReadQueue_Retired++;

	DPRINTF(FXLOG_UCODE_PER_MESSAGE,
		"qpn %lx RDMA READ COMPLETION WR Index %lx mORQ_Retired %lx Bytes %lx\n",
		(uint64_t)PerQP->mQP_Num, (uint64_t)SQE_Index,
		(uint64_t)PerQP->mOutboundReadQueue_Retired, (uint64_t)len);

	/* needs fixin' but this block puts in the size */
	struct roq_sqe *QP_SQ = (struct roq_sqe *)RoQ_MC_VIRT_FROM_DD_DMA_ADDR_TRANS((uint64_t *)PerQP->mDM_QP->mSendQueue.mItemArrayDmaAddr);
	struct roq_sqe *SQE_Ptr = &QP_SQ[SQE_Index];

	SQE_Ptr->wr.rdma.bytes = len;

	/*
	 *  this block handles SEND/WRITE completions to the user
	 *  NOTE: the one at the end of the call is the RX thread id. Needs to be cleaned up.
	 */
	PushCompletionForWorkRequest(PerQP->mDM_QP, SQE_Index, PerQP->mSendQueue_SQE_NextToSend, 1);

}

#ifdef ROQ_RESILIENCY
/* Begin: Resilience related stuff */
struct RoQ_Resiliency_Args_t
  {
    int dummy;
  };

extern struct RoQ_Resiliency_Args_t RoQ_Resiliency_Args;

void * RoQ_Resiliency_Init(void *args);
void * RoQ_Resiliency_Notifier(void *args);
void * RoQ_Resiliency_Checker(void *args);
void * RoQ_Resiliency_Forwarder(void *args);
void * RoQ_Resiliency_Router(void *args);
/* End: Resilience related stuff */
#endif

extern struct RoQ_RX_Arguments   RoQ_RX_Args[1];
extern struct RoQ_TX_Arguments   RoQ_TX_Args[1];
extern struct RoQ_TransportConnection RoQ_loc_TransportConnection[ROQ_MAX_IO_NODES];
extern struct RoQ_TransportConnection RoQ_rem_TransportConnection[ROQ_MAX_IO_NODES];
extern struct RoQ_DevicePrivatePerQPState RoQ_DevicePrivatePerQPState[RoQ_MAX_QP];
extern int RoQ_DD2MC_AddressTranslation;
extern Personality_t *personality;

extern int32_t  RoQ_LocalRank;     //  = -1 -- index into the above array for the lcoal nodes info
extern uint32_t RoQ_LocalIPV4Addr; //  = 0;
extern struct RoQ_NetworkDescriptor RoQ_NetworkDesc;
extern struct RoQ_NetworkDescriptor RoQ_NetworkDesc_rem;
extern volatile uint32_t RoQ_discovery_act;
extern uint64_t BootTime;

void *RoQ_RX(void *args);
void *RoQ_TX(void *args);

#endif /* __ROQ_FW_H__ */
