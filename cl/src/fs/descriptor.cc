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
#include "Kernel.h"
#include "virtFS.h"
#include <ramdisk/include/services/MessageHeader.h>
#include <unistd.h>

#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "../cl_debug.h"                                                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS
extern virtFS *virtFSPtr[];

/*
 * Define a bit array large enough for the maximum descriptors supported for CNK.
 * Take the maximum number of descriptors supported and divide by the number
 * of bits per long for the compiled architecture.  This will result in the number
 * of longs necessary for the bit set. 
 */
unsigned long FileAllocBits[ (CONFIG_MAX_PROCESSES * CNK_MAX_FDS / CNK_BITS_PER_LONG) ];

/* Define an array of ints for the maximum fds that CNK will support.  
 * The subscript will be the local allocated descriptor.  The value contained at the 
 * subscript will be the IO node descriptor if one was allocated.
 * NOTE: Currently the only local descriptors that would not contain an IO node descriptor
 *       would be the shared memory support that is handled totally in CNK. 
 */
CNK_Descriptor_Info_t FileDescriptors[ CONFIG_MAX_PROCESSES * CNK_MAX_FDS ];


// Calculate the word location of the bit
#define CNK_BITLOC(bitnum) ( (bitnum) / CNK_BITS_PER_LONG )
/*
 * Set up the bitmask to order the bits in each long as follows:
 * 0 1 2 3 4 5 ... 31  | 32 33 34 35 ... 63 | ...
 * The bit ordering is set up this way to allow a one's complement of the long being
 * examined for an open descriptor.  Using the cntlzw assembler instruction after
 * inversion will give us the next available FD.
 */
#define CNK_BITMASK(bitnum) ( 0x8000000000000000LL >> ((bitnum) - (CNK_BITLOC(bitnum) * CNK_BITS_PER_LONG) ) )
#define CNK_BITMASK_OFFSET(offset) ( 0xFFFFFFFFFFFFFFFFLL >> ((offset) - (CNK_BITLOC(offset) * CNK_BITS_PER_LONG) ) )

inline void File_SetBit( int bitnum, unsigned long *bitset)
{
    // Point to correct word in the bitset.
    unsigned long *bitptr = (unsigned long *)bitset + CNK_BITLOC(bitnum);

    *bitptr |= CNK_BITMASK(bitnum);
    return;
}

inline int File_IsBitSet( int bitnum, unsigned long *bitset)
{
   // Point to correct word in the bitset.
   unsigned long *bitptr = (unsigned long *)bitset + CNK_BITLOC(bitnum);

   return (( *bitptr & CNK_BITMASK(bitnum)) ? 1 : 0 );
}

inline void File_clear_bit( int bitnum, unsigned long *bitset)
{
    // Point to correct word in the bitset.
    unsigned long *bitptr = (unsigned long *)bitset + CNK_BITLOC(bitnum);

    *bitptr &= ~(CNK_BITMASK(bitnum));
    return;
}

/*
 * Function to find the next available descriptor by offset.  This function will byte swap the bit flags and
 * then take the complement.  This will be searched using count leading zeros to find the next
 * available descriptor that is not allocated after calculating the offset to start at.
 */
inline int File_find_next_zero_bit_offset( unsigned long *BitSet, int NumBits, int offset )
{
    int AvailBit = -1;
    int i = 0;
    uint64_t result = 0;
    uint64_t BitCompl;
    unsigned long *bitptr = (unsigned long *)BitSet;

    // Calculate the number of longs to be searched for available bit
    int NumLongs = ( NumBits / CNK_BITS_PER_LONG );

    // Bump the bitptr to the correct starting location based on offset
    bitptr += CNK_BITLOC( offset );

    // Adjust the fd bitmask to allow the search to start at the offset specified.
    BitCompl = ~( *bitptr | ~( CNK_BITMASK_OFFSET( offset ) ) );

    // Loop thru starting at the word the offset is in.
    for ( i = CNK_BITLOC( offset ); i < NumLongs; i++ )
    {
        result = cntlz64( BitCompl );

        // Available FD found in this set of bits or at end?
        if ( result < CNK_BITS_PER_LONG )
        {
            break;  // Get out, we found a descriptor
        }

        // Take the ones complement of the next long
        bitptr++;
        BitCompl = ~(*(bitptr));

    }

    /*
     * Did we find a valid FD in any of the bits in the bit set?
     * If we are out of the loop and the result is set to 32, this means
     * that all of the descriptors have been consumed and none are free.
     */
    if ( result < CNK_BITS_PER_LONG )
    {
        // Adjust the bit based on the base.  The incrementor from the loop is used
        // to calculate the correct bit number.
        AvailBit = result + ( (i) * CNK_BITS_PER_LONG );
    }

    return( AvailBit );

}  // End find_next_zero_bit_offset

