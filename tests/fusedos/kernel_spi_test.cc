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

#include <fcntl.h>
#if 0
#include <spi/include/kernel/location.h>
#endif
#define __INLINE__ static inline
#include <spi/include/kernel/process.h>
#include <spi/include/kernel/location.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void bittostr(uint64_t bitmap, char * str)
{

    for(int i=63; i>=0; i--) {
        str[i] = '0' + (char)(bitmap & 1);
        bitmap >>= 1;
    }
}

int main(int argc, char* argv[], char* envp[])
{
    //printf("Hello world, pid %d\n", getpid());

    uint32_t processCount = Kernel_ProcessCount();

    uint32_t processorCount = Kernel_ProcessorCount();

    printf("pid %d: processCount %d, processorCount %d\n", getpid(), processCount, processorCount);

    Personality_t personality;
    Kernel_GetPersonality( & personality, sizeof(personality) );

    //uint64_t threadMask = Kernel_ThreadMask();

    //char str[66];
    //memset(str, 0, sizeof(str));
    //bittostr(threadMask, str);
    //printf("|6  6   5   5   4   4   4   3   3   2   2   2   1   1   0   0   0|\n");
    //printf("|3210987654321098765432109876543210987654321098765432109876543210|\n");
    //printf(" %s\n", str);

	uint32_t tcoord = Kernel_MyTcoord();

	printf("pid %d: Tcoord %u\n", getpid(), tcoord);

    return(0);
}
