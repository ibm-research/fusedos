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

#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/syscalls.h>
#include <linux/kmod.h>

#include <linux/in.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>

#include <linux/version.h>

#include <linux/in6.h>
#include <asm/checksum.h>

#include <firmware/include/personality.h>
#include <firmware/include/Firmware_Interface.h>
#ifdef __FUSEDOS__
int bluegene_getPersonality(Personality_t*, size_t);
#else
#include <asm/bluegene.h>
#endif

#include "roq.h"
#include "roq_cm.h"

#define DRV_NAME_EXPLICIT "ROQ Ethernet device driver"
#define DRV_NAME          "roq_eth"

#define ROQ_ETH_DEBUG		1
#define PROC_DEBUG_ENTRY	1

#ifdef ROQ_ETH_DEBUG
#define PDEBUG(fmt, args...) printk(KERN_EMERG DRV_NAME": " fmt, ## args)
#else
#define PDEBUG(fmt, args...)
#endif

#define MAX_TX_SKBS (8*1024)
#define MAX_RX_SKBS (8*1024)
#define MAX_WC	16
#define DEFAULT_SKB_SIZE (65536 - 128)

#define INVALID_TX_SLOT (MAX_TX_SKBS + 1)


struct roq_eth_post_send {
	struct sk_buff *skb;
	uint64_t addr;
	uint32_t len;
};

struct roq_eth_tx_buffer {
	struct roq_eth_post_send post[MAX_TX_SKBS];
	atomic_t inuse;
	int head;
};

struct roq_eth_post_recv_t {
	struct sk_buff *skb;
	uint64_t addr;
};

struct roq_eth_rx_buffer {
	struct ib_sge recv_sgl;
	struct ib_recv_wr rq_wr;
	struct roq_eth_post_recv_t post[MAX_RX_SKBS];
};

struct roq_eth_priv {
	struct net_device_stats stats;
	spinlock_t lock;
	spinlock_t tx_lock;
	struct net_device *ndev;
	struct napi_struct napi;
	Personality_t personality;
	int part_size;
	char gw_dest[3];
	int rem_part_size;
	int fix_rem;
	int is_ionode;
	struct RoQ_NetworkDescriptor *netdesc;
	struct RoQ_NetworkDescriptor *netdesc_rem;

	struct roq_eth_tx_buffer tx_buf;
	struct roq_eth_rx_buffer rx_buf;

	struct ib_device *ibdev;

	struct ib_pd     *kpd;
	struct ib_qp     **qps;
	struct ib_qp     **qps_rem;
	struct ib_cq     *recv_cq;
	struct ib_cq     *send_cq;
	struct ib_srq    *srq;

#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
	struct proc_dir_entry *proc_dir, *txfifo_file;
#endif
};

#define MAX_TX_SEGS 4
union tx_slot_desc {
	u16 slot[MAX_TX_SEGS];
	u64 id;
};

typedef union tx_slot_desc tx_slot_desc_t;

static void roq_eth_add_one(struct ib_device *);
static void roq_eth_remove_one(struct ib_device *);
void roq_eth_init(struct net_device *, struct RoQ_DeviceMemory *);
void roq_eth_cleanup(struct net_device *);

/*
 * BlueG specific functions
 */
static inline int32_t
roq_tcoords_to_rank(struct RoQ_NetworkDescriptor *netdesc, uint32_t a,
		    uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
    /* match CNK's default map order ABCDET */
    /* rank = E + dim_E ( D + dim_D ( C + dim_C ( B + dim_B * A ))) */
	uint32_t rank = 0;
	int part_size = RoQ_NetworkSize(netdesc);

	rank = a * netdesc->Bnodes + b;
	rank = rank * netdesc->Cnodes + c;
	rank = rank * netdesc->Dnodes + d;
	rank = rank * netdesc->Enodes + e;

	if (rank >= part_size)
		printk(DRV_NAME": ERROR rank calculation "
		       "a=%d,b=%d,c=%d,d=%d,e=%d => %d %d\n",
		       a, b, c, d, e, rank, part_size);

	return rank >= part_size ? -EINVAL : rank;
}

/*
 * ethernet / infiniband stack specific functions
 */

#define SKB_SLOT_INUSE	(struct sk_buff *)-1

static inline int
roq_put_tx_slots(struct roq_eth_priv *priv, tx_slot_desc_t *slot_desc, int num)
{
	struct device *dev = &priv->ndev->dev;
	struct roq_eth_post_send *tx_desc =
		&priv->tx_buf.post[slot_desc->slot[0]];
	int i = 1;

	dma_unmap_single(dev, tx_desc->len, tx_desc->addr, DMA_TO_DEVICE);

	wmb();
	tx_desc->skb = NULL;

	while (--num && slot_desc->slot[i] != INVALID_TX_SLOT) {
		tx_desc = &priv->tx_buf.post[slot_desc->slot[i]];

		dma_unmap_page(dev, tx_desc->addr, tx_desc->len,
			       DMA_TO_DEVICE);
		wmb();
		tx_desc->skb = NULL;
		i++;
	}
	return i;
}

static inline void
roq_get_tx_slots(struct roq_eth_priv *priv, tx_slot_desc_t *slot_desc, int num)
{
	int i = 0;

	spin_lock(&priv->tx_lock);

	do {
		while (priv->tx_buf.post[priv->tx_buf.head].skb != NULL)
			priv->tx_buf.head =
				(priv->tx_buf.head + 1) % MAX_TX_SKBS;

		slot_desc->slot[i++] = priv->tx_buf.head;
		/* Mark the slot as in use */
		priv->tx_buf.post[priv->tx_buf.head].skb = SKB_SLOT_INUSE;
		num--;

	} while (num);

	spin_unlock(&priv->tx_lock);

	if (i < MAX_TX_SEGS)
		slot_desc->slot[i] = INVALID_TX_SLOT;
}

