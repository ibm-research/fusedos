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

#ifndef _UPCI_MODE_H_  // Prevent multiple inclusion
#define _UPCI_MODE_H_


//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief UPCI Mode Value
 *
 */
//@}

//#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "spi/include/upci/upc_c.h"
#include "spi/include/upci/node_parms.h"
#include "spi/include/upci/upc_p.h"
//#endif                                                                                                                 // FUSEDOS

__BEGIN_DECLS


/**
 * Structure to maintain global UPC operation
 * mode information and associated logic.
 */
typedef struct sUpci_Mode {
    unsigned  upcMode;
    unsigned  ctrMode;
    const Upci_Node_Parms_t *pNodeParms;  // ptr to nodeParms structure
} Upci_Mode_t;



#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
/**
 * \brief Upci_Mode_Init
 * Initialize the thread local mode value.
 * This routine will also cause the hardware config.  This should only be
 * done if no atomic global mode has been established.  Otherwise use
 * Upci_Mode_Merge().
 * \todo only upcMode 0 currently supported.
 * @param[in,out]  pMode       object to initialize
 * @param[in]      upcMode     The UPC mode to initialize on UPC_C (0, 1 or 2)
 * @param[in]      ctrMode     Indicate how to initialize other units and
 *                             whether to also initialize upc_p and
 *                             L2 unit mode and rings - normally yes
 * @param[in]      unit        Which UPC_P unit to attach to UPC_C for UPC modes 1 & 2
 *                             (-1) \ref UPC_LOCAL_UNIT indicates to use the UPC_P associated with the
 *                               the current hw thread.
 * @return
 *   - UPCI_EMODE_MISMATCH
 *   - UPCI_EMODE_MISMATCH_UNIT
 *
 * \note Implementation Notes
 *
 * - A software thread must not move between hardware threads after the Upci_Mode_Init has occurred,
 *   or results are undetermined.  CNK does not implicitly move sw between hardware threads.
 */
int Upci_Mode_Init(Upci_Mode_t *pMode, unsigned upcMode, UPC_Ctr_Mode_t ctrMode, unsigned unit);



/**
 * \brief Upci_Mode_Free
 * Free up this threads use of the UPC hardware
 * Should be paired with Upci_Mode_Init calls
 */
void Upci_Mode_Free();



//! Upci_Mode_Dump
//! Dump object for debug purposes.
void Upci_Mode_Dump(unsigned indent, Upci_Mode_t * o);
#endif                                                                                                                 // FUSEDOS


__END_DECLS

#endif
