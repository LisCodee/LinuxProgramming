.global tswitch, scheduler, running

tswitch:
SAVE:
    pushal
    pushfl
    movl    running, %ebx
    movl    %esp, 4(%ebx)
FIND:
    call scheduler
RESUME:
    movl    running, %ebx
    movl    4(%ebx), %esp
    popfl
    popal
    ret