static int
roq_eth_post_recv(struct roq_eth_priv *vdev, int idx)
{
	struct roq_eth_rx_buffer	*rx_buf = &vdev->rx_buf;
	struct ib_recv_wr		*bad_wr;
	int ret;

	spin_lock(&vdev->lock);

	rx_buf->recv_sgl.addr = rx_buf->post[idx].addr;
	rx_buf->recv_sgl.length = DEFAULT_SKB_SIZE;
	rx_buf->recv_sgl.lkey = 0;
	rx_buf->rq_wr.wr_id = idx;
	rx_buf->rq_wr.sg_list = &rx_buf->recv_sgl;
	rx_buf->rq_wr.num_sge = 1;
	rx_buf->rq_wr.next = NULL;

	ret = ib_post_srq_recv(vdev->srq, &rx_buf->rq_wr, &bad_wr);

	spin_unlock(&vdev->lock);

	return ret;
}

void
roq_eth_rx_ib_compl(struct ib_cq *cq, void *dev_ptr)
{
	struct net_device *dev = dev_ptr;
	struct roq_eth_priv *vdev = netdev_priv(dev);

	napi_schedule(&vdev->napi);
}

static void print_tx_ring(struct roq_eth_tx_buffer *tx_buf)
{
	char buf[512], *bufp;
	int i, num = 0;

	memset(buf, 0, 512);
	bufp = buf;

	for (i = 0; i < MAX_TX_SKBS; i++) {
		bufp += sprintf(bufp, "[%04d:%s]",
				i, tx_buf->post[i].skb ? "*" : " ");
		if (++num == 20) {
			printk("%s\n", buf);
			memset(buf, 0, 512);
			bufp = buf;
			num = 0;
		}
	}
	if (num)
		printk("%s\n", buf);
}

static inline int
poll_tx(struct roq_eth_priv *priv)
{
	struct roq_eth_tx_buffer *tx_buf = &priv->tx_buf;
	unsigned long flags;
	int i, num_cqe,  polled = 0, polled_last = 0, start = 1;

	spin_lock_irqsave(&priv->tx_lock, flags);
poll_again:
	do {
		struct ib_wc wc[MAX_WC];

		num_cqe = ib_poll_cq(priv->send_cq, MAX_WC, wc);
		if (unlikely(num_cqe < 0)) {
			pr_warn("poll_tx: CQ polling failed: %d", num_cqe);
			goto out;
		}
		polled += num_cqe;

		for (i = 0; i < num_cqe; i++) {
			struct sk_buff *skb;
			tx_slot_desc_t desc;
			int num_slots;

			desc.id = wc[i].wr_id;

			skb = tx_buf->post[desc.slot[0]].skb;

			if (unlikely(!skb || skb == SKB_SLOT_INUSE)) {
				print_tx_ring(tx_buf);
				BUG_ON(1);
			}
			num_slots = roq_put_tx_slots(priv, &desc, MAX_TX_SEGS);

			priv->stats.tx_packets++;
			priv->stats.tx_bytes += skb->len;

			if (0 && num_slots > 1)
				pr_info("put %d frags for 0x%p\n",
					num_slots - 1, skb);

			dev_kfree_skb_any(skb);

			atomic_sub(num_slots, &tx_buf->inuse);
		}
	} while (num_cqe);

	if (start || polled_last < polled) {
		if (unlikely(ib_req_notify_cq(priv->send_cq,
					      IB_CQ_NEXT_COMP) < 0)) {
			pr_info("poll_tx: cq re-arm error\n");
			goto out;
		}
		start = 0;
		polled_last = polled;
		goto poll_again;
	}
out:
	spin_unlock_irqrestore(&priv->tx_lock, flags);
	return polled;
}

void
roq_eth_tx_ib_compl(struct ib_cq *cq, void *dev_ptr)
{
	struct net_device *ndev = dev_ptr;
	struct roq_eth_priv *priv = netdev_priv(ndev);

	if (poll_tx(priv))
		netif_wake_queue(ndev);
}

static void
roq_eth_cleanup_ofa(struct roq_eth_priv *vdev)
{
	int i;

	if (vdev->send_cq)
		ib_destroy_cq(vdev->send_cq);
	if (vdev->recv_cq && vdev->recv_cq != vdev->send_cq)
		ib_destroy_cq(vdev->recv_cq);

	if (vdev->qps) {
		for (i = 0; i < vdev->part_size; i++)
			if (vdev->qps[i])
				ib_destroy_qp(vdev->qps[i]);
		kfree(vdev->qps);
	}
	if (vdev->qps_rem) {
		for (i = 0; i < vdev->rem_part_size; i++)
			if (vdev->qps_rem[i])
				ib_destroy_qp(vdev->qps_rem[i]);
		kfree(vdev->qps_rem);
	}
	if (vdev->kpd)
		ib_dealloc_pd(vdev->kpd);

	vdev->qps = vdev->qps_rem = NULL;
	vdev->recv_cq = vdev->send_cq = NULL;

	vdev->kpd = NULL;

	return;
}

static void
roq_free_rx_buffers(struct roq_eth_priv *priv)
{
	struct roq_eth_rx_buffer *buf = &priv->rx_buf;
	struct device *dev = &priv->ndev->dev;
	int i;

	for (i = 0; i < MAX_RX_SKBS; i++) {
		if (buf->post[i].addr)
			dma_unmap_single(dev,
					 DEFAULT_SKB_SIZE,
					 buf->post[i].addr,
					 DMA_FROM_DEVICE);
		if (buf->post[i].skb)
			dev_kfree_skb(buf->post[i].skb);
	}
	memset(buf, 0, sizeof *buf);
}

