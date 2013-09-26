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


// +--------------------------------------------------------------------------------+
// |                                                                                |
// |  NOTE: Other parts of the BG/Q system are sensitive to structural changes to   |
// |        the personality defined here.  When making changes, you will need to    |
// |        coordinate with (at a minimum) LLCS.                                    |
// |                                                                                |
// +--------------------------------------------------------------------------------+


#ifndef _PERSONALITY_H_
#define _PERSONALITY_H_

//! @page Personality
//!
//!  @section PERS_10 Overview
//!
//!  The personality is the fundamental structure containing configuration data for a node.
//!  The control system (or some other external agent) can "personalize" the behavior of
//!  firmware and kernels via the data contained here.
//!


// We want this file to be standalone, so we define our own types here:


typedef unsigned long long  fw_uint64_t;
typedef unsigned       fw_uint32_t;
typedef unsigned short fw_uint16_t;
typedef unsigned char  fw_uint8_t;

#if defined(__ASSEMBLY__)
#define FW_U64(x) x
#else
#define FW_U64(x) x##ull
#endif

#define FW_BIT(b)       ((FW_U64(1)<<(63-(b))))
#define FW_BIT_x2(b,x)  (((x) & FW_U64(0x03)    ) << (63-(b)))
#define FW_BIT_x4(b,x)  (((x) & FW_U64(0x0F)    ) << (63-(b)))
#define FW_BIT_x5(b,x)  (((x) & FW_U64(0x1F)    ) << (63-(b)))
#define FW_BIT_x12(b,x) (((x) & FW_U64(0xFFF)   ) << (63-(b)))
#define FW_BIT_x17(b,x) (((x) & FW_U64(0x1FFFF) ) << (63-(b)))

#define PERSONALITY_VERSION     (0x08)  //! The version of the personality.
#define DEFAULT_FREQ_MHZ        (1600)  //! The expected frequency of the BG/Q chip; currently 1.6GHz.
#define PERSONALITY_LEN_NFSDIR    (32)  //! The length (in bytes) of the NFS directory array.
#define PERSONALITY_LEN_SECKEY    (32)  //! The length (in bytes) of the security key array.
#define NUM_ND_ZONES              (4)   //! The number of dynamic routing zones in the Network Device.
// Personality.Kernel_Config.RASPolicy
#define PERS_RASPOLICY_VERBOSITY(x)   FW_BIT_x2(33,x)                  //! Verbosity as shown below
#define PERS_RASPOLICY_MINIMAL          PERS_RASPOLICY_VERBOSITY(0) //! Benchmarking level of capture and reporting
#define PERS_RASPOLICY_NORMAL           PERS_RASPOLICY_VERBOSITY(1) //! Normal production level of capture and reporting
#define PERS_RASPOLICY_VERBOSE          PERS_RASPOLICY_VERBOSITY(2) //! Manufacturing test and diagnostics
#define PERS_RASPOLICY_EXTREME          PERS_RASPOLICY_VERBOSITY(3) //! Report every event immediately - Thresholds set to 1
#define PERS_RASPOLICY_FATALEXIT      FW_BIT(34)                       //! Fatal is Fatal, so exit.

#define PERS_RASPOLICY_DEFAULT        (PERS_RASPOLICY_VERBOSE | PERS_RASPOLICY_FATALEXIT)


//! @brief The PERS_ENABLE_* masks are used to control various features and drivers of firmware
//!        and kernels.  They define bits in the Personality.NodeConfig element.

