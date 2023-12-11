OUTPUT_FORMAT("elf32-nmc")
OUTPUT_ARCH(nmc)
ENTRY(start)
MEMORY
{
 IMB (rwxai): ORIGIN = 0x00000200<<2, LENGTH = 0x00007E00<<2
 IM2 (!r!w!x): ORIGIN = 0x00008000<<2, LENGTH = 0x00004000<<2
 IM3 (!r!w!x): ORIGIN = 0x0000C000<<2, LENGTH = 0x00004000<<2
 IM4 (!r!w!x): ORIGIN = 0x00010000<<2, LENGTH = 0x00004000<<2
 IM5 (!r!w!x): ORIGIN = 0x00014000<<2, LENGTH = 0x00004000<<2
 IM6 (!r!w!x): ORIGIN = 0x00018000<<2, LENGTH = 0x00004000<<2
 IM7 (!r!w!x): ORIGIN = 0x0001C000<<2, LENGTH = 0x00004000<<2
 /*EMI (rwx): ORIGIN = 0x000D0000<<2, LENGTH = 0x00010000<<2*/
}
SECTIONS
{
 PROVIDE (__executable_start = ORIGIN(IMB));
 . = ORIGIN(IMB);
 .text.reset_stub 0x00000000<<2 : AT (__stack_limit - (0x200<<2))
 {
  KEEP (*(.text.reset_stub))
 }
 __reset_stub_begin = LOADADDR(.text.reset_stub);
 __reset_stub_limit = LOADADDR(.text.reset_stub) + SIZEOF(.text.reset_stub);
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
  KEEP (*(.bsst))
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
  KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*)))
  KEEP (*(.init_array))
  PROVIDE_HIDDEN (__init_array_end = ABSOLUTE(.));
 }
 __fix_2__ = .;
 .fini_array :
 {
  PROVIDE_HIDDEN (__fini_array_start = ABSOLUTE(.));
  KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
  KEEP (*(.fini_array))
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
 .bss (NOLOAD) :
 {
  __bss_start__ = ABSOLUTE(.);
  *(.bss .bss.* .gnu.linkonce.b.*)
  *(COMMON)
  __bss_end__ = ABSOLUTE(.);
 }
 __end__ = .;
 _end = .;
 PROVIDE (end = .);
 . = .;
 .stack (NOLOAD) : ALIGN(2<<2)
 {
  *(.stack)
  . = 0x2000 << 2;
  __stacksize = SIZEOF(.stack);
 } > IMB
 PROVIDE (__stack_limit = ADDR(.stack) + SIZEOF(.stack));
 .rpc_services : ALIGN(2<<2)
 {
  *(.rpc_services)
 }
 .noinit.rpc_services.bss (NOLOAD) : ALIGN(2<<2)
 {
  *(.rpc_services.bss)
 }
 .heap (NOLOAD) : ALIGN(2<<2)
 {
  *(.heap)
  *(.heap1)
  *(.heap2)
  *(.heap3)
  *(.heap4)
  *(.heap5)
  *(.heap6)
  *(.heap7)
  *(.heap8)
  *(.heap9)
  *(.heap10)
  *(.heap11)
  *(.heap12)
  *(.heap13)
  *(.heap14)
  *(.heap15)
  . = 0x2000 << 2;
 }
 . = ALIGN(2<<2);
 PROVIDE (__executable_limit = .);
 __HOST_EMI_BEGIN__ = 0x06000000<<2;
 __HOST_EMI_LIMIT__ = 0x08000000<<2;
 .stab 0 : { *(.stab) }
 .stabstr 0 : { *(.stabstr) }
 .stab.excl 0 : { *(.stab.excl) }
 .stab.exclstr 0 : { *(.stab.exclstr) }
 .stab.index 0 : { *(.stab.index) }
 .stab.indexstr 0 : { *(.stab.indexstr) }
 .comment 0 : { *(.comment) }
 .debug 0 : { *(.debug) }
 .line 0 : { *(.line) }
 .debug_srcinfo 0 : { *(.debug_srcinfo) }
 .debug_sfnames 0 : { *(.debug_sfnames) }
 .debug_aranges 0 : { *(.debug_aranges) }
 .debug_pubnames 0 : { *(.debug_pubnames) }
 .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) }
 .debug_abbrev 0 : { *(.debug_abbrev) }
 .debug_line 0 : { *(.debug_line .debug_line.* .debug_line_end ) }
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
 .gnu.attributes 0:
 {
  KEEP (*(.gnu.attributes))
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
