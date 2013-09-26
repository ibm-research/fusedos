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

#ifndef   _MU_DCR__H_ // Prevent multiple inclusion
#define   _MU_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define MU_DCR_base    (0x0E0000)  // Base address for all instances
#define MU_DCR_size    (0x0F45)    // Size of instance (largest offset + 1 )
  
#define MU_DCR(reg) _DCR_REG(MU_DCR, reg)
  
// Register:
//   Name:        imfifo_thold_int_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Enables imFIFOs n*32 to n*32+31 threshold interrrupts
#define MU_DCR__IMFIFO_THOLD_INT_EN_offset  (0x0D00) // Offset of register in instance 
#define MU_DCR__IMFIFO_THOLD_INT_EN_range  (0x11) // Range of external address space
#define MU_DCR__IMFIFO_THOLD_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: A 1 in bit i means that imFIFO 0*32+i can raise an interrupt if free space in this imFIFO is less than the injection FIFO threshold.
#define MU_DCR__IMFIFO_THOLD_INT_EN__RESERVED_width        32
#define MU_DCR__IMFIFO_THOLD_INT_EN__RESERVED_position     63
#define MU_DCR__IMFIFO_THOLD_INT_EN__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__IMFIFO_THOLD_INT_EN__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__IMFIFO_THOLD_INT_EN__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        imfifo_system
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: Controls imFIFOs n*32 to n*32+31 user program access
#define MU_DCR__IMFIFO_SYSTEM_offset  (0x0D11) // Offset of register in instance 
#define MU_DCR__IMFIFO_SYSTEM_range  (0x11) // Range of external address space
#define MU_DCR__IMFIFO_SYSTEM_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       reserved
//   Description: A 1 in bit i means that imFIFO 0*32+i is a system FIFO, and cannot be used by a user program.
#define MU_DCR__IMFIFO_SYSTEM__RESERVED_width        32
#define MU_DCR__IMFIFO_SYSTEM__RESERVED_position     63
#define MU_DCR__IMFIFO_SYSTEM__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__IMFIFO_SYSTEM__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__IMFIFO_SYSTEM__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        imfifo_rget
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: imFIFOs n*32 to n*32+31 available for remote get operation: 
#define MU_DCR__IMFIFO_RGET_offset  (0x0D22) // Offset of register in instance 
#define MU_DCR__IMFIFO_RGET_range  (0x11) // Range of external address space
#define MU_DCR__IMFIFO_RGET_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: A 1 in bit i means that imFIFO 0*32+i is reserved for use for remote get operations.
#define MU_DCR__IMFIFO_RGET__RESERVED_width        32
#define MU_DCR__IMFIFO_RGET__RESERVED_position     63
#define MU_DCR__IMFIFO_RGET__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__IMFIFO_RGET__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__IMFIFO_RGET__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        imu_thold
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This threshold register is used for all imFIFOs to check if free space in that imFIFO exceeded this value.  The value of free space is given in units of 64B (counts descriptors).
#define MU_DCR__IMU_THOLD_offset  (0x0D33) // Offset of register in instance 
#define MU_DCR__IMU_THOLD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: The threshold value.
#define MU_DCR__IMU_THOLD__VALUE_width        30
#define MU_DCR__IMU_THOLD__VALUE_position     63
#define MU_DCR__IMU_THOLD__VALUE_get(x)       _BGQ_GET(30,63,x)
#define MU_DCR__IMU_THOLD__VALUE_set(v)       _BGQ_SET(30,63,v)
#define MU_DCR__IMU_THOLD__VALUE_insert(x,v)  _BGQ_INSERT(30,63,x,v)

  
// Register:
//   Name:        rget_thold
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This threshold register is used for all imFIFOs used for remote get operation to check if free space in that imFIFO falls below this value.  The value of free space is given in units of 32B packet chunks.
#define MU_DCR__RGET_THOLD_offset  (0x0D34) // Offset of register in instance 
#define MU_DCR__RGET_THOLD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: The threshold value.
#define MU_DCR__RGET_THOLD__VALUE_width        30
#define MU_DCR__RGET_THOLD__VALUE_position     63
#define MU_DCR__RGET_THOLD__VALUE_get(x)       _BGQ_GET(30,63,x)
#define MU_DCR__RGET_THOLD__VALUE_set(v)       _BGQ_SET(30,63,v)
#define MU_DCR__RGET_THOLD__VALUE_insert(x,v)  _BGQ_INSERT(30,63,x,v)

  
// Register:
//   Name:        rmfifo_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: rmFIFOs n*16 to n*16+15 enable register
#define MU_DCR__RMFIFO_EN_offset  (0x0D35) // Offset of register in instance 
#define MU_DCR__RMFIFO_EN_range  (0x11) // Range of external address space
#define MU_DCR__RMFIFO_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that rmFIFO 0*16+i is enabled and can be used for receiving packets.
#define MU_DCR__RMFIFO_EN__VALUE_width        16
#define MU_DCR__RMFIFO_EN__VALUE_position     63
#define MU_DCR__RMFIFO_EN__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__RMFIFO_EN__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__RMFIFO_EN__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        rmfifo_thold_int_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: rmFIFOs n*16 to n*16+15 threshold interrupt enable register
#define MU_DCR__RMFIFO_THOLD_INT_EN_offset  (0x0D46) // Offset of register in instance 
#define MU_DCR__RMFIFO_THOLD_INT_EN_range  (0x11) // Range of external address space
#define MU_DCR__RMFIFO_THOLD_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that rmFIFO 0*16+i can raise an interrupt if free space in this rmFIFO drops below the reception FIFO threshold.
#define MU_DCR__RMFIFO_THOLD_INT_EN__VALUE_width        16
#define MU_DCR__RMFIFO_THOLD_INT_EN__VALUE_position     63
#define MU_DCR__RMFIFO_THOLD_INT_EN__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__RMFIFO_THOLD_INT_EN__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__RMFIFO_THOLD_INT_EN__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        rmfifo_packet_int_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: rmFIFOs n*16 to n*16+15 packet received interrupt enable register
#define MU_DCR__RMFIFO_PACKET_INT_EN_offset  (0x0D57) // Offset of register in instance 
#define MU_DCR__RMFIFO_PACKET_INT_EN_range  (0x11) // Range of external address space
#define MU_DCR__RMFIFO_PACKET_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that rmFIFO 0*16+i can raise an interrupt if a packet with interrupt bit set is received.
#define MU_DCR__RMFIFO_PACKET_INT_EN__VALUE_width        16
#define MU_DCR__RMFIFO_PACKET_INT_EN__VALUE_position     63
#define MU_DCR__RMFIFO_PACKET_INT_EN__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__RMFIFO_PACKET_INT_EN__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__RMFIFO_PACKET_INT_EN__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        rmfifo_system
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       000000000000FFFF
//   Description: Controls rmFIFOs n*16 to n*16+15 user program access
#define MU_DCR__RMFIFO_SYSTEM_offset  (0x0D68) // Offset of register in instance 
#define MU_DCR__RMFIFO_SYSTEM_range  (0x11) // Range of external address space
#define MU_DCR__RMFIFO_SYSTEM_reset  UNSIGNED64(0x000000000000FFFF)

//   Field:       value
//   Description: A 1 in bit i means that rmFIFO 0*16+i is a system FIFO, and cannot be used by a user program..
#define MU_DCR__RMFIFO_SYSTEM__VALUE_width        16
#define MU_DCR__RMFIFO_SYSTEM__VALUE_position     63
#define MU_DCR__RMFIFO_SYSTEM__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__RMFIFO_SYSTEM__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__RMFIFO_SYSTEM__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        rmu_thold
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This threshold register is used for all rmFIFOs to check if free space in that FIFO dropped below this value.  The threshold value for free space in this register is in 32B packet chunks.
#define MU_DCR__RMU_THOLD_offset  (0x0D79) // Offset of register in instance 
#define MU_DCR__RMU_THOLD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: The threshold value.
#define MU_DCR__RMU_THOLD__VALUE_width        31
#define MU_DCR__RMU_THOLD__VALUE_position     63
#define MU_DCR__RMU_THOLD__VALUE_get(x)       _BGQ_GET(31,63,x)
#define MU_DCR__RMU_THOLD__VALUE_set(v)       _BGQ_SET(31,63,v)
#define MU_DCR__RMU_THOLD__VALUE_insert(x,v)  _BGQ_INSERT(31,63,x,v)

  
// Register:
//   Name:        rputbase_system0
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFFFFFFFFFFFFFFF
//   Description: On a subgroup basis, controls user access to rput base addresses for groups 0-15
#define MU_DCR__RPUTBASE_SYSTEM0_offset  (0x0D7A) // Offset of register in instance 
#define MU_DCR__RPUTBASE_SYSTEM0_reset  UNSIGNED64(0xFFFFFFFFFFFFFFFF)

//   Field:       value
//   Description: A 1 in bit i means that the 8 rput base addresses for group int(i/4), subgroup i-int(i/4)*4 cannot be used by a user program.
#define MU_DCR__RPUTBASE_SYSTEM0__VALUE_width        64
#define MU_DCR__RPUTBASE_SYSTEM0__VALUE_position     63
#define MU_DCR__RPUTBASE_SYSTEM0__VALUE_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__RPUTBASE_SYSTEM0__VALUE_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__RPUTBASE_SYSTEM0__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        rputbase_system1
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       F000000000000000
//   Description: On a subgroup basis, controls user access to rput base addresses for group 16
#define MU_DCR__RPUTBASE_SYSTEM1_offset  (0x0D7B) // Offset of register in instance 
#define MU_DCR__RPUTBASE_SYSTEM1_reset  UNSIGNED64(0xF000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that the 8 rput base addresses for group 16, subgroup i cannot be used by a user program.
#define MU_DCR__RPUTBASE_SYSTEM1__VALUE_width        4
#define MU_DCR__RPUTBASE_SYSTEM1__VALUE_position     3
#define MU_DCR__RPUTBASE_SYSTEM1__VALUE_get(x)       _BGQ_GET(4,3,x)
#define MU_DCR__RPUTBASE_SYSTEM1__VALUE_set(v)       _BGQ_SET(4,3,v)
#define MU_DCR__RPUTBASE_SYSTEM1__VALUE_insert(x,v)  _BGQ_INSERT(4,3,x,v)

  
// Register:
//   Name:        sys_barrier
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Indicates that barrier class i is for system use.
#define MU_DCR__SYS_BARRIER_offset  (0x0D7C) // Offset of register in instance 
#define MU_DCR__SYS_BARRIER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: The system barrier mapping.
#define MU_DCR__SYS_BARRIER__VALUE_width        16
#define MU_DCR__SYS_BARRIER__VALUE_position     63
#define MU_DCR__SYS_BARRIER__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__SYS_BARRIER__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__SYS_BARRIER__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        barrier_int_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Classes 0-15 interrupt enable bits .
#define MU_DCR__BARRIER_INT_EN_offset  (0x0D7D) // Offset of register in instance 
#define MU_DCR__BARRIER_INT_EN_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class0
//   Description: Class 0 interrupt enable bits.
#define MU_DCR__BARRIER_INT_EN__CLASS0_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS0_position     3
#define MU_DCR__BARRIER_INT_EN__CLASS0_get(x)       _BGQ_GET(3,3,x)
#define MU_DCR__BARRIER_INT_EN__CLASS0_set(v)       _BGQ_SET(3,3,v)
#define MU_DCR__BARRIER_INT_EN__CLASS0_insert(x,v)  _BGQ_INSERT(3,3,x,v)

//   Field:       class1
//   Description: Class 1 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS1_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS1_position     7
#define MU_DCR__BARRIER_INT_EN__CLASS1_get(x)       _BGQ_GET(3,7,x)
#define MU_DCR__BARRIER_INT_EN__CLASS1_set(v)       _BGQ_SET(3,7,v)
#define MU_DCR__BARRIER_INT_EN__CLASS1_insert(x,v)  _BGQ_INSERT(3,7,x,v)

//   Field:       class2
//   Description: Class 2 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS2_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS2_position     11
#define MU_DCR__BARRIER_INT_EN__CLASS2_get(x)       _BGQ_GET(3,11,x)
#define MU_DCR__BARRIER_INT_EN__CLASS2_set(v)       _BGQ_SET(3,11,v)
#define MU_DCR__BARRIER_INT_EN__CLASS2_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       class3
//   Description: Class 3 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS3_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS3_position     15
#define MU_DCR__BARRIER_INT_EN__CLASS3_get(x)       _BGQ_GET(3,15,x)
#define MU_DCR__BARRIER_INT_EN__CLASS3_set(v)       _BGQ_SET(3,15,v)
#define MU_DCR__BARRIER_INT_EN__CLASS3_insert(x,v)  _BGQ_INSERT(3,15,x,v)

//   Field:       class4
//   Description: Class 4 interrupt enable bits.
#define MU_DCR__BARRIER_INT_EN__CLASS4_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS4_position     19
#define MU_DCR__BARRIER_INT_EN__CLASS4_get(x)       _BGQ_GET(3,19,x)
#define MU_DCR__BARRIER_INT_EN__CLASS4_set(v)       _BGQ_SET(3,19,v)
#define MU_DCR__BARRIER_INT_EN__CLASS4_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       class5
//   Description: Class 5 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS5_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS5_position     23
#define MU_DCR__BARRIER_INT_EN__CLASS5_get(x)       _BGQ_GET(3,23,x)
#define MU_DCR__BARRIER_INT_EN__CLASS5_set(v)       _BGQ_SET(3,23,v)
#define MU_DCR__BARRIER_INT_EN__CLASS5_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       class6
//   Description: Class 6 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS6_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS6_position     27
#define MU_DCR__BARRIER_INT_EN__CLASS6_get(x)       _BGQ_GET(3,27,x)
#define MU_DCR__BARRIER_INT_EN__CLASS6_set(v)       _BGQ_SET(3,27,v)
#define MU_DCR__BARRIER_INT_EN__CLASS6_insert(x,v)  _BGQ_INSERT(3,27,x,v)

//   Field:       class7
//   Description: Class 7 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS7_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS7_position     31
#define MU_DCR__BARRIER_INT_EN__CLASS7_get(x)       _BGQ_GET(3,31,x)
#define MU_DCR__BARRIER_INT_EN__CLASS7_set(v)       _BGQ_SET(3,31,v)
#define MU_DCR__BARRIER_INT_EN__CLASS7_insert(x,v)  _BGQ_INSERT(3,31,x,v)

//   Field:       class8
//   Description: Class 8 interrupt enable bits.
#define MU_DCR__BARRIER_INT_EN__CLASS8_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS8_position     35
#define MU_DCR__BARRIER_INT_EN__CLASS8_get(x)       _BGQ_GET(3,35,x)
#define MU_DCR__BARRIER_INT_EN__CLASS8_set(v)       _BGQ_SET(3,35,v)
#define MU_DCR__BARRIER_INT_EN__CLASS8_insert(x,v)  _BGQ_INSERT(3,35,x,v)

//   Field:       class9
//   Description: Class 9 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS9_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS9_position     39
#define MU_DCR__BARRIER_INT_EN__CLASS9_get(x)       _BGQ_GET(3,39,x)
#define MU_DCR__BARRIER_INT_EN__CLASS9_set(v)       _BGQ_SET(3,39,v)
#define MU_DCR__BARRIER_INT_EN__CLASS9_insert(x,v)  _BGQ_INSERT(3,39,x,v)

//   Field:       class10
//   Description: Class 10 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS10_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS10_position     43
#define MU_DCR__BARRIER_INT_EN__CLASS10_get(x)       _BGQ_GET(3,43,x)
#define MU_DCR__BARRIER_INT_EN__CLASS10_set(v)       _BGQ_SET(3,43,v)
#define MU_DCR__BARRIER_INT_EN__CLASS10_insert(x,v)  _BGQ_INSERT(3,43,x,v)

//   Field:       class11
//   Description: Class 11 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS11_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS11_position     47
#define MU_DCR__BARRIER_INT_EN__CLASS11_get(x)       _BGQ_GET(3,47,x)
#define MU_DCR__BARRIER_INT_EN__CLASS11_set(v)       _BGQ_SET(3,47,v)
#define MU_DCR__BARRIER_INT_EN__CLASS11_insert(x,v)  _BGQ_INSERT(3,47,x,v)

//   Field:       class12
//   Description: Class 12 interrupt enable bits.
#define MU_DCR__BARRIER_INT_EN__CLASS12_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS12_position     51
#define MU_DCR__BARRIER_INT_EN__CLASS12_get(x)       _BGQ_GET(3,51,x)
#define MU_DCR__BARRIER_INT_EN__CLASS12_set(v)       _BGQ_SET(3,51,v)
#define MU_DCR__BARRIER_INT_EN__CLASS12_insert(x,v)  _BGQ_INSERT(3,51,x,v)

//   Field:       class13
//   Description: Class 13 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS13_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS13_position     55
#define MU_DCR__BARRIER_INT_EN__CLASS13_get(x)       _BGQ_GET(3,55,x)
#define MU_DCR__BARRIER_INT_EN__CLASS13_set(v)       _BGQ_SET(3,55,v)
#define MU_DCR__BARRIER_INT_EN__CLASS13_insert(x,v)  _BGQ_INSERT(3,55,x,v)

//   Field:       class14
//   Description: Class 14 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS14_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS14_position     59
#define MU_DCR__BARRIER_INT_EN__CLASS14_get(x)       _BGQ_GET(3,59,x)
#define MU_DCR__BARRIER_INT_EN__CLASS14_set(v)       _BGQ_SET(3,59,v)
#define MU_DCR__BARRIER_INT_EN__CLASS14_insert(x,v)  _BGQ_INSERT(3,59,x,v)

//   Field:       class15
//   Description: Class 15 interrupt enable bits
#define MU_DCR__BARRIER_INT_EN__CLASS15_width        3
#define MU_DCR__BARRIER_INT_EN__CLASS15_position     63
#define MU_DCR__BARRIER_INT_EN__CLASS15_get(x)       _BGQ_GET(3,63,x)
#define MU_DCR__BARRIER_INT_EN__CLASS15_set(v)       _BGQ_SET(3,63,v)
#define MU_DCR__BARRIER_INT_EN__CLASS15_insert(x,v)  _BGQ_INSERT(3,63,x,v)

  
// Register:
//   Name:        min_usr_addr_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000FFFF00000
//   Description: User minimum range address n
#define MU_DCR__MIN_USR_ADDR_RANGE_offset  (0x0D80) // Offset of register in instance 
#define MU_DCR__MIN_USR_ADDR_RANGE_range  (0x14) // Range of external address space
#define MU_DCR__MIN_USR_ADDR_RANGE_reset  UNSIGNED64(0x0000000FFFF00000)

//   Field:       value
//   Description: Minimum valid address for user range check.
#define MU_DCR__MIN_USR_ADDR_RANGE__VALUE_width        16
#define MU_DCR__MIN_USR_ADDR_RANGE__VALUE_position     43
#define MU_DCR__MIN_USR_ADDR_RANGE__VALUE_get(x)       _BGQ_GET(16,43,x)
#define MU_DCR__MIN_USR_ADDR_RANGE__VALUE_set(v)       _BGQ_SET(16,43,v)
#define MU_DCR__MIN_USR_ADDR_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(16,43,x,v)

  
// Register:
//   Name:        max_usr_addr_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: User maximum range address n
#define MU_DCR__MAX_USR_ADDR_RANGE_offset  (0x0D94) // Offset of register in instance 
#define MU_DCR__MAX_USR_ADDR_RANGE_range  (0x14) // Range of external address space
#define MU_DCR__MAX_USR_ADDR_RANGE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Maximum valid address for user range check.
#define MU_DCR__MAX_USR_ADDR_RANGE__VALUE_width        16
#define MU_DCR__MAX_USR_ADDR_RANGE__VALUE_position     43
#define MU_DCR__MAX_USR_ADDR_RANGE__VALUE_get(x)       _BGQ_GET(16,43,x)
#define MU_DCR__MAX_USR_ADDR_RANGE__VALUE_set(v)       _BGQ_SET(16,43,v)
#define MU_DCR__MAX_USR_ADDR_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(16,43,x,v)

  
// Register:
//   Name:        min_sys_addr_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000FFFF00000
//   Description: System minimum range address n
#define MU_DCR__MIN_SYS_ADDR_RANGE_offset  (0x0DA8) // Offset of register in instance 
#define MU_DCR__MIN_SYS_ADDR_RANGE_range  (0x8) // Range of external address space
#define MU_DCR__MIN_SYS_ADDR_RANGE_reset  UNSIGNED64(0x0000000FFFF00000)

//   Field:       value
//   Description: Minimum valid address for system range check.
#define MU_DCR__MIN_SYS_ADDR_RANGE__VALUE_width        16
#define MU_DCR__MIN_SYS_ADDR_RANGE__VALUE_position     43
#define MU_DCR__MIN_SYS_ADDR_RANGE__VALUE_get(x)       _BGQ_GET(16,43,x)
#define MU_DCR__MIN_SYS_ADDR_RANGE__VALUE_set(v)       _BGQ_SET(16,43,v)
#define MU_DCR__MIN_SYS_ADDR_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(16,43,x,v)

  
// Register:
//   Name:        max_sys_addr_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: System maximum range address n
#define MU_DCR__MAX_SYS_ADDR_RANGE_offset  (0x0DB0) // Offset of register in instance 
#define MU_DCR__MAX_SYS_ADDR_RANGE_range  (0x8) // Range of external address space
#define MU_DCR__MAX_SYS_ADDR_RANGE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Maximum valid address for system range check.
#define MU_DCR__MAX_SYS_ADDR_RANGE__VALUE_width        16
#define MU_DCR__MAX_SYS_ADDR_RANGE__VALUE_position     43
#define MU_DCR__MAX_SYS_ADDR_RANGE__VALUE_get(x)       _BGQ_GET(16,43,x)
#define MU_DCR__MAX_SYS_ADDR_RANGE__VALUE_set(v)       _BGQ_SET(16,43,v)
#define MU_DCR__MAX_SYS_ADDR_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(16,43,x,v)

  
// Register:
//   Name:        usr_inj_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Specifies user injection-only address ranges
#define MU_DCR__USR_INJ_RANGE_offset  (0x0DB8) // Offset of register in instance 
#define MU_DCR__USR_INJ_RANGE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that i-th user range is an injection-only range, and cannot be used for reception. Similarly, a 0 in bit i means that i-th user range is an injection and reception range, and can be used for both.
#define MU_DCR__USR_INJ_RANGE__VALUE_width        20
#define MU_DCR__USR_INJ_RANGE__VALUE_position     63
#define MU_DCR__USR_INJ_RANGE__VALUE_get(x)       _BGQ_GET(20,63,x)
#define MU_DCR__USR_INJ_RANGE__VALUE_set(v)       _BGQ_SET(20,63,v)
#define MU_DCR__USR_INJ_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(20,63,x,v)

  
// Register:
//   Name:        sys_inj_range
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Specifies system injection-only address ranges
#define MU_DCR__SYS_INJ_RANGE_offset  (0x0DB9) // Offset of register in instance 
#define MU_DCR__SYS_INJ_RANGE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i means that i-th system range is an injection-only range, and cannot be used for reception. Similarly, a 0 in bit i means that i-th system range is an injection and reception range, and can be used for both.
#define MU_DCR__SYS_INJ_RANGE__VALUE_width        8
#define MU_DCR__SYS_INJ_RANGE__VALUE_position     63
#define MU_DCR__SYS_INJ_RANGE__VALUE_get(x)       _BGQ_GET(8,63,x)
#define MU_DCR__SYS_INJ_RANGE__VALUE_set(v)       _BGQ_SET(8,63,v)
#define MU_DCR__SYS_INJ_RANGE__VALUE_insert(x,v)  _BGQ_INSERT(8,63,x,v)

  
// Register:
//   Name:        ime_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Enable iME to start to process new packets.
#define MU_DCR__IME_ENABLE_offset  (0x0DC7) // Offset of register in instance 
#define MU_DCR__IME_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i enables i-th iME.
#define MU_DCR__IME_ENABLE__VALUE_width        16
#define MU_DCR__IME_ENABLE__VALUE_position     15
#define MU_DCR__IME_ENABLE__VALUE_get(x)       _BGQ_GET(16,15,x)
#define MU_DCR__IME_ENABLE__VALUE_set(v)       _BGQ_SET(16,15,v)
#define MU_DCR__IME_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(16,15,x,v)

  
// Register:
//   Name:        rme_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Enable rME to start to process new packets or go out from a wait state for full imFIFO/rmFIFO.
#define MU_DCR__RME_ENABLE_offset  (0x0DC8) // Offset of register in instance 
#define MU_DCR__RME_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 in bit i enables i-th rME.
#define MU_DCR__RME_ENABLE__VALUE_width        16
#define MU_DCR__RME_ENABLE__VALUE_position     15
#define MU_DCR__RME_ENABLE__VALUE_get(x)       _BGQ_GET(16,15,x)
#define MU_DCR__RME_ENABLE__VALUE_set(v)       _BGQ_SET(16,15,v)
#define MU_DCR__RME_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(16,15,x,v)

  
// Register:
//   Name:        rme_wait_full
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       1000000000000000
//   Description: Number of wait cycles in x16 clock when rME polls imFIFO/rmFIFO until they become not full.
#define MU_DCR__RME_WAIT_FULL_offset  (0x0DC9) // Offset of register in instance 
#define MU_DCR__RME_WAIT_FULL_reset  UNSIGNED64(0x1000000000000000)

//   Field:       value
//   Description: Number of x16 cycles to wait(0-255).
#define MU_DCR__RME_WAIT_FULL__VALUE_width        8
#define MU_DCR__RME_WAIT_FULL__VALUE_position     7
#define MU_DCR__RME_WAIT_FULL__VALUE_get(x)       _BGQ_GET(8,7,x)
#define MU_DCR__RME_WAIT_FULL__VALUE_set(v)       _BGQ_SET(8,7,v)
#define MU_DCR__RME_WAIT_FULL__VALUE_insert(x,v)  _BGQ_INSERT(8,7,x,v)

  
// Register:
//   Name:        rme_wait_tail
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0200000000000000
//   Description: Number of wait cycles in x16 clock when rME polls RCSRAM to commit rmFIFO tail pointer.
#define MU_DCR__RME_WAIT_TAIL_offset  (0x0DCA) // Offset of register in instance 
#define MU_DCR__RME_WAIT_TAIL_reset  UNSIGNED64(0x0200000000000000)

