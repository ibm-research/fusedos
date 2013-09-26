/*								    */                                     
/* Licensed Materials - Property of IBM				    */
/*								    */
/* Blue Gene/Q							    */
/*								    */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved	    */
/*								    */
/* US Government Users Restricted Rights - Use, duplication or 	    */
/* disclosure restricted by GSA ADP Schedule Contract with IBM 	    */
/* Corporation.							    */
/*                                                                  */
/*                                                                  */
/* This software is available to you under either the GNU General   */ 
/* Public License (GPL) version 2 or the Eclipse Public License     */ 
/* (EPL) at your discretion.                                        */
/*                                                                  */


#ifndef _BGQ_FIRMWARE_INTERFACE_H_ // Prevent multiple inclusion
#define _BGQ_FIRMWARE_INTERFACE_H_


//
// The Firmware exports no interfaces directly usable/callable from user space.
//
#if defined(__KERNEL__) && !defined(__ASSEMBLY__)

#if ! defined(FW_AVOID_VARARGS)
#include <stdarg.h>
#endif

#include <firmware/include/personality.h>

#define BGQ_FIRMWARE_VERSION (0x00000001)

typedef struct Firmware_DomainDescriptor_t {
  fw_uint32_t  coreMask;       // LSB indicates core 0 is active in this domain
  fw_uint64_t  ddrOrigin;      // Base (physical) address of this domain's DDR region.
  fw_uint64_t  ddrEnd;         // End of domain's (physical) memory; -1 signifies end of actual DDR
  fw_uint64_t  entryPoint;     // Entry point of domain
  fw_uint64_t  configAddress;  // Address of this domain's configuration area.
  fw_uint32_t  configLength;   // Length of this domain's configuration area.
  char         options[512];   // Domain's command line options
} Firmware_DomainDescriptor_t;

typedef struct Firmware_TLBEntries_t {
    fw_uint64_t mas1;
    fw_uint64_t mas2;
    fw_uint64_t mas7_3;
    fw_uint64_t mas8;
    fw_uint64_t mmucr3;
} Firmware_TLBEntries_t;

#define FW_CORE_ENABLED_IN_DOMAIN(n,dd) ( ( (dd)->coreMask & (1<<(n)) ) != 0 )

