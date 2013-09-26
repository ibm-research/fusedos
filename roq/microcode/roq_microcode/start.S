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

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#define __KERNEL__
#include <hwi/include/common/asm_support.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/common/asm_support.h>
#include <hwi/include/common/bgq_alignment.h>

#include <hwi/include/bqc/A2_core.h>
#include <firmware/include/Firmware.h>
#include <firmware/include/fwext/fwext_regs.h>
#include <firmware/include/fwext/fwext_Linkage.h>

#undef __KERNEL__

//#include "linkage.h"

         .file "start.S"

	
//------------------------------------------------------------------------------
// Make the jump into C code at entry point "_test_entry".  We don't futz
// with stack here (that will be done on the other side).
//------------------------------------------------------------------------------
		.extern _enter_domain
		.global _start_domain
		.section ".text.start.domain"
		#type _start_domain,@function	;
_start_domain:
		li     %r0,0
		IMM64( %r2, __FWTST_TOC_BASE )
		bl     _enter_domain
		nop
		b 0
		#.size _start_domain,.-_start_domain
		#.previous
_start_thread_domain:
		li     %r0,0
		IMM64( %r2, __FWTST_TOC_BASE )
		bl auxiliaryEntryPoint_domain
//------------------------------------------------------------------------------

// Machine check interrupts
#define MVEC( label, code )							\
        .align 5;								\
label:										\
        mtspr  SPRN_SPRG3, %r3;	        /* backup r3 */				\
        mfspr  %r3, SPRN_SPRG0;	        /* InterruptState[tid][0] */		\
	mtspr  SPRN_SPRG1, %r3;         /* need to restore %r3 later */         \
	std    %r4, REGS_OFS_GPR4(%r3); /* backup r4 */				\
	li     %r4, code;               /* store interrupt code in r4 */	\
        b      default_interrupt_entry;

// Critical interrupts
#define CVEC( label, code )							\
        .align 5;								\
label:										\
        mtspr  SPRN_SPRG3, %r3;	        /* backup r3 */				\
        mfspr  %r3, SPRN_SPRG0;	        /* InterruptState[tid][0] */		\
        addi   %r3, %r3, REGS_SIZEOF;   /* use InterruptState[tid][1] */	\
	mtspr  SPRN_SPRG1, %r3;         /* need to restore %r3 later */         \
	std    %r4, REGS_OFS_GPR4(%r3); /* backup r4 */				\
	li     %r4, code;               /* store interrupt code in r4 */	\
        b      critical_interrupt_entry;

// Standard interrupts
#define SVEC( label, code )							\
        .align 5;								\
label:										\
        mtspr  SPRN_SPRG3, %r3;	        /* backup r3 */				\
        mfspr  %r3, SPRN_SPRG0;	        /* InterruptState[tid][0] */		\
        addi   %r3, %r3, 2*REGS_SIZEOF; /* use InterruptState[tid][2] */	\
	mtspr  SPRN_SPRG1, %r3;         /* need to restore %r3 later */         \
	std    %r4, REGS_OFS_GPR4(%r3); /* backup r4 */				\
	li     %r4, code;               /* store interrupt code in r4 */	\
        b      default_interrupt_entry;

	.section ".text.domain"

	.align 11

    	.global _vec_domain_MCHK;

