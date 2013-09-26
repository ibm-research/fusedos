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

#ifndef	_MUSPI_TERMCHECK__H_ /* Prevent multiple inclusion */
#define	_MUSPI_TERMCHECK__H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/mu_dcr.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <hwi/include/bqc/nd_rese_dcr.h>
#include <hwi/include/bqc/nd_x2_dcr.h>
//#include <spi/include/kernel/debug.h>
#include <spi/include/kernel/MU.h>
#include <hwi/include/bqc/testint_inlines.h>


__BEGIN_DECLS

typedef struct MUSPI_TERMCHECK
{
    uint64_t (*getAddress)(int i);
    uint64_t (*getValueA)(int i);
    uint64_t (*getValueB)(int i);
    uint64_t (*getMask)(int i);
    uint32_t arrayLength;
} MUSPI_TERMCHECK_t;

typedef struct MUSPI_TERMCHECK_REGLIST
{
  uint64_t addr;
  uint64_t mask;

} MUSPI_TERMCHECK_REGLIST_t;


#define MUSPI_ND_DIR_IO 10
#define MUSPI_ND_500_NUM_FATAL_ERR_DCRs 6
#define MUSPI_ND_500_NUM_FATAL_ERR_DCRs   6
#define MUSPI_ND_500_NUM_RCP_FIFO_P_DCRs  4
#define MUSPI_ND_500_NUM_INJ_FIFO_P_DCRs  2
#define MUSPI_ND_500_NUM_STAT_DCRs        10
#define ND_X2_NUM_STAT_DCRs         2


Personality_t _MUSPI_TERMCHECK_Personality;

__INLINE__ int MUSPI_TERMCHECK_LoadPersonality( void ) {
    return Kernel_GetPersonality( &_MUSPI_TERMCHECK_Personality, sizeof(Personality_t) );
}


__INLINE__ int MUSPI_TERMCHECK_IsSenderEnabled(int i) {
    return ( _MUSPI_TERMCHECK_Personality.Network_Config.NetFlags2 & ( ND_ENABLE_LINK_A_MINUS >> i ) ) != 0;
}

/*__INLINE__*/ uint64_t getND500_FATAL_ERRn_Addr(int i) {
    return ND_500_DCR(FATAL_ERR0) + i;
}

/*__INLINE__*/ uint64_t getND500_FATAL_ERRn(int i) {
    return DCRReadPriv( ND_500_DCR(FATAL_ERR0) + i );

}

/*__INLINE__*/ uint64_t  getND500_FATAL_ERR_Mask(int i) {
  return DCRReadPriv(ND_500_DCR(FATAL_ERR_ENABLE)+i);

}

/*__INLINE__*/ uint64_t getNDX2_FATAL_ERRn_Addr(int i) {
    return ND_X2_DCR(FATAL_ERR0) + (i*2);
}

/*__INLINE__*/ uint64_t getNDX2_FATAL_ERRn(int i) {
  return DCRReadPriv( ND_X2_DCR(FATAL_ERR0) + (i*2) );  // fatal_err_enable has address right before fata_err
}

/*__INLINE__*/ uint64_t getNDX2_FATAL_ERR_Mask(int i) {
  return DCRReadPriv( ND_X2_DCR(FATAL_ERR0_ENABLE) + (i*2) );  // fatal_err_enable has address right after fatal_err
}



/*__INLINE__*/ uint64_t getNDRESE_FATAL_ERRn_Addr(int i) {
    return ND_RESE_DCR(i,FATAL_ERR);
}

/*__INLINE__*/ uint64_t getNDRESE_FATAL_ERRn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,FATAL_ERR) );
}

/*__INLINE__*/ uint64_t getNDRESE_FATAL_ERR_Mask(int i) {

    uint64_t mask = 0;

    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {

	// the last bit in this dcr is a nonfatal error, indicating
	//  a dropped packet so mask that off
       mask =   DCRReadPriv(ND_RESE_DCR(i,FATAL_ERR_ENABLE));
       mask = ~ _BN(63);  

	uint64_t ctrl_val = DCRReadPriv( ND_RESE_DCR(i, CTRL) );

	if ( 
	    (  ND_RESE_DCR__CTRL__DYN_TOK_REDUCE_get(ctrl_val) ) || 
	    (  ND_RESE_DCR__CTRL__BUBBLE_TOK_REDUCE_get(ctrl_val) ) ||
	    (  ND_RESE_DCR__CTRL__COLL_TOK_REDUCE_get(ctrl_val) )
	    )
	{
	   
	    if ( TI_isDD1() == 0 ) {
		// On DD2 if determisitic token reduction is done after unit is taken out of reset
		// the token overflow interrupt will fire, so mask out this possibility
		// TODO:  If the token reduction is ever needed for production, this
		// masking should be removed.
		mask &= ~ ND_RESE_DCR__FATAL_ERR__SE_TOKEN_OVERFLOW_set(1);
	    }
	}    
    }
    return mask;
}