#define PERS_ENABLE_MMU             FW_BIT(0)   //! Enables use of the MMU.  NOTE: this bit may go away once MMU support stabilizes.
#define PERS_ENABLE_IsIoNode        FW_BIT(1)   //! @deprecated Used as a proxy for testint bit
#define PERS_ENABLE_TakeCPU         FW_BIT(2)   //! Enables use of the takeCPU service.
#define PERS_ENABLE_MU              FW_BIT(3)   //! Enables the Messaging Unit
#define PERS_ENABLE_ND              FW_BIT(4)   //! Enables the Network Device
#define PERS_ENABLE_Timestamps      FW_BIT(5)   //! Enables automatic embedding of timestamps in mailbox stdout messages
#define PERS_ENABLE_BeDRAM          FW_BIT(6)   //! Enables BeDRAM initialization sequence
#define PERS_ENABLE_ClockStop       FW_BIT(7)   //! Enables ClockStop Unit initialization
#define PERS_ENABLE_DrArbiter       FW_BIT(8)   //! Enables DR Arbiter Unit initialization
#define PERS_ENABLE_DevBus          FW_BIT(9)   //! Enables Device Bus Unit initialization
#define PERS_ENABLE_L1P             FW_BIT(10)  //! Enables L1 Prefetch Unit initialization
#define PERS_ENABLE_L2              FW_BIT(11)  //! Enables L2 Unit initialization (including L2 Central
#define PERS_ENABLE_MSGC            FW_BIT(12)  //! Enables MSGC Unit initialization
#define PERS_ENABLE_TestInt         FW_BIT(13)  //! Enables Test Interface Unit initialization
#define PERS_ENABLE_NodeRepro       FW_BIT(14)  //! Enable single node cycle reproducibility
#define PERS_ENABLE_PartitionRepro  FW_BIT(15)  //! Enable partition-wide cycle reproducibility
#define PERS_ENABLE_DD1_Workarounds FW_BIT(16)  //! @deprecated Enables DD1 hardware workarounds in firmware.  DD1 hardware support may disappear in the future.
#define PERS_ENABLE_A2_Errata       FW_BIT(17)  //! Enables A2 workarounds
#define PERS_ENABLE_A2_IU_LLB       FW_BIT(18)  //! Enables the A2 I Unit Livelock Buster
#define PERS_ENABLE_A2_XU_LLB       FW_BIT(19)  //! Enables the A2 X Unit Livelock Buster
#define PERS_ENABLE_DDRCalibration  FW_BIT(20)  //! Enables DDR Calibration
#define PERS_ENABLE_DDRFastInit     FW_BIT(21)  //! Enables DDR Fast Init
#define PERS_ENABLE_DDRCellTest     FW_BIT(22)  //! Enables DDR Cell Test
#define PERS_ENABLE_DDRAutoSize     FW_BIT(23)  //! Enables Auto-detection of DDR size
#define PERS_ENABLE_MaskLinkErrors  FW_BIT(24)  //! Masks Network Link Errors (use with extreme caution!)
#define PERS_ENABLE_MaskCorrectables FW_BIT(25) //! Report first correctable and mask after that (preserves early behavior)
#define PERS_ENABLE_DDRDynamicRecal FW_BIT(26)  //! Enables Dynamic Memory Controller Recalibration
#define PERS_ENABLE_DDRNoTerm       FW_BIT(27)  //! Infinite Ohm on termination resistor.
#define PERS_ENABLE_DDRBackScrub    FW_BIT(28)  //! Background scrubbing
#define PERS_ENABLE_DDRDirScrub     FW_BIT(29)  //! Directed scrubbing
#define PERS_ENABLE__RESRVD_30      FW_BIT(30)
#define PERS_ENABLE__RESRVD_31      FW_BIT(31)
#define PERS_ENABLE__RESRVD_32      FW_BIT(32)
#define PERS_ENABLE__RESRVD_33      FW_BIT(33)
#define PERS_ENABLE__RESRVD_34      FW_BIT(34)
#define PERS_ENABLE__RESRVD_35      FW_BIT(35)
#define PERS_ENABLE_AppPreload      FW_BIT(36)  //! Indicates that application loading via CIOS is unecessary
#define PERS_ENABLE_IOServices      FW_BIT(37)  //! Enables CIOS function shipping
#define PERS_ENABLE_SpecCapDDR      FW_BIT(38)  //! DDR storage is speculation-capable
#define PERS_ENABLE_L2Only          FW_BIT(39)  //! Run out of L2, without DDR
#define PERS_ENABLE_FPGA            FW_BIT(40)  //! Running in FPGA simulation
#define PERS_ENABLE_DDRINIT         FW_BIT(41)  //! Enables the DDR controller initialization
#define PERS_ENABLE_Mailbox         FW_BIT(42)  //! Enables the mailbox
#define PERS_ENABLE_Simulation      FW_BIT(43)  //! Running in cycle simulation
#define PERS_ENABLE_Mambo           FW_BIT(44)  //! Running in Mambo (who uses this ... Linux?)
#define PERS_ENABLE_JTagConsole     FW_BIT(45)  //! Routes application console output through jtag (versus function shipped)
#define PERS_ENABLE_JTagLoader      FW_BIT(46)  //! Converse with the JTag host to load additional images (kernels and such)
#define PERS_ENABLE_FPU             FW_BIT(47)  //! Enable the QPX floating point hardware
#define PERS_ENABLE_L2Counters      FW_BIT(48)  //! Enable the L2 atomics (BeDRAM counters always enabled)
#define PERS_ENABLE_Wakeup          FW_BIT(49)  //! Enable the Wakeup Unit initialization
#define PERS_ENABLE_BIC             FW_BIT(50)  //! Enable Blue Gene Interrupt Controller
#define PERS_ENABLE_DDR             FW_BIT(51)  //! Enable DDR controllers
#define PERS_ENABLE__RESRVD_52      FW_BIT(52)
#define PERS_ENABLE__RESRVD_53      FW_BIT(53)
#define PERS_ENABLE_GlobalInts      FW_BIT(54)  //! Enable global interrupts embedded in Network
#define PERS_ENABLE_SerDes          FW_BIT(55)  //! Enable High-speed Serializer/Deserializer
#define PERS_ENABLE_UPC             FW_BIT(56)  //! Enable Universal Performance Counter Unit initializaiton
#define PERS_ENABLE_EnvMon          FW_BIT(57)  //! Enable Environmental Monitoring
#define PERS_ENABLE_PCIe            FW_BIT(58)  //! Enable PCIExpress
#define PERS_ENABLE__RESRVD_59      FW_BIT(59)
#define PERS_ENABLE__RESRVD_60      FW_BIT(60)
#define PERS_ENABLE_TimeSync        FW_BIT(61)  //! Enable A2 Timebase synchronization
#define PERS_ENABLE__RESRVD_62      FW_BIT(62)
#define PERS_ENABLE_DiagnosticsMode FW_BIT(63)  //! Enable diagnostics mode

