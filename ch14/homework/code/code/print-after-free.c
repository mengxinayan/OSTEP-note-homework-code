#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *data = (int *) malloc(100 * sizeof(int));
    data[0] = -99;
    free(data);
    printf("data[0] = %d\n", data[0]);
    return 0;
}
