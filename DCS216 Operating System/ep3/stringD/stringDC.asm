	.file	"stringD.c"
	.intel_syntax noprefix
	.text
	.globl	_countA
	.def	_countA;	.scl	2;	.type	32;	.endef
_countA:
LFB0:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	sub	esp, 16
	mov	DWORD PTR [ebp-4], 0
	mov	DWORD PTR [ebp-8], 0
	jmp	L2
L5:
	mov	edx, DWORD PTR [ebp-4]
	mov	eax, DWORD PTR [ebp+8]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	cmp	al, 97
	je	L3
	mov	edx, DWORD PTR [ebp-4]
	mov	eax, DWORD PTR [ebp+8]
	add	eax, edx
	movzx	eax, BYTE PTR [eax]
	cmp	al, 65
	jne	L4
L3:
	add	DWORD PTR [ebp-8], 1
L4:
	add	DWORD PTR [ebp-4], 1
L2:
	mov	eax, DWORD PTR [ebp-4]
	cmp	eax, DWORD PTR [ebp+12]
	jl	L5
	mov	eax, DWORD PTR [ebp-8]
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE0:
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
