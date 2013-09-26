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

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define COUNT_PER_THREAD 323456L
#define DEFAULT_NUM_THREADS 3

#define USE_LOCK

static uint64_t tally = 0;
#ifdef USE_LOCK
static pthread_mutex_t lock;
#endif

static void * tally_thread(void * data)
{

#ifdef USE_LOCK
    int rc;
#endif
    uint64_t i;
    int j;
    double x = 0, y = 1, z = 3.141;
    for(i = 0; i < COUNT_PER_THREAD; i++) {
#ifdef USE_LOCK
        rc = pthread_mutex_lock( &lock );
        if( rc ) {
            fprintf(stderr, "Error locking mutex: %s\n", strerror(rc) );
            return NULL;
        }
#endif
        tally++;
#ifdef USE_LOCK
        rc = pthread_mutex_unlock( &lock );
        if( rc ) {
            fprintf(stderr, "Error unlocking mutex: %s\n", strerror(rc) );
            return NULL;
        }
#endif
        if(tally % 5000 == 0) {
            printf(".");
            fflush(stdout);
        }

        /*if(i % 1000 == 0) {
        	printf(";");
        	fflush(stdout);
        }*/

        for(j = 0; j < 10; j++) {
            x+= 2.37;
            y*= x;
            z+= y;
        }
    }

    i = (uint64_t)z;
    return (void *) i;
}


int main(int argc, char **argv)
{

    int rc;
    int i;
    int num_threads = DEFAULT_NUM_THREADS;
    uint64_t expected_tally;
    pthread_t * threads;

    if(argc==2)
        num_threads = atoi(argv[1]);

    expected_tally = num_threads * COUNT_PER_THREAD;
    threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

    if(threads == NULL) {
        fprintf(stderr, "Error allocating memory for pthreads");
        exit(-1);
    }

    printf("using %d tally threads\n", num_threads);

#ifdef USE_LOCK
    rc = pthread_mutex_init( &lock, NULL );
    if( rc ) {
        fprintf(stderr, "Error creating mutex: %s\n", strerror(rc) );
        exit(-1);
    }
#endif

    for(i=0; i < num_threads; i++) {
        rc = pthread_create( &(threads[i]), NULL, tally_thread, NULL);
        if( rc ) {
            fprintf(stderr, "Error creating thread %d: %s\n", i, strerror(rc) );
            exit(-1);
        }
    }

    /*printf("sleeping for 5 seconds => take your snapshots now!\n");
    sleep(5);*/

    for(i=0; i < num_threads; i++) {
        rc = pthread_join( threads[i], NULL );
        if( rc ) {
            fprintf(stderr, "Error joining thread %d: %s\n", i, strerror(rc) );
            //exit(-1);
        }
    }

    printf("\ntally is: %"PRId64", should be %"PRId64"\n", tally,
           expected_tally);

    if( tally == expected_tally ) {
        printf("seems to match!\n");
    } else {
        printf("something went wrong!\n");
    }

    return 0;
}