//   Field:       value
//   Description: Number of x16 cycles to wait(0-255).
#define MU_DCR__RME_WAIT_TAIL__VALUE_width        8
#define MU_DCR__RME_WAIT_TAIL__VALUE_position     7
#define MU_DCR__RME_WAIT_TAIL__VALUE_get(x)       _BGQ_GET(8,7,x)
#define MU_DCR__RME_WAIT_TAIL__VALUE_set(v)       _BGQ_SET(8,7,v)
#define MU_DCR__RME_WAIT_TAIL__VALUE_insert(x,v)  _BGQ_INSERT(8,7,x,v)

  
// Register:
//   Name:        gip_int_en_0_7
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: GEA interrupt enable for cores 0 to 7
#define MU_DCR__GIP_INT_EN_0_7_offset  (0x0DCB) // Offset of register in instance 
#define MU_DCR__GIP_INT_EN_0_7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: This register masks the 32 direct interrupts (4 per core) for cores 0 to 7 that are sent to the central GEA unit.  A 1 in bit i means that the i-th interrupt is enabled to raise an interrupt in the GEA.
#define MU_DCR__GIP_INT_EN_0_7__VALUE_width        32
#define MU_DCR__GIP_INT_EN_0_7__VALUE_position     63
#define MU_DCR__GIP_INT_EN_0_7__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__GIP_INT_EN_0_7__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__GIP_INT_EN_0_7__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        gip_int_en_8_15
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: GEA interrupt enable for cores 8 to 15
#define MU_DCR__GIP_INT_EN_8_15_offset  (0x0DCC) // Offset of register in instance 
#define MU_DCR__GIP_INT_EN_8_15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: This register masks the 32 direct interrupts (4 per core) for cores 8 to 15 that are sent to the central GEA unit.  A 1 in bit i means that the i-th interrupt is enabled to raise an interrupt in the GEA.
#define MU_DCR__GIP_INT_EN_8_15__VALUE_width        32
#define MU_DCR__GIP_INT_EN_8_15__VALUE_position     63
#define MU_DCR__GIP_INT_EN_8_15__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__GIP_INT_EN_8_15__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__GIP_INT_EN_8_15__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        gip_int_en_16
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: GEA interrupt enable for core 16
#define MU_DCR__GIP_INT_EN_16_offset  (0x0DCD) // Offset of register in instance 
#define MU_DCR__GIP_INT_EN_16_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: This register masks the 4 direct interrupts for core 16 that are sent to the central GEA unit.  A 1 in bit i means that the i-th interrupt is enabled to raise an interrupt in the GEA.
#define MU_DCR__GIP_INT_EN_16__VALUE_width        4
#define MU_DCR__GIP_INT_EN_16__VALUE_position     63
#define MU_DCR__GIP_INT_EN_16__VALUE_get(x)       _BGQ_GET(4,63,x)
#define MU_DCR__GIP_INT_EN_16__VALUE_set(v)       _BGQ_SET(4,63,v)
#define MU_DCR__GIP_INT_EN_16__VALUE_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        xm_gen_cnt_delay
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000006
//   Description: How long XM drives the MSOR signals while the request is traversing the switch
#define MU_DCR__XM_GEN_CNT_DELAY_offset  (0x0DCE) // Offset of register in instance 
#define MU_DCR__XM_GEN_CNT_DELAY_reset  UNSIGNED64(0x0000000000000006)

//   Field:       value
//   Description: Delay value.
#define MU_DCR__XM_GEN_CNT_DELAY__VALUE_width        4
#define MU_DCR__XM_GEN_CNT_DELAY__VALUE_position     63
#define MU_DCR__XM_GEN_CNT_DELAY__VALUE_get(x)       _BGQ_GET(4,63,x)
#define MU_DCR__XM_GEN_CNT_DELAY__VALUE_set(v)       _BGQ_SET(4,63,v)
#define MU_DCR__XM_GEN_CNT_DELAY__VALUE_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        reset
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFF000FFFFFFFFFF
//   Description: Resets for individual sub-units in Messaging Unit
#define MU_DCR__RESET_offset  (0x0DCF) // Offset of register in instance 
#define MU_DCR__RESET_reset  UNSIGNED64(0xFFF000FFFFFFFFFF)

//   Field:       dcrs_out
//   Description: Reset all DCR registers except this one
#define MU_DCR__RESET__DCRS_OUT_width        1
#define MU_DCR__RESET__DCRS_OUT_position     0
#define MU_DCR__RESET__DCRS_OUT_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RESET__DCRS_OUT_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RESET__DCRS_OUT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       xm0
//   Description: Reset master port 0
#define MU_DCR__RESET__XM0_width        1
#define MU_DCR__RESET__XM0_position     1
#define MU_DCR__RESET__XM0_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RESET__XM0_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RESET__XM0_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       xm1
//   Description: Reset master port 1
#define MU_DCR__RESET__XM1_width        1
#define MU_DCR__RESET__XM1_position     2
#define MU_DCR__RESET__XM1_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RESET__XM1_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RESET__XM1_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       xm2
//   Description: Reset master port 2
#define MU_DCR__RESET__XM2_width        1
#define MU_DCR__RESET__XM2_position     3
#define MU_DCR__RESET__XM2_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RESET__XM2_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RESET__XM2_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       xs
//   Description: Reset slave port
#define MU_DCR__RESET__XS_width        1
#define MU_DCR__RESET__XS_position     4
#define MU_DCR__RESET__XS_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RESET__XS_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RESET__XS_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       icsram
//   Description: Reset ICSRAM logic (not SRAM)
#define MU_DCR__RESET__ICSRAM_width        1
#define MU_DCR__RESET__ICSRAM_position     5
#define MU_DCR__RESET__ICSRAM_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RESET__ICSRAM_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RESET__ICSRAM_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       mcsram
//   Description: Reset MCSRAM logic(not SRAM) 
#define MU_DCR__RESET__MCSRAM_width        1
#define MU_DCR__RESET__MCSRAM_position     6
#define MU_DCR__RESET__MCSRAM_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RESET__MCSRAM_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RESET__MCSRAM_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       rcsram
//   Description: Reset RCSRAM logic (not SRAM)
#define MU_DCR__RESET__RCSRAM_width        1
#define MU_DCR__RESET__RCSRAM_position     7
#define MU_DCR__RESET__RCSRAM_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RESET__RCSRAM_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RESET__RCSRAM_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       rputsram
//   Description: Reset RPUTSRAM logic (not SRAM)
#define MU_DCR__RESET__RPUTSRAM_width        1
#define MU_DCR__RESET__RPUTSRAM_position     8
#define MU_DCR__RESET__RPUTSRAM_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RESET__RPUTSRAM_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RESET__RPUTSRAM_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       mmregs
//   Description: Reset MMREGS
#define MU_DCR__RESET__MMREGS_width        1
#define MU_DCR__RESET__MMREGS_position     9
#define MU_DCR__RESET__MMREGS_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RESET__MMREGS_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RESET__MMREGS_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       upc
//   Description: Reset UPC counters
#define MU_DCR__RESET__UPC_width        1
#define MU_DCR__RESET__UPC_position     10
#define MU_DCR__RESET__UPC_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RESET__UPC_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RESET__UPC_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       mu_top
//   Description: Reset MU top level
#define MU_DCR__RESET__MU_TOP_width        1
#define MU_DCR__RESET__MU_TOP_position     11
#define MU_DCR__RESET__MU_TOP_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RESET__MU_TOP_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RESET__MU_TOP_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       ecc_vec
//   Description: Reset ECC correctable error counters
#define MU_DCR__RESET__ECC_VEC_width        8
#define MU_DCR__RESET__ECC_VEC_position     31
#define MU_DCR__RESET__ECC_VEC_get(x)       _BGQ_GET(8,31,x)
#define MU_DCR__RESET__ECC_VEC_set(v)       _BGQ_SET(8,31,v)
#define MU_DCR__RESET__ECC_VEC_insert(x,v)  _BGQ_INSERT(8,31,x,v)

//   Field:       rme_vec
//   Description: Reset rMEs
#define MU_DCR__RESET__RME_VEC_width        16
#define MU_DCR__RESET__RME_VEC_position     47
#define MU_DCR__RESET__RME_VEC_get(x)       _BGQ_GET(16,47,x)
#define MU_DCR__RESET__RME_VEC_set(v)       _BGQ_SET(16,47,v)
#define MU_DCR__RESET__RME_VEC_insert(x,v)  _BGQ_INSERT(16,47,x,v)

//   Field:       ime_vec
//   Description: Reset iMEs
#define MU_DCR__RESET__IME_VEC_width        16
#define MU_DCR__RESET__IME_VEC_position     63
#define MU_DCR__RESET__IME_VEC_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__RESET__IME_VEC_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__RESET__IME_VEC_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        mod_halt_en
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFFFFFFFFFE00000
//   Description: Enable MU submodules to halt when fatal hardware error is detected.
#define MU_DCR__MOD_HALT_EN_offset  (0x0DD0) // Offset of register in instance 
#define MU_DCR__MOD_HALT_EN_reset  UNSIGNED64(0xFFFFFFFFFFE00000)

//   Field:       xm_vec
//   Description: A 1 in i-th bit enables i-th switch master to halt on error.
#define MU_DCR__MOD_HALT_EN__XM_VEC_width        3
#define MU_DCR__MOD_HALT_EN__XM_VEC_position     2
#define MU_DCR__MOD_HALT_EN__XM_VEC_get(x)       _BGQ_GET(3,2,x)
#define MU_DCR__MOD_HALT_EN__XM_VEC_set(v)       _BGQ_SET(3,2,v)
#define MU_DCR__MOD_HALT_EN__XM_VEC_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       xs
//   Description: A 1 enables switch slave to halt on error.
#define MU_DCR__MOD_HALT_EN__XS_width        1
#define MU_DCR__MOD_HALT_EN__XS_position     3
#define MU_DCR__MOD_HALT_EN__XS_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MOD_HALT_EN__XS_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MOD_HALT_EN__XS_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       icsram
//   Description: A 1 enables ICSRAM to halt on error.
#define MU_DCR__MOD_HALT_EN__ICSRAM_width        1
#define MU_DCR__MOD_HALT_EN__ICSRAM_position     4
#define MU_DCR__MOD_HALT_EN__ICSRAM_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MOD_HALT_EN__ICSRAM_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MOD_HALT_EN__ICSRAM_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       mcsram
//   Description: A 1 enables MCSRAM to halt on error.
#define MU_DCR__MOD_HALT_EN__MCSRAM_width        1
#define MU_DCR__MOD_HALT_EN__MCSRAM_position     5
#define MU_DCR__MOD_HALT_EN__MCSRAM_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MOD_HALT_EN__MCSRAM_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MOD_HALT_EN__MCSRAM_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       rcsram
//   Description: A 1 enables RCSRAM to halt on error.
#define MU_DCR__MOD_HALT_EN__RCSRAM_width        1
#define MU_DCR__MOD_HALT_EN__RCSRAM_position     6
#define MU_DCR__MOD_HALT_EN__RCSRAM_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MOD_HALT_EN__RCSRAM_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MOD_HALT_EN__RCSRAM_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       rputsram
//   Description: A 1 enables RPUTSRAM to halt on error.
#define MU_DCR__MOD_HALT_EN__RPUTSRAM_width        1
#define MU_DCR__MOD_HALT_EN__RPUTSRAM_position     7
#define MU_DCR__MOD_HALT_EN__RPUTSRAM_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MOD_HALT_EN__RPUTSRAM_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MOD_HALT_EN__RPUTSRAM_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       mmregs
//   Description: A 1 enables MMREG to halt on error.
#define MU_DCR__MOD_HALT_EN__MMREGS_width        1
#define MU_DCR__MOD_HALT_EN__MMREGS_position     8
#define MU_DCR__MOD_HALT_EN__MMREGS_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MOD_HALT_EN__MMREGS_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MOD_HALT_EN__MMREGS_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       arlog
//   Description: A 1 enables iME-to-imFIFO arbitration logic to halt on error.
#define MU_DCR__MOD_HALT_EN__ARLOG_width        1
#define MU_DCR__MOD_HALT_EN__ARLOG_position     9
#define MU_DCR__MOD_HALT_EN__ARLOG_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MOD_HALT_EN__ARLOG_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MOD_HALT_EN__ARLOG_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       icsram_select
//   Description: A 1 enables rME-to-ICSRAM request arbiter to halt on error.
#define MU_DCR__MOD_HALT_EN__ICSRAM_SELECT_width        1
#define MU_DCR__MOD_HALT_EN__ICSRAM_SELECT_position     10
#define MU_DCR__MOD_HALT_EN__ICSRAM_SELECT_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MOD_HALT_EN__ICSRAM_SELECT_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MOD_HALT_EN__ICSRAM_SELECT_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       rme_vec
//   Description: A 1 in i-th bit enables i-th rME to halt on error.
#define MU_DCR__MOD_HALT_EN__RME_VEC_width        16
#define MU_DCR__MOD_HALT_EN__RME_VEC_position     26
#define MU_DCR__MOD_HALT_EN__RME_VEC_get(x)       _BGQ_GET(16,26,x)
#define MU_DCR__MOD_HALT_EN__RME_VEC_set(v)       _BGQ_SET(16,26,v)
#define MU_DCR__MOD_HALT_EN__RME_VEC_insert(x,v)  _BGQ_INSERT(16,26,x,v)

//   Field:       ime_vec
//   Description: A 1 in i-th bit enables i-th iME to halt on error.
#define MU_DCR__MOD_HALT_EN__IME_VEC_width        16
#define MU_DCR__MOD_HALT_EN__IME_VEC_position     42
#define MU_DCR__MOD_HALT_EN__IME_VEC_get(x)       _BGQ_GET(16,42,x)
#define MU_DCR__MOD_HALT_EN__IME_VEC_set(v)       _BGQ_SET(16,42,v)
#define MU_DCR__MOD_HALT_EN__IME_VEC_insert(x,v)  _BGQ_INSERT(16,42,x,v)

  
// Register:
//   Name:        mod_halted
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Submodule halted due to fatal hardware error.
#define MU_DCR__MOD_HALTED_offset  (0x0DD1) // Offset of register in instance 
#define MU_DCR__MOD_HALTED_reset  UNSIGNED64(0x0000000000000000)

//   Field:       xm_vec
//   Description: A 1 in i-th bit indicate that i-th switch master stopped by error.
#define MU_DCR__MOD_HALTED__XM_VEC_width        3
#define MU_DCR__MOD_HALTED__XM_VEC_position     2
#define MU_DCR__MOD_HALTED__XM_VEC_get(x)       _BGQ_GET(3,2,x)
#define MU_DCR__MOD_HALTED__XM_VEC_set(v)       _BGQ_SET(3,2,v)
#define MU_DCR__MOD_HALTED__XM_VEC_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       xs
//   Description: A 1 indicates switch slave stopped by error.
#define MU_DCR__MOD_HALTED__XS_width        1
#define MU_DCR__MOD_HALTED__XS_position     3
#define MU_DCR__MOD_HALTED__XS_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MOD_HALTED__XS_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MOD_HALTED__XS_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       icsram
//   Description: A 1 indicates ICSRAM stopped by error.
#define MU_DCR__MOD_HALTED__ICSRAM_width        1
#define MU_DCR__MOD_HALTED__ICSRAM_position     4
#define MU_DCR__MOD_HALTED__ICSRAM_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MOD_HALTED__ICSRAM_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MOD_HALTED__ICSRAM_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       mcsram
//   Description: A 1 indicates MCSRAM stopped by error.
#define MU_DCR__MOD_HALTED__MCSRAM_width        1
#define MU_DCR__MOD_HALTED__MCSRAM_position     5
#define MU_DCR__MOD_HALTED__MCSRAM_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MOD_HALTED__MCSRAM_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MOD_HALTED__MCSRAM_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       rcsram
//   Description: A 1 indicates RCSRAM stopped by error.
#define MU_DCR__MOD_HALTED__RCSRAM_width        1
#define MU_DCR__MOD_HALTED__RCSRAM_position     6
#define MU_DCR__MOD_HALTED__RCSRAM_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MOD_HALTED__RCSRAM_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MOD_HALTED__RCSRAM_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       rputsram
//   Description: A 1 indicates RPUTSRAM stopped by error.
#define MU_DCR__MOD_HALTED__RPUTSRAM_width        1
#define MU_DCR__MOD_HALTED__RPUTSRAM_position     7
#define MU_DCR__MOD_HALTED__RPUTSRAM_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MOD_HALTED__RPUTSRAM_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MOD_HALTED__RPUTSRAM_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       mmregs
//   Description: A 1 indicates MMREG stopped by error.
#define MU_DCR__MOD_HALTED__MMREGS_width        1
#define MU_DCR__MOD_HALTED__MMREGS_position     8
#define MU_DCR__MOD_HALTED__MMREGS_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MOD_HALTED__MMREGS_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MOD_HALTED__MMREGS_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       arlog
//   Description: A 1 indicates iME-to-imFIFO arbitration logic stopped by error.
#define MU_DCR__MOD_HALTED__ARLOG_width        1
#define MU_DCR__MOD_HALTED__ARLOG_position     9
#define MU_DCR__MOD_HALTED__ARLOG_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MOD_HALTED__ARLOG_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MOD_HALTED__ARLOG_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       icsram_select
//   Description: A 1 indicates rME-to-ICSRAM arbiter stopped by error.
#define MU_DCR__MOD_HALTED__ICSRAM_SELECT_width        1
#define MU_DCR__MOD_HALTED__ICSRAM_SELECT_position     10
#define MU_DCR__MOD_HALTED__ICSRAM_SELECT_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MOD_HALTED__ICSRAM_SELECT_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MOD_HALTED__ICSRAM_SELECT_insert(x,v)  _BGQ_INSERT(1,10,x,v)

  
// Register:
//   Name:        arlog_scan_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Enables imFIFO-to-iME arbitration logic to periodically read, correct, and write back latches that hold FIFO maps and imFIFO ready flags.
#define MU_DCR__ARLOG_SCAN_ENABLE_offset  (0x0DD2) // Offset of register in instance 
#define MU_DCR__ARLOG_SCAN_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 enables scanning
#define MU_DCR__ARLOG_SCAN_ENABLE__VALUE_width        1
#define MU_DCR__ARLOG_SCAN_ENABLE__VALUE_position     0
#define MU_DCR__ARLOG_SCAN_ENABLE__VALUE_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__ARLOG_SCAN_ENABLE__VALUE_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__ARLOG_SCAN_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

  
// Register:
//   Name:        rme_hdump
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Controls rME function to dump bad header to fixed area in L2 for debugging
#define MU_DCR__RME_HDUMP_offset  (0x0DD3) // Offset of register in instance 
#define MU_DCR__RME_HDUMP_reset  UNSIGNED64(0x0000000000000000)

//   Field:       en
//   Description: A 1 enables rMEs to dump bad headers
#define MU_DCR__RME_HDUMP__EN_width        1
#define MU_DCR__RME_HDUMP__EN_position     35
#define MU_DCR__RME_HDUMP__EN_get(x)       _BGQ_GET(1,35,x)
#define MU_DCR__RME_HDUMP__EN_set(v)       _BGQ_SET(1,35,v)
#define MU_DCR__RME_HDUMP__EN_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       base
//   Description: Base address to dump header. i-th rME dumps header to (this address)*512+base+32*i in L2
#define MU_DCR__RME_HDUMP__BASE_width        27
#define MU_DCR__RME_HDUMP__BASE_position     63
#define MU_DCR__RME_HDUMP__BASE_get(x)       _BGQ_GET(27,63,x)
#define MU_DCR__RME_HDUMP__BASE_set(v)       _BGQ_SET(27,63,v)
#define MU_DCR__RME_HDUMP__BASE_insert(x,v)  _BGQ_INSERT(27,63,x,v)

  
// Register:
//   Name:        ecc_pass
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Disables read data correction of RCSRAM,MCSRAM, and R-put SRAMs. Can be used for SRAM testing using backdoor.
#define MU_DCR__ECC_PASS_offset  (0x0DD4) // Offset of register in instance 
#define MU_DCR__ECC_PASS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: A 1 disables ECC correction.
#define MU_DCR__ECC_PASS__VALUE_width        1
#define MU_DCR__ECC_PASS__VALUE_position     0
#define MU_DCR__ECC_PASS__VALUE_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__ECC_PASS__VALUE_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__ECC_PASS__VALUE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

  
// Register:
//   Name:        mcsram_bd_ctl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Message control SRAM backdoor control
#define MU_DCR__MCSRAM_BD_CTL_offset  (0x0DD5) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_CTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       index
//   Description: SRAM index to access.
#define MU_DCR__MCSRAM_BD_CTL__INDEX_width        10
#define MU_DCR__MCSRAM_BD_CTL__INDEX_position     60
#define MU_DCR__MCSRAM_BD_CTL__INDEX_get(x)       _BGQ_GET(10,60,x)
#define MU_DCR__MCSRAM_BD_CTL__INDEX_set(v)       _BGQ_SET(10,60,v)
#define MU_DCR__MCSRAM_BD_CTL__INDEX_insert(x,v)  _BGQ_INSERT(10,60,x,v)

//   Field:       en
//   Description: Enables backdoor access. This flag is ANDed with rd, nowrecc, and wr flags.
#define MU_DCR__MCSRAM_BD_CTL__EN_width        1
#define MU_DCR__MCSRAM_BD_CTL__EN_position     61
#define MU_DCR__MCSRAM_BD_CTL__EN_get(x)       _BGQ_GET(1,61,x)
#define MU_DCR__MCSRAM_BD_CTL__EN_set(v)       _BGQ_SET(1,61,v)
#define MU_DCR__MCSRAM_BD_CTL__EN_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       nowrecc
//   Description: Disables ECC generation logic and writes bd_wrecc register value to ECC field (effective only when en=1).
#define MU_DCR__MCSRAM_BD_CTL__NOWRECC_width        1
#define MU_DCR__MCSRAM_BD_CTL__NOWRECC_position     62
#define MU_DCR__MCSRAM_BD_CTL__NOWRECC_get(x)       _BGQ_GET(1,62,x)
#define MU_DCR__MCSRAM_BD_CTL__NOWRECC_set(v)       _BGQ_SET(1,62,v)
#define MU_DCR__MCSRAM_BD_CTL__NOWRECC_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       rd
//   Description: Reads SRAM when changed from 0 to 1 (effective only when en=1).
#define MU_DCR__MCSRAM_BD_CTL__RD_width        1
#define MU_DCR__MCSRAM_BD_CTL__RD_position     63
#define MU_DCR__MCSRAM_BD_CTL__RD_get(x)       _BGQ_GET(1,63,x)
#define MU_DCR__MCSRAM_BD_CTL__RD_set(v)       _BGQ_SET(1,63,v)
#define MU_DCR__MCSRAM_BD_CTL__RD_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        mcsram_bd_wdata
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Message control SRAM backdoor write data & control.
#define MU_DCR__MCSRAM_BD_WDATA_offset  (0x0DD6) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_WDATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Data to be written to backdoor data latch.
#define MU_DCR__MCSRAM_BD_WDATA__VALUE_width        56
#define MU_DCR__MCSRAM_BD_WDATA__VALUE_position     55
#define MU_DCR__MCSRAM_BD_WDATA__VALUE_get(x)       _BGQ_GET(56,55,x)
#define MU_DCR__MCSRAM_BD_WDATA__VALUE_set(v)       _BGQ_SET(56,55,v)
#define MU_DCR__MCSRAM_BD_WDATA__VALUE_insert(x,v)  _BGQ_INSERT(56,55,x,v)

//   Field:       id
//   Description: Selects to which portion of data latch store the data 0:55.
#define MU_DCR__MCSRAM_BD_WDATA__ID_width        3
#define MU_DCR__MCSRAM_BD_WDATA__ID_position     61
#define MU_DCR__MCSRAM_BD_WDATA__ID_get(x)       _BGQ_GET(3,61,x)
#define MU_DCR__MCSRAM_BD_WDATA__ID_set(v)       _BGQ_SET(3,61,v)
#define MU_DCR__MCSRAM_BD_WDATA__ID_insert(x,v)  _BGQ_INSERT(3,61,x,v)

//   Field:       wr
//   Description: Data latch value will be written to MCSRAM when this bit changed from 0 to 1.
#define MU_DCR__MCSRAM_BD_WDATA__WR_width        1
#define MU_DCR__MCSRAM_BD_WDATA__WR_position     62
#define MU_DCR__MCSRAM_BD_WDATA__WR_get(x)       _BGQ_GET(1,62,x)
#define MU_DCR__MCSRAM_BD_WDATA__WR_set(v)       _BGQ_SET(1,62,v)
#define MU_DCR__MCSRAM_BD_WDATA__WR_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       latch_data
//   Description: Data 0:55 will be latched when this bit is toggled.
#define MU_DCR__MCSRAM_BD_WDATA__LATCH_DATA_width        1
#define MU_DCR__MCSRAM_BD_WDATA__LATCH_DATA_position     63
#define MU_DCR__MCSRAM_BD_WDATA__LATCH_DATA_get(x)       _BGQ_GET(1,63,x)
#define MU_DCR__MCSRAM_BD_WDATA__LATCH_DATA_set(v)       _BGQ_SET(1,63,v)
#define MU_DCR__MCSRAM_BD_WDATA__LATCH_DATA_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 0
#define MU_DCR__MCSRAM_BD_RDATA0_offset  (0x0DDD) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: First 64bits in bank0 111bits read data.
#define MU_DCR__MCSRAM_BD_RDATA0__VALUE_width        64
#define MU_DCR__MCSRAM_BD_RDATA0__VALUE_position     63
#define MU_DCR__MCSRAM_BD_RDATA0__VALUE_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__MCSRAM_BD_RDATA0__VALUE_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__MCSRAM_BD_RDATA0__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 1
#define MU_DCR__MCSRAM_BD_RDATA1_offset  (0x0DDE) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Last 47bits in bank0 111bits read data.
#define MU_DCR__MCSRAM_BD_RDATA1__VALUE_width        47
#define MU_DCR__MCSRAM_BD_RDATA1__VALUE_position     46
#define MU_DCR__MCSRAM_BD_RDATA1__VALUE_get(x)       _BGQ_GET(47,46,x)
#define MU_DCR__MCSRAM_BD_RDATA1__VALUE_set(v)       _BGQ_SET(47,46,v)
#define MU_DCR__MCSRAM_BD_RDATA1__VALUE_insert(x,v)  _BGQ_INSERT(47,46,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata2
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 2
#define MU_DCR__MCSRAM_BD_RDATA2_offset  (0x0DDF) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: First 64bits in bank1 221bits read data.
#define MU_DCR__MCSRAM_BD_RDATA2__VALUE_width        64
#define MU_DCR__MCSRAM_BD_RDATA2__VALUE_position     63
#define MU_DCR__MCSRAM_BD_RDATA2__VALUE_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__MCSRAM_BD_RDATA2__VALUE_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__MCSRAM_BD_RDATA2__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata3
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 3
#define MU_DCR__MCSRAM_BD_RDATA3_offset  (0x0DE0) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Second 64bits in bank1 221bits read data.
#define MU_DCR__MCSRAM_BD_RDATA3__VALUE_width        64
#define MU_DCR__MCSRAM_BD_RDATA3__VALUE_position     63
#define MU_DCR__MCSRAM_BD_RDATA3__VALUE_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__MCSRAM_BD_RDATA3__VALUE_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__MCSRAM_BD_RDATA3__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata4
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 4
#define MU_DCR__MCSRAM_BD_RDATA4_offset  (0x0DE1) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Third 64bits in bank1 221bits read data.
#define MU_DCR__MCSRAM_BD_RDATA4__VALUE_width        64
#define MU_DCR__MCSRAM_BD_RDATA4__VALUE_position     63
#define MU_DCR__MCSRAM_BD_RDATA4__VALUE_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__MCSRAM_BD_RDATA4__VALUE_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__MCSRAM_BD_RDATA4__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        mcsram_bd_rdata5
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read data 5
#define MU_DCR__MCSRAM_BD_RDATA5_offset  (0x0DE2) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDATA5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Last 29bits in bank1 221bits read data.
#define MU_DCR__MCSRAM_BD_RDATA5__VALUE_width        29
#define MU_DCR__MCSRAM_BD_RDATA5__VALUE_position     28
#define MU_DCR__MCSRAM_BD_RDATA5__VALUE_get(x)       _BGQ_GET(29,28,x)
#define MU_DCR__MCSRAM_BD_RDATA5__VALUE_set(v)       _BGQ_SET(29,28,v)
#define MU_DCR__MCSRAM_BD_RDATA5__VALUE_insert(x,v)  _BGQ_INSERT(29,28,x,v)

  
// Register:
//   Name:        mcsram_bd_rdecc
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Message control SRAM read ECC
#define MU_DCR__MCSRAM_BD_RDECC_offset  (0x0DE3) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_RDECC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ecc5
//   Description: 7bit ECC that covers 32bit data bd_rdata5 & '000'.
#define MU_DCR__MCSRAM_BD_RDECC__ECC5_width        7
#define MU_DCR__MCSRAM_BD_RDECC__ECC5_position     23
#define MU_DCR__MCSRAM_BD_RDECC__ECC5_get(x)       _BGQ_GET(7,23,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC5_set(v)       _BGQ_SET(7,23,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC5_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       ecc4
//   Description: 8bit ECC that covers 64bit data bd_rdata4.
#define MU_DCR__MCSRAM_BD_RDECC__ECC4_width        8
#define MU_DCR__MCSRAM_BD_RDECC__ECC4_position     31
#define MU_DCR__MCSRAM_BD_RDECC__ECC4_get(x)       _BGQ_GET(8,31,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC4_set(v)       _BGQ_SET(8,31,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC4_insert(x,v)  _BGQ_INSERT(8,31,x,v)

