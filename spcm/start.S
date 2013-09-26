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

#include <cnk/src/Debug.h>
#include <cnk/src/Regs.h>
#include <cnk/src/syscalls.h>
#include <cnk/include/SPRG_Usage.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/common/asm_support.h>
#include <hwi/include/common/bgq_bitnumbers.h>

#include "../cnk/src/asmConstants.h"

#include "fusedos_config.h"

#define REGS_GPR(n) (REGS_gpr + ((n)*8))
#define REGS_SPRG(n) (REGS_sprg + ((n)*8))
#define REGS_GSPRG(n) (REGS_gsprg + ((n)*8))

/* BGQ bits and pieces */
#define SPRN_PPR32                (  898)          // Program Priority Register

#define SPC_THREAD_PRI		  (0x100000)       // medium high

// From arch/powerpc/include/asm/ppc_asm.h
#define XGLUE(a,b) a##b
#define GLUE(a,b) XGLUE(a,b)
#define _GLOBAL(name) \
        .section ".text"; \
        .align 2 ; \
        .globl name; \
        .globl GLUE(.,name); \
        .section ".opd","aw"; \
name: \
        .quad GLUE(.,name); \
        .quad .TOC.@tocbase; \
        .quad 0; \
        .previous; \
        .type GLUE(.,name),@function; \
GLUE(.,name):

#define SPCC_OFS_BIS      (0x0640)
#define SPCC_OFS_BV       (0x0648)
#define SPCC_OFS_PPR32    (0x0650)
#define SPCC_OFS_IPIWU    (0x0658)
#define SPCC_OFS_ID       (0x0660)
#define SPCC_OFS_START    (0x0668)
#define SPCC_OFS_TLBC     (0x0680)
#define SPCC_OFS_TLBI     (0x0688)
#define SPCC_OFS_PMSP     (0x6a90)
#define SPCC_OFS_PMTOC    (0x6a98)
#define SPCC_OFS_EX       (0x6aa0)
#define SPCC_OFS_SCR0     (0x6b28)
#define SPCC_OFS_SCR1     (0x6b30)
#define SPCC_OFS_SCR2     (0x6b38)
#define SPCC_OFS_SCR3     (0x6b40)

#define SPRG_context SPRN_SPRG8

#define DEBUG_CODE_MCHK     (19)
#define DEBUG_CODE_CI       (20)
#define DEBUG_CODE_EI       (21)
#define DEBUG_CODE_SC       (22)
#define DEBUG_CODE_DEC      (23)
#define DEBUG_CODE_FIT      (24)
#define DEBUG_CODE_WDT      (25)
#define DEBUG_CODE_UDEC     (26)
#define DEBUG_CODE_PERFMON  (27)

_GLOBAL(monitor_start_1)
	// Store context passed in by takeCPU. If we are hw thread 0,
	// then we must use the high context address and then convert to a
	// low address for references after replacing the firmware
	// TLB entry.
	mtspr	SPRG_context, %r3
	ld	%r4, SPCC_OFS_ID(%r3)
	IMM64(%r5, THREADS_PER_CORE)
	subi	%r5, %r5, 1
	and	%r4, %r4, %r5
	cmpdi	%r4, 0
	bne	monitor_start_4
	
	// no branch, we are HW thread 0 on this core
	// [ calculate SPC context base = context - context->id * sizeof(spc_context_t)
	// calculate SPC monitor base = SPC context base - SPC_MONITOR_SIZE ]
	// much shorter: SPC monitor is 16-MB-aligned, just bitmask
	IMM64(%r6, 0xffffff)
	andc %r6, %r3, %r6

	// Convert context address to a low address for access once SPC
	// monitor and context are mapped to 0-16 MB in virtual memory
	sub	%r3, %r3, %r6
	mtspr	SPRG_context, %r3

	// Add temp 16 MB TLB entry for monitor and context, va 0x80000000
	// --> pa SPC monitor base (%r6)
	// Similar to
	// vmm_MapUserSpace(APP_FLAGS_NONSPECULATIVE|APP_FLAGS_LISTENABLE,
	// (void*)(SPC_MONITOR_PADDR), (void*)(SPC_MONITOR_TVADDR),
	// 0x01000000, 0, 0) except we are using tlbwe instead of tlbwe_slot
	// (do not pick a specific slot). Use cnk/src/vmm_mapuserspace
	// to generate spr values.
	li	%r4, 0x4000	// MAS0_ATSEL_TLB | MAS0_HES_HW_LRU | MAS0_WQ_ALWAYS
	mtspr	SPRN_MAS0, %r4
	IMM64(%r4, 0x80000700)
	mtspr	SPRN_MAS1, %r4
	IMM64(%r4, 0x60000004)
	mtspr	SPRN_MAS2, %r4
	// combine SPC monitor base address and flags
	ori %r4, %r6, 0x255
	mtspr	SPRN_MAS7_MAS3, %r4
	li	%r4, 0x0
	mtspr	SPRN_MAS8, %r4
	li	%r4, 0x310F
	mtspr	SPRN_MMUCR3, %r4
	isync
	tlbwe
	// Deref monitor_start_2 to get the real va
	IMM64(%r5, monitor_start_2)
	add	%r5, %r5, %r6 // SPC monitor base address in %r6
	ld	%r4, 0(%r5)
        // Put the real vaddr in the temp space
	IMM64(%r5, SPC_MONITOR_TVADDR)
	add	%r4, %r4, %r5
	mtlr	%r4
	blr

