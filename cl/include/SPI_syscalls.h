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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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
/**
 * \file cnk/SPI_syscalls.h
 */

//
// This file defines the kernel-side of CNK-specific SystemCall handlers,
//  and allows them to be (somewhat) independent of GLIBC's handlers.
//  The reason for this is that CNK's low-core knows about and requires this
//  information, but CNK's GLIBC support can be compile-time disabled.
//
// The user-space-side of these handlers is defined/implemented in CNK's SPI.
//
//
// Each SYSCALL(<name>) is expected to have a function sc_<name> defined in CNK.
//

#ifndef _SPI_SYSCALLS_H_ // Prevent multiple inclusion
#define _SPI_SYSCALLS_H_
#include <errno.h>
#include <asm/unistd.h>
#include <asm/ioctl.h>

#define FIOBGQATOMIC _IOWR(0xa5, 0, unsigned int)
#define SIGDCRVIOLATION 38

#define SYSCALL_START 1024      // Syscall number of first BG/Q SPI system call

__BEGIN_DECLS

// Create __NR_* definitions for userland syscalls
enum
{
    SPI_SYSCALL_BEGIN=SYSCALL_START-1,
#define SYSCALL(name) __NR_##name,
#include "cnk/include/SPI_syscalls.h"
    SPI_NUM_SYSCALLS
};

#ifdef __KERNEL__
// Create function prototypes for CNK
#define SYSCALL(name) uint64_t sc_##name(SYSCALL_FCN_ARGS);
#include "cnk/include/SPI_syscalls.h"
#endif

__END_DECLS

#endif


