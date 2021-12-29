# Virtualizing the CPU Homework Solution

This solution document only contains my answer without questions and code running result. If you want to get a more detailed documents, please visit [here](./homework/README.md).

## Chapter 4 (Simulation)

In this chapter homework, we should run [`process-run.py`](./code/ch4/homework-simulation/process-run.py) simulator. Before run it, you should read its [README](./code/ch4/homework-simulation/README.md).

[Chapter 4 detailed solution](./homework/ch4-simulation.md)

### 4.1

Process 0: 50%; Process 1: 50%; Total : 100%

### 4.2

Time is 11 = 4(RUN:cpu) + 1(RUN:io) + 5(WAITING) + 1(RUN:io_done)

When process 0 finishes, then process 1 will start. And IO takes 5 tricks(default).

### 4.3

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done)

Because process 1 will use CPU when process 0 waiting I/O.

### 4.4

Time will be 11 = 1(RUN:io) + 5(WAITING) + 1(RUN:io_done) + 4(RUN:cpu)

Because the system will NOT switch to another process while one is doing I/O, it will start process 1 when process 0 ends.

### 4.5

Time is 7 = 1(RUN:io) + 5(WAITING && RUN:cpu) + 1(RUN:io_done)

Because the system will switch another process while one is doing I/O, so process 1 will run during process doing I/O.

### 4.6

Time is 31.

When process 0 I/O done, the system will not start next I/O immediately.

No, Because process 0 need wait to start next I/O.

### 4.7

Time is 21. Time will be less compared with `IO_RUN_LATER`.

This will improve time utilization. Because one process can start I/O immediately, while run another process can run instructions.

### 4.8

`IO_RUN_IMMEDIATE`: take less time
`IO_RUN_LATER`: take more time

`SWITCH_ON_IO`: take less time
`SWITCH_ON_END`: take more time

## Chapter 5 (Simulation)

In this chapter simulation homework, we should run [`fork.py`](./code/ch5/homework-simulation/fork.py) simulator. Before run it, you should read its [README](./code/ch5/homework-simulation/README.md)). Note that this simulator will generate randomly, so its answer is **not unique**.

[Chapter 5 (simulation) detailed solution](./homework/ch5-simulation.md)

### 5.1

```
                           Process Tree:
                               a

Action: a forks b
Process Tree?                   a
                                |──b

Action: a forks c
Process Tree?                   a
                                |──b
                                |──c

Action: c EXITS
Process Tree?
                                a
                                |──b

Action: a forks d
Process Tree?
                                a
                                |──b
                                |──d

Action: a forks e
Process Tree?
                                a
                                |──b
                                |──d
                                |──e
```

### 5.2

The `fork_percentage` is more larger, there will more processes, the process tree will be more complex.

### 5.3

```
                           Process Tree:
                               a

Action: a forks b
                               a
                               └── b
Action: b EXITS
                               a
Action: a forks c
                               a
                               └── c
Action: a forks d
                               a
                               ├── c
                               └── d
Action: a forks e
                               a
                               ├── c
                               ├── d
                               └── e

```

### 5.4

Without `-R` flag, when a process's parent exits, its parent will change to initial process.

```
a
|──b
|──d
|──e
```

With `-R` flag, when a process's parent exits, its parent will change to its grandfather process.

```
a
|──b
   |──d
   |──e
```

### 5.5

```
                           Process Tree:
                               a

Action: a forks b
Action: b forks c
Action: b forks d
Action: c forks e
Action: b forks f

                        Final Process Tree:
                                a
                                |──b
                                   |──c
                                      |──e
                                   |──d
                                   |──f
```

### 5.6

```
                           Process Tree:
                               a

Action: a forks b
Action: ? forks c (Can't determine who forks c)
Action: a forks d
Action: b forks e
Action: C EXITS ?(Can't determine when C exit)

                        Final Process Tree:
                               a
                               ├── b
                               │   └── e
                               └── d

```

## Chapter 5 (Code)

[Chapter 5 (code) detailed solution](./homework/ch5-code.md)

### 5.1 

`x = 100` in child process. Because child and parent process have their own `x` variable, child process changes to `x` value **do not** affect the value of `x` in the parent process. 

[code](./code/ch5/homework-code/hw1.c)

### 5.2