_GLOBAL(monitor_start_2)
	// Invalidate firmware 1 GB TLB entry (va 0x0)
	li	%r4, 0xA00	// MAS6_SPID(0) | MAS6_ISIZE_1GB | MAS6_SAS(0)
	mtspr	SPRN_MAS6, %r4
	isync
	li	%r4, 0x0
	tlbilxva 0, %r4
	// Add real 16 MB TLB entry for monitor and context, va 0x00000000
	// --> pa SPC monitor base address in %r6
	// Similar to
	// vmm_MapUserSpace(APP_FLAGS_NONSPECULATIVE|APP_FLAGS_LISTENABLE,
	// (void*)(SPC_MONITOR_PADDR), (void*)(0),0x01000000, 0, 0)
	// except we are using tlbwe instead of tlbwe_slot (do not
	// pick a specific slot). Use cnk/src/vmm_mapuserspace to generate
	// spr values.
	li	%r4, 0x4000	// MAS0_ATSEL_TLB | MAS0_HES_HW_LRU | MAS0_WQ_ALWAYS
	mtspr	SPRN_MAS0, %r4
	IMM64(%r4, 0x80000700)
	mtspr	SPRN_MAS1, %r4
	li	%r4, 0x4
	mtspr	SPRN_MAS2, %r4
	// combine SPC monitor base address and flags
	ori %r4, %r6, 0x255
	mtspr	SPRN_MAS7_MAS3, %r4
	li	%r4, 0x0
	mtspr	SPRN_MAS8, %r4
	li	%r4, 0x310F
	mtspr	SPRN_MMUCR3, %r4
	isync
	tlbwe
	// Deref monitor_start_3 to get the real vaddr
	IMM64(%r4, SPC_MONITOR_TVADDR)
	IMM64(%r5, monitor_start_3)
	add	%r5, %r5, %r4
	ld	%r4, 0(%r5)
	mtlr	%r4
	blr

_GLOBAL(monitor_start_3)
	// Invalidate temp 16 GB TLB entry (SPC_MONITOR_TVADDR)
	li	%r4, 0x700	// MAS6_SPID(0) | MAS6_ISIZE_16MB | MAS6_SAS(0)
	mtspr	SPRN_MAS6, %r4
	isync
	IMM64(%r4, SPC_MONITOR_TVADDR)
	tlbilxva 0, %r4

_GLOBAL(monitor_start_4)
	// Get ready to jump into C, load our exception vector, sp,
	// and toc
	IMM64(%r4, exception_vector)
	mtivpr	%r4
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	b	monitor

_GLOBAL(get_context)
	mfspr	%r3, SPRG_context
	blr

