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
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <inttypes.h>

#include "roq.h"
#include "roq_abi.h"


extern const int roq_debug;

int roq_query_device(struct ibv_context *ctx, struct ibv_device_attr *attr)
{
	struct ibv_query_device	cmd;
	uint64_t raw_fw_ver;
	unsigned major, minor, sub_minor;
	int rv;

	rv = ibv_cmd_query_device(ctx, attr, &raw_fw_ver, &cmd, sizeof cmd);
	if (rv)
		return rv;

	major = (raw_fw_ver >> 32) & 0xffff;
	minor = (raw_fw_ver >> 16) & 0xffff;
	sub_minor = raw_fw_ver & 0xffff;

	snprintf(attr->fw_ver, sizeof attr->fw_ver,
		 "%d.%d.%d", major, minor, sub_minor);

	return 0;
}

int roq_query_port(struct ibv_context *ctx, uint8_t port,
		   struct ibv_port_attr *attr)
{
	struct ibv_query_port cmd;

	return ibv_cmd_query_port(ctx, port, attr, &cmd, sizeof cmd);
}


int roq_query_qp(struct ibv_qp *ofaqp, struct ibv_qp_attr *attr,
                        int attr_mask,
                        struct ibv_qp_init_attr *init_attr)
{
	int		rv;
	struct roq_qp	*qp = qp_ofa2roq(ofaqp);
	struct ibv_query_qp cmd;

	/*  one lock might be enough */
	pthread_spin_lock(&qp->rq_lock);
	rv = ibv_cmd_query_qp(ofaqp, attr, attr_mask,
			      init_attr, &cmd, sizeof(cmd));
	pthread_spin_unlock(&qp->rq_lock);

	return rv;
}


struct ibv_pd *roq_alloc_pd(struct ibv_context *ctx)
{
	struct roq_alloc_pd	 cmd;
	struct roq_alloc_pd_resp resp;
	struct roq_pd 		 *pd;

	pd = malloc(sizeof *pd);
	if (!pd)
		return NULL;

	if (ibv_cmd_alloc_pd(ctx, &pd->ofa_pd, (struct ibv_alloc_pd*)&cmd,
			     sizeof cmd, &resp.ofa_resp, sizeof resp)) {
		free(pd);
		return NULL;
	}
	return &pd->ofa_pd;
}

int roq_free_pd(struct ibv_pd *pd)
{
	int rv;

	rv = ibv_cmd_dealloc_pd(pd);
	if (rv)
		return rv;

	free(pd);
	return 0;
}


struct ibv_mr *roq_reg_mr(struct ibv_pd *pd, void *addr,
			  size_t len, int access)
{
	struct roq_mr			*mr;
	struct roq_cmd_reg_umr_req	req;
	struct roq_cmd_reg_umr_resp	resp;

	int		rv;

	mr = malloc(sizeof *mr);

	if (!mr)
		return NULL;

	rv = ibv_cmd_reg_mr(pd, addr, len, (uintptr_t)addr, access,
			    &mr->ofa_mr, &req.ofa_req, sizeof req,
			    &resp.ofa_resp, sizeof resp);

	if (rv) {
		free(mr);
		return NULL;
	}
	return &mr->ofa_mr;
}

int roq_dereg_mr(struct ibv_mr *ofa_mr)
{
	struct roq_mr	*mr = mr_ofa2roq(ofa_mr);
	int		rv;

	rv = ibv_cmd_dereg_mr(ofa_mr);

	if (rv)
		return rv;

	free(mr);

	return 0;
}

struct ibv_cq *roq_create_cq(struct ibv_context *ctx, int num_cqe,
			     struct ibv_comp_channel *channel, int comp_vector)
{
	struct roq_cq			*cq;
	struct roq_cmd_create_cq	cmd;
	struct roq_cmd_create_cq_resp	resp;
	int				rv;

	cq = malloc(sizeof *cq);
	if (!cq)
		return NULL;

	memset(cq, 0, sizeof *cq);

	rv = ibv_cmd_create_cq(ctx, num_cqe, channel, comp_vector,
			       &cq->ofa_cq, &cmd.ofa_cmd, sizeof cmd,
			       &resp.ofa_resp, sizeof resp);
	if (rv)
		goto fail;

	pthread_spin_init(&cq->lock, PTHREAD_PROCESS_PRIVATE);

