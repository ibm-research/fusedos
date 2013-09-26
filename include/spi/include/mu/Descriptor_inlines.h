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

#ifndef	_MUSPI_DESCRIPTOR_INLINES_H_ /* Prevent multiple inclusion */
#define	_MUSPI_DESCRIPTOR_INLINES_H_


/**
 * \file Descriptor_inlines.h
 *
 * \brief C Header File containing Message Unit SPI Descriptor Inline Functions
 *
 *
 * \note The interfaces in DescriptorBaseXX.h are C++ versions of the
 *       interfaces in this file.  A change to one should be done in the
 *       other.
 *
 * \note
 * Functions that set fields into the descriptor:
 *
 * - MUSPI_SetDataPacketType
 * - MUSPI_SetMessageUnitPacketType
 *
 * - MUSPI_SetBaseFields
 *   - MUSPI_SetPrefetchOnly
 *   - MUSPI_SetPayload
 *   - MUSPI_SetTorusInjectionFIFOMap
 *   - MUSPI_SetDestination
 *
 * - MUSPI_SetPt2PtFields
 *   - MUSPI_SetHints
 *   - MUSPI_SetHintsABCD
 *   - MUSPI_SetPt2PtMisc1
 *   - MUSPI_SetRouteToIONode
 *   - MUSPI_SetReturnFromIONode
 *   - MUSPI_SetRouting
 *   - MUSPI_SetDeposit
 *   - MUSPI_SetPt2PtMisc2
 *   - MUSPI_SetPt2PtVirtualChannel
 *   - MUSPI_SetZoneRouting
 *   - MUSPI_SetStayOnBubble
 *   - MUSPI_SetPt2PtSkip
 *
 * - MUSPI_SetCollectiveFields
 *   - MUSPI_SetOpCode
 *   - MUSPI_SetWordLength
 *   - MUSPI_SetClassRoute
 *   - MUSPI_SetCollectiveMisc
 *   - MUSPI_SetCollectiveVirtualChannel
 *   - MUSPI_SetCollectiveType
 *   - MUSPI_SetCollectiveSkip
 *
 * - MUSPI_SetMemoryFIFOFields
 *   - MUSPI_SetRecFIFOId
 *   - MUSPI_SetRecPutOffMUSPI_Set
 *   - MUSPI_SetInterrupt
 *   - MUSPI_SetSoftwareBit
 *   - MUSPI_SetSoftwareBytes
 *   - MUSPI_SetSoftwareByte
 *
 * - MUSPI_SetDirectPutFields
 *   - MUSPI_SetRecPayloadBaseAddressInfo
 *   - MUSPI_SetRecCounterBaseAddressInfo
 *   - MUSPI_SetPacing
 *
 * - MUSPI_SetRemoteGetFields
 *   - MUSPI_SetRemoteGetInjFIFOId
 */

#include "kernel_impl.h"
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Descriptor.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Util.h>


__BEGIN_DECLS


/******************************************************************************/
/**** Miscellaneous Descriptor Helper Functions *******************************/
/******************************************************************************/

/**
 * \brief Zero Out All Fields of a Descriptor
 *
 * This zeros out all 64 bytes of the descriptor, including
 * the packet header model.
 *
 * \param[in]  desc  Pointer to descriptor to be zero'd.
 *
 * \post The descriptor is zero'd.
 *
 */

__INLINE__ 
void  MUSPI_DescriptorZeroOut (
			       MUHWI_Descriptor_t *desc
			      )
{
/*   MUHWI_assert( desc != NULL ); */

  uint64_t *addr = (uint64_t *) desc ;

  /* Generates 8 stores */
  addr[0] = 0;
  addr[1] = 0;
  addr[2] = 0;
  addr[3] = 0;
  addr[4] = 0;
  addr[5] = 0;
  addr[6] = 0;
  addr[7] = 0;
}


/**
 * \brief Determine the Number of Packet Chunks for the First Packet of a
 *        Message
 *
 * Compute the best (largest) packet size in units of 32B chunks given the
 * message length.
 *
 * Note:  This function always returns 16 (a full packet) since the MU 
 *        automatically adjusts the packet based on the message length.
 *
 * \param[in]  message_length  Message length
 *
 * \retval  numPacketChunks  Number of 32B chunks needed in the first packet
 *                           of a message whose length is msg_len.
 *                           This will be a number from 0 through 16.
 */

__INLINE__ 
uint8_t  MUSPI_DescriptorGetNumPacketChunks(
					    uint64_t message_length
					   )
{
  /* The following calculates the number of chunks based on the message_length.
   * However, the MU automatically adjusts the packet chunks based on the
   * message length, so always set 16 in the packet (to save cycles building
   * descriptors).
   */
#if 0
  /* Do most common case of a nearly full packet first */
  if ( message_length > (512 - 32) ) return 16;

  /* Round message length to 32B multiple:
   *  (message_length + 31) / 32
   */
  return ( message_length + 31 ) >> 5;
#else
  return (16);
#endif
}


/**
 * \brief Set Up the Destination Structure
 *
 * Set the specified coordinates of the destination node into the
 * destination structure.
 *
 * \param[in]  dest  Pointer to destination structure to be set up.
 * \param[in]  A     A coordinate.
 * \param[in]  B     B coordinate.
 * \param[in]  C     C coordinate.
 * \param[in]  D     D coordinate.
 * \param[in]  E     E coordinate.
 *
 */

