#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void *worker(void *arg) {
    char *msg = strdup("hello world");
    if (msg == NULL) {
        perror("strdup");
        return NULL;
    }

    printf("worker: returns string '%s'\n", msg);
    return (void *)msg;
}

int main(void) {
    pthread_t tid;
    void *status;
    char *msg;
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

    msg = (char *)status;
    printf("main: received string = %s\n", msg);
    free(msg);
    return 0;
}
