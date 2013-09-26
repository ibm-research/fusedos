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

#include "roq.h"
#include "roq_abi.h"


#define ROQ_NODE_DESC		"RDMA on BG/Q Torus"

extern int __bg_cpu_count = 1;
const int roq_debug = 0;
const int do_poll_syscall = 0;
int roq_db_syscall;
const int user_mapped_db = 1;

static struct ibv_context_ops roq_context_ops = {
	.query_device	= roq_query_device,
	.query_port	= roq_query_port,
	.query_qp       = roq_query_qp,
	.alloc_pd	= roq_alloc_pd,
	.dealloc_pd	= roq_free_pd,
	.reg_mr		= roq_reg_mr,
	.dereg_mr	= roq_dereg_mr,
	.create_cq	= roq_create_cq,
	.resize_cq	= roq_resize_cq,
	.destroy_cq	= roq_destroy_cq,
	.create_srq	= roq_create_srq,
	.modify_srq	= roq_modify_srq,
	.destroy_srq	= roq_destroy_srq,
	.create_qp	= roq_create_qp,
	.modify_qp	= roq_modify_qp,
	.destroy_qp	= roq_destroy_qp,
	.create_ah	= roq_create_ah,
	.destroy_ah	= roq_destroy_ah,
	.attach_mcast	= roq_attach_mcast,
	.detach_mcast	= roq_detach_mcast,
	.async_event	= roq_async_event,
};

static void print_caps(unsigned long caps)
{
	if (!caps || roq_debug <= 1)
		return;

	printf("caps = |");

	if (caps & ROQ_MAPPED_SQ)
		printf("MAPPED_SQ|");
	if (caps & ROQ_MAPPED_RQ)
		printf("MAPPED_RQ|");
	if (caps & ROQ_MAPPED_CQ)
		printf("MAPPED_CQ|");
	if (caps & ROQ_SYSCALL_DB)
		printf("DOORBELL|");
	if (caps >= ROQ_CAP_MAX)
		printf(" + unknown %lx|",
		       caps & ~(ROQ_MAPPED_SQ|ROQ_MAPPED_RQ|
				ROQ_MAPPED_CQ|ROQ_SYSCALL_DB));

	printf("\n");
}

static struct ibv_context *roq_alloc_context(struct ibv_device *ofa_dev, int fd)
{
	struct roq_context *ctx;
	struct roq_alloc_ucontext_req req;
	struct ibv_get_context *cmd = &req.ofa_req;;
	struct roq_alloc_ucontext_resp resp;
	struct roq_device *roq_dev = dev_ofa2roq(ofa_dev);

	ctx = malloc(sizeof *ctx);
	if (!ctx)
		return NULL;

	memset(ctx, 0, sizeof *ctx);
	memset(&req, 0, sizeof req);

	ctx->ofa_ctx.cmd_fd = fd;

	if (user_mapped_db)
		req.flags = QUEUE_FLAG_MAPPEABLE;

	if (ibv_cmd_get_context(&ctx->ofa_ctx, cmd, sizeof req,
				&resp.ofa_resp, sizeof resp)) {
		free(ctx);
		return NULL;
	}
	ctx->ofa_ctx.device = ofa_dev;
	ctx->ofa_ctx.ops = roq_context_ops;