/*__INLINE__*/ uint64_t getZero( int i ) {
    return 0;
}

/*__INLINE__*/ uint64_t getND500_RCP_FIFO_Addr(int i) {
    return ND_500_DCR(RCP_FIFO_HEAD_P0) + i;
}

/*__INLINE__*/ uint64_t getND500_RCP_FIFO_Head(int i) {
    return DCRReadPriv( ND_500_DCR(RCP_FIFO_HEAD_P0) + i );
}

/*__INLINE__*/ uint64_t getND500_RCP_FIFO_Tail(int i) {
    return DCRReadPriv( ND_500_DCR(RCP_FIFO_TAIL_P0) + i );
}

/*__INLINE__*/ uint64_t getND500_INJ_FIFO_Addr(int i) {
    return ND_500_DCR(INJ_FIFO_HEAD_P0) + i;
}

/*__INLINE__*/ uint64_t getND500_INJ_FIFO_Head(int i) {
    return DCRReadPriv( ND_500_DCR(INJ_FIFO_HEAD_P0) + i );
}

/*__INLINE__*/ uint64_t getND500_INJ_FIFO_Tail(int i) {
    return DCRReadPriv( ND_500_DCR(INJ_FIFO_TAIL_P0) + i );
}

/*__INLINE__*/ uint64_t getAllBitsMask(int i) {
    return -1;
}


/*__INLINE__*/ uint64_t getND500_RCP_FIFOm(int i) {

  uint64_t m = -1ULL;
  switch(i)
  {
  case 0: // _p0 contains fifos 0 - 3
  case 1: // _p1 contains fifos 4 - 6,
  case 2: // _p2 contains fifos 8, 9, L0, L1 
    break;
 
  default: // _p3 contains fifos hp, io, c0 , c1, mask out io and c1
    ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_IO_HEAD_insert(m,0);
    ND_500_DCR__RCP_FIFO_HEAD_P3__RCP_C1_HEAD_insert(m,0);
    break;
    
  }
  return m;
}

/*__INLINE__*/ uint64_t getND500_INJ_FIFOm(int i) {

  uint64_t m = -1ULL;
  switch(i)
  {
  case 0: // _p0 contains fifos 0 - 7
    break;
 
  default: // _p1 contains fifos 8,9, L0, L1, hp, io, c0 , c1, mask out io and c1
    ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_IO_HEAD_insert(m,0);
    ND_500_DCR__INJ_FIFO_HEAD_P1__INJ_C1_HEAD_insert(m,0);
    break;
    
  }
  return m;
}



/*__INLINE__*/ uint64_t getMCSRAM_BD_FETCHING_DESC_Addr(int i) {
  return MU_DCR(MCSRAM_BD_FETCHING_DESC);
}

/*__INLINE__*/ uint64_t getMCSRAM_BD_FETCHING_DESC(int i) {
  return DCRReadPriv(MU_DCR(MCSRAM_BD_FETCHING_DESC)); 
}

/*__INLINE__*/ uint64_t getMCSRAM_BD_FETCHING_DESCm(int i) {
  return MU_DCR__MCSRAM_BD_FETCHING_DESC__USR_set(1);
}


/*__INLINE__*/ uint64_t getRME_STATE0_Addr(int i) {
  return MU_DCR(RME_STATE0) + i;
}

/*__INLINE__*/ uint64_t getRME_STATE0(int i) {
  return DCRReadPriv(MU_DCR(RME_STATE0) + i); 
}


/*__INLINE__*/ uint64_t getRME_STATE1_Addr(int i) {
  return MU_DCR(RME_STATE1) + i;
}

/*__INLINE__*/ uint64_t getRME_STATE1(int i) {
  return DCRReadPriv(MU_DCR(RME_STATE1) + i); 
}