static int
roq_alloc_rx_buffers(struct roq_eth_priv *priv)
{
	struct device *dev = &priv->ndev->dev;
	int i;

	memset(&priv->rx_buf, 0, sizeof(struct roq_eth_rx_buffer));

	/* alloc socket buffers */
	for (i = 0; i < MAX_RX_SKBS; i++) {
		priv->rx_buf.post[i].skb =
			netdev_alloc_skb_ip_align(priv->ndev,
						  DEFAULT_SKB_SIZE);
		if (!priv->rx_buf.post[i].skb) {
			pr_warn("Failed to alloc RX SKB Data Buffer %d\n", i);
			goto err;
		}
		priv->rx_buf.post[i].addr =
			dma_map_single(dev, priv->rx_buf.post[i].skb->data,
				       DEFAULT_SKB_SIZE, DMA_FROM_DEVICE);

		if (!priv->rx_buf.post[i].addr) {
			dev_kfree_skb(priv->rx_buf.post[i].skb);
			pr_warn("Mapping of RX SKB Data Buffer %d failed\n", i);
			goto err;
		}
	}

	return 0;
err:
	roq_free_rx_buffers(priv);
	return -ENOMEM;
}

int
roq_eth_rem_init_qp(struct net_device *ndev)
{
	struct roq_eth_priv	*vdev = netdev_priv(ndev);
	struct ib_qp_init_attr	create_qp_attrs;
	struct ib_qp_attr	qp_attr;
	enum ib_qp_attr_mask	qp_attr_mask;
	char *argv[] = {"/etc/init.d/post_discovery", NULL};
	char *env[] = {	"HOME=/",
			"TERM=linux",
			"PATH=/sbin:/usr/sbin:/bin:/usr/bin",
			"LD_LIBRARY_PATH=/lib:/usr/lib", NULL};
	int i, rank, size, ret = 0;

	if (vdev->send_cq == NULL || vdev->recv_cq == NULL ||
	    vdev->srq == NULL || vdev->kpd == NULL) {
		vdev->fix_rem = 1;
		pr_warn("roq_eth_rem_init: return w/o discovery\n");
		return 0;
	}


	/* clean old remote qps */
	if (vdev->rem_part_size) {
		for (i = 0; i < vdev->rem_part_size; i++)
			ib_destroy_qp(vdev->qps_rem[i]);
		kfree(vdev->qps_rem);
	}
	vdev->rem_part_size = RoQ_NetworkSize(vdev->netdesc_rem);

	rank = roq_tcoords_to_rank(vdev->netdesc,
			vdev->personality.Network_Config.Acoord,
			vdev->personality.Network_Config.Bcoord,
			vdev->personality.Network_Config.Ccoord,
			vdev->personality.Network_Config.Dcoord,
			vdev->personality.Network_Config.Ecoord);
	if (IS_ERR_VALUE(rank)) {
		ret = -EINVAL;
		pr_warn("roq_eth_rem_init: invalid rank\n");
		goto out;
	}

	memset(&create_qp_attrs, 0, sizeof(struct ib_qp_init_attr));
	create_qp_attrs.send_cq = vdev->send_cq;
	create_qp_attrs.recv_cq = vdev->recv_cq;

	/* set some more parameters */
	create_qp_attrs.qp_type       = IB_QPT_UD;
	create_qp_attrs.event_handler = NULL;
	create_qp_attrs.qp_context    = NULL;
	create_qp_attrs.srq           = vdev->srq;

	create_qp_attrs.cap.max_send_wr      = MAX_TX_SKBS;
	create_qp_attrs.cap.max_recv_wr      = 1;
	create_qp_attrs.cap.max_send_sge     = 1;
	create_qp_attrs.cap.max_recv_sge     = 1;
	create_qp_attrs.cap.max_inline_data  = 0;

	size = sizeof(struct ib_qp *) * vdev->rem_part_size;

	vdev->qps_rem = (struct ib_qp **)kmalloc(size, GFP_KERNEL);
	if (!vdev->qps_rem) {
		pr_warn("roq_eth_rem_init_qp: remote QP alloc failed");
		ret = -ENOMEM;
		goto out;
	}

	for (i = 0; i < vdev->rem_part_size; i++) {
		vdev->qps_rem[i] = ib_create_qp(vdev->kpd, &create_qp_attrs);
		if (IS_ERR(vdev->qps_rem[i])) {
			pr_warn("roq_eth_rem_init_qp: error creating qp %p\n",
				vdev->qps_rem[i]);
			ret = PTR_ERR(vdev->qps_rem[i]);
			goto out;
		}
	}

	for (i = 0; i < vdev->rem_part_size; i++) {
		qp_attr_mask          = 0;
		qp_attr_mask         |= IB_QP_STATE;
		qp_attr.qp_state      = IB_QPS_RTS;
		qp_attr_mask         |= IB_QP_AV;
		/* this QP will send to peer rank i (zero based) */
		qp_attr.ah_attr.dlid = 0x8000 | i;
		qp_attr_mask         |= IB_QP_DEST_QPN;
		/*
		 * this QP will send to peer qp num rank + 1
		 * (QP zero is reserved)
		 */
		qp_attr.dest_qp_num   = rank + 1;

		ib_modify_qp(vdev->qps_rem[i], &qp_attr, qp_attr_mask);
	}

	ret = call_usermodehelper(argv[0], argv, env, UMH_WAIT_EXEC);
out:
	return ret;
}

