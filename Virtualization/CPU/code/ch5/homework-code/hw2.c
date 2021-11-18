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
