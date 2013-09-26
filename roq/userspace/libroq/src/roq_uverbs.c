/*
 * RoQ Userspace Library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif				/* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <pthread.h>

#include <sys/syscall.h>

#include "roq.h"
#include "roq_abi.h"

#define _my_mb()		asm volatile ("sync" : : : "memory")

#define _load_shared(a)		(*(volatile typeof(a) *)&(a))
#define _store_shared(a, b)	do { \
					 _load_shared(a) = (b); _my_mb(); \
				} while (0)

extern const int roq_debug;
extern const int do_poll_syscall;
extern int roq_db_syscall;

int roq_notify_cq_mapped(struct ibv_cq *ibcq, int solicited)
{
	struct roq_cq *cq = cq_ofa2roq(ibcq);
	uint32_t flags = 0;
	int rv;

	pthread_spin_lock(&cq->lock);
	flags = solicited ?  ROQ_NOTIFY_SOLICITED : ROQ_NOTIFY_NEXT_COMPLETION;
	rv = syscall(roq_db_syscall, ROQ_DB_CQ_NOTIFY, cq->id, flags);
	pthread_spin_unlock(&cq->lock);

	return rv;
}

int roq_notify_cq_ofed(struct ibv_cq *ibcq, int solicited)
{
	return ibv_cmd_req_notify_cq(ibcq, solicited);
}

/* ToDo: make array */
static enum roq_opcode map_send_opcode(enum ibv_wr_opcode ibv_op)
{
	switch (ibv_op) {

	case IBV_WR_SEND:	return ROQ_OP_SEND;
	case IBV_WR_RDMA_WRITE: return ROQ_OP_WRITE;
	case IBV_WR_RDMA_READ:	return ROQ_OP_READ;
	default:
		printf("op %d not supported\n", ibv_op);
		exit(0);
	}
}

static inline int map_send_flags(int ibv_flags)
{
	int flags = ROQ_WQE_VALID;

	if (ibv_flags & IBV_SEND_SIGNALED)	flags |= ROQ_WQE_SIGNALLED;
	if (ibv_flags & IBV_SEND_SOLICITED)	flags |= ROQ_WQE_SOLICITED;
	if (ibv_flags & IBV_SEND_INLINE)	flags |= ROQ_WQE_INLINE;

	return flags;
}

static inline int push_recv_wqe(struct ibv_recv_wr *ofa_wr,
				 struct roq_rqe *roq_rqe)
{
	roq_rqe->id		= ofa_wr->wr_id;
	roq_rqe->num_sge	= ofa_wr->num_sge;
	
	if (ofa_wr->num_sge == 1) {
		roq_rqe->sge[0].addr	= ofa_wr->sg_list[0].addr;
		roq_rqe->sge[0].length	= ofa_wr->sg_list[0].length;
		roq_rqe->sge[0].key	= ofa_wr->sg_list[0].lkey;
	} else if (ofa_wr->num_sge && ofa_wr->num_sge <= ROQ_MAX_SGE)
		memcpy(roq_rqe->sge, ofa_wr->sg_list,
		       sizeof(struct ibv_sge) * ofa_wr->num_sge);
	else
		return 1;

	_store_shared(roq_rqe->flags, 0);
	_store_shared(roq_rqe->flags, ROQ_WQE_VALID);

	return 0;
}

static inline int push_send_wqe(struct ibv_send_wr *ofa_wr,
				struct roq_sqe *roq_sqe)
{
	uint32_t flags = map_send_flags(ofa_wr->send_flags);

	roq_sqe->id			= ofa_wr->wr_id;
	roq_sqe->num_sge		= ofa_wr->num_sge;
	roq_sqe->wr.rdma.remote_addr	= ofa_wr->wr.rdma.remote_addr;
	roq_sqe->wr.rdma.rkey		= ofa_wr->wr.rdma.rkey;

	roq_sqe->opcode = map_send_opcode(ofa_wr->opcode);

