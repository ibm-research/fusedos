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

#include <cnk/src/Regs.h>
#include <cnk/include/SPRG_Usage.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/common/compiler_support.h>
#include <sys/types.h>

#include "fusedos_config.h"
#include "fusedos.h"

extern spc_context_t* get_context(void);
extern void resume_app(void);
extern void start_app(void);
extern void save_fpu(spc_context_t*);
extern void upc_init(void);
extern void upcp_init(int, int, unsigned);

void App_SetIULivelockConfig()
{
    spc_context_t* context = get_context();
    uint64_t period = context->BG_IULLAVOIDPERIOD;
    uint64_t delay = context->BG_IULLAVOIDDELAY;

    uint64_t iullcr = mfspr(SPRN_IULLCR);
    iullcr &= ~IULLCR_IULL_EN;
    iullcr &= ~IULLCR_LL_HOLD_DLY(-1);
    iullcr &= ~IULLCR_LL_TRIG_DLY(-1);
    if ((period != 0) && (delay != 0))
        iullcr |= (IULLCR_IULL_EN | IULLCR_LL_HOLD_DLY(delay) | IULLCR_LL_TRIG_DLY(period));
    mtspr(SPRN_IULLCR, iullcr);
    isync();
}

void load_app_tlb()
{
    spc_context_t* context = get_context();
    int i;

    for (i = context->tlb_entry_install; i < context->tlb_entry_count; i++) {
        tlb_entry_t* te = &(context->tlb_entry[i]);
        // Write to a specific slot
        tlbwe_slot(te->slot, te->mas1, te->mas2, te->mas7_3, te->mas8, te->mmucr3);
        context->tlb_entry_install++;
    }
}

// From cnk/src/CoreDump.cc
static uint64_t TSIZE_MASK[] = {
    0,
    0x1000ul,      // 4KB
    0,
    0x10000ul,     // 646KB
    0,
    0x100000ul,    // 1MB
    0,
    0x1000000ul,   //16MB
    0,
    0x10000000ul,  // 256MB
    0x40000000ul,  // 1GB
    0x100000000ul, // 4GB
    0,
    0x4000000000ul,
};

// Derived from coredump_memory() in cnk/src/CoreDump.cc
void unload_app_tlb()
{
    spc_context_t* context = get_context();
    int i;

    for (i = 0; i < context->tlb_entry_count; i++) {
        tlb_entry_t* te = &(context->tlb_entry[i]);
        int size = (te->mas1 & MAS1_TSIZE(-1)) >> (63-55);
        uint64_t pageMask = TSIZE_MASK[size] - 1;
        uint64_t xMask = te->mas2 & MAS2_EPN(-1) & pageMask;
        uint64_t xOffset = (te->mmucr3 & MMUCR3_X(-1)) ? (xMask + 0x1000) : 0;
        uint64_t ea = (te->mas2 & MAS2_EPN(-1) & ~pageMask ) + xOffset;
        uint64_t pid = (te->mas1 & MAS1_TID(-1)) >> (63-47);
        switch(size) {
        case 5: // 1 MB
            tlbilx((void*)(ea), MAS6_SPID(pid) | MAS6_ISIZE_1MB | MAS6_SAS(0));
            break;
        case 7: // 16 MB
            tlbilx((void*)(ea), MAS6_SPID(pid) | MAS6_ISIZE_16MB | MAS6_SAS(0));
            break;
        case 10: // 1 GB
            tlbilx((void*)(ea), MAS6_SPID(pid) | MAS6_ISIZE_1GB | MAS6_SAS(0));
            break;
        }
        context->tlb_entry_install--;
    }
    context->tlb_entry_count = 0;
}

