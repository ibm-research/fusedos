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
#ifndef _A2_CORE_H_ // Prevent multiple inclusion
#define _A2_CORE_H_

#ifndef __UPCFS__                                                                                                      // FUSEDOS
//
// As of A2_um.pdf Version 0.83 March 19, 2008 (up to pg 101
//
// Special Purposed Registers:
//  4 Categories: Privileged vs User Access, and Per-Core vs Per-Thread
//
// 07/13/2009 Change CCR1 bits meaning for wake control according to a2_um_ver_94
//
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/common/bgq_bitnumbers.h>

__BEGIN_DECLS

#if defined(__KERNEL__)

//
// Interrupt Vector Offsets: Vector is (G)IVPR[0..51] || 12-bit Interrupt Offset
//

#define IVO_MCHK                 (0x000)           // Machine Check
#define IVO_CI                   (0x020)           // Critical Input
#define IVO_DEBUG                (0x040)           // Debug
#define IVO_DSI                  (0x060)           // Data Storage Interrupt
#define IVO_ISI                  (0x080)           // Instruction Storage Interrupt
#define IVO_EI                   (0x0A0)           // External Input
#define IVO_ALGN                 (0x0C0)           // Alignment
#define IVO_PROG                 (0x0E0)           // Program
#define IVO_FPU                  (0x100)           // Floating Point Unavailable
#define IVO_SC                   (0x120)           // System Call
#define IVO_APU                  (0x140)           // Auxillary Processo Unavailable
#define IVO_DEC                  (0x160)           // Decrementer
#define IVO_FIT                  (0x180)           // Fixed Interval Timer
#define IVO_WDT                  (0x1A0)           // WatchDog Timer
#define IVO_DTLB                 (0x1C0)           // Data TLB Error
#define IVO_ITLB                 (0x1E0)           // Instruction TLB Error
#define IVO_VECTOR               (0x200)           // Vector Unavailable Interrupt
#define IVO_PDBI                 (0x280)           // Processor DoorBell Interrupt
#define IVO_PDBCI                (0x2A0)           // Processor DoorBell Critical Interrupt
#define IVO_GDBI                 (0x2C0)           // Guest DoorBell Interrupt
#define IVO_GDBCI                (0x2E0)           // Guest DoorBell Critical or Machine Check Interrupt
#define IVO_EHVSC                (0x300)           // Embedded Hypervisor System Call
#define IVO_EHVPRIV              (0x320)           // Embedded Hypervisor Privilege
#define IVO_LRATE                (0x340)           // LRAT Error Interrupt
#define IVO_UDEC                 (0x800)           // User Decrementer
#define IVO_PERFMON              (0x820)           // Performance Monitor
// All other 32-bit aligned offsets up to 0xFE0 are reserved
#define IVO_UNDEFINED            (0xFFF)

//
// 64bit Mode ERAT(we|re|sx) Word Definitions
//
#define ERAT0_EPN(x)      ((x) & ~0xFFF)           //! Note:  this is not a page index, this is the effective address
#define ERAT0_Class(x)    _B2(53,x)
#define ERAT0_V           _BN(54)                  // Valid
#define ERAT0_X           _BN(55)                  // Exclusion Range Enable
#define ERAT0_Size(x)     _B4(59,x)                // Size:
#define  ERAT0_Size_4K     ERAT0_Size( 1)
#define  ERAT0_Size_64K    ERAT0_Size( 3)
#define  ERAT0_Size_1M     ERAT0_Size( 5)
#define  ERAT0_Size_16M    ERAT0_Size( 7)
#define  ERAT0_Size_1G     ERAT0_Size(10)
#define ERAT0_ThdID(x)    _B4(63,x)                // Thread ID bitmask

#ifndef __ASSEMBLY__
#define ERAT0_GET_EPN(x)     (((x) & 0xFFFFFFFFFFFFF000ul) >> 12)
#define ERAT0_GET_CLASS(x)   _G2(x,53)
#define ERAT0_GET_V(x)       _G1(x,54)
#define ERAT0_GET_X(x)       _G1(x,55)
#define ERAT0_GET_SIZE(x)    _G4(x,59)
#define ERAT0_GET_ThdID(x)   _G4(x,63)
#endif


#define ERAT1_RPN(x)      ((x) & 0x000003FFFFFFF000)  //! Note:  this is not a page index, this is the real address
#define ERAT1_U0          _BN(12)
#define ERAT1_U1          _BN(13)
#define ERAT1_U2          _BN(14)
#define ERAT1_U3          _BN(15)
#define ERAT1_R           _BN(16)                  // Referenced
#define ERAT1_C           _BN(17)                  // Changed
#define ERAT1_W           _BN(52)                  // Write-Thru
#define ERAT1_I           _BN(53)                  // Inhibit
#define ERAT1_M           _BN(54)                  // Memory Coherence Required
#define ERAT1_G           _BN(55)                  // Guarded
#define ERAT1_E           _BN(56)                  // Endian
#define ERAT1_VF          _BN(57)                  // Virtualization Fault (set to zero)
#define ERAT1_UX          _BN(58)                  // User Execute
#define ERAT1_SX          _BN(59)                  // Priv Execute
#define ERAT1_UW          _BN(60)                  // User Write
#define ERAT1_SW          _BN(61)                  // Priv Write
#define ERAT1_UR          _BN(62)                  // User Read
#define ERAT1_SR          _BN(63)                  // Priv Read

#ifndef __ASSEMBLY__
#define ERAT1_GET_RPN(x)  (((x) & 0x000003FFFFFFF000) >> 12)
#define ERAT1_GET_U0(x)   _G1(x,12)
#define ERAT1_GET_U1(x)   _G1(x,13)
#define ERAT1_GET_U2(x)   _G1(x,14)
#define ERAT1_GET_U3(x)   _G1(x,15)
#define ERAT1_GET_R(x)    _G1(x,16)
#define ERAT1_GET_C(x)    _G1(x,17)
#define ERAT1_GET_W(x)    _G1(x,52)
#define ERAT1_GET_I(x)    _G1(x,53)
#define ERAT1_GET_M(x)    _G1(x,54)
#define ERAT1_GET_G(x)    _G1(x,55)
#define ERAT1_GET_E(x)    _G1(x,56)
#define ERAT1_GET_VF(x)   _G1(x,57)
#define ERAT1_GET_UX(x)   _G1(x,58)
#define ERAT1_GET_UW(x)   _G1(x,59)
#define ERAT1_GET_UR(x)   _G1(x,60)
#define ERAT1_GET_SX(x)   _G1(x,61)
#define ERAT1_GET_SW(x)   _G1(x,62)
#define ERAT1_GET_SR(x)   _G1(x,63)
#endif

//
// Per-Core SPRs: Hypervisor Read/Write (unless otherwise noted)
//
#define SPRN_CCR0                 (0x3F0)          // Core Configuration Register 0
#define  CCR0_PME(x)               _B2(33,x)       //  Power Management Enable
#define   CCR0_PME_Disabled         CCR0_PME(0)
#define   CCR0_PME_Sleep            CCR0_PME(1)
#define   CCR0_PME_RVW              CCR0_PME(2)
#define  CCR0_WEM(x)               _B4(55,x)       //  Wait Enable Mask (R/O)
#define  CCR0_WE(x)                _B4(63,x)       //  Wait Enable (R/O)

#define SPRN_CCR1                 (0x3F1)          // Core Configuration Register 1
#define  CCR1_WC3_RSV_DIS          _BN(34)         //  Thread 3: Disables sleep on waitrsv
#define  CCR1_WC3_PIN_DIS          _BN(35)         //         3: Disables sleep on waitimpl (wake on falling edge of an_ac_sleep_en)
#define  CCR1_WC3_CI_EN            _BN(36)         //         3: Enables wake on Critical Input
#define  CCR1_WC3_EI_EN            _BN(37)         //         3: Enables wake on External Input
#define  CCR1_WC3_DEC_EN           _BN(38)         //         3: Enables wake on Decrementer
#define  CCR1_WC3_FIT_EN           _BN(39)         //         3: Enables wake on Fixed Interval Timer

#define  CCR1_WC2_RSV_DIS          _BN(42)         //  Thread 2: Disables sleep on waitrsv
#define  CCR1_WC2_PIN_DIS          _BN(43)         //         2: Disables sleep on waitimpl (wake on falling edge of an_ac_sleep_en)
#define  CCR1_WC2_CI_EN            _BN(44)         //         2: Enables wake on Critical Input
#define  CCR1_WC2_EI_EN            _BN(45)         //         2: Enables wake on External Input
#define  CCR1_WC2_DEC_EN           _BN(46)         //         2: Enables wake on Decrementer
#define  CCR1_WC2_FIT_EN           _BN(47)         //         2: Enables wake on Fixed Interval Timer

#define  CCR1_WC1_RSV_DIS          _BN(50)         //  Thread 1: Disables sleep on waitrsv
#define  CCR1_WC1_PIN_DIS          _BN(51)         //         1: Disables sleep on waitimpl (wake on falling edge of an_ac_sleep_en)
#define  CCR1_WC1_CI_EN            _BN(52)         //         1: Enables wake on Critical Input
#define  CCR1_WC1_EI_EN            _BN(53)         //         1: Enables wake on External Input
#define  CCR1_WC1_DEC_EN           _BN(54)         //         1: Enables wake on Decrementer
#define  CCR1_WC1_FIT_EN           _BN(55)         //         1: Enables wake on Fixed Interval Timer

#define  CCR1_WC0_RSV_DIS          _BN(58)         //  Thread 0: Disables sleep on waitrsv
#define  CCR1_WC0_PIN_DIS          _BN(59)         //         0: Disables sleep on waitimpl (wake on falling edge of an_ac_sleep_en)
#define  CCR1_WC0_CI_EN            _BN(60)         //         0: Enables wake on Critical Input
#define  CCR1_WC0_EI_EN            _BN(61)         //         0: Enables wake on External Input
#define  CCR1_WC0_DEC_EN           _BN(62)         //         0: Enables wake on Decrementer
#define  CCR1_WC0_FIT_EN           _BN(63)         //         0: Enables wake on Fixed Interval Timer

