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
 * \file spi/include/mu/Classroute_inlines.h
 * \brief inline routines for computing classroute links and selecting classroute IDs
 */

#ifndef _MUSPI_CLASSROUTE_INLINES_H_
#define _MUSPI_CLASSROUTE_INLINES_H_

#include <stdio.h>
#include <string.h>
#include <spi/include/mu/Classroute.h>

__BEGIN_DECLS

/**
 * \brief get-accessor for virtual channel bits in classroute
 * \param[in] crp	ClassRoute_t pointer
 * \return	Virtual channel field, unshifted
 * \see BGQ_CLASS_INPUT_VC_USER, BGQ_CLASS_INPUT_VC_SYSTEM, BGQ_CLASS_INPUT_VC_SUBCOMM
 */
#define GET_CR_ROUTE_VC(crp)	((crp)->input & BGQ_CLASS_INPUT_VC_MASK)

/**
 * \brief set-accessor for virtual channel bits in classroute
 * \param[in] crp	ClassRoute_t pointer
 * \param[in] vc	Virtual channel field, unshifted
 * \see BGQ_CLASS_INPUT_VC_USER, BGQ_CLASS_INPUT_VC_SYSTEM, BGQ_CLASS_INPUT_VC_SUBCOMM
 */
#define SET_CR_ROUTE_VC(crp,vc)	((crp)->input = ((crp)->input & ~BGQ_CLASS_INPUT_VC_MASK) | vc)

/**
 * \brief Array for selecting link bit from axis and direction without assuming any link bit pattern
 *
 * Only handles main torus links. I/O and LOCAL are handled separately/manually.
 */
#ifdef __cplusplus
static const uint16_t cr_links[][2] = {
  {BGQ_CLASS_LINK_AP, BGQ_CLASS_LINK_AM},
  {BGQ_CLASS_LINK_BP, BGQ_CLASS_LINK_BM},
  {BGQ_CLASS_LINK_CP, BGQ_CLASS_LINK_CM},
  {BGQ_CLASS_LINK_DP, BGQ_CLASS_LINK_DM},
  {BGQ_CLASS_LINK_EP, BGQ_CLASS_LINK_EM}
};
#else
static const uint16_t cr_links[][2] = {
[CR_AXIS_A][CR_SIGN_POS] = BGQ_CLASS_LINK_AP,
[CR_AXIS_A][CR_SIGN_NEG] = BGQ_CLASS_LINK_AM,
[CR_AXIS_B][CR_SIGN_POS] = BGQ_CLASS_LINK_BP,
[CR_AXIS_B][CR_SIGN_NEG] = BGQ_CLASS_LINK_BM,
[CR_AXIS_C][CR_SIGN_POS] = BGQ_CLASS_LINK_CP,
[CR_AXIS_C][CR_SIGN_NEG] = BGQ_CLASS_LINK_CM,
[CR_AXIS_D][CR_SIGN_POS] = BGQ_CLASS_LINK_DP,
[CR_AXIS_D][CR_SIGN_NEG] = BGQ_CLASS_LINK_DM,
[CR_AXIS_E][CR_SIGN_POS] = BGQ_CLASS_LINK_EP,
[CR_AXIS_E][CR_SIGN_NEG] = BGQ_CLASS_LINK_EM,
};
#endif

/**
 * \brief Macro for translating axis, direction into link bit
 *
 * Only handles main torus links. I/O and LOCAL are handled separately/manually.
 */
#define CR_LINK(n,s)    cr_links[n][s]

/** \brief Flag used to identify exact match of classroute rectangle
 */
#define CR_MATCH_RECT_FLAG	0x80000000

/**
 * \brief structure for managing classroute ID allocation
 *
 * All sub-comm classroutes are for full rectangles. comm-world may be
 * a sparse rectangle but the bounding rectangle can never be a sub-comm
 * (no match conflict) so there is no need to store the excluded node coordinates.
 */
struct cr_allocation {
	CR_RECT_T rect;		/**< classroute rectangle */
	uint32_t classid;	/**< classroute id (assigned) */
	ClassRoute_t classroute; /**< classroute link bits */
	struct cr_allocation *cr_peer;	/**< others with same same classroute */
};
struct cr_env {
	int isGI;
	struct cr_allocation *cr_alloc[BGQ_COLL_CLASS_MAX_CLASSROUTES];
};

/**
 * \brief Special value used to block-out system ids in allocation table
 *
 * Also used to block-out ids that are given to other components, such as
 * user code directly calling SPIs.
 */
