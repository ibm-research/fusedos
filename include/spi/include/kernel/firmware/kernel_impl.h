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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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
/*                                                                  */
/* Created by:                                                      */
/*      Jay S. Bryant (jsbryant@us.ibm.com)                         */
/*      January, 2010                                               */
/*                                                                  */

#ifndef  _KERNEL_FIRMWARE_KERNEL_IMPL_H_
#define  _KERNEL_FIRMWARE_KERNEL_IMPL_H_

/**
 * \file kernel_impl.h
 *
 * \brief C Header File containing firmware specific definitions for the Messaging Unit SPIs
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <hwi/include/bqc/A2_inlines.h>

/**
 * \brief MU SPI Assertion
 *
 */
#ifndef MUSPI_assert
#include <assert.h>
#define MUSPI_assert(x) assert(x)
#endif

#endif /* _KERNEL_FIRMWARE_KERNEL_IMPL_H_ */