#define SPRN_CCR2                 (0x3F2)          // Core Configuration Register 2
#define  CCR2_FRATSC_WT            _BN(49)         //  FRATSC: Force Write-Through
#define  CCR2_FRATSC_I             _BN(50)         //  FRATSC: Force Cache-Inhibited
#define  CCR2_FRATSC_M             _BN(51)         //  FRATSC: Force Memory Coherence Required
#define  CCR2_FRATSC_G             _BN(52)         //  FRATSC: Force Guarded
#define  CCR2_FRATSC_BE            _BN(53)         //  FRATSC: Force Big-Endian
#define  CCR2_FRAT                 _BN(54)         //  Force Real Address Translation: 0:Use Erats, 1:EA==RA
#define  CCR2_UCODE_DIS            _BN(55)         //  Disable microcoded instructions
#define  CCR2_AP0_EN               _BN(56)         //  Enable Auxillary Processor for Thread 0
#define  CCR2_AP1_EN               _BN(57)         //  Enable Auxillary Processor for Thread 1
#define  CCR2_AP2_EN               _BN(58)         //  Enable Auxillary Processor for Thread 2
#define  CCR2_AP3_EN               _BN(59)         //  Enable Auxillary Processor for Thread 3
#define  CCR2_ATTN_EN              _BN(60)         //  Enable Attn Instruction
#define  CCR2_CCR0UM_EN            _BN(62)         //  Enable User Mode Access to CCR0
#define  CCR2_NOTLB                _BN(63)         //  Disable TLB, Enable ERAT-only mode

#define SPRN_DAC1                 (0x13C)          // Data Address Compare 1
#define SPRN_DAC2                 (0x13D)          // Data Address Compare 2
#define SPRN_DAC3                 (0x351)          // Data Address Compare 3
#define SPRN_DAC4                 (0x352)          // Data Address Compare 4

#define SPRN_DVC1                 (0x13E)          // Data Value Compare 1
#define SPRN_DVC2                 (0x13F)          // Data Value Compare 2

#define SPRN_GIVPR                (  447)          // Guest Interrupt Vector Prefix Register (upper 52bits defined)
#define  GIVPR_MASK                (0xFFFFFFFFFFFFF000ULL)

#define SPRN_IAC1                 (0x138)          // Instruction Address Compare 1
#define SPRN_IAC2                 (0x139)          // Instruction Address Compare 2
#define SPRN_IAC3                 (0x13A)          // Instruction Address Compare 3
#define SPRN_IAC4                 (0x13B)          // Instruction Address Compare 4

#define SPRN_IMMR                 (0x371)          // Instruction Match Mask Register (lower 32b defined)

#define SPRN_IMR                  (0x370)          // Instruction Match Register

#define SPRN_IUCR0                (0x3F3)          // Instruction Unit Configuration Register 0
#define  IUCR0_BP_GS_LEN(x)        _B3(55,x)       //  Gshare History Length
#define  IUCR0_BP_BC_EN            _BN(56)         //  Enable branch prediction conditional instructions
#define  IUCR0_BP_BCLR_EN          _BN(57)         //  Enable branch prediction for branch to link register instructions
#define  IUCR0_BP_BCCTR_EN         _BN(58)         //  Enable branch prediction for branch to count register instructions
#define  IUCR0_BP_SW_EN            _BN(59)         //  Enable branch prediction software hints
#define  IUCR0_BP_DY_EN            _BN(60)         //  Enable dynamic hardware branch prediction (branch history table)
#define  IUCR0_BP_ST_EN            _BN(61)         //  Enable static hardware branch prediction (always predict taken)
#define  IUCR0_BP_TI_EN            _BN(62)         //  Enable thread isolation for BHT
#define  IUCR0_BP_GS_EN            _BN(63)         //  Enable gshare for BHT

#define SPRN_IULLCR               (0x37C)          // Instruction Unit Live Lock Control Register 
#define  IULLCR_LL_TRIG_DLY(x)    _B4(49,x)        //   IU Live Lock Trigger Delay
#define  IULLCR_LL_HOLD_DLY(x)    _B6(59,x)        //   IU Live Lock Hold Delay
#define  IULLCR_IULL_EN           _BN(63)          //   IU Live Lock Buster Enable

#define SPRN_IVPR                 (0x03F)          // Interrupt Vector Prefix Register (upper 52bits defined)
#define  IVPR_MASK                 (0xFFFFFFFFFFFFF000ULL)

#define SPRN_LPID                 (0x27E)          // Logical Partition ID
#define  LPID_LPID(x)              _B10(63,x)      //  10bit Logical Partition ID matched against TLB's TLPID field.

#define SPRN_LPIDR                (  338)          // Logical Partition Identification Register
#define  LPIDR_LPID(x)             _B8(63,x)       //  Logical Partition ID

#define SPRN_LRATCFG              (689)            // LRAT Configuration Register   (Hypervisor Read-Only)
#define  LRATCFG_Expect            (0x0000000000001008ULL) // fuly assoc LRAT, no LRAT inv prot, has CFG2, 8 LRAT entries

#define SPRN_LRATCFG2             (345)            // LRAT Configuration Register 2 (Hypervisor Read-Only)
#define  LRATCFG2_Expect           (0x000000000000ef50ULL) // not pgtbl loadable, no IND, 1TB, 256GB, 16GB, 4GB, 1GB, 256MB, 16MB, 1MB

#define SPRN_MMUCFG               (0x3F7)          // MMU Configuration Register (Hypervisor Read-Only)
#define  MMUCFG_Expect             (0x0000000008558380ULL) // 8b LPID, 42b RealAddr, LRAT, TLB WC, 14b PID, 1 TLB, MMU Ver 1.0

#define SPRN_MMUCSR0              (0x3F4)          // MMU Control and Status Register 0
#define  MMUCSR0_LAE               _BN(40)         //  LRAT Array Enable
#define  MMUCSR0_TLB0_FI           _BN(61)         //  TLB 0 Full Invalidate: write 1 to start, self-clears when done.

#define SPRN_MMUCR1               (1021)           // Memory Management Unit Control Register 1
#define  MMUCR1_IRRE               _BN(32)         //  I-ERAT LRU Round Robbin Enable
#define  MMUCR1_DRRE               _BN(33)         //  D-ERAT LRU Round Robbin Enable
#define  MMUCR1_REE                _BN(34)         //  Reference Exception Enable: 1 means clear R bit generates ISI/DSI
#define  MMUCR1_CEE                _BN(35)         //  Change Exception Enable: 1 means clear C bit generates ISI/DSI
#define  MMUCR1_CSINV(x)           _B2(37,x)       //  Context Sync Invalidate
                                                   //   0: All context syncs invalidate non-protected ERAT entries
                                                   //   1: All context syncs except isync invalidate non-protected ERAT entries
                                                   //   2: All context syncs will not invalidate ERATs
                                                   //   3: All context syncs will not invalidate ERATs (dup)
#define  MMUCR1_PEI_IERAT_WS0     _BN(38)          // Parity Error Inject I-ERAT WS=0
#define  MMUCR1_PEI_IERAT_WS1     _BN(39)          // Parity Error Inject I-ERAT WS=1
#define  MMUCR1_PEI_DERAT_WS0     _BN(40)          // Parity Error Inject D-ERAT WS=0
#define  MMUCR1_PEI_DERAT_WS1     _BN(41)          // Parity Error Inject D-ERAT WS=1
#define  MMUCR1_PEI_TLB_WS0       _BN(42)          // Parity Error Inject TLB WS=0
#define  MMUCR1_PEI_TLB_WS1       _BN(43)          // Parity Error Inject TLB WS=1
#define  MMUCR1_ERRDET            _BN(44)          // Error Detect: 1=Error Detected and IEEN,DEEN,TEEN contain snapshot
#define  MMUCR1_IEEN(x)           _B4(48,x)        // I-ERAT Error Entry Number
#define  MMUCR1_DEEN(x)           _B5(53,x)        // D-ERAT Error Entry Number
#define  MMUCR1_TEEN(x)           _B10(63,x)       // TLB    Error Entry Number
#define  MMUCR1_INIT              (MMUCR1_CSINV(3))

#define SPRN_MMUCR2               (1022)           // Memory Management Unit Control Register 2
#define  MMUCR2_PS4(x)             _B4(47,x)       //  TLB Page Size 4 Select
#define   MMUCR2_PS4_DISABLED       MMUCR2_PS4( 0) //   Do not apply the hash for this page size
#define   MMUCR2_PS4_4KB            MMUCR2_PS4( 1) //   Page size =  4KB
#define   MMUCR2_PS4_64KB           MMUCR2_PS4( 3) //   Page size = 64KB
#define   MMUCR2_PS4_1MB            MMUCR2_PS4( 5) //   Page size =  1MB
#define   MMUCR2_PS4_16MB           MMUCR2_PS4( 7) //   Page size = 16MB
#define   MMUCR2_PS4_1GB            MMUCR2_PS4(10) //   Page size =  1GB
#define  MMUCR2_PS3(x)             _B4(51,x)       //  TLB Page Size 3 Select
#define   MMUCR2_PS3_DISABLED       MMUCR2_PS3( 0) //   Do not apply the hash for this page size
#define   MMUCR2_PS3_4KB            MMUCR2_PS3( 1) //   Page size =  4KB
#define   MMUCR2_PS3_64KB           MMUCR2_PS3( 3) //   Page size = 64KB
#define   MMUCR2_PS3_1MB            MMUCR2_PS3( 5) //   Page size =  1MB
#define   MMUCR2_PS3_16MB           MMUCR2_PS3( 7) //   Page size = 16MB
#define   MMUCR2_PS3_1GB            MMUCR2_PS3(10) //   Page size =  1GB
#define  MMUCR2_PS2(x)             _B4(55,x)       //  TLB Page Size 2 Select
#define   MMUCR2_PS2_DISABLED       MMUCR2_PS2( 0) //   Do not apply the hash for this page size
#define   MMUCR2_PS2_4KB            MMUCR2_PS2( 1) //   Page size =  4KB
#define   MMUCR2_PS2_64KB           MMUCR2_PS2( 3) //   Page size = 64KB
#define   MMUCR2_PS2_1MB            MMUCR2_PS2( 5) //   Page size =  1MB
#define   MMUCR2_PS2_16MB           MMUCR2_PS2( 7) //   Page size = 16MB
#define   MMUCR2_PS2_1GB            MMUCR2_PS2(10) //   Page size =  1GB
#define  MMUCR2_PS1(x)             _B4(59,x)       //  TLB Page Size 1 Select
#define   MMUCR2_PS1_DISABLED       MMUCR2_PS1( 0) //   Do not apply the hash for this page size
#define   MMUCR2_PS1_4KB            MMUCR2_PS1( 1) //   Page size =  4KB
#define   MMUCR2_PS1_64KB           MMUCR2_PS1( 3) //   Page size = 64KB
#define   MMUCR2_PS1_1MB            MMUCR2_PS1( 5) //   Page size =  1MB
#define   MMUCR2_PS1_16MB           MMUCR2_PS1( 7) //   Page size = 16MB
#define   MMUCR2_PS1_1GB            MMUCR2_PS1(10) //   Page size =  1GB
#define  MMUCR2_PS0(x)             _B4(63,x)       //  TLB Page Size 0 Select
#define   MMUCR2_PS0_DISABLED       MMUCR2_PS0( 0) //   Do not apply the hash for this page size
#define   MMUCR2_PS0_4KB            MMUCR2_PS0( 1) //   Page size =  4KB
#define   MMUCR2_PS0_64KB           MMUCR2_PS0( 3) //   Page size = 64KB
#define   MMUCR2_PS0_1MB            MMUCR2_PS0( 5) //   Page size =  1MB
#define   MMUCR2_PS0_16MB           MMUCR2_PS0( 7) //   Page size = 16MB
#define   MMUCR2_PS0_1GB            MMUCR2_PS0(10) //   Page size =  1GB

