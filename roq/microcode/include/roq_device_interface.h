/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

#ifndef __ROQ_DEVICE_INTERFACE_H__
#define __ROQ_DEVICE_INTERFACE_H__

/*
 * the roq fw writes the location of dev mem in log lines
 * the dev mem location must be recorded here
 * the linux dd finds the dev mem at this physical location
 * 0xaeea5c0  bgqiosn
 * 0xaeeaa40  bgqssn0
 */
#ifdef _NEED_ROQ_MAGIC_LOCATION
static void *ExpectedLocationOf_RoQ_DevMem = (void *)0xaeeaa40;
#endif

#ifndef __KERNEL__
#define _bgp_msync() ppc_msync()
#include <pthread.h>
#include <time.h>
#include <roq_debug.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/*
 * This file contains structures that are shared between the RoQ
 * and it's device driver
 */
#include <SRSW_QueueSet.h>
#include <roq_common.h>

#define RoQ_IPADDR_SUBNET 12

/*
 * this should be changed to a vector coord as the calls below
 * should as well
 */
struct RoQ_NetworkDescriptor {
	uint32_t Acoord;
	uint32_t Bcoord;
	uint32_t Ccoord;
	uint32_t Dcoord;
	uint32_t Ecoord;
	uint32_t Anodes;
	uint32_t Bnodes;
	uint32_t Cnodes;
	uint32_t Dnodes;
	uint32_t Enodes;
	uint32_t IOport;
};

static inline int
RoQ_NetworkSize(struct RoQ_NetworkDescriptor *aNetDesc)
{
	int rc = aNetDesc->Anodes * aNetDesc->Bnodes * aNetDesc->Cnodes
		 * aNetDesc->Dnodes * aNetDesc->Enodes;
#ifndef __LINUX__
	DASSERT(rc >= 0, "%lx", (uint64_t)rc);
#endif
	return rc;
}

static inline void
RoQ_TorusCoords_To_IPV4Addr(struct RoQ_NetworkDescriptor *aNetDesc,
			    uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
			    uint32_t *aIPV4AddrPtr)
{
	uint32_t IPV4Addr = (RoQ_IPADDR_SUBNET << 24) | (a << 16) | (b << 12)
			| (c << 8) | (d << 4) | (e);
#ifndef __LINUX__
	DASSERT(RoQ_IPADDR_SUBNET <= 0xFF, "%d", RoQ_IPADDR_SUBNET);
	DASSERT(a <= aNetDesc->Anodes, "%d %d", a, aNetDesc->Anodes);
	DASSERT(b <= aNetDesc->Bnodes, "%d %d", b, aNetDesc->Bnodes);
	DASSERT(c <= aNetDesc->Cnodes, "%d %d", c, aNetDesc->Cnodes);
	DASSERT(d <= aNetDesc->Dnodes, "%d %d", d, aNetDesc->Dnodes);
	DASSERT(e <= aNetDesc->Enodes, "%d %d", e, aNetDesc->Enodes);
#endif
	*aIPV4AddrPtr = IPV4Addr;
}

static inline void
RoQ_IPV4Addr_To_TorusCoords(struct RoQ_NetworkDescriptor *aNetDesc,
			    uint32_t aIpAddr, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d,
			    uint32_t *e)
 {
#ifndef __LINUX__
	int subnet = (aIpAddr >> 24) & 0xFF;
	DASSERT(subnet == RoQ_IPADDR_SUBNET, "%d", subnet);
#endif
	/* BGQ_IP_PREFIX.0000aaaa.bbbbcccc.ddddeeee */
	*a = (aIpAddr >> 16) & 0xF;
	*b = (aIpAddr >> 12) & 0xF;
	*c = (aIpAddr >>  8) & 0xF;
	*d = (aIpAddr >>  4) & 0xF;
	*e = (aIpAddr & 0xF);
#ifndef __LINUX__
	DASSERT(*a <= aNetDesc->Anodes, "%d %d %d %d %d\n",
		aNetDesc->Anodes, aNetDesc->Bnodes, aNetDesc->Cnodes,
		aNetDesc->Dnodes, aNetDesc->Enodes);
	DASSERT(*b <= aNetDesc->Bnodes, "%d %d %d %d %d\n",
		aNetDesc->Anodes, aNetDesc->Bnodes, aNetDesc->Cnodes,
		aNetDesc->Dnodes, aNetDesc->Enodes);
	DASSERT(*c <= aNetDesc->Cnodes, "%d %d %d %d %d\n",
		aNetDesc->Anodes, aNetDesc->Bnodes, aNetDesc->Cnodes,
		aNetDesc->Dnodes, aNetDesc->Enodes);
	DASSERT(*d <= aNetDesc->Dnodes, "%d %d %d %d %d\n",
		aNetDesc->Anodes, aNetDesc->Bnodes, aNetDesc->Cnodes,
		aNetDesc->Dnodes, aNetDesc->Enodes);
	DASSERT(*e <= aNetDesc->Enodes, "%d %d %d %d %d\n",
		aNetDesc->Anodes, aNetDesc->Bnodes, aNetDesc->Cnodes,
		aNetDesc->Dnodes, aNetDesc->Enodes);
#endif
}

