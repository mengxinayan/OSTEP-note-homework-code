## Chapter 15. Mechanism: Address Translation

In CPU virtualization part, we focused on a general mechanism known as limited direct execution (or LDE). The idea behind LDE is simple: for the most part, let the program run directly on the hardware; however, at certain key points in time arrange so that the OS gets involved and makes sure the “right” thing happens. Thus, the OS with a little hardware support, tries its best to get out of the way of the running program, to deliver an efficient virtualization; however, by **interposing** at those critical points in time, the OS ensures that it maintains control over the hardware. Efficiency and control together are two of the main goals of any modern operating system.

In virtualizing memory, we will pursue a similar strategy, attaining both efficiency and control. *Efficiency* dictates that we make use of hardware support, which at first will be quite rudimentary but will grow to be fairly complex. *Control* implies that the OS ensures that no application is allowed to access any memory but its own; Finally, we will need a little more from the VM system, in terms of *flexibility*; specifically, we’d like for programs to be able to use their address spaces in whatever way they would like, thus making the system easier to program.

The generic technique is referred to as **hardware-based address translation**, or just **address translation** for short. With address translation, the hardware transforms each memory access, changing the **virtual** address provided by the instruction to a **physical** address where the desired information is actually located.

Of course, the hardware alone cannot virtualize memory, as it just provides the low-level mechanism for doing so efficiently. The OS must get involved at key points to set up the hardware so that the correct translations take place; it must thus **manage memory**, keeping track of which locations are free and which are in use, and judiciously intervening to maintain control over how memory is used.

Once again the goal of all of this work is to create a beautiful **illusion**: that the program has its own private memory, where its own code and data reside. Behind that virtual reality lies the ugly physical truth: that many programs are actually sharing memory at the same time, as the CPU (or CPUs) switches between running one program and the next. Through virtualization, the OS (with the hardware’s help) turns the ugly machine reality into a useful, powerful, and easy to use abstraction.

### 15.1 Assumptions

Specifically, we will assume for now that the user’s address space must be placed *contiguously* in physical memory. We will also assume, for simplicity, that the size of the address space is not too big; specifically, that it is *less than the size of physical memory*. Finally, we will also assume that each address space is exactly the *same size*. Don’t worry if these assumptions sound unrealistic; we will relax them as we go, thus achieving a realistic virtualization of memory.

### 15.2 An Example

Imagine there is a process whose address space is as indicated in Fig 15.1.  What we are going to examine here is a short code sequence that loads a value from memory, increments it by three, and then stores the value back into memory. The C-language representation of this code might look like this:

```c
void func() {
    int x = 3000; // thanks, Perry.
    x = x + 3; // line of code we are interested in
}
```

The compiler turns this line of code into assembly, which might look
something like this (in x86 assembly).

```x86asm
128: movl 0x0(%ebx), %eax ;load 0+ebx into eax
132: addl $0x03, %eax ;add 3 to eax register
135: movl %eax, 0x0(%ebx) ;store eax back to mem
```

In Figure 15.1 (page 4), observe how both the code and data are laid out in the process’s address space; the three-instruction code sequence is located at address 128 (in the code section near the top), and the value of the variable x at address 15 KB (in the stack near the bottom). In the figure, the initial value of x is 3000, as shown in its location on the stack.
When these instructions run, from the perspective of the process, the following memory accesses take place.

![Figure 15.1: A Process And Its Address Space](./fig/15-1.png)

From the program’s perspective, its **address space** starts at address 0 and grows to a maximum of 16 KB; However, to virtualize memory, the OS wants to place the process somewhere else in physical memory, not necessarily at address 0. Thus, we have the problem: how can we **relocate** this process in memory in a way that is **transparent** to the process? How can we provide the illusion of a virtual address space starting at 0, when in reality the address space is located at some other physical address?

An example of what physical memory might look like once this process’s address space has been placed in memory is found in Figure 15.2. In the figure, you can see the OS using the first slot of physical memory for itself, and that it has relocated the process from the example above into the slot starting at physical memory address 32 KB. The other two slots are free (16 KB-32 KB and 48 KB-64 KB).

![Figure 15.2: Physical Memory with a Single Relocated Process](./fig/15-2.png)

### 15.3 Dynamic (Hardware-based) Relocation

