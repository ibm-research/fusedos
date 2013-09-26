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

#ifndef	_MUHWI_PT2PT_NETWORK_HEADER_H_ /* Prevent multiple inclusion */
#define	_MUHWI_PT2PT_NETWORK_HEADER_H_


/**
 * \file MU_Pt2PtNetworkHeader.h
 *
 * \brief C Header File containing Message Unit Point-to-Point
 *        Network Packet Header Definitions
 *
 * \verbatim
 * MU_Descriptor.h
 * ---------------------------------------
 * | Descriptor                          |
 * | - Prefetch                          |
 * | - Interrupt                         |
 * | - Pa_Payload                        |
 * | - Message_Length                    |
 * | - Torus FIFO Map                    |
 * | - Packet Header                     | MU_PacketHeader.h
 * |   - One of:                         |
 * |     - Point-to-Point Network Header | MU_Pt2PtNetworkHeader.h <<<<<<<
 * |     - Collective Network Header     | MU_CollectiveNetworkHeader.h
 * |   - Message Unit Header             | MU_MessageUnitHeader.h
 * |--------------------------------------
 * \endverbatim
 *
 */


#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_PacketCommon.h>


__BEGIN_DECLS


/**
 * \brief Point-to-Point Data Packet Type
 */
#define MUHWI_PT2PT_DATA_PACKET_TYPE 0x55


/**
 * \brief Hint Bit: No hints for A direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_A_NONE  (0x00)


/**
 * \brief Hint Bit: Packet wants to travel in the A minus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_AM  (0x80)


/**
 * \brief Hint Bit: Packet wants to travel in the A plus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_AP  (0x40)


/**
 * \brief Hint Bit: No hints for B direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_B_NONE  (0x00)


/**
 * \brief Hint Bit: Packet wants to travel in the B minus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_BM  (0x20)


/**
 * \brief Hint Bit: Packet wants to travel in the B plus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_BP  (0x10)


/**
 * \brief Hint Bit: No hints for C direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_C_NONE  (0x00)


/**
 * \brief Hint Bit: Packet wants to travel in the C minus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_CM  (0x08)


/**
 * \brief Hint Bit: Packet wants to travel in the C plus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_CP  (0x04)


/**
 * \brief Hint Bit: No hints for D direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_D_NONE  (0x00)


/**
 * \brief Hint Bit: Packet wants to travel in the D minus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_DM  (0x02)


/**
 * \brief Hint Bit: Packet wants to travel in the D plus direction.
 *
 * Meant to be OR'd directly into the Hints field of the point-to-point
 * Network Header.
 */
#define MUHWI_PACKET_HINT_DP  (0x01)


/**
 * \brief Byte2: No hints for E direction.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_HINT_E_NONE  (0x00)


/**
 * \brief Byte2: Hint_E_Minus - Packet wants to travel in the E plus direction.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_HINT_EM  (0x80)


/**
 * \brief Byte2: Hint_E_Plus - Packet wants to travel in the E plus direction.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_HINT_EP  (0x40)


/**
 * \brief Byte2: Route_To_IO_Node - Route to I/O Node.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_ROUTE_TO_IO_NODE  (0x20)


/**
 * \brief Byte2: Route_To_IO_Node - Do not route to I/O Node.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE  (0x00)


/**
 * \brief Byte2: Return_From_IO_Node - Use Torus Port 6.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_6  (0x08)


/**
 * \brief Byte2: Return_From_IO_Node - Use Torus Port 7.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_TORUS_PORT_7  (0x10)


/**
 * \brief Byte2: Return_From_IO_Node - Use I/O Port 10.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_RETURN_FROM_IO_NODE_USING_IO_PORT  (0x18)


/**
 * \brief Byte2: Use Dynamic Routing.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_USE_DYNAMIC_ROUTING  (0x04)


/**
 * \brief Byte2: Use Deterministic Routing.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_USE_DETERMINISTIC_ROUTING  (0x00)


/**
 * \brief Byte2: Deposit.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_DEPOSIT  (0x02)


/**
 * \brief Byte2: Do Not Deposit.
 *
 * Meant to be OR'd directly into Byte2 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_DO_NOT_DEPOSIT  (0x00)


/**
 * \brief Byte3: Zone Routine ID 0
 *
 * Meant to be OR'd directly into Byte3 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_ZONE_ROUTING_0  (0x00)


/**
 * \brief Byte3: Zone Routing ID 1
 *
 * Meant to be OR'd directly into Byte3 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_ZONE_ROUTING_1  (0x08)


/**
 * \brief Byte3: Zone Routing ID 2
 *
 * Meant to be OR'd directly into Byte3 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_ZONE_ROUTING_2  (0x10)


/**
 * \brief Byte3: Zone Routing ID 3
 *
 * Meant to be OR'd directly into Byte3 of the point-to-point Network Header.
 */
#define MUHWI_PACKET_ZONE_ROUTING_3  (0x18)


/**
 * \brief Point-to-Point Network Header
 *
 * This is the point-to-point portion of the packet header.  It occupies bytes
 * 0 through 11 of the packet header.
 *
 * \see MU_PacketCommon.h
 * \see MU_MessageUnitHeader.h
 * \see MU_PacketHeader.h
 *
 */
