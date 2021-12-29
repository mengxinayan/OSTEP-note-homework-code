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
