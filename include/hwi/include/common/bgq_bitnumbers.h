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

#ifndef _BGQ_BITNUMBERS_H_  // Prevent multiple inclusion
#define _BGQ_BITNUMBERS_H_

#include <hwi/include/common/compiler_support.h>

__BEGIN_DECLS

// These defines allows use of IBM's bit numberings (MSb=0, LSb=63) for multi-bit fields
//  b = IBM bit number of the least significant bit (highest number)
//  x = value to set in field
//  s = size
#define _BS32(b,x,s) (((x) & (0x7FFFFFFF >> (31 - (s)))) << (31 - (b)))
#define _BG32(b,x,s) ((_BS32(b,0x7FFFFFFF,s) & (x)) >> (31 - (b)))
#define _BS64(b,x,s) (((x) & (0x7FFFFFFFFFFFFFFFLL >> (63 - (s)))) << ( 63 - (b)))
#define _BG64(b,x,s) (( _BS64(b, 0x7FFFFFFFFFFFFFFFLL,s) & (x)) >> (63 - (b)))


#if defined(__ASSEMBLY__)
#define UNSIGNED64(x) x
#else
#define UNSIGNED64(x) x##ULL
#endif

#define _BGQ_SET(w,b,v) (((v) & ((UNSIGNED64(2)<<((w)-1))-1)) << (63-(b)))
#define _BGQ_GET(w,b,x) (((x) >> (63-(b))) & ((UNSIGNED64(2)<<((w)-1))-1))

#if !defined(__ASSEMBLY__)
#define _BGQ_INSERT(w,b,x,v) RLIMI64(x, (v), 63-(b), (b)-(w)+1)
#endif

#define _BN(b)    _BGQ_SET(1,b,1)

#define _B1(b,v)  _BGQ_SET(1,b,v)
#define _B2(b,v)  _BGQ_SET(2,b,v)
#define _B3(b,v)  _BGQ_SET(3,b,v)
#define _B4(b,v)  _BGQ_SET(4,b,v)
#define _B5(b,v)  _BGQ_SET(5,b,v)
#define _B6(b,v)  _BGQ_SET(6,b,v)
#define _B7(b,v)  _BGQ_SET(7,b,v)
#define _B8(b,v)  _BGQ_SET(8,b,v)
#define _B9(b,v)  _BGQ_SET(9,b,v)
#define _B10(b,v) _BGQ_SET(10,b,v)
#define _B11(b,v) _BGQ_SET(11,b,v)
#define _B12(b,v) _BGQ_SET(12,b,v)
#define _B13(b,v) _BGQ_SET(13,b,v)
#define _B14(b,v) _BGQ_SET(14,b,v)
#define _B15(b,v) _BGQ_SET(15,b,v)
#define _B16(b,v) _BGQ_SET(16,b,v)
#define _B17(b,v) _BGQ_SET(17,b,v)
#define _B18(b,v) _BGQ_SET(18,b,v)
#define _B19(b,v) _BGQ_SET(19,b,v)
#define _B20(b,v) _BGQ_SET(20,b,v)
#define _B21(b,v) _BGQ_SET(21,b,v)
#define _B22(b,v) _BGQ_SET(22,b,v)
#define _B23(b,v) _BGQ_SET(23,b,v)
#define _B24(b,v) _BGQ_SET(24,b,v)
#define _B25(b,v) _BGQ_SET(25,b,v)
#define _B26(b,v) _BGQ_SET(26,b,v)
#define _B27(b,v) _BGQ_SET(27,b,v)
#define _B28(b,v) _BGQ_SET(28,b,v)
#define _B29(b,v) _BGQ_SET(29,b,v)
#define _B30(b,v) _BGQ_SET(30,b,v)
#define _B31(b,v) _BGQ_SET(31,b,v)
#define _B32(b,v) _BGQ_SET(32,b,v)
#define _B33(b,v) _BGQ_SET(33,b,v)
#define _B34(b,v) _BGQ_SET(34,b,v)
#define _B35(b,v) _BGQ_SET(35,b,v)
#define _B36(b,v) _BGQ_SET(36,b,v)
#define _B37(b,v) _BGQ_SET(37,b,v)
#define _B38(b,v) _BGQ_SET(38,b,v)
#define _B39(b,v) _BGQ_SET(39,b,v)
#define _B40(b,v) _BGQ_SET(40,b,v)
#define _B41(b,v) _BGQ_SET(41,b,v)
#define _B42(b,v) _BGQ_SET(42,b,v)
#define _B43(b,v) _BGQ_SET(43,b,v)
#define _B44(b,v) _BGQ_SET(44,b,v)
#define _B45(b,v) _BGQ_SET(45,b,v)
#define _B46(b,v) _BGQ_SET(46,b,v)
#define _B47(b,v) _BGQ_SET(47,b,v)
#define _B48(b,v) _BGQ_SET(48,b,v)
#define _B49(b,v) _BGQ_SET(49,b,v)
#define _B50(b,v) _BGQ_SET(50,b,v)
#define _B51(b,v) _BGQ_SET(51,b,v)
#define _B52(b,v) _BGQ_SET(52,b,v)
#define _B53(b,v) _BGQ_SET(53,b,v)
#define _B54(b,v) _BGQ_SET(54,b,v)
#define _B55(b,v) _BGQ_SET(55,b,v)
#define _B56(b,v) _BGQ_SET(56,b,v)
#define _B57(b,v) _BGQ_SET(57,b,v)
#define _B58(b,v) _BGQ_SET(58,b,v)
#define _B59(b,v) _BGQ_SET(59,b,v)
#define _B60(b,v) _BGQ_SET(60,b,v)
#define _B61(b,v) _BGQ_SET(61,b,v)
#define _B62(b,v) _BGQ_SET(62,b,v)
#define _B63(b,v) _BGQ_SET(63,b,v)
#define _B64(b,v) _BGQ_SET(64,b,v)


