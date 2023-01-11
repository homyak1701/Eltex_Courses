	.file	"switch2.c"
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
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\270\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261:("
	.align 8
.LC4:
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\270\320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC5:
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202\321\200\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC6:
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202ee\321\202\321\214 \320\275\320\270\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC7:
	.string	"\320\241qweqwe\320\265\320\263\320\276\320\264\320\275\321\217 \321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\270\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC8:
	.string	"\320\241\320\265\320\263\320\276\320\264\320\275\321\217wdefqwf \321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\270\321\207\320\265\320\263\320\276 \320\275\320\265 \320\261\321\203\320\264\320\265\320\274 :("
	.align 8
.LC9:
	.string	"\320\235\320\265\320\277\321\200\320\260\320\262\320\270\320\273\321\214\320\275\321\213\320\271 \320\262wfwvgf\320\262\320\276\320\264."
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
	cmpl	$100000, -4(%rbp)
	je	.L2
	cmpl	$100000, -4(%rbp)
	jg	.L3
	cmpl	$3000, -4(%rbp)
	je	.L4
	cmpl	$3000, -4(%rbp)
	jg	.L3
	cmpl	$9, -4(%rbp)
	jg	.L5
	cmpl	$4, -4(%rbp)
	jl	.L3
	movl	-4(%rbp), %eax
	subl	$4, %eax
	cmpl	$5, %eax
	ja	.L3
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L7(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L7(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L7:
	.long	.L12-.L7
	.long	.L11-.L7
	.long	.L10-.L7
	.long	.L9-.L7
	.long	.L8-.L7
	.long	.L6-.L7
	.text
.L5:
	cmpl	$1000, -4(%rbp)
	jne	.L3
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L2:
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L4:
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L12:
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L11:
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L10:
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L9:
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L8:
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L6:
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L13
.L3:
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L13:
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
