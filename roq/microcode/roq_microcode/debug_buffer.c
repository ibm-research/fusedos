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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <debug_buffer.h>
#include <roq_device_interface.h>
#include <hwi/include/common/compiler_support.h>
#include <roq_kernel.h>

static volatile uint8_t *debug_buffer;
static volatile uint8_t *debug_run;
static volatile uint64_t *total_len = (uint64_t*) (0x28 | ROQ_VIRTUAL_MAPPING);
static volatile uint64_t *cur_pos   = (uint64_t*) (0x30 | ROQ_VIRTUAL_MAPPING);
static struct SpinLockVar_t lock;


inline void
debug_buffer_cpy(uint8_t *buf, int len)
{
	uint64_t part_len;
	SpinLock(&lock);

	if (((uint64_t)debug_run + len) > ((uint64_t)debug_buffer + DEBUG_BUFFER_SIZE)) {
		part_len = ((uint64_t)debug_buffer+DEBUG_BUFFER_SIZE) - (uint64_t)debug_run;
		memcpy((void*)debug_run, buf, part_len);
		debug_run = debug_buffer;
		memcpy((void*)debug_run, buf+part_len, len-part_len);
		debug_run += (len-part_len);
		*total_len = len-part_len;
	} else {
		memcpy((void*)debug_run, buf, len);
		debug_run += len;
		*total_len += len;
	}
	*cur_pos = (uint64_t)debug_run;

	SpinUnlock(&lock);
}

void
debug_buffer_init()
{
	uint64_t *b  = (uint64_t*) (0x20 | ROQ_VIRTUAL_MAPPING);
	debug_buffer = (uint8_t*)malloc(DEBUG_BUFFER_SIZE);
	debug_run    = debug_buffer;
	*b = (uint64_t)debug_buffer;
	*total_len = 0;
	*cur_pos   = (uint64_t)debug_buffer;
	SpinInit(&lock);
}

int
debug_buffer_printf(const char *fmt, ...)
{
	va_list args;
	int len;
	char buffer[2048];

	va_start(args, fmt);

	len = vsprintf(&buffer[0], fmt, args);
	debug_buffer_cpy(buffer, len);

	va_end(args);

    if (len > sizeof(buffer)) {
		Terminate(__LINE__);
	}

	return len;
}
