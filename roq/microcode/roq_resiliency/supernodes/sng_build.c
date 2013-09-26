#include <string.h>

#include "common.h"
#include "sng_common.h"
#include "sng_build.h"
#include "torus.h"

/* Returns the supernode index that contains the node with coordinates 'nc' */
dim_t sn_get(dim_t nc) {
	dim_t si; // supernode index that contains nc
	int   d;  // dimensions
	int   i;  // 
	
	si.raw = 0;
	
	for(d = 0; d < torus.dim; d++) {
		for(i = 0; i < torus.size.dim[d]; i++) {
			
			DPRINTF(SN__GET, "%d <= %d && %d < %d\n", sng.anchor.dim[d][i], nc.dim[d], nc.dim[d], sng.anchor.dim[d][i+1]);
			
			if (sng.anchor.dim[d][i] <= nc.dim[d] && nc.dim[d] < sng.anchor.dim[d][i+1]) {
				si.dim[d] = i;
				break;
			}
		}
	}
	
	return si;
}


/* Returns row/col number of a supernode for a coordinate 'c' in dimension 'd' */
uint8_t sn_getIndex(uint8_t d, uint8_t c) {
	int i;
	
	for(i = 0; i <= sng.num.dim[d]; i++) {
		
		DPRINTF(SN__GET_DIM, "%d: %d <= %d && %d < %d\n", i, sng.anchor.dim[d][i], c, c, sng.anchor.dim[d][i+1]);
		
		if (sng.anchor.dim[d][i] <= c && c < sng.anchor.dim[d][i+1]) {
			DPRINTF(SN__GET_DIM, "--> found at %d\n", i);
			return i;
		}
	}
	
	// coordinate could not be found, so it must be beyond the torus dimensions, which is
	// the case, e.g. for wrap around links.
	return sng.num.dim[d];
}


/* Returns coordinate of a supernode for an index 'i' in dimension 'd' */
inline uint8_t sn_getCoord(uint8_t d, uint8_t i) {
	return sng.anchor.dim[d][i];
}


/* Returns size of a supernode for an index 'i' in dimension 'd' */
inline uint8_t sn_getSize(uint8_t d, uint8_t i) {
	return sng.size.dim[d][i];
}


/* Returns size-1 of a supernode for an index 'i' in dimension 'd' */
inline uint8_t sn_getMax(uint8_t d, uint8_t i) {
	return sng.size.dim[d][i]-1;
}


/* Returns the supernode anchor for supernode number 'n' */
inline dim_t sn_getAnchor(dim_t n) {
	return (dim_t) { sng.anchor.A[n.A],
			         sng.anchor.B[n.B],
			         sng.anchor.C[n.C],
			         sng.anchor.D[n.D],
			         sng.anchor.E[n.E]};
}



/* split along plane 'pc' in dimension 'pd'. 'dir' indicates whether the cut is on the
   plus or on the minus side of failure (used for routing).

   -> returns 1 if supernodes got split
   -> resutns 0 if the split has already been there

   'pc' is max torus.size.dim[pd]-1;
*/
int sn_split(uint8_t pd, uint8_t pc) {
	// skip -1 and alike.
	if (pc >= torus.size.dim[pd]) return 2;

	int i;
	int sn = sn_getIndex(pd, pc+1);
	
	DPRINTF(SN__SPLIT, "Splitting supernodes %s[%d] along plane %s[%d]\n", dim2str[pd], sn, dim2str[pd], pc);
	
	// check for coincidence of cuts
	if (sng.anchor.dim[pd][sn] == pc+1) return 0;
		
	// number of supernodes in this dimension increased by +1
	sng.num.dim[pd]++;
	
	// shift supernodes to make room for the new one.
	for(i = sng.num.dim[pd]; i > sn; i--) {
		sng.anchor.dim[pd][i] = sng.anchor.dim[pd][i-1];
		sng.size.dim[pd][i]   = sng.size.dim[pd][i-1];
	}
	
	// insert new supernode.
	sng.anchor.dim[pd][sn+1] = pc+1;
	sng.size.dim[pd][sn]     = sng.anchor.dim[pd][sn+1] - sng.anchor.dim[pd][sn];
	sng.size.dim[pd][sn+1]   = sng.anchor.dim[pd][sn+2] - sng.anchor.dim[pd][sn+1];
	
	return 1;
}


/*
  Merged supernode graph along the plane number 'pn' in dimension 'pd'. 'prn' is used to
  remove the 'new' flag from the plane in case it hasn't already been removed by the
  routing functions.
*/
void sn_merge(uint8_t pd, uint8_t pi, uint8_t pc) {
	int i;

	// number of supernodes in this dimension decreased by 1
	sng.num.dim[pd]--;
	
	// adjust size of row/col 'n'
	sng.size.dim[pd][pi] += sng.size.dim[pd][pi+1];
	
	// remove supernode, compact list
	for(i = pi+1; i <= sng.num.dim[pd]; i++) {
		sng.anchor.dim[pd][i] = sng.anchor.dim[pd][i+1];
		sng.size.dim[pd][i]   = sng.size.dim[pd][i+1];
	}
}


/* Checks if all faces of the block defined by the region 'box are closed, i.e. that none
   of the nodes inside the block are connected to the rest of the network by a single
   working link.

   -> It assumes that 'box' is in supernode coordinates, not raw coordinates.
 */
