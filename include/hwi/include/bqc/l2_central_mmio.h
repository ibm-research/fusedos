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
#ifndef _L2C_MMIO_H_
#define _L2C_MMIO_H_

#include <hwi/include/common/bgq_bitnumbers.h>
#include <hwi/include/bqc/l1p_mmio.h>

/********************************************************
 * L2 central MMIO
 ********************************************************
 */

#ifndef __ASSEMBLY__

#include <stdint.h>

typedef int64_t L2C_SPECID_t;

typedef struct
{
    uint64_t      DomainAttr;        // mode, kill on conflict, interrupt on conflict, interrupt on commit, interrupt on invalidate, commit to id 01 (read-write)
    L2C_SPECID_t  AllocPtr;          // allocate pointer (read-write)
    volatile L2C_SPECID_t  CommitPtr;         // commit pointer (read-write)
    volatile L2C_SPECID_t  ReclaimPtr;        // reclaim pointer (read-write)
    L2C_SPECID_t  PtrTryAlloc;       // try allocate using alloc ptr (read-only)
    L2C_SPECID_t  PtrForceCommit;    // force commit (write-only)
    L2C_SPECID_t  PtrTryCommit;      // try commit (read-only)
    uint64_t      reserved[9];
} L2Central_Domain_t;

typedef struct
{
    uint64_t     state;         // state (read-write)
    uint64_t     conflict;      // conflict table: conflicting ID, one conflict ID, more than 1 conflicting ID (read-write)
    uint64_t     conflictsc;    // conflict table: conflicting ID, one conflict ID, more than 1 conflicting ID (read-write)
    L2C_SPECID_t trycommit;     // try comit (read-only)
    uint64_t	 trychangestate; // change the state if old state matches predicate
    uint64_t     reserved[11];
} L2Central_ID_t;

typedef struct
{
    // L2 base address + 0x0
    uint64_t NumDomains;            // number of domains (read-write)
    uint64_t PrivMap;               // privilege map: 2b per function, allowing user access read and write access
    
    // L2 base address + 0x20
    uint64_t reserved1[2];          // reserved
    
    uint64_t IDStateTable[4];       // ID state, 256b across 4 addresses (read-only)
    
    uint64_t reserved2[248];    // reserved
    
    // L2 base address + 0x800
    L2Central_Domain_t domain[16];  // 16 domains
    
    uint64_t reserved3[3584];
    
    // L2 base address + 0x8000
    L2Central_ID_t id[128];
    
} L2Central_t;

#endif // !__ASSEMBLY__

/* --------------------------------------------------- */
/*  BGQ TLS memory-mapped register offsets             */
/* --------------------------------------------------- */

// L2 Central - privileged access
#define L2_CENTRAL_PRIV_BASE_TLS_ADDRESS        (PHYMAP_MINADDR_L2 + PHYMAP_PRIVILEGEDOFFSET)

// L2 Central - user access
#define L2_CENTRAL_USER_BASE_TLS_ADDRESS        (PHYMAP_MINADDR_L2)

#define L2C_TRYALLOC_OFFSET(groupmask)  (L2_CENTRAL_USER_BASE_TLS_ADDRESS + _B1(44,1) + (groupmask<<3))

