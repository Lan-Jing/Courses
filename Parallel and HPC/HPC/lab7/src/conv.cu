#include <iostream>
using namespace std;
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include "../include/conv.cuh"
#include "../include/helper.hpp"

// have the depth dimension change fastest.
int cudaImage::index(int i, int rIdx, int cIdx) const
{
    if(type == rowMajor) {
        return (rIdx * n + cIdx) * d + i;
    } else {
        return (cIdx * m + rIdx) * d + i;
    }
}

int cudaImage::get_size() const
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
    int rIdx;
#pragma omp parallel for private(rIdx)
    for(rIdx = 0;rIdx < m;rIdx++)
        for(int cIdx = 0;cIdx < n;cIdx++)
            for(int i = 0;i < d;i++) {
                int index = this->index(i, rIdx, cIdx);
                maxError = fmax(maxError, fabs(mat[index]-other.mat[index]));
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
    
    int size = get_size();
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
    
    int size = get_size();
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

    int size = get_size();
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


// construct left and right padding for both dimensions.
// return size of the result matrix as well.
void get_padding(int &rpl, int &rpr, int &cpl, int &cpr, 
                 int &om, int &on,
                 int im, int in, int km, int kn, int s)
{
    for(int rPadding = 0;rPadding < s;rPadding++) {
        if((im-km+rPadding) % s == 0) {
            rpl = rPadding / 2;
            rpr = rPadding - rpl;
            om = (im-km+rPadding) / s + 1;
            break;
        }
    }
    for(int cPadding = 0;cPadding < s;cPadding++) {
        if((in-kn+cPadding) % s == 0) {
            cpl = cPadding / 2;
            cpr = cPadding - cpl;
            on = (in-kn+cPadding) / s + 1;
            break;
        }
    }
}


cudaImage* conv_omp(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int im = input.m, in = input.n, d = input.d,
        km = kernel.m, kn = kernel.n,
        om, on;
    int rpl, rpr, cpl, cpr;
    
    get_padding(rpl, rpr, cpl, cpr, om, on, im, in, km, kn, s);
#ifdef DEBUG
    printf("Padding Info: %d %d %d %d %d %d\n", rpl, rpr, cpl, cpr, om, on);
#endif

    resImage = new cudaImage(om, on, 1, 0, rowMajor);
    omp_stime = omp_get_wtime();

    int rIdx;
    // loop over each output pixel. BE CAREFUL to the indexing!!!
#pragma omp parallel for private(rIdx)
    for(rIdx = -rpl;rIdx < im+rpr-(km-1);rIdx += s) {
        int rIdx_o = (rIdx+rpl)/s, cIdx_o = 0;
        for(int cIdx = -cpl;cIdx < in+cpr-(kn-1);cIdx += s, cIdx_o++) {
            dtype& out = (*resImage)[resImage->index(0, rIdx_o, cIdx_o)] = 0;
            // calculate the pixel pointed by 'out'.
            for(int ky = 0;ky < km;ky++)
                for(int kx = 0;kx < kn;kx++) {
                    int iy = rIdx+ky, ix = cIdx+kx;
                    // zero padding.
                    if(iy < 0 || ix < 0 || iy >= im || ix >= in)
                        continue;
                    for(int i = 0;i < d;i++) {
                        int iIdx = input.index(i, iy, ix),
                            kIdx = kernel.index(i, ky, kx);
                        out += input[iIdx] * kernel[kIdx];
                    }
                }
        }
    }
    omp_avgTime += omp_get_wtime() - omp_stime;

    return resImage;
}

cudaImage* conv_cuda(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL, *padInput = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int im = input.m, in = input.n, d = input.d,
        km = kernel.m, kn = kernel.n,
        om, on;
    int rpl, rpr, cpl, cpr;

    get_padding(rpl, rpr, cpl, cpr, om, on, im, in, km, kn, s);
    padInput = new cudaImage(im+rpl+rpr, in+cpl+cpr, d, 0, rowMajor);
    resImage = new cudaImage(om, on, 1, 0, rowMajor);
    // set up the padded matrix
    memset(padInput->mat, 0, padInput->get_size()*dsize);
    int rIdx;
#pragma omp parallel for private(rIdx)
    for(rIdx = 0;rIdx < im;rIdx++)
        for(int cIdx = 0;cIdx < in;cIdx++) {
            int prIdx = rIdx + rpl,
                pcIdx = cIdx + cpl;
            for(int i = 0;i < d;i++) {
                int pIdx = padInput->index(i, prIdx, pcIdx),
                    idx = input.index(i, rIdx, cIdx);
                (*padInput)[pIdx] = input[idx];
            }
        }

#ifdef DEBUG
    printf("Padding Info: %d %d %d %d %d %d\n", rpl, rpr, cpl, cpr, om, on);
    padInput->print();
#endif

    dtype *d_i, *d_o, *d_k;
    cudaMalloc(&d_i, padInput->get_size() * dsize);
    cudaMalloc(&d_o, resImage->get_size() * dsize);
    cudaMalloc(&d_k, kernel.get_size() * dsize);
    cudaMemcpy(d_i, padInput->mat, padInput->get_size() * dsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_k, kernel.mat, kernel.get_size() * dsize, cudaMemcpyHostToDevice);

    dim3 blockDim(blockSize, blockSize),
         gridDim((om+blockSize-1)/blockSize, (on+blockSize-1)/blockSize);

    cuda_stime = omp_get_wtime();
    conv_kernel2<<<gridDim, blockDim>>>(d_i, d_k, d_o,
                                        im+rpl+rpr, in+cpl+cpr, s, d,
                                        km, kn,
                                        om, on);
    cudaDeviceSynchronize();
    cuda_avgTime += omp_get_wtime() - cuda_stime;

    cudaMemcpy(resImage->mat, d_o, resImage->get_size() * dsize, cudaMemcpyDeviceToHost);
    cudaFree(d_i);
    cudaFree(d_o);
    cudaFree(d_k);

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
        int irIdx = rIdx_s;
        for(int ky = 0;ky < km;ky++, irIdx++) {
            int icIdx = cIdx_s;
            for(int kx = 0;kx < kn;kx++, icIdx++) {
                int kIdx = (ky * kn + kx) * d,
                    iIdx = (irIdx * in + icIdx) * d;
                for(int i = 0;i < d;i++, kIdx++, iIdx++) {
                    res += d_i[iIdx] * d_k[kIdx];
                }
            }
        }
        d_o[rIdx * on + cIdx] = res;
    }
}

