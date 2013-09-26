/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef _ROQ_DEBUG_H
#define _ROQ_DEBUG_H

#if defined(DEBUG_BUFFER) && DEBUG_BUFFER == 1
 #include <debug_buffer.h>
#endif


#ifndef FXLOG_ENABLE  // turn this on to enable log lines( or any of the macros for groups and debug is enabled)
#define FXLOG_ENABLE (0)
#endif


/*** DEBUG *********************************************************/
#define FXLOG_UCODE_ALL 0
#define FXLOG_UCODE_MED 0

#ifndef FXLOG_UCODE_TX
#define FXLOG_UCODE_TX  0
#endif

#ifndef FXLOG_TRANCONN_TX
#define FXLOG_TRANCONN_TX (0)
#endif

/* log lines that would come put per iteration of a high frequency loop */

#ifndef FXLOG_UCODE_MEMORY_ACCESS
#define FXLOG_UCODE_MEMORY_ACCESS (0)
#endif

#ifndef FXLOG_UCODE_PER_ITER
#define FXLOG_UCODE_PER_ITER (0 ||FXLOG_UCODE_ALL)
#endif

#ifndef FXLOG_UCODE_PER_PACKET
#define FXLOG_UCODE_PER_PACKET (0||FXLOG_UCODE_ALL)
#endif

#ifndef FXLOG_UCODE_PER_BURST
#define FXLOG_UCODE_PER_BURST (FXLOG_UCODE_MED||FXLOG_UCODE_PER_PACKET)
#endif

#ifndef FXLOG_UCODE_PER_MESSAGE
#define FXLOG_UCODE_PER_MESSAGE (0||FXLOG_UCODE_ALL ||FXLOG_UCODE_MED)
#endif

#ifndef FXLOG_UCODE_CNTL
#define FXLOG_UCODE_CNTL  (0||FXLOG_UCODE_ALL || FXLOG_UCODE_MED )
#endif

#ifndef PKFXLOG_NACK_HANDLING
#define PKFXLOG_NACK_HANDLING (0||FXLOG_UCODE_ALL || FXLOG_UCODE_MED )
#endif

#ifndef FXLOG_UCODE_QP_STATE
#define FXLOG_UCODE_QP_STATE (0||FXLOG_UCODE_ALL || FXLOG_UCODE_MED )
#endif

#ifndef FXLOG_REMOTE_GET
#define FXLOG_REMOTE_GET (1)
#endif
/* log lines to be kept in place but almost never turned on */
#ifndef FXLOG_UCODE_ALMOST_NEVER_NEEDED
#define FXLOG_UCODE_ALMOST_NEVER_NEEDED (0)
#endif

/* log lines to be kept in place but almost never turned on */
#ifndef FXLOG_UCODE_INIT
#define FXLOG_UCODE_INIT ( 0 || FXLOG_UCODE_ALL)
#endif

#ifndef DEBUG_APPL_OOB
#define DEBUG_APPL_OOB (0)
#endif

#ifndef DEBUG_REMOTE_GET
#define DEBUG_REMOTE_GET (0)
#endif

inline static uint64_t
get_timex(void)
{
	uint64_t raw = 0;
	asm volatile("mftb %0":"=r"(raw));
	// raw /= 1600;
	return raw;
}

inline static char*
PN2FN(char *PN)
{
	char* ch;
	char* rv = PN;
	for(ch = PN; *ch; ch++) {
		if( *ch == '/' )
			rv = ch + 1;
	}
	return( rv );
}
//#define NO_ASSERTIONS

#define DDBBGG (FXLOG_ENABLE | FXLOG_UCODE_PER_ITER | FXLOG_UCODE_PER_PACKET | FXLOG_UCODE_PER_BURST \
		      | FXLOG_UCODE_PER_MESSAGE | FXLOG_UCODE_CNTL |  PKFXLOG_NACK_HANDLING \
		      | FXLOG_UCODE_QP_STATE | FXLOG_UCODE_ALMOST_NEVER_NEEDED | FXLOG_UCODE_INIT)

#if defined(DEBUG_BUFFER) & DEBUG_BUFFER == 1
  #define DASSERT(r, f, ...) if (!(r)) { \
    printf("ASSERT[%s:%d] " f, __FILE__, __LINE__,  ##__VA_ARGS__); \
    debug_buffer_printf("ASSERT[%lld %s %4d] " f, get_timex(), PN2FN((char*)__FILE__), __LINE__, ##__VA_ARGS__); \
  }
  #define DSTRONG_ASSERT(r, f, ...) if (!(r)) { \
    printf("STRONG_ASSERT[%s:%d] " f, __FILE__, __LINE__,  ##__VA_ARGS__); \
	debug_buffer_printf("STRONG_ASSERT[%lld %s %4d] " f, get_timex(), PN2FN((char*)__FILE__), __LINE__, ##__VA_ARGS__); \
    while (1); \
  }
#else
#ifdef NO_ASSERTIONS
  #define DASSERT(r, f, ...) { ; }
  #define DSTRONG_ASSERT(r, f, ...) { ; }
#else
  #define DASSERT(r, f, ...) if (!(r)) { printf("ASSERT[%s:%d] " f, __FILE__, __LINE__,  ##__VA_ARGS__); while (1) sleep (1);}
  #define DSTRONG_ASSERT(r, f, ...) if (!(r)) { printf("STRONG_ASSERT[%s:%d] " f, __FILE__, __LINE__,  ##__VA_ARGS__); while (1); }
#endif /* NO_ASSERTIONS */
#endif

#if (DDBBGG >= 1)
 #if defined(USERSPACE_ROQ)
  #define DPRINTF(r, f, ...) if (r) printf("LOG[ %lld %s %d ] " f, get_timex(), PN2FN((char*)__FILE__), __LINE__, ##__VA_ARGS__);
 #else
  #if defined(DEBUG_BUFFER) & DEBUG_BUFFER == 1
   #define DPRINTF(r, f, ...) if (r) debug_buffer_printf("LOG[ %lld %s %4d %2d:%d] " f, get_timex(), PN2FN((char*)__FILE__), __LINE__,  ProcessorCoreID(), ProcessorThreadID(),  ##__VA_ARGS__);
  #else
   #define DPRINTF(r, f, ...) if (r) printf("LOG[ %lld %s %4d %2d:%d ] " f, get_timex(), PN2FN((char*)__FILE__), __LINE__,  ProcessorCoreID(), ProcessorThreadID(),  ##__VA_ARGS__);
  // #define DPRINTF(r, f, ...)
  #endif
 #endif
#else
  #define DPRINTF(r, f, ...)
#endif
#endif /* _ROQ_DEBUG_H */
