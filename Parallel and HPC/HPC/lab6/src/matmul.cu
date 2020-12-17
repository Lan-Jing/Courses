#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>
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
	cudaMat *matac = mata->type != matb->type ? alter_majorOrder(mata) : mata;

	double maxError = 0;
	for(int index = 0;index < mata->m * mata->n;index++)
		maxError = fmax(maxError, fabs(matac->mat[index]-matb->mat[index])/matb->mat[index]);

	if(matac != mata)
		free_mat(matac);

	printf("Maximum error: %lf, required: %lf\n", maxError, eps);
	return maxError > eps ? false : true;
}

cudaMat* matmul_omp(cudaMat *mata, cudaMat *matb)
{
	if(mata->n != matb->m)
		return NULL;

	cudaMat *matc = init_mat(mata->m, matb->n, 0, rowMajor);
	cudaMat *mataT = alter_majorOrder(mata);

	double stime = omp_get_wtime(), etime;
	int i;
#pragma omp parallel for private(i)
	for(i = 0;i < mata->m;i++)
		for(int j = 0;j < matb->n;j++) {
			float res = 0.0;
			for(int k = 0;k < matb->m;k++)
				res += mataT->mat[i * mata->n + k] * matb->mat[j * matb->m + k];
			matc->mat[i * mata->n + j] = res;    
		}
	etime = omp_get_wtime();
	avgTime_omp += etime-stime;

	free_mat(mataT);
	return matc;
}

cudaMat* matmul_cuda(cudaMat *mata, cudaMat *matb)
{
	if(mata->n != matb->m)
		return NULL;

	cudaMat *matc = init_mat(mata->m, matb->n, 0, colMajor);
	cudaMat *matbT = alter_majorOrder(matb);

	vtype d_A, d_B, d_C;
	cudaMalloc(&d_A, mata->m*mata->n * dsize);
	cudaMalloc(&d_B, matb->m*matb->n * dsize);
	cudaMalloc(&d_C, matc->m*matc->n * dsize);
	cudaMemcpy(d_A, mata->mat, mata->m*mata->n * dsize, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, matbT->mat, matb->m*matb->n * dsize, cudaMemcpyHostToDevice);
	
	dim3 gridDim((matc->m+blockSize-1)/blockSize, (matc->n+blockSize-1)/blockSize), 
		 blockDim(blockSize, subBlockSize);

	double stime = omp_get_wtime();
	matmul_kernel5<<<gridDim, blockDim>>>(d_A, d_B, d_C, mata->m, mata->n, matb->n);
	cudaDeviceSynchronize();
	avgTime_cuda += omp_get_wtime()-stime;

	cudaMemcpy(matc->mat, d_C, matc->m*matc->n * dsize, cudaMemcpyDeviceToHost);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	
	free_mat(matbT);
	return matc;
}

cudaMat* matmul_cuda_multiDevices(cudaMat *mata, cudaMat *matb, int num_threads)
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

	double stime;
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

		#pragma omp barrior
		if(i == 0) stime = omp_get_wtime();

		matmul_kernel4<<<gridDim, blockDim>>>(d_A, d_B, d_C, mata->m , mata->n, subSize);
		cudaDeviceSynchronize();
		
		#pragma omp barrior
		if(i == 0) avgTime_ompCuda += omp_get_wtime()-stime;

		cudaMemcpy(matc->mat + matc->m*begin, d_C, matc->m*subSize * dsize, cudaMemcpyDeviceToHost);
		cudaFree(d_A);
		cudaFree(d_B);
		cudaFree(d_C);
	}
	return matc;
}

cudaMat* matmul_cuBLAS(cudaMat *mata, cudaMat *matb, int num_threads)
{
	if(mata->n != matb->m)
		return NULL;
	int deviceCount;
	cudaGetDeviceCount(&deviceCount);
	if(num_threads > deviceCount) {
		printf("Exceeding available devices, maximum: %d\n", deviceCount);
		return NULL;
	}
	
	cudaMat *matc = init_mat(mata->m, matb->n, 0, colMajor);

	double stime;
	int i;
#pragma omp parallel for private(i)
	for(i = 0;i < num_threads;i++) {
		cublasHandle_t handle_i;
		cudaSetDevice(i);
		cublasCreate(&handle_i);

		int begin = row_begin(i, num_threads, matb->n),
			end = row_end(i, num_threads, matb->n),
			subSize = end-begin;

		vtype d_A, d_B, d_C;
		cudaMalloc(&d_A, mata->m*mata->n * dsize);
		cudaMalloc(&d_B, matb->m*subSize * dsize);
		cudaMalloc(&d_C, matc->m*subSize * dsize);

		cudaMemcpy(d_A, mata->mat, mata->m*mata->n * dsize, cudaMemcpyHostToDevice);
		cudaMemcpy(d_B, matb->mat + matb->m*begin, matb->m*subSize * dsize, cudaMemcpyHostToDevice);

		#pragma omp barrior
		if(i == 0) stime = omp_get_wtime();

		dtype alpha = 1.0, beta = 0.0;
		cublasDgemm(handle_i, 
					CUBLAS_OP_N, 
					CUBLAS_OP_N, 
					mata->m, subSize, mata->n, 
					&alpha, 
					d_A, mata->m,
					d_B, matb->m,
					&beta, 
					d_C, matc->m);

		cudaDeviceSynchronize();
		#pragma omp barrior
		if(i == 0) avgTime_cublas += omp_get_wtime()-stime;

		cudaMemcpy(matc->mat + matc->m*begin, d_C, matc->m*subSize * dsize, cudaMemcpyDeviceToHost);
		cudaFree(d_A);
		cudaFree(d_B);
		cudaFree(d_C);
		cublasDestroy(handle_i);
	}
	return matc;
}

