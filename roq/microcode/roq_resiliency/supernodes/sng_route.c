#include <string.h>
#include <time.h>

#include "sng_common.h"
#include "sng_build.h"
#include "sng_route.h"
#include "torus.h"

#ifdef ROQ_RESILIENCY_BGQ
#include "common.h"
#include <strings.h>

void *memmove(void *dest, const void *src, size_t n) {
	// memcpy != memmove, but it should be close enough. Only differs if dest and src
	// overlap, which I think it never will here.
	return memcpy(dest, src, n);
}
#endif

/* Dijkstra related variables */

// maximum amount of nodes in the border set, basically the number of nodes on the surface
// of the (hyper-)cube with the largest dimension-1.
#define MAX_BORDER  ((MAX_DIM_SIZE-1)*(MAX_DIM_SIZE-1)*(MAX_DIM_SIZE-1)*(MAX_DIM_SIZE-1))*MAX_DIM*2 // 5-D

// maximum amount of nodes in the border set, basically the number of nodes on the surface
// of the (hyper-)cube with the largest dimension.
#define MAX_UNKNOWN (MAX_DIM_SIZE*MAX_DIM_SIZE*MAX_DIM_SIZE*MAX_DIM_SIZE)*MAX_DIM*2 // 5-D


#define HEAP__INSERT       0
#define HEAP__DUMP         0
#define HEAP__FIND         0
#define HEAP__GET_MIN      0
#define HEAP__DECREASE_KEY 0

#define SNG__DUMP_NODES           1
#define SNG__ROUTE                1
#define SNG__ROUTE_PRINT_ALL      1
#define SNG__ROUTE_GET_SUCCESSORS 1
#define SNG__ROUTE_UPDATE         1
#define SNG__ROUTE_CHECK          1
#define SNG__ROUTE_DIJKSTRA       1
#define SNG__ROUTE_APPLY          1

static uint32_t tsm[MAX_NUM_SUPERNODES >> 3]; // Dijkstra's tree set matrix
static uint32_t bsm[MAX_NUM_SUPERNODES >> 3]; // Dijkstra's border set matrix
static uint32_t usm[MAX_NUM_SUPERNODES >> 3]; // Dijkstra's unknown set matrix
static node_heap_t bsh;                // border set heap
//static stack_region_t blocked;         // blocked region stack


void stack_region_init(stack_region_t *stack, uint32_t size) {
  stack->stack = malloc(sizeof(region_t)*size);
  stack->size  = size;
  stack->head  = 0;
}

void stack_region_reset(stack_region_t *stack) {
  stack->head = 0;
}

void stack_region_push(stack_region_t *stack, region_t element) {
  if (stack->head < stack->size-1) {
    stack->stack[stack->head++] = element;
  }
}

int stack_region_pop(stack_region_t *stack, region_t *element) {
  if (stack->head > 0) {
    *element = stack->stack[--stack->head];
    return 1;
  }
  return 0;
}

int stack_region_top(stack_region_t *stack, region_t *element) {
  if (stack->head > 0) {
    *element = stack->stack[stack->head-1];
    return 1;
  }
  return 0;
}

inline uint32_t stack_region_size(stack_region_t *stack) {
  return stack->head;
}


void heap_init(node_heap_t *heap, uint32_t size) {
  heap->heap = malloc(sizeof(node_element_t)*size);
  heap->size = size;
  heap->next = 0;
}

void heap_reset(node_heap_t *heap) {
  heap->next = 0;
}


void heap_insert(node_heap_t *heap, node_element_t element) {
  //	heap_dump(heap);

	DPRINTF(HEAP__INSERT, "Inserting node (%d,%d,%d,%d,%d) with cost %d at position %d\n",
		element.dim.A, element.dim.B, element.dim.C, element.dim.D, element.dim.E, element.cost, heap->next);

	if (heap->next <= heap->size) {
		uint32_t cp = heap->next; // current position = root node
		
		heap->heap[cp] = element; // insert new key at the end of the heap.
		
		while(cp > 0) {
		  uint32_t pp = (cp-1) >> 1; // parent
			
			if (heap->heap[pp].cost > heap->heap[cp].cost) { // swap
				node_element_t temp = heap->heap[pp];
				heap->heap[pp]      = heap->heap[cp];
				heap->heap[cp]      = temp;
			} else {
			  break;
			}
			cp = pp;
		}
		
		heap->next++;
	}

	//	heap_dump(heap);
}

/*
  - ni is the index of the node to be found.
 */

uint32_t heap_find(node_heap_t *heap, dim_t ni) {
	int i;
	
	DPRINTF(HEAP__FIND, "Trying to find node (%d,%d,%d,%d,%d)\n",
			ni.A, ni.B, ni.C, ni.D, ni.E);
	
	for(i = heap->next; i-- >= 0;) {
		DPRINTF(HEAP__FIND, "--> Position %d: (%d,%d,%d,%d,%d) ?= (%d,%d,%d,%d,%d)\n",
				i, ni.A, ni.B, ni.C, ni.D, ni.E, heap->heap[i].dim.A, heap->heap[i].dim.B, heap->heap[i].dim.C, heap->heap[i].dim.D, heap->heap[i].dim.E);
		if (heap->heap[i].dim.raw == ni.raw) return i;
	}
	
	return -1;
}

void heap_decreaseKey(node_heap_t *heap, uint32_t loc, uint8_t cost) {
  //	heap_dump(heap);

  // loc = 0, means that the node already is the root node.
  if (loc == 0) return;

	uint32_t cp = loc;

	if (loc >= heap->next) DPRINTF(1, "ERROR, expected loc < %d, actual loc = %d\n", heap->next, loc);
	DPRINTF(HEAP__DECREASE_KEY, "Decreasing key of node at position %d from %d to %d\n", loc,   heap->heap[loc].cost, cost);
	
	heap->heap[cp].cost = cost;
	
	uint32_t pp = (cp-1) >> 1; // pp = parent position
	
	while(cp > 0 && heap->heap[cp].cost < heap->heap[pp].cost) {
		// swap current position with right child position
		node_element_t temp = heap->heap[cp];
		heap->heap[cp]      = heap->heap[pp];
		heap->heap[pp]      = temp;
		
		cp = pp;
		pp = (cp-1) >> 1;
	}
	
	//	heap_dump(heap);
}