// r3 must contain context address
_GLOBAL(start_app)
	// Enable FP, MSR is not set until rfi
	mfmsr	%r10
	ori	%r0, %r10, MSR_FP
	mtmsr	%r0
	// FP status and control register
	lfd	%f0, REGS_fpscr(%r3)
	mtfsf	0, %f0, 1, 0
	// Cats and dogs
	li	%r5, 0	// Clear DEAR from previous uses
	mtdear	%r5
	IMM64(%r5, SPC_THREAD_PRI)
	mtspr	SPRN_PPR32, %r5
	ld	%r5, REGS_lr(%r3)
	ld	%r6, REGS_cr(%r3)
	ld	%r7, REGS_xer(%r3)
	ld	%r8, REGS_ctr(%r3)
	ld	%r9, REGS_ip(%r3)
	ld	%r10, REGS_msr(%r3)
	ld	%r11, REGS_pid(%r3)
	ld	%r12, REGS_SPRG(7)(%r3)	// SPI info SPR
	mtlr	%r5
	mtcr	%r6
	mtxer	%r7
	mtctr	%r8
	mtspr	SPRN_SRR0_IP, %r9
	mtspr	SPRN_SRR1_MSR, %r10
	mtspr   SPRN_PID, %r11
	mtspr   SPRG_SPIinfo, %r12
	// FP registers
	la	%r11, (REGS_qvr)(%r3)
	li	%r0, 32
	qvlfdux	%f0, %r11, %r0
	qvlfdux	%f1, %r11, %r0
	qvlfdux	%f2, %r11, %r0
	qvlfdux	%f3, %r11, %r0
	qvlfdux	%f4, %r11, %r0
	qvlfdux	%f5, %r11, %r0
	qvlfdux	%f6, %r11, %r0
	qvlfdux	%f7, %r11, %r0
	qvlfdux	%f8, %r11, %r0
	qvlfdux	%f9, %r11, %r0
	qvlfdux	%f10, %r11, %r0
	qvlfdux	%f11, %r11, %r0
	qvlfdux	%f12, %r11, %r0
	qvlfdux	%f13, %r11, %r0
	qvlfdux	%f14, %r11, %r0
	qvlfdux	%f15, %r11, %r0
	qvlfdux	%f16, %r11, %r0
	qvlfdux	%f17, %r11, %r0
	qvlfdux	%f18, %r11, %r0
	qvlfdux	%f19, %r11, %r0
	qvlfdux	%f20, %r11, %r0
	qvlfdux	%f21, %r11, %r0
	qvlfdux	%f22, %r11, %r0
	qvlfdux	%f23, %r11, %r0
	qvlfdux	%f24, %r11, %r0
	qvlfdux	%f25, %r11, %r0
	qvlfdux	%f26, %r11, %r0
	qvlfdux	%f27, %r11, %r0
	qvlfdux	%f28, %r11, %r0
	qvlfdux	%f29, %r11, %r0
	qvlfdux	%f30, %r11, %r0
	qvlfdux	%f31, %r11, %r0
	// GP registers
	ld	%r0, REGS_GPR(0)(%r3)
	ld	%r1, REGS_GPR(1)(%r3)
	ld	%r2, REGS_GPR(2)(%r3)
	// r3 must be last because it contains the context address
	ld	%r4, REGS_GPR(4)(%r3)
	ld	%r5, REGS_GPR(5)(%r3)
	ld	%r6, REGS_GPR(6)(%r3)
	ld	%r7, REGS_GPR(7)(%r3)
	ld	%r8, REGS_GPR(8)(%r3)
	ld	%r9, REGS_GPR(9)(%r3)
	ld	%r10, REGS_GPR(10)(%r3)
	ld	%r11, REGS_GPR(11)(%r3)
	ld	%r12, REGS_GPR(12)(%r3)
	ld	%r13, REGS_GPR(13)(%r3)
	ld	%r14, REGS_GPR(14)(%r3)
	ld	%r15, REGS_GPR(15)(%r3)
	ld	%r16, REGS_GPR(16)(%r3)
	ld	%r17, REGS_GPR(17)(%r3)
	ld	%r18, REGS_GPR(18)(%r3)
	ld	%r19, REGS_GPR(19)(%r3)
	ld	%r20, REGS_GPR(20)(%r3)
	ld	%r21, REGS_GPR(21)(%r3)
	ld	%r22, REGS_GPR(22)(%r3)
	ld	%r23, REGS_GPR(23)(%r3)
	ld	%r24, REGS_GPR(24)(%r3)
	ld	%r25, REGS_GPR(25)(%r3)
	ld	%r26, REGS_GPR(26)(%r3)
	ld	%r27, REGS_GPR(27)(%r3)
	ld	%r28, REGS_GPR(28)(%r3)
	ld	%r29, REGS_GPR(29)(%r3)
	ld	%r30, REGS_GPR(30)(%r3)
	ld	%r31, REGS_GPR(31)(%r3)
	ld	%r3, REGS_GPR(3)(%r3)
	rfi

#define _SYSC(label) \
	.align 5; \
	.global label; \
