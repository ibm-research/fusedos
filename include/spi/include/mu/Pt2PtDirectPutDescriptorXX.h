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


#ifndef _MUSPI_PT2PT_DIRECT_PUT_DESCRIPTOR_XX_H_ /* Prevent multiple inclusion */
#define _MUSPI_PT2PT_DIRECT_PUT_DESCRIPTOR_XX_H_


#include <stdint.h>
#include <hwi/include/common/compiler_support.h>
#include <spi/include/mu/DescriptorBaseXX.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file Pt2PtDirectPutDescriptorXX.h
///
/// \brief C++ Header File containing Message Unit SPI Point-to-Point Direct 
///        Put Descriptor Class Definitions
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_Pt2PtDirectPutDescriptor Class
///
/// This class defines the Point-to-Point Direct Put Descriptor which will be
/// injected into a memory injection FIFO.  It is derived from the Base
/// descriptor.  It contains only the base descriptor member data.
/// The setter and getter functions in the base descriptor class operate on the 
/// descriptor's data members.
///
/// \verbatim
/// DescriptorWrapperXX.h      Pt2PtDirectPutDescriptorXX.h &
/// ---------------------      DescriptorBaseXX.h
/// | DescriptorWrapper |      -----------------------------
/// | - desc ----------------->| Pt2PtDirectPutDescriptor  |
/// | - cb_done         |      | - DescriptorBase          |
/// | - sequenceNumber  |      -----------------------------
/// | - nextPtr/prevPtr |
/// | - FIFO number     |
/// ---------------------
/// \endverbatim
///
/// The DescriptorWrapper (left hand box) is not the real descriptor.  The real 
/// descriptor is the only thing inside the right hand box.
///
/// The default constructor zeros out the descriptor and sets the point-to-point
/// direct put packet type.  Beyond that, the following setter functions
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
/// - setPt2PtDirectPutFields
///   - setRecPayloadBaseAddressInfo
///   - setRecCounterBaseAddressInfo
///   - setPacing
///
/// The full constructor sets all of the fields, given input in an information
/// structure.  The setter functions listed above can be used to modify the
/// fields afterwards.
///
/// \see DescriptorBaseXX.h
///
////////////////////////////////////////////////////////////////////////////////

class MUSPI_Pt2PtDirectPutDescriptor : public MUSPI_DescriptorBase
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_Pt2PtDirectPutDescriptor Default Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets point-to-point direct put packet type fields.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_Pt2PtDirectPutDescriptor() :
  MUSPI_DescriptorBase( MUHWI_PT2PT_DATA_PACKET_TYPE,
			MUHWI_PACKET_TYPE_PUT )
  {}


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_Pt2PtDirectPutDescriptor Full Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets point-to-point direct put packet type fields.
  /// Sets most commonly-set fields.
  /// Sets point-to-point fields.
  /// Sets direct put fields.
  ///
  /// \param[in]  info  Pointer to point-to-point direct put information.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_Pt2PtDirectPutDescriptor( MUSPI_Pt2PtDirectPutDescriptorInfo_t *info ) :
    MUSPI_DescriptorBase( MUHWI_PT2PT_DATA_PACKET_TYPE,
			  MUHWI_PACKET_TYPE_PUT )
  {
    this->setBaseFields( &info->Base );
    this->setPt2PtFields( &info->Pt2Pt );
    this->setDirectPutFields( &info->DirectPut );
  }


}; // End: class MUSPI_Pt2PtDirectPutDescriptor




#endif /* _MUSPI_PT2PT_DIRECT_PUT_DESCRIPTOR_XX_H_ */
