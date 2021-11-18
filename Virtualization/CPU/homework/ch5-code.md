
1.

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

2. 

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

3. 

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

4.

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

5. 

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
