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
#ifndef _A2_INLINES_H_ // Prevent multiple inclusion
#define _A2_INLINES_H_

#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/A2_core.h>

#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <stdint.h>
#endif                                                                                                                 // FUSEDOS

__BEGIN_DECLS

#ifndef __UPCFS__                                                                                                      // FUSEDOS
//
// Functions below are non-privileged.
//

// Warning: Most SPRs are privileged.
// Note: Must be a define because SPRN is encoded in the instruction.
#define mfspr( SPRN )\
({\
   uint64_t tmp;\
   asm volatile ("mfspr %0,%1" : "=&r" (tmp) : "i" (SPRN) );\
   tmp;\
})


// Warning: Most SPRs are privileged.
// Note: Must be a define because SPRN is encoded in the instruction.
#define mtspr( SPRN, value ) do { asm volatile ("mtspr %0,%1": : "i" (SPRN), "r" (value) ); } while(0)


__INLINE__ void isync(void) { asm volatile ("isync" : : : "memory"); }

// A2 UM V0.83 2.14.3 "...the A2 core implements them (msync and mbar) in identical fashion."
__INLINE__ void ppc_msync(void) { asm volatile ("msync" : : : "memory"); }
#endif                                                                                                                 // FUSEDOS
__INLINE__ void mbar(void)  { asm volatile ("mbar"  : : : "memory"); }
#ifndef __UPCFS__                                                                                                      // FUSEDOS

// regular wait instruction and with bit 9 and 10 set to allow additional
// wakeup options
__INLINE__ void ppc_wait(void)  { asm volatile ("wait"); }
__INLINE__ void ppc_waitimpl(void)   { asm volatile ("waitimpl"); }
__INLINE__ void ppc_waitrsv(void)  { asm volatile ("waitrsv"); }
// Removing this one -- the BGQ toolchain does not have this opcode, it does not appear in any instruction document, and 
// Martin says it is not supported. 
//
//__INLINE__ void ppc_waitimplrsv(void){ asm volatile (".int 0x7C60007C"); }
#define bgq_wait_bit9    ppc_waitimpl     //! Obsolete
#define bgq_wait_bit10   ppc_waitrsv      //! Obsolete
#define bgq_wait_bit9_10 ppc_waitimplrsv  //! Obsolete

#define CurrentInstAddress()  ({uint64_t tmp; asm volatile("bl 4; mflr %0" : "=r" (tmp) : : "lr"); tmp; })
#define CurrentStackAddress() ({uint64_t tmp; asm volatile("mr %0,1": "=r" (tmp)) ;tmp;})
#define CurrentTOCAddress()   ({uint64_t tmp; asm volatile("mr %0,2": "=r" (tmp)) ;tmp;})

// This routine needs calibration. Currently, it will delay this number of
//  instruction execution slots for the calling thread.
__INLINE__ void Delay( uint64_t pclks )
{
   while( pclks-- )
      {
      asm volatile("nop;");
      }
}


__INLINE__ void Delay_ns( uint64_t nsecs, uint64_t core_freq )
{
  uint64_t pclks = (((int)(nsecs*16*1024/1000)*core_freq)>>14)+1;
  //printf("Called _bgp_Delay_ns with %Li nsecs, converted to %Li pclks\n",nsecs,pclks);
  Delay(pclks);
}


// mbar'd volatile 32bit write (volatile store that avoids write-combining)
__INLINE__ void out32_sync( uint32_t *vaddr, uint32_t value )
{
   // left-side casts are deprecated by gcc, so define a new variable
   volatile uint32_t *va = (volatile uint32_t *)vaddr;

   *va = (value);

   mbar(); // expensive
}


// volatile 32bit write
__INLINE__ void out32( uint32_t *vaddr, uint32_t value )
{
   // left-side casts are deprecated by gcc, so define a new variable
   volatile uint32_t *va = (volatile uint32_t *)vaddr;

   *va = (value);
}


// mbar'd volatile 64bit write (volatile store that avoids write-combining)
__INLINE__ void out64_sync( uint64_t *vaddr, uint64_t value )
{
   // left-side casts are deprecated by gcc, so define a new variable
   volatile uint64_t *va = (volatile uint64_t *)vaddr;

   *va = (value);

   mbar(); // expensive
}


