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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/rtnetlink.h>
#include <linux/if_arp.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/version.h>
#include <net/net_namespace.h>

#ifndef USERSPACE_EXTENSION
#define _NEED_ROQ_MAGIC_LOCATION
#endif
#include <firmware/include/personality.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/BIC.h>

#include <spi/include/kernel/klinux/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/MU.h>

#if !defined(__FUSEDOS__) || LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0) || LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
#include <asm/bluegene.h>
#else
#include <platforms/bgq/bic.h>
#endif
#include "roq.h"
#include "roq_cm.h"
#include "roq_verbs.h"
#include "roq_utils.h"

MODULE_DESCRIPTION("Blue Gene/Q I/O Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

int ip_prefix=12;
int gw_rank=-1;
module_param(ip_prefix, uint, 0);
module_param(gw_rank, int, 0);

#undef KERN_INFO
#define KERN_INFO KERN_EMERG

extern bool mapped_qp;
extern int __init roq_eth_init_module(struct net_device **,
				      struct RoQ_DeviceMemory *);
extern void __exit roq_eth_exit_module(struct net_device *);
extern int roq_eth_rem_init_qp(struct net_device *);
extern int bluegene_register_link_shutdown_handler(int (*handler)(u32));

#ifdef DOORBELL_SYSCALL
extern long roq_doorbell(u32, u32, u32);
long (*db_orig_call) (u32, u32, u32);
extern long (*doorbell_call)(u32, u32, u32);
#endif
extern bool mapped_cq;

extern atomic_t qp_db_idx;
extern atomic_t num_mapped_db;
extern volatile union roq_tx_db_req *roq_qp_db;

struct roq_net_event *roq_event_queue;
uint32_t roq_eventq_idx;


extern bool debug_cm;

#ifdef USERSPACE_EXTENSION
void __exit roq_usmu_module_exit(void);
uint64_t __init roq_usmu_module_init(void);
#endif

/* global variables */
extern struct device roq_generic_device;
extern struct ib_dma_mapping_ops roq_dma_mapping_ops;

struct roq_dev *roq_device = NULL;
uint64_t *roq_fsm;
unsigned int roq_active = 0;
DEFINE_SPINLOCK(roq_dev_lock);

volatile unsigned int *roq_cq_db = NULL;
atomic_t cq_db_idx;

static inline u32 __roq_poll_cq_db(void)
{
	int idx = atomic_read(&cq_db_idx) % ROQ_CQ_DB_SIZE;
	u32 cq_id =  *(volatile u32 *)&roq_cq_db[idx];

	if (cq_id) {
		/* increment index and clear old entry */
		atomic_inc(&cq_db_idx);
		set_mb(roq_cq_db[idx], 0);
	}
	return cq_id;
}

static int roq_poll_cq_doorbell(void)
{
	u32 cq_id;
	int polled = 0;
	struct roq_cq *cq = NULL;

poll_again:
	spin_lock(&roq_device->cq_lock);

	if (unlikely(!roq_cq_db))
		goto out;

	cq_id = __roq_poll_cq_db();
	if (cq_id) {

		if (unlikely(cq_id > ROQ_MAX_CQ)) {
			pr_warn("roq_poll_cq_doorbell: invalid CQ ID %d\n",
				cq_id);
			goto out;
		}

		cq = cq_array[cq_id];

		if (unlikely(!cq)) {
			pr_warn("CQ DB: no driver CQ %u\n", cq_id);
			spin_unlock(&roq_device->cq_lock);
			goto poll_again;
		}
		polled++;
		if (cq->ofa_cq.comp_handler)
			(*cq->ofa_cq.comp_handler)
				(&cq->ofa_cq, cq->ofa_cq.cq_context);
		else
			pr_warn("CQ[%d]: NULL completion handler\n", cq_id);

		spin_unlock(&roq_device->cq_lock);

		goto poll_again;
	}
out:
	spin_unlock(&roq_device->cq_lock);

	return polled;
}

static uint64_t where_the_roq_at;

void
roq_wq_link_shutdown(void)
{
	char* argv[] = {"/etc/init.d/roq_link_shutdown", NULL};
	char* env[] = {	"HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
			"LD_LIBRARY_PATH=/lib:/usr/lib",
			NULL};
	call_usermodehelper(argv[0], argv, env, 0);
}

static void
roq_wq_function(struct work_struct *w)
{
	struct roq_wq_t *work = (struct roq_wq_t *)w;

	pr_info("roq_wq_function: work type %d\n", work->type);

	switch (work->type) {
	case 1:
	//pr_info("%s:%d\n", __func__, __LINE__);
	//printk("%s:%d\n", __func__, __LINE__);
		if (roq_eth_rem_init_qp(roq_device->l2dev) == 0)
		{	
	pr_info("%s:%d\n", __func__, __LINE__);
	//printk("%s:%d\n", __func__, __LINE__);
			roq_device->discovery_state = 1;
		}
		break;
	case 2:
		roq_wq_link_shutdown();
		break;
	}
	roq_device->work = NULL;
	kfree(work);
}

int
link_shutdown_handler(uint32_t block_id)
{
	roq_device->discovery_state = 0;
	if (roq_device->wq) {
		roq_device->work =
			(struct roq_wq_t *)kmalloc(sizeof(struct roq_wq_t),
						   GFP_KERNEL);
		if (roq_device->work) {
			INIT_WORK((struct work_struct *)roq_device->work,
				  roq_wq_function);
			roq_device->work->type = 2;
			queue_work(roq_device->wq,
				   (struct work_struct *)roq_device->work);
		}
	}
	return 0;
}

/* SYSFS (START) */
#define ROQ_ATTR(NAME, FMT ...) \
	static ssize_t \
	NAME##_show(struct device *dev, struct device_attribute *attr, char *buf) \
	{ \
		ssize_t l = sprintf(buf, FMT); \
		return l; \
	} \
	static DEVICE_ATTR(NAME, 0444, NAME##_show, NULL);

static ssize_t
DISCOVERY_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct RoQ_DeviceMemory *RoQ = roq_device->RoQ;
	uint32_t rank;
	static int max_show = 5;
	ssize_t l;

	if (roq_device->discovery_state == 1)
		goto out;

	RoQ_TorusCoords_To_Rank(
	    (struct RoQ_NetworkDescriptor *)&roq_device->RoQ->mNetworkDesc,
	                           roq_device->RoQ->mNetworkDesc.Acoord,
	                           roq_device->RoQ->mNetworkDesc.Bcoord,
	                           roq_device->RoQ->mNetworkDesc.Ccoord,
	                           roq_device->RoQ->mNetworkDesc.Dcoord,
	                           roq_device->RoQ->mNetworkDesc.Ecoord,
	                           &rank);

	if (max_show && max_show-- > 0)
		pr_info("discovery: rank %d, state %d\n",
			rank, roq_device->discovery_state);

	if (rank == 0) {
		struct RoQ_CNTL_Command	*CtrlCmd;
		uint64_t my_req;
		int rv = roq_ctrl_slot_get(roq_device);
		if (rv) {
			pr_info("discovery failed\n");
			goto out;
		}

		spin_lock(&roq_device->command_lock);

		my_req= *(unsigned long volatile *)&RoQ->CNTL_RequestPosted;

		CtrlCmd = &RoQ->CNTL_Command[my_req % ROQ_CTRL_Q_SIZE];
		CtrlCmd->Type = RoQ_CNTL_Command_Discovery;

		wmb();

		set_mb(RoQ->CNTL_RequestPosted, my_req + 1);

		spin_unlock(&roq_device->command_lock);

		rv = wait_event_interruptible(roq_device->ctrl_waitq,
					      roq_device->stopping ||
			*(unsigned long volatile *)&RoQ->CNTL_RequestCompleted >
			my_req);

		roq_ctrl_slot_put(roq_device);
	}
out:
	l = sprintf(buf, "%d\n", roq_device->discovery_state);

	return l;
}


static ssize_t
api_caps_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	unsigned int api_caps;
#ifdef DOORBELL_SYSCALL
	api_caps = ROQ_SYSCALL_DB;
#else
	api_caps = ROQ_OFA_CALL;
#endif
	if (mapped_qp)
		api_caps |= ROQ_MAPPED_SQ|ROQ_MAPPED_RQ;
	if (mapped_cq)
		api_caps |= ROQ_MAPPED_CQ;

	return sprintf(buf, "%u\n", api_caps);
//	return sprintf(buf, "%u\n", roq_device->attrs.api_caps);
}

