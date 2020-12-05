#include <stdio.h>
#include <mpi.h>

double** init_mat(int m, int n);
void free_mat(double **mat, int m);

int compute_start(int rank, int num_procs, int size);
int compute_end(int rank, int num_procs, int size);

int pack_row(MPI_Datatype *dtype, double **mat, int n);
void row_exchange(double **myMat, int myRank, int num_procs, int m, int n);