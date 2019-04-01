	.file	"main.c"
	.section	.rodata
.LC0:
	.string	"The string: %s\n"
.LC1:
	.string	"New string: %s\n"
	.text
	.globl	test_copy_2
	.type	test_copy_2, @function
test_copy_2:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1650614625, -32(%rbp)
	movl	$1684300643, -28(%rbp)
	movb	$0, -24(%rbp)
	movl	$.LC0, %eax
	leaq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	leaq	-32(%rbp), %rax
	leaq	-32(%rbp), %rdx
	addq	$2, %rdx
	movl	(%rax), %ecx
	movl	%ecx, (%rdx)
	movzwl	4(%rax), %eax
	movw	%ax, 4(%rdx)
	movl	$.LC1, %eax
	leaq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$1650614625, -32(%rbp)
	movl	$1684300643, -28(%rbp)
	movb	$0, -24(%rbp)
	movl	$.LC0, %eax
	leaq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	leaq	-32(%rbp), %rax
	leaq	-32(%rbp), %rdx
	leaq	2(%rdx), %rcx
	movl	$6, %edx
	movq	%rax, %rsi
	movq	%rcx, %rdi
	call	memmove
	movl	$.LC1, %eax
	leaq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L2
	call	__stack_chk_fail
.L2:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	test_copy_2, .-test_copy_2
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %eax
	call	test_copy_2
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