#define SPRN_PIR                  (0x11E)          // Processor ID Register (See GPIR)             Hypervisor Read-only
#define  PID_CID(x)                _B8(61,x)       //  Processor Core ID   0..16 for BG/Q
#define  PID_TID(x)                _B2(63,x)       //  Processor Thread ID 0..3

#define SPRN_PVR                  (0x11F)          // Processor Version Register                   Hypervisor Read-only

#define SPRN_TBLWO                (0x11C)          // Time Base Lower                              Hypervisor Write-only
#define SPRN_TBUWO                (0x11D)          // Time Base Upper                              Hypervisor Write-only

#define SPRN_TENS                 ( 438)           // Thread Enable Register: Write-Set
#define SPRN_TENC                 ( 439)           // Thread Enable Register: Write-Clear
#define SPRN_TENSR                ( 437)           // Thread Enable Register: Read-Only
#define  TEN_TEN(x)                 _B4(63,x)      //  Thread Enables: 63-t
#define  TEN_TEN0                   TEN_TEN(0x1)
#define  TEN_TEN1                   TEN_TEN(0x2)
#define  TEN_TEN2                   TEN_TEN(0x4)
#define  TEN_TEN3                   TEN_TEN(0x8)
#define  TEN_TEN_ALL                TEN_TEN(0xf)
#define  TEN_TEN_INIT               TEN_TEN(0x1)   // At reset, only Thd[0] is enabled.


#define SPRN_TLB0CFG              (0x2B0)          // TLB 0 Configuration Register (Hypervisor Read-only)
#define  TLB0CFG_Expect            (0x000000000400b200ULL) // 4-Way Set Assoc, has Inv. Prot, HES Supported, has CFG2 spr, 512 entries

#define SPRN_TLB0CFG2             (344)            // TLB 0 Configuration Register 2 (Hypervisor Read-only)
#define   TLB0CFG2_Expect          (0x0000000000038255ULL) // h/w walker, IND supported, non-consec pg sizes, 1GB, 16MB, 1M, 64K, 4K

#define SPRN_XUCR0                (0x3F6)          // XU Configuration Register 0
#define  XUCR0_CLKG_CTL(x)         _B5(36,x)       //  Clock Gating Control
#define  XUCR0_MBAR_ACK            _BN(41)         //  Memory barrier acknowledge:
                                                   //    0 lwsync and mbar complete internal to the core.
                                                   //    1 lwsync and mbar complete after sync_done is received on the A2 L2 interface (same behavior as hwsync).
#define  XUCR0_TLBSYNC             _BN(42)         //  TLB Sync Ack: 0:Ack by A2, 1:Ack by L2
#define  XUCR0_SSDLY(x)            _B5(47,x)       //  Slow SPR Delay: delay cycles + 7 for Slow SPR hole
#define  XUCR0_UCPI_DIS            _BN(48)         //  uCode Pre-Issue Disable: 1=Prevent XU from checking exceptions on pre-issued uCode instructions
#define  XUCR0_AFLSTA              _BN(49)         //  Force Load/Store Alignment for AXU
#define  XUCR0_MDDP                _BN(50)         //  Machine Check on Data Cache Directory Parity Error: 0=h/w, 1=s/w mchk
#define  XUCR0_CRED                _BN(51)         //  L2 Credit Control: 0=1 of each, 1=one total
#define  XUCR0_REL                 _BN(52)         //  L2 Reload Control: 0=every other cycle, 1=back to back quadwords
#define  XUCR0_MDCP                _BN(53)         //  Machine Check on Data Cache Parity Error: 0=h/w, 1=s/w mchk
#define  XUCR0_TCS                 _BN(54)         //  Timer Clock Select: 0=core clock, 1=external pulse
#define  XUCR0_FLSTA               _BN(55)         //  Force Load/Store Alignment: integer unit
#define  XUCR0_L2SIW               _BN(56)         //  L2 Store Interface Width: 0=16B, 1=32B
#define  XUCR0_FLH2L2              _BN(57)         //  Force Load Hits to L2: 0=normal, 1=force all load hits forwarded to L2
#define  XUCR0_DC_DIS              _BN(58)         //  Data Cache Disable: 0=enabled, 1=disabled
#define  XUCR0_WLK                 _BN(59)         //  Data Cache Way Locking Enable: 0=disabled, 1=enabled and TLB[u7] is ClassID
#define  XUCR0_CSLC                _BN(60)         //  Cache Snoop Lock Clear (ie, invalidation/dcbi/... hit locked line)
#define  XUCR0_CUL                 _BN(61)         //  Cache Unable to Lock: 0=lock set instr was effective, 1=lock was ineffective
#define  XUCR0_CLO                 _BN(62)         //  Cache Lock Overflow 0=no lock overflow, 1=lock overflow was encountered
#define  XUCR0_CLFC                _BN(63)         //  Cache Lock Bits Flash Clear: 0=default, 1=flash clear initiate and still in progress
#define  XUCR0_INIT                (XUCR0_MBAR_ACK | XUCR0_REL | XUCR0_TLBSYNC | XUCR0_L2SIW | XUCR0_FLH2L2 | XUCR0_SSDLY(7))

#define SPRN_XUCR2                (0x3F8)          // XU Configuration Register 2
#define  XUCR2_RMT3(x)             _B8(39,x)       //  L1 Replacement Management Table Entry 3: each bit is way. 1 means eligible for replacement
#define  XUCR2_RMT2(x)             _B8(47,x)       //  L1 Replacement Management Table Entry 2: each bit is way. 1 means eligible for replacement
#define  XUCR2_RMT1(x)             _B8(55,x)       //  L1 Replacement Management Table Entry 1: each bit is way. 1 means eligible for replacement
#define  XUCR2_RMT0(x)             _B8(63,x)       //  L1 Replacement Management Table Entry 0: each bit is way. 1 means eligible for replacement

#define SPRN_XUDBG0               (885)
#define XUDBG0_WAY(x)             _B3(51,x)
#define XUDBG0_ROW(x)             _B5(57,x)
#define XUDBG0_EXEC               _BN(62)
#define XUDBG0_DONE               _BN(63)

#define SPRN_XUDBG1               (886)
#define XUDBG1_WATCH(x)           _B4(48,x)
#define XUDBG1_LRU(x)             _B7(55,x)
#define XUDBG1_PARITY(x)          _B4(59,x)
#define XUDBG1_LOCK               _BN(62)
#define XUDBG1_VALID              _BN(63)

#define SPRN_XUDBG2               (887)
#define XUDBG2_TAG(x)             _B31(63,x)

#define SPRN_IUDBG0               (888)
#define IUDBG0_WAY(x)             _B2(51,x)
#define IUDBG0_ROW(x)             _B6(57,x)
#define IUDBG0_EXEC               _BN(62)
#define IUDBG0_DONE               _BN(63)

#define SPRN_IUDBG1               (889)
#define IUDBG1_LRU(x)             _B2(55,x)
#define IUDBG1_PARITY(x)          _B4(59,x)
#define IUDBG1_ENDIAN             _BN(60)

#define SPRN_IUDBG2               (890)
#define IUDBG2_TAG(x)             _B30(63,x)

//
// Per-Thread SPRs
//
#define MSR_CM                    _BN(32)         //  Computation Mode: 1=64bit, 0=32bit
#define MSR_ICM                   _BN(33)         //  Interrupt Computation Mode: 1=64bit, 0=32bit
#define MSR_GS                    _BN(35)         //  Guest State
#define MSR_UCLE                  _BN(37)         //  User Cache Locking Enable
#define MSR_SPV                   _BN(38)         //  Vector Available
#define MSR_WE                    _BN(45)         //  Wait State Enable
#define MSR_CE                    _BN(46)         //  Critical Input Enable
#define MSR_EE                    _BN(48)         //  External Input Enable
#define MSR_PR                    _BN(49)         //  Problem State (User Mode) Enable
#define MSR_FP                    _BN(50)         //  FPU Available
#define MSR_ME                    _BN(51)         //  Machine Check Enable
#define MSR_FE0                   _BN(52)         //  Floating-Point Exception Mode 0
#define MSR_DE                    _BN(54)         //  Debug Interrupt Enable
#define MSR_FE1                   _BN(55)         //  Floating-Point Exception Mode 1
#define MSR_IS                    _BN(58)         //  Instruction Address Space
#define MSR_DS                    _BN(59)         //  Data Address Space
#define MSR_KERNEL                (MSR_CM | MSR_ICM | MSR_ME | MSR_CE | MSR_EE | MSR_FP)
#define MSR_KERNEL_DISABLED       (MSR_CM | MSR_ICM | MSR_ME | MSR_FP)
#define MSR_KERNEL_FULL_DISABLED  (MSR_CM | MSR_ICM | MSR_FP)

#define SPRN_CCR3                 ( 1013)          // Core Configuration Register 3
#define  CCR3_SI                   _BN(63)         //  Force Single-Instruction mode
#define  CCR3_EEPRI                _BN(62)         //  Raise priority when external interrupts are disabled  

#define SPRN_CSRR0                (0x03A)          // Critical Save/Restore Register 0 (IP)
#define SPRN_CSRR0_IP             SPRN_CSRR0       //  alias

#define SPRN_CSRR1                (0x03B)          // Critical Save/Restore Register 1 (MSR)
#define SPRN_CSRR1_MSR            SPRN_CSRR1       //  alias
#define  CSRR1_CM                  MSR_CM          //  Computation Mode: 1=64bit, 0=32bit
#define  CSRR1_GS                  MSR_GS          //  Guest State
#define  CSRR1_ICM                 MSR_ICM         //  Interrupt Computation Mode: 1=64bit, 0=32bit
#define  CSRR1_UCLE                MSR_UCLE        //  User Cache Locking Enable
#define  CSRR1_SPV                 MSR_SPV         //  Vector Available
#define  CSRR1_WE                  MSR_WE          //  Wait State Enable
#define  CSRR1_CE                  MSR_CE          //  Critical Input Enable
#define  CSRR1_EE                  MSR_EE          //  External Input Enable
#define  CSRR1_PR                  MSR_PR          //  Problem State (User Mode) Enable
#define  CSRR1_FP                  MSR_FP          //  FPU Available
#define  CSRR1_ME                  MSR_ME          //  Machine Check Enable
#define  CSRR1_FE0                 MSR_FE0         //  Floating-Point Exception Mode 0
#define  CSRR1_DE                  MSR_DE          //  Debug Interrupt Enable
#define  CSRR1_FE1                 MSR_FE1         //  Floating-Point Exception Mode 1
#define  CSRR1_IS                  MSR_IS          //  Instruction Address Space
#define  CSRR1_DS                  MSR_DS          //  Data Address Space

