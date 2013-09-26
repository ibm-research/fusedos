/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/*                                                                  */
/* US Government Users Restricted Rights - Use, duplication or      */
/* disclosure restricted by GSA ADP Schedule Contract with IBM      */
/* Corporation.                                                     */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */

#ifndef   _DEVBUS_DCR__H_ // Prevent multiple inclusion
#define   _DEVBUS_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define DEVBUS_DCR_base    (0x0CC000)  // Base address for all instances
#define DEVBUS_DCR_size    (0x003C)    // Size of instance (largest offset + 1 )
  
#define DEVBUS_DCR(reg) _DCR_REG(DEVBUS_DCR, reg)
  
// Register:
//   Name:        devbus_control
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       1AE0000000000000
//   Description: General control bits for the devbus.
#define DEVBUS_DCR__DEVBUS_CONTROL_offset  (0x0000) // Offset of register in instance 
#define DEVBUS_DCR__DEVBUS_CONTROL_reset  UNSIGNED64(0x1AE0000000000000)

//   Field:       msi_mode
//   Description: 1 = enable MSI support, 0 = enable MSI-X support
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_MODE_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_MODE_position     0
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_MODE_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_MODE_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_MODE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       msi_spread
//   Description: When MSI enabled, spread bits out so that there is one per byte.
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_SPREAD_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_SPREAD_position     1
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_SPREAD_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_SPREAD_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__MSI_SPREAD_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       gen_hold
//   Description: Number of clk_x2 cycles to consider an inbound write still present following switch transfer for the generation counters.
#define DEVBUS_DCR__DEVBUS_CONTROL__GEN_HOLD_width        4
#define DEVBUS_DCR__DEVBUS_CONTROL__GEN_HOLD_position     5
#define DEVBUS_DCR__DEVBUS_CONTROL__GEN_HOLD_get(x)       _BGQ_GET(4,5,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__GEN_HOLD_set(v)       _BGQ_SET(4,5,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__GEN_HOLD_insert(x,v)  _BGQ_INSERT(4,5,x,v)

