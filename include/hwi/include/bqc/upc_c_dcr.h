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

 
#ifndef   _UPC_C_DCR__H_ // Prevent multiple inclusion
#define   _UPC_C_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define UPC_C_DCR_base    (0x0)  // Base address for all instances
#define UPC_C_DCR_size    (0x0848)    // Size of instance (largest offset + 1 )
  
#define UPC_C_DCR(reg) _DCR_REG(UPC_C_DCR, reg)
  
// Register:
//   Name:        psram_data
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Processor SRAM locations 0-1023
#define UPC_C_DCR__PSRAM_DATA_offset  (0x0000) // Offset of register in instance 
#define UPC_C_DCR__PSRAM_DATA_range  (0x400) // Range of external address space
#define UPC_C_DCR__PSRAM_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: Processor SRAM data
#define UPC_C_DCR__PSRAM_DATA__DATA_width        64
#define UPC_C_DCR__PSRAM_DATA__DATA_position     63
#define UPC_C_DCR__PSRAM_DATA__DATA_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__PSRAM_DATA__DATA_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__PSRAM_DATA__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mode1_count
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Mode 1 Counter Data
#define UPC_C_DCR__MODE1_COUNT_offset  (0x0400) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNT_range  (0x60) // Range of external address space
#define UPC_C_DCR__MODE1_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ovf
//   Description: Counter overflow bit. Should not be set in normal operation. Any write to this register clears this bit.
#define UPC_C_DCR__MODE1_COUNT__OVF_width        1
#define UPC_C_DCR__MODE1_COUNT__OVF_position     49
#define UPC_C_DCR__MODE1_COUNT__OVF_get(x)       _BGQ_GET(1,49,x)
#define UPC_C_DCR__MODE1_COUNT__OVF_set(v)       _BGQ_SET(1,49,v)
#define UPC_C_DCR__MODE1_COUNT__OVF_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       data
//   Description: Mode 1 Counter Data. Counters 71,79,87,95 are not implemented. Unimplemented counters will return unknown data.
#define UPC_C_DCR__MODE1_COUNT__DATA_width        14
#define UPC_C_DCR__MODE1_COUNT__DATA_position     63
#define UPC_C_DCR__MODE1_COUNT__DATA_get(x)       _BGQ_GET(14,63,x)
#define UPC_C_DCR__MODE1_COUNT__DATA_set(v)       _BGQ_SET(14,63,v)
#define UPC_C_DCR__MODE1_COUNT__DATA_insert(x,v)  _BGQ_INSERT(14,63,x,v)

  
// Register:
//   Name:        psram_mode2_data
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Processor SRAM locations 0-255, mode2 missing data bits
#define UPC_C_DCR__PSRAM_MODE2_DATA_offset  (0x0500) // Offset of register in instance 
#define UPC_C_DCR__PSRAM_MODE2_DATA_range  (0x100) // Range of external address space
#define UPC_C_DCR__PSRAM_MODE2_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: Processor Mode2 SRAM data. This register is read-only. Writes to this register will have undefined results.
#define UPC_C_DCR__PSRAM_MODE2_DATA__DATA_width        32
#define UPC_C_DCR__PSRAM_MODE2_DATA__DATA_position     63
#define UPC_C_DCR__PSRAM_MODE2_DATA__DATA_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__PSRAM_MODE2_DATA__DATA_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__PSRAM_MODE2_DATA__DATA_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        iosram_data
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C IO SRAM locations 0-255
#define UPC_C_DCR__IOSRAM_DATA_offset  (0x0600) // Offset of register in instance 
#define UPC_C_DCR__IOSRAM_DATA_range  (0x100) // Range of external address space
#define UPC_C_DCR__IOSRAM_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: IO SRAM data
#define UPC_C_DCR__IOSRAM_DATA__DATA_width        64
#define UPC_C_DCR__IOSRAM_DATA__DATA_position     63
#define UPC_C_DCR__IOSRAM_DATA__DATA_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__IOSRAM_DATA__DATA_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__IOSRAM_DATA__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Configuration Register
#define UPC_C_DCR__CONFIG_offset  (0x0800) // Offset of register in instance 
#define UPC_C_DCR__CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       spare
//   Description: Spare R/W Configuration Bits
#define UPC_C_DCR__CONFIG__SPARE_width        14
#define UPC_C_DCR__CONFIG__SPARE_position     13
#define UPC_C_DCR__CONFIG__SPARE_get(x)       _BGQ_GET(14,13,x)
#define UPC_C_DCR__CONFIG__SPARE_set(v)       _BGQ_SET(14,13,v)
#define UPC_C_DCR__CONFIG__SPARE_insert(x,v)  _BGQ_INSERT(14,13,x,v)

//   Field:       mode
//   Description: 0: Distributed Count Mode 1: Detailed Count Mode 2: Trace Mode
#define UPC_C_DCR__CONFIG__MODE_width        2
#define UPC_C_DCR__CONFIG__MODE_position     33
#define UPC_C_DCR__CONFIG__MODE_get(x)       _BGQ_GET(2,33,x)
#define UPC_C_DCR__CONFIG__MODE_set(v)       _BGQ_SET(2,33,v)
#define UPC_C_DCR__CONFIG__MODE_insert(x,v)  _BGQ_INSERT(2,33,x,v)

