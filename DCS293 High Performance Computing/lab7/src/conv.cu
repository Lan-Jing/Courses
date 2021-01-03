#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cudnn.h>

#include "../include/conv.cuh"
#include "../include/helper.hpp"

#define checkCUDNN(expression)                               \
  {                                                          \
    cudnnStatus_t status = (expression);                     \
    if (status != CUDNN_STATUS_SUCCESS) {                    \
      std::cerr << "Error on line " << __LINE__ << ": "      \
                << cudnnGetErrorString(status) << std::endl; \
      std::exit(EXIT_FAILURE);                               \
    }                                                        \
  }

__constant__ dtype sd_k[blockSize*blockSize];

// have the depth dimension change slowest.
int cudaImage::index(int i, int rIdx, int cIdx) const
{
    if(type == rowMajor) {
        return i*m*n + rIdx * n + cIdx;
    } else {
        return i*m*n + cIdx * m + rIdx;
    }
}

int cudaImage::size() const
{
    return m*n*d;
}

dtype& cudaImage::operator [] (int idx) const
{
    return mat[idx];
}

bool cudaImage::operator == (cudaImage &other) const
{
    if(m != other.m || n != other.n || d != other.d)
        return false;
    
    dtype maxError = 0;
    for(int i = 0;i < d;i++) {
        int rIdx;
    #pragma omp parallel for private(rIdx)
        for(rIdx = 0;rIdx < m;rIdx++)
            for(int cIdx = 0;cIdx < n;cIdx++) {
                int index = this->index(i, rIdx, cIdx);
                maxError = fmax(maxError, fabs(mat[index]-other.mat[index]));
            }
    }
    if(maxError >= eps) {
        printf("Maximum Error: %lf, required: %lf\n", maxError, eps);
        return false;
    } else {
        return true;
    }
}

void cudaImage::operator = (cudaImage &other) 
{
    if(mat != NULL) {
        delete [] mat;
    }

    m = other.m, n = other.n, d = other.d, type = other.type;
    
    int size = this->size();
    mat = new dtype[size];

    memcpy(mat, other.mat, size*dsize);
}

cudaImage::cudaImage()
{
    m = n = d = 0;
    mat = NULL;
}

cudaImage::cudaImage(int _m, int _n, int _d, int _mode, matType _type)
{
    m = _m, n = _n, d = _d, type = _type;
    
    long long int size = this->size();
    mat = new dtype[size];

    if(_mode == 1) {
        for(int i = 0;i < size;i++)
        #ifdef DEBUG
            mat[i] = 1;
        #else
            mat[i] = rand_float(RAND_RBOUND);
        #endif
    }
}

cudaImage::cudaImage(cudaImage &other, matType _type)
{
    m = other.m, n = other.n, d = other.d, type = _type;

    int size = this->size();
    mat = new dtype[size];

    if(type == other.type) {
        memcpy(mat, other.mat, size*dsize);
    } else {
        for(int i = 0;i < d;i++)
            for(int rIdx = 0;rIdx < m;rIdx++)
                for(int cIdx = 0;cIdx < n;cIdx++) {
                    int index = this->index(i, rIdx, cIdx);
                    mat[index] = other.mat[index];
                }
    }
}

cudaImage::~cudaImage()
{
    delete [] mat;
}

