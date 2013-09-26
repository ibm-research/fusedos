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

#ifndef __SRSW_QUEUE_SET_HPP__
#define __SRSW_QUEUE_SET_HPP__

#ifndef FXLOG_SRSW_QUEUE_SET
#define FXLOG_SRSW_QUEUE_SET 0
#endif

#ifndef __LINUX__
 #include <sys/types.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <signal.h>
 #include <roq_debug.h>
#endif


#ifndef __LINUX__
#define SRSW_QueueMalloc(n) ((char*)malloc(n))
#else
#define SRSW_QueueMalloc(n) ((char*) kmalloc(n, GFP_KERNEL))
#endif

#define UWORD uint64_t
typedef volatile uint64_t RoQ_Atomic_t;
#define L1_CACHE_SIZE (32)

static __inline__ int64_t RoQ_AtomicIncrementAndReturn(RoQ_Atomic_t *v)
{
        long t;

        __asm__ __volatile__(
	"sync\n"
"1:     ldarx   %0,0,%1         # atomic64_inc_return\n\
        addic   %0,%0,1\n\
        stdcx.  %0,0,%1 \n\
        bne-    1b\n"
	"\n\tisync\n"
        : "=&r" (t)
        : "r" (v)
        : "cc", "xer", "memory");

        return t;
}

static __inline__ void RoQ_AtomicIncrement(RoQ_Atomic_t *v)
{
        long t;
        __asm__ __volatile__(
	"sync\n"
"1:     ldarx   %0,0,%2         # atomic64_inc\n\
        addic   %0,%0,1\n\
        stdcx.  %0,0,%2 \n\
        bne-    1b"
	 "\n\tisync\n"
        : "=&r" (t), "+m" (*v)
        : "r" (v)
        : "cc", "xer");
}

static __inline__ int64_t RoQ_AtomicRead(RoQ_Atomic_t *v)
{
        long t;
        __asm__ __volatile__("ld%U1%X1 %0,%1" : "=r"(t) : "m"(*v));
        return t;
}


static unsigned
RealDepth(unsigned aDepth)
{
	/*
	 * Round up all queue depths to the next higher power of 2
	 * because then we can do some arithmetic by bit-masking
	 * rather then by division/modulus.
	 */
	unsigned int rd = 1 ;
	int x;

	for(x = 0 ; x < 32 ; x += 1) {
		if (rd >= aDepth)
			return rd ;
		rd <<= 1 ;
	}

	/*
	 * You could get here if someone tried to configure a queue with
	 * more than 2**31 elements; but this is not sensible for other
	 * reasons, so a 'firewall' trap is OK.
	 */
#ifndef __LINUX__
	DSTRONG_ASSERT(0, "%s %lx", "aDepth too big at ", (uint64_t)aDepth);
#endif
	return 1024 ;
}

struct SRSW_Queue_t {
	struct {
		UWORD	mPutCount;        /* only updated by the write side */
		UWORD	mCachedGotCount;  /* copied from the ReadSide when out of space */
		char	*mItemArray;	   /* here to avoid false sharing */
		uint64_t mItemSize;
		uint64_t mDepthMask;       /* ditto */
	} mWriteSide;

	struct {
		UWORD	mCachedPutCount;
		UWORD	mGotCount;
		char	*mItemArray;
		uint64_t mItemSize;
		uint64_t mDepthMask;
	} mReadSide;
} __attribute__ ((aligned(32)));

static int
SRSW_Queue_WriterConnect(struct SRSW_Queue_t *aQ, uint64_t aItemSize, uint64_t aReqDepth, void *aItemArray)
{
	int64_t Depth = aReqDepth;

	/* lose check that this queue hasn't been allocated yet. */
#ifndef __LINUX__
	DSTRONG_ASSERT(aQ->mReadSide.mItemArray == NULL, "%p", aQ->mReadSide.mItemArray);
	DSTRONG_ASSERT(aQ->mWriteSide.mItemArray == NULL, "%p", aQ->mWriteSide.mItemArray);
#endif

#ifdef __LINUX__
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "aQ@ 0x%p aDepth %lld aItemArray 0x%p aItemSize %lld\n",
			aQ, aReqDepth, aItemArray, aItemSize );
#else
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "aQ 0x%p aReqDepth %lx aItemArray 0x%p aItemSize %lx",
		(void *)aQ, (uint64_t)aReqDepth, (void *)aItemArray, (uint64_t)aItemSize);