#define CR_SYSTEM_ID	((struct cr_allocation *)-1)

/**
 * \brief Compute classroute for node "me"
 *
 * \param[in] world	Entire partition rectangle (COMM_WORLD)
 * \param[in] worldroot	Coordinates of root node in COMM_WORLD
 * \param[in] me	Coordinates of my node
 * \param[in] comm	Rectangle to produce classroute for
 * \param[in] dims	(optional) Translation/mapping of dimensions
 * \param[in] dim0	Starting dimension (iterate circular in + direction)
 * \param[out] cr	Classroute bitmaps
 *
 * Rectangles are defined by the coordinates of the lower-left and upper-right corners.
 * Note: upper-right is not size. Classroute consists of "up tree" bits and "down tree"
 * bits. Each bit represents a link. The "local contribution" link bit is not set here,
 * it is assumed it would always be "1".
 *
 * The classroute is based on the COMM_WORLD classroute, such that a given node
 * will have basically the same links in use for any classroute, except for nodes
 * on the inner edges of 'comm' (with respect to 'world').
 *
 * The COMM_WORLD classroute is as pictured here (2-D example):
 *
 * \code
 *   O -> O -> O -> O <- O <- O
 *                  |
 *                  V
 *   O -> O -> O -> O <- O <- O
 *                  |
 *                  V
 *   O -> O -> O -> R <- O <- O
 *                  ^
 *                  |
 *   O -> O -> O -> O <- O <- O
 *                  ^
 *                  |
 *   O -> O -> O -> O <- O <- O
 * \endcode
 *
 * Where 'R' is the root node, and the primary dimension is horizontal.
 *
 * If a non-rectangle communicator is being used, the circumscribing rectangle
 * must participate in the classroute creation. All nodes in the rectangle are
 * part of the classroute and only nodes in the communicator have local contribution,
 * except for leafs/branches with no local contribution which must be pruned from
 * the resulting classroute. In order for a node to know it must be pruned, or
 * that it is at a prune point, the node may have to create the entire classroute
 * (for all nodes) and evaluate it's position in it - or at least may have to
 * create the classroute down-tree from itself. Such "sparse rectangles" are built
 * using the MUSPI_BuildNodeClassrouteSparse() routine, which calls this.
 */
__INLINE__
void MUSPI_BuildNodeClassroute(CR_RECT_T *world, CR_COORD_T *worldroot, CR_COORD_T *me,
				CR_RECT_T *comm, int *dims, int dim0, ClassRoute_t *cr) {
	int d, dim;
	ClassRoute_t cr0 = {0};

	for (dim = 0; dim < CR_NUM_DIMS; ++dim) {
		d = dim0 + dim;
		if (d >= CR_NUM_DIMS) d -= CR_NUM_DIMS;
		d = dims ? dims[d] : d;
		if (CR_COORD_DIM(me,d) <= CR_COORD_DIM(worldroot,d)) {
			if (CR_COORD_DIM(me,d) > CR_COORD_DIM(CR_RECT_LL(comm),d)) {
				cr0.input |= CR_LINK(d,CR_SIGN_NEG);
			}
		}
		if (CR_COORD_DIM(me,d) >= CR_COORD_DIM(worldroot,d)) {
			if (CR_COORD_DIM(me,d) < CR_COORD_DIM(CR_RECT_UR(comm),d)) {
				cr0.input |= CR_LINK(d,CR_SIGN_POS);
			}
		}
		if (CR_COORD_DIM(me,d) < CR_COORD_DIM(worldroot,d)) {
			if (CR_COORD_DIM(me,d) < CR_COORD_DIM(CR_RECT_UR(comm),d)) {
				cr0.output |= CR_LINK(d,CR_SIGN_POS);
				break;
			}
		} else if (CR_COORD_DIM(me,d) > CR_COORD_DIM(worldroot,d)) {
			if (CR_COORD_DIM(me,d) > CR_COORD_DIM(CR_RECT_LL(comm),d)) {
				cr0.output |= CR_LINK(d,CR_SIGN_NEG);
				break;
			}
		}
	}
	*cr = cr0;
}

/**
 * \brief Determine if two coordinates are the same (in all dimensions)
 *
 * \param[in] c0	First coordinate to compare
 * \param[in] c1	Second coordinate to compare
 * \return	"true" (non-zero) if coordinates are the same
 */
static inline int __MUSPI_eq_coords(CR_COORD_T *c0, CR_COORD_T *c1) {
	int d;
	for (d = 0; d < CR_NUM_DIMS; ++d) {
		if (CR_COORD_DIM(c0,d) != CR_COORD_DIM(c1,d)) return 0;
	}
	return 1;
}

