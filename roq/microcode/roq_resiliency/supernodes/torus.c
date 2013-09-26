#include <string.h>

#include "torus.h"
#include "sng_common.h"
#include "sng_build.h"
#include "sng_route.h"
#include "common.h"

#define TORUS__SETUP                 1
#define TORUS__SET_INTERMEDIATE_NODE 1
#define TORUS__SET_HINT_BITS         1
#define TORUS__PRINT_ROUTES          1


void torus_setDimensions(int d) {
	torus.dim = d;
	
	printf("Setting torus dimensions to %d\n", torus.dim);
}

void torus_setSize(dim_t s) {
	torus.size = (dim_t) { s.dim[torus.rdo.A],
						   s.dim[torus.rdo.B],
						   s.dim[torus.rdo.C],
						   s.dim[torus.rdo.D],
						   s.dim[torus.rdo.E] };
	
	torus.nodecount = s.A*s.B*s.C*s.D*s.E;

	printf("Setting torus size to logical (%d,%d,%d,%d,%d). Torus contains %d nodes\n",
		   torus.size.A, torus.size.B, torus.size.C, torus.size.D, torus.size.E, torus.nodecount);
}

/* Expects physical coordinates of the local/source node */
void torus_setLocal(dim_t l) {
	torus.local.log  = nd_phy2log(l);
	torus.local.phy  = l;
	torus.local.rank = nd_getRank(l);
	
	printf("Setting local node to logical (%d,%d,%d,%d,%d), physical (%d,%d,%d,%d,%d), rank %d\n",
		   torus.local.log.A, torus.local.log.B, torus.local.log.C, torus.local.log.D, torus.local.log.E,
		   torus.local.phy.A, torus.local.phy.B, torus.local.phy.C, torus.local.phy.D, torus.local.phy.E,
		   torus.local.rank);
}

void torus_setOffset(dim_t o) {
	torus.offset = (dim_t) { o.dim[torus.rdo.A],
							 o.dim[torus.rdo.B],
							 o.dim[torus.rdo.C],
							 o.dim[torus.rdo.D],
							 o.dim[torus.rdo.E] };
	
	printf("Setting logical offset to (%d,%d,%d,%d,%d)\n",
		   torus.offset.A, torus.offset.B, torus.offset.C, torus.offset.D, torus.offset.E);
}

void torus_setTorus(uint64_t t) {
	torus.closed.A = (t & (0x1 << (4-torus.rdo.A))) != 0;
	torus.closed.B = (t & (0x1 << (4-torus.rdo.B))) != 0;
	torus.closed.C = (t & (0x1 << (4-torus.rdo.C))) != 0;
	torus.closed.D = (t & (0x1 << (4-torus.rdo.D))) != 0;
	torus.closed.E = (t & (0x1 << (4-torus.rdo.E))) != 0;
	
	printf("Setting closed dimensions to 0x%02x, (%d,%d,%d,%d,%d)\n", t,
		   torus.closed.A,
		   torus.closed.B,
		   torus.closed.C,
		   torus.closed.D,
		   torus.closed.E);
}

void torus_setDimensionOrder(dim_t o) {
	torus.rdo = o;

	int d;
	for(d = 0; d < MAX_DIM; d++) {
		torus.odr.dim[torus.rdo.dim[d]] = d;
	}
	
	printf("Setting routing dimension order to (%d,%d,%d,%d,%d) = (%s,%s,%s,%s,%s), reverse (%d,%d,%d,%d,%d)\n",
		   torus.rdo.A, torus.rdo.B, torus.rdo.C, torus.rdo.D, torus.rdo.E,
		   dim2str[torus.rdo.A], dim2str[torus.rdo.B], dim2str[torus.rdo.C], dim2str[torus.rdo.D], dim2str[torus.rdo.E],
		   torus.odr.A, torus.odr.B, torus.odr.C, torus.odr.D, torus.odr.E);
}

void torus_setup() {
	DPRINTF(TORUS__SETUP, "Setting up the torus...\n");
	
	bzero((void *)&torus, sizeof(torus));
}