#endif

	memset(aQ, 0, sizeof *aQ);

	if (aItemArray == NULL) {
		Depth = RealDepth(aReqDepth);
#ifndef __LINUX__
		DPRINTF(FXLOG_SRSW_QUEUE_SET, "%lx", (uint64_t)Depth);
#endif
		aQ->mWriteSide.mItemArray = SRSW_QueueMalloc(aItemSize * Depth);
#ifndef __LINUX__
		DSTRONG_ASSERT(aQ->mWriteSide.mItemArray, "%p", aQ->mWriteSide.mItemArray);
#endif
	} else {
		/*
		 * memory was allocated outside for the
 		 * item array -- make sure it's power of 2
 		 */
		if ((Depth & (Depth - 1))) {
#ifndef __LINUX__
			DPRINTF(FXLOG_SRSW_QUEUE_SET, "ERROR bad depth %lx", (uint64_t)Depth);
#endif

#ifdef __LINUX__
			printk(KERN_INFO "**** BAD DEPTH ***\n");
#endif
			return (-1 * __LINE__);
		}
		aQ->mWriteSide.mItemArray = (char *)aItemArray;
	}

	if (aQ->mWriteSide.mItemArray == NULL) {
#ifndef __LINUX__
		DPRINTF(FXLOG_SRSW_QUEUE_SET, "%s", "null item array");
#endif
		return (-1 * __LINE__);
	}

	/*
	 * Set the item array for debugging -- turned up
	 * a bad qpn during a doorbell.
	 */
	memset(aQ->mWriteSide.mItemArray, 0xFFFFFFFF, aItemSize * Depth);

	aQ->mWriteSide.mItemSize = aItemSize;
	aQ->mWriteSide.mDepthMask = Depth - 1;

	/*
 	 * This pattern, with the write side address of the Q metadata
 	 * in the GotCount and the CachedPutCount = 0 signals an unaccepted connect.
 	 * The connect will occur when the reader first tries to access this queue
 	 * The reader will determin if the ItemArray needs translatioan
 	 * ASSUMING IT IS A LINEAR TRANSLATION WITH THE QUEUE ADDRESS
 	 */
	aQ->mReadSide.mGotCount = (UWORD)aQ;
	aQ->mReadSide.mCachedPutCount = 0;

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_Queue_t@ 0x%p mItemArray @ 0x%p mGotCount %lx mDepthMask %lx mCachedPutCount %lx mItemSize %lx "
		"mPutCount %lx mCachedGotCount %lx mDepthMask %lx mItemSize %lx",
		(void *)aQ, (void *)aQ->mWriteSide.mItemArray, (uint64_t)aQ->mReadSide.mGotCount, (uint64_t)aQ->mReadSide.mDepthMask,
		(uint64_t)aQ->mReadSide.mCachedPutCount, (uint64_t)aQ->mReadSide.mItemSize, (uint64_t)aQ->mWriteSide.mPutCount,
		(uint64_t)aQ->mWriteSide.mCachedGotCount, (uint64_t)aQ->mWriteSide.mDepthMask, (uint64_t)aQ->mWriteSide.mItemSize);
#endif

	return 0;
}

/*
 * provide a pointer to the next available item on a queue
 * return null if queue is empty
 * return null if queue has not been connected yet (because it's empty)
 */
