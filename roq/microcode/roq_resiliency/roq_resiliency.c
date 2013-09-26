#include "roq_resiliency.h"

#include <hwi/include/bqc/nd_rese_dcr.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <hwi/include/bqc/BIC.h>
#include <hwi/include/bqc/BIC_inlines.h>

config_t config;

volatile uint16_t lcl_badLinks = 0;

linkList_t gbl_badLinksNew; // list of all bad links present in the system.

rr_global_vars_t global; // all global variables

static MUSPI_GI_t GI[1];

void wakeupThread(unsigned int pid) {

  unsigned int cid = pid >> 2;         // core ID (0-16)
  unsigned int tid = (pid &  0x3) + 1; // thread ID (1-3)

  BIC_REGISTER packet = ((_B1(42, 1) >> cid )                | // core, bits 42:58
						 (_B2(60, BIC_C2C_INTTYPE_EXTERNAL)) | // interrupt type, bits 59:60 // BIC_C2C_INTTYPE_WAKE
						 (_B3(63, tid)));                      // target thread, bits 61:63

  BIC_WriteInterruptSend(packet);
}

void shutdownLink(uint8_t i) {
/**
 * Michael,
 *
 *		please do the following sequence for each RE/SE unit that you want to shutdown:
 *
 * 1. set "re_link_stop" to '1'.
 *
 * 2. set "se_drop_pkt_usr" and "se_drop_pkt_sys" to '1'.
 *
 *	3. set the following resets to '1', then to '0'.
 *	se, se_tok0, se_tok1, se_tok2, se_tok3, se_tok4, se_tok5
 *
 * Dong
 */

	uint64_t dcrval;

	// switch off interrupts or linux might crash
	dcrval  = DCRReadPriv(ND_RESE_DCR(i, FATAL_ERR_ENABLE));
	dcrval &= ~(ND_RESE_DCR__FATAL_ERR__SE_RETRANS_CNT_OVER_THRESH_set(1) |
				ND_RESE_DCR__FATAL_ERR__SE_DROPPED_PKT_set(1));
	DCRWritePriv(ND_RESE_DCR(i, FATAL_ERR_ENABLE), dcrval);

	dcrval  = DCRReadPriv(ND_RESE_DCR(i, CTRL));
	dcrval |= (ND_RESE_DCR__CTRL__RE_LINK_STOP_set(1));
	DCRWritePriv(ND_RESE_DCR(i, CTRL), dcrval);

	dcrval |= (ND_RESE_DCR__CTRL__SE_DROP_PKT_USR_set(1) |
			   ND_RESE_DCR__CTRL__SE_DROP_PKT_SYS_set(1));
	DCRWritePriv(ND_RESE_DCR(i, CTRL), dcrval);

	dcrval  = DCRReadPriv(ND_RESE_DCR(i, RESET));
	dcrval |= (ND_RESE_DCR__RESET__SE_set(1)      |
			   ND_RESE_DCR__RESET__SE_TOK0_set(1) |
			   ND_RESE_DCR__RESET__SE_TOK1_set(1) |
			   ND_RESE_DCR__RESET__SE_TOK2_set(1) |
			   ND_RESE_DCR__RESET__SE_TOK3_set(1) |
			   ND_RESE_DCR__RESET__SE_TOK4_set(1) |
			   ND_RESE_DCR__RESET__SE_TOK5_set(1));
	DCRWritePriv(ND_RESE_DCR(i, RESET), dcrval);

	dcrval &= ~(ND_RESE_DCR__RESET__SE_set(1)      |
				ND_RESE_DCR__RESET__SE_TOK0_set(1) |
				ND_RESE_DCR__RESET__SE_TOK1_set(1) |
				ND_RESE_DCR__RESET__SE_TOK2_set(1) |
				ND_RESE_DCR__RESET__SE_TOK3_set(1) |
				ND_RESE_DCR__RESET__SE_TOK4_set(1) |
				ND_RESE_DCR__RESET__SE_TOK5_set(1));
	DCRWritePriv(ND_RESE_DCR(i, RESET), dcrval);

	// TODO: Send RAS message to the control system about the failed link.
}

