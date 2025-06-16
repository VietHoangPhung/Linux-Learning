/**
 * make a fake shared mutex between 2 processes using SIGUSR1 and SIGUSR2
 * fire SIGUSR1 to ask for use, receive SIGUSR2 = granted
 * random sleep = use time and rest time
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

int rand_in_range(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void sigeditset_multi(sigset_t* set, int (*op)(sigset_t*, int), int sig_count, ...);

void sigusr1_handler(int, siginfo_t*, void*);

void process_func(int);


int main() {
    /* set up usr1 signal */
    struct sigaction usr1_act = {0};
    usr1_act.sa_sigaction = sigusr1_handler;
    usr1_act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &usr1_act, NULL);

    int pid = fork();
    if(pid == 0) {      // child
        sleep(1);
        process_func(getppid());
    } else {            // parent
        process_func(pid);
    }

    return 0;
}


/**
 * process function
 * 
 */
void process_func(int target_pid) {
    int duration;
    srand(time(NULL) ^ getpid());

    /* mask to block SIGUSR1, SIGUSR2, SIGINT, SIGTERM */
    sigset_t mask = {0};
    sigemptyset(&mask);
    sigeditset_multi(&mask, sigaddset, 4, SIGUSR1, SIGUSR2, SIGINT, SIGTERM);

    /* set to wait for SIGUSR2*/
    sigset_t wait_set;
    sigemptyset(&wait_set);
    sigaddset(&wait_set, SIGUSR2); 

    sleep(1);

    while(1) {
        /* block SIGUSR1, SIGUSR2, SIGINT, SIGTERM */
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("[%d] asking for permission from %d\n", getpid(), target_pid);
        kill(target_pid, SIGUSR1);          // fire usr1 to ask for permission

        int sig;
        /* wait for SIGUSR2 */
        sigwait(&wait_set, &sig);
        printf("[%d] got %d, got permission\n", getpid(), sig);

       /* simulate work time ... */
        duration = rand_in_range(2, 6);
        printf("[%d] in work in %d secs\n", getpid(), duration);
        sleep(duration);
        printf("[%d] done\n", getpid());

        /* unblock SIGUSR1, SIGINT, SIGTERM*/
        sigprocmask(SIG_UNBLOCK, &mask, NULL);

        /* simulate rest time */
        duration = rand_in_range(2, 6);
        printf("[%d] in rest in %d secs\n", getpid(), duration);
        sleep(duration);
        printf("[%d] woke up\n", getpid());
    } 
}

/**
 * @note    need flags = SA_SIGINFO
 * @brief   fire SIGUSR2 to target as permission granted
 * 
 */
void sigusr1_handler(int sig, siginfo_t *info, void *context) {
    printf("[%d] got sig %d from %d. Giving the permission\n", getpid(), sig, info->si_pid);
    kill(info->si_pid, SIGUSR2);            // done / unuse signal
}

/**
 * @brief   add or del multi signal to/from a set
 * @param   set -> pointer to the set
 * @param   int (*op) pointer to function sigaddset() or sigdelset()
 * @param   sig_count total signals to add
 * @param   ... -> the signals to add
 * @return  none 
 */
void sigeditset_multi(sigset_t* set, int (*op)(sigset_t*, int), int sig_count, ...) {
    va_list sigs;
    va_start(sigs, sig_count);      // where sigs list start

    for(int i = 0; i < sig_count; i++) {
        int sig = va_arg(sigs, int);

        /* check if it's in range of signal number <1 - 64>*/
        if(sig < 1 || sig > _NSIG - 1) {
            printf("[sigaddset_multi]smol warning: %d doesnt seem like a signo and was skipped\n", sig);
            continue;
        }
        // pass
        op(set, sig);
    }
    va_end(sigs);
}

