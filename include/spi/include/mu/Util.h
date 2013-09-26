/*                                                                  */
/* Licensed Materials - Property of IBM                             */
/*                                                                  */
/* Blue Gene/Q                                                      */
/*                                                                  */
/* (c) Copyright IBM Corp. 2011, 2012 All Rights Reserved           */
/*                                                                  */
/* US Government Users Restricted Rights - Use, duplication or      */
/* disclosure restricted by GSA ADP Schedule Contract with IBM      */
/* Corporation.                                                     */
/*                                                                  */
/* This software is available to you under either the GNU General   */
/* Public License (GPL) version 2 or the Eclipse Public License     */
/* (EPL) at your discretion.                                        */
/*                                                                  */

#ifndef	_MUSPI_UTIL_H_ /* Prevent multiple inclusion */
#define	_MUSPI_UTIL_H_


/**
 * \file Util.h
 *
 * \brief C Header File containing Message Unit SPI Utility Definitions
 *
 */


#include "kernel_impl.h" 
#include <hwi/include/common/compiler_support.h>


__BEGIN_DECLS

/** \mainpage System Programming Interfaces (SPI)

SPIs are provided for programmers who may wish to program at a level closest to
the hardware.  Guidance for specific areas are provided in the following pages:

- \subpage muSpis

*/

//-----------------------------------------------------------

/** \page muSpis Message Unit (MU) SPIs

This page provides an overview of the Message Unit (MU) SPIs.

The MU SPIs interface with the MU hardware.  Refer to \subpage muHardware for a
description of the MU hardware.  You will need to understand this before
programming the MU.

Programming the MU involves the following:

- \subpage muResources
- \subpage muDescriptors
- \subpage muPolling
- \subpage muAddressing
- \subpage muCoordinates
- \subpage muClassroutes
- \subpage muGlobalInterrupts
- \subpage muBarriers

A program that uses the MU SPIs can be the only user of the MU, or it can share
the MU with the communications component (MPI, PAMI, etc).  Refer to \ref
muSharing for details about sharing the MU.

There are examples here:
- \ref msg_common.h
- \ref msg_common.c
- \ref msg_alltoall.c
- \ref msg_collective.c
- \ref msg_pingpong.c

*/

//-----------------------------------------------------------

