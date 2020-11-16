#include <time.h>
#include "../include/MatrixOp.h"

// #define DEBUG
#define testRounds 10

// parameters of the random matrices.
int m, n, k;

int main() {
    srand(time(0));
    
    struct Matrix *mata = NULL, *matb = NULL, *resMat = NULL;
    int m1, n1, m2, n2;
    printf("Enter parameters m, n and k for the multiplication programme.\n");
    scanf("%d %d %d", &m, &n, &k);

    m1 = m, n1 = m2 = n, n2 = k;

    mata = new_mat(m1, n1, 0);
    matb = new_mat(m2, n2, 0);
#ifdef DEBUG
    // print_mat(mata);
    // print_mat(matb);
#endif

    double startTime, endTime, avgTime = 0;
    // run brutal multiplcation and record execution time;
    for(int i = 0;i < testRounds;i++) {
        startTime = omp_get_wtime();
        resMat = BrutalMul(mata, matb);
        endTime = omp_get_wtime();
        avgTime += endTime - startTime;
        free_mat(resMat);
    }
    printf("Average time of plain algorithm : %lfs.\n", avgTime / testRounds);
    avgTime = 0;
    // run for the Strassen algorithm;
    for(int i = 0;i < testRounds;i++) {
        startTime = omp_get_wtime();
        resMat = StrassenMul(mata, matb);
        endTime = omp_get_wtime();
        avgTime += endTime- startTime;
        if(testRounds != 1)
            free_mat(resMat);
    }
    printf("Average time of Strassen algorithm : %lfs.\n", avgTime / testRounds);
    // execute baseline test if DEBUG enabled;
#ifdef DEBUG
    int flag = 1;
    startTime = omp_get_wtime();
    struct Matrix *testRes = testMul(mata, matb);
    endTime = omp_get_wtime();
    printf("Baseline completed in %lfs.\n", endTime-startTime);
    // check correctness with the baseline;
    // print_mat(testRes); print_mat(resMat);
    for(int i = 0;i < testRes->m;i++) {
        for(int j = 0;j < testRes->n;j++) 
            if(abs(testRes->mat[i]->vec[j]-resMat->mat[i]->vec[j]) >= 1e-2) {
                printf("Mismatch with Baseline at (%d, %d): %lf vs. %lf\n", i, j, testRes->mat[i]->vec[j], resMat->mat[i]->vec[j]);
                flag = 0; break;
            }
        if(!flag) break;
    }
    if(flag) {
        printf("Correct implementation.\n");
    } else {
        printf("Poor implementation.\n");
    }
    free_mat(testRes);
#endif

    free_mat(mata);
    free_mat(matb);

    return 0;
}