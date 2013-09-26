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

#ifndef	_MUHWI_DESCRIPTOR_H_ /* Prevent multiple inclusion */
#define	_MUHWI_DESCRIPTOR_H_


/**
 * \file MU_Descriptor.h
 *
 * \brief C Header File containing Message Unit Descriptor Definitions
 *
 * The descriptor structure is defined here and inline setter
 * functions are provided to set individual fields.
 *
 * \verbatim
 * MU_Descriptor.h  <<<<<<<
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
 * |   - Message Unit Header             | MU_MessageUnitHeader.h
 * |--------------------------------------
 * \endverbatim
 *
 * Usage Scenario:  Point-to-Point Compute Node to Compute Node Routing
 *
 * Routing is similar to BG/L and BG/P.  The packet contains a virtual channel
 * (VC), a destination, hint bits and other routing control information.
 * There are 10 hint bits, 1 for each direction (2 bits per dimension)
 * indicating whether to route the packet in that direction.  It is illegal to
 * set the both the plus and minus hint bits for a dimension.  If all the hint
 * bits are 0, the hardware hint bit calculator will compute the hint bits.
 * Point-point packets flow along directions specified by the hint bits until
 * reaching their destination in that direction.  Statically routed packets flow
 * along a fixed order of dimensions: unlike BG/L and BG/P, this order can be
 * set by software in DCRs: all nodes must program these DCRs the same way, or
 * else deadlock can occur.  The hardware supports a broadcast down rows by
 * setting the deposit bit in the packet header. 
 *
 * Dynamic routing is enabled by specifying the dynamic bit and the dynamic VC.
 * To improve performance on asymmetric tori, zone routing can be used to force
 * dynamic packets down certain dimensions before others.  The packet contains
 * 2 zone bits, which point to 4 sets of DCRs containing zone masks.  These are
 * only used in dynamic routing.  For selecting a dynamic direction, the packet
 * hint bits are ANDed with the appropriate zone mask to restrict the set of
 * dynamic directions that may be chosen.  For a given set of zone masks, the
 * first mask is used until the destination in the first dimension is reached,
 * etc.  For example, in a 2n x n x n x n x 2 torus, one could set up the masks
 * so that packets are routed along A first, then either B, C or D, and finally
 * E.  For random traffic patterns this tends to have packets moving from more
 * busy links onto less busy links.  Regardless of the zone bits, a dynamic
 * packet may move to the bubble VC to prevent deadlocks.  If all the mask bits
 * are set to 1, there is such no ordering of dynamic directions.  In addition,
 * a "stay on bubble" bit may be set; if a dynamic packet enters the bubble VC,
 * this bit causes the packet to stay on the bubble VC until reaching its
 * destination.   
 *
 * Usage Scenario:  Point-to-Point Compute Node to I/O Node Routing
 *
 * For a compute node to send a message to IO node, it does the following:
 * 1. Sets the destination coordinates to be that of the compute node that is
 *    connected to the IO node via the 11th link.
 * 2. Sets route to I/O node bit to 1.
 * 3. Sets the VC to the system point-point channel.
 * 4. Optionally sets the hint bits, or leaves them at 0 in which case hardware
 *    sets them.
 *
 * The packet is deterministically routed to the compute node destination along
 * the system VC.  When the packet reaches the compute node destination, it
 * examines the route to I/O node bit.  If it is set, the packet is then sent
 * over port 10 (11th link) to the I/O node.  Upon reaching the I/O node, the
 * packet is received into the system network reception FIFO.  If the bit was
 * not set, the packet would have been received at the compute node destination.
 * The deposit and dynamic bits in the header must be 0 and the zone bits are
 * ignored.
 *
 * Note that the deposit bit may be set for broadcasts along lines of the torus
 * for packets on the system channel provided the route to I/O node bit is 0.
 * In this case, the packet stays entirely within the set of compute nodes.
 *
 * Usage Scenario:  Point-to-Point I/O Node to Compute Node Routing
 *
 * For an I/O node to send a message to a compute node, it does the following:
 * 1. Sets the destination coordinates to be that of the destination compute node.
 * 2. Sets the route to I/O node bit to 0, but sets the return from I/O node bits
 *    to the appropriate port, either 8, 9 or 10 depending on which link is to be
 *    used.
 * 3. Sets VC to the system point-point channel.
 * 4. Optionally sets the hint bits, or leaves them at 0 in which case hardware
 *    sets them upon entry to the first compute node.  If the hint bits are
 *    non-zero, they are respected subject to error checks.
 * The packet is first sent to the compute node along the port as specified in
 * the return from I/O node bits and is then deterministically routed to the
 * compute node destination along the system VC.  The deposit and dynamic bits
 * must be zero and the zone bits are ignored.  When the packet reaches the
 * compute node destination, it is received into the system network reception
 * FIFO.
 *
 * Usage Scenario: Local Transfer Messages
 *
 * 1. The message destination must equal the node coordinates.
 * 2. The Injection memory FIFO map must point to an appropriate network
 *    injection FIFO.
 *    a. For normal priority packets, there are 2 network injection FIFOs that
 *       are dedicated to local transfer.  One, or both must be selected.
 *    b. For high priority local transfer, point to the high priority injection
 *       FIFO.
 *    c. For system local transfer, point to the system injection FIFO.
 * 3. The VC must be consistent with the FIFO (e.g., for high priority local
 *    transfer, it must specify the high priority VC).
 * 4. The hint bits must be 0.
 *
 * Usage Scenarios with Atomic Operations
 *
 * 1. In the direct put descriptor, the counter offset or the value in 
 *    the counter's base address table slot must be formatted as an MU
 *    atomic address.  The MU sums the value in the base address table slot
 *    with the counter offset to produce an MU atomic address that references
 *    an 8-byte counter.  When a packet arrives, the MU stores the negaive of
 *    the number of bytes in the packet to that address, decrementing the
 *    counter.  The atomic opcode must be one of the following:
 *    - MUHWI_ATOMIC_OPCODE_STORE_ADD
 *    - MUHWI_ATOMIC_OPCODE_STORE_ADD_COHERENCE_ON_ZERO
 *
 * 2. Send from an atomic field.  When the physical address of the send
 *    buffer in the descriptor is formatted as an MU atomic address (with
 *    a "load" type of opcode), the message length must be 8 bytes.  The
 *    MU performs the atomic operation on the 8-byte send buffer, sending
 *    the loaded value as the data.
 *
 * 3. Receive into an atomic field.  For a direct put, when the put offset or the
 *    value in the put offset's base address table slot is formatted as an
 *    MU atomic address, the message length must be 8 bytes.  The MU sums the
 *    value in the base address table slot with the put offset to produce an 
 *    MU atomic address that references an 8-byte field.  When a packet 
 *    arrives, The MU stores the received data to that field, performing the 
 *    atomic operation on that field.  The atomic opcode must be one of the
 *    "store" opcodes.
 */


