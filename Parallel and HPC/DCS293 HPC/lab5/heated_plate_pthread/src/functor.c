#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "../include/parallel_for.h"
#include "../include/functor.h"

// shared variables.
extern double w[MM][NN], u[MM][NN], mean, diff;
extern int M, N;

// The outer method constructs arguments, passes variables(if needed) for the inner functions
// corresponds to the "#pragma" statement.
void initialization(int num_threads)
{
    parallel_index index;
    index.start = 1; index.end = M-1; index.increment = 1;
    parallel_for(index, init_loop1, num_threads, NULL, 0, none, NULL);

    index.start = 0; index.end = N; index.increment = 1;
    parallel_for(index, init_loop2, num_threads, NULL, 0, none, NULL);
}

// The inner functions extract variables from arguments then execute the loop
// corresponds to the body of a for loop
void* init_loop1(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    for (int i = start; i < end; i += inc) {
        w[i][0] = 100.0;
        w[i][N-1] = 100.0;
    }
    return NULL;
}

void* init_loop2(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    for(int i = start;i < end;i += inc) {
        w[M-1][i] = 100.0;
        w[0][i] = 0.0;
    }
    return NULL;
}

void compute_mean(int num_threads)
{
    parallel_index index;
    index.start = 1; index.end = M - 1; index.increment = 1;
    parallel_for(index, compute_mean_loop1, num_threads, NULL, 0, sum, &mean);

    index.start = 0; index.end = N; index.increment = 1;
    parallel_for(index, compute_mean_loop2, num_threads, NULL, 0, sum, &mean);
}

void* compute_mean_loop1(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    ((thread_args*)args)->returnVal = 0.0;
    for (int i = start; i < end; i += inc) {
        ((thread_args*)args)->returnVal += w[i][0] + w[i][N-1];
    }
}

void* compute_mean_loop2(void* args) 
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    ((thread_args*)args)->returnVal = 0.0;
    for (int i = start; i < end; i += inc) {
        ((thread_args*)args)->returnVal += w[M-1][i] + w[0][i];
    }
}

void set_mean(int num_threads)
{
    parallel_index index;
    index.start = 1; index.end = M-1; index.increment = 1;
    parallel_for(index, set_mean_loop, num_threads, NULL, 0, none, NULL);
}

void* set_mean_loop(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    for(int i = start;i < end;i += inc) {
        for(int j = 1;j < N-1;j++)
            w[i][j] = mean;
    }
    return NULL;
}

void iteration(int num_threads)
{
    parallel_index index;
    index.start = 0; index.end = M; index.increment = 1;
    parallel_for(index, copy_temp_loop, num_threads, NULL, 0, none, NULL);

    index.start = 1; index.end = M - 1; index.increment = 1;
    parallel_for(index, iteration_loop, num_threads, NULL, 0, none, NULL);
}

void* copy_temp_loop(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    for(int i = start;i < end;i += inc) {
        for(int j = 0;j < N;j++)
            u[i][j] = w[i][j];
    }
    return NULL;
}

void* iteration_loop(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    for(int i = start;i < end;i += inc) {
        for(int j = 1;j < N-1;j++)
            w[i][j] = (u[i-1][j]+u[i+1][j]+u[i][j-1]+u[i][j+1]) / 4.0;
    }

    return NULL;
}

void compute_diff(int num_threads)
{
    parallel_index index;
    index.start = 1; index.end = M - 1; index.increment = 1;
    parallel_for(index, compute_diff_loop, num_threads, NULL, 0, max, &diff);
}

void *compute_diff_loop(void *args)
{
    int start = ((thread_args*)args)->start;
    int end = ((thread_args*)args)->end;
    int inc = ((thread_args*)args)->inc;
    double local_diff = 0.0;
    for(int i = start;i < end;i += inc) {
        for(int j = 1;j < N-1;j++) 
            local_diff = fmax(local_diff, fabs(w[i][j]-u[i][j]));
    }
    ((thread_args*)args)->returnVal = local_diff;
    return NULL;
}