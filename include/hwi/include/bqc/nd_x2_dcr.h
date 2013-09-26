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

#ifndef   _ND_X2_DCR__H_ // Prevent multiple inclusion
#define   _ND_X2_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define ND_X2_DCR_base    (0x198000)  // Base address for all instances
#define ND_X2_DCR_size    (0x0039)    // Size of instance (largest offset + 1 )
  
#define ND_X2_DCR(reg) _DCR_REG(ND_X2_DCR, reg)
  
// Register:
//   Name:        reset
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFFFFFFF80000000
//   Description: Inj/Rcp fifo X2 resets.  A unit is in reset if its corresponding reset bit is 1.
#define ND_X2_DCR__RESET_offset  (0x0000) // Offset of register in instance 
#define ND_X2_DCR__RESET_reset  UNSIGNED64(0xFFFFFFFF80000000)

//   Field:       dcrs_out
//   Description: Reset all DCR registers except this one, lowering this bit first before releasing others
#define ND_X2_DCR__RESET__DCRS_OUT_width        1
#define ND_X2_DCR__RESET__DCRS_OUT_position     0
#define ND_X2_DCR__RESET__DCRS_OUT_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__RESET__DCRS_OUT_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__RESET__DCRS_OUT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t0
//   Description: Reset Injection fifo torus 0
#define ND_X2_DCR__RESET__INJ_T0_width        1
#define ND_X2_DCR__RESET__INJ_T0_position     1
#define ND_X2_DCR__RESET__INJ_T0_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__RESET__INJ_T0_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__RESET__INJ_T0_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t1
//   Description: Reset Injection fifo torus 1
#define ND_X2_DCR__RESET__INJ_T1_width        1
#define ND_X2_DCR__RESET__INJ_T1_position     2
#define ND_X2_DCR__RESET__INJ_T1_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__RESET__INJ_T1_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__RESET__INJ_T1_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t2
//   Description: Reset Injection fifo torus 2
#define ND_X2_DCR__RESET__INJ_T2_width        1
#define ND_X2_DCR__RESET__INJ_T2_position     3
#define ND_X2_DCR__RESET__INJ_T2_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__RESET__INJ_T2_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__RESET__INJ_T2_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t3
//   Description: Reset Injection fifo torus 3
#define ND_X2_DCR__RESET__INJ_T3_width        1
#define ND_X2_DCR__RESET__INJ_T3_position     4
#define ND_X2_DCR__RESET__INJ_T3_get(x)       _BGQ_GET(1,4,x)
#define ND_X2_DCR__RESET__INJ_T3_set(v)       _BGQ_SET(1,4,v)
#define ND_X2_DCR__RESET__INJ_T3_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t4
//   Description: Reset Injection fifo torus 4
#define ND_X2_DCR__RESET__INJ_T4_width        1
#define ND_X2_DCR__RESET__INJ_T4_position     5
#define ND_X2_DCR__RESET__INJ_T4_get(x)       _BGQ_GET(1,5,x)
#define ND_X2_DCR__RESET__INJ_T4_set(v)       _BGQ_SET(1,5,v)
#define ND_X2_DCR__RESET__INJ_T4_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t5
//   Description: Reset Injection fifo torus 5
#define ND_X2_DCR__RESET__INJ_T5_width        1
#define ND_X2_DCR__RESET__INJ_T5_position     6
#define ND_X2_DCR__RESET__INJ_T5_get(x)       _BGQ_GET(1,6,x)
#define ND_X2_DCR__RESET__INJ_T5_set(v)       _BGQ_SET(1,6,v)
#define ND_X2_DCR__RESET__INJ_T5_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t6
//   Description: Reset Injection fifo torus 6
#define ND_X2_DCR__RESET__INJ_T6_width        1
#define ND_X2_DCR__RESET__INJ_T6_position     7
#define ND_X2_DCR__RESET__INJ_T6_get(x)       _BGQ_GET(1,7,x)
#define ND_X2_DCR__RESET__INJ_T6_set(v)       _BGQ_SET(1,7,v)
#define ND_X2_DCR__RESET__INJ_T6_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t7
//   Description: Reset Injection fifo torus 7
#define ND_X2_DCR__RESET__INJ_T7_width        1
#define ND_X2_DCR__RESET__INJ_T7_position     8
#define ND_X2_DCR__RESET__INJ_T7_get(x)       _BGQ_GET(1,8,x)
#define ND_X2_DCR__RESET__INJ_T7_set(v)       _BGQ_SET(1,8,v)
#define ND_X2_DCR__RESET__INJ_T7_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t8
//   Description: Reset Injection fifo torus 8
#define ND_X2_DCR__RESET__INJ_T8_width        1
#define ND_X2_DCR__RESET__INJ_T8_position     9
#define ND_X2_DCR__RESET__INJ_T8_get(x)       _BGQ_GET(1,9,x)
#define ND_X2_DCR__RESET__INJ_T8_set(v)       _BGQ_SET(1,9,v)
#define ND_X2_DCR__RESET__INJ_T8_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_t9
//   Description: Reset Injection fifo torus 9
#define ND_X2_DCR__RESET__INJ_T9_width        1
#define ND_X2_DCR__RESET__INJ_T9_position     10
#define ND_X2_DCR__RESET__INJ_T9_get(x)       _BGQ_GET(1,10,x)
#define ND_X2_DCR__RESET__INJ_T9_set(v)       _BGQ_SET(1,10,v)
#define ND_X2_DCR__RESET__INJ_T9_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_l0
//   Description: Reset Injection fifo local 0
#define ND_X2_DCR__RESET__INJ_L0_width        1
#define ND_X2_DCR__RESET__INJ_L0_position     11
#define ND_X2_DCR__RESET__INJ_L0_get(x)       _BGQ_GET(1,11,x)
#define ND_X2_DCR__RESET__INJ_L0_set(v)       _BGQ_SET(1,11,v)
#define ND_X2_DCR__RESET__INJ_L0_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_l1
//   Description: Reset Injection fifo local 1
#define ND_X2_DCR__RESET__INJ_L1_width        1
#define ND_X2_DCR__RESET__INJ_L1_position     12
#define ND_X2_DCR__RESET__INJ_L1_get(x)       _BGQ_GET(1,12,x)
#define ND_X2_DCR__RESET__INJ_L1_set(v)       _BGQ_SET(1,12,v)
#define ND_X2_DCR__RESET__INJ_L1_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_hp
//   Description: Reset Injection fifo torus high priority
#define ND_X2_DCR__RESET__INJ_HP_width        1
#define ND_X2_DCR__RESET__INJ_HP_position     13
#define ND_X2_DCR__RESET__INJ_HP_get(x)       _BGQ_GET(1,13,x)
#define ND_X2_DCR__RESET__INJ_HP_set(v)       _BGQ_SET(1,13,v)
#define ND_X2_DCR__RESET__INJ_HP_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_io
//   Description: Reset Injection fifo torus system I/O
#define ND_X2_DCR__RESET__INJ_IO_width        1
#define ND_X2_DCR__RESET__INJ_IO_position     14
#define ND_X2_DCR__RESET__INJ_IO_get(x)       _BGQ_GET(1,14,x)
#define ND_X2_DCR__RESET__INJ_IO_set(v)       _BGQ_SET(1,14,v)
#define ND_X2_DCR__RESET__INJ_IO_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_c0
//   Description: Reset Injection fifo collective 0
#define ND_X2_DCR__RESET__INJ_C0_width        1
#define ND_X2_DCR__RESET__INJ_C0_position     15
#define ND_X2_DCR__RESET__INJ_C0_get(x)       _BGQ_GET(1,15,x)
#define ND_X2_DCR__RESET__INJ_C0_set(v)       _BGQ_SET(1,15,v)
#define ND_X2_DCR__RESET__INJ_C0_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       inj_c1
//   Description: Reset Injection fifo collective 1
#define ND_X2_DCR__RESET__INJ_C1_width        1
#define ND_X2_DCR__RESET__INJ_C1_position     16
#define ND_X2_DCR__RESET__INJ_C1_get(x)       _BGQ_GET(1,16,x)
#define ND_X2_DCR__RESET__INJ_C1_set(v)       _BGQ_SET(1,16,v)
#define ND_X2_DCR__RESET__INJ_C1_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t0
//   Description: Reset reception fifo torus 0
#define ND_X2_DCR__RESET__RCP_T0_width        1
#define ND_X2_DCR__RESET__RCP_T0_position     17
#define ND_X2_DCR__RESET__RCP_T0_get(x)       _BGQ_GET(1,17,x)
#define ND_X2_DCR__RESET__RCP_T0_set(v)       _BGQ_SET(1,17,v)
#define ND_X2_DCR__RESET__RCP_T0_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t1
//   Description: Reset reception fifo torus 1
#define ND_X2_DCR__RESET__RCP_T1_width        1
#define ND_X2_DCR__RESET__RCP_T1_position     18
#define ND_X2_DCR__RESET__RCP_T1_get(x)       _BGQ_GET(1,18,x)
#define ND_X2_DCR__RESET__RCP_T1_set(v)       _BGQ_SET(1,18,v)
#define ND_X2_DCR__RESET__RCP_T1_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t2
//   Description: Reset reception fifo torus 2
#define ND_X2_DCR__RESET__RCP_T2_width        1
#define ND_X2_DCR__RESET__RCP_T2_position     19
#define ND_X2_DCR__RESET__RCP_T2_get(x)       _BGQ_GET(1,19,x)
#define ND_X2_DCR__RESET__RCP_T2_set(v)       _BGQ_SET(1,19,v)
#define ND_X2_DCR__RESET__RCP_T2_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_t3
//   Description: Reset reception fifo torus 3
#define ND_X2_DCR__RESET__RCP_T3_width        1
#define ND_X2_DCR__RESET__RCP_T3_position     20
#define ND_X2_DCR__RESET__RCP_T3_get(x)       _BGQ_GET(1,20,x)
#define ND_X2_DCR__RESET__RCP_T3_set(v)       _BGQ_SET(1,20,v)
#define ND_X2_DCR__RESET__RCP_T3_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_t4
//   Description: Reset reception fifo torus 4
#define ND_X2_DCR__RESET__RCP_T4_width        1
#define ND_X2_DCR__RESET__RCP_T4_position     21
#define ND_X2_DCR__RESET__RCP_T4_get(x)       _BGQ_GET(1,21,x)
#define ND_X2_DCR__RESET__RCP_T4_set(v)       _BGQ_SET(1,21,v)
#define ND_X2_DCR__RESET__RCP_T4_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_t5
//   Description: Reset reception fifo torus 5
#define ND_X2_DCR__RESET__RCP_T5_width        1
#define ND_X2_DCR__RESET__RCP_T5_position     22
#define ND_X2_DCR__RESET__RCP_T5_get(x)       _BGQ_GET(1,22,x)
#define ND_X2_DCR__RESET__RCP_T5_set(v)       _BGQ_SET(1,22,v)
#define ND_X2_DCR__RESET__RCP_T5_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_t6
//   Description: Reset reception fifo torus 6
#define ND_X2_DCR__RESET__RCP_T6_width        1
#define ND_X2_DCR__RESET__RCP_T6_position     23
#define ND_X2_DCR__RESET__RCP_T6_get(x)       _BGQ_GET(1,23,x)
#define ND_X2_DCR__RESET__RCP_T6_set(v)       _BGQ_SET(1,23,v)
#define ND_X2_DCR__RESET__RCP_T6_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_t7
//   Description: Reset reception fifo torus 7
#define ND_X2_DCR__RESET__RCP_T7_width        1
#define ND_X2_DCR__RESET__RCP_T7_position     24
#define ND_X2_DCR__RESET__RCP_T7_get(x)       _BGQ_GET(1,24,x)
#define ND_X2_DCR__RESET__RCP_T7_set(v)       _BGQ_SET(1,24,v)
#define ND_X2_DCR__RESET__RCP_T7_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_t8
//   Description: Reset reception fifo torus 8
#define ND_X2_DCR__RESET__RCP_T8_width        1
#define ND_X2_DCR__RESET__RCP_T8_position     25
#define ND_X2_DCR__RESET__RCP_T8_get(x)       _BGQ_GET(1,25,x)
#define ND_X2_DCR__RESET__RCP_T8_set(v)       _BGQ_SET(1,25,v)
#define ND_X2_DCR__RESET__RCP_T8_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_t9
//   Description: Reset reception fifo torus 9
#define ND_X2_DCR__RESET__RCP_T9_width        1
#define ND_X2_DCR__RESET__RCP_T9_position     26
#define ND_X2_DCR__RESET__RCP_T9_get(x)       _BGQ_GET(1,26,x)
#define ND_X2_DCR__RESET__RCP_T9_set(v)       _BGQ_SET(1,26,v)
#define ND_X2_DCR__RESET__RCP_T9_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_l0
//   Description: Reset reception fifo local 0
#define ND_X2_DCR__RESET__RCP_L0_width        1
#define ND_X2_DCR__RESET__RCP_L0_position     27
#define ND_X2_DCR__RESET__RCP_L0_get(x)       _BGQ_GET(1,27,x)
#define ND_X2_DCR__RESET__RCP_L0_set(v)       _BGQ_SET(1,27,v)
#define ND_X2_DCR__RESET__RCP_L0_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_l1
//   Description: Reset reception fifo local 1
#define ND_X2_DCR__RESET__RCP_L1_width        1
#define ND_X2_DCR__RESET__RCP_L1_position     28
#define ND_X2_DCR__RESET__RCP_L1_get(x)       _BGQ_GET(1,28,x)
#define ND_X2_DCR__RESET__RCP_L1_set(v)       _BGQ_SET(1,28,v)
#define ND_X2_DCR__RESET__RCP_L1_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_hp
//   Description: Reset reception fifo torus high priority
#define ND_X2_DCR__RESET__RCP_HP_width        1
#define ND_X2_DCR__RESET__RCP_HP_position     29
#define ND_X2_DCR__RESET__RCP_HP_get(x)       _BGQ_GET(1,29,x)
#define ND_X2_DCR__RESET__RCP_HP_set(v)       _BGQ_SET(1,29,v)
#define ND_X2_DCR__RESET__RCP_HP_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_io
//   Description: Reset reception fifo torus system I/O
#define ND_X2_DCR__RESET__RCP_IO_width        1
#define ND_X2_DCR__RESET__RCP_IO_position     30
#define ND_X2_DCR__RESET__RCP_IO_get(x)       _BGQ_GET(1,30,x)
#define ND_X2_DCR__RESET__RCP_IO_set(v)       _BGQ_SET(1,30,v)
#define ND_X2_DCR__RESET__RCP_IO_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c0
//   Description: Reset reception fifo collective 0
#define ND_X2_DCR__RESET__RCP_C0_width        1
#define ND_X2_DCR__RESET__RCP_C0_position     31
#define ND_X2_DCR__RESET__RCP_C0_get(x)       _BGQ_GET(1,31,x)
#define ND_X2_DCR__RESET__RCP_C0_set(v)       _BGQ_SET(1,31,v)
#define ND_X2_DCR__RESET__RCP_C0_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       rcp_c1
//   Description: Reset reception fifo collective 1
#define ND_X2_DCR__RESET__RCP_C1_width        1
#define ND_X2_DCR__RESET__RCP_C1_position     32
#define ND_X2_DCR__RESET__RCP_C1_get(x)       _BGQ_GET(1,32,x)
#define ND_X2_DCR__RESET__RCP_C1_set(v)       _BGQ_SET(1,32,v)
#define ND_X2_DCR__RESET__RCP_C1_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: injection control x2.  When a unit_stop bit is 1, that unit will stop getting new packets from MU.  Packets in the injection fifo can still leave the injection fifo and get into the network
#define ND_X2_DCR__CTRL_offset  (0x0001) // Offset of register in instance 
#define ND_X2_DCR__CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_unit_stop
//   Description: injection fifo torus 0 unit stop
#define ND_X2_DCR__CTRL__INJ_T0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T0_UNIT_STOP_position     0
#define ND_X2_DCR__CTRL__INJ_T0_UNIT_STOP_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__CTRL__INJ_T0_UNIT_STOP_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__CTRL__INJ_T0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t1_unit_stop
//   Description: injection fifo torus 1 unit stop
#define ND_X2_DCR__CTRL__INJ_T1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T1_UNIT_STOP_position     1
#define ND_X2_DCR__CTRL__INJ_T1_UNIT_STOP_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__CTRL__INJ_T1_UNIT_STOP_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__CTRL__INJ_T1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t2_unit_stop
//   Description: injection fifo torus 2 unit stop
#define ND_X2_DCR__CTRL__INJ_T2_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T2_UNIT_STOP_position     2
#define ND_X2_DCR__CTRL__INJ_T2_UNIT_STOP_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__CTRL__INJ_T2_UNIT_STOP_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__CTRL__INJ_T2_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t3_unit_stop
//   Description: injection fifo torus 3 unit stop
#define ND_X2_DCR__CTRL__INJ_T3_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T3_UNIT_STOP_position     3
#define ND_X2_DCR__CTRL__INJ_T3_UNIT_STOP_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__CTRL__INJ_T3_UNIT_STOP_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__CTRL__INJ_T3_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t4_unit_stop
//   Description: injection fifo torus 4 unit stop
#define ND_X2_DCR__CTRL__INJ_T4_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T4_UNIT_STOP_position     4
#define ND_X2_DCR__CTRL__INJ_T4_UNIT_STOP_get(x)       _BGQ_GET(1,4,x)
#define ND_X2_DCR__CTRL__INJ_T4_UNIT_STOP_set(v)       _BGQ_SET(1,4,v)
#define ND_X2_DCR__CTRL__INJ_T4_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t5_unit_stop
//   Description: injection fifo torus 5 unit stop
#define ND_X2_DCR__CTRL__INJ_T5_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T5_UNIT_STOP_position     5
#define ND_X2_DCR__CTRL__INJ_T5_UNIT_STOP_get(x)       _BGQ_GET(1,5,x)
#define ND_X2_DCR__CTRL__INJ_T5_UNIT_STOP_set(v)       _BGQ_SET(1,5,v)
#define ND_X2_DCR__CTRL__INJ_T5_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t6_unit_stop
//   Description: injection fifo torus 6 unit stop
#define ND_X2_DCR__CTRL__INJ_T6_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T6_UNIT_STOP_position     6
#define ND_X2_DCR__CTRL__INJ_T6_UNIT_STOP_get(x)       _BGQ_GET(1,6,x)
#define ND_X2_DCR__CTRL__INJ_T6_UNIT_STOP_set(v)       _BGQ_SET(1,6,v)
#define ND_X2_DCR__CTRL__INJ_T6_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t7_unit_stop
//   Description: injection fifo torus 7 unit stop
#define ND_X2_DCR__CTRL__INJ_T7_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T7_UNIT_STOP_position     7
#define ND_X2_DCR__CTRL__INJ_T7_UNIT_STOP_get(x)       _BGQ_GET(1,7,x)
#define ND_X2_DCR__CTRL__INJ_T7_UNIT_STOP_set(v)       _BGQ_SET(1,7,v)
#define ND_X2_DCR__CTRL__INJ_T7_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t8_unit_stop
//   Description: injection fifo torus 8 unit stop
#define ND_X2_DCR__CTRL__INJ_T8_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T8_UNIT_STOP_position     8
#define ND_X2_DCR__CTRL__INJ_T8_UNIT_STOP_get(x)       _BGQ_GET(1,8,x)
#define ND_X2_DCR__CTRL__INJ_T8_UNIT_STOP_set(v)       _BGQ_SET(1,8,v)
#define ND_X2_DCR__CTRL__INJ_T8_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t9_unit_stop
//   Description: injection fifo torus 9 unit stop
#define ND_X2_DCR__CTRL__INJ_T9_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_T9_UNIT_STOP_position     9
#define ND_X2_DCR__CTRL__INJ_T9_UNIT_STOP_get(x)       _BGQ_GET(1,9,x)
#define ND_X2_DCR__CTRL__INJ_T9_UNIT_STOP_set(v)       _BGQ_SET(1,9,v)
#define ND_X2_DCR__CTRL__INJ_T9_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_l0_unit_stop
//   Description: injection fifo local 0 unit stop
#define ND_X2_DCR__CTRL__INJ_L0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_L0_UNIT_STOP_position     10
#define ND_X2_DCR__CTRL__INJ_L0_UNIT_STOP_get(x)       _BGQ_GET(1,10,x)
#define ND_X2_DCR__CTRL__INJ_L0_UNIT_STOP_set(v)       _BGQ_SET(1,10,v)
#define ND_X2_DCR__CTRL__INJ_L0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_l1_unit_stop
//   Description: injection fifo local 1 unit stop
#define ND_X2_DCR__CTRL__INJ_L1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_L1_UNIT_STOP_position     11
#define ND_X2_DCR__CTRL__INJ_L1_UNIT_STOP_get(x)       _BGQ_GET(1,11,x)
#define ND_X2_DCR__CTRL__INJ_L1_UNIT_STOP_set(v)       _BGQ_SET(1,11,v)
#define ND_X2_DCR__CTRL__INJ_L1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_hp_unit_stop
//   Description: injection fifo torus high priority unit stop
#define ND_X2_DCR__CTRL__INJ_HP_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_HP_UNIT_STOP_position     12
#define ND_X2_DCR__CTRL__INJ_HP_UNIT_STOP_get(x)       _BGQ_GET(1,12,x)
#define ND_X2_DCR__CTRL__INJ_HP_UNIT_STOP_set(v)       _BGQ_SET(1,12,v)
#define ND_X2_DCR__CTRL__INJ_HP_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_io_unit_stop
//   Description: injection fifo torus system I/O unit stop
#define ND_X2_DCR__CTRL__INJ_IO_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_IO_UNIT_STOP_position     13
#define ND_X2_DCR__CTRL__INJ_IO_UNIT_STOP_get(x)       _BGQ_GET(1,13,x)
#define ND_X2_DCR__CTRL__INJ_IO_UNIT_STOP_set(v)       _BGQ_SET(1,13,v)
#define ND_X2_DCR__CTRL__INJ_IO_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_c0_unit_stop
//   Description: injection fifo collective 0 (user) unit stop
#define ND_X2_DCR__CTRL__INJ_C0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_C0_UNIT_STOP_position     14
#define ND_X2_DCR__CTRL__INJ_C0_UNIT_STOP_get(x)       _BGQ_GET(1,14,x)
#define ND_X2_DCR__CTRL__INJ_C0_UNIT_STOP_set(v)       _BGQ_SET(1,14,v)
#define ND_X2_DCR__CTRL__INJ_C0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_c1_unit_stop
//   Description: injection fifo collective 1 (system) unit stop
#define ND_X2_DCR__CTRL__INJ_C1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__INJ_C1_UNIT_STOP_position     15
#define ND_X2_DCR__CTRL__INJ_C1_UNIT_STOP_get(x)       _BGQ_GET(1,15,x)
#define ND_X2_DCR__CTRL__INJ_C1_UNIT_STOP_set(v)       _BGQ_SET(1,15,v)
#define ND_X2_DCR__CTRL__INJ_C1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rcp_t0_unit_stop
//   Description: reception fifo torus 0 unit stop
#define ND_X2_DCR__CTRL__RCP_T0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T0_UNIT_STOP_position     16
#define ND_X2_DCR__CTRL__RCP_T0_UNIT_STOP_get(x)       _BGQ_GET(1,16,x)
#define ND_X2_DCR__CTRL__RCP_T0_UNIT_STOP_set(v)       _BGQ_SET(1,16,v)
#define ND_X2_DCR__CTRL__RCP_T0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t1_unit_stop
//   Description: reception fifo torus 1 unit stop
#define ND_X2_DCR__CTRL__RCP_T1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T1_UNIT_STOP_position     17
#define ND_X2_DCR__CTRL__RCP_T1_UNIT_STOP_get(x)       _BGQ_GET(1,17,x)
#define ND_X2_DCR__CTRL__RCP_T1_UNIT_STOP_set(v)       _BGQ_SET(1,17,v)
#define ND_X2_DCR__CTRL__RCP_T1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t2_unit_stop
//   Description: reception fifo torus 2 unit stop
#define ND_X2_DCR__CTRL__RCP_T2_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T2_UNIT_STOP_position     18
#define ND_X2_DCR__CTRL__RCP_T2_UNIT_STOP_get(x)       _BGQ_GET(1,18,x)
#define ND_X2_DCR__CTRL__RCP_T2_UNIT_STOP_set(v)       _BGQ_SET(1,18,v)
#define ND_X2_DCR__CTRL__RCP_T2_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t3_unit_stop
//   Description: reception fifo torus 3 unit stop
#define ND_X2_DCR__CTRL__RCP_T3_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T3_UNIT_STOP_position     19
#define ND_X2_DCR__CTRL__RCP_T3_UNIT_STOP_get(x)       _BGQ_GET(1,19,x)
#define ND_X2_DCR__CTRL__RCP_T3_UNIT_STOP_set(v)       _BGQ_SET(1,19,v)
#define ND_X2_DCR__CTRL__RCP_T3_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_t4_unit_stop
//   Description: reception fifo torus 4 unit stop
#define ND_X2_DCR__CTRL__RCP_T4_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T4_UNIT_STOP_position     20
#define ND_X2_DCR__CTRL__RCP_T4_UNIT_STOP_get(x)       _BGQ_GET(1,20,x)
#define ND_X2_DCR__CTRL__RCP_T4_UNIT_STOP_set(v)       _BGQ_SET(1,20,v)
#define ND_X2_DCR__CTRL__RCP_T4_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_t5_unit_stop
//   Description: reception fifo torus 5 unit stop
#define ND_X2_DCR__CTRL__RCP_T5_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T5_UNIT_STOP_position     21
#define ND_X2_DCR__CTRL__RCP_T5_UNIT_STOP_get(x)       _BGQ_GET(1,21,x)
#define ND_X2_DCR__CTRL__RCP_T5_UNIT_STOP_set(v)       _BGQ_SET(1,21,v)
#define ND_X2_DCR__CTRL__RCP_T5_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_t6_unit_stop
//   Description: reception fifo torus 6 unit stop
#define ND_X2_DCR__CTRL__RCP_T6_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T6_UNIT_STOP_position     22
#define ND_X2_DCR__CTRL__RCP_T6_UNIT_STOP_get(x)       _BGQ_GET(1,22,x)
#define ND_X2_DCR__CTRL__RCP_T6_UNIT_STOP_set(v)       _BGQ_SET(1,22,v)
#define ND_X2_DCR__CTRL__RCP_T6_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_t7_unit_stop
//   Description: reception fifo torus 7 unit stop
#define ND_X2_DCR__CTRL__RCP_T7_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T7_UNIT_STOP_position     23
#define ND_X2_DCR__CTRL__RCP_T7_UNIT_STOP_get(x)       _BGQ_GET(1,23,x)
#define ND_X2_DCR__CTRL__RCP_T7_UNIT_STOP_set(v)       _BGQ_SET(1,23,v)
#define ND_X2_DCR__CTRL__RCP_T7_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_t8_unit_stop
//   Description: reception fifo torus 8 unit stop
#define ND_X2_DCR__CTRL__RCP_T8_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T8_UNIT_STOP_position     24
#define ND_X2_DCR__CTRL__RCP_T8_UNIT_STOP_get(x)       _BGQ_GET(1,24,x)
#define ND_X2_DCR__CTRL__RCP_T8_UNIT_STOP_set(v)       _BGQ_SET(1,24,v)
#define ND_X2_DCR__CTRL__RCP_T8_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_t9_unit_stop
//   Description: reception fifo torus 9 unit stop
#define ND_X2_DCR__CTRL__RCP_T9_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_T9_UNIT_STOP_position     25
#define ND_X2_DCR__CTRL__RCP_T9_UNIT_STOP_get(x)       _BGQ_GET(1,25,x)
#define ND_X2_DCR__CTRL__RCP_T9_UNIT_STOP_set(v)       _BGQ_SET(1,25,v)
#define ND_X2_DCR__CTRL__RCP_T9_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_l0_unit_stop
//   Description: reception fifo local 0 unit stop
#define ND_X2_DCR__CTRL__RCP_L0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_L0_UNIT_STOP_position     26
#define ND_X2_DCR__CTRL__RCP_L0_UNIT_STOP_get(x)       _BGQ_GET(1,26,x)
#define ND_X2_DCR__CTRL__RCP_L0_UNIT_STOP_set(v)       _BGQ_SET(1,26,v)
#define ND_X2_DCR__CTRL__RCP_L0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_l1_unit_stop
//   Description: reception fifo local 1 unit stop
#define ND_X2_DCR__CTRL__RCP_L1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_L1_UNIT_STOP_position     27
#define ND_X2_DCR__CTRL__RCP_L1_UNIT_STOP_get(x)       _BGQ_GET(1,27,x)
#define ND_X2_DCR__CTRL__RCP_L1_UNIT_STOP_set(v)       _BGQ_SET(1,27,v)
#define ND_X2_DCR__CTRL__RCP_L1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_hp_unit_stop
//   Description: reception fifo torus high priority unit stop
#define ND_X2_DCR__CTRL__RCP_HP_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_HP_UNIT_STOP_position     28
#define ND_X2_DCR__CTRL__RCP_HP_UNIT_STOP_get(x)       _BGQ_GET(1,28,x)
#define ND_X2_DCR__CTRL__RCP_HP_UNIT_STOP_set(v)       _BGQ_SET(1,28,v)
#define ND_X2_DCR__CTRL__RCP_HP_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_io_unit_stop
//   Description: reception fifo torus system I/O unit stop
#define ND_X2_DCR__CTRL__RCP_IO_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_IO_UNIT_STOP_position     29
#define ND_X2_DCR__CTRL__RCP_IO_UNIT_STOP_get(x)       _BGQ_GET(1,29,x)
#define ND_X2_DCR__CTRL__RCP_IO_UNIT_STOP_set(v)       _BGQ_SET(1,29,v)
#define ND_X2_DCR__CTRL__RCP_IO_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_c0_unit_stop
//   Description: reception fifo collective 0 (user) unit stop
#define ND_X2_DCR__CTRL__RCP_C0_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_C0_UNIT_STOP_position     30
#define ND_X2_DCR__CTRL__RCP_C0_UNIT_STOP_get(x)       _BGQ_GET(1,30,x)
#define ND_X2_DCR__CTRL__RCP_C0_UNIT_STOP_set(v)       _BGQ_SET(1,30,v)
#define ND_X2_DCR__CTRL__RCP_C0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c1_unit_stop
//   Description: reception fifo collective 1 (system) unit stop
#define ND_X2_DCR__CTRL__RCP_C1_UNIT_STOP_width        1
#define ND_X2_DCR__CTRL__RCP_C1_UNIT_STOP_position     31
#define ND_X2_DCR__CTRL__RCP_C1_UNIT_STOP_get(x)       _BGQ_GET(1,31,x)
#define ND_X2_DCR__CTRL__RCP_C1_UNIT_STOP_set(v)       _BGQ_SET(1,31,v)
#define ND_X2_DCR__CTRL__RCP_C1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       reserved
//   Description: reserved
#define ND_X2_DCR__CTRL__RESERVED_width        32
#define ND_X2_DCR__CTRL__RESERVED_position     63
#define ND_X2_DCR__CTRL__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define ND_X2_DCR__CTRL__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define ND_X2_DCR__CTRL__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        fatal_err0_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: fatal error 0 enable, see corresponding bit definition in fatal error 0 below
#define ND_X2_DCR__FATAL_ERR0_ENABLE_offset  (0x0002) // Offset of register in instance 
#define ND_X2_DCR__FATAL_ERR0_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: fatal error 0 enable
#define ND_X2_DCR__FATAL_ERR0_ENABLE__VALUE_width        64
#define ND_X2_DCR__FATAL_ERR0_ENABLE__VALUE_position     63
#define ND_X2_DCR__FATAL_ERR0_ENABLE__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_X2_DCR__FATAL_ERR0_ENABLE__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_X2_DCR__FATAL_ERR0_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        fatal_err0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 0, bit 0 to 62 goes into nd_gip_int(0), bit 63 goes to nd_gip_int(1)
#define ND_X2_DCR__FATAL_ERR0_offset  (0x0003) // Offset of register in instance 
#define ND_X2_DCR__FATAL_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_t3_x2_perr
//   Description: injection fifo t0-t3 clk_x2 parity error
#define ND_X2_DCR__FATAL_ERR0__INJ_T0_T3_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR0__INJ_T0_T3_X2_PERR_position     0
#define ND_X2_DCR__FATAL_ERR0__INJ_T0_T3_X2_PERR_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__FATAL_ERR0__INJ_T0_T3_X2_PERR_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__FATAL_ERR0__INJ_T0_T3_X2_PERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t4_t7_x2_perr
//   Description: injection fifo t4-t7 clk_x2 parity error
#define ND_X2_DCR__FATAL_ERR0__INJ_T4_T7_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR0__INJ_T4_T7_X2_PERR_position     1
#define ND_X2_DCR__FATAL_ERR0__INJ_T4_T7_X2_PERR_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__FATAL_ERR0__INJ_T4_T7_X2_PERR_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__FATAL_ERR0__INJ_T4_T7_X2_PERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t8_l1_x2_perr
//   Description: injection fifo t8, t9, l0, l1 clk_x2 parity error
#define ND_X2_DCR__FATAL_ERR0__INJ_T8_L1_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR0__INJ_T8_L1_X2_PERR_position     2
#define ND_X2_DCR__FATAL_ERR0__INJ_T8_L1_X2_PERR_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__FATAL_ERR0__INJ_T8_L1_X2_PERR_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__FATAL_ERR0__INJ_T8_L1_X2_PERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_hp_c1_x2_perr
//   Description: injection fifo hp, io, c0, c1 clk_x2 parity error
#define ND_X2_DCR__FATAL_ERR0__INJ_HP_C1_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR0__INJ_HP_C1_X2_PERR_position     3
#define ND_X2_DCR__FATAL_ERR0__INJ_HP_C1_X2_PERR_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__FATAL_ERR0__INJ_HP_C1_X2_PERR_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__FATAL_ERR0__INJ_HP_C1_X2_PERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       mu_dbus_en_ptr_en_both_high
//   Description: MU drives dbus enable and ptr enable for same injection fifo high in the same cycle
#define ND_X2_DCR__FATAL_ERR0__MU_DBUS_EN_PTR_EN_BOTH_HIGH_width        1
#define ND_X2_DCR__FATAL_ERR0__MU_DBUS_EN_PTR_EN_BOTH_HIGH_position     4
#define ND_X2_DCR__FATAL_ERR0__MU_DBUS_EN_PTR_EN_BOTH_HIGH_get(x)       _BGQ_GET(1,4,x)
#define ND_X2_DCR__FATAL_ERR0__MU_DBUS_EN_PTR_EN_BOTH_HIGH_set(v)       _BGQ_SET(1,4,v)
#define ND_X2_DCR__FATAL_ERR0__MU_DBUS_EN_PTR_EN_BOTH_HIGH_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_top_x2_perr
//   Description: injection fifo top level clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR0__INJ_TOP_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR0__INJ_TOP_X2_PERR_position     5
#define ND_X2_DCR__FATAL_ERR0__INJ_TOP_X2_PERR_get(x)       _BGQ_GET(1,5,x)
#define ND_X2_DCR__FATAL_ERR0__INJ_TOP_X2_PERR_set(v)       _BGQ_SET(1,5,v)
#define ND_X2_DCR__FATAL_ERR0__INJ_TOP_X2_PERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       mu_t0_t3_dbus_en_gt1
//   Description: MU drives more than one t0-t3 dbus enables high
#define ND_X2_DCR__FATAL_ERR0__MU_T0_T3_DBUS_EN_GT1_width        1
#define ND_X2_DCR__FATAL_ERR0__MU_T0_T3_DBUS_EN_GT1_position     6
#define ND_X2_DCR__FATAL_ERR0__MU_T0_T3_DBUS_EN_GT1_get(x)       _BGQ_GET(1,6,x)
#define ND_X2_DCR__FATAL_ERR0__MU_T0_T3_DBUS_EN_GT1_set(v)       _BGQ_SET(1,6,v)
#define ND_X2_DCR__FATAL_ERR0__MU_T0_T3_DBUS_EN_GT1_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       mu_t4_t7_dbus_en_gt1
//   Description: MU drives more than one t4-t7 dbus enables high
#define ND_X2_DCR__FATAL_ERR0__MU_T4_T7_DBUS_EN_GT1_width        1
#define ND_X2_DCR__FATAL_ERR0__MU_T4_T7_DBUS_EN_GT1_position     7
#define ND_X2_DCR__FATAL_ERR0__MU_T4_T7_DBUS_EN_GT1_get(x)       _BGQ_GET(1,7,x)
#define ND_X2_DCR__FATAL_ERR0__MU_T4_T7_DBUS_EN_GT1_set(v)       _BGQ_SET(1,7,v)
#define ND_X2_DCR__FATAL_ERR0__MU_T4_T7_DBUS_EN_GT1_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       mu_t8_l1_dbus_en_gt1
//   Description: MU drives more than one of t8, t9, l0, l1 dbus enables high
#define ND_X2_DCR__FATAL_ERR0__MU_T8_L1_DBUS_EN_GT1_width        1
#define ND_X2_DCR__FATAL_ERR0__MU_T8_L1_DBUS_EN_GT1_position     8
#define ND_X2_DCR__FATAL_ERR0__MU_T8_L1_DBUS_EN_GT1_get(x)       _BGQ_GET(1,8,x)
#define ND_X2_DCR__FATAL_ERR0__MU_T8_L1_DBUS_EN_GT1_set(v)       _BGQ_SET(1,8,v)
#define ND_X2_DCR__FATAL_ERR0__MU_T8_L1_DBUS_EN_GT1_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       reserved
//   Description: reserved
#define ND_X2_DCR__FATAL_ERR0__RESERVED_width        52
#define ND_X2_DCR__FATAL_ERR0__RESERVED_position     60
#define ND_X2_DCR__FATAL_ERR0__RESERVED_get(x)       _BGQ_GET(52,60,x)
#define ND_X2_DCR__FATAL_ERR0__RESERVED_set(v)       _BGQ_SET(52,60,v)
#define ND_X2_DCR__FATAL_ERR0__RESERVED_insert(x,v)  _BGQ_INSERT(52,60,x,v)

