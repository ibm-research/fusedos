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

#ifndef   _ND_RESE_DCR__H_ // Prevent multiple inclusion
#define   _ND_RESE_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define ND_RESE_DCR_base    (0x168000)  // Base address for all instances
#define ND_RESE_DCR_size    (0x0059)    // Size of instance (largest offset + 1 )
#define ND_RESE_DCR_stride  (0x4000)    // Distance between instances
#define ND_RESE_DCR_num     (0x000B)    // Number of instances
  
#define ND_RESE_DCR__T0_base  (0x168000)  // Base address for instance T0
#define ND_RESE_DCR__T1_base  (0x16C000)  // Base address for instance T1
#define ND_RESE_DCR__T2_base  (0x170000)  // Base address for instance T2
#define ND_RESE_DCR__T3_base  (0x174000)  // Base address for instance T3
#define ND_RESE_DCR__T4_base  (0x178000)  // Base address for instance T4
#define ND_RESE_DCR__T5_base  (0x17C000)  // Base address for instance T5
#define ND_RESE_DCR__T6_base  (0x180000)  // Base address for instance T6
#define ND_RESE_DCR__T7_base  (0x184000)  // Base address for instance T7
#define ND_RESE_DCR__T8_base  (0x188000)  // Base address for instance T8
#define ND_RESE_DCR__T9_base  (0x18C000)  // Base address for instance T9
#define ND_RESE_DCR__IO_base  (0x190000)  // Base address for instance IO
  
#define ND_RESE_DCR(i,reg) _DCR_REG__INDEX(ND_RESE_DCR, i, reg)
  
#define ND_RESE_DCR__T0(reg) _DCR_REG__NAME(ND_RESE_DCR, T0, reg)
#define ND_RESE_DCR__T1(reg) _DCR_REG__NAME(ND_RESE_DCR, T1, reg)
#define ND_RESE_DCR__T2(reg) _DCR_REG__NAME(ND_RESE_DCR, T2, reg)
#define ND_RESE_DCR__T3(reg) _DCR_REG__NAME(ND_RESE_DCR, T3, reg)
#define ND_RESE_DCR__T4(reg) _DCR_REG__NAME(ND_RESE_DCR, T4, reg)
#define ND_RESE_DCR__T5(reg) _DCR_REG__NAME(ND_RESE_DCR, T5, reg)
#define ND_RESE_DCR__T6(reg) _DCR_REG__NAME(ND_RESE_DCR, T6, reg)
#define ND_RESE_DCR__T7(reg) _DCR_REG__NAME(ND_RESE_DCR, T7, reg)
#define ND_RESE_DCR__T8(reg) _DCR_REG__NAME(ND_RESE_DCR, T8, reg)
#define ND_RESE_DCR__T9(reg) _DCR_REG__NAME(ND_RESE_DCR, T9, reg)
#define ND_RESE_DCR__IO(reg) _DCR_REG__NAME(ND_RESE_DCR, IO, reg)
  
// Register:
//   Name:        reset
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       FFFF000000000000
//   Description: Reset individual RE/SE sub units
#define ND_RESE_DCR__RESET_offset  (0x0000) // Offset of register in instance 
#define ND_RESE_DCR__RESET_reset  UNSIGNED64(0xFFFF000000000000)

//   Field:       dcrs_out
//   Description: Reset all DCR registers except this one
#define ND_RESE_DCR__RESET__DCRS_OUT_width        1
#define ND_RESE_DCR__RESET__DCRS_OUT_position     0
#define ND_RESE_DCR__RESET__DCRS_OUT_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__RESET__DCRS_OUT_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__RESET__DCRS_OUT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       re
//   Description: Reset RE unit except VC fifos
#define ND_RESE_DCR__RESET__RE_width        1
#define ND_RESE_DCR__RESET__RE_position     1
#define ND_RESE_DCR__RESET__RE_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__RESET__RE_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__RESET__RE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       re_vc0
//   Description: Reset RE dynamic VC0
#define ND_RESE_DCR__RESET__RE_VC0_width        1
#define ND_RESE_DCR__RESET__RE_VC0_position     2
#define ND_RESE_DCR__RESET__RE_VC0_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__RESET__RE_VC0_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__RESET__RE_VC0_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       re_vc1
//   Description: Reset RE deterministic VC1
#define ND_RESE_DCR__RESET__RE_VC1_width        1
#define ND_RESE_DCR__RESET__RE_VC1_position     3
#define ND_RESE_DCR__RESET__RE_VC1_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__RESET__RE_VC1_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__RESET__RE_VC1_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       re_vc2
//   Description: Reset RE high priority VC2
#define ND_RESE_DCR__RESET__RE_VC2_width        1
#define ND_RESE_DCR__RESET__RE_VC2_position     4
#define ND_RESE_DCR__RESET__RE_VC2_get(x)       _BGQ_GET(1,4,x)
#define ND_RESE_DCR__RESET__RE_VC2_set(v)       _BGQ_SET(1,4,v)
#define ND_RESE_DCR__RESET__RE_VC2_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       re_vc3
//   Description: Reset RE system I/O VC3
#define ND_RESE_DCR__RESET__RE_VC3_width        1
#define ND_RESE_DCR__RESET__RE_VC3_position     5
#define ND_RESE_DCR__RESET__RE_VC3_get(x)       _BGQ_GET(1,5,x)
#define ND_RESE_DCR__RESET__RE_VC3_set(v)       _BGQ_SET(1,5,v)
#define ND_RESE_DCR__RESET__RE_VC3_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       re_vc4
//   Description: Reset RE collective VC0
#define ND_RESE_DCR__RESET__RE_VC4_width        1
#define ND_RESE_DCR__RESET__RE_VC4_position     6
#define ND_RESE_DCR__RESET__RE_VC4_get(x)       _BGQ_GET(1,6,x)
#define ND_RESE_DCR__RESET__RE_VC4_set(v)       _BGQ_SET(1,6,v)
#define ND_RESE_DCR__RESET__RE_VC4_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       re_vc5
//   Description: Reset RE collective VC1
#define ND_RESE_DCR__RESET__RE_VC5_width        1
#define ND_RESE_DCR__RESET__RE_VC5_position     7
#define ND_RESE_DCR__RESET__RE_VC5_get(x)       _BGQ_GET(1,7,x)
#define ND_RESE_DCR__RESET__RE_VC5_set(v)       _BGQ_SET(1,7,v)
#define ND_RESE_DCR__RESET__RE_VC5_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       se
//   Description: Reset SE unit except VC tokens
#define ND_RESE_DCR__RESET__SE_width        1
#define ND_RESE_DCR__RESET__SE_position     8
#define ND_RESE_DCR__RESET__SE_get(x)       _BGQ_GET(1,8,x)
#define ND_RESE_DCR__RESET__SE_set(v)       _BGQ_SET(1,8,v)
#define ND_RESE_DCR__RESET__SE_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       se_tok0
//   Description: Reset SE dynamic VC0 token
#define ND_RESE_DCR__RESET__SE_TOK0_width        1
#define ND_RESE_DCR__RESET__SE_TOK0_position     9
#define ND_RESE_DCR__RESET__SE_TOK0_get(x)       _BGQ_GET(1,9,x)
#define ND_RESE_DCR__RESET__SE_TOK0_set(v)       _BGQ_SET(1,9,v)
#define ND_RESE_DCR__RESET__SE_TOK0_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       se_tok1
//   Description: Reset SE deterministic VC1 token
#define ND_RESE_DCR__RESET__SE_TOK1_width        1
#define ND_RESE_DCR__RESET__SE_TOK1_position     10
#define ND_RESE_DCR__RESET__SE_TOK1_get(x)       _BGQ_GET(1,10,x)
#define ND_RESE_DCR__RESET__SE_TOK1_set(v)       _BGQ_SET(1,10,v)
#define ND_RESE_DCR__RESET__SE_TOK1_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       se_tok2
//   Description: Reset SE high priority VC2 token
#define ND_RESE_DCR__RESET__SE_TOK2_width        1
#define ND_RESE_DCR__RESET__SE_TOK2_position     11
#define ND_RESE_DCR__RESET__SE_TOK2_get(x)       _BGQ_GET(1,11,x)
#define ND_RESE_DCR__RESET__SE_TOK2_set(v)       _BGQ_SET(1,11,v)
#define ND_RESE_DCR__RESET__SE_TOK2_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       se_tok3
//   Description: Reset SE system I/O VC3 token
#define ND_RESE_DCR__RESET__SE_TOK3_width        1
#define ND_RESE_DCR__RESET__SE_TOK3_position     12
#define ND_RESE_DCR__RESET__SE_TOK3_get(x)       _BGQ_GET(1,12,x)
#define ND_RESE_DCR__RESET__SE_TOK3_set(v)       _BGQ_SET(1,12,v)
#define ND_RESE_DCR__RESET__SE_TOK3_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       se_tok4
//   Description: Reset SE collective VC0 token
#define ND_RESE_DCR__RESET__SE_TOK4_width        1
#define ND_RESE_DCR__RESET__SE_TOK4_position     13
#define ND_RESE_DCR__RESET__SE_TOK4_get(x)       _BGQ_GET(1,13,x)
#define ND_RESE_DCR__RESET__SE_TOK4_set(v)       _BGQ_SET(1,13,v)
#define ND_RESE_DCR__RESET__SE_TOK4_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       se_tok5
//   Description: Reset SE collective VC1 token
#define ND_RESE_DCR__RESET__SE_TOK5_width        1
#define ND_RESE_DCR__RESET__SE_TOK5_position     14
#define ND_RESE_DCR__RESET__SE_TOK5_get(x)       _BGQ_GET(1,14,x)
#define ND_RESE_DCR__RESET__SE_TOK5_set(v)       _BGQ_SET(1,14,v)
#define ND_RESE_DCR__RESET__SE_TOK5_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       reg_cnt
//   Description: Reset correctable error count
#define ND_RESE_DCR__RESET__REG_CNT_width        1
#define ND_RESE_DCR__RESET__REG_CNT_position     15
#define ND_RESE_DCR__RESET__REG_CNT_get(x)       _BGQ_GET(1,15,x)
#define ND_RESE_DCR__RESET__REG_CNT_set(v)       _BGQ_SET(1,15,v)
#define ND_RESE_DCR__RESET__REG_CNT_insert(x,v)  _BGQ_INSERT(1,15,x,v)

  
// Register:
//   Name:        ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: RE/SE unit control
#define ND_RESE_DCR__CTRL_offset  (0x0001) // Offset of register in instance 
#define ND_RESE_DCR__CTRL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       re_link_stop
//   Description: receiver link stop, shutoff incoming link from SerDes ports
#define ND_RESE_DCR__CTRL__RE_LINK_STOP_width        1
#define ND_RESE_DCR__CTRL__RE_LINK_STOP_position     0
#define ND_RESE_DCR__CTRL__RE_LINK_STOP_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__CTRL__RE_LINK_STOP_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__CTRL__RE_LINK_STOP_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       se_link_stop
//   Description: sender link stop, shutoff outgoing link, sending IDLEs to SerDes
#define ND_RESE_DCR__CTRL__SE_LINK_STOP_width        1
#define ND_RESE_DCR__CTRL__SE_LINK_STOP_position     1
#define ND_RESE_DCR__CTRL__SE_LINK_STOP_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__CTRL__SE_LINK_STOP_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__CTRL__SE_LINK_STOP_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       re_unit_stop
//   Description: receiver unit stop
#define ND_RESE_DCR__CTRL__RE_UNIT_STOP_width        1
#define ND_RESE_DCR__CTRL__RE_UNIT_STOP_position     2
#define ND_RESE_DCR__CTRL__RE_UNIT_STOP_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__CTRL__RE_UNIT_STOP_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__CTRL__RE_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       se_unit_stop
//   Description: sender unit stop
#define ND_RESE_DCR__CTRL__SE_UNIT_STOP_width        1
#define ND_RESE_DCR__CTRL__SE_UNIT_STOP_position     3
#define ND_RESE_DCR__CTRL__SE_UNIT_STOP_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__CTRL__SE_UNIT_STOP_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__CTRL__SE_UNIT_STOP_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       se_retrans_stop
//   Description: sender retransmission stop
#define ND_RESE_DCR__CTRL__SE_RETRANS_STOP_width        1
#define ND_RESE_DCR__CTRL__SE_RETRANS_STOP_position     4
#define ND_RESE_DCR__CTRL__SE_RETRANS_STOP_get(x)       _BGQ_GET(1,4,x)
#define ND_RESE_DCR__CTRL__SE_RETRANS_STOP_set(v)       _BGQ_SET(1,4,v)
#define ND_RESE_DCR__CTRL__SE_RETRANS_STOP_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       se_drop_pkt_usr
//   Description: sender drop user packets
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_width        1
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_position     5
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_get(x)       _BGQ_GET(1,5,x)
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_set(v)       _BGQ_SET(1,5,v)
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       se_drop_pkt_sys
//   Description: sender drop system packets
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_width        1
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_position     6
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_get(x)       _BGQ_GET(1,6,x)
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_set(v)       _BGQ_SET(1,6,v)
#define ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       re_disable_hdrchk
//   Description: receiver disable complex header checker
#define ND_RESE_DCR__CTRL__RE_DISABLE_HDRCHK_width        1
#define ND_RESE_DCR__CTRL__RE_DISABLE_HDRCHK_position     7
#define ND_RESE_DCR__CTRL__RE_DISABLE_HDRCHK_get(x)       _BGQ_GET(1,7,x)
#define ND_RESE_DCR__CTRL__RE_DISABLE_HDRCHK_set(v)       _BGQ_SET(1,7,v)
#define ND_RESE_DCR__CTRL__RE_DISABLE_HDRCHK_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       dyn_tok_reduce
//   Description: dynamic VC token reduce: bit 8 -- reduce 8 dynamic tokens, bit 9 -- reduce 16 tokens, bit 10 -- reduce 32 tokens, bit 11 -- reduce 64 tokens.  The total reduced is the sum of tokens from all programmed bits.  Maximum programmed value is 1011.  Two invalid patterns are 0111 and 1111
#define ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_width        4
#define ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_position     11
#define ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_get(x)       _BGQ_GET(4,11,x)
#define ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_set(v)       _BGQ_SET(4,11,v)
#define ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       bubble_tok_reduce
//   Description: bubble (deterministic) VC token reduce: bit 12 -- reduce 1 token, bit 13 -- reduce 2 tokens.  Programmed to 11 would reduce tokens by 3, from the default 7 to 4.  DD2 only
#define ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_width        2
#define ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_position     13
#define ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_get(x)       _BGQ_GET(2,13,x)
#define ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_set(v)       _BGQ_SET(2,13,v)
#define ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_insert(x,v)  _BGQ_INSERT(2,13,x,v)

