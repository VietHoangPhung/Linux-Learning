#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>


void handle_sigterm(int);   

int main() {
    signal(SIGTERM, handle_sigterm);

    printf("sending SIGTERM\n");
    //kill(getpid(), SIGTERM);
    raise(SIGTERM);
    return 0;
}

void handle_sigterm(int sig) {
    printf("received sigterm, no = %d\n", sig);
}