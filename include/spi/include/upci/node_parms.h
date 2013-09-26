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

#ifndef _UPCI_NODE_PARMS_H_  // Prevent multiple inclusion
#define _UPCI_NODE_PARMS_H_


//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief Node wide global parameters gathered within the
 *        kernel - but needed in several places in user state.
 *
 */
//@}

#include "spi/include/upci/upc_common.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "spi/include/upci/upc_atomic.h"
#endif                                                                                                                 // FUSEDOS

__BEGIN_DECLS




/**
 * Structure of various static global parameters
 * affecting global the Upci operation
 */
typedef struct sUpci_Node_Parms {
    uint64_t cpuSpeed;   // cpu cycles in Mhz
    uint64_t nwSpeed;    // network cycles in Mhz
    double   cycleRatio; // cpu / network cycle ratio. Use to scale nw metrics into cpu cycles
    short    spareCore;  // the spare physical core #, to align upc_p/upc_c counters when non-standard spare core exists.
    UpciBool_t   dd1Enabled; // cache value to determine if must adjust for physical spare core
    uint64_t nodeConfig; // Personality Node Config
} Upci_Node_Parms_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#define UPCI_NODE_PARMS_INIT { 0, 0, 0, 0, 0, 0 }


/**
 * Get static global node parameters, mostly derived from personality structures,
 * which must be queried throughout many upc, upci, and bgpm operations.
 * Since the data must be retrieved from kernel, this routine will cache the
 * results in static memory to reduce access times.
 * @return pointer to structure stored in static memory.
 * @note the cycleRatio value is only available in user state.  If called from
 * a fwext library the value will be zero.
 */
const Upci_Node_Parms_t *Upci_GetNodeParms();

void Upci_DumpNodeParms(unsigned indent);
#endif                                                                                                                 // FUSEDOS



__END_DECLS

#endif
