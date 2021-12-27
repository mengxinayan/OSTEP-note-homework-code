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

![Process: State transitions](./fig/ch4/4-2.png)

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

## Chapter 6. Direct Scheduling

Time sharing(run one process for a while, then run another one) has a few challenges:
obtain high *performance* and maintain *control*.

### 6.1 Basic Technique: Limited Direct Execution

**Limited Direct Execution**: "direct execution without limits" means just run the program directly on the CPU. But it will brings some problems.

### 6.2 Problem #1: Restricted Operations

A process must be able to perform I/O and some other restricted operations, but without giving the process complete control over the system.

The OS introduces a new processor mode to solve it.
- **user mode**: code that runs in user mode is restricted in what it can do. For example, it can't issue I/O requests.
- **kernel mode**: the operating system (or kernel) runs in this mode. In this mode, code that runs can do what it likes.

**system calls** allow the kernel to carefully expose certain key pieces of functionality to user programs, such as accessing the file system, creating and destroying processes, communicating with other processes.

To execute a system call, a program must execute a special **trap** instruction. This instruction simultaneously jumps into the kernel and raises the privilege level to kernel mode. once in the kernel, the system can now perform whatever privileged operations are needed (if allowed), and thus do the required work for the calling process. When finished, the OS calls a special **return-from-trap** instruction, which, as you might expect, returns into the calling user program while simultaneously reducing the privilege level back to user mode.

The hardware needs to be a bit careful when executing a trap, in that it
must make sure to save enough of the caller’s registers in order to be able
to return correctly when the OS issues the **return-from-trap** instruction.

The kernel must carefully control what code executes upon a trap, so the kernel does so by setting up a **trap table** at boot time. When the machine boots up, it does so in privileged (kernel) mode, and thus is free to configure machine hardware as need be. The OS informs the hardware of the locations of these **trap handlers**.

To specify the exact system call, a **system-call number** is usually assigned to each system call. The user code is thus responsible for placing the desired system-call number in a register or at a specified location on the stack; the OS, when handling the system call inside the trap handler, examines this number, ensures it is valid, and, if it is, executes the corresponding code. This level of indirection serves as a form of **protection**; user code cannot specify an exact address to jump to, but rather must   request a particular service via number.

One last aside: being able to execute the instruction to tell the hardware where the trap tables are is a very powerful capability. Thus, as you might have guessed, it is also a privileged operation.

There are two phases in the limited direct execution (LDE) protocol.
1. In the first(at boot time), the kernel initializes the trap table, and the CPU remembers its location for subsequent use. The kernel does so via a privileged instruction.
2. In the second(when running a process), rge kernel sets up a few things before using a return-form-trap to start the execution of the process; this switches the CPU to user mode and begins running the process. When the process wishes to issue a system call, it traps back into the OS, which handles it and once again returns control via a return-from-trap to the process. 

### 6.3 Problem #2: Switching Between Processes

The next problem with direct execution is achieving a switch between
processes. It actually is a little bit tricky: specifically, if a process is
running on the CPU, this by definition means the OS is not running. How can the operating system regain control of the CPU so that it can switch between processes?

#### A Cooperative Approach: Wait For System Calls

In this style, the OS *trusts* the processes of the system to behave reasonably. Systems like this often include an explicit **yield** system call, which does nothing except to transfer control to the OS so it can run other processes.

Applications also transfer control to the OS when they do something illegal. For example, an application divides by zero, or tries to access memory that it shouldn't be able to access, it will generate a **trap** to the OS.

In a cooperative scheduling system, the OS regains control of the CPU by waiting for a system call or an illegal operation of some kind to take place.

#### A Non-Cooperative Approach: The OS Takes Control

The OS regained control of the CPU when raising **an timer interrupt** by timer device, and thus can do what it pleases: stop the current process, and start a different one.

First, the OS must inform the hardware of which code to run when the timer interrupt occurs; Second, the OS must start the timer during the boot sequence.

Note that the hardware has some responsibility when an interrupt occurs, in particular to save enough of the state of the program that was running when the interrupt occurred such that a subsequent return-from-trap instruction will be able to resume the running program correctly.

#### Saving and Restoring Context

A **scheduler** is a part of the operating system, to decide whether to continue running the currently-running process, or switch to a different one. If the decision is made to switch, the OS then executes a low-level piece of code which we refer to as a **context switch**. A context switch is conceptually simple: all the OS has to do is save a few register values for the currently-executing process (onto its kernel stack, for example) and restore a few for the soon-to-be-executing process (from its kernel
stack). 

