#include <stdio.h>
#include <stdlib.h>

struct my_vector {
    int *data;
    int size;
    int capacity;
};

void init_vector(struct my_vector *vec);
void free_vector(struct my_vector *vec);
void push_back(struct my_vector *vec, int val);
int pop_back(struct my_vector *vec);
void print_vector(struct my_vector *vec);

int main()
{
    struct my_vector vec;
    struct my_vector *p_vec = &vec;

    init_vector(p_vec);

    push_back(p_vec, 1);
    print_vector(p_vec);

    printf("push: 2\n");
    push_back(p_vec, 2);
    printf("push: 3\n");
    push_back(p_vec, 3);
    print_vector(p_vec);
    printf("push: 4\n");
    push_back(p_vec, 4);
    printf("push: 5\n");
    push_back(p_vec, 5);
    printf("push: 6\n");
    push_back(p_vec, 6);
    print_vector(p_vec);

    printf("pop: %d\n", pop_back(p_vec));
    printf("pprint_vector(p_vec);op: %d\n", pop_back(p_vec));
    print_vector(p_vec);
    printf("pop: %d\n", pop_back(p_vec));
    printf("pop: %d\n", pop_back(p_vec));
    print_vector(p_vec);
    

    free_vector(p_vec);
    return 0;
}

void init_vector(struct my_vector *vec)
{
    vec->data = (int *) malloc(sizeof(int));
    vec->size = 0;
    vec->capacity = 1;
}

void free_vector(struct my_vector *vec)
{
    free(vec->data);
    vec->size = 0;
    vec->capacity = 0;
}

void push_back(struct my_vector *vec, int val)
{
    if (vec->size == vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
    }
    vec->data[vec->size] = val;
    vec->size++;
}

int pop_back(struct my_vector *vec)
{
    int ret = vec->data[(vec->size)-1];
    vec->size--;
    if (vec->size <= vec->capacity / 4) {
        vec->capacity /= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
    }
    return ret;
}

void print_vector(struct my_vector *vec)
{
    printf("\n*** size = %d, capacity = %d ***\n", vec->size, vec->capacity);
    printf("The data is as follows:\n");
    for (int i = 0; i < vec->size; i++) {
        printf("%d  ", vec->data[i]);
    }
    printf("\n***\n\n");
}
