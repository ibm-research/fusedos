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

CLDIR=$(TOP)/cl/

all: install

install: check_asf_target
	test -d $(ASF_ROOTFS_DIR) || mkdir -p $(ASF_ROOTFS_DIR)
	cd $(CURDIR)/src; tar -cf - . | (cd $(ASF_ROOTFS_DIR); tar -xf -)
	echo -n "ASF Target=$(ASF_TARGET)" > $(ASF_ROOTFS_DIR)/etc/asf-release
	test -n "$(ASF_RELEASE)" && echo " Release=$(ASF_RELEASE)" >> $(ASF_ROOTFS_DIR)/etc/asf-release \
	                         || echo >> $(ASF_ROOTFS_DIR)/etc/asf-release
	mkdir -p $(ASF_ROOTFS_DIR)/opt/fusedos/bin
	mkdir -p $(ASF_ROOTFS_DIR)/opt/fusedos/tests
	mkdir -p $(ASF_ROOTFS_DIR)/opt/fusedos/init 
	install -D -m 0755 $(CLDIR)/src/cl $(ASF_ROOTFS_DIR)/opt/fusedos/bin/cl
	install -D -m 0755 $(CLDIR)/src/sfjs $(ASF_ROOTFS_DIR)/opt/fusedos/bin/sfjs
	install -D -m 0755 $(CLDIR)/src/spcml $(ASF_ROOTFS_DIR)/opt/fusedos/init/spcml
	install -D -m 0755 $(TOP)/spcm/spcm $(ASF_ROOTFS_DIR)/opt/fusedos/init/spcm
	install -D -m 0755 $(TOP)/load_fusedos.sh $(ASF_ROOTFS_DIR)/opt/fusedos/init/load_fusedos.sh
	install -D -m 0755 $(TOP)/fusedosfs/fusedosfs.ko $(ASF_ROOTFS_DIR)/opt/fusedos/init/fusedosfs.ko
	install -D -m 0755 $(TOP)/mufs/mufs.ko $(ASF_ROOTFS_DIR)/opt/fusedos/init/mufs.ko
	install -D -m 0755 $(TOP)/tests/fusedos/hello_world $(ASF_ROOTFS_DIR)/opt/fusedos/tests/hello_world
	strip $(ASF_ROOTFS_DIR)/opt/fusedos/bin/cl
	strip $(ASF_ROOTFS_DIR)/opt/fusedos/bin/sfjs
	strip $(ASF_ROOTFS_DIR)/opt/fusedos/init/spcml
	strip $(ASF_ROOTFS_DIR)/opt/fusedos/init/spcm
	strip $(ASF_ROOTFS_DIR)/opt/fusedos/tests/hello_world

clean:
	$(RM) $(ASF_ROOTFS_DIR)

include $(TOP)/common/make.footer

