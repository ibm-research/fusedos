/*
 * RoQ device driver for Linux
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _ROQ_DEBUG_H
#define _ROQ_DEBUG_H

//#include <asm/uaccess.h>
//#include <linux/hardirq.h>	/* in_interrupt() */

#undef KERN_INFO
#define KERN_INFO KERN_EMERG

/*
 * dprint: Selective debug printing
 *
 * Use an OR combination of DBG_* as dbgcat in dprint*(dbgcat,...)
 * to assign debug messages to categories:
 *
 * dbgcat	Debug message belongs to category
 * -----------------------------------------------------------------------------
 * DBG_ON	Always on, for really important events or error conditions
 * DBG_TMP	Temporarily on for fine-grained debugging
 * DBQ_OBJ	Object management (object construction/destruction/refcounting)
 * DBG_MM	Memory management
 * DBG_EH	Event handling (completion events and asynchronous events)
 * DBG_CM	Connection management, QP states
 * DBG_WR	Work requests
 * DBG_IWTX	iWARP TX path
 * DBG_IWRX	iWARP RX path
 * DBG_SK	Socket operations
 * DBG_KT	Kernel threads
 * DBG_IRQ	Interrupt context (SoftIRQ or HardIRQ)
 * DBG_DM	Device management
 * DBG_DATA	Application data (payload)
 * DBG_QP	QP references (tentative dbg code)
 * DBG_TRC Performance tracing
 * DBG_ALL	All categories above
 */
#define DBG_ON		0x00000001
#define DBG_TMP		0x00000002
#define DBG_OBJ		0x00000004
#define DBG_MM		0x00000008
#define DBG_EH		0x00000010
#define DBG_CM		0x00000020
#define DBG_WR		0x00000040
#define DBG_IWTX	0x00000080
#define DBG_IWRX	0x00000100
#define DBG_SK		0x00000200
#define DBG_KT		0x00000400
#define DBG_IRQ		0x00000800
#define DBG_DM		0x00001000
#define DBG_DATA	0x00002000
#define DBG_QP		0x00004000
#define DBG_TRC		0x00008000
#define DBG_ALL		(DBG_IRQ|DBG_KT|DBG_SK|DBG_IWRX|DBG_IWTX|DBG_WR|\
DBG_CM|DBG_EH|DBG_MM|DBG_OBJ|DBG_TMP|DBG_DM|DBG_ON|DBG_DATA|DBG_QP|DBG_TRC)
#define DBG_ALL_NODATA	(DBG_IRQ|DBG_KT|DBG_SK|DBG_IWRX|DBG_IWTX|DBG_WR|\
DBG_CM|DBG_EH|DBG_MM|DBG_OBJ|DBG_TMP|DBG_DM|DBG_ON|DBG_TRC)
#define DBG_SEL_NODATA  (DBG_IRQ|DBG_KT|DBG_SK|DBG_IWRX|DBG_IWTX|\
DBG_CM|DBG_EH|DBG_MM|DBG_OBJ|DBG_TMP|DBG_DM|DBG_ON|DBG_TRC)
#define DBG_CTRL	(DBG_ON|DBG_CM|DBG_DM|DBG_QP)

/*
 * Set DPRINT_MASK to tailor your debugging needs:
 *
 * DPRINT_MASK value		Enables debug messages for
 * ---------------------------------------------------------------------
 * DBG_ON			Important events / error conditions only
 *				(minimum number of debug messages)
 * OR-ed combination of DBG_*	Selective debugging
 * DBG_KT|DBG_ON		Kernel threads
 * DBG_ALL			All categories
 */
#if 0
#define DPRINT_MASK	DBG_ON
#define DPRINT_MASK	(DBG_KT|DBG_ON)
#define DPRINT_MASK	(DBG_OBJ|DBG_ON)
#define DPRINT_MASK	(DBG_MM|DBG_ON)
#define DPRINT_MASK	(DBG_EH|DBG_ON)
#define DPRINT_MASK	(DBG_IWTX|DBG_CM|DBG_TMP|DBG_ON)
#define DPRINT_MASK	(DBG_IWRX|DBG_CM|DBG_TMP|DBG_ON)
#define DPRINT_MASK	(DBG_CM|DBG_TMP|DBG_ON)
#define DPRINT_MASK	(DBG_KT|DBG_CM|DBG_TMP|DBG_ON)
#define DPRINT_MASK	(DBG_CM|DBG_ON)
#define DPRINT_MASK	(DBG_WR|DBG_ON)
#define DPRINT_MASK	(DBG_CM|DBG_MM|DBG_EH|DBG_OBJ|DBG_WR)
#define DPRINT_MASK	DBG_DM
#define DPRINT_MASK	~DBG_ALL
#endif

