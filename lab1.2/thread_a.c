#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *worker(void *arg) {
    printf("worker: pid=%d tid=%ld start\n", getpid(), (long)pthread_self());
    sleep(1);
    printf("worker: pid=%d tid=%ld finish\n", getpid(), (long)pthread_self());
    return NULL;
}

int main(void) {
    pthread_t tid;
    int err;

    printf("main: pid=%d tid=%ld start\n", getpid(), (long)pthread_self());

    err = pthread_create(&tid, NULL, worker, NULL);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, NULL);
    if (err) {
        fprintf(stderr, "pthread_join() failed: %s\n", strerror(err));
        return 1;
    }

    printf("main: thread finished, main continues\n");
    return 0;
}