/*__INLINE__*/ uint64_t getRME_STATE1m(int i) {
  // binary 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111 1111 1111 1000 0011 1110 0000 = 0x000000ffffff83e0
  return 0x000000ffffff83e0UL;
}

/*__INLINE__*/ uint64_t getIME_STATE_Addr(int i) {
  return MU_DCR(IME_STATE) + i;
}

/*__INLINE__*/ uint64_t getIME_STATE(int i) {
  return DCRReadPriv(MU_DCR(IME_STATE) + i); 
}

/*__INLINE__*/ uint64_t getIME_STATEm(int i) {
  // binary 0000 0000 0000 0000 0011 1111 1111 1111 1111 1111 1111 1111 1111 1110 0011 1000 = 0x00003ffffffffe38
  return 0x00003ffffffffe38UL;
}

/*__INLINE__*/ uint64_t getIME_STATEe(int i) {
  // binary 0000 0000 0000 0000 0010 0100 1001 0010 0100 1001 0010 0100 1001 0010 0000 1000 = 0x0000249249249208
  return 0x0000249249249208UL;
}

// returns bit mask in which _BN(x), _BN(x+1), ..., _BN(y) are 1s
#define MUSPI_TERMCHECK_BN_XTOY(x, y) ((_BN(x) << 1) - _BN(y))

MUSPI_TERMCHECK_REGLIST_t _MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS[] = {
  { MU_DCR(MASTER_PORT0_INTERRUPTS__STATE), MUSPI_TERMCHECK_BN_XTOY(24, 25) },
  { MU_DCR(MASTER_PORT1_INTERRUPTS__STATE), MUSPI_TERMCHECK_BN_XTOY(24, 25) },
  { MU_DCR(MASTER_PORT2_INTERRUPTS__STATE), MUSPI_TERMCHECK_BN_XTOY(24, 25) },
  { MU_DCR(SLAVE_PORT_INTERRUPTS__STATE),   MUSPI_TERMCHECK_BN_XTOY(21, 27) },
  { MU_DCR(MMREGS_INTERRUPTS__STATE),       MUSPI_TERMCHECK_BN_XTOY(9,  18) },

#if 0
  // The rmfifo_insufficient_space bit is not a serious condition.  It is normal for this to happen in code that 
  // properly uses the SPI; the SPI handles the situation properly.  So we will ignore it
  { MU_DCR(FIFO_INTERRUPTS__STATE),         _BN(63) },
#endif

  { MU_DCR(MCSRAM_INTERRUPTS__STATE),       MUSPI_TERMCHECK_BN_XTOY(15, 27) | MUSPI_TERMCHECK_BN_XTOY(31, 32) },
  { MU_DCR(RCSRAM_INTERRUPTS__STATE),       MUSPI_TERMCHECK_BN_XTOY(12, 15) },
  { MU_DCR(RPUTSRAM_INTERRUPTS__STATE),     MUSPI_TERMCHECK_BN_XTOY(8, 13) },
  { MU_DCR(RME_INTERRUPTS0__STATE),         _BN(5)|_BN(11)|_BN(17)|_BN(23)|_BN(29)|_BN(35)|_BN(41)|_BN(47) },
  { MU_DCR(RME_INTERRUPTS1__STATE),         _BN(4)|_BN(10)|_BN(16)|_BN(22)|_BN(28)|_BN(34)|_BN(40)|_BN(46) },
  { MU_DCR(ICSRAM_INTERRUPTS__STATE),       MUSPI_TERMCHECK_BN_XTOY(20, 22)  },
  { MU_DCR(MISC_INTERRUPTS__STATE),         MUSPI_TERMCHECK_BN_XTOY(3, 5)  },

};

#undef MUSPI_TERMCHECK_BN_XTOY

/*__INLINE__*/ uint64_t getMU_SOFTWARE_ERRn_Addr(int i) {
  return _MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS[i].addr;
}

/*__INLINE__*/ uint64_t getMU_SOFTWARE_ERRn(int i) {
  return DCRReadPriv(_MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS[i].addr);
}

/*__INLINE__*/ uint64_t  getMU_SOFTWARE_ERR_Mask(int i) {
  return  _MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS[i].mask;
}