inline uint16_t torus_fillHintBits(dim_t n, uint16_t h) {
	dim_t s = torus.local.log;
	
	DPRINTF(1, "before hint = 0x%03x, s = (%d,%d,%d,%d,%d), n = (%d,%d,%d,%d,%d)\n", h,
			s.A, s.B, s.C, s.D, s.E, n.A, n.B, n.C, n.D, n.E);
	int d;
	for(d = torus.dim; d-- > 0;) {
		// if no hint for that dimension has been set already and the dimensions of src and dst differ
		if ((h & dim2hint[d]) == 0 && s.dim[d] != n.dim[d]) {
			if (s.dim[d] < n.dim[d]) h |= dir2hint[(d << 1) + 1]; // src < dst --> go plus
			else                     h |= dir2hint[(d << 1)];     // src > dst --> go minus
		}
	}

	DPRINTF(1, "after hint = 0x%03x\n", h);
	return h;
}

void torus_setRoute(dim_t n, dim_t fw, uint16_t hint, int flags) {
	// I have to fill missing hint bits with default hint bits. The problem is that I have
	// to set all hint bits if I set any. If two supernodes include more than 1 nodes each, 
	// the hint bits it calculates only have 1 dimension set, however the actual nodes may
	// differ in more than 1 dimension and the hw for some reason doesn't fill in the
	// defaults.

	if (flags == 0x3) { // hint bits describe path to intermediate node 'fw'
		hint = torus_fillHintBits(fw, hint);
	} else if (flags == 0x2) { // hint bits describe path to destination node 'n'
		hint = torus_fillHintBits(n, hint);
	}

#ifdef SNG_DEBUG
	uint32_t nr = nd_getRank(n);
	
	torus.nodes[nr].flags      = 0;
	torus.nodes[nr].fwnode.raw = 0;
	torus.nodes[nr].hint       = 0;
	
	if (flags & 0x1) {
		torus.nodes[nr].fwnode  = fw;
		torus.nodes[nr].flags  |= 0x1;
		
		DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Setting (%d,%d,%d,%d,%d) as intermediate forwarding node for destination node (%d,%d,%d,%d,%d) (flags=%x)\n",
				fw.A, fw.B, fw.C, fw.D, fw.E,
				n.A, n.B, n.C, n.D, n.E,
				torus.nodes[nr].flags);


		uint32_t fwr = nd_getRank(fw);
		torus.nodes[fwr].isIntermediate++;
		
	}
	
	if (flags & 0x2) {
		torus.nodes[nr].hint   = hint;
		torus.nodes[nr].flags |= 0x2;
		
		DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Setting hint bits %02x for destination node (%d,%d,%d,%d,%d) (flags=%x)\n",
				hint,
				n.A, n.B, n.C, n.D, n.E,
				torus.nodes[nr].flags);
  }

#endif
#ifdef ROQ_RESILIENCY_BGQ
	Router_setRoute(nd_log2phy(n), nd_log2phy(fw), hint_log2phy(hint), flags);
#endif
}

void torus_setIntermediateNode(dim_t n, dim_t fw) {
	uint32_t nr = nd_getRank(n);
	torus.nodes[nr].fwnode  = fw;
	torus.nodes[nr].flags  |= 0x1;
	
	DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Setting (%d,%d,%d,%d,%d) as intermediate forwarding node for destination node (%d,%d,%d,%d,%d) (flags=%x)\n",
			fw.A, fw.B, fw.C, fw.D, fw.E,
			n.A, n.B, n.C, n.D, n.E,
			torus.nodes[nr].flags);
	
#ifdef SNG_DEBUG
	uint32_t fwr = nd_getRank(fw);
	torus.nodes[fwr].isIntermediate++;
#endif
}


void torus_setHintBits(dim_t n, uint16_t h) {
	uint32_t nr = nd_getRank(n);
	torus.nodes[nr].hint   = h;
	torus.nodes[nr].flags |= 0x2;
	
	DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Setting hint bits %02x for destination node (%d,%d,%d,%d,%d) (flags=%x)\n",
			h,
			n.A, n.B, n.C, n.D, n.E,
			torus.nodes[nr].flags);
}

void torus_setCutOff(dim_t n) {
	uint32_t nr = nd_getRank(n);
	torus.nodes[nr].flags = 0x4;
	
	DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Marking node (%d,%d,%d,%d,%d) as cut off (flags=%x)\n",
			n.A, n.B, n.C, n.D, n.E,
			torus.nodes[nr].flags);
	
#ifdef SNG_DEBUG
	torus.nodes[nr].isCutOff = 1;
#endif
#ifdef ROQ_RESILIENCY_BGQ
	Router_setCutOff(nd_log2phy(n));
