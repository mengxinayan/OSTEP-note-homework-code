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

### 5.3 Finally, The `exec()` System Call

### 5.4 Why? Motivating the API

### 5.5 Process Control And Users

### 5.6 Useful Tools

### 5.7 Summary
