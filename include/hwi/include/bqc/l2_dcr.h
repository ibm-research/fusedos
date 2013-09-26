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

#ifndef   _L2_DCR__H_ // Prevent multiple inclusion
#define   _L2_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define L2_DCR_base    (0x100000)  // Base address for all instances
#define L2_DCR_size    (0x0111)    // Size of instance (largest offset + 1 )
#define L2_DCR_stride  (0x4000)    // Distance between instances
#define L2_DCR_num     (0x0010)    // Number of instances
  
#define L2_DCR__Core00_base  (0x100000)  // Base address for instance Core00
#define L2_DCR__Core01_base  (0x104000)  // Base address for instance Core01
#define L2_DCR__Core02_base  (0x108000)  // Base address for instance Core02
#define L2_DCR__Core03_base  (0x10C000)  // Base address for instance Core03
#define L2_DCR__Core04_base  (0x110000)  // Base address for instance Core04
#define L2_DCR__Core05_base  (0x114000)  // Base address for instance Core05
#define L2_DCR__Core06_base  (0x118000)  // Base address for instance Core06
#define L2_DCR__Core07_base  (0x11C000)  // Base address for instance Core07
#define L2_DCR__Core08_base  (0x120000)  // Base address for instance Core08
#define L2_DCR__Core09_base  (0x124000)  // Base address for instance Core09
#define L2_DCR__Core10_base  (0x128000)  // Base address for instance Core10
#define L2_DCR__Core11_base  (0x12C000)  // Base address for instance Core11
#define L2_DCR__Core12_base  (0x130000)  // Base address for instance Core12
#define L2_DCR__Core13_base  (0x134000)  // Base address for instance Core13
#define L2_DCR__Core14_base  (0x138000)  // Base address for instance Core14
#define L2_DCR__Core15_base  (0x13C000)  // Base address for instance Core15
  
#define L2_DCR(i,reg) _DCR_REG__INDEX(L2_DCR, i, reg)
  
#define L2_DCR__Core00(reg) _DCR_REG__NAME(L2_DCR, Core00, reg)
#define L2_DCR__Core01(reg) _DCR_REG__NAME(L2_DCR, Core01, reg)
#define L2_DCR__Core02(reg) _DCR_REG__NAME(L2_DCR, Core02, reg)
#define L2_DCR__Core03(reg) _DCR_REG__NAME(L2_DCR, Core03, reg)
#define L2_DCR__Core04(reg) _DCR_REG__NAME(L2_DCR, Core04, reg)
#define L2_DCR__Core05(reg) _DCR_REG__NAME(L2_DCR, Core05, reg)
#define L2_DCR__Core06(reg) _DCR_REG__NAME(L2_DCR, Core06, reg)
#define L2_DCR__Core07(reg) _DCR_REG__NAME(L2_DCR, Core07, reg)
#define L2_DCR__Core08(reg) _DCR_REG__NAME(L2_DCR, Core08, reg)
#define L2_DCR__Core09(reg) _DCR_REG__NAME(L2_DCR, Core09, reg)
#define L2_DCR__Core10(reg) _DCR_REG__NAME(L2_DCR, Core10, reg)
#define L2_DCR__Core11(reg) _DCR_REG__NAME(L2_DCR, Core11, reg)
#define L2_DCR__Core12(reg) _DCR_REG__NAME(L2_DCR, Core12, reg)
#define L2_DCR__Core13(reg) _DCR_REG__NAME(L2_DCR, Core13, reg)
#define L2_DCR__Core14(reg) _DCR_REG__NAME(L2_DCR, Core14, reg)
#define L2_DCR__Core15(reg) _DCR_REG__NAME(L2_DCR, Core15, reg)
  
// Register:
//   Name:        ctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000000009F3
//   Description: general L2 controls
#define L2_DCR__CTRL_offset  (0x0014) // Offset of register in instance 
#define L2_DCR__CTRL_reset  UNSIGNED64(0x00000000000009F3)

//   Field:       ooo_ctrl
//   Description: Out-of-order control
#define L2_DCR__CTRL__OOO_CTRL_width        3
#define L2_DCR__CTRL__OOO_CTRL_position     52
#define L2_DCR__CTRL__OOO_CTRL_get(x)       _BGQ_GET(3,52,x)
#define L2_DCR__CTRL__OOO_CTRL_set(v)       _BGQ_SET(3,52,v)
#define L2_DCR__CTRL__OOO_CTRL_insert(x,v)  _BGQ_INSERT(3,52,x,v)

//   Field:       disable_confl
//   Description: disable speculation conflict detection
#define L2_DCR__CTRL__DISABLE_CONFL_width        1
#define L2_DCR__CTRL__DISABLE_CONFL_position     53
#define L2_DCR__CTRL__DISABLE_CONFL_get(x)       _BGQ_GET(1,53,x)
#define L2_DCR__CTRL__DISABLE_CONFL_set(v)       _BGQ_SET(1,53,v)
#define L2_DCR__CTRL__DISABLE_CONFL_insert(x,v)  _BGQ_INSERT(1,53,x,v)

//   Field:       disable_rdrec
//   Description: disable speculative reader recording
#define L2_DCR__CTRL__DISABLE_RDREC_width        1
#define L2_DCR__CTRL__DISABLE_RDREC_position     54
#define L2_DCR__CTRL__DISABLE_RDREC_get(x)       _BGQ_GET(1,54,x)
#define L2_DCR__CTRL__DISABLE_RDREC_set(v)       _BGQ_SET(1,54,v)
#define L2_DCR__CTRL__DISABLE_RDREC_insert(x,v)  _BGQ_INSERT(1,54,x,v)

