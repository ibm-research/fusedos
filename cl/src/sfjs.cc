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
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <firmware/include/Firmware_Interface.h>

//#include "hwi/include/bqc/classroute.h"
#include "spi/include/mu/Classroute.h"
#include "spi/include/mu/Classroute_inlines.h"

#include "sfjs_cmdline.h"

#include "personality_envvar_names.h"
#include "../include/AppState.h"

extern "C" {
#include "fusedos_config.h"
#include "fusedos.h"
#include "spi/include/kernel/gi.h"
#include "spi/include/mu/GIBarrier.h"
#include "gi_impl.h"
#include "collective_impl.h"
};

#include "cl_mu.h"
#include "cl_coords.h"

extern void init_cl_mu_data();
extern int readPersonalityIntoEnvironment();
int debug = 0;

static const char * primordial_cr_filename = "/mu/classroute/primordial_cr";

enum classroute_type {
    GI,
    COLL
};

enum cr_ids {
    USER = 0,
    JOB = 14,
    BLOCK = 15
};


// copied from personality, as we cannot refer to that nested struct in
// a straight-forward way.
typedef struct PrimordialClassRoute {
  fw_uint16_t GlobIntUpPortInputs;	   // Global Interrupt Up Tree Inputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
  fw_uint16_t GlobIntUpPortOutputs;	  // Global Interrupt Up Tree Outputs. Only the 12 LSBs are relevant (11 torus links + 1 internal).
  fw_uint16_t CollectiveTypeAndUpPortInputs; // Collective type and Up Tree Inputs.  See encodings below
  fw_uint16_t CollectiveUpPortOutputs;       // Collective Up Tree Outputs.  Only the 12 LSBs are relevant (11 torus links + 1 internal).
} PrimordialClassRoute_t;

static AppNodeCoordinates_t rank2coords(int rank, const AppNodeCoordinates_t &block_shape) {

#define NODES(x) (block_shape.x##Coord)
#define COORD(x) (node.x##Coord)
    AppNodeCoordinates_t node;

    /* match CNK's default map order ABCDET */
    /* see how we calculate the rank from coordinates below
     * for why it works this way */
    uint32_t rank_rem = rank;

    COORD(e) = rank_rem % NODES(e);
    rank_rem /= NODES(e);

    COORD(d) = rank_rem % NODES(d);
    rank_rem /= NODES(d);

    COORD(c) = rank_rem % NODES(c);
    rank_rem /= NODES(c);

    COORD(b) = rank_rem % NODES(b);
    rank_rem /= NODES(b);

    COORD(a) = rank_rem % NODES(a);

    return node;
#undef NODES
#undef COORD
}

static int coords2rank(const AppNodeCoordinates_t &node, const AppNodeCoordinates_t &block_shape) {

#define NODES(x) (block_shape.x##Coord)
#define COORD(x) (node.x##Coord)

    /* match CNK's default map order ABCDET */
    /* rank = E + dim_E ( D + dim_D ( C + dim_C ( B + dim_B * A ))) */

    uint32_t rank = COORD(a) * NODES(b) + COORD(b);
    rank          = rank     * NODES(c) + COORD(c);
    rank          = rank     * NODES(d) + COORD(d);
    rank          = rank     * NODES(e) + COORD(e);

    return rank;
#undef NODES
#undef COORD
}

static AppNodeCoordinates_t __debug_node_coords;
static AppNodeCoordinates_t __debug_block_shape;
#define SFJS_DEBUG(FMT,...) if(debug) { printf("%04d:<%u,%u,%u,%u,%u> %s:%d " FMT, \
	coords2rank(__debug_node_coords, __debug_block_shape), \
	__debug_node_coords.aCoord, __debug_node_coords.bCoord, \
	__debug_node_coords.cCoord, __debug_node_coords.bCoord, \
	__debug_node_coords.eCoord, __func__, __LINE__, \
	##__VA_ARGS__ ); }

#define SFJS_ERROR(FMT,...) do { fprintf(stderr, "%04d:<%u,%u,%u,%u,%u> " FMT, \
	coords2rank(__debug_node_coords, __debug_block_shape), \
	__debug_node_coords.aCoord, __debug_node_coords.bCoord, \
	__debug_node_coords.cCoord, __debug_node_coords.bCoord, \
	__debug_node_coords.eCoord, ##__VA_ARGS__ ); } while(0);


