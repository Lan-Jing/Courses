#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>
#include <immintrin.h>

void print_vec(int* vec, int l) {
	for (int i = 0; i < l; i++)
		printf("%d ", vec[i]), fflush(stdout);
	printf("\n"); fflush(stdout);
}

void sum_AVX(int* vec1, int* vec2, int* vec3, int l) {
	for (int i = 0; i < l / 8; i++) {
		__m256i piece1 = _mm256_load_si256((const __m256i*)(vec1 + i * 8));
		__m256i piece2 = _mm256_load_si256((const __m256i*)(vec2 + i * 8));

		__m256i pieceSum = _mm256_add_epi32(piece1, piece2);

		_mm256_store_si256((__m256i*)(vec3 + i * 8), pieceSum);
	}
	for (int i = (l / 8) * 8; i < l; i++)
		vec3[i] = vec1[i] + vec2[i];
}

void sum_base(int* vec1, int* vec2, int* vec3, int l) {
	for (int i = 0; i < l; i++)
		vec3[i] = vec1[i] + vec2[i];
}


void vecSum_base(int* vec1_p, int* vec2_p, int* vec3_p, int l) {
	double startTime = MPI_Wtime();
	sum_base(vec1_p, vec2_p, vec3_p, l);
	double endTime = MPI_Wtime();
	printf("Base test completed in %lf\n", endTime - startTime); fflush(stdout);
	//print_vec(vec3_p, l);
	return;
}

void vecSum_AVX(int* vec1_p, int* vec2_p, int* vec3_p, int l) {
	double startTime = MPI_Wtime();
	sum_AVX(vec1_p, vec2_p, vec3_p, l);
	double endTime = MPI_Wtime();
	printf("AVX test completed in %lf\n", endTime - startTime); fflush(stdout);
	//print_vec(vec3_p, l);
	return;
}

