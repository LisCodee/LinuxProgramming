#include<stdio.h>

int main(){
    int ebp, esp,ebx, ecx, edx, esi, edi;
    ebp = get_ebp();
    esp = get_esp();
    ebx = get_ebx();
    ecx = get_ecx();
    edx = get_edx();
    esi = get_esi();
    edi = get_edi();
    printf("ebp=%8x\tesp=%8x\n", ebp, esp);
    printf("ebx=%8x\tecx=%8x\n", ebx, ecx);
    printf("edx=%8x\tesi=%8x,edi=%8x\n", edx, esi, edi);
}