__INLINE__ 
void  MUSPI_SetUpDestination ( MUHWI_Destination_t *dest,
			       uint32_t             A,
			       uint32_t             B,
			       uint32_t             C,
			       uint32_t             D,
			       uint32_t             E
			     )
{
  dest->Destination.Destination = 
    (A << 24) | (B << 18) | (C << 12) | (D << 6) | E;
}


/******************************************************************************/
/**** Functions That Set Common Descriptor Fields *****************************/
/******************************************************************************/


/**
 *
 * \brief Set the Data Packet Type
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  type  Packet type.  One of:
 *		       - MUHWI_PT2PT_DATA_PACKET_TYPE
 *                     - MUHWI_COLLECTIVE_DATA_PACKET_TYPE
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetDataPacketType ( MUHWI_Descriptor_t *desc,
			       uint8_t             type )
{
  /* Set pointer to the network header portion of the packet header.  
   * Note:  The field being set is in the same place in both the pt2pt and 
   *        the collective packet headers, so we can use one of those
   *        structures to set the field.
   */
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Data_Packet_Type = type; /* Set the packet type. */
}


/**
 *
 * \brief Set Message Unit Packet Type
 *
 * Specify the type of message unit packet associated with this descriptor.
 *
 * \param[in] desc  Pointer to descriptor.
 * \param[in] type  Message Unit Packet Type.  One of:
 *                       - MUHWI_PACKET_TYPE_FIFO
 *			 - MUHWI_PACKET_TYPE_PUT
 *                       - MUHWI_PACKET_TYPE_GET
 *                       - MUHWI_PACKET_TYPE_PACED_GET
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetMessageUnitPacketType( MUHWI_Descriptor_t *desc,
				     uint8_t             type )
{
  uint8_t byte8;

  /* Set pointer to the network header portion of the packet header.  
   * Note:  The field being set is in the same place in both the pt2pt and 
   *        the collective packet headers, so we can use one of those
   *        structures to set the field.
   */
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  byte8 = pkth->Byte8.Byte8 & 0x3F; /* Turn off packet type bits */
  pkth->Byte8.Byte8 = byte8 | (type & 0xC0); /* Set the MU packet type */
}


/******************************************************************************/
/**** Functions That Set Base Descriptor Fields *******************************/
/******************************************************************************/


/**
 *
 * \brief Set Base Fields
 *
 * Sets most commonly-set fields.
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to commonly-set values to be set.
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetBaseFields( MUHWI_Descriptor_t               *desc,
			  MUSPI_BaseDescriptorInfoFields_t *info )
{
  /* Set pointer to the network header portion of the packet header.  
   * Note:  The field being set is in the same place in both the pt2pt and 
   *        the collective packet headers, so we can use one of those
   *        structures to set the field.
   */
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  desc->Half_Word0.Half_Word0 = info->Pre_Fetch_Only; /* Set pre-fetch-only.  */

  desc->Pa_Payload = info->Payload_Address; /* Set payload physical address.  */

  desc->Message_Length = info->Message_Length; /* Set message length.         */

  pkth->Byte8.Size  = 16; /* Set number of packet chunks to the max.  The MU
			   * will adjust it from there.
			   */

  desc->Torus_FIFO_Map = info->Torus_FIFO_Map;  /* Set torus fifo map. */
    
  pkth->Destination = info->Dest; /* Set the destination. */
}


/**
 *
 * \brief Set the "Prefetch Only" indicator.
 *
 * The "prefetch only" indicator tells the MU that the data should not be
 * transferred.  It should only be fetched into L2 cache.
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  indicator  Prefetch indicator.  One of:
 *                        - MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_YES
 *                        - MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetPrefetchOnly( MUHWI_Descriptor_t *desc,
			    uint32_t            indicator )
{
  desc->Half_Word0.Half_Word0 &= 0xFFFFFFFE;      /* Turn off indicator. */
  desc->Half_Word0.Half_Word0 |= indicator & 0x1; /* Set indicator.      */
}


/**
 *
 * \brief Set the Size field.
 *
 * This field is hard-coded to 16, the largest number of chunks in a
 * packet.  The MU adjusts this automatically based on the message length.
 *
 * \param[in]  desc       Pointer to descriptor.
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetSize ( MUHWI_Descriptor_t   *desc )
{
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 
  pkth->Byte8.Size = 16;
}


/**
 *
 * \brief Set the Payload Location and Length.
 *
 * \param[in]  desc           Pointer to descriptor.
 * \param[in]  payloadAddress Physical address of the payload to be sent.
 * \param[in]  messageLength  Length of the payload, in bytes.
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetPayload ( MUHWI_Descriptor_t   *desc,
			uint64_t              payloadAddress,
			uint64_t              messageLength
		      )
{
  desc->Pa_Payload = payloadAddress; /* Set payload physical address. */
  
  desc->Message_Length = messageLength;  /* Set message length. */

}