#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_PacketCommon.h>
#include <hwi/include/bqc/MU_PacketHeader.h>



__BEGIN_DECLS


/**
 * \brief Pre-fetch Only - Yes
 *
 * Indicates that the data should be pre-fetched but not sent.
 */
#define MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_YES  (1)


/**
 * \brief Pre-fetch Only - No
 *
 * Indicates that the data should not be only pre-fetched, but sent instead.
 */
#define MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO  (0)


/**
 * \brief Interrupt on Packet Arrival - Yes
 * 
 * Indicates that an interrupt should occur when the last packet of this
 * message arrives at the destination node's reception memory FIFO.
 */
#define MUHWI_DESCRIPTOR_INTERRUPT_ON_PACKET_ARRIVAL  (1)


/**
 * \brief Interrupt on Packet Arrival - No
 * 
 * Indicates that an interrupt should not occur when the last packet of this
 * message arrives at the destination node's reception memory FIFO.
 */
#define MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL  (0)


/**
 * \brief Torus Injection FIFO Map - Torus A- FIFO
 *
 * Specifies to allow injection into the Torus A- hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM       (0x0000000000008000ULL)


/**
 * \brief Torus Injection FIFO Map - Torus A+ FIFO
 *
 * Specifies to allow injection into the Torus A+ hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP       (0x0000000000004000ULL)


/**
 * \brief Torus Injection FIFO Map - Torus B- FIFO
 *
 * Specifies to allow injection into the Torus B- hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM       (0x0000000000002000ULL)


/**
 * \brief Torus Injection FIFO Map - Torus B+ FIFO
 *
 * Specifies to allow injection into the Torus B+ hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP       (0x0000000000001000ULL)


/**
 * \brief Torus Injection FIFO Map - Torus C- FIFO
 *
 * Specifies to allow injection into the Torus C- hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM       (0x0000000000000800ULL)


/**
 * \brief Torus Injection FIFO Map - Torus C+ FIFO
 *
 * Specifies to allow injection into the Torus C+ hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP       (0x0000000000000400ULL)


/**
 * \brief Torus Injection FIFO Map - Torus D- FIFO
 *
 * Specifies to allow injection into the Torus D- hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM       (0x0000000000000200ULL)


/**
 * \brief Torus Injection FIFO Map - Torus D+ FIFO
 *
 * Specifies to allow injection into the Torus D+ hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP       (0x0000000000000100ULL)


/**
 * \brief Torus Injection FIFO Map - Torus E- FIFO
 *
 * Specifies to allow injection into the Torus E- hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM       (0x0000000000000080ULL)


/**
 * \brief Torus Injection FIFO Map - Torus E+ FIFO
 *
 * Specifies to allow injection into the Torus E+ hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP       (0x0000000000000040ULL)


/**
 * \brief Torus Injection FIFO Map - Local 0 FIFO
 *
 * Specifies to allow injection into the Local 0 hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0   (0x0000000000000020ULL)


/**
 * \brief Torus Injection FIFO Map - Local 1 FIFO
 *
 * Specifies to allow injection into the Local 1 hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1   (0x0000000000000010ULL)


/**
 * \brief Torus Injection FIFO Map - Priority FIFO
 *
 * Specifies to allow injection into the Priority hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_PRIORITY (0x0000000000000008ULL)


/**
 * \brief Torus Injection FIFO Map - System FIFO
 *
 * Specifies to allow injection into the System hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_SYSTEM   (0x0000000000000004ULL)


/**
 * \brief Torus Injection FIFO Map - Collective User FIFO
 *
 * Specifies to allow injection into the Collective User hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CUSER    (0x0000000000000002ULL)


/**
 * \brief Torus Injection FIFO Map - Collective System FIFO
 *
 * Specifies to allow injection into the Collective System hardware FIFO.
 * This is a bit in the descriptor's Torus_FIFO_Map field.
 */
