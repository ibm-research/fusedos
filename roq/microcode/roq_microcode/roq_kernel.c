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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <roq_device_interface.h>
#include <roq_kernel.h>

void RoQ_MC_Init();

/* ROQ START AND THREAD */
void
roq_kernel(void)
{
	printf("ROQ KERNEL (INF: %lx)\n", (uint64_t)RoQ_DevMem & ~ROQ_VIRTUAL_MAPPING);
	RoQ_MC_Init();
}
