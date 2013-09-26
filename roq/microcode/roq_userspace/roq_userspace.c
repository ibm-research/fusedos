#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sched.h>

uint64_t PCS;
int __gxx_personality_v0;
void RoQ_MC_Init();

/* ROQ START AND THREAD */
int main(void)
{
	int nrcpus = sysconf(_SC_NPROCESSORS_CONF);
	printf("USERSPACE ROQ (%d)\n", nrcpus); fflush(stdout);
	daemon(1,1);
	RoQ_MC_Init();
}
