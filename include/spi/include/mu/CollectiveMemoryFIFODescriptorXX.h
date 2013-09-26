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


#ifndef _MUSPI_COLLECTIVE_MEMORY_FIFO_DESCRIPTOR_XX_H_ /* Prevent multiple inclusion */
#define _MUSPI_COLLECTIVE_MEMORY_FIFO_DESCRIPTOR_XX_H_


#include <stdint.h>
#include <hwi/include/common/compiler_support.h>
#include <spi/include/mu/DescriptorBaseXX.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file CollectiveMemoryFIFODescriptorXX.h
///
/// \brief C++ Header File containing Message Unit SPI Collective Memory 
///        FIFO Descriptor Class Definitions
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_CollectiveMemoryFIFODescriptor Class
///
/// This class defines the Collective Memory FIFO Descriptor which will be
/// injected into a memory injection FIFO.  It is derived from the Base
/// descriptor.  It contains only the base descriptor member data.
/// The setter and getter functions in the base descriptor class operate on the 
/// descriptor's data members.
///
/// \verbatim
/// DescriptorWrapperXX.h      CollectiveMemoryFIFODescriptorXX.h &
/// ---------------------      DescriptorBaseXX.h
/// | DescriptorWrapper |      ----------------------------------
/// | - desc ----------------->| CollectiveMemoryFIFODescriptor |
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
/// memory FIFO packet type.  Beyond that, the following setter functions
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
/// - setMemoryFIFOFields
///   - setRecFIFOId
///   - setRecPutOffset
///   - setInterrupt
///   - setSoftwareBit
///   - setSoftwareBytes
///   - setSoftwareByte
///
/// The full constructor sets all of the fields, given input in an information
/// structure.  The setter functions listed above can be used to modify the
/// fields afterwards.
///
/// \see DescriptorBaseXX.h
///
////////////////////////////////////////////////////////////////////////////////

class MUSPI_CollectiveMemoryFIFODescriptor : public MUSPI_DescriptorBase
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_CollectiveMemoryFIFODescriptor Default Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets collective memory FIFO packet type fields.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_CollectiveMemoryFIFODescriptor() :
  MUSPI_DescriptorBase( MUHWI_COLLECTIVE_DATA_PACKET_TYPE,
			MUHWI_PACKET_TYPE_FIFO )
  {}


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_CollectiveMemoryFIFODescriptor Full Constructor
  ///
  /// Zeros out the descriptor.
  /// Sets collective memory FIFO packet type fields.
  /// Sets most commonly-set fields.
  /// Sets collective fields.
  /// Sets memory FIFO fields.
  ///
  /// \param[in]  info  Pointer to collective memory FIFO information.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_CollectiveMemoryFIFODescriptor( MUSPI_CollectiveMemoryFIFODescriptorInfo_t *info ) :
    MUSPI_DescriptorBase( MUHWI_COLLECTIVE_DATA_PACKET_TYPE,
			  MUHWI_PACKET_TYPE_FIFO )
  {
    this->setBaseFields( &info->Base );
    this->setCollectiveFields( &info->Collective );
    this->setMemoryFIFOFields( &info->MemFIFO );
  }


}; // End: class MUSPI_CollectiveMemoryFIFODescriptor




#endif /* _MUSPI_COLLECTIVE_MEMORY_FIFO_DESCRIPTOR_XX_H_ */
