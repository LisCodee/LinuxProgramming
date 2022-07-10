

.global Asum
Asum:
    pushl   %ebp
    movl    %esp, %ebp
    movl    -4, %ecx
    movl    -16(%ebp), %ebx
    
loop:
    movl    (%ebx), %eax
    addl    $4, %ebx
    subl    $1, %ecx
    jne     loop

    ret

