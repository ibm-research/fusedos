#include <linux/scatterlist.h>
#include <linux/gfp.h>
#include <rdma/ib_verbs.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/version.h>


/*
 * DMA mapping/address translation functions.
 * Used to populate roq private DMA mapping functions of
 * struct ib_dma_mapping_ops in struct ib_dev - see rdma/ib_verbs.h
 */

static int roq_mapping_error(struct ib_device *dev, u64 dma_addr)
{
	return dma_addr == 0;
}

static u64 roq_dma_map_single(struct ib_device *dev, void *kva, size_t size,
			       enum dma_data_direction dir)
{
	/* works only for kmalloc'ed memory */
	return (u64)virt_to_phys(kva);
}

static void roq_dma_unmap_single(struct ib_device *dev,
				 u64 addr, size_t size,
				 enum dma_data_direction dir)
{
	/* NOP */
}

static u64 roq_dma_map_page(struct ib_device *dev, struct page *page,
			    unsigned long offset, size_t size,
			    enum dma_data_direction dir)
{
	u64 addr = 0;

	BUG_ON(!valid_dma_direction(dir));

	/* tentative hotfix. allows object to span 2 pages */
	if (offset + size <= (PAGE_SIZE * 2)) {
	//if (offset + size <= PAGE_SIZE) {
		addr = page_to_phys(page);
		if (addr)
			addr += offset;
	}
	if (unlikely(!addr))
		pr_info("roq_dma_map_page failed: %lu %lu", size, offset);
	return addr;
}

static void roq_dma_unmap_page(struct ib_device *dev,
			       u64 addr, size_t size,
			       enum dma_data_direction dir)
{
	/* NOP */
}

static int roq_dma_map_sg(struct ib_device *dev, struct scatterlist *sgl,
			  int n_sge, enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	BUG_ON(!valid_dma_direction(dir));

	for_each_sg(sgl, sg, n_sge, i) {
		/* This is just a validity check */
		if (unlikely(page_address(sg_page(sg)) == NULL)) {
			pr_info("roq_dma_map_sg: NULL page address");
			return 0;
		}
		sg->dma_address = page_to_phys(sg_page(sg));
		sg_dma_len(sg) = sg->length;
	}
	return n_sge;
}

static void roq_dma_unmap_sg(struct ib_device *dev, struct scatterlist *sgl,
			     int n_sge, enum dma_data_direction dir)
{
	/* NOP */
}

static u64 roq_dma_address(struct ib_device *dev, struct scatterlist *sg)
{
	struct page *p = sg_page(sg);

	if (p)
		return page_to_phys(p) + sg->offset;

	return 0;
}

static unsigned int roq_dma_len(struct ib_device *dev,
				   struct scatterlist *sg)
{
	return sg_dma_len(sg);
}

static void roq_sync_single_for_cpu(struct ib_device *dev, u64 addr,
				    size_t size, enum dma_data_direction dir)
{
	/* NOP */
}

static void roq_sync_single_for_device(struct ib_device *dev, u64 addr,
				       size_t size,
				       enum dma_data_direction dir)
{
	/* NOP */
}

static void *roq_dma_alloc_coherent(struct ib_device *dev, size_t size,
				    u64 *dma_addr, gfp_t flag)
{
	struct page *page;
	void *kva = NULL;

	page = alloc_pages(flag, get_order(size));
	if (page)
		kva = page_address(page);
	if (dma_addr)
		*dma_addr = page_to_phys(page);

	return kva;
}

static void roq_dma_free_coherent(struct ib_device *dev, size_t size,
				  void *kva, u64 dma_addr)
{
	free_pages((unsigned long) kva, get_order(size));
}

