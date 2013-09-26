/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
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
/* This software is available to you under the                      */
/* Eclipse Public License (EPL).                                    */
/*                                                                  */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include "Kernel.h"
#if defined __FUSEDOS__                                                                                               // FUSEDOS
#include "fusedos_config.h"                                                                                           // FUSEDOS
#include "fusedos.h"                                                                                                  // FUSEDOS
#endif                                                                                                                // FUSEDOS
#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
#include <spi/include/l2/atomic.h>
#include <hwi/include/bqc/mu_dcr.h>
#include "hwi/include/bqc/BIC_inlines.h"
#include "hwi/include/bqc/gea_dcr.h"
#include "hwi/include/bqc/l2_central_inlines.h"

void IntHandler_Debug( Regs_t *context, int code  );

Firmware_TLBEntries_t tlbdata[17][512];

void vmm_populateTLBTable()
{
    uint64_t core = PhysicalProcessorID();
    uint64_t setway;
    
    for(setway=0; setway<512; setway++)
    {
        mtspr(SPRN_MAS0,
              MAS0_ATSEL_TLB  |  // Select TLB (versus LRAT)
              MAS0_HES_ESEL   |  // Use specified slot rather than LRU replacement
              MAS0_ESEL(setway%4) |
              MAS0_WQ_ALWAYS  ); // Write the entry (no reservation required)
        
        mtspr(SPRN_MAS1, MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB);
        mtspr(SPRN_MAS2, MAS2_EPN((setway/4) << (30-12)));
        isync();
        asm volatile ( "tlbre;" : : : "memory" );
        isync();
        ppc_msync();
        tlbdata[core][setway].mas1   = mfspr(SPRN_MAS1);
        tlbdata[core][setway].mas2   = mfspr(SPRN_MAS2);
        tlbdata[core][setway].mas7_3 = mfspr(SPRN_MAS7_MAS3);
        tlbdata[core][setway].mas8   = mfspr(SPRN_MAS8);
        tlbdata[core][setway].mmucr3 = mfspr(SPRN_MMUCR3);
        
        if((tlbdata[core][setway].mas1 & MAS1_V(-1)) == 0)
        {
            tlbdata[core][setway].mas1   = MAS1_TSIZE_1GB;
            tlbdata[core][setway].mas2   = MAS2_EPN(setway/4 * 1024*256);
            tlbdata[core][setway].mas7_3 = MAS7_3_RPN(setway/4 * 1024*256);
            tlbdata[core][setway].mas8   = MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0);
            tlbdata[core][setway].mmucr3 = MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF);
        }
    }
}

void vmm_tlb_touch_tlbs()
{
    uint64_t setway;
    for(setway=0; setway<512; setway++)
    {
        mtspr(SPRN_MAS0,
              MAS0_ATSEL_TLB  |  // Select TLB (versus LRAT)
              MAS0_HES_ESEL   |  // Use specified slot rather than LRU replacement
              MAS0_ESEL(setway%4) |
              MAS0_WQ_ALWAYS  ); // Write the entry (no reservation required)
        
        mtspr(SPRN_MAS1, MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB);
        mtspr(SPRN_MAS2, MAS2_EPN((setway/4) << (30-12)));
        isync();
        asm volatile ( "tlbre;" : : : "memory" );
        isync();
        ppc_msync();
    }
}

void vmm_tlb_inject_parity(uint64_t setway)
{
    uint64_t core = PhysicalProcessorID();
    uint64_t mmucr1 = mfspr(SPRN_MMUCR1);
    mtspr(SPRN_MMUCR1, mmucr1 | _BN(42));
    isync();
    
    tlbwe_slot(setway%4, tlbdata[core][setway].mas1, tlbdata[core][setway].mas2, tlbdata[core][setway].mas7_3, tlbdata[core][setway].mas8, tlbdata[core][setway].mmucr3);
    isync();
    
    mtspr(SPRN_MMUCR1, mmucr1);
    isync();
}


