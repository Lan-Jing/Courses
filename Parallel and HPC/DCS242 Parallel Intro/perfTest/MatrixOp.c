#include "MatrixOp.h"

void print_vec(struct Vector *vecp) {
    if(vecp == NULL) return ;
    for(int i = 0;i < vecp->length;i++) 
        printf("%lld ", vecp->vec[i]);
    printf("\n");
}

void print_mat(struct Matrix *matp) {
    if(matp == NULL) return ;
    for(int i = 0;i < matp->m;i++)
        print_vec(matp->mat[i]);
}

struct Vector * new_vec(int length, int mode) {
    struct Vector *vecp = (struct Vector *)malloc(sizeof(struct Vector));
    vecp->length = length;
    vecp->vec = (long long int*)malloc(vecp->length * sizeof(long long int));

    if(!mode) for(int i = 0;i < length;i++) vecp->vec[i] = rand() % (1<<8);
    return vecp;
    // print_vec(vecp);
}

void free_vec(struct Vector *vecp) {
    free(vecp->vec);
    free(vecp);
}

struct Matrix * new_mat(int m, int n, int mode) {
    struct Matrix *matp = (struct Matrix*)malloc(sizeof(struct Matrix));
    matp->m = m, matp->n = n;
    matp->mat = (struct Vector**)malloc(m * sizeof(struct Vector*));
    for(int i = 0;i < m;i++) {
        matp->mat[i] = new_vec(n, mode);
    }
    return matp;
}

void free_mat(struct Matrix *matp) {
    for(int i = 0;i < matp->m;i++)
        free_vec(matp->mat[i]);
    free(matp);
}

struct Matrix * subMat(struct Matrix *Mat, int upperLefty, int upperLeftx, int lowerRighty, int lowerRightx) {
    struct Matrix *resMat = NULL;
    if(upperLefty > lowerRighty || upperLeftx > lowerRightx) return resMat; // size of the matrix must be positive;
    if(upperLefty < 0 || upperLeftx < 0 || lowerRighty >= Mat->m || lowerRightx >= Mat->n) return resMat; // location should fit in valid range;

    resMat = new_mat(lowerRighty - upperLefty + 1, lowerRightx - upperLeftx + 1, 1);
    for(int i = upperLefty;i <= lowerRighty;i++) 
        for(int j = upperLeftx;j <= lowerRightx;j++) {
            resMat->mat[i-upperLefty]->vec[j-upperLeftx] = Mat->mat[i]->vec[j];
        }
    return resMat;
}

int sign[2]={1,-1};
struct Matrix * MatrixAddSub(struct Matrix *mata, struct Matrix *matb, int mode) {
// add for mode == 0 and sub for mode == 1
    struct Matrix *resMat = NULL;
    if(mata->m != matb->m || mata->n != matb->n)
        return resMat;
    
    resMat = new_mat(mata->m, mata->n, 1);
    for(int i = 0;i < mata->m;i++)
        for(int j = 0;j < mata->n;j++) 
            resMat->mat[i]->vec[j] = mata->mat[i]->vec[j] + sign[mode] * matb->mat[i]->vec[j];
     return resMat;
}

struct Matrix *combineMatrix(struct Matrix *upperLeftMat, struct Matrix *upperRightMat, 
                             struct Matrix *lowerLeftMat, struct Matrix *lowerRightMat) {
    struct Matrix *resMat = NULL;
    if(upperLeftMat->n != lowerLeftMat->n || upperRightMat->n != lowerRightMat->n) return resMat;
    if(upperLeftMat->m != upperRightMat->m || lowerLeftMat->m != lowerRightMat->m) return resMat;

    resMat = new_mat(upperLeftMat->m + lowerLeftMat->m, upperLeftMat->n + upperRightMat->n, 1);
    for(int i = 0;i < upperLeftMat->m;i++) {
        for(int j = 0;j < upperLeftMat->n;j++)
            resMat->mat[i]->vec[j] = upperLeftMat->mat[i]->vec[j];
        for(int j = 0;j < upperRightMat->n;j++)
            resMat->mat[i]->vec[j+upperLeftMat->n] = upperRightMat->mat[i]->vec[j];
    }
    for(int i = 0;i < lowerLeftMat->m;i++) {
        for(int j = 0;j < lowerLeftMat->n;j++)
            resMat->mat[i+upperLeftMat->m]->vec[j] = lowerLeftMat->mat[i]->vec[j];
        for(int j = 0;j < lowerRightMat->n;j++)
            resMat->mat[i+upperLeftMat->m]->vec[j+upperLeftMat->n] = lowerRightMat->mat[i]->vec[j];
    }
    return resMat;
}