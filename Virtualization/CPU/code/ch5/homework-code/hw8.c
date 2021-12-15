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