void vmm_tlbwe_slot(uint64_t way, uint64_t mas1, uint64_t mas2, uint64_t mas7_3, uint64_t mas8, uint64_t mmucr3) 
{
    uint64_t core = PhysicalProcessorID();
    NodeState.FW_Interface.setTLBTable(NULL);
    tlbwe_slot(way, mas1, mas2, mas7_3, mas8, mmucr3);
    vmm_populateTLBTable();
    NodeState.FW_Interface.setTLBTable((Firmware_TLBEntries_t*)&tlbdata[core]);
}

void vmm_tlbilx(void* va, uint64_t size)
{
    uint64_t core = PhysicalProcessorID();
    NodeState.FW_Interface.setTLBTable(NULL);
    tlbilx((void*)va, size);
    vmm_populateTLBTable();
    NodeState.FW_Interface.setTLBTable((Firmware_TLBEntries_t*)&tlbdata[core]);
}

int vmm_TranslateAddress(const void* va, uint64_t* base_va, uint64_t* base_pa, size_t* base_sz)
{
#define INRANGE(start, end, startp) if(( (uint64_t)va>=(uint64_t)(start)) && ((uint64_t)va<=(uint64_t)(end))) {     if(base_va) { *base_va = (uint64_t)(start); }  if(base_pa) { *base_pa = (uint64_t)(startp); } if(base_sz) { *base_sz = (end) - (start) + 1; } return 0; }
   
   INRANGE(&__KERNEL_TEXT_START, &__KERNEL_END, &__KERNEL_TEXT_START);
   
   AppProcess_t* p = GetMyProcess();
   INRANGE(p->Text_VStart,   p->Text_VEnd,      p->Text_PStart);
   INRANGE(p->Data_VStart,   p->Data_VEnd,      p->Data_PStart);
   INRANGE(p->Heap_VStart,   p->Heap_VEnd,      p->Heap_PStart);
   INRANGE(p->Shared_VStart, p->Shared_VEnd,    p->Shared_PStart);
   INRANGE(p->DYN_VStart,    p->DYN_VEnd,       p->DYN_PStart);
#undef INRANGE
   
   return EINVAL;
}
#endif                                                                                                                 // FUSEDOS

int VMM_IsAppAddress(const void* va, size_t size)
{
    return VMM_IsAppAddressForProcess(va, size, GetMyProcess());
}

int VMM_IsAppAddressForProcess(const void* va, size_t size, AppProcess_t* p)
{
// Note that startp is unused by provided for consistency with the above INRANGE macro.
#define INRANGE(start, end, startp) if(((uint64_t)va>=(uint64_t)(start)) && ((uint64_t)va+size<=(uint64_t)(end+1))) { return 1; }
   // Test for NULL. 
   if (va == NULL)
   {
       return 0;
   }
   // Test for a large VA at the end of virtual address space that wraps the dword. For example: va 0xFFFFFFFFFFFFFFFC with size 4
   if ((uint64_t)va > (uint64_t)((uint64_t)va + size))
   {
       return 0;
   }
   if(GetMyKThread()->KernelInternal)
   {
       INRANGE(&__KERNEL_TEXT_START, &__KERNEL_END, &__KERNEL_TEXT_START);
   }
   if (!p) 
   {
       return 0;
   }
   INRANGE(p->Text_VStart,   p->Text_VEnd,      p->Text_PStart);
   INRANGE(p->Data_VStart,   p->Data_VEnd,      p->Data_PStart);
   INRANGE(p->Heap_VStart,   p->Heap_VEnd,      p->Heap_PStart);
   INRANGE(p->Shared_VStart, p->Shared_VEnd,    p->Shared_PStart);
   INRANGE(p->DYN_VStart,    p->DYN_VEnd,       p->DYN_PStart);
#undef INRANGE
   return 0;
}

