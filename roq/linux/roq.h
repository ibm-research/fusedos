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

#ifndef _ROQ_H
#define _ROQ_H

#include <linux/idr.h>
#include <linux/hardirq.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#include <rdma/ib_verbs.h>
#include <rdma/iw_cm.h>
#include <rdma/ib_smi.h>
#include <rdma/ib_user_verbs.h>
#include <rdma/ib_pack.h>
#include <rdma/ib_umem.h>

#include "roq_user.h"
#include "roq_debug.h"

#include <roq_device_interface.h>
#include <roq_common.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
#define KERNEL_VERSION_PRE_2_6_26
#endif

/*
 * Fast system call to ring RoQ doorbell bypassing
 * OFA post/poll syscall interface for non-mapped
 * HW doorbell
 */
#if defined __NR_rdma_db
#define DOORBELL_SYSCALL
#endif

#define ROQ_VENDOR_ID	0x2355
#define ROQ_VENDORT_PART_ID	0
#define ROQ_SW_VERSION	1
#define ROQ_VERSION	1

#define ROQ_MAX_QP_WR	(1024 * 32)
#define ROQ_MAX_ORD	RoQ_MAX_OUTSTANDING_RDMA_READS
#define ROQ_MAX_IRD	ROQ_MAX_ORD
#define ROQ_MAX_SGE_RD	ROQ_MAX_SGE
#define ROQ_MAX_INLINE	PAGE_SIZE
#define ROQ_MAX_CQ	(1024 * 100)
#define ROQ_MAX_CQE	(ROQ_MAX_QP_WR * 10)
#define ROQ_MAX_MR	RoQ_MAX_MR
#define ROQ_MAX_MR_SIZE	(1024*1024*1024)
#define ROQ_MAX_QP	RoQ_MAX_QP
#define ROQ_MAX_PD	ROQ_MAX_QP
#define ROQ_MAX_MW	0	/* to be set if MW's are supported */
#define ROQ_MAX_FMR	0
#define ROQ_MAX_SRQ	ROQ_MAX_QP
#define ROQ_MAX_SRQ_WR	(ROQ_MAX_QP_WR * 10)

#define ROQ_NODE_DESC	"RDMA on BG/Q Torus"

/*
 * Softiwarp TX/RX configuration options
 */
#define CONFIG_RDMA_ROQ_CRC_ENFORCED	0

struct roq_devinfo {
	unsigned		device;
	unsigned		version;

	/* close match to ib_device_attr where appropriate */
	u32			vendor_id;
	u32			vendor_part_id;
	u32			sw_version;
	int			max_qp;
	int			max_qp_wr;
	int			max_ord; /* max. outbound read queue depth */
	int			max_ird; /* max. inbound read queue depth */

	enum ib_device_cap_flags	cap_flags;
	int			max_sge;
	int			max_sge_rd;
	int			max_cq;
	int			max_cqe;
	u64			max_mr_size;
	int			max_mr;
	int			max_pd;
	int			max_mw;
	int			max_fmr;
	int			max_srq;
	int			max_srq_wr;
	int			max_srq_sge;
	/* end ib_device_attr */

	enum roq_api_caps	api_caps;
};

#define ROQ_LISTEN_PORT_COUNT (64*1024-1)

struct roq_wq_t {
	struct work_struct	work;
	int			type;
};

struct roq_dev {
	struct ib_device	ofa_dev;
	struct net_device	*l2dev;
	struct roq_devinfo	attrs;
	unsigned int		stopping;

	/* Data Mover Ctrl Interface */
	atomic_t		ctrl_reqs_pending;
	wait_queue_head_t	ctrl_waitq;
	spinlock_t		command_lock;

	/* Discovery state */
	struct workqueue_struct *wq;
	struct roq_wq_t		*work;
	uint8_t			discovery_state;

	/* object management */
	spinlock_t		idr_lock;
	spinlock_t		mr_lock;
	spinlock_t		qp_lock;
	spinlock_t		cq_lock;

	struct list_head	qp_list;

	/* active objects statistics */
	atomic_t		num_qp;
	atomic_t		num_cq;
	atomic_t		num_pd;
	atomic_t		num_mem;
	atomic_t		num_srq;

	/* Data Mover's device control structure */
	struct RoQ_DeviceMemory *RoQ;

	/* need to map a listen port to a structure to eval
	 * an incoming conn req -- this should be much tighter
	 */
	spinlock_t		listen_lock;
	void *ListenPortToPointer[ROQ_LISTEN_PORT_COUNT];
};

struct roq_objhdr {
	struct kref	ref;
	struct roq_dev	*dev;
};

enum roq_uobj_type {
	ROQ_UOBJ_SQ,
	ROQ_UOBJ_RQ,
	ROQ_UOBJ_CQ,
	ROQ_UOBJ_SRQ
};

struct roq_uobj {
	struct list_head	list;
	enum roq_uobj_type	type;
	u64	addr;
	u32	size;
	u32	key;
};
	
#define ROQ_MAX_UOBJ_KEY	(PAGE_SIZE * 4096)

struct roq_ucontext {
	struct ib_ucontext	ofa_ucontext;
	spinlock_t		uobj_lock;
	struct list_head	uobj_list;
	u32			key;
	/* user mapped send queue doorbell */
	u32			sq_db_size;
	dma_addr_t		sq_db_phys;
	void			*sq_db_virt;
	uint64_t		db_id;
};

