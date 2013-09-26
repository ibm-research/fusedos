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

//
// This file is part of the mechanism for generating field offsets and other
// compile-time constants for use in assembly language.  It generates
// initialized data declarations with the desired constants as the
// initialized values.  The file is compiled to assembler (with the same
// cross-compiling toolchain as is used to build the kernel), and the
// resulting .s file is post-processed with awk to create asmConstants.h.
// See cnk/src/Makefile for the procedure.
//

#include <stddef.h>

#include "Kernel.h"
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "signal.h"                                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#include <cnk/include/SPI_syscalls.h>
#include <hwi/include/bqc/BIC.h>

#define CONSTANT(name, value) \
    unsigned long ASM_CONSTANT_ ## name = value

#define FIELD_OFFSET(prefix, type, field) \
    unsigned long ASM_CONSTANT_ ## prefix ## _ ## field = \
	offsetof(type, field)

#define FIELD_OFFSET2(prefix, type, parent, field) \
    unsigned long ASM_CONSTANT_ ## prefix ## _ ## field = \
	offsetof(type, parent.field)

    CONSTANT(REGS_SIZEOF, sizeof(Regs_t));
    CONSTANT(SC_ENTERJAILMODE, __NR_ENTERJAILMODE);
    CONSTANT(SC_EXITJAILMODE, __NR_EXITJAILMODE);
    CONSTANT(SE_ROLLBACK_RC, (Kernel_SpecReturnCode_INVALID |
				Kernel_SpecReturnCode_SEConflict));

    FIELD_OFFSET(REGS, Regs_t, gpr);
    FIELD_OFFSET(REGS, Regs_t, qvr);
    FIELD_OFFSET(REGS, Regs_t, fpscr);
    FIELD_OFFSET(REGS, Regs_t, sprg);
    FIELD_OFFSET(REGS, Regs_t, gsprg);
    FIELD_OFFSET(REGS, Regs_t, ip);
    FIELD_OFFSET(REGS, Regs_t, msr);
    FIELD_OFFSET(REGS, Regs_t, cr);
    FIELD_OFFSET(REGS, Regs_t, lr);
    FIELD_OFFSET(REGS, Regs_t, xer);
    FIELD_OFFSET(REGS, Regs_t, ctr);
    FIELD_OFFSET(REGS, Regs_t, pid);
    FIELD_OFFSET(REGS, Regs_t, mcsr);

    FIELD_OFFSET(KTHR, KThread_t, Reg_State);
    FIELD_OFFSET(KTHR, KThread_t, SyscallReturn);
    FIELD_OFFSET(KTHR, KThread_t, Pending);
    FIELD_OFFSET(KTHR, KThread_t, PendingKThread);
    FIELD_OFFSET(KTHR, KThread_t, PendingNVGPR);
    FIELD_OFFSET(KTHR, KThread_t, pAppProc);

    FIELD_OFFSET(HWTHR, HWThreadState_t, CriticalState);
    FIELD_OFFSET(HWTHR, HWThreadState_t, CriticalStack);
    FIELD_OFFSET(HWTHR, HWThreadState_t, StandardStack);
    FIELD_OFFSET(HWTHR, HWThreadState_t, RollbackState);
    FIELD_OFFSET(HWTHR, HWThreadState_t, pCurrentThread);
    FIELD_OFFSET(HWTHR, HWThreadState_t, SpecStateAddr);
    FIELD_OFFSET(HWTHR, HWThreadState_t, PhysicalPID);
    FIELD_OFFSET(HWTHR, HWThreadState_t, PhysicalSpecPID);
    FIELD_OFFSET(HWTHR, HWThreadState_t, L1P_cfg_pf_sys);
    FIELD_OFFSET(HWTHR, HWThreadState_t, SpecSaveR0);
    FIELD_OFFSET(HWTHR, HWThreadState_t, SpecSaveR6);
    FIELD_OFFSET(HWTHR, HWThreadState_t, SpecSaveR7);

    FIELD_OFFSET(SPEC, SpecState_t, hwt_state);
    FIELD_OFFSET(SPEC, SpecState_t, id_state);

    FIELD_OFFSET(SPEC_THR, SpecThread_t, specid);
    FIELD_OFFSET(SPEC_THR, SpecThread_t, ip);
    FIELD_OFFSET(SPEC_THR, SpecThread_t, gpr1);
    FIELD_OFFSET(SPEC_THR, SpecThread_t, gpr2);

    FIELD_OFFSET(SPEC_CTX, SpecContext_t, priority_key);

    FIELD_OFFSET(BIC, BIC_Device, _ext_int_summary);

    FIELD_OFFSET(L2C, L2Central_t, id);

    FIELD_OFFSET(L2C_ID, L2Central_ID_t, trychangestate);
    FIELD_OFFSET(L2C_ID, L2Central_ID_t, conflict);

    FIELD_OFFSET2(UCTX, struct kern_ucontext_t, uc_mcontext, gp_regs);
    FIELD_OFFSET2(UCTX, struct kern_ucontext_t, uc_mcontext, fp_regs);
    FIELD_OFFSET2(UCTX, struct kern_ucontext_t, uc_mcontext, v_regs);
    FIELD_OFFSET2(UCTX, struct kern_ucontext_t, uc_mcontext, vmx_reserve);
