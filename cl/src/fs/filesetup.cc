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

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Kernel.h"
#include <new>
#include "virtFS.h"
#include "filesetup.h"

// space for file systems - do we need align attribute here?
StoreFS_t storeFS[FD_TOTAL_FILESYS];


void File_InitFS(void)
{
#define FSDEFINE(id, ref_id) virtFSPtr[id] = virtFSPtr[ref_id];
#define FSMAKE(id, type) if(virtFSPtr[id] == virtFSPtr[FD_ERROR]) { virtFSPtr[id] = (virtFS*)&storeFS[id]; new(virtFSPtr[id]) type; if(virtFSPtr[id]->init() == -1) { virtFSPtr[id] = (virtFS*)&storeFS[FD_ERROR]; } }
    
    // Initialize FD_ERROR to base virtual file system class.
    virtFSPtr[FD_ERROR] = (virtFS *)&storeFS[FD_ERROR];
    new(virtFSPtr[FD_ERROR]) virtFS;
    virtFSPtr[FD_ERROR]->init();

    // Initialize all other file systems to FD_ERROR file system.
    for(int x=1; x<FD_TOTAL_FILESYS; x++)
    {
        FSDEFINE(x, FD_ERROR);
    }

#if defined __FUSEDOS__                                                                                               // FUSEDOS
    FSMAKE(FD_FILE, fusedosFS);                                                                                       // FUSEDOS
    FSDEFINE(FD_SOCKET, FD_FILE);                                                                                      // FUSEDOS
    // The shared memory, ramdisk, and standalone standard I/O file systems are always available on the node.
    FSMAKE(FD_SHARED_MEM, sharedmemFS);
#else                                                                                                                  // FUSEDOS
    FSMAKE(FD_PERSIST_MEM, persistmemFS);
    FSMAKE(FD_LOCAL, localFS);
    FSMAKE(FD_MEMFILE, memFS);
    FSMAKE(FD_CONSOLE, sastdioFS);
    FSMAKE(FD_PROC_MEM, procFS);
    
    
    if (Personality_IsJTagConsole())
    {
        FSDEFINE(FD_STDOUT, FD_CONSOLE);
        FSDEFINE(FD_STDERR, FD_STDOUT);
    }        
    
    if (Personality_CiosEnabled())
    {
        FSMAKE(FD_FILE, sysioFS);
        FSDEFINE(FD_SOCKET, FD_FILE);
        FSMAKE(FD_STDIN, stdioFS);
        FSDEFINE(FD_STDOUT, FD_STDIN);
        FSDEFINE(FD_STDERR, FD_STDIN);
        FSMAKE(FD_RDMA, rdmaFS);
    }
    else 
    {
        if (Personality_IsMambo())
        {
            FSMAKE(FD_FILE, mamboFS);
        }
    }
#endif                                                                                                                 // FUSEDOS

#if CONFIG_PRINTFSSIZE
#define PRINTSIZE(name) printf("sizeof(%s) = %ld\n", #name, sizeof(name));
    PRINTSIZE(virtFS);
    PRINTSIZE(sysioFS);
    PRINTSIZE(sastdioFS);
    PRINTSIZE(stdioFS);
    PRINTSIZE(sharedmemFS);
    PRINTSIZE(mamboFS);
    PRINTSIZE(memFS);
    PRINTSIZE(persistmemFS);
    PRINTSIZE(localFS);
    PRINTSIZE(procFS);
    PRINTSIZE(rdmaFS);
#endif
    return;
}

void File_TermFS(void)
{
    // Allow each file system to terminate during node shutdown.
    for (int fs = 1; fs < FD_TOTAL_FILESYS; fs++)
    {
        if(virtFSPtr[fs] != NULL)
        {
            int err = virtFSPtr[fs]->term();
            if (err != 0)
            {
                printf("(E) File_TermFS(): file system %d term method failed, error %d\n", fs, err);
            }
        }
    }
    return;
}

int File_JobSetup(void)
{
   // Allow each file system to prepare for new job.
   int rc = 0;
   for (int fs = 1; fs < FD_TOTAL_FILESYS; fs++)
   {
       int err = virtFSPtr[fs]->setupJob(fs);
       if (err != 0)
       {
           rc = -1;
           printf("(E) File_JobSetup(): file system %d setupJob method failed, error %d\n", fs, err);
       }
   }

   return rc;
}

int File_JobCleanup(void)
{
   // Allow each file system to cleanup after job.
   int rc = 0;
   for (int fs = 1; fs < FD_TOTAL_FILESYS; fs++)
   {
       int err = virtFSPtr[fs]->cleanupJob(fs);
       if (err != 0)
       {
           rc = -1;
           printf("(E) File_JobCleanup(): file system %d cleanupJob method failed, error %d\n", fs, err);
       }
   }

   return rc;
}

/*
 * Function to return the file system pointer associated with a path name.
 */
virtFS*  File_GetFSPtrFromPath( const char* pathname )
{
    AppProcess_t *app = GetMyProcess();
    CNK_Descriptors_t *pFD = &(app->App_Descriptors);
    
    // For an absolute pathname, look for a match in the list of file systems.
    if (pathname[0] == '/') {
        for (int fs = 1; fs < FD_TOTAL_FILESYS; fs++)
        {
            if (virtFSPtr[fs]->isMatch(pathname)) return virtFSPtr[fs];
        }
    }
    
    // For a relative pathname, use the file system pointer for the current working directory.
    return File_GetFSPtr(CWD_FILENO);
}

virtFS* File_GetFSPtrFromType( int type )
{
    if (type < 0 || type >= FD_TOTAL_FILESYS)
    {
        return NULL;
    }
    return virtFSPtr[type];
}
