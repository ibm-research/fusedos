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

#ifndef  __MUSPI_REC_FIFO__
#define  __MUSPI_REC_FIFO__

/**
 * \file RecFifo.h
 *
 * \brief C Header File containing Reception Fifo Definitions
 *
 * There are two reception fifo poll functions defined here:
 * 
 * MUSPI_RecFifoPoll - Requires pre-registration of functions to handle
 * different types of packets.  Pointers to the registered functions are
 * stored in a static array.  The array index is the registration ID.
 * The user must store the registration ID in the first byte of the 
 * "software bytes" field of the memory fifo packet header.  Specifically,
 * the first byte of the Unused2 field of the Memory_FIFO message unit
 * header.  When the packet arrives in the fifo, the poll function 
 * extracts this registration ID and calls the function registered
 * in the table.
 *
 * MUSPI_RecFifoExamplePoll - This is an example of how inline helper
 * functions can be used to poll the reception fifo and process the
 * packets.  Using these functions provides more flexibility for the user.
 * No pre-registration of functions and no function ID in the packet header
 * are required.  The user can recognize the packet using any technique
 * it wants, and call whatever function it wants with whatever parameters it
 * wants that function to have.  In this example function, the call to the
 * function could be replaced by a switch statement that calls different
 * functions with different parameters.
 */


#include "kernel_impl.h"
#include <hwi/include/bqc/MU_PacketHeader.h>
#include <spi/include/mu/Fifo.h>
#include <spi/include/mu/Util.h>

__BEGIN_DECLS

/**
 * \brief Re-definition of the msync instruction.
 */ 
#define _bgq_msync  ppc_msync


/**
 * \brief Reception Fifo Structure
 *
 * The structure corresponding to a reception fifo.  It contains a generic fifo
 * structure, a buffer used for handling a reception fifo wrap condition, and
 * other items.
 */
typedef struct MUSPI_RecFifo {
  MUSPI_Fifo_t       _fifo;        /**< Software fifo shadow. */
  uint64_t           numProcessed; /**< Number of packets
                                        processed. This parameter will
                                        allow us to update the head
                                        pointer in the SRAM every few
                                        packets. */
  char               _wrapBuf [544] __attribute__((__aligned__(32))); /**< 
					Temporary buffer to store a packet
                                        having some of its bytes at the end
					of the fifo and the remaining bytes
					at the start of the fifo. */
} MUSPI_RecFifo_t;


/**
 * \brief Reception Fifo Subgroup
 *
 * This structure defines one Reception Fifo Subgroup.  It consists of an
 * array of reception fifos, a pointer to reception fifo status, and the
 * ID of the subgroup.
 */
typedef struct MUSPI_RecFifoSubGroup {
  MUSPI_RecFifo_t          _recfifos [BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP]; /**<
					          Array of reception fifos. */

  MUHWI_RecFifoStatus_t  * groupStatus; /**< Pointer to the fifo status.    */
  
  uint32_t                  subgroupid; /**< Subgroup id from 0 to 
					     (BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-1). */

} MUSPI_RecFifoSubGroup_t;


/**
 * \brief Prototype for a Registered Receive Function.
 *
 * This function is called by MUSPI_RecFifoPoll() to process a single packet
 * found in the reception fifo.
 *
 * \param[in]  recv_func_parm  The opaque pointer that was provided when this
 *                             function was registered.
 * \param[in]  packet_ptr      Pointer to the packet header being processed.
 * \param[in]  packet_bytes    The number of bytes in the packet, including
 *                             the packet header.
 *
 * \retval  0         Success
 * \retval  non-zero  An error has occurred.
 * \note The return value is ignored by MUSPI_RecFifoPoll().
 */
typedef int  (*MUSPI_RecvFunction_t)(void                 *recv_func_parm,
				     MUHWI_PacketHeader_t *packet_ptr,
				     uint32_t              packet_bytes
				     );


/**
 * \brief Structure Defining an Element of the Reception Fifo Poll Registration Table.
 */
typedef struct MUSPI_RecvInfoElement 
{
  MUSPI_RecvFunction_t         recvFunction; /**< The receive function pointer.   */
  void                       * recvParam;    /**< The receive function parameter. */
} MUSPI_RecvInfoElement_t;


