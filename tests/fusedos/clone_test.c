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

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/types.h>

double random_data[] = {
    0.170393657665, 0.900001483608, 0.160969316048, 0.109190756857, 0.963922270258,
    0.101689744168, 0.945141249902, 0.480732880031, 0.52529050934, 0.935754899929,
    0.749028715254, 0.46012272133, 0.664855293962, 0.815724343257, 0.859354835402,
    0.072490935579, 0.883205575492, 0.00805912706433, 0.600920297584, 0.930585792936,
    0.346881428773, 0.73263241148, 0.785737385713, 0.394982905594, 0.982423609787,
    0.205208130175, 0.860297951262, 0.0874005388674, 0.050899833827, 0.99763240622,
};

#define NUM_ENTRIES (sizeof(random_data)/sizeof(double))

/* some reasonable meaningful FP test code */
double avg()
{
    double avg = 0;
    int i;

    for(i=0; i < NUM_ENTRIES; i++)
        avg += random_data[i];

    return avg / NUM_ENTRIES;
}

int i;

int f()
{
    //double f = 0;
    printf("f: i %d\n", i);
    sleep(1);
    //while(1);
    //hile(1) f*= avg();
    _exit(0);
}

int main()
{
    i = 7;

    printf("main: i %d, avg=%g\n", i, avg());

    printf("random data @ %p to @ %p\n", random_data, &(random_data[NUM_ENTRIES]) );

    char child_stack[1024];
    pid_t ptid = 0, ctid = 0;
    char tls[1024];

    printf("main thread, prepared child stack %p to %p\n", child_stack, &child_stack[1024]);
    printf("ptid @ %p, ctid @ %p, child tls @ %p\n", &ptid, &ctid, tls);

    clone(f, &child_stack[1024], CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID|CLONE_DETACHED, NULL, &ptid, tls, &ctid);

    printf("main: ptid %d, ctid %d\n", ptid, ctid);
    sleep(2);
    //while(1);
    return 0;
}
