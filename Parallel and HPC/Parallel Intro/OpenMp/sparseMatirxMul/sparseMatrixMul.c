#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include <time.h>
#include "mmio.h"

// rounds of test
#define mulTimes 50

#define randLimit (1 << 16)

// matrix information;
MM_typecode mtxType;
int m, n, l;
// matrix space
int *rowCord, *colCord, *colOffset;
double *valp, *resp;
// omp locks;
omp_lock_t *locks;

int main(int argc, char *argv[]) {
    // get number of threads
    int threadCount = argc < 3 ? 8 : strtol(argv[2], NULL, 10);

    // open matrix file
    FILE* mtx = fopen(argv[1], "rb");
    if(mtx == NULL) {
        printf("Failed to open matrix file!\n");
        return 0;
    }

    // read information of the matrix type;
    mm_read_banner(mtx, &mtxType);
    // read information of the matrix size;
    mm_read_mtx_crd_size(mtx, &m, &n, &l);

    // allocate a randome vector for testing;
    double *vecp = (double *)malloc(sizeof(double) * n);
    if(vecp == NULL) {
        printf("Failed to allocate random vector\n");
        return 0;
    }

    // allocate space to store the matrix;
    rowCord = (int *)malloc(sizeof(int) * l);
    colCord = (int *)malloc(sizeof(int) * l);
    valp = (double *)malloc(sizeof(double) * l);
    // store the result;
    colOffset = (int *)malloc(sizeof(int) * n);
    resp = (double *)malloc(sizeof(double) * m);
    if(rowCord == NULL || colCord == NULL || valp == NULL || colOffset == NULL || resp == NULL) {
        printf("Failed to allocate space for the matrix\n");
        return 0;
    } else {
        // use the predefined method to read data;
        int readRes = mm_read_mtx_crd_data(mtx, m, n, l, rowCord, colCord, valp, mtxType);
        if(readRes == MM_PREMATURE_EOF) {
            printf("Failed to read the matrix from file\n");
            return 0;
        } else {
            printf("Performing multiplication for a %d*%d matrix with %d entries\n", m, n, l);
        }
        // transfer it to compressed CSC format;
        // index of the current entry
        int nowInd = 0;
        for(int nowCol = 0;nowCol < n;nowCol++) {
            // nothing on this column;
            colOffset[nowCol] = INT32_MAX;
            if(nowInd < l && colCord[nowInd]-1 == nowCol) {
                // the first offset of the column
                colOffset[nowCol] = nowInd;
                // offset the index to the next column;
                while(nowInd < l && colCord[nowInd]-1 == nowCol)
                    nowInd++;
            }
        }
        // close the matrix file;
        fclose(mtx);
    }

    memset(resp, 0, sizeof(resp));
    // each row has its own lock;
    locks = (omp_lock_t *)malloc(sizeof(omp_lock_t) * m);
    for(int i = 0;i < m;i++)
        omp_init_lock(locks + i);

    double averageTime = 0;
    for(int i = 0;i < mulTimes;i++) {
        // setup vector
        for(int j = 0;j < n;j++)
            vecp[j] = rand() % randLimit;
        // start counting
        double startTime = omp_get_wtime();
        int col, ind;
    #   pragma omp parallel for num_threads(threadCount) \
        default(none), private(col, ind), shared(n, l, colOffset, colCord, rowCord, locks, valp, vecp, resp)
        for(col = 0;col < n;col++) {
            // for all entry of the same column;
            for(ind = colOffset[col];ind < l && colCord[ind]-1 == col;ind++) {
                // lock the corresponding row;
                omp_set_lock(locks + (rowCord[ind]-1));
                resp[rowCord[ind]-1] += valp[ind] * vecp[col];
                omp_unset_lock(locks + (rowCord[ind]-1));
            }
        }
        // end counting
        double endTime = omp_get_wtime();
        averageTime += (endTime-startTime);
    }
    averageTime /= mulTimes;
    printf("The average duration of %d times Matrix multiplication: %lfs\n", mulTimes, averageTime);

    FILE *resFtp = fopen("result.txt","wb");
    for(int i = 0;i < m;i++)
        fprintf(resFtp, "%lf\n", resp[i]);
    fclose(resFtp);

    // destory locks;
    for(int i = 0;i < m;i++)
        omp_destroy_lock(locks + i);
    free(locks);
    // destory arrays;
    free(rowCord);
    free(colCord);
    free(colOffset);
    free(valp);
    free(resp);
    free(vecp);

    return 0;
}