void cudaImage::print() const
{
    printf("Arranged in %s order\n", type ? "col-major" : "row-major");
    for(int i = 0;i < d;i++) {
        printf("Channel: %d\n", i);
        for(int rIdx = 0;rIdx < m;rIdx++) {
            for(int cIdx = 0;cIdx < n;cIdx++) {
                int index = this->index(i, rIdx, cIdx);
                printf("%lf ", mat[index]); 
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Rotate the index manner as well as the matrix itself.
void cudaImage::alter_major()
{
    matType newType = type == rowMajor ? colMajor : rowMajor;
    cudaImage tmpImage = cudaImage(*this, newType);
    for(int i = 0;i < d;i++)
        for(int rIdx = 0;rIdx < m;rIdx++)
            for(int cIdx = 0;cIdx < n;cIdx++) {
                int tIdx = tmpImage.index(i, rIdx, cIdx),
                    idx = this->index(i, rIdx, cIdx);
                tmpImage[tIdx] = (*this)[idx];
            }
    *this = tmpImage;
}

// return a padded image and corresponding output size.
cudaImage* cudaImage::padding(int &om, int &on, int km, int kn, int s) const
{
    int rpl, rpr, cpl, cpr;
    get_padding(rpl, rpr, cpl, cpr, om, on, m, n, km, kn, s);
    cudaImage *padImage = new cudaImage(m+rpl+rpr, n+cpl+cpr, d, 0, rowMajor);

#ifdef DEBUG
    printf("Padding Info: %d %d %d %d %d %d\n", rpl, rpr, cpl, cpr, om, on);
#endif

    memset(padImage->mat, 0, padImage->size()*dsize);
    for(int i = 0;i < d;i++) {
        int rIdx;
    #pragma omp parallel for private(rIdx)
        for(rIdx = 0;rIdx < m;rIdx++)
            for(int cIdx = 0;cIdx < n;cIdx++) {
                int prIdx = rIdx + rpl,
                    pcIdx = cIdx + cpl,
                    pIdx = padImage->index(i, prIdx, pcIdx),
                    idx = index(i, rIdx, cIdx);
               (*padImage)[pIdx] = (*this)[idx]; 
            }
    }
    return padImage;
}

cudaImage* cudaImage::im2col(int &om, int &on, int km, int kn, int s) const
{
    cudaImage *colImage = NULL, *padImage = NULL;
    if(km > m || kn > n)
        return colImage;
    
    padImage = padding(om, on, km, kn, s);
    // Be cautious. This operation is extermely mem-hungry.
    colImage = new cudaImage(km*kn*d, om*on, 1, 0, rowMajor);

    for(int i = 0;i < d;i++) {
        // for each row in the colImage
        int sIdx = 0;
    #pragma omp parallel for private(sIdx)
        for(sIdx = 0;sIdx < km*kn;sIdx++) {
            int sy = sIdx / km, sx = sIdx % km,
                cIdx = colImage->index(0, i*km*kn+sIdx, 0);
            for(int k = 0;k < om*on;k++, cIdx++) {
                int y = k / om, x = k % om,
                    pIdx = padImage->index(i, (sy+y*s), (sx+x*s));
                (*colImage)[cIdx] = (*padImage)[pIdx];
            }
        }
    }
    return colImage;
}

/*
============================== Implementation of convolution =====================
*/

cudaImage* conv_omp(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL, *padImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int km = kernel.m, kn = kernel.n, om, on;
    padImage = input.padding(om, on, km, kn, s);
    resImage = new cudaImage(om, on, 1, 0, rowMajor);

    omp_stime = omp_get_wtime();
    // loop over each output pixel. BE CAREFUL to the indexing!!!
    for(int i = 0;i < input.d;i++) {    
        int rIdx;
    #pragma omp parallel for private(rIdx)
        for(rIdx = 0;rIdx < om;rIdx++)
            for(int cIdx = 0;cIdx < on;cIdx++) {
                dtype &out = (*resImage)[resImage->index(0, rIdx, cIdx)];
                if(i == 0) out = 0;
                for(int ky = 0;ky < km;ky++)
                    for(int kx = 0;kx < kn;kx++) {
                        int irIdx = rIdx * s + ky,
                            icIdx = cIdx * s + kx,
                            kIdx = kernel.index(i, ky, kx),
                            iIdx = padImage->index(i, irIdx, icIdx);
                        out += (*padImage)[iIdx] * kernel[kIdx];
                    }
            }
    }
    omp_avgTime += omp_get_wtime() - omp_stime;

    delete padImage;
    return resImage;
}

cudaImage* conv_cuda(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL, *padImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int km = kernel.m, kn = kernel.n, om, on;
    padImage = input.padding(om, on, km, kn, s);
    resImage = new cudaImage(om, on, 1, 0, rowMajor);

    dtype *d_i, *d_o, *d_k;
    cudaMalloc(&d_i, padImage->size() * dsize);
    cudaMalloc(&d_o, resImage->size() * dsize);
    cudaMalloc(&d_k, kernel.size() * dsize);
    cudaMemcpy(d_i, padImage->mat, padImage->size() * dsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_k, kernel.mat, kernel.size() * dsize, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(sd_k, d_k, kernel.size() * dsize);

    dim3 blockDim(blockSize, blockSize),
         gridDim((om+blockSize-1)/blockSize, (on+blockSize-1)/blockSize);

    cuda_stime = omp_get_wtime();
    conv_kernel2<<<gridDim, blockDim>>>(d_i, d_k, d_o,
                                        padImage->m, padImage->n, s, padImage->d,
                                        km, kn,
                                        om, on);
    cudaDeviceSynchronize();
    cuda_avgTime += omp_get_wtime() - cuda_stime;

    cudaMemcpy(resImage->mat, d_o, resImage->size() * dsize, cudaMemcpyDeviceToHost);
    cudaFree(d_i);
    cudaFree(d_o);
    cudaFree(d_k);

    delete padImage;
    return resImage;
}

// No need to deal with the kernel, since it is already stored as a row vector.
cudaImage* conv_im2col(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL, *colImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int km = kernel.m, kn = kernel.n, om, on;
    colImage = input.im2col(om, on, km, kn, s);
    resImage = vmm_cuda(kernel, *colImage, om, on);

    delete colImage;
    return resImage;
}

cudaImage* conv_cudnn(cudaImage &input, cudaImage &kernel, int s) 
{
    cudaImage *resImage = NULL, *padImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int km = kernel.m, kn = kernel.n, kd = kernel.d, om, on;
    padImage = input.padding(om, on, km, kn, s);
    resImage = new cudaImage(om, on, 1, 0, rowMajor);

    cudnnHandle_t handle;
    cudnnCreate(&handle);

    cudnnTensorFormat_t format = CUDNN_TENSOR_NCHW;
    cudnnDataType_t type = CUDNN_DATA_FLOAT;

    cudnnTensorDescriptor_t xDesc, yDesc;
    cudnnCreateTensorDescriptor(&xDesc);
    cudnnSetTensor4dDescriptor(xDesc, 
                               format, 
                               type, 
                               1, padImage->d, padImage->m, padImage->n);
    cudnnCreateTensorDescriptor(&yDesc);
    cudnnSetTensor4dDescriptor(yDesc, 
                               format, 
                               type, 
                               1, 1, om, on);
    
    cudnnFilterDescriptor_t wDesc;
    cudnnCreateFilterDescriptor(&wDesc);
    cudnnSetFilter4dDescriptor(wDesc, 
                               type, 
                               format, 
                               1, kd, km, kn);

    cudnnConvolutionDescriptor_t convDesc;
    cudnnCreateConvolutionDescriptor(&convDesc);
    cudnnSetConvolution2dDescriptor(convDesc, 
                                    0, 0, // already pad on the input
                                    s, s, // stride
                                    1, 1, // no dilation
                                    CUDNN_CROSS_CORRELATION, 
                                    type);

#ifdef DEBUG
    // Check if descriptor for y is correctly set, given x, w and conv.
    int n, c, h, w;
    cudnnGetConvolution2dForwardOutputDim(convDesc,
                                          xDesc,
                                          wDesc,
                                          &n, &c, &h, &w);
    printf("Specification vs. expected: N%d,%d C%d,%d H%d,%d W%d,%d\n", 1, n, 1, c, om, h, on, w);
#endif

    // Can be set via heuristic method.
    cudnnConvolutionFwdAlgo_t algo = CUDNN_CONVOLUTION_FWD_ALGO_GEMM;
    
    size_t workspaceSize = 0;
    void *workspace = NULL;
    cudnnGetConvolutionForwardWorkspaceSize(handle, 
                                            xDesc, wDesc, convDesc, yDesc, 
                                            algo, 
                                            &workspaceSize);
    cudaMalloc(&workspace, workspaceSize);

#ifdef DEBUG
    cout<<"Additional GPU mem needed:"<<workspaceSize<<endl;
#endif

    dtype *x, *k, *y;
    cudaMalloc(&x, padImage->size()*dsize);
    cudaMalloc(&y, resImage->size()*dsize);
    cudaMalloc(&k, kernel.size()*dsize);
    cudaMemcpy(x, padImage->mat, padImage->size()*dsize, cudaMemcpyHostToDevice);
    cudaMemcpy(k, kernel.mat, kernel.size()*dsize, cudaMemcpyHostToDevice);

    cudnn_stime = omp_get_wtime();
    // Be cautious! Scalar values should be strictly set equal to other input. Float here. 
    const dtype alpha = 1.0, beta = 0.0;
    cudnnConvolutionForward(handle,
                            &alpha, xDesc, x,
                            wDesc, k,
                            convDesc,
                            algo,
                            workspace,
                            workspaceSize,
                            &beta, yDesc, y);
    cudaDeviceSynchronize();
    cudnn_avgTime += omp_get_wtime() - cudnn_stime;

    cudaMemcpy(resImage->mat, y, resImage->size()*dsize, cudaMemcpyDeviceToHost);
    cudaFree(x);
    cudaFree(y);
    cudaFree(k);
    cudaFree(workspace);

    cudnnDestroyTensorDescriptor(xDesc);
    cudnnDestroyTensorDescriptor(yDesc);
    cudnnDestroyConvolutionDescriptor(convDesc);
    cudnnDestroyFilterDescriptor(wDesc);

    cudnnDestroy(handle);
    delete padImage;
    return resImage;
}

// This kernel assumes all matrices arranged in row-major order.
__global__ void conv_kernel1(vtype d_i, vtype d_k, vtype d_o, 
                             const int im, const int in, const int s, const int d,
                             const int km, const int kn, 
                             const int om, const int on)
{
    int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
        cIdx = threadIdx.y + blockIdx.y * blockDim.y,
        rIdx_s = rIdx * s,
        cIdx_s = cIdx * s;

    dtype res = 0;
    if(rIdx < om && cIdx < on) {
        for(int i = 0;i < d;i++) {
            int irIdx = rIdx_s;
            for(int ky = 0;ky < km;ky++, irIdx++) {
                int icIdx = cIdx_s;
                for(int kx = 0;kx < kn;kx++, icIdx++) {
                    int kIdx = i*km*kn + ky*kn + kx,
                        iIdx = i*im*in + irIdx*in + icIdx;
                    res += d_i[iIdx] * d_k[kIdx];
                }
            }
        }
        d_o[rIdx * on + cIdx] = res;
    }
}

// input image -> shared memory
// the filter -> constant memory
__global__ void conv_kernel2(dtype *d_i, dtype *d_k, dtype *d_o, 
                             const int im, const int in, const int s, const int d,
                             const int km, const int kn,
                             const int om, const int on)
{
    // index in the thread block
    int brIdx = threadIdx.x, 
        bcIdx = threadIdx.y,
        brIdx_i = brIdx * s, 
        bcIdx_i = bcIdx * s,
    // index in the whole image
        grIdx = brIdx + blockIdx.x * blockDim.x, 
        gcIdx = bcIdx + blockIdx.y * blockDim.y,
        grIdx_i = grIdx * s, 
        gcIdx_i = gcIdx * s;

    dtype res = 0;
    __shared__ dtype sd_i[3*blockSize+10][3*blockSize+10];
    if(grIdx < om && gcIdx < on) {
        for(int i = 0;i < d;i++) {
            // each block loabds pixels it need into the shared mem, including the apron area.
            for(int r = 0;(brIdx == blockSize-1 || grIdx == om-1) ? r < km : r < s;r++)
                for(int c = 0;(bcIdx == blockSize-1 || gcIdx == on-1) ? c < kn : c < s;c++) {
                    int iIdx = i*im*in + (grIdx_i+r)*in + gcIdx_i+c;
                    sd_i[brIdx_i+r][bcIdx_i+c] = d_i[iIdx];
                }
            __syncthreads();
            int rIdx = brIdx_i;
            for(int ky = 0;ky < km;ky++, rIdx++) {
                int cIdx = bcIdx_i;
                for(int kx = 0;kx < kn;kx++, cIdx++) {
                    int kIdx = i*km*kn + ky*kn + kx;
                    res += sd_i[rIdx][cIdx] * sd_k[kIdx];
                }
            }
            __syncthreads();
        }
        d_o[grIdx * on + gcIdx] = res;
    }
}