//! @brief The TRACE_* masks are used to selectively enable tracing code.  They describe bit positions in the 
//!        Personality.TraceConfig field.
#define TRACE_BeDRAM    FW_BIT(0)    //! Trace BeDRAM firmware
#define TRACE_DevBus    FW_BIT(1)    //! Trace DevBus firmware
#define TRACE_DDR       FW_BIT(2)    //! Trace DDR controller
#define TRACE_L2        FW_BIT(3)    //! Trace L2
#define TRACE_ClockStop FW_BIT(4)    //! Trace ClockStop
#define TRACE_L1P       FW_BIT(5)    //! Trace L1
#define TRACE_ND        FW_BIT(32)   //! Trace Network Device firmware
#define TRACE_MU        FW_BIT(33)   //! Trace Messaging Unit firmware
#define TRACE_PCIe      FW_BIT(34)   //! Trace PCIe initialization activity
#define TRACE_Entry     FW_BIT(36)   //! Trace function entry and exit (lots of output)
#define TRACE_MChk      FW_BIT(40)   //! Trace machine check dispatch
#define TRACE_SysCall   FW_BIT(41)   //! Trace system calls
#define TRACE_VMM       FW_BIT(42)   //! Trace Virtual Memory Manager
#define TRACE_Debug     FW_BIT(43)   //! Trace debug events (app crashes etc)
#define TRACE_SerDes    FW_BIT(47)   //! Trace the SerDes unit
#define TRACE_Process   FW_BIT(52)   //! Trace process/thread mapping
#define TRACE_Exit_Sum  FW_BIT(53)   //! Trace per-core interrupt and error summary on exit()
#define TRACE_Sched     FW_BIT(54)   //! Trace scheduler information
#define TRACE_Futex     FW_BIT(57)   //! Trace futex operations
#define TRACE_MemAlloc  FW_BIT(58)   //! Trace MMAP and Shared Memory operations
#define TRACE_Verbose   FW_BIT(63)   //! Verbose tracing modifier (potentially makes any trace point more verbose)

//! @typedef Personality_Kernel_t
//! @struct  Personality_Kernel_t
//! @brief   The portion of the personality that is specific to firmware and kernel software

typedef struct Personality_Kernel_t
                {
		fw_uint64_t  UCI;                //!< Universal Component Identifier.
                fw_uint64_t  NodeConfig;         //!< Kernel device and core enables (see PERS_ENABLE_* masks)
                fw_uint64_t  TraceConfig;        //!< Kernel tracing enables
                fw_uint32_t  RASPolicy;          //!< Verbosity level, and other RAS Reporting Controls
                fw_uint32_t  FreqMHz;            //!< Clock_X1 Frequency in MegaHertz (e.g. 1000 for 1GHz)
                fw_uint64_t  ClockStop;          //!< Clockstop value
}
                Personality_Kernel_t;

//! @typedef Personality_DDR_t
//! @struct  Personality_DDR_t
//! @brief   The portion of the personlity that configures DDR memory and its associated controller.

typedef struct Personality_DDR_t
                {
                fw_uint32_t  DDRFlags;         //!< Miscellaneous flags and settings (see below)
                fw_uint32_t  DDRSizeMB;        //!< Total DDR size in mega bytes (likely 16GB).
                }
                Personality_DDR_t;


// DDRFlags is as follows:
//
//   +---------------------+----+
//   |  reserved (unused)  | PD |
//   +---------------------+----+
//   0                  29  30 31
//
//   PD - DDR Power Down mode
//

#define DDR_POWER_DOWN_MASK            0x00000003
#define DDR_POWER_DOWN(v)              ((v) & DDR_POWER_DOWN_MASK)
#define   DDR_POWER_DOWN_OFF           0
#define   DDR_POWER_DOWN_FAST          1
#define   DDR_POWER_DOWN_SLOW          2
#define DDR_GET_POWER_DOWN(flags)      ( ( (flags) & DDR_POWER_DOWN_MASK )  >> 0 )
#define DDR_SET_POWER_DOWN(flags,v)    flags = ( ( (flags) & ~DDR_POWER_DOWN_MASK ) | ( (v) & DDR_POWER_DOWN_MASK ) )


//! @typedef Personality_Networks_t
//! @struct  Personality_Networks_t
//! @brief   The portion of the personality that is specific to Blue Gene networks.