/**
 *
 * \brief Set the Torus Injection FIFO Map
 *
 * \param[in]  desc          Pointer to descriptor.
 * \param[in]  torusFIFOMap  Torus injection FIFO map.  A 1 in bit i
 *                           indicates that the message can be
 *   		       	injected into Torus injection FIFO i.
 *				0 <= i <= 15.
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetTorusInjectionFIFOMap ( MUHWI_Descriptor_t *desc,
				      uint64_t            torusFIFOMap )
{
  desc->Torus_FIFO_Map = torusFIFOMap; /* Set torus fifo map. */
}


/**
 *
 * \brief Set the Destination Coordinates
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  dest  Coordinates of destination
 *
 * \ingroup COMMON_DESC
 *
 */

__INLINE__ 
void MUSPI_SetDestination ( MUHWI_Descriptor_t *desc,
			    MUHWI_Destination_t dest )
{
  /* Set pointer to the network header portion of the packet header.  
   * Note:  The field being set is in the same place in both the pt2pt and 
   *        the collective packet headers, so we can use one of those
   *        structures to set the field.
   */
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Destination = dest; /* Set the destination. */
}


/******************************************************************************/
/**** Functions That Set Point-to-Point Descriptor Fields *********************/
/******************************************************************************/


/**
 *
 * \brief Set Point-to-Point Fields
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to point-to-point values to be set.
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetPt2PtFields( MUHWI_Descriptor_t                *desc,
			   MUSPI_Pt2PtDescriptorInfoFields_t *info )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 
  
  pkth->Hints = info->Hints_ABCD; /* Set ABCD hints.           */
  
  pkth->Byte2.Byte2 = info->Misc1;/* Set all byte2 fields
				     at once.                  */
  
  pkth->Byte3.Byte3 = info->Misc2;/* Set all byte3 fields
				     at once.                  */
  
  pkth->Injection_Info.Skip = info->Skip; /* Set number of 4-byte words to
                                             skip for checksum.  */
}


/**
 *
 * \brief Set the Hint Bits
 *
 * This function sets all of the hint bits, and no others.  Use this when
 * you want to only set the hint bits.  If you are setting all of the fields
 * in the descriptor, use setHintsABCD() and setPt2PtMisc1() to set the hint
 * bits and others in the same field.
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  hintsABCD  Hint bits for network routing.
 *			     Each bit corresponds to A+, A-, B+, B-,
 *    		     C+, C-, D+, D-.  If a bit is set, it 
 *			     indicates that the packet wants to travel
 *			     along the corresponding direction.  If
 *			     all bits are zero, the hardware
 *			     calculates the hint bits.  Both A+ and A-
 *			     cannot be set at the same time...same
 *			     with B, C, and D.  Refer to 
 *			     MUHWI_PACKET_HINT_xx definitions.
 * \param[in]  hintsE     Hint bits for network routing.
 *                        Only bits 0 and 1 are used.
 *                        One of:
 *                        - MUHWI_PACKET_HINT_E_NONE
 *                        - MUHWI_PACKET_HINT_EP
 *                        - MUHWI_PACKET_HINT_EM
 *                        Only one of these bits can be set.
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setHintsABCD()
 * \see setPt2PtMisc1()
 *
 */

__INLINE__ 
void MUSPI_SetHints ( MUHWI_Descriptor_t *desc,
		      uint8_t             hintsABCD,
		      uint8_t             hintsE
		    )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Hints = hintsABCD;
  
  pkth->Byte2.Byte2 &= 0x3F;          /* Turn of E bits. */
  pkth->Byte2.Byte2 |= hintsE & 0xC0; /* OR in E bits.   */
}


/**
 *
 * \brief Set the ABCD Direction Hint Bits
 *
 * This function sets all of the ABCD direction hint bits, but not the E
 * direction hint bits.  If you are setting all of the fields
 * in the descriptor, use setHintsABCD() and setPt2PtMisc1() to set the hint
 * bits and others in the same field.  If you are only setting the hint bits,
 * use setHints().
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  hintsABCD  Hint bits for network routing.
 *			     Each bit corresponds to A+, A-, B+, B-,
 *    		             C+, C-, D+, D-.  If a bit is set, it 
 *			     indicates that the packet wants to travel
 *			     along the corresponding direction.  If
 *			     all bits are zero, the hardware
 *			     calculates the hint bits.  Both A+ and A-
 *			     cannot be set at the same time...same
 *			     with B, C, and D.  Refer to 
 *			     MUHWI_PACKET_HINT_xx definitions.
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setHints()
 * \see setPt2PtMisc1()
 *
 */

__INLINE__ 
void MUSPI_SetHintsABCD ( MUHWI_Descriptor_t *desc,
			  uint8_t             hintsABCD )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Hints = hintsABCD;
}


