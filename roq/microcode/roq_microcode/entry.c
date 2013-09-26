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

#define __KERNEL__
#include <hwi/include/common/bgq_alignment.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/testint_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>
#include <hwi/include/bqc/pe_dcr.h>

#include <firmware/include/mailbox.h>
#include <firmware/include/personality.h>
#include <firmware/include/Firmware_Interface.h>
#include <firmware/include/BeDram_inlines.h>
#undef __KERNEL__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <spi/include/mu/Addressing.h>
#ifndef MUSPI_assert
 #define MUSPI_assert
#endif
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>
#include <transcon.h>

#include <roq_kernel.h>
#include <roq_debug.h>

#define STACK_SIZE (20*1024*1024)
#define CORES_COUNT 2
#define FIRST_CORE 15

#ifdef ROQ_RESILIENCY
#define NUM_INTERRUPT_CLASSES 128 // according to A2 user manual, page 209, table 7-2. We only use a small subset.
#define ROQ_THREAD ((ProcessorCoreID()-FIRST_CORE)*4 + ProcessorThreadID())

RoQ_Regs_t InterruptState[8][3] ALIGN_L2_CACHE; // Storage for 8 threads + 3 interrupt priorities
RoQ_InterruptHandler_t *interruptHandler[8][NUM_INTERRUPT_CLASSES]; // 1 pointer to an interrupt handler per interrupt class
#endif

#define KTEXT_ENTRY __attribute__((__section__(".text.domain")))

char _stack_domain[CORES_COUNT][4][STACK_SIZE] ALIGN_L2_CACHE /* = { 0, } */ ;
uint64_t spin_table_array[17][4];
uint64_t _thread_param[68];
volatile Firmware_Interface_t *_firmware_domain = 0;
Firmware_DomainDescriptor_t _descriptor_domain;
unsigned _numberOfCores_domain = 0;
volatile uint64_t _auxEntryPointBarrier_domain = 0;
volatile uint64_t _traditionalEntryBarrier_domain = 0;
volatile int64_t _leader_domain = -1;

int __gxx_personality_v0;
FILE *stdin;
FILE *stdout;
FILE *stderr;
uint64_t PCS;

extern void roq_kernel(void);
#ifdef ROQ_RESILIENCY
#define SET_STACKP() {													                                                 \
	int i;															                                                     \
	for (i = 0; i < 64; i++)											                                                 \
		_stack_domain[ProcessorCoreID()-FIRST_CORE][ProcessorThreadID()][STACK_SIZE-i] = i;                              \
    size_t pStk = (size_t)((size_t)&(_stack_domain[ProcessorCoreID()-FIRST_CORE][ProcessorThreadID()][STACK_SIZE-128])); \
    asm volatile ( "mr 1,%0;" : : "r" (pStk) : "r1", "memory" );		                                                 \
  }
#else
#define SET_STACKP() {													                                                \
	size_t pStk = (size_t)((size_t)&(_stack_domain[ProcessorCoreID()-FIRST_CORE][ProcessorThreadID()][STACK_SIZE-64])); \
    asm volatile ( "mr 1,%0;" : : "r" (pStk) : "r1", "memory" );		                                                \
  }
#endif

#define TRACE(a)
#define ABORT()     _firmware_domain->terminate(__LINE__)
#define FIRMWARE()  _firmware_domain
#define DD()        _descriptor_domain

__INLINE__ void write_derat(unsigned index, uint64_t hi, uint64_t lo)
{
	mtspr(SPRN_MMUCR0, MMUCR0_TLBSEL_DERAT);
	asm volatile(
		"eratwe %1,%0,1;"
		"eratwe %2,%0,0;"
		: : "r" (index), "r" (lo), "r" (hi)
		: "memory"
	);
}

__INLINE__ void write_ierat(unsigned index, uint64_t hi, uint64_t lo)
{
	mtspr(SPRN_MMUCR0, MMUCR0_TLBSEL_IERAT);
	asm volatile(
		"eratwe %1,%0,1;"
		"eratwe %2,%0,0;"
		: : "r" (index), "r" (lo), "r" (hi)
		: "memory"
	);
}