label: \
	b	syscall_exception

_GLOBAL(syscall_exception)
	// Save scratch registers, r2 and r3
	mtspr	SPRN_SPRG4, %r2
	mtspr	SPRN_SPRG5, %r3
	mfspr	%r3, SPRG_context
#if 1
	// Suspend cores?
	cmpdi	%r0, 1060
	bne	10f
	mflr	%r2			// Save LR
	std	%r2, REGS_lr(%r3)
	mtspr	SPRN_SPRG6, %r1
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	bl	suspend_cores
	mfspr	%r3, SPRG_context	// Probably not necessary
	ld	%r2, REGS_lr(%r3)
	mtlr	%r2
	mfspr	%r2, SPRN_SPRG4
	mfspr	%r3, SPRN_SPRG5
	mfspr	%r1, SPRN_SPRG6
	rfi
10:
	// Resume cores?
	cmpdi	%r0, 1061
	bne	11f
	mflr	%r2			// Save LR
	std	%r2, REGS_lr(%r3)
	mtspr	SPRN_SPRG6, %r1
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	bl	resume_cores
	mfspr	%r3, SPRG_context	// Probably not necessary
	ld	%r2, REGS_lr(%r3)
	mtlr	%r2
	mfspr	%r2, SPRN_SPRG4
	mfspr	%r3, SPRN_SPRG5
	mfspr	%r1, SPRN_SPRG6
	rfi
11:
#endif
	// Save state (overkill?)
	mflr	%r2
	std	%r2, REGS_lr(%r3)
	bl	common_exception
	// Save IP and MSR from SRR0 and SRR1
	mfspr	%r2, SPRN_SRR0_IP
	std	%r2, REGS_ip(%r3)
	mfspr	%r2, SPRN_SRR1_MSR
	std	%r2, REGS_msr(%r3)
	// Save r2 and r3
	mfspr	%r3, SPRG_context
	mfspr	%r2, SPRN_SPRG5
	std	%r2, REGS_GPR(3)(%r3)
	mfspr	%r2, SPRN_SPRG4
	std	%r2, REGS_GPR(2)(%r3)
	// Send an IPI
	li	%r2, 1
#if defined USE_WAITRSV
	std	%r2, SPCC_OFS_IPIWU(%r3)
#else
	ld	%r2, SPCC_OFS_BV(%r3)
	ld	%r1, SPCC_OFS_BIS(%r3)
	std	%r2, 0(%r1)
#endif
	// Go back to monitor
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	b	monitor

_GLOBAL(resume_app)
	mfspr	%r3, SPRG_context
	//
	// IntHandler_Syscall indicates failure by returning a value between
	// -(CNK_ERRNO_RANGE-1) and -1. A value in this range is converted to
	// an errno (by adding CNK_ERROR_RANGE) and returned in r3, along with
	// a failure indication set in the summary overflow (S0) bit of cr0.
	// All other values are returned unchanged in r0, with the cr0 field
	// cleared to indicate success.
	//
	ld	%r4, REGS_cr(%r3)	// Retrieve original cr
	ld	%r2, REGS_GPR(3)(%r3)	// Use r2 as r3
	addi	%r0, %r2, CNK_ERRNO_RANGE // Tentatively convert rc to an errno
	cmpldi	%r0, CNK_ERRNO_RANGE	// Check if it's in the errno range
	rldicl	%r4, %r4, 0, 36		// Clear cr0 field of cr to be restored
	bge+	1f			// Successful return, r3 unchanged
	std	%r0, REGS_GPR(3)(%r3)	// r3 is errno
	oris	%r4, %r4, 0x1000	// Set cr0 S0 bit to indicate failure
