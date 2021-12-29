#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    int loops = 1000;
    int fd = open("tmp.txt", O_RDONLY);
    if (fd == -1) {
        printf("open() failed\n");
    }

    struct timeval start_timeval;
    struct timeval end_timeval;

    gettimeofday(&start_timeval, NULL);
    for (int i = 0; i < loops; i++) {
        read(fd, NULL, 0);
    }
    gettimeofday(&end_timeval, NULL);

    float elapsed = (end_timeval.tv_sec - start_timeval.tv_sec) * 1000000.0
                    + end_timeval.tv_usec - start_timeval.tv_usec;
    printf("One read() system call takes %f ms\n", elapsed / loops);

    return 0;
}
