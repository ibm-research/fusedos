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
FUSEDOS_BOOT_DIR = $(PWD)/boot
include Make.rules

all:
	cd cl/src    && $(MAKE)
	cd spcm      && $(MAKE)
	cd tests     && $(MAKE)
	# PATCHSET is defined in the ramdisk config file. We are setting
	# it manually because we need to build linux for the kernel
	# modules.
	cd linux     && $(MAKE) ASF_BOOT_DIR=$(FUSEDOS_BOOT_DIR) PATCHSET=default
	cd fusedosfs && $(MAKE)
	cd mufs      && $(MAKE)
	make -C ramdisk ASF_BOOT_DIR=$(FUSEDOS_BOOT_DIR) ASF_TARGET=default
	install -D -m 0755 $(BGQ_FLOOR_DIR)/firmware/bin/bgq_firmware.elf $(FUSEDOS_BOOT_DIR)
	install -D -m 0755 common/boot/bg_console.exp $(FUSEDOS_BOOT_DIR)
	install -D -m 0755 common/boot/boot_bgq.sh $(FUSEDOS_BOOT_DIR)

clean:
	make -C linux     clean
	make -C cl/src    clean
	make -C spcm      clean
	make -C fusedosfs clean
	make -C mufs      clean
	make -C tests     clean
	make -C ramdisk ASF_BOOT_DIR=$(FUSEDOS_BOOT_DIR) ASF_TARGET=default clean

distclean:
	make -C linux distclean
	make -C cl/src    clean
	make -C spcm      clean
	make -C fusedosfs clean
	make -C mufs      clean
	make -C tests     clean
	make -C ramdisk ASF_BOOT_DIR=$(FUSEDOS_BOOT_DIR) ASF_TARGET=default distclean
	rm -rf $(FUSEDOS_BOOT_DIR)

