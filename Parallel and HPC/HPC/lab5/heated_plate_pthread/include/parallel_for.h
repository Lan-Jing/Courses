#include <stdio.h>

typedef void* (*parallel_functor)(void*);

typedef struct index {
    int start, end, increment;
} parallel_index;

enum reduction { none = 0, sum = 1, max = 2 };

typedef struct args {
    void *privateArgs;
    int privateArgSize;
    int start, end, inc;
    double returnVal;
} thread_args;

int index_begin(parallel_index index, int thread_num, int num_threads);
int index_end(parallel_index index, int thread_num, int num_threads);
void parallel_for(parallel_index index, parallel_functor functor, int num_threads, 
                  void *privateArgs, int privateArgSize,
                  enum reduction type, double *val);