/**
 * \brief Determine if any down-tree nodes are in communicator
 *
 * Recursive routine. Traverses down-tree and if a link has no local contributors
 * then the link is pruned.
 *
 * Must build each classroute, so needs enough information to do that. This also
 * builds the top-level classroute (for the actual node in question). All down-tree
 * classroutes are temporary - discarded before return.
 *
 * Uses recursion level to optimize the search on down-tree links and return
 * immediately when local contribution is detected. This means the return value
 * may not accurately indicate the number of local contributors beneath. Level 0
 * is the actual node for which the class route is being computed. Note, the
 * level 0 node may not actually have local contribution (may not be part of the
 * communicator) but must still traverse the branch in order to be certain no
 * local contribution exists.
 *
 * \param[in] world	The entire partition rectangle
 * \param[in] worldroot	The root coordinates
 * \param[in] me	Coordinates of current node
 * \param[in] comm	The sub-communicator rectangle
 * \param[in] exclude	Coordinates of nodes NOT in communicator
 * \param[in] nexclude	Number of coordinates in 'exclude'
 * \param[in] dims	(optional) Translation/mapping of dimensions
 * \param[in] dim0	Primary dimension for classroute algorithm
 * \param[in,out] cr	Classroute (top level only)
 * \param[in] level	Recursion level
 * \return	number of local contributors beneath current node
 *		(typically only tested for non-zero)
 */
static inline int __MUSPI_find_local_contrib(CR_RECT_T *world, CR_COORD_T *worldroot, CR_COORD_T *me,
			CR_RECT_T *comm, CR_COORD_T *exclude, int nexclude,
			int *dims, int dim0, ClassRoute_t *cr, int level) {
	uint32_t l, m;
	int n, s, t;
	CR_COORD_T c0, c1;
	ClassRoute_t cr1;
	int local = 1;
#ifdef __cplusplus
	static int signs[] = { 1, -1 };
#else
	static int signs[] = {
		[CR_SIGN_POS] = 1,
		[CR_SIGN_NEG] = -1,
	};
#endif
	for (n = 0; n < nexclude; ++n) {
		if (__MUSPI_eq_coords(&exclude[n], me)) {
			/* NOT local contributor... */
			local = 0;
			break;
		}
	}
	if (level && local) {
		/* we ARE a local contributor, so no need to go further */
		return 1;
	}
	MUSPI_BuildNodeClassroute(world, worldroot, me, comm, dims, dim0, &cr1);
	c0 = *me;
	l = cr1.input;
	t = 0;
	for (n = 0; n < CR_NUM_DIMS; ++n) {
		for (s = 0; s < 2; ++s) {
			m = CR_LINK(n, s);
			if (l & m) {
				c1 = c0;
				CR_COORD_DIM(&c1,n) += signs[s];
				/* assume no errors! */
				int f = __MUSPI_find_local_contrib(world, worldroot, &c1,
						comm, exclude, nexclude,
						dims, dim0, NULL, level + 1);
				if (!f) cr1.input &= ~m;
				t += f;
			}
		}
	}
	if (!level && local) cr1.input |= BGQ_CLASS_INPUT_LINK_LOCAL;
	if (!cr1.input && !local) cr1.output = 0;
	if (cr) *cr = cr1;
	return t;
}

/**
 * \brief Compute classroute for node 'me' when rectangle is sparse
 *
 * This is merely a wrapper for the recursive routine which actually
 * computes and prunes the classroute.
 *
 * \param[in] world	Entire partition rectangle (COMM_WORLD)
 * \param[in] worldroot	Coordinates of root node in COMM_WORLD
 * \param[in] me	Coordinates of my node
 * \param[in] comm	Rectangle to produce classroute for
 * \param[in] exclude	Array of coords for nodes excluded from comm
 * \param[in] nexclude	Number of coords in 'exclude'
 * \param[in] dims	(optional) Translation/mapping of dimensions
 * \param[in] dim0	Starting dimension (iterate circular increasing)
 * \param[out] cr	Classroute bitmaps
 */
__INLINE__
void MUSPI_BuildNodeClassrouteSparse(CR_RECT_T *world, CR_COORD_T *worldroot, CR_COORD_T *me,
				CR_RECT_T *comm, CR_COORD_T *exclude, int nexclude,
				int *dims, int dim0, ClassRoute_t *cr) {
	/*
	 * traverse down-tree and determine if any local contributors exist.
	 * unfortunately, we don't have any classroutes but our own, so we must
	 * build each down-tree node's classroute on the fly.
	 */
	__MUSPI_find_local_contrib(world, worldroot, me, comm, exclude, nexclude,
								dims, dim0, cr, 0);
}

