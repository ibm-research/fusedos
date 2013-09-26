ifndef TOP
   TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
   export TOP
endif
include $(TOP)/common/make.rules

TESTCASE=roq_ud_qp_test

VPATH=../src/

SRCS=roq.cpp roq_device_interface.cpp roq_bic.cpp roq_ud_qp_test.cpp bgq_hw_abstraction.cpp
CSRCS=../../linux/bgroq_verbs.c

COBJS=$(CSRCS:%.c=%.o)
OBJS=$(SRCS:%.cpp=%.o) $(COBJS)

CXX=$(CROSS_CXX)
CXXFLAGS=-m64 \
	 -I. -I.. -I../include \
	 -I$(TOP)/ibm_shared/FxLogger -I../roq \
	 -I$(BGQ_DRIVER_DIR) \
	 -I$(BGQ_DRIVER_DIR)/spi/include/kernel/cnk \
	 -I$(ASF_LINUX_BUILD_DIR)/include \
	 -I$(ASF_LINUX_BUILD_DIR)/arch/powerpc/include \
	 -I../../linux \
	 -D__powerpc64__ \
	 -DROQ_VERBOSE_LEVEL=2 \
	 -DTHREAD_SAFE_QUEUE_FXLOG=1 \
	 -DROQ_CNK \
	 -O0 -g \
	 -DPKFXLOG  \
	 -DPK_LINUX \
	 -DPK_BGP \
	 -D_LINUX_TYPES_H \
	 -D EXPLORE_BIC \
	 -D STANDALONE_UD_QP_TEST \
	 -D__CNK__ \
	 -DROQ_HAS_BGQ_MU \
	 -DROQ_USE_PTHREAD
CC=$(CROSS_CXX)
CFLAGS=$(CXXFLAGS)

SPI_DIR = $(BGQ_DRIVER_DIR)/spi/lib
SPI_LIB = SPI -l SPI_cnk

all: $(TESTCASE).elf

$(TESTCASE).elf: $(OBJS) $(TOP)/ibm_shared/FxLogger/libPkLinux.a
	$(CXX) -o $@ $^ -lpthread -static -L $(SPI_DIR) -l $(SPI_LIB)
	$(RM) ../../linux/bgroq_verbs.o

$(TOP)/ibm_shared/FxLogger/libPkLinux.a:
	$(MAKE) -C $(TOP)/ibm_shared/FxLogger/

BGQ_SVN_DIR=/gsa/yktgsa/home/b/e/benk/sandbox/bgq.mambo_test/bgq
$(BGQ_SVN_DIR)/mambo/multi_mambo/RunCnk.sh:
	make -C $(BGQ_SVN_DIR)/mambo/multi_mambo

runmultimambo: $(TESTCASE).elf $(BGQ_SVN_DIR)/mambo/multi_mambo/RunCnk.sh
	lamboot -v ./hostfile; \
	$(BGQ_SVN_DIR)/mambo/multi_mambo/RunCnk.sh \
	--N 2 \
	--exe $(TESTCASE).elf \
	--pers svchost \
	--map ./map.txt

clean:
	$(MAKE) -C $(TOP)/ibm_shared/FxLogger/ clean
	$(RM) -rf *.d *.o ../src/*.d ../src/*.o $(CSRCS:%.c=%.d) $(TESTCASE).elf* app-schema machinefiles/ multi_mambo_cnk.tcl work ../../linux/bgroq_verbs.o
	@find . -type l -exec $(RM) {} \;

# include dependency files
-include $(SRCS:.cpp=.d)
-include $(CSRCS:.c=.d)
