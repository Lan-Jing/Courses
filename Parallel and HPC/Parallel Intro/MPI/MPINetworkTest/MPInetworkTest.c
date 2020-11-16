#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAXSize 5*(1<<20)
#define testTime 10000

int main() {
    char greeting[MAXSize];
    int comm_sz, my_rank;

    MPI_Init(NULL, NULL); // initilize MPI status.
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // get total amount of processes. 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // get rank of the current process.

    memset(greeting, 1, sizeof(greeting));
    if (my_rank) {
        double averageLocalTime = 0;
        for(int i = 0;i < testTime;i++) {
            // set barrier to sync two processes
            MPI_Barrier(MPI_COMM_WORLD);
            double localStartTime = MPI_Wtime();
            // printf greeting content into a string.
            MPI_Send(greeting, MAXSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(greeting, MAXSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double localEndTime = MPI_Wtime();
            averageLocalTime += localEndTime-localStartTime;
        }
        printf("Sent:%d bytes after %lfs\n", MAXSize, averageLocalTime/testTime);
    }
    else {
        double averageRemoteTime = 0;
        for(int i = 0;i < testTime;i++) {
            MPI_Barrier(MPI_COMM_WORLD);
            double remoteStartTime = MPI_Wtime();
            MPI_Recv(greeting, MAXSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(greeting, MAXSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            double remoteEndTime = MPI_Wtime();
            averageRemoteTime += remoteEndTime-remoteStartTime;
        }
        printf("Received:%d bytes after %lfs\n", MAXSize, averageRemoteTime/testTime);
    }
    MPI_Finalize(); // clean up MPI 
    return 0;
}