#define SPRN_DBCR0                (0x134)          // Debug Control Register 0
#define  DBCR0_EDM                 _BN(32)         //  External Debug Mode
#define  DBCR0_IDM                 _BN(33)         //  Internal Debug Mode
#define  DBCR0_RST(x)              _B2(35,x)       //  Reset
#define   DBCR0_RST_NONE            DBCR0_RST(0)   //   No Action
#define   DBCR0_RST_CORE            DBCR0_RST(1)   //   Core Reset
#define   DBCR0_RST_CHIP            DBCR0_RST(2)   //   Chip Reset
#define   DBCR0_RST_SYSTEM          DBCR0_RST(3)   //   System Reset
#define  DBCR0_ICMP                _BN(36)         //  Instruction Completion Debug Event Enable
#define  DBCR0_BRT                 _BN(37)         //  Branch Taken           Debug Event Enable
#define  DBCR0_IRPT                _BN(38)         //  Interrupt Taken        Debug Event Enable
#define  DBCR0_TRAP                _BN(39)         //  Trap                   Debug Event Enable
#define  DBCR0_IAC1                _BN(40)         //  Instr Addr Compare 1   Debug Event Enable
#define  DBCR0_IAC2                _BN(41)         //  Instr Addr Compare 2   Debug Event Enable
#define  DBCR0_IAC3                _BN(42)         //  Instr Addr Compare 3   Debug Event Enable
#define  DBCR0_IAC4                _BN(43)         //  Instr Addr Compare 4   Debug Event Enable
#define  DBCR0_DAC1(x)             _B2(45,x)       //  Data  Addr Compare 1   Debug Event Enable
#define   DBCR0_DAC1_NONE           DBCR0_DAC1(0)  //   DCA1 events cannot occur
#define   DBCR0_DAC1_ST             DBCR0_DAC1(1)  //   DCA1 events can occur on store access
#define   DBCR0_DAC1_LD             DBCR0_DAC1(2)  //   DCA1 events can occur on load access
#define   DBCR0_DAC1_ANY            DBCR0_DAC1(3)  //   DCA1 events can occur on any access
#define  DBCR0_DAC2(x)             _B2(47,x)       //  Data  Addr Compare 2   Debug Event Enable
#define   DBCR0_DAC2_NONE           DBCR0_DAC2(0)  //   DCA2 events cannot occur
#define   DBCR0_DAC2_ST             DBCR0_DAC2(1)  //   DCA2 events can occur on store access
#define   DBCR0_DAC2_LD             DBCR0_DAC2(2)  //   DCA2 events can occur on load access
#define   DBCR0_DAC2_ANY            DBCR0_DAC2(3)  //   DCA2 events can occur on any access
#define  DBCR0_DAC3(x)             _B2(60,x)       //  Data  Addr Compare 3   Debug Event Enable
#define   DBCR0_DAC3_NONE           DBCR0_DAC3(0)  //   DCA3 events cannot occur
#define   DBCR0_DAC3_ST             DBCR0_DAC3(1)  //   DCA3 events can occur on store access
#define   DBCR0_DAC3_LD             DBCR0_DAC3(2)  //   DCA3 events can occur on load access
#define   DBCR0_DAC3_ANY            DBCR0_DAC3(3)  //   DCA3 events can occur on any access
#define  DBCR0_DAC4(x)             _B2(62,x)       //  Data  Addr Compare 4   Debug Event Enable
#define   DBCR0_DAC4_NONE           DBCR0_DAC4(0)  //   DCA4 events cannot occur
#define   DBCR0_DAC4_ST             DBCR0_DAC4(1)  //   DCA4 events can occur on store access
#define   DBCR0_DAC4_LD             DBCR0_DAC4(2)  //   DCA4 events can occur on load access
#define   DBCR0_DAC4_ANY            DBCR0_DAC4(3)  //   DCA4 events can occur on any access
#define  DBCR0_FT                  _BN(63)         //  Freeze Timers on debug event

#define SPRN_DBCR1                (0x135)          // Debug Control Register 1
#define  DBCR1_IAC1US(x)           _B2(33,x)       //  Instr Addr Compare 1 User/Supervisor Mode
#define   DBCR1_IAC1US_ANY         DBCR1_IAC1US(0) //   IAC1 events can occur
#define   DBCR1_IAC1US_PRIV        DBCR1_IAC1US(2) //   IAC1 events only if MSR[PR]=0
#define   DBCR1_IAC1US_USER        DBCR1_IAC1US(3) //   IAC1 events only if MSR[PR]=1 (User Mode)
#define  DBCR1_IAC1ER(x)           _B2(35,x)       //  Instr Addr Compare 2 Effective/Real Mode
#define   DBCR1_IAC1ER_EFF         DBCR1_IAC1ER(0) //   IAC1 events can occur based on effective addresses
#define   DBCR1_IAC1ER_EFF_IS0     DBCR1_IAC1ER(2) //   IAC1 events effective and only if MSR[IS]=0
#define   DBCR1_IAC1ER_EFF_IS1     DBCR1_IAC1ER(3) //   IAC1 events effective and only if MSR[IS]=1
#define  DBCR1_IAC2US(x)           _B2(37,x)       //  Instr Addr Compare 2 User/Supervisor Mode
#define   DBCR1_IAC2US_ANY         DBCR1_IAC2US(0) //   IAC2 events can occur
#define   DBCR1_IAC2US_PRIV        DBCR1_IAC2US(2) //   IAC2 events only if MSR[PR]=0
#define   DBCR1_IAC2US_USER        DBCR1_IAC2US(3) //   IAC2 events only if MSR[PR]=1 (User Mode)
#define  DBCR1_IAC2ER(x)           _B2(39,x)       //  Instr Addr Compare 2 Effective/Real Mode
#define   DBCR1_IAC2ER_EFF         DBCR1_IAC2ER(0) //   IAC2 events can occur based on effective addresses
#define   DBCR1_IAC2ER_EFF_IS0     DBCR1_IAC2ER(2) //   IAC2 events effective and only if MSR[IS]=0
#define   DBCR1_IAC2ER_EFF_IS1     DBCR1_IAC2ER(3) //   IAC2 events effective and only if MSR[IS]=1
#define  DBCR1_IAC12M              _BN(41)         //  Instr Addr Compare 1/2 Mode: 1=ExactMatch, 0=AddrBitMatch
#define  DBCR1_IAC3US(x)           _B2(49,x)       //  Instr Addr Compare 3 User/Supervisor Mode
#define   DBCR1_IAC3US_ANY         DBCR1_IAC3US(0) //   IAC3 events can occur
#define   DBCR1_IAC3US_PRIV        DBCR1_IAC3US(2) //   IAC3 events only if MSR[PR]=0
#define   DBCR1_IAC3US_USER        DBCR1_IAC3US(3) //   IAC3 events only if MSR[PR]=1 (User Mode)
#define  DBCR1_IAC3ER(x)           _B2(51,x)       //  Instr Addr Compare 3 Effective/Real Mode
#define   DBCR1_IAC3ER_EFF         DBCR1_IAC3ER(0) //   IAC3 events can occur based on effective addresses
#define   DBCR1_IAC3ER_EFF_IS0     DBCR1_IAC3ER(2) //   IAC3 events effective and only if MSR[IS]=0
#define   DBCR1_IAC3ER_EFF_IS1     DBCR1_IAC3ER(3) //   IAC3 events effective and only if MSR[IS]=1
#define  DBCR1_IAC4US(x)           _B2(53,x)       //  Instr Addr Compare 4 User/Supervisor Mode
#define   DBCR1_IAC4US_ANY         DBCR1_IAC4US(0) //   IAC4 events can occur
#define   DBCR1_IAC4US_PRIV        DBCR1_IAC4US(2) //   IAC4 events only if MSR[PR]=0
#define   DBCR1_IAC4US_USER        DBCR1_IAC4US(3) //   IAC4 events only if MSR[PR]=1 (User Mode)
#define  DBCR1_IAC4ER(x)           _B2(55,x)       //  Instr Addr Compare 4 Effective/Real Mode
#define   DBCR1_IAC4ER_EFF         DBCR1_IAC4ER(0) //   IAC4 events can occur based on effective addresses
#define   DBCR1_IAC4ER_EFF_IS0     DBCR1_IAC4ER(2) //   IAC4 events effective and only if MSR[IS]=0
#define   DBCR1_IAC4ER_EFF_IS1     DBCR1_IAC4ER(3) //   IAC4 events effective and only if MSR[IS]=1
#define  DBCR1_IAC34M              _BN(57)         //  Instr Addr Compare 3/4 Mode: 1=ExactMatch, 0=AddrBitMatch

#define SPRN_DBCR2                (0x136)          // Debug Control Register 2
#define  DBCR2_DAC1US(x)           _B2(33,x)       //  Data Addr Compare 1 User/Supervisor Mode
#define   DBCR2_DAC1US_ANY         DBCR2_DAC1US(0) //   DAC1 events can occur
#define   DBCR2_DAC1US_PRIV        DBCR2_DAC1US(2) //   DAC1 events only if MSR[PR]=0
#define   DBCR2_DAC1US_USER        DBCR2_DAC1US(3) //   DAC1 events only if MSR[PR]=1 (User Mode)
#define  DBCR2_DAC1ER(x)           _B2(35,x)       //  Data Addr Compare 2 Effective/Real Mode
#define   DBCR2_DAC1ER_EFF         DBCR2_DAC1ER(0) //   DAC1 events can occur based on effective addresses
#define   DBCR2_DAC1ER_EFF_DS0     DBCR2_DAC1ER(2) //   DAC1 events effective and only if MSR[DS]=0
#define   DBCR2_DAC1ER_EFF_DS1     DBCR2_DAC1ER(3) //   DAC1 events effective and only if MSR[DS]=1
#define  DBCR2_DAC2US(x)           _B2(37,x)       //  Data Addr Compare 2 User/Supervisor Mode
#define   DBCR2_DAC2US_ANY         DBCR2_DAC2US(0) //   DAC2 events can occur
#define   DBCR2_DAC2US_PRIV        DBCR2_DAC2US(2) //   DAC2 events only if MSR[PR]=0
#define   DBCR2_DAC2US_USER        DBCR2_DAC2US(3) //   DAC2 events only if MSR[PR]=1 (User Mode)
#define  DBCR2_DAC2ER(x)           _B2(39,x)       //  Data Addr Compare 2 Effective/Real Mode
#define   DBCR2_DAC2ER_EFF         DBCR2_DAC2ER(0) //   DAC2 events can occur based on effective addresses
#define   DBCR2_DAC2ER_EFF_DS0     DBCR2_DAC2ER(2) //   DAC2 events effective and only if MSR[DS]=0
#define   DBCR2_DAC2ER_EFF_DS1     DBCR2_DAC2ER(3) //   DAC2 events effective and only if MSR[DS]=1
#define  DBCR2_DAC12M              _BN(41)         //  Data Addr Compare 1/2 Mode: 0=ExactMatch, 1=AddrBitMatch
#define  DBCR2_DVC1M(x)            _B2(45,x)       //  Data Value Compare 1 Mode
#define   DBCR2_DVC1M_ANY           DBCR2_DVC1M(0) //   DAC1 events can occur
#define   DBCR2_DVC1M_ALL           DBCR2_DVC1M(1) //   DAC1 events can occur when all bytes match
#define   DBCR2_DVC1M_ONE           DBCR2_DVC1M(2) //   DAC1 events can occur when at least 1 byte matches
#define  DBCR2_DVC2M(x)            _B2(47,x)       //  Data Value Compare 2 Mode
#define   DBCR2_DVC2M_ANY           DBCR2_DVC2M(0) //   DAC2 events can occur
#define   DBCR2_DVC2M_ALL           DBCR2_DVC2M(1) //   DAC2 events can occur when all bytes match
#define   DBCR2_DVC2M_ONE           DBCR2_DVC2M(2) //   DAC2 events can occur when at least 1 byte matches
#define  DBCR2_DVC1BE(x)           _B8(55,x)       //  Data Value Compare 1 Byte Enables
#define  DBCR2_DVC2BE(x)           _B8(63,x)       //  Data Value Compare 2 Byte Enables