#define L2C_PRIVMAP_DISABLEDOMAIN(domain)    _B1(domain, 1)
#define L2C_PRIVMAP_DISABLEREADFNC(funcid)   _B1(funcid+32, 1)
#define L2C_PRIVMAP_DISABLEWRITEFNC(funcid)  _B1(funcid+48, 1)
    #define L2C_PRIVMAP_FUNC_TRYALLOC               0
    #define L2C_PRIVMAP_FUNC_NUMDOM                 1
    #define L2C_PRIVMAP_FUNC_PRIVMAP                2
    #define L2C_PRIVMAP_FUNC_IDSTTBL                3
    #define L2C_PRIVMAP_FUNC_DOMMODE                4
    #define L2C_PRIVMAP_FUNC_ALLOCPTR               5
    #define L2C_PRIVMAP_FUNC_COMMPTR                6
    #define L2C_PRIVMAP_FUNC_RCLPTR                 7
    #define L2C_PRIVMAP_FUNC_PTRTRYALLOC            8
    #define L2C_PRIVMAP_FUNC_PTRFORCECOM            9
    #define L2C_PRIVMAP_FUNC_PTRTRYCOM              10
    #define L2C_PRIVMAP_FUNC_IDST                   11
    #define L2C_PRIVMAP_FUNC_CONFL                  12
    #define L2C_PRIVMAP_FUNC_CONFLSC                13
    #define L2C_PRIVMAP_FUNC_IDTRYCOM               14

#define L2C_IDSTATE_AVAIL               0x00        // available
#define L2C_IDSTATE_SPEC                0x01        // speculative
#define L2C_IDSTATE_COMMIT              0x02        // committed
#define L2C_IDSTATE_INVAL               0x03        // invalid
#define L2C_IDSTATE_PRED_AVAIL          _B1(58+L2C_IDSTATE_AVAIL,1)   // predicate for available
#define L2C_IDSTATE_PRED_SPEC           _B1(58+L2C_IDSTATE_SPEC,1)    // predicate for speculative
#define L2C_IDSTATE_PRED_COMMIT         _B1(58+L2C_IDSTATE_COMMIT,1)  // predicate for committed
#define L2C_IDSTATE_PRED_INVAL          _B1(58+L2C_IDSTATE_INVAL,1)   // predicate for invalid

#define L2C_DOMAINATTR_MODE(mode)       _B3(63, mode)   // Mode of the doman (e.g., TLS, TM, Rollback)
    #define L2C_DOMAINATTR_MODE_LTLS        0x0         // Long running TLS
    #define L2C_DOMAINATTR_MODE_STLS        0x1         // Short running TLS
    #define L2C_DOMAINATTR_MODE_LTM         0x2         // Long running TM
    #define L2C_DOMAINATTR_MODE_STM         0x3         // Short running TM
    #define L2C_DOMAINATTR_MODE_ROLL        0x4         // Rollback
#define L2C_DOMAINATTR_INVAL_ON_CNFLT   _B1(60,1)       // Invalidate on conflict
#define L2C_DOMAINATTR_INT_ON_CNFLT     _B1(59,1)       // Interrupt on conflict
#define L2C_DOMAINATTR_INT_ON_COMMIT    _B1(58,1)       // Interrupt on commit
#define L2C_DOMAINATTR_INT_ON_INVAL     _B1(57,1)       // Interrupt on invalidate
#define L2C_DOMAINATTR_COMMITTO_ID01    _B1(56,1)       // Commit memory to ID01

#define L2C_CONFLICT_ID(n)                 _B7(63, n)   // Speculative ID that caused a single conflict.
#define L2C_CONFLICT_SINGLE                _B1(56, 1)   // Single conflict with speculative thread.  Needs to be cleared for commit
#define L2C_CONFLICT_SINGLEA               _B1(55, 1)   // Single conflict with speculative thread.  Needs to be cleared for reallocate, ignored for commit
#define L2C_CONFLICT_MULTI                 _B1(54, 1)   // More than one conflict with a speculative thread.  Needs to be cleared for commit
#define L2C_CONFLICT_NONSPEC               _B1(53, 1)   // Conflict with non-speculative thread.  Needs to be cleared for commit. 
#define L2C_CONFLICT_INVALIDATE            _B1(52, 1)   // FIXME@mschaal: this name is misleading. redefine as compiler error after a grace period saying that L2_CONFLICT_RESOURCE should be used instead
#define L2C_CONFLICT_RESOURCE              _B1(52, 1)   // Resource Conflict.  Thread has been invalidated due to eviction and cannot commit anymore. 
#endif
