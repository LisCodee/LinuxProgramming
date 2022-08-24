#include<stdio.h>
#include<stdarg.h>

int func(int m, int n, ...)
{
    int i;
    va_list ap;
    va_start(ap, n);
    for(i = 0;i < m; ++i)
        printf("%d ", va_arg(ap, int));
    for(i = 0; i < n; ++i)
        printf("%s ", va_arg(ap, char*));
    va_end(ap);
}

int main(){
    func(3,2,1,2,3,"test","OK");
    return 0;
}

