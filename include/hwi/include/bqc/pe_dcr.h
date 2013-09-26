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

#ifndef   _PE_DCR__H_ // Prevent multiple inclusion
#define   _PE_DCR__H_
  
__BEGIN_DECLS
  
#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/dcr_support.h>
  
#define PE_DCR_base    (0x0D0000)  // Base address for all instances
#define PE_DCR_size    (0x0200)    // Size of instance (largest offset + 1 )
  
#define PE_DCR(reg) _DCR_REG(PE_DCR, reg)
  
// Register:
//   Name:        pcie_utl
//   Type:        Read/Write External
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCI Express UTL Core registers. See UTL Specification for details
#define PE_DCR__PCIE_UTL_offset  (0x0100) // Offset of register in instance 
#define PE_DCR__PCIE_UTL_range  (0x100) // Range of external address space
#define PE_DCR__PCIE_UTL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: Data to/from PCIE UTL Core
#define PE_DCR__PCIE_UTL__DATA_width        32
#define PE_DCR__PCIE_UTL__DATA_position     63
#define PE_DCR__PCIE_UTL__DATA_get(x)       _BGQ_GET(32,63,x)
#define PE_DCR__PCIE_UTL__DATA_set(v)       _BGQ_SET(32,63,v)
#define PE_DCR__PCIE_UTL__DATA_insert(x,v)  _BGQ_INSERT(32,63,x,v)

  
// Register:
//   Name:        pcie_config
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0B50101020110400
//   Description: PCIE Configuration Register
#define PE_DCR__PCIE_CONFIG_offset  (0x0000) // Offset of register in instance 
#define PE_DCR__PCIE_CONFIG_reset  UNSIGNED64(0x0B50101020110400)

//   Field:       sys_out_pdat_tot
//   Description: Size of outbound data buffer for all VCs
#define PE_DCR__PCIE_CONFIG__SYS_OUT_PDAT_TOT_width        3
#define PE_DCR__PCIE_CONFIG__SYS_OUT_PDAT_TOT_position     2
#define PE_DCR__PCIE_CONFIG__SYS_OUT_PDAT_TOT_get(x)       _BGQ_GET(3,2,x)
#define PE_DCR__PCIE_CONFIG__SYS_OUT_PDAT_TOT_set(v)       _BGQ_SET(3,2,v)
#define PE_DCR__PCIE_CONFIG__SYS_OUT_PDAT_TOT_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       sys_pcie_nptrans_tot
//   Description: Maximum number of non-posted transactions outstanding on PCI Express
#define PE_DCR__PCIE_CONFIG__SYS_PCIE_NPTRANS_TOT_width        2
#define PE_DCR__PCIE_CONFIG__SYS_PCIE_NPTRANS_TOT_position     4
#define PE_DCR__PCIE_CONFIG__SYS_PCIE_NPTRANS_TOT_get(x)       _BGQ_GET(2,4,x)
#define PE_DCR__PCIE_CONFIG__SYS_PCIE_NPTRANS_TOT_set(v)       _BGQ_SET(2,4,v)
#define PE_DCR__PCIE_CONFIG__SYS_PCIE_NPTRANS_TOT_insert(x,v)  _BGQ_INSERT(2,4,x,v)

//   Field:       sys_in_pdat_tot
//   Description: Size of inbound posted data buffer for all VCs 0 = 0.5KB 1 = 1KB 2 = 2KB 3=4KB 4=8KB 5=16KB 6=32KB 7=64KB
#define PE_DCR__PCIE_CONFIG__SYS_IN_PDAT_TOT_width        3
#define PE_DCR__PCIE_CONFIG__SYS_IN_PDAT_TOT_position     7
#define PE_DCR__PCIE_CONFIG__SYS_IN_PDAT_TOT_get(x)       _BGQ_GET(3,7,x)
#define PE_DCR__PCIE_CONFIG__SYS_IN_PDAT_TOT_set(v)       _BGQ_SET(3,7,v)
#define PE_DCR__PCIE_CONFIG__SYS_IN_PDAT_TOT_insert(x,v)  _BGQ_INSERT(3,7,x,v)

//   Field:       sys_gbif_nptrans_tot
//   Description: Maximum number of inbound non-posted transactions outstanding on GBIF
#define PE_DCR__PCIE_CONFIG__SYS_GBIF_NPTRANS_TOT_width        2
#define PE_DCR__PCIE_CONFIG__SYS_GBIF_NPTRANS_TOT_position     9
#define PE_DCR__PCIE_CONFIG__SYS_GBIF_NPTRANS_TOT_get(x)       _BGQ_GET(2,9,x)
#define PE_DCR__PCIE_CONFIG__SYS_GBIF_NPTRANS_TOT_set(v)       _BGQ_SET(2,9,v)
#define PE_DCR__PCIE_CONFIG__SYS_GBIF_NPTRANS_TOT_insert(x,v)  _BGQ_INSERT(2,9,x,v)

