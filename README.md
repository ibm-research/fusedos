FusedOS
=======

FusedOS is an operating system (OS) research project at the IBM T.J.
Watson Research Center. This repository contains the prototype of FusedOS
for the IBM Blue Gene/Q system.

We have released this prototype under an open-source license to enable
collaboration with parties outside IBM. Most parts of FusedOS are
available to you under the Eclipse Public License (EPL) while some
parts are also available to you under the GNU General Public License
(GPL) version 2. Please feel free to explore the source code and the
documentation provided here.


Overview
--------

The documentation in this repository is structured as follows:

* What is FusedOS?
* [Architecture and Components](docs/Architecture.md)
* Getting Started
    1. Fulfiling the Requirements
    1. Cloning the Source Repositories
    1. [Preparing a Build Environment](buildenv/README.md)
    1. Building FusedOS
* [Booting FusedOS on IBM Blue Gene/Q](docs/Booting.md)
* [Running Applications on FusedOS](docs/Running.md)
* Developer Information
* [Authors, Contributors and Acknowledgements](Authors.md)


What is FusedOS
---------------

In this section, we first discuss the idea behind FusedOS and then
describe what you can do with our prototype today.

General-purpose operating systems provide a rich set of functionality to
support a broad set of applications, whereas specialized systems such as
the IBM Blue Gene/Q are tailored to a specific category of applications.
Therefore, specialized systems provide only the features that are
absolutely required, in a way to provide the targeted applications the
best possible quality of service.

FusedOS is our approach to bridge the gap between these two extremes. We
combine a general-purpose OS with a specialized OS and run them side
by side using resource partitioning. In contrast to operating systems
running next to each other in a virtualized environment, the OS instances
in FusedOS are much closer and process interaction can cross the boundary
between them.

Our prototype runs on the IBM Blue Gene/Q supercomputer and combines
Linux with the IBM compute node kernel (CNK), the production OS on Blue
Gene/Q compute nodes. Booting FusedOS turns a compute node partition
into a hybrid cluster; They can run both Linux and CNK applications.
Furthermore, Linux and CNK applications can run side by side on each node
at the same time, and even interact with each other. You can even run
several different CNK applications on a FusedOS node at the same time,
whereas CNK only allows one application to start as multiple processes.

