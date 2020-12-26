#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <time.h>

/*
	Try to use MPI to implement a concurrent vector summation.

	Use Bcast to broadcast data to every nodes in the group.
	Use Scatter and Gather to distribute and collect data to/from the group.
*/

void printVec(int* vec, int l) {
	for (int i = 0; i < l; i++) {
		printf("%d ", vec[i]);
		fflush(stdout);
	}
	printf("\n");
}

int main() {
	MPI_Init(NULL, NULL);

	int myRank, commSize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	int* vec1_p, * vec2_p, * vec3_p;
	int* sendCount = (int*)malloc(commSize * sizeof(int));
	int* displace = (int*)malloc(commSize * sizeof(int));
	int vecLength, localCount; // location information for each vector.
	if (!myRank) {
		printf("Enter the length of the two vectors:");
		fflush(stdout);
		scanf("%d", &vecLength); // enter total length of the vectors.

		vec1_p = (int*)malloc(vecLength * sizeof(int));
		vec2_p = (int*)malloc(vecLength * sizeof(int));
		vec3_p = (int*)malloc(vecLength * sizeof(int));
		if (vec1_p == NULL || vec2_p == NULL || vec3_p == NULL) {
			printf("Meomry allocation failed.\n");
			MPI_Finalize();
			return 0;
		} // allocate memory for each vector.

		srand((unsigned int)time(NULL));
		for (int i = 0; i < vecLength; i++) {
			vec1_p[i] = rand();
			vec2_p[i] = rand();
		} // get random numbers in the vectors.

		for (int i = 0; i < commSize; i++)
			sendCount[i] = vecLength / commSize + (i < vecLength % commSize);
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
			return 0;
		} // allocate memory for each vector.
	}

	double startTime = MPI_Wtime();
	if (!myRank) {
		MPI_Scatterv(vec1_p, sendCount, displace, MPI_INT, MPI_IN_PLACE, localCount, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec2_p, sendCount, displace, MPI_INT, MPI_IN_PLACE, localCount, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Scatterv(vec1_p, sendCount, displace, MPI_INT, vec1_p, localCount, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec2_p, sendCount, displace, MPI_INT, vec2_p, localCount, MPI_INT, 0, MPI_COMM_WORLD);
	}

	for (int i = 0; i < localCount; i++)
		vec3_p[i] = vec1_p[i] + vec2_p[i];
	// add up part of vectors for every process.

	if (!myRank) {
		MPI_Gatherv(MPI_IN_PLACE, localCount, MPI_INT, vec3_p, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Gatherv(vec3_p, localCount, MPI_INT, vec3_p, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
	}
	double endTime = MPI_Wtime();

	if (!myRank) {
	/*  printVec(vec1_p, vecLength);
		printVec(vec2_p, vecLength);
		printVec(vec3_p, vecLength); */

		printf("Completed in %lf\n", endTime - startTime);
	}

	free(vec1_p);
	free(vec2_p);
	free(vec3_p);
	free(sendCount);
	free(displace);

	MPI_Finalize();
	return 0;
}