//   Field:       link_init_en
//   Description: When set, enables buffer allocation registers initialization with the values specified by SYS_LINK_INIT_* strap inputs after reset. When reset, buffer allocation registers are initialized to 0 and require software initialization prior to link activation
#define PE_DCR__PCIE_CONFIG__LINK_INIT_EN_width        1
#define PE_DCR__PCIE_CONFIG__LINK_INIT_EN_position     10
#define PE_DCR__PCIE_CONFIG__LINK_INIT_EN_get(x)       _BGQ_GET(1,10,x)
#define PE_DCR__PCIE_CONFIG__LINK_INIT_EN_set(v)       _BGQ_SET(1,10,v)
#define PE_DCR__PCIE_CONFIG__LINK_INIT_EN_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       sys_link_init_rxph0
//   Description: The initial number of Inbound Posted Header credits allocated to VC0 - initializes the IP0H field in the Inbound Posted Header Buffer Allocation Register
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPH0_width        6
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPH0_position     16
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPH0_get(x)       _BGQ_GET(6,16,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPH0_set(v)       _BGQ_SET(6,16,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPH0_insert(x,v)  _BGQ_INSERT(6,16,x,v)

//   Field:       sys_link_init_rxpd0
//   Description: The initial number of Inbound Posted Data credits allocated to VC0 - initializes the IP0D field in the Inbound Posted Data Buffer Allocation Register (in units of 128 bytes)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPD0_width        8
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPD0_position     24
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPD0_get(x)       _BGQ_GET(8,24,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPD0_set(v)       _BGQ_SET(8,24,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXPD0_insert(x,v)  _BGQ_INSERT(8,24,x,v)

//   Field:       sys_link_init_rxnph0
//   Description: The initial number of Inbound Non-Posted Header credits allocated to VC0 - initializes the IN0H field in the Inbound Non-Posted Header Buffer Allocation Register
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXNPH0_width        6
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXNPH0_position     31
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXNPH0_get(x)       _BGQ_GET(6,31,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXNPH0_set(v)       _BGQ_SET(6,31,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_RXNPH0_insert(x,v)  _BGQ_INSERT(6,31,x,v)

//   Field:       sys_link_init_gbiftags0
//   Description:  The initial number of Inbound GBIF read tags allocated to VC0 - initializes the IC0T field in the GBIF Read Tags Allocation Register
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_GBIFTAGS0_width        6
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_GBIFTAGS0_position     37
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_GBIFTAGS0_get(x)       _BGQ_GET(6,37,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_GBIFTAGS0_set(v)       _BGQ_SET(6,37,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_INIT_GBIFTAGS0_insert(x,v)  _BGQ_INSERT(6,37,x,v)

//   Field:       sys_link_txpd0
//   Description: The initial size of Outbound Posted Data buffer tags allocated to VC0 - initializes the OP0D field in the Outbound Posted Data Buffer Allocation Register (in units of 128 bytes)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_TXPD0_width        7
#define PE_DCR__PCIE_CONFIG__SYS_LINK_TXPD0_position     44
#define PE_DCR__PCIE_CONFIG__SYS_LINK_TXPD0_get(x)       _BGQ_GET(7,44,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_TXPD0_set(v)       _BGQ_SET(7,44,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_TXPD0_insert(x,v)  _BGQ_INSERT(7,44,x,v)

//   Field:       sys_link_pcitags
//   Description: The initial number of outbound PCI read tags allocated to VC0 - initializes the OC0T field in the PCI Express Tags Allocation Register
#define PE_DCR__PCIE_CONFIG__SYS_LINK_PCITAGS_width        6
#define PE_DCR__PCIE_CONFIG__SYS_LINK_PCITAGS_position     50
#define PE_DCR__PCIE_CONFIG__SYS_LINK_PCITAGS_get(x)       _BGQ_GET(6,50,x)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_PCITAGS_set(v)       _BGQ_SET(6,50,v)
#define PE_DCR__PCIE_CONFIG__SYS_LINK_PCITAGS_insert(x,v)  _BGQ_INSERT(6,50,x,v)

//   Field:       txswing
//   Description: Transmit Swing
#define PE_DCR__PCIE_CONFIG__TXSWING_width        1
#define PE_DCR__PCIE_CONFIG__TXSWING_position     51
#define PE_DCR__PCIE_CONFIG__TXSWING_get(x)       _BGQ_GET(1,51,x)
#define PE_DCR__PCIE_CONFIG__TXSWING_set(v)       _BGQ_SET(1,51,v)
#define PE_DCR__PCIE_CONFIG__TXSWING_insert(x,v)  _BGQ_INSERT(1,51,x,v)

//   Field:       ctcdisable
//   Description: Clock Tolerance Disable
#define PE_DCR__PCIE_CONFIG__CTCDISABLE_width        1
#define PE_DCR__PCIE_CONFIG__CTCDISABLE_position     52
#define PE_DCR__PCIE_CONFIG__CTCDISABLE_get(x)       _BGQ_GET(1,52,x)
#define PE_DCR__PCIE_CONFIG__CTCDISABLE_set(v)       _BGQ_SET(1,52,v)
#define PE_DCR__PCIE_CONFIG__CTCDISABLE_insert(x,v)  _BGQ_INSERT(1,52,x,v)

//   Field:       sys_eidleontxerror
//   Description: Go to electrical idle on TX error
#define PE_DCR__PCIE_CONFIG__SYS_EIDLEONTXERROR_width        1
#define PE_DCR__PCIE_CONFIG__SYS_EIDLEONTXERROR_position     53
#define PE_DCR__PCIE_CONFIG__SYS_EIDLEONTXERROR_get(x)       _BGQ_GET(1,53,x)
#define PE_DCR__PCIE_CONFIG__SYS_EIDLEONTXERROR_set(v)       _BGQ_SET(1,53,v)
#define PE_DCR__PCIE_CONFIG__SYS_EIDLEONTXERROR_insert(x,v)  _BGQ_INSERT(1,53,x,v)

  
// Register:
//   Name:        pcie_sourceid
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Source ID Register
#define PE_DCR__PCIE_SOURCEID_offset  (0x0001) // Offset of register in instance 
#define PE_DCR__PCIE_SOURCEID_reset  UNSIGNED64(0x0000000000000000)

//   Field:       sourceid
//   Description: Source ID
#define PE_DCR__PCIE_SOURCEID__SOURCEID_width        16
#define PE_DCR__PCIE_SOURCEID__SOURCEID_position     15
#define PE_DCR__PCIE_SOURCEID__SOURCEID_get(x)       _BGQ_GET(16,15,x)
#define PE_DCR__PCIE_SOURCEID__SOURCEID_set(v)       _BGQ_SET(16,15,v)
#define PE_DCR__PCIE_SOURCEID__SOURCEID_insert(x,v)  _BGQ_INSERT(16,15,x,v)

  
// Register:
//   Name:        pcie_dl_config
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0040202000643000
//   Description: PCIE Data Link Config Register
#define PE_DCR__PCIE_DL_CONFIG_offset  (0x0002) // Offset of register in instance 
#define PE_DCR__PCIE_DL_CONFIG_reset  UNSIGNED64(0x0040202000643000)

//   Field:       rxcredittimeout_vc0
//   Description:  This signal controls the receiver credit update interval for VC0. Defined settings are: 000 - Disabled (schedule as soon as possible) 001 - 64 ns 010 - 128 ns 011 - 256 ns 100 - 512 ns 101 - 1 ms 110 - 2 ms 
#define PE_DCR__PCIE_DL_CONFIG__RXCREDITTIMEOUT_VC0_width        3
#define PE_DCR__PCIE_DL_CONFIG__RXCREDITTIMEOUT_VC0_position     2
#define PE_DCR__PCIE_DL_CONFIG__RXCREDITTIMEOUT_VC0_get(x)       _BGQ_GET(3,2,x)
#define PE_DCR__PCIE_DL_CONFIG__RXCREDITTIMEOUT_VC0_set(v)       _BGQ_SET(3,2,v)
#define PE_DCR__PCIE_DL_CONFIG__RXCREDITTIMEOUT_VC0_insert(x,v)  _BGQ_INSERT(3,2,x,v)

//   Field:       tctx_scrambleoff
//   Description: Forces Scramble Off mode on the link
#define PE_DCR__PCIE_DL_CONFIG__TCTX_SCRAMBLEOFF_width        1
#define PE_DCR__PCIE_DL_CONFIG__TCTX_SCRAMBLEOFF_position     3
#define PE_DCR__PCIE_DL_CONFIG__TCTX_SCRAMBLEOFF_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_DL_CONFIG__TCTX_SCRAMBLEOFF_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_DL_CONFIG__TCTX_SCRAMBLEOFF_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       tctx_loopback
//   Description: Forces Loopback mode on the link
#define PE_DCR__PCIE_DL_CONFIG__TCTX_LOOPBACK_width        1
#define PE_DCR__PCIE_DL_CONFIG__TCTX_LOOPBACK_position     4
#define PE_DCR__PCIE_DL_CONFIG__TCTX_LOOPBACK_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_DL_CONFIG__TCTX_LOOPBACK_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_DL_CONFIG__TCTX_LOOPBACK_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       tx_rplbuffersize
//   Description: xmit buffer replay size in lines
#define PE_DCR__PCIE_DL_CONFIG__TX_RPLBUFFERSIZE_width        13
#define PE_DCR__PCIE_DL_CONFIG__TX_RPLBUFFERSIZE_position     17
#define PE_DCR__PCIE_DL_CONFIG__TX_RPLBUFFERSIZE_get(x)       _BGQ_GET(13,17,x)
#define PE_DCR__PCIE_DL_CONFIG__TX_RPLBUFFERSIZE_set(v)       _BGQ_SET(13,17,v)
#define PE_DCR__PCIE_DL_CONFIG__TX_RPLBUFFERSIZE_insert(x,v)  _BGQ_INSERT(13,17,x,v)

//   Field:       localnfts_gen1
//   Description: This signal defines the number of FTS-ordered sets required by the local device?s receiver to exit the L0s state when operating at Generation 1 data rate (2.5 Gbps). It must not be changed after SYS_RESETN is deasserted.
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN1_width        8
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN1_position     25
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN1_get(x)       _BGQ_GET(8,25,x)
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN1_set(v)       _BGQ_SET(8,25,v)
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN1_insert(x,v)  _BGQ_INSERT(8,25,x,v)

//   Field:       localnfts_gen2
//   Description: This signal defines the number of FTS-ordered sets required by the local device?s receiver to exit the L0s state when operating at Generation 2 data rate (5.0 Gbps). It must not be changed after SYS_RESETN is deasserted.
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN2_width        8
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN2_position     33
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN2_get(x)       _BGQ_GET(8,33,x)
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN2_set(v)       _BGQ_SET(8,33,v)
#define PE_DCR__PCIE_DL_CONFIG__LOCALNFTS_GEN2_insert(x,v)  _BGQ_INSERT(8,33,x,v)

//   Field:       laneswap
//   Description: This signal defines the assignment of lanes at the PIPE interface. It must not be changed after SYS_RESETN is deasserted.
#define PE_DCR__PCIE_DL_CONFIG__LANESWAP_width        1
#define PE_DCR__PCIE_DL_CONFIG__LANESWAP_position     34
#define PE_DCR__PCIE_DL_CONFIG__LANESWAP_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__PCIE_DL_CONFIG__LANESWAP_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__PCIE_DL_CONFIG__LANESWAP_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       l0stime
//   Description: This signal defines the transmitter link L0s entry policy as a number of clock cycles. This field must be set to a value between 10 and 1750 decimal
#define PE_DCR__PCIE_DL_CONFIG__L0STIME_width        13
#define PE_DCR__PCIE_DL_CONFIG__L0STIME_position     47
#define PE_DCR__PCIE_DL_CONFIG__L0STIME_get(x)       _BGQ_GET(13,47,x)
#define PE_DCR__PCIE_DL_CONFIG__L0STIME_set(v)       _BGQ_SET(13,47,v)
#define PE_DCR__PCIE_DL_CONFIG__L0STIME_insert(x,v)  _BGQ_INSERT(13,47,x,v)

//   Field:       dllptlpratio
//   Description: defines the number of DLLPs that can be transmitted for each pending TLP. The recommended setting is 4'h3. See Section 4.4.8 of PCIEXG2TLDLP core databook for more information on tuning the performance of the link with this signal. Note: This signal must be set to a value of 1 to 14. It must not be set to 0 or 15.
#define PE_DCR__PCIE_DL_CONFIG__DLLPTLPRATIO_width        4
#define PE_DCR__PCIE_DL_CONFIG__DLLPTLPRATIO_position     51
#define PE_DCR__PCIE_DL_CONFIG__DLLPTLPRATIO_get(x)       _BGQ_GET(4,51,x)
#define PE_DCR__PCIE_DL_CONFIG__DLLPTLPRATIO_set(v)       _BGQ_SET(4,51,v)
#define PE_DCR__PCIE_DL_CONFIG__DLLPTLPRATIO_insert(x,v)  _BGQ_INSERT(4,51,x,v)

  
// Register:
//   Name:        pcie_dl_status
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE Data Link Status Register
#define PE_DCR__PCIE_DL_STATUS_offset  (0x0003) // Offset of register in instance 
#define PE_DCR__PCIE_DL_STATUS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       spare
//   Description: Spare status signal
#define PE_DCR__PCIE_DL_STATUS__SPARE_width        16
#define PE_DCR__PCIE_DL_STATUS__SPARE_position     31
#define PE_DCR__PCIE_DL_STATUS__SPARE_get(x)       _BGQ_GET(16,31,x)
#define PE_DCR__PCIE_DL_STATUS__SPARE_set(v)       _BGQ_SET(16,31,v)
#define PE_DCR__PCIE_DL_STATUS__SPARE_insert(x,v)  _BGQ_INSERT(16,31,x,v)

//   Field:       dl_linkup
//   Description: This signal is asserted when a physical link is established
#define PE_DCR__PCIE_DL_STATUS__DL_LINKUP_width        1
#define PE_DCR__PCIE_DL_STATUS__DL_LINKUP_position     32
#define PE_DCR__PCIE_DL_STATUS__DL_LINKUP_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__PCIE_DL_STATUS__DL_LINKUP_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__PCIE_DL_STATUS__DL_LINKUP_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       dl_tcrx_scrambleoff
//   Description: Indicates that the link has been established with scrambling disabled
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_SCRAMBLEOFF_width        1
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_SCRAMBLEOFF_position     33
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_SCRAMBLEOFF_get(x)       _BGQ_GET(1,33,x)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_SCRAMBLEOFF_set(v)       _BGQ_SET(1,33,v)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_SCRAMBLEOFF_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       dl_lb_active
//   Description: This signal is asserted (per lane) when the lane is in loopback. Bit 0 corresponds
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ACTIVE_width        8
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ACTIVE_position     41
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ACTIVE_get(x)       _BGQ_GET(8,41,x)
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ACTIVE_set(v)       _BGQ_SET(8,41,v)
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ACTIVE_insert(x,v)  _BGQ_INSERT(8,41,x,v)

//   Field:       dl_inbandpresence
//   Description: Indicates that a remote component was detected
#define PE_DCR__PCIE_DL_STATUS__DL_INBANDPRESENCE_width        1
#define PE_DCR__PCIE_DL_STATUS__DL_INBANDPRESENCE_position     42
#define PE_DCR__PCIE_DL_STATUS__DL_INBANDPRESENCE_get(x)       _BGQ_GET(1,42,x)
#define PE_DCR__PCIE_DL_STATUS__DL_INBANDPRESENCE_set(v)       _BGQ_SET(1,42,v)
#define PE_DCR__PCIE_DL_STATUS__DL_INBANDPRESENCE_insert(x,v)  _BGQ_INSERT(1,42,x,v)

//   Field:       dl_lb_error
//   Description: This signal is asserted  (per lane) for one cycle each time an error is detected in the received loopback data stream. Bit 48 corresponds to lane 0 and bit 55 corresponds to lane 8
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ERROR_width        8
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ERROR_position     55
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ERROR_get(x)       _BGQ_GET(8,55,x)
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ERROR_set(v)       _BGQ_SET(8,55,v)
#define PE_DCR__PCIE_DL_STATUS__DL_LB_ERROR_insert(x,v)  _BGQ_INSERT(8,55,x,v)

//   Field:       dl_tcrx_loopback
//   Description: Link is in loopback mode
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_LOOPBACK_width        1
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_LOOPBACK_position     56
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_LOOPBACK_get(x)       _BGQ_GET(1,56,x)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_LOOPBACK_set(v)       _BGQ_SET(1,56,v)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_LOOPBACK_insert(x,v)  _BGQ_INSERT(1,56,x,v)

//   Field:       dl_tcrx_disable
//   Description: Link is disabled
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_DISABLE_width        1
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_DISABLE_position     57
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_DISABLE_get(x)       _BGQ_GET(1,57,x)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_DISABLE_set(v)       _BGQ_SET(1,57,v)
#define PE_DCR__PCIE_DL_STATUS__DL_TCRX_DISABLE_insert(x,v)  _BGQ_INSERT(1,57,x,v)

  
// Register:
//   Name:        pcie_cfg00
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Configuration Space Device/Vendor Id Register
#define PE_DCR__PCIE_CFG00_offset  (0x0004) // Offset of register in instance 
#define PE_DCR__PCIE_CFG00_reset  UNSIGNED64(0x0000000000000000)

//   Field:       vendor_id
//   Description: Provides the 2-byte read-only value for the VENDOR ID register located in the PCI 2.3 legacy header(0/1) at PCI Offset 00h. This read-only value is assigned/registered by the PCI Special Interest Group and is a unique value for every PCI supplier (vendor).
#define PE_DCR__PCIE_CFG00__VENDOR_ID_width        16
#define PE_DCR__PCIE_CFG00__VENDOR_ID_position     15
#define PE_DCR__PCIE_CFG00__VENDOR_ID_get(x)       _BGQ_GET(16,15,x)
#define PE_DCR__PCIE_CFG00__VENDOR_ID_set(v)       _BGQ_SET(16,15,v)
#define PE_DCR__PCIE_CFG00__VENDOR_ID_insert(x,v)  _BGQ_INSERT(16,15,x,v)

//   Field:       device_id
//   Description: Provides the 2-byte read-only value for the DEVICE ID register located in the PCI 2.3 legacy header(0/1) at PCI Offset 00h. This read-only value is assigned by the customer
#define PE_DCR__PCIE_CFG00__DEVICE_ID_width        16
#define PE_DCR__PCIE_CFG00__DEVICE_ID_position     31
#define PE_DCR__PCIE_CFG00__DEVICE_ID_get(x)       _BGQ_GET(16,31,x)
#define PE_DCR__PCIE_CFG00__DEVICE_ID_set(v)       _BGQ_SET(16,31,v)
#define PE_DCR__PCIE_CFG00__DEVICE_ID_insert(x,v)  _BGQ_INSERT(16,31,x,v)

  
// Register:
//   Name:        pcie_cfg08
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Configuration Space Revision ID/Class Code Register
#define PE_DCR__PCIE_CFG08_offset  (0x0005) // Offset of register in instance 
#define PE_DCR__PCIE_CFG08_reset  UNSIGNED64(0x0000000000000000)

//   Field:       class_code
//   Description: Provides the 3-byte read-only value for the CLASS CODE register at PCI Offset 08h (PCI Offset = h08, bytes 1-3). This 24-bit read-only value is used to identify the generic function of the PCI Express device in the form of three bytes (base class code, sub class code, and interface code). See Appendix-D of the PCI specification, v3.0, for class code details.
#define PE_DCR__PCIE_CFG08__CLASS_CODE_width        24
#define PE_DCR__PCIE_CFG08__CLASS_CODE_position     55
#define PE_DCR__PCIE_CFG08__CLASS_CODE_get(x)       _BGQ_GET(24,55,x)
#define PE_DCR__PCIE_CFG08__CLASS_CODE_set(v)       _BGQ_SET(24,55,v)
#define PE_DCR__PCIE_CFG08__CLASS_CODE_insert(x,v)  _BGQ_INSERT(24,55,x,v)

//   Field:       revision_id
//   Description: Provides the 1-byte read-only value for the REVISON ID register at PCI Offset:08h Byte:0
#define PE_DCR__PCIE_CFG08__REVISION_ID_width        8
#define PE_DCR__PCIE_CFG08__REVISION_ID_position     63
#define PE_DCR__PCIE_CFG08__REVISION_ID_get(x)       _BGQ_GET(8,63,x)
#define PE_DCR__PCIE_CFG08__REVISION_ID_set(v)       _BGQ_SET(8,63,v)
#define PE_DCR__PCIE_CFG08__REVISION_ID_insert(x,v)  _BGQ_INSERT(8,63,x,v)

  
// Register:
//   Name:        pcie_ec00
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 00 Register
#define PE_DCR__PCIE_EC00_offset  (0x0006) // Offset of register in instance 
#define PE_DCR__PCIE_EC00_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ec00_slot
//   Description: This input pin controls the Slot implemented bit in the PCI Express capabilities register[8] and the implementation of the Slot Implemented bit (EC00[24]) and the optional slot support registers (EC14 and EC18) for supporting slots on PCI Express root ports and downstream switch port applications.    - If set, the read-only Slot Implemented bit (EC00[24]) is defined by input pin SYS_EC00_SLOT.   - If reset, the read-only Slot Implemented bit (EC00[24]) is set to read-only zero
#define PE_DCR__PCIE_EC00__EC00_SLOT_width        1
#define PE_DCR__PCIE_EC00__EC00_SLOT_position     0
#define PE_DCR__PCIE_EC00__EC00_SLOT_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_EC00__EC00_SLOT_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_EC00__EC00_SLOT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

  
// Register:
//   Name:        pcie_ec04
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       4000000000000000
//   Description: PCIE Extended Configuration 04 Register
#define PE_DCR__PCIE_EC04_offset  (0x0007) // Offset of register in instance 
#define PE_DCR__PCIE_EC04_reset  UNSIGNED64(0x4000000000000000)

//   Field:       max_payload
//   Description: This input indicates the max payload parameter supported by the device.
#define PE_DCR__PCIE_EC04__MAX_PAYLOAD_width        3
#define PE_DCR__PCIE_EC04__MAX_PAYLOAD_position     2
#define PE_DCR__PCIE_EC04__MAX_PAYLOAD_get(x)       _BGQ_GET(3,2,x)
#define PE_DCR__PCIE_EC04__MAX_PAYLOAD_set(v)       _BGQ_SET(3,2,v)
#define PE_DCR__PCIE_EC04__MAX_PAYLOAD_insert(x,v)  _BGQ_INSERT(3,2,x,v)

  
// Register:
//   Name:        pcie_ec08
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 08 Register
#define PE_DCR__PCIE_EC08_offset  (0x0008) // Offset of register in instance 
#define PE_DCR__PCIE_EC08_reset  UNSIGNED64(0x0000000000000000)

//   Field:       auxpower_avail
//   Description: Aux power capability available (1'b1). Used for implementing the specific behavior of the PM and EC bits related to VAUX support (EC08[10], PM04[8], PM04[15]). All PCI Express devices that do NOT require VAUX support should wire this pin to 1'b0
#define PE_DCR__PCIE_EC08__AUXPOWER_AVAIL_width        1
#define PE_DCR__PCIE_EC08__AUXPOWER_AVAIL_position     3
#define PE_DCR__PCIE_EC08__AUXPOWER_AVAIL_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_EC08__AUXPOWER_AVAIL_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_EC08__AUXPOWER_AVAIL_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       auxpower_detected
//   Description: Aux power detected to the PCI Express capabilities device status register. All PCI Express devices that do NOT require VAUX support should wire this pin to 1'b0
#define PE_DCR__PCIE_EC08__AUXPOWER_DETECTED_width        1
#define PE_DCR__PCIE_EC08__AUXPOWER_DETECTED_position     4
#define PE_DCR__PCIE_EC08__AUXPOWER_DETECTED_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_EC08__AUXPOWER_DETECTED_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_EC08__AUXPOWER_DETECTED_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       cmpl_timeout_advisory
//   Description: When set Completion Timeout Errors are treated as advisory non-fatal error
#define PE_DCR__PCIE_EC08__CMPL_TIMEOUT_ADVISORY_width        1
#define PE_DCR__PCIE_EC08__CMPL_TIMEOUT_ADVISORY_position     5
#define PE_DCR__PCIE_EC08__CMPL_TIMEOUT_ADVISORY_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_EC08__CMPL_TIMEOUT_ADVISORY_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_EC08__CMPL_TIMEOUT_ADVISORY_insert(x,v)  _BGQ_INSERT(1,5,x,v)

  
// Register:
//   Name:        pcie_ec0c
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       4020000800000000
//   Description: PCIE Extended Configuration 0C Register
#define PE_DCR__PCIE_EC0C_offset  (0x0009) // Offset of register in instance 
#define PE_DCR__PCIE_EC0C_reset  UNSIGNED64(0x4020000800000000)

//   Field:       pm_support
//   Description: Provides the 2-bit read-only value for the active state power management (ASPM) support bits in EC link capabilities register (EC0C[11:10]).  - The PCI Express Specification 2.0 (section 7.8) defines the following encoding: 2'b00 = Reserved 2'b01 = L0s entry supported 2'b10 = Reserved 2'b11 = L0s and L1 entry supported
#define PE_DCR__PCIE_EC0C__PM_SUPPORT_width        2
#define PE_DCR__PCIE_EC0C__PM_SUPPORT_position     1
#define PE_DCR__PCIE_EC0C__PM_SUPPORT_get(x)       _BGQ_GET(2,1,x)
#define PE_DCR__PCIE_EC0C__PM_SUPPORT_set(v)       _BGQ_SET(2,1,v)
#define PE_DCR__PCIE_EC0C__PM_SUPPORT_insert(x,v)  _BGQ_INSERT(2,1,x,v)

//   Field:       l0sexit_latency
//   Description: Provides the 3-bit read-only value for the link L0s exit latency bits in EC link capabilities register (EC0C[14:12]).  - The PCI Express Specification 2.0 (section 7.8) defines the following encoding: 3'b000 = Less than 64ns 3'b001 = 64?128 ns 3'b010 = 128?256 ns 3'b011 = 256?512 ns 3'b100 = 512 ns?1 ms 3'b101 =   1?2 ms 3'b110 =   2?4 ms 3'b111 = More than 4 ms
#define PE_DCR__PCIE_EC0C__L0SEXIT_LATENCY_width        3
#define PE_DCR__PCIE_EC0C__L0SEXIT_LATENCY_position     4
#define PE_DCR__PCIE_EC0C__L0SEXIT_LATENCY_get(x)       _BGQ_GET(3,4,x)
#define PE_DCR__PCIE_EC0C__L0SEXIT_LATENCY_set(v)       _BGQ_SET(3,4,v)
#define PE_DCR__PCIE_EC0C__L0SEXIT_LATENCY_insert(x,v)  _BGQ_INSERT(3,4,x,v)

//   Field:       l1exit_latency
//   Description: Provides the 3bit read-only value for the Link L1 Exit Latency bits in EC Link Capabilities Register (EC0C[17:15]). This input pin is required for all PCI Express implementations.  - The PCI Express Specification 2.0 (section 7.8) defines the following encoding: 3'b000 = Less than 1ms 3'b001 = 1?2 ms 3'b010 = 2?4 ms 3'b011 = 4?8 ms 3'b100 = 8?16 ms 3'b101 = 16?32 ms 3'b110 = 32?64 ms 3'b111 = More than 64 ms
#define PE_DCR__PCIE_EC0C__L1EXIT_LATENCY_width        3
#define PE_DCR__PCIE_EC0C__L1EXIT_LATENCY_position     7
#define PE_DCR__PCIE_EC0C__L1EXIT_LATENCY_get(x)       _BGQ_GET(3,7,x)
#define PE_DCR__PCIE_EC0C__L1EXIT_LATENCY_set(v)       _BGQ_SET(3,7,v)
#define PE_DCR__PCIE_EC0C__L1EXIT_LATENCY_insert(x,v)  _BGQ_INSERT(3,7,x,v)

//   Field:       maxlinkwidth
//   Description: Maximum link width to the PCI Express capabilities link capabilities register.
#define PE_DCR__PCIE_EC0C__MAXLINKWIDTH_width        6
#define PE_DCR__PCIE_EC0C__MAXLINKWIDTH_position     13
#define PE_DCR__PCIE_EC0C__MAXLINKWIDTH_get(x)       _BGQ_GET(6,13,x)
#define PE_DCR__PCIE_EC0C__MAXLINKWIDTH_set(v)       _BGQ_SET(6,13,v)
#define PE_DCR__PCIE_EC0C__MAXLINKWIDTH_insert(x,v)  _BGQ_INSERT(6,13,x,v)

//   Field:       portnumber
//   Description: Provides the1-byte read-only value for the port number register located in the EC link capabilities register (EC0C[31:24]). This input pin is optional for all PCI Express implementations.
#define PE_DCR__PCIE_EC0C__PORTNUMBER_width        8
#define PE_DCR__PCIE_EC0C__PORTNUMBER_position     23
#define PE_DCR__PCIE_EC0C__PORTNUMBER_get(x)       _BGQ_GET(8,23,x)
#define PE_DCR__PCIE_EC0C__PORTNUMBER_set(v)       _BGQ_SET(8,23,v)
#define PE_DCR__PCIE_EC0C__PORTNUMBER_insert(x,v)  _BGQ_INSERT(8,23,x,v)

//   Field:       surpr_dwn_cap
//   Description: When set to 1, enables support of detecting and reporting a surprise down error condition for downstream port.  For upstream port and components which doesn't support this option, this bit must be hardwired to 0
#define PE_DCR__PCIE_EC0C__SURPR_DWN_CAP_width        1
#define PE_DCR__PCIE_EC0C__SURPR_DWN_CAP_position     24
#define PE_DCR__PCIE_EC0C__SURPR_DWN_CAP_get(x)       _BGQ_GET(1,24,x)
#define PE_DCR__PCIE_EC0C__SURPR_DWN_CAP_set(v)       _BGQ_SET(1,24,v)
#define PE_DCR__PCIE_EC0C__SURPR_DWN_CAP_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       linkbwnotifcap
//   Description: If set, this input pin Indicates support of Link Bandwidth notification status and interrupt mechanism, otherwise this input must be reset.
#define PE_DCR__PCIE_EC0C__LINKBWNOTIFCAP_width        1
#define PE_DCR__PCIE_EC0C__LINKBWNOTIFCAP_position     25
#define PE_DCR__PCIE_EC0C__LINKBWNOTIFCAP_get(x)       _BGQ_GET(1,25,x)
#define PE_DCR__PCIE_EC0C__LINKBWNOTIFCAP_set(v)       _BGQ_SET(1,25,v)
#define PE_DCR__PCIE_EC0C__LINKBWNOTIFCAP_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       sys_rc_ec0c_supportedlinkspeeds
//   Description: Provides Supported Link Speeds of the associated port, defines the following encoding: 4'b0001 = 2.5Gb/s Link Speed supported 4'b0010 = 2.5Gb/s and 5Gb/s Link Speed supported All other encoding is reserved
#define PE_DCR__PCIE_EC0C__SYS_RC_EC0C_SUPPORTEDLINKSPEEDS_width        4
#define PE_DCR__PCIE_EC0C__SYS_RC_EC0C_SUPPORTEDLINKSPEEDS_position     29
#define PE_DCR__PCIE_EC0C__SYS_RC_EC0C_SUPPORTEDLINKSPEEDS_get(x)       _BGQ_GET(4,29,x)
#define PE_DCR__PCIE_EC0C__SYS_RC_EC0C_SUPPORTEDLINKSPEEDS_set(v)       _BGQ_SET(4,29,v)
#define PE_DCR__PCIE_EC0C__SYS_RC_EC0C_SUPPORTEDLINKSPEEDS_insert(x,v)  _BGQ_INSERT(4,29,x,v)

//   Field:       slotclock
//   Description: Provides the 1-bit read-only value for the slot clock configuration bit in EC link status register bit 12 (EC10[28]).  - The PCI Express Specification 2.0 (section 7.8) defines the following encoding: 1'b0 = Independent clock (non-connector clock)  1'b1 = Common clock (same as connector)
#define PE_DCR__PCIE_EC0C__SLOTCLOCK_width        1
#define PE_DCR__PCIE_EC0C__SLOTCLOCK_position     30
#define PE_DCR__PCIE_EC0C__SLOTCLOCK_get(x)       _BGQ_GET(1,30,x)
#define PE_DCR__PCIE_EC0C__SLOTCLOCK_set(v)       _BGQ_SET(1,30,v)
#define PE_DCR__PCIE_EC0C__SLOTCLOCK_insert(x,v)  _BGQ_INSERT(1,30,x,v)

  
// Register:
//   Name:        pcie_ec14
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 14 Register
#define PE_DCR__PCIE_EC14_offset  (0x000A) // Offset of register in instance 
#define PE_DCR__PCIE_EC14_reset  UNSIGNED64(0x0000000000000000)

//   Field:       atn_btn_present
//   Description: Attention button present to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot 
#define PE_DCR__PCIE_EC14__ATN_BTN_PRESENT_width        1
#define PE_DCR__PCIE_EC14__ATN_BTN_PRESENT_position     0
#define PE_DCR__PCIE_EC14__ATN_BTN_PRESENT_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_EC14__ATN_BTN_PRESENT_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_EC14__ATN_BTN_PRESENT_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       pwr_ctl_present
//   Description: Power controller present to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot 
#define PE_DCR__PCIE_EC14__PWR_CTL_PRESENT_width        1
#define PE_DCR__PCIE_EC14__PWR_CTL_PRESENT_position     1
#define PE_DCR__PCIE_EC14__PWR_CTL_PRESENT_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_EC14__PWR_CTL_PRESENT_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_EC14__PWR_CTL_PRESENT_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       mrl_present
//   Description: Mechanical release latch (MRL) present to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__MRL_PRESENT_width        1
#define PE_DCR__PCIE_EC14__MRL_PRESENT_position     2
#define PE_DCR__PCIE_EC14__MRL_PRESENT_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__PCIE_EC14__MRL_PRESENT_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__PCIE_EC14__MRL_PRESENT_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       atn_ind_present
//   Description: Attention indicator present to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__ATN_IND_PRESENT_width        1
#define PE_DCR__PCIE_EC14__ATN_IND_PRESENT_position     3
#define PE_DCR__PCIE_EC14__ATN_IND_PRESENT_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_EC14__ATN_IND_PRESENT_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_EC14__ATN_IND_PRESENT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       pwr_ind_present
//   Description: Power indicator present to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__PWR_IND_PRESENT_width        1
#define PE_DCR__PCIE_EC14__PWR_IND_PRESENT_position     4
#define PE_DCR__PCIE_EC14__PWR_IND_PRESENT_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_EC14__PWR_IND_PRESENT_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_EC14__PWR_IND_PRESENT_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       hotplug_surprise
//   Description: Hot-plug surprise to the slot capabilities register.  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot
#define PE_DCR__PCIE_EC14__HOTPLUG_SURPRISE_width        1
#define PE_DCR__PCIE_EC14__HOTPLUG_SURPRISE_position     5
#define PE_DCR__PCIE_EC14__HOTPLUG_SURPRISE_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_EC14__HOTPLUG_SURPRISE_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_EC14__HOTPLUG_SURPRISE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       hotplug_capable
//   Description: Hot-plug capable to the slot capabilities register.  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__HOTPLUG_CAPABLE_width        1
#define PE_DCR__PCIE_EC14__HOTPLUG_CAPABLE_position     6
#define PE_DCR__PCIE_EC14__HOTPLUG_CAPABLE_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__PCIE_EC14__HOTPLUG_CAPABLE_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__PCIE_EC14__HOTPLUG_CAPABLE_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       pwr_limit_value
//   Description: Slot power limit value to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__PWR_LIMIT_VALUE_width        8
#define PE_DCR__PCIE_EC14__PWR_LIMIT_VALUE_position     14
#define PE_DCR__PCIE_EC14__PWR_LIMIT_VALUE_get(x)       _BGQ_GET(8,14,x)
#define PE_DCR__PCIE_EC14__PWR_LIMIT_VALUE_set(v)       _BGQ_SET(8,14,v)
#define PE_DCR__PCIE_EC14__PWR_LIMIT_VALUE_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       pwr_limit_scale
//   Description: Slot power limit scale to the slot capabilities register.  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC14__PWR_LIMIT_SCALE_width        2
#define PE_DCR__PCIE_EC14__PWR_LIMIT_SCALE_position     16
#define PE_DCR__PCIE_EC14__PWR_LIMIT_SCALE_get(x)       _BGQ_GET(2,16,x)
#define PE_DCR__PCIE_EC14__PWR_LIMIT_SCALE_set(v)       _BGQ_SET(2,16,v)
#define PE_DCR__PCIE_EC14__PWR_LIMIT_SCALE_insert(x,v)  _BGQ_INSERT(2,16,x,v)

//   Field:       slot_number
//   Description: Physical slot number to the slot capabilities register. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot
#define PE_DCR__PCIE_EC14__SLOT_NUMBER_width        13
#define PE_DCR__PCIE_EC14__SLOT_NUMBER_position     29
#define PE_DCR__PCIE_EC14__SLOT_NUMBER_get(x)       _BGQ_GET(13,29,x)
#define PE_DCR__PCIE_EC14__SLOT_NUMBER_set(v)       _BGQ_SET(13,29,v)
#define PE_DCR__PCIE_EC14__SLOT_NUMBER_insert(x,v)  _BGQ_INSERT(13,29,x,v)

//   Field:       elm_present
//   Description: When set to 1, this input pin indicates that an electromechanical interlock is implemented on the chassis for this slot. 
#define PE_DCR__PCIE_EC14__ELM_PRESENT_width        1
#define PE_DCR__PCIE_EC14__ELM_PRESENT_position     30
#define PE_DCR__PCIE_EC14__ELM_PRESENT_get(x)       _BGQ_GET(1,30,x)
#define PE_DCR__PCIE_EC14__ELM_PRESENT_set(v)       _BGQ_SET(1,30,v)
#define PE_DCR__PCIE_EC14__ELM_PRESENT_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       no_cmd_cmplt_support
//   Description: When set to 1, this input pin indicates that this slot doesn't generate software indication when an issued commend is completed by the Hot-Plug controller It is only permitted to set this input pin to 1 if the Hot-Plug capable port is able to accept writes to all fields of the Slot Control register without delay between successive writes.
#define PE_DCR__PCIE_EC14__NO_CMD_CMPLT_SUPPORT_width        1
#define PE_DCR__PCIE_EC14__NO_CMD_CMPLT_SUPPORT_position     31
#define PE_DCR__PCIE_EC14__NO_CMD_CMPLT_SUPPORT_get(x)       _BGQ_GET(1,31,x)
#define PE_DCR__PCIE_EC14__NO_CMD_CMPLT_SUPPORT_set(v)       _BGQ_SET(1,31,v)
#define PE_DCR__PCIE_EC14__NO_CMD_CMPLT_SUPPORT_insert(x,v)  _BGQ_INSERT(1,31,x,v)

  
// Register:
//   Name:        pcie_ec18
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 18 Control Register
#define PE_DCR__PCIE_EC18_offset  (0x000B) // Offset of register in instance 
#define PE_DCR__PCIE_EC18_reset  UNSIGNED64(0x0000000000000000)

//   Field:       atn_btn_pressed
//   Description: Attention button pressed to the slot status register[0]. This signal is active high for one PCLK250 cycle. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC18__ATN_BTN_PRESSED_width        1
#define PE_DCR__PCIE_EC18__ATN_BTN_PRESSED_position     0
#define PE_DCR__PCIE_EC18__ATN_BTN_PRESSED_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_EC18__ATN_BTN_PRESSED_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_EC18__ATN_BTN_PRESSED_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       pwr_fault_detected
//   Description: Power fault detected (from power controller) to the slot status register[1]. This signal is active high for one PCLK250 cycle. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot
#define PE_DCR__PCIE_EC18__PWR_FAULT_DETECTED_width        1
#define PE_DCR__PCIE_EC18__PWR_FAULT_DETECTED_position     1
#define PE_DCR__PCIE_EC18__PWR_FAULT_DETECTED_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_EC18__PWR_FAULT_DETECTED_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_EC18__PWR_FAULT_DETECTED_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       mrl_sensor_changed
//   Description: Mechanical release latch (MRL) sensor changed to the slot status register[2]. This signal is active high for one PCLK250 cycle. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot
#define PE_DCR__PCIE_EC18__MRL_SENSOR_CHANGED_width        1
#define PE_DCR__PCIE_EC18__MRL_SENSOR_CHANGED_position     2
#define PE_DCR__PCIE_EC18__MRL_SENSOR_CHANGED_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__PCIE_EC18__MRL_SENSOR_CHANGED_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__PCIE_EC18__MRL_SENSOR_CHANGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       slot_detect_changed
//   Description: Slot presence detect changed to the slot status register[3]. This signal is active high for one PCLK250 cycle. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18__SLOT_DETECT_CHANGED_width        1
#define PE_DCR__PCIE_EC18__SLOT_DETECT_CHANGED_position     3
#define PE_DCR__PCIE_EC18__SLOT_DETECT_CHANGED_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_EC18__SLOT_DETECT_CHANGED_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_EC18__SLOT_DETECT_CHANGED_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       pwr_ctl_cmd_cmplt
//   Description: Command completed (from the hot-plug controller) to the slot status register[4]. This signal is active high for one PCLK250 cycle. This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18__PWR_CTL_CMD_CMPLT_width        1
#define PE_DCR__PCIE_EC18__PWR_CTL_CMD_CMPLT_position     4
#define PE_DCR__PCIE_EC18__PWR_CTL_CMD_CMPLT_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_EC18__PWR_CTL_CMD_CMPLT_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_EC18__PWR_CTL_CMD_CMPLT_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       mrl_sensor_state
//   Description: Mechanical release latch (MRL) sensor state to the slot status register[5].  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18__MRL_SENSOR_STATE_width        1
#define PE_DCR__PCIE_EC18__MRL_SENSOR_STATE_position     5
#define PE_DCR__PCIE_EC18__MRL_SENSOR_STATE_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_EC18__MRL_SENSOR_STATE_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_EC18__MRL_SENSOR_STATE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       slot_detect_state
//   Description: Slot presence detect state to the slot status register[6].  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18__SLOT_DETECT_STATE_width        1
#define PE_DCR__PCIE_EC18__SLOT_DETECT_STATE_position     6
#define PE_DCR__PCIE_EC18__SLOT_DETECT_STATE_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__PCIE_EC18__SLOT_DETECT_STATE_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__PCIE_EC18__SLOT_DETECT_STATE_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       elm_status
//   Description: Electromechanical Interlock Status to the slot status register[7].  This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18__ELM_STATUS_width        1
#define PE_DCR__PCIE_EC18__ELM_STATUS_position     7
#define PE_DCR__PCIE_EC18__ELM_STATUS_get(x)       _BGQ_GET(1,7,x)
#define PE_DCR__PCIE_EC18__ELM_STATUS_set(v)       _BGQ_SET(1,7,v)
#define PE_DCR__PCIE_EC18__ELM_STATUS_insert(x,v)  _BGQ_INSERT(1,7,x,v)

  
// Register:
//   Name:        pcie_ec18_stat
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 18 Status Register
#define PE_DCR__PCIE_EC18_STAT_offset  (0x000C) // Offset of register in instance 
#define PE_DCR__PCIE_EC18_STAT_reset  UNSIGNED64(0x0000000000000000)

//   Field:       slot_wake
//   Description: activate wake mechanism. This signal is generated from combinatorial 'and/or' logic using signals from the slot control and status registers to create one wake signal for slot events (see the PCI Express Specification section 6.7.7 for more details). - State after DL_HOTRESET = 0 - State after DL_PGRESET = 0 This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. 
#define PE_DCR__PCIE_EC18_STAT__SLOT_WAKE_width        1
#define PE_DCR__PCIE_EC18_STAT__SLOT_WAKE_position     8
#define PE_DCR__PCIE_EC18_STAT__SLOT_WAKE_get(x)       _BGQ_GET(1,8,x)
#define PE_DCR__PCIE_EC18_STAT__SLOT_WAKE_set(v)       _BGQ_SET(1,8,v)
#define PE_DCR__PCIE_EC18_STAT__SLOT_WAKE_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       slot_intr
//   Description: Slot activate interrupt mechanism. This signal is generated from combinatorial 'and/or' logic using signals from the slot control and status registers to create one interrupt signal for slot events. This signal is used, with the interrupt disable and MSI enable signals to activate the system specific slot Interrupt (INTx or MSI). (See the PCI Express Specification section 6.7.7 for more details). - State after DL_HOTRESET = 0 - State after DL_PGRESET = 0 This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot . 
#define PE_DCR__PCIE_EC18_STAT__SLOT_INTR_width        1
#define PE_DCR__PCIE_EC18_STAT__SLOT_INTR_position     9
#define PE_DCR__PCIE_EC18_STAT__SLOT_INTR_get(x)       _BGQ_GET(1,9,x)
#define PE_DCR__PCIE_EC18_STAT__SLOT_INTR_set(v)       _BGQ_SET(1,9,v)
#define PE_DCR__PCIE_EC18_STAT__SLOT_INTR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       atn_ind_control
//   Description: Attention indicator control from the slot control register[7:6]. - State after DL_HOTRESET = 0 - State after DL_PGRESET = 0 This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot.
#define PE_DCR__PCIE_EC18_STAT__ATN_IND_CONTROL_width        2
#define PE_DCR__PCIE_EC18_STAT__ATN_IND_CONTROL_position     11
#define PE_DCR__PCIE_EC18_STAT__ATN_IND_CONTROL_get(x)       _BGQ_GET(2,11,x)
#define PE_DCR__PCIE_EC18_STAT__ATN_IND_CONTROL_set(v)       _BGQ_SET(2,11,v)
#define PE_DCR__PCIE_EC18_STAT__ATN_IND_CONTROL_insert(x,v)  _BGQ_INSERT(2,11,x,v)

//   Field:       pwr_ind_control
//   Description: Power indicator control from the slot control register[9:8]. - State after DL_HOTRESET = 0 - State after DL_PGRESET = 0 This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot. Power controller control from the slot control register[10]. - State after DL_HOTRESET = 0 - State after DL_PGRESET = 0 This pin is valid only if the customer sets the SYS_EC00_SLOT input, indicating that this device is associated with a PCI Express slot CFG_RC_EC18_PWR_CTL_CONTROL 
#define PE_DCR__PCIE_EC18_STAT__PWR_IND_CONTROL_width        2
#define PE_DCR__PCIE_EC18_STAT__PWR_IND_CONTROL_position     13
#define PE_DCR__PCIE_EC18_STAT__PWR_IND_CONTROL_get(x)       _BGQ_GET(2,13,x)
#define PE_DCR__PCIE_EC18_STAT__PWR_IND_CONTROL_set(v)       _BGQ_SET(2,13,v)
#define PE_DCR__PCIE_EC18_STAT__PWR_IND_CONTROL_insert(x,v)  _BGQ_INSERT(2,13,x,v)

//   Field:       pcie_in_p
//   Description: This fields reflects the value of the PCI_IN_P(0 to 3) pins on BG/Q ASIC
#define PE_DCR__PCIE_EC18_STAT__PCIE_IN_P_width        4
#define PE_DCR__PCIE_EC18_STAT__PCIE_IN_P_position     19
#define PE_DCR__PCIE_EC18_STAT__PCIE_IN_P_get(x)       _BGQ_GET(4,19,x)
#define PE_DCR__PCIE_EC18_STAT__PCIE_IN_P_set(v)       _BGQ_SET(4,19,v)
#define PE_DCR__PCIE_EC18_STAT__PCIE_IN_P_insert(x,v)  _BGQ_INSERT(4,19,x,v)

  
// Register:
//   Name:        pcie_ec24
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Extended Configuration 24 Register
#define PE_DCR__PCIE_EC24_offset  (0x000D) // Offset of register in instance 
#define PE_DCR__PCIE_EC24_reset  UNSIGNED64(0x0000000000000000)

//   Field:       comp_to_ranges_support
//   Description: When set, this input pin indicates device support for the optional completion timeout programmability mechanism which allows system software to modify the completion timeout value. This input pin is applicable to Root ports, End-Ports that issue requests on their own behalf and PCI-Express to PCI/PCI-X bridges that make ownership of requests issued on PCI-Express. For all other devices, this input is reserved and must be hardwired to 4'b0000. 4 time value ranges are supported: Range A = 50uS to 10mS Range B= 10mS to 250mS Range C= 250mS to 4S Range D= 4S to 64S Bits are set according to the table below to show timeout value ranges supported: 4'b0000 = Completion Timeout programing not supported, the device must implement a timeout value in range 50uS to 50mS 4'b0001 = Range A 4'b0010 = Range B 4'b0011 = Range A and Range B 4'b0110 = Range B and Range C 4'b0111 = Range A and Range B and Range C 4'b1110 = Range B and Range C and Range D 4'b1111 = All 4 Ranges All other values are reserved 
#define PE_DCR__PCIE_EC24__COMP_TO_RANGES_SUPPORT_width        4
#define PE_DCR__PCIE_EC24__COMP_TO_RANGES_SUPPORT_position     3
#define PE_DCR__PCIE_EC24__COMP_TO_RANGES_SUPPORT_get(x)       _BGQ_GET(4,3,x)
#define PE_DCR__PCIE_EC24__COMP_TO_RANGES_SUPPORT_set(v)       _BGQ_SET(4,3,v)
#define PE_DCR__PCIE_EC24__COMP_TO_RANGES_SUPPORT_insert(x,v)  _BGQ_INSERT(4,3,x,v)

//   Field:       comp_to_dis_support
//   Description: When set, this input pin indicates the support for completion timeout disable mechanism which is required for endpoint that issue requests on their own behalf and PCI Express to PCI/PCI-X bridges that take ownership of requests issued on PCI-Express. This mechanism is optional for Root Ports,  For all other devices this input pin must be hardwired to 0
#define PE_DCR__PCIE_EC24__COMP_TO_DIS_SUPPORT_width        1
#define PE_DCR__PCIE_EC24__COMP_TO_DIS_SUPPORT_position     4
#define PE_DCR__PCIE_EC24__COMP_TO_DIS_SUPPORT_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_EC24__COMP_TO_DIS_SUPPORT_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_EC24__COMP_TO_DIS_SUPPORT_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       ari_fwd_support
//   Description: Should be set to 0
#define PE_DCR__PCIE_EC24__ARI_FWD_SUPPORT_width        1
#define PE_DCR__PCIE_EC24__ARI_FWD_SUPPORT_position     5
#define PE_DCR__PCIE_EC24__ARI_FWD_SUPPORT_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_EC24__ARI_FWD_SUPPORT_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_EC24__ARI_FWD_SUPPORT_insert(x,v)  _BGQ_INSERT(1,5,x,v)

  
// Register:
//   Name:        pcie_aer_config
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Advanced Error Reporting Configuration Register
#define PE_DCR__PCIE_AER_CONFIG_offset  (0x000E) // Offset of register in instance 
#define PE_DCR__PCIE_AER_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       error_advisory
//   Description: When set ECRC Error are treated as advisory non-fatal error.
#define PE_DCR__PCIE_AER_CONFIG__ERROR_ADVISORY_width        1
#define PE_DCR__PCIE_AER_CONFIG__ERROR_ADVISORY_position     0
#define PE_DCR__PCIE_AER_CONFIG__ERROR_ADVISORY_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_AER_CONFIG__ERROR_ADVISORY_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_AER_CONFIG__ERROR_ADVISORY_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       rec_poisoned_advisory
//   Description: When set Received Poisoned Error are treated as advisory non-fatal error.
#define PE_DCR__PCIE_AER_CONFIG__REC_POISONED_ADVISORY_width        1
#define PE_DCR__PCIE_AER_CONFIG__REC_POISONED_ADVISORY_position     1
#define PE_DCR__PCIE_AER_CONFIG__REC_POISONED_ADVISORY_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_AER_CONFIG__REC_POISONED_ADVISORY_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_AER_CONFIG__REC_POISONED_ADVISORY_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       ecrc_gen_avail
//   Description: The ECRC available signal is used to set the ECRC capability bit in the AER capabilities and control register AER18[5].
#define PE_DCR__PCIE_AER_CONFIG__ECRC_GEN_AVAIL_width        1
#define PE_DCR__PCIE_AER_CONFIG__ECRC_GEN_AVAIL_position     2
#define PE_DCR__PCIE_AER_CONFIG__ECRC_GEN_AVAIL_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__PCIE_AER_CONFIG__ECRC_GEN_AVAIL_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__PCIE_AER_CONFIG__ECRC_GEN_AVAIL_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       ecrc_check_avail
//   Description: The ECRC available signal is used to set the ECRC capability bit in the AER capabilities and control register AER18[7]. This signal is also used to gate the AER ECRC bits in the errors status, mask and severity registers to ensure read-only zero behavior when NO ECRC checking is present.
#define PE_DCR__PCIE_AER_CONFIG__ECRC_CHECK_AVAIL_width        1
#define PE_DCR__PCIE_AER_CONFIG__ECRC_CHECK_AVAIL_position     3
#define PE_DCR__PCIE_AER_CONFIG__ECRC_CHECK_AVAIL_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_AER_CONFIG__ECRC_CHECK_AVAIL_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_AER_CONFIG__ECRC_CHECK_AVAIL_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       msi_number
//   Description: These advanced error reporting MSI number signals provide the read-only value for the AER root error status register AER30[31:27].
#define PE_DCR__PCIE_AER_CONFIG__MSI_NUMBER_width        5
#define PE_DCR__PCIE_AER_CONFIG__MSI_NUMBER_position     8
#define PE_DCR__PCIE_AER_CONFIG__MSI_NUMBER_get(x)       _BGQ_GET(5,8,x)
#define PE_DCR__PCIE_AER_CONFIG__MSI_NUMBER_set(v)       _BGQ_SET(5,8,v)
#define PE_DCR__PCIE_AER_CONFIG__MSI_NUMBER_insert(x,v)  _BGQ_INSERT(5,8,x,v)

  
// Register:
//   Name:        pcie_ifc_cfg
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0100FF0000000000
//   Description: PCIe Interface Configuration
#define PE_DCR__PCIE_IFC_CFG_offset  (0x000F) // Offset of register in instance 
#define PE_DCR__PCIE_IFC_CFG_reset  UNSIGNED64(0x0100FF0000000000)

//   Field:       out_rd_relaxed
//   Description: Enable to cause all outbound read requests to be relaxed (i.e. set gbif_out_rd_relaxed).
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_RELAXED_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_RELAXED_position     0
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_RELAXED_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_RELAXED_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_RELAXED_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       out_rd_ecrc_en
//   Description: Enable to inject CRC errors into all outbound read TLPs by inverting the calculated ECRC value.
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_EN_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_EN_position     1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_EN_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_EN_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_EN_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       out_rd_ecrc_once
//   Description: Enable to inject CRC errors into all outbound read TLPs caused by the next outbound request. Disable to reset.
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_ONCE_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_ONCE_position     2
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_ONCE_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_ONCE_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_ECRC_ONCE_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       out_wr_relaxed
//   Description: Enable to cause all outbound write requests to be relaxed (i.e. set gbif_out_wr_relaxed).
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_RELAXED_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_RELAXED_position     3
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_RELAXED_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_RELAXED_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_RELAXED_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       out_wr_ecrc_en
//   Description: Enable to inject CRC errors into all outbound write TLPs by inverting the calculated ECRC value.
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_EN_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_EN_position     4
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_EN_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_EN_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_EN_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       out_wr_ecrc_once
//   Description: Enable to inject CRC errors into all outbound write TLPs caused by the next outbound request. Disable to reset.
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_ONCE_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_ONCE_position     5
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_ONCE_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_ONCE_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_ECRC_ONCE_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       in_rd_data_ecc_disable
//   Description: Enable to pass inbound read data (from L2) through the ECC checker without correcting it.
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DATA_ECC_DISABLE_width        1
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DATA_ECC_DISABLE_position     6
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DATA_ECC_DISABLE_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DATA_ECC_DISABLE_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DATA_ECC_DISABLE_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       in_relaxed
//   Description: Set the relaxed ordering attribute for the definable inbound memory range. 1 = relaxed, 0 = strict.
#define PE_DCR__PCIE_IFC_CFG__IN_RELAXED_width        1
#define PE_DCR__PCIE_IFC_CFG__IN_RELAXED_position     7
#define PE_DCR__PCIE_IFC_CFG__IN_RELAXED_get(x)       _BGQ_GET(1,7,x)
#define PE_DCR__PCIE_IFC_CFG__IN_RELAXED_set(v)       _BGQ_SET(1,7,v)
#define PE_DCR__PCIE_IFC_CFG__IN_RELAXED_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       in_wr_page_offset
//   Description: Number of 1/4 gigabyte pages to shift the inbound write address up by.
#define PE_DCR__PCIE_IFC_CFG__IN_WR_PAGE_OFFSET_width        8
#define PE_DCR__PCIE_IFC_CFG__IN_WR_PAGE_OFFSET_position     15
#define PE_DCR__PCIE_IFC_CFG__IN_WR_PAGE_OFFSET_get(x)       _BGQ_GET(8,15,x)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_PAGE_OFFSET_set(v)       _BGQ_SET(8,15,v)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_PAGE_OFFSET_insert(x,v)  _BGQ_INSERT(8,15,x,v)

//   Field:       in_wr_window_top
//   Description: Top of allowed inbound write address page range (after offset is applied), specified as a 1/4 gigabyte page number.
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_TOP_width        8
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_TOP_position     23
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_TOP_get(x)       _BGQ_GET(8,23,x)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_TOP_set(v)       _BGQ_SET(8,23,v)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_TOP_insert(x,v)  _BGQ_INSERT(8,23,x,v)

//   Field:       in_wr_window_bottom
//   Description: Bottom of allowed inbound write address page range (after offset is applied), specified as a 1/4 gigabyte page number.
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_BOTTOM_width        8
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_BOTTOM_position     31
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_BOTTOM_get(x)       _BGQ_GET(8,31,x)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_BOTTOM_set(v)       _BGQ_SET(8,31,v)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_WINDOW_BOTTOM_insert(x,v)  _BGQ_INSERT(8,31,x,v)

//   Field:       out_rd_nosnoop
//   Description: Enable to cause all outbound read requests to have the nosnoop bit set (i.e. set gbif_out_rd_nosnoop).
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_NOSNOOP_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_NOSNOOP_position     32
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_NOSNOOP_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_NOSNOOP_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_NOSNOOP_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       out_wr_nosnoop
//   Description: Enable to cause all outbound write requests to have the nosnoop bit set (i.e. set gbif_out_wr_nosnoop).
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_NOSNOOP_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_NOSNOOP_position     33
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_NOSNOOP_get(x)       _BGQ_GET(1,33,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_NOSNOOP_set(v)       _BGQ_SET(1,33,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_NOSNOOP_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       in_wr_strict_safe
//   Description: Enable to cause all strictly-ordered inbound write requests to completely drain the incoming write path before starting.
#define PE_DCR__PCIE_IFC_CFG__IN_WR_STRICT_SAFE_width        1
#define PE_DCR__PCIE_IFC_CFG__IN_WR_STRICT_SAFE_position     34
#define PE_DCR__PCIE_IFC_CFG__IN_WR_STRICT_SAFE_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_STRICT_SAFE_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__PCIE_IFC_CFG__IN_WR_STRICT_SAFE_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       in_rd_strict_safe
//   Description: Enable to allow only one strictly-ordered inbound read request to be processed at a time.
#define PE_DCR__PCIE_IFC_CFG__IN_RD_STRICT_SAFE_width        1
#define PE_DCR__PCIE_IFC_CFG__IN_RD_STRICT_SAFE_position     35
#define PE_DCR__PCIE_IFC_CFG__IN_RD_STRICT_SAFE_get(x)       _BGQ_GET(1,35,x)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_STRICT_SAFE_set(v)       _BGQ_SET(1,35,v)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_STRICT_SAFE_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       out_rd_dd1_revert
//   Description: Enable to revert outbound read logic back to DD1 behavior.
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_DD1_REVERT_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_DD1_REVERT_position     36
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_DD1_REVERT_get(x)       _BGQ_GET(1,36,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_DD1_REVERT_set(v)       _BGQ_SET(1,36,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_RD_DD1_REVERT_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       out_wr_dd1_revert
//   Description: Enable to revert outbound write logic back to DD1 behavior.
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_DD1_REVERT_width        1
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_DD1_REVERT_position     37
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_DD1_REVERT_get(x)       _BGQ_GET(1,37,x)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_DD1_REVERT_set(v)       _BGQ_SET(1,37,v)
#define PE_DCR__PCIE_IFC_CFG__OUT_WR_DD1_REVERT_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       in_rd_dd1_revert
//   Description: Enable to revert inbound read logic back to DD1 behavior.
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DD1_REVERT_width        1
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DD1_REVERT_position     38
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DD1_REVERT_get(x)       _BGQ_GET(1,38,x)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DD1_REVERT_set(v)       _BGQ_SET(1,38,v)
#define PE_DCR__PCIE_IFC_CFG__IN_RD_DD1_REVERT_insert(x,v)  _BGQ_INSERT(1,38,x,v)

  
// Register:
//   Name:        pcie_ei_control
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       0000000000000000
//   Description: PCIE Error Injection Control Register
#define PE_DCR__PCIE_EI_CONTROL_offset  (0x0011) // Offset of register in instance 
#define PE_DCR__PCIE_EI_CONTROL_reset  UNSIGNED64(0x0000000000000000)

//   Field:       cmd
//   Description: Error injection command. When set, an error injection command will be sent to PCIE. If the continous_error bit is set, errors will be constantly injected. If not set, a single error will be injected. 
#define PE_DCR__PCIE_EI_CONTROL__CMD_width        1
#define PE_DCR__PCIE_EI_CONTROL__CMD_position     0
#define PE_DCR__PCIE_EI_CONTROL__CMD_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_EI_CONTROL__CMD_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_EI_CONTROL__CMD_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       type
//   Description: Error injection command type - indicates type of error to be injected by the PCIe stack. Must be valid with the AL_EI_CMD assertion and remain stable until and including the cycle of error injection completions flagged by PCIE_EI_DONE signal. Refer to Table 7 on page 42 for the valid error type attribute values. All the values not specified in the table are reserved.
#define PE_DCR__PCIE_EI_CONTROL__TYPE_width        6
#define PE_DCR__PCIE_EI_CONTROL__TYPE_position     6
#define PE_DCR__PCIE_EI_CONTROL__TYPE_get(x)       _BGQ_GET(6,6,x)
#define PE_DCR__PCIE_EI_CONTROL__TYPE_set(v)       _BGQ_SET(6,6,v)
#define PE_DCR__PCIE_EI_CONTROL__TYPE_insert(x,v)  _BGQ_INSERT(6,6,x,v)

//   Field:       blockacknaktransmit
//   Description: Continuous error injection command - when asserted, forces TLDLP core to drop all Ack/Nak DLLPs pending transmission
#define PE_DCR__PCIE_EI_CONTROL__BLOCKACKNAKTRANSMIT_width        1
#define PE_DCR__PCIE_EI_CONTROL__BLOCKACKNAKTRANSMIT_position     7
#define PE_DCR__PCIE_EI_CONTROL__BLOCKACKNAKTRANSMIT_get(x)       _BGQ_GET(1,7,x)
#define PE_DCR__PCIE_EI_CONTROL__BLOCKACKNAKTRANSMIT_set(v)       _BGQ_SET(1,7,v)
#define PE_DCR__PCIE_EI_CONTROL__BLOCKACKNAKTRANSMIT_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       ei_blockacknakreceive
//   Description: Continuous error injection command - when asserted, forces TLDLP core to ignore all Ack/Nak DLLPs received from the link. 
#define PE_DCR__PCIE_EI_CONTROL__EI_BLOCKACKNAKRECEIVE_width        1
#define PE_DCR__PCIE_EI_CONTROL__EI_BLOCKACKNAKRECEIVE_position     8
#define PE_DCR__PCIE_EI_CONTROL__EI_BLOCKACKNAKRECEIVE_get(x)       _BGQ_GET(1,8,x)
#define PE_DCR__PCIE_EI_CONTROL__EI_BLOCKACKNAKRECEIVE_set(v)       _BGQ_SET(1,8,v)
#define PE_DCR__PCIE_EI_CONTROL__EI_BLOCKACKNAKRECEIVE_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       continous_error
//   Description: When asserted, error injection command indicated by type field will be constantly injected. The pcie_ei_status.ei_done bit will be set when the first error is injected.  
#define PE_DCR__PCIE_EI_CONTROL__CONTINOUS_ERROR_width        1
#define PE_DCR__PCIE_EI_CONTROL__CONTINOUS_ERROR_position     9
#define PE_DCR__PCIE_EI_CONTROL__CONTINOUS_ERROR_get(x)       _BGQ_GET(1,9,x)
#define PE_DCR__PCIE_EI_CONTROL__CONTINOUS_ERROR_set(v)       _BGQ_SET(1,9,v)
#define PE_DCR__PCIE_EI_CONTROL__CONTINOUS_ERROR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

  
// Register:
//   Name:        pcie_ei_status
//   Type:        Read Only
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE Error Injection Status Register
#define PE_DCR__PCIE_EI_STATUS_offset  (0x0012) // Offset of register in instance 
#define PE_DCR__PCIE_EI_STATUS_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ei_done
//   Description: Error injection command complete - indicates the completion of error injection. In continous mode, this bit will be set when the first error is injected. This bit is cleared when pcie_ei_control.cmd is reset.
#define PE_DCR__PCIE_EI_STATUS__EI_DONE_width        1
#define PE_DCR__PCIE_EI_STATUS__EI_DONE_position     0
#define PE_DCR__PCIE_EI_STATUS__EI_DONE_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_EI_STATUS__EI_DONE_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_EI_STATUS__EI_DONE_insert(x,v)  _BGQ_INSERT(1,0,x,v)

  
// Register:
//   Name:        pcie_reset
//   Type:        Read/Write Internal
//   Permission:  Read: All; Write: Privileged and TestInt
//   Reset:       C700000200000000
//   Description: PCIE Reset Register
#define PE_DCR__PCIE_RESET_offset  (0x0013) // Offset of register in instance 
#define PE_DCR__PCIE_RESET_reset  UNSIGNED64(0xC700000200000000)

//   Field:       utl_reset
//   Description: UTL Reset When set, the UTL core will be held in the reset state.
#define PE_DCR__PCIE_RESET__UTL_RESET_width        1
#define PE_DCR__PCIE_RESET__UTL_RESET_position     0
#define PE_DCR__PCIE_RESET__UTL_RESET_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_RESET__UTL_RESET_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_RESET__UTL_RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       sys_reset
//   Description: PHY Reset. Connected to reset input of PHY. The phy is held in reset when this bit is set.
#define PE_DCR__PCIE_RESET__SYS_RESET_width        1
#define PE_DCR__PCIE_RESET__SYS_RESET_position     1
#define PE_DCR__PCIE_RESET__SYS_RESET_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_RESET__SYS_RESET_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_RESET__SYS_RESET_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       pci_out_p
//   Description: Bits 2:4 correspond to PCI_OUT_P(0:2), and are reserved for future hotplug function. Bit 5: Slot Reset. Connected to PERST# signal on slot, via PCI_OUT_P(3) pin on BG/Q ASIC. When one, the slot is held in reset. 
#define PE_DCR__PCIE_RESET__PCI_OUT_P_width        4
#define PE_DCR__PCIE_RESET__PCI_OUT_P_position     5
#define PE_DCR__PCIE_RESET__PCI_OUT_P_get(x)       _BGQ_GET(4,5,x)
#define PE_DCR__PCIE_RESET__PCI_OUT_P_set(v)       _BGQ_SET(4,5,v)
#define PE_DCR__PCIE_RESET__PCI_OUT_P_insert(x,v)  _BGQ_INSERT(4,5,x,v)

//   Field:       tldlp_error_reset
//   Description: Setting this bit to one followed by zero allows capture of tldlp error events
#define PE_DCR__PCIE_RESET__TLDLP_ERROR_RESET_width        1
#define PE_DCR__PCIE_RESET__TLDLP_ERROR_RESET_position     6
#define PE_DCR__PCIE_RESET__TLDLP_ERROR_RESET_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__PCIE_RESET__TLDLP_ERROR_RESET_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__PCIE_RESET__TLDLP_ERROR_RESET_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       utl_pclk_error_reset
//   Description: Setting this bit to one followed by zero allows capture of utl pclk error events
#define PE_DCR__PCIE_RESET__UTL_PCLK_ERROR_RESET_width        1
#define PE_DCR__PCIE_RESET__UTL_PCLK_ERROR_RESET_position     7
#define PE_DCR__PCIE_RESET__UTL_PCLK_ERROR_RESET_get(x)       _BGQ_GET(1,7,x)
#define PE_DCR__PCIE_RESET__UTL_PCLK_ERROR_RESET_set(v)       _BGQ_SET(1,7,v)
#define PE_DCR__PCIE_RESET__UTL_PCLK_ERROR_RESET_insert(x,v)  _BGQ_INSERT(1,7,x,v)

//   Field:       simspeed
//   Description: Setting this bit to one enables fast simulation. This bit must be set to 0 during normal operation of PCIE link
#define PE_DCR__PCIE_RESET__SIMSPEED_width        1
#define PE_DCR__PCIE_RESET__SIMSPEED_position     8
#define PE_DCR__PCIE_RESET__SIMSPEED_get(x)       _BGQ_GET(1,8,x)
#define PE_DCR__PCIE_RESET__SIMSPEED_set(v)       _BGQ_SET(1,8,v)
#define PE_DCR__PCIE_RESET__SIMSPEED_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       sys_disabledetect
//   Description: Setting this bit to one prevents the TLPDLP core from bringup up the link. It should only be set when the sys_reset field is set to 1, 
#define PE_DCR__PCIE_RESET__SYS_DISABLEDETECT_width        1
#define PE_DCR__PCIE_RESET__SYS_DISABLEDETECT_position     9
#define PE_DCR__PCIE_RESET__SYS_DISABLEDETECT_get(x)       _BGQ_GET(1,9,x)
#define PE_DCR__PCIE_RESET__SYS_DISABLEDETECT_set(v)       _BGQ_SET(1,9,v)
#define PE_DCR__PCIE_RESET__SYS_DISABLEDETECT_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       sys_forcedetect
//   Description: Setting this bit to one causes the receiver detect results from the PHY to be overridden by the forcedetect result field in this register. This signal must not be changed after sys_reset is deasserted.
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECT_width        1
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECT_position     10
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECT_get(x)       _BGQ_GET(1,10,x)
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECT_set(v)       _BGQ_SET(1,10,v)
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECT_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       sys_forcedetectresults
//   Description: This signal specifies the receiver detect results when forcedetect is asserted. Bit 11 represents Lane 0, and Bit 26 represents Lane 15. Only bits 11 to 19 should be set to 1. This signal must not be changed after sys_reset is deasserted
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECTRESULTS_width        16
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECTRESULTS_position     26
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECTRESULTS_get(x)       _BGQ_GET(16,26,x)
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECTRESULTS_set(v)       _BGQ_SET(16,26,v)
#define PE_DCR__PCIE_RESET__SYS_FORCEDETECTRESULTS_insert(x,v)  _BGQ_INSERT(16,26,x,v)

//   Field:       sys_changelinkwidth
//   Description: Once the link is Active, a link width change can be initiated by driving this bit to 1. The sys_targetlinkwidth field is compared against the current link width and appropriate action is taken. The sys_targetlinkwidth field must be written via a seperate, prior DCR write for proper operation
#define PE_DCR__PCIE_RESET__SYS_CHANGELINKWIDTH_width        1
#define PE_DCR__PCIE_RESET__SYS_CHANGELINKWIDTH_position     27
#define PE_DCR__PCIE_RESET__SYS_CHANGELINKWIDTH_get(x)       _BGQ_GET(1,27,x)
#define PE_DCR__PCIE_RESET__SYS_CHANGELINKWIDTH_set(v)       _BGQ_SET(1,27,v)
#define PE_DCR__PCIE_RESET__SYS_CHANGELINKWIDTH_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       sys_targetlinkwidth
//   Description: This field defines a target link width to which the core will negotiate, directed by sys_changelinkwidth field. It must be set prior to setting the sys_changelinkwidth field. Legal values are 1,2,4, and 8.
#define PE_DCR__PCIE_RESET__SYS_TARGETLINKWIDTH_width        6
#define PE_DCR__PCIE_RESET__SYS_TARGETLINKWIDTH_position     33
#define PE_DCR__PCIE_RESET__SYS_TARGETLINKWIDTH_get(x)       _BGQ_GET(6,33,x)
#define PE_DCR__PCIE_RESET__SYS_TARGETLINKWIDTH_set(v)       _BGQ_SET(6,33,v)
#define PE_DCR__PCIE_RESET__SYS_TARGETLINKWIDTH_insert(x,v)  _BGQ_INSERT(6,33,x,v)

//   Field:       sys_async_stable
//   Description: Drives utl SYS_ASYNC_STABLE input
#define PE_DCR__PCIE_RESET__SYS_ASYNC_STABLE_width        1
#define PE_DCR__PCIE_RESET__SYS_ASYNC_STABLE_position     34
#define PE_DCR__PCIE_RESET__SYS_ASYNC_STABLE_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__PCIE_RESET__SYS_ASYNC_STABLE_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__PCIE_RESET__SYS_ASYNC_STABLE_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       iw_sm_dd1_revert
//   Description: Chicken switch for incoming write state machine in pcie_db_ifc, as per BQC_BUGS issue 1533.
#define PE_DCR__PCIE_RESET__IW_SM_DD1_REVERT_width        1
#define PE_DCR__PCIE_RESET__IW_SM_DD1_REVERT_position     48
#define PE_DCR__PCIE_RESET__IW_SM_DD1_REVERT_get(x)       _BGQ_GET(1,48,x)
#define PE_DCR__PCIE_RESET__IW_SM_DD1_REVERT_set(v)       _BGQ_SET(1,48,v)
#define PE_DCR__PCIE_RESET__IW_SM_DD1_REVERT_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       iw_capture_dd1_revert
//   Description: Chicken switch for incoming write data capture from UTL in pcie_db_ifc, as per BQC_BUGS issue 1534.
#define PE_DCR__PCIE_RESET__IW_CAPTURE_DD1_REVERT_width        1
#define PE_DCR__PCIE_RESET__IW_CAPTURE_DD1_REVERT_position     49
#define PE_DCR__PCIE_RESET__IW_CAPTURE_DD1_REVERT_get(x)       _BGQ_GET(1,49,x)
#define PE_DCR__PCIE_RESET__IW_CAPTURE_DD1_REVERT_set(v)       _BGQ_SET(1,49,v)
#define PE_DCR__PCIE_RESET__IW_CAPTURE_DD1_REVERT_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       spare
//   Description: Spare Control bits
#define PE_DCR__PCIE_RESET__SPARE_width        14
#define PE_DCR__PCIE_RESET__SPARE_position     63
#define PE_DCR__PCIE_RESET__SPARE_get(x)       _BGQ_GET(14,63,x)
#define PE_DCR__PCIE_RESET__SPARE_set(v)       _BGQ_SET(14,63,v)
#define PE_DCR__PCIE_RESET__SPARE_insert(x,v)  _BGQ_INSERT(14,63,x,v)

  
// Register:
//   Name:        upc_pcie_counter
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE UPC Counters Data
#define PE_DCR__UPC_PCIE_COUNTER_offset  (0x0040) // Offset of register in instance 
#define PE_DCR__UPC_PCIE_COUNTER_range  (0xC) // Range of external address space
#define PE_DCR__UPC_PCIE_COUNTER_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ovf
//   Description: Counter Overflow bit
#define PE_DCR__UPC_PCIE_COUNTER__OVF_width        1
#define PE_DCR__UPC_PCIE_COUNTER__OVF_position     47
#define PE_DCR__UPC_PCIE_COUNTER__OVF_get(x)       _BGQ_GET(1,47,x)
#define PE_DCR__UPC_PCIE_COUNTER__OVF_set(v)       _BGQ_SET(1,47,v)
#define PE_DCR__UPC_PCIE_COUNTER__OVF_insert(x,v)  _BGQ_INSERT(1,47,x,v)

//   Field:       data
//   Description: PCIE Counter Data
#define PE_DCR__UPC_PCIE_COUNTER__DATA_width        16
#define PE_DCR__UPC_PCIE_COUNTER__DATA_position     63
#define PE_DCR__UPC_PCIE_COUNTER__DATA_get(x)       _BGQ_GET(16,63,x)
#define PE_DCR__UPC_PCIE_COUNTER__DATA_set(v)       _BGQ_SET(16,63,v)
#define PE_DCR__UPC_PCIE_COUNTER__DATA_insert(x,v)  _BGQ_INSERT(16,63,x,v)

  
// Register:
//   Name:        upc_pcie_counter_control_rw
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE UPC Counters RW Control Register
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW_offset  (0x0050) // Offset of register in instance 
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW_range  (0x1) // Range of external address space
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: PCIE Counter Reset(RW)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__RESET_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__RESET_position     0
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__RESET_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__RESET_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: PCIE Counter Enable(RW)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__ENABLE_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__ENABLE_position     32
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_RW__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_pcie_counter_control_w1s
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE UPC Counters W1S Control Register
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S_offset  (0x0051) // Offset of register in instance 
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S_range  (0x1) // Range of external address space
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: PCIE Counter Reset(W1S)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_position     0
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: PCIE Counter Enable(W1S)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_position     32
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1S__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_pcie_counter_control_w1c
//   Type:        Read/Write External
//   Permission:  All
//   Reset:       0000000000000000
//   Description: PCIE UPC Counters W1C Control Register
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C_offset  (0x0052) // Offset of register in instance 
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C_range  (0x1) // Range of external address space
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C_reset  UNSIGNED64(0x0000000000000000)

//   Field:       reset
//   Description: PCIE Counter Enable(W1C)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__RESET_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__RESET_position     0
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__RESET_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__RESET_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__RESET_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       enable
//   Description: PCIE Counter Reset(W1C)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_width        1
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_position     32
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__UPC_PCIE_COUNTER_CONTROL_W1C__ENABLE_insert(x,v)  _BGQ_INSERT(1,32,x,v)

  
// Register:
//   Name:        upc_pcie_config
//   Type:        Read/Write Internal
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UPC_PCIE Configuration Register
#define PE_DCR__UPC_PCIE_CONFIG_offset  (0x0053) // Offset of register in instance 
#define PE_DCR__UPC_PCIE_CONFIG_reset  UNSIGNED64(0x0000000000000000)

//   Field:       upc_pcie_ring_enable
//   Description: Enables UPC_PCIE Ring logic. Must be set to one to pass daisy chain data.
#define PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_width        1
#define PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_position     3
#define PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__UPC_PCIE_CONFIG__UPC_PCIE_RING_ENABLE_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       sync_override
//   Description: Sync Override bits for all counters
#define PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_width        1
#define PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_position     48
#define PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_get(x)       _BGQ_GET(1,48,x)
#define PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_set(v)       _BGQ_SET(1,48,v)
#define PE_DCR__UPC_PCIE_CONFIG__SYNC_OVERRIDE_insert(x,v)  _BGQ_INSERT(1,48,x,v)

  
// Register:
//   Name:        utl_pclk_internal_error_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UTL PCLK Error Aux Info
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO_offset  (0x001E) // Offset of register in instance 
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       cfg_access_perr
//   Description: Config Core access control parity error (UTL 0)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__CFG_ACCESS_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__CFG_ACCESS_PERR_position     0
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__CFG_ACCESS_PERR_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__CFG_ACCESS_PERR_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__CFG_ACCESS_PERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       tldlp_rcv_error
//   Description: Receive TLDLP Interface processing logic internal error (UTL 1)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TLDLP_RCV_ERROR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TLDLP_RCV_ERROR_position     1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TLDLP_RCV_ERROR_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TLDLP_RCV_ERROR_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TLDLP_RCV_ERROR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       vc0_txph0_perr
//   Description: Parity error on VC0 TX Posted Header Queue (UTL 2)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPH0_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPH0_PERR_position     2
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPH0_PERR_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPH0_PERR_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPH0_PERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       vc0_txnph0_perr
//   Description: Parity error on VC0 TX Non-Posted Header Queue (UTL 6)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPH0_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPH0_PERR_position     6
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPH0_PERR_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPH0_PERR_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPH0_PERR_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       vc0_pcitags_perr
//   Description: Parity error on VC0 PCI Tags Queue (UTL 10)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_PCITAGS_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_PCITAGS_PERR_position     10
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_PCITAGS_PERR_get(x)       _BGQ_GET(1,10,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_PCITAGS_PERR_set(v)       _BGQ_SET(1,10,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_PCITAGS_PERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       txca_perr
//   Description: Parity error on TX Completions Attributes Array (UTL 14)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCA_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCA_PERR_position     14
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCA_PERR_get(x)       _BGQ_GET(1,14,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCA_PERR_set(v)       _BGQ_SET(1,14,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCA_PERR_insert(x,v)  _BGQ_INSERT(1,14,x,v)

//   Field:       vc0_txcif_perr
//   Description: Parity error on VC0 TX Completions Indications Queue (UTL 15)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXCIF_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXCIF_PERR_position     15
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXCIF_PERR_get(x)       _BGQ_GET(1,15,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXCIF_PERR_set(v)       _BGQ_SET(1,15,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXCIF_PERR_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rxpc_perr
//   Description: Parity error on RX Posted Queue write control logic (UTL 19)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPC_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPC_PERR_position     19
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPC_PERR_get(x)       _BGQ_GET(1,19,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPC_PERR_set(v)       _BGQ_SET(1,19,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPC_PERR_insert(x,v)  _BGQ_INSERT(1,19,x,v)

//   Field:       rxcifn_perr
//   Description: Parity error on RX Completions Queue write control logic (UTL 20)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_position     20
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_get(x)       _BGQ_GET(1,20,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_set(v)       _BGQ_SET(1,20,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_insert(x,v)  _BGQ_INSERT(1,20,x,v)

//   Field:       txpcn_perr
//   Description: Parity error on TX Posted Queues read control logic (UTL 21)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_position     21
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_get(x)       _BGQ_GET(1,21,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_set(v)       _BGQ_SET(1,21,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_insert(x,v)  _BGQ_INSERT(1,21,x,v)

//   Field:       rxnpn_perr
//   Description: Parity error on RX Non-posted Queue write control logic (UTL 22)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPN_PERR_position     22
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPN_PERR_get(x)       _BGQ_GET(1,22,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPN_PERR_set(v)       _BGQ_SET(1,22,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPN_PERR_insert(x,v)  _BGQ_INSERT(1,22,x,v)

//   Field:       rxprcrq_perr
//   Description: Parity error on RX Posted Requests Credit Release queue read control logic (UTL 24)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPRCRQ_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPRCRQ_PERR_position     24
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPRCRQ_PERR_get(x)       _BGQ_GET(1,24,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPRCRQ_PERR_set(v)       _BGQ_SET(1,24,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXPRCRQ_PERR_insert(x,v)  _BGQ_INSERT(1,24,x,v)

//   Field:       txnpn_perr
//   Description: Parity error on Tx Non-posted Queues read control logic (UTL 25)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXNPN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXNPN_PERR_position     25
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXNPN_PERR_get(x)       _BGQ_GET(1,25,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXNPN_PERR_set(v)       _BGQ_SET(1,25,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXNPN_PERR_insert(x,v)  _BGQ_INSERT(1,25,x,v)

//   Field:       rxnprcrq_perr
//   Description: Parity error on RX Non-Posted Requests Credit Release Queue read control logic (UTL 26)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPRCRQ_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPRCRQ_PERR_position     26
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPRCRQ_PERR_get(x)       _BGQ_GET(1,26,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPRCRQ_PERR_set(v)       _BGQ_SET(1,26,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXNPRCRQ_PERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rxhdr_perr
//   Description: Parity error on Inbound TLPs Header Processing Logic (UTL 27)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXHDR_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXHDR_PERR_position     27
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXHDR_PERR_get(x)       _BGQ_GET(1,27,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXHDR_PERR_set(v)       _BGQ_SET(1,27,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXHDR_PERR_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rxcredit_perr
//   Description: Parity error on RX Credits logic (UTL 28)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCREDIT_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCREDIT_PERR_position     28
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCREDIT_PERR_get(x)       _BGQ_GET(1,28,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCREDIT_PERR_set(v)       _BGQ_SET(1,28,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCREDIT_PERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       txcifn_perr
//   Description: Parity error on TX Completion Indication Queue read control logic (UTL 29)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_position     29
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_get(x)       _BGQ_GET(1,29,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_set(v)       _BGQ_SET(1,29,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       txmsg_perr
//   Description: Parity error on TX Message Construction logic (UTL 30)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXMSG_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXMSG_PERR_position     30
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXMSG_PERR_get(x)       _BGQ_GET(1,30,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXMSG_PERR_set(v)       _BGQ_SET(1,30,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXMSG_PERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       txcmgmt_perr
//   Description: Parity error on TX Credit Management logic (UTL 31)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCMGMT_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCMGMT_PERR_position     31
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCMGMT_PERR_get(x)       _BGQ_GET(1,31,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCMGMT_PERR_set(v)       _BGQ_SET(1,31,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCMGMT_PERR_insert(x,v)  _BGQ_INSERT(1,31,x,v)

//   Field:       rxftn_perr
//   Description: Parity error on GBIF Tags Queues write control logic (UTL 32)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_position     32
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_insert(x,v)  _BGQ_INSERT(1,32,x,v)

//   Field:       txftn_perr
//   Description: Parity error on PCI Tags Queues read control logic (UTL 33)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_position     33
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_get(x)       _BGQ_GET(1,33,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_set(v)       _BGQ_SET(1,33,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_insert(x,v)  _BGQ_INSERT(1,33,x,v)

//   Field:       vc0_txprp_perr
//   Description: Parity error on VC0 Transmit Posted Requests Processing logic (UTL 34)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPRP_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPRP_PERR_position     34
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPRP_PERR_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPRP_PERR_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXPRP_PERR_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       txpbrq_perr
//   Description: Parity error on Tx Posted Buffer Release Queue write control logic (UTL 38)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPBRQ_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPBRQ_PERR_position     38
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPBRQ_PERR_get(x)       _BGQ_GET(1,38,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPBRQ_PERR_set(v)       _BGQ_SET(1,38,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXPBRQ_PERR_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       vc0_txnprp_perr
//   Description: Parity error on VC0 Transmit Non-posted requests Processing logic (UTL 39)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPRP_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPRP_PERR_position     39
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPRP_PERR_get(x)       _BGQ_GET(1,39,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPRP_PERR_set(v)       _BGQ_SET(1,39,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TXNPRP_PERR_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       txtlp_perr
//   Description: Parity error on Transmit TLP Construction logic (UTL 43)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXTLP_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXTLP_PERR_position     43
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXTLP_PERR_get(x)       _BGQ_GET(1,43,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXTLP_PERR_set(v)       _BGQ_SET(1,43,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXTLP_PERR_insert(x,v)  _BGQ_INSERT(1,43,x,v)

//   Field:       vc0_tcpl_perr
//   Description: Parity error on VC0 Transmit Completions Processing logic (UTL 44)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TCPL_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TCPL_PERR_position     44
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TCPL_PERR_get(x)       _BGQ_GET(1,44,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TCPL_PERR_set(v)       _BGQ_SET(1,44,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__VC0_TCPL_PERR_insert(x,v)  _BGQ_INSERT(1,44,x,v)

//   Field:       txarb_perr
//   Description: Parity error on Transmit Arbitration logic (UTL 48)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXARB_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXARB_PERR_position     48
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXARB_PERR_get(x)       _BGQ_GET(1,48,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXARB_PERR_set(v)       _BGQ_SET(1,48,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXARB_PERR_insert(x,v)  _BGQ_INSERT(1,48,x,v)

//   Field:       rxcrmgmt_perr
//   Description: Parity error on RX Credit Management logic (UTL 49)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCRMGMT_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCRMGMT_PERR_position     49
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCRMGMT_PERR_get(x)       _BGQ_GET(1,49,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCRMGMT_PERR_set(v)       _BGQ_SET(1,49,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCRMGMT_PERR_insert(x,v)  _BGQ_INSERT(1,49,x,v)

//   Field:       txcplaa_perr
//   Description: Parity error on TX Completion Attributes Array (UTL 50)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCPLAA_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCPLAA_PERR_position     50
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCPLAA_PERR_get(x)       _BGQ_GET(1,50,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCPLAA_PERR_set(v)       _BGQ_SET(1,50,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__TXCPLAA_PERR_insert(x,v)  _BGQ_INSERT(1,50,x,v)

//   Field:       rxcplaa_perr
//   Description: Parity error on RX Completion Array Logic(UTL 51)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCPLAA_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCPLAA_PERR_position     51
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCPLAA_PERR_get(x)       _BGQ_GET(1,51,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCPLAA_PERR_set(v)       _BGQ_SET(1,51,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXCPLAA_PERR_insert(x,v)  _BGQ_INSERT(1,51,x,v)

//   Field:       rxbus_perr
//   Description: Parity error on RX Bus Parsing Logic(UTL 52)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXBUS_PERR_width        1
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXBUS_PERR_position     52
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXBUS_PERR_get(x)       _BGQ_GET(1,52,x)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXBUS_PERR_set(v)       _BGQ_SET(1,52,v)
#define PE_DCR__UTL_PCLK_INTERNAL_ERROR_INFO__RXBUS_PERR_insert(x,v)  _BGQ_INSERT(1,52,x,v)

  
// Register:
//   Name:        utl_bclk_internal_error_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: UTL BCLK Error Aux Info
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO_offset  (0x001F) // Offset of register in instance 
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       gbif_inwrreq_perr
//   Description: Inbound GBIF write requests control logic parity error (UTL 0)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_INWRREQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_INWRREQ_PERR_position     0
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_INWRREQ_PERR_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_INWRREQ_PERR_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_INWRREQ_PERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       gbif_wrfrag_perr
//   Description: Inbound GBIF write fragmentation logic parity error (UTL 1)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_WRFRAG_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_WRFRAG_PERR_position     1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_WRFRAG_PERR_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_WRFRAG_PERR_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_WRFRAG_PERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       gbif_rdtrans_perr
//   Description: Inbound GBIF read transactions management logic parity error (UTL 2)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDTRANS_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDTRANS_PERR_position     2
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDTRANS_PERR_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDTRANS_PERR_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDTRANS_PERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       gbif_rdreq_perr
//   Description: Inbound GBIF read request control logic parity error (UTL 3)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDREQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDREQ_PERR_position     3
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDREQ_PERR_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDREQ_PERR_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_RDREQ_PERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       rxcpca_perr
//   Description: RX Completions Command Arry  parity error (UTL 4)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCPCA_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCPCA_PERR_position     4
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCPCA_PERR_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCPCA_PERR_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCPCA_PERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       rxpc_perr
//   Description: RX Pcvs diff pcie_dcosted Queue read control logic parity error (UTL 5)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPC_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPC_PERR_position     5
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPC_PERR_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPC_PERR_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPC_PERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       rxnpc_perr
//   Description: RX Non-Posted Queue read control logic parity error (UTL 6)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPC_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPC_PERR_position     6
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPC_PERR_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPC_PERR_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPC_PERR_insert(x,v)  _BGQ_INSERT(1,6,x,v)

//   Field:       gbif_outwrreq_perr
//   Description: Outbound GBIF write requests parity error (UTL 8)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTWRREQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTWRREQ_PERR_position     8
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTWRREQ_PERR_get(x)       _BGQ_GET(1,8,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTWRREQ_PERR_set(v)       _BGQ_SET(1,8,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTWRREQ_PERR_insert(x,v)  _BGQ_INSERT(1,8,x,v)

//   Field:       gbif_outqctl_perr
//   Description: Outbound queue control parity error (UTL 9)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTQCTL_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTQCTL_PERR_position     9
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTQCTL_PERR_get(x)       _BGQ_GET(1,9,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTQCTL_PERR_set(v)       _BGQ_SET(1,9,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTQCTL_PERR_insert(x,v)  _BGQ_INSERT(1,9,x,v)

//   Field:       pci_tagalloc_perr
//   Description: PCI Tags Allocation and Release logic parity error (UTL 10)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__PCI_TAGALLOC_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__PCI_TAGALLOC_PERR_position     10
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__PCI_TAGALLOC_PERR_get(x)       _BGQ_GET(1,10,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__PCI_TAGALLOC_PERR_set(v)       _BGQ_SET(1,10,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__PCI_TAGALLOC_PERR_insert(x,v)  _BGQ_INSERT(1,10,x,v)

//   Field:       gbif_outrdresp_perr
//   Description: Outbound GBIF Read Response logic parity error (UTL 11)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDRESP_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDRESP_PERR_position     11
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDRESP_PERR_get(x)       _BGQ_GET(1,11,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDRESP_PERR_set(v)       _BGQ_SET(1,11,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDRESP_PERR_insert(x,v)  _BGQ_INSERT(1,11,x,v)

//   Field:       gbif_outrdftch_perr
//   Description: Outbound GBIF Read Data Fetching logic parity error (UTL 12)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDFTCH_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDFTCH_PERR_position     12
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDFTCH_PERR_get(x)       _BGQ_GET(1,12,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDFTCH_PERR_set(v)       _BGQ_SET(1,12,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__GBIF_OUTRDFTCH_PERR_insert(x,v)  _BGQ_INSERT(1,12,x,v)

//   Field:       txprq_perr
//   Description: Outbound TX Posted Buffer Release Queue read control logic parity error (UTL 15)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPRQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPRQ_PERR_position     15
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPRQ_PERR_get(x)       _BGQ_GET(1,15,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPRQ_PERR_set(v)       _BGQ_SET(1,15,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPRQ_PERR_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       rxpca_perr
//   Description: RX Posted Command Array parity error (UTL 16)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPCA_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPCA_PERR_position     16
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPCA_PERR_get(x)       _BGQ_GET(1,16,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPCA_PERR_set(v)       _BGQ_SET(1,16,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPCA_PERR_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       rxnpct_perr
//   Description: RX Non Posted Command and Tag Array parity error (UTL 17)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPCT_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPCT_PERR_position     17
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPCT_PERR_get(x)       _BGQ_GET(1,17,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPCT_PERR_set(v)       _BGQ_SET(1,17,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPCT_PERR_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       txpcn_perr
//   Description: TX Posted Request Queue write control logic parity error (UTL 26)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_position     26
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_get(x)       _BGQ_GET(1,26,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_set(v)       _BGQ_SET(1,26,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXPCN_PERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)

//   Field:       rxcifn_perr
//   Description: RX Completion Indication Queue read control logic parity error (UTL 26)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_position     27
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_get(x)       _BGQ_GET(1,27,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_set(v)       _BGQ_SET(1,27,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCIFN_PERR_insert(x,v)  _BGQ_INSERT(1,27,x,v)

//   Field:       rxpbrq_perr
//   Description: RX Posted Buffer Release Queue write control logic parity error (UTL 28)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPBRQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPBRQ_PERR_position     28
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPBRQ_PERR_get(x)       _BGQ_GET(1,28,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPBRQ_PERR_set(v)       _BGQ_SET(1,28,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXPBRQ_PERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)

//   Field:       rxnpbrq_perr
//   Description: RX Non-Posted Buffer Release Queue write control logic parity error (UTL 29)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPBRQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPBRQ_PERR_position     29
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPBRQ_PERR_get(x)       _BGQ_GET(1,29,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPBRQ_PERR_set(v)       _BGQ_SET(1,29,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXNPBRQ_PERR_insert(x,v)  _BGQ_INSERT(1,29,x,v)

//   Field:       txnprq_perr
//   Description: TX Non-Posted Request Queue write control logic parity error (UTL 30)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXNPRQ_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXNPRQ_PERR_position     30
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXNPRQ_PERR_get(x)       _BGQ_GET(1,30,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXNPRQ_PERR_set(v)       _BGQ_SET(1,30,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXNPRQ_PERR_insert(x,v)  _BGQ_INSERT(1,30,x,v)

//   Field:       txcifn_perr
//   Description: TX Completion Indication Queue write control logic parity error (UTL 34)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_position     34
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXCIFN_PERR_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       rxftn_perr
//   Description: GBIF Tags Queues read control logic parity error (UTL 37)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_position     37
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_get(x)       _BGQ_GET(1,37,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_set(v)       _BGQ_SET(1,37,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXFTN_PERR_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       txftn_perr
//   Description: PCI Tags Queues write control logic parity error (UTL 38)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_position     38
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_get(x)       _BGQ_GET(1,38,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_set(v)       _BGQ_SET(1,38,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__TXFTN_PERR_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       rxctma_perr
//   Description: RX Completion Tags Management Arrray parity error (UTL 39)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCTMA_PERR_width        1
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCTMA_PERR_position     39
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCTMA_PERR_get(x)       _BGQ_GET(1,39,x)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCTMA_PERR_set(v)       _BGQ_SET(1,39,v)
#define PE_DCR__UTL_BCLK_INTERNAL_ERROR_INFO__RXCTMA_PERR_insert(x,v)  _BGQ_INSERT(1,39,x,v)

  
// Register:
//   Name:        tldlp_pclk_internal_error_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: TLDLP BCLK Error Aux Info
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO_offset  (0x0020) // Offset of register in instance 
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       ltssm_perr
//   Description: Link Training and Status State Machine (TLDLP 0)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LTSSM_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LTSSM_PERR_position     0
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LTSSM_PERR_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LTSSM_PERR_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LTSSM_PERR_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       link_attr_perr
//   Description: Link Attributes (TLDLP 1)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_ATTR_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_ATTR_PERR_position     1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_ATTR_PERR_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_ATTR_PERR_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_ATTR_PERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       link_pwr_perr
//   Description: Link Power (TLDLP 2)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_PWR_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_PWR_PERR_position     2
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_PWR_PERR_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_PWR_PERR_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__LINK_PWR_PERR_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       vc0_fcnsm_perr
//   Description: VC0 - FCNSM (TLDLP 3)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_FCNSM_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_FCNSM_PERR_position     3
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_FCNSM_PERR_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_FCNSM_PERR_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_FCNSM_PERR_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       vc0_tx_credit_perr
//   Description: VC0 - Tx Credit (TLDLP 4)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_TX_CREDIT_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_TX_CREDIT_PERR_position     4
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_TX_CREDIT_PERR_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_TX_CREDIT_PERR_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_TX_CREDIT_PERR_insert(x,v)  _BGQ_INSERT(1,4,x,v)

//   Field:       vc0_rx_credit_perr
//   Description: VC0 - Rx Credit (TLDLP 5)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_RX_CREDIT_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_RX_CREDIT_PERR_position     5
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_RX_CREDIT_PERR_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_RX_CREDIT_PERR_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__VC0_RX_CREDIT_PERR_insert(x,v)  _BGQ_INSERT(1,5,x,v)

//   Field:       xmit_perr
//   Description: Xmit Logic (TLDLP 15)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_PERR_position     15
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_PERR_get(x)       _BGQ_GET(1,15,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_PERR_set(v)       _BGQ_SET(1,15,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_PERR_insert(x,v)  _BGQ_INSERT(1,15,x,v)

//   Field:       xmit_attr0_perr
//   Description: Xmit Logic (TLDLP 16)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR0_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR0_PERR_position     16
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR0_PERR_get(x)       _BGQ_GET(1,16,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR0_PERR_set(v)       _BGQ_SET(1,16,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR0_PERR_insert(x,v)  _BGQ_INSERT(1,16,x,v)

//   Field:       xmit_attr1_perr
//   Description: Xmit Logic (TLDLP 17)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR1_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR1_PERR_position     17
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR1_PERR_get(x)       _BGQ_GET(1,17,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR1_PERR_set(v)       _BGQ_SET(1,17,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_ATTR1_PERR_insert(x,v)  _BGQ_INSERT(1,17,x,v)

//   Field:       xmit_tlpq_perr
//   Description: Xmit TLP Queue (TLDLP 18-33)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQ_PERR_width        16
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQ_PERR_position     33
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQ_PERR_get(x)       _BGQ_GET(16,33,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQ_PERR_set(v)       _BGQ_SET(16,33,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQ_PERR_insert(x,v)  _BGQ_INSERT(16,33,x,v)

//   Field:       xmit_tlpqa_perr
//   Description: Xmit TLP Queue (TLDLP 34)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQA_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQA_PERR_position     34
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQA_PERR_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQA_PERR_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__XMIT_TLPQA_PERR_insert(x,v)  _BGQ_INSERT(1,34,x,v)

//   Field:       rcv_attr0_perr
//   Description: RCV Logic State (TLDLP 35)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR0_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR0_PERR_position     35
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR0_PERR_get(x)       _BGQ_GET(1,35,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR0_PERR_set(v)       _BGQ_SET(1,35,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR0_PERR_insert(x,v)  _BGQ_INSERT(1,35,x,v)

//   Field:       rcv_tlpi_perr
//   Description: RCV TLP Info (TLDLP 36)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPI_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPI_PERR_position     36
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPI_PERR_get(x)       _BGQ_GET(1,36,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPI_PERR_set(v)       _BGQ_SET(1,36,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPI_PERR_insert(x,v)  _BGQ_INSERT(1,36,x,v)

//   Field:       rcv_tlpie_perr
//   Description: RCV TLP End Info (TLDLP 38)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPIE_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPIE_PERR_position     37
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPIE_PERR_get(x)       _BGQ_GET(1,37,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPIE_PERR_set(v)       _BGQ_SET(1,37,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_TLPIE_PERR_insert(x,v)  _BGQ_INSERT(1,37,x,v)

//   Field:       rcv_attr1_perr
//   Description: RCV TLP End Info (TLDLP 39)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR1_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR1_PERR_position     38
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR1_PERR_get(x)       _BGQ_GET(1,38,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR1_PERR_set(v)       _BGQ_SET(1,38,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_ATTR1_PERR_insert(x,v)  _BGQ_INSERT(1,38,x,v)

//   Field:       rcv_ca_perr
//   Description: RCV Credit Allocated(TLDLP 44)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CA_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CA_PERR_position     39
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CA_PERR_get(x)       _BGQ_GET(1,39,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CA_PERR_set(v)       _BGQ_SET(1,39,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CA_PERR_insert(x,v)  _BGQ_INSERT(1,39,x,v)

//   Field:       rcv_cr_perr
//   Description: RCV Credit Received(TLDLP 45)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CR_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CR_PERR_position     40
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CR_PERR_get(x)       _BGQ_GET(1,40,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CR_PERR_set(v)       _BGQ_SET(1,40,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_CR_PERR_insert(x,v)  _BGQ_INSERT(1,40,x,v)

//   Field:       rcv_ovf_perr
//   Description: RCV Overflow(TLDLP 46)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_OVF_PERR_width        1
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_OVF_PERR_position     41
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_OVF_PERR_get(x)       _BGQ_GET(1,41,x)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_OVF_PERR_set(v)       _BGQ_SET(1,41,v)
#define PE_DCR__TLDLP_PCLK_INTERNAL_ERROR_INFO__RCV_OVF_PERR_insert(x,v)  _BGQ_INSERT(1,41,x,v)

  
// Register:
//   Name:        interrupt_internal_error_sw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for software type internal errors
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_offset  (0x0039) // Offset of register in instance 
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       read
//   Description: set if the transaction was a read
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_position     0
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__READ_insert(x,v)  _BGQ_INSERT(1,0,x,v)

//   Field:       write
//   Description: set if the transaction was a write
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_position     1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__WRITE_insert(x,v)  _BGQ_INSERT(1,1,x,v)

//   Field:       privileged
//   Description: set if the transaction had privileged permission
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_position     2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__PRIVILEGED_insert(x,v)  _BGQ_INSERT(1,2,x,v)

//   Field:       test_int
//   Description: set if the transaction had test_int permission
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_position     3
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__TEST_INT_insert(x,v)  _BGQ_INSERT(1,3,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_width        24
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_position     63
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_SW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_hw_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_offset  (0x003A) // Offset of register in instance 
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       address_parity_good
//   Description: reflects each individual parity status for address
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_width        3
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_position     6
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_get(x)       _BGQ_GET(3,6,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_set(v)       _BGQ_SET(3,6,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(3,6,x,v)

//   Field:       data_parity_good
//   Description: reflects each individual parity status for data
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_width        8
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_position     14
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_get(x)       _BGQ_GET(8,14,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_set(v)       _BGQ_SET(8,14,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__DATA_PARITY_GOOD_insert(x,v)  _BGQ_INSERT(8,14,x,v)

//   Field:       address
//   Description: reflects the address at the time of error
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_width        24
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_position     63
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_get(x)       _BGQ_GET(24,63,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_set(v)       _BGQ_SET(24,63,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_HW_INFO__ADDRESS_insert(x,v)  _BGQ_INSERT(24,63,x,v)

  
// Register:
//   Name:        interrupt_internal_error_data_info
//   Type:        Interrupt Information
//   Permission:  All
//   Reset:       0000000000000000
//   Description: Contains status for hardware type internal errors
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_offset  (0x003B) // Offset of register in instance 
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO_reset  UNSIGNED64(0x0000000000000000)

//   Field:       data
//   Description: reflects the local data at the time of error
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_width        64
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_position     63
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_get(x)       _BGQ_GET(64,63,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_set(v)       _BGQ_SET(64,63,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_DATA_INFO__DATA_insert(x,v)  _BGQ_INSERT(64,63,x,v)

  
// Interrupt State Register: pcie_interrupt_state
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define PE_DCR__PCIE_INTERRUPT_STATE__STATE_offset         (0x0014) // Offset of State register in instance
#define PE_DCR__PCIE_INTERRUPT_STATE__FIRST_offset         (0x0018) // Offset of First register in instance
#define PE_DCR__PCIE_INTERRUPT_STATE__FORCE_offset         (0x0017) // Offset of Force register in instance
#define PE_DCR__PCIE_INTERRUPT_STATE__MACHINE_CHECK_offset (0x0019) // Offset of Machine Check register in instance
#define PE_DCR__PCIE_INTERRUPT_STATE__CRITICAL_offset      (0x001A) // Offset of Critical register in instance
#define PE_DCR__PCIE_INTERRUPT_STATE__NONCRITICAL_offset   (0x001B) // Offset of Noncritical register in instance

#define PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_position     0
#define PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__LOCAL_RING_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__SERR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__SERR_position     1
#define PE_DCR__PCIE_INTERRUPT_STATE__SERR_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__SERR_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__SERR_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_position     2
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_UR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_UR_position     3
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_UR_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_UR_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_UR_insert(x,v)  _BGQ_INSERT(1,3,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_RETRY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_RETRY_position     4
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_RETRY_get(x)       _BGQ_GET(1,4,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_RETRY_set(v)       _BGQ_SET(1,4,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_WR_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(1,4,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_position     5
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_get(x)       _BGQ_GET(1,5,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_set(v)       _BGQ_SET(1,5,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OW_PTY_insert(x,v)  _BGQ_INSERT(1,5,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_position     6
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_get(x)       _BGQ_GET(1,6,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_set(v)       _BGQ_SET(1,6,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_insert(x,v)  _BGQ_INSERT(1,6,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_UR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_UR_position     7
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_UR_get(x)       _BGQ_GET(1,7,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_UR_set(v)       _BGQ_SET(1,7,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_UR_insert(x,v)  _BGQ_INSERT(1,7,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_RETRY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_RETRY_position     8
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_RETRY_get(x)       _BGQ_GET(1,8,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_RETRY_set(v)       _BGQ_SET(1,8,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_CFG_RD_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(1,8,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_position     9
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_get(x)       _BGQ_GET(1,9,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_set(v)       _BGQ_SET(1,9,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_PTY_insert(x,v)  _BGQ_INSERT(1,9,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_position     10
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_get(x)       _BGQ_GET(1,10,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_set(v)       _BGQ_SET(1,10,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_TAG_STORE_insert(x,v)  _BGQ_INSERT(1,10,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_position     11
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_get(x)       _BGQ_GET(1,11,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_set(v)       _BGQ_SET(1,11,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_OR_POISONED_insert(x,v)  _BGQ_INSERT(1,11,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_position     12
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_get(x)       _BGQ_GET(1,12,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_set(v)       _BGQ_SET(1,12,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_PTY_insert(x,v)  _BGQ_INSERT(1,12,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SIZE_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SIZE_position     13
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SIZE_get(x)       _BGQ_GET(1,13,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SIZE_set(v)       _BGQ_SET(1,13,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SIZE_insert(x,v)  _BGQ_INSERT(1,13,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_position     14
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_get(x)       _BGQ_GET(1,14,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_set(v)       _BGQ_SET(1,14,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_TYPE_insert(x,v)  _BGQ_INSERT(1,14,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_position     16
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_get(x)       _BGQ_GET(1,16,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_set(v)       _BGQ_SET(1,16,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_REQ_SNOOP_insert(x,v)  _BGQ_INSERT(1,16,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_START_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_START_position     17
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_START_get(x)       _BGQ_GET(1,17,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_START_set(v)       _BGQ_SET(1,17,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_START_insert(x,v)  _BGQ_INSERT(1,17,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_END_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_END_position     18
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_END_get(x)       _BGQ_GET(1,18,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_END_set(v)       _BGQ_SET(1,18,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_RANGE_END_insert(x,v)  _BGQ_INSERT(1,18,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_MAX_TOP_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_MAX_TOP_position     19
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_MAX_TOP_get(x)       _BGQ_GET(1,19,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_MAX_TOP_set(v)       _BGQ_SET(1,19,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_MAX_TOP_insert(x,v)  _BGQ_INSERT(1,19,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_BOUNDS_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_BOUNDS_position     20
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_BOUNDS_get(x)       _BGQ_GET(1,20,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_BOUNDS_set(v)       _BGQ_SET(1,20,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_BOUNDS_insert(x,v)  _BGQ_INSERT(1,20,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_position     21
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_get(x)       _BGQ_GET(1,21,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_set(v)       _BGQ_SET(1,21,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IW_SM_STATE_insert(x,v)  _BGQ_INSERT(1,21,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_position     22
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_get(x)       _BGQ_GET(1,22,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_set(v)       _BGQ_SET(1,22,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_REQ_SNOOP_insert(x,v)  _BGQ_INSERT(1,22,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_position     23
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_get(x)       _BGQ_GET(1,23,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_set(v)       _BGQ_SET(1,23,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_RESP_insert(x,v)  _BGQ_INSERT(1,23,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_position     24
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_get(x)       _BGQ_GET(1,24,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_set(v)       _BGQ_SET(1,24,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_IR_ECC_insert(x,v)  _BGQ_INSERT(1,24,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_position     25
#define PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_get(x)       _BGQ_GET(1,25,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_set(v)       _BGQ_SET(1,25,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__CFG_PERR_insert(x,v)  _BGQ_INSERT(1,25,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_position     26
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_get(x)       _BGQ_GET(1,26,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_set(v)       _BGQ_SET(1,26,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_PCLK_ERR_insert(x,v)  _BGQ_INSERT(1,26,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_position     27
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_get(x)       _BGQ_GET(1,27,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_set(v)       _BGQ_SET(1,27,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_BCLK_ERR_insert(x,v)  _BGQ_INSERT(1,27,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_position     28
#define PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_get(x)       _BGQ_GET(1,28,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_set(v)       _BGQ_SET(1,28,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__TLDLP_PCLK_ERR_insert(x,v)  _BGQ_INSERT(1,28,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_position     29
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_get(x)       _BGQ_GET(1,29,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_set(v)       _BGQ_SET(1,29,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_PINT_insert(x,v)  _BGQ_INSERT(1,29,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__AL_SINT_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_SINT_position     30
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_SINT_get(x)       _BGQ_GET(1,30,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_SINT_set(v)       _BGQ_SET(1,30,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__AL_SINT_insert(x,v)  _BGQ_INSERT(1,30,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_position     31
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_get(x)       _BGQ_GET(1,31,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_set(v)       _BGQ_SET(1,31,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__UTL_SM_PTY_insert(x,v)  _BGQ_INSERT(1,31,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__UPC_COUNTER_OVF_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__UPC_COUNTER_OVF_position     32
#define PE_DCR__PCIE_INTERRUPT_STATE__UPC_COUNTER_OVF_get(x)       _BGQ_GET(1,32,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__UPC_COUNTER_OVF_set(v)       _BGQ_SET(1,32,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(1,32,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_position     33
#define PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_get(x)       _BGQ_GET(1,33,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_set(v)       _BGQ_SET(1,33,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__EI_SM_ERROR_insert(x,v)  _BGQ_INSERT(1,33,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_position     34
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_get(x)       _BGQ_GET(1,34,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_set(v)       _BGQ_SET(1,34,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_insert(x,v)  _BGQ_INSERT(1,34,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_position     35
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_get(x)       _BGQ_GET(1,35,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_set(v)       _BGQ_SET(1,35,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_UR_insert(x,v)  _BGQ_INSERT(1,35,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_width        1
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_position     36
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_get(x)       _BGQ_GET(1,36,x)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_set(v)       _BGQ_SET(1,36,v)
#define PE_DCR__PCIE_INTERRUPT_STATE__PCI_MEM_RD_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(1,36,x,v)

  
// Interrupt State Register: interrupt_internal_error
//   State Permission: Read: All; Write: Privileged and TestInt
//   First Permission: All
//   Force Permission: Privileged and TestInt
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__STATE_offset         (0x0032) // Offset of State register in instance
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__FIRST_offset         (0x0035) // Offset of First register in instance
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__FORCE_offset         (0x0034) // Offset of Force register in instance
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__MACHINE_CHECK_offset (0x0036) // Offset of Machine Check register in instance
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__CRITICAL_offset      (0x0037) // Offset of Critical register in instance
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__NONCRITICAL_offset   (0x0038) // Offset of Noncritical register in instance

#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_position     0
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_get(x)       _BGQ_GET(1,0,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_set(v)       _BGQ_SET(1,0,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(1,0,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_position     1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_get(x)       _BGQ_GET(1,1,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_set(v)       _BGQ_SET(1,1,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_PRIV_insert(x,v)  _BGQ_INSERT(1,1,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_position     2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_get(x)       _BGQ_GET(1,2,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_set(v)       _BGQ_SET(1,2,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(1,2,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_width        1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_position     3
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(1,3,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(1,3,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(1,3,x,v)

  
// Interrupt Control Register: pcie_interrupt_state_control_low
//   Permission: Read: All; Write: Privileged and TestInt
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW_offset         (0x0015) // Offset of Control register in instance

#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_position     1
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_get(x)       _BGQ_GET(2,1,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_set(v)       _BGQ_SET(2,1,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__UPC_COUNTER_OVF_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__EI_SM_ERROR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__EI_SM_ERROR_position     3
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__EI_SM_ERROR_get(x)       _BGQ_GET(2,3,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__EI_SM_ERROR_set(v)       _BGQ_SET(2,3,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__EI_SM_ERROR_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_position     5
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_get(x)       _BGQ_GET(2,5,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_set(v)       _BGQ_SET(2,5,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_UR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_UR_position     7
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_UR_get(x)       _BGQ_GET(2,7,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_UR_set(v)       _BGQ_SET(2,7,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_UR_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_RETRY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_RETRY_position     9
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_RETRY_get(x)       _BGQ_GET(2,9,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_RETRY_set(v)       _BGQ_SET(2,9,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_LOW__PCI_MEM_RD_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(2,9,x,v)

  
// Interrupt Control Register: pcie_interrupt_state_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH_offset         (0x0016) // Offset of Control register in instance

#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_position     1
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_get(x)       _BGQ_GET(2,1,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_set(v)       _BGQ_SET(2,1,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__LOCAL_RING_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__SERR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__SERR_position     3
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__SERR_get(x)       _BGQ_GET(2,3,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__SERR_set(v)       _BGQ_SET(2,3,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__SERR_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_position     5
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_get(x)       _BGQ_GET(2,5,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_set(v)       _BGQ_SET(2,5,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_UR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_UR_position     7
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_UR_get(x)       _BGQ_GET(2,7,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_UR_set(v)       _BGQ_SET(2,7,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_UR_insert(x,v)  _BGQ_INSERT(2,7,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_RETRY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_RETRY_position     9
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_RETRY_get(x)       _BGQ_GET(2,9,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_RETRY_set(v)       _BGQ_SET(2,9,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_WR_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(2,9,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OW_PTY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OW_PTY_position     11
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OW_PTY_get(x)       _BGQ_GET(2,11,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OW_PTY_set(v)       _BGQ_SET(2,11,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OW_PTY_insert(x,v)  _BGQ_INSERT(2,11,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_position     13
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_get(x)       _BGQ_GET(2,13,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_set(v)       _BGQ_SET(2,13,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_insert(x,v)  _BGQ_INSERT(2,13,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_UR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_UR_position     15
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_UR_get(x)       _BGQ_GET(2,15,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_UR_set(v)       _BGQ_SET(2,15,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_UR_insert(x,v)  _BGQ_INSERT(2,15,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_RETRY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_RETRY_position     17
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_RETRY_get(x)       _BGQ_GET(2,17,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_RETRY_set(v)       _BGQ_SET(2,17,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_CFG_RD_ERROR_RETRY_insert(x,v)  _BGQ_INSERT(2,17,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_PTY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_PTY_position     19
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_PTY_get(x)       _BGQ_GET(2,19,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_PTY_set(v)       _BGQ_SET(2,19,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_PTY_insert(x,v)  _BGQ_INSERT(2,19,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_TAG_STORE_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_TAG_STORE_position     21
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_TAG_STORE_get(x)       _BGQ_GET(2,21,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_TAG_STORE_set(v)       _BGQ_SET(2,21,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_TAG_STORE_insert(x,v)  _BGQ_INSERT(2,21,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_POISONED_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_POISONED_position     23
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_POISONED_get(x)       _BGQ_GET(2,23,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_POISONED_set(v)       _BGQ_SET(2,23,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_OR_POISONED_insert(x,v)  _BGQ_INSERT(2,23,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_PTY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_PTY_position     25
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_PTY_get(x)       _BGQ_GET(2,25,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_PTY_set(v)       _BGQ_SET(2,25,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_PTY_insert(x,v)  _BGQ_INSERT(2,25,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SIZE_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SIZE_position     27
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SIZE_get(x)       _BGQ_GET(2,27,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SIZE_set(v)       _BGQ_SET(2,27,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SIZE_insert(x,v)  _BGQ_INSERT(2,27,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_TYPE_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_TYPE_position     29
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_TYPE_get(x)       _BGQ_GET(2,29,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_TYPE_set(v)       _BGQ_SET(2,29,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_TYPE_insert(x,v)  _BGQ_INSERT(2,29,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SNOOP_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SNOOP_position     33
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SNOOP_get(x)       _BGQ_GET(2,33,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SNOOP_set(v)       _BGQ_SET(2,33,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_REQ_SNOOP_insert(x,v)  _BGQ_INSERT(2,33,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_START_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_START_position     35
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_START_get(x)       _BGQ_GET(2,35,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_START_set(v)       _BGQ_SET(2,35,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_START_insert(x,v)  _BGQ_INSERT(2,35,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_END_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_END_position     37
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_END_get(x)       _BGQ_GET(2,37,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_END_set(v)       _BGQ_SET(2,37,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_RANGE_END_insert(x,v)  _BGQ_INSERT(2,37,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_MAX_TOP_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_MAX_TOP_position     39
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_MAX_TOP_get(x)       _BGQ_GET(2,39,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_MAX_TOP_set(v)       _BGQ_SET(2,39,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_MAX_TOP_insert(x,v)  _BGQ_INSERT(2,39,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_BOUNDS_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_BOUNDS_position     41
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_BOUNDS_get(x)       _BGQ_GET(2,41,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_BOUNDS_set(v)       _BGQ_SET(2,41,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_BOUNDS_insert(x,v)  _BGQ_INSERT(2,41,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_SM_STATE_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_SM_STATE_position     43
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_SM_STATE_get(x)       _BGQ_GET(2,43,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_SM_STATE_set(v)       _BGQ_SET(2,43,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IW_SM_STATE_insert(x,v)  _BGQ_INSERT(2,43,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_REQ_SNOOP_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_REQ_SNOOP_position     45
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_REQ_SNOOP_get(x)       _BGQ_GET(2,45,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_REQ_SNOOP_set(v)       _BGQ_SET(2,45,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_REQ_SNOOP_insert(x,v)  _BGQ_INSERT(2,45,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_RESP_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_RESP_position     47
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_RESP_get(x)       _BGQ_GET(2,47,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_RESP_set(v)       _BGQ_SET(2,47,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_RESP_insert(x,v)  _BGQ_INSERT(2,47,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_ECC_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_ECC_position     49
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_ECC_get(x)       _BGQ_GET(2,49,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_ECC_set(v)       _BGQ_SET(2,49,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__PCI_IR_ECC_insert(x,v)  _BGQ_INSERT(2,49,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__CFG_PERR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__CFG_PERR_position     51
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__CFG_PERR_get(x)       _BGQ_GET(2,51,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__CFG_PERR_set(v)       _BGQ_SET(2,51,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__CFG_PERR_insert(x,v)  _BGQ_INSERT(2,51,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_PCLK_ERR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_PCLK_ERR_position     53
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_PCLK_ERR_get(x)       _BGQ_GET(2,53,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_PCLK_ERR_set(v)       _BGQ_SET(2,53,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_PCLK_ERR_insert(x,v)  _BGQ_INSERT(2,53,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_BCLK_ERR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_BCLK_ERR_position     55
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_BCLK_ERR_get(x)       _BGQ_GET(2,55,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_BCLK_ERR_set(v)       _BGQ_SET(2,55,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_BCLK_ERR_insert(x,v)  _BGQ_INSERT(2,55,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__TLDLP_PCLK_ERR_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__TLDLP_PCLK_ERR_position     57
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__TLDLP_PCLK_ERR_get(x)       _BGQ_GET(2,57,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__TLDLP_PCLK_ERR_set(v)       _BGQ_SET(2,57,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__TLDLP_PCLK_ERR_insert(x,v)  _BGQ_INSERT(2,57,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_PINT_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_PINT_position     59
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_PINT_get(x)       _BGQ_GET(2,59,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_PINT_set(v)       _BGQ_SET(2,59,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_PINT_insert(x,v)  _BGQ_INSERT(2,59,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_SINT_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_SINT_position     61
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_SINT_get(x)       _BGQ_GET(2,61,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_SINT_set(v)       _BGQ_SET(2,61,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__AL_SINT_insert(x,v)  _BGQ_INSERT(2,61,x,v)


#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_SM_PTY_width        2
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_SM_PTY_position     63
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_SM_PTY_get(x)       _BGQ_GET(2,63,x)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_SM_PTY_set(v)       _BGQ_SET(2,63,v)
#define PE_DCR__PCIE_INTERRUPT_STATE_CONTROL_HIGH__UTL_SM_PTY_insert(x,v)  _BGQ_INSERT(2,63,x,v)

  
// Interrupt Control Register: interrupt_internal_error_control_high
//   Permission: Read: All; Write: Privileged and TestInt
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH_offset         (0x0033) // Offset of Control register in instance

#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_width        2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_position     1
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_get(x)       _BGQ_GET(2,1,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_set(v)       _BGQ_SET(2,1,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_insert(x,v)  _BGQ_INSERT(2,1,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_width        2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_position     3
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_get(x)       _BGQ_GET(2,3,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_set(v)       _BGQ_SET(2,3,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_PRIV_insert(x,v)  _BGQ_INSERT(2,3,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_width        2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_position     5
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_get(x)       _BGQ_GET(2,5,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_set(v)       _BGQ_SET(2,5,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_DATA_PARITY_insert(x,v)  _BGQ_INSERT(2,5,x,v)


#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_width        2
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_position     7
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_get(x)       _BGQ_GET(2,7,x)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_set(v)       _BGQ_SET(2,7,v)
#define PE_DCR__INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH__BAD_ADDRESS_PARITY_insert(x,v)  _BGQ_INSERT(2,7,x,v)

#ifndef __ASSEMBLY__

typedef struct pe_dcr { 
    /* 0x0000 */ uint64_t pcie_config;
    /* 0x0001 */ uint64_t pcie_sourceid;
    /* 0x0002 */ uint64_t pcie_dl_config;
    /* 0x0003 */ uint64_t pcie_dl_status;
    /* 0x0004 */ uint64_t pcie_cfg00;
    /* 0x0005 */ uint64_t pcie_cfg08;
    /* 0x0006 */ uint64_t pcie_ec00;
    /* 0x0007 */ uint64_t pcie_ec04;
    /* 0x0008 */ uint64_t pcie_ec08;
    /* 0x0009 */ uint64_t pcie_ec0c;
    /* 0x000A */ uint64_t pcie_ec14;
    /* 0x000B */ uint64_t pcie_ec18;
    /* 0x000C */ uint64_t pcie_ec18_stat;
    /* 0x000D */ uint64_t pcie_ec24;
    /* 0x000E */ uint64_t pcie_aer_config;
    /* 0x000F */ uint64_t pcie_ifc_cfg;
    /* 0x0010 */ uint64_t reserve_0010;
    /* 0x0011 */ uint64_t pcie_ei_control;
    /* 0x0012 */ uint64_t pcie_ei_status;
    /* 0x0013 */ uint64_t pcie_reset;
    /* 0x0014 */ uint64_t pcie_interrupt_state__state;
    /* 0x0015 */ uint64_t pcie_interrupt_state_control_low;
    /* 0x0016 */ uint64_t pcie_interrupt_state_control_high;
    /* 0x0017 */ uint64_t pcie_interrupt_state__force;
    /* 0x0018 */ uint64_t pcie_interrupt_state__first;
    /* 0x0019 */ uint64_t pcie_interrupt_state__machine_check;
    /* 0x001A */ uint64_t pcie_interrupt_state__critical;
    /* 0x001B */ uint64_t pcie_interrupt_state__noncritical;
    /* 0x001C */ uint64_t reserve_001C[0x0002];
    /* 0x001E */ uint64_t utl_pclk_internal_error_info;
    /* 0x001F */ uint64_t utl_bclk_internal_error_info;
    /* 0x0020 */ uint64_t tldlp_pclk_internal_error_info;
    /* 0x0021 */ uint64_t reserve_0021[0x0011];
    /* 0x0032 */ uint64_t interrupt_internal_error__state;
    /* 0x0033 */ uint64_t interrupt_internal_error_control_high;
    /* 0x0034 */ uint64_t interrupt_internal_error__force;
    /* 0x0035 */ uint64_t interrupt_internal_error__first;
    /* 0x0036 */ uint64_t interrupt_internal_error__machine_check;
    /* 0x0037 */ uint64_t interrupt_internal_error__critical;
    /* 0x0038 */ uint64_t interrupt_internal_error__noncritical;
    /* 0x0039 */ uint64_t interrupt_internal_error_sw_info;
    /* 0x003A */ uint64_t interrupt_internal_error_hw_info;
    /* 0x003B */ uint64_t interrupt_internal_error_data_info;
    /* 0x003C */ uint64_t reserve_003C[0x0004];
    /* 0x0040 */ uint64_t upc_pcie_counter[PE_DCR__UPC_PCIE_COUNTER_range];
    /* 0x004C */ uint64_t reserve_004C[0x0004];
    /* 0x0050 */ uint64_t upc_pcie_counter_control_rw;
    /* 0x0051 */ uint64_t upc_pcie_counter_control_w1s;
    /* 0x0052 */ uint64_t upc_pcie_counter_control_w1c;
    /* 0x0053 */ uint64_t upc_pcie_config;
    /* 0x0054 */ uint64_t reserve_0054[0x00AC];
    /* 0x0100 */ uint64_t pcie_utl[PE_DCR__PCIE_UTL_range];
} pe_dcr_t;
  
#define PE_DCR_USER_PTR _DCR_USER_PTR(pe_dcr_t, PE_DCR)
#define PE_DCR_PRIV_PTR _DCR_PRIV_PTR(pe_dcr_t, PE_DCR)
  
#endif // !__ASSEMBLY__
  
__END_DECLS
  
#endif // add nothing below this line.
