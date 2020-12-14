#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#include "../include/matmul.cuh"
#include "../include/helper.hpp"

#define blockSize 32
#define testRounds 1
// #define DEBUG

int main(int argc, char *argv[])
{
    init_rand();

    int m, n, k;
    m = n = k = strtol(argv[1], NULL, 10);
    printf("Conducting Multiplication to two matricies with size %d*%d\n", m, n);
    cudaMat *mata = init_mat(m, n, 1, colMajor),
            *matb = init_mat(n, k, 1, colMajor);

    for(int round = 0;round < testRounds;round++) {
        printf("Testing Round %d:\n", round);

        // cudaMat *omp_matc = matmul_omp(mata, matb);
        cudaMat *cuda_matc = matmul_cuda(mata, matb, blockSize);
        cudaMat *ompCuda_matc = matmul_cuda_multiDevices(mata, matb, blockSize, 4);
    
    #ifdef DEBUG
        print_mat(omp_matc);
        print_mat(cuda_matc);
        print_mat(ompCuda_matc);
    #endif
        
        printf("Result check %s\n\n", equal_mat(ompCuda_matc, cuda_matc) ? "accepted" : "failed");
        // free_mat(omp_matc);
        free_mat(cuda_matc);
        free_mat(ompCuda_matc);
    }
    return 0;
}