int get_slurm_job_size_from_env() {

    char * slurm_job_num_nodes = getenv("SLURM_JOB_NUM_NODES");

    if( slurm_job_num_nodes == NULL )
	return 0;

    char * endptr;
    long int num_nodes = strtol(slurm_job_num_nodes, &endptr, 10);

    if( *slurm_job_num_nodes == '\0' || *endptr != '\0' ) {
	SFJS_ERROR("SLURM_JOB_NUM_NODES value \"%s\" cannot be parsed as an int\n", slurm_job_num_nodes);
	return -1;
    }

    return num_nodes;
}

int get_slurm_job_shape_from_env(AppNodeCoordinates_t &ll,
	AppNodeCoordinates_t &ur, const AppNodeCoordinates_t &block_shape) {

    // check SLURM env for block crank[A-B]
    char *slurm_nodes = getenv("SLURM_JOB_NODELIST");
    if(!slurm_nodes) {
	SFJS_ERROR("Cannot use a subblock outside a SLURM job, but environment variable SLURM_JOB_NODELIST not found.\n");
	return -1;
    }

    int first_rank, last_rank;
    if( sscanf(slurm_nodes, "crank[%d-%d]", &first_rank, &last_rank) != 2 ) {
	SFJS_ERROR("SLURM_JOB_NODELIST value \"%s\" does not match the template crank[A-B] that we can parse.\n",
		slurm_nodes);
	return -1;
    }

    SFJS_DEBUG("SLURM job of ranks %d to %d\n", first_rank, last_rank);

    // calculate coordinates for block 
    ll = rank2coords(first_rank, block_shape);
    ur  = rank2coords(last_rank, block_shape);

    // check if block is a rectangle
#define VIOLATION(dim) (ur.dim##Coord < ll.dim##Coord)
    if( VIOLATION(a) || VIOLATION(b) || VIOLATION(c) ||
	    VIOLATION(d) || VIOLATION(e)) {
	SFJS_ERROR("SLURM job allocation is not a rectangle in the Blue Gene/Q torus. Cannot continue.\n");
	return -1;
    }
#undef VIOLATION

    return 0;
}

static int read_primordial_cr(PrimordialClassRoute_t *cr)
{
    // MUFS file classroute/primordial_cr provides the
    // value of the PrimordialClassRoute struct in the
    // personality by means of a simple read() implementation.
    int fd = open(primordial_cr_filename, O_RDONLY);
    if(fd == -1) {
	SFJS_ERROR("error %d opening file %s, %s\n", errno, primordial_cr_filename, strerror(errno));
	return -1;
    }

    ssize_t r = read(fd, cr, sizeof(PrimordialClassRoute_t));

    SFJS_DEBUG("primordial CR GI %4x / %4x coll %4x / %4x\n",
	    cr->GlobIntUpPortInputs, cr->GlobIntUpPortOutputs,
	    cr->CollectiveTypeAndUpPortInputs,
	    cr->CollectiveUpPortOutputs);

    if(r != sizeof(PrimordialClassRoute_t)) {
	SFJS_ERROR("error %d reading PrimordialClassRoute from file %s, %s\n", errno, primordial_cr_filename, strerror(errno));
	return -1;
    }

    return 0;
}

static const char * input_to_vc(uint16_t inp) {
    switch(inp & BGQ_CLASS_INPUT_VC_MASK) {
	case BGQ_CLASS_INPUT_VC_USER:
	    return "user comm-world";
	case BGQ_CLASS_INPUT_VC_SYSTEM:
	    return "system comm-world";
	case BGQ_CLASS_INPUT_VC_SUBCOMM:
	    return "subcomm";
	default:
	    return "<unknown VC>";
    }
}

