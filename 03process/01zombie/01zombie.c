#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void parent_process(int);
void child_process_1(void);

int main() {
	int pid = fork();
	if(pid == 0) {
		child_process_1();
	} else {
		parent_process(pid);
	}
}

void parent_process(int child_id) {
	unsigned int count;
	while(1) {
		printf("This is from parent. PID = %d, count = %d\n", getpid(), count++);
		if(count > 20) {
			int status = 0;
			int tmp = waitpid(child_id, &status, WNOHANG);
			if(tmp > 0) {
				printf("my child %d ended, done cleaning up\n", tmp);
				if(WIFEXITED(status)) {
					printf("It died in peace with code %d. Lomen\n", WEXITSTATUS(status));
				} else if(WIFSIGNALED(status)) {
					printf("It was terminated/killed by SIGNAL %d\n", WTERMSIG(status));
				} else if(WIFSTOPPED(status)) {
					printf("It was stopped by SIGNAL %d\n", WSTOPSIG(status));
				} else if(WIFCONTINUED(status)) {
					printf("Oh now it's continued\n");
				} else {
					printf("I dont know wtf is going on here\n");
				}
			}
		}
		sleep(2);

	}
}

void child_process_1(void) {
	unsigned int count;
	while(1) {
		int ppid = getppid();
		printf("I'm a child process, PID = %d,  my parent process %d, count = %d\n", getpid(), ppid, count++);
		if(count == 10) {
			printf("I npw wil exit and become a zombie till mah pa kill me\n");
			exit(0);
		}
		sleep(2);
	}
}
