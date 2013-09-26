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

.PHONY: install build modules

all: install

BUILD_DIR	 = $(CURDIR)/build/$(ASF_TARGET)
CONFIG_FILE	:= $(CURDIR)/config/$(ASF_TARGET)
ASF_POST_INSTALL ?= /bin/true

modules:
	test -r $(CONFIG_FILE) || exit -1;
	@cat $(CONFIG_FILE) | while read mod; do \
		test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue; \
		$(MAKE) -C $(TOP)/$$mod install || exit $$?; \
	done

externals:
	@mkdir -p $(ASF_ROOTFS_DIR); \
	set -e; TEMPFILE=`mktemp --suffix _$(ASF_TARGET)`; \
	echo -e "###\n### Collecting EXTERNALS\n###"; \
	cat $(CONFIG_FILE) |  while read mod parm;\
	do \
		test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue; \
		test -e $(TOP)/$$mod/EXTERNALS || continue; \
		echo "# $$mod"; \
		set -e; cat $(TOP)/$$mod/EXTERNALS | while read file; \
		do \
			test "$${file:0:1}" == '#' -o -z "$${file:0:1}" && continue; \
			echo "   $$file"; \
			echo "$$file" >> $$TEMPFILE; \
		done; \
	done; \
	echo -e "###\n### Starting rsync from $(ASF_EXTERNALS_DIR) to $(ASF_ROOTFS_DIR)\n###"; \
	set -e; cat $$TEMPFILE | sort | uniq | while read file; \
		do \
			if [ -e $(ASF_EXTERNALS_DIR)/$$file ]; then \
				echo "   $$file"; \
				cd $(ASF_EXTERNALS_DIR) && \
					rsync --chmod=Du+w -p -R -q -a -L .$$file $(ASF_ROOTFS_DIR)/ ; \
			 else \
			 	echo "$$mod: $(ASF_EXTERNALS_DIR)$$file missing;"; \
			 	exit -42; \
			 fi; \
		done; \
	rm -f $$TEMPFILE;

bg_profile: check_asf_target
	@set -e; mkdir -p $(ASF_ROOTFS_DIR)/etc; \
		echo "ASF_TARGET=\"$(ASF_TARGET)\"" > $(ASF_ROOTFS_DIR)/etc/bg_profile
	@if [ -f $(TOP)/roq/microcode/src/roq_tx.c ]; \
	then \
		ROQ_MESSAGE_UNIT_DMA_GET_ENABLED=NO; \
		grep -q '^#define RoQ_MESSAGE_UNIT_DMA_GET_ENABLED' $(TOP)/roq/microcode/src/roq_tx.c && \
			ROQ_MESSAGE_UNIT_DMA_GET_ENABLED=YES; \
		echo "ASF_ROQ_MESSAGE_UNIT_DMA_GET_ENABLED=$$ROQ_MESSAGE_UNIT_DMA_GET_ENABLED" >> $(ASF_ROOTFS_DIR)/etc/bg_profile; \
	fi

build: modules externals bg_profile
	cd $(ASF_ROOTFS_DIR) && find . | grep -vw .keep | cpio -o -H newc | gzip > $(BUILD_DIR)/ramdisk.cpio.gz
	$(CROSS_LD) -b binary -e 0 -o $(BUILD_DIR)/ramdisk.cpio.gz.bin $(BUILD_DIR)/ramdisk.cpio.gz
	$(SED) -e "s#%BUILD_DIR%#$(BUILD_DIR)#" < $(CURDIR)/scripts/elf_header.lds > $(BUILD_DIR)/elf_header.lds
	$(CROSS_LD) -T $(BUILD_DIR)/elf_header.lds -o $(BUILD_DIR)/ramdisk.cpio.gz.bin.elf

install: check_asf_target build
	install -D -m 0644 $(BUILD_DIR)/ramdisk.cpio.gz.bin.elf $(ASF_BOOT_DIR)/ramdisk_$(ASF_TARGET).gz
	install -D -m 0644 $(CONFIG_FILE) $(ASF_BOOT_DIR)/config/$$(basename $(CONFIG_FILE))
	-test -x $(ASF_POST_INSTALL) && $(ASF_POST_INSTALL)

tarfile:
	test -r $(CONFIG_FILE) || exit -1;
	cat $(CONFIG_FILE) | while read mod; do \
		test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue; \
		grep -q "tarfile:" $$(echo "$(TOP)/$$mod/" | cut -d' ' -f1)/Makefile || continue; \
		$(MAKE) -k -i -C $(TOP)/$$mod tarfile; \
	done

clean: check_asf_target
	@tac $(CONFIG_FILE) | while read mod; do \
		test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue; \
		$(MAKE) -C $(TOP)/$$mod clean || exit $$?; \
	done
	$(RM) $(BUILD_DIR)

distclean:
	@for c in config/*; do \
		tac $(CURDIR)/$$c | while read mod; do \
			test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue; \
			$(MAKE) -C $(TOP)/$$mod clean || exit $$?; \
		done \
	done
	@$(RM) build


include $(TOP)/common/make.footer
