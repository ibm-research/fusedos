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
// BGQ Interrupt Controller device control facilities.
//
#ifndef BIC_H
#define BIC_H

#include <hwi/include/bqc/PhysicalMap.h>
#ifndef __LINUX__
#include <stdint.h>
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif

__BEGIN_DECLS

#define BIC_DEVICE_BASE_ADDRESS  (unsigned long long)(PHYMAP_MINADDR_L1P + PHYMAP_PRIVILEGEDOFFSET)

// PUEA interrupt type decoding for mapping register
#define BIC_NO_INTERRUPT                     0x0
#define BIC_EXTERNAL_INTERRUPT               0x1
#define BIC_CRITICAL_INTERRUPT               0x2
#define BIC_MACHINE_CHECK                    0x3

// PUEA mapping lane assignments (by unit).  There are 32 lanes total but
// lanes 4..6 are unassigned.

#define BIC_MAP_L2C_LANE(n)  ( 0 + (n) )  // L2Central has lanes 0..3     so n=0..3
#define BIC_MAP_WAC_LANE     ( 6 )        // WAC Guard 
#define BIC_MAP_MU_LANE(n)   ( 7 + (n) )  // Message Unit has lanes 7..10 so n=0..3
#define BIC_MAP_GEA_LANE(n)  ( 11 + (n) ) // GEA has lanes 11..26         so n=0..15
#define BIC_MAP_L1P_LANE(n)  ( 27 + (n) ) // L1P has lanes 27..31         so n=0..4

// C2C Packet Format - int_type decoding
#define BIC_C2C_INTTYPE_EXTERNAL             0
#define BIC_C2C_INTTYPE_CRITICAL             1
#define BIC_C2C_INTTYPE_WAKE                 2

//  C2C Packet Format - dest_thread decoding
#define BIC_DEST_THREAD_EVENT_TO_NONE        0x00
#define BIC_DEST_THREAD_EVENT_TO_THREAD_0    0x01
#define BIC_DEST_THREAD_EVENT_TO_THREAD_1    0x02
#define BIC_DEST_THREAD_EVENT_TO_THREAD_2    0x03
#define BIC_DEST_THREAD_EVENT_TO_THREAD_3    0x04
#define BIC_DEST_THREAD_EVENT_TO_ALL_THREADS 0x07

// PUEA Interrupt Status Bits
#define PUEA_INTERRUPT_STATUS_L2C(n)  _B1( 30 + (n), 1 )  // L2 Central Event n Pending,  n = 0,..,3
#define PUEA_INTERRUPT_STATUS_WAC     _B1( 36, 1 )        // WAC Guard
#define PUEA_INTERRUPT_STATUS_MU(n)   _B1( 37 + (n), 1 )  // Message Unit Event n Pending, n = 0,..,3
#define PUEA_INTERRUPT_STATUS_GEA(n)  _B1( 41 + (n), 1 )  // Global Event Aggregator Event n Pending, n = 0,..,15
#define PUEA_INTERRUPT_STATUS_L1P(n)  _B1( 57 + (n), 1 )  // L1P Event n Pending, n = 0,..,4
#define PUEA_INTERRUPT_STATUS_C2C(n)  _B1( 62 + (n), 1 )  // Interrupt pending from Core-to-Core register n, n = 0,1

#define PUEA_INTERRUPT_STATUS_L2C_ALL_MASK ( \
          PUEA_INTERRUPT_STATUS_L2C(0) | \
          PUEA_INTERRUPT_STATUS_L2C(1) | \
          PUEA_INTERRUPT_STATUS_L2C(2) | \
          PUEA_INTERRUPT_STATUS_L2C(3) )

#define PUEA_INTERRUPT_STATUS_MU_ALL_MASK ( \
          PUEA_INTERRUPT_STATUS_MU(0) | \
          PUEA_INTERRUPT_STATUS_MU(1) | \
          PUEA_INTERRUPT_STATUS_MU(2) | \
          PUEA_INTERRUPT_STATUS_MU(3) )

