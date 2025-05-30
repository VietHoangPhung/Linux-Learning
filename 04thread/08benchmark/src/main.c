#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define THREADS_NUM     6
#define LOOP_COUNT      1000

void* spinlock_thread_func(void*);
void* mutex_thread_func(void*);
void* sem_thread_func(void*);
void* atomic_thread_func(void*);

void create_threads(int, void*(*)(void*));


pthread_spinlock_t spinlock;
pthread_mutex_t mutex;
sem_t semaphore;
atomic_flag a_flag = ATOMIC_FLAG_INIT;

struct timespec start, finish;
long sem_ns, mutex_ns, spinlock_ns, atomic_ns;

unsigned int count;

pthread_t* pTIDs = NULL;
char* pIDs = NULL;

int main() {
    if(pthread_spin_init(&spinlock, 0) == 0) {
        printf("successfully initialized spinlock\n\n");
    } else {
        printf("faied to create spinlock ...\n\n");
    }

    if(pthread_mutex_init(&mutex, NULL) == 0) {
        printf("successfully initialized mutex\n\n");
    } else {
        printf("faied to create mutex ...\n\n");
    }

    if(sem_init(&semaphore, 0, 1) == 0) {
        printf("successfully initialized semaphore\n\n");
    } else {
        printf("faied to create sempahore ...\n\n");
    }

    // spinlock
    printf("testing spinlock ...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    create_threads(THREADS_NUM, spinlock_thread_func);
    for(int i = 0; i < THREADS_NUM; i++) {
        pthread_join(*(pTIDs + i), NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    spinlock_ns = (finish.tv_sec - start.tv_sec) * 1e9 + (finish.tv_nsec - start.tv_nsec);

    printf("RESULT: spinlock: %ld seconds\n", spinlock_ns/ 1000000000);

    free(pTIDs);
    free(pIDs);

    pTIDs = NULL;
    pIDs = NULL;

    printf("done testing spinlock, count = %d\n\n", count);
    count = 0;

    // semaphore
    printf("testing semaphore ...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    create_threads(THREADS_NUM, sem_thread_func);
    for(int i = 0; i < THREADS_NUM; i++) {
        pthread_join(*(pTIDs + i), NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    sem_ns = (finish.tv_sec - start.tv_sec) * 1e9 + (finish.tv_nsec - start.tv_nsec);

    printf("RESULT: semaphore: %ld seconds\n", sem_ns/ 1000000000);

    free(pTIDs);
    free(pIDs);

    pTIDs = NULL;
    pIDs = NULL;

    printf("done testing semaphore, count = %d\n\n", count);
    count = 0;

    // mutex
    printf("testing mutex ...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    create_threads(THREADS_NUM, mutex_thread_func);
    for(int i = 0; i < THREADS_NUM; i++) {
        pthread_join(*(pTIDs + i), NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    mutex_ns = (finish.tv_sec - start.tv_sec) * 1e9 + (finish.tv_nsec - start.tv_nsec);
    free(pTIDs);
    free(pIDs);
    printf("RESULT: mutex: %ld seconds\n", mutex_ns/ 1000000000);

    pTIDs = NULL;
    pIDs = NULL;

    printf("done testing mutex, count = %d\n\n", count);
    count = 0;

    // atomic
    printf("testing atomic ...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);

    create_threads(THREADS_NUM, atomic_thread_func);
    for(int i = 0; i < THREADS_NUM; i++) {
        pthread_join(*(pTIDs + i), NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    atomic_ns = (finish.tv_sec - start.tv_sec) * 1e9 + (finish.tv_nsec - start.tv_nsec);

    printf("RESULT: atomic: %ld seconds\n", atomic_ns/ 1000000000);
    free(pTIDs);
    free(pIDs);

    pTIDs = NULL;
    pIDs = NULL;

    printf("done testing atomic, count = %d\n\n", count);

    printf("RESULT:\nsemaphore:   %ld nanosecs\nmutex:       %ld nanosecs\nspinlock:    %ld nanosecs\natomic flag: %ld nanosecs\n", sem_ns, mutex_ns, spinlock_ns, atomic_ns);
    pthread_spin_destroy(&spinlock);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    return 0;
}

/* create threads ---------------------------------------------------------------------------*/
void create_threads(int num, void*(*thread_func)(void*)) {
    pTIDs = malloc(num * sizeof(pthread_t));
    pIDs = malloc(num * sizeof(char));

    for(int i = 0; i < num; i++) {
        *(pIDs + i) = i;
        int create_ret = pthread_create((pTIDs + i), NULL, thread_func, (pIDs + i));
        if(create_ret != 0) {
            // printf("successfully created thread %d, TID = %lu\n", i, *(pTIDs + i));
        // } else {
            printf("failed to create thread %d, error code = %d\n", i, create_ret);
        }
    }
}

/* spinlock threads ---------------------------------------------------------------------------*/
void* spinlock_thread_func(void* arg) {
    char id = *(char*)arg;

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(id, &cpuset);
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // printf("I'm thread %d, pinned to core %d, TID = %lu\n", id, id, pthread_self());
    // printf("I'm thread %d, TID = %lu\n", id, pthread_self());

    for(int i = 0; i < LOOP_COUNT; i++) {
        pthread_spin_lock(&spinlock);
        count++;
        pthread_spin_unlock(&spinlock);
    }
    // printf("thread %d last count = %d\n", id, count);
    pthread_exit(NULL);
} 

/* mutex threads ---------------------------------------------------------------------------*/
void* mutex_thread_func(void* arg) {
    char id = *(char*)arg;

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(id, &cpuset);
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // printf("I'm thread %d, pinned to core %d, TID = %lu\n", id, id, pthread_self());
    // printf("I'm thread %d, TID = %lu\n", id, pthread_self());

    for(int i = 0; i < LOOP_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
    }

    // printf("thread %d last count = %d\n", id, count);
    pthread_exit(NULL);
} 

/* semaphore threads ---------------------------------------------------------------------------*/
void* sem_thread_func(void* arg) {
    char id = *(char*)arg;

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(id, &cpuset);
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // printf("I'm thread %d, pinned to core %d, TID = %lu\n", id, id, pthread_self());

    // printf("I'm thread %d, TID = %lu\n", id, pthrtf("I'm thread %d, pinned to core %d, TID = %lu\n", id, id, pthread_self());
    // printf("ead_self());

    for(int i = 0; i < LOOP_COUNT; i++) {
        sem_wait(&semaphore);
        count++;
        sem_post(&semaphore);
    }
    // printf("thread %d last count = %d\n", id, count);
    pthread_exit(NULL);
} 

/* atomic spinlock threads ---------------------------------------------------------------------------*/
void* atomic_thread_func(void* arg) {
    char id = *(char*)arg;

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(id, &cpuset);
    // pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    // printf("I'm thread %d, pinned to core %d, TID = %lu\n", id, id, pthread_self());
    // printf("I'm thread %d, TID = %lu\n", id, pthread_self());

    for(int i = 0; i < LOOP_COUNT; i++) {
        while(atomic_flag_test_and_set(&a_flag));
        count++;
        atomic_flag_clear(&a_flag);
    }
    // printf("thread %d last count = %d\n", id, count);
    pthread_exit(NULL);
} 
