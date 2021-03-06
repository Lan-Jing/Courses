#include "../include/MatrixOp.h"

//==================== vector operations ===========================
// put random values into the matrix if mode == 0
struct Vector * new_vec(int length, int mode) {
    struct Vector *vecp = (struct Vector *)malloc(sizeof(struct Vector));
    vecp->length = length;
    vecp->vec = (float*)malloc(vecp->length * sizeof(float));

    for(int i = 0;i < length;i++) 
        vecp->vec[i] = mode ? 0 : (((float)rand()) / RAND_MAX);
    return vecp;
}

void free_vec(struct Vector *vecp) {
    free(vecp->vec);
    free(vecp);
}

void print_vec(struct Vector *vecp) {
    if(vecp == NULL) return ;
    for(int i = 0;i < vecp->length;i++) 
        printf("%lf ", vecp->vec[i]);
    printf("\n");
}

//==================== matrix operations ===========================
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

void print_mat(struct Matrix *matp) {
    if(matp == NULL) return ;
    for(int i = 0;i < matp->m;i++)
        print_vec(matp->mat[i]);
    printf("\n");
}

// take two corners and return the corresponding sub-matrix
struct Matrix * submat(struct Matrix *Mat, int upperLefty, int upperLeftx, int lowerRighty, int lowerRightx) {
    struct Matrix *resMat = NULL;
    if(upperLefty > lowerRighty || upperLeftx > lowerRightx) return resMat; 
    if(upperLefty < 0 || upperLeftx < 0 || lowerRighty >= Mat->m || lowerRightx >= Mat->n) return resMat; 

    resMat = new_mat(lowerRighty - upperLefty + 1, lowerRightx - upperLeftx + 1, 1);
    int i, threadCount = omp_get_num_procs();
#pragma omp parallel for num_threads(threadCount) default(none) shared(upperLefty, upperLeftx, lowerRighty, lowerRightx, resMat, Mat) private(i)
    for(i = upperLefty;i <= lowerRighty;i++) 
        for(int j = upperLeftx;j <= lowerRightx;j++) {
            resMat->mat[i-upperLefty]->vec[j-upperLeftx] = Mat->mat[i]->vec[j];
        }
    return resMat;
}

// matrix addition/subtraction, optimized by OpenMP and AVX2
// add for mode == 0 and sub for mode == 1
int sign[2]={1,-1};
struct Matrix * matrix_add_sub(struct Matrix *mata, struct Matrix *matb, int mode) {
    struct Matrix *resMat = NULL;
    if(mata->m != matb->m || mata->n != matb->n)
        return resMat;
    
    resMat = new_mat(mata->m, mata->n, 1);
    int i, threadCount = omp_get_num_procs();

#pragma omp parallel for num_threads(threadCount) shared(resMat, mata, matb, sign, mode) private(i)
    for(i = 0;i < mata->m;i++) {
        for(int k = 0;k < mata->n / 8;k++) {
            __m256 intVec1 = _mm256_loadu_ps(&mata->mat[i]->vec[8*k]);
            __m256 intVec2 = _mm256_loadu_ps(&matb->mat[i]->vec[8*k]);

            __m256 intVec3 = mode ? _mm256_sub_ps(intVec1, intVec2) : 
                                    _mm256_add_ps(intVec1, intVec2);

            _mm256_storeu_ps(&resMat->mat[i]->vec[8*k], intVec3);
        }
        for(int k = (mata->n / 8)*8;k < mata->n;k++)
            resMat->mat[i]->vec[k] = mata->mat[i]->vec[k] + sign[mode] * matb->mat[i]->vec[k];
    }
    return resMat;
}

int mat_is_equal(struct Matrix *mata, struct Matrix *matb) {
    if(mata->m != matb->m || mata->n != matb->n)
        return 0;
    
    for(int i = 0;i < mata->m;i++)
        for(int j = 0;j < mata->n;j++)
            if(abs(mata->mat[i]->vec[j]-matb->mat[i]->vec[j]) > 1e-3) {
#ifdef DEBUG
                printf("%lf, %lf\n", mata->mat[i]->vec[j], matb->mat[i]->vec[j]);
#endif
                return 0;
            }
    return 1;
}


