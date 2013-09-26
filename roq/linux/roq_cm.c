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

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/net.h>
#include <linux/inetdevice.h>
#include <linux/workqueue.h>
#include <net/sock.h>
#include <net/tcp_states.h>

#include "roq.h"
#include "roq_cm.h"
#include "roq_utils.h"

#undef KERN_INFO
#define KERN_INFO KERN_EMERG

#define CM_PORT 7
#define DRV_NAME "ROQ_CM"

#undef KERN_INFO
#define KERN_INFO KERN_EMERG

extern struct roq_dev *roq_device;

struct roq_sock_transport *trans = NULL;
static struct workqueue_struct *cm_workq = NULL;
const static int debug_cm_timer = 1;

static int
roq_cm_recv(unsigned char *buf, int len)
{
	struct msghdr msg;
	struct iovec iov;
	mm_segment_t oldfs;
	int size = 0;

	if (trans->sock->sk == NULL)
		return 0;

	iov.iov_base = buf;
	iov.iov_len = len;

	msg.msg_flags = 0;
	msg.msg_name = &trans->addr;
	msg.msg_namelen  = sizeof(struct sockaddr_in);
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;

	oldfs = get_fs();
	set_fs(KERNEL_DS);
	size = sock_recvmsg(trans->sock, &msg, len, msg.msg_flags);
	set_fs(oldfs);

	return size;
}

static int
roq_cm_send(void *buf, uint32_t dest_addr, int size)
{
	struct msghdr msg;
	struct iovec iov;
	mm_segment_t oldfs;
	int rv;

	/*
	 * Current version uses one dedicated UDP port 'CM_PORT'
	 * to handle any inbound CM traffic.
	 */
	struct sockaddr_in peer;

	peer.sin_addr.s_addr = dest_addr;
	peer.sin_port = htons(CM_PORT);
	peer.sin_family = AF_INET;

	if (trans->sock_snd == NULL || trans->sock_snd->sk == NULL) {
		WARN_ON(1);
		return 0;
	}
	iov.iov_base = buf;
	iov.iov_len = size;

	msg.msg_flags = 0;
	msg.msg_name = &peer;
	msg.msg_namelen  = sizeof peer;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;

	oldfs = get_fs();
	set_fs(get_ds());
	rv = sock_sendmsg(trans->sock_snd, &msg, size);
	set_fs(oldfs);
	if (rv != size) {
		pr_info("roq_cm_send: error %d\n", rv);
		if (rv >= 0)
			rv = -EPIPE;
	} else
		rv = 0;

	return rv;
}

static int roq_cmid_matches_msg(struct iw_cm_id *id, struct roq_cm_msg *msg)
{
	return (id->local_addr.sin_addr.s_addr == msg->local_addr	&&
		id->local_addr.sin_port == msg->local_port		&&
		id->remote_addr.sin_addr.s_addr == msg->remote_addr	&&
		id->remote_addr.sin_port == msg->remote_port);
}

static struct roq_qp *roq_find_qp_by_addr(struct roq_cm_msg *cm_msg)
{
	struct roq_qp *dd_qp = NULL;
	unsigned long flags;

	spin_lock_irqsave(&roq_device->qp_lock, flags);

	list_for_each_entry(dd_qp, &roq_device->qp_list, devq) {
		struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(roq_device,
							roq_qpid(dd_qp));

		if (!dm_qp || !dm_qp->cm_id || dm_qp->qp_state != ROQ_QPS_RTS)
			continue;

		if ((dm_qp->cm_id->remote_addr.sin_addr.s_addr ==
		     cm_msg->local_addr) &&
		    (dm_qp->cm_id->remote_addr.sin_port ==
		     cm_msg->local_port)) {
			if (debug_cm_timer || debug_cm)
				pr_info("QP[%d]: ID found\n", roq_qpid(dd_qp));
			roq_qp_get_ref(&dd_qp->ofa_qp);
			spin_unlock_irqrestore(&roq_device->qp_lock, flags);
			return dd_qp;
		}
	}
	spin_unlock_irqrestore(&roq_device->qp_lock, flags);
	return NULL;
}

void roq_cm_cancel_timer(struct roq_qp *dd_qp)
{
	bool cancelled; 

	if (dd_qp->cm_work) {
		if (debug_cm_timer && dd_qp->cm_msg->retry > 0)
			pr_info("QP[%d]: Cancel timer %d at retry %d\n",
				roq_qpid(dd_qp),
				dd_qp->cm_msg ? dd_qp->cm_msg->type : -1,
				dd_qp->cm_msg ? dd_qp->cm_msg->retry : 0);

		cancelled = cancel_delayed_work_sync(&dd_qp->cm_work->work);
		if (cancelled) {
			kfree(dd_qp->cm_work);
			dd_qp->cm_work = NULL;
			kfree(dd_qp->cm_msg);
			dd_qp->cm_msg = NULL;

			roq_qp_put_ref(&dd_qp->ofa_qp);
		}
	}
}


/* Forward declaration */
static int roq_cm_queue_work(void *, enum roq_cm_work_type);


