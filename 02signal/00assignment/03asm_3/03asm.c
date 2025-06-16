/**
 * use sigprocmask to block SIGINT
 * use sigpending to check for SIGINT
 * exit if SIGINT is pending
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

/* */
void sigaddset_multi(sigset_t*, int, ...);

int main() {
    sigset_t block_set, pending_set;
    sigemptyset(&block_set);

    /* add SIGINT, SIGTERM to the block_set & apply block_set*/ 
    sigaddset_multi(&block_set, 4, SIGINT, SIGTERM, SIGUSR1, SIGUSR2);
    sigprocmask(SIG_BLOCK, &block_set, NULL);
    
    int is_pending = 0;

    while(1) {
        sigpending(&pending_set);
        //if(pending_set.__val[0] == 0)
        for(int sig = 1; sig < _NSIG; sig++) {
            if(sigismember(&pending_set, sig)) {
                printf("SIG: %s(%d) is pending\n", strsignal(sig), sig);
                if(sig == SIGINT) is_pending = 1;
            }
        }
        (is_pending == 1) ? exit(0) : sleep(1);
    }

    return 0;
}

/**
 * @brief   add multi signal to a set
 * @param   set -> pointer to the set
 * @param   sig_count total signals to add
 * @param   ... -> the signals to add
 * @return  none 
 */
void sigaddset_multi(sigset_t* set, int sig_count, ...) {
    va_list sigs;
    va_start(sigs, sig_count);      // where sigs list start

    for(int i = 0; i < sig_count; i++) {
        int sig = va_arg(sigs, int);

        /* check if it's in range of signal number <1 - 64>*/
        if(sig < 1 || sig > _NSIG - 1) {
            printf("[sigaddset_multi]smol warning: %d doesnt seem like a signo and was skipped", sig);
            continue;
        }
        // pass
        sigaddset(set, sig);
    }
    va_end(sigs);
}