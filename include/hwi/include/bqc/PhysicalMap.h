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

#ifndef _BQC_PHYSICALMAP_H_ // Prevent multiple inclusion
#define _BQC_PHYSICALMAP_H_

#ifndef BEDRAM_VERSION
//#define BEDRAM_VERSION 16     // BeDRAM_HLD_1.6.doc
#define BEDRAM_VERSION 17   // BeDRAM_HLD_1.7.doc
#endif

#if defined(__ASSEMBLY__)
    #define PHYMAP_CONST64(x) x
#else
    #define PHYMAP_CONST64(x) x ## LL
#endif

#define PHYMAP_MINADDR_MAINMEMORYACCESS PHYMAP_CONST64(0x00000000000)
#define PHYMAP_MAXADDR_MAINMEMORYACCESS PHYMAP_CONST64(0x2ffffffffff)
#define PHYMAP_SIZE_MAINMEMORYACCESS    (PHYMAP_MAXADDR_MAINMEMORYACCESS - PHYMAP_MINADDR_MAINMEMORYACCESS+1)
#define PHYMAP_MASK_MAINMEMORYACCESS    (PHYMAP_MAXADDR_MAINMEMORYACCESS ^ PHYMAP_MINADDR_MAINMEMORYACCESS)

#define PHYMAP_MINADDR_MAINMEMORY PHYMAP_CONST64(0x00000000000)
#define PHYMAP_MAXADDR_MAINMEMORY PHYMAP_CONST64(0x00fffffffff)
#define PHYMAP_SIZE_MAINMEMORY    (PHYMAP_MAXADDR_MAINMEMORY - PHYMAP_MINADDR_MAINMEMORY+1)
#define PHYMAP_MASK_MAINMEMORY    (PHYMAP_MAXADDR_MAINMEMORY ^ PHYMAP_MINADDR_MAINMEMORY)

#define PHYMAP_MINADDR_TLS1 PHYMAP_CONST64(0x01000000000)
#define PHYMAP_MAXADDR_TLS1 PHYMAP_CONST64(0x01fffffffff)
#define PHYMAP_SIZE_TLS1    (PHYMAP_MAXADDR_TLS1 - PHYMAP_MINADDR_TLS1+1)
#define PHYMAP_MASK_TLS1    (PHYMAP_MAXADDR_TLS1 ^ PHYMAP_MINADDR_TLS1)

#define PHYMAP_MINADDR_TLS2 PHYMAP_CONST64(0x02000000000)
#define PHYMAP_MAXADDR_TLS2 PHYMAP_CONST64(0x02fffffffff)
#define PHYMAP_SIZE_TLS2    (PHYMAP_MAXADDR_TLS2 - PHYMAP_MINADDR_TLS2+1)
#define PHYMAP_MASK_TLS2    (PHYMAP_MAXADDR_TLS2 ^ PHYMAP_MINADDR_TLS2)

#define PHYMAP_MINADDR_TLS3 PHYMAP_CONST64(0x03000000000)
#define PHYMAP_MAXADDR_TLS3 PHYMAP_CONST64(0x03fffffffff)
#define PHYMAP_SIZE_TLS3    (PHYMAP_MAXADDR_TLS3 - PHYMAP_MINADDR_TLS3+1)
#define PHYMAP_MASK_TLS3    (PHYMAP_MAXADDR_TLS3 ^ PHYMAP_MINADDR_TLS3)

#define PHYMAP_MINADDR_ATOMICOPS PHYMAP_CONST64(0x04000000000)
#define PHYMAP_MAXADDR_ATOMICOPS PHYMAP_CONST64(0x0ffffffffff)
#define PHYMAP_SIZE_ATOMICOPS    (PHYMAP_MAXADDR_ATOMICOPS - PHYMAP_MINADDR_ATOMICOPS+1)
#define PHYMAP_MASK_ATOMICOPS    (PHYMAP_MAXADDR_ATOMICOPS ^ PHYMAP_MINADDR_ATOMICOPS)

#define PHYMAP_MINADDR_CATOMICOPS PHYMAP_CONST64(0x10000000000)
#define PHYMAP_MAXADDR_CATOMICOPS PHYMAP_CONST64(0x2ffffffffff)
#define PHYMAP_SIZE_CATOMICOPS    (PHYMAP_MAXADDR_CATOMICOPS - PHYMAP_MINADDR_CATOMICOPS+1)
#define PHYMAP_MASK_CATOMICOPS    (PHYMAP_MAXADDR_CATOMICOPS ^ PHYMAP_MINADDR_CATOMICOPS)

