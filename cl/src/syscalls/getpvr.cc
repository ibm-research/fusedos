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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

// Includes
#include "Kernel.h"

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cl_debug.h"                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS

uint64_t sc_GETPVR(SYSCALL_FCN_ARGS)
{
    uint32_t* pvr = (uint32_t*)r3;
    if( !VMM_IsAppAddress(pvr, sizeof(uint32_t)))
    {
        return CNK_RC_SPI(EFAULT);
    }
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    *pvr = ProcessorVersion();
#else                                                                                                                  // FUSEDOS
	uint64_t tmp_pvr;
	int fd = open("/fusedosfs/pvr", O_RDONLY); // need to get this from kernel, as PVR SPR is privileged
	if(fd == -1) {
		CL_ERROR("error %d opening /fusedosfs/pvr, %s, cannot serve GETPVR syscall.\n", errno, strerror(errno));
		return CNK_RC_SPI(ENOSYS);
	}
	if( read(fd, &tmp_pvr, sizeof(tmp_pvr)) != sizeof(tmp_pvr) ) {
		close(fd);
		CL_ERROR("error %d reading /fusedosfs/pvr, %s, cannot serve GETPVR syscall.\n", errno, strerror(errno));
		return CNK_RC_SPI(ENOSYS);
	}
	close(fd);
	*pvr = tmp_pvr;
#endif                                                                                                                 // FUSEDOS
    return CNK_RC_SPI(0);
}

