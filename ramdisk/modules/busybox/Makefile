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

MAX_LOAD=$(shell grep -c '^processor' /proc/cpuinfo)

TAR_FILE = busybox.asf.tar.gz
PREBUILD = $(ASF_PREBUILD_DIR)/$(TAR_FILE)

SOURCE_FILE = $(ASF_PACKAGE_DIR)/busybox-1.17.3.tar.bz2

all: # build/busybox

prebuild:
	@if [ ! -e build/busybox ]; then \
		mkdir -p build; \
		$(TAR) -xzf $(PREBUILD) -C build ; \
	fi

build patch: patches/config.patch $(SOURCE_FILE)
	tar -xjf $(SOURCE_FILE)
	$(PATCH) -d busybox-1.17.3 -p 1 < patches/config.patch
	$(MV) busybox-1.17.3 build
	touch build/do_compile

build/busybox: build
	CFLAGS=$(ASF_GLOBAL_CFLAGS) $(MAKE) -s -j $(MAX_LOAD) -C build

install: check_asf_target links # $(PRE)
#	install -D -m 0755 $(CURDIR)/build/busybox $(ASF_ROOTFS_DIR)/bin/busybox
	install -D -m 0444 $(CURDIR)/busybox.links $(ASF_ROOTFS_DIR)/etc/.busybox.links

tarfile: build/busybox
	cd build && tar czf $(TAR_FILE) busybox

clean:
	$(RM) build

links: # build/busybox
	@for m in `/usr/sbin/busybox --list`; do \
    	file=`which --skip-alias --skip-functions -- $$m 2>/dev/null`; \
    	if [ -n "$$file" ]; then \
        	echo $$file; \
    	else \
        	echo /bin/$$m; \
		fi \
	done > busybox.links

include $(TOP)/common/make.footer

