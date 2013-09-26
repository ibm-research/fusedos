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

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/eventfd.h>

#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/BIC.h>
#include <firmware/include/personality.h>

#include <spi/include/kernel/klinux/kernel_impl.h>
#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/kernel/memory.h>
#include <spi/include/kernel/MU.h>

#define DRV_VERSION "0.1"
#define DRV_DESC  "IBM Blue Gene Userspace Memory Unit mapping"
#define DRV_NAME  "roq_usmu"

#define BGQ_TORUS_MAJOR_NUM 121

#include <rdma/ib_verbs.h>
#include <roq_device_interface.h>
#include <roq.h>
#include <transcon.h>
#include <ionlink.h>

#include <bgroq.h>

MODULE_DESCRIPTION(DRV_DESC);
MODULE_AUTHOR("IBM");
MODULE_LICENSE("GPL");

struct roq_usmu_t {
	int                  major;              /* device major */
	struct task_struct*  current_task;       /* process holding device */
	int                  signum;             /* signal to send holding process */
	struct cdev          cdev;               /* container device? */
	void                *devmem;
	struct eventfd_ctx  *eventfd_ctx;
};

struct roq_dma_init_t {
	uint32_t nports;
	uint32_t port_ids[2]; // FIXME: should be the size of nports!!!
	uint64_t ret;
};

struct roq_dma_usermem_t {
	uint64_t base_va;
	size_t size;
};

int bgroq_activate(struct bgroq_dev *srdev);
int bgroq_deactivate(struct bgroq_dev *srdev);
irqreturn_t bgproq_interrupt(int irq, void *dev);
extern struct bgroq_dev *bgroq_device;

struct roq_usmu_t *roq_usmu;
struct roq_us_map_t *roq_us_map;

#if 0
static uint64_t
malloc_memalign(void **memory, void **buffer, uint64_t alignment, uint32_t size)
{
  /* Malloc the memory to be the size of the buffer plus space for alignment. */
  void *myMemory = kmalloc(size + alignment, GFP_DMA);
  if ( !myMemory ) return -1;

  *memory = myMemory;
  *buffer = myMemory;

  return (((uint64_t)virt_to_phys((void*)myMemory) + alignment) & ~(alignment-1));
}
#endif

