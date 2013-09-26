Architecture and Components
===========================

The FusedOS prototype on IBM Blue Gene/Q provides a hybrid Linux and
IBM Compute Node Kernel (CNK) cluster on Blue Gene/Q compute partitions.
FusedOS runs on both compute and IO nodes, so that the IO nodes can mount
network filesystems from outside a Blue Gene/Q system and re-export
them to the compute nodes. CNK MPI applications use the same network
stack as on a production Blue Gene/Q system, while Linux employs RoQ
(*[roq/](../roq)*) for networking between both compute and IO nodes.
You can use SSH to login to an IO node, login to any other compute (or IO)
node, and use SLURM to launch jobs (both Linux and CNK) on the cluster.

In this file, we give an overview of the architecture of our FusedOS
prototype. When discussing a specific component or functionality
of the prototype, we provide references to source code or scripts
(*[relative/to/the/repository](../)*). After this brief overview, we
first discuss the FusedOS components on each node. Second, we describe how
each node's resources are partitioned during the boot process. Third, we
describe FusedOS networking for both CNK and Linux applications. Finally,
we name the components that tie the individual nodes together and enable
their convenient use as a hybrid cluster.

We are aware that this document may not convey all of FusedOS'
design in sufficient depth and may fall short of providing a guide to
understanding the prototype. We welcome any feedback about the quality
of this description, so that we can improve it. Your can use the gitgub
mechanisms to post feedback or see [Authors.md](../Authors.md) for how
to get in contact with us.


FusedOS Nodes
-------------

On each node, we run two OS instances side by side: Linux and CNK. As
both operating systems are designed to manage all the resources of a
system, however, we have to modify both to enable their co-existence
on a single system without virtualization. We added hooks to Linux
partition resources. A subset of the cores are used by Linux, the others
are reserved for CNK. We call these core general-purpose cores (GPCs,
for Linux) and special-purpose cores (SPCs, the others), respectively.
Similarly, Linux only uses a share of each node's memory, the remainder
being reserved for CNK applications.

Instead of the full CNK, we only run a very small system-level
monitor (derived from CNK) on the SPCs (called the SPC monitor,
*[spcm/](../spcm)*). Each spcm receives commands from GPCs via
shared-memory and signals events and exceptions back to the GPCs via
inter-processor interrupts (IPI).

The main part of CNK is encapsulated as a (sort of) library OS in our
tool *CNK-as-a-Library* (CL, *[cl/src/cl.cc](../cl/src/cl.cc)*). For
launching CNK applications, a user starts our CL tool as a process in
Linux user-space. CL loads the CNK application into memory and then
remotely triggers and controls its execution on one or several SPCs.

A Linux kernel module (*[fusedosfs/](../fusedosfs/)*) manages the
remote control interfaces of the SPC monitors (one instance per HW
thread) and exports them by means of a pseudo-filesystem (mounted
under */fusedosfs*). Further, this module allows Linux processes
to map the memory reserved for the SPCs (mmap as implemented in
*[fusedosfs/inode.c](../fusedosfs/inode.c)*). CL utilizes that mechanism
to replicate a CNK applications' address space in its own address space
(as a Linux process).

Our prototype of FusedOS is not limited to CNK, though. As we only run
a small and generic system-level monitor on the SPCs, you can provide
your own special-purpose library OS as a Linux process that plugs into
the *Fusedosfs* interface to control the SPCs.

