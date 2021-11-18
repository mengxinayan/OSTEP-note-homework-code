# Virtualizing the CPU Homework Solution

This solution document only contains my answer without questions and code running result. If you want to get a more detailed documents, please visit [here](./homework/README.md).

## Chapter 4 (Simulation)

In this chapter homework, we should run `process-run.py` simulator([here](./code/ch4/homework-simulation/process-run.py)). Before run it, you should read its README([here](./code/ch4/homework-simulation/README.md)).

[Chapter 4 detailed solution document](./homework/ch4.md)

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

In this chapter simulation homework, we should run `fork.py` simulator([here](./code/ch5/homework-simulation/fork.py)). Before run it, you should read its README([here](./code/ch4/homework-simulation/README.md)). Note that this simulator will generate randomly, so its answer is **not unique**.

[Chapter 5 (simulation) detailed solution document](./homework/ch5-simulation.md)

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

[Chapter 5 (code) detailed solution document](./homework/ch5-code.md)

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
