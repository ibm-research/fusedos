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

#ifndef __hwi_bqc_class_route_h__
#define __hwi_bqc_class_route_h__

/**
 * \file classroute.h
 *
 * \brief Class Route Definitions
 * 
 * There are two sets of 16 class routes:
 * 1. Collective
 * 2. GI (Barrier)
 * Within each set, each class route is identified by an Id (0..16).
 *
 * The nodes in a collective or barrier are arranged in a tree.
 * On each node of the tree, the class route defines that node's participation
 * in the tree.  Each class route may define a different participation for
 * the node.  There is an "input" participation, and an "output" participation.
 * 
 * A "collective" class route consists of 32 bits, organized as follows:
 *
 * - 2 bits unused
 *
 * - 2 bits class type ("00" is user commworld, "01" is system, "10" is user 
 *   subcomm).  These class type bits must correspond to the virtual
 *   channel (VC) specified in the packet.  For example, a VC=5 (user subcomm)
 *   packet must have a class route Id that has the class type bits set to "10".
 *
 * - 12 bits of uptree inputs.  This is a bit mask with 10 bits of torus link
 *   inputs, (ordered A-, A+, ..., E-, E+), 1 bit of IO link input, and 1 bit of
 *   local input.  If bit "i" is set,  then in a reduction,  as packets move up
 *   the tree, an input must be received on the corresponding link (injection 
 *   FIFO for local).
 *
 * - 4 bit unused
 * 
 * - 12 bits of uptree outputs.  This is a bit mask using the same ordering 
 *   convention as for the uptree inputs.  At most 1 bit may be set, and the
 *   local bit must be 0.  If an output bit is set, the corresponding input bit
 *   cannot be set.  The root of a tree has no output bit set.
 *
 * There are three types of packets, as specified in the packet header:
 *
 * - Broadcast:   One node in the tree injects packets,  They travel up the tree
 *   to the root of the tree (which has no output link) and then are rebroadcast
 *   back down the tree and are received by all nodes in the tree having the
 *   local input bit set.
 *
 * - All-Reduce:  Every node with an input bit set injects packets.  They are
 *   combined as they move up the tree and rebroadcast back down the tree.  The
 *   reduced packets are received at all nodes in the tree having the local
 *   input bit set.
 * 
 * - Reduce:  Every node with an input bit set injects packets.  They are
 *   combined as they move up the tree and rebroadcast back down the tree.  The
 *   reduced packets are received only at the node in the tree with the
 *   specified destination in the packet header.
 *
 * A "GI (barrier)" class route consists of 32 bits, organized as follows:
 * 
 * - 4 bits unused
 *
 * - 12 bits of uptree inputs.  This is a bit mask with 10 bits of torus link
 *   inputs, (ordered A-, A+, ..., E-, E+), 1 bit of IO link input, and 1 bit of
 *   local input.  If bit "i" is set,  then in a reduction,  as packets move up
 *   the the tree, an input must be received on the corresponding link
 *   (or local).
 *
 * - 4 bit unused
 *
 * - 12 bits of uptree output links.  This is a bit mask using the same ordering
 *   convention as for the uptree inputs.  At most 1 bit may be set, and the
 *   local bit must be 0.  If an output bit is set, the corresponding input bit
 *   cannot be set.  The root of a tree has no output bit set. 
 *
 */

#define BGQ_COLL_CLASS_MAX_CLASSROUTES        16 /**< Number of class routes for
						      collectives. */

#define BGQ_GI_CLASS_MAX_CLASSROUTES          16 /**< Number of class routes for
						      GI (barrier) */

#define BGQ_GI_CLASS_MAX_GI_PER_CLASSROUTE     3 /**< Number of global interrupts
						      in each class route */

#define BGQ_CLASS_INPUT_VC_USER      (0 << 12) /**< user comm-world VC 
                                                    (input only) */
#define BGQ_CLASS_INPUT_VC_SYSTEM    (1 << 12) /**< system comm-world VC
                                                    (input only) */
#define BGQ_CLASS_INPUT_VC_SUBCOMM   (2 << 12) /**< subcomm VC
                                                    (input only) */

#define BGQ_CLASS_LINK_AM	(1 << 11)	/**< Enable A- direction */
#define BGQ_CLASS_LINK_AP	(1 << 10)	/**< Enable A+ direction */

#define BGQ_CLASS_LINK_BM	(1 << 9)	/**< Enable B- direction */
#define BGQ_CLASS_LINK_BP	(1 << 8)	/**< Enable B+ direction */

#define BGQ_CLASS_LINK_CM	(1 << 7)	/**< Enable C- direction */
#define BGQ_CLASS_LINK_CP	(1 << 6)	/**< Enable C+ direction */

#define BGQ_CLASS_LINK_DM	(1 << 5)	/**< Enable D- direction */
#define BGQ_CLASS_LINK_DP	(1 << 4)	/**< Enable D+ direction */

#define BGQ_CLASS_LINK_EM	(1 << 3)	/**< Enable E- direction */
#define BGQ_CLASS_LINK_EP	(1 << 2)	/**< Enable E+ direction */

#define BGQ_CLASS_LINK_IO	(1 << 1)	/**< Enable I/O link */

#define BGQ_CLASS_INPUT_LINK_LOCAL   (1 << 0)   /**< Enable local contribution
                                                     (input only) */

#define BGQ_CLASS_INPUT_VC_MASK      (3 << 12)  /**< Mask over the VC bits, used
						     for extracting them from
						     the class route. */

/** \brief Class Route Structure Defining the Class Route */
typedef struct  ClassRoute {
  uint16_t           input;         /**< Input bit fields that show
				         which links provide inputs
				         to the collective/GI tree in the
					 up-tree phase (in the down-tree phase
					 these becomes outputs) */

  uint16_t           output;        /**< Output bit fields that show
				         which links are the
				         outputs of the collective/GI
				         tree in the up-tree phase (in the 
                                         down-tree phase these become inputs) */
} ClassRoute_t;

#endif /* __hwi_bqc_class_route_h__ */