1:
	// Restore state
	mtcr	%r4
	ld	%r2, REGS_lr(%r3)
	mtlr	%r2
	ld	%r2, REGS_ctr(%r3)
	mtctr	%r2
	ld	%r2, REGS_SPRG(7)(%r3)	// SPI info SPR
	mtspr   SPRG_SPIinfo, %r2
	ld	%r0, REGS_GPR(0)(%r3)
	ld	%r1, REGS_GPR(1)(%r3)
	ld	%r2, REGS_GPR(2)(%r3)
	ld	%r4, REGS_GPR(4)(%r3)
	ld	%r5, REGS_GPR(5)(%r3)
	ld	%r6, REGS_GPR(6)(%r3)
	ld	%r7, REGS_GPR(7)(%r3)
	ld	%r8, REGS_GPR(8)(%r3)
	ld	%r9, REGS_GPR(9)(%r3)
	ld	%r10, REGS_GPR(10)(%r3)
	ld	%r11, REGS_GPR(11)(%r3)
	ld	%r12, REGS_GPR(12)(%r3)
	ld	%r13, REGS_GPR(13)(%r3)
	ld	%r14, REGS_GPR(14)(%r3)
	ld	%r15, REGS_GPR(15)(%r3)
	ld	%r16, REGS_GPR(16)(%r3)
	ld	%r17, REGS_GPR(17)(%r3)
	ld	%r18, REGS_GPR(18)(%r3)
	ld	%r19, REGS_GPR(19)(%r3)
	ld	%r20, REGS_GPR(20)(%r3)
	ld	%r21, REGS_GPR(21)(%r3)
	ld	%r22, REGS_GPR(22)(%r3)
	ld	%r23, REGS_GPR(23)(%r3)
	ld	%r24, REGS_GPR(24)(%r3)
	ld	%r25, REGS_GPR(25)(%r3)
	ld	%r26, REGS_GPR(26)(%r3)
	ld	%r27, REGS_GPR(27)(%r3)
	ld	%r28, REGS_GPR(28)(%r3)
	ld	%r29, REGS_GPR(29)(%r3)
	ld	%r30, REGS_GPR(30)(%r3)
	ld	%r31, REGS_GPR(31)(%r3)
	ld	%r3, REGS_GPR(3)(%r3)
	rfi

_GLOBAL(save_fpu)
	// Enable FP
	mfmsr	%r10
	ori	%r0, %r10, MSR_FP
	mtmsr	%r0

	// context address is in r3
	la	%r11, (REGS_qvr)(%r3)
	li	%r0, 32

	// FP registers
	qvstfdux	%f0,  %r11, %r0
	qvstfdux	%f1,  %r11, %r0
	qvstfdux	%f2,  %r11, %r0
	qvstfdux	%f3,  %r11, %r0
	qvstfdux	%f4,  %r11, %r0
	qvstfdux	%f5,  %r11, %r0
	qvstfdux	%f6,  %r11, %r0
	qvstfdux	%f7,  %r11, %r0
	qvstfdux	%f8,  %r11, %r0
	qvstfdux	%f9,  %r11, %r0
	qvstfdux	%f10, %r11, %r0
	qvstfdux	%f11, %r11, %r0
	qvstfdux	%f12, %r11, %r0
	qvstfdux	%f13, %r11, %r0
	qvstfdux	%f14, %r11, %r0
	qvstfdux	%f15, %r11, %r0
	qvstfdux	%f16, %r11, %r0
	qvstfdux	%f17, %r11, %r0
	qvstfdux	%f18, %r11, %r0
	qvstfdux	%f19, %r11, %r0
	qvstfdux	%f20, %r11, %r0
	qvstfdux	%f21, %r11, %r0
	qvstfdux	%f22, %r11, %r0
	qvstfdux	%f23, %r11, %r0
	qvstfdux	%f24, %r11, %r0
	qvstfdux	%f25, %r11, %r0
	qvstfdux	%f26, %r11, %r0
	qvstfdux	%f27, %r11, %r0
	qvstfdux	%f28, %r11, %r0
	qvstfdux	%f29, %r11, %r0
	qvstfdux	%f30, %r11, %r0
	qvstfdux	%f31, %r11, %r0

	// FP status and control register
	mffs %f0
	stfd %f0, REGS_fpscr(%r3)

	// restore FPR0
	la	%r11, (REGS_qvr)(%r3)
	qvlfdx	%f0, %r11, %r0

	blr // return

#define _MCHK(label) \
	.align 5; \
	.global label; \
label: \
	mtspr	SPRN_SPRG0, %r2; \
	li	%r2, DEBUG_CODE_MCHK; \
	b	machine_check

