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
