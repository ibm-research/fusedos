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

// Includes
#include "Kernel.h"
#include "NodeController.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "JobControl.h"
#include "ToolControl.h"
#include <firmware/include/mailbox.h>
#include "rank.h"

// Define nodectl object in static storage.
static NodeController nodectl;

int Node_Init(void)
{
   if (Personality_CiosEnabled()) {
      return nodectl.init();
   }
   return 0;
}

int Node_Term(void)
{
   if (Personality_CiosEnabled()) {
      return nodectl.term();
   }
   return 0;
}
#endif                                                                                                                 // FUSEDOS

void __NORETURN Node_Run(void)
{

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   // If CIOS is enabled, use the I/O node to run jobs.
   if (Personality_CiosEnabled()) {

      // Tell the control system this node is initialized and ready to run jobs.
      NodeState.FW_Interface.sendBlockStatus(JMB_BLOCKSTATE_INITIALIZED,0,0);
      Kernel_WriteFlightLog(FLIGHTLOG, FL_KERNREADY, 0,0,0,0);
      
      // Poll for messages and call message handlers.
      nodectl.run();

      // No return
   }

   // If the application is preloaded in memory, finish the load and run the job.
   if (Personality_ApplicationPreLoaded()) {
#else                                                                                                                  // FUSEDOS
   if (1) {                                                                                                            // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      App_RunPreLoadedApp();
      // No return
   }

   printf("(I) Nothing to do, parking node controller thread\n");
   while (1);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int Node_ReportConnect(int cnv_error, uint64_t address, uint16_t port)
{
   if (Personality_CiosEnabled()) {
      return nodectl.report_connect_result(cnv_error, address, port);
   }
   return 0;
}

int
NodeController::init(void)
{
    // Where to initialize context?  How do we get context pointer here?
    assert(0 == cnv_open_dev(&_context));
    
    // Create a protection domain.
    assert(0 == cnv_alloc_pd(&_protectionDomain, _context));

    // Create a completion queue.
    assert(0 == cnv_create_cq(&_completionQ, _context, 2));
    
    // Initialize job control.
    assert(0 == jobControl.init(&_protectionDomain, &_completionQ));
    TRACE( TRACE_Nodectl, ("(I) NodeController::init%s: initialized job control\n", whoami()) );
    
    // Initialize tool control.
    assert(0 == toolControl.init(&_protectionDomain, &_completionQ));
    TRACE( TRACE_Nodectl, ("(I) NodeController::init%s: initialized tool control\n", whoami()) );
    
    return 0;
}

int
NodeController::term(void)
{
   int rc = 0;

   // Terminate tool control.
   int err = toolControl.term();
   if (err != 0) {
      rc = err;
   }

   // Terminate job control.
   err = jobControl.term();
   if (err != 0) {
      rc = err;
   }

   // Destroy completion queue.
   err = cnv_destroy_cq(&_completionQ);
   if (err != 0) {
      TRACE( TRACE_Nodectl, ("(E) NodeController::term%s: cnv_destroy_cq() failed, error %d\n", whoami(), err) );
      rc = err;
   }

   // Deallocate protection domain.
   err = cnv_dealloc_pd(&_protectionDomain);
   if (err != 0) {
      TRACE( TRACE_Nodectl, ("(E) NodeController::term%s: cnv_destroy_pd() failed, error %d\n", whoami(), err) );
      rc = err;
   }

   TRACE( TRACE_Nodectl, ("(I) NodeController::term%s: termination is complete\n", whoami()) );
   return rc;
}

int
NodeController::run(void)
{
   uint32_t jobctlQueuePairNum = jobControl.getQueuePairNum();
   uint32_t toolctlQueuePairNum = toolControl.getQueuePairNum();

   while (1) {
      // Wait for completion for the work request posted above.
      int totalCompletions = 1;
      cnv_wc completions[totalCompletions];
      int foundCompletions = 0;
      int err = cnv_poll_cq(&_completionQ, totalCompletions, completions, &foundCompletions, CNV_ANY_PROC_ID);
      if (err != 0) {
         printf("+++ failed to remove completions, error %d\n", err);
         return err;
      }

      // Run list of completions.
      for (int index = 0; index < foundCompletions; ++index) {
         switch (completions[index].opcode) {
            case CNV_WC_SEND:
            {
               TRACE( TRACE_Nodectl, ("(I) NodeController::run(): send operation completed successfully on queue pair %u\n",
                                      completions[index].qp_num) );
               break;
            }

            case CNV_WC_RECV:
            {
               // Process the message.
               TRACE( TRACE_Nodectl, ("(I) NodeController::run(): receive operation completed successfully on queue pair %u\n",
                                      completions[index].qp_num) );
               if (completions[index].qp_num == jobctlQueuePairNum) {
                  jobControl.processMessage(completions[index].wr_id);
               }
               else if (completions[index].qp_num == toolctlQueuePairNum) {
                  toolControl.processMessage((uint32_t)completions[index].wr_id);
               }
               else {
                  printf("(E) NodeController::run: queue pair %u from receive completion is unknown\n", completions[index].qp_num);
               }
               break;
            }

            default:
            {
               printf("(E) NodeController::run: completion opcode %u is not supported from queue pair %u\n",
                      completions[index].opcode, completions[index].qp_num);
               break;
            }
         }
      }

   }
   return 0;
}

int
NodeController::report_connect_result(int cnv_err, uint64_t address, uint16_t port)
{
    int rc = 0;
    int64_t myCoord = getPersonality()->Network_Config.Acoord;
    myCoord *=256;
    myCoord += getPersonality()->Network_Config.Bcoord;
    myCoord *=256;
    myCoord += getPersonality()->Network_Config.Ccoord;
    myCoord *=256;
    myCoord += getPersonality()->Network_Config.Dcoord;
    myCoord *=256;
    myCoord += getPersonality()->Network_Config.Ecoord;

    // This barrier is not needed for functionality, however it is being included to aid in debug in the event of a hardware problem.
    MUSPI_GIBarrierEnterAndWait( &systemBlockGIBarrier );

    volatile int64_t minCoord = cnv_err ? myCoord : -1;
    MUHWI_Destination_t dest;
    dest.Destination.Destination = 0;
    mudm_bcast_reduce(NodeState.MUDM, 
                      MUDM_REDUCE_ALL,
                      MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN,
                      (void*)&minCoord,
                      sizeof(minCoord),
                      dest,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                      15,
                      MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                      (void*)&minCoord);

    // If the returned MIN value does not equal the original initialized value, then at least one node 
    // had a connection problem.
    if (minCoord != -1)
    {
        // All of the nodes will now participate in a collective to determine the max number of failed connections
        uint64_t totalfails = cnv_err ? 1 : 0;
        mudm_bcast_reduce(NodeState.MUDM, 
                          MUDM_REDUCE_ALL,
                          MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD,
                          (void*)&totalfails,
                          sizeof(totalfails),
                          dest,  // ignored for MUHWI_COLLECTIVE_TYPE_ALLREDUCE
                          15,
                          MUHWI_COLLECTIVE_TYPE_ALLREDUCE,
                          (void*)&totalfails);

        // If we match the results of the mudm reduce operation,
        // then we are responsible for reporting this failure.
        if (minCoord == myCoord) 
        {
            RASBEGIN(4);
            RASPUSH(address);
            RASPUSH(port);
            RASPUSH(cnv_err);
            RASPUSH(totalfails);
            RASFINAL(RAS_KERNELCNVCONNECTFAIL);

            rc = -1; // return indication that this node generated ras

    }


    }
    return rc; // return indication that we did not report an error (note: this doesn't mean a cnv_err was not present)
}
#endif                                                                                                                 // FUSEDOS