static ssize_t
tx_db_array_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t len;
	int i, block = 0, line = 0, start = 1;
	char mybuf[512];
	union roq_tx_db_req	*db_req;
	
	if (!roq_qp_db)
		return sprintf(buf, "no tx db\n");
	
	db_req = (union roq_tx_db_req *)roq_qp_db;
	i = atomic_read(&qp_db_idx);
	len = sprintf(buf, "DB at 0x%p, idx %d (%d)\n", roq_qp_db, i % ROQ_SQ_DB_SIZE, i);

	buf = mybuf;

	for (i = 0; i < ROQ_SQ_DB_SIZE; i++) {
		ssize_t l;
		if (start) {
			printk("send db, idx: %d\n",
				atomic_read(&qp_db_idx) % ROQ_SQ_DB_SIZE);
			start = 0;
		}
		if (block == 0) {
			buf += sprintf(buf, "%d\t", line);
			line += 40;
		}
		l = sprintf(buf, "|%u %u",
			    db_req->qp_id,
			    db_req->num_wqe);
		if (++block == 40) {
			printk("%s\n", mybuf);
			buf = mybuf;
			block = 0;
		} else
			buf += l;
		db_req++;
	}
	buf = mybuf;
	start = 1;
	block = 0;
	line = 0;
	for (i = 0; i < ROQ_RQ_DB_SIZE; i++) {
		ssize_t l;
		if (start) {
			printk("read db:\n");
			start = 0;
		}
		if (block == 0) {
			buf += sprintf(buf, "%d\t", line);
			line += 40;
		}
		l = sprintf(buf, "|%u %u",
			    db_req->qp_id,
			    db_req->num_wqe);
		if (++block == 40) {
			printk("%s\n", mybuf);
			buf = mybuf;
			block = 0;
		} else
			buf += l;
		db_req++;
	}
	return len;
}
	

extern struct roq_sock_transport *trans;

static ssize_t
active_qps_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int i;
	if (!roq_device)
		return 0;

	spin_lock_bh(&roq_device->qp_lock);
	for (i = 1; i < ROQ_MAX_QP; i++) {
		struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(roq_device, i);
		struct roq_qp *dd_qp = roq_get_dd_qp(roq_device, i);
		struct iw_cm_id	*id;
		int sq_idx, sq_idx_prev;

		/* If the dd_qp is NULL the dm_qp is not used */
		if (!dd_qp)
			continue;

		id = dm_qp->cm_id;
		sq_idx = atomic_read(&dd_qp->sq_index) % dd_qp->sq_size;
		sq_idx_prev = (atomic_read(&dd_qp->sq_index) - 1) %
				dd_qp->sq_size;

		pr_info("QP[%d] at %p: state %d, cmid %p, laddr %x, lport %u, "
			"raddr %x, rport %u, k_sq_idx %d, dm_sq_get %llu, "
			"sqe %d:%d -- %d:%d, "
			"num_ref %d\n",
			roq_qpid(dd_qp), dd_qp, dm_qp->qp_state, id,
			id ? id->local_addr.sin_addr.s_addr : 0,
			id ? id->local_addr.sin_port : 0,
			id ? id->remote_addr.sin_addr.s_addr : 0,
			id ? id->remote_addr.sin_port : 0,
			sq_idx,
			dm_qp->mSendQueue.mPut,
			dd_qp->sq_virt[sq_idx_prev].flags & ROQ_WQE_VALID,
			dd_qp->sq_virt[sq_idx_prev].flags & ROQ_WQE_COMPLETED,
			dd_qp->sq_virt[sq_idx].flags & ROQ_WQE_VALID,
			dd_qp->sq_virt[sq_idx].flags & ROQ_WQE_COMPLETED,
			atomic_read(&dd_qp->refcnt.refcount));
		if (dd_qp->sq_virt[sq_idx_prev].flags & ROQ_WQE_VALID) {
			struct roq_sqe *sqe = dd_qp->sq_virt;
			for (sq_idx = 0; sq_idx < dd_qp->sq_size; sq_idx++) {
				pr_info("%d|%d ", sqe->flags & ROQ_WQE_VALID,
					sqe->flags & ROQ_WQE_COMPLETED);
				sqe++;
			}
			pr_info("\n");
		}
		roq_qp_put_ref(&dd_qp->ofa_qp);
	}
	spin_unlock_bh(&roq_device->qp_lock);

	pr_info("RX free queue: %d\n", atomic_read(&trans->rx_free));
	return 0;
}

static DEVICE_ATTR(active_qps, 0444, active_qps_show, NULL);

static DEVICE_ATTR(tx_db_array, 0444, tx_db_array_show, NULL);

static ssize_t
cq_db_array_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int i, column = 0, line = 0, start = 1;
	char mybuf[512];
	unsigned int *db_req;
	
	if (!roq_cq_db)
		return sprintf(buf, "no tx db\n");
	
	if (!(spin_trylock(&roq_device->cq_lock)))
		goto out_nonlocked;

	db_req = (unsigned int *)roq_cq_db;

	for (i = 0; i < ROQ_CQ_DB_SIZE; i++)
		if (roq_cq_db[i] != 0)
			break;

	if (i >= ROQ_CQ_DB_SIZE)
		goto out;

	buf = mybuf;

	for (i = 0; i < ROQ_CQ_DB_SIZE; i++) {
		ssize_t l;
		if (start) {
			printk("cq db idx: %d\n",
				atomic_read(&cq_db_idx) % ROQ_CQ_DB_SIZE);
			start = 0;
		}
		if (column == 0) {
			buf += sprintf(buf, "%d\t", line);
			line += 40;
		}
		l = sprintf(buf, "|%u", *db_req);
		if (++column == 40) {
			printk("%s\n", mybuf);
			buf = mybuf;
			column = 0;
		} else
			buf += l;
		db_req++;
	}
	if (column) {
		*buf = '\0';
		printk("%s\n", mybuf);
	}
out:
	spin_unlock(&roq_device->cq_lock);
out_nonlocked:
	return 0;
}
	
static DEVICE_ATTR(cq_db_array, 0444, cq_db_array_show, NULL);
static DEVICE_ATTR(DISCOVERY, 0444, DISCOVERY_show, NULL);
static DEVICE_ATTR(api_caps, 0444, api_caps_show, NULL);

ROQ_ATTR(MAGICS, "0: 0x%x 1: 0x%x 2: 0x%x 3: 0x%x 4: 0x%x\n",
		roq_device->RoQ->mMagic0,
		roq_device->RoQ->mMagic1,
		roq_device->RoQ->mMagic2,
		roq_device->RoQ->mMagic3,
		roq_device->RoQ->mMagic4)
ROQ_ATTR(REMOTE_DIMENSIONS, "<%d,%d,%d,%d,%d>\n",
		roq_device->RoQ->mNetworkDesc_rem.Anodes,
		roq_device->RoQ->mNetworkDesc_rem.Bnodes,
		roq_device->RoQ->mNetworkDesc_rem.Cnodes,
		roq_device->RoQ->mNetworkDesc_rem.Dnodes,
		roq_device->RoQ->mNetworkDesc_rem.Enodes)
ROQ_ATTR(interrupt, "disable_cnt: %d\n",
		roq_device->RoQ->INTERRUPT_DISABLE_CNT)
ROQ_ATTR(tx_doorbell, "put: %llx got: %llx\n",
		roq_device->RoQ->mTxDoorbellQueueSet[0].mSRSW_QueueArray[0].mWriteSide.mPutCount,
		roq_device->RoQ->mTxDoorbellQueueSet[0].mSRSW_QueueArray[0].mReadSide.mGotCount)
