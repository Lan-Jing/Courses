#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "../include/mat.h"
#include "../include/helper.h"
#include "../include/parallel_for.h"

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

mat_t* ompmul(mat_t *mata, mat_t *matb) 
{
    mat_t *matc = NULL;
    if(mata->n != matb->m) {
        return matc;
    } else {
        matc = new_mat(mata->m, matb->n, 0);
    }

    int i;

    #pragma omp parallel for \
    default(none) shared(mata, matb, matc) private(i)
    for(i = 0;i < mata->m;i++) {
        for(int j = 0;j < matb->n;j++)
            for(int k = 0;k < mata->n;k++)
                matc->mat[i][j] += mata->mat[i][k] * matb->mat[k][j];
    }
    return matc;
}

mat_t* paramul(mat_t *mata, mat_t *matb, int num_threads)
{
    extern mat_t *matc;

    matc = NULL;
    if(mata->n != matb->m) {
        return matc;
    } else {
        matc = new_mat(mata->m, matb->n, 0);
    }

    parallel_index index;
    index.start = 0, index.end = mata->m, index.increment = 1;

    int privateArgs = 0;
    parallel_for(index, paramul_loop, num_threads, &privateArgs);

    return matc;
}

void* paramul_loop(void *privateArgs) 
{
    // declare these variables as global
    extern mat_t *mata, *matb, *matc;

    parallel_index myIndex = ((parallel_index*)privateArgs)[0];
    
    for(int i = myIndex.start;i < myIndex.end;i += myIndex.increment) {
        for(int j = 0;j < matb->n;j++)
            for(int k = 0;k < mata->n;k++)
                matc->mat[i][j] += mata->mat[i][k] * matb->mat[k][j];
    }
    return NULL;
}