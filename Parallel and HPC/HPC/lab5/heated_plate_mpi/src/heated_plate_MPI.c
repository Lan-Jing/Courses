#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include "../include/comm_MPI.h"

#define M 500
#define N 500

int num_procs, myRank;

int main(int argc, char *argv[])
{
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    int start = compute_start(myRank, num_procs, M-2),
        end = compute_end(myRank, num_procs, M-2),
        rowSize = end-start+2;

    double **u = init_mat(rowSize, N),
           **w = init_mat(rowSize, N);
    if(u == NULL || w == NULL) {
        printf("Memory allocation failed.\n");
        MPI_Finalize();
        return 0;
    }
    
    double mean = 0.0;
    if(myRank == 0) {
        for(int j = 0;j < N;j++) 
            w[0][j] = 0.0;
    } else if(myRank == num_procs-1) {
        for(int j = 0;j < N;j++)
            w[rowSize-1][j] = 100.0, mean += 100.0;
    }
    
    for(int i = 1;i < rowSize-1;i++)
        w[i][0] = w[i][N-1] = 100.0, mean += 200.0;

    MPI_Allreduce(&mean, &mean, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    mean /= (double)(2 * M + 2 * N - 4);
    if(myRank == 0) {
        printf("\n  MEAN = %lf\n", mean);
    }

    for(int i = (myRank ? 0 : 1);i < (myRank == num_procs-1 ? rowSize-1 : rowSize);i++) {
        for(int j = 1;j < N-1;j++)
            w[i][j] = mean;
    }

    int iterations = 0, iterations_print = 1;
    double epsilon = 0.001, diff = epsilon;
    double wtime;

    if(myRank == 0) {
        wtime = omp_get_wtime();
    }
    while(epsilon <= diff) {
        for(int i = 0;i < rowSize;i++)
            for(int j = 0;j < N;j++)
                u[i][j] = w[i][j];

        for(int i = 1;i < rowSize-1;i++)
            for(int j = 1;j < N-1;j++)
                w[i][j] = (u[i-1][j]+u[i+1][j]+u[i][j-1]+u[i][j+1]) / 4.0;

        diff = 0.0;
        for(int i = 1;i < rowSize-1;i++)
            for(int j = 1;j < N-1;j++) 
                diff = fmax(diff, fabs(u[i][j]-w[i][j]));
        MPI_Allreduce(&diff, &diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        row_exchange(w, myRank, num_procs, rowSize, N);
        
        iterations++;
        if (myRank == 0 && iterations == iterations_print) {
            printf("  %8d  %f\n", iterations, diff);
            iterations_print *= 2;
        }
    }
    if(myRank == 0) {
        wtime = omp_get_wtime() - wtime;
        printf("  %8d  %f\n", iterations, diff);
        printf("  Wallclock time = %f\n", wtime);
    }

    free_mat(u, rowSize);
    free_mat(w, rowSize);
    MPI_Finalize();
    return 0;
}