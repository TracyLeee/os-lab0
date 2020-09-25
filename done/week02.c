/**
 * @file week02.c
 * @brief Exercises in C for week 2
 *
 * TODO's:
 * 1. Complete the implementation of w2_bork()
 * 2. Implement w2_fork()
 * 3. Implement w2_clone()
 *
 * @author Ahmad Hazimeh
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "week02.h"

void w2_bork(unsigned int n, void (*verify)(void))
{
    if (n == 0)
        return;

    pid_t cpid = fork();
    if (cpid < 0) {
        fprintf(stderr, "fork(): errno %d %s\n", errno, strerror(errno));
        return;
    }
    if (!cpid) {
        if (verify) (*verify)();
        exit(0);
    } else {
        /* This is only executed by the parent process */
        waitpid(cpid, NULL, 0);
        w2_bork(n-1, verify);
    }
}

void w2_fork(const bool *root, int index, void (*verify)(void))
{
    if (!root[index]) return;

    pid_t cpid = fork();

    if (cpid < 0) {
        fprintf(stderr, "fork(): errno %d %s\n", errno, strerror(errno));
        return;
    } else if (!cpid) {
        if (verify) (*verify)();
        w2_fork(root, 2*index+1, verify);
        exit(0);
    } else {
        waitpid(cpid, NULL, 0);
        w2_fork(root, 2*index+2, verify);
    }
}

void w2_clone(const struct pnode *root, int index, void * (*verify_thread)(void *), void (*verify_fork)(void))
{
    if (!root[index].must_fork) return;

    if (verify_thread) {
        /* Spawn threads */
        int thread_err;
        pthread_t tid[root[index].num_threads];

        for (int i = 0; i < root[index].num_threads; ++i) {
            if ((thread_err = pthread_create(tid+i, NULL, verify_thread, NULL))) {
                fprintf(stderr, "pthread_create(): error number %d\n", thread_err);
                return;
            }
        }

        /* Join threads */
        for (int i = 0; i < root[index].num_threads; ++i) {
            if ((thread_err = pthread_join(tid[i], NULL))) {
                fprintf(stderr, "pthread_join(): error number %d\n", thread_err);
                return;
            }
        }
    }

    pid_t cpid = fork();

    if (cpid < 0) {
        fprintf(stderr, "fork(): errno %d %s\n", errno, strerror(errno));
        return;
    } else if (!cpid) {
        if (verify_fork) (*verify_fork)();
        w2_clone(root, 2*index+1, verify_thread, verify_fork);
        exit(0);
    } else {
        waitpid(cpid, NULL, 0);
        w2_clone(root, 2*index+2, verify_thread, verify_fork);
    }
}
