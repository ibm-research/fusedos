/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef _BGQ_HW_ABSTRACTION_H
#define _BGQ_HW_ABSTRACTION_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef USERSPACE_ROQ
static inline int bluegene_getPersonality(void *buff, unsigned buffSize) { return 0;}

#define MUSPI_assert

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>

#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/kernel/memory.h>

#else

#define __KERNEL__
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>

#include <spi/include/kernel/firmware/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/process.h>
#include <spi/include/kernel/location.h>
#include <spi/include/kernel/firmware/location_impl.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/firmware/memory_impl.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/kernel/firmware/MU_impl.h>
#undef __KERNEL__

#endif /* USERSPACE_ROQ */

#ifdef __cplusplus
extern "C" {
#endif

int ROQ_dma_init(uint32_t *port_ids, int32_t nports);
static inline uint32_t ROQ_getnext_packet(int id, void *rfifo,
					  void **clientdata, int block);
int ROQ_Send(uint32_t iolink, uint32_t SendPortId,  void *MUHWI_DescPtr);
int ROQ_activate();
int ROQ_deactivate();
int ROQ_interrupt();
void ROQ_set_affinity(int m);
void *ROQ_RecFifo(int portid);
void *ROQ_ion_RecFifo(int portid);
int ROQ_ion_init(uint32_t* port_ids, int32_t nports);

#ifdef __cplusplus
}
#endif

extern int32_t firstLocalSubGrp; // base subgroup for FIFOs used for local traffic
extern int32_t firstLocalInjFifo;
extern int32_t firstLocalRecFifo;
extern int32_t firstLocalBaseAddressTable;

extern int32_t firstRemoteSubGrp; // base subgroup for FIFOS used for remote traffic
extern int32_t firstRemoteInjFifo;
extern int32_t firstRemoteRecFifo;
extern int32_t firstRemoteBaseAddressTable;

static uint32_t bytes_avail[4] = {0, 0, 0, 0};
static uint32_t bytes_proc[4]  = {0, 0, 0, 0};

static inline uint32_t
ROQ_getnext_packet(int id, void *rfifo, void **clientdata, int block)
{
	uint32_t wrap = 0;
	uint32_t cur_bytes = 0;
	uint32_t b = 0;

	if (bytes_proc[id] < bytes_avail[id]) {
		*clientdata = MUSPI_getNextPacketOptimized((MUSPI_RecFifo_t *)rfifo, &cur_bytes);
		bytes_proc[id] += cur_bytes;
	} else {

		if (bytes_proc[id] >= bytes_avail[id])
			MUSPI_syncRecFifoHwHead((MUSPI_RecFifo_t *)rfifo);

		if ((b = MUSPI_getAvailableBytes((MUSPI_RecFifo_t *)rfifo, &wrap)) != 0) {
			if (wrap == 0) {
				/* No fifo wrap.  Process each packet. */
				bytes_proc[id] = 0;
				bytes_avail[id] = b;
			} else {
				/*
				 * Packets wrap around to the top of the fifo.
				 * Handle the one packet that wraps.
				 */
				*clientdata = MUSPI_getNextPacketWrap((MUSPI_RecFifo_t *)rfifo, &cur_bytes);
				/* Store the updated fifo head. */
				bytes_proc[id] = 1;
				bytes_avail[id] = 0;
			}
		}
	}
	return cur_bytes;
}

static inline void
ROQ_getnext_packet_reset()
{
	int i;
	for (i = 0; i < 4; ++i) {
		bytes_avail[i] = 0;
		bytes_proc[i]  = 0;
	}
}

