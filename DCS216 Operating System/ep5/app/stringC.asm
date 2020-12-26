	.file	"string.c"
	.code16gcc
	.intel_syntax noprefix
	.text
	.p2align 2,,3
	.globl	_countA
	.def	_countA;	.scl	2;	.type	32;	.endef
_countA:
	push	ebx
	mov	ebx, DWORD PTR [esp+12]
	test	ebx, ebx
	jle	L5
	mov	edx, DWORD PTR [esp+8]
	add	ebx, edx
	xor	eax, eax
	.p2align 2,,3
L4:
	mov	cl, BYTE PTR [edx]
	and	ecx, -33
	cmp	cl, 65
	jne	L3
	inc	eax
L3:
	inc	edx
	cmp	ebx, edx
	jne	L4
	pop	ebx
	ret
	.p2align 2,,3
L5:
	xor	eax, eax
	pop	ebx
	ret
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
