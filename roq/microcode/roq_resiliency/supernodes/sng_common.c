#include "common.h"
#include "sng_common.h"
#include "sng_build.h"
#include "torus.h"

#include <string.h>

//queue_t nfQueue;   // new failure queue.

// Determine coorinates
// access: 
// - A's bit of A[D][C][B];  // 321
// - B's bit of B[D][C][A];  // 320
// - C's bit of C[D][B][A];  // 310
// - D's bit of D[C][B][A];  // 210
dim_t c[5] = { {0,4,3,2,1},
			   {1,4,3,2,0},
			   {2,4,3,1,0},
			   {3,4,2,1,0},
			   {4,3,2,1,0} };

#define SN__GET_LINK 1

#ifdef ROQ_RESILIENCY_BGQ
inline size_t strlen ( const char * str ) {
	size_t len = 0;
	while(1) {
		if (str[len] == '\0') return len;
		len++;
	}
}
#endif


/* takes node 'n' and returns its rank */
inline uint32_t nd_getRank(dim_t nc) {
  return (nc.A +
		  nc.B * torus.size.A +
		  nc.C * torus.size.B * torus.size.A +
		  nc.D * torus.size.C * torus.size.B * torus.size.A +
		  nc.E * torus.size.D * torus.size.C * torus.size.B * torus.size.A);
}

/*
  takes node 'n' and returns its index rank

  - 'ni' is the index of the supernode
*/
inline uint32_t sn_getRank(dim_t ni) {
  return (ni.A +
		  ni.B * sng.num.A +
		  ni.C * sng.num.B * sng.num.A +
		  ni.D * sng.num.C * sng.num.B * sng.num.A +
		  ni.E * sng.num.D * sng.num.C * sng.num.B * sng.num.A);
}

inline dim_t sn_getCoords(uint32_t nr) {
	dim_t ni;
	/* match the hardware / control system */
	ni.E = ((nr)
			/ (sng.num.A * sng.num.B * sng.num.C * sng.num.D));
	
	ni.D = ((nr - (ni.E * sng.num.D * sng.num.C * sng.num.B * sng.num.A))
			/ (sng.num.C * sng.num.B * sng.num.A));

	ni.C = ((nr - ((ni.E * sng.num.D * sng.num.C * sng.num.B * sng.num.A) +
				   (ni.D * sng.num.C * sng.num.B * sng.num.A)))
			/ (sng.num.B * sng.num.A));

	ni.B = ((nr - ((ni.E * sng.num.D * sng.num.C * sng.num.B * sng.num.A) +
				   (ni.D * sng.num.C * sng.num.B * sng.num.A)             +
				   (ni.C * sng.num.B * sng.num.A)))
			/ sng.num.A);

	ni.A = ((nr - ((ni.E * sng.num.D * sng.num.C * sng.num.B * sng.num.A) +
				   (ni.D * sng.num.C * sng.num.B * sng.num.A)             +
				   (ni.C * sng.num.B * sng.num.A)                         +
				   (ni.B * sng.num.A))));

	return ni;
}


/* takes a link 'l' and returns its rank */
uint32_t lnk_getRank(dim_t l) {
	// A link rank is its node rank combined with the link dimension. As there are 5
	// Dimensions, the node rank is multiplied with 5 (MAX_DIM), then the dimension is
	// added on top.

	dim_t    n0   = lnk_getSrcNode(l);
	uint32_t rank = (nd_getRank(n0) * MAX_DIM) + lnk_getDimension(l);
	return rank;
}


inline uint8_t lnk_getDimension(dim_t l) {
#if __BYTE_ORDER__ == __LITTLE_ENDIAN
  return __builtin_ffsl(l.raw & 0x0101010101) >> 3;
#else
  return (__builtin_clzl(l.raw & 0x0101010101) - 31) >> 3;
#endif
}