Each node in a Blue Gene/Q system has 17 cores with four hardware
threads, in addition to 16 GiB of RAM. In booting our prototype,
there are actually two steps where these resources get partitioned,
as we split them into three subsets: Besides the GPCs and
SPCs, one core is dedicated for the backend component of the
RoQ network driver (*[roq/microcode](../roq/microcode)*). In
the first step, the firmware reserves one core for RoQ and
allocates the other cores to Linux, controlled by the boot script
(*[boot/boot_bgq.sh](../boot/boot_bgq.sh)*). In
the second step, hooks in several stages of the Linux boot
code keep Linux from initializing a subset of the hardware
threads (in *arch/powerpc/boot/bgq.c* and *kernel/smp.c*)
and reserve memory (*arch/powerpc/platforms/bgq/setup.c* and
*arch/powerpc/platforms/bgq/fusedos.c*). Later, a user-space
utility loads the SPC monitor and boots it on the reserved cores,
thereby turning them into SPCs. The utility is called *spcml*
(*[cl/src/spcml.cc](../cl/src/spcml.cc)*) and uses an *ioctl* to
*FusedOSFS* (*[fusedosfs/inode.c](../fusedosfs/inode.c)*).


FusedOS Networking
------------------

Both the Linux and the CNK side of our prototype use the Blue Gene/Q
torus network. Parallel CNK applications employ an unmodified MPI stack
and reach performance close to a production CNK system. Linux utilizes
[RoQ](https://bgq.anl-external.org/wiki/index.php/RoQ) for TCP/IP
networking and RDMA over the torus.

CNK applications on Blue Gene/Q usually use a modified MPICH2 stacked
on the PAMI messaging library. We have designed and implemented FusedOS
in such a way that it supports that messaging stack unmodified.

The interface to the Blue Gene/Q torus on each node (what you would
call a NIC with Ethernet) has several hundred *FIFO queues* that are
used for giving transfer commands to the hardware and for addressing
a communication partner (very roughly similar to TCP ports). Despite
that abundance, their use needs to be coordinated. We have modified
RoQ source code and adjusted the FIFO numbers it uses for communication
to avoid overlaps with the FIFOs that the PAMI library uses by default (in
*[roq/microcode/roq_microcode/bgq_hw_us_abstraction.c](../roq/microcode/roq_microcode/bgq_hw_us_abstraction.c)*).
By changing RoQ instead of PAMI, we maintain compatibility with the
unmodified build environments of regular Blue Gene/Q systems. In fact,
we support unmodified CNK application binaries on FusedOS, which greatly
benefits the reproducibility of benchmarks.


FusedOS Clustering
------------------

We tie together individual nodes running FusedOS into a cluster. For that
purpose, we provide them with a unified view of network filesytems outside
Blue Gene/Q and employ the SLURM resource scheduler as the infrastructure
for launching jobs on several nodes.

The IO nodes of a Blue Gene/Q system typically access external storage
systems via InfiniBand or 10G Ethernet and provide CNK on the compute
nodes with access to these file systems. Similarly, FusedOS IO nodes
mount external file systems (via NFS) and re-export them to compute
nodes. The Linux instance on FusedOS compute nodes mounts the external
file systems from the compute nodes. When a CNK application performs an
IO system call, *cl* delegates them to Linux. Thereby, both Linux and
CNK applications have the same view of the file system: They both see
the virtual file system (VFS) of the Linux on their node, with mounted
external network file systems and node-local parts.

We employ a variant of the 9P protocol (from the research operating
system Plan 9) over TCP/IP to access an IO nodes' file system from
a compute node. On the IO node, we run the *[distributed I/O daemon
(diod)](http://code.google.com/p/diod/)* as the server for the 9P
protocol. On the client, we use the 9P client included in the mainline
Linux kernel. Both client and server communicate using TCP/IP sockets.
The IP packets are transferred by RoQ, using the RoQ Ethernet frontend
(in *[roq/linux/roq_eth.c](../roq/linux/roq_eth.c)*). See the init script
[ramdisk/modules/diod/diod](../ramdisk/modules/diod/diod) for details
on how we start the diod server on IO nodes and how we mount the file
systems on compute nodes.

The current version of the startup scripts forms one SLURM cluster out
of all the compute nodes and another SLURM cluster out of all the IO
nodes. Incorporating both types of nodes in a single SLURM configuration
would require only trivial changes.
