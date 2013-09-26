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

#include <linux/socket.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <net/net_namespace.h>
#include <asm/atomic.h>

//#include "InjFifo.h"

#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/BIC.h>
#include <firmware/include/personality.h>

#include <spi/include/kernel/klinux/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/MU.h>

#include <rdma/ib_verbs.h>
#include <roq_device_interface.h>
#include <roq.h>
#include <transcon.h>
#include <ionlink.h>

#define DRV_NAME  "roq_ion"

#define FIFO_CNT 1
#define SIZE (1024*1024)

struct roq_ion_ctx_t {
	struct task_struct *thread;
	Personality_t personality;
	uint32_t io_port;
	MUSPI_BaseAddressTableSubGroup_t  recAddressTable;
	Kernel_MemoryRegion_t rmregions[FIFO_CNT];
	Kernel_MemoryRegion_t imregions[FIFO_CNT];
	uint64_t *recvirt[FIFO_CNT];
	uint64_t *recdma[FIFO_CNT];
	uint64_t *injvirt[FIFO_CNT];
	uint64_t *injdma[FIFO_CNT];

	MUSPI_InjFifoSubGroup_t InjFifoSubgroups[1];
	MUSPI_RecFifoSubGroup_t RecFifoSubgroups[1];
	MUHWI_Descriptor_t desc[32] __attribute__ ((aligned (32)));

	int desc_cn_cnt;
	int desc_io_cnt;

	int running;
};

static struct roq_ion_ctx_t *ion_ctx;

inline uint32_t
MU_getnext_packet(void *rfifo, void **clientdata)
{
	uint32_t wrap = 0;
	uint32_t cur_bytes = 0;

	if (MUSPI_getAvailableBytes((MUSPI_RecFifo_t*)rfifo, &wrap) != 0) {
		if (wrap == 0) {
			/* No fifo wrap. Process each packet. */
			*clientdata = MUSPI_getNextPacketOptimized((MUSPI_RecFifo_t*)rfifo, &cur_bytes);
		} else {
			/* Packets wrap around to the top of the fifo. Handle the one packet
			* that wraps.
			*/
			*clientdata = MUSPI_getNextPacketWrap((MUSPI_RecFifo_t*)rfifo, &cur_bytes);
		}

		/* Store the updated fifo head. */
		MUSPI_syncRecFifoHwHead((MUSPI_RecFifo_t*)rfifo);
	}
	return cur_bytes;
}

static void
roq_ion_thread(struct roq_ion_ctx_t *ion_ctx)
{
	void *buf;
	int rc;

	/* kernel thread initialization */
	ion_ctx->running = 1;
	current->flags |= PF_NOFREEZE;

	/* daemonize (take care with signals, after daemonize() they are disabled) */
	daemonize(DRV_NAME);
	allow_signal(SIGKILL);

	/* main loop */
	for (;;) {
		while ((rc = MU_getnext_packet(MUSPI_IdToRecFifo(0, &ion_ctx->RecFifoSubgroups[0]), &buf)) <= 0) {
			if (signal_pending(current))
				goto out;
			schedule();
		}

		printk("%s:%d GOT PACKAGE (%d)\n", __func__, __LINE__, rc);
	}

out:
	ion_ctx->thread = NULL;
	ion_ctx->running = 0;
	return;
}

