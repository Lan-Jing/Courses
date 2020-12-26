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
  .rdata _TEXT_END : {
    *(.rdata)
  } > ram
}