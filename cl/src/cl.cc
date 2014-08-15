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
#include <asm/unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <time.h>
#include <linux/futex.h>
#include <sys/ucontext.h>
#include <sys/ptrace.h>

#include "hwi/include/bqc/BIC.h"
#include "hwi/include/bqc/BIC_inlines.h"

#include "hwi/include/bqc/PhysicalMap.h"

#include "Kernel.h"
#include "fusedos_config.h"
#include "fusedos.h"
#include "cl_cmdline.h"
#include "cl_debug.h"
#include "cl_coords.h"
#include "rank.h"
#include "cl_upc.h"

#include "personality_envvar_names.h"

int debug = 0;

#define SYSCALL_EXTERN(name) extern uint64_t sc_##name(SYSCALL_FCN_ARGS);
#define SYSCALL_CASE(name, number)                                          \
    case number: {                                                          \
        spc_context_t* pc = get_spc_context(this_spc());                    \
        CL_DEBUG("%s syscall, r3 0x%lx\n", #name, pc->regs.gpr[3]);         \
        ret = sc_##name(pc->regs.gpr[3], pc->regs.gpr[4], pc->regs.gpr[5],  \
                        pc->regs.gpr[6], pc->regs.gpr[7], pc->regs.gpr[8]); \
        CL_DEBUG("%s: %s syscall, ret 0x%lx / %ld\n", __func__, #name, ret, ret); \
        if (action_tracker) {                                               \
            pthread_mutex_lock( &NodeState.cl_thread_mutex );               \
            actions[number]++;                                              \
            pthread_mutex_unlock( &NodeState.cl_thread_mutex );             \
        }                                                                   \
        pc->regs.gpr[3] = ret;                                              \
    }

int spc_0 = -1;
int num_spcs = 0;
pthread_key_t spc_key;

int running_spcml = 0;

int exit_status = 0;

#define MAX_SYSCALL 1090
static int actions[MAX_SYSCALL];
static int action_tracker = 0;

extern int __bg_cpu_count;

extern int futex_counts[64][6];
extern int do_futex_counts;

extern pthread_mutex_t mapping_storageMutex;
extern pthread_mutex_t loadSequenceUpdateMutex;
extern pthread_mutex_t MMapRead_mutex;
extern pthread_mutex_t mappedFiles_mutex;

uint64_t __KERNEL_END = 0x01000000; // 16 MB
uint64_t __KERNEL_TEXT_START = 0x0; //  0 MB

void* mu_memory;
void* mu_priv_memory;

extern void App_CreateMainThread(uint64_t app_start, int priv);
extern int App_SetIULivelockConfig(void);
extern int App_SetupMemory(Personality_t* pPers);
extern int elf_loadimage(AppState_t* appState, uint64_t physical_offset, uint64_t *poffset_adjust);
extern "C" void File_InitFS(void);
extern "C" void File_TermFS(void);
extern int Process_SetupMemory();

extern int init_cl_shm_area();
extern void init_cl_mu_data();
extern int readPersonalityIntoEnvironment();

SYSCALL_EXTERN(exit);                        //    1
SYSCALL_EXTERN(read);                        //    3
SYSCALL_EXTERN(write);                       //    4
SYSCALL_EXTERN(open);                        //    5
SYSCALL_EXTERN(close);                       //    6
SYSCALL_EXTERN(creat);                       //    8
SYSCALL_EXTERN(link);                        //    9
SYSCALL_EXTERN(unlink);                      //   10
SYSCALL_EXTERN(chdir);                       //   12
SYSCALL_EXTERN(time);                        //   13
SYSCALL_EXTERN(lseek);                       //   19
SYSCALL_EXTERN(getpid);                      //   20
SYSCALL_EXTERN(setuid);                      //   23
SYSCALL_EXTERN(getuid);                      //   24
SYSCALL_EXTERN(alarm);                       //   27
SYSCALL_EXTERN(sync);                        //   36
SYSCALL_EXTERN(mkdir);                       //   39
SYSCALL_EXTERN(rmdir);                       //   40
SYSCALL_EXTERN(dup);                         //   41
SYSCALL_EXTERN(times);                       //   43
SYSCALL_EXTERN(brk);                         //   45
SYSCALL_EXTERN(setgid);                      //   46
SYSCALL_EXTERN(getgid);                      //   47
SYSCALL_EXTERN(geteuid);                     //   49
SYSCALL_EXTERN(ioctl);                       //   54
SYSCALL_EXTERN(fcntl);                       //   55
SYSCALL_EXTERN(setrlimit);                   //   75
SYSCALL_EXTERN(getrusage);                   //   77
SYSCALL_EXTERN(gettimeofday);                //   78
SYSCALL_EXTERN(setgroups);                   //   81
SYSCALL_EXTERN(symlink);                     //   83
SYSCALL_EXTERN(readlink);                    //   85
SYSCALL_EXTERN(mmap);                        //   90
SYSCALL_EXTERN(munmap);                      //   91
SYSCALL_EXTERN(ftruncate);                   //   93
SYSCALL_EXTERN(statfs);                      //   99
SYSCALL_EXTERN(socketcall);                  //  102
SYSCALL_EXTERN(stat);                        //  106
SYSCALL_EXTERN(fstat);                       //  108
SYSCALL_EXTERN(fsync);                       //  118
SYSCALL_EXTERN(clone);                       //  120
SYSCALL_EXTERN(uname);                       //  122
SYSCALL_EXTERN(mprotect);                    //  125
SYSCALL_EXTERN(writev);                      //  146
SYSCALL_EXTERN(sched_yield);                 //  158
SYSCALL_EXTERN(nansleep);                    //  162
SYSCALL_EXTERN(mremap);                      //  163
SYSCALL_EXTERN(poll);                        //  167
SYSCALL_EXTERN(rt_sigaction);                //  173
SYSCALL_EXTERN(rt_sigprocmask);              //  174
SYSCALL_EXTERN(getcwd);                      //  182
SYSCALL_EXTERN(ugetrlimit);                  //  190
SYSCALL_EXTERN(madvise);                     //  205
SYSCALL_EXTERN(gettid);                      //  207
SYSCALL_EXTERN(futex);                       //  221
SYSCALL_EXTERN(sched_setaffinity);           //  222
SYSCALL_EXTERN(sched_getaffinity);           //  223
SYSCALL_EXTERN(set_tid_address);             //  232
SYSCALL_EXTERN(exit_group);                  //  234
SYSCALL_EXTERN(tgkill);                      //  250
SYSCALL_EXTERN(GETMEMORYREGION);             // 1024
SYSCALL_EXTERN(GET_PERSONALITY);             // 1025
SYSCALL_EXTERN(ALLOCATE_INJ_FIFOS);          // 1032
SYSCALL_EXTERN(QUERY_INJ_FIFOS);             // 1033
SYSCALL_EXTERN(QUERY_REC_FIFOS);             // 1034
SYSCALL_EXTERN(QUERY_BASE_ADDRESS_TABLE);    // 1035
SYSCALL_EXTERN(ALLOCATE_REC_FIFOS);          // 1036
SYSCALL_EXTERN(ALLOCATE_BASE_ADDRESS_TABLE); // 1037
SYSCALL_EXTERN(REC_FIFO_ENABLE);             // 1038
SYSCALL_EXTERN(CFG_INJ_FIFO_INTS);           // 1039
SYSCALL_EXTERN(CFG_REC_FIFO_INTS);           // 1040
SYSCALL_EXTERN(CFG_INJ_FIFO_THRESH);         // 1041
SYSCALL_EXTERN(CFG_REC_FIFO_THRESH);         // 1042
SYSCALL_EXTERN(DEALLOC_INJ_FIFOS);           // 1043
SYSCALL_EXTERN(DEALLOC_REC_FIFOS);           // 1044
SYSCALL_EXTERN(DEALLOC_BASE_ADDR_TABLE);     // 1045
SYSCALL_EXTERN(RANKS2COORDS);                // 1055
SYSCALL_EXTERN(JOBCOORDS);                   // 1056
SYSCALL_EXTERN(UPCI_OP);                     // 1057
SYSCALL_EXTERN(ALLOCATEL2ATOMIC);            // 1059
SYSCALL_EXTERN(QUERYCOLLECTIVECLASSROUTE);   // 1065
SYSCALL_EXTERN(ALLOCATECOLLECTIVECLASSROUTE);// 1066
SYSCALL_EXTERN(SETCOLLECTIVECLASSROUTE);     // 1067
SYSCALL_EXTERN(DEALLOCATECOLLECTIVECLASSROUTE); // 1068
SYSCALL_EXTERN(QUERYGINTCLASSROUTE);         // 1069
SYSCALL_EXTERN(ALLOCATEGINTCLASSROUTE);      // 1070
SYSCALL_EXTERN(SETGINTCLASSROUTE);           // 1071
SYSCALL_EXTERN(DEALLOCATEGINTCLASSROUTE);    // 1072
SYSCALL_EXTERN(GETRANK);                     // 1076
SYSCALL_EXTERN(GETPVR);                      // 1089

static void pretty_print_TLB(tlb_entry_t* tlbs, int count);

void clear_context(spc_context_t* pc)
{
    memset((void*)(&(pc->regs)), 0, sizeof(regs_t));

    pc->ipi_wakeup = 0;

    pc->start = 0;
    pc->command = 0;

    memset((void*)(&(pc->tlb_entry)), 0, sizeof(tlb_entry_t) * MAX_TLB_ENTRIES);
    pc->tlb_entry_count = 0;
    pc->tlb_entry_install = 0;

    pc->text_pstart = 0;
    pc->text_pend = 0;
    pc->data_pstart = 0;
    pc->data_pend = 0;
    pc->heap_pstart = 0;
    pc->heap_pend = 0;
}

//#define DUMP_FP_CONTEXT

static inline void dump_context(spc_context_t* pc) {
    int i;
    regs_t* r = &(pc->regs);
    printf("id %lu, start %lu, command %lu, ppr32 0x%lx, ipi_wakeup %lu, ex_code %lu\n",
           pc->id, pc->start, pc->command, pc->ppr32, pc->ipi_wakeup, pc->ex_code);
    printf("mem_bot %016lx, tlb_entry_count %lu, tlb_entry_install %lu\n",
           pc->mem_bot, pc->tlb_entry_count, pc->tlb_entry_install);
    printf("spcm_sp %016lx, spcm_toc %016lx\n", pc->spcm_sp, pc->spcm_toc);
    printf("ipi fcn %016lx, parm1 %016lx, parm2 %016lx\n", pc->ipi_message.fcn, pc->ipi_message.parm1, pc->ipi_message.parm2);
    printf("bic_int_send %016lx, bic_value %016lx\n", (uint64_t)(pc->bic_int_send), pc->bic_value);
    printf("  gpr0 %016lx    gpr1 %016lx    gpr2 %016lx    gpr3 %016lx\n",   r->gpr[0],   r->gpr[1],   r->gpr[2],   r->gpr[3]);
    printf("  gpr4 %016lx    gpr5 %016lx    gpr6 %016lx    gpr7 %016lx\n",   r->gpr[4],   r->gpr[5],   r->gpr[6],   r->gpr[7]);
    printf("  gpr8 %016lx    gpr9 %016lx   gpr10 %016lx   gpr11 %016lx\n" ,  r->gpr[8],   r->gpr[9],  r->gpr[10],  r->gpr[11]);
    printf(" gpr12 %016lx   gpr13 %016lx   gpr14 %016lx   gpr15 %016lx\n",  r->gpr[12],  r->gpr[13],  r->gpr[14],  r->gpr[15]);
    printf(" gpr16 %016lx   gpr17 %016lx   gpr18 %016lx   gpr19 %016lx\n",  r->gpr[16],  r->gpr[17],  r->gpr[18],  r->gpr[19]);
    printf(" gpr20 %016lx   gpr21 %016lx   gpr22 %016lx   gpr23 %016lx\n",  r->gpr[20],  r->gpr[21],  r->gpr[22],  r->gpr[23]);
    printf(" gpr24 %016lx   gpr25 %016lx   gpr26 %016lx   gpr27 %016lx\n",  r->gpr[24],  r->gpr[25],  r->gpr[26],  r->gpr[27]);
    printf(" gpr28 %016lx   gpr29 %016lx   gpr30 %016lx   gpr31 %016lx\n",  r->gpr[28],  r->gpr[29],  r->gpr[30],  r->gpr[31]);
    printf(" sprg0 %016lx   sprg1 %016lx   sprg2 %016lx   sprg3 %016lx\n",  r->sprg[0],  r->sprg[1],  r->sprg[2],  r->sprg[3]);
    printf(" sprg4 %016lx   sprg5 %016lx   sprg6 %016lx   sprg7 %016lx\n",  r->sprg[4],  r->sprg[5],  r->sprg[6],  r->sprg[7]);
    printf(" sprg8 %016lx   sprg9 %016lx                              \n",  r->sprg[8],  r->sprg[9]                          );
    printf("gsprg0 %016lx  gsprg1 %016lx  gsprg2 %016lx  gsprg3 %016lx\n", r->gsprg[0], r->gsprg[1], r->gsprg[2], r->gsprg[3]);
    printf(" fpscr %016lx      ip %016lx     msr %016lx      cr %016lx\n",    r->fpscr,       r->ip,      r->msr,       r->cr);
    printf("    lr %016lx     xer %016lx     ctr %016lx     esr %016lx\n",       r->lr,      r->xer,      r->ctr,      r->esr);
    printf("  dear %016lx     pid %016lx    dbsr %016lx    mcsr %016lx\n",     r->dear,      r->pid,     r->dbsr,     r->mcsr);
    printf(" dbcr0 %016lx   dbcr1 %016lx   dbcr2 %016lx   dbcr3 %016lx\n",    r->dbcr0,    r->dbcr1,    r->dbcr2,    r->dbcr3);
    printf("  dac1 %016lx    dac2 %016lx    dac3 %016lx    dac4 %016lx\n",     r->dac1,     r->dac2,     r->dac3,     r->dac4);
    printf("  iac1 %016lx    iac2 %016lx    iac3 %016lx    iac4 %016lx\n",     r->iac1,     r->iac2,     r->iac3,     r->iac4);
#ifdef DUMP_FP_CONTEXT
    for(i=0; i< NUM_QVRS; i++)
        printf("  FPR%02d 0x%016lx %016lx %016lx %016lx\n", i, r->gvr[i].ll[0], r->gvr[i].ll[1], r->gvr[i].ll[2], r->gvr[i].ll[3]);
#else
    (void)i;
#endif
}

#ifdef __cplusplus
static inline void dump_context() {
    dump_context(get_spc_context(this_spc()));
}
#endif

void dump_actions()
{
    int count;
    CL_ERROR("total actions=%d\n", actions[0]);

    for(count = 0; count < MAX_SYSCALL; count++) {
        if (actions[count] > 0) CL_ERROR("   action[%d] = %d\n", count, actions[count]);
    }
}

extern "C"  int cl_SaveFPUState()
{
    spc_context_t* pc = get_spc_context(this_spc());
    pc->command = SPC_SAVE_FPU;
    CL_DEBUG("calling PM to save FPU state on SPC %d\n", this_spc());
    ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
    CL_DEBUG("returned from PM saving FPU state on SPC %d, ex_code %lu\n", this_spc(), pc->ex_code);
    return pc->ex_code;
}

static enum { LINEAR, CORES, TWO_THREADS } allocation_policy;

static char* str_alloc_policy()
{
    switch(allocation_policy) {
    case LINEAR:
        return (char*)"LINEAR";
    case CORES:
        return (char*)"CORES";
    case TWO_THREADS:
        return (char*)"TWO_THREADS";
    default:
        return (char*)"";
    }
}

extern "C" KThread_t* cl_GetAvailableKThread()
{
    KThread_t* found = NULL;
    int offset = 0;
    int i, j;

    for(i = NodeState.cl_firstspc, j = 0; j < NodeState.cl_numspcs; j++) {
        CL_DEBUG("considering SPC %d\n", i);
        if (NodeState.KThreads[i].cl_thread_state == IDLE) {
            found = &NodeState.KThreads[i];
            break;
        }

        switch(allocation_policy) {
        case LINEAR:
            i++;
            break;
        case CORES:
            i += 4;
            break;
        case TWO_THREADS:
            i += 2;
            break;
        }

        // reached last allocated SPC?
        if (i >= (NodeState.cl_firstspc + NodeState.cl_numspcs)) {
            // flip back to beginning
            offset++;
            i = NodeState.cl_firstspc + offset;
        }
    }

    return found;
}

static inline void stopSPC(spc_context_t* pc)
{
    CL_DEBUG("Stopping SPC %lu\n", pc->id);
    pc->command = SPC_EXIT;
    ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
}

void indicate_exit_to_threads()
{
    CL_DEBUG("in indicate_exit_to_threads for cl thread %d ... (acquiring mutex)\n", this_spc() );
    int rc;
    rc = pthread_mutex_lock( &NodeState.cl_thread_mutex );
    if (rc) {
        CL_ERROR("error acquiring cl thread mutex for KThread allocation %d, %s\n",
                 rc, strerror(rc));
        return;
    }

    CL_DEBUG("cl thread %d acquired cl mutex for exit, setting states to EXIT\n", this_spc());

    for (int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i++) {
        NodeState.KThreads[i].cl_thread_state = EXIT;
    }

    // wakeup cl threads in IDLE state
    pthread_cond_broadcast(& NodeState.cl_thread_cond);

    rc = pthread_mutex_unlock(&NodeState.cl_thread_mutex);
    if (rc) {
        CL_ERROR("error releasing cl thread mutex for exit %d, %s\n",
                 rc, strerror(rc));
        return;
    }

    for (int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i++) {
        spc_context_t* pc = get_spc_context(i);
        CL_DEBUG("sending IPI to SPC %d\n", i);
        ioctl(pc->fusedosfs_fd, SPC_IOCTL_IPI_QUIT, SPC_TO_CPU(i));
    }

    // send signal SIGUSR1 to all threads to abort blocking Linux syscalls
    // (common scenario: OpenMP threads blocking wait for a Futex while
    // the main thread calls exit).
    for (int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i++) {
        if (!pthread_equal(NodeState.KThreads[i].cl_thread, pthread_self())) {
            pthread_kill( NodeState.KThreads[i].cl_thread, SIGUSR1 );
        }
    }
}

static void stop_SPC_group()
{
    CL_DEBUG("stopping SPCs %d to %d\n", NodeState.cl_firstspc, NodeState.cl_firstspc + NodeState.cl_numspcs);
    for(int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i++) {
        spc_context_t* pc = get_spc_context(i);
        stopSPC(pc);
    }
    CL_DEBUG("stopping SPCs completed\n");
}

void unload_tlbs() {
    for (int i = spc_0; i < spc_0 + num_spcs; i += 4) {
        spc_context_t* pc = get_spc_context(i);
        CL_DEBUG("Unloading TLB in SPC %d, tlb_entry_install %lu, tlb_entry_count %lu\n",
                 i, pc->tlb_entry_install, pc->tlb_entry_count);
        pc->command = SPC_UNLOAD_TLB;
        ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
    }
}

// returns whether we were the last one
static int turn_clthread_idle(cl_thread_state_t* thread_state)
{
    int last_thread = 1;
    pthread_mutex_lock( &NodeState.cl_thread_mutex );
    if (*thread_state != EXIT) {
        *thread_state = IDLE;

        // if we just turned ourself IDLE, we might have been the last one
        // alive (and now dead) -> return 1 so that the caller can exit

        for(int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i++) {
            if(NodeState.KThreads[i].cl_thread_state == USE_SPC) {
                last_thread = 0;
                break;
            }
        }
    }

    pthread_mutex_unlock( &NodeState.cl_thread_mutex );

    return last_thread;
}

const char* ex_string[28] = {
    "NONE",	//  0
    "DEBUG",
    "DSI",
    "ISI",
    "ALGN",
    "PROG",	//  5
    "FPU",
    "APU",
    "DTLB",
    "ITLB",
    "VECT",	// 10
    "UNDEF",
    "PDBI",
    "PDBCI",
    "GDBI",
    "GDBCI",	// 15
    "EHVSC",
    "EHVPRIV",
    "LRATE",
    "MCHK",
    "CI",	// 20
    "EI",
    "SC",
    "DEC",
    "FIT",
    "WDT",	// 25
    "UDEC",
    "PERFMON",
};

void kthread(void* p)
{
    int rc;

    int spc = (long)p;

    pthread_setspecific(spc_key, &spc);
    spc_context_t* pc = get_spc_context(spc);

    NodeState.KThreads[spc].in_spc_ioctl = 0;

    while (1) {
        // check/wait loop until thread state is not IDLE
        rc = pthread_mutex_lock( &NodeState.cl_thread_mutex );
        if (rc) {
            CL_ERROR("error acquiring cl thread mutex, %d, %s\n", rc, strerror(rc));
            return;
        }

        while (NodeState.KThreads[spc].cl_thread_state == IDLE) {
            //CL_DEBUG("cl thread %d is IDLE, waiting on cl thread cond variable\n", spc);
            pthread_cond_wait(&NodeState.cl_thread_cond, &NodeState.cl_thread_mutex);
        }

        rc = pthread_mutex_unlock(&NodeState.cl_thread_mutex);
        if (rc) {
            CL_ERROR("error releasing cl thread mutex, %d, %s\n", rc, strerror(rc));
            return;
        }

        if (NodeState.KThreads[spc].cl_thread_state == EXIT) goto done;

        CL_DEBUG("cl thread %d starting SPC\n", spc);
        pc->command = SPC_START;
        ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
        while (NodeState.KThreads[spc].cl_thread_state == USE_SPC) {
            uint64_t ex_code = pc->ex_code;
            uint64_t syscall = pc->regs.gpr[0];
            uint64_t ret;

            if (action_tracker) {
                pthread_mutex_lock( &NodeState.cl_thread_mutex );
                actions[0]++;
                pthread_mutex_unlock( &NodeState.cl_thread_mutex );
            }

            if (ex_code) {
                // Just bail for now...
		if(ex_code > 0 && ex_code < 28) {
		    CL_ERROR("%s exception, dumping context\n", ex_string[ex_code]);
		} else {
		    CL_ERROR("Unknown exception %lu, dumping context\n", ex_code);
		}
                //CL_ERROR("<enter> to exit\n");
                //char buf[16];
                //if (read(0, buf, 1) == -1) CL_ERROR("read failed");
                dump_context();
				//CL_DEBUG("foo --- endless loop --- do a TLB dump ...\n");
				//while(1);

                indicate_exit_to_threads();
                stop_SPC_group();
                break;
            }
            CL_DEBUG("syscall at IP 0x%lx\n", pc->regs.ip);
            switch (syscall) {
            SYSCALL_CASE(exit, __NR_exit);                       //    1
                if (turn_clthread_idle(&NodeState.KThreads[spc].cl_thread_state)) {
                    // we were the last active thread, so exit now 
                    indicate_exit_to_threads();
                    stop_SPC_group();
                }
                break;
            SYSCALL_CASE(read, __NR_read);                       //    3
                break;
            SYSCALL_CASE(write, __NR_write);                     //    4
                break;
            SYSCALL_CASE(open, __NR_open);                       //    5
                break;
            SYSCALL_CASE(close, __NR_close);                     //    6
                break;
            SYSCALL_CASE(creat, __NR_creat);                     //    8
                break;
            SYSCALL_CASE(link, __NR_link);                       //    9
                break;
            SYSCALL_CASE(unlink, __NR_unlink);                   //   10
                break;
            SYSCALL_CASE(chdir, __NR_chdir);                     //   12
                break;
            SYSCALL_CASE(time, __NR_time);                       //   13
                break;
            SYSCALL_CASE(lseek, __NR_lseek);                     //   19
                break;
            SYSCALL_CASE(getpid, __NR_getpid);                   //   20
                break;
            SYSCALL_CASE(setuid, __NR_setuid);                   //   23
                break;
            SYSCALL_CASE(getuid, __NR_getuid);                   //   24
                break;
            SYSCALL_CASE(alarm, __NR_alarm);                     //   27
                break;
            SYSCALL_CASE(sync, __NR_sync);                       //   36
                break;
            SYSCALL_CASE(mkdir, __NR_mkdir);                     //   39
                break;
            SYSCALL_CASE(rmdir, __NR_rmdir);                     //   40
                break;
            SYSCALL_CASE(dup, __NR_dup);                         //   41
                break;
            SYSCALL_CASE(times, __NR_times);                     //   43
                break;
            SYSCALL_CASE(brk, __NR_brk);                         //   45
                break;
            SYSCALL_CASE(setgid, __NR_setgid);                   //   46
                break;
            SYSCALL_CASE(getgid, __NR_getgid);                   //   47
                break;
            SYSCALL_CASE(geteuid, __NR_geteuid);                 //   49
                break;
            SYSCALL_CASE(ioctl, __NR_ioctl);                     //   54
                break;
            SYSCALL_CASE(fcntl, __NR_fcntl);                     //   55
                break;
            SYSCALL_CASE(setrlimit, __NR_setrlimit);             //   75
                break;
            SYSCALL_CASE(getrusage, __NR_getrusage);             //   77
                break;
            SYSCALL_CASE(gettimeofday, __NR_gettimeofday);       //   78
                break;
            SYSCALL_CASE(setgroups, __NR_setgroups);             //   81
                break;
            SYSCALL_CASE(symlink, __NR_symlink);                 //   83
                break;
            SYSCALL_CASE(readlink, __NR_readlink);               //   85
                break;
            SYSCALL_CASE(mmap, __NR_mmap);                       //   90
                break;
            SYSCALL_CASE(munmap, __NR_munmap);                   //   91
                break;
            SYSCALL_CASE(ftruncate, __NR_ftruncate);             //   93
                break;
            SYSCALL_CASE(statfs, __NR_statfs);                   //   99
                break;
            SYSCALL_CASE(socketcall, __NR_socketcall);           //  102
                break;
            SYSCALL_CASE(stat, __NR_stat);                       //  106
                break;
            SYSCALL_CASE(fstat, __NR_fstat);                     //  108
                break;
            SYSCALL_CASE(fsync, __NR_fsync);                     //  118
                break;
            SYSCALL_CASE(clone, __NR_clone);                     //  120
                break;
            SYSCALL_CASE(uname, __NR_uname);                     //  122
                break;
            SYSCALL_CASE(mprotect, __NR_mprotect);               //  125
                break;
            SYSCALL_CASE(writev, __NR_writev);                   //  146
                break;
            SYSCALL_CASE(sched_yield, __NR_sched_yield);         //  158
                break;
            SYSCALL_CASE(nanosleep, __NR_nanosleep);             //  162
                break;
            SYSCALL_CASE(mremap, __NR_mremap);                   //  163
                break;
            SYSCALL_CASE(poll, __NR_poll);                       //  167
                break;
            SYSCALL_CASE(rt_sigaction, __NR_rt_sigaction);       //  173
                break;
            SYSCALL_CASE(rt_sigprocmask, __NR_rt_sigprocmask);   //  174
                break;
            SYSCALL_CASE(getcwd, __NR_getcwd);                   //  182
                break;
            SYSCALL_CASE(ugetrlimit, __NR_ugetrlimit);           //  190
                break;
            SYSCALL_CASE(madvise, __NR_madvise);                 //  205
                break;
            SYSCALL_CASE(gettid, __NR_gettid);                   //  207
                break;
            SYSCALL_CASE(futex, __NR_futex);                     //  221
                break;
            SYSCALL_CASE(sched_setaffinity, __NR_sched_setaffinity); // 222
                break;
            SYSCALL_CASE(sched_getaffinity, __NR_sched_getaffinity); // 223
                break;
            SYSCALL_CASE(set_tid_address, __NR_set_tid_address); //  232
                break;
            SYSCALL_CASE(exit_group, __NR_exit_group);           //  234
                exit_status = pc->regs.gpr[3];
                CL_DEBUG("exit_group syscall, exit_status %d\n", exit_status);
                indicate_exit_to_threads();
                stop_SPC_group();
                goto done;
            SYSCALL_CASE(tgkill, __NR_tgkill); // 250
                if( pc->regs.gpr[3] == 0 ) // currently, a successful tgkill means a SIGABRT to myself -> exit thread
                    if(turn_clthread_idle( &NodeState.KThreads[spc].cl_thread_state)) {
                        // we were the last active thread, so exit now 
                        indicate_exit_to_threads();
                        stop_SPC_group();
                    }
                break;
            case 300: // get_robust_list
                CL_DEBUG("get_robust_list syscall, returning ENOSYS\n");
                pc->regs.gpr[3] = ENOSYS;
                break;
            SYSCALL_CASE(GETMEMORYREGION, 1024);                 // 1024
                break;
            SYSCALL_CASE(GET_PERSONALITY, 1025);                 // 1025
                break;
            SYSCALL_CASE(ALLOCATE_INJ_FIFOS, 1032);              // 1032
                break;
            SYSCALL_CASE(QUERY_INJ_FIFOS, 1033);                 // 1033
                break;
            SYSCALL_CASE(QUERY_REC_FIFOS, 1034);                 // 1034
                break;
            SYSCALL_CASE(QUERY_BASE_ADDRESS_TABLE, 1035);        // 1035
                break;
            SYSCALL_CASE(ALLOCATE_REC_FIFOS, 1036);              // 1036
                break;
            SYSCALL_CASE(ALLOCATE_BASE_ADDRESS_TABLE, 1037);     // 1037
                break;
            SYSCALL_CASE(REC_FIFO_ENABLE, 1038);                 // 1038
                 break;
            SYSCALL_CASE(CFG_INJ_FIFO_INTS, 1039);               // 1039
                 break;
            SYSCALL_CASE(CFG_REC_FIFO_INTS, 1040);               // 1040
                 break;
            SYSCALL_CASE(CFG_INJ_FIFO_THRESH, 1041);             // 1041
                break;
            SYSCALL_CASE(CFG_REC_FIFO_THRESH, 1042);             // 1042
                break;
            SYSCALL_CASE(DEALLOC_INJ_FIFOS, 1043);               // 1043
                break;
            SYSCALL_CASE(DEALLOC_REC_FIFOS, 1044);               // 1044
                break;
            SYSCALL_CASE(DEALLOC_BASE_ADDR_TABLE, 1045);         // 1045
                break;
            SYSCALL_CASE(RANKS2COORDS, 1055);                    // 1055
                break;
            SYSCALL_CASE(JOBCOORDS, 1056);                       // 1056
                break;
            SYSCALL_CASE(UPCI_OP, 1057);                         // 1057
                break;
            SYSCALL_CASE(ALLOCATEL2ATOMIC, 1059); // 1059
                CL_DEBUG("ALLOCATEL2ATOMIC, tlb_entry_install %lu, tlb_entry_count %lu\n",
                         pc->tlb_entry_install, pc->tlb_entry_count);
                pretty_print_TLB(pc->tlb_entry, pc->tlb_entry_count);
                // Install these entries also in the TLBs of the other
                // involved cores. Note that we assume cl_firstspc is
                // hardware thread 0.
                for (int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i += 4) {
                    if (i == (int)(pc->id)) {
                        CL_DEBUG("ALLOCATEL2ATOMIC, skipping %d\n", i);
                        continue;
                    }
                    CL_DEBUG("ALLOCATEL2ATOMIC, adding entries from %lu to %d\n", pc->id, i);
                    spc_context_t* pc_tmp = get_spc_context(i);
                    for (unsigned int j = pc->tlb_entry_install; j < pc->tlb_entry_count; j++) {
                        pc_tmp->tlb_entry[j].slot = pc->tlb_entry[j].slot;
                        pc_tmp->tlb_entry[j].mas1 = pc->tlb_entry[j].mas1;
                        pc_tmp->tlb_entry[j].mas2 = pc->tlb_entry[j].mas2;
                        pc_tmp->tlb_entry[j].mas7_3 = pc->tlb_entry[j].mas7_3;
                        pc_tmp->tlb_entry[j].mas8 = pc->tlb_entry[j].mas8;
                        pc_tmp->tlb_entry[j].mmucr3 = pc->tlb_entry[j].mmucr3;
                        pc_tmp->tlb_entry_count++;
                    }
                }
                for (int i = NodeState.cl_firstspc; i < NodeState.cl_firstspc + NodeState.cl_numspcs; i += 4) {
                    spc_context_t* pc_tmp = get_spc_context(i);
                    CL_DEBUG("ALLOCATEL2ATOMIC, %d, tlb_entry_install %lu, tlb_entry_count %lu\n",
                             i, pc_tmp->tlb_entry_install, pc_tmp->tlb_entry_count);
                    pretty_print_TLB(pc_tmp->tlb_entry, pc_tmp->tlb_entry_count);
                    pc_tmp->command = SPC_LOAD_TLB;
                    ioctl(pc_tmp->fusedosfs_fd, SPC_IOCTL_COMMAND);
                }
                break;
            SYSCALL_CASE(QUERYCOLLECTIVECLASSROUTE, 1065);      // 1065
                break;
            SYSCALL_CASE(ALLOCATECOLLECTIVECLASSROUTE, 1066);   // 1066
                break;
            SYSCALL_CASE(SETCOLLECTIVECLASSROUTE, 1067);        // 1067
                break;
            SYSCALL_CASE(DEALLOCATECOLLECTIVECLASSROUTE, 1068); // 1068
                break;
            SYSCALL_CASE(QUERYGINTCLASSROUTE, 1069);            // 1069
                break;
            SYSCALL_CASE(ALLOCATEGINTCLASSROUTE, 1070);         // 1070
                break;
            SYSCALL_CASE(SETGINTCLASSROUTE, 1071);              // 1071
                break;
            SYSCALL_CASE(DEALLOCATEGINTCLASSROUTE, 1072);       // 1072
                break;
            SYSCALL_CASE(GETRANK, 1076);                        // 1076
                break;
            SYSCALL_CASE(GETPVR, 1089);                         // 1089
                break;
            default:
                CL_ERROR("%lu not implemented, exiting with -1\n", syscall);
                dump_context();
                sc_exit_group(pc->regs.gpr[3], pc->regs.gpr[4], pc->regs.gpr[5],
                              pc->regs.gpr[6], pc->regs.gpr[7], pc->regs.gpr[8]);
                stop_SPC_group();
                goto done;
            }
            // do not return from syscall when process is shutting down
            if (NodeState.KThreads[spc].cl_thread_state == EXIT) {
                CL_DEBUG("cl thread %d aborted in syscall, exitting\n", spc);
                goto done;
            }
            pc->command = SPC_RESUME;
//#define USE_FAST_SYSCALL_ACK
#ifdef USE_FAST_SYSCALL_ACK
            pc->start = 1;
            ioctl(pc->fusedosfs_fd, SPC_IOCTL_WAIT_CMD);
#else
            ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
#endif
        }
    }
    CL_ERROR("Should not be here\n");
done:
//  CL_ERROR("<enter> to exit\n");
//  char buf[16];
//  if (read(0, buf, 1) == -1) CL_ERROR("read failed");

    File_TermFS();
//  if (pc->id % 4 != 0) {
//    pc->command = SPC_EXIT;
//    ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
//  }
//  moved dumping syscall actions to main (no sense in several threads
//  dumping the same data)
}

static void sig_handler(int sig, siginfo_t * siginfo, void * ucontext_ptr)
{
    // Just bail for now...
    if (sig == SIGUSR1) {
        // signal to notify cl threads about exit
        // nothing to be done here - signal will cause
        // blocking system calls to abort, to allow the thread to exit
        return;
    } else if (sig == 2) {
        CL_ERROR_NOSPC("SIGINT, exiting\n");
    } else if (sig == 4) {
        CL_ERROR_NOSPC("SIGILL, exiting\n");
    } else if (sig == 11) {
        ucontext_t * ucontext = (ucontext_t *) ucontext_ptr;
        CL_ERROR_NOSPC("SIGSEGV, addr %lx nip %lx, link %lx, ctr %lx, dar %lx exiting\n",
                (long unsigned int)siginfo->si_addr,
                ucontext->uc_mcontext.regs->nip,
                ucontext->uc_mcontext.regs->link,
                ucontext->uc_mcontext.regs->ctr,
                ucontext->uc_mcontext.regs->dar);
        stop_SPC_group();
        unload_tlbs();
        exit(-1);
    } else {
        CL_ERROR("sig %d, exiting\n", sig);
    }

    sc_exit_group(0, 0, 0, 0, 0, 0);
    stop_SPC_group();
    unload_tlbs();
    if (action_tracker) dump_actions();
    exit(-1);
}

static void pretty_print_TLB(tlb_entry_t* tlbs, int count)
{
    if (!debug) return;

    printf("                                                                                             SSSUUU  GAI        \n");
    printf("TLB   V EA Range                          RA Range                          Size  ThID WIMGE RWXRWX XSSP TID  TL\n");
    printf("----- - ---------------- ---------------- ---------------- ---------------- ----- ---- ----- ------ ---- ---- --\n");

    for(int entry=0; entry<count; entry++) {
        //for(epn=0; epn<128; epn++)
        //{
        //for(way=0; way<4; way++)
        //{

        if ( tlbs[entry].mas1 & MAS1_V(1) ) {

            uint64_t mas1 = tlbs[entry].mas1;
            uint64_t mas2 = tlbs[entry].mas2;
            uint64_t mas7_3 = tlbs[entry].mas7_3;
            uint64_t mas8   = tlbs[entry].mas8;
            uint64_t mmucr3 = tlbs[entry].mmucr3;

            int size = (mas1 & MAS1_TSIZE(-1)) >> (63-55);

            static const char* TSIZE[] = {
                "?????", // [0]
                "4KB ",  // [1]
                "?????", // [2]
                "64KB ", // [3]
                "?????", // [4]
                "1MB  ", // [5]
                "?????", // [6]
                "16MB ", // [7]
                "?????", // [8]
                "256MB", // [9]
                "1GB  ", // [10]
                "4GB  ", // [11]
                "16GB ", // [12]
                "?????", // [13]
                "256GB", // [14]
                "1TB  ", // [15]
            };

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

            uint64_t pageMask = TSIZE_MASK[size] - 1;
            uint64_t xMask    = mas2 & MAS2_EPN(-1) & pageMask;
            uint8_t  ThdID    = (mmucr3 & MMUCR3_ThdID(-1)) >> (63-63);
            uint64_t xOffset  = (mmucr3 & MMUCR3_X(-1)) ? (xMask + 0x1000) : 0;

            printf(
                //"%3ld:%d V %016llX-%016llX %016llX-%016llX %s %d%d%d%d %c%c%c%c%c %c%c%c%c%c%c %c%c%c%c %04llX %02llX\n",
                "  %3d V %016llX-%016llX %016llX-%016llX %s %d%d%d%d %c%c%c%c%c %c%c%c%c%c%c %c%c%c%c %04llX %02llX\n",
                //epn, way,
                entry,
                ((mas2 & MAS2_EPN(-1)     & ~pageMask )) + xOffset , ((mas2 & MAS2_EPN(-1))     + xOffset) | pageMask ,
                ((mas7_3 & MAS7_3_RPN(-1) & ~pageMask )) + xOffset , ((mas7_3 & MAS7_3_RPN(-1)) + xOffset) | pageMask ,
                TSIZE[size],

                (ThdID & 0x8) ? 1 : 0,
                (ThdID & 0x4) ? 1 : 0,
                (ThdID & 0x2) ? 1 : 0,
                (ThdID & 0x1) ? 1 : 0,

                (mas2 & MAS2_W(1)) ? 'W' : '-',
                (mas2 & MAS2_I(1)) ? 'I' : '-',
                (mas2 & MAS2_M(1)) ? 'M' : '-',
                (mas2 & MAS2_G(1)) ? 'G' : '-',
                (mas2 & MAS2_E(1)) ? 'E' : '-',
                (mas7_3 & MAS3_SR(-1)) ? 'R' : '-',
                (mas7_3 & MAS3_SW(-1)) ? 'W' : '-',
                (mas7_3 & MAS3_SX(-1)) ? 'X' : '-',
                (mas7_3 & MAS3_UR(-1)) ? 'r' : '-',
                (mas7_3 & MAS3_UW(-1)) ? 'w' : '-',
                (mas7_3 & MAS3_UX(-1)) ? 'x' : '-',
                (mmucr3 & MMUCR3_X(-1)) ? 'X' : '-',
                (mas8 & MAS8_TGS(-1)) ? 'H' : '-',
                (mas1 & MAS1_TS(-1)) ? '1' : '-',
                (mas1 & MAS1_IPROT(-1)) ? '1' : '-',

                (mas1 & MAS1_TID(-1)) >> (63-47),
                (mas8 & MAS8_TLPID(-1)) >> (63-63)

            );
        }

        /*} // non-pretty version
        else {
        //printf("%3ld:%d  MAS1=%016lx  MAS2=%016lx  MAS7_3=%016lx  MAS8=%016lx  MMUCR3=%016lx  Vaddr=%016llx  Paddr=%016llx  Size=%7d kB  Exclude=%6lld kB\n", epn, way,
        printf("%4d  MAS1=%016lx  MAS2=%016lx  MAS7_3=%016lx  MAS8=%016lx  MMUCR3=%016lx  Vaddr=%016llx  Paddr=%016llx  Size=%7d kB  Exclude=%6lld kB\n", entry,
               tlbs[entry].mas1,
               tlbs[entry].mas2,
               tlbs[entry].mas7_3,
               tlbs[entry].mas8,
               tlbs[entry].mmucr3,
        //                   (((_G52(tlbs[epn][way].mas2, 51)) & (~((1<<(2 * (_G4(tlbs[epn][way].mas1, 55))))/4-1))+1)) * 4,
               (((_G52(tlbs[entry].mas2, 51)*4) & (~((1<<(2*_G4(tlbs[entry].mas1, 55)))-1))))*1024,
               (_G52(tlbs[entry].mas7_3, 51))*4096,
               (1<<(2*_G4(tlbs[entry].mas1, 55))),
               (((_G52(tlbs[entry].mas2, 51)) & ((1<<(2 * (_G4(tlbs[entry].mas1, 55))))/4-1))+1) * 4 * _G1(tlbs[entry].mmucr3, 49)
            );
        } */
    }
}

static int map_user_virt_AS(spc_context_t* pc)
{
    AppProcess_t* pProc = GetMyProcess();

    // setup relocation addresses in FUSEDOSFS
    pc->text_pstart   = pProc->Text_PStart;
    pc->text_pend     = pProc->Text_PEnd;
    pc->data_pstart   = pProc->Data_PStart;
    pc->data_pend     = pProc->Data_PEnd;
    pc->heap_pstart   = pProc->Heap_PStart;
    pc->heap_pend     = pProc->Heap_PEnd;
    pc->shared_pstart = pProc->Shared_PStart;
    pc->shared_pend   = pProc->Shared_PEnd;

    char fn[32];
    int fd;

    // Text segment
    if (pProc->Text_VStart != 0) {
        CL_DEBUG("Mapping spc text, Text_VStart 0x%lx, text_pstart 0x%lx\n", pProc->Text_VStart, pc->text_pstart);

        memset(fn, 0, sizeof(fn));
        snprintf(fn, sizeof(fn), "/fusedosfs/%d_text", (int) (pc->id));

        fd = open(fn, O_RDWR);
        if (fd == -1) {
            CL_ERROR("text open failed, errno %d, %s\n", errno, strerror(errno));
            return -1;
        }

        void* text_start = mmap((void*) (pProc->Text_VStart),
                                pProc->Text_VEnd - pProc->Text_VStart, PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_FIXED, fd, 0);
        if (text_start == MAP_FAILED) {
            CL_ERROR("text mmap failed, Text_VStart 0x%lu, errno %d, %s\n", pProc->Text_VStart, errno, strerror(errno));
            return -1;
        }
        CL_DEBUG("text_start %p\n", text_start);

        close(fd);
    }

    // Data segment
    CL_DEBUG("Mapping spc data, Data_VStart 0x%lx, data_pstart 0x%lx\n", pProc->Data_VStart, pc->data_pstart);
    memset(fn, 0, sizeof(fn));
    snprintf(fn, sizeof(fn), "/fusedosfs/%d_data", (int) (pc->id));

    fd = open(fn, O_RDWR);
    if (fd == -1) {
        CL_ERROR("data open failed, errno %d, %s\n", errno, strerror(errno));
        return -1;
    }

    void* data_start = mmap((void*) (pProc->Data_VStart),
                            pProc->Data_VEnd - pProc->Data_VStart, PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_FIXED, fd, 0);
    if (data_start == MAP_FAILED ) {
        CL_ERROR("data mmap failed, errno %d, %s\n", errno, strerror(errno));
        return -1;
    }

    CL_DEBUG("data_start %p\n", data_start);

    close(fd);

    // Heap
    CL_DEBUG("Mapping spc heap, Heap_VStart 0x%lx, heap_pstart 0x%lx\n", pProc->Heap_VStart, pc->heap_pstart);
    memset(fn, 0, sizeof(fn));
    snprintf(fn, sizeof(fn), "/fusedosfs/%d_heap", (int) (pc->id));

    fd = open(fn, O_RDWR);
    if (fd == -1) {
        CL_ERROR("heap open failed, errno %d, %s\n", errno, strerror(errno));
        return -1;
    }

    void* heap_start = mmap((void*) (pProc->Heap_VStart),
                            pProc->Heap_VEnd - pProc->Heap_VStart, PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_FIXED, fd, 0);
    if (heap_start == MAP_FAILED ) {
        CL_ERROR("heap mmap failed, errno %d, %s\n", errno, strerror(errno));
        return -1;
    }

    CL_DEBUG("heap_start %p\n", heap_start);

    close(fd);

    // Shared memory
    if (NodeState.cl_shm_size != 0) {
        memset(fn, 0, sizeof(fn));
        snprintf(fn, sizeof(fn), "/fusedosfs/%d_shared", (int) (pc->id));

        CL_DEBUG("Mapping shared memory area, Shared_VStart 0x%lx, Shared_PStart 0x%lx\n", pProc->Shared_VStart, pc->shared_pstart);

        fd = open(fn, O_RDWR);
        if (fd == -1) {
            CL_ERROR("shared open failed, errno %d, %s\n", errno, strerror(errno));
            return -1;
        }

        void* shared_start = mmap((void*) (pProc->Shared_VStart), pProc->Shared_VEnd - pProc->Shared_VStart,
                                  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
        if (shared_start == MAP_FAILED ) {
            CL_ERROR("shared mmap failed, errno %d, %s\n", errno, strerror(errno));
            return -1;
        }

        CL_DEBUG("shared_start %p\n", shared_start);
    }

    return 0;
}

int main(int argc, char* argv[], char* envp[])
{
    // workaround for missing info in Aux Vector
    __bg_cpu_count = 1; // causes glibc memset to use QPX

    // reset Futex call counters
    if (do_futex_counts) memset(futex_counts, 0, sizeof(futex_counts));

    cl_debug_init_logbuffers();

    struct cl_cmdline_args args;
    if (cl_cmdline_parser(argc, argv, &args)) {
        printf("error from cmdline_parser\n");
        cl_cmdline_parser_print_help();
        exit(-1); // error occurred
    }

    action_tracker  = args.action_tracker_flag;
    debug           = args.debug_flag || args.dbg_buffer_flag;
    debug_log_buffers = args.dbg_buffer_flag;
    spc_0           = args.spc0_arg;
    int num_procs   = args.num_procs_arg;
    int num_threads = args.num_threads_arg;
    int num_memchunks;
    int rc;

    switch (args.alloc_policy_arg) {
    case alloc_policy_arg_CORES:
        allocation_policy = CORES;
        break;
    case alloc_policy_arg_LINEAR:
        allocation_policy = LINEAR;
        break;
    case alloc_policy_arg_TWO_THREADS:
        allocation_policy = TWO_THREADS;
        break;
    default:
        fprintf(stderr, "unknown allocation policy argument %s!\n", args.alloc_policy_orig);
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    // setup SPC context and FusedOS config early so that CL_DEBUG and
    // CL_ERROR will work
    int fd = open("/fusedosfs/context", O_RDWR);
    if (fd == -1) {
        CL_ERROR_NOSPC("spc context open failed, errno %d\n", errno);
        exit(-1);
    }

    // Map context and spc config at 12 MB
    spc_context_t* pc = (spc_context_t*)mmap((void*)SPC_CONTEXT_VADDR, SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE,
                                             PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0);
    if (pc == MAP_FAILED) {
        CL_ERROR_NOSPC("spc context mmap failed, errno %d\n", errno);
        exit(-1);
    }

    close(fd);

    if (spc_0 < 0 || spc_0 > get_fusedos_config()->nr_spcs || spc_0 % 4 != 0) {
        fprintf(stderr, "SPC number out of range or not a multiple of 4\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }
    if (num_procs < 1 || num_procs > get_fusedos_config()->nr_spcs) {
        fprintf(stderr, "Number of processes (%d) out of range\n", num_procs);
        cl_cmdline_parser_print_help();
        exit(-1);
    }
    if (num_threads < 1 || num_threads > get_fusedos_config()->nr_spcs) {
        fprintf(stderr, "Number of threads out of range\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    num_spcs = num_procs * num_threads;
    if (spc_0 + num_spcs > get_fusedos_config()->nr_spcs) {
        fprintf(stderr, "Not enough SPCs to accomodate n*t application threads\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    // by default, use the SPC_MEMORY of all the SPCs assigned
    if (args.num_memchunks_given) {
        num_memchunks = args.num_memchunks_arg;
    } else {
        num_memchunks = num_spcs;
    }

    if (num_memchunks > num_spcs) {
        fprintf(stderr, "Number of memory chunks out of range\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    int num_cores = num_spcs / 4 + (num_spcs % 4 == 0 ? 0 : 1);

    // shared memory enabled? how much memory to reserve?
    if (args.shm_given) {
        if (args.shm_arg == -1) {
            NodeState.cl_shm_size = CONFIG_DEFAULT_SHAREDMEMSIZE;
        } else {
            NodeState.cl_shm_size = args.shm_arg;
        }
    } else {
        // no shared memory
        NodeState.cl_shm_size = 0;
    }

    // check shared memory size vs. allocated memory
    if (NodeState.cl_shm_size > SPC_MEMORY_SIZE * num_memchunks / CONFIG_CNK_PAGE_SIZE) {
        fprintf(stderr, "Requested shared memory segment is larger than allocated memory\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    if( (argc - optind) == 0 ) {
	fprintf(stderr, "No CNK application given as parameter.\n");
        cl_cmdline_parser_print_help();
        exit(-1);
    }

    CL_DEBUG_NOSPC("spc_0 %d, num_procs %d, num_threads %d, num_spcs %d, num_memchunks %d, cl_shm_size %d MB, alloc policy %s\n",
                   spc_0, num_procs, num_threads, num_spcs, num_memchunks, NodeState.cl_shm_size, str_alloc_policy());

    init_cl_mu_data(); // metadata for MUFS interface

    for (int i = 0; i < num_spcs; i++) {
        clear_context(&(pc[i]));
    }

    pthread_key_create(&spc_key, NULL);
    pthread_setspecific(spc_key, &spc_0);

    pc = get_spc_context(spc_0);
    CL_DEBUG("pc->mem_bot 0x%lx\n", pc->mem_bot);
    get_fusedos_config()->fusedos_debug = debug;

    for (int i = spc_0; i < spc_0 + num_spcs; i++) {
        char fn[32];
        memset(fn, 0, 32);
        sprintf(fn, "/fusedosfs/%d_memory", i);
        pthread_setspecific(spc_key, &i);
        pc = get_spc_context(i);
        pc->fusedosfs_fd = open(fn, O_RDWR);
        if (pc->fusedosfs_fd == -1) {
            CL_ERROR("spc memory open failed, errno %d\n", errno);
            exit(-1);
        }
    }

    pthread_setspecific(spc_key, &spc_0);
    pc = get_spc_context(spc_0);

    // use sigaction for SIGUSR1 to avoid flag SA_RESTART (restart syscall)
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags |= SA_SIGINFO;
    sa.sa_sigaction = sig_handler;
    if (sigaction(SIGUSR1, &sa, NULL))
        CL_ERROR("error setting signal handler for SIGUSR1, %d %s\n", errno, strerror(errno));
    if(sigaction(SIGINT, &sa, NULL))
        CL_ERROR("error setting signal handler for SIGINT, %d %s\n", errno, strerror(errno));
    if(sigaction(SIGILL, &sa, NULL))
        CL_ERROR("error setting signal handler for SIGILL, %d %s\n", errno, strerror(errno));
    if(sigaction(SIGSEGV, &sa, NULL))
        CL_ERROR("error setting signal handler for SIGSEGV, %d %s\n", errno, strerror(errno));

    // initialize and mmap cl shared memory area for access to MU alloc lock
    if (init_cl_shm_area())
        exit(-1); // an error occurred

    // Set up application state
    AppState_t* app_state = &(NodeState.AppState[spc_0]);

    // Copy args after "--" into app state
    memset(app_state->App_Args, 0, APP_MAX_ARGSTR);
    int i = 0;
    for (int j = optind; j < argc; j++) {
        strncpy(&(app_state->App_Args[i]), argv[j], strlen(argv[j]));
        i += strlen(argv[j]) + 1;
    }
    app_state->App_Argc = argc - optind;

    // Copy environment variables into app state
    char* p;
    app_state->Env_Total_Length = 0;
    while ((p = *envp++)) {
        strncpy(&(app_state->App_Env[app_state->Env_Total_Length]), p, strlen(p));
        app_state->Env_Total_Length += (strlen(p) + 1);
    }

    // Shape information to trick BGPM. Note that we redefined
    // CONFIG_MAX_APP_CORES to 1.
    app_state->shape.core = 16 /* CONFIG_MAX_APP_CORES */;

    app_state->Active_Cores     = num_cores;
    app_state->Active_Processes = num_procs;

    app_state->JobStartTime = GetCurrentTimeInMicroseconds();

    // read /dev/bgpers and populate environment variables (as most BG/Q
    // Linux init scripts do
    if(readPersonalityIntoEnvironment())
	exit(-1);

    char* nodeCoordsEnv = getenv(PERSO_ENV_NODE_COORDS);
    char* nodeDimensionsEnv = getenv(PERSO_ENV_NODE_DIMENSIONS);

    if (nodeCoordsEnv == NULL) {
        CL_ERROR("Did not find environment variable %s Using default <0,0,0,0,0> instead.\n", PERSO_ENV_NODE_COORDS);
        nodeCoordsEnv = (char*)"<0,0,0,0,0>";
    }

    if (nodeDimensionsEnv == NULL) {
        CL_ERROR("Did not find environment variable %s Using default <1,1,1,1,1> instead.\n", PERSO_ENV_NODE_DIMENSIONS);
        nodeDimensionsEnv = (char*)"<1,1,1,1,1>";
    }

    // Get job dimensions from parameters, or use whole block
    if( args.job_corner_given && args.job_shape_given ) {
	if( stringToCoords(args.job_corner_arg, &(app_state->corner))
	    || stringToCoords(args.job_shape_arg, &(app_state->shape)) ) {
	    CL_ERROR("argument job_corner \"%s\" or job_shape \"%s\" malformed.\n",
		    args.job_corner_arg, args.job_shape_arg);
	    exit(-1);
	}
    } else {
	// use default, whole block
	if (stringToCoords(nodeDimensionsEnv, &(app_state->shape))) {
	    CL_ERROR("failed to use %s value %s.\n", PERSO_ENV_NODE_DIMENSIONS, nodeDimensionsEnv);
	    exit(-1);
	}

	// block / node corner
	stringToCoords((char*)"<0,0,0,0,0>", &(app_state->corner)); // use whole block for now.
    }

    // this node's coordinates
    AppNodeCoordinates_t myLocation;
    if (stringToCoords(nodeCoordsEnv, &myLocation)) {
        CL_ERROR("failed to use %s value %s.\n", PERSO_ENV_NODE_COORDS, nodeCoordsEnv);
        exit(-1);
    }

    // the whole block
    AppNodeCoordinates_t block;
    if (stringToCoords(nodeDimensionsEnv, &block)) {
	CL_ERROR("failed to use %s value %s.\n", PERSO_ENV_NODE_DIMENSIONS, nodeDimensionsEnv);
	exit(-1);
    }

    // set torus dimension to rank mapping order
    strncpy(app_state->mapOrder, args.job_mapOrder_arg, 8); // default would be ABCDET
    CL_DEBUG("setting mapOrder to %s, result %s, at %p\n", args.job_mapOrder_arg, app_state->mapOrder, app_state->mapOrder);
    // no mapping file
    memset(app_state->mapFilePath, 0, sizeof(app_state->mapFilePath));

    // set dimensions and coordinates in personality
    NodeState.Personality.Network_Config.Anodes = block.aCoord;
    NodeState.Personality.Network_Config.Bnodes = block.bCoord;
    NodeState.Personality.Network_Config.Cnodes = block.cCoord;
    NodeState.Personality.Network_Config.Dnodes = block.dCoord;
    NodeState.Personality.Network_Config.Enodes = block.eCoord;

    NodeState.Personality.Network_Config.Acoord = myLocation.aCoord;
    NodeState.Personality.Network_Config.Bcoord = myLocation.bCoord;
    NodeState.Personality.Network_Config.Ccoord = myLocation.cCoord;
    NodeState.Personality.Network_Config.Dcoord = myLocation.dCoord;
    NodeState.Personality.Network_Config.Ecoord = myLocation.eCoord;

    // set the total number of ranks according to block dimensions and
    // processes per node
    unsigned int nodesInBlock = block.aCoord * block.bCoord
	    * block.cCoord * block.dCoord * block.eCoord;
    unsigned int nodesInJob = app_state->shape.aCoord
	* app_state->shape.bCoord * app_state->shape.cCoord
	* app_state->shape.dCoord * app_state->shape.eCoord;

    app_state->subBlockActive = (nodesInJob < nodesInBlock);

    CL_DEBUG_NOSPC("block <%d,%d,%d,%d,%d> loc <%d,%d,%d,%d,%d> job <%d,%d,%d,%d,%d> <%d,%d,%d,%d,%d>\n",
	    block.aCoord, block.bCoord, block.cCoord,
	    block.dCoord, block.eCoord,
	    myLocation.aCoord, myLocation.bCoord, myLocation.cCoord,
	    myLocation.dCoord, myLocation.eCoord,
	    app_state->corner.aCoord, app_state->corner.bCoord,
	    app_state->corner.cCoord, app_state->corner.dCoord,
	    app_state->corner.eCoord,
	    app_state->shape.aCoord, app_state->shape.bCoord,
	    app_state->shape.cCoord, app_state->shape.dCoord,
	    app_state->shape.eCoord);

    app_state->ranksPerNode = num_procs;
    app_state->ranksTotal   = nodesInJob * app_state->ranksPerNode; // (num_procs)

    // take number of ranks from parameter, or use whole block
    if (args.job_ranks_given) {
        app_state->ranksActive = args.job_ranks_arg;
    } else {
        // we default to using the whole block
        app_state->ranksActive = app_state->ranksTotal;
    }

    // Set up node state. We are pinning kthreads to hwthreads for now.
    for (int i = spc_0; i < spc_0 + num_spcs; i++) {
        pthread_setspecific(spc_key, &i);
        int proc = (i-spc_0) / num_threads;
        NodeState.CoreState[i / 4].HWThreads[(i - spc_0) % 4].SchedSlot[CONFIG_SCHED_SLOT_FIRST] = &(NodeState.KThreads[i]);

        NodeState.AppState[spc_0].ProcStateIndex[i] = proc;
        NodeState.pAppState[i] = &(NodeState.AppState[spc_0]);
        NodeState.KThreads[i].pAppProc = &(NodeState.AppProcess[proc]);
        NodeState.KThreads[i].ProcessorID = i;
        NodeState.KThreads[i].cl_thread_state = IDLE;

        // per-process initialization
        if ((i - spc_0) % num_threads == 0) {
            NodeState.AppProcess[proc].Tcoord = proc;
            rc = getMyRank(&NodeState.AppProcess[proc].Rank);
            if (rc) {
                CL_ERROR("failed setting Rank for Process %d\n", proc);
            }
            CL_DEBUG("installing Tcoord %d, Rank %d for process %d\n",
                     NodeState.AppProcess[proc].Tcoord, NodeState.AppProcess[proc].Rank, proc);
            NodeState.AppProcess[proc].ProcessLeader_KThread = &(NodeState.KThreads[i]);
            if (getcwd(NodeState.AppProcess[proc].CurrentDir, APP_MAX_PATHNAME) == NULL) {
                CL_ERROR("getcwd failed, errno %d\n", errno);
                exit(-1);
            }
            NodeState.AppProcess[proc].straceEnabled = debug;
			NodeState.AppProcess[proc].HWThread_Count = num_threads;
        }
    }
    pthread_setspecific(spc_key, &spc_0);

    if (debug)
        for(int i=0; i < 64; i++) {
            if (!NodeState.KThreads[i].pAppProc) continue;
            printf("%2d: processorId %2d appProc %p; pAppState %p\n", i,
                   NodeState.KThreads[i].ProcessorID, NodeState.KThreads[i].pAppProc,
                   NodeState.pAppState[i]);
        }

    NodeState.FirstAppAgentProcessorID = 1;
    NodeState.AppAgents = 0;
    NodeState.TraceConfig = 0;
    if (debug) NodeState.TraceConfig = TRACE_Jobctl|TRACE_VMM|TRACE_Process|TRACE_StaticMap|TRACE_VirtFS;

    // Map memory at its physical address for program loading and setup
    void* my_memory_bot = mmap((void*)((uint64_t)SPC_MEMORY_PADDR + SPC_MEMORY_SIZE * (uint64_t)spc_0),
                               SPC_MEMORY_SIZE * (uint64_t)(num_memchunks),
                               PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, pc->fusedosfs_fd, 0);
    if (my_memory_bot == MAP_FAILED) {
        CL_ERROR("spc memory mmap failed, errno %d\n", errno);
        exit(-1);
    }

    void* my_memory_top = (void*)((uint64_t)(my_memory_bot) + SPC_MEMORY_SIZE * (uint64_t)(num_memchunks));
    CL_DEBUG("my_memory_bot 0x%lx\n", (long unsigned int)(my_memory_bot));
    CL_DEBUG("my_memory_top 0x%lx\n", (long unsigned int)(my_memory_top));

//#define NSECS_PER_SEC 1000000000
//    struct timespec before, after;
//    clock_gettime(CLOCK_MONOTONIC_RAW, &before);

    memset(my_memory_bot, 0, (uint64_t)my_memory_top - (uint64_t)my_memory_bot);

//    clock_gettime(CLOCK_MONOTONIC_RAW, &after);
//    long nsecs = after.tv_sec*NSECS_PER_SEC + after.tv_nsec
//                 - (before.tv_sec * NSECS_PER_SEC + before.tv_nsec);
//    long mb = SPC_MEMORY_SIZE * (long)num_memchunks / 1024 / 1024;
//    double mb_per_sec = (1.0*mb / nsecs)*NSECS_PER_SEC;
//    CL_DEBUG("memset took %ld secs, rate of %g MB/sec\n", nsecs/NSECS_PER_SEC, mb_per_sec);

    // Tell CNK about the physical memory it may use
    NodeState.DomainDesc.ddrOrigin = get_spc_context(spc_0)->mem_bot;
    NodeState.DomainDesc.ddrEnd = get_spc_context(spc_0)->mem_bot + SPC_MEMORY_SIZE * (long)num_memchunks;

    // If the physical address is not 1GB-aligned (so we can use only
    // 16MB-pages anyway), we do not need to waste the 16MB kernel area.
    // Memory outside the SPC memory area is protected by the mmap's boundaries.
    if (NodeState.DomainDesc.ddrOrigin % 1024*1024*1024 != 0)
        NodeState.DomainDesc.ddrOrigin -= __KERNEL_END;

    CL_DEBUG("DomainDesc ddrOrigin %llx ddrEnd %llx\n",
             NodeState.DomainDesc.ddrOrigin, NodeState.DomainDesc.ddrEnd);

    // File System setup per process
    for (int i = spc_0; i < spc_0 + num_spcs; i+= num_threads) {
        pthread_setspecific(spc_key, &i);
        File_InitFS();
        rc = File_ProcessSetup();
        if (rc != 0) {
            CL_ERROR("Bad return code from File_ProcessSetup, rc %d\n", rc);
            exit(-1);
        }
    }

    pthread_setspecific(spc_key, &spc_0);

#if 0
    // If the UPC FS kernel module has been loaded, set it up
    if (Upci_Init_Linux(1) != 0) {
        CL_ERROR("Upci_Init_Linux failed\n");
    }
#endif

	// do not generate mappings for speculative aliases
	// (reduce complexity while we do not need them yet)
	vmm_setFlags(MAPPERFLAGS_NOALIASES, 1);

    // load ELF image and use ddrOrigin as an offset to the addresses in the ELF headers
    rc = elf_loadimage(&(NodeState.AppState[spc_0]), NodeState.DomainDesc.ddrOrigin, NULL);
    if (rc != 0) {
        CL_ERROR("Bad return code from elf_loadimage, rc %d\n", rc);
        exit(-1);
    }

    rc = vmm_setupTLBMapper(__KERNEL_END >> 20);
    if (rc != 0) {
        CL_ERROR("Bad return code from vmm_setupTLBMapper, rc %d\n", rc);
        exit(-1);
    }

    // numprocesses, numagents, core_index
    rc = vmm_resetStaticMap(num_procs, 0, -1);
    if (rc != 0) {
        CL_ERROR("Bad return code from vmm_resetStaticMap, rc %d\n", rc);
        exit(-1);
    }

    rc = App_SetupMemory(&(NodeState.Personality));
    if (rc != 0) {
        CL_ERROR("Bad return code from App_SetupMemory, rc %d\n", rc);
        exit(-1);
    }

    // setup memory per process
    for (int i = spc_0; i < spc_0 + num_spcs; i += num_threads) {
        pthread_setspecific(spc_key, &i);
        rc = Process_SetupMemory();
        if (rc != 0) {
            CL_ERROR("Bad return code from Process_SetupMemory, rc %d\n", rc);
            exit(-1);
        }
    }

    // load TLB per core
    for (int i = spc_0; i < spc_0 + num_spcs; i += 4) {
        pthread_setspecific(spc_key, &i);
        rc = vmm_installStaticTLBMap(GetMyProcess()->Tcoord);
        if (rc != 0) {
            CL_ERROR_NOSPC("Bad return code from vmm_installStaticTLBMap, rc %d\n", rc);
            exit(-1);
        }

        // Adding MU device memory segment. In AppSetup we have
        // announced this segment to the TLB mapping algorithm, so we
        // don't produce TLB entry conflicts.
		// TODO MARIUS ... experiment disabling this ...
		 /*
        rc = vmm_MapUserSpace(APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_GUARDED | APP_FLAGS_INHIBITED,
                              (void*)PHYMAP_MINADDR_MU, (void*)PHYMAP_MINADDR_MU, VMM_PAGE_SIZE_16M, 0, 0);
        if(rc) {
            CL_ERROR_NOSPC("Bad return code %d from vmm_addSegment\n", rc);
            exit(-1);
        }
        CL_DEBUG("called vmm_addSegment for MU MMIO segment\n");

        rc = vmm_MapUserSpace(APP_FLAGS_R | APP_FLAGS_W | APP_FLAGS_GUARDED | APP_FLAGS_INHIBITED,
                             (void*)PHYMAP_MINADDR_DCR, (void*)PHYMAP_MINADDR_DCR, VMM_PAGE_SIZE_16M, 0, 0);
        if(rc) {
            CL_ERROR_NOSPC("Bad return code %d from vmm_addSegment\n", rc);
            exit(-1);
        }
        CL_DEBUG( "called vmm_addSegment for DCR MMIO segment\n" );
		*/

        pc = get_spc_context( i );
        CL_DEBUG("installing TLB in SPC %d, tlb_entry_install %lu, tlb_entry_count %lu\n",
                 i, pc->tlb_entry_install, pc->tlb_entry_count);
        pretty_print_TLB(pc->tlb_entry, pc->tlb_entry_count);
        pc->command = SPC_LOAD_TLB;
        ioctl(pc->fusedosfs_fd, SPC_IOCTL_COMMAND);
        App_SetIULivelockConfig();
    }

    pthread_setspecific(spc_key, &spc_0);
    pc = get_spc_context( spc_0 );

    NodeState.cl_firstspc = spc_0;
    NodeState.cl_numspcs  = num_threads;

    // release SPC memory mapping to physical addresses
    if (munmap(my_memory_bot, SPC_MEMORY_SIZE * (uint64_t)(num_memchunks))) {
        CL_ERROR("Error munmapping SPC memory from virtual addresses == physical addresses area, %d, %s",
                 errno, strerror(errno));
        exit(-1);
    }

    pid_t child_pid[64]; // indexed by process number
    int spc = spc_0;

    for (int i = 1; i < num_procs; i++) {
        child_pid[i] = fork();

        if (child_pid[i] < 0) {
            CL_DEBUG("fork failed, errno %d, %s\n", errno, strerror(errno));
            exit(-1);
        }
        if (child_pid[i] > 0) {
            // Parent
            CL_DEBUG("Parent, ProcessorID() %d, fusedosfs_fd %d, i %d\n", ProcessorID(), pc->fusedosfs_fd, i);
            if (args.serial_procs_flag) {
                pid_t pid;
                int status;
                pid = wait(&status);
                CL_DEBUG("Parent, child done (i %d, pid %d, status %d)\n", i, pid, status);
            }
        } else {
            // Child
            spc = spc_0 + i*num_threads; // Need to reset spc for code after App_CreateMainThread()
            NodeState.cl_firstspc = spc;
            pthread_setspecific(spc_key, &spc);
            pc = get_spc_context(spc);
            CL_DEBUG("Child, spc %d, i %d, ProcessorID() %d, fusedosfs_fd %d\n", spc, i, ProcessorID(), pc->fusedosfs_fd);
            break;
        }
    }

    rc = pthread_mutex_init(&NodeState.cl_thread_mutex, NULL);
    if (rc) {
        CL_ERROR("error creating cl mutex, rc %d %s\n", rc, strerror(rc) );
        exit(-1);
    }

    rc = pthread_cond_init(&NodeState.cl_thread_cond, NULL);
    if (rc) {
        CL_ERROR("error creating cl condition variable, rc %d %s\n", rc, strerror(rc) );
        exit(-1);
    }

    rc = pthread_mutex_init(&mapping_storageMutex, NULL);
    if (rc) {
        CL_ERROR("could not initialize mutex mapping_storageMutex, %d, %s\n", rc, strerror(rc));
        exit(-1);
    }

	rc = pthread_mutex_init(&loadSequenceUpdateMutex, NULL);
    if (rc) {
        CL_ERROR("could not initialize mutex loadSequenceUpdateMutex, %d, %s\n", rc, strerror(rc));
        exit(-1);
    }

    // initialize mutexes in AppProcess
    rc = pthread_mutex_init(&(NodeState.AppProcess[spc].MMap_mutex), NULL);
    if (rc) { CL_ERROR_NOSPC("error %d allocating MMap_mutex, %s\n", rc, strerror(rc)); exit(-1); }

    rc = pthread_mutex_init(&(NodeState.AppProcess[spc].Mprotect_mutex), NULL);
    if (rc) { CL_ERROR_NOSPC("error %d allocating Mprotect_mutex, %s\n", rc, strerror(rc)); exit(-1); }

    rc = pthread_mutex_init(&(NodeState.AppProcess[spc].DescriptorTable_mutex), NULL);
    if (rc) { CL_ERROR_NOSPC("error %d allocating DescriptorTable_mutex, %s\n", rc, strerror(rc)); exit(-1); }

    rc = pthread_mutex_init(&MMapRead_mutex, NULL);
    if (rc) { CL_ERROR_NOSPC("error %d allocating MMapRead_mutex, %s\n", rc, strerror(rc)); exit(-1); }

    rc = pthread_mutex_init(&mappedFiles_mutex, NULL);
    if (rc) { CL_ERROR_NOSPC("error %d allocating mappedFiles_mutex, %s\n", rc, strerror(rc)); exit(-1); }

    // change from physical address layout for program loading to virtual layout for process runtime
    if (map_user_virt_AS(pc)) {
        // debug/error messages already printed in map_user_virt_AS()
        exit(-1);
    }

    CL_DEBUG("calling App_CreateMainThread, Proc_EntryPoint = %p \n", (void*)GetMyProcess()->Proc_EntryPoint);
    App_CreateMainThread(GetMyProcess()->Proc_EntryPoint, 0 /* priv */ );

    if (pc->regs.ip == 0) {
        CL_ERROR("IP for process startup is 0. Something must have gone wrong!\n");
        exit(-1);
    }

    // initialize cl thread states
    // important: do this before creating these threads to avoid race conditions
    NodeState.KThreads[spc].cl_thread_state = USE_SPC;

    for (int i = spc+1; i < spc+ num_threads; i++) {
        NodeState.KThreads[i].cl_thread_state = IDLE;
    }
    
    // create all cl threads, one per thread; most of them will sleep
    for (int i = spc; i < spc+ num_threads; i++) {
        rc = pthread_create( &NodeState.KThreads[i].cl_thread, NULL, (void* (*)(void*))(&kthread), (void*)i);
        if (rc) {
            CL_ERROR("error creating cl thread for SPC %d, %d, %s\n", i, rc, strerror(rc));
            exit(-1);
        }
    }

    CL_DEBUG("created all threads in main (SPC %d)\n", spc);

    // join all active threads
    CL_DEBUG("joining cl threads in main\n");
    for(int i = spc; i < spc+ num_threads; i++) {
        pthread_join( NodeState.KThreads[i].cl_thread, NULL);
    }

    if (spc == spc_0 && num_procs > 1) {
        int status;
        CL_DEBUG("Waiting for children\n");
        for (int i = 1; i < num_procs; i++)
            if (child_pid[i] > 0) waitpid(child_pid[i], &status, 0);
    }
    if (spc == spc_0) {
        stop_SPC_group();
        unload_tlbs();
    }

    CL_DEBUG("main completed (SPC %d, exit_status %d)\n", spc, exit_status);

    if (do_futex_counts) {
        printf("Futex syscall counts:\n");
        int wait_sum = 0, wake_sum = 0, wake_op_sum = 0, requeue_sum = 0, cmp_requeue_sum = 0;

        printf("THR|WAIT|WAKE|WKOP|REQ|CMP_REQ\n");

        for(int i = 0; i < 64; i++) {
            wait_sum        += futex_counts[i][FUTEX_WAIT];
            wake_sum        += futex_counts[i][FUTEX_WAKE];
            wake_op_sum     += futex_counts[i][FUTEX_WAKE_OP];
            requeue_sum     += futex_counts[i][FUTEX_REQUEUE];
            cmp_requeue_sum += futex_counts[i][FUTEX_CMP_REQUEUE];

            printf("%3d|%4d|%4d|%4d|%3d|%6d\n", i,
                   futex_counts[i][FUTEX_WAIT],
                   futex_counts[i][FUTEX_WAKE],
                   futex_counts[i][FUTEX_WAKE_OP],
                   futex_counts[i][FUTEX_REQUEUE],
                   futex_counts[i][FUTEX_CMP_REQUEUE]);
        }

        printf("\nTHR|WAIT|WAKE|WKOP|REQ|CMP_REQ\n");
        printf("SUM|%4d|%4d|%4d|%3d|%6d\n",
               wait_sum, wake_sum, wake_op_sum,
               requeue_sum, cmp_requeue_sum);
    }

    if (action_tracker) dump_actions();

    if (debug_log_buffers) cl_debug_dump_log_buffers();

    exit(exit_status);
}
