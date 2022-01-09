#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: memory-user <memory(KB)> <time(s)>\n");
        exit(EXIT_FAILURE);
    }

    printf("The PID is: %d\n", getpid());

    unsigned int length = atoi(argv[1]) * 1024 * 1024 / 4;
    unsigned int run_time = atoi(argv[2]);
    
    int *arr = malloc(length * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "malloc() failed, try smaller memory requirements\n");
        exit(EXIT_FAILURE);
    }
    clock_t start = clock();
    double elapsed = 0;

    while (1) {
        elapsed = (double) (clock() - start) / CLOCKS_PER_SEC;
        if (elapsed > run_time) {
            printf("Time is over!\n");
            break;
        }
        for (int i = 0; i < length; i++) {
            arr[i]++;
        }
    }

    free(arr);
    return 0;
}
