/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/* IBM Confidential                                                 */
/*                                                                  */
/* Licensed Machine Code Source Materials                           */
/*                                                                  */
/* Product(s):                                                      */
/*     Blue Gene/Q Licensed Machine Code                            */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011, 2012                        */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <pthread.h>
#include <spi/include/l2/barrier.h>
#include <spi/include/l2/atomic.h>
#include <spi/include/kernel/location.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define NUM_THREADS 20
#define COUNT_LIMIT 12345678
#define MATCH_INDICATOR 0x8000000000000000L

struct {
	L2_Barrier_t barrier;
	uint64_t counter;
	uint64_t bound;
} common_data = { .barrier = L2_BARRIER_INITIALIZER, .counter = 0, .bound = 12345678 };

uint64_t contributions[NUM_THREADS];
pthread_t threads[NUM_THREADS];


void * thread_func(void * data) {
	uint64_t thread_no = (uint64_t) data;

	contributions[thread_no] = 0;

	printf("thread_no %2lu, Kernel_ProcessorThreadId %d, ProcessorID %2d, __l2_op_ptr %p\n",
			thread_no, Kernel_ProcessorThreadID(), Kernel_ProcessorID(),
			__l2_op_ptr(&(common_data.counter), L2_ATOMIC_OPCODE_LOAD_INCREMENT_BOUNDED));


	L2_Barrier(&(common_data.barrier), NUM_THREADS);

	while(L2_AtomicLoadIncrementBounded(&(common_data.counter)) != MATCH_INDICATOR) contributions[thread_no]++;


	return NULL;
}

int main(int argc, char *argv[])
{
	int i;
	int rc;
	uint64_t sum;

	// TODO init
	rc = Kernel_L2AtomicsAllocate(&common_data, sizeof(common_data));
	if(rc) {
		printf("error %d calling Kernel_L2AtomicsAllocate, %s\n", rc, strerror(rc));
		exit(-1);
	}

	printf("common counter at address %p, <<5 that is %p\n",
			&(common_data.counter),
			(void *) ((uint64_t)&(common_data.counter) << 5) );

	printf("starting threads...");
	for(i = 1; i < NUM_THREADS; i++)
		pthread_create(&(threads[i]), NULL, thread_func, (void *) (uint64_t)i);
	printf(" done\ncounting...");

	thread_func((void *)0);

	for(i = 1; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	printf(" done\ncounter is %lu, contributions are\n", common_data.counter);
	sum = 0;
	for(i = 0; i < NUM_THREADS; i++) {
		printf("%d: %lu\n", i, contributions[i]);
		sum += contributions[i];
	}

	printf("sum: %lu, %s matching limit of %lu\n", sum, (sum == COUNT_LIMIT ? "" : "not"), (uint64_t)COUNT_LIMIT);

	return 0;
}
