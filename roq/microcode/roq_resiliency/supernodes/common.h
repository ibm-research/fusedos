#ifndef __COMMON__
#define __COMMON__

#include <stdint.h>

#define MAX_DIM       5 // maximal number of dimensions
#define MAX_DIR      10 // maximal number of directions/links
#define MAX_DIM_SIZE 32 // maximal number of nodes per dimension

#define is_minus(d) ((d&1) == 0)
#define is_plus(d)  ((d&1) != 0)

// this modulo works only for a = [-b..max]. a may never be smaller than -b!!!!!
#define mod(a,b) (((a) + (b)) % (b))

#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)

#define likely(x)   __builtin_expect(x, 1)
#define unlikely(x) __builtin_expect(x, 0)


//typedef unsigned long long int   uint64_t;
//typedef unsigned long int        uint32_t;
//typedef unsigned short int       uint16_t;
//typedef unsigned char            uint8_t;

typedef union {
	int8_t   dim[5];
	uint64_t raw : 40;
	struct{
		int8_t A;
		int8_t B;
		int8_t C;
		int8_t D;
		int8_t E;
	} __attribute__ ((packed));
} dim_t;

static char dir2str[10][4] = {"A-\0", "A+\0",
							  "B-\0", "B+\0",
							  "C-\0", "C+\0",
							  "D-\0", "D+\0",
							  "E-\0", "E+\0"};

static char dir2inv2str[10][4] = {"A+\0", "A-\0",
				  "B+\0", "B-\0",
				  "C+\0", "C-\0",
				  "D+\0", "D-\0",
				  "E+\0", "E-\0"};

static char dim2str[5][3] = {"A\0",
							 "B\0",
							 "C\0",
							 "D\0",
							 "E\0"};

static uint8_t dir2inv[10] = {1,0,  // A-, A+
							  3,2,  // B-, B+
							  5,4,  // C-, C+
							  7,6,  // D-, D+
							  9,8}; // E-, E+

static uint16_t dir2hint[10] = {0x0200,  // A-
								0x0100,  // A+
								0x0080,  // B-
								0x0040,  // B+
								0x0020,  // C-
								0x0010,  // C+
								0x0008,  // D-
								0x0004,  // D+
								0x0002,  // E-
								0x0001}; // E+

static uint16_t dim2hint[5] = {0x0300,  // A
							   0x00C0,  // B
							   0x0030,  // C
							   0x000C,  // D
							   0x0003}; // E

void roq_resiliency_setIntermediateNode(dim_t n, dim_t fw);
void roq_resiliency_setHintsBits(dim_t n, uint8_t h);

int   lnk_getStatus(dim_t l); // from sng_common.h, here so that it can be used in p2p notifier module.
dim_t lnk_phy2log(dim_t l);   // from sng_common.h, here so that it can be used in p2p notifier module.




#endif
