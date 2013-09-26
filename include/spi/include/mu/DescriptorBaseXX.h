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


#ifndef _MUSPI_DESCRIPTOR_BASE_XX_H_ // Prevent multiple inclusion
#define _MUSPI_DESCRIPTOR_BASE_XX_H_


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file DescriptorBaseXX.h
///
/// \brief C++ Header File containing Message Unit SPI Base Descriptor Class
///        Definitions
///
/// \see Descriptor.h and Descriptor_inlines.h for C interfaces
///
/// \note The interfaces in Descriptor_inlines.h are C versions of the
///       interfaces in this class.  A change to one should be done in the
///       other.
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_DescriptorBase Class
///
/// This class contains only the base MUHWI_Descriptor_t structure which will
/// ultimately be injected into a memory injection FIFO.  It has no other member
/// data.  There are setter and getter functions to operate on the descriptor's
/// data members.
///
/// This class is intended to be used as a base class for the various types
/// of descriptors.  Thus, users of descriptors should instantiate those
/// derived classes, not this base class.  Derived classes are defined in other
/// include files.
///
/// The derived classes document which setter functions must be called to set
/// all of the necessary fields into the descriptor.  There are also optional
/// setters that may be called.
///
/// \see MemoryFIFODescriptorXX.h
/// \see DirectPutDescriptorXX.h
/// \see RemoteGetDescriptorXX.h
/// \see Descriptor.h and Descriptor_inlines.h for C interfaces.
///
////////////////////////////////////////////////////////////////////////////////

class MUSPI_DescriptorBase : public MUHWI_Descriptor_t 
{
 public:

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup COMMON_DESC Common Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are common to
  /// all descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup POINT_TO_POINT_DESC Point-to-Point Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are only in
  /// point-to-point descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup COLLECTIVE_DESC Collective Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are only in
  /// collective descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup MEMORY_FIFO_DESC Memory FIFO Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are only in
  /// memory FIFO descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup DIRECT_PUT_DESC Direct Put Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are only in
  /// direct put descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \defgroup REMOTE_GET_DESC Remote Get Descriptor Functions
  ///
  /// This is a collection of functions that set/get fields that are only in
  /// remote get descriptors.
  ///
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_DescriptorBase Default Constructor
  ///
  /// Zeros out the descriptor.
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_DescriptorBase()
  {
    MUSPI_DescriptorZeroOut( this );
  }