static inline void
RoQ_Rank_To_TorusCoords(struct RoQ_NetworkDescriptor *aNetDesc, uint32_t aRank,
			uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d, uint32_t *e)
{
	/* match CNK's default map order ABCDET */
	/* see how we calculate the rank from coordinates below
	 * for why it works this way */
	uint32_t rank_rem = aRank;
	
	*e = rank_rem % aNetDesc->Enodes;
	rank_rem /= aNetDesc->Enodes;

	*d = rank_rem % aNetDesc->Dnodes;
	rank_rem /= aNetDesc->Dnodes;

	*c = rank_rem % aNetDesc->Cnodes;
	rank_rem /= aNetDesc->Cnodes;

	*b = rank_rem % aNetDesc->Bnodes;
	rank_rem /= aNetDesc->Bnodes;

	*a = rank_rem % aNetDesc->Anodes;
}

static inline void
RoQ_TorusCoords_To_Rank(struct RoQ_NetworkDescriptor *aNetDesc, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t *aRankPtr)
{
	/* match CNK's default map order ABCDET */
	/* rank = E + dim_E ( D + dim_D ( C + dim_C ( B + dim_B * A ))) */

	uint32_t Rank = a * aNetDesc->Bnodes + b;
	Rank = Rank * aNetDesc->Cnodes + c;
	Rank = Rank * aNetDesc->Dnodes + d;
	Rank = Rank * aNetDesc->Enodes + e;

#ifndef __LINUX__
	DASSERT(Rank < RoQ_NetworkSize(aNetDesc),
		"%lx %lx", (uint64_t)Rank,
		(uint64_t)RoQ_NetworkSize(aNetDesc));
#endif
	*aRankPtr = Rank;
}

static inline void
RoQ_GetLocalIPV4Addr(struct RoQ_NetworkDescriptor *aNetDesc, uint32_t *aIPV4Addr)
{
	RoQ_TorusCoords_To_IPV4Addr(aNetDesc, aNetDesc->Acoord, aNetDesc->Bcoord,
				    aNetDesc->Ccoord, aNetDesc->Dcoord,
				    aNetDesc->Ecoord, aIPV4Addr);
}

static inline void
RoQ_Rank_To_IPV4Addr(struct RoQ_NetworkDescriptor *aNetDesc, uint32_t aRank, uint32_t *aIPV4Addr_Ptr)
{
	uint32_t a, b, c, d, e;

	RoQ_Rank_To_TorusCoords(aNetDesc, aRank, &a, &b, &c, &d, &e);
	RoQ_TorusCoords_To_IPV4Addr(aNetDesc, a, b, c, d, e, aIPV4Addr_Ptr);
}

static inline void
RoQ_IPV4Addr_To_Rank(struct RoQ_NetworkDescriptor *aNetDesc, uint32_t aIPV4Addr, uint32_t *aRankPtr)
{
	uint32_t a, b, c, d, e, Rank;

	RoQ_IPV4Addr_To_TorusCoords(aNetDesc, aIPV4Addr, &a, &b, &c, &d, &e);
	RoQ_TorusCoords_To_Rank(aNetDesc, a, b, c, d, e, &Rank);

#ifndef __LINUX__
	DPRINTF(0, "TORUS ADDR MAP IP 0x%p a %lx b %lx c %lx d %lx e %lx Rank %lx",
		aIPV4Addr, (uint64_t)a, (uint64_t)b, (uint64_t)c, (uint64_t)d,
		(uint64_t)e, (uint64_t)Rank);
#endif

	*aRankPtr = Rank;
}

