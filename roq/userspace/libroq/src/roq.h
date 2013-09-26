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

#ifndef _ROQ_H
#define _ROQ_H

#include <pthread.h>
#include <inttypes.h>
#include <stddef.h>

#include <infiniband/driver.h>
#include <infiniband/arch.h>

#include <roq_common.h>

 
struct roq_device {
	struct ibv_device	ofa_dev;
	enum roq_api_caps	api_caps; /* OFA or private fast path */
	pthread_spinlock_t	lock;
};

struct roq_pd {
	struct ibv_pd	ofa_pd;
};

struct roq_srq {
	struct ibv_srq		ofa_srq;
	pthread_spinlock_t	lock;
};

struct roq_mr {
	struct ibv_mr	ofa_mr;
	uint64_t	fbo;
	uint32_t	pbl_addr;
	uint32_t	len;
};

struct roq_qp {
	struct ibv_qp		ofa_qp;
	struct roq_device	*roq_dev;

	uint32_t		id;
	uint32_t		sq_size;
	uint32_t		rq_size;

	struct roq_sqe		*sq;
	uint64_t		sq_put;

	struct roq_rqe		*rq;
	uint64_t		rq_put;

	union roq_tx_db_req	*db_sq;
	uint32_t		*db_put;
	uint32_t		db_size;

	pthread_spinlock_t	sq_lock;
	pthread_spinlock_t	rq_lock;
};

struct roq_cq {
	struct ibv_cq		ofa_cq;
	struct roq_device	*roq_dev;

	pthread_spinlock_t	lock;

	uint32_t		id;
	uint32_t		cq_size;

	struct roq_cqe		*wq;
	uint32_t		cq_get;
};


struct roq_context {
	struct ibv_context	ofa_ctx;
	union roq_tx_db_req	*db_req;
	uint32_t		db_size;
	uint32_t		db_put;
};


#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


/* from <linux/kernel.h> */
#ifndef container_of
	#define container_of(ptr, type, member) ({                      \
		const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
		(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#define ctx_ofa2roq(ibv_ctx) container_of(ibv_ctx, struct roq_context, ofa_ctx)
#define	dev_ofa2roq(ibv_dev) container_of(ibv_dev, struct roq_device, ofa_dev)
#define	qp_ofa2roq(ibv_qp)   container_of(ibv_qp, struct roq_qp, ofa_qp)
#define	cq_ofa2roq(ibv_cq)   container_of(ibv_cq, struct roq_cq, ofa_cq)
#define	mr_ofa2roq(ibv_mr)   container_of(ibv_mr, struct roq_mr, ofa_mr)
#define	srq_ofa2roq(ibv_srq) container_of(ibv_srq, struct roq_srq, ofa_srq)

extern int roq_query_device(struct ibv_context *, struct ibv_device_attr *);
extern int roq_query_port(struct ibv_context *, uint8_t, struct ibv_port_attr *);
/*
 * atr: Adding support for ibv_query_qp
 */
extern int roq_query_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr,
                        int attr_mask,
                        struct ibv_qp_init_attr *init_attr);

extern struct ibv_pd *roq_alloc_pd(struct ibv_context *);
extern int roq_free_pd(struct ibv_pd *);
extern struct ibv_mr *roq_reg_mr(struct ibv_pd *, void *, size_t, int);
extern int roq_dereg_mr(struct ibv_mr *);
extern struct ibv_cq *roq_create_cq(struct ibv_context *, int,
				   struct ibv_comp_channel *, int);
extern int roq_resize_cq(struct ibv_cq *, int);
extern int roq_destroy_cq(struct ibv_cq *);

extern int roq_notify_cq_ofed(struct ibv_cq *, int);
extern int roq_notify_cq_mapped(struct ibv_cq *, int);
extern int roq_poll_cq_ofed(struct ibv_cq *, int, struct ibv_wc *);
extern int roq_poll_cq_mapped(struct ibv_cq *, int, struct ibv_wc *);

extern struct ibv_srq *roq_create_srq(struct ibv_pd *,
				     struct ibv_srq_init_attr *);
extern int roq_modify_srq(struct ibv_srq *, struct ibv_srq_attr *, int);
extern int roq_destroy_srq(struct ibv_srq *);

extern int roq_post_srq_recv(struct ibv_srq *, struct ibv_recv_wr *,
			    struct ibv_recv_wr **);
extern struct ibv_qp *roq_create_qp(struct ibv_pd *, struct ibv_qp_init_attr *);
extern int roq_modify_qp(struct ibv_qp *, struct ibv_qp_attr *, int);
extern int roq_destroy_qp(struct ibv_qp *);

extern int roq_post_send_ofed(struct ibv_qp *, struct ibv_send_wr *,
			     struct ibv_send_wr **);
extern int roq_post_send_mmap(struct ibv_qp *, struct ibv_send_wr *,
			      struct ibv_send_wr **);
extern int roq_post_send_mmap_db(struct ibv_qp *, struct ibv_send_wr *,
				 struct ibv_send_wr **);
extern int roq_post_recv_ofed(struct ibv_qp *, struct ibv_recv_wr *,
			      struct ibv_recv_wr **);
extern int roq_post_recv_mmap(struct ibv_qp *, struct ibv_recv_wr *,
			      struct ibv_recv_wr **);
extern int roq_post_srq_recv_ofed(struct ibv_srq *, struct ibv_recv_wr *,
				  struct ibv_recv_wr **);

extern struct ibv_ah *roq_create_ah(struct ibv_pd *, struct ibv_ah_attr *);
extern int roq_destroy_ah(struct ibv_ah *);

extern int roq_attach_mcast(struct ibv_qp *, const union ibv_gid *, uint16_t);
extern int roq_detach_mcast(struct ibv_qp *, const union ibv_gid *, uint16_t);
extern void roq_async_event(struct ibv_async_event *);


#endif	/* _ROQ_H */