typedef struct Personality_Networks_t
                {
                fw_uint32_t  BlockID;         //!< a.k.a. PartitionID
                fw_uint64_t  NetFlags;        //!< See below
		fw_uint64_t  NetFlags2;
		fw_uint8_t   Anodes,          //!< The A dimension
          		  Bnodes,          //!< The B dimension
                          Cnodes,          //!< The C dimension
                          Dnodes,          //!< The D dimension
                          Enodes,          //!< The E dimension
                          Acoord,          //!< The A coordinate
                          Bcoord,          //!< The B coordinate
                          Ccoord,          //!< The C coordinate
                          Dcoord,          //!< The D coordinate
                          Ecoord;          //!< The E coordinate

		  struct PrimordialClassRoute {
		    fw_uint16_t GlobIntUpPortInputs;           // Global Interrupt Up Tree Inputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
		    fw_uint16_t GlobIntUpPortOutputs;          // Global Interrupt Up Tree Outputs. Only the 12 LSBs are relevant (11 torus links + 1 internal).
		    fw_uint16_t CollectiveTypeAndUpPortInputs; // Collective type and Up Tree Inputs.  See encodings below
		    fw_uint16_t CollectiveUpPortOutputs;       // Collective Up Tree Outputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
		  } PrimordialClassRoute;

		  fw_uint32_t  ZoneRoutingMasks[NUM_ND_ZONES];        // Each contains 5 masks.  See bit assignments below
		  fw_uint64_t  MuFlags;  //!< Message Unit flags (see below)
		  fw_uint8_t   cnBridge_A, cnBridge_B, cnBridge_C, cnBridge_D, cnBridge_E; //!< Torus coordinates of compute node bridge
                  fw_uint32_t  latencyFromRoot;  //!< GI Latency from root node in pclk.
                }
                Personality_Networks_t;

#define ND_TORUS_SIZE(net) ((net).Anodes * (net).Bnodes * (net).Cnodes * (net).Dnodes * (net).Enodes)

//! @brief The ND_ENABLE_* bits define the individual control bits for the NetFlags field (above).
//!    Note that since the field is only 16 bits, we don't use the bit macros from above (which assume
//!    in 64-bit Big Endian format).

#define ND_ENABLE_ION_LINK             FW_BIT(7)                      //! Enables the compute to IO node link (compute nodes only)
#define ND_DIM_A_LOOPBACK(v)           FW_BIT_x2(9,v)                 //! Torus/SerDes loopback for dimension A
#define ND_DIM_B_LOOPBACK(v)           FW_BIT_x2(11,v)                //! Torus/SerDes loopback for dimension B
#define ND_DIM_C_LOOPBACK(v)           FW_BIT_x2(13,v)                //! Torus/SerDes loopback for dimension C
#define ND_DIM_D_LOOPBACK(v)           FW_BIT_x2(15,v)                //! Torus/SerDes loopback for dimension D
#define ND_DIM_E_LOOPBACK(v)           FW_BIT_x2(17,v)                //! Torus/SerDes loopback for dimension E
#define ND_DIM_IO_LOOPBACK(v)          FW_BIT_x2(19,v)                //! Torus/SerDes loopback for IO
#define   ND_LOOPBACK_NONE             0
#define   ND_LOOPBACK_TORUS_INTERNAL   1
#define   ND_LOOPBACK_SERDES           2
#define   ND_LOOPBACK_HSS              3
#define   ND_DIM_LOOPBACK(n,v)         (ND_DIM_A_LOOPBACK(v) >> (n*2))  //! n = 0, 1, .., 5 
#define ND_ENABLE_TORUS_DIM_A          FW_BIT(20)       //! Dimension A is a torus (1) or a mesh (0)
#define ND_ENABLE_TORUS_DIM_B          FW_BIT(21)       //! Dimension B is a torus (1) or a mesh (0)
#define ND_ENABLE_TORUS_DIM_C          FW_BIT(22)       //! Dimension C is a torus (1) or a mesh (0)
#define ND_ENABLE_TORUS_DIM_D          FW_BIT(23)       //! Dimension D is a torus (1) or a mesh (0)
#define ND_ENABLE_TORUS_DIM_E          FW_BIT(24)       //! Dimension E is a torus (1) or a mesh (0)
#define   ND_ENABLE_TORUS(n)           (ND_ENABLE_TORUS_DIM_A >> (n))  //! n = 0, 1, .., 4 for { A, B, .., E }
#define ND_ENABLE_TORUS_RAN_HINTS_A    FW_BIT(25)       //! Ties for +/- choice are broken randomly
#define ND_ENABLE_TORUS_RAN_HINTS_B    FW_BIT(26)       //! Ties for +/- choice are broken randomly
#define ND_ENABLE_TORUS_RAN_HINTS_C    FW_BIT(27)       //! Ties for +/- choice are broken randomly
#define ND_ENABLE_TORUS_RAN_HINTS_D    FW_BIT(28)       //! Ties for +/- choice are broken randomly
#define ND_ENABLE_TORUS_RAN_HINTS_E    FW_BIT(29)       //! Ties for +/- choice are broken randomly
#define   ND_ENABLE_TORUS_RAN_HINTS(n) (ND_ENABLE_TORUS_RAN_HINTS_A >> (n))  //! n = 0, 1, .., 4
#define ND_SE_ARB_RE_INJ_RATIO(v)      FW_BIT_x4(33,v)   //! receiver/injection priority
#define ND_DET_ORDER_MASK_0(v)         FW_BIT_x5(38,v)   //! For example,0b10000 is A first.
#define ND_DET_ORDER_MASK_1(v)         FW_BIT_x5(43,v)   //! For example,0b01000 is B second.
#define ND_DET_ORDER_MASK_2(v)         FW_BIT_x5(48,v)   //! For example,0b00100 is C third.
#define ND_DET_ORDER_MASK_3(v)         FW_BIT_x5(53,v)   //! For example,0b00010 is D fourth.
#define ND_DET_ORDER_MASK_4(v)         FW_BIT_x5(58,v)   //! For example,0b00001 is E fifth.
#define ND_DET_ORDER_MASK(n,v)         (ND_DET_ORDER_MASK_0(v) >> (n*5))  //! n=mask number (0..4)

