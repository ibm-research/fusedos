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

#ifndef  _KERNEL_CL_MU_IMPL_H_
#define  _KERNEL_CL_MU_IMPL_H_

// CL-specific includes
#include "cl_debug.h"
#include "cl_mu.h" // CL-specific support code for interfacing MUFS
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/MU_Fifo.h>
//#include <hwi/include/bqc/nd_rese_dcr.h>
//#include <hwi/include/bqc/testint_inlines.h>
#include <spi/include/kernel/MU.h>


inline int open_subgroup_file(uint32_t subgrpid, const char * file, int flags)
{
    char filename[50];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/mu/subgrp_%02d/%s", subgrpid, file);

    return open(filename, flags);
}


inline int open_fifo_file(uint32_t subgrpid, uint32_t fifo_idx_in_subgrp, char * fifotype, int flags)
{
    char filename[50];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/mu/subgrp_%02u/%sFifo_%02u", subgrpid, fifotype, fifo_idx_in_subgrp);

    //CL_DEBUG("%s: pathname prepared %s\n", __func__, filename);
    return open(filename, flags);
}

inline int open_bat_file(uint32_t subgrpid, uint32_t bat_idx_in_subgrp, int flags)
{
    char filename[50];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename), "/mu/subgrp_%02u/bat_%02u", subgrpid, bat_idx_in_subgrp);

    //CL_DEBUG("%s: pathname prepared %s\n", __func__, filename);
    return open(filename, flags);
}


