#ifndef __TORUS__
#define __TORUS__

#include "common.h"
#include "sng_common.h"

typedef struct {
	dim_t        fwnode;     // intermediate forwarding node.
	unsigned int flags :  4; // flags: 0x1 = needs forwarding node, 0x2 = needs hint bits, 0x4 = cut off
	unsigned int hint  : 10; // hint bits
	
#ifdef SNG_DEBUG
	int isIntermediate;     // 0 = no intermediate node, > 0 = number of nodes to serve.
	int isCutOff;           // 0 = not cut off, 1 = cut off
	int isReachable;        // 0 = not reachable, 1 = reachable
#endif
} node_t;

#ifdef SNG_DEBUG
typedef struct {
	uint8_t status; // 0 = good, 1 = bad
	
	struct {
		uint32_t plus;  // load in plus direction
		uint32_t minus; // load in minus direction
	} weight; // this was load, but somehow the cross compiler doesn't like load. now it's weight.
} link_t;
#endif

typedef struct {
	dim_t size;    // size of the torus in each dimension
	int   dim;     // number of dimensions (2,3,4,5)
	
	struct {
		dim_t    log;   // the node I'm running on.
		dim_t    phy;
		uint32_t rank;
	} local;
	
	struct {
		dim_t plus;
		dim_t minus;
	} cutoffs;     // cutoffs for routing w.r.t. local node
	
	dim_t offset;  // offset for physical to logical mapping.
	
	dim_t rdo;     // routing dimension order
	dim_t odr;     // reverse dimension order for log->phy mapping (hint bits)
	
	dim_t closed;  // 1 = closed, 0 = open
	
	node_t nodes[1048576]; // status of all possible nodes
	
	uint32_t nodecount;
	uint32_t failures;
	
#ifdef SNG_DEBUG
	link_t links[4194304]; // status of all links.
#endif  
} torus_t;

extern torus_t torus;


void torus_setDimensions(int d);
void torus_setSize(dim_t s);
void torus_setLocal(dim_t l);
void torus_setOffset(dim_t o);
void torus_setDimensionOrder(dim_t o);

void torus_setRoute(dim_t n, dim_t fw, uint16_t h, int flags);
void torus_setIntermediateNode(dim_t n, dim_t fw);
void torus_setHintBits(dim_t n, uint16_t h);
void torus_setCutOff(dim_t n);
void torus_setReachable(dim_t n);

void torus_setup();

void torus_routeCheck();

void torus_printLatex(int phy);
void torus_printRoutes();

#endif