/** \page muHardware MU Hardware

This page provides an overview of the Messaging Unit (MU) hardware.

The MU provides data movement between the local memory system and the network.
It provides message injection where the MU divides the message into packets and
sends them to the network.  It provides packet reception where the MU receives
packets from the network and writes them into memory in one of three ways:

- Memory FIFO transfer:  Packets are written into a reception memory FIFO
pre-allocated by the MU user.  The MU user polls the memory FIFO, extracts the
packets, and moves the data to the final memory destination.
- Direct Put transfer:  Packet data is written directly into to the final memory
destination, at an offset from a pre-programmed base address.
- Remote Get transfer:  Packet payload is written into an injection memory FIFO
pre-allocated by the MU user.  The payload contains descriptors describing
messages to be sent.

\section muHWInjection Injection

Injection FIFOs are used to tell the MU to send a message.  An injection FIFO is
a contiguous block of memory allocated by the MU user.  It contains zero or more
"descriptors", each describing a message to be sent.  The injection FIFO has a
START address, a SIZE measured in number of descriptors, a HEAD address and a
TAIL address.  Before the FIFO is first used, START, HEAD, and TAIL all refer to
the beginning of the FIFO.  When the FIFO is empty, HEAD and TAIL are equal.

To tell the MU to send a message, the MU user creates and copies a descriptor to
the TAIL of the injection FIFO and moves the TAIL past that descriptor.  The
movement of the TAIL tells the MU to start sending messages, beginning with the
descriptor at the HEAD.

The MU processes the descriptors in the injection FIFO sequentially.  After
completely processing a descriptor, the MU moves the HEAD past that descriptor.
The MU continues processing descriptors until HEAD equals TAIL.

The MU processes a descriptor by dividing the message into packets and sending
those packets to the network.  When the last packet for the message has been
sent to the network, the descriptor is done.  The MU increments a count of
descriptors processed for this injection FIFO and moves the HEAD.

When HEAD or TAIL reaches the end of the injection FIFO, they wrap around to the
START.

When the injection FIFO is full of descriptors, no more can be injected until
the MU completes a descriptor and moves the HEAD.

The MU user can set up multiple injection FIFOs.  The MU can process up to 16
injection FIFOs in parallel on a node.  Thus, having multiple injection FIFOs
increases parallelism.

\section muHWReception Reception

There are three kinds of reception:  Memory FIFO, Direct Put, and Remote Get.

\subsection muHWMemFIFOReception Memory FIFO Reception

Reception FIFOs hold received memory FIFO packets until the MU user processes
them.  A reception FIFO is a contiguous block of memory allocated by the MU user.
It contains zero or more received packets.  The reception FIFO has a START
address, a SIZE measured in bytes, a HEAD address, and a TAIL address.  Before
the FIFO is first used, START, HEAD, and TAIL all refer to the beginning of the
FIFO.  When the FIFO is empty, HEAD and TAIL are equal.

Each memory FIFO packet contains the ID of the target reception FIFO.  When a
memory FIFO packet arrives at a node, the MU extracts the ID, maps it to a
particular reception FIFO, copies the packet into the fifo at the TAIL, and moves
the TAIL past the packet.  The movement of the TAIL tells the MU user that there
are packets in the FIFO to be processed.

The MU user processes the packets in the reception FIFO sequentially, starting at
the HEAD.  Each packet must be interpreted by the MU user and any data in the
packet must be copied out of the reception FIFO.  After completely processing a
packet, the MU user moves the HEAD past that packet.  The MU user continues
processing packets until HEAD equals TAIL.

When HEAD or TAIL reaches the end of the reception FIFO, they wrap around to the
START.

When the reception FIFO is full of packets, no more can be received into that
FIFO until the MU user completes a packet and moves the HEAD.  Packets can "back
up" in the network such that the sending MU cannot send more packets to the
network.  In that case, the descriptor in the injection FIFO remains there,
causing the other descriptors in the injection FIFO to remain there too,
eventually filling the injection FIFO.  Everything automatically starts moving
again when space the MU user processes packets in the reception FIFO.

The MU user can set up multiple reception FIFOs.  The MU can process up to 16
reception FIFOs in parallel on a node.  Thus, having multiple reception FIFOs
increases parallelism.

\subsection muHWDirectPutReception Direct Put Reception

A direct put transfer is where packet data is written directly into to the final
memory destination at an offset from a pre-programmed base address.  The base
address is set up on the receiving node by the MU user in one of several base
address slots.  The sending MU user puts the base address table slot number into
the descriptor, together with the offset from that base address of the final
memory destination.

Additionally, the receiving MU user has a counter, located at a base address and
offset, for each message.  The counter is initialized by the receiving MU user
to the length of the message before the message transfer begins, and is
decremented by the receiving MU as packets arrive for that message.  When the
counter's value hits zero, the receiving MU user knows the message has been
fully received.  The sending MU user puts the counter's base address table slot
number into the descriptor, together with the offset from that base address of
the counter.

The sending MU user must know the receiving node's base address table slot
number and offset for both the data buffer and the counter.  Either they are
well-known, or they are exchanged between the MU users prior to the direct put
transfer.

On the sending side, the MU sees that the descriptor is a direct put transfer.
The MU divides the message into packets.  Each packet contains the base address
table slot numbers and offsets.  The MU tranfers the counter's base address table
slot number and offset, and the data's base address table slot number unchanged
in each packet.  The first packet's data offset from the data's base address is
the initial value that is in the descriptor.  Since a packet holds 512 bytes, the
MU adds 512 to the data offset in each subsequent packet.

For each direct put packet that is received, the MU sums the data's base address
and the data's offset and stores the packet's data at that memory location.
Additionally, the receiving MU sums the counter's base address and the counter's
offset and decrements the counter at that memory location.  Refer to
\ref muHWAtomics for more detail.

\subsection muHWRemoteGet Remote Get Reception

A remote get transfer is where the packet's data is written into an injection
FIFO pre-allocated by the MU user, and designated as a remote get injection FIFO.
The packet's data contains descriptors describing messages to be sent.

The sending MU processes a remote get descriptor like it does a memory FIFO
descriptor.  The data being transferred is a set of descriptors describing
messages on the receiving node to be sent somewhere.  Usually those messages are
sent back to the node sending the remote get, but they could be sent to any
node in the job.

The receiving MU sees that the packet is a remote get.  The MU copies the
packet's data (the descriptors) into the remote get injection FIFO whose FIFO ID
is in the packet's header, as set up by the sending MU user, and moves the TAIL
past those descriptors.  This activates the MU to process those descriptors, as
if they had been injected locally on the node receiving the remote get.

Because there is the potential for deadlock should the remote get injection FIFO
become full, one of the following is recommended:

- Use flow control to ensure that the remote get injection FIFO never becomes
full.
- The remote get should be sent on the high priority virtual channel and use the
high priority torus FIFO.  Additionally, no other messages should use the high
priority virtual channel or high priority torus FIFO.

\section muHWAtomics MU Atomics

The MU uses atomic operations as designated in the message descriptor by the MU
user.  Specifically, the MU can do the following:

- Decrement the counter used by a direct put.
- Perform an atomic operation on the data being sent.
- Perform an atomic operation on the storage being received into.

The MU knows that the counter or data is to be treated atomically by bits encoded
in the address of the counter or data.  The MU uses 37-bit addresses.  The
high-order bit is the atomic bit that, when set, indicates this is an atomic
address.  The low-order 36 bit are the address of the counter or data.  However,
the counter or data must be aligned on an 8-byte boundary, making the low-order
three bits zero.  Those low-order 3 bits are used to encode the atomic op-code
indicating what atomic operation is to be done.  The counter or data involved in
the atomic operation must be 8-bytes in length.

The address of the counter used for a direct put must be an MU atomic address.
This means the sum of the counter's base address and the counter's offset must
have the high-order bit set, and the atomic op-code in the low-order 3 bits.  The
op-code must be either a store-add or a store-add-with-coherence-on-zero.  The MU
stores a negative one to this address, decrementing the counter.

In a message descriptor, the address of the data to be sent may be an MU atomic
address.  The length of the data being sent must be 8 bytes.  The atomic
operation designated in the atomic op-code will be performed by the MU as it
loads the 8-bytes of data into the packet.  Thus, the op-code should be some
type of load.

For a direct put, the sum of the data's base address and the data's offset may be
an MU atomic address.  The op-code may be some type of store.  The atomic
operation is performed as the data is being stored to the destination memory.
The length of the message must be 8 bytes.

\section muHWAddressing MU Addressing

The MU works with physical addresses.  The addresses of the injection and
reception FIFOs, data, and counters must be physical addresses.  There are MU
SPIs that facilitate the conversion of a virtual address to a physical address.
There are also MU SPIs that facilitate the construction of an MU atomic
address.  Refer to \ref muAddressing.

\section muHWDescriptors MU Descriptors

A MU descriptor describes data to be sent.  A descriptor is 64-bytes.  The first
32-bytes contain descriptive information, while the second 32-bytes are a
packet-header model used by the MU to construct the packet headers.  The MU user
constructs a descriptor, copies it into an injection FIFO, and moves the
FIFO's TAIL to cause the MU to send the described message.

The descriptive information in the descriptor includes the message's physical
address and length, and a bit mask indicating which torus FIFO(s) can be used by
the MU to send the packets to the network.

The packet header portion of the descriptor contains a network header (for either
point-to-point or collective messages) and a MU header (that is different for
memory FIFO, direct put, and remote get transfers).

There are MU SPIs to build a descriptor.  Refer to \ref muDescriptors.

\section muHWCollectives MU Collectives

The MU can perform collective operations.  The MU SPI application injects a
collective descriptor into an injection FIFO.  The descriptor is similar to
a point-to-point descriptor, except it additionally specifies a class route and
an opcode.  The class route describes this node's topology in the collective
network tree, indicating which network links are input and which network link
is output.  The network hardware performs the collective operation specified
by the opcode on the data when it receives all of the inputs from the nodes
down the tree, and passes the result to the node up the tree.  When the root
node at the top of the tree has received all of its inputs and performed
the collective operation, the final result is broadcast back down the tree
and received at each node.

The collective operations include broadcast, reduce, and allreduce.  The
opcodes include AND, OR, XOR, and unsigned, signed, and floating point ADD,
MIN, and MAX.

\section muHWGlobalInterrupts MU Global Interrupts and Barrier

The MU can be programmed to send a global interrupt to the nodes participating
in a global interrupt class route.  The MU can be programmed to generate an
interrupt, or the status can be polled to determine the global interrupt has
occurred.

The MU has a barrier built upon the global interrupt, called the global
interrupt barrier.  It allows the nodes participating in a global interrupt
class route to perform a barrier.

\section muHWResources MU Resources

MU resources include injection FIFOs, reception FIFOs, base address table
entries, collective class routes, and global interrupt class routes.
On a node, there can be up to 544 injection FIFOs, 272 reception FIFOs,
544 base address table entries, 16 collective class routes, and 16 global
interrupt class routes.

The FIFOs and base address table entries are evenly divided into 17 groups
(32 injection FIFOs, 16 reception FIFOs, and 32 base address table entries per
group).  They are further subdivided into 4 subgroups per group (8 injection
FIFOs, 4 reception FIFOs, and 8 base address table entries per subgroup).  Each
group is associated with a core, and each subgroup is associated with a
hardware thread.  Typically, any FIFO or base address table entry can be used
by software running on any core or hardware thread.  The association with
a core is for MU interrupts.  MU interrupts are delivered to the core associated
with the MU resource.

The application runs on the first 16 cores of a node.  It should use the MU
resources from the groups and subgroups associated with those cores.  By
convention, The last group associated with the 17th core is used by MPI/PAMI
messaging, application agents, and the compute node kernel (CNK).  Specifically,
MPI/PAMI messaging and its comm agent use some (but not all) of the MU resources
in the first two subgroups of the 17th core (subgroups 64 and 65).  CNK uses
some (but not all) of the MU resources in the last two subgroups of the 17th
core (subgroups 66 and 67).  Your MU SPI application always runs with CNK, so
your application should avoid using MU resources in subgroups 66 and 67.
CNK also uses collective and global interrupt class routes 14 and 15, and
sets up class route 0 to include all nodes in the job.
Everything else is available to your application.  If your MU SPI application
also uses MPI/PAMI messaging, then there are further restrictions documented
in \ref muSharing.

*/