static inline void *
SRSW_Queue_AccessNextReadItem(struct SRSW_Queue_t* aQ)
{
	if (aQ->mReadSide.mGotCount >= aQ->mReadSide.mCachedPutCount) {
		if (aQ->mReadSide.mGotCount == aQ->mReadSide.mCachedPutCount) {
			/* this branch is the cost of auto-accept on the reader side */
			/* reach for a fresh copy of the write side's put count -- possible cache miss */
			aQ->mReadSide.mCachedPutCount = aQ->mWriteSide.mPutCount;
		} else {
			/* Auto-accept happens here */
			struct SRSW_Queue_t* aQ_WriteSideAddr = (struct SRSW_Queue_t *)aQ->mReadSide.mGotCount;

			if (aQ_WriteSideAddr == aQ) {
				/* no translation -- reader is in the same address space as the writer */
				aQ->mReadSide.mItemArray = aQ->mWriteSide.mItemArray;
			} else {
				/*
				 * linearly translate the ItemArray address based
				 * on the differnce between reader and writer queue address
				 *
				 * !!! Should swizzle be a signed data type in case
				 * the subtraction results in a negative number?
				 */
				long long swizzle = ((long long) (uintptr_t) aQ_WriteSideAddr)
							- ((long long) (uintptr_t) aQ);

#ifndef __LINUX__
				DPRINTF(FXLOG_SRSW_QUEUE_SET,
					"SRSW_QueueSet_t@ 0x%p adding swizzle delta %lx "
					"old aQ->mWriteSide.mItemArray 0x%p new aQ->mWriteSide.mItemArray - swizzle 0x%p\n",
					(void *) aQ, (uint64_t)swizzle, (void *) aQ->mWriteSide.mItemArray,
					(void *)((uint64_t)aQ->mWriteSide.mItemArray - swizzle));
#else
				if (FXLOG_SRSW_QUEUE_SET)
					printk(KERN_INFO "%s aQ@ 0x%p swizzle %lld orig mWriteSize.mItemArray "
						"0x%p siwzzled 0x%p (=ReadSide.mItemArray)\n",
					__FUNCTION__, aQ, swizzle, aQ->mWriteSide.mItemArray,
					(aQ->mWriteSide.mItemArray - swizzle));
#endif

				/*
				 * add the difference in addresses between the reader and writer
				 * to the reader's array pointer -- should be right now for the reader.
				 */
				aQ->mReadSide.mItemArray = aQ->mWriteSide.mItemArray - swizzle;
			}

			aQ->mReadSide.mItemSize = aQ->mWriteSide.mItemSize;
			aQ->mReadSide.mDepthMask = aQ->mWriteSide.mDepthMask;
			aQ->mReadSide.mGotCount = 0;
			aQ->mReadSide.mCachedPutCount = aQ->mWriteSide.mPutCount;

#ifndef __LINUX__
			DPRINTF(FXLOG_SRSW_QUEUE_SET,
				"SRSW_Queue_t@ 0x%p mItemArray @ 0x%p mGotCount %lx "
				"mDepthMask %lx mCachedPutCount %lx mItemSize %lx "
				"mPutCount %lx mCachedGotCount %lx mDepthMask %lx mItemSize %lx\n",
				(void *)aQ, (void *)aQ->mReadSide.mItemArray, (uint64_t)aQ->mReadSide.mGotCount,
				(uint64_t)aQ->mReadSide.mDepthMask, (uint64_t)aQ->mReadSide.mCachedPutCount,
				(uint64_t)aQ->mReadSide.mItemSize, (uint64_t)aQ->mWriteSide.mPutCount, (uint64_t)aQ->mWriteSide.mCachedGotCount,
				(uint64_t)aQ->mWriteSide.mDepthMask, (uint64_t)aQ->mWriteSide.mItemSize);
#else
			if (FXLOG_SRSW_QUEUE_SET)
				printk(KERN_INFO "%s() SETUP aQ@ 0x%p"
					" R.mItemArray 0x%p R.mGotCount %lld R.mDepthMask %lld"
					" R.mCachedPutCount %lld R.mItemSize %lld\n",
					__FUNCTION__, aQ, aQ->mReadSide.mItemArray, aQ->mReadSide.mGotCount,
					aQ->mReadSide.mDepthMask, aQ->mReadSide.mCachedPutCount, aQ->mReadSide.mItemSize );
#endif
		}
	}

#ifdef __LINUX__
	BUG_ON(aQ->mReadSide.mGotCount > aQ->mReadSide.mCachedPutCount);
#endif

	/* this causes a reference to the write side put count */
	if (aQ->mReadSide.mGotCount != aQ->mReadSide.mCachedPutCount) {
		/* got one */
#ifndef __LINUX__
		DASSERT(aQ->mReadSide.mCachedPutCount > aQ->mReadSide.mGotCount,
			"%lx %s %lx",
			(uint64_t)aQ->mReadSide.mGotCount, " ", (uint64_t)aQ->mReadSide.mCachedPutCount);

		DASSERT(aQ->mReadSide.mCachedPutCount - aQ->mReadSide.mGotCount <= aQ->mReadSide.mDepthMask,
			"%lx %s %lx",
			(uint64_t)aQ->mReadSide.mCachedPutCount, " ", (uint64_t)aQ->mReadSide.mGotCount);
#endif
		int64_t ItemIndex = aQ->mReadSide.mGotCount & aQ->mReadSide.mDepthMask;
#ifndef __LINUX__
		DPRINTF(FXLOG_SRSW_QUEUE_SET,
			"SRSW_Queue_t@ 0x%p ItemIndex %lx rv 0x%p mGotCount %lx mDepthMask %lx mCachedPutCount %lx "
			"mPutCount %lx R.mItemArray 0x%p W.mItemeArray 0x%p R.mItemSize 0x%p\n",
			(void *)aQ, (uint64_t)ItemIndex, (void *)(aQ->mReadSide.mItemArray + (ItemIndex * aQ->mReadSide.mItemSize)),
			(uint64_t)aQ->mReadSide.mGotCount, (uint64_t)aQ->mReadSide.mDepthMask, (uint64_t)aQ->mReadSide.mCachedPutCount,
			(uint64_t)aQ->mWriteSide.mPutCount, (void *)aQ->mReadSide.mItemArray, (void *)aQ->mWriteSide.mItemArray,
			(void *)aQ->mReadSide.mItemSize);
#else
		if (FXLOG_SRSW_QUEUE_SET)
			printk(KERN_INFO "%s() GOT ONE aQ@ 0x%p"
				" R.mItemArray 0x%p R.mGotCount %lld R.mDepthMask %lld"
				" R.mCachedPutCount %lld R.mItemSize %lld\n",
				__FUNCTION__, aQ, aQ->mReadSide.mItemArray, aQ->mReadSide.mGotCount,
				aQ->mReadSide.mDepthMask, aQ->mReadSide.mCachedPutCount, aQ->mReadSide.mItemSize );
#endif

		/*
		 * does this need to be here? The idea is to make sure that
		 * we don't see stale queue elements after metadata is updated.
		 */
#ifndef __LINUX__
		_bgp_msync();
#else
		wmb();
#endif

		return aQ->mReadSide.mItemArray + (ItemIndex * aQ->mReadSide.mItemSize);
	}

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_Queue_t@ 0x%p return( NULL ) mGotCount %lx mDepthMask %lx mCachedPutCount %lx "
		"mPutCount %lx R.mItemArray 0x%p W.mItemeArray 0x%p R.mItemSize 0x%p",
		(void *)aQ, (uint64_t)aQ->mReadSide.mGotCount, (uint64_t)aQ->mReadSide.mDepthMask,
		(uint64_t)aQ->mReadSide.mCachedPutCount, (uint64_t)aQ->mWriteSide.mPutCount,
		(void *)aQ->mReadSide.mItemArray, (void *)aQ->mWriteSide.mItemArray, (void *)aQ->mReadSide.mItemSize);
