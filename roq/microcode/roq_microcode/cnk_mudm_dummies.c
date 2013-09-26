/*
 * Dummy functions to keep the linker happy
 */
#include <stdio.h>
int mudm_bcast_RDMA_write(void) { printf("%s",__func__) ; return 0 ; }
int mudm_bcast_reduce(void) { printf("%s",__func__) ; return 0 ; }
int mudm_remoteget_load_atomic(void) { printf("%s",__func__) ; return 0 ; }
int mudm_directput_store_atomic(void) { printf("%s",__func__) ; return 0 ; }
int mudm_rdma_read_on_torus(void) { printf("%s",__func__) ; return 0 ; }
int mudm_rdma_write_on_torus(void) { printf("%s",__func__) ; return 0 ; }

int pthread_create(void) { return 0 ; }
