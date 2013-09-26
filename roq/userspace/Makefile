ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

MAX_JOBS=$(shell grep -c '^processor' /proc/cpuinfo)

.PHONY: all libroq

all: libroq

libroq:
	@$(MAKE) -f $(CURDIR)/Makefile.libroq

install: all
	@$(MAKE) -f $(CURDIR)/Makefile.libroq install

clean:
	$(MAKE) -f Makefile.libroq clean

distclean: clean
