## Chapter 14. Interlude: Memory API (Note)

### 14.1 Types of Memory

There are two types of memory that are allocated.

The first is **stack** memory, its allocations and deallocations are managed *implicitly* by the compiler, sometimes it is called **automatic** memory.

Declaring memory on the stack in C is easy. To declare such a piece of memory, you just do something like this:

```c
void func()
{
    int x; // declares an integer on the stack
}
```

The compiler does the rest, making sure to make space on the stack when you call into `func()`. When you return from the function, the compiler deallocates the memory for you; thus, if you want some information to live beyond the call invocation, you had better not leave that information on the stack.

The second type of memory is **heap** memory, where all allocations and deallocations are *explicitly* handled by you, the programmer. A heavy responsibility, no doubt! Here is an example of how one might allocate an integer on the heap:

```c
void func()
{
    int *x = (int *) malloc(sizeof(int));
}
```

First the compiler knows to make room for a pointer to an integer when it sees your declaration of said pointer `(int *x)`; subsequently, when the program calls `malloc()`, it requests space for an integer on the heap; the routine returns the address of such an integer (upon success, or `NULL` on failure), which is then stored on the stack for use by the program.

Because of its explicit nature, and because of its more varied usage,heap memory Because presents more challenges to both users and systems.

### 14.2 The `malloc()` Call

The **`malloc()`** call is quite simple: you pass it a size asking for some room on the heap, and it either succeeds and gives you back a pointer to the newly-allocated space, or fails and returns `NULL`. Here is the manual page shows what you need to do to use malloc:

```c
#include <stdlib.h>

void *malloc(size_t size)
```

First, you should include the header file `stdlib.h` to use malloc. The single parameter `malloc()` takes is of type size t which simply describes how many bytes you need. However, most programmers do not type in a number here directly (such as 10); indeed, it would be considered poor form to do so.

The single parameter malloc() takes is of type size t which simply describes how many bytes you need. However, most programmers do not type in a number here directly (such as 10); indeed, it would be considered poor form to do so. Instead, various routines and macros are utilized. For example, to allocate space for a double-precision floating point value, you simply do this:

```c
double *d = (double *) malloc(sizeof(double));
```

In C, `sizeof()` is generally thought of as a *compile-time* operator, meaning that actual size is known at *compile time* and thus a number is substituted as the argument to `malloc()`. For this reason, `sizeof()` is correctly thought of as an operator and not a function call (a function call would take place at run time).

You can also pass in the name of a variable (and not just a type) to sizeof(), but in some cases you may not get the desired results, so be careful. For example

```c
int *x = malloc(10 * sizeof(int));
printf("%d\n", sizeof(x));
```

when we use `sizeof()` in the next line, it returns a small value, such as 4 (on 32-bit machines) or 8 (on 64-bit machines). The reason is that in this case, sizeof() thinks we are simply asking how big a pointer to an integer is, not how much memory we have dynamically allocated. However, sometimes `sizeof()` does work as you might expect:

```c
int x[10];
printf("%d\n", sizeof(x));
```

In this case, there is enough static information for the compiler to
know that 40 bytes have been allocated.

Another place to be careful is with strings. When declaring space for a string, use the following idiom: `malloc(strlen(s) + 1)`, which gets the length of the string using the function `strlen()`, and adds 1 to it in order to make room for the end-of-string character. Using `sizeof()` may lead to trouble here.

You might also notice that `malloc()` returns a pointer to type void. Doing so is just the way in C to pass back an address and let the programmer decide what to do with it. The programmer further helps out by using what is called a **cast**; Casting doesn’t really accomplish anything, other than tell the compiler and other programmers who might be reading your code: “yeah, I know what I’m doing.” By casting the result of `malloc()`, the programmer is just giving some reassurance; the cast is not needed for the correctness.

### 14.3 The `free()` Call

To free heap memory that is no longer in use, programmers simply call **free()**:

```c
int *x = malloc(10 * sizeof(int));
...
free(x);
```

The routine takes one argument, a pointer returned by `malloc()`. Thus, you might notice, the size of the allocated region is not passed in by the user, and must be tracked by the memory-allocation library itself.

### 14.4 Common Errors

There are a number of common errors that arise in the use of `malloc()` and `free()`. All of these error examples compile and run with nary a peep from the compiler;

Correct memory management has been such a problem, in fact, that many newer languages have support for **automatic memory management**. In such languages, while you call something akin to `malloc()` to allocate memory (usually **new** or something similar to allocate a new object), you never have to call something to free space; rather, a **garbage collector** runs and figures out what memory you no longer have references to and frees it for you.

#### Forgetting to Allocate Memory

Many routines expect memory to be allocated before you call them, such as `strcpy(dst, src)`, For example:

```
char *src = "hello";
char *dst; // oops! unallocated
strcpy(dst, src); // segfault and die
```

When you run such above code, it will lead to a **segmentation fault**. In this case, the proper code might instead look like this:

```c
char *src = "hello";
char *dst = (char *) malloc(strlen(src) + 1);
strcpy(dst, src); // work properly
```

Alternately, you could use `strdup()` and make your life even easier.

#### Not Allocating Enough Memory

Not allocating enough memory, sometimes called a **buffer overflow**. In the example above, a common error is to make almost enough
room for the destination buffer.

