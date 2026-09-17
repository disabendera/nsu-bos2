#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static void sigint_handler(int signo) {
    const char msg[] = "thread 2: caught SIGINT via handler\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void *thread_block_all(void *arg) {
    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    
    printf("thread 1: blocked all signals\n");
    while (1) { sleep(1); }
    return NULL;
}

void *thread_sigint_handler(void *arg) {
    sigset_t mask;
    
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

    printf("thread 2: unblocked SIGINT, waiting via handler\n");
    while (1) { sleep(1); }
    return NULL;
}

void *thread_sigquit_wait(void *arg) {
    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);

    printf("thread 3: waiting for SIGQUIT via sigwait\n");
    while (1) {
        if (sigwait(&set, &sig) == 0 && sig == SIGQUIT) {
            printf("thread 3: caught SIGQUIT via sigwait\n");
            break;
        }
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    pthread_create(&t1, NULL, thread_block_all, NULL);
    pthread_create(&t2, NULL, thread_sigint_handler, NULL);
    pthread_create(&t3, NULL, thread_sigquit_wait, NULL);

    sleep(1);
    
    printf("main: sending SIGINT to thread 2\n");
    pthread_kill(t2, SIGINT);

    sleep(1);
    printf("main: sending SIGQUIT to thread 3\n");
    pthread_kill(t3, SIGQUIT);

    pthread_join(t3, NULL);
    printf("main: thread 3 finished, exiting program\n");

    return 0;
}