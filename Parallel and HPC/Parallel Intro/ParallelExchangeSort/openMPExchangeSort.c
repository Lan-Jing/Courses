#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void print_vec(int* vec, int l) {
	for (int i = 0; i < l; i++)
		printf("%d ", vec[i]);
	printf("\n");
}

void serial_exchange_sort(int* vec, int l) {
	int* tmpVec = (int*)malloc(l * sizeof(int));
	memcpy(tmpVec, vec, l * sizeof(int));
	int flag = 0;
	for (int round = 0; round < l; round++) {
		flag = 0;
		for (int i = round % 2; i + 1 < l; i++) {
			if (tmpVec[i] > tmpVec[i + 1]) continue;
			flag = 1; 
			swap(&tmpVec[i], &tmpVec[i + 1]);
		}
		if (!flag) break;
	}
//	print_vec(tmpVec, l);
}

void openMP_exchange_sort(int* vec, int l) {
	int threadCount = 1;
#ifdef _OPENMP	
	threadCount = omp_get_num_threads();
#endif

	int* tmpVec = (int*)malloc(l * sizeof(int));
	memcpy(tmpVec, vec, l * sizeof(int));
	int round, i;
#	pragma omp parallel num_threads(threadCount) \ 
	default(none) shared(round, l, tmpVec) private(i)
	// option parallel creates new threads for concurrency. 
	// Setting up multithreads before the target loop prevents unnecessary initialization.
	for (round = 0; round < l; round++)
#		pragma omp for
	// option for only optimize the for loop below.
		for (i = round % 2; i < l - 1; i++)
			if (tmpVec[i] > tmpVec[i + 1]) swap(&tmpVec[i], &tmpVec[i + 1]);
//	print_vec(tmpVec, l);
}

int main(int argv, char* argc[]) {
	int threadCount = 1;
#ifdef _OPENMP
	threadCount = strtol(argc[1], NULL, 10); // get number of threads
#endif

	int vecLength, * vec;
	printf("Enter length of the unsorted array:");
	scanf("%d", &vecLength);
	vec = (int*)malloc(vecLength * sizeof(int));

	srand((unsigned int)time(NULL));
	for (int i = 0; i < vecLength; i++)
		vec[i] = rand();
//	print_vec(vec, vecLength);

	clock_t startTime = clock();
	serial_exchange_sort(vec, vecLength);
	clock_t endTime = clock();
	printf("Serial exchange sort completed in: %lf\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);

	startTime = clock();
	openMP_exchange_sort(vec, vecLength);
	endTime = clock();
	printf("openMP parallel exchange sort completed in: %lf", (double)(endTime - startTime) / CLOCKS_PER_SEC);

	return 0;
}