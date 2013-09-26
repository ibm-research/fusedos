/*
 * RoQ device driver for Linux
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _ROQ_VERBS_H
#define _ROQ_VERBS_H

#include <linux/errno.h>

#include <rdma/iw_cm.h>
#include <rdma/ib_verbs.h>
#include <rdma/ib_smi.h>
#include <rdma/ib_user_verbs.h>

#include "roq.h"
#include "roq_cm.h"

int roq_query_device(struct ib_device *, struct ib_device_attr *);

struct ib_ucontext *roq_alloc_ucontext(struct ib_device *,
				       struct ib_udata *);
int roq_dealloc_ucontext(struct ib_ucontext *);


int roq_query_port(struct ib_device *, u8, struct ib_port_attr *);
int roq_query_pkey(struct ib_device *, u8, u16, u16 *);
int roq_query_gid(struct ib_device *, u8, int, union ib_gid *);

struct ib_pd *roq_alloc_pd(struct ib_device *, struct ib_ucontext *,
			  struct ib_udata *);
int roq_dealloc_pd(struct ib_pd *);
struct ib_ah *roq_create_ah(struct ib_pd *, struct ib_ah_attr *);
int roq_destroy_ah(struct ib_ah *);


struct ib_qp *roq_create_qp(struct ib_pd *, struct ib_qp_init_attr *,
			   struct ib_udata *);
int roq_ofed_modify_qp(struct ib_qp *, struct ib_qp_attr *, int,
		      struct ib_udata *);
int roq_query_qp(struct ib_qp *qp, struct ib_qp_attr *qp_attr,
		 int qp_attr_mask, struct ib_qp_init_attr *qp_init_attr);
int roq_destroy_qp(struct ib_qp *);



int roq_post_send(struct ib_qp *, struct ib_send_wr *,
		  struct ib_send_wr **);
int roq_post_receive(struct ib_qp *, struct ib_recv_wr *,
		     struct ib_recv_wr **);


struct ib_cq *roq_create_cq(struct ib_device *, int, int,
			    struct ib_ucontext *, struct ib_udata *);
int roq_destroy_cq(struct ib_cq *);
int roq_poll_cq(struct ib_cq *, int num_entries, struct ib_wc *);
int roq_req_notify_cq(struct ib_cq *, enum ib_cq_notify_flags);


struct ib_mr *roq_reg_user_mr(struct ib_pd *, u64, u64, u64, int,
			      struct ib_udata *);
struct ib_mr *roq_get_dma_mr(struct ib_pd *, int);
int roq_dereg_mr(struct ib_mr *);


struct ib_srq *roq_create_srq(struct ib_pd *, struct ib_srq_init_attr *,
			     struct ib_udata *);
int roq_modify_srq(struct ib_srq *, struct ib_srq_attr *,
		  enum ib_srq_attr_mask, struct ib_udata *);
int roq_query_srq(struct ib_srq *, struct ib_srq_attr *);
int roq_destroy_srq(struct ib_srq *);
int roq_post_srq_recv(struct ib_srq *, struct ib_recv_wr *,
		     struct ib_recv_wr **);

int roq_mmap(struct ib_ucontext *, struct vm_area_struct *);

#endif