/**
 *
 * \brief Set the Point-to-Point Miscellaneous 1 Bits
 *
 * Set all of the miscellaneous 1 bits.  If you only want to set a subset of
 * these bits, use the individual setter functions.
 *
 * \param[in]  desc   Pointer to descriptor.
 * \param[in]  misc1  The miscellaneous 1 bits.  These include the following
 *                    OR'd together:
 *                    - Hint bits for E direction.  One of:
 *                      - MUHWI_PACKET_HINT_E_NONE
 *                      - MUHWI_PACKET_HINT_EP
 *                      - MUHWI_PACKET_HINT_EM
 *                    - Route to I/O Node.  One of:
 *                      - MUHWI_PACKET_ROUTE_TO_IO_NODE
 *                      - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE
 *                    - Return from I/O Node.  One of:
 *                      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6
 *                      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7
 *                      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT
 *                    - Routing.  One of:
 *                      - MUHWI_PACKET_USE_DYNAMIC_ROUTING
 *                      - MUHWI_PACKET_USE_DETERMINISTIC_ROUTING
 *                    - Deposit bit.  One of:
 *                      - MUHWI_PACKET_DEPOSIT
 *                      - MUHWI_PACKET_DO_NOT_DEPOSIT
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setHints()
 * \see setRouteToIONode()
 * \see setReturnFromIONode()
 * \see setRouting()
 * \see setDeposit()
 *
 */

__INLINE__ 
void MUSPI_SetPt2PtMisc1 ( MUHWI_Descriptor_t *desc,
			   uint8_t             misc1 )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte2.Byte2 = misc1;
}


/**
 *
 * \brief Set the Route To I/O Node Indicator
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  indicator  Route to I/O Node Indicator.  One of:
 *                        - MUHWI_PACKET_ROUTE_TO_IO_NODE
 *                        - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRouteToIONode( MUHWI_Descriptor_t *desc,
			     uint8_t             indicator )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte2.Byte2 &= 0xDF;             /* Turn off indicator. */
  pkth->Byte2.Byte2 |= indicator & 0x20; /* Set indicator.      */
}


/**
 *
 * \brief Set the Return From I/O Node Indicator.
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  port  Port to use when returning from I/O Node.  One of:
 *                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6
 *                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7
 *                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT
 *                   Only bits 3 and 4 are used.
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetReturnFromIONode( MUHWI_Descriptor_t *desc,
				uint8_t             port )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte2.Byte2 &= 0xE7;        /* Turn off port bits. */
  pkth->Byte2.Byte2 |= port & 0x18; /* Set port bits.      */
}


/**
 *
 * \brief Set the Routing Indicator (Dynamic or Deterministic)
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  indicator  Routing indicator.  One of:
 *                        - MUHWI_PACKET_USE_DYNAMIC_ROUTING
 *                        - MUHWI_PACKET_USE_DETERMINISTIC_ROUTING
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRouting ( MUHWI_Descriptor_t *desc,
			uint8_t             indicator )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte2.Byte2 &= 0xFB;             /* Turn off indicator. */
  pkth->Byte2.Byte2 |= indicator & 0x04; /* Set indicator.      */
}


/**
 *
 * \brief Set the Deposit Indicator
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  indicator  Deposit indicator.  One of:
 *                        - MUHWI_PACKET_DEPOSIT
 *                        - MUHWI_PACKET_DO_NOT_DEPOSIT
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetDeposit ( MUHWI_Descriptor_t *desc,
			uint8_t             indicator )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte2.Byte2 &= 0xFD;             /* Turn off indicator. */
  pkth->Byte2.Byte2 |= indicator & 0x02; /* Set indicator.      */
}


/**
 *
 * \brief Set the Point-to-Point Miscellaneous 2 Bits
 *
 * Set all of the miscellaneous 2 bits.  If you only want to set a subset of
 * these bits, use the individual setter functions.
 *
 * \param[in]  desc   Pointer to descriptor.
 * \param[in]  misc2  The miscellaneous 2 bits.  These include the following
 *                    OR'd together:
 *			 - Virtual Channel.  One of:
 *		           - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
 *		         - Zone Routing Id.  One of
 *		           - MUHWI_PACKET_ZONE_ROUTING_0
 *		           - MUHWI_PACKET_ZONE_ROUTING_1
 *		           - MUHWI_PACKET_ZONE_ROUTING_2
 *		           - MUHWI_PACKET_ZONE_ROUTING_3
 *			   Used only with dynamic routing.  This Id
 *			   refers to one of 4 sets of DCRs containing
 *			   zone masks.  For selecting a dynamic direction,
 *			   the packet hint bits are ANDed with the
 *			   appropriate zone mask to restrict the set of
 *			   dynamic directions that may be chosen.  For a
 *		           given set of DCRs identified by the zone
 *		           routine Id, the first mask is used until
 *		           the destination in the first dimension is
 *		           reached, then the next mask is used for
 *		           the next dimension, etc.  
 *		         - Stay on Bubble.  Used only with dynamic
 *		           routing.  One of:
 *		           - MUHWI_PACKET_STAY_ON_BUBBLE
 *			   - MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setVirtualChannel()
 * \see setZoneRoutingId()
 * \see setStayOnBubble()
 *
 */

__INLINE__ 
void MUSPI_SetPt2PtMisc2 ( MUHWI_Descriptor_t *desc,
			   uint8_t             misc2 )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte3.Byte3 = misc2;
}


/**
 *
 * \brief Set Point-to-Point Virtual Channel
 *
 * \param[in]  desc     Pointer to descriptor.
 * \param[in]  channel  Virtual Channel.  One of:
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
 *			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setPt2PtMisc2()
 *
 */

__INLINE__ 
void MUSPI_SetPt2PtVirtualChannel ( MUHWI_Descriptor_t *desc,
				    uint8_t             channel )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte3.Byte3 &= 0x1F;           /* Turn off virtual channel bits. */
  pkth->Byte3.Byte3 |= channel & 0xE0; /* Set virtual channel bits.      */
}