Note that there are two types of register saves/restores that happen during this protocol. The first is when the timer interrupt occurs; in this case, the user registers of the running process are implicitly saved by the hardware, using the kernel stack of that process. The second is when the OS decides to switch from A to B; in this case, the kernel registers are explicitly saved by the software (i.e., the OS), but this time into memory in the process structure of the process.

### 6.4 Worried About Concurrency?

Operating systems also have developed a number of sophisticated **locking** schemes to protect concurrent access to internal data structures. As we’ll see more about it in the next piece of this book on concurrency.

### 6.5 Summary

- The CPU should support at least two modes of execution: a restricted **user mode** and a privileged (non-restricted) **kernel mode**.
- Typical user applications run in user mode, and use a **system call** to **trap** into the kernel to request operating system services.
- The trap instruction saves register state carefully, changes the hardware status to kernel mode, and jumps into the OS to a pre-specified destination: the **trap table**.
- When the OS finishes servicing a system call, it returns to the user program via another special **return-from-trap** instruction, which reduces privilege and returns control to the instruction after the trap that jumped into the OS.
- The trap tables must be set up by the OS at boot time, and make sure that they cannot be readily modified by user programs. All of this is part of the **limited direct execution** protocol which runs programs efficiently but without loss of OS control.
- Once a program is running, the OS must use hardware mechanisms to ensure the user program does not run forever, namely the **timer interrupt**. This approach is a **non-cooperative** approach to CPU scheduling.
- Sometimes the OS, during a timer interrupt or system call, might wish to switch from running the current process to a different one, a low-level technique known as a **context switch**.

## Chapter 7. Scheduling: Introduction

This chapter describes a series of **scheduling policies** (sometimes called **disciplines**).

### 7.1 Workload Assumption

**workload**: the collectively processes running in the system.

In this book, author make the following assumptions about the processes, sometimes called **jobs**, that are running in the system:

1. Each job runs for the same amount of time.
2. All jobs arrive at the same time.
3. Once started, each job runs to completion.
4. All jobs only use the CPU (i.e., they perform no I/O)
5. The run-time of each job is known.

### 7.2 Scheduling Metrics

A **scheduling metric** is just something that we use to *measure* something, and there are a number of different metrics that make sense in scheduling.

For now, let us takes a single metric: **turnaround time**. The turnaround time of a job is defined as the time at which the job completes minus the time at which the job arrived in the system. More formally, the turnaround time T<sub>turnaround</sub> is:

T<sub>turnaround</sub> = T<sub>completion</sub> - T<sub>arrival</sub>

Because we have assumed that all jobs arrive at the same time, for now
T<sub>arrival</sub> = 0 and hence T<sub>turnaround</sub> = T<sub>completion</sub>.

You should note that turnaround time is a **performance** metric. Another metric of interest is **fairness**. Performance and fairness are often at odds in scheduling.

### 7.3 First In, First Out (FIFO)

The most basic algorithm we can implement is known as **First In, First Out (FIFO)** scheduling or sometimes **First Come, First Served (FCFS)**. FIFO has a number of positive properties: it is clearly simple and thus easy to implement. 

![Figure 7.1: FIFO Simple Example](./fig/ch7/7-1.png)

From Fig 7.1, A,B,C each job runs for 10 seconds. the average turnaround time for three jobs is (10+20+30)/3 = 20.

![Figure 7.2: Why FIFO Is Not That Great](./fig/ch7/7-2.png)

From Fig 7.2, this time A runs for 100 seconds while B and C run for 10 second each. The average turnaround time is a painful 110 seconds ((100+110+120)/3=110)

This problem is generally referred to as the **convoy effect**.

### 7.4 Shortest Job First (SJF)

This new scheduling discipline is known as **Shortest Job First (SJF)**, and the name should be easy to remember because it describes the policy quite completely: it runs the shortest job first, then the next shortest, and so on.

![Figure 7.3: SJF Simple Example](./fig/ch7/7-3.png)

From Fig 7.3, A runs for 100 seconds while B and C run for 10 second each. The average turnaround time from 110 second to 50 ((10+20+120)/3=50)

If all jobs arrive at the same time, SJF is indeed an **optimal** scheduling algorithm.

![Figure 7.4: SJF With Late Arrivals From B and C](./fig/ch7/7-4.png)

