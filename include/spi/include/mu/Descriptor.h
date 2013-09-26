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

#ifndef	_MUSPI_DESCRIPTOR_H_ /* Prevent multiple inclusion */
#define	_MUSPI_DESCRIPTOR_H_


/**
 * \file Descriptor.h
 *
 * \brief C Header File containing Message Unit SPI Descriptor Definitions
 *
 * Functions that build complete descriptors are prototyped, and inline setter
 * functions are provided to set individual fields.
 *
 * The following structures contain information that is used to initialize
 * the different types of descriptors.  There is information that is
 * common to all descriptors, point-to-point information, collective
 * information, memory FIFO information, direct put information, and
 * remote get information.  These structures are combined in various ways
 * and passed to functions and constructors that create descriptors.
 *
 * - Pt2PtMemoryFIFODescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - Pt2PtDescriptorInfoFields
 *   - MemoryFIFODescriptorInfoFields
 *
 * - Pt2PtDirectPutDescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - Pt2PtDescriptorInfoFields
 *   - DirectPutDescriptorInfoFields
 *
 * - Pt2PtRemoteGetDescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - Pt2PtDescriptorInfoFields
 *   - RemoteGetDescriptorInfoFields
 *
 * - CollectiveMemoryFIFODescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - CollectiveDescriptorInfoFields
 *   - MemoryFIFODescriptorInfoFields
 *
 * - CollectiveDirectPutDescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - CollectiveDescriptorInfoFields
 *   - DirectPutDescriptorInfoFields
 *
 * - CollectiveRemoteGetDescriptorInfo
 *   - BaseDescriptorInfoFields
 *   - CollectiveDescriptorInfoFields
 *   - RemoteGetDescriptorInfoFields
 */


#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Descriptor.h>
#include <hwi/include/bqc/MU_PacketCommon.h>
#include <hwi/include/bqc/MU_PacketHeader.h>
#include <spi/include/mu/Util.h>


__BEGIN_DECLS


/**
 * \brief Base Descriptor Information Fields
 *
 * Used to pass base information common to all types of descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_BaseDescriptorInfoFields
{
  uint32_t         Pre_Fetch_Only; /**< Specifies whether the message data
				        should be pre-fetched only, or whether
					it should be sent.  Specify one of:
					- MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_YES
					- MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO
				   */

  uint64_t         Payload_Address;/**< Payload physical address (least
				        significant 37 bits used).
				   */

  uint64_t         Message_Length; /**< Length of the payload, in bytes
				        (least 37 significant bits).
					May be zero.
				   */

  uint64_t         Torus_FIFO_Map; /**< Torus injection FIFO map.  A 1 in bit i
				        indicates that the message can be
					injected into Torus injection FIFO i.
					0 <= i <= 15.
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_PRIORITY
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CUSER
					- MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CSYSTEM
				   */

  MUHWI_Destination_t Dest;        /**< Coordinates of destination.
                                        For collectives, the destination is
				        only used for reduce. 
				   */

} MUSPI_BaseDescriptorInfoFields_t;


