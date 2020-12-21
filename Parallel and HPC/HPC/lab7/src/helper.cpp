#include <time.h>
#include <stdlib.h>
#include "../include/helper.hpp"

void init_rand()
{
    srand(time(0));
}

// construct a float number in [0, rbound]
float rand_float(float rbound) 
{
    return (float)((double)rand()/(double)(RAND_MAX/rbound));
}

int Max(int a, int b)
{
    return a > b ? a : b;
}