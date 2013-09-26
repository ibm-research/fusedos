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

#include "Kernel.h"
#include "ctype.h"
#include <limits.h>
#include <string.h>
#include <unistd.h>
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include <sys/time.h>                                                                                                  // FUSEDOS
#endif                                                                                                                 // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include "ran32.h"

uint64_t hasKernelCrashed K_ATOMIC;
uint64_t hasKernelExitted K_ATOMIC;

//
// NOTE: See util.h which contains #defines that work better!
//
//
// Misc. utility functions
//
void __NORETURN Kernel_Crash( int status )
{
    uint64_t crashcount = L2_AtomicLoadIncrement(&hasKernelCrashed);
    if(status != 0)
    {
        Kernel_WriteFlightLog(FLIGHTLOG_high, FL_KERNCRASH, status, mfspr(SPRN_LR), mfspr(SPRN_SRR0), mfspr(SPRN_CSRR0));
        if(crashcount == 0)
        {
            dumpFlightRecorder();
        }
    }
    NodeState.FW_Interface.terminate( status );
    while (1)
    {
    }
}

void __NORETURN Kernel_Exit( int status )
{
    uint64_t exitcount = L2_AtomicLoadIncrement(&hasKernelExitted);
    if(status != 0)
    {
        Kernel_WriteFlightLog(FLIGHTLOG, FL_KERNEXITD, status, mfspr(SPRN_LR), mfspr(SPRN_SRR0), mfspr(SPRN_CSRR0));
        if(exitcount == 0)
        {
            dumpFlightRecorder();
        }
    }
    NodeState.FW_Interface.terminate( status );
    while (1)
    {
    }
}

void __NORETURN Kernel_EarlyCrash( int status )
{
    NodeState.FW_Interface.terminate( status );
    while (1);
}

int usleep(useconds_t usec)
{
    uint64_t cyclesPerMicro = GetPersonality()->Kernel_Config.FreqMHz;
    uint64_t end = GetTimeBase() + usec * cyclesPerMicro;
    while(end > GetTimeBase())
    {
        Delay(100);
    }
    
    return 0;
}

int puts( const char *s )
{
   int rc;
   if((NodeState.TraceConfig == 0) && (CONFIG_ALLOWPRINTF == 0))
       return 0;
   
   size_t len = strlen_Inline(s);
   char buffer[256];
   memcpy(buffer, s, len);
   buffer[len] = '\n';
   buffer[len+1] = 0;
   rc = NodeState.FW_Interface.putn(buffer, len+1 );
   
   return( rc );
}

int printf( const char *fmt, ... )
{
    va_list args;
    char buffer[256];
    
    if((NodeState.TraceConfig == 0) && (CONFIG_ALLOWPRINTF == 0))
        return 0;
    
    va_start( args, fmt );

    int len = vsnprintf(buffer, sizeof(buffer), fmt, args );
    NodeState.FW_Interface.putn(buffer, len);

    va_end(args);

    return len;
}

int memcmp(const void *s1, const void *s2, size_t bytes)
{
   size_t i;
   char* m1 = (char*)s1;
   char* m2 = (char*)s2;
   for (i=0;i<bytes;i++) {
      if (*m1 != *m2) {
         return (*m1<*m2) ? -1:1;
      }
      m1++;
      m2++;
   }
   return 0;
}

