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

#ifndef __TESTINT_TDR_H__
#define __TESTINT_TDR_H__

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/common/bgq_bitnumbers.h>

#ifndef __bgq__
#include <hwi/include/bqc/testint_access.h>
#endif

// ACCESS_ID and CFAM_ID register hardcoded read-back values
#define BGQ_JTAG_IDCODE_VALUE_BQC_DD1  (0x09003049LL) // Value of BQC chip DD1.0 IEEE1149.1 IDcode
#define BGQ_JTAG_IDCODE_VALUE_BQC_DD2  (0x29003049LL) // Value of BQC chip DD2.0 IEEE1149.1 IDcode
#define BGQ_JTAG_IDCODE_VALUE_BQL_DD1  (0x01066049LL) // Value of BQL chip DD1.0 IEEE1149.1 IDcode
#define BGQ_JTAG_ACCESS_ID_VALUE       (0x0F002049LL) // Value of internal Access ID reg
#define BGQ_JTAG_CFAM_ID_VALUE         (0x00000049LL) // Value of internal CFAM ID reg

//
// testint tdr selectors are made up of three fields
// 0:7 - -- reserved
// 8:15  -- device select -- master or slave jtag device..
// 16:31 -- tdr length
// 32:63 -- tdr tdr instruction regsiter to use to access the tdr

#define BGQ_JSEL_RAW    0   // no select, no partiy, just raw 32 bit or less ir
#define BGQ_JSEL_NONE   1   // no select, but include parity
#define BGQ_JSEL_ACCESS 2   // ordnal indexes to select access or cfam macros..
#define BGQ_JSEL_CFAM   3

///////////////////////////////////////////////////////////////
/////
// SCOM definitions

// core-slave translation
__INLINE__ unsigned BGQCFAM_CoreToSlave (unsigned core) {
  //    static const unsigned slave[] = {  2,  3,  4,  5,  6,  7,  8,  9, 10,
  //                                      10, 11, 12, 13, 14, 15, 16, 17, 18 };
  static const unsigned slave[] = {  1,  2,  3,  4,  5,  6,  7,  8,  9,
				     10, 11, 12, 13, 14, 15, 17, 18, 19 };

  return (slave[core]);
}

// encoder
#define BGQ_SCOMSEL(x) 		(((((uint64_t)(x))&0x0f)) << (64-4))
#define BGQ_SCOMCHIPLET(x) 	(((((uint64_t)(x))&0x7f)) << (64-11))
#define BGQ_SCOMPORT(x) 	(((((uint64_t)(x))&0x0f)) << (64-15))
#define BGQ_SCOMREG(x) 		(((((uint64_t)(x))&0xffff)) << (64-31))

// decoder
#define _BGQ_SCOMSEL(x) 	(((x) >> (64-4)) & 0x0f)
#define _BGQ_SCOMCHIPLET(x) 	(((x) >> (64-11)) & 0x7f)
#define _BGQ_SCOMPORT(x) 	(((x) >> (64-15)) & 0x0f)
#define _BGQ_SCOMREG(x) 	(((x) >> (64-31)) & 0xffff)

// SCOM address definition,
// s - select (0 for none)
// c -- chiplet (incl. multicast-bit)
// p -- port
// r -- register
#define BGQ_SCOMDEF(s,c,p,r)  (BGQ_SCOMSEL(s) | BGQ_SCOMCHIPLET(c)	\
                               | BGQ_SCOMPORT(p) | BGQ_SCOMREG(r))

// Impossible address as End-of-List for writeScomMultiple
#define WRITESCOMMULT_ENDOFLIST_CORE    0x3F
#define WRITESCOMMULT_ENDOFLIST         BGQ_SCOMDEF(0x0F,		\
                                                    WRITESCOMMULT_ENDOFLIST_CORE, \
                                                    0x0F, 0xFFFF)

