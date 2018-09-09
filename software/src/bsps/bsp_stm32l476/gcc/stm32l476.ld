/* Linker script for STM32F407 */


OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")


MEMORY
{
  flash     (rx)  : ORIGIN = 0x08000000, LENGTH = 0x00080000
  sram2     (rwx) : ORIGIN = 0x10000000, LENGTH = 0x00008000
  sram1     (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00018000
}

SECTIONS
{
  /* Code */
  .text :
  {
    *(.startup)
    *(.text)
    *(.text*)

	*(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
	
	. = ALIGN(4);
  } > flash


  /* Constants */
  .rodata ALIGN(4) :
  {
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _DATA_ROM_START_ = .;
  } > flash

 /* Constructors */
  .ctors ALIGN(4) :
  {
    _CTORS_START = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    _CTORS_END = .;
  } > flash

  /* ARM specific stuff */
  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > flash
  .ARM : {
    PROVIDE_HIDDEN ( __exidx_start = .);
    *(.ARM.exidx*)
    PROVIDE_HIDDEN (__exidx_end = .);
  } > flash

  /* Stack */
  .stack :
  {
    . = ALIGN(4) + 0x00001000;
    _TOP_OF_STACKS_ = .;
  } > sram2
  
  /* Heap */
  .heap :
  {
    . = ALIGN(4);
    __end__ = .;
    end = __end__;
    __HeapBase = __end__;
    *(.heap*)
    . = ALIGN(4) + 0x0000;
    __HeapLimit = .;
  } > sram2
  
  /* Uninitialized data */
  .bss :
  {
    _BSS_START_ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _BSS_END_ = .;
  } > sram1

  /* Initialized data */
  .data ALIGN(4) :
  {
    _DATA_RAM_START_ = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _DATA_RAM_END_ = .;
  } > sram1 AT> flash

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

