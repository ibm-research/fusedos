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

#ifndef	_KERNEL_CNK_MU_IMPL_H_ /* Prevent multiple inclusion */
#define	_KERNEL_CNK_MU_IMPL_H_

#include "cnk/include/SPI_syscalls.h"


/**
 * \file cnk/MU_impl.h
 *
 * \brief C Header File containing Compute Node Kernel (CNK) Message Unit SPI
 *        implementations.
 *
 */


__INLINE__ int32_t Kernel_QueryInjFifos (uint32_t   subgrpid,
					 uint32_t * nfifos,
					 uint32_t * fifoids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(QUERY_INJ_FIFOS, subgrpid, nfifos, fifoids);
}





__INLINE__ int32_t Kernel_QueryRecFifos (uint32_t   subgrpid,
					 uint32_t * nfifos,
					 uint32_t * fifoids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(QUERY_REC_FIFOS, subgrpid, nfifos, fifoids);
}


__INLINE__ int32_t Kernel_QueryBaseAddressTable (uint32_t   subgrpid,
						 uint32_t * nbatids,
						 uint32_t * batids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(QUERY_BASE_ADDRESS_TABLE, subgrpid, nbatids, batids);
}



__INLINE__ int32_t Kernel_AllocateInjFifos (uint32_t                     subgrpid,
					    MUSPI_InjFifoSubGroup_t    * sg_ptr,
					    uint32_t                     nfifos,
					    uint32_t                   * fifoids,
					    Kernel_InjFifoAttributes_t * fifoAttrs)
{
    return (int32_t) CNK_SPI_SYSCALL_5(ALLOCATE_INJ_FIFOS, subgrpid, sg_ptr, nfifos, fifoids, fifoAttrs);
}


__INLINE__ int32_t Kernel_AllocateRecFifos (uint32_t                     subgrpid,
					    MUSPI_RecFifoSubGroup_t    * sg_ptr,
					    uint32_t                     nfifos,
					    uint32_t                   * fifoids,
					    Kernel_RecFifoAttributes_t * fifoAttrs)
{
    return (int32_t) CNK_SPI_SYSCALL_5(ALLOCATE_REC_FIFOS, subgrpid, sg_ptr, nfifos, fifoids, fifoAttrs);
}




__INLINE__ int32_t Kernel_AllocateBaseAddressTable (uint32_t                           subgrpid,
						    MUSPI_BaseAddressTableSubGroup_t * sg_ptr,
						    uint32_t                           nbatids,
						    uint32_t                         * batids,
						    uint32_t                           system)
{
    return (int32_t) CNK_SPI_SYSCALL_5(ALLOCATE_BASE_ADDRESS_TABLE, subgrpid, sg_ptr, nbatids, batids, system);

}

__INLINE__ int32_t Kernel_RecFifoEnable (  uint32_t groupid,
					   uint64_t enableBits )
{
    return (int32_t) CNK_SPI_SYSCALL_2(REC_FIFO_ENABLE, groupid, enableBits);
}


__INLINE__
int32_t Kernel_RecFifoDisable (  uint32_t groupid,
				 uint64_t disableBits )
{
  // \todo Add a syscall here
  //     return (int32_t) CNK_SPI_SYSCALL_2(REC_FIFO_DISABLE, groupid, disableBits);
  return 0;
}

__INLINE__ int32_t Kernel_ConfigureInjFifoInterrupts (MUSPI_InjFifoSubGroup_t    * sg_ptr,
						      uint32_t                     nfifos,
						      uint32_t                   * fifoids,
						      Kernel_InjFifoInterrupts_t * fifoInterrupts)
{
    return (int32_t) CNK_SPI_SYSCALL_4(CFG_INJ_FIFO_INTS, sg_ptr, nfifos, fifoids, fifoInterrupts);
}





__INLINE__ int32_t Kernel_ConfigureRecFifoInterrupts (MUSPI_RecFifoSubGroup_t    * sg_ptr,
						      uint32_t                     nfifos,
						      uint32_t                   * fifoids,
						      Kernel_RecFifoInterrupts_t * fifoInterrupts)
{
    return (int32_t) CNK_SPI_SYSCALL_4(CFG_REC_FIFO_INTS, sg_ptr, nfifos, fifoids, fifoInterrupts);
}


__INLINE__ int32_t Kernel_ConfigureInjFifoThresholds( uint64_t * normalThreshold,
						      uint64_t * remoteGetThreshold )
{
    return (int32_t) CNK_SPI_SYSCALL_2(CFG_INJ_FIFO_THRESH, normalThreshold, remoteGetThreshold);
}


__INLINE__ int32_t Kernel_ConfigureRecFifoThreshold( uint64_t threshold )
{
    return (int32_t) CNK_SPI_SYSCALL_1(CFG_REC_FIFO_THRESH, threshold);
}




__INLINE__ int32_t Kernel_DeallocateInjFifos (MUSPI_InjFifoSubGroup_t * sg_ptr,
					      uint32_t                  nfifos,
					      uint32_t                * fifoids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(DEALLOC_INJ_FIFOS, sg_ptr, nfifos, fifoids);
}


__INLINE__ int32_t Kernel_DeallocateRecFifos (MUSPI_RecFifoSubGroup_t * sg_ptr,
					      uint32_t                  nfifos,
					      uint32_t                * fifoids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(DEALLOC_REC_FIFOS, sg_ptr, nfifos, fifoids);
}


__INLINE__ int32_t Kernel_DeallocateBaseAddressTable (MUSPI_BaseAddressTableSubGroup_t *sg_ptr,
						      uint32_t                          nbatids,
						      uint32_t                         *batids)
{
    return (int32_t) CNK_SPI_SYSCALL_3(DEALLOC_BASE_ADDR_TABLE, sg_ptr, nbatids, batids);
}


__INLINE__ int32_t Kernel_Configure_MU_GEA(uint32_t subgrpid,
					   uint64_t gea_flag)
{
    return (int32_t) CNK_SPI_SYSCALL_2(CFG_MU_GEA, subgrpid, gea_flag);
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
  // not implemented as a syscall
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

  // not implemented as a syscall
  return ENOSYS;
}  


#endif /* _KERNEL_CNK_MU_IMPL_H_ */
