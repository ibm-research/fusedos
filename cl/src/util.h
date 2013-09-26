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

#ifndef _UTIL_H_ // Prevent multiple inclusion
#define _UTIL_H_

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <cnk/include/Config.h>
#include <spi/include/kernel/trace.h>
#include <spi/include/kernel/debug.h>
#include "flightlog.h"
#include "ras.h"

//
// Misc. utility functions
//
#define RASBEGIN(numexpected) { uint64_t rasstorage=numexpected; uint64_t rasindex=0; uint64_t rasarray[numexpected];
#define RASPUSH(value)          if(rasstorage > rasindex) { rasarray[rasindex++] = value; }
#define RASFINAL(message_id)    Kernel_InjectRAWRAS(message_id, rasindex*sizeof(uint64_t), rasarray); }

void __NORETURN Kernel_Crash( int status );
void __NORETURN Kernel_EarlyCrash( int status );
void __NORETURN Kernel_Exit( int status );

extern int       atoi_(const char *nptr);
extern uint64_t  strtoull_(const char *str, char **endptr, int base);
#endif                                                                                                                 // FUSEDOS
extern uint64_t  GetCurrentTimeInMicroseconds();
extern uint64_t  ConvertTimeToMicroseconds(uint64_t timeBaseCycles);
extern uint64_t  ConvertTimeToCycles(uint64_t timeinMicroseconds);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
size_t strnlen(const char* s, size_t maxlen);
#endif                                                                                                                 // FUSEDOS
uint64_t validatePathname(const char *pathname);
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
void*    cnk_malloc(size_t size);
void     cnk_free(void* ptr);

extern void Kernel_jitterbug(uint64_t seed);
#endif                                                                                                                 // FUSEDOS

#define MAX(a,b) ((a>b)?a:b)
#define MIN(a,b) ((a>b)?b:a)

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
__INLINE__ size_t strlen_Inline(const char* s) {
  size_t count = 0;
  while (*(s++) != 0) 
    count++;
  return count;
}

// Warning: Assumes "force load/store alignment" is off, i.e., misaligned ld/st is ok!
// \todo Actually, misaligned ld/st is not ok for performance.  Fix it!
__INLINE__ void *memcpy_Inline( void *dest, const void *src, size_t n )
{
   uint64_t *s64 = (uint64_t *)src;
   uint64_t *d64 = (uint64_t *)dest;
   dcache_block_touch( (void *)s64 );
   size_t h32  = (n >> 5);
   size_t t8, t1;
   register uint64_t ra, rb, rc, rd;

   while ( h32-- )
      {
      dcache_block_touch( (void *)(s64 + 4) );
      ra     = *s64++;
      rb     = *s64++;
      rc     = *s64++;
      rd     = *s64++;
      *d64++ = ra;
      *d64++ = rb;
      *d64++ = rc;
      *d64++ = rd;
      }

   if ( (t8 = ((n & 0x18) >> 3)) )
      {
      while ( t8-- )
         *d64++ = *s64++;
      }

   if ( (t1 = (n & 0x7)) )
      {
      uint8_t *s8 = (uint8_t *)s64;
      uint8_t *d8 = (uint8_t *)d64;

      while ( t1-- )
         *d8++ = *s8++;
      }

   return dest;
}


__INLINE__ void bzero_Inline( void *dest, size_t n )
{
    uint64_t iterations;
    uint8_t *d8 = (uint8_t*)dest;
    
    while(n)
    {
        if((((uint64_t)d8 & 63)==0)&&(n>=64))
        {
            iterations = (n>>6);
            n -= iterations*64;
            while(iterations--)
            {
                dcache_block_zero(d8);
                d8 += 64;
            }
        }
        else if((((uint64_t)d8 & 7)==0)&&(n>=8))
        {
            
            iterations = (MIN(n, ((((uint64_t)d8+63)&(~63))-(uint64_t)d8)) >> 3);
            if(iterations==0) iterations = n>>3;
            n -= iterations*8;
            while(iterations--)
            {
                *((uint64_t*)d8) = 0;
                d8+=8;
            }
        }
        else
        {
            iterations = MIN(n, ((((uint64_t)d8+7)&(~7))-(uint64_t)d8));
            if(iterations==0) iterations = n;
            n -= iterations;
            while(iterations--)
            {
                *((uint8_t*)d8) = 0;
                d8++;
            }
        }
    }
}
#endif                                                                                                                 // FUSEDOS

#endif // Add nothing below this line.

