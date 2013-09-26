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

#ifndef _COMPILER_SUPPORT_H_ // Prevent multiple inclusion
#define _COMPILER_SUPPORT_H_

#if !defined(__LINUX__) && !defined(__ASSEMBLY__)
#if !defined(__UPCFS__) && !defined(__FUSEDOSFS__) && !defined(__MUFS__)                                                   // FUSEDOS
#include <stdint.h>
#endif                                                                                                                 // FUSEDOS
#endif

#ifdef __LINUX__
#include <linux/kernel.h>
#endif

#ifdef MODULE                                                                                                          // FUSEDOS
#include <linux/kernel.h>                                                                                              // FUSEDOS
#endif                                                                                                                 // FUSEDOS


#ifndef __INLINE__
#define __INLINE__ extern inline __attribute__((always_inline))
#endif


#define GLIBC_TCB_OFFSET 0x7000

#define GLIBC_TOC_OFFSET 0x8000

#if !defined(__ASSEMBLY__) && defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#define __C_LINKAGE extern "C"
#else
#define __BEGIN_DECLS
#define __END_DECLS
#define __C_LINKAGE
#endif

#if __GNUC__ == 2 && __GNUC_MINOR__ < 96
#define __builtin_expect(x, expected_value) (x)
#endif

#define __LIKELY(cond)    __builtin_expect((cond),1)
#define __UNLIKELY(cond)  __builtin_expect((cond),0)


#define __SHORT_CALL      __attribute__ ((shortcall))
#define __LONG_CALL       __attribute__ ((longcall))
#define __NORETURN        __attribute__ ((noreturn))
#define __PACKED          __attribute__ ((packed))

#if !defined(__ASSEMBLY__)

//
// Taking the address of a function gives the address
//  in the .opd segment of the following structure.
//
typedef struct OPD_Entry_t
                {
                void *Entry_Addr;
                void *TOC_Addr;
                }
                OPD_Entry_t;

#endif // __ASSEMBLY__


#endif // Add nothing below this line.
