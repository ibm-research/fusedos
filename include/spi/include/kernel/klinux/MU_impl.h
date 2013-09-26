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

#ifndef  _KERNEL_KLINUX_MU_IMPL_H_
#define  _KERNEL_KLINUX_MU_IMPL_H_


/**
 * \file klinux/MU_impl.h
 *
 * \brief C Header File containing Message Unit SPI
 *        implementations for the Linux kernel.
 *
 */

/**
 * \brief Query Deallocated MU InjFifos within a SubGroup
 *
 * Query the allocation status of injection fifos in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated fifos.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid       SubGroup number being queried 
 *                             (0 to MU_NUM_INJ_FIFOS_PER_SUBGROUP-1)
 * \param[out]  nfifos         Pointer to an int where the number of deallocated
 *                             fifos in the specified SubGroup is returned
 * \param[out]  fifoids        Pointer to an array of nfifo ints where
 *                             the list of deallocated fifos is returned.
 *                             Each entry is the fifo number 
 *                             (0 to DMA_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *                             The caller must provide space for 
 *                             MU_NUM_INJ_FIFOS_PER_SUBGROUP ints,
 *                             in case the entire fifo SubGroup is deallocated.
 *
 * \retval  0  Successful.    nfifos and fifoids array set as described.
 * \retval  errno Unsuccessful.
 */
__INLINE__
int32_t Kernel_QueryInjFifos (uint32_t   subgrpid,
			      uint32_t * nfifos,
			      uint32_t * fifoids)
{
  int32_t rc=0;
  uint32_t i;
  uint32_t count = 0;
  MUHWI_Fifo_t *HWFifoPtr;
  int gid, sgid;
  
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* A fifo whose start address is zero is considered free.
   */
  for ( i=0; i<BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP; i++ )
    {
      HWFifoPtr = (MUHWI_Fifo_t *) BGQ_MU_iDMA_START_OFFSET (gid,sgid,i);

      if ( HWFifoPtr->pa_start == 0ULL )
	{
	  fifoids[count++] = i;
	}
    }
  *nfifos = count;  

  return rc;
}




/**
 * \brief Query Deallocated MU RecFifos within a SubGroup
 *
 * Query the allocation status of reception fifos in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated fifos.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid       SubGroup number being queried 
 *                             (0 to MU_NUM_REC_FIFOS_PER_SUBGROUP-1)
 * \param[out]  nfifos         Pointer to an int where the number of deallocated
 *                             fifos in the specified SubGroup is returned
 * \param[out]  fifoids        Pointer to an array of *nfifos ints where
 *                             the list of deallocated fifos is returned.
 *                             Each entry is the fifo number 
 *                             (0 to MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 *                             The caller must provide space for 
 *                             MU_NUM_REC_FIFOS_PER_SUBGROUP ints,
 *                             in case the entire fifo SubGroup is deallocated.
 *
 * \retval  0  Successful.     num_fifos and fifoids array set as described.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_QueryRecFifos (uint32_t   subgrpid,
			      uint32_t * nfifos,
			      uint32_t * fifoids)
{
  int32_t rc=0;
  uint32_t i;
  uint32_t count = 0;
  MUHWI_Fifo_t *HWFifoPtr;
  int gid, sgid;

  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* A fifo whose start address is zero is considered free.
   */

  for ( i=0; i<BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; i++ )
    {
      HWFifoPtr = (MUHWI_Fifo_t *) BGQ_MU_rDMA_START_OFFSET (gid,sgid,i);

      if ( HWFifoPtr->pa_start == 0ULL )
	{
	  fifoids[count++] = i;
	}
    }

  *nfifos = count;  

  return rc;
}

/**
 * \brief Query Deallocated Base Address Table within a SubGroup
 *
 * Query the allocation status of base address table entries in the specified
 * SubGroup.  An entry is either "allocated" or "deallocated".  This function
 * returns a list of the deallocated entries.  These are available to be
 * allocated.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped lock, 
 * if necessary.
 *
 * \param[in]   subgrpid  SubGroup being queried (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  nbatids   Pointer to the number IDs returned in batids 
 *                        (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).  Zero means
 *                        that no IDs are deallocated in this SubGroup.
 * \param[out]  batids    Pointer to an array of Base Address Table IDs where
 *                        the list of deallocated Base Address Table IDs is
 *                        returned.  Each entry is the Base Address Table ID
 *                        (0 to MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *                        The caller must provide space for
 *                        BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP IDs to be returned.
 *
 * \retval  0         Success
 * \retval  errno        Unsuccessful.
 *
 * \see Kernel_AllocateBaseAddressTable
 * \see Kernel_DeallocateBaseAddressTable
 */
__INLINE__
int32_t Kernel_QueryBaseAddressTable (uint32_t   subgrpid,
				      uint32_t * nbatids,
				      uint32_t * batids)
{
  int32_t  rc = 0;
  uint32_t i;
  uint32_t numFree = 0;
  MUHWI_BaseAddressTable_t bat;
  int gid, sgid;

  MUSPI_assert ( batids  != NULL );
  MUSPI_assert ( nbatids != NULL );
  MUSPI_assert ( subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE );

  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /*
   * A base address table entry value of 0x1FFFFFFFFFULL (all 37 address bits set ON)
   * indicates that the entry is free.  Loop through the entries in this subgroup,
   * and return the list of free entries.
   */
  bat = (MUHWI_BaseAddressTable_t)
    BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET ( gid, sgid );

  for ( i=0; i<BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP; i++ )
    {
      if ( bat[i] == 0x1FFFFFFFFFULL )  // Is this a free entry?
	{
	  batids[numFree++] = i;
	}
    }

  *nbatids = numFree;
  
  return rc;
}


/**
 * \brief Allocate Injection Fifos
 *
 * Allocate specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being allocated (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  sg_ptr    Pointer to a structure that is filled in upon
 *                        successful return for use in other inline 
 *                        functions to operate on the allocated fifos. 
 *                        This call first zeros all fields in the 
 *                        subgrp structure before filling in the 
 *                        allocated fifos.
 *                        \li fifos - Array of fifo structures. Structures
 *                                    for allocated fifos are initialized 
 *                                    as documented below.  Structures 
 *                                    for fifos not allocated by this 
 *                                    instance of this syscall are 
 *                                    initialized to binary zeros.  
 *                                    Allocated fifos are enabled.
 *                        \li status_ptr  - Points to status area within 
 *                                          the MU memory map.
 *                        \li permissions - Bits indicating which fifos 
 *                                          were allocated during this 
 *                                          syscall.
 *                        \li group_id    - The id of this group.
 * \param[in] nfifos     Number of fifos to be allocated from the group
 *                       (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in] fifoids    Pointer to an array of nfifos uint32_t's where
 *                       the list of fifos to be allocated is provided.
 *                       Each int32_t is the fifo number within the SubGroup
 *                       (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoAttrs  Pointer to an array of nfifos
 *                       Kernel_InjFifoAttributes_t's where the attributes of
 *                       each fifo are specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 */
