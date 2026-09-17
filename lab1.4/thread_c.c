#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void cleanup_handler(void *arg) {
    char *msg = (char *)arg;
    printf("cleanup: freeing string '%s'\n", msg);
    free(msg);
}

void *worker(void *arg) {
    char *msg = malloc(20);
    if (msg == NULL) {
        perror("malloc");
        return NULL;
    }

    snprintf(msg, 20, "hello world");
    pthread_cleanup_push(cleanup_handler, msg);

    while (1) {
        printf("worker: %s\n", msg);
        usleep(100000);
    }

    pthread_cleanup_pop(1);
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

    printf("main: thread was cancelled and memory cleaned\n");
    return 0;
}
