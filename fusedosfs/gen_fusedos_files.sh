#!/bin/bash 
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
# This software is available to you under either the GNU General
# Public License (GPL) version 2 or the Eclipse Public License
# (EPL) at your discretion.
#
# ================================================================

# include copyright header
head -n18 $0 | tail -n17 | sed "s/#/\/\//"

cat <<EOF
struct tree_descr fusedos_files[] = {
    { NULL, NULL, 0 },
    { NULL, NULL, 0 },
    {
       .name = "monitor",
       .ops = &fusedosfs_monitor_file_ops,
       .mode = S_IWUSR|S_IRUGO
    },
    {
       .name = "context",
       .ops = &fusedosfs_context_file_ops,
       .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "pvr",
        .ops = &fusedosfs_pvr_file_ops,
        .mode = S_IRUGO
    },
EOF

for i in {0..63}
do
	echo "    {"
	printf "        .name = \"%d_memory\",\n" $i
	echo "        .ops = &fusedosfs_memory_file_ops,"
	echo "        .mode = S_IWUSR|S_IRUGO"
	echo "    },"

	echo "    {"
	printf "        .name = \"%d_text\",\n" $i
	echo "        .ops = &fusedosfs_text_file_ops,"
	echo "        .mode = S_IWUSR|S_IRUGO"
	echo "    },"

	echo "    {"
	printf "        .name = \"%d_data\",\n" $i
	echo "        .ops = &fusedosfs_data_file_ops,"
	echo "        .mode = S_IWUSR|S_IRUGO"
	echo "    },"

	echo "    {"
	printf "        .name = \"%d_heap\",\n" $i
	echo "        .ops = &fusedosfs_heap_file_ops,"
	echo "        .mode = S_IWUSR|S_IRUGO"
	echo "    },"

	echo "    {"
	printf "        .name = \"%d_shared\",\n" $i
	echo "        .ops = &fusedosfs_shared_file_ops,"
	echo "        .mode = S_IWUSR|S_IRUGO"
	echo "    },"
done

echo "    { \"\", NULL, 0 }"
echo "};"