/**
 * \brief Size of the Table of Registered Receive Functions.
 */
#define MUSPI_MAX_FUNCTION_IDS     256


/**
 * \brief Reception Fifo Poll Registration Table.
 */
typedef struct MUSPI_RecvInfoTable 
{
  MUSPI_RecvInfoElement_t         table[MUSPI_MAX_FUNCTION_IDS]; /**< Array of 
						  functions and function 
						  parameters. */
} MUSPI_RecvInfoTable_t;


/**
 * \brief Declaration of the Reception Fifo Poll Registration Table.
 */
extern MUSPI_RecvInfoTable_t   _muspi_RecvInfo;


/**
 * \brief Next Free Registration Id in the Reception Fifo Poll Registration Table.
 */
extern uint32_t                _muspi_RecvInfoNextFreeId;


/**
 * DEPRECATED:  TO BE REMOVED SHORTLY.  USE MUSPI_RegisterReceiveFunction().
 *
 * \brief Register a Reception Fifo Receive Function.
 *
 * This function must be called one or more times prior to using 
 * MUSPI_RecFifoPoll().  Each call registers a receive function and
 * a parameter to be passed to the receive function.  A registration ID
 * is provided, uniquely identifying this receive function.  This ID must
 * be stored in the first byte of the "software bytes" field of the memory
 * fifo packet header.  Specifically, the first byte of the Unused2 field 
 * of the Memory_FIFO message unit header.  When the packet arrives in the
 * fifo, the poll function extracts this registration ID and calls the 
 * function registered with that registration ID.
 *
 * \param[in]  fn     Function pointer to the receive function.
 * \param[in]  param  Pointer to be passed to the receive function when
 *                    it is called by MUSPI_RecFifoPoll().
 * \param      id     Registration ID (0 to MUSPI_MAX_FUNCTION_IDS-1).
 *
 * \note No attempt is made to track which IDs are already registered.
 *       A subsequent registration attempt will overlay the previous
 *       registration.
 */

__INLINE__ void MUSPI_RegisterRecvFunction (MUSPI_RecvFunction_t    fn,
					    void                  * param,
					    uint32_t                id) 
{
  MUSPI_assert (id < MUSPI_MAX_FUNCTION_IDS);

  _muspi_RecvInfo.table[id].recvFunction = fn;
  _muspi_RecvInfo.table[id].recvParam    = param; 
}


/**
 * \brief Register a Reception Fifo Receive Function.
 *
 * This function must be called one or more times prior to using 
 * MUSPI_RecFifoPoll().  Each call registers a receive function and
 * a parameter to be passed to the receive function.  A registration ID
 * is returned, uniquely identifying this receive function.  This ID must
 * be stored in the first byte of the "software bytes" field of the memory
 * fifo packet header.  Specifically, the first byte of the Unused2 field 
 * of the Memory_FIFO message unit header.  When the packet arrives in the
 * fifo, the poll function extracts this registration ID and calls the 
 * function registered with that registration ID.
 *
 * \param[in]  fn     Function pointer to the receive function.
 * \param[in]  param  Pointer to be passed to the receive function when
 *                    it is called by MUSPI_RecFifoPoll().
 *
 * \retval     -1     Error.  All registration IDs are taken.
 * \retval     >=0    Registration ID (0 to MUSPI_MAX_FUNCTION_IDS-1).
 */

__INLINE__
int32_t MUSPI_RegisterReceiveFunction ( MUSPI_RecvFunction_t    fn,
				        void                  * param )
{
  uint32_t nextFreeId = _muspi_RecvInfoNextFreeId;
  uint32_t i;

  MUSPI_assert ( fn != NULL );

  for ( i=nextFreeId; i<MUSPI_MAX_FUNCTION_IDS; i++ ) /* Search for an empty slot */
    {
      if ( _muspi_RecvInfo.table[i].recvFunction == NULL ) /* Found a slot? */
        {
          _muspi_RecvInfo.table[i].recvFunction = fn;
          _muspi_RecvInfo.table[i].recvParam    = param;
	  _muspi_RecvInfoNextFreeId             = i+1;
          return i; /* Return the registration ID */
        }
    }
  /* No available slot was found.
   * Start at the beginning next time, to be sure.
   */
  _muspi_RecvInfoNextFreeId = 0;

  return -1; /* No open slots */
}


