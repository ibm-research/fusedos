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

/*------------------------------------------------------------------*/
/* Peter Boyle, Edinburgh University (for IBM), Jan 24 2009         */
/* Additional terms in letter agreement dated July 21,2011.         */
/*  See CoO for details.                                            */
/*------------------------------------------------------------------*/

#ifndef _L1P_MMIO_H_
#define _L1P_MMIO_H_

#include <hwi/include/common/bgq_bitnumbers.h>

/********************************************************
 * MMIO test read/write region 128 bytes of 64 bit regs
 ********************************************************
 */
#define L1P_MMIO_DEBUG_LO  (0x3ffe8001800ll)
#define L1P_MMIO_DEBUG_HI  (0x3ffe800187Fll)
#define L1P_MMIO_DISCARD   (0x3ffe80000c0)
/****************************************
 * Speculation mode relevant controls
 ****************************************
 */
#define L1P_CFG_SPEC (0x3ffe8000100ll)      /* (64 bit reg, 3 bits used, unpriv)                      */
#define L1P_CFG_SPEC_pf_hit_fwd_l2     _B1(61,1) /* (forward hits in l1p to L2)   */
#define L1P_CFG_SPEC_pf_dfc_disable    _B1(62,1) /* (disable demand fetch convert)*/
#define L1P_CFG_SPEC_l1_hit_fwd_l2     _B1(63,1) /* (forward hits from L1 to L2)  */

/****************************************
 * User mode alterable prefetch controls
 ****************************************
 */
#define L1P_CFG_PF_USR  (0x3ffe8000108ll)   /*  (64 bit reg, 23 bits wide, user/unpriv) */
#define L1P_CFG_PF_USR_dfetch_depth(x)          _B3(42,x) 
#define L1P_CFG_PF_USR_dfetch_max_footprint(x)  _B4(46,x)
#define L1P_CFG_PF_USR_ifetch_depth(x)          _B3(49,x)
#define L1P_CFG_PF_USR_ifetch_max_footprint(x)  _B4(53,x)
#define L1P_CFG_PF_USR_pf_stream_est_on_dcbt    _B1(54,1)
#define L1P_CFG_PF_USR_pf_stream_optimistic     _B1(55,1)
#define L1P_CFG_PF_USR_pf_stream_prefetch_enable  _B1(56,1)
#define L1P_CFG_PF_USR_pf_stream_establish_enable _B1(57,1)
#define L1P_CFG_PF_USR_pf_list_enable           _B1(58,1)
#define L1P_CFG_PF_USR_pf_adaptive_enable       _B1(59,1)
#define L1P_CFG_PF_USR_pf_adaptive_throttle(x)  _B4(63,x)

/*****************************************
 * System only prefetch controls
 *****************************************
 */
#define L1P_CFG_PF_SYS (0x3ffe8000110ll) /* (64 bit reg, 20 bits wide, priv)*/
#define L1P_CFG_PF_SYS_OFFSET (0x0110) /* offset from L1P base */
#define L1P_CFG_PF_SYS_msync_timer(x)             _B4(47,x)
#define L1P_CFG_PF_SYS_pfhint_enable              _B1(48,1)
#define L1P_CFG_PF_SYS_whint_evict_enable         _B1(49,1)
#define L1P_CFG_PF_SYS_whint_cracked_enable       _B1(50,1)
#define L1P_CFG_PF_SYS_lock_prefetch              _B1(51,1)
#define L1P_CFG_PF_SYS_lock_l1_only               _B1(52,1)
#define L1P_CFG_PF_SYS_lock_l2_set_lc             _B1(53,1)
#define L1P_CFG_PF_SYS_dcbfl_discard              (0x0)
#define L1P_CFG_PF_SYS_wrap_bug_dd2_bhv           _B1(54,1)
#define L1P_CFG_PF_SYS_pf_adaptive_total_depth(x) _B5(59,x)
#define L1P_CFG_PF_SYS_pf_adaptive_reset          _B1(60,1)
#define L1P_CFG_PF_SYS_pf_invalidate_all          _B1(61,1)
#define L1P_CFG_PF_SYS_pf_hit_enable              _B1(62,1)
#define L1P_CFG_PF_SYS_pf_stream_l2_op_immediate  _B1(63,1)

/*****************************************
 * Write combine controls - sys only
 *****************************************
 */
#define L1P_CFG_WC (0x3ffe8000118ll) /*  (64 bit reg, 3 bits wide, priv)*/
#define L1P_CFG_WC_wc_enable                _B1(61,1)
#define L1P_CFG_WC_wc_suppress_if_all_be    _B1(62,1)
#define L1P_CFG_WC_wc_aging                 _B1(63,1)