struct ib_dma_mapping_ops roq_dma_mapping_ops = {
	.mapping_error		= roq_mapping_error,
	.map_single		= roq_dma_map_single,
	.unmap_single		= roq_dma_unmap_single,
	.map_page		= roq_dma_map_page,
	.unmap_page		= roq_dma_unmap_page,
	.map_sg			= roq_dma_map_sg,
	.unmap_sg		= roq_dma_unmap_sg,
	.dma_address		= roq_dma_address,
	.dma_len		= roq_dma_len,
	.sync_single_for_cpu	= roq_sync_single_for_cpu,
	.sync_single_for_device	= roq_sync_single_for_device,
	.alloc_coherent		= roq_dma_alloc_coherent,
	.free_coherent		= roq_dma_free_coherent
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
static void *roq_dma_generic_alloc_coherent(struct device *dev, size_t size,
					    dma_addr_t *dma_handle, gfp_t gfp)
{
#else
static void *roq_dma_generic_alloc(struct device *dev, size_t size,
					    dma_addr_t *dma_handle, gfp_t gfp,
					    struct dma_attrs * attrs)
{
	BUG_ON(attrs != NULL); // do not support attributes now (cheap port from 2.6)
#endif
	return roq_dma_alloc_coherent(NULL, size, dma_handle, gfp);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
static void roq_dma_generic_free_coherent(struct device *dev, size_t size,
					  void *vaddr, dma_addr_t dma_handle)
{
#else
static void roq_dma_generic_free(struct device *dev, size_t size,
					  void *vaddr, dma_addr_t dma_handle,
					  struct dma_attrs * attrs)
{
	BUG_ON(attrs != NULL); // do not support attributes now (cheap port from 2.6)
#endif
	roq_dma_free_coherent(NULL, size, vaddr, dma_handle);
}

static dma_addr_t roq_dma_generic_map_page(struct device *dev,
					   struct page *page,
					   unsigned long offset,
					   size_t size,
					   enum dma_data_direction dir,
					   struct dma_attrs *attrs)
{
	return roq_dma_map_page(NULL, page, offset, size, dir);
}

static void roq_dma_generic_unmap_page(struct device *dev,
				       dma_addr_t handle,
				       size_t size,
				       enum dma_data_direction dir,
				       struct dma_attrs *attrs)
{
	roq_dma_unmap_page(NULL, handle, size, dir);
}

static int roq_dma_generic_map_sg(struct device *dev, struct scatterlist *sg,
				  int nents, enum dma_data_direction dir,
				  struct dma_attrs *attrs)
{
	return roq_dma_map_sg(NULL, sg, nents, dir);
}

static void roq_dma_generic_unmap_sg(struct device *dev,
				    struct scatterlist *sg,
				    int nents,
				    enum dma_data_direction dir,
				    struct dma_attrs *attrs)
{
	roq_dma_unmap_sg(NULL, sg, nents, dir);
}

static void roq_generic_sync_single_for_cpu(struct device *dev,
					    dma_addr_t dma_handle,
					    size_t size,
					    enum dma_data_direction dir)
{
	roq_sync_single_for_cpu(NULL, dma_handle, size, dir);
}


static void roq_generic_sync_single_for_device(struct device *dev,
					       dma_addr_t dma_handle,
					       size_t size,
					       enum dma_data_direction dir)
{
	roq_sync_single_for_device(NULL, dma_handle, size, dir);
}

static void roq_generic_sync_sg_for_cpu(struct device *dev,
					struct scatterlist *sg,
					int nents,
					enum dma_data_direction dir)
{
	/* NOP */
}

static void roq_generic_sync_sg_for_device(struct device *dev,
					   struct scatterlist *sg,
					   int nents,
					   enum dma_data_direction dir)
{
	/* NOP */
}

static int roq_dma_generic_mapping_error(struct device *dev,
					 dma_addr_t dma_addr)
{
	return roq_mapping_error(NULL, dma_addr);
}

static int roq_dma_generic_supported(struct device *dev, u64 mask)
{
	return 1;
}

static int roq_dma_generic_set_mask(struct device *dev, u64 mask)
{
	pr_info("roq_dma_generic_set_mask");
	if (!dev->dma_mask || !dma_supported(dev, mask))
		return -EIO;

	*dev->dma_mask = mask;

	return 0;
}

struct dma_map_ops roq_dma_generic_ops = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
	.alloc_coherent		= roq_dma_generic_alloc_coherent,
	.free_coherent		= roq_dma_generic_free_coherent,
#else
	.alloc			= roq_dma_generic_alloc,
	.free			= roq_dma_generic_free,
#endif
	.map_page		= roq_dma_generic_map_page,
	.unmap_page		= roq_dma_generic_unmap_page,
	.map_sg			= roq_dma_generic_map_sg,
	.unmap_sg		= roq_dma_generic_unmap_sg,
	.sync_single_for_cpu	= roq_generic_sync_single_for_cpu,
	.sync_single_for_device	= roq_generic_sync_single_for_device,
	.sync_sg_for_cpu	= roq_generic_sync_sg_for_cpu,
	.sync_sg_for_device	= roq_generic_sync_sg_for_device,
	.mapping_error		= roq_dma_generic_mapping_error,
	.dma_supported		= roq_dma_generic_supported,
	.set_dma_mask		= roq_dma_generic_set_mask,
	.is_phys		= 1
};

static void roq_device_release(struct device *dev)
{
	pr_info("%s device released\n", dev_name(dev));
}

struct device roq_generic_device = {
	.archdata.dma_ops	= &roq_dma_generic_ops,
	.init_name		= "RDMAonQ",
	.release		= roq_device_release
};

/*
 * Allocate physically contiguous memory area and provide its physical
 * address. The resulting memory area tries to minimize memory system
 * footprint (not always a page gets consumed) but is not mappeable
 * to user land.
 */
void *
roq_kmalloc(int size, dma_addr_t *dma_addr)
{
	void		*vaddr;

	if (unlikely(size <= 0)) {
		pr_warn("roq_kmalloc: size: %d\n", size);
		return NULL;
	}
	vaddr = kmalloc(size, GFP_KERNEL);
	if (!vaddr) {
		pr_warn("roq_kmalloc: kmalloc: %d\n", size);
		return NULL;
	}
	if (dma_addr) {
		u64 offset = (unsigned long)vaddr & ~PAGE_MASK;
		*dma_addr = page_to_phys(virt_to_page(vaddr)) + offset;
	}
	return vaddr;
}

void
roq_kfree(void *addr)
{
	kfree(addr);
}