__INLINE__
int32_t Kernel_AllocateInjFifos (uint32_t                     subgrpid,
				 MUSPI_InjFifoSubGroup_t    * sg_ptr,
				 uint32_t                     nfifos,
				 uint32_t                   * fifoids,
				 Kernel_InjFifoAttributes_t * fifoAttrs)
{
  int32_t rc=0;

  uint32_t idx = 0;
  uint32_t fifoid;
  uint32_t i, j;

  int fifoBitNumber;
  uint64_t fifoIsForRemoteGetMask    = 0;
  uint64_t fifoIsNotForRemoteGetMask = 0;
  uint64_t fifoIsSystemMask = 0;
  uint64_t fifoIsUserMask   = 0;
  uint64_t fifoIsHiPriorityMask     = 0;
  uint64_t fifoIsNormalPriorityMask = 0;

  uint32_t numFree;
  uint32_t freeFifoIds[BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP];
  uint32_t found;
  int gid, sgid, gfid;
  uint64_t perFifoSystem;
  volatile uint64_t *muDCR_fifoAvailableForRemoteGet_ptr;
  volatile uint64_t dcr_value;
  volatile uint64_t *muDCR_systemFifo_ptr;

  /* Indicates whether any of the fifo attrs specifies "System".
   * This value is AND'd with the privileged MU MMIO addresses to produce
   * the final address that is stored in the subgroup structure.
   * For system fifos, we AND it with all bits ON so the full privileged
   * address is used.  For user fifos, we AND it with the negated
   * privilege bit so the privilege bit is turned off.
   * 
   * This is used when filling in MU MMIO addresses for fields that
   * span multiple fifos (such as the status).  If any fifos are for
   * system use, then this address is returned as a privileged address.
   */
  uint64_t system = ~PHYMAP_PRIVILEGEDOFFSET;

  MUSPI_assert (subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE );
  MUSPI_assert (nfifos <= BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);
  MUSPI_assert (sg_ptr != NULL);
  MUSPI_assert (fifoids != NULL);
  MUSPI_assert (fifoAttrs != NULL);

  /* Query to see which fifos are free.
   * Validate that the requested fifos are free.
   */
  Kernel_QueryInjFifos (subgrpid,
			&numFree,
			freeFifoIds);

/*   printf("AllocateInjFifos: subgrpid=%u, nfifos=%u, numfree=%u\n",subgrpid,nfifos,numFree); */
/*   for (i=0; i<nfifos; i++) printf("fifoid=%u\n",fifoids[i]); */
/*   for (i=0; i<numFree; i++) printf("free=%u\n",freeFifoIds[i]); */

  if ( numFree < nfifos ) return EAGAIN;

  for ( i=0; i<nfifos; i++ ) /* Search through each of the specified Ids */
    {
      fifoid = fifoids[i];
      MUSPI_assert (fifoid < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);

      found = 0;
      for ( j=0; j<numFree; j++ ) /* Search for that Id in the free Ids */
	{
	  if ( fifoid == freeFifoIds[j] )
	    {
	      found = 1;
	      break;
	    }
	}
      if ( found == 0 ) return EAGAIN; /* If not found, exit */
    }

  /* Determine if any fifos are for "system" use.
   * This controls whether we return privileged (for system) or non-privileged
   * (for user) MU MMIO addresses in the subgroup structure.
   */
  for ( idx=0; idx<nfifos; idx++ )
    {
      if ( fifoAttrs[idx].System == 1 ) system = 0xFFFFFFFFFFFFFFFFULL;
    }

  memset(sg_ptr, 0, sizeof(MUSPI_InjFifoSubGroup_t));

  sg_ptr->subgroupid = subgrpid;
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* Set the group status MU MMIO pointer.  If any fifos are "system", then this is
   * a privileged address, otherwise it is a non-privileged address.
   */
  sg_ptr->groupStatus = 
    (MUHWI_InjFifoStatus_t *) ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(gid,sgid) &
				system );

  /* Loop through the fifos being allocated, collecting info about them, including
   * - priority
   * - remote get vs normal
   * - user vs system
   */
  for (idx = 0; idx < nfifos; idx++)
  {
    fifoid = fifoids[idx];

    /* Calculate global fifo id */
    gfid = fifoid + sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP;

    /* Track which fifos want priority */
    if ( fifoAttrs[idx].Priority == 1 )
      fifoIsHiPriorityMask     |= ( 0x0000000000000001ULL << (31 - gfid) );
    else
      fifoIsNormalPriorityMask |= ( 0x0000000000000001ULL << (31 - gfid) );

    /* Set up DCR bits based on the fifo attributes specified.
     */
    fifoBitNumber = 32 + (sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP) + fifoid;

    if ( fifoAttrs[idx].RemoteGet == 1 )
      fifoIsForRemoteGetMask    |= _BN(fifoBitNumber);
    else
      fifoIsNotForRemoteGetMask |= _BN(fifoBitNumber);

    /* The caller is responsible for ensuring that if a "system" fifo is
     * desired, that it is valid.  So, at this point, we blindly honor the
     * "system" attribute.
     */
    if ( fifoAttrs[idx].System == 0 )
	  fifoIsUserMask |= _BN(fifoBitNumber);
	else
	  fifoIsSystemMask |= _BN(fifoBitNumber);	  

  } /* End: Loop through fifos. */

  /* Set the priority */
  if ( fifoIsHiPriorityMask || fifoIsNormalPriorityMask )
    {
/*       printf("Kernel_AllocateInjFifos: Previous Priority Setting = 0x%lx\n",sg_ptr->groupStatus->priority); */

      sg_ptr->groupStatus->priority = 
	( sg_ptr->groupStatus->priority & (~fifoIsNormalPriorityMask) ) | fifoIsHiPriorityMask;

/*       printf("Kernel_AllocateInjFifos: New      Priority Setting = 0x%lx\n",sg_ptr->groupStatus->priority); */
    }
  
  /* Set the injection fifo remote get DCR */
  if ( fifoIsForRemoteGetMask  || fifoIsNotForRemoteGetMask )
    {
      muDCR_fifoAvailableForRemoteGet_ptr = 
	(uint64_t *)(BGQ_MU_DCR_iDMA_FIFO_AVAILABLE_REMOTE_GET_OFFSET(gid));

/*       printf("Kernel_AllocateInjFifos: Previous RemoteGet Setting = 0x%lx\n",*(muDCR_fifoAvailableForRemoteGet_ptr)); */

      /* Turn off normal fifo bits, and turn on remote get bits */
      dcr_value = ( *(muDCR_fifoAvailableForRemoteGet_ptr) &
		    (~fifoIsNotForRemoteGetMask) ) | fifoIsForRemoteGetMask;

/*       printf("Kernel_AllocateInjFifos: New      RemoteGet Setting = 0x%lx\n",dcr_value); */

      *(muDCR_fifoAvailableForRemoteGet_ptr) = dcr_value;
    }

  /* Set the system imFifo DCR */
  if ( fifoIsUserMask || fifoIsSystemMask )
    {
      muDCR_systemFifo_ptr = (uint64_t *)(BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(gid));

/*       printf("Kernel_AllocateInjFifos: Previous System Setting = 0x%lx\n",*(muDCR_systemFifo_ptr)); */

      /* Turn off system bits for all user mode fifos and turn on system bits for system fifos */
      dcr_value = (*(muDCR_systemFifo_ptr) | fifoIsSystemMask) & ~fifoIsUserMask;

/*       printf("Kernel_AllocateInjFifos: New      System Setting = 0x%lx\n",dcr_value); */

      *(muDCR_systemFifo_ptr) = dcr_value;
    }

  /* Now that the user vs system state DCR has been set, determine the MU MMIO
   * address that will be used to access each fifo.  Mark each fifo as allocated.
   */
  for (idx = 0; idx < nfifos; idx++)
  {
    if ( fifoAttrs[idx].System == 0 ) 
      perFifoSystem = ~PHYMAP_PRIVILEGEDOFFSET;
    else 
      perFifoSystem = 0xFFFFFFFFFFFFFFFFULL;

    fifoid = fifoids[idx];

    sg_ptr->_injfifos[fifoid]._fifo.hwfifo =
      (MUHWI_Fifo_t *)( (BGQ_MU_iDMA_START_OFFSET (gid,sgid,fifoid)) & perFifoSystem );
    sg_ptr->_injfifos[fifoid].hw_injfifo =
      (MUHWI_InjFifo_t *)( (BGQ_MU_iDMA_DESC_COUNTER_OFFSET (gid,sgid,fifoid)) & perFifoSystem );
    
    /* printf ("Injection SRAM Address for id %u is %llx \n", fifoid,
       sg_ptr->_injfifos[fifoid]._fifo.hwfifo); */

    /* Mark this fifo as allocated by setting its start to a valid non-zero value.
     */
    sg_ptr->_injfifos[fifoid]._fifo.hwfifo->pa_start = 0x40ULL;

  } /* End: Loop through fifos. */

  return rc;
}




