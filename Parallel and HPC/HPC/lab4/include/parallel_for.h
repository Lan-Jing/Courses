#include <stdio.h>

typedef void* (*parallel_functor)(void*);
typedef struct index {
    int start, end, increment;
} parallel_index;

int index_begin(parallel_index index, int thread_num, int num_threads);
int index_end(parallel_index index, int thread_num, int num_threads);

void* thread_work(void *rank);
void parallel_for(parallel_index index, parallel_functor functor, int num_threads, 
                  void *privateArgs);