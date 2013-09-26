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
#include "Kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/l2_central_inlines.h>

#define __KERNEL__ 1

/*! \brief Standard syscall table
 * \var GLIBC_SysCall_Table
 * This table contains a one-to-one mapping of each syscall.  Each position in this table MUST match the associated syscall number
 * that Linux uses (otherwise we'll potentially break the glibc compatibility)
 */

const SyscallTable_t GLIBC_SysCall_Table[] =
{
#define SYSCALL(name, spec) { sc_##name, spec },
#include "syscalls/stdsyscalls.h"
};


/*! \brief SPI specific syscall table
 * \var SPI_SysCall_Table
 * This table contains a one-to-one mapping of each syscall.  External to the kernel, these syscalls are indexed starting at 1024.
 * We start the syscalls at 1024 to avoid overlap with Linux's syscall table.
 *
 */
const SysCall_Fcn_t SPI_SysCall_Table[ SPI_NUM_SYSCALLS ] =
{
#define SYSCALL(name) sc_##name,
#include "cnk/include/SPI_syscalls.h"
};


__C_LINKAGE uint64_t IntHandler_Syscall( uint64_t r3, uint64_t r4, uint64_t r5,
					 uint64_t r6, uint64_t r7, uint64_t r8,
					 uint64_t sc_num, Regs_t *context )
{
    SysCall_Fcn_t sc_fcn;
    uint64_t rc;

    TRACE( TRACE_SysCall, ("(I) Syscall_Handler[%d]: SC=%ld R3=0x%lx R4=0x%lx R5=0x%lx R6=0x%lx R7=0x%lx R8=0x%lx "
                           "IP=0x%lx LR=0x%lx R1=0x%lx SP=0x%08lx pKThr=0x%p\n", ProcessorID(), sc_num,
                           r3, r4, r5, r6, r7, r8, context->ip, context->lr, context->gpr[1],
                           (long)CurrentStackAddress(), GetMyKThread()) );
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SYSCALLAT, sc_num, context->ip, context->lr, context->gpr[1]);
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SYSCALLEN, r3, r4, r5, r6);
    GetMyKThread()->syscallNum = sc_num;
    
    if ( (sc_num >= SYSCALL_START)                                  &&
         (sc_num < (SYSCALL_START + SPI_NUM_SYSCALLS))         &&
         (sc_fcn = SPI_SysCall_Table[ sc_num - SYSCALL_START ])   )
    {
        rc = sc_fcn( r3, r4, r5, r6, r7, r8 );
    }
    else if ( (sc_num < sizeof(GLIBC_SysCall_Table) / sizeof(GLIBC_SysCall_Table[0])) &&
              (sc_fcn = GLIBC_SysCall_Table[ sc_num ].call) )
    {
        if((GLIBC_SysCall_Table[sc_num].generateJMV) && 
           ((SPEC_GetSpeculationIDSelf_priv() & 0xff) > 0x01) &&
           (Speculation_CheckJailViolation(context)))
        {
            rc = context->gpr[3]; // GPR3 is not restored from Reg_State in syscall exception flow
        }
        else
            rc = sc_fcn( r3, r4, r5, r6, r7, r8 );
    }
    else
    {
	rc = CNK_RC_FAILURE(ENOSYS);
    }
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SYSCALLRT, rc, GetMyKThread()->Pending, 0, 0);

    if ( CNK_RC_IS_FAILURE(rc) )
    {
        TRACE( TRACE_SysCall, ("(I) SysCall_Handler[%d]: SC=%ld errno=%d\n", ProcessorID(), sc_num, CNK_RC_ERRNO(rc)) );
    }

    CountException(CNK_HWTPERFCOUNT_SYSCALL);

    return rc;
}
#endif                                                                                                                 // FUSEDOS

static char rc_string_space[68][32];

char *CNK_RC_STRING(uint64_t rc)
{
    char *myspace = rc_string_space[ProcessorID()];
    if (CNK_RC_IS_SUCCESS(rc))
    {
	snprintf(myspace, sizeof(rc_string_space[0]), "%ld", CNK_RC_VALUE(rc));
    }
    else
    {
	snprintf(myspace, sizeof(rc_string_space[0]), "ERRNO-%d", CNK_RC_ERRNO(rc));
    }
    return myspace;
}

char *CNK_RC_STRING_HEX(uint64_t rc)
{
    char *myspace = rc_string_space[ProcessorID()];
    if (CNK_RC_IS_SUCCESS(rc))
    {
	snprintf(myspace, sizeof(rc_string_space[0]), "0x%lx", CNK_RC_VALUE(rc));
    }
    else
    {
	snprintf(myspace, sizeof(rc_string_space[0]), "ERRNO-%d", CNK_RC_ERRNO(rc));
    }
    return myspace;
}
