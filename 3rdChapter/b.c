#include<stdio.h>

int main(int argc, char* argv[]){
    printf("this is b.out, my pid is %d\n", getpid());
    printf("argv is %s\n", argv[1]);
    printf("I'm dead\n");
}
