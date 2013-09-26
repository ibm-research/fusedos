/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef __ROQ_FW_TRACE_H__
#define __ROQ_FW_TRACE_H__
/*
 * Control of timestamp trace in the RoQ firmware
 * 20120308 tjcw
 */
#include <stdint.h>
#include <roq_fw.h>
#include <string.h>

enum {
  k_pirbits = 10 ,
  k_pirsize = 1 << k_pirbits
};
enum {
  k_pir=286 // register number for processor identification register
};
typedef struct
{
  char *currentAddress ;
  char *limitAddress ;
} addressTable_t ;

extern addressTable_t addressTable[k_pirsize] ;

typedef struct
{
  int mLockWordName ; // Lock, so that both threads can use trace names coherently
  int mLockWordData ; // Lock, so that both threads can allocate new data pages coherently
  int mGeneration ; // Generation number, so tracepoint names can be emitted again on each new trace run
  int mOn ; // Whether tracing is currently on
  int mHiTraceStreamId ; // for handing out trace stream IDs
  char *mNextTraceId ; // Where to put the next tracepoint name
  char *mLimitTraceId ; // One past the last sequential place for putting tracepoint names
//  struct RoQ_DevicePrivatePerQPState_t* mTraceQP ; // The QP that the 'modify' was issued on to start tracing
//  int id_lkey ; // Key of the memory region where we are putting tracepoint names
  int data_lkey ; // Key of the memory region where we are putting trace stamps
//  struct RoQ_MR_t* id_mr ; // Memory region where we are putting tracepoint names
  struct roq_dm_mr *data_mr ; // Memory region where we are putting trace stamps
  int data_page ;
} PkTraceState ;
extern PkTraceState TraceState ;
/**
 * atomic_add_unless - add unless the number is a given value
 * @v: pointer of type int
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
 * Returns non-zero if @v was not @u, and zero otherwise.
 */
static __inline__ int atomic_add_unless(int *v, int a, int u)
{
  int t;

  __asm__ __volatile__ (
"   sync\n"
"1: lwarx  %[t],0,%[v]   # atomic_add_unless\n"
"   cmpw   0,%[t],%[u] \n"
"   beq-   2f \n"
"   add    %[t],%[a],%[t] \n"
"   stwcx. %[t],0,%[v] \n"
"   bne-   1b \n"
"   isync \n"
"   subf   %[t],%[a],%[t] \n"
"2:"
  : [t] "=&r" (t)
  : [v] "r" (v), [a] "r" (a), [u] "r" (u)
  : "cc", "memory");

  return t != u;
}

static __inline__ int atomic_inc_return_nobarrier(int *v)
{
  int t;

  __asm__ __volatile__(
"1: lwarx %[t],0,%[v]   # atomic_inc_return_nobarrier\n"
"   addic %[t],%[t],1\n"
"   stwcx.  %[t],0,%[v] \n"
"   bne-  1b"
  : [t] "=&r" (t)
  : [v] "r" (v)
  : "cc", "xer", "memory");

  return t;
}

enum {
  k_Locking = 1
};
static inline void LockLock(int *LockWord)
  {
    if(k_Locking)
      {
        while (0 == atomic_add_unless(LockWord, 1,1)) { } ;
      }
  }
static inline void LockUnlock(int *LockWord)
  {
    if(k_Locking)
      {
        asm volatile ( "sync" ) ;
        *LockWord = 0 ;
      }
  }
/*
 * Turn trace on. Locate the places in memory to store IDs and stamps,
 * and find the limits of sequential-ascending access
 */