//   Field:       coll_tok_reduce
//   Description: collective VC token reduce: bit 14 -- reduce 1 token, bit 15 -- reduce 2 tokens.  Programmed to 11 would reduce tokens by 3, from the default 7 to 4.  DD2 only
#define ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_width        2
#define ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_position     15
#define ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_get(x)       _BGQ_GET(2,15,x)
#define ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_set(v)       _BGQ_SET(2,15,v)
#define ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_insert(x,v)  _BGQ_INSERT(2,15,x,v)

//   Field:       gi_stop
//   Description: stop sending GI packets, DD2 only
#define ND_RESE_DCR__CTRL__GI_STOP_width        1
#define ND_RESE_DCR__CTRL__GI_STOP_position     16
#define ND_RESE_DCR__CTRL__GI_STOP_get(x)       _BGQ_GET(1,16,x)
#define ND_RESE_DCR__CTRL__GI_STOP_set(v)       _BGQ_SET(1,16,v)
#define ND_RESE_DCR__CTRL__GI_STOP_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       ran_coll_sys_sel
//   Description: DD2 only, randomize collective and system grants chicken switch (default). 1 is DD1 behavior
#define ND_RESE_DCR__CTRL__RAN_COLL_SYS_SEL_width        1
#define ND_RESE_DCR__CTRL__RAN_COLL_SYS_SEL_position     17
#define ND_RESE_DCR__CTRL__RAN_COLL_SYS_SEL_get(x)       _BGQ_GET(1,17,x)
#define ND_RESE_DCR__CTRL__RAN_COLL_SYS_SEL_set(v)       _BGQ_SET(1,17,v)
#define ND_RESE_DCR__CTRL__RAN_COLL_SYS_SEL_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       ran_coll_up_down_sel
//   Description: DD2 only, randomize collective up tree and down tree grants chicken switch (default). 1 is DD1 behavior
#define ND_RESE_DCR__CTRL__RAN_COLL_UP_DOWN_SEL_width        1
#define ND_RESE_DCR__CTRL__RAN_COLL_UP_DOWN_SEL_position     18
#define ND_RESE_DCR__CTRL__RAN_COLL_UP_DOWN_SEL_get(x)       _BGQ_GET(1,18,x)
#define ND_RESE_DCR__CTRL__RAN_COLL_UP_DOWN_SEL_set(v)       _BGQ_SET(1,18,v)
#define ND_RESE_DCR__CTRL__RAN_COLL_UP_DOWN_SEL_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       reserved
//   Description: reserved
#define ND_RESE_DCR__CTRL__RESERVED_width        45
#define ND_RESE_DCR__CTRL__RESERVED_position     63
#define ND_RESE_DCR__CTRL__RESERVED_get(x)       _BGQ_GET(45,63,x)
#define ND_RESE_DCR__CTRL__RESERVED_set(v)       _BGQ_SET(45,63,v)
#define ND_RESE_DCR__CTRL__RESERVED_insert(x,v)  _BGQ_INSERT(45,63,x,v)

  
// Register:
//   Name:        upc_ctrl
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       7008050270000000
//   Description: RE/SE unit upc control
#define ND_RESE_DCR__UPC_CTRL_offset  (0x0002) // Offset of register in instance 
#define ND_RESE_DCR__UPC_CTRL_reset  UNSIGNED64(0x7008050270000000)

//   Field:       upc0_vc_enable
//   Description: sender packet chunks upc counter 0 vc enable, 1 bit per vc, default is b1110000 = 0x70, which counts torus user packets, vc = 0, 1, 2
#define ND_RESE_DCR__UPC_CTRL__UPC0_VC_ENABLE_width        7
#define ND_RESE_DCR__UPC_CTRL__UPC0_VC_ENABLE_position     7
#define ND_RESE_DCR__UPC_CTRL__UPC0_VC_ENABLE_get(x)       _BGQ_GET(7,7,x)
#define ND_RESE_DCR__UPC_CTRL__UPC0_VC_ENABLE_set(v)       _BGQ_SET(7,7,v)
#define ND_RESE_DCR__UPC_CTRL__UPC0_VC_ENABLE_insert(x,v)  _BGQ_INSERT(7,7,x,v)

//   Field:       upc1_vc_enable
//   Description: sender packet chunks upc counter 1 vc enable, 1 bit per vc, default is b0001000 = 0x08, which counts torus system packets, vc = 3
#define ND_RESE_DCR__UPC_CTRL__UPC1_VC_ENABLE_width        7
#define ND_RESE_DCR__UPC_CTRL__UPC1_VC_ENABLE_position     15
#define ND_RESE_DCR__UPC_CTRL__UPC1_VC_ENABLE_get(x)       _BGQ_GET(7,15,x)
#define ND_RESE_DCR__UPC_CTRL__UPC1_VC_ENABLE_set(v)       _BGQ_SET(7,15,v)
#define ND_RESE_DCR__UPC_CTRL__UPC1_VC_ENABLE_insert(x,v)  _BGQ_INSERT(7,15,x,v)

//   Field:       upc2_vc_enable
//   Description: sender packet chunks upc counter 2 vc enable, 1 bit per vc, default is b0000101 = 0x05, which counts collective comm world packets, vc = 4, 6
#define ND_RESE_DCR__UPC_CTRL__UPC2_VC_ENABLE_width        7
#define ND_RESE_DCR__UPC_CTRL__UPC2_VC_ENABLE_position     23
#define ND_RESE_DCR__UPC_CTRL__UPC2_VC_ENABLE_get(x)       _BGQ_GET(7,23,x)
#define ND_RESE_DCR__UPC_CTRL__UPC2_VC_ENABLE_set(v)       _BGQ_SET(7,23,v)
#define ND_RESE_DCR__UPC_CTRL__UPC2_VC_ENABLE_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       upc3_vc_enable
//   Description: sender packet chunks upc counter 3 vc enable, 1 bit per vc, default is b0000010 = 0x02, which counts collective sub comm packets, vc = 5
#define ND_RESE_DCR__UPC_CTRL__UPC3_VC_ENABLE_width        7
#define ND_RESE_DCR__UPC_CTRL__UPC3_VC_ENABLE_position     31
#define ND_RESE_DCR__UPC_CTRL__UPC3_VC_ENABLE_get(x)       _BGQ_GET(7,31,x)
#define ND_RESE_DCR__UPC_CTRL__UPC3_VC_ENABLE_set(v)       _BGQ_SET(7,31,v)
#define ND_RESE_DCR__UPC_CTRL__UPC3_VC_ENABLE_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       upc45_vc_enable
//   Description: receiver total packets upc 4 and packets in upc 5's vc enable, 1 bit per vc, default is b1110000 = 0x70, which counts user torus packets, vc = 0, 1, 2
#define ND_RESE_DCR__UPC_CTRL__UPC45_VC_ENABLE_width        7
#define ND_RESE_DCR__UPC_CTRL__UPC45_VC_ENABLE_position     39
#define ND_RESE_DCR__UPC_CTRL__UPC45_VC_ENABLE_get(x)       _BGQ_GET(7,39,x)
#define ND_RESE_DCR__UPC_CTRL__UPC45_VC_ENABLE_set(v)       _BGQ_SET(7,39,v)
#define ND_RESE_DCR__UPC_CTRL__UPC45_VC_ENABLE_insert(x,v)  _BGQ_INSERT(7,39,x,v)

//   Field:       reserved
//   Description: upc control, reserved
#define ND_RESE_DCR__UPC_CTRL__RESERVED_width        24
#define ND_RESE_DCR__UPC_CTRL__RESERVED_position     63
#define ND_RESE_DCR__UPC_CTRL__RESERVED_get(x)       _BGQ_GET(24,63,x)
#define ND_RESE_DCR__UPC_CTRL__RESERVED_set(v)       _BGQ_SET(24,63,v)
#define ND_RESE_DCR__UPC_CTRL__RESERVED_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        thresh_array_ce
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: RE/SE array correctable error count threshold
#define ND_RESE_DCR__THRESH_ARRAY_CE_offset  (0x0003) // Offset of register in instance 
#define ND_RESE_DCR__THRESH_ARRAY_CE_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       value
//   Description: CE threshold
#define ND_RESE_DCR__THRESH_ARRAY_CE__VALUE_width        32
#define ND_RESE_DCR__THRESH_ARRAY_CE__VALUE_position     63
#define ND_RESE_DCR__THRESH_ARRAY_CE__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__THRESH_ARRAY_CE__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__THRESH_ARRAY_CE__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        thresh_re_err
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: RE link error count threshold
#define ND_RESE_DCR__THRESH_RE_ERR_offset  (0x0004) // Offset of register in instance 
#define ND_RESE_DCR__THRESH_RE_ERR_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       value
//   Description: RE link error threshold
#define ND_RESE_DCR__THRESH_RE_ERR__VALUE_width        32
#define ND_RESE_DCR__THRESH_RE_ERR__VALUE_position     63
#define ND_RESE_DCR__THRESH_RE_ERR__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__THRESH_RE_ERR__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__THRESH_RE_ERR__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        thresh_se_retran
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000FFFFFFFF
//   Description: SE retransmission count threshold
#define ND_RESE_DCR__THRESH_SE_RETRAN_offset  (0x0005) // Offset of register in instance 
#define ND_RESE_DCR__THRESH_SE_RETRAN_reset  UNSIGNED64(0x00000000FFFFFFFF)

//   Field:       value
//   Description: SE retransmission count threshold
#define ND_RESE_DCR__THRESH_SE_RETRAN__VALUE_width        32
#define ND_RESE_DCR__THRESH_SE_RETRAN__VALUE_position     63
#define ND_RESE_DCR__THRESH_SE_RETRAN__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__THRESH_SE_RETRAN__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__THRESH_SE_RETRAN__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        clear
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: RE/SE unit clear
#define ND_RESE_DCR__CLEAR_offset  (0x0006) // Offset of register in instance 
#define ND_RESE_DCR__CLEAR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       re_clr_link_crc_usr
//   Description: clear receiver link crc user
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_USR_width        1
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_USR_position     0
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_USR_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_USR_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_USR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       re_clr_link_crc_sys
//   Description: clear receiver link crc system
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_SYS_width        1
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_SYS_position     1
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_SYS_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_SYS_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_CRC_SYS_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       se_clr_link_crc_usr
//   Description: clear sender link crc user
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_USR_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_USR_position     2
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_USR_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_USR_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_USR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       se_clr_link_crc_sys
//   Description: clear sender link crc system
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_SYS_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_SYS_position     3
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_SYS_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_SYS_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_LINK_CRC_SYS_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       re_clr_link_err_cnt
//   Description: clear receiver link error count
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_ERR_CNT_width        1
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_ERR_CNT_position     4
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_ERR_CNT_get(x)       _BGQ_GET(1,4,x)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_ERR_CNT_set(v)       _BGQ_SET(1,4,v)
#define ND_RESE_DCR__CLEAR__RE_CLR_LINK_ERR_CNT_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       re_clr_ce_cnt
//   Description: clear receiver array ecc correctable error count
#define ND_RESE_DCR__CLEAR__RE_CLR_CE_CNT_width        8
#define ND_RESE_DCR__CLEAR__RE_CLR_CE_CNT_position     12
#define ND_RESE_DCR__CLEAR__RE_CLR_CE_CNT_get(x)       _BGQ_GET(8,12,x)
#define ND_RESE_DCR__CLEAR__RE_CLR_CE_CNT_set(v)       _BGQ_SET(8,12,v)
#define ND_RESE_DCR__CLEAR__RE_CLR_CE_CNT_insert(x,v)  _BGQ_INSERT(8,12,x,v)