//   Field:       ddr_size
//   Description: Size of DDR memory as 2^x * 512MB
#define L2_DCR__CTRL__DDR_SIZE_width        3
#define L2_DCR__CTRL__DDR_SIZE_position     57
#define L2_DCR__CTRL__DDR_SIZE_get(x)       _BGQ_GET(3,57,x)
#define L2_DCR__CTRL__DDR_SIZE_set(v)       _BGQ_SET(3,57,v)
#define L2_DCR__CTRL__DDR_SIZE_insert(x,v)  _BGQ_INSERT(3,57,x,v)

//   Field:       si_delay
//   Description: MS orgen keepalive delay for invalidate transport
#define L2_DCR__CTRL__SI_DELAY_width        4
#define L2_DCR__CTRL__SI_DELAY_position     61
#define L2_DCR__CTRL__SI_DELAY_get(x)       _BGQ_GET(4,61,x)
#define L2_DCR__CTRL__SI_DELAY_set(v)       _BGQ_SET(4,61,v)
#define L2_DCR__CTRL__SI_DELAY_insert(x,v)  _BGQ_INSERT(4,61,x,v)

//   Field:       enable_prft
//   Description: enable L2 prefetcher
#define L2_DCR__CTRL__ENABLE_PRFT_width        1
#define L2_DCR__CTRL__ENABLE_PRFT_position     62
#define L2_DCR__CTRL__ENABLE_PRFT_get(x)       _BGQ_GET(1,62,x)
#define L2_DCR__CTRL__ENABLE_PRFT_set(v)       _BGQ_SET(1,62,v)
#define L2_DCR__CTRL__ENABLE_PRFT_insert(x,v)  _BGQ_INSERT(1,62,x,v)

//   Field:       enable_wa
//   Description: enable write allocation
#define L2_DCR__CTRL__ENABLE_WA_width        1
#define L2_DCR__CTRL__ENABLE_WA_position     63
#define L2_DCR__CTRL__ENABLE_WA_get(x)       _BGQ_GET(1,63,x)
#define L2_DCR__CTRL__ENABLE_WA_set(v)       _BGQ_SET(1,63,v)
#define L2_DCR__CTRL__ENABLE_WA_insert(x,v)  _BGQ_INSERT(1,63,x,v)

  
// Register:
//   Name:        status
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: L2 status
#define L2_DCR__STATUS_offset  (0x0015) // Offset of register in instance 
#define L2_DCR__STATUS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       dcrpend
//   Description: DCR request in progress
#define L2_DCR__STATUS__DCRPEND_width        1
#define L2_DCR__STATUS__DCRPEND_position     0
#define L2_DCR__STATUS__DCRPEND_get(x)       _BGQ_GET(1,0,x)
#define L2_DCR__STATUS__DCRPEND_set(v)       _BGQ_SET(1,0,v)
#define L2_DCR__STATUS__DCRPEND_insert(x,v)  _BGQ_INSERT(1,0,x,v)

  
// Register:
//   Name:        replpol
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       00000000000001E1
//   Description: replacement policy control
#define L2_DCR__REPLPOL_offset  (0x0016) // Offset of register in instance 
#define L2_DCR__REPLPOL_reset  UNSIGNED64(0x00000000000001E1)

//   Field:       numsets
//   Description: number of master sets
#define L2_DCR__REPLPOL__NUMSETS_width        5
#define L2_DCR__REPLPOL__NUMSETS_position     59
#define L2_DCR__REPLPOL__NUMSETS_get(x)       _BGQ_GET(5,59,x)
#define L2_DCR__REPLPOL__NUMSETS_set(v)       _BGQ_SET(5,59,v)
#define L2_DCR__REPLPOL__NUMSETS_insert(x,v)  _BGQ_INSERT(5,59,x,v)

//   Field:       thresh
//   Description: threshold for saturation
#define L2_DCR__REPLPOL__THRESH_width        4
#define L2_DCR__REPLPOL__THRESH_position     63
#define L2_DCR__REPLPOL__THRESH_get(x)       _BGQ_GET(4,63,x)
#define L2_DCR__REPLPOL__THRESH_set(v)       _BGQ_SET(4,63,v)
#define L2_DCR__REPLPOL__THRESH_insert(x,v)  _BGQ_INSERT(4,63,x,v)

  
// Register:
//   Name:        refctrl
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000204000020100
//   Description: refresh control
#define L2_DCR__REFCTRL_offset  (0x0017) // Offset of register in instance 
#define L2_DCR__REFCTRL_reset  UNSIGNED64(0x0000204000020100)

//   Field:       scb_run
//   Description: enable scrub
#define L2_DCR__REFCTRL__SCB_RUN_width        1
#define L2_DCR__REFCTRL__SCB_RUN_position     18
#define L2_DCR__REFCTRL__SCB_RUN_get(x)       _BGQ_GET(1,18,x)
#define L2_DCR__REFCTRL__SCB_RUN_set(v)       _BGQ_SET(1,18,v)
#define L2_DCR__REFCTRL__SCB_RUN_insert(x,v)  _BGQ_INSERT(1,18,x,v)

//   Field:       scb_load
//   Description: set to force load into scrub count reg
#define L2_DCR__REFCTRL__SCB_LOAD_width        1
#define L2_DCR__REFCTRL__SCB_LOAD_position     19
#define L2_DCR__REFCTRL__SCB_LOAD_get(x)       _BGQ_GET(1,19,x)
#define L2_DCR__REFCTRL__SCB_LOAD_set(v)       _BGQ_SET(1,19,v)
#define L2_DCR__REFCTRL__SCB_LOAD_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       scb_interval
//   Description: interval between scrub operations
#define L2_DCR__REFCTRL__SCB_INTERVAL_width        12
#define L2_DCR__REFCTRL__SCB_INTERVAL_position     31
#define L2_DCR__REFCTRL__SCB_INTERVAL_get(x)       _BGQ_GET(12,31,x)
#define L2_DCR__REFCTRL__SCB_INTERVAL_set(v)       _BGQ_SET(12,31,v)
#define L2_DCR__REFCTRL__SCB_INTERVAL_insert(x,v)  _BGQ_INSERT(12,31,x,v)