/******************************************
 * Timeout unit controls
 ******************************************
 */

#define L1P_CFG_TO (0x3ffe8000120ll) /*  (64 bit reg, 6 bits wide, priv,1)*/
#define L1P_CFG_TO_to_en          _B1(58,1)
#define L1P_CFG_TO_to_reload_en   _B1(59,1)
#define L1P_CFG_TO_to_duration(x) _B4(63,x)


/***************************
 *Clock gate control
 ***************************
 */
#define L1P_CFG_CLK_GATE (0x3ffe8000128ll) // Was clock gate, now generic chicken
#define L1P_CFG_CHICKEN  (0x3ffe8000128ll)

#define L1P_CFG_CLK_GATE_DD1 (0x0)
#define L1P_CFG_CHICKEN_DD2  (L1P_CFG_CHICKEN_spec_alias_dd2_bhv \
			     |L1P_CFG_CHICKEN_pf_list_hline_dd2_bhv \
			     |L1P_CFG_CHICKEN_pf_l2_op_dd2_bhv	    \
			     |L1P_CFG_CHICKEN_checkstop_int_dd2_bhv \
			     |L1P_CFG_CHICKEN_spec_info_dd2_bhv	    \
			     |L1P_CFG_CHICKEN_spec_dfc1_dd2_bhv	    \
			     |L1P_CFG_CHICKEN_spec_dfc2_dd2_bhv      )

#define L1P_CFG_CLK_GATE_clk_off_list   _B1(48,1) // 0
#define L1P_CFG_CLK_GATE_clk_on_list    _B1(49,1)
#define L1P_CFG_CLK_GATE_clk_on_lu      _B1(50,1)
#define L1P_CFG_CLK_GATE_clk_on_pfd     _B1(51,1)
#define L1P_CFG_CLK_GATE_clk_on_sw_req  _B1(52,1)
#define L1P_CFG_CLK_GATE_clk_on_mmio    _B1(53,1)
#define L1P_CFG_CLK_GATE_clk_on_srt     _B1(54,1) 
#define L1P_CFG_CHICKEN_spec_alias_dd1_bhv     _B1(55,1) 
#define L1P_CFG_CHICKEN_spec_alias_dd2_bhv     (0x0)
#define L1P_CFG_CHICKEN_pf_sd_miss_dd1_bhv     _B1(56,1) 
#define L1P_CFG_CHICKEN_pf_list_hline_dd2_bhv  _B1(57,1) 
#define L1P_CFG_CHICKEN_pf_l2_op_dd2_bhv       _B1(58,1) 
#define L1P_CFG_CHICKEN_spec_int_dd1_bhv       _B1(59,1) 
#define L1P_CFG_CHICKEN_checkstop_int_dd2_bhv  _B1(60,1) 
#define L1P_CFG_CHICKEN_spec_info_dd2_bhv      _B1(61,1) 
#define L1P_CFG_CHICKEN_spec_dfc1_dd2_bhv      _B1(62,1) 
#define L1P_CFG_CHICKEN_spec_dfc2_dd2_bhv      _B1(63,1) 


#define L1P_CFG_CLK_GATE_clk_spare4     _B1(59,1) // 7
#define L1P_CFG_CLK_GATE_clk_spare3     _B1(60,1) 
#define L1P_CFG_CLK_GATE_clk_spare2     _B1(61,1)
#define L1P_CFG_CLK_GATE_clk_spare1     _B1(62,1)
#define L1P_CFG_CLK_GATE_clk_spare0     _B1(63,1) //11

/***********************************
 * UPC control
 ***********************************
 */
#define L1P_CFG_UPC (0x3ffe8000130ll)
#define L1P_CFG_UPC_ENABLE    _B1(61,1)
#define L1P_CFG_UPC_SELECT(x) _B2(63,x)
#define L1P_CFG_UPC_LIST   L1P_CFG_UPC_SELECT(0x2)
#define L1P_CFG_UPC_SWITCH L1P_CFG_UPC_SELECT(0x1)
#define L1P_CFG_UPC_STREAM L1P_CFG_UPC_SELECT(0x0)



/*********************************************
 * Interrupt controls
 *********************************************
 */