inline uint8_t lnk_getDirection(dim_t l) {
#if __BYTE_ORDER__ == __LITTLE_ENDIAN
  return __builtin_ffsl(l.raw & 0x0101010101) >> 2;
#else
  return (__builtin_clzl(l.raw & 0x0101010101) - 31) >> 2;
#endif
}


inline void lnk_getEndpoints(dim_t l, dim_t *n0, dim_t *n1) {
	uint8_t d = lnk_getDimension(l);

	// get both ends of the link l
	n0->raw    = (l.raw >> 1) & 0x1F1F1F1F1F;

	n1->raw    = n0->raw;
	n1->dim[d] = (n0->dim[d] + 1) % torus.size.dim[d];
}


inline dim_t lnk_getSrcNode(dim_t l) {
	dim_t n0;
	n0.raw    = (l.raw >> 1) & 0x1F1F1F1F1F;

	return n0;
}

inline dim_t lnk_getDstNode(dim_t l) {
	uint8_t d = lnk_getDimension(l);

	dim_t n1;
	n1.raw    = (l.raw >> 1) & 0x1F1F1F1F1F;
	n1.dim[d] = (n1.dim[d] + 1) % torus.size.dim[d];

	return n1;
}


void sng_print() {
	int d,i;
	
	int sum;
	DPRINTF(1, "Source node = (%d,%d,%d,%d,%d), Source supernode = [%d,%d,%d,%d,%d]=(%d,%d,%d,%d,%d)\n",
			torus.local.log.A, torus.local.log.B, torus.local.log.C, torus.local.log.D, torus.local.log.E,
			sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E,
			sng.this.coord.A, sng.this.coord.B, sng.this.coord.C, sng.this.coord.D, sng.this.coord.E);
	
	// initialize supernode graph
	for(d = 0; d < MAX_DIM; d++) {
		char line[256];
		sprintf(line, "%s/%d = ", dim2str[d], d);
		sum = 0;
		for(i = 0; i < sng.num.dim[d]; i++) {
			sum += sng.size.dim[d][i];
			sprintf(line + strlen(line), "[%i|a=%u|s=%u] ", i, sng.anchor.dim[d][i], sng.size.dim[d][i]);
		}
		if (sum != torus.size.dim[d]) sprintf(line + strlen(line), " --> BUG! %d != %d \n", sum, torus.size.dim[d]);
		/*
		  printf("  |  ");
		  
		  for(i = sng.num.dim[d]; i < torus.size.dim[d]; i++) {
		  printf("[%i|a=%u|s=%u] ", i, sng.anchor.dim[d][i], sng.size.dim[d][i]);
		  }
		*/
		DPRINTF(1, "%s\n", line);
	}
	
	dim_t a;
	for(a.E = 0; a.E < torus.size.E; a.E++) {
		for(a.D = 0; a.D < torus.size.D; a.D++) {
			for(a.C = 0; a.C < torus.size.C; a.C++) {
				for(a.B = 0; a.B < torus.size.B; a.B++) {
					if (sng.axes.A[a.E][a.D][a.C][a.B] != 0) 
						DPRINTF(1, "Axis A[%u][%u][%u][%u] = %08x\n", a.E, a.D, a.C, a.B, (unsigned int) sng.axes.A[a.E][a.D][a.C][a.B]);
				}
			}
		}
	}

	for(a.E = 0; a.E < torus.size.E; a.E++) {
		for(a.D = 0; a.D < torus.size.D; a.D++) {
			for(a.C = 0; a.C < torus.size.C; a.C++) {
				for(a.A = 0; a.A < torus.size.A; a.A++) {
					if (sng.axes.B[a.E][a.D][a.C][a.A] != 0) 
						DPRINTF(1, "Axis B[%u][%u][%u][%u] = %08x\n", a.E, a.D, a.C, a.A, (unsigned int) sng.axes.B[a.E][a.D][a.C][a.A]);
				}
			}
		}
	}

	for(a.E = 0; a.E < torus.size.E; a.E++) {
		for(a.D = 0; a.D < torus.size.D; a.D++) {
			for(a.B = 0; a.B < torus.size.B; a.B++) {
				for(a.A = 0; a.A < torus.size.A; a.A++) {
					if (sng.axes.C[a.E][a.D][a.B][a.A] != 0) 
						DPRINTF(1, "Axis C[%u][%u][%u][%u] = %08x\n", a.E, a.D, a.B, a.A, (unsigned int) sng.axes.C[a.E][a.D][a.B][a.A]);
				}
			}
		}
	}


	for(a.E = 0; a.E < torus.size.E; a.E++) {
		for(a.C = 0; a.C < torus.size.C; a.C++) {
			for(a.B = 0; a.B < torus.size.B; a.B++) {
				for(a.A = 0; a.A < torus.size.A; a.A++) {
					if (sng.axes.D[a.E][a.C][a.B][a.A] != 0) 
						DPRINTF(1, "Axis D[%u][%u][%u][%u] = %08x\n", a.E, a.C, a.B, a.A, (unsigned int) sng.axes.D[a.E][a.C][a.B][a.A]);
				}
			}
		}
	}

	for(a.D = 0; a.D < torus.size.D; a.D++) {
		for(a.C = 0; a.C < torus.size.C; a.C++) {
			for(a.B = 0; a.B < torus.size.B; a.B++) {
				for(a.A = 0; a.A < torus.size.A; a.A++) {
					if (sng.axes.E[a.D][a.C][a.B][a.A] != 0) 
						DPRINTF(1, "Axis E[%u][%u][%u][%u] = %08x\n", a.D, a.C, a.B, a.A, (unsigned int) sng.axes.E[a.D][a.C][a.B][a.A]);
				}
			}
		}
	}
}

