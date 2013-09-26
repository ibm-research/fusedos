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

#ifndef _IONLINK_H
#define _IONLINK_H

struct roq_ion_send_t {
	uint8_t dest[5];
	uint8_t cn2ion;
	uint32_t putoffset;
	void *dma_adr;
	uint32_t len;
};

void roq_ion_send(struct roq_ion_send_t *s);
int roq_ion_init(void);
int __init roq_ionlink_init(void);

#endif /* _IONLINK_H */


