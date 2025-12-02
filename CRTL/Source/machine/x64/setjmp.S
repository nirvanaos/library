.text

.global setjmp
.global _setjmp
setjmp:
_setjmp:
	mov %rbx, 0x00(%rcx)
	mov %rbp, 0x08(%rcx)
	mov %r12, 0x10(%rcx)
	mov %r13, 0x18(%rcx)
	mov %r14, 0x20(%rcx)
	mov %r15, 0x28(%rcx)

	lea 8(%rsp), %rax # rsp before return rip is pushed
	mov %rax, 0x30(%rcx)
	mov (%rsp), %rax # return rip
	mov %rax, 0x38(%rcx)

	xor %rax, %rax
	ret

.global longjmp
.global _longjmp
longjmp:
_longjmp:
	mov 0x00(%rcx), %rbx
	mov 0x08(%rcx), %rbp
	mov 0x10(%rcx), %r12
	mov 0x18(%rcx), %r13
	mov 0x20(%rcx), %r14
	mov 0x28(%rcx), %r15

	mov  %rdx, %rax
	test %rax, %rax
	jnz 1f
	inc %rax
1:
	mov  0x30(%rcx), %rsp
	jmp  *0x38(%rcx)