#ifndef __LINUX__
static inline void
RoQ_TestAddressingConsistency(struct RoQ_NetworkDescriptor *aNetDesc)
{
	uint32_t rank;

	for(rank = 0; rank < (uint32_t)RoQ_NetworkSize(aNetDesc); rank++) {
		uint32_t r2a, r2b, r2c, r2d, r2e, xyz2rank, xyz2IpAddr,
		    ipa2a, ipa2b, ipa2c, ipa2d, ipa2e, rank2IpAddr,
		    IpAddr2Rank;

		RoQ_Rank_To_TorusCoords(aNetDesc, rank, &r2a, &r2b, &r2c, &r2d, &r2e);
		RoQ_TorusCoords_To_Rank(aNetDesc, r2a, r2b, r2c, r2d, r2e, &xyz2rank);

		RoQ_TorusCoords_To_IPV4Addr(aNetDesc, r2a, r2b, r2c, r2d, r2e, &xyz2IpAddr);
		RoQ_IPV4Addr_To_TorusCoords(aNetDesc, xyz2IpAddr, &ipa2a, &ipa2b, &ipa2c, &ipa2d, &ipa2e);


		RoQ_Rank_To_IPV4Addr(aNetDesc, rank, &rank2IpAddr);
		RoQ_IPV4Addr_To_Rank(aNetDesc, rank2IpAddr, &IpAddr2Rank);

		DPRINTF(1, "rank %d r2a %d r2b %d r2c %d r2d %d r2e %d xyz2rank %d xyz2IpAddr 0x%p ipa2a %d ipa2b %d ipa2c %d ipa2d %d ipa2e %d rank2IpAddr 0x%p IpAddr2Rank %d",
			(uint32_t)rank, (uint32_t)r2a, (uint32_t)r2b, (uint32_t)r2c, (uint32_t)r2d, (uint32_t)r2e, (uint32_t)xyz2rank, (uint32_t)xyz2IpAddr,
			(uint32_t)ipa2a, (uint32_t)ipa2b, (uint32_t)ipa2c, (uint32_t)ipa2d,(uint32_t)ipa2e, rank2IpAddr, (uint32_t)IpAddr2Rank);

		DSTRONG_ASSERT(rank == xyz2rank, "%d %d", rank, xyz2rank);
		DSTRONG_ASSERT(xyz2IpAddr == rank2IpAddr, "%d %d", xyz2IpAddr, rank2IpAddr);
		DSTRONG_ASSERT(ipa2a == r2a, "%d %d", ipa2a, r2a);
		DSTRONG_ASSERT(ipa2b == r2b, "%d %d", ipa2b, r2b);
		DSTRONG_ASSERT(ipa2c == r2c, "%d %d", ipa2c, r2c);
		DSTRONG_ASSERT(ipa2d == r2d, "%d %d", ipa2d, r2d);
		DSTRONG_ASSERT(ipa2e == r2e, "%d %d", ipa2e, r2e);
		DSTRONG_ASSERT(IpAddr2Rank == rank, "%d %d", IpAddr2Rank, rank);
	}
}
#endif

#define ROQ_PTHREAD_LOCKS 0  // 1 use pthreads. 0 use home grown
#define ROQ_PTHREAD_LOCKS_SPIN_IS_0_MUTEX_IS_1 0 // if using pthread locks, this selects spin or mutex


#if ROQ_PTHREAD_LOCKS
typedef struct SpinLockVar_t {
	void *m;
} SpinLockVar_t;
#else // home grown spinlock
typedef struct SpinLockVar_t {
	long long requesting_ticket ;
	volatile long long serving_ticket ;
} SpinLockVar_t;
#endif

#ifndef __LINUX__
#define FXLOG_SPINLOCK 0
#define SpinLock(aSpinLock) xxSpinLock(aSpinLock,__FILE__,__LINE__)
#if ROQ_PTHREAD_LOCKS // use pthreads routines
#if ROQ_PTHREAD_LOCKS_SPIN_IS_0_MUTEX_IS_1 // use pthreads mutexs

static void
xxSpinLock(volatile SpinLockVar_t *aSpinLock, const char *s, int i)
{
	if (aSpinLock->m == NULL) {
		aSpinLock->m = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init((pthread_mutex_t *)aSpinLock->m, NULL);
	}
	pthread_mutex_lock((pthread_mutex_t *)aSpinLock->m);
}

static void
SpinUnlock(volatile SpinLockVar_t *aSpinLock)
{
	pthread_mutex_unlock((pthread_mutex_t *)aSpinLock->m);
}

/*
 * Shouldn't normally need this since statics are implicitly initialised
 * to zero, but put it here in case we need a spinlock which isn't a static
 */
static void SpinInit(volatile SpinLockVar_t *aSpinLock)
{
	DPRINTF(FXLOG_SPINLOCK, "SpinLockVar@%p", (void *)aSpinLock);
	aSpinLock->m = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init((pthread_mutex_t*)aSpinLock->m, NULL);
}

