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

#ifndef _ROQ_USER_H
#define _ROQ_USER_H

/*
 * user commands/command responses must correlate with the roq_abi.h
 * in user land.
 */

struct roq_ureq_create_cq {
	/* not used */
};

struct roq_ureq_alloc_ctx {
	__u32	flags;
};

struct roq_uresp_alloc_ctx {
	__u32	db_size;
	__u32	db_key;
	__u32	flags;
	__u32	db_syscall;
};

struct roq_uresp_create_cq {
	__u32	cq_id;
	__u32	cq_size;
	__u32	cq_key;
	__u32	flags;
};

struct roq_ureq_create_qp {
	/* not used */
};


struct roq_uresp_create_qp {
	__u32	qp_id;
	__u32	sq_size;
	__u32	rq_size;
	__u32	sq_key;
	__u32	rq_key;
	__u32	flags;
};

struct roq_uresp_reg_mr {
	__u32	stag;
	__u32	pbl_addr;
};

struct roq_ureq_reg_mr {
	__u8	stag_key;
	__u8	reserved[3];
	__u32	pbl_addr;
};

#endif