#if 0
#define DPRINT_MASK	(DBG_CM)
#else
//#define DPRINT_MASK	(DBG_TRC)
#define DPRINT_MASK 0
//#define DPRINT_MASK DBG_SEL_NODATA
//#define DPRINT_MASK DBG_ON
#endif

inline static char* PN2FN(char *PN) {
	char* ch;
	char* rv = PN;

	for (ch = PN; *ch; ch++) {
		if (*ch == '/')
			rv = ch;
	}
	return rv;
}

/**
 * dprint - Selective debug print for process, SoftIRQ or HardIRQ context
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header
 * - "( pid /cpu) __FUNCTION__" for process context
 * - "( irq /cpu) __FUNCTION__" for IRQ context
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * @fmt		: printf compliant format string
 * @args	: printf compliant argument list
 */
#if DPRINT_MASK == 0
#define dprint(dbgcat, loglevel, fmt, args...)
#else
#define dprint(dbgcat, loglevel, fmt, args...)				\
	do {								\
		if ((dbgcat) & DPRINT_MASK) {				\
			if (!in_interrupt())				\
				printk(loglevel "(%5d/%1d) %s() " fmt " %s %d\n",	\
					current->pid,			\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);		\
			else						\
				printk(loglevel "( irq /%1d) %s() " fmt " %s %d\n",	\
						current_thread_info()->cpu,\
						__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);	\
		}							\
	} while (0)
#endif


/**
 * dprint_tsk - Selective debug print for process/task context
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header "( pid /cpu) __FUNCTION__".
 * More efficient than dprint() if context is known to be process context.
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * @fmt		: printf compliant format string
 * @args	: printf compliant argument list
 */
#if DPRINT_MASK == 0
#define dprint_tsk(dbgcat, loglevel, fmt, args...)
#else
#define dprint_tsk(dbgcat, loglevel, fmt, args...)			\
	do {								\
		if ((dbgcat) & DPRINT_MASK)				\
			printk(loglevel "(%5d/%1d) %s() " fmt " %s %d\n",	\
					current->pid,			\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__ ); \
	} while (0)
#endif


/**
 * dprint_irq - Selective debug print for SoftIRQ or HardIRQ context
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header "( irq /cpu) __FUNCTION__".
 * More efficient than dprint() if context is known to be SoftIRQ or HardIRQ.
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * @fmt		: printf compliant format string
 * @args	: printf compliant argument list
 */
#define dprint_irq(dbgcat, loglevel, fmt, args...)			\
	do {								\
		if ((dbgcat) & DPRINT_MASK)				\
			printk(loglevel "( irq /%1d) %s" fmt " %s %d\n",		\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args, PN2FN(__FILE__), __LINE__);		\
	} while (0)


/**
 * dprint_mem - Selective debug print for memory
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header
 *	"( pid /cpu) __FUNCTION__" fmt "dprint_mem(start)\n"
 * and ending with trailer
 *	"( pid /cpu) __FUNCTION__" fmt "dprint_mem(end)\n"
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * TODO: Complete this ...
 *
 * @fmt		: printf compliant format string for header/trailer
 * @args	: printf compliant argument list for header/trailer
 */
#if DPRINT_MASK == 0
#define dprint_mem(dbgcat, loglevel, mem_name, kva, num_bytes, fmt, args...)
#else
#define dprint_mem(dbgcat, loglevel, mem_name, kva, num_bytes, fmt, args...) {\
	do {								\
		if ((dbgcat) & DPRINT_MASK) {				\
			printk(loglevel "(%5d/%1d) %s" fmt		\
					"dprint_mem(start) %s %d\n",		\
					current->pid,			\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);		\
			__roq_utils_mem_print(mem_name, kva, num_bytes);\
			printk(loglevel "(%5d/%1d) %s" fmt " %s %d\n"		\
					"dprint_mem(end) %s %d\n",		\
					current->pid,			\
					current_thread_info()->cpu,	\
					 __FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);	\
		}							\
	} while (0);							\
}
#endif


