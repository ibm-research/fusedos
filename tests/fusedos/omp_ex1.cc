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

#include <cmath>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <spi/include/kernel/memory.h>

int fubar;

int main()
{
    const int size = 4;
    double sinTable[size];

    int rc = Kernel_L2AtomicsAllocate(&fubar, sizeof(fubar));
    if (rc) {
        printf("Kernel_L2AtomicsAllocate failed, rc %d\n", rc);
        exit(1);
    }

    printf("Starting OMP parallel computation...");

    #pragma omp parallel for
    for(int n=0; n<size; ++n)
        sinTable[n] = std::sin(2 * M_PI * n / size);

    // the table is now initialized
    printf("Completed OMP parallel computation. Exiting.\n");
}