#else
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "%s() EMPTY aQ@ 0x%p R.mItemArray 0x%p"
			" R.mGotCount %lld R.mDepthMask %lld R.mCachedPutCount %lld R.mItemSize %lld\n",
			__FUNCTION__, aQ, aQ->mReadSide.mItemArray, aQ->mReadSide.mGotCount,
			aQ->mReadSide.mDepthMask, aQ->mReadSide.mCachedPutCount, aQ->mReadSide.mItemSize );
#endif

	return NULL;
}

static inline void
SRSW_Queue_ReleaseAccessedReadItem(struct SRSW_Queue_t* aQ)
{
	aQ->mReadSide.mGotCount++;
}

/*
 * returns the address of an Item slot or NULL
 * the caller may then cast to the item size
 */
static inline void *
SRSW_Queue_ReserveNextWriteItem(struct SRSW_Queue_t *aQ)
{
	int64_t ItemIndex;

	if ((aQ->mWriteSide.mPutCount - aQ->mWriteSide.mCachedGotCount) > aQ->mWriteSide.mDepthMask) {
		/*
		 * Queue is full because a bit above those masked is set
		 * Here we must update aQ->mCachedGotCount and hope more space appears
		 */
		aQ->mWriteSide.mCachedGotCount = aQ->mReadSide.mGotCount;
		if ((aQ->mWriteSide.mPutCount - aQ->mWriteSide.mCachedGotCount) > aQ->mWriteSide.mDepthMask) {
#ifdef __LINUX__
			if (FXLOG_SRSW_QUEUE_SET)
				printk(KERN_INFO "%s() aQ@ 0x%p FULL QUEUE:"
					"(W.mPutCount=%lld - W.mCachedGotCount=%lld) > W.mDepthMask=%lld  R.mItemArray 0x%p\n",
					__FUNCTION__,aQ,aQ->mWriteSide.mPutCount,aQ->mWriteSide.mCachedGotCount,
					aQ->mWriteSide.mDepthMask, aQ->mReadSide.mItemArray);
#else
			DPRINTF(FXLOG_SRSW_QUEUE_SET,
				"SRSW_Queue_t@ 0x%p return( NULL ) mGotCount %lx mDepthMask %lx mCachedPutCount %lx mPutCount %lx "
				"R.mItemArray 0x%p W.mItemeArray 0x%p R.mItemSize 0x%p\n",
				(void *)aQ, (uint64_t)aQ->mReadSide.mGotCount, (uint64_t)aQ->mReadSide.mDepthMask,
				(uint64_t)aQ->mReadSide.mCachedPutCount, (uint64_t)aQ->mWriteSide.mPutCount,
				(void *)aQ->mReadSide.mItemArray, (void *)aQ->mWriteSide.mItemArray, (void *)aQ->mReadSide.mItemSize);
#endif
			return NULL; /* can't write, full */
		}
	}
	ItemIndex = aQ->mWriteSide.mPutCount & aQ->mWriteSide.mDepthMask;

#ifndef __LINUX__
	DSTRONG_ASSERT(aQ->mWriteSide.mItemArray, "aQ@ 0x%p", (void *) aQ);
#endif

#ifdef __LINUX__
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "aQ@ %p ->mWriteSide.mItemArray 0x%p ItemIndex %lld ItemSize %lld ItemOffset %lld ItemAddr 0x%p\n",
			aQ, aQ->mWriteSide.mItemArray, ItemIndex, aQ->mWriteSide.mItemSize, ItemIndex*aQ->mWriteSide.mItemSize,
			&aQ->mWriteSide.mItemArray[ItemIndex * aQ->mWriteSide.mItemSize]);