static inline void dump_cr(int id, const ClassRoute_t &cr, classroute_type type)
{
    AppNodeCoordinates_t node_coords = __debug_node_coords;
    if(!debug)
	return;

#define BIT(a, b) (a&b ? '1' : '0')
    printf(" %2u|%2u|%2u|%2u|%2u:%2d:%s:0x%04x:0x%04x|   |%c%c|%c%c|%c%c|%c%c|%c%c|%c%c|%c%c|%c%c|%c%c|%c%c|%c|%c%c|%s\n",
		node_coords.aCoord, node_coords.bCoord, node_coords.cCoord, node_coords.dCoord, node_coords.eCoord,
		id, (type == COLL ? "coll" : " gi "),
		cr.input, cr.output,
		BIT(cr.input, BGQ_CLASS_LINK_AP), BIT(cr.input, BGQ_CLASS_LINK_AM),
		BIT(cr.input, BGQ_CLASS_LINK_BP), BIT(cr.input, BGQ_CLASS_LINK_BM),
		BIT(cr.input, BGQ_CLASS_LINK_CP), BIT(cr.input, BGQ_CLASS_LINK_CM),
		BIT(cr.input, BGQ_CLASS_LINK_DP), BIT(cr.input, BGQ_CLASS_LINK_DM),
		BIT(cr.input, BGQ_CLASS_LINK_EP), BIT(cr.input, BGQ_CLASS_LINK_EM),
		BIT(cr.output, BGQ_CLASS_LINK_AP), BIT(cr.output, BGQ_CLASS_LINK_AM),
		BIT(cr.output, BGQ_CLASS_LINK_BP), BIT(cr.output, BGQ_CLASS_LINK_BM),
		BIT(cr.output, BGQ_CLASS_LINK_CP), BIT(cr.output, BGQ_CLASS_LINK_CM),
		BIT(cr.output, BGQ_CLASS_LINK_DP), BIT(cr.output, BGQ_CLASS_LINK_DM),
		BIT(cr.output, BGQ_CLASS_LINK_EP), BIT(cr.output, BGQ_CLASS_LINK_EM),
		BIT(cr.input, BGQ_CLASS_INPUT_LINK_LOCAL),
		BIT(cr.input, BGQ_CLASS_LINK_IO), BIT(cr.output, BGQ_CLASS_LINK_IO),
		input_to_vc(cr.input));
#undef BIT
}


// returns file descriptor or -1
static int setup_classroute(int id, ClassRoute_t cr, classroute_type type)
{
    int fd;
    int rc;

    if(debug) dump_cr(id, cr, type);

    // open classroute file, use classroute_impl
    switch(type) {
    case GI:
	rc = Kernel_AllocateGlobalInterruptClassRoute(id, NULL);
	fd = get_gi_cr_fd(id);
	break;
    case COLL:
	rc = Kernel_AllocateCollectiveClassRoute(id);
	fd = get_coll_cr_fd(id);
	break;
    default: return -1; // should not happen
    }

    if(rc || fd == -1) {
	SFJS_ERROR("error %d allocating class route %d, %s\n", rc, id, strerror(rc));
	return -1;
    }

    // set classroute, use classroute_impl
    switch(type) {
    case GI:
	rc = Kernel_SetGlobalInterruptClassRoute(id, &cr);
	break;
    case COLL:
	rc = Kernel_SetCollectiveClassRoute(id, &cr);
	break;
    default: return -1; // should not happen
    }

    return 0;
}

// returns file descriptor or -1
static int open_classroute(int id, classroute_type type)
{
    int fd;
    int rc;

    // open classroute file, use classroute_impl
    switch(type) {
    case GI:
	rc = Kernel_AllocateGlobalInterruptClassRoute(id, NULL);
	fd = get_gi_cr_fd(id);
	break;
    case COLL:
	rc = Kernel_AllocateCollectiveClassRoute(id);
	fd = get_coll_cr_fd(id);
	break;
    default: return -1; // should not happen
    }

    if(rc || fd == -1) {
	SFJS_ERROR("error %d allocating class route %d, %s\n", rc, id, strerror(rc));
	return -1;
    }

    return 0;
}

/* We do not give the sfjs Linux process direct access to the MMIO
 * regions that are used for the MU's barrier mechanism. Instead,
 * MUFS exports the values involved in the barrier mechanism via a file
 * interface. This mechanism introduces some sytem call overhead.
 * However, the barriers used by SFJS are not overly
 * performance-sensitive, as they are used only once for each MPI job.
 */
static int set_gibarrier_value(int cr_id, uint8_t value)
{
    if( ! have_gi_cr_fd(cr_id) ) return -1;

    ssize_t rc = write( get_gi_cr_fd(cr_id), &value, sizeof(value) );

    if(rc != sizeof(value)) {
	SFJS_ERROR("error %d setting GI/barrier value for class route %d, %s\n", errno, cr_id, strerror(errno));
	return -1;
    }

    return 0;
}

static int get_gibarrier_status_value(int cr_id)
{
    uint8_t value;
    if( ! have_gi_cr_fd(cr_id) ) return -1;

    ssize_t rc = read( get_gi_cr_fd(cr_id), &value, sizeof(value) );

    if(rc != sizeof(value)) {
	SFJS_ERROR("error %d getting GI/barrier value for class route %d, %s\n", errno, cr_id, strerror(errno));
	return -1;
    }

    return value;
}

static int get_gibarrier_control_value(int cr_id)
{
    uint8_t values[2];
    if( ! have_gi_cr_fd(cr_id) ) return -1;

    ssize_t rc = read( get_gi_cr_fd(cr_id), &values, sizeof(values) );

    if(rc != sizeof(values)) {
	SFJS_ERROR("error %d getting GI/barrier value for class route %d, %s\n", errno, cr_id, strerror(errno));
	return -1;
    }

    return values[1];
}