Yes, both parent and child process can use this file descriptor. All messages will be written in the file, but the sequence of them are not sure. 

[code](./code/ch5/homework-code/hw2.c)

### 5.3

Use `sleep()` before print 'goodbye' in parent process. \

[code](./code/ch5/homework-code/hw3.c)

### 5.4

To fit various demand. The differences between `exec()` family system call are as follow.

`execl*()` V.S. `execv*()`: `execl*()` requires a list of arguments while `execv*()` requires a vector of arguments. Specifically, `execl*()` needs `char *arg1, char *arg2, ..., char *argX`, `execv*()` needs `char *argv[]`. Note that for `execv*()`, the array  of  pointers  **must**  be  terminated  by  a  null pointer.

For suffix `p`, such as `execvp()`, `execlp()`. `p` means it will the `PATH` environment variable to find the program. For example, if you want to use `ls` command, with suffix `p`, use `ls` directly can work well, however, you should use `/bin/ls` rather than `ls` without the suffix `p`.

For suffix `e`, such as `execle()`, `execvpe()`. `e` allows you to specify the environment for the process.

[code](./code/ch5/homework-code/hw4.c)

### 5.5

`wait()` returns child process identifier(PID). If child process call `wait()`, it will return `-1`. 

[code](./code/ch5/homework-code/hw5.c)

### 5.6

The `wait()` system call suspends execution of the calling thread until one of its children terminates. The call `wait(&wstatus)` is equivalent to: `waitpid(-1, &wstatus, 0)`. The `waitpid()` system call suspends execution of the calling thread until a child specified by  pid  argument  has  changed state.  By default, `waitpid()` waits only for terminated children, but this behavior is modifiable via the options argument, as described below.

[code](./code/ch5/homework-code/hw6.c)

### 5.7

After closing stdout file descriptor, it can't print any message by calling `printf()`.

[code](./code/ch5/homework-code/hw7.c)

### 5.8

[code](./code/ch5/homework-code/hw8.c)

## Chapter 6 (Measurement)

[Chapter 6 (measurement) detailed solution](./homework/ch6-measurement.md)

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

Here is [code](,/code/ch6/homework-measurement/measure-system-call.c). The following is my execution result.

```console
$ ./measure-system-call
One read() system call takes 2.179000 ms.
```

### Measure the  of context switch

First, we should use `sched_setaffinity()` call to bind a process to a particular processor. Then running two processes on the same CPU, The first process then issues a write to the first pipe, and waits for a read on the second; upon seeing the first process waiting for something to read from the second pipe, the OS puts the first process in the blocked state, and switches to the other process, which reads from the first pipe and then writes to the second. When the second process tries to read from the first pipe again, it blocks, and thus the back-and-forth cycle of communication continues. By measuring the cost of communicating like this repeatedly.

Here is [code](,/code/ch6/homework-measurement/measure-context-switch.c). The following is my execution result.

```console
$ ./measure-context-switch 
One context switch system call takes 6.921300 ms
```

## Chapter 7 (Simulator)

In this chapter simulation homework, we should run [`scheduler.py`](./code/ch7/homework-simulation/scheduler.py) simulator. Before run it, you should read its [README](./code/ch7/homework-simulation/README.md).

[Chapter 7 (simulation) detailed solution](./homework/ch7-simulation.md)

### 7.1

For FIFO:

Turnaround time: 200, 400, 600. Average turnaround time is 400.

Response time: 0, 200, 400. Average turn around time is 200.

For SJF:

Turnaround time: 200, 400, 600. Average turnaround time is 400.

Response time: 0, 200, 400. Average turn around time is 200.

### 7.2

For FIFO:

Turnaround time: 100, 300, 600. Average turnaround time is 333.33.

Response time: 0, 100, 300. Average turn around time is 133.33.

For SJF:

Turnaround time: 100, 300, 600. Average turnaround time is 333.33.

Response time: 0, 100, 300. Average turn around time is 133.33.

### 7.3

For RR with time-slice of 1:

Turnaround time: 298, 499, 600. Average turnaround time is 465.67.

Response time: 0, 1, 2. Average turn around time is 1.

### 7.4

The length of arrival jobs is from short to long.

### 7.5

Each job of length is same and quantum length is equals to this length.

### 7.6

Response time will increase too.

