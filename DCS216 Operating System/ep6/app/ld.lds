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
  } > ram
}