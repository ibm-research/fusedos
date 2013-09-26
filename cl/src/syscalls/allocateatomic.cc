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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "spi/include/kernel/memory.h"
#else                                                                                                                  // FUSEDOS
#include "fusedos_config.h"                                                                                                // FUSEDOS
#include "fusedos.h"                                                                                                       // FUSEDOS
extern uint64_t __KERNEL_END;                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS

//! \brief  Implement the sc_ALLOCATEL2ATOMIC system call.
//! \param  r3,r4,...,r8 Syscall parameters.
//! \return CNK_RC_SPI(0) or CNK_RC_SPI(errno).

uint64_t  sc_ALLOCATEL2ATOMIC(SYSCALL_FCN_ARGS)
{
    int rc = 0;
    uint64_t l2atom_va = r3;
    uint64_t length    = r4;
    AppProcess_t* pProc = GetMyProcess();
    uint64_t sizeMapped = 0;

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACESYSCALL(("FUSEDOS sc_ALLOCATEL2ATOMIC: l2atom_va 0x%lx, length 0x%lx\n", l2atom_va, length));                // FUSEDOS
#else                                                                                                                  // FUSEDOS
    if(Syscall_GetIpiControl() != 0)
    {
        rc = EAGAIN;
    }
#endif                                                                                                                 // FUSEDOS
    if(rc == 0)
    {
        while(length > 0)
        {
#if defined __FUSEDOS__                                                                                               // FUSEDOS
            TRACESYSCALL(("FUSEDOS sc_ALLOCATEL2ATOMIC: sizeMapped 0x%lx\n", sizeMapped));                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
            rc = vmm_allocateAtomic(pProc->Tcoord, l2atom_va, &sizeMapped);
            if(rc)
                break;
            
            if(sizeMapped > length)
                break;
            l2atom_va += sizeMapped;
            length    -= sizeMapped;
        }
#if defined __FUSEDOS__                                                                                               // FUSEDOS
        TRACESYSCALL(("FUSEDOS sc_ALLOCATEL2ATOMIC: sizeMapped after loop 0x%lx\n", sizeMapped));                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Syscall_ReleaseIpiControl();
#endif                                                                                                                 // FUSEDOS
    }
    
    return CNK_RC_SPI(rc);
}