node_element_t heap_getMin(node_heap_t *heap) {
	
  //  heap_dump(heap);
	uint32_t cp = 0;
	
	// 1. remove smallest element
	node_element_t min = heap->heap[0];
	
	heap->next--;
	
	// replace root with last element.
	heap->heap[0] = heap->heap[heap->next];
	
	// sink down new root until it reaches its final position.
	while (1) {
		uint32_t lp = (cp << 1) + 1; // left child position
		uint32_t rp = (cp << 1) + 2; // right child position
		uint32_t np;
		
		
		if (lp <= heap->next && heap->heap[lp].cost < heap->heap[rp].cost) { // left child is smaller than right.
			//      DPRINTF(HEAP__GET_MIN, "follow left child: %d <=> %d\n", heap->heap[cp], heap->heap[lp]);
			np = lp;
		} else if (rp <= heap->next) { // right child is smaller than left child.
			//      DPRINTF(HEAP__GET_MIN, "follow right child: %d <=> %d\n", heap->heap[cp], heap->heap[rp]);
			np = rp;
		} else {
			// this means the element sank completely down to the bottom.
			//      DPRINTF(HEAP__GET_MIN, "Current position is %d, children are %d and %d\n", cp, lp, rp);
			break;
		}
		
		if (heap->heap[cp].cost < heap->heap[lp].cost &&
			heap->heap[cp].cost < heap->heap[rp].cost) {
			// element has reached its final position
			break;
		}
		
		// swap current position with right child position
		node_element_t temp = heap->heap[cp];
		heap->heap[cp]      = heap->heap[np];
		heap->heap[np]      = temp;
		
		cp = np;
	}
	
	DPRINTF(HEAP__GET_MIN, "Minimal node is (%d,%d,%d,%d,%d) with cost %d\n",
			min.dim.A, min.dim.B, min.dim.C, min.dim.D, min.dim.E, min.cost);
	
	//	heap_dump(heap);
	return min;
}


void heap_dump(node_heap_t *heap) {
	uint32_t i;
	PRINTF(HEAP__DUMP,"\n");
	for(i = 0; i < heap->next; i++) {
		DPRINTF(HEAP__DUMP, "heap[%d] = (%d,%d,%d,%d,%d) with cost %d\n",
				i, heap->heap[i].dim.A, heap->heap[i].dim.B, heap->heap[i].dim.C, heap->heap[i].dim.D, heap->heap[i].dim.E,
				heap->heap[i].cost);

		uint32_t child1 = 2*(i+1) - 1;
		uint32_t child2 = 2*(i+1);

		if (child1 < heap->next) {
		  if (! (heap->heap[i].cost <= heap->heap[child1].cost)) {
		    DPRINTF(HEAP__DUMP, "ERROR: heap[%d] = (%d,%d,%d,%d,%d) with cost %d, heap[%d] = (%d,%d,%d,%d,%d) with cost %d\n",
			    i, heap->heap[i].dim.A, heap->heap[i].dim.B, heap->heap[i].dim.C, heap->heap[i].dim.D, heap->heap[i].dim.E,
			    heap->heap[i].cost,
			    child1, heap->heap[child1].dim.A, heap->heap[child1].dim.B, heap->heap[child1].dim.C, heap->heap[child1].dim.D, heap->heap[child1].dim.E,
			    heap->heap[child1].cost);
		  }
		}

		if (child2 < heap->next) {
		  if (! (heap->heap[i].cost <= heap->heap[child2].cost)) {
		    DPRINTF(HEAP__DUMP, "ERROR: heap[%d] = (%d,%d,%d,%d,%d) with cost %d, heap[%d] = (%d,%d,%d,%d,%d) with cost %d\n",
			    i, heap->heap[i].dim.A, heap->heap[i].dim.B, heap->heap[i].dim.C, heap->heap[i].dim.D, heap->heap[i].dim.E,
			    heap->heap[i].cost,
			    child2, heap->heap[child2].dim.A, heap->heap[child2].dim.B, heap->heap[child2].dim.C, heap->heap[child2].dim.D, heap->heap[child2].dim.E,
			    heap->heap[child2].cost);
		  }
		}
	}
}


inline void sng_routeDijkstraAddToSet(uint32_t *set, uint32_t rank) {
	set[rank >> 5] |= (1 << (rank & 0x1F));
}

inline void sng_routeDijkstraRemoveFromSet(uint32_t *set, uint32_t rank) {
	set[rank >> 5] &= ~(1 << (rank & 0x1F));
}

inline int sng_routeDijkstraIsInSet(uint32_t* set, uint32_t rank) {
	return (set[rank >> 5] & (1 << (rank & 0x1F))) != 0;
}



/* Prints out all routes for every existing supernode */
void sng_routePrintAll() {
	dim_t ci; // current supernode numbers
	DPRINTF(SNG__ROUTE_PRINT_ALL, "Source supernode is [%d,%d,%d,%d,%d]\n", sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E);
	
	DPRINTF(SNG__ROUTE_PRINT_ALL, "Reachable Supernodes:\n");
	for(ci.A = 0; ci.A < sng.num.A; ci.A++) {
		for(ci.B = 0; ci.B < sng.num.B; ci.B++) {
			for(ci.C = 0; ci.C < sng.num.C; ci.C++) {
				for(ci.D = 0; ci.D < sng.num.D; ci.D++) {
					for(ci.E = 0; ci.E < sng.num.E; ci.E++) {
					
						uint32_t cir = sn_getRank(ci); // current supernode rank
						
						dim_t pi = sng_getNeighbor(ci, sng.nodes[cir].pred->dir);
						
						
						if ((tsm[cir >> 5] & (1 << (cir & 0x1F))) != 0) {
							DPRINTF(SNG__ROUTE_PRINT_ALL, "[%d,%d,%d,%d,%d] -- %-2s/%d --> [%d,%d,%d,%d,%d]\n",
									pi.A, pi.B, pi.C, pi.D, pi.E, dir2inv2str[__builtin_ffs(sng.nodes[cir].pred->dir)-1], (sng.nodes[cir].pred->dir != 0 ? __builtin_ffs(sng.nodes[cir].pred->dir)-1 : 0),
									ci.A, ci.B, ci.C, ci.D, ci.E);
						}
					}
				}
			}
		}
	}

	DPRINTF(SNG__ROUTE_PRINT_ALL, "Unreachable Supernodes:\n");
	for(ci.A = 0; ci.A < sng.num.A; ci.A++) {
		for(ci.B = 0; ci.B < sng.num.B; ci.B++) {
			for(ci.C = 0; ci.C < sng.num.C; ci.C++) {
				for(ci.D = 0; ci.D < sng.num.D; ci.D++) {
					for(ci.E = 0; ci.E < sng.num.E; ci.E++) {
						
						uint32_t cir = sn_getRank(ci); // current supernode rank
						
						dim_t pi = sng_getNeighbor(ci, sng.nodes[cir].pred->dir);
						
						
						if ((tsm[cir >> 5] & (1 << (cir & 0x1F))) == 0) {
							DPRINTF(SNG__ROUTE_PRINT_ALL, "[%d,%d,%d,%d,%d] -- %-2s/%d --> [%d,%d,%d,%d,%d]\n",
									pi.A, pi.B, pi.C, pi.D, pi.E, dir2inv2str[__builtin_ffs(sng.nodes[cir].pred->dir)-1], (sng.nodes[cir].pred->dir != 0 ? __builtin_ffs(sng.nodes[cir].pred->dir)-1 : 0),
									ci.A, ci.B, ci.C, ci.D, ci.E);
						}
					}
				}
			}
		}
	}

	DPRINTF(SNG__ROUTE_PRINT_ALL, "Routes from source supernode [%d,%d,%d,%d,%d] are:\n", sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E);
	for(ci.A = 0; ci.A < sng.num.A; ci.A++) {
		for(ci.B = 0; ci.B < sng.num.B; ci.B++) {
			for(ci.C = 0; ci.C < sng.num.C; ci.C++) {
				for(ci.D = 0; ci.D < sng.num.D; ci.D++) {
					for(ci.E = 0; ci.E < sng.num.E; ci.E++) {
	  
						uint32_t cir = sn_getRank(ci); // current supernode rank
						dim_t    pi  = sng_getNeighbor(ci, sng.nodes[cir].pred->dir);
						
						if ((usm[cir >> 5] & (1 << (cir & 0x1F))) != 0) {
							DPRINTF(SNG__ROUTE_PRINT_ALL, "[%d,%d,%d,%d,%d] ----------------> [%d,%d,%d,%d,%d]\n",
									sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E,
									ci.A, ci.B, ci.C, ci.D, ci.E);
						} else {
							DPRINTF(SNG__ROUTE_PRINT_ALL, "[%d,%d,%d,%d,%d] --> [%d,%d,%d,%d,%d] --> [%d,%d,%d,%d,%d]\n",
									sng.this.index.A, sng.this.index.B, sng.this.index.C, sng.this.index.D, sng.this.index.E,
									sng.nodes[cir].pred->fwnode.A, sng.nodes[cir].pred->fwnode.B, sng.nodes[cir].pred->fwnode.C, sng.nodes[cir].pred->fwnode.D, sng.nodes[cir].pred->fwnode.E,
									ci.A, ci.B, ci.C, ci.D, ci.E);
						}
					}
				}
			}
		}
	}
}

