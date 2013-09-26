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

//
// Macros for BGQ Torus and Messaging Unit
//

#ifndef _MU_MACROS_H_
#define _MU_MACROS_H_

#include <hwi/include/common/compiler_support.h>

#include <hwi/include/bqc/PhysicalMap.h>




// Uses the new address space for MU DCR's
#define BGQ_MU_DCR_USE_NEW_MAPPING 1

#ifdef BGQ_MU_DCR_USE_NEW_MAPPING
#include <hwi/include/bqc/mu_dcr.h>
#endif

// -------------------------------------
// BGQ Messaging Unit definitions
// -------------------------------------

// Number of groups 17 (same as number of cores)
// Number of sub-groups 4
// Number of Injection Memory FIFOs per group 32
// Number of Reception Memory FIFOs per group 16
// Number of Data and Counter Base Adress per group 32
// Each group contains 4 sub-groups

// Old address space - TO BE DISCONTINUED

//#define _BGQ_BASE_DMA       0x600000000ULL // Base address (64-bits)

//#define _BGQ_BASE_DMA        0x3FFFE800000ULL // MSG Unit range

//#define _BGQ_BASE_DMA        0x3fffe000000ull // MSG Unit start address

// MSG Unit Start Address (Supervisor access)
#define _BGQ_BASE_DMA        (PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET)

#define _BGQ_SIZE_DMA        1114112         // size  (17*65536 = 17*64K bytes)


#define BGQ_MU_NUM_FIFO_GROUPS              17  // 17 groups of FIFOs
#define BGQ_MU_NUM_FIFO_SUBGROUPS           4   // 4 sub-groups per group
#define BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE  (BGQ_MU_NUM_FIFO_GROUPS * BGQ_MU_NUM_FIFO_SUBGROUPS)

#define BGQ_MU_NUM_INJ_FIFO_GROUPS          BGQ_MU_NUM_FIFO_GROUPS
#define BGQ_MU_NUM_INJ_FIFO_SUBGROUPS       BGQ_MU_NUM_FIFO_SUBGROUPS
//#define BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP   8
#define BGQ_MU_NUM_INJ_FIFOS_PER_GROUP      32
#define BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP   (BGQ_MU_NUM_INJ_FIFOS_PER_GROUP / BGQ_MU_NUM_FIFO_SUBGROUPS)

#define BGQ_MU_NUM_INJ_FIFOS                (BGQ_MU_NUM_INJ_FIFO_GROUPS * BGQ_MU_NUM_INJ_FIFOS_PER_GROUP)

#define BGQ_MU_NUM_REC_FIFO_GROUPS          BGQ_MU_NUM_FIFO_GROUPS
#define BGQ_MU_NUM_REC_FIFO_SUBGROUPS       BGQ_MU_NUM_FIFO_SUBGROUPS
//#define BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP   4
#define BGQ_MU_NUM_REC_FIFOS_PER_GROUP      16 
#define BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP   (BGQ_MU_NUM_REC_FIFOS_PER_GROUP / BGQ_MU_NUM_FIFO_SUBGROUPS)

#define BGQ_MU_NUM_REC_FIFOS                (BGQ_MU_NUM_REC_FIFO_GROUPS * BGQ_MU_NUM_REC_FIFOS_PER_GROUP)

//#define BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP   8
#define BGQ_MU_NUM_DATA_COUNTERS_PER_GROUP  32
#define BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP  (BGQ_MU_NUM_DATA_COUNTERS_PER_GROUP /  BGQ_MU_NUM_FIFO_SUBGROUPS)




// ------------------------------------------------
// ------ BGQ Messaging Unit address range --------
// ------------------------------------------------

// Address range per sub-group
// 0x00000 - 0x028F  - registers
// 0x00290 - 0x00FFF - padding
// Address range per group
// 0x00000 - 0xFFFFF 

#define BGQ_MU_GROUP_SIZE                           0x010000
#define BGQ_MU_SUBGROUP_SIZE                        0x001000

#define BGQ_MU_GROUP_ADDRESS_OFFSET(g)              _BGQ_BASE_DMA + ((g)*(BGQ_MU_GROUP_SIZE))

#define BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg)        BGQ_MU_GROUP_ADDRESS_OFFSET(g) + (sg)*(BGQ_MU_SUBGROUP_SIZE)

// ------------------------------------
// Start offset for MU imFIFO 
// ------------------------------------
#define BGQ_MU_iDMA_GROUP_START_OFFSET(g)           ( BGQ_MU_GROUP_ADDRESS_OFFSET(g) + 0x00000 )

