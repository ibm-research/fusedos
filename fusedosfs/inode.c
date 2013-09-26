/* ================================================================ */
/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2012                              */
/*                                                                  */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */
/* ================================================================ */

#include <asm/cputhreads.h>
#include <asm/elf.h>
#include <asm/page.h>
#include <asm/uaccess.h>
#include <asm-generic/io.h>
#include <firmware/include/Firmware_Interface.h>
#include <hwi/include/common/bgq_bitnumbers.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

#include <linux/jiffies.h>

// differentiate between (old) kernel 3.5 and 2.6, and (new) 3.4
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0)) && (LINUX_VERSION_CODE > KERNEL_VERSION(3,0,0))
#include <platforms/bgq/fusedos_config.h>
#include <platforms/bgq/fusedos.h>
#else
#include <fusedos_config.h>
#include <fusedos.h>
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yoonho Park");

#define FUSEDOSFS_MAGIC 0xdeadbeef
#define FUSEDOSFS_DEBUG(...) if (fusedos_config->fusedos_debug) printk(__VA_ARGS__);

// From hwi/include/bqc/A2_core.h
#define  MAS0_ATSEL(x)             _B1(32,x)       //  Array Type Select: 0=TLB, 1=LRAT
#define   MAS0_ATSEL_TLB           MAS0_ATSEL(0)   //   TLB
#define    MAS0_HES_HW_LRU         _B1(49,1)       //   TLB entry is selected by the hardware LRU algorithm
#define  MAS0_WQ(x)                _B2(51,x)       //  Write Qualifier regarding TLB Reservation
#define   MAS0_WQ_ALWAYS            MAS0_WQ(0)     //   Always overwrite regardless of reservation
#define   MAS1_TSIZE_1GB            MAS1_TSIZE(10) //   IND=0         LRAT
#define  MAS1_V(x)                 _B1(32,x)       //  Valid
#define  MAS7_3_RPN(x)             _B30(51,x)      // Entire RPN
#define  MMUCR3_X(x)               _B1(49,x)       //  Exclusion Range Enable
#define  MMUCR3_R(x)               _B1(50,x)       //  Reference
#define  MMUCR3_C(x)               _B1(51,x)       //  Change
#define  MMUCR3_ECL(x)             _B2(53,x)       //  Extended Class
#define  MMUCR3_CLASS(x)           _B2(55,x)       //  Class
#define  MMUCR3_ThdID(x)           _B4(63,x)       //  Thread Identifier

// From hwi/include/bqc/A2_inlines.h
__INLINE__ uint64_t ProcessorVersion(void)
{
    return (mfspr(SPRN_PVR));
}

#ifdef CONFIG_BGQPM
// Should be defined in upc_hw.h
upc_c_mmio_t *const  upc_c = (upc_c_mmio_t* const) UPC_C_MMIO_ADDR;
#endif /* CONFIG_BGQPM */

extern Firmware_Interface_t* _fw;
extern void* spc_monitor;
extern fusedos_config_t* fusedos_config;
extern spc_context_t* spc_context;
extern wait_queue_head_t* cl_wait;

extern void do_message_pass(int, int);
extern int spc_memory_init(void);

extern void (*spc_ipi_fp)(int, uint64_t);

void spc_ipi_ko(int cpu, uint64_t spc_c2c_status)
{
    int i, wakeup = 0;
    uint64_t mask = 0x8000000000000000;

    // Search for the SPC (SPCs) that sent the IPI and wake it (them) up
    for (i = 0; i < fusedos_config->nr_spcs; i++) {
        if (mask & spc_c2c_status) {
            FUSEDOSFS_DEBUG("FUSEDOS spc_ipi: spc %d, spc_c2c_status %016llx\n", i, spc_c2c_status);
            spc_context[i].ipi_wakeup = 1;
            wakeup = 1;
            asm volatile("msync; isync");

            // wake up selectively
            wake_up_interruptible(&(cl_wait[i]));

            // Reset the bit we processed
            spc_c2c_status = spc_c2c_status & ~mask;
            // Exit early if there are no additional bits on
            if (!spc_c2c_status) break;
        }
        mask = mask>>1;
    }
}

static int fusedosfs_monitor_open(struct inode *inode, struct file *filp)
{
    filp->private_data = spc_monitor;
    return 0;
}