You can connect to each individual compute node via SSH and
run Linux processes as usual. For running CNK applications,
you start the tool cl from Linux. The popular [SLURM resource
manager](https://computing.llnl.gov/linux/slurm/slurm.html) is available
for launching jobs over several nodes. SLURM can be used to start CNK
MPI applications.

When FusedOS runs on both IO and compute nodes, the IO nodes can export
external network file systems to compute nodes. On compute nodes, both
CNK and Linux applications have the same view of local and remote file
systems. In case there are no external file systems available or IO nodes
cannot be rebooted, FusedOS can run out of node-local ramdisks. That mode
still provides an environment sufficient for simple benchmarks and tests.


Architecture and Components
----------------------------

[Please find the discussion of the FusedOS architecture
and a description of the components of the prototype in
docs/Architecture.md.](docs/Architecture.md)


Getting Started
---------------

In this section, we describe how to prepare a build environment and
how to build FusedOS. First, we list the requirements for a FusedOS
build environment. Second, we describe how to checkout the main FusedOS
repository and the source code of our modified Linux. Third, we provide
you a step-by-step guide for preparing a build environment for FusedOS,
namely a *Blue-Gene/Q Driver* with cross compilers. Finally, we discuss
the steps required for building FusedOS.


### Fulfilling the Requirements

We use Fedora 19 on PowerPC (64-bit) on our development systems. We
will assume that environment in the following guidelines. While we only
tested building FusedOS on actual hardware, a virtualized environment
(even QEMU on non-ppc architectures) should work as well.

Besides a basic Linux installation, you will need basic developement
tools such as gcc and make. We will provide instructions for installing
the required Blue Gene/Q-specific development tools in this guide.

For running FusedOS, you need a Blue Gene/Q firmware binary built with

        FW_KFLAGS    = -D__KERNEL__ -DFW_DUAL_DOMAIN=1

set in the Makefile. Most current BGQ drivers should work. In any case,
[preparing a build environment for FusedOS](buildenv/README.md) will
yield an appropriate firmware image.


### Cloning the Source Respositories

First, clone the main repository of FusedOS

        $ git clone https://github.com/ibm-research/fusedos.git

Then, checkout the source of our modified Linux version as a git
_submodule_ inside the main repository

        $ cd fusedos/
        $ git submodule init
        $ git submodule update

### Preparing a Build Environment

The FusedOS main repository contains instructions and a patch to build a
*Blue Gene/Q driver* in a Fedora 19 installation. [See buildenv/README.md
for a step-by-step guide](buildenv/README.md).


### Building FusedOS

Now that you have prepared your build environment, you can build
FusedOS. We recommend that you put your SSH public key(s) into
*ramdisk/modules/rootfs/src/root/.ssh/authorized_keys*. This will allow
you to login to the FusedOS instances you boot afterwards. Then, just
call make in the main FusedOS repository

   $ make

The build process will yield three binaries in *boot/*:

* a Linux kernel image
* a ramdisk image that will serve as the root file system of each node
* an RoQ image that will run on a separate core and provide networking
  for Linux

In addition, it will build the following tools:

* the SPC monitor *spcm* (in [spcm/](spcm/))
* the loader for the SPC monitor *spcml* (in [cl/src](cl/src))
* our CNK as a library OS *cl* (in [cl/src](cl/src))
* a simple CNK application *hello_world* (in [tests/fusedos](tests/fusedos))

If required you can customize the following properties of the build
process:

* output directory for the boot images (*FUSEDOS_BOOT_DIR* in the top-level [Makefile](Makefile))
* location of the BGQ driver that you built according to [the
instructions](buildenv/README.md) (*BGQ_FLOOR_DIR* in the top-level [Make.rules](Make.rules)).


Booting and Running FusedOS
---------------------------

Booting a FusedOS cluster on a Blue Gene/Q system involves three steps.
First, you determine the compute nodes and the attached IO nodes that
will comprise the FusedOS cluster. Second, you launch FusedOS on the
respective IO nodes and wait for the Linux boot process to complete.
Finally, you launch FusedOS on the chosen compute nodes.


### Selecting Nodes for FusedOS

We have successfully tested FusedOS on compute node partitions of 32,
64, 128, and 256 nodes. You need to run FusedOS both on a compute node
partition and on all the I/O nodes connected to that partition to use
all of its capabilities. All these I/O nodes have to be configured and
booted as one I/O block. Further, they should have no links to other
compute nodes than those used for FusedOS to avoid interference.

In addition to being configured as one block, they need to have the
separate I/O torus enabled, which is not used in regular operation. This
torus is a prerequisite for the [RoQ](roq/) network driver, which we use
to connect the Linux instances on I/O and compute nodes to each other.
The control system will enable the I/O torus when you add the additional
boot option *train_io_torus* to the I/O block. Up to eight I/O nodes
(those in one I/O drawer) can form a torus using electrical wiring
usually present in each I/O drawer. However, as far as we know, the I/O
torus is a purely experimental feature, so your experience may vary.

In our experience, we found the best configuration of compute node
partitions and connected I/O blocks fulfulls two requirements:

1. For all the I/O nodes, each I/O link connects to a compute node in
that partition.
1. For all compute nodes that have I/O links, that I/O link connects to
a node in that I/O block.

If you cannot dedicate I/O nodes to FusedOS, you can run it on compute
nodes only. However, you will not be able to SSH to the compute nodes
or access network file systems from outside Blue Gene/Q. Your FusedOS
instances will be constrained to use whatever you built into their
ramdisk. In that case, comment out the so-called *discovery step* in
[roq/linux/roq](roq/linux/roq). That mode of operation can be sufficient
to trigger simple experiments via the console, though we have used it
successfully to run CNK MPI applications.


### Launching FusedOS on Nodes

We have prepared the script [boot/boot_bgq.sh](boot/boot_bgq.sh) that
you can use to boot FusedOS on Blue Gene/Q. Call it as

        $ boot_bgq.sh <BG/Q block to boot> default

If you decide to place the FusedOS boot images at another location,
specify that directory in the environment variable *FUSEDOS_BOOT_DIR*.
In case you might experiment with several ramdisks of different
configurations, subsitute *default* with the specific configuration of
ramdisk that you want to boot (the ramdisk configurations are actually
text files in *[ramdisk/config](ramdisk/config)* and get copied to
*[boot/config](boot/config)*).


### Boot Example

We will illustrate the steps above using an example. Imagine you
have half a midplane available to run FusedOS, as a block labelled
R00-M1-N00-256. In Blue Gene/Q terminology that is a quarter of a rack
or 256 nodes. We assume that the specific Blue Gene/Q rack is configured
so that eight I/O nodes (a block labelled R00-IC) are connected to the
256 compute nodes in R00-M1-N00-256, not to any other compute nodes.

In the case described above, you can boot FusedOS on both I/O and compute
nodes without disturbing other users, as the I/O nodes do not have to
serve other compute nodes. Thus, you would first call

        $ boot_bgq.sh R00-IC-torus default

and wait until Linux on the I/O nodes has booted successfully (boot
message *"rcS done."*). Then, you can launch FusedOS on the compute
nodes by calling

        $ boot_bgq.sh R00-M1-N00-256 default

On the I/O nodes, Linux will configure the outside Ethernet interfaces
with the same IP addresses that a regular I/O node Linux would. It will
print these addresses as part of the final boot messages. At that point,
feel free to login to an I/O node using SSH.


### Running Simple CNK Applications

We have encapsulated CNK as a Linux application called *cl*. For running
a CNK application, you call *cl* and pass the application to launch as
a parameter. Other parameters to *cl* specify how many and which SPCs
should be allocated to the application. The default settings are enough
for a simple, *hello world-style* application.

Once the FusedOS nodes have booted up, you can use SSH to login. As
convenience features, the boot scripts generate an */etc/hosts* with
aliases irank<1..M> and crank<1..N> for the internal IPs of I/O and
compute nodes, and IO nodes display their external IPs during boot:

        Welcome to r00icj02 (R00-IC-J02 eth0:A.B.C.D NA:0.0.0.0)

Use these IPs and names to login to a compute node. As all the compute
nodes are basically equal, you may pick any of them:

        $ ssh root@<external ip of an IO node>
        Welcome to r00icj02 (R00-IC-J02 eth0:172.16.30.11 NA:0.0.0.0)
        root@r00icj02 [irank4 00100]:~ # ssh crank42
        Welcome to crank42 (R00-M1-N04-J16 NA:0.0.0.0 NA:0.0.0.0)
        root@crank42 [crank42 20110]:~ #

On the compute node, start the cl utility and pass the *hello_world*
CNK applications as its parameter. *cl* and the other FusedOS utilities
are located in /opt/fusedos/bin, which is in $PATH. Thus, just start

      root@crank42 [crank42 20110]:~ # cl /opt/fusedos/tests/hello_world
      Hello world, pid 1, fp sum 3.800000, getuid 0


### Running CNK MPI Applications

On regular Blue Gene/Q systems, users launch MPI jobs using the *runjob*
system. On FusedOS, launching CNK MPI jobs works differently, much closer
to how you launch MPI jobs in Linux.

CNK MPI applications require specific setup and cleanup work on each
node. We have packaged that functionality into the tool *sfjs*, which
works as a frontend for *cl*. Further, we use the [SLURM resource
scheduler](https://computing.llnl.gov/linux/slurm) to spread the job
over all participating nodes.

Launching a CNK MPI job in FusedOS involves creating an allocation in
SLURM and then calling srun to make SLURM run *sfjs* with appropriate
parameters on each node. The process is slightly different if you run an
MPI job over all the nodes in the compute node partition or only over a
subset. We describe both variants below.

#### Running CNK MPI jobs on a full compute node partition

        # salloc -N 256 # all the nodes in the partition
	salloc: Granted job allocation 3
        # srun sfjs -- -s -t 16 mpi_helloworld

Arguments after the double-dash are passed to *cl*. The parameter *-s*
and *-t 16* are required for CNK MPI applications (more precisely, for
the PAMI messaging library that they rely on). The parameters enable
shared memory support and allocate 16 threads and the associated memory
chunks for the CNK application. Both *cl* and *sjfs* print usage messages
when called with *-h* or without any parameters.

A more extensive example with 2 MPI ranks on each node and 24 SPCs per
MPI rank (i.e., process) would be as follows:

        # srun sfjs -p 2 -- -s -t 16 <some MPI app>

#### Running CNK MPI jobs on subblocks

The current support in FusedOS for running CNK MPI applications on
subblocks is fully functional but places a few restrictions on which
subsets of nodes can be used. Specifically, only nodes that form a
*rectangle* in the Blue Gene/Q torus can be used for a job. Selecting
appropriate blocks of nodes is made easy by the hostnames (crank<n\>) we
assign to compute nodes:
> A contiguous set of N compute nodes crankA - crank(A+N-1) can be used
> for a subblock MPI job in FusedOS if and only if A is a multiple of N.

A few examples:

* You can run jobs spanning 8 nodes on crank0-7, crank8-15, ...,
  crank64-71, crank72-79, and so on.
* However, you cannot run jobs on crank4-11, as 4 is not a multiple of 8.
* Jobs spanning 32 nodes work on blocks such as crank0-31, crank32-63,
  ..., crank128-159, ...
* While crank40-47 is fine for a job with 8 nodes, crank40-71 does not
  work for a job with 32 nodes, as 40 is not a multiple of 32.
* If your compute node partition spans 256 nodes, you can run 2 jobs
  over 128 nodes, at the same time: One on crank0-127, the other on
  crank128-255.

There is currently no explicit support for that allocation policy in
SLURM. However, SLURM's default behavior worked quite acceptably for our
experiments. *sfjs* will abort the job when the allocation does not
fulfill the requirements, so badly placed jobs do fail in a defined way
and leave the nodes and the torus in a usable state.

For technical reasons, you have to launch *sfjs* for two preparation
steps, before you can start the actual MPI job. These steps have to be
repeated before *every* MPI job:

        # salloc -N 16 --contiguous
	salloc: Granted job allocation 4
        # srun sfjs -s prep
        # srun sfjs -s gion
        # srun sfjs -s go -- -s -t 16 mpi_helloworld

Of course, you can place these commands in a shell script and use
SLURM's batch functionality:

        # sbatch -N 16 --contiguous some_batch_fusedos_mpi.sh

(More details about the preparation steps: Setting up the Blue Gene/Q
torus for an MPI job involves a barrier synchronization between all
involved nodes. CNK uses a barrier prepared by the firmware, which we
cannot use in FusedOS for subblock jobs. Instead, we use the SLURM job
steps, separated by the different srun calls, as an implicit barrier
synchronization.)

Developer Information
---------------------

There is no actual developer guide for FusedOS yet. We struggle with
that task ourselves!

If you want to get a grasp of the FusedOS source, reread the section on
the Architecture and Components of FusedOS and follow our pointers to
the source code. In addition, we provide a list of recommended reading
below. Please feel free to contact us if you have questions or
recommendations.

During Linux boot, the *[fusedos init
script](ramdisk/modules/rootfs/src/etc/init.d/fusedos)* loads
the *fusedosfs* and *mufs* kernel modules and boots the SPC
monitor on the SPCs, using binaries built into the ramdisk (see
[docsArchitecture.md](docs/Architecture.md) for an explanation of these
terms). When you are hacking FusedOS, you may want to disable that init
script and load these components manually from a remote file system
(or modify the init script to use your remote location).

Recommended reading and reference documentation:

* [FusedOS: Fusing LWK Performance
with FWK Functionality in a Heterogeneous
Environment](http://www.computer.org/csdl/proceedings/sbac-pad/2012/4907/00/4907a211-abs.html),
scientific publication by Yoonho Park, Eric Van Hensbergen, Marius
Hillenbrand, Todd Inglett, Bryan S. Rosenburg, Kyung Dong Ryu, and Robert
W. Wisniewski at SBAC-PAD 2012, New York, NY, USA. Note the different
notions for the roles of cores.
* [A2 Processor User's Manual for Blue
Gene/Q](https://bgq.anl-external.org/wiki/images/c/cf/A2.pdf), hosted
at Argonne National Laboratory
* [Power ISA version
2.07](https://www.power.org/documentation/power-isa-version-2-07/)