static int
roq_eth_init_qp(struct net_device *ndev)
{
	struct roq_eth_priv *vdev = netdev_priv(ndev);
	struct ib_qp_init_attr create_qp_attrs;
	struct ib_device *ibdev = vdev->ibdev;
	struct ib_qp_attr qp_attr;
	enum ib_qp_attr_mask qp_attr_mask;
	struct ib_srq_init_attr srq_attr;
	int rank, i, size, ret = 0;

	/* initialize variables */
	memset(&create_qp_attrs, 0, sizeof(struct ib_qp_init_attr));

	rank = roq_tcoords_to_rank(vdev->netdesc,
			vdev->personality.Network_Config.Acoord,
			vdev->personality.Network_Config.Bcoord,
			vdev->personality.Network_Config.Ccoord,
			vdev->personality.Network_Config.Dcoord,
			vdev->personality.Network_Config.Ecoord);
	if (IS_ERR_VALUE(rank)) {
		ret = -EINVAL;
		pr_info("roq_eth_init_qp: invalid rank\n");
		goto out;
	}

	if ((ret = roq_alloc_rx_buffers(vdev)) != 0)
		goto out;

	/* create completion queues */
	vdev->send_cq = ib_create_cq(ibdev, roq_eth_tx_ib_compl, NULL,
	                             vdev->ndev, MAX_TX_SKBS, 0);
	if (IS_ERR(vdev->send_cq)) {
		pr_warn("roq_eth_init_qp: ib_create_cq failed");
		ret = PTR_ERR(vdev->send_cq);
		vdev->send_cq = NULL;
		goto out;
	}
	vdev->recv_cq = ib_create_cq(ibdev, roq_eth_rx_ib_compl, NULL,
	                             vdev->ndev, MAX_RX_SKBS, 0);
	if (IS_ERR(vdev->recv_cq)) {
		pr_warn("roq_eth_init_qp: ib_create_cq failed");
		ret = PTR_ERR(vdev->recv_cq);
		vdev->recv_cq = NULL;
		goto out;
	}
	create_qp_attrs.send_cq = vdev->send_cq;
	create_qp_attrs.recv_cq = vdev->recv_cq;

	/* allocate protection domain and qp array */
	vdev->kpd = ib_alloc_pd(ibdev);
	if (IS_ERR(vdev->kpd)) {
		pr_warn("roq_eth_init_qp: ib_alloc_pd failed");
		ret = PTR_ERR(vdev->kpd);
		vdev->kpd = NULL;
		goto out;
	}

	memset(&srq_attr, 0, sizeof(struct ib_srq_init_attr));
	srq_attr.attr.max_wr  = MAX_RX_SKBS;
	srq_attr.attr.max_sge = 1;
	vdev->srq = ib_create_srq(vdev->kpd, &srq_attr);
	if (IS_ERR(vdev->srq)) {
		pr_warn("roq_eth_init_qp: ib_create_srq failed");
		ret = PTR_ERR(vdev->srq);
		vdev->srq = NULL;
		goto out;
	}

	/* set some more parameters */
	create_qp_attrs.qp_type       = IB_QPT_UD;
	create_qp_attrs.event_handler = NULL;
	create_qp_attrs.qp_context    = NULL;
	create_qp_attrs.srq           = vdev->srq;

	create_qp_attrs.cap.max_send_wr      = MAX_TX_SKBS;
	create_qp_attrs.cap.max_recv_wr      = 1;
	create_qp_attrs.cap.max_send_sge     = 1;
	create_qp_attrs.cap.max_recv_sge     = 1;
	create_qp_attrs.cap.max_inline_data  = 0;

	size = sizeof(struct ib_qp *) * vdev->part_size;
	vdev->qps = kmalloc(size, GFP_KERNEL);
	if (!vdev->qps) {
		pr_warn("roq_eth_init_qp: kmalloc failed\n");
		goto out;
	}

	for (i = 0; i < vdev->part_size; i++) {
		vdev->qps[i] = ib_create_qp(vdev->kpd, &create_qp_attrs);
		if (IS_ERR(vdev->qps[i])) {
			ret =  PTR_ERR(vdev->qps[i]);
			pr_warn("roq_eth_init_qp: ib_create_qp failed: %d",
				ret);
			goto out;
		}
	}

	ret = ib_req_notify_cq(vdev->send_cq, IB_CQ_NEXT_COMP);
	if (ret)
		goto out;
	ret = ib_req_notify_cq(vdev->recv_cq, IB_CQ_NEXT_COMP);
	if (ret)
		goto out;

	for (i = 0; i < vdev->part_size; i++) {
		qp_attr_mask          = 0;
		qp_attr_mask         |= IB_QP_STATE;
		qp_attr.qp_state      = IB_QPS_RTS;
		qp_attr_mask         |= IB_QP_AV;
		/* this QP will send to peer rank i (zero based) */
		qp_attr.ah_attr.dlid = i;

		qp_attr_mask         |= IB_QP_DEST_QPN;
		/*
		 * this QP will send to peer QP num rank + 1
		 * (QP zero is reserved)
		 */
		qp_attr.dest_qp_num   = rank + 1;

		ib_modify_qp(vdev->qps[i], &qp_attr, qp_attr_mask);
	}

	/* SETUP RECEIVE QP */
	for (i = 0; i < MAX_RX_SKBS; i++)
		roq_eth_post_recv(vdev, i);

	if (vdev->fix_rem == 1) {
		roq_eth_rem_init_qp(ndev);
		vdev->fix_rem = 0;
	}

out:
	if (ret) {
		pr_warn("roq_eth_init_qp: rv = %d\n", ret);
		roq_eth_cleanup_ofa(vdev);
	}
	return ret;
}

static int
roq_eth_renew_skb(struct roq_eth_priv *vdev, int skbno)
{
	struct net_device	*ndev = vdev->ndev;
	struct device		*dev = &vdev->ndev->dev;
	struct sk_buff		*skb = NULL;
	dma_addr_t		addr = 0;

	int rv = 0;

	BUG_ON(!vdev->rx_buf.post[skbno].skb);
	BUG_ON(!vdev->rx_buf.post[skbno].addr);

	dma_unmap_single(dev, DEFAULT_SKB_SIZE,
			 vdev->rx_buf.post[skbno].addr, DMA_FROM_DEVICE);

	skb = netdev_alloc_skb_ip_align(ndev, DEFAULT_SKB_SIZE);
	if (unlikely(!skb)) {
		pr_warn("roq_eth_renew_skb: Alloc failed");
		rv = -ENOMEM;
		goto out;
	}
	addr = dma_map_single(dev, skb->data, DEFAULT_SKB_SIZE,
			      DMA_FROM_DEVICE);

	if (unlikely(!addr)) {
		pr_warn("roq_eth_renew_skb: Mapping failed");
		dev_kfree_skb(skb);
		skb = NULL;
		rv = -ENOMEM;
		goto out;
	}
	skb->dev = vdev->ndev;
	skb->ip_summed = CHECKSUM_UNNECESSARY;
out:
	vdev->rx_buf.post[skbno].skb = skb;
	vdev->rx_buf.post[skbno].addr = addr;

	return rv;
}

