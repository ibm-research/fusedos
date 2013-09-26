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
/* (C) Copyright IBM Corp.  2009, 2012                              */
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

// Includes
#include "Kernel.h"
#include "util.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "virtFS.h"
#include "filesetup.h"

struct _Unwind_Exception;

// Catch routine to keep linker happy in case a pure virtual call is ever done.
// Send error message since there is no C++ run time support in the kernel 
extern "C" void __cxa_pure_virtual()
{
     printf("Invalid virtual function call\n");
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern "C" void _Unwind_Resume(struct _Unwind_Exception *object)
{
    printf("Invalid call to _Unwind_Resume\n");
}
#endif                                                                                                                 // FUSEDOS

// pointers to file systems
virtFS *virtFSPtr[FD_TOTAL_FILESYS] = { NULL, };
