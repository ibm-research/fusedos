#ifndef __ROQ_COMMON_H
#define __ROQ_COMMON_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif


#define	ROQ_MAX_SGE	6
#define QUEUE_FLAG_MAPPEABLE	1
#define RoQ_MAX_OUTSTANDING_RDMA_READS 64

/* Maximum number of mapped doorbell arrays */
#define ROQ_MAX_MAPPED_DB	16
#define ROQ_DB_ARRAY_SIZE	1024

/* avoid syscall for producer idx update after post_recv() */
#define MAPPED_RQ_FASTPATH



/*
 * Fast doorbell syscall resource IDs
 */
enum roq_db_type {
	ROQ_DB_SQ	= 0,
	ROQ_DB_RQ	= 1,
	ROQ_DB_SRQ	= 2,
	ROQ_DB_CQ_POLL	= 3,
	ROQ_DB_CQ_NOTIFY= 4
};

union roq_tx_db_req {
	struct {
		uint16_t	qp_id;
		uint16_t	num_wqe;
	};
	uint32_t	item;
};


enum roq_api_caps {
	ROQ_OFA_CALL	= 0,		/* OFA generic post/poll */
	ROQ_MAPPED_SQ	= (1 << 0),	/* Mapped SQ */
	ROQ_MAPPED_RQ	= (1 << 1),	/* Mapped RQ */
	ROQ_MAPPED_CQ	= (1 << 2),	/* Mapped CQ */
	ROQ_SYSCALL_DB	= (1 << 3),	/* Extra syscall for fast Doorbell */
	ROQ_CAP_MAX	= (1 << 4)	/* Unknown capability */
};

enum roq_qp_state {
	ROQ_QPS_RESET = 0,
	ROQ_QPS_INIT,
	ROQ_QPS_RTR,
	ROQ_QPS_RTS,
	ROQ_QPS_SQD,
	ROQ_QPS_SQE,
	ROQ_QPS_ERROR
};

enum roq_qp_type {
	ROQ_QPT_RC = 0,
	ROQ_QPT_UD
};

enum roq_opcode {
	ROQ_OP_WRITE	= 0,
	ROQ_OP_READ	= 1,
	ROQ_OP_SEND	= 2,
	ROQ_OP_RECEIVE	= 3,
	ROQ_NUM_OPCODES	= 4
	/* to be extended with atomics etc.*/
};

enum roq_wqe_flags {
	ROQ_WQE_VALID		= 1,
	ROQ_WQE_INLINE		= (1 << 1),
	ROQ_WQE_SIGNALLED	= (1 << 2),
	ROQ_WQE_SOLICITED	= (1 << 3),
	ROQ_WQE_ABSOLUTE_ADDR	= (1 << 4),
	ROQ_WQE_READ_RESPONSE	= (1 << 5), /* Used by transport */

	ROQ_WQE_COMPLETED	= (1 << 8)
};

enum roq_cqe_flags {
	ROQ_WC_FREE		= 1,
	ROQ_WC_WITH_IMM		= (1 << 1)
};

struct roq_sge {
	uint64_t	addr;
	uint32_t	length;
	uint32_t	key;
};

struct roq_sqe {
	uint64_t	id;	/* given by user, expected in work compl. */
	uint32_t	flags;	/* Bit list of roq_wqe_flags */
	uint32_t	num_sge;
	enum roq_opcode	opcode;
	union {
		struct {
			uint64_t	remote_addr;
			uint32_t	rkey;
			/* Only used by provider */
			uint32_t	bytes;
		} rdma;
		/*
		 * More types to be added.
		 */
	} wr;
	/* Only used by provider */
	uint64_t	timestamp;
	/* Local communication buffer list */
	struct roq_sge	sge[ROQ_MAX_SGE];
};

struct roq_rqe {
	uint64_t	id;	/* given by user, expected in work compl. */
	uint32_t	flags;	/* Bit list of roq_wqe_flags */
	uint32_t	num_sge;
	struct roq_sge	sge[ROQ_MAX_SGE];
};

enum roq_notify_flags {
	ROQ_NOTIFY_NOT			= (0),
	ROQ_NOTIFY_SOLICITED		= (1 << 0),
	ROQ_NOTIFY_NEXT_COMPLETION	= (1 << 1),
	ROQ_NOTIFY_MISSED_EVENTS	= (1 << 2)
}; 

/*
 * Semantically this is a subset of the work completion
 * errors as defined in /usr/include/infiniband/verbs.h
 * Additional error codes to be added if ROQ provider 
 * functionality evolves.
 * See /usr/include/infiniband/verbs.h for complete list.
 */
enum roq_wc_status {
	ROQ_WC_SUCCESS = 0,
	ROQ_WC_LOC_LEN_ERR,
	ROQ_WC_LOC_PROT_ERR,
	ROQ_WC_WR_FLUSH_ERR,
	ROQ_WC_BAD_RESP_ERR,
	ROQ_WC_LOC_ACCESS_ERR,
	ROQ_WC_REM_ACCESS_ERR,
	ROQ_WC_GENERAL_ERR
};

struct roq_cqe {
	uint64_t		id;
	uint32_t		flags;	/* Bit list of roq_wqe_flags */
	enum roq_opcode		opcode; /* corresponding sq/rq element */
	enum roq_wc_status	status;
	uint64_t		imm_data;
	uint32_t		bytes;	/* Bytes transferred/received */
	uint32_t		qp_num;	/* QP this WC was produced from */
	uint32_t		vendor_err;
};

#endif /* __ROQ_COMMON_H */