// PCB master: internal addresses
#define BGQ_TI_SCOM_CFAM_MS_CNT_REF(X)          \
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, (X))	// X = 0-6
#define BGQ_TI_SCOM_CFAM_MS_BIT_SEL		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x08)
#define BGQ_TI_SCOM_CFAM_MS_REC_ACK		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x10)
#define BGQ_TI_SCOM_CFAM_MS_REC_ERR		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x11)
#define BGQ_TI_SCOM_CFAM_MS_BRDCST_COMP_SEL	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x12)
#define BGQ_TI_SCOM_CFAM_MS_BRDCST_COMP_VAL	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x13)
#define BGQ_TI_SCOM_CFAM_MS_BRDCST_COMP_MASK	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x14)
#define BGQ_TI_SCOM_CFAM_MS_1ST_REP_LOG		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x15)
#define BGQ_TI_SCOM_CFAM_MS_TIMEOUT_CNTR_MATCH_VAL	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x16)
#define BGQ_TI_SCOM_CFAM_MS_ASS_INT_TYPE	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x17)
#define BGQ_TI_SCOM_CFAM_MS_IGNORE_PAR		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x1c)
#define BGQ_TI_SCOM_CFAM_MS_RESET		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x1d)
#define BGQ_TI_SCOM_CFAM_MS_1ST_ERR		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x1e)
#define BGQ_TI_SCOM_CFAM_MS_ERR_LOG		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, 0, 0xf, 0x1f)

// PCB slave: common internal addresses
#define BGQ_TI_SCOM_CFAM_SLV_PRIM_ADR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x00)
#define BGQ_TI_SCOM_CFAM_SLV_MCAST_GRP_1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x01)
#define BGQ_TI_SCOM_CFAM_SLV_MCAST_GRP_2(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x02)
#define BGQ_TI_SCOM_CFAM_SLV_MCAST_GRP_3(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x03)
#define BGQ_TI_SCOM_CFAM_SLV_TIMEOUT_SEL(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x10)
#define BGQ_TI_SCOM_CFAM_SLV_ASS_INT(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x11)
#define BGQ_TI_SCOM_CFAM_SLV_GP3_WRITE(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x12)
#define BGQ_TI_SCOM_CFAM_SLV_GP3_WAND(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x13)
#define BGQ_TI_SCOM_CFAM_SLV_GP3_WOR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x14)
#define BGQ_TI_SCOM_CFAM_SLV_ATT(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x1A)
#define BGQ_TI_SCOM_CFAM_SLV_RECOV_ERR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x1B)
#define BGQ_TI_SCOM_CFAM_SLV_CHECKSTOP(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x1C)
#define BGQ_TI_SCOM_CFAM_SLV_ERR_CAPT(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x1F)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_1(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x20)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_2(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x21)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_3(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x22)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_4(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x23)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_5(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x24)
#define BGQ_TI_SCOM_CFAM_SLV_HANG_PULSE_6(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x25)
#define BGQ_TI_SCOM_CFAM_SLV_PMC_NOTIF(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x30)
#define BGQ_TI_SCOM_CFAM_SLV_PM_ERR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x31)
#define BGQ_TI_SCOM_CFAM_SLV_PM_ERR_MASK(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x32)
#define BGQ_TI_SCOM_CFAM_SLV_PM_ERR_FIR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x33)
#define BGQ_TI_SCOM_CFAM_SLV_PM_STATE_VEC(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x34)
#define BGQ_TI_SCOM_CFAM_SLV_SPCL_WAKE(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0xf, 0x35)

// (selected) GP3 bits
#define GP3_TP_TC_CHIPLET_EN_DC			0x8000000000000000ULL
#define GP3_TP_TC_PCB_EP_RESET_DC		0x4000000000000000ULL
#define GP3_SYNC_DISABLE			0x2000000000000000ULL
#define GP3_TP_TC_NEST_NCLK_NODIV_DC		0x0400000000000000ULL
#define GP3_TP_TC_VTL_SCAN_DC			0x0200000000000000ULL
#define GP3_TP_TC_VTL_SCAN_IN_DC		0x0100000000000000ULL
#define GP3_TP_TC_VTL_THOLD_DC			0x0000800000000000ULL
#define GP3_TP_TC_FENC_EN_DC			0x0000200000000000ULL

// PCB-slave: GPIO endpoint
#define BGQ_TI_SCOM_CFAM_GPIO_GPOUT0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x0, 0x0000)
#define BGQ_TI_SCOM_CFAM_GPIO_GPIN0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x0, 0x0001)
#define BGQ_TI_SCOM_CFAM_GPIO_INTR_MASK(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x0, 0x0002)

