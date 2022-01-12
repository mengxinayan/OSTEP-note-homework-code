# Chapter 14. Interlude: Memory API Homework and Solution (Code)

## Homework (Code)

In this homework, you will gain some familiarity with memory allocation. First, you’ll write some buggy programs (fun!). Then, you’ll use some tools to help you find the bugs you inserted. Then, you will realize how awesome these tools are and use them in the future, thus making yourself more happy and productive. The tools are the debugger (e.g., `gdb`) and a memory-bug detector called `valgrind` [SN05].

## Solution

### 14.1

> 1. First, write a simple program called `null.c` that creates a pointer to an integer, sets it to `NULL`, and then tries to dereference it. Compile this into an executable called `null`. What happens when you run this program?

The code in [`null.c`](./code/null.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = NULL;
    printf("%d\n", *p);
    return 0;
}
```

When run this program, it will lead to a segmentation fault.

```console
$ gcc null.c -o null    
$ ./null
[1]    19709 segmentation fault (core dumped)  ./null
```

### 14.2

> 2. Next, compile this program with symbol information included (with the `-g` flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing `gdb null` and then, once `gdb` is running, typing `run`. What does gdb show you?

The output is as follows:

```console
$ gcc -g null.c -o null
$ gdb null             

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from null...
(gdb) run
Starting program: /home/mengxinayan/code/OSTEP-note-homework-code/ch14/homework/code/code/null 
Missing separate debuginfos, use: zypper install glibc-debuginfo-2.34-4.2.x86_64
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".

Program received signal SIGSEGV, Segmentation fault.
0x000000000040114a in main () at null.c:7
7           printf("%d\n", *p);
(gdb) 
```

### 14.3

> 3. Finally, use the `valgrind` tool on this program. We’ll use the `memcheck` tool that is a part of `valgrind` to analyze what happens. Run this by typing in the following: `valgrind --leak-check=yes null`. What happens when you run this? Can you interpret the output from the tool?

The output is as follows:

```console
$ valgrind --leak-check=yes ./null 
==21500== Memcheck, a memory error detector
==21500== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==21500== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==21500== Command: ./null
==21500== 
==21500== Invalid read of size 4
==21500==    at 0x40114A: main (null.c:7)
==21500==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==21500== 
==21500== 
==21500== Process terminating with default action of signal 11 (SIGSEGV): dumping core
==21500==  Access not within mapped region at address 0x0
==21500==    at 0x40114A: main (null.c:7)
==21500==  If you believe this happened as a result of a stack
==21500==  overflow in your program's main thread (unlikely but
==21500==  possible), you can try to increase the size of the
==21500==  main thread stack using the --main-stacksize= flag.
==21500==  The main thread stack size used in this run was 8388608.
==21500== 
==21500== HEAP SUMMARY:
==21500==     in use at exit: 0 bytes in 0 blocks
==21500==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==21500== 
==21500== All heap blocks were freed -- no leaks are possible
==21500== 
==21500== For lists of detected and suppressed errors, rerun with: -s
==21500== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
[1]    21500 segmentation fault (core dumped)  valgrind --leak-check=yes ./null
```

p is at Address 0x0, which is not stack'd, malloc'd or (recently) free'd

### 14.4

> 4. Write a simple program that allocates memory using `malloc()` but forgets to free it before exiting. What happens when this program runs? Can you use `gdb` to find any problems with it? How about `valgrind` (again with the `--leak-check=yes` flag)?

The code in [`forget-free.c`](./code/forget-free.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = (int *) malloc(sizeof(int));
    *p = 100;
    printf("%d\n", *p);
    return 0;
}
```

It will run normally without nothing error. 

GDB can not help to find any problems with it:

```console
$ gcc -g forget-free.c -o forget-free
$ gdb forget-free                    

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from forget-free...
(gdb) run
Starting program: /home/mengxinayan/code/OSTEP-note-homework-code/ch14/homework/code/code/forget-free 
Missing separate debuginfos, use: zypper install glibc-debuginfo-2.34-4.2.x86_64
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib64/libthread_db.so.1".
100
[Inferior 1 (process 24310) exited normally]
(gdb) 
```

