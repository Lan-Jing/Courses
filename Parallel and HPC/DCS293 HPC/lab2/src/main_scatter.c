#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../include/mpi_comm.h"

#define start_clock ttime = 0, stime = MPI_Wtime();
#define resume_clock stime = MPI_Wtime();
#define end_clock etime = MPI_Wtime(), ttime += etime-stime;

// parameters of the random matrices.
int rank, size;
// each process have to know the size of their sub-matrices
int start, end, m, n, k;
// timekeeper
double stime, etime, ttime;

int main(int argc, char *argv[]) {
    srand(time(0));
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    struct Matrix *mata = NULL, *matb = NULL, *resMat = NULL, *tmpMat = NULL;
#ifdef DEBUG
    struct Matrix *testMat = NULL;
#endif

    if(!rank) {
        printf("Enter parameters m, n and k for the multiplication programm.\n");
        scanf("%d %d %d", &m, &n, &k);
        mata = new_mat(m, n, 0);
        matb = new_mat(n, k, 0);
#ifdef DEBUG
        testMat = BrutalMul(mata, matb);
        // print_mat(mata), print_mat(matb);
#endif   
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&k, 1, MPI_INT, 0, MPI_COMM_WORLD);

    start = compute_start(rank, size, m);
    end = compute_end(rank, size, m);
    if(rank) {
        mata = new_mat(end-start, n, 1);
        matb = new_mat(n, k, 1);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    start_clock
    // broadcast mat b, scatter mat a
    broadcast_mat_by_row(0, n, matb, 0, rank);
    scatter_mat_by_row(mata, 0, rank, size);
    if(!rank) {
        resMat = mata;
        mata = submat(resMat, 0, 0, end-1, n-1);
        free_mat(resMat);
    }
    end_clock

    resMat = StrassenMul(mata, matb);

    resume_clock
    // gather matrix here
    if(!rank) {
        tmpMat = submat(resMat, 0, 0, resMat->m-1, resMat->n-1);
        free_mat(resMat);
        resMat = new_mat(m, k, 1);
    }
    gather_mat_by_row(resMat, 0, rank, size);

    if(!rank) {
        copy_matrix(tmpMat, resMat, start, 0, tmpMat->m, tmpMat->n);
        free(tmpMat);

        end_clock
        printf("Implementation with set communication ends in %lfs.\n", etime-stime);
#ifdef DEBUG
        printf("Result check %s\n", mat_is_equal(resMat, testMat) ? "accepted" : "failed");
        // print_mat(resMat), print_mat(testMat);
        free_mat(testMat);
#endif
    }

    free_mat(mata), free_mat(matb), free_mat(resMat);
    MPI_Finalize();
    return 0;
}