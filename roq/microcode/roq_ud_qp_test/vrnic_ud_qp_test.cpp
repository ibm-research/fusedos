/*********************************************
 * (2010) Benjamin Krill <krill@de.ibm.com>
 *********************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

typedef int wait_queue_head_t;
#include <ib_verbs_dekernelized.h>
#include <roq_verbs_dekernelized.h>
#include <bgq_hw_abstraction.hpp>
#include <roq.hpp>
#include <FxLogger.hpp>
#include <roq_device_interface.h>
#include "roq.h"

#include "roq_verbs.h"

extern Personality_t *personality;


#define DRV_NAME          "roq_ud_qp"

//#define ROQ_UD_QP_DEBUG 1
//#define BUG_ON(a)

#ifdef ROQ_UD_QP_DEBUG
#  define PDEBUG(fmt, args...) printf(DRV_NAME": " fmt, ## args)
#else
#  define PDEBUG(fmt, args...)
#endif

#define MAX_TX 1
#define MAX_RX 1
//#define BUF_SIZE (65536*2)
//#define BUF_SIZE 16384
//#define BUF_SIZE 8192
#define BUF_SIZE 512

#define mftb()          ({unsigned long rval;   \
                        asm volatile("mftb %0" : "=r" (rval)); rval;})
static inline u64 get_tb(void)
{
        return mftb();
}

struct roq_ud_qp_priv {
	char *tx_buf;
	struct ib_sge send_sgl;
	struct ib_send_wr sq_wr;

	char *rx_buf;
	struct ib_sge recv_sgl;
	struct ib_recv_wr rq_wr;

	struct ib_device *ibdev;

	struct ib_pd     *kpd;
	struct ib_qp     **qps;
	struct ib_cq     *recv_cq;
	struct ib_cq     *send_cq;

	struct ib_wc     recv_wc;
	struct ib_wc     send_wc;

	int rank;
	int part_size;
};

#ifdef USERSPACE_ROQ
extern uint8_t *dmabuf;
#endif

struct roq_ud_pd_priv *ndev;

static int
roq_ud_qp_post_recv(struct roq_ud_qp_priv *priv, int src_rank, char *buf, int len)
{
	struct ib_recv_wr *bad_wr;
	int ret;

	priv->recv_sgl.addr = (uint64_t)buf;
	priv->recv_sgl.length = len;
	priv->recv_sgl.lkey = 0;
	priv->rq_wr.wr_id = src_rank;
	priv->rq_wr.sg_list = &priv->recv_sgl;
	priv->rq_wr.num_sge = 1;

	while ((ret = roq_post_receive(priv->qps[src_rank], &priv->rq_wr, &bad_wr)) != 0)
		printf(DRV_NAME": cannot post recv buffer [%d]\n", ret);

	return 0;
}

void
roq_ud_qp_rx_ib_compl(struct roq_ud_qp_priv *priv)
{
	int frames_waiting = 0, i;

	while (frames_waiting == 0) {
		frames_waiting = roq_poll_cq(priv->recv_cq, MAX_RX, &priv->recv_wc);
		if (IS_ERR_VALUE(frames_waiting)) {
			printf(DRV_NAME": cq poll error\n");
			frames_waiting = 0;
		}

		for (i=0; i < frames_waiting; i++) {
			PDEBUG("RX DONE\n");
		}
	}
}

void
roq_ud_qp_tx_ib_compl(struct roq_ud_qp_priv *priv)
{
	int n = 0, i;

	while (n == 0) {
		n = roq_poll_cq(priv->send_cq, MAX_TX, &priv->send_wc);
		for (i=0; i < n; i++) {
			/* FREE the buffer again */
			PDEBUG("TX DONE\n");
		}
	}
}