static void
connection_manager(struct roq_cm_msg *cm_msg)
{
	struct RoQ_DeviceMemory *RoQ_DevMem = roq_device->RoQ;

	struct iw_cm_conn_param *cur_params = &cm_msg->params;

	struct roq_listen_info *listen_info;
	struct iw_cm_event event;

	struct RoQ_DM_QP *dm_qp;

	struct iw_cm_id	*cm_id = NULL;
	struct connreq_info	*req_info;

	struct roq_qp *dd_qp;
	struct ib_qp_attr qp_attr;
	enum ib_qp_attr_mask qp_attr_mask;
	unsigned long flags;
	int DestRank, rc = 0;

	if (debug_cm)
		pr_info("enter CM FSM, type %d\n", cm_msg->type);

	memset(&event, 0, sizeof event);

	switch (cm_msg->type) {

	case RoQ_CM_ConnRequest:
		if (cm_msg->retry > 0) {
			if (debug_cm_timer)
				pr_info("Got ConnReq retry #%d\n",
					cm_msg->retry);

			dd_qp = roq_find_qp_by_addr(cm_msg);
			if (dd_qp) {
				struct roq_cm_msg *ConResMsg = dd_qp->cm_msg;
				if (ConResMsg) {
					if (debug_cm_timer || debug_cm)
						pr_info("QP[%d]: resend Resp "
							"#%d\n",
							roq_qpid(dd_qp),
							ConResMsg->retry);
					/*
					 * Unable to send in hard intr context
					 * so must schedule work.
					 */
					roq_cm_queue_work(dd_qp,
							  ROQ_CM_WORK_SEND_MSG);
				}
				roq_qp_put_ref(&dd_qp->ofa_qp);
				break;
			}
		}

		spin_lock_irqsave(&roq_device->listen_lock, flags);
		listen_info =
			roq_device->ListenPortToPointer[cm_msg->remote_port];

		if (!listen_info) {
			spin_unlock_irqrestore(&roq_device->listen_lock, flags);
			
			if (debug_cm)
				pr_info("No listener on port %d",
					cm_msg->remote_port);

			/* Reuse current message for reject */
			cm_msg->type = RoQ_CM_ConnReject;
			cm_msg->params.private_data_len = 0;

			(void)roq_cm_send(cm_msg, cm_msg->local_addr,
					  sizeof *cm_msg);

			break;
		} else {
			/*
			 * Check if last ConnRequest not yet handled by
			 * application
			 */
			int wait_for_app = 0;
			list_for_each_entry(req_info, &listen_info->conn_info,
					    list) {
				if (req_info->peer_qp == cm_msg->active_qpn &&
				    req_info->peer_addr == cm_msg->local_addr &&
				    req_info->peer_port == cm_msg->local_port) {

					wait_for_app = 1;
					break;
				}
			}
			if (wait_for_app) {
				/*
				 * Wait for application accept/reject
				 */
				spin_unlock_irqrestore(&roq_device->listen_lock,
						       flags);
				if (debug_cm_timer || debug_cm)
					pr_info("ConnReq retry: "
						"await appl reply\n");
				break;
			}
		}
		dprint(DBG_ON, KERN_INFO, "found a listener");
		event.event = IW_CM_EVENT_CONNECT_REQUEST;
		event.status = 0;
		/* local addr of remote is the remote addr of this connect */
		event.local_addr.sin_family = AF_INET;
		event.local_addr.sin_addr.s_addr = cm_msg->remote_addr;
		event.local_addr.sin_port = cm_msg->remote_port;
		event.remote_addr.sin_family = AF_INET;
		event.remote_addr.sin_addr.s_addr = cm_msg->local_addr;
		event.remote_addr.sin_port = cm_msg->local_port;

		req_info = kmalloc(cur_params->private_data_len
				   + sizeof *req_info, GFP_KERNEL);
		if (!req_info) {
			spin_unlock_irqrestore(&roq_device->listen_lock, flags);
			pr_info("could not alloc conreq\n");
			break;
		}
		req_info->peer_qp = cm_msg->active_qpn;
		req_info->peer_addr = cm_msg->local_addr;
		req_info->peer_port = cm_msg->local_port;

		INIT_LIST_HEAD(&req_info->list);
		list_add_tail(&req_info->list, &listen_info->conn_info);

		if (cur_params->private_data_len) {
			event.private_data = req_info->pdata;
			memcpy(req_info->pdata, cm_msg->private_data,
			       cur_params->private_data_len);
			event.private_data_len = cur_params->private_data_len;
		} else {
			dprint(DBG_ON, KERN_INFO,
			       "roq conreq EVENT private data len 0");
			event.private_data_len = 0;
			event.private_data = NULL;
		}
		/* passed back during the accept/reject */
		event.provider_data = (void *)req_info;

		dprint(DBG_ON, KERN_INFO, "calling event handler");

		if (debug_cm)
			pr_info("Port %d:%d  CONREQ upcall\n",
				cm_msg->remote_port,
				cm_msg->local_port);

		rc = listen_info->cm_id->event_handler(listen_info->cm_id,
						       &event);
		if (rc)
			pr_info("roq ConReq upcall failed: %d, pdata %d\n",
				rc, event.private_data_len);

		spin_unlock_irqrestore(&roq_device->listen_lock, flags);

		break;

	case RoQ_CM_ConnResponse:
		if ((debug_cm_timer || debug_cm) && cm_msg->retry > 0) {
			pr_info("Got ConnResp retry #%d\n",
				cm_msg->retry);
		}
		dm_qp = roq_get_dm_qp(roq_device, cm_msg->active_qpn);
		if (!dm_qp) {
			pr_warn("RoQ_CM_ConnResponse with invalid QP ID %u\n",
				cm_msg->active_qpn);
			break;
		}
		spin_lock_irqsave(&dm_qp->dd_lock, flags);

		dd_qp = roq_get_dd_qp(roq_device, cm_msg->active_qpn);

		if (!dd_qp) {
			uint32_t peer_qp = cm_msg->active_qpn;
			uint32_t peer_addr = cm_msg->local_addr;
			uint16_t peer_port = cm_msg->local_port;

			if (debug_cm)
				pr_info("ConnResp: No valid QP[%d]\n",
					(int)cm_msg->active_qpn);

			spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

			cm_msg->type	    = RoQ_CM_Disconnect;
			cm_msg->active_qpn  = cm_msg->passive_qpn;
			cm_msg->passive_qpn = peer_qp;
			cm_msg->local_port  = cm_msg->remote_port;
			cm_msg->remote_port = peer_port;
			cm_msg->local_addr  = cm_msg->remote_addr;
			cm_msg->remote_addr = peer_addr;
			cm_msg->params.private_data_len = 0;

			roq_cm_send(cm_msg, peer_addr, sizeof *cm_msg);

			break;
		}
		roq_cm_cancel_timer(dd_qp);

		if (dm_qp->qp_state != ROQ_QPS_RTR) {
			/* This RTR means CONNECTING */
			if (debug_cm)
				pr_info("QP[%d]: skipped CM resp, state %d\n",
					roq_qpid(dd_qp), dm_qp->qp_state);

			spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
			roq_qp_put_ref(&dd_qp->ofa_qp);
			break;
		}

		cm_id = dm_qp->cm_id;

		if (!dd_qp->is_connecting) {
			if (debug_cm)
				pr_info("QP %04d: ID %p: ConnReply for conn. "
					"in state %d, CMID %p\n",
					roq_qpid(dd_qp), dm_qp->cm_id,
					dm_qp->qp_state,
					dm_qp->cm_id);

			if (!cm_id) {
				if (debug_cm)
					pr_info("QP %04d: NOP\n",
						roq_qpid(dd_qp));

				spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

				roq_qp_put_ref(&dd_qp->ofa_qp);
				break;
			}
			if (roq_cmid_matches_msg(cm_id, cm_msg)) {
				pr_info("QP %04d: CLOSE upcall\n",
					roq_qpid(dd_qp));

				event.event = IW_CM_EVENT_CLOSE;
				event.status = 0;
				event.local_addr.sin_family = AF_INET;
				event.local_addr.sin_addr.s_addr =
					cm_msg->remote_addr;
				event.local_addr.sin_port = cm_msg->remote_port;
				event.remote_addr.sin_family = AF_INET;
				event.remote_addr.sin_addr.s_addr =
					cm_msg->local_addr;
				event.remote_addr.sin_port = cm_msg->local_port;
				event.provider_data = NULL;
				event.private_data_len = 0;

				cm_id->event_handler(cm_id, &event);
				cm_id->rem_ref(cm_id);
				dm_qp->cm_id = NULL;

				spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

				roq_qp_put_ref(&dd_qp->ofa_qp);
				break;
			}
		}

		/* assume accept */
		event.status = 0;

		/*
		 * set the roq qp for the accept
		 * NOTE: swapping remote vs local -- should used Active vs
		 * Passive
		 */
		if ((cm_msg->local_addr ^
		     cm_msg->remote_addr) & 0x00c00000) {
			RoQ_IPV4Addr_To_Rank(&RoQ_DevMem->mNetworkDesc_rem,
					     cm_msg->local_addr, &DestRank);
			DestRank |= 0x8000 ;
		} else {
			RoQ_IPV4Addr_To_Rank(&RoQ_DevMem->mNetworkDesc,
					     cm_msg->local_addr, &DestRank);
		}

		qp_attr_mask  = 0;
		qp_attr_mask |= IB_QP_STATE;
		qp_attr.qp_state = IB_QPS_RTS;
		qp_attr_mask |= IB_QP_AV;
		qp_attr.ah_attr.dlid = DestRank;
		qp_attr_mask |= IB_QP_DEST_QPN;
		qp_attr.dest_qp_num = cm_msg->passive_qpn;

		if (debug_cm) {
			pr_info("QP %04d: ID %p: CONNREPLY. "
				"state %d, in_conn:active (%d:%d)\n",
				roq_qpid(dd_qp), cm_id,
				dm_qp->qp_state,
				dd_qp->is_connecting,
				dd_qp->active_connect);
		}

		dd_qp->is_connecting = 0;
		dd_qp->active_connect = 0;

		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

		if (down_write_trylock(&dd_qp->state_lock)) {
			roq_modify_qp(&dd_qp->ofa_qp, &qp_attr,
				      (int)qp_attr_mask, NULL);

			up_write(&dd_qp->state_lock);
		} else {
			roq_qp_put_ref(&dd_qp->ofa_qp);

			pr_info("connnect response: cannot get QP lock");

			break;
		}

		event.event = IW_CM_EVENT_CONNECT_REPLY;
		/* local addr of remote is the remote addr of this connect */
		event.local_addr.sin_family = AF_INET;
		event.local_addr.sin_addr.s_addr = cm_msg->remote_addr;
		event.local_addr.sin_port = cm_msg->remote_port;
		event.remote_addr.sin_family = AF_INET;
		event.remote_addr.sin_addr.s_addr = cm_msg->local_addr;
		event.remote_addr.sin_port = cm_msg->local_port;

		event.provider_data = NULL;

		if (cur_params->private_data_len) {
			event.private_data = 
			  kmalloc(cur_params->private_data_len, GFP_KERNEL);
			if (!event.private_data) {
				pr_info("roq ConRes: could not alloc "
					"private date\n");

				roq_qp_put_ref(&dd_qp->ofa_qp);
				break;
			}
			/* to free private data later */
			dd_qp->conn_data = event.private_data;

			memcpy(event.private_data, cm_msg->private_data,
			       cur_params->private_data_len);
			event.private_data_len = cur_params->private_data_len;
		} else {
			dprint(DBG_ON, KERN_INFO,
			       "roq conres EVENT private data len 0");
			event.private_data_len = 0;
			event.private_data = NULL;
		}

		if (debug_cm)
			pr_info("QP %04d: ID %p, Connect: ESTABLISHED upcall. "
				"state %d, in_conn:active (%d:%d)\n",
				roq_qpid(dd_qp), dm_qp->cm_id, dm_qp->qp_state,
				dd_qp->is_connecting, dd_qp->active_connect);

		spin_lock_irqsave(&dm_qp->dd_lock, flags);
		cm_id = dm_qp->cm_id;
		if (cm_id)
			rc = cm_id->event_handler(cm_id, &event);
		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

		if (rc)
			pr_info("cm event handler failed! rc=%d", rc);

		dprint(DBG_ON, KERN_INFO, "AsyncMsg ConRes Done CallBack");

		roq_qp_put_ref(&dd_qp->ofa_qp);
		break;

	case RoQ_CM_ConnReject:
		if (debug_cm)
			pr_info("Got ConRej: ActiveQPN %u ListenPort %d",
				cm_msg->active_qpn, cm_msg->remote_port);

		event.status = -ECONNREFUSED;
		event.event = IW_CM_EVENT_CONNECT_REPLY;
		event.local_addr.sin_family = AF_INET;
		event.local_addr.sin_addr.s_addr = cm_msg->remote_addr;
		event.local_addr.sin_port = cm_msg->remote_port;
		event.remote_addr.sin_family = AF_INET;
		event.remote_addr.sin_addr.s_addr = cm_msg->local_addr;
		event.remote_addr.sin_port = cm_msg->local_port;
		event.private_data_len = 0;
		event.private_data = NULL;

		if (cur_params->private_data_len) {
			event.private_data =
				kmalloc(cur_params->private_data_len,
					GFP_KERNEL);
			if (!event.private_data) {
				pr_info("roq ConRej: could not alloc "
					"private data\n");
				break;
			}
			memcpy(event.private_data, cm_msg->private_data,
			       cur_params->private_data_len);

			event.private_data_len = cur_params->private_data_len;
		}

		dm_qp = roq_get_dm_qp(roq_device, cm_msg->active_qpn);
		if (!dm_qp) {
			pr_warn("ConnReject with invalid QP ID %u\n",
				cm_msg->active_qpn);
			break;
		}
		spin_lock_irqsave(&dm_qp->dd_lock, flags);

		dd_qp = roq_get_dd_qp(roq_device, cm_msg->active_qpn);

		if (!dd_qp) {
			if (debug_cm)
				pr_info("roq ConRej: No DD QP\n");

			spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
			break;
		}
		if (debug_cm) {
			pr_info("QP %04d: ID %p: CONNREPLY REFUSE upcall. "
				"state %d, in_conn:active (%d:%d)\n",
				roq_qpid(dd_qp),
				dm_qp->cm_id,
				dm_qp->qp_state,
				dd_qp->is_connecting,
				dd_qp->active_connect);
		}
		roq_cm_cancel_timer(dd_qp);

		dd_qp->is_connecting = 0;
		dd_qp->active_connect = 0;

		if (!dm_qp->cm_id) {
			roq_qp_put_ref(&dd_qp->ofa_qp);

			if (debug_cm)
				pr_info("roq ConRej: No CM ID\n");

			spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
			break;
		}
		rc = dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);
		if (rc)
			pr_info("ConRej: cm event handler failed: %d\n",rc);

		dm_qp->cm_id->rem_ref(dm_qp->cm_id);
		dm_qp->cm_id = NULL;
		roq_qp_put_ref(&dd_qp->ofa_qp);

		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

		break;

	case RoQ_CM_Disconnect:
		dm_qp = roq_get_dm_qp(roq_device, cm_msg->active_qpn);
		if (!dm_qp) {
			pr_warn("RoQ_CM_Disconnect with invalid QP ID %u\n",
				cm_msg->active_qpn);
			break;
		}
		spin_lock_irqsave(&dm_qp->dd_lock, flags);

		dd_qp = roq_get_dd_qp(roq_device, cm_msg->active_qpn);

		if (!dd_qp) {
			if (debug_cm)
				pr_info("Disconnect: no matching QP[%d]\n",
					(int)cm_msg->active_qpn);
			spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
			break;
		}
		memset(&event, 0, sizeof(struct iw_cm_event));
		event.event = IW_CM_EVENT_CLOSE;
		event.status = -ECONNRESET;

		if (debug_cm)
			pr_info("QP[%d]: ID: %p: Peer Disconnect\n",
				roq_qpid(dd_qp), dm_qp->cm_id);

		if (dm_qp->cm_id) {
			dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);
			dm_qp->cm_id->rem_ref(dm_qp->cm_id);
			dm_qp->cm_id = NULL;

			roq_qp_put_ref(&dd_qp->ofa_qp);
		}
		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

		break;

	default:
		dprint(DBG_ON, KERN_INFO,
		       "Unknown asynchronous message: type=%d", cm_msg->type);
	}
	if (debug_cm)
		pr_info("leave CM FSM, type %d\n", cm_msg->type);
}

