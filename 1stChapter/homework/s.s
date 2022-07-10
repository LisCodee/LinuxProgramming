

.global get_esp, get_ebp, get_ebx, get_ecx, get_edx, get_esi, get_edi

get_esp:
    movl    %esp, %eax
    ret

get_ebp:
    movl    %ebp, %eax
    ret

get_ebx:
    movl    %ebx, %eax
    ret

get_ecx:
    movl    %ecx, %eax
    ret

get_edx:
    movl    %edx, %eax
    ret

get_esi:
    movl    %esi, %eax
    ret

get_edi:
    movl    %edi, %eax
    ret
