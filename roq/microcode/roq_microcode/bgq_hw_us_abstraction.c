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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/eventfd.h>
#include <asm/types.h>
#include <bgq_hw_abstraction.h>
#include <errno.h>

#define __KERNEL__
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/mu_dcr.h>

#include <spi/include/kernel/firmware/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
//#include <firmware/include/SPI_syscalls.h>
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
#include <hwi/include/bqc/gea_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <spi/include/mu/Reset.h>
#undef __KERNEL__

#include <roq_kernel.h>
#include <transcon.h>
#include <roq_fw.h>

#define FXLOG_REMOTE_GET 1

/**** STATICS ***********************************************************/
struct roq_us_map_t _roq_us_map;
struct roq_us_map_t *roq_us_map = &_roq_us_map;
uint64_t *dmabuf;
volatile BIC_Device* _puea = (BIC_Device*) BIC_DEVICE_BASE_ADDRESS;

 int32_t firstLocalSubGrp; // base subgroup for FIFOs used for local traffic
 int32_t firstLocalInjFifo;
 int32_t firstLocalRecFifo;
 int32_t firstLocalBaseAddressTable;

 int32_t firstRemoteSubGrp; // base subgroup for FIFOS used for remote traffic
 int32_t firstRemoteInjFifo;
 int32_t firstRemoteRecFifo;
 int32_t firstRemoteBaseAddressTable;

/**** FUNCTIONS *********************************************************/
void *
ROQ_RecFifo(int portid)
{
    DPRINTF(FXLOG_UCODE_INIT,"ROQ_RecFifo portid=%d\n",portid) ;
	return (void*)MUSPI_IdToRecFifo(portid, &(roq_us_map->RecFifoSubgroups)[0]);
}

void *
ROQ_ion_RecFifo(int portid)
{
    DPRINTF(FXLOG_UCODE_INIT,"ROQ_ion_RecFifo portid=%d\n",portid) ;
	return (void*)MUSPI_IdToRecFifo(portid, &(roq_us_map->rem.RecFifoSubgroups)[0]);
}

void
ROQ_set_affinity(int m)
{
}

int
ROQ_BGQ_MU_Torus_init(uint32_t* port_ids, uint32_t* RemoteGetEnableFlags, uint32_t* SystemFIFO, int32_t nports, int32_t nrcvfifos )
{
	int rc;
	int i;
	uint64_t recFifoEnableBits;
	Kernel_MemoryRegion_t *rmregion;
	Kernel_MemoryRegion_t *imregion;
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup;
	Kernel_RecFifoAttributes_t recFifoAttrs[8];
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup;
	Kernel_InjFifoAttributes_t injFifoAttrs[8];
#if 1 // like this is old debug scafolding and isn't needed at all
	//roq_us_map->virtbuf = (uint64_t*)malloc(sizeof(uint64_t) * RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT);
	roq_us_map->virtbuf = (uint64_t*)malloc(sizeof(uint64_t));
	roq_us_map->dmabuf = (uint64_t*)(ROQ_VIRT_CACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->virtbuf & ~ROQ_VIRTUAL_MAPPING));