//   Field:       run
//   Description: enable refresh
#define L2_DCR__REFCTRL__RUN_width        1
#define L2_DCR__REFCTRL__RUN_position     46
#define L2_DCR__REFCTRL__RUN_get(x)       _BGQ_GET(1,46,x)
#define L2_DCR__REFCTRL__RUN_set(v)       _BGQ_SET(1,46,v)
#define L2_DCR__REFCTRL__RUN_insert(x,v)  _BGQ_INSERT(1,46,x,v)

//   Field:       doload
//   Description: set to force load into refresh count reg
#define L2_DCR__REFCTRL__DOLOAD_width        1
#define L2_DCR__REFCTRL__DOLOAD_position     47
#define L2_DCR__REFCTRL__DOLOAD_get(x)       _BGQ_GET(1,47,x)
#define L2_DCR__REFCTRL__DOLOAD_set(v)       _BGQ_SET(1,47,v)
#define L2_DCR__REFCTRL__DOLOAD_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       interval
//   Description: interval between eDRAM refreshes
#define L2_DCR__REFCTRL__INTERVAL_width        16
#define L2_DCR__REFCTRL__INTERVAL_position     63
#define L2_DCR__REFCTRL__INTERVAL_get(x)       _BGQ_GET(16,63,x)
#define L2_DCR__REFCTRL__INTERVAL_set(v)       _BGQ_SET(16,63,v)
#define L2_DCR__REFCTRL__INTERVAL_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        lructrl0
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       2020202020202020
//   Description: LRU control
#define L2_DCR__LRUCTRL0_offset  (0x0018) // Offset of register in instance 
#define L2_DCR__LRUCTRL0_reset  UNSIGNED64(0x2020202020202020)

//   Field:       listrd
//   Description: L1p list read other LRU control
#define L2_DCR__LRUCTRL0__LISTRD_width        6
#define L2_DCR__LRUCTRL0__LISTRD_position     7
#define L2_DCR__LRUCTRL0__LISTRD_get(x)       _BGQ_GET(6,7,x)
#define L2_DCR__LRUCTRL0__LISTRD_set(v)       _BGQ_SET(6,7,v)
#define L2_DCR__LRUCTRL0__LISTRD_insert(x,v)  _BGQ_INSERT(6,7,x,v)

//   Field:       listrdlast
//   Description: L1p list read last LRU control
#define L2_DCR__LRUCTRL0__LISTRDLAST_width        6
#define L2_DCR__LRUCTRL0__LISTRDLAST_position     15
#define L2_DCR__LRUCTRL0__LISTRDLAST_get(x)       _BGQ_GET(6,15,x)
#define L2_DCR__LRUCTRL0__LISTRDLAST_set(v)       _BGQ_SET(6,15,v)
#define L2_DCR__LRUCTRL0__LISTRDLAST_insert(x,v)  _BGQ_INSERT(6,15,x,v)

//   Field:       listwr
//   Description: L1p list write other LRU control
#define L2_DCR__LRUCTRL0__LISTWR_width        6
#define L2_DCR__LRUCTRL0__LISTWR_position     23
#define L2_DCR__LRUCTRL0__LISTWR_get(x)       _BGQ_GET(6,23,x)
#define L2_DCR__LRUCTRL0__LISTWR_set(v)       _BGQ_SET(6,23,v)
#define L2_DCR__LRUCTRL0__LISTWR_insert(x,v)  _BGQ_INSERT(6,23,x,v)

//   Field:       listwrlast
//   Description: L1p list write last LRU control
#define L2_DCR__LRUCTRL0__LISTWRLAST_width        6
#define L2_DCR__LRUCTRL0__LISTWRLAST_position     31
#define L2_DCR__LRUCTRL0__LISTWRLAST_get(x)       _BGQ_GET(6,31,x)
#define L2_DCR__LRUCTRL0__LISTWRLAST_set(v)       _BGQ_SET(6,31,v)
#define L2_DCR__LRUCTRL0__LISTWRLAST_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       l1plin
//   Description: L1p linear LRU control
#define L2_DCR__LRUCTRL0__L1PLIN_width        6
#define L2_DCR__LRUCTRL0__L1PLIN_position     39
#define L2_DCR__LRUCTRL0__L1PLIN_get(x)       _BGQ_GET(6,39,x)
#define L2_DCR__LRUCTRL0__L1PLIN_set(v)       _BGQ_SET(6,39,v)
#define L2_DCR__LRUCTRL0__L1PLIN_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       l2pft
//   Description: L2 prefetch LRU control
#define L2_DCR__LRUCTRL0__L2PFT_width        6
#define L2_DCR__LRUCTRL0__L2PFT_position     47
#define L2_DCR__LRUCTRL0__L2PFT_get(x)       _BGQ_GET(6,47,x)
#define L2_DCR__LRUCTRL0__L2PFT_set(v)       _BGQ_SET(6,47,v)
#define L2_DCR__LRUCTRL0__L2PFT_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       lru1
//   Description: secondary LRU control
#define L2_DCR__LRUCTRL0__LRU1_width        6
#define L2_DCR__LRUCTRL0__LRU1_position     55
#define L2_DCR__LRUCTRL0__LRU1_get(x)       _BGQ_GET(6,55,x)
#define L2_DCR__LRUCTRL0__LRU1_set(v)       _BGQ_SET(6,55,v)
#define L2_DCR__LRUCTRL0__LRU1_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       lru0
//   Description: primary LRU control for regular hits and misses
#define L2_DCR__LRUCTRL0__LRU0_width        6
#define L2_DCR__LRUCTRL0__LRU0_position     63
#define L2_DCR__LRUCTRL0__LRU0_get(x)       _BGQ_GET(6,63,x)
#define L2_DCR__LRUCTRL0__LRU0_set(v)       _BGQ_SET(6,63,v)
#define L2_DCR__LRUCTRL0__LRU0_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        lructrl1
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       000AAA2020202020
//   Description: LRU control
#define L2_DCR__LRUCTRL1_offset  (0x0019) // Offset of register in instance 
#define L2_DCR__LRUCTRL1_reset  UNSIGNED64(0x000AAA2020202020)