/*
 * Function to find the next available descriptor.  This function just calls the find_next_zero_bit_offset
 * with a start at the first descriptor in the table.
 */
inline int File_find_next_zero_bit( unsigned long *BitSet, int NumBits )
{
    int AvailBit = -1;

    AvailBit = File_find_next_zero_bit_offset( BitSet, NumBits, 0 );

    return( AvailBit );

}

int File_ProcessSetup(void)
{
   AppProcess_t *process = GetMyProcess();
   TRACE( TRACE_Jobctl, ("(I) File_ProcessSetup: running setup for AppProcess=%p %ld\n", process, CNK_BITS_PER_LONG) );

   // Initialize the descriptor table.
   CNK_Descriptors_t *pFD = &(process->App_Descriptors);
   pFD->maxfds          = process->HWThread_Count * CNK_MAX_FDS;
   pFD->cnk_fd_bits     = &FileAllocBits[process->ProcessLeader_ProcessorID * CNK_MAX_FDS / CNK_BITS_PER_LONG];
   pFD->cnk_local_fd    = &FileDescriptors[process->ProcessLeader_ProcessorID * CNK_MAX_FDS];
  
   CL_DEBUG("%p / %p, %p / %p, ProcessLeader_ProcessorID %d\n",
		pFD->cnk_fd_bits, FileAllocBits,
		pFD->cnk_local_fd, FileDescriptors,
		process->ProcessLeader_ProcessorID);
   int NumLongs =  pFD->maxfds / CNK_BITS_PER_LONG;

   // Initialize bit set so all descriptors are marked available.
   for ( int fd = 0; fd < NumLongs; ++fd )
   {
       pFD->cnk_fd_bits[ fd ] = 0;
   }

   // Set all descriptors to error file system.
   for ( int fd = 0; fd < pFD->maxfds; ++fd )
   {
       File_SetFD(fd, -1, FD_ERROR);
   }

   // Set the current working directory.
   int localFD = File_GetFD(CWD_FILENO);
   if (localFD != CWD_FILENO) {
      printf("(E) allocating cwd returned descriptor %d instead of descriptor %d\n", localFD, CWD_FILENO);
   }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   if (Personality_CiosEnabled()) {
      File_SetFD(localFD, AT_FDCWD, FD_FILE);
   }
   else {
      if (Personality_IsMambo()) {
         File_SetFD(localFD, AT_FDCWD, FD_FILE);
      }
      else {
         File_SetFD(localFD, AT_FDCWD, FD_MEMFILE);
      }
   }
#else                                                                                                                  // FUSEDOS
   File_SetFD(localFD, AT_FDCWD, FD_FILE);                                                                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   virtFS *fsptr = File_GetFSPtr(CWD_FILENO);
   if (fsptr != virtFSPtr[FD_ERROR] && !IsAppAgent()) {
      uint64_t rc = fsptr->chdir(process->CurrentDir);
      if (CNK_RC_IS_FAILURE(rc)) 
      {
         App_SetLoadState(AppState_LoadFailed, bgcios::WorkingDirError, CNK_RC_ERRNO(rc));
         return -1;
      }
   }

   // Allocate descriptors for stdin, stdout, and stderr.

#if defined __FUSEDOS__                                                                                               // FUSEDOS
   localFD = File_AllocateFD(STDIN_FILENO, FD_FILE);
   if (localFD != STDIN_FILENO) {
      printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
   }
   localFD = File_AllocateFD(STDOUT_FILENO, FD_FILE);
   if (localFD != STDOUT_FILENO) {
      printf("(E) allocating stdout returned descriptor %d instead of descriptor %d\n", localFD, STDOUT_FILENO);
   }
   localFD = File_AllocateFD(STDERR_FILENO, FD_FILE);
   if (localFD != STDERR_FILENO) {
      printf("(E) allocating stderr returned descriptor %d instead of descriptor %d\n", localFD, STDERR_FILENO);
   }
#else                                                                                                                  // FUSEDOS
   if (Personality_IsSimulation()) {
      if (Personality_IsMambo()) {
         if (Personality_CiosEnabled()) {
            localFD = File_AllocateFD(STDIN_FILENO, FD_STDIN);
            if (localFD != STDIN_FILENO) {
               printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
            }
         }
         else {
            // Use a special file for stdin when running in mambo without CIOS. Use default filename that must be in the current directory.
            char defaultPath[]= "mambo.stdin";
            uint64_t rc = virtFSPtr[FD_FILE]->open(defaultPath, O_RDONLY, 0);

            if (CNK_RC_IS_SUCCESS(rc)) {
               localFD = (int) CNK_RC_VALUE(rc);
               if (localFD != STDIN_FILENO) {
                  printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
               }
            }

            // Special stdin file was not available so stdin is not supported.
            else {
               localFD = File_AllocateFD(STDIN_FILENO, FD_ERROR);
               if (localFD != STDIN_FILENO) {
                  printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
               }
            }
         }
      }
      else {
         localFD = File_AllocateFD(STDIN_FILENO, FD_ERROR);
         if (localFD != STDIN_FILENO) {
            printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
         }
      }
   }
   else {
      if (!IsAppAgent()) {
         localFD = File_AllocateFD(STDIN_FILENO, FD_STDIN);
         if (localFD != STDIN_FILENO) {
            printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
         }
      }
      else {
         localFD = File_AllocateFD(STDIN_FILENO, FD_ERROR);
         if (localFD != STDIN_FILENO) {
            printf("(E) allocating stdin returned descriptor %d instead of descriptor %d\n", localFD, STDIN_FILENO);
         }
      }
   }

   if (!IsAppAgent()) {
      localFD = File_AllocateFD(STDOUT_FILENO, FD_STDOUT);
      if (localFD != STDOUT_FILENO) {
         printf("(E) allocating stdout returned descriptor %d instead of descriptor %d\n", localFD, STDOUT_FILENO);
      }

      localFD = File_AllocateFD(STDERR_FILENO, FD_STDERR);
      if (localFD != STDERR_FILENO) {
         printf("(E) allocating stderr returned descriptor %d instead of descriptor %d\n", localFD, STDERR_FILENO);
      }
   }
   else {
      localFD = File_AllocateFD(STDOUT_FILENO, FD_CONSOLE);
      if (localFD != STDOUT_FILENO) {
         printf("(E) allocating stdout returned descriptor %d instead of descriptor %d\n", localFD, STDOUT_FILENO);
      }

      localFD = File_AllocateFD(STDERR_FILENO, FD_CONSOLE);
      if (localFD != STDERR_FILENO) {
         printf("(E) allocating stderr returned descriptor %d instead of descriptor %d\n", localFD, STDERR_FILENO);
      }

   }
#endif                                                                                                                 // FUSEDOS

   return 0;
}