// These defines ease extraction of bitfields.
//  x = 64 bit value
//  b = IBM bit number of least significant bit of field
//  when b is a const, compiler should generate a single rotate-and-mask instruction
#define _GN(x,b)  _BGQ_GET(1,b,x)
#define _G1(x,b)  _BGQ_GET(1,b,x)
#define _G2(x,b)  _BGQ_GET(2,b,x)
#define _G3(x,b)  _BGQ_GET(3,b,x)
#define _G4(x,b)  _BGQ_GET(4,b,x)
#define _G5(x,b)  _BGQ_GET(5,b,x)
#define _G6(x,b)  _BGQ_GET(6,b,x)
#define _G7(x,b)  _BGQ_GET(7,b,x)
#define _G8(x,b)  _BGQ_GET(8,b,x)
#define _G9(x,b)  _BGQ_GET(9,b,x)
#define _G10(x,b) _BGQ_GET(10,b,x)
#define _G11(x,b) _BGQ_GET(11,b,x)
#define _G12(x,b) _BGQ_GET(12,b,x)
#define _G13(x,b) _BGQ_GET(13,b,x)
#define _G14(x,b) _BGQ_GET(14,b,x)
#define _G15(x,b) _BGQ_GET(15,b,x)
#define _G16(x,b) _BGQ_GET(16,b,x)
#define _G17(x,b) _BGQ_GET(17,b,x)
#define _G18(x,b) _BGQ_GET(18,b,x)
#define _G19(x,b) _BGQ_GET(19,b,x)
#define _G20(x,b) _BGQ_GET(20,b,x)
#define _G21(x,b) _BGQ_GET(21,b,x)
#define _G22(x,b) _BGQ_GET(22,b,x)
#define _G23(x,b) _BGQ_GET(23,b,x)
#define _G24(x,b) _BGQ_GET(24,b,x)
#define _G25(x,b) _BGQ_GET(25,b,x)
#define _G26(x,b) _BGQ_GET(26,b,x)
#define _G27(x,b) _BGQ_GET(27,b,x)
#define _G28(x,b) _BGQ_GET(28,b,x)
#define _G29(x,b) _BGQ_GET(29,b,x)
#define _G30(x,b) _BGQ_GET(30,b,x)
#define _G31(x,b) _BGQ_GET(31,b,x)
#define _G32(x,b) _BGQ_GET(32,b,x)
#define _G33(x,b) _BGQ_GET(33,b,x)
#define _G34(x,b) _BGQ_GET(34,b,x)
#define _G35(x,b) _BGQ_GET(35,b,x)
#define _G36(x,b) _BGQ_GET(36,b,x)
#define _G37(x,b) _BGQ_GET(37,b,x)
#define _G38(x,b) _BGQ_GET(38,b,x)
#define _G39(x,b) _BGQ_GET(39,b,x)
#define _G40(x,b) _BGQ_GET(40,b,x)
#define _G41(x,b) _BGQ_GET(41,b,x)
#define _G42(x,b) _BGQ_GET(42,b,x)
#define _G43(x,b) _BGQ_GET(43,b,x)
#define _G44(x,b) _BGQ_GET(44,b,x)
#define _G45(x,b) _BGQ_GET(45,b,x)
#define _G46(x,b) _BGQ_GET(46,b,x)
#define _G47(x,b) _BGQ_GET(47,b,x)
#define _G48(x,b) _BGQ_GET(48,b,x)
#define _G49(x,b) _BGQ_GET(49,b,x)
#define _G50(x,b) _BGQ_GET(50,b,x)
#define _G51(x,b) _BGQ_GET(51,b,x)
#define _G52(x,b) _BGQ_GET(52,b,x)
#define _G53(x,b) _BGQ_GET(53,b,x)
#define _G54(x,b) _BGQ_GET(54,b,x)
#define _G55(x,b) _BGQ_GET(55,b,x)
#define _G56(x,b) _BGQ_GET(56,b,x)
#define _G57(x,b) _BGQ_GET(57,b,x)
#define _G58(x,b) _BGQ_GET(58,b,x)
#define _G59(x,b) _BGQ_GET(59,b,x)
#define _G60(x,b) _BGQ_GET(60,b,x)
#define _G61(x,b) _BGQ_GET(61,b,x)
#define _G62(x,b) _BGQ_GET(62,b,x)
#define _G63(x,b) _BGQ_GET(63,b,x)
#define _G64(x,b) _BGQ_GET(64,b,x)

__END_DECLS

#endif // Add nothing below this line.
