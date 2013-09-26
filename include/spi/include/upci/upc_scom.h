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

#ifndef _UPC_SCOM_H_
#define _UPC_SCOM_H_

//! \addtogroup upc_spi
//@{
/**
 * \file
 *
 * \brief UPC Firmware Only DCR2PIB Scom operations
 *
 */
//@}

#include <hwi/include/bqc/PhysicalMap.h>
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <firmware/include/Firmware.h>
#include <firmware/include/fwext/fwext.h>
#include <firmware/include/fwext/fwext_lib.h>

#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/testint_tdr.h>

#include "spi/include/kernel/location.h"
#endif                                                                                                                 // FUSEDOS
#include "spi/include/upci/upc_common.h"


__BEGIN_DECLS



//-----------------------------------------------------------------
//   SCOM Kernel-only Functions
//-----------------------------------------------------------------


#if 0
#define BG_DCRPCB_RESET_REQ 0x1000000000000000LL

#define BG_DCRPCB_READ_REQ  0x0000000000000000LL
#define BG_DCRPCB_WRITE_REQ 0x8000000000000000LL
#define BG_DCRPCB_ERROR_CLR 0x0800000000000000LL

__INLINE__
void BgDcrPcbReset()
{
    // reset start
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_RESET_REQ);
    // reset end
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), 0);
}


__INLINE__
void BgDcrPcbErrorClr()
{
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_ERROR_CLR);
}


__INLINE__
uint64_t BgDcrPcbRead (uint64_t nScomReg)
{
    uint64_t address = (_BGQ_SCOMCHIPLET(nScomReg) << 24)
                          | (_BGQ_SCOMPORT(nScomReg) << 16)
                          | _BGQ_SCOMREG(nScomReg);

    // Post Read request to PCB slave
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_READ_REQ | address) ;
    // Fetch data
    return (DCRReadPriv (TESTINT_DCR(DCR_PCB_DATA)));
}


__INLINE__
void BgDcrPcbWrite (uint64_t nScomReg, uint64_t ullScomData)
{
    uint64_t address = (_BGQ_SCOMCHIPLET(nScomReg) << 24)
                          | (_BGQ_SCOMPORT(nScomReg) << 16)
                          | _BGQ_SCOMREG(nScomReg);

    // set data
    DCRWritePriv (TESTINT_DCR(DCR_PCB_DATA), ullScomData);
    // Post write request to PCB slave
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_WRITE_REQ | address) ;
}
#endif



// A2 SCOM Addresses used by UPC
#define A2_SCOM_PCCR0             0x33
#define A2_SCOM_PCCR0_ResetAnd    0x34  //!< Write Mask Anded with contents to reset bits
#define A2_SCOM_PCCR0_SetOr       0x35  //!< Write Mask Ored with contents to set bits
#define A2_SCOM_ABDSR             0x3B  //!< AXU/BX Debug Select Register
#define A2_SCOM_IDSR              0x3C  //!< IU Debug Select Register
#define A2_SCOM_MPDSR             0x3D  //!< MMU/PC Debug Select Register
#define A2_SCOM_XDSR1             0x3E  //!< XU Debug Select Reg 1
#define A2_SCOM_XDSR2             0x3F  //!< XU Debug Select Reg 2
#define A2_SCOM_DEF(slave, regNum) BGQ_SCOMDEF(BGQ_JSEL_CFAM, slave, 0x1, (0x0400 | regNum))



//! \brief: UPC_Init_Scom
//! Kernel only operation to init DCR2PIB to receive scom communications
//! @return current timeout, which is altered by this routine.
//! \note
//! SCOM operations require a larger timeout than the defaults, so this
//! routine returns the current timeout value.
//! Upon completion of scom operations, the timeout should be restored
//! via UPC_Restore_Timeout();
__INLINE__ uint64_t UPC_Init_Scom()
{
    // Get current timeout value
    uint64_t saveto = DCRReadPriv(DC_ARBITER_DCR(ARB_CTRL));

    // set a DCR transaction timeout value of  0xF * 64 + 63 cycles
    DCRWritePriv(DC_ARBITER_DCR(ARB_CTRL),0x00000000060F0007 );

    // Initialise dcr-to-pib/pcb interface
    BgDcrPcbReset();

    Upci_Delay(50);

    return saveto;
}