//   Field:       ecc3
//   Description: 8bit ECC that covers 64bit data bd_rdata3.
#define MU_DCR__MCSRAM_BD_RDECC__ECC3_width        8
#define MU_DCR__MCSRAM_BD_RDECC__ECC3_position     39
#define MU_DCR__MCSRAM_BD_RDECC__ECC3_get(x)       _BGQ_GET(8,39,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC3_set(v)       _BGQ_SET(8,39,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC3_insert(x,v)  _BGQ_INSERT(8,39,x,v)

//   Field:       ecc2
//   Description: 8bit ECC that covers 64bit data bd_rdata2.
#define MU_DCR__MCSRAM_BD_RDECC__ECC2_width        8
#define MU_DCR__MCSRAM_BD_RDECC__ECC2_position     47
#define MU_DCR__MCSRAM_BD_RDECC__ECC2_get(x)       _BGQ_GET(8,47,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC2_set(v)       _BGQ_SET(8,47,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC2_insert(x,v)  _BGQ_INSERT(8,47,x,v)

//   Field:       ecc1
//   Description: 8bit ECC that covers 64bit data bd_rdata1 & '00000000000000000'.
#define MU_DCR__MCSRAM_BD_RDECC__ECC1_width        8
#define MU_DCR__MCSRAM_BD_RDECC__ECC1_position     55
#define MU_DCR__MCSRAM_BD_RDECC__ECC1_get(x)       _BGQ_GET(8,55,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC1_set(v)       _BGQ_SET(8,55,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC1_insert(x,v)  _BGQ_INSERT(8,55,x,v)

//   Field:       ecc0
//   Description: 8bit ECC that covers 64bit data bd_rdata0.
#define MU_DCR__MCSRAM_BD_RDECC__ECC0_width        8
#define MU_DCR__MCSRAM_BD_RDECC__ECC0_position     63
#define MU_DCR__MCSRAM_BD_RDECC__ECC0_get(x)       _BGQ_GET(8,63,x)
#define MU_DCR__MCSRAM_BD_RDECC__ECC0_set(v)       _BGQ_SET(8,63,v)
#define MU_DCR__MCSRAM_BD_RDECC__ECC0_insert(x,v)  _BGQ_INSERT(8,63,x,v)

  
// Register:
//   Name:        mcsram_bd_fetching_desc
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Descriptor fetch has been issued for an imFIFO and, if it has returned data, message control SRAM may be processing it.
#define MU_DCR__MCSRAM_BD_FETCHING_DESC_offset  (0x0DE4) // Offset of register in instance 
#define MU_DCR__MCSRAM_BD_FETCHING_DESC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sys
//   Description: Indicates at least one system imFIFO has outstanding descriptor fetches.
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__SYS_width        1
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__SYS_position     62
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__SYS_get(x)       _BGQ_GET(1,62,x)
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__SYS_set(v)       _BGQ_SET(1,62,v)
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__SYS_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       usr
//   Description: Indicates at least one user imFIFO has outstanding descriptor fetches.
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_width        1
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_position     63
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_get(x)       _BGQ_GET(1,63,x)
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_set(v)       _BGQ_SET(1,63,v)
#define MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        rcsram_bd_ctl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Reception control SRAM backdoor control
#define MU_DCR__RCSRAM_BD_CTL_offset  (0x0DE5) // Offset of register in instance 
#define MU_DCR__RCSRAM_BD_CTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       id
//   Description: Tail id to write to SRAM when wid_en flag is set.
#define MU_DCR__RCSRAM_BD_CTL__ID_width        5
#define MU_DCR__RCSRAM_BD_CTL__ID_position     51
#define MU_DCR__RCSRAM_BD_CTL__ID_get(x)       _BGQ_GET(5,51,x)
#define MU_DCR__RCSRAM_BD_CTL__ID_set(v)       _BGQ_SET(5,51,v)
#define MU_DCR__RCSRAM_BD_CTL__ID_insert(x,v)  _BGQ_INSERT(5,51,x,v)

//   Field:       ecc
//   Description: ECC value to write to SRAM when wecc_en flag is set.Only 7 LSBs are used for start, size, and head fields, because they are protected by 32bit ECC.
#define MU_DCR__RCSRAM_BD_CTL__ECC_width        8
#define MU_DCR__RCSRAM_BD_CTL__ECC_position     59
#define MU_DCR__RCSRAM_BD_CTL__ECC_get(x)       _BGQ_GET(8,59,x)
#define MU_DCR__RCSRAM_BD_CTL__ECC_set(v)       _BGQ_SET(8,59,v)
#define MU_DCR__RCSRAM_BD_CTL__ECC_insert(x,v)  _BGQ_INSERT(8,59,x,v)

//   Field:       tail_sel
//   Description: When set, advance tail is read out instead of commit tail for slave access.
#define MU_DCR__RCSRAM_BD_CTL__TAIL_SEL_width        1
#define MU_DCR__RCSRAM_BD_CTL__TAIL_SEL_position     60
#define MU_DCR__RCSRAM_BD_CTL__TAIL_SEL_get(x)       _BGQ_GET(1,60,x)
#define MU_DCR__RCSRAM_BD_CTL__TAIL_SEL_set(v)       _BGQ_SET(1,60,v)
#define MU_DCR__RCSRAM_BD_CTL__TAIL_SEL_insert(x,v)  _BGQ_INSERT(1,60,x,v)

//   Field:       rd_en
//   Description: When set, each time SRAM is read by slave, ECC value is saved to rcsram_bd_rdata register. Advance/commit tail id is also saved when they are read.
#define MU_DCR__RCSRAM_BD_CTL__RD_EN_width        1
#define MU_DCR__RCSRAM_BD_CTL__RD_EN_position     61
#define MU_DCR__RCSRAM_BD_CTL__RD_EN_get(x)       _BGQ_GET(1,61,x)
#define MU_DCR__RCSRAM_BD_CTL__RD_EN_set(v)       _BGQ_SET(1,61,v)
#define MU_DCR__RCSRAM_BD_CTL__RD_EN_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       wid_en
//   Description: When set, tail id generation logic is disabled and id field of this register is attached to advance/commit tail pointer when they are written by slave port.
#define MU_DCR__RCSRAM_BD_CTL__WID_EN_width        1
#define MU_DCR__RCSRAM_BD_CTL__WID_EN_position     62
#define MU_DCR__RCSRAM_BD_CTL__WID_EN_get(x)       _BGQ_GET(1,62,x)
#define MU_DCR__RCSRAM_BD_CTL__WID_EN_set(v)       _BGQ_SET(1,62,v)
#define MU_DCR__RCSRAM_BD_CTL__WID_EN_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       wecc_en
//   Description: When set, hardware ECC generator is disabled and ecc field of this register is attached to SRAM write data when it is written by slave port.
#define MU_DCR__RCSRAM_BD_CTL__WECC_EN_width        1
#define MU_DCR__RCSRAM_BD_CTL__WECC_EN_position     63
#define MU_DCR__RCSRAM_BD_CTL__WECC_EN_get(x)       _BGQ_GET(1,63,x)
#define MU_DCR__RCSRAM_BD_CTL__WECC_EN_set(v)       _BGQ_SET(1,63,v)
#define MU_DCR__RCSRAM_BD_CTL__WECC_EN_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        rcsram_bd_rdata
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Reception control SRAM ECC and tail ID read data.
#define MU_DCR__RCSRAM_BD_RDATA_offset  (0x0DE6) // Offset of register in instance 
#define MU_DCR__RCSRAM_BD_RDATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       id
//   Description: Tail id read out and saved by the last slave access when rcsram_bd_ctl.rd_en = 1.
#define MU_DCR__RCSRAM_BD_RDATA__ID_width        5
#define MU_DCR__RCSRAM_BD_RDATA__ID_position     55
#define MU_DCR__RCSRAM_BD_RDATA__ID_get(x)       _BGQ_GET(5,55,x)
#define MU_DCR__RCSRAM_BD_RDATA__ID_set(v)       _BGQ_SET(5,55,v)
#define MU_DCR__RCSRAM_BD_RDATA__ID_insert(x,v)  _BGQ_INSERT(5,55,x,v)

//   Field:       ecc
//   Description: ECC value read out and saved by the last slave access when rcsram_bd_ctl.rd_en = 1. For start, size, and head fields, 32bit ECC is used and thus only 7 LSBs are valid.
#define MU_DCR__RCSRAM_BD_RDATA__ECC_width        8
#define MU_DCR__RCSRAM_BD_RDATA__ECC_position     63
#define MU_DCR__RCSRAM_BD_RDATA__ECC_get(x)       _BGQ_GET(8,63,x)
#define MU_DCR__RCSRAM_BD_RDATA__ECC_set(v)       _BGQ_SET(8,63,v)
#define MU_DCR__RCSRAM_BD_RDATA__ECC_insert(x,v)  _BGQ_INSERT(8,63,x,v)

  
// Register:
//   Name:        rput_backdoor
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Rput SRAM backdoor control
#define MU_DCR__RPUT_BACKDOOR_offset  (0x0DE7) // Offset of register in instance 
#define MU_DCR__RPUT_BACKDOOR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       en
//   Description: Enables Rput SRAM backdoor access logic.
#define MU_DCR__RPUT_BACKDOOR__EN_width        1
#define MU_DCR__RPUT_BACKDOOR__EN_position     22
#define MU_DCR__RPUT_BACKDOOR__EN_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__RPUT_BACKDOOR__EN_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__RPUT_BACKDOOR__EN_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       nowrecc
//   Description: Disables ECC generation for write data (effective only when en=1).
#define MU_DCR__RPUT_BACKDOOR__NOWRECC_width        1
#define MU_DCR__RPUT_BACKDOOR__NOWRECC_position     23
#define MU_DCR__RPUT_BACKDOOR__NOWRECC_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__RPUT_BACKDOOR__NOWRECC_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__RPUT_BACKDOOR__NOWRECC_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       wrecc
//   Description: Data to be written to SRAM ECC bits when ECC generation is disabled.
#define MU_DCR__RPUT_BACKDOOR__WRECC_width        8
#define MU_DCR__RPUT_BACKDOOR__WRECC_position     31
#define MU_DCR__RPUT_BACKDOOR__WRECC_get(x)       _BGQ_GET(8,31,x)
#define MU_DCR__RPUT_BACKDOOR__WRECC_set(v)       _BGQ_SET(8,31,v)
#define MU_DCR__RPUT_BACKDOOR__WRECC_insert(x,v)  _BGQ_INSERT(8,31,x,v)

  
// Register:
//   Name:        rput_backdoor_rdecc
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Rput SRAM backdoor ECC read value
#define MU_DCR__RPUT_BACKDOOR_RDECC_offset  (0x0DE8) // Offset of register in instance 
#define MU_DCR__RPUT_BACKDOOR_RDECC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: last ECC data read out from Rput SRAM by crossbar slave port.
#define MU_DCR__RPUT_BACKDOOR_RDECC__VALUE_width        8
#define MU_DCR__RPUT_BACKDOOR_RDECC__VALUE_position     31
#define MU_DCR__RPUT_BACKDOOR_RDECC__VALUE_get(x)       _BGQ_GET(8,31,x)
#define MU_DCR__RPUT_BACKDOOR_RDECC__VALUE_set(v)       _BGQ_SET(8,31,v)
#define MU_DCR__RPUT_BACKDOOR_RDECC__VALUE_insert(x,v)  _BGQ_INSERT(8,31,x,v)

  
// Register:
//   Name:        ime_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Backdoor access for iME state.
#define MU_DCR__IME_STATE_offset  (0x0DE9) // Offset of register in instance 
#define MU_DCR__IME_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Each iME has 3 state bits. bit 16:18 is iME0 state, bit 19:21 is iME1 state, ..., bit 61:63 is iME15 state.
#define MU_DCR__IME_STATE__VALUE_width        48
#define MU_DCR__IME_STATE__VALUE_position     63
#define MU_DCR__IME_STATE__VALUE_get(x)       _BGQ_GET(48,63,x)
#define MU_DCR__IME_STATE__VALUE_set(v)       _BGQ_SET(48,63,v)
#define MU_DCR__IME_STATE__VALUE_insert(x,v)  _BGQ_INSERT(48,63,x,v)

  
// Register:
//   Name:        rme_state0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Backdoor access for rME0-7 state.
#define MU_DCR__RME_STATE0_offset  (0x0DEA) // Offset of register in instance 
#define MU_DCR__RME_STATE0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Each rME has 5 state bits. bit24:28 is rME0 state, bit 29:33 is rME1 state, ..., bit 59:63 is rME7 state.
#define MU_DCR__RME_STATE0__VALUE_width        40
#define MU_DCR__RME_STATE0__VALUE_position     63
#define MU_DCR__RME_STATE0__VALUE_get(x)       _BGQ_GET(40,63,x)
#define MU_DCR__RME_STATE0__VALUE_set(v)       _BGQ_SET(40,63,v)
#define MU_DCR__RME_STATE0__VALUE_insert(x,v)  _BGQ_INSERT(40,63,x,v)

  
// Register:
//   Name:        rme_state1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Backdoor access for rME8-15 state.
#define MU_DCR__RME_STATE1_offset  (0x0DEB) // Offset of register in instance 
#define MU_DCR__RME_STATE1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Each rME has 5 state bits. bit24:28 is rME8 state, bit 29:33 is rME9 state, ..., bit 59:63 is rME15 state.
#define MU_DCR__RME_STATE1__VALUE_width        40
#define MU_DCR__RME_STATE1__VALUE_position     63
#define MU_DCR__RME_STATE1__VALUE_get(x)       _BGQ_GET(40,63,x)
#define MU_DCR__RME_STATE1__VALUE_set(v)       _BGQ_SET(40,63,v)
#define MU_DCR__RME_STATE1__VALUE_insert(x,v)  _BGQ_INSERT(40,63,x,v)

  
// Register:
//   Name:        inj_ndfifo_avail
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Indicates whether each injection network FIFO has free space to store next packet.
#define MU_DCR__INJ_NDFIFO_AVAIL_offset  (0x0DEC) // Offset of register in instance 
#define MU_DCR__INJ_NDFIFO_AVAIL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: i-th leftmost bit indicates that i-th injection network FIFO has free space.
#define MU_DCR__INJ_NDFIFO_AVAIL__VALUE_width        16
#define MU_DCR__INJ_NDFIFO_AVAIL__VALUE_position     63
#define MU_DCR__INJ_NDFIFO_AVAIL__VALUE_get(x)       _BGQ_GET(16,63,x)
#define MU_DCR__INJ_NDFIFO_AVAIL__VALUE_set(v)       _BGQ_SET(16,63,v)
#define MU_DCR__INJ_NDFIFO_AVAIL__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        ecc_int_thresh
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: ECC correctable error counter threshould to fire interrupt
#define MU_DCR__ECC_INT_THRESH_offset  (0x0DED) // Offset of register in instance 
#define MU_DCR__ECC_INT_THRESH_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       value
//   Description: Interrupt will be issued when a ECC counter value is greater than this register.
#define MU_DCR__ECC_INT_THRESH__VALUE_width        32
#define MU_DCR__ECC_INT_THRESH__VALUE_position     63
#define MU_DCR__ECC_INT_THRESH__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__ECC_INT_THRESH__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__ECC_INT_THRESH__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        arlog_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in iME-to-imFIFO arbitration logic
#define MU_DCR__ARLOG_ECC_COUNT_offset  (0x0DEE) // Offset of register in instance 
#define MU_DCR__ARLOG_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__ARLOG_ECC_COUNT__VALUE_width        32
#define MU_DCR__ARLOG_ECC_COUNT__VALUE_position     63
#define MU_DCR__ARLOG_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__ARLOG_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__ARLOG_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        icsram_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in ICSRAM
#define MU_DCR__ICSRAM_ECC_COUNT_offset  (0x0DEF) // Offset of register in instance 
#define MU_DCR__ICSRAM_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__ICSRAM_ECC_COUNT__VALUE_width        32
#define MU_DCR__ICSRAM_ECC_COUNT__VALUE_position     63
#define MU_DCR__ICSRAM_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__ICSRAM_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__ICSRAM_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mcsram_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in MCSRAM
#define MU_DCR__MCSRAM_ECC_COUNT_offset  (0x0DF0) // Offset of register in instance 
#define MU_DCR__MCSRAM_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__MCSRAM_ECC_COUNT__VALUE_width        32
#define MU_DCR__MCSRAM_ECC_COUNT__VALUE_position     63
#define MU_DCR__MCSRAM_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__MCSRAM_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__MCSRAM_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        rcsram_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in RCSRAM
#define MU_DCR__RCSRAM_ECC_COUNT_offset  (0x0DF1) // Offset of register in instance 
#define MU_DCR__RCSRAM_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__RCSRAM_ECC_COUNT__VALUE_width        32
#define MU_DCR__RCSRAM_ECC_COUNT__VALUE_position     63
#define MU_DCR__RCSRAM_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__RCSRAM_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__RCSRAM_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        rput_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in RPUT SRAM
#define MU_DCR__RPUT_ECC_COUNT_offset  (0x0DF2) // Offset of register in instance 
#define MU_DCR__RPUT_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__RPUT_ECC_COUNT__VALUE_width        32
#define MU_DCR__RPUT_ECC_COUNT__VALUE_position     63
#define MU_DCR__RPUT_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__RPUT_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__RPUT_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        mmregs_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in MMREGS
#define MU_DCR__MMREGS_ECC_COUNT_offset  (0x0DF3) // Offset of register in instance 
#define MU_DCR__MMREGS_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__MMREGS_ECC_COUNT__VALUE_width        32
#define MU_DCR__MMREGS_ECC_COUNT__VALUE_position     63
#define MU_DCR__MMREGS_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__MMREGS_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__MMREGS_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        xs_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in XS
#define MU_DCR__XS_ECC_COUNT_offset  (0x0DF4) // Offset of register in instance 
#define MU_DCR__XS_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__XS_ECC_COUNT__VALUE_width        32
#define MU_DCR__XS_ECC_COUNT__VALUE_position     63
#define MU_DCR__XS_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__XS_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__XS_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        rme_ecc_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Total number of ECC correctable errors detected in rMEs
#define MU_DCR__RME_ECC_COUNT_offset  (0x0DF5) // Offset of register in instance 
#define MU_DCR__RME_ECC_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Number of correctable ECC errors.
#define MU_DCR__RME_ECC_COUNT__VALUE_width        32
#define MU_DCR__RME_ECC_COUNT__VALUE_position     63
#define MU_DCR__RME_ECC_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define MU_DCR__RME_ECC_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define MU_DCR__RME_ECC_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        spare_dcr
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Spare DCR/Switches for DD1 compatibility
#define MU_DCR__SPARE_DCR_offset  (0x0DF6) // Offset of register in instance 
#define MU_DCR__SPARE_DCR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: (In DD1:) Unused/spare 64b DCR. (In DD2:) The 6 LSBs work as switches for compatibility to DD1. A 1 in bit 58 to 63 disables MU fixes for issue 1101, 1113, 1117, 1152, 1179, and 1243, respectively. Disabling these fixes, MU hardware will be compatible to DD1, except for some DD2-specific DCR registers, including this register. By default these 6 LSBs are all 0 and thus these fixes are enabled. The other bits are unused/spare.
#define MU_DCR__SPARE_DCR__RESERVED_width        64
#define MU_DCR__SPARE_DCR__RESERVED_position     63
#define MU_DCR__SPARE_DCR__RESERVED_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__SPARE_DCR__RESERVED_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__SPARE_DCR__RESERVED_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        xm0_err_info
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Holds info that might help debug master port errors.
#define MU_DCR__XM0_ERR_INFO_offset  (0x0E12) // Offset of register in instance 
#define MU_DCR__XM0_ERR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sys
//   Description: System request violated range check
#define MU_DCR__XM0_ERR_INFO__SYS_width        1
#define MU_DCR__XM0_ERR_INFO__SYS_position     26
#define MU_DCR__XM0_ERR_INFO__SYS_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__XM0_ERR_INFO__SYS_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__XM0_ERR_INFO__SYS_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       addr
//   Description: Address that violated range check
#define MU_DCR__XM0_ERR_INFO__ADDR_width        37
#define MU_DCR__XM0_ERR_INFO__ADDR_position     63
#define MU_DCR__XM0_ERR_INFO__ADDR_get(x)       _BGQ_GET(37,63,x)
#define MU_DCR__XM0_ERR_INFO__ADDR_set(v)       _BGQ_SET(37,63,v)
#define MU_DCR__XM0_ERR_INFO__ADDR_insert(x,v)  _BGQ_INSERT(37,63,x,v)

  
// Register:
//   Name:        xm1_err_info
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Holds info that might help debug master port errors.
#define MU_DCR__XM1_ERR_INFO_offset  (0x0E1B) // Offset of register in instance 
#define MU_DCR__XM1_ERR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sys
//   Description: System request violated range check
#define MU_DCR__XM1_ERR_INFO__SYS_width        1
#define MU_DCR__XM1_ERR_INFO__SYS_position     26
#define MU_DCR__XM1_ERR_INFO__SYS_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__XM1_ERR_INFO__SYS_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__XM1_ERR_INFO__SYS_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       addr
//   Description: Address that violated range check
#define MU_DCR__XM1_ERR_INFO__ADDR_width        37
#define MU_DCR__XM1_ERR_INFO__ADDR_position     63
#define MU_DCR__XM1_ERR_INFO__ADDR_get(x)       _BGQ_GET(37,63,x)
#define MU_DCR__XM1_ERR_INFO__ADDR_set(v)       _BGQ_SET(37,63,v)
#define MU_DCR__XM1_ERR_INFO__ADDR_insert(x,v)  _BGQ_INSERT(37,63,x,v)

  
// Register:
//   Name:        xm2_err_info
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Holds info that might help debug master port errors.
#define MU_DCR__XM2_ERR_INFO_offset  (0x0E24) // Offset of register in instance 
#define MU_DCR__XM2_ERR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sys
//   Description: System request violated range check
#define MU_DCR__XM2_ERR_INFO__SYS_width        1
#define MU_DCR__XM2_ERR_INFO__SYS_position     26
#define MU_DCR__XM2_ERR_INFO__SYS_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__XM2_ERR_INFO__SYS_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__XM2_ERR_INFO__SYS_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       addr
//   Description: Address that violated range check
#define MU_DCR__XM2_ERR_INFO__ADDR_width        37
#define MU_DCR__XM2_ERR_INFO__ADDR_position     63
#define MU_DCR__XM2_ERR_INFO__ADDR_get(x)       _BGQ_GET(37,63,x)
#define MU_DCR__XM2_ERR_INFO__ADDR_set(v)       _BGQ_SET(37,63,v)
#define MU_DCR__XM2_ERR_INFO__ADDR_insert(x,v)  _BGQ_INSERT(37,63,x,v)

  
// Register:
//   Name:        xs_err_info
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Holds info that might help debug slave errors.
#define MU_DCR__XS_ERR_INFO_offset  (0x0E2D) // Offset of register in instance 
#define MU_DCR__XS_ERR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       src
//   Description: ID of core/master that issued request
#define MU_DCR__XS_ERR_INFO__SRC_width        5
#define MU_DCR__XS_ERR_INFO__SRC_position     18
#define MU_DCR__XS_ERR_INFO__SRC_get(x)       _BGQ_GET(5,18,x)
#define MU_DCR__XS_ERR_INFO__SRC_set(v)       _BGQ_SET(5,18,v)
#define MU_DCR__XS_ERR_INFO__SRC_insert(x,v)  _BGQ_INSERT(5,18,x,v)

//   Field:       tid
//   Description: ID of thread that issued request
#define MU_DCR__XS_ERR_INFO__TID_width        2
#define MU_DCR__XS_ERR_INFO__TID_position     20
#define MU_DCR__XS_ERR_INFO__TID_get(x)       _BGQ_GET(2,20,x)
#define MU_DCR__XS_ERR_INFO__TID_set(v)       _BGQ_SET(2,20,v)
#define MU_DCR__XS_ERR_INFO__TID_insert(x,v)  _BGQ_INSERT(2,20,x,v)

//   Field:       addr
//   Description: Address of request
#define MU_DCR__XS_ERR_INFO__ADDR_width        34
#define MU_DCR__XS_ERR_INFO__ADDR_position     54
#define MU_DCR__XS_ERR_INFO__ADDR_get(x)       _BGQ_GET(34,54,x)
#define MU_DCR__XS_ERR_INFO__ADDR_set(v)       _BGQ_SET(34,54,v)
#define MU_DCR__XS_ERR_INFO__ADDR_insert(x,v)  _BGQ_INSERT(34,54,x,v)

//   Field:       ttype
//   Description: Ttype of request
#define MU_DCR__XS_ERR_INFO__TTYPE_width        6
#define MU_DCR__XS_ERR_INFO__TTYPE_position     60
#define MU_DCR__XS_ERR_INFO__TTYPE_get(x)       _BGQ_GET(6,60,x)
#define MU_DCR__XS_ERR_INFO__TTYPE_set(v)       _BGQ_SET(6,60,v)
#define MU_DCR__XS_ERR_INFO__TTYPE_insert(x,v)  _BGQ_INSERT(6,60,x,v)