static void TraceOn( int data_lkey)
  {
    TraceState.mGeneration += 1 ;
    TraceState.mHiTraceStreamId = 0 ;
//    TraceState.mTraceQP = PerQP ;
//    TraceState.id_lkey = id_lkey ;
    TraceState.data_lkey = data_lkey ;
//    TraceState.id_mr = RoQ_Get_MR_Ptr_From_RDMA_Key(id_lkey) ;
    TraceState.data_mr = RoQ_Get_MR_Ptr_From_RDMA_Key(data_lkey) ;
    {
        int cidtid ;
        TraceState.data_page = 0 ;
        for( cidtid=0;cidtid < k_pirsize;cidtid += 1)
          {
            addressTable[cidtid].currentAddress =  NULL ;
            addressTable[cidtid].limitAddress = NULL ;
          }
    }
    {
      uint64_t id_linux_virtual_address = TraceState.data_mr->mBaseVirtualAddress ;
      uint64_t id_RoQ_DmaAddr  = RoQ_HostVa2DmaAddr(data_lkey, id_linux_virtual_address, 2 * sizeof(uint64_t),__LINE__) ;
      uint64_t id_RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr( id_RoQ_DmaAddr );
      TraceState.mNextTraceId = (char *) id_RoQ_VirtAddr ;

      uint64_t id_next_page = ((id_RoQ_VirtAddr/TraceState.data_mr->pagesize)+1)*TraceState.data_mr->pagesize ;
      uint64_t id_end_of_segment = id_RoQ_VirtAddr+TraceState.data_mr->mSize ;

      TraceState.mLimitTraceId = (char *)((id_next_page < id_end_of_segment) ? id_next_page : id_end_of_segment) ;
    }


    TraceState.mOn = 1 ;
  }
static void TraceOff(void)
  {
    TraceState.mOn = 0 ;
  }
typedef struct
  {
  int mGeneration ;
  unsigned short mTraceStreamId ;
  } PkTraceStream;

// fetch from timebase register
static inline uint64_t get_tb(void)
  {
    uint64_t timebase ;
    asm volatile("mftb %0" : "=r" (timebase)) ;
    return timebase ;
  }
// Fetch from processor identification register
static inline uint64_t get_pir(void)
  {
    uint64_t pir ;
    asm volatile("mfspr %0,286" : "=r" (pir)) ;
    return pir ;
  }

enum {
  k_fixingup = 1
};
/*
 * When the trace stamps get to their sequential limit, this will find and set up the next
 * page if there is one (and will turn trace off if there is no next page) . Note that there is
 * no handling for trace names overflowing the first page of that segment; adviseable to make
 * sure there is enough space in the first page to handle all trace names in a run.
 */
static void AllocateNewPage(int cidtid)
  {

    uint64_t data_base_linux_virtual_address = TraceState.data_mr->mBaseVirtualAddress ;
    uint64_t data_limit_linux_virtual_address = data_base_linux_virtual_address + TraceState.data_mr->mSize ;
    uint64_t data_base_linux_virtual_page = data_base_linux_virtual_address/TraceState.data_mr->pagesize ;
//    LockLock(&TraceState.mLockWordData) ; // todo: looks like this could be done with atomic increment
//    uint64_t next_data_page = TraceState.data_page ;
//    TraceState.data_page = next_data_page + 1 ;
//    LockUnlock(&TraceState.mLockWordData) ;
    uint64_t next_data_page=atomic_inc_return_nobarrier(&TraceState.data_page) ;

    uint64_t data_next_linux_virtual_address = data_base_linux_virtual_address + next_data_page*TraceState.data_mr->pagesize ;
    if( data_next_linux_virtual_address < data_limit_linux_virtual_address)
      {
        uint64_t data_RoQ_DmaAddr = RoQ_HostVa2DmaAddr(TraceState.data_lkey, data_next_linux_virtual_address, 2 * sizeof(uint64_t),__LINE__) ;
        uint64_t data_following_linux_virtual_address = data_next_linux_virtual_address + TraceState.data_mr->pagesize ;
        uint64_t data_RoQ_VirtAddr = RoQ_CnkDmaAddr_To_CnkVirtAddr( data_RoQ_DmaAddr );
        addressTable[cidtid].currentAddress = (char *)data_RoQ_VirtAddr ;
        if( data_following_linux_virtual_address > data_limit_linux_virtual_address)
          {
            // memory region ends in this page
            uint64_t end_offset = data_limit_linux_virtual_address % TraceState.data_mr->pagesize ;
            addressTable[cidtid].limitAddress=(char *)data_RoQ_VirtAddr+end_offset ;
          }
        else
          {
            // whole page is in the memory region
            addressTable[cidtid].limitAddress=(char *)data_RoQ_VirtAddr+TraceState.data_mr->pagesize ;
          }
      }
    else
      {
        TraceOff() ;
      }

  }
