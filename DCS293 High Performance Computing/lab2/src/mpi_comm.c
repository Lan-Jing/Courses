#include "../include/mpi_comm.h"

// compute start and end of each small block
// [start, end)
int compute_start(int rank, int prosCount, int size) {
    int blockSize = size/prosCount, margin = size%prosCount;
    return (blockSize + 1) * rank - Max(rank - margin, 0);
}

int compute_end(int rank, int prosCount, int size) {
    int blockSize = size/prosCount, margin = size%prosCount;
    int l = blockSize + (rank < margin);
    return compute_start(rank, prosCount, size) + l;
}

// divide the original matrix by rows, [start, end)
// to construct a MPI datatype for communication
int pack_subMatrix(MPI_Datatype *matType, struct Matrix *matp, int start, int end) {
    int m = matp->m;
    if(start < 0 || start >= m || end <= 0 || end > m || end <= start)
        return 0;
    
    int count = end - start;
    int *blockLength = (int*)malloc(sizeof(int) * count);
    MPI_Aint *displacement = (MPI_Aint*)malloc(sizeof(MPI_Aint) * count);
    MPI_Datatype *types = (MPI_Datatype*)malloc(sizeof(MPI_Datatype) * count);

    MPI_Aint baseAddr, nowAddr;
    
    for(int i = start, j = 0;i < end;i++, j=i-start) {
        if(!j)
            MPI_Get_address((const void*)matp->mat[i]->vec, &baseAddr);
        MPI_Get_address((const void*)matp->mat[i]->vec, &nowAddr);
        
        blockLength[j] = matp->n;
        displacement[j] = nowAddr - baseAddr;
        types[j] = MPI_FLOAT;
    }

    MPI_Type_create_struct(count, blockLength, displacement, types, matType);
    MPI_Type_commit(matType);

    free(blockLength);
    free(displacement);
    free(types);

    return 1;
}

// unpack the MPI receive buffer and put floats back to the matrix
int unpack_subMatrix(void *recvBuffer, struct Matrix *matp, int start, int end, int len) {
    int i, threadCount = omp_get_num_procs();
#pragma omp parallel for num_threads(threadCount) default(none) shared(start, end, len, matp, recvBuffer) private(i)
    for(i = start;i < end;i++) {
        int j = i - start;
        matp->mat[i]->length = len;
        for(int k = 0;k < len;k++)
            matp->mat[i]->vec[k] = ((float*)recvBuffer)[len*j + k];
    }
}

// send (part of) the matrix to a specific process
void send_mat_by_row(int row_start, int row_end, struct Matrix *matp, int sourceRank, int tarRank) {
    MPI_Datatype matType;
    
    pack_subMatrix(&matType, matp, row_start, row_end);
    // Be careful, only send 1 user-defined datatype to workers
    MPI_Send(matp->mat[row_start]->vec, 1, matType, tarRank, sourceRank, MPI_COMM_WORLD);
    MPI_Type_free(&matType);
}

// receive (part of) the matrix from the master process 
void recv_mat_by_row(int row_start, int row_end, int row_len, struct Matrix *matp, int sourceRank) {
    int bufSize = (row_end - row_start) * row_len;
    float *recvBuffer = (float*)malloc(sizeof(float) * bufSize);
    MPI_Status recvResult;

    MPI_Recv(recvBuffer, bufSize, MPI_FLOAT, sourceRank, sourceRank, MPI_COMM_WORLD, &recvResult);
    unpack_subMatrix(recvBuffer, matp, row_start, row_end, row_len);
    free(recvBuffer);
}

