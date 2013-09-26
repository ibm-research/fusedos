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

static ssize_t mu_mmap_file(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long length;
    unsigned long pfn;
    int ret;

    length = vma->vm_end - vma->vm_start;
    if (length != PHYMAP_SIZE_MU) {
        printk("FUSEDOS mu_mmap_file: ERROR, length 0x%lx, PHYMAP_SIZE_MU 0x%lx\n", length, PHYMAP_SIZE_MU);
        return -EIO;
    }

    if( ! strncmp("mupriv", filp->f_dentry->d_name.name, strlen("mupriv")) )
        pfn = (PHYMAP_MINADDR_MU + PHYMAP_PRIVILEGEDOFFSET) >> PAGE_SHIFT;
    else // mu
        pfn = PHYMAP_MINADDR_MU >> PAGE_SHIFT;

    vma->vm_flags |= VM_IO;
    vma->vm_flags |= VM_RESERVED;
    vma->vm_flags |= VM_PFN_AT_MMAP; // fully populate mapping

    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

    ret = io_remap_pfn_range(vma, vma->vm_start, pfn, PHYMAP_SIZE_MU, vma->vm_page_prot);
    if (ret < 0) return ret;
    return 0;
}

