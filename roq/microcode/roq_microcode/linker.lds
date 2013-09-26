/*
 * Microcode for RoQ device driver and library
 *
 * Copyright (c) 2008-2012, IBM Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 */

OUTPUT_FORMAT("elf64-powerpc", "elf64-powerpc",
	      "elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)
ENTRY(_start_domain)
SEARCH_DIR("/auto/BGQ/releases/bgqdev/floor-x86_64-2008-05-06/crosstools/powerpc64-quattro-linux/lib");

SECTIONS
{
  /* . = 0x00000200; */
  . = 0xc0000400;
  .text :
     {
      PROVIDE( __KERNEL_TEXT_START = . );
      *(.text.start.domain)
      *(.text.domain)
      *(.ktext.abs0)
      *(.ktext.start)
     . = ALIGN(0x1000);  /* Interrupt Vectors: (lower 12bits of vector start must be zero) */
     *(.ktext)
     *(.text)
     }

  .rodata :
     {
     . = ALIGN(64);
     *(.rodata .rodata.* .gnu.linkonce.r.*)
     *(.rodata1)
     PROVIDE( __KERNEL_TEXT_END = . );
     }

  .data :
     {
     PROVIDE( __KERNEL_DATA_START = . );
     *(.data)
     }

  .opd ALIGN(64) :
     {
     *(.opd)
     }

  .toc :
     {
     PROVIDE( __FWTST_TOC_BASE = (. + 0x8000) );
     *(.toc)
     }
  PROVIDE( __KERNEL_DATA_END = . );

  .bss :
     {
     PROVIDE( __KERNEL_BSS_START = . );
     *(.bss)
     PROVIDE( __KERNEL_BSS_END = . );
     }

  PROVIDE( __KERNEL_END = . );
}

