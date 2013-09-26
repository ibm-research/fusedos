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


#ifndef _MUSPI_PT2PT_REMOTE_GET_DESCRIPTOR_XX_H_ /* Prevent multiple inclusion */
#define _MUSPI_PT2PT_REMOTE_GET_DESCRIPTOR_XX_H_


#include <stdint.h>
#include <hwi/include/common/compiler_support.h>
#include <spi/include/mu/DescriptorBaseXX.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file Pt2PtRemoteGetDescriptorXX.h
///
/// \brief C++ Header File containing Message Unit SPI Point-to-Point Remote
///        Get Descriptor Class Definitions
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_Pt2PtRemoteGetDescriptor Class
///
/// This class defines the Point-to-Point Remote Get Descriptor which will be
/// injected into a memory injection FIFO.  It is derived from the Base
/// descriptor.  It contains only the base descriptor member data.
/// The setter and getter functions in the base descriptor class operate on the 
/// descriptor's data members.
///
/// \verbatim
/// DescriptorWrapperXX.h      Pt2PtRemoteGetDescriptorXX.h &
/// ---------------------      DescriptorBaseXX.h
/// | DescriptorWrapper |      -----------------------------
/// | - desc ----------------->| Pt2PtRemoteGetDescriptor  |
/// | - cb_done         |      | - DescriptorBase          |
/// | - sequenceNumber  |      -----------------------------
/// | - nextPtr/prevPtr |
/// | - FIFO number     |
/// ---------------------
/// \endverbatim
///
/// The default constructor zeros out the descriptor and sets the point-to-point
/// remote get packet type.  Beyond that, the following setter functions
/// (defined in the MUSPI_DescriptorBase class) can be used to set the
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
///   - setVirtualChannel
///   - setZoneRouting
///   - setStayOnBubble
///   - setSkip
///
/// - setPt2PtRemoteGetFields
///   - setRemoteGetInjFIFOId
///
/// The full constructor sets all of the fields, given input in an information
/// structure.  The setter functions listed above can be used to modify the
/// fields afterwards.
///
/// \see DescriptorBaseXX.h
///
////////////////////////////////////////////////////////////////////////////////

class MUSPI_Pt2PtRemoteGetDescriptor : public MUSPI_DescriptorBase
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_Pt2PtRemoteGetDescriptor Default Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets point-to-point remote get packet type fields.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_Pt2PtRemoteGetDescriptor() :
  MUSPI_DescriptorBase( MUHWI_PT2PT_DATA_PACKET_TYPE,
			MUHWI_PACKET_TYPE_GET )
  {}


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_Pt2PtRemoteGetDescriptor Full Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets point-to-point remote get packet type fields.
  /// Sets most commonly-set fields.
  /// Sets point-to-point fields.
  /// Sets remote get fields.
  ///
  /// \param[in]  info  Pointer to point-to-point remote get information.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_Pt2PtRemoteGetDescriptor( MUSPI_Pt2PtRemoteGetDescriptorInfo_t *info ) :
    MUSPI_DescriptorBase( MUHWI_PT2PT_DATA_PACKET_TYPE,
			  info->RemoteGet.Type )
  {
    this->setBaseFields( &info->Base );
    this->setPt2PtFields( &info->Pt2Pt );
    this->setRemoteGetFields( &info->RemoteGet );
  }


}; // End: class MUSPI_Pt2PtRemoteGetDescriptor




#endif /* _MUSPI_PT2PT_REMOTE_GET_DESCRIPTOR_XX_H_ */
