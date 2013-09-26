/*********************************************
 * (2011) Benjamin Krill <krill@de.ibm.com>
 *********************************************/
#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#define __KERNEL__
#include <hwi/include/common/asm_support.h>
#include <hwi/include/bqc/A2_core.h>
#undef __KERNEL__

	.file "wrapper_entry.S"

	.extern _enter_wrapper
	.global _start_wrapper
	#.type _start_wrapper,@function;
_start_wrapper:
	li     %r0,0
	IMM64( %r2, __WRAPPER_TOC_BASE )
	bl     _enter_wrapper
	nop
	b 0
	#.size _start_wrapper,.-_start_wrapper
	#.previous
