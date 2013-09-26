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

#ifndef	_MUHWI_PACKET_COMMON_H_ /* Prevent multiple inclusion */
#define	_MUHWI_PACKET_COMMON_H_


/**
 * \file MU_PacketCommon.h
 *
 * \brief C Header File containing Message Unit Common Packet
 *        Definitions.
 *
 */


#include <hwi/include/common/compiler_support.h>


__BEGIN_DECLS


/**
 * \brief Packet Header Size In Bytes
 */
#define MUHWI_PACKET_HEADER_SIZE_IN_BYTES 32


/**
 * \brief Packet Maximum Payload Size In Bytes
 */
#define MUHWI_PACKET_MAX_PAYLOAD_SIZE_IN_BYTES 512

/**
 * \brief Byte2 of the Network Packet Header: Interrupt.
 */
#define MUHWI_PACKET_INTERRUPT_ON_PACKET_ARRIVAL  (0x01)


/**
 * \brief Byte2 of the Network Packet Header: Do Not Interrupt.
 */
#define MUHWI_PACKET_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL  (0x00)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - Dynamic.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_DYNAMIC  (0x00)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - Deterministic.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC  (0x20)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - High Priority.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_HIGH_PRIORITY  (0x40)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - System.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM  (0x60)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - User CommWorld.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_USER_COMM_WORLD  (0x80)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - User SubCommunicator
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_USER_SUB_COMM    (0xa0)


/**
 * \brief Byte3 of the Network Packet Header: Virtual Channel - System 
 *        Collective.
 */
#define MUHWI_PACKET_VIRTUAL_CHANNEL_SYSTEM_COLLECTIVE  (0xc0)


/**
 * \brief Byte3 of the Network Packet Header: Stay On Bubble.
 */
#define MUHWI_PACKET_STAY_ON_BUBBLE  (0x04)


/**
 * \brief Byte3 of the Network Packet Header: Do Not Stay On Bubble.
 */
#define MUHWI_PACKET_DO_NOT_STAY_ON_BUBBLE  (0x00)


/**
 * \brief Byte8 of the Network Packet Header: Packet Type - FIFO.
 */
#define MUHWI_PACKET_TYPE_FIFO  (0x00)


/**
 * \brief Byte8 of the Network Packet Header: Packet Type - Put.
 */
#define MUHWI_PACKET_TYPE_PUT  (0x40)


/**
 * \brief Byte8 of the Network Packet Header: Packet Type - Get.
 */
#define MUHWI_PACKET_TYPE_GET  (0x80)


/**
 * \brief Byte8 of the Network Packet Header: Packet Type - Paced Get.
 */
#define MUHWI_PACKET_TYPE_PACED_GET  (0xc0)


/**
 * \brief Torus Destination
 * 
 * This structure has the coordinates of the destination node.  It is organized
 * such that the torus coordinates are in the same bit locations as in the
 * packet header.  This structure is used as input to descriptor-building
 * functions so these coordinates can be copied directly into the descriptor, 
 * minimizing bit movements.  Thus, it is to the caller's advantage to cache the
 * destination coordinates in this format.
 */
typedef struct MUHWI_Destination
{
  union
  {
    uint32_t Destination;       /**< The full destination field.
				     Set by software. 
				*/
    
    struct
    {
      uint32_t Reserved2     : 2; /**< Reserved.  Initialize to zero. */
    
      uint32_t A_Destination : 6; /**< A destination address. */
    
      uint32_t B_Destination : 6; /**< B destination address. */
    
      uint32_t C_Destination : 6; /**< C destination address. */
    
      uint32_t D_Destination : 6; /**< D destination address. */
    
      uint32_t E_Destination : 6; /**< E destination address. */

    } __attribute__((__packed__));

  } __attribute__((__packed__)) Destination;
  
} __attribute__((__packed__)) MUHWI_Destination_t;


__END_DECLS


#endif /* _MUHWI_PACKET_COMMON_H_ */