void File_ProcessCleanup(void)
{
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);
   
   // Push the close() method on any open descriptors.
   for ( int fd = 0; fd < pFD->maxfds; ++fd )
   {
      if (File_IsFDAllocated(fd))
      {
         (void) File_GetFSPtr(fd)->close(fd);
         File_SetFD(fd, -1, FD_ERROR);
      }
   }

   return;
}

int File_IsFDAllocated( int fd )
{
   int rc = 0;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      rc = File_IsBitSet( fd, pFD->cnk_fd_bits );
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   }

   return( rc );
}

int File_AllocateFD( int remote_fd, int type )
{
   int fd;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   fd = File_find_next_zero_bit( pFD->cnk_fd_bits, pFD->maxfds );

   // Any local descriptors available?
   if ( fd != -1 )
   {
       // Set the IO node fd accounting for the bit set number from 1
       pFD->cnk_local_fd[ fd ].Remote_FD = remote_fd;
       pFD->cnk_local_fd[ fd ].Type = type;
       pFD->cnk_local_fd[ fd ].FileSysPtr = virtFSPtr[type];
       File_SetBit( fd, pFD->cnk_fd_bits);
   }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   return( fd );
}

int File_GetFD( int offset )
{
   int fd;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   if ( offset > pFD->maxfds )
   {
      return( -1 );
   }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
   pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                    // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   fd = File_find_next_zero_bit_offset( pFD->cnk_fd_bits, pFD->maxfds, offset );

   // Any local descriptors available?
   if ( fd != -1 )
   {
      // Allocate the fd accounting for the bit set number from 1
      File_SetBit(fd, pFD->cnk_fd_bits);
   }

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
   Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
    pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   return( fd );
}

