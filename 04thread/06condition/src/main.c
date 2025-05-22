#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* consumer_func(void*);  
void* provider_func(void*);

volatile char is_ready = 0;

char* data = NULL;

char consumer_done = 0;

pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;

int main() {
    pthread_t consumer_tid;
    pthread_create(&consumer_tid, NULL, consumer_func, NULL);

    pthread_t provider_tid;
    pthread_create(&provider_tid, NULL, provider_func, NULL);

    pthread_join(consumer_tid, NULL);
    pthread_join(provider_tid, NULL);
}


// /* WITHOUT CONDITION BEGIN---------------------------------------------------------*/

void* consumer_func(void* arg) {
    printf("[CONSUMER------] trying to lock mutex\n");
    pthread_mutex_lock(&data_mutex);
    printf("[CONSUMER------] already locked the mutex, gonna sleep 5s ...\n");
    sleep(5);;
    printf("[CONSUMER------] done, final data = %s\n", data);
    pthread_mutex_unlock(&data_mutex);
    pthread_exit(NULL);
}

void* provider_func(void* arg) {
    printf("[PROVIDER++++++] trying to lock mutex\n");
    pthread_mutex_lock(&data_mutex);
    printf("[PROVIDER++++++] already locked the mutex, gonna sleep 5s ...\n");
    sleep(5);
    data = malloc(100);
    sprintf(data, "from provider\n");
    printf("[PROVIDER++++++] done\n");
    pthread_mutex_unlock(&data_mutex);
     pthread_exit(NULL);
}

/* WITHOUT CONDITION END ---------------------------------------------------------*/

/* WITH CONDITION BEGIN -----------------------------------------------------------*/

// void* consumer_func(void* arg) {
//     while(!consumer_done) { 
//         printf("[CONSUMER------] trying to lock mutex\n");

//         pthread_mutex_lock(&data_mutex);
//         printf("[CONSUMER------] already locked the mutex\n");
//         while(!is_ready) {
//             printf("[CONSUMER------] but data aint ready yet, gonna unlock mutex ...\n");
//             pthread_cond_wait(&data_cond, &data_mutex);
//         }
//         printf("[CONSUMER------] data is readay, gonna sleep 5s ...\n");
//         sleep(5);

//         printf("[CONSUMER------] done, final data = %s\n", data);

//         consumer_done = 1;

//         is_ready = 0;

//         printf("[CONSUMER------] consumer_done = %d\n", consumer_done);

//         pthread_mutex_unlock(&data_mutex);
//         //pthread_exit(NULL);
//     }
//     pthread_exit(NULL);
    
// }

// void* provider_func(void* arg) {
//     printf("[PROVIDER++++++] trying to lock mutex\n");

//     pthread_mutex_lock(&data_mutex);
//     printf("[PROVIDER++++++] already locked the mutex, gonna sleep 5s ...\n");
//     sleep(5);

//     data = malloc(100);
//     sprintf(data, "from provider\n");

//     printf("[PROVIDER++++++] done\n");

//     is_ready = 1;

//     pthread_cond_signal(&data_cond);

//     pthread_mutex_unlock(&data_mutex);
//     pthread_exit(NULL);
// }