//   Field:       size
//   Description: Size of load request
#define MU_DCR__XS_ERR_INFO__SIZE_width        3
#define MU_DCR__XS_ERR_INFO__SIZE_position     63
#define MU_DCR__XS_ERR_INFO__SIZE_get(x)       _BGQ_GET(3,63,x)
#define MU_DCR__XS_ERR_INFO__SIZE_set(v)       _BGQ_SET(3,63,v)
#define MU_DCR__XS_ERR_INFO__SIZE_insert(x,v)  _BGQ_INSERT(3,63,x,v)

  
// Register:
//   Name:        mmregs_fifo_prot_err
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the internal mmregs index that resulted in a protection violation that was caused by a user attempting to write a system location.
#define MU_DCR__MMREGS_FIFO_PROT_ERR_offset  (0x0E36) // Offset of register in instance 
#define MU_DCR__MMREGS_FIFO_PROT_ERR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       index
//   Description: The internal mmregs index of the offending write. Bits 0-4 are the group for FIFO protection errors. Bits 7-10 are the class for barrier protection errors.
#define MU_DCR__MMREGS_FIFO_PROT_ERR__INDEX_width        11
#define MU_DCR__MMREGS_FIFO_PROT_ERR__INDEX_position     63
#define MU_DCR__MMREGS_FIFO_PROT_ERR__INDEX_get(x)       _BGQ_GET(11,63,x)
#define MU_DCR__MMREGS_FIFO_PROT_ERR__INDEX_set(v)       _BGQ_SET(11,63,v)
#define MU_DCR__MMREGS_FIFO_PROT_ERR__INDEX_insert(x,v)  _BGQ_INSERT(11,63,x,v)

  
// Register:
//   Name:        rmfifo
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the ID of a rmFIFO, which raised an interrupt because it did not have sufficient space to receive the next packet.
#define MU_DCR__RMFIFO_offset  (0x0E3F) // Offset of register in instance 
#define MU_DCR__RMFIFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       violation_id
//   Description: The ID of the rmFIFO that has insufficient space
#define MU_DCR__RMFIFO__VIOLATION_ID_width        9
#define MU_DCR__RMFIFO__VIOLATION_ID_position     63
#define MU_DCR__RMFIFO__VIOLATION_ID_get(x)       _BGQ_GET(9,63,x)
#define MU_DCR__RMFIFO__VIOLATION_ID_set(v)       _BGQ_SET(9,63,v)
#define MU_DCR__RMFIFO__VIOLATION_ID_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        mcsram_err_fifo_id
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Holds imFIFO ID that caused descriptor error in MCSRAM
#define MU_DCR__MCSRAM_ERR_FIFO_ID_offset  (0x0E50) // Offset of register in instance 
#define MU_DCR__MCSRAM_ERR_FIFO_ID_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: imFIFO ID that caused error.
#define MU_DCR__MCSRAM_ERR_FIFO_ID__VALUE_width        10
#define MU_DCR__MCSRAM_ERR_FIFO_ID__VALUE_position     63
#define MU_DCR__MCSRAM_ERR_FIFO_ID__VALUE_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__MCSRAM_ERR_FIFO_ID__VALUE_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__MCSRAM_ERR_FIFO_ID__VALUE_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        imfifo
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the ID of a user imFIFO, if this imFIFO is specified as a user imFIFO, and a packet from that imFIFO tries to access a system network FIFO.
#define MU_DCR__IMFIFO_offset  (0x0E51) // Offset of register in instance 
#define MU_DCR__IMFIFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       violation_id
//   Description: The ID of the offending imFIFO
#define MU_DCR__IMFIFO__VIOLATION_ID_width        10
#define MU_DCR__IMFIFO__VIOLATION_ID_position     63
#define MU_DCR__IMFIFO__VIOLATION_ID_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__IMFIFO__VIOLATION_ID_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__IMFIFO__VIOLATION_ID_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rmfifo_access_error_id
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the rmFIFO ID on which software error was detected.
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID_offset  (0x0E6A) // Offset of register in instance 
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rmfifo_id
//   Description: The rmFIFO ID
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID__RMFIFO_ID_width        9
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID__RMFIFO_ID_position     63
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID__RMFIFO_ID_get(x)       _BGQ_GET(9,63,x)
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID__RMFIFO_ID_set(v)       _BGQ_SET(9,63,v)
#define MU_DCR__RMFIFO_ACCESS_ERROR_ID__RMFIFO_ID_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        ime_p_err0
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME0
#define MU_DCR__IME_P_ERR0_offset  (0x0E7B) // Offset of register in instance 
#define MU_DCR__IME_P_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map0
//   Description:  iME0 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR0__IME_P_ERR_MAP0_width        7
#define MU_DCR__IME_P_ERR0__IME_P_ERR_MAP0_position     63
#define MU_DCR__IME_P_ERR0__IME_P_ERR_MAP0_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR0__IME_P_ERR_MAP0_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR0__IME_P_ERR_MAP0_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err1
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME1
#define MU_DCR__IME_P_ERR1_offset  (0x0E7C) // Offset of register in instance 
#define MU_DCR__IME_P_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map1
//   Description:  iME1 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR1__IME_P_ERR_MAP1_width        7
#define MU_DCR__IME_P_ERR1__IME_P_ERR_MAP1_position     63
#define MU_DCR__IME_P_ERR1__IME_P_ERR_MAP1_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR1__IME_P_ERR_MAP1_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR1__IME_P_ERR_MAP1_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err2
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME2
#define MU_DCR__IME_P_ERR2_offset  (0x0E7D) // Offset of register in instance 
#define MU_DCR__IME_P_ERR2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map2
//   Description:  iME2 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR2__IME_P_ERR_MAP2_width        7
#define MU_DCR__IME_P_ERR2__IME_P_ERR_MAP2_position     63
#define MU_DCR__IME_P_ERR2__IME_P_ERR_MAP2_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR2__IME_P_ERR_MAP2_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR2__IME_P_ERR_MAP2_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err3
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME3
#define MU_DCR__IME_P_ERR3_offset  (0x0E7E) // Offset of register in instance 
#define MU_DCR__IME_P_ERR3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map3
//   Description:  iME3 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR3__IME_P_ERR_MAP3_width        7
#define MU_DCR__IME_P_ERR3__IME_P_ERR_MAP3_position     63
#define MU_DCR__IME_P_ERR3__IME_P_ERR_MAP3_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR3__IME_P_ERR_MAP3_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR3__IME_P_ERR_MAP3_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err4
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME4
#define MU_DCR__IME_P_ERR4_offset  (0x0E7F) // Offset of register in instance 
#define MU_DCR__IME_P_ERR4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map4
//   Description:  iME4 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR4__IME_P_ERR_MAP4_width        7
#define MU_DCR__IME_P_ERR4__IME_P_ERR_MAP4_position     63
#define MU_DCR__IME_P_ERR4__IME_P_ERR_MAP4_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR4__IME_P_ERR_MAP4_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR4__IME_P_ERR_MAP4_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err5
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME5
#define MU_DCR__IME_P_ERR5_offset  (0x0E80) // Offset of register in instance 
#define MU_DCR__IME_P_ERR5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map5
//   Description:  iME5 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR5__IME_P_ERR_MAP5_width        7
#define MU_DCR__IME_P_ERR5__IME_P_ERR_MAP5_position     63
#define MU_DCR__IME_P_ERR5__IME_P_ERR_MAP5_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR5__IME_P_ERR_MAP5_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR5__IME_P_ERR_MAP5_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err6
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME6
#define MU_DCR__IME_P_ERR6_offset  (0x0E81) // Offset of register in instance 
#define MU_DCR__IME_P_ERR6_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map6
//   Description:  iME6 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR6__IME_P_ERR_MAP6_width        7
#define MU_DCR__IME_P_ERR6__IME_P_ERR_MAP6_position     63
#define MU_DCR__IME_P_ERR6__IME_P_ERR_MAP6_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR6__IME_P_ERR_MAP6_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR6__IME_P_ERR_MAP6_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err7
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME7
#define MU_DCR__IME_P_ERR7_offset  (0x0E82) // Offset of register in instance 
#define MU_DCR__IME_P_ERR7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map7
//   Description:  iME7 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR7__IME_P_ERR_MAP7_width        7
#define MU_DCR__IME_P_ERR7__IME_P_ERR_MAP7_position     63
#define MU_DCR__IME_P_ERR7__IME_P_ERR_MAP7_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR7__IME_P_ERR_MAP7_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR7__IME_P_ERR_MAP7_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err8
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME8
#define MU_DCR__IME_P_ERR8_offset  (0x0E83) // Offset of register in instance 
#define MU_DCR__IME_P_ERR8_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map8
//   Description:  iME8 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR8__IME_P_ERR_MAP8_width        7
#define MU_DCR__IME_P_ERR8__IME_P_ERR_MAP8_position     63
#define MU_DCR__IME_P_ERR8__IME_P_ERR_MAP8_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR8__IME_P_ERR_MAP8_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR8__IME_P_ERR_MAP8_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err9
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME9
#define MU_DCR__IME_P_ERR9_offset  (0x0E84) // Offset of register in instance 
#define MU_DCR__IME_P_ERR9_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map9
//   Description:  iME9 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR9__IME_P_ERR_MAP9_width        7
#define MU_DCR__IME_P_ERR9__IME_P_ERR_MAP9_position     63
#define MU_DCR__IME_P_ERR9__IME_P_ERR_MAP9_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR9__IME_P_ERR_MAP9_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR9__IME_P_ERR_MAP9_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err10
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME10
#define MU_DCR__IME_P_ERR10_offset  (0x0E85) // Offset of register in instance 
#define MU_DCR__IME_P_ERR10_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map10
//   Description:  iME10 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR10__IME_P_ERR_MAP10_width        7
#define MU_DCR__IME_P_ERR10__IME_P_ERR_MAP10_position     63
#define MU_DCR__IME_P_ERR10__IME_P_ERR_MAP10_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR10__IME_P_ERR_MAP10_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR10__IME_P_ERR_MAP10_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err11
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME11
#define MU_DCR__IME_P_ERR11_offset  (0x0E86) // Offset of register in instance 
#define MU_DCR__IME_P_ERR11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map11
//   Description:  iME11 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR11__IME_P_ERR_MAP11_width        7
#define MU_DCR__IME_P_ERR11__IME_P_ERR_MAP11_position     63
#define MU_DCR__IME_P_ERR11__IME_P_ERR_MAP11_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR11__IME_P_ERR_MAP11_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR11__IME_P_ERR_MAP11_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err12
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME12
#define MU_DCR__IME_P_ERR12_offset  (0x0E87) // Offset of register in instance 
#define MU_DCR__IME_P_ERR12_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map12
//   Description:  iME12 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR12__IME_P_ERR_MAP12_width        7
#define MU_DCR__IME_P_ERR12__IME_P_ERR_MAP12_position     63
#define MU_DCR__IME_P_ERR12__IME_P_ERR_MAP12_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR12__IME_P_ERR_MAP12_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR12__IME_P_ERR_MAP12_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err13
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME13
#define MU_DCR__IME_P_ERR13_offset  (0x0E88) // Offset of register in instance 
#define MU_DCR__IME_P_ERR13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map13
//   Description:  iME13 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR13__IME_P_ERR_MAP13_width        7
#define MU_DCR__IME_P_ERR13__IME_P_ERR_MAP13_position     63
#define MU_DCR__IME_P_ERR13__IME_P_ERR_MAP13_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR13__IME_P_ERR_MAP13_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR13__IME_P_ERR_MAP13_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err14
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME14
#define MU_DCR__IME_P_ERR14_offset  (0x0E89) // Offset of register in instance 
#define MU_DCR__IME_P_ERR14_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map14
//   Description:  iME14 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR14__IME_P_ERR_MAP14_width        7
#define MU_DCR__IME_P_ERR14__IME_P_ERR_MAP14_position     63
#define MU_DCR__IME_P_ERR14__IME_P_ERR_MAP14_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR14__IME_P_ERR_MAP14_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR14__IME_P_ERR_MAP14_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        ime_p_err15
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in iME15
#define MU_DCR__IME_P_ERR15_offset  (0x0E8A) // Offset of register in instance 
#define MU_DCR__IME_P_ERR15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ime_p_err_map15
//   Description:  iME15 parity error flag for each of 7 groups of latch
#define MU_DCR__IME_P_ERR15__IME_P_ERR_MAP15_width        7
#define MU_DCR__IME_P_ERR15__IME_P_ERR_MAP15_position     63
#define MU_DCR__IME_P_ERR15__IME_P_ERR_MAP15_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__IME_P_ERR15__IME_P_ERR_MAP15_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__IME_P_ERR15__IME_P_ERR_MAP15_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err0
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME0
#define MU_DCR__RME_P_ERR0_offset  (0x0E9B) // Offset of register in instance 
#define MU_DCR__RME_P_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map0
//   Description:  rME0 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR0__RME_P_ERR_MAP0_width        10
#define MU_DCR__RME_P_ERR0__RME_P_ERR_MAP0_position     63
#define MU_DCR__RME_P_ERR0__RME_P_ERR_MAP0_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR0__RME_P_ERR_MAP0_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR0__RME_P_ERR_MAP0_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err0
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME0
#define MU_DCR__RME_HEADER_ERR0_offset  (0x0E9C) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map0
//   Description:  rME0 error flag for each of 7 types of errors. Bit 60: imFIFO ID too large (>543) for remote get packet, rmFIFO too large (>271) for memory FIFO packet, or remote put base address ID too large (>543) for remote put packet. Bit 61: A system rME received a packet that has user VC (i.e. VC !=3,6). Bit 62: remote put counter base address ID too large (>543) for remote put packet. Bit 57 to 59 and 63 will not be asserted unless there is a hardware problem.
#define MU_DCR__RME_HEADER_ERR0__RME_HEADER_ERR_MAP0_width        7
#define MU_DCR__RME_HEADER_ERR0__RME_HEADER_ERR_MAP0_position     63
#define MU_DCR__RME_HEADER_ERR0__RME_HEADER_ERR_MAP0_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR0__RME_HEADER_ERR_MAP0_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR0__RME_HEADER_ERR_MAP0_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err1
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME1
#define MU_DCR__RME_P_ERR1_offset  (0x0E9D) // Offset of register in instance 
#define MU_DCR__RME_P_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map1
//   Description:  rME1 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR1__RME_P_ERR_MAP1_width        10
#define MU_DCR__RME_P_ERR1__RME_P_ERR_MAP1_position     63
#define MU_DCR__RME_P_ERR1__RME_P_ERR_MAP1_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR1__RME_P_ERR_MAP1_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR1__RME_P_ERR_MAP1_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err1
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME1
#define MU_DCR__RME_HEADER_ERR1_offset  (0x0E9E) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map1
//   Description:  rME1 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR1__RME_HEADER_ERR_MAP1_width        7
#define MU_DCR__RME_HEADER_ERR1__RME_HEADER_ERR_MAP1_position     63
#define MU_DCR__RME_HEADER_ERR1__RME_HEADER_ERR_MAP1_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR1__RME_HEADER_ERR_MAP1_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR1__RME_HEADER_ERR_MAP1_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err2
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME2
#define MU_DCR__RME_P_ERR2_offset  (0x0E9F) // Offset of register in instance 
#define MU_DCR__RME_P_ERR2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map2
//   Description:  rME2 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR2__RME_P_ERR_MAP2_width        10
#define MU_DCR__RME_P_ERR2__RME_P_ERR_MAP2_position     63
#define MU_DCR__RME_P_ERR2__RME_P_ERR_MAP2_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR2__RME_P_ERR_MAP2_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR2__RME_P_ERR_MAP2_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err2
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME2
#define MU_DCR__RME_HEADER_ERR2_offset  (0x0EA0) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map2
//   Description:  rME2 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR2__RME_HEADER_ERR_MAP2_width        7
#define MU_DCR__RME_HEADER_ERR2__RME_HEADER_ERR_MAP2_position     63
#define MU_DCR__RME_HEADER_ERR2__RME_HEADER_ERR_MAP2_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR2__RME_HEADER_ERR_MAP2_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR2__RME_HEADER_ERR_MAP2_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err3
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME3
#define MU_DCR__RME_P_ERR3_offset  (0x0EA1) // Offset of register in instance 
#define MU_DCR__RME_P_ERR3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map3
//   Description:  rME3 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR3__RME_P_ERR_MAP3_width        10
#define MU_DCR__RME_P_ERR3__RME_P_ERR_MAP3_position     63
#define MU_DCR__RME_P_ERR3__RME_P_ERR_MAP3_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR3__RME_P_ERR_MAP3_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR3__RME_P_ERR_MAP3_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err3
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME3
#define MU_DCR__RME_HEADER_ERR3_offset  (0x0EA2) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map3
//   Description:  rME3 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR3__RME_HEADER_ERR_MAP3_width        7
#define MU_DCR__RME_HEADER_ERR3__RME_HEADER_ERR_MAP3_position     63
#define MU_DCR__RME_HEADER_ERR3__RME_HEADER_ERR_MAP3_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR3__RME_HEADER_ERR_MAP3_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR3__RME_HEADER_ERR_MAP3_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err4
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME4
#define MU_DCR__RME_P_ERR4_offset  (0x0EA3) // Offset of register in instance 
#define MU_DCR__RME_P_ERR4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map4
//   Description:  rME4 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR4__RME_P_ERR_MAP4_width        10
#define MU_DCR__RME_P_ERR4__RME_P_ERR_MAP4_position     63
#define MU_DCR__RME_P_ERR4__RME_P_ERR_MAP4_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR4__RME_P_ERR_MAP4_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR4__RME_P_ERR_MAP4_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err4
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME4
#define MU_DCR__RME_HEADER_ERR4_offset  (0x0EA4) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map4
//   Description:  rME4 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR4__RME_HEADER_ERR_MAP4_width        7
#define MU_DCR__RME_HEADER_ERR4__RME_HEADER_ERR_MAP4_position     63
#define MU_DCR__RME_HEADER_ERR4__RME_HEADER_ERR_MAP4_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR4__RME_HEADER_ERR_MAP4_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR4__RME_HEADER_ERR_MAP4_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err5
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME5
#define MU_DCR__RME_P_ERR5_offset  (0x0EA5) // Offset of register in instance 
#define MU_DCR__RME_P_ERR5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map5
//   Description:  rME5 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR5__RME_P_ERR_MAP5_width        10
#define MU_DCR__RME_P_ERR5__RME_P_ERR_MAP5_position     63
#define MU_DCR__RME_P_ERR5__RME_P_ERR_MAP5_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR5__RME_P_ERR_MAP5_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR5__RME_P_ERR_MAP5_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err5
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME5
#define MU_DCR__RME_HEADER_ERR5_offset  (0x0EA6) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map5
//   Description:  rME5 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR5__RME_HEADER_ERR_MAP5_width        7
#define MU_DCR__RME_HEADER_ERR5__RME_HEADER_ERR_MAP5_position     63
#define MU_DCR__RME_HEADER_ERR5__RME_HEADER_ERR_MAP5_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR5__RME_HEADER_ERR_MAP5_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR5__RME_HEADER_ERR_MAP5_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err6
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME6
#define MU_DCR__RME_P_ERR6_offset  (0x0EA7) // Offset of register in instance 
#define MU_DCR__RME_P_ERR6_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map6
//   Description:  rME6 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR6__RME_P_ERR_MAP6_width        10
#define MU_DCR__RME_P_ERR6__RME_P_ERR_MAP6_position     63
#define MU_DCR__RME_P_ERR6__RME_P_ERR_MAP6_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR6__RME_P_ERR_MAP6_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR6__RME_P_ERR_MAP6_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err6
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME6
#define MU_DCR__RME_HEADER_ERR6_offset  (0x0EA8) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR6_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map6
//   Description:  rME6 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR6__RME_HEADER_ERR_MAP6_width        7
#define MU_DCR__RME_HEADER_ERR6__RME_HEADER_ERR_MAP6_position     63
#define MU_DCR__RME_HEADER_ERR6__RME_HEADER_ERR_MAP6_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR6__RME_HEADER_ERR_MAP6_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR6__RME_HEADER_ERR_MAP6_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err7
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME7
#define MU_DCR__RME_P_ERR7_offset  (0x0EA9) // Offset of register in instance 
#define MU_DCR__RME_P_ERR7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map7
//   Description:  rME7 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR7__RME_P_ERR_MAP7_width        10
#define MU_DCR__RME_P_ERR7__RME_P_ERR_MAP7_position     63
#define MU_DCR__RME_P_ERR7__RME_P_ERR_MAP7_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR7__RME_P_ERR_MAP7_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR7__RME_P_ERR_MAP7_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err7
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME7
#define MU_DCR__RME_HEADER_ERR7_offset  (0x0EAA) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map7
//   Description:  rME7 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR7__RME_HEADER_ERR_MAP7_width        7
#define MU_DCR__RME_HEADER_ERR7__RME_HEADER_ERR_MAP7_position     63
#define MU_DCR__RME_HEADER_ERR7__RME_HEADER_ERR_MAP7_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR7__RME_HEADER_ERR_MAP7_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR7__RME_HEADER_ERR_MAP7_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err8
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME8
#define MU_DCR__RME_P_ERR8_offset  (0x0EAB) // Offset of register in instance 
#define MU_DCR__RME_P_ERR8_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map8
//   Description:  rME8 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR8__RME_P_ERR_MAP8_width        10
#define MU_DCR__RME_P_ERR8__RME_P_ERR_MAP8_position     63
#define MU_DCR__RME_P_ERR8__RME_P_ERR_MAP8_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR8__RME_P_ERR_MAP8_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR8__RME_P_ERR_MAP8_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err8
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME8
#define MU_DCR__RME_HEADER_ERR8_offset  (0x0EAC) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR8_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map8
//   Description:  rME8 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR8__RME_HEADER_ERR_MAP8_width        7
#define MU_DCR__RME_HEADER_ERR8__RME_HEADER_ERR_MAP8_position     63
#define MU_DCR__RME_HEADER_ERR8__RME_HEADER_ERR_MAP8_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR8__RME_HEADER_ERR_MAP8_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR8__RME_HEADER_ERR_MAP8_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err9
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME9
#define MU_DCR__RME_P_ERR9_offset  (0x0EAD) // Offset of register in instance 
#define MU_DCR__RME_P_ERR9_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map9
//   Description:  rME9 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR9__RME_P_ERR_MAP9_width        10
#define MU_DCR__RME_P_ERR9__RME_P_ERR_MAP9_position     63
#define MU_DCR__RME_P_ERR9__RME_P_ERR_MAP9_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR9__RME_P_ERR_MAP9_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR9__RME_P_ERR_MAP9_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err9
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME9
#define MU_DCR__RME_HEADER_ERR9_offset  (0x0EAE) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR9_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map9
//   Description:  rME9 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR9__RME_HEADER_ERR_MAP9_width        7
#define MU_DCR__RME_HEADER_ERR9__RME_HEADER_ERR_MAP9_position     63
#define MU_DCR__RME_HEADER_ERR9__RME_HEADER_ERR_MAP9_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR9__RME_HEADER_ERR_MAP9_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR9__RME_HEADER_ERR_MAP9_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err10
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME10
#define MU_DCR__RME_P_ERR10_offset  (0x0EAF) // Offset of register in instance 
#define MU_DCR__RME_P_ERR10_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map10
//   Description:  rME10 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR10__RME_P_ERR_MAP10_width        10
#define MU_DCR__RME_P_ERR10__RME_P_ERR_MAP10_position     63
#define MU_DCR__RME_P_ERR10__RME_P_ERR_MAP10_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR10__RME_P_ERR_MAP10_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR10__RME_P_ERR_MAP10_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err10
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME10
#define MU_DCR__RME_HEADER_ERR10_offset  (0x0EB0) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR10_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map10
//   Description:  rME10 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR10__RME_HEADER_ERR_MAP10_width        7
#define MU_DCR__RME_HEADER_ERR10__RME_HEADER_ERR_MAP10_position     63
#define MU_DCR__RME_HEADER_ERR10__RME_HEADER_ERR_MAP10_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR10__RME_HEADER_ERR_MAP10_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR10__RME_HEADER_ERR_MAP10_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err11
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME11
#define MU_DCR__RME_P_ERR11_offset  (0x0EB1) // Offset of register in instance 
#define MU_DCR__RME_P_ERR11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map11
//   Description:  rME11 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR11__RME_P_ERR_MAP11_width        10
#define MU_DCR__RME_P_ERR11__RME_P_ERR_MAP11_position     63
#define MU_DCR__RME_P_ERR11__RME_P_ERR_MAP11_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR11__RME_P_ERR_MAP11_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR11__RME_P_ERR_MAP11_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err11
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME11
#define MU_DCR__RME_HEADER_ERR11_offset  (0x0EB2) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map11
//   Description:  rME11 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR11__RME_HEADER_ERR_MAP11_width        7
#define MU_DCR__RME_HEADER_ERR11__RME_HEADER_ERR_MAP11_position     63
#define MU_DCR__RME_HEADER_ERR11__RME_HEADER_ERR_MAP11_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR11__RME_HEADER_ERR_MAP11_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR11__RME_HEADER_ERR_MAP11_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err12
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME12
#define MU_DCR__RME_P_ERR12_offset  (0x0EB3) // Offset of register in instance 
#define MU_DCR__RME_P_ERR12_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map12
//   Description:  rME12 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR12__RME_P_ERR_MAP12_width        10
#define MU_DCR__RME_P_ERR12__RME_P_ERR_MAP12_position     63
#define MU_DCR__RME_P_ERR12__RME_P_ERR_MAP12_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR12__RME_P_ERR_MAP12_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR12__RME_P_ERR_MAP12_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err12
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME12
#define MU_DCR__RME_HEADER_ERR12_offset  (0x0EB4) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR12_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map12
//   Description:  rME12 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR12__RME_HEADER_ERR_MAP12_width        7
#define MU_DCR__RME_HEADER_ERR12__RME_HEADER_ERR_MAP12_position     63
#define MU_DCR__RME_HEADER_ERR12__RME_HEADER_ERR_MAP12_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR12__RME_HEADER_ERR_MAP12_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR12__RME_HEADER_ERR_MAP12_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err13
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME13
#define MU_DCR__RME_P_ERR13_offset  (0x0EB5) // Offset of register in instance 
#define MU_DCR__RME_P_ERR13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map13
//   Description:  rME13 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR13__RME_P_ERR_MAP13_width        10
#define MU_DCR__RME_P_ERR13__RME_P_ERR_MAP13_position     63
#define MU_DCR__RME_P_ERR13__RME_P_ERR_MAP13_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR13__RME_P_ERR_MAP13_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR13__RME_P_ERR_MAP13_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err13
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME13
#define MU_DCR__RME_HEADER_ERR13_offset  (0x0EB6) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map13
//   Description:  rME13 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR13__RME_HEADER_ERR_MAP13_width        7
#define MU_DCR__RME_HEADER_ERR13__RME_HEADER_ERR_MAP13_position     63
#define MU_DCR__RME_HEADER_ERR13__RME_HEADER_ERR_MAP13_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR13__RME_HEADER_ERR_MAP13_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR13__RME_HEADER_ERR_MAP13_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err14
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME14
#define MU_DCR__RME_P_ERR14_offset  (0x0EB7) // Offset of register in instance 
#define MU_DCR__RME_P_ERR14_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map14
//   Description:  rME14 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR14__RME_P_ERR_MAP14_width        10
#define MU_DCR__RME_P_ERR14__RME_P_ERR_MAP14_position     63
#define MU_DCR__RME_P_ERR14__RME_P_ERR_MAP14_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR14__RME_P_ERR_MAP14_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR14__RME_P_ERR_MAP14_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err14
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME14
#define MU_DCR__RME_HEADER_ERR14_offset  (0x0EB8) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR14_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map14
//   Description:  rME14 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR14__RME_HEADER_ERR_MAP14_width        7
#define MU_DCR__RME_HEADER_ERR14__RME_HEADER_ERR_MAP14_position     63
#define MU_DCR__RME_HEADER_ERR14__RME_HEADER_ERR_MAP14_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR14__RME_HEADER_ERR_MAP14_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR14__RME_HEADER_ERR_MAP14_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        rme_p_err15
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds parity error type detected in rME15
#define MU_DCR__RME_P_ERR15_offset  (0x0EB9) // Offset of register in instance 
#define MU_DCR__RME_P_ERR15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_p_err_map15
//   Description:  rME15 parity error flag for each of 9 groups of latches
#define MU_DCR__RME_P_ERR15__RME_P_ERR_MAP15_width        10
#define MU_DCR__RME_P_ERR15__RME_P_ERR_MAP15_position     63
#define MU_DCR__RME_P_ERR15__RME_P_ERR_MAP15_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__RME_P_ERR15__RME_P_ERR_MAP15_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__RME_P_ERR15__RME_P_ERR_MAP15_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rme_header_err15
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds header error type detected in rME15
#define MU_DCR__RME_HEADER_ERR15_offset  (0x0EBA) // Offset of register in instance 
#define MU_DCR__RME_HEADER_ERR15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rme_header_err_map15
//   Description:  rME15 error flag for each of 7 types of errors. The meaning of each bit is same as rme_header_err0.
#define MU_DCR__RME_HEADER_ERR15__RME_HEADER_ERR_MAP15_width        7
#define MU_DCR__RME_HEADER_ERR15__RME_HEADER_ERR_MAP15_position     63
#define MU_DCR__RME_HEADER_ERR15__RME_HEADER_ERR_MAP15_get(x)       _BGQ_GET(7,63,x)
#define MU_DCR__RME_HEADER_ERR15__RME_HEADER_ERR_MAP15_set(v)       _BGQ_SET(7,63,v)
#define MU_DCR__RME_HEADER_ERR15__RME_HEADER_ERR_MAP15_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        icsram_rget_fifo_full
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the remote get imFIFO ID which is full.
#define MU_DCR__ICSRAM_RGET_FIFO_FULL_offset  (0x0EC3) // Offset of register in instance 
#define MU_DCR__ICSRAM_RGET_FIFO_FULL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       id
//   Description: The ID of the remote get imFIFO that is full.
#define MU_DCR__ICSRAM_RGET_FIFO_FULL__ID_width        10
#define MU_DCR__ICSRAM_RGET_FIFO_FULL__ID_position     63
#define MU_DCR__ICSRAM_RGET_FIFO_FULL__ID_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__ICSRAM_RGET_FIFO_FULL__ID_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__ICSRAM_RGET_FIFO_FULL__ID_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        imfifo_access_error_id
//   Type:        Interrupt Information
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: This register holds the remote-get imFIFO ID on which software error was detected.
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID_offset  (0x0ECC) // Offset of register in instance 
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID_reset  UNSIGNED64(0x0000000000000000)