#define PHYMAP_MINADDR_PCIE PHYMAP_CONST64(0x3fc00000000)
#define PHYMAP_MAXADDR_PCIE PHYMAP_CONST64(0x3fcffffffff)
#define PHYMAP_SIZE_PCIE    (PHYMAP_MAXADDR_PCIE - PHYMAP_MINADDR_PCIE+1)
#define PHYMAP_MASK_PCIE    (PHYMAP_MAXADDR_PCIE ^ PHYMAP_MINADDR_PCIE)

#define PHYMAP_MINADDR_MMIO PHYMAP_CONST64(0x3fdc0000000)
#define PHYMAP_MAXADDR_MMIO PHYMAP_CONST64(0x3fdffffffff)
#define PHYMAP_SIZE_MMIO   (PHYMAP_MAXADDR_MMIO - PHYMAP_MINADDR_MMIO+1)
#define PHYMAP_MASK_MMIO    (PHYMAP_MAXADDR_MMIO ^ PHYMAP_MINADDR_MMIO)

#define PHYMAP_MINADDR_DCR PHYMAP_CONST64(0x3fde0000000)
#define PHYMAP_MAXADDR_DCR PHYMAP_CONST64(0x3fde7ffffff)
#define PHYMAP_SIZE_DCR    (PHYMAP_MAXADDR_DCR - PHYMAP_MINADDR_DCR+1)
#define PHYMAP_MASK_DCR    (PHYMAP_MAXADDR_DCR ^ PHYMAP_MINADDR_DCR)

#define PHYMAP_MINADDR_L1P PHYMAP_CONST64(0x3fde8000000)
#define PHYMAP_MAXADDR_L1P PHYMAP_CONST64(0x3fde800ffff)
#define PHYMAP_SIZE_L1P    (PHYMAP_MAXADDR_L1P - PHYMAP_MINADDR_L1P+1)
#define PHYMAP_MASK_L1P    (PHYMAP_MAXADDR_L1P ^ PHYMAP_MINADDR_L1P)

#define PHYMAP_MINADDR_PCICFG PHYMAP_CONST64(0x3fdfd000000)
#define PHYMAP_MAXADDR_PCICFG PHYMAP_CONST64(0x3fdfd7fffff)
#define PHYMAP_SIZE_PCICFG    (PHYMAP_MAXADDR_PCICFG - PHYMAP_MINADDR_PCICFG+1)
#define PHYMAP_MASK_PCICFG    (PHYMAP_MAXADDR_PCICFG ^ PHYMAP_MINADDR_PCICFG)

#define PHYMAP_MINADDR_PCIIO PHYMAP_CONST64(0x3fdfd800000)
#define PHYMAP_MAXADDR_PCIIO PHYMAP_CONST64(0x3fdfdbfffff)
#define PHYMAP_SIZE_PCIIO    (PHYMAP_MAXADDR_PCIIO - PHYMAP_MINADDR_PCIIO+1)
#define PHYMAP_MASK_PCIIO    (PHYMAP_MAXADDR_PCIIO ^ PHYMAP_MINADDR_PCIIO)

#define PHYMAP_MINADDR_PCIINT PHYMAP_CONST64(0x3fdfdc00000)
#define PHYMAP_MAXADDR_PCIINT PHYMAP_CONST64(0x3fdfdffffff)
#define PHYMAP_SIZE_PCIINT    (PHYMAP_MAXADDR_PCIINT - PHYMAP_MINADDR_PCIINT+1)
#define PHYMAP_MASK_PCIINT    (PHYMAP_MAXADDR_PCIINT ^ PHYMAP_MINADDR_PCIINT)

#define PHYMAP_MINADDR_MU  PHYMAP_CONST64(0x3fdfe000000)
#define PHYMAP_MAXADDR_MU  PHYMAP_CONST64(0x3fdfe1fffff)
#define PHYMAP_SIZE_MU    (PHYMAP_MAXADDR_MU - PHYMAP_MINADDR_MU+1)
#define PHYMAP_MASK_MU    (PHYMAP_MAXADDR_MU ^ PHYMAP_MINADDR_MU)