/**
 * \brief De-Register a Reception Fifo Receive Function.
 *
 * This function removes the function pointer and parameter from the
 * receive function table, making it available for use by another function.
 *
 * \param[in]  id     Registration ID of the function to be removed,
 *                    previously returned from MUSPI_RegisterRecvFunction().
 *
 * \retval      0     Success.
 * \retval     -1     Error.  Registration ID is out of range.
 */

__INLINE__
int32_t MUSPI_DeRegisterReceiveFunction ( uint32_t id )
{
  if ( id < MUSPI_MAX_FUNCTION_IDS )
    {
      _muspi_RecvInfo.table[id].recvFunction = NULL;
      _muspi_RecvInfo.table[id].recvParam    = NULL;
      _muspi_RecvInfoNextFreeId              = 0; /* Start at front of table
                                                     next time */
      return 0;
    }
  return -1;
}


/**
 * \brief Handle a Reception Fifo Poll Wrap Condition
 *
 * This function is called when the head and tail pointers of a reception
 * fifo indicate that the packets wrap.  That is, tail < head.
 * This function processes all of the packets at the end of the fifo,
 * including the one that wraps.  It updates both the software head pointer
 * and the hardware head pointer, and returns the number of packets
 * processed.  It does not process any packets that may still be in the
 * fifo after the wrapped packet.  The function may prematurely end
 * the processing of packets if the specified npackets has been reached.
 *
 * \param[in]  rec_fifo  Pointer to reception fifo structure.
 * \param[in]  npackets  Maximum number of packets to process.
 *
 * \returns  Number of packets processed.
 *
 * \note This is an out-of-line call because it is not called very often.
 */
uint32_t MUSPI_RecFifoPollWrap (MUSPI_RecFifo_t   *rec_fifo,
				uint32_t           npackets);


/**
 * \brief Reception Fifo Poll
 *
 * Poll a specified reception fifo, processing packets by calling
 * a registered receive function based on a registration ID in the
 * packet header.
 * 
 * Prior to calling this function, MUSPI_RegisterRecvFunction() must be
 * called one or more times.  The registration ID for the appropriate
 * receive function must be in each packet's header in the first byte
 * of the Unused2 field of the Memory_FIFO Message Unit Header (also
 * known as the first byte of the SoftwareBytes of the Memory Fifo
 * descriptor).
 *
 * This poll function optimizes by processing packets as follows:
 * - If the packets do not wrap, all of the packets are processed with
 *   one fetch of the hardware tail.
 * - If the packets wrap, only the packets up to the one that wraps
 *   are processed in this call.  The next call will process the one
 *   packet that wraps.  The next call will process the packets after
 *   the one that wrapped.
 *
 * \param[in]  rec_fifo  Pointer to reception fifo structure.
 * \param[in]  npackets  Maximum number of packets to process.
 *
 * \returns  Number of packets processed.
 * 
 */
