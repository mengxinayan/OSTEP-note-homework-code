## Chapter 4. Processes

The definition of a process, informally, is quite simple: it is a running program.

### 4.1 The Abstraction: A Process

Process **machine state**: what a program can read or update when it is running. It including:
- *memory*. Instructions lie in memory. Thus the memory that the
process can address (called its **address space**) is part of the process.
- *register*. Many instructions explicitly read or update registers and thus clearly they are important to the execution of the process. Note that there are some particularly special registers that form part of this machine state.  For example, the **program counter (PC)** (sometimes called the instruction pointer or **IP**) tells us which instruction of the program will execute next; similarly a **stack pointer** and associated **frame pointer** are used to manage the stack for function parameters, local variables, and return addresses.
- *I/O information*. include a list of the files the process currently has open.

### 4.2 Process API

- **Create**. Create new processes.
- **Destroy**. Destroy processes forcefully. exit when complete, kill them
- **Wait**. Wait for a process to stop running.
- **Miscellaneous Control**. Other controls. For example, suspend or resume a process.
- **Status**. Get some status information about a process.

### 4.3 Process Creation: A Little More Detail

1. **Load** its code and any static data into memory, into the address space of the process. In early OS, it will done eagerly; while modern OSes perform the process lazily (based on the machinery of **paging** and **swapping**).
2. Some memory must be allocated for the program's **run-time stack**. (In C, it use stack for local variables, function parameters, and return addresses). The OS may also allocate some memory for the program’s **heap**. In C, the heap is used for explicitly requested dynamically-allocated data, such as space by calling `malloc()` and free it explicitly by calling `free()`.
3. The OS will also do some other initialization tasks, particularly as re-
lated to input/output (I/O). For example, in UNIX system, each process by default has three open file descriptors, for standard input, output, and error;
4. It thus has one last task: to start the program running at the entry point, namely `main()`. The OS transfers control of the CPU to the newly-created process, and thus the program begins its execution.

### 4.4 Process States

- **Running**: In the running state, a process is running on a processor. This means it is executing instructions.
- **Ready**: In the ready state, a process is ready to run but for some reason the OS has chosen not to run it at this given moment
- **Blocked**: In the blocked state, a process has performed some kind of operation that makes it not ready to run until some other event takes place. A common example: when a process initiates an I/O request to a disk, it becomes blocked and thus some other process can use the processor.

![Process: State transitions](./fig/4-2.png)

Sometimes a system will have an **initial state** that the process is in when it is being created. Also, a process could be placed in a **final state** where it has exited but has not yet been cleaned up (in UNIX-based systems, this is called the
zombie state1).

This final state can be useful as it allows other processes (usually the parent that created the process) to examine the return code of the process and see if the just-finished process executed successfully (usually, programs return zero in UNIX-based systems when they have accomplished a task successfully, and non-zero otherwise). When finished, the parent will make one final call (e.g., wait()) to wait for the completion of the child, and to also indicate to the OS that it can clean up any relevant data structures that referred to the now-extinct process.

### 4.5 Data Structures

- **Process List**. To track the state of each process.
- **Register Context**. hold for a stopped process, contents of its registers. Technique known as a **context switch**.

### 4.6 Summary

- The **process** is the major OS abstraction of a running program. At any point in time, the process can be described by its state: the contents of memory in its **address space**, the contents of CPU registers (including the **program counter** and **stack pointer**, among others), and information about I/O (such as open files which can be read or written).
- The **process API** consists of calls programs can make related to processes. Typically, this includes creation, destruction, and other useful calls.
- Processes exist in one of many different **process states**, including running, ready to run, and blocked. Different events (e.g., getting scheduled or descheduled, or waiting for an I/O to complete) transition a process from one of these states to the other.
- A **process list** contains information about all processes in the system. Each entry is found in what is sometimes called a **process control block (PCB)**, which is really just a structure that contains information about a specific process.

## Chapter 5. Process API

UNIX presents one of the most intriguing ways to create a new process with a pair of system calls: `fork()` and `exec()`. A third routine, `wait()`, can be used by a process wishing to wait for a process it has created to complete.

### 5.1 The `fork()` System Call

Each process has its **process identifier**, also known as a **PID**. The child process has its own copy of the address space, registers, PC, and so forth. 

The `fork()` system call is used to create a new process. The value it returns to the caller of `fork()` is different. Specifically, while the parent receives the PID of the newly-created child, the child receives a return code of zero.

Assuming run [Example p1](./code/ch5/sample/p1.c) on a system with a single CPU, the order of message printed by parent and child process cannot be determined. Because the **CPU scheduler**, it will leads to some interesting problems, particularly in **multi-threaded programs**.

### 5.2 The `wait()` System Call

Sometimes, it is useful for a parent to wait for a child process process to finish what it has been doing. This task is accomplished with the `wait()` system call (or its more complete sibling `waitpid()`);

In [Example p2](./code/ch5/sample/p2.c), the parent process calls `wait()` to delay its execution until the child finishes executing. So child will print message first, then parent print its message.

### 5.3 Finally, The `exec()` System Call

When you want to run a *different* program, `exec()` system call can do that. `exec()` loads code and static data from that executable and overwrites its current code segment (and current static data) with it; the heap and stack and other parts of the memory space of the program are re-initialized.

[Example p3](./code/ch5/sample/p3.c) will run the program `wc` on source file `p3.c`.

### 5.4 Why? Motivating the API

The separation of `fork()` and `exec()` is essential in building a UNIX shell, because it lets the shell run code after the call to `fork()` but before the call to `exec()`.

In most cases, the shell then figures out where in the file system the executable resides, calls `fork()` to create a new child process to run the command, calls some variant of `exec()` to run the command, and then waits for the command to complete by calling `wait()`. 

[Example p4](./code/ch5/sample/p4.c) shows a program that does redirection like `wc p4.c > newfile.txt`

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