static void roq_cm_tx_worker(struct work_struct *work)
{
	struct roq_cm_work *tx_work = container_of(work, struct roq_cm_work,
						   work.work);
	struct roq_qp		*dd_qp = tx_work->qp;
	struct RoQ_DM_QP	*dm_qp = roq_get_dm_qp(roq_device,
						       roq_qpid(dd_qp));
	struct roq_cm_msg	*cm_msg = dd_qp->cm_msg;
	int size;

	if (!cm_msg || !dm_qp) {
		if (debug_cm_timer || debug_cm)
			pr_info("CM timeout: no action\n");
		goto out;
	}

	if (debug_cm_timer || debug_cm)
		pr_info("QP[%d]: CM timeout: type %d, retry %d\n",
			roq_qpid(dd_qp), cm_msg->type, cm_msg->retry);

	/* Handle final timeout */
	if (++cm_msg->retry >= CM_MAX_RETRIES) {
		struct iw_cm_event	event;
		unsigned long flags;

		if (cm_msg->type != RoQ_CM_ConnRequest)
			goto out;

		memset(&event, 0, sizeof(struct iw_cm_event));

		event.event = IW_CM_EVENT_CONNECT_REPLY;
		event.status = -ETIMEDOUT;

		if (debug_cm_timer || debug_cm)
			pr_info("QP[%d]: ID: %p: ConnReq Timeout\n",
				roq_qpid(dd_qp), dm_qp->cm_id);

		spin_lock_irqsave(&dm_qp->dd_lock, flags);

		if (dm_qp->cm_id) {
			dm_qp->cm_id->event_handler(dm_qp->cm_id, &event);

			dm_qp->cm_id->rem_ref(dm_qp->cm_id);
			dm_qp->cm_id = NULL;
		}
		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

		goto out;
	}

	size = sizeof *cm_msg + cm_msg->params.private_data_len;

	switch (cm_msg->type) {

	case RoQ_CM_ConnRequest:
		down_read(&dd_qp->state_lock);

		if (dm_qp->qp_state > ROQ_QPS_RTR ||
		    dm_qp->qp_state == ROQ_QPS_RESET) {
			up_read(&dd_qp->state_lock);
			goto out;
		}

		roq_cm_queue_work(dd_qp, ROQ_CM_WORK_CONREQ_TIMEOUT);

		if (debug_cm_timer || debug_cm)
			pr_info("QP[%d]: ConnReq Timeout. Resend 0x%08X:%u "
				" -> 0x%08X:%u\n", roq_qpid(dd_qp),
				cm_msg->local_addr,
				cm_msg->local_port,
				cm_msg->remote_addr,
				cm_msg->remote_port);

		roq_cm_send(dd_qp->cm_msg, cm_msg->remote_addr, size);

		up_read(&dd_qp->state_lock);

		break;

	case RoQ_CM_ConnResponse:
		if (debug_cm_timer || debug_cm)
			pr_info("QP[%d]: ConnResp. Send 0x%08X:%u "
				" -> 0x%08X:%u\n", roq_qpid(dd_qp),
				cm_msg->local_addr, cm_msg->local_port,
				cm_msg->remote_addr, cm_msg->remote_port);
		
		roq_cm_send(cm_msg, cm_msg->remote_addr, size);

		break;

	default:
		break;
	}
out:
	kfree(tx_work);
	roq_qp_put_ref(&dd_qp->ofa_qp);
}


