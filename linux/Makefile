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
ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

MAX_JOBS=$(shell grep -c '^processor' /proc/cpuinfo)

all: install

ifeq ($(ASF_LINUX_VERSION),3.4.22-BGQ-rc3)
# 3.4 kernel only has a source directory. Need to drive from here...
$(ASF_LINUX_VERSION)/.config:
	${CP} $(ASF_LINUX_VERSION)/arch/powerpc/platforms/bgq/config $(ASF_LINUX_VERSION)/.config

install: $(ASF_LINUX_VERSION)/.config
	${MAKE} --directory $(ASF_LINUX_VERSION) -j $(MAX_JOBS)
	install -D -m 0755 $(ASF_LINUX_VERSION)/arch/powerpc/boot/dtbImage.bgq $(ASF_BOOT_DIR)/linux_$(PATCHSET).bgq
	install -D -m 0644 $(ASF_LINUX_VERSION)/System.map $(ASF_ROOTFS_DIR)/boot/System.map
	install -D -m 0644 $(ASF_LINUX_VERSION)/.config $(ASF_ROOTFS_DIR)/boot/config
	install -d -m 0755 $(ASF_ROOTFS_DIR)
	${MAKE} --directory $(ASF_LINUX_VERSION) INSTALL_MOD_PATH=$(ASF_ROOTFS_DIR) modules_install
else
install:
	@${MAKE} --directory $(ASF_LINUX_VERSION) install
endif

clean:
	@${MAKE} --directory $(ASF_LINUX_VERSION) clean

distclean:
	@${MAKE} --directory $(ASF_LINUX_VERSION) distclean

include $(TOP)/common/make.footer
