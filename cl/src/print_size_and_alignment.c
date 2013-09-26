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
#include <asm/unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <inttypes.h>

#include <stdio.h>
#include <stddef.h>

#include "fusedos_config.h"
#include "fusedos.h"

#define CACHE_LINE_SIZE 128

#define PRINT_OFFSET_INFO(STRUCT, ELEM) do {                                         \
	uint64_t offset = offsetof(STRUCT, ELEM);                                        \
	uint64_t cache_line = offset / CACHE_LINE_SIZE;                                  \
	uint64_t offset_in_line = offset % CACHE_LINE_SIZE;                              \
	printf("%8s in %8s has offset %lu, is in cache line %lu, offset %lu in line\n",  \
	#ELEM, #STRUCT, offset, cache_line, offset_in_line);                             \
	} while(0);

#define PRINT_OFFSET_FOR_PM(STRUCT, ELEM, NAME) do {      \
	uint64_t offset = offsetof(STRUCT, ELEM);             \
	printf("#define SPCC_OFS_%-9s (0x%04lx)\n", #NAME, offset); \
	} while (0);

int main(int argc, char *argv[])
{

	printf("sizeof SPC context is %lu, mod L1 cache line size (128B) is %lu, mod L2 cache line size (256B) is %lu\n",
			sizeof(spc_context_t), sizeof(spc_context_t) % 128, sizeof(spc_context_t) % 256);

	printf("offset of r3 %lu, offset of r8 %lu\n", offsetof(regs_t, gpr[3]), offsetof(regs_t, gpr[8]) );

	printf("offset of ip in regs_t %lu\n", offsetof(regs_t, ip));

	printf("offset of ipi_wakeup in spc_context_t %lu\n", offsetof(spc_context_t, ipi_wakeup));
	printf("offset of start in spc_context_t %lu\n", offsetof(spc_context_t, start));
	printf("offset of command in spc_context_t %lu\n", offsetof(spc_context_t, command));
	printf("offset of ex_code in spc_context_t %lu\n", offsetof(spc_context_t, ex_code));
	//printf("offset of ipi_wakeup in spc_context_t %lu\n", offsetof(spc_context_t, ipi_wakeup));
	//
	printf("\n");
	
	PRINT_OFFSET_INFO(regs_t, gpr[3]);
	PRINT_OFFSET_INFO(regs_t, gpr[8]);

	PRINT_OFFSET_INFO(regs_t, ip);

	PRINT_OFFSET_INFO(spc_context_t, ipi_wakeup);
	PRINT_OFFSET_INFO(spc_context_t, start);
	PRINT_OFFSET_INFO(spc_context_t, command);
	PRINT_OFFSET_INFO(spc_context_t, ex_code);
	PRINT_OFFSET_INFO(spc_context_t, id);

	printf("\n");

	PRINT_OFFSET_FOR_PM(spc_context_t, bic_int_send,      BIS);
	PRINT_OFFSET_FOR_PM(spc_context_t, bic_value,         BV);
	PRINT_OFFSET_FOR_PM(spc_context_t, ppr32,             PPR32);
	PRINT_OFFSET_FOR_PM(spc_context_t, ipi_wakeup,        IPIWU);
	PRINT_OFFSET_FOR_PM(spc_context_t, id,                ID);
	PRINT_OFFSET_FOR_PM(spc_context_t, start,             START);
	PRINT_OFFSET_FOR_PM(spc_context_t, tlb_entry_count,   TLBC);
	PRINT_OFFSET_FOR_PM(spc_context_t, tlb_entry_install, TLBI);
	PRINT_OFFSET_FOR_PM(spc_context_t, spcm_sp,           SPCMSP);
	PRINT_OFFSET_FOR_PM(spc_context_t, spcm_toc,          SPCMTOC);
	PRINT_OFFSET_FOR_PM(spc_context_t, ex_code,           EX);
	PRINT_OFFSET_FOR_PM(spc_context_t, scratch0,          SCR0);
	PRINT_OFFSET_FOR_PM(spc_context_t, scratch1,          SCR1);
	PRINT_OFFSET_FOR_PM(spc_context_t, scratch2,          SCR2);
	PRINT_OFFSET_FOR_PM(spc_context_t, scratch3,          SCR3);

	return 0;
}
