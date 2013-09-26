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

#include <firmware/include/fwext/fwext.h>
#include <firmware/include/mailbox.h>
#include <hwi/include/bqc/testint_tdr.h>
#include <hwi/include/bqc/testint_inlines.h>
#include <firmware/include/fwext/fwext_lib.h>
#include <stdio.h>
#include <roq_kernel.h>

uint64_t vaddrLimit = 0;
extern volatile Firmware_Interface_t *_firmware_domain;
#define FIRMWARE()  _firmware_domain


__INLINE__ int fw_isspace( char c ) {
  return ( ( c == ' ' ) || ( c == '\t') ) ? 1 : 0;
}

__INLINE__ int fw_isdigit( char c ) {
  return ( ( c >= '0' ) && ( c <= '9') ) ? 1 : 0;
}

__INLINE__ int fw_ishexlower( char c ) {
  return ( (c >= 'a') && (c <= 'f') ) ? 1 : 0;
}

__INLINE__ int fw_ishexupper( char c ) {
  return ( (c >= 'A') && (c <= 'F') ) ? 1 : 0;
}


// Warning: Assumes "force load/store alignment" is off, i.e., misaligned ld/st is ok!
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
    size_t h32  = (n >> 5);
    size_t t8, t1;
    uint64_t *d64 = (uint64_t *)dest;

    while ( h32-- )
    {
        *d64++ = 0;
        *d64++ = 0;
        *d64++ = 0;
        *d64++ = 0;
    }

    if ( (t8 = ((n & 0x18) >> 3)) )
    {
        while ( t8-- )
            *d64++ = 0;
    }

    if ( (t1 = (n & 0x7)) )
    {
        uint8_t *d8 = (uint8_t *)d64;

        while ( t1-- )
            *d8++ = 0;
    }
}



//
// NOTE: See util.h which contains #defines that work better!
//
//
// Misc. utility functions
//

int fwext_strcmp(const char* s1, const char* s2) {

  while (1) {

    if ( *s1 == *s2 ) {

      if (*s1 == 0) {
  return 0;
      }

      s1++;
      s2++;
    }
    else {
      return (*s1 < *s2) ? -1 : 1;
    }
  }
}

int fwext_strncpy(char* to, const char* from, int n) {

  int count = 0;

  while ( (*from != 0) && (count < n) ) {
    *to++ = *from++;
    count++;
  }

  return count;
}

//void __NORETURN Kernel_Crash( int status )
//{
//    FIRMWARE()->terminate( status );
//
//    while (1);
//}

int fflush (FILE *__stream) { return 0; }

void fwext_udelay( uint64_t microseconds ) {
  uint64_t end = GetTimeBase() + ( microseconds * FIRMWARE()->deprecated.personalityPtr->Kernel_Config.FreqMHz );
  while ( GetTimeBase() < end )
    /* spin */
    ;
}

unsigned int
sleep(unsigned int seconds)
{
  fwext_udelay(seconds * 1000000);
  return 0;
}

size_t BgGetRealLockIndex( size_t virtual_index )
{
  if (virtual_index >= FWEXT_LOCK_COUNT)
  {
    printf("FWEXT lock virtual index %ld too large\n", virtual_index);
    Terminate(1000); // terminate with failure
  }
  return BeDRAM_LOCKNUM_FWEXT_FIRST + virtual_index;
}

unsigned int BgGetNumThreads(void)
{
    static volatile uint64_t NumThreads = 0;
    uint64_t v;

    // Reading the thread_active DCR's from lots of threads is hideously
    // expensive.  We read them just once and cache the value.

    v = NumThreads;

    if (v == 0)
    {
  // NumThreads hasn't been computed yet.  See if we should compute it.
  size_t doneIndex = BgGetRealLockIndex(FWEXT_LOCKNUM_NUM_THRD_DONE);
  if (BeDRAM_ReadIncSat(doneIndex) == 0)
  {
      // We won.  Compute NumThreads.
      v = popcnt64(TESTINT_DCR_PRIV_PTR->thread_active0_rb) +
        popcnt64(TESTINT_DCR_PRIV_PTR->thread_active1_rb);
      if ((v <= 0) || (v > 68))
      {
    Terminate(1001);  // Die right away. Should never be zero.
      }
      NumThreads = v;  // Update NumThreads, releasing any threads
           // waiting below.
  }
  else
  {
      // Someone else is computing NumThreads.  Wait for it.
      if (((uint64_t) &NumThreads) >=
        (VA_MINADDR_BEDRAM | VA_PRIVILEGEDOFFSET))
      {
    // We're a bedram extension.  We can't just poll on NumThreads
    // without livelocking the core, so poll via the bedram
    // atomic space.
    do {
        v = BeDRAM_AtomicRead(&NumThreads);
    } while (v == 0);
      }
      else
      {
    // We're in ddr.  Just poll.  NumThreads will be cached in
    // L1 until it's final value is stored.
    do {
        v = NumThreads;
    } while (v == 0);
      }
  }
    }

    return v;
}

