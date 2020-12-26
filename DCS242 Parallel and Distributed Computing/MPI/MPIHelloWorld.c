#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAXSize 100

int main() {
    char greeting[MAXSize];
    int comm_sz, my_rank;

    MPI_Init(NULL, NULL); // initilize MPI status.
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // get total amount of processes. 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // get rank of the current process.

    if (my_rank) {
        sprintf(greeting, "Greeting from process %d of %d!\n", my_rank, comm_sz);
        // printf greeting content into a string.
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        // then send it to the main process.
        /* three parameters are used to determine a message: 
           1. a pointer to the beginning of the message space.
           2. total size of the message, actually size for send and maximum size for recv.
           3. date type of the message.
        */
    }
    else {
        printf("Greeting from process 0 of %d!\n", comm_sz);
        for (int i = 1; i < comm_sz; i++) {
            MPI_Recv(greeting, MAXSize, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            /* synchronize all processes.
               1. specify the source process by parameter src, MPI_ANY_SOURCE for any coming message.
               2. tags are used for seperating distinct kinds of usage of messages, MPI_ANY_TAG for any message from the same source.
               3. comm is the domain of the linked set of processes.
               4. status is for determing result parameters the are unknown to receivers: the error code, the tag of the message
                  and the rank of the source process(if not specified).
               5. use function MPI_GET_count(). Pass the status and get the actuall size of the message.

               Why seperate the function for counting size from the receiving one? Size we count may probably not used every time, thus wasting execution time.
            */
            printf("%s", greeting);
        }
    }
    MPI_Finalize(); // clean up MPI 
    return 0;
}