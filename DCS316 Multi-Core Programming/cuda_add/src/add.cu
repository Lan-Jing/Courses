#include <iostream>
using namespace std;
#include <cstdio>
#include <cmath>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>

#include "../include/add.hpp"
#include "../include/helper.hpp"

// a warper for checking cuda function calls
#define gpuCheck(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

// The matrix
matrix::matrix()
{
	height = width = 0;
}

matrix::matrix(const matrix& other)
{
	*this = other;
}

matrix::matrix(int _height, int _width, int _random)
{
	height = _height, width = _width;
	ents   = new MAT_ENTRY_T[_height*_width];

	// fill the matrix with random numbers
	if(_random) {
		for(int i = 0;i < _height;i++)
			for(int j = 0;j < _width;j++)
				(*this)(i, j) = rand(1.0);
	}
}

matrix::~matrix()
{
	if(size() <= 0) return ;
	delete [] ents;
}

int matrix::size() const 
{
	return height * width;
}

void matrix::print() const 
{
	if(size() <= 0) return ;
	for(int i = 0;i < height;i++) {
		for(int j = 0;j < width;j++) 
			printf("%lf ", (*this)(i, j));
		printf("\n");
	}
	printf("\n");
}

MAT_ENTRY_T& matrix::operator () (const int& y, const int& x) const 
{
	return ents[y*width+x];
}

bool matrix::operator == (const matrix& other) const 
{
	if(height != other.height || width != other.width) {
		printf("Sizes of the matrcies don't match.\n");
		return false;
	}
	
	for(int i = 0;i < height;i++)
		for(int j = 0;j < width;j++) {
			double err = abs((*this)(i, j)-other(i, j));
			if(err >= EPS) {
				printf("Check failed at (%d, %d): diff: %lf\n", i, j, err);
				return false;
			}
		}
	return true;
}

// a deep copy method
matrix& matrix::operator = (const matrix& other)
{
	if(!height && !width) {
		this->~matrix();
	}

	height = other.height;
	width  = other.width;

	ents = new MAT_ENTRY_T[height*width];

	for(int i = 0;i < height;i++)
		for(int j = 0;j < width;j++)
			(*this)(i, j) = other(i, j);

	return *this;
}

matrix& matrix::operator += (const matrix& other)
{
	if(height != other.height || width != other.width)
		return *this;

	int i;
	double t = omp_get_wtime();
#pragma omp parallel for
	for(i = 0;i < height;i++)
		for(int j = 0;j < width;j++)
			(*this)(i, j) += other(i, j);
	t = omp_get_wtime() - t;
	add_to_timer(0, 1000*t);

	return *this;
}

void matrix_add(	  matrix& c,
				const matrix& a,
				const matrix& b,
				add_type mode) 
{
	c.~matrix();
	
	const int height = a.height, width = a.width;
	if(a.height != b.height || a.width != b.width) {
		c.height = c.width = 0;
		return ;
	} else if(mode != cpu_add) {
		c.height = height, c.width = width;
		c.ents = new MAT_ENTRY_T[height*width];
	}

	if(mode == cpu_add) {
		c = a;
		c += b;
	} else {
		int size = c.size();
		MAT_ENTRY_T *d_A, *d_B, *d_C;
		gpuCheck( cudaMalloc(&d_C, size*sizeof(MAT_ENTRY_T)) );
		gpuCheck( cudaMalloc(&d_A, size*sizeof(MAT_ENTRY_T)) );
		gpuCheck( cudaMalloc(&d_B, size*sizeof(MAT_ENTRY_T)) );

		cudaEvent_t start, end;
		gpuCheck( cudaEventCreate(&start) );
		gpuCheck( cudaEventCreate(&end) );

		gpuCheck( cudaMemcpy(d_A, a.ents, size*sizeof(MAT_ENTRY_T), cudaMemcpyHostToDevice) );
		gpuCheck( cudaMemcpy(d_B, b.ents, size*sizeof(MAT_ENTRY_T), cudaMemcpyHostToDevice) );
		
		if(mode == gpu_add_1D) {
			constexpr int blockSize = BLOCKSIZE*BLOCKSIZE*WORKLOAD*WORKLOAD;
			dim3 blockDim(BLOCKSIZE*BLOCKSIZE),
				 gridDim((height*width+blockSize-1)/blockSize);

			gpuCheck( cudaEventRecord(start) );
			add_kernel_1D<WORKLOAD*WORKLOAD><<<gridDim, blockDim>>>(d_C, d_A, d_B, height, width);
			gpuCheck( cudaEventRecord(end) );
		} else {
			constexpr int blockSize = BLOCKSIZE*WORKLOAD;
			dim3 blockDim(BLOCKSIZE, BLOCKSIZE),
				 gridDim((height+blockSize-1)/blockSize, (width+blockSize-1)/blockSize);

			gpuCheck( cudaEventRecord(start) );
			add_kernel_2D<WORKLOAD><<<gridDim, blockDim>>>(d_C, d_A, d_B, height, width);
			gpuCheck( cudaEventRecord(end) );
		}
		
		gpuCheck( cudaEventSynchronize(end) );
		gpuCheck( cudaMemcpy(c.ents, d_C, size*sizeof(MAT_ENTRY_T), cudaMemcpyDeviceToHost) );

		float duration = 0;
		gpuCheck( cudaEventElapsedTime(&duration, start, end) );
		add_to_timer(mode, duration);

		gpuCheck( cudaEventDestroy(start) );
		gpuCheck( cudaEventDestroy(end) );

		gpuCheck( cudaFree(d_C) );
		gpuCheck( cudaFree(d_A) );
		gpuCheck( cudaFree(d_B) );
	} 

	return ;
}

template<int workload>
__global__ void add_kernel_1D(MAT_ENTRY_T* d_C,
							  MAT_ENTRY_T* d_A,
							  MAT_ENTRY_T* d_B,
							  const int height, const int width)
{
	const int idx = threadIdx.x + blockIdx.x * blockDim.x * workload;
	
	for(int i = 0;i < workload;i++) {
		int x = idx + blockDim.x*i;
		if(x < height*width) {
			d_C[x] = d_A[x] + d_B[x];
		}
	}
}

template<int workload>
__global__ void add_kernel_2D(MAT_ENTRY_T* d_C, 
						   	  MAT_ENTRY_T* d_A,
						   	  MAT_ENTRY_T* d_B, 
						   	  const int height, const int width)
{
	const int rIdx = threadIdx.x + blockIdx.x * blockDim.x * workload,
			  cIdx = threadIdx.y + blockIdx.y * blockDim.y * workload;

	for(int i = 0;i < workload*workload;i++) {
		int x = rIdx + blockDim.x*(i/workload),
			y = cIdx + blockDim.y*(i%workload),
			idx = x*width + y;
		if(x < height && y < width) {
			d_C[idx] = d_A[idx] + d_B[idx];
		}
	}

	// for(int i = 0;i < workload;i++) {
	// 	for(int j = 0;j < workload;j++) {
	// 		int x = rIdx + blockDim.x*i,
	// 			y = cIdx + blockDim.y*j,
	// 			idx = x*width + y;
	// 		if(x < height && y < width) {
	// 			d_C[idx] = d_A[idx] + d_B[idx];
	// 		}
	// 	}
	// }
}
