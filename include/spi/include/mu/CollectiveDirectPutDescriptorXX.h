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


#ifndef _MUSPI_COLLECTIVE_DIRECT_PUT_DESCRIPTOR_XX_H_ /* Prevent multiple inclusion */
#define _MUSPI_COLLECTIVE_DIRECT_PUT_DESCRIPTOR_XX_H_


#include <stdint.h>
#include <hwi/include/common/compiler_support.h>
#include <spi/include/mu/DescriptorBaseXX.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file CollectiveDirectPutDescriptorXX.h
///
/// \brief C++ Header File containing Message Unit SPI Collective Direct 
///        Put Descriptor Class Definitions
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_CollectiveDirectPutDescriptor Class
///
/// This class defines the Collective Direct Put Descriptor which will be
/// injected into a memory injection FIFO.  It is derived from the Base
/// descriptor.  It contains only the base descriptor member data.
/// The setter and getter functions in the base descriptor class operate on the 
/// descriptor's data members.
///
/// \verbatim
/// DescriptorWrapperXX.h      CollectiveDirectPutDescriptorXX.h &
/// ---------------------      DescriptorBaseXX.h
/// | DescriptorWrapper |      ----------------------------------
/// | - desc ----------------->| CollectiveDirectPutDescriptor  |
/// | - cb_done         |      | - DescriptorBase               |
/// | - sequenceNumber  |      ----------------------------------
/// | - nextPtr/prevPtr |
/// | - FIFO number     |
/// ---------------------
/// \endverbatim
///
/// The DescriptorWrapper (left hand box) is not the real descriptor.  The real 
/// descriptor is the only thing inside the right hand box.
///
/// The default constructor zeros out the descriptor and sets the collective
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
/// - setCollectiveFields
///   - setOpCode
///   - setWordLength
///   - setClassRoute
///   - setCollectiveMisc
///   - setCollectiveVirtualChannel
///   - setCollectiveType
///   - setCollectiveSkip
///
/// - setDirectPutFields
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

class MUSPI_CollectiveDirectPutDescriptor : public MUSPI_DescriptorBase
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_CollectiveDirectPutDescriptor Default Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets collective direct put packet type fields.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_CollectiveDirectPutDescriptor() :
  MUSPI_DescriptorBase( MUHWI_COLLECTIVE_DATA_PACKET_TYPE,
			MUHWI_PACKET_TYPE_PUT )
  {}


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_CollectiveDirectPutDescriptor Full Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets collective direct put packet type fields.
  /// Sets most commonly-set fields.
  /// Sets collective fields.
  /// Sets direct put fields.
  ///
  /// \param[in]  info  Pointer to collective direct put information.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_CollectiveDirectPutDescriptor( MUSPI_CollectiveDirectPutDescriptorInfo_t *info ) :
    MUSPI_DescriptorBase( MUHWI_COLLECTIVE_DATA_PACKET_TYPE,
			  MUHWI_PACKET_TYPE_PUT )
  {
    this->setBaseFields( &info->Base );
    this->setCollectiveFields( &info->Collective );
    this->setDirectPutFields( &info->DirectPut );
  }


}; // End: class MUSPI_CollectiveDirectPutDescriptor




#endif /* _MUSPI_COLLECTIVE_DIRECT_PUT_DESCRIPTOR_XX_H_ */