#ifdef ROQ_RESILIENCY
void
setInterruptHandler(RoQ_InterruptHandler_t handler, uint16_t class)
{
//	uint16_t thread = ProcessorCoreID() * 4 + ProcessorThreadID();
	OPD_Entry_t* opd = (OPD_Entry_t*)handler;
	DSTRONG_ASSERT((class <= 0xFFF) && ((class & 0x1F) == 0), "invalid interrupt class %x\n", class);
	interruptHandler[ROQ_THREAD][class >> 5] = opd->Entry_Addr;
}

void
defaultInterruptHandler(RoQ_Regs_t* context, uint64_t code)
{
	static char* INTERRUPT_CODES[] = {
		"Machine Check",
		"Critical Interrupt",
		"Debug",
		"Data Storage Exception",  
		"Instruction Storage Exception",
		"External Interrupt",
		"Alignment Interrupt",
		"Program Interrupt",
		"Floating Point Unavailable",
		"System Call",
		"APU",
		"Decrementer",
		"Fixed Interval Timer",
		"Watchdog Timer",
		"Data TLB Miss",
		"Instruction TLB Miss",
		"Vector Unavailable",
		"?",
		"?",
		"?",
		"Processor Doorbell",
		"Processor Doorbell Critical",
		"Guest Doorbell",
		"Guest Doorbell Critical",
		"Hypervisor System Call",
		"Hypervisor Privilege",
		"LRAT Error"
	};
	
	unsigned n = code>>5;
	
	printf("Unexpected Interrupt.  Code=%lX (%s)  IP=%lx  LR=%lx  MSR=%lx DEAR=%lx ESR=%lx MCSR=%lx Timestamp=%lx\n", 
		   (unsigned long)code, 
		   (n < sizeof(INTERRUPT_CODES)/sizeof(char*)) ? INTERRUPT_CODES[n] : "?",  
		   context->ip, 
		   context->lr, 
		   context->msr, 
		   mfspr(SPRN_DEAR), 
		   mfspr(SPRN_ESR),
		   mfspr(SPRN_MCSR),
		   GetTimeBase());

	while(1) {} ;
}

void
enableDecrementerInterrupt(uint32_t period, uint32_t initial, int are)
{
	mtspr( SPRN_DEC, initial );

	mtspr( SPRN_TSR, TSR_DIS);  // Ack decrementer interrupt

	if (are) { // auto-reload enable
		mtspr( SPRN_DECAR, period);
		mtspr( SPRN_TCR, mfspr(SPRN_TCR) | (TCR_DIE | TCR_ARE));
	} else {
		mtspr( SPRN_TCR, mfspr(SPRN_TCR) | TCR_DIE);  // Enable decrementer interrupt
	}

	asm volatile ("wrteei 1"); // set external enable bit
}

void
enableDataAccessDebugInterrupt(uint64_t address0, uint64_t mask0, uint64_t address1, uint64_t mask1)
{
	printf("enabling data access debug interrupt to %016lx & %016lx and %016lx & %016lx\n",
		   address0, mask0, address1, mask1);
	mtspr( SPRN_DAC1, address0);
	mtspr( SPRN_DAC2, mask0);

	mtspr( SPRN_DAC3, address1);
	mtspr( SPRN_DAC4, mask1);

	mtspr( SPRN_DBCR1, (DBCR1_IAC1US_ANY | DBCR1_IAC2US_ANY | DBCR1_IAC3US_ANY | DBCR1_IAC4US_ANY |
						DBCR1_IAC1ER_EFF | DBCR1_IAC2ER_EFF | DBCR1_IAC3ER_EFF | DBCR1_IAC4ER_EFF));
	mtspr( SPRN_DBCR2, DBCR2_DAC12M );
	mtspr( SPRN_DBCR3, DBCR3_DAC34M );

	mtspr( SPRN_DBCR0, DBCR0_IDM | DBCR0_DAC1_ST | DBCR0_DAC2_ST | DBCR0_DAC3_ST | DBCR0_DAC4_ST );

	mtmsr( mfmsr() | MSR_DE );
}

