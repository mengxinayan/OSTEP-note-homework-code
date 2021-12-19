# ch5 Process API Homework and Solution

If you just want to read the answers, that is a simple version for solutions, please visit [here](../solution.md).

## Homework (code)

In this homework, you are to gain some familiarity with the process management APIs about which you just read. Don’t worry – it’s even more fun than it sounds! You’ll in general be much better off if you find as much time as you can to write some code, so why not start now?

## Questions and Solutions

### 5.1

1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?

`x = 100` in child process. Because child and parent process have their own `x` variable, child process changes to `x` value **do not** affect the value of `x` in the parent process.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int x = 100;
    printf("Init: x = %d\n", x);
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("(child) x = %d\n", x);
        x = 200;
        printf("(child) Now x = %d\n", x);
    } else {
        int rc_wait = wait(NULL);
        printf("(parent) x = %d\n", x);
        x = 50;
        printf("(parent) Now x = %d\n", x);
    }
    return 0;
}
```

### 5.2

2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

Yes, both parent and child process can use this file descriptor. All messages will be written in the file, but the sequence of them are not sure.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        printf("open() failed.\n");
        exit(1);
    }
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        char *s = "child\n";
        write(fd, s, strlen(s));
    } else {
        char *s = "parent\n";
        write(fd, s, strlen(s));
    }
    return 0;
}
```

### 5.3

3. Write another program using `fork()`. The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this *without* calling **`wait()`** in the parent?

Use `sleep()` before print 'goodbye' in parent process.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("hello\n");
    } else {
        sleep(1);
        printf("goodbye\n");
    }
    return 0;
}
```

### 5.4

4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

To fit various demand. The differences between `exec()` family system call are as follow.

`execl*()` V.S. `execv*()`: `execl*()` requires a list of arguments while `execv*()` requires a vector of arguments. Specifically, `execl*()` needs `char *arg1, char *arg2, ..., char *argX`, `execv*()` needs `char *argv[]`. Note that for `execv*()`, the array  of  pointers  **must**  be  terminated  by  a  null pointer.

For suffix `p`, such as `execvp()`, `execlp()`. `p` means it will the `PATH` environment variable to find the program. For example, if you want to use `ls` command, with suffix `p`, use `ls` directly can work well, however, you should use `/bin/ls` rather than `ls` without the suffix `p`.

For suffix `e`, such as `execle()`, `execvpe()`. `e` allows you to specify the environment for the process.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        /**
         * Run `ls -l -a`, note that 
         * `ls` is the first argument, `-l` is the second argument, `-a` is the third argument.
         * For `v`, the array  of  pointers  must  be  terminated  by  a  null
       pointer.
         */
        char *my_argv[] = {"/bin/ls", "-l", "-a", NULL};
        // `l`, list arguments
        // execl(my_argv[0], my_argv[0], my_argv[1], my_argv[2]);
        // execlp("ls", "ls", my_argv[1], my_argv[2]);
        // execle(my_argv[0], my_argv[0], my_argv[1], my_argv[2], NULL); // Environment is NULL.

        // `v`, vector of arguments
        // execv(my_argv[0], my_argv);
        // execvp("ls", my_argv);
        execve(my_argv[0], my_argv, NULL);
    } else {
        int rc_wait = wait(NULL);
        printf("(main) goodbye\n");
    }
    return 0;
}
```

### 5.5

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

`wait()` returns child process identifier(PID). If child process call `wait()`, it will return `-1`. 

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        int child_wait = wait(NULL);
        printf("(child pid = %d) child_wait = %d, hello\n", (int) getpid(), child_wait);
    } else {
        int rc_wait = wait(NULL);
        printf("(parent pid = %d) rc_wait = %d, goodbye\n", (int) getpid(), rc_wait);
    }
    return 0;
}
```

### 5.6

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

The `wait()` system call suspends execution of the calling thread until one of its children terminates. The call `wait(&wstatus)` is equivalent to: `waitpid(-1, &wstatus, 0)`. The `waitpid()` system call suspends execution of the calling thread until a child specified by  pid  argument  has  changed state.  By default, `waitpid()` waits only for terminated children, but this behavior is modifiable via the options argument, as described below.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) {
        printf("fork() failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("(child pid = %d), hello\n", (int) getpid());
    } else {
        int rc_wait = waitpid(0, 0, 0);
        printf("(parent pid = %d) rc_wait = %d, goodbye\n", (int) getpid(), rc_wait);
    }
    return 0;
}
```

### 5.7

7. Write a program that creates a child process, and then in the child
closes standard output (`STDOUT_FILENO`). What happens if the child
calls `printf()` to print some output after closing the descriptor?

After closing stdout file descriptor, it can't print any message by calling `printf()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int rc = fork();
    if (rc < 0) { 
        printf("fork failed\n");
        exit(1);
    } else if (rc == 0) { 
        printf("(child pid = %d), hello\n", (int) getpid());
        // close stdout file
        close(STDOUT_FILENO);
        printf("(child pid = %d), goodbye\n", (int) getpid());
    } else { 
        // parent goes down this path(main)
        int rc_wait = wait(NULL);
        printf("(parent pid = %d) rc_wait = %d, goodbye\n", (int) getpid(), rc_wait);
    }
}
```

### 5.8

8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the `pipe()` system call.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        printf("pipe() failed");
        exit(1);
    }
    int rc1 = fork(); // create child 1
    if (rc1 < 0) { 
        printf("fork failed\n");
        exit(1);
    } else if (rc1 == 0) { 
        close(pipe_fd[0]); // close unused read end;
        char s[100];
        sprintf(s, "(child pid = %d) hello, I'm child 1.", (int) getpid());
        write(pipe_fd[1], &s, strlen(s));
        printf("%s\n", s);
    } else { 
        int rc2 = fork(); // create child2
        if (rc2 < 0) {
            printf("fork failed\n");
            exit(1);
        } else if (rc2 == 0) {
            close(pipe_fd[1]); // close unused write end
            char s[100];
            if (read(pipe_fd[0], &s, sizeof(s)) > 0) {
                printf("Receive message from child1: %s\n", s);
            } else {
                printf("read nothing.\n");
            }
        } else {
            int rc_wait = wait(NULL);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            printf("(parent pid = %d) rc_wait = %d, goodbye\n", (int) getpid(), rc_wait);
        }
    }
}
```
