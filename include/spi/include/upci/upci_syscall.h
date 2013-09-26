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

#ifndef _UPCI_SYSCALL_H_  // Prevent multiple inclusion
#define _UPCI_SYSCALL_H_

#include <stdlib.h>
#include "hwi/include/common/compiler_support.h"



//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief UPCI CNK Callback
 *
 * This header contains the SPI callbacks for CNK UPCI operations.
 * The functions are called from cnk/src/syscalls/upci_op.cc, core2core.c and and via the
 * context switch code.
 * The function implementations may be found in spi/src/upci/upci_syscall.c
 *
 */
//@}


__BEGIN_DECLS

uint64_t Upci_Do_Syscall(uint64_t op, uint64_t targ, void *pData, size_t dsize);

void Upci_Punit_SwapOut(void *pCfg);
void Upci_Punit_SwapIn(void *pCfg);

void Upci_PerfMon_Exception();

// Perform Punit UPC_P hardware updates within kernel which include a UPC_P lock to prevent concurrent updates to the UPC_P.
void Upci_Kernel_Punit_Init_Mode( int unit, int mode, unsigned ctrMode);
void Upci_Kernel_Punit_Disable( int unit );

struct sPunit_Cfg;  // forward declare for IPI_upc_apply() opaque structure
void Upci_Kernel_Punit_Cfg_Apply(struct sPunit_Cfg *pCfg);




__END_DECLS

#endif