#define L1P_ESR                  (0x3ffe8000000ll) /*(30 bits) (priv)*/
#define L1P_ESR_BIC_REALTIME     (0x3ffe8000008ll) /*(30 bits) (priv)*/
#define L1P_ESR_BIC_CRITICAL     (0x3ffe8000010ll) /*(30 bits) (priv)*/
#define L1P_ESR_BIC_NONCRITICAL  (0x3ffe8000018ll) /*(30 bits) (priv)*/
#define L1P_ESR_BIC_MACHINECHECK (0x3ffe8000020ll) /*(30 bits) (priv)*/
#define L1P_ESR_GEA              (0x3ffe8000028ll) /*(30 bits) (priv)*/
#define L1P_ESR_INJ              (0x3ffe8000030ll) /*(30 bits) (priv)*/
#define L1P_ESR_HINJ             (0x3ffe8000038ll) /*(30 bits) (priv)*/

#define L1P_ESR_a2_machine_check      _B1(0,1)
#define L1P_ESR_err_reload_p          _B1(1,1)
#define L1P_ESR_int_list_0		_B1(2,1)
#define L1P_ESR_int_list_1		_B1(3,1)
#define L1P_ESR_int_list_2		_B1(4,1)
#define L1P_ESR_int_list_3		_B1(5,1)
#define L1P_ESR_int_list_4		_B1(6,1)
#define L1P_ESR_int_speculation_0	_B1(7,1)
#define L1P_ESR_int_speculation_1	_B1(8,1)
#define L1P_ESR_int_speculation_2      	_B1(9,1)
#define L1P_ESR_int_speculation_3       _B1(10,1)
#define L1P_ESR_err_luq_ovfl		_B1(11,1)
#define L1P_ESR_err_sr_p		_B1(12,1)
#define L1P_ESR_err_sr_rd_valid_p	_B1(13,1)
#define L1P_ESR_err_sw_p		_B1(14,1)
#define L1P_ESR_err_si_ecc		_B1(15,1)
#define L1P_ESR_err_si_ecc_ue		_B1(16,1)
#define L1P_ESR_err_si_p		_B1(17,1)
#define L1P_ESR_err_sda_p		_B1(18,1)
#define L1P_ESR_err_sda_p_ue		_B1(19,1)
#define L1P_ESR_err_rqra_p		_B1(20,1)
#define L1P_ESR_err_reload_ecc_x2	_B1(21,1)
#define L1P_ESR_err_reload_ecc_ue_x2	_B1(22,1)
#define L1P_ESR_err_rira_p		_B1(23,1)
#define L1P_ESR_err_gctr_p		_B1(24,1)
#define L1P_ESR_err_lu_state_p		_B1(25,1)
#define L1P_ESR_err_lu_ttype		_B1(26,1)
#define L1P_ESR_err_lu_dcr_abort	_B1(27,1)
#define L1P_ESR_err_mmio_async		_B1(28,1)
#define L1P_ESR_err_mmio_state_p	_B1(29,1)
#define L1P_ESR_err_mmio_timeout	_B1(30,1)
#define L1P_ESR_err_mmio_priv		_B1(31,1)
#define L1P_ESR_err_mmio_rdata_p	_B1(32,1)
#define L1P_ESR_err_mmio_wdata_p	_B1(33,1)
#define L1P_ESR_err_mmio_dcrs_timeout	_B1(34,1)
#define L1P_ESR_err_mmio_dcrs_priv	_B1(35,1)
#define L1P_ESR_err_mmio_dcrs_par	_B1(36,1)
#define L1P_ESR_err_dcrm_crit		_B1(37,1)
#define L1P_ESR_err_dcrm_noncrit	_B1(38,1)
#define L1P_ESR_err_dcrm_mc		_B1(39,1)
#define L1P_ESR_err_tag_timeout		_B1(40,1)
#define L1P_ESR_err_valid_timeout	_B1(41,1)
#define L1P_ESR_err_hold_timeout	_B1(42,1)
#define L1P_ESR_err_ditc_req_x2		_B1(43,1)
#define L1P_ESR_err_pfd_addr_p		_B1(44,1)
#define L1P_ESR_err_pfd_avalid_p	_B1(45,1)
#define L1P_ESR_err_pfd_fill_pnd_p	_B1(46,1)
#define L1P_ESR_err_pfd_hit_pnd_p	_B1(47,1)
#define L1P_ESR_err_pfd_stream_p	_B1(48,1)
#define L1P_ESR_err_pfd_depth_p		_B1(49,1)
#define L1P_ESR_err_pfd_clone_p		_B1(50,1)
#define L1P_ESR_err_hitq_p		_B1(51,1)
#define L1P_ESR_err_sd_p		_B1(52,1)
#define L1P_ESR_err_pf2dfc_p		_B1(53,1)
#define L1P_ESR_err_wccm_p_x2		_B1(54,1)
#define L1P_ESR_err_wccm_wcd_p_x2	_B1(55,1)
#define L1P_ESR_err_lu_wcd_p		_B1(56,1)
#define L1P_ESR_err_lu_current_p	_B1(57,1)
#define L1P_ESR_err_l2cmd		_B1(58,1)
#define L1P_ESR_err_lu_dcr_dbus_p	_B1(59,1)
#define L1P_ESR_err_luq_p		_B1(60,1)
#define L1P_ESR_err_sda_phase_p		_B1(61,1)
#define L1P_ESR_slice_sel_ctrl_perr	_B1(62,1)
#define L1P_ESR_redun_ctrl_perr         _B1(63,1)


