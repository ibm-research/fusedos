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


//! \addtogroup upci_spi
//@{
//! \file
//!  UPC_P Control Functions
//!
//! Common UPC_P Operations and Utilities (See upc_p_llapi.h) for descriptions
//@}

#include <stdio.h>
#include <string.h>

#define UPC_P_C    // define global vars in H file.
#include "spi/include/upci/upc_p.h"
#include "spi/include/upci/upc_a2.h"




void UPC_P_Clear_Unit( int unit )
{
#ifdef __HWSTUBS__
    memset(dummyUPC_P_Array, 0, 0x5000);
#endif

    upc_p_mmio_t *const upc_p = UPC_P_Addr(unit);

    // make sure it's quiet first
    upc_p->p_config = 0;

    // clear counter config
    int i;
    for (i=0; i<UPC_P_NUM_COUNTERS; i++) {
        // counter config should always default to sync-override.
        // Otherwise a start signal from UPC_C will start counting regardless of control run bit settings.
        upc_p->counter_cfg[i] = UPC_P__COUNTER_CFG__RESET;
    }
    // Reset Counters and disable run
    upc_p->control = UPC_P__CONTROL__RESET_MASK;
    mbar();

    // Clear any counter inversions or edge detect configurations.
    upc_p->cfg_invert = 0;
    upc_p->cfg_edge = 0;

    // Insure any interrupt status is clear
    upc_p->int_status_w1c = UPC_P__INT_STATUS__ALL_STATUS;

    // Clear opcode matches
    upc_p->opcode_cfg = 0;

    // Clear thread combining masks
    upc_p->lp1_tc_config0 = 0;
    upc_p->lp1_tc_config1 = 0;
    upc_p->a2_tc_config = 0;

}



void UPC_P_Init_Unit_Mode( int unit, int mode, UPC_Ctr_Mode_t ctrMode)
{

    UPC_P_Clear_Unit(unit);

    UpciBool_t disconnected = (ctrMode==UPC_CM_DISCON);

	upc_p_mmio_t *const upc_p = UPC_P_Addr(unit);

	uint64_t config;
	config = upc_p->p_config;

	// DD1 & DD2 spare core workaround:
	// Enable Chicken switch if doing independent sync, and disable if counting is synchronized.
    if (ctrMode == UPC_CM_INDEP) {
        config |= UPC_P__CONFIG__DD1MODE_set(1);
    }

    // Now enable the mode
    // this code might be overkill - but I'm assuming the mode inits may diverge from this initial version.

    UPC_P__CONFIG__COUNTING_MODE_insert(config, mode);
    if (!disconnected) {
        UPC_P__CONFIG__JOIN_RING_insert(config, 1);
    }

	switch (mode) {
		case UPC_DISTRIB_MODE:
		    UPC_P__CONFIG__TRACE_SELECT_OPCODE_insert(config, 1UL);
			break;
		case UPC_DETAIL_MODE:
            UPC_P__CONFIG__TRACE_SELECT_OPCODE_insert(config, 1UL);
			break;
		case UPC_TRACE_MODE:
            UPC_P__CONFIG__TRACE_SELECT_OPCODE_insert(config, 0UL);
			break;
		default:
			//>>>>>>>>>>>>>>>>>>>>  What do I do with failures at this level?
			break;
	}

	// Set thread combining registers so thread 0 counts can be received.
    upc_p->lp1_tc_config0 =   UPC_P__TC_CFG0__L1P_E0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E1_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E2_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E3_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E4_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E5_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E6_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E7_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E8_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E9_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E10_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E11_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E12_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E13_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E14_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG0__L1P_E15_TCMT_set(0x8UL);

    upc_p->lp1_tc_config1 =    UPC_P__TC_CFG1__L1P_E16_TCMT_set(0x8UL)
                             | UPC_P__TC_CFG1__L1P_E17_TCMT_set(0x8UL)
                             | UPC_P__TC_CFG1__L1P_E18_TCMT_set(0x8UL)
                             | UPC_P__TC_CFG1__L1P_E19_TCMT_set(0x8UL)
                             | UPC_P__TC_CFG1__L1P_E20_TCMT_set(0x8UL)
                             | UPC_P__TC_CFG1__L1P_E21_TCMT_set(0x8UL);

    upc_p->a2_tc_config  =    UPC_P__TC_CFG2__A2_FU0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_FU1_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_LSU0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_LSU1_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_MMU0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_MMU1_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_IU0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_IU1_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_XU0_TCMT_set(0x8UL)
                            | UPC_P__TC_CFG2__A2_XU1_TCMT_set(0x8UL);


	UPC_P__CONFIG__ENABLE_insert(config, 1);
	upc_p->p_config = config;


}



void UPC_P_Disable_Unit( int unit )
{
    upc_p_mmio_t *const upc_p = UPC_P_Addr(unit);
    // Clear UPC_P Config reg to disable.
    // Specifically interested in disabling:  join_ring, enable bit
    upc_p->p_config = 0;
}



//! \brief: UPC_P_Enable_PM_Ints
void UPC_P_Enable_PM_Ints( int unit, short thread) {
    upc_p_mmio_t *const upc_p = UPC_P_Addr(unit);
    if (thread >= 0) {
        upc_p->p_config |= UPC_P__CONFIG__UPC_C_INT_ENABLE_bit(thread) | UPC_P__CONFIG__BIG_OVF_INT_ENABLE_bit(thread) | UPC_P__CONFIG__UPC_P_INT_ENABLE_bit(thread);
    }
    else {
        upc_p->p_config |= UPC_P__CONFIG__UPC_C_INT_ENABLE_set(0xF) | UPC_P__CONFIG__BIG_OVF_INT_ENABLE_set(0xF) | UPC_P__CONFIG__UPC_P_INT_ENABLE_set(0xF);
    }
}











