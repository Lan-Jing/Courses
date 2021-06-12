#include <iostream>
using namespace std;
#include <stdlib.h>
#include "../include/mat.hpp"

int main(int argc, char* argv[]) 
{
    init_rand();

    int n = argc > 2 ? strtol(argv[2], NULL, 10) : 10;
    int rounds = argc > 1 ? strtol(argv[1], NULL, 10) : 1;
    int passed = 1;
    
    mat a = mat(n, n, 1);
    for(int i = 0;i < rounds;i++) {
        float* entropy_omp = a.entropy_omp();
        float* entropy_cu_1 = a.entropy_cu(1);
        float* entropy_cu_2 = a.entropy_cu(2);
        float* entropy_cu_3 = a.entropy_cu(3);
        float* entropy_cu_4 = a.entropy_cu(4);
        float* entropy_cu_5 = a.entropy_cu(5);
    
        bool passed = check_entropy(entropy_cu_2, entropy_cu_5, n);
        
        free(entropy_omp);
        free(entropy_cu_1);
        free(entropy_cu_2);
        free(entropy_cu_3);
        free(entropy_cu_4);
        free(entropy_cu_5);
        if(!passed) {
			return 0;
		}
    }
    
    if(passed) {
        printf("Check Passed\n");
        printf("OMP  Time: %6lf ms \n", recorder.fetch_time(0));
        printf("Kernel1 Time: %6lf ms \n", recorder.fetch_time(1));    
        printf("Kernel2 Time: %6lf ms \n", recorder.fetch_time(2));    
        printf("Kernel3 Time: %6lf ms \n", recorder.fetch_time(3));            
        printf("Kernel4 Time: %6lf ms \n", recorder.fetch_time(4));            
        printf("Kernel5 Time: %6lf ms \n", recorder.fetch_time(5));            
    }

    return 0;
}