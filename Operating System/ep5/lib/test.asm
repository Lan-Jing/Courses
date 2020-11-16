	.file	"test.c"
	.code16gcc
	.intel_syntax noprefix
	.text
	.p2align 2,,3
	.globl	_start
	.def	_start;	.scl	2;	.type	32;	.endef
_start:
	push	ebx
	sub	esp, 44
	call	_getch
	movsx	eax, al
	mov	DWORD PTR [esp], eax
	call	_putchar
	call	_getchar
	movsx	eax, al
	mov	DWORD PTR [esp], eax
	call	_putchar
	lea	ebx, [esp+4]
	mov	DWORD PTR [esp], ebx
	call	_gets
	mov	DWORD PTR [esp], ebx
	call	_puts
	add	esp, 44
	pop	ebx
	ret
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
	.def	_getch;	.scl	2;	.type	32;	.endef
	.def	_putchar;	.scl	2;	.type	32;	.endef
	.def	_getchar;	.scl	2;	.type	32;	.endef
	.def	_gets;	.scl	2;	.type	32;	.endef
	.def	_puts;	.scl	2;	.type	32;	.endef