#define SPRN_DBCR3                (0x350)          // Debug Control Register 3
#define  DBCR3_DAC3US(x)           _B2(33,x)       //  Data Addr Compare 1 User/Supervisor Mode
#define   DBCR3_DAC3US_ANY         DBCR3_DAC3US(0) //   DAC3 events can occur
#define   DBCR3_DAC3US_PRIV        DBCR3_DAC3US(2) //   DAC3 events only if MSR[PR]=0
#define   DBCR3_DAC3US_USER        DBCR3_DAC3US(3) //   DAC3 events only if MSR[PR]=1 (User Mode)
#define  DBCR3_DAC3ER(x)           _B2(35,x)       //  Data Addr Compare 2 Effective/Real Mode
#define   DBCR3_DAC3ER_EFF         DBCR3_DAC3ER(0) //   DAC3 events can occur based on effective addresses
#define   DBCR3_DAC3ER_EFF_DS0     DBCR3_DAC3ER(2) //   DAC3 events effective and only if MSR[DS]=0
#define   DBCR3_DAC3ER_EFF_DS1     DBCR3_DAC3ER(3) //   DAC3 events effective and only if MSR[DS]=1
#define  DBCR3_DAC4US(x)           _B2(37,x)       //  Data Addr Compare 2 User/Supervisor Mode
#define   DBCR3_DAC4US_ANY         DBCR3_DAC4US(0) //   DAC4 events can occur
#define   DBCR3_DAC4US_PRIV        DBCR3_DAC4US(2) //   DAC4 events only if MSR[PR]=0
#define   DBCR3_DAC4US_USER        DBCR3_DAC4US(3) //   DAC4 events only if MSR[PR]=1 (User Mode)
#define  DBCR3_DAC4ER(x)           _B2(39,x)       //  Data Addr Compare 2 Effective/Real Mode
#define   DBCR3_DAC4ER_EFF         DBCR3_DAC4ER(0) //   DAC4 events can occur based on effective addresses
#define   DBCR3_DAC4ER_EFF_DS0     DBCR3_DAC4ER(2) //   DAC4 events effective and only if MSR[DS]=0
#define   DBCR3_DAC4ER_EFF_DS1     DBCR3_DAC4ER(3) //   DAC4 events effective and only if MSR[DS]=1
#define  DBCR3_DAC34M              _BN(41)         //  Data Addr Compare 3/4 Mode: 0=ExactMatch, 1=AddrBitMatch
#define  DBCR3_ICV                 _BN(63)         //  Instruction Value Compare Event Enable

#define SPRN_DBSR                 (0x130)          // Debug Status Register                        Hypervisor Read/Write-Clear
#define  DBSR_IDE                  _BN(32)         //  Imprecise Debug Event (happened while MSR_DE was 0)
#define  DBSR_UDE                  _BN(33)         //  Unconditional Debug Event
#define  DBSR_MRR(x)               _B2(35,x)       //  Most Recent Reset
#define   DBSR_MRR_NONE             DBSR_MRR(0)    //   No Action
#define   DBSR_MRR_CORE             DBSR_MRR(1)    //   Core reset
#define   DBSR_MRR_CHIP             DBSR_MRR(2)    //   Chip reset
#define   DBSR_MRR_SYSTEM           DBSR_MRR(3)    //   System reset
#define  DBSR_ICMP                 _BN(36)         // Instruction Complete Debug Event
#define  DBSR_BRT                  _BN(37)         // Branch Taken         Debug Event
#define  DBSR_IRPT                 _BN(38)         // Interrupt Taken      Debug Event
#define  DBSR_TRAP                 _BN(39)         // Trap Instruction     Debug Event
#define  DBSR_IAC1                 _BN(40)         // IAC1                 Debug Event
#define  DBSR_IAC2                 _BN(41)         // IAC2                 Debug Event
#define  DBSR_IAC3                 _BN(42)         // IAC3                 Debug Event
#define  DBSR_IAC4                 _BN(43)         // IAC4                 Debug Event
#define  DBSR_DAC1R                _BN(44)         // DAC1 Read            Debug Event
#define  DBSR_DAC1W                _BN(45)         // DAC1 Write           Debug Event
#define  DBSR_DAC2R                _BN(46)         // DAC2 Read            Debug Event
#define  DBSR_DAC2W                _BN(47)         // DAC2 Write           Debug Event
#define  DBSR_RET                  _BN(48)         // Return               Debug Event
#define  DBSR_DAC3R                _BN(59)         // DAC1 Read            Debug Event
#define  DBSR_DAC3W                _BN(60)         // DAC1 Write           Debug Event
#define  DBSR_DAC4R                _BN(61)         // DAC2 Read            Debug Event
#define  DBSR_DAC4W                _BN(62)         // DAC2 Write           Debug Event
#define  DBSR_IVC                  _BN(63)         // Instr Value Compare  Debug Event

#define SPRN_DBSRWR               (  306)          // Debug Status Register Write Register         Hypervisor Write-Only
#define  DBSRWR_IDE                _BN(32)         //  Imprecise Debug Event (happened while MSR_D was 0)
#define  DBSRWR_UDE                _BN(33)         //  Unconditional Debug Event
#define  DBSRWR_MRR(x)             _B2(35,x)       //  Most Recent Reset
#define   DBSRWR_MRR_NONE           DBSR_MRR(0)    //   No Action
#define   DBSRWR_MRR_CORE           DBSR_MRR(1)    //   Core reset
#define   DBSRWR_MRR_CHIP           DBSR_MRR(2)    //   Chip reset
#define   DBSRWR_MRR_SYSTEM         DBSR_MRR(3)    //   System reset
#define  DBSRWR_ICMP               _BN(36)         // Instruction Complete Debug Event
#define  DBSRWR_BRT                _BN(37)         // Branch Taken         Debug Event
#define  DBSRWR_IRPT               _BN(38)         // Interrupt Taken      Debug Event
#define  DBSRWR_TRAP               _BN(39)         // Trap Instruction     Debug Event
#define  DBSRWR_IAC1               _BN(40)         // IAC1                 Debug Event
#define  DBSRWR_IAC2               _BN(41)         // IAC2                 Debug Event
#define  DBSRWR_IAC3               _BN(42)         // IAC3                 Debug Event
#define  DBSRWR_IAC4               _BN(43)         // IAC4                 Debug Event
#define  DBSRWR_DAC1R              _BN(44)         // DAC1 Read            Debug Event
#define  DBSRWR_DAC1W              _BN(45)         // DAC1 Write           Debug Event
#define  DBSRWR_DAC2R              _BN(46)         // DAC2 Read            Debug Event
#define  DBSRWR_DAC2W              _BN(47)         // DAC2 Write           Debug Event
#define  DBSRWR_RET                _BN(48)         // Return               Debug Event
#define  DBSRWR_DAC3R              _BN(59)         // DAC1 Read            Debug Event
#define  DBSRWR_DAC3W              _BN(60)         // DAC1 Write           Debug Event
#define  DBSRWR_DAC4R              _BN(61)         // DAC2 Read            Debug Event
#define  DBSRWR_DAC4W              _BN(62)         // DAC2 Write           Debug Event
#define  DBSRWR_IVC                _BN(63)         // Instr Value Compare  Debug Event

#define SPRN_DEAR                 (0x03D)          // Data Exception Address Register

#define SPRN_DEC                  (0x016)          // Decrementer
#define SPRN_UDEC                 (  550)          // User Decrementer

#define SPRN_DECAR                (0x036)          // Decrementer Auto-Reload                      Hypervisor Write-only

#define SPRN_EHCSR                (  307)          // Embedded Hypervisor Control and Status Register
#define  EHCSR_EXTGS               _BN(32)         //  External Input interrupt directed to Guest State
#define  EHCSR_DTLBGS              _BN(33)         //  Data TLB Error interrupt directed to Guest State
#define  EHCSR_ITLBGS              _BN(34)         //  Instruction TLB Error interrupt directed to Guest State
#define  EHCSR_DSIGS               _BN(35)         //  Data Storage Interrupt directed to Guest State
#define  EHCSR_ISIGS               _BN(36)         //  Instruction Storage Interrupt directed to Guest State
#define  EHCSR_DUVD                _BN(37)         //  Disable Ultravisor Debug
#define  EHCSR_ICM                 _BN(38)         //  Interrupt Computation Mode
#define  EHCSR_GICM                _BN(39)         //  Guest Interrupt Computation Mode
#define  EHCSR_DGTMI               _BN(40)         //  Disable TLB Guest Management Instructions
#define  EHCSR_DMIUH               _BN(41)         //  Disable MAS Interrupt Updates for Hypervisor

#define SPRN_EPLC                 (0x3B3)          // External Process ID Load  Context Register
#define  ELPC_EPR                  _BN(32)         //  External Load Context PR bit
#define  ELPC_EAS                  _BN(33)         //  External Load Context AS bit
#define  ELPC_EGS                  _BN(34)         //  External Load Context GS bit (writeable by hypervisor only)
#define  ELPC_ELPID(x)             _B8(47,x)       //  External Load Context Logical Process ID value (writeable by hypervisor only)
#define  ELPC_EPID(x)              _B14(63,x)      //  External Load Context Process ID value

#define SPRN_EPR                  (  702)          // External Proxy Register: 32 bit Vector on External Input Interrupt

