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

#ifndef  __MUSPI_INJ_FIFO__
#define  __MUSPI_INJ_FIFO__

/**
 * \file InjFifo.h
 *
 * \brief C Header File containing Injection Fifo Definitions
 */

#include "kernel_impl.h"
#include <hwi/include/bqc/MU_Descriptor.h>
#include <hwi/include/bqc/MU_Fifo.h>
#include <spi/include/mu/Fifo.h>
#include <spi/include/mu/Util.h>
#include <spi/include/mu/Descriptor.h>


__BEGIN_DECLS


/**
 * \brief Injection Fifo Structure
 *
 * The structure corresponding to an injection fifo.  It contains a generic fifo
 * structure, a pointer to additional injection fifo information in the hardware
 * SRAM, and other items.
 */
typedef struct MUSPI_InjFifo
{ 
  MUSPI_Fifo_t     _fifo;       /**< Fifo shadow structure */
  uint64_t         descCount;   /**< Shadow of the number of
				     descriptors in this fifo that have been
				     completed by the MU */
  uint64_t         freeSpace;   /**< Shadow of the freespace.  This is in units
                                     of number of 64-byte descriptors. */
  uint64_t         numInjected; /**< Software counter of the number of
				     descriptors injected.  After the
				     application has injected a descriptor,
				     this counter is updated.  Completion of 
                                     the message can be observed by comparing
				     this software counter with the hardware
				     descCount in SRAM (or the descCount shadow
				     in this structure). */
  MUHWI_InjFifo_t *hw_injfifo;  /**< Pointer to additional injfifo in SRAM */
} MUSPI_InjFifo_t;


/**
 * \brief Injection Fifo Subgroup
 *
 * This structure defines one Injection Fifo Subgroup.  It consists of an
 * array of injection fifos, a pointer to injection fifo status, and the
 * ID of the subgroup.
 */
typedef struct  MUSPI_InjFifoSubGroup { 
  MUSPI_InjFifo_t         _injfifos[BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP]; /**< 
					     Fifos in this subgroup */
  MUHWI_InjFifoStatus_t *  groupStatus; /**< SRAM virtual address for status */
  
  uint32_t                 subgroupid;  /**< Subgroup Id from 0 to
					     (BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1). */
} MUSPI_InjFifoSubGroup_t;


/**
 * \brief Get the Freespace from the Shadow
 *
 * Returns the number of 64-byte descriptor slots that are free in the
 * injection fifo from the shadow.
 *
 * \param[in]  ififo  Pointer to the injection fifo shadow structure
 *
 * \returns Number of 64-byte descriptor slots that are free
 */
__INLINE__ uint64_t MUSPI_getFreeSpaceFromShadow (MUSPI_InjFifo_t  *ififo) 
{
  return ififo->freeSpace;
}


/**
 * \brief Get the Descriptor Count from the Shadow
 *
 * Returns the count of the number of descriptors in the specified fifo that
 * have been completed by the MU since the node was started, from the shadow.
 *
 * \param[in] ififo  Pointer to the injection fifo shadow structure
 *
 * \returns Descriptor count from the shadow
 */
__INLINE__ uint64_t MUSPI_getDescCountFromShadow (MUSPI_InjFifo_t  *ififo) 
{
  return ififo->descCount;
}


/**
 * \brief Get the Software Number of Descriptors Injected Counter
 *
 * Software keeps a count of the number of descriptors that have been
 * injected since the node started.
 *
 * \param[in]  ififo  Pointer to the injection fifo shadow structure
 *
 * \returns Number of descriptors injected
 */
__INLINE__ uint64_t MUSPI_getNumInjected (MUSPI_InjFifo_t  *ififo) 
{
  return ififo->numInjected;
}


/**
 * \brief Get the Freespace from the Hardware
 *
 * Returns the number of 64-byte descriptor slots that are free in the
 * injection fifo from the MU SRAM hardware.
 *
 * \param[in]  ififo  Pointer to the injection fifo shadow structure
 *
 * \returns Number of 64-byte descriptor slots that are free
 */
__INLINE__ uint64_t MUSPI_getHwFreeSpace (MUSPI_InjFifo_t  *ififo) 
{
/*    printf("Getting HW free space from %p, value=0x%llx\n",  */
/*  	 &(ififo->hw_injfifo->freeSpace),  */
/*  	 (long long unsigned int)ififo->hw_injfifo->freeSpace);  */
  return ififo->hw_injfifo->freeSpace;
}


