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

#ifndef	_KERNEL_SPEC_H_ /* Prevent multiple inclusion */
#define	_KERNEL_SPEC_H_

/*!
 * \file spec.h
 *
 * \brief C Header File containing SPI speculative and transactional memory functions
 *
 */


#include "kernel_impl.h" 

#include "hwi/include/common/compiler_support.h"
#include "hwi/include/bqc/l2_central_mmio.h"
#include "hwi/include/bqc/l2_dcr.h"

__BEGIN_DECLS

#ifndef __INLINE__
#define __INLINE__ extern inline
#endif

/*!
  \brief Flags provided by L2 that indicate various aspects of a speculation conflict
*/
typedef struct SpecConflictSliceInfo_s
{
    uint64_t was_conflictCaptured     : 1; //!< Conflict has occurred and this structure contains valid data
    uint64_t was_writeAfterRead       : 1; //!< Conflict occurred on a write operation that was preceded by a read operation
    uint64_t was_nonSpeculativeAccess : 1; //!< Conflict was due to a non-speculative write and a speculative thread had the old contents
    uint64_t conflict_vaddress_valid  : 1; //!< Conflicts virtual address (conflict_vaddress) is valid
    uint64_t specid_original          : 7; //!< Speculation ID
    uint64_t specid_violating         : 7; //!< Speculation ID
    uint64_t conflict_vaddress;            //!< Virtual address of the conflicting operation's address
    uint64_t conflict_paddress;            //!< Physical address of the conflicting operation's address
}
SpecConflictSliceInfo_t;

/*!
  \brief Structure containing conflict state for each L2 slice
*/
typedef struct SpecConflictInfo_t
{
    SpecConflictSliceInfo_t l2slice[L2_DCR_num];
}
SpecConflictInfo_t;

/*!
  \brief Speculative context data structure.  
*/
typedef struct SpecContext_s
{
    uint64_t priority_key; //!< Priority key to determine winner in thread conflicts
    uint64_t atn;          //!< Allocation token for SE.  \deprecated
    int32_t  *at;          //!< Pointer to the allocation token for SE.  \deprecated
    uint64_t *lctn;        //!< Pointer to the lowest conflicting thread number for SE  \deprecated
    uint64_t hwthreadid;   //!< Hardware thread ID that pertains to this context structure
    uint64_t padding[11];  //!< Padding for alignment and potential changes in size of the data structure
}
SpecContext_t;

/*!
  \brief Speculation thread rollback state
*/
typedef struct SpecThread_s
{
    uint64_t specid; //!< original speculation ID for the thread
    uint64_t ip;     //!< Address of the first instruction of the rollback handler
    uint64_t gpr1;   //!< Value of GPR1 when the rollback handler is called.  PowerPC ABI defines GPR1 as the stack pointer.
    uint64_t gpr2;   //!< Value of GPR2 when the rollback handler is called.  PowerPC ABI defines GPR2 as the table of contents pointer.
    uint64_t padding[12]; //!< Padding for alignment and potential changes in size of the data structure
}
SpecThread_t;

/*! 
  \brief Speculation state structure
*/

typedef struct SpecState_s
{
    SpecThread_t  hwt_state[68]; //!< SpecThread structure for each hardware thread
    SpecContext_t id_state[128]; //!< SpecContext structure for each speculation ID
}
SpecState_t;

// Conflict register value is not included in the return code if INVALID is set
#define Kernel_SpecReturnCode_INVALID           _BN(50)               //!< Conflict occurred but there is no conflict information available.
#define Kernel_SpecReturnCode_SINGLE            L2C_CONFLICT_SINGLE   //!< Conflict with 1 speculative ID occurred
#define Kernel_SpecReturnCode_NONSPEC           L2C_CONFLICT_NONSPEC  //!< Conflict with a non-speculative ID occurred
#define Kernel_SpecReturnCode_MULTI             L2C_CONFLICT_MULTI    //!< Conflict with multiple speculative IDs
#define Kernel_SpecReturnCode_RESOURCE          L2C_CONFLICT_RESOURCE //!< L2 was not able to hold more speculative state
#define Kernel_SpecReturnCode_KEY               _BN(47)               //!< \deprecated
#define Kernel_SpecReturnCode_JMV               _BN(48)               //!< Kernel jail mode violation
#define Kernel_SpecReturnCode_SEConflict        _BN(51)               //!< Conflict occurred with this thread in SE mode
#define Kernel_SpecReturnCode_SEConflictVictim  _BN(49)               //!< Speculation conflict (SE victim of older thread)