#define ND_GET_ENABLE_ION_LINK(f)         ( ((f) & ND_ENABLE_ION_LINK ) ? 1 : 0 )
#define ND_GET_LOOPBACK(n,f)              ( ((f) & ND_DIM_LOOPBACK(n,-1))        >> (63-9-((n)*2)) )  // n = 0..5 (includes I/O dimension)
#define ND_GET_TORUS(n,f)                 ( ((f) & ND_ENABLE_TORUS(n))           >> (63-20-(n))    )  // n = 0..4
#define ND_GET_ALL_TORUS(f)               ( ((f) & FW_BIT_x5(24,-1))             >> (63-24)        )  // Get all Torus Enable bits at once
#define ND_GET_TORUS_RAN_HINTS(n,f)       ( ((f) & ND_ENABLE_TORUS_RAN_HINTS(n)) >> (63-25-(n))    )  // n = 0..4
#define ND_GET_ALL_TORUS_RAN_HINTS(f)     ( ((f) & FW_BIT_x5(29,-1))             >> (63-29)        )  // Get all Random Hint bits at once
#define ND_GET_SE_ARB_RE_INJ_RATIO(f)     ( ((f) & ND_SE_ARB_RE_INJ_RATIO(-1))   >> (63-33)        )      
#define ND_GET_DET_ORDER_MASK(n,f)        ( ((f) & ND_DET_ORDER_MASK(n,-1))      >> (63-38-(n*5))  ) // n = 0..4



#define ND_ENABLE_LINK_A_MINUS   FW_BIT(0)
#define ND_ENABLE_LINK_A_PLUS    FW_BIT(1)
#define ND_ENABLE_LINK_B_MINUS   FW_BIT(2)
#define ND_ENABLE_LINK_B_PLUS    FW_BIT(3)
#define ND_ENABLE_LINK_C_MINUS   FW_BIT(4)
#define ND_ENABLE_LINK_C_PLUS    FW_BIT(5)
#define ND_ENABLE_LINK_D_MINUS   FW_BIT(6)
#define ND_ENABLE_LINK_D_PLUS    FW_BIT(7)
#define ND_ENABLE_LINK_E_MINUS   FW_BIT(8)
#define ND_ENABLE_LINK_E_PLUS    FW_BIT(9)
#define SD_ENABLE_T0             FW_BIT(10)  //! A-
#define SD_ENABLE_T1             FW_BIT(11)  //! A+
#define SD_ENABLE_T2             FW_BIT(12)  //! B-
#define SD_ENABLE_T3             FW_BIT(13)  //! B+
#define SD_ENABLE_T4             FW_BIT(14)  //! C-
#define SD_ENABLE_T5             FW_BIT(15)  //! C+
#define SD_ENABLE_T6             FW_BIT(16)  //! D-
#define SD_ENABLE_T7             FW_BIT(17)  //! D+
#define SD_ENABLE_T8             FW_BIT(18)  //! E-
#define SD_ENABLE_T9             FW_BIT(19)  //! E+
#define SD_ENABLE_TIO            FW_BIT(20)  //! F
#define SD_ENABLE_HALFSPEED      FW_BIT(21)  //! Run HSS at half speed
#define TI_USE_PORT6_FOR_IO      FW_BIT(22)
#define TI_USE_PORT7_FOR_IO      FW_BIT(23)
#define TI_TORUS_DIM_A_REVERSED  FW_BIT(24)
#define TI_TORUS_DIM_B_REVERSED  FW_BIT(25)
#define TI_TORUS_DIM_C_REVERSED  FW_BIT(26)
#define TI_TORUS_DIM_D_REVERSED  FW_BIT(27)
#define TI_TORUS_DIM_E_REVERSED  FW_BIT(28)
#define ND_CN_BRIDGE_PORT_6      FW_BIT(29) // The Compute Node Bridge is using Port 6
#define ND_CN_BRIDGE_PORT_7      FW_BIT(30) // The Compute Node Bridge is using Port 7
#define ND_CN_BRIDGE_PORT_10     FW_BIT(31) // The Compute Node Bridge is using Port 10
#define SD_RESET                 FW_BIT(32) // Indicates a full reset of the node (used in control system)

#define   TI_TORUS_DIM_REVERSED(v) FW_BIT_x5(28,v)

// The following macros are useful for reading and altering the link enable bits.
// The "dimension" argument is an integer (A=0, B=1, ..., E=4, IO=5).
// The "direction" argument is either 0 (minus) or 1 (plus).

#define ND_LINK_ENABLE_MASK(dim,direction)         _BN( (2*(dim) + (direction) ) )
#define ND_SET_LINK_ENABLE(dim,direction,flags)    flags |=  ND_LINK_ENABLE_MASK(dim,direction)
#define ND_CLEAR_LINK_ENABLE(dim,direction,flags)  flags &= ~ND_LINK_ENABLE_MASK(dim,direction)
#define ND_GET_LINK_ENABLE(dim,direction,flags)    ( (flags) & ND_LINK_ENABLE_MASK(dim,direction) )

