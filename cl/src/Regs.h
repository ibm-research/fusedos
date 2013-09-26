/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#ifndef _REGS_H_ // Prevent multiple inclusion.
#define _REGS_H_

#include <hwi/include/common/compiler_support.h>

#define NUM_GPRS  (32) //
#define NUM_QVRS  (32) //
#define NUM_SPRG  (10) // SPRG[0..8], and VRSAVE.
#define NUM_GSPRG ( 4) // If no active Hypervisor, there are also 4 Guest SPRGs: GPSRG[0..3]

#define _ABI_STACK_TAIL            (288)	// ppc64 ABI protects a certain
						// amount of space *beyond* the
						// stack pointer.  The number
						// is actually (18 + 18) * 8,
						// the space needed to save the
						// non-volatile gprs and fprs.
#define STACK_FRAME_SIZE           (128)

#define MSR_CNK_DISABLED  (MSR_CM | MSR_ICM | MSR_ME)
#define MSR_CNK           (MSR_CM | MSR_ICM | MSR_ME | MSR_CE)

#define MSR_APP_REQUIRED  (MSR_CM | MSR_ICM | MSR_ME | MSR_CE | \
			   MSR_EE | MSR_FP | MSR_PR)
#define MSR_APP_FORBIDDEN (MSR_GS | MSR_UCLE | MSR_SPV | MSR_WE | \
			   MSR_IS | MSR_DS)

#if !defined(__ASSEMBLY__)

#include <stdint.h>

#include <hwi/include/common/bgq_alignment.h>

#include <hwi/include/bqc/A2_core.h>


typedef union QPX_Reg_t
{
    double    d[ 4];
    float     f[ 8];
    uint64_t ll[ 4];
    uint32_t  l[ 8];
    uint8_t   b[32];
}
ALIGN_QPX_VECTOR QPX_Reg_t;

//
// This structure contains no per-core SPRs, only per-thread SPRs.
//
typedef struct Regs_t
{
    uint64_t    gpr[ NUM_GPRS ];
    QPX_Reg_t   qvr[ NUM_QVRS ];
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
                pid,
                dbcr0,
                dbcr1,
                dbcr2,
                dbcr3,
                dac1,
                dac2,
                dac3,
                dac4,
                dbsr,
                iac1,
                iac2,
                iac3,
                iac4,
		mcsr;
}
ALIGN_L1D_CACHE Regs_t;

#endif // __ASSEMBLY__

#endif // Add nothing below this line.
