## Chapter 2. Introduction to Operating System (Note)

- Virtualization
- Concurrency
- Persistence

How does OS virtualize resources?

virtualization: the OS takes a **physical** resource (such as the processor, or memory, or a disk) and transforms it into a more general, powerful, and easy-to-use **virtual** form of itself

OS provides a **standard library(system calls)** to applications.

the OS is sometimes known as a **resource manager**. Each of the CPU, memory, and disk is a **resource** of the system; it is thus the operating system’s role to **manage** those re-sources

### 2.1 Virtualizing the CPU

Turning a single CPU (or a small set of them) into a seemingly infinite number of CPUs and thus allowing many programs to seemingly run at once is what we call **virtualizing the CPU**, the focus of the first major part of this book.

Hence the role of the OS as a **resource manager**.

### 2.2 Virtualizing Memory

Each process accesses its own private **virtual address space**(sometimes just called its **address space**)

### 2.3 Concurrency

```
$ ./threads 9000000
Initial value: 0
Final value: 15347306
```

Because these three instructions do not execute atomically(all at once), strange things can happen. 

### 2.4 Persistence

To handle the problems of system crashes during writes, most file systems incorporate some kind of intricate write protocol, such as **journaling** or **copy-on-write**

### 2.5 Design Goals

One of the most basic goals is to build up some **abstractions** in order to make the system convenient and easy to use. 

One goal in designing and implementing an operating system is to provide high **performance**; another way to say this is our goal is to **minimize the overheads** of the OS. These overheads arise in a number of forms: extra time (more instructions) and extra space (in memory or on disk). We’ll seek solutions that minimize one or the other or both, if possible.

Another goal will be to provide **protection** between applications, as well as between the OS and applications.

Because of this dependence,operating systems often strive to provide a high degree of **reliability**

Other goals make sense: **energy-efficiency**、**security**、**mobility**

### 2.6 Some History

Omitted here.

### 2.7 Summary

There are some part of OS doesn't cover in the book. For example, networking code, graphics devices, security, etc.
