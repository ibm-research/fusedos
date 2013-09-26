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
/* (C) Copyright IBM Corp.  2009, 2012                              */
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
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "flih.h"
#endif                                                                                                                 // FUSEDOS
#include <spi/include/kernel/collective.h>

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#else                                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

uint64_t sc_QUERYCOLLECTIVECLASSROUTE( SYSCALL_FCN_ARGS )
{
    uint32_t* nClassRoutes = (uint32_t*)r3;
    uint32_t* classRouteIds = (uint32_t*)r4;
    size_t    sizeOfClassRouteIds = r5;
    int32_t rc;

    if (!VMM_IsAppAddress(nClassRoutes, sizeof(uint32_t))) 
    {
        return CNK_RC_SPI(EFAULT);
    }
    if (!VMM_IsAppAddress(classRouteIds, sizeOfClassRouteIds))
    {
        return CNK_RC_SPI(EFAULT);
    }
    
    rc = Kernel_QueryCollectiveClassRoutes(nClassRoutes, classRouteIds, sizeOfClassRouteIds);
    
    return CNK_RC_SPI(rc);
}

uint64_t sc_ALLOCATECOLLECTIVECLASSROUTE( SYSCALL_FCN_ARGS )
{
    int32_t id = (int32_t)r3;
    int32_t rc;
    
    rc = Kernel_AllocateCollectiveClassRoute(id);
    
    return CNK_RC_SPI(rc);
}


uint64_t sc_DEALLOCATECOLLECTIVECLASSROUTE( SYSCALL_FCN_ARGS )
{
    int32_t id = (int32_t)r3;
    int32_t rc;
    
    rc = Kernel_DeallocateCollectiveClassRoute(id);
    
    return CNK_RC_SPI(rc);
}

uint64_t sc_SETCOLLECTIVECLASSROUTE( SYSCALL_FCN_ARGS )
{
    int32_t id = (int32_t)r3;
    ClassRoute_t* cr = (ClassRoute_t*)r4;
    int32_t rc;

    if (!VMM_IsAppAddress(cr, sizeof(ClassRoute_t)))
    {
        return CNK_RC_SPI(EFAULT);
    }    
    rc = Kernel_SetCollectiveClassRoute(id, cr);
    
    return CNK_RC_SPI(rc);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_CLEARINJECTIONCHECKSUM( SYSCALL_FCN_ARGS )
{
    uint64_t linkID = (uint64_t)r3;
    int32_t rc;
    rc = Kernel_ClearInjectionChecksum(linkID);
    return CNK_RC_SPI(rc);
}
#endif                                                                                                                 // FUSEDOS