/*
  <rasevent 
    id="000D0003"
    category="Message_Unit"
    component="SPI"
    severity="WARN"
    message="MU Network termination check has failed.  Details: $(DETAILS)"
    description="A network termination check has failed.  This error indicates that one or more unexpected conditions exists in the network hardware state.  The conditions are serious and may indicate an integrity problem with data that passed through the network."
    control_action="END_JOB,FREE_COMPUTE_BLOCK"
    service_action="$(Diagnostics)"
    relevant_diags="processor,torus"
    decoder="fw_MU_termCheckDecoder"
    />
 */

/*__INLINE__*/ void MUSPI_ND_TERMCHECK_Miscompare( uint64_t actual, uint64_t expected, uint64_t dcrAddress, uint64_t mask ) {
    printf("(ERROR) actual=%016lX expected=%016lX addr=%016lX mask=%016lX [%s:%d]\n", actual & mask, expected & mask, dcrAddress, mask, __func__, __LINE__ );
    uint64_t details[16];
    details[0] = dcrAddress;
    details[1] = actual;
    details[2] = expected;
    details[3] = mask;
    Kernel_InjectRAWRAS( 0xD0003, 4*sizeof(uint64_t), details );
}


__INLINE__ int MUSPI_ND_TERMCHECK_step( MUSPI_TERMCHECK_t* term ) {

    int rc = 0;
    unsigned i;

    for ( i = 0; i < term->arrayLength; i++ ) {

	uint64_t a = term->getValueA(i);
	uint64_t b = term->getValueB(i);
	uint64_t m = term->getMask(i);

	//printf ("%s a=%016lX b=%016lX addr=%016lX mask=%016lX\n", __func__, a, b, term->getAddress(i), m );
	
	if ( (a&m) != (b&m) ) {
	    MUSPI_ND_TERMCHECK_Miscompare(a, b, term->getAddress(i), m );
	    rc++;
	}
    }

    return rc;
}

MUSPI_TERMCHECK_t _MUSPI_ND_TERMCHECK_FATAL_ERRORS[] = {
    { getND500_FATAL_ERRn_Addr,  getND500_FATAL_ERRn,   getZero, getND500_FATAL_ERR_Mask,  MUSPI_ND_500_NUM_FATAL_ERR_DCRs },
    { getNDX2_FATAL_ERRn_Addr,   getNDX2_FATAL_ERRn,    getZero, getNDX2_FATAL_ERR_Mask ,            2 },
    { getNDRESE_FATAL_ERRn_Addr, getNDRESE_FATAL_ERRn,  getZero, getNDRESE_FATAL_ERR_Mask, ND_RESE_DCR_num },
};

/*__INLINE__*/ uint64_t getND500_STAT_Addr(int i) {
    return ND_500_DCR(STAT0) + i;
}

/*__INLINE__*/ uint64_t getND500_STATn(int i) {
    if ( ( i < 4 ) || ( i  > 7 ) ) {
	return DCRReadPriv( ND_500_DCR(STAT0) + i );
    }
    else {
	return 0;
    }
}


/*__INLINE__*/ uint64_t getND500_STATe(int i) {

  uint64_t m = 0;
  switch(i)
  {
  case 0: // stat 0, injection fifo arbiter state machine, should be idle (state 1), except for io (l0, l1 and c0 and c1 not in this DCR)
    m =   ND_500_DCR__STAT0__INJ_T0_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T1_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T2_ARB_STATE_set(1)
	| ND_500_DCR__STAT0__INJ_T3_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T4_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T5_ARB_STATE_set(1)
	| ND_500_DCR__STAT0__INJ_T6_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T7_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_T8_ARB_STATE_set(1)
	| ND_500_DCR__STAT0__INJ_T9_ARB_STATE_set(1) |   ND_500_DCR__STAT0__INJ_HP_ARB_STATE_set(1);
    break;
    
  case 1: // stat1 - reception fifo network state machines,  sould be three bits 001 for all fifos except io and collectives 
    m  = ND_500_DCR__STAT1__RCP_T0_ND_STATE_set(1)   |  ND_500_DCR__STAT1__RCP_T1_ND_STATE_set(1) | ND_500_DCR__STAT1__RCP_T2_ND_STATE_set(1)
	 | ND_500_DCR__STAT1__RCP_T3_ND_STATE_set(1) |  ND_500_DCR__STAT1__RCP_T4_ND_STATE_set(1) | ND_500_DCR__STAT1__RCP_T5_ND_STATE_set(1) 
	 | ND_500_DCR__STAT1__RCP_T6_ND_STATE_set(1) |  ND_500_DCR__STAT1__RCP_T7_ND_STATE_set(1) | ND_500_DCR__STAT1__RCP_T8_ND_STATE_set(1) 
	 | ND_500_DCR__STAT1__RCP_T9_ND_STATE_set(1) |  ND_500_DCR__STAT1__RCP_L0_ND_STATE_set(1) | ND_500_DCR__STAT1__RCP_L1_ND_STATE_set(1)
	 | ND_500_DCR__STAT1__RCP_HP_ND_STATE_set(1) | ND_500_DCR__STAT1__RCP_C0_ND_STATE_set(1) ;
    break;

  default:
    // stat2, injection fifo network state machines T0 to L1, should be 0
    // stat3, injection fifo network state machines HP to C1, except for io and c1, which will be masked
    // stat4, T0 to T3 header check errors, should be 0
    // stat5, T4 to T7 header check errors, should be 0
    // stat6, T8 to IO header check errors, should be 0
    // stat7, C0 and C1 header check errors, should be 0
    m =0;
   break;
  }

  return m;
}


