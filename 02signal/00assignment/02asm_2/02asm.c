/**
 * use sigprocmask to get the current signal mask
 */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>

/* */
void sigaddset_multi(sigset_t*, int, ...);
sigset_t get_sigmask(void);

int main() {
    sigset_t block_set;
    sigemptyset(&block_set);

    /* add SIGINT, SIGTERM to the block_set & apply block_set*/ 
    sigaddset_multi(&block_set, 3, SIGINT, SIGTERM, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block_set, NULL);

    sigset_t mask = get_sigmask();

    for(int i = 1; i < _NSIG; i++) {
        if(sigismember(&mask, i)) {
            printf("SIG: %s(%d) is blocked\n", strsignal(i), i);
        }
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

/**
 * @brief   get process' signal mask
 * @return  sigset_t    process current mask
 */
sigset_t get_sigmask(void) {
    sigset_t current_mask;
    sigemptyset(&current_mask);

    // apply nothing and get current_mask as old mask
    sigprocmask(SIG_BLOCK, NULL, &current_mask);

    return current_mask;
}
