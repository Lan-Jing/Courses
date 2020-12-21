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
    image.print();
    image.alter_major();
    image.print();

    cudaImage kernel = cudaImage(kernelSize, kernelSize, 3, 0, rowMajor);
    for(int rIdx = 0;rIdx < kernelSize;rIdx++)
        for(int cIdx = 0;cIdx < kernelSize;cIdx++)
            for(int d = 0;d < 3;d++) {
                int index = kernel.index(d, rIdx, cIdx);
                kernel[index] = d;
            }
#ifdef DEBUG
    image.print();
    kernel.print();
#endif

    for(int stride = 1;stride <= 3;stride++) {
        cudaImage *resImage = conv_omp(image, kernel, stride);
        resImage->print();
    }

    return 0;
}