#define SD_LINK_ENABLE_MASK(dim,direction)         _BN( 2*(dim) + (direction) + 10 )
#define SD_SET_LINK_ENABLE(dim,direction,flags)    flags |=  SD_LINK_ENABLE_MASK(dim,direction)
#define SD_CLEAR_LINK_ENABLE(dim,direction,flags)  flags &= ~SD_LINK_ENABLE_MASK(dim,direction)
#define SD_GET_LINK_ENABLE(dim,direction,flags)    ( (flags) & SD_LINK_ENABLE_MASK(dim,direction) )

#define TI_TORUS_DIM_REVERSED_MASK(dim)              (TI_TORUS_DIM_A_REVERSED >> (dim))
#define TI_SET_TORUS_DIM_REVERSED(dim,flags)         flags |=  TI_TORUS_DIM_REVERSED_MASK(dim)
#define TI_CLEAR_TORUS_DIM_REVERSED(dim,flags)       flags &= ~TI_TORUS_DIM_REVERSED_MASK(dim)
#define TI_GET_TORUS_DIM_REVERSED(dim,flags)         ( ( (flags) & TI_TORUS_DIM_REVERSED_MASK(dim) ) >> (63-24-dim) )
#define TI_GET_ALL_TORUS_DIM_REVERSED(flags)         ( ( (flags) & TI_TORUS_DIM_REVERSED(-1) ) >> (63-28) )


//
// Class route Collective type and up tree input is encoded as follows:
//
//   +-----+-----------+--------------------+
//   | /// | Coll Type | Coll Up Port Input |
//   +-----+-----------+--------------------+
//   0    1 2         3 4                 15
//

#define ND_CR_COLL_TYPE(v)     (((v) & 0x3) << (15-3))
#define ND_CR_COLL_INPUT(v)    (((v) & 0xFFF))

#define ND_CR_GET_COLL_TYPE(v)    ( ((v) & 0x3000) >> (15-3) )
#define ND_CR_GET_COLL_INPUT(v)   ( ((v) & 0x0FFF) )

#define ND_CR_SET_COLL_TYPE(v,x)    { (x) &= ~ND_CR_COLL_TYPE(-1); (x) |= ND_CR_COLL_TYPE(v); }
#define ND_CR_SET_COLL_INPUT(v,x)   { (x) &= ~ND_CR_COLL_INPUT(-1); (x) |= ND_CR_COLL_INPUT(v); }

//
// +-----+-------+-------+-------+-------+-------+
// | /// | MASK0 | MASK1 | MASK2 | MASK3 | MASK4 |
// +-----+-------+-------+-------+-------+-------+
//  0   5 6    10 11   15 16   20 21   25 26    31
//

#define ND_DYNAMIC_ZONE_MASK0(v)     FW_BIT_x5(11+32,v)
#define ND_DYNAMIC_ZONE_MASK1(v)     FW_BIT_x5(16+32,v)
#define ND_DYNAMIC_ZONE_MASK2(v)     FW_BIT_x5(21+32,v)
#define ND_DYNAMIC_ZONE_MASK3(v)     FW_BIT_x5(26+32,v)
#define ND_DYNAMIC_ZONE_MASK4(v)     FW_BIT_x5(31+32,v)

// Encode mask field "m"
#define ND_DYNAMIC_ZONE_MASK(m,v)    (ND_DYNAMIC_ZONE_MASK0(v) >> ((m)*5))

// Getter and Setter.  z=zone (0..3), m=mask (0..4), v=value, nc=pointer to Network_Config structure

#define ND_SET_DYNAMIC_ZONE_MASK(z,m,v,nc) \
  (nc)->ZoneRoutingMasks[z] &= ~ND_DYNAMIC_ZONE_MASK(m,-1);	\
  (nc)->ZoneRoutingMasks[z] |=  ND_DYNAMIC_ZONE_MASK(m,v);

#define ND_GET_DYNAMIC_ZONE_MASK(z,m,nc) ( ( (nc)->ZoneRoutingMasks[z] & ND_DYNAMIC_ZONE_MASK(m,-1) ) >> (31-11-(m*5)) )

#define MU_ENABLE_GROUPS(v)           FW_BIT_x17(0,v)
#define MU_ENABLE_GROUP(n)            FW_BIT(0+(n))
#define MU_IS_GROUP_ENABLED(n,f)      ( ( (f) & MU_ENABLE_GROUP(n) ) ? 1 : 0 )

//! @typedef IP_Addr_t
//! @struct  IP_Addr_t
//! @brief   The IP address of this node.  There are 16 bytes of room in order to support IPv6
//!          addresses.  IPv4 addresses will occupy bytes 12-15

typedef struct IP_Addr_t
                {
                fw_uint8_t octet[ 16 ];
                }
                IP_Addr_t;


//! @typedef Personality_Ethernet_t
//! @struct  Personality_Ethernet_t
//! @brief   The ethernet configuration data.