`valgrind` can find that there is a missing free, the output is as follows:

```console
$ gcc -g forget-free.c -o forget-free
$ valgrind --leak-check=yes ./forget-free 
==24518== Memcheck, a memory error detector
==24518== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==24518== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==24518== Command: ./forget-free
==24518== 
100
==24518== 
==24518== HEAP SUMMARY:
==24518==     in use at exit: 4 bytes in 1 blocks
==24518==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated
==24518== 
==24518== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1
==24518==    at 0x48437B5: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==24518==    by 0x401157: main (forget-free.c:6)
==24518== 
==24518== LEAK SUMMARY:
==24518==    definitely lost: 4 bytes in 1 blocks
==24518==    indirectly lost: 0 bytes in 0 blocks
==24518==      possibly lost: 0 bytes in 0 blocks
==24518==    still reachable: 0 bytes in 0 blocks
==24518==         suppressed: 0 bytes in 0 blocks
==24518== 
==24518== For lists of detected and suppressed errors, rerun with: -s
==24518== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

### 14.5

> 5. Write a program that creates an array of integers called `data` of size 100 using `malloc`; then, set `data[100]` to zero. What happens when you run this program? What happens when you run this program using `valgrind`? Is the program correct?

The code in [`size-100.c`](./code/size-100.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data = (int *) malloc(100 * sizeof(int));
    data[100] = 100;
    free(data);
    return 0;
}
```

No error happens when run this program.

```console
$ gcc -g size-100.c -o size-100
$ ./size-100 
$ 
```

`valgrind` prompts that there is an error. Program is not correct, because `data[100]` is out of `data` bound.

```console
$ gcc -g size-100.c -o size-100
$ valgrind --leak-check=yes ./size-100   
==27178== Memcheck, a memory error detector
==27178== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==27178== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==27178== Command: ./size-100
==27178== 
==27178== Invalid write of size 4
==27178==    at 0x401166: main (size-100.c:7)
==27178==  Address 0x4a9a1d0 is 0 bytes after a block of size 400 alloc'd
==27178==    at 0x48437B5: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==27178==    by 0x401157: main (size-100.c:6)
==27178== 
==27178== 
==27178== HEAP SUMMARY:
==27178==     in use at exit: 0 bytes in 0 blocks
==27178==   total heap usage: 1 allocs, 1 frees, 400 bytes allocated
==27178== 
==27178== All heap blocks were freed -- no leaks are possible
==27178== 
==27178== For lists of detected and suppressed errors, rerun with: -s
==27178== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

### 14.6

> 6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use `valgrind` on it?

The code in [`print-after-free.c`](./code/print-after-free.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data = (int *) malloc(100 * sizeof(int));
    data[0] = -99;
    free(data);
    printf("data[0] = %d\n", data[0]);
    return 0;
}
```

The program runs fine but the the value of data is weird and different at each time, not what the program expected.

```console
$ gcc -g print-after-free.c -o print-after-free
$ ./print-after-free 
data[0] = 2928
$ ./print-after-free
data[0] = 5173
$ ./print-after-free
data[0] = 5651
```

`valgrind` prompts there will be an invalid read, here is its output:

```console
$ gcc -g print-after-free.c -o print-after-free
$ valgrind --leak-check=yes ./print-after-free 
==29564== Memcheck, a memory error detector
==29564== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==29564== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==29564== Command: ./print-after-free
==29564== 
==29564== Invalid read of size 4
==29564==    at 0x401186: main (print-after-free.c:9)
==29564==  Address 0x4a9a040 is 0 bytes inside a block of size 400 free'd
==29564==    at 0x484617B: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==29564==    by 0x401181: main (print-after-free.c:8)
==29564==  Block was alloc'd at
==29564==    at 0x48437B5: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==29564==    by 0x401167: main (print-after-free.c:6)
==29564== 
data[0] = -99
==29564== 
==29564== HEAP SUMMARY:
==29564==     in use at exit: 0 bytes in 0 blocks
==29564==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
==29564== 
==29564== All heap blocks were freed -- no leaks are possible
==29564== 
==29564== For lists of detected and suppressed errors, rerun with: -s
==29564== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

### 14.7

> 7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

The code in [`pass-funny-value-free.c`](./code/pass-funny-value-free.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data = (int *) malloc(100 * sizeof(int));
    data[0] = -99;
    data[50] = 0;
    data[99] = 99;
    free(&data[50]);
    printf("data[0] = %d\n", data[0]);
    printf("data[50] = %d\n", data[50]);
    printf("data[99] = %d\n", data[99]);
    return 0;
}
```

The compiler will prompts a warning when trying to compiling this program. It will say `free(): invalid pointer` then terminate.

```console
$ gcc -g pass-funny-value-free.c -o pass-funny-value-free   
pass-funny-value-free.c: In function ‘main’:
pass-funny-value-free.c:10:5: warning: ‘free’ called on pointer ‘data’ with nonzero offset 200 [-Wfree-nonheap-object]
   10 |     free(&data[50]);
      |     ^~~~~~~~~~~~~~~
pass-funny-value-free.c:6:25: note: returned from ‘malloc’
    6 |     int *data = (int *) malloc(100 * sizeof(int));
      |                         ^~~~~~~~~~~~~~~~~~~~~~~~~

$ ./pass-funny-value-free 
free(): invalid pointer
[1]    30605 IOT instruction (core dumped)  ./pass-funny-value-free
```

`valgrind` will prompts there is an invalid free() too.

```console
$ valgrind --leak-check=yes ./pass-funny-value-free
==31224== Memcheck, a memory error detector
==31224== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==31224== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==31224== Command: ./pass-funny-value-free
==31224== 
==31224== Invalid free() / delete / delete[] / realloc()
==31224==    at 0x484617B: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==31224==    by 0x4011A7: main (pass-funny-value-free.c:10)
==31224==  Address 0x4a9a108 is 200 bytes inside a block of size 400 alloc'd
==31224==    at 0x48437B5: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==31224==    by 0x401167: main (pass-funny-value-free.c:6)
==31224== 
data[0] = -99
data[50] = 0
data[99] = 99
==31224== 
==31224== HEAP SUMMARY:
==31224==     in use at exit: 400 bytes in 1 blocks
==31224==   total heap usage: 2 allocs, 2 frees, 1,424 bytes allocated
==31224== 
==31224== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1
==31224==    at 0x48437B5: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==31224==    by 0x401167: main (pass-funny-value-free.c:6)
==31224== 
==31224== LEAK SUMMARY:
==31224==    definitely lost: 400 bytes in 1 blocks
==31224==    indirectly lost: 0 bytes in 0 blocks
==31224==      possibly lost: 0 bytes in 0 blocks
==31224==    still reachable: 0 bytes in 0 blocks
==31224==         suppressed: 0 bytes in 0 blocks
==31224== 
==31224== For lists of detected and suppressed errors, rerun with: -s
==31224== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

### 14.8

> 8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use `realloc()` to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use `realloc()` to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use `valgrind` to help you find bugs.

The implementation and test code in [`my-vector.c`](./code/my-vector.c) is as follows:

```c
#include <stdio.h>
#include <stdlib.h>

struct my_vector {
    int *data;
    int size;
    int capacity;
};

void init_vector(struct my_vector *vec);
void free_vector(struct my_vector *vec);
void push_back(struct my_vector *vec, int val);
int pop_back(struct my_vector *vec);
void print_vector(struct my_vector *vec);