int sn_mergeHRCheckSurface(region_t *box) {
	region_t b;
	b.lo = sn_get(box->lo);
	b.hi = sn_get(box->hi);
	
	DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Checking faces of block [%d,%d,%d,%d,%d]-[%d,%d,%d,%d,%d] = (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d) for closure...\n",
			b.lo.A, b.lo.B, b.lo.C, b.lo.D, b.lo.E,
			b.hi.A, b.hi.B, b.hi.C, b.hi.D, b.hi.E,
			box->lo.A, box->lo.B, box->lo.C, box->lo.D, box->lo.E,
			box->hi.A, box->hi.B, box->hi.C, box->hi.D, box->hi.E);

	uint8_t d;      // dimensions
	
	dim_t   f[32];  // location of failures inside the block
	uint8_t fc = 0; // failure counter
	
	for (d = 0; d < torus.dim; d++) {
		uint8_t l = mod(sng.anchor.dim[d][b.lo.dim[d]]   - 1, torus.size.dim[d]); // position of the left failed links on the axies
		uint8_t r = mod(sng.anchor.dim[d][b.hi.dim[d]+1] - 1, torus.size.dim[d]); // position of the right failed links on the axies
		
		uint8_t sc[4], rc[4]; // supernode coordinates, raw/real coordinates;
		
		uint32_t mask = (1 << l) | (1 << r);
		
		DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Mask for all %s axes: %08x (%d,%d)\n", dim2str[d], (unsigned int) mask, l, r);
		
		// make inner node coordinate out of link 'l'. Needed for removable failure detection
		l = (l+1) % torus.size.dim[d];
		
		// NOTE: This all happens on supernode level
		// check if all axes that cross the block candidate match this mask
		sc[0] = b.lo.dim[c[d].dim[1]];
		do {
			rc[0] = sng.anchor.dim[c[d].dim[1]][sc[0]];
			sc[1] = b.lo.dim[c[d].dim[2]];
			do {
				rc[1] = sng.anchor.dim[c[d].dim[2]][sc[1]];
				sc[2] = b.lo.dim[c[d].dim[3]];
				do {
					rc[2] = sng.anchor.dim[c[d].dim[3]][sc[2]];
					sc[3] = b.lo.dim[c[d].dim[4]];
					do {
						rc[3] = sng.anchor.dim[c[d].dim[4]][sc[3]];
					
						uint32_t resAnd = sng.axes.dim[d][rc[0]][rc[1]][rc[2]][rc[3]] & mask;
						uint32_t resXor = sng.axes.dim[d][rc[0]][rc[1]][rc[2]][rc[3]] ^ mask;
						
						DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Checking axis %s[%d,%d,%d,%d] = %s(%d,%d,%d,%d) = %08x/%08x\n",
								dim2str[d], sc[0], sc[1], sc[2], sc[3],
								dim2str[d], rc[0], rc[1], rc[2], rc[3],
								(unsigned int) resAnd, (unsigned int) resXor);
					
						if (mask != resAnd) {
							DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Axis %s(%d,%d,%d,%d) = %08x/%08x not closed --> No block here.\n",
									dim2str[d], rc[0], rc[1], rc[2], rc[3], (unsigned int) resAnd, (unsigned int) resXor);
							return 0;
						}
						
						// detect failures inside the block.	  
						uint8_t  i;
						uint32_t m = 0; // mask to mask out already processed links on the axis
						
						for (i = __builtin_ffs(resXor); i != 0; i = __builtin_ffs(resXor ^ m)) {
							i--;
							m |= 1 << i;
							DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "l = %d, r = %d, i = %d\n", l, r, i);
							
							if (((l <= i && i < r) && (l < r)) ||  // normal boxes
								((l <= i || i < r) && (r < l))) {  // wrap-around boxes
								uint8_t tc[4]; // temporary coordinate (offset to raw/real coordinates)
								// remove all faults on the side of the supernode where the noticed failure is - all other links will be failures as well.
								for (tc[0] = 0; tc[0] < sng.size.dim[c[d].dim[1]][sc[0]]; tc[0]++) {
									for (tc[1] = 0; tc[1] < sng.size.dim[c[d].dim[2]][sc[1]]; tc[1]++) {
										for (tc[2] = 0; tc[2] < sng.size.dim[c[d].dim[3]][sc[2]]; tc[2]++) {
											for (tc[3] = 0; tc[3] < sng.size.dim[c[d].dim[4]][sc[3]]; tc[3]++) {
										
												// make link coordinates (2a, 2b, 2c, 2d).
												f[fc].dim[d]           = (i << 1) + 1;
												f[fc].dim[c[d].dim[1]] = (rc[0]+tc[0]) << 1;
												f[fc].dim[c[d].dim[2]] = (rc[1]+tc[1]) << 1;
												f[fc].dim[c[d].dim[3]] = (rc[2]+tc[2]) << 1;
												f[fc].dim[c[d].dim[4]] = (rc[3]+tc[3]) << 1;
												DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Failure at (%d,%d,%d,%d,%d) can be ignored\n", f[fc].A, f[fc].B, f[fc].C, f[fc].D, f[fc].E);
												
												fc++;
											}
										}
									}
								}
							}
						}
					} while (sc[3] != b.hi.dim[c[d].dim[4]] && ((sc[3] = (sc[3]+1) % sng.num.dim[c[d].dim[4]]) || 1)); // || 1 in case the sc[3] becomes 0
				} while (sc[2] != b.hi.dim[c[d].dim[3]] && ((sc[2] = (sc[2]+1) % sng.num.dim[c[d].dim[3]]) || 1)); // || 1 in case the sc[2] becomes 0
			} while (sc[1] != b.hi.dim[c[d].dim[2]] && ((sc[1] = (sc[1]+1) % sng.num.dim[c[d].dim[2]]) || 1)); // || 1 in case the sc[1] becomes 0
		} while (sc[0] != b.hi.dim[c[d].dim[1]] && (sc[0] = ((sc[0]+1) % sng.num.dim[c[d].dim[1]]) || 1)); // || 1 in case the sc[0] becomes 0
	}
	
	DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Block [%d,%d,%d,%d,%d]-[%d,%d,%d,%d,%d] = (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d) is closed --> Ignoring %d Failures\n",
			b.lo.A, b.lo.B, b.lo.C, b.lo.D, b.lo.E, 
			b.hi.A, b.hi.B, b.hi.C, b.hi.D, b.hi.E, 
			box->lo.A, box->lo.B, box->lo.C, box->lo.D, box->lo.E,
			box->hi.A, box->hi.B, box->hi.C, box->hi.D, box->hi.E,
			fc);
	
	// remove failures that can be ignored.
	uint8_t i;
	
	for(i = 0; i < fc; i++) {
		uint8_t d = lnk_getDimension(f[i]);
		dim_t   n = lnk_getSrcNode(f[i]);
		
		DPRINTF(SN__MERGE_BLOCK_FACE_CHECK, "Failure on axis %s(%d,%d,%d,%d) = %08x will be removed (%08x)\n",
				dim2str[d], n.dim[c[d].dim[1]], n.dim[c[d].dim[2]], n.dim[c[d].dim[3]], n.dim[c[d].dim[4]],
				(unsigned int) sng.axes.dim[d] [n.dim[c[d].dim[1]]] [n.dim[c[d].dim[2]]] [n.dim[c[d].dim[3]]] [n.dim[c[d].dim[4]]], (1 << n.dim[d]));
		
		sng.axes.dim[d] [n.dim[c[d].dim[1]]] [n.dim[c[d].dim[2]]] [n.dim[c[d].dim[3]]] [n.dim[c[d].dim[4]]] &= ~(1 << n.dim[d]);
		sng.plane.dim[d][n.dim[d]]--;
		
	}
	
	for(i = 0; i < fc; i++) {
		// Try to merge planes were the fault had been.
		// 1. All planes parallel to the fault (which is what sn_merge does)
		sn_mergeCheckAll(f[i]);
		
		// 2. The one plane orthogonal to the link.
		// get link dimension
		uint8_t d  = lnk_getDimension(f[i]);
		dim_t   n  = lnk_getSrcNode(f[i]);
		dim_t   sn = sn_get(n);
		
		sn_mergeCheck(d, sn.dim[d], f[i]);
	}
	
	return 1;
}


