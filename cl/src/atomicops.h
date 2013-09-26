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
/* (C) Copyright IBM Corp.  2007, 2012                              */
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

#ifndef ATOMIC_OPS_H_ // Prevent multiple inclusion
#define ATOMIC_OPS_H_


#if !defined(__ASSEMBLY__)

__BEGIN_DECLS

#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/bqc/A2_inlines.h>

//@MG: Note alignment need not be this coarse (32B), but must be >= 8B.
typedef struct T_BGP_Atomic
{
    volatile uint32_t atom;
}
ALIGN_L1D_CACHE Lock_Atomic_t;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
typedef struct T_BGP_Atomic64
{
    volatile uint64_t atom;
}
ALIGN_L1D_CACHE Lock_Atomic64_t;


//
// eg: Lock_Atomic_t my_atom = ATOMIC_INIT( 0 );
//
#define ATOMIC_INIT(val) { (val) }

// Read the current value of an atom.
//   Note: volatile attribute ensures value is not cached in a register.
#define ATOMIC_READ(pvar) (pvar->atom)
#define ATOMIC_WRITE(pvar, value) (pvar->atom = value)

// Returns current value of the atom.
__INLINE__ uint32_t fetch( Lock_Atomic_t *var ) { return var->atom; }

// returns the orginal value of the atom when the atomic addition has succeeded
__INLINE__ uint32_t fetch_and_add( Lock_Atomic_t *var, uint32_t value )
{
    register uint32_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved32( &(var->atom) );

        tmp_val = old_val + value;
    }
    while ( !StoreConditional32( &(var->atom), tmp_val ) );

    return( old_val );
}

// returns the orginal value of the atom when the atomic subtraction has succeeded
__INLINE__ uint32_t fetch_and_sub( Lock_Atomic_t *var, uint32_t value )
{
    register uint32_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved32( &(var->atom) );

        tmp_val = old_val - value;
    }
    while ( !StoreConditional32( &(var->atom), tmp_val ) );

    return( old_val );
}

// Set bit(s): returns the orginal value of the atom when the atomic OR has succeeded
__INLINE__ uint32_t fetch_and_or( Lock_Atomic_t *var, uint32_t mask )
{
    register uint32_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved32( &(var->atom) );

        tmp_val = old_val | mask;
    }
    while ( !StoreConditional32( &(var->atom), tmp_val ) );

    return( old_val );
}


// Clear bit(s): returns the orginal value of the atom when the atomic AND has succeeded
__INLINE__ uint32_t fetch_and_and( Lock_Atomic_t *var, uint32_t mask )
{
    register uint32_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved32( &(var->atom) );

        tmp_val = old_val & mask;
    }
    while ( !StoreConditional32( &(var->atom), tmp_val ) );

    return( old_val );
}


// Test and Set: Atomically OR mask with atom.
//    If any bits of mask are already set in atom, return 0 (FALSE) with atom unchanged.
//    Otherwise return 1 (TRUE) with atom atomically updated.
__INLINE__ int test_and_set( Lock_Atomic_t *var, uint32_t mask )
{
    register uint32_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved32( &(var->atom) );

        if ( old_val & mask )
            return(0);

        tmp_val = old_val | mask;
    }
    while ( !StoreConditional32( &(var->atom), tmp_val ) );

    return( 1 );
}
#endif                                                                                                                 // FUSEDOS


// Compare and Swap:
//   returns:
//      1 = var equals old value, new value atomically stored in var
//      0 = var does not equal old value, and *pOldValue is set to current value.
__INLINE__ int compare_and_swap( Lock_Atomic_t *var,
                                      uint32_t  *pOldValue,
                                      uint32_t  NewValue )
{
    uint32_t tmp_val;

    do
    {
        tmp_val = LoadReserved32( &(var->atom) );
        if ( *pOldValue != tmp_val  )
        {
            *pOldValue = tmp_val;
            return( 0 );
        }
    }
    while ( !StoreConditional32( &(var->atom), NewValue ) );

    return( 1 );
}
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
// Set bit(s): returns the orginal value of the atom when the atomic OR has succeeded
__INLINE__ uint64_t fetch_and_or64( Lock_Atomic64_t *var, uint64_t mask )
{
    register uint64_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved( &(var->atom) );

        tmp_val = old_val | mask;
    }
    while ( !StoreConditional( &(var->atom), tmp_val ) );

    return( old_val );
}


// Clear bit(s): returns the orginal value of the atom when the atomic AND has succeeded
__INLINE__ uint64_t fetch_and_and64( Lock_Atomic64_t *var, uint64_t mask )
{
    register uint64_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved( &(var->atom) );

        tmp_val = old_val & mask;
    }
    while ( !StoreConditional( &(var->atom), tmp_val ) );

    return( old_val );
}

// returns the orginal value of the atom when the atomic addition has succeeded
__INLINE__ uint64_t fetch_and_add64( Lock_Atomic64_t *var, uint64_t value )
{
    register uint64_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved( &(var->atom) );

        tmp_val = old_val + value;
    }
    while ( !StoreConditional( &(var->atom), tmp_val ) );

    return( old_val );
}

// returns the orginal value of the atom when the atomic subtraction has succeeded
__INLINE__ uint32_t fetch_and_sub64( Lock_Atomic64_t *var, uint64_t value )
{
    register uint64_t old_val, tmp_val;

    ppc_msync();

    do
    {
        old_val = LoadReserved( &(var->atom) );

        tmp_val = old_val - value;
    }
    while ( !StoreConditional( &(var->atom), tmp_val ) );

    return( old_val );
}


#endif                                                                                                                 // FUSEDOS

__END_DECLS

#endif




#endif // Add nothing below this line
