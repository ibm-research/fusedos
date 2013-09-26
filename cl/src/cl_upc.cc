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
#include "cl_upc.h"

static int mmio_fd;

int Kernel_Upci_Raw_Syscall(uint64_t op, uint64_t target, void* pData, size_t dSize)
{
    struct upci_op_args {
        uint64_t target;
        void* pData;
        size_t dSize;
    } args;
    args.target = target;
    args.pData = pData;
    args.dSize = dSize;
    return (int)ioctl(mmio_fd, op, &args);
}                                                                                  

int Upci_Init_Linux(int pin)
{
    if (pin) {
        cpu_set_t mask;
        CPU_ZERO(&mask);
        CPU_SET(0, &mask);
        sched_setaffinity(0, sizeof(cpu_set_t), &mask);
    }
    mmio_fd = open("/upcfs/mmio", O_RDWR);
    if (mmio_fd == -1) {
        CL_ERROR("open /upfs/mmio failed\n");
        return(-1);
    }
    void* mmio = mmap((void*)PHYMAP_MINADDR_MMIO, PHYMAP_SIZE_MMIO, PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_FIXED, mmio_fd, 0);
    if (mmio == MAP_FAILED) {
        CL_ERROR("mmap /upfs/mmio failed\n");
        return(-1);
    }
    CL_DEBUG("mmio %p, mmio_fd %d\n", mmio, mmio_fd);
    return(0);
}