//   Field:       upc_c_enable
//   Description: Enable UPC_C for counting. This bit clock gates most of the UPC_C central function.
#define UPC_C_DCR__CONFIG__UPC_C_ENABLE_width        1
#define UPC_C_DCR__CONFIG__UPC_C_ENABLE_position     34
#define UPC_C_DCR__CONFIG__UPC_C_ENABLE_get(x)       _BGQ_GET(1,34,x)
#define UPC_C_DCR__CONFIG__UPC_C_ENABLE_set(v)       _BGQ_SET(1,34,v)
#define UPC_C_DCR__CONFIG__UPC_C_ENABLE_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       l2_combine
//   Description: Combines all L2 slices into a single counter group. All results from L2(0-15) will be placed in the data area for L2(0)
#define UPC_C_DCR__CONFIG__L2_COMBINE_width        1
#define UPC_C_DCR__CONFIG__L2_COMBINE_position     47
#define UPC_C_DCR__CONFIG__L2_COMBINE_get(x)       _BGQ_GET(1,47,x)
#define UPC_C_DCR__CONFIG__L2_COMBINE_set(v)       _BGQ_SET(1,47,v)
#define UPC_C_DCR__CONFIG__L2_COMBINE_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       accum_reset
//   Description: Holds Accumulator State machine in reset state
#define UPC_C_DCR__CONFIG__ACCUM_RESET_width        1
#define UPC_C_DCR__CONFIG__ACCUM_RESET_position     48
#define UPC_C_DCR__CONFIG__ACCUM_RESET_get(x)       _BGQ_GET(1,48,x)
#define UPC_C_DCR__CONFIG__ACCUM_RESET_set(v)       _BGQ_SET(1,48,v)
#define UPC_C_DCR__CONFIG__ACCUM_RESET_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       mode2_run
//   Description: Enables Mode 2 Data Capture. Starts capture into sram. 
#define UPC_C_DCR__CONFIG__MODE2_RUN_width        1
#define UPC_C_DCR__CONFIG__MODE2_RUN_position     49
#define UPC_C_DCR__CONFIG__MODE2_RUN_get(x)       _BGQ_GET(1,49,x)
#define UPC_C_DCR__CONFIG__MODE2_RUN_set(v)       _BGQ_SET(1,49,v)
#define UPC_C_DCR__CONFIG__MODE2_RUN_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       mode2_capture_cycles
//   Description: Mode 2 Trigger Position. This setting sets the number of sram write cycles captured (6 cycles of x1 44 bit trace data for each sram location) into SRAM following a trigger event. The number of cycles captured by the hardware is mode2_capture_cycles + 1. A value of 255 is not allowed.
#define UPC_C_DCR__CONFIG__MODE2_CAPTURE_CYCLES_width        8
#define UPC_C_DCR__CONFIG__MODE2_CAPTURE_CYCLES_position     57
#define UPC_C_DCR__CONFIG__MODE2_CAPTURE_CYCLES_get(x)       _BGQ_GET(8,57,x)
#define UPC_C_DCR__CONFIG__MODE2_CAPTURE_CYCLES_set(v)       _BGQ_SET(8,57,v)
#define UPC_C_DCR__CONFIG__MODE2_CAPTURE_CYCLES_insert(x,v)  _BGQ_INSERT(8,57,x,v)

//   Field:       mode2_upc_p_trig_en
//   Description: Enables UPC_P to trigger mode2 capture
#define UPC_C_DCR__CONFIG__MODE2_UPC_P_TRIG_EN_width        1
#define UPC_C_DCR__CONFIG__MODE2_UPC_P_TRIG_EN_position     60
#define UPC_C_DCR__CONFIG__MODE2_UPC_P_TRIG_EN_get(x)       _BGQ_GET(1,60,x)
#define UPC_C_DCR__CONFIG__MODE2_UPC_P_TRIG_EN_set(v)       _BGQ_SET(1,60,v)
#define UPC_C_DCR__CONFIG__MODE2_UPC_P_TRIG_EN_insert(x,v)  _BGQ_INSERT(1,60,x,v)

//   Field:       mode1_sync_override
//   Description: Sync override for Mode 1 UPC_C counters
#define UPC_C_DCR__CONFIG__MODE1_SYNC_OVERRIDE_width        1
#define UPC_C_DCR__CONFIG__MODE1_SYNC_OVERRIDE_position     61
#define UPC_C_DCR__CONFIG__MODE1_SYNC_OVERRIDE_get(x)       _BGQ_GET(1,61,x)
#define UPC_C_DCR__CONFIG__MODE1_SYNC_OVERRIDE_set(v)       _BGQ_SET(1,61,v)
#define UPC_C_DCR__CONFIG__MODE1_SYNC_OVERRIDE_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       sram_deepsleep
//   Description: Put UPC_C Sram to Sleep
#define UPC_C_DCR__CONFIG__SRAM_DEEPSLEEP_width        1
#define UPC_C_DCR__CONFIG__SRAM_DEEPSLEEP_position     62
#define UPC_C_DCR__CONFIG__SRAM_DEEPSLEEP_get(x)       _BGQ_GET(1,62,x)
#define UPC_C_DCR__CONFIG__SRAM_DEEPSLEEP_set(v)       _BGQ_SET(1,62,v)
#define UPC_C_DCR__CONFIG__SRAM_DEEPSLEEP_insert(x,v)  _BGQ_INSERT(1,62,x,v)

  
// Register:
//   Name:        mode1_ec_config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Mode 1 Event Combine Configuration Register
#define UPC_C_DCR__MODE1_EC_CONFIG_offset  (0x0801) // Offset of register in instance 
#define UPC_C_DCR__MODE1_EC_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       combine
//   Description: Each bit controls the corresponding even mode1 local counter. When set, events from the matching odd counter will be accumulated into the even counter. Bits 16 to 63 map to even local counter numbers (Bit 16 to counter 0, bit 63 to counter 94). As an example, if bit 16 is set, counter 0 will count events 0 and 1 from CCG0. Odd counters are not affected by this field.
#define UPC_C_DCR__MODE1_EC_CONFIG__COMBINE_width        48
#define UPC_C_DCR__MODE1_EC_CONFIG__COMBINE_position     63
#define UPC_C_DCR__MODE1_EC_CONFIG__COMBINE_get(x)       _BGQ_GET(48,63,x)
#define UPC_C_DCR__MODE1_EC_CONFIG__COMBINE_set(v)       _BGQ_SET(48,63,v)
#define UPC_C_DCR__MODE1_EC_CONFIG__COMBINE_insert(x,v)  _BGQ_INSERT(48,63,x,v)

  
// Register:
//   Name:        status
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Status Register
#define UPC_C_DCR__STATUS_offset  (0x0802) // Offset of register in instance 
#define UPC_C_DCR__STATUS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       pring_send_state
//   Description: Processor UPC Ring Send State Machine current state
#define UPC_C_DCR__STATUS__PRING_SEND_STATE_width        7
#define UPC_C_DCR__STATUS__PRING_SEND_STATE_position     6
#define UPC_C_DCR__STATUS__PRING_SEND_STATE_get(x)       _BGQ_GET(7,6,x)
#define UPC_C_DCR__STATUS__PRING_SEND_STATE_set(v)       _BGQ_SET(7,6,v)
#define UPC_C_DCR__STATUS__PRING_SEND_STATE_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       pring_rcv_state
//   Description: Processor UPC Ring Receive State Machine current state
#define UPC_C_DCR__STATUS__PRING_RCV_STATE_width        12
#define UPC_C_DCR__STATUS__PRING_RCV_STATE_position     18
#define UPC_C_DCR__STATUS__PRING_RCV_STATE_get(x)       _BGQ_GET(12,18,x)
#define UPC_C_DCR__STATUS__PRING_RCV_STATE_set(v)       _BGQ_SET(12,18,v)
#define UPC_C_DCR__STATUS__PRING_RCV_STATE_insert(x,v)  _BGQ_INSERT(12,18,x,v)

