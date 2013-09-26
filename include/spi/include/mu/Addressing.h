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

#ifndef	_MUSPI_ADDRESSING_H_ /* Prevent multiple inclusion */
#define	_MUSPI_ADDRESSING_H_


/**
 * \file Addressing.h
 *
 * \brief C Header File containing Message Unit SPI Addressing Definitions
 *
 * Base Address Table structures are defined.
 *
 * \see Addressing_inlines.h
 *
 */


#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/MU_Addressing.h>
#include <spi/include/mu/Util.h>


__BEGIN_DECLS


/**
 * \brief Base Address Table SubGroup
 *
 * Contains information needed to manage the MU base address table for
 * a particular MU SubGroup.  Multiple users can allocate base address table
 * entries in the same SubGroup, but a particular entry can only be allocated
 * by one user.  This structure tracks the entries that have been allocated by
 * a particular user.
 *
 */
typedef struct MUSPI_BaseAddressTableSubGroup
{
  uint8_t subgrpid; /**< The MU SubGroup this base address table is associated
		         with (0 to BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
		    */

  uint8_t permissions; /**< Permission bits specifying which base address table
			    entries in SubGroup subgrpid are being managed by
			    this structure.  A 1 in bit i (i=0 to
			    BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1) means
			    that base address table entry i in SubGroup
			    subgrpid has been allocated and is being managed by
			    this structure.
		       */

  MUHWI_BaseAddressTable_t hw_bat; /**< Pointer to hardware base address table
				        for SubGroup subgrpid.
				   */

  MUHWI_BaseAddress_t sw_bat[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP]; /**<
			     A shadow of the hardware base address table. for
			     SubGroup subgrpid.  The MU SPIs maintain this so
			     offset calculations can use this shadow instead
			     of accessing the hardware base address table in
			     SRAM, which is slower.
									*/

} MUSPI_BaseAddressTableSubGroup_t;
 

__END_DECLS


#endif /* _MUSPI_ADDRESSING_H_  */