```console
$ python3 scheduler.py -p SJF -l 100,100,100 -c
$ python3 scheduler.py -p SJF -l 200,200,200 -c
$ python3 scheduler.py -p SJF -l 300,300,300 -c
```

### 7.7

Response time will increase too.

Assume quantum length is `t`, so the Nth job response time is `(N-1)*t`, the average response time is `(N-1)*t/2`.

## Chapter 8 (Simulator)

In this chapter simulation homework, we should run [`mlfq.py`](./code/ch8/homework-simulation/mlfq.py) simulator. Before run it, you should read its [README](./code/ch8/homework-simulation/README.md).

[Chapter 8 (simulation) detailed solution](./homework/ch8-simulation.md)

### 8.1

The parameters are as follows:

```console
$ python3 mlfq.py -n 2 -j 2 -l 0,20,0:20,30,0
```

Running result is as follows.

```
Time 0~10: job1(queue 1), then down in queue 2
Time 10~20: job2(queue 1), then down in queue 2
Time 20~30: job1(queue 2), finished at 30
Time 30~50: job2(queue 2), finished at 50
```

### 8.2

Example 1 (Figure 8.2 Long-running Job Over Time)

```console
$ python3 mlfq.py -n 3 -j 1 -l 0,200,0 -c
```

Example 2 (Figure 8.3: Along Came An Interactive Job)

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,180,0:100,20,0 -c
```

Example 3 (Figure 8.4: A Mixed I/O-intensive and CPU-intensive Workload)

Use `-i` flag to set IOTIME.

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,175,0:50,25,1 -i 4 -c
```

Example 4 (Figure 8.5(Left): Without Priority Boost)

Use `-i` flag to set IOTIME without `-B` flag.

```console
$ python3 mlfq.py -n 3 -j 3 -l 0,200,0:100,20,2:100,20,2 -i 2 -c
```

Example 4 (Figure 8.5(right): With Priority Boost)

Using `-B` flag to set how often to boost the priority of all jobs. The parameters are as follows.

```console
$ python3 mlfq.py -n 3 -j 3 -l 0,200,0:100,20,2:100,20,2 -i 2 -B 110 -c
```

Example 5 (Figure 8.6(Left): Without Gaming Tolerance)

Using `-S` flag to reset and stay at same priority level when issuing I/O. The parameters are as follows.

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,200,0:50,45,9 -i 0 -S -c
```

Example 5 (Figure 8.6(Right): With Gaming Tolerance)

Without `-S` flag. The parameters are as follows.

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,200,0:50,45,9 -i 0 -c
```

Example 6 (Figure 8.7: Lower Priority, Longer Quanta)

Using `-Q a,b,c` to specify length of time slice per queue level. The parameters are as follows.

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,150,0:10,150,0 -Q 10,20,30 -c
```

### 8.3

There is only one queue (by `-n 1`), and setting time slice <= (max job length) / (the number of jobs).

```console
$ python3 mlfq.py -n 1 -j 3 -l 0,50,0:0,50,0:0,50,0 -Q 1 -c 
```

### 8.4

This question just like Figure 8.6 (Right) without gaming tolerance. So its solution is the same as example 5 in 8.2.

```console
$ python3 mlfq.py -n 3 -j 2 -l 0,200,0:50,45,9 -i 0 -S -c
```

### 8.5

200ms. Because the problem requires a job to occupy at least 5% of the CPU, and the highest queue time slice is 10ms, so at least every 10 / 5% = 200ms to boost job back to the highest priority.

### 8.6

Use `-I` flag will pause current running job, start such job that just finished I/O.

Here is an example without `-I`

```console
$ python3 mlfq.py -n 2 -j 2 -l 0,100,0:0,50,5 -i 2 -c
```

## Chapter 9 (Simulator)

In this chapter simulation homework, we should run [`lottery.py`](./code/ch9/homework-simulation/lottery.py) simulator. Before run it, you should read its [README](./code/ch9/homework-simulation/README.md).

[Chapter 9 (simulation) detailed solution](./homework/ch9-simulation.md)

### 9.1

For seed is equals 1, the results are as follows. 

```console
$ python3 lottery.py -s 1 -j 3
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 1

Here is the job list, with the run time of each job: 
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )


