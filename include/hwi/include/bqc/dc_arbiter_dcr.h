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

#ifndef   _DC_ARBITER_DCR__H_ // Prevent multiple inclusion
#define   _DC_ARBITER_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define DC_ARBITER_DCR_base    (0x054000)  // Base address for all instances
#define DC_ARBITER_DCR_size    (0x0013)    // Size of instance (largest offset + 1 )
  
#define DC_ARBITER_DCR(reg) _DCR_REG(DC_ARBITER_DCR, reg)
  
// Register:
//   Name:        arb_ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000006000007
//   Description: This register provides control of the arbiter functions.
#define DC_ARBITER_DCR__ARB_CTRL_offset  (0x0000) // Offset of register in instance 
#define DC_ARBITER_DCR__ARB_CTRL_reset  UNSIGNED64(0x0000000006000007)

//   Field:       delay_val
//   Description: Set the delay value depending of the number of timing register added 
#define DC_ARBITER_DCR__ARB_CTRL__DELAY_VAL_width        4
#define DC_ARBITER_DCR__ARB_CTRL__DELAY_VAL_position     39
#define DC_ARBITER_DCR__ARB_CTRL__DELAY_VAL_get(x)       _BGQ_GET(4,39,x)
#define DC_ARBITER_DCR__ARB_CTRL__DELAY_VAL_set(v)       _BGQ_SET(4,39,v)
#define DC_ARBITER_DCR__ARB_CTRL__DELAY_VAL_insert(x,v)  _BGQ_INSERT(4,39,x,v)

//   Field:       timer_val
//   Description: Set the timer value depending of the length of the ring and                                 the time to access remote registers.
#define DC_ARBITER_DCR__ARB_CTRL__TIMER_VAL_width        4
#define DC_ARBITER_DCR__ARB_CTRL__TIMER_VAL_position     47
#define DC_ARBITER_DCR__ARB_CTRL__TIMER_VAL_get(x)       _BGQ_GET(4,47,x)
#define DC_ARBITER_DCR__ARB_CTRL__TIMER_VAL_set(v)       _BGQ_SET(4,47,v)
#define DC_ARBITER_DCR__ARB_CTRL__TIMER_VAL_insert(x,v)  _BGQ_INSERT(4,47,x,v)

//   Field:       enable_delay
//   Description: Enable the delay between 2 requests to compensate the timing registers padding.
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_DELAY_width        1
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_DELAY_position     61
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_DELAY_get(x)       _BGQ_GET(1,61,x)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_DELAY_set(v)       _BGQ_SET(1,61,v)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_DELAY_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       enable_pty
//   Description: Enable parity checking on address and data.
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_PTY_width        1
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_PTY_position     62
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_PTY_get(x)       _BGQ_GET(1,62,x)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_PTY_set(v)       _BGQ_SET(1,62,v)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_PTY_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       enable_timer
//   Description: Enable timer to check answers on the ring.                             Default ON to prevent from wrong address.
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_TIMER_width        1
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_TIMER_position     63
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_TIMER_get(x)       _BGQ_GET(1,63,x)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_TIMER_set(v)       _BGQ_SET(1,63,v)
#define DC_ARBITER_DCR__ARB_CTRL__ENABLE_TIMER_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        status_reg
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: In case of parity error or time out this register will store information on the first error.
#define DC_ARBITER_DCR__STATUS_REG_offset  (0x0001) // Offset of register in instance 
#define DC_ARBITER_DCR__STATUS_REG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ti_master
//   Description: TI master stored when the error occurs.
#define DC_ARBITER_DCR__STATUS_REG__TI_MASTER_width        1
#define DC_ARBITER_DCR__STATUS_REG__TI_MASTER_position     19
#define DC_ARBITER_DCR__STATUS_REG__TI_MASTER_get(x)       _BGQ_GET(1,19,x)
#define DC_ARBITER_DCR__STATUS_REG__TI_MASTER_set(v)       _BGQ_SET(1,19,v)
#define DC_ARBITER_DCR__STATUS_REG__TI_MASTER_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       pu_master
//   Description: PU master number stored when the error occurs.
#define DC_ARBITER_DCR__STATUS_REG__PU_MASTER_width        5
#define DC_ARBITER_DCR__STATUS_REG__PU_MASTER_position     27
#define DC_ARBITER_DCR__STATUS_REG__PU_MASTER_get(x)       _BGQ_GET(5,27,x)
#define DC_ARBITER_DCR__STATUS_REG__PU_MASTER_set(v)       _BGQ_SET(5,27,v)
#define DC_ARBITER_DCR__STATUS_REG__PU_MASTER_insert(x,v)  _BGQ_INSERT(5,27,x,v)

