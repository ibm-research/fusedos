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
#include <stdio.h>
#include "../include/AppState.h"

static int stringToCoords(char* str, AppNodeCoordinates_t* coords) {

    // coordinate/dimension environment variable are like "<1,2,3,4,5>"

    int num = sscanf(str, "<%hhu,%hhu,%hhu,%hhu,%hhu>",
                     &(coords->aCoord), &(coords->bCoord), &(coords->cCoord),
                     &(coords->dCoord), &(coords->eCoord));

	if(num!=5) {
		CL_DEBUG_NOSPC("failed to decode string %s into coordinates.\n", str);
	}

    return ! (num==5);
}
