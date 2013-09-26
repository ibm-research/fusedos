#!/bin/bash
#
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
# USAGE:
#       boot_bgq.sh <block> <configuration>
# Example:
#       boot_bgq.sh  Q02-I0 default
#

CURRDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TMP_DIR=/tmp

if [ -z "$FUSEDOS_BOOT_DIR" -o ! -d "$FUSEDOS_BOOT_DIR" ]; then
    echo "FUSEDOS_BOOT_DIR not set, using" $CURRDIR
    FUSEDOS_BOOT_DIR=$CURRDIR
fi

FIRMWARE_BIN=$FUSEDOS_BOOT_DIR/bgq_firmware.elf

error() {
    echo "###" >&2
    echo "### ERROR: $*" >&2
    echo "###" >&2
    exit 1
}

info() {
    echo "### INFO: $*" >&2
}

mmcs_commands() {
    BLOCK=$1; shift

    LINUX_MEM_START=0x0
    LINUX_MEM_END=0x3BFFFFFFF
    LINUX_CUSTADDR=0x3BFFF0000

    ROQ_MEM_START=0x3C0000000
    ROQ_MEM_END=-1
    ROQ_CUSTADDR=0x3FFFF0000

    if [ -n "$ROQ_BIN" ]; then
        DOMAIN="domain={images=$ROQ_BIN cores=16-16 id=ROQ memory=${ROQ_MEM_START}-${ROQ_MEM_END} custaddr=$ROQ_CUSTADDR} \
                domain={images=$LINUX_BIN,$RAMDISK_BIN cores=0-15  id=Linux memory=${LINUX_MEM_START}-${LINUX_MEM_END} custaddr=$LINUX_CUSTADDR}"
    else
        DOMAIN="domain={images=$LINUX_BIN,$RAMDISK_BIN id=Linux cores=0-16 memory=0--1 custaddr=$LINUX_CUSTADDR}"
    fi

    echo "
allocate_block ${BLOCK}
redirect_block on
boot_block \
uloader=$FIRMWARE_BIN \
$DOMAIN \
"
}

#### MAIN ############################################

if [ $# -lt 2 ]
then
    echo "Usage:   `basename $0` <block> <configuration>"
    echo "Example: `basename $0` R00-M0-N00-64 default"
    exit 1
fi

BLOCK=$1
TARGET=$2

# Linux
LINUX=linux_`for i in $(grep ^linux $FUSEDOS_BOOT_DIR/config/$TARGET); do  test -n "$(echo $i | grep  PATCHSET=)" && ( echo $i | cut -d= -f2 ) ; done`
LINUX_BIN=$FUSEDOS_BOOT_DIR/${LINUX}.bgq

# Ramdisk
RAMDISK_BIN=$FUSEDOS_BOOT_DIR/ramdisk_${TARGET}.gz

# roq kernel
ROQ_BIN=$( grep roq $FUSEDOS_BOOT_DIR/config/$TARGET | while read v; do
                 test "$${mod:0:1}" == '#' -o -z "$${mod:0:1}" && continue;
                 test -n "`echo $v | egrep '^roq '`" -a -z "`echo $v | grep USERSPACE_EXTENSION=1`" &&
		 ( test -e "$FUSEDOS_BOOT_DIR/roq_microcode.elf" && echo "$FUSEDOS_BOOT_DIR/roq_microcode.elf" || echo "$FUSEDOS_BOOT_DIR/roq_kernel.elf" ) && break;
             done )

info "firmware: $FIRMWARE_BIN"
info "linux:    $LINUX_BIN"
info "ramdisk:  $RAMDISK_BIN"
test -n "$ROQ_BIN" && info "roq:      $ROQ_BIN"

test -e $FIRMWARE_BIN || error "firmware '$FIRMWARE_BIN' not found"
test -e $LINUX_BIN    || error "linux    '$LINUX_BIN' not found"
test -e $RAMDISK_BIN  || error "ramdisk  '$RAMDISK_BIN' not found"
if [ -n "$ROQ_BIN" ]; then
    test -e $ROQ_BIN || error "roq      '$ROQ_BIN' not found"
fi

# boot block
INPUT_FILE=${TMP_DIR}/BOOT_${USER}_${BLOCK}
( mmcs_commands ${BLOCK} ) > $INPUT_FILE
$CURRDIR/bg_console.exp $INPUT_FILE $BLOCK

# free block
echo
read -p "free block $BLOCK ? [y/N]"
if [ "$REPLY" = "y" -o "$REPLY" = "Y" ]; then
    echo "Freeing $BLOCK. Please wait..."
    echo "free $BLOCK" | /bgsys/drivers/ppcfloor/bin/bg_console
fi