	cq->id = resp.cq_id;
	//printf("CQ[%d]: created\n", cq->id);
	//fflush(stdout) ; 
	if (resp.flags == QUEUE_FLAG_MAPPEABLE) {
		int size = resp.cq_size;

		cq->wq = (struct roq_cqe *)mmap(NULL, size,
					PROT_READ|PROT_WRITE, MAP_SHARED,
					ctx->cmd_fd, resp.cq_key);

		if (cq->wq == MAP_FAILED)
			goto fail;

		cq->cq_size = (resp.cq_size)/sizeof(struct roq_cqe);
		cq->cq_get = 0;

		memset(&cq->wq[0], 0, cq->cq_size * sizeof(struct roq_cqe));

		size = 0;

		while (size < cq->cq_size) 
			cq->wq[size++].flags = ROQ_WC_FREE;

		wmb();

	} else
		cq->wq = NULL;

	return &cq->ofa_cq;

fail:	free (cq);
	return (struct ibv_cq *) NULL;
}

int roq_resize_cq(struct ibv_cq *ofa_cq, int num_cqe)
{
	return -ENOSYS;
}

int roq_destroy_cq(struct ibv_cq *ofacq)
{
	struct roq_cq	*cq = cq_ofa2roq(ofacq);
	int 		rv;

	pthread_spin_lock(&cq->lock);

	if (cq->wq && cq->wq != MAP_FAILED)
		munmap((void *)cq->wq, cq->cq_size * sizeof(struct roq_cqe));

	rv = ibv_cmd_destroy_cq(ofacq);
	if (rv) {
		pthread_spin_unlock(&cq->lock);
		return rv;
	}
	pthread_spin_unlock(&cq->lock);

	free(cq);

	return 0;
}

struct ibv_srq *roq_create_srq(struct ibv_pd *pd,
			       struct ibv_srq_init_attr *attr)
{
	struct roq_cmd_create_srq	cmd;
	struct roq_cmd_create_srq_resp	resp;
	struct roq_srq			*srq = malloc(sizeof *srq);

	if (!srq)
		return NULL;

	if (ibv_cmd_create_srq(pd, &srq->ofa_srq, attr, &cmd.ofa_cmd,
			       sizeof cmd, &resp.ofa_resp, sizeof resp)) {
		free(pd);
		return NULL;
	}
	pthread_spin_init(&srq->lock, PTHREAD_PROCESS_PRIVATE);

	return &srq->ofa_srq;
}

int roq_modify_srq(struct ibv_srq *ofa_srq, struct ibv_srq_attr *attr,
		   int attr_mask)
{
	struct roq_srq *srq = srq_ofa2roq(ofa_srq);
	struct ibv_modify_srq cmd;
	int rv;

	pthread_spin_lock(&srq->lock);
	rv = ibv_cmd_modify_srq(ofa_srq, attr, attr_mask, &cmd, sizeof cmd);
	pthread_spin_unlock(&srq->lock);

	return rv;
}

int roq_destroy_srq(struct ibv_srq *ofa_srq)
{
	struct roq_srq	*srq = srq_ofa2roq(ofa_srq);

	pthread_spin_lock(&srq->lock);
	ibv_cmd_destroy_srq(ofa_srq);
	pthread_spin_unlock(&srq->lock);

	free(srq);

	return 0;
}

int roq_destroy_qp(struct ibv_qp *ofaqp)
{
	struct roq_qp	*qp = qp_ofa2roq(ofaqp);
	int		rv;

	pthread_spin_lock(&qp->sq_lock);
	pthread_spin_lock(&qp->rq_lock);

	if (qp->sq && qp->sq != MAP_FAILED)
		munmap((void *)qp->sq,
			qp->sq_size * sizeof(struct roq_sqe));

	if (qp->rq && qp->rq != MAP_FAILED)
		munmap((void *)qp->rq,
			qp->rq_size * sizeof(struct roq_rqe));

	qp->sq = NULL;
	qp->rq = NULL;

	rv = ibv_cmd_destroy_qp(ofaqp);
	if (rv) {
		pthread_spin_unlock(&qp->rq_lock);
		pthread_spin_unlock(&qp->sq_lock);
		return rv;
	}
	pthread_spin_unlock(&qp->rq_lock);
	pthread_spin_unlock(&qp->sq_lock);

	free(qp);

	return 0;
}

struct ibv_qp *roq_create_qp(struct ibv_pd *pd, struct ibv_qp_init_attr *attr)
{
	struct roq_cmd_create_qp	cmd;
	struct roq_cmd_create_qp_resp	resp;
	struct ibv_context		*ctx = pd->context;
	struct roq_context		*roq_ctx = ctx_ofa2roq(ctx);
	struct roq_qp			*qp;

