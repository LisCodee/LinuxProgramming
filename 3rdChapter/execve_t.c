#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[], char* env[]){
    char *myargv[3];
    myargv[0] = "ls";
    myargv[1] = "-l";
    myargv[2] = 0;
    int r = execve("/bin/ls", myargv, env);
    printf("execve failed\n");
}