static void FixUpLimitTraceData(uint64_t traceStamp, int cidtid)
  {
    AllocateNewPage(cidtid) ;
    char *currentAddress=addressTable[cidtid].currentAddress ;
    char *limitAddress=addressTable[cidtid].limitAddress ;
    if(currentAddress+sizeof(uint64_t) < limitAddress)
      {
        *(uint64_t *)currentAddress = traceStamp ;
        addressTable[cidtid].currentAddress=currentAddress+sizeof(uint64_t) ;
      }
  }
static void FixUpLimitTraceData1(uint64_t traceStamp, int ContextNumber, int cidtid)
  {
    AllocateNewPage(cidtid) ;
    char *currentAddress=addressTable[cidtid].currentAddress ;
    char *limitAddress=addressTable[cidtid].limitAddress ;
    if(currentAddress+sizeof(uint64_t)+sizeof(int) < limitAddress)
      {
        *(uint64_t *)currentAddress = traceStamp ;
        currentAddress+=sizeof(uint64_t) ;
        *(int *)currentAddress=ContextNumber ;
        addressTable[cidtid].currentAddress=currentAddress+sizeof(int) ;
      }
  }
static void FixUpLimitTraceData2(uint64_t traceStamp, int ContextNumber, int ContextNumber2, int cidtid)
  {
    AllocateNewPage(cidtid) ;
    char *currentAddress=addressTable[cidtid].currentAddress ;
    char *limitAddress=addressTable[cidtid].limitAddress ;
    if(currentAddress+sizeof(uint64_t)+2*sizeof(int) < limitAddress)
      {
        *(uint64_t *)currentAddress = traceStamp ;
        currentAddress+=sizeof(uint64_t) ;
        *(int *)currentAddress=ContextNumber ;
        currentAddress+=sizeof(int) ;
        *(int *)currentAddress=ContextNumber2 ;
        addressTable[cidtid].currentAddress=currentAddress+sizeof(int) ;
      }
  }
static void FixUpLimitTraceData3(uint64_t traceStamp, int ContextNumber, int ContextNumber2, int ContextNumber3, int cidtid)
  {

    AllocateNewPage(cidtid) ;
    char *currentAddress=addressTable[cidtid].currentAddress ;
    char *limitAddress=addressTable[cidtid].limitAddress ;
    if(currentAddress+sizeof(uint64_t)+3*sizeof(int) < limitAddress)
      {
        *(uint64_t *)currentAddress = traceStamp ;
        currentAddress+=sizeof(uint64_t) ;
        *(int *)currentAddress=ContextNumber ;
        currentAddress+=sizeof(int) ;
        *(int *)currentAddress=ContextNumber2 ;
        currentAddress+=sizeof(int) ;
        *(int *)currentAddress=ContextNumber3 ;
        addressTable[cidtid].currentAddress=currentAddress+sizeof(int) ;
      }

  }

// Taken from 'Pk', find the last N components of a file name
#define MAX_TRACE_NAME 256
#ifndef PKFXLOG_PATHNAME_DIRECTORIES_LIMIT
#define PKFXLOG_PATHNAME_DIRECTORIES_LIMIT ( 3 )
#endif
static inline
const char *
FxLogger_GetStartPointInFilePathName( const char * aFN, int aPathParts /* =PKFXLOG_PATHNAME_DIRECTORIES_LIMIT */ )
  {
  int starti = 0;

  // Find the end of the pathname
  int endi = 0;
  // while( aFN[ endi ] != '\0' )
#define PKFXLOG_MAX_PATH 240

  while( endi < PKFXLOG_MAX_PATH && aFN[ endi ] != '\0' )
    {
    endi++ ;
    }

//  if( endi == PKFXLOG_MAX_PATH )
//    FxLogger_Abort( "Unreasonably long file pathname" );

    int slashcount = 0;
    int slashstart ;
    for(  slashstart = endi; slashstart != 0; slashstart-- )
      {
      if( aFN[ slashstart ] ==  '/' )
         slashcount++;
      if( slashcount == aPathParts )
        {
        starti = slashstart+1;
        break;
        }
      }

    //  if( starti > aPathParts )
    //     starti = endi - aPathParts;

  return( & aFN[ starti ] );
  }