/**
 * \brief Allocate Reception Fifos
 *
 * Allocate specified reception fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being allocated (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out]  sg_ptr    Pointer to a structure that is filled in upon
 *                        successful return for use in other inline 
 *                        functions to operate on the allocated fifos. 
 *                        This call first zeros all fields in the 
 *                        subgrp structure before filling in the 
 *                        allocated fifos.
 *                        \li fifos - Array of fifo structures. Structures
 *                                    for allocated fifos are initialized 
 *                                    as documented below.  Structures 
 *                                    for fifos not allocated by this 
 *                                    instance of this syscall are 
 *                                    initialized to binary zeros.  
 *                                    Allocated fifos are enabled.
 *                        \li status_ptr  - Points to status area within 
 *                                          the MU memory map.
 *                        \li permissions - Bits indicating which fifos 
 *                                          were allocated during this 
 *                                          syscall.
 *                        \li group_id    - The id of this group.
 * \param[in] nfifos     Number of fifos to be allocated from the group
 *                       (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in] fifoids    Pointer to an array of nfifos int32_t's where
 *                       the list of fifos to be allocated is provided.
 *                       Each int32_t is the fifo number within the SubGroup
 *                       (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoAttrs  Pointer to an array of nfifos
 *                       Kernel_RecFifoAttributes_t's where the attributes of
 *                       each fifo are specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 */
__INLINE__
int32_t Kernel_AllocateRecFifos (uint32_t                     subgrpid,
				 MUSPI_RecFifoSubGroup_t    * sg_ptr,
				 uint32_t                     nfifos,
				 uint32_t                   * fifoids,
				 Kernel_RecFifoAttributes_t * fifoAttrs)
{
  int32_t rc=0;
  uint32_t idx = 0;
  uint32_t fifoid;
  uint32_t i, j;

  int fifoBitNumber;
  uint64_t fifoIsUserMask = 0;
  uint64_t fifoIsSystemMask = 0;

  /* Indicates whether any of the fifo attrs specifies "System".
   * This value is AND'd with the privileged MU MMIO addresses to produce
   * the final address that is stored in the subgroup structure.
   * For system fifos, we AND it with all bits ON so the full privileged
   * address is used.  For user fifos, we AND it with the negated
   * privilege bit so the privilege bit is turned off.
   * 
   * This is used when filling in MU MMIO addresses for fields that
   * span multiple fifos (such as the status).  If any fifos are for
   * system use, then this address is returned as a privileged address.
   */
  uint64_t system = ~PHYMAP_PRIVILEGEDOFFSET;

  uint32_t numFree;
  uint32_t freeFifoIds[BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP];
  uint32_t found;
  int gid, sgid;
  uint64_t perFifoSystem;
  volatile uint64_t *muDCR_systemFifo_ptr;
  volatile uint64_t dcr_value;
  
  MUSPI_assert (subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE);
  MUSPI_assert (nfifos <= BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP);
  MUSPI_assert (sg_ptr != NULL);
  MUSPI_assert (fifoids != NULL);
  MUSPI_assert (fifoAttrs != NULL);

  /* Query to see which fifos are free.
   * Validate that the requested fifos are free.
   */
  Kernel_QueryRecFifos (subgrpid,
			&numFree,
			freeFifoIds);

/*   printf("AllocateRecFifos: subgrpid=%u, nfifos=%u, numfree=%u\n",subgrpid,nfifos,numFree); */
/*   for (i=0; i<nfifos; i++) printf("fifoid=%u\n",fifoids[i]); */
/*   for (i=0; i<numFree; i++) printf("free=%u\n",freeFifoIds[i]); */

  if ( numFree < nfifos ) return EAGAIN;

  for ( i=0; i<nfifos; i++ ) /* Search through each of the specified Ids */
    {
      fifoid = fifoids[idx];
      MUSPI_assert (fifoid < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP);

      found = 0;
      for ( j=0; j<numFree; j++ ) /* Search for that Id in the free Ids */
	{
	  if ( fifoid == freeFifoIds[j] )
	    {
	      found = 1;
	      break;
	    }
	}
      if ( found == 0 ) return EAGAIN; /* If not found, exit */
    }

  /* Determine if any fifos are for "system" use.
   * This controls whether we return privileged (for system) or non-privileged
   * (for user) MU MMIO addresses in the subgroup structure.
   */
  for ( idx=0; idx<nfifos; idx++ )
    {
      if ( fifoAttrs[idx].System == 1 ) system = 0xFFFFFFFFFFFFFFFFULL;
    }

  memset(sg_ptr, 0, sizeof(MUSPI_RecFifoSubGroup_t));
  
  sg_ptr->subgroupid = subgrpid;
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* Set the group status MU MMIO pointer.  If any fifos are "system", then this is
   * a privileged address, otherwise it is a non-privileged address.
   */
  sg_ptr->groupStatus = 
    (MUHWI_RecFifoStatus_t *) ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(gid, sgid) &
				system );
    
  /* Loop through the fifos being allocated, collecting info about them, including
   * - user vs system
   */
  for (idx = 0; idx < nfifos; idx++) {

    fifoid = fifoids[idx];
    
    fifoBitNumber = 48 + (sgid * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) + fifoid;

    /* The caller is responsible for ensuring that if a "system" fifo is
     * desired, that it is valid.  So, at this point, we blindly honor the
     * "system" attribute.
     */
    if ( fifoAttrs[idx].System == 0 )
      fifoIsUserMask |= _BN(fifoBitNumber);
    else
      fifoIsSystemMask |= _BN(fifoBitNumber);

  }  /* End:  Loop through fifos. */
  