#if 0
static irqreturn_t roq_mu_irq(int virq, void *dev)
{
	//struct roq_usmu_t *vum = (struct roq_usmu_t *)dev;
	*(uint64_t*)BGQ_MU_CLEAR_INT_STATUS_REGISTER_OFFSET(0,0) = 0xffffffffffffffffULL;
	((BIC_Device*)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[0] |= 0x0001540000000000ULL;
	printk("INTERRUPT\n");
	return IRQ_HANDLED;
}
#endif

int
ROQ_dma_init(uint32_t* port_ids, uint32_t nports)
{
	int rc, i = 0;
	uint64_t recFifoEnableBits;
	//cpumask_t cpus_allowed = current->cpus_allowed;
	dma_addr_t tmp;
	//volatile BIC_Device* _puea;

	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup;
	Kernel_RecFifoAttributes_t *recFifoAttrs;
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup;
	Kernel_InjFifoAttributes_t *injFifoAttrs;

	/* so far only 2 nports possible */
	BUG_ON(nports > 2);

	roq_us_map = (struct roq_us_map_t *)kmalloc(sizeof(struct roq_us_map_t), GFP_DMA);
	if (roq_us_map == NULL) {
		printk("FAILED vmalloc\n");
		return -1;
	}
	roq_us_map->magic = 0x2355aa42;
	roq_us_map->nports = nports;

	/* allocate reception fifo */
	RecFifoSubgroup = &roq_us_map->RecFifoSubgroups[0];

	recFifoAttrs = (Kernel_RecFifoAttributes_t*)
	               kmalloc(nports*sizeof(Kernel_RecFifoAttributes_t), GFP_KERNEL);
	for (i=0; i < nports; i++) {
		recFifoAttrs[i].System = 0;
	}
	rc = Kernel_AllocateRecFifos(0, RecFifoSubgroup, nports, &port_ids[0], recFifoAttrs);
	kfree(recFifoAttrs);

	recFifoEnableBits = 0;
	for (i=0; i < nports; i++)
		recFifoEnableBits |= (0x0000000000000001ULL << (15 - port_ids[i]));
	rc = Kernel_RecFifoEnable(0, recFifoEnableBits);

	/* allocate injection fifo */
	InjFifoSubgroup = &roq_us_map->InjFifoSubgroups[0];
	injFifoAttrs = (Kernel_InjFifoAttributes_t*)
	               kmalloc(nports*sizeof(Kernel_InjFifoAttributes_t), GFP_KERNEL);

	for (i=0; i < nports; i++) {
		injFifoAttrs[i].RemoteGet = 0;
		injFifoAttrs[i].System    = 0;
	}
	rc = Kernel_AllocateInjFifos(0, InjFifoSubgroup, nports, port_ids, injFifoAttrs);
	kfree(injFifoAttrs);

	DCRWritePriv(MU_DCR(MIN_USR_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_USR_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */
	DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE), 0UL);      /* minimum physical system address is 0         */
	DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE), ~0UL);     /* maximum physical system address is unlimited */

	roq_us_map->virtbuf = (uint64_t*)kmalloc(512, GFP_KERNEL);
	roq_us_map->dmabuf = (uint64_t*)((uint64_t)virt_to_phys((void*)roq_us_map->virtbuf) | ROQ_VIRT_CACHE_OFF);

	roq_us_map->trans_idx = 0;
	for (i=0; i < MAX_NODE_COUNT; i++) {
#if 1
		roq_us_map->mu_ackdesc[i] = dma_alloc_coherent(bgroq_device->ofa_dev.dma_device,
		                                                  sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT,
														  &tmp, GFP_KERNEL);
		roq_us_map->mu_ackdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF | tmp);
		roq_us_map->mu_burstdesc[i] = dma_alloc_coherent(bgroq_device->ofa_dev.dma_device,
		                                                  sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT,
														  &tmp, GFP_KERNEL);
		roq_us_map->mu_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF | tmp);
		roq_us_map->tx_burstdesc[i] = dma_alloc_coherent(bgroq_device->ofa_dev.dma_device,
		                                                  sizeof(struct RoQ_TX_BurstDescriptor_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT,
														  &tmp, GFP_KERNEL);
		roq_us_map->tx_burstdesc_dma[i] = (void*)(ROQ_VIRT_CACHE_OFF | tmp);
#else
		roq_us_map->mu_ackdesc[i] = kmalloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_ACKS_IN_FLIGHT, GFP_DMA);
		roq_us_map->mu_ackdesc_dma[i] = (void*)((uint64_t)virt_to_phys((void*)roq_us_map->mu_ackdesc[i]) | ROQ_VIRT_CACHE_OFF);
		roq_us_map->mu_burstdesc[i] = kmalloc(sizeof(MUHWI_Descriptor_Aligned_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT, GFP_DMA);
		roq_us_map->mu_burstdesc_dma[i] = (void*)((uint64_t)virt_to_phys((void*)roq_us_map->mu_burstdesc[i]) | ROQ_VIRT_CACHE_OFF);
		roq_us_map->tx_burstdesc[i] = kmalloc(sizeof(struct RoQ_TX_BurstDescriptor_t)*RoQ_TRANCONN_MAX_BURSTS_IN_FLIGHT, GFP_DMA);
		roq_us_map->tx_burstdesc_dma[i] = (void*)((uint64_t)virt_to_phys((void*)roq_us_map->tx_burstdesc[i]) | ROQ_VIRT_CACHE_OFF);
#endif
	}

	for (i=0; i < nports; i++) {
#if 1
		roq_us_map->inj_memory_fifos[i] = dma_alloc_coherent(bgroq_device->ofa_dev.dma_device,
		                                                  INJ_MEMORY_FIFO_SIZE+64,
														  &tmp, GFP_KERNEL);
		roq_us_map->inj_dmabuf[i] = (void*)(((tmp) + 64) & ~(64-1));
		roq_us_map->rec_memory_fifos[i] = (uint8_t*)dma_alloc_coherent(bgroq_device->ofa_dev.dma_device,
		                                                  REC_MEMORY_FIFO_SIZE+32,
														  &tmp, GFP_KERNEL);
		roq_us_map->rec_dmabuf[i] = (void*)(((tmp) + 32) & ~(32-1));
#else
		roq_us_map->inj_dmabuf[i] = (uint8_t*)malloc_memalign(&roq_us_map->alloc_inj_bufs[i],
		                &roq_us_map->inj_memory_fifos[i], 64, INJ_MEMORY_FIFO_SIZE);
		roq_us_map->rec_dmabuf[i] = (uint8_t*)malloc_memalign(&roq_us_map->alloc_rec_bufs[i],
		                &roq_us_map->rec_memory_fifos[i], 32, REC_MEMORY_FIFO_SIZE);
#endif
	}

	/* generate machine check interrupts for MU errors. */
	DCRWritePriv(MU_DCR(INTERRUPT_INTERNAL_ERROR_CONTROL_HIGH), 0x1111111111111111ULL);
	//DCRWritePriv(MU_DCR(INTERRUPT_INTERNAL_ERROR_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IME_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IME_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT0_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT0_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT1_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT1_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT2_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MASTER_PORT2_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(SLAVE_PORT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(SLAVE_PORT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MMREGS_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MMREGS_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(FIFO_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IMU_ECC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(IMU_ECC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MCSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ARLOG_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ARLOG_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RCSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RCSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RPUTSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RPUTSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS0_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS0_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS1_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RME_INTERRUPTS1_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ICSRAM_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ICSRAM_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_CORR_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(RMU_ECC_CORR_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_ECC_CORR_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(MISC_ECC_CORR_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ECC_COUNT_INTERRUPTS_CONTROL_LOW), 0x1111111111111111ULL);
	DCRWritePriv(MU_DCR(ECC_COUNT_INTERRUPTS_CONTROL_HIGH), 0x1111111111111111ULL);