//-----------------------------------------------------------

/** \page muResources Allocating and Initializing MU Resources

This page describes how to allocate and initialize Messaging Unit (MU) resources.

The following MU resources need to be allocated and initialized by the MU SPI
application before any messages can be sent:

- \subpage muInjectionFifos
- \subpage muReceptionFifos
- \subpage muBatIDs

Optionally, if the MU SPI application uses class routes for collectives,
global interrupts, or global interrupt barriers, it needs to set up the class
routes.  Refer to \ref muClassroutes.

When sharing the MU with MPI or PAMI, these resources must be shared.  Refer to
\ref muSharing for details about sharing the MU.

*/

//-----------------------------------------------------------

/** \page muInjectionFifos Allocating and Initializing MU Injection FIFOs

This page describes how to allocate and initialize Message Unit (MU) injection
FIFOs.

-# Decide how many injection FIFOs to use.  The more injection FIFOs, the
more parallelism there will be.  There are ten torus network FIFOs used by the
hardware to send packets in each of the ten torus directions (A-, A+, B-, B+,
C-, C+, D-, D+, E-, E+).  It makes sense to allocate ten MU injection FIFOs that
correspond with these ten directions.  In this way, ten descriptors can be
injected at a time, one in each MU injection FIFO, and the MU will feed packets
into the corresponding ten torus network FIFOs in parallel. Additionally, if the
MU SPI application has multiple threads, there can be a set of ten MU injection
FIFOs for each thread so the threads can inject descriptors into its FIFOs
without interference from the other threads (no locking required).  This is what
PAMI does.  For collective operations, it is recommended to have one additional
FIFO for injecting collective descriptors.  One of these is sufficient.  At a
minimum, one MU injection FIFO is required.
If the MU SPI application uses remote gets, a single set of remote get injection
FIFOs per node need to be allocated and initialized.  These can be safely shared
among all of the processes on the node because only the MU injects descriptors
into them.  Similar to regular injection FIFOs, there could be ten of these,
one for each direction, or at a minimum, one.
-# Decide which subgroups and which injection FIFO IDs within those
subgroups to allocate.  In general, try to confine the MU resources to subgroups
that correspond to the hardware threads in your process.  Use the
\ref Kernel_ThreadMask SPI to get the list of hardware thread IDs (0..63) in
the process.  Those IDs correspond to the subgroups that should be used.  It is
allowed to use subgroups outside of this range, but keep in mind that those
may need to be used by other processes on the node.
If MPI or PAMI is also being used in the MU SPI application, the number of
injection FIFOs and their location is important because MPI or PAMI are also
using injection FIFOs from the same set of subgroups.  Refer to \ref muSharing
for more information.
-# Use \ref Kernel_QueryInjFifos to determine which FIFOs in which subgroups are
free.  Then, use \ref Kernel_AllocateInjFifos to allocate them.
-# Allocate space for the injection FIFOs themselves, for example, from the
heap.  The size of each FIFO must be at least 128 bytes (576 bytes if it is
used for remote gets) and a multiple of 64 bytes (which is the size of one
descriptor).  PAMI uses the \ref MUSPI_INJFIFOSIZE environment variable to determine
the size of its injection FIFOs (64 KB default).
-# Convert the virtual address of each injection FIFO to a physical address.
Do this using \ref Kernel_CreateMemoryRegion.  Calculate the physical address
as memoryRegion.BasePa + (virtualAddress - memoryRegion.BaseVa).
-# Use \ref Kernel_InjFifoInit to initialize each injection FIFO.
-# Use \ref Kernel_InjFifoActivate to activate the injection FIFOs.
-# At this point, the injection FIFOs are ready to use.
-# When the MU SPI application is ready to exit, use \ref Kernel_InjFifoActivate
to deactivate and \ref Kernel_DeallocateInjFifos to deallocate the injection
FIFOs.
.

*/

