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
/* (C) Copyright IBM Corp.  2010, 2012                              */
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
/**
 * \file spi/include/mu/Classroute.h
 * \brief Basic classroute datatypes and defines
 */

#ifndef _MUSPI_CLASSROUTE_H_
#define _MUSPI_CLASSROUTE_H_

/**
 * @defgroup CLASSROUTE_DIRECTIONS Directions along axii for use with classroute SPIs
 * @{
 */
#define CR_SIGN_POS	0	/**< positive (plus) direction */
#define CR_SIGN_NEG	1	/**< negative (minus) direction */
#define CR_NUM_DIRS	2	/**< total number of directions */
/** @} */

/**
 * @defgroup CLASSROUTE_AXII Axii definitions for use with classroute SPIs
 * @{
 */
#define CR_AXIS_A       0	/**< first axis, default name "A" */
#define CR_AXIS_B       1	/**< second axis, default name "B" */
#define CR_AXIS_C       2	/**< third axis, default name "C" */
#define CR_AXIS_D       3	/**< fourth axis, default name "D" */
#define CR_AXIS_E       4	/**< fifth axis, default name "E" */
#define CR_NUM_DIMS     5	/**< total number of dimensions/axii */
/** @} */

#define CR_DIM_NAMES    "ABCDE" /**< default dimension names (labels) */
#define CR_DIR_NAMES	"+-"	/**< default direction names (labels) */

#include "kernel_impl.h"
#include "hwi/include/bqc/classroute.h"

/**
 * \brief Classroute bits mask for torus links
 *
 * All exclude the IO link
 */
#define CR_COLL_CLASS_LINK_MASK		(BGQ_CLASS_LINK_AM |\
					BGQ_CLASS_LINK_AP |\
					BGQ_CLASS_LINK_BM |\
					BGQ_CLASS_LINK_BP |\
					BGQ_CLASS_LINK_CM |\
					BGQ_CLASS_LINK_CP |\
					BGQ_CLASS_LINK_DM |\
					BGQ_CLASS_LINK_DP |\
					BGQ_CLASS_LINK_EM |\
					BGQ_CLASS_LINK_EP)

/**
 * \brief Classroute bits mask for up-tree links
 */
#define CR_COLL_CLASS_OUTPUT_MASK	CR_COLL_CLASS_LINK_MASK

/**
 * \brief Classroute bits mask for down-tree links
 */
#define CR_COLL_CLASS_INPUT_MASK	(CR_COLL_CLASS_LINK_MASK |\
					BGQ_CLASS_INPUT_LINK_LOCAL |\
					BGQ_CLASS_INPUT_VC_MASK)

/**
 *  \page Classroute_impl.h The Classroute_impl.h header must define:
 *
 *  CR_LINK(n,s)	Create bit-mask for n-th link (dim) in 's' direction (CR_SIGN_POS...)
 *
 *  CR_COORD_T			datatype for a coordinate (CR_NUM_DIMS)
 *  CR_COORD_DIM(coordp,dim)	accessor for coordp->[dim] (must work for both set/get)
 *
 *  CR_RECT_T		datatype for a rectangle (two coordinates)
 *  CR_RECT_LL(rectp)	accessor for rectp->lower-left-coord (get/put/CR_COORD_DIM())
 *  CR_RECT_UR(rectp)	accessor for rectp->upper-right-coord (get/put/CR_COORD_DIM())
 *
 * User's makefile must add "spi/include/mu/$(TARGET)" to include path,
 * where TARGET is something like "pami", "default", etc.
 */
#include "Classroute_impl.h"

#endif /* _MUSPI_CLASSROUTE_H_ */