#endif

	int injSubgroupCount = ( nports+7 ) / 8 ;

	roq_us_map->trans_idx = 0;
	for (i=0; i < MAX_NODE_COUNT; i++) {
		roq_us_map->mu_ackdesc[i]       = malloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT);
		roq_us_map->mu_ackdesc_dma[i]   = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->mu_ackdesc[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->mu_burstdesc[i]     = malloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
		roq_us_map->mu_burstdesc_dma[i] = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->mu_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->tx_burstdesc[i]     = malloc(sizeof(struct RoQ_TX_BurstDescriptor)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
		roq_us_map->tx_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->tx_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->rx_burstdesc[i]     = malloc(sizeof(struct RoQ_RX_BurstAckDescriptor)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
		roq_us_map->rx_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->rx_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
	}
	for (i=0; i < nports; i++) {
		roq_us_map->inj_memory_fifos[i] = (void*)(((uint64_t)malloc(INJ_MEMORY_FIFO_SIZE+64) + 64) & ~(64-1));
		roq_us_map->inj_dmabuf[i] = (uint8_t*)(domain_physddr_start() + ((uint64_t)roq_us_map->inj_memory_fifos[i] & ~ROQ_VIRTUAL_MAPPING));
		if(i < nrcvfifos)
		  {
		    roq_us_map->rec_memory_fifos[i] = (uint8_t*)(((uint64_t)malloc(REC_MEMORY_FIFO_SIZE+32) + 32) & ~(32-1));
		    roq_us_map->rec_dmabuf[i] = (uint8_t*)(domain_physddr_start() + ((uint64_t)roq_us_map->rec_memory_fifos[i] & ~ROQ_VIRTUAL_MAPPING));
		  }
	}

	/* Receive side init */
	roq_us_map->RecFifoSubgroups = (MUSPI_RecFifoSubGroup_t*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)malloc(sizeof(MUSPI_RecFifoSubGroup_t)) & ~ROQ_VIRTUAL_MAPPING));
	RecFifoSubgroup = &roq_us_map->RecFifoSubgroups[0];

	for (i=0; i < nrcvfifos; i++) {
		memset(&recFifoAttrs[i], 0, sizeof(recFifoAttrs[i]));
		recFifoAttrs[i].System = 0;
	}

	// leave subgroups 64 and 65 for PAMI
	// leave subgroups 66 and 67 for bgvrnic / CNK
	firstLocalSubGrp = BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE - injSubgroupCount - 4;
	firstLocalRecFifo = firstLocalSubGrp * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP + port_ids[0];

	DPRINTF(FXLOG_UCODE_INIT, "Allocate %d RecFifos in subgrp %d, firstLocalRecFifo %d\n",
		nrcvfifos, firstLocalSubGrp, firstLocalRecFifo);
	rc = Kernel_AllocateRecFifos(firstLocalSubGrp, RecFifoSubgroup, nrcvfifos, &port_ids[0], recFifoAttrs);

	recFifoEnableBits = 0;
	for (i=0; i < nrcvfifos; i++)
		recFifoEnableBits |= (0x0000000000000001ULL << ( 15 - ( (firstLocalSubGrp % BGQ_MU_NUM_FIFO_SUBGROUPS)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) - port_ids[i] ));
	DPRINTF(FXLOG_UCODE_INIT, "%s: recFifoEnableBits %lx for subgroup %d in group %d\n",
		__func__, recFifoEnableBits, firstLocalSubGrp,
		firstLocalSubGrp/BGQ_MU_NUM_FIFO_SUBGROUPS);
	rc = Kernel_RecFifoEnable(firstLocalSubGrp/BGQ_MU_NUM_FIFO_SUBGROUPS,
		recFifoEnableBits);

	for (i=0; i < nrcvfifos; i++) {
		rmregion = &roq_us_map->rmregions[i];

		rmregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->rec_memory_fifos[i]);
		rmregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->rec_dmabuf[i]);
		rmregion->Bytes  = REC_MEMORY_FIFO_SIZE;

		rc = Kernel_RecFifoInit(RecFifoSubgroup, port_ids[i], rmregion, 0, REC_MEMORY_FIFO_SIZE);
	}
	// Set up base address for counters and receive DMA
  uint32_t batids[1] = {0};
  firstLocalBaseAddressTable = firstLocalSubGrp * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP + batids[0];
  DPRINTF(FXLOG_UCODE_INIT, "AllocateBaseAddressTable in subgrp %d, BAT %d\n",
		firstLocalSubGrp, firstLocalBaseAddressTable);
  rc=Kernel_AllocateBaseAddressTable ( firstLocalSubGrp,
      &roq_us_map->recAddressTable,
           1,
           batids,
           0  // Indicate that it is for 'user' use
           );
  DASSERT(0==rc,"Kernel_AllocateBaseAddressTable rc=%d\n",rc) ;
  DPRINTF(FXLOG_UCODE_INIT,"Kernel_AllocateBaseAddressTable rc=%d\n",rc) ;

  rc=MUSPI_SetBaseAddress(&roq_us_map->recAddressTable,0,0) ;
  DASSERT(0==rc,"MUSPI_SetBaseAddress rc=%d\n",rc) ;
  DPRINTF(FXLOG_UCODE_INIT,"MUSPI_SetBaseAddress rc=%d\n",rc) ;

	/* Injection side init */
	/* allocate injection fifo */
	roq_us_map->InjFifoSubgroups = (MUSPI_InjFifoSubGroup_t*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)malloc(injSubgroupCount*sizeof(MUSPI_InjFifoSubGroup_t)) & ~ROQ_VIRTUAL_MAPPING));
	InjFifoSubgroup = &roq_us_map->InjFifoSubgroups[0];