/*   printf("thresholdInterruptEnableMask=0x%llx\npacketInterruptEnableMask=0x%llx\nfifoIsUserMask=0x%llx\n", */
/* 	 (long long unsigned int)thresholdInterruptEnableMask, */
/* 	 (long long unsigned int)packetInterruptEnableMask, */
/* 	 (long long unsigned int)fifoIsUserMask); */

  /* Set the system rmFifo DCR */
  if ( fifoIsUserMask || fifoIsSystemMask )
    {
      muDCR_systemFifo_ptr = 
	(uint64_t *)(BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(gid));

/*       printf("Kernel_AllocateRecFifos: Previous System Setting = 0x%lx\n",*(muDCR_systemFifo_ptr)); */

      /* Turn off system bits for all user mode fifos, and turn on system bits for system fifos */
      dcr_value = (*(muDCR_systemFifo_ptr) | fifoIsSystemMask) & ~fifoIsUserMask;

/*       printf("Kernel_AllocateRecFifos: New      System Setting = 0x%lx\n",dcr_value); */

      *(muDCR_systemFifo_ptr) = dcr_value;
    }

  /* Now that the user vs system state DCR has been set, determine the MU MMIO
   * address that will be used to access each fifo.  Mark each fifo as allocated.
   */
  for (idx = 0; idx < nfifos; idx++) {

    if ( fifoAttrs[idx].System == 0 ) 
      perFifoSystem = ~PHYMAP_PRIVILEGEDOFFSET;
    else 
      perFifoSystem = 0xFFFFFFFFFFFFFFFFULL;

    fifoid = fifoids[idx];
    
    sg_ptr->_recfifos[fifoid]._fifo.hwfifo = 
      (MUHWI_Fifo_t *) ( BGQ_MU_rDMA_START_OFFSET (gid,sgid,fifoid) & perFifoSystem );
/*   printf("AllocateRecFifos fifoptr = %p\n",sg_ptr->_recfifos[fifoid]._fifo.hwfifo); */

    /* printf ("Reception SRAM Address for id %d is %llx\n", fifoid,
       sg_ptr->_recfifos[fifoid]._fifo.hwfifo); */

    /* Mark this fifo as allocated by setting its start to a valid non-zero value. */
    sg_ptr->_recfifos[fifoid]._fifo.hwfifo->pa_start = 0x240ULL;

  }  /* End:  Loop through fifos. */

  return rc;
}







/**
 * \brief Allocate Base Address Table
 *
 * Allocate specified base address table entries in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param[in]  subgrpid  SubGroup being allocated (0 to 
 *                       BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[out] sg_ptr    Pointer to a base address table SubGroup structure
 *                       that is initialized by this function.
 * \param[in]  nbatids   Number of IDs to be allocated (1 to 
 *                       BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).
 * \param[in]  batids    Array of Base Address Table IDs to be allocated.
 * \param[in]  system    Indicates whether the Base Address Table IDs being allocated
 *                       are for "system" use or for "user" use.
 *                       0 = User use.
 *                       1 = System use.
 *                       Note that all Base Address Table IDs in a subgroup have the
 *                       same "system" attribute.  That is, they are all either for
 *                       "system" use or the are all for "user" use.  Thus, if
 *                       there are some IDs already allocated, the "system" attribute
 *                       has already been determined.  If the "system" attribute
 *                       attribute specified on this call conflicts with that,
 *                       this call will fail and no IDs will be allocated.
 *
 * \retval  0  Successful.    batids are allocated.
 * \retval  errno Unsuccessful.  A specified ID is already allocated.
 * \retval  errno Unsuccessful.  There are already some IDs allocated in this subgroup
 *                            and the specified "system" parameter is inconsistent
 *                            with the "system" state of the already allocated IDs.
 *
 * \see Kernel_QueryBaseAddressTable
 * \see Kernel_DeallocateBaseAddressTable
 */
__INLINE__
int32_t Kernel_AllocateBaseAddressTable (uint32_t                           subgrpid,
					 MUSPI_BaseAddressTableSubGroup_t * sg_ptr,
					 uint32_t                           nbatids,
					 uint32_t                         * batids,
					 uint32_t                           system)
{
  uint32_t i, j;
  uint32_t numFree;
  uint32_t freeBatIds[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];
  uint32_t systemStatus = system;  /* Default matches what caller wants. */
  volatile uint64_t *muDCR_ptr;
  uint32_t found;
  uint64_t privMask;
  int      gid, sgid;
  volatile uint64_t dcr_value;
  uint32_t batid;

  MUSPI_assert ( batids != NULL);
  MUSPI_assert ( sg_ptr != NULL);
  MUSPI_assert ( subgrpid <  BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE );
  MUSPI_assert ( nbatids  <= BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
  MUSPI_assert ( nbatids  >  0 );
  MUSPI_assert ( (system == 0) || (system == 1) );

  /* Determine whether there are any base address table slots allocated in this
   * subgroup.  If there are, then the "system" parameter must match the
   * "system" specification of the already allocated fifos, since the subgroup
   * must be all "system" or all "user".
   */
  Kernel_QueryBaseAddressTable (subgrpid,
				&numFree,
				freeBatIds);

/*   printf("AllocateBaseAddressTable: subgrpid=%u, nbatids=%u, numfree=%u\n",subgrpid,nbatids,numFree); */
/*   for (i=0; i<nbatids; i++) printf("batid=%u\n",batids[i]); */
/*   for (i=0; i<numFree; i++) printf("free=%u\n",freeBatIds[i]); */

  if ( numFree < nbatids ) return EAGAIN;
  if ( numFree < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP ) /* Some Ids are allocated? */
    {
      if ( subgrpid <= 63 )
	{
	  muDCR_ptr = (volatile uint64_t *)
	              (BGQ_MU_DCR_BAT_MODE0_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));
	  if ( *muDCR_ptr & (0x8000000000000000ULL >> subgrpid) )
	    systemStatus = 1; /* This subgroup is already "system" use. */
	  else
	    systemStatus = 0; /* This subgroup is already "user" use. */
	}
      else
	{
	  muDCR_ptr = (volatile uint64_t *)
	              (BGQ_MU_DCR_BAT_MODE1_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));
	  if ( *muDCR_ptr & (0x8000000000000000ULL >> (subgrpid-64) ) )
	    systemStatus = 1; /* This subgroup is already "system" use. */
	  else
	    systemStatus = 0; /* This subgroup is already "user" use. */
	}
    }

  /* The caller's "system" request must match any already-present state */
  if ( system != systemStatus ) return EACCES;
  
  /*
   * Verify that the requested entries are free.
   */
  for ( i=0; i<nbatids; i++ ) /* Search through each of the specified Ids */
    {
      batid = batids[i];
      MUSPI_assert ( batid < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
      found = 0;
      for ( j=0; j<numFree; j++ ) /* Search for that Id in the free Ids */
	{
	  if ( batid == freeBatIds[j] )
	    {
	      found = 1;
	      break;
	    }
	}
      if ( found == 0 ) return EAGAIN; /* If not found, exit */
    }

  /* Determine a mask to be AND'd with the privileged MU MMIO pointer to the
   * Base Address Table for this subgroup.  This will either leave ON the
   * privileged bit (for "system") or turn it off (for "user").
   */
  privMask = 0xFFFFFFFFFFFFFFFFULL;
  if ( system == 0 ) privMask = ~PHYMAP_PRIVILEGEDOFFSET;					
  
  /* Set the MU DCR such that it reflects "system" or "user" access to the Base Address
   * Table entries in this subgroup.
   */
  if ( subgrpid < ( (BGQ_MU_NUM_FIFO_GROUPS-1)*BGQ_MU_NUM_FIFO_SUBGROUPS ) )
    {
      muDCR_ptr = 
	(uint64_t *)(BGQ_MU_DCR_BAT_MODE0_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));

/*       printf("Kernel_AllocateBaseAddressTable: Previous System DCR = 0x%lx\n",*muDCR_ptr); */
      
      if ( system == 0 )
	dcr_value = *(muDCR_ptr) & ~(_BN(subgrpid));
      else
	dcr_value = *(muDCR_ptr) | _BN(subgrpid);

/*       printf("Kernel_AllocateBaseAddressTable: New      System DCR = 0x%lx\n",dcr_value); */
      
      *(muDCR_ptr) = dcr_value;
    }
  else
    {
      muDCR_ptr = 
	(uint64_t *)(BGQ_MU_DCR_BAT_MODE1_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));