However, if jobs does not arrive at the same time, SJF will lead to a problem. From Fig 7.4, A arrives at t = 0 and needs to run for 100 seconds, whereas B and C arrive at t = 10 and each need to run for 10 seconds. The average turnaround time for three jobs is 103.33 second ((100+(110-10)+(120-10))/3=103.33)

### 7.5 Shortest Time-to-Completion First (STCF)

Now, let us relax assumption 3 (that jobs must run to completion). In **non-preemptive** schedulers, system would run each job to completion before considering whether to run a new job. While all modern schedulers are **preemptive**, and quite willing to stop one process from running in order to run another. In particular, the scheduler can perform a context switch, stopping one running process temporarily and resuming (or starting) another.

**Shortest Time-to-Completion First (STCF)** or **Preemptive Shortest Job First (PSJF)**: Any time a new job enters the system, the STCF scheduler determines which of the remaining jobs (including the new job) has the least time left, and schedules that one. 

![Figure 7.5: STCF Simple Example](./fig/ch7/7-5.png)

From Fig 7.5. using above example. The scheduler can certainly do something else when B and C arrive: it can preempt job A and decide to run another job, perhaps continuing A later. . The result is a much-improved average turnaround time: 50 seconds (((120-0)+(20-10)+(30-10))/3)

### 7.6 A New Metric: Response Time

**Response Time**: the time from when the job arrives in a system to the first time it is scheduled. More formally:

T<sub>response</sub> = T<sub>firstrun</sub> − T<sub>arrival</sub> 

![Figure 7.6: SJF Again(Bad for Response Time)](./fig/ch7/7-6.png)

For example, Fig 7.6 (with A arriving at time 0, B, C at time 10), the response time of each job is as follows: 0 for job A, 0 for job B, and 10 for job C, the average response time is 3.33.

### 7.7 Round Robin

**Round-Robin(RR)**: its basic idea is simple, instead of running jobs to completion, RR runs a job for a **time slice**(sometimes called **scheduling quantum**) and then switches to the next job in the run queue. It repeatedly does so until the jobs are finished. For this reason, RR is sometimes called **time-slicing**.

![Figure 7.7: Round Robin (Good for Response Time)](./fig/ch7/7-7.png)

For example, Fig 7.7(A, B, C arriving at time 0, and each wish to run 5 seconds), RR with a time-slice of 1 second. The average response time of RR is (0+1+2)/3 = 1; for SJF, average response time: (0+5+10)/3 = 5.

As you can see, the length of the time slice is critical for RR. The shorter it is, the better the performance of RR under the response-time metric. However, making the time slice too short is problematic: suddenly the cost of context switching will dominate overall performance. Thus, deciding on the length of the time slice presents a trade-off to a system designer, making it long enough to **amortize** the cost of switching without making it so long that the system is no longer responsive.

RR, with a reasonable time slice, is thus an excellent scheduler if response time is our only metric. How about turnaround time with RR? Using example above again, A finishes at 13, B at 14, C at 15, the average turnaround time is 14, pretty a awful! RR is indeed one of the *worst* policies if turnaround time is our metric.

More generally, any policy that is **fair**. Indeed, this is an inherent trade-off: if you are willing to be unfair, you can run shorter jobs to completion, but at the cost of response time; if you instead value fairness, response time is lowered, but at the cost of turnaround time. This type of **trade-off** is common in systems; you can’t have your cake and eat it too.

The first type (SJF, STCF) optimizes turnaround time, but is bad for response time. The second type(RR) optimizes response time but is bad for turnaround.

### 7.8 Incorporating I/O

Now we relax assumption 4 — of course all programs perform I/O. A scheduler clearly has a decision to make when a job initiates an I/O request, because jobs don't use CPU during the I/O, it is blocked waiting for I/O completion. The scheduler also has to make a decision when the I/O completes.

![Figure 7.8: Poor Use V.S. Overlap](./fig/ch7/7-8.png)

Here is an example to show the difference between poor use and overlap, Doing so allows for **overlap**, with the CPU being used by one process while waiting for the I/O of another process to complete; the system is thus better utilized.

### 7.9 No More Oracle

With a basic approach to I/O in place, we come to our final assumption: that the scheduler knows the length of each job. As we said before, this is likely the worst assumption we could make. In fact, in a general-purpose OS (like the ones we care about), the OS usually knows very little about the length of each job. 

### Summary

There are two families of approaches, the first runs the shortest job remaining and thus optimizes turnaround time; the second alternates between all jobs and thus optimizes response time. Both are bad where the other is good, alas, an inherent trade-off common in systems.