/**
 *
 * \brief Set Zone Routing
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  zone  Zone Routing.  One of:
 *		        - MUHWI_PACKET_ZONE_ROUTING_0
 *		        - MUHWI_PACKET_ZONE_ROUTING_1
 *		        - MUHWI_PACKET_ZONE_ROUTING_2
 *		        - MUHWI_PACKET_ZONE_ROUTING_3
 *		        Used only with dynamic routing.  This Id
 *		        refers to one of 4 sets of DCRs containing
 *		        zone masks.  For selecting a dynamic direction,
 *		        the packet hint bits are ANDed with the
 *		        appropriate zone mask to restrict the set of
 *		        dynamic directions that may be chosen.  For a
 *		        given set of DCRs identified by the zone
 *	                routine Id, the first mask is used until
 *	                the destination in the first dimension is
 *	                reached, then the next mask is used for
 *	                the next dimension, etc.  
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setPt2PtMisc2()
 *
 */

__INLINE__ 
void MUSPI_SetZoneRouting ( MUHWI_Descriptor_t *desc,
			    uint8_t             zone )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte3.Byte3 &= 0xE7;        /* Turn off zone routing bits. */
  pkth->Byte3.Byte3 |= zone & 0x18; /* Turn on zone routing bits.  */
}


/**
 *
 * \brief Set Stay On Bubble
 *
 * Used only with dynamic routing.
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  stay  Stay On Bubble.  One of:
 *			- MUHWI_PACKET_STAY_ON_BUBBLE
 *			- MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE
 *
 * \ingroup POINT_TO_POINT_DESC
 * \see setPt2PtMisc2()
 *
 */

__INLINE__ 
void MUSPI_SetStayOnBubble ( MUHWI_Descriptor_t *desc,
			     uint8_t             stay )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Byte3.Byte3 &= 0xFB;        /* Turn off stay on bubble bit. */
  pkth->Byte3.Byte3 |= stay & 0x04; /* Set stay on bubble bits.     */
}


/**
 *
 * \brief Set Point-to-Point Skip
 *
 * Set the number of 4-byte words to skip in the first packet of the message
 * for injection checksum.
 *
 * \param[in]  desc      Pointer to descriptor.
 * \param[in]  skip      Number of 4-byte words to skip (0 to 136).
 *
 * \ingroup POINT_TO_POINT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetPt2PtSkip ( MUHWI_Descriptor_t *desc,
			  uint16_t            skip )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  pkth->Injection_Info.Skip = skip; /* Set skip bits. */
}


/******************************************************************************/
/**** Functions That Set Collective Descriptor Fields *********************/
/******************************************************************************/


/**
 *
 * \brief Set Collective Fields
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to collective values to be set.
 *
 * \ingroup COLLECTIVE_DESC
 *
 */

__INLINE__ 
void MUSPI_SetCollectiveFields( MUHWI_Descriptor_t                     *desc,
				MUSPI_CollectiveDescriptorInfoFields_t *info )
{
  /* Word_Length (in bytes) must be mapped to the power of 2 exponent and
   * the exponent must be stored into the descriptor.  The following is the
   * mapping from the Word_Length specified in the info structure to the
   * Exponent:
   *     Word_Length in bytes     Exponent
   *         4                     0
   *         8                     1
   *        16                     2
   *        32                     3
   *        64                     4
   *       128                     5
   *       256                     6
   *       512                     7
   * To do this, subtract 1 from Word_Length, count the non-zero bits, and
   * subtract 2.
   */

  uint64_t wordLength         = info->Word_Length - 1;
  unsigned int numNonZeroBits = popcnt64 ( wordLength );

  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte1.Byte1 = info->Op_Code | (( numNonZeroBits - 2 ) & 0x7);

  pkth->Byte2.Byte2 = info->Class_Route << 4;

  pkth->Byte3.Byte3 = info->Misc;
  
  pkth->Injection_Info.Skip = info->Skip; /* Set number of 4-byte words to
                                             skip for checksum. */
}


/**
 *
 * \brief Set the Collective Op Code
 *
 * The collective op code is ignored when the collective type is "broadcast".
 *
 * \param[in]  desc    Pointer to descriptor.
 * \param[in]  opCode  Collective Op Code.  One of:
 *		       - MUHWI_COLLECTIVE_OP_CODE_AND
 *		       - MUHWI_COLLECTIVE_OP_CODE_OR
 *		       - MUHWI_COLLECTIVE_OP_CODE_XOR
 *		       - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD
 *		       - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN
 *		       - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MAX
 *		       - MUHWI_COLLECTIVE_OP_CODE_SIGNED_ADD
 *		       - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MIN
 *		       - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MAX
 *		       - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_ADD
 *		       - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MIN
 *		       - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MAX.
 *
 * \ingroup COLLECTIVE_DESC
 *
 */

__INLINE__ 
void MUSPI_SetOpCode ( MUHWI_Descriptor_t *desc,
		       uint8_t             opCode )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte1.Byte1 = ( pkth->Byte1.Byte1 & 0x0F ) | opCode;
}


