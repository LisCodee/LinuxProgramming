#include<stdio.h>
int B(int x, int y){
    return x - y;
}

int A(int x, int y){
    int d,e,f;
    d = 4;e = 5;f = 6;
    f = B(d,e);
}