// THIS IS FROM CHRIS
#define load "qvlfdx "
#define store "qvstfdx "
#define vr(X) #X
#define nl "\n\t"
#define QVLFDXA(QRT,RA,RB)  .long (31<<26)|((QRT)<<21)|((RA)<<16)|((RB)<<11)|(583<<1)|1
#define QVSTFDXA(QRS,RA,RB) .long (31<<26)|((QRS)<<21)|((RA)<<16)|((RB)<<11)|(711<<1)|1
static inline void FastMove_512(unsigned long long  *to, unsigned long long *from)
{
	unsigned long k0x020 = 0x020 ;
	unsigned long k0x040 = 0x040 ;
	unsigned long k0x060 = 0x060 ;
	unsigned long k0x080 = 0x080 ;
	unsigned long k0x0a0 = 0x0a0 ;
	unsigned long k0x0c0 = 0x0c0 ;
	unsigned long k0x0e0 = 0x0e0 ;
	unsigned long k0x100 = 0x100 ;
	unsigned long k0x120 = 0x120 ;
	unsigned long k0x140 = 0x140 ;
	unsigned long k0x160 = 0x160 ;
	unsigned long k0x180 = 0x180 ;
	unsigned long k0x1a0 = 0x1a0 ;
	unsigned long k0x1c0 = 0x1c0 ;
	unsigned long k0x1e0 = 0x1e0 ;
	asm volatile(
			load vr(0) ",0,%[from]" nl
			load vr(2) ", %[k0x040],%[from]" nl
			load vr(4) ", %[k0x080],%[from]" nl
			load vr(6) ", %[k0x0c0],%[from]" nl
			load vr(8) ", %[k0x100],%[from]" nl
			load vr(10) ", %[k0x140],%[from]" nl
			load vr(12) ", %[k0x180],%[from]" nl
			load vr(14) ", %[k0x1c0],%[from]" nl
			store vr(0) ",0,%[to] " nl
			load vr(1) ", %[k0x020],%[from]" nl
			store vr(2) ", %[k0x040],%[to] " nl
			load vr(3) ", %[k0x060],%[from]" nl
			store vr(4) ", %[k0x080],%[to] " nl
			load vr(5) ", %[k0x0a0],%[from]" nl
			store vr(6) ", %[k0x0c0],%[to] " nl
			load vr(7) ", %[k0x0e0],%[from]" nl
			store vr(8) ", %[k0x100],%[to] " nl
			load vr(9) ", %[k0x120],%[from]" nl
			store vr(10) ", %[k0x140],%[to] " nl
			load vr(11) ", %[k0x160],%[from]" nl
			store vr(12) ", %[k0x180],%[to] " nl
			load vr(13) ", %[k0x1a0],%[from]" nl
			store vr(14) ", %[k0x1c0],%[to] " nl
			load vr(15) ", %[k0x1e0],%[from]" nl
			store vr(1) ", %[k0x020],%[to]" nl
			store vr(3) ", %[k0x060],%[to]" nl
			store vr(5) ", %[k0x0a0],%[to]" nl
			store vr(7) ", %[k0x0e0],%[to]" nl
			store vr(9) ", %[k0x120],%[to]" nl
			store vr(11) ", %[k0x160],%[to]" nl
			store vr(13) ", %[k0x1a0],%[to]" nl
			store vr(15) ", %[k0x1e0],%[to]" nl
			: /* Outputs ( we trample the 'to' and 'from' inputs ) */
			  [to] "+b" (to),
			  [from] "+b" (from)
			: /* Inputs */
			  [k0x020] "b" (k0x020),
			  [k0x040] "b" (k0x040),
			  [k0x060] "b" (k0x060),
			  [k0x080] "b" (k0x080),
			  [k0x0a0] "b" (k0x0a0),
			  [k0x0c0] "b" (k0x0c0),
			  [k0x0e0] "b" (k0x0e0),
			  [k0x100] "b" (k0x100),
			  [k0x120] "b" (k0x120),
			  [k0x140] "b" (k0x140),
			  [k0x160] "b" (k0x160),
			  [k0x180] "b" (k0x180),
			  [k0x1a0] "b" (k0x1a0),
			  [k0x1c0] "b" (k0x1c0),
			  [k0x1e0] "b" (k0x1e0)
			:  /* Clobblers (memory) */
			 "memory"
	) ;

}

#endif /* _BGQ_HW_ABSTRACTION_H */