/**
 * \brief Point-to-Point Descriptor Information Fields
 *
 * Used to pass information common to point-to-point descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_Pt2PtDescriptorInfoFields
{
  uint8_t          Hints_ABCD; /**< Hint bits for A, B, C, and D directions.
				    OR together one of the following for each
				    direction, where "X" is A, B, C, or D:
				    - MUHWI_PACKET_HINT_X_NONE
				    - MUHWI_PACKET_HINT_XP
				    - MUHWI_PACKET_HINT_XM
			       */

  uint8_t          Misc1;      /**< Miscellaneous constants.  OR together the
                                    following constants:
				    - Hint bits for E direction.  One of:
				      - MUHWI_PACKET_HINT_E_NONE
				      - MUHWI_PACKET_HINT_EP
				      - MUHWI_PACKET_HINT_EM
				    - Route to I/O Node.  One of:
				      - MUHWI_PACKET_ROUTE_TO_IO_NODE
				      - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE
				    - Return from I/O Node.  One of:
				      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6
				      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7
				      - MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT
				    - Routing.  One of:
				      - MUHWI_PACKET_USE_DYNAMIC_ROUTING
				      - MUHWI_PACKET_USE_DETERMINISTIC_ROUTING
				    - Deposit bit.  One of:
				      - MUHWI_PACKET_DEPOSIT
				      - MUHWI_PACKET_DO_NOT_DEPOSIT
			       */

  uint8_t          Misc2; /**< Miscellaneous constants.  OR together the following
			       constants:
			       - Virtual Channel.  One of:
			         - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
				 - MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
			       - Zone Routing Id.  One of
			         - MUHWI_PACKET_ZONE_ROUTING_0
			         - MUHWI_PACKET_ZONE_ROUTING_1
			         - MUHWI_PACKET_ZONE_ROUTING_2
			         - MUHWI_PACKET_ZONE_ROUTING_3
				 Used only with dynamic routing.  This Id
				 refers to one of 4 sets of DCRs containing
				 zone masks.  For selecting a dynamic direction,
				 the packet hint bits are ANDed with the
				 appropriate zone mask to restrict the set of
				 dynamic directions that may be chosen.  For a
			         given set of DCRs identified by the zone
			         routine Id, the first mask is used until
			         the destination in the first dimension is
			         reached, then the next mask is used for
			         the next dimension, etc.  
			       - Stay on Bubble.  Used only with dynamic
			         routing.  One of:
			         - MUHWI_PACKET_STAY_ON_BUBBLE
				 - MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE
			  */

  uint16_t         Skip;  /**< Number of 4-byte words to skip for injection
			       checksum (0 to 136).
			  */

} MUSPI_Pt2PtDescriptorInfoFields_t;


/**
 * \brief Collective Descriptor Information Fields
 *
 * Used to pass information common to collective descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_CollectiveDescriptorInfoFields
{
  uint8_t          Op_Code; /**< Collective OP code.  One of:
			         - MUHWI_COLLECTIVE_OP_CODE_AND
				 - MUHWI_COLLECTIVE_OP_CODE_OR
				 - MUHWI_COLLECTIVE_OP_CODE_XOR
				 - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD
				 - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN
				 - MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MAX
				 - MUHWI_COLLECTIVE_OP_CODE_SIGNED_ADD
				 - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MIN
				 - MUHWI_COLLECTIVE_OP_CODE_SIGNED_MAX
				 - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_ADD
				 - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MIN
				 - MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MAX.
				 Op_Code is ignored when the collective
				 type is MUHWI_COLLECTIVE_TYPE_BROADCAST.
			    */

  uint16_t         Word_Length; /**< Number of bytes in a "collective word".
                                    The Op_Code will be performed on elements of
                                    this length.  Message_Length must be a
				    multiple of Word_Length.  Word_Length must
				    be one of:  4, 8, 16, 32, 64, 128, 256, 512.
				*/

  uint8_t          Class_Route; /**< The collective class route (0 through 15).
				 */

  uint8_t          Misc; /**< Miscellaneous constants.  OR together the following
			      constants:
			      - Virtual Channel.  One of:
			        - MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC
			        - MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC
			        - MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY
				- MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM
				- MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COLLECTIVE
				- MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD
				- MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE
                              - Collective Type.  One of:
			        - MUHWI_COLLECTIVE_TYPE_BROADCAST
				- MUHWI_COLLECTIVE_TYPE_REDUCE
				- MUHWI_COLLECTIVE_TYPE_ALLREDUCE
			  */

  uint16_t         Skip;  /**< Number of 4-byte words to skip for injection
			       checksum (0 to 136).
			  */

} MUSPI_CollectiveDescriptorInfoFields_t;