	if (ofa_wr->num_sge == 1) {
		roq_sqe->sge[0].addr	= ofa_wr->sg_list[0].addr;
		roq_sqe->sge[0].length	= ofa_wr->sg_list[0].length;
		roq_sqe->sge[0].key	= ofa_wr->sg_list[0].lkey;
		/* Assist provider for short msg's */
		roq_sqe->wr.rdma.bytes	= ofa_wr->sg_list[0].length;
#if 0
	} else if (ofa_wr->num_sge && ofa_wr->num_sge <= ROQ_MAX_SGE)
		memcpy(roq_sqe->sge, ofa_wr->sg_list,
		       roq_sqe->num_sge * sizeof (struct ibv_sge));
	else
		return -EINVAL;
#else
	} else
		memcpy(roq_sqe->sge, ofa_wr->sg_list,
		       ROQ_MAX_SGE * sizeof (struct ibv_sge));
#endif

	/* TODO: handle inline data */

	if (roq_sqe->opcode == ROQ_OP_WRITE)
		flags |= ROQ_WQE_ABSOLUTE_ADDR;

	_store_shared(roq_sqe->flags, 0);
	_store_shared(roq_sqe->flags, flags);

	return 0;
}

int roq_post_send_ofed(struct ibv_qp *ofa_qp, struct ibv_send_wr *wr,
		       struct ibv_send_wr **bad_wr)
{
	struct roq_qp	*qp = qp_ofa2roq(ofa_qp);
	int		rv;

	pthread_spin_lock(&qp->sq_lock);

	rv = ibv_cmd_post_send(ofa_qp, wr, bad_wr);

	pthread_spin_unlock(&qp->sq_lock);

	return rv;
}


/*
 * mapped send queue and doorbell syscall:
 * directly write into array and ring doorbell by syscall
 */
int roq_post_send_mmap_db(struct ibv_qp *ofa_qp, struct ibv_send_wr *wr,
			  struct ibv_send_wr **bad_wr)
{
	struct roq_qp	*qp = qp_ofa2roq(ofa_qp);
	uint64_t	sq_put;
	int new, rv = 0;

	if (!qp->sq)
		return roq_post_send_ofed(ofa_qp, wr, bad_wr);

	pthread_spin_lock(&qp->sq_lock);

	*bad_wr = NULL;
	sq_put = qp->sq_put;

	do {
		int idx = sq_put % qp->sq_size;
		struct roq_sqe *sqe = &qp->sq[idx];
		int retry = 0;
retry:
		rmb();

		if (_load_shared(sqe->flags) & ROQ_WQE_VALID) {
			if (++retry < 1000)
				goto retry;
			printf("QP %d: SQ overflow, idx %d\n", qp->id, idx);
			rv = -ENOMEM;
			*bad_wr = wr;
			break;
		}
		if (push_send_wqe(wr, sqe)) {
			*bad_wr = wr;
			rv = -EINVAL;
			break;
		}
		sq_put++;
		wr = wr->next;

	} while (wr);

	new = sq_put - qp->sq_put;

	if (new) {
		if (qp->db_sq) {
			union roq_tx_db_req req = {{
				.qp_id = qp->id,
				.num_wqe = new
			}}; 
			uint32_t db_idx = __sync_fetch_and_add(qp->db_put, 1);

			db_idx %= qp->db_size;
			if (!_load_shared(qp->db_sq[db_idx].item)) {
				_store_shared(qp->db_sq[db_idx].item, req.item);
			} else {
				/*
				 * Todo: make this atomic via test_and_set
				 */
				if ((__sync_sub_and_fetch(qp->db_put, 1) %
				     qp->db_size) != db_idx)
					printf("DB err for QP[%d]\n", qp->id);

				(void)syscall(roq_db_syscall, ROQ_DB_SQ, qp->id,
					      new);
				if (roq_debug)
					printf("QP[%d]: user DB overflow\n",
						qp->id);
			}
		} else
			(void)syscall(roq_db_syscall, ROQ_DB_SQ, qp->id, new);

		qp->sq_put = sq_put;
	}

	pthread_spin_unlock(&qp->sq_lock);

	return rv;
}


/*
 * mapped send queue:
 * directly write into array and ring doorbell by posting zero WR's
 */
int roq_post_send_mmap(struct ibv_qp *ofa_qp, struct ibv_send_wr *wr,
			  struct ibv_send_wr **bad_wr)
{
	struct roq_qp			*qp = qp_ofa2roq(ofa_qp);
	uint64_t			sq_put;
	int rv = 0;

	if (!qp->sq)
		return roq_post_send_ofed(ofa_qp, wr, bad_wr);

	pthread_spin_lock(&qp->sq_lock);

