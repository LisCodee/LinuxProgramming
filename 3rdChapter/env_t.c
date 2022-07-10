#include<stdio.h>

int main(int argc, char *argv[], char *env[]){
    int argv_len, env_len;
    printf("argc is %d\n", argc);
    int i = 0;
    for(; i < argc; ++i)
        printf("%s\n", argv[i]);
    for(i = 0;i < 20; ++i)
        printf("%s\n", env[i]);
}