_GLOBAL(machine_check)
	mtspr	SPRN_SPRG1, %r3
	mfspr	%r3, SPRG_context
	std	%r2, SPCC_OFS_EX(%r3)
	mflr	%r2
	std	%r2, REGS_lr(%r3)
	bl	common_exception
	// Store IP and MSR saved in MCSSR0 and MCSSR1
	mfspr	%r2, SPRN_MCSRR0_IP
	std	%r2, REGS_ip(%r3)
	mfspr	%r2, SPRN_MCSRR1_MSR
	std	%r2, REGS_msr(%r3)
	// Store Machine Check Syndrome Register
	mfspr	%r2, SPRN_MCSR
	std	%r2, REGS_mcsr(%r3)
        // Scratch
	mfspr	%r2, SPRN_SPRG0
	std	%r2, REGS_GPR(2)(%r3)
	mfspr	%r2, SPRN_SPRG1
	std	%r2, REGS_GPR(3)(%r3)
	b	common_exception_end

#define _CDBG(label, code) \
	.align 5; \
       	.global label; \
label: \
	mtspr	SPRN_SPRG2, %r2; \
	li	%r2, code; \
	b	critical_exception

_GLOBAL(critical_exception)
	mtspr	SPRN_SPRG3, %r3
	mfspr	%r3, SPRG_context
	std	%r2, SPCC_OFS_EX(%r3)
	mflr	%r2
	std	%r2, REGS_lr(%r3)
	bl	common_exception
	// Save IP and MSR from CSRR0 and CSRR1
	mfspr	%r2, SPRN_CSRR0_IP
	std	%r2, REGS_ip(%r3)
	mfspr	%r2, SPRN_CSRR1_MSR
	std	%r2, REGS_msr(%r3)
        // Scratch
	mfspr	%r2, SPRN_SPRG2
	std	%r2, REGS_GPR(2)(%r3)
	mfspr	%r2, SPRN_SPRG3
	std	%r2, REGS_GPR(3)(%r3)
	b	common_exception_end

#define _SDBG(label, code) \
	.align 5; \
       	.global label; \
label: \
	mtspr	SPRN_SPRG4, %r2; \
	li	%r2, code; \
	b	standard_exception

_GLOBAL(standard_exception)
	// Save scratch registers, r2 and r3
	mtspr	SPRN_SPRG4, %r2
	mtspr	SPRN_SPRG5, %r3
	mfspr	%r3, SPRG_context
	// Save state (overkill?)
	mflr	%r2
	std	%r2, REGS_lr(%r3)
	bl	common_exception
	// Save IP and MSR from SRR0 and SRR1
	mfspr	%r2, SPRN_SRR0_IP
	std	%r2, REGS_ip(%r3)
	mfspr	%r2, SPRN_SRR1_MSR
	std	%r2, REGS_msr(%r3)
	// Save r2 and r3
	mfspr	%r3, SPRG_context
	mfspr	%r2, SPRN_SPRG5
	std	%r2, REGS_GPR(3)(%r3)
	mfspr	%r2, SPRN_SPRG4
	std	%r2, REGS_GPR(2)(%r3)
	// IPI?
	cmpdi	%r2, 21
	bne	1f
	mflr	%r2			// Save LR
	std	%r2, REGS_lr(%r3)
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	bl	ipi_handler
        // Restore state
	mfspr	%r3, SPRG_context	// Probably not necessary
	ld	%r2, REGS_lr(%r3)
	mtlr	%r2
	ld	%r2, REGS_ctr(%r3)
	mtctr	%r2
	ld	%r0, REGS_GPR(0)(%r3)
	ld	%r1, REGS_GPR(1)(%r3)
	ld	%r2, REGS_GPR(2)(%r3)
	ld	%r4, REGS_GPR(4)(%r3)
	ld	%r5, REGS_GPR(5)(%r3)
	ld	%r6, REGS_GPR(6)(%r3)
	ld	%r7, REGS_GPR(7)(%r3)
	ld	%r8, REGS_GPR(8)(%r3)
	ld	%r9, REGS_GPR(9)(%r3)
	ld	%r10, REGS_GPR(10)(%r3)
	ld	%r11, REGS_GPR(11)(%r3)
	ld	%r12, REGS_GPR(12)(%r3)
	ld	%r13, REGS_GPR(13)(%r3)
	ld	%r14, REGS_GPR(14)(%r3)
	ld	%r15, REGS_GPR(15)(%r3)
	ld	%r16, REGS_GPR(16)(%r3)
	ld	%r17, REGS_GPR(17)(%r3)
	ld	%r18, REGS_GPR(18)(%r3)
	ld	%r19, REGS_GPR(19)(%r3)
	ld	%r20, REGS_GPR(20)(%r3)
	ld	%r21, REGS_GPR(21)(%r3)
	ld	%r22, REGS_GPR(22)(%r3)
	ld	%r23, REGS_GPR(23)(%r3)
	ld	%r24, REGS_GPR(24)(%r3)
	ld	%r25, REGS_GPR(25)(%r3)
	ld	%r26, REGS_GPR(26)(%r3)
	ld	%r27, REGS_GPR(27)(%r3)
	ld	%r28, REGS_GPR(28)(%r3)
	ld	%r29, REGS_GPR(29)(%r3)
	ld	%r30, REGS_GPR(30)(%r3)
	ld	%r31, REGS_GPR(31)(%r3)
	ld	%r3, REGS_GPR(3)(%r3)
	rfi