/*       printf("Kernel_AllocateBaseAddressTable: Previous System DCR = 0x%lx\n",*muDCR_ptr); */
      
      if ( system == 0 )
	dcr_value = *(muDCR_ptr) & ~(_BN(subgrpid - (BGQ_MU_NUM_FIFO_GROUPS-1)*BGQ_MU_NUM_FIFO_SUBGROUPS ));
      else
	dcr_value = *(muDCR_ptr) | _BN(subgrpid - (BGQ_MU_NUM_FIFO_GROUPS-1)*BGQ_MU_NUM_FIFO_SUBGROUPS );

/*       printf("Kernel_AllocateBaseAddressTable: New      System DCR = 0x%lx\n",*muDCR_ptr); */
      
      *(muDCR_ptr) = dcr_value;
    }

  /* Mark the requested Ids as allocated. */
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;
  sg_ptr->subgrpid    = subgrpid;
  sg_ptr->hw_bat      = (MUHWI_BaseAddressTable_t)
    ( BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET ( gid, sgid ) & privMask );
  sg_ptr->permissions = 0;
/*   printf("HW BAT = %p\n",sg_ptr->hw_bat); */
  for ( i=0; i<nbatids; i++ )
    {
      sg_ptr->permissions |= _BN(batids[i]) >> 56; /* Shift 56 to convert 64-bit
						      mask to 8 bit-mask */
      sg_ptr->sw_bat[batids[i]] = 0;
      sg_ptr->hw_bat[batids[i]] = 0; /* This marks the entry allocated */
    }
  
  return 0;
}





/**
 * \brief Enable MU RecFifos
 *
 * Enable MU reception fifos, given a fifo group id and
 * a bit mask indicating which reception fifos in the group
 * to enable.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling and disabling
 * and deallocations of reception fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  groupid     Id of the fifo group
 *                         (0 to BGQ_MU_NUM_FIFO_GROUPS-1).
 * \param[in]  enableBits  Bit mask indicating which fifos
 *                         to enable.  There are 16 fifos
 *                         in a group.  The low-order 16 bits
 *                         are used:
 *                         - Bit 48 is fifo 0
 *                         - Bit 49 is fifo 1
 *                         - and so on...
 *                         - Bit 63 is fifo 15.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_RecFifoEnable (  uint32_t groupid,
				uint64_t enableBits )
{
  int32_t rc=0;
  volatile uint64_t *muDCR_enabled_ptr; 
  volatile uint64_t dcr_value;
  
  MUSPI_assert (groupid < BGQ_MU_NUM_FIFO_GROUPS);
  MUSPI_assert (enableBits != 0);
  
  /*Enable these Reception Fifos*/
  muDCR_enabled_ptr = 
    (volatile uint64_t *)(BGQ_MU_DCR_rDMA_ENABLED_OFFSET(groupid));

  dcr_value = *(muDCR_enabled_ptr) | enableBits;

  *(muDCR_enabled_ptr) = dcr_value;

/*   printf("RecFifoEnable:  Store 0x%llx at DCR %p\n", */
/* 	 (unsigned long long int)enableBits, */
/* 	 muDCR_enabled_ptr); */

  return rc;
}


/**
 * \brief Disable MU RecFifos
 *
 * Disable MU reception fifos, given a fifo group id and
 * a bit mask indicating which reception fifos in the group
 * to disable.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling and disabling
 * and deallocations of reception fifos in the same group.  This can be done
 * by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  groupid     Id of the fifo group
 *                         (0 to BGQ_MU_NUM_FIFO_GROUPS-1).
 * \param[in]  disableBits Bit mask indicating which fifos
 *                         to disable.  There are 16 fifos
 *                         in a group.  The low-order 16 bits
 *                         are used:
 *                         - Bit 48 is fifo 0
 *                         - Bit 49 is fifo 1
 *                         - and so on...
 *                         - Bit 63 is fifo 15.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 *
 */
__INLINE__
int32_t Kernel_RecFifoDisable (  uint32_t groupid,
				 uint64_t disableBits )
{
  int32_t rc=0;
  volatile uint64_t *muDCR_enabled_ptr; 
  volatile uint64_t dcr_value;
  
  MUSPI_assert (groupid < BGQ_MU_NUM_FIFO_GROUPS);
  MUSPI_assert (disableBits != 0);
  
  /*Disable these Reception Fifos*/
  muDCR_enabled_ptr = 
    (volatile uint64_t *)(BGQ_MU_DCR_rDMA_ENABLED_OFFSET(groupid));

  dcr_value = *(muDCR_enabled_ptr) & ~(disableBits);

/*   printf("RecFifoDisable:  Before=0x%lx, Bits=0x%lx, Store 0x%lx at DCR %p\n", */
/* 	 *muDCR_enabled_ptr, */
/* 	 disableBits, */
/* 	 dcr_value, */
/* 	 muDCR_enabled_ptr); */

  *(muDCR_enabled_ptr) = dcr_value;

  return rc;
}




/**
 * \brief Configure Injection Fifo Interrupts
 *
 * Configure interrupts for specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos in the same group.  
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]   nfifos    Number of fifos to be configured within the subgroup.
 *                        (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]   fifoids   Pointer to an array of nfifos int32_t's where
 *                        the list of fifo numbers to be configured is provided.
 *                        Each int32_t is the fifo number within the SubGroup
 *                        (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoInterrupts  Pointer to an array of nfifos 
 *                            Kernel_InjFifoInterrupts_t's where
 *                            the interrupt configuration of each fifo is 
 *                            specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoThresholds
 */
__INLINE__
int32_t Kernel_ConfigureInjFifoInterrupts (MUSPI_InjFifoSubGroup_t    * sg_ptr,
					   uint32_t                     nfifos,
					   uint32_t                   * fifoids,
					   Kernel_InjFifoInterrupts_t * fifoInterrupts)
{
  int32_t rc=0;

  uint32_t idx = 0;
  int      fifoid = -1;

  int      fifoBitNumber;
  uint64_t interruptEnableMask  = 0;
  uint64_t interruptDisableMask = 0;
  int gid, sgid;
  uint32_t subgrpid;
  volatile uint64_t *muDCR_interrupt_ptr;
  uint64_t mu_DCR_value;

  MUSPI_assert (nfifos <= BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);
  MUSPI_assert (sg_ptr != NULL);
  MUSPI_assert (fifoids != NULL);
  MUSPI_assert (fifoInterrupts != NULL);

  subgrpid = sg_ptr->subgroupid;
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  for (idx = 0; idx < nfifos; idx++)
  {
    fifoid = fifoids[idx];
    MUSPI_assert (fifoid < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);
    
    /* Set up DCR bits based on the fifo interrupt attributes specified.
     * Bits 32-63 (one subgroup works with 8 of those bits).
     */
    fifoBitNumber = 32 + (sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP) + fifoid;

    if ( fifoInterrupts[idx].Threshold_Crossing == 1 )
      interruptEnableMask |= _BN(fifoBitNumber);
    else
      interruptDisableMask |= _BN(fifoBitNumber);

  }  /* End: Loop through fifos. */

/*   printf("interruptEnableMask=0x%llx\n", */
/* 	 (long long unsigned int)interruptEnableMask); */

  /* Set the injection fifo interrupts DCR. */
  if ( interruptEnableMask | interruptDisableMask )
    {
      muDCR_interrupt_ptr = 
	(uint64_t *)(BGQ_MU_DCR_iDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(gid));
      mu_DCR_value = *muDCR_interrupt_ptr;
      mu_DCR_value |= interruptEnableMask;
      mu_DCR_value &= ~interruptDisableMask;
      *(muDCR_interrupt_ptr) = mu_DCR_value;
    }

  return rc;
}



