// ================================================================
//
// Licensed Materials - Property of IBM
//
// Blue Gene/Q
//
// (C) Copyright IBM Corp.  2010, 2012
//
// US Government Users Restricted Rights -
// Use, duplication or disclosure restricted
// by GSA ADP Schedule Contract with IBM Corp.
//
// This software is available to you under either the GNU General
// Public License (GPL) version 2 or the Eclipse Public License
// (EPL) at your discretion.
//
// ================================================================
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
    {
        .name = "0_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "0_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "0_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "0_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "0_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "1_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "1_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "1_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "1_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "1_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "2_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "2_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "2_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "2_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "2_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "3_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "3_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "3_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "3_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "3_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "4_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "4_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "4_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "4_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "4_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "5_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "5_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "5_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "5_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "5_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "6_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "6_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "6_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "6_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "6_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "7_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "7_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "7_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "7_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "7_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "8_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "8_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "8_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "8_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "8_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "9_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "9_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "9_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "9_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "9_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "10_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "10_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "10_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "10_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "10_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "11_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "11_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "11_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "11_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "11_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "12_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "12_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "12_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "12_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "12_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "13_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "13_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "13_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "13_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "13_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "14_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "14_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "14_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "14_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "14_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "15_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "15_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "15_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "15_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "15_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "16_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "16_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "16_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "16_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "16_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "17_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "17_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "17_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "17_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "17_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "18_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "18_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "18_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "18_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "18_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "19_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "19_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "19_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "19_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "19_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "20_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "20_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "20_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "20_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "20_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "21_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "21_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "21_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "21_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "21_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "22_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "22_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "22_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "22_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "22_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "23_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "23_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "23_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "23_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "23_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "24_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "24_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "24_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "24_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "24_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "25_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "25_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "25_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "25_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "25_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "26_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "26_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "26_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "26_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "26_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "27_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "27_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "27_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "27_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "27_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "28_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "28_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "28_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "28_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "28_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "29_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "29_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "29_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "29_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "29_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "30_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "30_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "30_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "30_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "30_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "31_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "31_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "31_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "31_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "31_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "32_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "32_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "32_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "32_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "32_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "33_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "33_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "33_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "33_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "33_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "34_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "34_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "34_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "34_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "34_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "35_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "35_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "35_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "35_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "35_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "36_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "36_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "36_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "36_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "36_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "37_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "37_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "37_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "37_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "37_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "38_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "38_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "38_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "38_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "38_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "39_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "39_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "39_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "39_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "39_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "40_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "40_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "40_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "40_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "40_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "41_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "41_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "41_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "41_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "41_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "42_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "42_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "42_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "42_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "42_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "43_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "43_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "43_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "43_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "43_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "44_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "44_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "44_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "44_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "44_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "45_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "45_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "45_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "45_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "45_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "46_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "46_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "46_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "46_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "46_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "47_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "47_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "47_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "47_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "47_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "48_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "48_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "48_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "48_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "48_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "49_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "49_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "49_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "49_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "49_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "50_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "50_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "50_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "50_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "50_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "51_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "51_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "51_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "51_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "51_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "52_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "52_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "52_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "52_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "52_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "53_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "53_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "53_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "53_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "53_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "54_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "54_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "54_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "54_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "54_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "55_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "55_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "55_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "55_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "55_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "56_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "56_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "56_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "56_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "56_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "57_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "57_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "57_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "57_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "57_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "58_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "58_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "58_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "58_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "58_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "59_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "59_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "59_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "59_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "59_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "60_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "60_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "60_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "60_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "60_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "61_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "61_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "61_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "61_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "61_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "62_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "62_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "62_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "62_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "62_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "63_memory",
        .ops = &fusedosfs_memory_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "63_text",
        .ops = &fusedosfs_text_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "63_data",
        .ops = &fusedosfs_data_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "63_heap",
        .ops = &fusedosfs_heap_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    {
        .name = "63_shared",
        .ops = &fusedosfs_shared_file_ops,
        .mode = S_IWUSR|S_IRUGO
    },
    { "", NULL, 0 }
};