int VMM_IsAppProcessUniqueAddress(const void* va, size_t size)
{
// Note that startp is unused by provided for consistency with the above INRANGE macro.
#define INRANGE(start, end, startp) if(((uint64_t)va>=(uint64_t)(start)) && ((uint64_t)va+size<=(uint64_t)(end+1))) { return 1; }
   AppProcess_t* p = GetMyProcess();
   if (!p) 
   {
       return 0;
   }
   AppState_t *app = GetMyAppState();
   if(!app)
   {
       return 0;
   }
   if(!app->SharedTextSeg || (app->Active_Processes == 1))
   {
       INRANGE(p->Text_VStart,   p->Text_VEnd,      p->Text_PStart);
   }
   
   INRANGE(p->Data_VStart,   p->Data_VEnd,      p->Data_PStart);
   INRANGE(p->Heap_VStart,   p->Heap_VEnd,      p->Heap_PStart);
   INRANGE(p->DYN_VStart,    p->DYN_VEnd,       p->DYN_PStart);
#undef INRANGE
   return 0;
}

int vmm_MapUserSpace(uint64_t flags, void* physicaladdress, void* virtualaddress, size_t tlbsize, unsigned pid, size_t exclude)
{
#ifndef __FUSEDOS__                                                                                                   // FUSEDOS
    TRACE(TRACE_VMM, ("Mapping TLB: virtual=%p physical=%p  size=%ld exclude=%ld  flags=%lx  pid=%d\n", virtualaddress, physicaladdress, tlbsize, exclude, flags, pid));
#endif                                                                                                                 // FUSEDOS
    uint64_t xbit = 0;
    uint64_t tlbmask = 0;
    if(exclude)
    {
        xbit = 1;
        virtualaddress += exclude-1;
    }
    
    switch(tlbsize)
    {
        case VMM_PAGE_SIZE_1M:   tlbmask = MAS1_TSIZE_1MB;  break;
        case VMM_PAGE_SIZE_16M:  tlbmask = MAS1_TSIZE_16MB; break;
        case VMM_PAGE_SIZE_1G:   tlbmask = MAS1_TSIZE_1GB;  break;
        default: return -1;
    }
    uint64_t mas2 = 0;
    uint64_t mas3 = 0;
    if(flags & APP_FLAGS_R)
        mas3 |= MAS3_UR(1);
    if(flags & APP_FLAGS_W)
        mas3 |= MAS3_UW(1);
    if(flags & APP_FLAGS_X)
        mas3 |= MAS3_UX(1);
    if(flags & APP_FLAGS_NONSPECULATIVE)
        mas3 |= MAS3_U0(1);
    if(flags & APP_FLAGS_FLUSHSTORES)
        mas3 |= MAS3_U1(1);
    if(flags & APP_FLAGS_LISTENABLE)
        mas3 |= MAS3_U3(1);
    if(flags & APP_FLAGS_GUARDED)
        mas2 |= MAS2_G(1);
    if(flags & APP_FLAGS_INHIBITED)
        mas2 |= MAS2_I(1);

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_MAPTLB___, (uint64_t)virtualaddress, (uint64_t)physicaladdress, tlbsize, (flags<<32) | pid);  // exclusion range is encoded

    vmm_tlbwe_slot( 3 - (((flags)>>16) & 3),
          MAS1_V(1) | MAS1_TID(pid) | MAS1_TS(0) | tlbmask,
          MAS2_EPN(((uint64_t)virtualaddress)>>12)   | MAS2_W(0) | MAS2_M(1) | MAS2_E(0) | mas2,
          MAS7_3_RPN(((uint64_t)physicaladdress)>>12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | mas3,
          MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
          MMUCR3_X(xbit) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF)
          );
    isync();