#define PUEA_INTERRUPT_STATUS_GEA_ALL_MASK ( \
          PUEA_INTERRUPT_STATUS_GEA(0) | \
          PUEA_INTERRUPT_STATUS_GEA(1) | \
          PUEA_INTERRUPT_STATUS_GEA(2) | \
          PUEA_INTERRUPT_STATUS_GEA(3) | \
          PUEA_INTERRUPT_STATUS_GEA(4) | \
          PUEA_INTERRUPT_STATUS_GEA(5) | \
          PUEA_INTERRUPT_STATUS_GEA(6) | \
          PUEA_INTERRUPT_STATUS_GEA(7) | \
          PUEA_INTERRUPT_STATUS_GEA(8) | \
          PUEA_INTERRUPT_STATUS_GEA(9) | \
          PUEA_INTERRUPT_STATUS_GEA(10) | \
          PUEA_INTERRUPT_STATUS_GEA(11) | \
          PUEA_INTERRUPT_STATUS_GEA(12) | \
          PUEA_INTERRUPT_STATUS_GEA(13) | \
          PUEA_INTERRUPT_STATUS_GEA(14) | \
          PUEA_INTERRUPT_STATUS_GEA(15) )

#define PUEA_INTERRUPT_STATUS_L1P_ALL_MASK ( \
          PUEA_INTERRUPT_STATUS_L1P(0) \
          PUEA_INTERRUPT_STATUS_L1P(1) \
          PUEA_INTERRUPT_STATUS_L1P(2) \
          PUEA_INTERRUPT_STATUS_L1P(3) \
          PUEA_INTERRUPT_STATUS_L1P(4) )

#define PUEA_INTERRUPT_STATUS_C2C_ALL_MASK ( PUEA_INTERRUPT_STATUS_C2C(0) | PUEA_INTERRUPT_STATUS_C2C(1) )



// Memory-mapped register layout for the PUEA Unit 
typedef unsigned long long int BIC_REGISTER;
typedef struct
   {                                               // Address/8   Address Range     Register Name
    volatile BIC_REGISTER _reserved[0x400];        //----------   ---------------   --------------------
    volatile BIC_REGISTER _clear_external_reg_0[4];  // 0x400     0x2000 - 0x201F   C2C Thread 0-3 External Interrupt Clear0
    volatile BIC_REGISTER _clear_critical_reg_0[4];  // 0x404     0x2020 - 0x203F   C2C Thread 0-3 Critical Interrupt Clear0
    volatile BIC_REGISTER _clear_wakeup_reg_0[4];    // 0x408     0x2040 - 0x205F   C2C Thread 0-3 Wake Interrupt Clear0
    volatile BIC_REGISTER _hole_0[4];                // 0x40C     0x2060 - 0x207F   
    volatile BIC_REGISTER _clear_external_reg_1[4];  // 0x410     0x2080 - 0x209F   C2C Thread 0-3 External Interrupt Clear1
    volatile BIC_REGISTER _clear_critical_reg_1[4];  // 0x414     0x20A0 - 0x20BF   C2C Thread 0-3 Critical Interrupt Clear1
    volatile BIC_REGISTER _clear_wakeup_reg_1[4];    // 0x418     0x20c0 - 0x20DF   C2C Thread 0-3 Wake Interrupt Clear1
    volatile BIC_REGISTER __hole_1[4];               // 0x41C     0x20E0 - 0x20FF
    volatile BIC_REGISTER _set_external_reg_0[4];    // 0x420     0x2100 - 0x211F   C2C Thread 0-3 External Interrupt Set0
    volatile BIC_REGISTER _set_critical_reg_0[4];    // 0x424     0x2120 - 0x213F   C2C Thread 0-3 Critical Interrupt Set0
    volatile BIC_REGISTER _set_wakeup_reg_0[4];      // 0x428     0x2140 - 0x215F   C2C Thread 0-3 Wake Interrupt Set0
    volatile BIC_REGISTER _hole_2[4];                // 0x42C     0x2160 - 0x217F
    volatile BIC_REGISTER _set_external_reg_1[4];    // 0x430     0x2180 - 0x219F   C2C Thread 0-3 External Interrupt Set1
    volatile BIC_REGISTER _set_critical_reg_1[4];    // 0x434     0x21A0 - 0x21BF   C2C Thread 0-3 Critical Interrupt Set1
    volatile BIC_REGISTER _set_wakeup_reg_1[4];      // 0x438     0x21C0 - 0x21DF   C2C Thread 0-3 Wake Interrupt Set1
    volatile BIC_REGISTER _hole_3[4];                // 0x43C     0x21E0 - 0x21FF
    volatile BIC_REGISTER _interrupt_send;           // 0x440     0x2200 - 0x2207   C2C Interrupt Send
    volatile BIC_REGISTER _hole_4[7];                // 0x441     0x2208 - 0x223F
    volatile BIC_REGISTER _wakeup_send;              // 0x448     0x2240 - 0x2247   C2C Wake Send
    volatile BIC_REGISTER _hole_5[7];                // 0x449     0x2248 - 0x227F
    volatile BIC_REGISTER _map_interrupt[4];         // 0x450     0x2280 - 0x229F  PUEA Thread 0-3 Interrupt Map
    volatile BIC_REGISTER _ext_int_summary[4];       // 0x454     0x22A0 - 0x22BF  PUEA Thread 0-3 External Interrupt Summary 
    volatile BIC_REGISTER _crit_int_summary[4];      // 0x458     0x22C0 - 0x22DF  PUEA Thread 0-3 Critical Interrupt Summary 
    volatile BIC_REGISTER _mach_int_summary[4];      // 0x45C     0x22E0 - 0x22FF  PUEA Thread 0-3 MachCheck Interrupt Summary
    volatile BIC_REGISTER _input_status;             // 0x460     0x2300 - 0x2307  Input Status Register
} BIC_Device;

