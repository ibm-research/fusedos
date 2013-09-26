/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cl_debug.h"
#include "Kernel.h"

// maintain a shared memory area between CL instances
// 
// thereby provide:
// * a pthread_mutex lock for allocating MU resources

pthread_mutex_t * mu_resource_lock;
pthread_mutex_t * shm_lock;

ShmMgrEntry_t * SharedPool;

typedef struct {
	pthread_mutex_t mu_resource_lock;

	int initialized_mutexes;

	// shared memory for applications, shared between CL instances
	ShmMgr_t shared;

	pthread_mutex_t shm_lock;
	
	ShmMgrEntry_t SharedPool[ SHM_MAX_OPENS ];
} cl_shm_t;

typedef struct {
	ShmMgrEntry_t SharedPool[ SHM_MAX_OPENS ];
} cl_group_shm_t;

static const char * shm_name = "CL_SHM";

// initialization function to be called by cl.cc:main()
int init_cl_shm_area() {
	(void) shm_name;
	size_t shm_length = sysconf(_SC_PAGE_SIZE) * (sizeof(cl_shm_t) / sysconf(_SC_PAGE_SIZE)
			+ !(sizeof(cl_shm_t) % sysconf(_SC_PAGE_SIZE) == 0) );

	int is_creator = 1;
	// try to create the shared memory area
    int fd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG);

	if(fd<0) {
		CL_DEBUG_NOSPC("creating shm area did not work, %d %s\n", errno, strerror(errno));

		// we could not create the shm area -- probably it is already there 
		fd = shm_open(shm_name, O_RDWR, 0);

		if(fd<0) {
			CL_ERROR_NOSPC("error opening (existing) CL shared memory area, %d %s\n", errno, strerror(errno));
			if(errno == 38)
				printf("hint: Do you have /dev/shm mounted? Sorry for the inconvenience, I wanted to do it the \"proper\" way.\n");
			return -1;
		}

		is_creator = 0; // note that we are not the creator of the shm area
	}

	// extend file to matching size
	if(ftruncate(fd, shm_length)) {
		CL_ERROR_NOSPC("error in ftruncate of shared memory area, %d %s\n", errno, strerror(errno));
		return -1;
	}

	cl_shm_t * shm_area = (cl_shm_t *) mmap(0, shm_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(shm_area == MAP_FAILED) {
		CL_ERROR_NOSPC("error mmapping the CL shared memory area, %d %s\n", errno, strerror(errno));
		close(fd);
		return -1;
	}

	close(fd); // file descriptor not needed any more after mmap

	mu_resource_lock = &(shm_area->mu_resource_lock);
	shm_lock         = &(shm_area->shm_lock);

	if(is_creator) {
		int rc;
		pthread_mutexattr_t mutex_attr;

		memset(&mutex_attr, 0 , sizeof(mutex_attr));
		rc = pthread_mutexattr_init(&mutex_attr);

		if(rc) {
			CL_ERROR_NOSPC("error %d initializing attributes for mutexes in CL shm area, %s\n", rc, strerror(rc));
			return -1;
		}

		rc = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
		if(rc) {
			CL_ERROR_NOSPC("error %d setting pshared attribute for mutexes in CL shm area, %s\n", rc, strerror(rc));
			return -1;
		}

		rc = pthread_mutex_init(mu_resource_lock, &mutex_attr);
		if(rc) {
			CL_ERROR_NOSPC("error creating pthread_mutex for MU resource allocation, %d %s\n", rc, strerror(rc));
			return -1;
		}

		rc = pthread_mutex_init(shm_lock, &mutex_attr);
		if(rc) {
			CL_ERROR_NOSPC("error creating pthread_mutex shm_lock for application shared memory, %d %s\n", rc, strerror(rc));
			return -1;
		}
		CL_DEBUG("pthread_mutex_init for shm_lock at %p\n", shm_lock);

		shm_area->initialized_mutexes = 1;

		CL_DEBUG_NOSPC("creator CL initialized MU resource mutex in cl shm area.\n");
	} else {
		if(!(shm_area->initialized_mutexes))
			CL_DEBUG_NOSPC("shm non-creator waiting for mutex initialization.\n");
		while(!(shm_area->initialized_mutexes))
			sched_yield();
		CL_DEBUG_NOSPC("shm non-creator has found MU mutex to be initialized.\n");
	}

    // shared memory for a CL process group
    char group_shm_name[20];
    memset(group_shm_name, 0, sizeof(group_shm_name));
    snprintf(group_shm_name, sizeof(group_shm_name), "CL_SHM_%d", getpid());

    // try to create the shared memory area
    fd = shm_open(group_shm_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG);
    if(fd<0) {
    	CL_DEBUG_NOSPC("creating shm area %s for CL process group did not work, %d %s\n", group_shm_name, errno, strerror(errno));
    	return -1;
    }

    // we do not want to keep the name, but throw the shm area away as
    // soon as all the processes exit
    shm_unlink(group_shm_name);

    shm_length = sysconf(_SC_PAGE_SIZE) * (sizeof(cl_group_shm_t) / sysconf(_SC_PAGE_SIZE)
    		+ ! (sizeof(cl_group_shm_t) % sysconf(_SC_PAGE_SIZE) == 0 ) );

    // extend file to matching size
    if(ftruncate(fd, shm_length)) {
    	CL_ERROR_NOSPC("error in ftruncate of shared memory area for CL process group, %d %s\n", errno, strerror(errno));
    	return -1;
    }

    cl_group_shm_t * group_shm_area = (cl_group_shm_t *) mmap(0, shm_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if(group_shm_area == MAP_FAILED) {
    	CL_ERROR_NOSPC("error mmapping the CL group shared memory area, %d %s\n", errno, strerror(errno));
    	close(fd);
    	return -1;
    }

    close(fd); // file descriptor not needed any more after mmap

    SharedPool = group_shm_area->SharedPool;
    //SharedPool = shm_area->SharedPool;
    return 0;
}

