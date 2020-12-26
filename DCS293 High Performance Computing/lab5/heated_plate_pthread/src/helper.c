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