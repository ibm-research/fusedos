#ifndef __SNG_ROUTE__
#define __SNG_ROUTE__

#include "sng_common.h"
 
#define COST_HOP             1
#define COST_TURN            0
#define COST_UNNATURAL_TURN  5
#define COST_WRAPAROUND      0

void sng_printRoutes();

void sng_routeCopy(uint8_t pd, uint8_t oc, uint8_t cc);
void sng_routeSetDefaults(uint8_t pd, uint8_t pc);

void sng_route();
void sng_routeSetup();

void sng_routeApply(dim_t ni, uint32_t nir);
void sng_routeFindCutOffNodes();

void sng_routePrintAll();
void sng_routeCheck();

int  sng_getLinkStatus(dim_t nc, dim_t ni, uint8_t dir);


typedef struct {
    dim_t   dim;
    uint8_t cost;
} node_element_t;

typedef struct {
  node_element_t *heap;
  uint32_t        size;
  uint32_t        next;
} node_heap_t;

typedef struct {
  region_t *stack;
  uint32_t  size;
  uint32_t  head;
} stack_region_t;

void heap_init(node_heap_t *heap, uint32_t size);
void heap_reset(node_heap_t *heap);
void heap_insert(node_heap_t *heap, node_element_t element);
uint32_t heap_find(node_heap_t *heap, dim_t ni);
void heap_decreaseKey(node_heap_t *heap, uint32_t loc, uint8_t cost);
node_element_t heap_getMin(node_heap_t *heap);
void heap_dump(node_heap_t *heap);

void sng_routeReset();
#endif
 