// (selected) GP0 bits
#define GP0_TC_ABSTCLK_MUXSEL_DC		0x8000000000000000ULL
#define GP0_TC_SYNCCLK_MUXSEL_DC		0x4000000000000000ULL
#define GP0_TC_FLUSHMODE_INHIBIT		0x2000000000000000ULL
#define GP0_TC_FORCE_ALIGN                      0x1000000000000000ULL
#define GP0_TC_L1P_FENCE_DC                     0x0400000000000000ULL
#define GP0_TC_LBIST_EN_DC			0x0080000000000000ULL
#define GP0_TC_LBIST_AC_MODE_DC			0x0040000000000000ULL
#define GP0_TC_LBIST_ARY_WRT_THRU_DC		0x0020000000000000ULL
#define GP0_TC_ABIST_MODE_DC			0x0010000000000000ULL
#define GP0_TC_ABIST_START_TEST_DC		0x0008000000000000ULL
#define GP0_TC_CC_SCAN_DIS_DC_B                 0x0004000000000000ULL
#define GP0_TC_AC_THREAD_STOP_0			0x0000800000000000ULL
#define GP0_TC_AC_THREAD_STOP_1			0x0000400000000000ULL
#define GP0_TC_AC_THREAD_STOP_2			0x0000200000000000ULL
#define GP0_TC_AC_THREAD_STOP_3			0x0000100000000000ULL
#define GP0_TC_THREAD_STOP_FROM_TI_B		0x0000080000000000ULL
#define GP0_TC_FENCE_PERV_DC			0x0000000000000001ULL

// (selected) GP1 bits
#define GP1_TC_ABIST_DONE_DC			0x8000000000000000ULL
#define GP1_TC_OPCG_DONE_DC			0x0001000000000000ULL

// PCB-slave: PSCOMLE endpoint -- internal (ring 0, satellite 0)
#define BGQ_TI_SCOM_CFAM_PSCOMLE_INT_MODE(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0000)
#define BGQ_TI_SCOM_CFAM_PSCOMLE_INT_STATUS_ERR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0001)
#define BGQ_TI_SCOM_CFAM_PSCOMLE_INT_ERR_MASK(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0002)
#define BGQ_TI_SCOM_CFAM_PSCOMLE_INT_SER_TRAP(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0003)
#define BGQ_TI_SCOM_CFAM_PSCOMLE_INT_DBG_ERR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0004)

// PCB-slave: PSCOMLE endpoint -- A2 SCOM (ring 1, satellite 0)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0400)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_AND(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0401)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_OR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0402)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0A0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0403)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0A1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0404)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_MASK(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0406)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_MASK_AND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0407)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR0_MASK_OR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0408)

#define BGQ_TI_SCOM_CFAM_A2SCOM_ERRINJ(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0409)

#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040A)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1_AND(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040B)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1_OR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040C)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1A0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040D)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1A1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040E)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1_MASK(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0410)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1_MASK_AND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0411)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR1_MASK_OR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0412)

#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0414)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2_AND(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0415)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2_OR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0416)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2A0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0417)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2A1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0418)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2_MASK(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x041A)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2_MASK_AND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x041B)
#define BGQ_TI_SCOM_CFAM_A2SCOM_FIR2_MASK_OR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x041C)

#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMIC(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0428)
#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMI(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0429)

#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMC(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042A)
#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMC_WAND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042B)
#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMC_WOR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042C)

#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMD(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042D)
#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMDH(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042E)
#define BGQ_TI_SCOM_CFAM_A2SCOM_RAMDL(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x042F)

#define BGQ_TI_SCOM_CFAM_A2SCOM_THRCTL(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0430)
#define BGQ_TI_SCOM_CFAM_A2SCOM_THRCTL_WAND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0431)
#define BGQ_TI_SCOM_CFAM_A2SCOM_THRCTL_WOR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0432)

#define BGQ_TI_SCOM_CFAM_A2SCOM_PCCR0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0433)
#define BGQ_TI_SCOM_CFAM_A2SCOM_PCCR0_WAND(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0434)
#define BGQ_TI_SCOM_CFAM_A2SCOM_PCCR0_WOR(SLAVE)	\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0435)

#define BGQ_TI_SCOM_CFAM_A2SCOM_ABDSR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x043B)
#define BGQ_TI_SCOM_CFAM_A2SCOM_IDSR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x043C)
#define BGQ_TI_SCOM_CFAM_A2SCOM_MPDSR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x043D)
#define BGQ_TI_SCOM_CFAM_A2SCOM_XDSR1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x043E)
#define BGQ_TI_SCOM_CFAM_A2SCOM_XDSR2(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x043F)


