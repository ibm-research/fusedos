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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

//#define __INLINE__ inline
#include "Kernel.h"
#include "fusedos_config.h"
#include "fusedos.h"
#include "hwi/include/bqc/classroute.h"
//#include "spi/include/kernel/MU.h"
//#include "spi/include/kernel/collective.h"
//#include "spi/include/kernel/gi.h"

#include "cl_debug.h"

// temporarily disabled and unused
#if 0
int cl_classroute_setup(const char * cr_config_file, Personality_Networks_t *network_config) {

	int i;
	int coll_crs_used[BGQ_COLL_CLASS_MAX_CLASSROUTES];
	int gi_crs_used[BGQ_GI_CLASS_MAX_CLASSROUTES];

	FILE * f = fopen(cr_config_file, "r");

	if(f == NULL) {
		CL_ERROR_NOSPC("Error %d opening file %s, %s\n", errno, cr_config_file, strerror(errno));
		return -1;
	}

	for(i=0; i<BGQ_COLL_CLASS_MAX_CLASSROUTES; i++) coll_crs_used[i] = 0;
	for(i=0; i<BGQ_GI_CLASS_MAX_CLASSROUTES; i++)   gi_crs_used[i] = 0;


	// parse class route config
	char * line = NULL;
	size_t buflen = 0;
	ssize_t linelen;

	while( (linelen = getline( &line, &buflen, f)) >= 0 ) {
		uint32_t a_coord, b_coord, c_coord, d_coord, e_coord;
		unsigned int rank;
		unsigned int cr_id;
		char type[5];
		uint16_t input, output;

		if( linelen == 0 )
			continue;

		// skip header lines starting with |
		if(line[0] == '|')
			continue;

		// try to parse the line
		if( sscanf(line, " %2u| %2u| %2u| %2u| %2u:%u-%u: %4s :%hx:%hx",
					&a_coord, &b_coord, &c_coord, &d_coord, &e_coord,
					&rank, &cr_id, type, &input, &output) == 10) {
			// success

			// skip lines for other nodes 
			if( a_coord != network_config->Acoord
					|| b_coord != network_config->Bcoord
					|| c_coord != network_config->Ccoord
					|| d_coord != network_config->Dcoord
					|| e_coord != network_config->Ecoord )
				continue;

			// skip class routes with id 15, firmware sets them up for
			// us
			if(cr_id == 15)
				continue;

			printf("would configure %s classroute %u for %x in / %x out\n", type, cr_id, input, output);
			//
		} else {
			line[ (strlen(line) - 1) ] = '\0';
			CL_ERROR_NOSPC("failed to parse line \"%s\"\n", line);
		}
	}

	//
	//
	// configure class routes 
	//
	// TODO we cannot keep the class routes allocated, can we?
	// TODO does PAMI allocate the predefined classroutes?
	// TODO does unallocate reset the route config?!
	//
	// TODO configure / setup barriers that we prepare for the
	// application
	// TODO drop cr 15, as we do not need that
	// TODO only match to the class routes of our rank!!

	// initialize barriers
	//
	return 0;
}
#endif
