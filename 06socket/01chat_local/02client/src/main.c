#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    const char* host = "127.0.0.1";
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[4096];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_port = htons(10000);

    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    printf("connected to server\n");

    if(fork() == 0) {
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            int len = read(sockfd, buffer, sizeof(buffer));
            if (len <= 0) break;
            printf("[server]: %s\n", buffer);
        }
    }else{
        while(1) {
            memset(buffer, 0, sizeof(buffer));
            fgets(buffer, sizeof(buffer), stdin);
            write(sockfd, buffer, strlen(buffer));
        }
    }

    close(sockfd);
    return 0;
}
