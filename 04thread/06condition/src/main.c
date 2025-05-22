#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* assign_func(void*);  
void* display_func(void*);

volatile char is_ready = 0;

char* data = NULL;

char display_done = 0;

pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t data_cond = PTHREAD_COND_INITIALIZER;

int main() {
    pthread_t display_tid;
    pthread_create(&display_tid, NULL, display_func, NULL);

    pthread_t assign_tid;
    pthread_create(&assign_tid, NULL, assign_func, NULL);

    pthread_join(display_tid, NULL);
    pthread_join(assign_tid, NULL);
}


/* WITHOUT CONDITION BEGIN---------------------------------------------------------*/

void* display_func(void* arg) {
    printf("[DISPLAY------] trying to lock mutex\n");
    pthread_mutex_lock(&data_mutex);
    printf("[DISPLAY------] already locked the mutex, gonna sleep 5s ...\n");
    sleep(5);;
    printf("[DISPLAY------] done, final data = %s\n", data);
    pthread_mutex_unlock(&data_mutex);
    pthread_exit(NULL);
}

void* assign_func(void* arg) {
    printf("[ASSIGN++++++] trying to lock mutex\n");
    pthread_mutex_lock(&data_mutex);
    printf("[ASSIGN++++++] already locked the mutex, gonna sleep 5s ...\n");
    sleep(5);
    data = malloc(100);
    sprintf(data, "from assign\n");
    printf("[ASSIGN++++++] done\n");
    pthread_mutex_unlock(&data_mutex);
     pthread_exit(NULL);
}

/* WITHOUT CONDITION END ---------------------------------------------------------*/

/* WITH CONDITION BEGIN -----------------------------------------------------------*/

// void* display_func(void* arg) {
//     while(!display_done) { 
//         printf("[DISPLAY------] trying to lock mutex\n");

//         pthread_mutex_lock(&data_mutex);
//         printf("[DISPLAY------] already locked the mutex\n");
//         while(!is_ready) {
//             printf("[DISPLAY------] but data aint ready yet, gonna unlock mutex ...\n");
//             pthread_cond_wait(&data_cond, &data_mutex);
//         }
//         printf("[DISPLAY------] data is readay, gonna sleep 5s ...\n");
//         sleep(5);

//         printf("[DISPLAY------] done, final data = %s\n", data);

//         display_done = 1;

//         is_ready = 0;

//         printf("[DISPLAY------] display_done = %d\n", display_done);

//         pthread_mutex_unlock(&data_mutex);
//         //pthread_exit(NULL);
//     }
//     pthread_exit(NULL);
    
// }

// void* assign_func(void* arg) {
//     printf("[ASSIGN++++++] trying to lock mutex\n");

//     pthread_mutex_lock(&data_mutex);
//     printf("[ASSIGN++++++] already locked the mutex, gonna sleep 5s ...\n");
//     sleep(5);

//     data = malloc(100);
//     sprintf(data, "from assign\n");

//     printf("[ASSIGN++++++] done\n");

//     is_ready = 1;

//     pthread_cond_signal(&data_cond);

//     pthread_mutex_unlock(&data_mutex);
//     pthread_exit(NULL);
// }