/*
 * roq_cm_rx_worker()
 *
 * Process incoming CM messages. Assumes to be called by single-threaded
 * CM work queue.
 */
static void roq_cm_rx_worker(struct work_struct *work)
{
	struct roq_cm_work *rx_work = container_of(work, struct roq_cm_work,
						   work.work);
	struct roq_sock_transport *trans = rx_work->ctx;
	struct roq_cm_msg *cm_msg;
	static int idx = 0;

	spin_lock(&trans->con_lock);
	while (atomic_read(&trans->rx_free) < RX_BUF_DEPTH) {
		cm_msg = (struct roq_cm_msg *)trans->rxbuf[idx++];
		connection_manager(cm_msg);
		idx %= RX_BUF_DEPTH;
		atomic_inc(&trans->rx_free);
	}
	spin_unlock(&trans->con_lock);
	kfree(rx_work);
}

static int roq_cm_queue_work(void *ctx, enum roq_cm_work_type type)
{
	struct roq_cm_work *work = kmalloc(sizeof *work, GFP_KERNEL);
	unsigned long delay = 0;
	int rv = 0;

	if (work) {
		work->type = type;

		switch(type) {

		case ROQ_CM_WORK_RECV:
			work->ctx = (struct roq_sock_transport *)ctx;
			INIT_DELAYED_WORK(&work->work, roq_cm_rx_worker);

			break;

		case ROQ_CM_WORK_CONREQ_TIMEOUT:
		case ROQ_CM_WORK_CONRESP_TIMEOUT:
			work->qp = (struct roq_qp *)ctx;
			work->qp->cm_work = work;
			roq_qp_get_ref(&work->qp->ofa_qp);

			/* Randomize retransission delay */
			get_random_bytes(&delay, sizeof delay);
			delay = delay % ROQ_CM_TIMEOUT + ROQ_CM_TIMEOUT / 2;
			INIT_DELAYED_WORK(&work->work, roq_cm_tx_worker);

			break;

		case ROQ_CM_WORK_SEND_MSG:
			work->qp = (struct roq_qp *)ctx;
			roq_qp_get_ref(&work->qp->ofa_qp);
			INIT_DELAYED_WORK(&work->work, roq_cm_tx_worker);

			break;

		default:
			BUG();

			break;
		}
		queue_delayed_work(cm_workq, &work->work, delay);
	} else {
		pr_warn("roq_cm_queue_work: malloc failed\n");
		rv = -ENOBUFS;
	}
	return rv;
}

