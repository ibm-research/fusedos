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

#ifndef _BGQ_ALIGNMENT_H_ // Prevent multiple inclusion
#define _BGQ_ALIGNMENT_H_

#include <hwi/include/common/compiler_support.h>

__BEGIN_DECLS

#define L1_CACHE_LINE_SIZE 64
#define L2_CACHE_LINE_SIZE 128

#if defined(__ASSEMBLY__)

#define ALIGN_L1_DIRTYBIT  4
#define ALIGN_QUADWORD     5
#define ALIGN_L1_CACHE     6
#define ALIGN_L1I_CACHE    6
#define ALIGN_L1D_CACHE    6
#define ALIGN_L2_CACHE     7

#elif defined(__GNUC__)

#define ALIGN_L1_DIRTYBIT __attribute__ ((aligned (  8)))
#define ALIGN_QPX_VECTOR  __attribute__ ((aligned ( 32)))
#define ALIGN_QUADWORD    __attribute__ ((aligned ( 32)))
#define ALIGN_L1_CACHE    __attribute__ ((aligned (L1_CACHE_LINE_SIZE)))
#define ALIGN_L1I_CACHE   __attribute__ ((aligned (L1_CACHE_LINE_SIZE)))
#define ALIGN_L1D_CACHE   __attribute__ ((aligned (L1_CACHE_LINE_SIZE)))
#define ALIGN_L2_CACHE    __attribute__ ((aligned (L2_CACHE_LINE_SIZE)))

#elif defined(__xlc__) || defined(__xlC__)

#define ALIGN_L1_DIRTYBIT __attribute__ ((__aligned__ (  8)))
#define ALIGN_QPX_VECTOR  __attribute__ ((__aligned__ ( 32)))
#define ALIGN_QUADWORD    __attribute__ ((__aligned__ ( 32)))
#define ALIGN_L1_CACHE    __attribute__ ((__aligned__ (L1_CACHE_LINE_SIZE)))
#define ALIGN_L1I_CACHE   __attribute__ ((__aligned__ (L1_CACHE_LINE_SIZE)))
#define ALIGN_L1D_CACHE   __attribute__ ((__aligned__ (L1_CACHE_LINE_SIZE)))
#define ALIGN_L2_CACHE    __attribute__ ((__aligned__ (L2_CACHE_LINE_SIZE)))

#else

#warning "Need alignment directives for your compiler!"

#define ALIGN_QUADWORD
#define ALIGN_L1_CACHE
#define ALIGN_L1I_CACHE
#define ALIGN_L1D_CACHE
#define ALIGN_L2_CACHE

#endif // __ASSEMBLY__

__END_DECLS

#endif // Add nothing below this line

