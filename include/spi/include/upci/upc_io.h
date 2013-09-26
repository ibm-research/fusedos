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

#ifndef _UPC_IO_H_
#define _UPC_IO_H_

//! \addtogroup upci_spi
//@{
//! \file
//! UPC Low Level API Operation Definitions for "other" misc units.
//@}




#include <hwi/include/bqc/PhysicalMap.h>
#include <hwi/include/bqc/A2_inlines.h>
//#include <hwi/include/bqc/MU_Addressing.h>
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/devbus_dcr.h>
#include <hwi/include/bqc/pe_dcr.h>

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "spi/include/kernel/location.h"
#endif                                                                                                                 // FUSEDOS
#include "spi/include/kernel/upc_hw.h"

#include "upc_p.h"
#include "upc_c.h"


__BEGIN_DECLS

#define UPC_IO_Counter_Range 64  //!< # of possible I/O Counter indicies - see IOunitEvtTbl for an exact list.

//! \brief: UPC_MU_EnableUPC
//! Enable unit to send events to UPC_P
//! Note: Privileged Kernel only routine.
//! @param[in]  indepStart  independent start or wait for upc_c sync
__INLINE__ void UPC_MU_EnableUPC(UpciBool_t indepStart)
{
    //DCRWritePriv(MU_DCR(RESET), MU_DCR__RESET__UPC_set(0));
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1S),
            MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_set(1ULL) |
            MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 0 : 1ULL) ));
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1C),
            MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 1ULL : 0) ));
    DCRWriteCommon(MU_DCR(UPC_MU_CONFIG),
            MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_set(1ULL) |
            MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_set( (indepStart ? 1ULL : 0) ) );
    mbar();
}


//! \brief: UPC_MU_DisableUPC
__INLINE__ void UPC_MU_DisableUPC()
{
    DCRWriteCommon(MU_DCR(UPC_MU_CONFIG),
            MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_set(1));  // gain control over ctrs 1st
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1C),       // ensure stopped
            MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_set(1));
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1S),       // Reset ctrs
            MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_set(1));
    DCRWriteCommon(MU_DCR(UPC_MU_CONFIG),                    // disable ring
            MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_set(0) |
            MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_set(1));
    mbar();
}



//! \brief: UPC_PCIe_EnableUPC
//! Enable unit to send events to UPC_P
//! @param[in]  indepStart  Start counting immediatly or wait for upc_c sync
__INLINE__ void UPC_PCIe_EnableUPC(UpciBool_t indepStart)
{
    DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1S),
            PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_set(1) |
            PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 0 : 1)) );
    DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1C),
            PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 1 : 0)) );
    DCRWriteCommon(PE_DCR(UPC_PCIE_CONFIG),
            PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_set(1) |
            PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_set( (indepStart ? 1 : 0) ) );
    mbar();
}



//! \brief: UPC_PCIe_DisableUPC
__INLINE__ void UPC_PCIe_DisableUPC()
{
    DCRWriteCommon(PE_DCR(UPC_PCIE_CONFIG),
            PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_set(1));   // gain control over ctrs
    DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1C),
            PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_set(1));
    DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1S),
            PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_set(1));
    DCRWriteCommon(PE_DCR(UPC_PCIE_CONFIG),
            PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_set(0) |
            PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_set(1));
    mbar();
}



//! \brief: UPC_DevBus_EnableUPC
//! Enable unit to send events to UPC_P
//! @param[in]  indepStart  Start counting immediatly or wait for upc_c sync
__INLINE__ void UPC_DevBus_EnableUPC(UpciBool_t indepStart)
{
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1S),
            DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_set(1) |
            DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 0 : 1) ));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1C),
            DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_set( (indepStart ? 1 : 0) ));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_CONFIG),
            DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_set(1) |
            DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_set( (indepStart ? 1 : 0) ) );
    mbar();
}



//! \brief: UPC_DevBus_DisableUPC
__INLINE__ void UPC_DevBus_DisableUPC()
{
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_CONFIG),
            DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_set(1));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1C),
            DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_set(1));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1S),
            DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_set(1));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_CONFIG),
            DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_set(0) |
            DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_set(1));
    mbar();
}



