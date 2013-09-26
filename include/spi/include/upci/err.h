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

#ifndef _UPCI_ERR_H_  // Prevent multiple inclusion
#define _UPCI_ERR_H_


//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief UPCI SPI Error Values
 *
 */
//@}

//! Error values to be captured in "int" variables to remain consistent with "syscall" return variables
//! - 0 means successful
//! - error codes are negative, and values from -999 thru -1 map to negative errno values (usually returned by syscalls)
//! - errors from -1999 thru -1000 are UPCI errors
//! - errors from -2999 thru -2000 are Bgpm Errors
//! - positive values from 3000 thru 4000 are Bgpm Warning values
//! WARNING: The UPCI Error defines below must remain consistent with values in
//! - bgpm/include/err.h
//! - bgpm/src/err.cc

#define UPCI_EOK                            (0) //!<    operation successful

#define UPCI_MIN_ERRNO                      (-1000)
#define UPCI_EINTERNAL                      (-1000) //!<    Internal Error Encountered - see ??? for more information
#define UPCI_ENOTIMPL                       (-1001) //!<    operation or mode not implemented (yet)
#define UPCI_EINV_EVENT                     (-1002) //!<    invalid event
#define UPCI_EINV_PARM                      (-1003) //!<    invalid parm passed to function
#define UPCI_ENOT_INIT                      (-1004) //!<    not init yet
#define UPCI_ERESERVED                      (-1005) //!<    Generic Resource Already Reserved
#define UPCI_ESIGRESERVED                   (-1006) //!<    A2 Signals Reserved (possibles for event are all used)
#define UPCI_ECTRRESERVED                   (-1007) //!<    Counter Reserved (possibles for event are all used)
#define UPCI_EL1PMODERESERVED               (-1008) //!<    Current L1p mode for this core conflicts with requested event L1p mode
#define UPCI_ETHRDCORE_MISMATCH             (-1009) //!<    EventRec thread or core not equal to prior added events (PunitConfig)
#define UPCI_ESYSCALL_DATASIZE              (-1010) //!<    Syscall data size mismatch. Insure that application and kernel have been compiled with equal driver versions
#define UPCI_ESYSCALL_NOTIMPL               (-1011) //!<    Syscall is not implemented yet
#define UPCI_ESYSCALL_INVALID               (-1012) //!<    Syscall operation is invalid
#define UPCI_ESYSCALL_RETRY                 (-1013) //!<    (Internal Value) Retry this syscall - locked by another thread
#define UPCI_EMODE_MISMATCH                 (-1014) //!<    Syscall requested mode (upc or ctr) from Upci_Mode_Init does not match active hardware modes
#define UPCI_EMODE_MISMATCH_UNIT            (-1015) //!<    Syscall requested detailed or trace mode unit number does not match active hardware unit
#define UPCI_EINV_EVT_IDX                   (-1016) //!<    Invalid event index; no event configured at given index within event set
#define UPCI_EINV_OVF                       (-1017) //!<    Invalid Ovf Period - period value must be less than 63 bits (< 0x8000_0000_0000_0000)
#define UPCI_ERESERVED1                     (-1018) //!<    Unused reserved error value
#define UPCI_EINV_OPCTRMODE                 (-1019) //!<    Counter mode is invalid for this operation (can't set ovf if in sync mode)
#define UPCI_EINV_UKVER_MISMATCH            (-1020) //!<    Compiled version mismatch between User and Kernel syscall structures
#define UPCI_ETHD_CTRS_RSV                  (-1021) //!<    Core base pm counters normally used by this hwthread are already reserved by a different hwthread
#define UPCI_ECSEVT_REQALLTHDS              (-1022) //!<    Events with core-shared scope require that all core counters are owned by single thread

#define UPCI_MAX_ERRNO                      (-1022)




#endif
