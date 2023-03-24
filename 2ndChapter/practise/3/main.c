#include<stdio.h>

int a[100], N=100;

// Asum(int* a, int n)
// {
//     int res = 0;
//     for(int i = 0; i < n; ++i)
//     {
//         res += a[i];
//     }
//     return res;
// }
extern int Asum(int* a, int n);
int main()
{
    int i, sum;
    for(i = 0;i < N; ++i)
    {
        a[i] = i + 1;
    }
    sum = Asum(a, N);
    //pushl N
    //pushl a
    //call Asum
    printf("sum = %d\n", sum);
}