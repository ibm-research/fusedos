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

#ifndef _KERNEL_H_ // Prevent multiple inclusion.
#define _KERNEL_H_

//
// Kernel master-include file.
//
#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <cnk/include/Config.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <cnk/include/kcounters.h>

#include <hwi/include/common/asm_support.h>
#include <hwi/include/common/bgq_bitnumbers.h>
#include <firmware/include/personality.h>
#include <hwi/include/common/bgq_alignment.h>
#include <firmware/include/Firmware_Interface.h>
#endif                                                                                                                 // FUSEDOS

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/BeDram.h>
#include <firmware/include/BeDram_inlines.h>
#endif                                                                                                                 // FUSEDOS
    
#include "atomicops.h"

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <cnk/include/klibtypes.h>
#include "Kernel_Linkage.h"
#endif                                                                                                                 // FUSEDOS
#include "mmap.h"
#include "file.h"
#include <cnk/include/SPRG_Usage.h>
#include "Regs.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "Debug.h"
#endif                                                                                                                 // FUSEDOS
#include "syscalls.h"
#include <cnk/include/SPI_syscalls.h>
#include "Sched.h"
#include "KThread.h"
#include "CoreState.h"
#include <cnk/include/AppState.h>
#include <spi/include/kernel/spec.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "network/network.h"
#include "signal.h"
#endif                                                                                                                 // FUSEDOS
#include "AppProcess.h"
#include "NodeState.h"
#include "vmm.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "futex.h"
#include "timers.h"
#endif                                                                                                                 // FUSEDOS
#include "lock.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "speculation.h"
#include "tool.h"
#include "core2core.h"
#endif                                                                                                                 // FUSEDOS
#include "Personality_inlines.h"
#include "Trace.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "align.h"
#include "powermgmt.h"
#include "CoreDump.h"
#endif                                                                                                                 // FUSEDOS
#include "syscalls/internalsyscalls.h"
//#include "rank.h"

//#include "util.h"

extern void Kernel_Begin( void );
extern void Kernel_Finish( void );
extern void Kernel_Halt(void);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern void Kernel_EntryPrimary(Firmware_Interface_t* fw_interface);
extern void Kernel_EntrySecondary(void* fw_interface);
#endif                                                                                                                 // FUSEDOS


#ifdef __cplusplus
}
#endif

extern NodeState_t NodeState; // main.c

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#define Kernel_WriteFlightLog(a, b, c, d, e, f) {}                                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.