/**
 * \brief Configure Reception Fifo Interrupts
 *
 * Configure interrupts for specified reception fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos in the same group.  
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[out]  sg_ptr    Pointer to the fifo SubGroup structure.
 * \param[in]   nfifos    Number of fifos to be configured within the subgroup.
 *                        (1 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in]   fifoids   Pointer to an array of nfifos int32_t's where
 *                        the list of fifo numbers to be configured is provided.
 *                        Each int32_t is the fifo number within the SubGroup
 *                        (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP-1).
 * \param[in] fifoInterrupts  Pointer to an array of nfifos 
 *                            Kernel_RecFifoInterrupts_t's where
 *                            the interrupt configuration of each fifo is 
 *                            specified.
 *
 * \retval  0  Successful.    fifoids are allocated.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoThreshold
 *
 */
__INLINE__
int32_t Kernel_ConfigureRecFifoInterrupts (MUSPI_RecFifoSubGroup_t    * sg_ptr,
					   uint32_t                     nfifos,
					   uint32_t                   * fifoids,
					   Kernel_RecFifoInterrupts_t * fifoInterrupts)
{
  int32_t  rc=0;
  uint32_t idx = 0;
  int      fifoid = -1;

  int      fifoBitNumber;
  uint64_t thresholdInterruptEnableMask = 0;
  uint64_t packetInterruptEnableMask = 0;
  uint64_t thresholdInterruptDisableMask = 0;
  uint64_t packetInterruptDisableMask = 0;
  int gid, sgid;
  uint32_t subgrpid;
  volatile uint64_t *muDCR_interrupt_ptr;
  uint64_t mu_DCR_value;

  MUSPI_assert (nfifos <= BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP);
  MUSPI_assert (sg_ptr != NULL);
  MUSPI_assert (fifoids != NULL);
  MUSPI_assert (fifoInterrupts != NULL);

  subgrpid = sg_ptr->subgroupid;
  gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  for (idx = 0; idx < nfifos; idx++) 
    {
      fifoid = fifoids[idx];
      MUSPI_assert (fifoid < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP);
      
      fifoBitNumber = 48 + (sgid * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) + fifoid;
      
      /* Set up DCR bits based on the interrupt configuration specified.
       */
      if ( fifoInterrupts[idx].Threshold_Crossing == 1 )
	thresholdInterruptEnableMask |= _BN(fifoBitNumber);
      else
	thresholdInterruptDisableMask |= _BN(fifoBitNumber);
      
      if ( fifoInterrupts[idx].Packet_Arrival == 1 )
	packetInterruptEnableMask |= _BN(fifoBitNumber);
      else
	packetInterruptDisableMask |= _BN(fifoBitNumber);
    }  /* End:  Loop through fifos. */
  
/*   printf("thresholdInterruptEnableMask=0x%llx\npacketInterruptEnableMask=0x%llx\n", */
/* 	 (long long unsigned int)thresholdInterruptEnableMask, */
/* 	 (long long unsigned int)packetInterruptEnableMask); */

  /* Set the reception fifo threshold interrupts DCR, and associated fifo threshold */
  if ( thresholdInterruptEnableMask | thresholdInterruptDisableMask )
    {
      muDCR_interrupt_ptr = 
	(uint64_t *)(BGQ_MU_DCR_rDMA_THRESHOLD_INTERRUPT_ENABLE_OFFSET(gid));
      mu_DCR_value = *muDCR_interrupt_ptr;
      mu_DCR_value |= thresholdInterruptEnableMask;
      mu_DCR_value &= ~thresholdInterruptDisableMask;
      *(muDCR_interrupt_ptr) = mu_DCR_value;

    }

  /* Set the reception fifo packet interrupts DCR */
  if ( packetInterruptEnableMask | packetInterruptDisableMask )
    {
      muDCR_interrupt_ptr = 
	(uint64_t *)(BGQ_MU_DCR_rDMA_PACKET_RECEIVED_INTERRUPT_ENABLE_OFFSET(gid));
      mu_DCR_value = *muDCR_interrupt_ptr;
      mu_DCR_value |= packetInterruptEnableMask;
      mu_DCR_value &= ~packetInterruptDisableMask;
      *(muDCR_interrupt_ptr) = mu_DCR_value;
    }

  return rc;
}

/**
 * \brief Configure the Injection Fifo Thresholds
 *
 * Configure the normal and remote get Injection Fifo thresholds.  This setting spans
 * all subgroups on the node.  The MU will raise an interrupt if the free
 * space in an injection fifo becomes greater than the threshold value, 
 * regardless of whether the the fifo is activated.  The interrupt is raised if the 
 * fifo has been configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of injection fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in] normalThreshold     Pointer to the interrupt threshold for all "normal"
 *                                injection fifos on the node.  This value is in
 *                                units of descriptors (64-byte units).
 *                                If NULL, this threshold is not set.
 * \param[in] remoteGetThreshold  Pointer to the interrupt threshold for all "remote get"
 *                                injection fifos on the node.  This
 *                                value is in units of descriptors (64-byte 
 *                                units).  If NULL, this threshold is not set.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureInjFifoInterrupts
 */
__INLINE__
int32_t Kernel_ConfigureInjFifoThresholds( uint64_t * normalThreshold,
					   uint64_t * remoteGetThreshold )
{
  volatile uint64_t *muDCR_threshold_ptr;

  if ( normalThreshold )
    {
      muDCR_threshold_ptr =
	(uint64_t *)(BGQ_MU_DCR_iDMA_THRESHOLD_OFFSET);
      *(muDCR_threshold_ptr) = *normalThreshold;
    }
  
  if ( remoteGetThreshold )
    {
      muDCR_threshold_ptr =
	(uint64_t *)(BGQ_MU_DCR_iDMA_RGET_THRESHOLD_OFFSET);
      *(muDCR_threshold_ptr) = *remoteGetThreshold;
    }
  return 0;
}


/**
 * \brief Configure the Reception Fifo Threshold
 *
 * Configure the Reception Fifo threshold.  This setting spans
 * all subgrou ps on the node.  The MU will raise an interrupt if the free
 * space in a reception fifo drops below this value, regardless of whether
 * the fifo is enabled.  The interrupt is raised if the fifo has been
 * configured to raise the threshold crossing interrupt.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with enabling, disabling,
 * deallocations, and configuring of reception fifos on the node.
 * This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in] threshold     The interrupt threshold for all reception
 *                          fifos on the node.  This value is the number of 
 *                          32-byte packet chunks.
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 * \see Kernel_ConfigureRecFifoInterrupts
 *
 */
