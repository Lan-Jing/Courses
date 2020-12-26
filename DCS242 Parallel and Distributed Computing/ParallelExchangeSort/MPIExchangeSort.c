#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void print_vec(int* vec, int l, int myRank) {
	for (int i = 0; i < l; i++) {
		printf("(%d,%d)", myRank, vec[i]);
		fflush(stdout);
	}
	printf("\n");
}

int cmp(const int* a, const int* b) {
	return *a - * b; // Be careful on c quicksort. When return 0, the function may not swap items.
}

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void naive_sort(int* vec, int l) {
	int* tmpVec = (int*)malloc(l * sizeof(int));
	memcpy(tmpVec, vec, l * sizeof(int));
	
	double startTime = MPI_Wtime();
	qsort(tmpVec, l, sizeof(int), cmp); // quick sort.
	double endTime = MPI_Wtime();
	printf("Base test ends in: %lf\n", endTime - startTime); fflush(stdout);

//	print_vec(tmpVec, l, 0);
	free(tmpVec);
}

void merge_sort(int* vec1, int* vec2, int l1, int l2, int mode) {
	int* tmpVec = (int*)malloc(l1 * sizeof(int));
	int p1, p2, p;
	if (mode) {
		p = p1 = l1 - 1, p2 = l2 - 1;
		while (p >= 0) {
			if (p1 >= 0 && p2 >= 0)
				tmpVec[p--] = vec1[p1] > vec2[p2] ? vec1[p1--] : vec2[p2--];
			else tmpVec[p--] = p1 < 0 ? vec2[p2--] : vec1[p1--];
		}
	} else {
		p = p1 = p2 = 0;
		while (p < l1) {
			if (p1 < l1 && p2 < l2)
				tmpVec[p++] = vec1[p1] < vec2[p2] ? vec1[p1++] : vec2[p2++];
			else tmpVec[p++] = p1 >= l1 ? vec2[p2++] : vec1[p1++];
		}
	}
	memcpy(vec1, tmpVec, l1 * sizeof(int));
	free(tmpVec);
} // a slightly modified merge_sort, supporting both min/max grouping.

int main() {
	MPI_Init(NULL, NULL);

	int myRank, commSize;
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int vecLength, localLength;
	int* vec, * sendCount, * displace;
	sendCount = (int*)malloc(commSize * sizeof(int));
	displace = (int*)malloc(commSize * sizeof(int));

	double startTime, endTime;

	if (!myRank) {
		printf("Enter length of the unsorted vector:");
		fflush(stdout);
		scanf("%d", &vecLength);

		vec = (int*)malloc(vecLength * sizeof(int));
		srand((unsigned int)time(NULL));
		for (int i = 0; i < vecLength; i++)
			vec[i] = rand();
//		print_vec(vec, vecLength, 0);

		naive_sort(vec, vecLength); // run qsort, recording run time.
		
		startTime = MPI_Wtime(); // here begins the parallel sort.

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
		MPI_Barrier(MPI_COMM_WORLD);
		int target, * targetVec, targetLength;
		if (myRank % 2) {
			target = round % 2 ? myRank + 1 : myRank - 1;
		} else {
			target = round % 2 ? myRank - 1 : myRank + 1;
		}
		if (target < 0 || target == commSize) target = MPI_PROC_NULL, targetLength = 0;
		MPI_Sendrecv(&localLength, 1, MPI_INT, target, 0, &targetLength, 1, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		targetVec = (int*)malloc(targetLength * sizeof(int));
		MPI_Sendrecv(vec, localLength, MPI_INT, target, 0, targetVec, targetLength, MPI_INT, target, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		MPI_Barrier(MPI_COMM_WORLD);
		if (targetLength) merge_sort(vec, targetVec, localLength, targetLength, myRank > target);
		free(targetVec);
	} // Use MPI_SendRecv() to prevent deadlock.

	if (!myRank) {
		MPI_Gatherv(MPI_IN_PLACE, localLength, MPI_INT, vec, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD); // get synchronized before end time is recorded.
		
		endTime = MPI_Wtime();
		printf("MPI parallel sort ends in: %lf\n",endTime-startTime); fflush(stdout);
//		print_vec(vec, vecLength, 0);

		free(vec);
	} else {
		MPI_Gatherv(vec, localLength, MPI_INT, vec, sendCount, displace, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}