#define BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg)     ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + 0x00000 )


// Injection Memory FIFO

#define BGQ_MU_iDMA_FIFO_REGS_SIZE                  (8*8*8)     // 0x0200

// repeats 8 times i=0 to 7 per sub-group
#define BGQ_MU_iDMA_START_OFFSET(g,sg,i)            ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x00 + (i)*(8*8) )
#define BGQ_MU_iDMA_SIZE_OFFSET(g,sg,i)             ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x08 + (i)*(8*8) )
#define BGQ_MU_iDMA_HEAD_OFFSET(g,sg,i)             ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x10 + (i)*(8*8) ) 
#define BGQ_MU_iDMA_TAIL_OFFSET(g,sg,i)             ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x18 + (i)*(8*8) )
#define BGQ_MU_iDMA_DESC_COUNTER_OFFSET(g,sg,i)     ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x20 + (i)*(8*8) )
#define BGQ_MU_iDMA_FREE_SPACE_OFFSET(g,sg,i)       ( BGQ_MU_iDMA_SUBGROUP_START_OFFSET(g,sg) + 0x28 + (i)*(8*8) )

#define BGQ_MU_iDMA_FREE_HOLE1(g,sg,i)              ( BGQ_MU_iDMA_SUBGROUP_HOLE1_OFFSET(g,sg) + 0x30 + (i)*(8*8) )
#define BGQ_MU_iDMA_FREE_HOLE2(g,sg,i)              ( BGQ_MU_iDMA_SUBGROUP_HOLE2_OFFSET(g,sg) + 0x38 + (i)*(8*8) )

#define BGQ_MU_rDMA_STARTING_OFFSET                 (BGQ_MU_iDMA_FIFO_REGS_SIZE)

// computes offset for reception memory FIFO
#define BGQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg)     ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + BGQ_MU_iDMA_FIFO_REGS_SIZE )


// Reception Memory FIFO Size

#define BGQ_MU_rDMA_FIFO_REGS_SIZE                  (4*8*8)     // 0x0100

// repeats 4 times i=0 to 3 per sub-group
#define BGQ_MU_rDMA_START_OFFSET(g,sg,i)            ( BGQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x00 + (i)*(8*8) )
#define BGQ_MU_rDMA_SIZE_OFFSET(g,sg,i)             ( BGQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x08 + (i)*(8*8) )
#define BGQ_MU_rDMA_HEAD_OFFSET(g,sg,i)             ( BGQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x10 + (i)*(8*8) ) 
#define BGQ_MU_rDMA_TAIL_OFFSET(g,sg,i)             ( BGQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x18 + (i)*(8*8) )

#define BGQ_MU_rDMA_HOLE1_OFFSET(g,sg,i)            ( GQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x20 + (i)*(8*8) )
#define BGQ_MU_rDMA_HOLE2_OFFSET(g,sg,i)            ( GQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x28 + (i)*(8*8) )
#define BGQ_MU_rDMA_HOLE3_OFFSET(g,sg,i)            ( GQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x30 + (i)*(8*8) )
#define BGQ_MU_rDMA_HOLE4_OFFSET(g,sg,i)            ( GQ_MU_rDMA_SUBGROUP_START_OFFSET(g,sg) + 0x38 + (i)*(8*8) )



// Injection Memory FIFO miscelaneos registers

#define BGQ_MU_STATUS_CONTROL_STARTING_OFFSET       (BGQ_MU_iDMA_FIFO_REGS_SIZE + BGQ_MU_rDMA_FIFO_REGS_SIZE)

#define BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg)    ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + BGQ_MU_STATUS_CONTROL_STARTING_OFFSET )

#define BGQ_MU_STATUS_CONTROL_REGS_SIZE                  (5*8)       // 0x0028


#define BGQ_MU_iDMA_IS_ENABLED_OFFSET(g,sg)              ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x000)
#define BGQ_MU_iDMA_ENABLE_OFFSET(g,sg)                  ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x008)
#define BGQ_MU_iDMA_DISABLE_OFFSET(g,sg)                 ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x010)
#define BGQ_MU_iDMA_HIGH_PRIORITY_OFFSET(g,sg)           ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x018)
#define BGQ_MU_READ_INT_STATUS_REGISTER_OFFSET(g,sg)     ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x020)
#define BGQ_MU_CLEAR_INT_STATUS_REGISTER_OFFSET(g,sg)    ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(g,sg) + 0x028)



// Data and Counter Base Address