static int
roq_ud_qp_tx(struct roq_ud_qp_priv *priv, int dest_rank, char *buf, int len)
{
	struct ib_send_wr *bad_wr;
	int ret;

	/* POST THE SEND */
	priv->send_sgl.addr = (uint64_t)(unsigned long)buf;
	priv->send_sgl.length = len;
	priv->send_sgl.lkey = 0;

	priv->sq_wr.opcode = IB_WR_SEND;
	priv->sq_wr.send_flags = IB_SEND_FENCE;
	priv->sq_wr.sg_list = &priv->send_sgl;
	priv->sq_wr.num_sge = 1;

	PDEBUG("POST SEND\n");
	ret = roq_post_send(priv->qps[dest_rank], &priv->sq_wr, &bad_wr);
	if (IS_ERR_VALUE(ret))
		printf(DRV_NAME": ERROR post send - FIXME: re-inject request\n");

	return ret;
}

static void
roq_ud_qp_destroy_qp(struct roq_ud_qp_priv *priv)
{
	int i;

	for (i=0; i < priv->part_size; i++) {
		roq_destroy_qp(priv->qps[i]);
	}
	free(priv->qps);
	roq_dealloc_pd(priv->kpd);
	roq_destroy_cq(priv->send_cq);
	roq_destroy_cq(priv->recv_cq);
	return;
}

static int
roq_ud_qp_init_qp(struct roq_ud_qp_priv *priv)
{
	struct ib_device *ibdev = priv->ibdev;
	struct ib_qp_init_attr create_qp_attrs;
	struct ib_qp_attr qp_attr;
	enum ib_qp_attr_mask qp_attr_mask;
	int i;

	/* create completion queues */
	priv->send_cq = roq_create_cq(ibdev, 1, 0, NULL, NULL);
//	priv->send_cq = roq_create_cq(ibdev, roq_ud_qp_tx_ib_compl, NULL,
//	                             priv, 1, 0);
	create_qp_attrs.send_cq = priv->send_cq;
	BUG_ON( !create_qp_attrs.send_cq );

	priv->recv_cq = roq_create_cq(ibdev, 1, 0, NULL, NULL);
//	priv->recv_cq = ib_create_cq(ibdev, roq_ud_qp_rx_ib_compl, NULL,
//	                             priv, 1, 0);
	create_qp_attrs.recv_cq = priv->recv_cq;
	BUG_ON( !create_qp_attrs.recv_cq );

	/* set some more parameters */
	create_qp_attrs.qp_type       = IB_QPT_UD;
	create_qp_attrs.event_handler = NULL;
	create_qp_attrs.qp_context    = NULL;
	create_qp_attrs.srq           = NULL;

	create_qp_attrs.cap.max_send_wr      = 1;
	create_qp_attrs.cap.max_recv_wr      = 1;
	create_qp_attrs.cap.max_send_sge     = 1;
	create_qp_attrs.cap.max_recv_sge     = 1;
	create_qp_attrs.cap.max_inline_data  = 0;

	/* allocate protection domain and qp array */
	priv->kpd = roq_alloc_pd(ibdev, NULL, NULL);
	BUG_ON( !priv->kpd );

	priv->qps = (struct ib_qp**)malloc(sizeof(struct ib_qp*) * priv->part_size);
	BUG_ON(!priv->qps);

	for (i=0; i < priv->part_size; i++) {
		priv->qps[i] = roq_create_qp(priv->kpd, &create_qp_attrs, NULL);
		priv->qps[i]->device = priv->ibdev; // hack: ofa stack normally sets the device
		if (IS_ERR(priv->qps[i])) {
			printf(DRV_NAME ": error creating qp %p %d\n", priv->qps[i], i);
			return PTR_ERR(priv->qps[i]);
		}
	}

	for (i=0; i < priv->part_size; i++) {
		qp_attr_mask          = (enum ib_qp_attr_mask)(IB_QP_STATE | IB_QP_AV | IB_QP_DEST_QPN);;
		qp_attr.qp_state      = IB_QPS_RTS;
		/* this qp will send to peer rank i (zero based) */
		qp_attr.ah_attr.dlid  = i;
		/* this qp will send to peer qp num rank + 1 (qp zero is reserved) */
		qp_attr.dest_qp_num   = priv->rank + 1;

		roq_ofed_modify_qp(priv->qps[i], &qp_attr, qp_attr_mask, NULL);
	}

	return 0;
}

