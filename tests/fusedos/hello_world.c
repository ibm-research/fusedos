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

#include <fcntl.h>
#if 0
#include <spi/include/kernel/location.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

extern void HPM_Init(void);
extern void HPM_Start(char* this_label);
extern void HPM_Stop(char* this_label);
extern void HPM_Print(void);

int main(int argc, char* argv[], char* envp[])
{
    int segv = 0, dump_env = 0, spi_test = 0, bgpm_test = 0, loop = 0, c;
    while ((c = getopt(argc, argv, "sepbl")) != -1) {
        switch (c) {
        case 's':
            segv = 1;
            break;
        case 'e':
            dump_env = 1;
            break;
        case 'p':
            spi_test = 1;
            break;
        case 'b':
            bgpm_test = 1;
            break;
        case 'l':
            loop = 1;
            break;
        default:
            printf("usage: hello_world [-s|e|p|b|l]\n");
            return(-1);
        }
    }
    if (segv) {
        // Generate a segv
        if (open("/tmp/0123456789", O_RDONLY) == -1) perror("open failed");
        int* p;
        p = (int*)(0);
        *p = 666;
    }
    if (dump_env) {
        // Dump environment variables
        char* p;
        printf("Environment variables\n");
        while ((p = *envp++)) {
            printf("%s\n", p);
        }
    }
    if (spi_test) {
        printf("SPI test disabled\n");
#if 0
        uint64_t spr_info = mfspr(SPRG_SPIinfo_ro);
        uint64_t num_procs = (uint64_t)((spr_info >> SPRG_SPIinfo_NumProcs) & 0xFF);
        uint64_t num_cores = (uint64_t)((spr_info >> SPRG_SPIinfo_NumCores) & 0x0F) + 1;
        uint64_t app_leader_core = (uint64_t)((spr_info >> SPRG_SPIinfo_AppLeaderCore) & 0x0F);
        uint64_t thds = (num_cores*4)/num_procs;
        printf("spr_info 0x%lx, num_procs %lu, num_cores %lu, app_leader_core %lu, thds %lu\n", spr_info, num_procs, num_cores, app_leader_core, thds);
        printf("Kernel_ThreadMask(Kernel_MyTcoord()) 0x%lx\n", Kernel_ThreadMask(Kernel_MyTcoord()));
#endif
    }
    if (bgpm_test) {
        printf("BGPM test disabled\n");
#if 0
        printf("Calling HPM_Init()\n");
        HPM_Init();
        HPM_Start("test");
        HPM_Stop("test");
        HPM_Print();
#endif
    }
    printf("Hello world, pid %d, fp sum %f, getuid %d\n", getpid(), 1.3 + 2.5, getuid());
    if (loop) while (1);
//  if (loop) while (1) sleep(1);
//  write(1, "Hello world\n", strlen("Hello world\n"));
//  sleep(1);
    return(0);
}