//-----------------------------------------------------------

/** \page muReceptionFifos Allocating and Initializing MU Reception FIFOs

This page describes how to allocate and initialize Message Unit (MU) reception
FIFOs.  Reception FIFOs are used for memory FIFO transfers.

-# Decide how many reception FIFOs to use.  The more reception FIFOs, the
more parallelism there will be.  There are ten torus network FIFOs used by the
hardware to receive packets from each of the ten torus directions (A-, A+, B-, B+,
C-, C+, D-, D+, E-, E+).  Each memory FIFO packet designates a specific MU
reception FIFO where the packet will be copied.  There can be multiple MU reception
FIFOs, enabling parallelism.  However, because each thread that is polling reception
FIFOs has overhead to poll them, having one reception FIFO per polling thread may
be optimal (that's what PAMI does).  At least one reception FIFO is required when
using memory FIFO transfers.
-# Decide which subgroups and which reception FIFO IDs within those
subgroups to allocate.  In general, try to confine the MU resources to subgroups
that correspond to the hardware threads in your process.  Use the
\ref Kernel_ThreadMask SPI to get the list of hardware thread IDs (0..63) in
the process.  Those IDs correspond to the subgroups that should be used.  It is
allowed to use subgroups outside of this range, but keep in mind that those
may need to be used by other processes on the node.
If MPI or PAMI is also being used in the MU SPI application, the number of
reception FIFOs and their location is important because MPI or PAMI are also
using reception FIFOs from the same set of subgroups.  Refer to \ref muSharing
for more information.
-# Use \ref Kernel_QueryRecFifos to determine which FIFOs in which subgroups are
free.  Then, use \ref Kernel_AllocateRecFifos to allocate them.
-# Allocate space for the reception FIFOs themselves, for example, from the
heap.  The size of each FIFO must be at least 576 bytes and a multiple of 32 bytes.
PAMI uses the \ref MUSPI_RECFIFOSIZE environment variable to determine
the size of its reception FIFOs (1 MB default).
-# Convert the virtual address of each reception FIFO to a physical address.
Do this using \ref Kernel_CreateMemoryRegion.  Calculate the physical address
as memoryRegion.BasePa + (virtualAddress - memoryRegion.BaseVa).
-# Use \ref Kernel_RecFifoInit to initialize each reception FIFO.
-# Use \ref Kernel_RecFifoEnable to enable the reception FIFOs.
-# At this point, the reception FIFOs are ready to use.
-# When the MU SPI application is ready to exit, use \ref Kernel_RecFifoDisable
to disable and \ref Kernel_DeallocateRecFifos to deallocate the reception
FIFOs.
.

*/

//-----------------------------------------------------------

/** \page muBatIDs Allocating and Initializing MU Base Address Table Entries

This page describes how to allocate and initialize Message Unit (MU) base
address table entries.  Base address table entries are used for direct put
transfers.

-# Decide how many base address table entries to use.  Base address table entries
are used for both the data buffer and the MU reception counter, so possibly two
entries per transfer may be used.  Note that the same base address may be used
for multiple direct put transfers.
-# Decide which subgroups and which base address table entry IDs within those
subgroups to allocate.  In general, try to confine the MU resources to subgroups
that correspond to the hardware threads in your process.  Use the
\ref Kernel_ThreadMask SPI to get the list of hardware thread IDs (0..63) in
the process.  Those IDs correspond to the subgroups that should be used.  It is
allowed to use subgroups outside of this range, but keep in mind that those
may need to be used by other processes on the node.
If MPI or PAMI is also being used in the MU SPI application, the number of
base address table entries and their location is important because MPI or PAMI
are also using base address table entries from the same set of subgroups.
Refer to \ref muSharing for more information.
-# Use \ref Kernel_QueryBaseAddressTable to determine which entries in which
subgroups are free.  Then, use \ref Kernel_AllocateBaseAddressTable to allocate
them.
-# Convert the virtual addresses of the reception buffer and reception counter
to a physical address.  Do this using \ref Kernel_CreateMemoryRegion.  Calculate
the physical address as memoryRegion.BasePa + (virtualAddress - memoryRegion.BaseVa).
-# Use \ref MUSPI_SetBaseAddress to set the physical addresses into the base address
table entries.
-# If needed, use \ref MUSPI_GetOffsetFromBaseAddress to calculate the offset of
a buffer from the base address in a base address table entry.
-# Use \ref MUSPI_GetAtomicOffsetFromBaseAddress to calculate an offset encoded
for MU atomics from a base address.  This can be useful for obtaining the offset
for the reception counter. 
-# When the MU SPI application is ready to exit, use
\ref Kernel_DeallocateBaseAddressTable to deallocate the base address table
entries.
.

*/