//   Field:       pring_rcv_cycle
//   Description: Processor UPC Ring Receive State Machine current cycle being received
#define UPC_C_DCR__STATUS__PRING_RCV_CYCLE_width        9
#define UPC_C_DCR__STATUS__PRING_RCV_CYCLE_position     27
#define UPC_C_DCR__STATUS__PRING_RCV_CYCLE_get(x)       _BGQ_GET(9,27,x)
#define UPC_C_DCR__STATUS__PRING_RCV_CYCLE_set(v)       _BGQ_SET(9,27,v)
#define UPC_C_DCR__STATUS__PRING_RCV_CYCLE_insert(x,v)  _BGQ_INSERT(9,27,x,v)

//   Field:       io_rcv_state
//   Description: IO UPC Ring Receive State Machine current state
#define UPC_C_DCR__STATUS__IO_RCV_STATE_width        5
#define UPC_C_DCR__STATUS__IO_RCV_STATE_position     32
#define UPC_C_DCR__STATUS__IO_RCV_STATE_get(x)       _BGQ_GET(5,32,x)
#define UPC_C_DCR__STATUS__IO_RCV_STATE_set(v)       _BGQ_SET(5,32,v)
#define UPC_C_DCR__STATUS__IO_RCV_STATE_insert(x,v)  _BGQ_INSERT(5,32,x,v)

//   Field:       io_rcv_cycle
//   Description: IO UPC Ring Receive State Machine current cycle being received
#define UPC_C_DCR__STATUS__IO_RCV_CYCLE_width        9
#define UPC_C_DCR__STATUS__IO_RCV_CYCLE_position     41
#define UPC_C_DCR__STATUS__IO_RCV_CYCLE_get(x)       _BGQ_GET(9,41,x)
#define UPC_C_DCR__STATUS__IO_RCV_CYCLE_set(v)       _BGQ_SET(9,41,v)
#define UPC_C_DCR__STATUS__IO_RCV_CYCLE_insert(x,v)  _BGQ_INSERT(9,41,x,v)

//   Field:       mode2_trigger_addr
//   Description: Mode2 SRAM address where trigger occurred
#define UPC_C_DCR__STATUS__MODE2_TRIGGER_ADDR_width        8
#define UPC_C_DCR__STATUS__MODE2_TRIGGER_ADDR_position     49
#define UPC_C_DCR__STATUS__MODE2_TRIGGER_ADDR_get(x)       _BGQ_GET(8,49,x)
#define UPC_C_DCR__STATUS__MODE2_TRIGGER_ADDR_set(v)       _BGQ_SET(8,49,v)
#define UPC_C_DCR__STATUS__MODE2_TRIGGER_ADDR_insert(x,v)  _BGQ_INSERT(8,49,x,v)

