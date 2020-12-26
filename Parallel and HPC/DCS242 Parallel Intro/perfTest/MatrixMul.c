#include "MatrixOp.h"

struct Matrix * BrutalMul(struct Matrix *mata, struct Matrix *matb) {
    struct Matrix *resMat = NULL;
    if(mata->n != matb->m) {
        return resMat;
    }
    
    resMat = new_mat(mata->m, matb->n, 1);
    for(int i = 0;i < mata->m;i++)
        for(int j = 0;j < matb->n;j++) {
            resMat->mat[i]->vec[j] = 0;
            for(int k = 0;k < mata->n;k++)
                resMat->mat[i]->vec[j] += mata->mat[i]->vec[k] * matb->mat[k]->vec[j];
        }

    return resMat;
}

struct Matrix * StrassenMul(struct Matrix *mata, struct Matrix *matb) {
    struct Matrix *resMat = NULL;
    if(mata->n != matb->m) {
        return resMat;
    }

    if(mata->m % 2 == 0 && mata->n % 2 == 0 && matb->m % 2 == 0 && matb->n % 2 == 0) {
        struct Matrix *subMatrix[8], *tmpMatrix[7], *partMatrix[4], 
                      *resMat1, *resMat2; // to track temporary result for later delete;
        memset(subMatrix, 0, sizeof(subMatrix));
        memset(tmpMatrix, 0, sizeof(tmpMatrix));
        memset(partMatrix, 0, sizeof(partMatrix));

        struct Matrix *matPtr = NULL;
        for(int i = 0;i < 8;i++) {
            matPtr = i <= 3 ? mata : matb;
            int j = i % 4; // be careful of the index from two different matrixes!
            subMatrix[i] = subMat(matPtr, j / 2 ? matPtr->m/2 : 0, j % 2 ? matPtr->n/2 : 0, j / 2 ? matPtr->m-1 : matPtr->m/2-1 , j % 2 ? matPtr->n-1 : matPtr->n/2-1);
        }
        tmpMatrix[0] = StrassenMul(subMatrix[0], resMat1 = MatrixAddSub(subMatrix[5], subMatrix[7], 1));
        free_mat(resMat1);
        tmpMatrix[1] = StrassenMul(resMat1 = MatrixAddSub(subMatrix[0], subMatrix[1], 0), subMatrix[7]);
        free_mat(resMat1);
        tmpMatrix[2] = StrassenMul(resMat1 = MatrixAddSub(subMatrix[2], subMatrix[3], 0), subMatrix[4]);
        free_mat(resMat1);
        tmpMatrix[3] = StrassenMul(subMatrix[3], resMat1 = MatrixAddSub(subMatrix[6], subMatrix[4], 1));
        free_mat(resMat1);
        tmpMatrix[4] = StrassenMul(resMat1 = MatrixAddSub(subMatrix[0], subMatrix[3], 0), resMat2 = MatrixAddSub(subMatrix[4], subMatrix[7], 0));
        free_mat(resMat1), free_mat(resMat2);
        tmpMatrix[5] = StrassenMul(resMat1 = MatrixAddSub(subMatrix[1], subMatrix[3], 1), resMat2 = MatrixAddSub(subMatrix[6], subMatrix[7], 0));
        free_mat(resMat1), free_mat(resMat2);
        tmpMatrix[6] = StrassenMul(resMat1 = MatrixAddSub(subMatrix[0], subMatrix[2], 1), resMat2 = MatrixAddSub(subMatrix[4], subMatrix[5], 0)); 
        free_mat(resMat1), free_mat(resMat2);
        // free space allocated by function calls.

        partMatrix[0] = MatrixAddSub(resMat1 = MatrixAddSub(tmpMatrix[4], tmpMatrix[3], 0), resMat2 = MatrixAddSub(tmpMatrix[1], tmpMatrix[5], 1), 1);
        free_mat(resMat1), free_mat(resMat2);
        partMatrix[1] = MatrixAddSub(tmpMatrix[0], tmpMatrix[1], 0);
        partMatrix[2] = MatrixAddSub(tmpMatrix[2], tmpMatrix[3], 0);
        partMatrix[3] = MatrixAddSub(resMat1 = MatrixAddSub(tmpMatrix[4], tmpMatrix[0], 0), resMat2 = MatrixAddSub(tmpMatrix[2], tmpMatrix[6], 0), 1);
        free_mat(resMat1), free_mat(resMat2);
        resMat = combineMatrix(partMatrix[0], partMatrix[1], partMatrix[2], partMatrix[3]);
        // place four corner sub-matrixes into the final one.

        for(int i = 0;i < 8;i++) free_mat(subMatrix[i]);
        for(int i = 0;i < 7;i++) free_mat(tmpMatrix[i]);
        for(int i = 0;i < 4;i++) free_mat(partMatrix[i]);
        // free all temporary space.
    } else {
        resMat = BrutalMul(mata, matb);
    }

    return resMat;
}