## Chapter 8. Scheduling: The Multi-Level Feedback Queue

The **Multi-Level Feedback Queue(MLFQ)** was first described by Corbato(Turing Award 1990) et al. in 1962.

The fundamental problem MLFQ tries to address is two-fold. Without a *priori*
knowledge of job length , First, it would like to optimize *turnaround time*. Second, MLFQ would like to make a system fell responsive to interactive users and thus minimize *response time*. 

The multi-level feedback queue is an excellent example of a system that
learns from the past to predict the future. 

### 8.1 MLFQ: Basic Rules

The MLFQ has a number of distinct **queues**, each assigned a different **priority level**. MLFQ uses priorities to decide which job should run at a given time: a job with higher priority (i.e., a job on a higher queue) is chosen to run. Of course, more than one job may be on a given queue, and thus have the same priority. In this case, we will just use round-robin scheduling among those jobs.

The first two basic rules for MLFQ:
- **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn't)
- **Rule 2**: If Priority(A) = Priority(B), A & B run in RR.

Rather than giving a fixed priority to each job, MLFQ *varies* the priority of a job based on its *observed behavior*. If, for example, a job repeatedly relinquishes the CPU while waiting for input from the keyboard, MLFQ will keep its priority high, as this is how an interactive process might behave. If, instead, a job uses the CPU intensively for long periods of time, MLFQ will reduce its priority. In this way, MLFQ will try to *learn* about processes as they run, and thus use the *history* of the job to predict its *future* behavior.

### 8.2 Attempt #1: How To Change Priority

We must keep in mind our workload: a mix of interactive jobs that are short-running (and may frequently relinquish the CPU), and some longer-running “CPU-bound” jobs that need a lot of CPU time but where response time isn’t important. Here is our first attempt at a priority-adjustment algorithm:

- **Rule 3**: When a job enters the system, it is placed at the highest priority(the topmost queue)
- **Rule 4a**: If a jobs uses up an entire time slice while running, its priority is *reduced* (i.e, it moves down one queue)
- **Rule 4b**: If a job gives up the CPU before the time slice is up, it stays at the *same* priority level.

#### Example 1: A Single Long-Running Job

![Figure 8.2: Long-running Job Over Time](./fig/ch8/8-2.png)

Figure 8.2 shows a single long-running job in MLFQ. the job enters at the highest priority (Q2). After a single time-slice of 10 ms, the scheduler reduces the job’s priority by one, and thus the job is on Q1. After running at Q1 for a time slice, the job is finally lowered to the lowest priority in the system (Q0), where it remains.

#### Example 2: Along Came A Short Job

In this example, there are two jobs: A, which is a long-running CPU-intensive job, and B, which is a short-running interactive job. 

![Figure 8.3: Along Came An Interactive Job](./fig/ch8/8-3.png)

Figure 8.3 plots the results of this scenario. A (shown in black) is running along in the lowest-priority queue (as would any long-running CPU-intensive jobs); B (shown in gray) arrives at time T = 100, and thus is inserted into the highest queue; as its run-time is short (only 20 ms), B completes before reaching the bottom queue, in two time slices; then A resumes running (at low priority).

The major goals of the algorithm: because it doesn’t know whether a job will be a short job or a long-running job, it first assumes it might be a short job, thus giving the job high priority. If it actually is a short job, it will run quickly and complete; if it is not a short job, it will slowly move down the queues, and thus soon prove itself to be a long-running more batch-like process. In this manner, MLFQ approximates SJF.

#### Example 3: What About I/O?

As Rule 4b states above,  if an interactive job, for example, is doing a lot of I/O (say by waiting for user input from the keyboard or mouse), it will relinquish the CPU before its time slice is complete.

![Figure 8.4: A Mixed I/O-intensive and CPU-intensive Workload](./fig/ch8/8-4.png)

Figure 8.4 shows an example of how this works, with an interactive job B (shown in gray) that needs the CPU only for 1 ms before performing an I/O competing for the CPU with a long-running batch job A (shown in black). The MLFQ approach keeps B at the highest priority because B keeps releasing the CPU; if B is an interactive job, MLFQ further achieves its goal of running interactive jobs quickly.

#### Problems with Our Current MLFQ

First, there is the problem of **starvation**: if there are “too many” interactive jobs in the system, they will combine to consume all CPU time, and thus long-running jobs will never receive any CPU time (they starve).