void *memcpy( void *dest, const void *src, size_t n )
{
    if(n>128*1024)   Kernel_WriteFlightLog(FLIGHTLOG, FL_MEMCPY___, mfspr(SPRN_LR), (uint64_t)dest, (uint64_t)src, n);
    
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

void* memset(void* dest, int value, size_t n )
{
    if(n>128*1024)   Kernel_WriteFlightLog(FLIGHTLOG, FL_MEMSET___, mfspr(SPRN_LR), (uint64_t)dest, (uint64_t)value, n);
    if ( !value )
       {
       bzero_Inline( dest, n );
       }
    else
       {
       uint8_t* d = (uint8_t*)dest;
       while (n--)
           *d++ = value;
       }
    return dest;
}

//
// ascii to integer conversion
//
int atoi_( const char *str )
{
    int is_neg = 0;
    int value = 0;
    char *p = (char *)str;
    char *q = NULL;
    
    if ( !p )
        return(0); // terminated string. exit with zero
    while ( isspace(*p) )
        p++;
    if ( !*p )
        return(0); // terminated string. exit with zero
    if ( *p == '-' )
    {
        is_neg++;
        p++;
    }
    else if ( *p == '+' )
    {
        p++;
    }
    if ( !*p )
        return( 0 ); // terminated string. exit with zero
    q = p;
    while ( isdigit(*q) )
        q++;
    while ( p < q )
    {
        value *= 10;
        value += (*p - '0');
        p++;
    }
    return(is_neg ? (-(value)) : value);
}

//
// ascii to base-n conversion
//
uint64_t strtoull_(const char *str, char **endptr, int base)
{
    int is_neg = 0;
    uint64_t value = 0;
    uint64_t digitvalue;
    char *p = (char *)str;
    
    if ( !p )
        return(0); // terminated string. exit with zero
    
    while ( isspace(*p) )
        p++;
    if ( !*p )
        return(0); // terminated string. exit with zero
    if ( *p == '-' )
    {
        is_neg++;
        p++;
    }
    else if ( *p == '+' )
    {
        p++;
    }
    while((*p == '0') || (*p == 'x'))
    {
        p++;
    }
    
    while(isalnum(*p))
    {
        if(isdigit(*p))
            digitvalue = (*p) - '0';
        else
            digitvalue = (toupper(*p)) - 'A' + 10;
        if(digitvalue >= base)
            break;
        
        value *= base;
        value += digitvalue;
        p++;
    }
    if(endptr != NULL)
        *endptr = p;
    
    return(is_neg ? (-(value)) : value);
}

int snprintf( char *buf, size_t cnt, const char *fmt, ... )
{
    va_list args;
    int i;

    va_start(args, fmt);
    i=vsnprintf(buf,cnt, fmt, args);
    va_end(args);
    return i;
}

//! \brief  Calculate the length of a string up to a maximum length.
//! \param  s Pointer to string.
//! \param  maxlen Maximum length of string.
//! \retrun Number of bytes in string.

size_t strnlen(const char* s, size_t maxlen)
{
   size_t len;
   for(len=0; ((s[len]!=0) && (len<maxlen)); ++len);
   return len;
}
#endif                                                                                                                 // FUSEDOS

//! \brief  Validate a pathname string.
//! \param  pathname Pointer to pathname string.
//! \param  errnum Pointer to.
//! \return Length of pathname string when successful, -1 when unsuccessful.

uint64_t validatePathname(const char *pathname)
{
  // Make sure the pointer is valid.
  if (!VMM_IsAppAddress(pathname, 1)) {
    return CNK_RC_FAILURE(EFAULT);
  }

  // See if the pathname fits in user memory.
  size_t maxlen = PATH_MAX;
  size_t len = 0;
#if TODO
  if (!VMM_IsAppAddress(pathname, maxlen)) {
    maxlen = appEndAddress() - (unsigned int)pathname + 1;
  }
#endif

  // Get the length of the pathname string
  len = strnlen(pathname, maxlen);
  if (len == maxlen && maxlen != PATH_MAX) { // Went past the end of memory
    return CNK_RC_FAILURE(EINVAL);
  }
  if (len == maxlen) {
    return CNK_RC_FAILURE(ENAMETOOLONG);
  }
  ++len; // Include terminating null character in length

  return CNK_RC_SUCCESS(len);
}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
char mallocpool[MALLOC_ARENASIZE];
uint64_t _malloc_pointer = (uint64_t)((void*)&mallocpool[0]);
uint64_t _malloc_origin  = (uint64_t)((void*)&mallocpool[0]);
Lock_Atomic_t malloclock;

void* cnk_malloc(size_t size)
{   
   void* ptr = NULL;
   Kernel_WriteFlightLog(FLIGHTLOG, FL_MALLOC___, mfspr(SPRN_LR), size, 0, 0);   
   
   // thread safety lock
   Kernel_Lock(&malloclock);
   
   // shift malloc pointer to be aligned
   _malloc_pointer = (_malloc_pointer + (MALLOC_ALIGNMENT-1)) & (~(MALLOC_ALIGNMENT-1));
   
   // size calculation
   if(size + _malloc_pointer - _malloc_origin <= MALLOC_ARENASIZE)
   {
      ptr = (void *) _malloc_pointer;
      _malloc_pointer += size;
   }
   ppc_msync();
   
   Kernel_Unlock(&malloclock);
   return ptr;
}

void cnk_free(void* ptr)
{
   // Design decision:  CNK will not reclaim freed memory.
   return;
}
#endif                                                                                                                 // FUSEDOS

uint64_t ConvertTimeToMicroseconds(uint64_t timeBaseCycles)
{
    uint64_t cyclesPerMicro = GetPersonality()->Kernel_Config.FreqMHz;
    return((timeBaseCycles / cyclesPerMicro) + GetMyAppState()->Timeshift);
}

uint64_t ConvertTimeToCycles(uint64_t timeinMicroseconds)
{
    timeinMicroseconds -= GetMyAppState()->Timeshift;
    return timeinMicroseconds * GetPersonality()->Kernel_Config.FreqMHz;
}

uint64_t GetCurrentTimeInMicroseconds(void)
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    return ConvertTimeToMicroseconds(GetTimeBase());
#else                                                                                                                  // FUSEDOS
	struct timeval tv;                                                                                                 // FUSEDOS
	gettimeofday(&tv, NULL);                                                                                           // FUSEDOS
	return tv.tv_sec * 1000000L + tv.tv_usec;                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
Personality_t* getPersonality()
{
    return GetPersonality();
}

int __gxx_personality_v0;

void __NORETURN __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function)
{
    RASBEGIN(4);
    RASPUSH((uint64_t)file);
    RASPUSH(line);
    RASPUSH((uint64_t)function);
    RASPUSH((uint64_t)assertion);
    RASFINAL(RAS_KERNELASSERTIONFAILURE);
    
    Kernel_Crash(RAS_KERNELASSERTIONFAILURE);
}

