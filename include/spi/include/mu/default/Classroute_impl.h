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
 * \file spi/include/mu/default/Classroute_impl.h
 * \brief Default definitions for basic classroute node sets
 */

#ifndef _MUSPI_CLASSROUTE_IMPL_H_
#define _MUSPI_CLASSROUTE_IMPL_H_

#include <sys/types.h>
#include <stdint.h>

/**
 * \brief default representation of a node's coordinates
 */
typedef struct {
	uint32_t coords[CR_NUM_DIMS];	/**< one coord for each dimension */
} CR_COORD_T;

#define CR_COORD_INIT(c...)	(CR_COORD_T){{ c }}

/**
 * \brief accessor for a single dimension's coord
 *
 * \param[in] c		CR_COORD_T pointer
 * \param[in] d		dimension
 * \return	left or right-side expession for accessing the coord
 */
#define CR_COORD_DIM(c,d)		(c)->coords[d]

/**
 * \brief default representation of a rectangle
 */
typedef struct {
	CR_COORD_T ll;	/**< coords of lower-left corner of rectangle */
	CR_COORD_T ur;	/**< coords of upper-right corner of rectangle */
} CR_RECT_T;

/**
 * \brief accessor for lower-left corner of rectangle
 *
 * \param[in] r		CR_RECT_T pointer
 * \return	left or right-side expession for accessing the lower-left coord
 */
#define CR_RECT_LL(r)	(&(r)->ll)

/**
 * \brief accessor for upper-right corner of rectangle
 *
 * \param[in] r		CR_RECT_T pointer
 * \return	left or right-side expession for accessing the upper-right coord
 */
#define CR_RECT_UR(r)	(&(r)->ur)

#endif /* _MUSPI_CLASSROUTE_IMPL_H_ */