void vecSum_MPI(int* vec1, int* vec2, int* vec3, int l, void (*sum_p) (int*, int*, int*, int)) {
	int myRank, commSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	int* sendCount = (int*)malloc(commSize * sizeof(int));
	int* displace = (int*)malloc(commSize * sizeof(int));
	int localCount; // location information for each vector.
	int* vec1_p, * vec2_p, * vec3_p;
	if (!myRank) {
		vec1_p = vec1;
		vec2_p = vec2;
		vec3_p = vec3; // the master process get vector from function parameters.

		for (int i = 0; i < commSize; i++)
			sendCount[i] = l / commSize + (i < l % commSize);
		for (int i = 0; i < commSize; i++)
			displace[i] = i ? displace[i - 1] + sendCount[i - 1] : 0;
		// set parameters for scattering information.
		MPI_Scatter(sendCount, 1, MPI_INT, &localCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
		// distribute local length to all members.
	} else {
		MPI_Scatter(sendCount, 1, MPI_INT, &localCount, 1, MPI_INT, 0, MPI_COMM_WORLD);
		vec1_p = (int*)malloc(localCount * sizeof(int));
		vec2_p = (int*)malloc(localCount * sizeof(int));
		vec3_p = (int*)malloc(localCount * sizeof(int));
		if (vec1_p == NULL || vec2_p == NULL || vec3_p == NULL) {
			printf("Memory allocation failed.\n");
			fflush(stdout);
			return;
		} // allocate memory for each vector.
	}

	if (!myRank) {
		MPI_Scatterv(vec1_p, sendCount, displace, MPI_INT, MPI_IN_PLACE, localCount, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec2_p, sendCount, displace, MPI_INT, MPI_IN_PLACE, localCount, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Scatterv(vec1_p, sendCount, displace, MPI_INT, vec1_p, localCount, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec2_p, sendCount, displace, MPI_INT, vec2_p, localCount, MPI_INT, 0, MPI_COMM_WORLD);
	} // distribute two source vectors to other processes.

	sum_p(vec1_p, vec2_p, vec3_p, localCount);
	// add up part of vectors for every process.

	if (!myRank) {
		MPI_Gatherv(MPI_IN_PLACE, localCount, MPI_INT, vec3_p, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);

		//  print_vec(vec3_p, l);
	} else {
		MPI_Gatherv(vec3_p, localCount, MPI_INT, vec3_p, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
	} // collect sub-vectors from all processes.

	free(sendCount);
	free(displace);

	return;
}

void vecSum_openMP(int* vec1, int* vec2, int* vec3, int l, void (*sum_p) (int*, int*, int*, int)) {
	int threadNum = 1;
#ifdef _OPENMP
	threadNum = 8;
#endif

	double startTime = omp_get_wtime();
	int i;
	if (sum_p == sum_base) {
#pragma omp parallel for num_threads(threadNum) \
		default(none) shared(vec1, vec2, vec3, l) private(i)
		for (i = 0; i < l; i++)
			vec3[i] = vec1[i] + vec2[i];
	} else {
#pragma omp parallel for num_threads(threadNum) \
		default(none) shared(vec1, vec2, vec3, l) private(i)
		for (i = 0; i < l / 8; i++) {
			__m256i piece1 = _mm256_load_si256((const __m256i*)(vec1 + i * 8));
			__m256i piece2 = _mm256_load_si256((const __m256i*)(vec2 + i * 8));

			__m256i pieceSum = _mm256_add_epi32(piece1, piece2);

			_mm256_store_si256((__m256i*)(vec3 + i * 8), pieceSum);
		}
#pragma omp parallel for num_threads(threadNum) \
		default(none) shared(vec1, vec2, vec3, l) private(i)
		for (i = (l / 8) * 8; i < l; i++)
			vec3[i] = vec1[i] + vec2[i];
	}
	double endTime = omp_get_wtime();
	//print_vec(vec3, l);
	if (sum_p == sum_base)
		printf("openMP test completed in: %lf\n", endTime - startTime);
	else
		printf("openMP+AVX test completed in: %lf\n", endTime - startTime);
	return 0;
}

int main() {
	MPI_Init(NULL, NULL);

	int myRank, commSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	int* vec1_p, * vec2_p, * vec3_p;
	int vecLength; // location information for each vector.
	double startTime, endTime;
	if (!myRank) {
		printf("Input length of the summed vectors:"); fflush(stdout);
		scanf("%d", &vecLength); // enter total length of the vectors.

		vec1_p = (int*)malloc(vecLength * sizeof(int));
		vec2_p = (int*)malloc(vecLength * sizeof(int));
		vec3_p = (int*)malloc(vecLength * sizeof(int));
		if (vec1_p == NULL || vec2_p == NULL || vec3_p == NULL) {
			printf("Meomry allocation failed.\n"); fflush(stdout);
			MPI_Finalize();
			return 0;
		} // allocate memory for each vector.

		srand((unsigned int)time(NULL));
		for (int i = 0; i < vecLength; i++) {
			vec1_p[i] = rand();
			vec2_p[i] = rand();
		} // get random numbers in the vectors.

	//	print_vec(vec1_p, vecLength); print_vec(vec2_p, vecLength);

		vecSum_base(vec1_p, vec2_p, vec3_p, vecLength);
		vecSum_AVX(vec1_p, vec2_p, vec3_p, vecLength);
		vecSum_openMP(vec1_p, vec2_p, vec3_p, vecLength, sum_base);
		vecSum_openMP(vec1_p, vec2_p, vec3_p, vecLength, sum_AVX);
		// let process 0 complete these single-process implementation.

		MPI_Barrier(MPI_COMM_WORLD);
		startTime = MPI_Wtime();
		vecSum_MPI(vec1_p, vec2_p, vec3_p, vecLength, sum_base); // test using MPI multiprocessing.
		endTime = MPI_Wtime();
		printf("MPI test completed in: %lf\n", endTime - startTime); fflush(stdout);

		MPI_Barrier(MPI_COMM_WORLD);
		startTime = MPI_Wtime();
		vecSum_MPI(vec1_p, vec2_p, vec3_p, vecLength, sum_AVX); // test using MPI as well as AVX parallelism.
		endTime = MPI_Wtime();
		printf("MPI+AVX test completed in: %lf\n", endTime - startTime); fflush(stdout);

		free(vec1_p);
		free(vec2_p);
		free(vec3_p);
	} else {
		MPI_Barrier(MPI_COMM_WORLD);
		vecSum_MPI(NULL, NULL, NULL, 0, sum_base); // slave routine. Only receive parameters from master.

		MPI_Barrier(MPI_COMM_WORLD);
		vecSum_MPI(NULL, NULL, NULL, 0, sum_AVX);
	}

	MPI_Finalize();

	return 0;
}