//	for (i=0; i < nports; i++) {
//		//memset(&injFifoAttrs[i], sizeof(injFifoAttrs[i]));
////		injFifoAttrs[i].RemoteGet = RemoteGetEnableFlags[i];
////		injFifoAttrs[i].System    = 0;
//	    Kernel_InjFifoAttributes_t iFifoAttrs[1] ;
//	    uint32_t p_ids[1] ;
//	    iFifoAttrs[0].RemoteGet = RemoteGetEnableFlags[i];
//	    iFifoAttrs[0].System    = 0;
//	    p_ids[0] = i&7 ;
//	    rc=Kernel_AllocateInjFifos(i/8,InjFifoSubgroup+i/8,1,p_ids,iFifoAttrs) ;
//
//	    printf("Kernel_AllocateInjFifos i=%d rc=%d\n",i,rc) ; fflush(stdout) ;
////	    imregion = &roq_us_map->imregions[i];
////
////	    imregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->inj_memory_fifos[i]);
////	    imregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->inj_dmabuf[i]);
////	    imregion->Bytes  = INJ_MEMORY_FIFO_SIZE+1;
////
////	    rc=Kernel_InjFifoInit(InjFifoSubgroup+i/8, i&7, imregion, 0, INJ_MEMORY_FIFO_SIZE);
////      printf("Kernel_InjFifoInit i=%d rc=%d\n",i,rc) ; fflush(stdout) ;
////
//
////	    rc = Kernel_InjFifoActivate(InjFifoSubgroup+i/8, 1,p_ids, KERNEL_INJ_FIFO_ACTIVATE);
////      printf("Kernel_InjFifoActivate i=%d rc=%d\n",i,rc) ; fflush(stdout) ;
//	}

	firstLocalInjFifo = firstLocalSubGrp * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP;

	for(i=0;i<nports;i+=8)
	  {
	    Kernel_InjFifoAttributes_t iFifoAttrs[8];
	    uint32_t p_ids[8] = {0,1,2,3,4,5,6,7} ;
	    int j ;
	    for(j=0;j<8;j+=1)
	      {
	        if(i+j<nports)
	          {
	            iFifoAttrs[j].RemoteGet = RemoteGetEnableFlags[i+j] ;
	            iFifoAttrs[j].System    = SystemFIFO[i+j];
	          }
	      }
	    rc=Kernel_AllocateInjFifos( firstLocalSubGrp+i/8,
		    InjFifoSubgroup+i/8, (i+8<nports) ? 8 : (nports-i),
		    p_ids, iFifoAttrs);
	    DPRINTF(FXLOG_UCODE_INIT,"AllocateInjFifos %d FIFOs in subgrp %d, i=%d rc=%d\n",
		    (i+8<nports) ? 8 : (nports-i), firstLocalSubGrp+i/8, i,rc) ;
	  }
//	rc = Kernel_AllocateInjFifos(0, InjFifoSubgroup, nports, port_ids, injFifoAttrs);

	for (i=0; i < nports; i++) {
		imregion = &roq_us_map->imregions[i];

		imregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->inj_memory_fifos[i]);
		imregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->inj_dmabuf[i]);
		imregion->Bytes  = INJ_MEMORY_FIFO_SIZE+1;

		rc=Kernel_InjFifoInit(InjFifoSubgroup+i/8, i%8, imregion, 0, INJ_MEMORY_FIFO_SIZE);
		DPRINTF(FXLOG_UCODE_INIT,"Kernel_InjFifoInit i=%d rc=%d\n",i,rc) ;
	}