//-----------------------------------------------------------

/** \page muDescriptors Injecting Message Descriptors

This page describes how to create and inject message descriptors.

The descriptor (\ref MUHWI_Descriptor) consists of a base section and a
packet header that is used as a model fo the MU to build the packet headers
for the message.  The base section contains prefetch and interrupt bits,
the physical address of the message buffer to be sent, the message length, and
a torus FIFO map that tells the MU which network torus FIFOs to use to send
the message.  The packet header (\ref MUHWI_PacketHeader) consists of either
a point-to-point (\ref MUHWI_Pt2PtNetworkHeader) or a collective network header
(\ref MUHWI_CollectiveNetworkHeader) and a message unit header.
The message unit header (\ref MUHWI_MessageUnitHeader) is one of three forms
depending on the type of the message transfer: memory fifo, direct put, or
remote get.

There are two sets of functions that create descriptors: C and C++.

C functions that create descriptors are found in \ref Descriptor.h and
\ref Descriptor_inlines.h.  There are two ways to create descriptors using
these C functions:

-# Calling a function that builds the entire descriptor at once.  The input to
the function is a structure of sub-structures containing information that
is used to create the descriptor.  There are different functions for creating
different kinds of descriptors.  This is a simple way, but it does not perform
optimally because the sub-structures first has to be filled-in, and then the
function extracts that information and puts it into the descriptor.  This type
of function could be used to build a "model" descriptor during the
initialization phase of the MU SPI application, where performance is not
important.  Then, during the messaging phase of the application, the model
descriptor is copied into the injection FIFO and then method 2 described below
is used to set only the fields that are different from the model.  The
implementation for these functions that build the entire descriptor at once is
in the libSPI.a library.  These functions include:
 - \ref MUSPI_CreatePt2PtMemoryFIFODescriptor
 - \ref MUSPI_CreatePt2PtDirectPutDescriptor
 - \ref MUSPI_CreatePt2PtRemoteGetDescriptor
 - \ref MUSPI_CreateCollectiveMemoryFIFODescriptor
 - \ref MUSPI_CreateCollectiveDirectPutDescriptor
 - \ref MUSPI_CreateCollectiveRemoteGetDescriptor
 .
-# Calling inline functions that set individual fields into the descriptor.
.

C++ functions that create descriptors are found in the spi/include/mu/zzzzXX.h
include files.  \ref MUSPI_DescriptorBase is the base descriptor class.  It
has individual member functions that set all of the fields of the descriptor.
Derived off this base class are six sub-classes that each define a particular
type of descriptor:
 - \ref MUSPI_Pt2PtMemoryFIFODescriptor
 - \ref MUSPI_Pt2PtDirectPutDescriptor
 - \ref MUSPI_Pt2PtRemoteGetDescriptor
 - \ref MUSPI_CollectiveMemoryFIFODescriptor
 - \ref MUSPI_CollectiveDirectPutDescriptor
 - \ref MUSPI_CollectiveRemoteGetDescriptor
 .
These sub-classes each have two constructors.  
-# The full constructor takes a structure as input that contains all of the
information necessary to create a descriptor.  This constructor builds the
entire descriptor in this one call.  It is similar to the C function described
above that builds the entire descriptor at once.
-# The default constructor zeros out the descriptor and sets a few fields
unique to this type of descriptor.  The remaining fields must be set using
the individual member functions.
.

A descriptor must be injected into an injection FIFO in order for the MU to
send the message associated with that descriptor.  There are two ways to inject
a descriptor:

-# Using \ref MUSPI_InjFifoInject.  The descriptor that is already built in
storage is passed as input to this function.  This function copies the
descriptor to the tail of the specified injection FIFO and moves the tail past
that descriptor, essentially notifying the MU that the descriptor is ready to
process.
-# Using \ref MUSPI_InjFifoNextDesc and \ref MUSPI_InjFifoAdvanceDesc.  The
descriptor is not yet ready to inject.  MUSPI_InjFifoNextDesc is called to
find the next slot available in the injection FIFO, and it returns a pointer
to that slot.  The MU SPI application then builds the descriptor in that
slot (for example, is copies the model descriptor into the slot, and calls
descriptor functions to set some individual fields into that descriptor), and
then calls MUSPI_InjFifoAdvanceDesc to move the injection FIFO's tail past
that descriptor, essentially notifying the MU that the descriptor is ready to
process.
.

MUSPI_InjFifoInject and MUSPI_InjFifoNextDesc may return an error indicating
that the injection FIFO is full.  The MU SPI application can either retry
(hoping that the MU has processed one or more descriptors to free
up a slot in the injection fifo) or queue the descriptor on its own internal
queue so the descriptor can be injected at a later time.

When successful, MUSPI_InjFifoInject and MUSPI_InjFifoAdvanceDesc return
the descriptor number of the descriptor that was injected.  This descriptor
number can later be passed into \ref MUSPI_CheckDescComplete to determine
whether the descriptor has been completely processed by the MU (the last
packet for the message has been sent to the network).

*/

//-----------------------------------------------------------

