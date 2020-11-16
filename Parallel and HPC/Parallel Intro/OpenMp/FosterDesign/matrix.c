#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define size 1000
#define method2

void print_vec(int *vec, int l) {
    for(int i = 0;i < l;i++)
        printf("%d ", vec[i]);
    printf("\n");
}

void print_mat(int mat[size][size], int m, int n) {
    for(int i = 0;i < m;i++)
        print_vec(mat[i], n);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int mat[size][size], res[size], vector[size];

    memset(res, 0, sizeof(res));
    for(int i = 0;i < size;i++) {
        vector[i] = rand() % 10;
        for(int j = 0;j < size;j++)
            mat[i][j] = rand();
    }

    // print_mat(mat, size, size);
    // printf("\n");
    // print_vec(vector, size);

    int threadCount = 1;
#ifdef _OPENMP
    threadCount = argc > 1 ? strtol(argv[1], NULL, 10) : 1;
#endif

    int row, col;
    double startT, endT;

    printf("Using serial method\n");
    memset(res, 0, sizeof(res));

    startT = omp_get_wtime();
    for(row = 0;row < size;row++)
        for(col = 0;col < size;col++)
            res[row] += mat[row][col] * vector[col];
    endT = omp_get_wtime();
    printf("execution time: %f\n",endT-startT);

    printf("Using method 1\n");
    memset(res, 0, sizeof(res));

    startT = omp_get_wtime();
#   pragma omp parallel for num_threads(threadCount) schedule(guided) \
    default(none) shared(res, mat, vector) private(row, col)
    for(row = 0;row < size;row++) {
        for(col = 0;col < size;col++) 
            res[row] += mat[row][col] * vector[col];
    }
    endT = omp_get_wtime();
    printf("execution time: %f\n",endT-startT);

    printf("Using method 2\n");
    memset(res, 0, sizeof(res));
    
    startT = omp_get_wtime();
    int tmp;
    for(row = 0;row < size;row++)
#       pragma omp parallel for num_threads(threadCount) schedule(guided) \
        default(none) shared(res, mat, vector, row) private(col, tmp)
        for(col = 0;col < size;col++) {
            tmp = mat[row][col] * vector[col];
#           pragma omp critical
            res[row] += tmp;
        }
    endT = omp_get_wtime();
    printf("execution time: %f\n",endT-startT);
    
    return 0;
}