//   Field:       spare
//   Description: spare status bits
#define UPC_C_DCR__STATUS__SPARE_width        14
#define UPC_C_DCR__STATUS__SPARE_position     63
#define UPC_C_DCR__STATUS__SPARE_get(x)       _BGQ_GET(14,63,x)
#define UPC_C_DCR__STATUS__SPARE_set(v)       _BGQ_SET(14,63,v)
#define UPC_C_DCR__STATUS__SPARE_insert(x,v)  _BGQ_INSERT(14,63,x,v)

  
// Register:
//   Name:        mode1_counter_control0_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 0-31 RW Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW_offset  (0x0803) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 0-31(RW). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 0-31(RW)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_RW__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control0_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 0-31 W1S Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S_offset  (0x0804) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 0-31(W1S). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 0-31(W1S)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control0_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 0-31 W1C Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C_offset  (0x0805) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 0-31(W1C). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 0-31(W1C)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL0_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control1_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 32-63 RW Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW_offset  (0x0806) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 32-63(RW). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 32-63(RW)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_RW__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control1_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 32-63 W1S Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S_offset  (0x0807) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 32-63(W1S). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 32-63(W1S)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control1_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 32-63 W1C Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C_offset  (0x0808) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 32-63(W1C). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__RESET_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__RESET_position     31
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__RESET_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__RESET_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__RESET_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 32-63(W1C)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__ENABLE_width        32
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__ENABLE_position     63
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__ENABLE_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__ENABLE_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL1_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mode1_counter_control2_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 64-91 RW Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW_offset  (0x0809) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 64-91(RW). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__RESET_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__RESET_position     27
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__RESET_get(x)       _BGQ_GET(28,27,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__RESET_set(v)       _BGQ_SET(28,27,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__RESET_insert(x,v)  _BGQ_INSERT(28,27,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 64-91(RW)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__ENABLE_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__ENABLE_position     59
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__ENABLE_get(x)       _BGQ_GET(28,59,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__ENABLE_set(v)       _BGQ_SET(28,59,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_RW__ENABLE_insert(x,v)  _BGQ_INSERT(28,59,x,v)

  
// Register:
//   Name:        mode1_counter_control2_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 64-91 W1S Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S_offset  (0x080A) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 64-91(W1S). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__RESET_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__RESET_position     27
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__RESET_get(x)       _BGQ_GET(28,27,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__RESET_set(v)       _BGQ_SET(28,27,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__RESET_insert(x,v)  _BGQ_INSERT(28,27,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 64-91(W1S)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__ENABLE_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__ENABLE_position     59
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__ENABLE_get(x)       _BGQ_GET(28,59,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__ENABLE_set(v)       _BGQ_SET(28,59,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(28,59,x,v)

  
// Register:
//   Name:        mode1_counter_control2_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC Mode 1 Counters 64-91 W1C Control Register
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C_offset  (0x080B) // Offset of register in instance 
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C_range  (0x1) // Range of external address space
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: Mode1 Counter Reset 64-91(W1C). This field reads as 0
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__RESET_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__RESET_position     27
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__RESET_get(x)       _BGQ_GET(28,27,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__RESET_set(v)       _BGQ_SET(28,27,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__RESET_insert(x,v)  _BGQ_INSERT(28,27,x,v)

//   Field:       enable
//   Description: Mode1 Counter Enable 64-91(W1C)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__ENABLE_width        28
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__ENABLE_position     59
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__ENABLE_get(x)       _BGQ_GET(28,59,x)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__ENABLE_set(v)       _BGQ_SET(28,59,v)
#define UPC_C_DCR__MODE1_COUNTER_CONTROL2_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(28,59,x,v)

  
// Register:
//   Name:        ccg_int_en
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Processor CCG Interrupt Enable Register
#define UPC_C_DCR__CCG_INT_EN_offset  (0x080C) // Offset of register in instance 
#define UPC_C_DCR__CCG_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ie
//   Description: Processor/L2 Counter Group 0 Interrupt Enable, one bit for each of the 64 Counter Groups. Each bit in this register gates the corresponding bit in the Interrupt Status W1C/W1S register before being sent to the UPC_P.
#define UPC_C_DCR__CCG_INT_EN__IE_width        64
#define UPC_C_DCR__CCG_INT_EN__IE_position     63
#define UPC_C_DCR__CCG_INT_EN__IE_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__CCG_INT_EN__IE_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__CCG_INT_EN__IE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        counter_start
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_C Counter start register
#define UPC_C_DCR__COUNTER_START_offset  (0x0812) // Offset of register in instance 
#define UPC_C_DCR__COUNTER_START_reset  UNSIGNED64(0x0000000000000000)

//   Field:       enable
//   Description: Enable Counters for counting. When this bit is set, the UPC_C sends a counter enable pulse on the upc daisy chains. This causes any counters that have their sync_override bit set to 0 to start counting at the same cycle. When deasserted, all counters will stop at the same cycle.
#define UPC_C_DCR__COUNTER_START__ENABLE_width        1
#define UPC_C_DCR__COUNTER_START__ENABLE_position     35
#define UPC_C_DCR__COUNTER_START__ENABLE_get(x)       _BGQ_GET(1,35,x)
#define UPC_C_DCR__COUNTER_START__ENABLE_set(v)       _BGQ_SET(1,35,v)
#define UPC_C_DCR__COUNTER_START__ENABLE_insert(x,v)  _BGQ_INSERT(1,35,x,v)

  
// Register:
//   Name:        io_ccg_int_en
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: IO CCG Interrupt Enable Register
#define UPC_C_DCR__IO_CCG_INT_EN_offset  (0x081C) // Offset of register in instance 
#define UPC_C_DCR__IO_CCG_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ie
//   Description: IO Counter Group 0 Interrupt Enable, one bit for each of the 16 Counter Groups. Each bit in this register gates the corresponding bit in the IO Interrupt Status W1C/W1S register.
#define UPC_C_DCR__IO_CCG_INT_EN__IE_width        16
#define UPC_C_DCR__IO_CCG_INT_EN__IE_position     63
#define UPC_C_DCR__IO_CCG_INT_EN__IE_get(x)       _BGQ_GET(16,63,x)
#define UPC_C_DCR__IO_CCG_INT_EN__IE_set(v)       _BGQ_SET(16,63,v)
#define UPC_C_DCR__IO_CCG_INT_EN__IE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        ccg_int_status_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: CCG Interrupt Status W1C Register
#define UPC_C_DCR__CCG_INT_STATUS_W1C_offset  (0x080D) // Offset of register in instance 
#define UPC_C_DCR__CCG_INT_STATUS_W1C_range  (0x1) // Range of external address space
#define UPC_C_DCR__CCG_INT_STATUS_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ip
//   Description: Central Counter group counter overflow. Each bit corresponds to a CCG. The bit will be set when any counter in the CCG overflows. This bit is sticky. Writing a one to a given bit will clear it.
#define UPC_C_DCR__CCG_INT_STATUS_W1C__IP_width        64
#define UPC_C_DCR__CCG_INT_STATUS_W1C__IP_position     63
#define UPC_C_DCR__CCG_INT_STATUS_W1C__IP_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__CCG_INT_STATUS_W1C__IP_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__CCG_INT_STATUS_W1C__IP_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        ccg_int_status_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: CCG Interrupt Status W1S Register
#define UPC_C_DCR__CCG_INT_STATUS_W1S_offset  (0x080E) // Offset of register in instance 
#define UPC_C_DCR__CCG_INT_STATUS_W1S_range  (0x1) // Range of external address space
#define UPC_C_DCR__CCG_INT_STATUS_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ip
//   Description: Central Counter group counter overflow. Each bit corresponds to a CCG. The bit will be set when any counter in the CCG overflows. This bit is sticky. Writing a one to a given bit will set it.
#define UPC_C_DCR__CCG_INT_STATUS_W1S__IP_width        64
#define UPC_C_DCR__CCG_INT_STATUS_W1S__IP_position     63
#define UPC_C_DCR__CCG_INT_STATUS_W1S__IP_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__CCG_INT_STATUS_W1S__IP_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__CCG_INT_STATUS_W1S__IP_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        io_ccg_int_status_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: IO CCG Interrupt Status W1C Register
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C_offset  (0x081D) // Offset of register in instance 
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C_range  (0x1) // Range of external address space
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ip
//   Description: Central Counter group counter overflow. Each bit corresponds to a CCG. The bit will be set when any counter in the CCG overflows. This bit is sticky. Writing a one to a given bit will clear it.
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C__IP_width        16
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C__IP_position     63
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C__IP_get(x)       _BGQ_GET(16,63,x)
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C__IP_set(v)       _BGQ_SET(16,63,v)
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1C__IP_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        io_ccg_int_status_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: IO CCG Interrupt Status W1S Register
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S_offset  (0x081E) // Offset of register in instance 
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S_range  (0x1) // Range of external address space
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ip
//   Description: Central Counter group counter overflow. Each bit corresponds to a CCG. The bit will be set when any counter in the CCG overflows. This bit is sticky. Writing a one to a given bit will set it.
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S__IP_width        16
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S__IP_position     63
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S__IP_get(x)       _BGQ_GET(16,63,x)
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S__IP_set(v)       _BGQ_SET(16,63,v)
#define UPC_C_DCR__IO_CCG_INT_STATUS_W1S__IP_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        accum_control
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Accumulate Control Register
#define UPC_C_DCR__ACCUM_CONTROL_offset  (0x080F) // Offset of register in instance 
#define UPC_C_DCR__ACCUM_CONTROL_range  (0x1) // Range of external address space
#define UPC_C_DCR__ACCUM_CONTROL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       go
//   Description: Execute bit for Accumulation Engine. Set this bit to one with appropriate fields in this register to perform an accumulate operation.
#define UPC_C_DCR__ACCUM_CONTROL__GO_width        1
#define UPC_C_DCR__ACCUM_CONTROL__GO_position     0
#define UPC_C_DCR__ACCUM_CONTROL__GO_get(x)       _BGQ_GET(1,0,x)
#define UPC_C_DCR__ACCUM_CONTROL__GO_set(v)       _BGQ_SET(1,0,v)
#define UPC_C_DCR__ACCUM_CONTROL__GO_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       wait_time
//   Description: Time the Accumulate Engine waits before executing. This allows counter values in UPC_P to reach the UPC_C before being accumulated. Reads back as 0. Only valid when go bit is set.
#define UPC_C_DCR__ACCUM_CONTROL__WAIT_TIME_width        9
#define UPC_C_DCR__ACCUM_CONTROL__WAIT_TIME_position     15
#define UPC_C_DCR__ACCUM_CONTROL__WAIT_TIME_get(x)       _BGQ_GET(9,15,x)
#define UPC_C_DCR__ACCUM_CONTROL__WAIT_TIME_set(v)       _BGQ_SET(9,15,v)
#define UPC_C_DCR__ACCUM_CONTROL__WAIT_TIME_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       mask
//   Description: CCG Counter Bit Mask. Defines which counters in the destination that will be updated 
#define UPC_C_DCR__ACCUM_CONTROL__MASK_width        16
#define UPC_C_DCR__ACCUM_CONTROL__MASK_position     31
#define UPC_C_DCR__ACCUM_CONTROL__MASK_get(x)       _BGQ_GET(16,31,x)
#define UPC_C_DCR__ACCUM_CONTROL__MASK_set(v)       _BGQ_SET(16,31,v)
#define UPC_C_DCR__ACCUM_CONTROL__MASK_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       source_a
//   Description: A Source Operand SRAM address
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_A_width        8
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_A_position     39
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_A_get(x)       _BGQ_GET(8,39,x)
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_A_set(v)       _BGQ_SET(8,39,v)
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_A_insert(x,v)  _BGQ_INSERT(8,39,x,v)

//   Field:       source_b
//   Description: B Source Operand SRAM address
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_B_width        8
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_B_position     47
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_B_get(x)       _BGQ_GET(8,47,x)
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_B_set(v)       _BGQ_SET(8,47,v)
#define UPC_C_DCR__ACCUM_CONTROL__SOURCE_B_insert(x,v)  _BGQ_INSERT(8,47,x,v)

//   Field:       dest
//   Description: Destination SRAM address
#define UPC_C_DCR__ACCUM_CONTROL__DEST_width        8
#define UPC_C_DCR__ACCUM_CONTROL__DEST_position     55
#define UPC_C_DCR__ACCUM_CONTROL__DEST_get(x)       _BGQ_GET(8,55,x)
#define UPC_C_DCR__ACCUM_CONTROL__DEST_set(v)       _BGQ_SET(8,55,v)
#define UPC_C_DCR__ACCUM_CONTROL__DEST_insert(x,v)  _BGQ_INSERT(8,55,x,v)

//   Field:       busy_fail
//   Description: Accumulate Engine failed to execute accumulate transaction because accumulate operation was started while the accumulate engine was busy
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_FAIL_width        1
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_FAIL_position     62
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_FAIL_get(x)       _BGQ_GET(1,62,x)
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_FAIL_set(v)       _BGQ_SET(1,62,v)
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_FAIL_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       busy
//   Description: Accumulate Engine is busy.
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_width        1
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_position     63
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_get(x)       _BGQ_GET(1,63,x)
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_set(v)       _BGQ_SET(1,63,v)
#define UPC_C_DCR__ACCUM_CONTROL__BUSY_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        error_force
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged Only
//   Reset:       0000000000000000
//   Description: Error Force Register - Forces errors into UPC_C SRAM parity bits
#define UPC_C_DCR__ERROR_FORCE_offset  (0x0810) // Offset of register in instance 
#define UPC_C_DCR__ERROR_FORCE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       psram_write_perr
//   Description: Forces an error on each UPC_P/L2 SRAM and IO SRAM Write Port
#define UPC_C_DCR__ERROR_FORCE__PSRAM_WRITE_PERR_width        32
#define UPC_C_DCR__ERROR_FORCE__PSRAM_WRITE_PERR_position     63
#define UPC_C_DCR__ERROR_FORCE__PSRAM_WRITE_PERR_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__ERROR_FORCE__PSRAM_WRITE_PERR_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__ERROR_FORCE__PSRAM_WRITE_PERR_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        m2_trigger
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Mode 2 Trigger generation
#define UPC_C_DCR__M2_TRIGGER_offset  (0x0811) // Offset of register in instance 
#define UPC_C_DCR__M2_TRIGGER_range  (0x1) // Range of external address space
#define UPC_C_DCR__M2_TRIGGER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       trigger
//   Description: In mode 2, writing a 1 to this bit sends a trigger. This bit is read as 0
#define UPC_C_DCR__M2_TRIGGER__TRIGGER_width        1
#define UPC_C_DCR__M2_TRIGGER__TRIGGER_position     63
#define UPC_C_DCR__M2_TRIGGER__TRIGGER_get(x)       _BGQ_GET(1,63,x)
#define UPC_C_DCR__M2_TRIGGER__TRIGGER_set(v)       _BGQ_SET(1,63,v)
#define UPC_C_DCR__M2_TRIGGER__TRIGGER_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        upc_sram_parity_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Processor/L2 SRAM Parity Error Info
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO_offset  (0x082F) // Offset of register in instance 
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       hra_parity_error
//   Description: Holding Register A Parity Error
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRA_PARITY_ERROR_width        32
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRA_PARITY_ERROR_position     31
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRA_PARITY_ERROR_get(x)       _BGQ_GET(32,31,x)
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRA_PARITY_ERROR_set(v)       _BGQ_SET(32,31,v)
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRA_PARITY_ERROR_insert(x,v)  _BGQ_INSERT(32,31,x,v)

//   Field:       hrb_parity_error
//   Description: Holding Register B Parity Error
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRB_PARITY_ERROR_width        32
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRB_PARITY_ERROR_position     63
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRB_PARITY_ERROR_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRB_PARITY_ERROR_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__UPC_SRAM_PARITY_INFO__HRB_PARITY_ERROR_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        upc_io_sram_parity_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: IO SRAM Parity Error Info
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO_offset  (0x082E) // Offset of register in instance 
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       hrb_parity_error
//   Description: Holding Register B Parity Error
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO__HRB_PARITY_ERROR_width        32
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO__HRB_PARITY_ERROR_position     63
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO__HRB_PARITY_ERROR_get(x)       _BGQ_GET(32,63,x)
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO__HRB_PARITY_ERROR_set(v)       _BGQ_SET(32,63,v)
#define UPC_C_DCR__UPC_IO_SRAM_PARITY_INFO__HRB_PARITY_ERROR_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0837) // Offset of register in instance 
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        12
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(12,63,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(12,63,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0838) // Offset of register in instance 
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     5
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(2,5,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(2,5,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(2,5,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        12
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(12,63,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(12,63,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0839) // Offset of register in instance 
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: upc_c_interrupt_state
//   State Permission: All
//   First Permission: All
//   Force Permission: Privileged Only
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__STATE_offset         (0x0820) // Offset of State register in instance
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__FIRST_offset         (0x0824) // Offset of First register in instance
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__FORCE_offset         (0x0823) // Offset of Force register in instance
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MACHINE_CHECK_offset (0x0825) // Offset of Machine Check register in instance
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__CRITICAL_offset      (0x0826) // Offset of Critical register in instance
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__NONCRITICAL_offset   (0x0827) // Offset of Noncritical register in instance

#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__M1_OVF_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__M1_OVF_position     51
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__M1_OVF_get(x)       _BGQ_GET(1,51,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__M1_OVF_set(v)       _BGQ_SET(1,51,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__M1_OVF_insert(x,v)  _BGQ_INSERT(1,51,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_position     53
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_get(x)       _BGQ_GET(1,53,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_set(v)       _BGQ_SET(1,53,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_ERROR_insert(x,v)  _BGQ_INSERT(1,53,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_position     54
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_get(x)       _BGQ_GET(1,54,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_set(v)       _BGQ_SET(1,54,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IORING_RCV_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,54,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_position     55
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_get(x)       _BGQ_GET(1,55,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_set(v)       _BGQ_SET(1,55,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__IO_SRAM_PERR_insert(x,v)  _BGQ_INSERT(1,55,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_position     56
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_get(x)       _BGQ_GET(1,56,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_set(v)       _BGQ_SET(1,56,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__UPC_P_SRAM_PERR_insert(x,v)  _BGQ_INSERT(1,56,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_position     57
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_get(x)       _BGQ_GET(1,57,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_set(v)       _BGQ_SET(1,57,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__ACCUM_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,57,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_position     58
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_get(x)       _BGQ_GET(1,58,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_set(v)       _BGQ_SET(1,58,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__MMIO_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,58,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_position     59
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_get(x)       _BGQ_GET(1,59,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_set(v)       _BGQ_SET(1,59,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_RCV_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,59,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_position     60
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_get(x)       _BGQ_GET(1,60,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_set(v)       _BGQ_SET(1,60,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_SEND_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,60,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_position     61
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_get(x)       _BGQ_GET(1,61,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_set(v)       _BGQ_SET(1,61,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__PRING_ERROR_insert(x,v)  _BGQ_INSERT(1,61,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_width        1
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_position     62
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_get(x)       _BGQ_GET(1,62,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_set(v)       _BGQ_SET(1,62,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE__DEVBUS_CTL_PERR_insert(x,v)  _BGQ_INSERT(1,62,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged Only
//   First Permission: All
//   Force Permission: Privileged Only
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0830) // Offset of State register in instance
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0833) // Offset of First register in instance
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0832) // Offset of Force register in instance
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0834) // Offset of Machine Check register in instance
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0835) // Offset of Critical register in instance
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0836) // Offset of Noncritical register in instance

#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt State Register: upc_c_overflow_state
//   State Permission: All
//   First Permission: All
//   Force Permission: Privileged Only
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__STATE_offset         (0x0840) // Offset of State register in instance
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__FIRST_offset         (0x0844) // Offset of First register in instance
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__FORCE_offset         (0x0843) // Offset of Force register in instance
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MACHINE_CHECK_offset (0x0845) // Offset of Machine Check register in instance
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__CRITICAL_offset      (0x0846) // Offset of Critical register in instance
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__NONCRITICAL_offset   (0x0847) // Offset of Noncritical register in instance

#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MODE2_DONE_width        1
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MODE2_DONE_position     61
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MODE2_DONE_get(x)       _BGQ_GET(1,61,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MODE2_DONE_set(v)       _BGQ_SET(1,61,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__MODE2_DONE_insert(x,v)  _BGQ_INSERT(1,61,x,v)


#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__IO_COUNTER_OVF_width        1
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__IO_COUNTER_OVF_position     62
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__IO_COUNTER_OVF_get(x)       _BGQ_GET(1,62,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__IO_COUNTER_OVF_set(v)       _BGQ_SET(1,62,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__IO_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,62,x,v)


#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__PROC_COUNTER_OVF_width        1
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__PROC_COUNTER_OVF_position     63
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__PROC_COUNTER_OVF_get(x)       _BGQ_GET(1,63,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__PROC_COUNTER_OVF_set(v)       _BGQ_SET(1,63,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE__PROC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt Control Register: upc_c_interrupt_state_control_low
//   Permission: Read: All; Write: Privileged Only
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW_offset         (0x0821) // Offset of Control register in instance

#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_position     39
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_get(x)       _BGQ_GET(2,39,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_set(v)       _BGQ_SET(2,39,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__M1_OVF_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_position     43
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_get(x)       _BGQ_GET(2,43,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_set(v)       _BGQ_SET(2,43,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_ERROR_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_position     45
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_get(x)       _BGQ_GET(2,45,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_set(v)       _BGQ_SET(2,45,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IORING_RCV_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_position     47
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_get(x)       _BGQ_GET(2,47,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_set(v)       _BGQ_SET(2,47,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__IO_SRAM_PERR_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_position     49
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_get(x)       _BGQ_GET(2,49,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_set(v)       _BGQ_SET(2,49,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__UPC_P_SRAM_PERR_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_position     51
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_get(x)       _BGQ_GET(2,51,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_set(v)       _BGQ_SET(2,51,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__ACCUM_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_position     53
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_get(x)       _BGQ_GET(2,53,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_set(v)       _BGQ_SET(2,53,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__MMIO_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_position     55
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_get(x)       _BGQ_GET(2,55,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_set(v)       _BGQ_SET(2,55,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_RCV_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_position     57
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_get(x)       _BGQ_GET(2,57,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_set(v)       _BGQ_SET(2,57,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_SEND_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_position     59
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_get(x)       _BGQ_GET(2,59,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_set(v)       _BGQ_SET(2,59,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__PRING_ERROR_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_width        2
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_position     61
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_get(x)       _BGQ_GET(2,61,x)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_set(v)       _BGQ_SET(2,61,v)
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_LOW__DEVBUS_CTL_PERR_insert(x,v)  _BGQ_INSERT(2,61,x,v)

  
// Interrupt Control Register: upc_c_interrupt_state_control_high
//   Permission: Read: All; Write: Privileged Only
#define UPC_C_DCR__UPC_C_INTERRUPT_STATE_CONTROL_HIGH_offset         (0x0822) // Offset of Control register in instance
  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged Only
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0831) // Offset of Control register in instance

#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define UPC_C_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

  
// Interrupt Control Register: upc_c_overflow_state_control_low
//   Permission: Read: All; Write: Privileged Only
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW_offset         (0x0841) // Offset of Control register in instance

#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__MODE2_DONE_width        2
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__MODE2_DONE_position     59
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__MODE2_DONE_get(x)       _BGQ_GET(2,59,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__MODE2_DONE_set(v)       _BGQ_SET(2,59,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__MODE2_DONE_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__IO_COUNTER_OVF_width        2
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__IO_COUNTER_OVF_position     61
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__IO_COUNTER_OVF_get(x)       _BGQ_GET(2,61,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__IO_COUNTER_OVF_set(v)       _BGQ_SET(2,61,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__IO_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__PROC_COUNTER_OVF_width        2
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__PROC_COUNTER_OVF_position     63
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__PROC_COUNTER_OVF_get(x)       _BGQ_GET(2,63,x)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__PROC_COUNTER_OVF_set(v)       _BGQ_SET(2,63,v)
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_LOW__PROC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: upc_c_overflow_state_control_high
//   Permission: Read: All; Write: Privileged Only
#define UPC_C_DCR__UPC_C_OVERFLOW_STATE_CONTROL_HIGH_offset         (0x0842) // Offset of Control register in instance
#ifndef __ASSEMBLY__

typedef struct upc_c_dcr { 
    /* 0x0000 */ uint64_t psram_data[UPC_C_DCR__PSRAM_DATA_range];
    /* 0x0400 */ uint64_t mode1_count[UPC_C_DCR__MODE1_COUNT_range];
    /* 0x0460 */ uint64_t reserve_0460[0x00A0];
    /* 0x0500 */ uint64_t psram_mode2_data[UPC_C_DCR__PSRAM_MODE2_DATA_range];
    /* 0x0600 */ uint64_t iosram_data[UPC_C_DCR__IOSRAM_DATA_range];
    /* 0x0700 */ uint64_t reserve_0700[0x0100];
    /* 0x0800 */ uint64_t config;
    /* 0x0801 */ uint64_t mode1_ec_config;
    /* 0x0802 */ uint64_t status;
    /* 0x0803 */ uint64_t mode1_counter_control0_rw;
    /* 0x0804 */ uint64_t mode1_counter_control0_w1s;
    /* 0x0805 */ uint64_t mode1_counter_control0_w1c;
    /* 0x0806 */ uint64_t mode1_counter_control1_rw;
    /* 0x0807 */ uint64_t mode1_counter_control1_w1s;
    /* 0x0808 */ uint64_t mode1_counter_control1_w1c;
    /* 0x0809 */ uint64_t mode1_counter_control2_rw;
    /* 0x080A */ uint64_t mode1_counter_control2_w1s;
    /* 0x080B */ uint64_t mode1_counter_control2_w1c;
    /* 0x080C */ uint64_t ccg_int_en;
    /* 0x080D */ uint64_t ccg_int_status_w1c;
    /* 0x080E */ uint64_t ccg_int_status_w1s;
    /* 0x080F */ uint64_t accum_control;
    /* 0x0810 */ uint64_t error_force;
    /* 0x0811 */ uint64_t m2_trigger;
    /* 0x0812 */ uint64_t counter_start;
    /* 0x0813 */ uint64_t reserve_0813[0x0009];
    /* 0x081C */ uint64_t io_ccg_int_en;
    /* 0x081D */ uint64_t io_ccg_int_status_w1c;
    /* 0x081E */ uint64_t io_ccg_int_status_w1s;
    /* 0x081F */ uint64_t reserve_081F;
    /* 0x0820 */ uint64_t upc_c_interrupt_state__state;
    /* 0x0821 */ uint64_t upc_c_interrupt_state_control_low;
    /* 0x0822 */ uint64_t upc_c_interrupt_state_control_high;
    /* 0x0823 */ uint64_t upc_c_interrupt_state__force;
    /* 0x0824 */ uint64_t upc_c_interrupt_state__first;
    /* 0x0825 */ uint64_t upc_c_interrupt_state__machine_check;
    /* 0x0826 */ uint64_t upc_c_interrupt_state__critical;
    /* 0x0827 */ uint64_t upc_c_interrupt_state__noncritical;
    /* 0x0828 */ uint64_t reserve_0828[0x0006];
    /* 0x082E */ uint64_t upc_io_sram_parity_info;
    /* 0x082F */ uint64_t upc_sram_parity_info;
    /* 0x0830 */ uint64_t interrupt_internal_error__state;
    /* 0x0831 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0832 */ uint64_t interrupt_internal_error__force;
    /* 0x0833 */ uint64_t interrupt_internal_error__first;
    /* 0x0834 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0835 */ uint64_t interrupt_internal_error__critical;
    /* 0x0836 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0837 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0838 */ uint64_t interrupt_internal_error_hw_info;
    /* 0x0839 */ uint64_t interrupt_internal_error_data_info;
    /* 0x083A */ uint64_t reserve_083A[0x0006];
    /* 0x0840 */ uint64_t upc_c_overflow_state__state;
    /* 0x0841 */ uint64_t upc_c_overflow_state_control_low;
    /* 0x0842 */ uint64_t upc_c_overflow_state_control_high;
    /* 0x0843 */ uint64_t upc_c_overflow_state__force;
    /* 0x0844 */ uint64_t upc_c_overflow_state__first;
    /* 0x0845 */ uint64_t upc_c_overflow_state__machine_check;
    /* 0x0846 */ uint64_t upc_c_overflow_state__critical;
    /* 0x0847 */ uint64_t upc_c_overflow_state__noncritical;
} upc_c_dcr_t;
  
#define UPC_C_DCR_USER_PTR _DCR_USER_PTR(upc_c_dcr_t, UPC_C_DCR)
#define UPC_C_DCR_PRIV_PTR _DCR_PRIV_PTR(upc_c_dcr_t, UPC_C_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