__INLINE__ uint32_t MUSPI_RecFifoPoll (MUSPI_RecFifo_t   *rec_fifo,
				       uint32_t           npackets) 
{
  uint64_t bytes = 0;           /* Number of bytes processed */
  uint64_t bytes_recvd = 0;     /* Number of bytes received */
  uint32_t num_processed = 0;   /* Number of packets processed */
  
  MUSPI_RecvFunction_t      recv_func_ptr; /* Pointer to receive function*/
  void                    * recv_func_parm;/* Receive function parameter */
  int                       recv_func_id;  /* Function ID from the packet */
                                           /* header */

  MUHWI_PacketHeader_t * packet_ptr;       /* Pointer to packet header    */
  uint32_t               packet_bytes;     /* Number of bytes in the packet*/
  char         * va_tail;                  /* Snapshot of the fifo's tail */
  char         * va_end;                   /* Snapshot of the fifo's end */
  char         * va_head;                  /* Snapshot of the fifo's head */
  uint64_t       pa_tail;              /* Actual tail physical address */
  
  MUSPI_Fifo_t  *fifo_ptr = &rec_fifo->_fifo;
  
  va_head      = (char *) fifo_ptr->va_head;  
  /** touch head for first packet */
  muspi_dcbt (va_head, 0);
  
  pa_tail      = MUSPI_getHwTail (fifo_ptr);    /* Snapshot HW */
  va_tail      = (char *)(pa_tail - MUSPI_getStartPa (fifo_ptr) + 
			  (char *) MUSPI_getStartVa (fifo_ptr));

  MUSPI_assert (npackets >= 1);
  
  if (va_head == va_tail) {
    /* printf ("Reception Fifo is empty: va_head=%llx, pa_tail=%llx, address of pa_tail %llx\n", 
       va_head, MUSPI_getHwTail (fifo_ptr),
       &(fifo_ptr->hwfifo->pa_tail)); */
    return num_processed;        
  }

  va_end       = (char *) fifo_ptr->va_end;  
  if (va_tail > va_head) 
    /** There is no wrap and we can grab packets */
    bytes_recvd = va_tail - va_head;
  else if (va_head >= va_end - 544) 
    /** We have a wrap */
    return MUSPI_RecFifoPollWrap (rec_fifo, npackets);
  else { /** We wont process the wrap in this packet poll */
    va_end = (char *)MUSPI_getEndVa (fifo_ptr);
    bytes_recvd = (uint64_t)va_end - (uint64_t)va_head - 544ULL;
    va_tail = (char *)((uint64_t)va_end - 544ULL);
    MUSPI_assert (bytes_recvd <= fifo_ptr->size_1+1 - 544ULL);
  }
  
  /* Call msync */
  _bgq_msync();

  /* We know the fifo does not wrap */
  while ((bytes < bytes_recvd) && (num_processed < npackets)) {
    packet_ptr     = (MUHWI_PacketHeader_t *) va_head;
    /* Byte8 is identical between pt2pt and collective packets */
    packet_bytes   = (packet_ptr->NetworkHeader.pt2pt.Byte8.Size + 1) << 5;
    
    /* Standardize messaging header bytes */
    recv_func_id   = packet_ptr->messageUnitHeader.Packet_Types.Memory_FIFO.Unused2[0];
    recv_func_ptr  = _muspi_RecvInfo.table[recv_func_id].recvFunction;
    recv_func_parm = _muspi_RecvInfo.table[recv_func_id].recvParam;
    MUSPI_assert ( recv_func_ptr != 0 );
        
    num_processed++;
    
    va_head = (char *) ( (uint64_t) va_head + packet_bytes );     
    bytes += packet_bytes;
    
    /* printf ("Calling packet handler for %d byte packet, %lx, %lx\n", packet_bytes, va_head, MUSPI_getStartVa(fifo_ptr)); */
    /* printf("RecFifoPoll: Calling func %p with packet_ptr=%p, packet_bytes=%u\n",recv_func_ptr,packet_ptr,packet_bytes);*/
    /* printf("%08x %08x %08x %08x %08x %08x %08x %08x\n%08x %08x %08x %08x %08x %08x %08x %08x\n",
	   (uint32_t)(*(((uint32_t*)packet_ptr)+0)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+1)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+2)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+3)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+4)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+5)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+6)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+7)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+8)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+9)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+10)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+11)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+12)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+13)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+14)),
	   (uint32_t)(*(((uint32_t*)packet_ptr)+15)));*/

    (*recv_func_ptr)( recv_func_parm, 
		      packet_ptr, 
		      packet_bytes );    

    /* printf ("After packet handler for %d byte packet, hw fifo %lx, va_head %lx, start %lx\n", packet_bytes, 
       fifo_ptr, va_head, MUSPI_getStartVa(fifo_ptr)); */

    MUSPI_setHwHead (fifo_ptr, va_head - (char *) MUSPI_getStartVa(fifo_ptr));

    /* Touch head for next packet */
  } 

  /* Update the fifo head pointer */
  fifo_ptr->va_head = va_head;  

  return (num_processed); /* Return no packets processed */
}


