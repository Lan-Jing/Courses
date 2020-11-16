#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// included for Intel AVX optimization
#include <immintrin.h>
// included for openmp
#include <omp.h>

//================= Optimization options ================
// Get some of the modules to be executed in parallel;
#define OPENMP

// Enable AVX2 instructions in some modules; 
#define AVX

// Use register to speedup memory references;
#define REGISTERS

// Define granularity of matrix multiplications;
// #define VECTORMUL
#define BLOCKMUL

// define the granularity of the blockmul, 
// requiring BLOCKMUL and AVX to be enabled!!!
#define eightByEight

// define the minimal size of the submatrices.
// a small lowerbound would significantly increase memory overhead of the Strassen algorithm;
#define minSize 512

/* ============== Vector/Matrix  ======================
    definition of vector/matrix structures and basic operations on them.
*/
int Min(int a, int b);
int Max(int a, int b);

struct Vector {
    int length;
    float *vec;
};
struct Vector * new_vec(int length, int mode); 
void print_vec(struct Vector *vecp); 
void free_vec(struct Vector *vecp); 

struct Matrix {
    int m, n;
    struct Vector **mat;
};
struct Matrix * new_mat(int m, int n, int mode); 
void print_mat(struct Matrix *matp); 
void free_mat(struct Matrix *matp); 

// ============== helpers for Strassen multiplication ===================
// return a sub-matrix defined by the two corners;
struct Matrix * subMat(struct Matrix *mat, int upperLefty, int upperLeftx, 
                                           int lowerRighty, int lowerRightx);

// return the add/sub result of two matrixes of the same size;
struct Matrix * matrixAddSub(struct Matrix *mata, struct Matrix *matb, int mode);

// combine four sub-matrixes into one;
struct Matrix * combineMatrix(struct Matrix *upperLeftMat, struct Matrix *upperRightMat, 
                              struct Matrix *lowerLeftMat, struct Matrix *lowerRightMat);

// two matrix multiplcation algorithms;
struct Matrix * testMul(struct Matrix *mata, struct Matrix *matb);
struct Matrix * BrutalMul_4(struct Matrix *mata, struct Matrix *matb);
struct Matrix * BrutalMul_8(struct Matrix *mata, struct Matrix *matb);
struct Matrix * StrassenMul(struct Matrix *mata, struct Matrix *matb);

// helper function to multiply a block of elements;
void mul_1_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_1_by_4(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_4_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_4_by_4(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_4_by_4_AVX(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);

void mul_1_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_8_by_1(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);
void mul_8_by_8(struct Matrix *resMat, struct Matrix *mata, struct Matrix *matb, int i, int j);