ROQ_ATTR(cq_doorbell, "put: %llx got: %llx\n",
		roq_device->RoQ->mDD_DoorbellQueueSet.mSRSW_QueueArray[0].mWriteSide.mPutCount,
		roq_device->RoQ->mDD_DoorbellQueueSet.mSRSW_QueueArray[0].mReadSide.mGotCount)
ROQ_ATTR(obj_count, "qp: %d cq: %d pd: %d mem: %d srq: %d\n",
		roq_device->num_qp.counter, roq_device->num_cq.counter,
		roq_device->num_pd.counter, roq_device->num_mem.counter,
		roq_device->num_srq.counter)
ROQ_ATTR(nearest_io_rank, "%hu\n", roq_device->RoQ->nearest_io_rank)

static struct attribute *roq_device_attrs[] = {
	&dev_attr_MAGICS.attr,
	&dev_attr_interrupt.attr,
	&dev_attr_tx_doorbell.attr,
	&dev_attr_cq_doorbell.attr,
	&dev_attr_obj_count.attr,
	&dev_attr_REMOTE_DIMENSIONS.attr,
	&dev_attr_DISCOVERY.attr,
	&dev_attr_api_caps.attr,
	&dev_attr_tx_db_array.attr,
	&dev_attr_active_qps.attr,
	&dev_attr_cq_db_array.attr,
	&dev_attr_nearest_io_rank.attr,
	NULL
};

static const struct attribute_group roq_attr_group = {
	.attrs = roq_device_attrs,
};
/* SYSFS (END) */

int
roq_register_device(struct roq_dev *dev)
{
	int rc = 0;
	struct ib_device *ibdev = &dev->ofa_dev;
	ENTER;
	// Initialize the ib_dev structure.
	strlcpy(ibdev->name, "roq", IB_DEVICE_NAME_MAX);
	memset(&ibdev->node_guid, 0, sizeof(ibdev->node_guid));
	memcpy(&ibdev->node_guid, dev->l2dev->dev_addr,
	       min(sizeof(dev->l2dev->dev_addr), sizeof(ibdev->node_guid)));
	memcpy(ibdev->node_desc, ROQ_NODE_DESC, sizeof(ROQ_NODE_DESC));
	ibdev->owner = THIS_MODULE;
	ibdev->uverbs_cmd_mask =
		(1ull << IB_USER_VERBS_CMD_GET_CONTEXT) |
		(1ull << IB_USER_VERBS_CMD_QUERY_DEVICE) |
		(1ull << IB_USER_VERBS_CMD_QUERY_PORT) |
		(1ull << IB_USER_VERBS_CMD_ALLOC_PD) |
		(1ull << IB_USER_VERBS_CMD_DEALLOC_PD) |
		(1ull << IB_USER_VERBS_CMD_REG_MR) |
		(1ull << IB_USER_VERBS_CMD_DEREG_MR) |
		(1ull << IB_USER_VERBS_CMD_CREATE_COMP_CHANNEL) |
		(1ull << IB_USER_VERBS_CMD_CREATE_CQ) |
		(1ull << IB_USER_VERBS_CMD_POLL_CQ) |
		(1ull << IB_USER_VERBS_CMD_REQ_NOTIFY_CQ) |
		(1ull << IB_USER_VERBS_CMD_DESTROY_CQ) |
		(1ull << IB_USER_VERBS_CMD_CREATE_QP) |
		(1ull << IB_USER_VERBS_CMD_MODIFY_QP) |
		(1ull << IB_USER_VERBS_CMD_QUERY_QP) |
		(1ull << IB_USER_VERBS_CMD_DESTROY_QP) |
		(1ull << IB_USER_VERBS_CMD_POST_SEND) |
		(1ull << IB_USER_VERBS_CMD_POST_RECV) |
		(1ull << IB_USER_VERBS_CMD_CREATE_SRQ) |
		(1ull << IB_USER_VERBS_CMD_MODIFY_SRQ) |
		(1ull << IB_USER_VERBS_CMD_QUERY_SRQ) |
		(1ull << IB_USER_VERBS_CMD_DESTROY_SRQ) |
		(1ull << IB_USER_VERBS_CMD_POST_SRQ_RECV);

	ibdev->node_type = RDMA_NODE_RNIC;
	ibdev->phys_port_cnt = 1;
	ibdev->num_comp_vectors = 1;

	// Define the verb entry points.
	ibdev->query_device = roq_query_device;
	ibdev->query_port = roq_query_port;
	ibdev->modify_port = NULL;
	ibdev->query_pkey = roq_query_pkey;
	ibdev->query_gid = roq_query_gid;
	ibdev->alloc_ucontext = roq_alloc_ucontext;
	ibdev->dealloc_ucontext = roq_dealloc_ucontext;
	ibdev->mmap = roq_mmap;
	ibdev->alloc_pd = roq_alloc_pd;
	ibdev->dealloc_pd = roq_dealloc_pd;
	ibdev->create_ah = roq_create_ah;
	ibdev->destroy_ah = roq_destroy_ah;
	ibdev->create_qp = roq_create_qp;
	ibdev->modify_qp = roq_ofed_modify_qp;
	ibdev->query_qp = roq_query_qp;
	ibdev->destroy_qp = roq_destroy_qp;
	ibdev->create_cq = roq_create_cq;
	ibdev->destroy_cq = roq_destroy_cq;
	ibdev->resize_cq = NULL;
	ibdev->poll_cq = roq_poll_cq;
	ibdev->get_dma_mr = roq_get_dma_mr;
	ibdev->reg_phys_mr = NULL;
	ibdev->rereg_phys_mr = NULL;
	ibdev->reg_user_mr = roq_reg_user_mr;
	ibdev->dereg_mr = roq_dereg_mr;
	ibdev->alloc_mw = NULL;
	ibdev->bind_mw = NULL;
	ibdev->dealloc_mw = NULL;
	ibdev->create_srq = roq_create_srq;
	ibdev->modify_srq = roq_modify_srq;
	ibdev->query_srq = roq_query_srq;
	ibdev->destroy_srq = roq_destroy_srq;
	ibdev->post_srq_recv = roq_post_srq_recv;
	ibdev->attach_mcast = NULL;
	ibdev->detach_mcast = NULL;
	ibdev->process_mad = roq_no_mad;
	ibdev->req_notify_cq = roq_req_notify_cq;
	ibdev->post_send = roq_post_send;
	ibdev->post_recv = roq_post_receive;

	ibdev->dma_ops = &roq_dma_mapping_ops;

	// Allocate the iWarp connection manager.
	ibdev->iwcm = kmalloc(sizeof(struct iw_cm_verbs), GFP_KERNEL);
	if (!ibdev->iwcm)
		return ENOMEM;

	// Define the connection verb entry points.
	ibdev->iwcm->connect = roq_connect;
	ibdev->iwcm->accept = roq_accept;
	ibdev->iwcm->reject = roq_reject;
	ibdev->iwcm->create_listen = roq_create_listen;
	ibdev->iwcm->destroy_listen = roq_destroy_listen;
	ibdev->iwcm->add_ref = roq_qp_get_ref;
	ibdev->iwcm->rem_ref = roq_qp_put_ref;
	ibdev->iwcm->get_qp = roq_get_ofaqp;

	// Set device attributes.
	dev->attrs.version = ROQ_VERSION;
	dev->attrs.vendor_id = ROQ_VENDOR_ID;
	dev->attrs.vendor_part_id = ROQ_VENDORT_PART_ID;
	dev->attrs.sw_version = ROQ_SW_VERSION;
	dev->attrs.max_qp = ROQ_MAX_QP;
	dev->attrs.max_qp_wr = ROQ_MAX_QP_WR;
	dev->attrs.max_ord = ROQ_MAX_ORD;
	dev->attrs.max_ird = ROQ_MAX_IRD;
	dev->attrs.cap_flags = 0;
	dev->attrs.max_sge = ROQ_MAX_SGE;
	dev->attrs.max_sge_rd = ROQ_MAX_SGE_RD;
	dev->attrs.max_cq = ROQ_MAX_CQ;
	dev->attrs.max_cqe = ROQ_MAX_CQE;
	dev->attrs.max_mr = ROQ_MAX_MR;
	dev->attrs.max_mr_size = ROQ_MAX_MR_SIZE;
	dev->attrs.max_pd = ROQ_MAX_PD;
	dev->attrs.max_mw = ROQ_MAX_MW;
	dev->attrs.max_fmr = ROQ_MAX_FMR;
	dev->attrs.max_srq = ROQ_MAX_SRQ;
	dev->attrs.max_srq_wr = ROQ_MAX_SRQ_WR;
	dev->attrs.max_srq_sge = ROQ_MAX_SGE;
#ifdef DOORBELL_SYSCALL
	dev->attrs.api_caps = ROQ_SYSCALL_DB;
#else
	dev->attrs.api_caps = ROQ_OFA_CALL;
#endif
	if (mapped_qp)
		dev->attrs.api_caps |= ROQ_MAPPED_SQ|ROQ_MAPPED_RQ;
	if (mapped_cq)
		dev->attrs.api_caps |= ROQ_MAPPED_CQ;

	/*
	 * Register the IB device.
	 * Above verbs likely to be called before this returns.
	 */
	rc = ib_register_device(ibdev, NULL);
	if (rc) {
		kfree(ibdev->iwcm);
		return rc;
	}

	INIT_LIST_HEAD(&dev->qp_list);

	EXIT;
	return rc;

}

