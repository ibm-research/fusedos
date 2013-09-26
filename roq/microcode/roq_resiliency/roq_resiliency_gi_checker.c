#include "roq_resiliency.h"
#include <roq_kernel.h>

static uint64_t   lclMask[MAX_DIR];  // bits set by this node
static uint64_t   rmtMask[MAX_DIR];  // bits set by remote node

static uint64_t   rmt_lastState[MAX_DIR];
static uint64_t   rmt_timeout[MAX_DIR];

static uint8_t    rmt_seen[MAX_DIR];
static uint64_t    rmt_lastSeen[MAX_DIR];

static MUSPI_GI_t GI[MAX_DIR];

static inline void lcl_setUp(int i) {
	*(GI[i].controlRegPtr) |= lclMask[i];
}

static inline void lcl_setDown(int i) {
	*(GI[i].controlRegPtr) &= ~lclMask[i];
}

static inline int rmt_getState(int i) {
	return (int)((*(GI[i].statusRegPtr) & rmtMask[i]) != 0);
}

static void runChecker() {
	int dir       = 0;
	int rmt_state = 0;

	for (dir = 0; dir < MAX_DIR; dir++) {
		if (rmt_lastState[dir] == 0x2) continue; // skip dead links
		
		rmt_state = rmt_getState(dir);
		
		// decrement/reset timeout counter it
		if (rmt_lastState[dir] == rmt_state) {
			rmt_timeout[dir]--;
		} else {
			rmt_timeout[dir] = config.detection.max;
			rmt_lastSeen[dir] = GetTimeBase();
		}

		// other side failed to respond for too long --> mark link as bad once the timeout turned 0
		if (rmt_timeout[dir] == 0) {
			ERROR("Node (%d,%d,%d,%d,%d) has not answered via link %d in %u us, assuming it's dead.\n",
				  global.neighbors[dir]->A,
				  global.neighbors[dir]->B,
				  global.neighbors[dir]->C,
				  global.neighbors[dir]->D,
				  global.neighbors[dir]->E,
				  dir, config.detection.max*config.detection.period);

			lcl_badLinks |= 1 << dir;

			wakeupThread(ROQ_NOTIFIER_THREAD);

			rmt_lastState[dir] = 0x2;
	
			continue;
		}
		
		rmt_lastState[dir] = rmt_state;
		
		// simple rule: lcl.plus  =   rmt.minus
		//              lcl.minus = ! rmt.plus
		if (is_plus(dir)) {
			if (rmt_state) lcl_setUp(dir);
			else           lcl_setDown(dir);
		} else {
			if (rmt_state) lcl_setDown(dir);
			else           lcl_setUp(dir);
		}
	}
}

static int32_t barrierFunction(void *parm) {
	return 0;
}