#else /* use pthread spin locks */

	/*
	 * Note:
	 * Ben Krill says pthread_spin_lock is implemented the same way as pthread_mutex
	 * Taking out all locks functions for rdma_o2o and gives 40MiB/s improvement on top of 1247MiB/s
	 * Running that way for netperf RDMA_LAT didn't show improvement over 1200 ops/s
	 */
static void
xxSpinLock(volatile SpinLockVar_t *aSpinLock, const char* s, int i)
{
	if (aSpinLock->m == NULL) {
		aSpinLock->m = malloc(sizeof(pthread_spinlock_t));
		pthread_spin_init((pthread_spinlock_t*)aSpinLock->m, NULL);
	}
	pthread_spin_lock((pthread_spinlock_t*)aSpinLock->m);
}


static void
SpinUnlock(volatile SpinLockVar_t *aSpinLock)
{
	pthread_spin_unlock((pthread_spinlock_t*)aSpinLock->m);
}

/*
 * Shouldn't normally need this since statics are implicitly initialised
 * to zero, but put it here in case we need a spinlock which isn't a static
 */
static void SpinInit(volatile SpinLockVar_t *aSpinLock)
  {
    DPRINTF(FXLOG_SPINLOCK, "SpinLockVar@%p", (void *)aSpinLock);
    aSpinLock->m = malloc(sizeof(pthread_spinlock_t));
    pthread_spin_init((pthread_spinlock_t*)aSpinLock->m, NULL);
  }
#endif
#else /* home brew spinlock */

/*
 * From:  /bgsys/drivers/ppcfloor/linux/usr/src/kernels/2.6.32-71.el6.bgq/arch/powerpc/include/asm/atomic.h
 */
static __inline__ int SpinLock_AtomicIncrementAndReturn(volatile long long *v)
{
	long t;

	__asm__ __volatile__(
//// ????? took this out to compile ??? LWSYNC_ON_SMP
"	sync \n\
1:	ldarx	%0,0,%1		# atomic64_inc_return\n\
	addic	%0,%0,1\n\
	stdcx.	%0,0,%1 \n\
	bne-	1b \n\
	isync \n"
//// ????? took this out to compile ??? ISYNC_ON_SMP
	: "=&r" (t)
	: "r" (v)
	: "cc", "xer", "memory");

	return t;
}

static inline void
xxSpinLock(volatile SpinLockVar_t *aSpinLock, const char* s, int i)
{
	long long my_ticket=SpinLock_AtomicIncrementAndReturn(&(aSpinLock->requesting_ticket)) - 1 ;

	DPRINTF(FXLOG_SPINLOCK, "SpinLockVar@%p serving_ticket %lx requesting_ticket %lx line %ld file >%s<",
		(void *)aSpinLock,(uint64_t)aSpinLock->serving_ticket,
		(uint64_t)aSpinLock->requesting_ticket, (uint64_t)i, s);

	while (my_ticket != aSpinLock->serving_ticket)
		; /* Nothing, just spin */
}

static inline void
SpinUnlock(volatile SpinLockVar_t *aSpinLock)
{
	aSpinLock->serving_ticket += 1 ;
	DPRINTF(FXLOG_SPINLOCK, "SpinLockVar@%p serving_ticket %lx requesting_ticket %lx",
		(void *)aSpinLock, (uint64_t)aSpinLock->serving_ticket,
		(uint64_t)aSpinLock->requesting_ticket);
}

/*
 * Shouldn't normally need this since statics are implicitly initialised
 * to zero, but put it here in case we need a spinlock which isn't a static
 */
static inline void SpinInit(volatile SpinLockVar_t *aSpinLock)
{
	DPRINTF(FXLOG_SPINLOCK, "SpinLockVar@%p", (void *)aSpinLock);
	aSpinLock->requesting_ticket = 0 ;
	aSpinLock->serving_ticket = 0 ;
}

#endif
#endif

typedef uint64_t RoQ_Spinlock_t;  // NEED: lock routines usable by both linux and roq side
typedef RoQ_Atomic_t RoQ_AtomicCounter_t;

/*
 * BEGIN DEV MEM SIMPLE STUPID SRSW QUEUE
 * Simple, small (32B) srsw queue metadata
 * Note: this packs down but every update can result in a cache invalidate
 * Note: no power of 2 depth forced -- so mod is used. Assume little non productive polling
 */
struct KISS_SRSWQ {
	volatile uint64_t	mPut;
	volatile uint64_t	mGot;
	uint64_t		mItemArrayDmaAddr;
	uint64_t		mItemCount;
	uint64_t		mItemSize;
};


