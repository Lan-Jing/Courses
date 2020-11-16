#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

/*
	Use instrcution-based parallelism to improve vertor summation.
*/

int main() {
	srand(time(NULL));

	int vecLength;
	printf("Input length of the summed vectors:");
	scanf_s("%d", &vecLength);

	int* vec1_p = (int*)malloc(vecLength * sizeof(int));
	int* vec2_p = (int*)malloc(vecLength * sizeof(int));
	int* vec3_p = (int*)malloc(vecLength * sizeof(int));
	if (vec1_p == NULL || vec2_p == NULL || vec3_p == NULL) {
		printf("Memory allocation failed.\n");
		return 0;
	}
	for (int i = 0; i < vecLength; i++) {
		vec1_p[i] = rand();
		vec2_p[i] = rand();
	} // get all elements in the vectors.

	clock_t startTime = clock();
	for (int i = 0; i < vecLength; i++) {
		vec3_p[i] = vec1_p[i] + vec2_p[i];
	}
	clock_t endTime = clock();
	double duration = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	printf("Completed in %lf\n", duration);

	free(vec1_p);
	free(vec2_p);
	free(vec3_p);
	return 0;
}