static int hop_block_barrier(void)
{
    int current = get_gibarrier_control_value(BLOCK);
    SFJS_DEBUG("current sys barrier control value %d\n", current);
    if(current == -1)
	return -1; // debugging message above will explain

    int oldstate = current;
    int newval = current >> 1;
    if(newval == 0)
	newval = MUSPI_GIBARRIER_INITIAL_STATE;

    if(set_gibarrier_value(BLOCK, newval))
	return -1; // error

    SFJS_DEBUG("determined new value to set to be %d\n", newval);

    do {
	current = get_gibarrier_status_value(BLOCK);
    } while( current & oldstate ); // former bit still set

    SFJS_DEBUG("hopped over block barrier\n");
    return 0;
}

static int hop_job_barrier(void)
{
    int current = get_gibarrier_control_value(JOB);
    SFJS_DEBUG("current job barrier control value %d\n", current);
    if(current == -1)
	return -1; // debugging message above will explain

    int oldstate = current;
    int newval = current >> 1;
    if(newval == 0)
	newval = MUSPI_GIBARRIER_INITIAL_STATE;

    if(set_gibarrier_value(JOB, newval))
	return -1; // error

    SFJS_DEBUG("determined new value to set to be %d\n", newval);

    do {
	current = get_gibarrier_status_value(JOB);
    } while( current & oldstate ); // former bit still set

    SFJS_DEBUG("hopped over job barrier\n");
    return 0;
}


int setup_job_barrier_for_block(const PrimordialClassRoute_t &primordial_cr) {

    SFJS_DEBUG("Preparing to use firmware-provided system class route\n");
    if(Kernel_AllocateGlobalInterruptClassRoute(BLOCK, NULL))
	return -1;

    ClassRoute_t gi_job;

    gi_job.input  = primordial_cr.GlobIntUpPortInputs;
    gi_job.output = primordial_cr.GlobIntUpPortOutputs;

    SFJS_DEBUG("starting to allocate and setup class routes.\n");
    int gi_job_fd = setup_classroute( JOB, gi_job,   GI );

    if(gi_job_fd == -1) {
	SFJS_ERROR("error setting up class route for GI barrier\n");
	return -1;
    }

    // barrier
    if(hop_block_barrier()) {
	SFJS_ERROR("error entering and passing system barrier\n");
	return -1;
    }

    // enable: set own contribution
    SFJS_DEBUG("setting GI barrier values\n");
    set_gibarrier_value(JOB, MUSPI_GIBARRIER_INITIAL_STATE);

    // barrier
    if(hop_block_barrier()) {
	SFJS_ERROR("error entering and passing system barrier\n");
	return -1;
    }

    // check status of barrier 
    int status_value;
    status_value = get_gibarrier_status_value(14);
    if(status_value != MUSPI_GIBARRIER_INITIAL_STATE) {
        SFJS_ERROR("status reg of GI 14 is %d, should be %d\n", status_value, MUSPI_GIBARRIER_INITIAL_STATE);
	return -1;
    }

    return 0;
}

static void AppNodeCoordinates2CR_COORD_T(
	const AppNodeCoordinates_t &from, CR_COORD_T &to) {
    CR_COORD_DIM( &to, 0 ) = from.aCoord;
    CR_COORD_DIM( &to, 1 ) = from.bCoord;
    CR_COORD_DIM( &to, 2 ) = from.cCoord;
    CR_COORD_DIM( &to, 3 ) = from.dCoord;
    CR_COORD_DIM( &to, 4 ) = from.eCoord;
}

#if 0
static void printCoords(const AppNodeCoordinates_t &c) {
    printf("<%d,%d,%d,%d,%d>",
	    c.aCoord, c.bCoord, c.cCoord, c.dCoord, c.eCoord);
}
#endif

int determine_subblock_classroute(
    const AppNodeCoordinates_t &job_ll, const AppNodeCoordinates_t &job_ur,
    const AppNodeCoordinates_t &node_coords, ClassRoute_t * cr) {

    // calculate class routes
    CR_COORD_T me;
    CR_RECT_T world;

    AppNodeCoordinates2CR_COORD_T(node_coords, me);

    AppNodeCoordinates2CR_COORD_T(job_ll, world.ll);
    AppNodeCoordinates2CR_COORD_T(job_ur, world.ur);

    // 1. pick a root
    CR_COORD_T root;
    int pri_dim;
    MUSPI_PickWorldRoot( &world, NULL, &root, &pri_dim);

    SFJS_DEBUG("picked tree root at <%u,%u,%u,%u,%u>\n",
	CR_COORD_DIM( &root, 0), CR_COORD_DIM( &root, 1),
	CR_COORD_DIM( &root, 2), CR_COORD_DIM( &root, 3),
	CR_COORD_DIM( &root, 4));

    // 2. actually create class route
    MUSPI_BuildNodeClassroute( &world, &root, &me, &world,
	    NULL, pri_dim, cr);

    // 3. set local contribution bit
    cr->input |= BGQ_CLASS_INPUT_LINK_LOCAL;

    return 0;
}


