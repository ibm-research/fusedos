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


#include "Kernel.h"

#include <spi/include/kernel/MU.h>




uint64_t sc_QUERY_REC_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                     subgrpid  = (uint32_t)                    r3;
  uint32_t*                    nfifos    = (uint32_t*)                   r4;
  uint32_t*                    fifoids   = (uint32_t*)                   r5;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, nfifos=%p, fifoids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 nfifos,
		 fifoids
		 ));


  if ( ! VMM_IsAppAddress( nfifos,    sizeof(uint32_t) ) ||
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_QueryRecFifos( subgrpid, nfifos, fifoids );
  return CNK_RC_SPI(rc);
}

uint64_t sc_QUERY_BASE_ADDRESS_TABLE( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                     subgrpid  = (uint32_t)                    r3;
  uint32_t*                    nbatids   = (uint32_t*)                   r4;
  uint32_t*                    batids    = (uint32_t*)                   r5;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, nbatids=%p, batids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 nbatids,
		 batids
		 ));


  if ( ! VMM_IsAppAddress( nbatids,  sizeof(uint32_t) ) ||
       ! VMM_IsAppAddress( batids,   sizeof(uint32_t) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_QueryBaseAddressTable( subgrpid, nbatids, batids );
  return CNK_RC_SPI(rc);
}

uint64_t sc_QUERY_INJ_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                     subgrpid  = (uint32_t)                    r3;
  uint32_t*                    nfifos    = (uint32_t*)                   r4;
  uint32_t*                    fifoids   = (uint32_t*)                   r5;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, nfifos=%p, fifoids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 nfifos,
		 fifoids
		 ));


  if ( ! VMM_IsAppAddress( nfifos,    sizeof(uint32_t) ) ||
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_QueryInjFifos( subgrpid, nfifos, fifoids );
  return CNK_RC_SPI(rc);
}


uint64_t sc_ALLOCATE_INJ_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                     subgrpid  = (uint32_t)                    r3;
  MUSPI_InjFifoSubGroup_t*     sg_ptr    = (MUSPI_InjFifoSubGroup_t*)    r4;
  uint32_t                     nfifos    = (uint32_t)                    r5;
  uint32_t*                    fifoids   = (uint32_t*)                   r6;
  Kernel_InjFifoAttributes_t*  fifoAttrs = (Kernel_InjFifoAttributes_t*) r7;
  uint32_t i;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, sg_ptr=%p, nfifos=%d, fifoids=%p, fifoAttrs=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 sg_ptr,
		 nfifos,
		 fifoids,
		 fifoAttrs
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_InjFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) ||
       ! VMM_IsAppAddress( fifoAttrs, sizeof(Kernel_InjFifoAttributes_t) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  // Verify that the user-mode caller is not trying to allocate fifos for "system" use.
  for ( i=0; i<nfifos; i++ )
    {
      if ( fifoAttrs[i].System == 1 ) return CNK_RC_SPI(EACCES);
    }

  rc = Kernel_AllocateInjFifos( subgrpid, sg_ptr, nfifos, fifoids, fifoAttrs );
  
  return CNK_RC_SPI(rc);
}


uint64_t sc_ALLOCATE_REC_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                     subgrpid  = (uint32_t)                    r3;
  MUSPI_RecFifoSubGroup_t*     sg_ptr    = (MUSPI_RecFifoSubGroup_t*)    r4;
  uint32_t                     nfifos    = (uint32_t)                    r5;
  uint32_t*                    fifoids   = (uint32_t*)                   r6;
  Kernel_RecFifoAttributes_t*  fifoAttrs = (Kernel_RecFifoAttributes_t*) r7;
  uint32_t i;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, sg_ptr=%p, nfifos=%d, fifoids=%p, fifoAttrs=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 sg_ptr,
		 nfifos,
		 fifoids,
		 fifoAttrs
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_RecFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) ||
       ! VMM_IsAppAddress( fifoAttrs, sizeof(Kernel_RecFifoAttributes_t) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  // Verify that the user-mode caller is not trying to allocate fifos for "system" use.
  for ( i=0; i<nfifos; i++ )
    {
      if ( fifoAttrs[i].System == 1 ) return CNK_RC_SPI(EACCES);
    }

  rc = Kernel_AllocateRecFifos( subgrpid, sg_ptr, nfifos, fifoids, fifoAttrs );
  
  return CNK_RC_SPI(rc);
}