void __NORETURN exit( int status ) {

    // Livelock counter detection
    if ( ( status == 0 ) && ( fwext_getenv("BG_LLBCHECK") != 0 ) && ( ! TI_isDD1() ) ) {

  size_t cntIndex = BgGetRealLockIndex(FWEXT_LOCKNUM_TEST_EXIT_CNT);
  int cnt = BeDRAM_ReadIncWrap(cntIndex) + 1;

  while ( cnt < BgGetNumThreads() ) {
      fwext_udelay(1000);
      cnt = BeDRAM_Read(FWEXT_LOCKNUM_TEST_EXIT_CNT);
  }

  if ( ProcessorThreadID() == 0 ) {
//      extern void fwext_check_llb_firs();
//      fwext_check_llb_firs();
  }
    }

    FIRMWARE()->exit( status ); // never returns
    while (1); // shut up the compiler
}

void __NORETURN Terminate( int status )
{
  FIRMWARE()->terminate(status); // never returns
  while (1); // shut up the compiler
}

int putchar(int c)
{
#if !defined(__FUSEDOS__)
   char str[2];
   str[0] = c;
   str[1] = 0;
   int rc = FIRMWARE()->putn( str, 2 );
   return rc;
#else
   return c;
#endif
}

//int puts( const char *s )
//{
//  char buffer[4096+1];
//  int n = fwext_strncpy(buffer, s, sizeof(buffer) - 1);
//  buffer[n] = '\n';
//  buffer[n+1] = 0;
//  int rc = FIRMWARE()->putn( buffer, n+1 );
//
//  if ((n+1) > sizeof(buffer))
//  {
//    (void) FIRMWARE()->putn( "PUTS OVERFLOWED\n", 16 );
//    Terminate(__LINE__);
//  }
//
//  return( rc );
//}

int fprintf(FILE *stream, const char* fmt, ...) { return 0; }
int vfprintf(FILE *stream, const char *fmt, va_list ap) { return 0; }

//int printf( const char *fmt, ... )
//{
//    uint64_t currentTime = GetTimeBase();
//    va_list args;
//    char buffer[4096];
//    char* buffp = buffer;
//    int  embeddedTimestampLength = 0;
//
//    if ( ( FIRMWARE()->deprecated.personalityPtr->Kernel_Config.NodeConfig & PERS_ENABLE_Timestamps ) != 0 ) {
//
//      char ts[16];
//      int  n = 0;
//
//      while ( currentTime != 0 ) {
//  ts[n++] = '0' + ( currentTime % 10 );
//  currentTime /= 10;
//      }
//
//      embeddedTimestampLength = n+1;
//      n--;
//
//      while (n >= 0) {
//  *(buffp++) = ts[n--];
//      }
//
//      *(buffp++) = '|';
//    }
//
//    va_start( args, fmt );
//
//    //DELETEME int len = FIRMWARE()->vprintf( fmt, args );
//    int len = vsprintf( buffp, fmt, args );
//    FIRMWARE()->putn( buffer, len + embeddedTimestampLength );
//
//    va_end(args);
//
//    if ((len + embeddedTimestampLength) > sizeof(buffer))
//    {
//      (void) FIRMWARE()->putn( "PRINTF OVERFLOWED\n", 18 );
//      Terminate(__LINE__);
//    }
//
//    return len;
//}

int sprintf(char* buf, const char *fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    int len = vsprintf( buf, fmt, args );
    va_end( args );
    return len;
}

int writeRASEvent(uint32_t message_id, uint16_t number_of_details, const uint64_t details[])
{
#if !defined(__FUSEDOS__)
  return FIRMWARE()->writeRASEvent(message_id, number_of_details, (fw_uint64_t *)details);
#else
  return 0;
#endif
}

//int Kernel_InjectRAWRAS(uint32_t message_id, size_t raslength, const uint64_t* rasdata) {
//    return writeRASEvent(message_id, raslength, rasdata);
//}