typedef struct Personality_Ethernet_t
                {
		  fw_uint8_t     SecurityKey[ PERSONALITY_LEN_SECKEY ];   //!< Security Key for Service Node authentication
                }
                Personality_Ethernet_t;

//! @typedef Personality_Ethernet_t
//! @struct  Personality_Ethernet_t
//! @brief   The ethernet configuration data.

typedef struct Personality_t
                {
		fw_uint16_t                 CRC;                   //!< Crc16n starting from .Version
		fw_uint8_t                  Version;               //!< The personality version (see also PERSONALITY_VERSION constant)
                fw_uint8_t                  PersonalitySizeWords;  //!< (sizeof(Personality_t) >> 2)
 	        Personality_Kernel_t        Kernel_Config;         //!< The kernel configuration
		Personality_DDR_t           DDR_Config;            //!< The DDR configuration
		Personality_Networks_t      Network_Config;        //!< The network configurtion
		Personality_Ethernet_t      Ethernet_Config;       //!< The ethernet configuration
                }
                Personality_t;


#define PERSONALITY_DEFAULT()                                    \
  {                                                              \
    0,                                                           \
    PERSONALITY_VERSION, /* Version */                           \
    (sizeof(Personality_t) + 3)>>2, /* PersonalitySizeWords */   \
    { /* Kernel_Config */                                        \
      0, /* UCI */                                               \
      0 | /* NodeConfig */                                       \
      PERS_ENABLE_Mailbox |					 \
      PERS_ENABLE_DrArbiter |                                    \
      PERS_ENABLE_FPU |                                          \
      PERS_ENABLE_BIC |                                          \
      PERS_ENABLE_DDR |                                          \
      PERS_ENABLE_L1P |                                          \
      PERS_ENABLE_MSGC |					 \
      PERS_ENABLE_GlobalInts |                                   \
      PERS_ENABLE_SpecCapDDR |                                   \
      PERS_ENABLE_EnvMon |					 \
      /* PERS_ENABLE_MMU | @todo turn this on */                 \
	PERS_ENABLE_JTagConsole |				 \
	PERS_ENABLE_AppPreload |				 \
	  PERS_ENABLE_DiagnosticsMode |				 \
	  PERS_ENABLE_TimeSync |				 \
      0,                                                         \
	  0,							 \
      0, /* RASPolicy */                                         \
      DEFAULT_FREQ_MHZ /* FreqMHz */ ,                           \
    },                                                           \
    { /* DDR_Config */                                           \
	DDR_POWER_DOWN_FAST, /* DDRFlags */			 \
      16 * 1024, /* DDRSizeMB = 16GB */                          \
    },                                                           \
    { /* Network_Config */                                       \
      0, /* BlockID */                                           \
      0 | /* NetFlags */                                         \
        ND_ENABLE_ION_LINK | /* simulation only */               \
        ND_DIM_A_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_B_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_C_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_D_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_E_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_IO_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |         \
        ND_ENABLE_TORUS(0) |                                     \
        ND_ENABLE_TORUS(1) |                                     \
        ND_ENABLE_TORUS(2) |                                     \
        ND_ENABLE_TORUS(3) |                                     \
        ND_ENABLE_TORUS(4) |                                     \
        ND_ENABLE_TORUS_RAN_HINTS(0) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(1) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(2) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(3) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(4) |                           \
        ND_SE_ARB_RE_INJ_RATIO(0xF)  |                           \
        ND_DET_ORDER_MASK(0,0x10) | /* A first */                \
        ND_DET_ORDER_MASK(1,0x08) | /* B second */               \
        ND_DET_ORDER_MASK(2,0x04) | /* C third */                \
        ND_DET_ORDER_MASK(3,0x02) | /* D fourth */               \
        ND_DET_ORDER_MASK(4,0x01) | /* E fifth */                \
        0,                                                       \
	ND_ENABLE_LINK_A_MINUS | /* NetFlags2 */		 \
	ND_ENABLE_LINK_A_PLUS |					 \
	ND_ENABLE_LINK_B_MINUS |				 \
	ND_ENABLE_LINK_B_PLUS |					 \
	ND_ENABLE_LINK_C_MINUS |				 \
	ND_ENABLE_LINK_C_PLUS |					 \
	ND_ENABLE_LINK_D_MINUS |				 \
	ND_ENABLE_LINK_D_PLUS |					 \
	ND_ENABLE_LINK_E_MINUS |				 \
	ND_ENABLE_LINK_E_PLUS |					 \
	0,							 \
      1, 1, 1, 1, 1, /* Anodes,...,Enodes */	                 \
      0, 0, 0, 0, 0, /* Acoord,...,Ecoord */                     \
      { /* PrimordialClassRoute */                               \
	  1, 0, 0, 0 , /* local-input=1 for loopback */		 \
      }, /* ... */                                               \
      { 0, }, /* ZoneRoutingMasks */                             \
      FW_U64(0x8000000000000000), /* MuFlags - group 0 enabled */        \
      0,0,0,0,0, /* CN bridge address */			 \
      0,         /* latencyFromRoot */                           \
	},							 \
    { /* Ethernet_Config */                                      \
      { 0, }, /* SecurityKey */                                  \
  },							         \
};

