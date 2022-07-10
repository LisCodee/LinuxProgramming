#include<stdio.h>

int a[100], N=100;
int main(){
    int i, sum;
    for (i = 0; i < N;i++)
        a[i] = i + 1;
    sum = Asum(a, N);
    printf("sum = %d", sum);
}