#endif
}

void torus_setReachable(dim_t n) {
	uint32_t nr = nd_getRank(n);
	
	DPRINTF(TORUS__SET_INTERMEDIATE_NODE, "Marking node (%d,%d,%d,%d,%d) as reachable (flags=%x)\n",
			n.A, n.B, n.C, n.D, n.E,
			torus.nodes[nr].flags);
	
#ifdef SNG_DEBUG
	torus.nodes[nr].isReachable = 1;
#endif
}

#ifdef SNG_DEBUG
void torus_routeCheck() {
	void follow(dim_t cc, dim_t dc, uint16_t hint, dim_t ic, int flags) {
		if (cc.raw == dc.raw) {
			DPRINTF(TORUS__PRINT_ROUTES, "Destination node reached!\n");
			return;
		}
		
		int dir = -1;
		
		dim_t tc;       // temporaty
		dim_t nc = cc;  // next node
		
		char line[128];
		
		// once we reached the intermediate node, continue normally.
		if (cc.raw == ic.raw && flags&0x1) {
			flags ^= 0x1;
			hint   = 0;
			DPRINTF(TORUS__PRINT_ROUTES, "Intermediate node reached!\n");
			return;
		}
		
		if (flags&0x1) { // use intermediate node.
			tc.raw = dc.raw;
			dc.raw = ic.raw;
		}
		
		sprintf(line, "(%d,%d,%d,%d,%d) ---> ",
				cc.A, cc.B, cc.C, cc.D, cc.E);
		int i;
		for(i = 0; i < torus.dim; i++) {
			if (cc.dim[i] != dc.dim[i]) {
				if (hint & (0x2 << ((4-i) << 1))) {                // force minus direction
					nc.dim[i]--;
					dir = (i << 1);
				} else if (hint & (0x1 << ((4-i) << 1))) {         // force plus direction
					nc.dim[i]++;
					dir = (i << 1) + 1;
				} else {                                           // use default direction
					if (nc.dim[i] < dc.dim[i]) {
						nc.dim[i]++;
						dir = (i << 1) + 1;
					} else {
						nc.dim[i]--;
						dir = (i << 1);
					}
				}
				
				break;
			}
		}
		
		// check link health status
		if (cc.dim[i] < nc.dim[i]) {
			if (sng.axes.dim[i][cc.dim[c[i].dim[1]]][cc.dim[c[i].dim[2]]][cc.dim[c[i].dim[3]]][cc.dim[c[i].dim[4]]] & (1 << cc.dim[i])) {
				DPRINTF(TORUS__PRINT_ROUTES, "ERROR TRYING TO TRAVERSE A BROKEN LINK! BUG!\n");
				DPRINTF(TORUS__PRINT_ROUTES, "Link %d/%d/%d on axis %s[%d,%d,%d,%d] = %08x is bad (%d,%d,%d,%d,%d)\n",
					cc.dim[i], nc.dim[i], torus.size.dim[i], dim2str[i], cc.dim[c[i].dim[1]], cc.dim[c[i].dim[2]], cc.dim[c[i].dim[3]], cc.dim[c[i].dim[4]],
						sng.axes.dim[i][cc.dim[c[i].dim[1]]][cc.dim[c[i].dim[2]]][cc.dim[c[i].dim[3]]][cc.dim[c[i].dim[4]]],
						cc.A, cc.B, cc.C, cc.D, cc.E);
				//				return;
				sprintf(line + strlen(line), "BROKEN ---> ");

			}
		} else {
			if (sng.axes.dim[i][nc.dim[c[i].dim[1]]][nc.dim[c[i].dim[2]]][nc.dim[c[i].dim[3]]][nc.dim[c[i].dim[4]]] & (1 << nc.dim[i])) {
				DPRINTF(TORUS__PRINT_ROUTES, "ERROR TRYING TO TRAVERSE A BROKEN LINK! BUG!\n");
				DPRINTF(TORUS__PRINT_ROUTES, "Link %d|%d on axis %s[%d,%d,%d,%d] = %08x is bad (%d,%d,%d,%d,%d)\n",
					nc.dim[i], cc.dim[i], dim2str[i], nc.dim[c[i].dim[1]], nc.dim[c[i].dim[2]], nc.dim[c[i].dim[3]], nc.dim[c[i].dim[4]],
						sng.axes.dim[i][nc.dim[c[i].dim[1]]][nc.dim[c[i].dim[2]]][nc.dim[c[i].dim[3]]][nc.dim[c[i].dim[4]]],
						nc.A, nc.B, nc.C, nc.D, nc.E);
				//				return;
				sprintf(line + strlen(line), "BROKEN ---> ");
			}
		}
		
		nc.dim[dir >> 1] = mod(nc.dim[dir >> 1], torus.size.dim[dir >> 1]);
		
		sprintf(line + strlen(line), "(%d,%d,%d,%d,%d)",
				nc.A, nc.B, nc.C, nc.D, nc.E);
		
		DPRINTF(TORUS__PRINT_ROUTES, "%s\n", line);
		if (flags&0x1) { // use intermediate node
			dc.raw = tc.raw;
		}
		
		// Add link load
		if (is_plus(dir)) {
			dim_t l = (dim_t) {.raw = cc.raw << 1};
			l.dim[dir >> 1]++;
			uint32_t lr = lnk_getRank(l);
			torus.links[lr].weight.plus++;
		} else {
			dim_t l = (dim_t) {.raw = nc.raw << 1};
			l.dim[dir >> 1]++;
			uint32_t lr = lnk_getRank(l);
			torus.links[lr].weight.minus++;
		}
		
		follow(nc, dc, hint, ic, flags);
	}
	
	dim_t lc; // link coordinate
	uint8_t d[5];
	
	for(d[0] = torus.dim; d[0]-- > 0;) {
		d[1] = c[d[0]].dim[1];
		d[2] = c[d[0]].dim[2];
		d[3] = c[d[0]].dim[3];
		d[4] = c[d[0]].dim[4];
		
		for(lc.dim[d[1]] = 0; lc.dim[d[1]] < torus.size.dim[d[1]]; lc.dim[d[1]]++) {
			for(lc.dim[d[2]] = 0; lc.dim[d[2]] < torus.size.dim[d[2]]; lc.dim[d[2]]++) {
				for(lc.dim[d[3]] = 0; lc.dim[d[3]] < torus.size.dim[d[3]]; lc.dim[d[3]]++) {
					for(lc.dim[d[4]] = 0; lc.dim[d[4]] < torus.size.dim[d[4]]; lc.dim[d[4]]++) {
						for(lc.dim[d[0]] = 0; lc.dim[d[0]] < torus.size.dim[d[0]]; lc.dim[d[0]]++) {
							if (sng.axes.dim[d[0]][lc.dim[d[1]]][lc.dim[d[2]]][lc.dim[d[3]]][lc.dim[d[4]]] & (1 << lc.dim[d[0]])) {
								dim_t sc = lc;
								dim_t dc = lc;
								dc.dim[d[0]] = (dc.dim[d[0]]+1)%torus.size.dim[d[0]];
							}
						}
					}
				}
			}
		}
	}


	dim_t nc;
	
	for(nc.A = 0; nc.A < torus.size.A; nc.A++) {
		for(nc.B = 0; nc.B < torus.size.B; nc.B++) {
			for(nc.C = 0; nc.C < torus.size.C; nc.C++) {
				for(nc.D = 0; nc.D < torus.size.D; nc.D++) {
					for(nc.E = 0; nc.E < torus.size.E; nc.E++) {
						uint32_t nr = nd_getRank(nc);
						
						DPRINTF(TORUS__PRINT_ROUTES, "(%d,%d,%d,%d,%d) --> (%d,%d,%d,%d,%d), hint bits = %03x, intermediate node = (%d,%d,%d,%d,%d), flags = %x\n",
								torus.local.log.A, torus.local.log.B, torus.local.log.C, torus.local.log.D, torus.local.log.E,
								nc.A, nc.B, nc.C, nc.D, nc.E, 
								torus.nodes[nr].hint,
								torus.nodes[nr].fwnode.A, torus.nodes[nr].fwnode.B, torus.nodes[nr].fwnode.C, torus.nodes[nr].fwnode.D, torus.nodes[nr].fwnode.E,
								torus.nodes[nr].flags);
						
						if (torus.nodes[nr].flags != 0x4) // 0x4 = cut off.
							follow(torus.local.log, nc, torus.nodes[nr].hint, torus.nodes[nr].fwnode, torus.nodes[nr].flags);
					}
				}
			}
		}
	}
}
#else
void torus_routeCheck() {}
#endif