__global__ void conv_kernel2(dtype *d_i, dtype *d_k, dtype *d_o, 
                             const int im, const int in, const int s, const int d,
                             const int km, const int kn,
                             const int om, const int on)
{
    int rIdx = threadIdx.x + blockIdx.x * blockDim.x,
        cIdx = threadIdx.y + blockIdx.y * blockDim.y,
        rIdx_s = rIdx * s,
        cIdx_s = cIdx * s;
    
    __shared__ dtype sd_k[blockSize*blockSize];
    if(threadIdx.x < km && threadIdx.y < kn * d) {
        int kIdx = threadIdx.x * kn * d + threadIdx.y;
        sd_k[kIdx] = d_k[kIdx];
    }
    __syncthreads();

    dtype res = 0;
    if(rIdx < om && cIdx < on) {
        int irIdx = rIdx_s;
        for(int ky = 0;ky < km;ky++, irIdx++) {
            int icIdx = cIdx_s;
            for(int kx = 0;kx < kn;kx++, icIdx++) {
                int kIdx = (ky * kn + kx) * d,
                    iIdx = (irIdx * in + icIdx) * d;
                for(int i = 0;i < d;i++, kIdx++, iIdx++) {
                    res += d_i[iIdx] * sd_k[kIdx];
                }
            }
        }
        d_o[rIdx * on + cIdx] = res;
    }
}