/**
 * \file MU_impl.h
 *
 * \brief C Header File containing Message Unit SPI
 *        implementations for within CL.
 *        These implementations wrap SPI calls to MUFS operations
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
    int query_fd;

    if(subgrpid >= BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE)
        return EINVAL;

    query_fd = open_subgroup_file(subgrpid, "injFifo_query", O_RDONLY);

    if(query_fd == -1) {
        CL_ERROR("error %d opening injFifo_query file in subgroup %d: %s\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    rc = read(query_fd, fifoids, BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP * sizeof(uint32_t));
    if(rc == -1) {
        CL_ERROR("error %d reading injFifo_query file in subgroup %d: %s.\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    // read delivers all unallocated FIFO ids -> number of bytes read
    // indicates the number of unallocated FIFOs
    *nfifos = rc / sizeof(uint32_t);

    //CL_DEBUG("%s: read %u bytes / %u FIFO ids\n", __func__, rc, *nfifos);

    return 0;
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
    int query_fd;

    if(subgrpid >= BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE)
        return EINVAL;

    query_fd = open_subgroup_file(subgrpid, "recFifo_query", O_RDONLY);

    if(query_fd == -1) {
        CL_ERROR("error %d opening recFifo_query file in subgroup %d: %s\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    rc = read(query_fd, fifoids, BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP * sizeof(uint32_t));
    if(rc == -1) {
        CL_ERROR("error %d reading recFifo_query file in subgroup %d: %s.\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    // read delivers all unallocated FIFO ids -> number of bytes read
    // indicates the number of unallocated FIFOs
    *nfifos = rc / sizeof(uint32_t);

    return 0;
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
    int query_fd;

    if(subgrpid >= BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE)
        return EINVAL;

    query_fd = open_subgroup_file(subgrpid, "bat_query", O_RDONLY);

    if(query_fd == -1) {
        CL_ERROR("error %d opening bat_query file in subgroup %d: %s\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    rc = read(query_fd, batids, BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP * sizeof(uint32_t));
    if(rc == -1) {
        CL_ERROR("error %d reading bat_query file in subgroup %d: %s.\n", errno, subgrpid, strerror(errno));
        return EIO;
    }

    // read delivers all unallocated FIFO ids -> number of bytes read
    // indicates the number of unallocated FIFOs
    *nbatids = rc / sizeof(uint32_t);

    return 0;
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
    int32_t  rc  = 0;
    int      i   = 0;
    uint32_t fifoid;
    int      gid, sgid;

    CL_DEBUG("%s: enter\n", __func__);

    // check parameters
    if( ! (subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE ) )
        return EINVAL;
    if( ! (nfifos <= BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP) )
        return EINVAL;

    if( sg_ptr == NULL || fifoids == NULL || fifoAttrs == NULL )
        return EINVAL;

    for ( i = 0; i < (int)nfifos; i++ )
        if ( fifoAttrs[i].System == 1 ) return EINVAL;

    // check if requested FIFOs are free -> early abort
    // [ left out; we eagerly go on allocating FIFOs, aborting when we
    //   find one occupied -> avoid redundant checking code; a check
    //   happens in allocation code anyway; deadlocks cannot occur, as
    //   allocate requests for occupied FIFOs do not block. ]

    // initialize output struct for user
    //  * zero out
    memset(sg_ptr, 0, sizeof(MUSPI_InjFifoSubGroup_t));

    //  * subgroup_id
    sg_ptr->subgroupid = subgrpid;
    gid  = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
    sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

    /* Set the group status MU MMIO pointer.  If any fifos are "system", then this is
     * a privileged address, otherwise it is a non-privileged address.
     */
    sg_ptr->groupStatus = (MUHWI_InjFifoStatus_t *)
                          ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(gid,sgid) & ~PHYMAP_PRIVILEGEDOFFSET);

    //  allocate FIFOs, open()+write() to MUFS
    //  * if one fails, rollback all open FIFOs/files

    for( i=0; i < (int)nfifos; i++ ) {
        fifoid = fifoids[i];

        int fd = open_fifo_file( subgrpid, fifoid, (char*)"inj", O_RDWR);
        set_inj_fifo_fd( subgrpid, fifoid, fd);

        // if open() succeeded, continue with allocation process
        if( fd != -1 ) {

            // second stage: write() FIFO attributes, causing MUFS to
            // perform actual allocation
            if( write( fd, fifoAttrs, sizeof(Kernel_InjFifoAttributes_t)) != sizeof(Kernel_InjFifoAttributes_t) ) {
                rc = errno;

                CL_DEBUG("error write()ing into FIFO file for subgroup %u injection FIFO %u for allocation: %d - %s\n",
                         subgrpid, fifoid, errno, strerror(errno));
                break;
            }
        } else {
            rc = errno;
            CL_DEBUG("error open()ing FIFO file for subgroup %u injection FIFO %u for allocation: %d - %s\n",
                     subgrpid, fifoid, errno, strerror(errno));
            break;
        }
    }

    // if allocation failed for one FIFO, roll previous allocations backwards
    //  for loop backwards
    if(rc) {
        for( ; i>=0; i--)
            if( have_inj_fifo_fd(subgrpid, fifoids[i]) )
                close( get_inj_fifo_fd(subgrpid, fifoids[i]) );
        return rc;
    }

    //  complete output struct for user, complete FIFO array entries
    /* Now that the user vs system state DCR has been set, determine the MU MMIO
     * address that will be used to access each fifo.
     */

    for (i = 0; i < (int)nfifos; i++) {
        fifoid = fifoids[i];

        sg_ptr->_injfifos[fifoid]._fifo.hwfifo =
            (MUHWI_Fifo_t *)( (BGQ_MU_iDMA_START_OFFSET (gid,sgid,fifoid)) & ~PHYMAP_PRIVILEGEDOFFSET );
        sg_ptr->_injfifos[fifoid].hw_injfifo =
            (MUHWI_InjFifo_t *)( (BGQ_MU_iDMA_DESC_COUNTER_OFFSET (gid,sgid,fifoid)) & ~PHYMAP_PRIVILEGEDOFFSET );
    }

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
    int32_t  rc  = 0;
    uint32_t i   = 0;
    uint32_t fifoid;
    int      gid, sgid;

    // check parameters
    if( ! (subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE ) )
        return EINVAL;
    if( ! (nfifos <= BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) )
        return EINVAL;

    if( sg_ptr == NULL || fifoids == NULL || fifoAttrs == NULL )
        return -EINVAL;
    for ( i = 0; i < nfifos; i++ )
        if ( fifoAttrs[i].System == 1 ) return EINVAL;

    // check if requested FIFOs are free -> early abort
    // [ left out; we eagerly go on allocating FIFOs, aborting when we
    //   find one occupied -> avoid redundant checking code; a check
    //   happens in allocation code anyway; deadlocks cannot occur, as
    //   allocate requests for occupied FIFOs do not block. ]

    // initialize output struct for user
    //  * zero out
    memset(sg_ptr, 0, sizeof(MUSPI_RecFifoSubGroup_t));

    //  * subgroup_id
    sg_ptr->subgroupid = subgrpid;
    gid  = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
    sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

    /* Set the group status MU MMIO pointer.  If any fifos are "system", then this is
     * a privileged address, otherwise it is a non-privileged address.
     */
    sg_ptr->groupStatus = (MUHWI_RecFifoStatus_t *)
                          ( BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(gid, sgid) & ~PHYMAP_PRIVILEGEDOFFSET);

    //  allocate FIFOs, open() to MUFS
    //  * if one fails, rollback all open FIFOs/files
    for( i=0; i < nfifos; i++ ) {
        fifoid = fifoids[i];

        int fd = open_fifo_file( subgrpid, fifoid, (char *)"rec", O_RDWR);
        set_rec_fifo_fd( subgrpid, fifoid, fd);

        if( fd == -1 ) {
            // break from for loop and start rollback
            rc = errno;
            CL_DEBUG("error open()ing FIFO file for subgroup %u reception FIFO %u for allocation: %d - %s\n",
                     subgrpid, fifoid, errno, strerror(errno));
            break;
        }
    }

    // if allocation failed for one FIFO, roll previous allocations backwards
    //  for loop backwards
    if(rc) {
        for( ; i>=0; i--)
            if( have_inj_fifo_fd(subgrpid, fifoids[i]) )
                close( get_inj_fifo_fd(subgrpid, fifoids[i]) );
        return rc;
    }

    //  complete output struct for user, complete FIFO array entries
    /* Now that the user vs system state DCR has been set, determine the MU MMIO
     * address that will be used to access each fifo.
     */
    for (i = 0; i < nfifos; i++) {
        fifoid = fifoids[i];

        sg_ptr->_recfifos[fifoid]._fifo.hwfifo =
            (MUHWI_Fifo_t *) ( BGQ_MU_rDMA_START_OFFSET (gid,sgid,fifoid) & ~PHYMAP_PRIVILEGEDOFFSET );
    }

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
    int32_t  rc  = 0;
    uint32_t i   = 0;
    uint32_t bat_id;
    int      gid, sgid;

    // check parameters
    if( ! (subgrpid < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE ) )
        return EINVAL;
    if( ! (nbatids <= BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP) )
        return EINVAL;

    if( sg_ptr == NULL || batids == NULL )
        return -EINVAL;

    // initialize output struct for user
    //  * subgroup_id
    sg_ptr->subgrpid = subgrpid;
    gid  = subgrpid / BGQ_MU_NUM_FIFO_SUBGROUPS;
    sgid = subgrpid % BGQ_MU_NUM_FIFO_SUBGROUPS;

    sg_ptr->hw_bat      = (MUHWI_BaseAddressTable_t)
         ( BGQ_MU_DATA_COUNTER_BASE_ADDRESS_START_OFFSET ( gid, sgid ) & ~PHYMAP_PRIVILEGEDOFFSET );
    sg_ptr->permissions = 0;


    //  allocate BAT entries, open() to MUFS
    //  * if one fails, rollback all open BAT entries/files
    for( i=0; i < nbatids; i++ ) {
        bat_id = batids[i];

        int fd = open_bat_file( subgrpid, bat_id, O_RDWR);
        set_bat_fd( subgrpid, bat_id, fd);

        if( fd == -1 ) {
            // break from for loop and start rollback
            rc = errno;
            CL_DEBUG("error open()ing BAT entry file for subgroup %u BAT entry %u for allocation: %d - %s\n",
                     subgrpid, bat_id, errno, strerror(errno));
            break;
        }
    }

    // if allocation failed for one BAT entry, roll previous allocations backwards
    //  for loop backwards
    if(rc) {
        for( ; i>=0; i--)
            if( have_bat_fd(subgrpid, batids[i]) )
                close( get_bat_fd(subgrpid, batids[i]) );
        return rc;
    }

    //  complete output struct for user, complete FIFO array entries
    /* Now that the user vs system state DCR has been set, determine the MU MMIO
     * address that will be used to access each fifo.
     */
    for (i = 0; i < nbatids; i++) {
        bat_id = batids[i];

        sg_ptr->permissions |= _BN(batids[i]) >> 56;

        sg_ptr->sw_bat[batids[i]] = 0;
    }

    return rc;
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
    int     rc = 0;
    uint8_t b  = 1;

    int fifo_id;

    
    for(fifo_id = BGQ_MU_NUM_REC_FIFOS_PER_GROUP-1; fifo_id >= 0; fifo_id--, enableBits>>=1) {
        if( (enableBits & 0x1) && have_rec_fifo_fd_grp(groupid, fifo_id) ) {

            int fd = get_rec_fifo_fd_grp(groupid, fifo_id);
            CL_DEBUG("writing %u to fd %d, for enabling FIFO %u in group %u\n", (uint32_t) b, fd, fifo_id, groupid);
            if( write( fd, &b, 1 ) != 1 ) {
                // error indicating the enable operation to MUFS
                rc = errno;
                break;
            }
        }
    }

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
    CL_ERROR("TODO adapt to FusedOS (hey, there was no CNK syscall for disabling FIFOs before!)\n");
  // if necessary then just copy code from RecFifoEnable above (or,
  // better: move code to generic function and call it with the
  // respective flag)
    return ENOSYS;
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
    int rc;
    unsigned int i;
    int subgroupid = sg_ptr->subgroupid;
    char buf[5]; // discern the operation from the other write() to the inj FIFO file

    memset(buf, 0, sizeof(buf));

    for(i = 0; i < nfifos; i++) {
        if( !have_inj_fifo_fd(subgroupid, fifoids[i]) )
            return EINVAL;
    }

    for(i = 0; i < nfifos; i++) {
        memcpy(buf, &(fifoInterrupts[i]), sizeof(Kernel_InjFifoInterrupts_t));
        rc = write( get_inj_fifo_fd(subgroupid, fifoids[i]), buf, sizeof(buf) );
        if(rc != sizeof(buf))
            return errno;
    }

    return 0;
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
     int rc;
    unsigned int i;
    int subgroupid = sg_ptr->subgroupid;
    char buf[5]; // discern the operation from the other write() to the rec FIFO file

    memset(buf, 0, sizeof(buf));

    for(i = 0; i < nfifos; i++) {
        if( !have_rec_fifo_fd(subgroupid, fifoids[i]) )
            return EINVAL;
    }

    for(i = 0; i < nfifos; i++) {
        memcpy(buf, &(fifoInterrupts[i]), sizeof(Kernel_RecFifoInterrupts_t));
        rc = write( get_rec_fifo_fd(subgroupid, fifoids[i]), buf, sizeof(buf) );
        if(rc != sizeof(buf))
            return errno;
    }

    return 0;
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
    ssize_t rc;
    int fd;

    uint64_t thresholds[2];

    fd = open("/mu/inj_fifo_threshold", O_WRONLY);

    if(fd == -1)
        return EIO;

    thresholds[0] = *normalThreshold;
    thresholds[1] = *remoteGetThreshold;

    rc = write(fd, thresholds, sizeof(thresholds));

    if(rc != sizeof(thresholds)) {
        return errno;
    } else {
        return 0;
    }
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
    ssize_t rc;
    int fd;

    fd = open("/mu/rec_fifo_threshold", O_WRONLY);

    if(fd == -1)
        return EIO;

    rc = write(fd, &threshold, sizeof(threshold));

    if(rc != sizeof(threshold)) {
        return errno;
    } else {
        return 0;
    }
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
    int i;
    int rc = 0;

    // check parameters
    if( ! (nfifos <= BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP) )
        return EINVAL;

    for( i = 0; i < (int)nfifos; i++ ) {
        if( have_inj_fifo_fd( sg_ptr->subgroupid, fifoids[i] ) ) {
            rc = close( get_inj_fifo_fd( sg_ptr->subgroupid, fifoids[i] ) );
            if(rc)
                break;
        }
    }

    return rc;
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
    int i;
    int rc = 0;

    // check parameters
    if( ! (nfifos <= BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP) )
        return EINVAL;

    for( i = 0; i < (int)nfifos; i++ ) {
        if( have_rec_fifo_fd( sg_ptr->subgroupid, fifoids[i] ) ) {
            rc = close( get_rec_fifo_fd( sg_ptr->subgroupid, fifoids[i] ) );
            if(rc)
                break;
        }
    }

    return rc;
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
    int i;
    int rc = 0;

    // check parameters
    if( ! (nbatids <= BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP) )
        return EINVAL;

    for( i = 0; i < (int)nbatids; i++ ) {
        sg_ptr->permissions &= ~(_BN(batids[i])>>56);
        if( have_bat_fd( sg_ptr->subgrpid, batids[i] ) ) {
            rc = close( get_bat_fd( sg_ptr->subgrpid, batids[i] ) );
            if(rc)
                break;
        }
    }

    return rc;
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
    return ENOSYS;
}


