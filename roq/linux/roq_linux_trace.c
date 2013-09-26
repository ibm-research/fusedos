/*
 * RoQ device driver for Linux
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if defined(PKTRACE_ON)
#include <linux/in.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/netdevice.h>
#include <linux/interrupt.h>

#include <linux/vmalloc.h>
#include <asm/pgtable.h>


#include "roq_debug.h"

#include <asm/io.h>

#include "roq_linux_trace_base.h"
#include "roq_linux_trace.h"

PkLinuxTraceState LinuxTraceState ;
addressTable_t addressTable[k_pirsize] __attribute__((aligned(64))) ;
EXPORT_SYMBOL(LinuxTraceState) ;
EXPORT_SYMBOL(addressTable) ;
#endif
