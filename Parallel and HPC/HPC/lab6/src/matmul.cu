#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include "../include/matmul.cuh"
#include "../include/helper.hpp"

// the cuda matricies are stored in the column major order.
// and are unfolded into 1-dimensional arrays
cudaMat* init_mat(int m, int n, int mode, matType matType) 
{
    if(m < 0 || n < 0)
        return NULL;
    
    cudaMat *mat = (cudaMat*)malloc(sizeof(cudaMat));
    mat->mat = (vtype)malloc(dsize * m * n);
    mat->m = m, mat->n = n, mat->type = matType;
    if(mode != 0) {
        for(int i = 0;i < m * n;i++)
            mat->mat[i] = rand_float(RAND_RBOUND);
    }
    return mat;
}

void free_mat(cudaMat *mat)
{
    if(mat == NULL)
        return ;
    
    free(mat->mat);
    free(mat);
}

void print_mat(cudaMat *mat)
{
    if(mat == NULL)
        return ;
    
    if(mat->type == rowMajor) {
        for(int rIdx = 0;rIdx < mat->m;rIdx++) {
            for(int cIdx = 0;cIdx < mat->n;cIdx++)
                printf("%lf ", mat->mat[rIdx * mat->n + cIdx]);
            printf("\n");
        }
    } else {
        for(int cIdx = 0;cIdx < mat->n;cIdx++) {
            for(int rIdx = 0;rIdx < mat->m;rIdx++)
                printf("%lf ", mat->mat[cIdx * mat->m + rIdx]);
            printf("\n");
        }
    }
    printf("\n");
}

// This is an extremely cache-unfriendly function, which causes frequent cache miss anyway
cudaMat* alter_majorOrder(cudaMat *mat)
{
    if(mat == NULL)
        return NULL;

    cudaMat *resMat = init_mat(mat->m, mat->n, 0, mat->type == rowMajor ? colMajor : rowMajor);
    if(mat->type == rowMajor) {
        int rIdx;
    #pragma omp parallel for private(rIdx)
        for(rIdx = 0;rIdx < mat->m;rIdx++)
            for(int cIdx = 0;cIdx < mat->n;cIdx++)
                resMat->mat[cIdx * mat->m + rIdx] = mat->mat[rIdx * mat->n + cIdx];
    } else {
        int cIdx;
    #pragma omp parallel for private(cIdx)
        for(cIdx = 0;cIdx < mat->n;cIdx++)
            for(int rIdx = 0;rIdx < mat->m;rIdx++)
                resMat->mat[rIdx * mat->n + cIdx] = mat->mat[cIdx * mat->m + rIdx];
    }

    return resMat;
}

int equal_mat(cudaMat *mata, cudaMat *matb)
{
    if(mata == NULL || matb == NULL || mata->m != matb->m || mata->n != matb->n) 
        return false;
    
    // change order if they're different
    cudaMat *matac = mata;
    if(mata->type != matb->type)
        matac = alter_majorOrder(mata);
    for(int index = 0;index < mata->m * mata->n;index++)
        if(fabs(matac->mat[index]-matb->mat[index]) > eps) {
            printf("Unsatisfied Error: %lf\n", fabs(matac->mat[index]-matb->mat[index]));
            return false;
        }

    if(matac != mata)
        free_mat(matac);
    return true;
}

cudaMat* matmul_omp(cudaMat *mata, cudaMat *matb)
{
    if(mata->n != matb->m)
        return NULL;

    cudaMat *matc = init_mat(mata->m, matb->n, 0, rowMajor);
    cudaMat *mataT = alter_majorOrder(mata);

    double stime = omp_get_wtime();
    int i;
#pragma omp parallel for private(i)
    for(i = 0;i < mata->m;i++)
        for(int j = 0;j < matb->n;j++) {
            float res = 0.0;
            for(int k = 0;k < matb->m;k++)
                res += mataT->mat[i * mata->n + k] * matb->mat[j * matb->m + k];
            matc->mat[i * mata->n + j] = res;    
        }
    printf("Execution time on %d threads CPU: %lf\n", omp_get_num_procs(), omp_get_wtime()-stime);

    free_mat(mataT);
    return matc;
}

