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

#ifndef	_MUHWI_PACKET_HEADER_H_ /* Prevent multiple inclusion */
#define	_MUHWI_PACKET_HEADER_H_


/**
 * \file MU_PacketHeader.h
 *
 * \brief C Header File containing Message Unit Packet Header 
 *        Definitions.
 *
 * \verbatim Picture of data structures:
 *
 * MU_Descriptor.h
 * ---------------------------------------
 * | Descriptor                          |
 * | - Prefetch                          |
 * | - Interrupt                         |
 * | - Pa_Payload                        |
 * | - Message_Length                    |
 * | - Torus FIFO Map                    |
 * | - Packet Header                     | MU_PacketHeader.h <<<<<<<
 * |   - One of:                         |
 * |     - Point-to-Point Network Header | MU_Pt2PtNetworkHeader.h
 * |     - Collective Network Header     | MU_CollectiveNetworkHeader.h
 * |   - Message Unit Header             | MU_MessageUnitHeader.h
 * |--------------------------------------
 *
 * \endverbatim
 */


#include <hwi/include/common/compiler_support.h>
#include <hwi/include/bqc/MU_Pt2PtNetworkHeader.h>
#include <hwi/include/bqc/MU_CollectiveNetworkHeader.h>
#include <hwi/include/bqc/MU_MessageUnitHeader.h>


__BEGIN_DECLS


/**
 * \brief Packet Header
 *
 * This is the packet header.  It consists of either the point-to-point network 
 * header or the collective network header, followed by the message unit header.
 *
 * \see MU_Pt2PtNetworkHeader.h
 * \see MU_CollectiveNetworkHeader.h
 * \see MU_MessageUnitHeader.h
 *
 */
typedef struct MUHWI_PacketHeader
{
  union
  {
    MUHWI_Pt2PtNetworkHeader_t      pt2pt;      /**< Pt2Pt Network Header     */

    MUHWI_CollectiveNetworkHeader_t collective; /**< Collective Network Header*/

  } __attribute__((__packed__)) NetworkHeader;

  MUHWI_MessageUnitHeader_t  messageUnitHeader; /**< Message Unit portion     */

} __attribute__((__packed__)) MUHWI_PacketHeader_t;


__END_DECLS


#endif /* _MUHWI_PACKET_HEADER_H_ */