#ifndef FXLOG_KISS_SRSWQ
#define FXLOG_KISS_SRSWQ 0
#endif

static inline void
KISS_SRSWQ_Init(struct KISS_SRSWQ *aQ, uint64_t ItemSize, uint64_t ItemCount,
		uint64_t ItemArrayDmaAddr)
{
	memset(aQ, 0, sizeof *aQ);

	aQ->mPut              = 0;
	aQ->mGot              = 0;
	aQ->mItemArrayDmaAddr = ItemArrayDmaAddr;
	aQ->mItemSize         = ItemSize;
	aQ->mItemCount        = ItemCount;

#ifndef __LINUX__
	DPRINTF(FXLOG_KISS_SRSWQ,
		"KISS_SRSWQ@ 0x%p mItemArrayDmaAddr 0x%p mPutCount %lx "
		"mCachedGotCount %lx mItemCount %lx mItemSize %lx",
		(void *)aQ, (void *)aQ->mItemArrayDmaAddr, (uint64_t)aQ->mPut,
		(uint64_t)aQ->mGot, (uint64_t)aQ->mItemCount, (uint64_t)aQ->mItemSize);
#else
	if (FXLOG_KISS_SRSWQ)
		printk(KERN_INFO "%s aQ@ 0x%p ->mItemArrayDmaAddr %0llX "
			"->mItemCount %lld ->mItemSize %lld\n",
			__FUNCTION__, aQ, aQ->mItemArrayDmaAddr, aQ->mItemCount,
			aQ->mItemSize);
#endif
}

/*
 * returns and index into item array between 0 and mItemCount -1 or -1 for empty
 */
static inline int
KISS_SRSWQ_IsEmpty(struct KISS_SRSWQ *aQ)
{
	return (aQ->mPut == aQ->mGot) ? 1 : 0;
}

/*
 * returns and index into item array between 0 and mItemCount -1 or -1 for empty
 */
static inline int
KISS_SRSWQ_AccessNextReadItem(struct KISS_SRSWQ *aQ)
{
	int ItemIndex = -1;

	if (aQ->mGot != aQ->mPut) {
		/* got one */
#ifndef __LINUX__
		DASSERT(aQ->mPut > aQ->mGot, "%lx %lx", (uint64_t)aQ->mGot, (uint64_t)aQ->mPut);
		DASSERT(aQ->mPut - aQ->mGot <= aQ->mItemCount, "%lx %lx", (uint64_t)aQ->mPut, (uint64_t)aQ->mGot);
#endif
		ItemIndex = aQ->mGot % aQ->mItemCount;
	}

#ifndef __LINUX__
	DPRINTF(FXLOG_KISS_SRSWQ,
		"KISS_SRSWQ@ 0x%p Next Read Index %lx mItemArrayDmaAddr 0x%p mPutCount %lx mCachedGotCount %lx mItemCount %lx mItemSize %lx",
		(void *) aQ, (uint64_t)ItemIndex, (void *) aQ->mItemArrayDmaAddr, (uint64_t)aQ->mPut, (uint64_t)aQ->mGot,
		(uint64_t)aQ->mItemCount, (uint64_t)aQ->mItemSize);
#endif

	return(ItemIndex);
}

static inline int
KISS_SRSWQ_ReaccessReadItem(struct KISS_SRSWQ *aQ, int aItemIndex)
{
	/*
	 *  This routine could check to make sure an index
	 *  is still in the readers reading range between
	 *  what's been put and whats been got
	 */
#ifndef __LINUX__
	DPRINTF(FXLOG_KISS_SRSWQ,
		"KISS_SRSWQ@ 0x%p Next Read Index %lx mItemArrayDmaAddr 0x%p mPutCount %lx mCachedGotCount %lx mItemCount %lx mItemSize %lx",
		(void *) aQ, (uint64_t)aItemIndex, (void *) aQ->mItemArrayDmaAddr, (uint64_t)aQ->mPut, (uint64_t)aQ->mGot,
		(uint64_t)aQ->mItemCount, (uint64_t)aQ->mItemSize);
#endif

	return(aItemIndex);
}

/* END DEV MEM SIMPLE STUPID SRSW QUEUE */



#define ROQ_MAX_CN_NODES (1 * 1024)
#define ROQ_MAX_IO_NODES (1 * 1024)

/*
 * these arrays of structures are fat -- too fat for bg/p.
 * NOTE: thinning these will be work but it must be done.
 */
