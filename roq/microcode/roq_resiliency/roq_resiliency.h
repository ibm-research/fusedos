#ifndef ROQ_RESILIENCY_H
#define ROQ_RESILIENCY_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/eventfd.h>
#include <asm/types.h>
#include <bgq_hw_abstraction.h>
#include <errno.h>

#define __KERNEL__
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/classroute.h>

#include <spi/include/kernel/firmware/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
//#include <firmware/include/SPI_syscalls.h>
#include <spi/include/kernel/process.h>
#include <spi/include/kernel/location.h>
#include <spi/include/kernel/firmware/location_impl.h>
#include <spi/include/kernel/gi.h>

#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/mu/GI.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/firmware/memory_impl.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/kernel/firmware/MU_impl.h>
#include <hwi/include/bqc/gea_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <spi/include/mu/Reset.h>
#undef __KERNEL__

#include <spi/include/kernel/collective.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>

#include <roq_kernel.h>
#include <transcon.h>
#include <roq_fw.h>
#include "supernodes/common.h"

#define AM 0
#define AP 1
#define BM 2
#define BP 3
#define CM 4
#define CP 5
#define DM 6
#define DP 7
#define EM 8
#define EP 9

#define AC_DEBUG 1
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)>(b) ? (b) : (a))
#define SUB(a,b) ((a)>(b) ? (a)-(b) : 0)
#define DIFF(a,b) ((a)>(b) ? (a)-(b) : (b)-(a))

#define RR_INFO  0
#define RR_DEBUG 0
#define RR_ERROR 1

#define RR_ROUTER_DEBUG 0
#define RR_ROUTER_INFO  0
#define RR_ROUTER_ERROR 1

#define RR_CHECKER_DEBUG 0
#define RR_CHECKER_INFO  0
#define RR_CHECKER_ERROR 1

#define RR_FORWARDER_DEBUG 0
#define RR_FORWARDER_INFO  0
#define RR_FORWARDER_ERROR 1

#define PRINTF(l, fmt, ...) if (l) printf(fmt, ##__VA_ARGS__)

#define INFO(fmt, ...)  PRINTF(RR_INFO, fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...) PRINTF(RR_DEBUG, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) PRINTF(RR_ERROR, fmt, ##__VA_ARGS__)

#define DBGCODE(code) code

#define AC_MAX_BAD_NODES 10
#define AC_NODE_OFFLINE 0x1 // indicates that a node went offline
#define AC_NODE_ONLINE  0x2 // indicates that a node came back online

#define likely(x) __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

typedef struct {
	struct {
		uint32_t init;   // initial timeout (number of periods)
		uint32_t max;    // maximum regular timeout (number of periods)
		uint32_t period; // execution period
	} detection;

	struct {
		uint32_t period; // execution period
		uint32_t wait;   // time (in us) to wait for another packet after a packet has been recevied
	} broadcast;

	struct {
		uint32_t period; // execution period
		uint32_t wait;   // time (in us) to wait for another packet after a packet has been recevied
	} forwarder;

	struct {
		uint32_t wait;   // time (in us) between the last fault event and updating the routes.
	} router;
} config_t;

typedef struct {
	volatile dim_t    * list;
	volatile uint32_t   size; // size of the list
	volatile uint32_t   head; // index of latest entry
	RoQ_Atomic_t        lock; // lock required as these list are ued by at least 2 threads.
} linkList_t;

// array of neighbor coordinates
//extern dim_t * neighbor[8]; // list of neighbors.
//extern dim_t * badNodes[8]; // list of bad nodes.

extern volatile uint16_t lcl_badLinks;

/* Structure to store 1 bit for each axes. With an axes I mean e.g. the edges of a dice.
 * if I put two dices together they share 4 axes. Examples:
 * 2x2x2 torus has 2x2+2x2+2x2 = 12 axes
 * 3x2x2 torus has 3x2+2x2+2x3 = 16 axes
 * 3x3x2 torus has 3x3+3x2+2x3 = 21 axes
 * Having this structure makes routing diecissions faster. As I can quickly filter out good/bad axes
 * and then I don't have to check every single link on that axes anymore. It is also much smaller
 * than storing every bad link in an array.
 */
