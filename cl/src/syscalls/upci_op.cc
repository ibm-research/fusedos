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


#include "Kernel.h"
#include <fcntl.h>
#include <spi/include/kernel/location.h>
#include <spi/include/upci/upci_syscall.h>
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "cl_upc.h"                                                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

#define DEBUG_PRINT(...) // printf(__VA_ARGS__)

__BEGIN_DECLS

uint64_t sc_UPCI_OP(SYSCALL_FCN_ARGS)
{
    uint64_t op      = (uint64_t)r3;
    uint64_t targ    = (uint64_t)r4;
    void *   pData   = (void*)r5;
    size_t   dsize   = (size_t)r6;

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    TRACESYSCALL(("(I) %s%s: op=%ld, targ=%ld, pData=0x%016lx, dsize=%d\n",                                            // FUSEDOS
                  __func__, whoami(), op, targ, (uint64_t)pData, (int)dsize));                                         // FUSEDOS
#else                                                                                                                  // FUSEDOS
    TRACESYSCALL(("(I) %s%s: op=%ld, targ=%ld, pData=0x%016lx, dsize=%d\n",
                  __func__, whoami(), op, targ, (uint64_t)pData, dsize));
#endif                                                                                                                 // FUSEDOS
    DEBUG_PRINT("(I?) %s%s: op=%ld, targ=%ld, pData=0x%016lx, dsize=%ld\n",
                  __func__, whoami(), op, targ, (uint64_t)pData, dsize);

    if(pData == NULL)
    {
        DEBUG_PRINT("(E) %s%s: pData == NULL\n", __func__, whoami());
        return CNK_RC_SPI(EFAULT);
    }

    if(! VMM_IsAppAddress(pData, dsize))
    {
        DEBUG_PRINT("(E) %s%s: bad app address\n", __func__, whoami());
        return CNK_RC_SPI(EFAULT);
    }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    uint64_t rc = Upci_Do_Syscall(op, targ, pData, dsize);
#else                                                                                                                  // FUSEDOS
    uint64_t rc = Kernel_Upci_Raw_Syscall(op, targ, pData, dsize);                                                     // FUSEDOS
#endif                                                                                                                 // FUSEDOS
    
    DEBUG_PRINT("(I) %s%s: Upci_Do_Syscall Returned rc = %ld\n", __func__, whoami(), rc);

    return CNK_RC_SPI(rc);
}




//void FillPersonalityWA(Personality_t *p)
//{
//    memcpy(p, GetPersonality(), sizeof(Personality_t));
//}


__END_DECLS
