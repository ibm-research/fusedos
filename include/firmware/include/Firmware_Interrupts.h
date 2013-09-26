/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/* Blue Gene/Q                                                      */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/* US Government Users Restricted Rights - Use, duplication or      */
/*   disclosure restricted by GSA ADP Schedule Contract with IBM    */
/*   Corp.                                                          */
/*                                                                  */
/* This software is available to you under the Eclipse Public       */
/* License (EPL).                                                   */
/*                                                                  */
/*  --------------------------------------------------------------- */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */


#ifndef _FIRMWARE_INTERRUPTS_H_ // Prevent multiple inclusion
#define _FIRMWARE_INTERRUPTS_H_

#include <hwi/include/bqc/BIC.h>

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <hwi/include/bqc/bedram_dcr.h>
#include <hwi/include/bqc/cs_dcr.h>
#include <hwi/include/bqc/dc_arbiter_dcr.h>
#include <hwi/include/bqc/dr_arb_dcr.h>
#include <hwi/include/bqc/devbus_dcr.h>
#include <hwi/include/bqc/en_dcr.h>
#include <hwi/include/bqc/gea_dcr.h>
#include <hwi/include/bqc/l1p_mmio.h>
#include <hwi/include/bqc/l2_dcr.h>
#include <hwi/include/bqc/l2_central_dcr.h>
#include <hwi/include/bqc/l2_counter_dcr.h>
#include <hwi/include/bqc/ms_genct_dcr.h>
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/pe_dcr.h>
#endif                                                                                                                 // FUSEDOS


// +--------------------------------------------------------------------------------------------+
// | The following mask defines the PUEA status lanes owned by firmware.  Kernels are expected  |
// | to leave thise lanes alone or risk losing firmware functionality, such as RAS.             |
// +--------------------------------------------------------------------------------------------+

#define FW_RESERVED_PUEA_STATUS   PUEA_INTERRUPT_STATUS_GEA(0)


// +--------------------------------------------------------------------------------------------+
// | The following masks define the GEA status lanes in use by firmware.  Kernels should assume |
// | that these GEA input sources are in use by firmware.  They should not be remapped to other |
// | lanes.                                                                                     |
// +--------------------------------------------------------------------------------------------+

#define FW_RESERVED_GEA_STATUS_0 (					\
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P0_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P1_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P2_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P3_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P4_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P5_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P6_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P7_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P8_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P9_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P10_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P11_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P12_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P13_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P14_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P15_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__L1P16_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__DDR0_RT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_0__DDR1_RT_INT_set(1) | \
				  0 )

#define FW_RESERVED_GEA_STATUS_1 (					\
				  GEA_DCR__GEA_INTERRUPT_STATUS1__L2S_INT_set(-1) | \
				  GEA_DCR__GEA_INTERRUPT_STATUS1__L2C_INT_set(1)  | \
				  GEA_DCR__GEA_INTERRUPT_STATUS1__ND_INT_set(1)   | \
				  0 )


#define FW_RESERVED_GEA_STATUS_2 (					\
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__CS_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__DB_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__DCR_ARB_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__EDRAM_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__EN_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__GEA_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__L2CNTR0_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__L2CNTR1_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__L2CNTR2_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__L2CNTR3_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__MSGC_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__MU_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__PCIE_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__SD0_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__SD1_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__TESTINT_INT_set(1) | \
				  GEA_DCR__GEA_MAPPED_INTERRUPT_STATUS0_2__UPC_INT_set(1) | \
				  0 )


// +--------------------------------------------------------------------------------------------+
// | The following masks define the interrupt bits from the various interrupt status registers  |
// | throughout the BQC.  The masks have the name FW_RESERVED_*_BITS where * is (usually) the   |
// | name of a DCR.                                                                             |
// +--------------------------------------------------------------------------------------------+


// Unit: BeDRAM.
// Note: the "aligned" interrupt bit is not used by firmware but is considered reserved for
//       firmware use.

