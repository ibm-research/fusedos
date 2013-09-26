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

#ifndef _ROQ_CM_H
#define _ROQ_CM_H

#include <roq_device_interface.h>

extern bool debug_cm;

#define RX_BUF_SIZE	512
#define RX_BUF_DEPTH	1024

struct roq_sock_transport {
	struct socket		*sock;
	struct socket		*sock_snd;
	struct sockaddr_in	addr;
	struct task_struct	*thread_srv;
	struct net_device	*ndev;

	char			**rxbuf;

	atomic_t	rx_free;
	spinlock_t	con_lock;

	int clt_connect;
	int running;
};

#define ROQ_CM_TIMEOUT (HZ*5)
#define CM_MAX_RETRIES 5

enum roq_cm_work_type {
	ROQ_CM_WORK_RECV,
	ROQ_CM_WORK_SEND_MSG,
	ROQ_CM_WORK_CONREQ_TIMEOUT,
	ROQ_CM_WORK_CONRESP_TIMEOUT
};

struct roq_cm_work {
	enum roq_cm_work_type	type;
	struct delayed_work	work;
	union {
		struct roq_sock_transport *ctx;
		struct roq_qp *qp;
	};
};

struct roq_listen_info {
	struct iw_cm_id	*cm_id;
	spinlock_t	lock;
	struct roq_dev	*dev;
	/* list of pending connect requests */
	struct list_head conn_info;
};

struct connreq_info {
	struct list_head list;
	uint32_t	peer_qp;
	uint32_t	peer_addr;
	uint16_t	peer_port;
	char		pdata[0]; /* Variably length ConReq private data */
};

enum {
	RoQ_CM_ConnRequest,
	RoQ_CM_ConnResponse,
	RoQ_CM_ConnReject,
	RoQ_CM_Disconnect,
	RoQ_CM_Type_Count = RoQ_CM_Disconnect
};

struct roq_cm_msg {
	uint16_t	type;
	uint16_t	retry;

	/* Address information - to be extended for IPv6 */
	uint16_t	local_port;
	uint16_t	remote_port;
	uint32_t	local_addr;
	uint32_t	remote_addr;

	uint32_t	active_qpn;
	uint32_t	passive_qpn;

	struct iw_cm_conn_param	params;
	char private_data[0];
};

#define ROQ_MAX_PRIVATEDATA (RX_BUF_SIZE - sizeof(struct roq_cm_msg))

extern int roq_connect(struct iw_cm_id *, struct iw_cm_conn_param *);
extern int roq_accept(struct iw_cm_id *, struct iw_cm_conn_param *);
extern int roq_reject(struct iw_cm_id *, const void *, u8);
extern int roq_create_listen(struct iw_cm_id *, int);
extern int roq_destroy_listen(struct iw_cm_id *);

extern int roq_cm_upcall(struct roq_qp *, struct iw_cm_id *,
			 enum iw_cm_event_type, int);

extern void roq_listen_info_upcall(struct roq_listen_info *,
				   enum iw_cm_event_type);

extern void roq_listen_info_get(struct roq_listen_info *);
extern void roq_listen_info_put(struct roq_listen_info *);

extern int roq_cm_init(void);
extern void roq_cm_exit(void);


#endif