#define PERSONALITY_PROD_DEFAULT()                                    \
  {                                                              \
    0,                                                           \
    PERSONALITY_VERSION, /* Version */                           \
    (sizeof(Personality_t) + 3)>>2, /* PersonalitySizeWords */   \
    { /* Kernel_Config */                                        \
      0, /* UCI */                                               \
      0 | /* NodeConfig */                                       \
	  PERS_ENABLE_A2_Errata |				 \
	  PERS_ENABLE_A2_XU_LLB |				 \
	  PERS_ENABLE_A2_IU_LLB |				 \
	  PERS_ENABLE_AppPreload |				 \
	  PERS_ENABLE_BeDRAM |					 \
	  PERS_ENABLE_BIC |					 \
	  PERS_ENABLE_DrArbiter |				 \
	  PERS_ENABLE_DDR |					 \
	  PERS_ENABLE_DDRINIT |					 \
	  PERS_ENABLE_DDRCalibration |				 \
	  PERS_ENABLE_DDRFastInit |				 \
	  PERS_ENABLE_DevBus |					 \
	  PERS_ENABLE_EnvMon |					 \
	  PERS_ENABLE_FPU |					 \
	  PERS_ENABLE_GlobalInts |				 \
	  PERS_ENABLE_JTagConsole |				 \
	  PERS_ENABLE_JTagLoader |				 \
	  PERS_ENABLE_L1P |					 \
	  PERS_ENABLE_L2 |					 \
	  PERS_ENABLE_MSGC |					 \
	  PERS_ENABLE_Mailbox |					 \
	  PERS_ENABLE_MMU |					 \
	  PERS_ENABLE_SpecCapDDR |				 \
	  PERS_ENABLE_SerDes |					 \
	  PERS_ENABLE_TestInt |					 \
          PERS_ENABLE_MU |					 \
          PERS_ENABLE_ND |					 \
          PERS_ENABLE_TakeCPU |					 \
	  PERS_ENABLE_TimeSync |				 \
      0,                                                         \
	  0,							 \
      0, /* RASPolicy */                                         \
      DEFAULT_FREQ_MHZ /* FreqMHz */ ,                           \
    },                                                           \
    { /* DDR_Config */                                           \
	DDR_POWER_DOWN_FAST, /* DDRFlags */			 \
      4 * 1024, /* DDRSizeMB = 4GB for DD1 */                    \
    },                                                           \
    { /* Network_Config */                                       \
      0, /* BlockID */                                           \
      0 | /* NetFlags */                                         \
        ND_ENABLE_ION_LINK | /* simulation only */               \
        ND_DIM_A_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_B_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_C_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_D_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_E_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |          \
        ND_DIM_IO_LOOPBACK(ND_LOOPBACK_TORUS_INTERNAL) |         \
        ND_ENABLE_TORUS(0) |                                     \
        ND_ENABLE_TORUS(1) |                                     \
        ND_ENABLE_TORUS(2) |                                     \
        ND_ENABLE_TORUS(3) |                                     \
        ND_ENABLE_TORUS(4) |                                     \
        ND_ENABLE_TORUS_RAN_HINTS(0) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(1) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(2) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(3) |                           \
        ND_ENABLE_TORUS_RAN_HINTS(4) |                           \
        ND_SE_ARB_RE_INJ_RATIO(0xF)  |                           \
        ND_DET_ORDER_MASK(0,0x10) | /* A first */                \
        ND_DET_ORDER_MASK(1,0x08) | /* B second */               \
        ND_DET_ORDER_MASK(2,0x04) | /* C third */                \
        ND_DET_ORDER_MASK(3,0x02) | /* D fourth */               \
        ND_DET_ORDER_MASK(4,0x01) | /* E fifth */                \
        0,                                                       \
	ND_ENABLE_LINK_A_MINUS | /* NetFlags2 */		 \
	ND_ENABLE_LINK_A_PLUS |					 \
	ND_ENABLE_LINK_B_MINUS |				 \
	ND_ENABLE_LINK_B_PLUS |					 \
	ND_ENABLE_LINK_C_MINUS |				 \
	ND_ENABLE_LINK_C_PLUS |					 \
	ND_ENABLE_LINK_D_MINUS |				 \
	ND_ENABLE_LINK_D_PLUS |					 \
	ND_ENABLE_LINK_E_MINUS |				 \
	ND_ENABLE_LINK_E_PLUS |					 \
	0,							 \
      1, 1, 1, 1, 1, /* Anodes,...,Enodes */	                 \
      0, 0, 0, 0, 0, /* Acoord,...,Ecoord */                     \
      { /* ClassRoutes */                                        \
	   1, 0, 0, 0 , /* local-input=1 for loopback */	 \
      }, /* ... */                                               \
      { 0, }, /* ZoneRoutingMasks */                             \
      FW_U64(0xFFFF800000000000), /* MuFlags - all groups enabled */        \
      0,0,0,0,0, /* CN bridge address */			 \
      0,         /* latencyFromRoot */                           \
	},							 \
    { /* Ethernet_Config */                                      \
      { 0, }, /* SecurityKey */                                  \
  },							         \
};


#endif // Add nothing below this line.