int
roq_ud_qp_main(int rank, int part_size, void* bdev)
{
	int ret, i;
	uint64_t start_tb, end_tb;

	printf( DRV_NAME": %s %s %d %d\n", __DATE__, __TIME__, rank, part_size);

	struct roq_ud_qp_priv *priv = (struct roq_ud_qp_priv *)malloc(sizeof(struct roq_ud_qp_priv));
	if (!priv)
		return -1;

	bzero(priv, sizeof(struct roq_ud_qp_priv));

	priv->rank = rank;
	priv->part_size = part_size;
	priv->ibdev = &((struct roq_dev*)bdev)->ofa_dev;

#ifdef USERSPACE_ROQ
	priv->rx_buf = (char*)dmabuf;
	priv->tx_buf = (char*)dmabuf;
#else
	priv->rx_buf = (char*)malloc(BUF_SIZE);
	priv->tx_buf = (char*)malloc(BUF_SIZE);
#endif

	if ((ret = roq_ud_qp_init_qp(priv)) != 0)
			return ret;

	switch (rank) {
		case 0:
			for (i=0; i < BUF_SIZE/sizeof(uint32_t); i++)
				((uint32_t*)priv->tx_buf)[i] = i;

			sleep(10);

			/* we will get an package on qp 1! */
			roq_ud_qp_post_recv(priv, 1, priv->rx_buf, BUF_SIZE);

			start_tb = get_tb();
			/* send package to rank 1 */
			roq_ud_qp_tx(priv, 1, priv->tx_buf, BUF_SIZE);

			/* wait till tx is done */
			roq_ud_qp_tx_ib_compl(priv);

			/* wait till rx is done */
			roq_ud_qp_rx_ib_compl(priv);

			end_tb = get_tb();

			printf("TX END start=%llx end=%llx size=%d 1600 MHz\n", start_tb,
					end_tb,1*BUF_SIZE);

			//roq_ud_qp_destroy_qp(priv);
			break;
		case 1:
			/* we will get an package on qp 0! */
			roq_ud_qp_post_recv(priv, 0, priv->rx_buf, BUF_SIZE);

			/* wait till rx is done */
			roq_ud_qp_rx_ib_compl(priv);

			/* send ack */
			roq_ud_qp_tx(priv, 0, priv->tx_buf, BUF_SIZE);

			/* wait till tx is done */
			roq_ud_qp_tx_ib_compl(priv);

			/* Only display the first and last lines of the buffer */
			for (i=0; i < 0x10 ; i++) {
				printf("%02x ", priv->rx_buf[i]);
			}
			printf("\n  ..\n");
			for (i=BUF_SIZE-0x10; i <  BUF_SIZE ; i++) {
				printf("%02x ", priv->rx_buf[i]);
			}
			printf("\n");

			printf("RX END\n");

			//roq_ud_qp_destroy_qp(priv);
			break;
	}

	//free(priv);
	return 0;
}

void *roq_verbs_init(void *vdev_mem);
void *test_case(void* vdev_mem)
{
	BegLogLine(1) << __FUNCTION__ << "()" << " STARTED " << EndLogLine;

	SRSW_QueueSet_WriterConnect((union SRSW_QueueSet_t *) &RoQ_DevMem->mTxDoorbellQueueSet[0],    // Queue Set pointer
                    0, DD_DoorbellQueue_Size, NULL, sizeof(u32));

	int part_size = ND_TORUS_SIZE(personality->Network_Config);

	void* bla = roq_verbs_init(vdev_mem);
	if (!bla)
		return 0;

	roq_ud_qp_main(RoQ_LocalRank, part_size, bla);

	BegLogLine(1)  << __FUNCTION__ << "()" << " BYEBYE " << EndLogLine;
	while(1);
}

/* ROQ START AND THREAD */
void RoQ_MC_Init();
int main(void)
{
	printf("START ROQ UD QP TEST\n");
	while(1);
	RoQ_MC_Init();
}