//   Field:       overclup_thresh
//   Description: maximum number of cleanup lines per set
#define L2_DCR__LRUCTRL1__OVERCLUP_THRESH_width        4
#define L2_DCR__LRUCTRL1__OVERCLUP_THRESH_position     15
#define L2_DCR__LRUCTRL1__OVERCLUP_THRESH_get(x)       _BGQ_GET(4,15,x)
#define L2_DCR__LRUCTRL1__OVERCLUP_THRESH_set(v)       _BGQ_SET(4,15,v)
#define L2_DCR__LRUCTRL1__OVERCLUP_THRESH_insert(x,v)  _BGQ_INSERT(4,15,x,v)

//   Field:       spec_thresh
//   Description: maximum number of allowed speculative lines per set
#define L2_DCR__LRUCTRL1__SPEC_THRESH_width        4
#define L2_DCR__LRUCTRL1__SPEC_THRESH_position     19
#define L2_DCR__LRUCTRL1__SPEC_THRESH_get(x)       _BGQ_GET(4,19,x)
#define L2_DCR__LRUCTRL1__SPEC_THRESH_set(v)       _BGQ_SET(4,19,v)
#define L2_DCR__LRUCTRL1__SPEC_THRESH_insert(x,v)  _BGQ_INSERT(4,19,x,v)

//   Field:       overlock_thresh
//   Description: maximum number of allowed locked lines per set
#define L2_DCR__LRUCTRL1__OVERLOCK_THRESH_width        4
#define L2_DCR__LRUCTRL1__OVERLOCK_THRESH_position     23
#define L2_DCR__LRUCTRL1__OVERLOCK_THRESH_get(x)       _BGQ_GET(4,23,x)
#define L2_DCR__LRUCTRL1__OVERLOCK_THRESH_set(v)       _BGQ_SET(4,23,v)
#define L2_DCR__LRUCTRL1__OVERLOCK_THRESH_insert(x,v)  _BGQ_INSERT(4,23,x,v)

//   Field:       muwr
//   Description: MSG unit write other access LRU control
#define L2_DCR__LRUCTRL1__MUWR_width        6
#define L2_DCR__LRUCTRL1__MUWR_position     31
#define L2_DCR__LRUCTRL1__MUWR_get(x)       _BGQ_GET(6,31,x)
#define L2_DCR__LRUCTRL1__MUWR_set(v)       _BGQ_SET(6,31,v)
#define L2_DCR__LRUCTRL1__MUWR_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       muwrlast
//   Description: MSG unit write last access LRU control
#define L2_DCR__LRUCTRL1__MUWRLAST_width        6
#define L2_DCR__LRUCTRL1__MUWRLAST_position     39
#define L2_DCR__LRUCTRL1__MUWRLAST_get(x)       _BGQ_GET(6,39,x)
#define L2_DCR__LRUCTRL1__MUWRLAST_set(v)       _BGQ_SET(6,39,v)
#define L2_DCR__LRUCTRL1__MUWRLAST_insert(x,v)  _BGQ_INSERT(6,39,x,v)

//   Field:       murd
//   Description: MSG unit read other access LRU control
#define L2_DCR__LRUCTRL1__MURD_width        6
#define L2_DCR__LRUCTRL1__MURD_position     47
#define L2_DCR__LRUCTRL1__MURD_get(x)       _BGQ_GET(6,47,x)
#define L2_DCR__LRUCTRL1__MURD_set(v)       _BGQ_SET(6,47,v)
#define L2_DCR__LRUCTRL1__MURD_insert(x,v)  _BGQ_INSERT(6,47,x,v)

//   Field:       murdlast
//   Description: MSG unit read last access LRU control
#define L2_DCR__LRUCTRL1__MURDLAST_width        6
#define L2_DCR__LRUCTRL1__MURDLAST_position     55
#define L2_DCR__LRUCTRL1__MURDLAST_get(x)       _BGQ_GET(6,55,x)
#define L2_DCR__LRUCTRL1__MURDLAST_set(v)       _BGQ_SET(6,55,v)
#define L2_DCR__LRUCTRL1__MURDLAST_insert(x,v)  _BGQ_INSERT(6,55,x,v)

//   Field:       murmw
//   Description: MSG unit RMW access LRU control
#define L2_DCR__LRUCTRL1__MURMW_width        6
#define L2_DCR__LRUCTRL1__MURMW_position     63
#define L2_DCR__LRUCTRL1__MURMW_get(x)       _BGQ_GET(6,63,x)
#define L2_DCR__LRUCTRL1__MURMW_set(v)       _BGQ_SET(6,63,v)
#define L2_DCR__LRUCTRL1__MURMW_insert(x,v)  _BGQ_INSERT(6,63,x,v)

  
// Register:
//   Name:        confl_stat
//   Type:        Read/Write External
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: Conflict attributes
#define L2_DCR__CONFL_STAT_offset  (0x0020) // Offset of register in instance 
#define L2_DCR__CONFL_STAT_range  (0x10) // Range of external address space
#define L2_DCR__CONFL_STAT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: Data
#define L2_DCR__CONFL_STAT__DATA_width        64
#define L2_DCR__CONFL_STAT__DATA_position     63
#define L2_DCR__CONFL_STAT__DATA_get(x)       _BGQ_GET(64,63,x)
#define L2_DCR__CONFL_STAT__DATA_set(v)       _BGQ_SET(64,63,v)
#define L2_DCR__CONFL_STAT__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Register:
//   Name:        backaddr
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor access address
#define L2_DCR__BACKADDR_offset  (0x0030) // Offset of register in instance 
#define L2_DCR__BACKADDR_reset  UNSIGNED64(0x0000000000000000)

