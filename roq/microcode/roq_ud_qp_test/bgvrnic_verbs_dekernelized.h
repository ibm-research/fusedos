#ifndef _BGROQ_VERBS_DEKERNELIZED
#define _BGROQ_VERBS_DEKERNELIZED

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>

#define GFP_KERNEL 0
#define kfree(a) free(a)
#define kmalloc(a, b) malloc(a)
#define kzalloc(a, b) malloc(a)

typedef int wait_queue_head_t;
struct net_device {
	char dev_addr[10];
	int BLA;
};

#include <stdint.h>
#include <stdlib.h> // for malloc

#if 0
#include <ib_verbs_dekernelized.h>
#include <asm/atomic.h>
static __inline__ int atomic_read(const atomic_t *v)
{
        return *v;
}

static __inline__ void atomic_inc(atomic_t *v)
{
	(*v)++;
}

static __inline__ int atomic_inc_return(atomic_t *v)
{
	(*v)++;
        return *v;
}

static __inline__ void atomic_dec(atomic_t *v)
{
	(*v)--;
}

static __inline__ int atomic_dec_return(atomic_t *v)
{
	(*v)--;
        return *v;
}
#endif

#include <asm/atomic.h>


#include <ib_verbs_dekernelized.h>
#if 0
typedef struct {
	volatile int counter;
} atomic_t;
#endif

static __inline__ int atomic_read(const atomic_t *v)
{
        return v->counter;
}

static __inline__ void atomic_inc(atomic_t *v)
{
	v->counter++;
}

static __inline__ int atomic_inc_return(atomic_t *v)
{
	v->counter++;
        return v->counter;
}

static __inline__ void atomic_dec(atomic_t *v)
{
	v->counter--;
}

static __inline__ int atomic_dec_return(atomic_t *v)
{
	v->counter--;
        return v->counter;
}

#define atomic_dec_and_test(v)          (atomic_dec_return((v)) == 0)

static inline void out_be32(volatile unsigned *addr, int val)
{
        __asm__ __volatile__("stw%U0%X0 %1,%0; sync"
                             : "=m" (*addr) : "r" (val));
}

static inline unsigned in_be32(const volatile unsigned *addr)
{
        unsigned ret;

        __asm__ __volatile__("lwz%U1%X1 %0,%1; twi 0,%0,0; isync"
                             : "=r" (ret) : "m" (*addr));
        return ret;
}


//From: http://rswiki.csie.org/lxr/http/source/include/linux/stddef.h?v=linux-2.6.28.4;a=ppc#L20
// however.... /usr/lib/gcc/powerpc64-suse-linux/4.1.2/include/stddef.h:414:1: warning: this is the location of the previous definition
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
// from http://rswiki.csie.org/lxr/http/source/include/linux/kernel.h?v=linux-2.6.28.4;a=ppc#L481
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define _NEED_ROQ_MAGIC_LOCATION
#include <roq_device_interface.h>

#include <ib_verbs_dekernelized.h>
#include "bgroq.h"
#include "bgroq_utils.h"
#include "bgroq_user.h"

#include <linux/prefetch.h>
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     prefetch(pos->member.next), &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))



#define BUG_ON
#define BUG()

#define DBG_ON 0
#define DBG_CM 0
#define DBG_OBJ 0
#define DBG_WR 0
#define DBG_EH 0
#define DBG_IWRX 0
#define DBG_IWTX 0
#define DBG_MM 0
//#define PAGE_SHIFT 0x10

#define KERN_INFO 0
#define KERN_EMERG 0

#define dprint( ... )
#define dprint_tsk( ... )
#define printk( ... )

#define ENTER
#define EXIT

#define wake_up
#define wait_event

#define init_rwsem
#define init_waitqueue_head
#define spin_lock_bh
#define spin_unlock_bh
#define spin_lock_irqsave
#define spin_unlock_irqrestore
#define spin_lock_init

#define likely(v) v
#define unlikely(v) v

#define PAGE_SIZE (64*1024*1024)

#define down_read
#define up_read
#define down_write
#define up_write

#endif /* _BGROQ_VERBS_DEKERNELIZED */