//	rc = Kernel_InjFifoActivate(InjFifoSubgroup, nports, &port_ids[0], KERNEL_INJ_FIFO_ACTIVATE);

	for(i=0; i < nports; i++) {
	    uint32_t p_ids[1] ;
	    p_ids[0] = i%8 ;
	    rc = Kernel_InjFifoActivate(InjFifoSubgroup+i/8, 1,p_ids, KERNEL_INJ_FIFO_ACTIVATE);
	    DPRINTF(FXLOG_UCODE_INIT,"Kernel_InjFifoActivate i=%d rc=%d\n",i,rc) ;

	}

	DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */
	DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */
	/* generate machine check interrupts for MU errors. */

	/* CROSS Domain interrup!!! */
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS_CONTROL_LOW), 3);
#if 0
	DCRWritePriv(MU_DCR(INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH), 0x1111111111111111ULL);
	//DCRWritePriv(MU_DCR(INTERRUPT_INTERNAL_ERROR_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IME_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IME_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT0_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT0_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT1_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT1_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT2_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT2_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(SLAVE_PORT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(SLAVE_PORT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MMREGS_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MMREGS_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	//DCRWritePriv(MU_DCR(FIFO_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IMU_ECC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IMU_ECC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ARLOG_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ARLOG_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RCSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RCSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RPUTSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RPUTSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS0_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS0_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS1_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS1_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ICSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ICSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_ECC_CORR_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ECC_COUNT_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ECC_COUNT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
#endif

	return 0;
}

