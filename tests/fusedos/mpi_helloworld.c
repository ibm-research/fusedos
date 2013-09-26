/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ================================================================ */
/* IBM Confidential                                                 */
/*                                                                  */
/* Licensed Machine Code Source Materials                           */
/*                                                                  */
/* Product(s):                                                      */
/*     Blue Gene/Q Licensed Machine Code                            */
/*                                                                  */
/* (C) Copyright IBM Corp.  2010, 2011, 2012                        */
/*                                                                  */
/* The Source code for this program is not published  or otherwise  */
/* divested of its trade secrets,  irrespective of what has been    */
/* deposited with the U.S. Copyright Office.                        */
/* ================================================================ */
/*                                                                  */
/* end_generated_IBM_copyright_prolog                               */

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

#include <firmware/include/personality.h>
#include <spi/include/kernel/location.h>

// Basic MPI application for MPI bringup on FusedOS
//

#define BUFSIZE 100
int main(int argc, char *argv[]) {

	int myrank;
	int numprocs;

	MPI_Status status;

	char buf[BUFSIZE];
	char *msg = "Hello World!";

	// get torus coordinates (see mu-examples/msg_pingpong.c)
	Personality_t pers;
	Kernel_GetPersonality(&pers, sizeof(pers));
	
	int32_t     my_a = pers.Network_Config.Acoord;
	int32_t     my_b = pers.Network_Config.Bcoord;
	int32_t     my_c = pers.Network_Config.Ccoord;
	int32_t     my_d = pers.Network_Config.Dcoord;
	int32_t     my_e = pers.Network_Config.Ecoord;
	int32_t     my_t = Kernel_PhysicalProcessorID();
	
	uint32_t rank = Kernel_GetRank();


	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// print rank and coordinates
	printf("MPI rank %4d, CNK rank %4d: We are the node with coords %2d %2d %2d %2d %2d %2d (A,B,C,D,E,T)\n", 
			myrank, rank, my_a, my_b, my_c, my_d, my_e, my_t);


	// send from first to last
	memset(buf, 0, BUFSIZE);
	if(myrank == 0) {
		strcpy(buf, msg);
		MPI_Send(buf, BUFSIZE, MPI_CHAR, numprocs-1, 0, MPI_COMM_WORLD);
		printf("rank %d; sent message.\n", myrank);
	} else if(myrank == numprocs-1) {
		MPI_Recv(buf, BUFSIZE, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("rank %d; received message \"%s\"\n", myrank, buf);
	}


	// send from last to first
	memset(buf, 0, BUFSIZE);
	if(myrank == numprocs-1) {
		strcpy(buf, msg);
		printf("rank %d; sending message.\n", myrank);
		MPI_Send(buf, BUFSIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else if(myrank == 0) {
		printf("rank %d; receiving message.\n", myrank);
		MPI_Recv(buf, BUFSIZE, MPI_CHAR, numprocs-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("rank %d; received message \"%s\"\n", myrank, buf);
	}


	// global barrier
	printf("rank %d, waiting for barrier.\n", myrank);
	if(myrank == 7) {
		printf("rank 7: delaying, hihihihi.\n");
		sleep(3);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	printf("rank %d done, finalizing.\n", myrank);

	// TODO global reduce / scatter gather
	

	MPI_Finalize();
	return 0;
}
