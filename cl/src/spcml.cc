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
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include "Kernel.h"
#include "fusedos_config.h"
#include "fusedos.h"

int spc_0 = 0;
void* my_memory_bot;
void* my_memory_top;
pthread_key_t spc_key;
void* spc_monitor;

int running_spcml = 1;
int debug = 0;

uint64_t __KERNEL_END = 0x00000000; //
uint64_t __KERNEL_TEXT_START = 0x0; //  0 MB

extern int elf_loadimage(AppState_t* appState, uint64_t physical_offset, uint64_t *poffset_adjust);
extern int App_SetupMemory(Personality_t *pPers);
extern int Process_SetupMemory();
extern void App_CreateMainThread(uint64_t app_start, int priv);

extern int init_core_numbers();

void usage()
{
    printf("usage: spcml [-d] -m <spc monitor> -n <number of spcs>\n");
    exit(-1);
}

int main(int argc, char* argv[])
{
    char spc_mon[32];
    int num_spcs = 0, c;

    while ((c = getopt(argc, argv, "dm:n:")) != -1) {
        switch (c) {
        case 'd':
            debug = 1;
            break;
        case 'm':
            strncpy(spc_mon, optarg, strlen(optarg));
            break;
        case 'n':
            num_spcs = atoi(optarg);
            break;
        default:
            printf("unknown parameter\n");
            usage();
        }
    }

    int fd = open("/fusedosfs/monitor", O_RDWR);
    if (fd == -1) {
        printf("spc monitor open failed, errno %d\n", errno);
        exit(-1);
    }
    spc_monitor = (char*)(mmap((void *)(SPC_MONITOR_VADDR), SPC_MONITOR_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0));
    if (spc_monitor == MAP_FAILED) {
        printf("spc monitor mmap failed, errno %d\n", errno);
        exit(-1);
    }
    if (debug) printf("spc_monitor %p\n", spc_monitor);

    fd = open("/fusedosfs/context", O_RDWR);
    if (fd == -1) {
        printf("context open failed, errno %d", errno);
        exit(-1);
    }
    spc_context_t* pc = (spc_context_t*)(mmap((void *)(SPC_CONTEXT_VADDR), SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0));
    if (pc == MAP_FAILED) {
        printf("spc context mmap failed, errno %d", errno);
        exit(-1);
    }
    if (debug) printf("pc %p\n", pc);

    if (num_spcs == 0) num_spcs = get_fusedos_config()->nr_spcs;
    if (num_spcs < 1 || num_spcs > get_fusedos_config()->nr_spcs) {
        printf("specified %d for number of SPCs, %d are available\n",
	    num_spcs, get_fusedos_config()->nr_spcs);
        usage();
    }

    memset(spc_mon, 0, 32);
    strncpy(spc_mon, "spcm", strlen("spcm"));

    NodeState.CoreState[0].HWThreads[0].SchedSlot[CONFIG_SCHED_SLOT_FIRST] = &(NodeState.KThreads[0]);
    AppState_t *appState = &(NodeState.AppState[0]);
    strncpy(appState->App_Args, spc_mon, strlen(spc_mon));
    appState->Active_Processes = 1;
    NodeState.pAppState[0] = &(NodeState.AppState[0]);
    NodeState.AppAgents = 0;
    NodeState.KThreads[0].pAppProc = &(NodeState.AppProcess[0]);
    NodeState.TraceConfig = 0;
    if (debug) NodeState.TraceConfig = TRACE_Jobctl|TRACE_VMM|TRACE_Process|TRACE_StaticMap;

    NodeState.AppProcess[0].ProcessLeader_KThread = &(NodeState.KThreads[0]);

    pthread_key_create(&spc_key, NULL);
    int i = 0;
    pthread_setspecific(spc_key, &i);

    my_memory_bot = (void*)(SPC_MONITOR_VADDR);
    my_memory_top = (void*)(SPC_CONTEXT_VADDR);

    // These should be physical memory addresses, but we're mapping the
    // spcmon from vaddr 0
    NodeState.DomainDesc.ddrOrigin = SPC_MONITOR_VADDR;
    NodeState.DomainDesc.ddrEnd = SPC_CONTEXT_VADDR;

    int rc = elf_loadimage(&(NodeState.AppState[0]), 0, 0);
    if (rc != 0) {
        printf("Bad return code from elf_loadimage, rc %d\n", rc);
        exit(-1);
    }
    rc = vmm_setupTLBMapper(__KERNEL_END >> 20);
    if (rc != 0) {
        printf("Bad return code from vmm_setupTLBMapper, rc %d\n", rc);
        exit(-1);
    }
    // numprocesses, numagents, core_index
    rc = vmm_resetStaticMap(1, 0, 0);
    if (rc != 0) {
        printf("Bad return code from vmm_resetStaticMap, rc %d\n", rc);
        exit(-1);
    }
    Personality_t pPers;
    rc = App_SetupMemory(&pPers);
    if (rc != 0) {
        printf("Bad return code from App_SetupMemory, rc %d\n", rc);
        exit(-1);
    }
    rc = Process_SetupMemory();
    if (rc != 0) {
        printf("Bad return code from Process_SetupMemory, rc %d\n", rc);
        exit(-1);
    }
    rc = vmm_installStaticTLBMap(GetMyProcess()->Tcoord);
    if (rc != 0) {
        printf("Bad return code from vmm_installStaticTLBMap, rc %d\n", rc);
        exit(-1);
    }
    App_CreateMainThread(GetMyProcess()->Proc_EntryPoint, 0);

    for (int i = 0; i < num_spcs; i++) {
        if (i > 0) memcpy(&(pc[i].regs), &(pc[0].regs), sizeof(regs_t));

        // Reset count because monitor_start_{1,2,3}() routines set up TLB
        // entry for monitor and context
        pc[i].tlb_entry_count = 0;

        // Cache stack pointer and toc
        pc[i].spcm_sp = (uint64_t)(&(pc[i].spcm_stack[SPCM_STACK_SIZE - STACK_FRAME_SIZE - 1]));
        pc[i].spcm_toc = pc[i].regs.gpr[2];
        if (debug) printf("spcm_sp 0x%lX, spcm_toc 0x%lX\n", pc[i].spcm_sp, pc[i].spcm_toc);

        char fn[32];
        memset(fn, 0, 32);
        sprintf(fn, "/fusedosfs/%d_memory", i);
        if (debug) printf("fn %s\n", fn);
        fd = open(fn, O_RDWR);
        if (fd == -1) {
            printf("%d_memory open failed, errno %d\n", i, errno);
            exit(-1);
        }
        ioctl(fd, SPC_IOCTL_INIT);
        // Thread 0 must come up first
        if ((i & (THREADS_PER_CORE - 1)) == 0) usleep(10000);
    }
    if (debug) printf("Done\n");
    return(0);
}
