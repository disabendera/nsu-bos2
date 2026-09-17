#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define THREADS_COUNT 5

void *worker(void *arg) {
    int thread_num = *(int *)arg;

    printf("worker[%d]: pid=%d ppid=%d pthread_self=%lu gettid=%d\n",
           thread_num,
           getpid(),
           getppid(),
           (unsigned long)pthread_self(),
           (int)gettid());

    return NULL;
}

int main(void) {
    pthread_t tids[THREADS_COUNT];
    int thread_nums[THREADS_COUNT];
    int i;
    int err;

    printf("main: pid=%d ppid=%d pthread_self=%lu gettid=%d\n",
           getpid(),
           getppid(),
           (unsigned long)pthread_self(),
           (int)gettid());

    for (i = 0; i < THREADS_COUNT; i++) {
        thread_nums[i] = i;
        err = pthread_create(&tids[i], NULL, worker, &thread_nums[i]);
        if (err) {
            fprintf(stderr, "pthread_create() failed for thread %d: %s\n",
                    i, strerror(err));
            return 1;
        }
    }

    for (i = 0; i < THREADS_COUNT; i++) {
        err = pthread_join(tids[i], NULL);
        if (err) {
            fprintf(stderr, "pthread_join() failed for thread %d: %s\n",
                    i, strerror(err));
            return 1;
        }
    }

    printf("main: all 5 threads finished\n");
    return 0;
}