// Bit offset within mapping register for PUEA Interrupt Summary bit positions
#define BIC_INT30_MAPOFFSET    1
#define BIC_INT31_MAPOFFSET    3
#define BIC_INT32_MAPOFFSET    5
#define BIC_INT33_MAPOFFSET    7
#define BIC_INT36_MAPOFFSET   13
#define BIC_INT37_MAPOFFSET   15
#define BIC_INT38_MAPOFFSET   17
#define BIC_INT39_MAPOFFSET   19
#define BIC_INT40_MAPOFFSET   21
#define BIC_INT41_MAPOFFSET   23
#define BIC_INT42_MAPOFFSET   25 
#define BIC_INT43_MAPOFFSET   27
#define BIC_INT44_MAPOFFSET   29
#define BIC_INT45_MAPOFFSET   31
#define BIC_INT46_MAPOFFSET   33
#define BIC_INT47_MAPOFFSET   35
#define BIC_INT48_MAPOFFSET   37
#define BIC_INT49_MAPOFFSET   39
#define BIC_INT50_MAPOFFSET   41
#define BIC_INT51_MAPOFFSET   43
#define BIC_INT52_MAPOFFSET   45
#define BIC_INT53_MAPOFFSET   47
#define BIC_INT54_MAPOFFSET   49
#define BIC_INT55_MAPOFFSET   51
#define BIC_INT56_MAPOFFSET   53
#define BIC_INT57_MAPOFFSET   55
#define BIC_INT58_MAPOFFSET   57
#define BIC_INT59_MAPOFFSET   59
#define BIC_INT60_MAPOFFSET   61
#define BIC_INT61_MAPOFFSET   63

