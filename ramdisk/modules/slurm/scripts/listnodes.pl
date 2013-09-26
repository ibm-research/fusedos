#!/usr/bin/perl
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
# Description:
#   print list of nodes based on status in SLURM
#
use warnings;
use strict;
use 5.010;

use Getopt::Std;

usage() if /^-h/ ~~ @ARGV;

our($opt_d, $opt_t, $opt_c);
usage() if ! getopts('cd:t:');

my $delimiter = $opt_d || ' ';
my $state     = $opt_t;
my $count     = $opt_c;

sub usage {

    print <<"EOF";
    Usage: listnodes.pl [-t state] [-d delimiter] [-c]
        -t state     : list only nodes that are int the state
        -d delimiter : print delimiter after each node name
        -c           : print only number of nodes
EOF
    exit 1;
}

if ($count) {
    my $cmd = "sinfo -p debug -h --format='%D'";
    $cmd .= " -t $state" if $state;
    print qx($cmd);
    exit;
}

my $cmd = "sinfo -p debug -h --format='%N'";
$cmd .= " -t $state" if $state;
my $result = qx($cmd);
# irank[0-2,4,6-7]

exit 0  if ($result eq "");

die "invalid nodes" unless $result =~ /^(.rank)\[?([0-9,-]+)\]?/;

my $prefix  = $1;
my $numbers = $2;

my @nodes;

NODES:
for (split(/,/, $numbers)) {

    /^(\d+)$/ && do {
            # 13
            push @nodes, $1;
            next NODES;
        };
    /^(\d+)-(\d+)$/ && do {
            # 1-3
            push @nodes, $1..$2;
            next NODES;
        };

    die "ERROR: invalid range '$numbers'";
}

my $str;
for (@nodes) {
    $str .=  "${prefix}$_$delimiter";
}
$str =~ s/$delimiter$//;

print "$str\n";