//   Field:       re_clr_ue
//   Description: clear receiver array ecc uncorrectable error
#define ND_RESE_DCR__CLEAR__RE_CLR_UE_width        6
#define ND_RESE_DCR__CLEAR__RE_CLR_UE_position     18
#define ND_RESE_DCR__CLEAR__RE_CLR_UE_get(x)       _BGQ_GET(6,18,x)
#define ND_RESE_DCR__CLEAR__RE_CLR_UE_set(v)       _BGQ_SET(6,18,v)
#define ND_RESE_DCR__CLEAR__RE_CLR_UE_insert(x,v)  _BGQ_INSERT(6,18,x,v)

//   Field:       se_clr_retrans_cnt
//   Description: clear sender retransmission count
#define ND_RESE_DCR__CLEAR__SE_CLR_RETRANS_CNT_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_RETRANS_CNT_position     19
#define ND_RESE_DCR__CLEAR__SE_CLR_RETRANS_CNT_get(x)       _BGQ_GET(1,19,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_RETRANS_CNT_set(v)       _BGQ_SET(1,19,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_RETRANS_CNT_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       se_clr_ce_cnt
//   Description: clear sender ecc correctable error count
#define ND_RESE_DCR__CLEAR__SE_CLR_CE_CNT_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_CE_CNT_position     20
#define ND_RESE_DCR__CLEAR__SE_CLR_CE_CNT_get(x)       _BGQ_GET(1,20,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_CE_CNT_set(v)       _BGQ_SET(1,20,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_CE_CNT_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       se_clr_e2e_crc_err
//   Description: clear sender fixed crc error
#define ND_RESE_DCR__CLEAR__SE_CLR_E2E_CRC_ERR_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_E2E_CRC_ERR_position     21
#define ND_RESE_DCR__CLEAR__SE_CLR_E2E_CRC_ERR_get(x)       _BGQ_GET(1,21,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_E2E_CRC_ERR_set(v)       _BGQ_SET(1,21,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_E2E_CRC_ERR_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       se_clr_ue
//   Description: clear sender control uncorrectable ecc error
#define ND_RESE_DCR__CLEAR__SE_CLR_UE_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_UE_position     22
#define ND_RESE_DCR__CLEAR__SE_CLR_UE_get(x)       _BGQ_GET(1,22,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_UE_set(v)       _BGQ_SET(1,22,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_UE_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       reserved
//   Description: clear error conditions
#define ND_RESE_DCR__CLEAR__RESERVED_width        40
#define ND_RESE_DCR__CLEAR__RESERVED_position     62
#define ND_RESE_DCR__CLEAR__RESERVED_get(x)       _BGQ_GET(40,62,x)
#define ND_RESE_DCR__CLEAR__RESERVED_set(v)       _BGQ_SET(40,62,v)
#define ND_RESE_DCR__CLEAR__RESERVED_insert(x,v)  _BGQ_INSERT(40,62,x,v)

//   Field:       se_clr_dropped_pkt
//   Description: clear sender dropped packet error, this is a non fatal error
#define ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_width        1
#define ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_position     63
#define ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_get(x)       _BGQ_GET(1,63,x)
#define ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_set(v)       _BGQ_SET(1,63,v)
#define ND_RESE_DCR__CLEAR__SE_CLR_DROPPED_PKT_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        fatal_err
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: fatal errors, bit 0 to 61 are fatal errors that goes to nd_gip_int(0), bit 62 to 63 are non-fatal that goes to nd_gip_int(1)
#define ND_RESE_DCR__FATAL_ERR_offset  (0x0007) // Offset of register in instance 
#define ND_RESE_DCR__FATAL_ERR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       dcr_slave_mchk
//   Description: dcr slave machine check interrupt
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_MCHK_width        1
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_MCHK_position     0
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_MCHK_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_MCHK_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_MCHK_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       dcr_slave_crit
//   Description: dcr slave critical interrupt
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_CRIT_width        1
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_CRIT_position     1
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_CRIT_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_CRIT_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_CRIT_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       re_link_err_over_thresh
//   Description: receiver link error count over threshold
#define ND_RESE_DCR__FATAL_ERR__RE_LINK_ERR_OVER_THRESH_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_LINK_ERR_OVER_THRESH_position     2
#define ND_RESE_DCR__FATAL_ERR__RE_LINK_ERR_OVER_THRESH_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__FATAL_ERR__RE_LINK_ERR_OVER_THRESH_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__FATAL_ERR__RE_LINK_ERR_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       se_retrans_cnt_over_thresh
//   Description: sender retransmission count over threshold
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_position     3
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       re_ce_cnt_over_thresh
//   Description: receiver correctable error count over threshold.  bit 4 - vc0 array, 5 - vc1, 6 - vc2, 7 - vc3, 8 - vc4, 9 - vc5, 10 - vc0 pointer list, 11 - receiver pipe
#define ND_RESE_DCR__FATAL_ERR__RE_CE_CNT_OVER_THRESH_width        8
#define ND_RESE_DCR__FATAL_ERR__RE_CE_CNT_OVER_THRESH_position     11
#define ND_RESE_DCR__FATAL_ERR__RE_CE_CNT_OVER_THRESH_get(x)       _BGQ_GET(8,11,x)
#define ND_RESE_DCR__FATAL_ERR__RE_CE_CNT_OVER_THRESH_set(v)       _BGQ_SET(8,11,v)
#define ND_RESE_DCR__FATAL_ERR__RE_CE_CNT_OVER_THRESH_insert(x,v)  _BGQ_INSERT(8,11,x,v)

