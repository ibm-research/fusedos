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

#ifndef _TRANSCON_H
#define _TRANSCON_H

// need to control these values to fit within the memory allocated to the ROQ domain
// BGF: these values are being reduced as we add BGQ MU Get capabilty. Long standing values commented out.
#define REC_MEMORY_FIFO_SIZE     ((50 * 1024 * 1024)-1)    //  ((100 * 1024 * 1024)-1)
#define INJ_MEMORY_FIFO_SIZE     ((5 * 1024 * 1024)-1)    //  ((40 * 1024 * 1024)-1)

#define ROQ_VIRT_CACHE_OFF   0xB000000000ULL
#define ROQ_VIRT_MUCACHE_OFF 0xD000000000ULL
#define ROQ_VIRT_UNCACHE_OFF 0xC000000000ULL

#define MAX_NODE_COUNT 1024

#define NPORTS 35 /* 1 for TX, 1 for RX, 33 for BGQ MU DMA Gets */

struct roq_us_map_t {
	unsigned int magic;
	unsigned int nports;
#if defined(USERSPACE_ROQ) || defined(USERSPACE_EXTENSION)
	MUSPI_RecFifoSubGroup_t RecFifoSubgroups[1];
#else
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroups;
#endif
	void *rec_memory_fifos[NPORTS]; //[REC_MEMORY_FIFO_SIZE + 128]; // 128 for alignment
	void *alloc_rec_bufs[NPORTS];
	Kernel_MemoryRegion_t rmregions[NPORTS];
	uint8_t *rec_dmabuf[NPORTS];

#if defined(USERSPACE_ROQ) || defined(USERSPACE_EXTENSION)
	MUSPI_InjFifoSubGroup_t InjFifoSubgroups[1];
#else
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroups;
#endif
  MUSPI_BaseAddressTableSubGroup_t  recAddressTable; // Introduced for DMA suppport
	void *inj_memory_fifos[NPORTS]; //[INJ_MEMORY_FIFO_SIZE + 128]; // 128 for alignment
	void *alloc_inj_bufs[NPORTS];
	Kernel_MemoryRegion_t imregions[NPORTS];
	uint8_t *inj_dmabuf[NPORTS];

	int  trans_idx;
	void *mu_ackdesc[MAX_NODE_COUNT];
	void *mu_ackdesc_dma[MAX_NODE_COUNT];
	void *mu_burstdesc[MAX_NODE_COUNT];
	void *mu_burstdesc_dma[MAX_NODE_COUNT];
	void *tx_burstdesc[MAX_NODE_COUNT];
	void *tx_burstdesc_dma[MAX_NODE_COUNT];
	void *rx_burstdesc[MAX_NODE_COUNT];
	void *rx_burstdesc_dma[MAX_NODE_COUNT];

	uint64_t *virtbuf;
	uint64_t *dmabuf;

	/* REMOTE/IO-LINK */
	struct {
		uint32_t io_port;
		uint64_t *rec_virt[NPORTS];
		uint64_t *rec_dma[NPORTS];
		uint64_t *inj_virt[NPORTS];
		uint64_t *inj_dma[NPORTS];
		Kernel_MemoryRegion_t rmregions[NPORTS];
		Kernel_MemoryRegion_t imregions[NPORTS];
		MUSPI_InjFifoSubGroup_t *InjFifoSubgroups;
		MUSPI_RecFifoSubGroup_t *RecFifoSubgroups;
		MUSPI_BaseAddressTableSubGroup_t  recAddressTable;
		int  trans_idx;
		void *mu_ackdesc[MAX_NODE_COUNT];
		void *mu_ackdesc_dma[MAX_NODE_COUNT];
		void *mu_burstdesc[MAX_NODE_COUNT];
		void *mu_burstdesc_dma[MAX_NODE_COUNT];
		void *tx_burstdesc[MAX_NODE_COUNT];
		void *tx_burstdesc_dma[MAX_NODE_COUNT];
		void *rx_burstdesc[MAX_NODE_COUNT];
    void *rx_burstdesc_dma[MAX_NODE_COUNT];
	} rem;
};

extern struct device *bg_mu_dev;
#endif /* _TRANSCON_H */
