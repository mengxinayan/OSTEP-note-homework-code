## Chapter 6. Mechanism: Limited Direct Execution (Note)

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