/*!
  \brief Sets the speculation context data structure
  
  Set the speculation context structure for the process.
  \return Error indication
  \retval  0 Success
  \retval -1 Invalid parameters
*/

__INLINE__
uint32_t Kernel_SetSpecContext(size_t specContextSize, const SpecContext_t* specContextBasePtr);

/*!
  \brief Sets the speculative thread data structure.
  
  \deprecated use Kernel_SetSpecContext
  \return Error indication
  \retval  0 Success
  \retval -1 Invalid parameters
*/
__INLINE__
uint32_t Kernel_SetSpecThreadMap(size_t specThreadSize, const SpecThread_t* specThreadBasePtr);

/*!
  \brief Sets the speculative state data structure
  
  \deprecated 
  \see Kernel_SetSpecContext
  \return Error indication
  \retval  0 Success
  \retval -1 Invalid parameters
*/
__INLINE__
uint32_t Kernel_SetSpecState(size_t specStateSize, const SpecState_t* specStatePtr);

/*!
  \brief Put thread in speculation jail mode
  
  \param[in] isLongRunningSpeculation boolean indicating whether the thread has been put into long-running speculation
  \return Error indication
  \retval  0 Success
  \retval -1 Invalid parameters
*/
__INLINE__
uint32_t Kernel_EnterSpecJailMode(int isLongRunningSpeculation);

/*!
  \brief Take thread out of speculation jail mode
  \return Error indication
  \retval  0 Success
  \retval -1 Invalid parameters
*/
__INLINE__
uint32_t Kernel_ExitSpecJailMode(void);

/*!
  \brief Allocates a speculation domain
  
  \param[out] domain Returns the domain ID that was allocated
*/
__INLINE__
uint32_t Kernel_AllocateSpecDomain(unsigned int* domain);

/*!
  \brief Sets the total number of speculation domains needed on the node
  
  \param[in] new_domain_count number of domains
*/
__INLINE__
uint32_t Kernel_SetNumSpecDomains(uint32_t new_domain_count);

/*!
  \brief
*/
__INLINE__
uint32_t Kernel_GetNumSpecDomains(uint32_t* current_domain_count);

/*!
  \brief Returns the number of speculative domains that the kernel needs
*/
__INLINE__
uint32_t Kernel_GetNumKernelSpecDomains(uint32_t* kernel_domain_usage);

/*!
  \brief Sets the domain mode
  
  \deprecated
*/
__INLINE__
uint32_t Kernel_SetSpecDomainMode(unsigned int domain, uint64_t domainmode);


/*!
  \brief Controls the rate that the L2 reclaims speculation IDs
  
  \param[in] scrub_rate new scrub rate, in cycles
*/
__INLINE__
uint32_t Kernel_SetL2SpecScrubRate(uint64_t scrub_rate);

/*!
  \brief Obtains details on a speculation conflict
  \param[in] specConflictSize size of the info structure, in bytes
  \param[out] info pointer that will contain the conflictInformation
*/
__INLINE__
uint32_t Kernel_GetSpecConflictInfo(size_t specConflictSize, SpecConflictInfo_t* info);

/*!
  \brief
*/
__INLINE__
uint32_t Kernel_ClearSpecConflictInfo(uint32_t l2slice);

/*!
  \brief
*/
__INLINE__
uint32_t Kernel_EnableFastSpeculationPaths();

/*!
  \brief
*/
__INLINE__
uint32_t Kernel_GetSpeculationIDSelf(uint64_t* specid);

/**
 * \brief Include implementations of the above functions.
 */
#include "spec_impl.h"

__END_DECLS

#endif /* _KERNEL_SPEC_H_ */
