/**
 * Write a C program to wait for input from the user using `select()`. 
 * If there is input on stdin within 10 seconds,read and display it.
 * Otherwise, print a timeout message.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_FD      (STDIN_FILENO + 1)

int main() {
    fd_set readfds;                     // the fds set
    FD_ZERO(&readfds);                  // empty set
    FD_SET(STDIN_FILENO, &readfds);     // add STDIN to fds set

    /* 10s timeout */
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("waiting for input in stdin, timeout in 10s...\n");

    int re = select(MAX_FD, &readfds, NULL, NULL, &timeout);

    if(re == -1) {
        perror("select()");
        exit(1);
    } else if(re == 0) {
        printf("timeout\n");
    } else {
        /** --- this technically works but is kinda stupid for hard coding since I determined that it knows which fds raise event */
        // char buffer[1024];
        // int size_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        // if(size_read > 0) {
        //     buffer[size_read] = '\0'; 
        //     printf("read from stdin: %s", buffer);
        // } else if(size_read == 0) {
        //     printf("?\n");
        // } else {
        //     perror("read()");
        // }
        /** --- */
        if(FD_ISSET(STDIN_FILENO, &readfds)) {
            char buffer[1024];
            int size_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if(size_read > 0) {
                buffer[size_read] = '\0'; 
                printf("read from stdin: %s", buffer);
            } else if(size_read == 0) {
                printf("?\n");
            } else {
                perror("read()");
            }
        }
    }
    return 0;
}
