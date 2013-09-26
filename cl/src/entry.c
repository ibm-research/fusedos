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

NodeState_t NodeState ALIGN_L2_CACHE;

#if !(defined __FUSEDOS__)                                                                                            // FUSEDOS
extern char LC_Trampoline[], LC_Trampoline_End[];

//
// Enter here from Kernel_Start in start.S which was launched by the Firmware.
//
//  Note: (1) We're still running on the Firmware's 1KB stack, be careful.
//        (2) Firmware owns interrupt vectors (IVPR) until we're ready to handle interrupts.
//
__C_LINKAGE void __NORETURN Kernel_EntryPrimary( Firmware_Interface_t *fw_interface )
{

    // Setup the kernel stack
    uint64_t pStk =
	(uint64_t) &NodeState.CoreState[ProcessorCoreID()].
			HWThreads[ProcessorThreadID()].
			StandardStack[ KSTACK_SIZE - STACK_FRAME_SIZE ];
    
    asm volatile (
                  		  "mr    1,%0;"
                                        :
                                        : "r" (pStk)
                                        : "r1", "memory" 
                  );

    int i;    
    unsigned threadmask;    
    fw_interface->getDomainDescriptor(&NodeState.DomainDesc);

    uint32_t numHwThreads = BeDRAM_Read(BeDRAM_LOCKNUM_PRESENT);
    uint32_t numCores = numHwThreads / CONFIG_HWTHREADS_PER_CORE;
    NodeState.DomainDesc.coreMask &= (1 << numCores) - 1;
    
    // Do per-CNK s/w init and per-Node h/w init.
    ThreadPriority_High();

    // get local copy of the firmware interface
    memcpy_Inline( &(NodeState.FW_Interface), fw_interface, sizeof(Firmware_Interface_t) );
    
    // get local copy of the Personality
    fw_interface->getPersonality( &(NodeState.Personality), sizeof(Personality_t) );
    
#define VERIFY_PERSONALITY(a) ((NodeState.Personality.Kernel_Config.NodeConfig & (a)) == (a))
    
    if(! VERIFY_PERSONALITY(  PERS_ENABLE_TakeCPU | 
                              PERS_ENABLE_MMU | 
                              PERS_ENABLE_FPU | 
                              PERS_ENABLE_BIC
           ))
    {
        RASBEGIN(1);
        RASPUSH(NodeState.Personality.Kernel_Config.NodeConfig);
        RASFINAL(RAS_INVALIDPERSONALITY);
        Kernel_EarlyCrash(RAS_INVALIDPERSONALITY);
    }
    
    registerFlightRecorder();
    
    // Setup some base core config info within the NodeState object
    NodeState.NumCoresEnabled = popcnt64(NodeState.DomainDesc.coreMask);
    
    // The highest core index is reserved for CNK. 
    int reservedCoreID = 31 - cntlz32(NodeState.DomainDesc.coreMask); 
    NodeState.FirstAppAgentProcessorID = reservedCoreID * 4;
    NodeState.NodeLeaderProcessorID = (reservedCoreID*4)+2; // CNK thread for job control and tool management
    NodeState.NetworkManagerProcessorID = (reservedCoreID*4)+3; // CNK thread for network communication

    // Generate a mask of enabled application threads. 
    for (i=0; i< reservedCoreID; i++)
    {
        if (NodeState.DomainDesc.coreMask & (1<<i))
        {
            NodeState.EnabledAppThreads |= _B4((i*4)+3,(uint64_t)0xF);
        }
    }

    ppc_msync();
    
    for(i=0; i<CONFIG_MAX_CORES; i++)
    {
        if(NodeState.DomainDesc.coreMask & (1<<i))
        {
            if(ProcessorCoreID() == i)
                threadmask = 0xe;
            else
                threadmask = 0xf;
            
            fw_interface->takeCPU(i, threadmask, Kernel_EntrySecondary, fw_interface);
        }
    }
    Kernel_EntrySecondary(fw_interface);
    
    while(1)
    {
    }
}