#define SPRN_EPSC                 (0x3B4)          // External Process ID Store Context Register
#define  ESPC_EPR                  _BN(32)         //  External Store Context PR bit
#define  ESPC_EAS                  _BN(33)         //  External Store Context AS bit
#define  ESPC_EGS                  _BN(34)         //  External Store Context GS bit (writeable by hypervisor only)
#define  ESPC_ELPID(x)             _B8(47,x)       //  External Store Context Logical Process ID value (writeable by hypervisor only)
#define  ESPC_EPID(x)              _B14(63,x)      //  External Store Context Process ID value

#define SPRN_ESR                  (0x03E)          // Exception Syndrome Register
#define  ESR_PIL                   _BN(36)         //  Illegal Instruction exception
#define  ESR_PPR                   _BN(37)         //  Privileged Instruction exception
#define  ESR_PTR                   _BN(38)         //  Trap exception
#define  ESR_FP                    _BN(39)         //  Floating Point operation
#define  ESR_ST                    _BN(40)         //  Store operation
#define  ESR_DLK0                  _BN(42)         //  Data Locking exception 0
#define  ESR_DLK1                  _BN(43)         //  Data Locking exception 1
#define  ESR_AP                    _BN(44)         //  APU operation
#define  ESR_PUO                   _BN(45)         //  Unimplemented Operation exception
#define  ESR_BO                    _BN(46)         //  Byte Ordering exception
#define  ESR_PIE                   _BN(47)         //  Imprecise exception
#define  ESR_SPV                   _BN(56)         //  Vector operation
#define  ESR_EPID                  _BN(57)         //  External Process ID operation

#define SRPN_GDEAR                (  381)          // Guest Dear

#define SPRN_GEPR                 (  380)          // Guest External Proxy Register: 32 bit Vector on External Input Interrupt

#define SPRN_GESR                 (  383)          // Guest Exception Syndrome Register
#define  GESR_PIL                  _BN(36)         //  Illegal Instruction exception
#define  GESR_PPR                  _BN(37)         //  Privileged Instruction exception
#define  GESR_PTR                  _BN(38)         //  Trap exception
#define  GESR_FP                   _BN(39)         //  Floating Point operation
#define  GESR_ST                   _BN(40)         //  Store operation
#define  GESR_DLK0                 _BN(42)         //  Data Locking exception 0
#define  GESR_DLK1                 _BN(43)         //  Data Locking exception 1
#define  GESR_AP                   _BN(44)         //  APU operation
#define  GESR_PUO                  _BN(45)         //  Unimplemented Operation exception
#define  GESR_BO                   _BN(46)         //  Byte Ordering exception
#define  GESR_PIE                  _BN(47)         //  Imprecise exception
#define  GESR_SPV                  _BN(56)         //  Vector operation
#define  GESR_EPID                 _BN(57)         //  External Process ID operation

#define SPRN_GPIR                 (  286)          // Guest Processor Identification Register
#define  GPIR_VPTAG(x)             _B18(49,x)      //  Virtual Processor Tag
#define  GPIR_DBTAG(x)             _B14(63,x)      //  DoorBell Tag

#define SPRN_GSPRG0               (  368)          // Guest Special Purpose Register General 0 (see also SPRG0)
#define SPRN_GSPRG1               (  369)          // Guest Special Purpose Register General 1 (see also SPRG1)
#define SPRN_GSPRG2               (  370)          // Guest Special Purpose Register General 2 (see also SPRG2)
#define SPRN_GSPRG3               (  371)          // Guest Special Purpose Register General 3 (see also SPRG3, GSPRG3RO)

#define SPRN_GSRR0                ( 378)           // Guest Save Restore Register 0 (see SRR0)
#define SPRN_GSRR0_IP             SPRN_GSRR0       // alias

#define SPRN_GSRR1                ( 379)           // Guest Save Restore Register 1 (see SRR1)
#define SPRN_GSRR1_MSR            SPRN_GSRR1       // alias

#define SPRN_IAR                  (0x372)          // Instruction Address Register

#define SPRN_IUCR1                (  883)          // Instruction Unit Configuration Register 1
#define  IUCR1_PRI(x)              _B2(51,x)       //  Thread Priority
#define   IUCR1_HIPRI               IUCR1_PRI(1)   //   High Priority init value
#define  IUCR1_THRES(x)            _B6(63,x)       // Max amount of nclk cycles between low priority issues. (init=0)
#define  IUCR1_INIT                (IUCR1_HIPRI | IUCR1_THRES(4))

#define SPRN_IUCR2                 ( 884)

#define SPRN_MAS0                 (0x270)          // MMU Assist Register 0
#define  MAS0_ATSEL(x)             _B1(32,x)       //  Array Type Select: 0=TLB, 1=LRAT
#define   MAS0_ATSEL_TLB           MAS0_ATSEL(0)   //   TLB
#define   MAS0_ATSEL_LRAT          MAS0_ATSEL(1)   //   LRAT
#define  MAS0_ESEL(x)              _B3(47,x)       //  Entry Select
#define  MAS0_HES                  _BN(49)         //  Hardware Entry Select:
#define    MAS0_HES_ESEL           _B1(49,0)       //   TLB entry is ELSEL[1:2]||EPN[45:51]
#define    MAS0_HES_HW_LRU         _B1(49,1)       //   TLB entry is selected by the hardware LRU algorithm
#define  MAS0_WQ(x)                _B2(51,x)       //  Write Qualifier regarding TLB Reservation
#define   MAS0_WQ_ALWAYS            MAS0_WQ(0)     //   Always overwrite regardless of reservation
#define   MAS0_WQ_CONDITIONAL       MAS0_WQ(1)     //   Conditional Write based on reservation
#define   MAS0_WQ_CLEAR             MAS0_WQ(2)     //   Clear the TLB-reservation (no write)
#define   MAS0_WQ_Reserved          MAS0_WQ(3)     //   Reserved, don't use.

#define SPRN_MAS1                 (0x271)          // MMU Assist Register 1
#define  MAS1_V(x)                 _B1(32,x)       //  Valid
#define  MAS1_IPROT(x)             _B1(33,x)       //  Invalidate Protect: 0=unprotected, 1=protected from tlbivax, tlbilx, snoops
#define  MAS1_TID(x)               _B14(47,x)      //  TID
#define  MAS1_IND(x)               _B1(50,x)       //  Indirect: 0=translate, 1=HTW indirect page table pointer
#define  MAS1_TS(x)                _B1(51,x)       //  Translation Space
#define  MAS1_TSIZE(x)             _B4(55,x)       //  Translation Size:
#define   MAS1_TSIZE_4KB            MAS1_TSIZE( 1) //   IND=0
#define   MAS1_TSIZE_64KB           MAS1_TSIZE( 3) //   IND=0
#define   MAS1_TSIZE_1MB            MAS1_TSIZE( 5) //   IND=0  IND=1  LRAT
#define   MAS1_TSIZE_16MB           MAS1_TSIZE( 7) //                 LRAT
#define   MAS1_TSIZE_256MB          MAS1_TSIZE( 9) //   IND=0  IND=1  LRAT
#define   MAS1_TSIZE_1GB            MAS1_TSIZE(10) //   IND=0         LRAT
#define   MAS1_TSIZE_4GB            MAS1_TSIZE(11) //                 LRAT
#define   MAS1_TSIZE_16GB           MAS1_TSIZE(12) //                 LRAT
#define   MAS1_TSIZE_256GB          MAS1_TSIZE(14) //                 LRAT
#define   MAS1_TSIZE_1TB            MAS1_TSIZE(15) //                 LRAT

#define SPRN_MAS2                 (0x272)          // MMU Assist Register 2
#define  MAS2_EPN(x)               _B52(51,x)      //  Effective Page Number (page=4kB)
#define  MAS2_W(x)                 _B1(59,x)       //  Write-Through
#define  MAS2_I(x)                 _B1(60,x)       //  Cache-Inhibited
#define  MAS2_M(x)                 _B1(61,x)       //  Memory Coherence Required
#define  MAS2_G(x)                 _B1(62,x)       //  Guarded
#define  MAS2_E(x)                 _B1(63,x)       //  Endian : 0=Big, 1=Little

#define SPRN_MAS3                 (0x273)          // MMU Assist Register 3
#define  MAS3_RPNL(x)              _B20(51,x)      //  Real Page Number Lower bits, i.e., RPN[32:51]  (page=4kB)
#define  MAS3_U0(x)                _B1(54,x)       //  U0
#define  MAS3_U1(x)                _B1(55,x)       //  U1
#define  MAS3_U2(x)                _B1(56,x)       //  U2
#define  MAS3_U3(x)                _B1(57,x)       //  U3
#define  MAS3_UX(x)                _B1(58,x)       //  User Execute
#define  MAS3_SX(x)                _B1(59,x)       //  Supervisor Execute
#define  MAS3_UW(x)                _B1(60,x)       //  User Write
#define  MAS3_SW(x)                _B1(61,x)       //  Supervisor Write
#define  MAS3_UR(x)                _B1(62,x)       //  User Read
#define  MAS3_SR(x)                _B1(63,x)       //  Supervisor Read

#define SPRN_MAS4                 (0x274)          // MMU Assist Register 4
#define  MAS4_TSIZED(x)            _B4(55,x)       //  Default Translation Size Value
#define  MAS4_WD                   _BN(59)         //  Default Write-Through Value
#define  MAS4_ID                   _BN(60)         //  Default Cache-Inhibited Value
#define  MAS4_MD                   _BN(61)         //  Default Write-Through Value
#define  MAS4_GD                   _BN(62)         //  Default Guarded Value
#define  MAS4_ED                   _BN(63)         //  Default Endian Value

#define SPRN_MAS5                 (0x153)          // MMU Assist Register 5
#define  MAS5_SGS(x)               _B1(32,x)       //  Search Guest State
#define  MAS5_SLPID(x)             _B8(63,x)       //  Search Logical Partition Identifier

#define SPRN_MAS5_MAS6            (0x15C)

#define SPRN_MAS6                 (0x276)          // MMU Assist Register 6
#define  MAS6_SPID(x)              _B14(47,x)      //  Search Process Identifier
#define  MAS6_ISIZE(x)             _B4(55,x)       //  Invalidate Size
#define   MAS6_ISIZE_4KB            MAS6_ISIZE( 1) //   IND=0
#define   MAS6_ISIZE_64KB           MAS6_ISIZE( 3) //   IND=0
#define   MAS6_ISIZE_1MB            MAS6_ISIZE( 5) //   IND=0  IND=1  LRAT
#define   MAS6_ISIZE_16MB           MAS6_ISIZE( 7) //                 LRAT
#define   MAS6_ISIZE_256MB          MAS6_ISIZE( 9) //   IND=0  IND=1  LRAT
#define   MAS6_ISIZE_1GB            MAS6_ISIZE(10) //   IND=0         LRAT
#define   MAS6_ISIZE_4GB            MAS6_ISIZE(11) //                 LRAT
#define   MAS6_ISIZE_16GB           MAS6_ISIZE(12) //                 LRAT
#define   MAS6_ISIZE_256GB          MAS6_ISIZE(14) //                 LRAT
#define   MAS6_ISIZE_1TB            MAS6_ISIZE(15) //                 LRAT

