/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/* IBM Confidential                                                 */
/*                                                                  */
/* Licensed Machine Code Source Materials                           */
/*                                                                  */
/* Product(s):                                                      */
/*     Blue Gene/Q Licensed Machine Code                            */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011, 2012                        */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>


static const unsigned int CHUNK_SIZE = 32*1024*1024;

int main(int argc, char ** argv)
{

    int * chunks[50000];
    unsigned int allocated = 0;

    printf("Memory eater grabbing memory...\n");

    do {
        chunks[allocated] = (int *)malloc(CHUNK_SIZE);
		if(allocated % 10 == 9) printf(".");
    } while( allocated < 50000 && chunks[allocated++] != NULL );
    allocated--;

    printf("allocated %u chunks, that is %lu MB\n", allocated,
           allocated*(uint64_t)CHUNK_SIZE/(1024L*1024) );


    printf("now accessing the chunks...\n");

    for(unsigned int i=0; i < allocated; i++) {
        if(i%5 == 4) {
            printf("chunk at %p ", chunks[i]);
		    printf("\n");
		}
        fflush(stdout);

        for(unsigned int j=0; j < CHUNK_SIZE / sizeof(int); j+=16)
            chunks[i][j] = (i % 42 + 12) * (j % 17 + 24);
    }

    printf("finished.\n");

    return 0;
}