/**
 *
 * \brief Set the Collective Word Length
 *
 * The collective word length is the number of bytes to be operated on by
 * the collective opCode in an individual collective operation.  Word Length
 * is ignored when the collective type is "broadcast".
 *
 * \param[in]  desc        Pointer to descriptor.
 * \param[in]  wordLength  Number of bytes in a "collective word".
 *                         The Op_Code will be performed on elements of
 *                         this length.  Message_Length must be a
 *                         multiple of wordLength.  wordLength must
 *                         be one of:  4, 8, 16, 32, 64, 128, 256, 512.
 *
 * \ingroup COLLECTIVE_DESC
 *
 */

__INLINE__ 
void MUSPI_SetWordLength ( MUHWI_Descriptor_t *desc,
			   uint8_t             wordLength )
{
  /* wordLength (in bytes) must be mapped to the power of 2 exponent and
   * the exponent must be stored into the descriptor.  The following is the
   * mapping from the wordLength to the Exponent:
   *     Word_Length in bytes     Exponent
   *         4                     0
   *         8                     1
   *        16                     2
   *        32                     3
   *        64                     4
   *       128                     5
   *       256                     6
   *       512                     7
   * To do this, subtract 1 from Word_Length, count the non-zero bits, and
   * subtract 2.
   */

  uint64_t myWordLength       = wordLength - 1;
  unsigned int numNonZeroBits = popcnt64 ( myWordLength );

  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte1.Byte1 = ( pkth->Byte1.Byte1 & 0xF0 ) | (( numNonZeroBits - 2 ) & 0x7);

}


/**
 *
 * \brief Set the Collective Class Route
 *
 * \param[in]  desc        Pointer to descriptor.
 * \param[in]  classRoute  The collective class route (0 through 15).
 *
 * \ingroup COLLECTIVE_DESC
 *
 */

__INLINE__ 
void MUSPI_SetClassRoute ( MUHWI_Descriptor_t *desc,
			   uint8_t             classRoute )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte2.Byte2 = ( pkth->Byte2.Byte2 & 0x0F ) | classRoute << 4;
}


/**
 *
 * \brief Set the Collective Misc
 *
 * \param[in]  desc        Pointer to descriptor.
 * \param[in]  misc        Miscellaneous constants.  OR together the following
 *			   constants:
 * 		           - Virtual Channel.  One of:
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
 *			      - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
 *                         - Collective Type.  One of:
 *		              - MUHWI_COLLECTIVE_TYPE_BROADCAST
 *			      - MUHWI_COLLECTIVE_TYPE_REDUCE
 *			      - MUHWI_COLLECTIVE_TYPE_ALLREDUCE
 *			 
 *
 * \ingroup COLLECTIVE_DESC
 */

__INLINE__ 
void MUSPI_SetCollectiveMisc ( MUHWI_Descriptor_t *desc,
			       uint8_t             misc )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte3.Byte3 = misc;
}


/**
 *
 * \brief Set the Collective Virtual Channel
 *
 * \param[in]  desc            Pointer to descriptor.
 * \param[in]  virtualChannel  - Virtual Channel.  One of:
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
 *			         - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
 *
 * \ingroup COLLECTIVE_DESC
 */

__INLINE__ 
void MUSPI_SetCollectiveVirtualChannel ( MUHWI_Descriptor_t *desc,
					 uint8_t             virtualChannel )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte3.Byte3 = ( pkth->Byte3.Byte3 & 0x1F ) | virtualChannel;
}


/**
 *
 * \brief Set the Collective Type
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  type  Collective Type.  One of:
 *                   - MUHWI_COLLECTIVE_TYPE_BROADCAST
 *                   - MUHWI_COLLECTIVE_TYPE_REDUCE
 *                   - MUHWI_COLLECTIVE_TYPE_ALLREDUCE
 *
 * \ingroup COLLECTIVE_DESC
 */

__INLINE__ 
void MUSPI_SetCollectiveType ( MUHWI_Descriptor_t *desc,
			       uint8_t             type )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Byte3.Byte3 = ( pkth->Byte3.Byte3 & 0xE7 ) | type;
}


/**
 *
 * \brief Set Collective Skip
 *
 * Set the number of 4-byte words to skip in the first packet of the message
 * for injection checksum.
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  skip  Number of 4-byte words to skip (0 to 136).
 *
 * \ingroup COLLECTIVE_DESC
 *
 */

__INLINE__ 
void MUSPI_SetCollectiveSkip ( MUHWI_Descriptor_t *desc,
			       uint16_t            skip )
{
  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_CollectiveNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.collective); 

  pkth->Injection_Info.Skip = skip; /* Set skip bits. */
}


/******************************************************************************/
/**** Functions That Set Point-to-Point Memory FIFO Descriptor Fields *********/
/******************************************************************************/