/**
 * \brief Memory FIFO Descriptor Information Fields
 *
 * Used to pass information common to memory FIFO descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_MemoryFIFODescriptorInfoFields
{
  uint16_t         Rec_FIFO_Id; /**< Reception FIFO Id to receive the packet
				     (0 to 271).
				*/
  uint64_t         Rec_Put_Offset; /*<! Initial value for the reception side put
			       offset in the first packet.  Hardware updates this
			       in each packet.  For a memory FIFO packet, this
			       serves as a packet counter.  For a single packet
			       message, this is unused/unchanged by the hardware,
			       so it may be set to anything by software.
			       */

  uint32_t         Interrupt;      /**< Specifies whether an interrupt should
				        occur when the last packet of this
					message arrives in the destination's
					reception FIFO.  Specify one of:
					- MUHWI_DESCRIPTOR_INTERRUPT_ON_PACKET_ARRIVAL
					- MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL
				   */

  uint8_t          SoftwareBit;/**< A bit available to software that will be
				    passed unchanged to the destination in
				    the packet header.  This corresponds to 
				    "Unused1" in the Memory_FIFO portion of
				    the Message Unit Header of the packet.
				    Refer to PacketInlines.h for a function
				    to extract this field from the packet
				    header.  Set this to 0 or 1.
			       */

  uint8_t          SoftwareBytes[14];/**< Bytes available to software that will
				          be passed unchanged to the destination
					  in the packet header.  This
					  corresponds to "Unused2" in the 
					  Memory_FIFO portion of the Message
					  Unit Header of the packet.
					  Refer to PacketInlines.h for a
					  function to extract this field from
					  the packet header.
			       */
} MUSPI_MemoryFIFODescriptorInfoFields_t;


/**
 * \brief Direct Put Descriptor Information Fields
 *
 * Used to pass information common to direct put descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_DirectPutDescriptorInfoFields
{
  uint16_t         Rec_Payload_Base_Address_Id; /**< Base address id associated
			       with the payload.  This is an index into the base
			       address table on the reception side of an entry
			       containing a base physical address that is summed
			       with the put offset in the received packet header
			       to determine the physical address to store the
			       packet payload.
			       */

  uint64_t         Rec_Payload_Offset; /**< Offset in bytes of the reception
					    payload from the base address
					    identified by 
					    Rec_Payload_Base_Address_Id.
				       */

  uint16_t         Rec_Counter_Base_Address_Id; /**< Base address id associated
			       with the reception counter.  This is an index
			       into the base address table on the reception side
			       of an entry containing the base physical address
			       that is summed with the rec_counter_offset to
			       determine the physical address of the reception
			       counter to be updated as packets are received.
			       */

  uint64_t         Rec_Counter_Offset; /**< Offset in bytes of the reception
					    counter from base address identified
					    by Rec_Counter_Base_Address_Id.
				       */

  uint8_t          Pacing; /**< Specifies whether this direct put is a response
			        to a paced remote get.  Specify one of:
				- MUHWI_PACKET_DIRECT_PUT_IS_PACED
				- MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED
			   */
} MUSPI_DirectPutDescriptorInfoFields_t;


/**
 * \brief Remote Get Descriptor Information Fields
 *
 * Used to pass information common to remote get descriptors to
 * functions that build descriptors.
 *
 */
typedef struct MUSPI_RemoteGetDescriptorInfoFields
{
  uint8_t          Type;             /**< Type of remote get.  One of:
				          - MUHWI_PACKET_TYPE_GET
					  - MUHWI_PACKET_TYPE_PACED_GET
				     */

  uint16_t         Rget_Inj_FIFO_Id; /**< Remote get injection FIFO Id on the
				      destination node where the payload
				      descriptors will be injected (0 to 543).
				 */

} MUSPI_RemoteGetDescriptorInfoFields_t;