static void
roq_eth_pass_skb_up(struct roq_eth_priv *vdev, struct sk_buff *skb, int size)
{
	struct sk_buff *skb_shrinked;
	int err;

	skb_put(skb, size);
	skb->dev = vdev->ndev;
	skb->protocol = eth_type_trans(skb, vdev->ndev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;

	vdev->stats.rx_packets++;
	vdev->stats.rx_bytes += skb->len;

	if (size < ETH_DATA_LEN) {
		/*
		 * this call shrinks data buffer to real data length,
		 * otherwise ping doesn't work
		 */ 
		skb_shrinked = skb_copy_expand(skb, skb_headroom(skb),
					       0, GFP_ATOMIC);
		dev_kfree_skb(skb);
	} else
		skb_shrinked = skb;

//printk("%s:%d\n", __func__, __LINE__);

	if (likely(skb_shrinked)) {
		err = netif_receive_skb(skb_shrinked);
		if (unlikely(err)) {
			if (err == NET_RX_DROP)
				pr_warn("netif_receive_skb: dropped: %d : %d\n",
					skb->len, size);
			else
				pr_warn("netif_receive_skb: %d\n", err);
		}
	} else {
		pr_warn("skb_copy_expand: dropped\n");
	}
	return;
}

static int
roq_eth_rx_clean(struct roq_eth_priv *vdev, int budget)
{
	int num_cqe, polled = 0, polled_last = 0, start = 1, i;

poll_again:
	do {
		struct ib_wc wc[MAX_WC];
                int  loop_max_wc = min (MAX_WC, budget);

		num_cqe = ib_poll_cq(vdev->recv_cq, loop_max_wc, wc);
		if (unlikely(IS_ERR_VALUE(num_cqe))) {
			pr_info("roq_eth_rx_clean: cq poll error\n");
			goto out;
		}
//printk("%s:%d %d\n", __func__, __LINE__, num_cqe);
		polled += num_cqe;
		budget -= num_cqe;
		for (i = 0; i < num_cqe; i++) {
			unsigned long idx = wc[i].wr_id;
			int rx_len = wc[i].byte_len;

			if (unlikely(idx > MAX_RX_SKBS)) {
				pr_warn("roq_handle_rx: wr_id out of bounds\n");
				continue;
			}
			if (unlikely(rx_len == 0))
				pr_warn("roq_handle_rx: zero length packet\n");

			if (unlikely(wc[i].qp == NULL)) {
				pr_warn("roq_handle_rx: QP NULL\n");
				continue;
			}
			roq_eth_pass_skb_up(vdev, vdev->rx_buf.post[idx].skb,
					    rx_len);

			vdev->ndev->last_rx = jiffies;

			/* unmap old skb, get new skb and map new skb */
			if (roq_eth_renew_skb(vdev, idx) == 0)
				/* post skb data address to core */
				roq_eth_post_recv(vdev, idx);
			else
				pr_warn("SKB renew failed, idx %lu\n", idx);

		}
	} while (num_cqe && budget);

	if (start || polled_last < polled) {
		if (unlikely(ib_req_notify_cq(vdev->recv_cq,
					      IB_CQ_NEXT_COMP) < 0)) {
			pr_info("roq_eth_rx_clean: cq re-arm error\n");
			goto out;
		}
		start = 0;
		polled_last = polled;
		if (budget) goto poll_again;
	}
out:
	return polled;
}

static int
roq_eth_napi(struct napi_struct *napi, int budget)
{
	struct roq_eth_priv *vdev =
		container_of(napi, struct roq_eth_priv, napi);

	unsigned int npackets = roq_eth_rx_clean(vdev, budget);

	if (npackets < budget)
		napi_complete(&vdev->napi);

	return npackets;
}

static struct ib_client roq_eth_client = {
	.name   = "roq_eth",
	.add    = roq_eth_add_one,
	.remove = roq_eth_remove_one
};

static void
roq_eth_add_one(struct ib_device *ibdev)
{
	struct roq_eth_priv *priv;
	struct roq_dev *roq_dev = container_of(ibdev, struct roq_dev, ofa_dev);
	struct net_device *ndev = roq_dev->l2dev;

	/* roq is represented as RDMA_NODE_RNIC */
	if (rdma_node_get_transport(ibdev->node_type) != RDMA_TRANSPORT_IWARP
	    || strncmp(ibdev->name, "roq", IB_DEVICE_NAME_MAX) != 0)
		return;

	priv = ib_get_client_data(ibdev, &roq_eth_client);
	if (priv) {
		pr_warn("roq_eth_add_one: netdev already allocated!\n");
		return;
	}

	SET_NETDEV_DEV(ndev, ibdev->dma_device);

	priv = netdev_priv(ndev);
	priv->ndev  = ndev;
	priv->ibdev = ibdev;
	ib_set_client_data(ibdev, &roq_eth_client, priv);

	return;
}

static void
roq_eth_remove_one(struct ib_device *ibdev)
{
	struct roq_eth_priv *priv;

	if (rdma_node_get_transport(ibdev->node_type) != RDMA_TRANSPORT_IWARP
	    || strncmp(ibdev->name, "roq", IB_DEVICE_NAME_MAX) != 0)
		return;

	priv = ib_get_client_data(ibdev, &roq_eth_client);

	priv->ndev->dev.parent   = NULL;
	unregister_netdev(priv->ndev);
	free_netdev(priv->ndev);
}

int
roq_eth_open(struct net_device *ndev)
{
	struct roq_eth_priv *priv = netdev_priv(ndev);
	int rv;

	if (priv->ibdev == NULL) {
		pr_warn("roq_eth_open: no transport device set\n");
		return -ENODEV;
	}

	if ((rv = roq_eth_init_qp(ndev)) != 0)
		return rv;

	netif_napi_add(ndev, &priv->napi, roq_eth_napi, MAX_RX_SKBS);

	netif_start_queue(ndev);
	napi_enable(&priv->napi);

	rv = roq_cm_init();
	if (rv) {
		pr_warn("roq_eth_open: cannot init RoQ CM\n");
		return rv;
	}
	return 0;
}

int
roq_eth_release(struct net_device *ndev)
{
	struct roq_eth_priv *priv = netdev_priv(ndev);

	roq_cm_exit();
	napi_disable(&priv->napi);
	netif_stop_queue(ndev);

	roq_eth_cleanup(ndev);
	return 0;
}

int
roq_eth_config(struct net_device *ndev, struct ifmap *map)
{
	if (ndev->flags & IFF_UP)
		return -EBUSY;

	if (map->base_addr != ndev->base_addr) {
		pr_warn("roq_eth_config: Can't change I/O address\n");
		return -EOPNOTSUPP;
	}

	return 0;
}


static inline int
roq_eth_hw_tx(struct sk_buff *skb, struct net_device *ndev, int num_slots)
{
	struct ib_send_wr	sq_wr, *bad_wr;
	struct ib_sge		send_sgl[MAX_TX_SEGS], *sge = send_sgl;
	struct ib_qp		*qp;
	struct roq_eth_priv	*priv = netdev_priv(ndev);
	struct ethhdr		*eth = (struct ethhdr *)skb->data;
	unsigned int		len = skb_headlen(skb),
				mapped_slots = 0;
	tx_slot_desc_t		slot_desc;
	dma_addr_t		bus_addr;

	int rank, rv = -EINVAL;

	if (likely(len)) {
		bus_addr = dma_map_single(&ndev->dev, skb->data, len,
					  DMA_TO_DEVICE);
		if (unlikely(dma_mapping_error(&ndev->dev, bus_addr))) {
			pr_warn("DMA map of TX Data Buffer addr "
				"0x%016lx len %d failed\n",
				(unsigned long)skb->data, len);
			rv = -ENOMEM;
			goto out;
		}
		mapped_slots = 1;
	} else {
		/*
		 * Shall we handle that case ?
		 * For now we just disable fragmented TX
		 */
		ndev->features &= ~NETIF_F_SG;
		pr_info("Cannot handle skb w/o headlen, SG TX disabled\n");
		rv = -ENOBUFS;
		goto out;
	}

	/* Get free slots */
	roq_get_tx_slots(priv, &slot_desc, num_slots);

	priv->tx_buf.post[slot_desc.slot[0]].skb  = skb;
	priv->tx_buf.post[slot_desc.slot[0]].addr = bus_addr;
	priv->tx_buf.post[slot_desc.slot[0]].len  = len;

	sge->addr = bus_addr;
	sge->length = len;
	sge->lkey = 0;

	sq_wr.next = NULL;
	sq_wr.wr_id = slot_desc.id;
	sq_wr.sg_list = sge;
	sq_wr.num_sge = num_slots;
	sq_wr.opcode = IB_WR_SEND;
	sq_wr.send_flags = 0;

	if (--num_slots > 0) {
		struct skb_shared_info	*sk_info = skb_shinfo(skb);
		struct skb_frag_struct	*frag = sk_info->frags;

		int i = 1;

		do {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
			bus_addr = dma_map_page(&ndev->dev, frag->page,
						frag->page_offset, frag->size,
						DMA_TO_DEVICE);
#else
			bus_addr = dma_map_page(&ndev->dev, skb_frag_page(frag),
						frag->page_offset, frag->size,
						DMA_TO_DEVICE);
#endif

			if (unlikely(dma_mapping_error(&ndev->dev, bus_addr))) {
				rv = -ENOBUFS;
				goto out_unmap;
			}
			priv->tx_buf.post[slot_desc.slot[i]].addr = bus_addr;
			priv->tx_buf.post[slot_desc.slot[i]].len = frag->size;
			
			sge++;

			sge->addr = bus_addr;
			sge->length = frag->size;
			sge->lkey = 0;

			mapped_slots++;
			frag++;
			i++;

		} while (--num_slots > 0);
	}
	if (mapped_slots > 1) {
		int i;
		for (i = 0; i < mapped_slots; i++)
			pr_info("sge %d:: addr 0x%llx, len %u\n",
				i, send_sgl[i].addr, send_sgl[i].length);

		ndev->features &= ~NETIF_F_SG;
	}

	if ((priv->is_ionode && eth->h_dest[3] & 0x80)
	    || (!priv->is_ionode && eth->h_dest[3] & 0x40)) {
		rank = roq_tcoords_to_rank(priv->netdesc_rem,
					   eth->h_dest[3] & 0xf,
					   eth->h_dest[4] >> 4,
					   eth->h_dest[4] & 0xf,
					   eth->h_dest[5] >> 4,
					   eth->h_dest[5] & 0xf);
		if (rank < 0)
			goto out_unmap;

		qp = priv->qps_rem[rank];
	} else {
		rank = roq_tcoords_to_rank(priv->netdesc,
					   eth->h_dest[3] & 0xf,
					   eth->h_dest[4] >> 4,
					   eth->h_dest[4] & 0xf,
					   eth->h_dest[5] >> 4,
					   eth->h_dest[5] & 0xf);
		if (rank < 0)
			goto out_unmap;

		qp = priv->qps[rank];
	}
	if (unlikely(rank < 0 || qp == NULL)) {
		pr_info("roq_eth_hw_tx:: rank: %d, qp: 0x%p\n", rank, qp);
		rv = -EINVAL;
		goto out_unmap;
	}

	rv = ib_post_send(qp, &sq_wr, &bad_wr);
	if (unlikely(IS_ERR_VALUE(rv))) {
		pr_warn("roq_eth_hw_tx: post_send: err %d free %d\n",
			rv, atomic_read(&priv->tx_buf.inuse));
		goto out_unmap;
	}
out:
	return rv;

out_unmap:
	pr_info("roq_eth_hw_tx: failed, %d, num_slots %d, len %d, head %d\n",
		rv, mapped_slots, skb->len, skb_headlen(skb));
	roq_put_tx_slots(priv, &slot_desc, mapped_slots);

	return rv;
}

int
roq_eth_tx(struct sk_buff *skb, struct net_device *ndev)
{
	struct roq_eth_priv	*priv = netdev_priv(ndev);
	struct ethhdr		*eth = (struct ethhdr *)skb->data;
	struct iphdr		*iph =
		(struct iphdr *)((skb->data) + sizeof(struct ethhdr));
	int num_bufs, slots_inuse = 0, rv = 0;

	if (unlikely(skb->len < ETH_HLEN)) {
		dev_kfree_skb_any(skb);
		goto out;
	}
	num_bufs = skb_shinfo(skb)->nr_frags + 1;
	if (unlikely(num_bufs > MAX_TX_SEGS)) {
		/* Disable SG support */
		pr_info("Cannot handle %d TX frags, disable SG TX\n", num_bufs);
		ndev->features &= ~NETIF_F_SG;
		rv = -ENOBUFS;
		goto out;
	}
	if (num_bufs > 1)
		pr_info("got %d frags\n", num_bufs - 1);
	
	if (unlikely(eth->h_dest[1] != 0x80 && eth->h_dest[1] != 0x47)) {
		goto out_err;
	}

	/* handle torus internal / external routing */
	if (likely((iph->daddr >> 24) == ip_prefix)) {
		/* update destination mac address */
		eth->h_dest[3] = (iph->daddr >> 16) & 0xff;
		eth->h_dest[4] = (iph->daddr >>  8) & 0xff;
		eth->h_dest[5] = (iph->daddr & 0xff);
	} else if (gw_rank >= 0) {
		eth->h_dest[3] = priv->gw_dest[0];
		eth->h_dest[4] = priv->gw_dest[1];
		eth->h_dest[5] = priv->gw_dest[2];
	} else {
		pr_warn("roq_eth_tx: routing disabled. "
			"Use module option gw_rank to set "
			"up routing through the IO-torus.");
		rv = -EINVAL;
		goto out_err;
	}
	slots_inuse = atomic_add_return(num_bufs, &priv->tx_buf.inuse);

	if (unlikely(slots_inuse >= MAX_TX_SKBS)) {
		pr_warn("roq_eth_tx: no TX slot");
		rv = -ENOBUFS;
		goto out_err;
	}

	rv = roq_eth_hw_tx(skb, ndev, num_bufs);
	if (unlikely(rv)) {
		pr_warn("roq_eth_tx: Error sending frame of size %d",
			skb->len);
		goto out_err;
	}
	ndev->trans_start = jiffies;
out:
	return NETDEV_TX_OK;

out_err:
	dev_kfree_skb(skb);

	if (slots_inuse)
		atomic_sub(num_bufs, &priv->tx_buf.inuse);

	spin_lock_bh(&priv->tx_lock);
	priv->stats.tx_dropped++;
	if (rv) {
		priv->stats.tx_errors++;
		pr_warn("roq_eth_tx: Error %d", rv);
	}
	spin_unlock_bh(&priv->tx_lock);

	return NETDEV_TX_OK;
}

void
roq_eth_tx_timeout(struct net_device *ndev)
{
	struct roq_eth_priv *priv = netdev_priv(ndev);

	priv->stats.tx_errors++;
	netif_wake_queue(ndev);
	return;
}

int
roq_eth_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	PDEBUG("ioctl\n");
	return 0;
}