/**
 * Pick a root for 'world' rectangle
 *
 * Used for sub-comms on exclusive virtual channel.
 *
 * \param[in] world		Entire partition rectangle (COMM_WORLD)
 * \param[in] dims		(optional) dimension mapping
 * \param[out] worldroot	Coordinates of root node in COMM_WORLD
 * \param[out] pri_dim		Primary dimension for classroutes (index into dims[])
 */
__INLINE__
void MUSPI_PickWorldRoot(CR_RECT_T *world, int *dims, CR_COORD_T *worldroot, int *pri_dim) {
	int x, d, p=0;
	CR_COORD_T root;
	for (x = 0; x < CR_NUM_DIMS; ++x) {
		d = dims ? dims[x] : x;
		int size = CR_COORD_DIM(CR_RECT_UR(world),d) - CR_COORD_DIM(CR_RECT_LL(world),d) + 1;
		if (size > 1) p = x; // last non-one dimension should be primary...
		CR_COORD_DIM(&root,d) = CR_COORD_DIM(CR_RECT_LL(world),d) + size / 2;
	}
	*pri_dim = p;
	*worldroot = root;
}

/**
 * Pick a pair of roots for 'world' rectangle that will not conflict
 *
 * Used for comm-world with shared virtual channel (system and user comm-world)
 *
 * \param[in] world		Entire partition rectangle (COMM_WORLD)
 * \param[in] dims		Dimension ordering
 * \param[out] worldroot1	Coordinates of 1st root node in COMM_WORLD
 * \param[out] worldroot2	Coordinates of 2nd root node in COMM_WORLD
 * \param[out] pri_dim		Primary dimension for classroute (index into dims[])
 */
__INLINE__
void MUSPI_PickWorldRootPair(CR_RECT_T *world, int *dims,
		CR_COORD_T *worldroot1, CR_COORD_T *worldroot2, int *pri_dim) {
	int x, d;
	int min = 99999999;
	int min_dim = -1;
	CR_COORD_T root;
	for (x = 0; x < CR_NUM_DIMS; ++x) {
		d = dims ? dims[x] : x;
		int size = CR_COORD_DIM(CR_RECT_UR(world),d) - CR_COORD_DIM(CR_RECT_LL(world),d) + 1;
		CR_COORD_DIM(&root,d) = CR_COORD_DIM(CR_RECT_LL(world),d) + size / 2;
		if (size > 1 && size <= min) {
			min = size;
			min_dim = x;
		}
	}
	// assert(min_dim != -1);
	*pri_dim = min_dim;
	d = dims ? dims[min_dim] : min_dim;
	*worldroot1 = root;
	*worldroot2 = root;
	CR_COORD_DIM(worldroot1,d) = CR_COORD_DIM(CR_RECT_LL(world),d);
	CR_COORD_DIM(worldroot2,d) = CR_COORD_DIM(CR_RECT_UR(world),d);
}

/**
 * \brief Compare two rectangles
 *
 * \param[in] rect1	First rectangle to compare
 * \param[in] rect2	Second rectangle to compare
 * \return	0 = identical, 1 = disjoint, -1 = overlapping
 */
static inline int __MUSPI_rect_compare(CR_RECT_T *rect1, CR_RECT_T *rect2) {
	int d, n = 0, o = 0;
	unsigned r1ll, r2ll, r1ur, r2ur;
	for (d = 0; d < CR_NUM_DIMS; ++d) {
		r1ll = CR_COORD_DIM(CR_RECT_LL(rect1),d);
		r2ll = CR_COORD_DIM(CR_RECT_LL(rect2),d);
		r1ur = CR_COORD_DIM(CR_RECT_UR(rect1),d);
		r2ur = CR_COORD_DIM(CR_RECT_UR(rect2),d);
		if (r1ll == r2ll && r1ur == r2ur) {
			++n;	/* identical */
			++o;	/* also counts as overlapping */
			continue;
		}
		if (r1ur >= r2ll && r1ll <= r2ur) {
			++o;	/* overlapping */
			continue;
		}
	}
	if (n == CR_NUM_DIMS) return 0;		/* identical rectangles */
	if (o == CR_NUM_DIMS) return -1;	/* overlapping */
	return 1; /* disjoint rectangles */
}