#define RoQ_MAX_MR	(16 * 1024)
#define RoQ_MAX_QP	(16 * 1024)
#define RoQ_INVALID_QP	(RoQ_MAX_QP + 1)


union RoQ_QP_Id {
	uint32_t mWhole;
	struct {
		unsigned mType    :  3;
		unsigned mVersion :  5;
		/* index into the static array in the RoQ shared space */
		unsigned mIndex   : 24;
	};
};

typedef union RoQ_QP_Id RoQ_QP_Id_t;


/* This should go in a device registration structure or some such */
enum {
	RoQ_LogBase2Of_HostPageSize = 16
};

/*
 * want an expression that will make it
 * clear pages are powers of 2
 */
#define RoQ_HostPageSize (0x01 << RoQ_LogBase2Of_HostPageSize)
#define RoQ_HostPageMask (RoQ_HostPageSize - 1)

#define RoQ_MAX_PageList (RoQ_MAX_MR)
#define RoQ_PageListMask (RoQ_MAX_PageList - 1) /* requires power of 2 */

/*
 * This structure is use to list a set of physical pages
 * use contigiously by one or more virtual address spaces.
 * Using a separate structure is required for shared memory MRs.
 * NEED: Might need to compress this stucture into bit fields
 *
 * NOTE: This structure has no lock and is only modified
 * by the RoQ_CNTL_Thread.
 */
struct RoQ_PageList {
	uint32_t	mVersionNumber;
	uint32_t	mReferenceCount;
	uint32_t	mPageCount;
	/* pointer to physically contigious host memory */
	uint32_t	*mPageArray;
};


/*
 * Data movers representation of registered
 * user communication buffers. Local key is index
 * into global MR list.
 */
struct roq_dm_mr {
	/*
	 * opaque back pointer to DD MR. MR is valid
	 * if pointer is valid.
	 */
	void		*dd_mr;

	uint32_t	mVersionNumber;
	uint32_t	pagesize;
	uint64_t	mHostPageArrayDmaAddr;
	uint64_t	mHostPageArrayPtr;
	uint64_t	mBaseVirtualAddress;
	uint64_t	mSize;
	uint32_t	mRights;
	uint32_t	lkey;
	uint32_t	rkey;
	uint16_t	inuse_tx; /* Ongoing data transfer from MR */
	uint16_t	inuse_rx; /* Ongoing data transfer to MR */
};

enum roq_doorbell_type {
	RoQ_DB_CQ	= 1,
	RoQ_DB_CTRL	= 2,
	RoQ_DB_LINK	= 3,
	RoQ_DB_NET	= 4
};
#define ROQ_DB_ID_SHIFT	2	/* two bits currently fit all db types */
#define ROQ_DB_TYPE_MASK	(0x000000ff >> (8 - ROQ_DB_ID_SHIFT ))

struct roq_dm_cq {
	struct KISS_SRSWQ	DmQueue;
	struct roq_cqe		*cq_virt;
	struct roq_cqe		*cq_phys;
	struct SpinLockVar_t	lock;
	enum roq_notify_flags	notify;
	uint32_t		dd_cq_id;	/* used for CQ doorbell */
	uint8_t			is_mmapped;	/* work queue mapped to user */
};
	
/*
 * DM QP control interface
 */
#define ROQ_SQ_DB_SIZE		1024	/* Pending Send queue items */
#define ROQ_RQ_DB_SIZE		1024	/* Pending READ.response items */
#define ROQ_TX_DB_SIZE		(ROQ_SQ_DB_SIZE + ROQ_RQ_DB_SIZE)
#define ROQ_CQ_DB_SIZE		(1024 * 8)
#define ROQ_CTRL_Q_SIZE		16	/* Set to '1' to force sequential IF */
#define ROQ_EVENT_Q_SIZE	(1024 * 4)   /* pending network events */

enum net_event_type {
	NET_EVENT_UNASSIGNED		= 0,
	NET_EVENT_HOSTUNREACHABLE	= 1,
	NET_EVENT_CQ_OVERFLOW		= 2,
	NET_EVENT_RQ_EMPTY		= 3,
	NET_EVENT_SRQ_EMPTY		= 4,
	NET_EVENT_SQ_ERROR		= 5,
	NET_EVENT_QP_DISCONNECT		= 6,
	NET_EVENT_PROTOCOL_ERROR	= 7,
	NET_EVENT_MEM_ACCESS_ERROR	= 9,
	NET_EVENT_MEM_BASE_BOUNDS_ERROR	= 10
};

struct roq_net_event {
	enum net_event_type	type;
	union {
		struct sockaddr_in	inaddr;
		uint32_t		resource_id;
		uint64_t		value;
		/* add more values for more types */
	};
};