#define PHYMAP_MINADDR_L2  PHYMAP_CONST64(0x3fdfe200000)
#define PHYMAP_MAXADDR_L2  PHYMAP_CONST64(0x3fdfe2fffff)
#define PHYMAP_SIZE_L2    (PHYMAP_MAXADDR_L2 - PHYMAP_MINADDR_L2+1)
#define PHYMAP_MASK_L2    (PHYMAP_MAXADDR_L2 ^ PHYMAP_MINADDR_L2)

#define PHYMAP_MINADDR_UPC PHYMAP_CONST64(0x3fdfe400000)
#define PHYMAP_MAXADDR_UPC PHYMAP_CONST64(0x3fdfe40ffff)
#define PHYMAP_SIZE_UPC    (PHYMAP_MAXADDR_UPC - PHYMAP_MINADDR_UPC+1)
#define PHYMAP_MASK_UPC    (PHYMAP_MAXADDR_UPC ^ PHYMAP_MINADDR_UPC)

#define PHYMAP_MINADDR_INTAG PHYMAP_CONST64(0x3fdfe410000)
#define PHYMAP_MAXADDR_INTAG PHYMAP_CONST64(0x3fdfe41ffff)
#define PHYMAP_SIZE_INTAG    (PHYMAP_MAXADDR_INTAG - PHYMAP_MINADDR_INTAG+1)
#define PHYMAP_MASK_INTAG    (PHYMAP_MAXADDR_INTAG ^ PHYMAP_MINADDR_INTAG)

#if (BEDRAM_VERSION==16)
    #define PHYMAP_MINADDR_BEDRAM_ATOMICOPS PHYMAP_CONST64(0x3fdffc00000)
    #define PHYMAP_MAXADDR_BEDRAM_ATOMICOPS PHYMAP_CONST64(0x3fdffdfffff)
#else
    #define PHYMAP_MINADDR_BEDRAM_ATOMICOPS PHYMAP_CONST64(0x3fdff000000)
    #define PHYMAP_MAXADDR_BEDRAM_ATOMICOPS PHYMAP_CONST64(0x3fdff7fffff)
#endif
#define PHYMAP_SIZE_BEDRAM_ATOMICOPS    (PHYMAP_MAXADDR_BEDRAM_ATOMICOPS - PHYMAP_MINADDR_BEDRAM_ATOMICOPS+1)
#define PHYMAP_MASK_BEDRAM_ATOMICOPS    (PHYMAP_MAXADDR_BEDRAM_ATOMICOPS ^ PHYMAP_MINADDR_BEDRAM_ATOMICOPS)

#define PHYMAP_MINADDR_BEDRAM PHYMAP_CONST64(0x3fdfffc0000)
#define PHYMAP_MAXADDR_BEDRAM PHYMAP_CONST64(0x3fdffffffff)
#define PHYMAP_SIZE_BEDRAM   (PHYMAP_MAXADDR_BEDRAM - PHYMAP_MINADDR_BEDRAM+1)
#define PHYMAP_MASK_BEDRAM   (PHYMAP_MAXADDR_BEDRAM ^ PHYMAP_MINADDR_BEDRAM)

#define PHYMAP_PRIVILEGEDOFFSET PHYMAP_CONST64(0x00200000000)

