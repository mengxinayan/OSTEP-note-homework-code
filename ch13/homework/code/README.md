# Chapter 13. The Abstraction: Address Space Homework and Solution (Code)

## Homework (Code)

In this homework, we’ll just learn about a few useful tools to examine virtual memory usage on Linux-based systems. This will only be a brief hint at what is possible; you’ll have to dive deeper on your own to truly become an expert (as always!).

## Solution

### 13.1

> 1. The first Linux tool you should check out is the very simple tool `free`. First, type `man free` and read its entire manual page; it’s short, don’t worry!

Omitted. You can run `man free` to read `free` manual or visit the following website: [https://man7.org/linux/man-pages/man1/free.1.html](https://man7.org/linux/man-pages/man1/free.1.html)

### 13.2

> 2. Now, run `free`, perhaps using some of the arguments that might be useful (e.g., `-m`, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?

Here is output at my computer:

```console
$ free -m    
               total        used        free      shared  buff/cache   available
Mem:           31857        3584       23402         581        4869       27234
Swap:           2048           0        2048

$ free -h
               total        used        free      shared  buff/cache   available
Mem:            31Gi       3.5Gi        22Gi       579Mi       4.8Gi        26Gi
Swap:          2.0Gi          0B       2.0Gi
```

Total: 31857MB

Free: 23402MB

### 13.3

> 3. Next, create a little program that uses a certain amount of memory, called `memory-user.c`. This program should take one command-line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.

Here is [`memory-user.c` code](./code/memory-user.c) 

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: memory-user <memory(KB)> <time(s)>\n");
        exit(EXIT_FAILURE);
    }

    printf("The PID is: %d", getpid());

    unsigned int length = atoi(argv[1]) * 1024 * 1024 / 4;
    unsigned int run_time = atoi(argv[2]);
    
    int *arr = malloc(length * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "malloc() failed, try smaller memory requirements\n");
        exit(EXIT_FAILURE);
    }
    clock_t start = clock();
    double elapsed = 0;

    while (1) {
        elapsed = (double) (clock() - start) / CLOCKS_PER_SEC;
        if (elapsed > run_time) {
            printf("Time is over!\n");
            break;
        }
        for (int i = 0; i < length; i++) {
            arr[i]++;
        }
    }

    free(arr);
    return 0;
}
```

### 13.4

> 4. Now, while running your `memory-user` program, also (in a different terminal window, but on the same machine) run the `free` tool. How do the memory usage totals change when your program is running? How about when you kill the `memory-user` program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

Before run the `memory-user` program, The memory usage of my system is as follows: 

```console
$ free -m
               total        used        free      shared  buff/cache   available
Mem:           31857        4469       22244         710        5143       26221
Swap:           2048           0        2048
```

When I run `./memory-user 100 100`, the memory usage of my system is as follows:

```console
$ free -m                                 
               total        used        free      shared  buff/cache   available
Mem:           31857        4507       22209         706        5139       26186
Swap:           2048           0        2048
```

When I kill `./memory-user 100 100`, the memory usage of my system is as follows:

```console
$ free -m
               total        used        free      shared  buff/cache   available
Mem:           31857        4449       22266         707        5141       26243
Swap:           2048           0        2048
```

When I try to alloc too amounts of memory, the program will failed.

```console
$ ./memory-user 123456789 100
[1]    18229 segmentation fault (core dumped)  ./memory-user 123456789 100
```

### 13.5

> 5. Let’s try one more tool, known as `pmap`. Spend some time, and read the `pmap` manual page in detail.

Omitted. Note that you must know the process ID(PID) before running `pmap`. You can run `man pmap` to read `pmap` manual or visit the following website: [https://man7.org/linux/man-pages/man1/pmap.1.html](https://man7.org/linux/man-pages/man1/pmap.1.html). 

### 13.6

> 6. To use `pmap`, you have to know the **process ID** of the process you’re interested in. Thus, first run `ps auxw` to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your `memory-user` program in this case (indeed, you can even have that program call `getpid()` and print out its PID for your convenience).

First, run `./memory-user 100 100`, then run `pmap PID`, the output is as follows:

```console
$ ./memory-user 100 100
The PID is: 21655
...

# Run following command in another shell.

