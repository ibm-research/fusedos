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

.PHONY: all

all:
	@echo "Nothing todo so far..."

install: all
	install -D -m 0644 $(CURDIR)/ssh_config $(ASF_ROOTFS_DIR)/etc/ssh/ssh_config
	install -D -m 0644 $(CURDIR)/sshd_config $(ASF_ROOTFS_DIR)/etc/ssh/sshd_config
	install -D -m 0600 $(CURDIR)/ssh_host_dsa_key $(ASF_ROOTFS_DIR)/etc/ssh/ssh_host_dsa_key
	install -D -m 0600 $(CURDIR)/ssh_host_dsa_key.pub $(ASF_ROOTFS_DIR)/etc/ssh/ssh_host_dsa_key.pub
	install -D -m 0600 $(CURDIR)/ssh_host_rsa_key $(ASF_ROOTFS_DIR)/etc/ssh/ssh_host_rsa_key
	install -D -m 0600 $(CURDIR)/ssh_host_rsa_key.pub $(ASF_ROOTFS_DIR)/etc/ssh/ssh_host_rsa_key.pub
	install -d -m 0700 $(ASF_ROOTFS_DIR)/var/empty/sshd
	install -D -m 0755 openssh $(ASF_ROOTFS_DIR)/etc/init.d/openssh

clean:
	@echo "Nothing todo so far..."
