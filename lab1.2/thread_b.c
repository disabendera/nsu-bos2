#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *worker(void *arg) {
    int *result = malloc(sizeof(int));
    if (result == NULL) {
        perror("malloc");
        return NULL;
    }

    *result = 42;
    printf("worker: returns 42\n");
    return (void *)result;
}

int main(void) {
    pthread_t tid;
    void *status;
    int *value;
    int err;

    printf("main: start\n");

    err = pthread_create(&tid, NULL, worker, NULL);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, &status);
    if (err) {
        fprintf(stderr, "pthread_join() failed: %s\n", strerror(err));
        return 1;
    }

    value = (int *)status;
    printf("main: received value = %d\n", *value);
    free(value);
    return 0;
}
