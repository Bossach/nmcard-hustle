OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
MEMORY
{
 EMI (rwxai): ORIGIN = 0x00300800, LENGTH = 0x0007F800
}
SECTIONS
{
 PROVIDE (__executable_start = ORIGIN(EMI));
 . = ORIGIN(EMI);
 /DISCARD/ :
 {
  *(.text.vectors_reset)
 }
 .init :
 {
  *(.init.stub_start)
  KEEP (*(SORT_NONE(.init)))
  *(.init.stub_end)
 }
 .plt : { *(.plt) *(.iplt) }
 .text :
 {
  *(.text .text.* .gnu.linkonce.t.*)
  *(.stub .stub.*)
  *(.gnu.warning)
  *(.glue_7t)
  *(.glue_7)
  *(.vfp11_veneer)
  *(.v4_bx)
 }
 .fini :
 {
  *(.fini.stub_start)
  KEEP (*(SORT_NONE(.fini)))
  *(.fini.stub_end)
 }
 PROVIDE (__etext = .);
 PROVIDE (_etext = .);
 PROVIDE (etext = .);
 .rodata :
 {
  *(.rodata .rodata.* .gnu.linkonce.r.*)
 }
 .ARM.extab :
 {
  *(.ARM.extab* .gnu.linkonce.armextab.*)
 }
 .ARM.exidx :
 {
  PROVIDE_HIDDEN (__exidx_start = ABSOLUTE(.));
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  PROVIDE_HIDDEN (__exidx_end = ABSOLUTE(.));
 }
 .eh_frame_hdr :
 {
  *(.eh_frame_hdr)
  *(.eh_frame_entry .eh_frame_entry.*)
 }
 .eh_frame : ONLY_IF_RO
 {
  KEEP (*(.eh_frame))
  *(.eh_frame.*)
 }
 .gcc_except_table : ONLY_IF_RO
 {
  *(.gcc_except_table .gcc_except_table.*)
 }
 .gnu_extab : ONLY_IF_RO
 {
  *(.gnu_extab*)
 }
 .exception_ranges : ONLY_IF_RO
 {
  *(.exception_ranges*)
 }
 . = .;
 .eh_frame : ONLY_IF_RW
 {
  KEEP (*(.eh_frame))
  *(.eh_frame.*)
 }
 .gnu_extab : ONLY_IF_RW
 {
  *(.gnu_extab)
 }
 .gcc_except_table : ONLY_IF_RW
 {
  *(.gcc_except_table .gcc_except_table.*)
 }
 .exception_ranges : ONLY_IF_RW
 {
  *(.exception_ranges*)
 }
 __fix_0__ = .;
 .preinit_array :
 {
  PROVIDE_HIDDEN (__preinit_array_start = ABSOLUTE(.));
  KEEP (*(.preinit_array))
  PROVIDE_HIDDEN (__preinit_array_end = ABSOLUTE(.));
 }
 __fix_1__ = .;
 .init_array :
 {
  PROVIDE_HIDDEN (__init_array_start = ABSOLUTE(.));
  KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
  KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
  PROVIDE_HIDDEN (__init_array_end = ABSOLUTE(.));
 }
 __fix_2__ = .;
 .fini_array :
 {
  PROVIDE_HIDDEN (__fini_array_start = ABSOLUTE(.));
  KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
  KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
  PROVIDE_HIDDEN (__fini_array_end = ABSOLUTE(.));
 }
 __fix_3__ = .;
 .ctors :
 {
  KEEP (*crtbegin.o(.ctors))
  KEEP (*crtbegin?.o(.ctors))
  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
  KEEP (*(SORT(.ctors.*)))
  KEEP (*(.ctors))
 }
 .dtors :
 {
  KEEP (*crtbegin.o(.dtors))
  KEEP (*crtbegin?.o(.dtors))
  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
  KEEP (*(SORT(.dtors.*)))
  KEEP (*(.dtors))
 }
 .got : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }
 .data :
 {
  *(.data .data.* .gnu.linkonce.d.*)
  SORT(CONSTRUCTORS)
 }
 _edata = .;
 PROVIDE (edata = .);
 . = .;
 .bss (NOLOAD) : ALIGN(4)
 {
  __bss_start__ = ABSOLUTE(.);
  *(.bss .bss.* .gnu.linkonce.b.*)
  *(COMMON)
  . = ALIGN((. != 0) ? 4 : 1) + 256;
  __bss_end__ = ABSOLUTE(.);
 }
 __end__ = .;
 _end = .;
 PROVIDE (end = .);
 . = .;
 .stack (NOLOAD) : ALIGN(8)
 {
  __stack_end = ABSOLUTE(.);
  KEEP (*(.stack.default))
  . = ((. != 0x10000) ? 0x10000 : .);
  __stack_start = ABSOLUTE(.);
 }
 .rpc_services : ALIGN(8)
 {
  *(.rpc_services)
 }
 .noinit.rpc_services.bss (NOLOAD) : ALIGN(8)
 {
  *(.rpc_services.bss)
 }
 .heap (NOLOAD) : ALIGN(8)
 {
  PROVIDE_HIDDEN (__heap_start = ABSOLUTE(.));
  . = . + 0x10000;
  PROVIDE_HIDDEN (__heap_end = ABSOLUTE(.));
 } > EMI
 . = ALIGN(8);
 PROVIDE (__executable_limit = .);
 __HOST_EMI_BEGIN__ = 0x18000000;
 __HOST_EMI_LIMIT__ = 0x20000000;
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .comment 0 : { *(.comment) }
 .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end) }
 .debug_frame 0 : { *(.debug_frame) }
 .debug_str 0 : { *(.debug_str) }
 .debug_loc 0 : { *(.debug_loc) }
 .debug_macinfo 0 : { *(.debug_macinfo) }
 .debug_weaknames 0 : { *(.debug_weaknames) }
 .debug_funcnames 0 : { *(.debug_funcnames) }
 .debug_typenames 0 : { *(.debug_typenames) }
 .debug_varnames 0 : { *(.debug_varnames) }
 .debug_pubtypes 0 : { *(.debug_pubtypes) }
 .debug_ranges 0 : { *(.debug_ranges) }
 .debug_macro 0 : { *(.debug_macro) }
 .debug_addr 0 : { *(.debug_addr) }
 .ARM.attributes 0 :
 {
  KEEP (*(.ARM.attributes))
  KEEP (*(.gnu.attributes))
 }
 .note.gnu.arm.ident 0:
 {
  KEEP (*(.note.gnu.arm.ident))
 }
 /DISCARD/ :
 {
  *(.interp)
  *(.note.gnu.build-id)
  *(.hash)
  *(.gnu.hash)
  *(.dynsym)
  *(.dynstr)
  *(.gnu.version)
  *(.gnu.version_d)
  *(.gnu.version_r)
  *(.rel.*)
  *(.rela.*)
  *(.rodata1)
  *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
  *(.data1)
  *(.sdata .sdata.* .gnu.linkonce.s.*)
  *(.tdata .tdata.* .gnu.linkonce.td.*)
  *(.tbss .tbss.* .gnu.linkonce.tb.*)
  *(.tcommon)
  *(.jcr)
  *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*)
  *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*)
  *(.dynamic)
  *(.dynbss)
  *(.dynsbss)
  *(.sbss .sbss.* .gnu.linkonce.sb.*)
  *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
  *(.scommon)
  *(.dynlbss)
  *(.lbss .lbss.* .gnu.linkonce.lb.*)
  *(.lrodata .lrodata.* .gnu.linkonce.lr.*)
  *(.ldata .ldata.* .gnu.linkonce.l.*)
  *(.note.GNU-stack)
  *(.gnu_debuglink)
  *(.gnu.lto_*)
 }
}