// pkTraceStreamConnect called once per trace point to register that point
// it gets a trace stream id from the server.  It should involve a lock,
// but for usage with the RoQ we are running lockless.
static int FxLoggerNodeId = -1 ;
static int my_strlen(const char *s)
  {
    int rc=0 ;
    while(s[rc]) rc += 1 ;
    return rc ;
  }
static void my_strcpy(char *d, const char *s)
  {
    while(*s) *d++=*s++ ;
    *d = 0 ; // patch in a trailing null byte
  }
static void PkTraceStreamInit(PkTraceStream *TraceStream)
  {
    TraceStream->mGeneration = 0 ;
  }
static int
PkTraceStreamConnect( PkTraceStream *TraceStream,
    const char     *SourceFileName,
                      int       SourceLineNo,
                      const char     *TraceStreamContext,
                      int       TraceStreamContextOrdinal,
                      const char     *TraceStreamName )
  {

  char ComposedTraceName[ MAX_TRACE_NAME ];
  char TraceName[ MAX_TRACE_NAME ];

  if( TraceStreamContextOrdinal == -1 )
    {
    sprintf( ComposedTraceName, "<context>%s</context><name>%s</name>", TraceStreamContext, TraceStreamName );
    }
  else
    {
    sprintf( ComposedTraceName, "<context>%s</context><ordinal>%d</ordinal><name>%s</name>",
             TraceStreamContext,
             TraceStreamContextOrdinal,
             TraceStreamName );
    }

  const char* ShortSourceFileName  = FxLogger_GetStartPointInFilePathName( SourceFileName,PKFXLOG_PATHNAME_DIRECTORIES_LIMIT  );

  sprintf( TraceName, "<tracename><node>%04d</node><composedname>%s</composedname><fn>%s</fn><line>%04u</line></tracename>",
           FxLoggerNodeId == -1 ? TraceStreamContextOrdinal : FxLoggerNodeId,
//           0,
           ComposedTraceName,
           ShortSourceFileName,
           SourceLineNo);

  int nloop = 0;
  while(TraceName[nloop] != '\0')
    {
//    if( TraceName[nloop] == '/' )
//      TraceName[nloop] = '|';
//    else
      if( TraceName[nloop] == ' ' )
      TraceName[nloop] = '_';

    nloop++;
    }

  int namelength=my_strlen(TraceName) ;

  LockLock(&TraceState.mLockWordName) ;
  if (TraceState.mNextTraceId + namelength + 2 <  TraceState.mLimitTraceId)
    {
      my_strcpy(TraceState.mNextTraceId,TraceName) ;
      TraceState.mNextTraceId += namelength+1 ;
    }
  int TraceStreamId = TraceState.mHiTraceStreamId;
  TraceState.mHiTraceStreamId = TraceStreamId+1 ;
  LockUnlock(&TraceState.mLockWordName) ;

  TraceStream->mGeneration = TraceState.mGeneration ;

  return TraceStreamId;
  }
// Inline injected at each trace point. Simple case is handled inline, anything more complex
// results in a call to PkTraceStreamConnect or FixUpLimitTraceData to fix things up.
// Just a trace ID and a timestamp
static inline void PkTraceStreamSource(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName)
  {
    if(TraceState.mOn)
      {
        if(TraceStream->mGeneration !=  TraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        int cidtid=get_pir() & ( k_pirsize-1);
        uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) ;
        char *currentAddress=addressTable[cidtid].currentAddress ;
        char *limitAddress=addressTable[cidtid].limitAddress ;
        if(currentAddress+sizeof(uint64_t) < limitAddress)
          {
            *(uint64_t *)currentAddress = traceStamp ;
            addressTable[cidtid].currentAddress=currentAddress+sizeof(uint64_t) ;
          }
        else
          {
            FixUpLimitTraceData(traceStamp,cidtid) ;
          }
      }

  }
