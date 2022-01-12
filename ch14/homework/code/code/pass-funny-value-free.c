#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data = (int *) malloc(100 * sizeof(int));
    data[0] = -99;
    data[50] = 0;
    data[99] = 99;
    free(&data[50]);
    printf("data[0] = %d\n", data[0]);
    printf("data[50] = %d\n", data[50]);
    printf("data[99] = %d\n", data[99]);
    return 0;
}
