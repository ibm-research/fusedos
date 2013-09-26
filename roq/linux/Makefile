ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

ROQ_TRACING_1 = 0

export CFLAGS=


ifeq ($(ROQ_TRACING_1),1)
TRACE_FLAGS = -DPKTRACE_ON
endif

CL_DIR=$(TOP)/cl
BGQ_DRIVER_DIR=$(TOP)/include

BUILDROOT     = $(shell pwd)
EXTRA_CFLAGS := -g -I$(obj) -I$(TOP)/roq/microcode/include -DOFA_VERSION=141 $(ASF_GLOBAL_CFLAGS) $(TRACE_FLAGS)

obj-m    := roq.o
roq-objs := roq_main.o roq_cm.o roq_verbs.o \
	    roq_utils.o roq_eth.o \
	    roq_linux_trace.o roq_device.o
ccflags-y := -D__LINUX__ -D__ROQ__ -D__FUSEDOS__ $(EXTRA_CFLAGS)
ROQRC     = roq

ifeq ($(USERSPACE_EXTENSION), 1)
roq-objs += us_extension/roq_usmu.o us_extension/ionlink.o
EXTRA_CFLAGS := -I$(TOP)/roq/microcode/roq_userspace \
                -I$(TOP)/roq/linux/us_extension -DUSERSPACE_EXTENSION=1 \
                -I$(TOP)/roq/linux/us_extension/phoney \
                -I$(BGQ_DRIVER_DIR) -I$(BGQ_DRIVER_DIR)/spi/include/kernel/klinux \
                -I$(obj) -I$(TOP)/roq/microcode/include -DOFA_VERSION=141 $(ASF_GLOBAL_CFLAGS) $(TRACE_FLAGS) -D__LINUX__
ROQRC = us_extension/roq
else
EXTRA_CFLAGS := -I$(obj) -I$(TOP)/roq/microcode/include -DOFA_VERSION=141 $(ASF_GLOBAL_CFLAGS) $(TRACE_FLAGS) \
                -I$(BGQ_DRIVER_DIR) -I$(BGQ_DRIVER_DIR)/spi/include/kernel/klinux -DDEBUG
ROQRC = roq
endif

modules:
	$(MAKE) -C $(ASF_LINUX_BUILD_DIR) M=$(BUILDROOT) V=1

install: modules
	INSTALL_MOD_PATH=$(ASF_ROOTFS_DIR) INSTALL_MOD_STRIP=1 $(MAKE) -C $(ASF_LINUX_BUILD_DIR) M=$(BUILDROOT) modules_install
	install -D -m 0755 $(CURDIR)/$(ROQRC) $(ASF_ROOTFS_DIR)/etc/init.d/roq
	install -D -m 0755 $(CURDIR)/roq_link_shutdown $(ASF_ROOTFS_DIR)/etc/init.d/roq_link_shutdown
	install -d -m 0755 $(ASF_ROOTFS_DIR)/etc/libibverbs.d
	echo "driver roq" > $(ASF_ROOTFS_DIR)/etc/libibverbs.d/roq.driver

clean:
	rm -f *.ko *.o .*.cmd .*.o.flags *.mod.c Module.symvers Module.markers modules.order
	rm -f us_extension/*.ko us_extension/*.o us_extension/.*.cmd us_extension/.*.o.flags \
	      us_extension/*.mod.c us_extension/Module.symvers us_extension/Module.markers \
	      us_extension/modules.order *.ko.unsigned
	rm -rf .tmp_versions
