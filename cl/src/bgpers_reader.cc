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
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fusedos_config.h"
#include "fusedos.h"
#include "cl_debug.h"

int readPersonalityIntoEnvironment() {
    FILE* bgpers = fopen("/dev/bgpers", "r");

    if (bgpers==NULL) {
        CL_ERROR_NOSPC("cannot open /dev/bgpers to read Personality data, %s\n", strerror(errno));
        return -1;
    }

    char* buf      = NULL; // let getline do the allocation work
    size_t bufsize = 0;
    ssize_t linelen;

    while ((linelen=getline(&buf, &bufsize, bgpers)) > 0) {
        char* value = (char*) memchr(buf, '=', linelen);
        if (value != NULL) {
            *value='\0'; // separate name and value strings at position of =
            value++;

            // skip leading ", if present
            if (*value == '"') value++;

            // cut trailing ", if present
            char* pos = (char*) memchr(value, '"', strlen(value));
            if (pos != NULL) *pos = '\0';

            // skip trailing newline, if present (and not already cut)
            pos = (char*)memchr(value, '\n', strlen(value));
            if (pos != NULL) *pos = '\0';

            setenv(buf, value, 0); // keep existing values to allow overriding /dev/bgpers for testing
            //CL_DEBUG_NOSPC("read \"%s\"=\"%s\" from /dev/bgpers, in environment = \"%s\".\n", buf, value, getenv(buf));
        }
    }

    return 0;
}


