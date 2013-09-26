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

#include <linux/in.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/netdevice.h>
#include <linux/interrupt.h>
#include <linux/moduleparam.h>

#include <linux/vmalloc.h>
#include <asm/pgtable.h>


#include "roq.h"
#include "roq_cm.h"
#include "roq_utils.h"
#include "roq_debug.h"

#include <asm/io.h>

#define CAN_TURN_TRACE_ON
#define DMA_MR_KEY 0
#include "roq_linux_trace.h"

static TraceClient TraceOnLinuxStart __attribute__((unused)) ;
static TraceClient TraceOnLinuxFinis __attribute__((unused)) ;
static TraceClient gTraceClientSync __attribute__((unused)) ;
static TraceClient TraceCountLinuxStart __attribute__((unused)) ;
static TraceClient TraceCountLinuxFinis __attribute__((unused)) ;
static TraceClient TraceCountLinux2Start __attribute__((unused)) ;
static TraceClient TraceCountLinux2Finis __attribute__((unused)) ;
static TraceClient TraceCountLinux3Start __attribute__((unused)) ;
static TraceClient TraceCountLinux3Finis __attribute__((unused)) ;


#ifdef DOORBELL_SYSCALL
extern long (*db_orig_call) (u32, u32, u32);
#endif
extern struct roq_dev *roq_device;
extern unsigned int roq_active;

bool mapped_db = 1;
module_param(mapped_db, bool, 0644);
MODULE_PARM_DESC(mapped_db, "Allow limited SQ DB mapping to user space");

bool mapped_qp = 1;
module_param(mapped_qp, bool, 0644);
MODULE_PARM_DESC(mapped_qp, "Allow SQ/RQ mapping to user space");

bool mapped_cq = 1;
module_param(mapped_cq, bool, 0644);
MODULE_PARM_DESC(mapped_cq, "Map CQ to user space");

bool debug_cm = 0;
module_param(debug_cm, bool, 0644);
MODULE_PARM_DESC(debug_cm, "Debug RoC CM");


static inline struct RoQ_DeviceMemory *roq_dev2dm_ctrl(struct roq_dev *dev)
{
	return dev->RoQ;
}

static u32
roq_insert_uobj(struct roq_ucontext *uctx, void *vaddr, u32 size)
{
	struct roq_uobj *uobj;
	u32		key;

	uobj = kzalloc(sizeof *uobj, GFP_KERNEL);
	if (!uobj)
		return ROQ_MAX_UOBJ_KEY + 1;

	size = PAGE_ALIGN(size);

	spin_lock(&uctx->uobj_lock);
	
	if (list_empty(&uctx->uobj_list))
		uctx->key = 0;

	key = uctx->key;

	uobj->key = uctx->key;
	uctx->key += size; /* advance for next object */

	if (key > ROQ_MAX_UOBJ_KEY) {
		kfree (uobj);
		goto out;
	}
	uobj->size = size;
	uobj->addr = virt_to_phys(vaddr);
	
	list_add_tail(&uobj->list, &uctx->uobj_list);

	spin_unlock(&uctx->uobj_lock);
out:
	return key;
}

static struct roq_uobj *
roq_remove_uobj(struct roq_ucontext *uctx, u32 key, u32 size)
{
	struct list_head *pos, *nxt;

	spin_lock(&uctx->uobj_lock);

	list_for_each_safe(pos, nxt, &uctx->uobj_list) {
		struct roq_uobj *uobj = list_entry(pos, struct roq_uobj, list);
		if (uobj->key == key && uobj->size == size) {
			list_del(&uobj->list);
			spin_unlock(&uctx->uobj_lock);
			return uobj;
		}
	}
	spin_unlock(&uctx->uobj_lock);

	return NULL;
}

int
roq_mmap(struct ib_ucontext *ctx, struct vm_area_struct *vma)
{

	struct roq_ucontext	*uctx = roq_ctx_ofa2roq(ctx);
	struct roq_uobj		*uobj;
	u32	key = vma->vm_pgoff << PAGE_SHIFT;
	int	size = vma->vm_end - vma->vm_start;
	u64	addr;

	int	rv = -EINVAL;

	/*
	 * Must be page aligned
	 */
	if (vma->vm_start & (PAGE_SIZE - 1)) {
		pr_warn("map not page aligned\n");
		goto out;
	}

	uobj = roq_remove_uobj(uctx, key, size);
	if (!uobj) {
		pr_warn("mmap lookup failed: %u, %d\n", key, size);
		goto out;
	}
	addr = uobj->addr;
	kfree(uobj);

	vma->vm_flags |= VM_IO | VM_RESERVED;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	rv = remap_pfn_range(vma, vma->vm_start, addr >> PAGE_SHIFT,
			     size, vma->vm_page_prot);
out:
	return rv;
}

atomic_t num_mapped_db;

struct RoQ_DM_QP *
roq_get_dm_qp(struct roq_dev *rdev, uint32_t id)
{
	if (likely(id != 0 && id < (uint32_t)ROQ_MAX_QP))
		return &rdev->RoQ->QP_Array[id];

	return NULL;
}

/*
 * If found, returns DD QP with incremented reference count.
 * Requires DM_QP to be locked.
 */
struct roq_qp *
roq_get_dd_qp(struct roq_dev *rdev, uint32_t id)
{
	struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(rdev, id);

	if (dm_qp) {
		struct roq_qp *dd_qp = dm_qp->dd_qp;

		if (dd_qp && roq_qpid(dd_qp) == id) {
			roq_qp_get_ref(&dd_qp->ofa_qp);
			return dd_qp;
		} else
			WARN_ON(dd_qp && roq_qpid(dd_qp) != id);
	}
	return NULL;
}


struct ib_ucontext *
roq_alloc_ucontext(struct ib_device *ofa_dev, struct ib_udata *udata)
{
	struct roq_ucontext *uctx;
	struct roq_ureq_alloc_ctx ureq;
	struct roq_uresp_alloc_ctx uresp;
	int rv = 0;

	dprint_tsk(DBG_CM, KERN_INFO, "(device=%s)", ofa_dev->name);

	uctx = (struct roq_ucontext *)kzalloc(sizeof(*uctx), GFP_KERNEL);
	if (!uctx) {
		rv = -ENOMEM;
		goto err_out;
	}

	INIT_LIST_HEAD(&uctx->uobj_list);
	spin_lock_init(&uctx->uobj_lock);
	uctx->db_id = ROQ_INVALID_DB;

	memset(&uresp, 0, sizeof uresp);

	if (!udata)
		goto skip_db_mapping;

	if (mapped_db == 0)
		goto skip_db_mapping;

	rv = ib_copy_from_udata(&ureq, udata, sizeof ureq);
	if (rv)
		goto err_out;

#ifdef DOORBELL_SYSCALL
	/*
	 * Notify user about eventual DB system call
	 */
	uresp.db_syscall = __NR_rdma_db;
#else
	uresp.db_syscall = -1;
#endif

	if (!(ureq.flags & QUEUE_FLAG_MAPPEABLE))
		goto skip_db_mapping;

	if (atomic_inc_return(&num_mapped_db) > ROQ_MAX_MAPPED_DB) {
		static int warned = 0;
		if (!warned && debug_cm) {
			pr_info("roq_alloc_ucontext: "
				"Exceeded maximum mapped DB's: %d\n",
				ROQ_MAX_MAPPED_DB);
			warned = 1;
		}
		atomic_dec(&num_mapped_db);
		goto skip_db_mapping;
	} else {
		uctx->sq_db_size =
			ROQ_DB_ARRAY_SIZE * sizeof(union roq_tx_db_req);
		uctx->sq_db_virt = dma_alloc_coherent(ofa_dev->dma_device,
						      uctx->sq_db_size,
						      &uctx->sq_db_phys,
						      GFP_KERNEL);
		if (uctx->sq_db_virt) {
			uint64_t db_id;
			u32 key = roq_insert_uobj(uctx,
						  uctx->sq_db_virt,
						  uctx->sq_db_size);

			if (key <= ROQ_MAX_UOBJ_KEY) {
				memset(uctx->sq_db_virt, 0, uctx->sq_db_size);
				
				db_id = roq_propagate_db(roq_device,
							 uctx->sq_db_phys,
							 0,
							 ROQ_DB_QP_USER_CLIENT);

				if (db_id != ROQ_INVALID_DB) {
					uctx->db_id = db_id;
					uresp.flags = QUEUE_FLAG_MAPPEABLE;
					uresp.db_key = key;
					uresp.db_size = ROQ_DB_ARRAY_SIZE;
				} else {
					pr_info("No user mapped DB's\n");
					atomic_dec(&num_mapped_db);
				}
			} else {
				rv = -ENOMEM;
				atomic_dec(&num_mapped_db);
				goto err_out;
			}
		} else {
			rv = -ENOMEM;
			atomic_dec(&num_mapped_db);
			goto err_out;
		}
	}
skip_db_mapping:
	if (!rv && udata) {
		rv = ib_copy_to_udata(udata, &uresp, sizeof uresp);
		if (rv) 
			goto err_out;
	}
	return &uctx->ofa_ucontext;

err_out:
	pr_warn("alloc_ctx: failed: %d\n", rv);
	if (uctx->db_id != ROQ_INVALID_DB) {
		atomic_dec(&num_mapped_db);
		roq_revoke_db(roq_device, uctx->db_id, ROQ_DB_QP_USER_CLIENT);
	}
	if (uctx->sq_db_virt)
		dma_free_coherent(ofa_dev->dma_device, uctx->sq_db_size,
				  uctx->sq_db_virt, uctx->sq_db_phys);
	kfree(uctx);
	return (struct ib_ucontext *)ERR_PTR(rv);
}

int
roq_dealloc_ucontext(struct ib_ucontext *ctx)
{
	struct roq_ucontext *uctx = roq_ctx_ofa2roq(ctx);
	struct list_head *pos, *nxt;

	list_for_each_safe(pos, nxt, &uctx->uobj_list) {
		struct roq_uobj *uobj = list_entry(pos, struct roq_uobj, list);
		list_del(&uobj->list);
		pr_info("dealloc_uctx -- free unused map: key %u\n", uobj->key);
		kfree(uobj);
	}
	if (uctx->sq_db_virt && uctx->db_id != ROQ_INVALID_DB) {
		roq_revoke_db(roq_device, uctx->db_id, ROQ_DB_QP_USER_CLIENT);

		dma_free_coherent(ctx->device->dma_device,
				  ROQ_DB_ARRAY_SIZE * sizeof *uctx->sq_db_virt,
				  uctx->sq_db_virt,
				  uctx->sq_db_phys);

		atomic_dec(&num_mapped_db);
	}
	kfree(uctx);

	return 0;
}

