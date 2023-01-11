	.file	"switch.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"\302\253His Death Was Just The Beginning\302\273"
.LC1:
	.string	"\302\253Scary Just Got Sexy\302\273"
	.align 8
.LC2:
	.string	"\302\253Fight the dead. Fear the living\302\273"
	.align 8
.LC3:
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\270\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC4:
	.string	"\320\235\320\265\320\277\321\200\320\260\320\262\320\270\320\273\321\214\320\275\321\213\320\271 \320\262\320\262\320\276\320\264."
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
	movl	$4, -4(%rbp)
	cmpl	$4, -4(%rbp)
	je	.L2
	cmpl	$4, -4(%rbp)
	jg	.L3
	cmpl	$3, -4(%rbp)
	je	.L4
	cmpl	$3, -4(%rbp)
	jg	.L3
	cmpl	$1, -4(%rbp)
	je	.L5
	cmpl	$2, -4(%rbp)
	je	.L6
	jmp	.L3
.L5:
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L7
.L6:
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L7
.L4:
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L7
.L2:
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L7
.L3:
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L7:
	call	getchar@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
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
