// FooBar
//
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <spi/include/kernel/MU.h>
#include <hwi/include/bqc/MU_Macros.h>

#define CHECK_FAIL(str) if(rc) { fprintf(stderr, "error %d, %s, while %s\n", rc, strerror(rc), str); exit(-1); }

void print_free_fifo_ids(uint32_t num_free_fifos, uint32_t free_fifo_ids[],
	uint32_t max_id) {

    int j = 0;
    int i;

    for(i = 0; i <= max_id; i++) {
	if( j < num_free_fifos && free_fifo_ids[j] == i) {
	    printf("%u ", i);
	    j++;
	} else {
	    printf("  ");
	}
    }

}

int main( int argc, char *argv[] ) {
    int j;
    int rc;

    // query injection Fifos first
    uint32_t num_free_fifos;
    uint32_t free_fifo_ids[BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP];
    uint32_t free_bat_ids[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];

    for(j=0; j < BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE; j++) {
    	rc = Kernel_QueryInjFifos( j, &num_free_fifos, free_fifo_ids);
          CHECK_FAIL("calling Kernel_QueryInjFifos");
            printf("subgroup %02d: %u free INJ FIFOs  ", j, num_free_fifos);

	print_free_fifo_ids(num_free_fifos, free_fifo_ids, 7);

    	rc = Kernel_QueryRecFifos( j, &num_free_fifos, free_fifo_ids);
          CHECK_FAIL("calling Kernel_QueryRecFifos");
    	printf("   %u free REC FIFOs ", num_free_fifos);

	print_free_fifo_ids(num_free_fifos, free_fifo_ids, 3);

          rc = Kernel_QueryBaseAddressTable( j, &num_free_fifos, free_bat_ids);
          CHECK_FAIL("calling Kernel_QueryBaseAddressTable");
    	printf("   %u free BAT entries ", num_free_fifos);

	print_free_fifo_ids(num_free_fifos, free_fifo_ids, 3);
          printf("\n");
    }

    return 0;
}