#ifdef SNG_DEBUG
/*
  This function prints out information that can easily converted into latex.
  
  - if phy is 1, then if will use physical coordinates, otherwise, if will use logical
    ones.
 */
void torus_printLatex(int phy) {
	// 0. Print general information
	if (phy) {
		DPRINTF(1, "TORUS PHYSICAL COORDS\n");
	} else {
		DPRINTF(1, "TORUS LOGICAL COORDS\n");
	}
	
	DPRINTF(1, "TORUS DIMENSION ORDER : %s %s %s %s %s\n",
			dim2str[torus.rdo.A], dim2str[torus.rdo.B], dim2str[torus.rdo.C], dim2str[torus.rdo.D], dim2str[torus.rdo.E]);
	
	if (phy) {
		DPRINTF(1, "TORUS SIZE : %d %d %d %d %d\n",
				torus.size.dim[torus.rdo.A],
				torus.size.dim[torus.rdo.B],
				torus.size.dim[torus.rdo.C],
				torus.size.dim[torus.rdo.D],
				torus.size.dim[torus.rdo.E]);
	} else {
		DPRINTF(1, "TORUS SIZE : %d %d %d %d %d\n",
				torus.size.A,
				torus.size.B,
				torus.size.C,
				torus.size.D,
				torus.size.E);
	}
	
	if (phy) {
		DPRINTF(1, "TORUS OFFSET : %d %d %d %d %d\n",
				torus.offset.dim[torus.rdo.A],
				torus.offset.dim[torus.rdo.B],
				torus.offset.dim[torus.rdo.C],
				torus.offset.dim[torus.rdo.D],
				torus.offset.dim[torus.rdo.E]);
	} else {
		DPRINTF(1, "TORUS OFFSET : %d %d %d %d %d\n",
				torus.offset.A,
				torus.offset.B,
				torus.offset.C,
				torus.offset.D,
				torus.offset.E);
	}
	
	if (phy) {
		DPRINTF(1, "TORUS CUTOFF : A : %d\n",
				mod(torus.size.dim[torus.rdo.A] - torus.offset.dim[torus.rdo.A] - 1, torus.size.dim[torus.rdo.A]));
		DPRINTF(1, "TORUS CUTOFF : B : %d\n",
				mod(torus.size.dim[torus.rdo.B] - torus.offset.dim[torus.rdo.B] - 1, torus.size.dim[torus.rdo.B]));
		DPRINTF(1, "TORUS CUTOFF : C : %d\n",
				mod(torus.size.dim[torus.rdo.C] - torus.offset.dim[torus.rdo.C] - 1, torus.size.dim[torus.rdo.C]));
		DPRINTF(1, "TORUS CUTOFF : D : %d\n",
				mod(torus.size.dim[torus.rdo.D] - torus.offset.dim[torus.rdo.D] - 1, torus.size.dim[torus.rdo.D]));
		DPRINTF(1, "TORUS CUTOFF : E : %d\n",
				mod(torus.size.dim[torus.rdo.E] - torus.offset.dim[torus.rdo.E] - 1, torus.size.dim[torus.rdo.E]));
	}
	
	
	// 1. Print all nodes
	dim_t nc;
	
	for(nc.A = 0; nc.A < torus.size.A; nc.A++) {
		for(nc.B = 0; nc.B < torus.size.B; nc.B++) {
			for(nc.C = 0; nc.C < torus.size.C; nc.C++) {
				for(nc.D = 0; nc.D < torus.size.D; nc.D++) {
					for(nc.E = 0; nc.E < torus.size.E; nc.E++) {
						uint32_t nr = nd_getRank(nc);
						
						dim_t n = nc;
						if (phy) n = nd_log2phy(n);
						
						if (nc.raw == torus.local.log.raw) {
							DPRINTF(1, "TORUS NODE SOURCE : %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E);
							continue; 
						}
						
						if (torus.nodes[nr].isIntermediate > 0) {
							DPRINTF(1, "TORUS NODE INTERMEDIATE : %d %d %d %d %d : %d\n",
									n.A, n.B, n.C, n.D, n.E, torus.nodes[nr].isIntermediate);
							continue;
						}
						
						if (torus.nodes[nr].isCutOff == 1) {
							DPRINTF(1, "TORUS NODE CUTOFF : %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E);
							continue;
						}
						
						if (torus.nodes[nr].isReachable == 1) {
							DPRINTF(1, "TORUS NODE REACHABLE : %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E);
							continue;
						} else {
							DPRINTF(1, "TORUS NODE UNREACHABLE : %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E);
							continue;
						}
					}
				}
			}
		}
	}
	
	// 2. Print all groups/supernodes
	dim_t ni;
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
						uint32_t nir = sn_getRank(ni);
						dim_t    n   = sn_getAnchor(ni);
						
						if (ni.raw == sng.this.index.raw) {
							if (phy) n = nd_log2phy(n);
							
							DPRINTF(1, "TORUS GROUP SOURCE : %d %d %d %d %d : %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E,
									sng.size.A[ni.A], sng.size.B[ni.B], sng.size.C[ni.C], sng.size.D[ni.D], sng.size.E[ni.E]);
							
							continue;
						}
						
						if (sng.nodes[nir].pred->flags == 0) {
							if (phy) n = nd_log2phy(n);
							
							DPRINTF(1, "TORUS GROUP REACHABLE : %d %d %d %d %d: %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E,
									sng.size.A[ni.A], sng.size.B[ni.B], sng.size.C[ni.C], sng.size.D[ni.D], sng.size.E[ni.E]);
							
							continue;
						} else {
							if (phy) n = nd_log2phy(n);
							
							DPRINTF(1, "TORUS GROUP UNREACHABLE : %d %d %d %d %d: %d %d %d %d %d\n",
									n.A, n.B, n.C, n.D, n.E,
									sng.size.A[ni.A], sng.size.B[ni.B], sng.size.C[ni.C], sng.size.D[ni.D], sng.size.E[ni.E]);
							
							continue;
						}
					}
				}
			}
		}
	}
	
	
	// 3. Print all links
	dim_t lc;
	for(lc.A = 0; lc.A < torus.size.A; lc.A++) {
		for(lc.B = 0; lc.B < torus.size.B; lc.B++) {
			for(lc.C = 0; lc.C < torus.size.C; lc.C++) {
				for(lc.D = 0; lc.D < torus.size.D; lc.D++) {
					for(lc.E = 0; lc.E < torus.size.E; lc.E++) {
						
						dim_t l;
						uint8_t dim;
						
						// all links
						for(dim = 0; dim < torus.dim; dim++) {
							l.raw = lc.raw << 1;
							l.dim[dim]++;
							
							uint32_t lr = lnk_getRank(l);
							
							uint32_t lm = torus.links[lr].weight.minus;
							uint32_t lp = torus.links[lr].weight.plus;
							
							// unused healthy link.
							if ((lm + lp == 0) && torus.links[lr].status == 0) continue;
							
							//	    if (phy) l = lnk_log2phy(l);
							
							
							dim_t s, d;
							// get both ends of the link l
							if (lm > 0) {
								lnk_getEndpoints(l, &d, &s);
							} else {
								lnk_getEndpoints(l, &s, &d);
							}
							
							char prefix[16];
							char postfix[16];
							
							if (((sng.axes.dim[dim][s.dim[c[dim].dim[1]]][s.dim[c[dim].dim[2]]][s.dim[c[dim].dim[3]]][s.dim[c[dim].dim[4]]] & (1 << s.dim[dim])) == 0) && (torus.links[lr].status == 1)) {
								strcpy(postfix, "IGNORE");
							} else {
								strcpy(postfix, "");
							}
							
							if (phy) {
								s = nd_log2phy(s);
								d = nd_log2phy(d);
							}
							
							if (torus.links[lr].status == 1)
								strcpy(prefix, "LINK BAD");
							else
								strcpy(prefix, "LINK GOOD");
							
							if (d.dim[dim] == 0 && s.dim[dim] == torus.size.dim[dim]-1) {
								dim_t w = s;
								w.dim[dim]++;
								DPRINTF(1, "TORUS %s : %d %d %d %d %d : %d %d %d %d %d : %d %d : WRAP : %s\n",
										prefix, s.A, s.B, s.C, s.D, s.E, w.A, w.B, w.C, w.D, w.E, lp, lm, postfix);
								
								w.dim[dim] = -1;
								DPRINTF(1, "TORUS %s : %d %d %d %d %d : %d %d %d %d %d : %d %d : : %s\n",
										prefix, w.A, w.B, w.C, w.D, w.E, d.A, d.B, d.C, d.D, d.E, lp, lm, postfix);
								
							} else if (s.dim[dim] == 0 && d.dim[dim] == torus.size.dim[dim]-1) {
								dim_t w = s;
								w.dim[dim] = -1;
								DPRINTF(1, "TORUS %s : %d %d %d %d %d : %d %d %d %d %d : %d %d : WRAP : %s\n",
										prefix, s.A, s.B, s.C, s.D, s.E, w.A, w.B, w.C, w.D, w.E, lp, lm, postfix);
								
								w.dim[dim] = torus.size.dim[dim];
								DPRINTF(1, "TORUS %s : %d %d %d %d %d : %d %d %d %d %d : %d %d : : %s\n",
										prefix, w.A, w.B, w.C, w.D, w.E, d.A, d.B, d.C, d.D, d.E, lp, lm, postfix);
							} else {
								DPRINTF(1, "TORUS %s : %d %d %d %d %d : %d %d %d %d %d : %d %d : : %s\n",
										prefix, s.A, s.B, s.C, s.D, s.E, d.A, d.B, d.C, d.D, d.E, lp, lm, postfix);
							}
						}
					}
				}
			}
		}
	}
}
#else
void torus_printLatex(int phy) {}
#endif