int
ROQ_ion_init(uint32_t* port_ids, int32_t nports)
{
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup;
	Kernel_RecFifoAttributes_t recFifoAttrs[4];
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup;
	Kernel_InjFifoAttributes_t injFifoAttrs[4];
	Kernel_MemoryRegion_t *imregion;
	Kernel_MemoryRegion_t *rmregion;
	uint64_t recFifoEnableBits;
	int i, rc, sgid;

	// Pick an injection subgroup which is clear of the local ones
	sgid = firstLocalSubGrp - 1;
	firstRemoteSubGrp = sgid;

	roq_us_map->rem.trans_idx = 0;
	for (i=0; i < MAX_NODE_COUNT; i++) {
		roq_us_map->rem.mu_ackdesc[i]       = malloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT);
		roq_us_map->rem.mu_ackdesc_dma[i]   = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->rem.mu_ackdesc[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->rem.mu_burstdesc[i]     = malloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
		roq_us_map->rem.mu_burstdesc_dma[i] = (void*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->rem.mu_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->rem.tx_burstdesc[i]     = malloc(sizeof(struct RoQ_TX_BurstDescriptor)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
		roq_us_map->rem.tx_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->rem.tx_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
    roq_us_map->rem.rx_burstdesc[i]     = malloc(sizeof(struct RoQ_RX_BurstAckDescriptor)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT);
    roq_us_map->rem.rx_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF + domain_physddr_start() + ((uint64_t)roq_us_map->rem.rx_burstdesc[i] & ~ROQ_VIRTUAL_MAPPING));
	}
	for (i=0; i < nports; i++) {
		roq_us_map->rem.inj_virt[i] = (void*)(((uint64_t)malloc(INJ_MEMORY_FIFO_SIZE+64) + 64) & ~(64-1));
		roq_us_map->rem.inj_dma[i]  = (void*)(domain_physddr_start() + ((uint64_t)roq_us_map->rem.inj_virt[i] & ~ROQ_VIRTUAL_MAPPING));
		roq_us_map->rem.rec_virt[i] = (void*)(((uint64_t)malloc(REC_MEMORY_FIFO_SIZE+32) + 32) & ~(32-1));
		roq_us_map->rem.rec_dma[i]  = (void*)(domain_physddr_start() + ((uint64_t)roq_us_map->rem.rec_virt[i] & ~ROQ_VIRTUAL_MAPPING));
	}

	firstRemoteBaseAddressTable =
	    firstRemoteSubGrp * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP
	    + port_ids[0];
	DPRINTF(FXLOG_UCODE_INIT, "AllocateBaseAddressTable in subgroup %d, %d BATs starting at %d\n",
		firstRemoteSubGrp, nports, firstRemoteBaseAddressTable);
	rc = Kernel_AllocateBaseAddressTable(firstRemoteSubGrp,
		&roq_us_map->rem.recAddressTable, nports, port_ids, 1);
	if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);

	/* allocate reception fifo */
	void *p=malloc(sizeof(MUSPI_RecFifoSubGroup_t)) ;
	roq_us_map->rem.RecFifoSubgroups = (MUSPI_RecFifoSubGroup_t*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)p & ~ROQ_VIRTUAL_MAPPING));
	RecFifoSubgroup = &roq_us_map->rem.RecFifoSubgroups[0];
	DPRINTF(FXLOG_UCODE_INIT,"p=%p roq_us_map->rem.RecFifoSubgroups=%p RecFifoSubgroup=%p\n",p,roq_us_map->rem.RecFifoSubgroups,RecFifoSubgroup) ;

	memset(&recFifoAttrs, 0, nports*sizeof(Kernel_RecFifoAttributes_t));

	for (i=0; i < nports; i++) {
		recFifoAttrs[i].System = 1;
	}

	firstRemoteRecFifo =
	    firstRemoteSubGrp * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP
	    + port_ids[0];
	DPRINTF(FXLOG_UCODE_INIT, "AllocateRecFifos in subgrp %d for %d FIFOs starting at %d\n",
		firstRemoteSubGrp, nports, firstRemoteRecFifo);
	rc = Kernel_AllocateRecFifos(firstRemoteSubGrp,
		RecFifoSubgroup, nports, port_ids, recFifoAttrs);
	if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);

	recFifoEnableBits = 0;
	for (i=0; i < nports; i++) {
		recFifoEnableBits |= (0x0000000000000001ULL << ( 15 - ( (sgid % BGQ_MU_NUM_FIFO_SUBGROUPS)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) - port_ids[i] ));
//    recFifoEnableBits |= (0x0000000000000001ULL << ( 15 - ( (sgid % BGQ_MU_NUM_FIFO_SUBGROUPS)*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP)) - port_ids[i] );
	}

	DPRINTF(FXLOG_UCODE_INIT,"Kernel_RecFifoEnable %d 0x%016llx\n",sgid/BGQ_MU_NUM_FIFO_SUBGROUPS, recFifoEnableBits);
	rc = Kernel_RecFifoEnable(firstRemoteSubGrp/BGQ_MU_NUM_FIFO_SUBGROUPS,
		recFifoEnableBits);
	if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);

	for (i=0; i < nports; i++) {
		rmregion = &roq_us_map->rem.rmregions[i];

		rmregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->rem.rec_virt[i]);
		rmregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->rem.rec_dma[i]);
		rmregion->Bytes  = REC_MEMORY_FIFO_SIZE;

		DPRINTF(FXLOG_UCODE_INIT,"Kernel_RecFifoInit port_ids[i]=%d\n",port_ids[i]) ;

		rc = Kernel_RecFifoInit(RecFifoSubgroup, port_ids[i], rmregion, 0, REC_MEMORY_FIFO_SIZE);
		if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);
	}

	/* allocate injection fifo */
	roq_us_map->rem.InjFifoSubgroups = (MUSPI_InjFifoSubGroup_t*)(ROQ_VIRT_MUCACHE_OFF + domain_physddr_start() + ((uint64_t)malloc(sizeof(MUSPI_InjFifoSubGroup_t)) & ~ROQ_VIRTUAL_MAPPING));
	InjFifoSubgroup = &roq_us_map->rem.InjFifoSubgroups[0];
	memset(&injFifoAttrs, 0, nports*sizeof(Kernel_InjFifoAttributes_t));

	for (i=0; i < nports; i++) {
		injFifoAttrs[i].RemoteGet = 0;
		injFifoAttrs[i].System    = 1;
	}

	firstRemoteInjFifo =
	    firstRemoteSubGrp * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP + port_ids[0];
	DPRINTF(FXLOG_UCODE_INIT, "AllocateInjFifos in subgrp %d for %d FIFOs starting at %d\n",
		firstRemoteSubGrp, nports, firstRemoteInjFifo);
	rc = Kernel_AllocateInjFifos(firstRemoteSubGrp,
		InjFifoSubgroup, nports, port_ids, injFifoAttrs);
	if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);

	for (i=0; i < nports; i++) {
		imregion = &roq_us_map->rem.imregions[i];

		imregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->rem.inj_virt[i]);
		imregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->rem.inj_dma[i]);
		imregion->Bytes  = INJ_MEMORY_FIFO_SIZE+1;

		rc = Kernel_InjFifoInit(InjFifoSubgroup, port_ids[i], imregion, 0, INJ_MEMORY_FIFO_SIZE);
		if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);
	}

	rc = Kernel_InjFifoActivate(InjFifoSubgroup, nports, &port_ids[0], KERNEL_INJ_FIFO_ACTIVATE);
	if (rc != 0) DPRINTF(1, "%s:%d (%d)\n", __func__, __LINE__, rc);

	return 0;
}