/* 
   Returns cost of the path from supernode 'si' to adjacent supernode 'di'

   - 'si'    is the index of the source supernode (of that one hop)
   - 'di'    is the index of the destination supernode (of that one hop)
   - 'dir'   is the direction of the link between 'si' and 'di'
   - 'flags' 0x1 = unnatural turn, 0x2 = wraparound links
*/
uint8_t sng_routeGetCost(dim_t si, dim_t di, uint8_t dir, uint8_t *flags, uint8_t *dim) {
	uint8_t  d  = dir >> 1;
	
	//  dim_t    sc = sn_getAnchor(si);
	uint32_t sr = sn_getRank(si);
	
	// 1. number of links between the two supernodes. Not entierly correct yet.
	//  uint8_t csi  = sn_getCoord(d, si.dim[d]) + sn_getSize(d, si.dim[d]) >> 1;
	//  uint8_t cdi  = sn_getCoord(d, di.dim[d]) + sn_getSize(d, di.dim[d]) >> 1;
	//  uint8_t num_hop = abs(csi-cdi)+1;
	
	// The routing gets inaccurate in two cases, which I'm trying to solve here:
	// 1. If a supernode has an even number of nodes in a direction, there is no exact
	//    center, thus always prefering the path via the lower region of the supernode
	//    graph. This is obviously bad. As I don't want to use floating point, I simply
	//    double to costs, so that I can always get a integer result.
	// 2. If the source node is not in the center of the source supernode, the paths get
	//    distored. Every supernode that shares a coordinate with the source supernode
	//    won't use the center as reference point (in this dimension) but the source
	//    node's actual coordinate.
	
	uint8_t sss; // source supernode size
	uint8_t dss; // destination supernode size;
	
	if (si.dim[d] == sng.this.index.dim[d]) {
		// if the source supernode's dimension is the same a the source supernodes's (the one
		// that contains the source node) dimension, pretend that the source node is the
		// center of that supernode and calculate the size accordingly.
		if (is_plus(dir))
			sss = ((sn_getMax(d, si.dim[d]) - (torus.local.log.dim[d] - sn_getCoord(d, si.dim[d]))) << 1) + 1;
		else
			sss = ((torus.local.log.dim[d] - sn_getCoord(d, si.dim[d])) << 1) + 1;
	} else {
		sss = sn_getSize(d, si.dim[d]);
	}
	
	dss = sn_getSize(d, di.dim[d]);
	uint8_t num_hop = sss + dss;
	

	uint8_t  num_turn           = 0;
	uint8_t  num_unnatural_turn = 0;
	uint8_t  num_wraparound     = 0;
	uint32_t cost[2];

	// 2. turn costs: only dimensions, not directions are important.

	// As the source node has no predecessor, there cannot be any kind of turn to an
	// immediate successor of the source node.
	if (si.raw != sng.this.index.raw) {
	  num_turn           = ((__builtin_ffs(sng.nodes[sr].pred->dir)-1) >> 1) != d;
	  num_unnatural_turn = ((__builtin_ffs(sng.nodes[sr].pred->dir)-1) >> 1)  > d;
	}
	
	// 3. wrap-around costs:
	num_wraparound = (si.dim[d] == 0 && is_minus(dir)) || (di.dim[d] == 0 && is_plus(dir));
	
	// Costs via the selected predecessor of this node
	cost[0]  = ((num_hop            * COST_HOP)            +
		    (num_turn           * COST_TURN)           +
		    (num_unnatural_turn * COST_UNNATURAL_TURN) +
		    (num_wraparound     * COST_WRAPAROUND));
	cost[0] += sng.nodes[sr].pred->cost;
	
	// Costs via the same-dimension predecessor of this node.
	cost[1]  = ((num_hop            * COST_HOP)            +
		    (num_wraparound     * COST_WRAPAROUND));
	cost[1] += sng.nodes[sr].dim[d].cost;

	*flags = num_wraparound << 1; // this is the same, no matter what predecessor we're selecting

	DPRINTF(SNG__ROUTE_DIJKSTRA, "--[%s/%d]--> [%d,%d,%d,%d,%d] --[%s/%d]--> [%d,%d,%d,%d,%d] (best)\n",
		dir2str[(__builtin_ffs(sng.nodes[sr].pred->dir)-1)], sng.nodes[sr].pred->cost, si.A, si.B, si.C, si.D, si.E,
		dir2str[dir],                                        cost[0],                  di.A, di.B, di.C, di.D, di.E);

	DPRINTF(SNG__ROUTE_DIJKSTRA, "--[%s/%d]--> [%d,%d,%d,%d,%d] --[%s/%d]--> [%d,%d,%d,%d,%d] (same)\n",
		dir2str[(__builtin_ffs(sng.nodes[sr].dim[d].dir)-1)], sng.nodes[sr].dim[d].cost, si.A, si.B, si.C, si.D, si.E,
		dir2str[dir],                                         cost[1],                   di.A, di.B, di.C, di.D, di.E);

	if (cost[0] < cost[1]) {
	  // select this node's predecessor as the next node's pre-predecessor
	  *flags |= num_unnatural_turn;
	  *dim    = sng.nodes[sr].pred->dir == 0 ? 0 : (__builtin_ffs(sng.nodes[sr].pred->dir)-1) >> 1;

	  DPRINTF(SNG__ROUTE_DIJKSTRA, "[%d,%d,%d,%d,%d] --[%s]--> [%d,%d,%d,%d,%d] : num_hop = %d.%d(%d), num_turn = %d(%d), num_unnatural_turn = %d(%d), num_wraparound = %d(%d), cost = %d(%d), flags = %x, sr.pred = %s, pred = %s (best pred)\n",
		  si.A, si.B, si.C, si.D, si.E, dir2str[dir], di.A, di.B, di.C, di.D, di.E,
		  num_hop >> 1, 5*(num_hop & 1), COST_HOP,
		  num_turn, COST_TURN,
		  num_unnatural_turn, COST_UNNATURAL_TURN,
		  num_wraparound, COST_WRAPAROUND, 
		  cost[0], cost[1], *flags, dim2str[(__builtin_ffs(sng.nodes[sr].pred->dir)-1) >> 1], dim2str[d]);

	  return cost[0];
	} else {
	  *dim    = d;

	  DPRINTF(SNG__ROUTE_DIJKSTRA, "[%d,%d,%d,%d,%d] --[%s]--> [%d,%d,%d,%d,%d] : num_hop = %d.%d(%d), num_turn = %d(%d), num_unnatural_turn = %d(%d), num_wraparound = %d(%d), cost = %d(%d), sr.pred = %s, pred = %s (same dim)\n",
		  si.A, si.B, si.C, si.D, si.E, dir2str[dir], di.A, di.B, di.C, di.D, di.E,
		  num_hop >> 1, 5*(num_hop & 1), COST_HOP,
		  0, COST_TURN,
		  0, COST_UNNATURAL_TURN,
		  num_wraparound, COST_WRAPAROUND,
		  cost[1], cost[0], dim2str[(__builtin_ffs(sng.nodes[sr].pred->dir)-1) >> 1], dim2str[d]);
	  return cost[1];
	}
}