// volatile 64bit write
__INLINE__ void out64( uint64_t *vaddr, uint64_t value )
{
   // left-side casts are deprecated by gcc, so define a new variable
   volatile uint64_t *va = (volatile uint64_t *)vaddr;

   *va = (value);
}


// mbar'd volatile 32bit read
__INLINE__ uint32_t in32_sync( uint32_t *vaddr )
{
   volatile uint32_t *va = (volatile uint32_t *)vaddr;

   mbar(); // expensive

   return( *va );
}


// volatile 32bit read
__INLINE__ uint32_t in32( uint32_t *vaddr )
{
   volatile uint32_t *va = (volatile uint32_t *)vaddr;

   return( *va );
}


// mbar'd volatile 64bit read
__INLINE__ uint64_t in64_sync( uint64_t *vaddr )
{
   volatile uint64_t *va = (volatile uint64_t *)vaddr;

   mbar(); // expensive

   return( *va );
}


// volatile 64bit read
__INLINE__ uint64_t in64( uint64_t *vaddr )
{
   volatile uint64_t *va = (volatile uint64_t *)vaddr;

   return( *va );
}

#endif                                                                                                                 // FUSEDOS
// Load Reserved: 32bit atom
__INLINE__ uint32_t LoadReserved32( volatile uint32_t *pVar )
{
   register uint32_t Val;
   asm volatile ("lwarx   %[rc],0,%[pVar];"
		 : [rc] "=&b" (Val)
		 : [pVar] "b" (pVar));
   return( Val );
}


// Load Reserved: 64bit atom
__INLINE__ uint64_t LoadReserved( volatile uint64_t *pVar )
{
   register uint64_t Val;
   asm volatile ("ldarx   %[rc],0,%[pVar];"
		 : [rc] "=&b" (Val)
		 : [pVar] "b" (pVar));
   return( Val );
}


// Store Conditional: 32bit atom
//   Returns: 0 = Conditional Store Failed,
//            1 = Conditional Store Succeeded.
__INLINE__ int StoreConditional32( volatile uint32_t *pVar, uint32_t Val )
{
   register int rc = 1; // assume success
   asm volatile ("  stwcx.  %2,0,%1;"
		 "  beq     1f;"       // conditional store succeeded
		 "  li      %0,0;"
		 "1:;"
		 : "=b" (rc)
		 : "b"  (pVar),
		   "b"  (Val),
		   "0"  (rc)
		 : "cc", "memory" );
   return( rc );
}
#ifndef __UPCFS__                                                                                                      // FUSEDOS


// Store Conditional: 64bit atom
//   Returns: 0 = Conditional Store Failed,
//            1 = Conditional Store Succeeded.
__INLINE__ int StoreConditional( volatile uint64_t *pVar, uint64_t Val )
{
   register int rc = 1; // assume success
   asm volatile ("  stdcx.  %2,0,%1;"
		 "  beq     1f;"       // conditional store succeeded
		 "  li      %0,0;"
		 "1:;"
		 : "=b" (rc)
		 : "b"  (pVar),
		   "b"  (Val),
		   "0"  (rc)
		 : "cc", "memory" );
   return( rc );
}


// Compare and Swap: 32bit atomic operation
//   returns:
//      1 = var equals old value, new value atomically stored in var
//      0 = var does not equal old value, and *pOldValue is set to current value.
__INLINE__ int Compare_and_Swap32( volatile uint32_t *var,
                                    uint32_t  *pOldValue,
                                    uint32_t  NewValue )
{
    uint32_t tmp_val;

    do {
       tmp_val = LoadReserved32( var );
       if ( *pOldValue != tmp_val  )
          {
          *pOldValue = tmp_val;
          return( 0 );
          }
       }
       while( !StoreConditional32( var, NewValue ) );

    return( 1 );
}


// Compare and Swap: 64bit atomic operation
//   returns:
//      1 = var equals old value, new value atomically stored in var
//      0 = var does not equal old value, and *pOldValue is set to current value.
__INLINE__ int Compare_and_Swap( uint64_t *var,
                                  uint64_t  *pOldValue,
                                  uint64_t  NewValue )
{
    uint64_t tmp_val;

    do {
       tmp_val = LoadReserved( var );
       if ( *pOldValue != tmp_val  )
          {
          *pOldValue = tmp_val;
          return( 0 );
          }
       }
       while( !StoreConditional( var, NewValue ) );

    return( 1 );
}

