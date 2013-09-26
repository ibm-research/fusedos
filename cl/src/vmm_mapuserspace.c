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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Kernel.h"
#include "fusedos_config.h"
#include "fusedos.h"

//#include "AppProcess.h"
//#include "vmm.h"
#include <hwi/include/bqc/A2_core.h>

void usage() {
  printf("usage: -p <phys addr> -v <virt addr> -s <size>\n");
  exit(-1);
}

int main(int argc, char* argv[]) {
  char c;
  uint64_t pa = 0xffffffffffffffff;
  uint64_t va = 0xffffffffffffffff;
  size_t tlbsize = 0xffffffffffffffff;

  while ((c = getopt(argc, argv, "p:v:s:")) != -1) {
printf("c %c %d, pa %016lx, va %016lx, tlbsize %016lx\n", c, c, pa, va, tlbsize);
    switch (c) {
    case 'p':
      pa = atol(optarg);
      break;
    case 'v':
      va = atol(optarg);
      break;
    case 's':
      tlbsize = atol(optarg);
      break;
    case 255:
      goto hack; // How are we getting 255?!
    default:
      usage();
    }
  }
hack:
  if (pa == 0xffffffffffffffff || va == 0xffffffffffffffff || tlbsize == 0xffffffffffffffff) usage();

  uint64_t flags = APP_FLAGS_NONSPECULATIVE | APP_FLAGS_LISTENABLE;
  unsigned pid = 0;
  size_t exclude = 0;
  uint64_t xbit = 0;
  uint64_t tlbmask = 0;
  if (exclude) {
    xbit = 1;
    va += exclude - 1;
  }
  switch (tlbsize) {
    case VMM_PAGE_SIZE_1M:   tlbmask = MAS1_TSIZE_1MB;  break;
    case VMM_PAGE_SIZE_16M:  tlbmask = MAS1_TSIZE_16MB; break;
    case VMM_PAGE_SIZE_1G:   tlbmask = MAS1_TSIZE_1GB;  break;
    default: return -1;
  }
  uint64_t mas2 = 0;
  uint64_t mas3 = 0;
  if(flags & APP_FLAGS_R) mas3 |= MAS3_UR(1);
  if(flags & APP_FLAGS_W) mas3 |= MAS3_UW(1);
  if(flags & APP_FLAGS_X) mas3 |= MAS3_UX(1);
  if(flags & APP_FLAGS_NONSPECULATIVE) mas3 |= MAS3_U0(1);
  if(flags & APP_FLAGS_LISTENABLE) mas3 |= MAS3_U3(1);
  if(flags & APP_FLAGS_GUARDED) mas2 |= MAS2_G(1);
  if(flags & APP_FLAGS_INHIBITED) mas2 |= MAS2_I(1);
  printf("slot   %016lx\n", ((flags)>>16) & 3);
  printf("mas1   %016llx\n", MAS1_V(1) | MAS1_TID(pid) | MAS1_TS(0) | tlbmask);
  printf("mas2   %016llx\n", MAS2_EPN(va>>12) | MAS2_W(0) | MAS2_M(1) | MAS2_E(0) | mas2);
  printf("mas7_3 %016llx\n", MAS7_3_RPN(pa>>12) | MAS3_SR(1) | MAS3_SW(1) | MAS3_SX(1) | mas3);
  printf("mas8   %016llx\n", MAS8_TGS(0) | MAS8_VF(0) | MAS8_TLPID(0));
  printf("mmucr3 %016llx\n", MMUCR3_X(xbit) | MMUCR3_R(1) | MMUCR3_C(1) | MMUCR3_ECL(0) | MMUCR3_CLASS(1) | MMUCR3_ThdID(0xF));

  return(0);
}