#define BGQ_MU_DATA_COUNTER_BASE_ADDRESS_STARTING_OFFSET    0x0400

#define BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET(g,sg) ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + BGQ_MU_DATA_COUNTER_BASE_ADDRESS_STARTING_OFFSET )


#define BGQ_MU_COUNTER_REGS_SIZE                    (8*8)       // 0x0040


#define BGQ_MU_DATA_COUNTER_BASE_ADDRESS_OFFSET(g,sg,c)    ( BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET(g,sg) + (c)*8 )

// TODO: BARRIER

#define BGQ_MU_NUM_GI_CLASSES  16
#define BGQ_MU_GI_CONTROL_STARTING_OFFSET       0x0500
#define BGQ_MU_GI_STATUS_STARTING_OFFSET        0x0580
#define BGQ_MU_GI_STATUS_OFFSET(g,r)  ( BGQ_MU_GROUP_ADDRESS_OFFSET(g) + BGQ_MU_GI_STATUS_STARTING_OFFSET  + ( (r)*8) )
#define BGQ_MU_GI_CONTROL_OFFSET(g,r) ( BGQ_MU_GROUP_ADDRESS_OFFSET(g) + BGQ_MU_GI_CONTROL_STARTING_OFFSET + ( (r)*8) )



// TODO: This register is non-architected, has to be removed

#define BGQ_TRIGGER_rDMA_SCAN_STARTING_OFFSET    0x0600

#define BGQ_MU_TRIGGER_rDMA_SCAN_START_OFFSET(g,sg)         ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + BGQ_TRIGGER_rDMA_SCAN_STARTING_OFFSET )  

#define BGQ_TRIGGER_iDMA_SCAN_STARTING_OFFSET    0x0700

#define BGQ_MU_TRIGGER_iDMA_SCAN_START_OFFSET(g,sg)         ( BGQ_MU_SUBGROUP_ADDRESS_OFFSET(g,sg) + BGQ_TRIGGER_iDMA_SCAN_STARTING_OFFSET )  




// ------------------------------
// TODO: DCR mapped registers
// ------------------------------

#ifndef BGQ_MU_DCR_USE_NEW_MAPPING
 
#define BGQ_MU_DCR_MEM_MAPPED_START_OFFSET()                    (BGQ_MU_SUBGROUP_ADDRESS_OFFSET(BGQ_MU_NUM_FIFO_GROUPS,0))


#define BGQ_MU_DCR_iDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g)    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((g) * 8) )
#define BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(g)                   (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (17*8) + ((g) * 8) )
#define BGQ_MU_DCR_iDMA_HIGH_PRIORITY_FIFO_OFFSET(g)            (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (2*17*8) + ((g) * 8) )
#define BGQ_MU_DCR_iDMA_FIFO_AVAILABLE_REMOTE_GET_OFFSET(g)     (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (3*17*8) + ((g) * 8) )
#define BGQ_MU_DCR_iDMA_THRESHOLD_OFFSET()                      (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (4*17*8) )
#define BGQ_MU_DCR_iDMA_SYSTEM_INJECTION_NETWORK_FIFO_OFFSET()  (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (4*17*8) + (1 * 8) )

#define BGQ_MU_DCR_rDMA_ENABLED_OFFSET(g)                       (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (4*17*8) + (2 * 8) + ((g) * 8) )
#define BGQ_MU_DCR_rDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g)    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (5*17*8) + (2 * 8) + ((g) * 8) )
#define BGQ_MU_DCR_rDMA_PACKET_RECEIVED_INTERRUPT_ENABLE_OFFSET(g)  (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (6*17*8) + (2 * 8) + ((g) * 8) )
#define BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(g)                   (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (7*17*8) + (2 * 8) + ((g) * 8) )
#define BGQ_MU_DCR_rDMA_THRESHOLD_OFFSET()                      (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (8*17*8) + (2 * 8) )

#define BGQ_MU_DCR_MINIMUM_ADDRESS_OFFSET(g)                    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (8*17*8) + (3 * 8) + ((g) * 8) )
#define BGQ_MU_DCR_MAXIMUM_ADDRESS_OFFSET(g)                    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (9*17*8) + (3 * 8) + ((g) * 8) )

// --------------------------------------
// Class Route DCRs - leave a hole here
// --------------------------------------

#define BGQ_MU_DCR_CLASS_ROUTE_OFFSET(_id_)                     (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (10*17*8) + (3 * 8) + ((_id_) * 8) )

#else
// Uses new address for MU DCRs based on mu_dcr.h