int
roq_query_device(struct ib_device *ofa_dev, struct ib_device_attr *attr)
{
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_dev);


	memset(attr, 0, sizeof(*attr));

	attr->max_mr_size = dev->attrs.max_mr_size;
	attr->vendor_id = dev->attrs.vendor_id;
	attr->vendor_part_id = dev->attrs.vendor_part_id;
	attr->max_qp = dev->attrs.max_qp;
	attr->max_qp_wr = dev->attrs.max_qp_wr;
	attr->max_qp_rd_atom = dev->attrs.max_ord;
	attr->max_qp_init_rd_atom = dev->attrs.max_ird;
	attr->device_cap_flags = dev->attrs.cap_flags;
	attr->max_sge = dev->attrs.max_sge;
	attr->max_sge_rd = dev->attrs.max_sge_rd;
	attr->max_cq = dev->attrs.max_cq;
	attr->max_cqe = dev->attrs.max_cqe;
	attr->max_mr = dev->attrs.max_mr;
	attr->max_pd = dev->attrs.max_pd;
	attr->max_mw = dev->attrs.max_mw;
	attr->max_fmr = dev->attrs.max_fmr;
	attr->max_srq = dev->attrs.max_srq;
	attr->max_srq_wr = dev->attrs.max_srq_wr;
	attr->max_srq_sge = dev->attrs.max_srq_sge;
	memcpy(&attr->sys_image_guid, dev->l2dev->dev_addr,
	       min(sizeof(attr->sys_image_guid), sizeof(dev->l2dev->dev_addr)));

	return 0;

	/*
	   TODO: understand what of the following should
	   get useful information

	   attr->fw_ver;
	   attr->max_ah
	   attr->max_map_per_fmr
	   attr->max_ee
	   attr->max_rdd
	   attr->max_qp_rd_atom;
	   attr->max_ee_rd_atom;
	   attr->max_res_rd_atom;
	   attr->max_qp_init_rd_atom;
	   attr->max_ee_init_rd_atom;
	   attr->max_raw_ipv6_qp
	   attr->max_raw_ethy_qp
	   attr->max_mcast_grp
	   attr->max_mcast_qp_attach
	   attr->max_total_mcast_qp_attach
	   attr->max_pkeys
	   attr->atomic_cap;
	   attr->page_size_cap;
	   attr->hw_ver;
	   attr->local_ca_ack_delay;
	 */

}

/*
 * approximate translation of real MTU for IB
 * TODO: is that needed for RNIC's?
 */
static inline enum ib_mtu
roq_mtu_net2ofa(unsigned short mtu)
{
	if (mtu >= 4096)
		return IB_MTU_4096;
	if (mtu >= 2048)
		return IB_MTU_2048;
	if (mtu >= 1024)
		return IB_MTU_1024;
	if (mtu >= 512)
		return IB_MTU_512;
	if (mtu >= 256)
		return IB_MTU_256;
	return (enum ib_mtu)-1;
}

int
roq_query_port(struct ib_device *ofa_dev, u8 port,
		   struct ib_port_attr *attr)
{
	memset(attr, 0, sizeof(*attr));
	attr->state = IB_PORT_ACTIVE;
	attr->max_mtu = attr->active_mtu = IB_MTU_4096;
	attr->gid_tbl_len = 1;
	attr->port_cap_flags = IB_PORT_CM_SUP | IB_PORT_DEVICE_MGMT_SUP;
	attr->max_msg_sz = -1;
	attr->pkey_tbl_len = 1;
	attr->active_width = IB_WIDTH_4X;
	attr->active_speed = 2;

	return 0;
}

int
roq_query_pkey(struct ib_device *ofa_dev, u8 port, u16 idx, u16 * pkey)
{
	*pkey = 0;

	return 0;
}

int
roq_query_gid(struct ib_device *ofa_dev, u8 port, int idx,
		  union ib_gid *gid)
{
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_dev);

	memset(gid, 0, sizeof(*gid));
	memcpy(&gid->raw[0], dev->l2dev->dev_addr,
	       min(sizeof(dev->l2dev->dev_addr), sizeof(gid->raw)));

	return 0;
}

struct ib_pd *
roq_alloc_pd(struct ib_device *ofa_dev,
		 struct ib_ucontext *context, struct ib_udata *udata)
{
	struct ib_pd *pd;
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_dev);

	if (atomic_read(&dev->num_pd) >= ROQ_MAX_PD) {
		dprint(DBG_ON, KERN_INFO, "Out of PD's");
		return (struct ib_pd *)ERR_PTR(-ENOMEM);
	}
	pd = (struct ib_pd *)kmalloc(sizeof *pd, GFP_KERNEL);
	if (!pd) {
		dprint(DBG_ON, KERN_INFO, " malloc");
		return (struct ib_pd *)ERR_PTR(-ENOMEM);
	}
	if (context) {
		/* just use the point as pd number */
		if (ib_copy_to_udata(udata, pd, sizeof *pd)) {
			kfree(pd);
			return (struct ib_pd *)ERR_PTR(-EFAULT);
		}
	}
	atomic_inc(&dev->num_pd);

	/* need to do it for non-ofa invocations */
	pd->device = (struct ib_device *) dev;
	dprint(DBG_ON, KERN_INFO,
	       "created pd @ 0x%p on dev @ 0x%p pd->device 0x%p",
	       pd, ofa_dev, pd->device);

	return pd;
}

int
roq_dealloc_pd(struct ib_pd *ofa_pd)
{
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_pd->device);

	kfree(ofa_pd);

	atomic_dec(&dev->num_pd);

	return 0;
}

struct ib_ah *
roq_create_ah(struct ib_pd *pd, struct ib_ah_attr *attr)
{
	return (struct ib_ah *) ERR_PTR(-ENOSYS);
}

int
roq_destroy_ah(struct ib_ah *ah)
{
	return -ENOSYS;
}

int
roq_no_mad(struct ib_device *ofa_dev, int flags, u8 port,
	       struct ib_wc *wc, struct ib_grh *grh,
	       struct ib_mad *in_mad, struct ib_mad *out_mad)
{
	printk(KERN_EMERG "%s in %s not implemented\n", __func__, __FILE__);
	return -ENOSYS;
}


struct ib_qp *
roq_create_qp(struct ib_pd *ofa_pd, struct ib_qp_init_attr *init_attrs,
	      struct ib_udata *udata)
{
	struct roq_qp		*dd_qp = NULL;
	struct RoQ_DM_QP	*dm_qp = NULL;
	struct ib_device	*ofa_dev = ofa_pd->device;
	struct roq_dev		*dev = roq_dev_ofa2roq(ofa_dev);
	struct roq_cq		*scq = NULL;
	struct roq_cq		*rcq = NULL;
	u32		sq_alloc_size = 0;
	u32		rq_alloc_size = 0;
	static int	free_qp = 0;
	dma_addr_t	SendQueueDmaAddr;
	dma_addr_t	RecvQueueDmaAddr;
	int	i,
		sq_is_mapped = 0,
		rq_is_mapped = 0,
		rv = 0;

	dprint(DBG_OBJ | DBG_CM, KERN_INFO,
		": new QP on device %s dev@ 0x%p pd@ 0x%p",
		ofa_dev->name, dev, ofa_pd);

	if (init_attrs->qp_type != IB_QPT_RC &&
	    init_attrs->qp_type != IB_QPT_UD) {
		dprint(DBG_ON, KERN_INFO, "Only RC/UD QP's supported");
		return (struct ib_qp *)ERR_PTR(-EINVAL);
	}

	if ((init_attrs->cap.max_send_wr > ROQ_MAX_QP_WR) ||
	    (init_attrs->cap.max_recv_wr > ROQ_MAX_QP_WR) ||
	    (init_attrs->cap.max_send_sge > ROQ_MAX_SGE) ||
	    (init_attrs->cap.max_recv_sge > ROQ_MAX_SGE)) {
		dprint(DBG_ON|DBG_CM, KERN_INFO, "QP Size failure: "
		       "SQE=%u:%u, RQE=%u:%u, SSGE=%u:%u, RSGE=%u:%u\n",
			init_attrs->cap.max_send_wr, ROQ_MAX_QP_WR,
			init_attrs->cap.max_recv_wr, ROQ_MAX_QP_WR,
			init_attrs->cap.max_send_sge, ROQ_MAX_SGE,
			init_attrs->cap.max_recv_sge, ROQ_MAX_SGE);
		return (struct ib_qp*)ERR_PTR(-EINVAL);
	}

	/*
	 * NOTE: we allow for zero element SQ and RQ WQE's SGL's
	 * but not for a QP unable to hold any WQE (SQ + RQ)
	 */
	if (init_attrs->cap.max_send_wr + init_attrs->cap.max_recv_wr == 0)
		return (struct ib_qp *)ERR_PTR(-EINVAL);

	if (!init_attrs->send_cq || !init_attrs->recv_cq) {
		dprint(DBG_CM, KERN_INFO, "Fail: init_attrs->send_cq: 0x%p, "
			"init_attrs->recv_cq: 0x%p",
			init_attrs->send_cq, init_attrs->recv_cq);
		return (struct ib_qp *)ERR_PTR(-EINVAL);
	}

	dd_qp = (struct roq_qp *)kmalloc(sizeof *dd_qp, GFP_KERNEL);
	if (!dd_qp) {
		dprint(DBG_CM|DBG_ON, KERN_INFO, "Fail: alloc of roq_qp");
		return (struct ib_qp *)ERR_PTR(-EINVAL);
	}
	atomic_inc(&dev->num_qp);

	memset(dd_qp, 0, sizeof *dd_qp);

	/* we need just about all the fields contained
	 * in init_attrs, so well just copy that struct
	 */
	dd_qp->init_attrs = *init_attrs;
	/* XXX tentative - dapltest does not set value correctly */
	dd_qp->init_attrs.cap.max_send_sge = ROQ_MAX_SGE;

	/*
	 * Grab a QP slot in the vI.
	 *
	 * Try to use one not used for a while
	 * (to be changed to random number!)
	 */
	spin_lock_bh(&dev->qp_lock);
	for (i = 1; i < ROQ_MAX_QP; i++) {
		/* If the dd_qp is NULL the dm_qp is not used */
		if (++free_qp == ROQ_MAX_QP)
			free_qp = 1;
		if (dev->RoQ->QP_Array[free_qp].dd_qp == NULL) {
			dm_qp = &dev->RoQ->QP_Array[free_qp];
			break;
		}
	}
	if (!dm_qp) {
		spin_unlock_bh(&dev->qp_lock);
		rv = -ENOMEM;
		goto fail;
	}

	roq_qpid(dd_qp) = free_qp;

	/* zero the complete DM QP struct */
	memset(dm_qp, 0, sizeof *dm_qp);
	spin_lock_init(&dm_qp->dd_lock);

	/* associate the dev mem qp slot with the dd/ofa qp struct
	 * to be returned upon work completion in CQE
	 */
	dm_qp->dd_qp_id = free_qp;
	dm_qp->dd_qp = dd_qp;

	init_rwsem(&dd_qp->state_lock);
	init_waitqueue_head(&dd_qp->ctrl_waitq);

	spin_unlock_bh(&dev->qp_lock);

	/* Let the virtual device know the type of service */
	dm_qp->qp_type = init_attrs->qp_type;
	dprint(DBG_CM, KERN_INFO, "qp type %d %s ",
		dm_qp->qp_type,
		dm_qp->qp_type == IB_QPT_UD ? "IB_QPT_UD" : "IB_QPT_RC");

	/* hook up the scq dma addr so the RoQ can find the cq */
	scq = container_of(dd_qp->init_attrs.send_cq, struct roq_cq, ofa_cq);
	rcq = container_of(dd_qp->init_attrs.recv_cq, struct roq_cq, ofa_cq);

	dm_qp->mSendCQ_DmaPtr = (struct roq_dm_cq *)scq->dma_addr;
	dm_qp->mRecvCQ_DmaPtr = (struct roq_dm_cq *)rcq->dma_addr;

	/* Allocate and initialize the send queue. */
	if (init_attrs->cap.max_send_wr) {
		dd_qp->sq_size =
			roundup_pow_of_two(init_attrs->cap.max_send_wr);

		atomic_set(&dd_qp->sq_index, dd_qp->sq_size - 1);
		sq_alloc_size = dd_qp->sq_size * sizeof(struct roq_sqe);

		if (mapped_qp && udata) {
			dd_qp->sq_virt =
				dma_alloc_coherent(ofa_dev->dma_device,
						   sq_alloc_size,
						   &dd_qp->sq_phys,
						   GFP_KERNEL);

			SendQueueDmaAddr = dd_qp->sq_phys;
			sq_is_mapped = 1;
		} else
			dd_qp->sq_virt = roq_kmalloc(sq_alloc_size,
						     &SendQueueDmaAddr);
		
		if (!dd_qp->sq_virt) {
			pr_warn("SQ alloc failed: size %u\n", dd_qp->sq_size);
			goto fail;
		}
		dd_qp->sq_phys = (uintptr_t)SendQueueDmaAddr;

		KISS_SRSWQ_Init(&dm_qp->mSendQueue,
				sizeof(struct roq_sqe),
				dd_qp->sq_size,
				SendQueueDmaAddr);

		dprint(DBG_OBJ, KERN_INFO,
		      	" QP(%d): SendQueue dd_qp 0x%p ->sq_virt 0x%p "
			"DmaAddr 0x%p dm_qp@ 0x%p "
			"->mSendQueue.mItemArrayDmaAddr %0llX",
			OBJ_ID(dd_qp), dd_qp, dd_qp->sq_virt, SendQueueDmaAddr,
			dm_qp,
			dm_qp->mSendQueue.mItemArrayDmaAddr);
	} else {
		dprint(DBG_OBJ, KERN_INFO,
		       "WARNING: QP(%d): SendQueue has 0 length",
			OBJ_ID(dd_qp));
	}
	kref_init(&dd_qp->refcnt);

