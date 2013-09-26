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

#ifndef TRACE_H_ // Prevent multiple inclusion.
#define TRACE_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdint.h>
#include <stdio.h>

#include <cnk/include/Config.h>

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "util.h"
#include "Personality_inlines.h"
#endif                                                                                                                 // FUSEDOS

#include <firmware/include/personality.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <firmware/include/Firmware_Interface.h>
#endif                                                                                                                 // FUSEDOS

#if CONFIG_TRACE_ENABLE

// Trace categories for use as the Msk parameter in the TRACE and TRACEV macros.

#define TRACE_MamboFS   FW_BIT(9)      //! Trace mambo file system (someday this can go away?)
#define TRACE_VirtFS    FW_BIT(10)     //! Trace base virtual file system
#define TRACE_SysioFS   FW_BIT(11)     //! Trace system I/O file system
#define TRACE_StdioFS   FW_BIT(12)     //! Trace standard I/O file system
#define TRACE_ShmFS     FW_BIT(13)     //! Trace shared memory file system
#define TRACE_MemFS     FW_BIT(14)     //! Trace memory read-only ramdisk file system
#define TRACE_LocalFS   FW_BIT(15)     //! Trace memory local file system

#define TRACE_CoreDump  FW_BIT(24)     //! Trace core dumps
#define TRACE_FlightLog FW_BIT(25)     //! Trace Flight Recorder Log
#define TRACE_StaticMap FW_BIT(26)     //! Trace Static Mapper
#define TRACE_Spec      FW_BIT(27)     //! Trace speculation
#define TRACE_Verbs     FW_BIT(28)     //! Trace compute node verbs
#define TRACE_Jobctl    FW_BIT(29)     //! Trace job control
#define TRACE_Toolctl   FW_BIT(30)     //! Trace tool control
#define TRACE_Nodectl   FW_BIT(31)     //! Trace node controller

// Bits 32-63 are currently defined in personality.h. CNK uses the following:

// #define TRACE_SysCall   FW_BIT(41)   //! Trace system calls
// #define TRACE_VMM       FW_BIT(42)   //! Trace Virtual Memory Manager
// #define TRACE_Process   FW_BIT(52)   //! Trace process/thread mapping
// #define TRACE_Sched     FW_BIT(54)   //! Trace scheduler information
// #define TRACE_Futex     FW_BIT(57)   //! Trace futex operations
// #define TRACE_MemAlloc  FW_BIT(58)   //! Trace MMAP and Shared Memory operations
// #define TRACE_Verbose   FW_BIT(63)   //! Verbose tracing modifier (potentially makes any trace point more verbose)

// It seems like CNK should use the following:

// #define TRACE_Debug     FW_BIT(43)   //! Trace debug events (app crashes etc)
// #define TRACE_Exit_Sum  FW_BIT(53)   //! Trace per-core interrupt and error summary on exit()


#define TRACE_MSG_FCN printf
#define TRACE_MSG_FCN_SYSCALL printstrc

// Kernel Low-Core Tracing enabled at compile-time, subject to TraceConfig in Personality.
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "fusedos_config.h"                                                                                                // FUSEDOS
#include "fusedos.h"                                                                                                       // FUSEDOS
#define TRACE( Msk, Args )    do { if ( __UNLIKELY((Msk) & NodeState.TraceConfig) ) { printf("%lu ", get_spc_context(this_spc())->id); TRACE_MSG_FCN Args ; } } while(0)                                                                                   // FUSEDOS
#else                                                                                                                  // FUSEDOS
#define TRACE( Msk, Args )    do { if ( __UNLIKELY((Msk) & NodeState.TraceConfig) ) TRACE_MSG_FCN Args ; } while(0)
#endif                                                                                                                 // FUSEDOS

#define TRACE_FCN( Msk, Fcn ) do { if ( __UNLIKELY((Msk) & NodeState.TraceConfig) ) Fcn; } while(0)

// Enabled when Msk and _BGQ_TRACE_VERBOSE both set
#define TRACEV( Msk, Args )    do { uint64_t trc = NodeState.TraceConfig;    \
                                     uint64_t msk = ((Msk) | TRACE_Verbose); \
                                     if ( __UNLIKELY((msk & trc) == msk) ) TRACE_MSG_FCN Args ; } while(0)


#define TRACEV_FCN( Msk, Fcn ) do { uint64_t trc = NodeState.TraceConfig;    \
                                     uint64_t msk = ((Msk) | TRACE_Verbose); \
                                     if ( __UNLIKELY((msk & trc) == msk) ) Fcn; } while(0)

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#define TRACESYSCALL( Args ) do { if (__UNLIKELY(GetMyProcess()->straceEnabled)) { printf("%lu ", get_spc_context(this_spc())->id); TRACE_MSG_FCN Args ; } } while(0)                                                                                      // FUSEDOS
#else                                                                                                                  // FUSEDOS
#define TRACESYSCALL( Args ) do { if (__UNLIKELY(GetMyProcess()->straceEnabled)) TRACE_MSG_FCN_SYSCALL Args ; } while(0)
#endif                                                                                                                 // FUSEDOS

#else // Not CONFIG_TRACE_ENABLE

// Kernel Low-Core Tracing removed at compile-time
#define TRACE( Msk, Args )    do { /*nothing*/ } while(0)

#define TRACE_FCN( Msk, Fcn ) do { /*nothing*/ } while(0)

// Enabled when Msk and _BGQ_TRACE_VERBOSE both set
#define TRACEV( Msk, Args )    do { /*nothing*/ } while(0)

#define TRACEV_FCN( Msk, Fcn ) do { /*nothing*/ } while(0)

#define TRACESYSCALL(Args)


#endif // CONFIG_TRACE_ENABLE

#endif // __KERNEL and not __ASSEMBLY__

#endif // Add nothing below this line.

