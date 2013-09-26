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

#ifndef _DEBUG_H_ // Prevent multiple inclusion.
#define _DEBUG_H_

#include <hwi/include/common/compiler_support.h>

#if !defined(__ASSEMBLY__)

#endif

// NOTE:  These DEBUG_CODE's must be kept consistent with the corresponding
//        HWTPERFCOUNTER's defined in cnk/include/kcounters.h.
#define DEBUG_CODE_NONE     ( 0)
#define DEBUG_CODE_DEBUG    ( 1)
#define DEBUG_CODE_DSI      ( 2)
#define DEBUG_CODE_ISI      ( 3)
#define DEBUG_CODE_ALGN     ( 4)
#define DEBUG_CODE_PROG     ( 5)
#define DEBUG_CODE_FPU      ( 6)
#define DEBUG_CODE_APU      ( 7)
#define DEBUG_CODE_DTLB     ( 8)
#define DEBUG_CODE_ITLB     ( 9)
#define DEBUG_CODE_VECT     (10)
#define DEBUG_CODE_UNDEF    (11)
#define DEBUG_CODE_PDBI     (12)
#define DEBUG_CODE_PDBCI    (13)
#define DEBUG_CODE_GDBI     (14)
#define DEBUG_CODE_GDBCI    (15)
#define DEBUG_CODE_EHVSC    (16)
#define DEBUG_CODE_EHVPRIV  (17)
#define DEBUG_CODE_LRATE    (18)
#define NUM_DEBUG_CODES     (19)

#endif // Add nothing below this line.