#else                                                                                                                  // FUSEDOS
	spc_context_t* hwt0_pc = get_spc_context(this_spc() / 4 * 4);                                                      // FUSEDOS
    int entry = hwt0_pc->tlb_entry_count++;                                                                            // FUSEDOS
    if (hwt0_pc->tlb_entry_count > MAX_TLB_ENTRIES) {                                                                  // FUSEDOS
        printf("vmm_MapUserSpace: Cannot add TLB entry, tlb_entry_count %lu\n", hwt0_pc->tlb_entry_count);             // FUSEDOS
        return -1;                                                                                                     // FUSEDOS
    }                                                                                                                  // FUSEDOS
    tlb_entry_t *te = &(hwt0_pc->tlb_entry[entry]);                                                                    // FUSEDOS
    te->slot = ( 3 - (((flags)>>16) & 3) );                                                                                    // FUSEDOS
    te->mas1 = MAS1_V(1) | MAS1_TID(pid) | MAS1_TS(0) | tlbmask;                                                       // FUSEDOS
    te->mas2 = MAS2_EPN(((uint64_t)virtualaddress)>>12) | MAS2_W(0) | MAS2_M(1) | MAS2_E(0) | mas2;                    // FUSEDOS
    te->mas7_3 = MAS7_3_RPN(((uint64_t)physicaladdress)>>12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | mas3;            // FUSEDOS
    te->mas8 = MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0);                                                               // FUSEDOS
    te->mmucr3 = MMUCR3_X(xbit) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF);     // FUSEDOS
   // TRACE(TRACE_VMM, ("FUSEDOS vmm_MapUserSpace: tlb_entry_count %lu\n", hwt0_pc->tlb_entry_count));                // FUSEDOS
   // TRACE(TRACE_VMM, ("FUSEDOS vmm_MapUserSpace: mas2 & MAS2_EPN(-1) %016llX, mas7_3 & MAS7_3_RPN(-1) %016llX\n",   // FUSEDOS
   //       te->mas2 & MAS2_EPN(-1), te->mas7_3 & MAS7_3_RPN(-1)));                                                    // FUSEDOS
   // TRACE(TRACE_VMM, ("FUSEDOS vmm_MapUserSpace: tlb entry, slot 0x%lx, mas1 0x%lx, mas2 0x%lx\n",                  // FUSEDOS
   //       te->slot, te->mas1, te->mas2));                                                                            // FUSEDOS
   // TRACE(TRACE_VMM, ("FUSEDOS vmm_MapUserSpace: tlb entry, mas7_3 0x%lx, mas8 0x%lx, mmucr3 0x%lx\n",              // FUSEDOS
   //       te->mas7_3, te->mas8, te->mmucr3));                                                                        // FUSEDOS

#endif                                                                                                                 // FUSEDOS
    return 0;
}

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
int vmm_UnmapUserSpace(void* virtualaddress, size_t tlbsize, int pid)
{
    uint64_t tlbmask;
    TRACE(TRACE_VMM, ("Unmapping TLB: virtual=%p tlbsize=%ld  pid=%d\n", virtualaddress, tlbsize, pid));
    
    switch(tlbsize)
    {
        case 1024*1024:      tlbmask = MAS6_ISIZE_1MB;  break;
        case 16*1024*1024:   tlbmask = MAS6_ISIZE_16MB; break;
        case 1024*1024*1024: tlbmask = MAS6_ISIZE_1GB;  break;
        default: 
            printf("(E) vmm_UnmapUserSpace: invalid size %lu cannot be unmapped\n", tlbsize);
            return -1;
    }
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_UNMAPTLB_, (uint64_t)virtualaddress, tlbsize, pid, 0);
    
    vmm_tlbilx(virtualaddress, MAS6_SPID(pid) | tlbmask | MAS6_SAS(0) ); 
    isync();
    return 0;
}