void roq_ctrl_slot_put(struct roq_dev *dev)
{
	atomic_dec(&dev->ctrl_reqs_pending);
	if (debug_cm)
		pr_info("DM Ctrl Queue - put slot at %d\n",
			atomic_read(&dev->ctrl_reqs_pending));
	wake_up(&dev->ctrl_waitq);
}

int roq_ctrl_slot_get(struct roq_dev *dev)
{
	int rv = 0;

retry:
	rv = wait_event_interruptible(dev->ctrl_waitq, dev->stopping ||
				      atomic_read(&dev->ctrl_reqs_pending)
				      < ROQ_CTRL_Q_SIZE);

	if (dev->stopping && !rv)
		rv = -EIO;

	if (rv)
		goto out;

	if (atomic_inc_return(&dev->ctrl_reqs_pending) <= ROQ_CTRL_Q_SIZE) {
		if (debug_cm)
			pr_info("DM Ctrl Queue - got slot at %d\n",
				atomic_read(&dev->ctrl_reqs_pending));
		goto out;
	}
	atomic_dec(&dev->ctrl_reqs_pending);
	if (debug_cm)
		pr_info("DM Ctrl Queue - retry slot at %d\n",
			atomic_read(&dev->ctrl_reqs_pending));

	goto retry;

out:
	if (debug_cm && rv)
		pr_info("roq_ctrl_slot_get failed: %d\n", rv);

	return rv;
}


uint64_t roq_propagate_db(struct roq_dev *rdev, dma_addr_t send_db,
			  dma_addr_t read_db, enum roq_db_client client)
{
	volatile struct RoQ_DeviceMemory *RoQ =  rdev->RoQ;
	volatile struct RoQ_CNTL_Command *CtrlCmd;
	uint64_t my_req = 0;
	uint64_t rv;

	if (roq_ctrl_slot_get(rdev) != 0) {
		rv = ROQ_INVALID_DB;
		goto out;
	}

	spin_lock(&rdev->command_lock);

	my_req = *(unsigned long volatile *)&RoQ->CNTL_RequestPosted;

	CtrlCmd = &RoQ->CNTL_Command[my_req % ROQ_CTRL_Q_SIZE];
	CtrlCmd->Type = RoQ_CNTL_Command_RegisterDB;
	CtrlCmd->Request.RegisterDB.SQ_Addr = send_db;
	CtrlCmd->Request.RegisterDB.RQ_Addr = read_db;
	CtrlCmd->Request.RegisterDB.db_type = client;

	set_mb(RoQ->CNTL_RequestPosted, my_req + 1);

	spin_unlock(&rdev->command_lock);

	rv = wait_event_interruptible(rdev->ctrl_waitq,
			rdev->stopping ||
			*(unsigned long volatile *)&RoQ->CNTL_RequestCompleted >
			my_req);

	if (rdev->stopping)
		rv = -EIO;

	if (!rv) {
		smp_mb();
		rv = CtrlCmd->Return.RegisterDB.db_id;
	} else {
		pr_info("roq_propagate_db: Failed: %d\n", (int)rv);
		rv = ROQ_INVALID_DB;
	}
	roq_ctrl_slot_put(rdev);

out:
	if (debug_cm)
		pr_info("roq_propagate_db: %d, DB %llx -- %llx (%llu)\n",
			client, send_db, rv, my_req);
	return rv;
}


void roq_revoke_db(struct roq_dev *rdev, uint64_t id,
		   enum roq_db_client client)
{
	struct RoQ_DeviceMemory *RoQ = rdev->RoQ;
	struct RoQ_CNTL_Command	*CtrlCmd;
	uint64_t my_req = 0;
	unsigned long flags;
	int rv = 0;

	rv = roq_ctrl_slot_get(rdev);
	if (rv) 
		goto out;

	spin_lock_irqsave(&rdev->command_lock, flags);

	my_req = *(unsigned long volatile *)&RoQ->CNTL_RequestPosted;

	CtrlCmd = &RoQ->CNTL_Command[my_req % ROQ_CTRL_Q_SIZE];
	CtrlCmd->Type = RoQ_CNTL_Command_RevokeDB;
	CtrlCmd->Request.RevokeDB.db_type = client;
	CtrlCmd->Request.RevokeDB.db_id = id;

	wmb();

	set_mb(RoQ->CNTL_RequestPosted, my_req + 1);

	spin_unlock_irqrestore(&rdev->command_lock, flags);

	rv = wait_event_interruptible(rdev->ctrl_waitq,
				      rdev->stopping ||
			*(unsigned long volatile *)&RoQ->CNTL_RequestCompleted >
			my_req);
	if (rdev->stopping)
		rv = -EIO;

	roq_ctrl_slot_put(rdev);
out:
	if (debug_cm && rv)
		pr_info("roq_revoke_db: %d, %llu, %llu\n",
			rv, RoQ->CNTL_RequestCompleted, my_req);

	return;
}

static int roq_connect_qp_db(void)
{
	dma_addr_t tx_db_addr, read_db_addr;
	int rv = 0;

	if (roq_qp_db) {
		pr_warn("roq_connect_qp_db called again");
		rv = -EINVAL;
		goto out;
	}
	atomic_set(&qp_db_idx, ROQ_SQ_DB_SIZE - 1);

	roq_qp_db = roq_kmalloc(ROQ_TX_DB_SIZE * sizeof(union roq_tx_db_req),
				&tx_db_addr);
	if (!roq_qp_db) {
		pr_warn("roq_connect_qp_db: malloc failed");
		rv = -ENOMEM;
		goto out;
	}
	memset((void *)roq_qp_db, 0,
	       ROQ_TX_DB_SIZE * sizeof(union roq_tx_db_req));

	/* put READ doorbell just behind SQ doorbell */
	read_db_addr =
		tx_db_addr + ROQ_SQ_DB_SIZE * sizeof(union roq_tx_db_req);

	tx_db_addr = roq_propagate_db(roq_device, tx_db_addr, read_db_addr,
				      ROQ_DB_QP_PRIV_CLIENT);

	if (tx_db_addr == ROQ_INVALID_DB) {
		roq_kfree((void *)roq_qp_db);
		roq_qp_db = NULL;
		rv = -EFAULT;
	}
out:
	return rv;
}

