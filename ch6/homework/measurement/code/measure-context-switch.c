#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    int loops = 10000;
    int first_pipe[2];
    int second_pipe[2];
    if (pipe(first_pipe) == -1) {
        printf("first pipe() failed");
        exit(EXIT_FAILURE);
    }
    if (pipe(second_pipe) == -1) {
        printf("second pipe() failed");
        exit(EXIT_FAILURE);
    }

    cpu_set_t set;
    struct timeval start_timeval;
    struct timeval end_timeval;

    int rc = fork();
    if (rc < 0) {
        printf("fork() failed");
        exit(EXIT_FAILURE);
    } else if (rc == 0) { // child process
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
            printf("[child] sched_setaffinity() failed.");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < loops; i++) {
            read(first_pipe[0], NULL, 0);
            write(second_pipe[1], NULL, 0);
        }

    } else { // parent process
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1) {
            printf("[child] sched_setaffinity() failed.");
            exit(EXIT_FAILURE);
        }

        gettimeofday(&start_timeval, NULL);
        for (int i = 0; i < loops; i++) {
            write(first_pipe[1], NULL, 0);
            read(second_pipe[0], NULL, 0);
        }

        gettimeofday(&end_timeval, NULL);

        float elapsed = (end_timeval.tv_sec - start_timeval.tv_sec) * 1000000.0
                    + end_timeval.tv_usec - start_timeval.tv_usec;
        printf("One context switch takes %f ms\n", elapsed / loops);
    }
}