/**
 * \brief Get the Descriptor Count from the Hardware
 *
 * Returns the count of the number of descriptors in the specified fifo that
 * have been completed by the MU since the node was started, from the MU
 * SRAM hardware.
 *
 * \param[in] ififo  Pointer to the injection fifo shadow structure
 *
 * \returns Descriptor count from the MU SRAM hardware
 */
__INLINE__ uint64_t MUSPI_getHwDescCount (MUSPI_InjFifo_t  *ififo) 
{
  return ififo->hw_injfifo->descCount;
}


/**
 * \brief Retrieve the Address of the Next Descriptor in the Injection Fifo
 *
 * Returns a pointer to the next available slot in the specified injection
 * fifo.  The caller can then initialize a descriptor directly in the 
 * injection fifo, and afterwards, invoke the MUSPI_InjFifoAdvanceDesc()
 * function to advance the injection fifo tail pointer to send the
 * descriptor.
 *
 * \see MUSPI_InjFifoAdvanceDesc
 *
 * \param[in]  injfifo  Pointer to the injection fifo shadow structure
 * \param[out] desc     Pointer to the next injection descriptor
 *
 * \return Descriptor number of the next descriptor
 * \retval -1 Injection fifo is full
 */
__INLINE__ uint64_t MUSPI_InjFifoNextDesc (MUSPI_InjFifo_t   * injfifo,
					   void             ** desc) 
{
/*   printf ("Shadow Free space available %llx, size 0x%llx\n",  */
/* 	  (long long unsigned int)injfifo->freeSpace, */
/* 	  (long long unsigned int)injfifo->_fifo.size_1); */

  /* this should be made an unlikely if */
  if (injfifo->freeSpace == 0) {
    /* Shadow indicates Fifo is full, read MU state */
    injfifo->freeSpace = MUSPI_getHwFreeSpace (injfifo);

/*     printf ("Free space available (2) %llx\n", (long long unsigned int)injfifo->freeSpace); */

    /* Fifo is still full, return error */
    if (injfifo->freeSpace == 0)
      return (uint64_t)-1;
  }

  *desc = (void *) MUSPI_getTailVa(&injfifo->_fifo);

  /* The sequence number of the next injection descriptor is the number of
     descriptors previously injected plus one as this function does not
     actually inject any descriptors. When the descriptor returned by this
     function is injected, by simply updating the MU tail pointer, then the
     injection fifo numInjected field will be incremented. */
  return injfifo->numInjected + 1;
}


/**
 * \brief Advance Tail By One Descriptor Slot
 *
 * The descriptor located at the tail of the injection fifo has been
 * initialized by the caller and is ready to be processed by the MU. 
 * Advance the tail pointer to the next unitintialized descriptor in the
 * injection fifo.
 *
 * The pointer to the descriptor at the tail of the injection fifo was 
 * retrieved previously with MUSPI_InjFifoNextDesc().
 *
 * \see MUSPI_InjFifoNextDesc
 *
 * \param[in]  injfifo  Pointer to the injection fifo shadow structure
 *
 * \return Descriptor number of the descriptor that was injected.
 */
__INLINE__ uint64_t MUSPI_InjFifoAdvanceDesc (MUSPI_InjFifo_t * injfifo) 
{
  char * tail_va;

  /* update free space shadow */
  injfifo->freeSpace --;
  injfifo->numInjected ++;

  tail_va = (char *) MUSPI_getTailVa(&injfifo->_fifo);

  /* increment tail */
  tail_va = tail_va + BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES;

  /* using branching, we will add modulo arithmatic later */
  if (tail_va >= (char*) injfifo->_fifo.va_end)
    tail_va = (char *) injfifo->_fifo.va_start;

  /* Warm up the full L2 cache line (The entire 128b L2 line must be accessed to prevent L2 miss) */
  muspi_dcbz (tail_va, 0);

  /*   printf("InjFifoInject:  Setting Tail VA\n");   */
  MUSPI_setTailVa (&injfifo->_fifo, tail_va);

  /*
  printf ("Inject:  Before moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n",  
	  (long long unsigned int)injfifo->_fifo.va_head, 
	  (long long unsigned int)injfifo->_fifo.va_tail, 
	  (long long unsigned int)injfifo->_fifo.va_start,  
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, 
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, 
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); 
  */

  MUSPI_setHwTail (&injfifo->_fifo, 
		   tail_va - (char *)MUSPI_getStartVa(&injfifo->_fifo));

/*     printf ("Inject:  After moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n",  */
/* 	    (long long unsigned int)injfifo->_fifo.va_head, */
/* 	    (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	    (long long unsigned int)injfifo->_fifo.va_start,  */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */
  
  return injfifo->numInjected;
}