/*
  returns 1 if the supernode 'ni' is contained within the region 'box'.
*/
int sng_contains(region_t *box, dim_t ni) {
  if ((((box->lo.A <= box->hi.A) && (box->lo.A <= ni.A && ni.A <= box->hi.A)) || ((box->lo.A > box->hi.A) && (box->lo.A <= ni.A || ni.A <= box->hi.A))) &&
      (((box->lo.B <= box->hi.B) && (box->lo.B <= ni.B && ni.B <= box->hi.B)) || ((box->lo.B > box->hi.B) && (box->lo.B <= ni.B || ni.B <= box->hi.B))) &&
      (((box->lo.C <= box->hi.C) && (box->lo.C <= ni.C && ni.C <= box->hi.C)) || ((box->lo.C > box->hi.C) && (box->lo.C <= ni.C || ni.C <= box->hi.C))) &&
      (((box->lo.D <= box->hi.D) && (box->lo.D <= ni.D && ni.D <= box->hi.D)) || ((box->lo.D > box->hi.D) && (box->lo.D <= ni.D || ni.D <= box->hi.D))) &&
      (((box->lo.E <= box->hi.E) && (box->lo.E <= ni.E && ni.E <= box->hi.E)) || ((box->lo.E > box->hi.E) && (box->lo.E <= ni.E || ni.E <= box->hi.E))))
    return 1;
  return 0;
}


/* Detects completely unreachable blocks. Link 'l' is a link on any edge of such a
   block. If a block has been detected and failures inside that block exist, then these
   failures can be ignored. It then calls sn_mergeCheck to check if the supernode graph
   can be merged along the 'pn'th plane dimension 'pd' with the to-be-ignored failures
   removed.

   -> returns 0 if no block was found
   -> returns 1 if a block was found
 */
