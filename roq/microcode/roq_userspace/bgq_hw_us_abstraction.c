#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/eventfd.h>
#include <asm/types.h>
#include <pthread.h>
#include <bgq_hw_abstraction.h>
#include <errno.h>

#include <hwi/include/bqc/A2_core.h>
#include <hwi/include/bqc/A2_inlines.h>

#include <spi/include/mu/Addressing.h>
#include <spi/include/mu/Addressing_inlines.h>
#include <spi/include/kernel/MU.h>
#include <spi/include/mu/Descriptor.h>
#include <spi/include/mu/Descriptor_inlines.h>
#include <spi/include/mu/InjFifo.h>
#include <spi/include/mu/RecFifo.h>

#include <transcon.h>

struct roq_dma_init_t {
	uint32_t nports;
	uint32_t port_ids[2]; // FIXME: should be the size of nports!!!
	uint64_t ret;
};

/**** STATICS ***********************************************************/
static int fd, efd;
struct roq_us_map_t *roq_us_map;
uint64_t *dmabuf;

/**** FUNCTIONS *********************************************************/
void *
ROQ_RecFifo(int portid)
{
	return (void*)MUSPI_IdToRecFifo(portid, &roq_us_map->RecFifoSubgroups[0]);
}

void
ROQ_set_affinity(int m)
{
	int rc;
	cpu_set_t *mask;
	size_t size;
	struct sched_param param;
	int nrcpus = sysconf(_SC_NPROCESSORS_CONF);

	mask = CPU_ALLOC(nrcpus);
	size = CPU_ALLOC_SIZE(nrcpus);
	CPU_ZERO_S(size, mask);

	switch (m) {
	case 0: /* init mode pin to core0 */
		CPU_SET(0, mask);
		break;
	case 1: /* running mode pin so more cores */
		//CPU_SET(54, mask);
		CPU_SET(55, mask);
		CPU_SET(56, mask);
		CPU_SET(57, mask);
		break;
	default:
		CPU_SET(m, mask);
	}

	#ifdef ROQ_USE_PTHREAD
	if (pthread_setaffinity_np(pthread_self(), size, mask) == -1) {
	        printf("ERROR: cannot set pthread_setaffinity_np\n");
	}
	#else
	if (sched_setaffinity(0, size, mask) == -1) {
	        printf("ERROR: cannot set sched_setaffinity\n");
	}
	#endif

	memset(&param, 0, sizeof(struct sched_param));
	param.sched_priority = sched_get_priority_max(SCHED_OTHER);
	#ifdef ROQ_USE_PTHREAD
	if (rc = pthread_setschedparam(pthread_self(), SCHED_OTHER, &param)) {
		printf("ERROR: cannot set scheduler (%d) %s\n", rc, strerror(errno));
	}
	#else
	if ((rc = sched_setscheduler(0, SCHED_OTHER, &param))) {
		printf("ERROR: cannot set scheduler (%d) %s\n", rc, strerror(errno));
	}
	#endif
	#if 0
	sched_getaffinity(0, size, mask);
	for ( rc = 0; rc < nrcpus; rc++ ) {
		if ( CPU_ISSET_S(rc, size, mask) ) {
			printf("CPU %d is set\n", (rc+1));
		}
	}
	#endif
	CPU_FREE(mask);
}

int
ROQ_dma_init(uint32_t *port_ids, int32_t nports)
{
	int rc;
	struct roq_dma_init_t vdi;
	vdi.nports = nports;
	memcpy(vdi.port_ids, port_ids, nports * sizeof(uint32_t));
	int i;
	void *rec_memory_fifo;
	Kernel_MemoryRegion_t *rmregion;
	void *inj_memory_fifo;
	Kernel_MemoryRegion_t *imregion;
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup;
	Kernel_RecFifoAttributes_t *recFifoAttrs;
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup;
	Kernel_InjFifoAttributes_t *injFifoAttrs;

	fd = open("/dev/roq_usmu", O_RDWR);
	if (fd == -1) {
		printf("ERROR: open file /dev/roq_usmu\n");
		return -1;
	}

	/* kernel side dma init */
	rc = ioctl(fd, 1, &vdi);
	if (rc) {
		printf("ERROR: ioctl %d\n", rc);
		return -1;
	}

	/* roq.cpp maps the return value already */
	roq_us_map = (struct roq_us_map_t*)(vdi.ret|ROQ_VIRT_CACHE_OFF);

	if (roq_us_map->magic != 0x2355aa42) {
		printf("ERROR: roq_us_map magic wrong (%x)\n", roq_us_map->magic);
		return -1;
	}
	dmabuf = roq_us_map->dmabuf;

#if 0
	printf("REC: %p %p\n", roq_us_map->rec_memory_fifos[0], roq_us_map->rec_memory_fifos[1]);
	printf("REC: %p %p\n", roq_us_map->alloc_rec_bufs[0], roq_us_map->alloc_rec_bufs[1]);
	printf("REC: %llx %llx\n", roq_us_map->rec_dmabuf[0], roq_us_map->rec_dmabuf[1]);
	printf("INJ: %p %p\n", roq_us_map->inj_memory_fifos[0], roq_us_map->inj_memory_fifos[1]);
	printf("INJ: %p %p\n", roq_us_map->alloc_inj_bufs[0], roq_us_map->alloc_inj_bufs[1]);
	printf("INJ: %llx %llx\n", roq_us_map->inj_dmabuf[0], roq_us_map->inj_dmabuf[1]);
#endif

	/* Receive side init */
	RecFifoSubgroup = &roq_us_map->RecFifoSubgroups[0];

	for (i=0; i < nports; i++) {
		rmregion = &roq_us_map->rmregions[i];

		rmregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->rec_dmabuf[i] | ROQ_VIRT_UNCACHE_OFF);
		rmregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->rec_dmabuf[i]);
		rmregion->Bytes  = REC_MEMORY_FIFO_SIZE;

		rc = Kernel_RecFifoInit( RecFifoSubgroup, port_ids[i], rmregion, 0, REC_MEMORY_FIFO_SIZE);
	}

	/* Injection side init */
	InjFifoSubgroup = &roq_us_map->InjFifoSubgroups[0];

	for (i=0; i < nports; i++) {
		imregion = &roq_us_map->imregions[i];

		imregion->BaseVa = (uint8_t*)((uint64_t)roq_us_map->inj_dmabuf[i] | ROQ_VIRT_UNCACHE_OFF);
		imregion->BasePa = (uint8_t*)((uint64_t)roq_us_map->inj_dmabuf[i]);
		imregion->Bytes  = INJ_MEMORY_FIFO_SIZE+1;

		Kernel_InjFifoInit(InjFifoSubgroup, port_ids[i], imregion, 0, INJ_MEMORY_FIFO_SIZE);
	}

	rc = Kernel_InjFifoActivate(InjFifoSubgroup, nports, &port_ids[0], KERNEL_INJ_FIFO_ACTIVATE);

	/* keep file descriptor for later use  */
	//close(fd);
	//
	return 0;
}

