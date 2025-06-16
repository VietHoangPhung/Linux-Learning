/**
 * Write a C program to register a signal handler for SIGUSR1 and SIGUSR2. 
 * When SIGUSR1 is received, print a custom message. When SIGUSR2 is received, terminate the program.
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void sigusr1_handler(int);
void sigusr2_handler(int);

int main() {
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    while(1) {
        sleep(10);
    }
    return 0;
}

void sigusr1_handler(int sig) {
    printf("received SIGUSR1 || %s(%d)\n", strsignal(sig), sig);
}

void sigusr2_handler(int sig) {
    printf("received SIGUSR2 || %s(%d)\n", strsignal(sig), sig);
    printf("gonna exit....\n");
    exit(0);
}