/*__INLINE__*/ uint64_t getND500_STATm(int i) {

  uint64_t m = -1ULL;
  switch(i)
  {

  case 0: // stat 0, injection fifo arbiter state machine, should be idle (state 1), except for io (l0, l1 and c0 and c1 not in this DCR)
    ND_500_DCR__STAT0__INJ_IO_ARB_STATE_insert(m,0);
    break;
    
  case 1: // stat1 - reception fifo network state machines,  should be three bits 001 for all fifos except io and collectives 
    ND_500_DCR__STAT1__RCP_IO_ND_STATE_insert(m,0);
    ND_500_DCR__STAT1__RCP_C1_ND_STATE_insert(m,0);
    break;

  case 3: // stat3, injection fifo network state machines HP to C1, except for io and c1, which will be masked
    ND_500_DCR__STAT3__INJ_IO_ND_STATE_insert(m,0);
    ND_500_DCR__STAT3__INJ_C1_ND_STATE_insert(m,0);
    break;
    
  default:
    break;
  }
  return m;
}



/*__INLINE__*/ uint64_t getNDX2_STAT_Addr(int i) {
    return ND_X2_DCR(STAT0) + i;
}

/*__INLINE__*/ uint64_t getNDX2_STATn(int i) {
    return DCRReadPriv( ND_X2_DCR(STAT0) + i );
}

/*__INLINE__*/ uint64_t getNDX2_STATm(int i) {
  uint64_t m = -1ULL;
  if ( i ==0) 
  {
    ND_X2_DCR__STAT0__RCP_IO_MU_STATE_insert(m,0);
    ND_X2_DCR__STAT0__RCP_C1_MU_STATE_insert(m,0);
  }
 
  return m;
}

/*__INLINE__*/ uint64_t getNDRESE_INTERNAL_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_INTERNAL_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_INTERNAL_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_INTERNAL_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_INTERNAL_STATEe(int i) {
    return  0x0011110000000000ULL;
}

/*__INLINE__*/ uint64_t getNDRESE_INTERNAL_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return 0x00FFFFFFFFFFFFFFULL;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_PKT_CNT_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC0_PKT_CNT);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_PKT_CNTn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC0_PKT_CNT) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_PKT_CNTm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1ULL;
    }
    else {
	return 0;
    }
}


