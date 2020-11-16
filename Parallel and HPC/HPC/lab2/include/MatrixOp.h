#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <immintrin.h>
#include <omp.h>

#define minSize 512

int Min(int a, int b);
int Max(int a, int b);

//============== Vector/Matrix definition ==========
struct Vector {
    int length;
    float *vec;
};
struct Vector* new_vec(int length, int mode);
void print_vec(struct Vector *vecp);
void free_vec(struct Vector *vecp);

struct Matrix {
    int m, n;
    struct Vector **mat;
};
struct Matrix* new_mat(int m, int n, int mode);
void print_mat(struct Matrix* matp);
void free_mat(struct Matrix* matp);

// ============== helpers for Strassen multiplication ===================
// return a sub-matrix defined by the two corners;
struct Matrix* submat(struct Matrix *mat, int upperLefty, int upperLeftx, 
                                           int lowerRighty, int lowerRightx);

// return the add/sub result of two matrixes of the same size;
struct Matrix* matrix_add_sub(struct Matrix *mata, struct Matrix *matb, int mode);

// check if two matrices are equal
int mat_is_equal(struct Matrix *mata, struct Matrix *matb);

// combine four sub-matrixes into one;
struct Matrix* combine_matrix(struct Matrix *upperLeftMat, struct Matrix *upperRightMat, 
                              struct Matrix *lowerLeftMat, struct Matrix *lowerRightMat);

// copy matrix a into matrix b, when mat a is smaller than b;
// copy range specified by parameters
int copy_matrix(struct Matrix *mata, struct Matrix *matb, int uly, int ulx, int dy, int dx);

// two matrix multiplcation algorithms;
struct Matrix* testMul(struct Matrix *mata, struct Matrix *matb);
struct Matrix* BrutalMul(struct Matrix *mata, struct Matrix *matb);
struct Matrix* StrassenMul(struct Matrix *mata, struct Matrix *matb);

// helper function to multiply a block of elements;
void mul_1_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_1_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_8_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_8_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);