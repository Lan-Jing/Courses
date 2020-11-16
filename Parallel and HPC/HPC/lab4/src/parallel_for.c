#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../include/helper.h"
#include "../include/parallel_for.h"

// make arguments global so that every thread can see.
int g_num_threads = 1;
int g_rank = 0;
pthread_mutex_t rankMutex;

parallel_index g_index;
parallel_functor g_functor;

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

/*
    function that specified by functor should access private variables by function parameters passed through,
    and shared ones by global variables;
*/
void* thread_work(void *privateArgs)
{
    // each thread gets its rank.
    int myRank = g_rank++;
    pthread_mutex_unlock(&rankMutex);
    // and its own copy of private variables.
    void *myPrivateArgs = malloc(((int*)privateArgs)[0] + sizeof(parallel_index));

    parallel_index myIndex;
    myIndex.start = index_begin(g_index, myRank, g_num_threads);
    myIndex.end = index_end(g_index, myRank, g_num_threads);
    myIndex.increment = g_index.increment;

    memcpy(myPrivateArgs, &myIndex, sizeof(myIndex));

    g_functor(myPrivateArgs);

    free(myPrivateArgs);
    return NULL;
}

void parallel_for(parallel_index index, parallel_functor functor, int num_threads, 
                  void *privateArgs)
{
    // make shared variables global
    g_index = index, g_num_threads = num_threads, g_functor = functor;

    pthread_t *pVec = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);

    pthread_mutex_init(&rankMutex, NULL);
    for(int i = 0;i < num_threads;i++) {
        pthread_mutex_lock(&rankMutex);
        pthread_create(pVec + i, NULL, thread_work, privateArgs);
    }
    pthread_mutex_destroy(&rankMutex);
    for(int i = 0;i < num_threads;i++) {
        pthread_join(pVec[i], NULL);
    }
    
    free(pVec);
}