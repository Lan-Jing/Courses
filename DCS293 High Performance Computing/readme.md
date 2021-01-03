# Laboratory Homework for DCS293 High Performance Computing.

* Lab1: Basic GEMM optimization
  * Strassen's recursive multiplication
  * OpenMP parallelism
  * 8*8 tiling
  * AVX2 vectorization.
* Lab2: Multi-nodes parallelism based on Lab1
  * MPI send/recv operations
  * Group communication: scatter/gather
* Lab3: Sample programs with Pthread library.
  * Data parallelism 
  * Task parallelism
* Lab4: Simulating OpenMP loop factorization with Pthread.
  * Pack it up as lib file parallel_for.so
  * Call it from GEMM source to execute in parallel
* Lab5: Pthread/MPI implementation for a heated-plate simulation.
  * Use the lib generated in Lab4
  * Adapt MPI to reduce memory usage and communication overhead
  * Measure memory usage with Valgrind
* Lab6: GEMM with CUDA
  * Transpose the matrix
  * with shared memory
  * 1*8 tiling 
  * Scheduling 4 Tesla V100 with 4 OpenMP threads
* Lab7: Convolution with CUDA
  * padding
  * with shared memory
  * solve it via im2col + GEMM
  * cudNN