struct net_device_stats *
roq_eth_stats(struct net_device *ndev)
{
	struct roq_eth_priv *priv = netdev_priv(ndev);

	return &priv->stats;
}

int
roq_eth_change_mtu(struct net_device *ndev, int new_mtu)
{
	unsigned long flags;
	struct roq_eth_priv *priv = netdev_priv(ndev);
	spinlock_t *lock = &priv->lock;

	if ((new_mtu < 68) || (new_mtu > 64996))
		return -EINVAL;

	spin_lock_irqsave(lock, flags);
	ndev->mtu = new_mtu;
	spin_unlock_irqrestore(lock, flags);

	return 0;
}

static const struct net_device_ops roq_eth_netdev_ops = {
	.ndo_open               = roq_eth_open,
	.ndo_stop               = roq_eth_release,
	.ndo_start_xmit         = roq_eth_tx,
	.ndo_do_ioctl           = roq_eth_ioctl,
	.ndo_get_stats          = roq_eth_stats,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,2,0)
	.ndo_set_multicast_list = NULL,
#endif
	.ndo_change_mtu         = roq_eth_change_mtu,
	.ndo_tx_timeout         = roq_eth_tx_timeout,
	.ndo_set_mac_address    = NULL,
};


void
roq_eth_init(struct net_device *ndev, struct RoQ_DeviceMemory *vdm)
{
	struct roq_eth_priv *priv;

	ether_setup(ndev);

	priv = netdev_priv(ndev);
	memset(priv, 0, sizeof *priv);

	/* set mac */
	bluegene_getPersonality(&priv->personality, sizeof(Personality_t));

	ndev->dev_addr[0] = 0x00;
	ndev->dev_addr[1] = 0x80;
	ndev->dev_addr[2] = 0x47;
	ndev->dev_addr[3] = priv->personality.Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode ? 0x40 : 0x80;
	ndev->dev_addr[3] = ndev->dev_addr[3] | priv->personality.Network_Config.Acoord;
	ndev->dev_addr[4] = (priv->personality.Network_Config.Bcoord << 4) | priv->personality.Network_Config.Ccoord;
	ndev->dev_addr[5] = (priv->personality.Network_Config.Dcoord << 4) | priv->personality.Network_Config.Ecoord;

	ndev->netdev_ops = &roq_eth_netdev_ops;
	ndev->watchdog_timeo = 100;

	memcpy(ndev->name, "roq%d", sizeof(ndev->name));

	/* keep the default flags, just add NOARP */
	ndev->flags |= IFF_NOARP;
	ndev->flags &= ~(IFF_MULTICAST|IFF_ALLMULTI|IFF_BROADCAST);
	ndev->features = NETIF_F_HIGHDMA | NETIF_F_ALL_CSUM; //| NETIF_F_SG;
	ndev->mtu = 64996;
	ndev->tx_queue_len = MAX_TX_SKBS;

	priv->ndev = ndev;
	atomic_set(&priv->tx_buf.inuse, 0);

	priv->netdesc     = &vdm->mNetworkDesc;
	priv->netdesc_rem = &vdm->mNetworkDesc_rem;

	priv->part_size = ND_TORUS_SIZE(priv->personality.Network_Config);
	priv->is_ionode = priv->personality.Kernel_Config.NodeConfig & PERS_ENABLE_IsIoNode ? 1 : 0;
	pr_info("This is a%s node. Partition size: %d\n", priv->is_ionode ? "n IO" : " CP", priv->part_size);

	if (gw_rank >= 0) {
		if (gw_rank < priv->part_size) {
			// calculate coordinates from rank
    /* match CNK's default map order ABCDET */
    /* rank = E + dim_E ( D + dim_D ( C + dim_C ( B + dim_B * A ))) */
    /* thus, we can split off each coordinate by repeated mod and div */
			int rank = gw_rank;
			int faktor = priv->netdesc->Anodes * priv->netdesc->Bnodes
			             * priv->netdesc->Cnodes * priv->netdesc->Dnodes;
			int a, b, c, d, e;

			e = rank % priv->netdesc->Enodes;
			rank /= priv->netdesc->Enodes;

			d = rank % priv->netdesc->Dnodes;
			rank /= priv->netdesc->Dnodes;

			c = rank % priv->netdesc->Cnodes;
			rank /= priv->netdesc->Cnodes;

			b = rank % priv->netdesc->Bnodes;
			rank /= priv->netdesc->Bnodes;

			a = rank % priv->netdesc->Anodes;

			// set lower-part of "MAC"-address of gateway rank
			priv->gw_dest[0] = ((priv->is_ionode)? 0x40 : 0x80) | a;
			priv->gw_dest[1] = (b << 4) | c;
			priv->gw_dest[2] = (d << 4) | e;
			printk ("priv->gw_dest[0-2] = %x:%x:%x\n",
				priv->gw_dest[0], priv->gw_dest[1], priv->gw_dest[2]);

			// if we do routing outside of torus, we need checksums
			ndev->features &= ~NETIF_F_ALL_CSUM;
		} else {
			pr_warn("roq_eth_init: gw_rank greater than partition size (gw_rank=%d, partion size=%d)",
				gw_rank, priv->part_size);
			// disable gateway
			gw_rank = -1;
		}
	}

	spin_lock_init(&priv->lock);
	spin_lock_init(&priv->tx_lock);
}

