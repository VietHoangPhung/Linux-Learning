#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig);

int main() {
    signal(SIGINT, sigint_handler);
    while(1) {
        static int count = 0;
        printf("loop count = %d, PID = %d\n", count++, getpid());
        sleep(2);
    }
    return 0;
}

void sigint_handler(int sig) {
    printf("\nCaught signal number %d but hehe I aint dying\n", sig);
}