int sn_mergeHRDetect(dim_t l) {
#ifdef SNG_DEBUG
	sng_print();
#endif
	/*
            B0     B1     B2     B3      - Link a-b was the last to appear. It closes the block, which is
            |      |      |      |         now completely unreachable.
     A3  --[.]----[.]----[.]----[.]--    - To detect that, the algorithm starts at Axis B2 (where a-b is
            |                    |         on) and compares it to its neighbors.
            |                    |         - B[23] = B2 & B3 = 00000 --> less than two 1's -> no edge.
     A2  --[.]    [.]----[.]    [.]--      - B[12] = B1 & B2 = 01010 --> two possible edges detected.
            |      |      |      |         - B[01] = B0 & B1 = 00000 --> no edge, block ends here.
            |      |      |      |       
     A1  --[.]    [.]----[a]    [.]--    
            |                    |       
            |                    |       
     A0  --[.]----[.]----[b]----[.]--    
            |      |      |      |       
	*/

	DPRINTF(SN__MERGE_BLOCK_DETECTION, "Checking for block edge on link (%d,%d,%d,%d,%d)\n", l.A, l.B, l.C, l.D, l.E);
	// 0. Preparations
	
	// dimension of link 'l'
	uint8_t ld = lnk_getDimension(l);
	
	// link -> node coordinates
	dim_t n = lnk_getSrcNode(l);
	
	// first dimension axis 'fa'
	uint32_t fa = sng.axes.dim[ld][n.dim[c[ld].dim[1]]][n.dim[c[ld].dim[2]]][n.dim[c[ld].dim[3]]][n.dim[c[ld].dim[4]]];
	
	// Quick check to sort out hopeless cases
#ifdef ROQ_RESILIENCY_BGQ
	if (! has2ormore1s32(fa)) {
#else
	if (__builtin_popcount(fa) < 2) {
#endif
		DPRINTF(SN__MERGE_BLOCK_DETECTION, "Not enough failures on axis %s(%d,%d,%d,%d) to be a block candidate.\n",
				dim2str[c[ld].dim[0]], n.dim[c[ld].dim[1]], n.dim[c[ld].dim[2]], n.dim[c[ld].dim[3]], n.dim[c[ld].dim[4]]);
		return 0;
	}

	uint32_t dm[5]; // dimension mask
	bzero(&dm, sizeof(dm));
	dm[ld] = 1 << n.dim[ld]; // mask out starting point - it's the reference with which I'll
	                         // compare. Don't need to compare it to itself.

	DPRINTF(SN__MERGE_BLOCK_DETECTION, "coordinate order (%d,%d,%d,%d,%d)\n", c[ld].dim[0], c[ld].dim[1], c[ld].dim[2], c[ld].dim[3], c[ld].dim[4]);
	DPRINTF(SN__MERGE_BLOCK_DETECTION, "1. dimension axis : %s(%d,%d,%d,%d)\n", dim2str[c[ld].dim[0]], n.dim[c[ld].dim[1]], n.dim[c[ld].dim[2]], n.dim[c[ld].dim[3]], n.dim[c[ld].dim[4]]);
#if __BYTE_ORDER__ == __LITTLE_ENDIAN
	DPRINTF(SN__MERGE_BLOCK_DETECTION, "1. dimension edges: %08x (%d candidates)\n", (unsigned int) fa, __builtin_popcount(fa));
#else
#ifdef ROQ_RESILIENCY_BGQ
#ifdef SNG_DEBUG
	if (has2ormore1s32(fa)) {
		DPRINTF(SN__MERGE_BLOCK_DETECTION, "1. dimension edges: %08x (>=2 candidates)\n", (unsigned int) fa);
	} else {
		DPRINTF(SN__MERGE_BLOCK_DETECTION, "1. dimension edges: %08x (<1 candidates)\n", (unsigned int) fa);
	}
#endif
#endif
#endif

	uint8_t d[5] = {ld, c[ld].dim[1], c[ld].dim[2], c[ld].dim[3], c[ld].dim[4]};
  
	void sn_mergeHRDetectCorners(region_t *box, uint8_t i) {
		if (d[i] == torus.dim) {
			if (sng_contains(box, sng.this.coord)) {
				DPRINTF(SN__MERGE_BLOCK_DETECTION, "> Box (%d,%d,%d,%d,%d) - (%d,%d,%d,%d,%d) contains source supernode (%d,%d,%d,%d,%d).\n",
						box->lo.A, box->lo.B, box->lo.C, box->lo.D, box->lo.E,
						box->hi.A, box->hi.B, box->hi.C, box->hi.D, box->hi.E,
						sng.this.coord.A, sng.this.coord.B, sng.this.coord.C, sng.this.coord.D, sng.this.coord.E);
			} else {
				DPRINTF(SN__MERGE_BLOCK_DETECTION, "> Box (%d,%d,%d,%d,%d) - (%d,%d,%d,%d,%d)\n",
						box->lo.A, box->lo.B, box->lo.C, box->lo.D, box->lo.E,
						box->hi.A, box->hi.B, box->hi.C, box->hi.D, box->hi.E);
				
				// Check if the block is closed.
				sn_mergeHRCheckSurface(box);
			}
			return;
		}
		
		//    DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> Checking dimension %s\n", dim2str[d[i]]);
		
		dim_t al = n;   // lower axis
		al.dim[d[0]] = box->lo.dim[d[0]];
		
		dim_t ah = n;   // higher axis
		ah.dim[d[0]] = box->hi.dim[d[0]];
		
		uint32_t axis = axis_get(d[i], al) & axis_get(d[i], ah);
		
#ifdef ROQ_RESILIENCY_BGQ
		if (! has2ormore1s32(fa)) {
#else
		if (__builtin_popcount(axis) < 2) {
#endif
			DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> NO Box edge at %s[%d] and %s[%d] ",
					dim2str[d[0]], box->lo.dim[d[0]], dim2str[d[0]], box->hi.dim[d[0]]); 
			PRINTF(SN__MERGE_BLOCK_DETECTION, "--> Axis %s[%d][%d][%d][%d] = %08x, %s[%d][%d][%d][%d] = %08x\n",
				   dim2str[d[i]],
				   al.dim[c[d[i]].dim[1]], al.dim[c[d[i]].dim[2]], al.dim[c[d[i]].dim[3]], al.dim[c[d[i]].dim[4]],
				   sng.axes.dim[d[i]] [al.dim[c[d[i]].dim[1]]] [al.dim[c[d[i]].dim[2]]] [al.dim[c[d[i]].dim[3]]] [al.dim[c[d[i]].dim[4]]],
				   dim2str[d[i]],
				   ah.dim[c[d[i]].dim[1]], ah.dim[c[d[i]].dim[2]], ah.dim[c[d[i]].dim[3]], ah.dim[c[d[i]].dim[4]],
				   sng.axes.dim[d[i]] [ah.dim[c[d[i]].dim[1]]] [ah.dim[c[d[i]].dim[2]]] [ah.dim[c[d[i]].dim[3]]] [ah.dim[c[d[i]].dim[4]]]);
			
			return;
		}
		
		pair8_t nodes;
		// masks for upper and lower half of the axis. Divisor is at n.dim[i].
		uint32_t ml, mh; // mask for lower and for the higher half.
		uint32_t mlt[2], mht[2]; // mask templates
		
		// template for inner box
		mlt[1] = ~((1 << n.dim[d[i]])-1);
		mht[1] =   (1 << n.dim[d[i]])-1;
		
		// template for outer box
		mlt[0] =   (1 << (n.dim[d[i]]+1))-1;
		mht[0] = ~((1 << (n.dim[d[i]]+1))-1);
		
		int k; // 1 = inner, 0 = outer box
		for(k = 2; k-- != 0;) {
			ml = mlt[k];
			nodes.one = axis_getNextOne(axis, &ml);
			
			while(nodes.one != (uint8_t)-1) {
				mh = mht[k];
				nodes.two = axis_getNextOne(axis, &mh);
				
				while(nodes.two != (uint8_t)-1) {
					
					box->lo.dim[d[i]] = (nodes.one+1)%torus.size.dim[d[i]];
					box->hi.dim[d[i]] = nodes.two;
					
					// Check if these coordinates in this dimension would work for the first
					// dimension. If not, we can skip this combination and proceed to the next one.
					dim_t al = n;   // lower axis
					al.dim[d[i]] = box->lo.dim[d[i]];
					
					dim_t ah = n;   // higher axis
					ah.dim[d[i]] = box->hi.dim[d[i]];
					
					// this here checks if the other axis in the first dimension (the one that was
					// found in this dimension) also has the same edge as the first axis in the first
					// dimension.
					uint32_t fa = (sng.axes.dim[d[0]] [al.dim[d[1]]] [al.dim[d[2]]] [al.dim[d[3]]] [al.dim[d[4]]] & // first dimension axis
								   sng.axes.dim[d[0]] [ah.dim[d[1]]] [ah.dim[d[2]]] [ah.dim[d[3]]] [ah.dim[d[4]]]);
					uint32_t fm = ((1 << mod((box->lo.dim[d[0]]-1), torus.size.dim[d[0]])) |         // first dimension axis mask
								   (1 << box->hi.dim[d[0]]));
					
					if ((fa & fm) == fm) {
						DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> Box edge at %s[%d] and %s[%d] ",
								dim2str[d[i]], box->lo.dim[d[i]], dim2str[d[i]], box->hi.dim[d[i]]); 
						PRINTF(SN__MERGE_BLOCK_DETECTION, "--> Axis %s[%d][%d][%d][%d] = %08x, %s[%d][%d][%d][%d] = %08x, mask = %08x\n",
							   dim2str[d[0]], al.dim[d[1]], al.dim[d[2]], al.dim[d[3]], sng.axes.dim[d[0]] [al.dim[d[1]]] [al.dim[d[2]]] [al.dim[d[3]]] [al.dim[d[4]]],
							   dim2str[d[0]], ah.dim[d[1]], ah.dim[d[2]], ah.dim[d[3]], sng.axes.dim[d[0]] [ah.dim[d[1]]] [ah.dim[d[2]]] [ah.dim[d[3]]] [ah.dim[d[4]]], 
							   fm);
						sn_mergeHRDetectCorners(box, i-1);
					} else {
						DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> NO Box edge at %s[%d] and %s[%d] ",
								dim2str[d[i]], box->lo.dim[d[i]], dim2str[d[i]], box->hi.dim[d[i]]); 
						PRINTF(SN__MERGE_BLOCK_DETECTION, "--> Axis %s[%d][%d][%d][%d] = %08x, %s[%d][%d][%d][%d] = %08x, mask = %08x\n",
							   dim2str[d[0]], al.dim[d[1]], al.dim[d[2]], al.dim[d[3]], sng.axes.dim[d[0]] [al.dim[d[1]]] [al.dim[d[2]]] [al.dim[d[3]]] [al.dim[d[4]]],
							   dim2str[d[0]], ah.dim[d[1]], ah.dim[d[2]], ah.dim[d[3]], sng.axes.dim[d[0]] [ah.dim[d[1]]] [ah.dim[d[2]]] [ah.dim[d[3]]] [ah.dim[d[4]]], 
							   fm);
					}
					nodes.two = axis_getNextOne(axis, &mh);
				}
				nodes.one = axis_getNextOne(axis, &ml);
			}
		}
	}
		
	pair8_t nodes = (pair8_t) {0,0};
		
	region_t box;
	bzero(&box, sizeof(box));
	
	// in the first dimension, one point is fixed, as I already know, that this point is on
	// the surface of the block.
	nodes.one = n.dim[ld];                    // fixed
	nodes.two = axis_getNextOne(fa, &dm[ld]); // variable
	
	while (nodes.two != (uint8_t)-1) {
		
		box.lo.dim[d[0]] = (nodes.one+1)%torus.size.dim[d[0]];
		box.hi.dim[d[0]] = nodes.two;
		DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> Box edge at %s[%d] and %s[%d]\n",
				dim2str[d[0]], box.lo.dim[d[0]], dim2str[d[0]], box.hi.dim[d[0]]); 
		sn_mergeHRDetectCorners(&box, torus.dim-1);
		
		box.lo.dim[d[0]] = (nodes.two+1)%torus.size.dim[d[0]];
		box.hi.dim[d[0]] = nodes.one;
		DPRINTF(SN__MERGE_BLOCK_DETECTION, "--> Box edge at %s[%d] and %s[%d]\n",
				dim2str[d[0]], box.lo.dim[d[0]], dim2str[d[0]], box.hi.dim[d[0]]); 
		sn_mergeHRDetectCorners(&box, torus.dim-1);  
		
		nodes.two = axis_getNextOne(fa, &dm[d[0]]);
	}
	
	return 0;
}
	
