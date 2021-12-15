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