//void *memcpy( void *dest, const void *src, size_t n )
//{
//   uint64_t *s64 = (uint64_t *)src;
//   uint64_t *d64 = (uint64_t *)dest;
//   dcache_block_touch( (void *)s64 );
//   size_t h32  = (n >> 5);
//   size_t t8, t1;
//   register uint64_t ra, rb, rc, rd;
//
//   while ( h32-- )
//      {
//      dcache_block_touch( (void *)(s64 + 4) );
//      ra     = *s64++;
//      rb     = *s64++;
//      rc     = *s64++;
//      rd     = *s64++;
//      *d64++ = ra;
//      *d64++ = rb;
//      *d64++ = rc;
//      *d64++ = rd;
//      }
//
//   if ( (t8 = ((n & 0x18) >> 3)) )
//      {
//      while ( t8-- )
//         *d64++ = *s64++;
//      }
//
//   if ( (t1 = (n & 0x7)) )
//      {
//      uint8_t *s8 = (uint8_t *)s64;
//      uint8_t *d8 = (uint8_t *)d64;
//
//      while ( t1-- )
//         *d8++ = *s8++;
//      }
//
//   return dest;
//}

//void* memset(void* dest, int value, size_t n )
//{
//    if ( !value )
//       {
//       bzero_Inline( dest, n );
//       }
//    else
//       {
//
//   if ( (n > sizeof(uint64_t)) && ( ((uint64_t)dest & 0xF) == 0 ) ) {
//     uint64_t v = (uint64_t)(value & 0xFF);
//     uint64_t fill = (v << 0) | (v << 8) | (v << 16) | (v << 24) | (v<<32) | (v<<40) | (v << 48) | (v << 56);
//     for ( ; n > sizeof(uint64_t) ; dest += sizeof(uint64_t), n -= sizeof(uint64_t) )
//       *((uint64_t*)dest) = fill;
//   }
//
//       uint8_t* d = (uint8_t*)dest;
//       while (n--)
//           *d++ = value;
//       }
//    return dest;
//}

//
// ascii to integer conversion
//
int atoi( const char *str )
{
    int is_neg = 0;
    int value = 0;
    char *p = (char *)str;
    char *q = NULL;

    if ( !p )
        return(0); // terminated string. exit with zero
    while ( fw_isspace(*p) )
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
    while ( fw_isdigit(*q) )
        q++;
    while ( p < q )
    {
        value *= 10;
        value += (*p - '0');
        p++;
    }
    return(is_neg ? (-(value)) : value);
}

//void __NORETURN __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function)
//{
//    printf("assertion failed: file=%s line=%d function=%s  assertion=%s\n", file, line, function, assertion);
//    Terminate(1);
//}

extern uint64_t __KERNEL_END;
static uint64_t _malloc_pointer = (uint64_t)&__KERNEL_END;
//extern uint64_t _malloc_pointer ;


void* malloc(size_t size)
{
   void *ptr = NULL;

   if (vaddrLimit == 0) {
    Firmware_DomainDescriptor_t domain;
    FIRMWARE()->getDomainDescriptor(&domain);
    vaddrLimit = ROQ_VIRTUAL_MAPPING + ((domain.ddrEnd + 1) - domain.ddrOrigin);
    _malloc_pointer |= ROQ_VIRTUAL_MAPPING;
   }

   while(BeDRAM_ReadIncSat(BeDRAM_LOCKNUM_MALLOCLOCK) != 0) /* spin */;
   _malloc_pointer = (_malloc_pointer + 31) & (~0x1f); // 32-byte alignment
   if ((_malloc_pointer + size) <= vaddrLimit)
   {
       ptr = (void *) _malloc_pointer;
       _malloc_pointer += size;
   }
   ppc_msync();
   BeDRAM_ReadClear(BeDRAM_LOCKNUM_MALLOCLOCK);

   // For now at least, terminate the calling program if malloc failed.
   if (ptr == NULL)
   {
#if !defined(__FUSEDOS__)
       (void) FIRMWARE()->putn("MALLOC FAILED - OUT OF MEMORY\n", 30);
#endif
       Terminate(1);
   }

   return ptr;
}

void free(void* ptr)
{
   return;
}

int FWEXT_DATA __gxx_personality_v0;

char FWEXT_ENV_VARS fwext_EnvVars[1024 *4];

