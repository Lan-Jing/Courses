#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void print_vec(int* vec, int l) {
	for (int i = 0; i < l; i++) {
		printf("%d ", vec[i]);
		fflush(stdout);
	}
	printf("\n");
}

int cmp(const int* a, const int* b) {
	return *a - * b;
}

void merge_sort(int* vec1, int* vec2, int l1, int l2, int mode) {
	int* tmpVec = (int*)malloc(l1 * sizeof(int));
	int p1, p2, p;
	if (mode) p1 = l1 - 1, p2 = l2 - 1, p = l1 - 1;
	else p1 = p2 = p = 0;
	while (0 <= p1 && p1 < l1 && 0 <= p2 && p2 < l2 && 0 <= p && p < l1) {
		tmpVec[p] = mode ? max(vec1[p1], vec2[p2]) : min(vec1[p1], vec2[p2]);
		if (!mode) {
			if (vec1[p1] < vec2[p2]) p1++;
			else p2++;
			p++;
		} else {
			if (vec1[p1] < vec2[p2]) p2--;
			else p1--;
			p--;
		}
	}

	if (0 <= p && p < l1) tmpVec[p] = vec1[p1];
	for (int i = 0; i < l1; i++) vec1[i] = tmpVec[i];
	free(tmpVec);
}

int main() {
	MPI_Init(NULL, NULL);

	int myRank, commSize;
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int vecLength, localLength;
	int* vec, * sendCount, * displace;
	sendCount = (int*)malloc(commSize * sizeof(int));
	displace = (int*)malloc(commSize * sizeof(int));
	if (!myRank) {
		printf("Enter length of the unsorted vector:");
		fflush(stdout);
		scanf("%d", &vecLength);

		vec = (int*)malloc(vecLength * sizeof(int));
		srand((unsigned int)time(NULL));
		for (int i = 0; i < vecLength; i++)
			vec[i] = rand();
		print_vec(vec, vecLength);

		for (int i = 0; i < commSize; i++)
			sendCount[i] = vecLength / commSize + (i < vecLength % commSize);
		for (int i = 0; i < commSize; i++)
			displace[i] = i ? displace[i - 1] + sendCount[i - 1] : 0;

		MPI_Scatter(sendCount, 1, MPI_INT, &localLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Scatterv(vec, sendCount, displace, MPI_INT, MPI_IN_PLACE, localLength, MPI_INT, 0, MPI_COMM_WORLD);
	} else {
		MPI_Scatter(sendCount, 1, MPI_INT, &localLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		vec = (int*)malloc(localLength * sizeof(int));
		MPI_Scatterv(vec, sendCount, displace, MPI_INT, vec, localLength, MPI_INT, 0, MPI_COMM_WORLD);
	}

	qsort(vec, localLength, sizeof(int), cmp);

	for (int round = 0; round < commSize; round++) { // n rounds of exchange_sort will be enough.
		int target, * targetVec, targetLength;
		if (myRank % 2) {
			target = round % 2 ? myRank + 1 : myRank - 1;
			if (target < 0 || target == commSize) target = MPI_PROC_NULL, targetLength = 0;
			MPI_Recv(&targetLength, 1, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(&localLength, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
			targetVec = (int*)malloc(targetLength * sizeof(int));
			MPI_Recv(targetVec, targetLength, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Send(vec, localLength, MPI_INT, target, 0, MPI_COMM_WORLD);
		} else {
			target = round % 2 ? myRank - 1 : myRank + 1;
			if (target < 0 || target == commSize) target = MPI_PROC_NULL, targetLength = 0;
			MPI_Send(&localLength, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
			MPI_Recv(&targetLength, 1, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			targetVec = (int*)malloc(targetLength * sizeof(int));
			MPI_Send(vec, localLength, MPI_INT, target, 0, MPI_COMM_WORLD);
			MPI_Recv(targetVec, targetLength, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		if (targetLength) merge_sort(vec, targetVec, localLength, targetLength, (round + myRank) % 2);
		free(targetVec);
	} // use of MPI_Send and MPI_Recv will be unsafe. Result becomes unstable when vector scales up.

	if (!myRank) {
		MPI_Gatherv(MPI_IN_PLACE, localLength, MPI_INT, vec, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
		print_vec(vec, vecLength);

		free(vec);
	} else {
		MPI_Gatherv(vec, localLength, MPI_INT, vec, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}