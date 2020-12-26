#include <stdio.h>
#include <stdlib.h>
#include "mat.h"

int Max(int a, int b)
{
    return a > b ? a : b;
}

double float_rand(int r)
{
    return (double)rand() / (double)(RAND_MAX/r);
}

mat_t* new_mat(int m, int n, int mode) 
{
    mat_t *mat = (mat_t*)malloc(sizeof(mat_t));
    mat->m = m, mat->n = n;
    mat->mat = (vec_t*)malloc(sizeof(vec_t) * m);
    for(int i = 0;i < m;i++) {
        mat->mat[i] = (vec_t)malloc(sizeof(dtype) * n);
        for(int j = 0;j < n;j++)
            mat->mat[i][j] = mode ? float_rand(rand_rightBound) : 0;
    }
    return mat;
}

void free_mat(mat_t *matp)
{
    for(int i = 0;i < matp->m;i++)
        free(matp->mat[i]);
    free(matp->mat);
    free(matp);
}

void print_mat(mat_t *matp)
{
    for(int i = 0;i < matp->m;i++) {
        for(int j = 0;j < matp->n;j++)
            printf("%lf ", matp->mat[i][j]);
        printf("\n");
    }
}

mat_t* testmul(mat_t *mata, mat_t *matb)
{
    mat_t *matc = NULL;
    if(mata->n != matb->m) {
        return matc;
    } else {
        matc = new_mat(mata->m, matb->n, 0);
    }

    for(int i = 0;i < mata->m;i++)
        for(int j = 0;j < matb->n;j++)
            for(int k = 0;k < mata->n;k++)
                matc->mat[i][j] += mata->mat[i][k] * matb->mat[k][j];
    return matc;
}

int mat_is_equal(mat_t *mata, mat_t *matb)
{
    if(mata->m != matb->m || mata->n != matb->n)
        return 0;
    for(int i = 0;i < mata->m;i++)
        for(int j = 0;j < mata->n;j++)
            if(mata->mat[i][j] != matb->mat[i][j])
                return 0;
    return 1;
}