#define FW_RESERVED_BEDRAM_DCR_BEDRAM_INTERRUPT_STATUS_BITS ( 0 |	\
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_DB_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_PIPE_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_REF_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_RES_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_SCB_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_ERR_PARITY_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_DB_ECC_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_EDRAM_ECC_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_PIPE_ECC_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_ECC_CE_RAISE_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_ALIGNED_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_BYTE_SIZE_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__BEDRAM_NON_PRIV_set(1) | \
							      BEDRAM_DCR__BEDRAM_INTERRUPT_STATUS__LOCAL_RING_set(1) | \
							      0)


// Unit: ClockStop

#define FW_RESERVED_CS_DCR_CLOCKSTOP_INTERRUPT_STATE_BITS ( 0 |		\
							    CS_DCR__CLOCKSTOP_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							    CS_DCR__CLOCKSTOP_INTERRUPT_STATE__STOPPED_set(1) | \
							    0)



// Unit: DCR Arbiter
#define FW_RESERVED_DC_ARBITER_DCR_INT_REG_BITS ( 0 |			\
						  DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_set(1) | \
						  DC_ARBITER_DCR__INT_REG__INV_MON_STATE_set(1) | \
						  DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_set(1) | \
						  DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_set(1) | \
						  DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_set(1) | \
						  DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_set(1) | \
						  DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_set(1) | \
						  DC_ARBITER_DCR__INT_REG__DATA_ERR_set(1) | \
						  DC_ARBITER_DCR__INT_REG__ADD_ERR_set(1) | \
						  0)


// Unit: DDR Arbiter

#define FW_RESERVED_DR_ARB_DCR_L2_INTERRUPT_STATE_BITS ( 0 |		\
							 DR_ARB_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__MC_ERR_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__MCS_ERR_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__IOM_ERR_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__XSTOP_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__SPEC_ATTN_set(1) | \
							 DR_ARB_DCR__L2_INTERRUPT_STATE__RECOV_ERR_set(1) | \
							 0)




// Unit: DevBus

#define FW_RESERVED_DEVBUS_DCR_DB_INTERRUPT_STATE_BITS ( 0 |		\
							 DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_set(1) | \
							 DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_set(1) | \
							 0)





// Unit:  EnvMon
// Notes: We are currently assuming that firmware will own EvMon interrupts.

#define FW_RESERVED_EN_DCR_ENVMON_INTERRUPT_STATE_BITS ( 0 |		\
							 EN_DCR__ENVMON_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__EXCEEDS_EN0_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__EXCEEDS_EN1_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__EXCEEDS_EN2_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__EXCEEDS_EN3_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__EXCEEDS_EN4_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__GENERAL_INTERRUPT_EN5_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__LESSTHAN_EN0_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__LESSTHAN_EN1_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__LESSTHAN_EN2_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__LESSTHAN_EN3_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__LESSTHAN_EN4_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__FSM_CHECKSUM_FAIL_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__FSM_SEQ_FAIL_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__TVSENSE_EXCEEDS_MAX_set(1) | \
							 EN_DCR__ENVMON_INTERRUPT_STATE__TVSENSE_LESSTHAN_MIN_set(1) | \
							 0)




// Unit: GEA
// Note: Global Timer interrupts are delgated to the kernels

#define FW_RESERVED_GEA_DCR_GEA_INTERRUPT_STATE_BITS ( 0 |		\
						       GEA_DCR__GEA_INTERRUPT_STATE__DEVBUS_CTL_PERR_set(1) | \
						       0)




// Unit: L2
// Note: UPC counter overflow interrupts are delgated to the kernels

#define FW_RESERVED_L2_DCR_L2_INTERRUPT_STATE_BITS ( 0 |		\
						     L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__COH_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__COH_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__EDR_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__EDR_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__LRU_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__LRU_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__SPR_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__SPR_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__RQ_CE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__RQ_UE_set(1) | \
						     L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_set(1) | \
						     0)




