#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
    printf("this is %d in %s\n", getpid(), argv[0]);
    int r = execl("b.out", "b.out", "hi", 0);
    printf("SEE this line only execl failed\n");
}
