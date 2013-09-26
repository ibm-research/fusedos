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

#ifndef _ROQ_KERNEL_H
#define _ROQ_KERNEL_H

__C_LINKAGE uint64_t
domain_physddr_start(void);
#define ROQ_VIRTUAL_MAPPING 0xc0000000

#ifdef ROQ_RESILIENCY
// Interrupt Handling Stuff - copied & modified from firmware
#include <firmware/include/fwext/fwext_regs.h>

#define NUM_GPRS  (32) //
#define NUM_FPRS  (32) //
#define NUM_SPRG  (10) // SPRG[0..8], and VRSAVE.
#define NUM_GSPRG ( 4) // If no active Hypervisor, there are also 4 Guest SPRGs: GPSRG[0..3]

// threads used by the various functions
// Core 15 (60-63)
#define ROQ_TX_THREAD        60
#define ROQ_FORWARDER_THREAD 61
#define ROQ_ROUTER_THREAD    62

// Core 16 (64-67)
#define ROQ_CHECKER_THREAD   64
#define ROQ_NOTIFIER_THREAD  65
#define ROQ_RX_THREAD        67

//
// This structure contains no per-core SPRs, only per-thread SPRs.
//
// NOTE: If changing this structure, update tools/regs_ofs.c and
//       rerun it to generate the structure offsets above.
//
typedef struct RoQ_Regs_t
{
    uint64_t    gpr[ NUM_GPRS ];
    QPX_Reg_t   fpr[ NUM_FPRS ];
    //
    uint64_t    fpscr;
    //
    uint64_t    sprg[  NUM_SPRG  ];
    uint64_t    gsprg[ NUM_GSPRG ];
    //
    uint64_t    ip,            // from (mc|c)srr0
                msr,           // from (mc|c)srr1
                cr,
                lr,
                xer,
                ctr,
                esr,
                dear,
                pid;
    // and a whole bunch of other stuff...
}
ALIGN_L1D_CACHE RoQ_Regs_t;

typedef void (*RoQ_InterruptHandler_t)(RoQ_Regs_t* context, uint64_t code);

void setInterruptHandler(RoQ_InterruptHandler_t handler, uint16_t class);
void enableDecrementerInterrupt(uint32_t period, uint32_t initial, int are);
#endif
#endif /* _ROQ_KERNEL_H */
