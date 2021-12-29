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
