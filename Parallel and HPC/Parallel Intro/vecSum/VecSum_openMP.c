#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void print_vec(int* vec, int l) {
	for (int i = 0; i < l; i++)
		printf("%d ", vec[i]);
	printf("\n");
}

int main(int argv, char* argc[]) {
	int threadNum = 1;
#ifdef _OPENMP
	threadNum = strtol(argc[1], NULL, 10);
#endif
	int vecLength;

	printf("Enter length of the summed vector:");
	scanf("%d", &vecLength);

	int* vec1, * vec2, * vec3;
	vec1 = (int*)malloc(vecLength * sizeof(int));
	vec2 = (int*)malloc(vecLength * sizeof(int));
	vec3 = (int*)malloc(vecLength * sizeof(int));
	if (vec1 == NULL || vec2 == NULL || vec3 == NULL) {
		printf("Memory allocation failed.\n");
		return 0;
	}

	srand((unsigned int)time(NULL));
	for (int i = 0; i < vecLength; i++)
		vec1[i] = rand(), vec2[i] = rand();
	//print_vec(vec1, vecLength), print_vec(vec2, vecLength);

	double startTime = omp_get_wtime();
	int i;
#ifdef _OPENMP
	#pragma omp parallel for num_threads(threadNum) \
	default(none) shared(vecLength, vec1, vec2, vec3) private(i)
	for (i = 0; i < vecLength; i++)
		vec3[i] = vec1[i] + vec2[i];
#endif
	double endTime = omp_get_wtime();
	//print_vec(vec3, vecLength);
	printf("openMP test completed in: %lf\n", endTime - startTime);

	return 0;
}