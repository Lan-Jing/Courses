MEMORY
{
  ram(rwxa) : org = 0x100, l = 1M
}

SECTIONS
{
  .text : {
    *(.text)
    _TEXT_END = .;
  } > ram
  .data _TEXT_END : {
    *(.data)
    _DATA_END = .;
  } > ram
  .bss _DATA_END : {
    *(.bss)
    _BSS_END = .;
  } > ram
  .rdata _BSS_END : {
    *(.rdata)
  } > ram
}