#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    const char* host = "127.0.0.1";
    int listenfd, connfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[4096];  // send / receive buffer

    //  listen socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0); // TCP, ipv4

    server_addr.sin_family = AF_INET;           // ipv4
    server_addr.sin_addr.s_addr = inet_addr(host); // string -> binary
    server_addr.sin_port = htons(10000);        // port

    bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(listenfd, 10);

    printf("waiting for connection %s:10000...\n", host);
    connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    printf("client connected %s:%d\n", client_ip, client_port);

    if(fork() == 0) {
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            int len = read(connfd, buffer, sizeof(buffer));
            if (len <= 0) break;
            printf("[client]: %s\n", buffer);
        }
    }else{
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            write(connfd, buffer, strlen(buffer));
        }
    }

    close(connfd);
    close(listenfd);
    return 0;
}