/** \page muPolling Polling for Packets

This page describes how to poll for packets.

The descriptor for a memory FIFO transfer contains the reception FIFO number
on the remote node where the packets for the message will be received.  The
receiving MU SPI application must poll its reception FIFOs in order to
receive those packets.  There are two ways to poll:

-# Use \ref MUSPI_RecFifoPoll. Requires pre-registration of functions to handle
different types of packets. Pointers to the registered functions are stored
in a static array. The array index is the registration ID. The MU SPI application
must store the registration ID in the first byte of the "software bytes" field of
the memory FIFO packet header in the descriptor. Specifically, the first byte of
the Unused2 field of the Memory_FIFO message unit header. When the packet arrives
in the fifo, the poll function extracts this registration ID and calls the
function registered in the table.  A single call to this poll function may
process multiple packets, one at a time, calling the designated function
once per packet.  This requires that the MU SPI application be linked with the
libSPI.a library.
-# \ref MUSPI_RecFifoExamplePoll.  This is an example of how inline helper
functions can be used to poll the reception fifo and process the packets.
Using these functions provides more flexibility for the MU SPI user. No
pre-registration of functions and no function ID in the packet header are
required. The MU SPI user can recognize the packet using any technique it
wants, and call whatever function it wants with whatever parameters it wants
that function to have. In this example function, the call to the function could
be replaced by a switch statement that calls different functions with different
parameters. Just copy this example function into the MU SPI application and
modify it as you wish to recognize the packet and perform whatever processing
is necessary.
.

*/


//-----------------------------------------------------------

/** \page muAddressing Addressing

This page describes how address storage used by the Message Unit (MU).

The MU uses physical addresses to address FIFOs, buffers, and reception counters.
To convert a virtual address to a physical address, use
\ref Kernel_CreateMemoryRegion to create a memory region.  This takes the
virtual address and length of the buffer as input.  It fills in a memory region
structure with the length of the memory region, its physical address, and the
original virtual address.  This region spans the specified virtual address and
length.  It may span much more, but it is guaranteed to contain the specified
buffer.  From this memory region, the physical address = memoryRegion.BasePa +
(virtualAddress - memoryRegion.BaseVa).

*/


//-----------------------------------------------------------

/** \page muCoordinates Coordinates

This page describes how to work with node coordinates.

A job consists of a set of nodes, each node addressed by its ABCDE coordinates.
The \ref Kernel_JobCoords SPI returns the ABCDE coordinates of the "corner" node
for the job, and the sizes of the ABCDE dimensions stemming from that corner
node (known as the "shape").  The "full" size of the job is the product of the
shape's dimension sizes.

The coordinates of each node in the job relative to the corner node are obtained
from \ref Kernel_RanksToCoords.  It takes as input a pointer to the array where
the coordinates are returned.  There must be enough storage for "full" size
number of coordinates.  The "actual" number of nodes in the job is also returned.
This will be less than or equal to the "full" size.

The MU works with absolute coordinates, not relative.  The destination node's
coordinates placed in a descriptor must be absolute.  The absolute coordinates
of a node are the sum of the corner node's coordinates and the coordinates of
that node obtained from the Kernel_RanksToCoords array.

Following the procedures defined above will enable the MU SPI application to
work with both full blocks and subblocks.

For example, if the job is run in a full 512-node block, the corner node's
coordinates will always be (0,0,0,0,0) and the shape will be 4x4x4x4x2.  For
a particular node, Kernel_RanksToCoords returned its relative coordinates as
(2,3,1,4,0).  When sending to this node, the descriptor must contain its
absolute coordinates of (0,0,0,0,0) + (2,3,1,4,0) = (2,3,1,4,0).

A subblock example:  A job is run in a subblock of a 512 node block.  The
corner node has coordinates (0,1,2,3,1) and the shape of the subblock is
(2x2x2x1x1).  The full size is 8 nodes.  Assuming --np is not specified
on runjob, the actual size is 8 nodes.  For a particular node,
Kernel_RanksToCoords returned its relative coordinates as (1,0,1,0,0).
When sending to this node, the descriptor must contain its absolute
coordinates of (0,1,2,3,1) + (1,0,1,0,0) = (1,1,3,3,1).

*/


//-----------------------------------------------------------

/** \page muClassroutes Class Routes

The MU uses class routes to perform collectives, global interrupts, and
global interrupt barriers.  There are 16 collective class routes and 16
global interrupt class routes per node.  CNK uses class routes 14 and 15.
CNK sets up class route 0 to include all nodes in the job, so it can be
used to perform collectives, global interrupts, and global interrupt barriers
when all nodes in the job are participating.  

The MU SPI application can program the other class routes when subsets of
nodes are participating.

- Use \ref Kernel_QueryCollectiveClassRoutes and
\ref Kernel_QueryGlobalInterruptClassRoutes to find out which class routes
are available.
- Use \ref Kernel_AllocateCollectiveClassRoute and
\ref Kernel_AllocateGlobalInterruptClassRoute to allocate a class route.
- Use \ref Kernel_SetCollectiveClassRoute and
\ref Kernel_SetGlobalInterruptClassRoute to set up the class route.

*/


//-----------------------------------------------------------

/** \page muGlobalInterrupts Global Interrupts

For details on using MU global interrupts, refer to \ref GI.h.

*/


//-----------------------------------------------------------

/** \page muBarriers Global Interrupt Barriers

For details on using MU global interrupt barriers, refer to \ref GIBarrier.h.

*/

//-----------------------------------------------------------