//        _PROLOG_KVECTORS( _vec_domain_MCHK )
	// All interrupt vector offsets according to hwi/include/bqc/A2_core.h
	MVEC( _vec_domain_MCHK,     IVO_MCHK      )           // Machine Check
        CVEC( _vec_domain_CI,       IVO_CI        )           // Critical Input
        CVEC( _vec_domain_DEBUG,    IVO_DEBUG     )           // Debug
        SVEC( _vec_domain_DSI,      IVO_DSI       )           // Data Storage Interrupt
        SVEC( _vec_domain_ISI,      IVO_ISI       )           // Instruction Storage Interrupt
        SVEC( _vec_domain_EI,       IVO_EI        )           // External Input
        SVEC( _vec_domain_ALGN,     IVO_ALGN      )           // Alignment
        SVEC( _vec_domain_PROG,     IVO_PROG      )           // Program
        SVEC( _vec_domain_FPU,      IVO_FPU       )           // Floating Point Unavailable
        SVEC( _vec_domain_SC,       IVO_SC        )           // System Call
        SVEC( _vec_domain_APU,      IVO_APU       )           // Auxillary Processo Unavailable
        SVEC( _vec_domain_DEC,      IVO_DEC       )           // Decrementer
        SVEC( _vec_domain_FIT,      IVO_FIT       )           // Fixed Interval Timer
        CVEC( _vec_domain_WDT,      IVO_WDT       )           // WatchDog Timer
        SVEC( _vec_domain_DTLB,     IVO_DTLB      )           // Data TLB Error
        SVEC( _vec_domain_ITLB,     IVO_ITLB      )           // Instruction TLB Error
        SVEC( _vec_domain_VECTOR,   IVO_VECTOR    )           // Vector Unavailable Interrupt
	SVEC( _vec_domain_UNDEF220, IVO_UNDEFINED )
        SVEC( _vec_domain_UNDEF240, IVO_UNDEFINED )
        SVEC( _vec_domain_UNDEF260, IVO_UNDEFINED )
        SVEC( _vec_domain_PDBI,     IVO_PDBI      )           // Processor DoorBell Interrupt
        CVEC( _vec_domain_PDBCI,    IVO_PDBCI     )           // Processor DoorBell Critical Interrupt
        SVEC( _vec_domain_GDBI,     IVO_GDBI      )           // Guest DoorBell Interrupt
        CVEC( _vec_domain_GDBCI,    IVO_GDBCI     )           // Guest DoorBell Critical or Machine Check Interrupt
        CVEC( _vec_domain_EHVSC,    IVO_EHVSC     )           // Embedded Hypervisor System Call
        CVEC( _vec_domain_EHVPRIV,  IVO_EHVPRIV   )           // Embedded Hypervisor Privilege
        CVEC( _vec_domain_LRATE,    IVO_LRATE     )           // LRAT Error Interrupt
	SVEC( _vec_domain_UNDEF360, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF380, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF3A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF3C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF3E0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF400, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF420, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF440, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF460, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF480, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF4A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF4C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF4E0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF500, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF520, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF540, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF560, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF580, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF5A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF5C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF5E0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF600, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF620, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF640, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF660, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF680, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF6A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF6C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF6E0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF700, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF720, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF740, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF760, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF780, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF7A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF7C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF7E0, IVO_UNDEFINED )
        SVEC( _vec_domain_UDEC,     IVO_UDEC      )           // User Decrementer
        SVEC( _vec_domain_PERFMON,  IVO_PERFMON   )           // Performance Monitor
	SVEC( _vec_domain_UNDEF840, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF860, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF880, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF8A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF8C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF8E0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF900, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF920, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF940, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF960, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF980, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF9A0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF9C0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEF9E0, IVO_UNDEFINED )

	SVEC( _vec_domain_UNDEFa00, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFa20, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFa40, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFa60, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFa80, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFAA0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFAC0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFAE0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFB00, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFB20, IVO_UNDEFINED )

	SVEC( _vec_domain_UNDEFB40, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFB60, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFB80, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFBA0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFBC0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFBE0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFC00, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFC20, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFC40, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFC60, IVO_UNDEFINED )

	SVEC( _vec_domain_UNDEFC80, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFCA0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFCC0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFCE0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFD00, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFD20, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFD40, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFD60, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFD80, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFDA0, IVO_UNDEFINED )

	SVEC( _vec_domain_UNDEFDC0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFDE0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFE00, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFE20, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFE40, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFE60, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFE80, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFEA0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFEC0, IVO_UNDEFINED )
	SVEC( _vec_domain_UNDEFEE0, IVO_UNDEFINED )
// if I enable the ones below it's not working anymore. don't ask me why.
//	SVEC( _vec_domain_UNDEFF00, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFF20, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFF40, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFF60, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFF80, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFFA0, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFFC0, IVO_UNDEFINED )
//	SVEC( _vec_domain_UNDEFFE0, IVO_UNDEFINED )

