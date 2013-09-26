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


#include "Kernel.h"
#include "rank.h"
#include <fcntl.h>

__BEGIN_DECLS

uint64_t sc_RANKS2COORDS(SYSCALL_FCN_ARGS)
{
    size_t                  mapsize    = (size_t)r3;
    BG_CoordinateMapping_t* map        = (BG_CoordinateMapping_t*)r4;
    uint64_t*               numentries = (uint64_t*)r5;
        
    if(! VMM_IsAppAddress(numentries, sizeof(uint64_t)))
    {
        return CNK_RC_SPI(EFAULT);
    }
    
    if(! VMM_IsAppAddress(map, mapsize))
    {
        return CNK_RC_SPI(EFAULT);
    }
    
    return CNK_RC_SPI(computeTaskCoordinates(0, mapsize, map, numentries));
}

__END_DECLS