/**
 * \brief Initialize the classroute ID environment
 *
 * \param[in] free_ids	Array of classroute ids that are currently free
 * \param[in] nfree	Number of ids in array
 * \param[in] isGI      Indicates whether this is for Global Interrupts
 * \param[in,out] env   opaque memory for tracking classroute id allocations
 * \return      0
 */
__INLINE__
uint32_t MUSPI_InitClassrouteIds(uint32_t *free_ids, uint32_t nfree, int isGI, void **env) {
	struct cr_env *cr_env = (struct cr_env *)*env;
	if (!cr_env) {
		size_t len = sizeof(struct cr_env);
	        cr_env = (struct cr_env *)malloc(len);
		memset(cr_env, 0, len);
		cr_env->isGI = isGI;
		*env = cr_env;
	}
	if (free_ids && nfree > 0) {
		uint32_t x;
		// non-destructively determine "system" (in-use) classroute ids...
		for (x = 0; x < BGQ_COLL_CLASS_MAX_CLASSROUTES; ++x) {
			if (cr_env->cr_alloc[x] == NULL) {
				cr_env->cr_alloc[x] = CR_SYSTEM_ID;
			}
		}
		for (x = 0; x < nfree; ++x) {
			if (cr_env->cr_alloc[free_ids[x]] == CR_SYSTEM_ID) {
				cr_env->cr_alloc[free_ids[x]] = NULL;
			}
		}
		// assert that any ids not in free_ids[] are set to "CR_SYSTEM_ID"...
		// i.e. we must not have used an id that is being declared "not free"...
	}
	return 0;
}

/**
 * \brief Find all possible classroutes for given rectangle
 *
 * Will re-use classroute ID for identical rectangles, if found.
 * Typically, this bitmap is ANDed with all other participants,
 * and the least-significant "1" bit indicates the first classroute
 * that all agree upon.
 *
 * \param[in] vc        Virtual channel to be used by classroute
 * \param[in] subcomm   The rectangle for classroute to be created
 * \param[in,out] env   opaque memory for tracking classroute id allocations
 * \return      bitmap of possible classroutes, or
 *		CR_MATCH_RECT_FLAG OR'ed with single id bit to be re-used.
 */
__INLINE__
uint32_t MUSPI_GetClassrouteIds(int vc, CR_RECT_T *subcomm, void **env) {
	int x;
	struct cr_allocation *crp;
	uint32_t free = 0;

	MUSPI_InitClassrouteIds(NULL, 0, 0, env);
	struct cr_env *cr_env = (struct cr_env *) *env;
	/*
	 * assertion: if we find an identical rectangle that already has a classroute,
	 * then EVERYONE had better see the same thing. We return only one possible
	 * classroute in this case.
	 *
	 * \todo Consider virtual channel in search
	 */
	for (x = 0; x < BGQ_COLL_CLASS_MAX_CLASSROUTES; ++x) {
		if (cr_env->cr_alloc[x] == NULL) {
			free |= (1 << x);
			if (cr_env->isGI) {
				/* additional checks required? */
			}
			continue;
		} else if (cr_env->cr_alloc[x] == CR_SYSTEM_ID) {
			continue;
		}
		/*
		 * since all rectangles in a classroute must be identical,
		 * no need to check them all, just the first.
		 */
		crp = cr_env->cr_alloc[x];
		if (GET_CR_ROUTE_VC(&crp->classroute) != vc) {
			continue;
		}
		int y = __MUSPI_rect_compare(&crp->rect, subcomm);
		if (y == 0) {
			/*
			 * identical rectangle already in use,
			 * return it's classroute
			 */
			return CR_MATCH_RECT_FLAG | (1 << x);
		}
		if (cr_env->isGI) {
			/* additional checks required? */
		}
	}
	/*
	 * no matching rectangle found,
	 * return all unused classroutes
	 */
	return free;
}


/**
 * \brief Allocate a classroute for rectangle
 *
 * Will re-use classroute ID for identical rectangles, or where a
 * classroute has no overlapping rectangles in use at the moment.
 *
 * \param[in] id        Classroute ID
 * \param[in] vc        Virtual channel used by classroute
 * \param[in] subcomm   The rectangle for classroute to be created
 * \param[in,out] env   opaque memory for tracking classroute id allocations
 * \return      0 = re-using existing classroute, 1 = classroute DCRs must be set
 */
