#ifndef __SNG_COMMON__
#define __SNG_COMMON__

#include <stdio.h>
#include <stdlib.h>

#include "strings.h"
#include "common.h"

#ifdef ROQ_RESILIENCY_DEBUG
#define SNG_DEBUG                    1
#endif

#define SN__GET                      0
#define SN__GET_DIM                  0
#define SN__SPLIT                    0
#define SN__MERGE                    1
#define SN__MERGE_CHECK              1
#define SN__MERGE_BLOCK_DETECTION    1
#define SN__MERGE_BLOCK_FACE_CHECK   1
#define SN__INJECT_FAULT             1

#define SNG__BUILD_SETUP             1

#define LNK__INJECT_FAULT            1
#define LNK__GET_STATUS              0

#define SNG__ROUTE_INITIALIZE        1


#ifdef SNG_DEBUG
#define DPRINTF(lvl, fmt, ...) if(lvl) {printf("%-30s : " fmt, __FUNCTION__, ##__VA_ARGS__);}
#define DSPRINTF(lvl, str, fmt, ...) if(lvl) {sprintf(str, fmt, __FUNCTION__, ##__VA_ARGS__);}
#define PRINTF(lvl, fmt, ...) if(lvl) {printf(fmt, ##__VA_ARGS__);}
#else
#define DPRINTF(lvl, fmt, ...) 
#define DSPRINTF(lvl, str, fmt, ...) 
#define PRINTF(lvl, fmt, ...) 
#endif

//#define MAX_NUM_SUPERNODES 128
#define MAX_NUM_SUPERNODES 1048576

typedef union {
  uint8_t  num[2];
  uint16_t raw;
  struct {
    uint8_t one;
    uint8_t two;
  } __attribute__ ((packed));
} pair8_t;

typedef union {
	uint8_t raw[5];
	struct {
		dim_t        fwnode;       // intermediate forwarding node.
		unsigned int hint    : 10; // hint bits for dimension direction
		unsigned int flags   :  4; // flags: 0x1 = needs forwarding node, 0x2 = needs hint bits.
	  	unsigned int dir     : 10; // link to predecessor: 1 bit for each, only 1 possible
		unsigned int cost    : 16; // path costs to this node.
	} __attribute__ ((packed));
} path_t;

typedef struct {
  path_t   dim[MAX_DIM]; // paths via all possible links
  path_t * pred;         // path/link that has been selected for this node. -1 for not selected.
} supernode_t;

// * Row/Column Arrays (4x32) that stores the south/west coordinate of every
//   supernode
// * Row/Column Arrays (4x32) that stores size in each dimension of every
//   supernode
// * SN-Matrix with 1 entry for each supernode (32x32x32x32). Stores things like
//   predecessor, intermediate nodes, ... 1MB per byte of information required,
//   e.g. 32MB for 32 bytes of information. Referenced by entries in the R/C
//   arrays, except for things like predecessor, there's a direct referencing.

typedef struct {
  union {
    uint8_t dim[5][33];
    struct {
      uint8_t A[33];
      uint8_t B[33];
      uint8_t C[33];
      uint8_t D[33];
	  uint8_t E[33]; // actually at most 2
    };
  } anchor; // size of each super node

  union {
    uint8_t dim[5][33];
    struct {
      uint8_t A[33];
      uint8_t B[33];
      uint8_t C[33];
      uint8_t D[33];
	  uint8_t E[33]; // actually at most 2
    };
  } size; // size of each super node

  dim_t num; // number of super nodes in each dimension

  supernode_t nodes[MAX_NUM_SUPERNODES]; // status of all possible nodes + links
  
  union {
    // access: 
    // - A's bit of A[E][D][C][B];
    // - B's bit of B[E][D][C][A];
    // - C's bit of C[E][D][B][A];
    // - D's bit of D[E][C][B][A];
    // - E's bit of E[D][C][B][A];
    // 
    // 0 = good, 1 = bad
    uint32_t dim[5][32][32][32][32];
    struct {
      uint32_t A[32][32][32][32];
      uint32_t B[32][32][32][32];
      uint32_t C[32][32][32][32];
      uint32_t D[32][32][32][32];
      uint32_t E[32][32][32][32];
    };
  } axes; // normal axes

  union {
    // Let l be a link in dimension d, at position n. Then
    // dim[d][n]++, i.e. there's one more fault on plane d[d].
    // Example: link A+ on node (5,4,6,1) is broken.
    //          --> A[5]++
    uint16_t dim[5][32];
    struct {
      uint16_t A[32];
      uint16_t B[32];
      uint16_t C[32];
      uint16_t D[32];
      uint16_t E[32];
    };
  } plane; // plane status, 0 = good, 1 = bad.

  struct {
    dim_t    coord;
    dim_t    index;
    uint32_t rank;
  } this; // number of this=local=source supernode, i.e. the supernode that contains
          // the source node.

} supernode_matrix_t;

typedef union {
  dim_t corner[2];
  struct {
    dim_t lo;
    dim_t hi;
  };
} region_t;

extern dim_t              c[5];  // order in which the dimensions are stored for axes.
extern supernode_matrix_t sng;   // supernode graph

uint32_t lnk_getRank(dim_t l);
uint8_t  lnk_getDimension(dim_t l);
uint8_t  lnk_getDirection(dim_t l);

inline void  lnk_getEndpoints(dim_t l, dim_t *n0, dim_t *n1);
inline dim_t lnk_getSrcNode(dim_t l);
inline dim_t lnk_getDstNode(dim_t l);

inline uint32_t nd_getRank(dim_t n);  // coords rank

inline uint32_t sn_getRank(dim_t ni);
inline dim_t sn_getCoords(uint32_t nr);

void     sng_print();

dim_t    sng_getNeighbor(dim_t sn, uint16_t dv);

dim_t    nd_phy2log(dim_t n);
dim_t    nd_log2phy(dim_t n);

dim_t    lnk_phy2log(dim_t l);
dim_t    lnk_log2phy(dim_t l);

uint16_t hint_log2phy(uint16_t h);

inline uint32_t axis_get(uint8_t dim, dim_t axis);
inline uint8_t  axis_getNextOne(uint32_t axis, uint32_t *mask);

#ifdef ROQ_RESILIENCY_BGQ
inline int has2ormore1s32(uint32_t v);
#endif

/* From MU_Pt2PtNetworkHeader.h

#define MUHWI_PACKET_HINT_AM  (0x80)
#define MUHWI_PACKET_HINT_AP  (0x40)
#define MUHWI_PACKET_HINT_BM  (0x20)
#define MUHWI_PACKET_HINT_BP  (0x10)
#define MUHWI_PACKET_HINT_CM  (0x08)
#define MUHWI_PACKET_HINT_CP  (0x04)
#define MUHWI_PACKET_HINT_DM  (0x02)
#define MUHWI_PACKET_HINT_DP  (0x01)

#define MUHWI_PACKET_HINT_EM  (0x80)
#define MUHWI_PACKET_HINT_EP  (0x40)
*/
#endif