//   Field:       ring_ti
//   Description: DCR access stored when the error occurs.
#define DC_ARBITER_DCR__STATUS_REG__RING_TI_width        1
#define DC_ARBITER_DCR__STATUS_REG__RING_TI_position     32
#define DC_ARBITER_DCR__STATUS_REG__RING_TI_get(x)       _BGQ_GET(1,32,x)
#define DC_ARBITER_DCR__STATUS_REG__RING_TI_set(v)       _BGQ_SET(1,32,v)
#define DC_ARBITER_DCR__STATUS_REG__RING_TI_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       ring_priv
//   Description: DCR access stored when the error occurs.
#define DC_ARBITER_DCR__STATUS_REG__RING_PRIV_width        1
#define DC_ARBITER_DCR__STATUS_REG__RING_PRIV_position     33
#define DC_ARBITER_DCR__STATUS_REG__RING_PRIV_get(x)       _BGQ_GET(1,33,x)
#define DC_ARBITER_DCR__STATUS_REG__RING_PRIV_set(v)       _BGQ_SET(1,33,v)
#define DC_ARBITER_DCR__STATUS_REG__RING_PRIV_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       ring_add
//   Description: DCR address stored when the error occurs
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_width        24
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_position     59
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_get(x)       _BGQ_GET(24,59,x)
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_set(v)       _BGQ_SET(24,59,v)
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_insert(x,v)  _BGQ_INSERT(24,59,x,v)

//   Field:       ring_add_pty
//   Description: DCR address parity stored when the error occurs
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_PTY_width        3
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_PTY_position     63
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_PTY_get(x)       _BGQ_GET(3,63,x)
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_PTY_set(v)       _BGQ_SET(3,63,v)
#define DC_ARBITER_DCR__STATUS_REG__RING_ADD_PTY_insert(x,v)  _BGQ_INSERT(3,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0011) // Offset of register in instance 
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0012) // Offset of register in instance 
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0013) // Offset of register in instance 
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: int_reg
//   State Permission: All
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define DC_ARBITER_DCR__INT_REG__STATE_offset         (0x0002) // Offset of State register in instance
#define DC_ARBITER_DCR__INT_REG__FIRST_offset         (0x0006) // Offset of First register in instance
#define DC_ARBITER_DCR__INT_REG__FORCE_offset         (0x0005) // Offset of Force register in instance
#define DC_ARBITER_DCR__INT_REG__MACHINE_CHECK_offset (0x0007) // Offset of Machine Check register in instance
#define DC_ARBITER_DCR__INT_REG__CRITICAL_offset      (0x0008) // Offset of Critical register in instance
#define DC_ARBITER_DCR__INT_REG__NONCRITICAL_offset   (0x0009) // Offset of Noncritical register in instance

#define DC_ARBITER_DCR__INT_REG__LOCAL_RING_width        1
#define DC_ARBITER_DCR__INT_REG__LOCAL_RING_position     0
#define DC_ARBITER_DCR__INT_REG__LOCAL_RING_get(x)       _BGQ_GET(1,0,x)
#define DC_ARBITER_DCR__INT_REG__LOCAL_RING_set(v)       _BGQ_SET(1,0,v)
#define DC_ARBITER_DCR__INT_REG__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_width        1
#define DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_position     53
#define DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_get(x)       _BGQ_GET(1,53,x)
#define DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_set(v)       _BGQ_SET(1,53,v)
#define DC_ARBITER_DCR__INT_REG__INV_PTY_STATE_insert(x,v)  _BGQ_INSERT(1,53,x,v)