#ifndef __LINUX__
extern void roq_post_net_event(enum net_event_type, void *, int);
#endif

enum {
	RoQ_CNTL_Command_Unknown,
	RoQ_CNTL_Command_ModifyQP,
	RoQ_CNTL_Command_QueryQP,
	RoQ_CNTL_Command_RegisterDB,
	RoQ_CNTL_Command_RevokeDB,
	RoQ_CNTL_Command_Discovery
};

struct roq_qp_attr {
	enum roq_qp_state qp_state;
	enum roq_qp_state cur_qp_state;
	uint32_t	sq_size;
	uint32_t	rq_size;
	uint32_t	max_send_sge;
	uint32_t	max_recv_sge;
	uint32_t	max_inline;
	uint32_t	dest_qp_num;
	uint32_t	trace_key;
	uint16_t	dest_node_id;
	uint16_t	io_link_id; /* not yet used */
};

enum roq_qp_cmd_mask {
	ROQ_QP_SET_STATE	= 1,
	ROQ_QP_CUR_STATE	= (1 << 1),
	ROQ_QP_SET_CAP		= (1 << 2),
	ROQ_QP_SET_DEST_QP	= (1 << 3),
	ROQ_QP_SET_DEST_NODE	= (1 << 4),
	ROQ_QP_SET_ACCESS_FLAGS	= (1 << 5),
	ROQ_QP_START_TRACE	= (1 << 6),
	ROQ_QP_STOP_TRACE	= (1 << 7)
};

struct RoQ_CNTL_Command_ModifyQP {
	int			QP_Num;
	struct roq_qp_attr	qp_attr;
	enum roq_qp_cmd_mask	cmd_mask;
};

struct RoQ_CNTL_Return_ModifyQP {
	int			QP_Num;
	struct roq_qp_attr	qp_attr;
	int mRc;
};

struct RoQ_CNTL_Command_QueryQP {
	int			QP_Num;
	enum roq_qp_cmd_mask	cmd_mask;
};

struct RoQ_CNTL_Return_QueryQP {
	int mRc;
	enum roq_qp_cmd_mask	cmd_mask;
	struct roq_qp_attr	qp_attr;
};

enum roq_db_client {
	ROQ_DB_QP_PRIV_CLIENT,
	ROQ_DB_QP_USER_CLIENT,
	ROQ_DB_CQ_CLIENT,
	ROQ_DB_NET_EVENT
};

#define ROQ_INVALID_DB	0

struct RoQ_CNTL_RegisterDB {
	uint64_t		SQ_Addr;
	uint64_t		RQ_Addr;
	enum roq_db_client	db_type;
};

struct RoQ_CNTL_Return_RegisterDB {
	uint64_t		client_addr;
	uint64_t		db_id;
};

struct RoQ_CNTL_RevokeDB {
	enum roq_db_client	db_type;
	uint64_t		db_id;
};

union RoQ_CNTL_Request {
	struct RoQ_CNTL_Command_ModifyQP	ModifyQP;
	struct RoQ_CNTL_Command_QueryQP		QueryQP;
	struct RoQ_CNTL_RegisterDB		RegisterDB;
	struct RoQ_CNTL_RevokeDB		RevokeDB;
};

union RoQ_CNTL_Return {
	struct RoQ_CNTL_Return_ModifyQP		ModifyQP;
	struct RoQ_CNTL_Return_QueryQP		QueryQP;
	struct RoQ_CNTL_Return_RegisterDB	RegisterDB;
};

struct RoQ_CNTL_Command {
	uint32_t Type;
	union {
		union RoQ_CNTL_Request	Request;
		union RoQ_CNTL_Return	Return;
	};
};

/*****************
*     QP         *
*****************/
/* make this at least the size of the DD spinlock */
#define DD_LOCK_SIZE	4
struct RoQ_DM_QP {
	struct roq_dm_cq *mSendCQ_DmaPtr;
	struct roq_dm_cq *mRecvCQ_DmaPtr;

	/* probably need a spin lock var */
	uint64_t	mVersionNumber;
	/* incremented every time this QP port is reused */
	uint64_t	mConnectionId;
	enum roq_qp_type	qp_type;
	enum roq_qp_state	qp_state;

	/*
	 * the transport peer Rank is used to
	 * select the proper tx thread doorbell
	 */
	int mPeerRank;