int setup_job_barrier_for_subblock_prep(
    const AppNodeCoordinates_t &job_ll,
    const AppNodeCoordinates_t &job_ur,
    const AppNodeCoordinates_t &node_coords) {

    ClassRoute_t cr;

    if( determine_subblock_classroute( job_ll, job_ur,
	    node_coords, &cr) )
	return -1;

    int gi_job_fd = setup_classroute( JOB, cr,   GI );

    if(gi_job_fd == -1)
	return -1;

    return 0;
}


int setup_job_barrier_for_subblock_gion(int job_size, int block_size) {

    // just open pre-configured JOB GI (14)
    if(Kernel_AllocateGlobalInterruptClassRoute(JOB, NULL))
	return -1;
    return set_gibarrier_value(JOB, MUSPI_GIBARRIER_INITIAL_STATE);
}


int setup_user_single_node(AppNodeCoordinates_t &block_shape,
	AppNodeCoordinates_t &node_coords) {

    // setup class route
    ClassRoute_t coll_user;
    coll_user.input = BGQ_CLASS_INPUT_VC_SUBCOMM | BGQ_CLASS_INPUT_LINK_LOCAL;
    coll_user.output = 0;

    SFJS_DEBUG("setting up collective class route 0 for single node\n");
    int coll_user_fd = setup_classroute( USER, coll_user, COLL );
    if( coll_user_fd == -1) return -1;

    // TODO do we need GI 0 in this case?

    // massage environment
    // - set shape to 1x1x1x1x1 and coords to 0,0,0,0,0 for single-node case
    // (workaround currently needed for PAMI)

    block_shape.aCoord = block_shape.bCoord = block_shape.cCoord
	= block_shape.dCoord = block_shape.eCoord = 1;

    node_coords.aCoord = node_coords.bCoord = node_coords.cCoord
	= node_coords.dCoord = node_coords.eCoord = 0;

    //	 re-export job shape and node coordinate environment
    //	 variables to announce our changes to CL
    char env_val[26];
    snprintf(env_val, sizeof(env_val), "<%3d,%3d,%3d,%3d,%3d>",
	    block_shape.aCoord, block_shape.bCoord, block_shape.cCoord,
	    block_shape.dCoord, block_shape.eCoord);
    setenv(PERSO_ENV_NODE_DIMENSIONS, env_val, 1);

    snprintf(env_val, sizeof(env_val), "<%3d,%3d,%3d,%3d,%3d>",
	    node_coords.aCoord, node_coords.bCoord, node_coords.cCoord,
	    node_coords.dCoord, node_coords.eCoord);
    setenv(PERSO_ENV_NODE_COORDS, env_val, 1);

    return 0;
}


