#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include "../include/helper.h"
#include "../include/parallel_for.h"

// The two function below assume that the index be correctly assigned.
// If increment < 0 while start < end, an infinite for loop could occur.
int index_begin(parallel_index index, int thread_num, int num_threads)
{
    if(index.increment < 0) {
        swap(&index.start, &index.end);
        index.increment = -index.increment;
    }

    int tot = (index.end - index.start) / index.increment,
        blockSize = tot / num_threads,
        margin = tot % num_threads;
    return index.start + ((blockSize + 1) * thread_num - Max(0, thread_num - margin)) * index.increment;
}

int index_end(parallel_index index, int thread_num, int num_threads)
{
    if(index.increment < 0) {
        swap(&index.start, &index.end);
        index.increment = -index.increment;
    }

    int tot = (index.end - index.start) / index.increment,
        blockSize = tot / num_threads,
        margin = tot % num_threads;
    return index_begin(index, thread_num, num_threads) + (blockSize + (thread_num < margin)) * index.increment;
}

// This function partitions work into #threads pieces,
// then forks threads to execute each part.
// Normally privateArgs is useless, since by default openmp does NOT initialize private variables
void parallel_for(parallel_index index, parallel_functor functor, int num_threads, 
                  void *privateArgs, int privateArgSize, 
                  enum reduction type, double *val)
{
    pthread_t *pVec = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    thread_args *argVec = (thread_args*)malloc(sizeof(thread_args) * num_threads);
    for(int i = 0;i < num_threads;i++) {
        argVec[i].privateArgSize = privateArgSize;
        argVec[i].privateArgs = malloc(privateArgSize);
        
        argVec[i].start = index_begin(index, i, num_threads);
        argVec[i].end = index_end(index, i, num_threads);
        argVec[i].inc = index.increment;

        pthread_create(pVec + i, NULL, functor, argVec + i);
    }

    for(int i = 0;i < num_threads;i++) {
        pthread_join(pVec[i], NULL);

        free(argVec[i].privateArgs);
        // impose reduction to given variable
        if(type == sum) {
            *val += argVec[i].returnVal;
        } else if (type == max) {
            *val = fmax(*val, argVec[i].returnVal);
        }
    }
    
    free(pVec);
    free(argVec);
}