MEMORY
{
  ram(rwxa) : org = 0x10000, l = 2M
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