//   Field:       addr
//   Description: address
#define L2_DCR__BACKADDR__ADDR_width        16
#define L2_DCR__BACKADDR__ADDR_position     63
#define L2_DCR__BACKADDR__ADDR_get(x)       _BGQ_GET(16,63,x)
#define L2_DCR__BACKADDR__ADDR_set(v)       _BGQ_SET(16,63,v)
#define L2_DCR__BACKADDR__ADDR_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        backdata
//   Type:        Read/Write External
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: backdoor access data
#define L2_DCR__BACKDATA_offset  (0x0031) // Offset of register in instance 
#define L2_DCR__BACKDATA_range  (0x1) // Range of external address space
#define L2_DCR__BACKDATA_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: data
#define L2_DCR__BACKDATA__DATA_width        30
#define L2_DCR__BACKDATA__DATA_position     63
#define L2_DCR__BACKDATA__DATA_get(x)       _BGQ_GET(30,63,x)
#define L2_DCR__BACKDATA__DATA_set(v)       _BGQ_SET(30,63,v)
#define L2_DCR__BACKDATA__DATA_insert(x,v)  _BGQ_INSERT(30,63,x,v)

  
// Register:
//   Name:        upc_l2_counter
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: L2 UPC Counters 0-15 Data
#define L2_DCR__UPC_L2_COUNTER_offset  (0x0000) // Offset of register in instance 
#define L2_DCR__UPC_L2_COUNTER_range  (0x10) // Range of external address space
#define L2_DCR__UPC_L2_COUNTER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ovf
//   Description: Counter Overflow bit
#define L2_DCR__UPC_L2_COUNTER__OVF_width        1
#define L2_DCR__UPC_L2_COUNTER__OVF_position     53
#define L2_DCR__UPC_L2_COUNTER__OVF_get(x)       _BGQ_GET(1,53,x)
#define L2_DCR__UPC_L2_COUNTER__OVF_set(v)       _BGQ_SET(1,53,v)
#define L2_DCR__UPC_L2_COUNTER__OVF_insert(x,v)  _BGQ_INSERT(1,53,x,v)