char* fwext_nextString(char** bufp) {

  char* result = *bufp;

  if ( **bufp != 0 ) {
    while ( *((*bufp)++) != 0 );
  }

  return result;
}

char* fwext_getenv(const char* key) {

  /* ####################################################
   * For now, we simply perform a linear search of
   * the list
   * #################################################### */

  char* bufp = &(fwext_EnvVars[0]);
  char* name;

  do {
    name = fwext_nextString(&bufp);
    char* value = fwext_nextString(&bufp);

    if ( ! fwext_strcmp(key,name) ) {
      return value;
    }
  } while ( name[0] != 0 );
  return 0;
}


unsigned long fwext_strtoul(const char* s, char** endp, int base) {

  unsigned long result = 0xBADC0DE;

  if (s == 0)
    goto strtoul_out;

  if ( ( base == 0  ) || (base == 10) || (base == 16) ) {

    // Ignore leading white space:
    while ( fw_isspace(*s) ) s++;

    if (base == 0) {

      if ( s[0] == '0' ) {
  if ( ( s[1] == 'x' ) || ( s[1] == 'X' ) ) {
    base = 16;
    s += 2;
  }
  else if ( s[1] == 0 ) { // Just plain "0"
    result = 0;
    goto strtoul_out;
  }
  else
    goto strtoul_out; // base 8 is unsupported
      }
      else {
  base = 10;
      }

    }

    result = 0;

    if (base == 10) {
      while (1) {

  unsigned long digit = 0;

  if ( fw_isdigit(*s) ) {
    digit = *s - '0';
  }
  else {
    goto strtoul_out;
  }

  result = (result*10) + digit;
  s++;
      }
    }
    else if (base == 16) {

      // Leading 0x or 0X is ok:
      if ( ( s[1] == 'x' ) || ( s[1] == 'X' ) ) {
  s += 2;
      }

      while (1) {

  unsigned long nibble = 0;

  if ( fw_isdigit(*s) ) {
    nibble = *s - '0';
  }
  else if ( fw_ishexlower(*s) ) {
    nibble = *s - 'a' + 10;
  }
  else if ( fw_ishexupper(*s) ) {
    nibble = *s - 'A' + 10;
  }
  else {
    goto strtoul_out;
  }

  result = (result << 4) | nibble;
  s++;
      }
    }
  }

  strtoul_out:

    if (endp != 0) {
      *endp = (char*)s;
    }

    return result;
}

unsigned long strtoul(const char* s, char** endp, int base) {
  return fwext_strtoul(s, endp, base);
}

long fwext_strtol(const char* s, char** endp, int base) {

  unsigned long result = 0xBADC0DE;

  if (s == 0)
    goto strtol_out;

  if ( ( base == 0  ) || (base == 10) || (base == 16) ) {

    // Ignore leading white space:
    while ( fw_isspace(*s) ) s++;

    int negative = (s[0] == '-') ? (s++,1) : 0;

    unsigned long value = fwext_strtoul( s, endp, base );

    result = negative ? (0 - (long)value) : value;
  }

  strtol_out:
    return result;
}

long int strtol(const char *S, char **PTR, int BASE)
{
  return fwext_strtoul(S, PTR, BASE);
}

int fwext_termCheck( void ) {
  return FIRMWARE()->termCheck();
}

//Personality_t*
//getPersonality()
//{
//  return FIRMWARE()->deprecated.personalityPtr;
//}

int fwext_get( const char* buffer, unsigned bufferLength ) {

  int rc;

  fw_uint32_t messageType = 0;

  struct {
      MailBoxPayload_Stdin_t header;
      char buffer[1024*4];
  } stage;

  do {
      //rc = FIRMWARE()->get( buffer, bufferLength );
      rc = FIRMWARE()->pollInbox( (void*)&stage, &messageType, sizeof(stage) );
  } while ( ( rc == FW_EAGAIN ) || ( messageType != JMB_CMD2CORE_STDIN ) );

  if ( ( messageType == JMB_CMD2CORE_STDIN ) && ( rc > 0 ) ) {
      if ( rc > bufferLength ) {
    rc = FW_TOO_BIG;
      }
      else {
    memcpy( (void*)buffer, stage.buffer, rc );
      }
  }

  return rc;
}

void FW_Warning( const char* fmt, ... ) {

    char buffer[1024];

   va_list args;
   va_start( args, fmt );

   int len = 0;
   len = vsprintf( buffer, fmt, args );
   va_end( args );

   printf("(W) %s\n", buffer);
}