__INLINE__
int MUSPI_SetClassrouteId(int id, int vc, CR_RECT_T *subcomm, void **env) {
        struct cr_allocation *new_cr, *crp;

	MUSPI_InitClassrouteIds(NULL, 0, 0, env);
	struct cr_env *cr_env = (struct cr_env *) *env;

	// assert(cr_alloc[id] != CR_SYSTEM_ID);
	new_cr = (struct cr_allocation *) malloc(sizeof(struct cr_allocation));
	new_cr->rect = *subcomm;
	new_cr->classid = id;
	new_cr->classroute.output = 0;
	new_cr->classroute.input = 0;
	SET_CR_ROUTE_VC(&new_cr->classroute,vc);
	crp = cr_env->cr_alloc[id];
	/* all are the same, just insert at front */
	new_cr->cr_peer = crp;
	cr_env->cr_alloc[id] = new_cr;
	return (crp == NULL);
}


/**
 * \brief Release rectangle from use as a classroute
 *
 * Assumes that 'subcomm' previously succeeded when passed to get_classroute_id().
 *
 * \param[in] id        Classroute ID
 * \param[in] vc        Virtual channel used by classroute
 * \param[in] subcomm   The rectangle for classroute to be removed
 * \param[in,out] env   opaque memory for tracking classroute id allocations
 * \return 0 = classroute still in use, 1 = classroute free (DCRs must be cleared)
 */
__INLINE__
int MUSPI_ReleaseClassrouteId(int id, int vc, CR_RECT_T *subcomm, void **env) {
	struct cr_env *cr_env = (struct cr_env *) *env;
	struct cr_allocation *crp;

	// assert(cr_alloc != NULL);
	// assert(cr_alloc[id] != NULL);
	// assert(cr_alloc[id] != CR_SYSTEM_ID);
	crp = cr_env->cr_alloc[id];
	cr_env->cr_alloc[id] = crp->cr_peer;
	free(crp);
	return (cr_env->cr_alloc[id] == NULL);
}

/**
 * \brief Compute the size of a rectangle as dimension sizes in coordinate struct
 *
 * \param[in] c		Coordinate struct which contains dim sizes
 * \return	Size of rectangle
 */
static inline int __MUSPI_coords_size(CR_COORD_T *c) {
	int x = 1;
	int m;
	for (m = 0; m < CR_NUM_DIMS; ++m) { 
		x *= CR_COORD_DIM(c,m);
	}
	return x;
}

/**
 * \brief Compute the size of a rectangle
 *
 * \param[in] r		Rectangle struct
 * \return	Size of rectangle
 */
static inline int __MUSPI_rect_size(CR_RECT_T *r) {
	int x = 1;
	int d;
	for (d = 0; d < CR_NUM_DIMS; ++d) { 
		int i = CR_COORD_DIM(CR_RECT_UR(r),d) - CR_COORD_DIM(CR_RECT_LL(r),d) + 1;
		x *= i;
	}
	return x;
}

/**
 * \brief Recurse through coordinates of a "plane" and exclude 'num' nodes
 *
 * \param[in] comm	Rectangle to work from
 * \param[in] pri_dim	Primary dimension
 * \param[in] curr	Currrent coordinate being worked on
 * \param[in] d		Current dimension (index to dims[]) being worked
 * \param[in] dims	Dimension ordering (mapping)
 * \param[in,out] num	Number of nodes (left) to exclude
 * \param[out] ex	Array to populate with excluded coords
 * \param[out] nx	Number of coords in array
 *
 */
static inline void __MUSPI_exclude_coords(CR_RECT_T *comm, int pri_dim,
		CR_COORD_T *curr, int d, int *dims, int *num, CR_COORD_T *ex, int *nx) {
	int x;
	int dim = dims ? dims[CR_NUM_DIMS - d - 1] : d;
	for (x = CR_COORD_DIM(CR_RECT_UR(comm),dim);
	    *num > 0 && x >= (int)CR_COORD_DIM(CR_RECT_LL(comm),dim); --x) {
		CR_COORD_DIM(curr,dim) = x;
		if (d == CR_NUM_DIMS - 1) {
			ex[*nx] = *curr;
			++(*nx);
			--(*num);
		} else {
			__MUSPI_exclude_coords(comm, pri_dim, curr, d + 1, dims, num, ex, nx);
		}
	}
}

/**
 * \brief Compute a rectangle and primary dimension for making comm-world with -np
 *
 * \todo This may not work for -np 1 or other such cases.
 *
 * \param[in] univ	The entire booted block rectangle
 * \param[in] np	Number of nodes to run job (comm-world)
 * \param[in] dims	Dimension map ordering to use, e.g. Nth dim is CR_DIM_NAMES[dims[n]]
 * \param[out] comm	The comm-world circumscribing rectangle
 * \param[out] co	Coordinates of nodes to exclude from 'comm'
 * \param[out] nx	Number of coords in 'co'
 * \param[out] pri_dim	Primary dimension to use
 */
