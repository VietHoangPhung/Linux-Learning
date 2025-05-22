#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func_1(void*);
void* thread_func_2(void*);

void create_threads(int);

int count = 0;

pthread_t tids[2];

void* (*thread_funcs[]) (void*) = {thread_func_1, thread_func_2};

pthread_mutex_t countMutex;

int main() {
    create_threads(2);
    pthread_join(tids[1], NULL);
    pthread_join(tids[1], NULL);
    printf("done, final count = %d\n", count);
    return 0;
}

void* thread_func_1(void* arg) {
    pthread_detach(pthread_self());
    for(int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&countMutex);
        count++;
        pthread_mutex_unlock(&countMutex);
    }
    printf("thread 1 done. count = %d\n", count);
    pthread_exit(NULL);
}

void* thread_func_2(void* arg) {
    pthread_detach(pthread_self());
    for(int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&countMutex);
        count++;
        pthread_mutex_unlock(&countMutex);
    }
    printf("thread 2 done. count = %d\n", count);
    pthread_exit(NULL);
}

void create_threads(int num) {
    for(int i = 0; i < num; i++) {
        int creation_status = pthread_create(&tids[i], NULL, thread_funcs[i], NULL);
        if(creation_status != 0) {
            printf("failed to create thread %d, code = %d\n", i, creation_status);
        } else {
            printf("successfully created thread %d, code = %d, TID = %lu\n", i, creation_status, (unsigned long)tids[i]);
        }
    }
}