	/* Allocate and initialize the receive queue. */
	if (init_attrs->cap.max_recv_wr && !init_attrs->srq) {
		dd_qp->rq_size =
			roundup_pow_of_two(init_attrs->cap.max_recv_wr);

		atomic_set(&dd_qp->rq_index, dd_qp->rq_size - 1);
		rq_alloc_size = dd_qp->rq_size * sizeof(struct roq_rqe);

		if (mapped_qp && udata) {
			dd_qp->rq_virt = dma_alloc_coherent(ofa_dev->dma_device,
						    rq_alloc_size,
						    &dd_qp->rq_phys,
						    GFP_KERNEL);
			RecvQueueDmaAddr = dd_qp->rq_phys;
			rq_is_mapped = 1;

		} else
			dd_qp->rq_virt = roq_kmalloc(rq_alloc_size,
						     &RecvQueueDmaAddr);

		if (!dd_qp->rq_virt) {
			pr_warn("RQ alloc failed: size %u\n", dd_qp->rq_size);
			goto fail;
		}
		dd_qp->rq_phys = RecvQueueDmaAddr;

		KISS_SRSWQ_Init(&dm_qp->mRecvQueue,
				sizeof(struct roq_rqe),
				dd_qp->rq_size,
				RecvQueueDmaAddr);

		dprint(DBG_OBJ, KERN_INFO,
		       " QP(%d): RecvQueue dd_qp 0x%p ->rq_virt 0x%p "
			"DmaAddr 0x%p dm_qp@ 0x%p "
			"->mRecvQueue.mItemArrayDmaAddr %0llX",
		       OBJ_ID(dd_qp), dd_qp, dd_qp->rq_virt, RecvQueueDmaAddr,
		       dm_qp,
		       dm_qp->mRecvQueue.mItemArrayDmaAddr);
	} else if (init_attrs->srq) {
		/*
		 * SRQ support.
		 * Verbs 6.3.7: ignore RQ size, if SRQ present
		 * Verbs 6.3.5: do not check PD of SRQ against PD of QP
		 *
		 * DM QP's RQ control struct is provided by SRQ
		 */
		struct roq_srq *srq = container_of(init_attrs->srq,
						   struct roq_srq, ofa_srq);

		void *q_phys = (void *)srq->srq_phys
				+ offsetof(struct roq_srq, DmQueue);

		dm_qp->SharedRecvQueue = q_phys;
		dd_qp->ofa_qp.srq = init_attrs->srq;

		dprint(DBG_OBJ, KERN_INFO, " QP(%d): SRQ(%p) attached\n",
			roq_qpid(dd_qp), dd_qp->ofa_qp.srq);
	} else {
		dprint(DBG_OBJ, KERN_INFO,
			"WARNING: QP(%d): RecvQueue has 0 length",
			OBJ_ID(dd_qp));
	}

	/*
	 * Initialize RQ and SQ arrrays on behalf of the client
	 */
	if (dd_qp->sq_virt)
		memset(dd_qp->sq_virt, 0, sq_alloc_size);
	if (dd_qp->rq_virt)
		memset(dd_qp->rq_virt, 0, rq_alloc_size);

	if (udata) {
		/*
		 * User level QP:
		 * Return identifier of QP and queue sizes
		 * to allow user level memory mapping
		 */
		struct roq_uresp_create_qp	uresp;
		struct roq_ucontext		*uctx;

		uctx =	roq_ctx_ofa2roq(ofa_pd->uobject->context);
		memset(&uresp, 0, sizeof uresp);

		if (sq_is_mapped) {
			uresp.sq_key = roq_insert_uobj(uctx, dd_qp->sq_virt,
						       sq_alloc_size);
			if (uresp.sq_key > ROQ_MAX_UOBJ_KEY) {
				pr_warn("Preparing mmap SQ failed\n");
				rv = -ENOMEM;
				goto fail;
			}
			uresp.sq_size = sq_alloc_size;
			uresp.flags = QUEUE_FLAG_MAPPEABLE;
		} else
			uresp.sq_size = dd_qp->sq_size;

		if (init_attrs->srq)
			uresp.rq_size = 0;
		else {
			if (rq_is_mapped) {
				uresp.rq_key = roq_insert_uobj(uctx,
							dd_qp->rq_virt,
							rq_alloc_size);
				if (uresp.rq_key > ROQ_MAX_UOBJ_KEY) {
					pr_warn("Preparing mmap RQ failed\n");
					rv = -ENOMEM;
					goto fail;
				}
				uresp.rq_size = rq_alloc_size;
			} else
				uresp.rq_size = init_attrs->cap.max_recv_wr;
		}
		uresp.qp_id = roq_qpid(dd_qp);

		rv = ib_copy_to_udata(udata, &uresp, sizeof uresp);
		if (rv)
			goto fail;

		dd_qp->is_user = 1;
	}

	spin_lock_init(&dd_qp->sq_lock);
	spin_lock_init(&dd_qp->rq_lock);

	dprint(DBG_OBJ, KERN_INFO, " QP(%d): Initialized to IDLE",
	       OBJ_ID(dd_qp));

	/* heading for trouble because we don't reuse the slots */
	BUG_ON(atomic_read(&dev->num_qp) > ROQ_MAX_QP);

	if (debug_cm)
		pr_info("QP %04d: create\n", roq_qpid(dd_qp));

	if (sq_is_mapped) {
		dd_qp->sq_mmapped = 1;
		dm_qp->sq_mmapped = 1;
	}
	if (rq_is_mapped) {
		dd_qp->rq_mmapped = 1;
		dm_qp->rq_mmapped = 1;
	}

	INIT_LIST_HEAD(&dd_qp->devq);
	spin_lock_bh(&dev->qp_lock);
	list_add_tail(&dd_qp->devq, &dev->qp_list);
	spin_unlock_bh(&dev->qp_lock);

	return &dd_qp->ofa_qp;

fail:
	if (dd_qp->sq_virt) {
		if (sq_is_mapped)
			dma_free_coherent(ofa_dev->dma_device,
					  sq_alloc_size,
					  dd_qp->sq_virt,
					  dd_qp->sq_phys);
		else
			roq_kfree(dd_qp->sq_virt);
	}
	if (dd_qp->rq_virt) {
		if (rq_is_mapped)
			dma_free_coherent(ofa_dev->dma_device,
					  rq_alloc_size,
					  dd_qp->rq_virt,
					  dd_qp->rq_phys);
		else
			roq_kfree(dd_qp->rq_virt);
	}
	if (dm_qp) {
		dm_qp->dd_qp_id = RoQ_INVALID_QP;
		dm_qp->dd_qp = NULL;
	}
	kfree(dd_qp);

	atomic_dec(&dev->num_qp);
	pr_warn("FAILED rv %d\n", rv);

	return (struct ib_qp *)ERR_PTR(rv);
}


char ib_qp_state_to_string[IB_QPS_ERR + 1][6] = {
	[IB_QPS_RESET] = "RESET",
	[IB_QPS_INIT] = "INIT",
	[IB_QPS_RTR] = "RTR",
	[IB_QPS_RTS] = "RTS",
	[IB_QPS_SQD] = "SQD",
	[IB_QPS_SQE] = "SQE",
	[IB_QPS_ERR] = "ERR"
};

struct ib_qp *
roq_get_ofaqp(struct ib_device *dev, int id)
{
	struct roq_dev		*bdev = roq_dev_ofa2roq(dev);
	struct RoQ_DM_QP	*dm_qp;
	struct roq_qp		*dd_qp;

	BUG_ON(id > ROQ_MAX_QP || id < 0);

	dm_qp = (struct RoQ_DM_QP *)&bdev->RoQ->QP_Array[id];
	dd_qp = dm_qp->dd_qp;

	dprint_tsk(DBG_OBJ, KERN_INFO, " Dev Name: %s, OFA QPID: %d, QP: %p",
		   dev->name, id, dm_qp);

	return dd_qp != NULL ? &dd_qp->ofa_qp : NULL;

}

