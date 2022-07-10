.text
.global sub,a,b,printf
sub:
	pushl	%ebp
	movl	%esp,%ebp
	
	pushl	b
	pushl 	a
	pushl	$fmt
	call	printf
	addl 	$12,%esp
	
	movl	%ebp,%esp
	popl	%ebp
	ret

.data
fmt:	.asciz	"a=%d b=%d\n"