// Mapping register and offset associated with the GEA Interrupt Status Reg 0 bit positions
#define GEA_INT0_00_MAPOFFSET  3,39
#define GEA_INT0_01_MAPOFFSET  3,51
#define GEA_INT0_02_MAPOFFSET  4,3
#define GEA_INT0_03_MAPOFFSET  4,19 
#define GEA_INT0_04_MAPOFFSET  4,35
#define GEA_INT0_05_MAPOFFSET  4,51
#define GEA_INT0_06_MAPOFFSET  5,3
#define GEA_INT0_07_MAPOFFSET  5,19
#define GEA_INT0_08_MAPOFFSET  5,35
#define GEA_INT0_09_MAPOFFSET  5,51
#define GEA_INT0_10_MAPOFFSET  6,3
#define GEA_INT0_11_MAPOFFSET  6,19
#define GEA_INT0_12_MAPOFFSET  6,35
#define GEA_INT0_13_MAPOFFSET  6,51
#define GEA_INT0_14_MAPOFFSET  7,3
#define GEA_INT0_15_MAPOFFSET  7,11
#define GEA_INT0_16_MAPOFFSET  7,35
#define GEA_INT0_17_MAPOFFSET  7,51
#define GEA_INT0_18_MAPOFFSET  8,3
#define GEA_INT0_19_MAPOFFSET  1,11
#define GEA_INT0_20_MAPOFFSET  1,35
#define GEA_INT0_21_MAPOFFSET  1,39
#define GEA_INT0_22_MAPOFFSET  3,3
#define GEA_INT0_23_MAPOFFSET  3,7
#define GEA_INT0_24_MAPOFFSET  3,11
#define GEA_INT0_25_MAPOFFSET  3,15
#define GEA_INT0_31_MAPOFFSET  8,35
#define GEA_INT0_32_MAPOFFSET  8,19
#define GEA_INT0_33_MAPOFFSET  8,47 
#define GEA_INT0_34_MAPOFFSET  8,51
#define GEA_INT0_35_MAPOFFSET  8,55
#define GEA_INT0_36_MAPOFFSET  8,59
#define GEA_INT0_37_MAPOFFSET  8,63
#define GEA_INT0_38_MAPOFFSET  9,3
#define GEA_INT0_39_MAPOFFSET  9,7
#define GEA_INT0_40_MAPOFFSET  9,19
#define GEA_INT0_41_MAPOFFSET  9,35
#define GEA_INT0_42_MAPOFFSET  9,47
#define GEA_INT0_43_MAPOFFSET  4,7
#define GEA_INT0_44_MAPOFFSET  4,23
#define GEA_INT0_45_MAPOFFSET  4,39
#define GEA_INT0_46_MAPOFFSET  4,55
#define GEA_INT0_47_MAPOFFSET  5,7
#define GEA_INT0_48_MAPOFFSET  5,23
#define GEA_INT0_49_MAPOFFSET  5,39
#define GEA_INT0_50_MAPOFFSET  5,55
#define GEA_INT0_51_MAPOFFSET  6,7
#define GEA_INT0_52_MAPOFFSET  6,23
#define GEA_INT0_53_MAPOFFSET  6,39
#define GEA_INT0_54_MAPOFFSET  6,55
#define GEA_INT0_55_MAPOFFSET  7,7
#define GEA_INT0_56_MAPOFFSET  7,23
#define GEA_INT0_57_MAPOFFSET  7,39
#define GEA_INT0_58_MAPOFFSET  7,55
#define GEA_INT0_59_MAPOFFSET  8,7
#define GEA_INT0_61_MAPOFFSET  3,59

