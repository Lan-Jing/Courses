#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define blockSize 10

#define len 10000000
#define rightBound 100

int threadCount = 0;
pthread_mutex_t index_mutex;
pthread_mutex_t global_mutex;

double array[len];
int global_index = 0;
double global_sum = 0,
       check_sum = 0;

inline double float_rand(float r)
{
    return ((double)rand()/(double)(RAND_MAX/r));
}

// Threads attempt to exclusively fetch a global index
// then add arr[index] into their local sums
void* thread_sum()
{
    double local_sum = 0;
    int myIndex;
    while(1) {
        pthread_mutex_lock(&index_mutex);
#ifdef blockSum
        myIndex = global_index;
        global_index += blockSize;
#else
        myIndex = global_index++;
#endif
        pthread_mutex_unlock(&index_mutex);
        if(myIndex >= len) {
            break;
        } else {
#ifdef blockSum
            for(int i = 0;i < blockSize;i++)
                local_sum += array[myIndex + i];
#else
            local_sum += array[myIndex];
#endif
        }
    }

    pthread_mutex_lock(&global_mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&global_mutex);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    srand(time(NULL));
    for(int i = 0;i < len;i++) {
        array[i] = float_rand(rightBound);
    }

    // a serial check.
    double startTime, endTime;
    startTime = omp_get_wtime();
    for(int i = 0;i < len;i++) {
        check_sum += array[i];
    }
    endTime = omp_get_wtime();
#ifdef DEBUG
    printf("Baseline ends in %lfs.\n", endTime-startTime);
#endif

    threadCount = strtol(argv[1], NULL, 10);
    pthread_t *pVec = (pthread_t*)malloc(sizeof(pthread_t) * threadCount);
    
    pthread_mutex_init(&index_mutex, NULL);
    pthread_mutex_init(&global_mutex, NULL);
    startTime = omp_get_wtime();
    for(int i = 0;i < threadCount;i++) {
        pthread_create(&pVec[i], NULL, thread_sum, NULL);
    }
    for(int i = 0;i < threadCount;i++) {
        pthread_join(pVec[i], NULL);
    }
    endTime = omp_get_wtime();
    pthread_mutex_destroy(&index_mutex);
    pthread_mutex_destroy(&global_mutex);

#ifdef DEBUG
    printf("Pthread method ends in %lfs.\n", endTime-startTime);
#endif
    printf("Pthread: %lf\nSerial:%lf\n", global_sum, check_sum);
    if(abs(global_sum - check_sum) < 1e-5) {
        printf("Result check accepted.\n");
    } else {
        printf("Result check failed.\n");
    }

    free(pVec);
    return 0;
}