/*
  Checks if the link in direction 'dir' of supernode 'nc' is ok.

  - 'nc' are the coordinates of the supernode
  - 'ni' is the index of the supernode
  - 'dir' is the link direction w.r.t. 'nc'.

  - returns 0 for a good link and 1 for a bad link.
 */
int sng_getLinkStatus(dim_t nc, dim_t ni, uint8_t dir) {
	uint8_t  d = dir >> 1;
	
	uint32_t axis = sng.axes.dim[d][nc.dim[c[d].dim[1]]][nc.dim[c[d].dim[2]]][nc.dim[c[d].dim[3]]][nc.dim[c[d].dim[4]]]; // axis of the output link
	uint8_t  lp;

	if (torus.closed.dim[d] == 0) {
		// in a mesh, all wrap around links are "broken". Moreover for mesh dimensions,
		// physical = logical coordinates!
		if (is_minus(dir) && ni.dim[d] == 0)                return 2;
		if (is_plus(dir)  && ni.dim[d] == sng.num.dim[d]-1) return 2;
	}
	
	if (is_minus(dir)) {
		if (nc.dim[d] == 0) lp = torus.size.dim[d] - 1;
		else                lp = nc.dim[d]         - 1;
	} else {
		lp = nc.dim[d] + sn_getSize(d, ni.dim[d]) - 1;
	}
	
	DPRINTF(SNG__ROUTE_DIJKSTRA, "link [%d,%d,%d,%d,%d] %s: size = %d, lp = %d, sng.axes.dim[%d][%d][%d][%d][%d] = %08x, lp = %08x, is_minus(%d) = %d\n",
			ni.A, ni.B, ni.C, ni.D, ni.E, dir2str[dir], sn_getSize(d, ni.dim[d]), lp, d, nc.dim[c[d].dim[1]], nc.dim[c[d].dim[2]], nc.dim[c[d].dim[3]], nc.dim[c[d].dim[4]], axis, 1 << lp, dir, is_minus(dir));
	
	return ((axis & (1 << lp)) != 0);
}


