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

#ifndef _DEBUG_BUFFER_H
#define _DEBUG_BUFFER_H

#define DEBUG_BUFFER_SIZE (1024*1024*10)

#if !defined(__ASSEMBLY__) && defined(__cplusplus)
extern "C" {
#endif

void debug_buffer_init();
void debug_buffer_cpy(uint8_t *buf, int len);
int debug_buffer_printf(const char *fmt, ...);
	void debug_buffer_init();
	int debug_buffer_printf( const char *fmt, ...);

#if !defined(__ASSEMBLY__) && defined(__cplusplus)
};
#endif

#endif /* _DEBUG_BUFFER_H */
