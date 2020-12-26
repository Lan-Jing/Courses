	.file	"lib.c"
	.code16gcc
	.intel_syntax noprefix
	.text
	.p2align 2,,3
	.globl	_strlen
	.def	_strlen;	.scl	2;	.type	32;	.endef
_strlen:
	mov	edx, DWORD PTR [esp+4]
	xor	eax, eax
	cmp	BYTE PTR [edx], 0
	je	L4
	.p2align 2,,3
L3:
	inc	eax
	cmp	BYTE PTR [edx+eax], 0
	jne	L3
	ret
	.p2align 2,,3
L4:
	ret
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
