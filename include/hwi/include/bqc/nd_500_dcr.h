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

#ifndef   _ND_500_DCR__H_ // Prevent multiple inclusion
#define   _ND_500_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define ND_500_DCR_base    (0x194000)  // Base address for all instances
#define ND_500_DCR_size    (0x0099)    // Size of instance (largest offset + 1 )
  
#define ND_500_DCR(reg) _DCR_REG(ND_500_DCR, reg)
  
// Register:
//   Name:        reset
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFFFFFFFFC000000
//   Description: Inj/Rcp fifo 500 resets, a bit value of 1 puts a unit into reset
#define ND_500_DCR__RESET_offset  (0x0000) // Offset of register in instance 
#define ND_500_DCR__RESET_reset  UNSIGNED64(0xFFFFFFFFFC000000)

//   Field:       dcrs_out
//   Description: Reset all DCR registers except this one
#define ND_500_DCR__RESET__DCRS_OUT_width        1
#define ND_500_DCR__RESET__DCRS_OUT_position     0
#define ND_500_DCR__RESET__DCRS_OUT_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__RESET__DCRS_OUT_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__RESET__DCRS_OUT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t0
//   Description: Reset Injection FIFO torus 0
#define ND_500_DCR__RESET__INJ_T0_width        1
#define ND_500_DCR__RESET__INJ_T0_position     1
#define ND_500_DCR__RESET__INJ_T0_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__RESET__INJ_T0_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__RESET__INJ_T0_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t1
//   Description: Reset Injection FIFO torus 1
#define ND_500_DCR__RESET__INJ_T1_width        1
#define ND_500_DCR__RESET__INJ_T1_position     2
#define ND_500_DCR__RESET__INJ_T1_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__RESET__INJ_T1_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__RESET__INJ_T1_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t2
//   Description: Reset Injection FIFO torus 2
#define ND_500_DCR__RESET__INJ_T2_width        1
#define ND_500_DCR__RESET__INJ_T2_position     3
#define ND_500_DCR__RESET__INJ_T2_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__RESET__INJ_T2_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__RESET__INJ_T2_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t3
//   Description: Reset Injection FIFO torus 3
#define ND_500_DCR__RESET__INJ_T3_width        1
#define ND_500_DCR__RESET__INJ_T3_position     4
#define ND_500_DCR__RESET__INJ_T3_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__RESET__INJ_T3_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__RESET__INJ_T3_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t4
//   Description: Reset Injection FIFO torus 4
#define ND_500_DCR__RESET__INJ_T4_width        1
#define ND_500_DCR__RESET__INJ_T4_position     5
#define ND_500_DCR__RESET__INJ_T4_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__RESET__INJ_T4_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__RESET__INJ_T4_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t5
//   Description: Reset Injection FIFO torus 5
#define ND_500_DCR__RESET__INJ_T5_width        1
#define ND_500_DCR__RESET__INJ_T5_position     6
#define ND_500_DCR__RESET__INJ_T5_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__RESET__INJ_T5_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__RESET__INJ_T5_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t6
//   Description: Reset Injection FIFO torus 6
#define ND_500_DCR__RESET__INJ_T6_width        1
#define ND_500_DCR__RESET__INJ_T6_position     7
#define ND_500_DCR__RESET__INJ_T6_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__RESET__INJ_T6_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__RESET__INJ_T6_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t7
//   Description: Reset Injection FIFO torus 7
#define ND_500_DCR__RESET__INJ_T7_width        1
#define ND_500_DCR__RESET__INJ_T7_position     8
#define ND_500_DCR__RESET__INJ_T7_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__RESET__INJ_T7_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__RESET__INJ_T7_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t8
//   Description: Reset Injection FIFO torus 8
#define ND_500_DCR__RESET__INJ_T8_width        1
#define ND_500_DCR__RESET__INJ_T8_position     9
#define ND_500_DCR__RESET__INJ_T8_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__RESET__INJ_T8_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__RESET__INJ_T8_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_t9
//   Description: Reset Injection FIFO torus 9
#define ND_500_DCR__RESET__INJ_T9_width        1
#define ND_500_DCR__RESET__INJ_T9_position     10
#define ND_500_DCR__RESET__INJ_T9_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__RESET__INJ_T9_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__RESET__INJ_T9_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_l0
//   Description: Reset Injection FIFO local 0
#define ND_500_DCR__RESET__INJ_L0_width        1
#define ND_500_DCR__RESET__INJ_L0_position     11
#define ND_500_DCR__RESET__INJ_L0_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__RESET__INJ_L0_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__RESET__INJ_L0_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_l1
//   Description: Reset Injection FIFO local 1
#define ND_500_DCR__RESET__INJ_L1_width        1
#define ND_500_DCR__RESET__INJ_L1_position     12
#define ND_500_DCR__RESET__INJ_L1_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__RESET__INJ_L1_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__RESET__INJ_L1_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_hp
//   Description: Reset Injection FIFO torus high priority
#define ND_500_DCR__RESET__INJ_HP_width        1
#define ND_500_DCR__RESET__INJ_HP_position     13
#define ND_500_DCR__RESET__INJ_HP_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__RESET__INJ_HP_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__RESET__INJ_HP_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_io
//   Description: Reset Injection FIFO torus system for I/O
#define ND_500_DCR__RESET__INJ_IO_width        1
#define ND_500_DCR__RESET__INJ_IO_position     14
#define ND_500_DCR__RESET__INJ_IO_get(x)       _BGQ_GET(1,14,x)
#define ND_500_DCR__RESET__INJ_IO_set(v)       _BGQ_SET(1,14,v)
#define ND_500_DCR__RESET__INJ_IO_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_c0
//   Description: Reset Injection FIFO collective 0
#define ND_500_DCR__RESET__INJ_C0_width        1
#define ND_500_DCR__RESET__INJ_C0_position     15
#define ND_500_DCR__RESET__INJ_C0_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__RESET__INJ_C0_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__RESET__INJ_C0_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       inj_c1
//   Description: Reset Injection FIFO collective 1
#define ND_500_DCR__RESET__INJ_C1_width        1
#define ND_500_DCR__RESET__INJ_C1_position     16
#define ND_500_DCR__RESET__INJ_C1_get(x)       _BGQ_GET(1,16,x)
#define ND_500_DCR__RESET__INJ_C1_set(v)       _BGQ_SET(1,16,v)
#define ND_500_DCR__RESET__INJ_C1_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t0
//   Description: Reset reception fifo torus 0
#define ND_500_DCR__RESET__RCP_T0_width        1
#define ND_500_DCR__RESET__RCP_T0_position     17
#define ND_500_DCR__RESET__RCP_T0_get(x)       _BGQ_GET(1,17,x)
#define ND_500_DCR__RESET__RCP_T0_set(v)       _BGQ_SET(1,17,v)
#define ND_500_DCR__RESET__RCP_T0_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t1
//   Description: Reset reception fifo torus 1
#define ND_500_DCR__RESET__RCP_T1_width        1
#define ND_500_DCR__RESET__RCP_T1_position     18
#define ND_500_DCR__RESET__RCP_T1_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__RESET__RCP_T1_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__RESET__RCP_T1_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t2
//   Description: Reset reception fifo torus 2
#define ND_500_DCR__RESET__RCP_T2_width        1
#define ND_500_DCR__RESET__RCP_T2_position     19
#define ND_500_DCR__RESET__RCP_T2_get(x)       _BGQ_GET(1,19,x)
#define ND_500_DCR__RESET__RCP_T2_set(v)       _BGQ_SET(1,19,v)
#define ND_500_DCR__RESET__RCP_T2_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_t3
//   Description: Reset reception fifo torus 3
#define ND_500_DCR__RESET__RCP_T3_width        1
#define ND_500_DCR__RESET__RCP_T3_position     20
#define ND_500_DCR__RESET__RCP_T3_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__RESET__RCP_T3_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__RESET__RCP_T3_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_t4
//   Description: Reset reception fifo torus 4
#define ND_500_DCR__RESET__RCP_T4_width        1
#define ND_500_DCR__RESET__RCP_T4_position     21
#define ND_500_DCR__RESET__RCP_T4_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__RESET__RCP_T4_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__RESET__RCP_T4_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_t5
//   Description: Reset reception fifo torus 5
#define ND_500_DCR__RESET__RCP_T5_width        1
#define ND_500_DCR__RESET__RCP_T5_position     22
#define ND_500_DCR__RESET__RCP_T5_get(x)       _BGQ_GET(1,22,x)
#define ND_500_DCR__RESET__RCP_T5_set(v)       _BGQ_SET(1,22,v)
#define ND_500_DCR__RESET__RCP_T5_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_t6
//   Description: Reset reception fifo torus 6
#define ND_500_DCR__RESET__RCP_T6_width        1
#define ND_500_DCR__RESET__RCP_T6_position     23
#define ND_500_DCR__RESET__RCP_T6_get(x)       _BGQ_GET(1,23,x)
#define ND_500_DCR__RESET__RCP_T6_set(v)       _BGQ_SET(1,23,v)
#define ND_500_DCR__RESET__RCP_T6_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_t7
//   Description: Reset reception fifo torus 7
#define ND_500_DCR__RESET__RCP_T7_width        1
#define ND_500_DCR__RESET__RCP_T7_position     24
#define ND_500_DCR__RESET__RCP_T7_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__RESET__RCP_T7_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__RESET__RCP_T7_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_t8
//   Description: Reset reception fifo torus 8
#define ND_500_DCR__RESET__RCP_T8_width        1
#define ND_500_DCR__RESET__RCP_T8_position     25
#define ND_500_DCR__RESET__RCP_T8_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__RESET__RCP_T8_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__RESET__RCP_T8_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_t9
//   Description: Reset reception fifo torus 9
#define ND_500_DCR__RESET__RCP_T9_width        1
#define ND_500_DCR__RESET__RCP_T9_position     26
#define ND_500_DCR__RESET__RCP_T9_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__RESET__RCP_T9_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__RESET__RCP_T9_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_l0
//   Description: Reset reception fifo local 0
#define ND_500_DCR__RESET__RCP_L0_width        1
#define ND_500_DCR__RESET__RCP_L0_position     27
#define ND_500_DCR__RESET__RCP_L0_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__RESET__RCP_L0_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__RESET__RCP_L0_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_l1
//   Description: Reset reception fifo local 1
#define ND_500_DCR__RESET__RCP_L1_width        1
#define ND_500_DCR__RESET__RCP_L1_position     28
#define ND_500_DCR__RESET__RCP_L1_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__RESET__RCP_L1_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__RESET__RCP_L1_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_hp
//   Description: Reset reception fifo torus high priority
#define ND_500_DCR__RESET__RCP_HP_width        1
#define ND_500_DCR__RESET__RCP_HP_position     29
#define ND_500_DCR__RESET__RCP_HP_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__RESET__RCP_HP_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__RESET__RCP_HP_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_io
//   Description: Reset reception fifo torus system I/O
#define ND_500_DCR__RESET__RCP_IO_width        1
#define ND_500_DCR__RESET__RCP_IO_position     30
#define ND_500_DCR__RESET__RCP_IO_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__RESET__RCP_IO_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__RESET__RCP_IO_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c0
//   Description: Reset reception fifo collective 0
#define ND_500_DCR__RESET__RCP_C0_width        1
#define ND_500_DCR__RESET__RCP_C0_position     31
#define ND_500_DCR__RESET__RCP_C0_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__RESET__RCP_C0_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__RESET__RCP_C0_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       rcp_c1
//   Description: Reset reception fifo collective 1
#define ND_500_DCR__RESET__RCP_C1_width        1
#define ND_500_DCR__RESET__RCP_C1_position     32
#define ND_500_DCR__RESET__RCP_C1_get(x)       _BGQ_GET(1,32,x)
#define ND_500_DCR__RESET__RCP_C1_set(v)       _BGQ_SET(1,32,v)
#define ND_500_DCR__RESET__RCP_C1_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       gi
//   Description: Reset global barrier
#define ND_500_DCR__RESET__GI_width        1
#define ND_500_DCR__RESET__GI_position     33
#define ND_500_DCR__RESET__GI_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__RESET__GI_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__RESET__GI_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       coll
//   Description: Reset collective logic
#define ND_500_DCR__RESET__COLL_width        1
#define ND_500_DCR__RESET__COLL_position     34
#define ND_500_DCR__RESET__COLL_get(x)       _BGQ_GET(1,34,x)
#define ND_500_DCR__RESET__COLL_set(v)       _BGQ_SET(1,34,v)
#define ND_500_DCR__RESET__COLL_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       inj_ran
//   Description: Reset injection random unit
#define ND_500_DCR__RESET__INJ_RAN_width        1
#define ND_500_DCR__RESET__INJ_RAN_position     35
#define ND_500_DCR__RESET__INJ_RAN_get(x)       _BGQ_GET(1,35,x)
#define ND_500_DCR__RESET__INJ_RAN_set(v)       _BGQ_SET(1,35,v)
#define ND_500_DCR__RESET__INJ_RAN_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       rcp_ran
//   Description: Reset reception random unit
#define ND_500_DCR__RESET__RCP_RAN_width        1
#define ND_500_DCR__RESET__RCP_RAN_position     36
#define ND_500_DCR__RESET__RCP_RAN_get(x)       _BGQ_GET(1,36,x)
#define ND_500_DCR__RESET__RCP_RAN_set(v)       _BGQ_SET(1,36,v)
#define ND_500_DCR__RESET__RCP_RAN_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       reg_cnt
//   Description: Reset correctable error count
#define ND_500_DCR__RESET__REG_CNT_width        1
#define ND_500_DCR__RESET__REG_CNT_position     37
#define ND_500_DCR__RESET__REG_CNT_get(x)       _BGQ_GET(1,37,x)
#define ND_500_DCR__RESET__REG_CNT_set(v)       _BGQ_SET(1,37,v)
#define ND_500_DCR__RESET__REG_CNT_insert(x,v)  _BGQ_INSERT(1,37,x,v)

  
// Register:
//   Name:        ctrl_stop
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: 500 dcr control: unit stop and drop packets, a bit value of 1 enables unit stop or drop packets
#define ND_500_DCR__CTRL_STOP_offset  (0x0001) // Offset of register in instance 
#define ND_500_DCR__CTRL_STOP_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_unit_stop
//   Description: injection torus fifo 0 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T0_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T0_UNIT_STOP_position     0
#define ND_500_DCR__CTRL_STOP__INJ_T0_UNIT_STOP_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__CTRL_STOP__INJ_T0_UNIT_STOP_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__CTRL_STOP__INJ_T0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t1_unit_stop
//   Description: injection torus fifo 1 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T1_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T1_UNIT_STOP_position     1
#define ND_500_DCR__CTRL_STOP__INJ_T1_UNIT_STOP_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__CTRL_STOP__INJ_T1_UNIT_STOP_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__CTRL_STOP__INJ_T1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t2_unit_stop
//   Description: injection torus fifo 2 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T2_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T2_UNIT_STOP_position     2
#define ND_500_DCR__CTRL_STOP__INJ_T2_UNIT_STOP_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__CTRL_STOP__INJ_T2_UNIT_STOP_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__CTRL_STOP__INJ_T2_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t3_unit_stop
//   Description: injection torus fifo 3 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T3_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T3_UNIT_STOP_position     3
#define ND_500_DCR__CTRL_STOP__INJ_T3_UNIT_STOP_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__CTRL_STOP__INJ_T3_UNIT_STOP_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__CTRL_STOP__INJ_T3_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t4_unit_stop
//   Description: injection torus fifo 4 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T4_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T4_UNIT_STOP_position     4
#define ND_500_DCR__CTRL_STOP__INJ_T4_UNIT_STOP_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__CTRL_STOP__INJ_T4_UNIT_STOP_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__CTRL_STOP__INJ_T4_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t5_unit_stop
//   Description: injection torus fifo 5 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T5_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T5_UNIT_STOP_position     5
#define ND_500_DCR__CTRL_STOP__INJ_T5_UNIT_STOP_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__CTRL_STOP__INJ_T5_UNIT_STOP_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__CTRL_STOP__INJ_T5_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t6_unit_stop
//   Description: injection torus fifo 6 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T6_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T6_UNIT_STOP_position     6
#define ND_500_DCR__CTRL_STOP__INJ_T6_UNIT_STOP_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__CTRL_STOP__INJ_T6_UNIT_STOP_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__CTRL_STOP__INJ_T6_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t7_unit_stop
//   Description: injection torus fifo 7 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T7_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T7_UNIT_STOP_position     7
#define ND_500_DCR__CTRL_STOP__INJ_T7_UNIT_STOP_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__CTRL_STOP__INJ_T7_UNIT_STOP_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__CTRL_STOP__INJ_T7_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t8_unit_stop
//   Description: injection torus fifo 8 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T8_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T8_UNIT_STOP_position     8
#define ND_500_DCR__CTRL_STOP__INJ_T8_UNIT_STOP_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__CTRL_STOP__INJ_T8_UNIT_STOP_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__CTRL_STOP__INJ_T8_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t9_unit_stop
//   Description: injection torus fifo 9 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_T9_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_T9_UNIT_STOP_position     9
#define ND_500_DCR__CTRL_STOP__INJ_T9_UNIT_STOP_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__CTRL_STOP__INJ_T9_UNIT_STOP_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__CTRL_STOP__INJ_T9_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_l0_unit_stop
//   Description: injection local fifo 0 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_L0_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_L0_UNIT_STOP_position     10
#define ND_500_DCR__CTRL_STOP__INJ_L0_UNIT_STOP_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__CTRL_STOP__INJ_L0_UNIT_STOP_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__CTRL_STOP__INJ_L0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_l1_unit_stop
//   Description: injection local fifo 1 unit stop
#define ND_500_DCR__CTRL_STOP__INJ_L1_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_L1_UNIT_STOP_position     11
#define ND_500_DCR__CTRL_STOP__INJ_L1_UNIT_STOP_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__CTRL_STOP__INJ_L1_UNIT_STOP_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__CTRL_STOP__INJ_L1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_hp_unit_stop
//   Description: injection high priority torus fifo unit stop
#define ND_500_DCR__CTRL_STOP__INJ_HP_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_HP_UNIT_STOP_position     12
#define ND_500_DCR__CTRL_STOP__INJ_HP_UNIT_STOP_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__CTRL_STOP__INJ_HP_UNIT_STOP_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__CTRL_STOP__INJ_HP_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_io_unit_stop
//   Description: injection system (I/O) torus fifo unit stop
#define ND_500_DCR__CTRL_STOP__INJ_IO_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_IO_UNIT_STOP_position     13
#define ND_500_DCR__CTRL_STOP__INJ_IO_UNIT_STOP_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__CTRL_STOP__INJ_IO_UNIT_STOP_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__CTRL_STOP__INJ_IO_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_c0_unit_stop
//   Description: injection collective fifo 0 (user) unit stop
#define ND_500_DCR__CTRL_STOP__INJ_C0_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_C0_UNIT_STOP_position     14
#define ND_500_DCR__CTRL_STOP__INJ_C0_UNIT_STOP_get(x)       _BGQ_GET(1,14,x)
#define ND_500_DCR__CTRL_STOP__INJ_C0_UNIT_STOP_set(v)       _BGQ_SET(1,14,v)
#define ND_500_DCR__CTRL_STOP__INJ_C0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_c1_unit_stop
//   Description: injection collective fifo 1 (user) unit stop
#define ND_500_DCR__CTRL_STOP__INJ_C1_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__INJ_C1_UNIT_STOP_position     15
#define ND_500_DCR__CTRL_STOP__INJ_C1_UNIT_STOP_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__CTRL_STOP__INJ_C1_UNIT_STOP_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__CTRL_STOP__INJ_C1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rcp_t0_unit_stop
//   Description: reception torus fifo 0 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T0_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T0_UNIT_STOP_position     16
#define ND_500_DCR__CTRL_STOP__RCP_T0_UNIT_STOP_get(x)       _BGQ_GET(1,16,x)
#define ND_500_DCR__CTRL_STOP__RCP_T0_UNIT_STOP_set(v)       _BGQ_SET(1,16,v)
#define ND_500_DCR__CTRL_STOP__RCP_T0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t1_unit_stop
//   Description: reception torus fifo 1 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T1_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T1_UNIT_STOP_position     17
#define ND_500_DCR__CTRL_STOP__RCP_T1_UNIT_STOP_get(x)       _BGQ_GET(1,17,x)
#define ND_500_DCR__CTRL_STOP__RCP_T1_UNIT_STOP_set(v)       _BGQ_SET(1,17,v)
#define ND_500_DCR__CTRL_STOP__RCP_T1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t2_unit_stop
//   Description: reception torus fifo 2 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T2_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T2_UNIT_STOP_position     18
#define ND_500_DCR__CTRL_STOP__RCP_T2_UNIT_STOP_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__CTRL_STOP__RCP_T2_UNIT_STOP_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__CTRL_STOP__RCP_T2_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t3_unit_stop
//   Description: reception torus fifo 3 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T3_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T3_UNIT_STOP_position     19
#define ND_500_DCR__CTRL_STOP__RCP_T3_UNIT_STOP_get(x)       _BGQ_GET(1,19,x)
#define ND_500_DCR__CTRL_STOP__RCP_T3_UNIT_STOP_set(v)       _BGQ_SET(1,19,v)
#define ND_500_DCR__CTRL_STOP__RCP_T3_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_t4_unit_stop
//   Description: reception torus fifo 4 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T4_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T4_UNIT_STOP_position     20
#define ND_500_DCR__CTRL_STOP__RCP_T4_UNIT_STOP_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__CTRL_STOP__RCP_T4_UNIT_STOP_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__CTRL_STOP__RCP_T4_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_t5_unit_stop
//   Description: reception torus fifo 5 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T5_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T5_UNIT_STOP_position     21
#define ND_500_DCR__CTRL_STOP__RCP_T5_UNIT_STOP_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__CTRL_STOP__RCP_T5_UNIT_STOP_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__CTRL_STOP__RCP_T5_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_t6_unit_stop
//   Description: reception torus fifo 6 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T6_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T6_UNIT_STOP_position     22
#define ND_500_DCR__CTRL_STOP__RCP_T6_UNIT_STOP_get(x)       _BGQ_GET(1,22,x)
#define ND_500_DCR__CTRL_STOP__RCP_T6_UNIT_STOP_set(v)       _BGQ_SET(1,22,v)
#define ND_500_DCR__CTRL_STOP__RCP_T6_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_t7_unit_stop
//   Description: reception torus fifo 7 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T7_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T7_UNIT_STOP_position     23
#define ND_500_DCR__CTRL_STOP__RCP_T7_UNIT_STOP_get(x)       _BGQ_GET(1,23,x)
#define ND_500_DCR__CTRL_STOP__RCP_T7_UNIT_STOP_set(v)       _BGQ_SET(1,23,v)
#define ND_500_DCR__CTRL_STOP__RCP_T7_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_t8_unit_stop
//   Description: reception torus fifo 8 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T8_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T8_UNIT_STOP_position     24
#define ND_500_DCR__CTRL_STOP__RCP_T8_UNIT_STOP_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__CTRL_STOP__RCP_T8_UNIT_STOP_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__CTRL_STOP__RCP_T8_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_t9_unit_stop
//   Description: reception torus fifo 9 unit stop
#define ND_500_DCR__CTRL_STOP__RCP_T9_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T9_UNIT_STOP_position     25
#define ND_500_DCR__CTRL_STOP__RCP_T9_UNIT_STOP_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__CTRL_STOP__RCP_T9_UNIT_STOP_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__CTRL_STOP__RCP_T9_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_hp_unit_stop
//   Description: reception high priority torus fifo unit stop
#define ND_500_DCR__CTRL_STOP__RCP_HP_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_HP_UNIT_STOP_position     26
#define ND_500_DCR__CTRL_STOP__RCP_HP_UNIT_STOP_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__CTRL_STOP__RCP_HP_UNIT_STOP_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__CTRL_STOP__RCP_HP_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_io_unit_stop
//   Description: reception system (I/O) torus fifo unit stop
#define ND_500_DCR__CTRL_STOP__RCP_IO_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_IO_UNIT_STOP_position     27
#define ND_500_DCR__CTRL_STOP__RCP_IO_UNIT_STOP_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__CTRL_STOP__RCP_IO_UNIT_STOP_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__CTRL_STOP__RCP_IO_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_c0_unit_stop
//   Description: reception collective fifo 0 (user) unit stop
#define ND_500_DCR__CTRL_STOP__RCP_C0_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_C0_UNIT_STOP_position     28
#define ND_500_DCR__CTRL_STOP__RCP_C0_UNIT_STOP_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__CTRL_STOP__RCP_C0_UNIT_STOP_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__CTRL_STOP__RCP_C0_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_c1_unit_stop
//   Description: reception collective fifo 1 (system) unit stop
#define ND_500_DCR__CTRL_STOP__RCP_C1_UNIT_STOP_width        1
#define ND_500_DCR__CTRL_STOP__RCP_C1_UNIT_STOP_position     29
#define ND_500_DCR__CTRL_STOP__RCP_C1_UNIT_STOP_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__CTRL_STOP__RCP_C1_UNIT_STOP_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__CTRL_STOP__RCP_C1_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_t0_drop_pkt
//   Description: reception torus fifo 0 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T0_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T0_DROP_PKT_position     30
#define ND_500_DCR__CTRL_STOP__RCP_T0_DROP_PKT_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__CTRL_STOP__RCP_T0_DROP_PKT_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__CTRL_STOP__RCP_T0_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_t1_drop_pkt
//   Description: reception torus fifo 1 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T1_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T1_DROP_PKT_position     31
#define ND_500_DCR__CTRL_STOP__RCP_T1_DROP_PKT_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__CTRL_STOP__RCP_T1_DROP_PKT_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__CTRL_STOP__RCP_T1_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       rcp_t2_drop_pkt
//   Description: reception torus fifo 2 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T2_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T2_DROP_PKT_position     32
#define ND_500_DCR__CTRL_STOP__RCP_T2_DROP_PKT_get(x)       _BGQ_GET(1,32,x)
#define ND_500_DCR__CTRL_STOP__RCP_T2_DROP_PKT_set(v)       _BGQ_SET(1,32,v)
#define ND_500_DCR__CTRL_STOP__RCP_T2_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       rcp_t3_drop_pkt
//   Description: reception torus fifo 3 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T3_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T3_DROP_PKT_position     33
#define ND_500_DCR__CTRL_STOP__RCP_T3_DROP_PKT_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__CTRL_STOP__RCP_T3_DROP_PKT_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__CTRL_STOP__RCP_T3_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       rcp_t4_drop_pkt
//   Description: reception torus fifo 4 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T4_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T4_DROP_PKT_position     34
#define ND_500_DCR__CTRL_STOP__RCP_T4_DROP_PKT_get(x)       _BGQ_GET(1,34,x)
#define ND_500_DCR__CTRL_STOP__RCP_T4_DROP_PKT_set(v)       _BGQ_SET(1,34,v)
#define ND_500_DCR__CTRL_STOP__RCP_T4_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       rcp_t5_drop_pkt
//   Description: reception torus fifo 5 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T5_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T5_DROP_PKT_position     35
#define ND_500_DCR__CTRL_STOP__RCP_T5_DROP_PKT_get(x)       _BGQ_GET(1,35,x)
#define ND_500_DCR__CTRL_STOP__RCP_T5_DROP_PKT_set(v)       _BGQ_SET(1,35,v)
#define ND_500_DCR__CTRL_STOP__RCP_T5_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       rcp_t6_drop_pkt
//   Description: reception torus fifo 6 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T6_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T6_DROP_PKT_position     36
#define ND_500_DCR__CTRL_STOP__RCP_T6_DROP_PKT_get(x)       _BGQ_GET(1,36,x)
#define ND_500_DCR__CTRL_STOP__RCP_T6_DROP_PKT_set(v)       _BGQ_SET(1,36,v)
#define ND_500_DCR__CTRL_STOP__RCP_T6_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       rcp_t7_drop_pkt
//   Description: reception torus fifo 7 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T7_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T7_DROP_PKT_position     37
#define ND_500_DCR__CTRL_STOP__RCP_T7_DROP_PKT_get(x)       _BGQ_GET(1,37,x)
#define ND_500_DCR__CTRL_STOP__RCP_T7_DROP_PKT_set(v)       _BGQ_SET(1,37,v)
#define ND_500_DCR__CTRL_STOP__RCP_T7_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       rcp_t8_drop_pkt
//   Description: reception torus fifo 8 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T8_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T8_DROP_PKT_position     38
#define ND_500_DCR__CTRL_STOP__RCP_T8_DROP_PKT_get(x)       _BGQ_GET(1,38,x)
#define ND_500_DCR__CTRL_STOP__RCP_T8_DROP_PKT_set(v)       _BGQ_SET(1,38,v)
#define ND_500_DCR__CTRL_STOP__RCP_T8_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       rcp_t9_drop_pkt
//   Description: reception torus fifo 9 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_T9_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_T9_DROP_PKT_position     39
#define ND_500_DCR__CTRL_STOP__RCP_T9_DROP_PKT_get(x)       _BGQ_GET(1,39,x)
#define ND_500_DCR__CTRL_STOP__RCP_T9_DROP_PKT_set(v)       _BGQ_SET(1,39,v)
#define ND_500_DCR__CTRL_STOP__RCP_T9_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       rcp_hp_drop_pkt
//   Description: reception high priority torus fifo drop packets
#define ND_500_DCR__CTRL_STOP__RCP_HP_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_HP_DROP_PKT_position     40
#define ND_500_DCR__CTRL_STOP__RCP_HP_DROP_PKT_get(x)       _BGQ_GET(1,40,x)
#define ND_500_DCR__CTRL_STOP__RCP_HP_DROP_PKT_set(v)       _BGQ_SET(1,40,v)
#define ND_500_DCR__CTRL_STOP__RCP_HP_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       rcp_io_drop_pkt
//   Description: reception system (I/O) torus fifo drop packets
#define ND_500_DCR__CTRL_STOP__RCP_IO_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_IO_DROP_PKT_position     41
#define ND_500_DCR__CTRL_STOP__RCP_IO_DROP_PKT_get(x)       _BGQ_GET(1,41,x)
#define ND_500_DCR__CTRL_STOP__RCP_IO_DROP_PKT_set(v)       _BGQ_SET(1,41,v)
#define ND_500_DCR__CTRL_STOP__RCP_IO_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,41,x,v)