#define FW_RESERVED_L2_CENTRAL_DCR_L2C_INTERRUPT_STATE_BITS ( 0 |	\
							      L2_CENTRAL_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							      L2_CENTRAL_DCR__L2_INTERRUPT_STATE__ECC_UE_set(1) | \
							      L2_CENTRAL_DCR__L2_INTERRUPT_STATE__ECC_CE_set(1) | \
							      0)


#define FW_RESERVED_L2_COUNTER_DCR_L2CTR_INTERRUPT_STATE_BITS ( 0 |	\
								L2_COUNTER_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(1) | \
								L2_COUNTER_DCR__L2_INTERRUPT_STATE__BDRY_PAR_ERR_set(1) | \
								0)




#define FW_RESERVED_MS_GENCT_DCR_MSGC_INTERRUPT_STATE_BITS ( 0 |	\
							     MS_GENCT_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							     MS_GENCT_DCR__L2_INTERRUPT_STATE__TIMEOUT_E_set(1) | \
							     0)







#define FW_RESERVED_MU_DCR_ARLOG_INTERRUPTS_BITS ( 0 |			\
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_set(1) | \
						   MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_set(1) | \
						   0)

#define FW_RESERVED_MU_DCR_ECC_COUNT_INTERRUPTS_BITS ( 0 |		\
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_set(1) | \
						       MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_set(1) | \
						       0)

#define FW_RESERVED_MU_DCR_FIFO_INTERRUPTS_BITS ( 0 |	\
						  0)

#define FW_RESERVED_MU_DCR_ICSRAM_INTERRUPTS_BITS ( 0 |			\
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_set(1) | \
						    MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_set(1) | \
						    0)

#define FW_RESERVED_MU_DCR_IME_INTERRUPTS_BITS ( 0 |			\
						 MU_DCR__IME_INTERRUPTS__IME_ERR0_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR1_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR2_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR3_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR4_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR5_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR6_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR7_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR8_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR9_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR10_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR11_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR12_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR13_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR14_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR15_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR16_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR17_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR18_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR19_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR20_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR21_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR22_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR23_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR24_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR25_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR26_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR27_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR28_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR29_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR30_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR31_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR32_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR33_set(1) | \
						 MU_DCR__IME_INTERRUPTS__IME_ERR34_set(1) | \
						 0)

#define FW_RESERVED_MU_DCR_IMU_ECC_INTERRUPTS_BITS ( 0 |		\
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_set(1) | \
						     MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_set(1) | \
						     0)

#define FW_RESERVED_MU_DCR_MASTER_PORT0_INTERRUPTS_BITS ( 0 |		\
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_set(1) | \
							  MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_set(1) | \
							  0)

#define FW_RESERVED_MU_DCR_MASTER_PORT1_INTERRUPTS_BITS ( 0 |		\
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_set(1) | \
							  MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_set(1) | \
							  0)

#define FW_RESERVED_MU_DCR_MASTER_PORT2_INTERRUPTS_BITS ( 0 |		\
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_set(1) | \
							  MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_set(1) | \
							  0)

#define FW_RESERVED_MU_DCR_MCSRAM_INTERRUPTS_BITS ( 0 |			\
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_set(1) | \
						    MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_set(1) | \
						    0)

#define FW_RESERVED_MU_DCR_MISC_ECC_CORR_INTERRUPTS_BITS ( 0 |		\
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_set(1) | \
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_set(1) | \
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_set(1) | \
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_set(1) | \
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_set(1) | \
							   MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_set(1) | \
							   0)

#define FW_RESERVED_MU_DCR_MISC_INTERRUPTS_BITS ( 0 |			\
						  MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__RAN_ERR0_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__MU_ERR0_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__MU_ERR1_set(1) | \
						  MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_set(1) | \
						  0)

