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

#ifndef	_MUHWI_MESSAGE_UNIT_HEADER_H_ /* Prevent multiple inclusion */
#define	_MUHWI_MESSAGE_UNIT_HEADER_H_


/**
 * \file MU_MessageUnitHeader.h
 *
 * \brief C Header File containing Message Unit Packet Header Definitions
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
 * |     - Collective Network Header     | MU_CollectiveNetworkHeader.h
 * |   - Message Unit Header             | MU_MessageUnitHeader.h  <<<<<<<
 * |--------------------------------------
 * \endverbatim
 */


#include <hwi/include/common/compiler_support.h>


__BEGIN_DECLS


/**
 * \brief Packet is Not Paced
 * 
 * A boolean indicating this direct put packet is not paced.
 */
#define MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED  0


/**
 * \brief Packet is Paced
 * 
 * A boolean indicating this direct put packet is paced.
 */
#define MUHWI_PACKET_DIRECT_PUT_IS_PACED  1


/**
 * \brief Message Unit Header
 *
 * This is the message unit portion of the packet header.  It occupies bytes
 * 12 through 31 of the packet header, following the network header.
 *
 * \see MU_Pt2PtNetworkHeader.h
 * \see MU_CollectiveNetworkHeader.h
 *
 */
typedef struct MUHWI_MessageUnitHeader
{
  union 
  {
    /***********************************/
    /* Memory FIFO Message Unit        */
    /***********************************/
    struct 
    {
      struct
      {
	uint16_t Reserved1      :  1; /**< Reserved.  Cannot be used by
                                           software.
				      */
	uint16_t Rec_FIFO_Id    :  9; /**< Reception FIFO Id to receive the
					   packet (0 to 271).  Set by
					   software.
				      */
      
	uint16_t Unused1        :  1; /**< Unused/Unchanged by the
					   hardware.
				      */
      
	uint16_t Put_Offset_MSB :  5; /**< Put Offset (Most significant 5
				           bits).  Software sets this to an
					   initial value, and hardware
					   updates it in each packet.
					   For a memory FIFO packet, this
					   serves as a packet counter.
					   For a single packet message, this
					   is unused/unchanged by the
					   hardware, so it may be set to
					   anything by software.
				    */
      } __attribute__((__packed__));
      
      uint32_t Put_Offset_LSB;      /**< Put Offset (Least significant 32
				         bits).  Software sets this to an
					 initial value, and hardware
					 updates it in each packet.
					 For a memory FIFO packet, this
					 serves as a packet counter.
					 For a single packet message, this
					 is unused/unchanged by the
					 hardware, so it may be set to
					 anything by software.
				    */

      uint8_t  Unused2[14];         /**< Unused/Unchanged by the
				         hardware
				    */
      
    } __attribute__((__packed__)) Memory_FIFO;

    /**********************************/
    /* Direct Put Message Unit Header */
    /**********************************/
    struct 
    {
      struct
      {
	uint16_t Rec_Payload_Base_Address_Id : 10; /**< Reception payload's 
						        base address Id.
							Set by software.
						   */
      
	uint16_t Pacing         : 1; /**< Pacing bit.
				          0 = Packet is not paced
                                              (MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED).
					  1 = Packet is paced
					      (MUHWI_PACKET_DIRECT_PUT_IS_PACED).
					  Set by software.
				     */

	uint16_t Put_Offset_MSB : 5; /**< Reception Put Offset (Most
				          significant 5 bits).  Software
					  sets this to an initial value,
					  and hardware updates it in each
					  packet.
				     */
      } __attribute__((__packed__));
            
      uint32_t Put_Offset_LSB;     /**< Reception Put Offset (Least
				        significant 32 bits).  Software
					sets this to an initial value,
					and hardware updates it in each
					packet.
				   */

      struct
      {
	uint16_t Unused1                     : 6;  /**< Unused/Unchanged by 
						        the hardware 
						   */

	uint16_t Rec_Counter_Base_Address_Id : 10; /**< Reception counter's 
						        base address Id. 
							Set by software.
						   */
      } __attribute__((__packed__));

      uint16_t Valid_Bytes_In_Payload; /**< Number of valid bytes in the
					    packet payload.
					    Set by hardware.
				       */
      uint16_t Unused2;                /**< Unused/Unchanged by 
					    the hardware.
				       */
      
      uint64_t Counter_Offset;     /**< Counter offset LSB 37 bits.
				        Set by software.
				   */
      
    } __attribute__((__packed__)) Direct_Put;

    /**********************************/
    /* Remote Get Message Unit Header */
    /**********************************/
    struct 
    {
      struct
      {
	uint16_t Rget_Inj_FIFO_Id : 10; /**< Remote Get Injection FIFO Id.
					     Set by software.
					*/
      
	uint16_t Unused1          :  6; /**< Unused/Unchanged by the
					     hardware.
					*/
      } __attribute__((__packed__));
      
      uint8_t  Unused2[18];           /**< Unused/Unchanged by the
					   hardware.
				      */
    } __attribute__((__packed__)) Remote_Get;
    
  } __attribute__((__packed__)) Packet_Types; /* End of union of packet types */

} __attribute__((__packed__)) MUHWI_MessageUnitHeader_t;




__END_DECLS


#endif /* _MUHWI_MESSAGE_UNIT_HEADER_H_ */