/***********************************************************************
 * Per thread speculation id. User mode.
 ***********************************************************************
 */

#define L1P_THR_SPECULATION_SELF   (0x3ffe8000040ll)
#define L1P_THR_SPECULATION_SELF_OFFSET (0x0040) /* offset from L1P base */
#define L1P_THR_SPECULATION_TID(n) (0x3ffe8000060ll + 8*n)
#define L1P_THR_SPECULATION_TID_0  (0x3ffe8000060ll)
#define L1P_THR_SPECULATION_TID_1  (0x3ffe8000068ll)
#define L1P_THR_SPECULATION_TID_2  (0x3ffe8000070ll)
#define L1P_THR_SPECULATION_TID_3  (0x3ffe8000078ll) 

#define L1P_THR_SPECULATION_val(x) _B9(63,x)


#ifndef __ASSEMBLY__

typedef struct
{
    volatile uint64_t  speculationIdSelf;        /* speculation_id (self) */
    volatile uint64_t  reserved[3];              /* padding */
    volatile uint64_t  speculationIdTid[4];      /* speculation_id (TID_0...TID_3) */

} L1P_THR_SPECULATION_t;

#endif // !__ASSEMBLY__




/***********************************************************************
 * Write combine max age. Per thread, priv
 ***********************************************************************
 */

#define L1P_THR_WC_AGE_MAX_SELF   (0x3ffe8000080ll)
#define L1P_THR_WC_AGE_MAX_TID_0  (0x3ffe80000a0ll)
#define L1P_THR_WC_AGE_MAX_TID_1  (0x3ffe80000a8ll) 
#define L1P_THR_WC_AGE_MAX_TID_2  (0x3ffe80000b0ll)
#define L1P_THR_WC_AGE_MAX_TID_3  (0x3ffe80000b8ll)

#define L1P_THR_WC_AGE_MAX_val(x) _B4(63,x)

/***********************************************************************
 * Debug register - discard loads to specific address
 ***********************************************************************
 */
#define L1P_DISCARD  (0x3ffe80000c0ll)
#define L1P_DISCARD_address(A) _B42(63,A)
#define L1P_DISCARD_enable     _B1(21,1)

/***********************************************************************
 * MMIO Perfect Prefetcher Registers
 ***********************************************************************
 */
#define L1P_PP_THREADOFFSET    0x040
#define L1P_PP_CTRL            0x1000
#define L1P_PP_CTRL_DEPTH(x)   _B3(55,x)
#define L1P_PP_CTRL_MAXLIST(x) _B3(58,x)
#define L1P_PP_CTRL_INHIBIT    _B1(59,1)
#define L1P_PP_CTRL_IGNORETOL  _B1(60,1)
#define L1P_PP_CTRL_START      _B1(61,1)
#define L1P_PP_CTRL_LOAD       _B1(62,1)
#define L1P_PP_CTRL_PAUSE      _B1(63,1)

#define L1P_PP_STATUS           0x1028
#define L1P_PP_STATUS_FINISHED  _B1(60,1)
#define L1P_PP_STATUS_ABANDONED _B1(61,1)
#define L1P_PP_STATUS_MAXLENGTH _B1(62,1)
#define L1P_PP_STATUS_ENDOFLIST _B1(63,1)

#define L1P_PP_MAXTOL           0x1008
#define L1P_PP_MAXSIZE          0x1010
#define L1P_PP_ADDR_READ        0x1018
#define L1P_PP_ADDR_WRITE       0x1020
#define L1P_PP_ADDR_CURWRITE    0x1038
#define L1P_PP_ADDR_CURREAD     0x1030

/********************************
 * DCR view
 ********************************
 */