static void
roq_sock_server(struct roq_sock_transport *trans)
{
	int size, rv = 0;

	/* kernel thread initialization */
	trans->running = 1;
	current->flags |= PF_NOFREEZE;

	/*
	 * daemonize (take care with signals, after daemonize()
	 * they are disabled)
	 */
	daemonize(DRV_NAME);

	allow_signal(SIGKILL);

	/*
	 * dev points to the roq ethernet device. start creating
	 * and bind the socket
	 */ 
	rv = sock_create_kern(PF_INET, SOCK_DGRAM, IPPROTO_UDP,
			      &trans->sock);
	if (rv < 0) {
		pr_info("can't create UDP transport socket (%d).\n", rv);
		goto out;
	}

	memset(&trans->addr, 0, sizeof(struct sockaddr_in));
	trans->addr.sin_family = AF_INET;
	trans->addr.sin_addr.s_addr = htonl(((struct in_device *)
		trans->ndev->ip_ptr)->ifa_list->ifa_address);
	trans->addr.sin_port = htons(CM_PORT);

	rv = kernel_bind(trans->sock, (struct sockaddr *)&trans->addr,
			 sizeof(trans->addr));
	if (rv < 0) {
		pr_info("can't bind socket (%d).\n", rv);
		goto close_out;
	}

	/* main loop */
	for (;;) {
		struct roq_cm_msg *cm_msg;
		static int idx = 0;

		/* wait till rx buffer is free */
		while (atomic_read(&trans->rx_free) == 0) {
			static int informed = 0;
			if (!informed) {
				informed = 1;
				pr_info("no free rx buffer\n");
			}
			schedule();
		}
		if (signal_pending(current))
			break;

		size = roq_cm_recv(trans->rxbuf[idx], RX_BUF_SIZE);
		if (size <= 0) {
			pr_info("roq_cm_recv: %d\n", size);
			goto close_out;
		}
		cm_msg = (struct roq_cm_msg *)trans->rxbuf[idx];
		/*
		 * Skip all malformed CM messages
		 */
		if (size < sizeof *cm_msg) {
			pr_info("roq_cm_server: short CM message. size %d\n",
				size);
			continue;
		}
		if (cm_msg->params.private_data_len > ROQ_MAX_PRIVATEDATA) {
			pr_info("roq_cm_server: CM message %d: pdata %d\n",
				cm_msg->type, cm_msg->params.private_data_len);
			continue;
		}
		if (size != sizeof *cm_msg + cm_msg->params.private_data_len) {
			pr_info("roq_cm_server: CM message %d: len: %d != %d\n",
				cm_msg->type,
				(int)(size - sizeof *cm_msg),
				(int)cm_msg->params.private_data_len);
			continue;
		}
		idx++;
		idx %= RX_BUF_DEPTH;

		if (atomic_dec_return(&trans->rx_free) < RX_BUF_SIZE - 1) {
			if (debug_cm)
				pr_info("skip at %d\n",
					atomic_read(&trans->rx_free));
			continue;
		}
		if (IS_ERR_VALUE(roq_cm_queue_work(trans, ROQ_CM_WORK_RECV)))
			pr_info("roq_sock_server: cannot schedule work\n");
	}

close_out:
	sock_release(trans->sock);
	trans->sock = NULL;

out:
	pr_info("stopping roq_cm server thread\n");
	trans->thread_srv = NULL;
	trans->running = 0;
}

static int
roq_sock_client_create(void)
{
	struct socket	*s;
	unsigned long flags;
	int rv = 0;

	spin_lock_irqsave(&trans->con_lock, flags);

	if (trans->sock_snd)
		goto out;

	rv = sock_create_kern(PF_INET, SOCK_DGRAM, IPPROTO_UDP, &s);
	if (rv < 0)
		goto out;

	trans->sock_snd = s;
out:
	spin_unlock_irqrestore(&trans->con_lock, flags);

	if (rv < 0 && debug_cm)
		pr_info("Cannot create RoQ CM socket: %d\n", rv);

	return rv;
}


void
roq_cm_exit(void)
{
	int err;
	int i;

	spin_lock(&trans->con_lock);

	if (trans->sock_snd) {
		sock_release(trans->sock_snd);
		trans->sock_snd = NULL;
	}
	spin_unlock(&trans->con_lock);

	if (trans->thread_srv != NULL) {
		if (cm_workq) {
			flush_workqueue(cm_workq);
			destroy_workqueue(cm_workq);
		}
		err = send_sig(SIGKILL, trans->thread_srv, 1);

		/* wait for kernel thread to die */
		if (err < 0) {
			pr_info("unknown error %d while trying to "
				"terminate kernel thread\n", err);
		} else {
			while (trans->running == 1)
				msleep(10);

			pr_info("succesfully killed kernel thread\n");
		}
	}
	for (i = 0; i < RX_BUF_DEPTH; i++)
		kfree(trans->rxbuf[i]);

	kfree(trans->rxbuf);
	kfree(trans);

	/* XXX: more socket clean up here... */
	return;
}

int
roq_cm_init(void)
{
	struct net_device *dev;
	int rv, i;

	trans = kmalloc(sizeof(struct roq_sock_transport), GFP_KERNEL);
	if (trans == NULL) {
		pr_info("cannot alloc transport struct\n");
		return -ENOMEM;
	}
	memset(trans, 0, sizeof *trans);

	trans->rxbuf = kmalloc(RX_BUF_DEPTH * sizeof(char *), GFP_KERNEL);
	if (trans->rxbuf == NULL) {
		pr_info("cannot alloc rxbuffer\n");
		return -ENOMEM;
	}
	atomic_set(&trans->rx_free, RX_BUF_DEPTH);

	spin_lock_init(&trans->con_lock);

	for (i = 0; i < RX_BUF_DEPTH; i++) {
		trans->rxbuf[i] = kmalloc(RX_BUF_SIZE, GFP_KERNEL);
		if (trans->rxbuf[i] == NULL) {
			pr_warn("cannot alloc rxbuffer (i=%d)\n", i);
			return -ENOMEM;
		}
	}

	/* find roq device */
	dev = first_net_device(&init_net);
	while (dev != NULL) {
		if ((netif_running(dev))
		    && (dev->ip_ptr != NULL)
		    && (dev->dev_addr[0] == 0x00
		    && dev->dev_addr[1] == 0x80
		    && dev->dev_addr[2] == 0x47)) {
			struct in_device *id =
				(struct in_device *)(dev->ip_ptr);
			struct in_ifaddr *ia = id->ifa_list;

			pr_info(DRV_NAME": used device ... %s [%x]\n",
				dev->name,
				htonl(ia->ifa_address));
			break;
		}
		dev = next_net_device(dev);
	}

	trans->ndev = dev;

	if (dev == NULL) {
		printk(KERN_INFO DRV_NAME": no roq device found\n");
		return -ENODEV;
	}

	/*
	 * Init CM workqueue. One WQ should be sufficient and would enforce
	 * event processing to be ordered.
	 */ 
	cm_workq = create_singlethread_workqueue("roq_cm_wq");
	if (!cm_workq) {
		pr_info(KERN_INFO DRV_NAME": unable to start rq workq\n");
		return -ENOMEM;
	}
	/* create TX socket for outbound CM traffic */
	rv = roq_sock_client_create();
	if (rv < 0) {
		pr_info(KERN_INFO DRV_NAME" unable to create CM TX socket\n");
		return rv;
	}

	/* start kernel thread */
	trans->thread_srv = kthread_run((void *)roq_sock_server,
					trans, DRV_NAME);
	if (IS_ERR(trans->thread_srv)) {
		pr_warn(KERN_INFO DRV_NAME": unable to start kernel thread\n");
		kfree(trans->thread_srv);
		trans->thread_srv = NULL;
		return -ENOMEM;
	}
	return 0;
}