cudaMat* matmul_cuda(cudaMat *mata, cudaMat *matb, int blockSize)
{
    if(mata->n != matb->m)
        return NULL;
    
    cudaMat *matc = init_mat(mata->m, matb->n, 0, colMajor);
    cudaMat *matbT = alter_majorOrder(matb);

    vtype d_A, d_B, d_C;
    cudaMalloc(&d_A, mata->m*mata->n * dsize);
    cudaMalloc(&d_B, matb->m*matb->n * dsize);
    cudaMalloc(&d_C, matc->m*matc->n * dsize);

    double stime = omp_get_wtime();
    cudaMemcpy(d_A, mata->mat, mata->m*mata->n * dsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, matbT->mat, matb->m*matb->n * dsize, cudaMemcpyHostToDevice);

    dim3 gridDim((matc->m+blockSize-1)/blockSize, (matc->n+blockSize-1)/blockSize), 
         blockDim(blockSize, blockSize);

    matmul_kernel1<<<gridDim, blockDim>>>(d_A, d_B, d_C, mata->m, mata->n, matb->n);

    cudaDeviceSynchronize();
    cudaMemcpy(matc->mat, d_C, matc->m*matc->n * dsize, cudaMemcpyDeviceToHost);
    printf("Execution time on GPU: %lf\n", omp_get_wtime()-stime);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free_mat(matbT);
    return matc;
}

cudaMat* matmul_cuda_multiDevices(cudaMat *mata, cudaMat *matb, int blockSize, int num_threads)
{
    if(mata->n != matb->m)
        return NULL;
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    if(num_threads < deviceCount) {
        printf("Exceeding available devices, maximum: %d\n", deviceCount);
        return NULL;
    }
    
    cudaMat *matc = init_mat(mata->m, matb->n, 0, colMajor);

    double stime = omp_get_wtime();
    int i;
#pragma omp parallel for private(i)
    for(i = 0;i < num_threads;i++) {
        cudaSetDevice(i);
        int begin = row_begin(i, num_threads, matb->n),
            end = row_end(i, num_threads, matb->n),
            subSize = end-begin;

        vtype d_A, d_B, d_C;
        cudaMalloc(&d_A, mata->m*mata->n * dsize);
        cudaMalloc(&d_B, matb->m*subSize * dsize);
        cudaMalloc(&d_C, matc->m*subSize * dsize);

        cudaMemcpy(d_A, mata->mat, mata->m*mata->n * dsize, cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, matb->mat + matb->m*begin, matb->m*subSize * dsize, cudaMemcpyHostToDevice);

        dim3 gridDim((matc->m+blockSize-1)/blockSize, (subSize+blockSize-1)/blockSize), 
             blockDim(blockSize, blockSize);

        matmul_kernel2<<<gridDim, blockDim>>>(d_A, d_B, d_C, mata->m , mata->n, subSize);

        cudaMemcpy(matc->mat + matc->m*begin, d_C, matc->m*subSize * dsize, cudaMemcpyDeviceToHost);

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }
    printf("Execution time on %d GPUs: %lf\n", num_threads, omp_get_wtime()-stime);

    free_mat(mata);
    free_mat(matb);
    return matc;
}

// Matrix A in column major order, B in row major
__global__ void matmul_kernel1(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
    const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
              cIdx = threadIdx.y + blockIdx.y * blockDim.y,
              index = cIdx * m + rIdx;    
    if(rIdx < m && cIdx < k) {
        float res = 0;
        for(int i = 0;i < n;i++)
            res += d_A[i * m + rIdx] * d_B[i * k + cIdx];
        d_C[index] = res;
    }
}

// Matrix B shoule be in column major order.
__global__ void matmul_kernel2(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
    const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
              cIdx = threadIdx.y + blockIdx.y * blockDim.y,
              index = cIdx * m + rIdx;    
    if(rIdx < m && cIdx < k) {
        float res = 0;
        for(int i = 0;i < n;i++)
            res += d_A[i * m + rIdx] * d_B[cIdx * n + i];
        d_C[index] = res;
    }
}