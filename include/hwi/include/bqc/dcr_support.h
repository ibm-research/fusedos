/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/*                                                                  */
/* US Government Users Restricted Rights - Use, duplication or      */
/* disclosure restricted by GSA ADP Schedule Contract with IBM      */
/* Corporation.                                                     */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */

#ifndef   _DCR_SUPPORT_H_ // Prevent multiple inclusion
#define   _DCR_SUPPORT_H_

#include <hwi/include/bqc/PhysicalMap.h>

#ifndef __ASSEMBLY__

/*
 * NOTE:  These access routines and pointer definitions assume that the
 *        DCR space is mapped virtual==physical in the caller's
 *        environment.
 */

#include <hwi/include/common/compiler_support.h>

#define _DCR_USER_BASE PHYMAP_MINADDR_DCR
#define _DCR_PRIV_BASE (_DCR_USER_BASE + PHYMAP_PRIVILEGEDOFFSET)

__INLINE__ void DCRWriteUser(uint64_t dcr, uint64_t v)
{
    ((volatile uint64_t *)(_DCR_USER_BASE))[dcr] = v;
}

__INLINE__ uint64_t DCRReadUser(uint64_t dcr)
{
    return ((volatile uint64_t *)(_DCR_USER_BASE))[dcr];
}

__INLINE__ void DCRWritePriv(uint64_t dcr, uint64_t v)
{
    ((volatile uint64_t *)(_DCR_PRIV_BASE))[dcr] = v;
}

__INLINE__ uint64_t DCRReadPriv(uint64_t dcr)
{
    return ((volatile uint64_t *)(_DCR_PRIV_BASE))[dcr];
}

#define _DCR_PTR(type,base,offset) \
	((volatile type *) ((base) + (8 * (offset))))

#define _DCR_USER_PTR(type,entity) \
	_DCR_PTR(type, _DCR_USER_BASE, _DCR_BASE(entity))

#define _DCR_USER_PTR__NAME(type,entity,name) \
        _DCR_PTR(type, _DCR_USER_BASE, _DCR_BASE__NAME(entity, name))

#define _DCR_USER_PTR__INDEX(type,entity,index)\
	_DCR_PTR(type, _DCR_USER_BASE, _DCR_BASE__INDEX(entity, index))

#define _DCR_PRIV_PTR(type,entity) \
	_DCR_PTR(type, _DCR_PRIV_BASE, _DCR_BASE(entity))

#define _DCR_PRIV_PTR__NAME(type,entity,name) \
        _DCR_PTR(type, _DCR_PRIV_BASE, _DCR_BASE__NAME(entity, name))

#define _DCR_PRIV_PTR__INDEX(type,entity,index) \
	_DCR_PTR(type, _DCR_PRIV_BASE, _DCR_BASE__INDEX(entity, index))

#endif // !__ASSEMBLY__

/*
 * Most DCR entities are accessed through the global DCR space in the
 * physical memory map, but a few (gea, upc_c, and wu) are accessed through
 * dedicated MMIO regions.  These *_space macros give us a level of
 * indirection that allows the access methods and pointer declarations
 * to work for both types of entities.
 */
#define BEDRAM_DCR_space       0
#define CS_DCR_space           0
#define DC_ARBITER_DCR_space   0
#define DEVBUS_DCR_space       0
#define DR_ARB_DCR_space       0
#define EN_DCR_space           0
#define GEA_DCR_space          ((PHYMAP_MINADDR_INTAG - PHYMAP_MINADDR_DCR) / 8)
#define L1P_DCR_space          0
#define L2_CENTRAL_DCR_space   0
#define L2_COUNTER_DCR_space   0
#define L2_DCR_space           0
#define MS_GENCT_DCR_space     0
#define MU_DCR_space           0
#define ND_500_DCR_space       0
#define ND_RESE_DCR_space      0
#define ND_X2_DCR_space        0
#define PE_DCR_space           0
#define SERDES_LEFT_DCR_space  0
#define SERDES_RIGHT_DCR_space 0
#define TESTINT_DCR_space      0
#define UPC_C_DCR_space        ((PHYMAP_MINADDR_UPC - PHYMAP_MINADDR_DCR) / 8)
#define WU_DCR_space           (-1) // TODO define proper address

#define _DCR_BASE(entity) \
	((entity ## _space) + (entity ## _base))

#define _DCR_BASE__NAME(entity,name) \
	((entity ## _space) + (entity ## __ ## name ## _base))

#define _DCR_BASE__INDEX(entity,index) \
	((entity ## _space) + (entity ## _base) + \
		((index) * (entity ## _stride)))

#define _DCR_REG_OFFSET(entity,reg) (entity ## __ ## reg ## _offset)

#define _DCR_REG(entity,reg) \
	(_DCR_BASE(entity) + _DCR_REG_OFFSET(entity, reg))

#define _DCR_REG__NAME(entity,name,reg) \
	(_DCR_BASE__NAME(entity, name) + _DCR_REG_OFFSET(entity, reg))

#define _DCR_REG__INDEX(entity,index,reg) \
	(_DCR_BASE__INDEX(entity, index) + _DCR_REG_OFFSET(entity, reg))

#endif // add nothing below this line.