void monitor()
{
    spc_context_t* context = get_context();

// Disable all spcs except for spc 0
//if (context->id != 0) ppc_wait();

    if (context->id % 4 == 0) {
        App_SetIULivelockConfig();
    }

    // Need to set this for the core id and hw thread code used by UPC
    // even when an app is not running. start_app will set it from cl
    // (App_Setup.cc::App_CreateMainThread).
    mtspr(SPRG_SPIinfo, context->id);

    // Setup the CCR3[EEPRI] value
#define CNK_EEPRI_DEFAULT 1
    uint32_t enable_eepri = CNK_EEPRI_DEFAULT;
//    App_GetEnvValue("BG_EEPRIORITY", &enable_eepri);
    uint64_t ccr3_value = mfspr(SPRN_CCR3);
    if (enable_eepri) {
        ccr3_value |= CCR3_EEPRI;
    } else {
        ccr3_value &= ~(CCR3_EEPRI);
    }
    mtspr(SPRN_CCR3, ccr3_value); // Set the requested value in the CCR3 register.

    while (1) {
        uint64_t* bic_int_send = (uint64_t*)(context->bic_int_send);
        uint64_t mask;
        int mode;
        unsigned ctrMode;

        // Use the CNK MSR settings when we're in the monitor
        mtmsr(MSR_CNK);
        while (LoadReserved(&(context->start)) == 0) ppc_waitrsv();
        context->start = 0;
        switch (context->command) {
        case SPC_START:
            start_app();
            while (1);

        case SPC_RESUME:
            resume_app();
            while (1);

        case SPC_LOAD_TLB:
            load_app_tlb();
#if defined USE_WAITRSV
            context->ipi_wakeup = 1;
#else
            *bic_int_send = context->bic_value;
#endif
            break;

        case SPC_UNLOAD_TLB:
            unload_app_tlb();
#if defined USE_WAITRSV
            context->ipi_wakeup = 1;
#else
            *bic_int_send = context->bic_value;
#endif
            break;

        case SPC_EXIT:
            // CL will send an IPI to unstick a looping app. If the app
            // isn't looping, then clear the IPI before exiting.
            mask = BIC_ReadStatusExternalRegister0(SPC_TO_CPU(context->id) & 0x3);
            BIC_WriteClearExternalRegister0(SPC_TO_CPU(context->id) & 0x3, mask);

            ici();
            isync();
// Now done in SPC_UNLOAD_TLB
//            if (context->id % 4 == 0) unload_app_tlb();
            context->ex_code = 0;
            break;

        case SPC_SAVE_FPU:
            save_fpu(context);
            context->ex_code = 0;
#if defined USE_WAITRSV
            context->ipi_wakeup = 1;
#else
            *bic_int_send = context->bic_value;
#endif
            break;

        case SPC_UPC_INIT:
            upc_init();
#if defined USE_WAITRSV
            context->ipi_wakeup = 1;
#else
            *bic_int_send = context->bic_value;
#endif
            break;

        case SPC_UPCP_INIT:
            mode = (int)(context->ipi_message.parm1 >> 32);
            ctrMode = (unsigned)(context->ipi_message.parm1 & 0xffffffffull);
            upcp_init(context->id / 4, mode, ctrMode);
#if defined USE_WAITRSV
            context->ipi_wakeup = 1;
#else
            *bic_int_send = context->bic_value;
#endif
            break;

        default:
            while (1);
        }
    }
}

void ipi_handler()
{
    spc_context_t* context = get_context();

    // Clear the IPI to avoid looping
    uint64_t mask = BIC_ReadStatusExternalRegister0(SPC_TO_CPU(context->id) & 0x3);
    BIC_WriteClearExternalRegister0(SPC_TO_CPU(context->id) & 0x3, mask);

    switch (context->ipi_message.fcn) {
    case SPC_IPI_QUIT:
        context->ipi_message.fcn = 0;
        monitor();
    }

    // Should not be here...
    while (1);
}

void suspend_cores()
{
    spc_context_t* context = get_context();

    uint64_t threadmask[2] = { 0,0 };
    threadmask[(SPC_TO_CPU()/4)/16] = _BN(SPC_TO_CPU(context->id)%64);
    DCRWritePriv(TESTINT_DCR(THREAD_ACTIVE0), threadmask[0]);
    DCRWritePriv(TESTINT_DCR(THREAD_ACTIVE1), threadmask[1]);
}

void resume_cores()
{
    DCRWritePriv(TESTINT_DCR(THREAD_ACTIVE0), ~0ull);
    DCRWritePriv(TESTINT_DCR(THREAD_ACTIVE1), 0xf000000000000000ull);
}

void my_memset(void* s, int c, size_t n)
{
    char* sp = s;
    int i;

    for (i = 0; i < n; i++) *(sp + i) = c;
}

// Needed by assert used by UPC code. Should we throw an exception to
// the CL when this happens?
void __assert_fail(const char* assertion, const char* file, unsigned int line, const char* function)
{
    while (1);
}