_PROLOG_START(default_interrupt_entry)
default_interrupt_entry:
	// Backup all volatile registers
	// Backup general purpose registers
	std    %r0, REGS_OFS_GPR0(%r3)
        std    %r1, REGS_OFS_GPR1(%r3)
	std    %r2, REGS_OFS_GPR2(%r3)
	// r3 is in SPRN_SPRG3
	// r4 has already been saved
	std    %r5, REGS_OFS_GPR5(%r3)
        std    %r6, REGS_OFS_GPR6(%r3)
	std    %r7, REGS_OFS_GPR7(%r3)
        std    %r8, REGS_OFS_GPR8(%r3)
	std    %r9, REGS_OFS_GPR9(%r3)
        std   %r10, REGS_OFS_GPR10(%r3)
	std   %r11, REGS_OFS_GPR11(%r3)
        std   %r12, REGS_OFS_GPR12(%r3)
	std   %r13, REGS_OFS_GPR13(%r3)

	// Backup special purpose registers
	mfspr  %r5, SPRN_SRR0_IP
	std    %r5, REGS_OFS_IP(%r3)
	mfspr  %r5, SPRN_SRR1_MSR
	std    %r5, REGS_OFS_MSR(%r3)
	mflr   %r5
	std    %r5, REGS_OFS_LR(%r3)
	mfxer  %r5
	std    %r5, REGS_OFS_XER(%r3)
	mfctr  %r5
	std    %r5, REGS_OFS_CTR(%r3)
	mfcr   %r5
	std    %r5, REGS_OFS_CR(%r3)

	// Load address of C interrupt handler function into count register
	mfspr  %r5, SPRN_SPRG4 /* base address of array */
	andi.  %r6, %r4, 0xFE0
	srdi   %r6, %r6, 2     /* code = n*0x20, offset is n*0x08 --> code >> 2 */
	add    %r5, %r5, %r6   /* offset for interrupt class/code */
	ld     %r5, 0(%r5)     /* load interrupt handler address */
	mtspr  SPRN_CTR, %r5

	// Create stack frame
	la %r1,-INT_FRAME_SIZE(%r1)

	// Branch to actual interrupt handler (in count register)
	bctrl

	// Restore registers from memory location in sprg0
        mfspr  %r3, SPRN_SPRG1
//        addi %r3, %r3, 2*REGS_SIZEOF

	// Restore special purpose registers
	ld     %r5, REGS_OFS_IP(%r3)
	mtspr  SPRN_SRR0_IP, %r5
	ld     %r5, REGS_OFS_MSR(%r3)
	mtspr  SPRN_SRR1_MSR, %r5
	ld     %r5, REGS_OFS_LR(%r3)
	mtlr   %r5
	ld     %r5, REGS_OFS_XER(%r3)
	mtxer  %r5
	ld     %r5, REGS_OFS_CTR(%r3)
	mtctr  %r5
	ld     %r5, REGS_OFS_CR(%r3)
	mtcr   %r5

	// Restore general purpose registers
	ld     %r0, REGS_OFS_GPR0(%r3)
        ld     %r1, REGS_OFS_GPR1(%r3)
	ld     %r2, REGS_OFS_GPR2(%r3)
	// r3 is in SPRN_SPRG3
	ld     %r4, REGS_OFS_GPR4(%r3)
	ld     %r5, REGS_OFS_GPR5(%r3)
        ld     %r6, REGS_OFS_GPR6(%r3)
	ld     %r7, REGS_OFS_GPR7(%r3)
        ld     %r8, REGS_OFS_GPR8(%r3)
	ld     %r9, REGS_OFS_GPR9(%r3)
        ld    %r10, REGS_OFS_GPR10(%r3)
	ld    %r11, REGS_OFS_GPR11(%r3)
        ld    %r12, REGS_OFS_GPR12(%r3)
	ld    %r13, REGS_OFS_GPR13(%r3)

	mfspr %r3, SPRN_SPRG3 // restore r3

	rfi