int
roq_connect(struct iw_cm_id *id, struct iw_cm_conn_param *params)
{
	struct roq_cm_msg	*ConReqMsg;

	struct roq_dev		*dev = roq_dev_ofa2roq(id->device);
	struct net_device	*ndev = dev->l2dev;
	struct roq_qp		*dd_qp = NULL;
	struct RoQ_DM_QP	*dm_qp = NULL;
	struct ib_qp_attr	qp_attr;
	enum ib_qp_attr_mask	qp_attr_mask;

	unsigned long flags;
	int size, rv = 0, DestRank;

	if (params->private_data_len > ROQ_MAX_PRIVATEDATA) {
		pr_info("roq_connect: Private data to long: %d\n",
			params->private_data_len);
		return -EINVAL;
	}
	size = params->private_data_len + sizeof *ConReqMsg;

	ConReqMsg = kzalloc(size, GFP_KERNEL);
	if (!ConReqMsg) {
		if (debug_cm)
			pr_info("roq_connect: cannot alloc msg\n");
		return -ENOMEM;
	}
	if (debug_cm)
		pr_info("QP[%d]: start connect\n", params->qpn);

	/* when source addr is NULL, change to the device address */
	if (id->local_addr.sin_addr.s_addr == 0)
		id->local_addr.sin_addr.s_addr =
		    ((struct in_device *) ndev->ip_ptr)->ifa_list->ifa_address;

	dm_qp = roq_get_dm_qp(dev, params->qpn);
	if (!dm_qp) {
		pr_warn("ROQ Connect: Invalid QP ID %d\n", params->qpn);
		return -EINVAL;
	}
	spin_lock_irqsave(&dm_qp->dd_lock, flags);

	BUG_ON(dm_qp->cm_id);

	dd_qp = roq_get_dd_qp(dev, params->qpn);
	spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

	BUG_ON(!dd_qp);

	if (id->remote_addr.sin_addr.s_addr == id->local_addr.sin_addr.s_addr) {
		if (debug_cm)
			pr_info("Loopback not supported\n");
		rv = -EADDRNOTAVAIL;
		goto out;
	}

	/* Handle the case where the RDMA crosses the ION-CN boundary */
	if ((id->remote_addr.sin_addr.s_addr ^ id->local_addr.sin_addr.s_addr)
		& 0x00c00000) {
		RoQ_IPV4Addr_To_Rank(
			&dev->RoQ->mNetworkDesc_rem,
			id->remote_addr.sin_addr.s_addr,
			&DestRank);
		DestRank |= 0x8000;
	} else
		RoQ_IPV4Addr_To_Rank(
			(struct RoQ_NetworkDescriptor *)&dev->RoQ->mNetworkDesc,
			id->remote_addr.sin_addr.s_addr,
			&DestRank);

	if (DestRank > RoQ_MAX_QP) {
		if (debug_cm)
			pr_info("QP[%d]: Connect: Destination node 0x%08X "
				"unreachable\n", roq_qpid(dd_qp),
				id->remote_addr.sin_addr.s_addr);
		rv = -EHOSTUNREACH;
		goto out;
	}
	dprint(DBG_CM, KERN_INFO,
	       "Dest Node Rank params->qpn %d dest ip addr %08X, dest rank %d",
	       params->qpn, id->remote_addr.sin_addr.s_addr, DestRank);

	qp_attr_mask  = IB_QP_STATE;
	qp_attr.qp_state = IB_QPS_RTR;
	qp_attr_mask |= IB_QP_AV;
	qp_attr.ah_attr.dlid = DestRank;

	rv = ib_modify_qp(&dd_qp->ofa_qp, &qp_attr, qp_attr_mask);
	if (rv) {
		if (debug_cm)
			pr_info("QP[%d]: ib_modify_qp failed\n",
				roq_qpid(dd_qp));
		goto out;
	}

	dm_qp->cm_id = id;
	id->add_ref(id);

	/* Prepare the connect request. */
	dd_qp->cm_msg = ConReqMsg;

	ConReqMsg->type = RoQ_CM_ConnRequest;
	ConReqMsg->active_qpn = params->qpn;

	ConReqMsg->local_addr	= id->local_addr.sin_addr.s_addr;
	ConReqMsg->remote_addr	= id->remote_addr.sin_addr.s_addr;
	ConReqMsg->local_port	= id->local_addr.sin_port;
	ConReqMsg->remote_port	= id->remote_addr.sin_port;

	memcpy(&ConReqMsg->params, params, sizeof *params);

	if (params->private_data_len)
		memcpy(ConReqMsg->private_data, params->private_data,
		       params->private_data_len);

	dd_qp->is_connecting = 1;
	dd_qp->active_connect = 1;

	/* start retransmission timer */
	rv = roq_cm_queue_work(dd_qp, ROQ_CM_WORK_CONREQ_TIMEOUT);
	if (rv) {
		dm_qp->cm_id = NULL;
		id->rem_ref(id);

		goto out;
	}

	rv = roq_cm_send(ConReqMsg, ConReqMsg->remote_addr, size);
	if (rv && rv != -ENOBUFS) {
		pr_info("roq_cm_send: connreq: error %d\n", rv);

		dm_qp->cm_id = NULL;
		id->rem_ref(id);

		rv = -EINVAL;
	} else
		rv = 0; /* Allow for retransmit attempt if -ENOBUFS */

	if (debug_cm)
		pr_info("QP[%d]: ConnReq sent: 0x%08X:%u "
			" -> 0x%08X:%u\n", roq_qpid(dd_qp),
			id->local_addr.sin_addr.s_addr,
			id->local_addr.sin_port,
			id->remote_addr.sin_addr.s_addr,
			id->remote_addr.sin_port);
out:
	if (rv && dd_qp) {
		kfree(dd_qp->cm_msg);
		dd_qp->cm_msg = NULL;
	}
	if (dd_qp)
		roq_qp_put_ref(&dd_qp->ofa_qp);

	if (debug_cm)
		pr_info("QP[%d]: done connect, rv = %d\n", params->qpn, rv);

	return rv;
}