int Kernel_InjectRAWRAS(uint32_t message_id, size_t raslength, const uint64_t* rasdata) 
{
    return NodeState.FW_Interface.writeRASEvent(message_id, raslength/sizeof(uint64_t), (fw_uint64_t*)rasdata);
}

// Inject random length delays in random sets of hardware threads by toggling the thread enable DCRs. The usage of
// this tool on DD1 hardware may result in a hang or you may see a warning message printed. This is due to the control 
// system also saving/restoring the thread enables during mailbox activity and these thread enables are spread
// across two DCRs (cores0-15, core16). The expected use of this function is in mambo simulation environments.
//
void Kernel_jitterbug(uint64_t seed)
{
    uint32_t seed1;
    uint32_t seed2;
    uint32_t delay_cycles;
    uint32_t starting_seed = seed; 
    uint32_t maxdelay = 16; // Use a 16 bit field to limit the max value for the randomized delay cycles (0 - 65535)
    uint32_t hwthread_mask = 0;

    // setup two seeds 
    ran32_set_seed(&seed1, starting_seed);
    ran32_set_seed(&seed2, ran32_uint(&seed1));
    
    //printf("Jitterbug is active. seed=%u\n", starting_seed); 

    uint64_t threadActive0 = DCRReadPriv(TESTINT_DCR(THREAD_ACTIVE0));
    if (threadActive0 != 0xFFFFFFFFFFFFFFFF)
    {
        printf("(W) Jitterbug encountered unexpected THREAD_ACTIVE0 value: %016lx. Exiting.\n", threadActive0);
        return;
    }
    // Loop until the job exits
    while(1)
    {
        // Test to see if the job has exited
        if (!NodeState.AppState[0].Active_Cores)
        {
            // Time to bail out. Return back through the IPI interrupt flow that got us here.
            // We will be put back into the scheduler.
            return;
        }
        // Generate the mask indicating which hardware threads will be targeted for jitter
        hwthread_mask = ran32_64bits(&seed1);
        // Generate random delay 
        delay_cycles = ran32_nbits(&seed2, maxdelay); // Generate a delay value x bits in size
        
        // Turn off all the threads using the mask
        DCRWritePriv( TESTINT_DCR(THREAD_ACTIVE0), threadActive0 & hwthread_mask );
        // Spin for the delay value
        while (delay_cycles--)
        {
            if(!NodeState.AppState[0].Active_Cores) break;
        }
        // restore the original thread enables
        DCRWritePriv( TESTINT_DCR(THREAD_ACTIVE0), threadActive0);
    }
}


int PerfCtr_GetNodeData(int numitems, PerfCountItem_t* items)
{
    int x;
    for(x=0; x<numitems; x++)
    {
        if(items[x].id < CNK_NODEPERFCOUNT_NUM)
        {
            items[x].value = NodeState.PerformanceCounter[items[x].id];
        }
    }
    return 0;
}

int PerfCtr_GetProcData(int numitems, PerfCountItem_t* items)
{
    int x;
    AppProcess_t* proc = GetMyProcess();
    for(x=0; x<numitems; x++)
    {
        if(items[x].id < CNK_PROCPERFCOUNT_NUM)
        {
            items[x].value = proc->PerformanceCounter[items[x].id];
        }
    }
    return 0;
}

int PerfCtr_GetHWTData(int numitems, PerfCountItem_t* items)
{
    int x;
    HWThreadState_t* hwt = GetMyHWThreadState();
    for(x=0; x<numitems; x++)
    {
        if(items[x].id < CNK_HWTPERFCOUNT_NUM)
        {
            items[x].value = hwt->PerformanceCounter[items[x].id];
        }
    }
    return 0;
}
#endif                                                                                                                 // FUSEDOS

