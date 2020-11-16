#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include "mat.h"

#define rightBound 1e2

int threadCount;
pthread_t *pVec;
int *rankVec;

int m, n, k;
mat_t *mata, *matb, *matc, *testMat;

double startTime, endTime;

void* matmul_thread(void *rank)
{
    int myRank = *((int*)rank), 
        blockSize = mata->m / threadCount,
        margin = mata->m % threadCount;
    int first_row = (blockSize + 1) * myRank - Max(0, myRank-margin),
        last_row = first_row + blockSize + (myRank < margin);

    for(int i = first_row;i < last_row;i++)
        for(int j = 0;j < matb->n;j++)
            for(int k = 0;k < mata->n;k++)
                matc->mat[i][j] += mata->mat[i][k] * matb->mat[k][j];
    return NULL;
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    
    printf("Input m, n, k for the generation of matrix A and B.\n");
    scanf("%d %d %d", &m, &n, &k);
    
    threadCount = strtol(argv[1], NULL, 10);
    pVec = (pthread_t*)malloc(sizeof(pthread_t) * threadCount);
    rankVec = (int*)malloc(sizeof(int) * threadCount);
    mata = new_mat(m, n, 1);
    matb = new_mat(n, k, 1);
    matc = new_mat(m, k, 0);
    
#ifdef DEBUG
    startTime = omp_get_wtime();
    testMat = testmul(mata, matb);
    endTime = omp_get_wtime();
    printf("Baseline ends in %lfs.\n", endTime-startTime);
#endif

    startTime = omp_get_wtime();
    for(int i = 0;i < threadCount;i++) {
        rankVec[i] = i;
        pthread_create(&pVec[i], NULL, matmul_thread, &rankVec[i]);
    }
    for(int i = 0;i < threadCount;i++) {
        pthread_join(pVec[i], NULL);
    }
    endTime = omp_get_wtime();
    printf("Pthread implementation ends in %lfs.\n", endTime-startTime);

#ifdef DEBUG
    printf("Result check %s.\n", mat_is_equal(matc, testMat) ? "accepted" : "failed");
    free_mat(testMat);
#endif

    free(pVec);
    free(rankVec);
    
    free_mat(mata);
    free_mat(matb);
    free_mat(matc);
    return 0;
}