#define DC_ARBITER_DCR__INT_REG__INV_MON_STATE_width        1
#define DC_ARBITER_DCR__INT_REG__INV_MON_STATE_position     54
#define DC_ARBITER_DCR__INT_REG__INV_MON_STATE_get(x)       _BGQ_GET(1,54,x)
#define DC_ARBITER_DCR__INT_REG__INV_MON_STATE_set(v)       _BGQ_SET(1,54,v)
#define DC_ARBITER_DCR__INT_REG__INV_MON_STATE_insert(x,v)  _BGQ_INSERT(1,54,x,v)


#define DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_width        1
#define DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_position     55
#define DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_get(x)       _BGQ_GET(1,55,x)
#define DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_set(v)       _BGQ_SET(1,55,v)
#define DC_ARBITER_DCR__INT_REG__INV_ARB_STATE_insert(x,v)  _BGQ_INSERT(1,55,x,v)


#define DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_width        1
#define DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_position     58
#define DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_get(x)       _BGQ_GET(1,58,x)
#define DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_set(v)       _BGQ_SET(1,58,v)
#define DC_ARBITER_DCR__INT_REG__RING_NOT_CLEAN_insert(x,v)  _BGQ_INSERT(1,58,x,v)


#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_width        1
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_position     59
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_get(x)       _BGQ_GET(1,59,x)
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_set(v)       _BGQ_SET(1,59,v)
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_ACK_insert(x,v)  _BGQ_INSERT(1,59,x,v)


#define DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_width        1
#define DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_position     60
#define DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_get(x)       _BGQ_GET(1,60,x)
#define DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_set(v)       _BGQ_SET(1,60,v)
#define DC_ARBITER_DCR__INT_REG__NO_ACK_AFTER_REQ_insert(x,v)  _BGQ_INSERT(1,60,x,v)


#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_width        1
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_position     61
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_get(x)       _BGQ_GET(1,61,x)
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_set(v)       _BGQ_SET(1,61,v)
#define DC_ARBITER_DCR__INT_REG__NO_REQ_AFTER_GRT_insert(x,v)  _BGQ_INSERT(1,61,x,v)


#define DC_ARBITER_DCR__INT_REG__DATA_ERR_width        1
#define DC_ARBITER_DCR__INT_REG__DATA_ERR_position     62
#define DC_ARBITER_DCR__INT_REG__DATA_ERR_get(x)       _BGQ_GET(1,62,x)
#define DC_ARBITER_DCR__INT_REG__DATA_ERR_set(v)       _BGQ_SET(1,62,v)
#define DC_ARBITER_DCR__INT_REG__DATA_ERR_insert(x,v)  _BGQ_INSERT(1,62,x,v)


#define DC_ARBITER_DCR__INT_REG__ADD_ERR_width        1
#define DC_ARBITER_DCR__INT_REG__ADD_ERR_position     63
#define DC_ARBITER_DCR__INT_REG__ADD_ERR_get(x)       _BGQ_GET(1,63,x)
#define DC_ARBITER_DCR__INT_REG__ADD_ERR_set(v)       _BGQ_SET(1,63,v)
#define DC_ARBITER_DCR__INT_REG__ADD_ERR_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: All
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x000A) // Offset of State register in instance
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x000D) // Offset of First register in instance
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x000C) // Offset of Force register in instance
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x000E) // Offset of Machine Check register in instance
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x000F) // Offset of Critical register in instance
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0010) // Offset of Noncritical register in instance

#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: int_reg_control_low
//   Permission: Privileged and TestInt
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW_offset         (0x0003) // Offset of Control register in instance