void VMM_Init()
{   
    uint64_t l2addr;
    uint64_t minkerneladdress;
    uint64_t maxkerneladdress;
    
    uint64_t mask = ~(1<<(ProcessorThreadID()));
    mtspr(SPRN_TENC,((mask) & 0xf)); // temporarily disable the other hardware threads on this core
    isync();
    
    vmm_populateTLBTable();
    
    // Find the 1G Vaddr=0 i-erat and d-erat and mark them so they remain after a TLB invalidate
    // (erat index in bits 52:63. bits 50:51 are also returned indicating number of entries found)
    int index_d = eratsx(0, MMUCR0_TLBSEL_DERAT); 
    int index_i = eratsx(0, MMUCR0_TLBSEL_IERAT);
    uint64_t w0_d = 0; uint64_t w1_d = 0;
    eratre(index_d, &w0_d, &w1_d, MMUCR0_TLBSEL_DERAT);
    eratwe(index_d, w0_d, w1_d, MMUCR0_TLBSEL_DERAT | MMUCR0_ECL(1) );
    uint64_t w0_i = 0; uint64_t w1_i = 0;
    eratre(index_i, &w0_i, &w1_i, MMUCR0_TLBSEL_IERAT);
    eratwe(index_i, w0_i, w1_i, MMUCR0_TLBSEL_IERAT | MMUCR0_ECL(1) );
    isync();

    // Invalidate the 1G TLB entry at effective address 0 and tid 0
    //printf("ready to invalidate 1g tlb\n");
    vmm_tlbilx(0, MAS6_ISIZE_1GB);  /*BEAM: operating on NULL*/ 
    if (NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_L2Only)
    {
	// In L2Only mode, firmware will have mapped 31 MB starting
	// at vaddr 0 rather than 1 GB.  Invalidate those mappings.
	// The I- and D-ERATs will be holding 16MB mappings at vaddr=0 rather
	// than 1GB mappings, but the pinning that happened above and the
	// comments below still apply.
	vmm_tlbilx(0, MAS6_ISIZE_16MB);  /*BEAM: operating on NULL*/ 
	uint64_t m;
	for (m = 16; m < 31; m++)
	{
	    vmm_tlbilx((void *) (m * (1024*1024)), MAS6_ISIZE_1MB);
	}
    }
    isync();
    // Create 12 1M TLBs. Note as long as the 1G D-ERAT and I-ERAT entries exist, no hardware access through these TLBs will occur
    // therefore no overlap condition between the 1M TLB and the 1G erats at virtual address 0 will be detected
    //printf("starting loop to create new tlbs\n");
    int x; 
    int kernelwriteable = 0;
    uint64_t exclude;
#if CNK_ALLOW_KERNELNULLREAD
    exclude = 0;
#else
    exclude = 4095;
#endif
    size_t KernelSize = ((uint64_t)(&__KERNEL_END)) - ((uint64_t)(&__KERNEL_TEXT_START));
    
    for(x=0; x<(KernelSize + CONFIG_CNK_PAGE_SIZE- 1 ) / CONFIG_CNK_PAGE_SIZE; x++, exclude=0)
    {
        kernelwriteable = 0;
        if(1024*1024*x >= (uint64_t)(&__KERNEL_TEXT_END))
            kernelwriteable = 1;
        vmm_tlbwe_slot(3,
                   MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1MB,
                   MAS2_EPN((1024*1024*x)>>12)   | MAS2_W(0) | MAS2_I(0) | MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
                   MAS7_3_RPN((1024*1024*x + NodeState.DomainDesc.ddrOrigin)>>12) | MAS3_SR(1) | MAS3_SW(kernelwriteable) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U0(1),
                   MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
                   MMUCR3_X(exclude) | MMUCR3_R(1) |     MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
                   );
    }
    isync();
    //printf("Clearing ExtClass from erats\n");
    // Reset the ExtClass field so that these erats can be invalidated
    eratwe(index_d, w0_d, w1_d, MMUCR0_TLBSEL_DERAT | MMUCR0_ECL(0) );
    eratwe(index_i, w0_i, w1_i, MMUCR0_TLBSEL_IERAT | MMUCR0_ECL(0) );
    isync();

    // invalidate the 1G erat entries
    //printf("invalidate the 1G ERATS\n");
    eratilx(0, MMUCR0_TLBSEL_DERAT);   /*operating on NULL*/

    // Turn on the MMCR1[DTTID] and MMCR1[ITTID] bits so that the erats support a tid field of 2:13. (this adds 2:5 to the default 6:13)
    mtspr(SPRN_MMUCR1, mfspr(SPRN_MMUCR1) | _BN(47) | _BN(45) ); // do not isync() until after the invalidation of the ierat

    eratilx(0, MMUCR0_TLBSEL_IERAT);   /*operating on NULL*/

    // Next instruction fetch will cause an i-erat miss, followed by a tlb hit, bringing in the correctly sized erat for the new tlb
    isync();
    
    for(l2addr = (uint64_t)__l2_op_ptr(&__KERNEL_ATOMICS_START, 0); l2addr <= (uint64_t)__l2_op_ptr(&__KERNEL_ATOMICS_END, 0); l2addr+=16*1024*1024)
    {
        vmm_tlbwe_slot(2,
                   MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_16MB,
                   MAS2_EPN(  l2addr >> 12) | MAS2_W(0) | MAS2_I(1) |	MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
                   MAS7_3_RPN(l2addr >> 12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(0) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U1(1) | MAS3_U0(1),
                   MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
                   MMUCR3_X(0) | MMUCR3_R(1) |	MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
                   );
    }
    
    // Add non-guarded user space L1p and L2 MMIO mappings
    vmm_tlbwe_slot(3,
          MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1MB,
          MAS2_EPN(  CONFIG_FAST_L1P_BASE >> 12) | MAS2_W(0) | MAS2_I(1) |	MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
	  MAS7_3_RPN(PHYMAP_MINADDR_L1P >> 12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(0) | MAS3_UR(1) | MAS3_UW(1) | MAS3_UX(0) | MAS3_U1(0),
          MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
          MMUCR3_X(0) | MMUCR3_R(1) |	MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
         );
    vmm_tlbwe_slot(3,
          MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1MB,
          MAS2_EPN(  CONFIG_FAST_L2_BASE >> 12) | MAS2_W(0) | MAS2_I(1) |	MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
	  MAS7_3_RPN(PHYMAP_MINADDR_L2 >> 12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(0) | MAS3_UR(1) | MAS3_UW(1) | MAS3_UX(0) | MAS3_U1(0),
          MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
          MMUCR3_X(0) | MMUCR3_R(1) |	MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
         );

    isync();
    
    Kernel_WriteFlightLog(FLIGHTLOG, FL_VMMInited, (uint64_t)&__KERNEL_TEXT_START, (uint64_t)&__KERNEL_END - (uint64_t)&__KERNEL_TEXT_START, 
                          (uint64_t)&__KERNEL_TEXT_END - (uint64_t)&__KERNEL_TEXT_START, (uint64_t)&__KERNEL_ATOMICS_END - (uint64_t)&__KERNEL_ATOMICS_START);
    
    if((!Personality_IsFPGA()) && ((NodeState.Personality.Kernel_Config.NodeConfig & PERS_ENABLE_MU) != 0))
    {
        minkerneladdress = ROUND_UP_1M((uint64_t)(&__KERNEL_TEXT_END));
        maxkerneladdress = ROUND_DN_1M((uint64_t)(&__KERNEL_END));    
        DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE) + 0, minkerneladdress);
        DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE) + 0, maxkerneladdress);
        
        minkerneladdress = 16*1024*1024;
        maxkerneladdress = ((uint64_t)NodeState.Personality.DDR_Config.DDRSizeMB)*1024*1024 - 1;
        DCRWritePriv(MU_DCR(MIN_SYS_ADDR_RANGE) + 1, minkerneladdress);
        DCRWritePriv(MU_DCR(MAX_SYS_ADDR_RANGE) + 1, maxkerneladdress);
    }
    
    // re-enable the other hardware threads on this core
    mtspr(SPRN_TENS,((mask) & 0xf));
}

