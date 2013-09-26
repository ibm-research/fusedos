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

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{

    pid_t pid = getpid();
    char* p;
    int i;

    for(i=0; i < argc; i++) {
        printf("%d arg: %s\n", pid, argv[i]);
    }

    while ((p = *envp++)) {
        printf("%d env: %s\n", pid, p);
    }

    return 0;
}
