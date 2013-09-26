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
//#include <stdio.h>
//#include <ctype.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <stdlib.h>

#include <stdint.h>
#include <string.h>
#include "spi/include/kernel/location.h"

#define NUMDIMENSION     6

inline int MUSPI_PreCalculateMapOffsets(const char* mapFilename, const uint32_t* dimsize, uint32_t* dimoffset)
{
    int x;
    int y;
    uint32_t lastmultiplier = 1;
    int hitcount[NUMDIMENSION];
    memset(hitcount, 0, sizeof(hitcount));
    
    if((strlen(mapFilename) >= NUMDIMENSION) && (!isspace(mapFilename[NUMDIMENSION])) && (mapFilename[NUMDIMENSION] != 0))
    {
        // Not a pre-calculated mapping
        return -1;
    }
    
    for(x=0; x<NUMDIMENSION; x++)
    {
        if(((mapFilename[x] >= 'A') && (mapFilename[x] <= 'E')) ||
           (mapFilename[x] == 'T'))
        {
            y = (mapFilename[x] - 'A') & 0x1f;
            if(y == 'T'-'A')
                y = 5;
            hitcount[y]++;
            if(hitcount[y] > 1)
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }
    
    for(x=NUMDIMENSION; x>0; )
    {
        x--;
        y = (mapFilename[x] - 'A') & 0x1f;
        if(y == 'T'-'A')
            y = 5;
        dimoffset[y] = lastmultiplier;
        lastmultiplier = lastmultiplier * dimsize[y];
    }
    return 0;
}

inline int MUSPI_GenerateCoordinates(const char *mapFilename, const BG_JobCoords_t* jobcoord, BG_CoordinateMapping_t* mycoord, uint32_t numProcesses, uint32_t np, 
                                     size_t tmpStorageSize, void* tmpStorage, BG_CoordinateMapping_t map[], uint32_t* myRank, uint32_t* mpmdFound)
{
    uint32_t x;
    uint32_t y;
    uint32_t dimoffset[NUMDIMENSION];
    uint32_t dimsize[NUMDIMENSION];
    uint32_t coord[NUMDIMENSION];
    uint64_t nodeOffset;
    bool     calculatedMap = false;
    bool     comment;
    bool     hasDigits;
    int fd        = -1;
    size_t offset = 0;
    size_t size   = 0;
    
    size_t buffersize = tmpStorageSize;
    char* buffer = (char*)tmpStorage;
    char line[256];
    
    if((myRank != NULL) && (mycoord == NULL))
        return -1;
    
    if((jobcoord == NULL) || (mapFilename == NULL))
        return -1;
    
    if(mpmdFound)
        *mpmdFound = 0;
    
    dimsize[0] = jobcoord->shape.a;
    dimsize[1] = jobcoord->shape.b;
    dimsize[2] = jobcoord->shape.c;
    dimsize[3] = jobcoord->shape.d;
    dimsize[4] = jobcoord->shape.e;
    dimsize[5] = numProcesses;
    
    // Determine if this is a calculate map
    if(MUSPI_PreCalculateMapOffsets(mapFilename, dimsize, dimoffset) == 0)
        calculatedMap = true;
    
    if(!calculatedMap)
    {
        fd = open(mapFilename, 0,0);
        if(fd<0)
            return -1;
        offset = 0;
        size = read(fd, buffer, buffersize);
    }
    
    for(x=0; x<np; x++)
    {
        if(calculatedMap)
        {
            for(y=0; y<NUMDIMENSION; y++)
                coord[y] = (x / dimoffset[y]) % dimsize[y];
        }
        else
        {
            do
            {
                y=0;
                comment = false;
                hasDigits = false;
                
                if(buffer[offset] == 0)
                    break;
                
                while(buffer[offset])
                {
                    int value = buffer[offset];
                    offset++;
                    if((isdigit(value)) && (comment == false))
                        hasDigits = true;
                    if(value == '#')
                    {
                        if((!comment) && (mpmdFound != NULL) && (strncmp(&buffer[offset-1], "#mpmd", 5) == 0))
                        {
                            *mpmdFound = 1;
                        }
                        if((!comment) && (strncmp(&buffer[offset-1], "#mapping", 8) == 0))
                        {
                            char* str = &buffer[offset+7];
                            while(isspace(*str))
                                str++;
                            if(MUSPI_PreCalculateMapOffsets(str, dimsize, dimoffset) == 0)
                                calculatedMap = true;
                            else
                            {
                                // invalid value
                                if(fd >= 0)
                                {
                                    close(fd);
                                    fd = -1;
                                }
                                return -1;
                            }
                            for(int yy=0; yy<NUMDIMENSION; yy++)
                                coord[yy] = (x / dimoffset[yy]) % dimsize[yy];
                            
                            hasDigits = true;
                        }
                        value = 0;
                        comment = true;
//                        line[y++] = value;
//                        break;
                    }
                    else if((isalpha(value)) && (comment == false))
                    {
                        if(fd >= 0)
                        {
                            close(fd);
                            fd = -1;
                        }
                        return -1;
                    }
                    else if(value == '\n')
                    {
                        line[y] = 0;
                        break;
                    }
                    line[y++] = value;
                    if((size_t)y >= sizeof(line))
                    {
                        if(fd >= 0)
                        {
                            close(fd);
                            fd = -1;
                        }
                        return -1;
                    }
                }
            }
            while(!hasDigits);

            if(!hasDigits)
            {
                if(fd >= 0)
                {
                    close(fd);
                    fd = -1;
                }
                return -1;
            }
            
            if(!calculatedMap)
            {
                char* ptr = line;
                for(y=0; y<NUMDIMENSION; y++)
                {
                    coord[y] = strtoull(ptr, &ptr, 10);
                    
                }
            }
            if(offset > buffersize/2)
            {
                memcpy(&buffer[0], &buffer[buffersize/2], size-buffersize/2);
                offset -= buffersize/2;
                size   -= buffersize/2;
                size   += read(fd, &buffer[size], buffersize/2);
            }
        }
        
        uint32_t nodeSize=1;
        nodeOffset = 0;
        for(y=0; y<NUMDIMENSION; y++)
        {
            nodeOffset = nodeOffset*dimsize[y] + coord[y];
            nodeSize   = nodeSize*dimsize[y];
        }
        if(nodeOffset >= nodeSize)
        {
            if(fd >= 0)
            {
                close(fd);
                fd = -1;
            }
            return -1;
        }
        if (map != NULL) 
        {
            map[x].a = coord[0];
            map[x].b = coord[1];
            map[x].c = coord[2];
            map[x].d = coord[3];
            map[x].e = coord[4];
            map[x].t = coord[5];
            map[x].reserved = 0;
        }
        if(myRank != NULL)
        {
            // mycoord is relative to the job, not the entire block.
            if ((coord[0] == mycoord->a) &&
                (coord[1] == mycoord->b) &&
                (coord[2] == mycoord->c) &&
                (coord[3] == mycoord->d) &&
                (coord[4] == mycoord->e) &&
                (coord[5] == mycoord->t))
            {
                *myRank = x;
                if (map == NULL)   // map[] isn't being generated, perform early exit.
                {
                    if(fd >= 0)
                    {
                        close(fd);
                        fd = -1;
                    }
                    return 0;
                }
            }
        }
    }
    if(fd >= 0)
    {
        close(fd);
        fd = -1;
    }
    return 0;
}


inline int MUSPI_GetMPMDData(const char *mapFilename, uint32_t myRank,
                             size_t tmpStorageSize, void* tmpStorage, uint32_t* sequenceID, size_t cmdstrsize, char* cmdline)
{
    int fd        = -1;
    size_t offset = 0;
    size_t size   = 0;
    int y = 0;
    
    size_t buffersize = tmpStorageSize;
    char* buffer = (char*)tmpStorage;
    char* str;
    char* str2;
    char line[256];
    uint32_t rank0, rank1, stride;
    int mpmddone = 0;
    int mpmdID = 0;
    int inBeginBlock = 0;
    int mpmdCmdCount = 0;
    
    int foundme = 0;
    
    if(mapFilename == NULL)
        return -1;
    
    fd = open(mapFilename, 0,0);
    if(fd<0)
        return -1;
    offset = 0;
    size = read(fd, buffer, buffersize);
    
    do
    {
        y=0;
        while(buffer[offset])
        {
            int value = buffer[offset];
            offset++;
            
            if((value == '#') && (strncmp(&buffer[offset-1], "#mpmd", 5) != 0))
            {
                line[y] = 0;
                break;
            }
            if(value == '\n')
            {
                line[y] = 0;
                
                // erase any trailing spaces
                while((y>0) && (isspace(line[y-1]) != 0))
                {
                    y--;
                    line[y] = 0;
                }
                break;
            }
            if(!isprint(value))
                continue;
            
            line[y++] = value;
            
            if((size_t)y >= sizeof(line))
            {
                if(fd >= 0)
                {
                    close(fd);
                    fd = -1;
                }
                return -1;
            }
            
            if(offset > buffersize/2)
            {
                memcpy(&buffer[0], &buffer[buffersize/2], size-buffersize/2);
                offset -= buffersize/2;
                size   -= buffersize/2;
                size   += read(fd, &buffer[size], buffersize/2);
            }
        }
        
        str = strstr(line, "#mpmdbegin ");
        if(str)
        {
            str += strlen("#mpmdbegin");
            
            do
            {
                if(inBeginBlock)
                {
                    // error
                    if(fd >= 0)
                    {
                        close(fd);
                        fd = -1;
                    }
                    return -1;
                }
                str2 = str;
                while(*str2)
                {
                    if((!isdigit(*str2)) && (*str2 != '-') && (*str2 != ',') && (*str2 != 'x') && (!isspace(*str2)))
                    {
                        if(fd >= 0)
                        {
                            close(fd);
                            fd = -1;
                        }
                        return -1;
                    }
                    str2++;
                }
                
                rank0 = strtoull(str, &str, 10);
                if((rank0 & 0x80000000ul) != 0)
                {
                    if(fd >= 0)
                    {
                        close(fd);
                        fd = -1;
                    }
                    return -1;
                }
                if(*str == '-')
                {
                    rank1 = strtoull(str+1, &str, 10);
                    if((rank1 < rank0) || ((rank1 & 0x80000000ul) != 0))
                    {
                        if(fd >= 0)
                        {
                            close(fd);
                            fd = -1;
                        }
                        return -1;
                    }
                    if((myRank >= rank0) && (myRank <= rank1))
                    {
                        stride = 1;
                        if(*str == 'x')
                            stride = strtoull(str+1, &str, 10);
                        if((myRank - rank0)%stride == 0)
                        {
                            foundme = true;
                        }
                    }
                }
                else if(myRank == rank0)
                    foundme = true;
                
                while((*str) && (*str != ','))
                    str++;
                if(*str == ',') 
                    str++;
            }
            while(*str);
            inBeginBlock = 1;
            mpmdCmdCount = 0;
        }
        
        str = strstr(line, "#mpmdcmd ");
        if(str)
        {
            if((mpmdCmdCount > 0) || (inBeginBlock == 0))
            {
                // error
                if(fd >= 0)
                {
                    close(fd);
                    fd = -1;
                }
                return -1;
            }
            str += strlen("#mpmdcmd");
            while(isspace(*str))
                str++;
            strncpy(cmdline, str, cmdstrsize);
            mpmdCmdCount++;
        }
        
        str = strstr(line, "#mpmdend");
        if(str)
        {
            str += strlen("#mpmdend");
            while(isspace(*str))
                str++;
            if(*str == 0)
            {
                if(mpmdCmdCount != 1)
                {
                    // error
                    if(fd >= 0)
                    {
                        close(fd);
                        fd = -1;
                    }
                    return -1;
                }
                if(inBeginBlock == 0)
                {
                    // error
                    if(fd >= 0)
                    {
                        close(fd);
                        fd = -1;
                    }
                    return -1;
                }
                if(foundme)
                {
                    *sequenceID = mpmdID;
                    mpmddone = true;
                }
                mpmdID++;
                inBeginBlock = 0;
            }
        }
        
        if(buffer[offset] == 0)
            break;
        
    }
    while(!mpmddone);
    
    if(inBeginBlock)
    {
        // error
        if(fd >= 0)
        {
            close(fd);
            fd = -1;
        }
        return -1;
    }
    if(fd >= 0)
    {
        close(fd);
        fd = -1;
    }
    if(foundme == 0)
        return -1;
    
    return 0;
}
