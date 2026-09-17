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
    int err;
    int i = 0;

    while (1) {
        i++;
        err = pthread_create(&tid, NULL, worker, NULL);
        if (err) {
            fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
            return 1;
        }

        printf("main: created thread #%d\n", i);

        sleep(15);
    }

    return 0;
}
