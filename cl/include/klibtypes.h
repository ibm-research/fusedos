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
#ifndef _KERNEL_TYPES_H_ // Prevent multiple inclusion.
#define _KERNEL_TYPES_H_

#include <hwi/include/common/compiler_support.h>

#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#include <cnk/include/Config.h>

#include <stdint.h>

#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/common/bgq_bitnumbers.h>

#define K_ATOMIC           __attribute__ ((__section__ (".atomics")))

typedef volatile int32_t Futex_t;  // lowlevellock.h uses 32bit locks

#ifndef __cplusplus
typedef char bool;
#endif

#endif // __KERNEL__ and not __ASSEMBLY__

#endif // Add nothing below this line.