$ pmap 21655
21655:   ./memory-user 100 100
Address           Kbytes     RSS     PSS   Dirty    Swap Mode  Mapping
0000000000400000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4       4       4       0       0 r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4       4       4       4       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4       4       4       4       0 rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000001eb1000     132       4       4       4       0 rw-p-   [ anon ]
00007f4180087000  102416  102412  102412  102412       0 rw-p-   [ anon ]
00007f418648b000     252     252       1       0       0 r--p- /usr/lib64/libc.so.6
00007f41864ca000    1544     920       6       0       0 r-xp- /usr/lib64/libc.so.6
00007f418664c000     336     128       0       0       0 r--p- /usr/lib64/libc.so.6
00007f41866a0000       4       0       0       0       0 ---p- /usr/lib64/libc.so.6
00007f41866a1000      12      12      12      12       0 r--p- /usr/lib64/libc.so.6
00007f41866a4000      36      36      36      36       0 rw-p- /usr/lib64/libc.so.6
00007f41866ad000      52      20      20      20       0 rw-p-   [ anon ]
00007f41866cd000       8       8       8       8       0 rw-p-   [ anon ]
00007f41866cf000      12      12       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f41866d2000     152     152       0       0       0 r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007f41866f8000      40      40       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f4186702000       8       8       8       8       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f4186704000       8       8       8       8       0 rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007fff19997000     132      16      16      16       0 rw-p-   [ stack ]
00007fff199cb000      16       0       0       0       0 r--p-   [ anon ]
00007fff199cf000       8       4       0       0       0 r-xp-   [ anon ]
ffffffffff600000       4       0       0       0       0 --xp-   [ anon ]
---------------- ------- ------- ------- ------- ------- 
total kB          105192  104052  102551  102532       0
```

### 13.7

> 7. Now run `pmap` on some of these processes, using various flags (like `-X`) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?

Just like homework 13.6, the `pmap` outputs with different flags are as follows:

With `-x` flag:

```
$ pmap -x 22829
22829:   ./memory-user 100 100
0000000000400000      4K r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000      4K r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000      4K r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000      4K r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000      4K rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000550000    132K rw-p-   [ anon ]
00007f10cc3bd000 102416K rw-p-   [ anon ]
00007f10d27c1000    252K r--p- /usr/lib64/libc.so.6
00007f10d2800000   1544K r-xp- /usr/lib64/libc.so.6
00007f10d2982000    336K r--p- /usr/lib64/libc.so.6
00007f10d29d6000      4K ---p- /usr/lib64/libc.so.6
00007f10d29d7000     12K r--p- /usr/lib64/libc.so.6
00007f10d29da000     36K rw-p- /usr/lib64/libc.so.6
00007f10d29e3000     52K rw-p-   [ anon ]
00007f10d2a03000      8K rw-p-   [ anon ]
00007f10d2a05000     12K r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a08000    152K r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a2e000     40K r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a38000      8K r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a3a000      8K rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007ffcaea46000    132K rw-p-   [ stack ]
00007ffcaeb31000     16K r--p-   [ anon ]
00007ffcaeb35000      8K r-xp-   [ anon ]
ffffffffff600000      4K --xp-   [ anon ]
 total           105192K
