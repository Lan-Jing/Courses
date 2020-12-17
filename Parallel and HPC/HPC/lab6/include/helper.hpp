
void init_rand();
float rand_float(float rbound);
int Max(int a, int b);

int row_begin(int rank, int num_threads, int size);
int row_end(int rank, int num_threads, int size);

void CUDA_CHECK_LAST();

extern double avgTime_omp;
extern double avgTime_cuda;
extern double avgTime_ompCuda;
extern double avgTime_cublas;