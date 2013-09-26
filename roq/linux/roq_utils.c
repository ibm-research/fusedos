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

#include <linux/errno.h>
#include <linux/types.h>
#include <net/tcp.h>

#include "roq.h"
#include "roq_cm.h"

#ifdef CONFIG_INFINIBAND_BGROQ_DEBUG
static int roq_debug = 1;
#else
static int roq_debug = 0;
#endif

#define NUM_BYTES_PER_LINE 16

/**
 * __roq_utils_mem_print
 *
 * Prints memory contents in hex, with NUM_BYTES_PER_LINE bytes
 * per line of output.
 */

void
__roq_utils_mem_print(char *mem_name, unsigned char *kva,
			  unsigned int num_bytes)
{
	int cnt, n, num_chars;
	int rem = num_bytes;
	/* for one line of printk output */
	char buf[2 * NUM_BYTES_PER_LINE + 1];
	char *p_str;

	pr_info("%s (%u bytes, in hex): \n", mem_name, num_bytes);

	while (rem > 0) {
		cnt = min(rem, NUM_BYTES_PER_LINE);
		p_str = buf;

		for (n = 0; n < cnt; n++) {
			num_chars = sprintf(p_str, "%02x", *kva++);
			p_str += num_chars;
		}
		pr_info("%s\n", buf);
		rem -= cnt;
	}
}

#undef BGROQ_UTILS_KVEC_PRINT_DATA


#ifndef BGROQ_UTILS_KVEC_PRINT_DATA

void
__roq_utils_kvec_print(char *vec_name, struct kvec *vec,
			   unsigned int num_elts)
{
	int i;
	struct kvec *p_vec = vec;

	for (i = 0; i < num_elts; i++) {
		pr_info("%s[%d].iov_base=0x%p, %s[%d].iov_len=%u\n",
			vec_name, i, p_vec->iov_base,
			vec_name, i, (uint32_t) p_vec->iov_len);
		p_vec++;
	}
}

#else

void
__roq_utils_kvec_print(char *vec_name, struct kvec *vec,
			   unsigned int num_elts)
{
	int i, rem, cnt, n, num_chars;

	struct kvec *p_vec;
	unsigned char *kva;
	/* for one line of printk output */
	char buf[2 * NUM_BYTES_PER_LINE + 1];
	char *p_str;

	p_vec = vec;

	for (i = 0; i < num_elts; i++) {
		pr_info("%s[%d].iov_base=0x%p, %s[%d].iov_len=%u\n",
			vec_name, i, p_vec->iov_base,
			vec_name, i, (uint32_t) p_vec->iov_len);
		p_vec++;
	}

	p_vec = vec;

	for (i = 0; i < num_elts; i++) {
		pr_info("%s[%d] (hex): \n", vec_name, i);

		kva = p_vec->iov_base;
		rem = p_vec->iov_len;

		while (rem > 0) {
			cnt = min(rem, NUM_BYTES_PER_LINE);
			p_str = buf;

			for (n = 0; n < cnt; n++) {
				/*
				 * Use unsigned char pointer to avoid printing
				 * more than two (i.e., 8) characters per byte
				 * if the most significant bit of *kva happens
				 * to be 1.
				 */
				num_chars = sprintf(p_str, "%02x", *kva++);
				p_str += num_chars;
			}
			pr_info("%s\n", buf);
			rem -= cnt;
		}
		p_vec++;
	}
}

#endif