int vmm_SetupBackgroundScrub()
{
    uint64_t memory_on_node   = NodeState.Personality.DDR_Config.DDRSizeMB * 1024ull;      // total memory in kB
    uint64_t memory_per_scrub = 32768;                                                     // kB scrubbed per timer event
    uint64_t clockrate        = 1600ull * 1000 * 1000 / 2;                                 // gea timers are on the 2x domain
    uint64_t rate             = 2 * 3600;                                                  // number of seconds to scrub entire memory
    uint64_t period           = clockrate * rate / (memory_on_node / memory_per_scrub);    // gea clocks per second
    
    BIC_WriteGeaInterruptControlRegisterHigh(GEA_DCR__GEA_INTERRUPT_STATE_CONTROL_HIGH__TIMER2_INT_set(2));
    DCRWritePriv(GEA_DCR(TIMER2_CONFIG), GEA_DCR__TIMER2_CONFIG__RELOAD_VAL_set(period) | GEA_DCR__TIMER2_CONFIG__IE_set(1) | GEA_DCR__TIMER2_CONFIG__ARE_set(1));
    DCRWritePriv(GEA_DCR(TIMER2), GEA_DCR__TIMER2__COUNT_set(period));
    ppc_msync();
    return 0;
}

Lock_Atomic_t backgroundScrubLock;
uint64_t backgroundScrubVA   = 0;
uint64_t backgroundScrubPA   = 0;
uint64_t backgroundScrubSize = 0;
uint64_t backgroundScrubSlot = ~0;
int vmm_setScrubSlot()
{
    size_t   size;
    uint64_t paddr;
    Kernel_Lock(&backgroundScrubLock);
    vmm_getSegment(CONFIG_MAX_APP_PROCESSES, IS_SCRUBWINDOW, &backgroundScrubVA, &paddr, &backgroundScrubSize);
    vmm_getSegmentEntry(CONFIG_MAX_APP_PROCESSES, backgroundScrubVA, &paddr, &size, &backgroundScrubSlot);
    Kernel_WriteFlightLog(FLIGHTLOG, FL_SCRUBSETP, backgroundScrubVA, backgroundScrubSize, backgroundScrubSlot, 0);
    
    Kernel_Unlock(&backgroundScrubLock);
    return 0;
}

