#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *worker(void *arg) {
    long long counter = 0;

    while (1) {
        counter++;
        // Решение проблемы: явная проверка флага отмены в TCB
        // pthread_testcancel();
    }

    return NULL;
}

int main(void) {
    pthread_t tid;
    int err;

    err = pthread_create(&tid, NULL, worker, NULL);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        return 1;
    }

    sleep(1);

    err = pthread_cancel(tid);
    if (err) {
        fprintf(stderr, "pthread_cancel() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, NULL);
    if (err) {
        fprintf(stderr, "pthread_join() failed: %s\n", strerror(err));
        return 1;
    }

    printf("main: thread was cancelled\n");
    return 0;
}