/* Checks if the supernode graph can be merged along the 'pi'th plane in dimension
   'pd'. Check will start at link 'l' but will eventually check everything, if
   necessary. Coordiantes are supernode coordinates, not raw coordinates. */

// BUG SOMEWHERE HERE! IT SEEMS AS IF IT DOESN'T CHECK ALL AXES FOR EQUALITY.
int sn_mergeCheck(uint8_t pd, uint8_t pi, dim_t l) {
  /* In a 3-D torus (it's analogoous for 4-D) there are 3x32 planes. Each plane covers 2
     (3-1) dimensions. A supernode graph can only be merged along a plane.

     A plane is basically a cut through the torus through all orthogonal links that share
     one coordinate - the plane coordinate as shown for 2-D in the example below.

           AP0    AP1    AP2    AP3      Planes AP0-AP3 are planes in the A-Dimension.
	    |	   | 	  | 	 |       They cover all links in the A dimension that share
	    v 	   v 	  v 	 v       the same A coordinate.
        B0     B1     B2     B3
        |      |      |      |           e.g. Plane AP0 covers all links L[0][b], b = 0..3
     --[.]----[a]----[d]----[.]--        in a 3-D torus this would be L[0][b][c], b,c = 0..3
        |                    |       
        |                    |           If one wants to merge supernodes SN[1][b] and
     --[.]----[b]----[c]----[.]--        SN[2][b], one would have to do the follwing checks:
        |      |      |      |           1. Plane AP1 must be fault free:
        |      |      |      |              --> sng.plane.A[1] == 0
     --[.]----[.]----[.]----[.]--        2. All pairs of axes B1 == B2:
        |      |      |      |              --> sng.axes.B[0][0][1] == sng.axes.B[0][0][2]
        |      |      |      |              --> sng.axes.B[D][C][1] == snd.axes.B[D][C][2]
     --[.]----[.]----[.]----[.]--               for all D and C.
        |      |      |      |   

  */

	uint8_t pc = sn_getCoord(pd, pi+1)-1; // plane raw number, not supernode number.
	
	DPRINTF(SN__MERGE_CHECK, "\n");
	DPRINTF(SN__MERGE_CHECK, "Checking if I can merge along plane %s[%d] (sng %s[%d])\n", dim2str[pd], pc, dim2str[pd], pi);
	
	// 1. Check: Is the plane ok?
	if (sng.plane.dim[pd][pc] != 0) {
		DPRINTF(SN__MERGE_CHECK, "Plane %s[%d] contains %d!=0 failures --> Cannot Merge\n",
				dim2str[pd],pc,sng.plane.dim[pd][pc]);
		return -1;
	} else {
		DPRINTF(SN__MERGE_CHECK, "Plane %s[%d] contains %d failures --> Merge Candidate\n",
				dim2str[pd],pc,sng.plane.dim[pd][pc]);
	}
	
	// 2. Check: Are all axes ok? To optimize this, start with the recently injected
	//           failure. This should sort out most unmergable planes quickly.
	
	// dimension of link 'l'
	uint8_t ld = lnk_getDimension(l);
	// link -> node coordinates
	dim_t n = lnk_getSrcNode(l);
	
	DPRINTF(SN__MERGE_CHECK, "dimension order (%d,%d,%d,%d,%d)\n", c[ld].dim[0], c[ld].dim[1], c[ld].dim[2], c[ld].dim[3], c[ld].dim[4]);
	
	dim_t a0c = n;  // coordinates of the axes parallel to the plane.
	a0c.dim[pd] = pc;
	
	dim_t a1c = n;  // coordinates of the axes parallel to the plane.
	a1c.dim[pd] = pc+1;
	
	// axes left and right of the plane.
	uint32_t a0 = sng.axes.dim[c[ld].dim[0]][a0c.dim[c[ld].dim[1]]][a0c.dim[c[ld].dim[2]]][a0c.dim[c[ld].dim[3]]][a0c.dim[c[ld].dim[4]]];
	uint32_t a1 = sng.axes.dim[c[ld].dim[0]][a1c.dim[c[ld].dim[1]]][a1c.dim[c[ld].dim[2]]][a1c.dim[c[ld].dim[3]]][a1c.dim[c[ld].dim[4]]];

	DPRINTF(SN__MERGE_CHECK, "Link (%d,%d,%d,%d,%d) is on the axis (%d,%d,%d,%d,%d) on dimension %d with status %08x\n",
			l.A, l.B, l.C, l.D, l.E,
			n.A, n.B, n.C, n.D, n.E,
			ld,
			(unsigned int) a0);

	if (a0 != a1) {
		DPRINTF(SN__MERGE_CHECK, "Quick checking axes %s[%d][%d][%d][%d] == %s[%d][%d][%d][%d]: %08x != %08x --> Cannot Merge\n", 
				dim2str[c[ld].dim[0]], a0c.dim[c[ld].dim[1]], a0c.dim[c[ld].dim[2]], a0c.dim[c[ld].dim[3]], a0c.dim[c[ld].dim[4]],
				dim2str[c[ld].dim[0]], a1c.dim[c[ld].dim[1]], a1c.dim[c[ld].dim[2]], a1c.dim[c[ld].dim[3]], a1c.dim[c[ld].dim[4]],
				(unsigned int) a0, (unsigned int) a1);
		return -1; 
	} else {
		DPRINTF(SN__MERGE_CHECK, "Quick checking axes %s[%d][%d][%d][%d] == %s[%d][%d][%d][%d]: %08x == %08x --> Merge Candidate\n", 
				dim2str[c[ld].dim[0]], a0c.dim[c[ld].dim[1]], a0c.dim[c[ld].dim[2]], a0c.dim[c[ld].dim[3]], a0c.dim[c[ld].dim[4]],
				dim2str[c[ld].dim[0]], a1c.dim[c[ld].dim[1]], a1c.dim[c[ld].dim[2]], a1c.dim[c[ld].dim[3]], a1c.dim[c[ld].dim[4]],
				(unsigned int) a0, (unsigned int) a1);
	}
	
	// 3. Check: All axes parallel to the plane must be pair-wise equal on both sides of the
	//           plane. TODO: This can possibly be optimized by also summing up axes into
	//           planes or something like that.
	
	dim_t snc, rc0, rc1; // supernode coordinate, real coordinate for axis 0 and 1
  
	uint8_t c0;
	for (c0 = 0; c0 < torus.dim; c0++) { // iterate through all dimensions A-D
		if (c0 == pd) continue; // skip plane dimension.

		uint8_t c1 = c[c0].dim[1];
		uint8_t c2 = c[c0].dim[2];
		uint8_t c3 = c[c0].dim[3];
		uint8_t c4 = c[c0].dim[4];

		// iterate through all dimensions, except for the dimension of the plane. There we
		// only compare 2 coordiantes, left and right of the plane. We compute these in the
		// center.
		for(snc.dim[c1] = 0; snc.dim[c1] < (c1 == pd ? 1 : sng.num.dim[c1]); snc.dim[c1]++) {
			rc0.dim[c1] = sng.anchor.dim[c1][snc.dim[c1]];
			for(snc.dim[c2] = 0; snc.dim[c2] < (c2 == pd ? 1 : sng.num.dim[c2]); snc.dim[c2]++) {
				rc0.dim[c2] = sng.anchor.dim[c2][snc.dim[c2]];
				for(snc.dim[c3] = 0; snc.dim[c3] < (c3 == pd ? 1 : sng.num.dim[c3]); snc.dim[c3]++) {
					rc0.dim[c3] = sng.anchor.dim[c3][snc.dim[c3]];
					for(snc.dim[c4] = 0; snc.dim[c4] < (c4 == pd ? 1 : sng.num.dim[c4]); snc.dim[c4]++) {
						rc0.dim[c4] = sng.anchor.dim[c4][snc.dim[c4]];
					
						rc1 = rc0;

						// compare left/right of off plane + convert plane from supernode to real coordinates
						rc0.dim[pd] = pc;
						rc1.dim[pd] = pc+1;

						a0 = sng.axes.dim[c0][rc0.dim[c1]][rc0.dim[c2]][rc0.dim[c3]][rc0.dim[c4]];
						a1 = sng.axes.dim[c0][rc1.dim[c1]][rc1.dim[c2]][rc1.dim[c3]][rc1.dim[c4]];

						if (a0 == a1) {
							DPRINTF(SN__MERGE_CHECK, "Checking axes %s[%d][%d][%d][%d] == %s[%d][%d][%d][%d]: %08x == %08x --> Merge Candidate\n", 
									dim2str[c0], rc0.dim[c1], rc0.dim[c2], rc0.dim[c3], rc0.dim[c4],
									dim2str[c0], rc1.dim[c1], rc1.dim[c2], rc1.dim[c3], rc1.dim[c3],
									(unsigned int) a0, (unsigned int) a1);
						} else {
							DPRINTF(SN__MERGE_CHECK, "Checking axes %s[%d][%d][%d][%d] == %s[%d][%d][%d][%d]: %08x != %08x --> Cannot Merge\n", 
									dim2str[c0], rc0.dim[c1], rc0.dim[c2], rc0.dim[c3], rc0.dim[c4],
									dim2str[c0], rc1.dim[c1], rc1.dim[c2], rc1.dim[c3], rc1.dim[c4],
									(unsigned int) a0, (unsigned int) a1);
							return -1;
						}
					}
				}
			}
		}
	}


	// If we made it until this point, we can finally merge!
	
	DPRINTF(SN__MERGE_CHECK, "All tests passed --> Merging Plane %s[%d]\n", dim2str[pd], pc);
	
	sn_merge(pd, pi, pc);
	
	return 0;
}