// only works if the check values have been put in place at the top of each stack
int check_stack(int tid) {
	int rc  = 0;
	int t,i = 0;

	int err;
	for (t = (tid >= 0 ? tid : 0); t < (tid >= 0 ? tid+1 : 8); t++) {
		err = 0;
		for (i = 0; i < 64; i++) {
			if (_stack_domain[t/4][t%4][STACK_SIZE-i] != i) {
				rc = -1;
				err = -1;
//				printf("ERROR: Stack %u is corrupted: expected 0x%x, got 0x%x\n", t, 0xF0, _stack_domain[t/4][t%4][STACK_SIZE-i]);
			}
		}
		if (err == 0) printf ("Stack %u is good\n", t);
		else          printf ("Stack %u is corrupted\n", t);
	}

	return rc;
}
#endif

void
fw_memset(char *ptr, char value, size_t size)
{
	int i;
	for (i = 0; i < (int)size; i++) {
		ptr[i] = value;
	}
}

int64_t
testAndSet(volatile int64_t * atomic, int64_t preCondition, int64_t value)
{

	register int64_t original, newValue;

	ppc_msync();

	do {
		original = LoadReserved((uint64_t *) atomic);
		if (original == preCondition) {
			newValue = value;
		} else {
			newValue = original;
		}
	}
	while (!StoreConditional((uint64_t *) atomic, newValue));

	return newValue;
}

void KTEXT_ENTRY
spin_table(volatile uint64_t *atomic, uint64_t preCondition)
{
	register uint64_t original;
	void *(*gogogo)(void *);

	ppc_msync();
#ifdef ROQ_RESILIENCY
	if (ProcessorID() != ROQ_TX_THREAD        &&
		ProcessorID() != ROQ_RX_THREAD        &&
		ProcessorID() != ROQ_FORWARDER_THREAD &&
		ProcessorID() != ROQ_ROUTER_THREAD    &&
		ProcessorID() != ROQ_CHECKER_THREAD   &&
	    ProcessorID() != ROQ_NOTIFIER_THREAD) {
		
		// Disable this thread. Wake it up by writing into TENS
		mtspr(SPRN_TENC, TEN_TEN(1 << ProcessorThreadID()));
	}
#else
	if (ProcessorID() != 67) {
	        /* PPC_INST_WAIT */
	        asm volatile("1: .long 0x7c00007c; b 1b");
	}
#endif
	do {
		original = LoadReserved((uint64_t *) atomic);
		if (original != preCondition) {
			gogogo = (void *(*)(void *))original;
			gogogo((void*)_thread_param[ProcessorID()]);
		}
	} while (1);
}

#ifdef ROQ_RESILIENCY
// dirty hack to get more than 1 thread - mkaufman
uint8_t hw_thread = 67; // 61,62,63 and 67 allowed.
#endif

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
{
#ifdef ROQ_RESILIENCY
	uint8_t core_id   = hw_thread/4;
	uint8_t thread_id = hw_thread%4;

	_thread_param[hw_thread] = (uint64_t)arg;
	spin_table_array[core_id][thread_id] = (uint64_t) start_routine;
#else
	_thread_param[67] = (uint64_t)arg;
	spin_table_array[16][3] = (uint64_t) start_routine;
#endif
	return 0;
}

void KTEXT_ENTRY
barrierThreads(volatile uint64_t * barrier, unsigned numberOfThreads)
{
	int64_t timeout = 1000000;

	Fetch_and_Add((uint64_t *) barrier, 1);

	while ((*barrier < numberOfThreads) && (--timeout > 0))
		/* spin */
		;

	if (timeout <= 0) {
		printf
		    ("(E) Barrier @%p->%ld timed out (number of threads = %d)\n",
		     barrier, *barrier, numberOfThreads);
		FIRMWARE()->terminate(__LINE__);
	}
	return;
}