int setup_user( int job_size, int block_size,
	const PrimordialClassRoute_t &primordial_cr,
	const AppNodeCoordinates_t &job_ll,
	const AppNodeCoordinates_t &job_ur,
	const AppNodeCoordinates_t &node_coords) {
    // setup user class routes for job, GI 0 and Collective 0

    SFJS_DEBUG("starting to setup user class routes.\n");
    ClassRoute_t gi_user;

    if(job_size == block_size) {
        gi_user.input  = primordial_cr.GlobIntUpPortInputs;
        gi_user.output = primordial_cr.GlobIntUpPortOutputs;

    } else {
	// subblock, use same class route as in GI 14
	if( determine_subblock_classroute( job_ll, job_ur,
	    node_coords, &gi_user) )
	return -1;
    }

    int gi_user_fd = setup_classroute( USER, gi_user, GI );

    if( gi_user_fd == -1 ) {
	SFJS_ERROR("error setting up user class routes\n");
	return -1;
    }

    // * setup "np classroutes" -- class routes only for the nodes in the job
    ClassRoute_t coll_user;
    //	 - special case: all nodes == global class routes
    if( job_size == block_size ) {
	coll_user.input = (primordial_cr.CollectiveTypeAndUpPortInputs & ~BGQ_CLASS_INPUT_VC_MASK) | BGQ_CLASS_INPUT_VC_SUBCOMM;
	coll_user.output = primordial_cr.CollectiveUpPortOutputs;
    } else {
	// we have calculated the class route for the job rectangle above
	coll_user.input = (gi_user.input & ~BGQ_CLASS_INPUT_VC_MASK)
	    | BGQ_CLASS_INPUT_VC_SUBCOMM;
	coll_user.output = gi_user.output;
    }

    int coll_user_fd = setup_classroute( USER, coll_user, COLL );
    if(coll_user_fd == -1) {
	SFJS_ERROR("error setting up user collective class route\n");
	return -1;
    }

    // barrier
    if( hop_job_barrier() ) {
	SFJS_ERROR("error entering and passing system barrier\n");
	return -1;
    }

    SFJS_DEBUG("setting GI barrier values\n");
    set_gibarrier_value(USER, MUSPI_GIBARRIER_INITIAL_STATE);

    // barrier
    if( hop_job_barrier() ) {
	SFJS_ERROR("error entering and passing system barrier\n");
	return -1;
    }

    // read status reg of gi 0 -> should be 100
    // (MUSPI_GIBARRIER_INITIAL_STATE)
    int status_value;
    status_value = get_gibarrier_status_value(USER);
    if(status_value != MUSPI_GIBARRIER_INITIAL_STATE) {
        SFJS_ERROR("status reg of GI 0 is %d, should be %d\n", status_value, MUSPI_GIBARRIER_INITIAL_STATE);
        return -1;
    }

    // final barrier between checking gi barrier values and starting the job
    if( hop_job_barrier() ) {
	SFJS_ERROR("error entering and passing system barrier\n");
	return -1;
    }

    return 0;
}


int launch_cl(int argc, char ** argv, int job_size,
	struct sfjs_cmdline_args args,
	const AppNodeCoordinates_t &job_corner,
	const AppNodeCoordinates_t &job_shape )
{
    // * start CL and wait for it to finish
    //	 fork + execve + wait
    pid_t pid = fork();

    if( pid == 0 ) {
	// child process, start cl

	// build argv list for child
	// * optind points to the first remaining argument
	#define SFJS_PROVIDED_ARGS 8
	int child_argc = (argc-optind+1 + SFJS_PROVIDED_ARGS);
	char **child_argv = (char **)malloc( (child_argc + 1) * sizeof(char *) );

	for(int i = 1 + SFJS_PROVIDED_ARGS; (i+optind-(1+SFJS_PROVIDED_ARGS)) < argc; i++) {
	    child_argv[i] = argv[optind + i - (1+SFJS_PROVIDED_ARGS)];
	}

	// parameter job_ranks for CL
	child_argv[1] = (char *)"--job_ranks";
	char job_ranks[20];
	snprintf(job_ranks, sizeof(job_ranks), "%d", job_size * args.procs_arg);
	child_argv[2] = job_ranks;

	// parameter number of procs for CL
	child_argv[3] = (char *)"--num_procs";
	char num_procs[20];
	snprintf(num_procs, sizeof(num_procs), "%d", args.procs_arg);
	child_argv[4] = num_procs;

	// parameter job corner
	child_argv[5] = (char *)"--job_corner";
	char job_corner_str[23];
	snprintf(job_corner_str, sizeof(job_corner_str),
	    "<%d,%d,%d,%d,%d>",
	    job_corner.aCoord, job_corner.bCoord, job_corner.cCoord,
	    job_corner.dCoord, job_corner.eCoord);
	child_argv[6] = job_corner_str;

	// parameter job shape
	child_argv[7] = (char *)"--job_shape";
	char job_shape_str[23];
	snprintf(job_shape_str, sizeof(job_shape_str),
	    "<%d,%d,%d,%d,%d>",
	    job_shape.aCoord, job_shape.bCoord, job_shape.cCoord,
	    job_shape.dCoord, job_shape.eCoord);
	child_argv[8] = job_shape_str;


	// program name and argument list termination
	child_argv[0] = (char *)"cl";
	child_argv[child_argc] = NULL;

#if 0
	printf("argc %d, optind %d, child_argc %d\n", argc, optind, child_argc);
	for(int i=0; i < child_argc; i++)
	    printf("%d: \"%s\"\n", i, child_argv[i]);
#endif

	execvp("cl", child_argv); // use current process's environment

	// returned from execv, must be because of an error
	printf("error %d calling cl, %s.\n", errno, strerror(errno));
	exit(-1);

    } else {
	int status;
	// parent process, wait for child to complete
	waitpid(pid, &status, 0);
	if(status != 0) SFJS_ERROR("CL exited with status %d\n", status);
	return status;
    }
}