/*
 * roq_accept - Let RoQ accept an RDMA connection request
 *
 * @id:		New connection management id to be used for accepted
 *		connection request
 * @params:	Connection parameters provided by ULP for accepting connection
 */
int
roq_accept(struct iw_cm_id *id, struct iw_cm_conn_param *params)
{
	struct roq_cm_msg	*ConResMsg = NULL;

	struct roq_dev		*dev = roq_dev_ofa2roq(id->device);
	struct roq_qp		*dd_qp = NULL;
	struct RoQ_DM_QP	*dm_qp = NULL;
	struct ib_qp_attr	qp_attr;
	enum ib_qp_attr_mask	qp_attr_mask;

	struct connreq_info	*req_info = id->provider_data;
	unsigned long 	flags;
	int		DestRank,
			size,
			rv = 0;

	if (debug_cm)
		pr_info("QP[%d]: start accept\n", params->qpn);

	if (params->private_data_len) {
		if (params->private_data_len > ROQ_MAX_PRIVATEDATA) {
			pr_info("roq_accept: Private data to long :%d\n",
				params->private_data_len);
			rv = -EINVAL;
			goto error;
		}
	}

	size = sizeof *ConResMsg + params->private_data_len;

	ConResMsg = kzalloc(size, GFP_KERNEL);
	if (!ConResMsg) {
		rv = -ENOBUFS;
		goto error;
	}

	spin_lock_irqsave(&dev->listen_lock, flags);
	list_del(&req_info->list);
	spin_unlock_irqrestore(&dev->listen_lock, flags);

	dm_qp = roq_get_dm_qp(dev, params->qpn);
	if (!dm_qp) {
		pr_warn("ROQ Accept: Invalid QP ID %u\n", params->qpn);
		rv = -EINVAL;
		goto error;
	}
	spin_lock_irqsave(&dm_qp->dd_lock, flags);
	BUG_ON(dm_qp->cm_id);

	dd_qp = roq_get_dd_qp(dev, params->qpn);
	BUG_ON(!dd_qp);

	dm_qp->cm_id = id;
	id->add_ref(id);

	spin_unlock_irqrestore(&dm_qp->dd_lock, flags);

	dprint_tsk(DBG_CM, KERN_INFO, "(id=0x%p, QP%d): Moving to RTS",
		   id, roq_qpid(dd_qp));

	/* Handle the case where the RDMA crosses the ION-CN boundary */
	if ((id->remote_addr.sin_addr.s_addr ^ id->local_addr.sin_addr.s_addr)
	    & 0x00c00000) {
		RoQ_IPV4Addr_To_Rank(
			(struct RoQ_NetworkDescriptor *)
				&dev->RoQ->mNetworkDesc_rem,
			id->remote_addr.sin_addr.s_addr,
			&DestRank);
		DestRank |= 0x8000;
	} else
		RoQ_IPV4Addr_To_Rank(
			(struct RoQ_NetworkDescriptor *)
				&dev->RoQ->mNetworkDesc,
			id->remote_addr.sin_addr.s_addr,
			&DestRank);

	if (DestRank > RoQ_MAX_QP) {
		if (debug_cm)
			pr_info("QP[%d]: Accept: Destination node 0x%08X "
				"unreachable\n", roq_qpid(dd_qp),
				id->remote_addr.sin_addr.s_addr);
		rv = -EHOSTUNREACH;
		goto error;
	}
	dprint(DBG_CM, KERN_INFO,
		"Dest Node Rank params->qpn %d dest ip addr %08X, "
		"dest rank %d, Dest qpn %lld",
		params->qpn, id->remote_addr.sin_addr.s_addr,
		DestRank, req_info->peer_qp);

	qp_attr_mask = IB_QP_STATE|IB_QP_AV|IB_QP_DEST_QPN;
	qp_attr.qp_state = IB_QPS_RTS;
	qp_attr.ah_attr.dlid = DestRank;
	qp_attr.dest_qp_num = req_info->peer_qp;

	rv = ib_modify_qp(&dd_qp->ofa_qp, &qp_attr, qp_attr_mask);
	if (rv)
		goto error;

	/* Prepare the connect reply. */
	dd_qp->cm_msg = ConResMsg;

	ConResMsg->type = RoQ_CM_ConnResponse;
	ConResMsg->active_qpn = req_info->peer_qp;
	ConResMsg->passive_qpn = roq_qpid(dd_qp);

	ConResMsg->local_addr	= id->local_addr.sin_addr.s_addr;
	ConResMsg->remote_addr	= id->remote_addr.sin_addr.s_addr;
	ConResMsg->local_port	= id->local_addr.sin_port;
	ConResMsg->remote_port	= id->remote_addr.sin_port;

	memcpy(&ConResMsg->params, params, sizeof *params);

	if (params->private_data_len)
		memcpy(ConResMsg->private_data, params->private_data,
			params->private_data_len);

	rv = roq_cm_send(ConResMsg, ConResMsg->remote_addr, size);
	if (!rv) {
		rv = roq_cm_upcall(dd_qp, dm_qp->cm_id,
				   IW_CM_EVENT_ESTABLISHED, 0);
		if (debug_cm && rv)
			pr_warn("event_handler returned %d\n", rv);

		dd_qp->is_connecting = 0;
		dd_qp->active_connect = 0;

		if (debug_cm)
			pr_info("QP[%d]: done accept\n", params->qpn);

		roq_qp_put_ref(&dd_qp->ofa_qp);

		return 0;
	} else {
		if (debug_cm)
			pr_info("QP[%d]: accept: error sending ConnReply: %d\n",
				roq_qpid(dd_qp), rv);
	}
error:
	kfree(req_info);

	if (dm_qp) {
		spin_lock_irqsave(&dm_qp->dd_lock, flags);
		if (dm_qp->cm_id) {
			dm_qp->cm_id->rem_ref(id);
			dm_qp->cm_id = NULL;
		}
		spin_unlock_irqrestore(&dm_qp->dd_lock, flags);
	}
	if (ConResMsg) {
		if (dd_qp)
			dd_qp->cm_msg = NULL;

		kfree(ConResMsg);
	}
	if (debug_cm)
		pr_info("RoQ Accept: error %d (id=0x%p, QP%d)",
			rv, id, roq_qpid(dd_qp));

	if (dd_qp)
		roq_qp_put_ref(&dd_qp->ofa_qp);

	if (debug_cm)
		pr_info("QP[%d]: accept failed: %d\n", params->qpn, rv);

	return rv;
}