Second, a smart user could rewrite their program to **game the scheduler**. The algorithm we have described is susceptible to the following attack: before the time slice is over, issue an I/O operation (to some file you don’t care about) and thus relinquish the CPU; doing so allows you to remain in the same queue, and thus gain a higher percentage of CPU time. When done right (e.g., by running for 99% of a time slice before relinquishing the CPU), a job could nearly monopolize the CPU.

Finally, a program may *change its behavior* over time;

### 8.3 Attempt #2: The Priority Boost

The simple idea here is to periodically **boost** the priority of all the jobs in system. There are many ways to achieve this, but let’s just do something simple: throw them all in the topmost queue; hence, a new rule:

- **Rule 5**: After some time period S, move all the jobs in the system to topmost queue.

Such new rule solves two problems mentioned above at once. First, processes are guaranteed not to starve: by sitting in the top queue, a job will share the CPU with other high-priority jobs in a round-robin fashion, and thus eventually receive service. Second, if a CPU-bound job has become interactive, the scheduler treats it properly once it has received the priority boost.

![Figure 8.5: Without(Left) and With(Right) Priority Boost](./fig/ch8/8-5.png)

Figure 8.5 shows an example. On the left, there is no priority boost, and thus the long-running job gets starved once the two short jobs arrive; on the right, there is a priority boost every 50 ms and thus we at least guarantee that the long-running job will make some progress, getting boosted to the highest priority every 50 ms and thus getting to run periodically.

There is an obvious question: what should S be set to?  John Ousterhout used to call such values in systems voo-doo constants, because they seemed to require some form of black magic to set them correctly. Unfortunately, S has that flavor. If it is set too high, long-running jobs could starve; too low, and interactive jobs may not get a proper share of the CPU. TIP: Avoid voo-doo constants is a good idea whenever possible(**Ousterhout’s Law**) **(TODO Why?)**

### 8.4 Attempt #3: Better Accounting

How to prevent gaming of our scheduler? The solution here is to perform better accounting of CPU time at each level of the MLFQ. Instead of forgetting how much of a time slice a process used at a given level, the scheduler should keep track; once a process has used its allotment, it is demoted to the next priority queue. Whether it uses the time slice in one long burst or many small ones does not matter. We thus rewrite Rules 4a and 4b to the following single rule:

- **Rule 4**: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e, it moves down one queue).

![Figure 8.6: Without (Left) and With (Right) Gaming Tolerance](./fig/ch8/8-6.png)

Figure 8.6 shows an example what happens when a workload tries to game the scheduler with the old Rules 4a and 4b (on the left) as well the new anti-gaming Rule 4. Without any protection from gaming, a process can issue an I/O just before a time slice ends and thus dominate CPU time. With such protections in place, regardless of the I/O behavior of the process, it slowly moves down the queues, and thus cannot gain an unfair share of the CPU.

### 8.5 Tuning MLFQ And Other Issues

One big question is how to **parameterize** such a scheduler. For example, how many queues should there be? How big should the time slice be per queue? How often should priority be boosted in order to avoid starvation and account for changes in behavior? There are no easy answers to these questions, and thus only some experience with workloads and subsequent tuning of the scheduler will lead to a satisfactory balance.

![Figure 8.7: Lower Priority, Longer Quanta](./fig/ch8/8-7.png)

For example, most MLFQ variants allow for varying time-slice length across different queues. The high-priority queues are usually given short time slices, they are comprised of interactive jobs, after all, and thus quickly alternating between them makes sense. The low-priority queues, in contrast, contain long-running jobs that are CPU-bound; hence, longer time slices work well. As Figure 8.7 shown.

Solaris MLFQ implementation -- Default values for the table are 60 queues, with slowly increasing time-slice lengths from 20 milliseconds (highest priority) to a few hundred milliseconds (lowest), and priorities boosted around every 1 second or so.

FreeBSD scheduler use a formula to calculate the current priority level of a job, basing it on how much CPU the process has used.

Finally,  many schedulers have a few other features that you might encounter. For example, some schedulers reverse the highest priority levels for operating system work. Some systems also provide interfaces to allow users or administrators to provide some hints to help the OS set priorities(e.g. with `nice`, `madvise`).

### 8.6 MLFQ: Summary

Multi-Level Feedback Queue(MLFQ) has multiple levels of queues, and uses feedback to determine the priority of a given job. History is its guide: pay attention to how jobs
behave over time and treat them accordingly.

