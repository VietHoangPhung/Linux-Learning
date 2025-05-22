#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TOTAL   2       // total number of threads

/* prototypes ------------------------*/
// thread functions
void* thread_func_1(void*);
void* thread_func_2(void*);

// to join exited thread
void polling_join(void);

// to create threads
void thread_create(int);

/* some variables --------------------*/
// thread ids 
pthread_t tids[TOTAL];

// threads' statuses
char threads_status[TOTAL];

// thread functions
void* (*thread_funcs[]) (void*) = {thread_func_1, thread_func_2};
int thread_num = TOTAL;

int main() {
    thread_create(TOTAL);

    pthread_t tid = pthread_self();

    static int count = 0;
    while(count < 20) {
        printf("main thread. TID = %lu, count = %d\n", tid, count++);
        sleep(2);
    }
    return 0;

}

void* thread_func_1(void* arg) {
    unsigned int count = 0;
    pthread_detach(pthread_self());
    while(1) {
        pthread_t tid = pthread_self();
        printf("I'm a living thread. My TID = %lu, thread count = %d\n", (unsigned long)tid, count++);
        if(count > 10) {
            printf("I'm thread 1. I'm gonna leave.\n");
            break;
        }
        sleep(2);
    }
    pthread_exit(NULL);
}

void* thread_func_2(void* arg) {
    unsigned int count = 0;
    pthread_detach(pthread_self());
    while(1) {
        pthread_t tid = pthread_self();
        printf("I'm a living thread. My TID = %lu, thread count = %d\n", (unsigned long)tid, count++);
        if(count > 15) {
            printf("I'm thread 2. I'm gonna leave\n");
            break;
        }
        sleep(2);
    }
    pthread_exit(NULL);
}

void thread_create(int num) {
    for(int i = 0; i < num; i++) {
        int creation_status = pthread_create(&tids[i], NULL, thread_funcs[i], NULL);
        if(creation_status != 0) {
            printf("failed to create thread %d, code = %d\n", i, creation_status);
        } else {
            printf("successfully created thread %d, code = %d, TID = %lu\n", i, creation_status, (unsigned long)tids[i]);
        }
    }
}