/* Checks if the newly introduced fault on link 'l' opens up opportunities to merge
   supernodes.

   TODO: Possible performance improvement: Integrate merge into sn_split because nodes
         that just got split are no merging candidate from the beginning, so most of
	 the merge checks could be safed. Aactually, merging is probably only possible
	 if the split has already been there.
 */
int sn_mergeCheckAll(dim_t l) {
  /* A fault may open up the opportunity to merge supernodes parallel to the link.

        A0     A1     A2     A3
        |      |      |      |   
     --[.]----[a]----[d]----[.]--    Assume link d-c just failed. This opens up the
        |                    |       opportunity to merge column A1 and A2 due to
        |                    |       the already existing fault on link a-b.
     --[.]----[b]----[c]----[.]--    
        |      |      |      |       All parallel supernodes to link d-c have to be
        |      |      |      |       checked for mergability, i.e. A1-A2, A2-A3 and
     --[.]----[.]----[.]----[.]--    also all other dimensions?
        |      |      |      |   
        |      |      |      |   
     --[.]----[.]----[.]----[.]--
        |      |      |      |   

   */

  // dimension of link 'l'
	uint8_t d = lnk_getDimension(l);
	DPRINTF(SN__MERGE, "link (%d,%d,%d,%d,%d), (%010x) is on dimension %d\n", l.A, l.B, l.C, l.D, l.E, l.raw, d);
	
	// source node of link 'l'
	dim_t n = lnk_getSrcNode(l);

	// get supernode of node 'n'
	dim_t sn = sn_get(n);
	DPRINTF(SN__MERGE, "supernode = [%d,%d,%d,%d,%d]\n", sn.A, sn.B, sn.C, sn.D, sn.E);

	// 0/A-link -> Check planes B,C,D for merging
	// 1/B-link -> Check planes A,C,D for merging
	// 2/C-link -> Check planes A,B,D for merging
	// 3/D-link -> Check planes A,B,C for merging
	
	// Check for totally unreachable blocks, i.e. a set of nodes that is not connected by
	// working links to the set of nodes where this node is part of. In case it finds such
	// blocks, it will remove the failures inside so that these blocks can be merged by the
	// succeeding sn_mergeChecks.
	// TODO: don't cann this every time. Find a good criteria, e.g.
	// - amout of total failures in the system
	// - amount of new failures within the last few seconds.
	// - state of neighboring links.
	
	// NOTE: Merging always merges the supernode with the higher index into the supernode
	// with the lower index. Therefore, one must first try to merge the one with the higher
	// index. Otherwise, the supernode numbers are not correct anymore (sn.A vs. sn.A-1).
	if (d != 0 && sng.num.A != 1) { // Link 'l' is not on the A dimension, so we can try
                                    // merging on the A dimension. There are two possible
                                    // planes to merge. Left and right of the failure.

		sn_mergeCheck(0, sn.A, l);                  // Right of the failure
		if (sn.A != 0) sn_mergeCheck(0, sn.A-1, l); // Left of the failure
	}

	if (d != 1 && sng.num.B != 1) { // Link 'l' is not on the B dimension, so we can try
		                            // merging on the B dimension. There are two possible
		                            // planes to merge. Left and right of the failure.

		sn_mergeCheck(1, sn.B, l);                  // Right of the failure
		if (sn.B != 0) sn_mergeCheck(1, sn.B-1, l); // Left of the failure
	}

	if (d != 2 && sng.num.C != 1) { // Link 'l' is not on the C dimension, so we can try
		                            // merging on the C dimension. There are two possible
		                            // planes to merge. Left and right of the failure.
		
		sn_mergeCheck(2, sn.C, l);                  // Right of the failure
		if (sn.C != 0) sn_mergeCheck(2, sn.C-1, l); // Left of the failure
	}
	
	if (d != 3 && sng.num.D != 1) { // Link 'l' is not on the D dimension, so we can try
		                            // merging on the D dimension. There are two possible
		                            // planes to merge. Left and right of the failure.
		
		sn_mergeCheck(3, sn.D, l);                  // Right of the failure
		if (sn.D != 0) sn_mergeCheck(3, sn.D-1, l); // Left of the failure
	}

	if (d != 4 && sng.num.E != 1) { // Link 'l' is not on the E dimension, so we can try
		                            // merging on the E dimension. There are two possible
		                            // planes to merge. Left and right of the failure.
		
		sn_mergeCheck(4, sn.E, l);                  // Right of the failure
		if (sn.E != 0) sn_mergeCheck(4, sn.E-1, l); // Left of the failure
	}

	return 0;
}