/**
 * \brief Tell the network to drop packets that attempt to go over a link
 *
 * This is used for separating sub-block user partitions.  It can prevent
 * user (and/or system) packets from crossing from one sub-block job into another.
 * This is not meant to be called from user space
 *
 * \param[in]  link_id      The id of the link on which to drop packets,
 * \param[in]  drop_user    If non-zero, drop user packets
 * \param[in]  drop_system  If non-zero, drop system packets
 *
 */
__INLINE__
int32_t Kernel_DropPacketsOnLink (uint32_t  link_id,
                                  uint32_t  drop_user,
                                  uint32_t  drop_system)
{
    return ENOSYS;
}

/**
 * \brief Enable Sender Only and tell the network to drop packets that attempt to go over a link
 *
 * This can be used at the edge of mesh to detect and drop packets that request senders
 * at the mesh edge
 * This is not meant to be called from user space
 *
 * \param[in]  link_id      The id of the link on which to drop packets,
 * \param[in]  drop_user    If non-zero, drop user packets
 * \param[in]  drop_system  If non-zero, drop system packets
 *
 */
__INLINE__
int32_t Kernel_EnableSenderAndDropPacketsOnLink (uint32_t  link_id,
        uint32_t  drop_user,
        uint32_t  drop_system)

{
    return ENOSYS;
}



