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

/*
 *
 * This file contains the declarations for the memory area shared by the roq and it's device driver.
 *
 *
 *
 */
#include <stdint.h>
#include <stdlib.h>

#include <bgq_hw_abstraction.h>
#include <transcon.h>
#include <roq_fw.h>
#include <SRSW_QueueSet.h>

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <roq_device_interface.h>


struct RoQ_DeviceMemory  RoQ_DeviceMemory;
struct RoQ_DeviceMemory *RoQ_DevMem = &RoQ_DeviceMemory;

extern int roq_net_event_db_idx;
extern struct roq_net_event *RoQ_NET_EVENT_DB;


/*
 * Net event producer function. Only called from microcode.
 */
void roq_post_net_event(enum net_event_type type, void *value, int producer_id)
{
	static SpinLockVar_t	event_lock; /* initialized */
	struct roq_net_event	*evp;

	if (RoQ_NET_EVENT_DB) {
		int success = 1;

		SpinLock(&event_lock);

		evp = &RoQ_NET_EVENT_DB[roq_net_event_db_idx];

		if (evp->type == NET_EVENT_UNASSIGNED) {
			if (type != NET_EVENT_QP_DISCONNECT)
				DPRINTF(FXLOG_UCODE_CNTL, "Report event %d for resource %d\n", type, *(int *)value);

			switch (type) {

			case NET_EVENT_HOSTUNREACHABLE:
				memcpy(&evp->inaddr, value, sizeof evp->inaddr);

				break;

			case NET_EVENT_QP_DISCONNECT:
			case NET_EVENT_SQ_ERROR:
			case NET_EVENT_PROTOCOL_ERROR:
			case NET_EVENT_MEM_ACCESS_ERROR:
			case NET_EVENT_MEM_BASE_BOUNDS_ERROR:
			case NET_EVENT_CQ_OVERFLOW:
			case NET_EVENT_RQ_EMPTY:
			case NET_EVENT_SRQ_EMPTY:
				evp->resource_id = *(uint32_t *)value;

				break;

			default:
				DPRINTF(FXLOG_UCODE_CNTL, "Net Event Queue overflow: unknown event %d\n", type);
				success = 0;

				break;
			}
		} else {
			DPRINTF(FXLOG_UCODE_CNTL, "Net Event Queue overflow: cannot report %d\n", type);
			success = 0;
		}
		if (success) {
			evp->type = type;
			roq_net_event_db_idx++;
			roq_net_event_db_idx %= ROQ_EVENT_Q_SIZE;
		}
		SpinUnlock(&event_lock);

		if (success) /* Inform Linux device driver */
			RoQ_Post_DM_Event(RoQ_DB_NET, producer_id);
	}
}
