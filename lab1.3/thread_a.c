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
    return NULL;
}

int main(void) {
    pthread_t tid;
    struct task t;
    int err;

    t.value = 123;
    t.text = "hello from struct";

    err = pthread_create(&tid, NULL, worker, &t);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, NULL);
    if (err) {
        fprintf(stderr, "pthread_join() failed: %s\n", strerror(err));
        return 1;
    }

    printf("main: finished\n");
    return 0;
}
