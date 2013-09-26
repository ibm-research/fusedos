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


#ifndef _SHAREDMEM_H_ // Prevent multiple inclusion.
#define _SHAREDMEM_H_

// Includes
#include "shmFS.h"

#define DEVSHM_PREFIX "/dev/shm/"
#define DEVSHM_ROOT   "/dev/shm"


//  sharedmemFS class - shared memory supported functions are defined here
//                implementation is in sharedmemFS.cc
// Please keep functions in alphabetical order for ease of maintenance


class sharedmemFS : public shmFS
{
public:

   //! \brief  Default constructor.

   sharedmemFS(void) : shmFS() { };

   //! \brief  Check if pathname is match for this file system.
   //! \param  path Pathname to check.
   //! \return True if pathname is a match, otherwise false.

   bool isMatch(const char *path) { return strncmp(path, DEVSHM_PREFIX, strlen(DEVSHM_PREFIX)) == 0 ? true : false; }

   int init(void);
   virtual ShmMgr_t* getShmManager() { return &(NodeState.SharedMemory); };
   virtual uint64_t  getDeviceID()   { return 0x01021994; };
};

#endif