#else
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"ItemIndex %lx rv 0x%p SRSW_Queue_t@ 0x%p R.mGotCount %lx R.mDepthMask %lx R.mCachedPutCount %lx W.mmPutCount %lx "
		"W.mItemeArray 0x%p W.mItemeArray 0x%p W.mItemSize 0x%p",
		(uint64_t)ItemIndex, (void *) (& aQ->mWriteSide.mItemArray[ ItemIndex * aQ->mWriteSide.mItemSize ]),
		(void *)aQ, (uint64_t)aQ->mReadSide.mGotCount, (uint64_t)aQ->mReadSide.mDepthMask,
		(uint64_t)aQ->mReadSide.mCachedPutCount, (uint64_t)aQ->mWriteSide.mPutCount,
		(void *)aQ->mReadSide.mItemArray, (void *)aQ->mWriteSide.mItemArray, (void *)aQ->mWriteSide.mItemSize);
#endif

	return ((void *)&aQ->mWriteSide.mItemArray[ItemIndex * aQ->mWriteSide.mItemSize]);
}


static inline int
SRSW_Queue_PostNextWriteItem(struct SRSW_Queue_t *aQ)
{
	/*
	 * NOTE: both msyncs are required!!! If you leave out the first,
	 * when the read side is hunting for the active queue, it could
	 * see an out of order put vs data values.
	 */

#ifndef __LINUX__
	_bgp_msync(); /* publish the put data before the put count */

	int64_t ItemIndex = aQ->mWriteSide.mPutCount & aQ->mWriteSide.mDepthMask;

	DASSERT((*((int64_t*)(&aQ->mWriteSide.mItemArray[ ItemIndex * aQ->mWriteSide.mItemSize ]))),
	      "%ld", (*((int64_t*)(&aQ->mWriteSide.mItemArray[ ItemIndex * aQ->mWriteSide.mItemSize ]))));
#else
	int64_t ItemIndex;

	do {
		asm volatile ("msync" : : : "memory");
	} while(0);

	wmb();
	ItemIndex = aQ->mWriteSide.mPutCount & aQ->mWriteSide.mDepthMask;
	/* check that the queue item is not zero */
	BUG_ON(!(*((int64_t*)(&aQ->mWriteSide.mItemArray[ItemIndex * aQ->mWriteSide.mItemSize]))));
#endif

	aQ->mWriteSide.mPutCount++;

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_Queue_t@ 0x%p ItemIndex %lx addr 0x%p value 0x%p mPutCount %lx mCachedGotCount %lx",
		(void *)aQ, (uint64_t)ItemIndex, (void *) (& aQ->mWriteSide.mItemArray[ ItemIndex * aQ->mWriteSide.mItemSize ]),
		(void *)(*((int64_t*)(&aQ->mWriteSide.mItemArray[ ItemIndex * aQ->mWriteSide.mItemSize ]))),
		(uint64_t)aQ->mWriteSide.mPutCount, (uint64_t)aQ->mWriteSide.mCachedGotCount);
