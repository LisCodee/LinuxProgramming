	.file	"h3.c"
	.comm	a,400,32
	.globl	N
	.data
	.align 4
	.type	N, @object
	.size	N, 4
N:
	.long	100
	.section	.rodata
.LC0:
	.string	"sum = %d"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$20, %esp
	movl	$0, -16(%ebp)
	jmp	.L2
.L3:
	movl	-16(%ebp), %eax
	leal	1(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, a(,%eax,4)
	addl	$1, -16(%ebp)
.L2:
	movl	N, %eax
	cmpl	%eax, -16(%ebp)
	jl	.L3
	movl	N, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$a
	call	Asum
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	subl	$8, %esp
	pushl	-12(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	movl	$0, %eax
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