//
//  Physical Memory Map as of 7/24/2008.
//
//  L1P: A2 accessible via L1P, includes DBM and DBS
//  DBM: accessible via DEVBUS master, includes DBS
//  DBS: accessible via DEVBUS slave
//
//
//  DBM   0x000_0000_0000 - 0x2ff_ffff_ffff:   main memory access
//  DBM   0x000_0000_0000 - 0x00f_ffff_ffff:   main memory, 64GB
//  DBM   0x010_0000_0000 - 0x03f_ffff_ffff:   3 mirrored copies of above, used for aliasing 4 threads in TLS, only L1-cached accesses
//  DBM   0x100_0000_0000 - 0x2ff_ffff_ffff:   atomic ops for main memory, 8*4*64GB base space, non-cached access only, opcode bits: 58-60, alias bits: 56-57
//
//      0x300_0000_0000 - 0x3ff_ffff_ffff:   memory mapped I/O, boot eDRAM space
//        0x300_0000_0000 - 0x3fb_ffff_ffff:   mirrors of the I/O address spaces below
//
//        0x3fc_0000_0000 - 0x3fd_ffff_ffff:   non-privileged I/O space
//  DBS     0x3fc_0000_0000 - 0x3fc_ffff_ffff:   PCIe device memory space, 4GB
//          0x3fd_0000_0000 - 0x3fd_ffff_ffff:   other I/O
//  L1P       0x3fd_e000_0000 - 0x3fd_e7ff_ffff:   DCR space, 16M*8B==128MB
//  L1P       0x3fd_e800_0000 - 0x3fd_e800_ffff:   L1P-local I/O, 64KB
//  DBM       0x3fd_fe80_0000 - 0x3fd_fe80_ffff:   MSG unit
//  DBS       0x3fd_fe81_0000 - 0x3fd_fe81_ffff:   Network Unit
//  DBS       0x3fd_fe82_0000 - 0x3fd_fe82_ffff:   L2 control, 1KB
//  DBS       0x3fd_fe83_0000 - 0x3fd_fe83_ffff:   Performance counter
//  DBS       0x3fd_fe84_0000 - 0x3fd_fe84_ffff:   Interrupt aggregator
//  DBS       0x3fd_fe85_0000 - 0x3fd_fe85_ffff:   PCIe configuration
//  DBS       0x3fd_fe86_0000 - 0x3fd_fe86_ffff:   Flash controller
//  DBS       0x3fd_fe87_0000 - 0x3fd_fe87_ffff:   PCIe device I/O space, 8MB
//  DBS       0x3fd_ff00_0000 - 0x3fd_ff7f_ffff:   atomic ops for boot eDRAM, 4*8*256KB, non-cached access only, opcode in bits 58-60.
//  DBS       0x3fd_fffc_0000 - 0x3fd_ffff_ffff:   boot eDRAM, 256KB
//
//        0x3fe_0000_0000 - 0x3ff_ffff_ffff:   privileged mirror of above I/O space
//  DBS     0x3fe_0000_0000 - 0x3fc_ffff_ffff:   PCIe device memory space, 4GB
//          0x3ff_0000_0000 - 0x3ff_ffff_ffff:   other I/O
//  L1P       0x3ff_e000_0000 - 0x3ff_e7ff_ffff:   DCR space, 16M*8B==128MB
//  L1P       0x3ff_e800_0000 - 0x3ff_e800_ffff:   L1P-local I/O, 64KB
//  DBM       0x3ff_fe80_0000 - 0x3ff_fe80_ffff:   MSG unit
//  DBS       0x3ff_fe81_0000 - 0x3ff_fe81_ffff:   Network Unit
//  DBS       0x3ff_fe82_0000 - 0x3ff_fe82_ffff:   L2 control, 1KB
//  DBS       0x3ff_fe83_0000 - 0x3ff_fe83_ffff:   Performance counter
//  DBS       0x3ff_fe84_0000 - 0x3ff_fe84_ffff:   Interrupt aggregator
//  DBS       0x3ff_fe85_0000 - 0x3ff_fe85_ffff:   PCIe configuration
//  DBS       0x3ff_fe86_0000 - 0x3ff_fe86_ffff:   Flash controller
//  DBS       0x3ff_fe87_0000 - 0x3ff_fe87_ffff:   PCIe device I/O space, 8MB
//  DBS       0x3ff_ff00_0000 - 0x3ff_ff7f_ffff:   atomic ops for boot eDRAM, 4*8*256KB, non-cached access only, opcode in bits 58-60.
//  DBS       0x3ff_fffc_0000 - 0x3ff_ffff_ffff:   boot eDRAM, 256KB
//


