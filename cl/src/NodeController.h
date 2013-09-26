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

#ifndef _CNK_NODECONTROLLER_H
#define _CNK_NODECONTROLLER_H

// Includes
#include "Kernel.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <cnk/include/Verbs.h>
#include <ramdisk/include/services/MessageHeader.h>
#endif                                                                                                                 // FUSEDOS

extern "C"
{
   int Node_Init(void);
   int Node_Term(void);
   void __NORETURN  Node_Run(void);
   int Node_ReportConnect(int cnv_error, uint64_t address, uint16_t port);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
class NodeController
{
public:

   //! \brief  Initialize the node controller.
   //! \return 0 when successful, errno when unsuccessful.

   int init(void);

   //! \brief  Terminate the node controller.
   //! \return 0 when successful, errno when unsuccessful.

   int term(void);

   //! \brief  Run the node controller thread, handling messages received from jobctld and toolctld.
   //! \return 0 when successful, errno when unsuccessful.

   int run(void);

   //! \brief  Report the results of this compute node connection with the IO node.
   //! \return 0 when successful, errno when unsuccessful.

   int report_connect_result(int cnv_error, uint64_t address, uint16_t port);


private:

   //! Device context.
   cnv_context *_context;

   //! Protection domain.
   cnv_pd _protectionDomain;

   //! Completion queue.
   cnv_cq _completionQ;

};
#endif                                                                                                                 // FUSEDOS

#endif // _CNK_NODECONTROLLER_H

