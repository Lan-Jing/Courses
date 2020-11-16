#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define precision 10

struct func {
	double a, b, c;
};

double f(struct func* func, double x) {
	return func->a * x * x + func->b * x + func->c;
}

double integral(struct func* func, double lb, double rb) {
	int myRank = 0, threadCount = 1;
#ifdef _OPENMP // use marco to tackle availble openMP.
	myRank = omp_get_thread_num();
	threadCount = omp_get_num_threads();
#endif
	double subSegment = (rb - lb) / threadCount, unit = subSegment / (1 << precision);
	double myLeftBound = lb + myRank * subSegment;

	double localRes = 0;
	for (int i = 0; i < (1 << precision); i++) 
		localRes += (f(func, myLeftBound + i * unit) + f(func, myLeftBound + (i + 1) * unit)) * unit / 2;
	return localRes;
}

int main(int argc, char* argv[]) {
	int threadCount = 1;
#ifdef _OPENMP
	threadCount = strtol(argv[1], NULL, 10);
#endif

	double leftBound, rightBound;
	struct func param;
	printf("Enter three parameters of the two-order polynomial function:");
	scanf_s("%lf %lf %lf", &param.a, &param.b, &param.c);
	printf("Enter left and right boundary of the interval for intergal:");
	scanf_s("%lf %lf", &leftBound, &rightBound);

	double res = 0;

#ifdef _OPENMP
#pragma omp parallel num_threads(threadCount) reduction(+: res)
#endif
	res += integral(&param, leftBound, rightBound);

	printf("The integral of the given segment is: %lf\n", res);
	return 0;
}