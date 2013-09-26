# begin_generated_IBM_copyright_prolog
#
# This is an automatically generated copyright prolog.
# After initializing,  DO NOT MODIFY OR MOVE
# ================================================================
#
# Licensed Materials - Property of IBM
#
# Blue Gene/Q
#
# (C) Copyright IBM Corp.  2010, 2012
#
# US Government Users Restricted Rights -
# Use, duplication or disclosure restricted
# by GSA ADP Schedule Contract with IBM Corp.
#
# This software is available to you under the
# Eclipse Public License (EPL).
#
# ================================================================
#
# end_generated_IBM_copyright_prolog
EXTRA_CFLAGS += -D__KERNEL__ -D__SPCM__ -iquote$(TOP)/cl/src
EXTRA_CFLAGS += -iquote$(TOP)/$(FW_INC_DIR) -I$(FW_INC_DIR)
EXTRA_CFLAGS += -iquote$(LINUX_PPC_INC_DIR) -I$(CL_DIR) -I$(TOP)
EXTRA_CFLAGS += -I$(TOP)/include
EXTRA_ASFLAGS += -D__KERNEL__ -D__FUSEDOS__ -D__SPCM__
include ../Make.rules

OBJS = monitor.o start.o upc_glue.o upc_p.o
LDFLAGS = -static --script=ld_script -nostdlib --entry=monitor_start_1

spcm: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o spcm
	$(OBJDUMP) -dlS spcm > spcm.objdump

upc_glue.o: upc_glue.c
	$(CC) -m64 -g -O2 -Werror -Wall -fno-strict-aliasing -D__FUSEDOS__ -D__SPCM__ -I. -I$(TOP)/include/spi/include/kernel/cnk -I$(TOP)/include -I$(TOP) -c -o upc_glue.o upc_glue.c

upc_p.o: upc_p.c
	$(CC) -m64 -g -O2 -Werror -Wall -fno-strict-aliasing -D__FUSEDOS__ -D__SPCM__ -I. -I$(TOP)/include/spi/include/kernel/cnk -I$(TOP)/include -I$(TOP) -c -o upc_p.o upc_p.c

clean:
	rm -f spcm spcm.objdump $(OBJS)