#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_PTY_STATE_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_PTY_STATE_position     43
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_PTY_STATE_get(x)       _BGQ_GET(2,43,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_PTY_STATE_set(v)       _BGQ_SET(2,43,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_PTY_STATE_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_MON_STATE_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_MON_STATE_position     45
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_MON_STATE_get(x)       _BGQ_GET(2,45,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_MON_STATE_set(v)       _BGQ_SET(2,45,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_MON_STATE_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_ARB_STATE_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_ARB_STATE_position     47
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_ARB_STATE_get(x)       _BGQ_GET(2,47,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_ARB_STATE_set(v)       _BGQ_SET(2,47,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__INV_ARB_STATE_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__RING_NOT_CLEAN_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__RING_NOT_CLEAN_position     53
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__RING_NOT_CLEAN_get(x)       _BGQ_GET(2,53,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__RING_NOT_CLEAN_set(v)       _BGQ_SET(2,53,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__RING_NOT_CLEAN_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_ACK_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_ACK_position     55
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_ACK_get(x)       _BGQ_GET(2,55,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_ACK_set(v)       _BGQ_SET(2,55,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_ACK_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_ACK_AFTER_REQ_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_ACK_AFTER_REQ_position     57
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_ACK_AFTER_REQ_get(x)       _BGQ_GET(2,57,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_ACK_AFTER_REQ_set(v)       _BGQ_SET(2,57,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_ACK_AFTER_REQ_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_GRT_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_GRT_position     59
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_GRT_get(x)       _BGQ_GET(2,59,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_GRT_set(v)       _BGQ_SET(2,59,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__NO_REQ_AFTER_GRT_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__DATA_ERR_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__DATA_ERR_position     61
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__DATA_ERR_get(x)       _BGQ_GET(2,61,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__DATA_ERR_set(v)       _BGQ_SET(2,61,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__DATA_ERR_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__ADD_ERR_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__ADD_ERR_position     63
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__ADD_ERR_get(x)       _BGQ_GET(2,63,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__ADD_ERR_set(v)       _BGQ_SET(2,63,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_LOW__ADD_ERR_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: int_reg_control_high
//   Permission: Privileged and TestInt
#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH_offset         (0x0004) // Offset of Control register in instance

#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH__LOCAL_RING_width        2
#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH__LOCAL_RING_position     1
#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH__LOCAL_RING_get(x)       _BGQ_GET(2,1,x)
#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH__LOCAL_RING_set(v)       _BGQ_SET(2,1,v)
#define DC_ARBITER_DCR__INT_REG_CONTROL_HIGH__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,1,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Privileged and TestInt
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x000B) // Offset of Control register in instance

#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define DC_ARBITER_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct dc_arbiter_dcr { 
    /* 0x0000 */ uint64_t arb_ctrl;
    /* 0x0001 */ uint64_t status_reg;
    /* 0x0002 */ uint64_t int_reg__state;
    /* 0x0003 */ uint64_t int_reg_control_low;
    /* 0x0004 */ uint64_t int_reg_control_high;
    /* 0x0005 */ uint64_t int_reg__force;
    /* 0x0006 */ uint64_t int_reg__first;
    /* 0x0007 */ uint64_t int_reg__machine_check;
    /* 0x0008 */ uint64_t int_reg__critical;
    /* 0x0009 */ uint64_t int_reg__noncritical;
    /* 0x000A */ uint64_t interrupt_internal_error__state;
    /* 0x000B */ uint64_t interrupt_internal_error_control_high;
    /* 0x000C */ uint64_t interrupt_internal_error__force;
    /* 0x000D */ uint64_t interrupt_internal_error__first;
    /* 0x000E */ uint64_t interrupt_internal_error__machine_check;
    /* 0x000F */ uint64_t interrupt_internal_error__critical;
    /* 0x0010 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0011 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0012 */ uint64_t interrupt_internal_error_hw_info;
} dc_arbiter_dcr_t;
  
#define DC_ARBITER_DCR_USER_PTR _DCR_USER_PTR(dc_arbiter_dcr_t, DC_ARBITER_DCR)
#define DC_ARBITER_DCR_PRIV_PTR _DCR_PRIV_PTR(dc_arbiter_dcr_t, DC_ARBITER_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
