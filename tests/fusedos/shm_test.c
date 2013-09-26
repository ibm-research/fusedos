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
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static inline void ppc_msync(void) { do { asm volatile ("msync" : : : "memory"); } while(0); }

#define SHM_NAME "foobar-shm"

#define COUNT 12345678

int main(int argc, char **argv) {

	size_t shm_length = sysconf(_SC_PAGE_SIZE);

	int is_creator = 1;
	// try to create the shared memory area
    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG);

	if(fd<0) {
		printf("creating shm area with O_CREAT did not work, %d %s\n", errno, strerror(errno));

		// we could not create the shm area -- probably it is already there 
		fd = shm_open(SHM_NAME, O_RDWR, 0);

		if(fd<0) {
			printf("error opening (existing) shared memory area, %d %s\n", errno, strerror(errno));
			if(errno == 38)
				printf("hint: Do you have /dev/shm mounted? Sorry for the inconvenience, I wanted to do it the \"proper\" way.\n");
			return -1;
		}

		is_creator = 0; // note that we are not the creator of the shm area
	}

	// extend file to matching size
	if(ftruncate(fd, shm_length)) {
		printf("error in ftruncate of shared memory area, %d %s\n", errno, strerror(errno));
		return -1;
	}

	void * shm_area = mmap(0, shm_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(shm_area == MAP_FAILED) {
		printf("error mmapping the shared memory area, %d %s\n", errno, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd); // file descriptor not needed any more after mmap

	volatile unsigned int * counter = (unsigned int *) shm_area;

	printf("pid %d has shm area at addr %p, current value %u\n", getpid(), counter, *counter);

	if(is_creator) {
		printf("pid %d is creator of the shm area\n", getpid());
		*counter = 0;
	} else {
		printf("pid %d DID NOT CREATE the shm area\n", getpid());
		sleep(1); // crude way to wait for counter initialization
	}


	int i = 0;

	while( *counter < COUNT ) {
		(*counter)++;
		i++;
		ppc_msync();
	}

	printf("pid %d contributed %d, counter at %u\n", getpid(), i, *counter);

	return 0;
}