/**
 * \brief Retrieve the Addresses of the Next N Descriptors in the Injection Fifo
 *
 * Returns a pointer to the next available N slots in the specified injection
 * fifo.  The caller can then initialize descriptors directly in the 
 * injection fifo, and afterwards, invoke the MUSPI_InjFifoAdvanceDescs()
 * function to advance the injection fifo tail pointer to send the
 * descriptors.
 *
 * \see MUSPI_InjFifoAdvanceDescMultiple
 * \see MUSPI_InjFifoNextDescMultipleNoCheck
 *
 * \param[in]  injfifo  Pointer to the injection fifo shadow structure
 * \param[in]  N        Number of descriptors
 * \param[out] descs    Array of pointers to the next injection descriptors
 *
 * \retval Descriptor number of the last of these next descriptors
 * \retval -1 Injection fifo is full.  No descriptor pointers returned.
 * \returns Descriptor pointers
 */
__INLINE__ uint64_t MUSPI_InjFifoNextDescMultiple ( MUSPI_InjFifo_t  * injfifo,
						    uint32_t           N,
						    void            ** descs ) 
{
  uint32_t  i;
  MUHWI_Descriptor_t *nextDesc;
  MUHWI_Descriptor_t *end;

/*   printf ("Shadow Free space available %llx, size 0x%llx\n",  */
/* 	  (long long unsigned int)injfifo->freeSpace, */
/* 	  (long long unsigned int)injfifo->_fifo.size_1); */

  /* this should be made an unlikely if */
  if (injfifo->freeSpace < N) {
    /* Shadow indicates Fifo is full, read MU state */
    injfifo->freeSpace = MUSPI_getHwFreeSpace (injfifo);

/*     printf ("Free space available (2) %llx\n", (long long unsigned int)injfifo->freeSpace); */

    /* Fifo is still full, return error */
    if (injfifo->freeSpace < N)
      return (uint64_t)-1;
  }

  /* Fill in the next descriptor pointers */
  nextDesc = (MUHWI_Descriptor_t*) MUSPI_getTailVa(&injfifo->_fifo);
  end      = (MUHWI_Descriptor_t*) MUSPI_getEndVa (&injfifo->_fifo);

  for ( i=0; i<N; i++)
    {
      descs[i] = (void*)nextDesc;
      nextDesc++;
      if ( nextDesc >= end ) nextDesc = (MUHWI_Descriptor_t*) MUSPI_getStartVa(&injfifo->_fifo);
    }

  /* The sequence number of the next injection descriptor is the number of
     descriptors previously injected plus one as this function does not
     actually inject any descriptors. When the descriptor returned by this
     function is injected, by simply updating the MU tail pointer, then the
     injection fifo numInjected field will be incremented. */
  return injfifo->numInjected + N;
}


/**
 * \brief Retrieve the Addresses of the Next N Descriptors in the Injection Fifo With No Freespace Checking
 *
 * Returns a pointer to the next available N slots in the specified injection
 * fifo.  The caller can then initialize descriptors directly in the 
 * injection fifo, and afterwards, invoke the MUSPI_InjFifoAdvanceDescs()
 * function to advance the injection fifo tail pointer to send the
 * descriptors.
 *
 * \warning No freespace checking is done.
 *
 * \see MUSPI_InjFifoAdvanceDescMultiple
 * \see MUSPI_InjFifoNextDescMultiple
 *
 * \param[in]  injfifo  Pointer to the injection fifo shadow structure
 * \param[in]  N        Number of descriptors
 * \param[out] descs    Array of pointers to the next injection descriptors
 *
 * \retval Descriptor number of the last of these next descriptors
 * \returns Descriptor pointers
 */