/* takes a link 'l' an injects a fault into the internal data structures */
void sn_injectFault(dim_t l) {
	/*
	 *  (3,3)   |   (4,3)
	 *  ----------------- (2nd cut)
	 *  (3,2)   x   (4,2)
	 *  ----------------- (1st cut)
	 *  (3,1)   |   (4,1)
	 *          :
	 *      (3rd cut)
	 *
	 *  link = (7,4)
	 */

	// we cannot start merging until we fully splitted the supernode graph, thus we need to
	// remember possible merge candidates and merge them at the end.
	pair8_t mc[MAX_DIR]; // merge candidates;
	uint8_t mcc = 0;     // merge candidate counter;
	
	// get both ends of the link l
	dim_t n0 = lnk_getSrcNode(l);
	
	// get link dimension
	uint8_t ld = lnk_getDimension(l);

#ifdef SNG_DEBUG
	dim_t n1 = lnk_getDstNode(l);
	DPRINTF(SN__INJECT_FAULT, "injecting fault at (%d,%d,%d,%d,%d)----(%d,%d,%d,%d,%d)----(%d,%d,%d,%d,%d)\n",
			n0.A, n0.B, n0.C, n0.D, n0.E,
			l.A,  l.B,  l.C,  l.D, l.E,
			n1.A, n1.B, n1.C, n1.D, n1.E);
#endif
	
	// Perform parallel cutting
	uint8_t d;
	for(d = 0; d < torus.dim; d++) {
		if (d == ld) continue; // orthogonal cut in this dimension
		
		uint8_t cp0 = n0.dim[d]-1; // first parallel cut
		uint8_t cp1 = n0.dim[d];   // second parallel cut
		
		DPRINTF(SN__INJECT_FAULT, "1st parallel cut along plane %s[%d]\n", dim2str[d], cp0);
		if (sn_split(d, cp0) == 0) {
			DPRINTF(SN__INJECT_FAULT, "split has already been there --> Merge Candidate\n");
			mc[mcc++] = (pair8_t) {d, cp0};
		}
		
		DPRINTF(SN__INJECT_FAULT, "2nd parallel cut along plane %s[%d]\n", dim2str[d], cp1);
		if (sn_split(d, cp1) == 0) {
			DPRINTF(SN__INJECT_FAULT, "split has already been there --> Merge Candidate\n");
			mc[mcc++] = (pair8_t) {d, cp1};
		}
	}
	
	// Perform orthogonal cut
	uint8_t co = n0.dim[ld];
	DPRINTF(SN__INJECT_FAULT, "orthogonal cut along plane %s[%d]\n", dim2str[ld], co);
	sn_split(ld, co);
	
	// check merge candidates and merge if possible
	uint8_t i;
	for(i = 0; i < mcc; i++) {
		sn_mergeCheck(mc[i].one, sn_getIndex(mc[i].one, mc[i].two), l);
	}
	
	/*
	  a block of completely unreachable nodes is only possible under certain circumstances
	  - all parallel neighbor links are bad (in case the current link is somewhere in the
        middle of a block face.
	  - half of the parallel neighbor links are bad (in case the current link is in the
        corner of a block
	  - something inbetween the above two cases (in case the current link is at one or more
        edges of the block.
	  - some more cases, I guess...
	  - I think a block that include ignorable failures can only happen iff the above
        sn_mergeCheck was not successful for at least 1 possible merge. But I think that's
        not worth checking as it will be the case almost always.
	  
	  What stands out is that in all cases one or more cuts coincide. Moreover the opposite
	  is true as well: If no cut coincides, then there can be no block. This will be used
	  here to limit the block detection. I'm sure I can do better, but this is ok for now.
	*/
	if (mcc != 0) sn_mergeHRDetect(l); // TODO: Do better than this! Be more precise
	                                   // and limit the cases to what's really
	                                   // necessary.

	// update source supernode
	sng.this.index = sn_get(torus.local.log);
	sng.this.coord = sn_getAnchor(sng.this.index);
	sng.this.rank  = sn_getRank(sng.this.index);
	
	DPRINTF(SN__INJECT_FAULT, "Setting new source supernode number to (%d,%d,%d,%d,%d)=[%d,%d,%d,%d,%d]\n",
			sng.this.coord.A, sng.this.coord.B, sng.this.coord.C, sng.this.coord.D, sng.this.coord.E,
			sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E);
}


/* Injects fault 'l' into the link matrix.
 */
int lnk_injectFault(dim_t l) {
  {
    dim_t n0 = lnk_getSrcNode(l);
    dim_t n1 = lnk_getDstNode(l);
    
    DPRINTF(SN__INJECT_FAULT, "injecting fault at {%d,%d,%d,%d,%d}----{%d,%d,%d,%d,%d}----{%d,%d,%d,%d,%d}\n",
	    n0.A, n0.B, n0.C, n0.D, n0.E,
	    l.A,  l.B,  l.C,  l.D, l.E,
	    n1.A, n1.B, n1.C, n1.D, n1.E);
  }

	// get source node of link 'l'.
	l = lnk_phy2log(l);

	dim_t n0 = lnk_getSrcNode(l);
	
	// get dimension of link 'l'.
	uint8_t d = lnk_getDimension(l);
	
	// Add the fault to the axes and planes that are used for efficient merging.
	if ((sng.axes.dim[d][n0.dim[c[d].dim[1]]][n0.dim[c[d].dim[2]]][n0.dim[c[d].dim[3]]][n0.dim[c[d].dim[4]]] & (1 << n0.dim[d])) == 0) {
		sng.axes.dim[d][n0.dim[c[d].dim[1]]][n0.dim[c[d].dim[2]]][n0.dim[c[d].dim[3]]][n0.dim[c[d].dim[4]]] |= (1 << n0.dim[d]);
		sng.plane.dim[d][n0.dim[d]]++;
		
		sn_injectFault(l);
		
		torus.failures++;

#ifdef SNG_DEBUG
		uint32_t lr = lnk_getRank(l);
		torus.links[lr].status = 1;
#endif
		
		return 1;
	}
	
	return 0;
}


/*
  This function prints out information that can easily converted into latex.
  
  - if phy is 1, then if will use physical coordinates, otherwise, if will use logical
    ones.
 */