int File_SetFD(int fd, int remote_fd, int type)
{
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      pFD->cnk_local_fd[ fd ].Remote_FD = remote_fd;
      pFD->cnk_local_fd[ fd ].Type = type;
      pFD->cnk_local_fd[ fd ].FileSysPtr = virtFSPtr[type];

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   }
   else {
      return( -1 );
   }

   return( fd );
}

int File_FreeFD(int fd)
{
   int rc = -1;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      pFD->cnk_local_fd[ fd ].Remote_FD = -1;
      pFD->cnk_local_fd[ fd ].Type = FD_ERROR;
      pFD->cnk_local_fd[ fd ].FileSysPtr = virtFSPtr[FD_ERROR];
      
      // Bits start at number 1 so need to increment to get correct bit in bit set.
      File_clear_bit( fd, pFD->cnk_fd_bits );
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

      rc = 0;
   }

   return( rc );
}

int File_GetFDType(int fd)
{
   int type = -1;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      type = pFD->cnk_local_fd[ fd ].Type;
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   }  // End valid descriptor passed in

   return( type );
}

int File_GetRemoteFD(int fd)
{
   int remote_fd = -1;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);

   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      remote_fd = pFD->cnk_local_fd[ fd ].Remote_FD;
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

   }  // End valid descriptor passed in

   return( remote_fd );
}

virtFS* File_GetFSPtr( int fd )
{
    virtFS *fsPtr = NULL;
    AppProcess_t *app = GetMyProcess();
    CNK_Descriptors_t *pFD = &(app->App_Descriptors);

    // Is this a valid descriptor passed in?
    if ( (fd >= 0) && (fd < pFD->maxfds) )
    {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_lock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS

        fsPtr = (virtFS* ) (pFD->cnk_local_fd[ fd ].FileSysPtr);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
        Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
        pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                             // FUSEDOS
#endif                                                                                                                 // FUSEDOS

    }  // End valid descriptor passed in

    // return error file system pointer if no pointer in descriptor (invalid decsriptor)

    fsPtr = (NULL == fsPtr) ? virtFSPtr[FD_ERROR]:fsPtr;

    return(fsPtr);
}

long File_GetCurrentOffset( int fd )
{
   long curOffset  = -1;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);
   
   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      curOffset = pFD->cnk_local_fd[ fd ].CurrOffset;
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
   }  // End valid descriptor passed in
   
   return( curOffset );
}

int File_SetCurrentOffset( int fd, long curOffset )
{
   int rc  = -1;
   AppProcess_t *app = GetMyProcess();
   CNK_Descriptors_t *pFD = &(app->App_Descriptors);
   
   // Is this a valid descriptor passed in?
   if ( (fd >= 0) && (fd < pFD->maxfds) )
   {
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Lock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_lock(&app->DescriptorTable_mutex);                                                                 // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      pFD->cnk_local_fd[ fd ].CurrOffset = curOffset;
      
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
      Kernel_Unlock(&app->DescriptorTableLock);
#else                                                                                                                  // FUSEDOS
      pthread_mutex_unlock(&app->DescriptorTable_mutex);                                                               // FUSEDOS
#endif                                                                                                                 // FUSEDOS
      
      rc = 0;  
      
   }  // End valid descriptor passed in
   
   return( rc );
}

int File_GetCurrentDirFD( void )
{
    AppProcess_t *app = GetMyProcess();
    CNK_Descriptors_t *pFD = &(app->App_Descriptors);
    return File_GetRemoteFD(CWD_FILENO);
}

void *File_GetCurrentFSPtr( void )
{
    AppProcess_t *app = GetMyProcess();
    CNK_Descriptors_t *pFD = &(app->App_Descriptors);
    return File_GetFSPtr(CWD_FILENO);
}

int printstrc( const char *fmt, ... )
{
   va_list args;
   char buffer[512];

   va_start( args, fmt );
   int len = vsnprintf(buffer, sizeof(buffer), fmt, args );
   va_end(args);

#if defined __FUSEDOS__                                                                                               // FUSEDOS
   ::write(STDERR_FILENO, buffer, len);
#else                                                                                                                  // FUSEDOS
   File_GetFSPtr(STDERR_FILENO)->write(STDERR_FILENO, buffer, len);
#endif                                                                                                                 // FUSEDOS
   return len;
}