/**
 * \brief Gets the Expected number of Network Tokens that are programmed for a given link and VC
 *
 * if the return code is 0, token_ptr contains valid data
 * if non-zero, there is an error in either the input or the programming of the DCR
 * Warning, for vc !=0, the check is only valid on DD2 or higher
 * \param[in]  link_id      The id of the link
 * \param[in]  vc           The virtual channel
 * \param[out] token_ptr    pointer to the number of tokens for this link and VC
 *
 */
__INLINE__
int32_t Kernel_GetNDExpectedTokens (uint32_t  link_id,
                                    uint32_t  vc,
                                    uint32_t  *token_ptr)

{
    return ENOSYS;
}
/**
 * \brief Constructs the expected TOKENS DCR based on the token reduction DCRS
 *
 * if the return code is 0, token_ptr contains valid data
 * if non-zero, there is an error in either the input or the programming of the DCR
 * Warning, for vc !=0, the check is only valid on DD2 or higher
 * \param[in]  link_id      The id of the link
 * \param[out] token_ptr    pointer to the expected TOKENS DCR dcrs for this link
 *
 */
__INLINE__
int32_t Kernel_GetNDExpectedTokensDCR (uint32_t  link_id,
                                       uint64_t  *token_ptr)

{
    return ENOSYS;
}


/**
 * \brief Returns 1 if physical VC 4 on a link is used for UserCommWorld
 *
 * Internal Physical VC 4 storage can be used by either System Collective, or User Commworld
 * but not both,

 * if rc = -1, link_id is invalid,
 * if rc = -2, link is used for both system and user commworld, or class_type =3
 * if rc =  0, link is not used in any class map
 * if rc =  1, link is used by user_commworld
 * if rc =  2, link is used by system collective
  * \param[in]  link_id      The id of the link
 *
 */
__INLINE__
int32_t Kernel_NDWhoUsesVC4 (int32_t  link_id )

{
    return ENOSYS;
}




#endif /* _KERNEL_FIRMWARE_MU_IMPL_H_ */
