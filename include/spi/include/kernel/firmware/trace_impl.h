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
/* (C) Copyright IBM Corp.  2008, 2012                              */
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

#ifndef	_KERNEL_IMPL_TRACE_H_ /* Prevent multiple inclusion */
#define	_KERNEL_IMPL_TRACE_H_

#include "kernel_impl.h" 
#include "hwi/include/common/compiler_support.h"
#include "spi/include/l2/atomic.h"
#include <stdio.h>

__BEGIN_DECLS

__INLINE__
uint64_t Kernel_WriteFlightLog(uint64_t* flightlock, BG_FlightRecorderLog_t flightlog[], uint64_t flightsize, 
                               uint32_t ID, uint64_t data0, uint64_t data1, uint64_t data2, uint64_t data3)
{
      uint64_t myentry = L2_AtomicLoadIncrement(flightlock) % flightsize;
      flightlog[myentry].timestamp = GetTimeBase();
      flightlog[myentry].id      = ID;
      flightlog[myentry].hwthread= (mfspr(SPRN_SPRG7))&0x7f;
      flightlog[myentry].data[0] = data0;
      flightlog[myentry].data[1] = data1;
      flightlog[myentry].data[2] = data2;
      flightlog[myentry].data[3] = data3;
      return myentry;
}


__INLINE__
int Kernel_DecodeFlightLogs(uint32_t numlogs, BG_FlightRecorderRegistry_t* logregistry, size_t bufferSize, char* buffer, uint64_t* moreData)
{
    uint64_t x;
    uint64_t l;
    uint64_t lsel;
    uint64_t length;
    uint64_t nexttime;
    uint64_t count[8];
    uint64_t counttotal[8];
    
    // Acquire current flight recorder lock pointers
    for(l=0; l<numlogs; l++)
    {
        if(logregistry[l].lastStateSet == 0)
        {
            count[l] = L2_AtomicLoad(logregistry[l].flightlock) % logregistry[l].flightsize;
            counttotal[l] = 0;
            
            for(x=0; x<logregistry[l].flightsize; x++)
            {
                if((logregistry[l].flightlog[ count[l] ].id) != 0)
                    break;
                count[l] = (count[l]+1) % logregistry[l].flightsize;
                counttotal[l]++;
            }
        }
        else
        {
            count[l] = logregistry[l].lastState;
            counttotal[l] = logregistry[l].lastStateTotal;
        }
    }
    
    do
    {
        *moreData = 0;
        nexttime = 0xffffffffffffffffull;
        lsel     = 0xffffffffffffffffull;
        for(l=0; l<numlogs; l++)
        {
            if((logregistry[l].flightlog[count[l]].timestamp < nexttime) && (counttotal[l] < logregistry[l].flightsize))
            {
                nexttime = logregistry[l].flightlog[count[l]].timestamp;
                lsel = l;
            }
        }
        
        if(lsel != 0xffffffffffffffffull)
        {
            *moreData = 1;
            if(bufferSize < 256)
            {
                return 0;
            }
            
            int set = logregistry[lsel].lastStateSet;
            BG_FlightRecorderLog_t* log = &logregistry[lsel].flightlog[count[lsel]];
            count[lsel] = (count[lsel]+1) % logregistry[lsel].flightsize;
            counttotal[lsel]++;
            logregistry[lsel].lastStateSet = 1;
            logregistry[lsel].lastState = count[lsel];
            logregistry[lsel].lastStateTotal = counttotal[lsel];
            
            if(log->id >= logregistry[lsel].num_ids)
            {
                length = snprintf(buffer, bufferSize, "TB=%016lx FL_INVALDLOG:%-2d An invalid entry with registry=\"%s\"  id=%d was detected (valid ID range 0-%d)\n", log->timestamp, log->hwthread, logregistry[lsel].registryName, log->id, logregistry[lsel].num_ids);
                bufferSize -= length;
                buffer     += length;
                return 0;
            }
            
            BG_FlightRecorderFormatter_t* fmt = &logregistry[lsel].flightformatter[log->id];
            if(set == 0)
            {
                length = snprintf(buffer, bufferSize, "TB=%016lx FL_BEGIN_LOG:-- Starting log \"%s\"\n", log->timestamp, logregistry[lsel].registryName);
                bufferSize -= length;
                buffer     += length;
            }
            
            length = snprintf(buffer, bufferSize, "TB=%016lx %s:%-2d ", log->timestamp, fmt->id_str, log->hwthread);
            bufferSize -= length;
            buffer     += length;
            
            switch(fmt->type)
            {
                case FLIGHTRECORDER_PRINTF:
                    length = snprintf(buffer, bufferSize, fmt->formatString, log->data[0], log->data[1], log->data[2], log->data[3]);
                    bufferSize -= length;
                    buffer += length;
                    break;
                case FLIGHTRECORDER_FUNC:
                    fmt->formatFunction(bufferSize, buffer, log, fmt->anchorPtr);
                    length = strlen(buffer);
                    if(! (bufferSize >= length))
                    {
                        printf("Flightlog Formatter %s did not adhere to buffer length.  BufferSize=%ld  length=%ld\n", fmt->id_str, bufferSize, length);
                        printf("value=%lx %lx %lx %lx\n", log->data[0], log->data[1], log->data[2], log->data[3]);
                        printf("wrotebuffer: %s\n", buffer);
                    }
                    assert(bufferSize >= length);
                    bufferSize -= length;
                    buffer     += length;
                    break;
                default:
                    break;
            }
            
            if(bufferSize >= 1)
            {
                buffer[0] = '\n';
                buffer[1] = 0;
                bufferSize--;
                buffer++;
            }
        }
    }
    while(*moreData);
    return 0;
}

__END_DECLS

#endif /* _KERNEL_TRACE_H_ */