/**
 *
 * \brief Set Memory FIFO Fields
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to memory FIFO values to be set.
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetMemoryFIFOFields( 
			     MUHWI_Descriptor_t                     *desc,
			     MUSPI_MemoryFIFODescriptorInfoFields_t *info )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id =
    info->Rec_FIFO_Id;                   /* Set reception FIFO Id.         */
  
  muh->Packet_Types.Memory_FIFO.Put_Offset_MSB =
    ( info->Rec_Put_Offset >> 32 ) & 0x01f; /* Set most significant
					       5 bits of the 37-bit
					       put offset.           */
  
  muh->Packet_Types.Memory_FIFO.Put_Offset_LSB =
    info->Rec_Put_Offset & 0xFFFFFFFF;              /* Set least significant
						       32 bits of the 37-bit
						       put offset.           */
  
  muh->Packet_Types.Memory_FIFO.Unused1 =
    (info->SoftwareBit & 0x01);             /* Set unused bit to be passed
					       unchanged to the destination.   */
  
  memcpy( muh->Packet_Types.Memory_FIFO.Unused2,
	  info->SoftwareBytes,
	  sizeof( muh->Packet_Types.Memory_FIFO.Unused2 ) ); /* Set unused
								bytes to be
								passed
								unchanged to
								destination. */
  
  desc->Half_Word1.Half_Word1 = info->Interrupt;            /* Set interrupt 
							       indicator.    */
}


/**
 *
 * \brief Set the Reception FIFO Id
 *
 * Set the reception fifo Id to receive the packet into the descriptor.
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  id    The reception FIFO Id (0 to 271).
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRecFIFOId( MUHWI_Descriptor_t *desc,
			 uint16_t            id )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id = id; /* Set id. */
}


/**
 *
 * \brief Set the Reception Put Offset
 *
 * Set the initial value for the reception side put offset in the first
 * packet.  Hardware updates this in each packet.  For a memory FIFO packet,
 * this serves as a packet counter.  For a single packet  message, this is
 * unused/unchanged by the hardware, so it may be set to anything by software.
 *
 * \param[in]  desc    Pointer to descriptor.
 * \param[in]  offset  The reception side put offset.
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRecPutOffset( MUHWI_Descriptor_t *desc,
			    uint64_t            offset )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Memory_FIFO.Put_Offset_MSB = (offset >> 32) & 0x1F; /* Set
							      * most significant
							      * 5 bits.
							      */
    muh->Packet_Types.Memory_FIFO.Put_Offset_LSB = offset & 0xFFFFFFFF; /* Set
							      * least significant
							      * 32 bits.
							      */
}


/**
 *
 * \brief Set the "Interrupt" indicator.
 *
 * The "interrupt" indicator tells the MU to set the "interrupt on packet
 * arrival" bit in the last packet of the message associated with this
 * descriptor.
 *
 * \param[in]  desc       Pointer to descriptor.
 * \param[in]  indicator  Interrupt indicator.  One of:
 *                        - MUHWI_DESCRIPTOR_INTERRUPT_ON_PACKET_ARRIVAL
 *                        - MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetInterrupt( MUHWI_Descriptor_t *desc,
			 uint32_t            indicator )
{
  desc->Half_Word1.Half_Word1 &= 0xfffffffe;      /* Turn off indicator. */
  desc->Half_Word1.Half_Word1 |= indicator & 0x1; /* Set indicator.      */
}


/**
 *
 * \brief Set the "Software Bit" (unused bit).
 *
 * The "software bit" is an unused bit in the memory FIFO packet header that
 * is passed unchanged to the destination.
 *
 * \param[in]  desc         Pointer to descriptor.
 * \param[in]  softwareBit  Software bit.  0 or 1.
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetSoftwareBit( MUHWI_Descriptor_t *desc,
			   uint8_t             softwareBit )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Memory_FIFO.Unused1 = (softwareBit & 0x01);
}


/**
 *
 * \brief Set the "Software Bytes" (unused bytes).
 *
 * The "software bytes" are unused bytes in the memory FIFO packet header that
 * are passed unchanged to the destination.
 *
 * \param[in]  desc           Pointer to descriptor.
 * \param[in]  softwareBytes  Software bytes (14 bytes).
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetSoftwareBytes( MUHWI_Descriptor_t *desc,
			     uint8_t             softwareBytes[] )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  memcpy( muh->Packet_Types.Memory_FIFO.Unused2,
	  softwareBytes,
	  sizeof( muh->Packet_Types.Memory_FIFO.Unused2 ) );
}


/**
 *
 * \brief Set one of the "Software Bytes" (unused bytes).
 *
 * The "software bytes" are unused bytes in the memory FIFO packet header that
 * are passed unchanged to the destination.  This function sets one of the
 * bytes, as specified.
 *
 * \param[in]  desc           Pointer to descriptor.
 * \param[in]  byteNumber     Specifies which of the 14 byes to set (0..13).
 * \param[in]  byteValue      Software byte value to be set.
 *
 * \ingroup MEMORY_FIFO_DESC
 *
 */

__INLINE__ 
void MUSPI_SetSoftwareByte( MUHWI_Descriptor_t *desc,
			    uint8_t             byteNumber,
			    uint8_t             byteValue )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Memory_FIFO.Unused2[byteNumber] = byteValue;
}


/******************************************************************************/
/**** Functions That Set Point-to-Point Direct Put Descriptor Fields **********/
/******************************************************************************/