//   Field:       rcp_l0_drop_pkt
//   Description: reception local fifo 0 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_L0_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_L0_DROP_PKT_position     42
#define ND_500_DCR__CTRL_STOP__RCP_L0_DROP_PKT_get(x)       _BGQ_GET(1,42,x)
#define ND_500_DCR__CTRL_STOP__RCP_L0_DROP_PKT_set(v)       _BGQ_SET(1,42,v)
#define ND_500_DCR__CTRL_STOP__RCP_L0_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       rcp_l1_drop_pkt
//   Description: reception local fifo 1 drop packets
#define ND_500_DCR__CTRL_STOP__RCP_L1_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_L1_DROP_PKT_position     43
#define ND_500_DCR__CTRL_STOP__RCP_L1_DROP_PKT_get(x)       _BGQ_GET(1,43,x)
#define ND_500_DCR__CTRL_STOP__RCP_L1_DROP_PKT_set(v)       _BGQ_SET(1,43,v)
#define ND_500_DCR__CTRL_STOP__RCP_L1_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       rcp_c0_drop_pkt
//   Description: reception collective fifo 0 (user) drop packets
#define ND_500_DCR__CTRL_STOP__RCP_C0_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_C0_DROP_PKT_position     44
#define ND_500_DCR__CTRL_STOP__RCP_C0_DROP_PKT_get(x)       _BGQ_GET(1,44,x)
#define ND_500_DCR__CTRL_STOP__RCP_C0_DROP_PKT_set(v)       _BGQ_SET(1,44,v)
#define ND_500_DCR__CTRL_STOP__RCP_C0_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       rcp_c1_drop_pkt
//   Description: reception collective fifo 1 (system) drop packets
#define ND_500_DCR__CTRL_STOP__RCP_C1_DROP_PKT_width        1
#define ND_500_DCR__CTRL_STOP__RCP_C1_DROP_PKT_position     45
#define ND_500_DCR__CTRL_STOP__RCP_C1_DROP_PKT_get(x)       _BGQ_GET(1,45,x)
#define ND_500_DCR__CTRL_STOP__RCP_C1_DROP_PKT_set(v)       _BGQ_SET(1,45,v)
#define ND_500_DCR__CTRL_STOP__RCP_C1_DROP_PKT_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       reserved
//   Description: control stop reserved
#define ND_500_DCR__CTRL_STOP__RESERVED_width        18
#define ND_500_DCR__CTRL_STOP__RESERVED_position     63
#define ND_500_DCR__CTRL_STOP__RESERVED_get(x)       _BGQ_GET(18,63,x)
#define ND_500_DCR__CTRL_STOP__RESERVED_set(v)       _BGQ_SET(18,63,v)
#define ND_500_DCR__CTRL_STOP__RESERVED_insert(x,v)  _BGQ_INSERT(18,63,x,v)

  
// Register:
//   Name:        ctrl_internal
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: 500 dcr control: internal
#define ND_500_DCR__CTRL_INTERNAL_offset  (0x0002) // Offset of register in instance 
#define ND_500_DCR__CTRL_INTERNAL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       loopback
//   Description: loop back control, each bit controls one dimension, in the order of A, B, C, D, E, IO.  1 means torus internal loopback
#define ND_500_DCR__CTRL_INTERNAL__LOOPBACK_width        6
#define ND_500_DCR__CTRL_INTERNAL__LOOPBACK_position     5
#define ND_500_DCR__CTRL_INTERNAL__LOOPBACK_get(x)       _BGQ_GET(6,5,x)
#define ND_500_DCR__CTRL_INTERNAL__LOOPBACK_set(v)       _BGQ_SET(6,5,v)
#define ND_500_DCR__CTRL_INTERNAL__LOOPBACK_insert(x,v)  _BGQ_INSERT(6,5,x,v)

//   Field:       is_torus
//   Description: is torus.  Each bit indicates if a dimension is a torus, in order of A, B, C, D, E.  This is used for dynamic injection hint bit calculator to break ties when going to a node half the torus dimension size away
#define ND_500_DCR__CTRL_INTERNAL__IS_TORUS_width        5
#define ND_500_DCR__CTRL_INTERNAL__IS_TORUS_position     10
#define ND_500_DCR__CTRL_INTERNAL__IS_TORUS_get(x)       _BGQ_GET(5,10,x)
#define ND_500_DCR__CTRL_INTERNAL__IS_TORUS_set(v)       _BGQ_SET(5,10,v)
#define ND_500_DCR__CTRL_INTERNAL__IS_TORUS_insert(x,v)  _BGQ_INSERT(5,10,x,v)

//   Field:       vcdn_ran_hints
//   Description: vcdn ran hints per dimension.  Does not work, set to 0 always
#define ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_width        5
#define ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_position     15
#define ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_get(x)       _BGQ_GET(5,15,x)
#define ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_set(v)       _BGQ_SET(5,15,v)
#define ND_500_DCR__CTRL_INTERNAL__VCDN_RAN_HINTS_insert(x,v)  _BGQ_INSERT(5,15,x,v)

//   Field:       se_arb_re_inj_ratio
//   Description: sender arbibor re/inj ratio.  When the number of grants to receivers is greater than this value, give injection high priority on the next arbitration and reset receiver grant count to zero.  In other words, the ratio of grants given to receivers and to injection fifos is (n+1):1 if this register's value is n
#define ND_500_DCR__CTRL_INTERNAL__SE_ARB_RE_INJ_RATIO_width        4
#define ND_500_DCR__CTRL_INTERNAL__SE_ARB_RE_INJ_RATIO_position     19
#define ND_500_DCR__CTRL_INTERNAL__SE_ARB_RE_INJ_RATIO_get(x)       _BGQ_GET(4,19,x)
#define ND_500_DCR__CTRL_INTERNAL__SE_ARB_RE_INJ_RATIO_set(v)       _BGQ_SET(4,19,v)
#define ND_500_DCR__CTRL_INTERNAL__SE_ARB_RE_INJ_RATIO_insert(x,v)  _BGQ_INSERT(4,19,x,v)

//   Field:       coll_up_arb_se_avail_sel
//   Description: collective uptree arbiter sender available select: 0 token avail, 1 token avail and link avail.  Must set to 0
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_SE_AVAIL_SEL_width        1
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_SE_AVAIL_SEL_position     20
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_SE_AVAIL_SEL_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_SE_AVAIL_SEL_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_SE_AVAIL_SEL_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       coll_up_arb_re_drop_req_sel
//   Description: collective uptree arbiter receiver drop packet select: 0 old behavior, drop request when packet is flowing, 1 new behavior, drop request only when necessary.  Should set to 1
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_RE_DROP_REQ_SEL_width        1
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_RE_DROP_REQ_SEL_position     21
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_RE_DROP_REQ_SEL_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_RE_DROP_REQ_SEL_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__CTRL_INTERNAL__COLL_UP_ARB_RE_DROP_REQ_SEL_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       reserved
//   Description: control internal reserved
#define ND_500_DCR__CTRL_INTERNAL__RESERVED_width        42
#define ND_500_DCR__CTRL_INTERNAL__RESERVED_position     63
#define ND_500_DCR__CTRL_INTERNAL__RESERVED_get(x)       _BGQ_GET(42,63,x)
#define ND_500_DCR__CTRL_INTERNAL__RESERVED_set(v)       _BGQ_SET(42,63,v)
#define ND_500_DCR__CTRL_INTERNAL__RESERVED_insert(x,v)  _BGQ_INSERT(42,63,x,v)

  
// Register:
//   Name:        ctrl_clear0
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: 500 dcr control: clear 0
#define ND_500_DCR__CTRL_CLEAR0_offset  (0x0003) // Offset of register in instance 
#define ND_500_DCR__CTRL_CLEAR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       clr_hdrchk_err
//   Description: clear header checker error, one bit per torus injection fifo (excluding local).  In the order of T0, T1, .. T9, HP, IO.  Not working on DD1
#define ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_width        12
#define ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_position     11
#define ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_get(x)       _BGQ_GET(12,11,x)
#define ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_set(v)       _BGQ_SET(12,11,v)
#define ND_500_DCR__CTRL_CLEAR0__CLR_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(12,11,x,v)

//   Field:       clr_coll_hdrchk_err
//   Description: clear collective header checker error, one bit per collective injection fifo
#define ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_width        2
#define ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_position     13
#define ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_get(x)       _BGQ_GET(2,13,x)
#define ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_set(v)       _BGQ_SET(2,13,v)
#define ND_500_DCR__CTRL_CLEAR0__CLR_COLL_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(2,13,x,v)

//   Field:       unused
//   Description: reserved
#define ND_500_DCR__CTRL_CLEAR0__UNUSED_width        10
#define ND_500_DCR__CTRL_CLEAR0__UNUSED_position     23
#define ND_500_DCR__CTRL_CLEAR0__UNUSED_get(x)       _BGQ_GET(10,23,x)
#define ND_500_DCR__CTRL_CLEAR0__UNUSED_set(v)       _BGQ_SET(10,23,v)
#define ND_500_DCR__CTRL_CLEAR0__UNUSED_insert(x,v)  _BGQ_INSERT(10,23,x,v)

//   Field:       int_rcp_500_clr
//   Description: clear collective reception fifo non fatal errors, within this field, bit 0 and 4 are unused, bit 1 clears C0 integer overflow, bit 2 clears C0 fifo NAN flag, bit 3 clears C0 fifo other errors, like operand mismatch, etc;  Bit 5 clears C1 integer overflow, bit 6 clears C1 fifo NAN flag, bit 7 clears C1 fifo other errors
#define ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_width        8
#define ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_position     31
#define ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_get(x)       _BGQ_GET(8,31,x)
#define ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_set(v)       _BGQ_SET(8,31,v)
#define ND_500_DCR__CTRL_CLEAR0__INT_RCP_500_CLR_insert(x,v)  _BGQ_INSERT(8,31,x,v)

//   Field:       coll_nonfatal_clr
//   Description: clear collective non fatal errors.  Within this field, bit 0 clears grant timeout interrupt, bit 1 clears local NAN generated, bit 2 clears local interger overflow generated, bit 3 clears local header mismatch.  Bits 4 to 31 are unused
#define ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_width        32
#define ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_position     63
#define ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__CTRL_CLEAR0__COLL_NONFATAL_CLR_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ctrl_coords
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: control 1 node coord/machine nodes
#define ND_500_DCR__CTRL_COORDS_offset  (0x0004) // Offset of register in instance 
#define ND_500_DCR__CTRL_COORDS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       node_coord_a
//   Description: node coord A
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_A_width        6
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_A_position     7
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_A_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_A_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_A_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       node_coord_b
//   Description: node coord B
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_B_width        6
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_B_position     13
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_B_get(x)       _BGQ_GET(6,13,x)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_B_set(v)       _BGQ_SET(6,13,v)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_B_insert(x,v)  _BGQ_INSERT(6,13,x,v)

//   Field:       node_coord_c
//   Description: node coord C
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_C_width        6
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_C_position     19
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_C_get(x)       _BGQ_GET(6,19,x)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_C_set(v)       _BGQ_SET(6,19,v)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_C_insert(x,v)  _BGQ_INSERT(6,19,x,v)

//   Field:       node_coord_d
//   Description: node coord D
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_D_width        6
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_D_position     25
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_D_get(x)       _BGQ_GET(6,25,x)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_D_set(v)       _BGQ_SET(6,25,v)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_D_insert(x,v)  _BGQ_INSERT(6,25,x,v)

//   Field:       node_coord_e
//   Description: node coord E
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_E_width        6
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_E_position     31
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_E_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_E_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__CTRL_COORDS__NODE_COORD_E_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       max_coord_a
//   Description: max coord A
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_A_width        6
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_A_position     39
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_A_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_A_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_A_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       max_coord_b
//   Description: max coord B
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_B_width        6
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_B_position     45
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_B_get(x)       _BGQ_GET(6,45,x)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_B_set(v)       _BGQ_SET(6,45,v)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_B_insert(x,v)  _BGQ_INSERT(6,45,x,v)

//   Field:       max_coord_c
//   Description: max coord C
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_C_width        6
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_C_position     51
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_C_get(x)       _BGQ_GET(6,51,x)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_C_set(v)       _BGQ_SET(6,51,v)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_C_insert(x,v)  _BGQ_INSERT(6,51,x,v)

//   Field:       max_coord_d
//   Description: max coord D
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_D_width        6
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_D_position     57
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_D_get(x)       _BGQ_GET(6,57,x)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_D_set(v)       _BGQ_SET(6,57,v)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_D_insert(x,v)  _BGQ_INSERT(6,57,x,v)

//   Field:       max_coord_e
//   Description: max coord E
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_E_width        6
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_E_position     63
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_E_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_E_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__CTRL_COORDS__MAX_COORD_E_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        ctrl_cutoffs
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: control 2 plus/minus cutoff
#define ND_500_DCR__CTRL_CUTOFFS_offset  (0x0005) // Offset of register in instance 
#define ND_500_DCR__CTRL_CUTOFFS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       a_plus
//   Description: A plus
#define ND_500_DCR__CTRL_CUTOFFS__A_PLUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__A_PLUS_position     7
#define ND_500_DCR__CTRL_CUTOFFS__A_PLUS_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__CTRL_CUTOFFS__A_PLUS_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__CTRL_CUTOFFS__A_PLUS_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       b_plus
//   Description: B plus
#define ND_500_DCR__CTRL_CUTOFFS__B_PLUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__B_PLUS_position     13
#define ND_500_DCR__CTRL_CUTOFFS__B_PLUS_get(x)       _BGQ_GET(6,13,x)
#define ND_500_DCR__CTRL_CUTOFFS__B_PLUS_set(v)       _BGQ_SET(6,13,v)
#define ND_500_DCR__CTRL_CUTOFFS__B_PLUS_insert(x,v)  _BGQ_INSERT(6,13,x,v)

//   Field:       c_plus
//   Description: C plus
#define ND_500_DCR__CTRL_CUTOFFS__C_PLUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__C_PLUS_position     19
#define ND_500_DCR__CTRL_CUTOFFS__C_PLUS_get(x)       _BGQ_GET(6,19,x)
#define ND_500_DCR__CTRL_CUTOFFS__C_PLUS_set(v)       _BGQ_SET(6,19,v)
#define ND_500_DCR__CTRL_CUTOFFS__C_PLUS_insert(x,v)  _BGQ_INSERT(6,19,x,v)

//   Field:       d_plus
//   Description: D plus
#define ND_500_DCR__CTRL_CUTOFFS__D_PLUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__D_PLUS_position     25
#define ND_500_DCR__CTRL_CUTOFFS__D_PLUS_get(x)       _BGQ_GET(6,25,x)
#define ND_500_DCR__CTRL_CUTOFFS__D_PLUS_set(v)       _BGQ_SET(6,25,v)
#define ND_500_DCR__CTRL_CUTOFFS__D_PLUS_insert(x,v)  _BGQ_INSERT(6,25,x,v)

//   Field:       e_plus
//   Description: E plus
#define ND_500_DCR__CTRL_CUTOFFS__E_PLUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__E_PLUS_position     31
#define ND_500_DCR__CTRL_CUTOFFS__E_PLUS_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__CTRL_CUTOFFS__E_PLUS_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__CTRL_CUTOFFS__E_PLUS_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       a_minus
//   Description: A minus
#define ND_500_DCR__CTRL_CUTOFFS__A_MINUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__A_MINUS_position     39
#define ND_500_DCR__CTRL_CUTOFFS__A_MINUS_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__CTRL_CUTOFFS__A_MINUS_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__CTRL_CUTOFFS__A_MINUS_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       b_minus
//   Description: B minus
#define ND_500_DCR__CTRL_CUTOFFS__B_MINUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__B_MINUS_position     45
#define ND_500_DCR__CTRL_CUTOFFS__B_MINUS_get(x)       _BGQ_GET(6,45,x)
#define ND_500_DCR__CTRL_CUTOFFS__B_MINUS_set(v)       _BGQ_SET(6,45,v)
#define ND_500_DCR__CTRL_CUTOFFS__B_MINUS_insert(x,v)  _BGQ_INSERT(6,45,x,v)

//   Field:       c_minus
//   Description: C minus
#define ND_500_DCR__CTRL_CUTOFFS__C_MINUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__C_MINUS_position     51
#define ND_500_DCR__CTRL_CUTOFFS__C_MINUS_get(x)       _BGQ_GET(6,51,x)
#define ND_500_DCR__CTRL_CUTOFFS__C_MINUS_set(v)       _BGQ_SET(6,51,v)
#define ND_500_DCR__CTRL_CUTOFFS__C_MINUS_insert(x,v)  _BGQ_INSERT(6,51,x,v)

//   Field:       d_minus
//   Description: D minus
#define ND_500_DCR__CTRL_CUTOFFS__D_MINUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__D_MINUS_position     57
#define ND_500_DCR__CTRL_CUTOFFS__D_MINUS_get(x)       _BGQ_GET(6,57,x)
#define ND_500_DCR__CTRL_CUTOFFS__D_MINUS_set(v)       _BGQ_SET(6,57,v)
#define ND_500_DCR__CTRL_CUTOFFS__D_MINUS_insert(x,v)  _BGQ_INSERT(6,57,x,v)

