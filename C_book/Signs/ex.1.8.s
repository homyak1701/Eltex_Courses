	.file	"3.c"
	.text
	.section	.rodata
.LC0:
	.string	"lines=%d, spaces=%d, tabs=%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$0, -16(%rbp)
	movl	$0, -12(%rbp)
	movl	$0, -8(%rbp)
	call	getchar@PLT
	movl	%eax, -4(%rbp)
	jmp	.L2
.L5:
	cmpl	$10, -4(%rbp)
	jne	.L3
	addl	$1, -16(%rbp)
	jmp	.L2
.L3:
	cmpl	$32, -4(%rbp)
	jne	.L4
	addl	$1, -12(%rbp)
	jmp	.L2
.L4:
	cmpl	$9, -4(%rbp)
	jne	.L2
	addl	$1, -8(%rbp)
.L2:
	cmpl	$-1, -4(%rbp)
	jne	.L5
	movl	-8(%rbp), %ecx
	movl	-12(%rbp), %edx
	movl	-16(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
