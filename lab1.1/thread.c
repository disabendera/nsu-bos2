#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void *mythread(void *arg) {
    printf("mythread: pid=%d ppid=%d tid=%d\n",
           getpid(), getppid(), gettid());
    return NULL;
}

int main(void) {
    pthread_t tid;
    int err;

    printf("main: pid=%d ppid=%d tid=%d\n",
           getpid(), getppid(), gettid());

    err = pthread_create(&tid, NULL, mythread, NULL);
    if (err) {
        fprintf(stderr, "pthread_create() failed: %s\n", strerror(err));
        return 1;
    }

    err = pthread_join(tid, NULL);
    if (err) {
        fprintf(stderr, "pthread_join() failed: %s\n", strerror(err));
        return 1;
    }

    printf("main: thread finished\n");
    return 0;
}