static int fusedosfs_monitor_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    void* monitor = (void*)(filp->private_data);
    unsigned long pfn = virt_to_phys(monitor) >> PAGE_SHIFT;
    int ret = remap_pfn_range(vma, vma->vm_start, pfn, SPC_MONITOR_SIZE, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

static int fusedosfs_context_open(struct inode *inode, struct file *filp)
{
    filp->private_data = spc_context;
    return 0;
}

static int fusedosfs_context_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long length;
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    if (length != SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE) {
        printk("FUSEDOS fusedosfs_context_mmap_file: ERROR, length 0x%lx, SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE 0x%x\n", length, SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE);
        return -EIO;
    }
    pfn = virt_to_phys((void*)(filp->private_data)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

static int fusedosfs_memory_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long length;
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    if (length == 0 || (length % SPC_MEMORY_SIZE) != 0) {
        printk("FUSEDOS fusedosfs_memory_mmap_file: ERROR, length 0x%lx, SPC_MEMORY_SIZE 0x%x\n", length, SPC_MEMORY_SIZE);
        return -EIO;
    }
    pfn = virt_to_phys((void *)(((spc_context_t*)(filp->private_data))->mem_bot)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

// From hwi/include/bqc/A2_inlines.h
// Load Reserved: 64bit atom
static inline uint64_t LoadReserved( volatile uint64_t *pVar )
{
    register uint64_t Val;
    asm volatile ("ldarx   %[rc],0,%[pVar];"
                  : [rc] "=&b" (Val)
                  : [pVar] "b" (pVar));
    return( Val );
}

// From hwi/include/bqc/A2_inlines.h
// Store Conditional: 64bit atom
//   Returns: 0 = Conditional Store Failed,
//            1 = Conditional Store Succeeded.
static inline int StoreConditional( volatile uint64_t *pVar, uint64_t Val )
{
    register int rc = 1; // assume success
    asm volatile ("  stdcx.  %2,0,%1;"
                  "  beq     1f;"       // conditional store succeeded
                  "  li      %0,0;"
                  "1:;"
                  : "=b" (rc)
                  : "b"  (pVar),
                  "b"  (Val),
                  "0"  (rc)
                  : "cc", "memory" );
    return( rc );
}

const char* spc_cmd_string[] = {
    "NONE",
    "START",
    "RESUME",
    "LOAD_TLB",
    "UNLOAD_TLB",
    "SPC_EXIT",
    "SAVE_FPU",
    "UPC_INIT",
    "UPCP_INIT"
};

static long fusedosfs_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    spc_context_t* context;
    char* mem_bot;
    char buf[1024];
    struct ppc64_opd_entry* spcm_func;
    int threadId, rc;
    //long remaining_timeout = 0;

    context = (spc_context_t*)(filp->private_data);
    switch (cmd) {
    case SPC_IOCTL_TEST:
        mem_bot = (char*)(__va(context->mem_bot));
        strncpy(buf, mem_bot, strlen("Hello world"));
        buf[strlen("Hello world")] = 0;
        printk("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_TEST, buf %s, arg %lu\n", buf, arg);
        printk("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_TEST, before IPI\n");
        do_message_pass(4, PPC_MSG_CALL_FUNCTION);
        printk("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_TEST, after IPI\n");
        return(0);
    case SPC_IOCTL_INIT:
        spcm_func = &(context->spcm_func);
        spcm_func->r2 = context->regs.gpr[2];
        threadId = cpu_thread_in_core(SPC_TO_CPU(context->id));
        FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_INIT, starting cpu %lld as spc %lld, threadId %d\n",
                    SPC_TO_CPU(context->id), context->id, threadId);
        // pm starting on thread 0 of each core removes the firmware 1 GB TLB entry
        // that provides a one-to-one virtual to physical mapping and replaces it
        // with a 16 MB entry that maps virtual address 0 to physical address
        // spc_monitor. We need to swizzle accordingly.
        if (threadId == 0) {
            spcm_func->funcaddr = context->regs.ip + (unsigned long)spc_monitor;
            rc = _fw->takeCPU(SPC_TO_CPU(context->id) / threads_per_core, 1 << threadId,
                              (void (*)(void *))(__pa(spcm_func)),
                              (void*)(__pa((void*)context)));
        } else {
            spcm_func->funcaddr = context->regs.ip;
            rc = _fw->takeCPU(SPC_TO_CPU(context->id) / threads_per_core, 1 << threadId,
                              (void (*)(void *))(__pa(spcm_func) - (unsigned long)spc_monitor),
                              (void*)(__pa((void*)context) - (unsigned long)spc_monitor));
        }
        FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_INIT, funcaddr 0x%lx, r2 0x%lx\n", spcm_func->funcaddr, spcm_func->r2);
        FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_INIT, takeCPU returned 0x%d\n", rc);
        if (rc == 0) {
            printk("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_INIT, started cpu %lld as spc %lld, threadId %d\n",
                   SPC_TO_CPU(context->id), context->id, threadId);
        } else {
            printk("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_INIT, ERROR failed to start cpu %lld as spc %lld, threadId %d\n",
                   SPC_TO_CPU(context->id), context->id, threadId);
        }
        return(0);
    case SPC_IOCTL_COMMAND:
        FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_COMMAND, spc %lld, command %s\n",
                    context->id, spc_cmd_string[context->command]);
        context->start = 1;
        // If we're not exiting then wait for the SPC to send an IPI (see
        // spc_ipi())
        if (context->command != SPC_EXIT) {
#if defined USE_WAITRSV
            while (LoadReserved(&(context->ipi_wakeup)) == 0) {
                asm volatile ("waitrsv");
            }
#else // USE_WAITRSV
            // Wait for SPC to send IPI, see spc_ipi()
#if 0
            while (!(context->ipi_wakeup == 1)) {
                remaining_timeout = wait_event_interruptible_timeout(cl_wait[context->id], (context->ipi_wakeup == 1), HZ);
                if (remaining_timeout == -ERESTARTSYS)
                    break;
                if (remaining_timeout == 0 && (context->ipi_wakeup == 1))
                    printk(KERN_ERR "%s: Timeout expired and wakeup condition was fulfilled.\n", __func__);
            }
#else
            wait_event_interruptible(cl_wait[context->id], (context->ipi_wakeup == 1));
#endif
#endif // USE_WAITRSV
            context->ipi_wakeup = 0;
            FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_COMMAND, after wait, spc %lld, gpr[3] 0x%llx\n",
                        context->id, context->regs.gpr[3]);
        }
        return(0);
    case SPC_IOCTL_IPI_QUIT:
        FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: SPC_IOCTL_IPI_QUIT, targ cpu %lu, spc %lu\n", arg, CPU_TO_SPC(arg));
        if (arg < fusedos_config->nr_gpcs) {
            FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_ioctl: ERROR, %lu is a GPC\n", arg);
            return(-1);
        }
        spc_context[CPU_TO_SPC(arg)].ipi_message.fcn = SPC_IPI_QUIT;
        spc_context[CPU_TO_SPC(arg)].ipi_message.parm1 = 0;
        spc_context[CPU_TO_SPC(arg)].ipi_message.parm2 = 0;
        do_message_pass(arg, PPC_MSG_CALL_FUNCTION); // PPC_MSG_CALL_FUNCTION not really used
        return(0);
    default:
        return(-ENOTTY);
    }
}