//   Field:       data
//   Description: L2 Counter Data
#define L2_DCR__UPC_L2_COUNTER__DATA_width        10
#define L2_DCR__UPC_L2_COUNTER__DATA_position     63
#define L2_DCR__UPC_L2_COUNTER__DATA_get(x)       _BGQ_GET(10,63,x)
#define L2_DCR__UPC_L2_COUNTER__DATA_set(v)       _BGQ_SET(10,63,v)
#define L2_DCR__UPC_L2_COUNTER__DATA_insert(x,v)  _BGQ_INSERT(10,63,x,v)

  
// Register:
//   Name:        upc_l2_counter_control_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: L2 UPC Counters 0-15 RW Control Register
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW_offset  (0x0010) // Offset of register in instance 
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW_range  (0x1) // Range of external address space
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: L2 Counter Reset 0-15(RW)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__RESET_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__RESET_position     15
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__RESET_get(x)       _BGQ_GET(16,15,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__RESET_set(v)       _BGQ_SET(16,15,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__RESET_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       enable
//   Description: L2 Counter Enable 0-15(RW)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__ENABLE_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__ENABLE_position     47
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__ENABLE_get(x)       _BGQ_GET(16,47,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__ENABLE_set(v)       _BGQ_SET(16,47,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_RW__ENABLE_insert(x,v)  _BGQ_INSERT(16,47,x,v)

  
// Register:
//   Name:        upc_l2_counter_control_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: L2 UPC Counters 0-15 W1S Control Register
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S_offset  (0x0011) // Offset of register in instance 
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S_range  (0x1) // Range of external address space
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: L2 Counter Reset 0-15(W1S)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__RESET_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__RESET_position     15
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__RESET_get(x)       _BGQ_GET(16,15,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__RESET_set(v)       _BGQ_SET(16,15,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__RESET_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       enable
//   Description: L2 Counter Enable 0-15(W1S)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__ENABLE_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__ENABLE_position     47
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__ENABLE_get(x)       _BGQ_GET(16,47,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__ENABLE_set(v)       _BGQ_SET(16,47,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(16,47,x,v)

  
// Register:
//   Name:        upc_l2_counter_control_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: L2 UPC Counters 0-15 W1C Control Register
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C_offset  (0x0012) // Offset of register in instance 
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C_range  (0x1) // Range of external address space
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: L2 Counter Reset 0-15(W1C)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__RESET_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__RESET_position     15
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__RESET_get(x)       _BGQ_GET(16,15,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__RESET_set(v)       _BGQ_SET(16,15,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__RESET_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       enable
//   Description: L2 Counter Enable 0-15(W1C)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__ENABLE_width        16
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__ENABLE_position     47
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__ENABLE_get(x)       _BGQ_GET(16,47,x)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__ENABLE_set(v)       _BGQ_SET(16,47,v)
#define L2_DCR__UPC_L2_COUNTER_CONTROL_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(16,47,x,v)

  
// Register:
//   Name:        upc_l2_config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_L2 Configuration Register
#define L2_DCR__UPC_L2_CONFIG_offset  (0x0013) // Offset of register in instance 
#define L2_DCR__UPC_L2_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_l2_ring_enable
//   Description: Enables UPC_L2 Ring logic. Must be set to one to pass daisy chain data.
#define L2_DCR__UPC_L2_CONFIG__UPC_L2_RING_ENABLE_width        1
#define L2_DCR__UPC_L2_CONFIG__UPC_L2_RING_ENABLE_position     3
#define L2_DCR__UPC_L2_CONFIG__UPC_L2_RING_ENABLE_get(x)       _BGQ_GET(1,3,x)
#define L2_DCR__UPC_L2_CONFIG__UPC_L2_RING_ENABLE_set(v)       _BGQ_SET(1,3,v)
#define L2_DCR__UPC_L2_CONFIG__UPC_L2_RING_ENABLE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       sync_override
//   Description: Sync Override bits for counters 0-15
#define L2_DCR__UPC_L2_CONFIG__SYNC_OVERRIDE_width        16
#define L2_DCR__UPC_L2_CONFIG__SYNC_OVERRIDE_position     63
#define L2_DCR__UPC_L2_CONFIG__SYNC_OVERRIDE_get(x)       _BGQ_GET(16,63,x)
#define L2_DCR__UPC_L2_CONFIG__SYNC_OVERRIDE_set(v)       _BGQ_SET(16,63,v)
#define L2_DCR__UPC_L2_CONFIG__SYNC_OVERRIDE_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x010F) // Offset of register in instance 
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x0110) // Offset of register in instance 
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x0111) // Offset of register in instance 
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: l2_interrupt_state
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define L2_DCR__L2_INTERRUPT_STATE__STATE_offset         (0x0100) // Offset of State register in instance
#define L2_DCR__L2_INTERRUPT_STATE__FIRST_offset         (0x0104) // Offset of First register in instance
#define L2_DCR__L2_INTERRUPT_STATE__FORCE_offset         (0x0103) // Offset of Force register in instance
#define L2_DCR__L2_INTERRUPT_STATE__MACHINE_CHECK_offset (0x0105) // Offset of Machine Check register in instance
#define L2_DCR__L2_INTERRUPT_STATE__CRITICAL_offset      (0x0106) // Offset of Critical register in instance
#define L2_DCR__L2_INTERRUPT_STATE__NONCRITICAL_offset   (0x0107) // Offset of Noncritical register in instance

#define L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_width        1
#define L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_position     0
#define L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_get(x)       _BGQ_GET(1,0,x)
#define L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_set(v)       _BGQ_SET(1,0,v)
#define L2_DCR__L2_INTERRUPT_STATE__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_width        1
#define L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_position     1
#define L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_get(x)       _BGQ_GET(1,1,x)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_set(v)       _BGQ_SET(1,1,v)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_ERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_width        1
#define L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_position     2
#define L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_get(x)       _BGQ_GET(1,2,x)
#define L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_set(v)       _BGQ_SET(1,2,v)
#define L2_DCR__L2_INTERRUPT_STATE__CTRL_ERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_width        1
#define L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_position     3
#define L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_get(x)       _BGQ_GET(1,3,x)
#define L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_set(v)       _BGQ_SET(1,3,v)
#define L2_DCR__L2_INTERRUPT_STATE__RDB_ERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__COH_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__COH_CE_position     4
#define L2_DCR__L2_INTERRUPT_STATE__COH_CE_get(x)       _BGQ_GET(1,4,x)
#define L2_DCR__L2_INTERRUPT_STATE__COH_CE_set(v)       _BGQ_SET(1,4,v)
#define L2_DCR__L2_INTERRUPT_STATE__COH_CE_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__COH_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__COH_UE_position     5
#define L2_DCR__L2_INTERRUPT_STATE__COH_UE_get(x)       _BGQ_GET(1,5,x)
#define L2_DCR__L2_INTERRUPT_STATE__COH_UE_set(v)       _BGQ_SET(1,5,v)
#define L2_DCR__L2_INTERRUPT_STATE__COH_UE_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_position     6
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_get(x)       _BGQ_GET(1,6,x)
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_set(v)       _BGQ_SET(1,6,v)
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_CE_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_position     7
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_get(x)       _BGQ_GET(1,7,x)
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_set(v)       _BGQ_SET(1,7,v)
#define L2_DCR__L2_INTERRUPT_STATE__DIRB_UE_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__EDR_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__EDR_CE_position     8
#define L2_DCR__L2_INTERRUPT_STATE__EDR_CE_get(x)       _BGQ_GET(1,8,x)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_CE_set(v)       _BGQ_SET(1,8,v)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_CE_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_position     9
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_get(x)       _BGQ_GET(1,9,x)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_set(v)       _BGQ_SET(1,9,v)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OCE_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_position     10
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_get(x)       _BGQ_GET(1,10,x)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_set(v)       _BGQ_SET(1,10,v)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_OUE_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__EDR_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__EDR_UE_position     11
#define L2_DCR__L2_INTERRUPT_STATE__EDR_UE_get(x)       _BGQ_GET(1,11,x)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_UE_set(v)       _BGQ_SET(1,11,v)
#define L2_DCR__L2_INTERRUPT_STATE__EDR_UE_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__LRU_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__LRU_CE_position     12
#define L2_DCR__L2_INTERRUPT_STATE__LRU_CE_get(x)       _BGQ_GET(1,12,x)
#define L2_DCR__L2_INTERRUPT_STATE__LRU_CE_set(v)       _BGQ_SET(1,12,v)
#define L2_DCR__L2_INTERRUPT_STATE__LRU_CE_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__LRU_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__LRU_UE_position     13
#define L2_DCR__L2_INTERRUPT_STATE__LRU_UE_get(x)       _BGQ_GET(1,13,x)
#define L2_DCR__L2_INTERRUPT_STATE__LRU_UE_set(v)       _BGQ_SET(1,13,v)
#define L2_DCR__L2_INTERRUPT_STATE__LRU_UE_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__SPR_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__SPR_CE_position     14
#define L2_DCR__L2_INTERRUPT_STATE__SPR_CE_get(x)       _BGQ_GET(1,14,x)
#define L2_DCR__L2_INTERRUPT_STATE__SPR_CE_set(v)       _BGQ_SET(1,14,v)
#define L2_DCR__L2_INTERRUPT_STATE__SPR_CE_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__SPR_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__SPR_UE_position     15
#define L2_DCR__L2_INTERRUPT_STATE__SPR_UE_get(x)       _BGQ_GET(1,15,x)
#define L2_DCR__L2_INTERRUPT_STATE__SPR_UE_set(v)       _BGQ_SET(1,15,v)
#define L2_DCR__L2_INTERRUPT_STATE__SPR_UE_insert(x,v)  _BGQ_INSERT(1,15,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__RQ_CE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__RQ_CE_position     16
#define L2_DCR__L2_INTERRUPT_STATE__RQ_CE_get(x)       _BGQ_GET(1,16,x)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_CE_set(v)       _BGQ_SET(1,16,v)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_CE_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__RQ_UE_width        1
#define L2_DCR__L2_INTERRUPT_STATE__RQ_UE_position     17
#define L2_DCR__L2_INTERRUPT_STATE__RQ_UE_get(x)       _BGQ_GET(1,17,x)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_UE_set(v)       _BGQ_SET(1,17,v)
#define L2_DCR__L2_INTERRUPT_STATE__RQ_UE_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_width        1
#define L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_position     18
#define L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_get(x)       _BGQ_GET(1,18,x)
#define L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_set(v)       _BGQ_SET(1,18,v)
#define L2_DCR__L2_INTERRUPT_STATE__OVERLOCK_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define L2_DCR__L2_INTERRUPT_STATE__UPC_COUNTER_OVF_width        1
#define L2_DCR__L2_INTERRUPT_STATE__UPC_COUNTER_OVF_position     19
#define L2_DCR__L2_INTERRUPT_STATE__UPC_COUNTER_OVF_get(x)       _BGQ_GET(1,19,x)
#define L2_DCR__L2_INTERRUPT_STATE__UPC_COUNTER_OVF_set(v)       _BGQ_SET(1,19,v)
#define L2_DCR__L2_INTERRUPT_STATE__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,19,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0108) // Offset of State register in instance
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x010B) // Offset of First register in instance
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x010A) // Offset of Force register in instance
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x010C) // Offset of Machine Check register in instance
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x010D) // Offset of Critical register in instance
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x010E) // Offset of Noncritical register in instance

#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: l2_interrupt_state_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_LOW_offset         (0x0101) // Offset of Control register in instance
  
// Interrupt Control Register: l2_interrupt_state_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH_offset         (0x0102) // Offset of Control register in instance

#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_position     1
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_get(x)       _BGQ_GET(2,1,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_set(v)       _BGQ_SET(2,1,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_ERR_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_ERR_position     3
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_ERR_get(x)       _BGQ_GET(2,3,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_ERR_set(v)       _BGQ_SET(2,3,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_ERR_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__CTRL_ERR_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__CTRL_ERR_position     5
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__CTRL_ERR_get(x)       _BGQ_GET(2,5,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__CTRL_ERR_set(v)       _BGQ_SET(2,5,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__CTRL_ERR_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RDB_ERR_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RDB_ERR_position     7
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RDB_ERR_get(x)       _BGQ_GET(2,7,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RDB_ERR_set(v)       _BGQ_SET(2,7,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RDB_ERR_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_CE_position     9
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_CE_get(x)       _BGQ_GET(2,9,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_CE_set(v)       _BGQ_SET(2,9,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_CE_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_UE_position     11
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_UE_get(x)       _BGQ_GET(2,11,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_UE_set(v)       _BGQ_SET(2,11,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__COH_UE_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_CE_position     13
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_CE_get(x)       _BGQ_GET(2,13,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_CE_set(v)       _BGQ_SET(2,13,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_CE_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_UE_position     15
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_UE_get(x)       _BGQ_GET(2,15,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_UE_set(v)       _BGQ_SET(2,15,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__DIRB_UE_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_CE_position     17
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_CE_get(x)       _BGQ_GET(2,17,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_CE_set(v)       _BGQ_SET(2,17,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_CE_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OCE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OCE_position     19
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OCE_get(x)       _BGQ_GET(2,19,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OCE_set(v)       _BGQ_SET(2,19,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OCE_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OUE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OUE_position     21
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OUE_get(x)       _BGQ_GET(2,21,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OUE_set(v)       _BGQ_SET(2,21,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_OUE_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_UE_position     23
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_UE_get(x)       _BGQ_GET(2,23,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_UE_set(v)       _BGQ_SET(2,23,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__EDR_UE_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_CE_position     25
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_CE_get(x)       _BGQ_GET(2,25,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_CE_set(v)       _BGQ_SET(2,25,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_CE_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_UE_position     27
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_UE_get(x)       _BGQ_GET(2,27,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_UE_set(v)       _BGQ_SET(2,27,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__LRU_UE_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_CE_position     29
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_CE_get(x)       _BGQ_GET(2,29,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_CE_set(v)       _BGQ_SET(2,29,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_CE_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_UE_position     31
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_UE_get(x)       _BGQ_GET(2,31,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_UE_set(v)       _BGQ_SET(2,31,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__SPR_UE_insert(x,v)  _BGQ_INSERT(2,31,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_CE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_CE_position     33
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_CE_get(x)       _BGQ_GET(2,33,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_CE_set(v)       _BGQ_SET(2,33,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_CE_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_UE_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_UE_position     35
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_UE_get(x)       _BGQ_GET(2,35,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_UE_set(v)       _BGQ_SET(2,35,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__RQ_UE_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__OVERLOCK_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__OVERLOCK_position     37
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__OVERLOCK_get(x)       _BGQ_GET(2,37,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__OVERLOCK_set(v)       _BGQ_SET(2,37,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__OVERLOCK_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__UPC_COUNTER_OVF_width        2
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__UPC_COUNTER_OVF_position     39
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__UPC_COUNTER_OVF_get(x)       _BGQ_GET(2,39,x)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__UPC_COUNTER_OVF_set(v)       _BGQ_SET(2,39,v)
#define L2_DCR__L2_INTERRUPT_STATE_CONTROL_HIGH__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,39,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0109) // Offset of Control register in instance

#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define L2_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct l2_dcr { 
    /* 0x0000 */ uint64_t upc_l2_counter[L2_DCR__UPC_L2_COUNTER_range];
    /* 0x0010 */ uint64_t upc_l2_counter_control_rw;
    /* 0x0011 */ uint64_t upc_l2_counter_control_w1s;
    /* 0x0012 */ uint64_t upc_l2_counter_control_w1c;
    /* 0x0013 */ uint64_t upc_l2_config;
    /* 0x0014 */ uint64_t ctrl;
    /* 0x0015 */ uint64_t status;
    /* 0x0016 */ uint64_t replpol;
    /* 0x0017 */ uint64_t refctrl;
    /* 0x0018 */ uint64_t lructrl0;
    /* 0x0019 */ uint64_t lructrl1;
    /* 0x001A */ uint64_t reserve_001A[0x0006];
    /* 0x0020 */ uint64_t confl_stat[L2_DCR__CONFL_STAT_range];
    /* 0x0030 */ uint64_t backaddr;
    /* 0x0031 */ uint64_t backdata;
    /* 0x0032 */ uint64_t reserve_0032[0x00CE];
    /* 0x0100 */ uint64_t l2_interrupt_state__state;
    /* 0x0101 */ uint64_t l2_interrupt_state_control_low;
    /* 0x0102 */ uint64_t l2_interrupt_state_control_high;
    /* 0x0103 */ uint64_t l2_interrupt_state__force;
    /* 0x0104 */ uint64_t l2_interrupt_state__first;
    /* 0x0105 */ uint64_t l2_interrupt_state__machine_check;
    /* 0x0106 */ uint64_t l2_interrupt_state__critical;
    /* 0x0107 */ uint64_t l2_interrupt_state__noncritical;
    /* 0x0108 */ uint64_t interrupt_internal_error__state;
    /* 0x0109 */ uint64_t interrupt_internal_error_control_high;
    /* 0x010A */ uint64_t interrupt_internal_error__force;
    /* 0x010B */ uint64_t interrupt_internal_error__first;
    /* 0x010C */ uint64_t interrupt_internal_error__machine_check;
    /* 0x010D */ uint64_t interrupt_internal_error__critical;
    /* 0x010E */ uint64_t interrupt_internal_error__noncritical;
    /* 0x010F */ uint64_t interrupt_internal_error_sw_info;
    /* 0x0110 */ uint64_t interrupt_internal_error_hw_info;
} l2_dcr_t;
  
#define L2_DCR_USER_PTR(i) _DCR_USER_PTR__INDEX(l2_dcr_t, L2_DCR, i)
#define L2_DCR_PRIV_PTR(i) _DCR_PRIV_PTR__INDEX(l2_dcr_t, L2_DCR, i)
  
#define L2_DCR_USER_PTR__Core00 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core00)
#define L2_DCR_USER_PTR__Core01 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core01)
#define L2_DCR_USER_PTR__Core02 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core02)
#define L2_DCR_USER_PTR__Core03 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core03)
#define L2_DCR_USER_PTR__Core04 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core04)
#define L2_DCR_USER_PTR__Core05 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core05)
#define L2_DCR_USER_PTR__Core06 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core06)
#define L2_DCR_USER_PTR__Core07 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core07)
#define L2_DCR_USER_PTR__Core08 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core08)
#define L2_DCR_USER_PTR__Core09 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core09)
#define L2_DCR_USER_PTR__Core10 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core10)
#define L2_DCR_USER_PTR__Core11 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core11)
#define L2_DCR_USER_PTR__Core12 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core12)
#define L2_DCR_USER_PTR__Core13 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core13)
#define L2_DCR_USER_PTR__Core14 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core14)
#define L2_DCR_USER_PTR__Core15 _DCR_USER_PTR__NAME(l2_dcr_t, L2_DCR, Core15)
  
#define L2_DCR_PRIV_PTR__Core00 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core00)
#define L2_DCR_PRIV_PTR__Core01 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core01)
#define L2_DCR_PRIV_PTR__Core02 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core02)
#define L2_DCR_PRIV_PTR__Core03 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core03)
#define L2_DCR_PRIV_PTR__Core04 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core04)
#define L2_DCR_PRIV_PTR__Core05 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core05)
#define L2_DCR_PRIV_PTR__Core06 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core06)
#define L2_DCR_PRIV_PTR__Core07 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core07)
#define L2_DCR_PRIV_PTR__Core08 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core08)
#define L2_DCR_PRIV_PTR__Core09 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core09)
#define L2_DCR_PRIV_PTR__Core10 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core10)
#define L2_DCR_PRIV_PTR__Core11 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core11)
#define L2_DCR_PRIV_PTR__Core12 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core12)
#define L2_DCR_PRIV_PTR__Core13 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core13)
#define L2_DCR_PRIV_PTR__Core14 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core14)
#define L2_DCR_PRIV_PTR__Core15 _DCR_PRIV_PTR__NAME(l2_dcr_t, L2_DCR, Core15)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