/**
 *
 * \brief Set Direct Put Fields
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to direct put values to be set.
 *
 * \ingroup DIRECT_PUT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetDirectPutFields( 
			     MUHWI_Descriptor_t                    *desc,
			     MUSPI_DirectPutDescriptorInfoFields_t *info )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Direct_Put.Pacing = info->Pacing;/* Set pacing 
							indicator.            */
  
  muh->Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
    info->Rec_Payload_Base_Address_Id; /* Set payload base address id.        */
  
  muh->Packet_Types.Direct_Put.Put_Offset_MSB =
    ( info->Rec_Payload_Offset >> 32 ) & 0x01f; /* Set most significant 5 bits
					           of the 37-bit put offset.  */
  
  muh->Packet_Types.Direct_Put.Put_Offset_LSB =
    info->Rec_Payload_Offset & 0xFFFFFFFF; /* Set least significant 32 bits
					      of the 37-bit put offset.       */
  
  muh->Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
    info->Rec_Counter_Base_Address_Id; /* Set reception counter base
					  address id.                         */
  
  muh->Packet_Types.Direct_Put.Counter_Offset =
    info->Rec_Counter_Offset;                /* Set reception counter offset. */
}


/**
 *
 * \brief Set the Reception Payload Base Address Info
 *
 * Set the reception payload base address Id and offset.
 *
 * \param[in] desc       Pointer to descriptor.
 * \param[in] id         The reception payload base address id.
 * \param[in] offset     The reception payload offset from the
 *                       base address associated with the specified
 *                       base address id.
 *
 * \ingroup DIRECT_PUT_DESC
 */

__INLINE__ 
void MUSPI_SetRecPayloadBaseAddressInfo( MUHWI_Descriptor_t   *desc,
					 uint16_t              id,
					 uint64_t              offset )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 

  muh->Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id = id; /* Set id. */

  muh->Packet_Types.Direct_Put.Put_Offset_MSB =
    ( offset >> 32 ) & 0x01f; /* Set most significant 5 bits
				 of the 37-bit put offset.     */

  muh->Packet_Types.Direct_Put.Put_Offset_LSB =
    offset & 0xFFFFFFFF;      /* Set least significant 32 bits */
}


/**
 *
 * \brief Set the Reception Counter Base Address Info
 *
 * Set the reception counter base address Id and offset.
 *
 * \param[in] desc       Pointer to descriptor.
 * \param[in] id         The reception counter base address id.
 * \param[in] offset     The reception counter offset from the
 *                       base address associated with the specified
 *                       base address id.
 *
 * \ingroup DIRECT_PUT_DESC
 */

__INLINE__ 
void MUSPI_SetRecCounterBaseAddressInfo( MUHWI_Descriptor_t   *desc,
					 uint16_t              id,
					 uint64_t              offset )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 

  muh->Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id = id; /* Set id. */

  muh->Packet_Types.Direct_Put.Counter_Offset = offset; /* Set offset. */

}


/**
 *
 * \brief Set Pacing
 *
 * Specify whether this direct put is a response to a paced remote get.
 *
 * \param[in] desc       Pointer to descriptor.
 * \param[in] indicator  Indicates whether this direct put descriptor is
 *                       involved in pacing.  One of:
 *                       - MUHWI_PACKET_DIRECT_PUT_IS_PACED
 *			    - MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED
 *
 * \ingroup DIRECT_PUT_DESC
 *
 */

__INLINE__ 
void MUSPI_SetPacing( MUHWI_Descriptor_t *desc,
		      uint8_t             indicator )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Direct_Put.Pacing = indicator;
}


/******************************************************************************/
/**** Functions That Set Point-to-Point Remote Get Descriptor Fields **********/
/******************************************************************************/


/**
 *
 * \brief Set Remote Get Fields
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  info  Pointer to remote get values to be set.
 *
 * \ingroup REMOTE_GET_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRemoteGetFields( 
			     MUHWI_Descriptor_t                    *desc,
			     MUSPI_RemoteGetDescriptorInfoFields_t *info )
{
  uint8_t byte8;

  /* Set pointer to the network header portion of the packet header. */ 
  MUHWI_Pt2PtNetworkHeader_t *pkth = &(desc->PacketHeader.NetworkHeader.pt2pt); 

  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 

  byte8 = pkth->Byte8.Byte8 & 0x3F; // Turn off packet type bits.
  pkth->Byte8.Byte8 = byte8 | (info->Type & 0xC0); // Set the 
                                                   // message unit packet type.

  muh->Packet_Types.Remote_Get.Rget_Inj_FIFO_Id =
    info->Rget_Inj_FIFO_Id;    /* Set remote get injection FIFO Id */
}


/**
 *
 * \brief Set Remote Get Injection FIFO Id
 *
 * \param[in]  desc  Pointer to descriptor.
 * \param[in]  id    The FIFO Id on the destination node where the payload
 *                   descriptors will be injected (0 to 543).
 *
 * \ingroup REMOTE_GET_DESC
 *
 */

__INLINE__ 
void MUSPI_SetRemoteGetInjFIFOId( MUHWI_Descriptor_t *desc,
				  uint16_t            id )
{
  /* Set pointer to message unit header within the packet header. */
  MUHWI_MessageUnitHeader_t *muh = 
    &(desc->PacketHeader.messageUnitHeader); 
  
  muh->Packet_Types.Remote_Get.Rget_Inj_FIFO_Id = id;
}


__END_DECLS


#endif /* _MUSPI_DESCRIPTOR_INLINES_H_ */