static int roq_connect_event_queue(void)
{
	dma_addr_t eventq_addr;
	int qsize = ROQ_EVENT_Q_SIZE * sizeof(struct roq_net_event),
	    rv = 0;

	if (roq_event_queue) {
		pr_warn("roq_event_queue already assigned\n");
		rv = -EINVAL;
		goto out;
	}
	roq_eventq_idx = 0;

	roq_event_queue = roq_kmalloc(qsize, &eventq_addr);

	if (!roq_event_queue) {
		pr_warn("roq_connect_event_db: malloc %d failed\n", qsize);
		rv = -ENOMEM;
		goto out;
	}
	memset((void *)roq_event_queue, 0, qsize);

	eventq_addr = roq_propagate_db(roq_device, eventq_addr, 0,
				       ROQ_DB_NET_EVENT);

	if (eventq_addr == ROQ_INVALID_DB) {
		roq_kfree((void *)roq_event_queue);
		roq_event_queue = NULL;
		rv = -EFAULT;
	}
out:
	return rv;
}

static void roq_force_cm_close(struct RoQ_DM_QP *dm_qp)
{
	struct iw_cm_event	event;
	struct roq_qp		*dd_qp = dm_qp->dd_qp;

	memset(&event, 0, sizeof(struct iw_cm_event));

	event.event = IW_CM_EVENT_DISCONNECT;
	event.status = 0;

	if (debug_cm) {
		pr_info("QP[%d]: ID: %p: DISCONNECT state %d, "
			"in_conn:active (%d:%d)\n",
			dd_qp ? roq_qpid(dd_qp) : -1,
			dm_qp->cm_id,
			dm_qp->qp_state,
			dd_qp ? dd_qp->is_connecting : -1,
			dd_qp ? dd_qp->active_connect : -1);
	}
	dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);

	event.event = IW_CM_EVENT_CLOSE;
	event.status = 0;

	if (debug_cm) {
		pr_info("QP[%d]: ID: %p: CLOSE state %d, "
			"in_conn:active (%d:%d)\n",
			dd_qp ? roq_qpid(dd_qp) : -1,
			dm_qp->cm_id,
			dm_qp->qp_state,
			dd_qp ? dd_qp->is_connecting : -1,
			dd_qp ? dd_qp->active_connect : -1);
	}
	dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);

	dm_qp->cm_id->rem_ref(dm_qp->cm_id);
	dm_qp->cm_id = NULL;
}

static void
roq_dm_disconnect(struct roq_dev *dev, uint32_t qp_id)
{
	struct RoQ_DM_QP	*dm_qp = roq_get_dm_qp(dev, qp_id);
	struct roq_qp		*dd_qp;

	if (!dm_qp) {
		pr_warn("ROQ DM Disconnect: Invalid QP ID %u\n", qp_id);
		goto out;
	}
	spin_lock_bh(&dm_qp->dd_lock);

	if (!dm_qp->cm_id) {
		if (debug_cm)
			pr_info("roq_dm_disconnect: QP[%d] empty\n", qp_id);
		goto out;
	}

	dd_qp = roq_get_dd_qp(dev, qp_id);

	if (dd_qp && dd_qp->is_connecting && dd_qp->active_connect) {
		struct iw_cm_event event;

		memset(&event, 0, sizeof event);

		event.event = IW_CM_EVENT_CONNECT_REPLY;
		event.status = -ECONNREFUSED;
		event.local_addr = dm_qp->cm_id->remote_addr;
		event.remote_addr = dm_qp->cm_id->local_addr;
		event.provider_data = NULL;
		event.private_data_len = 0;
		event.private_data = NULL;

		if (debug_cm) {
			pr_info("QP %04d: ID %p: CONNREFUSE "
				"upcall. state %d, in_conn:active ("
				"%d:%d) CMID %p\n",
				roq_qpid(dd_qp),
				dm_qp->cm_id,
				dm_qp->qp_state,
				dd_qp->is_connecting,
				dd_qp->active_connect,
				dm_qp->cm_id);
		}
		dd_qp->is_connecting = 0;
		dd_qp->active_connect = 0;

		dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);

		dm_qp->cm_id->rem_ref(dm_qp->cm_id);
		dm_qp->cm_id = NULL;
	} else
		roq_force_cm_close(dm_qp);

	if (dd_qp)
		roq_qp_put_ref(&dd_qp->ofa_qp);
out:
	spin_unlock_bh(&dm_qp->dd_lock);

	return;
}


static void roq_proc_net_event(struct roq_dev *dev)
{
	while (roq_event_queue) {
		struct roq_net_event *evp = &roq_event_queue[roq_eventq_idx];
		struct RoQ_DM_QP *dm_qp;
		struct roq_qp *dd_qp;

		if (evp->type == NET_EVENT_UNASSIGNED)
			break;

		switch (evp->type) {

		case NET_EVENT_HOSTUNREACHABLE:
			/*
			 * Bring down associated connections.
			 */
			spin_lock_bh(&dev->qp_lock);

			list_for_each_entry(dd_qp, &dev->qp_list, devq) {

				dm_qp = roq_get_dm_qp(dev, roq_qpid(dd_qp));
				if (!dm_qp) {
					pr_warn("ROQ net event: Invalid "
						"QP ID %u\n", roq_qpid(dd_qp));
					continue;
				}
				spin_lock_bh(&dm_qp->dd_lock);

				if (!dm_qp->cm_id) {
					spin_unlock_bh(&dm_qp->dd_lock);
					continue;
				}
				/*
				 * TODO: Handle all other states
				 */
				if (dm_qp->qp_state != ROQ_QPS_RTS) {
					spin_unlock_bh(&dm_qp->dd_lock);
					continue;
				}
				/*
				 * TODO2: For some reasons, DM addresses
				 *        missing some bits.
				 */
				evp->inaddr.sin_addr.s_addr |= 0x00400000;

				if (dm_qp->cm_id->remote_addr.sin_addr.s_addr
				    != evp->inaddr.sin_addr.s_addr) {
					spin_unlock_bh(&dm_qp->dd_lock);
					continue;
				}
				pr_info("Peer Host 0x%08x unreachable: "
					"affects QP[%d]\n",
					evp->inaddr.sin_addr.s_addr,
					roq_qpid(dd_qp));

				roq_force_cm_close(dm_qp);

				spin_unlock_bh(&dm_qp->dd_lock);
			}
			spin_unlock_bh(&dev->qp_lock);
			break;

		/*
		 * Application level errors. TODO: signal async event.
		 */
		case NET_EVENT_SQ_ERROR:
		case NET_EVENT_CQ_OVERFLOW:
		case NET_EVENT_RQ_EMPTY:
		case NET_EVENT_SRQ_EMPTY:
		case NET_EVENT_MEM_ACCESS_ERROR:
		case NET_EVENT_MEM_BASE_BOUNDS_ERROR:
		/*
		 * Provider level errors. TODO: signal async event.
		 */
		case NET_EVENT_PROTOCOL_ERROR:
			/* XXX: Console output for all provider errors. */
			pr_info("net_event: QP[%d], event %d\n",
				(int)evp->resource_id, evp->type);
		/*
		 * Peer disconnected.
		 */
		case NET_EVENT_QP_DISCONNECT:
			if (debug_cm && evp->type == NET_EVENT_QP_DISCONNECT)
				pr_info("net_event: QP[%d], event %d\n",
					(int)evp->resource_id, evp->type);

			roq_dm_disconnect(dev, evp->resource_id);

			break;

		default:
			pr_info("roq_proc_net_event: "
				"cannot handle event %d, value %llu\n",
				evp->type, evp->value);
		}
		evp->type = NET_EVENT_UNASSIGNED;

		roq_eventq_idx++;
		roq_eventq_idx %= ROQ_EVENT_Q_SIZE;
	}
}

extern void roq_eth_cleanup(struct net_device *);

void
roq_deregister_device(struct roq_dev *dev)
{
	struct ib_device *ibdev = &dev->ofa_dev;

//roq_eth_cleanup(dev->l2dev);
	ib_unregister_device(ibdev);
	if (ibdev->iwcm)
		kfree(ibdev->iwcm);

	return;
}

