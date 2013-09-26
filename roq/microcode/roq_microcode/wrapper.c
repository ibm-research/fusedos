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

#define __KERNEL__
#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <hwi/include/bqc/pe_dcr.h>

#include <firmware/include/mailbox.h>
#include <firmware/include/personality.h>
#include <firmware/include/Firmware_Interface.h>
#include <firmware/include/BeDram_inlines.h>
#undef __KERNEL__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <roq_kernel.h>

Firmware_DomainDescriptor_t _descriptor_domain;
#define DD()        _descriptor_domain

__C_LINKAGE void __NORETURN
_enter_wrapper(Firmware_Interface_t *fw_interface)
{
	if (fw_interface->getDomainDescriptor(&_descriptor_domain) != FW_OK) {
		while(1);
	}

	if (ProcessorThreadID() == 0) {
		tlbwe( //_slot(index,
			MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB,
			MAS2_EPN((ROQ_VIRTUAL_MAPPING >> 12)) | MAS2_W(0) | MAS2_I(0) | MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
			//MAS7_3_RPN((0x3c0000000ULL >> 12)) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U3(1),
			MAS7_3_RPN(DD().ddrOrigin >> 12) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U3(1),
			MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
			MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF)
		);
		isync();
	} else {
		while (tlbsx(ROQ_VIRTUAL_MAPPING, MAS5_SGS(0) | MAS5_SLPID(0), MAS6_SPID(0) | MAS6_SAS(0)) == -1) ;;
	}

	asm volatile ("mr 3, %0"::"r"(fw_interface));
	goto *(void*)(ROQ_VIRTUAL_MAPPING | 0x800);
}

__C_LINKAGE void __NORETURN
_enter_wrapper2(Firmware_Interface_t *fw_interface)
{
	if (ProcessorThreadID() == 0) {
			tlbwe( //_slot(index,
				MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB,
				MAS2_EPN((ROQ_VIRTUAL_MAPPING >> 12)) | MAS2_W(0) | MAS2_I(0) | MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
				//MAS7_3_RPN((0x3c0000000ULL >> 12)) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U3(1),
				MAS7_3_RPN(DD().ddrOrigin >> 12) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U3(1),
				MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
				MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF)
			);
			isync();
	} else {
		while (tlbsx(0xc0000000ULL, MAS5_SGS(0) | MAS5_SLPID(0), MAS6_SPID(0) | MAS6_SAS(0)) == -1) ;;
	}

	goto *(void*)(0xc0000824);
}
