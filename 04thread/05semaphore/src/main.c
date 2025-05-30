#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM  10
#define RSRC_NUM    3
#define NO          -1

void* thread_func(void*);

void create_threads(int);

char get_rsrc(char*);

sem_t rsrc_sem;

pthread_mutex_t status_mutex;

char rsrcs_status[RSRC_NUM] = {0};

pthread_t tids[THREAD_NUM];
int ids[THREAD_NUM];

int main() {
    sem_init(&rsrc_sem, 0, RSRC_NUM);
    create_threads(THREAD_NUM);
    while(1) {
        sleep(5);
    }
    return 0;
}

void create_threads(int num) {
    for(int i = 0; i < num; i++) {
        ids[i] = i;
        int creation_status = pthread_create(&tids[i], NULL, thread_func, &ids[i]);
        if(creation_status != 0) {
            printf("failed to create thread %d, code = %d\n", i, creation_status);
        } else {
            printf("successfully created thread %d, code = %d, TID = %lu\n", i, creation_status, (unsigned long)tids[i]);
        }
    }
}

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;
    //pthread_detach(pthread_self());
    printf("thread %d waiting for semaphore\n", ids[thread_id]);
    sem_wait(&rsrc_sem);
    printf("thread %d took semaphore\n", ids[thread_id]);

    int rsrc_id = get_rsrc(rsrcs_status);

    if(rsrc_id != NO) {
        printf("thread %d is gonna use rsrc no %d in %d second(s)\n", thread_id, rsrc_id, (5 + thread_id));
        sleep(5 + thread_id);
        pthread_mutex_lock(&status_mutex);
        rsrcs_status[rsrc_id] = 0;
        pthread_mutex_unlock(&status_mutex);
    } else {
        printf("thread %d found no rsrc available\n", thread_id);
    }
    
    sem_post(&rsrc_sem);
    printf("thread %d done and gave semaphore\n\n", ids[thread_id]);
    pthread_exit(NULL);
}


char get_rsrc(char* statuses) {
    char res = NO;
    pthread_mutex_lock(&status_mutex);
    for(int i = 0; i < RSRC_NUM; i++) {
        if(*(statuses + i) == 0) {
            res = i;
            *(statuses + i) = 1;
            break;
        }
    }
    pthread_mutex_unlock(&status_mutex);
    return res;
}