// Create dummy definition for SYSCALL() if not already defined
#ifndef SYSCALL
#define SYSCALL(name)
#endif

    // IMPORTANT!!!  Place new syscalls at the end of this list to avoid binary incompatibility breakage.  

    SYSCALL(GETMEMORYREGION)                // 1024
    SYSCALL(GET_PERSONALITY)                // 1025
    SYSCALL(SETSPECCONTEXT)                 // 1026 !OBSOLETE
    SYSCALL(ENTERJAILMODE)                  // 1027
    SYSCALL(EXITJAILMODE)                   // 1028
    SYSCALL(SPECALLOCATEDOMAIN)             // 1029
    SYSCALL(SPECSETDOMAINMODE)              // 1030
    SYSCALL(ROLLBACKINDICATOR)              // 1031
    SYSCALL(ALLOCATE_INJ_FIFOS)             // 1032
    SYSCALL(QUERY_INJ_FIFOS)                // 1033
    SYSCALL(QUERY_REC_FIFOS)                // 1034
    SYSCALL(QUERY_BASE_ADDRESS_TABLE)       // 1035
    SYSCALL(ALLOCATE_REC_FIFOS)             // 1036
    SYSCALL(ALLOCATE_BASE_ADDRESS_TABLE)    // 1037
    SYSCALL(REC_FIFO_ENABLE)                // 1038
    SYSCALL(CFG_INJ_FIFO_INTS)              // 1039
    SYSCALL(CFG_REC_FIFO_INTS)              // 1040
    SYSCALL(CFG_INJ_FIFO_THRESH)            // 1041
    SYSCALL(CFG_REC_FIFO_THRESH)            // 1042
    SYSCALL(DEALLOC_INJ_FIFOS)              // 1043
    SYSCALL(DEALLOC_REC_FIFOS)              // 1044
    SYSCALL(DEALLOC_BASE_ADDR_TABLE)        // 1045
    SYSCALL(CFG_MU_GEA)                     // 1046
    SYSCALL(SETL1PPATTERNADDR)              // 1047
    SYSCALL(GETL1PCURPATTERNADDR)           // 1048
    SYSCALL(SETL1PCURPATTERNADDR)           // 1049
    SYSCALL(COMMTHREADPOOF)                 // 1050
    SYSCALL(COMMTHREADINSTALLHANDLER)       // 1051
    SYSCALL(COMMTHREADENABLEINTS)           // 1052
    SYSCALL(SETSPECTHREADMAP)               // 1053 !OBSOLETE
    SYSCALL(COMMTHREADDISABLEINTS)          // 1054
    SYSCALL(RANKS2COORDS)                   // 1055
    SYSCALL(JOBCOORDS)                      // 1056
    SYSCALL(UPCI_OP)                        // 1057
    SYSCALL(SETDEBUGREG)                    // 1058
    SYSCALL(ALLOCATEL2ATOMIC)               // 1059
    SYSCALL(INJECTRAWRAS)                   // 1060
    SYSCALL(INJECTASCIIRAS)                 // 1061
    SYSCALL(TMWRITE)                        // 1062
    SYSCALL(SETSPECSTATE)                   // 1063
    SYSCALL(SETL2SCRUBRATE)                 // 1064
    SYSCALL(QUERYCOLLECTIVECLASSROUTE)      // 1065
    SYSCALL(ALLOCATECOLLECTIVECLASSROUTE)   // 1066
    SYSCALL(SETCOLLECTIVECLASSROUTE)        // 1067
    SYSCALL(DEALLOCATECOLLECTIVECLASSROUTE) // 1068
    SYSCALL(QUERYGINTCLASSROUTE)            // 1069
    SYSCALL(ALLOCATEGINTCLASSROUTE)         // 1070
    SYSCALL(SETGINTCLASSROUTE)              // 1071
    SYSCALL(DEALLOCATEGINTCLASSROUTE)       // 1072
    SYSCALL(CONFIGUREGIINTERRUPTS)          // 1073
    SYSCALL(GETNUMSPECDOMAINS)              // 1074
    SYSCALL(SETNUMSPECDOMAINS)              // 1075
    SYSCALL(GETRANK)                        // 1076
    SYSCALL(GETNUMKERNELSPECDOMAINS)        // 1077
    SYSCALL(GETSPECCONFLICTINFO)            // 1078
    SYSCALL(CLEARSPECCONFLICTINFO)          // 1079
    SYSCALL(GETAXUCR0)                      // 1080
    SYSCALL(SETAXUCR0)                      // 1081
    SYSCALL(GETDEBUGREG)                    // 1082
    SYSCALL(GETMEMORYSIZE)                  // 1083
    SYSCALL(GETFLIGHTLOG)                   // 1084
    SYSCALL(SETLOCALFSWINDOW)               // 1085
    SYSCALL(CLEARINJECTIONCHECKSUM)         // 1086
    SYSCALL(GETJOBID)                       // 1087
    SYSCALL(GETRAWENVMONPOWERDATA)          // 1088
    SYSCALL(GETPVR)                         // 1089
    SYSCALL(ENABLEFASTSPECULATIONPATHS)     // 1090
    SYSCALL(SETPOWERCONSUMPTIONPARAM)       // 1091
    SYSCALL(GETSPECIDSELF)                  // 1092
    SYSCALL(RDMA_CONNECT)                   // 1093
    SYSCALL(RDMA_SEND)                      // 1094
    SYSCALL(RDMA_RECV)                      // 1095
    SYSCALL(RDMA_POLL_CQ)                   // 1096
    SYSCALL(RDMA_REG_MEM)                   // 1097
    SYSCALL(RDMA_DEREG_MEM)                 // 1098
    SYSCALL(SENDX)                          // 1099
    SYSCALL(ISGUESTTHREAD)                  // 1100
    SYSCALL(RDMA_OPEN)                      // 1101
    SYSCALL(GETMAPPING)                     // 1102
    SYSCALL(EXECENVMON2COMMAND)             // 1103
    SYSCALL(GPFSFCNTL)                      // 1104

    // IMPORTANT!!!  Place new syscalls at the end of this list to avoid binary incompatibility breakage.  

#undef SYSCALL

// Wrappers for CNK-specific SPI syscalls.
//
// NOTE1:  These wrappers ignore the syscall failure indication in cr0.so
//         and do not modify errno.  SPI syscalls are expected to use actual
//         return values to indicate success or failure.
//
// NOTE2:  None of the SPI syscalls can block or otherwise lead to a context
//         switch.  All of them therefore preserve the entire QPX, so none of
//         these wrappers need to consider any floating-point registers to be
//         "clobbered".