/*
  Returns neighbor of supernode 'ni' in direction given by the direction vector 'dv'

  - 'ni'  is the index of the supernode
  - 'dv' is the direction vector w.r.t. 'ni'
 */
dim_t sng_getNeighbor(dim_t ni, uint16_t dv) {
  if (dv == 0) return ni;
  uint8_t dir = __builtin_ffs(dv)-1;
  uint8_t d   = dir >> 1;

  if (is_minus(dir)) {
    ni.dim[d] = (ni.dim[d] == 0) ? sng.num.dim[d]-1 : ni.dim[d]-1;
  } else {
    ni.dim[d] = (ni.dim[d] + 1) % sng.num.dim[d];
  }

  return ni;
}

/* 
   Converts physical node coordinates to logical ones.
   1. apply dimenion mapping according to the routing dimension order
   2. apply offset
*/
dim_t nd_phy2log(dim_t pc) {
	dim_t lc;
	lc.A = mod(pc.dim[torus.rdo.A] + torus.offset.A, torus.size.A);
	lc.B = mod(pc.dim[torus.rdo.B] + torus.offset.B, torus.size.B);
	lc.C = mod(pc.dim[torus.rdo.C] + torus.offset.C, torus.size.C);
	lc.D = mod(pc.dim[torus.rdo.D] + torus.offset.D, torus.size.D);
	lc.E = mod(pc.dim[torus.rdo.E] + torus.offset.E, torus.size.E);
	
	return lc;
}

/*
  Converts logical node coordinates to physical ones.
  1. apply dimenion mapping according to the routing dimension order
  2. apply offset
*/
dim_t nd_log2phy(dim_t lc) {
	dim_t pc;
	pc.dim[torus.rdo.A] = mod(lc.A - torus.offset.A, torus.size.A);
	pc.dim[torus.rdo.B] = mod(lc.B - torus.offset.B, torus.size.B);
	pc.dim[torus.rdo.C] = mod(lc.C - torus.offset.C, torus.size.C);
	pc.dim[torus.rdo.D] = mod(lc.D - torus.offset.D, torus.size.D);
	pc.dim[torus.rdo.E] = mod(lc.E - torus.offset.E, torus.size.E);
	
	return pc;
}


