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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fusedos_config.h"
#include "fusedos.h"

int main()
{
    spc_context_t *context;
    void* memory;

    int fd = open("/fusedosfs/context", O_RDWR);
    if (fd == -1) {
        perror("context open failed");
        exit(-1);
    }
    context = mmap(NULL, SPC_CONTEXT_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (context == MAP_FAILED) {
        perror("context mmap failed");
        printf("errno %d\n", errno);
        exit(-1);
    }
    printf("context 0x%lx\n", (long unsigned int)(context));
    printf("mem_bot 0x%lx\n", context[0].mem_bot);

    fd = open("/fusedosfs/0_memory", O_RDWR);
    if (fd == -1) {
        perror("memory open failed");
        exit(-1);
    }
    memory = mmap(NULL, SPC_MEMORY_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        perror("memory mmap failed");
        printf("errno %d\n", errno);
        exit(-1);
    }

    strncpy(memory, "Hello world", strlen("Hello world"));
    char buf[1024];
    strncpy(buf, memory, strlen("Hello world"));
    printf("buf %s\n", buf);

    ioctl(fd, SPC_IOCTL_TEST);
    close(fd);

    printf("Done\n");
    return(0);
}
