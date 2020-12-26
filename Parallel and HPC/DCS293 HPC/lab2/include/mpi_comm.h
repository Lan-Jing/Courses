#include <stdio.h>
#include <mpi.h>

#include "MatrixOp.h"

int compute_start(int rank, int prosCount, int size);
int compute_end(int rank, int prosCount, int size);

// ============= packing matrix and communication ============

int pack_subMatrix(MPI_Datatype *matType, struct Matrix *matp, int start, int end);
int unpack_subMatrix(void *recvBuffer, struct Matrix *matp, int start, int end, int len);

void send_mat_by_row(int row_start, int row_end, struct Matrix *matp, int sourceRank, int tarRank);
void recv_mat_by_row(int row_start, int row_end, int row_len, struct Matrix *matp, int sourceRank);
void broadcast_mat_by_row(int row_start, int row_end, struct Matrix *matp, int sourceRank, int rank);
void scatter_mat_by_row(struct Matrix *matp, int sourceRank, int rank, int prosCount);
void gather_mat_by_row(struct Matrix *matp, int tarRank, int rank, int prosCount);