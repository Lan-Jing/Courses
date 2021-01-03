#include <iostream>
using namespace std;
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "../include/conv.cuh"
#include "../include/helper.hpp"

#define testRounds 10
#define num_devices 4
#define kernelSize 3

int main(int argc, char *argv[])
{
    int imageSize = strtol(argv[1], NULL, 10);
    cudaImage image = cudaImage(imageSize, imageSize, 3, 1, rowMajor);
    cudaImage kernel = cudaImage(kernelSize, kernelSize, 3, 1, rowMajor);

#ifdef DEBUG
    // image.print();
    // kernel.print();
#endif

    cudaDeviceSetCacheConfig(cudaFuncCachePreferShared);
    for(int stride = 1;stride <= 3;stride++) {
        cudaImage *omp_resImage = NULL, 
                  *cud_resImage = NULL,
                  *col_resImage = NULL,
                  *cudnn_resImage = NULL;
        
        omp_avgTime = cuda_avgTime = col_avgTime = cudnn_avgTime = 0;
        for(int i = 0;i < testRounds;i++) {
            omp_resImage = conv_omp(image, kernel, stride);
            cud_resImage = conv_cuda(image, kernel, stride);
            col_resImage = conv_im2col(image, kernel, stride);
            cudnn_resImage = conv_cudnn(image, kernel, stride);

            if(i == testRounds-1) {
            #ifdef DEBUG
                omp_resImage->print();
                cud_resImage->print();
                col_resImage->print();
                cudnn_resImage->print();
            #endif
                printf("Result check %s\n", (*cudnn_resImage) == (*cud_resImage) ? "accepted" : "failed");
            }

            if(omp_resImage) delete omp_resImage;
            if(cud_resImage) delete cud_resImage;
            if(col_resImage) delete col_resImage;
            if(cudnn_resImage) delete cudnn_resImage;
        }
        printf("Average time on %d CPU threads: %lf\n", omp_get_num_procs(), omp_avgTime/testRounds);
        printf("Average time on CUDA: %lf\n", cuda_avgTime/testRounds);
        printf("Average time on CUDA, im2col version: %lf\n", col_avgTime/testRounds);
        printf("Average time on cuDNN library: %lf\n", cudnn_avgTime/testRounds);
    }

    return 0;
}