// returns the orginal value of the atom when the atomic addition has succeeded
__INLINE__ uint64_t Fetch_and_Add( uint64_t *pVar, uint64_t value )
{
    register uint64_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved( pVar );

        tmp_val = old_val + value;
    }
    while ( !StoreConditional( pVar, tmp_val ) );

    return( old_val );
}

// count leading zeros in a 32bit word
__INLINE__ uint64_t cntlz32( uint32_t v )
{
    uint64_t rval;
    asm volatile ("cntlzw %0,%1" : "=r" (rval) : "r" (v) );
    return( rval );
}

// count leading zeros in a 64bit word
__INLINE__ uint64_t cntlz64( uint64_t v )
{
    uint64_t rval;
    asm volatile ("cntlzd %0,%1" : "=r" (rval) : "r" (v) );
    return( rval );
}

// count non-zero bits in a 64bit word
__INLINE__ uint64_t popcnt64( uint64_t v )
{
    int rval;
    asm volatile ("popcntd %0,%1" : "=r" (rval) : "r" (v) );
    return( rval );
}

// Rotate left double word and clear left
// Must be a macro rather than an inline because of the constant operands.
// Note: modifies initial argument (ra).
#define RLDCL64(ra,rs,rb,mb) \
asm("rldcl  %0,%2,%3,%4" \
: "=r" (ra) : "0" (ra), "r" (rs), "r" (rb), "i" (mb))


// rotate left and insert with mask
// Must be a macro rather than an inline because of the constant operands.
// Note: modifies initial argument (ra).
#define RLIMI64(ra,rs,sh,mb) \
    asm("rldimi %0,%2,%3,%4" \
	: "=r" (ra) : "0" (ra), "r" (rs), "i" (sh), "i" (mb))

// Non-Privileged!
#endif                                                                                                                 // FUSEDOS
__INLINE__ uint64_t GetTimeBase( void )
{
    return mfspr( SPRN_TBRO );         // 64-bit timebase
}
#ifndef __UPCFS__                                                                                                      // FUSEDOS

__INLINE__ void DelayTimeBase( uint64_t tb_delta )
{
    uint64_t now = GetTimeBase();
    while (GetTimeBase() < (now + tb_delta)) /*spin*/;
}

// The following interfaces Must be macros rather than inlines because
// of the constant operands.

// Data-cache-block-flush, with cache-level indicator
#define DCBF_CT(vaddr,ct) \
    do { asm volatile ("dcbf 0,%0,%1" : : "b" (vaddr), "i" (ct)); } while(0)