struct async_message_timer_state {
	struct timer_list tl;
	volatile struct RoQ_DeviceMemory *RoQ_DevMem;
	struct roq_dev *dd_dev;
};

/*
 * Interrupt handler to wakeup clients waiting for DM control
 * completion
 */
static void roq_process_ctrl_response(struct RoQ_DeviceMemory *RoQ)
{
	static uint64_t	processed = 0;
	int wakeup_ctrl = 0;

	while (*(unsigned long volatile *)&RoQ->CNTL_RequestCompleted >
	       processed) {

		int idx = processed % ROQ_CTRL_Q_SIZE;

		struct RoQ_CNTL_Command	*CtrlCmd = &RoQ->CNTL_Command[idx];
		struct RoQ_DM_QP	*dm_qp;
		struct roq_qp		*dd_qp;
		
		if (debug_cm)
			pr_info("roq_process_ctrl_response: event %d\n",
				CtrlCmd->Type);

		switch (CtrlCmd->Type) {

		case RoQ_CNTL_Command_ModifyQP:
			idx = CtrlCmd->Return.ModifyQP.QP_Num;

			BUG_ON(idx > RoQ_MAX_QP);

			dm_qp = &RoQ->QP_Array[idx];
			dd_qp = dm_qp->dd_qp;

			if (dd_qp) {
				wake_up(&dd_qp->ctrl_waitq);
				roq_qp_put_ref(&dd_qp->ofa_qp);
			}

			break;

		case RoQ_CNTL_Command_RegisterDB:
		case RoQ_CNTL_Command_RevokeDB:
		case RoQ_CNTL_Command_Discovery:

			break;

		default:
			pr_info("unknown response %d not processed\n",
				CtrlCmd->Type);
		}
		processed++;

		wakeup_ctrl = 1;
	}
	if (wakeup_ctrl)
		wake_up(&roq_device->ctrl_waitq);
}

static void
roq_proc_link_event(struct roq_dev *dev)
{
	if (dev->wq) {
		struct roq_wq_t *work = kmalloc(sizeof *work, GFP_KERNEL);
		if (unlikely(!work)) {
			pr_info("roq cm: disconnect not scheduled\n");
			return;
		}
		INIT_WORK((struct work_struct *)work, roq_wq_function);

		work->type = 1;
		roq_device->work = work;

		queue_work(roq_device->wq, (struct work_struct *)work);
	}
}

static void
bgproq_slih_handler(unsigned long data)
{
	struct RoQ_DeviceMemory *RoQ_DevMem = roq_device->RoQ;

	union SRSW_QueueSet_t	*db_q;
	enum roq_doorbell_type	DBType;
	void	*obj_id_p;

	RoQ_DevMem->interrupt_pending = 0;

	wmb();

next_event:
	db_q = (union SRSW_QueueSet_t *)&RoQ_DevMem->mDD_DoorbellQueueSet;
	
	if (SRSW_QueueSet_GetAggregateItemCount(db_q) <= 0)
		goto out;

	obj_id_p = SRSW_QueueSet_AccessNextReadItem(
		(union SRSW_QueueSet_t *)&RoQ_DevMem->mDD_DoorbellQueueSet);

	BUG_ON(!obj_id_p);

	DBType = *(u32 *)obj_id_p;

	SRSW_QueueSet_ReleaseAccessedReadItem((union SRSW_QueueSet_t *)
		      &RoQ_DevMem->mDD_DoorbellQueueSet);

	switch (DBType) {

	case RoQ_DB_CQ:
		roq_poll_cq_doorbell();
		break;

	case RoQ_DB_CTRL:
		roq_process_ctrl_response(RoQ_DevMem);
		break;

	case RoQ_DB_NET:
		roq_proc_net_event(roq_device);
		break;

	case RoQ_DB_LINK:
		roq_proc_link_event(roq_device);
		break;

	default:
		pr_info("roq: DB type %d unknown\n", DBType);
	}
	goto next_event;
out:
	return;
}

DECLARE_TASKLET(bgproq_slih, bgproq_slih_handler, 0);

void display_mu_registers(void)
  {
    pr_info( "%s 0x%016lx\n","MU_DCR(MOD_HALTED)",DCRReadPriv(MU_DCR(MOD_HALTED))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MCSRAM_BD_FETCHING_DESC)",DCRReadPriv(MU_DCR(MCSRAM_BD_FETCHING_DESC))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(IME_STATE)",DCRReadPriv(MU_DCR(IME_STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RME_STATE0)",DCRReadPriv(MU_DCR(RME_STATE0))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RME_STATE1)",DCRReadPriv(MU_DCR(RME_STATE1))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(INJ_NDFIFO_AVAIL)",DCRReadPriv(MU_DCR(INJ_NDFIFO_AVAIL))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MASTER_PORT0_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT0_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MASTER_PORT1_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT1_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MASTER_PORT2_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT2_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(SLAVE_PORT_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(SLAVE_PORT_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(XS_ERR_INFO)",DCRReadPriv(MU_DCR(XS_ERR_INFO))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MMREGS_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MMREGS_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MMREGS_FIFO_PROT_ERR)", DCRReadPriv(MU_DCR(MMREGS_FIFO_PROT_ERR))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MCSRAM_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(MCSRAM_ERR_FIFO_ID)",DCRReadPriv(MU_DCR(MCSRAM_ERR_FIFO_ID))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RCSRAM_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RMFIFO)", DCRReadPriv(MU_DCR(RMFIFO))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(RMFIFO_ACCESS_ERROR_ID))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(RPUTSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RPUTSRAM_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(ICSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(ICSRAM_INTERRUPTS__STATE))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(IMFIFO)", DCRReadPriv(MU_DCR(IMFIFO))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(IMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(IMFIFO_ACCESS_ERROR_ID))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(XM0_ERR_INFO)",DCRReadPriv(MU_DCR(XM0_ERR_INFO))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(XM1_ERR_INFO)",DCRReadPriv(MU_DCR(XM1_ERR_INFO))) ;
    pr_info( "%s 0x%016lx\n","MU_DCR(XM2_ERR_INFO)",DCRReadPriv(MU_DCR(XM2_ERR_INFO))) ;
    pr_info( "%s 0x%016lx\n","ND_RESE_DCR(10,RESET)",DCRReadPriv(ND_RESE_DCR(10,RESET))) ;
    pr_info( "%s 0x%016lx\n","ND_RESE_DCR(10,CTRL)",DCRReadPriv(ND_RESE_DCR(10,CTRL))) ;
}

