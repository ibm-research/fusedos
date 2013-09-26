EXTRA_CFLAGS += -D__KERNEL__
include ../../../Make.rules

SYSC_OBJS = allocateatomic.o chown.o clone.o close.o collective.o creat.o
SYSC_OBJS += dup.o exit.o fcntl.o fstat.o fsync.o ftruncate.o futex.o getcwd.o
SYSC_OBJS += getmemregion.o getpersonality.o getpid.o getpvr.o getrank.o
SYSC_OBJS += getrlimit.o getrusage.o gettid.o gettimeofday.o gi.o
SYSC_OBJS += ioctl.o itimers.o jobcoords.o kill.o lseek.o mmap.o mu.o
SYSC_OBJS += open.o poll.o pread64.o ranks2coords.o read.o readlink.o
SYSC_OBJS += sched_getaffinity.o sched_setaffinity.o sched_yield.o
SYSC_OBJS += setrlimit.o signals.o statfs.o socketcall.o stat.o sync.o
SYSC_OBJS += time.o times.o uid.o uname.o unimplemented.o unlink.o
SYSC_OBJS += upci_op.o write.o writev.o

syscalls.a: $(SYSC_OBJS)
	$(AR) $(ARFLAGS) syscalls.a $(SYSC_OBJS)

mu.o: ../MU_impl.h

collective.o: ../collective_impl.h

gi.o: ../gi_impl.h

clean:
	rm -f syscalls.a $(SYSC_OBJS)
