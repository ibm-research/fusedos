Preparing a Build Environment
=============================

Preparing a build environment consists of two main steps: (1) install
Fedora 19 PPC64 and (2) build a partial Blue Gene/Q distribution.

The usual base for the build environment is Red Hat Enterprise Linux
6.4 PPC64. To avoid licensing issues, we recommend using Fedora
19 PPC64 which can be downloaded from one of the mirrors listed
[here](http://mirrors.fedoraproject.org/publiclist/Fedora/19/ppc64
"Fedora 19 mirrors"). The Blue Gene/Q distribution has numerous
issues with the Fedora 19 texinfo (version 5.1). To avoid
these issues, the Fedora 18 texinfo (version 4.13) should be
installed. The RPM (texinfo-4.13a-18.fc18.ppc64.rpm) can be found
[here](http://mirrors.kernel.org/fedora-secondary/releases/18/Fedora/ppc64/os/Packages/t
"Fedora 18 mirror") and installed with the following commands:

  1. yum erase texinfo.ppc64
  1. yum install texinfo-4.13a-18.fc18.ppc64.rpm

The source code for the Blue Gene/Q distribution can be downloaded
from the Argonne National Laboratory externel repository
[here](https://repo.anl-external.org/repos/bgq-driver/V1R2M1 "ANL
external repository"). The following commands can be used to download
the necessary tarballs:

  1. mkdir -p /bgsys/downloads
  1. cd /bgsys/downloads
  1. wget https://repo.anl-external.org/repos/bgq-driver/V1R2M1/bgq-V1R2M1-toolchain.tar
  1. wget https://repo.anl-external.org/repos/bgq-driver/V1R2M1/bgq-V1R2M1.tar.gz
  1. wget https://repo.anl-external.org/repos/bgq-driver/V1R2M1/bgq-V1R2M1-gpl.tar.gz

Building the Blue Gene/Q distribution consists of two steps. First, the
toolchain is built. After the toolchain is built the remainder of the
distribution is built including the firmware, hardware interface support,
and system programming interface support. To build the toolchain, use
the following steps:

  1. cd /bgsys/downloads
  1. tar xfv bgq-V1R2M1-toolchain.tar
     * This results in two RPM files in /bgsys/downloads.
  1. rpm2cpio bgq-V1R2M1-toolchain-1.2.1-0.ppc64.rpm | ( cd / ; cpio -idmv )
     * This creates /bgsys/drivers/V1R2M1/ppc64.
  1. rm /bgsys/drivers/V1R2M1/ppc64/gnu-linux
     * The build process (step 8) needs to create a gnu-linux directory
  1. cd /bgsys/drivers/V1R1M1/ppc64
  1. patch -p0 -E < \[FusedOS repository location/\]buildenv/toolchain-fc19.patch
  1. cd /bgsys/downloads
  1. /bgsys/drivers/V1R2M1/ppc64/toolchain/retrieveToolChains.sh
  1. /bgsys/drivers/V1R2M1/ppc64/toolchain/buildToolChain.sh
     * This takes a while...
  1. ln -s /bgsys/drivers/V1R2M1/ppc64 /bgsys/drivers/ppcfloor

To build the remainder of the distribution, use the steps below:

  1. cd /bgsys/downloads
  1. tar xfv bgq-V1R2M1.tar.gz
     * This creates /bgsys/downloads/bgq-V1R2M1.
  1. tar xfv bgq-V1R2M1-gpl.tar.gz
  1. patch -p0 -E < \[FusedOS repository location/\]buildenv/bgq-V1R2M1-fc19.patch
  1. cd bgq-V1R1M1
  1. ./scripts/configure --with-floor=/bgsys/drivers/ppcfloor --with-revision=1
  1. make
     * This takes a while...
  1. cd work
  1. rm gnu-linux toolchain
  1. mv * /bgsys/drivers/ppcfloor

/bgsys/drivers/ppcfloor now contains a partial Blue Gene/Q Linux
distribution.