	sq_put = qp->sq_put;

	do {
		struct ibv_post_send		req;
		struct ibv_post_send_resp	resp;

		int idx = sq_put % qp->sq_size;

		req.command	= IB_USER_VERBS_CMD_POST_SEND;
		req.in_words	= (sizeof req) / 4;
		req.out_words	= (sizeof resp) / 4; 
		req.response	= (uintptr_t)&resp;
		req.qp_handle	= ofa_qp->handle;
		req.wr_count	= 0;
		req.sge_count	= 0;
		req.wqe_size	= sizeof *wr;

		if (push_send_wqe(wr, &qp->sq[idx])) {
			*bad_wr = wr;
			rv = -EINVAL;
			break;
		}

		/*
		 * Currently writes single WR by WR. To be enhanced after
		 * RoQ WQ interface design update
		 */
		rv = write(ofa_qp->context->cmd_fd, &req, sizeof req);
		if (rv != sizeof req) {
			*bad_wr = wr;
			perror("write: ");
			break;
		}
		rv = 0;

		sq_put++;
		wr = wr->next;
	} while (wr);

	qp->sq_put = sq_put;

	pthread_spin_unlock(&qp->sq_lock);

	return rv;
}

int roq_post_recv_ofed(struct ibv_qp *ofa_qp, struct ibv_recv_wr *wr,
		      struct ibv_recv_wr **bad_wr)
{
	struct roq_qp	*qp = qp_ofa2roq(ofa_qp);
	int		rv;

	pthread_spin_lock(&qp->rq_lock);
	rv = ibv_cmd_post_recv(ofa_qp, wr, bad_wr);
	pthread_spin_unlock(&qp->rq_lock);

	return rv;
}

int roq_post_recv_mmap(struct ibv_qp *ofa_qp, struct ibv_recv_wr *wr,
		       struct ibv_recv_wr **bad_wr)
{
	struct roq_qp	*qp = qp_ofa2roq(ofa_qp);
	uint64_t	rq_put;
	int		new, rv = 0;

	if (!qp->rq)
		return roq_post_recv_ofed(ofa_qp, wr, bad_wr);

	pthread_spin_lock(&qp->rq_lock);

	rq_put = qp->rq_put;

	do {
		int idx = rq_put % qp->rq_size;
#if 1
		_my_mb();

		if (!(_load_shared(qp->rq[idx].flags) & ROQ_WQE_COMPLETED)) {
			printf("QP %d: RQ overflow\n", qp->id);
			*bad_wr = wr;
			rv = -ENOMEM;
			break;
		}
#endif
		if (push_recv_wqe(wr, &qp->rq[idx])) {
			*bad_wr = wr;
			rv = -EINVAL;
			break;
		}

		rq_put++;
		wr = wr->next;

	} while (wr);

	new = rq_put - qp->rq_put;

	if (new) {
#ifndef MAPPED_RQ_FASTPATH
		/* That doorbell call is actually not needed anymore */
		rv = syscall(roq_db_syscall, ROQ_DB_RQ, qp->id, new);

		if (roq_debug && rv)
			printf("recv_doorbell failed: %d (%d)\n", rv, new);
		if (!rv)
#endif
			qp->rq_put += new;
	} 

	pthread_spin_unlock(&qp->rq_lock);

	if (roq_debug && rv)
		printf("roq_post_recv_mmap failed: %d\n", rv);

	return rv;
}

int roq_post_srq_recv_ofed(struct ibv_srq *ofa_srq, struct ibv_recv_wr *wr,
			   struct ibv_recv_wr **bad_wr)
{
	struct roq_srq	*srq = srq_ofa2roq(ofa_srq);
	int rv;

	pthread_spin_lock(&srq->lock);
	rv = ibv_cmd_post_srq_recv(ofa_srq, wr, bad_wr);
	pthread_spin_unlock(&srq->lock);

	return rv;
}

int roq_poll_cq_ofed(struct ibv_cq *ibcq, int num_entries, struct ibv_wc *wc)
{
	struct roq_cq	*cq = cq_ofa2roq(ibcq);
	int		rv;

	pthread_spin_lock(&cq->lock);
	rv = ibv_cmd_poll_cq(ibcq, num_entries, wc);
	pthread_spin_unlock(&cq->lock);

	// printf("roq_poll_cq_ofed rv=%d\n",rv) ; fflush(stdout) ; 
	return rv;
}