#endif

	return 0;
}


#define SRSW_QueueSet_MaxQueueCount (2<<3) // power of two pls

union SRSW_QueueSet_t {
	struct {
		/*
		 * reader posts the address the queue set mapped to so
		 * connectors (writers) can swizzle queue item array pointers
		 */
		union SRSW_QueueSet_t *mReaderThisPointer;
		RoQ_Atomic_t	mWriteEventCount;
		RoQ_Atomic_t	mReadEventCount;
		/*
 		 * needs to be atomic since writers connect asyc and
 		 * get the queue slot from an atomic inc of this value
 		 */
		RoQ_Atomic_t	mMappedQueues;
		short	mPossibleReaderActiveQueue;
		short	mReaderActiveQueue;
		struct SRSW_Queue_t	mSRSW_QueueArray[SRSW_QueueSet_MaxQueueCount];
	};
	char pad[L1_CACHE_SIZE];
} __attribute__ ((aligned(32))) ;



/*
 * init is called by the roq to init the read side.
 * must be done before any writers connect.
 * writers may connect in any order in the reserved area
 * any queues polled before connection shall show empty
 */
static inline void
SRSW_QueueSet_Init(union SRSW_QueueSet_t* aQSet, int64_t aItemSize, int64_t aReserveArea  )
{
	memset(aQSet, 0, sizeof *aQSet);

	aQSet->mReaderActiveQueue = -1;
	/*
 	 * assign the reader address. as writers connect,
 	 * they can swizzle pointers for the reader to get to the item arrays
 	 */
	aQSet->mReaderThisPointer = aQSet;
	/* These queues should not by dynamically given out */
	aQSet->mMappedQueues      = aReserveArea;

#ifndef __LINUX__
	/* publish the put data before the put count */
	_bgp_msync();
#else
	do {
		asm volatile ("msync" : : : "memory");
	 } while(0);
#endif

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p ->mMappedQueues %lx ->aItemSize %lx",
		(void *) aQSet, (uint64_t)aQSet->mMappedQueues, (uint64_t)aItemSize);
#endif
}

/*
 * probably not the best way to connect, especially with more threads. but for now...
 * caller must know a pre-mapped srsw queue index or call with aQueueIndex of
 * -1 for dynamic allocation queue area may be provided
 */
static inline int
SRSW_QueueSet_WriterConnect(union SRSW_QueueSet_t *aQSet, uint64_t aQueueIndex,
			    int64_t aDepth, void *aItemArray, int64_t aItemSize )
{
	uint64_t QueueIndex;
	struct SRSW_Queue_t* srswq;

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_QueueSet_t@ 0x%p Reader QSet@ 0x%p aQueueIndex %lx aDepth %lx aItemSize %lx aItemArray@ 0x%lx mMappedQueues%lx",
		(void *) aQSet, (void *) aQSet->mReaderThisPointer, (uint64_t)aQueueIndex, (uint64_t)aDepth,
		(uint64_t)aItemSize, (uint64_t)aItemArray, (uint64_t)aQSet->mMappedQueues);

	/*
	 * we no longer expect the reader to connect before the writer.
	 * StrongAssertLogLine( aQSet->mReaderThisPointer ) << EndLogLine;
	 */
#else
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "aQSet@ 0x%p aQueueIndex %lld"
			" aDepth %lld aItemArray 0x%p aItemSize %lld MappedQueues %lld\n",
			aQSet, aQueueIndex, aDepth, aItemArray, aItemSize, aQSet->mMappedQueues);
#endif

	QueueIndex = aQueueIndex;

	if (QueueIndex == (uint64_t)-1)
		QueueIndex = RoQ_AtomicIncrementAndReturn(&aQSet->mMappedQueues);
	else {
		if (aQSet->mMappedQueues <= aQueueIndex)
			aQSet->mMappedQueues = aQueueIndex + 1;
	}

#ifndef __LINUX__
	DSTRONG_ASSERT(QueueIndex < SRSW_QueueSet_MaxQueueCount,
			"%lx %s %lx",
			(uint64_t)QueueIndex, " ", (uint64_t)SRSW_QueueSet_MaxQueueCount);
#endif

	srswq = &aQSet->mSRSW_QueueArray[QueueIndex];

	SRSW_Queue_WriterConnect(srswq, aItemSize, aDepth, aItemArray);

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p AtomicIncremented mConnectQueues - now %lx",
		(void *) aQSet, (uint64_t)aQSet->mMappedQueues);