The refined set of MLFQ rules, spread throughout the chapter, are reproduced here for your viewing pleasure:
- Rule 1: If Priority(A) > Priority(B), A runs (B doesn’t).
- Rule 2: If Priority(A) = Priority(B), A & B run in round-robin fashion using the time slice (quantum length) of the given queue.
- Rule 3: When a job enters the system, it is placed at the highest priority (the topmost queue).
- Rule 4: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).
- Rule 5: After some time period S, move all the jobs in the system to the topmost queue.

MLFQ is interesting for the following reason: instead of demanding a priori knowledge of the nature of a job, it observes the execution of a job and prioritizes it accordingly. 

## Chapter 9. Scheduling: Proportional Share

**Proportional-share(fair-share)** scheduler: it try to guarantee that each job obtain a certain percentage of CPU time. An excellent early example is **lottery scheduling**.

The basic idea of **lottery scheduling** is quite simple: every so often, hold a lottery to determine which process should get to run next; processes that should run more often should be given more chances to win the lottery. 

### 9.1 Basic Concept: Tickets Represent Your Share

**Tickets** are used to represent the share of a resource that a process should receive. The percent of tickets that a process has represents its share of the system resource in question. For example, A has 75 tickets while B has 25 tickets, Thus, A receive 75% of the CPU and B the remaining 25%.

Holding a lottery is straightforward: the scheduler must know how many total tickets there are (in our example, there are 100). The scheduler then picks a winning ticket, which is a number from 0 to 991. Assuming A holds tickets 0 through 74 and B 75 through 99, the winning ticket simply determines whether A or B runs. The scheduler then loads the state of that winning process and runs it. The longer these two jobs compete, the more likely they are to achieve the desired percentages.

One of the most beautiful aspects of lottery scheduling is its use of randomness. Random approaches have at least three advantages over more traditional decisions. First, random often avoids strange corner-case behaviors that a more traditional algorithm may have trouble handling. Second, random also is lightweight, requiring little state to track alternatives. Finally, random can be quite fast.

In fact, tickets can be applied much more broadly. For example, on virtual memory management for hypervisors, represent a guest operating system's share of memory. Thus, if you are ever in need of a mechanism to represent a proportion of ownership, this concept just might be ... (wait for it) ... the ticket.

### 9.2 Ticket Mechanisms

Lottery scheduling also provides a number of mechanisms to manipulate tickets in different and sometimes useful ways. One way is with the concept of **ticket currency**. Currency allows a user with a set of tickets to allocate tickets among their own jobs in whatever currency they would like; the system then automatically converts said currency into the correct global value. 

For example, A running two jobs, A1 and A2, then give them each 500 ticks. B running one job, B1 and give it 10 tickets. First, the system convert 500 each in A to 50 each, B1's 10 tickets is converted to 100 tickets, then the lottery held over the global ticket currency (200 total) to determine which job runs.

Another useful mechanism is **ticket transfer**. With transfers, a process can temporarily hand off its tickets to another process. This ability is especially useful in a client/server setting, where a client process sends a message to a server asking it to do some work on the client’s behalf. To speed up the work, the client can pass the tickets to the server and thus try to maximize the performance of the server while the server is handling the client’s request. When finished, the server then transfers the tickets back to the client and all is as before.

Finally, **ticket inflation** can sometimes be a useful technique. With inflation, a process can temporarily raise or lower the number of tickets it owns. In a competitive scenario with processes that do not trust each other, this make little sense, because one greedy process could give itself a vast number of tickets. Rather, ticket inflation can be applied in an environment where a group of processes trust one another; in such a case, if any one process knows it needs more CPU time, it can boost its ticket value as a way to reflect that need to the system, all without communicating with any other processes.

### 9.3 Implementation

Probably the most amazing thing about lottery scheduling is the simplicity of its implementation. To make a scheduling decision, we first have to pick a random number (the winner) from the total number of tickets. Then, we simply traverse the list, with a simple counter used to help us find the winner.

The Lottery Scheduling Decision code is as follows. Adding each ticket value to `counter`
until the value exceeds `winner`. Once that is the cache, the current list element is the winner.

```c
// counter: used to track if we’ve found the winner yet
int counter = 0;

// winner: use some call to a random number generator to
// get a value, between 0 and the total # of tickets
int winner = getrandom(0, totaltickets);

// current: use this to walk through the list of jobs
node_t *current = head;
while (current) {
    counter = counter + current->tickets;
    if (counter > winner)
        break; // found the winner
    current = current->next;
}
// ’current’ is the winner: schedule it...
```