/**
 * \brief Get the Number of Available Bytes to be Processed in a Reception Fifo
 *
 * Examines a specified reception fifo and returns the number of available
 * bytes, including packet headers and data, waiting to be processed.
 *
 * This function optimizes by returning the number of bytes as follows:
 * - If the packets do not wrap, the total length of all of the packets is
 *   returned so they can all be processed by the caller.
 * - If the packets wrap, only the total length of the packets up to the one 
 *   that wraps is returned.  
 * - If the first packet is the one that wraps, the "wrap" indicator is
 *   returned, indicating that a wrap condition must be processed.  The 
 *   number of bytes returned is the number of bytes to the end of the fifo.
 *
 * \param[in]  rec_fifo  Pointer to reception fifo structure.
 * \param[out] wrap      Pointer to a uint32_t that is set to 
 *                       - 0 if no wrap needs to be processed.
 *                       - 1 if a wrap needs to be processed.
 *
 * \returns  Number of bytes to be processed, and the wrap condition.
 */
__INLINE__ uint32_t MUSPI_getAvailableBytes (MUSPI_RecFifo_t   * rec_fifo,
					     uint32_t          * wrap)
{
  uint32_t      bytes_recvd;       /* Number of bytes received*/
  void        * va_tail;           /* Snapshot of the fifo's tail */
  void        * va_end;            /* Snapshot of the fifo's end */
  void        * va_head;           /* Snapshot of the fifo's head */
  volatile uint64_t  pa_tail;      /* Actual tail physical address */
  
  MUSPI_Fifo_t  *fifo_ptr = &rec_fifo->_fifo;
  
  va_head      = fifo_ptr->va_head;
  va_end       = fifo_ptr->va_end;
  pa_tail      = MUSPI_getHwTail (fifo_ptr);    /* Snapshot HW */
  va_tail      = (void*)(pa_tail - MUSPI_getStartPa (fifo_ptr) + 
			 (uint64_t)(MUSPI_getStartVa (fifo_ptr)));
  
  MUSPI_setTailVa (fifo_ptr, va_tail); /** extra store over older poll loop*/

  if (va_tail == va_head)
    return 0;
  
  *wrap = 0;

  /** touch head for first packet */
  muspi_dcbt (va_head, 0);
  _bgq_msync();
  
  if (va_tail > va_head) /** one branch less over older poll loop */
    bytes_recvd = (uint64_t)va_tail - (uint64_t)va_head; /** No wrap and we can grab packets */
  else if ( va_head < (void*)((uint64_t)va_end - 544UL) ) 
    /** We wont process the wrap in this packet poll */
    bytes_recvd = (uint64_t)va_end - (uint64_t)va_head - 544UL;
  else {
    *wrap = 1;
    bytes_recvd = (uint64_t)va_end - (uint64_t)va_head;
  }
  
  return bytes_recvd;
}


/**
 * \brief Get the Pointer and Length of the Next Packet - Optimized
 *
 * Using a quick technique, get the pointer and length of the next packet
 * to be processed in a reception fifo.  The technique eliminates checks
 * for edge conditions that must have been previously screened.  
 * Specifically,
 * - the fifo head must be less than the fifo tail, or
 * - the fifo head must NOT be within the last 544 bytes of the end of the fifo.
 *
 * \param[in]  rec_fifo  Pointer to reception fifo structure.
 * \param[out] pbytes    Pointer to the number of bytes in the packet.
 *
 * \returns  Pointer to the next packet to be processed, and number of bytes
 *           in that packet.
 *
 * \see MUSPI_getNextPacketWrap()
 */

__INLINE__ MUHWI_PacketHeader_t *MUSPI_getNextPacketOptimized (
						    MUSPI_RecFifo_t  *rec_fifo,
						    uint32_t         *pbytes)
{  
  char                   * va_head;       /* Snapshot of the fifo's head */  
  char                   * va_end;        /* Snapshot of the fifo's end */  
  MUHWI_PacketHeader_t   * hdr;
  MUSPI_Fifo_t  *fifo_ptr = &rec_fifo->_fifo;
  
  va_head = (char *) fifo_ptr->va_head;  
  va_end  = (char *) fifo_ptr->va_end;  
  MUSPI_assert ((va_head < (char *) fifo_ptr->va_tail) || 
		(va_head < va_end - 544UL));
  
  hdr = (MUHWI_PacketHeader_t *) va_head;
  *pbytes   = (hdr->NetworkHeader.pt2pt.Byte8.Size + 1) << 5;
  va_head += *pbytes;
  fifo_ptr->va_head = va_head;
  return hdr;
}


