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

// cl_mu.c

// include cl_mu.h and define data structures here
#define __CL_MU_EXTERN_
#include "cl_mu.h"

#include <string.h>


void init_cl_mu_data()
{

    memset(inj_fifo_fds, 0, sizeof(inj_fifo_fds));
    memset(rec_fifo_fds, 0, sizeof(rec_fifo_fds));
    memset(bat_fds, 0, sizeof(bat_fds));

	memset(coll_cr_fds, 0, sizeof(coll_cr_fds));
	memset(gi_cr_fds, 0, sizeof(gi_cr_fds));

    for(int i=0; i < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP; i++)
        inj_fifo_fds[i] = -1;

    for(int i=0; i < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; i++)
        rec_fifo_fds[i] = -1;

    for(int i=0; i < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP; i++)
        bat_fds[i] = -1;

	for(int i=0; i < BGQ_COLL_CLASS_MAX_CLASSROUTES; i++)
		coll_cr_fds[i] = -1;

	for(int i=0; i < BGQ_GI_CLASS_MAX_CLASSROUTES; i++)
		gi_cr_fds[i] = -1;
}