static int setupChecker() {
	int rc = 0;
	
	INFO("Setting up class routes for a %dD torus\n", global.dimensions);
	
	int i,j;
	
	uint32_t nClassRoutes;
	uint32_t classRouteIds[BGQ_GI_CLASS_MAX_CLASSROUTES];
	uint32_t classRouteId;
	
	/* Check if there are enough free class routes */
	rc = Kernel_QueryGlobalInterruptClassRoutes(&nClassRoutes,
												classRouteIds,
												sizeof(classRouteIds));
	if (rc) {
		ERROR("Kernel_QueryGlobalInterruptClassRoutes failed (rc=%d)\n",rc);
		return -1;
	}
	if (nClassRoutes < global.dimensions*2) {
		ERROR("Not enough class routes available. Need %d, got %d\n", global.dimensions*2, nClassRoutes);
		return -1;
	}
	
	INFO("There are %d class routes available\n", nClassRoutes);
	
	/* Setup class route pairs and allocate them */
	uint8_t pair[8][2]; 
	for (i = 0; i < MAX_DIR+2; i++) {
		//    if (neighbor[i] == NULL) continue;
		
		pair[i/2][i%2] = (i < 7) ? i : i + 2; // skip 7 and 8.
		
		/* Allocate Class routes */
		Kernel_GlobalInterruptClassRouteAttributes_t attrs;
		attrs.System = 0; // 0 = user or system, 1 = system only
		rc = Kernel_AllocateGlobalInterruptClassRoute(classRouteIds[pair[i/2][i%2]],
													  &attrs);
		if (rc) {
			ERROR("Kernel_AllocateGlobalInterruptClassRoute failed for class route %d (rc=%d)\n", pair[i/2][i%2], rc);
			return -1;
		}
		INFO("Successfully allocated class route %d\n", pair[i/2][i%2]);
	}
	
	/* Assign the right class routes to each direction */
	for (i = 0; i < MAX_DIR; i++) { // looping through all DIRECTIONS (0..9), not dimensions (0..4)
		if (global.neighbors[i] == NULL) continue;
		
		ClassRoute_t cr;

		/* Assumption is that a dimension is always even */
//		DSTRONG_ASSERT(global.nodes.dim[i/2] & 0x1 != 0,
//					   "Number of nodes in dimension %d is odd (%d). Only blocks with even number of nodes in every active dimension are supported",
//					   i/2, global.nodes.dim[i/2]);
		// if the dimension is odd, we switch the class id so that it matches the class id of the other end of the link.
		if (global.coords.dim[i/2] & 0x1) // odd dimension
			classRouteId = classRouteIds[pair[i/2][(i+1)%2]];
		else // even dimension
			classRouteId = classRouteIds[pair[i/2][i%2]];
		
		/* select local and remote mask. */
		lclMask[i] = i & 0x1 ? 0x2 : 0x1; // minus links use 0x1
		rmtMask[i] = i & 0x1 ? 0x1 : 0x2; // plus links use 0x2
		
		/* Choosing if this link direction is an input or an output */
		if (i & 0x1) { // that means we have a PLUS link to a neighbor here.
			cr.input  = BGQ_CLASS_INPUT_LINK_LOCAL;
			cr.output = BGQ_CLASS_LINK_AP >> (i-1);
			INFO("Assigning class route %d to output link direction %d. lclMask = 0x%x, rmtMask = 0x%x, cr.in = 0x%04x, cr.out = 0x%04x\n", classRouteId, i, lclMask[i], rmtMask[i], cr.input, cr.output);
		} else { // that means we have a MINUS link to a neighbor here.
			cr.input  = BGQ_CLASS_INPUT_LINK_LOCAL | (BGQ_CLASS_LINK_AM >> i);
			cr.output = 0;
			INFO("Assigning class route %d to input link direction %d. lclMask = 0x%x, rmtMask = 0x%x, cr.in = 0x%04x, cr.out = 0x%04x\n", classRouteId, i, lclMask[i], rmtMask[i], cr.input, cr.output);
		}
		
		rc = Kernel_SetGlobalInterruptClassRoute (classRouteId, &cr);
		if (rc) {
			ERROR("Kernel_SetGlobalInterruptClassRoute failed for class route %d (rc=%d)\n", classRouteId, rc);
			return -1;
		}
		
		rc = MUSPI_GIInitMU(classRouteId,
							0,
							barrierFunction, // I think I don't need a barrier function here.
							(void*)NULL,
							1000000);
		if (rc) {
			ERROR("MUSPI_GIIinitMU failed for class route %d/global interrupt %d (rc=%d)\n", classRouteId, 0, rc);
			return -1;
		}
		
		rc = MUSPI_GIInit(&GI[i],
						  classRouteId,
						  0);
		if (rc) {
			ERROR("MUSPI_GIIinit failed for class route %d/global interrupt %d (rc=%d)\n", classRouteId, 0, rc);
			return -1;
		}
		
		// Pull all links down. Initialization actually doesn't matter. It works with any initial state.
		lcl_setDown(i);
	}

	// Initialize last seen and give a 10 second tolerance until all neighbor nodes have to be up.
	uint64_t now = get_time();
	for (i = 0; i < MAX_DIR; i++) {
	    rmt_timeout[i]   = config.detection.init; // 10 seconds tolerance until all nodes are up.
		rmt_lastState[i] = global.neighbors[i] == NULL ? 0x2 : 0x0; // mark non-existing links as dead from the beginning.
		rmt_seen[i]      = 0;
		rmt_lastSeen[i]  = 0;
	}
}

static void Checker_decrementerInterruptHandler(RoQ_Regs_t* context, uint64_t code) {
	runChecker();

	// Ack decrementer interrupt
	mtspr(SPRN_TSR, TSR_DIS);
}

void Checker_GlobalInterrupts() {  
	setupChecker();

	// Allow all cores to sleep
	mtspr(SPRN_CCR0, CCR0_WEM(0xF));

	setInterruptHandler(Checker_decrementerInterruptHandler, IVO_DEC);

	uint32_t decr = config.detection.period*global.freqMhz;
	enableDecrementerInterrupt(decr, decr, 1); // 1 = autoreload

	while (1) {
		// go back to sleep and wait for next interrupt to wake me up again.
		asm volatile ("wait" : : : "memory" );
	}
}