/**
 * \brief Point-to-Point Memory FIFO Descriptor Information Structure
 *
 * Used to pass parameters specific to Point-to-Point Memory FIFO descriptors to
 * functions that build memory FIFO descriptors.
 * 
 */
typedef struct MUSPI_Pt2PtMemoryFIFODescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base; /**< Base (common) descriptor info   */

  MUSPI_Pt2PtDescriptorInfoFields_t Pt2Pt; /**< Point-to-point descriptor info*/

  MUSPI_MemoryFIFODescriptorInfoFields_t MemFIFO; /**< Memory FIFO
						       descriptor info   */

} MUSPI_Pt2PtMemoryFIFODescriptorInfo_t;


/**
 * \brief Point-to-Point Direct Put Descriptor Information Structure
 *
 * Used to pass parameters specific to Point-to-Point Direct-Put descriptors to
 * functions that build direct-put descriptors.
 * 
 */
typedef struct MUSPI_Pt2PtDirectPutDescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base;  /**< Base (common) descriptor info  */

  MUSPI_Pt2PtDescriptorInfoFields_t Pt2Pt;/**< Point-to-point descriptor info */

  MUSPI_DirectPutDescriptorInfoFields_t DirectPut;/**< Direct put
						       descriptor info   */

} MUSPI_Pt2PtDirectPutDescriptorInfo_t;


/**
 * \brief Point-to-Point Remote Get Descriptor Information Structure
 *
 * Used to pass parameters specific to Point-to-Point Remote Get descriptors to
 * functions that build remote get descriptors.
 * 
 */
typedef struct MUSPI_Pt2PtRemoteGetDescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base;  /**< Base (common) descriptor info  */

  MUSPI_Pt2PtDescriptorInfoFields_t Pt2Pt;/**< Point-to-point descriptor info */

  MUSPI_RemoteGetDescriptorInfoFields_t RemoteGet;/**< Remote get
						       descriptor info   */

} MUSPI_Pt2PtRemoteGetDescriptorInfo_t;


/**
 * \brief Collective Memory FIFO Descriptor Information Structure
 *
 * Used to pass parameters specific to Collective Memory FIFO descriptors to 
 * functions that build memory FIFO descriptors.
 * 
 */
typedef struct MUSPI_CollectiveMemoryFIFODescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base; /**< Base (common) descriptor info   */

  MUSPI_CollectiveDescriptorInfoFields_t Collective; /**< Point-to-point descriptor info*/

  MUSPI_MemoryFIFODescriptorInfoFields_t MemFIFO; /**< Memory FIFO
						       descriptor info   */

} MUSPI_CollectiveMemoryFIFODescriptorInfo_t;


/**
 * \brief Collective Direct Put Descriptor Information Structure
 *
 * Used to pass parameters specific to Collective Direct-Put descriptors to
 * functions that build direct-put descriptors.
 * 
 */
typedef struct MUSPI_CollectiveDirectPutDescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base;  /**< Base (common) descriptor info  */

  MUSPI_CollectiveDescriptorInfoFields_t Collective;/**< Point-to-point descriptor info */

  MUSPI_DirectPutDescriptorInfoFields_t DirectPut;/**< Direct put
						       descriptor info   */

} MUSPI_CollectiveDirectPutDescriptorInfo_t;


/**
 * \brief Collective Remote Get Descriptor Information Structure
 *
 * Used to pass parameters specific to Collective Remote Get descriptors to 
 * functions that build remote get descriptors.
 * 
 */
typedef struct MUSPI_CollectiveRemoteGetDescriptorInfo
{
  MUSPI_BaseDescriptorInfoFields_t Base;  /**< Base (common) descriptor info  */

  MUSPI_CollectiveDescriptorInfoFields_t Collective;/**< Point-to-point descriptor info */

  MUSPI_RemoteGetDescriptorInfoFields_t RemoteGet;/**< Remote get
						       descriptor info   */

} MUSPI_CollectiveRemoteGetDescriptorInfo_t;