static int fusedosfs_text_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    spc_context_t* pc = (spc_context_t*)(filp->private_data);
    unsigned long length;
    unsigned long text_length = pc->text_pend - pc->text_pstart + 1; // Round up
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_text_mmap_file: id %llu, pstart 0x%llx, pend 0x%llx, text_length 0x%lx, length 0x%lx\n",
                pc->id, pc->text_pstart, pc->text_pend, text_length, length);
    if (length != text_length) {
        printk("FUSEDOS fusedosfs_text_mmap_file: ERROR, id %llu, length 0x%lx, text_length 0x%lx\n", pc->id, length, text_length);
        return -EIO;
    }
    pfn = virt_to_phys((void *)(pc->text_pstart)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

static int fusedosfs_data_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    spc_context_t* pc = (spc_context_t*)(filp->private_data);
    unsigned long length;
    unsigned long data_length = pc->data_pend - pc->data_pstart + 1; // Round up
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_data_mmap_file: id %llu, pstart 0x%llx, pend 0x%llx, data_length 0x%lx, length 0x%lx\n",
                pc->id, pc->data_pstart, pc->data_pend, data_length, length);
    if (length != data_length) {
        printk("FUSEDOS fusedosfs_data_mmap_file: ERROR, id %llu, length 0x%lx, data_length 0x%lx\n", pc->id, length, data_length);
        return -EIO;
    }
    pfn = virt_to_phys((void *)(pc->data_pstart)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}


