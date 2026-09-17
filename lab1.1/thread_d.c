#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define THREADS_COUNT 2

int global_var = 10;

void *worker(void *arg) {
    int thread_num = *(int *)arg;
    int local_var = 20 + thread_num;

    printf("Thread %d before: local_var=%d global_var=%d\n",
           thread_num, local_var, global_var);

    local_var += 5;
    global_var += 5;

    printf("Thread %d after: local_var=%d global_var=%d\n",
           thread_num, local_var, global_var);

    return NULL;
}

int main(void) {
    pthread_t tids[THREADS_COUNT];
    int thread_nums[THREADS_COUNT];
    int i;
    int err;

    for (i = 0; i < THREADS_COUNT; i++) {
        thread_nums[i] = i;
        err = pthread_create(&tids[i], NULL, worker, &thread_nums[i]);
        if (err) {
            fprintf(stderr, "pthread_create failed for thread %d: %s\n",
                    i, strerror(err));
            return 1;
        }
    }

    for (i = 0; i < THREADS_COUNT; i++) {
        err = pthread_join(tids[i], NULL);
        if (err) {
            fprintf(stderr, "pthread_join failed for thread %d: %s\n",
                    i, strerror(err));
            return 1;
        }
    }

    printf("Final global_var = %d\n", global_var);
    return 0;
}