#endif

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_QueueSet_t@ 0x%p Reader QSet@ 0x%p Allocated as QueueIndex %lx srswq->mReadSide.mItemArray 0x%p srswq->mWriteSide.mItemArray 0x%p",
		(void *) aQSet, (void *) aQSet->mReaderThisPointer, (uint64_t)QueueIndex,
		(void *) srswq->mReadSide.mItemArray, (void *) srswq->mWriteSide.mItemArray);
#endif
	return 0;
}

#define SRSW_QueueSet_LogInfo(aQSet)               \
    DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_LogInfo() called from here aQueSet@ 0x%lx", (uint64_t)aQSet);     \
							    xSRSW_QueueSet_LogInfo(aQSet )

static inline void
xSRSW_QueueSet_LogInfo(union SRSW_QueueSet_t* aQSet)
{
#ifndef __LINUX__
	DPRINTF(1,
		"SRSW_QueueSet_t@ 0x%p mReaderThisPointer 0x%p ActiveEvents %lx mWriteEventCount %lx mReadEventCount %lx mMappedQueues %lx mReaderActiveQueue %lx",
		(void *) aQSet, (void *) aQSet->mReaderThisPointer, (uint64_t)aQSet->mWriteEventCount - aQSet->mReadEventCount,
		(uint64_t)aQSet->mWriteEventCount, (uint64_t)aQSet->mReadEventCount, (uint64_t)aQSet->mMappedQueues, (uint64_t)aQSet->mReaderActiveQueue);
#endif

	struct SRSW_Queue_t * srswq;
	int i;

	/* for( int i = 0; aQSet->mMappedQueues ; i++) */
	for(i = 0; i < 2 ; i++) {
		srswq = &aQSet->mSRSW_QueueArray[i];

#ifndef __LINUX__
		DPRINTF(1,
			"QueueIndex %lx SRSW_Queue 0x%p R.mItemArray 0x%p W.mItemArray 0x%p R.mGotCount %lx R.mCachedPutCount %lx "
			"R.mDepthMask %lx W.mPutCount %lx W.mCachedGotCount %lx W.mDepthMask %lx",
			(uint64_t)i, (void *) srswq, (void *)srswq->mReadSide.mItemArray, (void *)srswq->mWriteSide.mItemArray,
			(uint64_t)srswq->mReadSide.mGotCount, (uint64_t)srswq->mReadSide.mCachedPutCount, (uint64_t)srswq->mReadSide.mDepthMask,
			(uint64_t)srswq->mWriteSide.mPutCount, (uint64_t)srswq->mWriteSide.mCachedGotCount, (uint64_t)srswq->mWriteSide.mDepthMask);
#endif

	}
	return;
}

/*
 * this is called to poll the whole queue set.
 * it uses atomically written aggregate read/write counts
 * to see if there is anything in any queue.
 */
static inline int
SRSW_QueueSet_GetAggregateItemCount(union SRSW_QueueSet_t * aQSet)
{
	UWORD ActiveEvents = aQSet->mWriteEventCount - aQSet->mReadEventCount;
	return ActiveEvents;
}


/*
 * this is called when there is a known active element
 */
static inline void *
SRSW_QueueSet_AccessNextReadItem(union SRSW_QueueSet_t *aQSet)
{
	int SnapshotActiveOfQueueIndex;
	void *ItemPtr = NULL;
	int q, i;
	struct SRSW_Queue_t *srswq;

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_QueueSet_t@ 0x%p ActiveEvents %lx mWriteEventCount %lx mReadEventCount %lx mMappedQueues %lx",
		(void *) aQSet, (uint64_t)aQSet->mWriteEventCount - aQSet->mReadEventCount, (uint64_t)aQSet->mWriteEventCount,
		(uint64_t)aQSet->mReadEventCount, (uint64_t)aQSet->mMappedQueues);

	DASSERT(aQSet->mWriteEventCount - aQSet->mReadEventCount, "%lx %lx",
		(uint64_t)aQSet->mWriteEventCount, (uint64_t)aQSet->mReadEventCount);

	DASSERT(aQSet->mReaderActiveQueue == -1, "%d", aQSet->mReaderActiveQueue);
#else
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "%s aQ@ 0x%p AggregateItemCount %d\n",
			__FUNCTION__, aQSet, SRSW_QueueSet_GetAggregateItemCount(aQSet));
