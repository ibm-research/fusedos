/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/* IBM Confidential                                                 */
/*                                                                  */
/* Licensed Machine Code Source Materials                           */
/*                                                                  */
/* Product(s):                                                      */
/*     Blue Gene/Q Licensed Machine Code                            */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011, 2012                        */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fusedos_config.h"
#include "fusedos.h"

pthread_key_t spc_key;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage: dump_context <spc>\n");
        exit(-1);
    }
    int spc = atoi(argv[1]);

    pthread_key_create(&spc_key, NULL);
    pthread_setspecific(spc_key, &spc);

    int fd = open("/fusedosfs/context", O_RDWR);
    if (fd == -1) {
        perror("context open failed");
        exit(-1);
    }
    spc_context_t* sc = (spc_context_t*)mmap((void*)SPC_CONTEXT_VADDR, SPC_CONTEXT_SIZE + FUSEDOS_CONFIG_SIZE,
                                             PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, fd, 0);
    if (sc == MAP_FAILED) {
        perror("spc_context mmap failed");
        printf("errno %d\n", errno);
        exit(-1);
    }
    close(fd);

    sc = get_spc_context(this_spc());
    regs_t* r = &(sc->regs);
    printf("id %lu, start %lu, command %lu, ppr32 0x%lx, ipi_wakeup %lu, ex_code %lu\n", sc->id, sc->start, sc->command, sc->ppr32, sc->ipi_wakeup, sc->ex_code);
    printf("mem_bot %016lx, tlb_entry_count %lu, tlb_entry_install %lu\n", sc->mem_bot, sc->tlb_entry_count, sc->tlb_entry_install);
    printf("spcm_sp %016lx, spcm_toc %016lx\n", sc->spcm_sp, sc->spcm_toc);
    printf("ipi fcn %016lx, parm1 %016lx, parm2 %016lx\n", sc->ipi_message.fcn, sc->ipi_message.parm1, sc->ipi_message.parm2);
    printf("bic_int_send %016lx, bic_value %016lx\n", (uint64_t)(sc->bic_int_send), sc->bic_value);
    printf("BG_IULLAVOIDPERIOD %016lx, BG_IULLAVOIDDELAY %016lx\n", (uint64_t)(sc->BG_IULLAVOIDPERIOD), sc->BG_IULLAVOIDDELAY);
    printf("scratch0 %016lx, scratch1 %016lx, scratch2 %016lx, scratch3 %016lx\n", sc->scratch0, sc->scratch1, sc->scratch2, sc->scratch3);
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
    printf("  dear %016lx     pid %016lx    dbsr %016lx               \n",     r->dear,      r->pid,     r->dbsr             );
    printf(" dbcr0 %016lx   dbcr1 %016lx   dbcr2 %016lx   dbcr3 %016lx\n",    r->dbcr0,    r->dbcr1,    r->dbcr2,    r->dbcr3);
    printf("  dac1 %016lx    dac2 %016lx    dac3 %016lx    dac4 %016lx\n",     r->dac1,     r->dac2,     r->dac3,     r->dac4);
    printf("  iac1 %016lx    iac2 %016lx    iac3 %016lx    iac4 %016lx\n",     r->iac1,     r->iac2,     r->iac3,     r->iac4);
    int i;
    for (i = 0; i < sc->tlb_entry_count; i++) {
        printf("tlb %d\n", i);
        printf("  slot %016lx    mas1 %016lx    mas2 %016lx\n", sc->tlb_entry[i].slot, sc->tlb_entry[i].mas1, sc->tlb_entry[i].mas2);
        printf("mas7_3 %016lx    mas8 %016lx  mmucr3 %016lx\n", sc->tlb_entry[i].mas7_3, sc->tlb_entry[i].mas8, sc->tlb_entry[i].mmucr3);
    }

#if 0
    printf("Offsets\n");
#define OFFSET(x) ((void*)(&(sc->x)) - (void*)(pc))
    printf("  bic_int_send      0x%lx\n", OFFSET(bic_int_send));
    printf("  bic_value         0x%lx\n", OFFSET(bic_value));
    printf("  ppr32             0x%lx\n", OFFSET(ppr32));
    printf("  ipi_wakeup        0x%lx\n", OFFSET(ipi_wakeup));
    printf("  id                0x%lx\n", OFFSET(id));
    printf("  start             0x%lx\n", OFFSET(start));
    printf("  tlb_entry_count   0x%lx\n", OFFSET(tlb_entry_count));
    printf("  tlb_entry_install 0x%lx\n", OFFSET(tlb_entry_install));
    printf("  pm_sp             0x%lx\n", OFFSET(pm_sp));
    printf("  pm_toc            0x%lx\n", OFFSET(pm_toc));
    printf("  ex_code           0x%lx\n", OFFSET(ex_code));
    printf("  scratch0          0x%lx\n", OFFSET(scratch0));
    printf("  scratch1          0x%lx\n", OFFSET(scratch1));
    printf("  scratch2          0x%lx\n", OFFSET(scratch2));
    printf("  scratch3          0x%lx\n", OFFSET(scratch3));
#endif

    return(0);
}