/**
 * dprint_mem_irq - Selective debug print for memory for SoftIRQ/HardIRQ context
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header
 *	"( irq /cpu) __FUNCTION__" fmt "dprint_mem(start)\n"
 * and ending with trailer
 *	"( irq /cpu) __FUNCTION__" fmt "dprint_mem(end)\n"
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * TODO: Complete this ...
 *
 * @fmt		: printf compliant format string for header/trailer
 * @args	: printf compliant argument list for header/trailer
 */
#if DPRINT_MASK == 0
#define dprint_mem_irq(dbgcat, loglevel, mem_name, kva, num_bytes,	\
		fmt, args...)
#else
#define dprint_mem_irq(dbgcat, loglevel, mem_name, kva, num_bytes,	\
		fmt, args...) {						\
	do {								\
		if ((dbgcat) & DPRINT_MASK) {				\
			printk(loglevel "( irq /%1d) %s" fmt		\
					"dprint_mem(start) %s %d\n",		\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);		\
			__roq_utils_mem_print(mem_name, kva, num_bytes);\
			printk(loglevel "( irq /%1d) %s" fmt		\
					"dprint_mem(end) %s %d\n",		\
					current_thread_info()->cpu,	\
					 __FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);	\
		}							\
	} while (0);							\
}
#endif


/**
 * dprint_kvec - Selective debug print for a struct kvec array
 *
 * Debug print with selectable debug categories and loglevel,
 * starting with header "( pid /cpu) __FUNCTION__".
 * starting with header
 *	"( pid /cpu) __FUNCTION__" fmt "dprint_kvec(start)\n"
 * and ending with trailer
 *	"( pid /cpu) __FUNCTION__" fmt "dprint_kvec(end)\n"
 *
 * @dbgcat	: Set of debug categories (OR-ed combination of DBG_* above),
 *		  to which this debug message is assigned.
 * @loglevel	: One of KERN_DEBUG, KERN_INFO, KERN_NOTICE,
 *		  KERN_WARNING, KERN_ERR ...
 * TODO: Complete this ...
 *
 * @fmt		: printf compliant format string for header/trailer
 * @args	: printf compliant argument list for header/trailer
 */
#if DPRINT_MASK == 0
#define dprint_kvec(dbgcat, loglevel, vec_name, vec, num_elts, fmt, args...)
#else
#define dprint_kvec(dbgcat, loglevel, vec_name, vec, num_elts, fmt, args...) {\
	do {								\
		if ((dbgcat) & DPRINT_MASK) {				\
			printk(loglevel "(%5d/%1d) %s() " fmt		\
					"dprint_kvec(start) %s %d\n",		\
					current->pid,			\
					current_thread_info()->cpu,	\
					__FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);		\
			__roq_utils_kvec_print(vec_name, vec, num_elts);\
			printk(loglevel "(%5d/%1d) %s() " fmt		\
					"dprint_kvec(end) %s %d\n",		\
					current->pid,			\
					current_thread_info()->cpu,	\
					 __FUNCTION__, ## args , PN2FN(__FILE__), __LINE__);	\
		}							\
	} while (0);							\
}
#endif


/* Alternative debug printing */

#undef ROQ_DEBUG

#ifdef ROQ_DEBUG

#define printd(format, args...) \
	do { printk(format, ## args); } while (0)

#else

#define printd(format, args...) \
	do { ; } while (0)

#endif


/*
 * Disable these macros for now - they generate way too much debug output
 *
 * TODO:
 * Replace uses of ENTER and EXIT selectively by dprint_...() where needed.
 */
#if (DPRINT_MASK >= DBG_ALL)
#define ENTER	do {							\
			if (current != NULL) {				\
				printk(KERN_EMERG "(%d)->%30s() %s %d\n",	\
					current->pid, __FUNCTION__,__FILE__,__LINE__);	\
			} else						\
				 printk(KERN_EMERG "(?)->%30s() %s %d\n", __FUNCTION__,__FILE__,__LINE__);\
		} while (0)

#define EXIT	do {							\
			if (current != NULL) {				\
				printk(KERN_EMERG "(%d)<-%30s() %s %d\n",	\
					current->pid, __FUNCTION__,__FILE__,__LINE__);	\
			} else						\
				 printk(KERN_EMERG "(?)<-%30s() %s %d\n", __FUNCTION__,__FILE__,__LINE__);\
		} while (0)
#else
#define ENTER
#define EXIT
#endif

#endif
