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


#ifndef _MUSPI_DESCRIPTOR_WRAPPER_XX_H_ /* Prevent multiple inclusion */
#define _MUSPI_DESCRIPTOR_WRAPPER_XX_H_


#include <hwi/include/common/compiler_support.h>
#include <spi/include/mu/DescriptorBaseXX.h>


////////////////////////////////////////////////////////////////////////////////
///
/// \file DescriptorWrapperXX.h
///
/// \brief C++ MU SPI Descriptor Wrapper Class
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///
/// \brief MUSPI_Descriptor Wrapper Class
///
/// This class contains a reference to a MUSPI_DescriptorBase class, or a
/// derivitive of it, which will be injected into a MU injection FIFO.
///
/// This class also contains some additional data members that assist with
/// managing the descriptor.  It contains queuing pointers, a descriptor
/// sequence number used to determine when the descriptor is complete, and
/// callback information for notification of descriptor completion.
///
/// \verbatim
/// DescriptorWrapperXX.h      Pt2PtMemoryFIFODescriptorXX.h &
/// ---------------------      DescriptorBaseXX.h
/// | DescriptorWrapper |      -----------------------------
/// | - desc ----------------->| Pt2PtMemoryFIFODescriptor |
/// | - cb_done         |      | - DescriptorBase          |
/// | - sequenceNumber  |      -----------------------------
/// | - nextPtr/prevPtr |
/// | - FIFO number     |
/// ---------------------
/// \endverbatim
///
/// In place of Pt2PtMemoryFIFODescriptor, other descriptors can be substituted.
/// The DescriptorWrapper (left hand box) is not the real descriptor.  The real 
/// descriptor is the only thing inside the right hand box.
///
////////////////////////////////////////////////////////////////////////////////

class MUSPI_DescriptorWrapper
{
 public:

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief MUSPI_DescriptorWrapper constructor
  ///
  /// \param[in]  desc  Pointer to the descriptor that is managed by this class.
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_DescriptorWrapper( MUSPI_DescriptorBase *desc=NULL) :
  _desc(desc),
  _FIFO(-1),
  _sequenceNumber(0),
  _cb_done(NULL),
  _clientdata(NULL),
  _nextPtr(NULL),
  _prevPtr(NULL)
  { }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Default MUSPI_DescriptorWrapper Constructor
  ///
  /// Not to be directly used
  ///
  //////////////////////////////////////////////////////////////////////////////

