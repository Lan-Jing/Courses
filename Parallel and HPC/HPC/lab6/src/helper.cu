#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>
#include <cuda_runtime.h>

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

int row_begin(int rank, int num_threads, int size)
{
    int blockSize = size/num_threads, margin = size%num_threads;
    return (blockSize+1)*rank - Max(rank-margin, 0);
}

int row_end(int rank, int num_threads, int size)
{
    int blockSize = size/num_threads, margin = size%num_threads;
    return row_begin(rank, num_threads, size) + blockSize + (rank < margin);
}

// check if there's error when executing the last step.
void CUDA_CHECK_LAST()
{
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) 
    printf("Error: %s\n", cudaGetErrorString(err));
}