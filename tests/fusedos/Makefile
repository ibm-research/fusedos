include ../../Make.rules

# Override CFLAGS in ../Make.rules
CFLAGS = -m64 -g -Wall -Werror
#CFLAGS = -m64 -O2 -Wall -Werror
LDFLAGS = -static

CNK_INC_FLAGS = -iquote$(TOP)/include/spi/include/kernel/cnk
CNK_INC_FLAGS+= -I$(TOP)/include -I$(TOP)

TESTPROGS=clone_test dump_context dump_env hello_world
TESTPROGS+=memeater fusedosfs_test pthread_prodcons
TESTPROGS+=shm_test pthread_tally pthread_test
TESTPROGS+=kernel_spi_test l2atomicstest query_free_fifos
ifeq ($(FEDORA), 0)
TESTPROGS+=omp_ex1 mpi_helloworld
endif

OBJDUMPS=$(TESTPROGS:%=%.objdump)

all: $(TESTPROGS) $(OBJDUMPS)

%.objdump: %
	objdump -dlS $< > $@

clone_test: clone_test.c
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -static -o $@ $^

counter: counter.c
	$(CC) $(CFLAGS) -static -o $@ $^

dump_context: dump_context.c
	$(CC) $(CFLAGS) -iquote$(LINUX_PPC_INC_DIR) -static -pthread -D__CL__ -o $@ $^

hello_world: hello_world.c
	$(CC) $(CFLAGS) -c $(CNK_INC_FLAGS) -o hello_world.o hello_world.c
	$(CC) $(LDFLAGS) -o hello_world hello_world.o -lrt

fusedosfs_test: fusedosfs_test.c
	$(CC) $(CFLAGS) -iquote$(LINUX_PPC_INC_DIR) -static -o $@ $^

pthread_test: pthread_test.c
	$(CC) $(CFLAGS) -static -pthread -o $@ $^

pthread_tally: pthread_tally.c
	$(CC) $(CFLAGS) -static -pthread -o $@ $^

pthread_prodcons: pthread_prodcons.cc
	$(CXX) $(CFLAGS) -static -pthread -o $@ $^
	strip $@

memeater: memeater.cc
	$(CXX) $(CFLAGS) -static -o $@ $^
	strip $@

dump_env: dump_env.c
	$(CC) $(CFLAGS) -static -o $@ $^

mpi_helloworld: mpi_helloworld.c
	$(MPICC) $(CFLAGS) -o $@ $^
	strip $@

omp_ex1: omp_ex1.cc
	$(CXX) -o $@ $^ -fopenmp $(CNK_INC_FLAGS)
	strip $@

kernel_spi_test: kernel_spi_test.cc
	$(CXX) $(CFLAGS) $(CNK_INC_FLAGS) -static -o $@ $^

shm_test: shm_test.o
	$(CC) -static -o $@ $^ -lrt

l2atomicstest: l2atomicstest.c
	$(CC) $(CFLAGS) $(CNK_INC_FLAGS) -static -pthread -o $@ $^

query_free_fifos: query_free_fifos.c
	$(CC) $(CFLAGS) -D__FUSEDOS__ $(CNK_INC_FLAGS) -static -o $@ $^
clean:
	rm -f *.objdump *.o $(TESTPROGS) $(OBJDUMPS)
