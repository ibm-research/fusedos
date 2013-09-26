ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

MAX_JOBS=$(shell grep -c '^processor' /proc/cpuinfo)

BUILD_COMMON = linux userspace

ifeq ($(USERSPACE_EXTENSION), 1)
BUILD = microcode/roq_userspace $(BUILD_COMMON)
MAKEFLAGS = USERSPACE_EXTENSION=1
else
BUILD = microcode/roq_microcode $(BUILD_COMMON)
endif

CURR_DIR := $(shell pwd)

.PHONY: $(BUILD) all

all:
	set -e; for i in $(BUILD); do \
		make -C $$i; \
	done

install: all
	set -e; for i in $(BUILD); do \
		make -C $$i install; \
	done

clean:
	@set -e; for i in $(BUILD); do \
		make -C $$i clean; \
	done

distclean:
	@for i in $(BUILD_COMMON) firmware; do \
		make -C $$i clean; \
	done