__INLINE__
void MUSPI_MakeNpRectMap(CR_RECT_T *univ, int np, int *dims,
		CR_RECT_T *comm, CR_COORD_T *co, int *nx, int *pri_dim) {
	CR_COORD_T uz, cz, cc;
	CR_RECT_T r;
	int x, d, n, l, p;

	//p = dims[0];
	n = 1;
	for (x = 0; x < CR_NUM_DIMS; ++x) {
		d = dims[x];
		l = CR_COORD_DIM(CR_RECT_UR(univ),d) - CR_COORD_DIM(CR_RECT_LL(univ),d) + 1;
		CR_COORD_DIM(&uz,d) = n;
		n *= l;
	}
	for (x = CR_NUM_DIMS - 1; x >= 0; --x) {
		d = dims[x];
		l = (np + CR_COORD_DIM(&uz,d) - 1) / CR_COORD_DIM(&uz,d);
		CR_COORD_DIM(&cz,d) = l;
		if (l > 1) {
			for (; x >= 0; --x) {
				d = dims[x];
				CR_COORD_DIM(&cz,d) = CR_COORD_DIM(CR_RECT_UR(univ),d) - CR_COORD_DIM(CR_RECT_LL(univ),d) + 1;
			}
		}
	}

	/*
	 * Convert dimension sizes into an actual rectangle, based at lower-left
	 * corner of 'univ'.
	 */
	p = 0;
	for (x = 0; x < CR_NUM_DIMS; ++x) { 
		d = dims[x];
		n = CR_COORD_DIM(&cz,d);
		if (n > 1) p = d; // last non-one dimension should be primary...
		CR_COORD_DIM(CR_RECT_LL(&r),d) = CR_COORD_DIM(CR_RECT_LL(univ),d);
		CR_COORD_DIM(CR_RECT_UR(&r),d) = n + CR_COORD_DIM(CR_RECT_LL(univ),d) - 1;
	}

	*comm = r;
	*pri_dim = p;

	/*
	 * Recurse through the primary dimension "plane" and remove nodes
	 * until we reach exactly 'np'. The coordinates of removed nodes
	 * are returned in the 'co' array.
	 */
	*nx = 0;
	x = __MUSPI_coords_size(&cz) - np;
	if (x > 0) {
		cc = *CR_RECT_UR(&r);
		__MUSPI_exclude_coords(&r, p, &cc, 0, dims, &x, co, nx);
	}
}

#include <math.h>
/**
 * \brief Compute a rectangle and primary dimension for making comm-world with -np
 *
 * \todo This may not work for -np 1 or other such cases.
 *
 * \todo Need to incorporate dimension ordering?
 *
 * \param[in] univ	The entire booted block rectangle
 * \param[in] np	Number of nodes to run job (comm-world)
 * \param[out] comm	The comm-world circumscribing rectangle
 * \param[out] co	Coordinates of nodes excluded from 'comm' - caller provides storage
 * \param[out] nx	Number of coords in 'co'
 * \param[out] pri_dim	Primary dimension to use
 */
