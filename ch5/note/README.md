
## Chapter 5. Process API

UNIX presents one of the most intriguing ways to create a new process with a pair of system calls: `fork()` and `exec()`. A third routine, `wait()`, can be used by a process wishing to wait for a process it has created to complete.

### 5.1 The `fork()` System Call

Each process has its **process identifier**, also known as a **PID**. The child process has its own copy of the address space, registers, PC, and so forth. 

The `fork()` system call is used to create a new process. The value it returns to the caller of `fork()` is different. Specifically, while the parent receives the PID of the newly-created child, the child receives a return code of zero.

Assuming run [Example p1](./sample_code/p1.c) on a system with a single CPU, the order of message printed by parent and child process cannot be determined. Because the **CPU scheduler**, it will leads to some interesting problems, particularly in **multi-threaded programs**.

### 5.2 The `wait()` System Call

Sometimes, it is useful for a parent to wait for a child process process to finish what it has been doing. This task is accomplished with the `wait()` system call (or its more complete sibling `waitpid()`);

In [Example p2](./sample_code/p2.c), the parent process calls `wait()` to delay its execution until the child finishes executing. So child will print message first, then parent print its message.

### 5.3 Finally, The `exec()` System Call

When you want to run a *different* program, `exec()` system call can do that. `exec()` loads code and static data from that executable and overwrites its current code segment (and current static data) with it; the heap and stack and other parts of the memory space of the program are re-initialized.

[Example p3](./sample_code/p3.c) will run the program `wc` on source file `p3.c`.

### 5.4 Why? Motivating the API

The separation of `fork()` and `exec()` is essential in building a UNIX shell, because it lets the shell run code after the call to `fork()` but before the call to `exec()`.

In most cases, the shell then figures out where in the file system the executable resides, calls `fork()` to create a new child process to run the command, calls some variant of `exec()` to run the command, and then waits for the command to complete by calling `wait()`. 

[Example p4](./sample_code/p4.c) shows a program that does redirection like `wc p4.c > newfile.txt`

UNIX pipes are implemented in a similar way, but with the `pipe()` system call. In this case, the output of one process is connected to an in-kernel pipe (i.e., queue), and the input of another process is connected to that same pipe;

### 5.5 Process Control And Users

There are a lot of other interfaces for interacting with processes in UNIX systems. For example, the `kill()` system call is used to send signals to a process, including directives to pause, die, and other useful imperatives. For example, Ctrl-c sends a `SIGINT`(interrupt) to process, Ctrl-z sends a `SIGTSTP`(stop) signnal to pause the process.

Modern systems include a strong conception of the notion of a user. Not everyone on system can send any signal.

### 5.6 Useful Tools

There are many command-line tools that are useful as well. For example, `man` can help you read **man pages**. `ps` allows you to see which processes are running. `top` displays the processes of the system and how much CPU and other resources they are eating up. `kill` can send arbitrary signals to processes.

### 5.7 Summary

- Each process has a name; in most systems, that name is a number known as a **process ID (PID)**.
- The `fork()` system call is used in UNIX systems to create a new process. The creator is called the **parent**; the newly created process is called the **child**. As sometimes occurs in real life [J16], the child process is a nearly identical copy of the parent.
- The `wait()` system call allows a parent to wait for its child to complete execution.
- The `exec()` family of system calls allows a child to break free from its similarity to its parent and execute an entirely new program.
- A UNIX shell commonly uses `fork()`, `wait()`, and `exec()` to launch user commands; the separation of fork and exec enables features like **input/output redirection**, **pipes**, and other cool features, all without changing anything about the programs being run.
- Process control is available in the form of **signals**, which can cause jobs to stop, continue, or even terminate.
- Which processes can be controlled by a particular person is encapsulated in the notion of a **user**; the operating system allows multiple users onto the system, and ensures users can only control their own processes.
- A **superuser** can control all processes (and indeed do many other things); this role should be assumed infrequently and with caution for security reasons.