/** \page muSharing Sharing the MU

This page describes how to share the Message Unit (MU) with the communications
component (MPI, PAMI, etc).  

An MU SPI application may be written to also use MPI or PAMI.  In the following
discussion, "MPI" refers to either, and "MPI_Init" refers to either MPI or PAMI
initialization.

When an MU SPI application also uses MPI, the MU resources must be divided
between the them.  The MU SPI application uses its own MU resources while MPI
uses its own.

When the MU SPI application allocates its MU resources BEFORE calling MPI_Init,
MPI detects which resources are already taken by the MU SPI application and tries
to run with the resources that are left over.

When the MU SPI application calls MPI_Init BEFORE it allocates its MU resources,
MPI may not leave enough MU resources for the MU SPI application.  To ensure that
there are enough MU resources left for the MU SPI application, environment
variables should be specified to reserve some of the MU resources for the MU SPI
application.  Specifically, \ref MUSPI_NUMINJFIFOS, \ref MUSPI_NUMRECFIFOS,
\ref MUSPI_NUMBATIDS, and \ref MUSPI_NUMCLASSROUTES.  The number of resources
specified on these environment variables is the total number of those resources
reserved for the MU SPI application for the process.  In general, MPI assumes
that these resources are allocated evenly across the cores in the process.  For
example, when MUSPI_NUMINJFIFOS=10 with 8 processes per node (2 cores per
process), MPI assumes that the MU SPI application will allocate 5 injection FIFOs
in subgroups associated with the first core, and 5 injection FIFOs in subgroups
associated with the second core.  MPI assumes that the MU SPI application will
only use 5 injection FIFOs out of the 4 subgroups associated with any core in
the process.  Even if your MU SPI application is not coded to spread its FIFO
allocation in this way, it is important to specify the values on these
environment variables as if it was coded that way so MPI reserves the proper
amount of resources per core for the MU SPI application.  For example, if the
MU SPI application is coded to use at most 10 injection FIFOs in the subgroups
associated with any core of the process, then it should set MUSPI_NUMINJFIFOS
to 10 * number of cores in the process (in the 2 core per process example, this
would be 20).

The following describes how MPI allocates MU resources.  Knowing this may help
in coding the MU SPI application such that there are enough MU resources
available for both the MU SPI application and MPI.

Before main() receives control, 

- CNK allocates MU resources in subgroups 66 and 67.  MU SPI applications
should avoid allocating MU resources in these subgroups.
- CNK allocates collective class routes 0, 14, and 15.  Collective class route
0 is the equivalent of MPI_COMM_WORLD and can be used by the MU SPI application
for collectives involving the all of the nodes in the job.  Collective class
routes 14 and 15 are only for CNK's use.
- CNK allocates global interrupt class routes 0, 14, and 15.  Global interrupt
class route 0 is the equivalent of MPI_COMM_WORLD and can be used by the MU SPI
application for global interrupts involving the all of the nodes in the job.
Global interrupt class routes 14 and 15 are only for CNK's use.
- MPI allocates some MU resources in subgroups 64 and 65.  These are global
resources used by MPI for all processes on the node.  Specifically, MPI
allocates 12 injection FIFOs, 1 reception FIFO, and 8 base address table
entries somewhere in subgroups 64 and 65.
- MPI allocates the remaining collective and global interrupt class routes
2 through 13.  Environment variable \ref MUSPI_NUMCLASSROUTES can be used to
reserve some of these class routes for the MU SPI application.
.

During MPI_Init, MPI allocates MU resources for each communications context.
Contexts are similar to channels in a communications system.  Each context
runs independently of the others.  Communications threads each drive one or more
contexts.  Thus, each context has its own set of MU resources.  The number of
contexts in a process depends on the MPICH library being used, whether
MPI_Init_thread is called with MPI_THREAD_MULTIPLE, and the number of processes
per node.  When the MU SPI application is linked with one of the legacy MPICH
libraries (gcc.legacy, xl.legacy, xl.legacy.ndebug) or when MPI_Init_thread
is called without MPI_THREAD_MULTIPLE, there is 1 context per process.
When the MU SPI application is linked with one of the non-legacy MPICH libraries
(gcc, xl, xl.ndebug) and MPI_Init_thread is called with MPI_THREAD_MULTIPLE,
the number of contexts per process depends on the number of processes per node
as follows: 1 PPN - 32 contexts, 2 PPN - 16 contexts, 4 PPN - 8 contexts,
8 PPN - 4 contexts, 16 PPN - 2 contexts, 32 or 64 PPN - 1 context.

MPI_Init first divides the MU resources on a node by the number of processes on
the node.  That determines how many subgroups are available to each process,
as follows:

\verbatim
NumProcesses  NumSubgroups  NumInjFifos  NumRecFifos  NumBATids
PerNode       Available     Available    Available    Available
              PerProcess    PerProcess   PerProcess   PerProcess
      1           64           512          256          512
      2           32           256          128          256
      4           16           128           64          128
      8            8            64           32           64
     16            4            32           16           32
     32            2            16            8           16
     64            1             8            4            8
\endverbatim

MPI_Init spreads its MU resource allocations across the subgroups associated with
a process.  Each context is associated with a core within the process.  The
resources for that context will come from the subgroups associated with that core
(each core has 4 subgroups).  So, the MU SPI application should look at all of the
subgroups associated with the cores in the process to allocate its MU resources.
It should spread those allocations across those subgroups.

Here is an example with 4 processes per node:

- process 0 will be on cores 0,1,2,3.  
- process 1 will be on cores 4,5,6,7.
- process 2 will be on cores 8,9,10,11.
- process 3 will be on cores 12,13,14,15.  
- The subgroups associated with 
  - process 0 are 0 through 15.
  - process 1 has subgroups 16-31.
  - process 2 has subgroups 32-47.
  - process 3 has subgroups 48-63.  
  .
- MPI_Init allocates MU resources for contexts starting from the subgroups associated
with the last core in each process.  The first context gets its resources from
the last core in the process.  The second context gets its resources from the
second to last core in the process.  This continues through the first core in
the process, and then if there are still more contexts, it begins again with
the last core of the process, then the second to last core of the process, etc,
until the final context has been set up.  Thus, there can be multiple contexts
getting their MU resources from the same subgroups associated with the same core.
  - process 0, context 0 resources will come from subgroups 12-15.
  - process 1, context 0 resources will come from subgroups 28-31.
  - process 2, context 0 resources will come from subgroups 44-47.
  - process 3, context 0 resources will come from subgroups 60-63.
  - process 0, context 1 resources will come from subgroups 8-11.
  - process 1, context 1 resources will come from subgroups 24-27.
  - process 2, context 1 resources will come from subgroups 40-43.
  - process 3, context 1 resources will come from subgroups 56-59.
  - etc.
  .
.

Minimally, each context must have at least 1 injection fifo and 1 reception fifo.
Optimially, each context will have 10 injection fifos and 1 reception fifo.

MPI allocates all of the base address table entries that are available to each
context within the subgroups listed above.  Minimally, MPI requires 15 base
address table entries in the first context of the first process on each node.
Use environment variable \ref MUSPI_NUMBATIDS to reserve base address table
entries for the MU SPI application.  Because of the 15 requirement, the largest
value you can specify on MUSPI_NUMBATIDS is 17 * NumberOfCoresInTheProcess, which
is 17 * (16/NumberOfProcessesPerNode).  The MU SPI application should be coded to
look for free base address table entries in all of the subgroups associated with
the process.

*/