void torus_printRoutes() {
	printf("SNG SIZE : %d %d %d %d %d\n",
		   sng.num.A,
		   sng.num.B,
		   sng.num.C,
		   sng.num.D,
		   sng.num.E);

	printf("TORUS DIMENSION COUNT : %d\n", torus.dim);
	printf("TORUS FAILURE COUNT : %d\n", torus.failures);
	
	printf("TORUS COSTS : HOP %d : TURN %d : UNNATURAL TURN %d : WRAP %d\n", COST_HOP, COST_TURN, COST_UNNATURAL_TURN, COST_WRAPAROUND);

	printf("TORUS DIMENSION ORDER : %s %s %s %s %s\n",
		   dim2str[torus.rdo.A], dim2str[torus.rdo.B], dim2str[torus.rdo.C], dim2str[torus.rdo.D], dim2str[torus.rdo.E]);
	
	printf("TORUS SIZE : %d %d %d %d %d\n",
		   torus.size.dim[torus.rdo.A],
		   torus.size.dim[torus.rdo.B],
		   torus.size.dim[torus.rdo.C],
		   torus.size.dim[torus.rdo.D],
		   torus.size.dim[torus.rdo.E]);

	printf("TORUS OFFSET : %d %d %d %d %d\n",
		   torus.offset.dim[torus.rdo.A],
		   torus.offset.dim[torus.rdo.B],
		   torus.offset.dim[torus.rdo.C],
		   torus.offset.dim[torus.rdo.D],
		   torus.offset.dim[torus.rdo.E]);

	dim_t nc;
/*	
	for(nc.A = 0; nc.A < torus.size.A; nc.A++) {
		for(nc.B = 0; nc.B < torus.size.B; nc.B++) {
			for(nc.C = 0; nc.C < torus.size.C; nc.C++) {
				for(nc.D = 0; nc.D < torus.size.D; nc.D++) {
					for(nc.E = 0; nc.E < torus.size.E; nc.E++) {
						uint32_t nr = nd_getRank(nc);
						
						dim_t n = nc;
						n = nd_log2phy(n);
						
						if (nc.raw == torus.local.log.raw) {
							continue; 
						}
						
						if (torus.nodes[nr].flags == 0x4) {
							continue;
						}
						
						dim_t    src   = torus.local.log;
						dim_t    fw    = torus.nodes[nr].fwnode;
						uint16_t hints = torus_fillHintBits(nc, torus.nodes[nr].hint);
						uint8_t  flags = torus.nodes[nr].flags;
						
						src   = nd_log2phy(src);
						fw    = nd_log2phy(fw);
						hints = hint_log2phy(hints);
						
						printf("TORUS ROUTE : %d %d %d %d %d : %d %d %d %d %d : %d %d %d %d %d : %x %x %x %x %x : %x\n",
								src.A, src.B, src.C, src.D, src.E, fw.A, fw.B, fw.C, fw.D, fw.E, n.A, n.B, n.C, n.D, n.E, 
								((hints >> 8) & 0x3),
								((hints >> 6) & 0x3),
								((hints >> 4) & 0x3),
								((hints >> 2) & 0x3),
								((hints >> 0) & 0x3),
								flags);
					}
				}
			}
		}
	}
*/
}