_EPILOG(default_interrupt_entry)

_PROLOG_START(critical_interrupt_entry)
critical_interrupt_entry:
	// Backup all volatile registers
	// Backup general purpose registers
	std    %r0, REGS_OFS_GPR0(%r3)
        std    %r1, REGS_OFS_GPR1(%r3)
	std    %r2, REGS_OFS_GPR2(%r3)
	// r3 is in SPRN_SPRG3
	// r4 has already been saved
	std    %r5, REGS_OFS_GPR5(%r3)
        std    %r6, REGS_OFS_GPR6(%r3)
	std    %r7, REGS_OFS_GPR7(%r3)
        std    %r8, REGS_OFS_GPR8(%r3)
	std    %r9, REGS_OFS_GPR9(%r3)
        std   %r10, REGS_OFS_GPR10(%r3)
	std   %r11, REGS_OFS_GPR11(%r3)
        std   %r12, REGS_OFS_GPR12(%r3)
	std   %r13, REGS_OFS_GPR13(%r3)

	// Backup special purpose registers
	mfspr  %r5, SPRN_CSRR0_IP
	std    %r5, REGS_OFS_IP(%r3)
	mfspr  %r5, SPRN_CSRR1_MSR
	std    %r5, REGS_OFS_MSR(%r3)
	mflr   %r5
	std    %r5, REGS_OFS_LR(%r3)
	mfxer  %r5
	std    %r5, REGS_OFS_XER(%r3)
	mfctr  %r5
	std    %r5, REGS_OFS_CTR(%r3)
	mfcr   %r5
	std    %r5, REGS_OFS_CR(%r3)

	// Load address of C interrupt handler function into count register
	mfspr  %r5, SPRN_SPRG4 /* base address of array */
	andi.  %r6, %r4, 0xFE0
	srdi   %r6, %r6, 2     /* code = n*0x20, offset is n*0x08 --> code >> 2 */
	add    %r5, %r5, %r6   /* offset for interrupt class/code */
	ld     %r5, 0(%r5)     /* load interrupt handler address */
	mtspr  SPRN_CTR, %r5

	// Create stack frame
	la %r1,-INT_FRAME_SIZE(%r1)

	// Branch to actual interrupt handler (in count register)
	bctrl

	// Restore registers from memory location in sprg0
        mfspr  %r3, SPRN_SPRG1
//        addi %r3, %r3, 2*REGS_SIZEOF

	// Restore special purpose registers
	ld     %r5, REGS_OFS_IP(%r3)
	mtspr  SPRN_CSRR0_IP, %r5
	ld     %r5, REGS_OFS_MSR(%r3)
	mtspr  SPRN_CSRR1_MSR, %r5
	ld     %r5, REGS_OFS_LR(%r3)
	mtlr   %r5
	ld     %r5, REGS_OFS_XER(%r3)
	mtxer  %r5
	ld     %r5, REGS_OFS_CTR(%r3)
	mtctr  %r5
	ld     %r5, REGS_OFS_CR(%r3)
	mtcr   %r5

	// Restore general purpose registers
	ld     %r0, REGS_OFS_GPR0(%r3)
        ld     %r1, REGS_OFS_GPR1(%r3)
	ld     %r2, REGS_OFS_GPR2(%r3)
	// r3 is in SPRN_SPRG3
	ld     %r4, REGS_OFS_GPR4(%r3)
	ld     %r5, REGS_OFS_GPR5(%r3)
        ld     %r6, REGS_OFS_GPR6(%r3)
	ld     %r7, REGS_OFS_GPR7(%r3)
        ld     %r8, REGS_OFS_GPR8(%r3)
	ld     %r9, REGS_OFS_GPR9(%r3)
        ld    %r10, REGS_OFS_GPR10(%r3)
	ld    %r11, REGS_OFS_GPR11(%r3)
        ld    %r12, REGS_OFS_GPR12(%r3)
	ld    %r13, REGS_OFS_GPR13(%r3)

	mfspr %r3, SPRN_SPRG3 // restore r3

	rfci
_EPILOG(critical_interrupt_entry)