// A trace ID, timestamp, and one integer
static inline void PkTraceStreamSource1(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber)
  {
    if(TraceState.mOn)
      {
        if(TraceStream->mGeneration !=  TraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        int cidtid=get_pir() & ( k_pirsize-1);
        char *currentAddress=addressTable[cidtid].currentAddress ;
        char *limitAddress=addressTable[cidtid].limitAddress ;
        uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0x4000000000000000ULL;
        if( currentAddress +sizeof(uint64_t) + sizeof(int) <= limitAddress)
          {
            *(uint64_t *)currentAddress = traceStamp ;
            currentAddress += sizeof(uint64_t) ;
            *(int *)currentAddress = ContextNumber ;
            addressTable[cidtid].currentAddress=currentAddress+sizeof(int) ;
          }
        else
          {
            FixUpLimitTraceData1(traceStamp,ContextNumber,cidtid) ;
          }
      }

  }
// A trace ID, timestamp, and two integers
static inline void PkTraceStreamSource2(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber, int ContextNumber2)
  {
    if(TraceState.mOn)
      {
        if(TraceStream->mGeneration !=  TraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        int cidtid=get_pir() & ( k_pirsize-1);
        char *currentAddress=addressTable[cidtid].currentAddress ;
        char *limitAddress=addressTable[cidtid].limitAddress ;
        uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0x8000000000000000ULL;
        if( currentAddress +sizeof(uint64_t) + 2*sizeof(int) <= limitAddress)
          {
            *(uint64_t *)currentAddress = traceStamp ;
            currentAddress += sizeof(uint64_t) ;
            *(int *)currentAddress = ContextNumber ;
            currentAddress += sizeof(int) ;
            *(int *)currentAddress = ContextNumber2 ;
            addressTable[cidtid].currentAddress =  currentAddress + sizeof(int) ;
          }
        else
          {
            FixUpLimitTraceData2(traceStamp,ContextNumber,ContextNumber2,cidtid) ;
          }
      }

  }
// A trace ID, timestamp, and three integers
static inline void PkTraceStreamSource3(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber, int ContextNumber2, int ContextNumber3)
  {
    if(TraceState.mOn)
      {
        if(TraceStream->mGeneration !=  TraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        int cidtid=get_pir() & ( k_pirsize-1);
        char *currentAddress=addressTable[cidtid].currentAddress ;
        char *limitAddress=addressTable[cidtid].limitAddress ;
        uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0xc000000000000000ULL;
        if( currentAddress +sizeof(uint64_t) + 3*sizeof(int) <= limitAddress)
          {
            *(uint64_t *)currentAddress = traceStamp ;
            currentAddress += sizeof(uint64_t) ;
            *(int *)currentAddress = ContextNumber ;
            currentAddress += sizeof(int) ;
            *(int *)currentAddress = ContextNumber2 ;
            currentAddress += sizeof(int) ;
            *(int *)currentAddress = ContextNumber3 ;
            addressTable[cidtid].currentAddress =  currentAddress + sizeof(int) ;
          }
        else
          {
            FixUpLimitTraceData3(traceStamp,ContextNumber,ContextNumber2,ContextNumber3,cidtid) ;
          }
      }

  }
#if defined(PKTRACE_ON)
#define HitOE(EventName,ContextName,Ordinal) PkTraceStreamSource(&EventName, __LINE__,__FILE__, ContextName, Ordinal, #EventName)
#define HitOE1(EventName,ContextName,Ordinal,ContextNumber) PkTraceStreamSource1(&EventName, __LINE__,__FILE__, ContextName, Ordinal, #EventName, ContextNumber)
#define HitOE2(EventName,ContextName,Ordinal,ContextNumber,ContextNumber2) PkTraceStreamSource2(&EventName, __LINE__,__FILE__, ContextName, Ordinal, #EventName, ContextNumber, ContextNumber2)
#define HitOE3(EventName,ContextName,Ordinal,ContextNumber,ContextNumber2,ContextNumber3) PkTraceStreamSource3(&EventName, __LINE__,__FILE__, ContextName, Ordinal, #EventName, ContextNumber, ContextNumber2, ContextNumber3)
#define TraceInit(EventName) EventName.mGeneration=0
#define TraceClient PkTraceStream
#else
#define HitOE(EventName,ContextName,Ordinal)
#define HitOE1(EventName,ContextName,Ordinal,ContextNumber)
#define HitOE2(EventName,ContextName,Ordinal,ContextNumber,ContextNumber2)
#define HitOE3(EventName,ContextName,Ordinal,ContextNumber,ContextNumber2,ContextNumber3)
#define TraceInit(EventName)
#define TraceClient PkTraceStream
#endif

#endif
