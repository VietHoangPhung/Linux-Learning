#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TOTAL   2

void* thread_func_1(void* );
void* thread_func_2(void* );

int main() {
    pthread_t tid1, tid2;

    int creation_status = pthread_create(&tid1, NULL, thread_func_1, NULL);
    if(creation_status != 0) {
        printf("failed to create thread 1, code = %d\n", creation_status);
    } else {
        printf("successfully created thread 1, code = %d, TID = %lu\n", creation_status, (unsigned long)tid1);
    }

    creation_status = pthread_create(&tid2, NULL, thread_func_2, NULL);
    if(creation_status != 0) {
        printf("failed to create thread 1, code = %d\n", creation_status);
    } else {
        printf("successfully created thread 1, code = %d, TID = %lu\n", creation_status, (unsigned long)tid2);
    }

    if(pthread_join(tid1, NULL) == 0) {
        printf("joined thread 1\n");
    }
    
    if(pthread_join(tid2, NULL) == 0) {
        printf("joined thread 2\n");
    }

    return 0;

}

void* thread_func_1(void* arg) {
    unsigned int count = 0;
    while(1) {
        pthread_t tid = pthread_self();
        printf("I'm a living thread. My TID = %lu, thread count = %d\n", (unsigned long)tid, count++);
        if(count > 10) {
            printf("I'm thread 1. I'm gonna leave.\n");
            break;
        }
        sleep(2);
    }
    return NULL;
}

void* thread_func_2(void* arg) {
    unsigned int count = 0;
    while(1) {
        pthread_t tid = pthread_self();
        printf("I'm a living thread. My TID = %lu, thread count = %d\n", (unsigned long)tid, count++);
        if(count > 20) {
            printf("I'm thread 2. I'm gonna leave\n");
            break;
        }
        sleep(2);
    }
    return NULL;
}
