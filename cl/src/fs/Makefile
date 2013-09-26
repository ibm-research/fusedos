EXTRA_CFLAGS += -D__KERNEL__
include ../../../Make.rules

FS_OBJS = descriptor.o filesetup.o virtFS.o shmFS.o sharedmemFS.o

fs.a: $(FS_OBJS)
	$(AR) $(ARFLAGS) fs.a $(FS_OBJS)

virtFS.o: filesetup.h fusedosFS.h

clean:
	rm -f fs.a $(FS_OBJS)
