    .global running, scheduler, tswitch
tswitch:
SAVE:   
    pushl   %eax
    pushl   %ebx
    pushl   %ecx
    pushl   %edx
    pushl   %ebp
    pushl   %esi
    pushl   %edi
    pushfl
    movl    running, %ebx   # ebx->proc
    movl    %esp,4(%ebx)    #proc.save_sp = exp
FIND:
    call    scheduler
RESUME:
    movl    running, %ebx
    movl    4(%ebx), %esp
    popfl
    popl    %edi
    popl    %esi
    popl    %ebp
    popl    %edx
    popl    %ecx
    popl    %ebx
    popl    %eax
    ret

