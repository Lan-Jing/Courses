# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>
# include "../include/functor.h"
# include "../include/parallel_for.h"

int num_threads = 1;

const int M = MM;
const int N = NN;

double u[MM][NN], w[MM][NN];
double diff, mean = 0.0;

int main ( int argc, char *argv[] )
{
  double epsilon = 0.001;
  int i, j;
  int iterations, iterations_print;
  double wtime;

  printf ( "\n" );
  printf ( "HEATED_PLATE_PTHREAD\n" );
  printf ( "  C/Pthread version\n" );
  printf ( "  A program to solve for the steady state temperature distribution over a rectangular plate.\n" );
  printf ( "\n" );
  printf ( "  Spatial grid of %d by %d points.\n", M, N );
  printf ( "  The iteration will be repeated until the change <= %e\n", epsilon ); 
  printf ( "  Number of threads availble =     %d\n", omp_get_max_threads() );
  printf ( "  Number of threads used     =     %d\n", num_threads = strtol(argv[1], NULL, 10));

  // initialize edges of the plate, then compute the mean value.
  initialization(num_threads); 
  compute_mean(num_threads);

  mean = mean / ( double ) ( 2 * M + 2 * N - 4 );
  printf ( "\n  MEAN = %f\n", mean );

  // set up the rest of the plate.
  set_mean(num_threads);

  iterations = 0;
  iterations_print = 1;
  printf ( "\n Iteration  Change\n\n" );

  diff = epsilon;
  wtime = omp_get_wtime();
  while ( epsilon <= diff )
  {
    diff = 0.0;

    // update the plate, then compute the error
    iteration(num_threads);
    compute_diff(num_threads);

    iterations++;
    if ( iterations == iterations_print )
    {
      printf ( "  %8d  %f\n", iterations, diff );
      iterations_print *= 2;
    }
  } 
  wtime = omp_get_wtime ( ) - wtime;

  printf ( "  %8d  %f\n\n", iterations, diff );
  printf ( "  Error tolerance achieved.\n" );
  printf ( "  Wallclock time = %f\n\n", wtime );
  printf ( "HEATED_PLATE_PTHREAD:\n" );
  printf ( "  Normal end of execution.\n" );

  return 0;
}
