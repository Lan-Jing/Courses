#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <immintrin.h>

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
	scanf_s("%d", &vecLength);

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
	print_vec(vec1, vecLength), print_vec(vec2, vecLength);

	double startTime = omp_get_wtime();
	int i;
#ifdef _OPENMP
#pragma omp parallel for num_threads(threadNum) \
	default(none) shared(vecLength, vec1, vec2, vec3) private(i)
	for (i = 0; i < vecLength / 8; i++) {
		__m256i intVec1 = _mm256_load_si256((__m256i*)(vec1 + i * 8));
		__m256i intVec2 = _mm256_load_si256((__m256i*)(vec2 + i * 8));

		__m256i intVec3 = _mm256_add_epi32(intVec1, intVec2);

		_mm256_store_si256(vec3 + i * 8, intVec3);
	}
#endif
	for (i = (vecLength / 8) * 8; i < vecLength; i++)
		vec3[i] = vec1[i] + vec2[i];
	double endTime = omp_get_wtime();
	print_vec(vec3, vecLength);
	printf("openMP+AVX test completed in: %lf\n", endTime - startTime);

	return 0;
}