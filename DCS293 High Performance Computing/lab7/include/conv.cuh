#define RAND_RBOUND 1.0
#define eps 1e-4
#define dtype float
#define dsize sizeof(dtype)
#define blockSize 32
// #define DEBUG

typedef dtype* vtype;

typedef enum matType {
	rowMajor = 0,
	colMajor = 1
} matType ;

class cudaImage
{
    public:
        int m, n, d;
        matType type;
        vtype mat;

        cudaImage();
        cudaImage(int _m, int _n, int _d, int _mode, matType _type);
        cudaImage(cudaImage &other, matType _type);
        ~cudaImage();
        
        int size() const;
        int index(int d, int rIdx, int cIdx) const;
        void print() const;
        void alter_major();
        cudaImage* padding(int &om, int &on, int km, int kn, int s) const ;
        cudaImage* im2col(int &om, int &on, int km, int kn, int s) const ;

        dtype& operator [] (int idx) const;
        bool operator == (cudaImage &other) const;
        void operator = (cudaImage &other);
};

void get_padding(int &rpl, int &rpr, int &cpl, int &cpr, int im, int in, int km, int kn, int s);

/*
=================== Convolution ==============================
*/
cudaImage* conv_omp(cudaImage &input, cudaImage &kernel, int stride);
cudaImage* conv_cuda(cudaImage &input, cudaImage &kernel, int stride);
cudaImage* conv_im2col(cudaImage &input, cudaImage &kernel, int stride);
cudaImage* conv_cudnn(cudaImage &input, cudaImage &kernel, int stride);

__global__ void conv_kernel1(dtype *d_i, dtype *d_k, dtype *d_o, 
                             const int im, const int in, const int s, const int d,
                             const int km, const int kn,
                             const int om, const int on);
__global__ void conv_kernel2(dtype *d_i, dtype *d_k, dtype *d_o, 
                             const int im, const int in, const int s, const int d,
                             const int km, const int kn,
                             const int om, const int on);

/*
=========================== VMM ==============================
*/
cudaImage* vmm_cuda(cudaImage &mata, cudaImage &matb, int om, int on);

__global__ void vmm_kernel(vtype d_A, vtype d_B, vtype d_C, 
                           int n, int k);