int
MUSPI_CreatePt2PtMemoryFIFODescriptor(MUHWI_Descriptor_t *desc, MUSPI_Pt2PtMemoryFIFODescriptorInfo_t *info)
{
	/* Set pointer to packet header portion of the descriptor. */
	MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt);

	/* Set pointer to message unit header within the packet header. */
	MUHWI_MessageUnitHeader_t *muh = &(desc->PacketHeader.messageUnitHeader);
	/* Clear the descriptor                    */
	MUSPI_DescriptorZeroOut(desc);
	/* Set pt2pt type */
	pkth->Data_Packet_Type = MUHWI_PT2PT_DATA_PACKET_TYPE;
	/* Set ABCD hints.       */
	pkth->Hints = info->Pt2Pt.Hints_ABCD;
	/* Set all byte2 fields at once. */
	pkth->Byte2.Byte2 = info->Pt2Pt.Misc1;
	/* Set all byte3 fields at once. */
	pkth->Byte3.Byte3 = info->Pt2Pt.Misc2;
	/* Set the destination.*/
	pkth->Destination = info->Base.Dest;
	/* Set packet type.        */
	pkth->Byte8.Byte8 = MUHWI_PACKET_TYPE_FIFO;

	/* Set number of packet chunks in first packet.          */
	pkth->Byte8.Size  = MUSPI_DescriptorGetNumPacketChunks ( info->Base.Message_Length );
	/* Set bytes to skip for checksum.    */
	pkth->Injection_Info.Skip = info->Pt2Pt.Skip;
	/* Set reception FIFO Id.          */
	muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id = info->MemFIFO.Rec_FIFO_Id;
	/* Set most significant 5 bits of the 37-bit put offset. */
	muh->Packet_Types.Memory_FIFO.Put_Offset_MSB = ( info->MemFIFO.Rec_Put_Offset >> 32 ) & 0x01f;
	/* Set least significant 32 bits of the 37-bit put offset.                */
	muh->Packet_Types.Memory_FIFO.Put_Offset_LSB = info->MemFIFO.Rec_Put_Offset & 0xFFFFFFFF;

	memcpy(muh->Packet_Types.Memory_FIFO.Unused2,info->MemFIFO.SoftwareBytes,sizeof(info->MemFIFO.SoftwareBytes)) ;
	/* Set pre-fetch-only indicator.     */
	desc->Half_Word0.Half_Word0 = info->Base.Pre_Fetch_Only;
	/* Set interrupt indicator.     */
	desc->Half_Word1.Half_Word1 = info->MemFIFO.Interrupt;
	/* Set payload physical address.  */
	desc->Pa_Payload = info->Base.Payload_Address;
	/* Set message length.   */
	desc->Message_Length = info->Base.Message_Length;
	/* Set torus fifo map.   */
	desc->Torus_FIFO_Map = info->Base.Torus_FIFO_Map;

	return 0;
}

void
roq_ion_desc_init(MUHWI_Descriptor_t *desc, int cn2ion)
{
	MUSPI_Pt2PtMemoryFIFODescriptorInfo_t info;
	memset(&info, 0, sizeof(MUSPI_Pt2PtMemoryFIFODescriptorInfo_t));

	if (cn2ion) {
		info.Pt2Pt.Hints_ABCD       = 0;
		info.Pt2Pt.Misc1            = MUHWI_PACKET_ROUTE_TO_IO_NODE;
		info.Pt2Pt.Misc2            = MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM;
		info.Pt2Pt.Skip             = 0;
		info.MemFIFO.Rec_FIFO_Id    = 1;
		info.MemFIFO.Rec_Put_Offset = -1;
		info.Base.Pre_Fetch_Only    = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		info.MemFIFO.Interrupt      = MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		info.MemFIFO.SoftwareBit    = 0;
		info.Base.Payload_Address   = (uint64_t)NULL;
		info.Base.Message_Length    = 0;
		info.Base.Torus_FIFO_Map    = 0;
		info.Base.Torus_FIFO_Map    = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;
	} else {
		info.Pt2Pt.Hints_ABCD       = 0;
		info.Pt2Pt.Misc1            = MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE | ion_ctx->io_port;
		info.Pt2Pt.Misc2            = MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM;
		info.Pt2Pt.Skip             = 0;
		info.MemFIFO.Rec_FIFO_Id    = 1;
		info.MemFIFO.Rec_Put_Offset = -1;
		info.Base.Pre_Fetch_Only    = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		info.MemFIFO.Interrupt      = MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		info.MemFIFO.SoftwareBit    = 0;
		info.Base.Payload_Address   = (uint64_t)NULL;
		info.Base.Message_Length    = 0;
		info.Base.Torus_FIFO_Map    = 0;
		info.Base.Torus_FIFO_Map    = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM;
	}

	MUSPI_CreatePt2PtMemoryFIFODescriptor(desc, &info);
}