// Data-cache-block-touch, with cache-level indicator
#define DCBT_CT(vaddr,ct) \
    do { asm volatile ("dcbt %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Data-cache-block-touch-for-store, with cache-level indicator
#define DCBTST_CT(vaddr,ct) \
    do { asm volatile ("dcbtst %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Data-cache-block-touch-and-lock-set, with cache-level indicator
#define DCBTLS_CT(vaddr,ct) \
    do { asm volatile ("dcbtls %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Data-cache-block-touch-for-store-and-lock-set, with cache-level indicator
#define DCBTSTLS_CT(vaddr,ct) \
    do { asm volatile ("dcbtstls %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Data-cache-block-lock-clear, with cache-level indicator
#define DCBLC_CT(vaddr,ct) \
    do { asm volatile ("dcblc %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Instruction-cache-block-touch, with cache-level indicator
#define ICBT_CT(vaddr,ct) \
    do { asm volatile ("icbt %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Instruction-cache-block-touch-and-lock-set, with cache-level indicator
#define ICBTLS_CT(vaddr,ct) \
    do { asm volatile ("icbtls %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

// Instruction-cache-block-lock-clear, with cache-level indicator
#define ICBLC_CT(vaddr,ct) \
    do { asm volatile ("icblc %1,0,%0" : : "b" (vaddr), "i" (ct)); } while(0)

__INLINE__ void dcache_block_touch( void *vaddr )
{
    DCBT_CT(vaddr, 0);
}

__INLINE__ void icache_block_touch( void *vaddr )
{
    ICBT_CT(vaddr, 0);
}

// Data-cache-block-touch at l2 level
__INLINE__ void dcbt_l2( void *vaddr )
{
    DCBT_CT(vaddr, 2);
}

// Data-cache-block-zero
__INLINE__ void dcache_block_zero( void *vaddr )
{
    asm volatile ("dcbz 0,%0" : : "b" (vaddr) : "memory");
}

// Data-cache-block-invalidate
__INLINE__ void dcache_block_inval( void *vaddr )
{
    asm volatile ("dcbi 0,%0" : : "b" (vaddr) : "memory");
}

// Instruction-cache-block-invalidate
__INLINE__ void icache_block_inval( void *vaddr )
{
    asm volatile ("icbi 0,%0" : : "b" (vaddr) : "memory");
}


#endif                                                                                                                 // FUSEDOS
//
// See A2 UM V0.83 Setion 2.4.1.3
//
// In the A2 Core, one has a choice of threads competing fairly for instruction issue slots, or
//   enabling threads to skip issue slots, thereby becoming disfavored or lower priority.
//
// The priority can be changed in user-space via special forms of no-op instructions.
// This call only affects the calling h/w thread.
//
__INLINE__ void ThreadPriority_Medium( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 2,2,2" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ void ThreadPriority_MediumLow( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 6,6,6" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ void ThreadPriority_Low( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 1,1,1" : : );
#endif                                                                                                                 // FUSEDOS
}

#define Kernel_ThreadPriority_High   ThreadPriority_Medium     //! Obsolete
#define Kernel_ThreadPriority_Medium ThreadPriority_MediumLow  //! Obsolete
#define Kernel_ThreadPriority_Low    ThreadPriority_Low        //! Obsolete
#define Kernel_GetTimeBase           GetTimeBase               //! Obsolete
#ifndef __UPCFS__                                                                                                      // FUSEDOS


#if defined(__KERNEL__) // Functions below are always privileged.

__INLINE__ void ThreadPriority_VeryHigh( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 7,7,7" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ void ThreadPriority_High( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 3,3,3" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ void ThreadPriority_MediumHigh( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 5,5,5" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ void ThreadPriority_VeryLow( void )
{
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
  asm volatile ( "or 31,31,31" : : );
#endif                                                                                                                 // FUSEDOS
}

__INLINE__ uint64_t mfmsr( void )
{
   uint64_t tmp;
   asm volatile ("mfmsr %0": "=&r" (tmp) : );
   return( tmp );
}

__INLINE__ void mtmsr( uint64_t value )
{
   asm volatile ("mtmsr %0": : "r" (value) : "memory" );
}

__INLINE__ void rfi( void )
{
   asm volatile ("rfi" : : : "memory" );
}


// Flash-invalidate the A2 data cache.  Technically, this needs to be followed by an msync.  
// use CT=2, flushes L1P as well
__INLINE__ void dci( void )     { asm volatile ("dci 2" : : : "memory"); }


// Flash invalidate the A2 instruction cache.  Technically, this needs to be followed by an isync.  
// use CT=2, flushes L1P as well
__INLINE__ void ici( void )     { asm volatile ("ici 2" : : : "memory"); }


__INLINE__ void Kernel_ClearTimeBase()
{
    mtspr(SPRN_TBLWO, 0);    // ensure no roll over while updating
    mtspr(SPRN_TBUWO, 0);
}

__INLINE__ void Kernel_SetTimeBase( uint64_t pclks )
{
   union {
         uint32_t ul[2];
         uint64_t ull;
         }
         hack;

   hack.ull = pclks;

   uint64_t old_msr = mfmsr();      // ensure no interrupts during update
   mtmsr( MSR_KERNEL_DISABLED );
   isync();                         // ensure msr update takes effect
   mtspr( SPRN_TBLWO, 0 );          // ensure no roll over while updating
   mtspr( SPRN_TBUWO, hack.ul[0] );
   mtspr( SPRN_TBLWO, hack.ul[1] );
   mtmsr( old_msr );                // restore original msr
   isync();                         // ensure msr update takes effect
}


#if defined __FUSEDOS__                                                                                                // FUSEDOS
#include <pthread.h>                                                                                                   // FUSEDOS
extern pthread_key_t spc_key;                                                                                          // FUSEDOS
#endif                                                                                                                 // FUSEDOS

// @brief Returns the implicit core ID (PIR[CID]).  In Blue Gene Q, this is a number between
//        0 and 16, inclusive.
__INLINE__ int ProcessorCoreID( void )
{
#if defined __FUSEDOS__                                                                                                // FUSEDOS
   int* spc = (int*)(pthread_getspecific(spc_key));                                                                    // FUSEDOS
   return (*spc / 4);                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
   uint64_t pir = mfspr( SPRN_PIR );

   return( (int)((pir & 0x3FC) >> 2) );
#endif                                                                                                                 // FUSEDOS
}

// @brief Returns the implicit thread ID within the current core (PIR[TID]).  In
//        Blue Gene Q, this is a number between 0 and 3 inclusive.
__INLINE__ int ProcessorThreadID( void )
{
#if defined __FUSEDOS__                                                                                                // FUSEDOS
   int* spc = (int*)(pthread_getspecific(spc_key));                                                                    // FUSEDOS
   return (*spc % 4);                                                                                                  // FUSEDOS
#else                                                                                                                  // FUSEDOS
   uint64_t pir = mfspr( SPRN_PIR );

   return( (int)(pir & 0x3) );
#endif                                                                                                                 // FUSEDOS
}

// @brief Returns the implicit thread ID within the node.  In Blue Gene Q, this
//        is a number between 0 and 67, inclusive.
__INLINE__ int ProcessorID( void )
{
#if defined __FUSEDOS__                                                                                                // FUSEDOS
   int* spc = (int*)(pthread_getspecific(spc_key));                                                                    // FUSEDOS
   return (*spc);                                                                                                      // FUSEDOS
#else                                                                                                                  // FUSEDOS
   uint64_t pir = mfspr( SPRN_PIR );

   return( (int)(pir & 0x3FF) );
#endif                                                                                                                 // FUSEDOS
}


#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// read my CPU ID (0 to 16) from SPRN_PIR[ PIR_CID ]
// @deprecated Use ProcessorCoreID instead.
__INLINE__ int PhysicalProcessorID( void )
{
  return ProcessorCoreID();
}
#endif                                                                                                                 // FUSEDOS

// read my Thread ID (0 to 3) from SPRN_PIR[ PIR_TID ]
// @deprecated Use ProcessorThreadID instead.
__INLINE__ int PhysicalThreadID( void )
{
  return ProcessorThreadID();
}

// read my Thread Index (0 to 67) from SPRN_PIR
// @deprecated  Use ProcessorID() instead.
__INLINE__ int PhysicalThreadIndex( void )
{
  return ProcessorID();
}


// read the A2 Processor Version
__INLINE__ uint64_t ProcessorVersion( void )
{
   return( mfspr(SPRN_PVR) );
}

// check for DD1 version of BQC chip
__INLINE__ uint64_t A2_isDD1( void )
{
  return (ProcessorVersion() & 0x200) == 0;
}

// check for DD2 version of BQC chip
__INLINE__ uint64_t A2_isDD2( void )
{
  return (ProcessorVersion() & 0x200) != 0;
}

// Write complete ERAT entry
__INLINE__ void eratwe(uint64_t index, uint64_t w0, uint64_t w1, uint64_t mmucr)
{
    mtspr(SPRN_MMUCR0, mmucr);
    isync(); // wait for context to be updated
    asm volatile ("eratwe %2, %0, 1;"
                  "eratwe %1, %0, 0;"
                  : 
                  : "r"  (index),
                    "r"  (w0),
                    "r"  (w1)
                  : "cc", "memory" );
}

// Set the ERAT LRU watermark
__INLINE__ void eratwatermark(uint64_t index, uint64_t mmucr)
{
    mtspr(SPRN_MMUCR0, mmucr);
    isync(); // wait for context to be updated
    asm volatile ("eratwe 0, %0, 3;"
                  : 
                  : "r"  (index)
                  : "cc", "memory" );
}


// Read complete ERAT entry
__INLINE__ void eratre(uint64_t index, uint64_t* w0, uint64_t* w1, uint64_t mmucr)
{
    uint64_t w0c, w1c;
    mtspr(SPRN_MMUCR0, mmucr);
    isync(); // wait for context to be updated
    asm volatile ("eratre %1, %2, 1;"
                  "eratre %0, %2, 0;"
                  : "=&r"  (w0c),
                    "=&r"  (w1c)
                  : "r"  (index)
                  : "cc", "memory" );
    *w0 = w0c;
    *w1 = w1c;
}

__INLINE__ uint64_t eratsx(uint64_t ea, uint64_t mmucr)
{
    uint64_t index = 0;
    mtspr(SPRN_MMUCR0, mmucr);
    isync(); // wait for context to be updated
    asm volatile ("eratsx %0, 0, %1;"
                  : "=&r" (index)
                  : "r"  (ea)
                  : "cc", "memory" );
    return index;
}

// Invalidate an ERAT entry
__INLINE__ void eratilx(void* ea, uint64_t mmucr)
{     
    mtspr(SPRN_MMUCR0, mmucr);
    isync();
    asm volatile ( "eratilx 3, 0, %0;" 
                   : 
                   : "r" (ea) 
                   : "memory" );
}


// Write a TLB entry

__INLINE__ void tlbwe(uint64_t mas1, uint64_t mas2, uint64_t mas7_3, uint64_t mas8, uint64_t mmucr3) {

  mtspr(SPRN_MAS0,
	MAS0_ATSEL_TLB  |  // Select TLB (versus LRAT)
	MAS0_HES_HW_LRU |  // Use LRU replacement algorithm (versus specified hash alias index)
	MAS0_WQ_ALWAYS  ); // Write the entry (no reservation required)
  mtspr(SPRN_MAS1,      mas1);
  mtspr(SPRN_MAS2,      mas2);
  mtspr(SPRN_MAS7_MAS3, mas7_3);
  mtspr(SPRN_MAS8,      mas8);
  mtspr(SPRN_MMUCR3,    mmucr3);
  isync(); // wait for context to be updated
  asm volatile ( "tlbwe;" : : : "memory" );
}

// Write a TLB entry slot
__INLINE__ void tlbwe_slot(uint64_t slot, uint64_t mas1, uint64_t mas2, uint64_t mas7_3, uint64_t mas8, uint64_t mmucr3) {
    
    mtspr(SPRN_MAS0,
          MAS0_ATSEL_TLB  |  // Select TLB (versus LRAT)
          MAS0_HES_ESEL   |  // Use specified slot rather than LRU replacement
          MAS0_ESEL(slot%4) | 
          MAS0_WQ_ALWAYS  ); // Write the entry (no reservation required)
    
    mtspr(SPRN_MAS1,      mas1);
    mtspr(SPRN_MAS2,      mas2);
    mtspr(SPRN_MAS7_MAS3, mas7_3);
    mtspr(SPRN_MAS8,      mas8);
    mtspr(SPRN_MMUCR3,    mmucr3);
    isync(); // wait for context to be updated
    
    asm volatile ( "tlbwe;" : : : "memory" );
}


// Read a TLB entry
__INLINE__ void tlbre(uint64_t index, uint64_t* mas1, uint64_t* mas2, uint64_t* mas7_3, uint64_t* mas8, uint64_t* mmucr3) {
    
    mtspr(SPRN_MAS0,
          MAS0_ATSEL_TLB     |  // Select TLB (versus LRAT)
          MAS0_HES_ESEL      |  // Use LRU replacement algorithm (versus specified hash alias index)
          MAS0_ESEL(index%4) |  // Select WAY of MMU
          MAS0_WQ_ALWAYS  ); // Write the entry (no reservation required)
    isync(); // wait for context to be updated
    
    asm volatile ( "tlbre;" : : : "memory" );
    
    *mas1   = mfspr(SPRN_MAS1);
    *mas2   = mfspr(SPRN_MAS2);
    *mas7_3 = mfspr(SPRN_MAS7_MAS3);
    *mas8   = mfspr(SPRN_MAS8);
    *mmucr3 = mfspr(SPRN_MMUCR3);    
}

// Search for a TLB entry
__INLINE__ uint64_t tlbsx(uint64_t ea, uint64_t mas5, uint64_t mas6)
{
    mtspr(SPRN_MAS5, mas5);
    mtspr(SPRN_MAS6, mas6);
    isync(); // wait for context to be updated
    asm volatile ("tlbsx 0, %0" : : "r" (ea) );
    isync();
    return ((mfspr(SPRN_MAS1) & MAS1_V(1)) == 0) ? -1 : mfspr(SPRN_MAS0);
}

// Invalidate a TLB entry
__INLINE__ void tlbilx(void* virtualaddress, uint64_t mas6)
{     
    mtspr(SPRN_MAS6,      mas6);
    isync(); // wait for context to be updated
    asm volatile ( "tlbilx 3, 0, %0;" : : "r" (virtualaddress) : "memory" );
}


#endif // __KERNEL__

#endif                                                                                                                 // FUSEDOS
__END_DECLS

#endif // Add nothing below this line.