  MUSPI_DescriptorWrapper(int dummy) {}

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Descriptor Pointer
  ///
  /// Set the pointer to the descriptor managed by this class
  ///
  /// \param[in]  desc  Pointer to the descriptor that is managed by this class.
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDescriptorPtr( MUSPI_DescriptorBase *desc )
  {
    _desc = desc;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Get Descriptor Pointer
  ///
  /// Get the pointer to the descriptor managed by this class
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline MUSPI_DescriptorBase * getDescriptorPtr()
  {
    return (_desc);
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Injection FIFO Number
  ///
  /// Set the global injection FIFO number that this descriptor is associated
  /// with.  This FIFO number is stored within this object, but not in the 
  /// descriptor.
  ///
  /// \param[in]  fnum  Injection FIFO number
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setFIFONum( int fnum )
  {
    this->_FIFO = fnum;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Get Injection FIFO Number
  ///
  /// Retrieve the injection FIFO number previously stored in this object.
  ///
  /// \returns  FIFO number
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline uint16_t getFIFONum() const
  {
    return( this->_FIFO );
  }


  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Sequence Number
  ///
  /// Set the sequence number for this descriptor.  This is stored within this
  /// object, but not in the descriptor itself.
  ///
  /// \param[in]  sequenceNumber  The sequence number.
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setSequenceNumber( uint64_t sequenceNumber )
  {
    this->_sequenceNumber = sequenceNumber;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Get Sequence Number
  ///
  /// Get the sequence number of this descriptor in the FIFO.
  ///
  /// \returns  Sequence Number
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline uint64_t getSequenceNumber()
  {
    return( this->_sequenceNumber );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set the descriptor callback information
  ///
  /// This information is stored within this object, but not in the descriptor
  /// itself.
  ///
  /// \param[in]  cb_done     Callback function
  /// \param[in]  clientdata  Pointer to opaque data for the callback function
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setDoneCallback ( void (*cb_done)(void *),
				void * clientdata )
  {
    this->_cb_done    = cb_done;
    this->_clientdata = clientdata;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Return Whether a Callback is Desired When this Descriptor has
  ///        been Processed by the Message Unit.
  ///
  /// \retval  0  No callback is desired
  /// \retval  1  A callback is desired
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline uint32_t isCallbackDesired ( )
  {
    return ( this->_cb_done != NULL );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Perform the Callback
  ///
  /// Call the cb_done function previously set for this descriptor.
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void performCallback ( )
  {
    this->_cb_done ( this->_clientdata );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Next Pointer
  ///
  /// Set the "next" pointer in this descriptor to point to the next descriptor
  /// in the list.  This is stored in this object, but not in the descriptor
  /// itself.
  ///
  /// \param[in]  nextPtr  Next pointer to be set
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setNextPtr ( MUSPI_DescriptorWrapper *nextPtr )
  {
    this->_nextPtr = nextPtr;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Get Next Pointer
  ///
  /// Get the "next" pointer in this descriptor
  ///
  /// \retval  nextPtr  Pointer to next descriptor in the list
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline MUSPI_DescriptorWrapper * getNextPtr ( )
  {
    return ( this->_nextPtr );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Set Previous Pointer
  ///
  /// Set the "previous" pointer in this descriptor to point to the previous
  /// descriptor in the list.  This is stored in this object, but not in the 
  /// descriptor itself.
  ///
  /// \param[in]  prevPtr  Previous pointer to be set
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void setPrevPtr ( MUSPI_DescriptorWrapper *prevPtr )
  {
    this->_prevPtr = prevPtr;
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Get Previous Pointer
  ///
  /// Get the "previous" pointer in this descriptor
  ///
  /// \retval  prevPtr  Pointer to previous descriptor in the list
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline MUSPI_DescriptorWrapper * getPrevPtr ( )
  {
    return ( this->_prevPtr );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Clone this Descriptor Wrapper
  ///
  /// This is a clone, copying "this" object to a specified DescriptorWrapper
  /// object.
  ///
  /// \param[in]  obj  Reference to the target DescriptorWrapper object to copy
  ///                  to.
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void clone( MUSPI_DescriptorWrapper &obj )
  {
    memcpy( (void*)&obj, (void*)this, sizeof(MUSPI_DescriptorWrapper) );
  }

  //////////////////////////////////////////////////////////////////////////////
  ///
  /// \brief Dump Descriptor
  ///
  //////////////////////////////////////////////////////////////////////////////

  inline void dump()
  {
    _desc->dump();
  }

 protected:

  ///
  /// \brief _desc       Pointer to descriptor managed by this class
  ///
  MUSPI_DescriptorBase * _desc;

  ///
  /// \brief  _FIFO  The fifo number (global) where the descriptor will be 
  ///                injected.
  ///
  /// \see setFIFONum()
  /// \see getFIFONum()
  ///
  uint16_t _FIFO;

  ///
  /// \brief _sequenceNumber  The sequence number assigned to this descriptor
  ///                      when it is injected.  This sequence number grows
  ///                      sequentially (won't wrap in the lifetime of a job).
  ///                      It is recalled and used later to determine if the
  ///                      descriptor has been processed by the MU (it has left
  ///                      the FIFO).
  ///
  /// \see setSequenceNumber()
  /// \see getSequenceNumber()
  ///
  uint64_t _sequenceNumber;

  ///
  /// \brief _cb_done  Callback when this descriptor has completed.
  ///
  void (*_cb_done)(void *);

  ///
  /// \brief  _clientdata  A pointer to opaque data to be passed to _cb_done.
  ///
  void *_clientdata;

  ///
  /// \brief  nextPtr  Pointer to next descriptor on the queue
  ///
  MUSPI_DescriptorWrapper *_nextPtr;

  ///
  /// \brief  prevPtr  Pointer to previous descriptor on the queue
  ///
  MUSPI_DescriptorWrapper *_prevPtr;


 private:

}; // End: class MUSPI_DescriptorWrapper



#endif // _MUSPI_DESCRIPTOR_WRAPPER_XX_H_
