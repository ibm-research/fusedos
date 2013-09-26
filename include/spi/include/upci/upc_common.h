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

//! \addtogroup upci_spi
//@{
/**
 * \file
 *
 * \brief Common and Global structures and definitions for UPCI operations.
 */
//@}

#ifndef _UPC_COMMON_H_
#define _UPC_COMMON_H_

#ifndef __UPCFS__                                                                                                      // FUSEDOS
#include <assert.h>
#include <memory.h>
#endif                                                                                                                 // FUSEDOS
#include "hwi/include/bqc/testint_inlines.h"
#include "spi/include/kernel/location.h"

//#if defined( __KERNEL__ ) // Work around missing personality spi for kernel
//extern void FillPersonalityWA(Personality_t *p);
//#else
//void FillPersonalityWA(Personality_t *p) {
//    Kernel_GetPersonality(p, sizeof(Personality_t));
//}
//#endif

//! UPC Common types and definitions

__BEGIN_DECLS


typedef ushort UpciBool_t;     //!< if it doesn't already exist, create a boolean type
#ifndef UpciTrue
#define UpciTrue 1
#define UpciFalse 0
#endif

#ifndef UPCI_MIN
#define UPCI_MIN(a,b) ( ( (a) > (b) ) ? (b) : (a) )
#define UPCI_MAX(a,b) ( ( (a) > (b) ) ? (a) : (b) )
#endif

// Define gymnastics to create a compile time AT string.
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define _AT_ __FILE__ ":" TOSTRING(__LINE__)


//! UPCI Syscall Configuration Version
//! \warning Increase value if there are major Punit configuration changes or if the processing
//! logic has changed which must remain common between the kernel usage and the user state application
//! usage.  The syscalls which reference the punit config object will verify that this value is the
//! same to insure that the CNK revision and the version compiled or linked into the user application
//! are compatible.
//! Versions
//! -  0x00020004  V1R1M0 GA
//! -  0x00020005  bgpm-io-mux-overflows - changes need updated cnk image as well to work properly
#define UPCI_CFG_VERSION     0x00020005    //!< Current config structure version
                                           //!< increase if major structure changes or logic between kernel
                                           //!< bgpm/upci API is required to be consistent for get correct operation
                                           //!< (just in case there is new driver and app hasn't been recompiled).


//! UPC_Assert
//! always assert even when NDEBUG is set.
#ifdef NDEBUG
extern void __assert_fail (__const char *__assertion, __const char *__file,
                           unsigned int __line, __const char *__function)
                          __THROW __attribute__ ((__noreturn__));
#endif
#define UPC_Assert(expr)   \
  ((expr) ? 0 : (__assert_fail (__STRING(expr), __FILE__, __LINE__, __func__), 0))



//! Is_Pers_Enabled
//! A macro to test if particular personality is enabled
__INLINE__ int Is_Pers_Enabled(uint64_t mask)
{
    Personality_t personality;
    Kernel_GetPersonality(&personality, sizeof(Personality_t));
    //FillPersonalityWA(&personality);
    return (personality.Kernel_Config.NodeConfig & mask);
}




//! Unit Status
typedef uint32_t Upci_UnitStatus_t;
#define UPCI_UnitStatus_Running  0x00000001   //!< Counters actively running
#define UPCI_UnitStatus_Applied  0x00000002   //!< Unit Event config is applied to hardware threads



__END_DECLS

#endif


