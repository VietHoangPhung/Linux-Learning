#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void parent_process(void);
void child_process_1(void);

int main() {
	int pid = fork();
	if(pid == 0) {
		child_process_1();
	} else {
		parent_process();
	}
}

void parent_process(void) {
	unsigned int count;
	while(1) {
		printf("This is from parent. PID = %d, count = %d\n", getpid(), count++);
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