/**
 * \brief Create a Descriptor For a Point-to-Point Memory FIFO Message
 *
 * A memory FIFO message is one that is sent to another node and whose packets
 * are placed into a designated in-memory reception FIFO (as specified in the
 * packet header).  The reception FIFO must be polled by the core(s) to process
 * the packets.
 *
 * This function creates the memory FIFO descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreatePt2PtMemoryFIFODescriptor( MUHWI_Descriptor_t                    *desc,
					   MUSPI_Pt2PtMemoryFIFODescriptorInfo_t *info 
					 );


/**
 * \brief Create a Descriptor For a Point-to-Point Direct Put Message
 *
 * A direct put message is one that is sent to another node and its data
 * is directly put into memory by the MU on the destination node...it does
 * not go into a reception fifo.
 *
 * This function creates the direct-put descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreatePt2PtDirectPutDescriptor( MUHWI_Descriptor_t                   *desc,
					  MUSPI_Pt2PtDirectPutDescriptorInfo_t *info 
					);


/**
 * \brief Create a Descriptor For a Point-to-Point Remote Get Message
 *
 * A remote get message is one that is sent to another node and its data
 * (containing one or more descriptors) is injected into a remote get injection
 * FIFO by the MU.  It is not received by the cores.
 *
 * This function creates the remote get descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreatePt2PtRemoteGetDescriptor( MUHWI_Descriptor_t                   *desc,
					  MUSPI_Pt2PtRemoteGetDescriptorInfo_t *info 
					);


/**
 * \brief Create a Descriptor For a Collective Memory FIFO Message
 *
 * A memory FIFO message is one that is sent to another node and whose packets
 * are placed into a designated in-memory reception FIFO (as specified in the
 * packet header).  The reception FIFO must be polled by the core(s) to process
 * the packets.
 *
 * This function creates the memory FIFO descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreateCollectiveMemoryFIFODescriptor( 
				  MUHWI_Descriptor_t                         *desc,
				  MUSPI_CollectiveMemoryFIFODescriptorInfo_t *info );


/**
 * \brief Create a Descriptor For a Collective Direct Put Message
 *
 * A direct put message is one that is sent to another node and its data
 * is directly put into memory by the MU on the destination node...it does
 * not go into a reception fifo.
 *
 * This function creates the direct-put descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreateCollectiveDirectPutDescriptor( 
				  MUHWI_Descriptor_t                        *desc,
				  MUSPI_CollectiveDirectPutDescriptorInfo_t *info );


/**
 * \brief Create a Descriptor For a Collective Remote Get Message
 *
 * A remote get message is one that is sent to another node and its data
 * (containing one or more descriptors) is injected into a remote get injection
 * FIFO by the MU.  It is not received by the cores.
 *
 * This function creates the remote get descriptor that can be injected into
 * a memory injection FIFO.
 *
 * \param[in,out]  desc             Pointer to the storage where the descriptor
 *                                  will be created.
 * \param[in]      info             Pointer to a structure containing parameters
 *                                  needed to build the descriptor.
 *
 * \retval  0  Success
 */

int MUSPI_CreateCollectiveRemoteGetDescriptor( 
				  MUHWI_Descriptor_t                        *desc,
				  MUSPI_CollectiveRemoteGetDescriptorInfo_t *info );


/**
 * \brief Dump a Descriptor
 *
 * Dump the contents of a descriptor to stdout.
 *
 * \param[in]  identifier  Pointer to a string to be printed.
 * \param[in]  desc        Pointer to the descriptor.
 *
 * Refer to spi/src/mu/Descriptors.cc for implementation.
 */

void MUSPI_DescriptorDumpHex ( char               *identifier,
			       MUHWI_Descriptor_t *desc );


__END_DECLS


#endif /* _MUSPI_DESCRIPTOR_H_  */
