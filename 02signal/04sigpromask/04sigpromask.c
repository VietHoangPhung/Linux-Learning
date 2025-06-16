#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//sigset_t block_set;

volatile int flag;

void sigusr1_handler(int);

void sigint_handler(int);

void check_blocked();

int main() {
    printf("main tid: %ld\n", pthread_self());
    printf("PID = %d\n", getpid());
    struct sigaction action_user1, action_int;

    printf("mask init blocked:\n");
    check_blocked();

    // block sigint
    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigprocmask(SIG_BLOCK, &block_set, NULL);


    // set up user1_handler
    // action_user1.sa_handler = sigusr1_handler;
    // sigemptyset(&action_user1.sa_mask);
    // action_user1.sa_flags = 0;
    // sigaction(SIGUSR1, &action_user1, NULL);

    // // set up sigint_handler
    // action_int.sa_handler = sigint_handler;
    // sigemptyset(&action_int.sa_mask);
    // action_int.sa_flags = 0;
    // sigaction(SIGINT /*|| SIGUSR1*/, &action_int, NULL);
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    printf("mask before loop blocked:\n");
    check_blocked();

    //raise(SIGUSR1);

    printf("SIGINT is blocked till receive SIGUSR1\n");

    while(1) {
        // if(flag == 1) {
        //     printf("BEFORE unblock: \n");
        //     check_blocked();

        //     sigset_t temp;
        //     sigemptyset(&temp);
        //     sigaddset(&temp, SIGINT);
        //     sigprocmask(SIG_UNBLOCK, &temp, NULL);

        //     printf("AFTER unblock (still in handler):\n");
        //     check_blocked();
        // }
        sleep(1);
    }

    return 0;
}
void sigusr1_handler(int) {
    printf("sigusr1_handler tid: %ld\n", pthread_self());
    printf("BEFORE unblock: \n");
    check_blocked();

    sigset_t temp;
    sigemptyset(&temp);
    sigaddset(&temp, SIGINT);
    sigprocmask(SIG_UNBLOCK, &temp, NULL);

    printf("AFTER unblock (still in handler):\n");
    check_blocked();

    flag = 1;
}


void sigint_handler(int sig) {
    printf("caught SIGINT, exiting\n");
    exit(0);
}

void check_blocked() {
    sigset_t curr_mask;
    sigemptyset(&curr_mask);
    sigprocmask(0, NULL, &curr_mask);

    for(int i = 1; i < NSIG; i++) {
        if(sigismember(&curr_mask, i)) {
            printf("Signal %d is blocked\n\n", i);
        }
    }
}