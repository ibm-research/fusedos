/*
 * RoQ device driver for Linux
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _ROQ_LINUX_TRACE_H
#define _ROQ_LINUX_TRACE_H

#include "roq_debug.h"
#include "roq_linux_trace_base.h"

enum {
  k_pir = 286 // register number for processor identification register
};

enum {
  k_PageSize = 65536
};

#if defined(CAN_TURN_TRACE_ON)
static void TraceOnLinux(struct RoQ_DeviceMemory *RoQ, int lkey) __attribute__((unused));
static void TraceOnLinux(struct RoQ_DeviceMemory *RoQ, int lkey)
  {
#if defined(PKTRACE_ON)
    struct roq_dm_mr *dm_mr = &RoQ->mMR_Array[lkey >> (8+4)];
    dprint_tsk(DBG_TRC, KERN_INFO, "TraceOnLinux(%p,%i) dm_mr=%p\n",RoQ,lkey,dm_mr);
    dprint_tsk(DBG_TRC, KERN_INFO, " umem=%p mVersionNumber=%d pagesize=%d mHostPageArrayDmaAddr=0x%016llx mHostPageArrayPtr=0x%016llx\n",
        dm_mr->umem,dm_mr->mVersionNumber,dm_mr->pagesize,dm_mr->mHostPageArrayDmaAddr,dm_mr->mHostPageArrayPtr);
    dprint_tsk(DBG_TRC, KERN_INFO, "mBaseVirtualAddress=0x%016llx mSize=%lld mRights=0x%08x\n",
        dm_mr->mBaseVirtualAddress,dm_mr->mSize,dm_mr->mRights);
    {
	struct roq_mr  *dd_mr = (struct roq_mr *)dm_mr->dd_mr;
        struct ib_umem *umem  = dd_mr->umem;
        struct ib_umem_chunk  *chunk;

        list_for_each_entry(chunk, &umem->chunk_list, list) {
            dprint_tsk(DBG_TRC, KERN_INFO, "chunk=%p nents=%d\n",chunk,chunk->nents);
            break;
        }
        LinuxTraceState.nents = chunk->nents;
        LinuxTraceState.mTraceIdPlist = &chunk->page_list[0];
        dprint_tsk(DBG_TRC, KERN_INFO, " mTraceIdPlist=%p\n",LinuxTraceState.mTraceIdPlist);
        {
            int cidtid;
            LinuxTraceState.data_page = 0;
            for( cidtid=0;cidtid < k_pirsize;cidtid += 1)
              {
                addressTable[cidtid].kernelPage = NULL;
                addressTable[cidtid].currentOffset =  k_PageSize;
              }
        }
    }
    LinuxTraceState.mNextTraceId = 0;
    LinuxTraceState.mGeneration += 1;
    LinuxTraceState.mHiTraceStreamId = 128;
    spin_lock_init(&LinuxTraceState.mLockWordName);
    LinuxTraceState.mOn = 1;
#endif
  }
#endif

static void WriteBackPage(int cidtid)
  {
    char *kernelPage = addressTable[cidtid].kernelPage;
    if(kernelPage)
      {
        struct scatterlist *plist = LinuxTraceState.mTraceIdPlist + addressTable[cidtid].currentPage;
        struct page *mapPage=sg_page(plist);
        char *mapAddress = kmap(mapPage);
        memcpy(mapAddress,kernelPage,addressTable[cidtid].currentOffset);
        kunmap(mapPage);
      }
  }
static void TraceOffLinux(void)
  {
#if defined(PKTRACE_ON)
    int cidtid;
    dprint_tsk(DBG_TRC, KERN_INFO, "TraceOffLinux\n");
    LinuxTraceState.mOn = 0;
    for(cidtid=0; cidtid < k_pirsize; cidtid += 1)
      {
        WriteBackPage(cidtid);
        if(addressTable[cidtid].kernelPage)
          {
            kfree(addressTable[cidtid].kernelPage);
            addressTable[cidtid].kernelPage=NULL;
          }
      }
#endif
  }

typedef struct
  {
  int mGeneration;
  unsigned short mTraceStreamId;
  } PkTraceStream;

// Fetch from processor identification register
static inline uint64_t get_pir(void)
  {
    uint64_t pir;
    asm volatile("mfspr %0,286" : "=r" (pir));
    return pir;
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

    unsigned long flags;
    uint64_t next_data_page;
    WriteBackPage(cidtid);
    spin_lock_irqsave(&LinuxTraceState.mLockWordData, flags);
    next_data_page=LinuxTraceState.data_page + 1;
    LinuxTraceState.data_page = next_data_page;
    spin_unlock_irqrestore(&LinuxTraceState.mLockWordData, flags);

    addressTable[cidtid].currentOffset = 0;
    if( addressTable[cidtid].kernelPage == NULL)
      {
        addressTable[cidtid].kernelPage = kmalloc(k_PageSize, GFP_KERNEL);
      }
    if( next_data_page < LinuxTraceState.nents && addressTable[cidtid].kernelPage )
      {
        addressTable[cidtid].currentPage = next_data_page;
      }
    else
      {
        TraceOffLinux();
      }

  }
static void FixUpLimitTraceData(uint64_t traceStamp, int cidtid)
  {
    AllocateNewPage(cidtid);
    if(LinuxTraceState.mOn)
      {
        *(uint64_t *) addressTable[cidtid].kernelPage = traceStamp;
        addressTable[cidtid].currentOffset = sizeof(uint64_t);
      }
  }
static void FixUpLimitTraceData1(uint64_t traceStamp, int cidtid, int ContextNumber)
  {
    AllocateNewPage(cidtid);
    if(LinuxTraceState.mOn)
      {
        *(uint64_t *) addressTable[cidtid].kernelPage = traceStamp;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)) = ContextNumber;
        addressTable[cidtid].currentOffset = sizeof(uint64_t)+sizeof(int);
      }
  }
static void FixUpLimitTraceData2(uint64_t traceStamp, int cidtid, int ContextNumber, int ContextNumber2)
  {
    AllocateNewPage(cidtid);
    if(LinuxTraceState.mOn)
      {
        *(uint64_t *) addressTable[cidtid].kernelPage = traceStamp;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)) = ContextNumber;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)+sizeof(int)) = ContextNumber2;
        addressTable[cidtid].currentOffset = sizeof(uint64_t)+2*sizeof(int);
      }
  }
static void FixUpLimitTraceData3(uint64_t traceStamp, int cidtid, int ContextNumber, int ContextNumber2, int ContextNumber3)
  {
    AllocateNewPage(cidtid);
    if(LinuxTraceState.mOn)
      {
        *(uint64_t *) addressTable[cidtid].kernelPage = traceStamp;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)) = ContextNumber;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)+sizeof(int)) = ContextNumber2;
        *(int *) (addressTable[cidtid].kernelPage+sizeof(uint64_t)+2*sizeof(int)) = ContextNumber3;
        addressTable[cidtid].currentOffset = sizeof(uint64_t)+3*sizeof(int);
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
    endi++;
    }

//  if( endi == PKFXLOG_MAX_PATH )
//    FxLogger_Abort( "Unreasonably long file pathname" );

  {
    int slashcount = 0;
    int slashstart;
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
  }
    //  if( starti > aPathParts )
    //     starti = endi - aPathParts;

  return( & aFN[ starti ] );
  }
// pkTraceStreamConnect called once per trace point to register that point
// it gets a trace stream id from the server.  It should involve a lock,
// but for usage with the RoQ we are running lockless.
static int FxLoggerNodeId = -1;
static void PkTraceStreamInit(PkTraceStream *TraceStream) __attribute__((unused));
static void PkTraceStreamInit(PkTraceStream *TraceStream)
  {
    TraceStream->mGeneration = 0;
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
  const char *ShortSourceFileName;
  int nloop = 0;

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

  ShortSourceFileName = FxLogger_GetStartPointInFilePathName( SourceFileName,PKFXLOG_PATHNAME_DIRECTORIES_LIMIT  );

  sprintf( TraceName, "<tracename><node>%04d</node><composedname>%s</composedname><fn>%s</fn><line>%04u</line></tracename>",
           FxLoggerNodeId == -1 ? TraceStreamContextOrdinal : FxLoggerNodeId,
//           0,
           ComposedTraceName,
           ShortSourceFileName,
           SourceLineNo);

  while(TraceName[nloop] != '\0')
    {
//    if( TraceName[nloop] == '/' )
//      TraceName[nloop] = '|';
//    else
      if( TraceName[nloop] == ' ' )
      TraceName[nloop] = '_';

    nloop++;
    }

  {
    int namelength=strlen(TraceName);
    unsigned long flags;

    dprint_tsk(DBG_TRC, KERN_INFO, " TraceName=\"%s\" namelength=%d\n",TraceName,namelength);
    spin_lock_irqsave(&LinuxTraceState.mLockWordName,flags);
    if (LinuxTraceState.mNextTraceId + namelength + 2 < k_PageSize)
      {
        struct page *mapPage=sg_page(LinuxTraceState.mTraceIdPlist);
        char *mapAddress = kmap(mapPage);
        dprint_tsk(DBG_TRC, KERN_INFO, " mapPage=%p mapAddress=%p mNextTraceId=%d\n",
            mapPage,mapAddress,LinuxTraceState.mNextTraceId);
        strcpy(mapAddress+LinuxTraceState.mNextTraceId,TraceName);
        kunmap(mapPage);
        LinuxTraceState.mNextTraceId += namelength+1;
      }
    {
      int TraceStreamId = LinuxTraceState.mHiTraceStreamId;
      LinuxTraceState.mHiTraceStreamId = TraceStreamId+1;
      spin_unlock_irqrestore(&LinuxTraceState.mLockWordName,flags);

      TraceStream->mGeneration = LinuxTraceState.mGeneration;

      return TraceStreamId;
    }
  }
  }
// Inline injected at each trace point. Simple case is handled inline, anything more complex
// results in a call to PkTraceStreamConnect or FixUpLimitTraceData to fix things up.
// Just a trace ID and a timestamp
static inline void PkTraceStreamSource(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName)
  {
    if(LinuxTraceState.mOn)
      {
        unsigned long flags;
        if(TraceStream->mGeneration !=  LinuxTraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        local_irq_save(flags);
        {
          int cidtid=get_pir() & ( k_pirsize-1);
          uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48);
          int currentOffset = addressTable[cidtid].currentOffset;
          if( currentOffset < k_PageSize)
            {
              *(uint64_t *) (addressTable[cidtid].kernelPage+currentOffset) = traceStamp;
              addressTable[cidtid].currentOffset = currentOffset+sizeof(uint64_t);
            }
          else
            {
              FixUpLimitTraceData(traceStamp,cidtid);
            }
        }
        local_irq_restore(flags);
      }

  }
// A trace ID, timestamp, and one integer
static inline void PkTraceStreamSource1(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber)
  {
    if(LinuxTraceState.mOn)
      {
        unsigned long flags;
        if(TraceStream->mGeneration !=  LinuxTraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        local_irq_save(flags);
        {
          int cidtid=get_pir() & ( k_pirsize-1);
          uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0x4000000000000000ULL;
          int currentOffset = addressTable[cidtid].currentOffset;
          if( currentOffset+sizeof(uint64_t)+sizeof(int) <= k_PageSize)
            {
              *(uint64_t *) (addressTable[cidtid].kernelPage+currentOffset) = traceStamp;
              currentOffset = currentOffset+sizeof(uint64_t);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber;
              addressTable[cidtid].currentOffset = currentOffset+sizeof(int);
            }
          else
            {
              FixUpLimitTraceData1(traceStamp,cidtid,ContextNumber);
            }
        }
        local_irq_restore(flags);
      }

  }
// A trace ID, timestamp, and two integers
static inline void PkTraceStreamSource2(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber, int ContextNumber2)
  {
    if(LinuxTraceState.mOn)
      {
        unsigned long flags;
        if(TraceStream->mGeneration !=  LinuxTraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        local_irq_save(flags);
        {
          int cidtid=get_pir() & ( k_pirsize-1);
          uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0x8000000000000000ULL;
          int currentOffset = addressTable[cidtid].currentOffset;
          if( currentOffset+sizeof(uint64_t)+2*sizeof(int) <= k_PageSize)
            {
              *(uint64_t *) (addressTable[cidtid].kernelPage+currentOffset) = traceStamp;
              currentOffset = currentOffset+sizeof(uint64_t);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber;
              currentOffset = currentOffset+sizeof(int);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber2;
              addressTable[cidtid].currentOffset = currentOffset+sizeof(int);
            }
          else
            {
              FixUpLimitTraceData2(traceStamp,cidtid,ContextNumber,ContextNumber2);
            }
        }
        local_irq_restore(flags);
      }

  }
// A trace ID, timestamp, and three integers
static inline void PkTraceStreamSource3(PkTraceStream *TraceStream, int LineNo, const char *FileName, const char *ContextName, int Ordinal, const char *EventName, int ContextNumber, int ContextNumber2, int ContextNumber3)
  {
    if(LinuxTraceState.mOn)
      {
        unsigned long flags;
        if(TraceStream->mGeneration !=  LinuxTraceState.mGeneration)
          {
            TraceStream->mTraceStreamId=PkTraceStreamConnect(TraceStream,FileName,LineNo,ContextName,Ordinal,EventName);
          }
        local_irq_save(flags);
        {
          int cidtid=get_pir() & ( k_pirsize-1);
          uint64_t traceStamp = (get_tb() & 0x0000ffffffffffffULL) | (((uint64_t)TraceStream->mTraceStreamId) << 48) | 0xc000000000000000ULL;
          int currentOffset = addressTable[cidtid].currentOffset;
          if( currentOffset+sizeof(uint64_t)+3*sizeof(int) <= k_PageSize)
            {
              *(uint64_t *) (addressTable[cidtid].kernelPage+currentOffset) = traceStamp;
              currentOffset = currentOffset+sizeof(uint64_t);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber;
              currentOffset = currentOffset+sizeof(int);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber2;
              currentOffset = currentOffset+sizeof(int);
              *(int *)(addressTable[cidtid].kernelPage+currentOffset) = ContextNumber3;
              addressTable[cidtid].currentOffset = currentOffset+sizeof(int);
            }
          else
            {
              FixUpLimitTraceData3(traceStamp,cidtid,ContextNumber,ContextNumber2,ContextNumber3);
            }
        }
        local_irq_restore(flags);
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
