#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP 
#include <omp.h>
#endif
// If openMP is not supported, DO NOT try to use any openMP thing and try to keep it runable as well.

void Hello() {
#ifdef _OPENMP
	int myRank = omp_get_thread_num();
	int threadCount = omp_get_num_threads();
#else
	int myRank = 0;
	int threadCount = 1; // if openMP is not available, try to code for single-thread programm.
#endif
	printf("Hello world from thread %d of %d\n", myRank, threadCount);
}

int main(int argc, char* argv[]) {
	int threadCount = strtol(argv[1], NULL, 10);
	// "10" represent the base of the counted number.
	// If threadCount argument is detected as a non-positive number, the system will assign one itself.

#pragma omp parallel num_threads(threadCount)
	Hello();

	return 0;
}