// PCB-slave: ClockController endpoint
#define BGQ_TI_SCOM_CFAM_CC_SYNC_CONF(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0000)
#define BGQ_TI_SCOM_CFAM_CC_PHASE_SHADOW(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0001)
#define BGQ_TI_SCOM_CFAM_CC_OPCG_REG0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0002)
#define BGQ_TI_SCOM_CFAM_CC_OPCG_REG1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0003)
#define BGQ_TI_SCOM_CFAM_CC_OPCG_REG2(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0004)
#define BGQ_TI_SCOM_CFAM_CC_OPCG_REG3(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0005)
#define BGQ_TI_SCOM_CFAM_CC_CLK_REGION(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0006)
#define BGQ_TI_SCOM_CFAM_CC_SCANSELQ(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0007)
#define BGQ_TI_SCOM_CFAM_CC_CLK_STAT(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0008)
#define BGQ_TI_SCOM_CFAM_CC_ERR_STAT(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x3, 0x0009)

// (selected) OPCG_REG0 bits
#define OPCG0_RUNN_MODE				0x8000000000000000ULL
#define OPCG0_OPCG_GO				0x4000000000000000ULL
#define OPCG0_RUN_SCAN0				0x2000000000000000ULL

// (selected) CLK_REGION bits
#define CFAM_CLK_REGION_NOP			                 0ULL
#define CFAM_CLK_REGION_START			0x4000000000000000ULL
#define CFAM_CLK_REGION_STOP			0x8000000000000000ULL
#define CFAM_CLK_REGION_PULSE			0xC000000000000000ULL

#define CFAM_CLK_REGION_VTL			0x1000000000000000ULL
#define CFAM_CLK_REGION_PERV			0x0800000000000000ULL
#define CFAM_CLK_REGION_CORE			0x0400000000000000ULL

#define CFAM_CLK_REGION_SCAN			0x0000080000000000ULL
#define CFAM_CLK_REGION_NONSCAN			0x0000040000000000ULL
#define CFAM_CLK_REGION_ARY			0x0000020000000000ULL

// PCB-slave: FIR endpoint
#define BGQ_TI_SCOM_CFAM_FIR_CHKSTOP(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0000)
#define BGQ_TI_SCOM_CFAM_FIR_RECOV_ERR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0001)
#define BGQ_TI_SCOM_CFAM_FIR_FIR_MASK(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0002)
#define BGQ_TI_SCOM_CFAM_FIR_SPATTN(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0004)
#define BGQ_TI_SCOM_CFAM_FIR_SPATTN_MASK(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0007)
#define BGQ_TI_SCOM_CFAM_FIR_LFIR_ERR(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x000A)
#define BGQ_TI_SCOM_CFAM_FIR_LFIR_MASK(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x0010)
#define BGQ_TI_SCOM_CFAM_FIR_LFIR_A0(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x000D)
#define BGQ_TI_SCOM_CFAM_FIR_LFIR_A1(SLAVE)		\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x4, 0x000E)
#define BGQ_TI_SCOM_CFAM_FIR0(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0400)
#define BGQ_TI_SCOM_CFAM_FIR1(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040A)
#define BGQ_TI_SCOM_CFAM_FIR2(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0414)
#define BGQ_TI_SCOM_CFAM_FIR0_A0(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0403)
#define BGQ_TI_SCOM_CFAM_FIR1_A0(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040D)
#define BGQ_TI_SCOM_CFAM_FIR2_A0(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0417)
#define BGQ_TI_SCOM_CFAM_FIR0_A1(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0404)
#define BGQ_TI_SCOM_CFAM_FIR1_A1(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x040E)
#define BGQ_TI_SCOM_CFAM_FIR2_A1(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0418)
#define BGQ_TI_SCOM_CFAM_FIR0M(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0406)
#define BGQ_TI_SCOM_CFAM_FIR1M(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x0410)
#define BGQ_TI_SCOM_CFAM_FIR2M(SLAVE)			\
  BGQ_SCOMDEF(BGQ_JSEL_CFAM, (SLAVE), 0x1, 0x041A)

