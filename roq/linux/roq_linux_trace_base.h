#ifndef _ROQ_LINUX_TRACE_BASE_H
#define _ROQ_LINUX_TRACE_BASE_H

#include <linux/highmem.h>
#include <linux/spinlock.h>
enum {
	k_pirbits = 10 ,
	k_pirsize = 1 << k_pirbits
};
typedef struct {
	char *kernelPage;
	int currentPage;
	int currentOffset;
} addressTable_t;

extern addressTable_t addressTable[k_pirsize];

typedef struct {
	spinlock_t mLockWordName; // Lock, so that both threads can use trace names coherently
	spinlock_t mLockWordData; // Lock, so that both threads can allocate new data pages coherently
	int mGeneration; // Generation number, so tracepoint names can be emitted again on each new trace run
	int mOn; // Whether tracing is currently on
	int mHiTraceStreamId; // for handing out trace stream IDs
	int mNextTraceId; // Where to put the next tracepoint name
	struct scatterlist * mTraceIdPlist; // Physical page where we are putting tracepoint names
	int nents; // Number of physical pages
	int data_page;
} PkLinuxTraceState;

extern PkLinuxTraceState LinuxTraceState;

#endif