1:
	std	%r2, SPCC_OFS_EX(%r3)
	mflr	%r2
	std	%r2, REGS_lr(%r3)
	b	common_exception_end

_GLOBAL(common_exception)
	// GP registers
	std	%r0, REGS_GPR(0)(%r3)
	std	%r1, REGS_GPR(1)(%r3)
	std	%r4, REGS_GPR(4)(%r3)
	std	%r5, REGS_GPR(5)(%r3)
	std	%r6, REGS_GPR(6)(%r3)
	std	%r7, REGS_GPR(7)(%r3)
	std	%r8, REGS_GPR(8)(%r3)
	std	%r9, REGS_GPR(9)(%r3)
	std	%r10, REGS_GPR(10)(%r3)
	std	%r11, REGS_GPR(11)(%r3)
	std	%r12, REGS_GPR(12)(%r3)
	std	%r13, REGS_GPR(13)(%r3)
	std	%r14, REGS_GPR(14)(%r3)
	std	%r15, REGS_GPR(15)(%r3)
	std	%r16, REGS_GPR(16)(%r3)
	std	%r17, REGS_GPR(17)(%r3)
	std	%r18, REGS_GPR(18)(%r3)
	std	%r19, REGS_GPR(19)(%r3)
	std	%r20, REGS_GPR(20)(%r3)
	std	%r21, REGS_GPR(21)(%r3)
	std	%r22, REGS_GPR(22)(%r3)
	std	%r23, REGS_GPR(23)(%r3)
	std	%r24, REGS_GPR(24)(%r3)
	std	%r25, REGS_GPR(25)(%r3)
	std	%r26, REGS_GPR(26)(%r3)
	std	%r27, REGS_GPR(27)(%r3)
	std	%r28, REGS_GPR(28)(%r3)
	std	%r29, REGS_GPR(29)(%r3)
	std	%r30, REGS_GPR(30)(%r3)
	std	%r31, REGS_GPR(31)(%r3)
	// SPRGs and GSPRGs. What about vrsave?
	mfspr	%r2, SPRN_PPR32
	std	%r2, SPCC_OFS_PPR32(%r3)
	mfspr	%r2, SPRN_SPRG0
	std	%r2, REGS_SPRG(0)(%r3)
	mfspr	%r2, SPRN_SPRG1
	std	%r2, REGS_SPRG(1)(%r3)
	mfspr	%r2, SPRN_SPRG2
	std	%r2, REGS_SPRG(2)(%r3)
	mfspr	%r2, SPRN_SPRG3
	std	%r2, REGS_SPRG(3)(%r3)
	mfspr	%r2, SPRN_SPRG4
	std	%r2, REGS_SPRG(4)(%r3)
	mfspr	%r2, SPRN_SPRG5
	std	%r2, REGS_SPRG(5)(%r3)
	mfspr	%r2, SPRN_SPRG6
	std	%r2, REGS_SPRG(6)(%r3)
	mfspr	%r2, SPRN_SPRG7
	std	%r2, REGS_SPRG(7)(%r3)
	mfspr	%r2, SPRN_SPRG8
	std	%r2, REGS_SPRG(8)(%r3)
	mfspr	%r2, SPRN_GSPRG0
	std	%r2, REGS_GSPRG(0)(%r3)
	mfspr	%r2, SPRN_GSPRG1
	std	%r2, REGS_GSPRG(1)(%r3)
	mfspr	%r2, SPRN_GSPRG2
	std	%r2, REGS_GSPRG(2)(%r3)
	mfspr	%r2, SPRN_GSPRG3
	std	%r2, REGS_GSPRG(3)(%r3)
	// Cats and dogs
	mfcr	%r2
	std	%r2, REGS_cr(%r3)
	mfxer	%r2
	std	%r2, REGS_xer(%r3)
	mfctr	%r2
	std	%r2, REGS_ctr(%r3)
	mfesr	%r2
	std	%r2, (REGS_ctr + 8)(%r3)
	mfdear	%r2
	std	%r2, (REGS_ctr +16)(%r3)
	mfpid	%r2
	std	%r2, REGS_pid(%r3)
	blr

