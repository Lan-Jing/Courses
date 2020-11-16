#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

/*
		Input a polynomial function and a inteval.
		Output integral to a given precision.

		Use MPI set communication methods.
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
		scanf("%lf%lf%lf", &Int.a, &Int.b, &Int.c);
		printf("Enter boundary of the segment:\n");
		fflush(stdout);
		scanf("%lf%lf", &Int.leftBound, &Int.rightBound); // get global parameters.

		MPI_Bcast(&Int, 5, MPI_DOUBLE, 0, MPI_COMM_WORLD); // use set communication function for sending parameters.
	} else {
		MPI_Bcast(&Int, 5, MPI_DOUBLE, 0, MPI_COMM_WORLD); // do not use point2point comm API to respond set-type comm.
	}

	double subSegment = (Int.rightBound - Int.leftBound) / commSize;
	double myLeftBound = Int.leftBound + subSegment * myRank;
	double myRightBound = myLeftBound + subSegment; // compute subsegment for each process.

	double myRes = localCompute(&Int, myLeftBound, myRightBound, sepCount); // compute local result.

	if (!myRank) {
		double gloRes = 0;
		MPI_Reduce(&myRes, &gloRes, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

		printf("The integral of the given interval is: %lf\n", gloRes);
	} else {
		MPI_Reduce(&myRes, NULL, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();  // terminate MPI
	return 0;
}