static int fusedosfs_heap_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    spc_context_t* pc = (spc_context_t*)(filp->private_data);
    unsigned long length;
    unsigned long heap_length = pc->heap_pend - pc->heap_pstart + 1; // Round up
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_heap_mmap_file: id %llu, pstart 0x%llx, pend 0x%llx, heap_length 0x%lx, length 0x%lx\n",
                pc->id, pc->heap_pstart, pc->heap_pend, heap_length, length);
    if (length != heap_length) {
        printk("FUSEDOS fusedosfs_heap_mmap_file: ERROR, id %llu, length 0x%lx, heap_length 0x%lx\n", pc->id, length, heap_length);
        return -EIO;
    }
    pfn = virt_to_phys((void *)(pc->heap_pstart)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

static int fusedosfs_generic_open(struct inode *inode, struct file *filp)
{
    int spc;
    sscanf(filp->f_dentry->d_name.name, "%d_", &spc);
    filp->private_data = &spc_context[spc];
    return 0;
}


static int fusedosfs_shared_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    spc_context_t* pc = (spc_context_t*)(filp->private_data);
    unsigned long length;
    unsigned long shared_length = pc->shared_pend - pc->shared_pstart + 1; // Round up
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    FUSEDOSFS_DEBUG("FUSEDOS fusedosfs_shared_mmap_file: id %llu, pstart 0x%llx, pend 0x%llx, shared_length 0x%lx, length 0x%lx\n",
                pc->id, pc->shared_pstart, pc->shared_pend, shared_length, length);
    if (length != shared_length) {
        printk("FUSEDOS fusedosfs_shared_mmap_file: ERROR, id %llu, length 0x%lx, shared_length 0x%lx\n", pc->id, length, shared_length);
        return -EIO;
    }
    pfn = virt_to_phys((void *)(pc->shared_pstart)) >> PAGE_SHIFT;
    ret = remap_pfn_range(vma, vma->vm_start, pfn, length, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

ssize_t fusedosfs_pvf_read(struct file *filp, char __user * buf, size_t len, loff_t * offset) {

	uint64_t processor_version = ProcessorVersion();

	if(len < sizeof(processor_version))
		return -EINVAL;

	if( copy_to_user( buf, &processor_version, sizeof(processor_version) ) )
		return -EINVAL;

	return sizeof(processor_version);
}

static struct file_operations fusedosfs_monitor_file_ops = {
    .open = fusedosfs_monitor_open,
    .mmap = fusedosfs_monitor_mmap_file,
};

static struct file_operations fusedosfs_context_file_ops = {
    .open = fusedosfs_context_open,
    .mmap = fusedosfs_context_mmap_file,
};

static struct file_operations fusedosfs_memory_file_ops = {
    .open = fusedosfs_generic_open,
    .mmap = fusedosfs_memory_mmap_file,
    .unlocked_ioctl = fusedosfs_ioctl,
};

static struct file_operations fusedosfs_text_file_ops = {
    .open = fusedosfs_generic_open,
    .mmap = fusedosfs_text_mmap_file,
};

static struct file_operations fusedosfs_data_file_ops = {
    .open = fusedosfs_generic_open,
    .mmap = fusedosfs_data_mmap_file,
};

static struct file_operations fusedosfs_heap_file_ops = {
    .open = fusedosfs_generic_open,
    .mmap = fusedosfs_heap_mmap_file,
};

static struct file_operations fusedosfs_shared_file_ops = {
    .open = fusedosfs_generic_open,
    .mmap = fusedosfs_shared_mmap_file,
};

static struct file_operations fusedosfs_pvr_file_ops = {
	.read = fusedosfs_pvf_read,
};

// struct tree_descr array is generated by gen_fusedos_files.sh and included
// here
#include "fusedos_files.h"

static int fusedosfs_fill_super (struct super_block *sb, void *data, int silent)
{
    int ret = simple_fill_super(sb, FUSEDOSFS_MAGIC, fusedos_files);
	if(!ret)
		dget_dlock(sb->s_root); // avoid dentry counter reaching -1 on unmount
	return ret;
}

static int fusedosfs_set_super(struct super_block *s, void *data)
{
    s->s_fs_info = data;
    return set_anon_super(s, data);
}

static struct dentry *fusedosfs_mount(struct file_system_type *fst, int flags, const char *devname, void *data) {
    int err;
    struct super_block *sb = sget(fst, NULL, fusedosfs_set_super, data);
    err = fusedosfs_fill_super(sb, NULL, 0);
    if(err < 0)
        return ERR_PTR(err);

    return sb->s_root;
}

static struct file_system_type fusedosfs_type = {
    .owner = THIS_MODULE,
    .name = "fusedosfs",
    .mount = fusedosfs_mount,
    .kill_sb = kill_litter_super,
};

static int __init fusedosfs_init(void)
{
    int i;

    spc_ipi_fp = spc_ipi_ko;

    for(i = 0; i < fusedos_config->nr_spcs; i++)
        init_waitqueue_head(&(cl_wait[i]));

    return register_filesystem(&fusedosfs_type);
}

static void __exit fusedosfs_exit(void)
{
    unregister_filesystem(&fusedosfs_type);
}

module_init(fusedosfs_init);
module_exit(fusedosfs_exit);
