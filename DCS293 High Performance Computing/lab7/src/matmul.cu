#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

#include "../include/conv.cuh"
#include "../include/helper.hpp"

// vector-matrix multiplication. 1-d image is treated as matrix.
// This trick would be very likely useless on GPU
__constant__ dtype sd_A[16384];
cudaImage* vmm_cuda(cudaImage &mata, cudaImage &matb, int om, int on)
{
    int nCol = mata.size();
    if(nCol != matb.m)
        return NULL;

    cudaImage *matc = new cudaImage(om, on, 1, 0, rowMajor);

    dtype* d_A, *d_B, *d_C;
    cudaMalloc(&d_A, mata.size() * dsize);
    cudaMalloc(&d_B, matb.size() * dsize);
    cudaMalloc(&d_C, matc->size() * dsize);
    cudaMemcpy(d_A, mata.mat, mata.size()*dsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, matb.mat, matb.size()*dsize, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(sd_A, d_A, mata.size()*dsize);

    dim3 blockDim(blockSize, 1),
         gridDim((matb.n+blockSize-1)/blockSize, 1);

    col_stime = omp_get_wtime();
    vmm_kernel<<<gridDim, blockDim>>>(d_A, d_B, d_C, 
                                      matb.m, matb.n);
    cudaDeviceSynchronize();
    col_avgTime += omp_get_wtime()-col_stime;

    cudaMemcpy(matc->mat, d_C, matc->size()*dsize, cudaMemcpyDeviceToHost);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return matc;
}

__global__ void vmm_kernel(vtype d_A, vtype d_B, vtype d_C, 
                           int n, int k)
{
	const int idx = threadIdx.x + blockDim.x * blockIdx.x;

    if(idx < k) {
        dtype res = 0;
        
        for(int i = 0, bIdx = idx;i < n;i++, bIdx += k)
            res += sd_A[i] * d_B[bIdx];

        d_C[idx] = res;
    }
}