_GLOBAL(common_exception_end)
	// Send an IPI
	li	%r2, 1
#if defined USE_WAITRSV
	std	%r2, SPCC_OFS_IPIWU(%r3)
#else
	ld	%r2, SPCC_OFS_BV(%r3)
	ld	%r1, SPCC_OFS_BIS(%r3)
	std	%r2, 0(%r1)
#endif
	// Go back to monitor
	ld	%r1, SPCC_OFS_PMSP(%r3)
	ld	%r2, SPCC_OFS_PMTOC(%r3)
	b	monitor

	.align	12
	.global exception_vector
exception_vector:
	_MCHK(Vector_MCHK)
	_CDBG(Vector_CI,       DEBUG_CODE_CI)
	_CDBG(Vector_DEBUG,    DEBUG_CODE_DEBUG)
	_SDBG(Vector_DSI,      DEBUG_CODE_DSI)
	_SDBG(Vector_ISI,      DEBUG_CODE_ISI)
	_SDBG(Vector_EI,       DEBUG_CODE_EI)
	_SDBG(Vector_ALGN,     DEBUG_CODE_ALGN)
	_SDBG(Vector_PROG,     DEBUG_CODE_PROG)
	_SDBG(Vector_FPU,      DEBUG_CODE_FPU)
	_SYSC(Vector_SC)
	_SDBG(Vector_APU,      DEBUG_CODE_APU)
	_SDBG(Vector_DEC,      DEBUG_CODE_DEC)
	_SDBG(Vector_FIT,      DEBUG_CODE_FIT)
	_CDBG(Vector_WDT,      DEBUG_CODE_WDT)
	_SDBG(Vector_DTLB,     DEBUG_CODE_DTLB)
	_SDBG(Vector_ITLB,     DEBUG_CODE_ITLB)
	_SDBG(Vector_VECTOR,   DEBUG_CODE_VECT)
	_SDBG(Vector_Undef220, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef240, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef260, DEBUG_CODE_UNDEF)
	_SDBG(Vector_PDBI,     DEBUG_CODE_PDBI)
	_CDBG(Vector_PDBCI,    DEBUG_CODE_PDBCI)
	_SDBG(Vector_GDBI,     DEBUG_CODE_GDBI)
	_CDBG(Vector_GDBCI,    DEBUG_CODE_GDBCI)
	_SDBG(Vector_EHVSC,    DEBUG_CODE_EHVSC)
	_SDBG(Vector_EHVPRIV,  DEBUG_CODE_EHVPRIV)
	_SDBG(Vector_LRATE,    DEBUG_CODE_LRATE)
	_SDBG(Vector_Undef360, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef380, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef3a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef3c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef3e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef400, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef420, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef440, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef460, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef480, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef4a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef4c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef4e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef500, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef520, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef540, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef560, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef580, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef5a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef5c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef5e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef600, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef620, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef640, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef660, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef680, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef6a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef6c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef6e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef700, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef720, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef740, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef760, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef780, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef7a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef7c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef7e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_UDEC,     DEBUG_CODE_UDEC)
	_SDBG(Vector_PERFMON,  DEBUG_CODE_PERFMON)
	_SDBG(Vector_Undef840, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef860, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef880, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef8a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef8c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef8e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef900, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef920, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef940, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef960, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef980, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef9a0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef9c0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undef9e0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefa00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefa20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefa40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefa60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefa80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefaa0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefac0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefae0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefb00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefb20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefb40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefb60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefb80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefba0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefbc0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefbe0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefc00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefc20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefc40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefc60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefc80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefca0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefcc0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefce0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefd00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefd20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefd40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefd60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefd80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefda0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefdc0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefde0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefe00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefe20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefe40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefe60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefe80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefea0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefec0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undefee0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeff00, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeff20, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeff40, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeff60, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeff80, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeffa0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeffc0, DEBUG_CODE_UNDEF)
	_SDBG(Vector_Undeffe0, DEBUG_CODE_UNDEF)