extern uint32_t *badAxes;

extern config_t config;

static char linkNum2Str[10][3] = {"AM\0", "AP\0",
								  "BM\0", "BP\0",
								  "CM\0", "CP\0",
								  "DM\0", "DP\0",
								  "EM\0", "EP\0"};
static char dim2Str[5][2] = {"A\0", "B\0", "C\0", "D\0", "E\0"};

extern linkList_t gbl_badLinksNew; // list of all bad links present in the system.

extern uint64_t benchmarkCyclesMin;
extern uint64_t benchmarkCyclesMax;
extern uint64_t benchmarkCycles;
extern volatile uint64_t benchmarkBegin;
extern volatile uint64_t benchmarkEnd;
extern uint64_t benchmarkAdjust;
extern uint64_t benchmarkDuration;
extern uint64_t benchmarkDurationFirst;
extern uint64_t benchmarkSamples;

#ifdef BENCHMARK
#define COUNTCYCLES(code)												\
	benchmarkBegin     = GetTimeBase();									\
	benchmarkEnd       = GetTimeBase();									\
	benchmarkAdjust    = benchmarkEnd - benchmarkBegin;					\
	benchmarkBegin     = GetTimeBase();									\
    code																\
	benchmarkEnd       = GetTimeBase();									\
	benchmarkDuration  = SUB(benchmarkEnd-benchmarkBegin,benchmarkAdjust); \
	benchmarkDurationFirst = benchmarkDurationFirst == 0 ? benchmarkDuration : benchmarkDurationFirst; \
	benchmarkCyclesMin = (benchmarkCyclesMin > benchmarkDuration ? benchmarkDuration : benchmarkCyclesMin); \
	benchmarkCyclesMax = (benchmarkCyclesMax < benchmarkDuration ? benchmarkDuration : benchmarkCyclesMax); \
	benchmarkSamples++;													\
	benchmarkCycles    += benchmarkDuration;
#else
#define COUNTCYCLES(code) code
#endif

typedef struct {
	uint64_t       nodeCount;     // total amount of nodes in the system
	dim_t          nodes;         // amount of nodes per dimension
	dim_t          coords;        // coordinates of this node
	uint8_t        dimensions;    // amount of dimensions
	dim_t        * neighbors[8];  // coordinates of neighbors (or NULL if there is no neighbor)
	
	uint8_t      * linkTable;     // all working links between all nodes
	uint8_t      * routingTable;  // 
	
	RoQ_Atomic_t   setupLock;

	uint16_t       freqMhz;
} rr_global_vars_t;

extern rr_global_vars_t global;

static uint64_t m_w = 0;
static uint64_t m_z = 0;

static uint32_t get_random(uint32_t max) {
	if (m_w == 0) {
		uint64_t seed = GetTimeBase();
		m_w = seed & 0xFFFFFFFF;
//		m_z = (seed >> 16) & 0xFFFFFFFF;
	}

//	m_z = 36969 * (m_z & 0xFFFFFFFF) + (m_z >> 32);
	m_w = 18000 * (m_w & 0xFFFFFFFF) + (m_w >> 32);

	uint32_t res = m_w;

	return (m_w % max);
	//return (m_z << 32) + m_w;
}

inline void static uwait(uint32_t us) {
	volatile uint64_t end = get_time() + us;
	while (end > get_time()) {}
}


__INLINE__ dim_t lnk_getLinkByDir(int dir) {
  dim_t link = global.coords;
  int   dim  = dir>>1;

  link.raw <<= 1;

  if (is_minus(dir)) {
	  link.dim[dim] = mod((link.dim[dim] - 1), global.nodes.dim[dim] << 1); // -1 is the wraparound link == max coord + 1
  } else {
	  link.dim[dim] = link.dim[dim] + 1; // max coord +1  is the wrap-around link. No mod needed here.
  }

  return link;
}

