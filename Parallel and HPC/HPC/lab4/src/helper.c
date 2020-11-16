#include <stdlib.h>
#include "../include/helper.h"

int Max(int a, int b)
{
    return a > b ? a : b;
}

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

double float_rand(int r)
{
    return (double)rand() / (double)(RAND_MAX/r);
}