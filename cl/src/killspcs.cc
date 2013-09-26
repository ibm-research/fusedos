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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fusedos_config.h"
#include "fusedos.h"
#include "cl_debug.h"

pthread_key_t spc_key;

int main(int argc, char *argv[])
{
    int spc_0 = 0;
    int num_spcs = 56;
    int fusedosfs_fd[num_spcs];
    int fd;
    spc_context_t* pc;

    printf("opening SPC context ...");
    fflush(stdout);
	
    fd = open("/fusedosfs/context", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "spc context open failed, errno %d\n", errno);
        exit(-1);
    }

    // Map context and fusedos config at 12 MB
    pc = (spc_context_t*)mmap((void*)SPC_CONTEXT_VADDR, SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE,
                              PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (pc == MAP_FAILED) {
        fprintf(stderr, "spc context mmap failed, errno %d\n", errno);
        exit(-1);
    }

    close(fd);
    printf(" done\n");

    printf("opening SPC memory files...");
    fflush(stdout);

    for (int i = spc_0; i < spc_0 + num_spcs; i++) {
        char fn[32];
        memset(fn, 0, 32);
        sprintf(fn, "/fusedosfs/%d_memory", i);
        fusedosfs_fd[i] = open(fn, O_RDWR);
        if (fusedosfs_fd[i] == -1) {
            CL_ERROR("spc memory open failed, errno %d\n", errno);
            exit(-1);
        }
    }
    printf(" done\n");

    printf("killing all SPC applications by sending them IPIs...");
    fflush(stdout);

    for (int i = spc_0; i < spc_0 + num_spcs; i++) {
	printf(" %d", i);
	fflush(stdout);
        ioctl(fusedosfs_fd[i], SPC_IOCTL_IPI_QUIT, SPC_TO_CPU(i));
    }
	
    printf(" done\n");

    printf("causing SPC monitor to unload application TLB entries...");
    fflush(stdout);
	
    for (int i = spc_0; i < spc_0 + num_spcs; i++) {
	printf(" %d", i);
	fflush(stdout);
        pc[i].command = SPC_EXIT;
        ioctl(fusedosfs_fd[i], SPC_IOCTL_COMMAND, SPC_TO_CPU(i));
        pc[i].command = SPC_UNLOAD_TLB;
        ioctl(fusedosfs_fd[i], SPC_IOCTL_COMMAND, SPC_TO_CPU(i));
    }

    printf(" done\n");
}