To make this process most efficient, it might generally be best to organize the list in sorted order, from the highest number of tickets to the lowest. The ordering does not affect the correctness of the algorithm; however, it does ensure in general that the fewest number of list iterations are taken, especially if there are a few processes that possess most of the tickets.

### 9.4 An Example

Assuming there are two jobs competing against one another, each with the same number of tickets(100) and the same run time(R, which we will vary).

We define a simple **fairness metric(F)**, which is simply the time the first job completes divided by the time that the second job completes. For example, if R = 10, the first job finishes at time 10, the second job finishes at time 20. F = 10/20 = 0.5. Our goal to get a perfectly fair scheduler would achieve F=1.

![Figure 9.2: Lottery Fairness Study](./fig/ch9/9-2.png)

Figure 9.2 plots the average fairness as the length of the two jobs (R) is varied from 1 to 1000 over thirty trials. As you can see from the graph, when the job length is not very long, average fairness can be quite low. Only as the jobs run for a significant number of time slices does the lottery scheduler approach the desired fair outcome.

### 9.5 How To Assign Tickets

One problem we have not addressed with lottery scheduling is: how to assign tickets to jobs? This problem is a tough one, because of course how the system behaves is strongly dependent on how tickets are allocated. One approach is to assume that the users know best; in such a case, each user is handed some number of tickets, and a user can allocate tickets to any jobs they run as desired. However, this solution is a non-solution: it really doesn’t tell you what to do. Thus, given a set of jobs, the “ticket-assignment problem” remains open.

### 9.6 Stride Scheduling

**Stride Scheduling** is a deterministic fair-share scheduler. Each job in the system has a stride, which is inverse in proportion to the number of tickets it has. We can compute the stride of each by dividing some large number by the number of tickets each process has been assigned. We call this value the **stride** of each process; every time a process runs, we will increment a counter for it (called its **pass** value) by its stride to track its global progress. For example, A, B, and C, with 100, 50, and 250 tickets. the stride value for A, B, C is 100, 200, and 40.

The scheduler then uses the stride and pass to determine which process should run next. The basic idea is simple: at any given time, pick the process to run that has the lowest pass value so far; when you run a process, increment its pass counter by its stride. The pseudocode is as follows:

```c
curr = remove_min(queue);   // pick client with min pass
schedule(curr);             // run for quantum
curr->pass += curr->stride; // update pass using stride
insert(queue, curr);        // return curr to queue
```

![Figure 9.3: Stride Scheduling: A Trace](./fig/ch9/9-3.png)

As figure 9.3 shown, C ran five times, A twice, and B just once. Lottery scheduling achieves the proportions probabilistically over time; stride scheduling gets them exactly right at the end of each scheduling cycle.

Lottery scheduling has one nice property that stride scheduling does not: no global state. Imagine a new job enters in the middle of our stride scheduling example above; what should its pass value be? Should it be set to 0? If so, it will monopolize the CPU. With lottery scheduling, there is no global state per process; we simply add a new process with whatever tickets it has, update the single global variable to track how many total tickets we have, and go from there. In this way, lottery makes it much easier to incorporate new processes in a sensible manner.

### 9.7 The Linux Completely Fair Scheduler (CFS)

The Linux **Completely Fair Scheduler**(or **CFS**) achieves fair-share scheduling, but does so in a highly efficient and scalable manner.

To achieve its efficiency goals, CFS aims to spend very little time making scheduling decisions, through both its inherent design and its clever use of data structures well-suited to the task. For example, in a study of Google data centers, Kanev et al. show that even after aggressive optimization, scheduling uses about 5% of overall data center CPU time. Reducing that overhead as much as possible is thus a key goal in modern scheduler architecture.

#### Basic Operation

To fairly dived a CPU evenly among all competing processes, CFS takes a simple counting-based technique known as **virtual runtime(vruntime)**.

As each process runs, it accumulates `vruntime`. In the most basic case, each process’s `vruntime` increases at the same rate, in proportion with physical (real) time. When a scheduling decision occurs, CFS will pick the process with the lowest `vruntime` to run next.

If CFS switches processes too often, fairness is increased, but at the cost of performance (too much context switching); If CFS switches less often, performance is increased (reduced context switching), but at the cost of near-term fairness.

CFS manages this tension through various control parameters. The first is `sched_latency`. CFS uses this value to determine how long one process should run before considering a switch (effectively determining its time slice but in a dynamic fashion). A typical `sched_latency` value is 48 (milliseconds); CFS divides this value by the number (n) of processes running on the CPU to determine the time slice for a process, and thus ensures that over this period of time, CFS will be completely fair. For example, if n = 4, then `sched_latency` will be 12ms, that means a per-process time slice is 12ms.

