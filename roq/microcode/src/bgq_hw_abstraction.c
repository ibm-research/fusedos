#include <stdlib.h>
#include <stdint.h>
#include <asm/types.h>
#include <bgq_hw_abstraction.hpp>
#include <FxLogger.hpp>

#define REC_MEMORY_FIFO_SIZE	((20 * 1024 * 1024)-1)
#define INJ_MEMORY_FIFO_SIZE	((20 * 1024 * 1024)-1)

#ifndef FXLOG_UCODE_MU_HEADER
#define FXLOG_UCODE_MU_HEADER 1
#endif

/**** STATICS ***********************************************************/
static MUSPI_RecFifoSubGroup_t RecFifoSubgroups[2];
static void **rec_memory_fifos;
static void **alloc_rec_bufs;
static Kernel_MemoryRegion_t *rmregions;

static MUSPI_InjFifoSubGroup_t InjFifoSubgroups[2];
static void **inj_memory_fifos;
static void **alloc_inj_bufs;
static Kernel_MemoryRegion_t *imregions;

/**** FUNCTIONS *********************************************************/
static int
malloc_memalign(void **memory, void **buffer, uint64_t alignment, uint32_t size)
{
	/* Malloc the memory to be the size of the buffer plus space for alignment. */
	void *myMemory = malloc ( size + alignment );

	if (!myMemory)
		return -1;

	*memory = myMemory;
	*buffer = (void*)( ((uint64_t)myMemory + alignment) & ~(alignment - 1));

	return 0;
}

int
ROQ_dma_init(uint32_t* port_ids, uint32_t nports)
{
	int rc, i = 0;
	uint64_t recFifoEnableBits;

	void *rec_memory_fifo;
	void *alloc_rec_buf;
	Kernel_MemoryRegion_t *rmregion;
	void *inj_memory_fifo;
	void *alloc_inj_buf;
	Kernel_MemoryRegion_t *imregion;

	/* allocate all needed stuff */
	rec_memory_fifos = (void**)malloc(nports * sizeof(void*));
	alloc_rec_bufs	 = (void**)malloc(nports * sizeof(void*));
	rmregions	 = (Kernel_MemoryRegion_t*)malloc(nports * sizeof(Kernel_MemoryRegion_t));

	inj_memory_fifos = (void**)malloc(nports * sizeof(void*));
	alloc_inj_bufs	 = (void**)malloc(nports * sizeof(void*));
	imregions	 = (Kernel_MemoryRegion_t*)malloc(nports * sizeof(Kernel_MemoryRegion_t));

	/* allocate reception fifo */
	MUSPI_RecFifoSubGroup_t *RecFifoSubgroup = &RecFifoSubgroups[0];

	Kernel_RecFifoAttributes_t *recFifoAttrs =
		(Kernel_RecFifoAttributes_t*)malloc(nports*sizeof(Kernel_RecFifoAttributes_t));

	for (i = 0; i < nports; i++)
		recFifoAttrs[i].System = 0;

	rc = Kernel_AllocateRecFifos(0, RecFifoSubgroup, nports, &port_ids[0], recFifoAttrs);

	free(recFifoAttrs);

	for (i = 0; i < nports; i++) {
		rec_memory_fifo = rec_memory_fifos[i];
		alloc_rec_buf = alloc_rec_bufs[i];
		rmregion = &rmregions[i];

		rc = malloc_memalign(&alloc_rec_buf, &rec_memory_fifo, 32, REC_MEMORY_FIFO_SIZE+1 );
		if (rc) {
			printf("rec_memory_fifo malloc failed with rc %d\n",rc);
			exit(1);
		}
		Kernel_CreateMemoryRegion(rmregion, rec_memory_fifo, REC_MEMORY_FIFO_SIZE);

		rc = Kernel_RecFifoInit(RecFifoSubgroup, port_ids[i], rmregion,
					(uint64_t)rec_memory_fifo - (uint64_t)rmregion->BaseVa,
					REC_MEMORY_FIFO_SIZE);
	}

	recFifoEnableBits = 0;
	for (i = 0; i < nports; i++)
		recFifoEnableBits |= (0x0000000000000001ULL << (15 - port_ids[i]));

	rc = Kernel_RecFifoEnable(0, recFifoEnableBits);

	/* allocate injection fifo */
	MUSPI_InjFifoSubGroup_t *InjFifoSubgroup = &InjFifoSubgroups[0];
	Kernel_InjFifoAttributes_t *injFifoAttrs =
		(Kernel_InjFifoAttributes_t *)malloc(nports * sizeof(Kernel_InjFifoAttributes_t));

	for (i = 0; i < nports; i++) {
		injFifoAttrs[i].RemoteGet = 0;
		injFifoAttrs[i].System	  = 0;
	}

	rc = Kernel_AllocateInjFifos(0, InjFifoSubgroup, nports, port_ids, injFifoAttrs);

	free(injFifoAttrs);

	for (i = 0; i < nports; i++) {
		inj_memory_fifo = inj_memory_fifos[i];
		alloc_inj_buf = alloc_inj_bufs[i];
		imregion = &imregions[i];

		rc = malloc_memalign(&alloc_inj_buf, &inj_memory_fifo, 64, INJ_MEMORY_FIFO_SIZE+1 );
		if (rc) {
			printf("inj_memory_fifo malloc failed with rc %d\n",rc);
			exit(1);
		}

		Kernel_CreateMemoryRegion(imregion, inj_memory_fifo, INJ_MEMORY_FIFO_SIZE+1);

		Kernel_InjFifoInit(InjFifoSubgroup, port_ids[i], imregion,
				   (uint64_t)inj_memory_fifo - (uint64_t)imregion->BaseVa,
				   INJ_MEMORY_FIFO_SIZE);
	}
	rc = Kernel_InjFifoActivate(InjFifoSubgroup, nports, &port_ids[0], KERNEL_INJ_FIFO_ACTIVATE);
}