Introduced in the first time-sharing machines of the late 1950's is a simple idea referred to as **base and bounds**, the technique is also referred to as **dynamic relocation**;

Specifically, we'll need two hardware registers within each CPU: one is called the **base** register, and the other is the **bounds** registers(some times called **limit** registers). This base-and-bounds pair is going to allow us to place the address space anywhere we’d like in physical memory, and do so while ensuring that the process can only access its own address space.

When a program starts running, the OS decides where in physical memory it should be loaded and sets the base register to that value. In the example above, the OS decides to load the process at physical address 32 KB and thus sets the base register to this value. Now, when any memory reference is generated by the process, it is translated by the processor in the following manner:

```
physical address = virtual address + base
```

Each memory reference generated by the process is a **virtual address**; the hardware in turn adds the contents of the base register to this address and the result is a **physical address** that can be issued to the memory system.

Transforming a virtual address into a physical address is exactly the technique we refer to as **address translation**; that is, the hardware takes a virtual address the process thinks it is referencing and transforms it into a physical address which is where the data actually resides. Because this relocation of the address happens at runtime, and because we can move address spaces even after the process has started running, the technique is often referred to as **dynamic relocation**.

How about bounds(limit) register? Specifically, the processor will first check that the memory reference is within bounds to make sure it is legal; in the simple example above, the bounds register would always be set to 16 KB. If a process generates a virtual address that is greater than the bounds, or one that is negative, the CPU will raise an exception, and the process will likely be terminated. The point of the bounds is thus to make sure that all addresses generated by the process are legal and within the “bounds” of the process.

Bounds registers can be defined in one of two way. In one way, it holds the *size* of the address space, and thus the hardware checks the virtual address against it first before adding the base. In the second way, it holds the *physical address* of the end of the address space, and thus the hardware first adds the base and then makes sure the address is within bounds. Both methods are logically equivalent; for simplicity, we’ll usually assume the former method.

In a word, a **base** register is used to transform virtual addresses (generated by the program) into physical addresses. A **bounds** (or limit) register ensures that such addresses are within the confines of the address space. Together they provide a simple and efficient virtualization of memory. We should note that the base and bounds registers are hardware structures kept on the chip (one pair per CPU). Sometimes people call the part of the processor that helps with address translation the **memory management unit (MMU)**.

#### Example Translations

Imagine a process with an address space of size 4KB has been loaded at physical address 16KB. Here are the results of a number of address translations:

| Virtual Address | Physical Address |
| -: | -: |
| 0 | 16KB |
| 1KB | 17KB |
| 3000 | 19384 |
| 4400 | Fault(out of bounds) |

As you can see from the example, it is easy for you to simply add the base address to the virtual address (which can rightly be viewed as an offset into the address space) to get the resulting physical address. Only if the virtual address is “too big” or negative will the result be a fault, causing an exception to be raised.

### 15.4 Hardware Support: A Summary

Let's summarize the support we need from the hardware.

First, there are two different CPU modes, that is **privileged mode** (or **kernel mode**) and **user mode**. A single bit, perhaps stored in some kind of processor status word, indicates which mode the CPU is currently running in; upon certain special occasions (e.g., a system call or some other kind of exception or interrupt), the CPU switches modes.

Second, the hardware must provide the **base and bounds registers**, each CPU thus has an additional pair of registers, part of the memory management unit (MMU) of the CPU. And the hardware must also be able to check whether the address is valid.

Third, the hardware should provide special instructions to modify the base and bounds registers, allowing the OS to change them when different processes run. These instructions are privileged; only in kernel (or privileged) mode can the registers be modified. 

Finally, the CPU must be able to generate exceptions in situations where a user program tries to access memory illegally (with an address that is “out of bounds”); in this case, the CPU should stop executing the user program and arrange for the OS “out-of-bounds” exception handler to run. Similarly, if a user program tries to change the value of the (privileged) base and bound registers, the CPU should raise an exception and run the "tried to execute a privileged operation while in user mode" handler.

The summary of **dynamic relocation: hardware requirements** is shown in Fig 15.3:

![Figure 15.3: Dynamic Relocation: Hardware Requirements](./fig/15-3.png)

### 15.5 Operating System Issues

Just as the hardware provides new features to support dynamic relocation, the OS now has new issues it must handle. Specifically, there are a few critical junctures where the OS must get involved to implement our base-and-bounds version of virtual memory.

