#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//void sigint_handler(int);
void sigint_handler(int);

int main() {
    printf("pid = %d\n", getpid());
    struct sigaction intSigAction = {0};
    intSigAction.sa_handler = sigint_handler;       // handler

    sigemptyset(&intSigAction.sa_mask);             
    sigaddset(&intSigAction.sa_mask, SIGTERM);      // add SIGTERM to mask set

    sigaction(SIGINT, &intSigAction, NULL);

    while(1) {
        sleep(2);
    }
    return 0;
}

void sigint_handler(int sig) {
    printf("I'm undead in 10 sec but dont use silver bullet\n");
    sleep(20);
    printf("ok i'm back to normal now\n");
}