/* returns 1 if successful, 0 if not */
__INLINE__ int64_t RoQ_AtomicTestAndSet(RoQ_Atomic_t *v) {
  long t = 0;

  /* t = *v;
   * if (t == 0) {
   *    t++;
   *    v = t;
   * } else {
   *    t  = 0;
   * }
   * return t;
   */
  __asm__ __volatile__(
        "sync \n\
1:      ldarx   %0,0,%1         # load and reserve v into r0 \n\
        cmpdi   %0,0            # compare if r0 is 0 \n\
        bne-    2f              # forward jump to 2 if cmpdi was false \n\
        addic   %0,%0,1         # increment r0, r0=1 (always) \n\
        stdcx.  %0,0,%1         # store if reservation is still valid \n\
        bne-    1b              # try again, if reservation was not valid anymore \n\
        isync                     \n\
        b       3f                \n\
2:      subc    %0,%0,%0        # set t=0 to inticate that test result was false \n\
3:"

        : "=&r" (t)
        : "r" (v)
        : "cc", "xer", "memory");

  return t;
}

__INLINE__ void RoQ_AtomicClear(RoQ_Atomic_t *v) {
  long t = 0;

  __asm__ __volatile__(
        "sync \n\
1:      ldarx   %0,0,%1         # load and reserve v into r0 \n\
        subc    %0,%0,%0        # clear \n\
        stdcx.  %0,0,%1         # store if reservation is still valid \n\
        bne-    1b              # try again, if reservation was not valid anymore \n\
        isync                     \n"
        : "=&r" (t)
        : "r" (v)
        : "cc", "xer", "memory");
}

static inline void RoQ_LockAcquire(RoQ_Atomic_t *lock) {
  while(! RoQ_AtomicTestAndSet(lock)) {};
}

static inline void RoQ_LockRelease(RoQ_Atomic_t *lock) {
  // not sure if *lock=0 wouldn't also do the job.
  RoQ_AtomicClear(lock);
}

static int push(linkList_t *linkList, dim_t *link) {
  int rc = 0;

  RoQ_LockAcquire(&(linkList->lock));

  if (linkList->head < linkList->size) {
    linkList->list[linkList->head] = *link;
    linkList->head++;
  } else {
    ERROR("link list is full (max=%u)\n", linkList->size);
    rc = -1;
  }

  RoQ_LockRelease(&(linkList->lock));

  return rc;
}

static int pushOnce(linkList_t *linkList, dim_t *link) {
  int rc = 0;
  int i  = 0;
  //  INFO("trying to push new link item\n");

  RoQ_LockAcquire(&(linkList->lock));

  if (linkList->head < linkList->size) {
    for (i = linkList->head-1; i >= 0; i--) { // double entries are most likely at the end of the list.
      if (linkList->list[i].raw == link->raw) {rc=-1; break;}
    }
    if (rc == 0) {
      linkList->list[linkList->head] = *link;
      linkList->head++;
    }
  } else {
    ERROR("link list is full (max=%u)\n", linkList->size);
    rc = -1;
  }

  RoQ_LockRelease(&(linkList->lock));

  return rc;
}

static int pop(linkList_t *linkList, dim_t *link) {
  int rc = 0;

  RoQ_LockAcquire(&(linkList->lock));

  if (linkList->head > 0) {
    rc = linkList->head;
    linkList->head--;
    *link = linkList->list[linkList->head];
  }

  RoQ_LockRelease(&(linkList->lock));

  return rc;
}

static int unary2Int(int uval) {
  switch (uval) {
  case 16: return 0;
  case 8:  return 1;
  case 4:  return 2;
  case 2:  return 3;
  case 1:  return 4;
  default: return -1;
  }
}

void wakeupThread(unsigned int pid);
void shutdownLink(uint8_t i);

/* Online checker */
void Checker_GlobalInterrupts();
void Checker_Msg();

/* Bad node notifiers */
void Notifier_Peer2Peer();

/* Router */
void Router();
int  isFree(dim_t *link, dim_t *source, dim_t *target);
void RecalculateRoutes();
void initLinkTable();
void updateLinkTable(dim_t *link);
void updateRoutingTable(int maxHops);
int injectFailure(dim_t l);

#endif // ROQ_RESILIENCY_H