// combine four sub-matrices into one matrix;
struct Matrix *combine_matrix(struct Matrix *upperLeftMat, struct Matrix *upperRightMat, 
                             struct Matrix *lowerLeftMat, struct Matrix *lowerRightMat) {
    struct Matrix *resMat = NULL;
    if(upperLeftMat->n != lowerLeftMat->n || upperRightMat->n != lowerRightMat->n) return resMat;
    if(upperLeftMat->m != upperRightMat->m || lowerLeftMat->m != lowerRightMat->m) return resMat;

    resMat = new_mat(upperLeftMat->m + lowerLeftMat->m, upperLeftMat->n + upperRightMat->n, 1);
    int i, threadCount = omp_get_num_procs();

#pragma omp parallel for num_threads(threadCount) default(none) shared(upperLeftMat, upperRightMat, resMat) private(i)
    for(i = 0;i < upperLeftMat->m;i++) {
        for(int j = 0;j < upperLeftMat->n;j++)
            resMat->mat[i]->vec[j] = upperLeftMat->mat[i]->vec[j];
        for(int j = 0;j < upperRightMat->n;j++)
            resMat->mat[i]->vec[j+upperLeftMat->n] = upperRightMat->mat[i]->vec[j];
    }

#pragma omp parallel for num_threads(threadCount) default(none) shared(upperLeftMat, lowerLeftMat, lowerRightMat, resMat) private(i)
    for(i = 0;i < lowerLeftMat->m;i++) {
        for(int j = 0;j < lowerLeftMat->n;j++)
            resMat->mat[i+upperLeftMat->m]->vec[j] = lowerLeftMat->mat[i]->vec[j];
        for(int j = 0;j < lowerRightMat->n;j++)
            resMat->mat[i+upperLeftMat->m]->vec[j+upperLeftMat->n] = lowerRightMat->mat[i]->vec[j];
    }
    return resMat;
}

// copy matrix a into matrix b
int copy_matrix(struct Matrix *mata, struct Matrix *matb, int uly, int ulx, int dy, int dx) {
    if(uly < 0 || ulx < 0 || dy < 0 || dx < 0 || dy > mata->m || dx > mata->n ||
       uly + dy > matb->m || ulx + dx > matb->n)
        return 0;
    
    int y, x, threadCount = omp_get_num_procs(), i;
#pragma omp parallel for num_threads(threadCount) default(none) shared(uly, ulx, dy, dx, mata, matb) private(i, x, y)
    for(i = 0;i < dy;i++)
        for(int j = 0;j < dx;j++) {
            y = uly + i, x = ulx + j;
            matb->mat[y]->vec[x] = mata->mat[i]->vec[j];
        }
}

// optimization for the brutal multiplication
void mul_1_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j) {
    register float res = 0;
    for(int k = 0;k < mata->n;k++)
        res += mata->mat[i]->vec[k] * matb->mat[k]->vec[j];
    resMat->mat[i]->vec[j] = res;
}

void mul_1_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j) {
    register float resMat0, resMat1, resMat2, resMat3, resMat4, resMat5, resMat6, resMat7, mata0;
    resMat0 = resMat1 = resMat2 = resMat3 = resMat4 = resMat5 = resMat6 = resMat7 = 0;
    for(int k = 0;k < mata->n;k++) {
        mata0 = mata->mat[i]->vec[k];
        resMat0 += mata0 * matb->mat[k]->vec[j];
        resMat1 += mata0 * matb->mat[k]->vec[j+1];
        resMat2 += mata0 * matb->mat[k]->vec[j+2];
        resMat3 += mata0 * matb->mat[k]->vec[j+3];
        resMat4 += mata0 * matb->mat[k]->vec[j+4];
        resMat5 += mata0 * matb->mat[k]->vec[j+5];
        resMat6 += mata0 * matb->mat[k]->vec[j+6];
        resMat7 += mata0 * matb->mat[k]->vec[j+7];
    }
    resMat->mat[i]->vec[j] = resMat0;
    resMat->mat[i]->vec[j+1] = resMat1;
    resMat->mat[i]->vec[j+2] = resMat2;
    resMat->mat[i]->vec[j+3] = resMat3;
    resMat->mat[i]->vec[j+4] = resMat4;
    resMat->mat[i]->vec[j+5] = resMat5;
    resMat->mat[i]->vec[j+6] = resMat6;
    resMat->mat[i]->vec[j+7] = resMat7;
}

