ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

ROQ_SILENT=1
ROQ_TRACING=0
TESTCASE=roq_userspace
VPATH=../src/

#SRCS=roq.cpp
CSRC=roq_device_interface.c bgq_hw_us_abstraction.c roq_userspace.c \
     transport_connection.c roq_tx.c roq_rx.c roq_global.c roq_init.c

OBJS=$(CSRC:%.c=%.o) $(SRCS:%.cpp=%.o)

CC=$(CROSS_CC)
CFLAGS=-m64 -I. \
	 -I../roq_ud_qp_test -I../ -I../include \
	 -I../../linux -I../../linux/us_extension -I../../linux/us_extension/phoney/ \
	 -I$(BGQ_DRIVER_DIR) \
	 -I$(BGQ_DRIVER_DIR)/spi/include/kernel/klinux \
	 -I$(ASF_LINUX_BUILD_DIR)/include \
	 -I$(ASF_LINUX_BUILD_DIR)/arch/powerpc/include \
	 -I$(TOP)/ibm_shared/FxLogger \
	 -D__powerpc64__ \
	 -DBITS_PER_LONG=64 \
	 -DROQ_CNK \
	 -DPK_LINUX \
	 $(ASF_GLOBAL_CFLAGS) \
	 -O2 -g -pg \
	 -DUSERSPACE_ROQ \
	 -DROQ_USE_PTHREAD

ifeq ($(ROQ_SILENT),1)
CFLAGS += -DPKFXLOG_DUMMY_OUT=1
else
ADD_LIB := $(TOP)/ibm_shared/FxLogger/libPkLinux.a
CFLAGS += -DTHREAD_SAFE_QUEUE_FXLOG=1 \
	    -DPKFXLOG  \
	    -DPK_BGP
endif

ifeq ($(ROQ_TRACING),1)
ADD_LIB := $(TOP)/ibm_shared/FxLogger/libPkLinux.a
CFLAGS += -DPKTRACE_ON \
	    -DTHREAD_SAFE_QUEUE_FXLOG=1 \
	    -DPKFXLOG  \
	    -DPK_BGP
endif
CXXFLAGS=$(CFLAGS)

SPI_DIR = $(BGQ_DRIVER_DIR)/spi/lib
SPI_LIB = -l SPI

all: $(TESTCASE)

$(TESTCASE): $(OBJS) $(ADD_LIB)
	$(CROSS_CC) -rdynamic -o $@ $^ $(ADD_LIB) -lpthread -L $(SPI_DIR) $(SPI_LIB)

$(TOP)/ibm_shared/FxLogger/libPkLinux.a:
	make -C $(TOP)/ibm_shared/FxLogger

install: all
	install -D -m 0755 $(CURDIR)/$(TESTCASE) $(ASF_ROOTFS_DIR)/usr/bin/$(TESTCASE)

clean:
	$(RM) -rf *.ii *.s *.d *.o ../src/*.o ../roq_ud_qp_test/src/*.o $(TESTCASE)

# include dependency files
-include $(CSRC:.c=.d)
-include $(SRCS:.cpp=.d)
