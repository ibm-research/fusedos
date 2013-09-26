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

#ifndef    __MUSPI_FIFO__
#define    __MUSPI_FIFO__

/**
 * \file Fifo.h
 *
 * \brief C Header File containing Fifo Definitions Common to 
 *          Injection and Reception
 */

#include "kernel_impl.h"
#include "Util.h"

#include "hwi/include/bqc/MU_Fifo.h"
#include <spi/include/mu/Util.h>


__BEGIN_DECLS

/**
 * \brief Fifo Shadow Structure
 *
 * Contains shadows of the fifo start, head, tail, and end, and a pointer to
 * the hardware fifo structure in the MU SRAM.
 */
typedef struct MUSPI_Fifo
{  
  uint64_t             pa_start;   /**< The fifo start physical address */
  void              *  va_start;   /**< The fifo start pointer */
  uint64_t             size_1;	   /**< The size of the fifo - 1, in bytes */
  void              *  va_head;	   /**< The fifo head pointer */
  void              *  va_tail;	   /**< The fifo tail pointer */
  void              *  va_end;	   /**< Pointer to mark end of the fifo */
      
  MUHWI_Fifo_t      *  hwfifo;     /**< Virtual address pointer to MU
				        SRAM for this fifo */
} MUSPI_Fifo_t;


/**
 * \brief Get the Size-1 in Bytes of the Fifo from the Shadow
 *
 * The MU SRAM stores size-1.
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns The fifo size, minus 1, in bytes
 */
__INLINE__ uint64_t MUSPI_getSize (MUSPI_Fifo_t *fifo) 
{
  return fifo->size_1;
}


/**
 * \brief Get the Fifo Start Physical Address from the Shadow
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo start physical address
 */
__INLINE__ uint64_t MUSPI_getStartPa (MUSPI_Fifo_t *fifo) 
{
  return fifo->pa_start;
}


/**
 * \brief Get the Fifo Start Virtual Address from the Shadow
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo start virtual address
 */
__INLINE__ void * MUSPI_getStartVa (MUSPI_Fifo_t *fifo) 
{
  return fifo->va_start;
}

/**
 * \brief Get the Fifo Head Virtual Address from the Shadow
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo head virtual address
 */
__INLINE__ void * MUSPI_getHeadVa (MUSPI_Fifo_t *fifo) 
{
  return fifo->va_head;
}

/**
 * \brief Get the Fifo Tail Virtual Address from the Shadow
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo tail virtual address
 */
__INLINE__ void * MUSPI_getTailVa (MUSPI_Fifo_t *fifo) 
{
  return fifo->va_tail;
}


/**
 * \brief Get the Fifo End Virtual Address from the Shadow
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo end virtual address
 */
__INLINE__ void * MUSPI_getEndVa (MUSPI_Fifo_t *fifo) 
{
  return fifo->va_end;
}


/**
 * \brief Get the Size-1 in Bytes of the Fifo from the Hardware
 *
 * Gets the size-1 from the MU SRAM, which stores size-1.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed. 
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns The fifo size, minus 1, in bytes, from the hardware
 */
__INLINE__ uint64_t MUSPI_getHwSize(MUSPI_Fifo_t *fifo) 
{
  return fifo->hwfifo->size_1;
}


/**
 * \brief Get the Fifo Start Physical Address from the Hardware
 *
 * Gets the start physical address from the MU SRAM.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed.
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo start physical address
 */
__INLINE__ uint64_t MUSPI_getHwStart(MUSPI_Fifo_t *fifo) 
{
  return fifo->hwfifo->pa_start;
}


/**
 * \brief Get the Fifo Head Physical Address from the Hardware
 *
 * Gets the head physical address from the MU SRAM.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed.
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo head physical address
 */
__INLINE__ uint64_t MUSPI_getHwHead(MUSPI_Fifo_t *fifo) 
{
  return fifo->hwfifo->pa_head;
}


/**
 * \brief Get the Fifo Tail Physical Address from the Hardware
 *
 * Gets the tail physical address from the MU SRAM.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed.
 *
 * \param[in]  fifo  Pointer to the fifo shadow structure
 *
 * \returns Fifo tail physical address
 */
__INLINE__ uint64_t MUSPI_getHwTail(MUSPI_Fifo_t *fifo) 
{
  return fifo->hwfifo->pa_tail;
}



/**
 * \brief Set the Fifo Head Virtual Address into the Shadow
 *
 * The application is responsible for keeping data structures
 * consistent and updating by the right increments. For example,
 * the reception fifo head must be incremented by 32 bytes
 * or multiples of 32 bytes.
 *
 * \param[in]  fifo      Pointer to the fifo shadow structure
 * \param[in]  new_head  New head virtual address
 */
__INLINE__ void MUSPI_setHeadVa (MUSPI_Fifo_t *fifo, 
				 void         *new_head) 
{
  /* Reception fifo head should be a multiple of 32 bytes */   
  MUSPI_assert (((uint64_t)new_head % 32) == 0);
  fifo->va_head = new_head;  
}


/**
 * \brief Set the Fifo Tail Virtual Address into the Shadow
 *
 * The application is responsible for keeping data structures
 * consistent and updating by the right increments. For example,
 * the injection fifo tail must be incremented by 64 bytes.
 *
 * \param[in]  fifo      Pointer to the fifo shadow structure
 * \param[in]  new_tail  New tail virtual address
 */
__INLINE__ void MUSPI_setTailVa (MUSPI_Fifo_t *fifo, 
				 void         *new_tail) 
{
  /* Injection fifo pointers should be multiples of 64 bytes.
   * However, since this function can be called to set the
   * shadow tail for a reception fifo, use 32 byte alignment
   * for the assertion, since that is the least common
   * denominator. */   
  MUSPI_assert (((uint64_t)new_tail % 32) == 0);
  fifo->va_tail = new_tail;  
}


/**
 * \brief Set the Fifo Head Physical Address into the Hardware
 *
 * Sets the head physical address into the MU SRAM.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed.
 *
 * The application is responsible for keeping data structures
 * consistent and updating by the right increments. For example,
 * the reception fifo head must be incremented by 32 bytes
 * or multiples of 32 bytes.
 *
 * \param[in]  fifo         Pointer to the fifo shadow structure
 * \param[in]  head_offset  Offset from the start of the fifo of the
 *                          new head.
 */
__INLINE__ void MUSPI_setHwHead (MUSPI_Fifo_t *fifo, 
				 uint64_t      head_offset) 
{
  MUSPI_assert (head_offset <= fifo->size_1);
  fifo->hwfifo->pa_head = fifo->pa_start + head_offset;
}


/**
 * \brief Set the Fifo Tail Physical Address into the Hardware
 *
 * Sets the tail physical address into the MU SRAM.
 * The SRAM TLB is configured such that all accesses are serialized and
 * no additional memory sync operations are needed.
 *
 * The application is responsible for keeping data structures
 * consistent and updating by the right increments. For example,
 * the injection fifo tail must be incremented by 64 bytes.
 *
 * \param[in]  fifo         Pointer to the fifo shadow structure
 * \param[in]  tail_offset  Offset from the start of the fifo of the
 *                          new tail.
 */
__INLINE__ void MUSPI_setHwTail (MUSPI_Fifo_t *fifo, 
				 uint64_t      tail_offset) 
{
  MUSPI_assert (tail_offset < fifo->size_1);
  fifo->hwfifo->pa_tail = fifo->pa_start + tail_offset;
}


__END_DECLS

#endif
