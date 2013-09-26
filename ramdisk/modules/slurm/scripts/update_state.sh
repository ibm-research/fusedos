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
#
. /etc/bg_profile

numbers=$1
state=$2


usage() {

    cat <<EOF
USAGE: $0 <nodes> <state>
E.g. : $0 4-8 down
state: down|resume|idle|maint
EOF
    exit 1
}

case "$numbers" in
    [0-9-]*)
        true
        ;;
    *) usage
        ;;
esac

STATE=$(echo $state| tr a-z A-Z)
case "$STATE" in
    DOWN|RESUME|IDLE|MAINT)
        true
        ;;
    *) usage
        ;;
esac

scontrol update NodeName=$ASF_BASENAME[$numbers] State=$STATE
if [ $? -eq 0 ]; then
    MACHINEFILE=/etc/machinefile
    echo -n Updating $MACHINEFILE ...
    echo -n -e `/opt/slurm/bin/listnodes.pl -t idle -d "\n"` > $MACHINEFILE
    echo done.
    echo
    sinfo
fi