#define FW_RESERVED_MU_DCR_MMREGS_INTERRUPTS_BITS ( 0 |			\
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_set(1) | \
						    MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_set(1) | \
						    0)

#define FW_RESERVED_MU_DCR_RCSRAM_INTERRUPTS_BITS ( 0 |			\
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_set(1) | \
						    MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_set(1) | \
						    0)

#define FW_RESERVED_MU_DCR_RME_INTERRUPTS0_BITS ( 0 |			\
						  MU_DCR__RME_INTERRUPTS0__RME_ERR0_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR1_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR2_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR3_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR4_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR6_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR7_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR8_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR9_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR10_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR12_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR13_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR14_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR15_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR16_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR18_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR19_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR20_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR21_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR22_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR24_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR25_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR26_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR27_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR28_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR30_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR31_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR32_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR33_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR34_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR36_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR37_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR38_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR39_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR40_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR42_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR43_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR44_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR45_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR46_set(1) | \
						  MU_DCR__RME_INTERRUPTS0__RME_ERR48_set(1) | \
						  0)

#define FW_RESERVED_MU_DCR_RME_INTERRUPTS1_BITS ( 0 |			\
						  MU_DCR__RME_INTERRUPTS1__RME_ERR49_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR50_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR51_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR52_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR54_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR55_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR56_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR57_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR58_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR60_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR61_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR62_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR63_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR64_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR66_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR67_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR68_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR69_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR70_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR72_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR73_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR74_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR75_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR76_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR78_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR79_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR80_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR81_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR82_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR84_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR85_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR86_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR87_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR88_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR90_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR91_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR92_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR93_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR94_set(1) | \
						  MU_DCR__RME_INTERRUPTS1__RME_ERR96_set(1) | \
						  0)

#define FW_RESERVED_MU_DCR_RMU_ECC_CORR_INTERRUPTS_BITS ( 0 |		\
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_set(1) | \
							  MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_set(1) | \
							  0)

#define FW_RESERVED_MU_DCR_RMU_ECC_INTERRUPTS_BITS ( 0 |		\
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_set(1) | \
						     MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_set(1) | \
						     0)

#define FW_RESERVED_MU_DCR_RPUTSRAM_INTERRUPTS_BITS ( 0 |		\
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_set(1) | \
						      MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_set(1) | \
						      0)

#define FW_RESERVED_MU_DCR_SLAVE_PORT_INTERRUPTS_BITS ( 0 |		\
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_set(1) | \
							MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_set(1) | \
							0)