#endif

	/* take a snapshot of the best guess at the last queue posted */
	SnapshotActiveOfQueueIndex = aQSet->mPossibleReaderActiveQueue;

	for(i = 0; ItemPtr == NULL ; i++) {
		q = (SnapshotActiveOfQueueIndex + i) % aQSet->mMappedQueues;
		srswq = &aQSet->mSRSW_QueueArray[ q ];

		ItemPtr = SRSW_Queue_AccessNextReadItem(srswq);
	}

	aQSet->mReaderActiveQueue = q;

#ifndef __LINUX__
	/*
	 * you should poll first then call this routine
	 * only when sure an item is in the queue set
	 */
	DASSERT(ItemPtr, "%p", ItemPtr);

	DPRINTF(FXLOG_SRSW_QUEUE_SET,
		"SRSW_QueueSet_t@ 0x%p mReaderActiveQueue %lx ItemPtr 0x%p mGotCount %lx mDepthMask %lx mCachedPutCount %lx mPutCount %lx",
		(void *) aQSet, (uint64_t)aQSet->mReaderActiveQueue, (void *)ItemPtr, (uint64_t)srswq->mReadSide.mGotCount,
		(uint64_t)srswq->mReadSide.mDepthMask, (uint64_t)srswq->mReadSide.mCachedPutCount, (uint64_t)srswq->mWriteSide.mPutCount);
#endif
	/*
	 *  this should almsot be an assert since we probably polled
	 *  before calling the access
	 */
	return ItemPtr; 
  }

static inline void
SRSW_QueueSet_ReleaseAccessedReadItem(union SRSW_QueueSet_t *aQSet)
{
#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p", (void *) aQSet);
	DASSERT(aQSet->mReaderActiveQueue != -1, "%d", aQSet->mReaderActiveQueue);
#endif
	aQSet->mSRSW_QueueArray[ aQSet->mReaderActiveQueue ].mReadSide.mGotCount++;
	aQSet->mReadEventCount++;
	aQSet->mReaderActiveQueue = -1; /* this bit could come out */
}

static inline void *
SRSW_QueueSet_ReserveNextWriteItem(union SRSW_QueueSet_t* aQSet, int64_t aQueueIndex)
{
	struct SRSW_Queue_t * srswq;
#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p aQueueIndex %lx", (void *) aQSet, (uint64_t)aQueueIndex);
#else
	if (FXLOG_SRSW_QUEUE_SET)
		printk(KERN_INFO "%s() aQSet@ 0x%p aQueueIndex %lld\n", __FUNCTION__, aQSet, aQueueIndex);
#endif
	srswq = &aQSet->mSRSW_QueueArray[aQueueIndex];

	return SRSW_Queue_ReserveNextWriteItem(srswq);
}


static inline void *
SRSW_QueueSet_SpinReserveNextWriteItem(union SRSW_QueueSet_t *aQSet, int64_t aQueueIndex)
{
#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p aQueueIndex %lx", (void *) aQSet, (uint64_t)aQueueIndex);
#endif

	void * ItemPtr = NULL;

	while (ItemPtr == NULL)
		ItemPtr = SRSW_QueueSet_ReserveNextWriteItem(aQSet, aQueueIndex);

	return ItemPtr;
}

static inline int
SRSW_QueueSet_PostNextWriteItem(union SRSW_QueueSet_t *aQSet, int64_t aQueueIndex)
{
#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p aQueueIndex %lx", (void *) aQSet, (uint64_t)aQueueIndex);
#endif
	struct SRSW_Queue_t *srswq = &aQSet->mSRSW_QueueArray[aQueueIndex];

	SRSW_Queue_PostNextWriteItem(srswq);

#ifndef __LINUX__
	/* publish the put count in the queue before the atomic inc */
	_bgp_msync();
#else
	do {
		asm volatile ("msync" : : : "memory");
	} while (0);
#endif

	/* increment the total event count of this queue set */
	RoQ_AtomicIncrement(&aQSet->mWriteEventCount);

#ifndef __LINUX__
	DPRINTF(FXLOG_SRSW_QUEUE_SET, "SRSW_QueueSet_t@ 0x%p WriteEventCount %lx aQueueIndex %lx mPutCount %lx mCachedGotCount %lx",
		(void *) aQSet, (uint64_t)aQSet->mWriteEventCount, (uint64_t)aQueueIndex, (uint64_t)srswq->mWriteSide.mPutCount,
		(uint64_t)srswq->mWriteSide.mCachedGotCount);
#endif

	return 0;
}

#endif
