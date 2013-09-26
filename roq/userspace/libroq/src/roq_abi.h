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

#ifndef _ROQ_ABI_H
#define _ROQ_ABI_H


#include <infiniband/kern-abi.h>

/*
 * request/response structures for resource allocation calls
 */

struct roq_alloc_ucontext_req {
	struct ibv_get_context ofa_req;
	__u32	flags;
};

struct roq_alloc_ucontext_resp {
	struct ibv_get_context_resp ofa_resp;
	__u32	db_size;
	__u32	db_key;
	__u32	flags;
	__u32	db_syscall;
};

struct roq_alloc_pd {
	struct ibv_alloc_pd ofa_cmd;
};
	
struct roq_alloc_pd_resp {
	struct ibv_alloc_pd_resp ofa_resp;
	uint32_t pd_id;
};

struct roq_cmd_reg_umr_req {
	struct ibv_reg_mr ofa_req;
	uint8_t	 stag_key;
	uint8_t reserved[3];
};

struct roq_cmd_reg_umr_resp {
	struct ibv_reg_mr_resp ofa_resp;
	uint32_t stag;
};

struct roq_cmd_create_cq {
	struct ibv_create_cq ofa_cmd;
};

struct roq_cmd_create_cq_resp {
	struct ibv_create_cq_resp ofa_resp;
	uint32_t cq_id;
	uint32_t cq_size;
	uint32_t cq_key;
	uint32_t flags;
};

struct roq_cmd_create_qp {
	struct ibv_create_qp ofa_cmd;
};

struct roq_cmd_create_qp_resp {
	struct ibv_create_qp_resp ofa_resp;
	uint32_t qp_id;
	uint32_t sq_size;
	uint32_t rq_size;
	uint32_t sq_key;
	uint32_t rq_key;
	uint32_t flags;
};

struct roq_cmd_create_srq {
	struct ibv_create_srq ofa_cmd;
};

struct roq_cmd_create_srq_resp {
	struct ibv_create_srq_resp ofa_resp;
};
#endif	/* _ROQ_ABI_H */