int main()
{
    struct my_vector vec;
    struct my_vector *p_vec = &vec;

    init_vector(p_vec);

    push_back(p_vec, 1);
    print_vector(p_vec);

    printf("push: 2\n");
    push_back(p_vec, 2);
    printf("push: 3\n");
    push_back(p_vec, 3);
    print_vector(p_vec);
    printf("push: 4\n");
    push_back(p_vec, 4);
    printf("push: 5\n");
    push_back(p_vec, 5);
    printf("push: 6\n");
    push_back(p_vec, 6);
    print_vector(p_vec);

    printf("pop: %d\n", pop_back(p_vec));
    printf("pprint_vector(p_vec);op: %d\n", pop_back(p_vec));
    print_vector(p_vec);
    printf("pop: %d\n", pop_back(p_vec));
    printf("pop: %d\n", pop_back(p_vec));
    print_vector(p_vec);
    

    free_vector(p_vec);
    return 0;
}

void init_vector(struct my_vector *vec)
{
    vec->data = (int *) malloc(sizeof(int));
    vec->size = 0;
    vec->capacity = 1;
}

void free_vector(struct my_vector *vec)
{
    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;
}

void push_back(struct my_vector *vec, int val)
{
    if (vec->size == vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
    }
    vec->data[vec->size] = val;
    vec->size++;
}

int pop_back(struct my_vector *vec)
{
    int ret = vec->data[(vec->size)-1];
    vec->size--;
    if (vec->size <= vec->capacity / 4) {
        vec->capacity /= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
    }
    return ret;
}

void print_vector(struct my_vector *vec)
{
    printf("\n*** size = %d, capacity = %d ***\n", vec->size, vec->capacity);
    printf("The data is as follows:\n");
    for (int i = 0; i < vec->size; i++) {
        printf("%d  ", vec->data[i]);
    }
    printf("\n***\n\n");
}
```

The output of this program is as follows:

```console
$ gcc -g my-vector.c -o my-vector
$ ./my-vector 

*** size = 1, capacity = 1 ***
The data is as follows:
1  
***

push: 2
push: 3

*** size = 3, capacity = 4 ***
The data is as follows:
1  2  3  
***

push: 4
push: 5
push: 6

*** size = 6, capacity = 8 ***
The data is as follows:
1  2  3  4  5  6  
***

pop: 6
pprint_vector(p_vec);op: 5

*** size = 4, capacity = 8 ***
The data is as follows:
1  2  3  4  
***

pop: 4
pop: 3

*** size = 2, capacity = 4 ***
The data is as follows:
1  2  
***
```

The output of using `valgrind` is as follows:

```console
$ valgrind --leak-check=yes ./my-vector            
==5774== Memcheck, a memory error detector
==5774== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==5774== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==5774== Command: ./my-vector
==5774== 

*** size = 1, capacity = 1 ***
The data is as follows:
1  
***

push: 2
push: 3

*** size = 3, capacity = 4 ***
The data is as follows:
1  2  3  
***

push: 4
push: 5
push: 6

*** size = 6, capacity = 8 ***
The data is as follows:
1  2  3  4  5  6  
***

pop: 6
pprint_vector(p_vec);op: 5

*** size = 4, capacity = 8 ***
The data is as follows:
1  2  3  4  
***

pop: 4
pop: 3

*** size = 2, capacity = 4 ***
The data is as follows:
1  2  
***

==5774== 
==5774== HEAP SUMMARY:
==5774==     in use at exit: 0 bytes in 0 blocks
==5774==   total heap usage: 6 allocs, 6 frees, 1,100 bytes allocated
==5774== 
==5774== All heap blocks were freed -- no leaks are possible
==5774== 
==5774== For lists of detected and suppressed errors, rerun with: -s
==5774== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

You can get the performance comparison of array, dynamic array, linked list and so on at [Dynamic array - Wikipedia](https://en.wikipedia.org/wiki/Dynamic_array#Performance)

### 14.9

> 9. Spend more time and read about using `gdb` and `valgrind`. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.

Omitted.