#define L1P_DCR_BASE           (0x3fde0010000ll)
#define L1P_DCR_CORE_SEL(C)    (C<<17)
#define L1P_PRIV(A)            ((A>>33)&0x1)
#define L1P_DCR_ADDR(A,C)  (((A)&0xFFFF) | L1P_DCR_BASE | L1P_DCR_CORE_SEL(C) | ((A)&(0x1ULL<<33)) )


#define L1P_MMIO_DEBUG_LO_DCR(C) L1P_DCR_ADDR(L1P_MMIO_DEBUG_LO,C)
#define L1P_MMIO_DEBUG_HI_DCR(C) L1P_DCR_ADDR(L1P_MMIO_DEBUG_HI,C)
#define L1P_MMIO_DISCARD_DCR(C) L1P_DCR_ADDR(L1P_MMIO_DISCARD,C)
#define L1P_CFG_SPEC_DCR(C) L1P_DCR_ADDR(L1P_CFG_SPEC,C)
#define L1P_CFG_PF_USR_DCR(C) L1P_DCR_ADDR(L1P_CFG_PF_USR,C)
#define L1P_CFG_PF_SYS_DCR(C) L1P_DCR_ADDR(L1P_CFG_PF_SYS,C)
#define L1P_CFG_WC_DCR(C) L1P_DCR_ADDR(L1P_CFG_WC,C)
#define L1P_CFG_TO_DCR(C) L1P_DCR_ADDR(L1P_CFG_TO,C)
#define L1P_CFG_UPC_DCR(C) L1P_DCR_ADDR(L1P_CFG_UPC,C)
#define L1P_CFG_CLK_GATE_DCR(C) L1P_DCR_ADDR(L1P_CFG_CLK_GATE,C)
#define L1P_ESR_DCR(C) L1P_DCR_ADDR(L1P_ESR,C)
#define L1P_ESR_BIC_REALTIME_DCR(C) L1P_DCR_ADDR(L1P_ESR_BIC_REALTIME,C)
#define L1P_ESR_BIC_CRITICAL_DCR(C) L1P_DCR_ADDR(L1P_ESR_BIC_CRITICAL,C)
#define L1P_ESR_BIC_NONCRITICAL_DCR(C) L1P_DCR_ADDR(L1P_ESR_BIC_NONCRITICAL,C)
#define L1P_ESR_BIC_MACHINECHECK_DCR(C) L1P_DCR_ADDR(L1P_ESR_BIC_MACHINECHECK,C)
#define L1P_ESR_GEA_DCR(C) L1P_DCR_ADDR(L1P_ESR_GEA,C)
#define L1P_ESR_INJ_DCR(C) L1P_DCR_ADDR(L1P_ESR_INJ,C)
#define L1P_ESR_HINJ_DCR(C) L1P_DCR_ADDR(L1P_ESR_HINJ,C)
#define L1P_THR_SPECULATION_SELF_DCR(C) L1P_DCR_ADDR(L1P_THR_SPECULATION_SELF,C)
#define L1P_THR_SPECULATION_TID_0_DCR(C) L1P_DCR_ADDR(L1P_THR_SPECULATION_TID_0,C)
#define L1P_THR_SPECULATION_TID_1_DCR(C) L1P_DCR_ADDR(L1P_THR_SPECULATION_TID_1,C)
#define L1P_THR_SPECULATION_TID_2_DCR(C) L1P_DCR_ADDR(L1P_THR_SPECULATION_TID_2,C)
#define L1P_THR_SPECULATION_TID_3_DCR(C) L1P_DCR_ADDR(L1P_THR_SPECULATION_TID_3,C)
#define L1P_THR_WC_AGE_MAX_SELF_DCR(C) L1P_DCR_ADDR(L1P_THR_WC_AGE_MAX_SELF,C)
#define L1P_THR_WC_AGE_MAX_TID_0_DCR(C) L1P_DCR_ADDR(L1P_THR_WC_AGE_MAX_TID_0,C)
#define L1P_THR_WC_AGE_MAX_TID_1_DCR(C) L1P_DCR_ADDR(L1P_THR_WC_AGE_MAX_TID_1,C)
#define L1P_THR_WC_AGE_MAX_TID_2_DCR(C) L1P_DCR_ADDR(L1P_THR_WC_AGE_MAX_TID_2,C)
#define L1P_THR_WC_AGE_MAX_TID_3_DCR(C) L1P_DCR_ADDR(L1P_THR_WC_AGE_MAX_TID_3,C)
#define L1P_DCR_BASE_DCR(C) L1P_DCR_ADDR(L1P_DCR_BASE,C)


#endif
