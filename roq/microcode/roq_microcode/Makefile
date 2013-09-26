ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

ROQ_SILENT=0
ROQ_TRACING_1=0
ROQ_TORUS_WRAP=1

TESTCASE=roq_microcode
VPATH=../src/

#SRCS=roq.cpp
CSRC=entry.c fwext_vprintf.c fwext_util.c roq_kernel.c \
     roq_device_interface.c bgq_hw_us_abstraction.c \
     transport_connection.c roq_tx.c roq_rx.c roq_global.c roq_init.c \
     roq_fw_trace.c

ifeq ($(ROQ_RESILIENCY),1)
CSRC+=../roq_resiliency/roq_resiliency.c \
      ../roq_resiliency/roq_resiliency_forwarder.c \
      ../roq_resiliency/roq_resiliency_gi_checker.c \
      ../roq_resiliency/roq_resiliency_p2p_notify.c \
      ../roq_resiliency/roq_resiliency_router.c \
      ../roq_resiliency/supernodes/sng_build.c \
      ../roq_resiliency/supernodes/sng_common.c \
      ../roq_resiliency/supernodes/sng_route.c \
      ../roq_resiliency/supernodes/torus.c
endif

TXRXSRC=roq_tx.c roq_rx.c invoke_roq_rx.c bgq_hw_us_abstraction.c transport_connection.c \
		roq_device_interface.c \
		roq_global.c \
		roq_init.c \
		cnk_mudm_dummies.c \
		cnk_util.c \
		cnk_entryutil.c \
		fwext_vprintf.c

TXRXOBJS=$(TXRXSRC:%.c=%.o)

ifeq ($(DEBUG_BUFFER), 1)
CSRC+=debug_buffer.c
endif
ASRC=start.S

OBJS=$(CSRC:%.c=%.o) $(ASRC:%.S=%.o) $(SRCS:%.cpp=%.o)
WRAPPER=wrapper_entry.o wrapper.o

INCS := -I. \
	-I../ -I../include \
	-I../../linux -I../../linux/us_extension \
	-I$(BGQ_DRIVER_DIR) \
	-I$(BGQ_DRIVER_DIR)/spi/include/kernel/firmware \
	-I$(TOP)/ibm_shared/FxLogger \
	-I$(ASF_LINUX_BUILD_DIR)/include \
	-I$(ASF_LINUX_BUILD_DIR)/arch/powerpc/include \


CC=$(CROSS_CC)
AS=$(CROSS_CC)
CFLAGS=-m64 $(INCS) \
	 -D__powerpc64__ \
	 -D__FWEXT__ \
	 -D__FUSEDOS__ \
	 -D__PK_API_HPP__ \
	 -DBITS_PER_LONG=64 \
	 -DROQ_CNK \
	 -DPK_LINUX \
	 $(ASF_GLOBAL_CFLAGS) \
	 -DROQ_USE_PTHREAD \
	 -O2 -g \
	 -fno-strict-aliasing -c

ifeq ($(DEBUG_BUFFER), 1)
CFLAGS += -DDEBUG_BUFFER=1
endif

ifeq ($(ROQ_SILENT),1)
CFLAGS += -DPKFXLOG_DUMMY_OUT=1
else
CFLAGS += -DTHREAD_SAFE_QUEUE_FXLOG=1 \
           -DPKFXLOG  \
           -DPK_BGP -DFXLOG_UCODE_INIT
#-DFXLOG_ENABLE -DDEBUG_REMOTE_GET \
#-DFXLOG_UCODE_TX -DFXLOG_TRANCONN_TX -DFXLOG_UCODE_PER_BURST \
#-DFXLOG_UCODE_PER_MESSAGE -DFXLOG_UCODE_CNTL -DFXLOG_UCODE_QP_STATE \
#-DFXLOG_REMOTE_GET -DFXLOG_UCODE_INIT -DPKFXLOG_NACK_HANDLING
endif

ifeq ($(ROQ_TORUS_WRAP),1)
CFLAGS += -DTORUS_WRAPPED
endif

ifeq ($(ROQ_TRACING_1),1)
CFLAGS += -DPKTRACE_ON \
           -DTHREAD_SAFE_QUEUE_FXLOG=1 \
           -DPKFXLOG  \
           -DPK_BGP
endif

ifeq ($(ROQ_RESILIENCY),1)
CFLAGS += -DROQ_RESILIENCY=1 -DROQ_RESILIENCY_BGQ=1
#CFLAGS += -DBENCHMARK=1
CFLAGS += -g
LDFLAGS += -g
endif

ifeq ($(ROQ_RESILIENCY_DEBUG),1)
CFLAGS += -DROQ_RESILIENCY_DEBUG=1
endif


ASFLAGS=$(CFLAGS)
CXXFLAGS=$(CFLAGS)

DOMAINS_LDMAP = -Map $(basename $@).map

SPI_DIR = $(BGQ_DRIVER_DIR)/spi/lib
SPI_LIB = -l SPI

all: $(TESTCASE) libtxrx.a

libtxrx.a: $(TXRXOBJS)
	ar crv libtxrx.a $(TXRXOBJS)

$(TESTCASE): $(OBJS) $(WRAPPER) linker.lds wrapper.lds
	$(CROSS_LD) -static $(DOMAINS_LDMAP) --script linker.lds -o kernel.elf $(OBJS) -static -nostdlib -L$(SPI_DIR) $(SPI_LIB)
	$(CROSS_OBJCOPY) -Obinary kernel.elf kernel.bin
	$(CROSS_OBJCOPY) -I binary -O elf64-powerpc -B powerpc kernel.bin kernel.o
	$(CROSS_LD) -Bstatic -T wrapper.lds -o $@ $(WRAPPER) kernel.o -static -nostdlib

install: all
	$(CP) -f $(TESTCASE) $(ASF_BOOT_DIR)/$(TESTCASE).elf

clean:
	$(RM) -rf *.i *.ii *.s *.d *.o ../src/*.o ../roq_ud_qp_test/src/*.o $(TESTCASE) kernel.elf kernel.bin $(TESTCASE).bin libtxrx.a $(TESTCASE).map

RPM_DIR	:= $(CURDIR)/../../..
rpm: all
	rpmbuild -bb \
		--define "_topdir $(RPM_DIR)"  \
		--define "ROQMICRO_DIR $(CURDIR)" \
		./roq-microcode.spec

# include dependency files
-include $(SRCS:.cpp=.d)
-include $(CSRC:.c=.d)