//
//
//  semantics of address bits, assuming physical address vector addr(22 to 63):
//
//  0x20000000000 addr(22):       MMIO select if also addr(23) is set
//
//                L1-inhibited:   compressed atomic main memory op select
//  0x1ffffffff00 addr(23 to 55): 8B counter select for compressed atomic ops
//  0x000000000c0 addr(56 to 57): aliasing for LMQ serialization avoidance for compressed atomic ops
//  0x00000000038 addr(58 to 60): main memory compressed atomic op select
//
//  0x0c000000000 addr(24 to 25): non-compressed atomic OP select, subset of 3 operations ("00": regular access)
//  0x03000000000 addr(26 to 27): TLS thread aliasing support, evaluted in L1P for address match disambiguation and inside L2 for thread disambiguation
//  0x00fffffffff addr(28 to 63): main memory location
//
//  0x00200000000 addr(30):       privileged I/O select, passed on to I/O device
//  0x00100000000 addr(31):       PCIe specifier/ other I/O
//  0x000ffffffff addr(32 to 63): PCIe location
//  0x00010000000 addr(35):       local I/O,DCR vs crossbar I/O
//  0x00008000000 addr(36):       local I/O vs DCR specifier
//  0x00007fffff8 addr(37 to 60): DCR index
//
//  0x00000800000 addr(40):       0: bedram atomic operations select, 1: bedram memory access
//  0x000007fff00 addr(41 to 55): bedram counter select
//  0x000000000c0 addr(56 to 57): aliasing for LMQ serialization avoidance for atomic ops
//  0x00000000038 addr(58 to 60): bedram atomic op select
//  0x0000003ffff addr(46 to 63): bedram memory address
//
//
//  TLB attribute U0-U3 semantics:
//  U0: 0: the local thread register controls whether an access via this TLB entry is speculative or non-speculative
//      1: all accesses via this TLB entry are non-speculative
//  U1: guarded access, 0: write combining enabled, 1: write combining disabled, store flushes all requests to switch
//  U2: not defined
//  U3: not defined
//
//
//  List of DCR slave base addresses
//
//  DCR          physical MMIO
//  index        address           Unit          Name
//
//  0x000000     0x3fde0000000     PU 0          PU00_Base
//  0x004000     0x3fde0020000     PU 1          PU01_Base
//  0x008000     0x3fde0040000     PU 2          PU02_Base
//  0x00c000     0x3fde0060000     PU 3          PU03_Base
//  0x010000     0x3fde0080000     PU 4          PU04_Base
//  0x014000     0x3fde00a0000     PU 5          PU05_Base
//  0x018000     0x3fde00b0000     PU 6          PU06_Base
//  0x01c000     0x3fde00c0000     PU 7          PU07_Base
//  0x020000     0x3fde0100000     PU 8          PU08_Base
//  0x024000     0x3fde0120000     PU 9          PU09_Base
//  0x028000     0x3fde0140000     PU 10         PU10_Base
//  0x02c000     0x3fde0160000     PU 11         PU11_Base
//  0x030000     0x3fde0180000     PU 12         PU12_Base
//  0x034000     0x3fde01a0000     PU 13         PU13_Base
//  0x038000     0x3fde01b0000     PU 14         PU14_Base
//  0x03c000     0x3fde01c0000     PU 15         PU15_Base
//  0x040000     0x3fde0200000     PU 16         PU16_Base
//
//  0x060000     0x3fde0300000     L2 Counter 0  L2CTR0_Base
//  0x064000     0x3fde0320000     L2 Counter 1  L2CTR1_Base
//  0x068000     0x3fde0340000     L2 Counter 2  L2CTR2_Base
//  0x06c000     0x3fde0360000     L2 Counter 3  L2CTR3_Base
//
//  0x070000     0x3fde0380000     L2 Central    L2CTRL_Base
//
//  0x078000     0x3fde03c0000     DDR 0         DDR0_Base
//  0x07c000     0x3fde03e0000     DDR 1         DDR1_Base
//
//  0x080000     0x3fde0400000     L2 0          L200_Base
//  0x084000     0x3fde0420000     L2 1          L201_Base
//  0x088000     0x3fde0440000     L2 2          L202_Base
//  0x08c000     0x3fde0460000     L2 3          L203_Base
//  0x090000     0x3fde0480000     L2 4          L204_Base
//  0x094000     0x3fde04a0000     L2 5          L205_Base
//  0x098000     0x3fde04b0000     L2 6          L206_Base
//  0x09c000     0x3fde04c0000     L2 7          L207_Base
//  0x0a0000     0x3fde0500000     L2 8          L208_Base
//  0x0a4000     0x3fde0520000     L2 9          L209_Base
//  0x0a8000     0x3fde0540000     L2 10         L210_Base
//  0x0ac000     0x3fde0560000     L2 11         L211_Base
//  0x0b0000     0x3fde0580000     L2 12         L212_Base
//  0x0b4000     0x3fde05a0000     L2 13         L213_Base
//  0x0b8000     0x3fde05b0000     L2 14         L214_Base
//  0x0bc000     0x3fde05c0000     L2 15         L215_Base
//
//  0x0c0000     0x3fde0600000     MU            MU_Base
//  0x0c4000     0x3fde0620000     NET           NET_Base
//  0x0c8000     0x3fde0640000     TESTINT       TI_Base
//  0x0cc000     0x3fde0660000     DEVBUS        DB_Base
//  0x0d0000     0x3fde0680000     UPC           UPC_Base
//  0x0d4000     0x3fde06a0000     MSYNC         MS_Base
//
//

#endif // Add nothing below this line.
