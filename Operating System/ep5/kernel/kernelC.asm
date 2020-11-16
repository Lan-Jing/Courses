	.file	"kernel.c"
	.code16gcc
	.intel_syntax noprefix
	.text
	.globl	_pcbPtr
	.data
	.align 4
_pcbPtr:
	.long	_Pcb
	.globl	_Pcb
	.bss
	.align 4
_Pcb:
	.space 28
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