void sng_printLatex() {
	// 0. Print general information
	DPRINTF(1, "SNG LOGICAL COORDS\n");
	
	DPRINTF(1, "SNG DIMENSION ORDER : %s %s %s %s %s\n",
			dim2str[torus.rdo.A],
			dim2str[torus.rdo.B],
			dim2str[torus.rdo.C],
			dim2str[torus.rdo.D],
			dim2str[torus.rdo.E]);
	
	DPRINTF(1, "SNG SIZE : %d %d %d %d %d\n",
			sng.num.A,
			sng.num.B,
			sng.num.C,
			sng.num.D,
			sng.num.E);

	// 1. Print all nodes
	dim_t ni;
	
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
						uint32_t nr = sn_getRank(ni);
						
						if (ni.raw == sng.this.index.raw) {
							DPRINTF(1, "SNG NODE SOURCE : %d %d %d %d %d\n",
									ni.A, ni.B, ni.C, ni.D, ni.E);
							continue; 
						}
						/*
						  if (sng.nodes[nr].pred->flags & 0x4) {
						  DPRINTF(1, "SNG NODE INTERMEDIATE : %d %d %d %d : %d\n",
						  ni.A, ni.B, ni.C, ni.D, torus.nodes[nr].isIntermediate);
						  continue;
						  }
						*/
						if (sng.nodes[nr].pred->flags & 0x4) {
							DPRINTF(1, "SNG NODE CUTOFF : %d %d %d %d %d\n",
									ni.A, ni.B, ni.C, ni.D, ni.E);
							continue;
						}
						
						if (sng.nodes[nr].pred->flags == 0) {
							DPRINTF(1, "SNG NODE REACHABLE : %d %d %d %d %d\n",
									ni.A, ni.B, ni.C, ni.D, ni.E);
							continue;
						} else {
							DPRINTF(1, "SNG NODE UNREACHABLE : %d %d %d %d %d\n",
									ni.A, ni.B, ni.C, ni.D, ni.E);
							continue;
						}
					}
				}
			}
		}
	}

	// 3. Print all bad links
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
					
						dim_t   si = ni; // link source
						dim_t   sc = sn_getAnchor(si);
						uint8_t d;
						
						// check all links of supernode 'ni'
						for(d = 0; d < torus.dim; d++) {
							dim_t di = ni; // link destination in plus direction
							di.dim[d] = (di.dim[d] + 1) % sng.num.dim[d];
							
							if (! sng_getLinkStatus(sc, si, (d << 1)+1)) continue;
							
							// We only have to consider plus directions
							if (di.dim[d] == 0) {
								dim_t wi = si;
								wi.dim[d]++;
								DPRINTF(1, "SNG LINK BAD : %d %d %d %d %d : %d %d %d %d %d : WRAP : %s %d\n",
										si.A, si.B, si.C, si.D, si.E, wi.A, wi.B, wi.C, wi.D, wi.E, dim2str[d], d);
								
								wi.dim[d] = -1;
								DPRINTF(1, "SNG LINK BAD : %d %d %d %d %d : %d %d %d %d %d : %s %d\n",
										wi.A, wi.B, wi.C, wi.D, wi.E, di.A, di.B, di.C, di.D, di.E, dim2str[d], d);
							} else {
								DPRINTF(1, "SNG LINK BAD : %d %d %d %d %d : %d %d %d %d %d : : %s %d\n",
										si.A, si.B, si.C, si.D, si.E, di.A, di.B, di.C, di.D, di.E, dim2str[d], d);
							}
						}
					}
				}
			}
		}
	}
	
	// 4. Print all routes
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
					
						uint32_t nr = sn_getRank(ni);
						
						if (sng.nodes[nr].pred->dir == 0) continue;
						
						uint8_t dir = __builtin_ffs(sng.nodes[nr].pred->dir)-1;
						uint8_t d   = dir >> 1;
						
						dim_t   di  = ni;                            // link destination
						dim_t   si  = sng_getNeighbor(di, 1 << dir); // link source
						
						// dir is from di -> si, but I need si -> di next, thus invert direction
						dir = dir2inv[dir];
						
						if (di.dim[d] == 0 && is_plus(dir)) {
							dim_t wi = si;
							wi.dim[d]++;
							DPRINTF(1, "SNG LINK GOOD : %d %d %d %d %d : %d %d %d %d %d : WRAP : %s %d\n",
									si.A, si.B, si.C, si.D, si.E, wi.A, wi.B, wi.C, wi.D, wi.E, dir2str[dir], dir);
							
							wi.dim[d] = -1;
							DPRINTF(1, "SNG LINK GOOD : %d %d %d %d %d : %d %d %d %d %d : : %s %d\n",
									wi.A, wi.B, wi.C, wi.D, wi.E, di.A, di.B, di.C, di.D, di.E, dir2str[dir], dir);
						} else if (si.dim[d] == 0 && is_minus(dir)) {
							dim_t wi = si;
							wi.dim[d] = -1;
							DPRINTF(1, "SNG LINK GOOD : %d %d %d %d %d : %d %d %d %d %d : WRAP : %s %d\n",
									si.A, si.B, si.C, si.D, si.E, wi.A, wi.B, wi.C, wi.D, wi.E, dir2str[dir], dir);
							
							wi.dim[d] = sng.num.dim[d];
							DPRINTF(1, "SNG LINK GOOD : %d %d %d %d %d : %d %d %d %d %d : : %s %d\n",
									wi.A, wi.B, wi.C, wi.D, wi.E, di.A, di.B, di.C, di.D, di.E, dir2str[dir], dir);
						} else {
							DPRINTF(1, "SNG LINK GOOD : %d %d %d %d %d : %d %d %d %d %d : : %s %d\n",
									si.A, si.B, si.C, si.D, si.E, di.A, di.B, di.C, di.D, di.E, dir2str[dir], dir);
						}
					}
				}
			}
		}
	}
}


void sng_buildSetup() {
	DPRINTF(SNG__BUILD_SETUP, "Setting up the supernode graph...\n");
	
	// initialize supernode graph
	bzero((void *)&sng, sizeof(sng)); // clear complete supernode graph
	
	// 1 supernode per dimension
	sng.num.raw = 0x0101010101;

	int d,i;
	for(d = 0; d < MAX_DIM; d++) {
		
		// setup the initial supernode
		sng.anchor.dim[d][0] = 0;
		sng.size.dim[d][0]   = torus.size.dim[d];
		
		// initialize currently 
		for(i = 1; i <= MAX_DIM_SIZE; i++) {
			sng.anchor.dim[d][i] = torus.size.dim[d];
			sng.size.dim[d][i]   = 1;
		}
	}

	sng.this.index.raw = 0;
	sng.this.coord.raw = 0;
	sng.this.rank      = 0;

	DPRINTF(1, "c[0] = {%d,%d,%d,%d,%d}, c[1] = {%d,%d,%d,%d,%d}, c[2] = {%d,%d,%d,%d,%d}, c[3] = {%d,%d,%d,%d,%d}, c[4] = {%d,%d,%d,%d,%d}\n",
			c[0].dim[0], c[0].dim[1], c[0].dim[2], c[0].dim[3], c[0].dim[4],
			c[1].dim[0], c[1].dim[1], c[1].dim[2], c[1].dim[3], c[1].dim[4],
			c[2].dim[0], c[2].dim[1], c[2].dim[2], c[2].dim[3], c[2].dim[4],
			c[3].dim[0], c[3].dim[1], c[3].dim[2], c[3].dim[3], c[3].dim[4],
			c[4].dim[0], c[4].dim[1], c[4].dim[2], c[4].dim[3], c[4].dim[4]);
}