static enum ibv_wc_status map_cqe_status(enum roq_wc_status roq_status)
{
	if (roq_status == ROQ_WC_SUCCESS)
		return IBV_WC_SUCCESS;
	if (roq_status == ROQ_WC_LOC_LEN_ERR)
		return IBV_WC_LOC_LEN_ERR;
	if (roq_status == ROQ_WC_LOC_PROT_ERR)
		return IBV_WC_LOC_PROT_ERR;
	if (roq_status == ROQ_WC_WR_FLUSH_ERR)
		return IBV_WC_WR_FLUSH_ERR;
	if (roq_status == ROQ_WC_BAD_RESP_ERR)
		return IBV_WC_BAD_RESP_ERR;
	if (roq_status == ROQ_WC_LOC_ACCESS_ERR)
		return IBV_WC_LOC_ACCESS_ERR;
	if (roq_status == ROQ_WC_REM_ACCESS_ERR)
		return IBV_WC_REM_ACCESS_ERR;
	if (roq_status == ROQ_WC_GENERAL_ERR)
		return IBV_WC_GENERAL_ERR;

	return IBV_WC_GENERAL_ERR;
}

static inline enum ibv_wc_flags map_cqe_flags(enum roq_cqe_flags roq_flags)
{
	enum ibv_wc_flags flags = 0;

	if (roq_flags & ROQ_WC_WITH_IMM)
		flags = IBV_WC_WITH_IMM;

	return flags;
}

static inline enum ibv_wc_opcode map_cqe_opcode(enum roq_opcode roq_opcode)
{
	if (roq_opcode == ROQ_OP_WRITE)
		return IBV_WC_RDMA_WRITE;
	if (roq_opcode == ROQ_OP_READ)
		return IBV_WC_RDMA_READ;
	if (roq_opcode == ROQ_OP_SEND)
		return IBV_WC_SEND;
	if (roq_opcode == ROQ_OP_RECEIVE)
		return IBV_WC_RECV;

	printf("unknown work completion type: %d\n", roq_opcode);
	return ROQ_OP_WRITE;
}
	
static void copy_cqe(struct roq_cqe *cqe, struct ibv_wc *wc)
{
	wc->wr_id = cqe->id;
	wc->status = map_cqe_status(cqe->status);
	wc->opcode = map_cqe_opcode(cqe->opcode);
	wc->vendor_err = 0;
	wc->byte_len = cqe->bytes;
	wc->imm_data = 0;
	wc->qp_num = cqe->qp_num;
	wc->src_qp = 0;
	wc->wc_flags = map_cqe_flags(cqe->flags);
	
	if (roq_debug && cqe->status != ROQ_WC_SUCCESS)
		printf("copy_cqe: bad wc: qp[%d], status %d, bytes %d,\n",
			wc->qp_num, cqe->status, cqe->bytes);

	_my_mb();
	_store_shared(cqe->flags, ROQ_WC_FREE);
}

int roq_poll_cq_mapped(struct ibv_cq *ibcq, int num_entries, struct ibv_wc *wc)
{
	struct roq_cq	*cq = cq_ofa2roq(ibcq);
	int		new = 0;

	if (!cq->wq)
	{
		int rc= roq_poll_cq_ofed(ibcq, num_entries, wc);
	//	printf("roq_poll_cq_mapped rc=%d\n",rc) ;
		return rc ; 
	}

	for (; num_entries--; wc++) {
		struct roq_cqe	*cqe;
		uint32_t	idx;

		pthread_spin_lock(&cq->lock);

		_my_mb();
		idx = _load_shared(cq->cq_get);
		cqe = &cq->wq[idx % cq->cq_size];

		if (_load_shared(cqe->flags) & ROQ_WC_FREE) {
			pthread_spin_unlock(&cq->lock);
			break;
		}
		++cq->cq_get;
		pthread_spin_unlock(&cq->lock);

		copy_cqe(cqe, wc);

		new++;
	}
	if (do_poll_syscall && new) {
		int rv;

		rv = syscall(roq_db_syscall, ROQ_DB_CQ_POLL, cq->id, new);
		new = rv ? rv : new;
	}
	// printf("roq_poll_cq_mapped returns %d\n",new) ; fflush(stdout) ; 

	return new;
}