// PCB-slave multicast
//   t - multicast type
//   g - multicast group
#define BGQ_PCB_MULTSLV(t,g)	(0x40 | (((t) & 7) << 3) | ((g) & 7))
#define BGQ_PCB_MCST_GROUP(g,p) (0xE000000000000000ULL			\
                                 | (((uint64_t)(g) & 0x07) << (63 - 5))	\
                                 | (((uint64_t)(p) & 0x07) << (63 - 21)))

#define BGQ_PCB_MULT_READ_OR		0x00
#define BGQ_PCB_MULT_READ_AND		0x01
#define BGQ_PCB_MULT_READ_BIT_X		0x02
#define BGQ_PCB_MULT_READ_COMPARE	0x04
#define BGQ_PCB_MULT_WRITE		0x05

#define BGQ_PCB_MULT_WRITEALL	BGQ_PCB_MULTSLV(BGQ_PCB_MULT_WRITE, 7)

#define BGQ_PCB_SPARE		19


////////////////////////////////////////////////////////////////////////////////////////////
// DEVBUS_CNTL1 definitions
//

#define BGQ_TI_DEVBUS_CNTL2_ENA  _BN(0)    // 0 -- assume DEVBUS_CNTL2 is defaulted to 0xFFFF 0006 0000 0000
                                           // 1 -- apply actual DEVBUS_CNTL2 settings
#define BGQ_TI_DEVBUS_MASTER     _BN(4)    // 0 -- use Debus Slave, fast access to beDRAM, PCIe, BIC, UPC, L2 control,
                                           // 1 -- use devbus master, slower access to any memory address
#define BGQ_TI_DEVBUS_PRE_INH    _BN(5)    // prefetch inhibit.
                                           // When set to 1, data is not prefetched into DEVBUS_DATA on
                                           //DEVBUS_CNTL update or DEVBUS_DATA capture
#define BGQ_TI_DEVBUS_INC_INH    _BN(6)    // Auto-increment inhibit
                                           // When set to 1, the address in bits 22:63 is not incremented on
                                           // DEVBUS_DATA update/capture.
#define BGQ_TI_DEVBUS_RW         _BN(7)    // 0 = Read from Devbus address
                                           // 1 = Write to Devbus address
#define BGQ_TI_DEVBUS_ERR_ENA    _BN(10)   // Error clear enable
                                           // If this bit is written as a 1, error bits 11:15 in this register will be updated with
                                           // the value they are being written to. If this bit is written as a 0, error bits will
                                           // remain unchanged for this particular write. This bit will always be read as 0.
#define BGQ_TI_DEVBUS_ERR_RD_CE  _BN(11)   // Read data Correctable ECC error
                                           // This bit will be set to a 1 if a correctable ECC error is detected on data returned from the Devbus.
                                           // This bit is "sticky", i.e. it persists until cleared by software by writing this bit to a 0 while bit 10 (error clear enable) is on.
                                           // However, unlike the errors below, this bit does *not* block further use of the interface,
                                           // and is not transmitted to Glob_Att / Alert_Out pin.
#define BGQ_TI_DEVBUS_ERR_RD_UE  _BN(12)   // Read data Uncorrectable ECC error
                                           // This bit will be set to a 1 if an uncorrectable ECC error is detected on data returned from the Devbus.
                                           // This bit is "sticky", i.e. it persists until cleared by software by writing this bit to a 0 while bit 10 (error clear enable) is on.
                                           // Occurrence of this error is signaled to the Glob_Att logic / Alert_Out pin,
                                           // and blocks further use of the JTAG_DEVBUS interface until cleared.
#define BGQ_TI_DEVBUS_ERR_DBRD  _BN(13)    // Devbus read error
                                           // This bit will be set to a 1 if the Devbus detects an error on a read access. This
                                           // bit is "sticky", i.e. it persists until cleared by software by writing this bit to a 0
                                           // while bit 10 (error clear enable) is on.
                                           // Occurrence of this error is signaled to the Glob_Att logic / Alert_Out pin,
                                           // and blocks further use of the JTAG_DEVBUS interface until cleared.
#define BGQ_TI_DEVBUS_ERR_WR    _BN(14)    // Devbus write error
                                           // This bit will be set to a 1 if the Devbus detects an error on a write access.
                                           // This bit is "sticky", i.e. it persists until cleared by software by writing this bit
                                           // to a 0 while bit 10 (error clear enable) is on.
                                           // Occurrence of this error is signaled to the Glob_Att logic / Alert_Out pin,
                                           // and blocks further use of the JTAG_DEVBUS interface until cleared.