void sng_routeDijkstraTraverseLink(dim_t cc, dim_t ci, uint32_t cir, uint16_t dir) {
  // get neighbor node
  uint8_t  d   = dir >> 1;
  dim_t    ni  = sng_getNeighbor(ci, 1 << dir);
  uint32_t nir = sn_getRank(ni);
  
  // skip nodes that are already in the tree set.
  if (sng_routeDijkstraIsInSet(tsm, nir)) return;
  
  // check link health to neigbor node
  int ls = sng_getLinkStatus(cc, ci, dir);
  if (ls == 2) {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "--> [%d,%d,%d,%d,%d] -- %s --> [%d,%d,%d,%d,%d] does not exist (mesh).\n",
	    ci.A, ci.B, ci.C, ci.D, ci.E, dir2str[dir], ni.A, ni.B, ni.C, ni.D, ni.E);
    return;
  } else if (ls != 0) {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "--> [%d,%d,%d,%d,%d] -- %s --> [%d,%d,%d,%d,%d] is blocked.\n",
	    ci.A, ci.B, ci.C, ci.D, ci.E, dir2str[dir], ni.A, ni.B, ni.C, ni.D, ni.E);
    return;
  } else {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "--> [%d,%d,%d,%d,%d] -- %s --> [%d,%d,%d,%d,%d] is free.\n",
	    ci.A, ci.B, ci.C, ci.D, ci.E, dir2str[dir], ni.A, ni.B, ni.C, ni.D, ni.E);
  }
  
  uint8_t flags = 0;
  uint8_t pd    = 0;
  uint8_t cost  = sng_routeGetCost(ci, ni, dir, &flags, &pd);

  // backup for later comparison
  uint8_t cur_cost  = sng.nodes[nir].pred->cost;
  uint8_t cur_flags = sng.nodes[nir].pred->flags;

  if ((cost < sng.nodes[nir].dim[d].cost) ||
      ((cost == sng.nodes[nir].dim[d].cost) && (flags & 0x2) == 0 && (sng.nodes[nir].dim[d].flags & 0x2) != 0)) {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "Updating route to [%d,%d,%d,%d,%d] in %s dimension as costs are lower (%d < %d) or flags are better (%x vs. %x). New predecessor is %s\n",
    	    ni.A, ni.B, ni.C, ni.D, ni.E, dim2str[d], cost, sng.nodes[nir].dim[d].cost, flags, sng.nodes[nir].dim[d].flags, dir2str[dir2inv[dir]]);

    sng.nodes[nir].dim[d].cost   = cost;
    sng.nodes[nir].dim[d].dir    = 1 << dir2inv[dir];
    sng.nodes[nir].dim[d].flags  = sng.nodes[cir].dim[pd].flags;
    sng.nodes[nir].dim[d].fwnode = sng.nodes[cir].dim[pd].fwnode;
    sng.nodes[nir].dim[d].hint   = sng.nodes[cir].dim[pd].hint;
    // Only if no forwarding node has been set so far, use information from the current link
    if ((sng.nodes[nir].dim[d].flags & 0x1) == 0) {
      if (flags == 0x2) {
	sng.nodes[nir].dim[d].flags  |= 0x2;
      }
      if (flags & 0x1) {
	sng.nodes[nir].dim[d].flags  |= 0x1;
	sng.nodes[nir].dim[d].fwnode  = ci;
      } else {
	sng.nodes[nir].dim[d].hint   |= dir2hint[dir];
      }
    }
  } else {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "Not updating route to [%d,%d,%d,%d,%d] in %s dimension as costs are higher (%d > %d) and flags aren't better (%x vs. %x).\n",
    	    ni.A, ni.B, ni.C, ni.D, ni.E, dim2str[d], cost, sng.nodes[nir].dim[d].cost, flags, sng.nodes[nir].dim[d].flags);
  }
  
  // update path if costs are lower or if costs are the same but flags are better.
  if (((cost <  cur_cost)) || 
      ((cost == cur_cost) && (flags & 0x2) == 0 && (cur_flags & 0x2) != 0)) {
    // costs are lower, update path and costs to this node.

    sng.nodes[nir].pred = &sng.nodes[nir].dim[d];
    
    if (! sng_routeDijkstraIsInSet(bsm, nir)) {
      DPRINTF(SNG__ROUTE_DIJKSTRA, "Adding node [%d,%d,%d,%d,%d] to the border set: cost = %d, predecessor = %s, flags = %x, hint = %x, fwnode = [%d,%d,%d,%d,%d]\n",
	      ni.A, ni.B, ni.C, ni.D, ni.E, sng.nodes[nir].pred->cost, dir2str[__builtin_ffs(sng.nodes[nir].pred->dir)-1], sng.nodes[nir].pred->flags, sng.nodes[nir].pred->hint,
	      sng.nodes[nir].pred->fwnode.A, sng.nodes[nir].pred->fwnode.B, sng.nodes[nir].pred->fwnode.C, sng.nodes[nir].pred->fwnode.D, sng.nodes[nir].pred->fwnode.E);
      
      heap_insert(&bsh, (node_element_t){ni, cost});
      sng_routeDijkstraAddToSet(bsm, nir);
    } else {
      DPRINTF(SNG__ROUTE_DIJKSTRA, "Updating node [%d,%d,%d,%d,%d] in the border set: cost = %d, predecessor = %s, flags = %x, hint = %x, fwnode = [%d,%d,%d,%d,%d]\n",
	      ni.A, ni.B, ni.C, ni.D, ni.E, sng.nodes[nir].pred->cost, dir2str[__builtin_ffs(sng.nodes[nir].pred->dir)-1], sng.nodes[nir].pred->flags, sng.nodes[nir].pred->hint,
	      sng.nodes[nir].pred->fwnode.A, sng.nodes[nir].pred->fwnode.B, sng.nodes[nir].pred->fwnode.C, sng.nodes[nir].pred->fwnode.D, sng.nodes[nir].pred->fwnode.E);
      
      heap_decreaseKey(&bsh, heap_find(&bsh, ni), cost);
    }
  } else {
    DPRINTF(SNG__ROUTE_DIJKSTRA, "--> Costs via [%d,%d,%d,%d,%d] -- %s --> [%d,%d,%d,%d,%d] are higher (%d > %d)\n",
	    ci.A, ci.B, ci.C, ci.D, ci.E, dir2str[dir], ni.A, ni.B, ni.C, ni.D, ni.E, cost, sng.nodes[nir].pred->cost);
  }
}

/*
  Executes the Dijkstra SSSP algorithm using the tree set 'ts' and the border set 'bs'.
 */
void sng_routeDijkstraRun(dim_t si, node_heap_t *bsh) {
  heap_dump(bsh);
  while(bsh->next > 0) {
    // 1. get node with the least costs.
    node_element_t next = heap_getMin(bsh);
    dim_t    ci   = next.dim;
    uint8_t  cost = next.cost;
    dim_t    cc   = sn_getAnchor(ci);
    uint32_t cir  = sn_getRank(ci);
    
    // 2. add it to the set of tree nodes, remove it from the border
    sng_routeDijkstraAddToSet(tsm, cir);
    sng_routeDijkstraRemoveFromSet(bsm, cir);
	sng_routeApply(ci, cir);
    
    DPRINTF(SNG__ROUTE_DIJKSTRA, "Adding node [%d,%d,%d,%d,%d] to the tree set. Its cost are %d, its predecessor is %s\n",
	    ci.A, ci.B, ci.C, ci.D, ci.E, cost, dir2str[__builtin_ffs(sng.nodes[cir].pred->dir)-1]);
    
    // 3. Update border set with neighbors of 'ci'
    uint8_t dim;
    for (dim = 0; dim < torus.dim; dim++) {
      if (ci.dim[dim] == 0) { // minus link is wrap-around
	sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1) + 1); // take the inner link first...
	sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1));     // ...then the wrap-around link
      } else { // plus link might be a wrap-around link
	sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1));     // take the inner link first...
	sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1) + 1); // ...then the wrap-around link
      }
    }
  }		
}