#define MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CSYSTEM  (0x0000000000000001ULL)


/**
 * \brief Message Unit Descriptor
 *
 * This is the descriptor that is injected into an in-memory-injection-FIFO.
 * It consists of a descriptor section, followed by the packet header.
 *
 * \see MU_PacketHeader.h
 *
 */
typedef struct MUHWI_Descriptor
{
  union 
  {
    uint32_t Half_Word0;           /**< Half word 0. */

    struct
    {
      uint32_t Unused1       : 31; /**< Not used. */
      
      uint32_t Prefetch_Only :  1; /**< Prefetch indicator.  One of:
				        - MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO
					  - No prefetch.  Message is sent over
					    the network.
					- MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_YES
					  - Prefetch only.  The message unit
					    only prefetches the data into L2.
					    No message is sent.
				   */
    } __attribute__((__packed__));

  } __attribute__((__packed__)) Half_Word0; /* End of Half word 0 union */

  union 
  {
    uint32_t Half_Word1;       /**< Half word 1. */

    struct
    {
      uint32_t Unused2   : 31; /**< Not used. */
      
      uint32_t Interrupt :  1; /**< Interrupt indicator.  One of:
			   - MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL
			     - No interrupt on packet arrival.
			   - MUHWI_DESCRIPTOR_INTERRUPT_ON_PACKET_ARRIVAL
			     - The message unit will set the interrupt bit in
			       the packet header, causing an interrupt when the
			       last packet of this message arrives at the
			       destination.  For memory FIFO packet types only.
			 */
    } __attribute__((__packed__));

  } __attribute__((__packed__)) Half_Word1; /* End of Half word 1 union */

  uint64_t Pa_Payload; /**< Payload starting physical address.
			    The least significant 37 bits are used.
		       */

  uint64_t Message_Length; /**< Message length.
			        Note that 0 is valid for memory FIFO
				packet types, but not for direct puts
				and remote gets.
			   */

  uint64_t Torus_FIFO_Map; /**< Torus injection FIFO map.
			        A 1 in bit i indicates that the
				message can be injected into Torus
				injection FIFO i.
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

  MUHWI_PacketHeader_t PacketHeader;

} __attribute__((__packed__, __aligned__(32))) MUHWI_Descriptor_t;

__END_DECLS


#endif /* _MUHWI_DESCRIPTOR_H_  */