#define BGQ_TI_DEVBUS_ERR_DB_OVR _BN(15)   // JTAG/Devbus overrun
                                           // This bit will be set to a 1 if JTAG attempts a request before Devbus services a
                                           // request that's already outstanding. This bit is "sticky", i.e. it persists until
                                           // cleared by software by writing this bit to a 0 while bit 10 (error clear enable)
                                           // is on.
                                           // Occurrence of this error is signaled to the Glob_Att logic / Alert_Out pin,
                                           // and blocks further use of the JTAG_DEVBUS interface until cleared.
#define BGQ_TI_DEVBUS_ERR(x)     _B5(15,x) // Error bits = DEVBUS_CNTL1(11 to 15)

#define BGQ_TI_DEVBUS_ADDR(x)    _B42(63,x)// DEVBUS_CNTL1(22 to 63) =_Address for subsequent DEVBUS access operations.
                                           // Only bits 30:63 are actually transmitted. 22:29 are assumed by Devbus to be '1's
                                           // (i.e. any bits written to 22:29 are ignored and replaced by "1111 1111".)
                                           // Address is auto-incremented. See auto-increment description in doc.
                                           // Legal addresses for access via this mechanism are given in the Memory Map:
                                           // https://yktgsa.ibm.com/projects/b/bgq/web/design/memory_map.html
                                           // Address ranges labeled DBS can be accessed via the DCR Slave (bit4 = 0);
                                           // address ranges labeled DBM or DBS can be accessed via the DCR master
                                           // (bit4 = 1).
                                           // Access to another address than specified above will lead to a Devbus Read or
                                           // Write error (i.e. bit 13 or 14 will be set).

////////////////////////////////////////////////////////////////////////////////////////////
// DEVBUS_CNTL2 definitions
//
#define BGQ_TI_DEVBUS_WBYTE_ENA(x) _B16(15,x) // DEVBUS_CNTL2(0 to 15) = Byte Write Enables for byte 0 (MSB) to 15 (LSB) of DEVBUS_DATA.
                                              // On Update: 1 = Enable, 0=Disable Write of the corresponding byte.
                                              // On Capture: can be ignored.
#define BGQ_TI_DEVBUS_SIZE_ENC(x) _B3(31,x)   // Special size encoding for reads and writes. Follows the A2 size encoding
                                              // format <reference>.
                                              // b'000' = 64 bytes (illegal over this interface)
                                              // b'001' = 1 byte
                                              // b'010' = 2 bytes
                                              // b'011' = 128 bytes (illegal over this interface)
                                              // b'100' = 4 bytes
                                              // b'101' = 8 bytes
                                              // b'110' = 16 bytes (default)
                                              // b'111' = 32 bytes (illegal over this interface)
                                              // For the illegal cases (32, 64, 128 bytes), no transaction will be executed, and
                                              // the Devbus read or write error bit (DEVBUS_CNTL1(13 or 14) will be set.
                                              // b'110'
#define BGQ_TI_DEVBUS_SPEC_ID(x) _B8(63,x)    // L2 cache SPEC_ID. The ID used for speculative multi-threading or for the
                                              // generations tag. See Memory System documentation <reference>.


// global attention bits
#define BQC_TI_GLOB_ATT_OVERTEMP            _BN(49)
#define BQC_TI_GLOB_ATT_CRIT_OVERTEMP       _BN(50)
#define BGQ_TI_GLOB_ATT_DB_ERR              _BN(51)
#define BGQ_TI_GLOB_ATT_DCR_ERR             _BN(52)
#define BGQ_TI_GLOB_ATT_TI_PARITY_ERR       _BN(53)
#define BGQ_TI_GLOB_ATT_CYCLE_REPRO_RESET   _BN(54)
#define BQC_TI_GLOB_ATT_USER1               _BN(55)
#define BQC_TI_GLOB_ATT_USER2               _BN(56)
#define BQC_TI_GLOB_ATT_USER3               _BN(57)
#define BQC_TI_GLOB_ATT_USER4               _BN(58)
#define BQC_TI_GLOB_ATT_USER5               _BN(59)
#define BGQ_TI_GLOB_ATT_MB_MSG_FROM_COMPUTE _BN(61) // mailbox register 0 is not equal to mailbox register 1.

#endif
