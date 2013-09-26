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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hwi/include/bqc/A2_inlines.h"

// large maximum number of SPCs, so that we can use a static array

#define MAX_NUM_SPCS 67

#define LOG_BUFFER_SIZE (10 * 1024 * 1024)

int debug_log_buffers;

typedef struct {
	char * base;
	char *next;
} log_buffer_t;

log_buffer_t log_buffers[MAX_NUM_SPCS];

void cl_debug_init_logbuffers(void)
{
	memset(log_buffers, 0, sizeof(log_buffers));
}

char * cl_debug_next_str(uint64_t spc_id)
{
	// TODO add some form of overflow protection/handling

	// make sure that log buffer is allocated
	if(log_buffers[spc_id].base == NULL) {
		log_buffers[spc_id].base = (char *) malloc(sizeof(char) * LOG_BUFFER_SIZE);
		log_buffers[spc_id].next = log_buffers[spc_id].base;

		memset(log_buffers[spc_id].base, 0, sizeof(char) * LOG_BUFFER_SIZE);
	}

	// add timestamp
	uint64_t * ts = (uint64_t *) log_buffers[spc_id].next;
	*ts = GetTimeBase();

	log_buffers[spc_id].next += sizeof(uint64_t);

	return log_buffers[spc_id].next;
}

void cl_debug_complete(uint64_t spc_id, int strlen)
{
	log_buffers[spc_id].next += strlen + 1; // leave a '\0'
}

void cl_debug_dump_log_buffers()
{
	int i;
	int empty_round = 1;
	uint64_t cur_ts = 0;
	char * visit_ptr[MAX_NUM_SPCS];

	memset(visit_ptr, 0, sizeof(visit_ptr));

	printf("Dumping log buffers, sorted by timestamp:\n");

	for(i=0; i < MAX_NUM_SPCS; i++) {
		if(log_buffers[i].base == NULL) continue; // skip unused slots

		visit_ptr[i] = log_buffers[i].base;
		
		uint64_t *ts = (uint64_t *) log_buffers[i].base;

		if(*ts < cur_ts || cur_ts == 0)
			cur_ts = *ts;
	}

	if(cur_ts == 0) return; // no used slots found

	// simple, stupid
	do {
		uint64_t lowest_ts = 0;
		int lowest_idx = 0;

		empty_round = 1;

		for(i=0; i < MAX_NUM_SPCS; i++) {
			// skip empty entries
			if(log_buffers[i].base == NULL)
				continue;
			// skip entries we have visited completely
			if(visit_ptr[i] == log_buffers[i].next)
				continue;

			// found one
			empty_round = 0;

			if( *((uint64_t*)(visit_ptr[i])) < lowest_ts || lowest_ts == 0) {
				lowest_ts = *((uint64_t*)(visit_ptr[i]));
				lowest_idx = i;
			}
		}

		if(empty_round) break; // no more entries found

		// dump entry
		printf("%lu: %s",
				*((uint64_t *)(visit_ptr[lowest_idx])),
				visit_ptr[lowest_idx] + sizeof(uint64_t)
		);

		// advance visit pointer for that entry
		visit_ptr[lowest_idx] += sizeof(uint64_t);
		visit_ptr[lowest_idx] += strlen(visit_ptr[lowest_idx]) + 1;
	} while(!empty_round);
}