void KTEXT_ENTRY
auxiliaryEntryPoint_domain(void *arg)
{
	//  +--------------------------------------------------------------------------------+
	//  | NOTE: This function represents the common entry point for all threads in the   |
	//  |       domain.  Thus it provides the high level implementation of the test code |
	//  |       that is executed on every thread.                                        |
	//  +--------------------------------------------------------------------------------+

	// If we are in takeCPU mode and we are not the leader thread, then we are still on
	// the BeDRAM (tiny) stack.  Get off immediately.
	// @todo : move this into an aux functtion

	if ((FIRMWARE()->deprecated.personalityPtr->Kernel_Config.
	     NodeConfig & PERS_ENABLE_TakeCPU) != 0) {
		if (_leader_domain != ProcessorID()) {
			SET_STACKP();
		}
	}
#if 0
	//uint64_t domain = (uint64_t)arg;
	TRACE(("(>) %s arg=%p : leader=%ld this=%d\n", __func__, arg,
	       _leader_domain, ProcessorID()));

#endif


#ifdef ROQ_RESILIENCY
//	uint16_t thread = ProcessorCoreID() * 4 + ProcessorThreadID();
	// Thread 0 on every node installs the interrupt vector:

	// Context is saved here in case of an interrupt. Only for threads 60-67
	mtspr(SPRN_SPRG0, (uint64_t)(&InterruptState[ROQ_THREAD][0]));	

	// Array with pointers to interrupt handler for each class
	mtspr(SPRN_SPRG4, (uint64_t)(&interruptHandler[ROQ_THREAD]));

	// Install default interrupt handler for every class. User can overwrite this later
	int c;
	for(c = 0; c <= 0xFFF; c += 0x20) {
		setInterruptHandler(defaultInterruptHandler, c);
	}

	extern uint64_t _vec_domain_MCHK;
	if (ROQ_THREAD == 0) {
		printf("(*) Installing IVEC @%p\n", &_vec_domain_MCHK);
		if (FIRMWARE()->installInterruptVector(&_vec_domain_MCHK, 0x1000) != FW_OK) {
			printf("(E) bad rc from installInterruptVector\n");
			tlbilx(0,  MAS6_SPID(0) | MAS6_SAS(0));
			ABORT();
		}
	}
#endif
	// Wait for all threads in this domain to arrive at this point:
	barrierThreads( &_auxEntryPointBarrier_domain, _numberOfCores_domain * 4 );

	/* core tlb init */
	int i,j;
	if (ProcessorThreadID() == 0) {
		/* search and invalidate ZERO tlb */
		uint32_t index = tlbsx(0x3c0000000ULL >> 12, MAS5_SGS(0) | MAS5_SLPID(0), MAS6_SPID(0) | MAS6_SAS(0));
		tlbwe_slot(index,
			MAS1_V(0) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_4KB,
			MAS2_EPN((0x00000000ULL >> 12)) | MAS2_W(0) | MAS2_I(0) | MAS2_M(0) | MAS2_G(0) | MAS2_E(0),
			MAS7_3_RPN((-1ULL >> 12)) | MAS3_U0(1) | MAS3_SR(0) | MAS3_SW(0) | MAS3_SX(0) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U3(0),
			MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
			MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF)
		);
		isync();

		/* remove old ERATS */
		for (i = 0; i < 2; i++) {
			unsigned erat_size;

			if (i==0) {
				erat_size = 32;
				mtspr(SPRN_MMUCR0, MMUCR0_TLBSEL_DERAT);
			} else {
				erat_size = 16;
				mtspr(SPRN_MMUCR0, MMUCR0_TLBSEL_IERAT);
			}

			for (j = 0; j < (int)erat_size; j++) {
				uint64_t erat_entry_hi;
				asm volatile( "eratre %0,%1,0;" : "=&r" (erat_entry_hi) : "r" (j) : "memory" );

				if (erat_entry_hi & ERAT0_V) {
					if ( ERAT0_GET_EPN(erat_entry_hi) == (ROQ_VIRTUAL_MAPPING >> 12) ) {
						continue;
					}

					if (i == 0) {
						write_derat(j, 0, 0);
					} else {
						write_ierat(j, 0, 0);
					}
				}
			}
		}

		/* map unpriviliaged MMIO area */
		tlbwe(
			MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB | MAS1_IPROT(1),
			MAS2_EPN(   (PHYMAP_MINADDR_MMIO) >> 12) | MAS2_W(0) | MAS2_I(1) | MAS2_M(1) | MAS2_G(1) | MAS2_E(0),
			MAS7_3_RPN( (PHYMAP_MINADDR_MMIO) >> 12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U1(1),
			MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
			MMUCR3_X(0) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
		);
		/* map RAM */
		/* XXX: MAP 16GB!!! */
		for (i=0; i < 16; ++i) {
			//printf("TLBWE: %llx to %llx\n", ROQ_VIRT_CACHE_OFF + (i*0x40000000ULL), (i*0x40000000ULL));
			tlbwe(
				MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB,
				MAS2_EPN((ROQ_VIRT_CACHE_OFF + (i*0x40000000ULL)) >> 12)   | MAS2_W(0) | MAS2_I(0) | MAS2_M(1) | MAS2_G(0) | MAS2_E(0),
				MAS7_3_RPN((i*0x40000000ULL) >> 12) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0),
				MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
				MMUCR3_X(0) | MMUCR3_R(1) |   MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
			);
			tlbwe(
				MAS1_V(1) | MAS1_TID(0) | MAS1_TS(0) | MAS1_TSIZE_1GB,
				MAS2_EPN((ROQ_VIRT_MUCACHE_OFF + (i*0x40000000ULL)) >> 12)   | MAS2_W(0) | MAS2_I(0) | MAS2_M(1) | MAS2_G(1) | MAS2_E(0),
				MAS7_3_RPN((i*0x40000000ULL) >> 12) | MAS3_U0(1) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | MAS3_UR(0) | MAS3_UW(0) | MAS3_UX(0) | MAS3_U1(1),
				MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0),
				MMUCR3_X(0) | MMUCR3_R(1) |   MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) |MMUCR3_ThdID(0xF)
			);
		}
		isync();
	}

	// Wait for all threads in this domain to arrive at this point:
	TRACE(("(*) Before barrier numcores=%d\n", _numberOfCores_domain));
	barrierThreads( &_auxEntryPointBarrier_domain, _numberOfCores_domain * 4 );
	TRACE(("(*) After barrier numcores=%d\n", _numberOfCores_domain));

	if (_leader_domain == ProcessorID()) {
		/* invalidate 0 TLB */
		//tlbilx(0,  MAS6_SPID(0) | MAS6_SAS(0));
		roq_kernel();
	} else {
		spin_table(&spin_table_array[ProcessorCoreID()][ProcessorThreadID()], -1);
	}

	FIRMWARE()->exit(0);
}

