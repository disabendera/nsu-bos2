#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define THREADS_COUNT 5

int global_var = 100;

struct thread_arg {
    int idx;
    pthread_t tid;
};

void *worker(void *arg) {
    struct thread_arg *a = (struct thread_arg *)arg;

    int local_var = 10 + a->idx;
    static int local_static_var;
    const int local_const_var = 50 + a->idx;

    local_static_var = 30 + a->idx;

    pthread_t self = pthread_self();

    printf("thread[%d]: pid=%d ppid=%d pthread_self=%lu gettid=%d\n",
           a->idx,
           getpid(),
           getppid(),
           (unsigned long)self,
           (int)gettid());

    printf("thread[%d]: created_tid=%lu self=%lu equal=%d\n",
           a->idx,
           (unsigned long)a->tid,
           (unsigned long)self,
           pthread_equal(self, a->tid));

    printf("thread[%d]: addresses:\n", a->idx);
    printf("  local_var         = %p\n", (void *)&local_var);
    printf("  local_static_var  = %p\n", (void *)&local_static_var);
    printf("  local_const_var   = %p\n", (void *)&local_const_var);
    printf("  global_var        = %p\n", (void *)&global_var);

    return NULL;
}

int main(void) {
    struct thread_arg args[THREADS_COUNT];
    int i;
    int err;

    printf("main: pid=%d ppid=%d pthread_self=%lu gettid=%d\n",
           getpid(),
           getppid(),
           (unsigned long)pthread_self(),
           (int)gettid());

    for (i = 0; i < THREADS_COUNT; i++) {
        args[i].idx = i;
        err = pthread_create(&args[i].tid, NULL, worker, &args[i]);
        if (err) {
            fprintf(stderr, "pthread_create failed for thread %d: %s\n", i, strerror(err));
            return 1;
        }
    }

    for (i = 0; i < THREADS_COUNT; i++) {
        err = pthread_join(args[i].tid, NULL);
        if (err) {
            fprintf(stderr, "pthread_join failed for thread %d: %s\n", i, strerror(err));
            return 1;
        }
    }

    printf("main: all threads finished\n");
    return 0;
}