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

#if defined(PKTRACE_ON)
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

#include <roq_fw_trace.h>
PkTraceState TraceState;
addressTable_t addressTable[k_pirsize] __attribute__((aligned(64))) ;
#endif
