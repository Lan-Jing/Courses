#include "../include/MatrixOp.h"

// reference implementation without any optimization;
struct Matrix * testMul(struct Matrix *mata, struct Matrix *matb) {
    struct Matrix *resMat = NULL;
    if(mata->n != matb->m) {
        return resMat;
    } else {
        resMat = new_mat(mata->m, matb->n, 1);
    }

    for(int i = 0;i < mata->m;i++) 
        for(int j = 0;j < matb->n;j++)
            for(int k = 0;k < mata->n;k++)
                resMat->mat[i]->vec[j] += mata->mat[i]->vec[k] * matb->mat[k]->vec[j];
    return resMat;
}

// Optimized GEMM multiplication;
struct Matrix * BrutalMul(struct Matrix *mata, struct Matrix *matb) {
    struct Matrix *resMat = NULL;
    if(mata->n != matb->m) {
        return resMat;
    } else {
        resMat = new_mat(mata->m, matb->n, 1);
    }
    
    int i, threadCount = omp_get_num_procs();
#pragma omp parallel for num_threads(threadCount) default(none) shared(mata, matb, resMat) private(i)
    for(i = 0;i < mata->m;i += 8) {
        if(i+8 >= mata->m) {
            for(int p = i;p < mata->m;p++) {
                for(int j = 0;j < matb->n;j += (j+8 >= matb->n ? 1 : 8)) {
                    if(j+8 >= matb->n) {
                        mul_1_by_1(resMat, mata, matb, p, j);
                    } else {
                        mul_1_by_8(resMat, mata, matb, p, j);
                    }
                }
            }
        } else {
            for(int j = 0;j < matb->n;j += (j+8 >= matb->n ? 1 : 8)) {
                if(j+8 >= matb->n) {
                    mul_8_by_1(resMat, mata, matb, i, j);
                } else {
                    mul_8_by_8(resMat, mata, matb, i, j);
                }
            }
        }
    }
    return resMat;
}

struct Matrix * StrassenMul(struct Matrix *mata, struct Matrix *matb) {
    struct Matrix *resMat = NULL;
    if(mata->n != matb->m) {
        return resMat;
    }

    // setup lowerbound of those submatrices, to prevent massive memory operations;
    if(mata->m % 2 == 0 && mata->n % 2 == 0 && matb->m % 2 == 0 && matb->n % 2 == 0 &&
       Min(mata->m, mata->n) >= minSize && Min(matb->m, matb->n) >= minSize) {
        struct Matrix *subMatrix[8], *tmpMatrix[7], *partMatrix[4], *resMat1, *resMat2; 
        memset(subMatrix, 0, sizeof(subMatrix));
        memset(tmpMatrix, 0, sizeof(tmpMatrix));
        memset(partMatrix, 0, sizeof(partMatrix));

        struct Matrix *matPtr = NULL;
        int i, threadCount = omp_get_num_procs();
    
    #pragma omp parallel for num_threads(threadCount) default(none) shared(subMatrix, mata, matb) private(i, matPtr)
        for(i = 0;i < 8;i++) {
            matPtr = i <= 3 ? mata : matb;
            // be careful of the index from two different matrixes!
            int j = i % 4; 
            subMatrix[i] = submat(matPtr, j / 2 ? matPtr->m/2 : 0, j % 2 ? matPtr->n/2 : 0, j / 2 ? matPtr->m-1 : matPtr->m/2-1 , j % 2 ? matPtr->n-1 : matPtr->n/2-1);
        }
        // compute the temporary matrices needed;
        tmpMatrix[0] = StrassenMul(subMatrix[0], resMat1 = matrix_add_sub(subMatrix[5], subMatrix[7], 1)); 
        free_mat(resMat1);
        tmpMatrix[1] = StrassenMul(resMat1 = matrix_add_sub(subMatrix[0], subMatrix[1], 0), subMatrix[7]); 
        free_mat(resMat1);
        tmpMatrix[2] = StrassenMul(resMat1 = matrix_add_sub(subMatrix[2], subMatrix[3], 0), subMatrix[4]); 
        free_mat(resMat1);
        tmpMatrix[3] = StrassenMul(subMatrix[3], resMat1 = matrix_add_sub(subMatrix[6], subMatrix[4], 1)); 
        free_mat(resMat1);

        tmpMatrix[4] = StrassenMul(resMat1 = matrix_add_sub(subMatrix[0], subMatrix[3], 0), resMat2 = matrix_add_sub(subMatrix[4], subMatrix[7], 0)); 
        free_mat(resMat1), free_mat(resMat2);
        tmpMatrix[5] = StrassenMul(resMat1 = matrix_add_sub(subMatrix[1], subMatrix[3], 1), resMat2 = matrix_add_sub(subMatrix[6], subMatrix[7], 0));
        free_mat(resMat1), free_mat(resMat2);
        tmpMatrix[6] = StrassenMul(resMat1 = matrix_add_sub(subMatrix[0], subMatrix[2], 1), resMat2 = matrix_add_sub(subMatrix[4], subMatrix[5], 0)); 
        free_mat(resMat1), free_mat(resMat2);

        // place four corner sub-matrixes into the final one;
        partMatrix[0] = matrix_add_sub(resMat1 = matrix_add_sub(tmpMatrix[4], tmpMatrix[3], 0), resMat2 = matrix_add_sub(tmpMatrix[1], tmpMatrix[5], 1), 1);
        free_mat(resMat1), free_mat(resMat2);
        partMatrix[1] = matrix_add_sub(tmpMatrix[0], tmpMatrix[1], 0);
        partMatrix[2] = matrix_add_sub(tmpMatrix[2], tmpMatrix[3], 0);
        partMatrix[3] = matrix_add_sub(resMat1 = matrix_add_sub(tmpMatrix[4], tmpMatrix[0], 0), resMat2 = matrix_add_sub(tmpMatrix[2], tmpMatrix[6], 0), 1);
        free_mat(resMat1), free_mat(resMat2);

        resMat = combine_matrix(partMatrix[0], partMatrix[1], partMatrix[2], partMatrix[3]);

    #pragma omp parallel for num_threads(threadCount) default(none) shared(subMatrix) private(i)
        for(i = 0;i < 8;i++) free_mat(subMatrix[i]);
    #pragma omp parallel for num_threads(threadCount) default(none) shared(tmpMatrix) private(i)
        for(i = 0;i < 7;i++) free_mat(tmpMatrix[i]);
    #pragma omp parallel for num_threads(threadCount) default(none) shared(partMatrix) private(i)
        for(i = 0;i < 4;i++) free_mat(partMatrix[i]);
    } else {
        // directly call brutal method if matrices are small enough
        resMat = BrutalMul(mata, matb);
    }

    return resMat;
}