void
roq_ion_send(struct roq_ion_send_t *s)
{
	MUHWI_Destination_t dest;
	MUHWI_Descriptor_t *desc;

	if (s->cn2ion) {
		desc = &ion_ctx->desc[(ion_ctx->desc_cn_cnt++ % 16) + 15];
	} else {
		desc = &ion_ctx->desc[(ion_ctx->desc_io_cnt++ % 16)];
	}

	MUSPI_SetPayload(desc, (uint64_t)s->dma_adr, s->len);
	MUSPI_SetRecPutOffset(desc, s->putoffset);

	MUSPI_SetUpDestination(&dest, s->dest[0], s->dest[1], s->dest[2], s->dest[3], s->dest[4]);
	MUSPI_SetDestination(desc, dest);

	printk("%s: dest=<%d,%d,%d,%d,%d> payload=0x%p len=%d putoffset=%x\n", __func__,
	       s->dest[0], s->dest[1], s->dest[2], s->dest[3], s->dest[4],
		   s->dma_adr, s->len, s->putoffset);

	MUSPI_SetRecFIFOId(desc, 16); // 4 * sgid + fifoid
	while ((MUSPI_InjFifoInject(MUSPI_IdToInjFifo(0, &ion_ctx->InjFifoSubgroups[0]), desc)) == -1);;
	while (MUSPI_CheckDescComplete(MUSPI_IdToInjFifo(0, &ion_ctx->InjFifoSubgroups[0]), 1) == 0);;
}

void
roq_ion_exit(void)
{
	int err;

	if (ion_ctx->thread != NULL) {
		err = send_sig(SIGKILL, ion_ctx->thread, 1);

		/* wait for kernel thread to die */
		if (err < 0) {
			printk(KERN_INFO DRV_NAME": unknown error %d while trying to terminate kernel thread\n", -err);
		} else {
			while (ion_ctx->running == 1)
				msleep(10);
			printk(KERN_INFO DRV_NAME": succesfully killed kernel thread!\n");
		}
	}
	kfree(ion_ctx);

	return;
}

