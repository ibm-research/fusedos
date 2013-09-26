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

#include <bgq_hw_abstraction.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <asm/types.h>

#ifdef USERSPACE_ROQ
#include <execinfo.h>
#include <signal.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/process.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/MU.h>
#define _NEED_ROQ_MAGIC_LOCATION
#include <roq_device_interface.h>
#endif /* USERSPACE_ROQ */

#include <transcon.h>
#include <roq_fw.h>
#include <SRSW_QueueSet.h>

#if defined(USERSPACE_ROQ)
#include <sys/mman.h>
#include <unistd.h>
#endif

/*** STATIC ********************************************************/
#ifdef ROQ_RESILIENCY
struct RoQ_Resiliency_Args_t RoQ_Resiliency_Args;
#endif

struct RoQ_RX_Arguments   RoQ_RX_Args[1];
struct RoQ_TX_Arguments   RoQ_TX_Args[1];
struct RoQ_TransportConnection		RoQ_loc_TransportConnection[ROQ_MAX_IO_NODES];
struct RoQ_TransportConnection		RoQ_rem_TransportConnection[ROQ_MAX_IO_NODES];
struct RoQ_DevicePrivatePerQPState	RoQ_DevicePrivatePerQPState[RoQ_MAX_QP];
int RoQ_DD2MC_AddressTranslation;

Personality_t *personality;

int32_t  RoQ_LocalRank;     //  = -1; // index into the above array for the lcoal nodes info
uint32_t RoQ_LocalIPV4Addr; //  = 0;
struct RoQ_NetworkDescriptor RoQ_NetworkDesc;
struct RoQ_NetworkDescriptor RoQ_NetworkDesc_rem;
uint32_t volatile RoQ_discovery_act = 0;
uint64_t BootTime = 0;