/* Initialize border and tree set */
void sng_routeDijkstraInitialize(node_heap_t *bsh) {
	dim_t    ci  = sng.this.index;
	dim_t    cc  = sng.this.coord;
	uint32_t cir = sng.this.rank;

	// 1. Add source supernode to the tree set
	bzero(&sng.nodes[cir], sizeof(supernode_t));
	sng.nodes[cir].pred = &sng.nodes[cir].dim[0];

	sng_routeDijkstraAddToSet(tsm, cir); // add source not to tree set.
	sng_routeApply(ci, cir);
	
	DPRINTF(SNG__ROUTE_DIJKSTRA, "Adding node [%d,%d,%d,%d,%d] to the tree set: cost = %d, predecessor = N/A, flags = %x, hint = %x, fwnode = [%d,%d,%d,%d,%d]\n",
			ci.A, ci.B, ci.C, ci.D, ci.E, sng.nodes[cir].pred->cost, sng.nodes[cir].pred->flags, sng.nodes[cir].pred->hint,
			sng.nodes[cir].pred->fwnode.A, sng.nodes[cir].pred->fwnode.B, sng.nodes[cir].pred->fwnode.C, sng.nodes[cir].pred->fwnode.D, sng.nodes[cir].pred->fwnode.E);
	
	// 2. Add all direct neighbors of the source supernode to the border set.
	uint8_t dim;
	for (dim = 0; dim < torus.dim; dim++) {
	  if (ci.dim[dim] == 0) { // minus link is wrap-around
	    sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1) + 1); // take the inner link first...
	    sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1));     // ...then the wrap-around link
	  } else { // plus link might be a wrap-around link
	    sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1));     // take the inner link first...
	    sng_routeDijkstraTraverseLink(cc, ci, cir, (dim << 1) + 1); // ...then the wrap-around link
	  }
	}
}


void sng_route() {
	/*
	  I slightly modified the Dijkstra algorithm to make use of already existing information.
	  
	  - The tree set is initialized with the set of all reachable supernodes.
	  
	  - One could also add supernodes that are not directly reachable but already have a
        known (and unchanged, compared to the last run) shortest path.
	*/
	
	dim_t si = sng.this.index;
	
	// 1. Initialize border and tree set
	sng_routeDijkstraInitialize(&bsh);

	// 2. Run Dijkstra
	sng_routeDijkstraRun(si, &bsh);

	// 3. Find unreachable nodes and mark them
	sng_routeFindCutOffNodes();
}


void sng_routeSetRoute(dim_t di, uint32_t dir, int flags) {
	dim_t si = sng.this.index;        // source node index
	dim_t ii = sng.nodes[dir].pred->fwnode; // intermediate node index
	
	dim_t lo; lo.raw = 0;
	dim_t hi; hi.raw = 0;
	
	uint16_t hint = sng.nodes[dir].pred->hint; // manually forced hint bits
	
	// Apply restrictions
	if (flags & 0x1) {
		int d;
		for (d = torus.dim; d-- > 0;) {
			// source-intermediate restrictions
			if (si.dim[d] == ii.dim[d]) {
				lo.dim[d] = torus.local.log.dim[d];
				hi.dim[d] = torus.local.log.dim[d];
			} else {
				lo.dim[d] = sng.anchor.dim[d][ii.dim[d]];
				hi.dim[d] = sng.anchor.dim[d][ii.dim[d]] + sng.size.dim[d][ii.dim[d]]-1;
			}
			
			// intermediate-destination restrictions
			
			if (ii.dim[d] < si.dim[d] && ii.dim[d] < di.dim[d]) {
				lo.dim[d] = hi.dim[d];
			} else if (ii.dim[d] > si.dim[d] && ii.dim[d] > di.dim[d]) {
				hi.dim[d] = lo.dim[d];
			}
		}
		
		DPRINTF(SNG__ROUTE_APPLY, "--> Box (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d)\n",
				lo.A, lo.B, lo.C, lo.D, lo.E, hi.A, hi.B, hi.C, hi.D, hi.E);
	}
	
	
	// scaling
	
	// TODO: Get the intermediate nodes to interleave, depending on the coordinates of the
	//       source node (even/odd)
	dim_t dso; // destination supernode offset
	
	for(dso.E = sng.size.E[di.E]; dso.E-- > 0;) {
		for(dso.D = sng.size.D[di.D]; dso.D-- > 0;) {
			for(dso.C = sng.size.C[di.C]; dso.C-- > 0;) {
				for(dso.B = sng.size.B[di.B]; dso.B-- > 0;) {
					for(dso.A = sng.size.A[di.A]; dso.A-- > 0;) {
						
						dim_t dc;
						dc.A = dso.A + sng.anchor.A[di.A];
						dc.B = dso.B + sng.anchor.B[di.B];
						dc.C = dso.C + sng.anchor.C[di.C];
						dc.D = dso.D + sng.anchor.D[di.D];
						dc.E = dso.E + sng.anchor.E[di.E];
						
						dim_t ic;
						if (flags & 0x1) {
							ic.A = lo.A + ((hi.A-lo.A) * (dso.A + 1)) / sng.size.A[di.A];
							ic.B = lo.B + ((hi.B-lo.B) * (dso.B + 1)) / sng.size.B[di.B];
							ic.C = lo.C + ((hi.C-lo.C) * (dso.C + 1)) / sng.size.C[di.C];
							ic.D = lo.D + ((hi.D-lo.D) * (dso.D + 1)) / sng.size.D[di.D];
							ic.E = lo.E + ((hi.E-lo.E) * (dso.E + 1)) / sng.size.E[di.E];
						}

						torus_setRoute(dc, ic, hint, flags);
					}
				}
			}
		}
	}
}