#if 0
	/* make sure, this runs on core 0!!! */
	set_cpus_allowed_ptr(current, cpumask_of(0));
	/* Interrupt stuff */
	/* FIXME: we have to decide which interrupts should be enabled! */
	DCRWritePriv(MU_DCR(RMFIFO_PACKET_INT_EN), 0x8000); /* Enable interrupt on packet arrival in FIFO   */
	DCRWritePriv(MU_DCR(IMFIFO_THOLD_INT_EN), 0x8000);
	DCRWritePriv(MU_DCR(RMFIFO_THOLD_INT_EN), 0x8000);

	DCRWritePriv(ND_500_DCR(NON_FATAL_ERR_ENABLE)+0, ~0UL);
	DCRWritePriv(ND_500_DCR(NON_FATAL_ERR_ENABLE)+1, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+0, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+1, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+2, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+3, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+4, ~0UL);
	DCRWritePriv(ND_500_DCR(FATAL_ERR_ENABLE)+5, ~0UL);
	DCRWritePriv(ND_500_DCR(N500_INTERRUPTS_CONTROL_LOW), 0x1);
	DCRWritePriv(ND_500_DCR(N500_INTERRUPTS_CONTROL_HIGH), 0x1111000000000000ULL);

	rc = request_irq(17, roq_mu_irq, IRQF_DISABLED | IRQF_PERCPU, "MU[0]", roq_usmu);
	BUG_ON(rc);
	/* map MU interrupts as external interrupts */
	((BIC_Device*)BIC_DEVICE_BASE_ADDRESS)->_map_interrupt[0] |= 0x0001540000000000ULL;
	set_cpus_allowed_ptr(current, &cpus_allowed);
#endif

	return 0;
}

