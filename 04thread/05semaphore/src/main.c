#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM  10
#define RSRC_NUM    3

void* thread_func(void*);

void create_threads(int);

sem_t rsrcSem;

pthread_t tids[THREAD_NUM];
int ids[THREAD_NUM];

int main() {
    sem_init(&rsrcSem, 0, RSRC_NUM);
    create_threads(THREAD_NUM);
    while(1) {
        sleep(5);
    }
    return 0;
}

void* thread_func(void* arg) {
    int id = *(int*)arg;
    //pthread_detach(pthread_self());
    printf("thread %d waiting for semaphore\n", ids[id]);
    sem_wait(&rsrcSem);
    printf("thread %d took semaphore\n", ids[id]);
    sleep(5 + id);
    sem_post(&rsrcSem);
    printf("thread %d done and gave semaphore\n\n", ids[id]);
    pthread_exit(NULL);
}


void create_threads(int num) {
    for(int i = 0; i < num; i++) {
        ids[i] = i;
        int creationStatus = pthread_create(&tids[i], NULL, thread_func, &ids[i]);
        if(creationStatus != 0) {
            printf("failed to create thread %d, code = %d\n", i, creationStatus);
        } else {
            printf("successfully created thread %d, code = %d, TID = %lu\n", i, creationStatus, (unsigned long)tids[i]);
        }
    }
}