static void display_injection_descriptor(MUHWI_Descriptor_t *desc)
{
	MUHWI_PacketHeader_t *h=&(desc->PacketHeader) ;
	MUHWI_Pt2PtNetworkHeader_t *p2p=&(h->NetworkHeader.pt2pt) ;
	MUHWI_MessageUnitHeader_t *muh=&(h->messageUnitHeader) ;
	MUHWI_Destination_t d=p2p->Destination ;
	(,
	    "%lx",
	    (uint64_t)BegLogLine ( FXLOG_UCODE_MU_HEADER ));
	(,
	    "%lx %s %ld %s %ld %s %ld %s %lx %s %ld",
	    (uint64_t)BegLogLine ( FXLOG_UCODE_MU_HEADER ), "hw0=", (void*) (desc->Half_Word0.Half_Word0),
	    " hw1=", (void*) (desc->Half_Word1.Half_Word1), " Pa=", (void*) (desc->Pa_Payload),
	    " len=", (uint64_t)desc->Message_Length, " map=", (void*) (desc->Torus_FIFO_Map));

	(,
	    "%lx %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld %s %ld",
	    (uint64_t)BegLogLine ( FXLOG_UCODE_MU_HEADER ), " type=", (void*) (p2p->Data_Packet_Type),
	    " hints=", (void*) (p2p->Hints), " B2=", (void*) (p2p->Byte2.Byte2),
	    " B3=", (void*) (p2p->Byte3.Byte3), " dest=", (void*) (d.Destination.Destination),
	    " B8=", (void*) (p2p->Byte8.Byte8), " R4=", (void*) (p2p->Injection_Info.Reserved4),
	    " skip=", (void*) (p2p->Injection_Info.Skip), " FIFOId=", (void*) (muh->Packet_Types.Memory_FIFO.Rec_FIFO_Id),
	    " POM=", (void*) (muh->Packet_Types.Memory_FIFO.Put_Offset_MSB), " POL=",
	    (void*) (muh->Packet_Types.Memory_FIFO.Put_Offset_LSB));
}

