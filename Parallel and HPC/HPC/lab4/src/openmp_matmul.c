#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include "../include/mat.h"

int m, n, k;
mat_t *mata, *matb, *matc, *testMat;

double startTime, endTime;

int main() 
{
    srand(time(NULL));

    printf("Input m, n, k for the generation of matrix A and B.\n");
    scanf("%d %d %d", &m, &n, &k);

    mata = new_mat(m, n, 1);
    matb = new_mat(n, k, 1);

#ifdef DEBUG
    startTime = omp_get_wtime();
    testMat = testmul(mata, matb);
    endTime = omp_get_wtime();
    printf("Baseline ends in %lfs.\n", endTime-startTime);
#endif

    startTime = omp_get_wtime();
    matc = ompmul(mata, matb);
    endTime = omp_get_wtime();
    printf("Openmp implementation ends in %lfs.\n", endTime-startTime);

#ifdef DEBUG
    printf("Result check %s.\n", mat_is_equal(matc, testMat) ? "accepted" : "failed");
    free_mat(testMat);
#endif

    free_mat(mata);
    free_mat(matb);
    free_mat(matc);
    return 0;
}