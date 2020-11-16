#include <cstdio>
#include <cmath>
#include "error_checks.h" // Macros CUDA_CHECK and CHECK_ERROR_MSG

int count = 0;

__global__ void vector_add(double *C, const double *A, const double *B, int N)
{
    // Add the kernel code
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // Do not try to access past the allocated memory
    if (idx < N) {
        C[idx] = A[idx] + B[idx];
    }

    // try to access host memory in device kernel
    // cannot pass the compile process: undefined in the kernel code.
    // count = 100;
}


int main(void)
{
    const int N = 20;
    const int ThreadsInBlock = 128;
    double *dA, *dB, *dC;
    double hA[N], hB[N], hC[N];
  
    for(int i = 0; i < N; ++i) {
        hA[i] = (double) i;
        hB[i] = (double) i * i;
    //    printf("%lf %lf\n", hA[i], hB[i]);
    }

    /* 
       Add memory allocations and copies. Wrap your runtime function
       calls with CUDA_CHECK( ) macro
    */
    CUDA_CHECK( cudaMalloc((void**)&dA, sizeof(double)*N) );
    CUDA_CHECK( cudaMalloc((void**)&dB, sizeof(double)*N) );
    CUDA_CHECK( cudaMalloc((void**)&dC, sizeof(double)*N) );
    CUDA_CHECK( cudaMemcpy(dA, hA, sizeof(double)*N, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(dB, hB, sizeof(double)*N, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(dC, hC, sizeof(double)*N, cudaMemcpyHostToDevice) );

    // Note the maximum size of threads in a block
    // One block with 256 threads
    dim3 grid(1, 1, 1), threads(ThreadsInBlock, 1, 1);
    // try to overflow the thread block: nothing changed ???
    // dim3 grid(1, 1, 1), threads((1 << 16), (1 << 16), (1 << 16));

    // try to dereference a pointer to device memory
    // the program crashed, most likely by accessing invalid address.
    // *dC = 100;

    //// Add the kernel call here
    vector_add<<<grid, threads>>>(dC, dA, dB, N);

    // Here we add an explicit synchronization so that we catch errors
    // as early as possible. Don't do this in production code!
    // cudaDeviceSynchronize();
    CHECK_ERROR_MSG("vector_add kernel");

    //// Copy back the results and free the device memory
    CUDA_CHECK( cudaMemcpy(hC, dC, sizeof(double)*N, cudaMemcpyDeviceToHost) );
    CUDA_CHECK( cudaFree(dA) );
    CUDA_CHECK( cudaFree(dB) );
    CUDA_CHECK( cudaFree(dC) );    

    for (int i = 0; i < N; i++)
        printf("%5.1f\n", hC[i]);

    return 0;
}