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

MAX_JOBS=$(shell grep -c '^processor' /proc/cpuinfo)

SLURM_VERSION = 2.6.1
# 2.3.0-2
SOURCE_FILE = slurm-$(SLURM_VERSION).tar.bz2

PREFIX = /opt/slurm

.PHONY: install

all: build/installed

build/configure:
	$(RM) build
	wget http://www.schedmd.com/download/archive/$(SOURCE_FILE)
	$(TAR) -xjf $(SOURCE_FILE)
	$(MV) slurm-$(SLURM_VERSION) build
	touch build/configure
	perl -pi -e 's/bg_default_dirs=".*"/bg_default_dirs=""/' build/configure
	mkdir -p build/$(PREFIX)
	touch build/do_compile

build/Makefile: build/configure
	cd build; CFLAGS="$(ASF_GLOBAL_CFLAGS)" ./configure --prefix=$(PREFIX) \
					--sysconfdir=/opt/slurm/etc \
					-without-readline \
					--disable-gtktest

build/installed: build/Makefile
	@$(MAKE) -C build -j $(MAX_JOBS)
	@$(MAKE) DESTDIR=$(CURDIR)/build -C build install-strip
	touch build/installed

install: check_asf_target $(PRE)
	cd build; find .$(PREFIX)/{bin,lib,libexec,sbin} \
		! -name "*.a" ! -name "*.la" \
		| egrep -v "bin/sacc*|bin/sview|bin/strigger|bin/sreport|bin/sshare" \
		| cpio -pdu $(ASF_ROOTFS_DIR)/
	install -D -m 0755 slurm 		 			$(ASF_ROOTFS_DIR)/etc/init.d/slurm
	install -D -m 0644 slurm.sh 	 			$(ASF_ROOTFS_DIR)/etc/profile.d/slurm.sh
	install -D -m 0644 slurm_ld.conf 			$(ASF_ROOTFS_DIR)/etc/ld.so.conf.d/slurm.conf
	install -D -m 0644 slurm.conf 	 			$(ASF_ROOTFS_DIR)$(PREFIX)/etc/slurm.conf.tmpl
	install -D -m 0644 slurm.cert 	 			$(ASF_ROOTFS_DIR)$(PREFIX)/etc/slurm.cert
	install -D -m 0644 slurm.key  	 			$(ASF_ROOTFS_DIR)$(PREFIX)/etc/slurm.key
	install -D -m 0755 scripts/listnodes.pl  	$(ASF_ROOTFS_DIR)$(PREFIX)/bin/listnodes.pl
	install -D -m 0755 scripts/update_state.sh 	$(ASF_ROOTFS_DIR)$(PREFIX)/bin/update_state.sh

clean:
	$(RM) build $(SOURCE_FILE)

include $(TOP)/common/make.footer