	/* circular queues of work requests for SQ/RQ/SRQ */
	struct KISS_SRSWQ	mSendQueue;
	struct KISS_SRSWQ	mRecvQueue;
	/* Points to associated SRQ if valid */
	struct KISS_SRSWQ	*SharedRecvQueue;

	struct roq_sqe		*mDD_SendQueuePtr;
	struct roq_sqe		*mMC_SendQueuePtr;
	struct SpinLockVar_t	mSendQueueLock;
	struct SpinLockVar_t	mRecvQueueLock;

	/* Information on device driver QP */
	void		*dd_qp;	/* Back pointer, used by device driver */
#ifdef __KERNEL__
	union {
		spinlock_t	dd_lock;
		char		dd_private[DD_LOCK_SIZE];
	};
#else
	char		dd_private[DD_LOCK_SIZE];
#endif
	uint32_t	dd_qp_id;
	uint8_t		sq_mmapped;	/* SQ work queues mapped to user */
	uint8_t		rq_mmapped;	/* RQ work queues mapped to user */

	struct iw_cm_id *cm_id;
};


#define RoQ_CNTL_MAX_REQUESTS	1
#define MAX_TX_THREADS		1

#define RoQ_DeviceMemory_Magic0 0xF0E5D9C3
#define RoQ_DeviceMemory_Magic1 0xA1B2C3D4

#define CQ_DOORBELL_QUEUE_SIZE	(16*1024)

#define ROQ_FW_VERSION		0

typedef unsigned long RoQ_Addr_t;

struct RoQ_DeviceMemory {
	uint32_t	mMagic0; /* MUST BE FIRST FIELD */
	uint32_t	mMagic1;
	uint32_t	mMagic2;
	uint32_t	mSizeOfDevMemInDevice;
	uint32_t	mVersion;

	RoQ_AtomicCounter_t mTestTime;

	/* The DD writes this addres on boot. This address
	 * is used to create a translation to kmalloc() objects
	 * Tes is currently used to allow the MC to calculate a
	 * linear offset to DD dynamic object which come out 
	 * which are drawn from a ++ allocator at the bottom of
	 * this struct.
	 */
	RoQ_Addr_t mDD_DevMem_Address;

	/*
	 * local ip addr -- defined as a function the nodes
	 * physical machine location -- written by the MC
	 */
	uint32_t mLocalIPV4Addr;
	uint32_t mLocalRank;
	uint32_t mPartitionSize;
	struct RoQ_NetworkDescriptor mNetworkDesc;
	struct RoQ_NetworkDescriptor mNetworkDesc_rem;
	uint16_t nearest_io_rank;

	/*
	 * Request/Response queue of depth ROQ_CTRL_Q_SIZE. Local device
	 * command interface. DD posts request, increments posted
	 * count. Waits for MC to signal completion via interrupt and incremented
	 * CNTL_RequestCompleted.
	 */
	RoQ_AtomicCounter_t	CNTL_RequestPosted;	/* Maintained by DD */
	RoQ_AtomicCounter_t	CNTL_RequestCompleted;	/* Maintained by DM */
	struct RoQ_CNTL_Command	CNTL_Command[ROQ_CTRL_Q_SIZE];

	struct RoQ_DM_QP	QP_Array[RoQ_MAX_QP];

	uint32_t	mMagic3;
	uint64_t	mMR_Next; /* NEED: a free list allocation system */

	struct roq_dm_mr	mMR_Array[RoQ_MAX_MR];
	struct RoQ_PageList	mPageListArray[RoQ_MAX_PageList];
	/*
	 * NOTE: during failover, the TxThread can be changed
	 * -- if so, this pointer will change.
	 */
	uint8_t	mTxThreadForDest[ROQ_MAX_IO_NODES];

	/* Doorbell queues */
	union SRSW_QueueSet_t	mTxDoorbellQueueSet[MAX_TX_THREADS];

	/*
	 * CQ Doorbell -- these queue sets are written by
	 * the TX/RX threads an are ready by Linux interrupt
	 * handler
	 */
	union SRSW_QueueSet_t  mDD_DoorbellQueueSet;
	uint32_t		DD_DoorbellQueue[2][CQ_DOORBELL_QUEUE_SIZE];

	uint32_t  mMagic4;

	/*
	 * the DD uses this to control the rate of
	 * fake interrupts polling.
	 */
	uint32_t mNextFakeInterruptPoll;

	struct SpinLockVar_t  int_lock;
	volatile uint32_t INTERRUPT_DISABLE;
	volatile uint32_t INTERRUPT_DISABLE_CNT;
	volatile uint32_t interrupt_pending;
};


extern struct RoQ_DeviceMemory *RoQ_DevMem;

#endif