//   Field:       imfifo_id
//   Description: The imFIFO ID
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID__IMFIFO_ID_width        10
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID__IMFIFO_ID_position     63
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID__IMFIFO_ID_get(x)       _BGQ_GET(10,63,x)
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID__IMFIFO_ID_set(v)       _BGQ_SET(10,63,v)
#define MU_DCR__IMFIFO_ACCESS_ERROR_ID__IMFIFO_ID_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        upc_mu_counter
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: MU UPC Counters Data
#define MU_DCR__UPC_MU_COUNTER_offset  (0x0F00) // Offset of register in instance 
#define MU_DCR__UPC_MU_COUNTER_range  (0x28) // Range of external address space
#define MU_DCR__UPC_MU_COUNTER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ovf
//   Description: Counter Overflow bit
#define MU_DCR__UPC_MU_COUNTER__OVF_width        1
#define MU_DCR__UPC_MU_COUNTER__OVF_position     49
#define MU_DCR__UPC_MU_COUNTER__OVF_get(x)       _BGQ_GET(1,49,x)
#define MU_DCR__UPC_MU_COUNTER__OVF_set(v)       _BGQ_SET(1,49,v)
#define MU_DCR__UPC_MU_COUNTER__OVF_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       data
//   Description: MU Counter Data
#define MU_DCR__UPC_MU_COUNTER__DATA_width        14
#define MU_DCR__UPC_MU_COUNTER__DATA_position     63
#define MU_DCR__UPC_MU_COUNTER__DATA_get(x)       _BGQ_GET(14,63,x)
#define MU_DCR__UPC_MU_COUNTER__DATA_set(v)       _BGQ_SET(14,63,v)
#define MU_DCR__UPC_MU_COUNTER__DATA_insert(x,v)  _BGQ_INSERT(14,63,x,v)

  
// Register:
//   Name:        upc_mu_counter_control_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: MU UPC Counters RW Control Register
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW_offset  (0x0F41) // Offset of register in instance 
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW_range  (0x1) // Range of external address space
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: MU Counter Reset(RW)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__RESET_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__RESET_position     0
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__RESET_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__RESET_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: MU Counter Enable(RW)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__ENABLE_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__ENABLE_position     32
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_RW__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_mu_counter_control_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: MU UPC Counters W1S Control Register
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S_offset  (0x0F42) // Offset of register in instance 
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S_range  (0x1) // Range of external address space
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: MU Counter Reset(W1S)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_position     0
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: MU Counter Enable(W1S)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_position     32
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_mu_counter_control_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: MU UPC Counters W1C Control Register
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C_offset  (0x0F43) // Offset of register in instance 
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C_range  (0x1) // Range of external address space
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: MU Counter Reset(W1C)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__RESET_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__RESET_position     0
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__RESET_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__RESET_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: MU Counter Enable(W1C)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_width        1
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_position     32
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__UPC_MU_COUNTER_CONTROL_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_mu_config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_MU Configuration Register
#define MU_DCR__UPC_MU_CONFIG_offset  (0x0F44) // Offset of register in instance 
#define MU_DCR__UPC_MU_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_mu_ring_enable
//   Description: Enables UPC_MU Ring logic. Must be set to one to pass daisy chain data.
#define MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_width        1
#define MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_position     3
#define MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__UPC_MU_CONFIG__UPC_MU_RING_ENABLE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       sync_override
//   Description: Sync Override bits for all counters
#define MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_width        1
#define MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_position     48
#define MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_get(x)       _BGQ_GET(1,48,x)
#define MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_set(v)       _BGQ_SET(1,48,v)
#define MU_DCR__UPC_MU_CONFIG__SYNC_OVERRIDE_insert(x,v)  _BGQ_INSERT(1,48,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0E07) // Offset of register in instance 
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0E08) // Offset of register in instance 
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0E09) // Offset of register in instance 
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0E00) // Offset of State register in instance
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0E03) // Offset of First register in instance
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0E02) // Offset of Force register in instance
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0E04) // Offset of Machine Check register in instance
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0E05) // Offset of Critical register in instance
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0E06) // Offset of Noncritical register in instance

#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt State Register: master_port0_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MASTER_PORT0_INTERRUPTS__STATE_offset         (0x0E0A) // Offset of State register in instance
#define MU_DCR__MASTER_PORT0_INTERRUPTS__FIRST_offset         (0x0E0E) // Offset of First register in instance
#define MU_DCR__MASTER_PORT0_INTERRUPTS__FORCE_offset         (0x0E0D) // Offset of Force register in instance
#define MU_DCR__MASTER_PORT0_INTERRUPTS__MACHINE_CHECK_offset (0x0E0F) // Offset of Machine Check register in instance
#define MU_DCR__MASTER_PORT0_INTERRUPTS__CRITICAL_offset      (0x0E10) // Offset of Critical register in instance
#define MU_DCR__MASTER_PORT0_INTERRUPTS__NONCRITICAL_offset   (0x0E11) // Offset of Noncritical register in instance

#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_position     0
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_position     1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_position     2
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_position     3
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_position     4
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_position     5
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_position     6
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_position     7
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_position     8
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_position     9
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_position     10
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_position     11
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_position     12
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_position     13
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_position     14
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_position     15
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_position     16
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_position     17
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_position     18
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_position     19
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_position     20
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_position     21
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_position     22
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_position     23
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR24_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR24_position     24
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR25_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR25_position     25
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_width        1
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_position     26
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS__XM0_CORR_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)

  
// Interrupt State Register: master_port1_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MASTER_PORT1_INTERRUPTS__STATE_offset         (0x0E13) // Offset of State register in instance
#define MU_DCR__MASTER_PORT1_INTERRUPTS__FIRST_offset         (0x0E17) // Offset of First register in instance
#define MU_DCR__MASTER_PORT1_INTERRUPTS__FORCE_offset         (0x0E16) // Offset of Force register in instance
#define MU_DCR__MASTER_PORT1_INTERRUPTS__MACHINE_CHECK_offset (0x0E18) // Offset of Machine Check register in instance
#define MU_DCR__MASTER_PORT1_INTERRUPTS__CRITICAL_offset      (0x0E19) // Offset of Critical register in instance
#define MU_DCR__MASTER_PORT1_INTERRUPTS__NONCRITICAL_offset   (0x0E1A) // Offset of Noncritical register in instance

#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_position     0
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_position     1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_position     2
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_position     3
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_position     4
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_position     5
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_position     6
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_position     7
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_position     8
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_position     9
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_position     10
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_position     11
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_position     12
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_position     13
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_position     14
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_position     15
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_position     16
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_position     17
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_position     18
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_position     19
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_position     20
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_position     21
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_position     22
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_position     23
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR24_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR24_position     24
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR25_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR25_position     25
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_width        1
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_position     26
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS__XM1_CORR_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)

  
// Interrupt State Register: master_port2_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MASTER_PORT2_INTERRUPTS__STATE_offset         (0x0E1C) // Offset of State register in instance
#define MU_DCR__MASTER_PORT2_INTERRUPTS__FIRST_offset         (0x0E20) // Offset of First register in instance
#define MU_DCR__MASTER_PORT2_INTERRUPTS__FORCE_offset         (0x0E1F) // Offset of Force register in instance
#define MU_DCR__MASTER_PORT2_INTERRUPTS__MACHINE_CHECK_offset (0x0E21) // Offset of Machine Check register in instance
#define MU_DCR__MASTER_PORT2_INTERRUPTS__CRITICAL_offset      (0x0E22) // Offset of Critical register in instance
#define MU_DCR__MASTER_PORT2_INTERRUPTS__NONCRITICAL_offset   (0x0E23) // Offset of Noncritical register in instance

#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_position     0
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_position     1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_position     2
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_position     3
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_position     4
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_position     5
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_position     6
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_position     7
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_position     8
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_position     9
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_position     10
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_position     11
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_position     12
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_position     13
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_position     14
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_position     15
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_position     16
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_position     17
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_position     18
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_position     19
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_position     20
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_position     21
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_position     22
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_position     23
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR24_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR24_position     24
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR25_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR25_position     25
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_width        1
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_position     26
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS__XM2_CORR_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)

  
// Interrupt State Register: slave_port_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__SLAVE_PORT_INTERRUPTS__STATE_offset         (0x0E25) // Offset of State register in instance
#define MU_DCR__SLAVE_PORT_INTERRUPTS__FIRST_offset         (0x0E29) // Offset of First register in instance
#define MU_DCR__SLAVE_PORT_INTERRUPTS__FORCE_offset         (0x0E28) // Offset of Force register in instance
#define MU_DCR__SLAVE_PORT_INTERRUPTS__MACHINE_CHECK_offset (0x0E2A) // Offset of Machine Check register in instance
#define MU_DCR__SLAVE_PORT_INTERRUPTS__CRITICAL_offset      (0x0E2B) // Offset of Critical register in instance
#define MU_DCR__SLAVE_PORT_INTERRUPTS__NONCRITICAL_offset   (0x0E2C) // Offset of Noncritical register in instance

#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_position     0
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_position     1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_position     2
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_position     3
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_position     4
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_position     5
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_position     6
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_position     7
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_position     8
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_position     9
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_position     10
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_position     11
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_position     12
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_position     13
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_position     14
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_position     15
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_position     16
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_position     17
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_position     18
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_position     19
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_position     20
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR21_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR21_position     21
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR22_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR22_position     22
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR23_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR23_position     23
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR24_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR24_position     24
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR25_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR25_position     25
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR26_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR26_position     26
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR27_width        1
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR27_position     27
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS__XS_SOFTWARE_ERR27_insert(x,v)  _BGQ_INSERT(1,27,x,v)

  
// Interrupt State Register: mmregs_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MMREGS_INTERRUPTS__STATE_offset         (0x0E2E) // Offset of State register in instance
#define MU_DCR__MMREGS_INTERRUPTS__FIRST_offset         (0x0E32) // Offset of First register in instance
#define MU_DCR__MMREGS_INTERRUPTS__FORCE_offset         (0x0E31) // Offset of Force register in instance
#define MU_DCR__MMREGS_INTERRUPTS__MACHINE_CHECK_offset (0x0E33) // Offset of Machine Check register in instance
#define MU_DCR__MMREGS_INTERRUPTS__CRITICAL_offset      (0x0E34) // Offset of Critical register in instance
#define MU_DCR__MMREGS_INTERRUPTS__NONCRITICAL_offset   (0x0E35) // Offset of Noncritical register in instance

#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_position     0
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_position     1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_position     2
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_position     3
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_position     4
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_position     5
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_position     6
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_position     7
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_position     8
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR9_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR9_position     9
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR10_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR10_position     10
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR11_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR11_position     11
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR12_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR12_position     12
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR13_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR13_position     13
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR14_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR14_position     14
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR15_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR15_position     15
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR16_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR16_position     16
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR17_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR17_position     17
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR18_width        1
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR18_position     18
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__MMREGS_INTERRUPTS__MMREGS_SOFTWARE_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)

  
// Interrupt State Register: fifo_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__FIFO_INTERRUPTS__STATE_offset         (0x0E37) // Offset of State register in instance
#define MU_DCR__FIFO_INTERRUPTS__FIRST_offset         (0x0E3B) // Offset of First register in instance
#define MU_DCR__FIFO_INTERRUPTS__FORCE_offset         (0x0E3A) // Offset of Force register in instance
#define MU_DCR__FIFO_INTERRUPTS__MACHINE_CHECK_offset (0x0E3C) // Offset of Machine Check register in instance
#define MU_DCR__FIFO_INTERRUPTS__CRITICAL_offset      (0x0E3D) // Offset of Critical register in instance
#define MU_DCR__FIFO_INTERRUPTS__NONCRITICAL_offset   (0x0E3E) // Offset of Noncritical register in instance

#define MU_DCR__FIFO_INTERRUPTS__LOCAL_RING_width        1
#define MU_DCR__FIFO_INTERRUPTS__LOCAL_RING_position     61
#define MU_DCR__FIFO_INTERRUPTS__LOCAL_RING_get(x)       _BGQ_GET(1,61,x)
#define MU_DCR__FIFO_INTERRUPTS__LOCAL_RING_set(v)       _BGQ_SET(1,61,v)
#define MU_DCR__FIFO_INTERRUPTS__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,61,x,v)


#define MU_DCR__FIFO_INTERRUPTS__RMFIFO_INSUFFICIENT_SPACE_width        1
#define MU_DCR__FIFO_INTERRUPTS__RMFIFO_INSUFFICIENT_SPACE_position     63
#define MU_DCR__FIFO_INTERRUPTS__RMFIFO_INSUFFICIENT_SPACE_get(x)       _BGQ_GET(1,63,x)
#define MU_DCR__FIFO_INTERRUPTS__RMFIFO_INSUFFICIENT_SPACE_set(v)       _BGQ_SET(1,63,v)
#define MU_DCR__FIFO_INTERRUPTS__RMFIFO_INSUFFICIENT_SPACE_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt State Register: imu_ecc_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__IMU_ECC_INTERRUPTS__STATE_offset         (0x0E40) // Offset of State register in instance
#define MU_DCR__IMU_ECC_INTERRUPTS__FIRST_offset         (0x0E44) // Offset of First register in instance
#define MU_DCR__IMU_ECC_INTERRUPTS__FORCE_offset         (0x0E43) // Offset of Force register in instance
#define MU_DCR__IMU_ECC_INTERRUPTS__MACHINE_CHECK_offset (0x0E45) // Offset of Machine Check register in instance
#define MU_DCR__IMU_ECC_INTERRUPTS__CRITICAL_offset      (0x0E46) // Offset of Critical register in instance
#define MU_DCR__IMU_ECC_INTERRUPTS__NONCRITICAL_offset   (0x0E47) // Offset of Noncritical register in instance

#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_position     0
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_position     1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_position     2
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_position     3
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_position     4
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_position     5
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_position     6
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_position     7
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_position     8
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_position     9
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_position     10
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_UE10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_position     11
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE0_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_position     12
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE1_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_position     13
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE2_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_position     14
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE3_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_position     15
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE4_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_position     16
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE5_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_position     17
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE6_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_position     18
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE7_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_position     19
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE8_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_position     20
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE9_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_position     21
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE10_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_position     22
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE11_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_position     23
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE12_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_position     24
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE13_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_position     25
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE14_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_position     26
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE15_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_position     27
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE16_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_position     28
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE17_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_position     29
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE18_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_position     30
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE19_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_position     31
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE20_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_width        1
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_position     32
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__IMU_ECC_INTERRUPTS__IMU_ECC_CE21_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Interrupt State Register: mcsram_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MCSRAM_INTERRUPTS__STATE_offset         (0x0E48) // Offset of State register in instance
#define MU_DCR__MCSRAM_INTERRUPTS__FIRST_offset         (0x0E4C) // Offset of First register in instance
#define MU_DCR__MCSRAM_INTERRUPTS__FORCE_offset         (0x0E4B) // Offset of Force register in instance
#define MU_DCR__MCSRAM_INTERRUPTS__MACHINE_CHECK_offset (0x0E4D) // Offset of Machine Check register in instance
#define MU_DCR__MCSRAM_INTERRUPTS__CRITICAL_offset      (0x0E4E) // Offset of Critical register in instance
#define MU_DCR__MCSRAM_INTERRUPTS__NONCRITICAL_offset   (0x0E4F) // Offset of Noncritical register in instance

#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_position     0
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_position     1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_position     2
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_position     3
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_position     4
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_position     5
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_position     6
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_position     7
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_position     8
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_position     9
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_position     10
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_position     11
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_position     12
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_position     13
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_position     14
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR15_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR15_position     15
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR16_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR16_position     16
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR17_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR17_position     17
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR18_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR18_position     18
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR19_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR19_position     19
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR20_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR20_position     20
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR21_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR21_position     21
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR22_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR22_position     22
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR23_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR23_position     23
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR24_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR24_position     24
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR25_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR25_position     25
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR26_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR26_position     26
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR27_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR27_position     27
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR27_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_position     28
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR28_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_position     29
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR29_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_position     30
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_ERR30_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR0_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR0_position     31
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR0_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR0_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR0_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR1_width        1
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR1_position     32
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR1_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR1_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__MCSRAM_INTERRUPTS__MCSRAM_DD2_ERR1_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Interrupt State Register: arlog_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__ARLOG_INTERRUPTS__STATE_offset         (0x0E52) // Offset of State register in instance
#define MU_DCR__ARLOG_INTERRUPTS__FIRST_offset         (0x0E56) // Offset of First register in instance
#define MU_DCR__ARLOG_INTERRUPTS__FORCE_offset         (0x0E55) // Offset of Force register in instance
#define MU_DCR__ARLOG_INTERRUPTS__MACHINE_CHECK_offset (0x0E57) // Offset of Machine Check register in instance
#define MU_DCR__ARLOG_INTERRUPTS__CRITICAL_offset      (0x0E58) // Offset of Critical register in instance
#define MU_DCR__ARLOG_INTERRUPTS__NONCRITICAL_offset   (0x0E59) // Offset of Noncritical register in instance

#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_position     0
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_position     1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_position     2
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_position     3
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_position     4
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_position     5
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_position     6
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_position     7
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_position     8
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_width        1
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_position     9
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__ARLOG_INTERRUPTS__ARLOG_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)

  
// Interrupt State Register: rmu_ecc_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RMU_ECC_INTERRUPTS__STATE_offset         (0x0E5A) // Offset of State register in instance
#define MU_DCR__RMU_ECC_INTERRUPTS__FIRST_offset         (0x0E5E) // Offset of First register in instance
#define MU_DCR__RMU_ECC_INTERRUPTS__FORCE_offset         (0x0E5D) // Offset of Force register in instance
#define MU_DCR__RMU_ECC_INTERRUPTS__MACHINE_CHECK_offset (0x0E5F) // Offset of Machine Check register in instance
#define MU_DCR__RMU_ECC_INTERRUPTS__CRITICAL_offset      (0x0E60) // Offset of Critical register in instance
#define MU_DCR__RMU_ECC_INTERRUPTS__NONCRITICAL_offset   (0x0E61) // Offset of Noncritical register in instance

