#!/bin/bash
# begin_generated_IBM_copyright_prolog
#
# This is an automatically generated copyright prolog.
# After initializing,  DO NOT MODIFY OR MOVE
# ================================================================
#
# Licensed Materials - Property of IBM
#
# Blue Gene/Q
#
# (C) Copyright IBM Corp.  2010, 2012
#
# US Government Users Restricted Rights -
# Use, duplication or disclosure restricted
# by GSA ADP Schedule Contract with IBM Corp.
#
# This software is available to you under the
# Eclipse Public License (EPL).
#
# ================================================================
#
# end_generated_IBM_copyright_prolog

# load and mount FusedOSfs
insmod fusedosfs.ko
mkdir -p /fusedosfs
mount -t fusedosfs none /fusedosfs/

# load and mount MUFS
insmod mufs.ko
mkdir -p /mu
mount -t mufs none /mu

# Load SPC monitor
./spcml