/**
 * \brief Get the Pointer and Length of the Next Packet - Wrap
 *
 * When in a reception fifo wrap situation, get the pointer and length of the
 * next packet to be processed.  This function will work whether the packet
 * wraps or not.  However, if the packet does not wrap, it is most efficient to
 * call MUSPI_getNextPacketOptimized().  
 *
 * When the packet does wrap, this function copies the packet pieces from the
 * fifo into a temporary buffer within the rec_fifo structure and returns a
 * pointer to that.
 *
 * \param[in]  rec_fifo  Pointer to reception fifo structure.
 * \param[out] pbytes    Pointer to the number of bytes in the packet.
 *
 * \returns  Pointer to the next packet to be processed, and number of bytes
 *           in that packet.
 *
 * \see MUSPI_getNextPacketOptimized()
 */

__INLINE__ MUHWI_PacketHeader_t *MUSPI_getNextPacketWrap (
						     MUSPI_RecFifo_t  *rec_fifo,
						     uint32_t         *pbytes) 
{
  
  void                   * va_head;         /* Snapshot of the fifo's head */  
  MUHWI_PacketHeader_t   * hdr;
  MUSPI_Fifo_t  *fifo_ptr = &rec_fifo->_fifo;
  int end_bytes=0, start_bytes=0;

  va_head = fifo_ptr->va_head;  
  /*MUSPI_assert (va_head <= fifo_ptr->va_tail);*/
  
  hdr = (MUHWI_PacketHeader_t *) va_head;
  *pbytes   = (hdr->NetworkHeader.pt2pt.Byte8.Size + 1) << 5;

  if ((uint64_t)va_head + *pbytes < (uint64_t)(fifo_ptr->va_end)) {
    fifo_ptr->va_head = (void*)((uint64_t)va_head + *pbytes);
    return hdr;
  }

  end_bytes = (uint64_t)(fifo_ptr->va_end) - (uint64_t)va_head;  
  start_bytes = *pbytes - end_bytes;

  memcpy (rec_fifo->_wrapBuf, va_head, end_bytes);
  memcpy (rec_fifo->_wrapBuf + end_bytes, fifo_ptr->va_start, start_bytes);

  fifo_ptr->va_head = (void*)((uint64_t)(fifo_ptr->va_start) + start_bytes);
  
  return (MUHWI_PacketHeader_t *) rec_fifo->_wrapBuf;
}


/**
 * \brief Sync the Reception Fifo Head Pointer
 *
 * Updates the hardware reception fifo head pointer with the value of the 
 * head stored in the fifo structure (the shadow).
 * 
 * param[in]  rec_fifo  Pointer to reception fifo structure.
 */
__INLINE__ void MUSPI_syncRecFifoHwHead (MUSPI_RecFifo_t  *rec_fifo) 
{  
  void                  * va_head;         /* Snapshot of the fifo's head */  
  MUSPI_Fifo_t  *fifo_ptr = &rec_fifo->_fifo;
  
  va_head = fifo_ptr->va_head;
  MUSPI_setHwHead ( fifo_ptr, (uint64_t)va_head - (uint64_t)(fifo_ptr->va_start) );
}


/**
 * \brief Reception Fifo Inline Poll Function Example
 *
 * An example that demonstrates the use of the Reception fifo
 * inlines to poll packets. This poll routine only returns when all
 * packets have been processed from the reception fifo.
 * An alternative to using these inlines is to call the MUSPI_RecFifoPoll
 * function after registering receive functions.
 *
 * \param[in]  rfifo  Reception fifo structure.
 * \param[in]  fn     Function pointer to the function to be called to
 *                    handle each packet.
 * \param[in]  clientdata  Pointer to client data to be passed to the
 *                         receive function fn.
 */

