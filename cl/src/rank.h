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

#include <spi/include/kernel/location.h>
#include <ramdisk/include/services/JobctlMessages.h>

#include <spi/include/mu/GIBarrier.h>

extern MUSPI_GIBarrier_t systemBlockGIBarrier;   // classroute 15 barrier
extern MUSPI_GIBarrier_t systemJobGIBarrier;     // classroute 14 barrier
extern MUSPI_GIBarrier_t systemLoadJobGIBarrier; // classroute 13 barrier

int computeTaskCoordinates(uint32_t startEntry, size_t mapsize, BG_CoordinateMapping_t* map, uint64_t* numEntries);
int generateCoordinates(const char *mapping, BG_CoordinateMapping_t* map, uint32_t np, uint32_t *myRank);
int getMyRank(uint32_t* myrank);
int configureJobClassroutes(struct bgcios::jobctl::SetupJobMessage* jobinfo);
int configureLoadJobClassroutes(int includeNode);

int deconfigureUserGI(void);

extern "C"
{
    int configureBlockGIBarrier();
}