void
__roq_print_qp_attr_mask(enum ib_qp_attr_mask attr_mask)
{
	if (!roq_debug)
		return;

	pr_info("------- qp attr mask ---\n");
	if (IB_QP_STATE & attr_mask)
		pr_info("IB_QP_STATE\n");
	if (IB_QP_CUR_STATE & attr_mask)
		pr_info("IB_QP_CUR_STATE\n");
	if (IB_QP_EN_SQD_ASYNC_NOTIFY & attr_mask)
		pr_info("IB_QP_EN_SQD_ASYNC_NOTIFY\n");
	if (IB_QP_ACCESS_FLAGS & attr_mask)
		pr_info("IB_QP_ACCESS_FLAGS\n");
	if (IB_QP_PKEY_INDEX & attr_mask)
		pr_info("IB_QP_PKEY_INDEX\n");
	if (IB_QP_PORT & attr_mask)
		pr_info("IB_QP_PORT\n");
	if (IB_QP_QKEY & attr_mask)
		pr_info("IB_QP_QKEY\n");
	if (IB_QP_AV & attr_mask)
		pr_info("IB_QP_AV\n");
	if (IB_QP_PATH_MTU & attr_mask)
		pr_info("IB_QP_PATH_MTU\n");
	if (IB_QP_TIMEOUT & attr_mask)
		pr_info("IB_QP_TIMEOUT\n");
	if (IB_QP_RETRY_CNT & attr_mask)
		pr_info("IB_QP_RETRY_CNT\n");
	if (IB_QP_RNR_RETRY & attr_mask)
		pr_info("IB_QP_RNR_RETRY\n");
	if (IB_QP_RQ_PSN & attr_mask)
		pr_info("IB_QP_RQ_PSN\n");
	if (IB_QP_MAX_QP_RD_ATOMIC & attr_mask)
		pr_info("IB_QP_MAX_QP_RD_ATOMIC\n");
	if (IB_QP_ALT_PATH & attr_mask)
		pr_info("IB_QP_ALT_PATH\n");
	if (IB_QP_MIN_RNR_TIMER & attr_mask)
		pr_info("IB_QP_MIN_RNR_TIMER\n");
	if (IB_QP_SQ_PSN & attr_mask)
		pr_info("IB_QP_SQ_PSN\n");
	if (IB_QP_MAX_DEST_RD_ATOMIC & attr_mask)
		pr_info("IB_QP_MAX_DEST_RD_ATOMIC\n");
	if (IB_QP_PATH_MIG_STATE & attr_mask)
		pr_info("IB_QP_PATH_MIG_STATE\n");
	if (IB_QP_CAP & attr_mask)
		pr_info("IB_QP_CAP\n");
	if (IB_QP_DEST_QPN & attr_mask)
		pr_info("IB_QP_DEST_QPN\n");
	pr_info("------------------------\n");
}

void
__roq_print_ib_wr_send(struct ib_send_wr *wr)
{
	struct ib_sge *sge;
	int i;

	if (!roq_debug)
		return;

	switch (wr->opcode) {

	case IB_WR_SEND:
		pr_info("SEND: ");
		break;

	case IB_WR_RDMA_WRITE:
		pr_info("WRITE: ");
		break;

	case IB_WR_RDMA_READ:
		pr_info("RREAD: ");
		break;

	default:
		pr_info("??%d: ", wr->opcode);

	}
	pr_info("__roq_print_ib_wr_send(): id=%llu, num_sge=%d, \n"
		"opcode=%d, flags=0x%04x, rem_addr=0x%016llx, rkey=0x%08x\n",
		(unsigned long long) wr->wr_id,
		wr->num_sge,
		wr->opcode,
		wr->send_flags,
		(unsigned long long)wr->wr.rdma.remote_addr,
		wr->wr.rdma.rkey);

	for (sge = wr->sg_list, i = 0; i < wr->num_sge; i++, sge++)
		pr_info("sge%d: addr=0x%016llx, len=%u, key=0x%08x\n",
			i, (unsigned long long) sge->addr,
			sge->length, sge->lkey);
}

void
__roq_print_ib_wr_recv(struct ib_recv_wr *wr)
{
	struct ib_sge *sge;
	int i;

	if (!roq_debug)
		return;

	printk(KERN_INFO
	       "__roq_print_ib_wr_recv(): id=0x%llu, num_sge=%d\n",
	       (unsigned long long) wr->wr_id, wr->num_sge);

	for (sge = wr->sg_list, i = 0; i < wr->num_sge; i++, sge++)
		pr_info("sge%d: addr=0x%016llx, len=%u, key=0x%08x\n",
			i, (unsigned long long) sge->addr,
			sge->length, sge->lkey);
}


void
__roq_print_umem(struct ib_umem *mem)
{
	struct ib_umem_chunk *chunk;
	int i, j;

	if (!roq_debug)
		return;

	pr_info("\n:::::::::::: __roq_print_umem(): start :::::\n");
	pr_info("length=%lu\toffset=%d\n", (unsigned long) mem->length,
		mem->offset);
	pr_info("chunklist::::\n");

	i = 0;
	list_for_each_entry(chunk, &mem->chunk_list, list) {
		pr_info("chunk%d: nent=%d, nmap=%d\n", i, chunk->nents,
			chunk->nmap);
		i++;
		for (j = 0; j < chunk->nents; j++) {

			struct scatterlist *sg = &chunk->page_list[j];

			pr_info("sg%d: page=%lx, offset=%d, dma=%llx, "
				"length=%d\n",
				j, sg->page_link, sg->offset,
				(unsigned long long) sg->dma_address,
				sg->length);
		}
	}
	printk(":::::::::::: __roq_print_umem(): end :::::::\n\n");
}

void
__roq_dump_bytes(void *buf, int len)
{
	unsigned char *addr = (unsigned char *) buf;
	int i = 0, j = 4;

	while (len--) {
		if (j == 4) {
			pr_info("\n%d.\t", i);
			j = 0;
		}
		pr_info("%02x", (unsigned char) *addr);
		addr += 1;
		if (((unsigned long) addr & 0x3) == 0) {
			pr_info("  ");
			i++;
			j++;
		}
	}
	pr_info("\n");
}