// Matrix A in column-major order, B in row-major
__global__ void matmul_kernel1(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
	const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
			  cIdx = threadIdx.y + blockIdx.y * blockDim.y,
			  index = cIdx * m + rIdx;
	dtype res = 0;
	for(int i = 0;i < n;i++)
		res += d_A[i * m + rIdx] * d_B[i * k + cIdx];
	if(rIdx < m && cIdx < k) 
		d_C[index] = res;
}

// In this kernel A and B should both be in column-major order
__global__ void matmul_kernel2(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
	const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
			  cIdx = threadIdx.y + blockIdx.y * blockDim.y,
			  index = cIdx * m + rIdx;    
	dtype res = 0;
	for(int i = 0;i < n;i++)
		res += d_A[i * m + rIdx] * d_B[cIdx * n + i];
	if(rIdx < m && cIdx < k) 
		d_C[index] = res;
}

// Using shared memory based on kernel 1.
// Note that shared memory is managed in a row-major order.
__global__ void matmul_kernel3(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
	const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
			  cIdx = threadIdx.y + blockIdx.y * blockDim.y,
			  index = cIdx * m + rIdx;

	dtype res = 0;
	__shared__ dtype sd_A[blockSize+1][blockSize+1], 
					 sd_B[blockSize+1][blockSize+1];
	for(int bIdx = 0;bIdx < n;bIdx += blockSize) {
		sd_A[threadIdx.y][threadIdx.x] = (rIdx < m && bIdx+threadIdx.y < n) 
									   ? d_A[(bIdx+threadIdx.y) * m + rIdx]
									   : 0;
		sd_B[threadIdx.y][threadIdx.x] = (cIdx < k && bIdx+threadIdx.x < n) 
									   ? d_B[(bIdx+threadIdx.x) * k + cIdx]
									   : 0;
		__syncthreads();
		for(int i = 0;i < blockSize;i++)
			res += sd_A[i][threadIdx.x] * sd_B[threadIdx.y][i];
		__syncthreads();
	}
	if(rIdx < m && cIdx < k) 
		d_C[index] = res;
}

// Based on kernel 2, assuming matrix B in row-major order.
__global__ void matmul_kernel4(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
	const int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
			  cIdx = threadIdx.y + blockIdx.y * blockDim.y,
			  index = cIdx * m + rIdx;    

	dtype res = 0;
	__shared__ dtype sd_A[blockSize+1][blockSize+1], 
					 sd_B[blockSize+1][blockSize+1];
	for(int bIdx = 0;bIdx < n;bIdx += blockSize) {
		sd_A[threadIdx.y][threadIdx.x] = (rIdx < m && bIdx+threadIdx.y < n) 
									   ? d_A[(bIdx+threadIdx.y) * m + rIdx]
									   : 0;
		sd_B[threadIdx.y][threadIdx.x] = (cIdx < k && bIdx+threadIdx.x < n) 
									   ? d_B[cIdx * n + bIdx+threadIdx.x]
									   : 0;
		__syncthreads();
		for(int i = 0;i < blockSize;i++)
			res += sd_A[i][threadIdx.x] * sd_B[threadIdx.y][i];
		__syncthreads();
	}
	if(rIdx < m && cIdx < k) 
		d_C[index] = res;
}

// more work per thread.
__global__ void matmul_kernel5(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k)
{
	const int b_rIdx = threadIdx.x, 
			  b_cIdx = threadIdx.y,
			  rIdx = b_rIdx + blockIdx.x * blockSize,
			  cIdx = b_cIdx + blockIdx.y * blockSize;
	
	dtype res[elementPerBlock];
	memset(res, 0, elementPerBlock*dsize);
	__shared__ dtype sd_A[blockSize+1][blockSize+1],
					 sd_B[blockSize+1][blockSize+1];

	for(int bIdx = 0;bIdx < n;bIdx += blockSize) {
		const int t_rIdx = bIdx + b_rIdx,
				  t_cIdx = bIdx + b_cIdx;
		for(int i = 0;i < elementPerBlock;i++) {
			sd_A[b_cIdx+i*subBlockSize][b_rIdx] = (rIdx < m && t_cIdx+i*subBlockSize < n)
												? d_A[(t_cIdx+i*subBlockSize) * m + rIdx]
												: 0;
			sd_B[b_rIdx][b_cIdx+i*subBlockSize] = (cIdx+i*subBlockSize < k && t_rIdx < n)
												? d_B[t_rIdx * k + cIdx+i*subBlockSize]
												: 0;
		}
		__syncthreads();
		for(int j = 0;j < blockSize;j++)
			for(int i = 0;i < elementPerBlock;i++) 
				res[i] += sd_A[j][b_rIdx] * sd_B[j][b_cIdx+i*subBlockSize];
		__syncthreads();
	}
	for(int i = 0;i < elementPerBlock;i++) {
		int index = (cIdx+i*subBlockSize) * m + rIdx;
		if(rIdx < m && cIdx+i*subBlockSize < k)
			d_C[index] = res[i];
	}
}