int
ROQ_activate()
{
	//return ioctl(fd, 2, 0);
	return 0;
}

int
ROQ_deactivate()
{
	//return ioctl(fd, 2, 1);
	return 0;
}

// Hardware count of number of descriptors transmitted
// This code is buggy: RoQ uses several injection FIFO subgroups;
// however, this code only considers a single one (an assertion would
// fire in the MU SPI code)
/*
long ROQ_getHwDescCount(uint32_t iolink, uint32_t SendPortId)
  {
    MUSPI_InjFifo_t *injfifo;
    injfifo = (MUSPI_InjFifo_t*)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToInjFifo(SendPortId, (iolink)?&roq_us_map->rem.InjFifoSubgroups[0]:&roq_us_map->InjFifoSubgroups[0]));
    return MUSPI_getHwDescCount(injfifo) ;
  }
*/
static void display_mu_registers(void)
  {
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MOD_HALTED)",DCRReadPriv(MU_DCR(MOD_HALTED))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MCSRAM_BD_FETCHING_DESC)",DCRReadPriv(MU_DCR(MCSRAM_BD_FETCHING_DESC))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(IME_STATE)",DCRReadPriv(MU_DCR(IME_STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(RME_STATE0)",DCRReadPriv(MU_DCR(RME_STATE0))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(RME_STATE1)",DCRReadPriv(MU_DCR(RME_STATE1))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(INJ_NDFIFO_AVAIL)",DCRReadPriv(MU_DCR(INJ_NDFIFO_AVAIL))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MASTER_PORT0_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT0_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MASTER_PORT1_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT1_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MASTER_PORT2_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MASTER_PORT2_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(SLAVE_PORT_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(SLAVE_PORT_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(XS_ERR_INFO)",DCRReadPriv(MU_DCR(XS_ERR_INFO))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MMREGS_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MMREGS_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(MCSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(MCSRAM_ERR_FIFO_ID)",DCRReadPriv(MU_DCR(MCSRAM_ERR_FIFO_ID))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(RCSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RCSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(RMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(RMFIFO_ACCESS_ERROR_ID))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(RPUTSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(RPUTSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(ICSRAM_INTERRUPTS__STATE)",DCRReadPriv(MU_DCR(ICSRAM_INTERRUPTS__STATE))) ;
    DPRINTF(FXLOG_REMOTE_GET,"%s 0x%016lx\n","MU_DCR(IMFIFO_ACCESS_ERROR_ID)",DCRReadPriv(MU_DCR(IMFIFO_ACCESS_ERROR_ID))) ;

  }

int
ROQ_Send(uint32_t iolink, uint32_t SendPortId, void* MUHWI_DescPtr)
{
	int rc;
	MUSPI_InjFifo_t *injfifo;
	injfifo = (MUSPI_InjFifo_t*)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToInjFifo(SendPortId, (iolink)?&roq_us_map->rem.InjFifoSubgroups[0]:&roq_us_map->InjFifoSubgroups[0]));
//  injfifo = (MUSPI_InjFifo_t*)(ROQ_VIRT_MUCACHE_OFF | (uint64_t)MUSPI_IdToInjFifo(SendPortId, &roq_us_map->InjFifoSubgroups[0]));

	DPRINTF(FXLOG_UCODE_TX,"ROQ_Send iolink=%d SendPortId=%d injfifo=%p\n",iolink,SendPortId,injfifo) ;
	MUHWI_InjFifo_t *hw_injfifo=injfifo->hw_injfifo ;
	//printf("(>) ROQ_Send") ; fflush(stdout) ;
	while ((rc = MUSPI_InjFifoInject(injfifo, (MUHWI_Descriptor_t*)MUHWI_DescPtr)) == -1)
	  {
//	    printf("ROQ_Send spinning\n") ;
//	    fflush(stdout) ;
	  }
		;
	//printf("(<) ROQ_Send\n") ;
	//fflush(stdout) ;
	return rc;
}

int
ROQ_interrupt()
{
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS__FORCE), 1);
	uint64_t b = DCRReadPriv(MU_DCR(FIFO_INTERRUPTS__STATE));

	return b;
}
