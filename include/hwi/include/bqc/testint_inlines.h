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


#ifndef _TI_INLINES_H_
#define _TI_INLINES_H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/testint_tdr.h>
#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <stdint.h>
#endif                                                                                                                 // FUSEDOS
#include <hwi/include/bqc/A2_inlines.h>

__BEGIN_DECLS


#define BGQ_JTAG_IDCODE_VALUE_BQC_DD1_DCR      (0x0000000009003049LL)      // Value of BQC chip DD1.0 IEEE1149.1 IDcode.  
#define BGQ_JTAG_IDCODE_VALUE_BQC_DD2_DCR      (0x0000000029003049LL)      // Value of BQC chip DD1.0 IEEE1149.1 IDcode.  
                                                                  

#define TI_GetIDCODE() DCRReadPriv(TESTINT_DCR(JTAG_IDCODE))
#define TI_isDD1() (TI_GetIDCODE() == BGQ_JTAG_IDCODE_VALUE_BQC_DD1_DCR)
#define TI_isDD2() (TI_GetIDCODE() == BGQ_JTAG_IDCODE_VALUE_BQC_DD2_DCR)

/**
 * @brief Returns the "torus dimension is swapped" bits, as determined by the
 *        card (or overridden by software).
 * @param dimension specifies the torus dimension (A=0, B=1, ..., E=4).
 */

__INLINE__ int TI_GetDimensionSwap(unsigned dimension) {

  // +--------------------------------------------------------------+
  // | NOTE: dimension swaps are in bits 121-125 of the config TDR, |
  // |       which translates into bits 57-61 of the CONFIG1 DCR    |
  // +--------------------------------------------------------------+

  uint64_t config1 = DCRReadPriv(TESTINT_DCR(CONFIG1));
  return ( config1 & _BN(57+dimension) ) ? 1 : 0;
}

/**
 * @brief Returns all 5 torus dimension swapped bits.  The LSB is
 *        the E dimension, the second LSB is the D dimension, and
 *        so on.
 */

#define TESTINT_CONFIG1_BIT_SWAP(v)        _BGQ_SET(5,125-64,v)
#define TESTINT_CONFIG1_IS_IONODE          _BN(127-64)
#define TESTINT_CONFIG1_USE_PORT6_FOR_IO   _BN(105-64)
#define TESTINT_CONFIG1_USE_PORT7_FOR_IO   _BN(106-64)

__INLINE__ uint64_t TI_GetDimensionSwaps( void ) {
  return ( DCRReadPriv(TESTINT_DCR(CONFIG1)) >> 2 ) & 0x1F;
}


__INLINE__ int TI_IsIoNode( void ) {
    return ( DCRReadPriv(TESTINT_DCR(CONFIG1)) & TESTINT_CONFIG1_IS_IONODE ) ? 1 : 0;
}


__INLINE__ int TI_UsePort6forIO( void ) {
  return ( DCRReadPriv(TESTINT_DCR(CONFIG1)) & TESTINT_CONFIG1_USE_PORT6_FOR_IO ) ? 1 : 0;
}

__INLINE__ int TI_UsePort7forIO( void ) {
  return ( DCRReadPriv(TESTINT_DCR(CONFIG1)) & TESTINT_CONFIG1_USE_PORT7_FOR_IO ) ? 1 : 0;
}



__INLINE__ int TI_NetworkFullSpeed()
{
  
  uint64_t clockDCR =  DCRReadPriv(TESTINT_DCR(CLKTREE_CTRL18));
  //  if bit 14 is 0, then network is full rate (500 Mhz)
  //  if bit 14 is 1, then network is half rate (250 Mhz)
  if _GN(clockDCR,14) return 0;
  else return 1;
  
}

__INLINE__ uint64_t TI_NetworkGBS()
{
  // returns the Gigabytes/sec on each link 
  uint64_t clockDCR =  DCRReadPriv(TESTINT_DCR(CLKTREE_CTRL18));
  //  if bit 14 is 0, then network is full rate (500 Mhz), 2 GB/s
  //  if bit 14 is 1, then network is half rate (250 Mhz), 1 GB/s
  if _GN(clockDCR,14) return (uint64_t) 1;
  else return (uint64_t) 2;
  
}



__INLINE__ uint64_t  TI_CoreFrequency()
{
  
    // Determine the core frequency in MHz:
    // DCR bits 12-13:
    // 00 ==> 1600 MHz
    // 01 ==>  800
    // 10 ==> 1200
    // 11 ==> 2000,  not supported, so return 0
    uint64_t frequencyDCR = DCRReadPriv(TESTINT_DCR(CLKTREE_CTRL18));
    uint64_t coreFrequency = 1600;
    
    switch ( (frequencyDCR & _B2(13,0xF) ) )
      {
      case 0:
	coreFrequency    = 1600;
	break;
      case _B2(13,0x01):
	coreFrequency    =  800;
	break;
      case _B2(13,0x02):
	coreFrequency    = 1200;
	break;
      default:
	return 0;
	
      }
    return coreFrequency;
    

    
}



__INLINE__ uint64_t TI_GigaBitsPerLink()
  
{
  // return how many Gigabits could have been sent over a link since time 0

  uint64_t tb = GetTimeBase();
  uint64_t core_freq =   TI_CoreFrequency(); // in Mhz
  int full_speed = TI_NetworkFullSpeed(); //  1 if full speed, else 0
  uint64_t giga_bits;
  if ( full_speed == 0) giga_bits = tb*8;
  else  giga_bits = tb*16;
  giga_bits =  giga_bits /( core_freq * 1000000);
  return giga_bits;


  
}

#define BG_DCRPCB_RESET_REQ	0x1000000000000000LL

#define BG_DCRPCB_READ_REQ	0x0000000000000000LL
#define BG_DCRPCB_WRITE_REQ	0x8000000000000000LL
#define BG_DCRPCB_ERROR_CLR	0x0800000000000000LL

__INLINE__
void BgDcrPcbReset()
{
    // reset start
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_RESET_REQ);
    // reset end
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), 0);
}


__INLINE__
void BgDcrPcbErrorClr()
{
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_ERROR_CLR);
}


__INLINE__
uint64_t BgDcrPcbRead (uint64_t nScomReg)
{
    uint64_t address = (_BGQ_SCOMCHIPLET(nScomReg) << 24)
                          | (_BGQ_SCOMPORT(nScomReg) << 16)
                          | _BGQ_SCOMREG(nScomReg);

    // Post Read request to PCB slave
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_READ_REQ | address) ;
    // Fetch data
    return (DCRReadPriv (TESTINT_DCR(DCR_PCB_DATA)));
}


__INLINE__
void BgDcrPcbWrite (uint64_t nScomReg, uint64_t ullScomData)
{
    uint64_t address = (_BGQ_SCOMCHIPLET(nScomReg) << 24)
                          | (_BGQ_SCOMPORT(nScomReg) << 16)
                          | _BGQ_SCOMREG(nScomReg);

    // set data
    DCRWritePriv (TESTINT_DCR(DCR_PCB_DATA), ullScomData);
    // Post write request to PCB slave
    DCRWritePriv (TESTINT_DCR(DCR_PCB_CNTL), BG_DCRPCB_WRITE_REQ | address) ;
}



__END_DECLS

#endif