//! \brief: UPC_Restore_Timeout
//! Kernel only operation to store DCR2PIB timeout returned by UPC_Init_Scom
__INLINE__ void UPC_Restore_Timeout(uint64_t to)
{
    DCRWritePriv(DC_ARBITER_DCR(ARB_CTRL), to );
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS

#if 1
//! \brief: UPC_Enable_Core_Scom
//! Kernel only operations to prep core connections for Scom operations
//! Requires that UPC_Init_Scom be performed first.
//! @param[in]  core  The core index A2.
__INLINE__ void UPC_Enable_Core_Scom(short core)
{
     if (core < 0) core = Kernel_PhysicalProcessorID();

     // core = PhysicalProcessorID();
     unsigned slave = BGQCFAM_CoreToSlave (core);

     // enable chiplet
     BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_SLV_GP3_WOR(slave), 0x8000000000000000ULL);
     //if ((err = upc_scom_error()) != 0x8800000000000000) { PERROR_X("SCOM Write Error: 0x%016lx\n", err); }

     // drop fence in GP0
     BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_GPIO_GPOUT0(slave), 0x3005f00000000000ULL);
     //if ((err = upc_scom_error()) != 0x8800000000000000) { PERROR_X("SCOM Write Error: 0x%016lx\n", err); }

     // switch on clocks
     BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_CC_CLK_REGION(slave), 0x7FFFFFFFFFFFFFFFULL);
     //if ((err = upc_scom_error()) != 0x8800000000000000) { PERROR_X("SCOM Write Error: 0x%016lx\n", err); }

     // drop fence
     BgDcrPcbWrite (BGQ_TI_SCOM_CFAM_SLV_GP3_WAND(slave), ~0x0000202000000000ULL);
     //if ((err = upc_scom_error()) != 0x8800000000000000) { PERROR_X("SCOM Write Error: 0x%016lx\n", err); }
}
#endif



//! UPC_ScomError()
//! Errors from SCOM documented in section 5.3.3 of P7 Pervasive Workbook (Receive Error Register, Tbl 5-19, 5-9)
//!
//! \verbatim
//! Error log register bits 0-31:  (bits 32 correspond to slaves 8 thru 15.
//! Errors for 16, 17 & 18 are duplicate slave positions 1, 2 & 3, when single slave operations
//!    have been used.
//!
//! 0 1 2 3   4 5 6 7  8 9 10 11  12 13 14 15  16 17 18 19  20 21 22 23   24 25 26 27  28 29 30 31
//! slave 03  slave 1   slave 2     slave 3      slave 4      slave 5       slave 6      slave 7
//! R1 err2   R1 err2  R1 err2    R1 err2      R1 err2      R1 err2       R1 err2      R1 err2
//!   1: Response bit. When asserted the slave responded.
//!   2: Error code for slave
//!        000 no error.
//!        001 XSCOM command blocked due pending errors.
//!        010 chiplet offline
//!        011 partial good
//!        100 invalid address / address error / bad access type
//!        101 clock error.
//!        110 parity error / received unexpected packet / wrong packet number
//!        111 timeout
//!   3: This entry corresponds to the PCB master.
//!      In this position the highest error of the PCB master is stored. On any other accesses all
//!      other entries from the corresponding addressed slave(s) is / are valid..
//! \endverbatim
//!
__INLINE__ uint64_t UPC_Scom_Error()
{
    uint64_t error = BgDcrPcbRead (BGQ_TI_SCOM_CFAM_MS_REC_ERR);
    Upci_Delay(50);
    return error;
}

//! Get SCOM error bits from errcode returned by UPC_Scom_Error()
//! Master value is encoded in upper nibble, and value for particular core in lower nibble.
__INLINE__ uint32_t upc_scom_err_bits(uint64_t errcode, unsigned core) {
    uint32_t err = (uint32_t)((errcode >> (64-8)) & 0xf0);
    err |= (errcode >> ((64-8) - ((core % 15) * 4))) & 0x0f;
    return err;
}






//! \brief: UPC_Scom_Read
//! Kernel only operations to read an A2 Scom Register
//! Requires that UPC_Enable_Core_Scom be performed first.
//! @param[in]  core  The core index A2.
//! @param[in]  regNum (e.g. A2_SCOM_PCCR0)
//! @return     value.  UPC_Scom_Error should be tested after the fact.
__INLINE__ uint64_t UPC_Scom_Read(  short core, short regNum) {
    unsigned slave = BGQCFAM_CoreToSlave(core);
    uint64_t result = BgDcrPcbRead( A2_SCOM_DEF(slave, regNum) );
    //upc_scom_error();
    Upci_Delay(50);
    return ( result );
}


#endif                                                                                                                 // FUSEDOS

//! \brief: UPC_Scom_Write
//! Kernel only operations to write an A2 Scom Register
//! Requires that UPC_Enable_Core_Scom be performed first.
//! @param[in]  core  The core index A2.
//! @param[in]  regNum (e.g. A2_SCOM_PCCR0)
//! @param[in]  data
//! @return     none  UPC_Scom_Error should be tested after the fact.
__INLINE__ void UPC_Scom_Write( short core, short regNum, uint64_t data) {
    unsigned slave = BGQCFAM_CoreToSlave(core);
    BgDcrPcbWrite( A2_SCOM_DEF(slave, regNum), data) ;
    //upc_scom_error();
    Upci_Delay(50);
}






__END_DECLS


#endif