typedef struct Firmware_Interface_t {

  fw_uint32_t Crc;      // Ensure accessible without corruption. Crc32n starting from .Version.
  fw_uint32_t Version;  // Compiled version number and observed version number must match!
  
  // Utility functions

  // ------------------------------------------------------------------------------------------
  //! @brief Writes the given string to the outbound JTAG mailbox, i.e. a print statement
  //!     delivered to the host control system.  
  //! @param[in] s A null-terminated string.
  //! @return Non-negative if the string was successfully written to the mailbox; -1 if an
  //!     error occurred.
  // ------------------------------------------------------------------------------------------
  int (*_delete_me_puts)( const char *s ); // DELETEME

#if defined (FW_AVOID_VARARGS)
  int (*_printf_placeholder)(); // do not use
#else
  int (*_delete_me_printf)( const char *format, ... ); //DELETEME
#endif

  // ------------------------------------------------------------------------------------------
  //! @brief Gracefully shut down the node.  This call will barrier until all hardware threads
  //!     on all cores have rendesvoused in exit.  The logical OR of all threads' exit codes
  //!     is delivered to the host control system via a TERMINATE message.  This call never
  //!     returns.
  //! @param[in] status The exit code for this thread.
  // ------------------------------------------------------------------------------------------
  void (*exit)( int status );

  // ------------------------------------------------------------------------------------------
  //! @brief Immediately shuts down the node.  This call will immediately deliver a TERMINATE
  //!     message to the host control system.  This call never returns.
  //! @param[in] status The exit code.
  // ------------------------------------------------------------------------------------------

  void (*terminate)( int status );

#if defined (FW_AVOID_VARARGS)
  int (*_vprintf_placeholder)(); // do not use
#else
  int (*_delete_me_vprintf)( const char *format, va_list args); //DELETEME
#endif

  // ------------------------------------------------------------------------------------------
  //! @brief Writes the null-terminated string to the outbound mailbox.
  //! param[in] s is a null-terminated ASCII string.
  //! return Non-negative value indicating the number of characters written.  A negative
  //!   return code indicates some form of error (no characters were written).  Specifically,
  //!   FW_TOO_BIG indicates that the message is too large to fit into the mailbox.  The
  //!   code FW_EAGAIN indicates that the message will not fit at this point in time.
  // ------------------------------------------------------------------------------------------

  int (*_delete_me_put)( const char *s ); // DELETEME
  

  // ------------------------------------------------------------------------------------------
  //! @brief Copies the current firmware personality into the given buffer.
  //! @param[in] personality A pointer to the target buffer.
  //! @param[in] size The size of the buffer (typically passed as sizeof(Personality_t)).
  //! @return Zero (0) if the personality was copied; non-zero if some error occurred (such as
  //!   a size mismatch).
  // ------------------------------------------------------------------------------------------

  int (*getPersonality)( Personality_t* personality, unsigned size );


  // ------------------------------------------------------------------------------------------
  //! @brief Writes a (binary) RAS event.  If there is not sufficient room in the mailbox to
  //!     hold the message, this call may block.
  //! @param[in] messageId 
  //! @param[in] numberOfDetails
  //! @param[in] details
  //! @return Zero if the RAS message was delivered to the JTAG mailbox; non-zero if it was 
  //!     not.  Note that delivery to the mailbox is not an acknowledgement from the control
  //!     system that the message has been received and logged.
  // ------------------------------------------------------------------------------------------
  int (*writeRASEvent)(fw_uint32_t messageId, fw_uint16_t numberOfDetails, fw_uint64_t details[]);


  // ------------------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------------------
  int (*writeRASString)(fw_uint32_t messageId, const char* message);


  //! @brief All of the following may go away at any point in the future.  The specifics will not
  //!   be documented further.  Use at your own risk.  This is your only warning.
  struct {
    Personality_t* personalityPtr;
    void (*sendReadyMessage)(void);
    int (*backdoorTest)(void*);
      void (*machineCheckHandler)(void);
    fw_uint64_t       space[7];
  } deprecated;


  //--------------------------------------------------------------------------------------------
  //! @brief Writes n characters to the outbox.
  //! @param[in] s points to the first character to be written.
  //! @param[in] length specifies the number of characters to be written.
  //! @return The actual number of characters written or a negative value
  //     if none of the data was written.  This could be because the 
  //     mailbox was full (FW_EAGAIN).
  //--------------------------------------------------------------------------------------------

  int (*putn)(const char* s, unsigned length);

  //--------------------------------------------------------------------------------------------
  //! @brief A call to determine if this node is an I/O node.
  //! @return 1 if the node is an I/O node; 0 if it is not.
  //--------------------------------------------------------------------------------------------
  
  int (*isIoNode)(void);

  //--------------------------------------------------------------------------------------------
  //! @brief TBD
  //! @return 0 if successful; non-zero if not successful.
  //--------------------------------------------------------------------------------------------

  int (*takeCPU)( unsigned coreNumber, unsigned threadMask, void (*entryPoint)(void*), void* arg );

  //--------------------------------------------------------------------------------------------
  //! @brief TBD
  //! @return 0 if successful; non-zero if not successful.
  //--------------------------------------------------------------------------------------------

  void (*returnCPU)( void );


  //--------------------------------------------------------------------------------------------
  //! @brief Performs end of job termination checking services.  Various hardware devices
  //!   will be scrubbed for anomalous conditions.
  //--------------------------------------------------------------------------------------------

  int (*termCheck)( void );


  //--------------------------------------------------------------------------------------------
  //! @brief "Scribbles" the given interrupt vector into a dedicated BeDRAM area.  The portion
  //!   of the vector associated with Machine Check Interrupts will not be copied, but instead
  //!   will be use by firmware to catch machine checks.  This service will update IVPR to
  //!   point to the vector that has been installed in BeDRAM.  This service should be called
  //!   from every node (in the domain).
  //! @param vec is a pointer to the interrupt vector being installed (typically 4K aligned)
  //! @param size is the size (in bytes) of the area being copied.  It cannot exceed 4K.
  //! @return FW_INVALID if bad argument(s) were passed.  FW_OK if the call completed.
  //--------------------------------------------------------------------------------------------

  int (*installInterruptVector)( void* vec, unsigned size );


  //--------------------------------------------------------------------------------------------
  //! @brief Fetches a copy of the descriptor associated with the caller's (implied) 
  //!   domain.
  //! @param buffer points to a writeable area into which the descriptor will be
  //!   copied.
  //! @return FW_OK if the descriptor was copied successfully.  
  //--------------------------------------------------------------------------------------------

  int (*getDomainDescriptor)( Firmware_DomainDescriptor_t* buffer );


  //--------------------------------------------------------------------------------------------
  //! @brief Sends a short, unformatted message from the (implicit) domain to another specified
  //!        domain.  Message delivery is one-way asynchronous.
  //! @param[in] targetDomain specifies the domain for which the message is intended.
  //! @param[in] msg is the message payload.
  //! @param[in] length is the message length.  It must not be longer than 128 bytes.
  //! @return FW_OK if the message was successfully placed in this domain's message box (this
  //!    does not imply that the message has been received).  FW_EAGAIN if the message box is
  //!    busy.  FW_INVALID if some illegal argument is passed (e.g. a length that exceeds the
  //!    message box size).
  //--------------------------------------------------------------------------------------------

  int (*sendMessageToDomain)( unsigned targetDomain, void* msg, unsigned length );

  //--------------------------------------------------------------------------------------------
  //! @brief Retrieves a message from the (implicit) domain's message box.
  //! @param[out] sourceDomain indicates the domain from which the message was sent.
  //! @param[out] message provides a buffer into which the message will be placed.  The buffer 
  //!   is assumed to be long enough to contain any message that might be in the message box
  //!   (which is no longer than 128 bytes).
  //! @param[out] length indicates the length of the message.
  //! @return FW_OK if a message was successfully retrieved.  FW_EAGAIN if there was no message
  //!   in the box or if the message box was too busy.  FW_INVALID if an illegal argument was
  //!   passed.
  //--------------------------------------------------------------------------------------------

  int (*retrieveMessage)( unsigned* sourceDomain, void* msg, unsigned* length );

  //--------------------------------------------------------------------------------------------
  //! @brief Issues a barrier request to the control system and blocks until a response is
  //!   received from the control system or until the request times out.  Only one barrier
  //!   request may be outstanding on a given node and firmware does use a semaphore to enforce
  //!   this.
  //! @param[in] timeoutInMicroseconds specifies the aproximate amount of time that the caller
  //!   is willing to wait for the barrier to complete.  A value of 0 indicates that the caller
  //!   is willing to wait indefinitly.
  //! @return FW_OK if the barrier completed normally (i.e. all nodes rendesvoused); all other
  //!   values indicate some form of error.  Specifically, FW_TIMEOUT indicates that firmware
  //!   either did not receive the response in time or that it could not even acquire the
  //!   lock to send the request in the first place.
  //--------------------------------------------------------------------------------------------

  int (*barrier)( fw_uint64_t timeoutInMicroseconds );

  //--------------------------------------------------------------------------------------------
  //! @brief Reads characters from the inbox.
  //! @param[in] s points to the first character of a buffer into which the input will be
  //!    placed.
  //! @param[in] length specifies the length of the buffer.
  //! @return The actual number of characters placed into the buffer or a negative value
  //!    if none of the data was written.  This could be because the mailbox was empty (FW_EAGAIN)
  //!    or because the message was too big to fit in the buffer (FW_TOO_BIG).
  //! @deprecated.  Use pollInbox instead.
  //--------------------------------------------------------------------------------------------

  int (*get)(const char* s, unsigned length);

  //-------------------------------------------------------------------------------------------
  //! @brief Sends the status of this nodes block over the mailbox.
  //! @param[in] status is the block's status and is one of JMB_BLOCKSTATE_* values listed 
  //!   in mailbox.h.
  //! @param[in] numArgs The number of additional arguments.
  //! @param[in] args Additional arguments (example: block ID for JMB_BLOCKSTATE_IO_LINK_CLOSED
  //!   messages).
  //-------------------------------------------------------------------------------------------

  int (*sendBlockStatus)( fw_uint16_t status, fw_uint16_t numArgs, fw_uint64_t args[] );

  //--------------------------------------------------------------------------------------------
  //! @brief Reads the next message from the inbox.
  //! @param[in] buffer is the address a buffer into which the message will be placed.
  //! @param[out] messageType is the type of message that was retrieved from the inbox.  Values
  //!    are one of JMB_CMND2CORE_NONE, JMB_CMD2CORE_STDIN or JMB_CMD2CORE_SYSTEM_REQUEST.
  //! @param[in] length specifies the length of the buffer.
  //! @return The actual number of characters placed into the buffer or a negative value
  //!    if none of the data was written.  This could be because the mailbox was empty (FW_EAGAIN)
  //!    or because the message was too big to fit in the buffer (FW_TOO_BIG).
  //--------------------------------------------------------------------------------------------

  int (*pollInbox)(void* buffer, fw_uint32_t* messageType, unsigned length);


    void (*flushRasBuffers)(void);

  //--------------------------------------------------------------------------------------------
  //! @brief Determines if any messages are pending in the outbox (i.e. not yet picked up by
  //!        the control system.
  //! @return 1 if there is one or more messages still in the outbox.  0 if there are no messages
  //!        in the outbox.
  //--------------------------------------------------------------------------------------------

    int (*pollOutbox)(void);


  //--------------------------------------------------------------------------------------------
  //! @brief Sets the pointer to the TLB table using by TLB parity recovery for the core
  //! @param[in] coreMMU contains a pointer to 512 TLB entries that would be restored during a TLB parity error
  //!            This pointer is preserved by firmware.  When changing the table, the operating system
  //!            should pass in NULL during the change, followed by a second call with the new/updated
  //!            table.  There should be 1 table per core, do not share a table between cores.
  //!            For operating systems that page fault, performance can be critical.  Populating the table
  //!            with only essential (e.g., TLBs covering the miss handler instruction and data) entries
  //!            with remaining entries marked invalid.  If the table is NULL and a parity error occurs,
  //!            firmware will abort with a hardware failure RAS.  
  //! @return 1 if there is one or more messages still in the outbox.  0 if there are no messages
  //!        in the outbox.
  //--------------------------------------------------------------------------------------------

    int (*setTLBTable)(Firmware_TLBEntries_t* coreMMU);
   
} Firmware_Interface_t;


#define FW_OK      (0)
#define FW_ERROR   (-1)
#define FW_EAGAIN  (-2)
#define FW_EINVAL  (-3)
#define FW_TIMEOUT (-4)

#define FW_INVALID   FW_EINVAL  //! @deprecated Use FW_EINVAL instead
#define FW_TOO_BIG   FW_EINVAL  //! @deprecated Use FW_EINVAL instead

#endif // __KERNEL__ and not __ASSEMBLY__


#define FW_GI_BARRIER_BOOT_CLASS_ROUTE 15

#endif // Add nothing below this line.

