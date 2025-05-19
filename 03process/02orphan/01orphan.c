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
		if(count > 15) {
			printf("I'm Jack, goodbye\n");
			exit(0);
		}
		printf("This is from parent. PID = %d, count = %d\n", getpid(), count++);
		sleep(2);
	}
}

void child_process_1(void) {
	unsigned int count;
	int isOrphan = 0;
	while(1) {
		int ppid = getppid();
		if(ppid == 1) {
			isOrphan = 1;
			printf("\n");
		}

		if(isOrphan) {
			printf("I'm an orphan. PID = %d, my parent now: %d, count = %d\n", getpid(), getppid(), count++);
		} else {
			printf("I'm a child process, PID = %d,  my parent process %d, count = %d\n", getpid(), ppid, count++);
		}
		sleep(2);
	}
}