#define CNK_SPI_SYSCALL_0(name)                                            \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3");                                     \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3)                                                   \
   :   "0"(r0)                                                             \
   : "r4","r5","r6","r7","r8","r9","r10","r11","r12","cr0","memory");      \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_1(name, arg0)                                      \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3)                                                   \
   :   "0"(r0),  "1"(r3)                                                   \
   : "r4","r5","r6","r7","r8","r9","r10","r11","r12","cr0","memory");      \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_2(name, arg0, arg1)                                \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  register uint64_t r4 __asm__ ("r4") = ((uint64_t) (arg1));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3),"=&r"(r4)                                         \
   :   "0"(r0),  "1"(r3),  "2"(r4)                                         \
   : "r5","r6","r7","r8","r9","r10","r11","r12","cr0","memory");           \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_3(name, arg0, arg1, arg2)                          \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  register uint64_t r4 __asm__ ("r4") = ((uint64_t) (arg1));               \
  register uint64_t r5 __asm__ ("r5") = ((uint64_t) (arg2));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3),"=&r"(r4),"=&r"(r5)                               \
   :   "0"(r0),  "1"(r3),  "2"(r4),  "3"(r5)                               \
   : "r6","r7","r8","r9","r10","r11","r12","cr0","memory");                \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_4(name, arg0, arg1, arg2, arg3)                    \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  register uint64_t r4 __asm__ ("r4") = ((uint64_t) (arg1));               \
  register uint64_t r5 __asm__ ("r5") = ((uint64_t) (arg2));               \
  register uint64_t r6 __asm__ ("r6") = ((uint64_t) (arg3));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3),"=&r"(r4),"=&r"(r5),"=&r"(r6)                     \
   :   "0"(r0),  "1"(r3),  "2"(r4),  "3"(r5),  "4"(r6)                     \
   : "r7","r8","r9","r10","r11","r12","cr0","memory");                     \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_5(name, arg0, arg1, arg2, arg3, arg4)              \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  register uint64_t r4 __asm__ ("r4") = ((uint64_t) (arg1));               \
  register uint64_t r5 __asm__ ("r5") = ((uint64_t) (arg2));               \
  register uint64_t r6 __asm__ ("r6") = ((uint64_t) (arg3));               \
  register uint64_t r7 __asm__ ("r7") = ((uint64_t) (arg4));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3),"=&r"(r4),"=&r"(r5),"=&r"(r6),"=&r"(r7)           \
   :   "0"(r0),  "1"(r3),  "2"(r4),  "3"(r5),  "4"(r6),  "5"(r7)           \
   : "r8","r9","r10","r11","r12","cr0","memory");                          \
  r3;                                                                      \
})

#define CNK_SPI_SYSCALL_6(name, arg0, arg1, arg2, arg3, arg4, arg5)        \
({                                                                         \
  register uint64_t r0 __asm__ ("r0") = (__NR_ ## name);                   \
  register uint64_t r3 __asm__ ("r3") = ((uint64_t) (arg0));               \
  register uint64_t r4 __asm__ ("r4") = ((uint64_t) (arg1));               \
  register uint64_t r5 __asm__ ("r5") = ((uint64_t) (arg2));               \
  register uint64_t r6 __asm__ ("r6") = ((uint64_t) (arg3));               \
  register uint64_t r7 __asm__ ("r7") = ((uint64_t) (arg4));               \
  register uint64_t r8 __asm__ ("r8") = ((uint64_t) (arg5));               \
  __asm__ __volatile__                                                     \
  ("sc"                                                                    \
   : "=&r"(r0),"=&r"(r3),"=&r"(r4),"=&r"(r5),"=&r"(r6),"=&r"(r7),"=&r"(r8) \
   :   "0"(r0),  "1"(r3),  "2"(r4),  "3"(r5),  "4"(r6),  "5"(r7),  "6"(r8) \
   : "r9","r10","r11","r12","cr0","memory");                               \
  r3;                                                                      \
})
