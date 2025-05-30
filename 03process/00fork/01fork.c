#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void childProcess(void);
void parentProcess();

int main() {
    int child1_pid = fork();
    if(child1_pid == 0) {
        childProcess();
    } else {
        parentProcess();
    }
    return 0;
}

void childProcess(void) {
    int childCount = 0;
    while(1) {
        int ppid = getppid();
        printf("this is child process, PID = %d\nMy parent's PID = %d\nchild count = %d\n", getpid(), ppid, childCount++);
        // if(childCount == 10) {
        //     printf("It's 10 already, I QUIT!!!\n\n\n");
        //     exit(0);
        // }

        sleep(2);   
    }
}

void parentProcess() {
    int parentCount = 0;
    while(1) {
        printf("this is parent process, PID = %d\nparent count = %d\n", getpid(), parentCount++);
        // int status = 0;
        // int childEnd = waitpid(-1, &status, WNOHANG);
        // if(childEnd > 0) {
        //     printf("my child %d ended\n", childEnd);
        //     if(WIFEXITED(status)) {
        //         printf("It died in peace with code %d. Lomen\n", WEXITSTATUS(status));
        //     } else if(WIFSIGNALED(status)) {
        //         printf("It was terminated/killed by SIGNAL %d\n", WTERMSIG(status));
        //     } else if(WIFSTOPPED(status)) {
        //         printf("It was stopped by SIGNAL %d\n", WSTOPSIG(status));
        //     } else if(WIFCONTINUED(status)) {
        //         printf("Oh now it's continued\n");
        //     } else {
        //         printf("I dont know wtf is going on here\n");
        //     }
        // }
        sleep(2);
    }
}

