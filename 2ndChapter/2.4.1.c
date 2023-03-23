#include <stdio.h>

int sub(int x, int y)
{
    int u, v;
    u = 4;
    v = 5;
    return x + y + u + v;
}

int main()
{
    int a,b,c;
    a = 1;
    b = 2;
    c = 3;
    c = sub(a,b);
    printf("c=%d\n", c);
}