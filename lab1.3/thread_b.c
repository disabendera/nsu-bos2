#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

struct task {
    int value;
    char *text;
};

void *worker(void *arg) {
    struct task *t = (struct task *)arg;
    printf("worker: value=%d text=%s\n", t->value, t->text);
    free();
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_attr_t attr;
    struct task *t;
    int err;

    t = malloc(sizeof(*t));
    if (t == NULL) {
        perror("malloc");
        return 1;
    }

    t->value = 456;
    t->text = "detached hello";

    err = pthread_attr_init(&attr);
    if (err) {
        fprintf(stderr, "pthread_attr_init() failed: %s\n", strerror(err));
        free(t);
        return 1;
    }

    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err) {
        fprintf(stderr, "pthread_attr_setdetachstate() failed: %s\n", strerror(err));
        pthread_attr_destroy(&attr);
        free(t);
        return 1;
    }

    err = pthread_create(&tid, &attr, worker, t);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        pthread_attr_destroy(&attr);
        free(t);
        return 1;
    }

    pthread_attr_destroy(&attr);
    printf("main: detached thread started\n");
    sleep(1);
    return 0;
}
