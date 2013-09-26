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
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
void f(void* x)
{
    int* _i = (int*)x;
    printf("f: *_i %d\n", *_i);
    sleep(1);
    pthread_exit(0);
}
int main()
{
    pthread_t t;
    int i;
    i = 7;
    printf("main: i %d\n", i);
    pthread_create(&t, NULL, (void*)(&f), &i);
    sleep(2);
    pthread_join(t, NULL);
    return(0);
}