void KTEXT_ENTRY
mainCommon_domain(void)
{
	int rc = 0;
	TRACE(("(>) %s this=%d\n", __func__, ProcessorID()));

	/* Fetch the domain descriptor from firmware: */
	if ((rc = FIRMWARE()->getDomainDescriptor(&_descriptor_domain)) != FW_OK) {
		printf("(E) bad return code (%d) from getDomainDescriptor()\n",
		       rc);
		ABORT();
	}

	/* Count the number of active cores in this domain: */
	uint32_t mask = 1;
	while (mask) {
		if (DD().coreMask & mask) {
			_numberOfCores_domain++;
		}
		mask <<= 1;
	}

#if 1
	printf("(*) %s : domain-descriptor={cores=%05X (%d), ddr=%016llX-%016llX entry=%016llX} - leader:%ld\n",
	     __func__, DD().coreMask, _numberOfCores_domain, DD().ddrOrigin,
	     DD().ddrEnd, DD().entryPoint, _leader_domain);
#endif

	TRACE(("(<) %s this=%d numberOfCores=%d\n", __func__,
	      ProcessorID(), _numberOfCores_domain));
}

int KTEXT_ENTRY
traditionalMain_domain(uint64_t domain)
{
	TRACE(("(>) %s : domain:%ld fw:%p descriptor:%p\n", __func__, domain,
	       FIRMWARE(), &_descriptor_domain));

	// Execute common main code on the leader:
	if (_leader_domain == ProcessorID()) {
		mainCommon_domain();
	}
	// Wait for the domain to get here:
	barrierThreads(&_traditionalEntryBarrier_domain,
		       _numberOfCores_domain * 4);

	// Finally, jump to the same place that all other threads are (rendesvous).

	auxiliaryEntryPoint_domain((void *) 0);

	// should never get here
	while (1)
		/* spin */
		;

}

