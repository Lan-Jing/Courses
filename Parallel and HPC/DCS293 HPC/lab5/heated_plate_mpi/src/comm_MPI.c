#include <stdlib.h>
#include "../include/comm_MPI.h"

int Max(int a, int b) { return a > b ? a : b; }

double **init_mat(int m, int n)
{
    if(m < 0 || n < 0)
        return NULL;
    
    double **res = (double**)malloc(sizeof(double*) * m);
    for(int i = 0;i < m;i++)
        res[i] = (double*)malloc(sizeof(double) * n);

    return res;
}

void free_mat(double **mat, int m)
{
    for(int i = 0;i < m;i++)
        free(mat[i]);
    free(mat);
}

// return a [start, end) interval
int compute_start(int rank, int num_procs, int size) 
{
    int blockSize = size / num_procs, margin = size % num_procs;
    return (blockSize + 1) * rank - Max(rank - margin, 0);
}

int compute_end(int rank, int num_procs, int size)
{
    int blockSize = size / num_procs, margin = size % num_procs;
    int l = blockSize + (rank < margin);
    return compute_start(rank, num_procs, size) + l;
}

int pack_row(MPI_Datatype *dtype, double **mat, int n)
{
    MPI_Aint displacement = 0;
    MPI_Datatype type = MPI_DOUBLE;
    int blockLength = n;

    MPI_Type_create_struct(1, &blockLength, &displacement, &type, dtype);
    MPI_Type_commit(dtype);

    return 1;
}

void row_exchange(double **myMat, int myRank, int num_procs, int m, int n)
{
    int nextRank = myRank + 1, prevRank = myRank - 1;

    MPI_Datatype firstRow, lastRow;
    pack_row(&firstRow, myMat, n);
    pack_row(&lastRow, myMat, n);

    if(myRank != num_procs-1) {
        MPI_Sendrecv(myMat[m-2], 1, lastRow, nextRank, myRank,
                     myMat[m-1], 1, firstRow, nextRank, nextRank, MPI_COMM_WORLD, NULL);
    }
    if(myRank != 0) {
        MPI_Sendrecv(myMat[1], 1, firstRow, prevRank, myRank,
                     myMat[0], 1, lastRow, prevRank, prevRank, MPI_COMM_WORLD, NULL);
    }
}