__INLINE__
int32_t Kernel_ConfigureRecFifoThreshold( uint64_t threshold )
{
  volatile uint64_t *muDCR_threshold_ptr;

  muDCR_threshold_ptr =
    (uint64_t *)(BGQ_MU_DCR_rDMA_THRESHOLD_OFFSET);
  *(muDCR_threshold_ptr) = threshold;

  return 0;
}



/**
 * \brief Deallocate Injection Fifos
 *
 * Deallocate specified injection fifos in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of injection fifos.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]  sg_ptr   Pointer to fifo SubGroup structure.
 * \param[in]  nfifos   Number of fifos to be deallocated from the group
 *                      (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids  Pointer to an array of nfifos int32_t's where
 *                      the list of fifos to be deallocated is provided.
 *                      Each int32_t is the fifo number within the SubGroup
 *                      (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All fifos were deallocated.
 * \retval  errno  Unsuccessful.
 */
__INLINE__
int32_t Kernel_DeallocateInjFifos (MUSPI_InjFifoSubGroup_t * sg_ptr,
				   uint32_t                  nfifos,
				   uint32_t                * fifoids)
{
  uint32_t i;
  MUHWI_Fifo_t *HWFifoPtr;
  int gid, sgid, gfid, fifoBitNumber;
  uint64_t fifoRemoteGetMask = 0;
  uint64_t fifoSystemMask    = 0;
  uint64_t fifoPriorityMask  = 0;
  uint32_t fifoid;
  volatile uint64_t *muDCR_fifoAvailableForRemoteGet_ptr;
  volatile uint64_t  dcr_value;
  volatile uint64_t *muDCR_systemFifo_ptr;

  gid = sg_ptr->subgroupid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = sg_ptr->subgroupid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* A fifo whose start address is zero is considered free.
   */
  for ( i=0; i<nfifos; i++ )
    {
      fifoid = fifoids[i];

      MUSPI_assert (fifoid < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);

      /* Calculate global fifo id */
      gfid = fifoid + sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP;

      /* Update a mask of priority bits to turn off */
      fifoPriorityMask |= ( 0x0000000000000001ULL << (31 - gfid) );

      /* Calculate a bit number for the remote get and system DCRs */
      fifoBitNumber = 32 + (sgid * BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP) + fifoid;

      /* Update a mask of remote get bits to turn off */
      fifoRemoteGetMask |= _BN(fifoBitNumber);

      /* Update a mask of system fifo bits to turn off */
      fifoSystemMask |= _BN(fifoBitNumber);

      /* Clear the start pointer to indicate the fifo is free */
      HWFifoPtr = (MUHWI_Fifo_t *) BGQ_MU_iDMA_START_OFFSET (gid,sgid,fifoid);

      HWFifoPtr->pa_start = 0ULL;
    }

  /* Reset the priority bits to zero */
  if ( fifoPriorityMask )
    {
/*       printf("Kernel_DeallocateInjFifos: Previous Priority Setting = 0x%lx\n",sg_ptr->groupStatus->priority); */

      sg_ptr->groupStatus->priority = 
	sg_ptr->groupStatus->priority & (~fifoPriorityMask);

/*       printf("Kernel_DeallocateInjFifos: New      Priority Setting = 0x%lx\n",sg_ptr->groupStatus->priority); */
    }
  
  /* Reset the remote get bits to zero */
  if ( fifoRemoteGetMask )
    {
      muDCR_fifoAvailableForRemoteGet_ptr = 
	(uint64_t *)(BGQ_MU_DCR_iDMA_FIFO_AVAILABLE_REMOTE_GET_OFFSET(gid));

/*       printf("Kernel_DeallocateInjFifos: Previous RemoteGet Setting = 0x%lx\n",*(muDCR_fifoAvailableForRemoteGet_ptr)); */

      dcr_value = *(muDCR_fifoAvailableForRemoteGet_ptr) & (~fifoRemoteGetMask);

/*       printf("Kernel_DeallocateInjFifos: New      RemoteGet Setting = 0x%lx\n",dcr_value); */

      *(muDCR_fifoAvailableForRemoteGet_ptr) = dcr_value;
    }
  
  /* Reset the system fifo bits to 1 */
  if ( fifoSystemMask )
    {
      muDCR_systemFifo_ptr = (uint64_t *)(BGQ_MU_DCR_iDMA_SYSTEM_FIFO_OFFSET(gid));

/*       printf("Kernel_DeallocateInjFifos: Previous System Setting = 0x%lx\n",*(muDCR_systemFifo_ptr)); */

      dcr_value = *(muDCR_systemFifo_ptr) | fifoSystemMask;

/*       printf("Kernel_DeallocateInjFifos: New      System Setting = 0x%lx\n",dcr_value); */

      *(muDCR_systemFifo_ptr) = dcr_value;
    }

  return 0;
}


/**
 * \brief Deallocate Reception Fifos
 *
 * Deallocate specified reception fifos in the specified SubGroup.
  *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of reception fifos.  This can be done by obtaining a node-scoped lock, if necessary.
*
 * \param[in]  sg_ptr   Pointer to fifo SubGroup structure.
 * \param[in]  nfifos   Number of fifos to be deallocated from the group
 *                      (1 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]  fifoids  Pointer to an array of nfifos int32_t's where
 *                      the list of fifos to be deallocated is provided.
 *                      Each int32_t is the fifo number within the SubGroup
 *                      (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All fifos were deallocated.
 * \retval  errno  Unsuccessful.
 */
__INLINE__
int32_t Kernel_DeallocateRecFifos (MUSPI_RecFifoSubGroup_t * sg_ptr,
				   uint32_t                  nfifos,
				   uint32_t                * fifoids)
{
  uint32_t i;
  MUHWI_Fifo_t *HWFifoPtr;
  int gid, sgid, fifoBitNumber;
  uint64_t fifoSystemMask    = 0;
  volatile uint64_t *muDCR_systemFifo_ptr;
  volatile uint64_t  dcr_value;
  uint32_t fifoid;
  
  gid = sg_ptr->subgroupid / BGQ_MU_NUM_FIFO_SUBGROUPS;
  sgid = sg_ptr->subgroupid % BGQ_MU_NUM_FIFO_SUBGROUPS;

  /* A fifo whose start address is zero is considered free.
   */
  for ( i=0; i<nfifos; i++ )
    {
      fifoid = fifoids[i];

      MUSPI_assert (fifoid < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP);

      /* Calculate a bit number for the system DCR */
      fifoBitNumber = 48 + (sgid * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) + fifoid;

      /* Update a mask of system fifo bits to turn off */
      fifoSystemMask |= _BN(fifoBitNumber);

      /* Clear the start pointer to indicate the fifo is free */
      HWFifoPtr = (MUHWI_Fifo_t *) BGQ_MU_rDMA_START_OFFSET (gid,sgid,fifoid);

      HWFifoPtr->pa_start = 0ULL;
    }

  /* Reset the system rmFifo DCR bits to 1s */
  if ( fifoSystemMask )
    {
      muDCR_systemFifo_ptr = 
	(uint64_t *)(BGQ_MU_DCR_rDMA_SYSTEM_FIFO_OFFSET(gid));

/*       printf("Kernel_DeallocateRecFifos: Previous System Setting = 0x%lx\n",*(muDCR_systemFifo_ptr)); */

      dcr_value = *(muDCR_systemFifo_ptr) | fifoSystemMask;

/*       printf("Kernel_DeallocateRecFifos: New      System Setting = 0x%lx\n",dcr_value); */

      *(muDCR_systemFifo_ptr) = dcr_value;
    }

  return 0;
}