static void show_mu_dcr(unsigned int x, uint64_t v)
{
	DPRINTF(1,
	    "%s %ld %s %ld %s",
	    " dcr[ mu+0x", (void *)x, " ]= 0x", (void *)v, " ");
}
#define DC(X) show_mu_dcr(X,dcrpriv[0x0e0000+X])
static void show_nd_dcr(unsigned int x, uint64_t v)
{
	DPRINTF(1,
	    "%s %ld %s %ld %s",
	    " dcr[ nd+0x", (void *)x, " ]= 0x", (void *)v, " ");
}
#define DCN(X) show_mu_dcr(X,dcrpriv[0x0e0000+X])
static void display_dcrs(void)
{
	unsigned int x ;
	volatile uint64_t *dcrpriv=(volatile uint64_t *)BGQ_MU_DCR_MEM_MAPPED_START_OFFSET() ;
	for(x=0xd00;x<0xd7e;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	for(x=0xd80;x<0xdba;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	for(x=0xdc7;x<0xdd7;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	for(x=0xddd;x<0xdf7;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	DC(0xe12) ;
	DC(0xe1b) ;
	DC(0xe24) ;
	DC(0xe2d) ;
	DC(0xe36) ;
	DC(0xe3f) ;
	DC(0xe50) ;
	DC(0xe51) ;
	DC(0xe6a) ;
	for(x=0xe7b;x<0xe8b;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	for(x=0xe9b;x<0xebb;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	DC(0xec3) ;
	DC(0xecc) ;
	for(x=0xf00;x<0xf28;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	for(x=0xf41;x<0xf45;x+=1) show_mu_dcr(x,dcrpriv[0x0e0000+x]) ;
	/* Show the 'ND' DCRs as well */
	for(x=0x00;x<0x1c;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ;
//	DCN(0x21) ;  /* This one traps  */
	for(x=0x23;x<0x2d;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* OK */
//	DCN(0x2e) ; /* This one traps  */
	for(x=0x30;x<0x3c;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* OK */
//	DCN(0x46) ; /* This one traps  */
	for(x=0x4f;x<0x56;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* OK */
//	DCN(0x60) ; /* Not cheecked */
	for(x=0x70;x<0x7c;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* OK */
//	for(x=0x80;x<0x88;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* This one traps  */
//	for(x=0x90;x<0x9a;x+=1) show_nd_dcr(x,dcrpriv[0x194000+x]) ; /* This one traps  */
}

static void show_mu_sram(void)
{
	uint64_t *mu_sram_base=(uint64_t *)_BGQ_BASE_DMA ;
	unsigned int x;
	DPRINTF(1,
	    "%s",
	    "() entry");
	for(x=0;x<_BGQ_SIZE_DMA/sizeof(uint64_t);x+=4)
		{
			uint64_t v0=mu_sram_base[x] ;
			uint64_t v1=mu_sram_base[x+1] ;
			uint64_t v2=mu_sram_base[x+2] ;
			uint64_t v3=mu_sram_base[x+3] ;
			unsigned int bx=x*sizeof(uint64_t) ;
			if(v0|v1|v2|v3)
				{
					DPRINTF(1,
					    "%s %ld %s %ld %s %ld %s %ld %s %ld %s",
					    " ", (void *) bx, " [ ", (void *) v0, " ", (void *) v1, " ", (void *) v2,
					    " ", (void *) v3, " ]");
				}
		}
	DPRINTF(1,
	    "%s",
	    "() exit");
}

int
ROQ_Send(uint32_t SendPortId, void* MUHWI_DescPtr)
{
	MUHWI_Descriptor_t* desc = (MUHWI_Descriptor_t*)MUHWI_DescPtr;
	if(0) display_injection_descriptor(desc);
	if(0) display_dcrs();
	if(0) show_mu_sram();
	while ((MUSPI_InjFifoInject(MUSPI_IdToInjFifo(SendPortId, &InjFifoSubgroups[0]), desc)) == -1);;
	if(0) display_dcrs();
	if(0) show_mu_sram();
	return 0;
}

void
ROQ_set_affinity(int m)
{
	return;
}

void *
ROQ_RecFifo(int portid)
{
	return (void*)MUSPI_IdToRecFifo(portid, &RecFifoSubgroups[0]);
}
