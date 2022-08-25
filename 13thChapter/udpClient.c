#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 1234
#define BUFLEN 256

char line[BUFLEN];
struct sockaddr_in server;
int sock, rlen, slen = sizeof(server);

int main()
{
    printf("1. create a UDP socket\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    printf("2. fill in server address and port number\n");
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_HOST, &server.sin_addr);

    while(1)
    {
        printf("enter a line:\n");
        fgets(line, BUFLEN, stdin);
        line[strlen(line)-1] = 0;
        printf("send line to server\n");
        sendto(sock, line, strlen(line), 0, (struct sockaddr*)&server, slen);
        memset(line, 0, BUFLEN);
        printf("try to receive a line from server...\n");
        rlen = recvfrom(sock, line, BUFLEN, 0, (struct sockaddr*)&server, &slen);
        printf("rlen=%d:line=%s\n", rlen, line);
    }
    sleep(1);
}