int
roq_ion_init(void)
{
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup;
	Kernel_RecFifoAttributes_t *recFifoAttrs;
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup;
	Kernel_InjFifoAttributes_t *injFifoAttrs;
	Kernel_MemoryRegion_t *imregion;
	Kernel_MemoryRegion_t *rmregion;
	uint64_t recFifoEnableBits;
	int i, rc, nports, sgid;
	uint32_t port_ids[FIFO_CNT];

	sgid = 4;
	nports = FIFO_CNT;
	port_ids[0] = 0;

	ion_ctx = kmalloc(sizeof(struct roq_ion_ctx_t), GFP_KERNEL);
	if (ion_ctx == NULL) {
		printk(KERN_ERR DRV_NAME": cannot alloc context struct\n");
		return -ENOMEM;
	}
	memset(ion_ctx, 0, sizeof(struct roq_ion_ctx_t));

	for (i=0; i < nports; i++) {
		ion_ctx->recvirt[i] = (uint64_t*)kmalloc(SIZE, GFP_KERNEL);
		ion_ctx->recdma[i]  = (uint64_t*)((uint64_t)virt_to_phys((void*)ion_ctx->recvirt[i]));
		memset(ion_ctx->recvirt[i], 0, SIZE);
		ion_ctx->injvirt[i] = (uint64_t*)kmalloc(SIZE, GFP_KERNEL);
		ion_ctx->injdma[i]  = (uint64_t*)((uint64_t)virt_to_phys((void*)ion_ctx->injvirt[i]));
		memset(ion_ctx->injvirt[i], 0, SIZE);
	}

	rc = Kernel_AllocateBaseAddressTable(sgid, &ion_ctx->recAddressTable, nports, port_ids, 1);
	if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);

	/* allocate reception fifo */
	RecFifoSubgroup = &ion_ctx->RecFifoSubgroups[0];

	recFifoAttrs = (Kernel_RecFifoAttributes_t*)
	               kmalloc(nports*sizeof(Kernel_RecFifoAttributes_t), GFP_KERNEL);
	memset(recFifoAttrs, 0, nports*sizeof(Kernel_RecFifoAttributes_t));

	for (i=0; i < nports; i++) {
		recFifoAttrs[i].System = 1;
	}
	rc = Kernel_AllocateRecFifos(sgid, RecFifoSubgroup, nports, port_ids, recFifoAttrs);
	if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);
	kfree(recFifoAttrs);

	recFifoEnableBits = 0;
	for (i=0; i < nports; i++) {
		recFifoEnableBits |= (0x0000000000000001ULL << ( 15 - ( (sgid % BGQ_MU_NUM_FIFO_SUBGROUPS)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) - port_ids[i] ));
	}

	rc = Kernel_RecFifoEnable(sgid/BGQ_MU_NUM_FIFO_SUBGROUPS, recFifoEnableBits);
	if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);

	for (i=0; i < nports; i++) {
		rmregion = &ion_ctx->rmregions[i];

		rmregion->BaseVa = (uint8_t*)((uint64_t)ion_ctx->recvirt[i]);
		rmregion->BasePa = (uint8_t*)((uint64_t)ion_ctx->recdma[i]);
		rmregion->Bytes  = SIZE;

		rc = Kernel_RecFifoInit(RecFifoSubgroup, port_ids[i], rmregion, 0, SIZE-1);
		if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);
	}

	/* allocate injection fifo */
	InjFifoSubgroup = &ion_ctx->InjFifoSubgroups[0];
	injFifoAttrs = (Kernel_InjFifoAttributes_t*)
	               kmalloc(nports*sizeof(Kernel_InjFifoAttributes_t), GFP_KERNEL);
	memset(injFifoAttrs, 0, nports*sizeof(Kernel_InjFifoAttributes_t));

	for (i=0; i < nports; i++) {
		injFifoAttrs[i].RemoteGet = 0;
		injFifoAttrs[i].System    = 1;
	}
	rc = Kernel_AllocateInjFifos(sgid, InjFifoSubgroup, nports, port_ids, injFifoAttrs);
	if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);
	kfree(injFifoAttrs);

	for (i=0; i < nports; i++) {
		imregion = &ion_ctx->imregions[i];

		imregion->BaseVa = (uint8_t*)((uint64_t)ion_ctx->injvirt[i]);
		imregion->BasePa = (uint8_t*)((uint64_t)ion_ctx->injdma[i]);
		imregion->Bytes  = SIZE;

		rc = Kernel_InjFifoInit(InjFifoSubgroup, port_ids[i], imregion, 0, SIZE-1);
		if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);
	}

	rc = Kernel_InjFifoActivate(InjFifoSubgroup, nports, &port_ids[0], KERNEL_INJ_FIFO_ACTIVATE);
	if (rc != 0) printk("%s:%d (%d)\n", __func__, __LINE__, rc);

	bluegene_getPersonality(&ion_ctx->personality, sizeof(Personality_t));

	/* discover io port */
	if (ion_ctx->personality.Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_6){
		ion_ctx->io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6;
	}
	if (ion_ctx->personality.Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_7){
		ion_ctx->io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7;
	}
	if (ion_ctx->personality.Network_Config.NetFlags2 & ND_CN_BRIDGE_PORT_10){
		ion_ctx->io_port = MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT;
	}

	/* init descriptors */
	for (i=0; i < 32; ++i) {
		roq_ion_desc_init(&ion_ctx->desc[i], (i >= 15));
	}

	/* start kernel thread */
	ion_ctx->thread = kthread_run((void *)roq_ion_thread, ion_ctx, DRV_NAME);
	if (IS_ERR(ion_ctx->thread)) {
		printk(KERN_INFO DRV_NAME": unable to start kernel thread\n");
		kfree(ion_ctx->thread);
		ion_ctx->thread = NULL;
		return -ENOMEM;
	}

	return 0;
}
