#include <omp.h>
#include <cuda.h>
#include <cuda_runtime.h>
#define BLOCKSIZE 32

void init_rand();
char rand_char();
int divup(int a, int b);

class timer
{
public:
    timer(int _l);
    ~timer();
    void init_time();
    void update_time(int idx, double t);
    double fetch_time(int idx);
private:
    double* timeArr;
    int l;
};
extern timer recorder;

class mat 
{
public:
    mat(int _m, int _n, int _mode);
    ~mat();
    int size() const ;
    void print() const ;
    float* entropy_omp();
    float* entropy_cu(int mode);

    int m, n;
    char* val;
};

// a warper for checking cuda function calls
#define gpuCheck(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPU assert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

void print_entropy(float* entropy, int m, int n);
bool check_entropy(float* entropy_a, float* entropy_b, int size);

__global__ void entropy_kernel1(float* d_entropy, char* d_val, int m, int n);
__global__ void entropy_kernel2(float* d_entropy, char* d_val, float* d_logTable, int m, int n);
template<int bsize>
__global__ void entropy_kernel3(float* d_entropy, char* d_val, int m, int n);
template<int bsize>
__global__ void entropy_kernel4(float* d_entropy, char* d_val, int m, int n);
template<int bsize_x, int bsize_y>
__global__ void entropy_kernel5(float* d_entropy, char* d_val, float* d_logTable, int m, int n);