#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_position     0
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_position     1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_position     2
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_position     3
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_position     4
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_position     5
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_position     6
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_position     7
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_position     8
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_position     9
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_position     10
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_position     11
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_position     12
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_position     13
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_position     14
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_position     15
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_position     16
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_position     17
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_position     18
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_position     19
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_position     20
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_position     21
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_position     22
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_position     23
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_position     24
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_position     25
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_position     26
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_position     27
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE27_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_position     28
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE28_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_position     29
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE29_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_position     30
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE30_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_position     31
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE31_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_position     32
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE32_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_position     33
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_get(x)       _BGQ_GET(1,33,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_set(v)       _BGQ_SET(1,33,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE33_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_position     34
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_get(x)       _BGQ_GET(1,34,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_set(v)       _BGQ_SET(1,34,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE34_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_position     35
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_get(x)       _BGQ_GET(1,35,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_set(v)       _BGQ_SET(1,35,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE35_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_position     36
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_get(x)       _BGQ_GET(1,36,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_set(v)       _BGQ_SET(1,36,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE36_insert(x,v)  _BGQ_INSERT(1,36,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_position     37
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_get(x)       _BGQ_GET(1,37,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_set(v)       _BGQ_SET(1,37,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE37_insert(x,v)  _BGQ_INSERT(1,37,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_position     38
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_get(x)       _BGQ_GET(1,38,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_set(v)       _BGQ_SET(1,38,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE38_insert(x,v)  _BGQ_INSERT(1,38,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_width        1
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_position     39
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_get(x)       _BGQ_GET(1,39,x)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_set(v)       _BGQ_SET(1,39,v)
#define MU_DCR__RMU_ECC_INTERRUPTS__RMU_ECC_UE39_insert(x,v)  _BGQ_INSERT(1,39,x,v)

  
// Interrupt State Register: rcsram_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RCSRAM_INTERRUPTS__STATE_offset         (0x0E62) // Offset of State register in instance
#define MU_DCR__RCSRAM_INTERRUPTS__FIRST_offset         (0x0E66) // Offset of First register in instance
#define MU_DCR__RCSRAM_INTERRUPTS__FORCE_offset         (0x0E65) // Offset of Force register in instance
#define MU_DCR__RCSRAM_INTERRUPTS__MACHINE_CHECK_offset (0x0E67) // Offset of Machine Check register in instance
#define MU_DCR__RCSRAM_INTERRUPTS__CRITICAL_offset      (0x0E68) // Offset of Critical register in instance
#define MU_DCR__RCSRAM_INTERRUPTS__NONCRITICAL_offset   (0x0E69) // Offset of Noncritical register in instance

#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_position     0
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_position     1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_position     2
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_position     3
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_position     4
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_position     5
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_position     6
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_position     7
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_position     8
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_position     9
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_position     10
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_position     11
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR12_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR12_position     12
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR13_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR13_position     13
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR14_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR14_position     14
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR15_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR15_position     15
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_position     16
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_position     17
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_position     18
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_width        1
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_position     19
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__RCSRAM_INTERRUPTS__RCSRAM_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)

  
// Interrupt State Register: rputsram_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RPUTSRAM_INTERRUPTS__STATE_offset         (0x0E6B) // Offset of State register in instance
#define MU_DCR__RPUTSRAM_INTERRUPTS__FIRST_offset         (0x0E6F) // Offset of First register in instance
#define MU_DCR__RPUTSRAM_INTERRUPTS__FORCE_offset         (0x0E6E) // Offset of Force register in instance
#define MU_DCR__RPUTSRAM_INTERRUPTS__MACHINE_CHECK_offset (0x0E70) // Offset of Machine Check register in instance
#define MU_DCR__RPUTSRAM_INTERRUPTS__CRITICAL_offset      (0x0E71) // Offset of Critical register in instance
#define MU_DCR__RPUTSRAM_INTERRUPTS__NONCRITICAL_offset   (0x0E72) // Offset of Noncritical register in instance

#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_position     0
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_position     1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_position     2
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_position     3
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_position     4
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_position     5
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_position     6
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_position     7
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR8_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR8_position     8
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR9_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR9_position     9
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR10_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR10_position     10
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR11_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR11_position     11
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR12_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR12_position     12
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR13_width        1
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR13_position     13
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS__RPUTSRAM_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)

  
// Interrupt State Register: ime_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__IME_INTERRUPTS__STATE_offset         (0x0E73) // Offset of State register in instance
#define MU_DCR__IME_INTERRUPTS__FIRST_offset         (0x0E77) // Offset of First register in instance
#define MU_DCR__IME_INTERRUPTS__FORCE_offset         (0x0E76) // Offset of Force register in instance
#define MU_DCR__IME_INTERRUPTS__MACHINE_CHECK_offset (0x0E78) // Offset of Machine Check register in instance
#define MU_DCR__IME_INTERRUPTS__CRITICAL_offset      (0x0E79) // Offset of Critical register in instance
#define MU_DCR__IME_INTERRUPTS__NONCRITICAL_offset   (0x0E7A) // Offset of Noncritical register in instance

#define MU_DCR__IME_INTERRUPTS__IME_ERR0_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR0_position     0
#define MU_DCR__IME_INTERRUPTS__IME_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR1_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR1_position     1
#define MU_DCR__IME_INTERRUPTS__IME_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR2_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR2_position     2
#define MU_DCR__IME_INTERRUPTS__IME_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR3_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR3_position     3
#define MU_DCR__IME_INTERRUPTS__IME_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR4_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR4_position     4
#define MU_DCR__IME_INTERRUPTS__IME_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR5_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR5_position     5
#define MU_DCR__IME_INTERRUPTS__IME_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR6_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR6_position     6
#define MU_DCR__IME_INTERRUPTS__IME_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR7_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR7_position     7
#define MU_DCR__IME_INTERRUPTS__IME_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR8_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR8_position     8
#define MU_DCR__IME_INTERRUPTS__IME_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR9_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR9_position     9
#define MU_DCR__IME_INTERRUPTS__IME_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR10_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR10_position     10
#define MU_DCR__IME_INTERRUPTS__IME_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR11_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR11_position     11
#define MU_DCR__IME_INTERRUPTS__IME_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR12_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR12_position     12
#define MU_DCR__IME_INTERRUPTS__IME_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR13_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR13_position     13
#define MU_DCR__IME_INTERRUPTS__IME_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR14_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR14_position     14
#define MU_DCR__IME_INTERRUPTS__IME_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR15_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR15_position     15
#define MU_DCR__IME_INTERRUPTS__IME_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR16_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR16_position     16
#define MU_DCR__IME_INTERRUPTS__IME_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR17_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR17_position     17
#define MU_DCR__IME_INTERRUPTS__IME_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR18_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR18_position     18
#define MU_DCR__IME_INTERRUPTS__IME_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR19_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR19_position     19
#define MU_DCR__IME_INTERRUPTS__IME_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR20_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR20_position     20
#define MU_DCR__IME_INTERRUPTS__IME_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR21_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR21_position     21
#define MU_DCR__IME_INTERRUPTS__IME_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR22_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR22_position     22
#define MU_DCR__IME_INTERRUPTS__IME_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR23_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR23_position     23
#define MU_DCR__IME_INTERRUPTS__IME_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR24_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR24_position     24
#define MU_DCR__IME_INTERRUPTS__IME_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR25_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR25_position     25
#define MU_DCR__IME_INTERRUPTS__IME_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR26_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR26_position     26
#define MU_DCR__IME_INTERRUPTS__IME_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR27_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR27_position     27
#define MU_DCR__IME_INTERRUPTS__IME_ERR27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR27_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR28_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR28_position     28
#define MU_DCR__IME_INTERRUPTS__IME_ERR28_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR28_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR28_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR29_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR29_position     29
#define MU_DCR__IME_INTERRUPTS__IME_ERR29_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR29_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR29_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR30_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR30_position     30
#define MU_DCR__IME_INTERRUPTS__IME_ERR30_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR30_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR30_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR31_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR31_position     31
#define MU_DCR__IME_INTERRUPTS__IME_ERR31_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR31_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR31_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR32_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR32_position     32
#define MU_DCR__IME_INTERRUPTS__IME_ERR32_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR32_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR32_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR33_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR33_position     33
#define MU_DCR__IME_INTERRUPTS__IME_ERR33_get(x)       _BGQ_GET(1,33,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR33_set(v)       _BGQ_SET(1,33,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR33_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define MU_DCR__IME_INTERRUPTS__IME_ERR34_width        1
#define MU_DCR__IME_INTERRUPTS__IME_ERR34_position     34
#define MU_DCR__IME_INTERRUPTS__IME_ERR34_get(x)       _BGQ_GET(1,34,x)
#define MU_DCR__IME_INTERRUPTS__IME_ERR34_set(v)       _BGQ_SET(1,34,v)
#define MU_DCR__IME_INTERRUPTS__IME_ERR34_insert(x,v)  _BGQ_INSERT(1,34,x,v)

  
// Interrupt State Register: rme_interrupts0
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS0__STATE_offset         (0x0E8B) // Offset of State register in instance
#define MU_DCR__RME_INTERRUPTS0__FIRST_offset         (0x0E8F) // Offset of First register in instance
#define MU_DCR__RME_INTERRUPTS0__FORCE_offset         (0x0E8E) // Offset of Force register in instance
#define MU_DCR__RME_INTERRUPTS0__MACHINE_CHECK_offset (0x0E90) // Offset of Machine Check register in instance
#define MU_DCR__RME_INTERRUPTS0__CRITICAL_offset      (0x0E91) // Offset of Critical register in instance
#define MU_DCR__RME_INTERRUPTS0__NONCRITICAL_offset   (0x0E92) // Offset of Noncritical register in instance

#define MU_DCR__RME_INTERRUPTS0__RME_ERR0_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR0_position     0
#define MU_DCR__RME_INTERRUPTS0__RME_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR1_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR1_position     1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR2_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR2_position     2
#define MU_DCR__RME_INTERRUPTS0__RME_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR3_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR3_position     3
#define MU_DCR__RME_INTERRUPTS0__RME_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR4_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR4_position     4
#define MU_DCR__RME_INTERRUPTS0__RME_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR5_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR5_position     5
#define MU_DCR__RME_INTERRUPTS0__RME_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR6_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR6_position     6
#define MU_DCR__RME_INTERRUPTS0__RME_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR7_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR7_position     7
#define MU_DCR__RME_INTERRUPTS0__RME_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR8_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR8_position     8
#define MU_DCR__RME_INTERRUPTS0__RME_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR9_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR9_position     9
#define MU_DCR__RME_INTERRUPTS0__RME_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR10_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR10_position     10
#define MU_DCR__RME_INTERRUPTS0__RME_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR11_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR11_position     11
#define MU_DCR__RME_INTERRUPTS0__RME_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR12_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR12_position     12
#define MU_DCR__RME_INTERRUPTS0__RME_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR13_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR13_position     13
#define MU_DCR__RME_INTERRUPTS0__RME_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR14_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR14_position     14
#define MU_DCR__RME_INTERRUPTS0__RME_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR15_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR15_position     15
#define MU_DCR__RME_INTERRUPTS0__RME_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR16_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR16_position     16
#define MU_DCR__RME_INTERRUPTS0__RME_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR17_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR17_position     17
#define MU_DCR__RME_INTERRUPTS0__RME_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR18_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR18_position     18
#define MU_DCR__RME_INTERRUPTS0__RME_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR19_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR19_position     19
#define MU_DCR__RME_INTERRUPTS0__RME_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR20_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR20_position     20
#define MU_DCR__RME_INTERRUPTS0__RME_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR21_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR21_position     21
#define MU_DCR__RME_INTERRUPTS0__RME_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR22_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR22_position     22
#define MU_DCR__RME_INTERRUPTS0__RME_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR23_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR23_position     23
#define MU_DCR__RME_INTERRUPTS0__RME_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR24_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR24_position     24
#define MU_DCR__RME_INTERRUPTS0__RME_ERR24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR25_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR25_position     25
#define MU_DCR__RME_INTERRUPTS0__RME_ERR25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR26_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR26_position     26
#define MU_DCR__RME_INTERRUPTS0__RME_ERR26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR27_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR27_position     27
#define MU_DCR__RME_INTERRUPTS0__RME_ERR27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR27_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR28_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR28_position     28
#define MU_DCR__RME_INTERRUPTS0__RME_ERR28_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR28_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR28_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR29_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR29_position     29
#define MU_DCR__RME_INTERRUPTS0__RME_ERR29_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR29_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR29_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR30_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR30_position     30
#define MU_DCR__RME_INTERRUPTS0__RME_ERR30_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR30_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR30_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR31_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR31_position     31
#define MU_DCR__RME_INTERRUPTS0__RME_ERR31_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR31_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR31_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR32_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR32_position     32
#define MU_DCR__RME_INTERRUPTS0__RME_ERR32_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR32_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR32_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR33_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR33_position     33
#define MU_DCR__RME_INTERRUPTS0__RME_ERR33_get(x)       _BGQ_GET(1,33,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR33_set(v)       _BGQ_SET(1,33,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR33_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR34_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR34_position     34
#define MU_DCR__RME_INTERRUPTS0__RME_ERR34_get(x)       _BGQ_GET(1,34,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR34_set(v)       _BGQ_SET(1,34,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR34_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR35_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR35_position     35
#define MU_DCR__RME_INTERRUPTS0__RME_ERR35_get(x)       _BGQ_GET(1,35,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR35_set(v)       _BGQ_SET(1,35,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR35_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR36_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR36_position     36
#define MU_DCR__RME_INTERRUPTS0__RME_ERR36_get(x)       _BGQ_GET(1,36,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR36_set(v)       _BGQ_SET(1,36,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR36_insert(x,v)  _BGQ_INSERT(1,36,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR37_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR37_position     37
#define MU_DCR__RME_INTERRUPTS0__RME_ERR37_get(x)       _BGQ_GET(1,37,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR37_set(v)       _BGQ_SET(1,37,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR37_insert(x,v)  _BGQ_INSERT(1,37,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR38_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR38_position     38
#define MU_DCR__RME_INTERRUPTS0__RME_ERR38_get(x)       _BGQ_GET(1,38,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR38_set(v)       _BGQ_SET(1,38,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR38_insert(x,v)  _BGQ_INSERT(1,38,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR39_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR39_position     39
#define MU_DCR__RME_INTERRUPTS0__RME_ERR39_get(x)       _BGQ_GET(1,39,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR39_set(v)       _BGQ_SET(1,39,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR39_insert(x,v)  _BGQ_INSERT(1,39,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR40_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR40_position     40
#define MU_DCR__RME_INTERRUPTS0__RME_ERR40_get(x)       _BGQ_GET(1,40,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR40_set(v)       _BGQ_SET(1,40,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR40_insert(x,v)  _BGQ_INSERT(1,40,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR41_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR41_position     41
#define MU_DCR__RME_INTERRUPTS0__RME_ERR41_get(x)       _BGQ_GET(1,41,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR41_set(v)       _BGQ_SET(1,41,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR41_insert(x,v)  _BGQ_INSERT(1,41,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR42_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR42_position     42
#define MU_DCR__RME_INTERRUPTS0__RME_ERR42_get(x)       _BGQ_GET(1,42,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR42_set(v)       _BGQ_SET(1,42,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR42_insert(x,v)  _BGQ_INSERT(1,42,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR43_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR43_position     43
#define MU_DCR__RME_INTERRUPTS0__RME_ERR43_get(x)       _BGQ_GET(1,43,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR43_set(v)       _BGQ_SET(1,43,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR43_insert(x,v)  _BGQ_INSERT(1,43,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR44_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR44_position     44
#define MU_DCR__RME_INTERRUPTS0__RME_ERR44_get(x)       _BGQ_GET(1,44,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR44_set(v)       _BGQ_SET(1,44,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR44_insert(x,v)  _BGQ_INSERT(1,44,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR45_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR45_position     45
#define MU_DCR__RME_INTERRUPTS0__RME_ERR45_get(x)       _BGQ_GET(1,45,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR45_set(v)       _BGQ_SET(1,45,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR45_insert(x,v)  _BGQ_INSERT(1,45,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR46_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR46_position     46
#define MU_DCR__RME_INTERRUPTS0__RME_ERR46_get(x)       _BGQ_GET(1,46,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR46_set(v)       _BGQ_SET(1,46,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR46_insert(x,v)  _BGQ_INSERT(1,46,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR47_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR47_position     47
#define MU_DCR__RME_INTERRUPTS0__RME_ERR47_get(x)       _BGQ_GET(1,47,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR47_set(v)       _BGQ_SET(1,47,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR47_insert(x,v)  _BGQ_INSERT(1,47,x,v)


#define MU_DCR__RME_INTERRUPTS0__RME_ERR48_width        1
#define MU_DCR__RME_INTERRUPTS0__RME_ERR48_position     48
#define MU_DCR__RME_INTERRUPTS0__RME_ERR48_get(x)       _BGQ_GET(1,48,x)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR48_set(v)       _BGQ_SET(1,48,v)
#define MU_DCR__RME_INTERRUPTS0__RME_ERR48_insert(x,v)  _BGQ_INSERT(1,48,x,v)

  
// Interrupt State Register: rme_interrupts1
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS1__STATE_offset         (0x0E93) // Offset of State register in instance
#define MU_DCR__RME_INTERRUPTS1__FIRST_offset         (0x0E97) // Offset of First register in instance
#define MU_DCR__RME_INTERRUPTS1__FORCE_offset         (0x0E96) // Offset of Force register in instance
#define MU_DCR__RME_INTERRUPTS1__MACHINE_CHECK_offset (0x0E98) // Offset of Machine Check register in instance
#define MU_DCR__RME_INTERRUPTS1__CRITICAL_offset      (0x0E99) // Offset of Critical register in instance
#define MU_DCR__RME_INTERRUPTS1__NONCRITICAL_offset   (0x0E9A) // Offset of Noncritical register in instance

#define MU_DCR__RME_INTERRUPTS1__RME_ERR49_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR49_position     0
#define MU_DCR__RME_INTERRUPTS1__RME_ERR49_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR49_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR49_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR50_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR50_position     1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR50_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR50_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR50_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR51_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR51_position     2
#define MU_DCR__RME_INTERRUPTS1__RME_ERR51_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR51_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR51_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR52_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR52_position     3
#define MU_DCR__RME_INTERRUPTS1__RME_ERR52_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR52_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR52_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR53_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR53_position     4
#define MU_DCR__RME_INTERRUPTS1__RME_ERR53_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR53_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR53_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR54_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR54_position     5
#define MU_DCR__RME_INTERRUPTS1__RME_ERR54_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR54_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR54_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR55_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR55_position     6
#define MU_DCR__RME_INTERRUPTS1__RME_ERR55_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR55_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR55_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR56_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR56_position     7
#define MU_DCR__RME_INTERRUPTS1__RME_ERR56_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR56_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR56_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR57_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR57_position     8
#define MU_DCR__RME_INTERRUPTS1__RME_ERR57_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR57_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR57_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR58_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR58_position     9
#define MU_DCR__RME_INTERRUPTS1__RME_ERR58_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR58_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR58_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR59_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR59_position     10
#define MU_DCR__RME_INTERRUPTS1__RME_ERR59_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR59_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR59_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR60_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR60_position     11
#define MU_DCR__RME_INTERRUPTS1__RME_ERR60_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR60_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR60_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR61_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR61_position     12
#define MU_DCR__RME_INTERRUPTS1__RME_ERR61_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR61_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR61_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR62_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR62_position     13
#define MU_DCR__RME_INTERRUPTS1__RME_ERR62_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR62_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR62_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR63_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR63_position     14
#define MU_DCR__RME_INTERRUPTS1__RME_ERR63_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR63_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR63_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR64_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR64_position     15
#define MU_DCR__RME_INTERRUPTS1__RME_ERR64_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR64_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR64_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR65_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR65_position     16
#define MU_DCR__RME_INTERRUPTS1__RME_ERR65_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR65_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR65_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR66_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR66_position     17
#define MU_DCR__RME_INTERRUPTS1__RME_ERR66_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR66_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR66_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR67_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR67_position     18
#define MU_DCR__RME_INTERRUPTS1__RME_ERR67_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR67_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR67_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR68_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR68_position     19
#define MU_DCR__RME_INTERRUPTS1__RME_ERR68_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR68_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR68_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR69_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR69_position     20
#define MU_DCR__RME_INTERRUPTS1__RME_ERR69_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR69_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR69_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR70_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR70_position     21
#define MU_DCR__RME_INTERRUPTS1__RME_ERR70_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR70_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR70_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR71_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR71_position     22
#define MU_DCR__RME_INTERRUPTS1__RME_ERR71_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR71_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR71_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR72_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR72_position     23
#define MU_DCR__RME_INTERRUPTS1__RME_ERR72_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR72_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR72_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR73_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR73_position     24
#define MU_DCR__RME_INTERRUPTS1__RME_ERR73_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR73_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR73_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR74_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR74_position     25
#define MU_DCR__RME_INTERRUPTS1__RME_ERR74_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR74_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR74_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR75_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR75_position     26
#define MU_DCR__RME_INTERRUPTS1__RME_ERR75_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR75_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR75_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR76_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR76_position     27
#define MU_DCR__RME_INTERRUPTS1__RME_ERR76_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR76_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR76_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR77_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR77_position     28
#define MU_DCR__RME_INTERRUPTS1__RME_ERR77_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR77_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR77_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR78_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR78_position     29
#define MU_DCR__RME_INTERRUPTS1__RME_ERR78_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR78_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR78_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR79_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR79_position     30
#define MU_DCR__RME_INTERRUPTS1__RME_ERR79_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR79_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR79_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR80_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR80_position     31
#define MU_DCR__RME_INTERRUPTS1__RME_ERR80_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR80_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR80_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR81_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR81_position     32
#define MU_DCR__RME_INTERRUPTS1__RME_ERR81_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR81_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR81_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR82_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR82_position     33
#define MU_DCR__RME_INTERRUPTS1__RME_ERR82_get(x)       _BGQ_GET(1,33,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR82_set(v)       _BGQ_SET(1,33,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR82_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR83_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR83_position     34
#define MU_DCR__RME_INTERRUPTS1__RME_ERR83_get(x)       _BGQ_GET(1,34,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR83_set(v)       _BGQ_SET(1,34,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR83_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR84_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR84_position     35
#define MU_DCR__RME_INTERRUPTS1__RME_ERR84_get(x)       _BGQ_GET(1,35,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR84_set(v)       _BGQ_SET(1,35,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR84_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR85_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR85_position     36
#define MU_DCR__RME_INTERRUPTS1__RME_ERR85_get(x)       _BGQ_GET(1,36,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR85_set(v)       _BGQ_SET(1,36,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR85_insert(x,v)  _BGQ_INSERT(1,36,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR86_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR86_position     37
#define MU_DCR__RME_INTERRUPTS1__RME_ERR86_get(x)       _BGQ_GET(1,37,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR86_set(v)       _BGQ_SET(1,37,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR86_insert(x,v)  _BGQ_INSERT(1,37,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR87_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR87_position     38
#define MU_DCR__RME_INTERRUPTS1__RME_ERR87_get(x)       _BGQ_GET(1,38,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR87_set(v)       _BGQ_SET(1,38,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR87_insert(x,v)  _BGQ_INSERT(1,38,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR88_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR88_position     39
#define MU_DCR__RME_INTERRUPTS1__RME_ERR88_get(x)       _BGQ_GET(1,39,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR88_set(v)       _BGQ_SET(1,39,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR88_insert(x,v)  _BGQ_INSERT(1,39,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR89_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR89_position     40
#define MU_DCR__RME_INTERRUPTS1__RME_ERR89_get(x)       _BGQ_GET(1,40,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR89_set(v)       _BGQ_SET(1,40,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR89_insert(x,v)  _BGQ_INSERT(1,40,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR90_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR90_position     41
#define MU_DCR__RME_INTERRUPTS1__RME_ERR90_get(x)       _BGQ_GET(1,41,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR90_set(v)       _BGQ_SET(1,41,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR90_insert(x,v)  _BGQ_INSERT(1,41,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR91_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR91_position     42
#define MU_DCR__RME_INTERRUPTS1__RME_ERR91_get(x)       _BGQ_GET(1,42,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR91_set(v)       _BGQ_SET(1,42,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR91_insert(x,v)  _BGQ_INSERT(1,42,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR92_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR92_position     43
#define MU_DCR__RME_INTERRUPTS1__RME_ERR92_get(x)       _BGQ_GET(1,43,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR92_set(v)       _BGQ_SET(1,43,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR92_insert(x,v)  _BGQ_INSERT(1,43,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR93_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR93_position     44
#define MU_DCR__RME_INTERRUPTS1__RME_ERR93_get(x)       _BGQ_GET(1,44,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR93_set(v)       _BGQ_SET(1,44,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR93_insert(x,v)  _BGQ_INSERT(1,44,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR94_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR94_position     45
#define MU_DCR__RME_INTERRUPTS1__RME_ERR94_get(x)       _BGQ_GET(1,45,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR94_set(v)       _BGQ_SET(1,45,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR94_insert(x,v)  _BGQ_INSERT(1,45,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR95_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR95_position     46
#define MU_DCR__RME_INTERRUPTS1__RME_ERR95_get(x)       _BGQ_GET(1,46,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR95_set(v)       _BGQ_SET(1,46,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR95_insert(x,v)  _BGQ_INSERT(1,46,x,v)


#define MU_DCR__RME_INTERRUPTS1__RME_ERR96_width        1
#define MU_DCR__RME_INTERRUPTS1__RME_ERR96_position     47
#define MU_DCR__RME_INTERRUPTS1__RME_ERR96_get(x)       _BGQ_GET(1,47,x)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR96_set(v)       _BGQ_SET(1,47,v)
#define MU_DCR__RME_INTERRUPTS1__RME_ERR96_insert(x,v)  _BGQ_INSERT(1,47,x,v)

  
// Interrupt State Register: icsram_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__ICSRAM_INTERRUPTS__STATE_offset         (0x0EBB) // Offset of State register in instance
#define MU_DCR__ICSRAM_INTERRUPTS__FIRST_offset         (0x0EBF) // Offset of First register in instance
#define MU_DCR__ICSRAM_INTERRUPTS__FORCE_offset         (0x0EBE) // Offset of Force register in instance
#define MU_DCR__ICSRAM_INTERRUPTS__MACHINE_CHECK_offset (0x0EC0) // Offset of Machine Check register in instance
#define MU_DCR__ICSRAM_INTERRUPTS__CRITICAL_offset      (0x0EC1) // Offset of Critical register in instance
#define MU_DCR__ICSRAM_INTERRUPTS__NONCRITICAL_offset   (0x0EC2) // Offset of Noncritical register in instance

#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_position     0
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_position     1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_position     2
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_position     3
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_position     4
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_position     5
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_position     6
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_position     7
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_position     8
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_position     9
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_position     10
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_position     11
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_position     12
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_position     13
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_position     14
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_position     15
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_position     16
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_position     17
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_position     18
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_position     19
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR20_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR20_position     20
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR21_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR21_position     21
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR22_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR22_position     22
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR23_width        1
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR23_position     23
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__ICSRAM_INTERRUPTS__ICSRAM_ERR23_insert(x,v)  _BGQ_INSERT(1,23,x,v)

  
// Interrupt State Register: misc_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MISC_INTERRUPTS__STATE_offset         (0x0EC4) // Offset of State register in instance
#define MU_DCR__MISC_INTERRUPTS__FIRST_offset         (0x0EC8) // Offset of First register in instance
#define MU_DCR__MISC_INTERRUPTS__FORCE_offset         (0x0EC7) // Offset of Force register in instance
#define MU_DCR__MISC_INTERRUPTS__MACHINE_CHECK_offset (0x0EC9) // Offset of Machine Check register in instance
#define MU_DCR__MISC_INTERRUPTS__CRITICAL_offset      (0x0ECA) // Offset of Critical register in instance
#define MU_DCR__MISC_INTERRUPTS__NONCRITICAL_offset   (0x0ECB) // Offset of Noncritical register in instance

#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_position     0
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_position     1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_position     2
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR3_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR3_position     3
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR4_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR4_position     4
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR5_width        1
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR5_position     5
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MISC_INTERRUPTS__ICSRAM_SELECT_ERR5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__MISC_INTERRUPTS__RAN_ERR0_width        1
#define MU_DCR__MISC_INTERRUPTS__RAN_ERR0_position     6
#define MU_DCR__MISC_INTERRUPTS__RAN_ERR0_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__MISC_INTERRUPTS__RAN_ERR0_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__MISC_INTERRUPTS__RAN_ERR0_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__MISC_INTERRUPTS__MU_ERR0_width        1
#define MU_DCR__MISC_INTERRUPTS__MU_ERR0_position     7
#define MU_DCR__MISC_INTERRUPTS__MU_ERR0_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__MISC_INTERRUPTS__MU_ERR0_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__MISC_INTERRUPTS__MU_ERR0_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__MISC_INTERRUPTS__MU_ERR1_width        1
#define MU_DCR__MISC_INTERRUPTS__MU_ERR1_position     8
#define MU_DCR__MISC_INTERRUPTS__MU_ERR1_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__MISC_INTERRUPTS__MU_ERR1_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__MISC_INTERRUPTS__MU_ERR1_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_width        1
#define MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_position     9
#define MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__MISC_INTERRUPTS__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,9,x,v)

  
// Interrupt State Register: rmu_ecc_corr_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__STATE_offset         (0x0ECD) // Offset of State register in instance
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__FIRST_offset         (0x0ED1) // Offset of First register in instance
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__FORCE_offset         (0x0ED0) // Offset of Force register in instance
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__MACHINE_CHECK_offset (0x0ED2) // Offset of Machine Check register in instance
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__CRITICAL_offset      (0x0ED3) // Offset of Critical register in instance
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__NONCRITICAL_offset   (0x0ED4) // Offset of Noncritical register in instance

#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_position     0
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_position     1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_position     2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_position     3
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_position     4
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_position     5
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_position     6
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_position     7
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE7_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_position     8
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_get(x)       _BGQ_GET(1,8,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_set(v)       _BGQ_SET(1,8,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE8_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_position     9
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_get(x)       _BGQ_GET(1,9,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_set(v)       _BGQ_SET(1,9,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE9_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_position     10
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_get(x)       _BGQ_GET(1,10,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_set(v)       _BGQ_SET(1,10,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE10_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_position     11
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_get(x)       _BGQ_GET(1,11,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_set(v)       _BGQ_SET(1,11,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE11_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_position     12
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_get(x)       _BGQ_GET(1,12,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_set(v)       _BGQ_SET(1,12,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE12_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_position     13
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_get(x)       _BGQ_GET(1,13,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_set(v)       _BGQ_SET(1,13,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE13_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_position     14
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_get(x)       _BGQ_GET(1,14,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_set(v)       _BGQ_SET(1,14,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE14_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_position     15
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_get(x)       _BGQ_GET(1,15,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_set(v)       _BGQ_SET(1,15,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE15_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_position     16
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_get(x)       _BGQ_GET(1,16,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_set(v)       _BGQ_SET(1,16,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE16_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_position     17
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_get(x)       _BGQ_GET(1,17,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_set(v)       _BGQ_SET(1,17,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE17_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_position     18
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_get(x)       _BGQ_GET(1,18,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_set(v)       _BGQ_SET(1,18,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE18_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_position     19
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_get(x)       _BGQ_GET(1,19,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_set(v)       _BGQ_SET(1,19,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE19_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_position     20
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_get(x)       _BGQ_GET(1,20,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_set(v)       _BGQ_SET(1,20,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE20_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_position     21
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_get(x)       _BGQ_GET(1,21,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_set(v)       _BGQ_SET(1,21,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE21_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_position     22
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_get(x)       _BGQ_GET(1,22,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_set(v)       _BGQ_SET(1,22,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE22_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_position     23
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_get(x)       _BGQ_GET(1,23,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_set(v)       _BGQ_SET(1,23,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE23_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_position     24
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_get(x)       _BGQ_GET(1,24,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_set(v)       _BGQ_SET(1,24,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE24_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_position     25
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_get(x)       _BGQ_GET(1,25,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_set(v)       _BGQ_SET(1,25,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE25_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_position     26
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_get(x)       _BGQ_GET(1,26,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_set(v)       _BGQ_SET(1,26,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE26_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_position     27
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_get(x)       _BGQ_GET(1,27,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_set(v)       _BGQ_SET(1,27,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE27_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_position     28
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_get(x)       _BGQ_GET(1,28,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_set(v)       _BGQ_SET(1,28,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE28_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_position     29
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_get(x)       _BGQ_GET(1,29,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_set(v)       _BGQ_SET(1,29,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE29_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_position     30
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_get(x)       _BGQ_GET(1,30,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_set(v)       _BGQ_SET(1,30,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE30_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_position     31
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_get(x)       _BGQ_GET(1,31,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_set(v)       _BGQ_SET(1,31,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE31_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_position     32
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_get(x)       _BGQ_GET(1,32,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_set(v)       _BGQ_SET(1,32,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE32_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_position     33
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_get(x)       _BGQ_GET(1,33,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_set(v)       _BGQ_SET(1,33,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE33_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_position     34
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_get(x)       _BGQ_GET(1,34,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_set(v)       _BGQ_SET(1,34,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE34_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_position     35
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_get(x)       _BGQ_GET(1,35,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_set(v)       _BGQ_SET(1,35,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE35_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_position     36
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_get(x)       _BGQ_GET(1,36,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_set(v)       _BGQ_SET(1,36,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE36_insert(x,v)  _BGQ_INSERT(1,36,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_position     37
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_get(x)       _BGQ_GET(1,37,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_set(v)       _BGQ_SET(1,37,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE37_insert(x,v)  _BGQ_INSERT(1,37,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_position     38
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_get(x)       _BGQ_GET(1,38,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_set(v)       _BGQ_SET(1,38,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE38_insert(x,v)  _BGQ_INSERT(1,38,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_width        1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_position     39
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_get(x)       _BGQ_GET(1,39,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_set(v)       _BGQ_SET(1,39,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS__RMU_ECC_CE39_insert(x,v)  _BGQ_INSERT(1,39,x,v)

  
// Interrupt State Register: misc_ecc_corr_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__STATE_offset         (0x0ED5) // Offset of State register in instance
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__FIRST_offset         (0x0ED9) // Offset of First register in instance
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__FORCE_offset         (0x0ED8) // Offset of Force register in instance
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MACHINE_CHECK_offset (0x0EDA) // Offset of Machine Check register in instance
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__CRITICAL_offset      (0x0EDB) // Offset of Critical register in instance
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__NONCRITICAL_offset   (0x0EDC) // Offset of Noncritical register in instance

#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_position     0
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_position     1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_position     2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__MMREGS_ECC_CE2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_position     3
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE0_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_position     4
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE1_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_width        1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_position     5
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS__XS_ECC_CE2_insert(x,v)  _BGQ_INSERT(1,5,x,v)

  
// Interrupt State Register: ecc_count_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define MU_DCR__ECC_COUNT_INTERRUPTS__STATE_offset         (0x0EDD) // Offset of State register in instance
#define MU_DCR__ECC_COUNT_INTERRUPTS__FIRST_offset         (0x0EE1) // Offset of First register in instance
#define MU_DCR__ECC_COUNT_INTERRUPTS__FORCE_offset         (0x0EE0) // Offset of Force register in instance
#define MU_DCR__ECC_COUNT_INTERRUPTS__MACHINE_CHECK_offset (0x0EE2) // Offset of Machine Check register in instance
#define MU_DCR__ECC_COUNT_INTERRUPTS__CRITICAL_offset      (0x0EE3) // Offset of Critical register in instance
#define MU_DCR__ECC_COUNT_INTERRUPTS__NONCRITICAL_offset   (0x0EE4) // Offset of Noncritical register in instance

#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_position     0
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_get(x)       _BGQ_GET(1,0,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_set(v)       _BGQ_SET(1,0,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER0_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_position     1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_get(x)       _BGQ_GET(1,1,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_set(v)       _BGQ_SET(1,1,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER1_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_position     2
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_get(x)       _BGQ_GET(1,2,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_set(v)       _BGQ_SET(1,2,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER2_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_position     3
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_get(x)       _BGQ_GET(1,3,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_set(v)       _BGQ_SET(1,3,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER3_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_position     4
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_get(x)       _BGQ_GET(1,4,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_set(v)       _BGQ_SET(1,4,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER4_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_position     5
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_get(x)       _BGQ_GET(1,5,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_set(v)       _BGQ_SET(1,5,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER5_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_position     6
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_get(x)       _BGQ_GET(1,6,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_set(v)       _BGQ_SET(1,6,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER6_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_width        1
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_position     7
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_get(x)       _BGQ_GET(1,7,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_set(v)       _BGQ_SET(1,7,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS__ECC_COUNT_OVER7_insert(x,v)  _BGQ_INSERT(1,7,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0E01) // Offset of Control register in instance

#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

  
// Interrupt Control Register: master_port0_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_LOW_offset         (0x0E0B) // Offset of Control register in instance
  
// Interrupt Control Register: master_port0_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH_offset         (0x0E0C) // Offset of Control register in instance

#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR0_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR0_position     1
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR1_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR1_position     3
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR2_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR2_position     5
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR3_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR3_position     7
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR4_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR4_position     9
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR5_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR5_position     11
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR6_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR6_position     13
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR7_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR7_position     15
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR8_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR8_position     17
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR9_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR9_position     19
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR10_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR10_position     21
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR11_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR11_position     23
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR12_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR12_position     25
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR13_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR13_position     27
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR14_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR14_position     29
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR15_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR15_position     31
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR16_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR16_position     33
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR17_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR17_position     35
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR18_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR18_position     37
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR19_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR19_position     39
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR20_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR20_position     41
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR21_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR21_position     43
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR22_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR22_position     45
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR23_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR23_position     47
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR24_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR24_position     49
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR25_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR25_position     51
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_CORR_ERR26_width        2
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_CORR_ERR26_position     53
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_CORR_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_CORR_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__MASTER_PORT0_INTERRUPTS_CONTROL_HIGH__XM0_CORR_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)

  
// Interrupt Control Register: master_port1_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_LOW_offset         (0x0E14) // Offset of Control register in instance
  
// Interrupt Control Register: master_port1_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH_offset         (0x0E15) // Offset of Control register in instance

#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR0_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR0_position     1
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR1_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR1_position     3
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR2_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR2_position     5
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR3_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR3_position     7
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR4_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR4_position     9
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR5_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR5_position     11
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR6_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR6_position     13
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR7_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR7_position     15
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR8_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR8_position     17
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR9_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR9_position     19
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR10_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR10_position     21
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR11_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR11_position     23
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR12_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR12_position     25
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR13_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR13_position     27
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR14_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR14_position     29
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR15_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR15_position     31
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR16_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR16_position     33
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR17_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR17_position     35
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR18_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR18_position     37
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR19_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR19_position     39
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR20_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR20_position     41
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR21_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR21_position     43
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR22_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR22_position     45
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR23_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR23_position     47
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR24_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR24_position     49
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR25_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR25_position     51
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_CORR_ERR26_width        2
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_CORR_ERR26_position     53
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_CORR_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_CORR_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__MASTER_PORT1_INTERRUPTS_CONTROL_HIGH__XM1_CORR_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)

  
// Interrupt Control Register: master_port2_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_LOW_offset         (0x0E1D) // Offset of Control register in instance
  
// Interrupt Control Register: master_port2_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH_offset         (0x0E1E) // Offset of Control register in instance

#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR0_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR0_position     1
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR1_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR1_position     3
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR2_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR2_position     5
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR3_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR3_position     7
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR4_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR4_position     9
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR5_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR5_position     11
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR6_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR6_position     13
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR7_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR7_position     15
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR8_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR8_position     17
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR9_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR9_position     19
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR10_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR10_position     21
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR11_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR11_position     23
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR12_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR12_position     25
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR13_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR13_position     27
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR14_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR14_position     29
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR15_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR15_position     31
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR16_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR16_position     33
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR17_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR17_position     35
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR18_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR18_position     37
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR19_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR19_position     39
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR20_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR20_position     41
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR21_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR21_position     43
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR22_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR22_position     45
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR23_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR23_position     47
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_FATAL_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR24_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR24_position     49
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR25_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR25_position     51
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_CORR_ERR26_width        2
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_CORR_ERR26_position     53
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_CORR_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_CORR_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__MASTER_PORT2_INTERRUPTS_CONTROL_HIGH__XM2_CORR_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)

  
// Interrupt Control Register: slave_port_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_LOW_offset         (0x0E26) // Offset of Control register in instance
  
// Interrupt Control Register: slave_port_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH_offset         (0x0E27) // Offset of Control register in instance

#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR0_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR0_position     1
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR1_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR1_position     3
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR2_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR2_position     5
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR3_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR3_position     7
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR4_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR4_position     9
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR5_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR5_position     11
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR6_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR6_position     13
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR7_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR7_position     15
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR8_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR8_position     17
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR9_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR9_position     19
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR10_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR10_position     21
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR11_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR11_position     23
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR12_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR12_position     25
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR13_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR13_position     27
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR14_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR14_position     29
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR15_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR15_position     31
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR16_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR16_position     33
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR17_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR17_position     35
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR18_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR18_position     37
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR19_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR19_position     39
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR20_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR20_position     41
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_FATAL_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR21_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR21_position     43
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR22_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR22_position     45
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR23_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR23_position     47
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR24_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR24_position     49
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR25_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR25_position     51
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR26_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR26_position     53
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR27_width        2
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR27_position     55
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__SLAVE_PORT_INTERRUPTS_CONTROL_HIGH__XS_SOFTWARE_ERR27_insert(x,v)  _BGQ_INSERT(2,55,x,v)

  
// Interrupt Control Register: mmregs_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_LOW_offset         (0x0E2F) // Offset of Control register in instance
  
// Interrupt Control Register: mmregs_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH_offset         (0x0E30) // Offset of Control register in instance

#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR0_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR0_position     1
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR1_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR1_position     3
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR2_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR2_position     5
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR3_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR3_position     7
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR4_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR4_position     9
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR5_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR5_position     11
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR6_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR6_position     13
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR7_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR7_position     15
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR8_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR8_position     17
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_FATAL_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR9_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR9_position     19
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR10_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR10_position     21
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR11_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR11_position     23
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR12_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR12_position     25
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR13_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR13_position     27
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR14_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR14_position     29
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR15_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR15_position     31
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR16_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR16_position     33
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR17_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR17_position     35
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR18_width        2
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR18_position     37
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__MMREGS_INTERRUPTS_CONTROL_HIGH__MMREGS_SOFTWARE_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)

  
// Interrupt Control Register: fifo_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW_offset         (0x0E38) // Offset of Control register in instance

#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__LOCAL_RING_width        2
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__LOCAL_RING_position     59
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__LOCAL_RING_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__LOCAL_RING_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__RMFIFO_INSUFFICIENT_SPACE_width        2
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__RMFIFO_INSUFFICIENT_SPACE_position     63
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__RMFIFO_INSUFFICIENT_SPACE_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__RMFIFO_INSUFFICIENT_SPACE_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_LOW__RMFIFO_INSUFFICIENT_SPACE_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: fifo_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__FIFO_INTERRUPTS_CONTROL_HIGH_offset         (0x0E39) // Offset of Control register in instance
  
// Interrupt Control Register: imu_ecc_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW_offset         (0x0E41) // Offset of Control register in instance

#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW__IMU_ECC_CE21_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW__IMU_ECC_CE21_position     1
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW__IMU_ECC_CE21_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW__IMU_ECC_CE21_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_LOW__IMU_ECC_CE21_insert(x,v)  _BGQ_INSERT(2,1,x,v)

  
// Interrupt Control Register: imu_ecc_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH_offset         (0x0E42) // Offset of Control register in instance

#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE0_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE0_position     1
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE1_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE1_position     3
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE2_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE2_position     5
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE3_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE3_position     7
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE4_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE4_position     9
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE5_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE5_position     11
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE6_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE6_position     13
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE7_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE7_position     15
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE8_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE8_position     17
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE9_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE9_position     19
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE10_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE10_position     21
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_UE10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE0_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE0_position     23
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE0_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE0_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE0_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE1_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE1_position     25
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE1_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE1_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE1_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE2_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE2_position     27
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE2_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE2_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE2_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE3_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE3_position     29
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE3_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE3_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE3_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE4_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE4_position     31
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE4_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE4_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE4_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE5_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE5_position     33
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE5_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE5_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE5_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE6_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE6_position     35
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE6_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE6_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE6_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE7_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE7_position     37
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE7_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE7_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE7_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE8_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE8_position     39
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE8_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE8_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE8_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE9_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE9_position     41
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE9_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE9_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE9_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE10_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE10_position     43
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE10_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE10_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE10_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE11_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE11_position     45
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE11_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE11_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE11_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE12_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE12_position     47
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE12_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE12_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE12_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE13_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE13_position     49
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE13_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE13_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE13_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE14_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE14_position     51
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE14_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE14_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE14_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE15_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE15_position     53
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE15_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE15_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE15_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE16_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE16_position     55
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE16_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE16_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE16_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE17_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE17_position     57
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE17_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE17_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE17_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE18_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE18_position     59
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE18_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE18_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE18_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE19_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE19_position     61
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE19_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE19_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE19_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE20_width        2
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE20_position     63
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE20_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE20_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__IMU_ECC_INTERRUPTS_CONTROL_HIGH__IMU_ECC_CE20_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: mcsram_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW_offset         (0x0E49) // Offset of Control register in instance

#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW__MCSRAM_DD2_ERR1_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW__MCSRAM_DD2_ERR1_position     1
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW__MCSRAM_DD2_ERR1_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW__MCSRAM_DD2_ERR1_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_LOW__MCSRAM_DD2_ERR1_insert(x,v)  _BGQ_INSERT(2,1,x,v)

  
// Interrupt Control Register: mcsram_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH_offset         (0x0E4A) // Offset of Control register in instance

#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR0_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR0_position     1
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR1_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR1_position     3
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR2_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR2_position     5
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR3_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR3_position     7
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR4_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR4_position     9
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR5_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR5_position     11
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR6_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR6_position     13
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR7_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR7_position     15
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR8_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR8_position     17
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR9_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR9_position     19
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR10_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR10_position     21
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR11_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR11_position     23
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR12_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR12_position     25
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR13_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR13_position     27
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR14_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR14_position     29
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR15_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR15_position     31
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR16_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR16_position     33
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR17_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR17_position     35
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR18_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR18_position     37
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR19_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR19_position     39
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR20_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR20_position     41
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR21_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR21_position     43
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR22_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR22_position     45
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR23_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR23_position     47
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR24_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR24_position     49
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR25_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR25_position     51
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR26_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR26_position     53
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR27_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR27_position     55
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR27_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR28_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR28_position     57
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR28_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR28_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR28_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR29_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR29_position     59
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR29_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR29_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR29_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR30_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR30_position     61
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR30_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR30_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_ERR30_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_DD2_ERR0_width        2
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_DD2_ERR0_position     63
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_DD2_ERR0_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_DD2_ERR0_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__MCSRAM_INTERRUPTS_CONTROL_HIGH__MCSRAM_DD2_ERR0_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: arlog_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_LOW_offset         (0x0E53) // Offset of Control register in instance
  
// Interrupt Control Register: arlog_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH_offset         (0x0E54) // Offset of Control register in instance

#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR0_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR0_position     1
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR1_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR1_position     3
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR2_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR2_position     5
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR3_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR3_position     7
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR4_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR4_position     9
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR5_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR5_position     11
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR6_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR6_position     13
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR7_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR7_position     15
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR8_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR8_position     17
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR9_width        2
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR9_position     19
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__ARLOG_INTERRUPTS_CONTROL_HIGH__ARLOG_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)

  
// Interrupt Control Register: rmu_ecc_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW_offset         (0x0E5B) // Offset of Control register in instance

#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE32_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE32_position     1
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE32_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE32_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE32_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE33_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE33_position     3
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE33_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE33_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE33_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE34_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE34_position     5
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE34_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE34_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE34_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE35_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE35_position     7
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE35_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE35_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE35_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE36_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE36_position     9
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE36_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE36_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE36_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE37_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE37_position     11
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE37_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE37_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE37_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE38_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE38_position     13
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE38_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE38_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE38_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE39_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE39_position     15
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE39_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE39_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_LOW__RMU_ECC_UE39_insert(x,v)  _BGQ_INSERT(2,15,x,v)

  
// Interrupt Control Register: rmu_ecc_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH_offset         (0x0E5C) // Offset of Control register in instance

#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE0_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE0_position     1
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE1_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE1_position     3
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE2_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE2_position     5
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE3_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE3_position     7
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE4_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE4_position     9
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE5_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE5_position     11
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE6_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE6_position     13
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE7_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE7_position     15
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE8_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE8_position     17
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE9_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE9_position     19
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE10_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE10_position     21
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE11_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE11_position     23
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE12_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE12_position     25
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE13_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE13_position     27
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE14_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE14_position     29
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE15_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE15_position     31
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE16_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE16_position     33
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE17_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE17_position     35
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE18_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE18_position     37
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE19_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE19_position     39
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE20_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE20_position     41
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE21_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE21_position     43
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE22_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE22_position     45
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE23_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE23_position     47
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE24_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE24_position     49
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE25_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE25_position     51
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE26_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE26_position     53
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE27_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE27_position     55
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE27_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE28_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE28_position     57
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE28_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE28_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE28_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE29_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE29_position     59
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE29_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE29_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE29_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE30_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE30_position     61
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE30_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE30_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE30_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE31_width        2
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE31_position     63
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE31_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE31_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__RMU_ECC_INTERRUPTS_CONTROL_HIGH__RMU_ECC_UE31_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: rcsram_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_LOW_offset         (0x0E63) // Offset of Control register in instance
  
// Interrupt Control Register: rcsram_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH_offset         (0x0E64) // Offset of Control register in instance

#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR0_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR0_position     1
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR1_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR1_position     3
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR2_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR2_position     5
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR3_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR3_position     7
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR4_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR4_position     9
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR5_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR5_position     11
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR6_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR6_position     13
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR7_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR7_position     15
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR8_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR8_position     17
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR9_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR9_position     19
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR10_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR10_position     21
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR11_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR11_position     23
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR12_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR12_position     25
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR13_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR13_position     27
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR14_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR14_position     29
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR15_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR15_position     31
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR16_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR16_position     33
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR17_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR17_position     35
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR18_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR18_position     37
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR19_width        2
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR19_position     39
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__RCSRAM_INTERRUPTS_CONTROL_HIGH__RCSRAM_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)

  
// Interrupt Control Register: rputsram_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_LOW_offset         (0x0E6C) // Offset of Control register in instance
  
// Interrupt Control Register: rputsram_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH_offset         (0x0E6D) // Offset of Control register in instance

#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR0_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR0_position     1
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR1_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR1_position     3
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR2_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR2_position     5
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR3_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR3_position     7
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR4_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR4_position     9
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR5_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR5_position     11
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR6_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR6_position     13
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR7_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR7_position     15
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR8_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR8_position     17
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR9_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR9_position     19
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR10_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR10_position     21
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR11_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR11_position     23
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR12_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR12_position     25
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR13_width        2
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR13_position     27
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RPUTSRAM_INTERRUPTS_CONTROL_HIGH__RPUTSRAM_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)

  
// Interrupt Control Register: ime_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW_offset         (0x0E74) // Offset of Control register in instance

#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR32_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR32_position     1
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR32_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR32_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR32_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR33_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR33_position     3
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR33_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR33_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR33_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR34_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR34_position     5
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR34_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR34_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_LOW__IME_ERR34_insert(x,v)  _BGQ_INSERT(2,5,x,v)

  
// Interrupt Control Register: ime_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH_offset         (0x0E75) // Offset of Control register in instance

#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR0_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR0_position     1
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR1_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR1_position     3
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR2_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR2_position     5
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR3_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR3_position     7
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR4_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR4_position     9
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR5_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR5_position     11
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR6_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR6_position     13
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR7_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR7_position     15
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR8_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR8_position     17
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR9_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR9_position     19
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR10_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR10_position     21
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR11_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR11_position     23
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR12_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR12_position     25
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR13_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR13_position     27
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR14_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR14_position     29
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR15_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR15_position     31
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR16_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR16_position     33
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR17_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR17_position     35
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR18_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR18_position     37
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR19_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR19_position     39
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR20_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR20_position     41
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR21_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR21_position     43
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR22_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR22_position     45
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR23_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR23_position     47
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR24_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR24_position     49
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR25_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR25_position     51
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR26_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR26_position     53
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR27_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR27_position     55
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR27_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR28_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR28_position     57
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR28_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR28_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR28_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR29_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR29_position     59
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR29_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR29_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR29_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR30_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR30_position     61
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR30_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR30_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR30_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR31_width        2
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR31_position     63
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR31_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR31_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__IME_INTERRUPTS_CONTROL_HIGH__IME_ERR31_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: rme_interrupts0_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW_offset         (0x0E8C) // Offset of Control register in instance

#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR32_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR32_position     1
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR32_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR32_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR32_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR33_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR33_position     3
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR33_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR33_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR33_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR34_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR34_position     5
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR34_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR34_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR34_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR35_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR35_position     7
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR35_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR35_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR35_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR36_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR36_position     9
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR36_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR36_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR36_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR37_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR37_position     11
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR37_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR37_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR37_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR38_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR38_position     13
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR38_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR38_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR38_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR39_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR39_position     15
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR39_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR39_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR39_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR40_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR40_position     17
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR40_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR40_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR40_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR41_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR41_position     19
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR41_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR41_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR41_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR42_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR42_position     21
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR42_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR42_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR42_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR43_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR43_position     23
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR43_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR43_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR43_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR44_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR44_position     25
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR44_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR44_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR44_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR45_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR45_position     27
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR45_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR45_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR45_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR46_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR46_position     29
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR46_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR46_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR46_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR47_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR47_position     31
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR47_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR47_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR47_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR48_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR48_position     33
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR48_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR48_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_LOW__RME_ERR48_insert(x,v)  _BGQ_INSERT(2,33,x,v)

  
// Interrupt Control Register: rme_interrupts0_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH_offset         (0x0E8D) // Offset of Control register in instance

#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR0_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR0_position     1
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR1_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR1_position     3
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR2_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR2_position     5
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR3_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR3_position     7
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR4_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR4_position     9
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR5_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR5_position     11
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR6_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR6_position     13
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR7_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR7_position     15
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR8_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR8_position     17
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR9_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR9_position     19
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR10_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR10_position     21
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR11_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR11_position     23
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR12_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR12_position     25
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR13_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR13_position     27
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR14_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR14_position     29
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR15_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR15_position     31
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR16_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR16_position     33
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR17_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR17_position     35
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR18_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR18_position     37
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR19_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR19_position     39
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR20_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR20_position     41
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR21_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR21_position     43
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR22_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR22_position     45
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR23_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR23_position     47
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR24_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR24_position     49
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR25_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR25_position     51
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR26_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR26_position     53
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR27_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR27_position     55
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR27_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR28_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR28_position     57
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR28_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR28_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR28_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR29_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR29_position     59
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR29_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR29_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR29_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR30_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR30_position     61
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR30_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR30_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR30_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR31_width        2
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR31_position     63
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR31_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR31_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__RME_INTERRUPTS0_CONTROL_HIGH__RME_ERR31_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: rme_interrupts1_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW_offset         (0x0E94) // Offset of Control register in instance

#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR81_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR81_position     1
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR81_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR81_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR81_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR82_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR82_position     3
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR82_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR82_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR82_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR83_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR83_position     5
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR83_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR83_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR83_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR84_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR84_position     7
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR84_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR84_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR84_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR85_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR85_position     9
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR85_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR85_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR85_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR86_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR86_position     11
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR86_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR86_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR86_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR87_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR87_position     13
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR87_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR87_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR87_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR88_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR88_position     15
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR88_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR88_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR88_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR89_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR89_position     17
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR89_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR89_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR89_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR90_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR90_position     19
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR90_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR90_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR90_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR91_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR91_position     21
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR91_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR91_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR91_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR92_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR92_position     23
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR92_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR92_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR92_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR93_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR93_position     25
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR93_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR93_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR93_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR94_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR94_position     27
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR94_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR94_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR94_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR95_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR95_position     29
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR95_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR95_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR95_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR96_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR96_position     31
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR96_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR96_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_LOW__RME_ERR96_insert(x,v)  _BGQ_INSERT(2,31,x,v)

  
// Interrupt Control Register: rme_interrupts1_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH_offset         (0x0E95) // Offset of Control register in instance

#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR49_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR49_position     1
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR49_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR49_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR49_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR50_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR50_position     3
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR50_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR50_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR50_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR51_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR51_position     5
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR51_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR51_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR51_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR52_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR52_position     7
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR52_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR52_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR52_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR53_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR53_position     9
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR53_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR53_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR53_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR54_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR54_position     11
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR54_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR54_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR54_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR55_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR55_position     13
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR55_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR55_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR55_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR56_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR56_position     15
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR56_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR56_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR56_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR57_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR57_position     17
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR57_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR57_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR57_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR58_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR58_position     19
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR58_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR58_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR58_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR59_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR59_position     21
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR59_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR59_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR59_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR60_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR60_position     23
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR60_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR60_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR60_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR61_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR61_position     25
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR61_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR61_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR61_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR62_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR62_position     27
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR62_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR62_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR62_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR63_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR63_position     29
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR63_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR63_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR63_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR64_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR64_position     31
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR64_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR64_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR64_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR65_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR65_position     33
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR65_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR65_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR65_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR66_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR66_position     35
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR66_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR66_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR66_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR67_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR67_position     37
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR67_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR67_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR67_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR68_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR68_position     39
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR68_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR68_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR68_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR69_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR69_position     41
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR69_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR69_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR69_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR70_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR70_position     43
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR70_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR70_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR70_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR71_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR71_position     45
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR71_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR71_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR71_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR72_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR72_position     47
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR72_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR72_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR72_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR73_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR73_position     49
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR73_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR73_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR73_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR74_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR74_position     51
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR74_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR74_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR74_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR75_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR75_position     53
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR75_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR75_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR75_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR76_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR76_position     55
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR76_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR76_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR76_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR77_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR77_position     57
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR77_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR77_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR77_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR78_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR78_position     59
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR78_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR78_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR78_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR79_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR79_position     61
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR79_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR79_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR79_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR80_width        2
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR80_position     63
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR80_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR80_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__RME_INTERRUPTS1_CONTROL_HIGH__RME_ERR80_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: icsram_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_LOW_offset         (0x0EBC) // Offset of Control register in instance
  
// Interrupt Control Register: icsram_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH_offset         (0x0EBD) // Offset of Control register in instance

#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR0_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR0_position     1
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR1_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR1_position     3
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR2_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR2_position     5
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR3_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR3_position     7
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR4_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR4_position     9
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR5_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR5_position     11
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR6_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR6_position     13
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR7_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR7_position     15
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR8_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR8_position     17
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR9_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR9_position     19
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR10_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR10_position     21
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR11_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR11_position     23
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR12_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR12_position     25
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR13_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR13_position     27
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR14_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR14_position     29
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR15_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR15_position     31
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR16_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR16_position     33
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR17_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR17_position     35
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR18_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR18_position     37
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR19_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR19_position     39
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR20_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR20_position     41
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR21_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR21_position     43
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR22_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR22_position     45
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR23_width        2
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR23_position     47
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__ICSRAM_INTERRUPTS_CONTROL_HIGH__ICSRAM_ERR23_insert(x,v)  _BGQ_INSERT(2,47,x,v)

  
// Interrupt Control Register: misc_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MISC_INTERRUPTS_CONTROL_LOW_offset         (0x0EC5) // Offset of Control register in instance
  
// Interrupt Control Register: misc_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH_offset         (0x0EC6) // Offset of Control register in instance

#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR0_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR0_position     1
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR1_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR1_position     3
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR2_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR2_position     5
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR3_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR3_position     7
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR4_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR4_position     9
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR5_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR5_position     11
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__ICSRAM_SELECT_ERR5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__RAN_ERR0_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__RAN_ERR0_position     13
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__RAN_ERR0_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__RAN_ERR0_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__RAN_ERR0_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR0_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR0_position     15
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR0_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR0_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR0_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR1_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR1_position     17
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR1_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR1_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__MU_ERR1_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__UPC_COUNTER_OVF_width        2
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__UPC_COUNTER_OVF_position     19
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__UPC_COUNTER_OVF_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__UPC_COUNTER_OVF_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__MISC_INTERRUPTS_CONTROL_HIGH__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,19,x,v)

  
// Interrupt Control Register: rmu_ecc_corr_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW_offset         (0x0ECE) // Offset of Control register in instance

#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE32_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE32_position     1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE32_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE32_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE32_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE33_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE33_position     3
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE33_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE33_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE33_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE34_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE34_position     5
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE34_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE34_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE34_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE35_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE35_position     7
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE35_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE35_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE35_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE36_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE36_position     9
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE36_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE36_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE36_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE37_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE37_position     11
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE37_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE37_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE37_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE38_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE38_position     13
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE38_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE38_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE38_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE39_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE39_position     15
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE39_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE39_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW__RMU_ECC_CE39_insert(x,v)  _BGQ_INSERT(2,15,x,v)

  
// Interrupt Control Register: rmu_ecc_corr_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH_offset         (0x0ECF) // Offset of Control register in instance

#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE0_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE0_position     1
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE1_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE1_position     3
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE2_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE2_position     5
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE3_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE3_position     7
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE4_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE4_position     9
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE5_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE5_position     11
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE6_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE6_position     13
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE7_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE7_position     15
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE7_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE8_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE8_position     17
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE8_get(x)       _BGQ_GET(2,17,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE8_set(v)       _BGQ_SET(2,17,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE8_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE9_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE9_position     19
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE9_get(x)       _BGQ_GET(2,19,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE9_set(v)       _BGQ_SET(2,19,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE9_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE10_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE10_position     21
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE10_get(x)       _BGQ_GET(2,21,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE10_set(v)       _BGQ_SET(2,21,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE10_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE11_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE11_position     23
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE11_get(x)       _BGQ_GET(2,23,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE11_set(v)       _BGQ_SET(2,23,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE11_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE12_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE12_position     25
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE12_get(x)       _BGQ_GET(2,25,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE12_set(v)       _BGQ_SET(2,25,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE12_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE13_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE13_position     27
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE13_get(x)       _BGQ_GET(2,27,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE13_set(v)       _BGQ_SET(2,27,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE13_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE14_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE14_position     29
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE14_get(x)       _BGQ_GET(2,29,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE14_set(v)       _BGQ_SET(2,29,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE14_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE15_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE15_position     31
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE15_get(x)       _BGQ_GET(2,31,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE15_set(v)       _BGQ_SET(2,31,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE15_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE16_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE16_position     33
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE16_get(x)       _BGQ_GET(2,33,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE16_set(v)       _BGQ_SET(2,33,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE16_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE17_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE17_position     35
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE17_get(x)       _BGQ_GET(2,35,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE17_set(v)       _BGQ_SET(2,35,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE17_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE18_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE18_position     37
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE18_get(x)       _BGQ_GET(2,37,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE18_set(v)       _BGQ_SET(2,37,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE18_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE19_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE19_position     39
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE19_get(x)       _BGQ_GET(2,39,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE19_set(v)       _BGQ_SET(2,39,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE19_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE20_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE20_position     41
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE20_get(x)       _BGQ_GET(2,41,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE20_set(v)       _BGQ_SET(2,41,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE20_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE21_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE21_position     43
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE21_get(x)       _BGQ_GET(2,43,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE21_set(v)       _BGQ_SET(2,43,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE21_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE22_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE22_position     45
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE22_get(x)       _BGQ_GET(2,45,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE22_set(v)       _BGQ_SET(2,45,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE22_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE23_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE23_position     47
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE23_get(x)       _BGQ_GET(2,47,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE23_set(v)       _BGQ_SET(2,47,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE23_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE24_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE24_position     49
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE24_get(x)       _BGQ_GET(2,49,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE24_set(v)       _BGQ_SET(2,49,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE24_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE25_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE25_position     51
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE25_get(x)       _BGQ_GET(2,51,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE25_set(v)       _BGQ_SET(2,51,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE25_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE26_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE26_position     53
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE26_get(x)       _BGQ_GET(2,53,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE26_set(v)       _BGQ_SET(2,53,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE26_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE27_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE27_position     55
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE27_get(x)       _BGQ_GET(2,55,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE27_set(v)       _BGQ_SET(2,55,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE27_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE28_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE28_position     57
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE28_get(x)       _BGQ_GET(2,57,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE28_set(v)       _BGQ_SET(2,57,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE28_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE29_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE29_position     59
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE29_get(x)       _BGQ_GET(2,59,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE29_set(v)       _BGQ_SET(2,59,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE29_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE30_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE30_position     61
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE30_get(x)       _BGQ_GET(2,61,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE30_set(v)       _BGQ_SET(2,61,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE30_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE31_width        2
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE31_position     63
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE31_get(x)       _BGQ_GET(2,63,x)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE31_set(v)       _BGQ_SET(2,63,v)
#define MU_DCR__RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH__RMU_ECC_CE31_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: misc_ecc_corr_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_LOW_offset         (0x0ED6) // Offset of Control register in instance
  
// Interrupt Control Register: misc_ecc_corr_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH_offset         (0x0ED7) // Offset of Control register in instance

#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE0_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE0_position     1
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE1_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE1_position     3
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE2_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE2_position     5
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__MMREGS_ECC_CE2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE0_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE0_position     7
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE0_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE0_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE0_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE1_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE1_position     9
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE1_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE1_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE1_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE2_width        2
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE2_position     11
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE2_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE2_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH__XS_ECC_CE2_insert(x,v)  _BGQ_INSERT(2,11,x,v)

  
// Interrupt Control Register: ecc_count_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_LOW_offset         (0x0EDE) // Offset of Control register in instance
  
// Interrupt Control Register: ecc_count_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH_offset         (0x0EDF) // Offset of Control register in instance

#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER0_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER0_position     1
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER0_get(x)       _BGQ_GET(2,1,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER0_set(v)       _BGQ_SET(2,1,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER0_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER1_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER1_position     3
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER1_get(x)       _BGQ_GET(2,3,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER1_set(v)       _BGQ_SET(2,3,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER1_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER2_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER2_position     5
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER2_get(x)       _BGQ_GET(2,5,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER2_set(v)       _BGQ_SET(2,5,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER2_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER3_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER3_position     7
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER3_get(x)       _BGQ_GET(2,7,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER3_set(v)       _BGQ_SET(2,7,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER3_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER4_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER4_position     9
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER4_get(x)       _BGQ_GET(2,9,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER4_set(v)       _BGQ_SET(2,9,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER4_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER5_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER5_position     11
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER5_get(x)       _BGQ_GET(2,11,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER5_set(v)       _BGQ_SET(2,11,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER5_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER6_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER6_position     13
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER6_get(x)       _BGQ_GET(2,13,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER6_set(v)       _BGQ_SET(2,13,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER6_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER7_width        2
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER7_position     15
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER7_get(x)       _BGQ_GET(2,15,x)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER7_set(v)       _BGQ_SET(2,15,v)
#define MU_DCR__ECC_COUNT_INTERRUPTS_CONTROL_HIGH__ECC_COUNT_OVER7_insert(x,v)  _BGQ_INSERT(2,15,x,v)

#ifndef __ASSEMBLY__

typedef struct mu_dcr { 
    /* 0x0000 */ uint64_t reserve_0000[0x0D00];
    /* 0x0D00 */ uint64_t imfifo_thold_int_en[MU_DCR__IMFIFO_THOLD_INT_EN_range];
    /* 0x0D11 */ uint64_t imfifo_system[MU_DCR__IMFIFO_SYSTEM_range];
    /* 0x0D22 */ uint64_t imfifo_rget[MU_DCR__IMFIFO_RGET_range];
    /* 0x0D33 */ uint64_t imu_thold;
    /* 0x0D34 */ uint64_t rget_thold;
    /* 0x0D35 */ uint64_t rmfifo_en[MU_DCR__RMFIFO_EN_range];
    /* 0x0D46 */ uint64_t rmfifo_thold_int_en[MU_DCR__RMFIFO_THOLD_INT_EN_range];
    /* 0x0D57 */ uint64_t rmfifo_packet_int_en[MU_DCR__RMFIFO_PACKET_INT_EN_range];
    /* 0x0D68 */ uint64_t rmfifo_system[MU_DCR__RMFIFO_SYSTEM_range];
    /* 0x0D79 */ uint64_t rmu_thold;
    /* 0x0D7A */ uint64_t rputbase_system0;
    /* 0x0D7B */ uint64_t rputbase_system1;
    /* 0x0D7C */ uint64_t sys_barrier;
    /* 0x0D7D */ uint64_t barrier_int_en;
    /* 0x0D7E */ uint64_t reserve_0D7E[0x0002];
    /* 0x0D80 */ uint64_t min_usr_addr_range[MU_DCR__MIN_USR_ADDR_RANGE_range];
    /* 0x0D94 */ uint64_t max_usr_addr_range[MU_DCR__MAX_USR_ADDR_RANGE_range];
    /* 0x0DA8 */ uint64_t min_sys_addr_range[MU_DCR__MIN_SYS_ADDR_RANGE_range];
    /* 0x0DB0 */ uint64_t max_sys_addr_range[MU_DCR__MAX_SYS_ADDR_RANGE_range];
    /* 0x0DB8 */ uint64_t usr_inj_range;
    /* 0x0DB9 */ uint64_t sys_inj_range;
    /* 0x0DBA */ uint64_t reserve_0DBA[0x000D];
    /* 0x0DC7 */ uint64_t ime_enable;
    /* 0x0DC8 */ uint64_t rme_enable;
    /* 0x0DC9 */ uint64_t rme_wait_full;
    /* 0x0DCA */ uint64_t rme_wait_tail;
    /* 0x0DCB */ uint64_t gip_int_en_0_7;
    /* 0x0DCC */ uint64_t gip_int_en_8_15;
    /* 0x0DCD */ uint64_t gip_int_en_16;
    /* 0x0DCE */ uint64_t xm_gen_cnt_delay;
    /* 0x0DCF */ uint64_t reset;
    /* 0x0DD0 */ uint64_t mod_halt_en;
    /* 0x0DD1 */ uint64_t mod_halted;
    /* 0x0DD2 */ uint64_t arlog_scan_enable;
    /* 0x0DD3 */ uint64_t rme_hdump;
    /* 0x0DD4 */ uint64_t ecc_pass;
    /* 0x0DD5 */ uint64_t mcsram_bd_ctl;
    /* 0x0DD6 */ uint64_t mcsram_bd_wdata;
    /* 0x0DD7 */ uint64_t reserve_0DD7[0x0006];
    /* 0x0DDD */ uint64_t mcsram_bd_rdata0;
    /* 0x0DDE */ uint64_t mcsram_bd_rdata1;
    /* 0x0DDF */ uint64_t mcsram_bd_rdata2;
    /* 0x0DE0 */ uint64_t mcsram_bd_rdata3;
    /* 0x0DE1 */ uint64_t mcsram_bd_rdata4;
    /* 0x0DE2 */ uint64_t mcsram_bd_rdata5;
    /* 0x0DE3 */ uint64_t mcsram_bd_rdecc;
    /* 0x0DE4 */ uint64_t mcsram_bd_fetching_desc;
    /* 0x0DE5 */ uint64_t rcsram_bd_ctl;
    /* 0x0DE6 */ uint64_t rcsram_bd_rdata;
    /* 0x0DE7 */ uint64_t rput_backdoor;
    /* 0x0DE8 */ uint64_t rput_backdoor_rdecc;
    /* 0x0DE9 */ uint64_t ime_state;
    /* 0x0DEA */ uint64_t rme_state0;
    /* 0x0DEB */ uint64_t rme_state1;
    /* 0x0DEC */ uint64_t inj_ndfifo_avail;
    /* 0x0DED */ uint64_t ecc_int_thresh;
    /* 0x0DEE */ uint64_t arlog_ecc_count;
    /* 0x0DEF */ uint64_t icsram_ecc_count;
    /* 0x0DF0 */ uint64_t mcsram_ecc_count;
    /* 0x0DF1 */ uint64_t rcsram_ecc_count;
    /* 0x0DF2 */ uint64_t rput_ecc_count;
    /* 0x0DF3 */ uint64_t mmregs_ecc_count;
    /* 0x0DF4 */ uint64_t xs_ecc_count;
    /* 0x0DF5 */ uint64_t rme_ecc_count;
    /* 0x0DF6 */ uint64_t spare_dcr;
    /* 0x0DF7 */ uint64_t reserve_0DF7[0x0009];
    /* 0x0E00 */ uint64_t interrupt_internal_error__state;
    /* 0x0E01 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0E02 */ uint64_t interrupt_internal_error__force;
    /* 0x0E03 */ uint64_t interrupt_internal_error__first;
    /* 0x0E04 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0E05 */ uint64_t interrupt_internal_error__critical;
    /* 0x0E06 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0E07 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0E08 */ uint64_t interrupt_internal_error_hw_info;
    /* 0x0E09 */ uint64_t interrupt_internal_error_data_info;
    /* 0x0E0A */ uint64_t master_port0_interrupts__state;
    /* 0x0E0B */ uint64_t master_port0_interrupts_control_low;
    /* 0x0E0C */ uint64_t master_port0_interrupts_control_high;
    /* 0x0E0D */ uint64_t master_port0_interrupts__force;
    /* 0x0E0E */ uint64_t master_port0_interrupts__first;
    /* 0x0E0F */ uint64_t master_port0_interrupts__machine_check;
    /* 0x0E10 */ uint64_t master_port0_interrupts__critical;
    /* 0x0E11 */ uint64_t master_port0_interrupts__noncritical;
    /* 0x0E12 */ uint64_t xm0_err_info;
    /* 0x0E13 */ uint64_t master_port1_interrupts__state;
    /* 0x0E14 */ uint64_t master_port1_interrupts_control_low;
    /* 0x0E15 */ uint64_t master_port1_interrupts_control_high;
    /* 0x0E16 */ uint64_t master_port1_interrupts__force;
    /* 0x0E17 */ uint64_t master_port1_interrupts__first;
    /* 0x0E18 */ uint64_t master_port1_interrupts__machine_check;
    /* 0x0E19 */ uint64_t master_port1_interrupts__critical;
    /* 0x0E1A */ uint64_t master_port1_interrupts__noncritical;
    /* 0x0E1B */ uint64_t xm1_err_info;
    /* 0x0E1C */ uint64_t master_port2_interrupts__state;
    /* 0x0E1D */ uint64_t master_port2_interrupts_control_low;
    /* 0x0E1E */ uint64_t master_port2_interrupts_control_high;
    /* 0x0E1F */ uint64_t master_port2_interrupts__force;
    /* 0x0E20 */ uint64_t master_port2_interrupts__first;
    /* 0x0E21 */ uint64_t master_port2_interrupts__machine_check;
    /* 0x0E22 */ uint64_t master_port2_interrupts__critical;
    /* 0x0E23 */ uint64_t master_port2_interrupts__noncritical;
    /* 0x0E24 */ uint64_t xm2_err_info;
    /* 0x0E25 */ uint64_t slave_port_interrupts__state;
    /* 0x0E26 */ uint64_t slave_port_interrupts_control_low;
    /* 0x0E27 */ uint64_t slave_port_interrupts_control_high;
    /* 0x0E28 */ uint64_t slave_port_interrupts__force;
    /* 0x0E29 */ uint64_t slave_port_interrupts__first;
    /* 0x0E2A */ uint64_t slave_port_interrupts__machine_check;
    /* 0x0E2B */ uint64_t slave_port_interrupts__critical;
    /* 0x0E2C */ uint64_t slave_port_interrupts__noncritical;
    /* 0x0E2D */ uint64_t xs_err_info;
    /* 0x0E2E */ uint64_t mmregs_interrupts__state;
    /* 0x0E2F */ uint64_t mmregs_interrupts_control_low;
    /* 0x0E30 */ uint64_t mmregs_interrupts_control_high;
    /* 0x0E31 */ uint64_t mmregs_interrupts__force;
    /* 0x0E32 */ uint64_t mmregs_interrupts__first;
    /* 0x0E33 */ uint64_t mmregs_interrupts__machine_check;
    /* 0x0E34 */ uint64_t mmregs_interrupts__critical;
    /* 0x0E35 */ uint64_t mmregs_interrupts__noncritical;
    /* 0x0E36 */ uint64_t mmregs_fifo_prot_err;
    /* 0x0E37 */ uint64_t fifo_interrupts__state;
    /* 0x0E38 */ uint64_t fifo_interrupts_control_low;
    /* 0x0E39 */ uint64_t fifo_interrupts_control_high;
    /* 0x0E3A */ uint64_t fifo_interrupts__force;
    /* 0x0E3B */ uint64_t fifo_interrupts__first;
    /* 0x0E3C */ uint64_t fifo_interrupts__machine_check;
    /* 0x0E3D */ uint64_t fifo_interrupts__critical;
    /* 0x0E3E */ uint64_t fifo_interrupts__noncritical;
    /* 0x0E3F */ uint64_t rmfifo;
    /* 0x0E40 */ uint64_t imu_ecc_interrupts__state;
    /* 0x0E41 */ uint64_t imu_ecc_interrupts_control_low;
    /* 0x0E42 */ uint64_t imu_ecc_interrupts_control_high;
    /* 0x0E43 */ uint64_t imu_ecc_interrupts__force;
    /* 0x0E44 */ uint64_t imu_ecc_interrupts__first;
    /* 0x0E45 */ uint64_t imu_ecc_interrupts__machine_check;
    /* 0x0E46 */ uint64_t imu_ecc_interrupts__critical;
    /* 0x0E47 */ uint64_t imu_ecc_interrupts__noncritical;
    /* 0x0E48 */ uint64_t mcsram_interrupts__state;
    /* 0x0E49 */ uint64_t mcsram_interrupts_control_low;
    /* 0x0E4A */ uint64_t mcsram_interrupts_control_high;
    /* 0x0E4B */ uint64_t mcsram_interrupts__force;
    /* 0x0E4C */ uint64_t mcsram_interrupts__first;
    /* 0x0E4D */ uint64_t mcsram_interrupts__machine_check;
    /* 0x0E4E */ uint64_t mcsram_interrupts__critical;
    /* 0x0E4F */ uint64_t mcsram_interrupts__noncritical;
    /* 0x0E50 */ uint64_t mcsram_err_fifo_id;
    /* 0x0E51 */ uint64_t imfifo;
    /* 0x0E52 */ uint64_t arlog_interrupts__state;
    /* 0x0E53 */ uint64_t arlog_interrupts_control_low;
    /* 0x0E54 */ uint64_t arlog_interrupts_control_high;
    /* 0x0E55 */ uint64_t arlog_interrupts__force;
    /* 0x0E56 */ uint64_t arlog_interrupts__first;
    /* 0x0E57 */ uint64_t arlog_interrupts__machine_check;
    /* 0x0E58 */ uint64_t arlog_interrupts__critical;
    /* 0x0E59 */ uint64_t arlog_interrupts__noncritical;
    /* 0x0E5A */ uint64_t rmu_ecc_interrupts__state;
    /* 0x0E5B */ uint64_t rmu_ecc_interrupts_control_low;
    /* 0x0E5C */ uint64_t rmu_ecc_interrupts_control_high;
    /* 0x0E5D */ uint64_t rmu_ecc_interrupts__force;
    /* 0x0E5E */ uint64_t rmu_ecc_interrupts__first;
    /* 0x0E5F */ uint64_t rmu_ecc_interrupts__machine_check;
    /* 0x0E60 */ uint64_t rmu_ecc_interrupts__critical;
    /* 0x0E61 */ uint64_t rmu_ecc_interrupts__noncritical;
    /* 0x0E62 */ uint64_t rcsram_interrupts__state;
    /* 0x0E63 */ uint64_t rcsram_interrupts_control_low;
    /* 0x0E64 */ uint64_t rcsram_interrupts_control_high;
    /* 0x0E65 */ uint64_t rcsram_interrupts__force;
    /* 0x0E66 */ uint64_t rcsram_interrupts__first;
    /* 0x0E67 */ uint64_t rcsram_interrupts__machine_check;
    /* 0x0E68 */ uint64_t rcsram_interrupts__critical;
    /* 0x0E69 */ uint64_t rcsram_interrupts__noncritical;
    /* 0x0E6A */ uint64_t rmfifo_access_error_id;
    /* 0x0E6B */ uint64_t rputsram_interrupts__state;
    /* 0x0E6C */ uint64_t rputsram_interrupts_control_low;
    /* 0x0E6D */ uint64_t rputsram_interrupts_control_high;
    /* 0x0E6E */ uint64_t rputsram_interrupts__force;
    /* 0x0E6F */ uint64_t rputsram_interrupts__first;
    /* 0x0E70 */ uint64_t rputsram_interrupts__machine_check;
    /* 0x0E71 */ uint64_t rputsram_interrupts__critical;
    /* 0x0E72 */ uint64_t rputsram_interrupts__noncritical;
    /* 0x0E73 */ uint64_t ime_interrupts__state;
    /* 0x0E74 */ uint64_t ime_interrupts_control_low;
    /* 0x0E75 */ uint64_t ime_interrupts_control_high;
    /* 0x0E76 */ uint64_t ime_interrupts__force;
    /* 0x0E77 */ uint64_t ime_interrupts__first;
    /* 0x0E78 */ uint64_t ime_interrupts__machine_check;
    /* 0x0E79 */ uint64_t ime_interrupts__critical;
    /* 0x0E7A */ uint64_t ime_interrupts__noncritical;
    /* 0x0E7B */ uint64_t ime_p_err0;
    /* 0x0E7C */ uint64_t ime_p_err1;
    /* 0x0E7D */ uint64_t ime_p_err2;
    /* 0x0E7E */ uint64_t ime_p_err3;
    /* 0x0E7F */ uint64_t ime_p_err4;
    /* 0x0E80 */ uint64_t ime_p_err5;
    /* 0x0E81 */ uint64_t ime_p_err6;
    /* 0x0E82 */ uint64_t ime_p_err7;
    /* 0x0E83 */ uint64_t ime_p_err8;
    /* 0x0E84 */ uint64_t ime_p_err9;
    /* 0x0E85 */ uint64_t ime_p_err10;
    /* 0x0E86 */ uint64_t ime_p_err11;
    /* 0x0E87 */ uint64_t ime_p_err12;
    /* 0x0E88 */ uint64_t ime_p_err13;
    /* 0x0E89 */ uint64_t ime_p_err14;
    /* 0x0E8A */ uint64_t ime_p_err15;
    /* 0x0E8B */ uint64_t rme_interrupts0__state;
    /* 0x0E8C */ uint64_t rme_interrupts0_control_low;
    /* 0x0E8D */ uint64_t rme_interrupts0_control_high;
    /* 0x0E8E */ uint64_t rme_interrupts0__force;
    /* 0x0E8F */ uint64_t rme_interrupts0__first;
    /* 0x0E90 */ uint64_t rme_interrupts0__machine_check;
    /* 0x0E91 */ uint64_t rme_interrupts0__critical;
    /* 0x0E92 */ uint64_t rme_interrupts0__noncritical;
    /* 0x0E93 */ uint64_t rme_interrupts1__state;
    /* 0x0E94 */ uint64_t rme_interrupts1_control_low;
    /* 0x0E95 */ uint64_t rme_interrupts1_control_high;
    /* 0x0E96 */ uint64_t rme_interrupts1__force;
    /* 0x0E97 */ uint64_t rme_interrupts1__first;
    /* 0x0E98 */ uint64_t rme_interrupts1__machine_check;
    /* 0x0E99 */ uint64_t rme_interrupts1__critical;
    /* 0x0E9A */ uint64_t rme_interrupts1__noncritical;
    /* 0x0E9B */ uint64_t rme_p_err0;
    /* 0x0E9C */ uint64_t rme_header_err0;
    /* 0x0E9D */ uint64_t rme_p_err1;
    /* 0x0E9E */ uint64_t rme_header_err1;
    /* 0x0E9F */ uint64_t rme_p_err2;
    /* 0x0EA0 */ uint64_t rme_header_err2;
    /* 0x0EA1 */ uint64_t rme_p_err3;
    /* 0x0EA2 */ uint64_t rme_header_err3;
    /* 0x0EA3 */ uint64_t rme_p_err4;
    /* 0x0EA4 */ uint64_t rme_header_err4;
    /* 0x0EA5 */ uint64_t rme_p_err5;
    /* 0x0EA6 */ uint64_t rme_header_err5;
    /* 0x0EA7 */ uint64_t rme_p_err6;
    /* 0x0EA8 */ uint64_t rme_header_err6;
    /* 0x0EA9 */ uint64_t rme_p_err7;
    /* 0x0EAA */ uint64_t rme_header_err7;
    /* 0x0EAB */ uint64_t rme_p_err8;
    /* 0x0EAC */ uint64_t rme_header_err8;
    /* 0x0EAD */ uint64_t rme_p_err9;
    /* 0x0EAE */ uint64_t rme_header_err9;
    /* 0x0EAF */ uint64_t rme_p_err10;
    /* 0x0EB0 */ uint64_t rme_header_err10;
    /* 0x0EB1 */ uint64_t rme_p_err11;
    /* 0x0EB2 */ uint64_t rme_header_err11;
    /* 0x0EB3 */ uint64_t rme_p_err12;
    /* 0x0EB4 */ uint64_t rme_header_err12;
    /* 0x0EB5 */ uint64_t rme_p_err13;
    /* 0x0EB6 */ uint64_t rme_header_err13;
    /* 0x0EB7 */ uint64_t rme_p_err14;
    /* 0x0EB8 */ uint64_t rme_header_err14;
    /* 0x0EB9 */ uint64_t rme_p_err15;
    /* 0x0EBA */ uint64_t rme_header_err15;
    /* 0x0EBB */ uint64_t icsram_interrupts__state;
    /* 0x0EBC */ uint64_t icsram_interrupts_control_low;
    /* 0x0EBD */ uint64_t icsram_interrupts_control_high;
    /* 0x0EBE */ uint64_t icsram_interrupts__force;
    /* 0x0EBF */ uint64_t icsram_interrupts__first;
    /* 0x0EC0 */ uint64_t icsram_interrupts__machine_check;
    /* 0x0EC1 */ uint64_t icsram_interrupts__critical;
    /* 0x0EC2 */ uint64_t icsram_interrupts__noncritical;
    /* 0x0EC3 */ uint64_t icsram_rget_fifo_full;
    /* 0x0EC4 */ uint64_t misc_interrupts__state;
    /* 0x0EC5 */ uint64_t misc_interrupts_control_low;
    /* 0x0EC6 */ uint64_t misc_interrupts_control_high;
    /* 0x0EC7 */ uint64_t misc_interrupts__force;
    /* 0x0EC8 */ uint64_t misc_interrupts__first;
    /* 0x0EC9 */ uint64_t misc_interrupts__machine_check;
    /* 0x0ECA */ uint64_t misc_interrupts__critical;
    /* 0x0ECB */ uint64_t misc_interrupts__noncritical;
    /* 0x0ECC */ uint64_t imfifo_access_error_id;
    /* 0x0ECD */ uint64_t rmu_ecc_corr_interrupts__state;
    /* 0x0ECE */ uint64_t rmu_ecc_corr_interrupts_control_low;
    /* 0x0ECF */ uint64_t rmu_ecc_corr_interrupts_control_high;
    /* 0x0ED0 */ uint64_t rmu_ecc_corr_interrupts__force;
    /* 0x0ED1 */ uint64_t rmu_ecc_corr_interrupts__first;
    /* 0x0ED2 */ uint64_t rmu_ecc_corr_interrupts__machine_check;
    /* 0x0ED3 */ uint64_t rmu_ecc_corr_interrupts__critical;
    /* 0x0ED4 */ uint64_t rmu_ecc_corr_interrupts__noncritical;
    /* 0x0ED5 */ uint64_t misc_ecc_corr_interrupts__state;
    /* 0x0ED6 */ uint64_t misc_ecc_corr_interrupts_control_low;
    /* 0x0ED7 */ uint64_t misc_ecc_corr_interrupts_control_high;
    /* 0x0ED8 */ uint64_t misc_ecc_corr_interrupts__force;
    /* 0x0ED9 */ uint64_t misc_ecc_corr_interrupts__first;
    /* 0x0EDA */ uint64_t misc_ecc_corr_interrupts__machine_check;
    /* 0x0EDB */ uint64_t misc_ecc_corr_interrupts__critical;
    /* 0x0EDC */ uint64_t misc_ecc_corr_interrupts__noncritical;
    /* 0x0EDD */ uint64_t ecc_count_interrupts__state;
    /* 0x0EDE */ uint64_t ecc_count_interrupts_control_low;
    /* 0x0EDF */ uint64_t ecc_count_interrupts_control_high;
    /* 0x0EE0 */ uint64_t ecc_count_interrupts__force;
    /* 0x0EE1 */ uint64_t ecc_count_interrupts__first;
    /* 0x0EE2 */ uint64_t ecc_count_interrupts__machine_check;
    /* 0x0EE3 */ uint64_t ecc_count_interrupts__critical;
    /* 0x0EE4 */ uint64_t ecc_count_interrupts__noncritical;
    /* 0x0EE5 */ uint64_t reserve_0EE5[0x001B];
    /* 0x0F00 */ uint64_t upc_mu_counter[MU_DCR__UPC_MU_COUNTER_range];
    /* 0x0F28 */ uint64_t reserve_0F28[0x0019];
    /* 0x0F41 */ uint64_t upc_mu_counter_control_rw;
    /* 0x0F42 */ uint64_t upc_mu_counter_control_w1s;
    /* 0x0F43 */ uint64_t upc_mu_counter_control_w1c;
    /* 0x0F44 */ uint64_t upc_mu_config;
} mu_dcr_t;
  
#define MU_DCR_USER_PTR _DCR_USER_PTR(mu_dcr_t, MU_DCR)
#define MU_DCR_PRIV_PTR _DCR_PRIV_PTR(mu_dcr_t, MU_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
