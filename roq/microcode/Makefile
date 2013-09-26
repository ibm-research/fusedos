ifndef TOP
    TOP = $(shell while ! test -e common/make.rules; do cd ..; done; pwd)
    export TOP
endif
include $(TOP)/common/make.rules

all:

clean:
	$(MAKE) -C roq_ud_qp_test/ clean
	$(MAKE) -C roq_userspace/ clean
	$(MAKE) -C roq_microcode/ clean
	$(RM) $(OBJS)

# include dependency files
-include $(SRCS:.cpp=.d)
-include $(CSRCS:.c=.d)
