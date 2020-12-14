#define RAND_RBOUND 1.0
#define eps 1e-4
#define dtype double
#define dsize sizeof(dtype)

typedef dtype* vtype;

typedef enum matType {
    rowMajor = 0,
    colMajor = 1
} matType;

typedef struct cudaMat {
    int m, n;
    matType type;
    vtype mat;
} cudaMat ;

cudaMat* init_mat(int m, int n, int mode, matType matType);
void free_mat(cudaMat *mat);
void print_mat(cudaMat *mat);
cudaMat* alter_majorOrder(cudaMat *mat);
int equal_mat(cudaMat *mata, cudaMat *matb);

cudaMat* matmul_omp(cudaMat *mata, cudaMat *matb);
cudaMat* matmul_cuda(cudaMat *mata, cudaMat *matb, int blockSize);
cudaMat* matmul_cuda_multiDevices(cudaMat *mata, cudaMat *matb, int blockSize, int num_threads);
cudaMat* matmul_cuBLAS(cudaMat *mata, cudaMat *matb, int blockSize);
__global__ void matmul_kernel1(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k);
__global__ void matmul_kernel2(vtype d_A, vtype d_B, vtype d_C, int m, int n, int k);