// broadcast (part of) the matrix to all nodes in the communicator
// receivers should initialize matp first.
void broadcast_mat_by_row(int row_start, int row_end, struct Matrix *matp, int sourceRank, int rank) {
    if(rank == sourceRank) {
        MPI_Datatype matType;

        pack_subMatrix(&matType, matp, row_start, row_end);
        MPI_Bcast(matp->mat[row_start]->vec, 1, matType, sourceRank, MPI_COMM_WORLD);
    } else {
        int bufSize = (row_end - row_start) * matp->n;
        float *recvBuffer = (float*)malloc(sizeof(float) * bufSize);

        MPI_Bcast(recvBuffer, bufSize, MPI_FLOAT, sourceRank, MPI_COMM_WORLD);
        unpack_subMatrix(recvBuffer, matp, 0, row_end-row_start, matp->n);
    }
}

// scatter (part of) the matrix to nodes in the communicator
// receivers should initialize matp first.
void scatter_mat_by_row(struct Matrix *matp, int sourceRank, int rank, int prosCount) {
    int  recvCount = matp->m * matp->n,
        *sendCounts = (int*)malloc(sizeof(int) * prosCount),
        *displs = (int*)malloc(sizeof(int) * prosCount);
    float *sendBuffer = (float*)malloc(sizeof(float) * matp->m * matp->n), 
          *recvBuffer = (float*)malloc(sizeof(float) * matp->m * matp->n);

    if(rank == sourceRank) {
        // gather data from matrix.
        int i, threadCount = omp_get_num_procs();
    #pragma omp parallel for num_threads(threadCount) default(none) shared(sendBuffer, matp) private(i)
        for(i = 0;i < matp->m;i++)
            for(int j = 0;j < matp->n;j++)
                sendBuffer[matp->n*i+j] = matp->mat[i]->vec[j];
        
        for(int i = 0;i < prosCount;i++) {
            int row_start = compute_start(i, prosCount, matp->m),
                row_end = compute_end(i, prosCount, matp->m);
            sendCounts[i] = (row_end - row_start) * matp->n;
            displs[i] = row_start * matp->n;
        }
    }
    
    MPI_Scatterv(sendBuffer, sendCounts, displs, MPI_FLOAT, recvBuffer, recvCount, MPI_FLOAT, sourceRank, MPI_COMM_WORLD);
    
    if(rank != sourceRank) {
        unpack_subMatrix(recvBuffer, matp, 0, matp->m, matp->n);
    }

    free(sendBuffer), free(recvBuffer);
    free(sendCounts), free(displs);
}

// gather matrix from nodes in the communicator
// receiver should initalize matp first.
void gather_mat_by_row(struct Matrix *matp, int tarRank, int rank, int prosCount) {
    int  sendCount = matp->m * matp->n,
        *recvCounts = (int*)malloc(sizeof(int) * prosCount),
        *displs = (int*)malloc(sizeof(int) * prosCount);
    float *sendBuffer = (float*)malloc(sizeof(float) * matp->m * matp->n),
          *recvBuffer = (float*)malloc(sizeof(float) * matp->m * matp->n);

    if(rank != tarRank) {
        int i, threadCount = omp_get_num_procs();
    #pragma omp parallel for num_threads(threadCount) default(none) shared(sendBuffer, matp) private(i)
        for(i = 0;i < matp->m;i++)
            for(int j = 0;j < matp->n;j++)
                sendBuffer[matp->n*i+j] = matp->mat[i]->vec[j];
    } else {
        for(int i = 0;i < prosCount;i++) {
            // skip the receiver itself
            if(i == rank) {
                recvCounts[i] = displs[i] = 0;
                continue;
            }

            int row_start = compute_start(i, prosCount, matp->m),
                row_end = compute_end(i, prosCount, matp->m);
            recvCounts[i] = (row_end - row_start) * matp->n;
            displs[i] = row_start * matp->n;
        }
    }

    MPI_Gatherv(sendBuffer, sendCount, MPI_FLOAT, recvBuffer, recvCounts, displs, MPI_FLOAT, tarRank, MPI_COMM_WORLD);

    if(rank == tarRank) {
        // the receiver has to fill its own part into the matrix
        unpack_subMatrix(recvBuffer, matp, 0, matp->m, matp->n);
    }

    free(sendBuffer), free(recvBuffer);
    free(recvCounts), free(displs);
}