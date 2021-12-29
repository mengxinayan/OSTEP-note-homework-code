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