/*
  Converts physical link coordinates to logical ones.
  1. apply dimenion mapping according to the routing dimension order
  2. apply offset
*/
dim_t lnk_phy2log(dim_t pc) {
	dim_t lc;
	lc.A = mod(pc.dim[torus.rdo.A] + (torus.offset.A << 1), (torus.size.A << 1));
	lc.B = mod(pc.dim[torus.rdo.B] + (torus.offset.B << 1), (torus.size.B << 1));
	lc.C = mod(pc.dim[torus.rdo.C] + (torus.offset.C << 1), (torus.size.C << 1));
	lc.D = mod(pc.dim[torus.rdo.D] + (torus.offset.D << 1), (torus.size.D << 1));
	lc.E = mod(pc.dim[torus.rdo.E] + (torus.offset.E << 1), (torus.size.E << 1));
	
	return lc;
}

/*
  Converts logical link coordinates to physical ones.
  1. apply dimenion mapping according to the routing dimension order
  2. apply offset
*/
dim_t lnk_log2phy(dim_t lc) {
	dim_t pc;
	pc.dim[torus.rdo.A] = mod(lc.A - (torus.offset.A << 1), (torus.size.A << 1));
	pc.dim[torus.rdo.B] = mod(lc.B - (torus.offset.B << 1), (torus.size.B << 1));
	pc.dim[torus.rdo.C] = mod(lc.C - (torus.offset.C << 1), (torus.size.C << 1));
	pc.dim[torus.rdo.D] = mod(lc.D - (torus.offset.D << 1), (torus.size.D << 1));
	pc.dim[torus.rdo.E] = mod(lc.E - (torus.offset.E << 1), (torus.size.E << 1));
	
	return pc;
}

uint16_t hint_log2phy(uint16_t lh) {
	uint16_t ph;

	/*

	  rdo      = 3,2,1,0,4
	  log hint = dd.cc.bb.aa.ee
	  phy hint = aa.bb.cc.dd.ee

	  1. isolate dimension         : int = log hint >> (4-rdo.A)*2 & 0x3
	  2. move to physical location : int << 0x8
	  3. do that with every dimension and OR the results.

	 */


	ph = ((((lh >> ((4-torus.odr.A) << 1)) & 0x3) << 0x8) |
		  (((lh >> ((4-torus.odr.B) << 1)) & 0x3) << 0x6) |
		  (((lh >> ((4-torus.odr.C) << 1)) & 0x3) << 0x4) |
		  (((lh >> ((4-torus.odr.D) << 1)) & 0x3) << 0x2) |
		  (((lh >> ((4-torus.odr.E) << 1)) & 0x3) << 0x0));

	return ph;
}


#ifdef ROQ_RESILIENCY_BGQ
// PowerPCs don't have popcount asm and gcc fails to generate code in a cross-compile
// scenario.
inline int has2ormore1s32(uint32_t v) {
	// if ffs and 32-clz return the same value, it's the same one, thus there's only one
	// 1, except if v == 0.
	return v != 0 && (__builtin_ffs(v) != (32-__builtin_clz(v)));
}
#endif

inline uint32_t axis_get(uint8_t d, dim_t ac) {
	return sng.axes.dim[d] [ac.dim[c[d].dim[1]]] [ac.dim[c[d].dim[2]]] [ac.dim[c[d].dim[3]]] [ac.dim[c[d].dim[4]]];
}

/*
  Takes a bit vector 'axis', that represents every link on an axis with 1 bit (0=good,
  1=broken) and a 'mask'. Upon every call, it will return the next broken link on the
  axis.
 */
inline uint8_t axis_getNextOne(uint32_t axis, uint32_t *mask) {
  uint8_t next = __builtin_ffs(axis & ~(*mask))-1;
  *mask |= 1 << next;

  return next;
}
