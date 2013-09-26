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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

#ifndef _CNK_SHM_H_ // Prevent multiple inclusion
#define _CNK_SHM_H_


#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include "Kernel.h"

#define __USE_LARGEFILE64 1   // This define is needed to generate the stat64 structure within stat.h
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

typedef struct ShmMgr_t
{
    uint64_t PStart;        // Base physical address for shared memory allocation
    uint64_t VStart;        // Base virtual address for shared memory allocation
    size_t   Size;          // Size of the shared memory allocation
} ShmMgr_t;


typedef struct ShmMgrEntry_t
{
    ShmMgr_t* owner;            // Owning pool manager
    char      FileName[256];    // Name of file to associate as handle for the file descriptor
    int       Oflags;           // Open flags
    int       Mode;             // Permissions 
    int       pinned;           // Whether the virtual address has been pinned or not.
    int       locked;           // Boolean.  Set if file is locked
    int       needAtomic;       // Boolean.  Atomic region needed
    uint32_t  LinkCount;        // Number of links to this memory area
    uint64_t  lastAccess;       // Timebase of last access
    uint64_t  lastMod;          // Timebase of last modification
    uint64_t  AllocatedAddr;    // Address given for this entry
    size_t    AllocatedSize;    // Size of this memory area
    uid_t     UserID;           // Owner's user id
    gid_t     GroupID;          // Owner's group id
} ShmMgrEntry_t;

#endif // Add nothing below this line