void * RoQ_Resiliency_Checker(void *args) {
  INFO("Starting RoQ Resiliency Checker\n");

  //Checker_Msg(); // this one is kinda broken. Only works for rather large config.delay values.
  Checker_GlobalInterrupts();

  while(1) {}
}

void * RoQ_Resiliency_Notifier(void *args) {
  INFO("Starting RoQ Resiliency Notifier\n");

  Notifier_Peer2Peer();

  while(1) {}
}

void * RoQ_Resiliency_Forwarder(void *args) {
  INFO("Starting RoQ Resiliency Forwarder\n");

  Forwarder();

  while(1) {}
}

void * RoQ_Resiliency_Router(void *args) {
  INFO("Starting RoQ Resiliency Router\n");

  Router();

  while(1) {}
}

void * RoQ_Resiliency_Init(void *args) {
	DPRINTF(FXLOG_UCODE_INIT, "RoQ Resiliency Built %s\n", __TIMESTAMP__);

	DSTRONG_ASSERT(args, "thread args pointer NULL %p\n", args);
	struct RoQ_Alife_Checker_Args_t *AcArgs = (struct RoQ_Alife_Checker_Args_t *) args;
	
	// config values for the gi checker/failure detection module
	config.detection.init   = 1000;
	config.detection.max    = 3;
	config.detection.period = 50000; // 50ms

	// config values for the failure broadcasting module
	config.broadcast.period = 10000;  // 10ms
	config.broadcast.wait   = 100000; // 100ms
	
	// config values for the forwarder module
	config.forwarder.period = 10000;  // 10ms
	config.forwarder.wait   = 2000000; // 2000ms

    // config values for the router module
	config.router.wait      = 100000; // 100ms

	global.setupLock = 0;

	// Allow threads 1-3 (= all resiliency cores) on this core to sleep if they want to.
	mtspr(SPRN_CCR0, CCR0_WEM(0x7));
	
	// global link list stores all bad links in the partition.
	gbl_badLinksNew.size = 128;
	gbl_badLinksNew.head = 0;
	gbl_badLinksNew.lock = 0;
	
	gbl_badLinksNew.list = malloc(sizeof(dim_t)*gbl_badLinksNew.size);
	bzero(gbl_badLinksNew.list, sizeof(dim_t)*gbl_badLinksNew.size);
	
	global.freqMhz = personality->Kernel_Config.FreqMHz;


	uint64_t dcrval = 0;
	
	// Getting my coordinates
	dcrval = DCRReadPriv(ND_500_DCR(CTRL_COORDS));
	
	global.coords.A = ND_500_DCR__CTRL_COORDS__NODE_COORD_A_get(dcrval);
	global.coords.B = ND_500_DCR__CTRL_COORDS__NODE_COORD_B_get(dcrval);
	global.coords.C = ND_500_DCR__CTRL_COORDS__NODE_COORD_C_get(dcrval);
	global.coords.D = ND_500_DCR__CTRL_COORDS__NODE_COORD_D_get(dcrval);
	global.coords.E = ND_500_DCR__CTRL_COORDS__NODE_COORD_E_get(dcrval);

	// Getting the dimension of the partition
	dcrval = DCRReadPriv(ND_500_DCR(CTRL_COORDS));
	
	global.nodes.A = ND_500_DCR__CTRL_COORDS__MAX_COORD_A_get(dcrval)+1;
	global.nodes.B = ND_500_DCR__CTRL_COORDS__MAX_COORD_B_get(dcrval)+1;
	global.nodes.C = ND_500_DCR__CTRL_COORDS__MAX_COORD_C_get(dcrval)+1;
	global.nodes.D = ND_500_DCR__CTRL_COORDS__MAX_COORD_D_get(dcrval)+1;
	global.nodes.E = ND_500_DCR__CTRL_COORDS__MAX_COORD_E_get(dcrval)+1;

	
	global.dimensions = ((global.nodes.A > 1) + 
						 (global.nodes.B > 1) + 
						 (global.nodes.C > 1) + 
						 (global.nodes.D > 1) +
						 (global.nodes.E > 1));
	
	global.nodeCount  = (global.nodes.A *
						 global.nodes.B *
						 global.nodes.C *
						 global.nodes.D *
						 global.nodes.E);
	INFO("There are %llu nodes in this system\n", global.nodeCount);
	
	// Compiling a list of direct neighbors.
	// List of 10 neighbors. NULL means there is no neighbor in dis dimension/direction.
	bzero(global.neighbors, sizeof(dim_t*)*MAX_DIR);
	
	INFO("RoQ Resiliency running on node (%u,%u,%u,%u,%u). Torus dimensions are (%u,%u,%u,%u,%u)\n",
		 global.coords.A, global.coords.B, global.coords.C, global.coords.D, global.coords.E,
		 global.nodes.A, global.nodes.B, global.nodes.C, global.nodes.D, global.nodes.E);

	// neighbor coordinates
	dcrval = DCRReadPriv(ND_500_DCR(CTRL_NEIGHBOR_COORDS));
	INFO("ctrl_neighbor_coords = 0x%016x\n", dcrval);

	if (global.nodes.A > 1) {
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_A_MINUS) {
			global.neighbors[AM]  = malloc(sizeof(dim_t));
			*global.neighbors[AM] = global.coords;
			global.neighbors[AM]->dim[0] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_MINUS_get(dcrval);
		}
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_A_PLUS) {
			global.neighbors[AP]  = malloc(sizeof(dim_t));
			*global.neighbors[AP] = global.coords;
			global.neighbors[AP]->dim[0] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__A_PLUS_get(dcrval);
		}
	}

	if (global.nodes.B > 1) {
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_B_MINUS) {
			global.neighbors[BM]  = malloc(sizeof(dim_t));
			*global.neighbors[BM] = global.coords;
			global.neighbors[BM]->dim[1] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_MINUS_get(dcrval);
		}
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_B_PLUS) {
			global.neighbors[BP]  = malloc(sizeof(dim_t));
			*global.neighbors[BP] = global.coords;
			global.neighbors[BP]->dim[1] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__B_PLUS_get(dcrval);
		}
	}

	if (global.nodes.C > 1) {
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_C_MINUS) {
			global.neighbors[CM]  = malloc(sizeof(dim_t));
			*global.neighbors[CM] = global.coords;
			global.neighbors[CM]->dim[2] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_MINUS_get(dcrval);
		}
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_C_PLUS) {
			global.neighbors[CP]  = malloc(sizeof(dim_t));
			*global.neighbors[CP] = global.coords;
			global.neighbors[CP]->dim[2] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__C_PLUS_get(dcrval);
		}
	}

	if (global.nodes.D > 1) {
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_D_MINUS) {
			global.neighbors[DM]  = malloc(sizeof(dim_t));
			*global.neighbors[DM] = global.coords;
			global.neighbors[DM]->dim[3] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_MINUS_get(dcrval);
		}
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_D_PLUS) {
			global.neighbors[DP]  = malloc(sizeof(dim_t));
			*global.neighbors[DP] = global.coords;
			global.neighbors[DP]->dim[3] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__D_PLUS_get(dcrval);
		}
	}

	if (global.nodes.E > 1) {
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_E_MINUS) {
			global.neighbors[EM]  = malloc(sizeof(dim_t));
			*global.neighbors[EM] = global.coords;
			global.neighbors[EM]->dim[4] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_MINUS_get(dcrval);
		}
		if (personality->Network_Config.NetFlags2 & ND_ENABLE_LINK_E_PLUS) {
			global.neighbors[EP]  = malloc(sizeof(dim_t));
			*global.neighbors[EP] = global.coords;
			global.neighbors[EP]->dim[4] = ND_500_DCR__CTRL_NEIGHBOR_COORDS__E_PLUS_get(dcrval);
		}
	}

	uint32_t d = 0;
	for (d = 0; d < MAX_DIR; d++) {
		if (global.neighbors[d] != NULL) {
			INFO("%s (%d,%d,%d,%d,%d)\n", linkNum2Str[d], global.neighbors[d]->A, global.neighbors[d]->B, global.neighbors[d]->C, global.neighbors[d]->D, global.neighbors[d]->E);
		} else {
			DEBUG("%s no neighbor\n", linkNum2Str[d]);
		}
	}

	return NULL;
}
