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
EXTRA_CFLAGS += -g -D__KERNEL__ -iquote$(CL_DIR)/src
EXTRA_CFLAGS += -iquote$(FW_INC_DIR) -I$(FW_INC_DIR)
EXTRA_CFLAGS += -iquote$(LINUX_PPC_INC_DIR) -I$(TOP) -I$(TOP)/include
EXTRA_CFLAGS += -iquote$(TOP)/include/spi/include/mu/default
export EXTRA_CFLAGS
include ../../Make.rules

OBJS = AppCleanup.o AppSetup.o NodeController.o ProcessSetup.o elfimage.o entry.o kthread.o
OBJS+= lock.o mmap.o statictlb.o util.o vmm.o rank.o syscalls.o cl_cmdline.o cl_debug.o cl_shm.o cl_mu.o cl_upc.o
OBJS+= bgpers_reader.o

# Subset of OBJS
#SPCML_OBJS = AppCleanup.o AppSetup.o NodeController.o ProcessSetup.o elfimage.o entry.o statictlb.o vmm.o
SPCML_OBJS = AppSetup.o NodeController.o ProcessSetup.o elfimage.o entry.o statictlb.o vmm.o cl_debug.o
KILLSPCS_OBJS = killspcs.o

SFJS_OBJS = sfjs.o sfjs_classroute.o sfjs_cmdline.o bgpers_reader.o cl_mu.o cl_debug.o

SUBDIRS = syscalls fs

all: asmConstants.h cl spcml vmm_mapuserspace killspcs sfjs print_size_and_alignment

cl: cl.o $(OBJS) $(SUBDIRS)
	$(CXX) -static -pthread -Xlinker --script=ld_script cl.o $(OBJS) syscalls/syscalls.a fs/fs.a -o $@ -lrt

cl_cmdline.h cl_cmdline.c: cl.ggo
	gengetopt -i cl.ggo

cl.o: cl_cmdline.h

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	cd $@ && $(MAKE)

spcml: spcml.o $(SPCML_OBJS)
	$(CXX) -static -pthread spcml.o $(SPCML_OBJS) -o $@

vmm_mapuserspace: vmm_mapuserspace.c
	$(CC) -static $(CFLAGS) vmm_mapuserspace.c -o $@

killspcs: $(KILLSPCS_OBJS)
	$(CXX) -static -pthread -o $@ $^

sfjs_cmdline.h sfjs_cmdline.c: sfjs.ggo
	gengetopt -i sfjs.ggo

sfjs.o: sfjs_cmdline.h gi_impl.h collective_impl.h

sfjs: $(SFJS_OBJS)
	$(CXX) -static -pthread $^ -o $@

print_size_and_alignment: print_size_and_alignment.c
	gcc $(CFLAGS) $^ -o $@

K_CPU_FLAGS = -m64 -g -O2 -Werror -MMD -MF .dep.$<.d

asmConstants.h: genAsmConstants.c
	@echo "Generating asmConstants.h"
	$(SILENT)$(CC) -D__FUSEDOS__ $(EXTRA_CFLAGS) $(K_CPU_FLAGS) $(K_KFLAGS) $(K_INCLUDES) -S $<
	$(SILENT)awk ' \
	    BEGIN { \
		printf "// THIS FILE IS AUTOMATICALLY GENERATED\n"; \
		printf "// Modify genAsmConstants.c to make changes.\n"; \
		printf "\n"; \
		name = NULL; \
	    } \
	    /^ASM_CONSTANT_[a-zA-Z0-9_]*:$$/ \
		{name = substr($$1, 14, length($$1)-14)} \
	    /^	.quad/ \
	    	{if (name != NULL) { printf "#define %s %s\n", name, $$2; name = NULL}} \
	    /^	.zero/ \
	    	{if (name != NULL) { printf "#define %s 0\n", name; name = NULL}} \
	    ' genAsmConstants.s > asmConstants.h
	$(SILENT)sed -e '1s/genAsmConstants\.o/asmConstants.h/' \
	    .dep.genAsmConstants.c.d > tmp.d
	$(SILENT)mv tmp.d .dep.genAsmConstants.c.d


clean:
	make -C syscalls clean
	make -C fs clean
	rm -f cl spcml cl.o spcml.o $(KILLSPCS_OBJS) $(SFJS_OBJS) $(OBJS) killspcs print_size_and_alignment sfjs vmm_mapuserspace genAsmConstants.s .dep.genAsmConstants.c.d