	int				rv;

	qp = malloc(sizeof *qp);
	if (!qp)
		return NULL;

	memset(qp, 0, sizeof *qp);

	rv = ibv_cmd_create_qp(pd, &qp->ofa_qp, attr, &cmd.ofa_cmd,
			       sizeof cmd, &resp.ofa_resp, sizeof resp);
	if (rv) {
		free(qp);
		return NULL;
	}
	qp->id = resp.qp_id;

	if (roq_ctx->db_req) {
		qp->db_sq = roq_ctx->db_req;
		qp->db_put = &roq_ctx->db_put;
		qp->db_size = roq_ctx->db_size;
	}

	pthread_spin_init(&qp->sq_lock, PTHREAD_PROCESS_PRIVATE);
	pthread_spin_init(&qp->rq_lock, PTHREAD_PROCESS_PRIVATE);

	if (resp.flags == QUEUE_FLAG_MAPPEABLE) {

		int idx;

		qp->sq_size = (resp.sq_size)/sizeof(struct roq_sqe);
		qp->rq_size = (resp.rq_size)/sizeof(struct roq_rqe);
		qp->sq_put = 0;
		qp->rq_put = 0;

		qp->sq = (struct roq_sqe *)mmap(NULL, resp.sq_size,
					PROT_READ|PROT_WRITE, MAP_SHARED,
					ctx->cmd_fd, resp.sq_key);

		if (qp->sq == MAP_FAILED)
			goto fail;

		qp->rq = (struct roq_rqe *)mmap(NULL, resp.rq_size,
					PROT_READ|PROT_WRITE, MAP_SHARED,
					ctx->cmd_fd, resp.rq_key);

		if (qp->rq == MAP_FAILED)
			goto fail;

		/* initialize SQ and RQ arrays */
		memset(&qp->sq[0], 0, resp.sq_size);
		for (idx = 0; idx < qp->sq_size; idx++) 
			qp->sq[idx].flags = ROQ_WQE_COMPLETED;

		memset(&qp->rq[0], 0, resp.rq_size);
		for (idx = 0; idx < qp->rq_size; idx++) 
			qp->rq[idx].flags = ROQ_WQE_COMPLETED;

	} else {
		qp->sq_size = resp.sq_size;
		qp->rq_size = resp.rq_size;
		qp->sq = NULL;
		qp->rq = NULL;
	}
	if (roq_debug > 1 && resp.flags == QUEUE_FLAG_MAPPEABLE)
		printf("RQ size: asked %u, got %u, RQE size %lu\n",
			attr->cap.max_recv_wr, qp->rq_size,
			sizeof(struct roq_rqe));

	return &qp->ofa_qp;

fail:
	if (roq_debug)
		printf("QP create failed\n");

	roq_destroy_qp(&qp->ofa_qp);

	return (struct ibv_qp *) NULL;
}

int roq_modify_qp(struct ibv_qp *ofaqp, struct ibv_qp_attr *attr,
		  int attr_mask)
{
	struct roq_qp		*qp = qp_ofa2roq(ofaqp);
	struct ibv_modify_qp	cmd;
	int			rv;

	pthread_spin_lock(&qp->sq_lock);
	pthread_spin_lock(&qp->rq_lock);
	rv = ibv_cmd_modify_qp(ofaqp, attr, attr_mask, &cmd, sizeof cmd);
	pthread_spin_unlock(&qp->rq_lock);
	pthread_spin_unlock(&qp->sq_lock);

	return rv;
}

struct ibv_ah *roq_create_ah(struct ibv_pd *pd, struct ibv_ah_attr *attr)
{
	return NULL;
}

int roq_destroy_ah(struct ibv_ah *ah)
{
	return -ENOSYS;
}

int roq_attach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid)
{
	return -ENOSYS;
}

int roq_detach_mcast(struct ibv_qp *qp, const union ibv_gid *gid, uint16_t lid)
{
	return -ENOSYS;
}

void roq_async_event(struct ibv_async_event *event)
{

	switch (event->event_type) {

	case IBV_EVENT_CQ_ERR:
		break;

	case IBV_EVENT_QP_FATAL:
	case IBV_EVENT_QP_REQ_ERR:
	case IBV_EVENT_QP_ACCESS_ERR:
		/* TODO: flush qp */
		break;

	case IBV_EVENT_SQ_DRAINED:
	case IBV_EVENT_COMM_EST:
	case IBV_EVENT_QP_LAST_WQE_REACHED:
		break;

	default:
		break;
	}
}