```

With `-d` flag:

```console
$ pmap -d 22829
22829:   ./memory-user 100 100
Address           Kbytes Mode  Offset           Device    Mapping
0000000000400000       4 r--p- 0000000000000000 008:00001 /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4 r-xp- 0000000000001000 008:00001 /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4 r--p- 0000000000002000 008:00001 /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4 r--p- 0000000000002000 008:00001 /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4 rw-p- 0000000000003000 008:00001 /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000550000     132 rw-p- 0000000000000000 000:00000   [ anon ]
00007f10cc3bd000  102416 rw-p- 0000000000000000 000:00000   [ anon ]
00007f10d27c1000     252 r--p- 0000000000000000 000:00026 /usr/lib64/libc.so.6
00007f10d2800000    1544 r-xp- 000000000003f000 000:00026 /usr/lib64/libc.so.6
00007f10d2982000     336 r--p- 00000000001c1000 000:00026 /usr/lib64/libc.so.6
00007f10d29d6000       4 ---p- 0000000000215000 000:00026 /usr/lib64/libc.so.6
00007f10d29d7000      12 r--p- 0000000000215000 000:00026 /usr/lib64/libc.so.6
00007f10d29da000      36 rw-p- 0000000000218000 000:00026 /usr/lib64/libc.so.6
00007f10d29e3000      52 rw-p- 0000000000000000 000:00000   [ anon ]
00007f10d2a03000       8 rw-p- 0000000000000000 000:00000   [ anon ]
00007f10d2a05000      12 r--p- 0000000000000000 000:00026 /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a08000     152 r-xp- 0000000000003000 000:00026 /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a2e000      40 r--p- 0000000000029000 000:00026 /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a38000       8 r--p- 0000000000032000 000:00026 /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a3a000       8 rw-p- 0000000000034000 000:00026 /usr/lib64/ld-linux-x86-64.so.2
00007ffcaea46000     132 rw-p- 0000000000000000 000:00000   [ stack ]
00007ffcaeb31000      16 r--p- 0000000000000000 000:00000   [ anon ]
00007ffcaeb35000       8 r-xp- 0000000000000000 000:00000   [ anon ]
ffffffffff600000       4 --xp- 0000000000000000 000:00000   [ anon ]
mapped: 105192K    writeable/private: 102788K    shared: 0K
```

With `-q` flag:

```
$ pmap -q 22829
22829:   ./memory-user 100 100
0000000000400000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4       4       4       0       0 r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4       4       4       4       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4       4       4       4       0 rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000550000     132       4       4       4       0 rw-p-   [ anon ]
00007f10cc3bd000  102416  102412  102412  102412       0 rw-p-   [ anon ]
00007f10d27c1000     252     252       1       0       0 r--p- /usr/lib64/libc.so.6
00007f10d2800000    1544     960       7       0       0 r-xp- /usr/lib64/libc.so.6
00007f10d2982000     336     184       1       0       0 r--p- /usr/lib64/libc.so.6
00007f10d29d6000       4       0       0       0       0 ---p- /usr/lib64/libc.so.6
00007f10d29d7000      12      12      12      12       0 r--p- /usr/lib64/libc.so.6
00007f10d29da000      36      36      36      36       0 rw-p- /usr/lib64/libc.so.6
00007f10d29e3000      52      20      20      20       0 rw-p-   [ anon ]
00007f10d2a03000       8       8       8       8       0 rw-p-   [ anon ]
00007f10d2a05000      12      12       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a08000     152     152       1       0       0 r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a2e000      40      40       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a38000       8       8       8       8       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f10d2a3a000       8       8       8       8       0 rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007ffcaea46000     132      16      16      16       0 rw-p-   [ stack ]
00007ffcaeb31000      16       0       0       0       0 r--p-   [ anon ]
00007ffcaeb35000       8       4       0       0       0 r-xp-   [ anon ]
ffffffffff600000       4       0       0       0       0 --xp-   [ anon ]
```

The address space of program includes C library.

### 13.8

> 8. Finally, let’s run `pmap` on your `memory-user` program, with different amounts of used memory. What do you see here? Does the output from `pmap` match your expectations?

When running `./memory-user 100 100`, the `pmap` output is as follows:

```console
$ pmap 24215
24215:   ./memory-user 100 100
Address           Kbytes     RSS     PSS   Dirty    Swap Mode  Mapping
0000000000400000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4       4       4       0       0 r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4       4       4       4       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4       4       4       4       0 rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000926000     132       4       4       4       0 rw-p-   [ anon ]
00007fab9b546000  102416  102412  102412  102412       0 rw-p-   [ anon ]
00007faba194a000     252     252       1       0       0 r--p- /usr/lib64/libc.so.6
00007faba1989000    1544    1052       8       0       0 r-xp- /usr/lib64/libc.so.6
00007faba1b0b000     336     148       1       0       0 r--p- /usr/lib64/libc.so.6
00007faba1b5f000       4       0       0       0       0 ---p- /usr/lib64/libc.so.6
00007faba1b60000      12      12      12      12       0 r--p- /usr/lib64/libc.so.6
00007faba1b63000      36      36      36      36       0 rw-p- /usr/lib64/libc.so.6
00007faba1b6c000      52      20      20      20       0 rw-p-   [ anon ]
00007faba1b8c000       8       8       8       8       0 rw-p-   [ anon ]
00007faba1b8e000      12      12       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007faba1b91000     152     152       1       0       0 r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007faba1bb7000      40      40       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007faba1bc1000       8       8       8       8       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007faba1bc3000       8       8       8       8       0 rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007ffeba466000     132      16      16      16       0 rw-p-   [ stack ]
00007ffeba557000      16       0       0       0       0 r--p-   [ anon ]
00007ffeba55b000       8       4       0       0       0 r-xp-   [ anon ]
ffffffffff600000       4       0       0       0       0 --xp-   [ anon ]
---------------- ------- ------- ------- ------- ------- 
total kB          105192  104204  102555  102532       0
```

When running `./memory-user 200 100`, the `pmap` output is as follows:

```console
$ pmap 24583
24583:   ./memory-user 200 100
Address           Kbytes     RSS     PSS   Dirty    Swap Mode  Mapping
0000000000400000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4       4       4       0       0 r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4       4       4       4       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4       4       4       4       0 rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000001f6e000     132       4       4       4       0 rw-p-   [ anon ]
00007f2f2d782000  204816  204812  204812  204812       0 rw-p-   [ anon ]
00007f2f39f86000     252     252       1       0       0 r--p- /usr/lib64/libc.so.6
00007f2f39fc5000    1544    1004       7       0       0 r-xp- /usr/lib64/libc.so.6
00007f2f3a147000     336     164       1       0       0 r--p- /usr/lib64/libc.so.6
00007f2f3a19b000       4       0       0       0       0 ---p- /usr/lib64/libc.so.6
00007f2f3a19c000      12      12      12      12       0 r--p- /usr/lib64/libc.so.6
00007f2f3a19f000      36      36      36      36       0 rw-p- /usr/lib64/libc.so.6
00007f2f3a1a8000      52      20      20      20       0 rw-p-   [ anon ]
00007f2f3a1c8000       8       8       8       8       0 rw-p-   [ anon ]
00007f2f3a1ca000      12      12       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f2f3a1cd000     152     152       1       0       0 r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007f2f3a1f3000      40      40       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f2f3a1fd000       8       8       8       8       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f2f3a1ff000       8       8       8       8       0 rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007ffce0472000     132      12      12      12       0 rw-p-   [ stack ]
00007ffce050c000      16       0       0       0       0 r--p-   [ anon ]
00007ffce0510000       8       4       0       0       0 r-xp-   [ anon ]
ffffffffff600000       4       0       0       0       0 --xp-   [ anon ]
---------------- ------- ------- ------- ------- ------- 
total kB          207592  206568  204950  204928       0
```

When running `./memory-user 400 100`, the `pmap` output is as follows:

```console
$ pmap 24692
24692:   ./memory-user 400 100
Address           Kbytes     RSS     PSS   Dirty    Swap Mode  Mapping
0000000000400000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000401000       4       4       4       0       0 r-xp- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000402000       4       4       4       0       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000403000       4       4       4       4       0 r--p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000000404000       4       4       4       4       0 rw-p- /home/mengxinayan/code/OSTEP-note-homework-code/ch13/homework/code/code/memory-user
0000000001db6000     132       4       4       4       0 rw-p-   [ anon ]
00007f555b105000  409616  409612  409612  409612       0 rw-p-   [ anon ]
00007f5574109000     252     252       1       0       0 r--p- /usr/lib64/libc.so.6
00007f5574148000    1544    1056       8       0       0 r-xp- /usr/lib64/libc.so.6
00007f55742ca000     336     152       1       0       0 r--p- /usr/lib64/libc.so.6
00007f557431e000       4       0       0       0       0 ---p- /usr/lib64/libc.so.6
00007f557431f000      12      12      12      12       0 r--p- /usr/lib64/libc.so.6
00007f5574322000      36      36      36      36       0 rw-p- /usr/lib64/libc.so.6
00007f557432b000      52      20      20      20       0 rw-p-   [ anon ]
00007f557434b000       8       8       8       8       0 rw-p-   [ anon ]
00007f557434d000      12      12       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f5574350000     152     152       1       0       0 r-xp- /usr/lib64/ld-linux-x86-64.so.2
00007f5574376000      40      40       0       0       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f5574380000       8       8       8       8       0 r--p- /usr/lib64/ld-linux-x86-64.so.2
00007f5574382000       8       8       8       8       0 rw-p- /usr/lib64/ld-linux-x86-64.so.2
00007fff19975000     132      12      12      12       0 rw-p-   [ stack ]
00007fff199f7000      16       0       0       0       0 r--p-   [ anon ]
00007fff199fb000       8       4       0       0       0 r-xp-   [ anon ]
ffffffffff600000       4       0       0       0       0 --xp-   [ anon ]
---------------- ------- ------- ------- ------- ------- 
total kB          412392  411408  409751  409728       0
```

From a series of `pmap` outputs, it shows that the memory usage is increasing as expected.
