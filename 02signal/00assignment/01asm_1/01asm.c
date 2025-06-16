/**
 * block SIGINT by using sigaction or signal syscall to handle SIGINT
 */
#define _POSIX_C_SOURCE     200809L

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void sigint_handler(int);

int main() {
    printf("pid = %d\n", getpid());
    
    struct sigaction act = {0};

    /* add SIGINT to mask, default flags */
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);

    act.sa_flags = 0;

    act.sa_handler = sigint_handler;

    sigaction(SIGINT, &act, NULL);

    while(1) {
        sleep(2);
    }

    return 0;
}

/**
 * simply print without exiting
 */
void sigint_handler(int sig) {
    printf("\n%s aint killing me anymore. Hit me harder\n", strsignal(sig));
}