#define FW_RESERVED_L1P_ESR__L1P_ESR_STATE_BITS ( 0 |			\
						  L1P_ESR_err_luq_ovfl | \
						  L1P_ESR_err_sr_p |	\
						  L1P_ESR_err_sr_rd_valid_p | \
						  L1P_ESR_err_sw_p |	\
						  L1P_ESR_err_si_ecc |	\
						  L1P_ESR_err_si_ecc_ue | \
						  L1P_ESR_err_si_p |	\
						  L1P_ESR_err_sda_p |	\
						  L1P_ESR_err_sda_p_ue | \
						  L1P_ESR_err_rqra_p |	\
						  L1P_ESR_err_reload_ecc_x2 | \
						  L1P_ESR_err_reload_ecc_ue_x2 | \
						  L1P_ESR_err_rira_p |	\
						  L1P_ESR_err_gctr_p |	\
						  L1P_ESR_err_lu_state_p | \
						  L1P_ESR_err_lu_ttype | \
						  L1P_ESR_err_lu_dcr_abort | \
						  L1P_ESR_err_mmio_async | \
						  L1P_ESR_err_mmio_state_p | \
						  L1P_ESR_err_mmio_timeout | \
						  L1P_ESR_err_mmio_priv | \
						  L1P_ESR_err_mmio_rdata_p | \
						  L1P_ESR_err_mmio_wdata_p | \
						  L1P_ESR_err_mmio_dcrs_timeout | \
						  L1P_ESR_err_mmio_dcrs_priv | \
						  L1P_ESR_err_mmio_dcrs_par | \
						  L1P_ESR_err_dcrm_crit | \
						  L1P_ESR_err_dcrm_noncrit | \
						  L1P_ESR_err_dcrm_mc | \
						  L1P_ESR_err_tag_timeout | \
						  L1P_ESR_err_valid_timeout | \
						  L1P_ESR_err_hold_timeout | \
						  L1P_ESR_err_ditc_req_x2 | \
						  L1P_ESR_err_pfd_addr_p | \
						  L1P_ESR_err_pfd_avalid_p | \
						  L1P_ESR_err_pfd_fill_pnd_p | \
						  L1P_ESR_err_pfd_hit_pnd_p | \
						  L1P_ESR_err_pfd_stream_p | \
						  L1P_ESR_err_pfd_depth_p | \
						  L1P_ESR_err_pfd_clone_p | \
						  L1P_ESR_err_hitq_p |	\
						  L1P_ESR_err_sd_p |	\
						  L1P_ESR_err_pf2dfc_p | \
						  L1P_ESR_err_wccm_p_x2 | \
						  L1P_ESR_err_wccm_wcd_p_x2 | \
						  L1P_ESR_err_lu_wcd_p | \
						  L1P_ESR_err_lu_current_p | \
						  L1P_ESR_err_l2cmd |	\
						  L1P_ESR_err_lu_dcr_dbus_p | \
						  L1P_ESR_err_luq_p |	\
						  L1P_ESR_err_sda_phase_p | \
						  L1P_ESR_slice_sel_ctrl_perr | \
						  L1P_ESR_redun_ctrl_perr | \
						  0)




#define FW_RESERVED_PE_DCR__PCIE_INTERRUPT_STATE_BITS ( 0 |		\
							PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_set(1) | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_set(1) /* DD2 */ | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_set(1) /* DD2 */ | \
							PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_set(1) /* DD2 */ | \
							0)

#define FW_RESERVED_SERDES_LEFT_DCR_SERDES_INTERRUPT_STATE_BITS ( 0 |	\
								  SERDES_LEFT_DCR__SERDES_INTERRUPT_STATE__A_PLLA_LOCK_LOST_set(1) | \
								  SERDES_LEFT_DCR__SERDES_INTERRUPT_STATE__B_PLLA_LOCK_LOST_set(1) | \
								  SERDES_LEFT_DCR__SERDES_INTERRUPT_STATE__C_PLLA_LOCK_LOST_set(1) | \
								  0)


#define FW_RESERVED_SERDES_RIGHT_DCR_SERDES_INTERRUPT_STATE_BITS ( 0 |	\
								   SERDES_RIGHT_DCR__SERDES_INTERRUPT_STATE__D_PLLA_LOCK_LOST_set(1) | \
								   SERDES_RIGHT_DCR__SERDES_INTERRUPT_STATE__E_PLLB_LOCK_LOST_set(1) | \
								   SERDES_RIGHT_DCR__SERDES_INTERRUPT_STATE__IO_PLLA_LOCK_LOST_set(1) | \
								   0)



#define FW_RESERVED_TESTINT_DCR_TI_INTERRUPT_STATE_BITS ( 0 |		\
							  TESTINT_DCR__TI_INTERRUPT_STATE__DCR_TO_PCB_ERR_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__LOCAL_RING_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__INT_PARITY_ERROR_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_EQ_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_01_NE_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_EQ_set(1) | \
							  TESTINT_DCR__TI_INTERRUPT_STATE__INT_MBOX_23_NE_set(1) | \
							  0)



#define FW_RESERVED_UPC_C_DCR_UPC_C_INTERRUPT_STATE_BITS ( 0 |		\
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_set(1) | \
							   UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_set(1) | \
							   0)

#endif // Add nothing below this line.

