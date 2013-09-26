OUTPUT_FORMAT("elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)
ENTRY(_start_wrapper)

SECTIONS
{
  . = 0x00000000;
  .text             : { *(.text) }
  .opd ALIGN(64)    : { *(.opd)  }
  .toc ALIGN(64)    : { PROVIDE( __WRAPPER_TOC_BASE = . + 0x8000 ); *(.toc) }
  .bss ALIGN(64)    : { *(.bss)  }
  .rodata ALIGN(64) : { *(.rodata .rodata.* .gnu.linkonce.r.*) }

  . = 0x00000800;
  .data : { *(.data) }
}
