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

SOURCE_REPO	= https://github.com/chaos/diod.git

PREFIX = /opt/diod

.PHONY: install

all: build/installed

build/configure:
	$(RM) build
	git clone $(SOURCE_REPO) build
	(cd build; patch -p1 < ../fc19.patch )
	touch build/configure
	mkdir -p build/$(PREFIX)
	touch build/do_compile

build/Makefile: build/configure
	cd build; CFLAGS="$(ASF_GLOBAL_CFLAGS)" ./configure --prefix=$(PREFIX)

build/installed: build/Makefile
	@$(MAKE) -C build -j $(MAX_JOBS)
	@$(MAKE) DESTDIR=$(CURDIR)/build -C build install-strip
	touch build/installed

install: check_asf_target $(PRE) build/installed
	cd build; find .$(PREFIX)/{bin,lib,libexec,sbin} \
		! -name "*.a" ! -name "*.la" \
		| cpio -pdu $(ASF_ROOTFS_DIR)/
	install -D -m 0755 diod	$(ASF_ROOTFS_DIR)/etc/init.d/diod

clean:
	$(RM) build

include $(TOP)/common/make.footer