int KTEXT_ENTRY
takeCpuMain_domain(void)
{
	int rc = 0;
	//  +-------------------------------------------------------------------------+
	//  |  NOTE: this function is the domain's "leader" thread for the test.  It  |
	//  |        will pull other threads out of firmware and thus launch the      |
	//  |        domain's test.                                                   |
	//  +-------------------------------------------------------------------------+
	//printf("(>) %s : fw:%p\n", __func__, FIRMWARE());

	mainCommon_domain();
	/* Take the remaining threads 3 from our own core: */
	rc = FIRMWARE()->takeCPU(ProcessorCoreID(),
				 0xF ^ (1 << ProcessorThreadID()),
				 auxiliaryEntryPoint_domain, (void *) 0);

	if (rc != FW_OK) {
		printf("(E) couldnt take auxiliary threads on domain leader (rc=%d)\n", rc);
		ABORT();
	}

	/*
	 *And now take all remaining threads from remaining cores.  Attempts to steal
	 * cores/threads from the other domain should fail:
	 */
	int cpu;
	for (cpu = 0; cpu < 17; cpu++) {

		if (cpu == ProcessorCoreID())
			continue;

		rc = FIRMWARE()->takeCPU(cpu, 0xF, (void (*)(void *))(0x258),
					 (void *) 0);

		if (rc != FW_OK) {
			if ((DD().coreMask & (1 << cpu)) == 0) {
				rc = 0;
			}
		}
	}

	/* Finally, jump to the same place that all other threads are (rendesvous). */
	auxiliaryEntryPoint_domain((void *) 0);

	/* spin */
	while (1);
}

__C_LINKAGE void __NORETURN KTEXT_ENTRY
crash_domain(int status)
{
	printf("I am here unexpetedly -> %s:%s:%d\n", __func__, __FILE__,
	       __LINE__);
	ABORT();
	while (1)
		/* spin */
		;
}

/*
 * The entry point for C code (via start.S via firmware)
 *
 *  Note: (1) We're still running on the Firmware's 1KB stack, be careful.
 *        (2) Firmware owns interrupt vectors (IVPR) until we're ready to handle interrupts.
 */
__C_LINKAGE void __NORETURN
_enter_domain(Firmware_Interface_t * fw_interface)
{
	int i;
	//int threadID       = ProcessorID(); // 0..67
	int processorIndex = ProcessorCoreID();	// 0..16
	int threadIndex = ProcessorThreadID();	// 0..3

	OPD_Entry_t *opd_main = 0;

	if ((fw_interface->deprecated.personalityPtr->Kernel_Config.NodeConfig & PERS_ENABLE_TakeCPU) == 0) {
		opd_main = (OPD_Entry_t *) traditionalMain_domain;
	} else {
		opd_main = (OPD_Entry_t *) takeCpuMain_domain;
	}

	// Nominate a domain leader (first one in wins!).  In takeCPU mode, there is really
	// no race here ... but in traditional (thundering herd) mode, all threads may arrive
	// here at around the same time.

	testAndSet(&_leader_domain, (int64_t) - 1, ProcessorID());

	FIRMWARE() = fw_interface;

	/* init spin table */
	if (_leader_domain == ProcessorID()) {
		for (i=0; i < 17; ++i) {
			spin_table_array[i][0] = -1;
			spin_table_array[i][1] = -1;
			spin_table_array[i][2] = -1;
			spin_table_array[i][3] = -1;
		}
	}

	do {
		OPD_Entry_t *opd_exit = (OPD_Entry_t *) crash_domain;

#ifdef ROQ_RESILIENCY
		// Insert check values to detect stack overflow
		for (i = 0; i < 64; i++)										
			_stack_domain[ProcessorCoreID()-FIRST_CORE][ProcessorThreadID()][STACK_SIZE-i] = i;

		size_t pStk = (size_t)((size_t)&(_stack_domain[processorIndex-FIRST_CORE][threadIndex][STACK_SIZE - 128]));
#else
		size_t pStk = (size_t)((size_t)&(_stack_domain[processorIndex-FIRST_CORE][threadIndex][STACK_SIZE - 64]));
#endif
		mtspr(SPRN_SRR0_IP, opd_main->Entry_Addr);
		mtspr(SPRN_SRR1_MSR, MSR_KERNEL);
		isync();
		asm volatile ("mtlr  %2;"
			      "li    0,0;"
			      "mr    1,%0;"
			      "mr    2,%1;"
			      "li    3,%3;"
			      "rfi;"
			      "nop;"::"r" (pStk),
			      "r"(opd_main->TOC_Addr),
			      "r"(opd_exit->Entry_Addr), "i"(0)
			      :"r0", "r1", "r2", "r3", "lr", "cc", "memory");
	} while (1);
}

__C_LINKAGE uint64_t
domain_physddr_start(void)
{
	return DD().ddrOrigin;
}