// Mapping register and offset associated with the GEA Interrupt Status Reg 1 bit positions
#define GEA_INT1_00_MAPOFFSET  3,43    
#define GEA_INT1_01_MAPOFFSET  3,55  
#define GEA_INT1_02_MAPOFFSET  4,11  
#define GEA_INT1_03_MAPOFFSET  4,27     
#define GEA_INT1_04_MAPOFFSET  4,43    
#define GEA_INT1_05_MAPOFFSET  4,59    
#define GEA_INT1_06_MAPOFFSET  5,11    
#define GEA_INT1_07_MAPOFFSET  5,27    
#define GEA_INT1_08_MAPOFFSET  5,43    
#define GEA_INT1_09_MAPOFFSET  5,59    
#define GEA_INT1_10_MAPOFFSET  6,11    
#define GEA_INT1_11_MAPOFFSET  6,27    
#define GEA_INT1_12_MAPOFFSET  6,43    
#define GEA_INT1_13_MAPOFFSET  6,59    
#define GEA_INT1_14_MAPOFFSET  7,11    
#define GEA_INT1_15_MAPOFFSET  7,27    
#define GEA_INT1_16_MAPOFFSET  7,43    
#define GEA_INT1_17_MAPOFFSET  7,59    
#define GEA_INT1_18_MAPOFFSET  8,11    
#define GEA_INT1_19_MAPOFFSET  1,15    
#define GEA_INT1_20_MAPOFFSET  1,39    
#define GEA_INT1_21_MAPOFFSET  1,51    
#define GEA_INT1_27_MAPOFFSET  2,19
#define GEA_INT1_31_MAPOFFSET  8,39    
#define GEA_INT1_32_MAPOFFSET  8,23     
#define GEA_INT1_33_MAPOFFSET  1,31     
#define GEA_INT1_35_MAPOFFSET  8,63    
#define GEA_INT1_39_MAPOFFSET  9,11     
#define GEA_INT1_40_MAPOFFSET  9,23    
#define GEA_INT1_41_MAPOFFSET  9,39    
#define GEA_INT1_42_MAPOFFSET  9,51    
#define GEA_INT1_43_MAPOFFSET  0,3    
#define GEA_INT1_44_MAPOFFSET  0,7    
#define GEA_INT1_45_MAPOFFSET  0,11    
#define GEA_INT1_46_MAPOFFSET  0,15    
#define GEA_INT1_47_MAPOFFSET  0,19    
#define GEA_INT1_48_MAPOFFSET  0,23    
#define GEA_INT1_49_MAPOFFSET  0,27    
#define GEA_INT1_50_MAPOFFSET  0,31    
#define GEA_INT1_51_MAPOFFSET  0,35    
#define GEA_INT1_52_MAPOFFSET  0,39    
#define GEA_INT1_53_MAPOFFSET  0,43    
#define GEA_INT1_54_MAPOFFSET  0,47    
#define GEA_INT1_55_MAPOFFSET  0,51    
#define GEA_INT1_56_MAPOFFSET  0,55     
#define GEA_INT1_57_MAPOFFSET  0,59    
#define GEA_INT1_58_MAPOFFSET  0,63
#define GEA_INT1_60_MAPOFFSET  1,3
#define GEA_INT1_61_MAPOFFSET  9,59   
#define GEA_INT1_62_MAPOFFSET  1,23    

// Mapping register and offset associated with the GEA Interrupt Status Reg 2 bit positions
#define GEA_INT2_00_MAPOFFSET  3,47   
#define GEA_INT2_01_MAPOFFSET  9,31    
#define GEA_INT2_02_MAPOFFSET  4,15    
#define GEA_INT2_03_MAPOFFSET  4,31     
#define GEA_INT2_04_MAPOFFSET  4,47    
#define GEA_INT2_05_MAPOFFSET  4,63    
#define GEA_INT2_06_MAPOFFSET  5,15    
#define GEA_INT2_07_MAPOFFSET  5,31    
#define GEA_INT2_08_MAPOFFSET  5,47    
#define GEA_INT2_09_MAPOFFSET  5,63    
#define GEA_INT2_10_MAPOFFSET  6,15    
#define GEA_INT2_11_MAPOFFSET  6,31    
#define GEA_INT2_12_MAPOFFSET  6,47    
#define GEA_INT2_13_MAPOFFSET  6,63    
#define GEA_INT2_14_MAPOFFSET  7,15    
#define GEA_INT2_15_MAPOFFSET  7,31    
#define GEA_INT2_16_MAPOFFSET  7,47    
#define GEA_INT2_17_MAPOFFSET  7,63    
#define GEA_INT2_18_MAPOFFSET  8,15    
#define GEA_INT2_19_MAPOFFSET  1,19    
#define GEA_INT2_20_MAPOFFSET  1,43    
#define GEA_INT2_21_MAPOFFSET  1,55    
#define GEA_INT2_31_MAPOFFSET  8,43    
#define GEA_INT2_32_MAPOFFSET  8,27     
#define GEA_INT2_33_MAPOFFSET  3,63     
#define GEA_INT2_39_MAPOFFSET  9,15     
#define GEA_INT2_40_MAPOFFSET  9,27    
#define GEA_INT2_41_MAPOFFSET  9,43    
#define GEA_INT2_42_MAPOFFSET  9,55    
#define GEA_INT2_43_MAPOFFSET  3,19    
#define GEA_INT2_44_MAPOFFSET  3,23    
#define GEA_INT2_45_MAPOFFSET  3,27    
#define GEA_INT2_46_MAPOFFSET  3,31    
#define GEA_INT2_47_MAPOFFSET  3,35    
#define GEA_INT2_60_MAPOFFSET  1,7    
#define GEA_INT2_61_MAPOFFSET  9,63    
#define GEA_INT2_62_MAPOFFSET  1,27    

__END_DECLS

#endif