int vmm_clearScrubSlot()
{
    Kernel_Lock(&backgroundScrubLock);
    backgroundScrubVA   = 0;
    backgroundScrubSize = 0;
    Kernel_Unlock(&backgroundScrubLock);
    return 0;
}

void IntHandler_MemoryScrub(int status_reg, int bitnum)
{
    size_t index;
    Kernel_Lock(&backgroundScrubLock);
    
    if(backgroundScrubVA != 0)
    {
        Kernel_WriteFlightLog(FLIGHTLOG, FL_SCRUBSTRT, backgroundScrubPA, (backgroundScrubSize>>5), 0, 0);
        vmm_tlb_touch_tlbs();
        vmm_tlbwe_slot( 3 - (backgroundScrubSlot%4),
                        MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB,
                        MAS2_EPN(((uint64_t)backgroundScrubVA)>>12)   | MAS2_W(0) | MAS2_M(1) | MAS2_E(0) | MAS2_I(1) | MAS2_G(1),
                        MAS7_3_RPN(((uint64_t)0x10000000000ull | (backgroundScrubPA<<5))>>12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_U0(1),
                        MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
                        MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF)
            );
        isync();
        uint64_t* l2atomicbase = (uint64_t*)((backgroundScrubVA - Kernel_L2AtomicsBaseAddress())>>5);
        for(index=0; index < (backgroundScrubSize>>5) / sizeof(uint64_t); index += 128 / sizeof(uint64_t))
            L2_AtomicStoreAdd(&l2atomicbase[index], 0);
        
        backgroundScrubPA += (backgroundScrubSize>>5);
        if(backgroundScrubPA >= NodeState.Personality.DDR_Config.DDRSizeMB * 1024ull * 1024ull)
            backgroundScrubPA = 0;
        
        Kernel_WriteFlightLog(FLIGHTLOG, FL_SCRUBSTOP, backgroundScrubPA, backgroundScrubSize, 0, 0);
    }
    
    Kernel_Unlock(&backgroundScrubLock);
    
    GEA_DCR_PRIV_PTR->gea_interrupt_state__noncritical = GEA_DCR__GEA_INTERRUPT_STATE__TIMER2_INT_set(1);
    BIC_WriteGeaInterruptState(GEA_DCR__GEA_INTERRUPT_STATE__TIMER2_INT_set(1));
}
#endif                                                                                                                 // FUSEDOS