//#define BGQ_MU_GROUP_ADDRESS_OFFSET(g)              _BGQ_BASE_DMA + ((g)*(BGQ_MU_GROUP_SIZE))

// ---------------------------------------------------------
// Base address for DCR  0x3fde0000000 - privileged access
// ---------------------------------------------------------

// ------------------------------------------------
// MU DCR registers - new definition
// ------------------------------------------------

#define BGQ_MU_DCR_MEM_MAPPED_START_OFFSET()                    (PHYMAP_MINADDR_DCR + PHYMAP_PRIVILEGEDOFFSET)


#define BGQ_MU_DCR_iDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g)    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMFIFO_THOLD_INT_EN)+(g)) * 8) )
#define BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(g)                   (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMFIFO_SYSTEM)+(g)) * 8) )
#define BGQ_MU_DCR_iDMA_FIFO_AVAILABLE_REMOTE_GET_OFFSET(g)     (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMFIFO_RGET)+(g)) * 8) )
#define BGQ_MU_DCR_iDMA_THRESHOLD_OFFSET                        (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMU_THOLD)) * 8) )
#define BGQ_MU_DCR_iDMA_RGET_THRESHOLD_OFFSET                   (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RGET_THOLD)) * 8) )

//#define BGQ_MU_DCR_iDMA_SYSTEM_INJECTION_NETWORK_FIFO_OFFSET()  (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (4*17*8) + (1 * 8) )

#define BGQ_MU_DCR_rDMA_ENABLED_OFFSET(g)                       (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMFIFO_EN)+(g)) * 8) )
#define BGQ_MU_DCR_rDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(g)    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMFIFO_THOLD_INT_EN)+(g)) * 8) )
#define BGQ_MU_DCR_rDMA_PACKET_RECEIVED_INTERRUPT_ENABLE_OFFSET(g)  (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMFIFO_PACKET_INT_EN)+(g)) * 8) )
#define BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(g)                   (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMFIFO_SYSTEM)+(g)) * 8) )
#define BGQ_MU_DCR_rDMA_THRESHOLD_OFFSET                        (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMU_THOLD)) * 8) )

// GEA DCRs
#define BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_0_7_OFFSET()            (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (MU_DCR(GIP_INT_EN_0_7) * 8) )
#define BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_8_15_OFFSET()           (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (MU_DCR(GIP_INT_EN_8_15) * 8) )
#define BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_16_OFFSET()             (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (MU_DCR(GIP_INT_EN_16) * 8) )

// New definition
#define BGQ_MU_DCR_rDMA_RPUTBASE_SYSTEM_FIFO_OFFSET(g)          (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RPUTBASE_SYSTEM)+(g)) * 8) )


//#define BGQ_MU_DCR_MINIMUM_ADDRESS_OFFSET(g)                    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (8*17*8) + (3 * 8) + ((g) * 8) )
//#define BGQ_MU_DCR_MAXIMUM_ADDRESS_OFFSET(g)                    (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (9*17*8) + (3 * 8) + ((g) * 8) )

// New registers
#define BGQ_MU_DCR_iDMA_MINIMUM_ADDRESS_OFFSET(g)               (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMIN_ADDR_RANGE)+(g)) * 8) )
#define BGQ_MU_DCR_iDMA_MAXIMUM_ADDRESS_OFFSET(g)               (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(IMAX_ADDR_RANGE)+(g)) * 8) )

// New registers
#define BGQ_MU_DCR_rDMA_MINIMUM_ADDRESS_OFFSET(g)               (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMIN_ADDR_RANGE)+(g)) * 8) )
#define BGQ_MU_DCR_rDMA_MAXIMUM_ADDRESS_OFFSET(g)               (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ((MU_DCR(RMAX_ADDR_RANGE)+(g)) * 8) )


// --------------------------------------
// Class Route DCRs
// --------------------------------------

#define BGQ_MU_DCR_CLASS_ROUTE_OFFSET(_id_)                     (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + ( (ND_500_DCR(CTRL_COLL_CLASS_00_01) + ((_id_) >> 1)) * 8) )

// Base Address Table DCRs


#endif /* #ifndef BGQ_MU_DCR_USE_NEW_MAPPING */
#define BGQ_MU_DCR_BAT_MODE0_OFFSET(g)                           (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (MU_DCR(RPUTBASE_SYSTEM0) * 8) )
#define BGQ_MU_DCR_BAT_MODE1_OFFSET(g)                           (BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() + (MU_DCR(RPUTBASE_SYSTEM1) * 8) )


#endif  /* _MU_MACROS_H_ */