int
ROQ_ion_init(uint32_t *port_ids, int32_t nports)
{
	return 0;
}

int
ROQ_activate()
{
	return ioctl(fd, 2, 0);
}

int
ROQ_deactivate()
{
	return ioctl(fd, 2, 1);
}

int
ROQ_Send(uint32_t iolink, uint32_t SendPortId, void* MUHWI_DescPtr)
{
	int rc;
	//ioctl(fd, 3, 0); // DCR display
	//ioctl(fd, 3, 1); // SRAM display
	//ioctl(fd, 3, (unsigned long)MUHWI_DescPtr); // injection descriptor display
#if 0
	MUHWI_Descriptor_t *desc        = (MUHWI_Descriptor_t*)MUHWI_DescPtr;
	MUHWI_PacketHeader_t *h         = &(desc->PacketHeader);
	MUHWI_Pt2PtNetworkHeader_t *p2p = &(h->NetworkHeader.pt2pt);
	MUHWI_MessageUnitHeader_t *muh  = &(h->messageUnitHeader);
	MUHWI_Destination_t d           = p2p->Destination;

	printf("<%d> iolink=%d SendPortId=%d MUHWI_DescPtr=%p\n", SendPortId, iolink, SendPortId, MUHWI_DescPtr);
	printf("<%d> hw0=0x%08x hw1=0x%08x Pa=0x%016llx len=0x%016llx map=0x%016llx\n",
	        SendPortId, desc->Half_Word0.Half_Word0, desc->Half_Word1.Half_Word1, desc->Pa_Payload,
	        desc->Message_Length, desc->Torus_FIFO_Map) ;

	printf("<%d> type=0x%02x hints=0x%02x B2=0x%02x B3=0x%02x dest=0x%08x B8=0x%02x R4=0x%04x skip=0x%02x FIFOId=0x%04x POM=0x%04x POL=0x%08x CounterOffset=0x%016llx\n",
            SendPortId, p2p->Data_Packet_Type, p2p->Hints, p2p->Byte2.Byte2, p2p->Byte3.Byte3, d.Destination.Destination, p2p->Byte8.Byte8, p2p->Injection_Info.Reserved4,
            p2p->Injection_Info.Skip, muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id, muh->Packet_Types.Memory_FIFO.Put_Offset_MSB, muh->Packet_Types.Memory_FIFO.Put_Offset_LSB,
            muh->Packet_Types.Direct_Put.Counter_Offset);

#if 0
	for (rc=0; rc < desc->Message_Length/4 ; rc+=2) {
		printf("SEND %p: %x %x", &((uint32_t*)(ROQ_VIRT_CACHE_OFF|desc->Pa_Payload))[rc],
				((uint32_t*)(ROQ_VIRT_CACHE_OFF|desc->Pa_Payload))[rc],
				((uint32_t*)(ROQ_VIRT_CACHE_OFF|desc->Pa_Payload))[rc+1]);
	}
#endif

#endif
	MUSPI_InjFifo_t *injfifo;
	injfifo = MUSPI_IdToInjFifo(SendPortId, &roq_us_map->InjFifoSubgroups[0]);

	while ((rc = MUSPI_InjFifoInject(injfifo, (MUHWI_Descriptor_t*)MUHWI_DescPtr)) == -1);;

	//while (MUSPI_CheckDescComplete(MUSPI_IdToInjFifo(SendPortId, &roq_us_map->InjFifoSubgroups[0]), rc) == 0);;

	//ioctl(fd, 3, 0); // DCR display
	//ioctl(fd, 3, 1); // SRAM display
	//ioctl(fd, 3, (unsigned long)MUHWI_DescPtr); // injection descriptor display
	return rc;
}

int
ROQ_interrupt()
{
	int rc;
	while ((rc = ioctl(fd, 0x42, 0)) != 0);
}