/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_STATE_Addr(int i) {

    return ND_RESE_DCR(i,RE_VC0_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC0_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_STATEe(int i) {
 
    if ( i < MUSPI_ND_DIR_IO ) {
	return 0x1111111110000000ULL;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1ULL;
    }
    else {
	return 0;
    }
}


/*__INLINE__*/ uint64_t getNDRESE_RE_VC1_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC1_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC1_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC1_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC1_STATEe(int i) {
 
    if ( i < MUSPI_ND_DIR_IO ) {
	return 0x0011111110000000ULL;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC1_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1ULL;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC2_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC2_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC2_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC2_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC2_STATEe(int i) {
 
    if ( i < MUSPI_ND_DIR_IO ) {
	return 0x0011111110000000ULL;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC2_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1ULL;
    }
    else {
	return 0;
    }
}


/*__INLINE__*/ uint64_t getNDRESE_RE_VC3_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC3_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC3_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC3_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC3_STATEe(int i) {
 
    //if ( i < MUSPI_ND_DIR_IO ) {
    return 0x0011111110000000ULL;
	//}
	//else {
	//return 0;
	//}
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC3_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1ULL;
    }
    else {
	return 0;
    }
}


/*__INLINE__*/ uint64_t getNDRESE_RE_VC4_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC4_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC4_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC4_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC4_STATEe(int i) {

    //     b=0x0000110000000000 , the 1's are the idle state
    return ND_RESE_DCR__RE_VC4_STATE__CTRL_WR_set(1) |  ND_RESE_DCR__RE_VC4_STATE__CTRL_RD_set(1);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC4_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	// For collectives were seeing miscompares from idle state, eg
	// Error: a != b addr=0x0000000000184039 line=247   a=0x0000113000000000 b=0x0000111000000000
	// the difference is because  constant S_RD2: std_ulogic_vector(0 to 3) := "0011"; is ok, this is in
	// the S_RD2 can be a valid state, if a link errors caused a false collective packet type being seen by the bypass logic.  
	// If this false packet is the last one seen by a collective VC fifo, i.e., no good collective packets follow after that, 
	// then that particular state machine could stay in S_RD2 state.
 
	// ND_RESE_DCR__RE_VC4_STATE__VALUE_width  0-7 is valid 0, 8 - 11 head, 12-15 tail (head = tail)
	//   3 4 bit 1s, 9 4 bit 0's
	uint64_t mask =  0xFFFFFFFFFFFFFFFFULL; // mask out head tail,fifo_rd 
	ND_RESE_DCR__RE_VC4_STATE__CTRL_HEAD_insert(mask,0);
	ND_RESE_DCR__RE_VC4_STATE__CTRL_TAIL_insert(mask,0);      
	ND_RESE_DCR__RE_VC4_STATE__FIFO_RD_insert(mask,0);

	return mask;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_STATE_Addr(int i) {
    return ND_RESE_DCR(i,RE_VC5_STATE);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_STATEn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,RE_VC5_STATE) );
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_STATEe(int i) {

    if ( i < MUSPI_ND_DIR_IO ) {
	//     b=0x0000110000000000 , the 1's are the idle state
	return ND_RESE_DCR__RE_VC5_STATE__CTRL_WR_set(1) |  ND_RESE_DCR__RE_VC5_STATE__CTRL_RD_set(1);
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_STATEm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	// For collectives were seeing miscompares from idle state, eg
	// Error: a != b addr=0x0000000000184039 line=247   a=0x0000113000000000 b=0x0000111000000000
	// the difference is because  constant S_RD2: std_ulogic_vector(0 to 3) := "0011"; is ok, this is in
	// the S_RD2 can be a valid state, if a link errors caused a false collective packet type being seen by the bypass logic.  
	// If this false packet is the last one seen by a collective VC fifo, i.e., no good collective packets follow after that, 
	// then that particular state machine could stay in S_RD2 state.
 
	// ND_RESE_DCR__RE_VC5_STATE__VALUE_width  0-7 is valid 0, 8 - 11 head, 12-15 tail (head = tail)
	//   3 4 bit 1s, 9 4 bit 0's
	uint64_t mask =  0xFFFFFFFFFFFFFFFFULL; // mask out head tail,fifo_rd 
	ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_insert(mask,0);
	ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_insert(mask,0);      
	ND_RESE_DCR__RE_VC5_STATE__FIFO_RD_insert(mask,0);

	return mask;
    }
    else {
	return 0;
    }
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_HEAD_Addr(int i) 
{
  return ND_RESE_DCR(i,RE_VC0_HEAD);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_Head(int i) 
{
  return DCRReadPriv(ND_RESE_DCR(i,RE_VC0_HEAD));
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC0_Tail(int i) 
{
  return DCRReadPriv(ND_RESE_DCR(i,RE_VC0_TAIL));
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_Head(int i) 
{
  uint64_t x =  DCRReadPriv(ND_RESE_DCR(i,RE_VC5_STATE));
  return ND_RESE_DCR__RE_VC5_STATE__CTRL_HEAD_get(x);
}

/*__INLINE__*/ uint64_t getNDRESE_RE_VC5_Tail(int i) 
{
  uint64_t x =  DCRReadPriv(ND_RESE_DCR(i,RE_VC5_STATE));
  return ND_RESE_DCR__RE_VC5_STATE__CTRL_TAIL_get(x);
 
}


  




/*__INLINE__*/ uint64_t getNDRESE_SE_TOKENS_Addr(int i) {
    return ND_RESE_DCR(i,SE_TOKENS);
}

/*__INLINE__*/ uint64_t getNDRESE_SE_TOKENSn(int i) {
    return DCRReadPriv( ND_RESE_DCR(i,SE_TOKENS) );
}

/*__INLINE__*/ uint64_t getNDRESE_SE_TOKENSe(int i) {
    uint64_t expected = 0;
    if ( Kernel_GetNDExpectedTokensDCR(i,&expected) == 0 ) {
	return expected;
    }
    else {
	return -1; // This is going to miscompare!
    }
}

/*__INLINE__*/ uint64_t getNDRESE_SE_TOKENSm(int i) {
    if ( MUSPI_TERMCHECK_IsSenderEnabled(i) ) {
	return -1;
    }
    else {
	return 0;
    }
}

MUSPI_TERMCHECK_t _MUSPI_ND_TERMCHECK_STATE_MACHINE[] = {
    { getND500_RCP_FIFO_Addr,         getND500_RCP_FIFO_Head,    getND500_RCP_FIFO_Tail,     getND500_RCP_FIFOm,     MUSPI_ND_500_NUM_RCP_FIFO_P_DCRs  }, // Reception FIFO head/tail check
    { getND500_INJ_FIFO_Addr,         getND500_INJ_FIFO_Head,    getND500_INJ_FIFO_Tail,     getND500_INJ_FIFOm,     MUSPI_ND_500_NUM_INJ_FIFO_P_DCRs  }, // Injection FIFO head/tail check
    { getND500_STAT_Addr,             getND500_STATn,            getND500_STATe,             getND500_STATm,         MUSPI_ND_500_NUM_STAT_DCRs        }, 
    { getNDX2_STAT_Addr,              getNDX2_STATn,             getZero,                    getNDX2_STATm,          ND_X2_NUM_STAT_DCRs         }, 
    //  Receiver internal state, cannot be checked since system packets in flight
    //    { getNDRESE_INTERNAL_STATE_Addr,  getNDRESE_INTERNAL_STATEn, getNDRESE_INTERNAL_STATEe,  getNDRESE_INTERNAL_STATEm,  ND_RESE_DCR_num             }, 
    //  VC0 packet count checked eearlier as part of CRC exchange
    //    { getNDRESE_RE_VC0_PKT_CNT_Addr,  getNDRESE_RE_VC0_PKT_CNTn, getZero,                    getNDRESE_RE_VC0_PKT_CNTm,  ND_RESE_DCR_num             }, 
    { getNDRESE_RE_VC0_STATE_Addr,    getNDRESE_RE_VC0_STATEn,   getNDRESE_RE_VC0_STATEe,    getNDRESE_RE_VC0_STATEm,    ND_RESE_DCR_num             }, 
    { getNDRESE_RE_VC1_STATE_Addr,    getNDRESE_RE_VC1_STATEn,   getNDRESE_RE_VC1_STATEe,    getNDRESE_RE_VC1_STATEm,    ND_RESE_DCR_num             }, 
    { getNDRESE_RE_VC2_STATE_Addr,    getNDRESE_RE_VC2_STATEn,   getNDRESE_RE_VC2_STATEe,    getNDRESE_RE_VC2_STATEm,    ND_RESE_DCR_num             },
    //    System packets may still be in flight, don't check 
    //    { getNDRESE_RE_VC3_STATE_Addr,    getNDRESE_RE_VC3_STATEn,   getNDRESE_RE_VC3_STATEe,    getNDRESE_RE_VC3_STATEm,    ND_RESE_DCR_num             }, 
    // VC4 is used on some links by system collectives 
    // { getNDRESE_RE_VC4_STATE_Addr,    getNDRESE_RE_VC4_STATEn,   getNDRESE_RE_VC4_STATEe,    getNDRESE_RE_VC4_STATEm,    ND_RESE_DCR_num             }, 
    { getNDRESE_RE_VC5_STATE_Addr,    getNDRESE_RE_VC5_STATEn,   getNDRESE_RE_VC5_STATEe,    getNDRESE_RE_VC5_STATEm,    ND_RESE_DCR_num             }, 
    //   Issue 5102 - do not expect VC0 head == VC0 tail
    //    { getNDRESE_RE_VC0_HEAD_Addr,     getNDRESE_RE_VC0_Head,     getNDRESE_RE_VC0_Tail,      getAllBitsMask,    ND_RESE_DCR_num             },    
    { getNDRESE_RE_VC5_STATE_Addr,    getNDRESE_RE_VC5_Head,     getNDRESE_RE_VC5_Tail,      getAllBitsMask,    ND_RESE_DCR_num             },     

    //    Tokens checked earlier as part of CRC exchange
    //    { getNDRESE_SE_TOKENS_Addr,       getNDRESE_SE_TOKENSn,      getNDRESE_SE_TOKENSe,       getNDRESE_SE_TOKENSm,       ND_RESE_DCR_num             }, 

    // Include MU Term Checks here for convenience
    // Check if still fetching any user descriptors
    { getMCSRAM_BD_FETCHING_DESC_Addr,getMCSRAM_BD_FETCHING_DESC,getZero,                    getMCSRAM_BD_FETCHING_DESCm, 0x1               },
    // Check that all user rMEs are idle
    { getRME_STATE0_Addr             ,getRME_STATE0             ,getZero,                    getAllBitsMask             , 0x1               },
    { getRME_STATE1_Addr             ,getRME_STATE1             ,getZero,                    getRME_STATE1m             , 0x1               },
    // Check that all user iMEs are idle
    { getIME_STATE_Addr              ,getIME_STATE              ,getIME_STATEe,              getIME_STATEm              , 0x1               },
    { getMU_SOFTWARE_ERRn_Addr       ,getMU_SOFTWARE_ERRn       ,getZero,                    getMU_SOFTWARE_ERR_Mask    , 
      sizeof(_MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS)/sizeof(_MUSPI_TERMCHECK_MU_SOFTWARE_ERRORS[0]) },
};

__INLINE__ int32_t MUSPI_ND_TermCheck_FatalErrors(void) {

    int rc = 0;
    unsigned i;


    for  ( i = 0; i < sizeof(_MUSPI_ND_TERMCHECK_FATAL_ERRORS)/sizeof(_MUSPI_ND_TERMCHECK_FATAL_ERRORS[0]); i++ ) {
	rc += MUSPI_ND_TERMCHECK_step( _MUSPI_ND_TERMCHECK_FATAL_ERRORS + i );
    }

    return rc;
}

__INLINE__ int32_t MUSPI_ND_TermCheck_StateMachine(void) {

    int rc = 0;
    unsigned i;
    // Note: can only check user level information, cannot check RE_INTERNAL_STATE of SE_INTERNAL_STATE
    // since there may be system messaging in-flight 
    
    for  ( i = 0; i < sizeof(_MUSPI_ND_TERMCHECK_STATE_MACHINE)/sizeof(_MUSPI_ND_TERMCHECK_STATE_MACHINE[0]); i++ ) {
	rc += MUSPI_ND_TERMCHECK_step( _MUSPI_ND_TERMCHECK_STATE_MACHINE + i );
    }
    return rc;
}


__INLINE__ int32_t MUSPI_ND_TermCheck( MUSPI_GIBarrier_t* barrier, int jobRc ) {


  int32_t rc = 0;

  rc = MUSPI_TERMCHECK_LoadPersonality();

  if ( rc != 0 ) {
      return rc;
  }
  
  uint64_t timeout = /* 5 minutes */
      300ull *
      (uint64_t)_MUSPI_TERMCHECK_Personality.Kernel_Config.FreqMHz *
      1000ull * 1000ull;

  rc = MUSPI_GIBarrierEnter( barrier );

  if ( rc != 0 ) {
      printf("(E) Unexpected error entering barrier (%s:%d) rc=%d\n", __func__, __LINE__, rc);
      return rc;
  }
 
  rc = MUSPI_GIBarrierPollWithTimeout ( barrier, timeout );
  
  if ( rc != 0 ) {
      printf("(E) Barrier time out. (%s:%d) rc=%d\n", __func__, __LINE__, rc);
      return rc;
  }

  //DelayTimeBase(160000000); // Delay about 0.1 seconds to allow gi and ack to clear, not a guarantee
  
  rc += MUSPI_ND_TermCheck_FatalErrors();
  rc += MUSPI_ND_TermCheck_StateMachine();


  // @todo: CRC exchange

  // @todo : if error, issue end of job RAS event.
  return rc;
}


__END_DECLS

#endif