#define  MAS6_SAS(x)               _B1(63,x)       //  Search Address Space

#define SPRN_MAS7                 (0x3B0)          // MMU Assist Register 7
#define  MAS7_RPNU(x)              _B10(63,x)      //  Real Page Number Upper bits, i.e., RPN[22:31]

#define SPRN_MAS8                 (0x155)          // MMU Assist Register 8
#define  MAS8_TGS(x)               _B1(32,x)       //  Translation Guest Space
#define  MAS8_VF(x)                _B1(33,x)       //  Translation Virtualization Fault
#define  MAS8_TLPID(x)             _B8(63,x)       //  Translation Logical Partition Identifier

#define SPRN_MAS7_MAS3            (372)
#define  MAS7_3_RPN(x)             _B30(51,x)      // Entire RPN

#define SPRN_MCSR                 (0x23C)          // Machine Check Status Register
#define  MCSR_DPOVR                _BN(48)         //  DITC Data Port Overrun
#define  MCSR_DDMH                 _BN(49)         //  Data Cache Directory Multi-hit Error
#define  MCSR_TLBIVAXSR            _BN(50)         //  tlbivax Snuup Reject
#define  MCSR_TLBLRUPE             _BN(51)         //  TLB LRU Parity Error
#define  MCSR_IL2ECC               _BN(52)         //  L2 Instruction Cache Uncorrectable ECC Error
#define  MCSR_DL2ECC               _BN(53)         //  L2 Data        Cache Uncorrectable ECC Error
#define  MCSR_DDPE                 _BN(54)         //  Data Cache Directory Parity Error (see XUCR0[MDDP]=1)
#define  MCSR_EXT                  _BN(55)         //  External Machine Check
#define  MCSR_DCPE                 _BN(56)         //  Data Cache Parity Error
#define  MCSR_IEMH                 _BN(57)         //  I-ERAT Multi-Hit Error
#define  MCSR_DEMH                 _BN(58)         //  D-ERAT Multi-Hit Error
#define  MCSR_TLBMH                _BN(59)         //  TLB Multi-Hit Error
#define  MCSR_IEPE                 _BN(60)         //  I-ERAT Parity Error
#define  MCSR_DEPE                 _BN(61)         //  D-ERAT Parity Error
#define  MCSR_TLBPE                _BN(62)         //  TLB Parity Error

#define SPRN_MCSRR0               (0x23A)          // Machine Check Save Restore Register 0 (PC)
#define SPRN_MCSRR0_IP            SPRN_MCSRR0      // alias

#define SPRN_MCSRR1               (0x23B)          // Machine Check Save Restore Register 1 (MSR)
#define SPRN_MCSRR1_MSR           SPRN_MCSRR1      // alias
#define  MCSRR1_CM                 MSR_CM          //  Computation Mode: 1=64bit, 0=32bit
#define  MCSRR1_GS                 MSR_GS          //  Guest State
#define  MCSRR1_ICM                MSR_ICM         //  Interrupt Computation Mode: 1=64bit, 0=32bit
#define  MCSRR1_UCLE               MSR_UCLE        //  User Cache Locking Enable
#define  MCSRR1_SPV                MSR_SPV         //  Vector Available
#define  MCSRR1_WE                 MSR_WE          //  Wait State Enable
#define  MCSRR1_CE                 MSR_CE          //  Critical Input Enable
#define  MCSRR1_EE                 MSR_EE          //  External Input Enable
#define  MCSRR1_PR                 MSR_PR          //  Problem State (User Mode) Enable
#define  MCSRR1_FP                 MSR_FP          //  FPU Available
#define  MCSRR1_ME                 MSR_ME          //  Machine Check Enable
#define  MCSRR1_FE0                MSR_FE0         //  Floating-Point Exception Mode 0
#define  MCSRR1_DE                 MSR_DE          //  Debug Interrupt Enable
#define  MCSRR1_FE1                MSR_FE1         //  Floating-Point Exception Mode 1
#define  MCSRR1_IS                 MSR_IS          //  Instruction Address Space
#define  MCSRR1_DS                 MSR_DS          //  Data Address Space

#define SPRN_MMUCR0               (1020)           // Memory Management Unit Control Register 0
#define  MMUCR0_ECL(x)             _B1(32,x)       //  Extended Class of ERAT Entry
#define  MMUCR0_TGS                _BN(34)         //  Translation Guest State of ERAT Entry
#define  MMUCR0_TS                 _BN(35)         //  Translation Space if ERAT Entry
#define  MMUCR0_TLBSEL(x)          _B2(37,x)       //  TLB Select:
#define   MMUCR0_TLBSEL_IERAT       MMUCR0_TLBSEL(2) //   I-ERAT Select
#define   MMUCR0_TLBSEL_DERAT       MMUCR0_TLBSEL(3) //   D-ERAT Select
#define  MMUCR0_TID(x)             _B14(63,x)      //  TID Field of ERAT Entry

#define SPRN_MMUCR3               (1023)           // Memory Management Unit Control Register 3
#define  MMUCR3_X(x)               _B1(49,x)       //  Exclusion Range Enable
#define  MMUCR3_R(x)               _B1(50,x)       //  Reference
#define  MMUCR3_C(x)               _B1(51,x)       //  Change
#define  MMUCR3_ECL(x)             _B2(53,x)       //  Extended Class
#define  MMUCR3_CLASS(x)           _B2(55,x)       //  Class
#define  MMUCR3_WLC(x)             _B1(56,x)       //  L1 D-Cache Way Locking Class Attribute
#define  MMUCR3_ThdID(x)           _B4(63,x)       //  Thread Identifier

#define SPRN_MSRP                 (  311)          // Machine State Register Protect Register
#define  MSRP_UCLEP                _BN(37)         //  User Cache Lock Enable Protect: 0=guest can modift MSR[UCLE]
#define  MSRP_DEP                  _BN(54)         //  Debug Enable Protect: 0=Guest can modify MSR[DE]

#define SPRN_PID                  (0x030)          // Process ID
#define  PID_PID(x)                _B14(63,x)      ///  14bit Process ID field for TLB

#define SPRN_SPRG0                (0x110)          // Special Purpose Register General 0 (see also GSPRG0)
#define SPRN_SPRG1                (0x111)          // Special Purpose Register General 1 (see also GSPRG1)
#define SPRN_SPRG2                (0x112)          // Special Purpose Register General 2 (see also GSPRG2)
#define SPRN_SPRG3                (0x113)          // Special Purpose Register General 3 (see also GSPRG3)
#define SPRN_SPRG4                (0x114)          // Special Purpose Register General 4
#define SPRN_SPRG5                (0x115)          // Special Purpose Register General 5
#define SPRN_SPRG6                (0x116)          // Special Purpose Register General 6
#define SPRN_SPRG7                (0x117)          // Special Purpose Register General 7
#define SPRN_SPRG8                (0x25C)          // Special Purpose Register General 8

#define SPRN_SRR0                 (0x01A)          // Save/Restore Register 0 (PC)
#define SPRN_SRR0_IP              SPRN_SRR0        // alias

#define SPRN_SRR1                 (0x01B)          // Save/Restore Register 1 (MSR)
#define SPRN_SRR1_MSR             SPRN_SRR1        // alias
#define  SRR1_CM                   MSR_CM          //  Computation Mode: 1=64bit, 0=32bit
#define  SRR1_GS                   MSR_GS          //  Guest State
#define  SRR1_ICM                  MSR_ICM         //  Interrupt Computation Mode: 1=64bit, 0=32bit
#define  SRR1_UCLE                 MSR_UCLE        //  User Cache Locking Enable
#define  SRR1_SPV                  MSR_SPV         //  Vector Available
#define  SRR1_WE                   MSR_WE          //  Wait State Enable
#define  SRR1_CE                   MSR_CE          //  Critical Input Enable
#define  SRR1_EE                   MSR_EE          //  External Input Enable
#define  SRR1_PR                   MSR_PR          //  Problem State (User Mode) Enable
#define  SRR1_FP                   MSR_FP          //  FPU Available
#define  SRR1_ME                   MSR_ME          //  Machine Check Enable
#define  SRR1_FE0                  MSR_FE0         //  Floating-Point Exception Mode 0
#define  SRR1_DE                   MSR_DE          //  Debug Interrupt Enable
#define  SRR1_FE1                  MSR_FE1         //  Floating-Point Exception Mode 1
#define  SRR1_IS                   MSR_IS          //  Instruction Address Space
#define  SRR1_DS                   MSR_DS          //  Data Address Space

#define SPRN_TCR                  (0x154)          // Timer Control Register
#define  TCR_WP(x)                 _B2(33,x)       //  Watchdog Timer Period
#define   TCR_WP_23                 TCR_WP(0)      //    2^23 time base clocks
#define   TCR_WP_27                 TCR_WP(1)      //    2^27 time base clocks
#define   TCR_WP_31                 TCR_WP(2)      //    2^31 time base clocks
#define   TCR_WP_35                 TCR_WP(3)      //    2^35 time base clocks
#define  TCR_WRC(x)                _B2(35,x)       //  Watchdog Timer Reset Control
#define   TCR_WRC_NONE              TCR_WRC(0)     //    No watchdog timer reset will occur
#define   TCR_WRC_CORE              TCR_WRC(1)     //    Core reset
#define   TCR_WRC_CHIP              TCR_WRC(2)     //    Chip reset
#define   TCR_WRC_SYSTEM            TCR_WRC(3)     //    System reset
#define  TCR_WIE                   _BN(36)         //  Watchdog Timer Interrupt Enable
#define  TCR_DIE                   _BN(37)         //  Decrementer Interrupt Enable
#define  TCR_FP(x)                 _B2(39,x)       //  Fixed-Interval Timer Period
#define   TCR_FP_15                 TCR_FP(0)      //   2^15 time base clocks
#define   TCR_FP_19                 TCR_FP(1)      //   2^19 time base clocks
#define   TCR_FP_23                 TCR_FP(2)      //   2^23 time base clocks
#define   TCR_FP_27                 TCR_FP(3)      //   2^27 time base clocks
#define  TCR_FIE                   _BN(40)         //  Fixed-Interval Timer Interrupt Enable
#define  TCR_ARE                   _BN(41)         //  Auto-Reload Enable
#define  TCR_UDIE                  _BN(42)         //  User Decrementer Interrupt Enable
#define  TCR_UD                    _BN(51)         //  User Decrementer Available