void sng_routeSetIntermediateNode(dim_t di, uint32_t dir) {
	dim_t si = sng.this.index;        // source node index
	dim_t ii = sng.nodes[dir].pred->fwnode; // intermediate node index
	
	dim_t lo; lo.raw = 0;
	dim_t hi; hi.raw = 0;
	
	// Apply restrictions
	int d;
	for (d = torus.dim; d-- > 0;) {
		// source-intermediate restrictions
		if (si.dim[d] == ii.dim[d]) {
			lo.dim[d] = torus.local.log.dim[d];
			hi.dim[d] = torus.local.log.dim[d];
		} else {
			lo.dim[d] = sng.anchor.dim[d][ii.dim[d]];
			hi.dim[d] = sng.anchor.dim[d][ii.dim[d]] + sng.size.dim[d][ii.dim[d]]-1;
		}
		
		// intermediate-destination restrictions
		
		if (ii.dim[d] < si.dim[d] && ii.dim[d] < di.dim[d]) {
			lo.dim[d] = hi.dim[d];
		} else if (ii.dim[d] > si.dim[d] && ii.dim[d] > di.dim[d]) {
			hi.dim[d] = lo.dim[d];
		}
		
	}

	DPRINTF(SNG__ROUTE_APPLY, "--> Box (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d)\n",
			lo.A, lo.B, lo.C, lo.D, lo.E, hi.A, hi.B, hi.C, hi.D, hi.E);
	
	// scaling
	
	// TODO: Get the intermediate nodes to interleave, depending on the coordinates of the
	//       source node (even/odd)
	dim_t dso; // destination supernode offset
	
	for(dso.E = sng.size.E[di.E]; dso.E-- > 0;) {
		for(dso.D = sng.size.D[di.D]; dso.D-- > 0;) {
			for(dso.C = sng.size.C[di.C]; dso.C-- > 0;) {
				for(dso.B = sng.size.B[di.B]; dso.B-- > 0;) {
					for(dso.A = sng.size.A[di.A]; dso.A-- > 0;) {
						
						dim_t dc;
						dc.A = dso.A + sng.anchor.A[di.A];
						dc.B = dso.B + sng.anchor.B[di.B];
						dc.C = dso.C + sng.anchor.C[di.C];
						dc.D = dso.D + sng.anchor.D[di.D];
						dc.E = dso.E + sng.anchor.E[di.E];

						dim_t ic;
						ic.A = lo.A + ((hi.A-lo.A) * (dso.A + 1)) / sng.size.A[di.A];
						ic.B = lo.B + ((hi.B-lo.B) * (dso.B + 1)) / sng.size.B[di.B];
						ic.C = lo.C + ((hi.C-lo.C) * (dso.C + 1)) / sng.size.C[di.C];
						ic.D = lo.D + ((hi.D-lo.D) * (dso.D + 1)) / sng.size.D[di.D];
						ic.E = lo.E + ((hi.E-lo.E) * (dso.E + 1)) / sng.size.E[di.E];
						
						torus_setIntermediateNode(dc, ic);
					}
				}
			}
		}
	}
}


void sng_routeSetHintBits(dim_t di, uint32_t dir) {
	uint16_t hint = sng.nodes[dir].pred->hint; // manually forced hint bits
	
	DPRINTF(SNG__ROUTE_APPLY, "supernode [%d,%d,%d,%d,%d], size (%d,%d,%d,%d,%d), hint bits = %02x\n",
			di.A, di.B, di.C, di.D, di.E, 
			sng.size.A[di.A], sng.size.B[di.B], sng.size.C[di.C], sng.size.D[di.D], sng.size.E[di.E],
			hint);
	
	dim_t dso; // destination supernode offset
	
	for(dso.E = sng.size.E[di.E]; dso.E-- > 0;) {
		for(dso.D = sng.size.D[di.D]; dso.D-- > 0;) {
			for(dso.C = sng.size.C[di.C]; dso.C-- > 0;) {
				for(dso.B = sng.size.B[di.B]; dso.B-- > 0;) {
					for(dso.A = sng.size.A[di.A]; dso.A-- > 0;) {
						
						dim_t dc;
						dc.A = dso.A + sng.anchor.A[di.A];
						dc.B = dso.B + sng.anchor.B[di.B];
						dc.C = dso.C + sng.anchor.C[di.C];
						dc.D = dso.D + sng.anchor.D[di.D];
						dc.E = dso.E + sng.anchor.E[di.E];
						
						torus_setHintBits(dc, hint);
					}
				}
			}
		}
	}
}


void sng_routeSetCutOff(dim_t di) {
	DPRINTF(SNG__ROUTE_APPLY, "supernode [%d,%d,%d,%d,%d], size (%d,%d,%d,%d,%d), cut off!\n",
			di.A, di.B, di.C, di.D, di.E, 
			sng.size.A[di.A], sng.size.B[di.B], sng.size.C[di.C], sng.size.D[di.D], sng.size.E[di.E]);
	
	dim_t dso; // destination supernode offset
	
	for(dso.E = sng.size.E[di.E]; dso.E-- > 0;) {
		for(dso.D = sng.size.D[di.D]; dso.D-- > 0;) {
			for(dso.C = sng.size.C[di.C]; dso.C-- > 0;) {
				for(dso.B = sng.size.B[di.B]; dso.B-- > 0;) {
					for(dso.A = sng.size.A[di.A]; dso.A-- > 0;) {
						
						dim_t dc;
						dc.A = dso.A + sng.anchor.A[di.A];
						dc.B = dso.B + sng.anchor.B[di.B];
						dc.C = dso.C + sng.anchor.C[di.C];
						dc.D = dso.D + sng.anchor.D[di.D];
						dc.E = dso.E + sng.anchor.E[di.E];
						
						torus_setCutOff(dc);
					}
				}
			}
		}
	}
}

void sng_routeSetReachable(dim_t di) {
	DPRINTF(SNG__ROUTE_APPLY, "supernode [%d,%d,%d,%d,%d], size (%d,%d,%d,%d,%d) is reachable\n",
			di.A, di.B, di.C, di.D, di.E,
			sng.size.A[di.A], sng.size.B[di.B], sng.size.C[di.C], sng.size.D[di.D], sng.size.E[di.E]);
	
	dim_t dso; // destination supernode offset
	
	for(dso.E = sng.size.E[di.E]; dso.E-- > 0;) {
		for(dso.D = sng.size.D[di.D]; dso.D-- > 0;) {
			for(dso.C = sng.size.C[di.C]; dso.C-- > 0;) {
				for(dso.B = sng.size.B[di.B]; dso.B-- > 0;) {
					for(dso.A = sng.size.A[di.A]; dso.A-- > 0;) {
						
						dim_t dc;
						dc.A = dso.A + sng.anchor.A[di.A];
						dc.B = dso.B + sng.anchor.B[di.B];
						dc.C = dso.C + sng.anchor.C[di.C];
						dc.D = dso.D + sng.anchor.D[di.D];
						dc.E = dso.E + sng.anchor.E[di.D];
						
						torus_setReachable(dc);
					}
				}
			}
		}
	}
}


void sng_routeFindCutOffNodes() {
	// number of supernodes in the system
	uint32_t num = (sng.num.A * 
					sng.num.B *
					sng.num.C *
					sng.num.D *
					sng.num.E);

	uint32_t sr; // supernode rank
	for (sr = 0; sr < num; sr++) {
		if (! sng_routeDijkstraIsInSet(tsm, sr)) {
			dim_t si = sn_getCoords(sr);
			sng_routeSetCutOff(si);
		}
	}
}


/*
  This function applies routes for destination supernode 'di' to all nodes contained in
  'di'.

  - 'di'  is the index of the supernode
  - 'dir' is the index rank of the supernode
 */