uint64_t sc_ALLOCATE_BASE_ADDRESS_TABLE( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                           subgrpid  = (uint32_t)                          r3;
  MUSPI_BaseAddressTableSubGroup_t*  sg_ptr    = (MUSPI_BaseAddressTableSubGroup_t*) r4;
  uint32_t                           nbatids   = (uint32_t)                          r5;
  uint32_t*                          batids    = (uint32_t*)                         r6;
  uint32_t                           system    = (uint32_t)                          r7;


  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%d, sg_ptr=%p, nbatids=%d, batids=%p, system=%d.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 sg_ptr,
		 nbatids,
		 batids,
		 system
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_BaseAddressTableSubGroup_t)    ) || 
       ! VMM_IsAppAddress( batids,    sizeof(uint32_t)                   )  )

    {
      return CNK_RC_SPI(EFAULT);
    }

  // Verify that the user-mode caller is not trying to allocate BAT IDs for "system" use.
  if ( system == 1 ) return CNK_RC_SPI(EACCES);

  rc = Kernel_AllocateBaseAddressTable( subgrpid, sg_ptr, nbatids, batids, system );
  
  return CNK_RC_SPI(rc);
}



uint64_t sc_REC_FIFO_ENABLE( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t    groupid     = (uint32_t)  r3;
  uint64_t    enableBits  = (uint64_t)  r4;

  TRACESYSCALL(( "(I) %s[%d]: groupid=%d, enableBitsr=%016lX.\n", 
		 __func__, 
		 ProcessorID(),
		 groupid,
		 enableBits
		 ));

  rc = Kernel_RecFifoEnable( groupid, enableBits );
  
  return CNK_RC_SPI(rc);
}

uint64_t sc_CFG_INJ_FIFO_INTS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  MUSPI_InjFifoSubGroup_t*    sg_ptr            = (MUSPI_InjFifoSubGroup_t*)          r3;
  uint32_t                    nfifos            = (uint32_t)                          r4;
  uint32_t*                   fifoids           = (uint32_t*)                         r5;
  Kernel_InjFifoInterrupts_t* fifoInterrupts    = (Kernel_InjFifoInterrupts_t*)       r6;


  TRACESYSCALL(( "(I) %s[%d]: sg_ptr=%p, nfifos=%d, fifoids=%p, fifoInterrupts=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 sg_ptr,
		 nfifos,
		 fifoids,
		 fifoInterrupts
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_InjFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) ||
       ! VMM_IsAppAddress( fifoInterrupts, sizeof(Kernel_InjFifoInterrupts_t) ) )

    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_ConfigureInjFifoInterrupts( sg_ptr, nfifos, fifoids, fifoInterrupts );
  
  return CNK_RC_SPI(rc);
}


uint64_t sc_CFG_REC_FIFO_INTS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  MUSPI_RecFifoSubGroup_t*    sg_ptr            = (MUSPI_RecFifoSubGroup_t*)          r3;
  uint32_t                    nfifos            = (uint32_t)                          r4;
  uint32_t*                   fifoids           = (uint32_t*)                         r5;
  Kernel_RecFifoInterrupts_t* fifoInterrupts    = (Kernel_RecFifoInterrupts_t*)       r6;


  TRACESYSCALL(( "(I) %s[%d]: sg_ptr=%p, nfifos=%d, fifoids=%p, fifoInterrupts=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 sg_ptr,
		 nfifos,
		 fifoids,
		 fifoInterrupts
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_RecFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) ||
       ! VMM_IsAppAddress( fifoInterrupts, sizeof(Kernel_RecFifoInterrupts_t) ) )

    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_ConfigureRecFifoInterrupts( sg_ptr, nfifos, fifoids, fifoInterrupts );
  
  return CNK_RC_SPI(rc);
}