  //Descriptor used for cloning
  MUSPI_DescriptorBase(int dummy)  
  {
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_DescriptorBase Main Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets packet type fields.
  ///
  /// \param[in]  data_packet_type  The type of the data packet.  One of:
  ///			            - MUHWI_PT2PT_DATA_PACKET_TYPE
  ///                               - MUHWI_COLLECTIVE_DATA_PACKET_TYPE
  /// \param[in]  MU_packet_type    The message unit packet type.  One of:
  ///                               - MUHWI_PACKET_TYPE_FIFO
  ///			            - MUHWI_PACKET_TYPE_PUT
  ///                               - MUHWI_PACKET_TYPE_GET
  ///                               - MUHWI_PACKET_TYPE_PACED_GET
  ///
  /// \ingroup COMMON_DESC
  ///
  /// \note
  /// This main constructor zeros out the descriptor and sets the packet type.
  /// Beyond that, the following setter functions can be used to set the
  /// remaining fields:
  ///
  /// - setDataPacketType
  /// - setMessageUnitPacketType
  ///
  /// - setBaseFields
  ///   - setPrefetchOnly
  ///   - setPayload
  ///   - setTorusInjectionFIFOMap
  ///   - setDestination
  ///
  /// - setPt2PtFields
  ///   - setHints
  ///   - setHintsABCD
  ///   - setPt2PtMisc1
  ///   - setRouteToIONode
  ///   - setReturnFromIONode
  ///   - setRouting
  ///   - setDeposit
  ///   - setPt2PtMisc2
  ///   - setPt2PtVirtualChannel
  ///   - setZoneRouting
  ///   - setStayOnBubble
  ///   - setPt2PtSkip
  ///
  /// - setCollectiveFields
  ///   - setOpCode
  ///   - setWordLength
  ///   - setClassRoute
  ///   - setCollectiveMisc
  ///   - setCollectiveVirtualChannel
  ///   - setCollectiveType
  ///   - setCollectiveSkip
  ///
  /// - setPt2PtMemoryFIFOFields
  ///   - setRecFIFOId
  ///   - setRecPutOffset
  ///   - setInterrupt
  ///   - setSoftwareBit
  ///   - setSoftwareBytes
  ///   - setSoftwareByte
  ///
  /// - setPt2PtDirectPutFields
  ///   - setRecPayloadBaseAddressInfo
  ///   - setRecCounterBaseAddressInfo
  ///   - setPacing
  ///
  /// - setPt2PtRemoteGetFields
  ///   - setRemoteGetInjFIFOId
  ///
  /// - clone
  /// - dump
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_DescriptorBase(	uint8_t data_packet_type,
			uint8_t MU_packet_type )
  {
    MUSPI_DescriptorZeroOut( this );

    /* Set pointer to the network header portion of the packet header.  
     * Note:  The field being set is in the same place in both the pt2pt and 
     *        the collective packet headers, so we can use one of those
     *        structures to set the field.
     */
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
    
    pkth->Data_Packet_Type = data_packet_type; /* Set the packet type. */

    uint8_t byte8 = pkth->Byte8.Byte8 & 0x3F; /* Turn off packet type bits */
    pkth->Byte8.Byte8 = byte8 | (MU_packet_type & 0xC0); /* Set the MU packet type */

    pkth->Byte8.Size  = 16; /* Set number of packet chunks to 16 (max number).
			     * The MU adjusts it automatically.
			     */
  }


  ///***************************************************************************
  ///*** Functions That Set Common Descriptor Fields ***************************
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Data Packet Type
  ///
  /// \param[in]  type  Packet type.  One of:
  ///			  - MUHWI_PT2PT_DATA_PACKET_TYPE
  ///                     - MUHWI_COLLECTIVE_DATA_PACKET_TYPE
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDataPacketType ( uint8_t type )
  {
    /* Set pointer to the network header portion of the packet header.  
     * Note:  The field being set is in the same place in both the pt2pt and 
     *        the collective packet headers, so we can use one of those
     *        structures to set the field.
     */
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
    
    pkth->Data_Packet_Type = type; /* Set the packet type. */
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Message Unit Packet Type
  ///
  /// Specify the type of message unit packet associated with this descriptor.
  ///
  /// \param[in] type  Message Unit Packet Type.  One of:
  ///                       - MUHWI_PACKET_TYPE_FIFO
  ///			    - MUHWI_PACKET_TYPE_PUT
  ///                       - MUHWI_PACKET_TYPE_GET
  ///                       - MUHWI_PACKET_TYPE_PACED_GET
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setMessageUnitPacketType( uint8_t type )
  {
    uint8_t byte8;
    
    /* Set pointer to the network header portion of the packet header.  
     * Note:  The field being set is in the same place in both the pt2pt and 
     *        the collective packet headers, so we can use one of those
     *        structures to set the field.
     */
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
    
    byte8 = pkth->Byte8.Byte8 & 0x3F; /* Turn off packet type bits */
    pkth->Byte8.Byte8 = byte8 | (type & 0xC0); /* Set the MU packet type */
  }


  ///***************************************************************************
  ///*** Functions That Set Base Descriptor Fields *****************************
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Base Fields
  ///
  /// Sets most commonly-set fields.
  ///
  /// \param[in]  info  Pointer to commonly-set values to be set.
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setBaseFields( MUSPI_BaseDescriptorInfoFields_t *info )
  {
    // Set pointer to the network header portion of the packet header.  
    // Note:  The field being set is in the same place in both the pt2pt and 
    //        the collective packet headers, so we can use one of those
    //        structures to set the field.
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 

    this->Half_Word0.Half_Word0 = info->Pre_Fetch_Only; // Set pre-fetch-only.

    this->Pa_Payload = info->Payload_Address; // Set payload physical address.

    this->Message_Length = info->Message_Length;  // Set message length.

    this->Torus_FIFO_Map = info->Torus_FIFO_Map;  // Set torus fifo map.
    
    pkth->Destination = info->Dest; // Set the destination.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the "Prefetch Only" indicator.
  ///
  /// The "prefetch only" indicator tells the MU that the data should not be
  /// transferred.  It should only be fetched into L2 cache.
  ///
  /// \param[in]  indicator  Prefetch indicator.  One of:
  ///                        - MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_YES
  ///                        - MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPrefetchOnly( uint32_t indicator )
  {
    this->Half_Word0.Half_Word0 &= 0xFFFFFFFE;       // Turn off indicator.
    this->Half_Word0.Half_Word0 |= indicator & 0x1;  // Set indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Payload Location and Length.
  ///
  /// \param[in]  payloadAddress Physical address of the payload to be sent.
  /// \param[in]  messageLength  Length of the payload, in bytes.
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPayload ( uint64_t payloadAddress,
			   uint64_t messageLength
			 )
  {
    this->Pa_Payload = payloadAddress; /* Set payload physical address. */

    this->Message_Length = messageLength;  // Set message length.

  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Torus Injection FIFO Map
  ///
  /// \param[in]  torusFIFOMap  Torus injection FIFO map.  A 1 in bit i
  ///                           indicates that the message can be
  ///   		       	injected into Torus injection FIFO i.
  ///				0 <= i <= 15.
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setTorusInjectionFIFOMap ( uint64_t torusFIFOMap )
  {
    this->Torus_FIFO_Map = torusFIFOMap; // Set torus fifo map.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Destination Coordinates
  ///
  /// \param[in]  dest  Coordinates of destination
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDestination ( MUHWI_Destination_t dest )
  {
    // Set pointer to the network header portion of the packet header.  
    // Note:  The field being set is in the same place in both the pt2pt and 
    //        the collective packet headers, so we can use one of those
    //        structures to set the field.
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 

    pkth->Destination = dest; // Set the destination.
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Destination Coordinates as a 32bit integer
  ///
  /// \param[in]  dest  Coordinates of destination
  ///
  /// \ingroup COMMON_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDestination ( uint32_t dest )
  {
    // Set pointer to the network header portion of the packet header.  
    // Note:  The field being set is in the same place in both the pt2pt and 
    //        the collective packet headers, so we can use one of those
    //        structures to set the field.
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 

    pkth->Destination.Destination.Destination = dest; // Set the destination.
  }

  ///***************************************************************************
  ///*** Functions That Set Point-to-Point Descriptor Fields *******************
  ///***************************************************************************

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Point-to-Point Fields
  ///
  /// \param[in]  info  Pointer to point-to-point values to be set.
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPt2PtFields( MUSPI_Pt2PtDescriptorInfoFields_t *info )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.
  
    pkth->Hints = info->Hints_ABCD; // Set ABCD hints.

    pkth->Byte2.Byte2 = info->Misc1;// Set all byte2 fields at once.     

    pkth->Byte3.Byte3 = info->Misc2;// Set all byte3 fields at once.

    pkth->Injection_Info.Skip = info->Skip; // Set number of 4-byte words to
                                            // skip for checksum.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Hint Bits
  ///
  /// This function sets all of the hint bits, and no others.  Use this when
  /// you want to only set the hint bits.  If you are setting all of the fields
  /// in the descriptor, use setHintsABCD() and setPt2PtMisc1() to set the hint
  /// bits and others in the same field.
  ///
  /// \param[in]  hintsABCD  Hint bits for network routing.
  ///			     Each bit corresponds to A+, A-, B+, B-,
  ///    		     C+, C-, D+, D-.  If a bit is set, it 
  ///			     indicates that the packet wants to travel
  ///			     along the corresponding direction.  If
  ///			     all bits are zero, the hardware
  ///			     calculates the hint bits.  Both A+ and A-
  ///			     cannot be set at the same time...same
  ///			     with B, C, and D.  Refer to 
  ///			     MUHWI_PACKET_HINT_xx definitions.
  /// \param[in]  hintsE     Hint bits for network routing.
  ///                        Only bits 0 and 1 are used.
  ///                        One of:
  ///                        - MUHWI_PACKET_HINT_E_NONE
  ///                        - MUHWI_PACKET_HINT_EP
  ///                        - MUHWI_PACKET_HINT_EM
  ///                        Only one of these bits can be set.
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setHintsABCD()
  /// \see setPt2PtMisc1()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setHints ( uint8_t hintsABCD,
			 uint8_t hintsE
		       )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Hints = hintsABCD;

    pkth->Byte2.Byte2 &= 0x3F;          // Turn of E bits.
    pkth->Byte2.Byte2 |= hintsE & 0xC0; // OR in E bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the ABCD Direction Hint Bits
  ///
  /// This function sets all of the ABCD direction hint bits, but not the E
  /// direction hint bits.  If you are setting all of the fields
  /// in the descriptor, use setHintsABCD() and setPt2PtMisc1() to set the hint
  /// bits and others in the same field.  If you are only setting the hint bits,
  /// use setHints().
  ///
  /// \param[in]  hintsABCD  Hint bits for network routing.
  ///			     Each bit corresponds to A+, A-, B+, B-,
  ///    		     C+, C-, D+, D-.  If a bit is set, it 
  ///			     indicates that the packet wants to travel
  ///			     along the corresponding direction.  If
  ///			     all bits are zero, the hardware
  ///			     calculates the hint bits.  Both A+ and A-
  ///			     cannot be set at the same time...same
  ///			     with B, C, and D.  Refer to 
  ///			     MUHWI_PACKET_HINT_xx definitions.
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setHints()
  /// \see setPt2PtMisc1()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setHintsABCD ( uint8_t hintsABCD )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Hints = hintsABCD;
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Point-to-Point Miscellaneous 1 Bits
  ///
  /// Set all of the miscellaneous 1 bits.  If you only want to set a subset of
  /// these bits, use the individual setter functions.
  ///
  /// \param[in]  misc1  The miscellaneous 1 bits.  These include the following
  ///                    OR'd together:
  ///                    - Hint bits for E direction.  One of:
  ///			   - MUHWI_PACKET_HINT_E_NONE
  ///			   - MUHWI_PACKET_HINT_EP
  ///			   - MUHWI_PACKET_HINT_EM
  ///			 - Route to I/O Node.  One of:
  ///                      - MUHWI_PACKET_ROUTE_TO_IO_NODE
  ///			   - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE
  ///                    - Return from I/O Node.  One of:
  ///			   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6
  ///			   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7
  ///			   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT
  ///			 - Routing.  One of:
  ///			   - MUHWI_PACKET_USE_DYNAMIC_ROUTING
  ///			   - MUHWI_PACKET_USE_DETERMINISTIC_ROUTING
  ///			 - Deposit bit.  One of:
  ///			   - MUHWI_PACKET_DEPOSIT
  ///			   - MUHWI_PACKET_DO_NOT_DEPOSIT
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setHints()
  /// \see setRouteToIONode()
  /// \see setReturnFromIONode()
  /// \see setRouting()
  /// \see setDeposit()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPt2PtMisc1 ( uint8_t misc1 )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte2.Byte2 = misc1;
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Route To I/O Node Indicator
  ///
  /// \param[in]  indicator  Route to I/O Node Indicator.  One of:
  ///                        - MUHWI_PACKET_ROUTE_TO_IO_NODE
  ///                        - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRouteToIONode( uint8_t indicator )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte2.Byte2 &= 0xDF;             // Turn off indicator.
    pkth->Byte2.Byte2 |= indicator & 0x20; // Set indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Return From I/O Node Indicator.
  ///
  /// \param[in]  port  Port to use when returning from I/O Node.  One of:
  ///                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6
  ///                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7
  ///                   - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT
  ///                   Only bits 3 and 4 are used.
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setReturnFromIONode( uint8_t port )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte2.Byte2 &= 0xE7; // Turn off port bits.
    pkth->Byte2.Byte2 |= port & 0x18; // Set port bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Routing Indicator (Dynamic or Deterministic)
  ///
  /// \param[in]  indicator  Routing indicator.  One of:
  ///                        - MUHWI_PACKET_USE_DYNAMIC_ROUTING
  ///                        - MUHWI_PACKET_USE_DETERMINISTIC_ROUTING
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRouting ( uint8_t indicator )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte2.Byte2 &= 0xFB;             // Turn off indicator.
    pkth->Byte2.Byte2 |= indicator & 0x04; // Set indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Deposit Indicator
  ///
  /// \param[in]  indicator  Deposit indicator.  One of:
  ///                        - MUHWI_PACKET_DEPOSIT
  ///                        - MUHWI_PACKET_DO_NOT_DEPOSIT
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDeposit ( uint8_t indicator )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte2.Byte2 &= 0xFD;             // Turn off indicator.
    pkth->Byte2.Byte2 |= indicator & 0x02; // Set indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Point-to-Point Miscellaneous 2 Bits
  ///
  /// Set all of the miscellaneous 2 bits.  If you only want to set a subset of
  /// these bits, use the individual setter functions.
  ///
  /// \param[in]  misc2  The miscellaneous 2 bits.  These include the following
  ///                    OR'd together:
  ///			 - Virtual Channel.  One of:
  ///		           - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
  ///		         - Zone Routing Id.  One of
  ///		           - MUHWI_PACKET_ZONE_ROUTING_0
  ///		           - MUHWI_PACKET_ZONE_ROUTING_1
  ///		           - MUHWI_PACKET_ZONE_ROUTING_2
  ///		           - MUHWI_PACKET_ZONE_ROUTING_3
  ///			   Used only with dynamic routing.  This Id
  ///			   refers to one of 4 sets of DCRs containing
  ///			   zone masks.  For selecting a dynamic direction,
  ///			   the packet hint bits are ANDed with the
  ///			   appropriate zone mask to restrict the set of
  ///			   dynamic directions that may be chosen.  For a
  ///		           given set of DCRs identified by the zone
  ///		           routine Id, the first mask is used until
  ///		           the destination in the first dimension is
  ///		           reached, then the next mask is used for
  ///		           the next dimension, etc.  
  ///		         - Stay on Bubble.  Used only with dynamic
  ///		           routing.  One of:
  ///		           - MUHWI_PACKET_STAY_ON_BUBBLE
  ///			   - MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setPt2PtVirtualChannel()
  /// \see setZoneRoutingId()
  /// \see setStayOnBubble()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPt2PtMisc2 ( uint8_t misc2 )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte3.Byte3 = misc2;
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Point-to-Point Virtual Channel
  ///
  /// \param[in]  channel  Virtual Channel.  One of:
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
  ///			   - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setPt2PtMisc2()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPt2PtVirtualChannel ( uint8_t channel )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte3.Byte3 &= 0x1F;           // Turn off virtual channel bits.
    pkth->Byte3.Byte3 |= channel & 0xE0; // Set virtual channel bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Zone Routing
  ///
  /// \param[in]  zone  Zone Routing.  One of:
  ///		        - MUHWI_PACKET_ZONE_ROUTING_0
  ///		        - MUHWI_PACKET_ZONE_ROUTING_1
  ///		        - MUHWI_PACKET_ZONE_ROUTING_2
  ///		        - MUHWI_PACKET_ZONE_ROUTING_3
  ///		        Used only with dynamic routing.  This Id
  ///		        refers to one of 4 sets of DCRs containing
  ///		        zone masks.  For selecting a dynamic direction,
  ///		        the packet hint bits are ANDed with the
  ///		        appropriate zone mask to restrict the set of
  ///		        dynamic directions that may be chosen.  For a
  ///		        given set of DCRs identified by the zone
  ///	                routine Id, the first mask is used until
  ///	                the destination in the first dimension is
  ///	                reached, then the next mask is used for
  ///	                the next dimension, etc.  
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setPt2PtMisc2()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setZoneRouting ( uint8_t zone )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte3.Byte3 &= 0xE7;        // Turn off zone routing bits.
    pkth->Byte3.Byte3 |= zone & 0x18; // Turn on zone routing bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Stay On Bubble
  ///
  /// Used only with dynamic routing.
  ///
  /// \param[in]  stay  Stay On Bubble.  One of:
  ///			- MUHWI_PACKET_STAY_ON_BUBBLE
  ///			- MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE
  ///
  /// \ingroup POINT_TO_POINT_DESC
  /// \see setPt2PtMisc2()
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setStayOnBubble ( uint8_t stay )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Byte3.Byte3 &= 0xFB;        // Turn off stay on bubble bit.
    pkth->Byte3.Byte3 |= stay & 0x04; // Set stay on bubble bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Point-to-Point Skip
  ///
  /// Set the number of 4-byte words to skip in the first packet of the message
  /// injection checksum.
  ///
  /// \param[in]  skip  Number of 4-byte words to skip (0 to 136).
  ///
  /// \ingroup POINT_TO_POINT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPt2PtSkip ( uint16_t skip )
  {
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 
                                                  // Set pointer to packet
                                                  // header portion of the
                                                  // descriptor.

    pkth->Injection_Info.Skip = skip; // Set skip bits.
  }


  ///***************************************************************************
  ///*** Functions That Set Collective Descriptor Fields ***********************
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Collective Fields
  ///
  /// \param[in]  info  Pointer to collective values to be set.
  ///
  /// \ingroup COLLECTIVE_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setCollectiveFields( MUSPI_CollectiveDescriptorInfoFields_t *info )
  {
    // Word_Length (in bytes) must be mapped to the power of 2 exponent and
    // the exponent must be stored into the descriptor.  The following is the
    // mapping from the Word_Length specified in the info structure to the
    // Exponent:
    //     Word_Length in bytes     Exponent
    //         4                     0
    //         8                     1
    //        16                     2
    //        32                     3
    //        64                     4
    //       128                     5
    //       256                     6
    //       512                     7
    // To do this, subtract 1 from Word_Length, count the non-zero bits, and
    // subtract 2.
    
    uint64_t wordLength         = info->Word_Length - 1;
    unsigned int numNonZeroBits = popcnt64 ( wordLength );
    
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte1.Byte1 = info->Op_Code | (( numNonZeroBits - 2 ) & 0x7);
    
    pkth->Byte2.Byte2 = info->Class_Route << 4;
    
    pkth->Byte3.Byte3 = info->Misc;
    
    pkth->Injection_Info.Skip = info->Skip; // Set number of 4-byte words to
                                            // skip for checksum.
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Op Code
  ///
  /// The collective op code is ignored when the collective type is "broadcast".
  ///
  /// \param[in]  opCode  Collective Op Code.  One of:
  ///	 	        - MUHWI_COLLECTIVE_OP_CODE_AND
  ///		        - MUHWI_COLLECTIVE_OP_CODE_OR
  ///	                - MUHWI_COLLECTIVE_OP_CODE_XOR
  ///	                - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD
  ///	                - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN
  ///	                - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MAX
  ///	                - MUHWI_COLLECTIVE_OP_CODE_SIGNED_ADD
  ///	                - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MIN
  ///	                - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MAX
  ///	                - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_ADD
  ///	                - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MIN
  ///	                - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MAX.
  ///
  /// \ingroup COLLECTIVE_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setOpCode ( uint8_t opCode )
  {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte1.Byte1 = ( pkth->Byte1.Byte1 & 0x0F ) | opCode;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Word Length
  ///
  /// The collective word length is the number of bytes to be operated on by
  /// the collective opCode in an individual collective operation.  Word Length
  /// is ignored when the collective type is "broadcast".
  ///
  /// \param[in]  wordLength  Number of bytes in a "collective word".
  ///                         The Op_Code will be performed on elements of
  ///                         this length.  Message_Length must be a
  ///                         multiple of wordLength.  wordLength must
  ///                         be one of:  4, 8, 16, 32, 64, 128, 256, 512.
  ///
  /// \ingroup COLLECTIVE_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setWordLength ( uint8_t wordLength )
  {
    // wordLength (in bytes) must be mapped to the power of 2 exponent and
    // the exponent must be stored into the descriptor.  The following is the
    // mapping from the wordLength to the Exponent:
    //     wordLength in bytes     Exponent
    //         4                     0
    //         8                     1
    //        16                     2
    //        32                     3
    //        64                     4
    //       128                     5
    //       256                     6
    //       512                     7
    // To do this, subtract 1 from wordLength, count the non-zero bits, and
    // subtract 2.
    
    uint64_t myWordLength       = wordLength - 1;
    unsigned int numNonZeroBits = popcnt64 ( myWordLength );
    
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte1.Byte1 = ( pkth->Byte1.Byte1 & 0xF0 ) | (( numNonZeroBits - 2 ) & 0x7);
    
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Class Route
  ///
  /// \param[in]  classRoute  The collective class route (0 through 15).
  ///
  /// \ingroup COLLECTIVE_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setClassRoute ( uint8_t classRoute )
  {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte2.Byte2 = ( pkth->Byte2.Byte2 & 0x0F ) | classRoute << 4;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Misc
  ///
  /// \param[in]  misc        Miscellaneous constants.  OR together the following
  ///			   constants:
  /// 		           - Virtual Channel.  One of:
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
  ///			      - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
  ///                         - Collective Type.  One of:
  ///		              - MUHWI_COLLECTIVE_TYPE_BROADCAST
  ///			      - MUHWI_COLLECTIVE_TYPE_REDUCE
  ///			      - MUHWI_COLLECTIVE_TYPE_ALLREDUCE
  ///			 
  ///
  /// \ingroup COLLECTIVE_DESC
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setCollectiveMisc ( uint8_t misc )
  {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte3.Byte3 = misc;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Virtual Channel
  ///
  /// \param[in]  virtualChannel  - Virtual Channel.  One of:
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
  ///			         - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
  ///
  /// \ingroup COLLECTIVE_DESC
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setCollectiveVirtualChannel ( uint8_t virtualChannel )
    {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte3.Byte3 = ( pkth->Byte3.Byte3 & 0x1F ) | virtualChannel;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Collective Type
  ///
  /// \param[in]  type  Collective Type.  One of:
  ///                   - MUHWI_COLLECTIVE_TYPE_BROADCAST
  ///                   - MUHWI_COLLECTIVE_TYPE_REDUCE
  ///                   - MUHWI_COLLECTIVE_TYPE_ALLREDUCE
  ///
  /// \ingroup COLLECTIVE_DESC
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setCollectiveType ( uint8_t type )
  {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Byte3.Byte3 = ( pkth->Byte3.Byte3 & 0xE7 ) | type;
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Collective Skip
  ///
  /// Set the number of 4-byte words to skip in the first packet of the message
  /// for injection checksum.
  ///
  /// \param[in]  skip  Number of 4-byte words to skip (0 to 136).
  ///
  /// \ingroup COLLECTIVE_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void setCollectiveSkip ( uint16_t skip )
  {
    // Set pointer to the network header portion of the packet header.  
    MUHWI_CollectiveNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.collective); 
    
    pkth->Injection_Info.Skip = skip; // Set skip bits. 
  }
  

  ///***************************************************************************
  ///*** Functions That Set Point-to-Point Memory FIFO Descriptor Fields *******
  ///***************************************************************************

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Memory FIFO Fields
  ///
  /// \param[in]  info  Pointer to memory FIFO values to be set.
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setMemoryFIFOFields( 
			     MUSPI_MemoryFIFODescriptorInfoFields_t *info )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id =
      info->Rec_FIFO_Id;                     // Set reception FIFO Id.

    muh->Packet_Types.Memory_FIFO.Put_Offset_MSB =
      ( info->Rec_Put_Offset >> 32 ) & 0x01f; // Set most significant
                                              // 5 bits of the 37-bit
					      // put offset.

    muh->Packet_Types.Memory_FIFO.Put_Offset_LSB =
      info->Rec_Put_Offset & 0xFFFFFFFF; // Set least significant
                                         // 32 bits of the 37-bit
					 // put offset.

    muh->Packet_Types.Memory_FIFO.Unused1 =
      (info->SoftwareBit & 0x01);        // Set unused bit to be passed
					 // unchanged to the destination.

    memcpy( muh->Packet_Types.Memory_FIFO.Unused2,
	    info->SoftwareBytes,
	    sizeof( muh->Packet_Types.Memory_FIFO.Unused2 ) ); // Set unused
							       // bytes to be
							       // passed
							       // unchanged to
							       // destination.

    this->Half_Word1.Half_Word1 = info->Interrupt;             // Set interrupt 
							       // indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Reception FIFO Id
  ///
  /// Set the reception fifo Id to receive the packet into the descriptor.
  ///
  /// \param[in]  id  The reception FIFO Id (0 to 271).
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRecFIFOId( uint16_t id )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id = id; // Set id.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Reception Put Offset
  ///
  /// Set the initial value for the reception side put offset in the first
  /// packet.  Hardware updates this in each packet.  For a memory FIFO packet,
  /// this serves as a packet counter.  For a single packet  message, this is
  /// unused/unchanged by the hardware, so it may be set to anything by software.
  ///
  /// \param[in] offset  The reception side put offset.
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRecPutOffset( uint64_t offset )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Memory_FIFO.Put_Offset_MSB = (offset >> 32) & 0x1F; // Set
                                                              // most significant 
                                                              // 5 bits.
    muh->Packet_Types.Memory_FIFO.Put_Offset_LSB = offset & 0xFFFFFFFF; // Set
                                                             // least significant
                                                             // 32 bits.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the "Interrupt" indicator.
  ///
  /// The "interrupt" indicator tells the MU to set the "interrupt on packet
  /// arrival" bit in the last packet of the message associated with this
  /// descriptor.
  ///
  /// \param[in]  indicator  Interrupt indicator.  One of:
  ///                        - MUHWI_DESCRIPTOR_INTERRUPT_ON_PACKET_ARRIVAL
  ///                        - MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setInterrupt( uint32_t indicator )
  {
    this->Half_Word1.Half_Word1 &= 0xfffffffe;       // Turn off indicator.
    this->Half_Word1.Half_Word1 |= indicator & 0x1;  // Set indicator.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the "Software Bit" (unused bit).
  ///
  /// The "software bit" is an unused bit in the memory FIFO packet header that
  /// is passed unchanged to the destination.
  ///
  /// \param[in]  softwareBit  Software bit.  0 or 1.
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setSoftwareBit( uint8_t softwareBit )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 
  
    muh->Packet_Types.Memory_FIFO.Unused1 = (softwareBit & 0x01);
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the "Software Bytes" (unused bytes).
  ///
  /// The "software bytes" are unused bytes in the memory FIFO packet headert
  /// that are passed unchanged to the destination.
  ///
  /// \param[in]  softwareBytes  Software bytes (14 bytes).
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setSoftwareBytes( uint8_t softwareBytes[] )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 
  
    memcpy( muh->Packet_Types.Memory_FIFO.Unused2,
	    softwareBytes,
	    sizeof( muh->Packet_Types.Memory_FIFO.Unused2 ) );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set one of the "Software Bytes" (unused bytes).
  ///
  /// The "software bytes" are unused bytes in the memory FIFO packet header that
  /// are passed unchanged to the destination.  This function sets one of the
  /// bytes, as specified.
  ///
  /// \param[in]  byteNumber     Specifies which of the 14 byes to set (0..13).
  /// \param[in]  byteValue      Software byte value to be set.
  ///
  /// \ingroup MEMORY_FIFO_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setSoftwareByte( uint8_t byteNumber,
			       uint8_t byteValue )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 
  
    muh->Packet_Types.Memory_FIFO.Unused2[byteNumber] = byteValue;
  }

  ///***************************************************************************
  ///*** Functions That Set Point-to-Point Direct Put Descriptor Fields ********
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Direct Put Fields
  ///
  /// \param[in]  info  Pointer to direct put values to be set.
  ///
  /// \ingroup DIRECT_PUT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDirectPutFields( 
			     MUSPI_DirectPutDescriptorInfoFields_t *info )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Direct_Put.Pacing = info->Pacing;// Set pacing indicator.

    muh->Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
      info->Rec_Payload_Base_Address_Id; // Set payload base address id.

    muh->Packet_Types.Direct_Put.Put_Offset_MSB =
      ( info->Rec_Payload_Offset >> 32 ) & 0x01f; // Set most significant 5 bits
						  // of the 37-bit put offset.
    
    muh->Packet_Types.Direct_Put.Put_Offset_LSB =
      info->Rec_Payload_Offset & 0xFFFFFFFF; // Set least significant 32 bits
					     // of the 37-bit put offset.

    muh->Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
      info->Rec_Counter_Base_Address_Id; // Set reception counter base
					 // address id.
  
    muh->Packet_Types.Direct_Put.Counter_Offset =
      info->Rec_Counter_Offset;          // Set reception counter offset.
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Reception Payload Base Address Info
  ///
  /// Set the reception payload base address id and offset.
  ///
  /// \param[in] id         The reception payload base address id.
  /// \param[in] offset     The reception payload offset from the
  ///                       base address associated with the specified
  ///                       base address id.
  ///
  /// \ingroup DIRECT_PUT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRecPayloadBaseAddressInfo( uint16_t id,
					    uint64_t offset )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id = id; // Set id.

    muh->Packet_Types.Direct_Put.Put_Offset_MSB =
      ( offset >> 32 ) & 0x01f; // Set most significant 5 bits
				// of the 37-bit put offset.
    
    muh->Packet_Types.Direct_Put.Put_Offset_LSB =
      offset & 0xFFFFFFFF;      // Set least significant 32 bits.

  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the Reception Counter Base Address Info
  ///
  /// Set the reception counter base address id and offset.
  ///
  /// \param[in] id         The reception counter base address id.
  /// \param[in] offset     The reception counter offset from the
  ///                       base address associated with the specified
  ///                       base address id.
  ///
  /// \ingroup DIRECT_PUT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRecCounterBaseAddressInfo( uint16_t id,
					    uint64_t offset )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id = id; // Set id.

    muh->Packet_Types.Direct_Put.Counter_Offset = offset; // Set offset.

  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Pacing
  ///
  /// Specify whether this direct put is a response to a paced remote get.
  ///
  /// \param[in] indicator  Indicates whether this direct put descriptor is
  ///                       involved in pacing.  One of:
  ///                       - MUHWI_PACKET_DIRECT_PUT_IS_PACED
  ///			    - MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED
  ///
  /// \ingroup DIRECT_PUT_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPacing( uint8_t indicator )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Direct_Put.Pacing = indicator;
  }


  ///***************************************************************************
  ///*** Functions That Set Point-to-Point Remote Get Descriptor Fields ********
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Remote Get Fields
  ///
  /// \param[in]  info  Pointer to remote get values to be set.
  ///
  /// \ingroup REMOTE_GET_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRemoteGetFields( 
			     MUSPI_RemoteGetDescriptorInfoFields_t *info )
  {
    uint8_t byte8;

    // Set pointer to the network header portion of the packet header.  
    // Note:  The field being set is in the same place in both the pt2pt and 
    //        the collective packet headers, so we can use one of those
    //        structures to set the field.
    MUHWI_Pt2PtNetworkHeader_t *pkth = &(this->PacketHeader.NetworkHeader.pt2pt); 

    MUHWI_MessageUnitHeader_t *muh = &(this->PacketHeader.messageUnitHeader);

    byte8 = pkth->Byte8.Byte8 & 0x3F; // Turn off packet type bits.
    pkth->Byte8.Byte8 = byte8 | (info->Type & 0xC0); // Set the 
                                                     // message unit packet type.

    muh->Packet_Types.Remote_Get.Rget_Inj_FIFO_Id =
      info->Rget_Inj_FIFO_Id;    // Set remote get injection FIFO Id.
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Remote Get Injection FIFO Id
  ///
  /// \param[in] id  The FIFO Id on the destination node where the payload
  ///                descriptors will be injected (0 to 543).
  ///
  /// \ingroup REMOTE_GET_DESC
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setRemoteGetInjFIFOId( uint16_t id )
  {
    // Set pointer to message unit header within the packet header.
    MUHWI_MessageUnitHeader_t *muh = 
      &(this->PacketHeader.messageUnitHeader); 

    muh->Packet_Types.Remote_Get.Rget_Inj_FIFO_Id = id;
  }


  ///***************************************************************************
  ///*** Miscellaneous Descriptor Functions ************************************
  ///***************************************************************************


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Clone the Descriptor
  ///
  /// This is a clone, copying "this" object to a specified DescriptorBase
  /// object.
  ///
  /// \param[in]  obj  Reference to the target DescriptorBase object to copy to.
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void clone (MUSPI_DescriptorBase & obj)
  {
    //Assume alignment
    memcpy( (void*)&obj, (void*)this, sizeof(MUSPI_DescriptorBase) );

    /* register double fp0 asm("fr0");
       register double fp1 asm("fr1"); */
    /* VECTOR_LOAD_NU (this,  0, fp0);  /\* Load first 32 bytes to reg 0*\/ */
    /* VECTOR_LOAD_NU (this, 32, fp1);  /\* Load second 32 bytes to reg 1*\/ */    
    /* VECTOR_STORE_NU((void*)&obj,  0, fp0);  */
    /* VECTOR_STORE_NU((void*)&obj, 32, fp1);    */
  }
   
  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Dump this descriptor
  ///
  //////////////////////////////////////////////////////////////////////////////
  
  inline void dump()
  {
    printf("%p: %08x %08x %08x %08x %08x %08x %08x %08x\n%08x %08x %08x %08x %08x %08x %08x %08x\n",
	   this,
	   (uint32_t)(*(((uint32_t*)this)+0)),
	   (uint32_t)(*(((uint32_t*)this)+1)),
	   (uint32_t)(*(((uint32_t*)this)+2)),
	   (uint32_t)(*(((uint32_t*)this)+3)),
	   (uint32_t)(*(((uint32_t*)this)+4)),
	   (uint32_t)(*(((uint32_t*)this)+5)),
	   (uint32_t)(*(((uint32_t*)this)+6)),
	   (uint32_t)(*(((uint32_t*)this)+7)),
	   (uint32_t)(*(((uint32_t*)this)+8)),
	   (uint32_t)(*(((uint32_t*)this)+9)),
	   (uint32_t)(*(((uint32_t*)this)+10)),
	   (uint32_t)(*(((uint32_t*)this)+11)),
	   (uint32_t)(*(((uint32_t*)this)+12)),
	   (uint32_t)(*(((uint32_t*)this)+13)),
	   (uint32_t)(*(((uint32_t*)this)+14)),
	   (uint32_t)(*(((uint32_t*)this)+15)));
  }

}; // End: class MUSPI_DescriptorBase




#endif // _MUSPI_DESCRIPTOR_BASE_XX_H_
