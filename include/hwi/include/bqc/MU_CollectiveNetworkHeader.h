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

#ifndef	_MUHWI_COLLECTIVE_NETWORK_HEADER_H_ /* Prevent multiple inclusion */
#define	_MUHWI_COLLECTIVE_NETWORK_HEADER_H_

/**
 * \file MU_CollectiveNetworkHeader.h
 *
 * \brief C Header File containing Message Unit Collective
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
 * |     - Point-to-Point Network Header | MU_Pt2PtNetworkHeader.h
 * |     - Collective Network Header     | MU_CollectiveNetworkHeader.h <<<<<<<
 * |   - Message Unit Header             | MU_MessageUnitHeader.h
 * |--------------------------------------
 * \endverbatim
 *
 */


#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_PacketCommon.h>


__BEGIN_DECLS


/**
 * \brief Collective Data Packet Type
 */
#define MUHWI_COLLECTIVE_DATA_PACKET_TYPE 0x5a


/**
 * \brief Collective OpCode: and.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_AND (0x00)


/**
 * \brief Collective OpCode: or.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_OR (0x10)


/**
 * \brief Collective OpCode: xor.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_XOR (0x20)


/**
 * \brief Collective OpCode: unsigned add.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_ADD (0x40)


/**
 * \brief Collective OpCode: unsigned min.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MIN (0x50)


/**
 * \brief Collective OpCode: unsigned max.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_UNSIGNED_MAX (0x60)


/**
 * \brief Collective OpCode: signed add.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_SIGNED_ADD (0x80)


/**
 * \brief Collective OpCode: signed min.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_SIGNED_MIN (0x90)


/**
 * \brief Collective OpCode: signed max.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_SIGNED_MAX (0xa0)


/**
 * \brief Collective OpCode: floating point add.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_ADD (0xc0)


/**
 * \brief Collective OpCode: floating point min.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MIN (0xd0)


/**
 * \brief Collective OpCode: floating point max.
 *
 * Meant to be OR'd directly into Byte1 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_OP_CODE_FLOATING_POINT_MAX (0xe0)


/**
 * \brief Collective Type: broadcast.
 *
 * Meant to be OR'd directly into Byte3 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_TYPE_BROADCAST (0x00)


/**
 * \brief Collective Type: all-reduce.
 *
 * Meant to be OR'd directly into Byte3 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_TYPE_ALLREDUCE (0x10)


/**
 * \brief Collective Type: reduce.
 *
 * Meant to be OR'd directly into Byte3 of the Collective Network Header.
 */
#define MUHWI_COLLECTIVE_TYPE_REDUCE (0x18)


/**
 * \brief Collective Network Header
 *
 * This is the collective portion of the packet header.  It occupies bytes
 * 0 through 11 of the packet header.
 *
 * \see MU_PacketCommon.h
 * \see MU_MessageUnitHeader.h
 * \see MU_PacketHeader.h
 *
 */
typedef struct MUHWI_CollectiveNetworkHeader
{
  uint8_t Data_Packet_Type; /**< Byte 0.  Packet type: 
			         MUHWI_COLLECTIVE_DATA_PACKET_TYPE.
				 Set by software.
			    */

  union
  {
    uint8_t Byte1;          /**< Byte 1.  All bits set by software.
			     */

    struct
    {
      uint8_t OpCode : 4;     /**< Collective op code.
				   Ignored for broadcast.
				   0000 = and,
				   0001 = or,
				   0010 = xor,
				   0011 = invalid,
				   0100 = unsigned add,
				   0101 = unsigned min,
				   0110 = unsigned max,
				   0111 = invalid,
				   1000 = signed add,
				   1001 = signed min,
				   1010 = signed max,
				   1011 = invalid,
				   1100 = floating point add,
				   1101 = floating point min,
				   1110 = floating point max,
				   1111 = invalid.
				   Refer to the following to set these op codes
				   into Byte1:
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
			      */
      
      uint8_t Word_Length : 4; /**< Collective word length exponent (n)
				    - Word_Length is 8 (bytes) for all floating
                                      point operations (single precision not
                                      supported).
                                    - A word is 4 bytes.
				    - Word_Length is the exponent (n) in 2^n.
				    - 2^n is the number of words.
				    - Valid values for Word_Length and their
                                      corresponding byte lengths are:
                                      Word_Length   Bytes
                                      0               4
                                      1               8
                                      2              16
                                      3              32
                                      4              64
                                      5             128
                                      6             256
                                      7             512
                                      The resulting number of bytes must
                                      divide the data packet size evenly.
                                    - For floating point operations, Word_Length
                                      is 1.
                                    - For broadcasts:
                                      - If a non-floating point opcode is used,
                                        then the payload is never changed but
                                        the Bytes must divide the packet size
                                        evenly, also for the last packet in
                                        the message.
                                      - If a floating point opcode is used,
                                        NaNs will be converted to a standard 
                                        signalling NaN and Word_Length must
                                        be 1.
			       */
    } __attribute__((__packed__));

  } __attribute__((__packed__)) Byte1; /* End of Byte1 union. */
      
  union
  {
    uint8_t Byte2;           /**< Byte 2.  All bits set by software.
			      */

    struct
    {
      uint8_t Class_Route : 4; /**< Collective class route. */

      uint8_t Reserved1   : 3; /**< Reserved. */

      uint8_t Interrupt   : 1; /**< Interrupt indicator:
				    0 = No interrupt on packet arrival.
				    1 = Interrupt on packet arrival.
				        This bit is set by the message
					unit, depending on a bit in the
					descriptor.  It is only set to 1
					in the last packet of a message.
					For memory FIFO packet types only.
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

      uint8_t Collective_Type : 2; /**< Collective type:
				        00 = Broadcast.
                                        01 = Reserved.
                                        10 = All-reduce.
					11 = Reduce.
					Refer to the following to set these
					types into Byte 3:
					- MUHWI_COLLECTIVE_TYPE_BROADCAST
                                        - MUHWI_COLLECTIVE_TYPE_ALLREDUCE
					- MUHWI_COLLECTIVE_TYPE_REDUCE
				   */

      uint8_t Reserved2       : 3; /**< Reserved.        */

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
				    11 = Not valid for collectives.
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

} __attribute__((__packed__)) MUHWI_CollectiveNetworkHeader_t;


__END_DECLS


#endif /* _MUHWI_COLLECTIVE_NETWORK_HEADER_H_ */