/**
 * \addtogroup env_vars_ Environment Variables
 *
 * Environment variable documentation uses the custom doxygen tags "\envs",
 * "\env", and "\default". These tags are defined as the following ALIASES in
 * the Doxyfile:
 *
 * \code
 *   ALIASES  = default="\n \par Default:\n"
 *   ALIASES += envs{3}="\addtogroup env_vars_\2 \3 \n \ingroup env_vars_\1 "
 *   ALIASES += env{2}="\addtogroup env_vars_\1 \n \anchor \2 \section \2 "
 * \endcode
 * 
 * \envs{ ,muspi,MU SPI}
 *
 * The MU SPI functions do not internally query any environment variables,
 * however, as the MU SPIs operate on a node scope it is recommended that
 * the following environment variables be used by any software library that
 * uses the MU SPIs. Failure to coordinate the MU SPI configuration between
 * independently developed software libraries can lead to unexpected results.
 *
 * \env{muspi,MUSPI_NUMBATIDS}
 * Number of base address table IDs per process reserved for use by an MU SPI
 * application.
 * \default 0
 * 
 * \env{muspi,MUSPI_NUMCLASSROUTES}
 * Number of collective class routes reserved for use by an MU SPI application.
 * This is also the number of global interrupt class routes reserved for use by
 * an MU SPI application.
 * \default 0
 * 
 * \env{muspi,MUSPI_NUMINJFIFOS}
 * Number of injection fifos per process reserved for use by an MU SPI
 * application.
 * \default 0
 *
 * \env{muspi,MUSPI_NUMRECFIFOS}
 * Number of reception fifos per process reserved for use by an MU SPI
 * application.
 * \default 0
 * 
 * \env{muspi,MUSPI_INJFIFOSIZE}
 * The size, in bytes, of each injection FIFO.  These
 * FIFOs store 64-byte descriptors, each describing a memory buffer to be
 * sent on the torus.  Making this larger can reduce overhead when there are
 * many outstanding messages.  Making this smaller can increase that overhead.
 * \default 65536
 *
 * \env{muspi,MUSPI_RECFIFOSIZE}
 * The size, in bytes, of each reception FIFO.  Incoming
 * torus packets are stored in this fifo until software can process
 * them.  Making this larger can reduce torus network congestion.  Making this
 * smaller leaves more memory available to the application.
 * \default 1048576
 */

/**
 * \brief __INLINE__ definition
 * 
 * Option 1:
 * Make all functions be "static inline":
 * - They are inlined if the compiler can do it
 * - If the compiler does not inline it, a single copy of the function is
 *   placed in the translation unit (eg. xxx.c)for use within that unit.
 *   The function is not externalized for use by another unit...we want this
 *   so we don't end up with multiple units exporting the same function,
 *   which would result in linker errors.
 *
 * Option 2:
 * A GNU C model: Use "extern inline" in a common header (this one) and provide
 * a definition in a .c file somewhere, perhaps using macros to ensure that the
 * same code is used in each case. For instance, in the header file:
 *
 * \verbatim
   #ifndef INLINE
   # define INLINE extern inline
   #endif
   INLINE int max(int a, int b) {
     return a > b ? a : b;
   }
   \endverbatim
 *
 * ...and in exactly one source file (in runtime/SPI), that is included in a
 * library...
 *
 * \verbatim
   #define INLINE
   #include "header.h"
   \endverbatim
 * 
 * This allows inlining, where possible, but when not possible, only one 
 * instance of the function is in storage (in the library).
 */
#ifndef __INLINE__
#define __INLINE__ extern inline
#endif


/**
 * \brief DCBT instruction
 */
#define muspi_dcbt(vaddr,inc) { do { asm volatile ("dcbt %0,%1" :: "b" (vaddr), "r" (inc) : "memory"); } while(0); }


/**
 * \brief DCBT L2 instruction
 */
#define muspi_dcbt_l2(vaddr,inc) { do { asm volatile ("dcbt 0x2,%0,%1" :: "b" (vaddr), "r" (inc) : "memory"); } while(0); }


/**
 * \brief DCBZ instruction
 */
#define muspi_dcbz(vaddr,inc) { do { asm volatile ("dcbz %0,%1" :: "b" (vaddr), "r" (inc) : "memory"); } while(0); }


/**
 * \brief Vector Load QPX Inline
 */
#define VECTOR_LOAD_NU(si,sb,fp) \
  do { \
    asm volatile("qvlfdx %0,%1,%2" : "=f" (fp) : "b" (si), "r" (sb)); \
  } while(0)


/**
 * \brief Vector Store QPX Inline
 */
#define VECTOR_STORE_NU(si,sb,fp) \
  do { \
    asm volatile("qvstfdx %0,%1,%2" :: "f" (fp), "b" (si), "r" (sb) :"memory"); \
  } while(0)



__END_DECLS


#endif /* _MUSPI_UTIL_H_  */