static long roq_usmu_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	struct roq_ion_send_t vis;
	void *tmp;
	switch (cmd) {
	case 1:
		tmp = kmalloc(sizeof(struct roq_dma_init_t), GFP_KERNEL);
		copy_from_user(tmp, (void*)arg, sizeof(struct roq_dma_init_t));
		ROQ_dma_init(((struct roq_dma_init_t*)tmp)->port_ids,
		               ((struct roq_dma_init_t*)tmp)->nports);
		((struct roq_dma_init_t*)tmp)->ret = (uint64_t)virt_to_phys(roq_us_map); //((uint64_t)roq_us_map & 0xffffffffffff) | ROQ_VIRT_NOCACHE_OFF;
		copy_to_user((void*)arg, tmp, sizeof(struct roq_dma_init_t));
		kfree(tmp);
		return 0;
	case 2:
		if (arg == 0) {
			bgroq_activate(bgroq_device);
		} else {
			bgroq_deactivate(bgroq_device);
		}
		return 0;
	case 4:
		roq_usmu->eventfd_ctx = eventfd_ctx_fdget(arg);
		return 0;
	case 10:
		roq_ion_init();
		break;
	case 11:
		copy_from_user(&vis, (void*)arg, sizeof(struct roq_ion_send_t));
		roq_ion_send(&vis);
		break;
	case 0x42:
		bgproq_interrupt(0x42, NULL);
		break;
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

static int roq_usmu_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long vsize = vma->vm_end - vma->vm_start;
	uint64_t ptr;

	if (vma->vm_start == 0x3fdfe000000ULL) {
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		vma->vm_flags     |= VM_IO;
		vma->vm_flags     |= VM_RESERVED;
		ptr = (uint64_t)vma->vm_pgoff;
	} else if (vma->vm_start == 0x23000000) {
		ptr = virt_to_phys((void*)roq_usmu->devmem) >> PAGE_SHIFT;
	} else {
		ptr = virt_to_phys((void*)roq_us_map) >> PAGE_SHIFT;
	}
	//printk(DRV_NAME ": MMAP AREA %016llx %lx\n", ptr, vma->vm_start);
	if (remap_pfn_range(vma, vma->vm_start, ptr, vsize, vma->vm_page_prot) < 0) {
		printk("remap_pfn_range failed\n");
		return -EIO;
	}

	return 0;
}

static int roq_usmu_open(struct inode *inode, struct file *filp)
{
	struct roq_usmu_t *dev = container_of(inode->i_cdev, struct roq_usmu_t, cdev);

	dev->current_task = current;
	filp->private_data = (void*) dev;

#if 0
	printk(DRV_NAME": device (%d,%d) opened by process \"%s\" pid %i",
	       MAJOR(inode->i_rdev), MINOR(inode->i_rdev), current->comm, current->pid);
#endif

	return 0;
}

static int roq_usmu_release(struct inode *inode, struct file * filp)
{
#if 0
	struct roq_usmu_t *dev=(struct roq_usmu_t*)filp->private_data;
	dev->current_task = NULL;

	printk(DRV_NAME": device (%d,%d) successfully released",
	       MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
	bgroq_deactivate(bgroq_device);
#endif
	return 0;
}

static struct file_operations roq_usmu_fops =
{
	.owner          = THIS_MODULE,
	.open           = roq_usmu_open,
	.write          = NULL,
	.unlocked_ioctl = roq_usmu_ioctl,
	.release        = roq_usmu_release,
	.mmap           = roq_usmu_mmap,
};

uint64_t __init
roq_usmu_module_init(void)
{
	int ret;

	roq_usmu = kmalloc(sizeof(struct roq_usmu_t), GFP_KERNEL);
	roq_usmu->devmem = kmalloc(sizeof(struct RoQ_DeviceMemory_t), GFP_KERNEL);

	/* initilize struct */
	roq_usmu->major  = BGQ_TORUS_MAJOR_NUM;

	/* register i.e., /dev/device */
	ret = register_chrdev(BGQ_TORUS_MAJOR_NUM, (char*)DRV_NAME, &roq_usmu_fops);

	if (ret) {
		printk(KERN_WARNING DRV_NAME ": couldn't register device (%d,%d)"
		                             " register_chrdev err=%d\n",
		       roq_usmu->major, 0, ret);
		return ret;
	}

	/* signal to pass to owning process, should be altered using ioctl */
	roq_usmu->signum=-1;

    return (uint64_t) roq_usmu->devmem;
}

void __exit
roq_usmu_module_exit(void)
{
	int i;
	unregister_chrdev(roq_usmu->major, DRV_NAME);
	if (roq_us_map != NULL) {
		for (i=0; i < MAX_NODE_COUNT; i++) {
			kfree(roq_us_map->mu_ackdesc[i]);
			kfree(roq_us_map->mu_burstdesc[i]);
			kfree(roq_us_map->tx_burstdesc[i]);
		}
		for (i=0; i < roq_us_map->nports; i++) {
			kfree(roq_us_map->alloc_rec_bufs[i]);
			kfree(roq_us_map->alloc_inj_bufs[i]);
		}
		kfree(roq_us_map->virtbuf);
		kfree(roq_us_map);
		roq_us_map = NULL;
	}

	if (roq_usmu != NULL) {
		kfree(roq_usmu->devmem);
		kfree(roq_usmu);
		roq_usmu = NULL;
	}
	return;
}