Here is the set of random numbers you will need (at most):
Random 651593   (* Job 2)
Random 788724   (* Job 0, Job 0 done at time 2)
Random 93859    (* Job 1)
Random 28347    (* Job 2)
Random 835765   (* Job 2)
Random 432767   (* Job 2, Job 2 done at time 6)
Random 762280   (* Job 1)
Random 2106     (* Job 1)
Random 445387   (* Job 1)
Random 721540   (* Job 1)
Random 228762   (* Job 1)
Random 945271   (* Job 1, Job 1 done at time 6)
```

You can see detailed process by running:

```console
$ python3 lottery.py -s 1 -j 3 -c
```

For seed is equals 2, the results are as follows. 

```console
$ python3 lottery.py -s 2 -j 3
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 2

Here is the job list, with the run time of each job: 
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )


Here is the set of random numbers you will need (at most):
Random 605944   (* Job 2)
Random 606802   (* Job 0)
Random 581204   (* Job 0)
Random 158383   (* Job 2)
Random 430670   (* Job 0)
Random 393532   (* Job 1)
Random 723012   (* Job 0)
Random 994820   (* Job 2)
Random 949396   (* Job 0)
Random 544177   (* Job 0)
Random 444854   (* Job 0)
Random 268241   (* Job 1)
Random 35924    (* Job 0)
Random 27444    (* Job 0, Job 0 done at time 14)
Random 464894   (* Job 1)
Random 318465   (* Job 1)
Random 380015   (* Job 2)
Random 891790   (* Job 1)
Random 525753   (* Job 1)
Random 560510   (* Job 2)
Random 236123   (* Job 1)
Random 23858    (* Job 1, Job 1 done at time 22)
Random 325143   (* Job 2, Job 2 done at time 23)
```

You can see detailed process by running:

```console
$ python3 lottery.py -s 2 -j 3 -c
```

For seed is equals 3, the results are as follows. 

```console
$ python3 lottery.py -s 3 -j 3
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 3

Here is the job list, with the run time of each job: 
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )


Here is the set of random numbers you will need (at most):
Random 13168    (* Job 1)
Random 837469   (* Job 1)
Random 259354   (* Job 0)
Random 234331   (* Job 1, Job 1 done at time 4)
Random 995645   (* Job 0, Job 0 done at time 5)
Random 470263   (* Job 2)
Random 836462   (* Job 2)
Random 476353   (* Job 2)
Random 639068   (* Job 2)
Random 150616   (* Job 2)
Random 634861   (* Job 2, Job 2 done at time 11)
```

You can see detailed process by running:

```console
$ python3 lottery.py -s 3 -j 3 -c
```

### 9.2

Job 1 will run more frequently.

It is possible that Job 0 will be completed before Job 1, the probability is about 9.5%. Because if you want job 1 to complete before job 0 is run, job 1 needs to be run for the first 10 times, and its probability is (100/101)^10. Therefore, the probability of job 1 running before job 0 is completed is 1-(100/101)^10 is approximately equal to 9.5%.

### 9.3

You should run `python3 lottery.py -s 1 -l 100:100,100:100 -c`

When the random seed is equal to 1, F = 196/200 = 0.98
When the random seed is equal to 2, F = 190/200 = 0.95
When the random seed is equal to 3, F = 196/200 = 0.98
When the random seed is equal to 4, F = 199/200 = 0.995
When the random seed is equal to 5, F = 181/200 = 0.905

### 9.4

Generally, fairness will gradually decrease with the increase of quantum, but with the increase of quantum. The change of fairness in a certain quantum range will be more stable, because the total number of time slices required within a certain quantum range is the same. For example, when the quantum range is 50 to 65, two jobs can be completed by four runs.

The graph of fairness changes with quantum is as follows ([code](../code/ch9/homework-simulation/9-4-quantum.py)):

![Figure 9.4 Fairness with Quantum](../fig/ch9/9-4-quantum.png)

### 9.5

For job length, the graph of fairness changes with job length is as follows ([code](../code/ch9/homework-simulation/9-5-length.py)):

![Figure 9.5 Fairness with Job Length](../fig/ch9/9-5-length.png)

For stride scheduler, the graph of fairness changes with th ratio of job 1 tickets in total tickets is as follows ([code](../code/ch9/homework-simulation/9-5-stride.py)):

![Figure 9.5 Fairness with Job Length](../fig/ch9/9-5-stride.png)