__INLINE__ uint64_t MUSPI_InjFifoNextDescMultipleNoCheck ( MUSPI_InjFifo_t  * injfifo,
							   uint32_t           N,
							   void            ** descs ) 
{
  uint32_t  i;
  MUHWI_Descriptor_t *nextDesc;
  MUHWI_Descriptor_t *end;

  /* Fill in the next descriptor pointers */
  nextDesc = (MUHWI_Descriptor_t*) MUSPI_getTailVa(&injfifo->_fifo);
  end      = (MUHWI_Descriptor_t*) MUSPI_getEndVa (&injfifo->_fifo);

  for ( i=0; i<N; i++)
    {
      descs[i] = (void*)nextDesc;
      nextDesc++;
      if ( nextDesc >= end ) nextDesc = (MUHWI_Descriptor_t*) MUSPI_getStartVa(&injfifo->_fifo);
    }

  /* The sequence number of the next injection descriptor is the number of
     descriptors previously injected plus one as this function does not
     actually inject any descriptors. When the descriptor returned by this
     function is injected, by simply updating the MU tail pointer, then the
     injection fifo numInjected field will be incremented. */
  return injfifo->numInjected + N;
}


/**
 * \brief Advance Tail By N Descriptor Slots
 *
 * The descriptors located at the tail of the injection fifo has been
 * initialized by the caller and is ready to be processed by the MU. 
 * Advance the tail pointer to the next unitintialized descriptor in the
 * injection fifo.
 *
 * The pointer to the descriptor at the tail of the injection fifo was 
 * retrieved previously with MUSPI_InjFifoNextDesc().
 *
 * \see MUSPI_InjFifoNextDesc
 *
 * \param[in]  injfifo  Pointer to the injection fifo shadow structure
 * \param[in]  N        Number of descriptors
 *
 * \return Descriptor number of the last descriptor that was injected.
 */
__INLINE__ uint64_t MUSPI_InjFifoAdvanceDescMultiple ( MUSPI_InjFifo_t * injfifo,
						       uint32_t          N ) 
{
  char * tail_va, *end_va;

  /* update free space shadow */
  injfifo->freeSpace   -= N;
  injfifo->numInjected += N;

  tail_va = (char *) MUSPI_getTailVa(&injfifo->_fifo);

  /* increment tail */
  tail_va = tail_va + ( N << BGQ_MU_DESCRIPTOR_SIZE_IN_POWER_OF_2 );

  /* using branching, we will add modulo arithmetic later */
  end_va = (char *) MUSPI_getEndVa(&injfifo->_fifo);
  
  if ( tail_va >= end_va )
    tail_va = (char *) injfifo->_fifo.va_start + ( tail_va - end_va );

  /* Warm up the full L2 cache line (The entire 128b L2 line must be accessed to prevent L2 miss) */
  muspi_dcbz (tail_va, 0);

  /*   printf("InjFifoInject:  Setting Tail VA\n");   */
  MUSPI_setTailVa (&injfifo->_fifo, tail_va);

  
/*   printf ("AdvanceMultiple:  Before moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, End VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->_fifo.va_head, */
/* 	  (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.va_start, */
/* 	  (long long unsigned int)injfifo->_fifo.va_end, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */

  MUSPI_setHwTail (&injfifo->_fifo, 
		   tail_va - (char *)MUSPI_getStartVa(&injfifo->_fifo));

/*   printf ("AdvanceMultiple:  After  moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, End VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->_fifo.va_head, */
/* 	  (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.va_start, */
/* 	  (long long unsigned int)injfifo->_fifo.va_end, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */
  
  return injfifo->numInjected;
}


/**
 * \brief Inject a Descriptor Into the Injection Fifo
 *
 * \param[in] injfifo  Pointer to the injection fifo shadow structure
 * \param[in] desc     Pointer to the descriptor
 *
 * \return Descriptor number of the descriptor that was injected.
 * \retval -1 Injection fifo is full
 */