#define SPRN_TSR                  (0x150)          // Timer Status Register                        Hypervisor Read/Clear
#define  TSR_ENW                   _BN(32)         //  Enable Next Watchdog Timer
#define  TSR_WIS                   _BN(33)         //  Watchdog Timer Interrupt Status
#define  TSR_WRS(x)                _B2(35,x)       //  Watchdog Timer Reset Status
#define   TSR_WRS_NONE              TSR_WRS(0)     //   No watchdog Timer reset has occurred
#define   TSR_WRS_CORE              TSR_WRS(1)     //   Core reset was forced
#define   TSR_WRS_CHIP              TSR_WRS(2)     //   Chip reset was forced
#define   TSR_WRS_SYSTEM            TSR_WRS(3)     //   System reset was forced
#define  TSR_DIS                   _BN(36)         //  Decrementer Interrupt Status
#define  TSR_FIS                   _BN(37)         //  Fixed-Interval Interrupt Status
#define  TSR_UDIS                  _BN(38)         //  User Decrementer Interrupt Status

#define SPRN_XUCR1                (  851)              // XU Configuration Register 1
#define  XUCR1_LL_TB_SEL(x)        _B3(59,x)           //  LiveLock Buster Hang Pulse Timebase select
#define   XUCR1_LL_TB_SEL_9         XUCR1_LL_TB_SEL(1) //   2^9  time base clocks
#define   XUCR1_LL_TB_SEL_11        XUCR1_LL_TB_SEL(2) //   2^11
#define   XUCR1_LL_TB_SEL_13        XUCR1_LL_TB_SEL(3) //   2^13
#define   XUCR1_LL_TB_SEL_15        XUCR1_LL_TB_SEL(4) //   2^15
#define   XUCR1_LL_TB_SEL_19        XUCR1_LL_TB_SEL(5) //   2^19
#define   XUCR1_LL_TB_SEL_23        XUCR1_LL_TB_SEL(6) //   2^23
#define   XUCR1_LL_TB_SEL_27        XUCR1_LL_TB_SEL(7) //   2^27
#define  XUCR1_LL_STATE(x)         _B2(61,x)           //  LiveLock Buster State (read-only)
#define   XUCR1_LL_STATE_NORMAL     XUCR1_LL_STATE(0)  //   Normal operation
#define   XUCR1_LL_STATE_POTENTIAL  XUCR1_LL_STATE(1)  //   Potential LiveLock
#define   XUCR1_LL_STATE_LL_THIS    XUCR1_LL_STATE(2)  //   LiveLock, attempting forward progress on this thread only
#define   XUCR1_LL_STATE_LL_OTHER   XUCR1_LL_STATE(3)  //   LiveLock, attempting forward progress on another thread
#define  XUCR1_LL_SEL              _BN(62)             //  LiveLock Buster Hang Pulse Select: 0=timebase, 1=external pulse
#define  XUCR1_LL_EN               _BN(63)             //  LiveLock Buster Enable: 0=Disable, 1=Enable LL Buster logic

#define SPRN_XUCR4                 (853)
#define   XUCR4_MMU_MCHK           _BN(46)



#endif // __KERNEL__

//
// A2 Processor Version Values for DD1 and DD2. 
//
#define SPRN_PVR_DD1              (0x490000)       // BGQ DD1's PVR value 
#define SPRN_PVR_DD2              (0x490200)       // BGQ DD2's PVR value
                                                   // Other values are not yet defined.

//
// User Accessible Per-Core SPRs
//
#endif                                                                                                                 // FUSEDOS
#define  SPRN_TBRO                (0x10C)          // Time Base 64-bit                             User Read-only
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#define  SPRN_TBLRO               (0x10C)          // Time Base Lower                              User Read-only
#define  SPRN_TBURO               (0x10D)          // Time Base Upper                              User Read-only

//
// User Accessible Per-Thread SPRs
//
#define SPRN_CTR                  (0x009)          // Count Register                               User Read/Write

#define SPRN_GSPRG3RO             (  595)          // Guest Special Purpose Register General 3 (see also SPRG3, GSPRG3)

#define SPRN_LR                   (0x008)          // Link Register                                User Read/Write

#define SPRN_SPRG3RO              (0x103)          // Special Purpose Register General 3           User Read-only
#define SPRN_SPRG4RO              (0x104)          // Special Purpose Register General 4           User Read-only
#define SPRN_SPRG5RO              (0x105)          // Special Purpose Register General 5           User Read-only
#define SPRN_SPRG6RO              (0x106)          // Special Purpose Register General 6           User Read-only
#define SPRN_SPRG7RO              (0x107)          // Special Purpose Register General 7           User Read-only

#define SPRN_VRSAVE               (0x100)          // VR Save Register General                     User Read/Write

#define SPRN_XER                  (0x001)          // Integer Exception Register                   User Read/Write

#ifndef AIX
#define  XER_SO                    _BN(32)         //  Summary Overflow
#define  XER_OV                    _BN(33)         //  Overflow
#define  XER_CA                    _BN(34)         //  Carry bit from extended arithmetic ops
#define  XER_SI(x)                 _B7(63,x)       //  String Index
#endif

// FPSCR: Floating Point Status and Control Register
#define FPSCR_DRN(x)               _B3(31,x)       // DFP Rounding Control
#define  FPSCR_DRN_NEAREST_EVEN     FPSCR_DRN(0)   //  Round to Nearest, Ties to Even
#define  FPSCR_DRN_ZERO             FPSCR_DRN(1)   //  Round toward Zero
#define  FPSCR_DRN_PLUS_INF         FPSCR_DRN(2)   //  Round toward +Infinity
#define  FPSCR_DRN_MINUS_INF        FPSCR_DRN(3)   //  Round toward -Infinity
#define  FPSCR_DRN_NEAREST_NZ       FPSCR_DRN(4)   //  Round to Nearesst, Ties away from 0
#define  FPSCR_DRN_NEAREST_ZERO     FPSCR_DRN(5)   //  Round to Nearesst, Ties toward 0
#define  FPSCR_DRN_NONZERO          FPSCR_DRN(6)   //  Round to away from 0
#define  FPSCR_DRN_SHORTER          FPSCR_DRN(7)   //  Round to prepare for shorter precision
#define FPSCR_FX                   _BN(32)         //   FP Exception Summary
#define FPSCR_FEX                  _BN(33)         //   FP Enabled Exception Summary
#define FPSCR_VX                   _BN(34)         //   FP Invalid Operation Exception Summary
#define FPSCR_OX                   _BN(35)         //   FP Invalid Overflow Exception (Stickey)
#define FPSCR_UX                   _BN(36)         //   FP Invalid Underflow Exception (Stickey)
#define FPSCR_ZX                   _BN(37)         //   FP Zero Divide Exception (Stickey)
#define FPSCR_XX                   _BN(38)         //   FP Inexact Exception (Stickey)
#define FPSCR_VXSNAN               _BN(39)         //   FP Invalid Operation Exception for SNaN (Stickey)
#define FPSCR_VXISI                _BN(40)         //   FP Invalid Operation Exception for Inf-Inf (Stickey)
#define FPSCR_VXIDI                _BN(41)         //   FP Invalid Operation Exception for Inf/Inf (Stickey)
#define FPSCR_VXZDZ                _BN(42)         //   FP Invalid Operation Exception for 0/0 (Stickey)
#define FPSCR_VXIMZ                _BN(43)         //   FP Invalid Operation Exception for Inf*0 (Stickey)
#define FPSCR_VXVC                 _BN(44)         //   FP Invalid Operation Exception for Invalid Compare (Stickey)
#define FPSCR_FR                   _BN(45)         //   FP Fraction Rounded
#define FPSCR_FI                   _BN(46)         //   FP Fraction Inexact
#define FPSCR_FPRF                 _BN(47)         //   FP Result Flag
#define FPSCR_FL                   _BN(48)         //   FP Less Than or Negative
#define FPSCR_FG                   _BN(49)         //   FP Greater Than or Positive
#define FPSCR_FE                   _BN(50)         //   FP Equal to Zero
#define FPSCR_FU                   _BN(51)         //   FP Unordered or NaN
#define FPSCR_Rsvd52               _BN(52)         //   Reserved
#define FPSCR_VXSOFT               _BN(53)         //   FP Invalid Operation Exception for Software Request (Stickey)
#define FPSCR_VXSQRT               _BN(54)         //   FP Invalid Operation Exception for Invalid Square Root (Stickey)
#define FPSCR_VXCVI                _BN(55)         //   FP Invalid Operation Exception for Invalid Integer Convert (Stickey)
#define FPSCR_VE                   _BN(56)         //   FP Invalid Operation Exception Enable
#define FPSCR_OE                   _BN(57)         //   FP Overflow Exception Enable
#define FPSCR_UE                   _BN(58)         //   FP Underflow Exception Enable
#define FPSCR_ZE                   _BN(59)         //   FP Zero-Divide Exception Enable
#define FPSCR_XE                   _BN(60)         //   FP Inexact Exception Enable
#define FPSCR_NI                   _BN(61)         //   FP Non-IEEE Mode
#define FPSCR_RN(x)                _B2(63,x)       //   FP Rounding Control
#define   FPSCR_RN_NEAR             FPSCR_RN(0)    //     Round Nearest
#define   FPSCR_RN_ZERO             FPSCR_RN(1)    //     Round Toward Zero
#define   FPSCR_RN_PINF             FPSCR_RN(2)    //     Round Toward +Infinity
#define   FPSCR_RN_MINF             FPSCR_RN(3)    //     Round Toward -Infinity
#define FPSCR_INIT                 (0)             // if Debug: (FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE)

#define L1I_CACHE_LINE_SIZE        64
#define L1D_CACHE_LINE_SIZE        64

/////////////////////////////////////////////////////
//
// QPX definitions
//

#define SPRN_AXUCR0                (0x3D0)         // Auxiliary unit control register 0

#define AXUCR0_LFSR_RESET          _BN(15)         // Reset stochastic rounding engine
#define AXUCR0_SR_ENABLE           _BN(16)         // Enable stochastic rounding
#define AXUCR0_ENAB_NAN_IND_0      _BN(20)         // Enable NaN indicate for thread 0.
#define AXUCR0_ENAB_INF_IND_0      _BN(21)
#define AXUCR0_ENAB_NAN_IND_1      _BN(22)
#define AXUCR0_ENAB_INF_IND_1      _BN(23)
#define AXUCR0_ENAB_NAN_IND_2      _BN(24)
#define AXUCR0_ENAB_INF_IND_2      _BN(25)
#define AXUCR0_ENAB_NAN_IND_3      _BN(26)
#define AXUCR0_ENAB_INF_IND_3      _BN(27)
#define AXUCR0_ENAB_IND_ALL(x)     _B8(27,x)
#define AXUCR0_ENAB_IND(th,x)      (_B2(21,x) >> ((th)*2))

__END_DECLS

#endif                                                                                                                 // FUSEDOS
#endif // _A2_CORE_H_ Add nothing below this line.