//   Field:       se_ce_cnt_over_thresh
//   Description: sender retransmission fifo array ce count over threshold
#define ND_RESE_DCR__FATAL_ERR__SE_CE_CNT_OVER_THRESH_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_CE_CNT_OVER_THRESH_position     12
#define ND_RESE_DCR__FATAL_ERR__SE_CE_CNT_OVER_THRESH_get(x)       _BGQ_GET(1,12,x)
#define ND_RESE_DCR__FATAL_ERR__SE_CE_CNT_OVER_THRESH_set(v)       _BGQ_SET(1,12,v)
#define ND_RESE_DCR__FATAL_ERR__SE_CE_CNT_OVER_THRESH_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       re_vc0_ue
//   Description: receiver vc0 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_UE_position     13
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_UE_get(x)       _BGQ_GET(1,13,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_UE_set(v)       _BGQ_SET(1,13,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_UE_insert(x,v)  _BGQ_INSERT(1,13,x,v)

//   Field:       re_vc1_ue
//   Description: receiver vc1 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_UE_position     14
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_UE_get(x)       _BGQ_GET(1,14,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_UE_set(v)       _BGQ_SET(1,14,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_UE_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       re_vc2_ue
//   Description: receiver vc2 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_UE_position     15
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_UE_get(x)       _BGQ_GET(1,15,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_UE_set(v)       _BGQ_SET(1,15,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_UE_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       re_vc3_ue
//   Description: receiver vc3 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_UE_position     16
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_UE_get(x)       _BGQ_GET(1,16,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_UE_set(v)       _BGQ_SET(1,16,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_UE_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       re_vc4_ue
//   Description: receiver vc4 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_UE_position     17
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_UE_get(x)       _BGQ_GET(1,17,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_UE_set(v)       _BGQ_SET(1,17,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_UE_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       re_vc5_ue
//   Description: receiver vc5 fifo read ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_UE_position     18
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_UE_get(x)       _BGQ_GET(1,18,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_UE_set(v)       _BGQ_SET(1,18,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_UE_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       re_pipe_ue
//   Description: receiver pipe data ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_UE_position     19
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_UE_get(x)       _BGQ_GET(1,19,x)
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_UE_set(v)       _BGQ_SET(1,19,v)
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_UE_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       re_pipe_perr
//   Description: receiver pipe internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_PERR_position     20
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_PERR_get(x)       _BGQ_GET(1,20,x)
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_PERR_set(v)       _BGQ_SET(1,20,v)
#define ND_RESE_DCR__FATAL_ERR__RE_PIPE_PERR_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       re_vc0_list_ue
//   Description: receiver vc0 list pointers ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_UE_position     21
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_UE_get(x)       _BGQ_GET(1,21,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_UE_set(v)       _BGQ_SET(1,21,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_UE_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       re_fifo_wr_perr
//   Description: receiver fifo write logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_FIFO_WR_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_FIFO_WR_PERR_position     22
#define ND_RESE_DCR__FATAL_ERR__RE_FIFO_WR_PERR_get(x)       _BGQ_GET(1,22,x)
#define ND_RESE_DCR__FATAL_ERR__RE_FIFO_WR_PERR_set(v)       _BGQ_SET(1,22,v)
#define ND_RESE_DCR__FATAL_ERR__RE_FIFO_WR_PERR_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       re_vc0_ctrl_perr
//   Description: receiver vc0 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_CTRL_PERR_position     23
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_CTRL_PERR_get(x)       _BGQ_GET(1,23,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_CTRL_PERR_set(v)       _BGQ_SET(1,23,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,23,x,v)

//   Field:       re_vc0_fifo_rd_perr
//   Description: receiver vc0 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_FIFO_RD_PERR_position     24
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_FIFO_RD_PERR_get(x)       _BGQ_GET(1,24,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_FIFO_RD_PERR_set(v)       _BGQ_SET(1,24,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       re_vc0_list_perr
//   Description: receiver vc0 list logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_PERR_position     25
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_PERR_get(x)       _BGQ_GET(1,25,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_PERR_set(v)       _BGQ_SET(1,25,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC0_LIST_PERR_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       re_vc1_ctrl_perr
//   Description: receiver vc1 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_CTRL_PERR_position     26
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_CTRL_PERR_get(x)       _BGQ_GET(1,26,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_CTRL_PERR_set(v)       _BGQ_SET(1,26,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       re_vc1_fifo_rd_perr
//   Description: receiver vc1 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_FIFO_RD_PERR_position     27
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_FIFO_RD_PERR_get(x)       _BGQ_GET(1,27,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_FIFO_RD_PERR_set(v)       _BGQ_SET(1,27,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC1_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       re_vc2_ctrl_perr
//   Description: receiver vc2 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_CTRL_PERR_position     28
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_CTRL_PERR_get(x)       _BGQ_GET(1,28,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_CTRL_PERR_set(v)       _BGQ_SET(1,28,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       re_vc2_fifo_rd_perr
//   Description: receiver vc2 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_FIFO_RD_PERR_position     29
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_FIFO_RD_PERR_get(x)       _BGQ_GET(1,29,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_FIFO_RD_PERR_set(v)       _BGQ_SET(1,29,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC2_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       re_vc3_ctrl_perr
//   Description: receiver vc3 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_CTRL_PERR_position     30
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_CTRL_PERR_get(x)       _BGQ_GET(1,30,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_CTRL_PERR_set(v)       _BGQ_SET(1,30,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       re_vc3_fifo_rd_perr
//   Description: receiver vc3 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_FIFO_RD_PERR_position     31
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_FIFO_RD_PERR_get(x)       _BGQ_GET(1,31,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_FIFO_RD_PERR_set(v)       _BGQ_SET(1,31,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC3_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       re_vc4_ctrl_perr
//   Description: receiver vc4 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_CTRL_PERR_position     32
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_CTRL_PERR_get(x)       _BGQ_GET(1,32,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_CTRL_PERR_set(v)       _BGQ_SET(1,32,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       re_vc4_fifo_rd_perr
//   Description: receiver vc4 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_FIFO_RD_PERR_position     33
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_FIFO_RD_PERR_get(x)       _BGQ_GET(1,33,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_FIFO_RD_PERR_set(v)       _BGQ_SET(1,33,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC4_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       re_vc5_ctrl_perr
//   Description: receiver vc5 control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_CTRL_PERR_position     34
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_CTRL_PERR_get(x)       _BGQ_GET(1,34,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_CTRL_PERR_set(v)       _BGQ_SET(1,34,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       re_vc5_fifo_rd_perr
//   Description: receiver vc5 fifo read logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_FIFO_RD_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_FIFO_RD_PERR_position     35
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_FIFO_RD_PERR_get(x)       _BGQ_GET(1,35,x)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_FIFO_RD_PERR_set(v)       _BGQ_SET(1,35,v)
#define ND_RESE_DCR__FATAL_ERR__RE_VC5_FIFO_RD_PERR_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       unused
//   Description: unused
#define ND_RESE_DCR__FATAL_ERR__UNUSED_width        1
#define ND_RESE_DCR__FATAL_ERR__UNUSED_position     36
#define ND_RESE_DCR__FATAL_ERR__UNUSED_get(x)       _BGQ_GET(1,36,x)
#define ND_RESE_DCR__FATAL_ERR__UNUSED_set(v)       _BGQ_SET(1,36,v)
#define ND_RESE_DCR__FATAL_ERR__UNUSED_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       se_e2e_crc_err
//   Description: sender input packet end-to-end crc error
#define ND_RESE_DCR__FATAL_ERR__SE_E2E_CRC_ERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_E2E_CRC_ERR_position     37
#define ND_RESE_DCR__FATAL_ERR__SE_E2E_CRC_ERR_get(x)       _BGQ_GET(1,37,x)
#define ND_RESE_DCR__FATAL_ERR__SE_E2E_CRC_ERR_set(v)       _BGQ_SET(1,37,v)
#define ND_RESE_DCR__FATAL_ERR__SE_E2E_CRC_ERR_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       se_data_in_ue
//   Description: sender input dbus ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__SE_DATA_IN_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_DATA_IN_UE_position     38
#define ND_RESE_DCR__FATAL_ERR__SE_DATA_IN_UE_get(x)       _BGQ_GET(1,38,x)
#define ND_RESE_DCR__FATAL_ERR__SE_DATA_IN_UE_set(v)       _BGQ_SET(1,38,v)
#define ND_RESE_DCR__FATAL_ERR__SE_DATA_IN_UE_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       se_token_overflow
//   Description: sender token overflow
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_position     39
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_get(x)       _BGQ_GET(1,39,x)
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_set(v)       _BGQ_SET(1,39,v)
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       se_token_parity_err
//   Description: sender token parity error
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_PARITY_ERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_PARITY_ERR_position     40
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_PARITY_ERR_get(x)       _BGQ_GET(1,40,x)
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_PARITY_ERR_set(v)       _BGQ_SET(1,40,v)
#define ND_RESE_DCR__FATAL_ERR__SE_TOKEN_PARITY_ERR_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       se_retrans_array_ue
//   Description: sender retransmission fifo array ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_ARRAY_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_ARRAY_UE_position     41
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_ARRAY_UE_get(x)       _BGQ_GET(1,41,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_ARRAY_UE_set(v)       _BGQ_SET(1,41,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_ARRAY_UE_insert(x,v)  _BGQ_INSERT(1,41,x,v)

//   Field:       se_retrans_pkt_type_err
//   Description: sender retransmission fifo packet type error on read
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_PKT_TYPE_ERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_PKT_TYPE_ERR_position     42
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_PKT_TYPE_ERR_get(x)       _BGQ_GET(1,42,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_PKT_TYPE_ERR_set(v)       _BGQ_SET(1,42,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_PKT_TYPE_ERR_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       se_retrans_state_in_perr
//   Description: sender retransmission fifo write state machine parity error
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_IN_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_IN_PERR_position     43
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_IN_PERR_get(x)       _BGQ_GET(1,43,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_IN_PERR_set(v)       _BGQ_SET(1,43,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_IN_PERR_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       se_retrans_state_out_perr
//   Description: sender retransmission fifo read state machine parity error
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_OUT_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_OUT_PERR_position     44
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_OUT_PERR_get(x)       _BGQ_GET(1,44,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_OUT_PERR_set(v)       _BGQ_SET(1,44,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_STATE_OUT_PERR_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       se_retrans_pointer_perr
//   Description: sender retransmission fifo pointer parity error
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_POINTER_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_POINTER_PERR_position     45
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_POINTER_PERR_get(x)       _BGQ_GET(1,45,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_POINTER_PERR_set(v)       _BGQ_SET(1,45,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_POINTER_PERR_insert(x,v)  _BGQ_INSERT(1,45,x,v)

//   Field:       se_retrans_fifo_overflow
//   Description: sender retransmission fifo overflow
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_FIFO_OVERFLOW_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_FIFO_OVERFLOW_position     46
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_FIFO_OVERFLOW_get(x)       _BGQ_GET(1,46,x)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_FIFO_OVERFLOW_set(v)       _BGQ_SET(1,46,v)
#define ND_RESE_DCR__FATAL_ERR__SE_RETRANS_FIFO_OVERFLOW_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       se_gi_ue
//   Description: sender gi unit ecc uncorrectable error
#define ND_RESE_DCR__FATAL_ERR__SE_GI_UE_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_GI_UE_position     47
#define ND_RESE_DCR__FATAL_ERR__SE_GI_UE_get(x)       _BGQ_GET(1,47,x)
#define ND_RESE_DCR__FATAL_ERR__SE_GI_UE_set(v)       _BGQ_SET(1,47,v)
#define ND_RESE_DCR__FATAL_ERR__SE_GI_UE_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       se_arb_perr
//   Description: sender arbiter internal parity error
#define ND_RESE_DCR__FATAL_ERR__SE_ARB_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_ARB_PERR_position     48
#define ND_RESE_DCR__FATAL_ERR__SE_ARB_PERR_get(x)       _BGQ_GET(1,48,x)
#define ND_RESE_DCR__FATAL_ERR__SE_ARB_PERR_set(v)       _BGQ_SET(1,48,v)
#define ND_RESE_DCR__FATAL_ERR__SE_ARB_PERR_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       se_ctrl_perr
//   Description: sender control logic internal parity error
#define ND_RESE_DCR__FATAL_ERR__SE_CTRL_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_CTRL_PERR_position     49
#define ND_RESE_DCR__FATAL_ERR__SE_CTRL_PERR_get(x)       _BGQ_GET(1,49,x)
#define ND_RESE_DCR__FATAL_ERR__SE_CTRL_PERR_set(v)       _BGQ_SET(1,49,v)
#define ND_RESE_DCR__FATAL_ERR__SE_CTRL_PERR_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       se_tok_rels_perr
//   Description: sender token release parity error
#define ND_RESE_DCR__FATAL_ERR__SE_TOK_RELS_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_TOK_RELS_PERR_position     50
#define ND_RESE_DCR__FATAL_ERR__SE_TOK_RELS_PERR_get(x)       _BGQ_GET(1,50,x)
#define ND_RESE_DCR__FATAL_ERR__SE_TOK_RELS_PERR_set(v)       _BGQ_SET(1,50,v)
#define ND_RESE_DCR__FATAL_ERR__SE_TOK_RELS_PERR_insert(x,v)  _BGQ_INSERT(1,50,x,v)

//   Field:       dcr_pipe_perr
//   Description: dcr pipeline latch parity error
#define ND_RESE_DCR__FATAL_ERR__DCR_PIPE_PERR_width        1
#define ND_RESE_DCR__FATAL_ERR__DCR_PIPE_PERR_position     52
#define ND_RESE_DCR__FATAL_ERR__DCR_PIPE_PERR_get(x)       _BGQ_GET(1,52,x)
#define ND_RESE_DCR__FATAL_ERR__DCR_PIPE_PERR_set(v)       _BGQ_SET(1,52,v)
#define ND_RESE_DCR__FATAL_ERR__DCR_PIPE_PERR_insert(x,v)  _BGQ_INSERT(1,52,x,v)

//   Field:       reserved
//   Description: reserved
#define ND_RESE_DCR__FATAL_ERR__RESERVED_width        9
#define ND_RESE_DCR__FATAL_ERR__RESERVED_position     61
#define ND_RESE_DCR__FATAL_ERR__RESERVED_get(x)       _BGQ_GET(9,61,x)
#define ND_RESE_DCR__FATAL_ERR__RESERVED_set(v)       _BGQ_SET(9,61,v)
#define ND_RESE_DCR__FATAL_ERR__RESERVED_insert(x,v)  _BGQ_INSERT(9,61,x,v)

//   Field:       dcr_slave_non_crit
//   Description: dcr slave non critical interrupt
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_NON_CRIT_width        1
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_NON_CRIT_position     62
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_NON_CRIT_get(x)       _BGQ_GET(1,62,x)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_NON_CRIT_set(v)       _BGQ_SET(1,62,v)
#define ND_RESE_DCR__FATAL_ERR__DCR_SLAVE_NON_CRIT_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       se_dropped_pkt
//   Description: sender dropped packets
#define ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_width        1
#define ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_position     63
#define ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_get(x)       _BGQ_GET(1,63,x)
#define ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_set(v)       _BGQ_SET(1,63,v)
#define ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        fatal_err_enable
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: fatal error enable
#define ND_RESE_DCR__FATAL_ERR_ENABLE_offset  (0x0008) // Offset of register in instance 
#define ND_RESE_DCR__FATAL_ERR_ENABLE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: fatal error enable, see corresponding bits in fatal errors 
#define ND_RESE_DCR__FATAL_ERR_ENABLE__VALUE_width        64
#define ND_RESE_DCR__FATAL_ERR_ENABLE__VALUE_position     63
#define ND_RESE_DCR__FATAL_ERR_ENABLE__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__FATAL_ERR_ENABLE__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__FATAL_ERR_ENABLE__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        re_link_err_cnt
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver link error count
#define ND_RESE_DCR__RE_LINK_ERR_CNT_offset  (0x0009) // Offset of register in instance 
#define ND_RESE_DCR__RE_LINK_ERR_CNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver link error count
#define ND_RESE_DCR__RE_LINK_ERR_CNT__VALUE_width        32
#define ND_RESE_DCR__RE_LINK_ERR_CNT__VALUE_position     63
#define ND_RESE_DCR__RE_LINK_ERR_CNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__RE_LINK_ERR_CNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__RE_LINK_ERR_CNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        se_retrans_cnt
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender retransmission count
#define ND_RESE_DCR__SE_RETRANS_CNT_offset  (0x000A) // Offset of register in instance 
#define ND_RESE_DCR__SE_RETRANS_CNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: sender retransmission count
#define ND_RESE_DCR__SE_RETRANS_CNT__VALUE_width        32
#define ND_RESE_DCR__SE_RETRANS_CNT__VALUE_position     63
#define ND_RESE_DCR__SE_RETRANS_CNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__SE_RETRANS_CNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__SE_RETRANS_CNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        ce_count
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: correctable error count n
#define ND_RESE_DCR__CE_COUNT_offset  (0x000B) // Offset of register in instance 
#define ND_RESE_DCR__CE_COUNT_range  (0x9) // Range of external address space
#define ND_RESE_DCR__CE_COUNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver vc0 RF2 array ce count
#define ND_RESE_DCR__CE_COUNT__VALUE_width        32
#define ND_RESE_DCR__CE_COUNT__VALUE_position     63
#define ND_RESE_DCR__CE_COUNT__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__CE_COUNT__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__CE_COUNT__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        upc
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: performance count n
#define ND_RESE_DCR__UPC_offset  (0x0014) // Offset of register in instance 
#define ND_RESE_DCR__UPC_range  (0x6) // Range of external address space
#define ND_RESE_DCR__UPC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: sender upc count 0, see upc_ctrl for how to config
#define ND_RESE_DCR__UPC__VALUE_width        64
#define ND_RESE_DCR__UPC__VALUE_position     63
#define ND_RESE_DCR__UPC__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__UPC__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__UPC__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_ctl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor control
#define ND_RESE_DCR__BACKDOOR_CTL_offset  (0x0020) // Offset of register in instance 
#define ND_RESE_DCR__BACKDOOR_CTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       coll_class_remap_en
//   Description: collective class remap enable, 11 to enable
#define ND_RESE_DCR__BACKDOOR_CTL__COLL_CLASS_REMAP_EN_width        2
#define ND_RESE_DCR__BACKDOOR_CTL__COLL_CLASS_REMAP_EN_position     3
#define ND_RESE_DCR__BACKDOOR_CTL__COLL_CLASS_REMAP_EN_get(x)       _BGQ_GET(2,3,x)
#define ND_RESE_DCR__BACKDOOR_CTL__COLL_CLASS_REMAP_EN_set(v)       _BGQ_SET(2,3,v)
#define ND_RESE_DCR__BACKDOOR_CTL__COLL_CLASS_REMAP_EN_insert(x,v)  _BGQ_INSERT(2,3,x,v)

//   Field:       re_vc0_list_sel
//   Description: receiver vc0 list read select
#define ND_RESE_DCR__BACKDOOR_CTL__RE_VC0_LIST_SEL_width        7
#define ND_RESE_DCR__BACKDOOR_CTL__RE_VC0_LIST_SEL_position     23
#define ND_RESE_DCR__BACKDOOR_CTL__RE_VC0_LIST_SEL_get(x)       _BGQ_GET(7,23,x)
#define ND_RESE_DCR__BACKDOOR_CTL__RE_VC0_LIST_SEL_set(v)       _BGQ_SET(7,23,v)
#define ND_RESE_DCR__BACKDOOR_CTL__RE_VC0_LIST_SEL_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       en
//   Description: array backdoor access enable, 11 to enable
#define ND_RESE_DCR__BACKDOOR_CTL__EN_width        2
#define ND_RESE_DCR__BACKDOOR_CTL__EN_position     33
#define ND_RESE_DCR__BACKDOOR_CTL__EN_get(x)       _BGQ_GET(2,33,x)
#define ND_RESE_DCR__BACKDOOR_CTL__EN_set(v)       _BGQ_SET(2,33,v)
#define ND_RESE_DCR__BACKDOOR_CTL__EN_insert(x,v)  _BGQ_INSERT(2,33,x,v)

//   Field:       write
//   Description: array backdoor access write
#define ND_RESE_DCR__BACKDOOR_CTL__WRITE_width        1
#define ND_RESE_DCR__BACKDOOR_CTL__WRITE_position     34
#define ND_RESE_DCR__BACKDOOR_CTL__WRITE_get(x)       _BGQ_GET(1,34,x)
#define ND_RESE_DCR__BACKDOOR_CTL__WRITE_set(v)       _BGQ_SET(1,34,v)
#define ND_RESE_DCR__BACKDOOR_CTL__WRITE_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       read
//   Description: array backdoor access read
#define ND_RESE_DCR__BACKDOOR_CTL__READ_width        1
#define ND_RESE_DCR__BACKDOOR_CTL__READ_position     35
#define ND_RESE_DCR__BACKDOOR_CTL__READ_get(x)       _BGQ_GET(1,35,x)
#define ND_RESE_DCR__BACKDOOR_CTL__READ_set(v)       _BGQ_SET(1,35,v)
#define ND_RESE_DCR__BACKDOOR_CTL__READ_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       array_sel
//   Description: backdoor array select, 0000 to 0101 to select receiver vc, 1000 sender retransmission fifo.
#define ND_RESE_DCR__BACKDOOR_CTL__ARRAY_SEL_width        4
#define ND_RESE_DCR__BACKDOOR_CTL__ARRAY_SEL_position     39
#define ND_RESE_DCR__BACKDOOR_CTL__ARRAY_SEL_get(x)       _BGQ_GET(4,39,x)
#define ND_RESE_DCR__BACKDOOR_CTL__ARRAY_SEL_set(v)       _BGQ_SET(4,39,v)
#define ND_RESE_DCR__BACKDOOR_CTL__ARRAY_SEL_insert(x,v)  _BGQ_INSERT(4,39,x,v)

//   Field:       address
//   Description: backdoor access address
#define ND_RESE_DCR__BACKDOOR_CTL__ADDRESS_width        16
#define ND_RESE_DCR__BACKDOOR_CTL__ADDRESS_position     63
#define ND_RESE_DCR__BACKDOOR_CTL__ADDRESS_get(x)       _BGQ_GET(16,63,x)
#define ND_RESE_DCR__BACKDOOR_CTL__ADDRESS_set(v)       _BGQ_SET(16,63,v)
#define ND_RESE_DCR__BACKDOOR_CTL__ADDRESS_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        coll_class_remap_vec
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: collective class remap vector
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC_offset  (0x0021) // Offset of register in instance 
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: coll class remap, every 4 bits corresponds to a new class for an input class value
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC__VALUE_width        64
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC__VALUE_position     63
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__COLL_CLASS_REMAP_VEC__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_wr_data
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor write data
#define ND_RESE_DCR__BACKDOOR_WR_DATA_offset  (0x0022) // Offset of register in instance 
#define ND_RESE_DCR__BACKDOOR_WR_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor write data 
#define ND_RESE_DCR__BACKDOOR_WR_DATA__VALUE_width        64
#define ND_RESE_DCR__BACKDOOR_WR_DATA__VALUE_position     63
#define ND_RESE_DCR__BACKDOOR_WR_DATA__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__BACKDOOR_WR_DATA__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__BACKDOOR_WR_DATA__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_wr_ecc
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor write ecc
#define ND_RESE_DCR__BACKDOOR_WR_ECC_offset  (0x0023) // Offset of register in instance 
#define ND_RESE_DCR__BACKDOOR_WR_ECC_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor write ecc 
#define ND_RESE_DCR__BACKDOOR_WR_ECC__VALUE_width        16
#define ND_RESE_DCR__BACKDOOR_WR_ECC__VALUE_position     63
#define ND_RESE_DCR__BACKDOOR_WR_ECC__VALUE_get(x)       _BGQ_GET(16,63,x)
#define ND_RESE_DCR__BACKDOOR_WR_ECC__VALUE_set(v)       _BGQ_SET(16,63,v)
#define ND_RESE_DCR__BACKDOOR_WR_ECC__VALUE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        backdoor_rd_data
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: backdoor read data
#define ND_RESE_DCR__BACKDOOR_RD_DATA_offset  (0x0024) // Offset of register in instance 
#define ND_RESE_DCR__BACKDOOR_RD_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: backdoor read data
#define ND_RESE_DCR__BACKDOOR_RD_DATA__VALUE_width        64
#define ND_RESE_DCR__BACKDOOR_RD_DATA__VALUE_position     63
#define ND_RESE_DCR__BACKDOOR_RD_DATA__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__BACKDOOR_RD_DATA__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__BACKDOOR_RD_DATA__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backdoor_rd
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: backdoor read
#define ND_RESE_DCR__BACKDOOR_RD_offset  (0x0025) // Offset of register in instance 
#define ND_RESE_DCR__BACKDOOR_RD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       list_used
//   Description: vc0 list used
#define ND_RESE_DCR__BACKDOOR_RD__LIST_USED_width        1
#define ND_RESE_DCR__BACKDOOR_RD__LIST_USED_position     8
#define ND_RESE_DCR__BACKDOOR_RD__LIST_USED_get(x)       _BGQ_GET(1,8,x)
#define ND_RESE_DCR__BACKDOOR_RD__LIST_USED_set(v)       _BGQ_SET(1,8,v)
#define ND_RESE_DCR__BACKDOOR_RD__LIST_USED_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       list_pointer
//   Description: vc0 list, 5 bit ecc + 7 bit pointer
#define ND_RESE_DCR__BACKDOOR_RD__LIST_POINTER_width        12
#define ND_RESE_DCR__BACKDOOR_RD__LIST_POINTER_position     31
#define ND_RESE_DCR__BACKDOOR_RD__LIST_POINTER_get(x)       _BGQ_GET(12,31,x)
#define ND_RESE_DCR__BACKDOOR_RD__LIST_POINTER_set(v)       _BGQ_SET(12,31,v)
#define ND_RESE_DCR__BACKDOOR_RD__LIST_POINTER_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       ecc
//   Description: backdoor read ecc
#define ND_RESE_DCR__BACKDOOR_RD__ECC_width        16
#define ND_RESE_DCR__BACKDOOR_RD__ECC_position     63
#define ND_RESE_DCR__BACKDOOR_RD__ECC_get(x)       _BGQ_GET(16,63,x)
#define ND_RESE_DCR__BACKDOOR_RD__ECC_set(v)       _BGQ_SET(16,63,v)
#define ND_RESE_DCR__BACKDOOR_RD__ECC_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        se_gi_data
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender gi data
#define ND_RESE_DCR__SE_GI_DATA_offset  (0x0026) // Offset of register in instance 
#define ND_RESE_DCR__SE_GI_DATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: sender gi data, every 4 bit field contains a bit 0 plus 3 bits of GI data for a class
#define ND_RESE_DCR__SE_GI_DATA__VALUE_width        32
#define ND_RESE_DCR__SE_GI_DATA__VALUE_position     63
#define ND_RESE_DCR__SE_GI_DATA__VALUE_get(x)       _BGQ_GET(32,63,x)
#define ND_RESE_DCR__SE_GI_DATA__VALUE_set(v)       _BGQ_SET(32,63,v)
#define ND_RESE_DCR__SE_GI_DATA__VALUE_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        re_link_crc_usr
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver link crc user
#define ND_RESE_DCR__RE_LINK_CRC_USR_offset  (0x0027) // Offset of register in instance 
#define ND_RESE_DCR__RE_LINK_CRC_USR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver link crc user
#define ND_RESE_DCR__RE_LINK_CRC_USR__VALUE_width        64
#define ND_RESE_DCR__RE_LINK_CRC_USR__VALUE_position     63
#define ND_RESE_DCR__RE_LINK_CRC_USR__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__RE_LINK_CRC_USR__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__RE_LINK_CRC_USR__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        re_link_crc_sys
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver link crc system
#define ND_RESE_DCR__RE_LINK_CRC_SYS_offset  (0x0028) // Offset of register in instance 
#define ND_RESE_DCR__RE_LINK_CRC_SYS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver link crc system
#define ND_RESE_DCR__RE_LINK_CRC_SYS__VALUE_width        64
#define ND_RESE_DCR__RE_LINK_CRC_SYS__VALUE_position     63
#define ND_RESE_DCR__RE_LINK_CRC_SYS__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__RE_LINK_CRC_SYS__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__RE_LINK_CRC_SYS__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        se_link_crc_usr
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender link crc user
#define ND_RESE_DCR__SE_LINK_CRC_USR_offset  (0x0029) // Offset of register in instance 
#define ND_RESE_DCR__SE_LINK_CRC_USR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver link crc user
#define ND_RESE_DCR__SE_LINK_CRC_USR__VALUE_width        64
#define ND_RESE_DCR__SE_LINK_CRC_USR__VALUE_position     63
#define ND_RESE_DCR__SE_LINK_CRC_USR__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__SE_LINK_CRC_USR__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__SE_LINK_CRC_USR__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        se_link_crc_sys
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender link crc system
#define ND_RESE_DCR__SE_LINK_CRC_SYS_offset  (0x002A) // Offset of register in instance 
#define ND_RESE_DCR__SE_LINK_CRC_SYS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       value
//   Description: receiver link crc system
#define ND_RESE_DCR__SE_LINK_CRC_SYS__VALUE_width        64
#define ND_RESE_DCR__SE_LINK_CRC_SYS__VALUE_position     63
#define ND_RESE_DCR__SE_LINK_CRC_SYS__VALUE_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__SE_LINK_CRC_SYS__VALUE_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__SE_LINK_CRC_SYS__VALUE_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        re_internal_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver internal state
#define ND_RESE_DCR__RE_INTERNAL_STATE_offset  (0x0030) // Offset of register in instance 
#define ND_RESE_DCR__RE_INTERNAL_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       re_pipe_state
//   Description: receiver input pipe state machine state.  DD2 only.  DD1 tied to 0
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_PIPE_STATE_width        6
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_PIPE_STATE_position     7
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_PIPE_STATE_get(x)       _BGQ_GET(6,7,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_PIPE_STATE_set(v)       _BGQ_SET(6,7,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_PIPE_STATE_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       re_ts_arb_state
//   Description: receiver torus traffic arbiter state machine state
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_TS_ARB_STATE_width        4
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_TS_ARB_STATE_position     11
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_TS_ARB_STATE_get(x)       _BGQ_GET(4,11,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_TS_ARB_STATE_set(v)       _BGQ_SET(4,11,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_TS_ARB_STATE_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       re_byp_arb_state
//   Description: receiver bypass arbiter state machine state
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_BYP_ARB_STATE_width        3
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_BYP_ARB_STATE_position     15
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_BYP_ARB_STATE_get(x)       _BGQ_GET(3,15,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_BYP_ARB_STATE_set(v)       _BGQ_SET(3,15,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_BYP_ARB_STATE_insert(x,v)  _BGQ_INSERT(3,15,x,v)

//   Field:       re_fifo_ctrl_state
//   Description: receiver vc0 - vc3 fifo write control state machine state
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_STATE_width        3
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_STATE_position     19
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_STATE_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_STATE_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_STATE_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       re_fifo_ctrl_coll_state
//   Description: receiver collective fifo write control state machine state
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_COLL_STATE_width        3
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_COLL_STATE_position     23
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_COLL_STATE_get(x)       _BGQ_GET(3,23,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_COLL_STATE_set(v)       _BGQ_SET(3,23,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__RE_FIFO_CTRL_COLL_STATE_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       value
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_INTERNAL_STATE__VALUE_width        40
#define ND_RESE_DCR__RE_INTERNAL_STATE__VALUE_position     63
#define ND_RESE_DCR__RE_INTERNAL_STATE__VALUE_get(x)       _BGQ_GET(40,63,x)
#define ND_RESE_DCR__RE_INTERNAL_STATE__VALUE_set(v)       _BGQ_SET(40,63,v)
#define ND_RESE_DCR__RE_INTERNAL_STATE__VALUE_insert(x,v)  _BGQ_INSERT(40,63,x,v)

  
// Register:
//   Name:        re_vc0_head
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc0 head pointer, not used and tied to 0 on the I/O link receiver
#define ND_RESE_DCR__RE_VC0_HEAD_offset  (0x0031) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC0_HEAD_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC0_HEAD__RESERVED_width        8
#define ND_RESE_DCR__RE_VC0_HEAD__RESERVED_position     7
#define ND_RESE_DCR__RE_VC0_HEAD__RESERVED_get(x)       _BGQ_GET(8,7,x)
#define ND_RESE_DCR__RE_VC0_HEAD__RESERVED_set(v)       _BGQ_SET(8,7,v)
#define ND_RESE_DCR__RE_VC0_HEAD__RESERVED_insert(x,v)  _BGQ_INSERT(8,7,x,v)

//   Field:       que0
//   Description: receiver vc0 que0 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE0_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE0_position     15
#define ND_RESE_DCR__RE_VC0_HEAD__QUE0_get(x)       _BGQ_GET(7,15,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE0_set(v)       _BGQ_SET(7,15,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE0_insert(x,v)  _BGQ_INSERT(7,15,x,v)

//   Field:       que1
//   Description: receiver vc0 que1 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE1_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE1_position     23
#define ND_RESE_DCR__RE_VC0_HEAD__QUE1_get(x)       _BGQ_GET(7,23,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE1_set(v)       _BGQ_SET(7,23,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE1_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       que2
//   Description: receiver vc0 que2 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE2_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE2_position     31
#define ND_RESE_DCR__RE_VC0_HEAD__QUE2_get(x)       _BGQ_GET(7,31,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE2_set(v)       _BGQ_SET(7,31,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE2_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       que3
//   Description: receiver vc0 que3 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE3_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE3_position     39
#define ND_RESE_DCR__RE_VC0_HEAD__QUE3_get(x)       _BGQ_GET(7,39,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE3_set(v)       _BGQ_SET(7,39,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE3_insert(x,v)  _BGQ_INSERT(7,39,x,v)

//   Field:       que4
//   Description: receiver vc0 que4 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE4_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE4_position     47
#define ND_RESE_DCR__RE_VC0_HEAD__QUE4_get(x)       _BGQ_GET(7,47,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE4_set(v)       _BGQ_SET(7,47,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE4_insert(x,v)  _BGQ_INSERT(7,47,x,v)

//   Field:       que5
//   Description: receiver vc0 que5 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE5_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE5_position     55
#define ND_RESE_DCR__RE_VC0_HEAD__QUE5_get(x)       _BGQ_GET(7,55,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE5_set(v)       _BGQ_SET(7,55,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE5_insert(x,v)  _BGQ_INSERT(7,55,x,v)

//   Field:       que6
//   Description: receiver vc0 que6 head pointer
#define ND_RESE_DCR__RE_VC0_HEAD__QUE6_width        7
#define ND_RESE_DCR__RE_VC0_HEAD__QUE6_position     63
#define ND_RESE_DCR__RE_VC0_HEAD__QUE6_get(x)       _BGQ_GET(7,63,x)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE6_set(v)       _BGQ_SET(7,63,v)
#define ND_RESE_DCR__RE_VC0_HEAD__QUE6_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        re_vc0_tail
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc0 tail pointer, not used and tied to 0 on the I/O link receiver
#define ND_RESE_DCR__RE_VC0_TAIL_offset  (0x0032) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC0_TAIL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC0_TAIL__RESERVED_width        8
#define ND_RESE_DCR__RE_VC0_TAIL__RESERVED_position     7
#define ND_RESE_DCR__RE_VC0_TAIL__RESERVED_get(x)       _BGQ_GET(8,7,x)
#define ND_RESE_DCR__RE_VC0_TAIL__RESERVED_set(v)       _BGQ_SET(8,7,v)
#define ND_RESE_DCR__RE_VC0_TAIL__RESERVED_insert(x,v)  _BGQ_INSERT(8,7,x,v)

//   Field:       que0
//   Description: receiver vc0 que0 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE0_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE0_position     15
#define ND_RESE_DCR__RE_VC0_TAIL__QUE0_get(x)       _BGQ_GET(7,15,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE0_set(v)       _BGQ_SET(7,15,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE0_insert(x,v)  _BGQ_INSERT(7,15,x,v)

//   Field:       que1
//   Description: receiver vc0 que1 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE1_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE1_position     23
#define ND_RESE_DCR__RE_VC0_TAIL__QUE1_get(x)       _BGQ_GET(7,23,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE1_set(v)       _BGQ_SET(7,23,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE1_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       que2
//   Description: receiver vc0 que2 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE2_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE2_position     31
#define ND_RESE_DCR__RE_VC0_TAIL__QUE2_get(x)       _BGQ_GET(7,31,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE2_set(v)       _BGQ_SET(7,31,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE2_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       que3
//   Description: receiver vc0 que3 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE3_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE3_position     39
#define ND_RESE_DCR__RE_VC0_TAIL__QUE3_get(x)       _BGQ_GET(7,39,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE3_set(v)       _BGQ_SET(7,39,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE3_insert(x,v)  _BGQ_INSERT(7,39,x,v)

//   Field:       que4
//   Description: receiver vc0 que4 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE4_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE4_position     47
#define ND_RESE_DCR__RE_VC0_TAIL__QUE4_get(x)       _BGQ_GET(7,47,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE4_set(v)       _BGQ_SET(7,47,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE4_insert(x,v)  _BGQ_INSERT(7,47,x,v)

//   Field:       que5
//   Description: receiver vc0 que5 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE5_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE5_position     55
#define ND_RESE_DCR__RE_VC0_TAIL__QUE5_get(x)       _BGQ_GET(7,55,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE5_set(v)       _BGQ_SET(7,55,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE5_insert(x,v)  _BGQ_INSERT(7,55,x,v)

//   Field:       que6
//   Description: receiver vc0 que6 tail pointer
#define ND_RESE_DCR__RE_VC0_TAIL__QUE6_width        7
#define ND_RESE_DCR__RE_VC0_TAIL__QUE6_position     63
#define ND_RESE_DCR__RE_VC0_TAIL__QUE6_get(x)       _BGQ_GET(7,63,x)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE6_set(v)       _BGQ_SET(7,63,v)
#define ND_RESE_DCR__RE_VC0_TAIL__QUE6_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        re_vc0_pkt_cnt
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc0 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT_offset  (0x0033) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC0_PKT_CNT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC0_PKT_CNT__RESERVED_width        8
#define ND_RESE_DCR__RE_VC0_PKT_CNT__RESERVED_position     7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__RESERVED_get(x)       _BGQ_GET(8,7,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__RESERVED_set(v)       _BGQ_SET(8,7,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__RESERVED_insert(x,v)  _BGQ_INSERT(8,7,x,v)

//   Field:       que0
//   Description: receiver vc0 que0 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE0_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE0_position     15
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE0_get(x)       _BGQ_GET(7,15,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE0_set(v)       _BGQ_SET(7,15,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE0_insert(x,v)  _BGQ_INSERT(7,15,x,v)

//   Field:       que1
//   Description: receiver vc0 que1 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE1_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE1_position     23
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE1_get(x)       _BGQ_GET(7,23,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE1_set(v)       _BGQ_SET(7,23,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE1_insert(x,v)  _BGQ_INSERT(7,23,x,v)

//   Field:       que2
//   Description: receiver vc0 que2 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE2_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE2_position     31
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE2_get(x)       _BGQ_GET(7,31,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE2_set(v)       _BGQ_SET(7,31,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE2_insert(x,v)  _BGQ_INSERT(7,31,x,v)

//   Field:       que3
//   Description: receiver vc0 que3 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE3_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE3_position     39
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE3_get(x)       _BGQ_GET(7,39,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE3_set(v)       _BGQ_SET(7,39,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE3_insert(x,v)  _BGQ_INSERT(7,39,x,v)

//   Field:       que4
//   Description: receiver vc0 que4 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE4_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE4_position     47
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE4_get(x)       _BGQ_GET(7,47,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE4_set(v)       _BGQ_SET(7,47,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE4_insert(x,v)  _BGQ_INSERT(7,47,x,v)

//   Field:       que5
//   Description: receiver vc0 que5 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE5_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE5_position     55
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE5_get(x)       _BGQ_GET(7,55,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE5_set(v)       _BGQ_SET(7,55,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE5_insert(x,v)  _BGQ_INSERT(7,55,x,v)

//   Field:       que6
//   Description: receiver vc0 que6 packet count
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE6_width        7
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE6_position     63
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE6_get(x)       _BGQ_GET(7,63,x)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE6_set(v)       _BGQ_SET(7,63,v)
#define ND_RESE_DCR__RE_VC0_PKT_CNT__QUE6_insert(x,v)  _BGQ_INSERT(7,63,x,v)

  
// Register:
//   Name:        re_vc0_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc0 state
#define ND_RESE_DCR__RE_VC0_STATE_offset  (0x0034) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC0_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_WR_position     3
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_WR_get(x)       _BGQ_GET(3,3,x)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_WR_set(v)       _BGQ_SET(3,3,v)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,3,x,v)

//   Field:       ctrl_hdr
//   Description: control logic header fetch state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_HDR_width        3
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_HDR_position     7
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_HDR_get(x)       _BGQ_GET(3,7,x)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_HDR_set(v)       _BGQ_SET(3,7,v)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_HDR_insert(x,v)  _BGQ_INSERT(3,7,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_RD_position     11
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_RD_get(x)       _BGQ_GET(4,11,x)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_RD_set(v)       _BGQ_SET(4,11,v)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       ctrl_trans0
//   Description: control logic path 0 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS0_width        3
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS0_position     15
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS0_get(x)       _BGQ_GET(3,15,x)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS0_set(v)       _BGQ_SET(3,15,v)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS0_insert(x,v)  _BGQ_INSERT(3,15,x,v)

//   Field:       ctrl_trans1
//   Description: control logic path 1 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS1_width        3
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS1_position     19
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS1_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS1_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC0_STATE__CTRL_TRANS1_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       arb
//   Description: arbitration state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC0_STATE__ARB_width        3
#define ND_RESE_DCR__RE_VC0_STATE__ARB_position     23
#define ND_RESE_DCR__RE_VC0_STATE__ARB_get(x)       _BGQ_GET(3,23,x)
#define ND_RESE_DCR__RE_VC0_STATE__ARB_set(v)       _BGQ_SET(3,23,v)
#define ND_RESE_DCR__RE_VC0_STATE__ARB_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       fifo_rd0
//   Description: path 0 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD0_width        4
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD0_position     27
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD0_get(x)       _BGQ_GET(4,27,x)
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD0_set(v)       _BGQ_SET(4,27,v)
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD0_insert(x,v)  _BGQ_INSERT(4,27,x,v)

//   Field:       fifo_rd1
//   Description: path 1 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD1_width        4
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD1_position     31
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD1_get(x)       _BGQ_GET(4,31,x)
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD1_set(v)       _BGQ_SET(4,31,v)
#define ND_RESE_DCR__RE_VC0_STATE__FIFO_RD1_insert(x,v)  _BGQ_INSERT(4,31,x,v)

//   Field:       list_wr
//   Description: pointer list write state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC0_STATE__LIST_WR_width        4
#define ND_RESE_DCR__RE_VC0_STATE__LIST_WR_position     35
#define ND_RESE_DCR__RE_VC0_STATE__LIST_WR_get(x)       _BGQ_GET(4,35,x)
#define ND_RESE_DCR__RE_VC0_STATE__LIST_WR_set(v)       _BGQ_SET(4,35,v)
#define ND_RESE_DCR__RE_VC0_STATE__LIST_WR_insert(x,v)  _BGQ_INSERT(4,35,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC0_STATE__RESERVED_width        28
#define ND_RESE_DCR__RE_VC0_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC0_STATE__RESERVED_get(x)       _BGQ_GET(28,63,x)
#define ND_RESE_DCR__RE_VC0_STATE__RESERVED_set(v)       _BGQ_SET(28,63,v)
#define ND_RESE_DCR__RE_VC0_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(28,63,x,v)

  
// Register:
//   Name:        re_vc1_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc1 state
#define ND_RESE_DCR__RE_VC1_STATE_offset  (0x0035) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC1_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_slot_valid
//   Description: control logic packet slot valid bit field.  A 1 in a bit means there is a valid packet in that slot
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_width        7
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_position     6
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_get(x)       _BGQ_GET(7,6,x)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_set(v)       _BGQ_SET(7,6,v)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_SLOT_VALID_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_WR_position     11
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_WR_get(x)       _BGQ_GET(3,11,x)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_WR_set(v)       _BGQ_SET(3,11,v)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_RD_position     15
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_RD_get(x)       _BGQ_GET(4,15,x)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_RD_set(v)       _BGQ_SET(4,15,v)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       ctrl_trans0
//   Description: control logic path 0 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS0_width        3
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS0_position     19
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS0_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS0_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS0_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       ctrl_trans1
//   Description: control logic path 1 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS1_width        3
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS1_position     23
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS1_get(x)       _BGQ_GET(3,23,x)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS1_set(v)       _BGQ_SET(3,23,v)
#define ND_RESE_DCR__RE_VC1_STATE__CTRL_TRANS1_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       arb
//   Description: arbitration state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC1_STATE__ARB_width        3
#define ND_RESE_DCR__RE_VC1_STATE__ARB_position     27
#define ND_RESE_DCR__RE_VC1_STATE__ARB_get(x)       _BGQ_GET(3,27,x)
#define ND_RESE_DCR__RE_VC1_STATE__ARB_set(v)       _BGQ_SET(3,27,v)
#define ND_RESE_DCR__RE_VC1_STATE__ARB_insert(x,v)  _BGQ_INSERT(3,27,x,v)

//   Field:       fifo_rd0
//   Description: path 0 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD0_width        4
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD0_position     31
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD0_get(x)       _BGQ_GET(4,31,x)
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD0_set(v)       _BGQ_SET(4,31,v)
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD0_insert(x,v)  _BGQ_INSERT(4,31,x,v)

//   Field:       fifo_rd1
//   Description: path 1 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD1_width        4
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD1_position     35
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD1_get(x)       _BGQ_GET(4,35,x)
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD1_set(v)       _BGQ_SET(4,35,v)
#define ND_RESE_DCR__RE_VC1_STATE__FIFO_RD1_insert(x,v)  _BGQ_INSERT(4,35,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC1_STATE__RESERVED_width        28
#define ND_RESE_DCR__RE_VC1_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC1_STATE__RESERVED_get(x)       _BGQ_GET(28,63,x)
#define ND_RESE_DCR__RE_VC1_STATE__RESERVED_set(v)       _BGQ_SET(28,63,v)
#define ND_RESE_DCR__RE_VC1_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(28,63,x,v)

  
// Register:
//   Name:        re_vc2_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc2 state
#define ND_RESE_DCR__RE_VC2_STATE_offset  (0x0036) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC2_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_slot_valid
//   Description: control logic packet slot valid bit field.  A 1 in a bit means there is a valid packet in that slot
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_width        7
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_position     6
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_get(x)       _BGQ_GET(7,6,x)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_set(v)       _BGQ_SET(7,6,v)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_SLOT_VALID_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_WR_position     11
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_WR_get(x)       _BGQ_GET(3,11,x)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_WR_set(v)       _BGQ_SET(3,11,v)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_RD_position     15
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_RD_get(x)       _BGQ_GET(4,15,x)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_RD_set(v)       _BGQ_SET(4,15,v)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       ctrl_trans0
//   Description: control logic path 0 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS0_width        3
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS0_position     19
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS0_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS0_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS0_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       ctrl_trans1
//   Description: control logic path 1 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS1_width        3
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS1_position     23
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS1_get(x)       _BGQ_GET(3,23,x)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS1_set(v)       _BGQ_SET(3,23,v)
#define ND_RESE_DCR__RE_VC2_STATE__CTRL_TRANS1_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       arb
//   Description: arbitration state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC2_STATE__ARB_width        3
#define ND_RESE_DCR__RE_VC2_STATE__ARB_position     27
#define ND_RESE_DCR__RE_VC2_STATE__ARB_get(x)       _BGQ_GET(3,27,x)
#define ND_RESE_DCR__RE_VC2_STATE__ARB_set(v)       _BGQ_SET(3,27,v)
#define ND_RESE_DCR__RE_VC2_STATE__ARB_insert(x,v)  _BGQ_INSERT(3,27,x,v)

//   Field:       fifo_rd0
//   Description: path 0 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD0_width        4
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD0_position     31
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD0_get(x)       _BGQ_GET(4,31,x)
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD0_set(v)       _BGQ_SET(4,31,v)
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD0_insert(x,v)  _BGQ_INSERT(4,31,x,v)

//   Field:       fifo_rd1
//   Description: path 1 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD1_width        4
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD1_position     35
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD1_get(x)       _BGQ_GET(4,35,x)
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD1_set(v)       _BGQ_SET(4,35,v)
#define ND_RESE_DCR__RE_VC2_STATE__FIFO_RD1_insert(x,v)  _BGQ_INSERT(4,35,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC2_STATE__RESERVED_width        28
#define ND_RESE_DCR__RE_VC2_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC2_STATE__RESERVED_get(x)       _BGQ_GET(28,63,x)
#define ND_RESE_DCR__RE_VC2_STATE__RESERVED_set(v)       _BGQ_SET(28,63,v)
#define ND_RESE_DCR__RE_VC2_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(28,63,x,v)

  
// Register:
//   Name:        re_vc3_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc3 state
#define ND_RESE_DCR__RE_VC3_STATE_offset  (0x0037) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC3_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_slot_valid
//   Description: control logic packet slot valid bit field.  A 1 in a bit means there is a valid packet in that slot
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_SLOT_VALID_width        7
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_SLOT_VALID_position     6
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_SLOT_VALID_get(x)       _BGQ_GET(7,6,x)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_SLOT_VALID_set(v)       _BGQ_SET(7,6,v)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_SLOT_VALID_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_WR_position     11
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_WR_get(x)       _BGQ_GET(3,11,x)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_WR_set(v)       _BGQ_SET(3,11,v)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,11,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_RD_position     15
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_RD_get(x)       _BGQ_GET(4,15,x)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_RD_set(v)       _BGQ_SET(4,15,v)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       ctrl_trans0
//   Description: control logic path 0 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS0_width        3
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS0_position     19
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS0_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS0_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS0_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       ctrl_trans1
//   Description: control logic path 1 data transfer state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS1_width        3
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS1_position     23
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS1_get(x)       _BGQ_GET(3,23,x)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS1_set(v)       _BGQ_SET(3,23,v)
#define ND_RESE_DCR__RE_VC3_STATE__CTRL_TRANS1_insert(x,v)  _BGQ_INSERT(3,23,x,v)

//   Field:       arb
//   Description: arbitration state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC3_STATE__ARB_width        3
#define ND_RESE_DCR__RE_VC3_STATE__ARB_position     27
#define ND_RESE_DCR__RE_VC3_STATE__ARB_get(x)       _BGQ_GET(3,27,x)
#define ND_RESE_DCR__RE_VC3_STATE__ARB_set(v)       _BGQ_SET(3,27,v)
#define ND_RESE_DCR__RE_VC3_STATE__ARB_insert(x,v)  _BGQ_INSERT(3,27,x,v)

//   Field:       fifo_rd0
//   Description: path 0 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD0_width        4
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD0_position     31
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD0_get(x)       _BGQ_GET(4,31,x)
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD0_set(v)       _BGQ_SET(4,31,v)
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD0_insert(x,v)  _BGQ_INSERT(4,31,x,v)

//   Field:       fifo_rd1
//   Description: path 1 fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD1_width        4
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD1_position     35
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD1_get(x)       _BGQ_GET(4,35,x)
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD1_set(v)       _BGQ_SET(4,35,v)
#define ND_RESE_DCR__RE_VC3_STATE__FIFO_RD1_insert(x,v)  _BGQ_INSERT(4,35,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC3_STATE__RESERVED_width        28
#define ND_RESE_DCR__RE_VC3_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC3_STATE__RESERVED_get(x)       _BGQ_GET(28,63,x)
#define ND_RESE_DCR__RE_VC3_STATE__RESERVED_set(v)       _BGQ_SET(28,63,v)
#define ND_RESE_DCR__RE_VC3_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(28,63,x,v)

  
// Register:
//   Name:        re_vc4_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc4 state
#define ND_RESE_DCR__RE_VC4_STATE_offset  (0x0038) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC4_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_slot_valid
//   Description: control logic packet slot valid bit field.  A 1 in a bit means there is a valid packet in that slot
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_SLOT_VALID_width        7
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_SLOT_VALID_position     6
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_SLOT_VALID_get(x)       _BGQ_GET(7,6,x)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_SLOT_VALID_set(v)       _BGQ_SET(7,6,v)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_SLOT_VALID_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       ctrl_head
//   Description: control logic fifo head pointer
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_width        4
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_position     11
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_get(x)       _BGQ_GET(4,11,x)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_set(v)       _BGQ_SET(4,11,v)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       ctrl_tail
//   Description: control logic fifo tail pointer
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_width        4
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_position     15
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_get(x)       _BGQ_GET(4,15,x)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_set(v)       _BGQ_SET(4,15,v)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_position     19
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_position     23
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_get(x)       _BGQ_GET(4,23,x)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_set(v)       _BGQ_SET(4,23,v)
#define ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,23,x,v)

//   Field:       fifo_rd
//   Description: fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_width        4
#define ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_position     27
#define ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_get(x)       _BGQ_GET(4,27,x)
#define ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_set(v)       _BGQ_SET(4,27,v)
#define ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_insert(x,v)  _BGQ_INSERT(4,27,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC4_STATE__RESERVED_width        36
#define ND_RESE_DCR__RE_VC4_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC4_STATE__RESERVED_get(x)       _BGQ_GET(36,63,x)
#define ND_RESE_DCR__RE_VC4_STATE__RESERVED_set(v)       _BGQ_SET(36,63,v)
#define ND_RESE_DCR__RE_VC4_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(36,63,x,v)

  
// Register:
//   Name:        re_vc5_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: receiver vc5 state
#define ND_RESE_DCR__RE_VC5_STATE_offset  (0x0039) // Offset of register in instance 
#define ND_RESE_DCR__RE_VC5_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ctrl_slot_valid
//   Description: control logic packet slot valid bit field.  A 1 in a bit means there is a valid packet in that slot
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_SLOT_VALID_width        7
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_SLOT_VALID_position     6
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_SLOT_VALID_get(x)       _BGQ_GET(7,6,x)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_SLOT_VALID_set(v)       _BGQ_SET(7,6,v)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_SLOT_VALID_insert(x,v)  _BGQ_INSERT(7,6,x,v)

//   Field:       ctrl_head
//   Description: control logic fifo head pointer
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_width        4
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_position     11
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_get(x)       _BGQ_GET(4,11,x)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_set(v)       _BGQ_SET(4,11,v)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_insert(x,v)  _BGQ_INSERT(4,11,x,v)

//   Field:       ctrl_tail
//   Description: control logic fifo tail pointer
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_width        4
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_position     15
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_get(x)       _BGQ_GET(4,15,x)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_set(v)       _BGQ_SET(4,15,v)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       ctrl_wr
//   Description: control logic write state machine state.  IDLE state is b001
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_width        3
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_position     19
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_get(x)       _BGQ_GET(3,19,x)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_set(v)       _BGQ_SET(3,19,v)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_insert(x,v)  _BGQ_INSERT(3,19,x,v)

//   Field:       ctrl_rd
//   Description: control logic read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_width        4
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_position     23
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_get(x)       _BGQ_GET(4,23,x)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_set(v)       _BGQ_SET(4,23,v)
#define ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_insert(x,v)  _BGQ_INSERT(4,23,x,v)

//   Field:       fifo_rd
//   Description: fifo read state machine state.  IDLE state is b0001
#define ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_width        4
#define ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_position     27
#define ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_get(x)       _BGQ_GET(4,27,x)
#define ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_set(v)       _BGQ_SET(4,27,v)
#define ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_insert(x,v)  _BGQ_INSERT(4,27,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__RE_VC5_STATE__RESERVED_width        36
#define ND_RESE_DCR__RE_VC5_STATE__RESERVED_position     63
#define ND_RESE_DCR__RE_VC5_STATE__RESERVED_get(x)       _BGQ_GET(36,63,x)
#define ND_RESE_DCR__RE_VC5_STATE__RESERVED_set(v)       _BGQ_SET(36,63,v)
#define ND_RESE_DCR__RE_VC5_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(36,63,x,v)

  
// Register:
//   Name:        se_internal_state
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender internal state
#define ND_RESE_DCR__SE_INTERNAL_STATE_offset  (0x003A) // Offset of register in instance 
#define ND_RESE_DCR__SE_INTERNAL_STATE_reset  UNSIGNED64(0x0000000000000000)

//   Field:       arb
//   Description: arbiter fsm state.  IDLE state is b0001
#define ND_RESE_DCR__SE_INTERNAL_STATE__ARB_width        4
#define ND_RESE_DCR__SE_INTERNAL_STATE__ARB_position     3
#define ND_RESE_DCR__SE_INTERNAL_STATE__ARB_get(x)       _BGQ_GET(4,3,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__ARB_set(v)       _BGQ_SET(4,3,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__ARB_insert(x,v)  _BGQ_INSERT(4,3,x,v)

//   Field:       ctrl
//   Description: control logic fsm state.  IDLE states are b00001 or b00010
#define ND_RESE_DCR__SE_INTERNAL_STATE__CTRL_width        5
#define ND_RESE_DCR__SE_INTERNAL_STATE__CTRL_position     11
#define ND_RESE_DCR__SE_INTERNAL_STATE__CTRL_get(x)       _BGQ_GET(5,11,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__CTRL_set(v)       _BGQ_SET(5,11,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__CTRL_insert(x,v)  _BGQ_INSERT(5,11,x,v)

//   Field:       retrans_in
//   Description: retransmission fifo input (write) logic fsm state.  IDLE state is 001
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_IN_width        3
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_IN_position     15
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_IN_get(x)       _BGQ_GET(3,15,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_IN_set(v)       _BGQ_SET(3,15,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_IN_insert(x,v)  _BGQ_INSERT(3,15,x,v)

//   Field:       retrans_out
//   Description: retransmission fifo output (read) logic fsm state.  IDLE state is 0001
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_OUT_width        4
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_OUT_position     19
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_OUT_get(x)       _BGQ_GET(4,19,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_OUT_set(v)       _BGQ_SET(4,19,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RETRANS_OUT_insert(x,v)  _BGQ_INSERT(4,19,x,v)

//   Field:       begin_ptr
//   Description: retransmission fifo begin pointer.  DD2 only.  DD1 tied to 0
#define ND_RESE_DCR__SE_INTERNAL_STATE__BEGIN_PTR_width        12
#define ND_RESE_DCR__SE_INTERNAL_STATE__BEGIN_PTR_position     31
#define ND_RESE_DCR__SE_INTERNAL_STATE__BEGIN_PTR_get(x)       _BGQ_GET(12,31,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__BEGIN_PTR_set(v)       _BGQ_SET(12,31,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__BEGIN_PTR_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       end_ptr
//   Description: retransmission fifo end pointer.  DD2 only.  DD1 tied to 0
#define ND_RESE_DCR__SE_INTERNAL_STATE__END_PTR_width        12
#define ND_RESE_DCR__SE_INTERNAL_STATE__END_PTR_position     43
#define ND_RESE_DCR__SE_INTERNAL_STATE__END_PTR_get(x)       _BGQ_GET(12,43,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__END_PTR_set(v)       _BGQ_SET(12,43,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__END_PTR_insert(x,v)  _BGQ_INSERT(12,43,x,v)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__SE_INTERNAL_STATE__RESERVED_width        20
#define ND_RESE_DCR__SE_INTERNAL_STATE__RESERVED_position     63
#define ND_RESE_DCR__SE_INTERNAL_STATE__RESERVED_get(x)       _BGQ_GET(20,63,x)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RESERVED_set(v)       _BGQ_SET(20,63,v)
#define ND_RESE_DCR__SE_INTERNAL_STATE__RESERVED_insert(x,v)  _BGQ_INSERT(20,63,x,v)

  
// Register:
//   Name:        se_tokens
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: sender tokens
#define ND_RESE_DCR__SE_TOKENS_offset  (0x003B) // Offset of register in instance 
#define ND_RESE_DCR__SE_TOKENS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reserved
//   Description: unused, tied to zero
#define ND_RESE_DCR__SE_TOKENS__RESERVED_width        34
#define ND_RESE_DCR__SE_TOKENS__RESERVED_position     33
#define ND_RESE_DCR__SE_TOKENS__RESERVED_get(x)       _BGQ_GET(34,33,x)
#define ND_RESE_DCR__SE_TOKENS__RESERVED_set(v)       _BGQ_SET(34,33,v)
#define ND_RESE_DCR__SE_TOKENS__RESERVED_insert(x,v)  _BGQ_INSERT(34,33,x,v)

//   Field:       vc_dynamic
//   Description: vc user dynamic
#define ND_RESE_DCR__SE_TOKENS__VC_DYNAMIC_width        10
#define ND_RESE_DCR__SE_TOKENS__VC_DYNAMIC_position     43
#define ND_RESE_DCR__SE_TOKENS__VC_DYNAMIC_get(x)       _BGQ_GET(10,43,x)
#define ND_RESE_DCR__SE_TOKENS__VC_DYNAMIC_set(v)       _BGQ_SET(10,43,v)
#define ND_RESE_DCR__SE_TOKENS__VC_DYNAMIC_insert(x,v)  _BGQ_INSERT(10,43,x,v)

//   Field:       vc_normal
//   Description: vc user normal bubble
#define ND_RESE_DCR__SE_TOKENS__VC_NORMAL_width        4
#define ND_RESE_DCR__SE_TOKENS__VC_NORMAL_position     47
#define ND_RESE_DCR__SE_TOKENS__VC_NORMAL_get(x)       _BGQ_GET(4,47,x)
#define ND_RESE_DCR__SE_TOKENS__VC_NORMAL_set(v)       _BGQ_SET(4,47,v)
#define ND_RESE_DCR__SE_TOKENS__VC_NORMAL_insert(x,v)  _BGQ_INSERT(4,47,x,v)

//   Field:       vc_priority
//   Description: vc user priority bubble
#define ND_RESE_DCR__SE_TOKENS__VC_PRIORITY_width        4
#define ND_RESE_DCR__SE_TOKENS__VC_PRIORITY_position     51
#define ND_RESE_DCR__SE_TOKENS__VC_PRIORITY_get(x)       _BGQ_GET(4,51,x)
#define ND_RESE_DCR__SE_TOKENS__VC_PRIORITY_set(v)       _BGQ_SET(4,51,v)
#define ND_RESE_DCR__SE_TOKENS__VC_PRIORITY_insert(x,v)  _BGQ_INSERT(4,51,x,v)

//   Field:       vc_system
//   Description: vc system i/o bubble
#define ND_RESE_DCR__SE_TOKENS__VC_SYSTEM_width        4
#define ND_RESE_DCR__SE_TOKENS__VC_SYSTEM_position     55
#define ND_RESE_DCR__SE_TOKENS__VC_SYSTEM_get(x)       _BGQ_GET(4,55,x)
#define ND_RESE_DCR__SE_TOKENS__VC_SYSTEM_set(v)       _BGQ_SET(4,55,v)
#define ND_RESE_DCR__SE_TOKENS__VC_SYSTEM_insert(x,v)  _BGQ_INSERT(4,55,x,v)

//   Field:       vc_coll0
//   Description: vc collective user comm world or system
#define ND_RESE_DCR__SE_TOKENS__VC_COLL0_width        4
#define ND_RESE_DCR__SE_TOKENS__VC_COLL0_position     59
#define ND_RESE_DCR__SE_TOKENS__VC_COLL0_get(x)       _BGQ_GET(4,59,x)
#define ND_RESE_DCR__SE_TOKENS__VC_COLL0_set(v)       _BGQ_SET(4,59,v)
#define ND_RESE_DCR__SE_TOKENS__VC_COLL0_insert(x,v)  _BGQ_INSERT(4,59,x,v)

//   Field:       vc_coll1
//   Description: vc collective user sub comm
#define ND_RESE_DCR__SE_TOKENS__VC_COLL1_width        4
#define ND_RESE_DCR__SE_TOKENS__VC_COLL1_position     63
#define ND_RESE_DCR__SE_TOKENS__VC_COLL1_get(x)       _BGQ_GET(4,63,x)
#define ND_RESE_DCR__SE_TOKENS__VC_COLL1_set(v)       _BGQ_SET(4,63,v)
#define ND_RESE_DCR__SE_TOKENS__VC_COLL1_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0057) // Offset of register in instance 
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0058) // Offset of register in instance 
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0059) // Offset of register in instance 
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: rese_interrupts
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_RESE_DCR__RESE_INTERRUPTS__STATE_offset         (0x0040) // Offset of State register in instance
#define ND_RESE_DCR__RESE_INTERRUPTS__FIRST_offset         (0x0044) // Offset of First register in instance
#define ND_RESE_DCR__RESE_INTERRUPTS__FORCE_offset         (0x0043) // Offset of Force register in instance
#define ND_RESE_DCR__RESE_INTERRUPTS__MACHINE_CHECK_offset (0x0045) // Offset of Machine Check register in instance
#define ND_RESE_DCR__RESE_INTERRUPTS__CRITICAL_offset      (0x0046) // Offset of Critical register in instance
#define ND_RESE_DCR__RESE_INTERRUPTS__NONCRITICAL_offset   (0x0047) // Offset of Noncritical register in instance

#define ND_RESE_DCR__RESE_INTERRUPTS__LOCAL_RING_width        1
#define ND_RESE_DCR__RESE_INTERRUPTS__LOCAL_RING_position     63
#define ND_RESE_DCR__RESE_INTERRUPTS__LOCAL_RING_get(x)       _BGQ_GET(1,63,x)
#define ND_RESE_DCR__RESE_INTERRUPTS__LOCAL_RING_set(v)       _BGQ_SET(1,63,v)
#define ND_RESE_DCR__RESE_INTERRUPTS__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0050) // Offset of State register in instance
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0053) // Offset of First register in instance
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0052) // Offset of Force register in instance
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0054) // Offset of Machine Check register in instance
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0055) // Offset of Critical register in instance
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0056) // Offset of Noncritical register in instance

#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: rese_interrupts_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW_offset         (0x0041) // Offset of Control register in instance

#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW__LOCAL_RING_width        2
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW__LOCAL_RING_position     63
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW__LOCAL_RING_get(x)       _BGQ_GET(2,63,x)
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW__LOCAL_RING_set(v)       _BGQ_SET(2,63,v)
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_LOW__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: rese_interrupts_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_RESE_DCR__RESE_INTERRUPTS_CONTROL_HIGH_offset         (0x0042) // Offset of Control register in instance
  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0051) // Offset of Control register in instance

#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define ND_RESE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct nd_rese_dcr { 
    /* 0x0000 */ uint64_t reset;
    /* 0x0001 */ uint64_t ctrl;
    /* 0x0002 */ uint64_t upc_ctrl;
    /* 0x0003 */ uint64_t thresh_array_ce;
    /* 0x0004 */ uint64_t thresh_re_err;
    /* 0x0005 */ uint64_t thresh_se_retran;
    /* 0x0006 */ uint64_t clear;
    /* 0x0007 */ uint64_t fatal_err;
    /* 0x0008 */ uint64_t fatal_err_enable;
    /* 0x0009 */ uint64_t re_link_err_cnt;
    /* 0x000A */ uint64_t se_retrans_cnt;
    /* 0x000B */ uint64_t ce_count[ND_RESE_DCR__CE_COUNT_range];
    /* 0x0014 */ uint64_t upc[ND_RESE_DCR__UPC_range];
    /* 0x001A */ uint64_t reserve_001A[0x0006];
    /* 0x0020 */ uint64_t backdoor_ctl;
    /* 0x0021 */ uint64_t coll_class_remap_vec;
    /* 0x0022 */ uint64_t backdoor_wr_data;
    /* 0x0023 */ uint64_t backdoor_wr_ecc;
    /* 0x0024 */ uint64_t backdoor_rd_data;
    /* 0x0025 */ uint64_t backdoor_rd;
    /* 0x0026 */ uint64_t se_gi_data;
    /* 0x0027 */ uint64_t re_link_crc_usr;
    /* 0x0028 */ uint64_t re_link_crc_sys;
    /* 0x0029 */ uint64_t se_link_crc_usr;
    /* 0x002A */ uint64_t se_link_crc_sys;
    /* 0x002B */ uint64_t reserve_002B[0x0005];
    /* 0x0030 */ uint64_t re_internal_state;
    /* 0x0031 */ uint64_t re_vc0_head;
    /* 0x0032 */ uint64_t re_vc0_tail;
    /* 0x0033 */ uint64_t re_vc0_pkt_cnt;
    /* 0x0034 */ uint64_t re_vc0_state;
    /* 0x0035 */ uint64_t re_vc1_state;
    /* 0x0036 */ uint64_t re_vc2_state;
    /* 0x0037 */ uint64_t re_vc3_state;
    /* 0x0038 */ uint64_t re_vc4_state;
    /* 0x0039 */ uint64_t re_vc5_state;
    /* 0x003A */ uint64_t se_internal_state;
    /* 0x003B */ uint64_t se_tokens;
    /* 0x003C */ uint64_t reserve_003C[0x0004];
    /* 0x0040 */ uint64_t rese_interrupts__state;
    /* 0x0041 */ uint64_t rese_interrupts_control_low;
    /* 0x0042 */ uint64_t rese_interrupts_control_high;
    /* 0x0043 */ uint64_t rese_interrupts__force;
    /* 0x0044 */ uint64_t rese_interrupts__first;
    /* 0x0045 */ uint64_t rese_interrupts__machine_check;
    /* 0x0046 */ uint64_t rese_interrupts__critical;
    /* 0x0047 */ uint64_t rese_interrupts__noncritical;
    /* 0x0048 */ uint64_t reserve_0048[0x0008];
    /* 0x0050 */ uint64_t interrupt_internal_error__state;
    /* 0x0051 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0052 */ uint64_t interrupt_internal_error__force;
    /* 0x0053 */ uint64_t interrupt_internal_error__first;
    /* 0x0054 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0055 */ uint64_t interrupt_internal_error__critical;
    /* 0x0056 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0057 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0058 */ uint64_t interrupt_internal_error_hw_info;
} nd_rese_dcr_t;
  
#define ND_RESE_DCR_USER_PTR(i) _DCR_USER_PTR__INDEX(nd_rese_dcr_t, ND_RESE_DCR, i)
#define ND_RESE_DCR_PRIV_PTR(i) _DCR_PRIV_PTR__INDEX(nd_rese_dcr_t, ND_RESE_DCR, i)
  
#define ND_RESE_DCR_USER_PTR__T0 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T0)
#define ND_RESE_DCR_USER_PTR__T1 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T1)
#define ND_RESE_DCR_USER_PTR__T2 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T2)
#define ND_RESE_DCR_USER_PTR__T3 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T3)
#define ND_RESE_DCR_USER_PTR__T4 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T4)
#define ND_RESE_DCR_USER_PTR__T5 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T5)
#define ND_RESE_DCR_USER_PTR__T6 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T6)
#define ND_RESE_DCR_USER_PTR__T7 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T7)
#define ND_RESE_DCR_USER_PTR__T8 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T8)
#define ND_RESE_DCR_USER_PTR__T9 _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T9)
#define ND_RESE_DCR_USER_PTR__IO _DCR_USER_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, IO)
  
#define ND_RESE_DCR_PRIV_PTR__T0 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T0)
#define ND_RESE_DCR_PRIV_PTR__T1 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T1)
#define ND_RESE_DCR_PRIV_PTR__T2 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T2)
#define ND_RESE_DCR_PRIV_PTR__T3 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T3)
#define ND_RESE_DCR_PRIV_PTR__T4 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T4)
#define ND_RESE_DCR_PRIV_PTR__T5 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T5)
#define ND_RESE_DCR_PRIV_PTR__T6 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T6)
#define ND_RESE_DCR_PRIV_PTR__T7 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T7)
#define ND_RESE_DCR_PRIV_PTR__T8 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T8)
#define ND_RESE_DCR_PRIV_PTR__T9 _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, T9)
#define ND_RESE_DCR_PRIV_PTR__IO _DCR_PRIV_PTR__NAME(nd_rese_dcr_t, ND_RESE_DCR, IO)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
