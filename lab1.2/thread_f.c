#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *worker(void *arg) {
    printf("worker: pthread_self=%lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_attr_t attr;
    int err;
    int i = 0;

    err = pthread_attr_init(&attr);
    if (err) {
        fprintf(stderr, "pthread_attr_init() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        fprintf(stderr, "pthread_attr_setdetachstate() failed: %s\n", strerror(err));
        return 1;
    }

    while (1) {
        i++;
        err = pthread_create(&tid, &attr, worker, NULL);
        if (err) {
            fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
            return 1;
        }

        printf("main: created thread #%d\n", i);
        usleep(100000); /* 100 ms */
    }

    pthread_attr_destroy(&attr);
    return 0;
}
