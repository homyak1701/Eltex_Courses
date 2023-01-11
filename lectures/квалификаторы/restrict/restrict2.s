	.file	"restrict2.c"
	.text
	.globl	update
	.type	update, @function
update:
.LFB23:
	.cfi_startproc
	endbr64
	movl	(%rdx), %eax
	addl	%eax, (%rdi)
	movl	(%rdx), %eax
	addl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE23:
	.size	update, .-update
	.globl	main
	.type	main, @function
main:
.LFB24:
	.cfi_startproc
	endbr64
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE24:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