typedef struct MUHWI_Pt2PtNetworkHeader
{
  uint8_t Data_Packet_Type; /**< Byte 0.  Packet type: 
			         MUHWI_PT2PT_DATA_PACKET_TYPE.
			    */
      
  uint8_t Hints;            /**< Byte 1: Hint bits for network routing.
			         Each bit corresponds to A+, A-, B+, B-,
				 C+, C-, D+, D-.  If a bit is set, it 
				 indicates that the packet wants to travel
				 along the corresponding direction.  If
				 all bits are zero, the hardware
				 calculates the hint bits.  Both A+ and A-
				 cannot be set at the same time...same
				 with B, C, and D.  Refer to 
				 MUHWI_PACKET_HINT_xx definitions.  Hints for
				 the E dimension are in Byte2.
				 Set by software.
			    */

  union
  {
    uint8_t Byte2;                  /**< Byte 2.  All bits set by software.
				     */

    struct
    {
      uint8_t Hint_E_plus        : 1; /**< Hint bit for E+ direction. */
      
      uint8_t Hint_E_minus       : 1; /**< Hint bit for E- direction. */

      uint8_t Route_To_IO_Node   : 1; /**< 0 = Route to compute node.
					       - MUHWI_PACKET_DO_NOT_ROUTE_TO_IO_NODE.
					   1 = Route to I/O node.  The
					       Destination is the compute node
					       that is connected to the I/O
					       node.  That compute node does not
					       receive the packet, but instead
					       routes the packet on to the I/O
					       node.
					       - MUHWI_PACKET_ROUTE_TO_IO_NODE.
				      */

      uint8_t Return_From_IO_Node: 2; /**< Return from I/O node routing:
					   00 = Not returning from IO node.
					   01 = Use torus port 6.
					   10 = Use torus port 7.
					   11 = Use I/O port 10.
					   When returning from I/O node, the
					   packet first travels along the
					   specified port to the compute node
					   connected to the I/O node, and then
					   it is deterministically routed to
					   the compute node Destination.
				      */

      uint8_t Dynamic            : 1; /**< Routing indicator:
					   0 = Use deterministic routing.
					   1 = Use dynamic routing.  Should also
					       specify the dynamic virtual
					       channel.
				      */

      uint8_t Deposit            : 1; /**< Deposit indicator:
					   0 = Deposit only at destination.
					   1 = Deposit at every node along
					       the line.
				      */

      uint8_t Interrupt          : 1; /**< Interrupt indicator:
					   0 = No interrupt on packet
					       arrival.
					   1 = Interrupt on packet arrival.
					       This bit is set by the message
					       unit, depending on a bit in the
					       descriptor.  It is only set to 1
					       in the last packet of a message.
				      */
    } __attribute__((__packed__));

  } __attribute__((__packed__)) Byte2; /* End of Byte2 union. */

  union
  {
    uint8_t Byte3;               /**< Byte 3.  All bits set by software.
				  */

    struct
    {
      uint8_t Virtual_channel : 3; /**< Virtual channel:
				        0 = Dynamic.
					1 = Deterministic (escape).
					2 = High priority.
					3 = System.
					4 = User CommWorld.
					5 = User SubCommunicator.
					6 = System collective.
				   */

      uint8_t Zone_Routing_Id : 2; /**< Zone Routing Id.  Used only with dynamic
				        routing.  This Id refers to one of 4
					sets of DCRs containing zone masks.  For
					selecting a dynamic direction, the packet
					hint bits are ANDed with the appropriate
					zone mask to restrict the set of dynamic
					directions that may be chosen.  For a
					given set of DCRs identified by the zone
					routine Id, the first mask is used until
					the destination in the first dimension is
					reached, then the next mask is used for
					the next dimension, etc.  
				   */

      uint8_t Stay_On_Bubble  : 1; /**< Stay on bubble.  Used only with dynamic
				        routing.  Dynamic packets may move to the
					"bubble" virtual channel to prevent
					deadlocks.  Once a packet has moved to
					the bubble VC, this indicator controls
					whether the packet will stay on the
					bubble VC or not, as follows:
					0 = Do not stay on the bubble VC.
					1 = Stay on the bubble VC until the
					    packet has reached its destination.
				   */

      uint8_t Reserved1       : 2; /**< Reserved.        */

    } __attribute__((__packed__));

  } __attribute__((__packed__)) Byte3; /* End of Byte3 union. */

  MUHWI_Destination_t Destination; /**< Bytes 4 - 7.  Destination field.
				        Set by software.
				   */

  union
  {
    uint8_t Byte8;           /**< Byte 8.  All bits set by software.
			      */
    
    struct {
      uint8_t Packet_Type : 2; /**< Packet type:
				    00 = FIFO.
				    01 = Put.
				    10 = Get.
				    11 = Paced-get.
			       */
    
      uint8_t Reserved3   : 1; /**< Reserved. */
    
      uint8_t Size        : 5; /**< Number of 32 byte data payload chunks
				    in the packet (0 to 16).
			       */

    } __attribute__((__packed__));

  } __attribute__((__packed__)) Byte8; /* End of Byte8 union. */


  /** The last 3 bytes of this header can be viewed in two ways:
   *  - On injection, the last byte is set by software.  Other bits are used
   *    by hardware during the injection, and are not defined here.
   *  - Once inside the network, all 3 bytes are used/modified by hardware
   *    and are not defined here.
   */
  union
  {
    uint8_t Injection_Info[3]; /**< Bytes 9 - 11.  Injection Information.
				*/

    struct
    {
      uint16_t Reserved4;      /**< Used by hardware. */
    
      uint8_t Skip;            /**< Number of 4-byte words to skip for
		                    injection checksum.
		                    Set by software.
		               */
    } __attribute__((__packed__));
    
  } __attribute__((__packed__)) Injection_Info; /* End: Injection_Info union. */

} __attribute__((__packed__)) MUHWI_Pt2PtNetworkHeader_t;

__END_DECLS


#endif /* _MUHWI_PT2PT_NETWORK_HEADER_H_ */