enum roq_access_flags {
	SR_MEM_LREAD	= (1<<0),
	SR_MEM_LWRITE	= (1<<1),
	SR_MEM_RREAD	= (1<<2),
	SR_MEM_RWRITE	= (1<<3),

	SR_MEM_FLAGS_LOCAL =
		(SR_MEM_LREAD | SR_MEM_LWRITE),
	SR_MEM_FLAGS_REMOTE =
		(SR_MEM_RWRITE | SR_MEM_RREAD)
};


#define ROQ_STAG_MAX	0xffffffff

struct roq_mr;

/*
 * generic memory representation for registered roq memory.
 * memory lookup always via higher 24 bit of stag (stag index).
 * the stag is stored as part of the roq object header (id).
 * object relates to memory window if embedded mr pointer is valid
 */
struct roq_mem {
	struct roq_objhdr	hdr;

	struct roq_mr	*mr;		/* assoc. MR if MW, NULL if MR */

	u32	stag_state	:1,	/* VALID or INVALID */
		is_zbva		:1,	/* zero based virt. addr. */
		mw_bind_enabled	:1,	/* check only if MR */
		remote_inval_enabled:1,	/* VALID or INVALID */
		consumer_owns_key:1,	/* key/index split ? */
		rsvd:27;

	enum roq_access_flags	perms;	/* local/remote READ & WRITE */

	u64	va;		/* VA of memory */
	u32	len;		/* amount of memory bytes */
	u32	fbo;		/* first byte offset */
};


/*
 * NOTE: This structure has no lock and is
 * only modified by the RoQ_CNTL_Thread.
 */
struct roq_mr {
	struct ib_mr		ofa_mr;
	struct roq_dm_mr	*dm_mr;
	struct ib_umem		*umem; /* returned by ib_umem_get() */
};

#define ROQ_MEM_IS_MW(m)	((m)->mr != NULL)
#define ROQ_INLINED_DATA(w)	((w)->m_ib_send_wr.send_flags & IB_SEND_INLINE)

struct roq_cq {
	struct roq_dm_cq	dm_cq;	/* Provider CQ object, must be #1 */
	struct ib_cq		ofa_cq;	/* OFA middleware CQ object */
	spinlock_t		lock;
	dma_addr_t		dma_addr;
	u32			qlen;	/* number of elements */
};

struct roq_qp {
	struct ib_qp		ofa_qp; /* With qp_num used as DM QP index */
	struct list_head	devq;
	struct kref		refcnt;
	struct rw_semaphore	state_lock;
	wait_queue_head_t	ctrl_waitq;
	struct roq_sqe		*sq_virt;
	struct roq_rqe		*rq_virt;
	dma_addr_t	sq_phys;
	dma_addr_t	rq_phys;
	spinlock_t	sq_lock;
	spinlock_t	rq_lock;
	atomic_t	sq_index;
	atomic_t	rq_index;
	u32	sq_size;
	u32	rq_size;

	struct ib_qp_init_attr	init_attrs;

	struct roq_cm_msg	*cm_msg;     /* sent CM msg, resend if lost */
	void 			*conn_data;  /* Private data from conn reply */
	struct roq_cm_work	*cm_work;    /* any scheduled timeout */

	u8	is_user;
	u8	sq_mmapped;
	u8	rq_mmapped;
	u8	is_connecting;
	u8	active_connect;
};

struct roq_srq {
	struct KISS_SRSWQ	DmQueue; /* Provider SRQ object */
	struct ib_srq		ofa_srq;
	struct ib_pd	*ofa_pd;
	spinlock_t	lock;
	unsigned int	max_sge;
	atomic_t	space;  /* current space for posting wqe's */
	unsigned int	limit;  /* low watermark for async event */
	unsigned int	max_wr; /* max # of wqe's allowed */
	char		armed;  /* inform user if limit hit */
	dma_addr_t	srq_phys;

	struct roq_rqe		*rq_virt;
	dma_addr_t		rq_phys;
	u32			rq_alloc_size;

};

extern int gw_rank;
extern int ip_prefix;

#define roq_qpid(x)		(x)->ofa_qp.qp_num
#define OBJ_ID(obj)		(obj)->id

extern struct RoQ_DM_QP *roq_get_dm_qp(struct roq_dev *, uint32_t);
extern struct roq_qp *roq_get_dd_qp(struct roq_dev *, uint32_t);

extern void *roq_kmalloc(int, dma_addr_t *);
extern void *roq_kfree(void *);
extern struct ib_qp *roq_get_ofaqp(struct ib_device *, int);
extern void roq_qp_get_ref(struct ib_qp *);
extern void roq_qp_put_ref(struct ib_qp *);
extern void roq_cm_cancel_timer(struct roq_qp *);

extern int roq_no_mad(struct ib_device *, int, u8, struct ib_wc *,
		      struct ib_grh *, struct ib_mad *, struct ib_mad *);
extern int roq_qp_modify_state(struct roq_qp *, enum ib_qp_state);
extern int roq_ofed_modify_qp(struct ib_qp *, struct ib_qp_attr *, int,
			      struct ib_udata *);
extern int roq_modify_qp(struct ib_qp *, struct ib_qp_attr *, int,
			 struct ib_udata *);
extern struct roq_cq *cq_array[];
extern uint64_t roq_propagate_db(struct roq_dev *, dma_addr_t, dma_addr_t,
				 enum roq_db_client client);
extern void roq_revoke_db(struct roq_dev *, uint64_t, enum roq_db_client);
extern void roq_ctrl_slot_put(struct roq_dev *);
extern int roq_ctrl_slot_get(struct roq_dev *);

#endif
