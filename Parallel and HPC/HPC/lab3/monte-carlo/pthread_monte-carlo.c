#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define rightBound 1

int testRound = 1e8, threadCount = 0;

double area = 0;
pthread_mutex_t area_mutex;

inline float float_rand(float r, int *seed)
{
    return (float)((double)rand_r(seed)/(double)(RAND_MAX/r));
}

inline float isIn(float x, float y) 
{
    return y <= x*x;
}

void* check(void* rank) 
{
    int myRank = *((int*)rank), subTestRound = testRound / threadCount + (myRank < testRound % threadCount),
        mySeed = clock() + myRank;
    double x, y, acc = 0;
    for(int i = 0;i < subTestRound;i++) {
        x = float_rand(rightBound, &mySeed),
        y = float_rand(rightBound, &mySeed); 
        acc += isIn(x, y);
    }
    pthread_mutex_lock(&area_mutex);
    area += acc;
    pthread_mutex_unlock(&area_mutex);

    return NULL;
}

int main(int argc, char* argv[])
{
    threadCount = strtol(argv[1], NULL, 10);

    pthread_t* pVec = (pthread_t*)malloc(sizeof(pthread_t) * threadCount);
    int* rankVec = (int*)malloc(sizeof(int) * threadCount);

    pthread_mutex_init(&area_mutex, NULL);
    for(int i = 0;i < threadCount;i++) {
        // have to add this vector. Otherwise myRank could change before a thread uses it.
        rankVec[i] = i;
        pthread_create(&pVec[i], NULL, check, &rankVec[i]);
    }
    for(int i = 0;i < threadCount;i++) {
        pthread_join(pVec[i], NULL);
    }
    pthread_mutex_destroy(&area_mutex);

    area *= (double)(rightBound * rightBound) / (double)testRound;
    printf("Area under f(x)=x^2 between [0, 1] is: %lf\n", area);

    free(pVec);
    free(rankVec);
    return 0;
}