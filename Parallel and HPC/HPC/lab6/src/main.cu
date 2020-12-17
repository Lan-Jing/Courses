#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#include "../include/matmul.cuh"
#include "../include/helper.hpp"

#define testRounds 10
#define num_threads 4
// #define DEBUG

int main(int argc, char *argv[])
{
	init_rand();

	int m, n, k;
	m = n = k = strtol(argv[1], NULL, 10);
	printf("Conducting Multiplication of two matricies with size %d*%d\n", m, n);
	cudaMat *mata = init_mat(m, n, 1, colMajor),
			*matb = init_mat(n, k, 1, colMajor);

	// Likely useful for double-precision operations.
	cudaDeviceSetSharedMemConfig(cudaSharedMemBankSizeEightByte);

	for(int round = 0;round < testRounds;round++) {
		printf("Testing Round %d:\n", round);

		// cudaMat *omp_matc = matmul_omp(mata, matb);
		cudaMat *cuda_matc = matmul_cuda(mata, matb);
		cudaMat *ompCuda_matc = matmul_cuda_multiDevices(mata, matb, num_threads);
		cudaMat *cublas_matc = matmul_cuBLAS(mata, matb, 1);

	#ifdef DEBUG
		// print_mat(omp_matc);
		print_mat(cuda_matc);
		print_mat(ompCuda_matc);
		print_mat(cublas_matc);
	#endif
		
		printf("Result check %s\n\n", equal_mat(cuda_matc, cublas_matc) ? "accepted" : "failed");
		
		// free_mat(omp_matc);
		free_mat(cuda_matc);
		free_mat(ompCuda_matc);
		free_mat(cublas_matc);
	}
	// printf("Average execution time: %lf\n", avgTime_omp/testRounds);
	printf("Average execution time on GPU: %lf\n", avgTime_cuda/testRounds);
	printf("Average execution time on %d GPUS: %lf\n", num_threads, avgTime_ompCuda/testRounds);
	printf("Average execution time of cuBLAS, on %d GPUS: %lf\n", num_threads, avgTime_cublas/testRounds);
	return 0;
}