```c
char *src = "hello";
char *dst = (char *) malloc(strlen(src)); // too small!
strcpy(dst, src); // work properly
```

In some cases, it will overwrite a variable that isn't used anymore. While in some cases, these overflows can be incredibly harmful, and in fact are the source of many security vulnerabilities in systems. In other cases, the malloc library allocated a little extra space anyhow, and thus your program actually doesn’t scribble on some other variable’s value and works quite fine. And thus we learn another valuable lesson: even though it ran correctly once, doesn’t mean it’s correct.

#### Forgetting to Initialize Allocated Memory

With this error, you call `malloc()` properly, but forget to fill in some values into your newly-allocated data type. If you do forget, your program will eventually encounter an uninitialized read, where it reads from the heap some data of unknown value. Who knows what might be in there? 

#### Forgetting to Free Memory

Another common error is known as a **memory leak**, and it occurs when you forget to free memory. In long-running applications or systems (such as the OS itself), this is a huge problem, as slowly leaking memory eventually leads one to run out of memory, at which point a restart is required.  Note that using a garbage-collected language doesn’t help here: if you still have a reference to some chunk of memory, no garbage collector will ever free it, and thus memory leaks remain a problem even in more modern languages.

In some cases, it may seem like not calling `free()` is reasonable. For example, your program is short-lived, and will soon exit; in this case, when the process dies, the OS will clean up all of its allocated pages and thus no memory leak will take place per se. The reason is simple: there are really two levels of memory management in the system. The first level of memory management is performed by the OS, which hands out memory to processes when they run, and takes it back when processes exit (or otherwise die). The second level of management is within each process, for example within the heap when you call `malloc()` and `free()`. Even if you fail to call `free()` (and thus leak memory in the heap), the operating system will reclaim all the memory of the process (including those pages for code, stack, and, as relevant here, heap) when the program is finished running. No matter what the state of your heap in your address space, the OS takes back all of those pages when the process dies, thus ensuring that no memory is lost despite the fact that you didn’t free it. Thus, for short-lived programs, leaking memory often does not cause any
operational problems (though it may be considered poor form). 

In the long run, one of your goals as a programmer is to develop good habits; one of those habits is understanding how you are managing memory, and (in languages like C), freeing the blocks you have allocated. Even if you can get away with not doing so, it is probably good to get in the habit of freeing each and every byte you explicitly allocate. When you write a long-running server (such as a web server or database management system, which never exit), leaked memory is a much bigger issue, and will eventually lead to a crash when the application runs out of memory. And of course, leaking memory is an even larger issue inside one particular program: the operating system itself. Showing us once again: those who write the kernel code have the toughest job of all...

#### Free Memory Before You Are Done With It

Sometimes a program will free memory before it is finished using it; such a mistake is called a **dangling pointer**, and it, as you can guess, is also a bad thing.

#### Freeing Memory Repeatedly

Programs also sometimes free memory more than once; this is known as the **double free**. The result of doing so is undefined. 

#### Calling `free()` Incorrectly

One last problem we discuss is the call of `free()` incorrectly. After all, `free()` expects you only to pass to it one of the pointers you received from `malloc()` earlier. When you pass in some other value, bad things can (and do) happen. Thus, such **invalid frees** are dangerous and of course should also be avoided.

#### Summary

There are lots of ways to abuse memory, including:

- Forgetting to Allocate Memory
- Not Allocating Enough Memory
- Forgetting to Initialize Allocated Memory
- Forgetting to Free Memory
- Freeing Memory before You are Done with It
- Freeing Memory Repeatedly
- Calling `free()` Incorrectly

a whole ecosphere of tools have developed to help find such problems in your code. Check out both **purify** and **valgrind**. Both are excellent at helping you locate the source of your memory-related problems.

### 14.5 Underlying OS Support

`malloc()` and `free()` are not system calls, but rather library calls. Thus the malloc library manages space within your virtual address space, but itself is built on top of some system calls which call into the OS to ask for more memory or release some back to the system.

One such system call is `brk`, which is used to change the location of the program’s break: the location of the end of the heap. It takes one argument (the address of the new break), and thus either increases or decreases the size of the heap based on whether the new break is larger or smaller than the current break. An additional call `sbrk` is passed an increment but otherwise serves a similar purpose.

Note that you should never directly call either `brk` or `sbrk`. They are used by the memory-allocation library; if you try to use them, you will likely make something go (horribly) wrong. Stick to `malloc()` and `free()` instead.

Finally, you can also obtain memory from the operating system via the `mmap()` call. By passing in the correct arguments, `mmap()` can create an **anonymous** memory region within your program — a region which is not associated with any particular file but rather with **swap space**, something we’ll discuss in detail later on in virtual memory. This memory can then also be treated like a heap and managed as such.

### 14.6 Other Calls

There are a few that the memory-allocation library supports. For example, `calloc()` allocates memory and also zeroes it before returning, which can prevents some errors where you assume that memory is zeroed and forget to initialize it yourself; `realloc()` makes a new larger region of memory, copies the old region into it, and returns the pointer to the new region.

### 14.7 Summary

We have introduced some of the APIs dealing with memory allocation. As always, we have just covered the basics; more details are available elsewhere.