__C_LINKAGE void __NORETURN Kernel_EntrySecondary(void *fw_interface_void)
{    
    // Setup the kernel stack
    uint64_t pStk =
	(uint64_t) &NodeState.CoreState[ProcessorCoreID()].
			HWThreads[ProcessorThreadID()].
			StandardStack[ KSTACK_SIZE - STACK_FRAME_SIZE ];
    
    asm volatile (
                  		  "mr    1,%0;"
                                        :
                                        : "r" (pStk)
                                        : "r1", "memory" 
                  );


    Firmware_Interface_t* fw_interface = (Firmware_Interface_t*)fw_interface_void;
    unsigned CoreID = ProcessorCoreID();
    unsigned ThdID = ProcessorThreadID();
    
    // Call the special "first" cnk barrier interface and wait for all enabled cores/threads to arrive
    Kernel_Barrier_First(NodeState.NumCoresEnabled * CONFIG_HWTHREADS_PER_CORE);
    
    // Initialize the HWThreadState object and the Kthreads that it manages
    KThread_InitHwThread();
    
    // Barrier with other threads in the core. We can now we can use the normal barrier interfaces
    Kernel_Barrier(Barrier_HwthreadsInCore);	

    // Now we can print etc.
    TRACE( TRACE_Entry, ("(I) Kernel_Entry[%2d:%d]: Entry at 0x%08lx.\n", CoreID, ThdID, GetTimeBase()) );

    // Per-Core Initialization
    if ( 0 == ThdID )
    {
        uint64_t num_cores_enabled = NodeState.NumCoresEnabled;
        if ((popcnt64(num_cores_enabled-1)) != 1)
        {
            RASBEGIN(2);
            RASPUSH(NodeState.DomainDesc.coreMask);
            RASPUSH(num_cores_enabled);
            RASFINAL(RAS_INSUFFICIENTCORES);
            
            printf("(E) CNK requires numcores = 2,3,5,9, or 17\n"); 
            Kernel_EarlyCrash(RAS_INSUFFICIENTCORES); 
        }
        
        // VMM Init
       VMM_Init();
       

	// Install the trampoline vector into BeDRAM.
	if (fw_interface->installInterruptVector(
		LC_Trampoline, LC_Trampoline_End - LC_Trampoline) != FW_OK)
	{
	    //<! @todo This is fatal.
	}


        // synchronize and unlock the Core Init lock
        isync();
        ppc_msync();
    }
    // Barrier with other threads in the core. We can now we can use the normal barrier interfaces
    Kernel_Barrier(Barrier_HwthreadsInCore);

    // Become a KThread
    do
    {
        CoreState_t     *pCS  = &(NodeState.CoreState[ CoreID ]);
        HWThreadState_t *pHWT = &(pCS->HWThreads[ ThdID ]);
        KThread_t       *pKTh= pHWT->SchedSlot[ CONFIG_SCHED_KERNEL_SLOT_INDEX ]; 
        OPD_Entry_t     *opd_main = (OPD_Entry_t *)Kernel_Begin;
        OPD_Entry_t     *opd_exit = (OPD_Entry_t *)Kernel_Finish;

        // Set the current KThread for this hardware thread to be this Kernel thread
        pHWT->pCurrentThread = pKTh;

        // Setup the kernel stack
	uint64_t pStk =
	    (uint64_t) &NodeState.CoreState[ProcessorCoreID()].
			    HWThreads[ProcessorThreadID()].
			    StandardStack[ KSTACK_SIZE - STACK_FRAME_SIZE ];

        mtspr( SPRG_pHWThread, pHWT );
        mtspr( SPRN_SRR0_IP,   opd_main->Entry_Addr );
        mtspr( SPRN_SRR1_MSR,  MSR_CNK_DISABLED );
        isync();
        asm volatile ("mtlr  %2;"
                      "li    0,0;"
                      "mr    1,%0;"
                      "mr    2,%1;"
                      "li    3,0;"
                      "rfi;"
                      "nop;"
                      :
                      : "r" (pStk),
                      "r" (opd_main->TOC_Addr),
                      "r" (opd_exit->Entry_Addr)
                      : "r0", "r1", "r2", "r3", "lr", "cc", "memory" );
    }
    while (1);
}

#endif                                                                                                                 // FUSEDOS
