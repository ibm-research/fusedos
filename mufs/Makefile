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
# This software is available to you under either the GNU General
# Public License (GPL) version 2 or the Eclipse Public License
# (EPL) at your discretion.
#
# ================================================================

DEBUG = y


# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m	:= mufs.o
mufs-y := allocation.o mufs_init.o query.o classroute.o threshold.o primordial_cr.o
ccflags-y := -I$(src)/../ -I$(src)/../include -I$(src)
ccflags-y += -I$(src)/../include/spi/include/kernel/klinux/ -D__FUSEDOS__
ccflags-y += -D__MUFS__ $(DEBFLAGS)

else

# Make.rules from repository top-level
include ../Make.rules

#KERNELDIR ?= $(LINUX_DIR)/$(LINUX_VERS)/build
KERNELDIR ?= $(LINUX_DIR)/$(LINUX_VERS)/
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LINUX_PPC_INC_DIR=$(LINUX_PPC_INC_DIR) modules
	objdump -dlS mufs.ko > mufs.objdump

endif



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.objdump

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