void sng_routeApply(dim_t di, uint32_t dir) {
#ifdef SNG_DEBUG
	char line[512];
	DSPRINTF(SNG__ROUTE_APPLY, line, "Mapping routes for supernode [%d,%d,%d,%d,%d] = (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d):",
			 di.A, di.B, di.C, di.D, di.E,
			 sng.anchor.A[di.A], sng.anchor.B[di.B], sng.anchor.C[di.C], sng.anchor.D[di.D], sng.anchor.E[di.E],
			 sng.anchor.A[di.A+1]-1, sng.anchor.B[di.B+1]-1, sng.anchor.C[di.C+1]-1, sng.anchor.D[di.D+1]-1, sng.anchor.E[di.E+1]-1);

	if (sng.nodes[dir].pred->flags & 0x1) {
		dim_t si = sng.this.index;        // source node index
		dim_t ii = sng.nodes[dir].pred->fwnode; // intermediate node index
		
		DSPRINTF(SNG__ROUTE_APPLY, line + strlen(line), " Using intermediate supernode [%d,%d,%d,%d,%d] = (%d,%d,%d,%d,%d)-(%d,%d,%d,%d,%d).",
				 ii.A, ii.B, ii.C, ii.D, ii.E, 
				 sng.anchor.A[ii.A], sng.anchor.B[ii.B], sng.anchor.C[ii.C], sng.anchor.D[ii.D], sng.anchor.E[ii.E],
				 sng.anchor.A[ii.A+1]-1, sng.anchor.B[ii.B+1]-1, sng.anchor.C[ii.C+1]-1, sng.anchor.D[ii.D+1]-1, sng.anchor.E[ii.E+1]-1);
	}
	
	if (sng.nodes[dir].pred->flags & 0x2) {
		DSPRINTF(SNG__ROUTE_APPLY, line + strlen(line), " Setting hint bits 0x%02x.", sng.nodes[dir].pred->hint);
	}
	
#endif

	if (sng.nodes[dir].pred->flags & 0x3) {
	  sng_routeSetRoute(di, dir, sng.nodes[dir].pred->flags);
	} else {
	    DSPRINTF(SNG__ROUTE_APPLY, line + strlen(line), " This supernode is reachable on a natural route");
	    sng_routeSetReachable(di);
	} 

	DPRINTF(SNG__ROUTE_APPLY, "%s\n", line);
}


void sng_routeCheck() {
	void follow(dim_t ni) {
		uint32_t nir = sn_getRank(ni);
		
		if (sng.nodes[nir].pred->dir != 0) {
			dim_t fi = sng_getNeighbor(ni, sng.nodes[nir].pred->dir);
			
			follow(fi);
			
			DPRINTF(SNG__ROUTE_CHECK, "          ---[%s]--> [%d,%d,%d,%d,%d]\n", 
					dir2inv2str[__builtin_ffs(sng.nodes[nir].pred->dir)-1], 
					ni.A, ni.B, ni.C, ni.D, ni.E);
		} else if (ni.raw == sng.this.index.raw){
			DPRINTF(SNG__ROUTE_CHECK, "[%d,%d,%d,%d,%d] --------->\n", 
					ni.A, ni.B, ni.C, ni.D, ni.E);
		} else {
			DPRINTF(SNG__ROUTE_CHECK, "[%d,%d,%d,%d,%d] --------->           (ERROR)\n", 
					ni.A, ni.B, ni.C, ni.D, ni.E);
		}
	}
	
	dim_t ni;
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
						uint32_t nir = sn_getRank(ni);
						
						if ((usm[nir >> 5] & (1 << (nir & 0x1F))) != 0) {
							DPRINTF(SNG__ROUTE_CHECK, "--------------------------------------------------------------------------------\n");
							follow(ni);
						}
					}
				}
			}
		}
	}
}

/* 
   Prints all routes so that they can be parsed easily
 */
void sng_routePrintNicely() {
	void follow(dim_t ni) {
		uint32_t nir = sn_getRank(ni);
		
		if (ni.raw != sng.this.index.raw) {
			uint8_t pred;
			
			if (sng.nodes[nir].pred->dir == 0) {
				DPRINTF(SNG__ROUTE_CHECK, "ERROR: End of path. [%d,%d,%d,%d,%d] has no predecessor defined and it's not the source either!\n",
						ni.A, ni.B, ni.C, ni.D, ni.E);
				return;
			}
			
			pred = sng.nodes[nir].pred->dir;
			
			dim_t fi = sng_getNeighbor(ni, pred);
			
			follow(fi);
		}
	}
	
	dim_t ni;
	for(ni.A = 0; ni.A < sng.num.A; ni.A++) {
		for(ni.B = 0; ni.B < sng.num.B; ni.B++) {
			for(ni.C = 0; ni.C < sng.num.C; ni.C++) {
				for(ni.D = 0; ni.D < sng.num.D; ni.D++) {
					for(ni.E = 0; ni.E < sng.num.E; ni.E++) {
						uint32_t nir = sn_getRank(ni);
						
						if ((usm[nir >> 5] & (1 << (nir & 0x1F))) != 0) {
							follow(ni);
						}
					}
				}
			}
		}
	}
}

/*
  Initialize supernode graph with default routes and path costs.
  -> Initializes every possible supernode with a default route and costs
  -> Sets predecessor, successor(s), used-up turns and path costs.
 */
void sng_routeSetup() {
	DPRINTF(SNG__ROUTE_INITIALIZE, "Initializing routes on the supernode graph...\n");
	
	heap_init(&bsh, 1024);
	//  stack_region_init(&blocked, 1024);
	
	sng_routeReset();
	
	DPRINTF(SNG__ROUTE_INITIALIZE, "Costs: turn (%d), unnatural turn (%d), hop (%d), wraparound (%d)\n",
			COST_TURN, COST_UNNATURAL_TURN, COST_HOP, COST_WRAPAROUND);
	
}


void sng_routeReset() {
	DPRINTF(SNG__ROUTE_INITIALIZE, "Resetting routes on the supernode graph...\n");
	
	bzero(&tsm, sizeof(tsm)); // clear Dijkstra's tree set.
	bzero(&bsm, sizeof(bsm)); // clear Dijkstra's tree set.
	bzero(&usm, sizeof(usm)); // clear Dijkstra's tree set.
	
	heap_reset(&bsh);
	
	supernode_t reset;
	// set costs to "infinity"/0xFFF, everything else to 0.
	bzero(&reset, sizeof(supernode_t));

	int l;
	for(l = 0; l < MAX_DIM; l++)
	  reset.dim[l].cost  = 0xFFFF;
	
	DPRINTF(SNG__ROUTE_INITIALIZE, "Resetting %d supernode routes...\n", MAX_NUM_SUPERNODES);

	uint32_t n;
	for(n = 0; n < MAX_NUM_SUPERNODES; n++) {
		memcpy(&sng.nodes[n], &reset, sizeof(supernode_t));
		sng.nodes[n].pred = &sng.nodes[n].dim[0];
	}

	bzero((void *)&torus.nodes, sizeof(torus.nodes));
}