irqreturn_t
bgproq_interrupt(int irq, void *dev)
{
#ifndef USERSPACE_EXTENSION
	uint64_t b = DCRReadPriv(MU_DCR(FIFO_INTERRUPTS__STATE));
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS__STATE), b);
#endif

	if( roq_device->RoQ->interrupt_pending ) {
	    tasklet_schedule(&bgproq_slih);
	} else if( DCRReadPriv(MU_DCR(XM0_ERR_INFO)) || DCRReadPriv(MU_DCR(XM1_ERR_INFO))
		|| DCRReadPriv(MU_DCR(XM2_ERR_INFO)) || DCRReadPriv(MU_DCR(XS_ERR_INFO))
		|| DCRReadPriv(MU_DCR(MMREGS_FIFO_PROT_ERR)) || DCRReadPriv(MU_DCR(RMFIFO))
		|| DCRReadPriv(MU_DCR(MCSRAM_ERR_FIFO_ID)) || DCRReadPriv(MU_DCR(IMFIFO))
		|| DCRReadPriv(MU_DCR(RMFIFO_ACCESS_ERROR_ID))
		|| DCRReadPriv(MU_DCR(IMFIFO_ACCESS_ERROR_ID))
		|| DCRReadPriv(MU_DCR(MASTER_PORT0_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(MASTER_PORT1_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(MASTER_PORT2_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(SLAVE_PORT_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(MMREGS_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(MCSRAM_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(RCSRAM_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(RPUTSRAM_INTERRUPTS__STATE))
		|| DCRReadPriv(MU_DCR(ICSRAM_INTERRUPTS__STATE)) )
	{
	    pr_emerg("%s: other interrupt from MU, probably indicating error; cannot handle that.\n", __func__);
	    display_mu_registers();
	    BUG();
	} else {
	    /* ignore, probably a pending interrupt from RoQ DM whose
	     * reason we already handled */
	}
	return IRQ_HANDLED;
}

static int roq_connect_cq_db(void)
{
	dma_addr_t cq_db_addr;
	int rv = 0;

	if (roq_cq_db) {
		pr_warn("roq_connect_cq_db called again\n");
		rv = -EINVAL;
		goto out;
	}
	atomic_set(&cq_db_idx, 0);

	roq_cq_db = roq_kmalloc(ROQ_CQ_DB_SIZE * sizeof(unsigned int),
				&cq_db_addr);
	if (!roq_cq_db) {
		pr_warn("roq_connect_cq_db: malloc failed");
		rv = -ENOMEM;
		goto out;
	}
	memset((void *)roq_cq_db, 0, ROQ_CQ_DB_SIZE * sizeof(unsigned int));

	cq_db_addr = roq_propagate_db(roq_device, cq_db_addr, 0,
				      ROQ_DB_CQ_CLIENT);

	if (cq_db_addr == ROQ_INVALID_DB)
		rv = -EFAULT;
out:
	return rv;
}

int __init roq_activate(struct roq_dev *rdev)
{
	struct net_device *netdev;
#ifndef USERSPACE_EXTENSION
	uint32_t *fw;
	int w, i, rc = 0;
#endif
	if (sizeof(spinlock_t) > DD_LOCK_SIZE) {
		pr_info("Resize DD_LOCK_SIZE to be at least %d\n",
			(int)sizeof(spinlock_t));
		return -EINVAL;
	}
	

	spin_lock_init(&rdev->mr_lock);
	spin_lock_init(&rdev->qp_lock);
	spin_lock_init(&rdev->command_lock);
	spin_lock_init(&rdev->listen_lock);

	init_waitqueue_head(&rdev->ctrl_waitq);
	atomic_set(&rdev->ctrl_reqs_pending, 0);

#ifndef USERSPACE_EXTENSION

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
	roq_fsm = ioremap_prot(where_the_roq_at & 0xFFFF000000ULL,
				0x0FFFFFFF, _PAGE_COHERENT | _PAGE_RW);
#else
	roq_fsm = ioremap_flags(where_the_roq_at & 0xFFFF000000ULL,
				0x0FFFFFFF, _PAGE_COHERENT | _PAGE_RW);
#endif
	fw = (uint32_t *)roq_fsm;

	printk(KERN_EMERG"SEARCH ROQ DEVICE (%p) ...\n", fw);
	for (w = 0; w < 0x0ffffff; w++) {
		if (fw[w] == RoQ_DeviceMemory_Magic0
		    && ((struct RoQ_DeviceMemory *)&fw[w])->mMagic3 ==
			(RoQ_DeviceMemory_Magic0 ^ RoQ_DeviceMemory_Magic1)
		    && ((struct RoQ_DeviceMemory *)&fw[w])->mMagic4 ==
			(sizeof(struct RoQ_DeviceMemory) ^
				RoQ_DeviceMemory_Magic1)) {
			rdev->RoQ = (struct RoQ_DeviceMemory *)&fw[w];
			printk(KERN_EMERG"ROQ MAGIC FOUND [%p]\n", &fw[w]);
			break;
		}
	}
	if (w >= 0x0ffffff) {
		printk("ROQ DEVICE NOT PRESENT *!* ABORT *!*\n");
		return -1;
	}
	dprint(DBG_ON, KERN_INFO,
	       "Valid magic value dev %p dev->RoQ %p dev->RoQ->mTestTime@ %p "
		"Offset %lld",
	       rdev, rdev->RoQ, &rdev->RoQ->mTestTime,
	       ((uint64_t) rdev->RoQ - where_the_roq_at));
#else

#ifdef LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
	rdev->RoQ = ioremap_prot(where_the_roq_at, sizeof(struct RoQ_DeviceMemory), _PAGE_COHERENT | _PAGE_RW);
#else
	rdev->RoQ = ioremap_flags(where_the_roq_at, sizeof(struct RoQ_DeviceMemory), _PAGE_COHERENT | _PAGE_RW);
#endif

#endif

	/* Verify the magic value. */
	if (rdev->RoQ->mMagic0 != RoQ_DeviceMemory_Magic0)
		dprint(DBG_ON, KERN_INFO,
		       "*!*!*!*!*!* BAD Magic0 *!*!*!*!*!* value (%08x != %08x)",
		       rdev->RoQ->mMagic0, RoQ_DeviceMemory_Magic0);

	if (rdev->RoQ->mMagic3 !=
	    (RoQ_DeviceMemory_Magic0 ^ RoQ_DeviceMemory_Magic1))
		dprint(DBG_ON, KERN_INFO,
		       "*!*!*!*!*!* BAD Magic3 *!*!*!*!*!* value (%08x != %08x)",
		       rdev->RoQ->mMagic3, RoQ_DeviceMemory_Magic0 ^ RoQ_DeviceMemory_Magic1);

	if (rdev->RoQ->mMagic4 !=
	    (sizeof(*(rdev->RoQ)) ^ RoQ_DeviceMemory_Magic1))
		dprint(DBG_ON, KERN_INFO,
		       "*!*!*!*!*!* BAD Magic4 *!*!*!*!*!* value (%08x != %08x)",
		       rdev->RoQ->mMagic4, RoQ_DeviceMemory_Magic0 | RoQ_DeviceMemory_Magic1);

	dprint(DBG_ON, KERN_INFO,
	       "Magic values 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X",
	       rdev->RoQ->mMagic0, rdev->RoQ->mMagic1,
	       rdev->RoQ->mMagic2, rdev->RoQ->mMagic3,
	       rdev->RoQ->mMagic4);

	dprint(DBG_ON, KERN_INFO, "Magic addresses 0x%p 0x%p 0x%p 0x%p 0x%p",
	       &rdev->RoQ->mMagic0, &rdev->RoQ->mMagic1,
	       &rdev->RoQ->mMagic2, &rdev->RoQ->mMagic3,
		   &rdev->RoQ->mMagic4);

	dprint(DBG_ON, KERN_INFO,
	       "Magic offset 0x%016llX 0x%016llX 0x%016llX 0x%016llX 0x%016llX",
	       ((uint64_t) (&rdev->RoQ->mMagic0)) - ((uint64_t) (rdev->RoQ)),
	       ((uint64_t) (&rdev->RoQ->mMagic1)) - ((uint64_t) (rdev->RoQ)),
	       ((uint64_t) (&rdev->RoQ->mMagic2)) - ((uint64_t) (rdev->RoQ)),
	       ((uint64_t) (&rdev->RoQ->mMagic3)) - ((uint64_t) (rdev->RoQ)),
	       ((uint64_t) (&rdev->RoQ->mMagic4)) - ((uint64_t) (rdev->RoQ)));

	/*
	 * Ensure that the RoQ interface version matches the one we
	 * built against.
	 */
	BUG_ON(rdev->RoQ->mVersion != ROQ_FW_VERSION);

	/*
	 * Let the DM know the address offset between DD and DM
	 */
	*(uint64_t *)&rdev->RoQ->mDD_DevMem_Address = (uint64_t)rdev->RoQ;

	/* use mTestTime to store a pointer device driver main structure */
	out_be64(&rdev->RoQ->mTestTime, (uint64_t) rdev);

	wmb();

	/* bring up the ethernet device */
	rc = roq_eth_init_module(&netdev,
				 (struct RoQ_DeviceMemory *)rdev->RoQ);
	if (rc) {
		printk(KERN_EMERG "ROQ: cannot init ethernet.\n");
		goto out;
	}
	/* Use the eth-over-IP net device */
	if (netdev) {
		netdev->dev.archdata = rdev->ofa_dev.dma_device->archdata;

		rtnl_lock();
		rdev->l2dev = netdev;

		/* Register an IB device. */
		rc = roq_register_device(rdev);
		if (rc)
			ib_dealloc_device(&rdev->ofa_dev);

		rtnl_unlock();
		if (rc)
			goto out;
	}
	for (i = 0; i < ROQ_MAX_QP; i++)
		spin_lock_init(&rdev->RoQ->QP_Array[i].dd_lock);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0) || LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
	rc = request_irq(21, bgproq_interrupt, IRQF_DISABLED | IRQF_PERCPU,
			 "BGAS CTRL", roq_device);
#else
	rc = request_irq(BGQ_IRQ_MU6, bgproq_interrupt,
		IRQF_DISABLED | IRQF_PERCPU, "BGAS CTRL", roq_device);
#endif
	if (rc) {
	    printk(KERN_EMERG "ROQ: cannot install interrupt handler.\n");
	    goto out;
	}

	rc = roq_connect_qp_db();
	if (rc)
		goto out_sysfs;

	atomic_set(&num_mapped_db, 0);

	rc = roq_connect_event_queue();
	if (rc)
		goto out_sysfs;

	rc = roq_connect_cq_db();
	if (rc)
		goto out_sysfs;

	/* Add the device attributes */
	rc = sysfs_create_group(&rdev->ofa_dev.dev.kobj, &roq_attr_group);
	if (rc) {
		printk("Error creating sysfs files (%d)\n", rc);
		goto out_sysfs;
	}

	roq_active = 1;
	return rc;

out_sysfs:
	sysfs_remove_group(&rdev->ofa_dev.dev.kobj, &roq_attr_group);
out:
	return rc;
}

int __exit roq_deactivate(struct roq_dev *rdev)
{
	if (roq_active == 1) {
		roq_active = 0;

		/* Disable SW interrupt before removing device & handler */
		free_irq(21, rdev);

		roq_deregister_device(rdev);

		if (roq_fsm) {
			iounmap((uint64_t *) (roq_fsm));
			roq_fsm = NULL;
		} else
			iounmap(rdev->RoQ);

		roq_eth_exit_module(rdev->l2dev);
		ib_dealloc_device(&rdev->ofa_dev);
	}
	return 0;
}

#if defined(__FUSEDOS__) && LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0) && LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#include "get_personality_of.h"
int bluegene_getPersonality(Personality_t* personality, size_t size)
{
    BUILD_BUG_ON(sizeof(Personality_t) != sizeof(struct bgq_personality));

    struct bgq_personality * pers_ptr = get_bgpersonality();
    if(pers_ptr == NULL)
	return -EBUSY;
    memcpy(personality, pers_ptr, min(size, sizeof(struct bgq_personality)));
    return 0;
}
#endif

static int __init roq_probe(void)
{
	struct roq_dev *rdev;
#ifndef USERSPACE_EXTENSION
	Personality_t pers;
	cpumask_t cpus_allowed;
	int rc;
#endif
	pr_info("roq_probe called\n");

	/* ROQ already allocated */
	if (roq_device) {
		pr_info("roq_probe: device already allocated\n");
		return -EBUSY;
	}

	rdev = (struct roq_dev *)ib_alloc_device(sizeof(*rdev));
	if (!rdev) {
		printk(KERN_EMERG "ib_alloc_device() failed\n");
		return -ENOMEM;
	}
	rdev->ofa_dev.dma_device = &roq_generic_device;

	/*
	 * If you lose this, look at a boot of the RoQ and see the
	 * address there
	 */
#ifdef USERSPACE_EXTENSION
	where_the_roq_at = roq_usmu_module_init();
	rdev->RoQ = (struct RoQ_DeviceMemory *)where_the_roq_at;
#else
	bluegene_getPersonality(&pers, sizeof(Personality_t));

	if ((pers.DDR_Config.DDRSizeMB & 0xf) != 0) {
		where_the_roq_at = (uint64_t)ExpectedLocationOf_RoQ_DevMem +
			(((uint64_t)pers.DDR_Config.DDRSizeMB & 0xfffffff0)
			| (((uint64_t)pers.DDR_Config.DDRSizeMB & 0xf) << 32));
	} else {
		where_the_roq_at = (uint64_t)ExpectedLocationOf_RoQ_DevMem +
			(((uint64_t)pers.DDR_Config.DDRSizeMB * 1024ULL * 1024ULL) - 0x40000000);
	}
#endif

	/* save device point for clean-up */
	roq_device = rdev;

	roq_device->wq = create_singlethread_workqueue("ROQ_WQ");

#ifndef USERSPACE_EXTENSION
	// DCRWritePriv(MU_DCR(RMFIFO_PACKET_INT_EN), 0x8000);
	cpus_allowed = current->cpus_allowed;
	set_cpus_allowed_ptr(current, cpumask_of(0));

	rc = roq_activate(rdev);

	set_cpus_allowed_ptr(current, &cpus_allowed);
#endif

	return 0;
}

static int __exit roq_remove(void)
{
	roq_deactivate(roq_device);
#ifdef USERSPACE_EXTENSION
	if (roq_device != NULL)
		roq_usmu_module_exit();
#endif
	roq_device = NULL;
	return 0;
}

static int roq_netdev_event(struct notifier_block *nb, unsigned long event,
			    void *arg)
{
	struct net_device *netdev = arg;
	int rv = NOTIFY_OK;

	if (strncmp(netdev->name, "roq", 3)) {
		rv = NOTIFY_DONE;
		goto done;
	}
	if (!spin_trylock(&roq_dev_lock)) {
		rv = NOTIFY_DONE;
                goto done;
	}
	switch (event) {

	case NETDEV_UP:
		pr_info("NETDEV_UP\n");
		break;
	case NETDEV_DOWN:
		pr_info("NETDEV_DOWN\n");
		break;
	case NETDEV_REGISTER:
		pr_info("NETDEV_REGISTER\n");
		break;
	case NETDEV_UNREGISTER:
		pr_info("NETDEV_UNREGISTER\n");
		break;
	case NETDEV_CHANGEADDR:
		pr_info("NETDEV_CHANGEADDR\n");
		break;
	case NETDEV_CHANGEMTU:
		pr_info("NETDEV_CHANGEMTU\n");
		break;
	case NETDEV_GOING_DOWN:
		pr_info("NETDEV_GOING_DOWN\n");
		break;
	case NETDEV_CHANGE:
		pr_info("NETDEV_CHANGE\n");
		break;
	case NETDEV_PRE_UP:
		pr_info("NETDEV_PRE_UP\n");
		break;
	default:
		pr_info("unexpected netdev event %lu\n", event);
		rv = NOTIFY_DONE;
		break;
	}
	spin_unlock(&roq_dev_lock);
done:
	return rv;
}

static struct notifier_block roq_netdev_nb = {
	.notifier_call = roq_netdev_event,
};

static __init int
roq_init_module(void)
{
	int rv = 0;

	rv =  device_register(&roq_generic_device);
	if (rv)
		goto out;

	rv = roq_probe();
	if (rv) {
		device_unregister(&roq_generic_device);
		goto out;
	}
	rv = register_netdevice_notifier(&roq_netdev_nb);
	if (rv) {
		pr_info("register_netdevice_notifier failed with %d\n", rv);
		goto out;
	}

#ifdef DOORBELL_SYSCALL
	pr_info("re-register doorbell syscall %p from %p\n",
		roq_doorbell, db_orig_call);

	db_orig_call = doorbell_call;
	doorbell_call = roq_doorbell;
#endif

out:
	if (rv)
		pr_warn("roq_init_module failed with %d\n", rv);
	return rv;
}

static void __exit
roq_exit_module(void)
{
	spin_lock(&roq_dev_lock);
	unregister_netdevice_notifier(&roq_netdev_nb);
	spin_unlock(&roq_dev_lock);

#ifdef DOORBELL_SYSCALL
	doorbell_call = db_orig_call;
#endif
	roq_remove();
	device_unregister(&roq_generic_device);

	return;
}

module_init(roq_init_module);
module_exit(roq_exit_module);