void
roq_eth_cleanup(struct net_device *ndev)
{
	struct roq_eth_priv *priv = netdev_priv(ndev);

	if (ndev) {
		roq_eth_cleanup_ofa(priv);
		roq_free_rx_buffers(priv);
	}
	return;
}

#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
static int
proc_read_txfifo(char *page, char **start, off_t off, int count,
                             int *eof, void *data)
{
	struct roq_eth_tx_buffer *txbuf = (struct roq_eth_tx_buffer *)data;
	int len = sprintf(page, "txfifo.inuse = %d\n",
			  atomic_read(&txbuf->inuse));

	return len;
}
#endif

/*
 * kernel module specific functions
 */
int __init
roq_eth_init_module(struct net_device **ndev, struct RoQ_DeviceMemory *vdm)
{
#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
	struct roq_eth_priv *priv;
#endif
	int rv;

	pr_info(KERN_INFO DRV_NAME_EXPLICIT": %s %s\n", __DATE__, __TIME__);

	*ndev = alloc_etherdev(sizeof(struct roq_eth_priv));
	if (!ndev) {
		rv = -ENOMEM;
		goto out;
	}
	roq_eth_init(*ndev, vdm);

	rv = register_netdev(*ndev);
	if (rv) {
		pr_info(DRV_NAME ": error registering device \"%s\"\n",
		       (*ndev)->name);
		goto out;
	}

#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
	priv = netdev_priv(*ndev);
	/* create directory */
	priv->proc_dir = proc_mkdir(DRV_NAME, NULL);
	if (priv->proc_dir == NULL) {
		rv = -ENOMEM;
		goto out;
	}

	/* create jiffies using convenience function */
	priv->txfifo_file =
		create_proc_read_entry("txfifo", 0444, priv->proc_dir,
					proc_read_txfifo, NULL);
	if (priv->txfifo_file == NULL) {
		rv  = -ENOMEM;
		goto out1;
	}
	priv->txfifo_file->data  = &priv->tx_buf;

#endif

	rv = ib_register_client(&roq_eth_client);
#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
	goto out;
out1:
	remove_proc_entry(DRV_NAME, NULL);
#endif
out:
	if (rv) {
		printk(DRV_NAME": init error %d\n", rv);
		roq_eth_cleanup(*ndev);
		*ndev = NULL;
	}
	return rv;
}

void __exit
roq_eth_exit_module(struct net_device *ndev)
{
#if defined(PROC_DEBUG_ENTRY) && PROC_DEBUG_ENTRY == 1
	struct roq_eth_priv *priv = netdev_priv(ndev);
	remove_proc_entry("txfifo", priv->proc_dir);
	remove_proc_entry(DRV_NAME, NULL);
#endif
	ib_unregister_client(&roq_eth_client);
}