/*
 * local connection reject case.
 * send private data back to peer and
 */
int
roq_reject(struct iw_cm_id *id, const void *pdata, u8 plen)
{
	struct roq_cm_msg	*ConRejMsg;
	struct connreq_info	*req_info = id->provider_data;
	unsigned long flags;
	int size, rv = 0;

	if (debug_cm)
		pr_info("CM Reject: Peer addr 0x%08X, port %u, QP[%d]\n",
			id->remote_addr.sin_addr.s_addr,
			id->remote_addr.sin_port,
			(int)(int64_t)id->provider_data);

	if (plen > ROQ_MAX_PRIVATEDATA) {
		pr_info("roq_reject: Private data to long: %d\n", plen);
		plen = 0;
	}

	size = sizeof *ConRejMsg + plen;
	ConRejMsg = kzalloc(size, GFP_KERNEL);
	if (!ConRejMsg) {
		pr_info("roq_reject: cannot allocate %d\n", size);
		rv = -ENOMEM;
		goto out;
	}

	ConRejMsg->type = RoQ_CM_ConnReject;
	ConRejMsg->active_qpn = req_info->peer_qp;
	ConRejMsg->params.private_data_len = plen;

	spin_lock_irqsave(&roq_device->listen_lock, flags);
	list_del(&req_info->list);
	spin_unlock_irqrestore(&roq_device->listen_lock, flags);

	kfree(req_info);

	ConRejMsg->local_addr	= id->local_addr.sin_addr.s_addr;
	ConRejMsg->remote_addr	= id->remote_addr.sin_addr.s_addr;
	ConRejMsg->local_port	= id->local_addr.sin_port;
	ConRejMsg->remote_port	= id->remote_addr.sin_port;

	if (pdata)
		memcpy(ConRejMsg->private_data, pdata, plen);

	rv = roq_cm_send(ConRejMsg, ConRejMsg->remote_addr, size);
	if (rv && debug_cm)
		pr_info("ROQ Reject: Unable to send message: %d\n", rv);
out:
	kfree(ConRejMsg);
	return rv;
}

/*
 * roq_create_listen - Create resources for a listener's IWCM ID @id
 */
int
roq_create_listen(struct iw_cm_id *id, int backlog)
{
	struct ib_device *ofa_dev = id->device;
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_dev);
	struct roq_listen_info *listen_info = NULL;
	unsigned long flags;
	int rc = 0;

	dprint_tsk(DBG_CM, KERN_INFO, "(id=0x%p): dev(id)=%s, l2dev=%s",
		   id, ofa_dev->name, dev->l2dev->name);

	dprint(DBG_CM, KERN_INFO,
	       "local addr 0x%08X local port %08X remote addr"
	       " %08X remote port %08X context %p event_handler@ 0x%p",
	       id->local_addr.sin_addr.s_addr, id->local_addr.sin_port,
	       id->remote_addr.sin_addr.s_addr, id->remote_addr.sin_port,
	       (void *) id->context, id->event_handler);

	spin_lock_irqsave(&dev->listen_lock, flags);

	/*
	 * make sure the dumb array map from listen port to listen_info
	 * structure isn't over run
	 */
	if (id->local_addr.sin_port > ROQ_LISTEN_PORT_COUNT) {
		dprint(DBG_CM, KERN_INFO,
		       "Local listen port out of range at %d (range is 0-%d)",
		       id->local_addr.sin_port, ROQ_LISTEN_PORT_COUNT);
		rc = -EINVAL;
		goto error;
	}

	/* make sure there isnt already a listen on this port */
	if (dev->ListenPortToPointer[id->local_addr.sin_port]) {
		dprint(DBG_CM, KERN_EMERG,
			"Local listen port already in use port# %d pointer "
			"is 0x%p",
			id->local_addr.sin_port,
			dev->ListenPortToPointer[id->local_addr.sin_port]);
		pr_info("create_listen: port %d  already in use\n",
			id->local_addr.sin_port);
		rc = -EINVAL;
		goto error;
	}

	listen_info = kzalloc(sizeof *listen_info, GFP_KERNEL);

	if (!listen_info) {
		rc = -ENOMEM;
		goto error;
	}

	spin_lock_init(&listen_info->lock);
	INIT_LIST_HEAD(&listen_info->conn_info);

	listen_info->cm_id = id;
	listen_info->dev = dev;
	id->add_ref(id);

	/* listen sequence not using provider data field at this time */
	BUG_ON(id->provider_data);

	/*
	 * for now, just give the asycn message handler the ability to
	 * find the listener cep -- no list
	 */
	dev->ListenPortToPointer[id->local_addr.sin_port] = (void *)listen_info;

	dprint(DBG_CM, KERN_INFO, "listening on port %d with listen_info@ 0x%p",
	       id->local_addr.sin_port, (void *) listen_info);

	spin_unlock_irqrestore(&dev->listen_lock, flags);

	return rc;

error:
	spin_unlock_irqrestore(&dev->listen_lock, flags);

	dprint(DBG_CM, KERN_INFO, "error return rc %d", rc);

	return rc;
}

static LIST_HEAD(orphaned_accepts);

int
roq_destroy_listen(struct iw_cm_id *id)
{
	struct ib_device *ofa_dev = id->device;
	struct roq_dev *dev = roq_dev_ofa2roq(ofa_dev);
	struct roq_listen_info *listen_info;
	unsigned long flags;

	BUG_ON(id->provider_data);

	spin_lock_irqsave(&dev->listen_lock, flags);

 	listen_info = dev->ListenPortToPointer[id->local_addr.sin_port];

	if (!list_empty(&listen_info->conn_info)) {
		pr_warn("destroy_listen: port %d: accept orphaned",
			id->local_addr.sin_port);
	//	list_splice(&listen_info->conn_info, &orphaned_accepts);
	} else {
		kfree(listen_info);

		dev->ListenPortToPointer[id->local_addr.sin_port] = NULL;
		id->rem_ref(id);
	}
	spin_unlock_irqrestore(&dev->listen_lock, flags);

	return 0;
}

int
roq_cm_upcall(struct roq_qp *dd_qp, struct iw_cm_id *cm_id,
	      enum iw_cm_event_type reason, int status)
{
	struct iw_cm_event  event;

	memset(&event, 0, sizeof event);
	event.status = status;
	event.event = reason;

	if (debug_cm) {
		struct RoQ_DM_QP *dm_qp = roq_get_dm_qp(roq_device,
							roq_qpid(dd_qp));
		pr_info("QP[%d], CM_ID 0x%p: CM upcall: status %d, "
			"reason %d, DM QP state %d, in_conn %d, active %d\n",
			roq_qpid(dd_qp), cm_id, status, reason,
			dm_qp ? dm_qp->qp_state:-1,
			dd_qp->is_connecting,
			dd_qp->active_connect);
	}
	return cm_id->event_handler(cm_id, &event);
}