First, the OS must take action when a process is created, finding space for its address space in memory. Fortunately, given our assumptions that each address space is (a) smaller than the size of physical memory and (b) the same size, this is quite easy for the OS; it can simply view physical memory as an array of slots, and track whether each one is free or in use. When a new process is created, the OS will have to search a data structure (often called a **free list**) to find room for the new address space and then mark it used. 

Second, the OS must do some work when a process is terminated (i.e., when it exits gracefully, or is forcefully killed because it misbehaved), reclaiming all of its memory for use in other processes or the OS. Upon termination of a process, the OS thus puts its memory back on the free list, and cleans up any associated data structures as need be.

Third, the OS must also perform a few additional steps when a context switch occurs. There is only one base and bounds register pair on each CPU, after all, and their values differ for each running program, as each program is loaded at a different physical address in memory. Thus, the OS must *save and restore* the base-and-bounds pair when it switches between processes, in some per-process structure such as the **process structure** or **process control block** (PCB).

We should note that when a process is stopped (i.e., not running), it is possible for the OS to move an address space from one location in memory to another rather easily. To move a process’s address space, the OS first deschedules the process; then, the OS copies the address space from the current location to the new location; finally, the OS updates the saved base register (in the process structure) to point to the new location. When the process is resumed, its (new) base register is restored, and it begins running again, oblivious that its instructions and data are now in a completely new spot in memory.

Fourth, the OS must provide exception handlers, or functions to be called, as discussed above; the OS installs these handlers at boot time (via privileged instructions). For example, if a process tries to access memory outside its bounds, the CPU will raise an exception; the OS must be prepared to take action when such an exception arises. The common reaction of the OS will be one of hostility: it will likely terminate the offending process. The OS should be highly protective of the machine it is running, and thus it does not take kindly to a process trying to access memory or execute instructions that it shouldn’t.

The summary of **dynamic relocation: operating system responsibilities** is shown in Fig 15.4:

![Figure 15.4: Dynamic Relocation: Operating System Responsibilities](./fig/15-4.png)

Figure 15.5 shows what the OS does at boot time to ready the machine for use.

![Figure 15.5: Limited Direct Execution (Dynamic Relocation) @ Boot](./fig/15-5.png)

Figure 15.6 shows what happens when a process A start running, note how its memory translations are handled by the hardware with no OS intervention. As you can see from the figures, we are still following the basic approach of limited direct execution. In most cases, the OS just sets up the hardware appropriately and lets the process run directly on the CPU; only when the process misbehaves does the OS have to become involved.

![Figure 15.6: Limited Direct Execution (Dynamic Relocation) @ Runtime](./fig/15-6.png)

### 15.6 Summary

In this chapter, we have extended the concept of limited direct execution with a specific mechanism used in virtual memory, known as **address translation**. With address translation, the OS can control each and every memory access from a process, ensuring the accesses stay within the bounds of the address space. Key to the efficiency of this technique is hardware support, which performs the translation quickly for each access, turning virtual addresses (the process’s view of memory) into physical ones (the actual view). All of this is performed in a way that is *transparent* to the process that has been relocated; the process has no idea its memory references are being translated, making for a wonderful illusion.

We have also seen one particular form of virtualization, known as base and bounds or dynamic relocation. Base-and-bounds virtualization is quite *efficient*, as only a little more hardware logic is required to add a base register to the virtual address and check that the address generated by the process is in bounds. Base-and-bounds also offers *protection*; the OS and hardware combine to ensure no process can generate memory references outside its own address space. Protection is certainly one of the most important goals of the OS; without it, the OS could not control the machine (if processes were free to overwrite memory, they could easily do nasty things like overwrite the trap table and take over the system).

Unfortunately, this simple technique of dynamic relocation does have its inefficiencies. For example, as you can see in Figure 15.2 (page 5), the relocated process is using physical memory from 32 KB to 48 KB; however, because the process stack and heap are not too big, all of the space between the two is simply *wasted*. This type of waste is usually called **internal fragmentation**, as the space inside the allocated unit is not all used (i.e., is fragmented) and thus wasted. In our current approach, although there might be enough physical memory for more processes, we are currently restricted to placing an address space in a fixed-sized slot and thus internal fragmentation can arise2. Thus, we are going to need more sophisticated machinery, to try to better utilize physical memory and avoid
internal fragmentation.