//   Field:       e_minus
//   Description: E minus
#define ND_500_DCR__CTRL_CUTOFFS__E_MINUS_width        6
#define ND_500_DCR__CTRL_CUTOFFS__E_MINUS_position     63
#define ND_500_DCR__CTRL_CUTOFFS__E_MINUS_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__CTRL_CUTOFFS__E_MINUS_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__CTRL_CUTOFFS__E_MINUS_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        ctrl_neighbor_coords
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: neighbor coordinates
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS_offset  (0x0006) // Offset of register in instance 
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       a_minus
//   Description: A minus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_position     9
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_get(x)       _BGQ_GET(6,9,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_set(v)       _BGQ_SET(6,9,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_insert(x,v)  _BGQ_INSERT(6,9,x,v)

//   Field:       a_plus
//   Description: A plus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_position     15
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_get(x)       _BGQ_GET(6,15,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_set(v)       _BGQ_SET(6,15,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       b_minus
//   Description: B minus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_position     21
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_get(x)       _BGQ_GET(6,21,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_set(v)       _BGQ_SET(6,21,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_insert(x,v)  _BGQ_INSERT(6,21,x,v)

//   Field:       b_plus
//   Description: B plus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_position     27
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_get(x)       _BGQ_GET(6,27,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_set(v)       _BGQ_SET(6,27,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_insert(x,v)  _BGQ_INSERT(6,27,x,v)

//   Field:       c_minus
//   Description: C minus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_position     33
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_get(x)       _BGQ_GET(6,33,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_set(v)       _BGQ_SET(6,33,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_insert(x,v)  _BGQ_INSERT(6,33,x,v)

//   Field:       c_plus
//   Description: C plus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_position     39
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       d_minus
//   Description: D minus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_position     45
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_get(x)       _BGQ_GET(6,45,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_set(v)       _BGQ_SET(6,45,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_insert(x,v)  _BGQ_INSERT(6,45,x,v)

//   Field:       d_plus
//   Description: D plus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_position     51
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_get(x)       _BGQ_GET(6,51,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_set(v)       _BGQ_SET(6,51,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_insert(x,v)  _BGQ_INSERT(6,51,x,v)

//   Field:       e_minus
//   Description: E minus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_position     57
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_get(x)       _BGQ_GET(6,57,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_set(v)       _BGQ_SET(6,57,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_insert(x,v)  _BGQ_INSERT(6,57,x,v)

//   Field:       e_plus
//   Description: E plus
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_width        6
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_position     63
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        ctrl_det_order
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: deterministic order mask
#define ND_500_DCR__CTRL_DET_ORDER_offset  (0x0007) // Offset of register in instance 
#define ND_500_DCR__CTRL_DET_ORDER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       mask0
//   Description: mask 0, for example, 10000 is A 1st.  One bit set for each mask, non-overlapping
#define ND_500_DCR__CTRL_DET_ORDER__MASK0_width        5
#define ND_500_DCR__CTRL_DET_ORDER__MASK0_position     43
#define ND_500_DCR__CTRL_DET_ORDER__MASK0_get(x)       _BGQ_GET(5,43,x)
#define ND_500_DCR__CTRL_DET_ORDER__MASK0_set(v)       _BGQ_SET(5,43,v)
#define ND_500_DCR__CTRL_DET_ORDER__MASK0_insert(x,v)  _BGQ_INSERT(5,43,x,v)

//   Field:       mask1
//   Description: mask 1, for example, 01000 is B 2nd.  One bit set for each mask, non-overlapping
#define ND_500_DCR__CTRL_DET_ORDER__MASK1_width        5
#define ND_500_DCR__CTRL_DET_ORDER__MASK1_position     48
#define ND_500_DCR__CTRL_DET_ORDER__MASK1_get(x)       _BGQ_GET(5,48,x)
#define ND_500_DCR__CTRL_DET_ORDER__MASK1_set(v)       _BGQ_SET(5,48,v)
#define ND_500_DCR__CTRL_DET_ORDER__MASK1_insert(x,v)  _BGQ_INSERT(5,48,x,v)

//   Field:       mask2
//   Description: mask 2, for example, 00100 is C 3rd.  One bit set for each mask, non-overlapping
#define ND_500_DCR__CTRL_DET_ORDER__MASK2_width        5
#define ND_500_DCR__CTRL_DET_ORDER__MASK2_position     53
#define ND_500_DCR__CTRL_DET_ORDER__MASK2_get(x)       _BGQ_GET(5,53,x)
#define ND_500_DCR__CTRL_DET_ORDER__MASK2_set(v)       _BGQ_SET(5,53,v)
#define ND_500_DCR__CTRL_DET_ORDER__MASK2_insert(x,v)  _BGQ_INSERT(5,53,x,v)

//   Field:       mask3
//   Description: mask 3, for example, 00010 is D 4th.  One bit set for each mask, non-overlapping
#define ND_500_DCR__CTRL_DET_ORDER__MASK3_width        5
#define ND_500_DCR__CTRL_DET_ORDER__MASK3_position     58
#define ND_500_DCR__CTRL_DET_ORDER__MASK3_get(x)       _BGQ_GET(5,58,x)
#define ND_500_DCR__CTRL_DET_ORDER__MASK3_set(v)       _BGQ_SET(5,58,v)
#define ND_500_DCR__CTRL_DET_ORDER__MASK3_insert(x,v)  _BGQ_INSERT(5,58,x,v)

//   Field:       mask4
//   Description: mask 4, for example, 00001 is E 5th.  One bit set for each mask, non-overlapping
#define ND_500_DCR__CTRL_DET_ORDER__MASK4_width        5
#define ND_500_DCR__CTRL_DET_ORDER__MASK4_position     63
#define ND_500_DCR__CTRL_DET_ORDER__MASK4_get(x)       _BGQ_GET(5,63,x)
#define ND_500_DCR__CTRL_DET_ORDER__MASK4_set(v)       _BGQ_SET(5,63,v)
#define ND_500_DCR__CTRL_DET_ORDER__MASK4_insert(x,v)  _BGQ_INSERT(5,63,x,v)

  
// Register:
//   Name:        ctrl_dynamic_zone01
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: dynamic zone mask 0 and 1
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01_offset  (0x0008) // Offset of register in instance 
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01_reset  UNSIGNED64(0x0000000000000000)

//   Field:       zone0_mask0
//   Description: zone0 mask0, for example, 11000 is routing A or B 1st
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK0_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK0_position     11
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK0_get(x)       _BGQ_GET(5,11,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK0_set(v)       _BGQ_SET(5,11,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK0_insert(x,v)  _BGQ_INSERT(5,11,x,v)

//   Field:       zone0_mask1
//   Description: zone0 mask1, for example, 00110 is routing C or D 2nd
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK1_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK1_position     16
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK1_get(x)       _BGQ_GET(5,16,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK1_set(v)       _BGQ_SET(5,16,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK1_insert(x,v)  _BGQ_INSERT(5,16,x,v)

//   Field:       zone0_mask2
//   Description: zone0 mask2, for example, 00001 is routing E 3rd
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK2_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK2_position     21
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK2_get(x)       _BGQ_GET(5,21,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK2_set(v)       _BGQ_SET(5,21,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK2_insert(x,v)  _BGQ_INSERT(5,21,x,v)

//   Field:       zone0_mask3
//   Description: zone0 mask3, for example, 00000 is 4th routing zone, not active in this example
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK3_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK3_position     26
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK3_get(x)       _BGQ_GET(5,26,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK3_set(v)       _BGQ_SET(5,26,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK3_insert(x,v)  _BGQ_INSERT(5,26,x,v)

//   Field:       zone0_mask4
//   Description: zone0 mask4, for example, 00000 is 5th routing zone, not active in this example
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK4_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK4_position     31
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK4_get(x)       _BGQ_GET(5,31,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK4_set(v)       _BGQ_SET(5,31,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE0_MASK4_insert(x,v)  _BGQ_INSERT(5,31,x,v)

//   Field:       zone1_mask0
//   Description: zone1 mask0, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK0_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK0_position     43
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK0_get(x)       _BGQ_GET(5,43,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK0_set(v)       _BGQ_SET(5,43,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK0_insert(x,v)  _BGQ_INSERT(5,43,x,v)

//   Field:       zone1_mask1
//   Description: zone1 mask1, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK1_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK1_position     48
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK1_get(x)       _BGQ_GET(5,48,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK1_set(v)       _BGQ_SET(5,48,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK1_insert(x,v)  _BGQ_INSERT(5,48,x,v)

//   Field:       zone1_mask2
//   Description: zone1 mask2, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK2_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK2_position     53
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK2_get(x)       _BGQ_GET(5,53,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK2_set(v)       _BGQ_SET(5,53,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK2_insert(x,v)  _BGQ_INSERT(5,53,x,v)

//   Field:       zone1_mask3
//   Description: zone1 mask3, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK3_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK3_position     58
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK3_get(x)       _BGQ_GET(5,58,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK3_set(v)       _BGQ_SET(5,58,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK3_insert(x,v)  _BGQ_INSERT(5,58,x,v)

//   Field:       zone1_mask4
//   Description: zone1 mask4, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK4_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK4_position     63
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK4_get(x)       _BGQ_GET(5,63,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK4_set(v)       _BGQ_SET(5,63,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE01__ZONE1_MASK4_insert(x,v)  _BGQ_INSERT(5,63,x,v)

  
// Register:
//   Name:        ctrl_dynamic_zone23
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: dynamic zone mask 2 and 3
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23_offset  (0x0009) // Offset of register in instance 
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23_reset  UNSIGNED64(0x0000000000000000)

//   Field:       zone2_mask0
//   Description: zone2 mask0, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK0_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK0_position     11
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK0_get(x)       _BGQ_GET(5,11,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK0_set(v)       _BGQ_SET(5,11,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK0_insert(x,v)  _BGQ_INSERT(5,11,x,v)

//   Field:       zone2_mask1
//   Description: zone2 mask1, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK1_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK1_position     16
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK1_get(x)       _BGQ_GET(5,16,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK1_set(v)       _BGQ_SET(5,16,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK1_insert(x,v)  _BGQ_INSERT(5,16,x,v)

//   Field:       zone2_mask2
//   Description: zone2 mask2, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK2_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK2_position     21
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK2_get(x)       _BGQ_GET(5,21,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK2_set(v)       _BGQ_SET(5,21,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK2_insert(x,v)  _BGQ_INSERT(5,21,x,v)

//   Field:       zone2_mask3
//   Description: zone2 mask3, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK3_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK3_position     26
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK3_get(x)       _BGQ_GET(5,26,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK3_set(v)       _BGQ_SET(5,26,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK3_insert(x,v)  _BGQ_INSERT(5,26,x,v)

//   Field:       zone2_mask4
//   Description: zone2 mask4, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK4_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK4_position     31
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK4_get(x)       _BGQ_GET(5,31,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK4_set(v)       _BGQ_SET(5,31,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE2_MASK4_insert(x,v)  _BGQ_INSERT(5,31,x,v)

//   Field:       zone3_mask0
//   Description: zone3 mask0, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK0_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK0_position     43
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK0_get(x)       _BGQ_GET(5,43,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK0_set(v)       _BGQ_SET(5,43,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK0_insert(x,v)  _BGQ_INSERT(5,43,x,v)

//   Field:       zone3_mask1
//   Description: zone3 mask1, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK1_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK1_position     48
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK1_get(x)       _BGQ_GET(5,48,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK1_set(v)       _BGQ_SET(5,48,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK1_insert(x,v)  _BGQ_INSERT(5,48,x,v)

//   Field:       zone3_mask2
//   Description: zone3 mask2, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK2_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK2_position     53
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK2_get(x)       _BGQ_GET(5,53,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK2_set(v)       _BGQ_SET(5,53,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK2_insert(x,v)  _BGQ_INSERT(5,53,x,v)

//   Field:       zone3_mask3
//   Description: zone3 mask3, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK3_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK3_position     58
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK3_get(x)       _BGQ_GET(5,58,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK3_set(v)       _BGQ_SET(5,58,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK3_insert(x,v)  _BGQ_INSERT(5,58,x,v)

//   Field:       zone3_mask4
//   Description: zone3 mask4, see above zone0
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK4_width        5
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK4_position     63
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK4_get(x)       _BGQ_GET(5,63,x)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK4_set(v)       _BGQ_SET(5,63,v)
#define ND_500_DCR__CTRL_DYNAMIC_ZONE23__ZONE3_MASK4_insert(x,v)  _BGQ_INSERT(5,63,x,v)

  
// Register:
//   Name:        ctrl_timeout
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000002000200
//   Description: control timeout
#define ND_500_DCR__CTRL_TIMEOUT_offset  (0x000A) // Offset of register in instance 
#define ND_500_DCR__CTRL_TIMEOUT_reset  UNSIGNED64(0x0000000002000200)

//   Field:       coll_grant
//   Description: collective grant timeout in network cycles
#define ND_500_DCR__CTRL_TIMEOUT__COLL_GRANT_width        16
#define ND_500_DCR__CTRL_TIMEOUT__COLL_GRANT_position     47
#define ND_500_DCR__CTRL_TIMEOUT__COLL_GRANT_get(x)       _BGQ_GET(16,47,x)
#define ND_500_DCR__CTRL_TIMEOUT__COLL_GRANT_set(v)       _BGQ_SET(16,47,v)
#define ND_500_DCR__CTRL_TIMEOUT__COLL_GRANT_insert(x,v)  _BGQ_INSERT(16,47,x,v)

//   Field:       se_retrans
//   Description: sender retransmission timeout in network cycles
#define ND_500_DCR__CTRL_TIMEOUT__SE_RETRANS_width        16
#define ND_500_DCR__CTRL_TIMEOUT__SE_RETRANS_position     63
#define ND_500_DCR__CTRL_TIMEOUT__SE_RETRANS_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__CTRL_TIMEOUT__SE_RETRANS_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__CTRL_TIMEOUT__SE_RETRANS_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_00_01
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 0, 1 map
#define ND_500_DCR__CTRL_GI_CLASS_00_01_offset  (0x000B) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_00_01_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class00_up_port_i
//   Description: class 0 up tree input ports, 10 bits for each torus direction, in A-, A+, B-, B+, ... E-, E+ order, 1 bit for IO link, 1 bit for local 
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class00_up_port_o
//   Description: class 0 up tree output ports, 10 bits for each torus direction, 1 bit for IO link, the local bit should always be 0
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS00_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class01_up_port_i
//   Description: class 1 up tree input ports, see class 0
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class01_up_port_o
//   Description: class 1 up tree output ports, see class 0
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_00_01__CLASS01_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_02_03
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 2, 3 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_02_03_offset  (0x000C) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_02_03_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class02_up_port_i
//   Description: class 2 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class02_up_port_o
//   Description: class 2 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS02_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class03_up_port_i
//   Description: class 3 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class03_up_port_o
//   Description: class 3 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_02_03__CLASS03_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_04_05
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 4, 5 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_04_05_offset  (0x000D) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_04_05_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class04_up_port_i
//   Description: class 4 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class04_up_port_o
//   Description: class 4 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS04_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class05_up_port_i
//   Description: class 5 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class05_up_port_o
//   Description: class 5 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_04_05__CLASS05_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_06_07
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 6, 7 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_06_07_offset  (0x000E) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_06_07_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class06_up_port_i
//   Description: class 6 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class06_up_port_o
//   Description: class 6 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS06_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class07_up_port_i
//   Description: class 7 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class07_up_port_o
//   Description: class 7 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_06_07__CLASS07_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_08_09
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 8, 9 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_08_09_offset  (0x000F) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_08_09_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class08_up_port_i
//   Description: class 8 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class08_up_port_o
//   Description: class 8 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS08_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class09_up_port_i
//   Description: class 9 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class09_up_port_o
//   Description: class 9 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_08_09__CLASS09_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_10_11
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 10, 11 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_10_11_offset  (0x0010) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_10_11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class10_up_port_i
//   Description: class 10 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class10_up_port_o
//   Description: class 10 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS10_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class11_up_port_i
//   Description: class 11 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class11_up_port_o
//   Description: class 11 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_10_11__CLASS11_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_12_13
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 12, 13 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_12_13_offset  (0x0011) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_12_13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class12_up_port_i
//   Description: class 12 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class12_up_port_o
//   Description: class 12 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS12_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class13_up_port_i
//   Description: class 13 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class13_up_port_o
//   Description: class 13 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_12_13__CLASS13_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_gi_class_14_15
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: gi class 14, 15 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_GI_CLASS_14_15_offset  (0x0012) // Offset of register in instance 
#define ND_500_DCR__CTRL_GI_CLASS_14_15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class14_up_port_i
//   Description: class 14 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class14_up_port_o
//   Description: class 14 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS14_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class15_up_port_i
//   Description: class 15 up tree input ports
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class15_up_port_o
//   Description: class 15 up tree output ports
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_GI_CLASS_14_15__CLASS15_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_00_01
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 0, 1 map
#define ND_500_DCR__CTRL_COLL_CLASS_00_01_offset  (0x0013) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_00_01_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class00_type
//   Description: class 0 type, 0 user comm world, 1 system, 2 user subcomm
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class00_up_port_i
//   Description: class 0 up tree input ports, 10 bits torus, in the order of A-, A+, B-, B+, ..., E-, E+, 1 bit IO link, 1 bit local
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class00_up_port_o
//   Description: class 0 up tree output ports, 10 bit torus, 1 bit I/O, 1 bit local.  The local bit should always be 0
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS00_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class01_type
//   Description: class 1 type, see class 0
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class01_up_port_i
//   Description: class 1 up tree input ports, see class 0
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class01_up_port_o
//   Description: class 1 up tree output ports, see class 0
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_00_01__CLASS01_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_02_03
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 2, 3 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_02_03_offset  (0x0014) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_02_03_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class02_type
//   Description: class 2 type
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class02_up_port_i
//   Description: class 2 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class02_up_port_o
//   Description: class 2 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS02_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class03_type
//   Description: class 3 type
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class03_up_port_i
//   Description: class 3 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class03_up_port_o
//   Description: class 3 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_02_03__CLASS03_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_04_05
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 4, 5 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_04_05_offset  (0x0015) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_04_05_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class04_type
//   Description: class 4 type
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class04_up_port_i
//   Description: class 4 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class04_up_port_o
//   Description: class 4 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS04_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class05_type
//   Description: class 5 type
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class05_up_port_i
//   Description: class 5 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class05_up_port_o
//   Description: class 5 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_04_05__CLASS05_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_06_07
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 6, 7 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_06_07_offset  (0x0016) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_06_07_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class06_type
//   Description: class 6 type
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class06_up_port_i
//   Description: class 6 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class06_up_port_o
//   Description: class 6 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS06_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class07_type
//   Description: class 7 type
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class07_up_port_i
//   Description: class 7 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class07_up_port_o
//   Description: class 7 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_06_07__CLASS07_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_08_09
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 8, 9 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_08_09_offset  (0x0017) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_08_09_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class08_type
//   Description: class 8 type
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class08_up_port_i
//   Description: class 8 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class08_up_port_o
//   Description: class 8 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS08_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class09_type
//   Description: class 9 type
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class09_up_port_i
//   Description: class 9 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class09_up_port_o
//   Description: class 9 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_08_09__CLASS09_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_10_11
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 10, 11 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_10_11_offset  (0x0018) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_10_11_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class10_type
//   Description: class 10 type
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class10_up_port_i
//   Description: class 10 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class10_up_port_o
//   Description: class 10 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS10_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class11_type
//   Description: class 11 type
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class11_up_port_i
//   Description: class 11 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class11_up_port_o
//   Description: class 11 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_10_11__CLASS11_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_12_13
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 12, 13 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_12_13_offset  (0x0019) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_12_13_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class12_type
//   Description: class 12 type
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class12_up_port_i
//   Description: class 12 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class12_up_port_o
//   Description: class 12 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS12_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class13_type
//   Description: class 13 type
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class13_up_port_i
//   Description: class 13 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class13_up_port_o
//   Description: class 13 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_12_13__CLASS13_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        ctrl_coll_class_14_15
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class 14, 15 map, see class 0 on how to setup
#define ND_500_DCR__CTRL_COLL_CLASS_14_15_offset  (0x001A) // Offset of register in instance 
#define ND_500_DCR__CTRL_COLL_CLASS_14_15_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class14_type
//   Description: class 14 type
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_TYPE_position     3
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_TYPE_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_TYPE_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_TYPE_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       class14_up_port_i
//   Description: class 14 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_I_position     15
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_I_get(x)       _BGQ_GET(12,15,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_I_set(v)       _BGQ_SET(12,15,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,15,x,v)

//   Field:       class14_up_port_o
//   Description: class 14 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_O_position     31
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_O_get(x)       _BGQ_GET(12,31,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_O_set(v)       _BGQ_SET(12,31,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS14_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       class15_type
//   Description: class 15 type
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_TYPE_width        2
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_TYPE_position     35
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_TYPE_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_TYPE_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_TYPE_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       class15_up_port_i
//   Description: class 15 up tree input ports
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_I_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_I_position     47
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_I_get(x)       _BGQ_GET(12,47,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_I_set(v)       _BGQ_SET(12,47,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_I_insert(x,v)  _BGQ_INSERT(12,47,x,v)

//   Field:       class15_up_port_o
//   Description: class 15 up tree output ports
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_O_width        12
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_O_position     63
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_O_get(x)       _BGQ_GET(12,63,x)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_O_set(v)       _BGQ_SET(12,63,v)
#define ND_500_DCR__CTRL_COLL_CLASS_14_15__CLASS15_UP_PORT_O_insert(x,v)  _BGQ_INSERT(12,63,x,v)

  
// Register:
//   Name:        fatal_err_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: fatal error n enable
#define ND_500_DCR__FATAL_ERR_ENABLE_offset  (0x001B) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR_ENABLE_range  (0x6) // Range of external address space
#define ND_500_DCR__FATAL_ERR_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: fatal error 0 enable
#define ND_500_DCR__FATAL_ERR_ENABLE__VALUE_width        64
#define ND_500_DCR__FATAL_ERR_ENABLE__VALUE_position     63
#define ND_500_DCR__FATAL_ERR_ENABLE__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__FATAL_ERR_ENABLE__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__FATAL_ERR_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        non_fatal_err_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: non fatal error n enable
#define ND_500_DCR__NON_FATAL_ERR_ENABLE_offset  (0x0021) // Offset of register in instance 
#define ND_500_DCR__NON_FATAL_ERR_ENABLE_range  (0x2) // Range of external address space
#define ND_500_DCR__NON_FATAL_ERR_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: non fatal error 0 enable
#define ND_500_DCR__NON_FATAL_ERR_ENABLE__VALUE_width        64
#define ND_500_DCR__NON_FATAL_ERR_ENABLE__VALUE_position     63
#define ND_500_DCR__NON_FATAL_ERR_ENABLE__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__NON_FATAL_ERR_ENABLE__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__NON_FATAL_ERR_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        fatal_err0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 0
#define ND_500_DCR__FATAL_ERR0_offset  (0x0023) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_parity_err
//   Description: injection fifo t0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_PARITY_ERR_position     0
#define ND_500_DCR__FATAL_ERR0__INJ_T0_PARITY_ERR_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_PARITY_ERR_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t0_inv_state
//   Description: injection fifo t0 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_INV_STATE_position     1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_INV_STATE_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_INV_STATE_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t0_ue0
//   Description: injection fifo t0 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE0_position     2
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE0_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE0_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE0_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t0_ue1
//   Description: injection fifo t0 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE1_position     3
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE1_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE1_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_UE1_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t0_valid_bytes_err
//   Description: injection fifo t0 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T0_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_VALID_BYTES_ERR_position     4
#define ND_500_DCR__FATAL_ERR0__INJ_T0_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t1_parity_err
//   Description: injection fifo t1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_PARITY_ERR_position     5
#define ND_500_DCR__FATAL_ERR0__INJ_T1_PARITY_ERR_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_PARITY_ERR_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t1_inv_state
//   Description: injection fifo t1 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_INV_STATE_position     6
#define ND_500_DCR__FATAL_ERR0__INJ_T1_INV_STATE_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_INV_STATE_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t1_ue0
//   Description: injection fifo t1 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE0_position     7
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE0_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE0_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE0_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t1_ue1
//   Description: injection fifo t1 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE1_position     8
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE1_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE1_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_UE1_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t1_valid_bytes_err
//   Description: injection fifo t1 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T1_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T1_VALID_BYTES_ERR_position     9
#define ND_500_DCR__FATAL_ERR0__INJ_T1_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T1_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_t2_parity_err
//   Description: injection fifo t2 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T2_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_PARITY_ERR_position     10
#define ND_500_DCR__FATAL_ERR0__INJ_T2_PARITY_ERR_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_PARITY_ERR_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_t2_inv_state
//   Description: injection fifo t2 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T2_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_INV_STATE_position     11
#define ND_500_DCR__FATAL_ERR0__INJ_T2_INV_STATE_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_INV_STATE_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_t2_ue0
//   Description: injection fifo t2 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE0_position     12
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE0_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE0_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE0_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_t2_ue1
//   Description: injection fifo t2 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE1_position     13
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE1_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE1_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_UE1_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_t2_valid_bytes_err
//   Description: injection fifo t2 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T2_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T2_VALID_BYTES_ERR_position     14
#define ND_500_DCR__FATAL_ERR0__INJ_T2_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,14,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,14,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T2_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_t3_parity_err
//   Description: injection fifo t3 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T3_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_PARITY_ERR_position     15
#define ND_500_DCR__FATAL_ERR0__INJ_T3_PARITY_ERR_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_PARITY_ERR_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       inj_t3_inv_state
//   Description: injection fifo t3 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T3_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_INV_STATE_position     16
#define ND_500_DCR__FATAL_ERR0__INJ_T3_INV_STATE_get(x)       _BGQ_GET(1,16,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_INV_STATE_set(v)       _BGQ_SET(1,16,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_INV_STATE_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       inj_t3_ue0
//   Description: injection fifo t3 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE0_position     17
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE0_get(x)       _BGQ_GET(1,17,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE0_set(v)       _BGQ_SET(1,17,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE0_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       inj_t3_ue1
//   Description: injection fifo t3 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE1_position     18
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE1_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE1_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_UE1_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       inj_t3_valid_bytes_err
//   Description: injection fifo t3 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T3_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T3_VALID_BYTES_ERR_position     19
#define ND_500_DCR__FATAL_ERR0__INJ_T3_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,19,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,19,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T3_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       inj_t0_t3_top_parity_err
//   Description: injection fifo t0 to t3 clk_500 top level parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T0_T3_TOP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T0_T3_TOP_PARITY_ERR_position     20
#define ND_500_DCR__FATAL_ERR0__INJ_T0_T3_TOP_PARITY_ERR_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_T3_TOP_PARITY_ERR_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T0_T3_TOP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       inj_t4_parity_err
//   Description: injection fifo t4 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T4_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_PARITY_ERR_position     21
#define ND_500_DCR__FATAL_ERR0__INJ_T4_PARITY_ERR_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_PARITY_ERR_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       inj_t4_inv_state
//   Description: injection fifo t4 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T4_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_INV_STATE_position     22
#define ND_500_DCR__FATAL_ERR0__INJ_T4_INV_STATE_get(x)       _BGQ_GET(1,22,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_INV_STATE_set(v)       _BGQ_SET(1,22,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_INV_STATE_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       inj_t4_ue0
//   Description: injection fifo t4 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE0_position     23
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE0_get(x)       _BGQ_GET(1,23,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE0_set(v)       _BGQ_SET(1,23,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE0_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       inj_t4_ue1
//   Description: injection fifo t4 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE1_position     24
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE1_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE1_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_UE1_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       inj_t4_valid_bytes_err
//   Description: injection fifo t4 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T4_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_VALID_BYTES_ERR_position     25
#define ND_500_DCR__FATAL_ERR0__INJ_T4_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       inj_t5_parity_err
//   Description: injection fifo t5 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T5_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_PARITY_ERR_position     26
#define ND_500_DCR__FATAL_ERR0__INJ_T5_PARITY_ERR_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_PARITY_ERR_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       inj_t5_inv_state
//   Description: injection fifo t5 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T5_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_INV_STATE_position     27
#define ND_500_DCR__FATAL_ERR0__INJ_T5_INV_STATE_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_INV_STATE_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_INV_STATE_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       inj_t5_ue0
//   Description: injection fifo t5 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE0_position     28
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE0_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE0_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE0_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       inj_t5_ue1
//   Description: injection fifo t5 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE1_position     29
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE1_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE1_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_UE1_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       inj_t5_valid_bytes_err
//   Description: injection fifo t5 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T5_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T5_VALID_BYTES_ERR_position     30
#define ND_500_DCR__FATAL_ERR0__INJ_T5_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T5_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       inj_t6_parity_err
//   Description: injection fifo t6 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T6_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_PARITY_ERR_position     31
#define ND_500_DCR__FATAL_ERR0__INJ_T6_PARITY_ERR_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_PARITY_ERR_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       inj_t6_inv_state
//   Description: injection fifo t6 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T6_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_INV_STATE_position     32
#define ND_500_DCR__FATAL_ERR0__INJ_T6_INV_STATE_get(x)       _BGQ_GET(1,32,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_INV_STATE_set(v)       _BGQ_SET(1,32,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_INV_STATE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       inj_t6_ue0
//   Description: injection fifo t6 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE0_position     33
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE0_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE0_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE0_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       inj_t6_ue1
//   Description: injection fifo t6 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE1_position     34
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE1_get(x)       _BGQ_GET(1,34,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE1_set(v)       _BGQ_SET(1,34,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_UE1_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       inj_t6_valid_bytes_err
//   Description: injection fifo t6 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T6_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T6_VALID_BYTES_ERR_position     35
#define ND_500_DCR__FATAL_ERR0__INJ_T6_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,35,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,35,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T6_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       inj_t7_parity_err
//   Description: injection fifo t7 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T7_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_PARITY_ERR_position     36
#define ND_500_DCR__FATAL_ERR0__INJ_T7_PARITY_ERR_get(x)       _BGQ_GET(1,36,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_PARITY_ERR_set(v)       _BGQ_SET(1,36,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       inj_t7_inv_state
//   Description: injection fifo t7 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T7_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_INV_STATE_position     37
#define ND_500_DCR__FATAL_ERR0__INJ_T7_INV_STATE_get(x)       _BGQ_GET(1,37,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_INV_STATE_set(v)       _BGQ_SET(1,37,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_INV_STATE_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       inj_t7_ue0
//   Description: injection fifo t7 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE0_position     38
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE0_get(x)       _BGQ_GET(1,38,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE0_set(v)       _BGQ_SET(1,38,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE0_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       inj_t7_ue1
//   Description: injection fifo t7 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE1_position     39
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE1_get(x)       _BGQ_GET(1,39,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE1_set(v)       _BGQ_SET(1,39,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_UE1_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       inj_t7_valid_bytes_err
//   Description: injection fifo t7 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T7_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T7_VALID_BYTES_ERR_position     40
#define ND_500_DCR__FATAL_ERR0__INJ_T7_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,40,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,40,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T7_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       inj_t4_t7_top_parity_err
//   Description: injection fifo t4 to t7 clk_500 top level parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T4_T7_TOP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T4_T7_TOP_PARITY_ERR_position     41
#define ND_500_DCR__FATAL_ERR0__INJ_T4_T7_TOP_PARITY_ERR_get(x)       _BGQ_GET(1,41,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_T7_TOP_PARITY_ERR_set(v)       _BGQ_SET(1,41,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T4_T7_TOP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,41,x,v)

//   Field:       inj_t8_parity_err
//   Description: injection fifo t8 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T8_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_PARITY_ERR_position     42
#define ND_500_DCR__FATAL_ERR0__INJ_T8_PARITY_ERR_get(x)       _BGQ_GET(1,42,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_PARITY_ERR_set(v)       _BGQ_SET(1,42,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       inj_t8_inv_state
//   Description: injection fifo t8 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T8_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_INV_STATE_position     43
#define ND_500_DCR__FATAL_ERR0__INJ_T8_INV_STATE_get(x)       _BGQ_GET(1,43,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_INV_STATE_set(v)       _BGQ_SET(1,43,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_INV_STATE_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       inj_t8_ue0
//   Description: injection fifo t8 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE0_position     44
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE0_get(x)       _BGQ_GET(1,44,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE0_set(v)       _BGQ_SET(1,44,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE0_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       inj_t8_ue1
//   Description: injection fifo t8 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE1_position     45
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE1_get(x)       _BGQ_GET(1,45,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE1_set(v)       _BGQ_SET(1,45,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_UE1_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       inj_t8_valid_bytes_err
//   Description: injection fifo t8 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T8_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_VALID_BYTES_ERR_position     46
#define ND_500_DCR__FATAL_ERR0__INJ_T8_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,46,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,46,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       inj_t9_parity_err
//   Description: injection fifo t9 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T9_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_PARITY_ERR_position     47
#define ND_500_DCR__FATAL_ERR0__INJ_T9_PARITY_ERR_get(x)       _BGQ_GET(1,47,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_PARITY_ERR_set(v)       _BGQ_SET(1,47,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       inj_t9_inv_state
//   Description: injection fifo t9 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_T9_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_INV_STATE_position     48
#define ND_500_DCR__FATAL_ERR0__INJ_T9_INV_STATE_get(x)       _BGQ_GET(1,48,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_INV_STATE_set(v)       _BGQ_SET(1,48,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_INV_STATE_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       inj_t9_ue0
//   Description: injection fifo t9 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE0_position     49
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE0_get(x)       _BGQ_GET(1,49,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE0_set(v)       _BGQ_SET(1,49,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE0_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       inj_t9_ue1
//   Description: injection fifo t9 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE1_position     50
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE1_get(x)       _BGQ_GET(1,50,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE1_set(v)       _BGQ_SET(1,50,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_UE1_insert(x,v)  _BGQ_INSERT(1,50,x,v)

//   Field:       inj_t9_valid_bytes_err
//   Description: injection fifo t9 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_T9_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T9_VALID_BYTES_ERR_position     51
#define ND_500_DCR__FATAL_ERR0__INJ_T9_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,51,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,51,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T9_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,51,x,v)

//   Field:       inj_l0_parity_err
//   Description: injection fifo l0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_L0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_PARITY_ERR_position     52
#define ND_500_DCR__FATAL_ERR0__INJ_L0_PARITY_ERR_get(x)       _BGQ_GET(1,52,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_PARITY_ERR_set(v)       _BGQ_SET(1,52,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,52,x,v)

//   Field:       inj_l0_inv_state
//   Description: injection fifo l0 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_L0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_INV_STATE_position     53
#define ND_500_DCR__FATAL_ERR0__INJ_L0_INV_STATE_get(x)       _BGQ_GET(1,53,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_INV_STATE_set(v)       _BGQ_SET(1,53,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,53,x,v)

//   Field:       inj_l0_ue0
//   Description: injection fifo l0 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE0_position     54
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE0_get(x)       _BGQ_GET(1,54,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE0_set(v)       _BGQ_SET(1,54,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE0_insert(x,v)  _BGQ_INSERT(1,54,x,v)

//   Field:       inj_l0_ue1
//   Description: injection fifo l0 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE1_position     55
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE1_get(x)       _BGQ_GET(1,55,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE1_set(v)       _BGQ_SET(1,55,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_UE1_insert(x,v)  _BGQ_INSERT(1,55,x,v)

//   Field:       inj_l0_valid_bytes_err
//   Description: injection fifo l0 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_L0_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L0_VALID_BYTES_ERR_position     56
#define ND_500_DCR__FATAL_ERR0__INJ_L0_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,56,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,56,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L0_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,56,x,v)

//   Field:       inj_l1_parity_err
//   Description: injection fifo l1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_L1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_PARITY_ERR_position     57
#define ND_500_DCR__FATAL_ERR0__INJ_L1_PARITY_ERR_get(x)       _BGQ_GET(1,57,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_PARITY_ERR_set(v)       _BGQ_SET(1,57,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,57,x,v)

//   Field:       inj_l1_inv_state
//   Description: injection fifo l1 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR0__INJ_L1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_INV_STATE_position     58
#define ND_500_DCR__FATAL_ERR0__INJ_L1_INV_STATE_get(x)       _BGQ_GET(1,58,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_INV_STATE_set(v)       _BGQ_SET(1,58,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,58,x,v)

//   Field:       inj_l1_ue0
//   Description: injection fifo l1 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE0_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE0_position     59
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE0_get(x)       _BGQ_GET(1,59,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE0_set(v)       _BGQ_SET(1,59,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE0_insert(x,v)  _BGQ_INSERT(1,59,x,v)

//   Field:       inj_l1_ue1
//   Description: injection fifo l1 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE1_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE1_position     60
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE1_get(x)       _BGQ_GET(1,60,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE1_set(v)       _BGQ_SET(1,60,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_UE1_insert(x,v)  _BGQ_INSERT(1,60,x,v)

//   Field:       inj_l1_valid_bytes_err
//   Description: injection fifo l1 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR0__INJ_L1_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_L1_VALID_BYTES_ERR_position     61
#define ND_500_DCR__FATAL_ERR0__INJ_L1_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,61,x)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,61,v)
#define ND_500_DCR__FATAL_ERR0__INJ_L1_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,61,x,v)

//   Field:       inj_t8_l1_top_parity_err
//   Description: injection fifo t8, t9, l0, l1 clk_500 top level parity error
#define ND_500_DCR__FATAL_ERR0__INJ_T8_L1_TOP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_T8_L1_TOP_PARITY_ERR_position     62
#define ND_500_DCR__FATAL_ERR0__INJ_T8_L1_TOP_PARITY_ERR_get(x)       _BGQ_GET(1,62,x)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_L1_TOP_PARITY_ERR_set(v)       _BGQ_SET(1,62,v)
#define ND_500_DCR__FATAL_ERR0__INJ_T8_L1_TOP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       inj_hp_parity_err
//   Description: injection fifo hp clk_500 parity error
#define ND_500_DCR__FATAL_ERR0__INJ_HP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR0__INJ_HP_PARITY_ERR_position     63
#define ND_500_DCR__FATAL_ERR0__INJ_HP_PARITY_ERR_get(x)       _BGQ_GET(1,63,x)
#define ND_500_DCR__FATAL_ERR0__INJ_HP_PARITY_ERR_set(v)       _BGQ_SET(1,63,v)
#define ND_500_DCR__FATAL_ERR0__INJ_HP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        fatal_err1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 1
#define ND_500_DCR__FATAL_ERR1_offset  (0x0024) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_hp_inv_state
//   Description: injection fifo hp clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INV_STATE_position     0
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INV_STATE_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INV_STATE_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INV_STATE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_hp_ue0
//   Description: injection fifo hp clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE0_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE0_position     1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE0_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE0_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE0_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_hp_ue1
//   Description: injection fifo hp clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE1_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE1_position     2
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE1_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE1_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_UE1_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_hp_valid_bytes_err
//   Description: injection fifo hp packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR1__INJ_HP_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_VALID_BYTES_ERR_position     3
#define ND_500_DCR__FATAL_ERR1__INJ_HP_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_io_parity_err
//   Description: injection fifo io clk_500 parity error
#define ND_500_DCR__FATAL_ERR1__INJ_IO_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_PARITY_ERR_position     4
#define ND_500_DCR__FATAL_ERR1__INJ_IO_PARITY_ERR_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_PARITY_ERR_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_io_inv_state
//   Description: injection fifo io clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INV_STATE_position     5
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INV_STATE_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INV_STATE_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INV_STATE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_io_ue0
//   Description: injection fifo io clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE0_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE0_position     6
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE0_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE0_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE0_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_io_ue1
//   Description: injection fifo io clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE1_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE1_position     7
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE1_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE1_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_UE1_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_io_valid_bytes_err
//   Description: injection fifo io packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR1__INJ_IO_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_VALID_BYTES_ERR_position     8
#define ND_500_DCR__FATAL_ERR1__INJ_IO_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_c0_parity_err
//   Description: injection fifo c0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR1__INJ_C0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_PARITY_ERR_position     9
#define ND_500_DCR__FATAL_ERR1__INJ_C0_PARITY_ERR_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_PARITY_ERR_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_c0_inv_state
//   Description: injection fifo c0 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INV_STATE_position     10
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INV_STATE_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INV_STATE_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_c0_ue0
//   Description: injection fifo c0 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE0_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE0_position     11
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE0_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE0_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE0_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_c0_ue1
//   Description: injection fifo c0 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE1_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE1_position     12
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE1_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE1_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_UE1_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_c0_valid_bytes_err
//   Description: injection fifo c0 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR1__INJ_C0_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_VALID_BYTES_ERR_position     13
#define ND_500_DCR__FATAL_ERR1__INJ_C0_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       inj_c1_parity_err
//   Description: injection fifo c1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR1__INJ_C1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_PARITY_ERR_position     14
#define ND_500_DCR__FATAL_ERR1__INJ_C1_PARITY_ERR_get(x)       _BGQ_GET(1,14,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_PARITY_ERR_set(v)       _BGQ_SET(1,14,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       inj_c1_inv_state
//   Description: injection fifo c1 clk_500 fsm invalid state
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INV_STATE_position     15
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INV_STATE_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INV_STATE_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       inj_c1_ue0
//   Description: injection fifo c1 clk_500 uncorrectable error 0
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE0_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE0_position     16
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE0_get(x)       _BGQ_GET(1,16,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE0_set(v)       _BGQ_SET(1,16,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE0_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       inj_c1_ue1
//   Description: injection fifo c1 clk_500 uncorrectable error 1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE1_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE1_position     17
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE1_get(x)       _BGQ_GET(1,17,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE1_set(v)       _BGQ_SET(1,17,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_UE1_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       inj_c1_valid_bytes_err
//   Description: injection fifo c1 packet number of valid bytes is greater than packet size
#define ND_500_DCR__FATAL_ERR1__INJ_C1_VALID_BYTES_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_VALID_BYTES_ERR_position     18
#define ND_500_DCR__FATAL_ERR1__INJ_C1_VALID_BYTES_ERR_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_VALID_BYTES_ERR_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_VALID_BYTES_ERR_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       inj_hp_c1_top_parity_err
//   Description: injection fifo hp, io, c0, c1 clk_500 top level parity error
#define ND_500_DCR__FATAL_ERR1__INJ_HP_C1_TOP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_C1_TOP_PARITY_ERR_position     19
#define ND_500_DCR__FATAL_ERR1__INJ_HP_C1_TOP_PARITY_ERR_get(x)       _BGQ_GET(1,19,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_C1_TOP_PARITY_ERR_set(v)       _BGQ_SET(1,19,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_C1_TOP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       inj_t0_invalid_grant
//   Description: injection fifo t0 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T0_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T0_INVALID_GRANT_position     20
#define ND_500_DCR__FATAL_ERR1__INJ_T0_INVALID_GRANT_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T0_INVALID_GRANT_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T0_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       inj_t1_invalid_grant
//   Description: injection fifo t1 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T1_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T1_INVALID_GRANT_position     21
#define ND_500_DCR__FATAL_ERR1__INJ_T1_INVALID_GRANT_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T1_INVALID_GRANT_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T1_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       inj_t2_invalid_grant
//   Description: injection fifo t2 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T2_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T2_INVALID_GRANT_position     22
#define ND_500_DCR__FATAL_ERR1__INJ_T2_INVALID_GRANT_get(x)       _BGQ_GET(1,22,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T2_INVALID_GRANT_set(v)       _BGQ_SET(1,22,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T2_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       inj_t3_invalid_grant
//   Description: injection fifo t3 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T3_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T3_INVALID_GRANT_position     23
#define ND_500_DCR__FATAL_ERR1__INJ_T3_INVALID_GRANT_get(x)       _BGQ_GET(1,23,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T3_INVALID_GRANT_set(v)       _BGQ_SET(1,23,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T3_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       inj_t4_invalid_grant
//   Description: injection fifo t4 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T4_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T4_INVALID_GRANT_position     24
#define ND_500_DCR__FATAL_ERR1__INJ_T4_INVALID_GRANT_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T4_INVALID_GRANT_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T4_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       inj_t5_invalid_grant
//   Description: injection fifo t5 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T5_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T5_INVALID_GRANT_position     25
#define ND_500_DCR__FATAL_ERR1__INJ_T5_INVALID_GRANT_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T5_INVALID_GRANT_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T5_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       inj_t6_invalid_grant
//   Description: injection fifo t6 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T6_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T6_INVALID_GRANT_position     26
#define ND_500_DCR__FATAL_ERR1__INJ_T6_INVALID_GRANT_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T6_INVALID_GRANT_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T6_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       inj_t7_invalid_grant
//   Description: injection fifo t7 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T7_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T7_INVALID_GRANT_position     27
#define ND_500_DCR__FATAL_ERR1__INJ_T7_INVALID_GRANT_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T7_INVALID_GRANT_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T7_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       inj_t8_invalid_grant
//   Description: injection fifo t8 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T8_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T8_INVALID_GRANT_position     28
#define ND_500_DCR__FATAL_ERR1__INJ_T8_INVALID_GRANT_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T8_INVALID_GRANT_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T8_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       inj_t9_invalid_grant
//   Description: injection fifo t9 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_T9_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T9_INVALID_GRANT_position     29
#define ND_500_DCR__FATAL_ERR1__INJ_T9_INVALID_GRANT_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T9_INVALID_GRANT_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T9_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       inj_l0_invalid_grant
//   Description: injection fifo l0 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_L0_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_L0_INVALID_GRANT_position     30
#define ND_500_DCR__FATAL_ERR1__INJ_L0_INVALID_GRANT_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__FATAL_ERR1__INJ_L0_INVALID_GRANT_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__FATAL_ERR1__INJ_L0_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       inj_l1_invalid_grant
//   Description: injection fifo l1 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_L1_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_L1_INVALID_GRANT_position     31
#define ND_500_DCR__FATAL_ERR1__INJ_L1_INVALID_GRANT_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__FATAL_ERR1__INJ_L1_INVALID_GRANT_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__FATAL_ERR1__INJ_L1_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       inj_hp_invalid_grant
//   Description: injection fifo hp received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INVALID_GRANT_position     32
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INVALID_GRANT_get(x)       _BGQ_GET(1,32,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INVALID_GRANT_set(v)       _BGQ_SET(1,32,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       inj_io_invalid_grant
//   Description: injection fifo io received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INVALID_GRANT_position     33
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INVALID_GRANT_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INVALID_GRANT_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       inj_c0_invalid_grant
//   Description: injection fifo c0 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INVALID_GRANT_position     34
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INVALID_GRANT_get(x)       _BGQ_GET(1,34,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INVALID_GRANT_set(v)       _BGQ_SET(1,34,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C0_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       inj_c1_invalid_grant
//   Description: injection fifo c1 received grant in incorrect state
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INVALID_GRANT_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INVALID_GRANT_position     35
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INVALID_GRANT_get(x)       _BGQ_GET(1,35,x)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INVALID_GRANT_set(v)       _BGQ_SET(1,35,v)
#define ND_500_DCR__FATAL_ERR1__INJ_C1_INVALID_GRANT_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       inj_t0_arb_parity_err
//   Description: injection fifo t0 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T0_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T0_ARB_PARITY_ERR_position     36
#define ND_500_DCR__FATAL_ERR1__INJ_T0_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,36,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T0_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,36,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T0_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       inj_t1_arb_parity_err
//   Description: injection fifo t1 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T1_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T1_ARB_PARITY_ERR_position     37
#define ND_500_DCR__FATAL_ERR1__INJ_T1_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,37,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T1_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,37,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T1_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       inj_t2_arb_parity_err
//   Description: injection fifo t2 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T2_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T2_ARB_PARITY_ERR_position     38
#define ND_500_DCR__FATAL_ERR1__INJ_T2_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,38,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T2_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,38,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T2_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       inj_t3_arb_parity_err
//   Description: injection fifo t3 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T3_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T3_ARB_PARITY_ERR_position     39
#define ND_500_DCR__FATAL_ERR1__INJ_T3_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,39,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T3_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,39,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T3_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       inj_t4_arb_parity_err
//   Description: injection fifo t4 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T4_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T4_ARB_PARITY_ERR_position     40
#define ND_500_DCR__FATAL_ERR1__INJ_T4_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,40,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T4_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,40,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T4_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       inj_t5_arb_parity_err
//   Description: injection fifo t5 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T5_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T5_ARB_PARITY_ERR_position     41
#define ND_500_DCR__FATAL_ERR1__INJ_T5_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,41,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T5_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,41,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T5_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,41,x,v)

//   Field:       inj_t6_arb_parity_err
//   Description: injection fifo t6 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T6_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T6_ARB_PARITY_ERR_position     42
#define ND_500_DCR__FATAL_ERR1__INJ_T6_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,42,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T6_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,42,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T6_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       inj_t7_arb_parity_err
//   Description: injection fifo t7 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T7_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T7_ARB_PARITY_ERR_position     43
#define ND_500_DCR__FATAL_ERR1__INJ_T7_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,43,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T7_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,43,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T7_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       inj_t8_arb_parity_err
//   Description: injection fifo t8 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T8_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T8_ARB_PARITY_ERR_position     44
#define ND_500_DCR__FATAL_ERR1__INJ_T8_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,44,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T8_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,44,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T8_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       inj_t9_arb_parity_err
//   Description: injection fifo t9 arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_T9_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_T9_ARB_PARITY_ERR_position     45
#define ND_500_DCR__FATAL_ERR1__INJ_T9_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,45,x)
#define ND_500_DCR__FATAL_ERR1__INJ_T9_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,45,v)
#define ND_500_DCR__FATAL_ERR1__INJ_T9_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       inj_hp_arb_parity_err
//   Description: injection fifo hp arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_HP_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_HP_ARB_PARITY_ERR_position     46
#define ND_500_DCR__FATAL_ERR1__INJ_HP_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,46,x)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,46,v)
#define ND_500_DCR__FATAL_ERR1__INJ_HP_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       inj_io_arb_parity_err
//   Description: injection fifo io arbiter parity error
#define ND_500_DCR__FATAL_ERR1__INJ_IO_ARB_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR1__INJ_IO_ARB_PARITY_ERR_position     47
#define ND_500_DCR__FATAL_ERR1__INJ_IO_ARB_PARITY_ERR_get(x)       _BGQ_GET(1,47,x)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_ARB_PARITY_ERR_set(v)       _BGQ_SET(1,47,v)
#define ND_500_DCR__FATAL_ERR1__INJ_IO_ARB_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       reserved
//   Description: reserved, tied to zero
#define ND_500_DCR__FATAL_ERR1__RESERVED_width        16
#define ND_500_DCR__FATAL_ERR1__RESERVED_position     63
#define ND_500_DCR__FATAL_ERR1__RESERVED_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__FATAL_ERR1__RESERVED_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__FATAL_ERR1__RESERVED_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        fatal_err2
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 2
#define ND_500_DCR__FATAL_ERR2_offset  (0x0025) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_parity_err
//   Description: reception fifo t0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T0_PARITY_ERR_position     0
#define ND_500_DCR__FATAL_ERR2__RCP_T0_PARITY_ERR_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T0_PARITY_ERR_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       rcp_t0_inv_state
//   Description: reception fifo t0 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T0_INV_STATE_position     1
#define ND_500_DCR__FATAL_ERR2__RCP_T0_INV_STATE_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T0_INV_STATE_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       rcp_t1_parity_err
//   Description: reception fifo t1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T1_PARITY_ERR_position     2
#define ND_500_DCR__FATAL_ERR2__RCP_T1_PARITY_ERR_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T1_PARITY_ERR_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       rcp_t1_inv_state
//   Description: reception fifo t1 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T1_INV_STATE_position     3
#define ND_500_DCR__FATAL_ERR2__RCP_T1_INV_STATE_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T1_INV_STATE_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       rcp_t2_parity_err
//   Description: reception fifo t2 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T2_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T2_PARITY_ERR_position     4
#define ND_500_DCR__FATAL_ERR2__RCP_T2_PARITY_ERR_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T2_PARITY_ERR_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T2_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       rcp_t2_inv_state
//   Description: reception fifo t2 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T2_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T2_INV_STATE_position     5
#define ND_500_DCR__FATAL_ERR2__RCP_T2_INV_STATE_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T2_INV_STATE_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T2_INV_STATE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       rcp_t3_parity_err
//   Description: reception fifo t3 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T3_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T3_PARITY_ERR_position     6
#define ND_500_DCR__FATAL_ERR2__RCP_T3_PARITY_ERR_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T3_PARITY_ERR_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T3_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       rcp_t3_inv_state
//   Description: reception fifo t3 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T3_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T3_INV_STATE_position     7
#define ND_500_DCR__FATAL_ERR2__RCP_T3_INV_STATE_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T3_INV_STATE_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T3_INV_STATE_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       rcp_t4_parity_err
//   Description: reception fifo t4 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T4_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T4_PARITY_ERR_position     8
#define ND_500_DCR__FATAL_ERR2__RCP_T4_PARITY_ERR_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T4_PARITY_ERR_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T4_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       rcp_t4_inv_state
//   Description: reception fifo t4 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T4_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T4_INV_STATE_position     9
#define ND_500_DCR__FATAL_ERR2__RCP_T4_INV_STATE_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T4_INV_STATE_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T4_INV_STATE_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       rcp_t5_parity_err
//   Description: reception fifo t5 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T5_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T5_PARITY_ERR_position     10
#define ND_500_DCR__FATAL_ERR2__RCP_T5_PARITY_ERR_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T5_PARITY_ERR_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T5_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       rcp_t5_inv_state
//   Description: reception fifo t5 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T5_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T5_INV_STATE_position     11
#define ND_500_DCR__FATAL_ERR2__RCP_T5_INV_STATE_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T5_INV_STATE_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T5_INV_STATE_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       rcp_t6_parity_err
//   Description: reception fifo t6 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T6_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T6_PARITY_ERR_position     12
#define ND_500_DCR__FATAL_ERR2__RCP_T6_PARITY_ERR_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T6_PARITY_ERR_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T6_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       rcp_t6_inv_state
//   Description: reception fifo t6 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T6_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T6_INV_STATE_position     13
#define ND_500_DCR__FATAL_ERR2__RCP_T6_INV_STATE_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T6_INV_STATE_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T6_INV_STATE_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       rcp_t7_parity_err
//   Description: reception fifo t7 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T7_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T7_PARITY_ERR_position     14
#define ND_500_DCR__FATAL_ERR2__RCP_T7_PARITY_ERR_get(x)       _BGQ_GET(1,14,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T7_PARITY_ERR_set(v)       _BGQ_SET(1,14,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T7_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       rcp_t7_inv_state
//   Description: reception fifo t7 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T7_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T7_INV_STATE_position     15
#define ND_500_DCR__FATAL_ERR2__RCP_T7_INV_STATE_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T7_INV_STATE_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T7_INV_STATE_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rcp_t8_parity_err
//   Description: reception fifo t8 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T8_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T8_PARITY_ERR_position     16
#define ND_500_DCR__FATAL_ERR2__RCP_T8_PARITY_ERR_get(x)       _BGQ_GET(1,16,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T8_PARITY_ERR_set(v)       _BGQ_SET(1,16,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T8_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rcp_t8_inv_state
//   Description: reception fifo t8 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T8_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T8_INV_STATE_position     17
#define ND_500_DCR__FATAL_ERR2__RCP_T8_INV_STATE_get(x)       _BGQ_GET(1,17,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T8_INV_STATE_set(v)       _BGQ_SET(1,17,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T8_INV_STATE_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       rcp_t9_parity_err
//   Description: reception fifo t9 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_T9_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T9_PARITY_ERR_position     18
#define ND_500_DCR__FATAL_ERR2__RCP_T9_PARITY_ERR_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T9_PARITY_ERR_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T9_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       rcp_t9_inv_state
//   Description: reception fifo t9 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_T9_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_T9_INV_STATE_position     19
#define ND_500_DCR__FATAL_ERR2__RCP_T9_INV_STATE_get(x)       _BGQ_GET(1,19,x)
#define ND_500_DCR__FATAL_ERR2__RCP_T9_INV_STATE_set(v)       _BGQ_SET(1,19,v)
#define ND_500_DCR__FATAL_ERR2__RCP_T9_INV_STATE_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rcp_l0_parity_err
//   Description: reception fifo l0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_L0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_L0_PARITY_ERR_position     20
#define ND_500_DCR__FATAL_ERR2__RCP_L0_PARITY_ERR_get(x)       _BGQ_GET(1,20,x)
#define ND_500_DCR__FATAL_ERR2__RCP_L0_PARITY_ERR_set(v)       _BGQ_SET(1,20,v)
#define ND_500_DCR__FATAL_ERR2__RCP_L0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       rcp_l0_inv_state
//   Description: reception fifo l0 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_L0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_L0_INV_STATE_position     21
#define ND_500_DCR__FATAL_ERR2__RCP_L0_INV_STATE_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__FATAL_ERR2__RCP_L0_INV_STATE_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__FATAL_ERR2__RCP_L0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rcp_l1_parity_err
//   Description: reception fifo l1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_L1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_L1_PARITY_ERR_position     22
#define ND_500_DCR__FATAL_ERR2__RCP_L1_PARITY_ERR_get(x)       _BGQ_GET(1,22,x)
#define ND_500_DCR__FATAL_ERR2__RCP_L1_PARITY_ERR_set(v)       _BGQ_SET(1,22,v)
#define ND_500_DCR__FATAL_ERR2__RCP_L1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rcp_l1_inv_state
//   Description: reception fifo l1 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_L1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_L1_INV_STATE_position     23
#define ND_500_DCR__FATAL_ERR2__RCP_L1_INV_STATE_get(x)       _BGQ_GET(1,23,x)
#define ND_500_DCR__FATAL_ERR2__RCP_L1_INV_STATE_set(v)       _BGQ_SET(1,23,v)
#define ND_500_DCR__FATAL_ERR2__RCP_L1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       rcp_hp_parity_err
//   Description: reception fifo hp clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_HP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_HP_PARITY_ERR_position     24
#define ND_500_DCR__FATAL_ERR2__RCP_HP_PARITY_ERR_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__FATAL_ERR2__RCP_HP_PARITY_ERR_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__FATAL_ERR2__RCP_HP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       rcp_hp_inv_state
//   Description: reception fifo hp clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_HP_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_HP_INV_STATE_position     25
#define ND_500_DCR__FATAL_ERR2__RCP_HP_INV_STATE_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__FATAL_ERR2__RCP_HP_INV_STATE_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__FATAL_ERR2__RCP_HP_INV_STATE_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_io_parity_err
//   Description: reception fifo io clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_IO_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_IO_PARITY_ERR_position     26
#define ND_500_DCR__FATAL_ERR2__RCP_IO_PARITY_ERR_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__FATAL_ERR2__RCP_IO_PARITY_ERR_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__FATAL_ERR2__RCP_IO_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_io_inv_state
//   Description: reception fifo io clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_IO_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_IO_INV_STATE_position     27
#define ND_500_DCR__FATAL_ERR2__RCP_IO_INV_STATE_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__FATAL_ERR2__RCP_IO_INV_STATE_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__FATAL_ERR2__RCP_IO_INV_STATE_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rcp_c0_parity_err
//   Description: reception fifo c0 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_C0_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_C0_PARITY_ERR_position     28
#define ND_500_DCR__FATAL_ERR2__RCP_C0_PARITY_ERR_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__FATAL_ERR2__RCP_C0_PARITY_ERR_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__FATAL_ERR2__RCP_C0_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_c0_inv_state
//   Description: reception fifo c0 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_C0_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_C0_INV_STATE_position     29
#define ND_500_DCR__FATAL_ERR2__RCP_C0_INV_STATE_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__FATAL_ERR2__RCP_C0_INV_STATE_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__FATAL_ERR2__RCP_C0_INV_STATE_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_c1_parity_err
//   Description: reception fifo c1 clk_500 parity error
#define ND_500_DCR__FATAL_ERR2__RCP_C1_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_C1_PARITY_ERR_position     30
#define ND_500_DCR__FATAL_ERR2__RCP_C1_PARITY_ERR_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__FATAL_ERR2__RCP_C1_PARITY_ERR_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__FATAL_ERR2__RCP_C1_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c1_inv_state
//   Description: reception fifo c1 clk_500 invalid state
#define ND_500_DCR__FATAL_ERR2__RCP_C1_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR2__RCP_C1_INV_STATE_position     31
#define ND_500_DCR__FATAL_ERR2__RCP_C1_INV_STATE_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__FATAL_ERR2__RCP_C1_INV_STATE_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__FATAL_ERR2__RCP_C1_INV_STATE_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       reserved
//   Description: reserved, tied to zero
#define ND_500_DCR__FATAL_ERR2__RESERVED_width        32
#define ND_500_DCR__FATAL_ERR2__RESERVED_position     63
#define ND_500_DCR__FATAL_ERR2__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__FATAL_ERR2__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__FATAL_ERR2__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        fatal_err3
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 3
#define ND_500_DCR__FATAL_ERR3_offset  (0x0026) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_overflow
//   Description: reception fifo t0 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T0_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T0_OVERFLOW_position     0
#define ND_500_DCR__FATAL_ERR3__RCP_T0_OVERFLOW_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T0_OVERFLOW_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T0_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       unused_t0
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T0_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T0_position     2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T0_get(x)       _BGQ_GET(2,2,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T0_set(v)       _BGQ_SET(2,2,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T0_insert(x,v)  _BGQ_INSERT(2,2,x,v)

//   Field:       rcp_t1_overflow
//   Description: reception fifo t1 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T1_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T1_OVERFLOW_position     3
#define ND_500_DCR__FATAL_ERR3__RCP_T1_OVERFLOW_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T1_OVERFLOW_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T1_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       unused_t1
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T1_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T1_position     5
#define ND_500_DCR__FATAL_ERR3__UNUSED_T1_get(x)       _BGQ_GET(2,5,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T1_set(v)       _BGQ_SET(2,5,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T1_insert(x,v)  _BGQ_INSERT(2,5,x,v)

//   Field:       rcp_t2_overflow
//   Description: reception fifo t2 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T2_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T2_OVERFLOW_position     6
#define ND_500_DCR__FATAL_ERR3__RCP_T2_OVERFLOW_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T2_OVERFLOW_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T2_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       unused_t2
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T2_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T2_position     8
#define ND_500_DCR__FATAL_ERR3__UNUSED_T2_get(x)       _BGQ_GET(2,8,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T2_set(v)       _BGQ_SET(2,8,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T2_insert(x,v)  _BGQ_INSERT(2,8,x,v)

//   Field:       rcp_t3_overflow
//   Description: reception fifo t3 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T3_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T3_OVERFLOW_position     9
#define ND_500_DCR__FATAL_ERR3__RCP_T3_OVERFLOW_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T3_OVERFLOW_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T3_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       unused_t3
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T3_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T3_position     11
#define ND_500_DCR__FATAL_ERR3__UNUSED_T3_get(x)       _BGQ_GET(2,11,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T3_set(v)       _BGQ_SET(2,11,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T3_insert(x,v)  _BGQ_INSERT(2,11,x,v)

//   Field:       rcp_t4_overflow
//   Description: reception fifo t4 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T4_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T4_OVERFLOW_position     12
#define ND_500_DCR__FATAL_ERR3__RCP_T4_OVERFLOW_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T4_OVERFLOW_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T4_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       unused_t4
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T4_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T4_position     14
#define ND_500_DCR__FATAL_ERR3__UNUSED_T4_get(x)       _BGQ_GET(2,14,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T4_set(v)       _BGQ_SET(2,14,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T4_insert(x,v)  _BGQ_INSERT(2,14,x,v)

//   Field:       rcp_t5_overflow
//   Description: reception fifo t5 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T5_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T5_OVERFLOW_position     15
#define ND_500_DCR__FATAL_ERR3__RCP_T5_OVERFLOW_get(x)       _BGQ_GET(1,15,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T5_OVERFLOW_set(v)       _BGQ_SET(1,15,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T5_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       unused_t5
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T5_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T5_position     17
#define ND_500_DCR__FATAL_ERR3__UNUSED_T5_get(x)       _BGQ_GET(2,17,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T5_set(v)       _BGQ_SET(2,17,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T5_insert(x,v)  _BGQ_INSERT(2,17,x,v)

//   Field:       rcp_t6_overflow
//   Description: reception fifo t6 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T6_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T6_OVERFLOW_position     18
#define ND_500_DCR__FATAL_ERR3__RCP_T6_OVERFLOW_get(x)       _BGQ_GET(1,18,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T6_OVERFLOW_set(v)       _BGQ_SET(1,18,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T6_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       unused_t6
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T6_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T6_position     20
#define ND_500_DCR__FATAL_ERR3__UNUSED_T6_get(x)       _BGQ_GET(2,20,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T6_set(v)       _BGQ_SET(2,20,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T6_insert(x,v)  _BGQ_INSERT(2,20,x,v)

//   Field:       rcp_t7_overflow
//   Description: reception fifo t7 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T7_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T7_OVERFLOW_position     21
#define ND_500_DCR__FATAL_ERR3__RCP_T7_OVERFLOW_get(x)       _BGQ_GET(1,21,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T7_OVERFLOW_set(v)       _BGQ_SET(1,21,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T7_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       unused_t7
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T7_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T7_position     23
#define ND_500_DCR__FATAL_ERR3__UNUSED_T7_get(x)       _BGQ_GET(2,23,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T7_set(v)       _BGQ_SET(2,23,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T7_insert(x,v)  _BGQ_INSERT(2,23,x,v)

//   Field:       rcp_t8_overflow
//   Description: reception fifo t8 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T8_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T8_OVERFLOW_position     24
#define ND_500_DCR__FATAL_ERR3__RCP_T8_OVERFLOW_get(x)       _BGQ_GET(1,24,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T8_OVERFLOW_set(v)       _BGQ_SET(1,24,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T8_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       unused_t8
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T8_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T8_position     26
#define ND_500_DCR__FATAL_ERR3__UNUSED_T8_get(x)       _BGQ_GET(2,26,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T8_set(v)       _BGQ_SET(2,26,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T8_insert(x,v)  _BGQ_INSERT(2,26,x,v)

//   Field:       rcp_t9_overflow
//   Description: reception fifo t9 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_T9_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_T9_OVERFLOW_position     27
#define ND_500_DCR__FATAL_ERR3__RCP_T9_OVERFLOW_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__FATAL_ERR3__RCP_T9_OVERFLOW_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__FATAL_ERR3__RCP_T9_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       unused_t9
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_T9_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_T9_position     29
#define ND_500_DCR__FATAL_ERR3__UNUSED_T9_get(x)       _BGQ_GET(2,29,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T9_set(v)       _BGQ_SET(2,29,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_T9_insert(x,v)  _BGQ_INSERT(2,29,x,v)

//   Field:       rcp_l0_overflow
//   Description: reception fifo l0 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_L0_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_L0_OVERFLOW_position     30
#define ND_500_DCR__FATAL_ERR3__RCP_L0_OVERFLOW_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__FATAL_ERR3__RCP_L0_OVERFLOW_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__FATAL_ERR3__RCP_L0_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       unused_l0
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_L0_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_L0_position     32
#define ND_500_DCR__FATAL_ERR3__UNUSED_L0_get(x)       _BGQ_GET(2,32,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_L0_set(v)       _BGQ_SET(2,32,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_L0_insert(x,v)  _BGQ_INSERT(2,32,x,v)

//   Field:       rcp_l1_overflow
//   Description: reception fifo l0 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_L1_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_L1_OVERFLOW_position     33
#define ND_500_DCR__FATAL_ERR3__RCP_L1_OVERFLOW_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__FATAL_ERR3__RCP_L1_OVERFLOW_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__FATAL_ERR3__RCP_L1_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       unused_l1
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_L1_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_L1_position     35
#define ND_500_DCR__FATAL_ERR3__UNUSED_L1_get(x)       _BGQ_GET(2,35,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_L1_set(v)       _BGQ_SET(2,35,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_L1_insert(x,v)  _BGQ_INSERT(2,35,x,v)

//   Field:       rcp_hp_overflow
//   Description: reception fifo hp overflow
#define ND_500_DCR__FATAL_ERR3__RCP_HP_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_HP_OVERFLOW_position     36
#define ND_500_DCR__FATAL_ERR3__RCP_HP_OVERFLOW_get(x)       _BGQ_GET(1,36,x)
#define ND_500_DCR__FATAL_ERR3__RCP_HP_OVERFLOW_set(v)       _BGQ_SET(1,36,v)
#define ND_500_DCR__FATAL_ERR3__RCP_HP_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       unused_hp
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_HP_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_HP_position     38
#define ND_500_DCR__FATAL_ERR3__UNUSED_HP_get(x)       _BGQ_GET(2,38,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_HP_set(v)       _BGQ_SET(2,38,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_HP_insert(x,v)  _BGQ_INSERT(2,38,x,v)

//   Field:       rcp_io_overflow
//   Description: reception fifo io overflow
#define ND_500_DCR__FATAL_ERR3__RCP_IO_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_IO_OVERFLOW_position     39
#define ND_500_DCR__FATAL_ERR3__RCP_IO_OVERFLOW_get(x)       _BGQ_GET(1,39,x)
#define ND_500_DCR__FATAL_ERR3__RCP_IO_OVERFLOW_set(v)       _BGQ_SET(1,39,v)
#define ND_500_DCR__FATAL_ERR3__RCP_IO_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       unused_io
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_IO_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_IO_position     41
#define ND_500_DCR__FATAL_ERR3__UNUSED_IO_get(x)       _BGQ_GET(2,41,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_IO_set(v)       _BGQ_SET(2,41,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_IO_insert(x,v)  _BGQ_INSERT(2,41,x,v)

//   Field:       rcp_c0_overflow
//   Description: reception fifo c0 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_C0_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_C0_OVERFLOW_position     42
#define ND_500_DCR__FATAL_ERR3__RCP_C0_OVERFLOW_get(x)       _BGQ_GET(1,42,x)
#define ND_500_DCR__FATAL_ERR3__RCP_C0_OVERFLOW_set(v)       _BGQ_SET(1,42,v)
#define ND_500_DCR__FATAL_ERR3__RCP_C0_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       unused_c0
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_C0_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_C0_position     44
#define ND_500_DCR__FATAL_ERR3__UNUSED_C0_get(x)       _BGQ_GET(2,44,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_C0_set(v)       _BGQ_SET(2,44,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_C0_insert(x,v)  _BGQ_INSERT(2,44,x,v)

//   Field:       rcp_c0_ue
//   Description: reception fifo c0 uncorrectable ecc error
#define ND_500_DCR__FATAL_ERR3__RCP_C0_UE_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_C0_UE_position     45
#define ND_500_DCR__FATAL_ERR3__RCP_C0_UE_get(x)       _BGQ_GET(1,45,x)
#define ND_500_DCR__FATAL_ERR3__RCP_C0_UE_set(v)       _BGQ_SET(1,45,v)
#define ND_500_DCR__FATAL_ERR3__RCP_C0_UE_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       rcp_c1_overflow
//   Description: reception fifo c1 overflow
#define ND_500_DCR__FATAL_ERR3__RCP_C1_OVERFLOW_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_C1_OVERFLOW_position     46
#define ND_500_DCR__FATAL_ERR3__RCP_C1_OVERFLOW_get(x)       _BGQ_GET(1,46,x)
#define ND_500_DCR__FATAL_ERR3__RCP_C1_OVERFLOW_set(v)       _BGQ_SET(1,46,v)
#define ND_500_DCR__FATAL_ERR3__RCP_C1_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       unused_c1
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__UNUSED_C1_width        2
#define ND_500_DCR__FATAL_ERR3__UNUSED_C1_position     48
#define ND_500_DCR__FATAL_ERR3__UNUSED_C1_get(x)       _BGQ_GET(2,48,x)
#define ND_500_DCR__FATAL_ERR3__UNUSED_C1_set(v)       _BGQ_SET(2,48,v)
#define ND_500_DCR__FATAL_ERR3__UNUSED_C1_insert(x,v)  _BGQ_INSERT(2,48,x,v)

//   Field:       rcp_c1_ue
//   Description: reception fifo c1 uncorrectable ecc error
#define ND_500_DCR__FATAL_ERR3__RCP_C1_UE_width        1
#define ND_500_DCR__FATAL_ERR3__RCP_C1_UE_position     49
#define ND_500_DCR__FATAL_ERR3__RCP_C1_UE_get(x)       _BGQ_GET(1,49,x)
#define ND_500_DCR__FATAL_ERR3__RCP_C1_UE_set(v)       _BGQ_SET(1,49,v)
#define ND_500_DCR__FATAL_ERR3__RCP_C1_UE_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR3__RESERVED_width        14
#define ND_500_DCR__FATAL_ERR3__RESERVED_position     63
#define ND_500_DCR__FATAL_ERR3__RESERVED_get(x)       _BGQ_GET(14,63,x)
#define ND_500_DCR__FATAL_ERR3__RESERVED_set(v)       _BGQ_SET(14,63,v)
#define ND_500_DCR__FATAL_ERR3__RESERVED_insert(x,v)  _BGQ_INSERT(14,63,x,v)

  
// Register:
//   Name:        fatal_err4
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 4
#define ND_500_DCR__FATAL_ERR4_offset  (0x0027) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       coll_alu_parity_err
//   Description: collective logic alu tree parity error
#define ND_500_DCR__FATAL_ERR4__COLL_ALU_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_ALU_PARITY_ERR_position     0
#define ND_500_DCR__FATAL_ERR4__COLL_ALU_PARITY_ERR_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR4__COLL_ALU_PARITY_ERR_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR4__COLL_ALU_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       coll_fp_front_parity_err
//   Description: collective floating point front end logic parity error
#define ND_500_DCR__FATAL_ERR4__COLL_FP_FRONT_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_FP_FRONT_PARITY_ERR_position     1
#define ND_500_DCR__FATAL_ERR4__COLL_FP_FRONT_PARITY_ERR_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__FATAL_ERR4__COLL_FP_FRONT_PARITY_ERR_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__FATAL_ERR4__COLL_FP_FRONT_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       coll_fp_back_parity_err
//   Description: collective floating point back end logic parity error
#define ND_500_DCR__FATAL_ERR4__COLL_FP_BACK_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_FP_BACK_PARITY_ERR_position     2
#define ND_500_DCR__FATAL_ERR4__COLL_FP_BACK_PARITY_ERR_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__FATAL_ERR4__COLL_FP_BACK_PARITY_ERR_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__FATAL_ERR4__COLL_FP_BACK_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       coll_uptree_parity_err
//   Description: collective up tree arbiters parity error
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_PARITY_ERR_position     3
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_PARITY_ERR_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_PARITY_ERR_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       coll_downtree_parity_err
//   Description: collective down tree arbiters parity error
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_PARITY_ERR_position     4
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_PARITY_ERR_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_PARITY_ERR_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       coll_top_parity_err
//   Description: collective top level logic parity error
#define ND_500_DCR__FATAL_ERR4__COLL_TOP_PARITY_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_TOP_PARITY_ERR_position     5
#define ND_500_DCR__FATAL_ERR4__COLL_TOP_PARITY_ERR_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__FATAL_ERR4__COLL_TOP_PARITY_ERR_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__FATAL_ERR4__COLL_TOP_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       coll_uptree_inv_state
//   Description: collective up tree fsm invalid state
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_INV_STATE_position     6
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_INV_STATE_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_INV_STATE_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR4__COLL_UPTREE_INV_STATE_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       coll_downtree_inv_state
//   Description: collective down tree fsm invalid state
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_INV_STATE_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_INV_STATE_position     7
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_INV_STATE_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_INV_STATE_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__FATAL_ERR4__COLL_DOWNTREE_INV_STATE_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       coll_dbus_err
//   Description: collective any input data bus feeding both up and down tree logic in the same cycle
#define ND_500_DCR__FATAL_ERR4__COLL_DBUS_ERR_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_DBUS_ERR_position     8
#define ND_500_DCR__FATAL_ERR4__COLL_DBUS_ERR_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__FATAL_ERR4__COLL_DBUS_ERR_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__FATAL_ERR4__COLL_DBUS_ERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       coll_ce_or_ue
//   Description: collective logic correctable or uncorrectable ecc error
#define ND_500_DCR__FATAL_ERR4__COLL_CE_OR_UE_width        1
#define ND_500_DCR__FATAL_ERR4__COLL_CE_OR_UE_position     9
#define ND_500_DCR__FATAL_ERR4__COLL_CE_OR_UE_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR4__COLL_CE_OR_UE_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR4__COLL_CE_OR_UE_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR4__RESERVED_width        54
#define ND_500_DCR__FATAL_ERR4__RESERVED_position     63
#define ND_500_DCR__FATAL_ERR4__RESERVED_get(x)       _BGQ_GET(54,63,x)
#define ND_500_DCR__FATAL_ERR4__RESERVED_set(v)       _BGQ_SET(54,63,v)
#define ND_500_DCR__FATAL_ERR4__RESERVED_insert(x,v)  _BGQ_INSERT(54,63,x,v)

  
// Register:
//   Name:        fatal_err5
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal error 5
#define ND_500_DCR__FATAL_ERR5_offset  (0x0028) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_fifo_a_ce_over_thresh
//   Description: injection fifo group A: t0, t1, t2, t3, correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_A_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_A_CE_OVER_THRESH_position     0
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_A_CE_OVER_THRESH_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_A_CE_OVER_THRESH_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_A_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_fifo_b_ce_over_thresh
//   Description: injection fifo group B: t4, t5, t6, t7, correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_B_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_B_CE_OVER_THRESH_position     1
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_B_CE_OVER_THRESH_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_B_CE_OVER_THRESH_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_B_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_fifo_c_ce_over_thresh
//   Description: injection fifo group C: t8, t9, l0, l1, correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_C_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_C_CE_OVER_THRESH_position     2
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_C_CE_OVER_THRESH_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_C_CE_OVER_THRESH_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_C_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_fifo_d_ce_over_thresh
//   Description: injection fifo group D: hp, io, c0, c1, correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_D_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_D_CE_OVER_THRESH_position     3
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_D_CE_OVER_THRESH_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_D_CE_OVER_THRESH_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__FATAL_ERR5__INJ_FIFO_D_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       rcp_fifo_c0_ce_over_thresh
//   Description: reception fifo c0 correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C0_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C0_CE_OVER_THRESH_position     4
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C0_CE_OVER_THRESH_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C0_CE_OVER_THRESH_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C0_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       rcp_fifo_c1_ce_over_thresh
//   Description: reception fifo c1 correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C1_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C1_CE_OVER_THRESH_position     5
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C1_CE_OVER_THRESH_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C1_CE_OVER_THRESH_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__FATAL_ERR5__RCP_FIFO_C1_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       coll_downtree_ce_over_thresh
//   Description: collective down tree logic correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__COLL_DOWNTREE_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__COLL_DOWNTREE_CE_OVER_THRESH_position     6
#define ND_500_DCR__FATAL_ERR5__COLL_DOWNTREE_CE_OVER_THRESH_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__FATAL_ERR5__COLL_DOWNTREE_CE_OVER_THRESH_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__FATAL_ERR5__COLL_DOWNTREE_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       unused
//   Description: reserved, tied to zero
#define ND_500_DCR__FATAL_ERR5__UNUSED_width        1
#define ND_500_DCR__FATAL_ERR5__UNUSED_position     7
#define ND_500_DCR__FATAL_ERR5__UNUSED_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__FATAL_ERR5__UNUSED_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__FATAL_ERR5__UNUSED_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       gi_ce_over_thresh
//   Description: global barrier logic correctable ecc error count over threshold
#define ND_500_DCR__FATAL_ERR5__GI_CE_OVER_THRESH_width        1
#define ND_500_DCR__FATAL_ERR5__GI_CE_OVER_THRESH_position     8
#define ND_500_DCR__FATAL_ERR5__GI_CE_OVER_THRESH_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__FATAL_ERR5__GI_CE_OVER_THRESH_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__FATAL_ERR5__GI_CE_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       gi_ue
//   Description: global barrier logic uncorrectable ecc error
#define ND_500_DCR__FATAL_ERR5__GI_UE_width        1
#define ND_500_DCR__FATAL_ERR5__GI_UE_position     9
#define ND_500_DCR__FATAL_ERR5__GI_UE_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__FATAL_ERR5__GI_UE_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__FATAL_ERR5__GI_UE_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       dcr_slave_mchk
//   Description: dcr slave machine check interrupt
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_MCHK_width        1
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_MCHK_position     10
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_MCHK_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_MCHK_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_MCHK_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       dcr_slave_crit
//   Description: dcr slave crital interrupt
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_CRIT_width        1
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_CRIT_position     11
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_CRIT_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_CRIT_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__FATAL_ERR5__DCR_SLAVE_CRIT_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_500_DCR__FATAL_ERR5__RESERVED_width        52
#define ND_500_DCR__FATAL_ERR5__RESERVED_position     63
#define ND_500_DCR__FATAL_ERR5__RESERVED_get(x)       _BGQ_GET(52,63,x)
#define ND_500_DCR__FATAL_ERR5__RESERVED_set(v)       _BGQ_SET(52,63,v)
#define ND_500_DCR__FATAL_ERR5__RESERVED_insert(x,v)  _BGQ_INSERT(52,63,x,v)

  
// Register:
//   Name:        non_fatal_err0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: non fatal error 0
#define ND_500_DCR__NON_FATAL_ERR0_offset  (0x0029) // Offset of register in instance 
#define ND_500_DCR__NON_FATAL_ERR0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_hdrchk_err
//   Description: injection fifo t0 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T0_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T0_HDRCHK_ERR_position     0
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T0_HDRCHK_ERR_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T0_HDRCHK_ERR_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T0_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       inj_t1_hdrchk_err
//   Description: injection fifo t1 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T1_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T1_HDRCHK_ERR_position     1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T1_HDRCHK_ERR_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T1_HDRCHK_ERR_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T1_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       inj_t2_hdrchk_err
//   Description: injection fifo t2 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T2_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T2_HDRCHK_ERR_position     2
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T2_HDRCHK_ERR_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T2_HDRCHK_ERR_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T2_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       inj_t3_hdrchk_err
//   Description: injection fifo t3 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T3_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T3_HDRCHK_ERR_position     3
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T3_HDRCHK_ERR_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T3_HDRCHK_ERR_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T3_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       inj_t4_hdrchk_err
//   Description: injection fifo t4 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T4_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T4_HDRCHK_ERR_position     4
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T4_HDRCHK_ERR_get(x)       _BGQ_GET(1,4,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T4_HDRCHK_ERR_set(v)       _BGQ_SET(1,4,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T4_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       inj_t5_hdrchk_err
//   Description: injection fifo t5 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T5_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T5_HDRCHK_ERR_position     5
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T5_HDRCHK_ERR_get(x)       _BGQ_GET(1,5,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T5_HDRCHK_ERR_set(v)       _BGQ_SET(1,5,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T5_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       inj_t6_hdrchk_err
//   Description: injection fifo t6 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T6_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T6_HDRCHK_ERR_position     6
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T6_HDRCHK_ERR_get(x)       _BGQ_GET(1,6,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T6_HDRCHK_ERR_set(v)       _BGQ_SET(1,6,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T6_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       inj_t7_hdrchk_err
//   Description: injection fifo t7 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T7_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T7_HDRCHK_ERR_position     7
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T7_HDRCHK_ERR_get(x)       _BGQ_GET(1,7,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T7_HDRCHK_ERR_set(v)       _BGQ_SET(1,7,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T7_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       inj_t8_hdrchk_err
//   Description: injection fifo t8 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T8_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T8_HDRCHK_ERR_position     8
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T8_HDRCHK_ERR_get(x)       _BGQ_GET(1,8,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T8_HDRCHK_ERR_set(v)       _BGQ_SET(1,8,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T8_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       inj_t9_hdrchk_err
//   Description: injection fifo t9 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T9_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T9_HDRCHK_ERR_position     9
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T9_HDRCHK_ERR_get(x)       _BGQ_GET(1,9,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T9_HDRCHK_ERR_set(v)       _BGQ_SET(1,9,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_T9_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       inj_hp_hdrchk_err
//   Description: injection fifo hp header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_HP_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_HP_HDRCHK_ERR_position     10
#define ND_500_DCR__NON_FATAL_ERR0__INJ_HP_HDRCHK_ERR_get(x)       _BGQ_GET(1,10,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_HP_HDRCHK_ERR_set(v)       _BGQ_SET(1,10,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_HP_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       inj_io_hdrchk_err
//   Description: injection fifo io header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_IO_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_IO_HDRCHK_ERR_position     11
#define ND_500_DCR__NON_FATAL_ERR0__INJ_IO_HDRCHK_ERR_get(x)       _BGQ_GET(1,11,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_IO_HDRCHK_ERR_set(v)       _BGQ_SET(1,11,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_IO_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       inj_c0_hdrchk_err
//   Description: injection fifo c0 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C0_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C0_HDRCHK_ERR_position     12
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C0_HDRCHK_ERR_get(x)       _BGQ_GET(1,12,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C0_HDRCHK_ERR_set(v)       _BGQ_SET(1,12,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C0_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       inj_c1_hdrchk_err
//   Description: injection fifo c1 header checker error
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C1_HDRCHK_ERR_width        1
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C1_HDRCHK_ERR_position     13
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C1_HDRCHK_ERR_get(x)       _BGQ_GET(1,13,x)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C1_HDRCHK_ERR_set(v)       _BGQ_SET(1,13,v)
#define ND_500_DCR__NON_FATAL_ERR0__INJ_C1_HDRCHK_ERR_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       unused0
//   Description: reserved, tied to 0
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED0_width        11
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED0_position     24
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED0_get(x)       _BGQ_GET(11,24,x)
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED0_set(v)       _BGQ_SET(11,24,v)
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED0_insert(x,v)  _BGQ_INSERT(11,24,x,v)

//   Field:       rcp_c0_overflow_set
//   Description: reception fifo c0 integer overflow interrupt bit set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OVERFLOW_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OVERFLOW_SET_position     25
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OVERFLOW_SET_get(x)       _BGQ_GET(1,25,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OVERFLOW_SET_set(v)       _BGQ_SET(1,25,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OVERFLOW_SET_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rcp_c0_nan_set
//   Description: reception fifo c0 floating point NaN interrupt bit set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_NAN_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_NAN_SET_position     26
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_NAN_SET_get(x)       _BGQ_GET(1,26,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_NAN_SET_set(v)       _BGQ_SET(1,26,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_NAN_SET_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rcp_c0_other_set
//   Description: reception fifo c0 other interrupt bit (header mismatch, etc) set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OTHER_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OTHER_SET_position     27
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OTHER_SET_get(x)       _BGQ_GET(1,27,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OTHER_SET_set(v)       _BGQ_SET(1,27,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C0_OTHER_SET_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       unused1
//   Description: reserved, tied to 0
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED1_width        1
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED1_position     28
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED1_get(x)       _BGQ_GET(1,28,x)
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED1_set(v)       _BGQ_SET(1,28,v)
#define ND_500_DCR__NON_FATAL_ERR0__UNUSED1_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rcp_c1_overflow_set
//   Description: reception fifo c1 integer overflow interrupt bit set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OVERFLOW_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OVERFLOW_SET_position     29
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OVERFLOW_SET_get(x)       _BGQ_GET(1,29,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OVERFLOW_SET_set(v)       _BGQ_SET(1,29,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OVERFLOW_SET_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       rcp_c1_nan_set
//   Description: reception fifo c1 floating point NaN interrupt bit set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_NAN_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_NAN_SET_position     30
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_NAN_SET_get(x)       _BGQ_GET(1,30,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_NAN_SET_set(v)       _BGQ_SET(1,30,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_NAN_SET_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       rcp_c1_other_set
//   Description: reception fifo c1 other interrupt bit (header mismatch, etc) set
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OTHER_SET_width        1
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OTHER_SET_position     31
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OTHER_SET_get(x)       _BGQ_GET(1,31,x)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OTHER_SET_set(v)       _BGQ_SET(1,31,v)
#define ND_500_DCR__NON_FATAL_ERR0__RCP_C1_OTHER_SET_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       coll_downtree_grant_timeout
//   Description: collective down tree logic grant timed out
#define ND_500_DCR__NON_FATAL_ERR0__COLL_DOWNTREE_GRANT_TIMEOUT_width        1
#define ND_500_DCR__NON_FATAL_ERR0__COLL_DOWNTREE_GRANT_TIMEOUT_position     32
#define ND_500_DCR__NON_FATAL_ERR0__COLL_DOWNTREE_GRANT_TIMEOUT_get(x)       _BGQ_GET(1,32,x)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_DOWNTREE_GRANT_TIMEOUT_set(v)       _BGQ_SET(1,32,v)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_DOWNTREE_GRANT_TIMEOUT_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       coll_nan_generated
//   Description: collective floating point NaN generated in this node
#define ND_500_DCR__NON_FATAL_ERR0__COLL_NAN_GENERATED_width        1
#define ND_500_DCR__NON_FATAL_ERR0__COLL_NAN_GENERATED_position     33
#define ND_500_DCR__NON_FATAL_ERR0__COLL_NAN_GENERATED_get(x)       _BGQ_GET(1,33,x)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_NAN_GENERATED_set(v)       _BGQ_SET(1,33,v)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_NAN_GENERATED_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       coll_overflow
//   Description: collective interger overflow in this node
#define ND_500_DCR__NON_FATAL_ERR0__COLL_OVERFLOW_width        1
#define ND_500_DCR__NON_FATAL_ERR0__COLL_OVERFLOW_position     34
#define ND_500_DCR__NON_FATAL_ERR0__COLL_OVERFLOW_get(x)       _BGQ_GET(1,34,x)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_OVERFLOW_set(v)       _BGQ_SET(1,34,v)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       coll_header_mismatch
//   Description: collective non-fatal header mismatch
#define ND_500_DCR__NON_FATAL_ERR0__COLL_HEADER_MISMATCH_width        1
#define ND_500_DCR__NON_FATAL_ERR0__COLL_HEADER_MISMATCH_position     35
#define ND_500_DCR__NON_FATAL_ERR0__COLL_HEADER_MISMATCH_get(x)       _BGQ_GET(1,35,x)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_HEADER_MISMATCH_set(v)       _BGQ_SET(1,35,v)
#define ND_500_DCR__NON_FATAL_ERR0__COLL_HEADER_MISMATCH_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_500_DCR__NON_FATAL_ERR0__RESERVED_width        28
#define ND_500_DCR__NON_FATAL_ERR0__RESERVED_position     63
#define ND_500_DCR__NON_FATAL_ERR0__RESERVED_get(x)       _BGQ_GET(28,63,x)
#define ND_500_DCR__NON_FATAL_ERR0__RESERVED_set(v)       _BGQ_SET(28,63,v)
#define ND_500_DCR__NON_FATAL_ERR0__RESERVED_insert(x,v)  _BGQ_INSERT(28,63,x,v)

  
// Register:
//   Name:        non_fatal_err1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: non fatal error 1
#define ND_500_DCR__NON_FATAL_ERR1_offset  (0x002A) // Offset of register in instance 
#define ND_500_DCR__NON_FATAL_ERR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       dcr_slave_non_crit
//   Description: dcr slave non critical interrupt
#define ND_500_DCR__NON_FATAL_ERR1__DCR_SLAVE_NON_CRIT_width        1
#define ND_500_DCR__NON_FATAL_ERR1__DCR_SLAVE_NON_CRIT_position     0
#define ND_500_DCR__NON_FATAL_ERR1__DCR_SLAVE_NON_CRIT_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__NON_FATAL_ERR1__DCR_SLAVE_NON_CRIT_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__NON_FATAL_ERR1__DCR_SLAVE_NON_CRIT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       reserved
//   Description: reserved, tied to 0
#define ND_500_DCR__NON_FATAL_ERR1__RESERVED_width        63
#define ND_500_DCR__NON_FATAL_ERR1__RESERVED_position     63
#define ND_500_DCR__NON_FATAL_ERR1__RESERVED_get(x)       _BGQ_GET(63,63,x)
#define ND_500_DCR__NON_FATAL_ERR1__RESERVED_set(v)       _BGQ_SET(63,63,v)
#define ND_500_DCR__NON_FATAL_ERR1__RESERVED_insert(x,v)  _BGQ_INSERT(63,63,x,v)

  
// Register:
//   Name:        backdoor_ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Array backdoor control
#define ND_500_DCR__BACKDOOR_CTRL_offset  (0x002B) // Offset of register in instance 
#define ND_500_DCR__BACKDOOR_CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       en
//   Description: backdoor access enable, set to 11 to enable backdoor
#define ND_500_DCR__BACKDOOR_CTRL__EN_width        2
#define ND_500_DCR__BACKDOOR_CTRL__EN_position     1
#define ND_500_DCR__BACKDOOR_CTRL__EN_get(x)       _BGQ_GET(2,1,x)
#define ND_500_DCR__BACKDOOR_CTRL__EN_set(v)       _BGQ_SET(2,1,v)
#define ND_500_DCR__BACKDOOR_CTRL__EN_insert(x,v)  _BGQ_INSERT(2,1,x,v)

//   Field:       write
//   Description: backdoor access write
#define ND_500_DCR__BACKDOOR_CTRL__WRITE_width        1
#define ND_500_DCR__BACKDOOR_CTRL__WRITE_position     2
#define ND_500_DCR__BACKDOOR_CTRL__WRITE_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__BACKDOOR_CTRL__WRITE_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__BACKDOOR_CTRL__WRITE_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       read
//   Description: backdoor access read
#define ND_500_DCR__BACKDOOR_CTRL__READ_width        1
#define ND_500_DCR__BACKDOOR_CTRL__READ_position     3
#define ND_500_DCR__BACKDOOR_CTRL__READ_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__BACKDOOR_CTRL__READ_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__BACKDOOR_CTRL__READ_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       array_sel
//   Description: backdoor access array sel, 00000 to 00101 for injection fifo write, 10000 to 11111 for reception fifo read
#define ND_500_DCR__BACKDOOR_CTRL__ARRAY_SEL_width        5
#define ND_500_DCR__BACKDOOR_CTRL__ARRAY_SEL_position     11
#define ND_500_DCR__BACKDOOR_CTRL__ARRAY_SEL_get(x)       _BGQ_GET(5,11,x)
#define ND_500_DCR__BACKDOOR_CTRL__ARRAY_SEL_set(v)       _BGQ_SET(5,11,v)
#define ND_500_DCR__BACKDOOR_CTRL__ARRAY_SEL_insert(x,v)  _BGQ_INSERT(5,11,x,v)

//   Field:       address
//   Description: backdoor access array address
#define ND_500_DCR__BACKDOOR_CTRL__ADDRESS_width        10
#define ND_500_DCR__BACKDOOR_CTRL__ADDRESS_position     23
#define ND_500_DCR__BACKDOOR_CTRL__ADDRESS_get(x)       _BGQ_GET(10,23,x)
#define ND_500_DCR__BACKDOOR_CTRL__ADDRESS_set(v)       _BGQ_SET(10,23,v)
#define ND_500_DCR__BACKDOOR_CTRL__ADDRESS_insert(x,v)  _BGQ_INSERT(10,23,x,v)

  
// Register:
//   Name:        backdoor_wr
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor write
#define ND_500_DCR__BACKDOOR_WR_offset  (0x002C) // Offset of register in instance 
#define ND_500_DCR__BACKDOOR_WR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ecc
//   Description: ecc
#define ND_500_DCR__BACKDOOR_WR__ECC_width        7
#define ND_500_DCR__BACKDOOR_WR__ECC_position     31
#define ND_500_DCR__BACKDOOR_WR__ECC_get(x)       _BGQ_GET(7,31,x)
#define ND_500_DCR__BACKDOOR_WR__ECC_set(v)       _BGQ_SET(7,31,v)
#define ND_500_DCR__BACKDOOR_WR__ECC_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       data
//   Description: data
#define ND_500_DCR__BACKDOOR_WR__DATA_width        32
#define ND_500_DCR__BACKDOOR_WR__DATA_position     63
#define ND_500_DCR__BACKDOOR_WR__DATA_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__BACKDOOR_WR__DATA_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__BACKDOOR_WR__DATA_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        backdoor_rd_data
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: backdoor read data n
#define ND_500_DCR__BACKDOOR_RD_DATA_offset  (0x002E) // Offset of register in instance 
#define ND_500_DCR__BACKDOOR_RD_DATA_range  (0x2) // Range of external address space
#define ND_500_DCR__BACKDOOR_RD_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor read data 0
#define ND_500_DCR__BACKDOOR_RD_DATA__VALUE_width        64
#define ND_500_DCR__BACKDOOR_RD_DATA__VALUE_position     63
#define ND_500_DCR__BACKDOOR_RD_DATA__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__BACKDOOR_RD_DATA__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__BACKDOOR_RD_DATA__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_rd_ecc
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: backdoor read ecc
#define ND_500_DCR__BACKDOOR_RD_ECC_offset  (0x0030) // Offset of register in instance 
#define ND_500_DCR__BACKDOOR_RD_ECC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor read ecc
#define ND_500_DCR__BACKDOOR_RD_ECC__VALUE_width        16
#define ND_500_DCR__BACKDOOR_RD_ECC__VALUE_position     63
#define ND_500_DCR__BACKDOOR_RD_ECC__VALUE_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__BACKDOOR_RD_ECC__VALUE_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__BACKDOOR_RD_ECC__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 0
#define ND_500_DCR__STAT0_offset  (0x0031) // Offset of register in instance 
#define ND_500_DCR__STAT0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_arb_state
//   Description: injection fifo T0 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T0_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T0_ARB_STATE_position     3
#define ND_500_DCR__STAT0__INJ_T0_ARB_STATE_get(x)       _BGQ_GET(4,3,x)
#define ND_500_DCR__STAT0__INJ_T0_ARB_STATE_set(v)       _BGQ_SET(4,3,v)
#define ND_500_DCR__STAT0__INJ_T0_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,3,x,v)

//   Field:       inj_t1_arb_state
//   Description: injection fifo T1 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T1_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T1_ARB_STATE_position     7
#define ND_500_DCR__STAT0__INJ_T1_ARB_STATE_get(x)       _BGQ_GET(4,7,x)
#define ND_500_DCR__STAT0__INJ_T1_ARB_STATE_set(v)       _BGQ_SET(4,7,v)
#define ND_500_DCR__STAT0__INJ_T1_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,7,x,v)

//   Field:       inj_t2_arb_state
//   Description: injection fifo T2 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T2_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T2_ARB_STATE_position     11
#define ND_500_DCR__STAT0__INJ_T2_ARB_STATE_get(x)       _BGQ_GET(4,11,x)
#define ND_500_DCR__STAT0__INJ_T2_ARB_STATE_set(v)       _BGQ_SET(4,11,v)
#define ND_500_DCR__STAT0__INJ_T2_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       inj_t3_arb_state
//   Description: injection fifo T3 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T3_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T3_ARB_STATE_position     15
#define ND_500_DCR__STAT0__INJ_T3_ARB_STATE_get(x)       _BGQ_GET(4,15,x)
#define ND_500_DCR__STAT0__INJ_T3_ARB_STATE_set(v)       _BGQ_SET(4,15,v)
#define ND_500_DCR__STAT0__INJ_T3_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       inj_t4_arb_state
//   Description: injection fifo T4 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T4_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T4_ARB_STATE_position     19
#define ND_500_DCR__STAT0__INJ_T4_ARB_STATE_get(x)       _BGQ_GET(4,19,x)
#define ND_500_DCR__STAT0__INJ_T4_ARB_STATE_set(v)       _BGQ_SET(4,19,v)
#define ND_500_DCR__STAT0__INJ_T4_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,19,x,v)

//   Field:       inj_t5_arb_state
//   Description: injection fifo T5 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T5_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T5_ARB_STATE_position     23
#define ND_500_DCR__STAT0__INJ_T5_ARB_STATE_get(x)       _BGQ_GET(4,23,x)
#define ND_500_DCR__STAT0__INJ_T5_ARB_STATE_set(v)       _BGQ_SET(4,23,v)
#define ND_500_DCR__STAT0__INJ_T5_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,23,x,v)

//   Field:       inj_t6_arb_state
//   Description: injection fifo T6 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T6_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T6_ARB_STATE_position     27
#define ND_500_DCR__STAT0__INJ_T6_ARB_STATE_get(x)       _BGQ_GET(4,27,x)
#define ND_500_DCR__STAT0__INJ_T6_ARB_STATE_set(v)       _BGQ_SET(4,27,v)
#define ND_500_DCR__STAT0__INJ_T6_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,27,x,v)

//   Field:       inj_t7_arb_state
//   Description: injection fifo T7 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T7_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T7_ARB_STATE_position     31
#define ND_500_DCR__STAT0__INJ_T7_ARB_STATE_get(x)       _BGQ_GET(4,31,x)
#define ND_500_DCR__STAT0__INJ_T7_ARB_STATE_set(v)       _BGQ_SET(4,31,v)
#define ND_500_DCR__STAT0__INJ_T7_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,31,x,v)

//   Field:       inj_t8_arb_state
//   Description: injection fifo T8 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T8_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T8_ARB_STATE_position     35
#define ND_500_DCR__STAT0__INJ_T8_ARB_STATE_get(x)       _BGQ_GET(4,35,x)
#define ND_500_DCR__STAT0__INJ_T8_ARB_STATE_set(v)       _BGQ_SET(4,35,v)
#define ND_500_DCR__STAT0__INJ_T8_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,35,x,v)

//   Field:       inj_t9_arb_state
//   Description: injection fifo T9 arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_T9_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_T9_ARB_STATE_position     39
#define ND_500_DCR__STAT0__INJ_T9_ARB_STATE_get(x)       _BGQ_GET(4,39,x)
#define ND_500_DCR__STAT0__INJ_T9_ARB_STATE_set(v)       _BGQ_SET(4,39,v)
#define ND_500_DCR__STAT0__INJ_T9_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,39,x,v)

//   Field:       inj_hp_arb_state
//   Description: injection fifo HP arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_HP_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_HP_ARB_STATE_position     43
#define ND_500_DCR__STAT0__INJ_HP_ARB_STATE_get(x)       _BGQ_GET(4,43,x)
#define ND_500_DCR__STAT0__INJ_HP_ARB_STATE_set(v)       _BGQ_SET(4,43,v)
#define ND_500_DCR__STAT0__INJ_HP_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,43,x,v)

//   Field:       inj_io_arb_state
//   Description: injection fifo IO arbiter state.  IDLE state is b0001
#define ND_500_DCR__STAT0__INJ_IO_ARB_STATE_width        4
#define ND_500_DCR__STAT0__INJ_IO_ARB_STATE_position     47
#define ND_500_DCR__STAT0__INJ_IO_ARB_STATE_get(x)       _BGQ_GET(4,47,x)
#define ND_500_DCR__STAT0__INJ_IO_ARB_STATE_set(v)       _BGQ_SET(4,47,v)
#define ND_500_DCR__STAT0__INJ_IO_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,47,x,v)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT0__RESERVED_width        16
#define ND_500_DCR__STAT0__RESERVED_position     63
#define ND_500_DCR__STAT0__RESERVED_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__STAT0__RESERVED_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__STAT0__RESERVED_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 1
#define ND_500_DCR__STAT1_offset  (0x0032) // Offset of register in instance 
#define ND_500_DCR__STAT1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_nd_state
//   Description: reception fifo T0 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T0_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T0_ND_STATE_position     2
#define ND_500_DCR__STAT1__RCP_T0_ND_STATE_get(x)       _BGQ_GET(3,2,x)
#define ND_500_DCR__STAT1__RCP_T0_ND_STATE_set(v)       _BGQ_SET(3,2,v)
#define ND_500_DCR__STAT1__RCP_T0_ND_STATE_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       rcp_t1_nd_state
//   Description: reception fifo T1 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T1_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T1_ND_STATE_position     5
#define ND_500_DCR__STAT1__RCP_T1_ND_STATE_get(x)       _BGQ_GET(3,5,x)
#define ND_500_DCR__STAT1__RCP_T1_ND_STATE_set(v)       _BGQ_SET(3,5,v)
#define ND_500_DCR__STAT1__RCP_T1_ND_STATE_insert(x,v)  _BGQ_INSERT(3,5,x,v)

//   Field:       rcp_t2_nd_state
//   Description: reception fifo T2 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T2_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T2_ND_STATE_position     8
#define ND_500_DCR__STAT1__RCP_T2_ND_STATE_get(x)       _BGQ_GET(3,8,x)
#define ND_500_DCR__STAT1__RCP_T2_ND_STATE_set(v)       _BGQ_SET(3,8,v)
#define ND_500_DCR__STAT1__RCP_T2_ND_STATE_insert(x,v)  _BGQ_INSERT(3,8,x,v)

//   Field:       rcp_t3_nd_state
//   Description: reception fifo T3 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T3_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T3_ND_STATE_position     11
#define ND_500_DCR__STAT1__RCP_T3_ND_STATE_get(x)       _BGQ_GET(3,11,x)
#define ND_500_DCR__STAT1__RCP_T3_ND_STATE_set(v)       _BGQ_SET(3,11,v)
#define ND_500_DCR__STAT1__RCP_T3_ND_STATE_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       rcp_t4_nd_state
//   Description: reception fifo T4 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T4_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T4_ND_STATE_position     14
#define ND_500_DCR__STAT1__RCP_T4_ND_STATE_get(x)       _BGQ_GET(3,14,x)
#define ND_500_DCR__STAT1__RCP_T4_ND_STATE_set(v)       _BGQ_SET(3,14,v)
#define ND_500_DCR__STAT1__RCP_T4_ND_STATE_insert(x,v)  _BGQ_INSERT(3,14,x,v)

//   Field:       rcp_t5_nd_state
//   Description: reception fifo T5 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T5_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T5_ND_STATE_position     17
#define ND_500_DCR__STAT1__RCP_T5_ND_STATE_get(x)       _BGQ_GET(3,17,x)
#define ND_500_DCR__STAT1__RCP_T5_ND_STATE_set(v)       _BGQ_SET(3,17,v)
#define ND_500_DCR__STAT1__RCP_T5_ND_STATE_insert(x,v)  _BGQ_INSERT(3,17,x,v)

//   Field:       rcp_t6_nd_state
//   Description: reception fifo T6 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T6_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T6_ND_STATE_position     20
#define ND_500_DCR__STAT1__RCP_T6_ND_STATE_get(x)       _BGQ_GET(3,20,x)
#define ND_500_DCR__STAT1__RCP_T6_ND_STATE_set(v)       _BGQ_SET(3,20,v)
#define ND_500_DCR__STAT1__RCP_T6_ND_STATE_insert(x,v)  _BGQ_INSERT(3,20,x,v)

//   Field:       rcp_t7_nd_state
//   Description: reception fifo T7 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T7_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T7_ND_STATE_position     23
#define ND_500_DCR__STAT1__RCP_T7_ND_STATE_get(x)       _BGQ_GET(3,23,x)
#define ND_500_DCR__STAT1__RCP_T7_ND_STATE_set(v)       _BGQ_SET(3,23,v)
#define ND_500_DCR__STAT1__RCP_T7_ND_STATE_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       rcp_t8_nd_state
//   Description: reception fifo T8 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T8_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T8_ND_STATE_position     26
#define ND_500_DCR__STAT1__RCP_T8_ND_STATE_get(x)       _BGQ_GET(3,26,x)
#define ND_500_DCR__STAT1__RCP_T8_ND_STATE_set(v)       _BGQ_SET(3,26,v)
#define ND_500_DCR__STAT1__RCP_T8_ND_STATE_insert(x,v)  _BGQ_INSERT(3,26,x,v)

//   Field:       rcp_t9_nd_state
//   Description: reception fifo T9 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_T9_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_T9_ND_STATE_position     29
#define ND_500_DCR__STAT1__RCP_T9_ND_STATE_get(x)       _BGQ_GET(3,29,x)
#define ND_500_DCR__STAT1__RCP_T9_ND_STATE_set(v)       _BGQ_SET(3,29,v)
#define ND_500_DCR__STAT1__RCP_T9_ND_STATE_insert(x,v)  _BGQ_INSERT(3,29,x,v)

//   Field:       rcp_l0_nd_state
//   Description: reception fifo L0 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_L0_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_L0_ND_STATE_position     32
#define ND_500_DCR__STAT1__RCP_L0_ND_STATE_get(x)       _BGQ_GET(3,32,x)
#define ND_500_DCR__STAT1__RCP_L0_ND_STATE_set(v)       _BGQ_SET(3,32,v)
#define ND_500_DCR__STAT1__RCP_L0_ND_STATE_insert(x,v)  _BGQ_INSERT(3,32,x,v)

//   Field:       rcp_l1_nd_state
//   Description: reception fifo L1 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_L1_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_L1_ND_STATE_position     35
#define ND_500_DCR__STAT1__RCP_L1_ND_STATE_get(x)       _BGQ_GET(3,35,x)
#define ND_500_DCR__STAT1__RCP_L1_ND_STATE_set(v)       _BGQ_SET(3,35,v)
#define ND_500_DCR__STAT1__RCP_L1_ND_STATE_insert(x,v)  _BGQ_INSERT(3,35,x,v)

//   Field:       rcp_hp_nd_state
//   Description: reception fifo HP network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_HP_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_HP_ND_STATE_position     38
#define ND_500_DCR__STAT1__RCP_HP_ND_STATE_get(x)       _BGQ_GET(3,38,x)
#define ND_500_DCR__STAT1__RCP_HP_ND_STATE_set(v)       _BGQ_SET(3,38,v)
#define ND_500_DCR__STAT1__RCP_HP_ND_STATE_insert(x,v)  _BGQ_INSERT(3,38,x,v)

//   Field:       rcp_io_nd_state
//   Description: reception fifo IO network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_IO_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_IO_ND_STATE_position     41
#define ND_500_DCR__STAT1__RCP_IO_ND_STATE_get(x)       _BGQ_GET(3,41,x)
#define ND_500_DCR__STAT1__RCP_IO_ND_STATE_set(v)       _BGQ_SET(3,41,v)
#define ND_500_DCR__STAT1__RCP_IO_ND_STATE_insert(x,v)  _BGQ_INSERT(3,41,x,v)

//   Field:       rcp_c0_nd_state
//   Description: reception fifo C0 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_C0_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_C0_ND_STATE_position     44
#define ND_500_DCR__STAT1__RCP_C0_ND_STATE_get(x)       _BGQ_GET(3,44,x)
#define ND_500_DCR__STAT1__RCP_C0_ND_STATE_set(v)       _BGQ_SET(3,44,v)
#define ND_500_DCR__STAT1__RCP_C0_ND_STATE_insert(x,v)  _BGQ_INSERT(3,44,x,v)

//   Field:       rcp_c1_nd_state
//   Description: reception fifo C1 network fsm state.  IDLE state is b001
#define ND_500_DCR__STAT1__RCP_C1_ND_STATE_width        3
#define ND_500_DCR__STAT1__RCP_C1_ND_STATE_position     47
#define ND_500_DCR__STAT1__RCP_C1_ND_STATE_get(x)       _BGQ_GET(3,47,x)
#define ND_500_DCR__STAT1__RCP_C1_ND_STATE_set(v)       _BGQ_SET(3,47,v)
#define ND_500_DCR__STAT1__RCP_C1_ND_STATE_insert(x,v)  _BGQ_INSERT(3,47,x,v)

//   Field:       coll_up_arb_state
//   Description: collective uptree main arbiter fsm state.  IDLE state is b000000
#define ND_500_DCR__STAT1__COLL_UP_ARB_STATE_width        6
#define ND_500_DCR__STAT1__COLL_UP_ARB_STATE_position     53
#define ND_500_DCR__STAT1__COLL_UP_ARB_STATE_get(x)       _BGQ_GET(6,53,x)
#define ND_500_DCR__STAT1__COLL_UP_ARB_STATE_set(v)       _BGQ_SET(6,53,v)
#define ND_500_DCR__STAT1__COLL_UP_ARB_STATE_insert(x,v)  _BGQ_INSERT(6,53,x,v)

//   Field:       coll_down_arb_state
//   Description: collective downtree main arbiter fsm state.  IDLE state is b000000
#define ND_500_DCR__STAT1__COLL_DOWN_ARB_STATE_width        6
#define ND_500_DCR__STAT1__COLL_DOWN_ARB_STATE_position     59
#define ND_500_DCR__STAT1__COLL_DOWN_ARB_STATE_get(x)       _BGQ_GET(6,59,x)
#define ND_500_DCR__STAT1__COLL_DOWN_ARB_STATE_set(v)       _BGQ_SET(6,59,v)
#define ND_500_DCR__STAT1__COLL_DOWN_ARB_STATE_insert(x,v)  _BGQ_INSERT(6,59,x,v)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT1__RESERVED_width        4
#define ND_500_DCR__STAT1__RESERVED_position     63
#define ND_500_DCR__STAT1__RESERVED_get(x)       _BGQ_GET(4,63,x)
#define ND_500_DCR__STAT1__RESERVED_set(v)       _BGQ_SET(4,63,v)
#define ND_500_DCR__STAT1__RESERVED_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        stat2
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 2
#define ND_500_DCR__STAT2_offset  (0x0033) // Offset of register in instance 
#define ND_500_DCR__STAT2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_nd_state
//   Description: injection fifo T0 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T0_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T0_ND_STATE_position     4
#define ND_500_DCR__STAT2__INJ_T0_ND_STATE_get(x)       _BGQ_GET(5,4,x)
#define ND_500_DCR__STAT2__INJ_T0_ND_STATE_set(v)       _BGQ_SET(5,4,v)
#define ND_500_DCR__STAT2__INJ_T0_ND_STATE_insert(x,v)  _BGQ_INSERT(5,4,x,v)

//   Field:       inj_t1_nd_state
//   Description: injection fifo T1 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T1_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T1_ND_STATE_position     9
#define ND_500_DCR__STAT2__INJ_T1_ND_STATE_get(x)       _BGQ_GET(5,9,x)
#define ND_500_DCR__STAT2__INJ_T1_ND_STATE_set(v)       _BGQ_SET(5,9,v)
#define ND_500_DCR__STAT2__INJ_T1_ND_STATE_insert(x,v)  _BGQ_INSERT(5,9,x,v)

//   Field:       inj_t2_nd_state
//   Description: injection fifo T2 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T2_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T2_ND_STATE_position     14
#define ND_500_DCR__STAT2__INJ_T2_ND_STATE_get(x)       _BGQ_GET(5,14,x)
#define ND_500_DCR__STAT2__INJ_T2_ND_STATE_set(v)       _BGQ_SET(5,14,v)
#define ND_500_DCR__STAT2__INJ_T2_ND_STATE_insert(x,v)  _BGQ_INSERT(5,14,x,v)

//   Field:       inj_t3_nd_state
//   Description: injection fifo T3 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T3_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T3_ND_STATE_position     19
#define ND_500_DCR__STAT2__INJ_T3_ND_STATE_get(x)       _BGQ_GET(5,19,x)
#define ND_500_DCR__STAT2__INJ_T3_ND_STATE_set(v)       _BGQ_SET(5,19,v)
#define ND_500_DCR__STAT2__INJ_T3_ND_STATE_insert(x,v)  _BGQ_INSERT(5,19,x,v)

//   Field:       inj_t4_nd_state
//   Description: injection fifo T4 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T4_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T4_ND_STATE_position     24
#define ND_500_DCR__STAT2__INJ_T4_ND_STATE_get(x)       _BGQ_GET(5,24,x)
#define ND_500_DCR__STAT2__INJ_T4_ND_STATE_set(v)       _BGQ_SET(5,24,v)
#define ND_500_DCR__STAT2__INJ_T4_ND_STATE_insert(x,v)  _BGQ_INSERT(5,24,x,v)

//   Field:       inj_t5_nd_state
//   Description: injection fifo T5 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T5_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T5_ND_STATE_position     29
#define ND_500_DCR__STAT2__INJ_T5_ND_STATE_get(x)       _BGQ_GET(5,29,x)
#define ND_500_DCR__STAT2__INJ_T5_ND_STATE_set(v)       _BGQ_SET(5,29,v)
#define ND_500_DCR__STAT2__INJ_T5_ND_STATE_insert(x,v)  _BGQ_INSERT(5,29,x,v)

//   Field:       inj_t6_nd_state
//   Description: injection fifo T6 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T6_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T6_ND_STATE_position     34
#define ND_500_DCR__STAT2__INJ_T6_ND_STATE_get(x)       _BGQ_GET(5,34,x)
#define ND_500_DCR__STAT2__INJ_T6_ND_STATE_set(v)       _BGQ_SET(5,34,v)
#define ND_500_DCR__STAT2__INJ_T6_ND_STATE_insert(x,v)  _BGQ_INSERT(5,34,x,v)

//   Field:       inj_t7_nd_state
//   Description: injection fifo T7 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T7_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T7_ND_STATE_position     39
#define ND_500_DCR__STAT2__INJ_T7_ND_STATE_get(x)       _BGQ_GET(5,39,x)
#define ND_500_DCR__STAT2__INJ_T7_ND_STATE_set(v)       _BGQ_SET(5,39,v)
#define ND_500_DCR__STAT2__INJ_T7_ND_STATE_insert(x,v)  _BGQ_INSERT(5,39,x,v)

//   Field:       inj_t8_nd_state
//   Description: injection fifo T8 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T8_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T8_ND_STATE_position     44
#define ND_500_DCR__STAT2__INJ_T8_ND_STATE_get(x)       _BGQ_GET(5,44,x)
#define ND_500_DCR__STAT2__INJ_T8_ND_STATE_set(v)       _BGQ_SET(5,44,v)
#define ND_500_DCR__STAT2__INJ_T8_ND_STATE_insert(x,v)  _BGQ_INSERT(5,44,x,v)

//   Field:       inj_t9_nd_state
//   Description: injection fifo T9 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_T9_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_T9_ND_STATE_position     49
#define ND_500_DCR__STAT2__INJ_T9_ND_STATE_get(x)       _BGQ_GET(5,49,x)
#define ND_500_DCR__STAT2__INJ_T9_ND_STATE_set(v)       _BGQ_SET(5,49,v)
#define ND_500_DCR__STAT2__INJ_T9_ND_STATE_insert(x,v)  _BGQ_INSERT(5,49,x,v)

//   Field:       inj_l0_nd_state
//   Description: injection fifo L0 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_L0_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_L0_ND_STATE_position     54
#define ND_500_DCR__STAT2__INJ_L0_ND_STATE_get(x)       _BGQ_GET(5,54,x)
#define ND_500_DCR__STAT2__INJ_L0_ND_STATE_set(v)       _BGQ_SET(5,54,v)
#define ND_500_DCR__STAT2__INJ_L0_ND_STATE_insert(x,v)  _BGQ_INSERT(5,54,x,v)

//   Field:       inj_l1_nd_state
//   Description: injection fifo L1 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT2__INJ_L1_ND_STATE_width        5
#define ND_500_DCR__STAT2__INJ_L1_ND_STATE_position     59
#define ND_500_DCR__STAT2__INJ_L1_ND_STATE_get(x)       _BGQ_GET(5,59,x)
#define ND_500_DCR__STAT2__INJ_L1_ND_STATE_set(v)       _BGQ_SET(5,59,v)
#define ND_500_DCR__STAT2__INJ_L1_ND_STATE_insert(x,v)  _BGQ_INSERT(5,59,x,v)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT2__RESERVED_width        4
#define ND_500_DCR__STAT2__RESERVED_position     63
#define ND_500_DCR__STAT2__RESERVED_get(x)       _BGQ_GET(4,63,x)
#define ND_500_DCR__STAT2__RESERVED_set(v)       _BGQ_SET(4,63,v)
#define ND_500_DCR__STAT2__RESERVED_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        stat3
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 3
#define ND_500_DCR__STAT3_offset  (0x0034) // Offset of register in instance 
#define ND_500_DCR__STAT3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_hp_nd_state
//   Description: injection fifo HP network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT3__INJ_HP_ND_STATE_width        5
#define ND_500_DCR__STAT3__INJ_HP_ND_STATE_position     4
#define ND_500_DCR__STAT3__INJ_HP_ND_STATE_get(x)       _BGQ_GET(5,4,x)
#define ND_500_DCR__STAT3__INJ_HP_ND_STATE_set(v)       _BGQ_SET(5,4,v)
#define ND_500_DCR__STAT3__INJ_HP_ND_STATE_insert(x,v)  _BGQ_INSERT(5,4,x,v)

//   Field:       inj_io_nd_state
//   Description: injection fifo IO network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT3__INJ_IO_ND_STATE_width        5
#define ND_500_DCR__STAT3__INJ_IO_ND_STATE_position     9
#define ND_500_DCR__STAT3__INJ_IO_ND_STATE_get(x)       _BGQ_GET(5,9,x)
#define ND_500_DCR__STAT3__INJ_IO_ND_STATE_set(v)       _BGQ_SET(5,9,v)
#define ND_500_DCR__STAT3__INJ_IO_ND_STATE_insert(x,v)  _BGQ_INSERT(5,9,x,v)

//   Field:       inj_c0_nd_state
//   Description: injection fifo C0 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT3__INJ_C0_ND_STATE_width        5
#define ND_500_DCR__STAT3__INJ_C0_ND_STATE_position     14
#define ND_500_DCR__STAT3__INJ_C0_ND_STATE_get(x)       _BGQ_GET(5,14,x)
#define ND_500_DCR__STAT3__INJ_C0_ND_STATE_set(v)       _BGQ_SET(5,14,v)
#define ND_500_DCR__STAT3__INJ_C0_ND_STATE_insert(x,v)  _BGQ_INSERT(5,14,x,v)

//   Field:       inj_c1_nd_state
//   Description: injection fifo C1 network fsm state.  IDLE state is b00000
#define ND_500_DCR__STAT3__INJ_C1_ND_STATE_width        5
#define ND_500_DCR__STAT3__INJ_C1_ND_STATE_position     19
#define ND_500_DCR__STAT3__INJ_C1_ND_STATE_get(x)       _BGQ_GET(5,19,x)
#define ND_500_DCR__STAT3__INJ_C1_ND_STATE_set(v)       _BGQ_SET(5,19,v)
#define ND_500_DCR__STAT3__INJ_C1_ND_STATE_insert(x,v)  _BGQ_INSERT(5,19,x,v)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT3__RESERVED_width        44
#define ND_500_DCR__STAT3__RESERVED_position     63
#define ND_500_DCR__STAT3__RESERVED_get(x)       _BGQ_GET(44,63,x)
#define ND_500_DCR__STAT3__RESERVED_set(v)       _BGQ_SET(44,63,v)
#define ND_500_DCR__STAT3__RESERVED_insert(x,v)  _BGQ_INSERT(44,63,x,v)

  
// Register:
//   Name:        stat4
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 4
#define ND_500_DCR__STAT4_offset  (0x0035) // Offset of register in instance 
#define ND_500_DCR__STAT4_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_hdrchk_err_vec
//   Description: injection fifo T0 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT4__INJ_T0_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT4__INJ_T0_HDRCHK_ERR_VEC_position     15
#define ND_500_DCR__STAT4__INJ_T0_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,15,x)
#define ND_500_DCR__STAT4__INJ_T0_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,15,v)
#define ND_500_DCR__STAT4__INJ_T0_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       inj_t1_hdrchk_err_vec
//   Description: injection fifo T1 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT4__INJ_T1_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT4__INJ_T1_HDRCHK_ERR_VEC_position     31
#define ND_500_DCR__STAT4__INJ_T1_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,31,x)
#define ND_500_DCR__STAT4__INJ_T1_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,31,v)
#define ND_500_DCR__STAT4__INJ_T1_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       inj_t2_hdrchk_err_vec
//   Description: injection fifo T2 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT4__INJ_T2_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT4__INJ_T2_HDRCHK_ERR_VEC_position     47
#define ND_500_DCR__STAT4__INJ_T2_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,47,x)
#define ND_500_DCR__STAT4__INJ_T2_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,47,v)
#define ND_500_DCR__STAT4__INJ_T2_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,47,x,v)

//   Field:       inj_t3_hdrchk_err_vec
//   Description: injection fifo T3 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT4__INJ_T3_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT4__INJ_T3_HDRCHK_ERR_VEC_position     63
#define ND_500_DCR__STAT4__INJ_T3_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__STAT4__INJ_T3_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__STAT4__INJ_T3_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat5
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 5
#define ND_500_DCR__STAT5_offset  (0x0036) // Offset of register in instance 
#define ND_500_DCR__STAT5_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t4_hdrchk_err_vec
//   Description: injection fifo T4 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT5__INJ_T4_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT5__INJ_T4_HDRCHK_ERR_VEC_position     15
#define ND_500_DCR__STAT5__INJ_T4_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,15,x)
#define ND_500_DCR__STAT5__INJ_T4_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,15,v)
#define ND_500_DCR__STAT5__INJ_T4_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       inj_t5_hdrchk_err_vec
//   Description: injection fifo T5 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT5__INJ_T5_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT5__INJ_T5_HDRCHK_ERR_VEC_position     31
#define ND_500_DCR__STAT5__INJ_T5_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,31,x)
#define ND_500_DCR__STAT5__INJ_T5_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,31,v)
#define ND_500_DCR__STAT5__INJ_T5_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       inj_t6_hdrchk_err_vec
//   Description: injection fifo T6 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT5__INJ_T6_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT5__INJ_T6_HDRCHK_ERR_VEC_position     47
#define ND_500_DCR__STAT5__INJ_T6_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,47,x)
#define ND_500_DCR__STAT5__INJ_T6_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,47,v)
#define ND_500_DCR__STAT5__INJ_T6_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,47,x,v)

//   Field:       inj_t7_hdrchk_err_vec
//   Description: injection fifo T7 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT5__INJ_T7_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT5__INJ_T7_HDRCHK_ERR_VEC_position     63
#define ND_500_DCR__STAT5__INJ_T7_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__STAT5__INJ_T7_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__STAT5__INJ_T7_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat6
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 6
#define ND_500_DCR__STAT6_offset  (0x0037) // Offset of register in instance 
#define ND_500_DCR__STAT6_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t8_hdrchk_err_vec
//   Description: injection fifo T8 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT6__INJ_T8_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT6__INJ_T8_HDRCHK_ERR_VEC_position     15
#define ND_500_DCR__STAT6__INJ_T8_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,15,x)
#define ND_500_DCR__STAT6__INJ_T8_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,15,v)
#define ND_500_DCR__STAT6__INJ_T8_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       inj_t9_hdrchk_err_vec
//   Description: injection fifo T9 point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT6__INJ_T9_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT6__INJ_T9_HDRCHK_ERR_VEC_position     31
#define ND_500_DCR__STAT6__INJ_T9_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,31,x)
#define ND_500_DCR__STAT6__INJ_T9_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,31,v)
#define ND_500_DCR__STAT6__INJ_T9_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       inj_hp_hdrchk_err_vec
//   Description: injection fifo HP point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT6__INJ_HP_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT6__INJ_HP_HDRCHK_ERR_VEC_position     47
#define ND_500_DCR__STAT6__INJ_HP_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,47,x)
#define ND_500_DCR__STAT6__INJ_HP_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,47,v)
#define ND_500_DCR__STAT6__INJ_HP_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,47,x,v)

//   Field:       inj_io_hdrchk_err_vec
//   Description: injection fifo IO point-to-point header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT6__INJ_IO_HDRCHK_ERR_VEC_width        16
#define ND_500_DCR__STAT6__INJ_IO_HDRCHK_ERR_VEC_position     63
#define ND_500_DCR__STAT6__INJ_IO_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(16,63,x)
#define ND_500_DCR__STAT6__INJ_IO_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(16,63,v)
#define ND_500_DCR__STAT6__INJ_IO_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        stat7
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 7
#define ND_500_DCR__STAT7_offset  (0x0038) // Offset of register in instance 
#define ND_500_DCR__STAT7_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_c0_hdrchk_err_vec
//   Description: injection fifo C0 collective header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT7__INJ_C0_HDRCHK_ERR_VEC_width        8
#define ND_500_DCR__STAT7__INJ_C0_HDRCHK_ERR_VEC_position     7
#define ND_500_DCR__STAT7__INJ_C0_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(8,7,x)
#define ND_500_DCR__STAT7__INJ_C0_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(8,7,v)
#define ND_500_DCR__STAT7__INJ_C0_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(8,7,x,v)

//   Field:       inj_c1_hdrchk_err_vec
//   Description: injection fifo C1 collective header checker error vector.  See the MU high level design doc for the meaning of each bit
#define ND_500_DCR__STAT7__INJ_C1_HDRCHK_ERR_VEC_width        8
#define ND_500_DCR__STAT7__INJ_C1_HDRCHK_ERR_VEC_position     15
#define ND_500_DCR__STAT7__INJ_C1_HDRCHK_ERR_VEC_get(x)       _BGQ_GET(8,15,x)
#define ND_500_DCR__STAT7__INJ_C1_HDRCHK_ERR_VEC_set(v)       _BGQ_SET(8,15,v)
#define ND_500_DCR__STAT7__INJ_C1_HDRCHK_ERR_VEC_insert(x,v)  _BGQ_INSERT(8,15,x,v)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT7__RESERVED_width        48
#define ND_500_DCR__STAT7__RESERVED_position     63
#define ND_500_DCR__STAT7__RESERVED_get(x)       _BGQ_GET(48,63,x)
#define ND_500_DCR__STAT7__RESERVED_set(v)       _BGQ_SET(48,63,v)
#define ND_500_DCR__STAT7__RESERVED_insert(x,v)  _BGQ_INSERT(48,63,x,v)

  
// Register:
//   Name:        stat8
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 8
#define ND_500_DCR__STAT8_offset  (0x0039) // Offset of register in instance 
#define ND_500_DCR__STAT8_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT8__RESERVED_width        64
#define ND_500_DCR__STAT8__RESERVED_position     63
#define ND_500_DCR__STAT8__RESERVED_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__STAT8__RESERVED_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__STAT8__RESERVED_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        stat9
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: stat 9
#define ND_500_DCR__STAT9_offset  (0x003A) // Offset of register in instance 
#define ND_500_DCR__STAT9_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to 0
#define ND_500_DCR__STAT9__RESERVED_width        64
#define ND_500_DCR__STAT9__RESERVED_position     63
#define ND_500_DCR__STAT9__RESERVED_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__STAT9__RESERVED_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__STAT9__RESERVED_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        gi_stat_re
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: gi stat receiver n
#define ND_500_DCR__GI_STAT_RE_offset  (0x003B) // Offset of register in instance 
#define ND_500_DCR__GI_STAT_RE_range  (0xB) // Range of external address space
#define ND_500_DCR__GI_STAT_RE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver 0 GI state.  The least significant 3 bits of every 4 bits is the GI state per class
#define ND_500_DCR__GI_STAT_RE__VALUE_width        32
#define ND_500_DCR__GI_STAT_RE__VALUE_position     63
#define ND_500_DCR__GI_STAT_RE__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__GI_STAT_RE__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__GI_STAT_RE__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ce_cnt
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: correctable error count n
#define ND_500_DCR__CE_CNT_offset  (0x0046) // Offset of register in instance 
#define ND_500_DCR__CE_CNT_range  (0x9) // Range of external address space
#define ND_500_DCR__CE_CNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: injection fifo T0, T1, T2, T3 combined correctable ecc error count
#define ND_500_DCR__CE_CNT__VALUE_width        32
#define ND_500_DCR__CE_CNT__VALUE_position     63
#define ND_500_DCR__CE_CNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__CE_CNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__CE_CNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ce_threshold
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: correctable error count threshold
#define ND_500_DCR__CE_THRESHOLD_offset  (0x004F) // Offset of register in instance 
#define ND_500_DCR__CE_THRESHOLD_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       value
//   Description: ce threshold
#define ND_500_DCR__CE_THRESHOLD__VALUE_width        32
#define ND_500_DCR__CE_THRESHOLD__VALUE_position     63
#define ND_500_DCR__CE_THRESHOLD__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__CE_THRESHOLD__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__CE_THRESHOLD__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ctrl_clear1
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: control clear 1
#define ND_500_DCR__CTRL_CLEAR1_offset  (0x0050) // Offset of register in instance 
#define ND_500_DCR__CTRL_CLEAR1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ce_clr
//   Description: correctable error count clear. Each bit clears a correctable error count.  It also clears the corresponding CE count overthreashold interrupt bit fatal_err5
#define ND_500_DCR__CTRL_CLEAR1__CE_CLR_width        9
#define ND_500_DCR__CTRL_CLEAR1__CE_CLR_position     8
#define ND_500_DCR__CTRL_CLEAR1__CE_CLR_get(x)       _BGQ_GET(9,8,x)
#define ND_500_DCR__CTRL_CLEAR1__CE_CLR_set(v)       _BGQ_SET(9,8,v)
#define ND_500_DCR__CTRL_CLEAR1__CE_CLR_insert(x,v)  _BGQ_INSERT(9,8,x,v)

//   Field:       unused
//   Description: unused
#define ND_500_DCR__CTRL_CLEAR1__UNUSED_width        7
#define ND_500_DCR__CTRL_CLEAR1__UNUSED_position     15
#define ND_500_DCR__CTRL_CLEAR1__UNUSED_get(x)       _BGQ_GET(7,15,x)
#define ND_500_DCR__CTRL_CLEAR1__UNUSED_set(v)       _BGQ_SET(7,15,v)
#define ND_500_DCR__CTRL_CLEAR1__UNUSED_insert(x,v)  _BGQ_INSERT(7,15,x,v)

//   Field:       inj_csum_clr
//   Description: injection fifo checksum clear, one bit per fifo, in order of t0, t1, ..., t9, l0, l1, hp, io, c0, c1.  Not working on DD1
#define ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_width        16
#define ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_position     31
#define ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_get(x)       _BGQ_GET(16,31,x)
#define ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_set(v)       _BGQ_SET(16,31,v)
#define ND_500_DCR__CTRL_CLEAR1__INJ_CSUM_CLR_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       reserved
//   Description: control clear 1 reserved
#define ND_500_DCR__CTRL_CLEAR1__RESERVED_width        32
#define ND_500_DCR__CTRL_CLEAR1__RESERVED_position     63
#define ND_500_DCR__CTRL_CLEAR1__RESERVED_get(x)       _BGQ_GET(32,63,x)
#define ND_500_DCR__CTRL_CLEAR1__RESERVED_set(v)       _BGQ_SET(32,63,v)
#define ND_500_DCR__CTRL_CLEAR1__RESERVED_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ctrl_reserved
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: control reserved
#define ND_500_DCR__CTRL_RESERVED_offset  (0x0051) // Offset of register in instance 
#define ND_500_DCR__CTRL_RESERVED_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: reserved
#define ND_500_DCR__CTRL_RESERVED__VALUE_width        64
#define ND_500_DCR__CTRL_RESERVED__VALUE_position     63
#define ND_500_DCR__CTRL_RESERVED__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__CTRL_RESERVED__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__CTRL_RESERVED__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        ctrl_upc_clear
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: control upc clear
#define ND_500_DCR__CTRL_UPC_CLEAR_offset  (0x0052) // Offset of register in instance 
#define ND_500_DCR__CTRL_UPC_CLEAR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_clr
//   Description: upc clear, 5 bits per RE/SE unit, from 0 to 10
#define ND_500_DCR__CTRL_UPC_CLEAR__UPC_CLR_width        55
#define ND_500_DCR__CTRL_UPC_CLEAR__UPC_CLR_position     54
#define ND_500_DCR__CTRL_UPC_CLEAR__UPC_CLR_get(x)       _BGQ_GET(55,54,x)
#define ND_500_DCR__CTRL_UPC_CLEAR__UPC_CLR_set(v)       _BGQ_SET(55,54,v)
#define ND_500_DCR__CTRL_UPC_CLEAR__UPC_CLR_insert(x,v)  _BGQ_INSERT(55,54,x,v)

  
// Register:
//   Name:        ctrl_upc_stop
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: control upc stop
#define ND_500_DCR__CTRL_UPC_STOP_offset  (0x0053) // Offset of register in instance 
#define ND_500_DCR__CTRL_UPC_STOP_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_stop
//   Description: upc stop, 5 bits per RE/SE unit, from 0 to 10
#define ND_500_DCR__CTRL_UPC_STOP__UPC_STOP_width        55
#define ND_500_DCR__CTRL_UPC_STOP__UPC_STOP_position     54
#define ND_500_DCR__CTRL_UPC_STOP__UPC_STOP_get(x)       _BGQ_GET(55,54,x)
#define ND_500_DCR__CTRL_UPC_STOP__UPC_STOP_set(v)       _BGQ_SET(55,54,v)
#define ND_500_DCR__CTRL_UPC_STOP__UPC_STOP_insert(x,v)  _BGQ_INSERT(55,54,x,v)

  
// Register:
//   Name:        fatal_err_stat
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Aggregated fatal status bits, one bit per dcr slave
#define ND_500_DCR__FATAL_ERR_STAT_offset  (0x0054) // Offset of register in instance 
#define ND_500_DCR__FATAL_ERR_STAT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Bit 0 to 10, rese[0-9,a].  Bit 11, dcr_x2. Bit 12, dcr_500
#define ND_500_DCR__FATAL_ERR_STAT__VALUE_width        64
#define ND_500_DCR__FATAL_ERR_STAT__VALUE_position     63
#define ND_500_DCR__FATAL_ERR_STAT__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__FATAL_ERR_STAT__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__FATAL_ERR_STAT__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        non_fatal_err_stat
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Aggregated non fatal status bits, one bit per dcr slave
#define ND_500_DCR__NON_FATAL_ERR_STAT_offset  (0x0055) // Offset of register in instance 
#define ND_500_DCR__NON_FATAL_ERR_STAT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: Bit 0 to 10, rese[0-9,a].  Bit 11, dcr_x2. Bit 12, dcr_500
#define ND_500_DCR__NON_FATAL_ERR_STAT__VALUE_width        64
#define ND_500_DCR__NON_FATAL_ERR_STAT__VALUE_position     63
#define ND_500_DCR__NON_FATAL_ERR_STAT__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__NON_FATAL_ERR_STAT__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__NON_FATAL_ERR_STAT__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        inj_fifo_chksum
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: injection fifo checksum n
#define ND_500_DCR__INJ_FIFO_CHKSUM_offset  (0x0060) // Offset of register in instance 
#define ND_500_DCR__INJ_FIFO_CHKSUM_range  (0x10) // Range of external address space
#define ND_500_DCR__INJ_FIFO_CHKSUM_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: value
#define ND_500_DCR__INJ_FIFO_CHKSUM__VALUE_width        64
#define ND_500_DCR__INJ_FIFO_CHKSUM__VALUE_position     63
#define ND_500_DCR__INJ_FIFO_CHKSUM__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__INJ_FIFO_CHKSUM__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__INJ_FIFO_CHKSUM__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        inj_fifo_head_p0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: injection fifo head pointer 0
#define ND_500_DCR__INJ_FIFO_HEAD_P0_offset  (0x0070) // Offset of register in instance 
#define ND_500_DCR__INJ_FIFO_HEAD_P0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_head
//   Description: injection fifo T0 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T0_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T0_HEAD_position     7
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T0_HEAD_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T0_HEAD_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T0_HEAD_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       inj_t1_head
//   Description: injection fifo T1 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T1_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T1_HEAD_position     15
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T1_HEAD_get(x)       _BGQ_GET(6,15,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T1_HEAD_set(v)       _BGQ_SET(6,15,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T1_HEAD_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       inj_t2_head
//   Description: injection fifo T2 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T2_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T2_HEAD_position     23
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T2_HEAD_get(x)       _BGQ_GET(6,23,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T2_HEAD_set(v)       _BGQ_SET(6,23,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T2_HEAD_insert(x,v)  _BGQ_INSERT(6,23,x,v)

//   Field:       inj_t3_head
//   Description: injection fifo T3 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T3_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T3_HEAD_position     31
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T3_HEAD_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T3_HEAD_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T3_HEAD_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       inj_t4_head
//   Description: injection fifo T4 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T4_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T4_HEAD_position     39
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T4_HEAD_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T4_HEAD_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T4_HEAD_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       inj_t5_head
//   Description: injection fifo T5 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T5_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T5_HEAD_position     47
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T5_HEAD_get(x)       _BGQ_GET(6,47,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T5_HEAD_set(v)       _BGQ_SET(6,47,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T5_HEAD_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       inj_t6_head
//   Description: injection fifo T6 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T6_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T6_HEAD_position     55
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T6_HEAD_get(x)       _BGQ_GET(6,55,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T6_HEAD_set(v)       _BGQ_SET(6,55,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T6_HEAD_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       inj_t7_head
//   Description: injection fifo T7 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T7_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T7_HEAD_position     63
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T7_HEAD_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T7_HEAD_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P0__INJ_T7_HEAD_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        inj_fifo_head_p1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: injection fifo head pointer 1
#define ND_500_DCR__INJ_FIFO_HEAD_P1_offset  (0x0071) // Offset of register in instance 
#define ND_500_DCR__INJ_FIFO_HEAD_P1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t8_head
//   Description: injection fifo T8 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T8_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T8_HEAD_position     7
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T8_HEAD_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T8_HEAD_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T8_HEAD_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       inj_t9_head
//   Description: injection fifo T9 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T9_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T9_HEAD_position     15
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T9_HEAD_get(x)       _BGQ_GET(6,15,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T9_HEAD_set(v)       _BGQ_SET(6,15,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_T9_HEAD_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       inj_l0_head
//   Description: injection fifo L0 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L0_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L0_HEAD_position     23
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L0_HEAD_get(x)       _BGQ_GET(6,23,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L0_HEAD_set(v)       _BGQ_SET(6,23,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L0_HEAD_insert(x,v)  _BGQ_INSERT(6,23,x,v)

//   Field:       inj_l1_head
//   Description: injection fifo L1 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L1_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L1_HEAD_position     31
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L1_HEAD_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L1_HEAD_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_L1_HEAD_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       inj_hp_head
//   Description: injection fifo HP head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_HP_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_HP_HEAD_position     39
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_HP_HEAD_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_HP_HEAD_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_HP_HEAD_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       inj_io_head
//   Description: injection fifo IO head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_position     47
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_get(x)       _BGQ_GET(6,47,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_set(v)       _BGQ_SET(6,47,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       inj_c0_head
//   Description: injection fifo C0 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C0_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C0_HEAD_position     55
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C0_HEAD_get(x)       _BGQ_GET(6,55,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C0_HEAD_set(v)       _BGQ_SET(6,55,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C0_HEAD_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       inj_c1_head
//   Description: injection fifo C1 head pointer
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_width        6
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_position     63
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        inj_fifo_tail_p0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: injection fifo tail pointer 0
#define ND_500_DCR__INJ_FIFO_TAIL_P0_offset  (0x0072) // Offset of register in instance 
#define ND_500_DCR__INJ_FIFO_TAIL_P0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t0_tail
//   Description: injection fifo T0 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T0_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T0_TAIL_position     7
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T0_TAIL_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T0_TAIL_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T0_TAIL_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       inj_t1_tail
//   Description: injection fifo T1 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T1_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T1_TAIL_position     15
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T1_TAIL_get(x)       _BGQ_GET(6,15,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T1_TAIL_set(v)       _BGQ_SET(6,15,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T1_TAIL_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       inj_t2_tail
//   Description: injection fifo T2 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T2_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T2_TAIL_position     23
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T2_TAIL_get(x)       _BGQ_GET(6,23,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T2_TAIL_set(v)       _BGQ_SET(6,23,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T2_TAIL_insert(x,v)  _BGQ_INSERT(6,23,x,v)

//   Field:       inj_t3_tail
//   Description: injection fifo T3 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T3_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T3_TAIL_position     31
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T3_TAIL_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T3_TAIL_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T3_TAIL_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       inj_t4_tail
//   Description: injection fifo T4 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T4_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T4_TAIL_position     39
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T4_TAIL_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T4_TAIL_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T4_TAIL_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       inj_t5_tail
//   Description: injection fifo T5 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T5_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T5_TAIL_position     47
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T5_TAIL_get(x)       _BGQ_GET(6,47,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T5_TAIL_set(v)       _BGQ_SET(6,47,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T5_TAIL_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       inj_t6_tail
//   Description: injection fifo T6 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T6_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T6_TAIL_position     55
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T6_TAIL_get(x)       _BGQ_GET(6,55,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T6_TAIL_set(v)       _BGQ_SET(6,55,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T6_TAIL_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       inj_t7_tail
//   Description: injection fifo T7 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T7_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T7_TAIL_position     63
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T7_TAIL_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T7_TAIL_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P0__INJ_T7_TAIL_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        inj_fifo_tail_p1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: injection fifo tail pointer 1
#define ND_500_DCR__INJ_FIFO_TAIL_P1_offset  (0x0073) // Offset of register in instance 
#define ND_500_DCR__INJ_FIFO_TAIL_P1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       inj_t8_tail
//   Description: injection fifo T8 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T8_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T8_TAIL_position     7
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T8_TAIL_get(x)       _BGQ_GET(6,7,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T8_TAIL_set(v)       _BGQ_SET(6,7,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T8_TAIL_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       inj_t9_tail
//   Description: injection fifo T9 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T9_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T9_TAIL_position     15
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T9_TAIL_get(x)       _BGQ_GET(6,15,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T9_TAIL_set(v)       _BGQ_SET(6,15,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_T9_TAIL_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       inj_l0_tail
//   Description: injection fifo L0 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L0_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L0_TAIL_position     23
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L0_TAIL_get(x)       _BGQ_GET(6,23,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L0_TAIL_set(v)       _BGQ_SET(6,23,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L0_TAIL_insert(x,v)  _BGQ_INSERT(6,23,x,v)

//   Field:       inj_l1_tail
//   Description: injection fifo L1 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L1_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L1_TAIL_position     31
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L1_TAIL_get(x)       _BGQ_GET(6,31,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L1_TAIL_set(v)       _BGQ_SET(6,31,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_L1_TAIL_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       inj_hp_tail
//   Description: injection fifo HP tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_HP_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_HP_TAIL_position     39
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_HP_TAIL_get(x)       _BGQ_GET(6,39,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_HP_TAIL_set(v)       _BGQ_SET(6,39,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_HP_TAIL_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       inj_io_tail
//   Description: injection fifo IO tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_IO_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_IO_TAIL_position     47
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_IO_TAIL_get(x)       _BGQ_GET(6,47,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_IO_TAIL_set(v)       _BGQ_SET(6,47,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_IO_TAIL_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       inj_c0_tail
//   Description: injection fifo C0 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C0_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C0_TAIL_position     55
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C0_TAIL_get(x)       _BGQ_GET(6,55,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C0_TAIL_set(v)       _BGQ_SET(6,55,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C0_TAIL_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       inj_c1_tail
//   Description: injection fifo C1 tail pointer
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C1_TAIL_width        6
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C1_TAIL_position     63
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C1_TAIL_get(x)       _BGQ_GET(6,63,x)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C1_TAIL_set(v)       _BGQ_SET(6,63,v)
#define ND_500_DCR__INJ_FIFO_TAIL_P1__INJ_C1_TAIL_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        rcp_fifo_head_p0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo head pointer 0
#define ND_500_DCR__RCP_FIFO_HEAD_P0_offset  (0x0074) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_HEAD_P0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_head
//   Description: reception fifo T0 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T0_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T0_HEAD_position     15
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T0_HEAD_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T0_HEAD_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T0_HEAD_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t1_head
//   Description: reception fifo T1 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T1_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T1_HEAD_position     31
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T1_HEAD_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T1_HEAD_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T1_HEAD_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_t2_head
//   Description: reception fifo T2 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T2_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T2_HEAD_position     47
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T2_HEAD_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T2_HEAD_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T2_HEAD_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_t3_head
//   Description: reception fifo T3 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T3_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T3_HEAD_position     63
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T3_HEAD_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T3_HEAD_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P0__RCP_T3_HEAD_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_head_p1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo head pointer 1
#define ND_500_DCR__RCP_FIFO_HEAD_P1_offset  (0x0075) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_HEAD_P1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t4_head
//   Description: reception fifo T4 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T4_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T4_HEAD_position     15
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T4_HEAD_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T4_HEAD_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T4_HEAD_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t5_head
//   Description: reception fifo T5 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T5_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T5_HEAD_position     31
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T5_HEAD_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T5_HEAD_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T5_HEAD_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_t6_head
//   Description: reception fifo T6 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T6_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T6_HEAD_position     47
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T6_HEAD_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T6_HEAD_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T6_HEAD_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_t7_head
//   Description: reception fifo T7 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T7_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T7_HEAD_position     63
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T7_HEAD_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T7_HEAD_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P1__RCP_T7_HEAD_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_head_p2
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo head pointer 2
#define ND_500_DCR__RCP_FIFO_HEAD_P2_offset  (0x0076) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_HEAD_P2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t8_head
//   Description: reception fifo T8 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T8_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T8_HEAD_position     15
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T8_HEAD_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T8_HEAD_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T8_HEAD_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t9_head
//   Description: reception fifo T9 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T9_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T9_HEAD_position     31
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T9_HEAD_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T9_HEAD_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_T9_HEAD_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_l0_head
//   Description: reception fifo L0 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L0_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L0_HEAD_position     47
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L0_HEAD_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L0_HEAD_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L0_HEAD_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_l1_head
//   Description: reception fifo L1 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L1_HEAD_width        9
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L1_HEAD_position     63
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L1_HEAD_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L1_HEAD_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P2__RCP_L1_HEAD_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_head_p3
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo head pointer 3
#define ND_500_DCR__RCP_FIFO_HEAD_P3_offset  (0x0077) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_HEAD_P3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_hp_head
//   Description: reception fifo HP head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_HP_HEAD_width        10
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_HP_HEAD_position     15
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_HP_HEAD_get(x)       _BGQ_GET(10,15,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_HP_HEAD_set(v)       _BGQ_SET(10,15,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_HP_HEAD_insert(x,v)  _BGQ_INSERT(10,15,x,v)

//   Field:       rcp_io_head
//   Description: reception fifo IO head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_width        10
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_position     31
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_get(x)       _BGQ_GET(10,31,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_set(v)       _BGQ_SET(10,31,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_insert(x,v)  _BGQ_INSERT(10,31,x,v)

//   Field:       rcp_c0_head
//   Description: reception fifo C0 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C0_HEAD_width        10
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C0_HEAD_position     47
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C0_HEAD_get(x)       _BGQ_GET(10,47,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C0_HEAD_set(v)       _BGQ_SET(10,47,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C0_HEAD_insert(x,v)  _BGQ_INSERT(10,47,x,v)

//   Field:       rcp_c1_head
//   Description: reception fifo C1 head pointer
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_width        10
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_position     63
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_get(x)       _BGQ_GET(10,63,x)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_set(v)       _BGQ_SET(10,63,v)
#define ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        rcp_fifo_tail_p0
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo tail pointer 0
#define ND_500_DCR__RCP_FIFO_TAIL_P0_offset  (0x0078) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_TAIL_P0_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t0_tail
//   Description: reception fifo T0 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T0_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T0_TAIL_position     15
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T0_TAIL_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T0_TAIL_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T0_TAIL_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t1_tail
//   Description: reception fifo T1 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T1_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T1_TAIL_position     31
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T1_TAIL_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T1_TAIL_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T1_TAIL_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_t2_tail
//   Description: reception fifo T2 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T2_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T2_TAIL_position     47
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T2_TAIL_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T2_TAIL_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T2_TAIL_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_t3_tail
//   Description: reception fifo T3 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T3_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T3_TAIL_position     63
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T3_TAIL_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T3_TAIL_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P0__RCP_T3_TAIL_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_tail_p1
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo tail pointer 1
#define ND_500_DCR__RCP_FIFO_TAIL_P1_offset  (0x0079) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_TAIL_P1_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t4_tail
//   Description: reception fifo T4 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T4_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T4_TAIL_position     15
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T4_TAIL_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T4_TAIL_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T4_TAIL_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t5_tail
//   Description: reception fifo T5 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T5_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T5_TAIL_position     31
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T5_TAIL_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T5_TAIL_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T5_TAIL_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_t6_tail
//   Description: reception fifo T6 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T6_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T6_TAIL_position     47
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T6_TAIL_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T6_TAIL_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T6_TAIL_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_t7_tail
//   Description: reception fifo T7 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T7_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T7_TAIL_position     63
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T7_TAIL_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T7_TAIL_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P1__RCP_T7_TAIL_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_tail_p2
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo tail pointer 2
#define ND_500_DCR__RCP_FIFO_TAIL_P2_offset  (0x007A) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_TAIL_P2_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_t8_tail
//   Description: reception fifo T8 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T8_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T8_TAIL_position     15
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T8_TAIL_get(x)       _BGQ_GET(9,15,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T8_TAIL_set(v)       _BGQ_SET(9,15,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T8_TAIL_insert(x,v)  _BGQ_INSERT(9,15,x,v)

//   Field:       rcp_t9_tail
//   Description: reception fifo T9 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T9_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T9_TAIL_position     31
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T9_TAIL_get(x)       _BGQ_GET(9,31,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T9_TAIL_set(v)       _BGQ_SET(9,31,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_T9_TAIL_insert(x,v)  _BGQ_INSERT(9,31,x,v)

//   Field:       rcp_l0_tail
//   Description: reception fifo L0 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L0_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L0_TAIL_position     47
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L0_TAIL_get(x)       _BGQ_GET(9,47,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L0_TAIL_set(v)       _BGQ_SET(9,47,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L0_TAIL_insert(x,v)  _BGQ_INSERT(9,47,x,v)

//   Field:       rcp_l1_tail
//   Description: reception fifo L1 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L1_TAIL_width        9
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L1_TAIL_position     63
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L1_TAIL_get(x)       _BGQ_GET(9,63,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L1_TAIL_set(v)       _BGQ_SET(9,63,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P2__RCP_L1_TAIL_insert(x,v)  _BGQ_INSERT(9,63,x,v)

  
// Register:
//   Name:        rcp_fifo_tail_p3
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: reception fifo tail pointer 3
#define ND_500_DCR__RCP_FIFO_TAIL_P3_offset  (0x007B) // Offset of register in instance 
#define ND_500_DCR__RCP_FIFO_TAIL_P3_reset  UNSIGNED64(0x0000000000000000)

//   Field:       rcp_hp_tail
//   Description: reception fifo HP tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_HP_TAIL_width        10
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_HP_TAIL_position     15
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_HP_TAIL_get(x)       _BGQ_GET(10,15,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_HP_TAIL_set(v)       _BGQ_SET(10,15,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_HP_TAIL_insert(x,v)  _BGQ_INSERT(10,15,x,v)

//   Field:       rcp_io_tail
//   Description: reception fifo IO tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_IO_TAIL_width        10
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_IO_TAIL_position     31
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_IO_TAIL_get(x)       _BGQ_GET(10,31,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_IO_TAIL_set(v)       _BGQ_SET(10,31,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_IO_TAIL_insert(x,v)  _BGQ_INSERT(10,31,x,v)

//   Field:       rcp_c0_tail
//   Description: reception fifo C0 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C0_TAIL_width        10
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C0_TAIL_position     47
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C0_TAIL_get(x)       _BGQ_GET(10,47,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C0_TAIL_set(v)       _BGQ_SET(10,47,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C0_TAIL_insert(x,v)  _BGQ_INSERT(10,47,x,v)

//   Field:       rcp_c1_tail
//   Description: reception fifo C1 tail pointer
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C1_TAIL_width        10
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C1_TAIL_position     63
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C1_TAIL_get(x)       _BGQ_GET(10,63,x)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C1_TAIL_set(v)       _BGQ_SET(10,63,v)
#define ND_500_DCR__RCP_FIFO_TAIL_P3__RCP_C1_TAIL_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0097) // Offset of register in instance 
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0098) // Offset of register in instance 
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0099) // Offset of register in instance 
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: n500_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_500_DCR__N500_INTERRUPTS__STATE_offset         (0x0080) // Offset of State register in instance
#define ND_500_DCR__N500_INTERRUPTS__FIRST_offset         (0x0084) // Offset of First register in instance
#define ND_500_DCR__N500_INTERRUPTS__FORCE_offset         (0x0083) // Offset of Force register in instance
#define ND_500_DCR__N500_INTERRUPTS__MACHINE_CHECK_offset (0x0085) // Offset of Machine Check register in instance
#define ND_500_DCR__N500_INTERRUPTS__CRITICAL_offset      (0x0086) // Offset of Critical register in instance
#define ND_500_DCR__N500_INTERRUPTS__NONCRITICAL_offset   (0x0087) // Offset of Noncritical register in instance

#define ND_500_DCR__N500_INTERRUPTS__LOCAL_RING_width        1
#define ND_500_DCR__N500_INTERRUPTS__LOCAL_RING_position     63
#define ND_500_DCR__N500_INTERRUPTS__LOCAL_RING_get(x)       _BGQ_GET(1,63,x)
#define ND_500_DCR__N500_INTERRUPTS__LOCAL_RING_set(v)       _BGQ_SET(1,63,v)
#define ND_500_DCR__N500_INTERRUPTS__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0090) // Offset of State register in instance
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0093) // Offset of First register in instance
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0092) // Offset of Force register in instance
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0094) // Offset of Machine Check register in instance
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0095) // Offset of Critical register in instance
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0096) // Offset of Noncritical register in instance

#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: n500_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW_offset         (0x0081) // Offset of Control register in instance

#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW__LOCAL_RING_width        2
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW__LOCAL_RING_position     63
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW__LOCAL_RING_get(x)       _BGQ_GET(2,63,x)
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW__LOCAL_RING_set(v)       _BGQ_SET(2,63,v)
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_LOW__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: n500_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_500_DCR__N500_INTERRUPTS_CONTROL_HIGH_offset         (0x0082) // Offset of Control register in instance
  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0091) // Offset of Control register in instance

#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define ND_500_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct nd_500_dcr { 
    /* 0x0000 */ uint64_t reset;
    /* 0x0001 */ uint64_t ctrl_stop;
    /* 0x0002 */ uint64_t ctrl_internal;
    /* 0x0003 */ uint64_t ctrl_clear0;
    /* 0x0004 */ uint64_t ctrl_coords;
    /* 0x0005 */ uint64_t ctrl_cutoffs;
    /* 0x0006 */ uint64_t ctrl_neighbor_coords;
    /* 0x0007 */ uint64_t ctrl_det_order;
    /* 0x0008 */ uint64_t ctrl_dynamic_zone01;
    /* 0x0009 */ uint64_t ctrl_dynamic_zone23;
    /* 0x000A */ uint64_t ctrl_timeout;
    /* 0x000B */ uint64_t ctrl_gi_class_00_01;
    /* 0x000C */ uint64_t ctrl_gi_class_02_03;
    /* 0x000D */ uint64_t ctrl_gi_class_04_05;
    /* 0x000E */ uint64_t ctrl_gi_class_06_07;
    /* 0x000F */ uint64_t ctrl_gi_class_08_09;
    /* 0x0010 */ uint64_t ctrl_gi_class_10_11;
    /* 0x0011 */ uint64_t ctrl_gi_class_12_13;
    /* 0x0012 */ uint64_t ctrl_gi_class_14_15;
    /* 0x0013 */ uint64_t ctrl_coll_class_00_01;
    /* 0x0014 */ uint64_t ctrl_coll_class_02_03;
    /* 0x0015 */ uint64_t ctrl_coll_class_04_05;
    /* 0x0016 */ uint64_t ctrl_coll_class_06_07;
    /* 0x0017 */ uint64_t ctrl_coll_class_08_09;
    /* 0x0018 */ uint64_t ctrl_coll_class_10_11;
    /* 0x0019 */ uint64_t ctrl_coll_class_12_13;
    /* 0x001A */ uint64_t ctrl_coll_class_14_15;
    /* 0x001B */ uint64_t fatal_err_enable[ND_500_DCR__FATAL_ERR_ENABLE_range];
    /* 0x0021 */ uint64_t non_fatal_err_enable[ND_500_DCR__NON_FATAL_ERR_ENABLE_range];
    /* 0x0023 */ uint64_t fatal_err0;
    /* 0x0024 */ uint64_t fatal_err1;
    /* 0x0025 */ uint64_t fatal_err2;
    /* 0x0026 */ uint64_t fatal_err3;
    /* 0x0027 */ uint64_t fatal_err4;
    /* 0x0028 */ uint64_t fatal_err5;
    /* 0x0029 */ uint64_t non_fatal_err0;
    /* 0x002A */ uint64_t non_fatal_err1;
    /* 0x002B */ uint64_t backdoor_ctrl;
    /* 0x002C */ uint64_t backdoor_wr;
    /* 0x002D */ uint64_t reserve_002D;
    /* 0x002E */ uint64_t backdoor_rd_data[ND_500_DCR__BACKDOOR_RD_DATA_range];
    /* 0x0030 */ uint64_t backdoor_rd_ecc;
    /* 0x0031 */ uint64_t stat0;
    /* 0x0032 */ uint64_t stat1;
    /* 0x0033 */ uint64_t stat2;
    /* 0x0034 */ uint64_t stat3;
    /* 0x0035 */ uint64_t stat4;
    /* 0x0036 */ uint64_t stat5;
    /* 0x0037 */ uint64_t stat6;
    /* 0x0038 */ uint64_t stat7;
    /* 0x0039 */ uint64_t stat8;
    /* 0x003A */ uint64_t stat9;
    /* 0x003B */ uint64_t gi_stat_re[ND_500_DCR__GI_STAT_RE_range];
    /* 0x0046 */ uint64_t ce_cnt[ND_500_DCR__CE_CNT_range];
    /* 0x004F */ uint64_t ce_threshold;
    /* 0x0050 */ uint64_t ctrl_clear1;
    /* 0x0051 */ uint64_t ctrl_reserved;
    /* 0x0052 */ uint64_t ctrl_upc_clear;
    /* 0x0053 */ uint64_t ctrl_upc_stop;
    /* 0x0054 */ uint64_t fatal_err_stat;
    /* 0x0055 */ uint64_t non_fatal_err_stat;
    /* 0x0056 */ uint64_t reserve_0056[0x000A];
    /* 0x0060 */ uint64_t inj_fifo_chksum[ND_500_DCR__INJ_FIFO_CHKSUM_range];
    /* 0x0070 */ uint64_t inj_fifo_head_p0;
    /* 0x0071 */ uint64_t inj_fifo_head_p1;
    /* 0x0072 */ uint64_t inj_fifo_tail_p0;
    /* 0x0073 */ uint64_t inj_fifo_tail_p1;
    /* 0x0074 */ uint64_t rcp_fifo_head_p0;
    /* 0x0075 */ uint64_t rcp_fifo_head_p1;
    /* 0x0076 */ uint64_t rcp_fifo_head_p2;
    /* 0x0077 */ uint64_t rcp_fifo_head_p3;
    /* 0x0078 */ uint64_t rcp_fifo_tail_p0;
    /* 0x0079 */ uint64_t rcp_fifo_tail_p1;
    /* 0x007A */ uint64_t rcp_fifo_tail_p2;
    /* 0x007B */ uint64_t rcp_fifo_tail_p3;
    /* 0x007C */ uint64_t reserve_007C[0x0004];
    /* 0x0080 */ uint64_t n500_interrupts__state;
    /* 0x0081 */ uint64_t n500_interrupts_control_low;
    /* 0x0082 */ uint64_t n500_interrupts_control_high;
    /* 0x0083 */ uint64_t n500_interrupts__force;
    /* 0x0084 */ uint64_t n500_interrupts__first;
    /* 0x0085 */ uint64_t n500_interrupts__machine_check;
    /* 0x0086 */ uint64_t n500_interrupts__critical;
    /* 0x0087 */ uint64_t n500_interrupts__noncritical;
    /* 0x0088 */ uint64_t reserve_0088[0x0008];
    /* 0x0090 */ uint64_t interrupt_internal_error__state;
    /* 0x0091 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0092 */ uint64_t interrupt_internal_error__force;
    /* 0x0093 */ uint64_t interrupt_internal_error__first;
    /* 0x0094 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0095 */ uint64_t interrupt_internal_error__critical;
    /* 0x0096 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0097 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0098 */ uint64_t interrupt_internal_error_hw_info;
} nd_500_dcr_t;
  
#define ND_500_DCR_USER_PTR _DCR_USER_PTR(nd_500_dcr_t, ND_500_DCR)
#define ND_500_DCR_PRIV_PTR _DCR_PRIV_PTR(nd_500_dcr_t, ND_500_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