__INLINE__
void MUSPI_MakeNpRect(CR_RECT_T *univ, int np, CR_RECT_T *comm, CR_COORD_T *co, int *nx,
								int *pri_dim) {
	int n, e, f, x, z, m, p, q, y, t;
	CR_RECT_T r;
	CR_COORD_T wz, cz;
	CR_COORD_T cc;
	double d, dx;

	/*
	 * Compute size of each "univ" dimension and save in handy coord struct
	 */
	for (m = 0; m < CR_NUM_DIMS; ++m) {
		n = (CR_COORD_DIM(CR_RECT_UR(univ),m) - CR_COORD_DIM(CR_RECT_LL(univ),m) + 1);
		CR_COORD_DIM(&wz,m) = n;
	}

	/*
	 * We preserve the E dimension in the resulting rectangle because
	 * the links are on-chip and the most efficient for communication.
	 * We then make an attempt to find a regular rectangle in the
	 * remaining (CR_NUM_DIMS - 1) dimensions by taking Nth root.
	 */
	n = CR_COORD_DIM(&wz,CR_NUM_DIMS - 1);
	e = np / n; // divide by 1 or 2
	d = e;
	dx = 1.0 / (CR_NUM_DIMS - 1); /* for taking Nth root */
	d = pow(d, dx);
	f = (int) d;
	/*
	 * Now 'f' has the dimension size of a regular rectangle that is
	 * (likely) smaller than we need. But not all dimensions of the rectangle
	 * may be as large as 'f' so we have to make adjustments. At the same
	 * time we'll try to pick up extra nodes to ensuire our rectangle
	 * circumscribes the required -np size.
	 *
	 * First, build a handy coord set of dimension sizes for the prospective
	 * rectangle, this will also be our working set for adjustments. All
	 * dimensions except E will start with size 'f', and E will be the same
	 * size as in the 'univ' (booted block).
	 */
	z = 1;
	for (m = 0; m < CR_NUM_DIMS; ++m) {
		p = (m == CR_NUM_DIMS - 1 ? n : f);
		CR_COORD_DIM(&cz,m) = p;
		z *= p;
	}
	x = z - np;
	/*
	 * Now, 'x' tells us how many nodes we are short in the prospective
	 * rectangle. We will work through the dimensions seeing how to adjust
	 * the rectangle such that we have at least np nodes (x >= 0).
	 * We also check if our prospective rectangle is too big in a given
	 * dimension, in which case we have to reset it's value and also
	 * update our deficite accordingly. Note, this code makes only one
	 * pass.  It is concievable that more than one pass is required.
	 *
	 * This loop does not process the E dimension, that stays as-is.
	 */
	for (m = 0; m < CR_NUM_DIMS - 1; ++m) {
		cc = cz;
		CR_COORD_DIM(&cc,m) = 1;
		y = __MUSPI_coords_size(&cc); 
		// each +1 to this dim size adds 'y' nodes...

		n = CR_COORD_DIM(&wz,m);
		if (n < f) {
			/*
			 * Prospective dimension too big, reduce to
			 * max value and update deficite with the
			 * number of nodes that represents.
			 */
			q = n;
			x -= (f - n) * y;
		} else if (n > f && x < 0) {
			/*
			 * Prospective dimension less than max, and we
			 * have a deficite. We might be able to pick up
			 * some of these nodes here.
			 */
			t = ((-x + y-1) / y); /* number to incr dim for all */
			if (n <= f + t) {
				/* can't take all the deficite, do as many as we can */
				q = n;
				x += (n - f) * y;
			} else if (-x < y) {
				/* only need +1 (already handled below?) */
				q = (f + 1);
				x += y;
			} else {
				/* take all the deficite */
				q = (f + t);
				x += t * y;
			}
		} else {
			/*
			 * no room for adjustment in this dimension, keep it as-is.
			 */
			q = f;
		}
		/* store the computed dimension size */
		CR_COORD_DIM(&cz,m) = q;
	}
	/*
	 * At this point we should have a rectangle that circumscribes
	 * the number of nodes specified. Next step is to choose a
	 * dimension to be the primary dimensions, which is the
	 * dimension on which the excluded nodes will be.
	 *
	 * This could be smarter about choosing an optimal dimension.
	 * It does not consider the E dimension. This may not work
	 * for the case of -np 1 where no dimensions except E have
	 * any depth.
	 */
	p = 0;
	if (x < 0) {
		// error?
	} else if (x > 0) {
		for (m = 0; m < CR_NUM_DIMS - 1; ++m) {
			if (CR_COORD_DIM(&cz,m) == 1) continue;
			cc = cz;
			CR_COORD_DIM(&cc,m) = 1;
			y = __MUSPI_coords_size(&cc); 
			if (y > x) {
				p = m;
				break;
			}
		}
	}

	/*
	 * Convert dimension sizes into an actual rectangle, based at lower-left
	 * corner of 'univ'.
	 */
	for (m = 0; m < CR_NUM_DIMS; ++m) { 
		n = CR_COORD_DIM(&cz,m);
		CR_COORD_DIM(CR_RECT_LL(&r),m) = CR_COORD_DIM(CR_RECT_LL(univ),m);
		CR_COORD_DIM(CR_RECT_UR(&r),m) = n + CR_COORD_DIM(CR_RECT_LL(univ),m) - 1;
	}

	*comm = r;
	*pri_dim = p;

	/*
	 * Recurse through the primary dimension "plane" and remove nodes
	 * until we reach exactly 'np'. The coordinates of removed nodes
	 * are returned in the 'co' array.
	 */
	*nx = 0;
	if (x > 0) {
		cc = *CR_RECT_UR(&r);
		__MUSPI_exclude_coords(&r, p, &cc, 0, NULL, &x, co, nx);
	}
}

__END_DECLS

#endif /* _MUSPI_CLASSROUTE_INLINES_H_ */