void mul_8_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j) {
    mul_1_by_1(resMat, mata, matb, i, j);
    mul_1_by_1(resMat, mata, matb, i+1, j);
    mul_1_by_1(resMat, mata, matb, i+2, j);
    mul_1_by_1(resMat, mata, matb, i+3, j);
    mul_1_by_1(resMat, mata, matb, i+4, j);
    mul_1_by_1(resMat, mata, matb, i+5, j);
    mul_1_by_1(resMat, mata, matb, i+6, j);
    mul_1_by_1(resMat, mata, matb, i+7, j);
}

void mul_8_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j) {
    __m256 vecb, veca0, veca1, veca2, veca3, veca4, veca5, veca6, veca7,
           resMat0, resMat1, resMat2, resMat3, resMat4, resMat5, resMat6, resMat7;
    for(int k = 0;k < mata->n;k++) {
        vecb = _mm256_loadu_ps(&matb->mat[k]->vec[j]);
        veca0 = _mm256_broadcast_ss(&mata->mat[i]->vec[k]);
        veca1 = _mm256_broadcast_ss(&mata->mat[i+1]->vec[k]);
        veca2 = _mm256_broadcast_ss(&mata->mat[i+2]->vec[k]);
        veca3 = _mm256_broadcast_ss(&mata->mat[i+3]->vec[k]);
        veca4 = _mm256_broadcast_ss(&mata->mat[i+4]->vec[k]);
        veca5 = _mm256_broadcast_ss(&mata->mat[i+5]->vec[k]);
        veca6 = _mm256_broadcast_ss(&mata->mat[i+6]->vec[k]);
        veca7 = _mm256_broadcast_ss(&mata->mat[i+7]->vec[k]);

        resMat0 = k ? _mm256_add_ps(resMat0, _mm256_mul_ps(veca0, vecb)) : 
                      _mm256_mul_ps(veca0, vecb);
        resMat1 = k ? _mm256_add_ps(resMat1, _mm256_mul_ps(veca1, vecb)) : 
                      _mm256_mul_ps(veca1, vecb);
        resMat2 = k ? _mm256_add_ps(resMat2, _mm256_mul_ps(veca2, vecb)) : 
                      _mm256_mul_ps(veca2, vecb);
        resMat3 = k ? _mm256_add_ps(resMat3, _mm256_mul_ps(veca3, vecb)) : 
                      _mm256_mul_ps(veca3, vecb);
        resMat4 = k ? _mm256_add_ps(resMat4, _mm256_mul_ps(veca4, vecb)) : 
                      _mm256_mul_ps(veca4, vecb);
        resMat5 = k ? _mm256_add_ps(resMat5, _mm256_mul_ps(veca5, vecb)) : 
                      _mm256_mul_ps(veca5, vecb);
        resMat6 = k ? _mm256_add_ps(resMat6, _mm256_mul_ps(veca6, vecb)) : 
                      _mm256_mul_ps(veca6, vecb);
        resMat7 = k ? _mm256_add_ps(resMat7, _mm256_mul_ps(veca7, vecb)) : 
                      _mm256_mul_ps(veca7, vecb);
    }
    _mm256_storeu_ps(&resMat->mat[i]->vec[j], resMat0);
    _mm256_storeu_ps(&resMat->mat[i+1]->vec[j], resMat1);
    _mm256_storeu_ps(&resMat->mat[i+2]->vec[j], resMat2);
    _mm256_storeu_ps(&resMat->mat[i+3]->vec[j], resMat3);
    _mm256_storeu_ps(&resMat->mat[i+4]->vec[j], resMat4);
    _mm256_storeu_ps(&resMat->mat[i+5]->vec[j], resMat5);
    _mm256_storeu_ps(&resMat->mat[i+6]->vec[j], resMat6);
    _mm256_storeu_ps(&resMat->mat[i+7]->vec[j], resMat7);
}

//==================== Other helper operations ===========================
int Min(int a, int b) { return a > b ? b : a; }
int Max(int a, int b) { return a > b ? a : b; }