static inline void
print_ib_qp_attr_mask(int attr_mask, struct ib_qp_attr *attrs)
{
#ifndef DPRINT_MASK
		return;
#else
	dprint(DBG_ON, KERN_INFO, "------- qp attr mask ---");
	if (IB_QP_STATE & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_STATE - Req %d", attrs->qp_state );
	if (IB_QP_CUR_STATE & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_CUR_STATE");
	if (IB_QP_EN_SQD_ASYNC_NOTIFY & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_EN_SQD_ASYNC_NOTIFY");
	if (IB_QP_ACCESS_FLAGS & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_ACCESS_FLAGS");
	if (IB_QP_PKEY_INDEX & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_PKEY_INDEX");
	if (IB_QP_PORT & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_PORT");
	if (IB_QP_QKEY & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_QKEY");
	if (IB_QP_AV & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_AV");
	if (IB_QP_PATH_MTU & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_PATH_MTU");
	if (IB_QP_TIMEOUT & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_TIMEOUT");
	if (IB_QP_RETRY_CNT & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_RETRY_CNT");
	if (IB_QP_RNR_RETRY & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_RNR_RETRY");
	if (IB_QP_RQ_PSN & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_RQ_PSN");
	if (IB_QP_MAX_QP_RD_ATOMIC & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_MAX_QP_RD_ATOMIC");
	if (IB_QP_ALT_PATH & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_ALT_PATH");
	if (IB_QP_MIN_RNR_TIMER & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_MIN_RNR_TIMER");
	if (IB_QP_SQ_PSN & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_SQ_PSN");
	if (IB_QP_MAX_DEST_RD_ATOMIC & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_MAX_DEST_RD_ATOMIC");
	if (IB_QP_PATH_MIG_STATE & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_PATH_MIG_STATE");
	if (IB_QP_CAP & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_CAP");
	if (IB_QP_DEST_QPN & attr_mask)
		dprint(DBG_ON, KERN_INFO, "IB_QP_DEST_QPN");
	dprint(DBG_ON, KERN_INFO, "------------------------");
#endif
}

volatile union roq_tx_db_req *roq_qp_db = NULL;
atomic_t qp_db_idx;

static inline void
roq_tx_doorbell(u32 qp_id, int num_wqe)
{
	union roq_tx_db_req req = {{
		.qp_id = qp_id,
		.num_wqe = num_wqe
	}}; 
	int idx = atomic_inc_return(&qp_db_idx) % ROQ_SQ_DB_SIZE;

	if (likely(roq_qp_db[idx].item == 0))
		roq_qp_db[idx].item = req.item;
	else
		pr_warn("QP DB full for QP %u at idx %d", qp_id, idx);
}

static enum
roq_qp_state roq_qp_map_state(enum ib_qp_state ofa_state)
{
	switch (ofa_state) {
		case IB_QPS_RESET:	return ROQ_QPS_RESET;
		case IB_QPS_INIT:	return ROQ_QPS_INIT;
		case IB_QPS_RTR:	return ROQ_QPS_RTR;
		case IB_QPS_RTS:	return ROQ_QPS_RTS;
		case IB_QPS_SQD:	return ROQ_QPS_SQD;
		case IB_QPS_SQE:	return ROQ_QPS_SQE;
		case IB_QPS_ERR:	return ROQ_QPS_ERROR;

		/* Function can be called with invalid state */
		default:
			return ROQ_QPS_RESET;
	}
}


static void
roq_qp_map_attr(struct ib_qp_attr *ofa_attr, struct roq_qp_attr *roq_attr)
{
	memset(roq_attr, 0, sizeof *roq_attr);

	roq_attr->dest_qp_num	= ofa_attr->dest_qp_num;
	roq_attr->trace_key	= ofa_attr->sq_psn;
	roq_attr->dest_node_id	= ofa_attr->ah_attr.dlid;
	roq_attr->io_link_id	= ofa_attr->ah_attr.port_num;
	roq_attr->sq_size	= ofa_attr->cap.max_send_wr;
	roq_attr->rq_size	= ofa_attr->cap.max_recv_wr;
	roq_attr->max_inline	= ofa_attr->cap.max_inline_data;
	roq_attr->max_send_sge	= ofa_attr->cap.max_send_sge;
	roq_attr->max_recv_sge	= ofa_attr->cap.max_recv_sge;
	roq_attr->qp_state	= roq_qp_map_state(ofa_attr->qp_state);
	roq_attr->cur_qp_state	= roq_qp_map_state(ofa_attr->cur_qp_state);
}

static u32
roq_qp_map_attr_mask(int ofa_mask)
{
	enum roq_qp_cmd_mask roq_mask = 0;

	if (ofa_mask & IB_QP_STATE)
		roq_mask = ROQ_QP_SET_STATE;
	if (ofa_mask & IB_QP_CUR_STATE)
		roq_mask |= ROQ_QP_CUR_STATE;
	if (ofa_mask & IB_QP_ACCESS_FLAGS)
		roq_mask |= ROQ_QP_SET_ACCESS_FLAGS;
	if (ofa_mask & IB_QP_DEST_QPN)
		roq_mask |= ROQ_QP_SET_DEST_QP;
	if (ofa_mask & IB_QP_AV)
		roq_mask |= ROQ_QP_SET_DEST_NODE;
	if (ofa_mask & IB_QP_CAP)
		roq_mask |= ROQ_QP_SET_CAP;
	if (ofa_mask & IB_QP_RQ_PSN)
		roq_mask |= ROQ_QP_START_TRACE;
	if (ofa_mask & IB_QP_SQ_PSN)
		roq_mask |= ROQ_QP_STOP_TRACE;

	return roq_mask;
}


static int roq_dm_qp_modify(struct roq_dev *dev, struct roq_qp *qp,
			    struct roq_qp_attr *attr, u32 cmd_mask)
{
	struct RoQ_DeviceMemory *RoQ = roq_dev2dm_ctrl(dev);
	struct RoQ_CNTL_Command	*CtrlCmd;
	uint64_t my_req = 0;
	unsigned long flags;
	int rv = 0;

	rv = roq_ctrl_slot_get(dev);
	if (rv)
		goto out;

	/*
	 * Keep the QP around until response comes back from DM even if it
	 * gets abandoned by user
	 */
	roq_qp_get_ref(&qp->ofa_qp);

	spin_lock_irqsave(&dev->command_lock, flags);

	my_req =  *(unsigned long volatile *)&RoQ->CNTL_RequestPosted;

	CtrlCmd = &RoQ->CNTL_Command[my_req % ROQ_CTRL_Q_SIZE];
	CtrlCmd->Type = RoQ_CNTL_Command_ModifyQP;
	CtrlCmd->Request.ModifyQP.QP_Num = roq_qpid(qp);
	CtrlCmd->Request.ModifyQP.cmd_mask = cmd_mask;
	memcpy(&(CtrlCmd->Request.ModifyQP.qp_attr), attr, sizeof *attr);

	set_mb(RoQ->CNTL_RequestPosted, my_req + 1);

	spin_unlock_irqrestore(&dev->command_lock, flags);

	rv = wait_event_interruptible(qp->ctrl_waitq,
		      dev->stopping ||
		      *(unsigned long volatile *)&RoQ->CNTL_RequestCompleted >
		      my_req);

	if (dev->stopping)
		rv = -EIO;

	if (!rv)
		rv = CtrlCmd->Return.ModifyQP.mRc;

	roq_ctrl_slot_put(dev);
out:
	if (debug_cm)
		pr_info("roq_dm_qp_modify: done: %d, %llu, %llu\n",
			rv, RoQ->CNTL_RequestCompleted, my_req + 1);

	return rv;
}


/*
 * Must be called with state_lock semaphore held
 */
int
roq_modify_qp(struct ib_qp *ofa_qp, struct ib_qp_attr *ofa_attr,
		   int ofa_attr_mask, struct ib_udata *udata)
{
	struct roq_qp		*dd_qp = roq_qp_ofa2roq(ofa_qp);
	struct ib_device	*ib_dev = ofa_qp->device;
	struct roq_dev		*rdev = roq_dev_ofa2roq(ib_dev);
	struct RoQ_DM_QP	*dm_qp;
	struct roq_qp_attr	attr;
	struct iw_cm_id		*cm_id = NULL;
	u32			cmd_mask = roq_qp_map_attr_mask(ofa_attr_mask);

	struct RoQ_DeviceMemory *RoQ = roq_dev2dm_ctrl(rdev);

	unsigned long flags;
	int set_state, rv = 0;

	roq_qp_map_attr(ofa_attr, &attr);
	
	if (unlikely(roq_active == 0)) {
		rv = -ENETDOWN;
		goto out;
	}
	if (cmd_mask == 0)
		goto out;

	set_state = (cmd_mask & ROQ_QP_SET_STATE) ? 1:0;

	print_ib_qp_attr_mask(ofa_attr_mask, ofa_attr);

	dm_qp = roq_get_dm_qp(rdev, roq_qpid(dd_qp));
	if (!dm_qp) {
		pr_info("roq_modify_qp: invalid QP ID %u\n", roq_qpid(dd_qp));
		rv = -EINVAL;
		goto out;
	}
	spin_lock_irqsave(&dm_qp->dd_lock, flags);

	if (dm_qp->dd_qp != dd_qp) {
		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
		pr_info("roq_modify_qp: invalid QP ID %u\n", roq_qpid(dd_qp));
		rv = -EINVAL;
		goto out;
	}
	if (set_state && attr.qp_state > ROQ_QPS_SQD) {
		cm_id = dm_qp->cm_id;
		dm_qp->cm_id = NULL;
	}
	if (set_state && attr.qp_state == ROQ_QPS_RTS)
		dd_qp->is_connecting = 0;

	if (dm_qp->qp_state > ROQ_QPS_RTR) {
		dd_qp->is_connecting = 0;
		dd_qp->active_connect = 0;
	}
	spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

	if (debug_cm && set_state)
		pr_info("QP[%d]: roq_qp_modify: %d --> %d (%p)\n",
			roq_qpid(dd_qp), dm_qp->qp_state,
			attr.qp_state, dm_qp->cm_id);

	if (cmd_mask & ROQ_QP_SET_CAP) {
		/* No queue receizing allowed, but num SGE may change */
		if ((attr.sq_size != dd_qp->init_attrs.cap.max_send_wr) ||
		    (attr.rq_size != dd_qp->init_attrs.cap.max_recv_wr) ||
		    (attr.max_send_sge > ROQ_MAX_SGE) ||
		    (attr.max_recv_sge > ROQ_MAX_SGE) ||
		    (attr.max_inline > ROQ_MAX_INLINE)) {

			if (debug_cm)
				pr_info("QP[%d]: roq_qp_modify() CAP error: "
					"%u -- %u, %u -- %u, %u, %u, %u\n",
					roq_qpid(dd_qp), attr.sq_size,
					dd_qp->init_attrs.cap.max_send_wr,
					attr.rq_size,
					dd_qp->init_attrs.cap.max_recv_wr,
					attr.max_send_sge, attr.max_recv_sge,
					attr.max_inline);
			rv = -EINVAL;
			goto out;
		}
		dd_qp->init_attrs.cap.max_send_sge = attr.sq_size;
		dd_qp->init_attrs.cap.max_recv_sge = attr.rq_size;
		dd_qp->init_attrs.cap.max_inline_data = attr.max_inline;
	}
// #define PKTRACE_TEST_EXTENSION
	if (cmd_mask & ROQ_QP_START_TRACE) {
		int a;

		TraceOnLinux(RoQ, attr.trace_key);
		HitOE(gTraceClientSync,"TraceClientSync", 0);

		for (a = 0; a < 10; a += 1) {
#if defined(PKTRACE_TEST_EXTENSION)
	      		HitOE1(TraceCountLinuxStart,
				"TraceCountLinux_%02x",0,a % 16);
			HitOE2(TraceCountLinux2Start,
				"TraceCountLinux_%02x_%02x",0,a%16,(a/2)%16);
			HitOE3(TraceCountLinux3Start,
				"TraceCountLinux_%02x_%02x_%02x"
				,0,a%16,(a/2)%16,(a/4)%16);
#endif
			HitOE(TraceOnLinuxStart,"TraceOnLinux", 0);
			HitOE(TraceOnLinuxFinis,"TraceOnLinux", 0);

#if defined(PKTRACE_TEST_EXTENSION)
			HitOE3(TraceCountLinux3Finis,
				"TraceCountLinux_%02x_%02x_%02x",
				0,a%16,(a/2)%16,(a/4)%16);
			HitOE2(TraceCountLinux2Finis,
				"TraceCountLinux_%02x_%02x",0,a%16,(a/2)%16);
			HitOE1(TraceCountLinuxFinis,
				"TraceCountLinux_%02x",0,a % 16);
#endif
 		}
	}
	if (cmd_mask & ROQ_QP_STOP_TRACE)
		TraceOffLinux();

	roq_qp_get_ref(ofa_qp);

	rv = roq_dm_qp_modify(rdev, dd_qp, &attr, cmd_mask);

	if (cm_id) {
		if (debug_cm) {
			pr_info("QP %04d: drop_conn - connecting:active "
				"(%d:%d), cb %p\n",
				roq_qpid(dd_qp), dd_qp->is_connecting,
				dd_qp->active_connect, cm_id);
		}
		if (dd_qp->is_connecting) {
			if (dd_qp->active_connect)
				roq_cm_upcall(dd_qp, cm_id,
					      IW_CM_EVENT_CONNECT_REPLY,
					      -EINVAL);
			else {
				roq_cm_upcall(dd_qp, cm_id,
					      IW_CM_EVENT_DISCONNECT, 0);
				roq_cm_upcall(dd_qp, cm_id, IW_CM_EVENT_CLOSE,
					      0);
			}
		} else 
			roq_cm_upcall(dd_qp, cm_id, IW_CM_EVENT_CLOSE, 0);

		dd_qp->is_connecting = 0;

		cm_id->rem_ref(cm_id);
	}
        if (debug_cm)
		pr_info("QP[%d]: roq_qp_modify() done. rv %d\n",
			roq_qpid(dd_qp), rv);

	roq_qp_put_ref(ofa_qp);

out:
	return rv;
}

int
roq_ofed_modify_qp(struct ib_qp *ofa_qp, struct ib_qp_attr *ofa_attr,
		   int ofa_attr_mask, struct ib_udata *udata)
{
	struct roq_qp *dd_qp = roq_qp_ofa2roq(ofa_qp);
	int rv;

	down_write(&dd_qp->state_lock);
	rv = roq_modify_qp(ofa_qp, ofa_attr, ofa_attr_mask, udata);
	up_write(&dd_qp->state_lock);

	return rv;
}

int roq_query_qp(struct ib_qp *qp, struct ib_qp_attr *qp_attr,
                     int qp_attr_mask, struct ib_qp_init_attr *qp_init_attr)
{
    qp_attr->cap.max_inline_data = ROQ_MAX_INLINE;
    qp_init_attr->cap.max_inline_data = 0;
    return 0;
}


/*
 * Free QP structure if all references are gone
 */
static void roq_free_qp(struct kref *ref)
{
	struct roq_qp	 *dd_qp = container_of(ref, struct roq_qp, refcnt);
	struct RoQ_DM_QP *dm_qp;
	unsigned long flags;

	if (debug_cm)
		pr_info("QP %04d: free\n", roq_qpid(dd_qp));

	roq_cm_cancel_timer(dd_qp);

	dm_qp = roq_get_dm_qp(roq_device, roq_qpid(dd_qp));
	if (!dm_qp) {
		pr_warn("roq_free_qp: invalid QP ID %u\n", roq_qpid(dd_qp));
		goto out;
	}
	spin_lock_irqsave(&dm_qp->dd_lock, flags);

	dm_qp->dd_qp = NULL;
	dm_qp->dd_qp_id = RoQ_INVALID_QP;

	kfree(dd_qp->cm_msg);
	dd_qp->cm_msg = NULL;

	spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
out:
	kfree(dd_qp);

	atomic_dec(&roq_device->num_qp);
}


void
roq_qp_get_ref(struct ib_qp *ofa_qp)
{
	struct roq_qp *qp = roq_qp_ofa2roq(ofa_qp);

	if (debug_cm)
		pr_info("QP %04d: get ref: %u\n", roq_qpid(qp),
			atomic_read(&qp->refcnt.refcount) + 1);

	kref_get(&qp->refcnt);
}

void
roq_qp_put_ref(struct ib_qp *ofa_qp)
{
	struct roq_qp *qp = roq_qp_ofa2roq(ofa_qp);

	if (debug_cm)
		pr_info("QP %04d: put ref: %u\n",
			roq_qpid(qp), atomic_read(&qp->refcnt.refcount) - 1);

	kref_put(&qp->refcnt, roq_free_qp);
}

int
roq_destroy_qp(struct ib_qp *ofa_qp)
{
	struct ib_device	*ofa_dev = ofa_qp->device;
	struct roq_dev		*dev = roq_dev_ofa2roq(ofa_dev);
	struct roq_qp		*qp = roq_qp_ofa2roq(ofa_qp);
	struct RoQ_DM_QP	*dm_qp = roq_get_dm_qp(dev, roq_qpid(qp));
	struct ib_qp_attr	qp_attr;
	enum ib_qp_attr_mask	qp_attr_mask;

	if (unlikely(!dm_qp))
		return -EINVAL;

	memset(&qp_attr, 0, sizeof qp_attr);

	if (debug_cm)
		pr_info("QP %04d: destroy, DM QP state %d, sq=0x%p::0x%llx, "
			"rq=0x%p::0x%llx, mapped=%d:%d\n",
			roq_qpid(qp), dm_qp->qp_state, qp->sq_virt,
			qp->sq_phys,
			qp->rq_virt, qp->rq_phys,
			qp->sq_mmapped, qp->rq_mmapped);

	down_write(&qp->state_lock);

	if (dm_qp->qp_state != ROQ_QPS_ERROR) {
		qp_attr_mask = IB_QP_STATE;
		qp_attr.qp_state = IB_QPS_ERR;

		roq_modify_qp(&qp->ofa_qp, &qp_attr, (int)qp_attr_mask, NULL);
	}

	/* Free memory allocated for queues. */
	if (qp->sq_virt) {
		if (qp->sq_mmapped)
			dma_free_coherent(ofa_dev->dma_device,
					  qp->sq_size * sizeof(struct roq_sqe),
					  qp->sq_virt,
					  qp->sq_phys);
		else
			roq_kfree(qp->sq_virt);
	}
	if (!dm_qp->SharedRecvQueue && qp->rq_virt) {
		if (qp->rq_mmapped)
			dma_free_coherent(ofa_dev->dma_device,
					  qp->rq_size * sizeof(struct roq_rqe),
					  qp->rq_virt,
					  qp->rq_phys);
		else
			roq_kfree(qp->rq_virt);
	}
	qp->init_attrs.send_cq = NULL;
	qp->init_attrs.recv_cq = NULL;

	qp_attr_mask     = IB_QP_STATE;
	qp_attr.qp_state = IB_QPS_RESET;

	roq_modify_qp(&qp->ofa_qp, &qp_attr, (int)qp_attr_mask, NULL);

	kfree(qp->conn_data);
	qp->conn_data = NULL;

	up_write(&qp->state_lock);

	spin_lock_bh(&dev->qp_lock);
	list_del(&qp->devq);
	spin_unlock_bh(&dev->qp_lock);

	roq_qp_put_ref(ofa_qp);

	return 0;
}

struct	roq_cq *cq_array[ROQ_MAX_CQ + 1];

static int get_cq_slot(struct roq_dev *dev, struct roq_cq *cq)
{
	static int cq_idx = 0;
	int i = 0, found = -ENOMEM;
	unsigned long flags;

	/* Initializer */
	if (cq_idx == 0) {
		spin_lock_init(&dev->cq_lock);
		for (cq_idx = 1; cq_idx < ROQ_MAX_CQ + 1; cq_idx++)
			cq_array[cq_idx] = NULL;
		cq_idx = 1;
	}
	spin_lock_irqsave(&dev->cq_lock, flags);

	while (i++ < ROQ_MAX_CQ) {
		if (cq_array[cq_idx] == NULL) {
			cq_array[cq_idx] = cq;
			found = cq_idx;
		}
		if (++cq_idx == ROQ_MAX_CQ + 1)
			cq_idx = 1;
		if (found > 0)
			break;
	}
	spin_unlock_irqrestore(&dev->cq_lock, flags);

	return found;
}

#ifdef DOORBELL_SYSCALL

static unsigned long roq_doorbell_rq(u32 qp_id, u32 num_wqe)
{
	struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(roq_device, qp_id);
	/*
	 * TODO: check if QP is assigned with current process.
	 */
	if (likely(dm_qp && dm_qp->qp_state <= ROQ_QPS_RTS)) {
		dm_qp->mRecvQueue.mPut += num_wqe;
		smp_wmb();

		return 0;
	}
	if (dm_qp)
		return -ESHUTDOWN;

	return -EINVAL;
}

static unsigned long roq_doorbell_sq(u32 qp_id, u32 num_wqe)
{
	struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(roq_device, qp_id);
	unsigned long flags;
	int rv = 0;

	if (likely(dm_qp)) {
		spin_lock_irqsave(&dm_qp->dd_lock, flags);

		if (likely(dm_qp->qp_state == ROQ_QPS_RTS))
			roq_tx_doorbell(qp_id, num_wqe);
		else
			rv = -ESHUTDOWN;

		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
	} else
		rv = -EINVAL;

	return rv;
}

static long roq_doorbell_cq_poll(u32 cq_id, u32 num_cqe)
{
	struct roq_cq	 *dd_cq;

	if (likely(cq_id < ROQ_MAX_CQ))
		dd_cq = cq_array[cq_id];
	else
		goto error;

	if (likely(dd_cq)) {
		struct roq_dm_cq *dm_cq;

		spin_lock(&dd_cq->lock);

		dm_cq = &dd_cq->dm_cq;

		dm_cq->DmQueue.mGot += num_cqe;
		smp_wmb();

		spin_unlock(&dd_cq->lock);

		return 0;
	}
error:
	pr_info("unknown poll %u at CQ %u\n", num_cqe, cq_id);

	return -EINVAL;
}

static long roq_doorbell_cq_notify(u32 cq_id, u32 reason)
{
	int rv = 0;

	if (likely(cq_id < ROQ_MAX_CQ)) {
		struct roq_cq *dd_cq = cq_array[cq_id];

		if (likely(dd_cq)) {
			struct roq_dm_cq *dm_cq = &dd_cq->dm_cq;

			spin_lock(&dd_cq->lock);

			switch (reason) {

			case IB_CQ_SOLICITED:
				*(volatile enum roq_notify_flags *)
				&dm_cq->notify = ROQ_NOTIFY_SOLICITED;
				break;

			case IB_CQ_NEXT_COMP:
				*(volatile enum roq_notify_flags *)
				&dm_cq->notify = ROQ_NOTIFY_NEXT_COMPLETION;
				break;

			default:
				pr_info("unknown notify %u at CQ %u\n",
					reason, cq_id);
				rv = -EINVAL;
			}
			smp_wmb();

			spin_unlock(&dd_cq->lock);
		} else {
			rv = -EINVAL;
			pr_info("CQ ID %u unassigned (notify %u)\n",
				cq_id, reason);
		}
	} else {
		pr_info("CQ ID %u out of bounds (notify %u)\n",
			cq_id, reason);
		rv = -EINVAL;
	}
	return rv;
}

static long roq_doorbell_srq(u32 cq_id, u32 reason)
{
	pr_warn("SRQ DB not implemented\n");
	return -ENOSYS;
}

long roq_doorbell(enum roq_db_type type, u32 id, u32 arg)
{
	switch (type) {

	case ROQ_DB_SQ:		return roq_doorbell_sq(id, arg);
	case ROQ_DB_CQ_POLL:	return roq_doorbell_cq_poll(id, arg);
	case ROQ_DB_CQ_NOTIFY:	return roq_doorbell_cq_notify(id, arg);
	case ROQ_DB_RQ:		return roq_doorbell_rq(id, arg);
	case ROQ_DB_SRQ:	return roq_doorbell_srq(id, arg);

	default:
		if (db_orig_call)
			return (*db_orig_call)(type, id, arg);

		pr_warn("unknown doorbell %u\n", type);
		return -ENOSYS;
	}
}

#endif /* DOORBELL_SYSCALL */

static int roq_post_send_mapped(struct roq_qp *dd_qp, struct roq_dev *rdev)
{
	struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(rdev, roq_qpid(dd_qp));

	if (unlikely(!dm_qp))
		return -EINVAL;

	if (unlikely((dm_qp->mSendQueue.mPut - dm_qp->mSendQueue.mGot) >
		      dm_qp->mSendQueue.mItemCount)) {
			pr_info("post_send: SQ overflow %llu, %llu, %llu\n",
				dm_qp->mSendQueue.mPut, dm_qp->mSendQueue.mGot,
				dm_qp->mSendQueue.mItemCount);
		return -ENOMEM;
	}
	roq_tx_doorbell(roq_qpid(dd_qp), 1);

	return 0;
}

int
roq_post_send(struct ib_qp *ofa_qp, struct ib_send_wr *wr,
	      struct ib_send_wr **bad_wr)
{
	struct roq_qp		*dd_qp = roq_qp_ofa2roq(ofa_qp);
	struct roq_dev		*rdev = roq_dev_ofa2roq(ofa_qp->device);
	struct RoQ_DM_QP	*dm_qp = roq_get_dm_qp(rdev, roq_qpid(dd_qp));
	struct ib_send_wr	*first;

	int num_wqe = 0, rv = 0;

	BUG_ON(!dm_qp);

	*bad_wr = NULL;

	if (unlikely(dm_qp->qp_state != ROQ_QPS_RTS)) {
		dprint_tsk(DBG_WR | DBG_IWTX, KERN_INFO,
			   "ERROR: (QP%d): state=%d. expected RTS",
			   roq_qpid(dd_qp), dm_qp->qp_state);
		*bad_wr = wr;
		return -ESHUTDOWN;
	}
	if (dd_qp->sq_mmapped) {
		if (unlikely (wr))
			pr_warn("WR for mapped SQ?\n");
		return roq_post_send_mapped(dd_qp, rdev);
	}
	first = wr;

	while (wr) {
		struct roq_sqe	*sqe;
		u32 flags = ROQ_WQE_VALID;
		int sq_idx =
			atomic_inc_return(&dd_qp->sq_index) % dd_qp->sq_size;
		/*
		 * Most sanity checks for WR's are to be delayed until the
		 * data mover hits them.
		 */
		if (unlikely(wr->num_sge >
		             dd_qp->init_attrs.cap.max_send_sge)) {
			/*
			 * NOTE: we allow for zero length wr's here.
			 */
			dprint_tsk(DBG_WR | DBG_ON, KERN_INFO,
				   "(QP%d): WR %d: Num SGE: %d > "
				   "RoQ_MAX_SGE_NUM",
				   roq_qpid(dd_qp), wr->opcode, wr->num_sge);
			rv = -EINVAL;
			break;
		}
		/*
		 * use the index with the kernel virtual address for the
		 * queue body
		 */
		sqe = &dd_qp->sq_virt[sq_idx];
		/*
		 * Check for space in the qp's send queue for new postings
		 */
		smp_rmb();
		if (*(volatile int *)&sqe->flags & ROQ_WQE_VALID) {
			pr_warn("QP[%d]: SQ full\n", roq_qpid(dd_qp));
			rv = -ENOMEM;
			break;
		}

		/*
		 * Transfer OFA WR into ROQ private SQE representation
		 */
		sqe->id = wr->wr_id;
		sqe->num_sge = wr->num_sge;

		if ((wr->send_flags & IB_SEND_SIGNALED) ||
		    (dd_qp->init_attrs.sq_sig_type == IB_SIGNAL_ALL_WR))
			flags |= ROQ_WQE_SIGNALLED;

		if (wr->send_flags & IB_SEND_SOLICITED)
			flags |= ROQ_WQE_SOLICITED;

		switch (wr->opcode) {

		case IB_WR_RDMA_WRITE:
			sqe->opcode = ROQ_OP_WRITE;
			flags |= ROQ_WQE_ABSOLUTE_ADDR;
			sqe->wr.rdma.remote_addr = wr->wr.rdma.remote_addr;
			sqe->wr.rdma.rkey = wr->wr.rdma.rkey;

			break;

		case IB_WR_RDMA_READ:
		case IB_WR_RDMA_READ_WITH_INV:
			if (unlikely(sqe->num_sge != 1))
				rv = -EINVAL;

			sqe->opcode = ROQ_OP_READ;
			sqe->wr.rdma.remote_addr = wr->wr.rdma.remote_addr;
			sqe->wr.rdma.rkey = wr->wr.rdma.rkey;

			break;

		case IB_WR_SEND:
			sqe->opcode = ROQ_OP_SEND;

			break;

		default:
			rv = -EINVAL;
			break;
		}

		/*
		 *  This assumes OFA sg element to have same layout as
		 *  struct roq_sge.
		 */
		if (likely(!rv)) {

			memcpy(sqe->sge, wr->sg_list,
			       sizeof(struct roq_sge) * sqe->num_sge);

			/* publish the wr to the DM */
			wmb();

			set_mb(sqe->flags, flags);

			num_wqe++;

			wr = wr->next;
		} else
			break;
	}

	/* ring tx doorbell if any wr made into the wq */
	if (likely(wr != first))
		roq_tx_doorbell(roq_qpid(dd_qp), num_wqe);

	if (unlikely(rv)) {
		dprint_tsk(DBG_WR | DBG_ON, KERN_INFO, "(QP%d): error=%d",
			   roq_qpid(dd_qp), rv);
		*bad_wr = wr;
		if (first)
			atomic_dec(&dd_qp->sq_index);
	}

	return rv;
}

int
roq_post_receive(struct ib_qp *ofa_qp, struct ib_recv_wr *wr,
		 struct ib_recv_wr **bad_wr)
{
	struct roq_qp		*dd_qp = roq_qp_ofa2roq(ofa_qp);
	struct roq_dev		*rdev = roq_dev_ofa2roq(ofa_qp->device);
	struct RoQ_DM_QP	*dm_qp = roq_get_dm_qp(rdev, roq_qpid(dd_qp));
	int	rv = 0;

	if (dd_qp->ofa_qp.srq)
		return -EOPNOTSUPP;	/* what else from errno.h? */

	BUG_ON(!dm_qp);

	if (dm_qp->qp_state > ROQ_QPS_RTS) {
		dprint(DBG_ON, KERN_INFO,
		       "(QP%d): Dev Driver QP not RTS state=%d",
			roq_qpid(dd_qp), dm_qp->qp_state);
		return -EINVAL;
	}

	while (wr) {
		struct roq_rqe *rqe;
		u32 num_sge = wr->num_sge;
		int rq_idx =
			atomic_inc_return(&dd_qp->rq_index) % dd_qp->rq_size;

		if (num_sge > dd_qp->init_attrs.cap.max_recv_sge) {
			dprint_tsk(DBG_WR | DBG_ON, KERN_INFO,
				   "(QP%d): Receive has too many SGEs: %d",
				   roq_qpid(dd_qp),
				   wr->num_sge);
			rv = -EINVAL;
			break;
		}

		rqe = &dd_qp->rq_virt[rq_idx];

		/*
		 * Check if current RQE is empty
		 */
		if (*(volatile u32 *)&rqe->flags & ROQ_WQE_VALID) {
			pr_warn("QP[%d]: RQ full\n", roq_qpid(dd_qp));
			rv = -ENOMEM;
			break;
		}

		/*
		 *  This assumes OFA sg element to have same layout as
		 *  struct roq_sge.
		 */
		memcpy(rqe->sge, wr->sg_list,
		       sizeof(struct roq_sge) * num_sge);

		*(volatile u64 *)&rqe->id = wr->wr_id;
		*(volatile u32 *)&rqe->num_sge = wr->num_sge;

		set_mb(rqe->flags, ROQ_WQE_VALID);

		wr = wr->next;
	}

	if (rv) {
		pr_warn("QP[%d]: post_receive error: %d\n",
			roq_qpid(dd_qp), rv);
		atomic_dec(&dd_qp->rq_index);
		*bad_wr = wr;
	}
	return rv;
}

int
roq_destroy_cq(struct ib_cq *ofa_cq)
{
	struct ib_device *ofa_dev = ofa_cq->device;
	struct roq_dev *dev	  = roq_dev_ofa2roq(ofa_dev);
	struct roq_cq *dd_cq	  = roq_cq_ofa2roq(ofa_cq);
	struct roq_dm_cq *dm_cq	  = &dd_cq->dm_cq;
	int wq_size		  = dd_cq->ofa_cq.cqe * sizeof(struct roq_cqe);
	int cq_id		  = dm_cq->dd_cq_id;
	unsigned long flags;

	if (debug_cm)
		pr_info("CQ[%d]: Destroy\n", cq_id);

	spin_lock_irqsave(&dev->cq_lock, flags);

	if (dm_cq->is_mmapped && dm_cq->cq_virt)
		dma_free_coherent(ofa_dev->dma_device,
				  wq_size,
				  dm_cq->cq_virt,
				  (dma_addr_t)dm_cq->cq_phys);
	else if (dm_cq->cq_virt)
		roq_kfree(dm_cq->cq_virt);

	/* Inform the provider about CQ destruction */
	dm_cq->cq_phys = NULL;

	roq_kfree(dd_cq);

	atomic_dec(&dev->num_cq);

	cq_array[cq_id] = NULL;

	spin_unlock_irqrestore(&dev->cq_lock, flags);

	return 0;
}


struct ib_cq *
roq_create_cq(struct ib_device *ofa_dev, int size, int vec /* unused */ ,
	      struct ib_ucontext *ib_context, struct ib_udata *udata)
{
	struct roq_cq		*dd_cq = NULL;
	struct roq_dm_cq	*dm_cq = NULL;
	struct roq_dev		*dev = roq_dev_ofa2roq(ofa_dev);
	dma_addr_t	CQ_DmaAddr;
	dma_addr_t	CQ_CqeAddr;
	int	cqe_array_size,
		idx,
		rv = 0,
		cq_is_mmapped = (ib_context && mapped_cq) ? 1 : 0;

	if (atomic_inc_return(&dev->num_cq) > ROQ_MAX_CQ) {
		pr_info("Out of CQ's");
		rv = -ENOMEM;
		goto err_out;
	}

	size = roundup_pow_of_two(size);

	if (size < 1 || size > ROQ_MAX_CQE) {
		pr_warn("bad CQ size %d must be 1..%d", size, ROQ_MAX_CQE);
		rv = -EINVAL;
		goto err_out;
	}

	cqe_array_size = size * sizeof(struct roq_cqe);

	/*
	 * Allocate both driver CQ structure and DataMover object
	 */
	dd_cq = roq_kmalloc(sizeof *dd_cq, &CQ_DmaAddr);
	if (!dd_cq) {
		pr_warn("CQ allocation failure.\n");
		rv = -ENOMEM;
		goto err_out;
	}
	memset(dd_cq, 0, sizeof *dd_cq);
	dm_cq = &dd_cq->dm_cq;

	/* Allocate and initialize the CQE array */
	if (cq_is_mmapped)
		dm_cq->cq_virt = dma_alloc_coherent(ofa_dev->dma_device,
						    cqe_array_size,
						    (dma_addr_t *)&CQ_CqeAddr,
						    GFP_KERNEL);
	else 
		dm_cq->cq_virt = roq_kmalloc(cqe_array_size, &CQ_CqeAddr);

	if (!dm_cq->cq_virt) {
		pr_warn("CQE array alloc failed\n");
		rv = -ENOMEM;
		goto err_out;
	}

	/*
	 * Cleanup the CQ array
	 */
	idx = 0;
	while (idx < size) {
		struct roq_cqe *cqe = &dm_cq->cq_virt[idx];

		memset(cqe, 0, sizeof(struct roq_cqe));
		cqe->flags = ROQ_WC_FREE;
		idx++;
	}
	dm_cq->is_mmapped = cq_is_mmapped;
	dm_cq->cq_phys = (struct roq_cqe *)CQ_CqeAddr;

	dd_cq->dma_addr = CQ_DmaAddr;	/* Needed during create_qp() call */

	spin_lock_init(&dd_cq->lock);

	dm_cq->dd_cq_id = get_cq_slot(dev, dd_cq);
	if (dm_cq->dd_cq_id <= 0) {
		rv = -ENOMEM;
		goto err_out;
	}
	dprint(DBG_ON, KERN_INFO, "CQ %u DmaAddr 0x%p",
	       dm_cq->dd_cq_id, CQ_DmaAddr);

	KISS_SRSWQ_Init(&dm_cq->DmQueue, sizeof(struct roq_cqe), size, -1);

	if (debug_cm)
		pr_info("CQ[%d] with CQ array at %p\n",
			dm_cq->dd_cq_id, dm_cq->cq_phys);

	dd_cq->ofa_cq.cqe = size;

	if (ib_context) {
		struct roq_uresp_create_cq uresp;
		struct roq_ucontext	   *uctx = roq_ctx_ofa2roq(ib_context);

		memset(&uresp, 0, sizeof uresp);

		uresp.cq_id = dm_cq->dd_cq_id;

		if (cq_is_mmapped) {
			uresp.cq_key = roq_insert_uobj(uctx,
				 		       dm_cq->cq_virt,
						       cqe_array_size);
			uresp.cq_size = cqe_array_size;
			uresp.flags = QUEUE_FLAG_MAPPEABLE;
		}
		rv = ib_copy_to_udata(udata, &uresp, sizeof uresp);
		if (rv)
			goto err_out;
	}
	/* publish to DD/DM shared memory area before putting into use. */
	wmb();

	return &dd_cq->ofa_cq;

err_out:
	dprint_tsk(DBG_OBJ, KERN_INFO, "CQ creation failed");

	if (dd_cq) {
		if (!cq_is_mmapped)
			roq_kfree(dm_cq->cq_virt);

		else if (dm_cq->cq_virt)
			dma_free_coherent(ofa_dev->dma_device,
					  cqe_array_size,
					  dm_cq->cq_virt,
					  (dma_addr_t)dm_cq->cq_phys);

		roq_kfree(dd_cq);
		if (dm_cq->dd_cq_id > 0) {
			unsigned long flags;
			spin_lock_irqsave(&dev->cq_lock, flags);
			cq_array[dm_cq->dd_cq_id] = NULL;
			spin_unlock_irqrestore(&dev->cq_lock, flags);
		}
	}
	atomic_dec(&dev->num_cq);

	return (struct ib_cq *)ERR_PTR(rv);
}


static enum ib_wc_status map_cqe_status(enum roq_wc_status roq_status)
{
	if (roq_status == ROQ_WC_SUCCESS)
		return IB_WC_SUCCESS;
	if (roq_status == ROQ_WC_LOC_LEN_ERR)
		return IB_WC_LOC_LEN_ERR;
	if (roq_status == ROQ_WC_LOC_PROT_ERR)
		return IB_WC_LOC_PROT_ERR;
	if (roq_status == ROQ_WC_WR_FLUSH_ERR)
		return IB_WC_WR_FLUSH_ERR;
	if (roq_status == ROQ_WC_BAD_RESP_ERR)
		return IB_WC_BAD_RESP_ERR;
	if (roq_status == ROQ_WC_LOC_ACCESS_ERR)
		return IB_WC_LOC_ACCESS_ERR;
	if (roq_status == ROQ_WC_REM_ACCESS_ERR)
		return IB_WC_REM_ACCESS_ERR;
	if (roq_status == ROQ_WC_GENERAL_ERR)
		return IB_WC_GENERAL_ERR;

	return IB_WC_GENERAL_ERR;
}

static inline enum ib_wc_flags map_cqe_flags(enum roq_cqe_flags roq_flags)
{
	enum ib_wc_flags flags = 0;

	if (roq_flags & ROQ_WC_WITH_IMM)
		flags = IB_WC_WITH_IMM;

	return flags;
}

static struct {
	enum roq_opcode	  roq;
	enum ib_wc_opcode ofa;
} map_cqe_opcode [ROQ_NUM_OPCODES] = {
	{ROQ_OP_WRITE,		IB_WC_RDMA_WRITE},
	{ROQ_OP_READ,		IB_WC_RDMA_READ},
	{ROQ_OP_SEND,		IB_WC_SEND},
	{ROQ_OP_RECEIVE,	IB_WC_RECV}
};

static void copy_cqe(struct roq_cqe *cqe, struct ib_wc *wc,
		     struct roq_cq *roq_cq)
{
	u32 qp_id = cqe->qp_num;

	memset(wc, 0, sizeof *wc);

	wc->wr_id = cqe->id;
	wc->byte_len = cqe->bytes;

	wc->status = map_cqe_status(cqe->status);
	wc->opcode = map_cqe_opcode[cqe->opcode].ofa;
	wc->wc_flags = map_cqe_flags(cqe->flags);

	if (qp_id < ROQ_MAX_QP) {
		/*
		 * The user may maliciously write into the CQE, so
		 * do some correctness check for the QP ID given
		 */
		struct roq_qp *dd_qp = roq_device->RoQ->QP_Array[qp_id].dd_qp;

		if (likely((dd_qp != NULL) &&
		    (&roq_cq->ofa_cq == dd_qp->init_attrs.send_cq ||
                     &roq_cq->ofa_cq == dd_qp->init_attrs.recv_cq)))
			wc->qp = &dd_qp->ofa_qp;
		else
			pr_warn("CQE: invalid QP number %d\n", cqe->qp_num);
	}
}

int
roq_poll_cq(struct ib_cq *ofa_cq, int num_entries, struct ib_wc *ofa_wc)
{
	struct roq_cqe		*new_wc;
	struct roq_cq		*dd_cq = roq_cq_ofa2roq(ofa_cq);
	struct roq_dm_cq	*dm_cq = &dd_cq->dm_cq;
	struct roq_cqe		*cqe = dm_cq->cq_virt;
	int i;

	spin_lock(&dd_cq->lock);

	for (i = 0; i < num_entries; i++) {
		volatile u64 cq_index =
			*(volatile u64 *)&dm_cq->DmQueue.mGot;
		new_wc = &(cqe[cq_index % dm_cq->DmQueue.mItemCount]);

		if (*(volatile int *)&new_wc->flags & ROQ_WC_FREE)
			break;

		copy_cqe(new_wc, &ofa_wc[i], dd_cq);

		*(volatile int *)&new_wc->flags = ROQ_WC_FREE;

		dm_cq->DmQueue.mGot++;

		dprint_tsk(DBG_EH, KERN_INFO,
			   "CQ@ 0x%p %d/%d wc@ 0x%p new_wc 0x%p ->wr_id %lld "
			   "->status %d ->opcode %d qp@ 0x%p Q.G/P/C %lld / "
			   "%lld / %lld",
			   dd_cq, i, num_entries, ofa_wc, new_wc,
			   ofa_wc->wr_id, ofa_wc->status, ofa_wc->opcode,
			   ofa_wc->qp, dm_cq->DmQueue.mGot,
			   dm_cq->DmQueue.mPut, dm_cq->DmQueue.mItemCount);
	}
	spin_unlock(&dd_cq->lock);

	return i;
}

int
roq_req_notify_cq(struct ib_cq *ofa_cq, enum ib_cq_notify_flags reason)
{
	struct roq_cq *dd_cq	= roq_cq_ofa2roq(ofa_cq);
	struct roq_dm_cq *dm_cq = &dd_cq->dm_cq;

	int rv = 0;

	dprint_tsk(DBG_EH, KERN_INFO, "(CQ%p:) reason: %d", dd_cq, reason);

	switch (reason) {

	case IB_CQ_SOLICITED:
		*(volatile enum roq_notify_flags *)&dm_cq->notify =
			ROQ_NOTIFY_SOLICITED;
		break;

	case IB_CQ_NEXT_COMP:
		*(volatile enum roq_notify_flags *)&dm_cq->notify =
			ROQ_NOTIFY_NEXT_COMPLETION;

		break;

	default:
		rv = -EINVAL;
	}
	smp_wmb();

	return rv;
}

/*
 * roq_dereg_mr()
 *
 * release memory region.
 * TODO: memory window management: we still have to understand
 * if ofed core is checking for mw dependencies for current
 * mr. for now we assume so and are freeing mr w/o any
 * further checks of attached mw's
 */
int
roq_dereg_mr(struct ib_mr *ib_mr)
{
	struct roq_dev	*dev = (struct roq_dev *)ib_mr->device;
	struct roq_mr	*dd_mr = container_of(ib_mr, struct roq_mr, ofa_mr);
	struct roq_dm_mr *dm_mr = dd_mr->dm_mr;

	if (dm_mr) {
		int report_wait = 1;
		out_be64((volatile u64 *)&dm_mr->dd_mr, 0);
		// dm_mr->dd_mr = NULL;
		smp_wmb();
		while (dm_mr->inuse_tx || dm_mr->inuse_rx) {
			if (report_wait == 1) {
				pr_info("dereg_mr: wait for DM (%u:%u)\n",
					dm_mr->inuse_tx, dm_mr->inuse_rx);
				report_wait = 2;
			}
			schedule();
		}
		if (report_wait == 2) 
			pr_info("dereg_mr: DM done\n");

		roq_kfree((void *)dm_mr->mHostPageArrayPtr);
		out_be32(&dm_mr->mVersionNumber, dm_mr->mVersionNumber - 1);
	}
	if (dd_mr->umem)
		ib_umem_release(dd_mr->umem);

	atomic_dec(&dev->num_mem);

	kfree(dd_mr);

	return 0;
}

static struct roq_mr *
roq_alloc_mr(struct roq_dev *dev, struct ib_umem *umem, uint64_t start,
	     uint64_t len, int rights)
{
	struct RoQ_DeviceMemory	*dev_mem = roq_dev2dm_ctrl(dev);
	struct roq_dm_mr	*dm_mr;
	struct roq_mr		*dd_mr;
	uint32_t NewMrIndex, AlignByBits, AlignByMask;
	static int index = 1;
	int i;

	if (atomic_read(&dev->num_mem) >= RoQ_MAX_MR) {
		/*
		 * TODO: this allows no reuse -- run out and you're done
		 * for this driver instantiation.
		 */
		pr_info("Out of MR's: %d", atomic_read(&dev->num_mem));
		return (struct roq_mr *)ERR_PTR(-ENOMEM);
	}
	dd_mr = kzalloc(sizeof *dd_mr, GFP_KERNEL);
	if (!dd_mr) {
		pr_info("Cannot malloc MR\n");
		return (struct roq_mr *)ERR_PTR(-ENOMEM);
	}
	if (!umem) {
		atomic_inc(&dev->num_mem);
		dd_mr->ofa_mr.device = (struct ib_device *)dev;
		return dd_mr;
	}
	spin_lock_bh(&dev->mr_lock);

	/* find free MR array entry */
	for (i = 0; i < RoQ_MAX_MR; i++) {
		NewMrIndex = index;
		dm_mr = &dev_mem->mMR_Array[index++];
		/*
		 * The MR is in use if mVersion number
		 * is not even.
		 */
		if (index == RoQ_MAX_MR)
			index = 1;

		if ((dm_mr->mVersionNumber & 0x01) == 0)
			break;
	}
	if (i == RoQ_MAX_MR) {
		spin_unlock_bh(&dev->mr_lock);
		kfree(dd_mr);
		return (struct roq_mr *)ERR_PTR(-ENOMEM);
	}

	memset(dm_mr, 0, sizeof *dm_mr);
	dd_mr->dm_mr = dm_mr;

	/*
	 * temporary setting to make sure another process entering this
	 * routine won't find the same entry to be free
	 * Increment the device memory mr's version number making it an even
	 * value and thus active
	 */
	out_be32(&dm_mr->mVersionNumber, 1);

	spin_unlock_bh(&dev->mr_lock);

	atomic_inc(&dev->num_mem);

	dprint(DBG_ON, KERN_INFO, "Preparing MR Index %d", NewMrIndex);
	/*
	 * keys are the index of the memory region -- NEED: a better
	 * way to give out and reuse keys shift 8 per ofa agreement
	 * to give lower 8 bits to user
	 */
	AlignByBits = 4;
	AlignByMask = ((1 << AlignByBits) - 1);

	dd_mr->ofa_mr.rkey = ((NewMrIndex << AlignByBits)
				| (start & AlignByMask)) << 8;
	dd_mr->ofa_mr.lkey = dd_mr->ofa_mr.rkey;
	dd_mr->umem = umem;
	dd_mr->ofa_mr.device = (struct ib_device*)dev;
	dm_mr->pagesize = PAGE_SIZE;
	dm_mr->mSize = len;
	dm_mr->mRights = rights;

	return dd_mr;
}

struct ib_mr *
roq_get_dma_mr(struct ib_pd *ib_pd, int rights)
{
	struct roq_dev	*dev = (struct roq_dev *)ib_pd->device;
	struct roq_mr	*mr = roq_alloc_mr(dev, NULL, 0, 0, rights);

	if (IS_ERR(mr))
		return (struct ib_mr *)mr;

	mr->ofa_mr.pd = ib_pd;
	mr->ofa_mr.lkey = mr->ofa_mr.lkey = DMA_MR_KEY;

	return &mr->ofa_mr;
}

struct ib_mr *
roq_reg_user_mr(struct ib_pd *ofa_pd, u64 start, u64 len, u64 rnic_va,
                    int rights, struct ib_udata *udata)
{
	struct ib_umem		*umem;
	struct roq_ureq_reg_mr	ureq;
	struct roq_uresp_reg_mr	uresp;
	struct roq_dev		*dev = (struct roq_dev *)ofa_pd->device;
	struct roq_mr		*mr = NULL;
	struct ib_umem_chunk	*chunk;
	dma_addr_t		PageArrayDmaAddr;
	uint64_t		*PageArrayPtr;
	struct scatterlist	*sl;
	int shift, rv, pp_array_size = 0, i, j, k, len2;

	dprint(DBG_ON, KERN_INFO,
		"Entered pd@ %p start 0x%016llX len %lld rnic_va 0x%016llX "
		"rights 0x%08X", ofa_pd, start, len, rnic_va, rights);

	if (!len)
		return (struct ib_mr *)ERR_PTR(-EINVAL);

	umem = ib_umem_get(ofa_pd->uobject->context, start, len, rights, 0);

	if (IS_ERR(umem)) {
		dprint_tsk(DBG_MM, KERN_INFO,
			   " ib_umem_get failed:%ld LOCKED:%lu, LIMIT:%lu",
			   PTR_ERR(umem), current->mm->locked_vm,
			   current->signal->rlim[RLIMIT_MEMLOCK].rlim_cur >>
			   PAGE_SHIFT);

		return (struct ib_mr *)ERR_PTR(PTR_ERR(umem));
	}

	mr = roq_alloc_mr(dev, umem, start, len, rights);
	if (IS_ERR(mr))
		return (struct ib_mr *)mr;

	if (udata) {
		rv = ib_copy_from_udata(&ureq, udata, sizeof ureq);
		if (rv)
			goto err_out;

		mr->ofa_mr.lkey |= (0x000000FF & ureq.stag_key);
		mr->ofa_mr.rkey |= (0x000000FF & ureq.stag_key);

		uresp.stag = mr->ofa_mr.lkey;

		rv = ib_copy_to_udata(udata, &uresp, sizeof uresp);
		if (rv)
			goto err_out;
	}

	dprint(DBG_ON, KERN_INFO, "Preparing mr@ 0x%p lkey 0x%08X rkey 0x%8X",
	       (void *)mr, mr->ofa_mr.lkey, mr->ofa_mr.rkey);
	dprint(DBG_ON, KERN_INFO,
		"called ib_umem_get(), got ib_mem@ 0x%p ->length %ld "
		"->offset %d ->page_size %d",
		umem, umem->length, umem->offset, umem->page_size);

	/* begin filling out MR for */
	shift = ffs(mr->umem->page_size) - 1;

	pp_array_size = ib_umem_page_count(mr->umem) * sizeof(uint64_t);

	PageArrayPtr = roq_kmalloc(pp_array_size, &PageArrayDmaAddr);
	if (!PageArrayPtr) {
		pr_warn("dma_alloc_coherent() failed: bytes requested %d\n",
			pp_array_size);
		rv = -ENOMEM;
		goto err_out;
	}

	i = 0;
	list_for_each_entry(chunk, &mr->umem->chunk_list, list) {
		dprint(DBG_ON, KERN_INFO,
			"chunk info: chunk@ 0x%p ->nnets %d ->nmap %d "
			"(pages so far %d)",
			chunk, chunk->nents, chunk->nmap, i);

		for (j = 0; j < chunk->nmap; ++j) {
			sl = &chunk->page_list[j];

			len2 = sg_dma_len(&chunk->page_list[j]) >> shift;
			for (k = 0; k < len2; ++k) {
				PageArrayPtr[i++] =
					chunk->page_list[j].dma_address;
			}
		}
	}

	mr->dm_mr->mHostPageArrayPtr = (uint64_t)PageArrayPtr;
	mr->dm_mr->mHostPageArrayDmaAddr = (uint64_t)PageArrayDmaAddr;
	mr->dm_mr->pagesize = umem->page_size;
	mr->dm_mr->mBaseVirtualAddress = rnic_va;
	mr->dm_mr->lkey = mr->ofa_mr.lkey;
	mr->dm_mr->rkey = mr->ofa_mr.rkey;
	mr->dm_mr->dd_mr = mr;

	mr->ofa_mr.pd = ofa_pd;

	dprint(DBG_ON, KERN_INFO,
		"mr created DmaAddr 0x%p mr@ 0x%p ->mBaseVA 0x%016llX "
		"->mSize %lld ->ofa_mr.lkey 0x%08X DMA start 0x%016llX "
		"npages %d",
		PageArrayDmaAddr, mr, mr->dm_mr->mBaseVirtualAddress,
		mr->dm_mr->mSize, mr->ofa_mr.lkey, PageArrayPtr[0],
		pp_array_size / sizeof(uint64_t));

	return &mr->ofa_mr;

err_out:
	roq_dereg_mr(&mr->ofa_mr);
	return (struct ib_mr *)ERR_PTR(rv);
}

struct ib_srq *
roq_create_srq(struct ib_pd *ofa_pd, struct ib_srq_init_attr *init_attrs,
	       struct ib_udata *udata)
{
	struct roq_srq		*srq;
	struct ib_srq_attr	*attrs = &init_attrs->attr;
	struct roq_dev		*dev = (struct roq_dev *)ofa_pd->device;
	dma_addr_t		srq_dma_addr;
	int ret;

	if (attrs->max_wr > ROQ_MAX_SRQ_WR || attrs->max_sge > ROQ_MAX_SGE ||
	    attrs->srq_limit > attrs->max_wr) {
		dprint(DBG_ON, KERN_INFO, " Out of max value: "
		"%d::%d, %d::%d, %d::%d\n",
		attrs->max_wr, ROQ_MAX_SRQ_WR,
		attrs->max_sge, ROQ_MAX_SGE,
		attrs->srq_limit, attrs->max_wr);
		return (struct ib_srq *)ERR_PTR(-EINVAL);
	}

	if (atomic_read(&dev->num_srq) >= ROQ_MAX_SRQ) {
		dprint(DBG_ON, KERN_INFO, " Out of SRQ's\n");
		return (struct ib_srq *)ERR_PTR(-ENOMEM);
	}

	srq = roq_kmalloc(sizeof *srq, &srq_dma_addr);
	if (!srq) {
		dprint(DBG_ON, KERN_INFO, " malloc\n");
		return (struct ib_srq *)ERR_PTR(-ENOMEM);
	}
	memset(srq, 0, sizeof *srq);
	srq->srq_phys = srq_dma_addr;

	if (attrs->max_wr) {
		srq->max_wr = attrs->max_wr;
		srq->rq_alloc_size = attrs->max_wr * sizeof(struct roq_rqe);

		srq->rq_virt = roq_kmalloc(srq->rq_alloc_size, &srq_dma_addr);
		if (!srq->rq_virt) {
			pr_warn("SRQ WQ allocation failed\n");
			ret = -ENOMEM;
			goto fail;
		}
		memset(srq->rq_virt, 0, srq->rq_alloc_size);
		srq->rq_phys = srq_dma_addr;

		KISS_SRSWQ_Init(&srq->DmQueue, sizeof(struct roq_rqe),
				attrs->max_wr, srq_dma_addr);
	} else {
		dprint(DBG_OBJ, KERN_INFO, "SRQ: RecvQueue has 0 length");
	}

	srq->max_sge = attrs->max_sge;
	srq->limit = attrs->srq_limit;

	if (srq->limit)
		srq->armed = 1;

	srq->ofa_pd = ofa_pd;

	spin_lock_init(&srq->lock);
	atomic_inc(&dev->num_srq);

	return &srq->ofa_srq;
fail:
	roq_kfree(srq);

	return (struct ib_srq *)ERR_PTR(ret);
}

int
roq_modify_srq(struct ib_srq *ofa_srq, struct ib_srq_attr *attrs,
		   enum ib_srq_attr_mask attr_mask, struct ib_udata *udata)
{
	struct roq_srq *srq = container_of(ofa_srq, struct roq_srq, ofa_srq);
	int rv = 0;

	spin_lock_bh(&srq->lock);

	if (attr_mask & IB_SRQ_MAX_WR) {
		/* resize request */
		if (attrs->max_wr > ROQ_MAX_SRQ_WR) {
			rv =  -EINVAL;
			goto out;
		}

		if (attrs->max_wr < srq->max_wr) { /* shrink */
			if (attrs->max_wr < srq->max_wr -
				(srq->DmQueue.mPut - srq->DmQueue.mGot)) {
				rv = -EBUSY;
				goto out;
			}
			srq->DmQueue.mItemCount -= srq->max_wr - attrs->max_wr;
		} else { /* grow */
			srq->DmQueue.mItemCount += attrs->max_wr - srq->max_wr;
			srq->max_wr = attrs->max_wr;
		}
	}

	if (attr_mask & IB_SRQ_LIMIT) {
		if (attrs->srq_limit) {
			if (attrs->srq_limit > srq->max_wr) {
				rv = -EINVAL;
				/* FIXME: restore old space & max_wr?? */
				goto out;
			}
			srq->armed = 1;
		} else {
			srq->armed = 0;
		}
		srq->limit = attrs->srq_limit;
	}

out:
	spin_unlock_bh(&srq->lock);
	return rv;
}

int
roq_query_srq(struct ib_srq *ofa_srq, struct ib_srq_attr *attrs)
{
	struct roq_srq *srq = container_of(ofa_srq, struct roq_srq, ofa_srq);

	spin_lock_bh(&srq->lock);

	attrs->max_wr = srq->max_wr;
	attrs->max_sge = srq->max_sge;
	attrs->srq_limit = srq->limit;

	spin_unlock_bh(&srq->lock);

	return 0;
}

int
roq_destroy_srq(struct ib_srq *ofa_srq)
{
	struct roq_srq *srq = container_of(ofa_srq, struct roq_srq, ofa_srq);
	struct roq_dev *dev = (struct roq_dev *)srq->ofa_pd->device;

	roq_kfree(srq->rq_virt);
	roq_kfree(srq);

	atomic_dec(&dev->num_srq);

	return 0;
}

int
roq_post_srq_recv(struct ib_srq *ofa_srq, struct ib_recv_wr *wr,
		      struct ib_recv_wr **bad_wr)
{
	struct roq_srq *srq = container_of(ofa_srq, struct roq_srq, ofa_srq);
	struct roq_rqe *rqe;
	uint64_t rqe_index;
	int rv = 0;

	while (wr) {
		spin_lock_bh(&srq->lock);

		/* check for space in srq */
		if (srq->DmQueue.mPut - srq->DmQueue.mGot >
		    srq->DmQueue.mItemCount) {

			spin_unlock_bh(&srq->lock);
			rv = -ENOMEM;
			break;
		}

		if (wr->num_sge > srq->max_sge) {
			dprint_tsk(DBG_WR | DBG_ON, KERN_INFO,
				   "(SRQ): Receive has too many SGEs: %d",
				   wr->num_sge);
			rv = -EINVAL;
			break;
		}
		dprint(DBG_WR, KERN_INFO,
		       "(SRQ): wr@ 0x%p ->num_sge %d wr->next 0x%p",
		       wr, wr->num_sge, wr->next);


		rqe_index = srq->DmQueue.mPut % srq->DmQueue.mItemCount;
		rqe = &srq->rq_virt[rqe_index];

		/*
		 * Transfer OFA WR into ROQ private RQE representation
		 */
		memset(rqe, 0, sizeof *rqe);

		/*
		 *  This assumes OFA sg element to have same layout as
		 *  struct roq_sge.
		 */
		memcpy(rqe->sge, wr->sg_list,
		       sizeof(struct roq_sge) * wr->num_sge);

		rqe->id = wr->wr_id;
		rqe->num_sge = wr->num_sge;
		rqe->flags = ROQ_WQE_VALID;

		/* update WQ srsw counter */
		srq->DmQueue.mPut++;

		spin_unlock_bh(&srq->lock);

		wr = wr->next;
	}
	return rv > 0 ? 0 : rv;
}
