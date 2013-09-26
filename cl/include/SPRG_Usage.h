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
#ifndef _SPRG_USAGE_H_ // Prevent multiple inclusion
#define _SPRG_USAGE_H_

#include <hwi/include/common/compiler_support.h>

#include <hwi/include/bqc/A2_core.h>

//
// Core SPRG Usage:
//   sprg0:            Crit      Save R3
//   sprg1:            Crit      Save R4
//   sprg2:            Crit      Save R5
//   sprg3: User R/O   Std/Sysc  Save R3
//   sprg4: User R/O   Std/Sysc  Save R4
//   sprg5: User R/O   Std/Sysc  Save R5
//   sprg6: User R/O   _HWThread_t *pHWThread
//   sprg7: User R/O   SPI_Info: h/w thread id, etc.
//   sprg8:            
//   vrsave: User R/W  Compiler, SPI, Library use.
//
//   gsprg0:           Sysc      Save CR
//   gsprg1:
//   gsprg2:           MChk      Save R3
//   gsprg3:           MChk      Save R4
//
#define SPRG_pHWThread SPRN_SPRG6
#define SPRG_SPIinfo    SPRN_SPRG7
#define SPRG_SPIinfo_ro SPRN_SPRG7RO

// Indices of SPRGs using above convention
#define SPRG_Index_CritR3   (0)
#define SPRG_Index_CritR4   (1)
#define SPRG_Index_CritR5   (2)
#define SPRG_Index_StdR3    (3)
#define SPRG_Index_StdR4    (4)
#define SPRG_Index_StdR5    (5)
#define SPRG_Index_HWThread (6)
#define SPRG_Index_SPIinfo  (7)

// Field Usage within SPRG_SPIinfo (bitshift values)
#define SPRG_SPIinfo_AppLeaderCore 36 // byte 3 bits 0:3 App leader core index of current job
#define SPRG_SPIinfo_NumCores      32 // byte 3 bits 4:7 Cores allocated to current Job 
#define SPRG_SPIinfo_Runnable      24 // byte 4 Number of runnable software threads on this hdwr thread
#define SPRG_SPIinfo_NumThds       16 // byte 5 Number of software threads on this hdwr thread
#define SPRG_SPIinfo_NumProcs       8 // byte 6 Number of processes in the current job
#define SPRG_SPIinfo_HWTindex       0 // byte 7 ProcessorID of the current hardware thread


#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <stdint.h>



#endif // __KERNEL__


#endif // Add nothing below this line.