int reset_block_barrier() {

    if(Kernel_AllocateGlobalInterruptClassRoute(BLOCK, NULL))
	return -1;

    if(set_gibarrier_value(BLOCK, MUSPI_GIBARRIER_INITIAL_STATE))
	return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    AppNodeCoordinates_t node_coords;
    AppNodeCoordinates_t block_shape;

    init_cl_mu_data(); // metadata for MUFS interface

    // new workflow
    // I. parse parameters
    // II. determine block size (personality)
    //
    // III-1. determine job size (parameter or SLURM env variables)
    // III-2. determine job corners and shape (from SLURM env variables)
    //
    // IVa: for complete block:
    //	    use block GI barrier to setup job GI barrier
    // IVb: for subblock:
    //	    get stage from parameter:
    //	    IVb1 prep: set class route for job GI barrier
    //	    IVb2 enable: set own contribution for job GI barrier
    //	    IVb3 go: move on 
    // IVc: for single node, skip
    //
    // Va: for complete block or subblock
    //	use job GI barrier to setup user class routes (GI + collective)
    // Vb: for single node
    //  configure local-only user class routes
    //  massage environment for PAMI
    //
    // VI launch cl
    //
    // VII reset job GI barrier to inactive, disable user collective,
    //     disable user GI

    // I. parse parameters
    struct sfjs_cmdline_args args;
    if (sfjs_cmdline_parser(argc, argv, &args)) {
	printf("error from cmdline_parser\n");
	sfjs_cmdline_parser_print_help();
	exit(-1); // error occurred
    }

    debug = args.debug_flag;

    if( (argc - optind) == 0 // too few arguments, except ...
	    // subblock preparations
	&& ! ( args.subblock_given && 
	    ( args.subblock_arg == subblock_arg_prep
	   || args.subblock_arg == subblock_arg_gion ) )
	    // or a reset of the block barrier
	&& ! args.reset_block_barrier_given ) {

	fprintf(stderr, "Called without arguments for cl. That does not seem right, exiting.\n");
	sfjs_cmdline_parser_print_help();
	exit(-1); // error occurred
    }

    // shortcut: do we just reset the block barrier?
    if( args.reset_block_barrier_given && args.reset_block_barrier_flag )
	return reset_block_barrier();

    // II. determine block size (personality)
    // * read /dev/bgpers and populate environment variables (as most BG/Q
    // Linux init scripts do
    if(readPersonalityIntoEnvironment())
	exit(-1);

    // get block dimensions and node coordinates from environment
    if( stringToCoords( getenv(PERSO_ENV_NODE_DIMENSIONS), &block_shape) ) {
	SFJS_ERROR("failed to use %s as block shape\n", getenv(PERSO_ENV_NODE_DIMENSIONS));
	return -1;
    }
    if( stringToCoords( getenv(PERSO_ENV_NODE_COORDS), &node_coords) ) {
	SFJS_ERROR("failed to use %s as node coordinates\n", getenv(PERSO_ENV_NODE_COORDS));
	return -1;
    }

    // fill global variables for debugging
    __debug_node_coords = node_coords;
    __debug_block_shape = block_shape;

    int block_size = block_shape.aCoord * block_shape.bCoord 
	* block_shape.cCoord * block_shape.dCoord * block_shape.eCoord;


    // III-1. determine job size (parameter or SLURM env variables)
    int job_size;
    int slurm_job_size = get_slurm_job_size_from_env();

    // check for contradictions between SLURM and parameter,
    // use the one that is available

    if( slurm_job_size == -1) // error parsing
	exit(-1);

    if( slurm_job_size == 0 && ! args.nodes_given ) {
	fprintf(stderr, "not SLURM job and no nodes argument given");
	sfjs_cmdline_parser_print_help();
	exit(-1);
    }

    if( slurm_job_size != 0 && args.nodes_given
	    && slurm_job_size != args.nodes_arg ) {
	fprintf(stderr, "SLURM claims %d nodes in job, %d nodes given as parameter; mismatch indicates an error.\n",
		slurm_job_size, args.nodes_arg);
	exit(-1);
    }

    if( slurm_job_size != 0) {
	job_size = slurm_job_size;
    } else {
	job_size = args.nodes_given;
    }

    if( job_size > block_size) {
	printf("Cannot use more nodes for job (%d specified) than those in the subblock (%d)\n",
		job_size, block_size);
	exit(-1);
    }

    // III-2. determine job corners and shape (from SLURM env variables)
    AppNodeCoordinates_t job_ll, job_ur;
    AppNodeCoordinates_t job_corner;
    AppNodeCoordinates_t job_shape;

    if( job_size > 1 ) {
        if( get_slurm_job_shape_from_env(job_ll, job_ur, block_shape) )
	   exit(-1);

        job_corner = job_ll;

        job_shape.aCoord = job_ur.aCoord - job_ll.aCoord + 1;
        job_shape.bCoord = job_ur.bCoord - job_ll.bCoord + 1;
        job_shape.cCoord = job_ur.cCoord - job_ll.cCoord + 1;
        job_shape.dCoord = job_ur.dCoord - job_ll.dCoord + 1;
        job_shape.eCoord = job_ur.eCoord - job_ll.eCoord + 1;

    } else {
	// single node
	job_corner = node_coords;
	job_shape.aCoord = job_shape.bCoord = job_shape.cCoord =
	job_shape.dCoord = job_shape.eCoord = 1;
    }

    SFJS_DEBUG("block <%d,%d,%d,%d,%d> job <%d,%d,%d,%d,%d> - <%d,%d,%d,%d,%d> ||  <%d,%d,%d,%d,%d> +  <%d,%d,%d,%d,%d>\n",
	block_shape.aCoord, block_shape.bCoord, block_shape.cCoord,
	block_shape.dCoord, block_shape.eCoord,
	job_ll.aCoord, job_ll.bCoord, job_ll.cCoord,
	job_ll.dCoord, job_ll.eCoord,
	job_ur.aCoord, job_ur.bCoord, job_ur.cCoord,
	job_ur.dCoord, job_ur.eCoord,
	job_corner.aCoord, job_corner.bCoord, job_corner.cCoord,
	job_corner.dCoord, job_corner.eCoord,
	job_shape.aCoord, job_shape.bCoord, job_shape.cCoord,
	job_shape.dCoord, job_shape.eCoord);

    PrimordialClassRoute_t primordial_cr;

    // IV. complete block, subblock, or single node 
    if( job_size == block_size ) {
        // IVa: for complete block:
        // use block GI barrier to setup job GI barrier
	if( read_primordial_cr( &primordial_cr))
	    return -1;

	if(setup_job_barrier_for_block(primordial_cr))
	    exit(-1);

    } else if ( job_size > 1 ) {
        // IVb: for subblock:
        //	    get stage from parameter:
	if( !args.subblock_given ) {
	    fprintf(stderr, "for running job on a subblock, need to specifiy subblock preparation stage\n");
	    sfjs_cmdline_parser_print_help();
	}

	switch(args.subblock_arg) {
        // IVb1 prep: set class route for job GI barrier
	    case subblock_arg_prep:
		if(setup_job_barrier_for_subblock_prep( job_ll,
			job_ur, node_coords ))
		    exit(-1);
		exit(0); // jobstep for setting class route ends here
		break;
        // IVb2 enable: set own contribution for job GI barrier
	    case subblock_arg_gion:
		if(setup_job_barrier_for_subblock_gion( job_size, block_size ))
		    exit(-1);
		exit(0); // jobstep for enabling GI barrier ends here
		break;
        // IVb3 go: move on 
	    case subblock_arg_go:
		// just open the class route file
		if( open_classroute(JOB, GI) == -1)
		    exit(-1);
		break;
	    default:
		SFJS_DEBUG("implementation error, unknown parameter value\n");
		exit(-1);
	}
    } else {
        // IVc: for single node, skip step
    }

    // V: go on setup user GI barrier and collective

    // Va: for complete block or subblock
    if( job_size > 1 ) {
    //	use job GI barrier to setup user class routes (GI + collective)
	if( setup_user( job_size, block_size, primordial_cr,
		job_ll, job_ur,
		node_coords) )
	    exit(-1);
    } else {
    // Vb: for single node
    //  configure local-only user class routes
    //  massage environment for PAMI
	if( setup_user_single_node(block_shape, node_coords) )
	    exit(-1);
    }

    // VI launch cl
    int status = launch_cl( argc, argv, job_size, args,
	    job_corner, job_shape);

    // VII reset job GI barrier to inactive, disable user collective,
    //     disable user GI 
    Kernel_DeallocateGlobalInterruptClassRoute(USER); // MUFS disables CR
    Kernel_DeallocateGlobalInterruptClassRoute(JOB);
    Kernel_DeallocateCollectiveClassRoute(USER); // MUFS disables CR

    ClassRoute_t disabled_cr;
    disabled_cr.input = 0;
    disabled_cr.output = 0;
    setup_classroute(JOB, disabled_cr, GI);

    // finally, exit
    exit(status);
}