//   Field:       strong_msync
//   Description: When enabled, devbus msync will not complete until all inbound PCIe writes that have been accepted are completed.
#define DEVBUS_DCR__DEVBUS_CONTROL__STRONG_MSYNC_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__STRONG_MSYNC_position     6
#define DEVBUS_DCR__DEVBUS_CONTROL__STRONG_MSYNC_get(x)       _BGQ_GET(1,6,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__STRONG_MSYNC_set(v)       _BGQ_SET(1,6,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__STRONG_MSYNC_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       sr_limit
//   Description: Number of outstanding response switch requests allowed.
#define DEVBUS_DCR__DEVBUS_CONTROL__SR_LIMIT_width        4
#define DEVBUS_DCR__DEVBUS_CONTROL__SR_LIMIT_position     11
#define DEVBUS_DCR__DEVBUS_CONTROL__SR_LIMIT_get(x)       _BGQ_GET(4,11,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__SR_LIMIT_set(v)       _BGQ_SET(4,11,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__SR_LIMIT_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       dd1_revert_watchdog
//   Description: Chicken switch for watchdog counter extension.
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_WATCHDOG_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_WATCHDOG_position     12
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_WATCHDOG_get(x)       _BGQ_GET(1,12,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_WATCHDOG_set(v)       _BGQ_SET(1,12,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_WATCHDOG_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       dd1_revert_badreq
//   Description: Chicken switch for slave request bad request handling.
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_BADREQ_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_BADREQ_position     13
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_BADREQ_get(x)       _BGQ_GET(1,13,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_BADREQ_set(v)       _BGQ_SET(1,13,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_BADREQ_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       dd1_revert_cfgio
//   Description: Chicken switch for PCIe config or IO unprivileged decode.
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_CFGIO_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_CFGIO_position     14
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_CFGIO_get(x)       _BGQ_GET(1,14,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_CFGIO_set(v)       _BGQ_SET(1,14,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_CFGIO_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       dd1_revert_decode
//   Description: Chicken switch for decode miss handling.
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_DECODE_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_DECODE_position     15
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_DECODE_get(x)       _BGQ_GET(1,15,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_DECODE_set(v)       _BGQ_SET(1,15,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_DECODE_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       dd1_revert_pcie
//   Description: Chicken switch for PCIe alignment and unsupported size handling.
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_PCIE_width        1
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_PCIE_position     16
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_PCIE_get(x)       _BGQ_GET(1,16,x)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_PCIE_set(v)       _BGQ_SET(1,16,v)
#define DEVBUS_DCR__DEVBUS_CONTROL__DD1_REVERT_PCIE_insert(x,v)  _BGQ_INSERT(1,16,x,v)

  
// Register:
//   Name:        devbus_debug
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Debugging features.
#define DEVBUS_DCR__DEVBUS_DEBUG_offset  (0x0001) // Offset of register in instance 
#define DEVBUS_DCR__DEVBUS_DEBUG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       outbound_ctrl_ecc_disable
//   Description: Enables pass-through on the ECC checker for outbound address and control bits.
#define DEVBUS_DCR__DEVBUS_DEBUG__OUTBOUND_CTRL_ECC_DISABLE_width        1
#define DEVBUS_DCR__DEVBUS_DEBUG__OUTBOUND_CTRL_ECC_DISABLE_position     0
#define DEVBUS_DCR__DEVBUS_DEBUG__OUTBOUND_CTRL_ECC_DISABLE_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__DEVBUS_DEBUG__OUTBOUND_CTRL_ECC_DISABLE_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__DEVBUS_DEBUG__OUTBOUND_CTRL_ECC_DISABLE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       rdbuf_a_disable
//   Description: Disable slave requests to the response switch from port A (BeDRAM).
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_A_DISABLE_width        1
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_A_DISABLE_position     1
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_A_DISABLE_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_A_DISABLE_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_A_DISABLE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       rdbuf_b_disable
//   Description: Disable slave requests to the response switch from port B (all slaves except BeDRAM).
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_B_DISABLE_width        1
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_B_DISABLE_position     2
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_B_DISABLE_get(x)       _BGQ_GET(1,2,x)
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_B_DISABLE_set(v)       _BGQ_SET(1,2,v)
#define DEVBUS_DCR__DEVBUS_DEBUG__RDBUF_B_DISABLE_insert(x,v)  _BGQ_INSERT(1,2,x,v)

  
// Register:
//   Name:        devbus_peek
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: See the state of some devbus signals.
#define DEVBUS_DCR__DEVBUS_PEEK_offset  (0x0002) // Offset of register in instance 
#define DEVBUS_DCR__DEVBUS_PEEK_reset  UNSIGNED64(0x0000000000000000)

//   Field:       slave_request_pending
//   Description: Device request is pending (0=L2, 1=MSI, 2=UPC, 3=GEA).
#define DEVBUS_DCR__DEVBUS_PEEK__SLAVE_REQUEST_PENDING_width        4
#define DEVBUS_DCR__DEVBUS_PEEK__SLAVE_REQUEST_PENDING_position     3
#define DEVBUS_DCR__DEVBUS_PEEK__SLAVE_REQUEST_PENDING_get(x)       _BGQ_GET(4,3,x)
#define DEVBUS_DCR__DEVBUS_PEEK__SLAVE_REQUEST_PENDING_set(v)       _BGQ_SET(4,3,v)
#define DEVBUS_DCR__DEVBUS_PEEK__SLAVE_REQUEST_PENDING_insert(x,v)  _BGQ_INSERT(4,3,x,v)

//   Field:       MSI_int_pending
//   Description: MSI interrupt request to GEA is pending.
#define DEVBUS_DCR__DEVBUS_PEEK__MSI_INT_PENDING_width        4
#define DEVBUS_DCR__DEVBUS_PEEK__MSI_INT_PENDING_position     7
#define DEVBUS_DCR__DEVBUS_PEEK__MSI_INT_PENDING_get(x)       _BGQ_GET(4,7,x)
#define DEVBUS_DCR__DEVBUS_PEEK__MSI_INT_PENDING_set(v)       _BGQ_SET(4,7,v)
#define DEVBUS_DCR__DEVBUS_PEEK__MSI_INT_PENDING_insert(x,v)  _BGQ_INSERT(4,7,x,v)

//   Field:       cfgio_wr_busy
//   Description: Outbound PCIe configuration or IO write is outstanding.
#define DEVBUS_DCR__DEVBUS_PEEK__CFGIO_WR_BUSY_width        1
#define DEVBUS_DCR__DEVBUS_PEEK__CFGIO_WR_BUSY_position     8
#define DEVBUS_DCR__DEVBUS_PEEK__CFGIO_WR_BUSY_get(x)       _BGQ_GET(1,8,x)
#define DEVBUS_DCR__DEVBUS_PEEK__CFGIO_WR_BUSY_set(v)       _BGQ_SET(1,8,v)
#define DEVBUS_DCR__DEVBUS_PEEK__CFGIO_WR_BUSY_insert(x,v)  _BGQ_INSERT(1,8,x,v)

  
// Register:
//   Name:        pcie_cfg
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000FF0000000000
//   Description: PCIe configuration
#define DEVBUS_DCR__PCIE_CFG_offset  (0x0003) // Offset of register in instance 
#define DEVBUS_DCR__PCIE_CFG_reset  UNSIGNED64(0x0000FF0000000000)

//   Field:       out_rd_data_ecc_disable
//   Description: Enable to pass outbound read data through the ECC checker without correcting it.
#define DEVBUS_DCR__PCIE_CFG__OUT_RD_DATA_ECC_DISABLE_width        1
#define DEVBUS_DCR__PCIE_CFG__OUT_RD_DATA_ECC_DISABLE_position     0
#define DEVBUS_DCR__PCIE_CFG__OUT_RD_DATA_ECC_DISABLE_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__PCIE_CFG__OUT_RD_DATA_ECC_DISABLE_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__PCIE_CFG__OUT_RD_DATA_ECC_DISABLE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       in_wr_data_ecc_disable
//   Description: Enable to pass inbound write data through the ECC checker without correcting it.
#define DEVBUS_DCR__PCIE_CFG__IN_WR_DATA_ECC_DISABLE_width        1
#define DEVBUS_DCR__PCIE_CFG__IN_WR_DATA_ECC_DISABLE_position     1
#define DEVBUS_DCR__PCIE_CFG__IN_WR_DATA_ECC_DISABLE_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__PCIE_CFG__IN_WR_DATA_ECC_DISABLE_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__PCIE_CFG__IN_WR_DATA_ECC_DISABLE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       in_rd_page_offset
//   Description: Number of 1/4 gigabyte pages to shift the inbound read address up by.
#define DEVBUS_DCR__PCIE_CFG__IN_RD_PAGE_OFFSET_width        8
#define DEVBUS_DCR__PCIE_CFG__IN_RD_PAGE_OFFSET_position     15
#define DEVBUS_DCR__PCIE_CFG__IN_RD_PAGE_OFFSET_get(x)       _BGQ_GET(8,15,x)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_PAGE_OFFSET_set(v)       _BGQ_SET(8,15,v)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_PAGE_OFFSET_insert(x,v)  _BGQ_INSERT(8,15,x,v)

//   Field:       in_rd_window_top
//   Description: Top of allowed page range (after offset is applied), specified as a  1/4 gigabyte page number.
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_TOP_width        8
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_TOP_position     23
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_TOP_get(x)       _BGQ_GET(8,23,x)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_TOP_set(v)       _BGQ_SET(8,23,v)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_TOP_insert(x,v)  _BGQ_INSERT(8,23,x,v)

//   Field:       in_rd_window_bottom
//   Description: Bottom of allowed page range (after offset is applied), specified as a  1/4 gigabyte page number.
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_BOTTOM_width        8
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_BOTTOM_position     31
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_BOTTOM_get(x)       _BGQ_GET(8,31,x)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_BOTTOM_set(v)       _BGQ_SET(8,31,v)
#define DEVBUS_DCR__PCIE_CFG__IN_RD_WINDOW_BOTTOM_insert(x,v)  _BGQ_INSERT(8,31,x,v)

  
// Register:
//   Name:        switch_cfg
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Crossbar switch configuration
#define DEVBUS_DCR__SWITCH_CFG_offset  (0x0004) // Offset of register in instance 
#define DEVBUS_DCR__SWITCH_CFG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sw_cfg
//   Description: Request switch configuration
#define DEVBUS_DCR__SWITCH_CFG__SW_CFG_width        5
#define DEVBUS_DCR__SWITCH_CFG__SW_CFG_position     4
#define DEVBUS_DCR__SWITCH_CFG__SW_CFG_get(x)       _BGQ_GET(5,4,x)
#define DEVBUS_DCR__SWITCH_CFG__SW_CFG_set(v)       _BGQ_SET(5,4,v)
#define DEVBUS_DCR__SWITCH_CFG__SW_CFG_insert(x,v)  _BGQ_INSERT(5,4,x,v)

//   Field:       sr_cfg
//   Description: Response switch configuration
#define DEVBUS_DCR__SWITCH_CFG__SR_CFG_width        4
#define DEVBUS_DCR__SWITCH_CFG__SR_CFG_position     8
#define DEVBUS_DCR__SWITCH_CFG__SR_CFG_get(x)       _BGQ_GET(4,8,x)
#define DEVBUS_DCR__SWITCH_CFG__SR_CFG_set(v)       _BGQ_SET(4,8,v)
#define DEVBUS_DCR__SWITCH_CFG__SR_CFG_insert(x,v)  _BGQ_INSERT(4,8,x,v)

  
// Register:
//   Name:        spare
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Spare register with 8 bits
#define DEVBUS_DCR__SPARE_offset  (0x0005) // Offset of register in instance 
#define DEVBUS_DCR__SPARE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       dd1_revert_be_cntr
//   Description: Chicken switch for outstanding bedram read counter.
#define DEVBUS_DCR__SPARE__DD1_REVERT_BE_CNTR_width        1
#define DEVBUS_DCR__SPARE__DD1_REVERT_BE_CNTR_position     0
#define DEVBUS_DCR__SPARE__DD1_REVERT_BE_CNTR_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__SPARE__DD1_REVERT_BE_CNTR_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__SPARE__DD1_REVERT_BE_CNTR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       dd1_revert_rdbuf_arb
//   Description: Chicken switch for slave return data arbiter.
#define DEVBUS_DCR__SPARE__DD1_REVERT_RDBUF_ARB_width        1
#define DEVBUS_DCR__SPARE__DD1_REVERT_RDBUF_ARB_position     1
#define DEVBUS_DCR__SPARE__DD1_REVERT_RDBUF_ARB_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__SPARE__DD1_REVERT_RDBUF_ARB_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__SPARE__DD1_REVERT_RDBUF_ARB_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       spare
//   Description: Spare latches
#define DEVBUS_DCR__SPARE__SPARE_width        6
#define DEVBUS_DCR__SPARE__SPARE_position     7
#define DEVBUS_DCR__SPARE__SPARE_get(x)       _BGQ_GET(6,7,x)
#define DEVBUS_DCR__SPARE__SPARE_set(v)       _BGQ_SET(6,7,v)
#define DEVBUS_DCR__SPARE__SPARE_insert(x,v)  _BGQ_INSERT(6,7,x,v)

  
// Register:
//   Name:        upc_db_counter
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Devbus UPC Counters Data
#define DEVBUS_DCR__UPC_DB_COUNTER_offset  (0x0030) // Offset of register in instance 
#define DEVBUS_DCR__UPC_DB_COUNTER_range  (0x7) // Range of external address space
#define DEVBUS_DCR__UPC_DB_COUNTER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ovf
//   Description: Counter Overflow bit
#define DEVBUS_DCR__UPC_DB_COUNTER__OVF_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER__OVF_position     47
#define DEVBUS_DCR__UPC_DB_COUNTER__OVF_get(x)       _BGQ_GET(1,47,x)
#define DEVBUS_DCR__UPC_DB_COUNTER__OVF_set(v)       _BGQ_SET(1,47,v)
#define DEVBUS_DCR__UPC_DB_COUNTER__OVF_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       data
//   Description: Devbus Counter Data
#define DEVBUS_DCR__UPC_DB_COUNTER__DATA_width        16
#define DEVBUS_DCR__UPC_DB_COUNTER__DATA_position     63
#define DEVBUS_DCR__UPC_DB_COUNTER__DATA_get(x)       _BGQ_GET(16,63,x)
#define DEVBUS_DCR__UPC_DB_COUNTER__DATA_set(v)       _BGQ_SET(16,63,v)
#define DEVBUS_DCR__UPC_DB_COUNTER__DATA_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        upc_db_counter_control_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Devbus UPC Counters RW Control Register
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW_offset  (0x0038) // Offset of register in instance 
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW_range  (0x1) // Range of external address space
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Devbus Counter Reset(RW)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__RESET_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__RESET_position     0
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__RESET_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__RESET_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: Devbus Counter Enable(RW)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__ENABLE_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__ENABLE_position     32
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_RW__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_db_counter_control_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Devbus UPC Counters W1S Control Register
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S_offset  (0x0039) // Offset of register in instance 
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S_range  (0x1) // Range of external address space
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Devbus Counter Reset(W1S)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_position     0
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: Devbus Counter Enable(W1S)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_position     32
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_db_counter_control_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Devbus UPC Counters W1C Control Register
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C_offset  (0x003A) // Offset of register in instance 
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C_range  (0x1) // Range of external address space
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Devbus Counter Enable(W1C)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__RESET_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__RESET_position     0
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__RESET_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__RESET_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: Devbus Counter Reset(W1C)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_width        1
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_position     32
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define DEVBUS_DCR__UPC_DB_COUNTER_CONTROL_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_db_config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Devbus UPC Configuration Register
#define DEVBUS_DCR__UPC_DB_CONFIG_offset  (0x003B) // Offset of register in instance 
#define DEVBUS_DCR__UPC_DB_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_db_ring_enable
//   Description: Enables UPC devbus Ring logic. Must be set to one to pass daisy chain data.
#define DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_width        1
#define DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_position     3
#define DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_get(x)       _BGQ_GET(1,3,x)
#define DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_set(v)       _BGQ_SET(1,3,v)
#define DEVBUS_DCR__UPC_DB_CONFIG__UPC_DB_RING_ENABLE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       sync_override
//   Description: Sync Override bits for all counters
#define DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_width        1
#define DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_position     48
#define DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_get(x)       _BGQ_GET(1,48,x)
#define DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_set(v)       _BGQ_SET(1,48,v)
#define DEVBUS_DCR__UPC_DB_CONFIG__SYNC_OVERRIDE_insert(x,v)  _BGQ_INSERT(1,48,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0027) // Offset of register in instance 
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0028) // Offset of register in instance 
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0029) // Offset of register in instance 
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: db_interrupt_state
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define DEVBUS_DCR__DB_INTERRUPT_STATE__STATE_offset         (0x0010) // Offset of State register in instance
#define DEVBUS_DCR__DB_INTERRUPT_STATE__FIRST_offset         (0x0014) // Offset of First register in instance
#define DEVBUS_DCR__DB_INTERRUPT_STATE__FORCE_offset         (0x0013) // Offset of Force register in instance
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MACHINE_CHECK_offset (0x0015) // Offset of Machine Check register in instance
#define DEVBUS_DCR__DB_INTERRUPT_STATE__CRITICAL_offset      (0x0016) // Offset of Critical register in instance
#define DEVBUS_DCR__DB_INTERRUPT_STATE__NONCRITICAL_offset   (0x0017) // Offset of Noncritical register in instance

#define DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_position     0
#define DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_position     1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_STORE_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_position     2
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_get(x)       _BGQ_GET(1,2,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_set(v)       _BGQ_SET(1,2,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_BAD_LOAD_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_position     3
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_get(x)       _BGQ_GET(1,3,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_set(v)       _BGQ_SET(1,3,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_VALIDSRC_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_position     4
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_get(x)       _BGQ_GET(1,4,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_set(v)       _BGQ_SET(1,4,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_CTRL_ECC_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_position     5
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_get(x)       _BGQ_GET(1,5,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_set(v)       _BGQ_SET(1,5,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_PARITY_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_position     6
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_get(x)       _BGQ_GET(1,6,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_set(v)       _BGQ_SET(1,6,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_SM_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_position     7
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_get(x)       _BGQ_GET(1,7,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_set(v)       _BGQ_SET(1,7,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_FIFO_DATA_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_position     8
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_get(x)       _BGQ_GET(1,8,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_set(v)       _BGQ_SET(1,8,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLVOUT_WDATA_ECC_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_position     9
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_get(x)       _BGQ_GET(1,9,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_set(v)       _BGQ_SET(1,9,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_ADDR_PARITY_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_position     10
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_get(x)       _BGQ_GET(1,10,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_set(v)       _BGQ_SET(1,10,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_BEN_PARITY_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_position     11
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_get(x)       _BGQ_GET(1,11,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_set(v)       _BGQ_SET(1,11,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IW_WDATA_UE_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_position     12
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_get(x)       _BGQ_GET(1,12,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_set(v)       _BGQ_SET(1,12,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_IO_RCVD_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_position     13
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_get(x)       _BGQ_GET(1,13,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_set(v)       _BGQ_SET(1,13,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_ATTR_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_position     14
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_get(x)       _BGQ_GET(1,14,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_set(v)       _BGQ_SET(1,14,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_START_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_position     15
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_get(x)       _BGQ_GET(1,15,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_set(v)       _BGQ_SET(1,15,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_RANGE_END_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_position     16
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_get(x)       _BGQ_GET(1,16,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_set(v)       _BGQ_SET(1,16,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_MAX_TOP_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_position     17
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_get(x)       _BGQ_GET(1,17,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_set(v)       _BGQ_SET(1,17,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_BOUNDS_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_position     18
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_get(x)       _BGQ_GET(1,18,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_set(v)       _BGQ_SET(1,18,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_IR_SM_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_position     19
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_get(x)       _BGQ_GET(1,19,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_set(v)       _BGQ_SET(1,19,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_RD_ATTR_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_position     20
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_get(x)       _BGQ_GET(1,20,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_set(v)       _BGQ_SET(1,20,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_TAG_FIFO_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_position     21
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_get(x)       _BGQ_GET(1,21,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_set(v)       _BGQ_SET(1,21,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_STACK_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_position     22
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_get(x)       _BGQ_GET(1,22,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_set(v)       _BGQ_SET(1,22,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_RA_CTRL_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_position     23
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_get(x)       _BGQ_GET(1,23,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_set(v)       _BGQ_SET(1,23,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_TGT_FIFO_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_position     24
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_get(x)       _BGQ_GET(1,24,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_set(v)       _BGQ_SET(1,24,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_GRANT_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_position     25
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_get(x)       _BGQ_GET(1,25,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_set(v)       _BGQ_SET(1,25,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSYNC_GEN_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_position     26
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_get(x)       _BGQ_GET(1,26,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_set(v)       _BGQ_SET(1,26,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_SIZE_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_position     27
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_get(x)       _BGQ_GET(1,27,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_set(v)       _BGQ_SET(1,27,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_FIFO_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_position     28
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_get(x)       _BGQ_GET(1,28,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_set(v)       _BGQ_SET(1,28,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__BEDRAM_COUNTER_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_position     29
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_get(x)       _BGQ_GET(1,29,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_set(v)       _BGQ_SET(1,29,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_MEM_SIZE_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_position     30
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_get(x)       _BGQ_GET(1,30,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_set(v)       _BGQ_SET(1,30,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_WR_SIZE_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_position     31
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_get(x)       _BGQ_GET(1,31,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_set(v)       _BGQ_SET(1,31,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_CFGIO_RD_SIZE_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_position     32
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_get(x)       _BGQ_GET(1,32,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_set(v)       _BGQ_SET(1,32,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ALIGN_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_position     33
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_get(x)       _BGQ_GET(1,33,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_set(v)       _BGQ_SET(1,33,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ATTR_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_position     34
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_get(x)       _BGQ_GET(1,34,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_set(v)       _BGQ_SET(1,34,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_OR_ECC_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_position     35
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_get(x)       _BGQ_GET(1,35,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_set(v)       _BGQ_SET(1,35,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__MSI_PARITY_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_position     36
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_get(x)       _BGQ_GET(1,36,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_set(v)       _BGQ_SET(1,36,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_PARITY_insert(x,v)  _BGQ_INSERT(1,36,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_position     37
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_get(x)       _BGQ_GET(1,37,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_set(v)       _BGQ_SET(1,37,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_WDATA_PARITY_insert(x,v)  _BGQ_INSERT(1,37,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_position     38
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_get(x)       _BGQ_GET(1,38,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_set(v)       _BGQ_SET(1,38,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_RDATA_PARITY_insert(x,v)  _BGQ_INSERT(1,38,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_position     39
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_get(x)       _BGQ_GET(1,39,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_set(v)       _BGQ_SET(1,39,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_REQ_TIMEOUT_insert(x,v)  _BGQ_INSERT(1,39,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_position     40
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_get(x)       _BGQ_GET(1,40,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_set(v)       _BGQ_SET(1,40,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_PARITY_insert(x,v)  _BGQ_INSERT(1,40,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_position     41
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_get(x)       _BGQ_GET(1,41,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_set(v)       _BGQ_SET(1,41,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_WDATA_PARITY_insert(x,v)  _BGQ_INSERT(1,41,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_position     42
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_get(x)       _BGQ_GET(1,42,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_set(v)       _BGQ_SET(1,42,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_RDATA_PARITY_insert(x,v)  _BGQ_INSERT(1,42,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_position     43
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_get(x)       _BGQ_GET(1,43,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_set(v)       _BGQ_SET(1,43,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__GEA_REQ_TIMEOUT_insert(x,v)  _BGQ_INSERT(1,43,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_position     44
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_get(x)       _BGQ_GET(1,44,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_set(v)       _BGQ_SET(1,44,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_IFC_FIFO_PARITY_insert(x,v)  _BGQ_INSERT(1,44,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_position     45
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_get(x)       _BGQ_GET(1,45,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_set(v)       _BGQ_SET(1,45,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__L2_SLICE_SELECT_insert(x,v)  _BGQ_INSERT(1,45,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_position     46
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_get(x)       _BGQ_GET(1,46,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_set(v)       _BGQ_SET(1,46,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_A_STACK_PTY_insert(x,v)  _BGQ_INSERT(1,46,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_position     47
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_get(x)       _BGQ_GET(1,47,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_set(v)       _BGQ_SET(1,47,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_B_STACK_PTY_insert(x,v)  _BGQ_INSERT(1,47,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_position     48
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_get(x)       _BGQ_GET(1,48,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_set(v)       _BGQ_SET(1,48,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_FIFO_PTY_insert(x,v)  _BGQ_INSERT(1,48,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_position     49
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_get(x)       _BGQ_GET(1,49,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_set(v)       _BGQ_SET(1,49,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_READY_REG_insert(x,v)  _BGQ_INSERT(1,49,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_position     50
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_get(x)       _BGQ_GET(1,50,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_set(v)       _BGQ_SET(1,50,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__RDBUF_SR_GNT_insert(x,v)  _BGQ_INSERT(1,50,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_position     51
#define DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_get(x)       _BGQ_GET(1,51,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_set(v)       _BGQ_SET(1,51,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__TI_ADDR_PARITY_insert(x,v)  _BGQ_INSERT(1,51,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_position     52
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_get(x)       _BGQ_GET(1,52,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_set(v)       _BGQ_SET(1,52,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_A_insert(x,v)  _BGQ_INSERT(1,52,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_position     53
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_get(x)       _BGQ_GET(1,53,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_set(v)       _BGQ_SET(1,53,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_B_insert(x,v)  _BGQ_INSERT(1,53,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_position     54
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_get(x)       _BGQ_GET(1,54,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_set(v)       _BGQ_SET(1,54,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_C_insert(x,v)  _BGQ_INSERT(1,54,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_position     55
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_get(x)       _BGQ_GET(1,55,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_set(v)       _BGQ_SET(1,55,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__PCIE_INT_D_insert(x,v)  _BGQ_INSERT(1,55,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_position     56
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_get(x)       _BGQ_GET(1,56,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_set(v)       _BGQ_SET(1,56,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,56,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_position     57
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_get(x)       _BGQ_GET(1,57,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_set(v)       _BGQ_SET(1,57,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SW_SWITCH_insert(x,v)  _BGQ_INSERT(1,57,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_position     58
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_get(x)       _BGQ_GET(1,58,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_set(v)       _BGQ_SET(1,58,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SR_SWITCH_insert(x,v)  _BGQ_INSERT(1,58,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_position     59
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_get(x)       _BGQ_GET(1,59,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_set(v)       _BGQ_SET(1,59,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SI_SWITCH_insert(x,v)  _BGQ_INSERT(1,59,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_width        1
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_position     60
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_get(x)       _BGQ_GET(1,60,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_set(v)       _BGQ_SET(1,60,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE__SLAVE_DECODE_insert(x,v)  _BGQ_INSERT(1,60,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0020) // Offset of State register in instance
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0023) // Offset of First register in instance
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0022) // Offset of Force register in instance
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0024) // Offset of Machine Check register in instance
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0025) // Offset of Critical register in instance
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0026) // Offset of Noncritical register in instance

#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: db_interrupt_state_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW_offset         (0x0011) // Offset of Control register in instance

#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ALIGN_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ALIGN_position     1
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ALIGN_get(x)       _BGQ_GET(2,1,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ALIGN_set(v)       _BGQ_SET(2,1,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ALIGN_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ATTR_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ATTR_position     3
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ATTR_get(x)       _BGQ_GET(2,3,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ATTR_set(v)       _BGQ_SET(2,3,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ATTR_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ECC_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ECC_position     5
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ECC_get(x)       _BGQ_GET(2,5,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ECC_set(v)       _BGQ_SET(2,5,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_OR_ECC_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__MSI_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__MSI_PARITY_position     7
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__MSI_PARITY_get(x)       _BGQ_GET(2,7,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__MSI_PARITY_set(v)       _BGQ_SET(2,7,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__MSI_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_PARITY_position     9
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_PARITY_get(x)       _BGQ_GET(2,9,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_PARITY_set(v)       _BGQ_SET(2,9,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_PARITY_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_WDATA_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_WDATA_PARITY_position     11
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_WDATA_PARITY_get(x)       _BGQ_GET(2,11,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_WDATA_PARITY_set(v)       _BGQ_SET(2,11,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_WDATA_PARITY_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_RDATA_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_RDATA_PARITY_position     13
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_RDATA_PARITY_get(x)       _BGQ_GET(2,13,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_RDATA_PARITY_set(v)       _BGQ_SET(2,13,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_RDATA_PARITY_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_TIMEOUT_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_TIMEOUT_position     15
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_TIMEOUT_get(x)       _BGQ_GET(2,15,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_TIMEOUT_set(v)       _BGQ_SET(2,15,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_REQ_TIMEOUT_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_PARITY_position     17
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_PARITY_get(x)       _BGQ_GET(2,17,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_PARITY_set(v)       _BGQ_SET(2,17,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_PARITY_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_WDATA_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_WDATA_PARITY_position     19
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_WDATA_PARITY_get(x)       _BGQ_GET(2,19,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_WDATA_PARITY_set(v)       _BGQ_SET(2,19,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_WDATA_PARITY_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_RDATA_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_RDATA_PARITY_position     21
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_RDATA_PARITY_get(x)       _BGQ_GET(2,21,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_RDATA_PARITY_set(v)       _BGQ_SET(2,21,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_RDATA_PARITY_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_TIMEOUT_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_TIMEOUT_position     23
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_TIMEOUT_get(x)       _BGQ_GET(2,23,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_TIMEOUT_set(v)       _BGQ_SET(2,23,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__GEA_REQ_TIMEOUT_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_IFC_FIFO_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_IFC_FIFO_PARITY_position     25
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_IFC_FIFO_PARITY_get(x)       _BGQ_GET(2,25,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_IFC_FIFO_PARITY_set(v)       _BGQ_SET(2,25,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_IFC_FIFO_PARITY_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_SLICE_SELECT_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_SLICE_SELECT_position     27
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_SLICE_SELECT_get(x)       _BGQ_GET(2,27,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_SLICE_SELECT_set(v)       _BGQ_SET(2,27,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__L2_SLICE_SELECT_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_A_STACK_PTY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_A_STACK_PTY_position     29
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_A_STACK_PTY_get(x)       _BGQ_GET(2,29,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_A_STACK_PTY_set(v)       _BGQ_SET(2,29,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_A_STACK_PTY_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_B_STACK_PTY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_B_STACK_PTY_position     31
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_B_STACK_PTY_get(x)       _BGQ_GET(2,31,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_B_STACK_PTY_set(v)       _BGQ_SET(2,31,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_B_STACK_PTY_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_FIFO_PTY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_FIFO_PTY_position     33
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_FIFO_PTY_get(x)       _BGQ_GET(2,33,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_FIFO_PTY_set(v)       _BGQ_SET(2,33,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_FIFO_PTY_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_READY_REG_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_READY_REG_position     35
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_READY_REG_get(x)       _BGQ_GET(2,35,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_READY_REG_set(v)       _BGQ_SET(2,35,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_READY_REG_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_GNT_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_GNT_position     37
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_GNT_get(x)       _BGQ_GET(2,37,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_GNT_set(v)       _BGQ_SET(2,37,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__RDBUF_SR_GNT_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__TI_ADDR_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__TI_ADDR_PARITY_position     39
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__TI_ADDR_PARITY_get(x)       _BGQ_GET(2,39,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__TI_ADDR_PARITY_set(v)       _BGQ_SET(2,39,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__TI_ADDR_PARITY_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_A_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_A_position     41
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_A_get(x)       _BGQ_GET(2,41,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_A_set(v)       _BGQ_SET(2,41,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_A_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_B_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_B_position     43
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_B_get(x)       _BGQ_GET(2,43,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_B_set(v)       _BGQ_SET(2,43,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_B_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_C_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_C_position     45
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_C_get(x)       _BGQ_GET(2,45,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_C_set(v)       _BGQ_SET(2,45,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_C_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_D_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_D_position     47
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_D_get(x)       _BGQ_GET(2,47,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_D_set(v)       _BGQ_SET(2,47,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__PCIE_INT_D_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_position     49
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_get(x)       _BGQ_GET(2,49,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_set(v)       _BGQ_SET(2,49,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SW_SWITCH_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SW_SWITCH_position     51
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SW_SWITCH_get(x)       _BGQ_GET(2,51,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SW_SWITCH_set(v)       _BGQ_SET(2,51,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SW_SWITCH_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SR_SWITCH_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SR_SWITCH_position     53
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SR_SWITCH_get(x)       _BGQ_GET(2,53,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SR_SWITCH_set(v)       _BGQ_SET(2,53,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SR_SWITCH_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SI_SWITCH_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SI_SWITCH_position     55
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SI_SWITCH_get(x)       _BGQ_GET(2,55,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SI_SWITCH_set(v)       _BGQ_SET(2,55,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SI_SWITCH_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SLAVE_DECODE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SLAVE_DECODE_position     57
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SLAVE_DECODE_get(x)       _BGQ_GET(2,57,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SLAVE_DECODE_set(v)       _BGQ_SET(2,57,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_LOW__SLAVE_DECODE_insert(x,v)  _BGQ_INSERT(2,57,x,v)

  
// Interrupt Control Register: db_interrupt_state_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH_offset         (0x0012) // Offset of Control register in instance

#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_position     1
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_get(x)       _BGQ_GET(2,1,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_set(v)       _BGQ_SET(2,1,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_STORE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_STORE_position     3
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_STORE_get(x)       _BGQ_GET(2,3,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_STORE_set(v)       _BGQ_SET(2,3,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_STORE_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_LOAD_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_LOAD_position     5
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_LOAD_get(x)       _BGQ_GET(2,5,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_LOAD_set(v)       _BGQ_SET(2,5,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_BAD_LOAD_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_VALIDSRC_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_VALIDSRC_PARITY_position     7
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_VALIDSRC_PARITY_get(x)       _BGQ_GET(2,7,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_VALIDSRC_PARITY_set(v)       _BGQ_SET(2,7,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_VALIDSRC_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_CTRL_ECC_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_CTRL_ECC_position     9
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_CTRL_ECC_get(x)       _BGQ_GET(2,9,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_CTRL_ECC_set(v)       _BGQ_SET(2,9,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_CTRL_ECC_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_PARITY_position     11
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_PARITY_get(x)       _BGQ_GET(2,11,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_PARITY_set(v)       _BGQ_SET(2,11,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_PARITY_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_SM_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_SM_position     13
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_SM_get(x)       _BGQ_GET(2,13,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_SM_set(v)       _BGQ_SET(2,13,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_SM_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_DATA_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_DATA_position     15
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_DATA_get(x)       _BGQ_GET(2,15,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_DATA_set(v)       _BGQ_SET(2,15,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLAVE_FIFO_DATA_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_WDATA_ECC_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_WDATA_ECC_position     17
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_WDATA_ECC_get(x)       _BGQ_GET(2,17,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_WDATA_ECC_set(v)       _BGQ_SET(2,17,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SLVOUT_WDATA_ECC_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_ADDR_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_ADDR_PARITY_position     19
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_ADDR_PARITY_get(x)       _BGQ_GET(2,19,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_ADDR_PARITY_set(v)       _BGQ_SET(2,19,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_ADDR_PARITY_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_BEN_PARITY_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_BEN_PARITY_position     21
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_BEN_PARITY_get(x)       _BGQ_GET(2,21,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_BEN_PARITY_set(v)       _BGQ_SET(2,21,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_BEN_PARITY_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_WDATA_UE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_WDATA_UE_position     23
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_WDATA_UE_get(x)       _BGQ_GET(2,23,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_WDATA_UE_set(v)       _BGQ_SET(2,23,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IW_WDATA_UE_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_IO_RCVD_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_IO_RCVD_position     25
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_IO_RCVD_get(x)       _BGQ_GET(2,25,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_IO_RCVD_set(v)       _BGQ_SET(2,25,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_IO_RCVD_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_ATTR_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_ATTR_position     27
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_ATTR_get(x)       _BGQ_GET(2,27,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_ATTR_set(v)       _BGQ_SET(2,27,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_ATTR_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_START_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_START_position     29
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_START_get(x)       _BGQ_GET(2,29,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_START_set(v)       _BGQ_SET(2,29,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_START_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_END_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_END_position     31
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_END_get(x)       _BGQ_GET(2,31,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_END_set(v)       _BGQ_SET(2,31,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_RANGE_END_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_MAX_TOP_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_MAX_TOP_position     33
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_MAX_TOP_get(x)       _BGQ_GET(2,33,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_MAX_TOP_set(v)       _BGQ_SET(2,33,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_MAX_TOP_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_BOUNDS_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_BOUNDS_position     35
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_BOUNDS_get(x)       _BGQ_GET(2,35,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_BOUNDS_set(v)       _BGQ_SET(2,35,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_BOUNDS_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_SM_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_SM_position     37
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_SM_get(x)       _BGQ_GET(2,37,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_SM_set(v)       _BGQ_SET(2,37,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_IR_SM_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_RD_ATTR_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_RD_ATTR_position     39
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_RD_ATTR_get(x)       _BGQ_GET(2,39,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_RD_ATTR_set(v)       _BGQ_SET(2,39,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_RD_ATTR_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_TAG_FIFO_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_TAG_FIFO_position     41
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_TAG_FIFO_get(x)       _BGQ_GET(2,41,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_TAG_FIFO_set(v)       _BGQ_SET(2,41,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SR_TAG_FIFO_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_STACK_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_STACK_position     43
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_STACK_get(x)       _BGQ_GET(2,43,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_STACK_set(v)       _BGQ_SET(2,43,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_STACK_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_RA_CTRL_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_RA_CTRL_position     45
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_RA_CTRL_get(x)       _BGQ_GET(2,45,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_RA_CTRL_set(v)       _BGQ_SET(2,45,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_RA_CTRL_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_TGT_FIFO_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_TGT_FIFO_position     47
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_TGT_FIFO_get(x)       _BGQ_GET(2,47,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_TGT_FIFO_set(v)       _BGQ_SET(2,47,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_TGT_FIFO_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_GRANT_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_GRANT_position     49
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_GRANT_get(x)       _BGQ_GET(2,49,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_GRANT_set(v)       _BGQ_SET(2,49,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__SW_GRANT_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__MSYNC_GEN_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__MSYNC_GEN_position     51
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__MSYNC_GEN_get(x)       _BGQ_GET(2,51,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__MSYNC_GEN_set(v)       _BGQ_SET(2,51,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__MSYNC_GEN_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_SIZE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_SIZE_position     53
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_SIZE_get(x)       _BGQ_GET(2,53,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_SIZE_set(v)       _BGQ_SET(2,53,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_SIZE_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_FIFO_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_FIFO_position     55
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_FIFO_get(x)       _BGQ_GET(2,55,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_FIFO_set(v)       _BGQ_SET(2,55,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_FIFO_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_COUNTER_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_COUNTER_position     57
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_COUNTER_get(x)       _BGQ_GET(2,57,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_COUNTER_set(v)       _BGQ_SET(2,57,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__BEDRAM_COUNTER_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_MEM_SIZE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_MEM_SIZE_position     59
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_MEM_SIZE_get(x)       _BGQ_GET(2,59,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_MEM_SIZE_set(v)       _BGQ_SET(2,59,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_MEM_SIZE_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_WR_SIZE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_WR_SIZE_position     61
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_WR_SIZE_get(x)       _BGQ_GET(2,61,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_WR_SIZE_set(v)       _BGQ_SET(2,61,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_WR_SIZE_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_RD_SIZE_width        2
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_RD_SIZE_position     63
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_RD_SIZE_get(x)       _BGQ_GET(2,63,x)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_RD_SIZE_set(v)       _BGQ_SET(2,63,v)
#define DEVBUS_DCR__DB_INTERRUPT_STATE_CONTROL_HIGH__PCIE_CFGIO_RD_SIZE_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0021) // Offset of Control register in instance

#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define DEVBUS_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct devbus_dcr { 
    /* 0x0000 */ uint64_t devbus_control;
    /* 0x0001 */ uint64_t devbus_debug;
    /* 0x0002 */ uint64_t devbus_peek;
    /* 0x0003 */ uint64_t pcie_cfg;
    /* 0x0004 */ uint64_t switch_cfg;
    /* 0x0005 */ uint64_t spare;
    /* 0x0006 */ uint64_t reserve_0006[0x000A];
    /* 0x0010 */ uint64_t db_interrupt_state__state;
    /* 0x0011 */ uint64_t db_interrupt_state_control_low;
    /* 0x0012 */ uint64_t db_interrupt_state_control_high;
    /* 0x0013 */ uint64_t db_interrupt_state__force;
    /* 0x0014 */ uint64_t db_interrupt_state__first;
    /* 0x0015 */ uint64_t db_interrupt_state__machine_check;
    /* 0x0016 */ uint64_t db_interrupt_state__critical;
    /* 0x0017 */ uint64_t db_interrupt_state__noncritical;
    /* 0x0018 */ uint64_t reserve_0018[0x0008];
    /* 0x0020 */ uint64_t interrupt_internal_error__state;
    /* 0x0021 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0022 */ uint64_t interrupt_internal_error__force;
    /* 0x0023 */ uint64_t interrupt_internal_error__first;
    /* 0x0024 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0025 */ uint64_t interrupt_internal_error__critical;
    /* 0x0026 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0027 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0028 */ uint64_t interrupt_internal_error_hw_info;
    /* 0x0029 */ uint64_t interrupt_internal_error_data_info;
    /* 0x002A */ uint64_t reserve_002A[0x0006];
    /* 0x0030 */ uint64_t upc_db_counter[DEVBUS_DCR__UPC_DB_COUNTER_range];
    /* 0x0037 */ uint64_t reserve_0037;
    /* 0x0038 */ uint64_t upc_db_counter_control_rw;
    /* 0x0039 */ uint64_t upc_db_counter_control_w1s;
    /* 0x003A */ uint64_t upc_db_counter_control_w1c;
    /* 0x003B */ uint64_t upc_db_config;
} devbus_dcr_t;
  
#define DEVBUS_DCR_USER_PTR _DCR_USER_PTR(devbus_dcr_t, DEVBUS_DCR)
#define DEVBUS_DCR_PRIV_PTR _DCR_PRIV_PTR(devbus_dcr_t, DEVBUS_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