__INLINE__ void MUSPI_RecFifoExamplePoll (MUSPI_RecFifo_t       * rfifo,
					  MUSPI_RecvFunction_t    fn,
					  void                  * clientdata)
{    
  uint32_t wrap = 0;
  uint32_t cur_bytes = 0;
  uint32_t total_bytes = 0;
  uint32_t cumulative_bytes = 0;
  MUHWI_PacketHeader_t *hdr = 0;

  while ((total_bytes = MUSPI_getAvailableBytes (rfifo, &wrap)) != 0) 
  {
    if (wrap == 0) 
    {
      /* No fifo wrap.  Process each packet. */
      cumulative_bytes = 0;
      while (cumulative_bytes < total_bytes ) 
      {
	hdr = MUSPI_getNextPacketOptimized (rfifo, &cur_bytes);
	(*fn)(clientdata, hdr, cur_bytes); /* Call user-defined packet handler. */
	cumulative_bytes += cur_bytes;
	/* Touch head for next packet. */
      }
    }
    else 
    {
      /* Packets wrap around to the top of the fifo.  Handle the one packet
       * that wraps.
       */
      hdr = MUSPI_getNextPacketWrap (rfifo, &cur_bytes);
      (*fn)(clientdata, hdr, cur_bytes); /* Call user-defined packet handler.   */
    }

    /* Store the updated fifo head. */
    MUSPI_syncRecFifoHwHead (rfifo);
  }
}


/**
 * \brief Get a Reception Fifo Pointer Given an ID and Reception Fifo Subgroup
 *
 * \param[in]  fifoid  Number of the fifo, relative to the fifo subgroup
 *                     (0 to BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP).
 * \param[in]  fg_ptr  Pointer to reception fifo subgroup structure.
 *
 * \returns  Pointer to reception fifo structure.
 */
__INLINE__ MUSPI_RecFifo_t *MUSPI_IdToRecFifo (int fifoid,
					       MUSPI_RecFifoSubGroup_t * fg_ptr) 
{  
  return &fg_ptr->_recfifos[fifoid];
}


/**
 * \brief Clear MU Fifo Interrupts
 *
 * Clear MU fifo interrupts for an MU group.
 *
 * \param[in]  sg_ptr  Pointer to a Reception Fifo SubGroup structure.
 *                     This is used to access the MU MMIO storage.  It can be
 *                     a reception fifo subgroup structure from any of the
 *                     subgroups in the group to be cleared.
 * \param[in]  mask  Indicates which interrupts to clear.
 *                   - Bits  0 through 31 clear injection fifo threshold crossing
 *                   - Bits 32 through 47 clear reception fifo threshold crossing
 *                   - Bits 48 through 63 clear reception fifo packet arrival
 */
__INLINE__ void MUSPI_ClearInterrupts ( MUSPI_RecFifoSubGroup_t * sg_ptr,
					uint64_t mask )
{
  if ( mask ) sg_ptr->groupStatus->clearInterruptStatus = mask;
}


/**
 * \brief Get the Direct Pointer to the MU MMIO Clear Interrupt Status
 *
 * \param[in]  sg_ptr  Pointer to a Reception Fifo SubGroup structure.
 *                     This is used to access the MU MMIO storage.  It can be
 *                     a reception fifo subgroup structure from any of the
 *                     subgroups in the group to be cleared.
 *
 * \returns  clearInterruptStatusPtr  Pointer to the MU MMIO clearInterruptStatus
 *
 * \see MUSPI_ClearInterruptsDirect()
 */
__INLINE__ volatile uint64_t *MUSPI_GetClearInterruptsStatusPtr ( MUSPI_RecFifoSubGroup_t * sg_ptr )
{
  return &sg_ptr->groupStatus->clearInterruptStatus;
}


/**
 * \brief Clear MU Fifo Interrupts Using Direct Pointer
 *
 * Clear MU fifo interrupts for an MU group.  The pointer to the MU MMIO storage
 * where the clearInterruptStatus is located is provided (faster).
 *
 * \param[in]  clearInterruptStatusPtr  Pointer to the clearInterruptStatus in the
 *                                      MU MMIO.
 * \param[in]  mask  Indicates which interrupts to clear.
 *                   - Bits  0 through 31 clear injection fifo threshold crossing
 *                   - Bits 32 through 47 clear reception fifo threshold crossing
 *                   - Bits 48 through 63 clear reception fifo packet arrival
 *
 * \see MUSPI_GetClearInterruptStatusPtr()
 */
__INLINE__ void MUSPI_ClearInterruptsDirect ( volatile uint64_t * clearInterruptStatusPtr,
					      uint64_t mask )
{
  if ( mask ) *clearInterruptStatusPtr = mask;
}

__END_DECLS


#endif
