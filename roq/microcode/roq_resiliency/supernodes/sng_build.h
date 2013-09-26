#ifndef __SNG_BUILD__
#define __SNG_BUILD__

#include "sng_common.h"

void     sng_buildSetup();

dim_t    sn_get(dim_t nc);

uint8_t  sn_getIndex(uint8_t d, uint8_t c);
uint8_t  sn_getCoord(uint8_t d, uint8_t i);
uint8_t  sn_getSize(uint8_t d, uint8_t i);
uint8_t  sn_getMax(uint8_t d, uint8_t i);

dim_t    sn_getAnchor(dim_t n);

int      sn_split(uint8_t pd, uint8_t pi);

void     sn_merge(uint8_t pd, uint8_t pi, uint8_t pc);
int      sn_mergeBlockFaceCheck(region_t *box);
int      sn_mergeBlockDetection(dim_t l);
int      sn_mergeCheck(uint8_t pd, uint8_t pn, dim_t l);
int      sn_mergeCheckAll(dim_t l);

void     sn_injectFault(dim_t l);
int      lnk_injectFault(dim_t l);

void     sng_printLatex();

#endif
