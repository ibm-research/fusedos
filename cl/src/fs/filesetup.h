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

#ifndef _CNK_FILESETUP_H_
#define _CNK_FILESETUP_H_

// To add a file system:
// 
// 1) Update file.h Descriptor types with a descriptor value for the new file system.
//    Also increment FD_TOTAL_FILESYS to include the new file system
// 2) Add the file system specific include to the includes below
// 3) Add a line to the typedef below to indicate the file system size
//     The typedef will indicate the size of the largest file sysytem. 
//     This value will be used to allocate storage.
// 4) Update the File_SimInit() routine in filesetup.cc to add the new file system
//    class pointer to the file system array.

// Includes
#include "virtFS.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "mamboFS.h"
#include "sastdioFS.h"
#include "stdioFS.h"
#endif                                                                                                                 // FUSEDOS
#include "sharedmemFS.h"
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "persistmemFS.h"
#include "memFS.h"
#include "sysioFS.h"
#include "localFS.h"
#include "procFS.h"
#include "rdmaFS.h"
#else                                                                                                                  // FUSEDOS
#include "fusedosFS.h"                                                                                                // FUSEDOS
#endif                                                                                                                 // FUSEDOS

// Define maximum storage need by a file system class.
// This is used in virtFS.cc to allocate storage for the file system classes.
// This is used in filesetup.cc to init the class pointers.

typedef union { 
  char  virtFS_store[sizeof(virtFS)];       // FD_ERROR
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  char  sysioFS_store[sizeof(sysioFS)];     // FD_FILE with CIOS
  char  sastdioFS_store[sizeof(sastdioFS)]; // FD_STDOUT, FD_STDERR
  char  stdioFS_store[sizeof(stdioFS)];     // FD_STDIN, FD_STDOUT, FD_STDERR
#endif                                                                                                                 // FUSEDOS
  char  sharedmemFS_store[sizeof(sharedmemFS)]; // FD_SHM
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  char  mamboFS_store[sizeof(mamboFS)];     // FD_FILE with mambo
  char  memFS_store[sizeof(memFS)];         // FD_MEMFILE
  char  persistFS_store[sizeof(persistmemFS)];         // FD_SHM
  char  localFS_store[sizeof(localFS)];     // FD_LOCAL
  char  procFS_store[sizeof(procFS)];       // FD_PROC_MEM
  char  rdmaFS_store[sizeof(rdmaFS)];       // FD_RDMA
#else                                                                                                                  // FUSEDOS
  char  fusedosFS_store[sizeof(fusedosFS)];                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
} ALIGN_QUADWORD StoreFS_t;

#ifdef __cplusplus
extern "C"
{
#endif

void File_InitFS(void);
void File_TermFS(void);

#ifdef __cplusplus
}
#endif

int File_JobSetup(void);
int File_JobCleanup(void);

#endif // _CNK_FILESETUP_H_