//! \brief: UPC_IO_EnableUPC
//! Enable the I/O counter units
//! @param[in]  indepStart  Start counting immediatly or wait for upc_c sync
//! @param[in]  nodeConfig  personality node config.  for example:
//! \verbatim
//!    Personality_t personality;
//!    Kernel_GetPersonality(&personality, sizeof(Personality_t));
//!    uint64_t nodeConfig = personality.Kernel_Config.NodeConfig;
//! \endverbatim

__INLINE__ void UPC_IO_EnableUPC(UpciBool_t indepStart, uint64_t nodeConfig) {
    if (!(nodeConfig & PERS_ENABLE_FPGA)) { // we are not on FPGA
        UPC_MU_EnableUPC(indepStart);
        if (nodeConfig & PERS_ENABLE_PCIe) UPC_PCIe_EnableUPC(indepStart);
    }
    UPC_DevBus_EnableUPC(indepStart);
}



//! \brief: UPC_IO_DisableUPC
//! Disable the I/O counter units
//! @param[in]  nodeConfig  personality node config.
__INLINE__ void UPC_IO_DisableUPC(uint64_t nodeConfig) {
    if (!(nodeConfig & PERS_ENABLE_FPGA)) { // we are not on FPGA
        UPC_MU_DisableUPC();
        if (nodeConfig & PERS_ENABLE_PCIe) UPC_PCIe_DisableUPC();
    }
    UPC_DevBus_DisableUPC();
}



//! \brief: UPC_IO_Clear()
//! Clear the UPC_IO_Counters
//! @param[in]  nodeConfig  personality node config.
void UPC_IO_Clear(uint64_t nodeConfig);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS


//! \brief: UPC_IO_Start()
//! Start the UPC_IO_Counters
//! @param[in]  nodeConfig  personality node config.
__INLINE__ void UPC_IO_Start(uint64_t nodeConfig)
{
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1S), MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_set(1));
    if (nodeConfig & PERS_ENABLE_PCIe) DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1S), PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_set(1));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1S), DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_set(1));
}



//! \brief: UPC_IO_Stop()
//! Stop the UPC_IO_Counters
//! @param[in]  nodeConfig  personality node config.
__INLINE__ void UPC_IO_Stop(uint64_t nodeConfig)
{
    DCRWriteCommon(MU_DCR(UPC_MU_COUNTER_CONTROL_W1C), MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_set(1));
    if (nodeConfig & PERS_ENABLE_PCIe) DCRWriteCommon(PE_DCR(UPC_PCIE_COUNTER_CONTROL_W1C), PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_set(1));
    DCRWriteCommon(DEVBUS_DCR(UPC_DB_COUNTER_CONTROL_W1C), DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_set(1));
}



//! UPC IO Hardwired Counter indicies




//! \brief: UPC_IO_ReadEvtCtr()
//! Retrieve a specific I/O Ctr Value given an I/O Event Number
//! @param  evtNum  From I/O Event Table
//! \note UPC_IO_ReadEvtCtr function really intended for SPI testing,
//!       and the overhead won't be needed in within Bgpm.
uint64_t UPC_IO_ReadEvtCtr(unsigned evtNum);




//! UPC_IO_SnapShot_t
//! Structure to hold a debug snapshot of all the upc_c I/O counters.
typedef struct sUPC_IO_SnapShot {
    uint64_t ctr[UPC_IO_Counter_Range];
} UPC_IO_SnapShot_t;



//! UPC_IO_GetSnapShot
//! Read UPC_C counters to get I/O counter snapshot
void  UPC_IO_GetSnapShot(UPC_IO_SnapShot_t *pss);



//! UPC_IO_DumpSnapShot
//! Print snapshot of all I/O counters
void  UPC_IO_DumpSnapShot(unsigned indent, UPC_IO_SnapShot_t *o);



//! UPC_IO_DumpState
//! Print UPC register state of I/O units
//! @param[in]  indent      indent level of print output
//! @param[in]  nodeConfig  personality node config.
void  UPC_IO_DumpState(unsigned indent, uint64_t nodeConfig);


#endif                                                                                                                 // FUSEDOS
__END_DECLS

#endif


