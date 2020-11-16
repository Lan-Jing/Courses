	.file	"kernel.c"
	.code16gcc
	.intel_syntax noprefix
	.text
	.p2align 2,,3
	.globl	_schedule
	.def	_schedule;	.scl	2;	.type	32;	.endef
_schedule:
	cmp	WORD PTR _proCount, 1
	je	L20
	mov	ax, WORD PTR _nowPid
	cmp	WORD PTR _ready, 0
	je	L22
	.p2align 2,,3
L5:
	cmp	ax, 4
	jne	L17
	cmp	BYTE PTR _pcbList+81, 0
	jne	L19
	mov	eax, 1
L17:
	inc	eax
	movsx	edx, ax
	lea	ecx, [edx+edx*4]
	cmp	BYTE PTR _pcbList[edx+40+ecx*8], 0
	je	L5
L8:
	mov	WORD PTR _nowPid, ax
	lea	eax, [edx+edx*4]
	lea	eax, _pcbList[edx+eax*8]
	mov	DWORD PTR _pcbPtr, eax
	ret
L16:
	mov	eax, 2
L11:
	inc	eax
	movsx	edx, ax
	lea	ecx, [edx+edx*4]
	cmp	BYTE PTR _pcbList[edx+40+ecx*8], 0
	jne	L8
	test	ax, ax
	jne	L9
L20:
	mov	WORD PTR _nowPid, 0
	mov	eax, OFFSET FLAT:_pcbList
L3:
	mov	DWORD PTR _pcbPtr, eax
	ret
	.p2align 2,,3
L19:
	mov	edx, 1
	mov	eax, 1
	jmp	L8
L22:
	test	ax, ax
	jne	L9
	mov	eax, OFFSET FLAT:_pcbList
	jmp	L3
L9:
	cmp	ax, 4
	jne	L11
	cmp	BYTE PTR _pcbList+81, 0
	jne	L19
	cmp	BYTE PTR _pcbList+122, 0
	je	L16
	mov	edx, 2
	mov	eax, 2
	jmp	L8
	.globl	_ready
	.bss
	.align 2
_ready:
	.space 2
	.globl	_proCount
	.align 2
_proCount:
	.space 2
	.globl	_nowPid
	.align 2
_nowPid:
	.space 2
	.globl	_pcbPtr
	.align 4
_pcbPtr:
	.space 4
	.globl	_pcbList
	.align 32
_pcbList:
	.space 205
	.ident	"GCC: (MinGW.org GCC-8.2.0-5) 8.2.0"