uint64_t sc_CFG_INJ_FIFO_THRESH( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint64_t*   normalThreshold      = (uint64_t*)  r3;
  uint64_t*   remoteGetThreshold   = (uint64_t*)  r4;


  TRACESYSCALL(( "(I) %s[%d]: normalThreshold=%p, remoteGetThreshold=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 normalThreshold,
		 remoteGetThreshold
		 ));
	  

  if ( ( ( normalThreshold    != 0 ) && ( ! VMM_IsAppAddress( normalThreshold, sizeof(uint64_t) ) ) ) ||
       ( ( remoteGetThreshold != 0 ) && ( ! VMM_IsAppAddress( remoteGetThreshold, sizeof(uint64_t) ) ) ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_ConfigureInjFifoThresholds( normalThreshold, remoteGetThreshold );
  
  return CNK_RC_SPI(rc);
}


uint64_t sc_CFG_REC_FIFO_THRESH( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint64_t  threshold      = (uint64_t)  r3;


  TRACESYSCALL(( "(I) %s[%d]: threshold=%ld.\n",
		 __func__, 
		 ProcessorID(),
		 threshold
		 ));
	  


  rc = Kernel_ConfigureRecFifoThreshold( threshold );
  
  return CNK_RC_SPI(rc);
}

uint64_t sc_DEALLOC_INJ_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  MUSPI_InjFifoSubGroup_t*     sg_ptr    = (MUSPI_InjFifoSubGroup_t*)    r3;
  uint32_t                     nfifos    = (uint32_t)                    r4;
  uint32_t*                    fifoids   = (uint32_t*)                   r5;

  TRACESYSCALL(( "(I) %s[%d]: sg_ptr=%p, nfifos=%d, fifoids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 sg_ptr,
		 nfifos,
		 fifoids
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_InjFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_DeallocateInjFifos( sg_ptr, nfifos, fifoids );
  
  return CNK_RC_SPI(rc);
}

uint64_t sc_DEALLOC_REC_FIFOS( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  MUSPI_RecFifoSubGroup_t*     sg_ptr    = (MUSPI_RecFifoSubGroup_t*)    r3;
  uint32_t                     nfifos    = (uint32_t)                    r4;
  uint32_t*                    fifoids   = (uint32_t*)                   r5;

  TRACESYSCALL(( "(I) %s[%d]: sg_ptr=%p, nfifos=%d, fifoids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 sg_ptr,
		 nfifos,
		 fifoids
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_RecFifoSubGroup_t)    ) || 
       ! VMM_IsAppAddress( fifoids,   sizeof(uint32_t)                   ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_DeallocateRecFifos( sg_ptr, nfifos, fifoids );
  
  return CNK_RC_SPI(rc);
}

uint64_t sc_DEALLOC_BASE_ADDR_TABLE( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  MUSPI_BaseAddressTableSubGroup_t*     sg_ptr    = (MUSPI_BaseAddressTableSubGroup_t*)  r3;
  uint32_t                              nbatids   = (uint32_t)                           r4;
  uint32_t*                             batids    = (uint32_t*)                          r5;

  TRACESYSCALL(( "(I) %s[%d]: sg_ptr=%p, nbatids=%d, batids=%p.\n", 
		 __func__, 
		 ProcessorID(),
		 sg_ptr,
		 nbatids,
		 batids
		 ));


  if ( ! VMM_IsAppAddress( sg_ptr,    sizeof(MUSPI_BaseAddressTableSubGroup_t)    ) || 
       ! VMM_IsAppAddress( batids,   sizeof(uint32_t)                   ) )
    {
      return CNK_RC_SPI(EFAULT);
    }

  rc = Kernel_DeallocateBaseAddressTable( sg_ptr, nbatids, batids );
  
  return CNK_RC_SPI(rc);
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
uint64_t sc_CFG_MU_GEA( SYSCALL_FCN_ARGS ) {

  int32_t rc = -1;
  uint32_t                              subgrpid   = (uint32_t)  r3;
  uint64_t                              gea_flag   = (uint64_t)  r4;

  TRACESYSCALL(( "(I) %s[%d]: subgrpid=%u, gea_flag=%ld.\n", 
		 __func__, 
		 ProcessorID(),
		 subgrpid,
		 gea_flag
		 ));

  rc = Kernel_Configure_MU_GEA( subgrpid, gea_flag );
  
  return CNK_RC_SPI(rc);
}

#endif                                                                                                                 // FUSEDOS
