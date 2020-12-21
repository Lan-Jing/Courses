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

dtype& cudaImage::operator [] (int idx)
{
    return mat[idx];
}

bool cudaImage::operator == (cudaImage &other)
{
    if(m != other.m || n != other.n || d != other.d)
        return false;
    
    for(int i = 0;i < d;i++)
        for(int rIdx = 0;rIdx < m;rIdx++)
            for(int cIdx = 0;cIdx < n;cIdx++) {
                int index = this->index(i, rIdx, cIdx);
                if(mat[index] != other.mat[index])
                    return false;
            } 
    return true;
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

cudaImage* conv_omp(cudaImage &input, cudaImage &kernel, int s)
{
    cudaImage *resImage = NULL;
    if(input.d != kernel.d || kernel.m > input.m || kernel.n > input.n)
        return resImage;
    
    int im = input.m, in = input.n, d = input.d,
        km = kernel.m, kn = kernel.n,
        om, on;
    int rpl, rpr, cpl, cpr;
    // compute left and right padding for both dimensions.
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
#ifdef DEBUG
    printf("Padding Info: %d %d %d %d %d %d\n", rpl, rpr, cpl, cpr, om, on);
#endif

    resImage = new cudaImage(om, on, 1, 0, rowMajor);
    int rIdx;
    // loop over each output pixel. BE CAREFUL to the indexing!!!
#pragma omp parallel for private(rIdx)
    for(rIdx = -rpl;rIdx < im+rpr-(km-1);rIdx += s) {
        int rIdx_o = (rIdx+rpl)/s, cIdx_o = 0;
        for(int cIdx = -cpl;cIdx < in+cpr-(kn-1);cIdx += s, cIdx_o++) {
            dtype& out = (*resImage)[resImage->index(0, rIdx_o, cIdx_o)] = 0;
            // calculate this pixel.
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

    return resImage;
}