/**
 * \brief Deallocate Base Address Table
 *
 * Deallocate specified base address table entries in the specified SubGroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with allocations and deallocations
 * of base address table entries.  This can be done by obtaining a node-scoped
 * lock, if necessary.
 *
 * \param[in]  sg_ptr   Pointer to Base Address Table SubGroup structure.
 * \param[in]  nbatids  Number of IDs to be deallocated (1 to
 *                      BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP).
 * \param[in]  batids   Pointer to an array of nfifos int32_t's where
 *                      the list of Base Address Table IDs to be deallocated
 *                      is provided.  Each int32_t is the Base Address Table ID
 *                      within the SubGroup
 *                      (0 to BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP-1).
 *
 * \retval  0   Successful.    All IDs were deallocated.
 * \retval  errno  Unsuccessful.
 *
 * \see Kernel_QueryBaseAddressTable
 * \see Kernel_AllocateBaseAddressTable
 */
__INLINE__
int32_t Kernel_DeallocateBaseAddressTable (MUSPI_BaseAddressTableSubGroup_t *sg_ptr,
					   uint32_t                          nbatids,
					   uint32_t                         *batids)
{
  uint32_t i;
  uint32_t numFree;
  uint32_t subgrpid;
  uint32_t freeBatIds[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];
  volatile uint64_t *muDCR_ptr;
  volatile uint64_t  dcr_value;

  MUSPI_assert ( batids != NULL);
  MUSPI_assert ( sg_ptr != NULL);
  MUSPI_assert ( nbatids  <= BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
  MUSPI_assert ( nbatids  >  0 );
  
  /* Turn off the permission bits in the subgroup structure and
   * clear the BAT slot.
   */
  for ( i=0; i<nbatids; i++ )
  {
    MUSPI_assert ( batids[i] < BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP );
    sg_ptr->permissions       &= ~(_BN(batids[i])>>56); /* Shift 56 to convert
                   					     64-bit mask to
							     8-bit mask */
    sg_ptr->sw_bat[batids[i]]  = 0;
    sg_ptr->hw_bat[batids[i]]  = 0x1FFFFFFFFF;
  }

  /* After deallocating the requested BAT IDs, 
   * - if there are still BAT IDs allocated in this subgroup, leave the "user" / "system"
   *   DCR setting for this subgroup unchanged.
   * - if there are no more BAT IDs allocated in this subgroup, reset the "user" / "system"
   *   DCR bit to the HW reset value (on = "system").
   */
  subgrpid = sg_ptr->subgrpid;

  Kernel_QueryBaseAddressTable (subgrpid,
				&numFree,
				freeBatIds);

  if ( numFree == BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP ) /* No BAT IDs allocated? */
    {
      /* Set the MU DCR to the reset value */
      if ( subgrpid < ( (BGQ_MU_NUM_FIFO_GROUPS-1)*BGQ_MU_NUM_FIFO_SUBGROUPS ) )
	{
	  muDCR_ptr = 
	    (uint64_t *)(BGQ_MU_DCR_BAT_MODE0_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));

/* 	  printf("Kernel_DeallocateBaseAddressTable: Previous BAT DCR = 0x%lx\n",*(muDCR_ptr)); */
	  
	  dcr_value = *(muDCR_ptr) | _BN(subgrpid);

/* 	  printf("Kernel_DeallocateBaseAddressTable: New      BAT DCR = 0x%lx\n",dcr_value); */
	  
	  *(muDCR_ptr) = dcr_value;
	}
      else
	{
	  muDCR_ptr = 
	    (uint64_t *)(BGQ_MU_DCR_BAT_MODE1_OFFSET(subgrpid/BGQ_MU_NUM_FIFO_SUBGROUPS));

/* 	  printf("Kernel_DeallocateBaseAddressTable: Previous BAT DCR = 0x%lx\n",*(muDCR_ptr)); */
	  
	  dcr_value = *(muDCR_ptr) | (_BN(subgrpid - (BGQ_MU_NUM_FIFO_GROUPS-1)*BGQ_MU_NUM_FIFO_SUBGROUPS ));

/* 	  printf("Kernel_DeallocateBaseAddressTable: New      BAT DCR = 0x%lx\n",dcr_value); */
      
	  *(muDCR_ptr) = dcr_value;
	}
    }

  return 0;
}


/**
 * \brief Configure the MU GEA for a Subgroup
 *
 * Configure the MU Global Event Aggregator for a subgroup.
 *
 * The caller of this function is responsible for ensuring that this is an
 * atomic operation that does not conflict with configuring the MU GEA
 * for other subgroups.  This can be done by obtaining a node-scoped lock, if necessary.
 *
 * \param[in]   subgrpid  SubGroup being configured (0 to 
 *                        BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1).
 * \param[in]   gea_flag  Global Event Aggregation flag.  Indicates whether
 *                        interrupts in this subgroup participate in raising
 *                        one of the interrupt lines connected to the
 *                        global event aggregator unit.
 *                        - KERNEL_SUBGROUP_INTERRUPTS_GO_TO_GEA means "yes".
 *                        - KERNEL_SUBGROUP_INTERRUPTS_DO_NOT_GO_TO_GEA means "no".
 *
 * \retval  0  Successful.
 * \retval  errno Unsuccessful.
 *
 */
__INLINE__
int32_t Kernel_Configure_MU_GEA(uint32_t subgrpid,
				uint64_t gea_flag)
{
  int gid = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;

  volatile uint64_t *muDCR_gea_ptr;
  volatile uint64_t dcr_value;
  uint64_t gea_bit = 0;
  
  /* There are 3 DCRs, depending on the group number */
  if ( gid <= 7 )
    {
      muDCR_gea_ptr =
	(uint64_t *)(BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_0_7_OFFSET());
      gea_bit = _BN(32 + subgrpid);
    }
  else
    if ( gid <= 15 )
      {
	muDCR_gea_ptr =
	  (uint64_t *)(BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_8_15_OFFSET());
	gea_bit = _BN(32 + (subgrpid-32));
      }
    else
      {
	muDCR_gea_ptr =
	  (uint64_t *)(BGQ_MU_DCR_GEA_INTERRUPT_ENABLE_16_OFFSET());
	gea_bit = _BN(60 + (subgrpid-32-32));
      }
  /*       printf("Setting GEA DCR %p bit 0x%llx\n",muDCR_gea_ptr,(long long unsigned int)gea_bit);       */

  if ( gea_flag == KERNEL_SUBGROUP_INTERRUPTS_GO_TO_GEA )
    {
      dcr_value = *(muDCR_gea_ptr) | gea_bit; /* Turn on the GEA bit in the appropriate
				                 GEA DCR. */
    }
  else
    {
      dcr_value = *(muDCR_gea_ptr) & ~gea_bit; /* Turn off the GEA bit in the appropriate
				                  GEA DCR. */
    }

  *(muDCR_gea_ptr) = dcr_value;

/*       printf("GEA DCR value is now 0x%llx\n",(long long unsigned int)*(muDCR_gea_ptr));       */

  return 0;
}



#endif /* _KERNEL_KLINUX_MU_IMPL_H_ */