But what if there are too many processes running? CFS add another parameter `min_granularity`, which is usually set to a value like 6 ms. CFS will never set the time slice of a process to less than this value, ensuring that not too much time is
spent in scheduling overhead. For example, if n = 10, although `sched_latency` is 4.8ms, `min_granularity` is 6ms, so the time slice of each process is 6ms instead.

Note that CFS utilizes a periodic timer interrupt, which means it can only make decisions at fixed time intervals. This interrupt goes off frequently (e.g., every 1 ms), giving CFS a chance to wake up and determine if the current job has reached the end of its run. If a job has a time slice that is not a perfect multiple of the timer interrupt interval, that is OK; CFS tracks vruntime precisely, which means that over the long haul, it will eventually approximate ideal sharing of the CPU.

#### Weighting (Niceness)

CFS also enables controls process priority through a classic UNIX mechanism known as the **nice** level of a process. The nice parameter can be set anywhere from -20 to +19 for a process, with a default of 0. Positive nice values imply *lower* priority and negative values imply *higher* priority; 

CFS maps the nice value of each process to a weight, as shown here:

```c
static const int prio_to_weight[40] = {
/* -20 */ 88761, 71755, 56483, 46273, 36291,
/* -15 */ 29154, 23254, 18705, 14949, 11916,
/* -10 */ 9548, 7620, 6100, 4904, 3906,
/* -5  */ 3121, 2501, 1991, 1586, 1277,
/* 0   */ 1024, 820, 655, 526, 423,
/* 5   */ 335, 272, 215, 172, 137,
/* 10  */ 110, 87, 70, 56, 45,
/* 15  */ 36, 29, 23, 18, 15,
};
```

These weights allow us to compute the effective time slice of each process (as we did before), but now accounting for their priority differences. The formula used to do so is as follows, assuming n processes:

![Formula 1](./fig/ch9/formula-1.png)

In addition to generalizing the time slice calculation, the way CFS calculates vruntime must also be adapted. Here is the new formula, which takes the actual run time that process i has accrued (runtime<sub>i</sub>) and scales it inversely by the weight of the process, by dividing the default weight of 1024 (weight<sub>0</sub>) by its weight, weight<sub>i</sub>.

![Formula 2](./fig/ch9/formula-2.png)

#### Using Red-Black Trees

CFS find the next job to run by keeping processes in a **red-blck tree**. A red-black tree is one of many types of balanced trees. CFS **does not** keep *all* process in this structure; rather, **only running** (or **runnable**) processes are kept therein. Processes are ordered in the tree by `vruntime`, and most operations (such as insertion and deletion) are logarithmic in time, i.e., O(log n).

#### Dealing With I/O And Sleeping Processes

One problem with picking the lowest `vruntime` to run next arises with jobs that have gone to sleep for a long period of time. When a job finish I/O and ready to run. CFS will sets the runtime of that job to the minimum value found in the tree(remember, the tree only contains running jobs). In this way, CFS avoids starvation, but not without a cost: jobs that sleep for short periods of time frequently do not ever get their fair share of the CPU.

#### Other CFS Fun

CFS has many other features, too many to discuss at this point in the book. It includes numerous heuristics to improve cache performance, has strategies for handling multiple CPUs effectively  (as discussed later in the book), can schedule across large groups of processes (instead of treating each process as an independent entity), and many other interesting features.

### 9.8 Summary

We have introduced the concept of proportional-share scheduling and briefly discussed three approaches: lottery scheduling, stride scheduling, and the Completely Fair Scheduler (CFS) of Linux. Lottery uses randomness in a clever way to achieve proportional share; stride does so deterministically. CFS, the only “real” scheduler discussed in this chapter, is a bit like weighted round-robin with dynamic time slices, but built to scale and perform well under load; to our knowledge, it is the most widely used fair-share scheduler in existence today.

No scheduler is a panacea, and fair-share schedulers have their fair share of problems. One issue is that such approaches do not particularly mesh well with I/O; as mentioned above, jobs that perform I/O occasionally may not get their fair share of CPU. Another issue is that they leave open the hard problem of ticket or priority assignment, i.e., how do you know how many tickets your browser should be allocated, or to what nice value to set your text editor? Other general-purpose schedulers (such as the MLFQ we discussed previously, and other similar Linux schedulers) handle these issues automatically and thus may be more easily deployed.
