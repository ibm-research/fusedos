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

#ifndef	_ASM_SUPPORT_H_ // Prevent multiple inclusion
#define	_ASM_SUPPORT_H_

#if defined(__ASSEMBLY__)

//
// Useful Assembler Support Macros
//

#define IMM64(R,V)             \
          lis    R,V@highest;  \
          ori    R,R,V@higher; \
          rldicr R,R,32,31;    \
          oris   R,R,V@h;      \
          ori    R,R,V@l;

// Load an address or symbol into a register
#define IMM32(R,V) \
         addis R,%r0,V@h; \
         ori   R,R,V@l;

// Load an unsigned constant into a register.  This macro is provided so
//  you can change the const in some header file without having to change
//  the code needed to use that constant.
// Handles 16bit (upper or lower half) or 32bit constants.
// Note: Use "li" instr for known constant signed values < 16bits. In that
//  case, the assembler will generate the correct single instruction code.
#define IMM32C(R,V) \
.if (((V) & 0xFFFF8000) == 0 || ((V) & 0xFFFF8000) == 0xFFFF8000) ; /* 16 (or less) bit consts (addi sign extends) */ \
      addi  R,%r0,(V) ; \
.else ; \
      addis R,%r0,V@h ;  /* get upper bits, zeroing lower bits */ \
.if ((V) & 0x0000FFFF) ; \
      ori   R,R,V@l ;    /* get any remaining low bits */ \
.endif ; \
.endif

// Set an Interrupt Vector Register from the Low-Half of a Symbol
#define SET_IVOR( R, I, V )\
        li  R,V@l; \
        mtspr I,R;

// Stuff a 32-bit Address or Symbol into a Special Purposed Register
#define SET_SPR( R, S, V ) \
         IMM32( R, V ); \
         mtspr  S,R;

// Stuff a 32-bit Constant into a Special Purposed Register
#define SET_SPRC( R, S, V ) \
         IMM32C( R, V ); \
         mtspr  S,R;


#endif // __ASSEMBLY__

#endif // Add nothing below this line.
