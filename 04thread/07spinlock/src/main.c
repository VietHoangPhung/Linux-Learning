#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

#define THREADS_NUM     4

void* thread_func(void*);

void create_threads(int);

//atomic_flag spinlock /*= ATOMIC_FLAG_INIT*/;

pthread_spinlock_t spinlock;

unsigned long count;

pthread_t* pTIDs = NULL;
char* pIDs = NULL;

int main() {
    if(pthread_spin_init(&spinlock, 0) == 0) {
        printf("successfully initialized spinlock\n");
    } else {
        printf("faied to create spinlock ...\n");
    }

    create_threads(THREADS_NUM);

    for(int i = 0; i < THREADS_NUM; i++) {
        pthread_join(*(pTIDs + i), NULL);
    }
    free(pTIDs);
    free(pIDs);

    printf("done, final count = %lu\n", count);
    return 0;
}

void create_threads(int num) {
    pTIDs = malloc(num * sizeof(pthread_t));
    pIDs = malloc(num * sizeof(char));

    for(int i = 0; i < num; i++) {
        *(pIDs + i) = i;
        int create_ret = pthread_create((pTIDs + i), NULL, thread_func, (pIDs + i));
        if(create_ret == 0) {
            printf("successfully created thread %d, TID = %lu\n", i, *(pTIDs + i));
        } else {
            printf("failed to create thread %d, error code = %d\n", i, create_ret);
        }
    }
}

void* thread_func(void* arg) {
    char id = *(char*)arg;
    printf("I'm thread %d, TID = %lu\n", id, pthread_self());

    for(int i = 0; i < 50000000; i++) {
        pthread_spin_lock(&spinlock);
        count ++;
        pthread_spin_unlock(&spinlock);
    }
    printf("thread %d last count = %lu\n", id, count);
    pthread_exit(NULL);
} 
