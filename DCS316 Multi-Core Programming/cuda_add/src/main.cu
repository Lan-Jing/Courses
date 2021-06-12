#include <iostream>
using namespace std;
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "../include/add.hpp"
#include "../include/helper.hpp"

int main(int argc, char *argv[])
{
    init_rand();
    create_timer(10);

    int n = argc < 2 ? 10 : atoi(argv[1]);
    int testRounds = argc < 3 ? 1 : atoi(argv[2]);
    
    printf("Matrix Size:              %5d\n", n);
    printf("Thread Block Size:        %5d\n", BLOCKSIZE);
    printf("Workload per CUDA thread: %5d\n", WORKLOAD*WORKLOAD);
    printf("Test Rounds:              %5d\n", testRounds);
    for(int i = 0;i < testRounds;i++) {
        matrix a(n, n, 1);
        matrix b(n, n, 1);
        matrix aa = a, c, d;

        a += b;
        matrix_add(c, aa, b, gpu_add_1D);
        matrix_add(d, aa, b, gpu_add_2D);

        if(i%1 == 0) {
            printf("   %s\n", a == c && a == d ? "Check Passed" : "Check Failed");
        }
    }
	printf("CPU    Min Execution Time:%10lf ms\n", check_timer(0));
    printf("GPU_1D Min Execution Time:%10lf ms\n", check_timer(1));
    printf("GPU_2D Min Execution Time:%10lf ms\n", check_timer(2));
    printf("\n");

    destroy_timer();
    return 0;
}