__INLINE__ uint64_t MUSPI_InjFifoInject (MUSPI_InjFifo_t   * injfifo, 
					 void              * desc) 
{

  char *tail_va;
/*   MUSPI_DescriptorDumpHex((char*)"Injecting...",(MUHWI_Descriptor_t*)desc); */
/*   printf ("Shadow Free space available %llx, size 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->freeSpace, */
/* 	  (long long unsigned int)injfifo->_fifo.size_1); */

  /* this should be made an unlikely if */
  if (injfifo->freeSpace == 0) {
    /* Shadow indicates Fifo is full, read MU state */
    injfifo->freeSpace = MUSPI_getHwFreeSpace (injfifo);

/*     printf ("Free space available (2) %llx\n", (long long unsigned int)injfifo->freeSpace); */
    
    /* Fifo is still full, return error */
    if (injfifo->freeSpace == 0)
      return (uint64_t)-1;
  }
  
  /* update free space shadow */
  injfifo->freeSpace --;
  injfifo->numInjected ++;

  tail_va = (char *) MUSPI_getTailVa(&injfifo->_fifo);

/*   printf("InjFifoInject:  Copying Descriptor into fifo\n"); */
  /* copy the descriptor (add QPX alignment optimization later)*/
#ifdef __LINUX__ 
  memcpy (tail_va, desc, BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES);     

#else 
  /*Descriptor pointers must be 32byte aligned */
  MUSPI_assert((((uint64_t) desc) & 0xFFFFFFFFFFFFFFE0ULL) == (uint64_t) desc);

  /* reduce #loads/stores from 8 to 2*/
#ifdef __GNUC__
  register double fp0 asm("fr0");
  register double fp1 asm("fr1");
#else
  register double fp0 asm("f0");
  register double fp1 asm("f1");
#endif
  VECTOR_LOAD_NU (desc,  0, fp0);  /* Load first 32 bytes to reg 0*/
  VECTOR_LOAD_NU (desc, 32, fp1);  /* Load second 32 bytes to reg 1*/
  
  VECTOR_STORE_NU(tail_va,  0, fp0); 
  VECTOR_STORE_NU(tail_va, 32, fp1);   
#endif

  /* increment tail */
  tail_va = tail_va + BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES;  
  
  /* using branching, we will add modulo arithmatic later */
  if (tail_va >= (char*) injfifo->_fifo.va_end)
    tail_va = (char *) injfifo->_fifo.va_start;
  
  /* Warm up the full L2 cache line (The entire 128b L2 line must be accessed to prevent L2 miss) */
  muspi_dcbz (tail_va, 0);  
  
  /*   printf("InjFifoInject:  Setting Tail VA\n");   */
  MUSPI_setTailVa (&injfifo->_fifo, tail_va);  

  /*
  printf ("Inject:  Before moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n",  
	  (long long unsigned int)injfifo->_fifo.va_head, 
	  (long long unsigned int)injfifo->_fifo.va_tail, 
	  (long long unsigned int)injfifo->_fifo.va_start,  
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, 
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, 
	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); 
  */

  MUSPI_setHwTail (&injfifo->_fifo, 
		   tail_va - (char *)MUSPI_getStartVa(&injfifo->_fifo));

  
/*     printf ("Inject:  After moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n",  */
/* 	    (long long unsigned int)injfifo->_fifo.va_head, */
/* 	    (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	    (long long unsigned int)injfifo->_fifo.va_start,  */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	    (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */
  
  return injfifo->numInjected;
}

/**
 * \brief Check If a Descriptor is Complete
 *
 * When a descriptor is injected using MUSPI_InjFifoInject() or 
 * MUSPI_InjFifoAdvanceDesc(), the descriptor number of that injected
 * descriptor is returned.  That descriptor number can be passed into
 * this function to find out if that descriptor has been completed by
 * the MU hardware (i.e. if the message has been sent).
 * 
 * \param[in] injfifo     Pointer to the injection fifo shadow structure
 * \param[in] desc_count  Descriptor number of the descriptor being tested
 *
 * \see MUSPI_InjFifoInject() 
 * \see MUSPI_InjFifoAdvanceDesc()
 *
 * \todo This version uses branching. Explore other mechanisms later 
 */
__INLINE__ uint32_t MUSPI_CheckDescComplete (MUSPI_InjFifo_t  * injfifo,
					     uint64_t           desc_count) 
{
  if (desc_count <= injfifo->descCount)
    return 1;
  
  injfifo->descCount = MUSPI_getHwDescCount (injfifo);

  /* This branch can be eliminated with arithmatic that returns the
     sign bit field of the difference */
  if (desc_count <= injfifo->descCount)
    return 1;
  
  return 0;
}


/**
 * \brief Inject Multiple Descriptors Into the Injection Fifo
 *
 * \param[in] injfifo  Pointer to the injection fifo shadow structure
 * \param[in] desc     Pointer to the descriptors, one after the other
 * \param[in] ndesc    Number of descriptors
 *
 * \return Descriptor number of the last descriptor that was injected.
 * \retval -1 Injection fifo is full
 */
__INLINE__ uint64_t MUSPI_InjFifoInjectMultiple (MUSPI_InjFifo_t * injfifo, 
						 void            * desc, 
						 uint32_t          ndesc)
{

  char *tail_va, *end_va, *start_va;
  uint32_t i;

/*   for (i=0; i<ndesc; i++) */
/*     { */
/*       MUSPI_DescriptorDumpHex((char*)"InjectMultiple...",&(((MUHWI_Descriptor_t*)desc)[i])); */
/*     } */
/*   printf ("Shadow Free space available %llx, size 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->freeSpace, */
/* 	  (long long unsigned int)injfifo->_fifo.size_1); */

  /* this should be made an unlikely if */
  if (injfifo->freeSpace < ndesc) {
    /* Shadow indicates Fifo is full, read MU state */
    injfifo->freeSpace = MUSPI_getHwFreeSpace (injfifo);

/*     printf ("Free space available (2) %llx\n", (long long unsigned int)injfifo->freeSpace); */
    
    /* Fifo is still full, return error */
    if (injfifo->freeSpace < ndesc)
      return (uint64_t)-1;
  }
  
  /* update free space shadow */
  injfifo->freeSpace   -= ndesc;
  injfifo->numInjected += ndesc;

  tail_va   = (char*) MUSPI_getTailVa (&injfifo->_fifo);
  end_va    = (char*) MUSPI_getEndVa  (&injfifo->_fifo);
  start_va  = (char*) MUSPI_getStartVa(&injfifo->_fifo);

  /* copy the descriptors */
  for (i=0; i<ndesc; i++)
    {
#ifdef __LINUX__ 
      memcpy (tail_va, desc, BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES);     
#else 
      /*Descriptor pointers must be 32byte aligned */
      MUSPI_assert((((uint64_t) desc) & 0xFFFFFFFFFFFFFFE0ULL) == (uint64_t) desc);
      
      /* reduce #loads/stores from 8 to 2*/
#ifdef __GNUC__
      register double fp0 asm("fr0");
      register double fp1 asm("fr1");      
#else
      register double fp0 asm("f0");
      register double fp1 asm("f1");      
#endif
      VECTOR_LOAD_NU (desc,  0, fp0);  /* Load first 32 bytes to reg 0*/
      VECTOR_LOAD_NU (desc, 32, fp1);  /* Load second 32 bytes to reg 1*/
      
      VECTOR_STORE_NU(tail_va,  0, fp0); 
      VECTOR_STORE_NU(tail_va, 32, fp1);   
#endif
/*       MUSPI_DescriptorDumpHex((char*)"InjectMultiple in fifo...",(MUHWI_Descriptor_t*)tail_va); */
      tail_va += BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES;
      if ( tail_va >= end_va ) tail_va = start_va;
      desc = (void*)( (char*)desc + BGQ_MU_DESCRIPTOR_SIZE_IN_BYTES );
    }
  
  /* Warm up the full L2 cache line (The entire 128b L2 line must be accessed to prevent L2 miss) */
  muspi_dcbz (tail_va, 0);  
  
  /*   printf("InjFifoInject:  Setting Tail VA\n");   */
  MUSPI_setTailVa (&injfifo->_fifo, tail_va);  

  
/*   printf ("InjectMultiple:  Before moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->_fifo.va_head, */
/* 	  (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.va_start, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */
  
  MUSPI_setHwTail (&injfifo->_fifo, 
		   tail_va - (char *)MUSPI_getStartVa(&injfifo->_fifo));
  
  
/*   printf ("InjectMultiple:  After moving tail:  Head VA 0x%llx, Tail VA 0x%llx, Start VA 0x%llx, Head PA 0x%llx, Tail PA 0x%llx Start PA 0x%llx\n", */
/* 	  (long long unsigned int)injfifo->_fifo.va_head, */
/* 	  (long long unsigned int)injfifo->_fifo.va_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.va_start, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_head, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_tail, */
/* 	  (long long unsigned int)injfifo->_fifo.hwfifo->pa_start); */
  
  return injfifo->numInjected;
}


/**
 * \brief Get an Injection Fifo Pointer Given an ID and Injection Fifo Subgroup
 *
 * \param[in]  fifoid  Number of the fifo, relative to the fifo subgroup
 *                     (0 to BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP).
 * \param[in]  fg_ptr  Pointer to injection fifo subgroup structure.
 *
 * \returns  Pointer to injection fifo structure.
 */
__INLINE__ MUSPI_InjFifo_t *MUSPI_IdToInjFifo (int                       fifoid,
					       MUSPI_InjFifoSubGroup_t * fg_ptr) 
{ 
  MUSPI_assert (fifoid < BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP);
  return &fg_ptr->_injfifos[fifoid];
}


__END_DECLS


#endif
