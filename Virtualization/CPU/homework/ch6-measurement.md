# ch6 Direct Execution Homework and Solution (Measurement)

If you just want to read the answers, that is a simple version for solutions, please visit [here](../solution.md).

Back to [contents](./README) for other chapter solution.

## Homework (Measurement)

In this homework, you’ll measure the costs of a system call and context switch. Measuring the cost of a system call is relatively easy. For example, you could repeatedly call a simple system call (e.g., performing a 0-byte read), and time how long it takes; dividing the time by the number of iterations gives you an estimate of the cost of a system call.

One thing you’ll have to take into account is the precision and accuracy of your timer. A typical timer that you can use is `gettimeofday()`; read the man page for details. What you’ll see there is that `gettimeofday()` returns the time in microseconds since 1970; however, this does not mean that the timer is precise to the microsecond. Measure back-to-back calls to `gettimeofday()` to learn something about how precise the timer really is; this will tell you how many iterations of your null system-call test you’ll have to run in order to get a good measurement result. If `gettimeofday()` is not precise enough for you, you might look into using the `rdtsc` instruction available on x86 machines.

Measuring the cost of a context switch is a little trickier. The lmbench benchmark does so by running two processes on a single CPU, and setting up two UNIX pipes between them; a pipe is just one of many ways processes in a UNIX system can communicate with one another. The first process then issues a write to the first pipe, and waits for a read on the second; upon seeing the first process waiting for something to read from the second pipe, the OS puts the first process in the blocked state, and switches to the other process, which reads from the first pipe and then writes to the second. When the second process tries to read from the first pipe again, it blocks, and thus the back-and-forth cycle of communication continues. By measuring the cost of communicating like this repeatedly, lmbench can make a good estimate of the cost of a context switch. You can try to re-create something similar here, using pipes, or perhaps some other communication mechanism such as UNIX sockets.

One difficulty in measuring context-switch cost arises in systems with more than one CPU; what you need to do on such a system is ensure that your context-switching processes are located on the same processor. Fortunately, most operating systems have calls to bind a process to a particular processor; on Linux, for example, the `sched setaffinity()` call is what you’re looking for. By ensuring both processes are on the same processor, you are making sure to measure the cost of the OS stopping one process and restoring another on the same CPU.

## Solutions

We can use `gettimeofday()` to measure time. By calculating the difference as the elapsed time. 

```c
struct timeval start_timeval;
struct timeval end_timeval;

gettimeofday(&start_timeval, NULL);
// execution code.
gettimeofday(&end_timeval, NULL);

float elapsed = (end_timeval.tv_sec - start_timeval.tv_sec) * 1000000.0
                  + end_timeval.tv_usec - start_timeval.tv_usec
```

### Measure the costs of a system call

We can repeatedly call a simple system call (e.g. performing a 0-byte read()), and time how long it takes; dividing the time by the number of iterations gives you an estimate of the cost of a system call.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    int loops = 1000;
    int fd = open("tmp.txt", O_RDONLY);
    if (fd == -1) {
        printf("open() failed\n");
    }

    struct timeval start_timeval;
    struct timeval end_timeval;
    
    gettimeofday(&start_timeval, NULL);
    for (int i = 0; i < loops; i++) {
        read(fd, NULL, 0);
    }
    gettimeofday(&end_timeval, NULL);

    float elapsed = (end_timeval.tv_sec - start_timeval.tv_sec) * 1000000.0
                    + end_timeval.tv_usec - start_timeval.tv_usec;
    printf("One read() system call takes %f ms\n", elapsed / loops);

    return 0;
}
```

The following is my execution result.

```console
$ ./measure-system-call
One read() system call takes 2.179000 ms.
```

### Measure the  of context switch

First, we should use `sched_setaffinity()` call to bind a process to a particular processor. Then running two processes on the same CPU, The first process then issues a write to the first pipe, and waits for a read on the second; upon seeing the first process waiting for something to read from the second pipe, the OS puts the first process in the blocked state, and switches to the other process, which reads from the first pipe and then writes to the second. When the second process tries to read from the first pipe again, it blocks, and thus the back-and-forth cycle of communication continues. By measuring the cost of communicating like this repeatedly.

```c
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

```

The following is my execution result.

```console
$ ./measure-context-switch 
One context switch system call takes 6.921300 ms
```
 
