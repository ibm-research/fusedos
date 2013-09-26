#!/bin/sh
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

BGQ_IP_PREFIX=${BGQ_IP_PREFIX:-12}

# Example:
#   COORDINATES=<1,0,0,0,0>
#   DIMENSIONS=<2,1,1,1,1>

#
# trim coordinate string
#
bg_trim_coords(){
	# <1,2,3,4,5> -> "1 2 3 4 5"
	COORDINATES=$1
	echo $COORDINATES | tr -d "\<" | tr -d "\>" | tr "," " "
}

#
# Generate IP address (BGQ_IP_PREFIX.[0x8|0x4]aaaa.bbbbcccc.ddddeeee)
#
bg_coords2ip(){
	COORDINATES=$1
	NODE_TYPE=$2
	set -- `bg_trim_coords $COORDINATES`
	IP0=${BGQ_IP_PREFIX}
	test $NODE_TYPE = "ION" && let "IP1 = (64 | $1)" || let "IP1 = (128 | $1)"
	let "IP2 = ($2 << 4 | $3)"
	let "IP3 = ($4 << 4 | $5)"
	echo $IP0.$IP1.$IP2.$IP3
}

#
# Generate rank address
#
bg_coords2rank(){
	COORDINATES=$1
	DIMENSIONS=$2

	set -- `bg_trim_coords $COORDINATES`
	A=$1 B=$2 C=$3 D=$4 E=$5
	set -- `bg_trim_coords $DIMENSIONS`
	DA=$1 DB=$2 DC=$3 DD=$4 DE=$5

	# in contrast to BGAS, we map coordinates to 
	# rank like CNK's default mapOrder ABCDE(T)
	# rank = E + dim_E ( D + dim_D ( C + dim_C ( B + dim_B * A ) ) )
	# equivalent to
	# rank = A * dim_B * dim_C * dim_D * dim_E
	#      + B * dim_C * dim_D * dim_E
	#      + C * dim_D * dim_E
	#      + D * dim_D * dim_E
	#      + E
	rank=$(expr $A \* $DB + $B )
	rank=$(expr $rank \* $DC + $C )
	rank=$(expr $rank \* $DD + $D )
	rank=$(expr $rank \* $DE + $E )
	echo $rank;
}

#
# Generate coordinates from rank
# (this is actually unused)
#bg_rank2coords(){
#	DIMENSIONS=$1
#	set -- `bg_trim_coords $DIMENSIONS`
#	DA=$1 DB=$2 DC=$3 DD=$4 DE=$5
#
#	tmp=$rank
#	e=$(expr $tmp % $DE )
#	tmp=$(expr $tmp \/ $DE )
#	d=$(expr $tmp % $DD )
#	tmp=$(expr $tmp \/ $DD )
#	c=$(expr $tmp % $DC )
#	tmp=$(expr $tmp \/ $DC )
#	b=$(expr $tmp % $DB )
#	tmp=$(expr $tmp \/ $DB )
#	a=$(expr $tmp % $DA )
#
#	echo "<$a,$b,$c,$d,$e>"
#}

bg_size(){
	DIMENSIONS=$1
	set -- `bg_trim_coords $DIMENSIONS`
	DA=$1 DB=$2 DC=$3 DD=$4 DE=$5
	echo $(expr $DA \* $DB \* $DC \* $DD \* $DE)
}

#
# Calculate hostname
#
bg_hostname(){
    BASENAME=$1
    COORDINATES=$2
    DIMENSIONS=$3
    RANK=`bg_coords2rank $COORDINATES $DIMENSIONS`
    echo ${BASENAME}$RANK
}

#
# bg_profile
#
bg_add_profile(){
    NAME=$1
    shift
    VALUE=$*
    echo ${NAME}=\"${VALUE}\" >> /etc/bg_profile
}
