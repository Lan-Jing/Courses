#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

/*
		Input a polynomial function and a inteval.
		Output Integral to a given precision.

		Use point2point to develop a set-type comm.
*/

#define precision 10

struct integral {
	double a, b, c;
	double leftBound, rightBound;
};
double f(struct integral* data, double x) {
	return data->a * x * x + data->b * x + data->c;
}

double localCompute(struct integral* data, double myLeftBound, double myRightBound, int sepCount) {
	double ans = 0;
	double unit = (myRightBound - myLeftBound) / sepCount;
	for (int i = 0; i < sepCount; i++)
		ans += (f(data, myLeftBound + i * unit) + f(data, myLeftBound + (i + 1) * unit)) * unit / 2;
	return ans;
}

void recRecv(double* myRes, int nowRank, int totLevel) {
	int nowLevel = 0;
	double secRes = 0;
	while (nowLevel <= totLevel && ((1 << nowLevel) & nowRank) == 0) {
		MPI_Recv(&secRes, 1, MPI_DOUBLE, nowRank + (1 << nowLevel), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		*myRes += secRes;
		nowLevel++;
	}
	if (nowLevel <= totLevel)
		MPI_Send(myRes, 1, MPI_DOUBLE, nowRank - (1 << nowLevel), 0, MPI_COMM_WORLD);
	return;
}

int main() {
	struct integral Int; // global parameters for integral.

	int myRank, commSize; // local parameters.
	int sepCount = pow(2, precision); // determine number of units a process will compute.

	MPI_Init(NULL, NULL);  // start up MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &commSize);

	if (!myRank) {
		printf("Specify a two order function by entering three parameters:\n");
		fflush(stdout);
		scanf("%lf %lf %lf", &Int.a, &Int.b, &Int.c);
		printf("Enter boundary of the segment:\n");
		fflush(stdout);
		scanf("%lf %lf", &Int.leftBound, &Int.rightBound); // get global parameters.

		for (int i = 1; i < commSize; i++)
			MPI_Send(&Int, 5, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
	} else {
		MPI_Recv(&Int, 5, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	double subSegment = (Int.rightBound - Int.leftBound) / commSize;
	double myLeftBound = Int.leftBound + subSegment * myRank;
	double myRightBound = myLeftBound + subSegment; // compute subsegment for each process.

	double myRes = localCompute(&Int, myLeftBound, myRightBound, sepCount); // compute local result.

	recRecv(&myRes, myRank, (int)log2(commSize) - 1);

	if (!myRank) {
		printf("The integral of the given segment is:%lf\n", myRes);
		fflush(stdout);
	}

	MPI_Finalize();  // terminate MPI
	return 0;
}