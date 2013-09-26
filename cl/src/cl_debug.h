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
#ifndef _CL_DEBUG_H
#define _CL_DEBUG_H

#include <stdio.h>
#include <inttypes.h>

extern int debug;
extern int debug_log_buffers;

#ifdef __cplusplus
extern "C" {
#endif

extern char * cl_debug_next_str(uint64_t spc_id);
extern void   cl_debug_complete(uint64_t spc_id, int strlen);

extern void cl_debug_init_logbuffers(void);
extern void cl_debug_dump_log_buffers(void);

#ifdef __cplusplus
}
#endif

#define CL_ERROR(FMT, ...) { printf("%lu %s:%d: " FMT, get_spc_context(this_spc())->id, __func__, __LINE__, ##__VA_ARGS__); }
#define CL_DEBUG(FMT, ...) { if (debug) { uint64_t spc_id=get_spc_context(this_spc())->id;                                           \
	if(debug_log_buffers)                                                                                                         \
		cl_debug_complete(spc_id, sprintf(cl_debug_next_str(spc_id), "%lu %s:%d: " FMT, spc_id, __func__, __LINE__, ##__VA_ARGS__)); \
	else printf("%lu %s:%d: " FMT, spc_id, __func__, __LINE__, ##__VA_ARGS__); } }

#define CL_ERROR_NOSPC(FMT, ...) { printf("%s:%d: " FMT, __func__, __LINE__, ##__VA_ARGS__); }
#define CL_DEBUG_NOSPC(FMT, ...) { if (debug) {                                                                 \
	if(debug_log_buffers)                                                                                    \
		cl_debug_complete(0, sprintf(cl_debug_next_str(0), "%s:%d: " FMT, __func__, __LINE__, ##__VA_ARGS__) ); \
	else printf("%s:%d: " FMT, __func__, __LINE__, ##__VA_ARGS__); } }

#endif // _CL_DEBUG_H