	if (roq_dev->api_caps == ROQ_OFA_CALL) {
		/*
		 * minimum functionality: no explicit resource
		 * mapping nor doorbell system call support.
		 * All fast path operations via OFA mid layer.
		 */
		ctx->ofa_ctx.ops.post_send = roq_post_send_ofed;
		ctx->ofa_ctx.ops.post_recv = roq_post_recv_ofed;
		ctx->ofa_ctx.ops.post_srq_recv = roq_post_srq_recv_ofed;
		ctx->ofa_ctx.ops.poll_cq = roq_poll_cq_ofed;
		ctx->ofa_ctx.ops.req_notify_cq = roq_notify_cq_ofed;
	} else {
		if ((roq_dev->api_caps & ROQ_MAPPED_SQ) &&
		    (roq_dev->api_caps & ROQ_SYSCALL_DB))
			ctx->ofa_ctx.ops.post_send = roq_post_send_mmap_db;
		else if (roq_dev->api_caps & ROQ_MAPPED_SQ)
			ctx->ofa_ctx.ops.post_send = roq_post_send_mmap;
		else
			ctx->ofa_ctx.ops.post_send = roq_post_send_ofed;

#if 1
		if (roq_dev->api_caps & ROQ_MAPPED_CQ)
			ctx->ofa_ctx.ops.poll_cq = roq_poll_cq_mapped;
		else
			ctx->ofa_ctx.ops.poll_cq = roq_poll_cq_ofed;

		if (roq_dev->api_caps & ROQ_SYSCALL_DB)
			ctx->ofa_ctx.ops.req_notify_cq =
					roq_notify_cq_mapped;
		else
			ctx->ofa_ctx.ops.req_notify_cq = roq_notify_cq_ofed;
#else
		if ((roq_dev->api_caps & ROQ_MAPPED_CQ) &&
		    (roq_dev->api_caps & ROQ_SYSCALL_DB)) {
			ctx->ofa_ctx.ops.poll_cq = roq_poll_cq_mapped;
			ctx->ofa_ctx.ops.req_notify_cq =
					roq_notify_cq_mapped;
		} else {
			ctx->ofa_ctx.ops.poll_cq = roq_poll_cq_ofed;
			ctx->ofa_ctx.ops.req_notify_cq = roq_notify_cq_ofed;
		}
#endif
		/*
 		 * to be done: mapped SRQ
 		 */
		if (roq_dev->api_caps & ROQ_MAPPED_RQ)
			ctx->ofa_ctx.ops.post_recv = roq_post_recv_mmap;
		else
			ctx->ofa_ctx.ops.post_recv = roq_post_recv_ofed;

		ctx->ofa_ctx.ops.post_srq_recv = roq_post_srq_recv_ofed;
	}

	if (resp.flags & QUEUE_FLAG_MAPPEABLE && resp.db_size) {
		ctx->db_size = resp.db_size;
		if (roq_debug > 1)
			printf("Try mapping doorbell\n");
		resp.db_size *= sizeof(union roq_tx_db_req);
		ctx->db_req = (union roq_tx_db_req *)mmap(NULL, resp.db_size,
							  PROT_READ|PROT_WRITE,
							  MAP_SHARED,
							  ctx->ofa_ctx.cmd_fd,
							  resp.db_key);

		if (!ctx->db_req || ctx->db_req == MAP_FAILED) {
			ctx->db_req = NULL;
			if (roq_debug)
				printf("mapping DB failed\n");
			goto out;
		} 
	}
	roq_db_syscall = resp.db_syscall;
	if (roq_debug)
		printf("roq DB syscall: %d\n", roq_db_syscall);
out:
	return &ctx->ofa_ctx;
}

static void roq_free_context(struct ibv_context *ofa_ctx)
{
	struct roq_context *ctx = ctx_ofa2roq(ofa_ctx);

	if (ctx->db_req)
		munmap((void *)ctx->db_req,
			ctx->db_size * sizeof(union roq_tx_db_req));
	free(ctx);
}

static struct ibv_device_ops roq_dev_ops = {
	.alloc_context = roq_alloc_context,
	.free_context = roq_free_context
};

static struct ibv_device *roq_driver_init(const char *uverbs_sys_path,
					  int abi_version)
{
	char			value[16],
				roq_devpath[IBV_SYSFS_PATH_MAX],
				node_desc[24];
	struct roq_device	*dev;
	unsigned long		api_caps;

	if (ibv_read_sysfs_file(uverbs_sys_path, "ibdev",
				value, sizeof value) < 0)
		return NULL;

	memset(roq_devpath, 0, IBV_SYSFS_PATH_MAX);

	snprintf(roq_devpath, IBV_SYSFS_PATH_MAX, "%s/class/infiniband/%s",
		 ibv_get_sysfs_path(), value);

	if (ibv_read_sysfs_file(roq_devpath, "node_desc",
       				node_desc, sizeof node_desc) < 0)
		return NULL;

	if (strncmp(ROQ_NODE_DESC, node_desc, strlen(ROQ_NODE_DESC)))
		return NULL;

	if (ibv_read_sysfs_file(roq_devpath, "api_caps",
       				value, sizeof value) < 0)
		return NULL;

	errno = 0;
	api_caps =  strtoul(value, NULL, 10);
	if (errno)
		return NULL;

	print_caps(api_caps);
	
	dev = malloc(sizeof *dev);
	if (!dev)
		return NULL;

	pthread_spin_init(&dev->lock, PTHREAD_PROCESS_PRIVATE);
	dev->ofa_dev.ops = roq_dev_ops;
	dev->api_caps = (enum roq_api_caps)api_caps;

	return &dev->ofa_dev;
}

static __attribute__((constructor)) void roq_register_driver(void)
{
	ibv_register_driver("roq", roq_driver_init);
}