//   Field:       dcr_slave_mchk
//   Description: dcr slave machine check interrupt
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_MCHK_width        1
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_MCHK_position     61
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_MCHK_get(x)       _BGQ_GET(1,61,x)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_MCHK_set(v)       _BGQ_SET(1,61,v)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_MCHK_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       dcr_slave_crit
//   Description: dcr slave critical interrupt
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_CRIT_width        1
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_CRIT_position     62
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_CRIT_get(x)       _BGQ_GET(1,62,x)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_CRIT_set(v)       _BGQ_SET(1,62,v)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_CRIT_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       dcr_slave_non_crit
//   Description: dcr slave non critical interrupt, tie to non fatal error, nd_gip_int(1)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_NON_CRIT_width        1
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_NON_CRIT_position     63
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_NON_CRIT_get(x)       _BGQ_GET(1,63,x)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_NON_CRIT_set(v)       _BGQ_SET(1,63,v)
#define ND_X2_DCR__FATAL_ERR0__DCR_SLAVE_NON_CRIT_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        fatal_err1_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: fatal error 1 enable, see corresponding bit definition in fatal error 1 below
#define ND_X2_DCR__FATAL_ERR1_ENABLE_offset  (0x0004) // Offset of register in instance 
#define ND_X2_DCR__FATAL_ERR1_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: fatal error 1 enable
#define ND_X2_DCR__FATAL_ERR1_ENABLE__VALUE_width        64
#define ND_X2_DCR__FATAL_ERR1_ENABLE__VALUE_position     63
#define ND_X2_DCR__FATAL_ERR1_ENABLE__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_X2_DCR__FATAL_ERR1_ENABLE__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_X2_DCR__FATAL_ERR1_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        fatal_err1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 1
#define ND_X2_DCR__FATAL_ERR1_offset  (0x0005) // Offset of register in instance 
#define ND_X2_DCR__FATAL_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_x2_perr
//   Description: reception fifo t0 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_PERR_position     0
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_PERR_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_PERR_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_PERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       rcp_t0_x2_inv_state
//   Description: reception fifo t0 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_INV_STATE_position     1
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_INV_STATE_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_INV_STATE_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       rcp_t1_x2_perr
//   Description: reception fifo t1 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_PERR_position     2
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_PERR_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_PERR_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_PERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       rcp_t1_x2_inv_state
//   Description: reception fifo t1 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_INV_STATE_position     3
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_INV_STATE_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_INV_STATE_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       rcp_t2_x2_perr
//   Description: reception fifo t2 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_PERR_position     4
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_PERR_get(x)       _BGQ_GET(1,4,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_PERR_set(v)       _BGQ_SET(1,4,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_PERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       rcp_t2_x2_inv_state
//   Description: reception fifo t2 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_INV_STATE_position     5
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_INV_STATE_get(x)       _BGQ_GET(1,5,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_INV_STATE_set(v)       _BGQ_SET(1,5,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       rcp_t3_x2_perr
//   Description: reception fifo t3 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_PERR_position     6
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_PERR_get(x)       _BGQ_GET(1,6,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_PERR_set(v)       _BGQ_SET(1,6,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_PERR_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       rcp_t3_x2_inv_state
//   Description: reception fifo t3 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_INV_STATE_position     7
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_INV_STATE_get(x)       _BGQ_GET(1,7,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_INV_STATE_set(v)       _BGQ_SET(1,7,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       rcp_t4_x2_perr
//   Description: reception fifo t4 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_PERR_position     8
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_PERR_get(x)       _BGQ_GET(1,8,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_PERR_set(v)       _BGQ_SET(1,8,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_PERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       rcp_t4_x2_inv_state
//   Description: reception fifo t4 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_INV_STATE_position     9
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_INV_STATE_get(x)       _BGQ_GET(1,9,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_INV_STATE_set(v)       _BGQ_SET(1,9,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       rcp_t5_x2_perr
//   Description: reception fifo t5 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_PERR_position     10
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_PERR_get(x)       _BGQ_GET(1,10,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_PERR_set(v)       _BGQ_SET(1,10,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_PERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       rcp_t5_x2_inv_state
//   Description: reception fifo t5 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_INV_STATE_position     11
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_INV_STATE_get(x)       _BGQ_GET(1,11,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_INV_STATE_set(v)       _BGQ_SET(1,11,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       rcp_t6_x2_perr
//   Description: reception fifo t6 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_PERR_position     12
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_PERR_get(x)       _BGQ_GET(1,12,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_PERR_set(v)       _BGQ_SET(1,12,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_PERR_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       rcp_t6_x2_inv_state
//   Description: reception fifo t6 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_INV_STATE_position     13
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_INV_STATE_get(x)       _BGQ_GET(1,13,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_INV_STATE_set(v)       _BGQ_SET(1,13,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       rcp_t7_x2_perr
//   Description: reception fifo t7 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_PERR_position     14
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_PERR_get(x)       _BGQ_GET(1,14,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_PERR_set(v)       _BGQ_SET(1,14,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_PERR_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       rcp_t7_x2_inv_state
//   Description: reception fifo t7 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_INV_STATE_position     15
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_INV_STATE_get(x)       _BGQ_GET(1,15,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_INV_STATE_set(v)       _BGQ_SET(1,15,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rcp_t8_x2_perr
//   Description: reception fifo t8 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_PERR_position     16
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_PERR_get(x)       _BGQ_GET(1,16,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_PERR_set(v)       _BGQ_SET(1,16,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_PERR_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t8_x2_inv_state
//   Description: reception fifo t8 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_INV_STATE_position     17
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_INV_STATE_get(x)       _BGQ_GET(1,17,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_INV_STATE_set(v)       _BGQ_SET(1,17,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t9_x2_perr
//   Description: reception fifo t9 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_PERR_position     18
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_PERR_get(x)       _BGQ_GET(1,18,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_PERR_set(v)       _BGQ_SET(1,18,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_PERR_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t9_x2_inv_state
//   Description: reception fifo t9 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_INV_STATE_position     19
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_INV_STATE_get(x)       _BGQ_GET(1,19,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_INV_STATE_set(v)       _BGQ_SET(1,19,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_l0_x2_perr
//   Description: reception fifo l0 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_PERR_position     20
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_PERR_get(x)       _BGQ_GET(1,20,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_PERR_set(v)       _BGQ_SET(1,20,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_PERR_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_l0_x2_inv_state
//   Description: reception fifo l0 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_INV_STATE_position     21
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_INV_STATE_get(x)       _BGQ_GET(1,21,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_INV_STATE_set(v)       _BGQ_SET(1,21,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_l1_x2_perr
//   Description: reception fifo l1 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_PERR_position     22
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_PERR_get(x)       _BGQ_GET(1,22,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_PERR_set(v)       _BGQ_SET(1,22,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_PERR_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_l1_x2_inv_state
//   Description: reception fifo l1 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_INV_STATE_position     23
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_INV_STATE_get(x)       _BGQ_GET(1,23,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_INV_STATE_set(v)       _BGQ_SET(1,23,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_hp_x2_perr
//   Description: reception fifo hp clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_PERR_position     24
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_PERR_get(x)       _BGQ_GET(1,24,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_PERR_set(v)       _BGQ_SET(1,24,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_PERR_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_hp_x2_inv_state
//   Description: reception fifo hp clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_INV_STATE_position     25
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_INV_STATE_get(x)       _BGQ_GET(1,25,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_INV_STATE_set(v)       _BGQ_SET(1,25,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_io_x2_perr
//   Description: reception fifo io clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_PERR_position     26
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_PERR_get(x)       _BGQ_GET(1,26,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_PERR_set(v)       _BGQ_SET(1,26,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_PERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_io_x2_inv_state
//   Description: reception fifo io clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_INV_STATE_position     27
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_INV_STATE_get(x)       _BGQ_GET(1,27,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_INV_STATE_set(v)       _BGQ_SET(1,27,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_c0_x2_perr
//   Description: reception fifo c0 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_PERR_position     28
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_PERR_get(x)       _BGQ_GET(1,28,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_PERR_set(v)       _BGQ_SET(1,28,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_PERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_c0_x2_inv_state
//   Description: reception fifo c0 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_INV_STATE_position     29
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_INV_STATE_get(x)       _BGQ_GET(1,29,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_INV_STATE_set(v)       _BGQ_SET(1,29,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_c1_x2_perr
//   Description: reception fifo c1 clk_x2 parity err
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_PERR_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_PERR_position     30
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_PERR_get(x)       _BGQ_GET(1,30,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_PERR_set(v)       _BGQ_SET(1,30,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_PERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c1_x2_inv_state
//   Description: reception fifo c1 clk_x2 invalid state
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_INV_STATE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_INV_STATE_position     31
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_INV_STATE_get(x)       _BGQ_GET(1,31,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_INV_STATE_set(v)       _BGQ_SET(1,31,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_X2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       rcp_t0_mu_inv_done
//   Description: reception fifo t0 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_DONE_position     32
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_DONE_get(x)       _BGQ_GET(1,32,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_DONE_set(v)       _BGQ_SET(1,32,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       rcp_t0_mu_inv_req
//   Description: reception fifo t0 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_REQ_position     33
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_REQ_get(x)       _BGQ_GET(1,33,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_REQ_set(v)       _BGQ_SET(1,33,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T0_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       rcp_t1_mu_inv_done
//   Description: reception fifo t1 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_DONE_position     34
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_DONE_get(x)       _BGQ_GET(1,34,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_DONE_set(v)       _BGQ_SET(1,34,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       rcp_t1_mu_inv_req
//   Description: reception fifo t1 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_REQ_position     35
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_REQ_get(x)       _BGQ_GET(1,35,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_REQ_set(v)       _BGQ_SET(1,35,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T1_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       rcp_t2_mu_inv_done
//   Description: reception fifo t2 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_DONE_position     36
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_DONE_get(x)       _BGQ_GET(1,36,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_DONE_set(v)       _BGQ_SET(1,36,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       rcp_t2_mu_inv_req
//   Description: reception fifo t2 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_REQ_position     37
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_REQ_get(x)       _BGQ_GET(1,37,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_REQ_set(v)       _BGQ_SET(1,37,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T2_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       rcp_t3_mu_inv_done
//   Description: reception fifo t3 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_DONE_position     38
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_DONE_get(x)       _BGQ_GET(1,38,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_DONE_set(v)       _BGQ_SET(1,38,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       rcp_t3_mu_inv_req
//   Description: reception fifo t3 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_REQ_position     39
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_REQ_get(x)       _BGQ_GET(1,39,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_REQ_set(v)       _BGQ_SET(1,39,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T3_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       rcp_t4_mu_inv_done
//   Description: reception fifo t4 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_DONE_position     40
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_DONE_get(x)       _BGQ_GET(1,40,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_DONE_set(v)       _BGQ_SET(1,40,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       rcp_t4_mu_inv_req
//   Description: reception fifo t4 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_REQ_position     41
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_REQ_get(x)       _BGQ_GET(1,41,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_REQ_set(v)       _BGQ_SET(1,41,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T4_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,41,x,v)

//   Field:       rcp_t5_mu_inv_done
//   Description: reception fifo t5 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_DONE_position     42
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_DONE_get(x)       _BGQ_GET(1,42,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_DONE_set(v)       _BGQ_SET(1,42,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       rcp_t5_mu_inv_req
//   Description: reception fifo t5 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_REQ_position     43
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_REQ_get(x)       _BGQ_GET(1,43,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_REQ_set(v)       _BGQ_SET(1,43,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T5_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       rcp_t6_mu_inv_done
//   Description: reception fifo t6 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_DONE_position     44
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_DONE_get(x)       _BGQ_GET(1,44,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_DONE_set(v)       _BGQ_SET(1,44,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       rcp_t6_mu_inv_req
//   Description: reception fifo t6 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_REQ_position     45
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_REQ_get(x)       _BGQ_GET(1,45,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_REQ_set(v)       _BGQ_SET(1,45,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T6_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       rcp_t7_mu_inv_done
//   Description: reception fifo t7 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_DONE_position     46
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_DONE_get(x)       _BGQ_GET(1,46,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_DONE_set(v)       _BGQ_SET(1,46,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       rcp_t7_mu_inv_req
//   Description: reception fifo t7 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_REQ_position     47
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_REQ_get(x)       _BGQ_GET(1,47,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_REQ_set(v)       _BGQ_SET(1,47,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T7_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       rcp_t8_mu_inv_done
//   Description: reception fifo t8 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_DONE_position     48
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_DONE_get(x)       _BGQ_GET(1,48,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_DONE_set(v)       _BGQ_SET(1,48,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       rcp_t8_mu_inv_req
//   Description: reception fifo t8 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_REQ_position     49
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_REQ_get(x)       _BGQ_GET(1,49,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_REQ_set(v)       _BGQ_SET(1,49,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T8_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       rcp_t9_mu_inv_done
//   Description: reception fifo t9 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_DONE_position     50
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_DONE_get(x)       _BGQ_GET(1,50,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_DONE_set(v)       _BGQ_SET(1,50,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,50,x,v)

//   Field:       rcp_t9_mu_inv_req
//   Description: reception fifo t9 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_REQ_position     51
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_REQ_get(x)       _BGQ_GET(1,51,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_REQ_set(v)       _BGQ_SET(1,51,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_T9_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,51,x,v)

//   Field:       rcp_l0_mu_inv_done
//   Description: reception fifo l0 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_DONE_position     52
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_DONE_get(x)       _BGQ_GET(1,52,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_DONE_set(v)       _BGQ_SET(1,52,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,52,x,v)

//   Field:       rcp_l0_mu_inv_req
//   Description: reception fifo l0 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_REQ_position     53
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_REQ_get(x)       _BGQ_GET(1,53,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_REQ_set(v)       _BGQ_SET(1,53,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L0_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,53,x,v)

//   Field:       rcp_l1_mu_inv_done
//   Description: reception fifo l1 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_DONE_position     54
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_DONE_get(x)       _BGQ_GET(1,54,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_DONE_set(v)       _BGQ_SET(1,54,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,54,x,v)

//   Field:       rcp_l1_mu_inv_req
//   Description: reception fifo l1 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_REQ_position     55
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_REQ_get(x)       _BGQ_GET(1,55,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_REQ_set(v)       _BGQ_SET(1,55,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_L1_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,55,x,v)

//   Field:       rcp_hp_mu_inv_done
//   Description: reception fifo hp raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_DONE_position     56
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_DONE_get(x)       _BGQ_GET(1,56,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_DONE_set(v)       _BGQ_SET(1,56,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,56,x,v)

//   Field:       rcp_hp_mu_inv_req
//   Description: reception fifo hp raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_REQ_position     57
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_REQ_get(x)       _BGQ_GET(1,57,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_REQ_set(v)       _BGQ_SET(1,57,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_HP_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,57,x,v)

//   Field:       rcp_io_mu_inv_done
//   Description: reception fifo io raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_DONE_position     58
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_DONE_get(x)       _BGQ_GET(1,58,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_DONE_set(v)       _BGQ_SET(1,58,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,58,x,v)

//   Field:       rcp_io_mu_inv_req
//   Description: reception fifo io raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_REQ_position     59
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_REQ_get(x)       _BGQ_GET(1,59,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_REQ_set(v)       _BGQ_SET(1,59,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_IO_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,59,x,v)

//   Field:       rcp_c0_mu_inv_done
//   Description: reception fifo c0 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_DONE_position     60
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_DONE_get(x)       _BGQ_GET(1,60,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_DONE_set(v)       _BGQ_SET(1,60,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,60,x,v)

//   Field:       rcp_c0_mu_inv_req
//   Description: reception fifo c0 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_REQ_position     61
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_REQ_get(x)       _BGQ_GET(1,61,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_REQ_set(v)       _BGQ_SET(1,61,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C0_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       rcp_c1_mu_inv_done
//   Description: reception fifo c1 raises done during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_DONE_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_DONE_position     62
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_DONE_get(x)       _BGQ_GET(1,62,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_DONE_set(v)       _BGQ_SET(1,62,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_DONE_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       rcp_c1_mu_inv_req
//   Description: reception fifo c1 raises request during wrong state
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_REQ_width        1
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_REQ_position     63
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_REQ_get(x)       _BGQ_GET(1,63,x)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_REQ_set(v)       _BGQ_SET(1,63,v)
#define ND_X2_DCR__FATAL_ERR1__RCP_C1_MU_INV_REQ_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        backdoor_ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Array backdoor control
#define ND_X2_DCR__BACKDOOR_CTRL_offset  (0x0006) // Offset of register in instance 
#define ND_X2_DCR__BACKDOOR_CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       en
//   Description: backdoor access enable, set to 11 to enable backdoor
#define ND_X2_DCR__BACKDOOR_CTRL__EN_width        2
#define ND_X2_DCR__BACKDOOR_CTRL__EN_position     1
#define ND_X2_DCR__BACKDOOR_CTRL__EN_get(x)       _BGQ_GET(2,1,x)
#define ND_X2_DCR__BACKDOOR_CTRL__EN_set(v)       _BGQ_SET(2,1,v)
#define ND_X2_DCR__BACKDOOR_CTRL__EN_insert(x,v)  _BGQ_INSERT(2,1,x,v)

//   Field:       write
//   Description: backdoor access write
#define ND_X2_DCR__BACKDOOR_CTRL__WRITE_width        1
#define ND_X2_DCR__BACKDOOR_CTRL__WRITE_position     2
#define ND_X2_DCR__BACKDOOR_CTRL__WRITE_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__BACKDOOR_CTRL__WRITE_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__BACKDOOR_CTRL__WRITE_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       read
//   Description: backdoor access read
#define ND_X2_DCR__BACKDOOR_CTRL__READ_width        1
#define ND_X2_DCR__BACKDOOR_CTRL__READ_position     3
#define ND_X2_DCR__BACKDOOR_CTRL__READ_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__BACKDOOR_CTRL__READ_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__BACKDOOR_CTRL__READ_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       array_sel
//   Description: backdoor access array sel, 00000 to 00101 for injection fifo write, 10000 to 11111 for reception fifo read
#define ND_X2_DCR__BACKDOOR_CTRL__ARRAY_SEL_width        5
#define ND_X2_DCR__BACKDOOR_CTRL__ARRAY_SEL_position     11
#define ND_X2_DCR__BACKDOOR_CTRL__ARRAY_SEL_get(x)       _BGQ_GET(5,11,x)
#define ND_X2_DCR__BACKDOOR_CTRL__ARRAY_SEL_set(v)       _BGQ_SET(5,11,v)
#define ND_X2_DCR__BACKDOOR_CTRL__ARRAY_SEL_insert(x,v)  _BGQ_INSERT(5,11,x,v)

//   Field:       address
//   Description: backdoor access array address
#define ND_X2_DCR__BACKDOOR_CTRL__ADDRESS_width        10
#define ND_X2_DCR__BACKDOOR_CTRL__ADDRESS_position     23
#define ND_X2_DCR__BACKDOOR_CTRL__ADDRESS_get(x)       _BGQ_GET(10,23,x)
#define ND_X2_DCR__BACKDOOR_CTRL__ADDRESS_set(v)       _BGQ_SET(10,23,v)
#define ND_X2_DCR__BACKDOOR_CTRL__ADDRESS_insert(x,v)  _BGQ_INSERT(10,23,x,v)

  
// Register:
//   Name:        backdoor_wr_data
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor write data n
#define ND_X2_DCR__BACKDOOR_WR_DATA_offset  (0x0007) // Offset of register in instance 
#define ND_X2_DCR__BACKDOOR_WR_DATA_range  (0x2) // Range of external address space
#define ND_X2_DCR__BACKDOOR_WR_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor write data 0
#define ND_X2_DCR__BACKDOOR_WR_DATA__VALUE_width        64
#define ND_X2_DCR__BACKDOOR_WR_DATA__VALUE_position     63
#define ND_X2_DCR__BACKDOOR_WR_DATA__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_X2_DCR__BACKDOOR_WR_DATA__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_X2_DCR__BACKDOOR_WR_DATA__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_wr_ecc
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor write ecc
#define ND_X2_DCR__BACKDOOR_WR_ECC_offset  (0x0009) // Offset of register in instance 
#define ND_X2_DCR__BACKDOOR_WR_ECC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor write ecc 
#define ND_X2_DCR__BACKDOOR_WR_ECC__VALUE_width        16
#define ND_X2_DCR__BACKDOOR_WR_ECC__VALUE_position     63
#define ND_X2_DCR__BACKDOOR_WR_ECC__VALUE_get(x)       _BGQ_GET(16,63,x)
#define ND_X2_DCR__BACKDOOR_WR_ECC__VALUE_set(v)       _BGQ_SET(16,63,v)
#define ND_X2_DCR__BACKDOOR_WR_ECC__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        backdoor_rd
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: backdoor read data, ecc
#define ND_X2_DCR__BACKDOOR_RD_offset  (0x000A) // Offset of register in instance 
#define ND_X2_DCR__BACKDOOR_RD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ecc
//   Description: backdoor read ecc
#define ND_X2_DCR__BACKDOOR_RD__ECC_width        7
#define ND_X2_DCR__BACKDOOR_RD__ECC_position     31
#define ND_X2_DCR__BACKDOOR_RD__ECC_get(x)       _BGQ_GET(7,31,x)
#define ND_X2_DCR__BACKDOOR_RD__ECC_set(v)       _BGQ_SET(7,31,v)
#define ND_X2_DCR__BACKDOOR_RD__ECC_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       data
//   Description: backdoor read data
#define ND_X2_DCR__BACKDOOR_RD__DATA_width        32
#define ND_X2_DCR__BACKDOOR_RD__DATA_position     63
#define ND_X2_DCR__BACKDOOR_RD__DATA_get(x)       _BGQ_GET(32,63,x)
#define ND_X2_DCR__BACKDOOR_RD__DATA_set(v)       _BGQ_SET(32,63,v)
#define ND_X2_DCR__BACKDOOR_RD__DATA_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        stat0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 0, reception fifo mu interface state
#define ND_X2_DCR__STAT0_offset  (0x000B) // Offset of register in instance 
#define ND_X2_DCR__STAT0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_mu_state
//   Description: reception fifo T0 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T0_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T0_MU_STATE_position     2
#define ND_X2_DCR__STAT0__RCP_T0_MU_STATE_get(x)       _BGQ_GET(3,2,x)
#define ND_X2_DCR__STAT0__RCP_T0_MU_STATE_set(v)       _BGQ_SET(3,2,v)
#define ND_X2_DCR__STAT0__RCP_T0_MU_STATE_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       rcp_t1_mu_state
//   Description: reception fifo T1 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T1_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T1_MU_STATE_position     5
#define ND_X2_DCR__STAT0__RCP_T1_MU_STATE_get(x)       _BGQ_GET(3,5,x)
#define ND_X2_DCR__STAT0__RCP_T1_MU_STATE_set(v)       _BGQ_SET(3,5,v)
#define ND_X2_DCR__STAT0__RCP_T1_MU_STATE_insert(x,v)  _BGQ_INSERT(3,5,x,v)

//   Field:       rcp_t2_mu_state
//   Description: reception fifo T2 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T2_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T2_MU_STATE_position     8
#define ND_X2_DCR__STAT0__RCP_T2_MU_STATE_get(x)       _BGQ_GET(3,8,x)
#define ND_X2_DCR__STAT0__RCP_T2_MU_STATE_set(v)       _BGQ_SET(3,8,v)
#define ND_X2_DCR__STAT0__RCP_T2_MU_STATE_insert(x,v)  _BGQ_INSERT(3,8,x,v)

//   Field:       rcp_t3_mu_state
//   Description: reception fifo T3 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T3_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T3_MU_STATE_position     11
#define ND_X2_DCR__STAT0__RCP_T3_MU_STATE_get(x)       _BGQ_GET(3,11,x)
#define ND_X2_DCR__STAT0__RCP_T3_MU_STATE_set(v)       _BGQ_SET(3,11,v)
#define ND_X2_DCR__STAT0__RCP_T3_MU_STATE_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       rcp_t4_mu_state
//   Description: reception fifo T4 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T4_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T4_MU_STATE_position     14
#define ND_X2_DCR__STAT0__RCP_T4_MU_STATE_get(x)       _BGQ_GET(3,14,x)
#define ND_X2_DCR__STAT0__RCP_T4_MU_STATE_set(v)       _BGQ_SET(3,14,v)
#define ND_X2_DCR__STAT0__RCP_T4_MU_STATE_insert(x,v)  _BGQ_INSERT(3,14,x,v)

//   Field:       rcp_t5_mu_state
//   Description: reception fifo T5 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T5_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T5_MU_STATE_position     17
#define ND_X2_DCR__STAT0__RCP_T5_MU_STATE_get(x)       _BGQ_GET(3,17,x)
#define ND_X2_DCR__STAT0__RCP_T5_MU_STATE_set(v)       _BGQ_SET(3,17,v)
#define ND_X2_DCR__STAT0__RCP_T5_MU_STATE_insert(x,v)  _BGQ_INSERT(3,17,x,v)

//   Field:       rcp_t6_mu_state
//   Description: reception fifo T6 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T6_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T6_MU_STATE_position     20
#define ND_X2_DCR__STAT0__RCP_T6_MU_STATE_get(x)       _BGQ_GET(3,20,x)
#define ND_X2_DCR__STAT0__RCP_T6_MU_STATE_set(v)       _BGQ_SET(3,20,v)
#define ND_X2_DCR__STAT0__RCP_T6_MU_STATE_insert(x,v)  _BGQ_INSERT(3,20,x,v)

//   Field:       rcp_t7_mu_state
//   Description: reception fifo T7 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T7_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T7_MU_STATE_position     23
#define ND_X2_DCR__STAT0__RCP_T7_MU_STATE_get(x)       _BGQ_GET(3,23,x)
#define ND_X2_DCR__STAT0__RCP_T7_MU_STATE_set(v)       _BGQ_SET(3,23,v)
#define ND_X2_DCR__STAT0__RCP_T7_MU_STATE_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       rcp_t8_mu_state
//   Description: reception fifo T8 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T8_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T8_MU_STATE_position     26
#define ND_X2_DCR__STAT0__RCP_T8_MU_STATE_get(x)       _BGQ_GET(3,26,x)
#define ND_X2_DCR__STAT0__RCP_T8_MU_STATE_set(v)       _BGQ_SET(3,26,v)
#define ND_X2_DCR__STAT0__RCP_T8_MU_STATE_insert(x,v)  _BGQ_INSERT(3,26,x,v)

//   Field:       rcp_t9_mu_state
//   Description: reception fifo T9 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_T9_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_T9_MU_STATE_position     29
#define ND_X2_DCR__STAT0__RCP_T9_MU_STATE_get(x)       _BGQ_GET(3,29,x)
#define ND_X2_DCR__STAT0__RCP_T9_MU_STATE_set(v)       _BGQ_SET(3,29,v)
#define ND_X2_DCR__STAT0__RCP_T9_MU_STATE_insert(x,v)  _BGQ_INSERT(3,29,x,v)

//   Field:       rcp_l0_mu_state
//   Description: reception fifo L0 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_L0_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_L0_MU_STATE_position     32
#define ND_X2_DCR__STAT0__RCP_L0_MU_STATE_get(x)       _BGQ_GET(3,32,x)
#define ND_X2_DCR__STAT0__RCP_L0_MU_STATE_set(v)       _BGQ_SET(3,32,v)
#define ND_X2_DCR__STAT0__RCP_L0_MU_STATE_insert(x,v)  _BGQ_INSERT(3,32,x,v)

//   Field:       rcp_l1_mu_state
//   Description: reception fifo L1 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_L1_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_L1_MU_STATE_position     35
#define ND_X2_DCR__STAT0__RCP_L1_MU_STATE_get(x)       _BGQ_GET(3,35,x)
#define ND_X2_DCR__STAT0__RCP_L1_MU_STATE_set(v)       _BGQ_SET(3,35,v)
#define ND_X2_DCR__STAT0__RCP_L1_MU_STATE_insert(x,v)  _BGQ_INSERT(3,35,x,v)

//   Field:       rcp_hp_mu_state
//   Description: reception fifo HP MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_HP_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_HP_MU_STATE_position     38
#define ND_X2_DCR__STAT0__RCP_HP_MU_STATE_get(x)       _BGQ_GET(3,38,x)
#define ND_X2_DCR__STAT0__RCP_HP_MU_STATE_set(v)       _BGQ_SET(3,38,v)
#define ND_X2_DCR__STAT0__RCP_HP_MU_STATE_insert(x,v)  _BGQ_INSERT(3,38,x,v)

//   Field:       rcp_io_mu_state
//   Description: reception fifo IO MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_IO_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_IO_MU_STATE_position     41
#define ND_X2_DCR__STAT0__RCP_IO_MU_STATE_get(x)       _BGQ_GET(3,41,x)
#define ND_X2_DCR__STAT0__RCP_IO_MU_STATE_set(v)       _BGQ_SET(3,41,v)
#define ND_X2_DCR__STAT0__RCP_IO_MU_STATE_insert(x,v)  _BGQ_INSERT(3,41,x,v)

//   Field:       rcp_c0_mu_state
//   Description: reception fifo C0 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_C0_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_C0_MU_STATE_position     44
#define ND_X2_DCR__STAT0__RCP_C0_MU_STATE_get(x)       _BGQ_GET(3,44,x)
#define ND_X2_DCR__STAT0__RCP_C0_MU_STATE_set(v)       _BGQ_SET(3,44,v)
#define ND_X2_DCR__STAT0__RCP_C0_MU_STATE_insert(x,v)  _BGQ_INSERT(3,44,x,v)

//   Field:       rcp_c1_mu_state
//   Description: reception fifo C1 MU interface fsm state.  IDLE state is b000
#define ND_X2_DCR__STAT0__RCP_C1_MU_STATE_width        3
#define ND_X2_DCR__STAT0__RCP_C1_MU_STATE_position     47
#define ND_X2_DCR__STAT0__RCP_C1_MU_STATE_get(x)       _BGQ_GET(3,47,x)
#define ND_X2_DCR__STAT0__RCP_C1_MU_STATE_set(v)       _BGQ_SET(3,47,v)
#define ND_X2_DCR__STAT0__RCP_C1_MU_STATE_insert(x,v)  _BGQ_INSERT(3,47,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_X2_DCR__STAT0__RESERVED_width        16
#define ND_X2_DCR__STAT0__RESERVED_position     63
#define ND_X2_DCR__STAT0__RESERVED_get(x)       _BGQ_GET(16,63,x)
#define ND_X2_DCR__STAT0__RESERVED_set(v)       _BGQ_SET(16,63,v)
#define ND_X2_DCR__STAT0__RESERVED_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 1, reserved
#define ND_X2_DCR__STAT1_offset  (0x000C) // Offset of register in instance 
#define ND_X2_DCR__STAT1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: stat 1
#define ND_X2_DCR__STAT1__VALUE_width        64
#define ND_X2_DCR__STAT1__VALUE_position     63
#define ND_X2_DCR__STAT1__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_X2_DCR__STAT1__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_X2_DCR__STAT1__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0037) // Offset of register in instance 
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0038) // Offset of register in instance 
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0039) // Offset of register in instance 
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: x2_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_X2_DCR__X2_INTERRUPTS__STATE_offset         (0x0020) // Offset of State register in instance
#define ND_X2_DCR__X2_INTERRUPTS__FIRST_offset         (0x0024) // Offset of First register in instance
#define ND_X2_DCR__X2_INTERRUPTS__FORCE_offset         (0x0023) // Offset of Force register in instance
#define ND_X2_DCR__X2_INTERRUPTS__MACHINE_CHECK_offset (0x0025) // Offset of Machine Check register in instance
#define ND_X2_DCR__X2_INTERRUPTS__CRITICAL_offset      (0x0026) // Offset of Critical register in instance
#define ND_X2_DCR__X2_INTERRUPTS__NONCRITICAL_offset   (0x0027) // Offset of Noncritical register in instance

#define ND_X2_DCR__X2_INTERRUPTS__LOCAL_RING_width        1
#define ND_X2_DCR__X2_INTERRUPTS__LOCAL_RING_position     63
#define ND_X2_DCR__X2_INTERRUPTS__LOCAL_RING_get(x)       _BGQ_GET(1,63,x)
#define ND_X2_DCR__X2_INTERRUPTS__LOCAL_RING_set(v)       _BGQ_SET(1,63,v)
#define ND_X2_DCR__X2_INTERRUPTS__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0030) // Offset of State register in instance
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0033) // Offset of First register in instance
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0032) // Offset of Force register in instance
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0034) // Offset of Machine Check register in instance
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0035) // Offset of Critical register in instance
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0036) // Offset of Noncritical register in instance

#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: x2_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW_offset         (0x0021) // Offset of Control register in instance

#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW__LOCAL_RING_width        2
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW__LOCAL_RING_position     63
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW__LOCAL_RING_get(x)       _BGQ_GET(2,63,x)
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW__LOCAL_RING_set(v)       _BGQ_SET(2,63,v)
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_LOW__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: x2_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_X2_DCR__X2_INTERRUPTS_CONTROL_HIGH_offset         (0x0022) // Offset of Control register in instance
  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0031) // Offset of Control register in instance

#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define ND_X2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct nd_x2_dcr { 
    /* 0x0000 */ uint64_t reset;
    /* 0x0001 */ uint64_t ctrl;
    /* 0x0002 */ uint64_t fatal_err0_enable;
    /* 0x0003 */ uint64_t fatal_err0;
    /* 0x0004 */ uint64_t fatal_err1_enable;
    /* 0x0005 */ uint64_t fatal_err1;
    /* 0x0006 */ uint64_t backdoor_ctrl;
    /* 0x0007 */ uint64_t backdoor_wr_data[ND_X2_DCR__BACKDOOR_WR_DATA_range];
    /* 0x0009 */ uint64_t backdoor_wr_ecc;
    /* 0x000A */ uint64_t backdoor_rd;
    /* 0x000B */ uint64_t stat0;
    /* 0x000C */ uint64_t stat1;
    /* 0x000D */ uint64_t reserve_000D[0x0013];
    /* 0x0020 */ uint64_t x2_interrupts__state;
    /* 0x0021 */ uint64_t x2_interrupts_control_low;
    /* 0x0022 */ uint64_t x2_interrupts_control_high;
    /* 0x0023 */ uint64_t x2_interrupts__force;
    /* 0x0024 */ uint64_t x2_interrupts__first;
    /* 0x0025 */ uint64_t x2_interrupts__machine_check;
    /* 0x0026 */ uint64_t x2_interrupts__critical;
    /* 0x0027 */ uint64_t x2_interrupts__noncritical;
    /* 0x0028 */ uint64_t reserve_0028[0x0008];
    /* 0x0030 */ uint64_t interrupt_internal_error__state;
    /* 0x0031 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0032 */ uint64_t interrupt_internal_error__force;
    /* 0x0033 */ uint64_t interrupt_internal_error__first;
    /* 0x0034 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0035 */ uint64_t interrupt_internal_error__critical;
    /* 0x0036 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0037 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0038 */ uint64_t interrupt_internal_error_hw_info;
} nd_x2_dcr_t;
  
#define ND_X2_DCR_USER_PTR _DCR_USER_PTR(nd_x2_dcr_t, ND_X2_DCR)
#define ND_X2_DCR_PRIV_PTR _DCR_PRIV_PTR(nd_x2_dcr_t, ND_X2_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
