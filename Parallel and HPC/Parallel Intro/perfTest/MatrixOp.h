#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
    definition of vector/matrix structures and basic operation on them.
*/

struct Vector {
    int length;
    long long int *vec;
};

struct Matrix {
    int m, n;
    struct Vector **mat;
};

struct Vector * new_vec(int length, int mode); // return a pointer to the new vector;
void print_vec(struct Vector *vecp); // display the given vector;
void free_vec(struct Vector *vecp); // free a given vector, always pairs with a new_vec;
struct Matrix * new_mat(int m, int n, int mode); // return a pointer to the new matrix;
void print_mat(struct Matrix *matp); // display the given matrix;
void free_mat(struct Matrix *matp); // free a given matrix, always pairs with a new_mat;

struct Matrix * subMat(struct Matrix *mat, int upperLefty, int upperLeftx, 
                                           int lowerRighty, int lowerRightx);
// return a sub-matrix defined by the two corners;
struct Matrix * MatrixAddSub(struct Matrix *mata, struct Matrix *matb, int mode);
// return the add/sub result of two matrixes of the same size;
struct Matrix * combineMatrix(struct Matrix *upperLeftMat, struct Matrix *upperRightMat, 
                              struct Matrix *lowerLeftMat, struct Matrix *lowerRightMat);
// combine four sub-matrixes into one;
struct Matrix * BrutalMul(struct Matrix *mata, struct Matrix *matb);
struct Matrix * StrassenMul(struct Matrix *mata, struct Matrix *matb);
// two matrix multiplcation algorithms;