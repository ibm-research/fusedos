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

#ifndef __ROQ_UTILS_H
#define __ROQ_UTILS_H

extern void __roq_utils_mem_print(char *mem_name, unsigned char *kva,
				  unsigned int num_bytes);
extern void __roq_utils_kvec_print(char *vec_name, struct kvec *vec,
				  unsigned int num_elts);

extern void __roq_print_qp_attr_mask(enum ib_qp_attr_mask);
extern void __roq_print_ib_wr_send(struct ib_send_wr *);
extern void __roq_print_ib_wr_recv(struct ib_recv_wr *);
extern void __roq_print_umem(struct ib_umem *);
extern void __roq_dump_bytes(void *, int);

extern char ib_qp_state_to_string[IB_QPS_ERR+1][6];

static inline struct roq_dev *roq_dev_ofa2roq(struct ib_device *ofa_dev)
{
	return container_of(ofa_dev, struct roq_dev, ofa_dev);
}

static inline struct roq_qp *
roq_qp_ofa2roq(struct ib_qp *ofa_qp)
{
	return container_of(ofa_qp, struct roq_qp, ofa_qp);
}

static inline struct roq_ucontext *
roq_ctx_ofa2roq(struct ib_ucontext *ofa_ctx)
{
	return container_of(ofa_ctx, struct roq_ucontext, ofa_ucontext);
}

static inline struct roq_cq *
roq_cq_ofa2roq(struct ib_cq *ofa_cq)
{
	return container_of(ofa_cq, struct roq_cq, ofa_cq);
}

static inline struct roq_srq *
roq_srq_ofa2roq(struct ib_srq *ofa_srq)
{
	return container_of(ofa_srq, struct roq_srq, ofa_srq);
}

#endif
