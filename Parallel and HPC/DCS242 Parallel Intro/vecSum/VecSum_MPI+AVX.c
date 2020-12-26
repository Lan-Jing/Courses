#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include <immintrin.h>

void vecSum_AVX(int* vec1, int* vec2, int* vec3, int l) {
	for (int i = 0; i < l / 8; i++) {
		__m256i piece1 = _mm256_load_si256((const __m256i*)(vec1 + i * 8));
		__m256i piece2 = _mm256_load_si256((const __m256i*)(vec2 + i * 8));

		__m256i pieceSum = _mm256_add_epi32(piece1, piece2);

		_mm256_store_si256((__m256i*)(vec3 + i * 8), pieceSum);
	}
	for (int i = (l / 8) * 8; i < l; i++)
		vec3[i] = vec1[i] + vec2[i];
}

void print_vec(int* vec, int l) {
	for (int i = 0; i < l; i++)
		printf("%d ", vec[i]), fflush(stdout);
	printf("\n"); fflush(stdout);
}

int main() {
	MPI_Init(NULL, NULL);

	int myRank, commSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	int localLength, vecLength;
	int* vec1, * vec2, * vec3;
	int* sendCount = (int*)malloc(commSize * sizeof(int));
	int* displace = (int*)malloc(commSize * sizeof(int));

	if (!myRank) {
		printf("Enter length of the summed vector:"); fflush(stdout);
		scanf_s("%d", &vecLength);

		vec1 = (int*)malloc(vecLength * sizeof(int));
		vec2 = (int*)malloc(vecLength * sizeof(int));
		vec3 = (int*)malloc(vecLength * sizeof(int));
		if (vec1 == NULL || vec2 == NULL || vec3 == NULL) {
			printf("Memory allocation failed at process %d\n", myRank); fflush(stdout);
			free(vec1); free(vec2); free(vec3);
			return 0;
		}

		srand((unsigned int)time(NULL));
		for (int i = 0; i < vecLength; i++)
			vec1[i] = rand(), vec2[i] = rand();
		print_vec(vec1, vecLength); print_vec(vec2, vecLength);

		for (int i = 0; i < commSize; i++)
			sendCount[i] = vecLength / commSize + (i < vecLength % commSize),
			displace[i] = i ? displace[i - 1] + sendCount[i - 1] : 0;

		MPI_Scatter(sendCount, 1, MPI_INT, &localLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Scatterv(vec1, sendCount, displace, MPI_INT, MPI_IN_PLACE, localLength, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec2, sendCount, displace, MPI_INT, MPI_IN_PLACE, localLength, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Scatter(NULL, 1, MPI_INT, &localLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		vec1 = (int*)malloc(localLength * sizeof(int));
		vec2 = (int*)malloc(localLength * sizeof(int));
		vec3 = (int*)malloc(localLength * sizeof(int));
		if (vec1 == NULL || vec2 == NULL || vec3 == NULL) {
			printf("Memory allocation failed at process %d\n", myRank); fflush(stdout);
			free(vec1); free(vec2); free(vec3);
			return 0;
		}

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Scatterv(NULL, NULL, NULL, MPI_INT, vec1, localLength, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(NULL, NULL, NULL, MPI_INT, vec2, localLength, MPI_INT, 0, MPI_COMM_WORLD);
	}

	vecSum_AVX(vec1, vec2, vec3, localLength);

	if (!myRank) {
		MPI_Gatherv(MPI_IN_PLACE, localLength, MPI_INT, vec3, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);

		print_vec(vec3, vecLength);
	} else {
		MPI_Gatherv(vec3, localLength, MPI_INT, vec3, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	free(vec1);
	free(vec2);
	free(vec3);
	return 0;
}